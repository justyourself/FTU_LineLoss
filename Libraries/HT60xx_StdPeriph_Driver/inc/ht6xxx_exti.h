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
* File         : ht6xxx_exti.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  :
**********************************************************************************************************
*/

#ifndef __HT6XXX_EXTI_H__
#define __HT6XXX_EXTI_H__

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

void HT_EXTIRise_ITConfig(uint32_t ITEn, FunctionalState NewState);
void HT_EXTIFall_ITConfig(uint32_t ITEn, FunctionalState NewState);
ITStatus HT_EXTIRise_ITFlagStatusGet(uint32_t ITFlag);
ITStatus HT_EXTIFall_ITFlagStatusGet(uint32_t ITFlag);
void HT_EXTIRise_ClearITPendingBit(uint32_t ITFlag);
void HT_EXTIFall_ClearITPendingBit(uint32_t ITFlag);
void HT_EXTI_FilterConfig(uint32_t ITFLTEn,FunctionalState NewState);


#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_EXTI_H__ */
