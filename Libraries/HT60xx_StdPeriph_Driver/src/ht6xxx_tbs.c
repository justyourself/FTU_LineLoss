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
* File         : ht6xxx_tbs.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_TBS_C

#include "ht6xxx_tbs.h"

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


/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     CONFIGUE TBS MODULE
*
* 函数说明: 配置TBS子模块
*
* 入口参数: TBS_SubModule    TBS内各个子模块，可以为以下参数或其组合 
*                             @arg TBS_TBSCON_TMPEn      ：温度测量模块
*                             @arg TBS_TBSCON_VBATEn     ：电池电压测量模块
*                             @arg TBS_TBSCON_ADC0En     ：ADC0测量模块
*                             @arg TBS_TBSCON_ADC1En     ：ADC1测量模块
*                             @arg TBS_TBSCON_VCCEn      ：电源电压测量模块
*                             @arg TBS_TBSCON_ADC2En     ：ADC2测量模块      (only for HT501x)
*                             @arg TBS_TBSCON_VREFEn     ：Vref测量模块      (only for HT501x)
*
*           NewState         = ENABLE： 模块使能
*                            = DISABLE：模块关闭                      
*
* 返回参数: 无                                      
* 
* 特殊说明: 1) 由于RTC补偿涉及温度传感器，不建议关闭温度传感器模块。
*           2) 建议在使能电池电压比较之前，先调用函数HT_TBS_BatCmpThresholdSet() 设置好电池电压比较阈值,
*           3) 电池电压比较功能若想正确执行，需使能电池电压测量模块
*********************************************************************************************************
*/
void HT_TBSConfig(uint32_t TBS_SubModule, FunctionalState NewState)
{
    /*  assert_param  */

    uint32_t tempreg;

    tempreg = HT_TBS->TBSCON & TBS_TBSCON_xEn;  

    if(HT_CMU->CLKSTA & CMU_CLKSTA_LFSEL)
    {
        tempreg |= 0x6d60;                       /*!< MEMS TPS 默认配置         */
    }  
    else
    {
#if defined HT501x
        tempreg |= 0x100;
#else
        tempreg |= 0x6540;                       /*!< OSC TPS 默认配置         */
#endif
    }  
    
    TBS_SubModule &= TBS_TBSCON_xEn;
    if (NewState != DISABLE)
    {       
        tempreg |= (uint32_t)TBS_SubModule;      /*!< 使能TBS模块                 */
    }
    else
    {
        tempreg &= ~(uint32_t)TBS_SubModule;     /*!< 关闭TBS模块                 */
    } 

    HT_TBS->TBSCON = tempreg;                    /*!< 写寄存器                    */
  
} 

