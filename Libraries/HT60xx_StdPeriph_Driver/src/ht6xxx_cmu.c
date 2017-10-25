/*
*********************************************************************************************************
*                                              HT6XXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : HT6xxx_cmu.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_CMU_C

#include "ht6xxx_cmu.h"

/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             本地变量
*********************************************************************************************************
*/

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};

/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/

static void Wait2mS(void);

/*
*********************************************************************************************************
*                                         INITIAL CMU MODULE
*
* 函数说明: 初始化时钟配置模块
*
* 入口参数: CMU_InitStruct     CMU模块初始化结构体指针，主要包含3个参数: 
*                              1) SysClkSel   : 系统时钟（Fsys）源选择，可为以下一个参数：
*                                               a) SysFLRC    LRC--->Fsys
*                                               b) SysFLF     LF---->Fsys
*                                               c) SysFHRC    HRC--->Fsys
*                                               d) SysFPLL    PLL--->Fsys
*                                               e) SysFMEMS   Mems-->Fsys                                               
*                              2) HRCDiv      : 高频RC分频，如果 SysClkSel = FHRC，则Fsys = HRC/HRCDiv
*                              3) CPUDiv      : CPU时钟分频设置  Fcpu = Fsys/CPUDiv                            
*
* 返回参数: 无                                      
* 
* 特殊说明: 用户应保证此函数执行过程中寄存器写保护状态不被修改
*********************************************************************************************************
*/
void HT_CMU_Init(CMU_InitTypeDef* CMU_InitStruct)
{
    /*  assert_param  */
      
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
#if  defined  HT501x  || HT6x2x  ||  HT502x   
        if((CMU_InitStruct->SysClkSel & CMU_SYSCLKDIV_SEL40M)==0)          /*!< 选择PLL时钟20M         */ 
        {
            CMU_InitStruct->CPUDiv &= ~CMU_SYSCLKDIV_SEL40M;
        }
        else                                                               /*!< 否则选择PLL时钟X2 40M  */
        {
            CMU_InitStruct->CPUDiv |= CMU_SYSCLKDIV_SEL40M;
        }
#endif	
		HT_CMU->SYSCLKDIV = CMU_InitStruct->CPUDiv;                            /*!< CPU时钟分频设置        */
    
    if((CMU_InitStruct->SysClkSel & CMU_SYSCLKCFG_CLKSEL) == SysPLL)       /*!< Fpll ---> Fsys         */
    {
        HT_CMU->CLKCTRL0 |= CMU_CLKCTRL0_PLLEN;                            /*!< PLL使能                */
        Wait2mS();                                                         /*!< 等待2ms                */
        HT_CMU->SYSCLKCFG = (CMU_SYSCLKCFG_CLKSEL_PLL | CMU_SYSCLKCFG_WCLKEN);
                                                                           /*!< 配置系统为PLL时钟      */  
    }
    else if((CMU_InitStruct->SysClkSel & CMU_SYSCLKCFG_CLKSEL) == CMU_SYSCLKCFG_CLKSEL_HRC)                       
    {                                                                      /*!< Fhrc ---> Fsys         */
        HT_CMU->CLKCTRL0 |= CMU_CLKCTRL0_HRCEN;                            /*!< HRC使能                */
        HT_CMU->HRCDIV    = CMU_InitStruct->SysClkSel>>4;                  /*!< HRC时钟分频设置        */  
        Wait2mS();                                                         /*!< 等待2ms                */
        HT_CMU->SYSCLKCFG = (CMU_SYSCLKCFG_CLKSEL_HRC | CMU_SYSCLKCFG_WCLKEN);   
                                                                           /*!< 配置系统为HRC时钟      */      
    }
    else                                                                   /*!< Fsys = FLrc/FLf/Fmems  */
    {
        HT_CMU->SYSCLKCFG = (CMU_InitStruct->SysClkSel | CMU_SYSCLKCFG_WCLKEN);   
                                                                           /*!< 配置系统为设定时钟     */
    }
      
    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
} 

