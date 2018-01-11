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
* File         : ht6x3x_emu.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : Only support HT6x3x
**********************************************************************************************************
*/

#ifndef __HT6X3X_EMU_H__
#define __HT6X3X_EMU_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ht6xxx.h"

#if defined  HT6x3x                                             /* This File Only support HT6x3x */

/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/
typedef enum
{
    EMU_PowerInQ = 0x00,                              /*!< 无功功率输入寄存器            */
    EMU_PowerInP = 0x01,                              /*!< 有功功率输入寄存器            */
    EMU_HFConst  = 0x02,                              /*!< 输出脉冲频率设置              */
    EMU_EnergyP  = 0x04,                              /*!< 有功能量计数寄存器            */
    EMU_EnergyQ  = 0x08,                              /*!< 无功能量计数寄存器            */
}EMUReg_TypeDef;                                /*!< end of group EMUReg_TypeDef   */

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
uint32_t HT_EMU_RegisterRead(EMUReg_TypeDef EMU_Register);
void HT_EMU_RegisterWrite(EMUReg_TypeDef EMU_Register,uint32_t RegisterValue);

#endif                                               /* This File Only support HT6x3x */

#ifdef __cplusplus
}
#endif

#endif /* __HT6X3X_EMU_H__ */
