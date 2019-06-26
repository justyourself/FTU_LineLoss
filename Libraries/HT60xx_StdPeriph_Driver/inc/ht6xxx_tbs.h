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
* File         : ht6xxx_tbs.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.4
* Description  :
**********************************************************************************************************
*/

#ifndef __HT6XXX_TBS_H__
#define __HT6XXX_TBS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ht6xxx.h"

/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/

typedef enum
{
    TMPPRD  = TBS_TBSPRD_TMPPRD,         /*!< 温度                                */
    VBATPRD = TBS_TBSPRD_VBATPRD,        /*!< 电池电压                            */
    ADC0PRD = TBS_TBSPRD_ADC0PRD,        /*!< ADC0                                */
    ADC1PRD = TBS_TBSPRD_ADC1PRD,        /*!< ADC1                                */
    VCCPRD  = TBS_TBSPRD_VCCPRD,         /*!< 电源电压                            */
#if  defined  HT501x ||  defined  HT502x
    ADC2PRD = TBS_TBSPRD_ADC2PRD,        /*!< ADC2                                */
#endif
#if  defined  HT501x
    VREFPRD = TBS_TBSPRD_VREFPRD,        /*!< Vref                                */
#endif
}TBS_PeriodTypeDef;                      /*!< end of group TBS_PeriodTypeDef      */

typedef enum
{
    TBS_TMP  = 0,                        /*!< 温度                                */
    TBS_VBAT ,                           /*!< 电池电压                            */
    TBS_ADC0 ,                           /*!< ADC0                                */
    TBS_ADC1 ,                           /*!< ADC1                                */
    TBS_VCC  ,                           /*!< 电源电压                            */
#if  defined  HT501x  ||  defined  HT502x
    TBS_ADC2 ,                           /*!< ADC2                                */
#endif
}TBS_SubModuleTypeDef;                   /*!< end of group TBS_SubModuleTypeDef   */

typedef enum
{
    CMP_VBAT =0,                         /*!< 电池电压                            */
#if  defined  HT6x2x  ||  defined  HT6x3x  ||  defined  HT501x  ||  defined  HT502x
    CMP_ADC0 ,                           /*!< ADC0                                */
#endif
#if  defined  HT501x  ||  defined  HT502x
    CMP_ADC1 ,                           /*!< ADC1                                */
#endif
}TBS_CMPRegTypeDef;                      /*!< end of group TBS_CMPRegTypeDef      */

typedef enum                             /* 目前只限温度测量 */
{
    TBS_ADCFilterCnt_1 = 0x0000,         /*!< ADC输出 1次滤波                      */
    TBS_ADCFilterCnt_2 = 0x0100,         /*!< ADC输出 2次滤波                      */
    TBS_ADCFilterCnt_4 = 0x0200,         /*!< ADC输出 4次滤波                      */
    TBS_ADCFilterCnt_8 = 0x0300,         /*!< ADC输出 8次滤波                      */
}TBS_ADCFilterCntTypeDef;                /*!< end of group TBS_ADCFilterCntTypeDef */

#if  defined  HT6x3x
typedef enum
{
    TBS_CLKSEL_OSC    = 0x0000,          /*!< 时钟源选为OSC                        */
    TBS_CLKSEL_PLLDIV = 0x0020,          /*!< 时钟源选为PLL/168                    */
}TBS_ClkSelectTypeDef;                   /*!< end of group TBS_ClkSelectTypeDef   */

typedef enum
{
    TBS_TRI_TMP  = TBS_TRIREQ_TMPTRI,    /*!< 快速触发温度采样                     */
    TBS_TRI_VBAT = TBS_TRIREQ_VBATTRI,   /*!< 快速触发电池采样                     */
    TBS_TRI_ADC0 = TBS_TRIREQ_ADC0TRI,   /*!< 快速触发ADC0采样                     */
    TBS_TRI_ADC1 = TBS_TRIREQ_ADC1TRI,   /*!< 快速触发ADC1采样                     */
    TBS_TRI_VCC  = TBS_TRIREQ_VCCTRI,    /*!< 快速触发VCC采样                      */
    TBS_TRI_ADC3 = TBS_TRIREQ_ADC3TRI,   /*!< 快速触发ADC3采样                     */
    TBS_TRI_ADC4 = TBS_TRIREQ_ADC4TRI,   /*!< 快速触发ADC4采样                     */
    TBS_TRI_ADC5 = TBS_TRIREQ_ADC5TRI,   /*!< 快速触发ADC5采样                     */
}TBS_TriChnTypeDef;                      /*!< end of group TBS_TriChnTypeDef      */

typedef enum
{
    TBS_CLKSEL_SUCC       = 0,           /*!< TBS时钟选择修改成功                   */
    TBS_CLKSEL_PARA_ERR   = -1,          /*!< TBS时钟选择修改失败：输入参数错误     */
    TBS_CLKSEL_PLL_EN_ERR = -2,          /*!< TBS时钟选择修改失败：PLL未打开        */
}TBS_ClkSelReturnTypeDef;                /*!< end of group TBS_ClkSelReturnTypeDef*/
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

void HT_TBSConfig(uint32_t TBS_SubModule, FunctionalState NewState);
void HT_TBS_PeriodSet(TBS_PeriodTypeDef PRDModule, uint32_t Period);
void HT_TBS_CMPThresholdSet(TBS_CMPRegTypeDef CmpReg, int16_t Threshold);
int16_t HT_TBS_ValueRead(TBS_SubModuleTypeDef SubModule);
void HT_TBS_ITConfig(uint32_t ITEn, FunctionalState NewState);
ITStatus HT_TBS_ITFlagStatusGet(uint32_t ITFlag);
void HT_TBS_ClearITPendingBit(uint32_t ITFlag);
void HT_TBS_FilCntConfig(TBS_ADCFilterCntTypeDef FilterVal);

#if  defined  HT6x3x
TBS_ClkSelReturnTypeDef HT_TBS_ClkSelConfig(TBS_ClkSelectTypeDef ClkSel);
void HT_TBS_ADCTriSample(TBS_TriChnTypeDef TriChn);
#endif


#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_TBS_H__ */