/*
*********************************************************************************************************
*                                     GET FSYS CLOCK FREQUENCY
*
* 函数说明: 获取系统时钟频率（即Fsys，非Fcpu），返回以Hz为单位的频率值
*
* 入口参数: 无                         
*
* 返回参数: sysclk   系统时钟（Fsys）频率，32位无符号数，以Hz为单位                                   
* 
* 特殊说明: 如果系统时钟源是高频RC或者低频RC，返回的时钟频率将不会完全准确，对高频RC，会有2%左右的误差，
*           对低频RC，会有超过10%的误差。
*********************************************************************************************************
*/
uint32_t HT_CMU_SysClkGet(void)
{
    /*  assert_param  */
      
    uint32_t sysclk, clksel;       
    
    clksel = HT_CMU->SYSCLKCFG & CMU_SYSCLKCFG_CLKSEL;
    
    switch(clksel)
    {
        case CMU_SYSCLKCFG_CLKSEL_LRC:               /*!< 系统时钟源为LRC     */
        case CMU_SYSCLKCFG_CLKSEL_LF:                /*!< 系统时钟源为LF      */
             sysclk = 32768;
        break;
        
        case CMU_SYSCLKCFG_CLKSEL_HRC:               /*!< 系统时钟源为HRC     */
#if defined  HT6x1x
				sysclk = 8000000>>(HT_CMU->HRCDIV & CMU_HRCDIV );
#elif defined  HT6x2x
				sysclk = 11010048>>(HT_CMU->HRCDIV & CMU_HRCDIV );
#elif defined  HT501x
				sysclk = 14000000;
#elif defined  HT502x
				sysclk = 8800000;
#endif
        break;
        
        case CMU_SYSCLKCFG_CLKSEL_PLL:               /*!< 系统时钟源为PLL     */
#if  defined  HT501x || HT502x
             if((HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV_SEL40M))
             {
                 sysclk =  32768*600*2;  
             }
             else
             {
                 sysclk =  32768*600;
             }
#elif defined HT6x2x
             if((HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV_SEL40M))
             {
                 sysclk =  32768*672*2;  
             }
             else
             {
                 sysclk =  32768*672;
             }
#else        
             sysclk = 32768*672;
#endif        
        break;
        
        default:                                     /*!< 系统时钟源为Mems    */
             sysclk = 524288;                        /*!< HT501x 不适用       */
        break;
    }
    
    return sysclk;                                   /*!< 返回系统时钟频率    */
} 

/*
*********************************************************************************************************
*                                     GET CPU CLOCK FREQUENCY
*
* 函数说明: 获取CPU时钟频率，返回以Hz为单位的频率值
*
* 入口参数: 无                         
*
* 返回参数: cpuclk   CPU时钟（Fcpu）频率，32位无符号数，以Hz为单位                                   
* 
* 特殊说明: 如果系统时钟源是高频RC或者低频RC，返回的时钟频率将不会完全准确，对高频RC，会有2%左右的误差，
*           对低频RC，会有超过10%的误差。
*********************************************************************************************************
*/
uint32_t HT_CMU_CPUClkGet(void)
{
    /*  assert_param  */
      
    uint32_t cpuclk, clksel, cpuclkdiv;       
    
    clksel = HT_CMU->SYSCLKCFG & CMU_SYSCLKCFG_CLKSEL;
	  cpuclkdiv = HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV;          /*!< 获取CPU时钟分频    */
    
    switch(clksel)
    {
        case CMU_SYSCLKCFG_CLKSEL_LRC:               /*!< 系统时钟源为LRC     */
        case CMU_SYSCLKCFG_CLKSEL_LF:                /*!< 系统时钟源为LF      */
             cpuclk = 32768;
        break;
        
        case CMU_SYSCLKCFG_CLKSEL_HRC:               /*!< 系统时钟源为HRC     */
#if defined  HT6x1x
				cpuclk = 8000000>>(HT_CMU->HRCDIV & CMU_HRCDIV );
#elif defined  HT6x2x
				cpuclk = 11010048>>(HT_CMU->HRCDIV & CMU_HRCDIV );
#elif defined  HT501x
				cpuclk = 14000000;
#elif defined  HT502x
				cpuclk = 8800000;
#endif
        break;
        
        case CMU_SYSCLKCFG_CLKSEL_PLL:               /*!< 系统时钟源为PLL     */
#if  defined  HT501x  ||  HT502x 
             if((HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV_SEL40M))
             {
                 cpuclk =  32768*600*2;  
             }
             else
             {
                 cpuclk =  32768*600;
             }
#elif defined HT6x2x
             if((HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV_SEL40M))
             {
                 cpuclk =  32768*672*2;  
             }
             else
             {
                 cpuclk =  32768*672;
             }
#else        
             cpuclk = 32768*672;
#endif
        break;
        
        default:                                     /*!< 系统时钟源为Mems    */
             cpuclk = 524288;         							 /*!< HT501x 不适用       */
        break;
    }
    cpuclk = cpuclk >> cpuclkdiv;         					/*!< 代入CPU时钟分频    */
    return cpuclk;                                   /*!< 返回系统时钟频率    */
} 

