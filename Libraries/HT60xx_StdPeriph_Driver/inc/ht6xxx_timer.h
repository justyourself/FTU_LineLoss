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
* Version      : V1.0.0
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
    TIMPWMCountUpLevelHigh = 0x02,               /*!< PWM向上计数方式，初始电平高    */
    TIMPWMCountDownLevelHigh = 0x12,             /*!< PWM向下计数方式，初始电平高    */
    TIMPWMCenterAlignedLevelHigh = 0x22,         /*!< PWM中央对齐计数方式，初始电平高*/
    TIMPWMCountUpLevelLow = 0x42,                /*!< PWM向上计数方式，初始电平低    */
    TIMPWMCountDownLevelLow	= 0x52,              /*!< PWM向下计数方式，初始电平低    */
    TIMPWMCenterAlignedLevelLow = 0x62,          /*!< PWM中央对齐计数方式，初始电平低*/
    TIMCaptureRiseEdge = 0x04,                   /*!< 上升沿捕获方式                 */
    TIMCaptureFallEdge = 0x0c,                   /*!< 下升沿捕获方式                 */
#if defined HT6x2x
    TIMEventRecordRiseEdge = 0x00,               /*!< 上升沿事件记录                 */
    TIMEventRecordFallEdge = 0x08,               /*!< 下升沿事件记录                 */    
#endif
    TIMCycleTiming = 0x06,                       /*!< 定时器周期定时                 */
}TMRMode_TypeDef;                                /*!< end of group TMRMode_TypeDef   */   

/* 
* @brief  TimerExt工作模式定义     only for HT6x2x  Timer4 & Timer5  
*/
#if defined HT6x2x
typedef enum
{ 
    TIMExtPWMCountUpLevelHigh = 0x02,            /*!< PWM向上计数方式，初始电平高    */
    TIMExtPWMCountDownLevelHigh = 0x12,          /*!< PWM向下计数方式，初始电平高    */
    TIMExtPWMCenterAlignedLevelHigh = 0x22,      /*!< PWM中央对齐计数方式，初始电平高*/
    TIMExtPWMCountUpLevelLow = 0x42,             /*!< PWM向上计数方式，初始电平低    */
    TIMExtPWMCountDownLevelLow	= 0x52,          /*!< PWM向下计数方式，初始电平低    */
    TIMExtPWMCenterAlignedLevelLow = 0x62,       /*!< PWM中央对齐计数方式，初始电平低*/
    TIMExtCycleTiming = 0x06,                    /*!< 定时器周期定时                 */
}TMRExtMode_TypeDef;                             /*!< end of group TMRExtMode_TypeDef*/ 

typedef enum
{ 
    TIMExtClk_LRC = TMR_TMRCON_CLKSEL_LRC,       /*!< Timer Clk时钟源为LRC                */
    TIMExtClk_LF  = TMR_TMRCON_CLKSEL_LF,        /*!< Timer Clk时钟源为LF                 */
    TIMExtClk_HRC = TMR_TMRCON_CLKSEL_HRC,       /*!< Timer Clk时钟源为HRC                */
    TIMExtClk_PLL = TMR_TMRCON_CLKSEL_PLL,       /*!< Timer Clk时钟源为PLL                */    
    TIMExtClk_MEMS= TMR_TMRCON_CLKSEL_MEMS,      /*!< Timer Clk时钟源为MEMS               */     
}TMRExtClkSource_TypeDef;                        /*!< end of group TMRExtClkSource_TypeDef*/ 
#endif

/* 
* @brief  Timer模块初始化结构定义
*/
typedef struct
{
    TMRMode_TypeDef TimerMode;                   /*!< 定时器工作模式                 */
    uint16_t TimerPreDiv;                        /*!< 定时器时钟预分频设置           */ 
    uint16_t TimerPeriod;                        /*!< 定时器定时周期                 */
    uint16_t TimerCompare;                       /*!< 定时器比较寄存器               */                      
}TMR_InitTypeDef;                                /*!< end of group TMR_InitTypeDef   */

/* 
* @brief  TimerExt模块初始化结构定义
*/
#if defined HT6x2x
typedef struct
{
    TMRExtClkSource_TypeDef TimerSource;         /*!< 定时器时钟源选择               */
    TMRExtMode_TypeDef TimerMode;                /*!< 定时器工作模式                 */
    uint16_t TimerPreDiv;                        /*!< 定时器时钟预分频设置           */ 
    uint16_t TimerPeriod;                        /*!< 定时器定时周期                 */     
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
#if defined HT6x2x
void HT_TMRExt_Init(HT_TMR_TypeDef* TMRx, TMRExt_InitTypeDef* TMR_InitStruct); /*only for HT6x2x Timer4&Timer5 */
#endif
uint16_t HT_TMR_CaptureGet(HT_TMR_TypeDef* TMRx);
uint16_t HT_TMR_CountGet(HT_TMR_TypeDef* TMRx);
void HT_TMR_ITConfig(HT_TMR_TypeDef* TMRx, uint8_t ITEn, FunctionalState NewState);
ITStatus HT_TMR_ITFlagStatusGet(HT_TMR_TypeDef* TMRx, uint8_t ITFlag);
void HT_TMR_ClearITPendingBit(HT_TMR_TypeDef* TMRx, uint8_t ITFlag);




#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_TIMER_H__ */ 
