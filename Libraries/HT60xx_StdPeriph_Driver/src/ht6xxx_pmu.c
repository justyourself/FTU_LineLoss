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
* File         : ht6xxx_pmu.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.3
* Description  :
*********************************************************************************************************
*/

#define  __HT6XXX_PMU_C

#include "ht6xxx_pmu.h"

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


/*
*********************************************************************************************************
*                                         INITIAL PMU MODULE
*
* 函数说明: 初始化PMU模块
*
* 入口参数: PMU_InitStruct     PMU初始化结构体指针，主要包含几个参数:
*                              1) Lvdin0State      : Lvdin0状态设置
*                              2) Lvdin1State      : Lvdin1状态设置 (for HT501x, HT502x, HT602x, HT6x3x)
*                              3) Lvdin2State      : Lvdin2状态设置 (for HT6x3x)
*                              4) POWState         : Pow状态设置    (for HT501x, HT502x, HT602x, HT6x3x)
*                              5) POWFilter        : Pow滤波设置    (for HT6x3x)
*                              6) BorState         : Bor状态设置
*                              7) VccLvlSet        : Vcc检测阈值设定
*                              8) BorLvlSet        : Bor检测阈值设定
*                              9) VccBorDetTime    : Vcc以及Bor检测时间设置
*                              10) VccBorDetPeriod : Vcc以及Bor检测周期设置
*
* 返回参数: 无
*
* 特殊说明: 用户应保证此函数执行过程中寄存器写保护状态不被修改
*********************************************************************************************************
*/
void HT_PMU_Init(PMU_InitTypeDef* PMU_InitStruct)
{
    /*  assert_param  */
    uint32_t tempreg = 0;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    tempreg = HT_PMU->PMUCON & 0x8010;
    tempreg |= (PMU_InitStruct->Lvdin0State & 0x000F);                     /*!< LVDIN0状态设置         */
#if  defined  HT6x2x  ||  defined  HT6x3x  ||  defined  HT501x  ||  defined  HT502x
    tempreg |= ((PMU_InitStruct->Lvdin1State & 0x000F)<<1);                /*!< LVDIN1状态设置         */
#endif
#if  defined  HT6x3x
    tempreg |= ((PMU_InitStruct->Lvdin2State & 0x000F)<<4);                /*!< LVDIN2状态设置         */
#endif
#if  defined  HT6x2x  ||  defined  HT6x3x
    tempreg |= PMU_InitStruct->POWState;                                   /*!< 配置POW功能            */
#endif
    tempreg |= PMU_InitStruct->BorState;                                   /*!< 配置BOR功能            */
    HT_PMU->PMUCON = tempreg;

#if  defined  HT6x3x
    tempreg = 0;                                                           /*!< 配置LVDIN的QR功能      */
    tempreg |= ((PMU_InitStruct->Lvdin0State & 0x00F0)>>4);                /*!< LVDIN0状态设置         */
    tempreg |= ((PMU_InitStruct->Lvdin1State & 0x00F0)>>3);                /*!< LVDIN1状态设置         */
    tempreg |= ((PMU_InitStruct->Lvdin2State & 0x00F0)>>2);                /*!< LVDIN2状态设置         */
    HT_PMU->LVDINQR = tempreg;
#endif

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */

    HT_PMU->VDETCFG &= ~(PMU_VDETCFG_VCCLVL | PMU_VDETCFG_BORLVL);
    HT_PMU->VDETCFG |= ((uint32_t)PMU_InitStruct->VccLvlSet | (uint32_t)PMU_InitStruct->BorLvlSet);
                                                                           /*!< VccBor检测阈值设置     */
    HT_PMU->VDETPCFG &= ~(PMU_VDETPCFG_VDETPRD | PMU_VDETPCFG_VDETTIME);
    HT_PMU->VDETPCFG |= ((uint32_t)PMU_InitStruct->VccBorDetTime | (uint32_t)PMU_InitStruct->VccBorDetPeriod);
#if defined  HT6x2x  ||  defined  HT6x3x                                   /*!< 低功耗检测滤波设置     */
    HT_PMU->PDTFLT = PMU_InitStruct->POWFilter & 0x000F;                   /*!< POW数字滤波值设置      */
#endif
}

/*
*********************************************************************************************************
*                            Enable/Disable THE BIG POWER LDO ON HOLD MODE
*
* 函数说明: 配置Hold模式下大功耗LDO
*
* 入口参数: NewState    = ENABLE： Hold模式下使能大功耗LDO
*                       = DISABLE：Hold模式下关闭大功耗LDO
*
* 返回参数: 无
*
* 特殊说明: 用户应保证此函数执行过程中寄存器写保护状态不被修改
*********************************************************************************************************
*/
void HT_PMU_HoldLDOConfig(FunctionalState NewState)
{
    /*  assert_param  */
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    if (NewState != DISABLE)
    {
        HT_PMU->PMUCON |= PMU_PMUCON_HoldLDO;                              /*!< Hold模式下使能大功耗LDO*/
    }
    else
    {
        HT_PMU->PMUCON &= ~PMU_PMUCON_HoldLDO;                             /*!< Hold模式下关闭大功耗LDO*/
    }

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}