/*
*********************************************************************************************************
*                                  SET CLKOUT TO SPECIFIC FREQUENCY 
*
* 函数说明: 设置Clkout输出频率
*
* 入口参数: Clkout_InitTypeDef Clkout模块初始化结构体指针，主要包含2个参数: 
*                              1) ClkoutSel     Clkout时钟源选择，可为以下一个参数：
*                                               a) CMU_CLKOUTSEL_LRC    LRC--->Clkout
*                                               b) CMU_CLKOUTSEL_LF     LF---->Clkout
*                                               c) CMU_CLKOUTSEL_HRC    HRC--->Clkout
*                                               d) CMU_CLKOUTSEL_SYS    SYS--->Clkout
*                                               e) CMU_CLKOUTSEL_MEMS   Mems-->Clkout                                               
*                              2) ClkoutDiv     Clkout输出时钟分频设置，分频系数为：2*(ClkoutDiv[3..0]+1)
*                                               
* 返回参数: 无                                 
* 
* 特殊说明: 用户应保证此函数执行过程中寄存器写保护状态不被修改
*********************************************************************************************************
*/
void HT_CMU_ClkoutSet(Clkout_InitTypeDef* Clkout_InitStruct)
{
    /*  assert_param  */
      
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
    
    HT_CMU->CLKOUTSEL = Clkout_InitStruct->ClkoutSel;                      /*!< 设置Clkout输出时钟源   */
    
    HT_CMU->CLKOUTDIV = Clkout_InitStruct->ClkoutDiv & CMU_CLKOUTDIV;      /*!< Clkout输出时钟分频设置 */
    
    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
} 