/*
*********************************************************************************************************
*                                 SET SubModule OPEN PERIOD
*
* 函数说明: 设置子模块打开频率周期
*
* 入口参数: PRDModule    : 子模块周期控制，可以为以下参数
*                          @arg TMPPRD      ：温度测量周期
*                          @arg VBATPRD     ：电池电压测量周期
*                          @arg ADC0PRD     ：ADC0测量周期
*                          @arg ADC1PRD     ：ADC1测量周期
*                          @arg VCCPRD      ：VCC测量周期
*                          @arg ADC2PRD     ：ADC2测量周期    (only for HT501x)
*                          @arg VREFPRD     ：VREF测量周期    (only for HT501x)
*                          @arg ADC3PRD     ：ADC3测量周期    (only for HT6x2x)
*                          @arg ADC4PRD     ：ADC4测量周期    (only for HT6x2x)
*                          @arg ADC5PRD     ：ADC5测量周期    (only for HT6x2x)
*
*           Period       : 周期设定，根据不同芯片，可以为以下参数
*                                   HT501x  HT502x                  HT6x1x  HT6x2x
*                             TMPPRD:                           TMPPRD:
*                        @arg TBS_TBSPRD_TMPPRD_1S              TBS_TBSPRD_TMPPRD_OSC0P5S_MEM8S    
*                        @arg TBS_TBSPRD_TMPPRD_8S              TBS_TBSPRD_TMPPRD_OSC1S_MEM4S      
*                        @arg TBS_TBSPRD_TMPPRD_32S             TBS_TBSPRD_TMPPRD_OSC2S_MEM2S      
*                        @arg TBS_TBSPRD_TMPPRD_125mS           TBS_TBSPRD_TMPPRD_OSC4S_MEM1S      
*                                                               TBS_TBSPRD_TMPPRD_OSC8S_MEM0P5S                                                                                                  
*                             VBATPRD:                          TBS_TBSPRD_TMPPRD_OSC16S_MEM125mS  
*                        @arg TBS_TBSPRD_VBATPRD_1S             TBS_TBSPRD_TMPPRD_OSC32S_MEM31mS   
*                        @arg TBS_TBSPRD_VBATPRD_4S             TBS_TBSPRD_TMPPRD_OSC64S_MEM8mS    
*                        @arg TBS_TBSPRD_VBATPRD_8S                                                
*                        @arg TBS_TBSPRD_VBATPRD_32S            VBATPRD:                
*                                                               TBS_TBSPRD_VBATPRD_1S              
*                             VCCPRD:                           TBS_TBSPRD_VBATPRD_2S              
*                        @arg TBS_TBSPRD_VCCPRD_1S              TBS_TBSPRD_VBATPRD_8S              
*                        @arg TBS_TBSPRD_VCCPRD_4S              TBS_TBSPRD_VBATPRD_16S             
*                        @arg TBS_TBSPRD_VCCPRD_8S                                                 
*                        @arg TBS_TBSPRD_VCCPRD_32S             ADC0PRD:                 
*                                                               TBS_TBSPRD_ADC0PRD_1S              
*                             ADC0PRD:                          TBS_TBSPRD_ADC0PRD_2S              
*                        @arg TBS_TBSPRD_ADC0PRD_1S             TBS_TBSPRD_ADC0PRD_8S              
*                        @arg TBS_TBSPRD_ADC0PRD_4S             TBS_TBSPRD_ADC0PRD_16S             
*                        @arg TBS_TBSPRD_ADC0PRD_8S                                                
*                        @arg TBS_TBSPRD_ADC0PRD_32S            ADC1PRD:                 
*                                                               TBS_TBSPRD_ADC1PRD_1S              
*                             ADC1PRD:                          TBS_TBSPRD_ADC1PRD_2S              
*                        @arg TBS_TBSPRD_ADC1PRD_1S             TBS_TBSPRD_ADC1PRD_8S              
*                        @arg TBS_TBSPRD_ADC1PRD_4S             TBS_TBSPRD_ADC1PRD_16S             
*                        @arg TBS_TBSPRD_ADC1PRD_8S                                                
*                        @arg TBS_TBSPRD_ADC1PRD_32S            VCCPRD:                  
*                                                               TBS_TBSPRD_VCCPRD_1S               
*                             ADC2PRD:                          TBS_TBSPRD_VCCPRD_2S               
*                        @arg TBS_TBSPRD_ADC2PRD_1S             TBS_TBSPRD_VCCPRD_8S               
*                        @arg TBS_TBSPRD_ADC2PRD_4S             TBS_TBSPRD_VCCPRD_16S              
*                        @arg TBS_TBSPRD_ADC2PRD_8S    
*                        @arg TBS_TBSPRD_ADC2PRD_32S            ADC3PRD:(only for HT6x2x)
*                                                               TBS_TBSPRD_ADC3PRD_1S
*                             VREFPRD:                          TBS_TBSPRD_ADC3PRD_2S
*                        @arg TBS_TBSPRD_VREFPRD_1S             TBS_TBSPRD_ADC3PRD_8S    
*                        @arg TBS_TBSPRD_VREFPRD_4S             TBS_TBSPRD_ADC3PRD_16S
*                        @arg TBS_TBSPRD_VREFPRD_8S    
*                        @arg TBS_TBSPRD_VREFPRD_32S            ADC4PRD:(only for HT6x2x)
*                                                               TBS_TBSPRD_ADC4PRD_1S
*                                                               TBS_TBSPRD_ADC4PRD_2S
*                                                               TBS_TBSPRD_ADC4PRD_8S
*                                                               TBS_TBSPRD_ADC4PRD_16S
*
*                                                               ADC5PRD:(only for HT6x2x)
*                                                               TBS_TBSPRD_ADC5PRD_1S
*                                                               TBS_TBSPRD_ADC5PRD_2S
*                                                               TBS_TBSPRD_ADC5PRD_8S
*                                                               TBS_TBSPRD_ADC5PRD_16S
*
* 返回参数: 无                                     
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_TBS_PeriodSet(TBS_PeriodTypeDef PRDModule, uint32_t Period)
{
    /*  assert_param  */

     uint32_t tempreg = (HT_TBS->TBSPRD & (~PRDModule));  
     tempreg |= Period;
     HT_TBS->TBSPRD = tempreg;                             /*!< 配置测量周期           */     
}

/*
*********************************************************************************************************
*                                     SET COMPARE THRESHOLD
*
* 函数说明: 设置比较阈值
*
* 入口参数: CmpReg    比较寄存器，可以为以下参数
*                     @arg CMP_VBAT     ：电池电压比较寄存器  
*                     @arg CMP_ADC0     ：ADC0比较寄存器      (only for HT501x, HT502x, HT6x2x)
*                     @arg CMP_ADC1     ：ADC1比较寄存器      (only for HT501x, HT502x)
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_TBS_CMPThresholdSet(TBS_CMPRegTypeDef CmpReg, int16_t Threshold)
{
    /*  assert_param  */
    
    if(CmpReg == CMP_VBAT)
        HT_TBS->VBATCMP = Threshold;                /*!< 设置电池电压比较阈值           */
#if defined HT501x  ||  HT6x2x  ||  HT502x
    else if(CmpReg == CMP_ADC0)
        HT_TBS->ADC0CMP = Threshold;                /*!< 设置ADC0比较阈值               */
