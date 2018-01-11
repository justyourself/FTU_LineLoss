/*
**********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*
*
* Project      : HT6xxx
* File         : ht6xxx_timer.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.2
* Description  :
**********************************************************************************************************
*/

#ifndef __HT6XXX_TIMER_H__
#define __HT6XXX_TIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ht6xxx.h"

/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/

/*
* @brief  Timer工作模式定义
*/
typedef enum
{
    TIMPWMCountUpLevelHigh       = (TMR_TMRCON_PWMHL_HIGH|TMR_TMRCON_PWMC_UP|TMR_TMRCON_MODE_PWM),      /*!< PWM向上计数方式，初始电平高    */
    TIMPWMCountDownLevelHigh     = (TMR_TMRCON_PWMHL_HIGH|TMR_TMRCON_PWMC_DOWM|TMR_TMRCON_MODE_PWM),    /*!< PWM向下计数方式，初始电平高    */
    TIMPWMCenterAlignedLevelHigh = (TMR_TMRCON_PWMHL_HIGH|TMR_TMRCON_PWMC_CENTER|TMR_TMRCON_MODE_PWM),  /*!< PWM中央对齐计数方式，初始电平高*/
    TIMPWMCountUpLevelLow        = (TMR_TMRCON_PWMHL_LOW|TMR_TMRCON_PWMC_UP|TMR_TMRCON_MODE_PWM),       /*!< PWM向上计数方式，初始电平低    */
    TIMPWMCountDownLevelLow      = (TMR_TMRCON_PWMHL_LOW|TMR_TMRCON_PWMC_DOWM|TMR_TMRCON_MODE_PWM),     /*!< PWM向下计数方式，初始电平低    */
    TIMPWMCenterAlignedLevelLow  = (TMR_TMRCON_PWMHL_LOW|TMR_TMRCON_PWMC_CENTER|TMR_TMRCON_MODE_PWM),   /*!< PWM中央对齐计数方式，初始电平低*/
    TIMCaptureRiseEdge           = (TMR_TMRCON_CCMODE_RISE|TMR_TMRCON_MODE_CAPTURE),                    /*!< 上升沿捕获方式                */
    TIMCaptureFallEdge           = (TMR_TMRCON_CCMODE_FALL|TMR_TMRCON_MODE_CAPTURE),                    /*!< 下升沿捕获方式                */
#if  defined  HT6x1x  ||  defined  HT501x  ||  defined  HT502x
    TIMCycleTiming               = TMR_TMRCON_MODE_COMPARE,                                             /*!< 定时器周期定时                */
#else
    TIMCycleTiming               = TMR_TMRCON_MODE_TIMING,                                              /*!< 定时器周期定时                */
#endif
}TMRMode_TypeDef;                                /*!< end of group TMRMode_TypeDef   */

/*
* @brief  TimerPWM输出模式和极性     only for HT6x3x
*/
#if  defined  HT6x3x
typedef enum
{
    TIMPWMSyncOut                  = TMR_TMRCODT_SYNC_OUT,                     /*!< PWM模式下同步输出模式无死区    */
    TIMPWMCompOutDeadTimeLevelLow  = (TMR_TMRCODT_COMP_OUT|TMR_TMRCODT_DT_LL), /*!< PWM模式下互补输出模式死区低极性 */
    TIMPWMCompOutDeadTimeLevelHigh = (TMR_TMRCODT_COMP_OUT|TMR_TMRCODT_DT_HL), /*!< PWM模式下互补输出模式死区高极性 */
}TMRPwnMode_TypeDef;                             /*!< end of group TMRPwnMode_TypeDef */
#endif

/*
* @brief  TimerExt工作模式定义     only for HT6x2x & HT6x3x  Timer4 & Timer5
*/
#if defined HT6x2x  ||  defined  HT6x3x
typedef enum
{
    TIMExtPWMCountUpLevelHigh       = (TMR_TMRCON_PWMHL_HIGH|TMR_TMRCON_PWMC_UP|TMR_TMRCON_MODE_PWM),     /*!< PWM向上计数方式，初始电平高    */
    TIMExtPWMCountDownLevelHigh     = (TMR_TMRCON_PWMHL_HIGH|TMR_TMRCON_PWMC_DOWM|TMR_TMRCON_MODE_PWM),   /*!< PWM向下计数方式，初始电平高    */
    TIMExtPWMCenterAlignedLevelHigh = (TMR_TMRCON_PWMHL_HIGH|TMR_TMRCON_PWMC_CENTER|TMR_TMRCON_MODE_PWM), /*!< PWM中央对齐计数方式，初始电平高*/
    TIMExtPWMCountUpLevelLow        = (TMR_TMRCON_PWMHL_LOW|TMR_TMRCON_PWMC_UP|TMR_TMRCON_MODE_PWM),      /*!< PWM向上计数方式，初始电平低    */
    TIMExtPWMCountDownLevelLow      = (TMR_TMRCON_PWMHL_LOW|TMR_TMRCON_PWMC_DOWM|TMR_TMRCON_MODE_PWM),    /*!< PWM向下计数方式，初始电平低    */
    TIMExtPWMCenterAlignedLevelLow  = (TMR_TMRCON_PWMHL_LOW|TMR_TMRCON_PWMC_CENTER|TMR_TMRCON_MODE_PWM),  /*!< PWM中央对齐计数方式，初始电平低*/
    TIMExtCycleTiming               = TMR_TMRCON_MODE_TIMING,                                             /*!< 定时器周期定时                 */
}TMRExtMode_TypeDef;                             /*!< end of group TMRExtMode_TypeDef*/