/*
*********************************************************************************************************
*                                  CONFIGUE CLKCTRL0 REGISTER 
*
* 函数说明: 配置ClkCtrl0寄存器，使能或者关闭
*
* 入口参数: ClkCtrl0Module     ClkCtrl0寄存器模块控制位，可以为以下参数或其组合: 
*                               @arg  CMU_CLKCTRL0         配置所有控制位
*                               @arg  CMU_CLKCTRL0_LCDEN     
*                               @arg  CMU_CLKCTRL0_SPIEN     
*                               @arg  CMU_CLKCTRL0_I2CEN     
*                               @arg  CMU_CLKCTRL0_PLLEN     
*                               @arg  CMU_CLKCTRL0_HRCEN     
*                               @arg  CMU_CLKCTRL0_PLLLOCKEN 
*                               @arg  CMU_CLKCTRL0_LFDETEN   
*                               @arg  CMU_CLKCTRL0_PLLDETEN  
*                               @arg  CMU_CLKCTRL0_HRCDETEN  
*                               @arg  CMU_CLKCTRL0_WDTEN     
*                               @arg  CMU_CLKCTRL0_CLKOUTEN  
*                               @arg  CMU_CLKCTRL0_1P5LBOREN 
*                               @arg  CMU_CLKCTRL0_3DESRADEN (Only for HT6x1x)
*                               @arg  CMU_CLKCTRL0_ADESRADEN (Only for HT501x, HT6x2x, HT502x)
*                               @arg  CMU_CLKCTRL0_CRCEN     (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL0_EMUEN     (Only for HT501x, HT502x)
*                               @arg  CMU_CLKCTRL0_ECCEN     (Only for HT502x)
*
*           NewState           = ENABLE： 使能相应模块
*                              = DISABLE：关闭相应模块        
*                                               
* 返回参数: 无                                 
* 
* 特殊说明: 用户应保证此函数执行过程中寄存器写保护状态不被修改
*********************************************************************************************************
*/
void HT_CMU_ClkCtrl0Config(uint32_t ClkCtrl0Module, FunctionalState NewState)
{
    /*  assert_param  */
      
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
    
    ClkCtrl0Module &= CMU_CLKCTRL0;
    
    if (NewState != DISABLE)
    {       
        HT_CMU->CLKCTRL0 |= ClkCtrl0Module;                                /*!< 使能Ctrl0内相应模块    */ 
    }
    else
    {
#if defined HT6x1x	
        ClkCtrl0Module &= ~CMU_CLKCTRL0_Reserved;                          /*!< Bit6 保留为1           */
#endif
        HT_CMU->CLKCTRL0 &= ~ClkCtrl0Module;                               /*!< 关闭Ctrl0内相应模块    */ 
    }     
     
    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
} 

/*
*********************************************************************************************************
*                                  CONFIGUE CLKCTRL0 REGISTER 
*
* 函数说明: 配置ClkCtrl1寄存器，使能或者关闭
*
* 入口参数: ClkCtrl1Module     ClkCtrl1寄存器模块控制位，可以为以下参数或其组合:
*                               @arg  CMU_CLKCTRL1            配置所有控制位
*                               @arg  CMU_CLKCTRL1_TMR0EN      
*                               @arg  CMU_CLKCTRL1_TMR1EN      
*                               @arg  CMU_CLKCTRL1_TMR2EN      
*                               @arg  CMU_CLKCTRL1_TMR3EN      
*                               @arg  CMU_CLKCTRL1_UART0EN     
*                               @arg  CMU_CLKCTRL1_UART1EN     
*                               @arg  CMU_CLKCTRL1_UART2EN     
*                               @arg  CMU_CLKCTRL1_UART37816EN 
*                               @arg  CMU_CLKCTRL1_UART47816EN 
*                               @arg  CMU_CLKCTRL1_UART5EN  
*                               @arg  CMU_CLKCTRL1_TMR4EN         (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_TMR5EN         (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_UART6EN        (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_SPI1EN         (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_SOFTWDTEN      (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_LRCRTCEN       (Only for HT6x2x)
*                               @arg  CMU_CLKCTRL1_LRCTFREQEN     (Only for HT501x, HT502x)
*                               @arg  CMU_CLKCTRL1_LRCRTCEN       (Only for HT501x, HT502x)
*                               @arg  CMU_CLKCTRL1_AUTORCCAL_EN   (Only for HT6x2x)
*
*           NewState           = ENABLE： 使能相应模块
*                              = DISABLE：关闭相应模块        
*                                               
* 返回参数: 无                                 
* 
* 特殊说明: 用户应保证此函数执行过程中寄存器写保护状态不被修改
*********************************************************************************************************
*/
void HT_CMU_ClkCtrl1Config(uint32_t ClkCtrl1Module, FunctionalState NewState)
{
    /*  assert_param  */
      
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
      
    ClkCtrl1Module &= CMU_CLKCTRL1;
    
    if (NewState != DISABLE)
    {       
        HT_CMU->CLKCTRL1 |= ClkCtrl1Module;                                /*!< 使能Ctrl1内相应模块    */ 
    }
    else
    {
        HT_CMU->CLKCTRL1 &= ~ClkCtrl1Module;                               /*!< 关闭Ctrl1内相应模块    */ 
    }     
     
    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
} 

