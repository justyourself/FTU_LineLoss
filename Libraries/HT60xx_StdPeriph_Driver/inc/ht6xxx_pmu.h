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
* File         : ht6xxx_pmu.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_PMU_H__
#define __HT6XXX_PMU_H__

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
* @brief  LVDIN状态定义
*/
typedef enum
{ 
    LVDINEnable = 0x04,                   /*!< LVDIN使能                        */
    LVDINDisable = 0x00,                  /*!< LVDIN关闭                        */
}LVDINState_TypeDef;                      /*!< end of group LVDIN_StateTypeDef  */   

/* 
* @brief  BOR状态定义
*/
typedef enum
{ 
    BORDisable = 0x00,                    /*!< BOR关闭                          */
    BORReset = 0x03,                      /*!< BOR使能，产生复位                */
    BORInterrupt = 0x01,                  /*!< BOR使能，产生中断                */
}BORState_TypeDef;                        /*!< end of group BOR_StateTypeDef    */

/* 
* @brief  VCC阈值定义
*/
typedef enum
{ 
    VccLVL_2V4 = PMU_VDETCFG_VCCLVL_2V4,  /*!< Vcc检测阈值 = 2.4V               */
    VccLVL_2V6 = PMU_VDETCFG_VCCLVL_2V6,  /*!< Vcc检测阈值 = 2.6V               */
    VccLVL_2V8 = PMU_VDETCFG_VCCLVL_2V8,  /*!< Vcc检测阈值 = 2.8V               */
    VccLVL_3V0 = PMU_VDETCFG_VCCLVL_3V0,  /*!< Vcc检测阈值 = 3.0V               */
#if  defined  HT6x1x ||  defined  HT6x2x    
    VccLVL_3V2 = PMU_VDETCFG_VCCLVL_3V2,  /*!< Vcc检测阈值 = 3.2V               */
    VccLVL_3V4 = PMU_VDETCFG_VCCLVL_3V4,  /*!< Vcc检测阈值 = 3.4V               */
    VccLVL_3V6 = PMU_VDETCFG_VCCLVL_3V6,  /*!< Vcc检测阈值 = 3.6V               */
    VccLVL_3V8 = PMU_VDETCFG_VCCLVL_3V8,  /*!< Vcc检测阈值 = 3.8V               */
    VccLVL_4V0 = PMU_VDETCFG_VCCLVL_4V0,  /*!< Vcc检测阈值 = 4.0V               */
    VccLVL_4V2 = PMU_VDETCFG_VCCLVL_4V2,  /*!< Vcc检测阈值 = 4.2V               */    
    VccLVL_4V4 = PMU_VDETCFG_VCCLVL_4V4,  /*!< Vcc检测阈值 = 4.4V               */
    VccLVL_4V6 = PMU_VDETCFG_VCCLVL_4V6,  /*!< Vcc检测阈值 = 4.6V               */
    VccLVL_4V8 = PMU_VDETCFG_VCCLVL_4V8,  /*!< Vcc检测阈值 = 4.8V               */ 
    VccLVL_5V0 = PMU_VDETCFG_VCCLVL_5V0,  /*!< Vcc检测阈值 = 5.0V               */ 
#endif
}VCCLVL_TypeDef;                          /*!< end of group VCCLVL_TypeDef      */

/* 
* @brief  BOR阈值定义
*/
typedef enum
{
#if !defined HT6x2x	
    BORLVL_2V2 = PMU_VDETCFG_BORLVL_2V2,  /*!< BOR检测阈值 = 2.2V               */
    BORLVL_2V4 = PMU_VDETCFG_BORLVL_2V4,  /*!< BOR检测阈值 = 2.4V               */
    BORLVL_2V6 = PMU_VDETCFG_BORLVL_2V6,  /*!< BOR检测阈值 = 2.6V               */
    BORLVL_2V8 = PMU_VDETCFG_BORLVL_2V8,  /*!< BOR检测阈值 = 2.8V               */ 
#else
    BORLVL_2V2 = PMU_VDETCFG_BORLVL_2V0,  /*!< BOR检测阈值 = 2.0V               */
    BORLVL_2V4 = PMU_VDETCFG_BORLVL_2V2,  /*!< BOR检测阈值 = 2.2V               */
    BORLVL_2V6 = PMU_VDETCFG_BORLVL_2V6,  /*!< BOR检测阈值 = 2.6V               */
    BORLVL_2V8 = PMU_VDETCFG_BORLVL_2V8,  /*!< BOR检测阈值 = 2.8V               */	
#endif
}BOTLVL_TypeDef;                          /*!< end of group BOTLVL_TypeDef      */