/*
*********************************************************************************************************
*                                    DISCHARGE  CONFIGURE
*
* 函数说明: 电池钝化配置
*
* 入口参数: ChargeMode  = DisCharge0mA
*                       = DisCharge1mA
*                       = DisCharge2mA
*                       = DisCharge50uA  (for HT6x2x, HT6x3x)
*                       = DisCharge100uA (for HT6x2x, HT6x3x)
*
* 返回参数: 无
*
* 特殊说明: 用户应保证此函数执行过程中寄存器写保护状态不被修改
*********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT6x3x  ||  defined HT501x  ||  HT502x
void HT_PMU_DisChargeConfig(DISCharge_TypeDef ChargeMode)
{
    /*  assert_param  */
    uint32_t tempreg;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    tempreg  =  HT_PMU->PMUCON & 0x7FFF;
    tempreg |=  ChargeMode & 0x8000;
    HT_PMU->PMUCON = tempreg;                                              /*!< 设置DisCharge使能      */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */

#if defined  HT6x2x  ||  defined  HT6x3x
    tempreg  =  HT_PMU->VDETCFG & 0x3FFF;
    tempreg |=  (ChargeMode>>4) & 0xC000;
#elif defined  HT501x  ||  defined  HT502x
    tempreg  =  HT_PMU->VDETCFG & 0x7FFF;
    tempreg |=  (ChargeMode>>4) & 0x8000;
#endif
    HT_PMU->VDETCFG = tempreg;                                             /*!< 设置DisCharge电流      */
}
#endif

/*
*********************************************************************************************************
*                               GET SPECIFIED PMU STATE FLAG STATUS
*
* 函数说明: 获取相应PMU检测状态标志
*
* 入口参数: STAFlag    想要检查的某个PMU中断，可以为以下参数:
*                        @arg PMU_PMUSTA_VCCFLG
*                        @arg PMU_PMUSTA_BORFLG
*                        @arg PMU_PMUSTA_LVD0FLG
*                        @arg PMU_PMUSTA_LVD1FLG (for HT501x, HT502x, HT6x2x, HT6x3x)
*                        @arg PMU_PMUSTA_POWFLG  (for HT6x2x, HT6x3x)
*                        @arg PMU_PMUSTA_LVD2FLG (for HT6x3x)
*
* 返回参数: ITStatus    = SET：  检测值大于设定阈值
*                       = RESET：检测值低于设定阈值
*
* 特殊说明: 无
*********************************************************************************************************
*/
FlagStatus HT_PMUState_FlagStatusGet(uint8_t STAFlag)
{
    /*  assert_param  */
    if (HT_PMU->PMUSTA & STAFlag)
    {
        return SET;                        /*!< 检测值大于设定阈值         */
    }
    else
    {
        return RESET;                      /*!< 检测值低于设定阈值         */
    }
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE PMU INTERRUPT
*
* 函数说明: 使能或者关闭PMU中断
*
* 入口参数: ITEn       PMU中断设置位，可以为以下参数或其组合
*                        @arg PMU_PMUIE_VCCIE
*                        @arg PMU_PMUIE_BORIE
*                        @arg PMU_PMUIE_LVD0IE
*                        @arg PMU_PMUIE_LVD1IE (for HT501x, HT502x, HT6x2x, HT6x3x)
*                        @arg PMU_PMUIE_POWIE  (for HT6x2x, HT6x3x)
*                        @arg PMU_PMUIE_LVD2IE (for HT6x3x)
*
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_PMU_ITConfig(uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    if (NewState != DISABLE)
    {
        HT_PMU->PMUIE |= (uint32_t)(ITEn & PMU_PMUIE);            /*!< 使能PMU中断           */
    }
    else
    {
        HT_PMU->PMUIE &= ~(uint32_t)ITEn;                         /*!< 关闭PMU中断           */
    }
}

/*
*********************************************************************************************************
*                            GET SPECIFIED PMU INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应PMU中断标志状态
*
* 入口参数: ITFlag     想要检查的某个PMU中断，可以为以下参数:
*                        @arg PMU_PMUIF_VCCIF
*                        @arg PMU_PMUIF_BORIF
*                        @arg PMU_PMUIF_LVD0IF
*                        @arg PMU_PMUIF_LVD1IF (for HT501x, HT502x, HT6x2x, HT6x3x)
*                        @arg PMU_PMUIF_POWIF  (for HT6x2x, HT6x3x)
*                        @arg PMU_PMUIF_LVD2IF (for HT6x3x)
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
*
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_PMU_ITFlagStatusGet(uint8_t ITFlag)
{
    /*  assert_param  */
    if (HT_PMU->PMUIF & ITFlag)
    {
        return SET;                        /*!< PMU Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< PMU Interrupt Flag is reset */
    }
}

/*
*********************************************************************************************************
*                                CLEAR PMU INTERRUPT FLAG
*
* 函数说明: 清除PMU中断标志
*
* 入口参数: ITFlag     想要清除的某个PMU中断标志，可以为以下参数或其组合:
*                        @arg PMU_PMUIF_VCCIF
*                        @arg PMU_PMUIF_BORIF
*                        @arg PMU_PMUIF_LVD0IF
*                        @arg PMU_PMUIF_LVD1IF (for HT501x, HT502x, HT6x2x, HT6x3x)
*                        @arg PMU_PMUIF_POWIF  (for HT6x2x, HT6x3x)
*                        @arg PMU_PMUIF_LVD2IF (for HT6x3x)
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_PMU_ClearITPendingBit(uint8_t ITFlag)
{
    /*  assert_param  */
    HT_PMU->PMUIF &= ~((uint32_t)ITFlag);                  /*!< Clear PMU Interrupt Flag    */
}
