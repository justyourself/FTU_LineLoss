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
* File         : ht6xxx_reset.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  :
*********************************************************************************************************
*/

#define  __HT6XXX_RESET_C

#include "ht6xxx_reset.h"
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


/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                               GET SPECIFIED WAKE FLAG STATUS
*
* 函数说明: 获取相应唤醒状态标志
*
* 入口参数: STAFlag    想要检查的某个唤醒标志，可以为以下参数:
*                       @arg PMU_WAKEIF_PMUWKIF
*                       @arg PMU_WAKEIF_INT0WKIF
*                       @arg PMU_WAKEIF_INT1WKIF
*                       @arg PMU_WAKEIF_INT2WKIF
*                       @arg PMU_WAKEIF_INT3WKIF
*                       @arg PMU_WAKEIF_INT4WKIF
*                       @arg PMU_WAKEIF_INT5WKIF
*                       @arg PMU_WAKEIF_INT6WKIF
*                       @arg PMU_WAKEIF_RX0WKIF
*                       @arg PMU_WAKEIF_RX1WKIF
*                       @arg PMU_WAKEIF_RX2WKIF
*                       @arg PMU_WAKEIF_RX3WKIF
*                       @arg PMU_WAKEIF_RX4WKIF
*                       @arg PMU_WAKEIF_RX5WKIF
*                       @arg PMU_WAKEIF_TBSWKIF
*                       @arg PMU_WAKEIF_RTCWKIF
*                       @arg PMU_WAKEIF_WDTWKIF     (for HT6x1x, HT501x)
*                       @arg PMU_WAKEIF_AREGWKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR0WKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR1WKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR2WKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR3WKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR4WKIF    (for HT6x2x, HT6x3x)
*                       @arg PMU_WAKEIF_TMR5WKIF    (for HT6x2x, HT6x3x)
*                       @arg PMU_WAKEIF_IICWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_SPIWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_SELFTESTWKIF(for HT502x)
*                       @arg PMU_WAKEIF_EMUWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_DMAWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_KEYWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_RX6WKIF     (for HT6x2x, HT6x3x)
*                       @arg PMU_WAKEIF_INT7WKIF    (for HT6x2x, HT6x3x, HT502x)
*                       @arg PMU_WAKEIF_INT8WKIF    (for HT6x2x, HT6x3x, HT502x)
*                       @arg PMU_WAKEIF_INT9WKIF    (for HT6x2x, HT6x3x, HT502x)
*                       @arg PMU_WAKEIF_NMIWKIF     (for HT502x)
*
* 返回参数: ITStatus    = SET：  唤醒标志置起
*                       = RESET：唤醒标志未置起
*
* 特殊说明: Hold和Sleep唤醒都会导致相应唤醒标志置位，用户如果想进一步区分是Hold唤醒还是Sleep唤醒，可以查看
*           RSTSTA寄存器中PMU_RSTSTA_SleepFlag和PMU_RSTSTA_HoldFlag标志位来区分
*********************************************************************************************************
*/
FlagStatus HT_WakeFlagStatusGet(uint32_t STAFlag)
{
    /*  assert_param  */
    if (HT_PMU->WAKEIF & STAFlag)
    {
        return SET;                        /*!< 产生相应唤醒标志          */
    }
    else
    {
        return RESET;                      /*!< 未产生相应唤醒标志        */
    }
}

/*
*********************************************************************************************************
*                               CLEAR SPECIFIED WAKE FLAG STATUS
*
* 函数说明: 清除相应唤醒标志
*
* 入口参数: STAFlag    想要清除的某个唤醒标志，可以为以下参数:
*                       @arg PMU_WAKEIF           此项清除所有标志
*                       @arg PMU_WAKEIF_PMUWKIF
*                       @arg PMU_WAKEIF_INT0WKIF
*                       @arg PMU_WAKEIF_INT1WKIF
*                       @arg PMU_WAKEIF_INT2WKIF
*                       @arg PMU_WAKEIF_INT3WKIF
*                       @arg PMU_WAKEIF_INT4WKIF
*                       @arg PMU_WAKEIF_INT5WKIF
*                       @arg PMU_WAKEIF_INT6WKIF
*                       @arg PMU_WAKEIF_RX0WKIF
*                       @arg PMU_WAKEIF_RX1WKIF
*                       @arg PMU_WAKEIF_RX2WKIF
*                       @arg PMU_WAKEIF_RX3WKIF
*                       @arg PMU_WAKEIF_RX4WKIF
*                       @arg PMU_WAKEIF_RX5WKIF
*                       @arg PMU_WAKEIF_TBSWKIF
*                       @arg PMU_WAKEIF_RTCWKIF
*                       @arg PMU_WAKEIF_WDTWKIF     (for HT6x1x, HT501x)
*                       @arg PMU_WAKEIF_AREGWKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR0WKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR1WKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR2WKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR3WKIF    (for HT501x, HT502x)
*                       @arg PMU_WAKEIF_TMR4WKIF    (for HT6x2x, HT6x3x)
*                       @arg PMU_WAKEIF_TMR5WKIF    (for HT6x2x, HT6x3x)
*                       @arg PMU_WAKEIF_IICWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_SPIWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_SELFTESTWKIF(for HT502x)
*                       @arg PMU_WAKEIF_EMUWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_DMAWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_KEYWKIF     (for HT502x)
*                       @arg PMU_WAKEIF_RX6WKIF     (for HT6x2x, HT6x3x)
*                       @arg PMU_WAKEIF_INT7WKIF    (for HT6x2x, HT6x3x, HT502x)
*                       @arg PMU_WAKEIF_INT8WKIF    (for HT6x2x, HT6x3x, HT502x)
*                       @arg PMU_WAKEIF_INT9WKIF    (for HT6x2x, HT6x3x, HT502x)
*                       @arg PMU_WAKEIF_NMIWKIF     (for HT502x)
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_WakeFlagClear(uint32_t STAFlag)
{
    /*  assert_param  */
    HT_PMU->WAKEIF &= ~STAFlag;                 /*!< 清除相应唤醒标志          */
}

