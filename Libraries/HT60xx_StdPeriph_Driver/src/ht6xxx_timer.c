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
* File         : ht6xxx_timer.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.2
* Description  :
*********************************************************************************************************
*/

#define  __HT6XXX_TIMER_C

#include "ht6xxx_timer.h"

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
*                                         INITIAL TIMER MODULE
*
* 函数说明: 初始化TIMER模块
*
* 入口参数: TMRx               只能是HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3中一个
*
*           TMR_InitStruct     TIMER初始化结构体指针，主要包含4个参数:
*                              1) TimerMode    : 定时器工作模式
*                              2) TimerPeriod  : 定时器定时周期设置
*                              3) TimerCompare : 定时器比较寄存器设置
*                              4) TimerPreDiv  : 定时器预分频设置
*
* 返回参数: 无
*
* 特殊说明: 用户在配置TIMER寄存器前应先使能TIMER模块，具体参见HT_CMU_ClkCtrl1Config()函数
*********************************************************************************************************
*/
void HT_TMR_Init(HT_TMR_TypeDef* TMRx, TMR_InitTypeDef* TMR_InitStruct)
{
    /*  assert_param  */
    uint32_t tempreg;

    if ((TMRx == HT_TMR0) || (TMRx == HT_TMR1) || (TMRx == HT_TMR2) || (TMRx == HT_TMR3))
    {
        TMRx->TMRCON &= (~TMR_TMRCON_CNTEN);                    /*!< 关闭定时器使能             */
        TMRx->TMRDIV = TMR_InitStruct->TimerPreDiv;             /*!< 设置定时器预分频器          */
        TMRx->TMRPRD = TMR_InitStruct->TimerPeriod;             /*!< 设置定时器周期寄存器        */
        TMRx->TMRCMP = TMR_InitStruct->TimerCompare;            /*!< 设置定时器比较寄存器        */
        TMRx->TMRCNT = 0;                                       /*!< 清空定时器计数寄存器        */
        TMRx->TMRCAP = 0;                                       /*!< 清空定时器捕捉寄存器        */
#if  defined  HT6x3x
        TMRx->TMRCODT = TMR_InitStruct->PwmMode & TMR_TMRCODT;  /*!< PWM输出模式                */
        TMRx->TMRRDT = TMR_InitStruct->PwmDeadTime;             /*!< PWM死区时间寄存器          */
#endif
        tempreg = TMR_TMRCON_CNTEN;                             /*!< 定时器使能                 */
        tempreg |= (uint32_t)TMR_InitStruct->TimerMode;
        TMRx->TMRCON = tempreg;                                 /*!< 设置定时器工作模式          */
    }
}

/*
*********************************************************************************************************
*                                         INITIAL TIMEREXT MODULE
*
* 函数说明: 初始化TIMER模块
*
* 入口参数: TMRx               只能是HT_TMR4/HT_TMR5中一个
*
*           TMR_InitStruct     TIMER初始化结构体指针，主要包含4个参数:
*                              1) TimerSource  : 定时器时钟源选择
*                              2) TimerMode    : 定时器工作模式
*                              3) TimerPeriod  : 定时器定时周期设置
*                              4) TimerCompare : 定时器比较寄存器设置
*                              5) TimerPreDiv  : 定时器预分频设置
*
* 返回参数: 无
*
* 特殊说明: 用户在配置TIMER寄存器前应先使能TIMER模块，具体参见HT_CMU_ClkCtrl1Config()函数
*********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT6x3x
void HT_TMRExt_Init(HT_TMR_TypeDef* TMRx, TMRExt_InitTypeDef* TMR_InitStruct)
{
    /*  assert_param  */
    uint32_t tempreg;

    if ((TMRx == HT_TMR4) || (TMRx == HT_TMR5))
    {
        TMRx->TMRCON &= (~TMR_TMRCON_CNTEN);               /*!< 关闭定时器使能             */
        TMRx->TMRDIV = TMR_InitStruct->TimerPreDiv;        /*!< 设置定时器预分频器          */
        TMRx->TMRPRD = TMR_InitStruct->TimerPeriod;        /*!< 设置定时器周期寄存器        */
        TMRx->TMRCMP = TMR_InitStruct->TimerCompare;       /*!< 设置定时器比较寄存器        */
        TMRx->TMRCNT = 0;                                  /*!< 清空定时器计数寄存器        */
        tempreg = TMR_TMRCON_CNTEN;                        /*!< 定时器使能                 */
        tempreg |= ((uint32_t)TMR_InitStruct->TimerMode|(uint32_t)TMR_InitStruct->TimerSource);
        TMRx->TMRCON = tempreg;                            /*!< 设置定时器工作模式          */
    }
}
#endif

/*
*********************************************************************************************************
*                                         SET TIMER PERIOD VALUE
*
* 函数说明: 设置周期寄存器值并清空当前计数值
*
* 入口参数: TMRx               只能是HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5中一个
*
*           TimerPeriod        定时器周期值
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_TMR_PeriodSet(HT_TMR_TypeDef* TMRx, uint16_t TimerPeriod)
{
    /*  assert_param  */
    TMRx->TMRPRD = TimerPeriod;                             /*!< 设置定时器周期寄存器        */
    TMRx->TMRCNT = 0;                                       /*!< 清空定时器计数寄存器        */
}

