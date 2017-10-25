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
* File         : ht6xxx_cmu.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_CMU_H__
#define __HT6XXX_CMU_H__

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
* @brief  系统时钟源选择定义
*/
typedef enum
{ 
    SysLRC = CMU_SYSCLKCFG_CLKSEL_LRC,            /*!< Fsys = 内部低频RC时钟            */
    SysLF  = CMU_SYSCLKCFG_CLKSEL_LF,             /*!< Fsys = 外部晶体时钟 or mems/16   */
    SysHRCDiv1 = (CMU_SYSCLKCFG_CLKSEL_HRC|0x00), /*!< Fsys = 内部高频RC时钟/1          */
    SysHRCDiv2 = (CMU_SYSCLKCFG_CLKSEL_HRC|0x10), /*!< Fsys = 内部高频RC时钟/2          */
    SysHRCDiv4 = (CMU_SYSCLKCFG_CLKSEL_HRC|0x20), /*!< Fsys = 内部高频RC时钟/4          */
    SysHRCDiv8 = (CMU_SYSCLKCFG_CLKSEL_HRC|0x30), /*!< Fsys = 内部高频RC时钟/8          */
    SysPLL = CMU_SYSCLKCFG_CLKSEL_PLL,            /*!< Fsys = 内部PLL时钟               */
#if  defined  HT6x1x ||  defined  HT6x2x
    SysMEMS= CMU_SYSCLKCFG_CLKSEL_MEMS,           /*!< Fsys = 内部mems时钟              */
#endif
#if defined HT501x || defined  HT6x2x  ||  defined  HT502x
    SysPLLX2= (CMU_SYSCLKCFG_CLKSEL_PLL|0x08),    /*!< Fsys = 内部PLL时钟 X 2           */
#endif
}SYSCLK_SelectTypeDef;                             /*!< end of group SYSCLK_SelectTypeDef*/  

/* 
* @brief  Clkout时钟源选择定义
*/
typedef enum
{ 
    ClkoutFLRC = CMU_CLKOUTSEL_LRC,     /*!< 内部低频RC时钟 ---> Clkout       */
    ClkoutFLF  = CMU_CLKOUTSEL_LF,      /*!< osc or mems/16 ---> Clkout       */
    ClkoutFHRC = CMU_CLKOUTSEL_HRC,     /*!< 外部高频RC时钟 ---> Clkout       */
    ClkoutFSys = CMU_CLKOUTSEL_SYS,     /*!< 内部PLL时钟    ---> Clkout       */
#if  defined  HT6x1x ||  defined  HT6x2x    
    ClkoutFMEMS= CMU_CLKOUTSEL_MEMS,    /*!< 内部mems时钟   ---> Clkout       */
#endif
}Clkout_SelectTypeDef;                  /*!< end of group Clkout_SelectTypeDef*/ 

/* 
* @brief  CPU时钟分频定义
*/
typedef enum
{ 
    CPUDiv1   = CMU_CPUCLKDIV_1,        /*!< Fcpu=Fsys                        */
    CPUDiv2   = CMU_CPUCLKDIV_2,        /*!< Fcpu=Fsys/2                      */
    CPUDiv4   = CMU_CPUCLKDIV_4,        /*!< Fcpu=Fsys/4                      */
    CPUDiv8   = CMU_CPUCLKDIV_8,        /*!< Fcpu=Fsys/8                      */
    CPUDiv16  = CMU_CPUCLKDIV_16,       /*!< Fcpu=Fsys/16                     */
    CPUDiv32  = CMU_CPUCLKDIV_32,       /*!< Fcpu=Fsys/32                     */
    CPUDiv64  = CMU_CPUCLKDIV_64,       /*!< Fcpu=Fsys/64                     */
    CPUDiv128 = CMU_CPUCLKDIV_128,      /*!< Fcpu=Fsys/128                    */    
}CPU_DivTypeDef;                        /*!< end of group CPU_DivTypeDef      */ 

/* 
* @brief  系统时钟配置模块初始化结构定义
*/
typedef struct
{
   SYSCLK_SelectTypeDef  SysClkSel;    /*!< 系统时钟选择                      */
   CPU_DivTypeDef        CPUDiv;       /*!< CPU时钟分频                       */
}CMU_InitTypeDef;                      /*!< end of group CMU_InitTypeDef      */

/* 
* @brief  系统时钟配置模块初始化结构定义
*/
typedef struct
{
   Clkout_SelectTypeDef  ClkoutSel;    /*!< Clkout时钟源选择                  */
   uint8_t  ClkoutDiv;                 /*!< Clkout输出时钟分频设置            
                                            ClkoutDiv 设置范围为：0~15
                                            分频系数 = 2*(ClkoutDiv[3..0]+1)  */
}Clkout_InitTypeDef;                   /*!< end of group Clkout_InitTypeDef   */


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
void HT_CMU_Init(CMU_InitTypeDef* CMU_InitStruct);
uint32_t HT_CMU_SysClkGet(void);
uint32_t HT_CMU_CPUClkGet(void);
void HT_CMU_ClkoutSet(Clkout_InitTypeDef* Clkout_InitStruct);
void HT_CMU_ClkCtrl0Config(uint32_t ClkCtrl0Module, FunctionalState NewState);
void HT_CMU_ClkCtrl1Config(uint32_t ClkCtrl1Module, FunctionalState NewState);
Bool HT_CMU_JTAGStatusGet(void);
FlagStatus HT_CMU_StopFlagGet(uint8_t STAFlag);
void	HT_CMU_FlashDly_Set(FunctionalState NewState);
void	HT_CMU_Prefetch_Set(FunctionalState NewState);
#ifdef __cplusplus
}
#endif

#endif /* __HT6xxx_CMU_H__ */ 