/*
*********************************************************************************************************
*                               GET SPECIFIED RESET FLAG STATUS
*
* 函数说明: 获取相应系统复位标志
*
* 入口参数: STAFlag    想要检查的某个复位标志，可以为以下参数:
*                       @arg PMU_RSTSTA_PORRST
*                       @arg PMU_RSTSTA_LBORRST
*                       @arg PMU_RSTSTA_WDTRST
*                       @arg PMU_RSTSTA_WakeupRST
*                       @arg PMU_RSTSTA_ExtRST
*                       @arg PMU_RSTSTA_SoftRST
*                       @arg PMU_RSTSTA_DebugRST
*                       @arg PMU_RSTSTA_BORRST
*                       @arg PMU_RSTSTA_SleepFlag
*                       @arg PMU_RSTSTA_HoldFlag


*
* 返回参数: ITStatus    = SET：  复位标志置起
*                       = RESET  复位标志未置起
*
* 特殊说明: 无
*********************************************************************************************************
*/
FlagStatus HT_ResetFlagStatusGet(uint32_t STAFlag)
{
    /*  assert_param  */
    if (HT_PMU->RSTSTA & STAFlag)
    {
        return SET;                        /*!< 产生相应唤醒标志          */
    }
    else
    {
        return RESET;                      /*!< 未产生相应唤醒标志        */
    }
}

/*
*********************************************************************************************************
*                               CLEAR SPECIFIED RESET FLAG STATUS
*
* 函数说明: 清除相应复位标志
*
* 入口参数: STAFlag    想要清除的某个复位标志，可以为以下参数:
*                       @arg PMU_RSTSTA           此项清除所有标志
*                       @arg PMU_RSTSTA_PORRST
*                       @arg PMU_RSTSTA_LBORRST
*                       @arg PMU_RSTSTA_WDTRST
*                       @arg PMU_RSTSTA_WakeupRST
*                       @arg PMU_RSTSTA_ExtRST
*                       @arg PMU_RSTSTA_SoftRST
*                       @arg PMU_RSTSTA_DebugRST
*                       @arg PMU_RSTSTA_BORRST
*                       @arg PMU_RSTSTA_SleepFlag
*                       @arg PMU_RSTSTA_HoldFlag
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_ResetFlagClear(uint32_t STAFlag)
{
    /*  assert_param  */
    HT_PMU->RSTSTA &= ~STAFlag;                     /*!< 清除相应复位标志          */
}

/*
*********************************************************************************************************
*                                     ENTER HOLD MODE
*
* 函数说明: 配置寄存器，使芯片进入Hold低功耗模式
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_EnterHold(void)
{
    SCB->SCR = 0x0000;
    __WFI();
}

/*
*********************************************************************************************************
*                                     ENTER SLEEP MODE
*
* 函数说明: 配置寄存器，使芯片进入Sleep低功耗模式
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_EnterSleep(void)
{
    SCB->SCR = 0x0004;
    __WFI();
}

/*
*********************************************************************************************************
*                                         FREE DOG
*
* 函数说明: 喂狗
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 这里设置了WatchDog溢出时间为约4s，用户可以根据实际需求改变，WatchDog溢出时间计算公式如下：
*           64ms * (1+WDTCLR[7:0])
*********************************************************************************************************
*/
void HT_FreeDog(void)
{
#if defined HT6x1x
    HT_WDT->WDTCFG = 0x0;
#endif
    HT_WDT->WDTCLR = 0xAA40;                                        /*!< 看门狗喂狗                   */
}

/*
*********************************************************************************************************
*                                    WATCHDOG CONFIGURE ON SLEEP/HOLD MODE
*
* 函数说明: Sleep/Hold模式下WatchDog配置
*
* 入口参数: NewState   = ENABLE： Sleep/Hold模式下WatchDog使能
*                      = DISABLE：Sleep/Hold模式下WatchDog关闭
*
* 返回参数: 无
*
* 特殊说明: 注意：此函数涉及到ClkCtrl0寄存器配置，用户应保证调用完此函数后，如果还需要配置ClkCtrl0寄存器，
*           应保证不会修改CMU_CLKCTRL0_WDTEN控制位
*********************************************************************************************************
*/
void HT_SHModeDogConfigure(FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
//        HT_RTC->CTRLBYFLASH  = 0x00;                               /*!< Hold/Sleep模式下使能LRC      */
#if  defined  HT6x1x                                                 /*!< HT6x1x               */
        HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_WDTEN, ENABLE);           /*!< Hold/Sleep模式下使能WatchDog */
#endif
    }
    else
    {
//        HT_RTC->CTRLBYFLASH  = 0x02;                               /*!< Hold/Sleep模式下关闭LRC      */
#if  defined  HT6x1x                                                 /*!< HT6x1x               */
        HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_WDTEN, DISABLE);          /*!< Hold/Sleep模式下关闭WatchDog */
#endif
    }
}