/*
*********************************************************************************************************
*                                      GET JTAG STATUS
*
* 函数说明: 获取JTAG状态
*
* 入口参数: 无
*
* 返回参数: Bool    = TRUE： 芯片处于调试状态
*                   = FALSE：芯片处于正常状态
* 
* 特殊说明: 无
*********************************************************************************************************
*/
Bool HT_CMU_JTAGStatusGet()
{
    /*  assert_param  */
    
    return (HT_CMU->JTAGSTA ? TRUE : FALSE);
 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED CLOCK STOP FLAG
*
* 函数说明: 获取相应时钟停振标志
*
* 入口参数: STAFlag     想要检查的某个时钟停振标志，可以为以下参数:
*                        @arg CMU_CLKSTA_LFFLAG
*                        @arg CMU_CLKSTA_HRCFLAG
*                        @arg CMU_CLKSTA_PLLFLAG
*						 @arg CMU_CLKSTA_PLLLOCK
*
* 返回参数: FlagStatus  = SET：  相应时钟发生停振
*                       = RESET：相应时钟未停振
* 
* 特殊说明: 用户应打开时钟停振检测使能位才能得到有效的标志，参见HT_CMU_ClkCtrl0Config()
*********************************************************************************************************
*/
FlagStatus HT_CMU_StopFlagGet(uint8_t STAFlag)
{
    /*  assert_param  */
    
    if (HT_CMU->CLKSTA & STAFlag)
    {       
        return SET;                        /*!< Clock Stop Flag is set   */
    }
    else
    {
        return RESET;                      /*!< Clock Stop Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                            Flash delay set
*
* 函数说明: 设置flash_dly
*
* 入口参数: STAFlag     想要检查的某个时钟停振标志，可以为以下参数:
*          NewState           = ENABLE： 使能相应模块
*                             = DISABLE：关闭相应模块  
*********************************************************************************************************
*/

void	HT_CMU_FlashDly_Set(FunctionalState NewState)
{
#if  defined  HT501x	||  HT502x
		uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
    
    if (NewState != DISABLE)
    {       
        HT_CMU->FLASHDLY = CMU_FLASHDLY_ENABLE;                           /*!< 使能FLASH_DLY          */ 
    }
    else
    {
        HT_CMU->FLASHDLY = CMU_FLASHDLY_DISABLE;                          /*!< 关闭Ctrl1内相应模块    */ 
    }  

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
#endif
}
/*
*********************************************************************************************************
*                            Flash prefetch set
*
* 函数说明: 设置prefetch
*
* 入口参数: STAFlag     想要检查的某个时钟停振标志，可以为以下参数:
*          NewState           = ENABLE： 使能相应模块
*                             = DISABLE：关闭相应模块  
*********************************************************************************************************
*/
void	HT_CMU_Prefetch_Set(FunctionalState NewState)
{
#if  defined  HT6x2x	
		uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
    
    if (NewState != DISABLE)
    {       
        HT_CMU->PREFETCH = CMU_PREFETCH_ENABLE;                           /*!< 使能prefetch         */ 
    }
    else
    {
        HT_CMU->PREFETCH = CMU_PREFETCH_DISABLE;                          /*!< 关闭prefetch    */ 
    }  

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
#endif
}
/*
*********************************************************************************************************
*                                     Wait For 2 MILLISECOND
*
* 函数说明: 等待2ms的延迟，模块内部函数，
*
* 入口参数: 无                         
*
* 返回参数: 无                                   
* 
* 特殊说明: 延迟时间只是大概正确（注：目前没有调试，延迟时间还未确定）
*********************************************************************************************************
*/
void Wait2mS(void)
{
    
    uint32_t delay;
    
    delay = HT_CMU_SysClkGet();                             /*!< 获取当前系统时钟Fsys     */
    delay = delay>>(HT_CMU->SYSCLKDIV & CMU_SYSCLKDIV );    /*!< 获取当前CPU时钟Fcpu      */
    
    delay = delay>>10;                                      /*!< 500 X 2                  */
    
    while(delay--);                                         /*!< 等待2ms                  */       
}