typedef enum
{
    TIMExtClk_LRC = TMR_TMRCON_CLKSEL_LRC,       /*!< Timer Clk时钟源为LRC                */
    TIMExtClk_LF  = TMR_TMRCON_CLKSEL_LF,        /*!< Timer Clk时钟源为LF                 */
    TIMExtClk_HRC = TMR_TMRCON_CLKSEL_HRC,       /*!< Timer Clk时钟源为HRC                */
    TIMExtClk_PLL = TMR_TMRCON_CLKSEL_PLL,       /*!< Timer Clk时钟源为PLL                */
#if  defined  HT6x2x
    TIMExtClk_MEMS= TMR_TMRCON_CLKSEL_MEMS,      /*!< Timer Clk时钟源为MEMS               */
#endif
}TMRExtClkSource_TypeDef;                        /*!< end of group TMRExtClkSource_TypeDef*/
#endif

/*
* @brief  Timer模块初始化结构定义
*/
typedef struct
{
    TMRMode_TypeDef TimerMode;                   /*!< 定时器工作模式                 */
#if  defined  HT6x3x
    TMRPwnMode_TypeDef PwmMode;                  /*!< PWM输出模式                    */
    uint16_t PwmDeadTime;                        /*!< PWM死区时间                    */
#endif
    uint16_t TimerPreDiv;                        /*!< 定时器时钟预分频设置           */
    uint16_t TimerPeriod;                        /*!< 定时器定时周期                 */
    uint16_t TimerCompare;                       /*!< 定时器比较寄存器               */
}TMR_InitTypeDef;                                /*!< end of group TMR_InitTypeDef   */

/*
* @brief  TimerExt模块初始化结构定义
*/
#if defined HT6x2x  ||  defined  HT6x3x
typedef struct
{
    TMRExtClkSource_TypeDef TimerSource;         /*!< 定时器时钟源选择               */
    TMRExtMode_TypeDef TimerMode;                /*!< 定时器工作模式                 */
    uint16_t TimerPreDiv;                        /*!< 定时器时钟预分频设置           */
    uint16_t TimerPeriod;                        /*!< 定时器定时周期                 */
    uint16_t TimerCompare;                       /*!< 定时器比较寄存器               */
}TMRExt_InitTypeDef;                             /*!< end of group TMRExt_InitTypeDef*/
#endif

/*
*********************************************************************************************************
*                                             全局变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           全局函数申明
*********************************************************************************************************
*/

void HT_TMR_Init(HT_TMR_TypeDef* TMRx, TMR_InitTypeDef* TMR_InitStruct);
#if defined HT6x2x  ||  defined  HT6x3x
void HT_TMRExt_Init(HT_TMR_TypeDef* TMRx, TMRExt_InitTypeDef* TMR_InitStruct); /*only for HT6x2x & HT6x3x Timer4&Timer5 */
#endif
void HT_TMR_PeriodSet(HT_TMR_TypeDef* TMRx, uint16_t TimerPeriod);
void HT_TMR_CompareSet(HT_TMR_TypeDef* TMRx, uint16_t TimerCompare);
uint16_t HT_TMR_CaptureGet(HT_TMR_TypeDef* TMRx);
uint16_t HT_TMR_CountGet(HT_TMR_TypeDef* TMRx);
void HT_TMR_ITConfig(HT_TMR_TypeDef* TMRx, uint16_t ITEn, FunctionalState NewState);
ITStatus HT_TMR_ITFlagStatusGet(HT_TMR_TypeDef* TMRx, uint16_t ITFlag);
void HT_TMR_ClearITPendingBit(HT_TMR_TypeDef* TMRx, uint16_t ITFlag);


#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_TIMER_H__ */