#endif
#if defined HT501x  ||  HT502x
    else if(CmpReg == CMP_ADC1)
        HT_TBS->ADC1CMP = Threshold;                /*!< 设置ADC1比较阈值               */
#endif    
} 

/*
*********************************************************************************************************
*                                     GET MEASURED VALUES
*
* 函数说明: 获取测量值
*
* 入口参数: SubModule    : 子模块选择                       
*
* 返回参数: 被选中子模块测量值，返回值为16位有符号数                                     
* 
* 特殊说明: 无
*********************************************************************************************************
*/
int16_t HT_TBS_ValueRead(TBS_SubModuleTypeDef SubModule)
{
    /*  assert_param  */

    switch(SubModule)
    {
        case TBS_TMP:
            return  (int16_t)HT_TBS->TMPDAT;       /*!< 返回温度测量值           */
        case TBS_VBAT:
            return  (int16_t)HT_TBS->VBATDAT;      /*!< 返回电池电压测量值       */
        case TBS_ADC0:
            return  (int16_t)HT_TBS->ADC0DAT;      /*!< 返回ADC0测量值           */
        case TBS_ADC1:
            return  (int16_t)HT_TBS->ADC1DAT;      /*!< 返回ADC1测量值           */
#if defined HT501x
        case TBS_ADC2:
            return  (int16_t)HT_TBS->ADC2DAT;      /*!< 返回ADC2测量值           */
#endif
        default:
            return  (int16_t)HT_TBS->VCCDAT;       /*!< 返回电源电压测量值       */
    }
} 
/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE TBS INTERRUPT    
*
* 函数说明: 使能或者关闭TBS中断
*
* 入口参数: ITEn       TBS中断设置位，可以为以下参数或其组合
*                        @arg TBS_TBSIE_TMPIE
*                        @arg TBS_TBSIE_VBATIE
*                        @arg TBS_TBSIE_ADC0IE
*                        @arg TBS_TBSIE_ADC1IE
*                        @arg TBS_TBSIE_VBATCMPIE
*                        @arg TBS_TBSIE_VCCIE
*                        @arg TBS_TBSIE_ADC2IE        (only for HT501x)
*                        @arg TBS_TBSIE_VREFIE        (only for HT501x)
*                        @arg TBS_TBSIE_ADC0CMPIE     (only for HT501x, HT6x2x, HT502x)
*                        @arg TBS_TBSIE_ADC1CMPIE     (only for HT501x, HT502x)
*
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_TBS_ITConfig(uint32_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_TBS->TBSIE |= (uint32_t)ITEn;            /*!< 使能TBS中断           */
    }
    else
    {
        HT_TBS->TBSIE &= ~(uint32_t)ITEn;           /*!< 关闭TBS中断           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED TBS INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应TBS中断标志状态
*
* 入口参数: ITFlag     想要检查的某个TBS中断，可以为以下参数:
*                        @arg TBS_TBSIF_TMPIF
*                        @arg TBS_TBSIF_VBATIF
*                        @arg TBS_TBSIF_ADC0IF
*                        @arg TBS_TBSIF_ADC1IF
*                        @arg TBS_TBSIF_VBATCMPIF
*                        @arg TBS_TBSIF_VCCIF
*                        @arg TBS_TBSIF_ADC2IF        (only for HT501x)
*                        @arg TBS_TBSIF_VREFIF        (only for HT501x)
*                        @arg TBS_TBSIF_ADC0CMPIF     (only for HT501x, HT6x2x, HT502x)
*                        @arg TBS_TBSIF_ADC1CMPIF     (only for HT501x, HT502x)
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
* 
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_TBS_ITFlagStatusGet(uint32_t ITFlag)
{
    /*  assert_param  */
    
    if (HT_TBS->TBSIF & ITFlag)
    {       
        return SET;                        /*!< TBS Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< TBS Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR TBS INTERRUPT FLAG
*
* 函数说明: 清除TBS中断标志
*
* 入口参数: ITFlag     想要清除的某个TBS中断标志，可以为以下参数或其组合:
*                        @arg TBS_TBSIF_TMPIF
*                        @arg TBS_TBSIF_VBATIF
*                        @arg TBS_TBSIF_ADC0IF
*                        @arg TBS_TBSIF_ADC1IF
*                        @arg TBS_TBSIF_VBATCMPIF
*                        @arg TBS_TBSIF_VCCIF
*                        @arg TBS_TBSIF_ADC2IF        (only for HT501x)
*                        @arg TBS_TBSIF_VREFIF        (only for HT501x)
*                        @arg TBS_TBSIF_ADC0CMPIF     (only for HT501x, HT6x2x, HT502x)
*                        @arg TBS_TBSIF_ADC1CMPIF     (only for HT501x, HT502x)
*
* 返回参数: 无
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_TBS_ClearITPendingBit(uint32_t ITFlag)
{
    /*  assert_param  */
    
      
    HT_TBS->TBSIF  &= ~((uint32_t)ITFlag);                  /*!< Clear TBS Interrupt Flag       */
    
}