/* 
* @brief  VCC_DET,BOR_DET分时检测时间定义
*/
typedef enum
{ 
    DETTIM_300uS = PMU_VDETPCFG_VDETTIME_300uS,  /*!< 分时检测时间 = 300us        */
    DETTIM_360uS = PMU_VDETPCFG_VDETTIME_360uS,  /*!< 分时检测时间 = 360us        */
    DETTIM_480uS = PMU_VDETPCFG_VDETTIME_480uS,  /*!< 分时检测时间 = 480us        */
#if defined HT6x2x
    DETTIM_1120uS = PMU_VDETPCFG_VDETTIME_1120uS,  /*!< 分时检测时间 = 1120us        */
#else
    DETTIM_560uS = PMU_VDETPCFG_VDETTIME_560uS,   /*!< 分时检测时间 = 560us        */
#endif	
}DETTIM_TypeDef;                                 /*!< end of group DETTIM_TypeDef */

/* 
* @brief  VCC_DET,BOR_DET分时检测周期定义
*/
typedef enum
{ 
    DETPRD_16P5mS = PMU_VDETPCFG_VDETPRD_16P5mS, /*!< 分时检测周期 = 16.5ms       */
    DETPRD_33mS   = PMU_VDETPCFG_VDETPRD_33mS,   /*!< 分时检测周期 = 33ms         */
    DETPRD_67mS   = PMU_VDETPCFG_VDETPRD_67mS,   /*!< 分时检测周期 = 67ms         */
    DETPRD_134mS  = PMU_VDETPCFG_VDETPRD_134mS,  /*!< 分时检测周期 = 134ms        */ 
    DETPRD_268mS  = PMU_VDETPCFG_VDETPRD_268mS,  /*!< 分时检测周期 = 268ms        */
    DETPRD_536mS  = PMU_VDETPCFG_VDETPRD_536mS,  /*!< 分时检测周期 = 536ms        */
    DETPRD_1072mS = PMU_VDETPCFG_VDETPRD_1072mS, /*!< 分时检测周期 = 1072ms       */
    DETPRD_2144mS = PMU_VDETPCFG_VDETPRD_2144mS, /*!< 分时检测周期 = 2144ms       */
}DETPRD_TypeDef;                                 /*!< end of group DETPRD_TypeDef */

/* 
* @brief  DISCharge模式定义
*/
#if  defined  HT6x2x  ||  defined HT501x  ||  HT502x
typedef enum
{ 
    DisCharge0mA =0,                            /*!< 关闭电池钝化功能              */
    DisCharge1mA =0x8000,                       /*!< 电池钝化使能，电流1mA         */
    DisCharge2mA =0x88000,                      /*!< 电池钝化使能，电流2mA         */
	  DisCharge50uA =0x48000,                     /*!< 电池钝化使能，电流50uA        */
	  DisCharge100uA =0xC8000,                    /*!< 电池钝化使能，电流100uA       */
}DISCharge_TypeDef;                             /*!< end of group DISCharge_TypeDef*/
#endif

/* 
* @brief  POW状态定义
*/
#if  defined HT6x2x
typedef enum
{ 
    POWEnable = 0x20,                   /*!< POW_DET使能                        */
    POWDisable = 0x00,                  /*!< POW_DET关闭                        */
}POWState_TypeDef;                      /*!< end of group POWState_TypeDef			 */   
#endif

/* 
* @brief  PMU模块初始化结构定义
*/
typedef struct
{
    LVDINState_TypeDef Lvdin0State;              /*!< LVDIN0状态设置              */
#if  defined  HT501x ||  defined  HT6x2x  ||  defined  HT502x
    LVDINState_TypeDef Lvdin1State;              /*!< LVDIN1状态设置              */
#endif  
#if  defined  HT6x2x
	POWState_TypeDef POWState;				 	 /*!< POW状态设置   	           */		
	uint32_t	POWFilter;						 /*!< POW数字滤波设置            */	
#endif	
    BORState_TypeDef BorState;                   /*!< BOR状态设置                 */
    VCCLVL_TypeDef VccLvlSet;                    /*!< Vcc检测阈值设置             */
    BOTLVL_TypeDef BorLvlSet;                    /*!< BOR检测阈值设置             */
    DETTIM_TypeDef VccBorDetTime;                /*!< Vcc和Bor检测时间设置        */
    DETPRD_TypeDef VccBorDetPeriod;              /*!< Vcc和Bor检测周期设置        */                                                         
}PMU_InitTypeDef;                                /*!< end of group PMU_InitTypeDef*/


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
void HT_PMU_Init(PMU_InitTypeDef* PMU_InitStruct);
void HT_PMU_HoldLDOConfig(FunctionalState NewState);
FlagStatus HT_PMUState_FlagStatusGet(uint8_t STAFlag);
void HT_PMU_ITConfig(uint8_t ITEn, FunctionalState NewState);
ITStatus HT_PMU_ITFlagStatusGet(uint8_t ITFlag);
void HT_PMU_ClearITPendingBit(uint8_t ITFlag);
#if  defined  HT501x ||  defined  HT6x2x 
void HT_PMU_DisChargeConfig(DISCharge_TypeDef ChargeMode);
#endif



#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_PMU_H__ */ 
