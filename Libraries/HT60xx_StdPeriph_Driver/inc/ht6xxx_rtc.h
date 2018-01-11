/*
*********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*
*
* Project      : HT6xxx
* File         : ht6xxx_rtc.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.4
* Description  :
**********************************************************************************************************
*/

#ifndef __HT6XXX_RTC_H__
#define __HT6XXX_RTC_H__

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
* @brief  RTC Timer定义
*/
typedef enum
{
    RTCTMR1 = RTC_RTCCON_RTC1EN,   /*!< RTC定时器1                        */
    RTCTMR2 = RTC_RTCCON_RTC2EN,   /*!< RTC定时器2                        */
}RTCTMR_SelectTypeDef;             /*!< end of group RTCTMR_SelectTypeDef */

/*
* @brief  RTC Timer初始化定义
*/
typedef struct
{
    FunctionalState NewState;      /*!< 定时器状态设置：使能or关闭        */
    uint16_t  Count;               /*!< 定时器定时周期设置                */
}RTCTMR_InitTypeDef;               /*!< end of group RTCTMR_InitTypeDef   */

/*
* @brief  Tout输出定义
*/
typedef enum
{
    ToutLOW    = RTC_RTCCON_TOUT_LOW,
    ToutHigh   = RTC_RTCCON_TOUT_HIGH,
    ToutLF     = RTC_RTCCON_TOUT_LF,
    Tout1Hz    = RTC_RTCCON_TOUT_1HZ,
    Tout2Hz    = RTC_RTCCON_TOUT_2HZ,
    Tout4Hz    = RTC_RTCCON_TOUT_4HZ,
    Tout8Hz    = RTC_RTCCON_TOUT_8HZ,
#if defined  HT6x1x ||  defined  HT6x2x  ||  defined  HT6x3x
    Tout16Hz   = RTC_RTCCON_TOUT_16HZ,
    Tout32Hz   = RTC_RTCCON_TOUT_32HZ,
    Tout64Hz   = RTC_RTCCON_TOUT_64HZ,
    Tout128Hz  = RTC_RTCCON_TOUT_128HZ,
#endif
}RTCTout_TypeDef;                  /*!< end of group RTCTout_TypeDef  */

/*
* @brief  闹钟设置定义
*/
typedef union
{
    struct
    {
        uint8_t minute;            /*!< 闹钟寄存器中分钟设置          */
        uint8_t hour;              /*!< 闹钟寄存器中小时设置          */
    }Muster;
    uint16_t HourAndMinute;
}RTCAlarm_TypeDef;                 /*!< end of group RTCAlarm_TypeDef */

/*
* @brief  RTC模块初始化结构定义
*/
typedef struct
{
    RTCTMR_InitTypeDef RTC_TMR1;   /*!< RTC定时器1设置                */
    RTCTMR_InitTypeDef RTC_TMR2;   /*!< RTC定时器2设置                */
    RTCAlarm_TypeDef RTC_Alarm;    /*!< RTC闹钟设置                   */
    RTCTout_TypeDef RTC_TOUT;      /*!< RTC Tout输出设置              */
}RTC_InitTypeDef;                  /*!< end of group RTC_InitTypeDef  */


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

void HT_RTC_Init(RTC_InitTypeDef* RTC_InitStruct);
void HT_RTC_Read(void* pRTCTime);
void HT_RTC_Write(void* pRTCTime);
void HT_RTC_TMRConfig(RTCTMR_SelectTypeDef RTCTMRx, RTCTMR_InitTypeDef* RTCTMR_InitStruct);
void HT_RTC_ToutSet(RTCTout_TypeDef RTCToutSet);
void HT_RTC_AlarmSet(RTCAlarm_TypeDef RTCAlarmSet);
void HT_RTC_ITConfig(uint16_t ITEn, FunctionalState NewState);
ITStatus HT_RTC_ITFlagStatusGet(uint16_t ITFlag);
void HT_RTC_ClearITPendingBit(uint16_t ITFlag);
Bool HT_RTC_LoadInfoData(void);

#if defined  HT501x  ||  defined  HT502x  ||  defined  HT6x2x  ||  defined  HT6x3x
void HT_RTC_LRC_Amend(uint32_t TimeCalVal);
void HT_RTC_LRC_Read(void* pRTCTime);
void HT_RTC_LRC_Write(void* pRTCTime);
#endif


#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_RTC_H__ */