/*
*********************************************************************************************************
*                                         SET TIMER COMPARE VALUE
*
* 函数说明: 设置比较寄存器值
*
* 入口参数: TMRx               只能是HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5中一个
*
*           TimerCompare       定时器比较值
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_TMR_CompareSet(HT_TMR_TypeDef* TMRx, uint16_t TimerCompare)
{
    /*  assert_param  */
    TMRx->TMRCMP = TimerCompare;                            /*!< 设置定时器比较寄存器        */
}

/*
*********************************************************************************************************
*                                         GET TIMER CAPTURE VALUE
*
* 函数说明: 获取捕获寄存器捕获值
*
* 入口参数: TMRx               只能是HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3中一个
*
* 返回参数: 返回定时器捕获值
*
* 特殊说明: 无
*********************************************************************************************************
*/
uint16_t HT_TMR_CaptureGet(HT_TMR_TypeDef* TMRx)
{
    /*  assert_param  */
    if ((TMRx == HT_TMR0) || (TMRx == HT_TMR1) || (TMRx == HT_TMR2) || (TMRx == HT_TMR3))
    {
        return  ((uint16_t)TMRx->TMRCAP);                /*!< 返回捕获寄存器捕获值        */
    }
    else
    {
        return 0xFFFF;
    }
}

/*
*********************************************************************************************************
*                                         GET TIMER COUNT VALUE
*
* 函数说明: 获取定时器当前定时值
*
* 入口参数: TMRx               只能是HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5中一个
*
* 返回参数: 返回定时器定时值
*
* 特殊说明: 无
*********************************************************************************************************
*/
uint16_t HT_TMR_CountGet(HT_TMR_TypeDef* TMRx)
{
    /*  assert_param  */
    return  ((uint16_t)TMRx->TMRCNT);                /*!< 返回定时器定时值         */
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE TIMER INTERRUPT
*
* 函数说明: 使能或者关闭TIMER中断
*
* 入口参数: TMRx       只能是HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5中一个
*
*           ITEn       TIMER中断设置位，可以为以下参数或其组合
*                        @arg TMR_TMRIE_PRDIE
*                        @arg TMR_TMRIE_CAPIE
*                        @arg TMR_TMRIE_CMPIE
*                        @arg TMR_TMRIE_ACIE    (only for HT502x, HT6x2x, HT6x3x)
*
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_TMR_ITConfig(HT_TMR_TypeDef* TMRx, uint16_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    if (NewState != DISABLE)
    {
        TMRx->TMRIE |= (uint32_t)ITEn;               /*!< 使能TIMER中断           */
    }
    else
    {
        TMRx->TMRIE &= ~(uint32_t)ITEn;              /*!< 关闭TIMER中断           */
    }
}

/*
*********************************************************************************************************
*                            GET SPECIFIED TIMER INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应TIMER中断标志状态
*
* 入口参数: TMRx       只能是HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5中一个
*
*           ITFlag     想要检查的某个TIMER中断，可以为以下参数:
*                        @arg TMR_TMRIF_PRDIF
*                        @arg TMR_TMRIF_CAPIF
*                        @arg TMR_TMRIF_CMPIF
*                        @arg TMR_TMRIF_ACIF    (only for HT6x2x)
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
*
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_TMR_ITFlagStatusGet(HT_TMR_TypeDef* TMRx, uint16_t ITFlag)
{
    /*  assert_param  */
    if (TMRx->TMRIF & ITFlag)
    {
        return SET;                        /*!< TIMER Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< TIMER Interrupt Flag is reset */
    }
}

/*
*********************************************************************************************************
*                                   CLEAR TIMER INTERRUPT FLAG
*
* 函数说明: 清除TIMER中断标志
*
* 入口参数: TMRx       只能是HT_TMR0/HT_TMR1/HT_TMR2/HT_TMR3/HT_TMR4/HT_TMR5中一个
*
*           ITFlag     想要清除的某个TIMER中断标志，可以为以下参数或其组合:
*                        @arg TMR_TMRIF_PRDIF
*                        @arg TMR_TMRIF_CAPIF
*                        @arg TMR_TMRIF_CMPIF
*                        @arg TMR_TMRIF_ACIF    (only for HT6x2x, HT502x)
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_TMR_ClearITPendingBit(HT_TMR_TypeDef* TMRx, uint16_t ITFlag)
{
    /*  assert_param  */
#if  defined  HT6x1x  ||  defined  HT501x                /*!< HT6x1x, HT501x                  */
    ITFlag |= 0x08;
#endif                                                   /*!< HT6x2x, HT502x                  */
    TMRx->TMRIF  &= ~((uint32_t)ITFlag);                 /*!< Clear TIMER Interrupt Flag      */
}
