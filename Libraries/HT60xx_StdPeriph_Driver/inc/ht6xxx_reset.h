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
* File         : ht6xxx_reset.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  :
**********************************************************************************************************
*/

#ifndef __HT6XXX_RESET_H__
#define __HT6XXX_RESET_H__

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
*********************************************************************************************************
*                                             全局变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           全局函数申明
*********************************************************************************************************
*/
FlagStatus HT_WakeFlagStatusGet(uint32_t STAFlag);
void HT_WakeFlagClear(uint32_t STAFlag);
FlagStatus HT_ResetFlagStatusGet(uint32_t STAFlag);
void HT_ResetFlagClear(uint32_t STAFlag);
void HT_EnterHold(void);
void HT_EnterSleep(void);
void HT_FreeDog(void);
void HT_SHModeDogConfigure(FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_RESET_H__ */
