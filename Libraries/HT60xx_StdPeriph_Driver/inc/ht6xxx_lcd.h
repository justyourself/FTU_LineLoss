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
* File         : ht6xxx_lcd.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  :
**********************************************************************************************************
*/

#ifndef __HT6XXX_LCD_H__
#define __HT6XXX_LCD_H__

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
* @brief  LCD偏压驱动选择位定义
*/
typedef enum
{
    LCDBias_OneThird = LCD_LCDCLK_BIASCTL_3,  /*!< 1/3 bias 偏压驱动              */
    LCDBias_OneFourth = LCD_LCDCLK_BIASCTL_4, /*!< 1/4 bias 偏压驱动              */
}LCDBias_TypeDef;                          /*!< end of group LCDBias_TypeDef   */

/*
* @brief  LCDCom定义
*/
typedef enum
{
    LCD_4COM = LCD_LCDCLK_DUTY_COM4,       /*!< 4Com                           */
    LCD_6COM = LCD_LCDCLK_DUTY_COM6,       /*!< 6Com                           */
    LCD_8COM = LCD_LCDCLK_DUTY_COM8,       /*!< 8Com                           */
}LCDCom_TypeDef;                           /*!< end of group LCDCom_TypeDef    */

/*
* @brief  LCD扫描频率选择位定义
*/
typedef enum
{
    LCDCLK_Div1 = LCD_LCDCLK_LCLK_FRAME128HZ, /*!< LCD扫描频率最高 = 128Hz     */
    LCDCLK_Div2 = LCD_LCDCLK_LCLK_FRAME64HZ,  /*!< LCD扫描频率最高/2           */
    LCDCLK_Div4 = LCD_LCDCLK_LCLK_FRAME32HZ,  /*!< LCD扫描频率最高/4           */
    LCDCLK_Div8 = LCD_LCDCLK_LCLK_FRAME16HZ,  /*!< LCD扫描频率最高/8           */
    LCDCLK_Div16 = LCD_LCDCLK_LCLK_FRAME8HZ,  /*!< LCD扫描频率最高/16          */
}LCDCLK_TypeDef;                           /*!< end of group LCDCLK_TypeDef    */

/*
* @brief  LCD充电模式定义
*/
typedef enum
{
    LCDCharge_HighCurrent = 0,                                                                                        /*!< LCD大电流充电模式                   */
    LCDCharge_LittleCurrent = ((~LCD_LCDCON_FC)&LCD_LCDCON_LC),                                                       /*!< LCD小电流充电模式                   */
#if  defined  HT502x  ||  defined  HT6x3x
    LCDChargeQuick_PeriodDiv16_ResDef = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_16|LCD_LCDCON_FCSET2_KEEPR),    /*!< 快速充电模式，充电时间1/16个Flcd周期，电阻默认  */
    LCDChargeQuick_PeriodDiv32_ResDef = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_32|LCD_LCDCON_FCSET2_KEEPR),    /*!< 快速充电模式，充电时间1/32个Flcd周期，电阻默认  */
    LCDChargeQuick_PeriodDiv64_ResDef = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_64|LCD_LCDCON_FCSET2_KEEPR),    /*!< 快速充电模式，充电时间1/64个Flcd周期，电阻默认  */
    LCDChargeQuick_PeriodDiv128_ResDef = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_128|LCD_LCDCON_FCSET2_KEEPR),  /*!< 快速充电模式，充电时间1/128个Flcd周期，电阻默认 */
    LCDChargeQuick_PeriodDiv16_ResChg = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_16|LCD_LCDCON_FCSET2_CHANGER),  /*!< 快速充电模式，充电时间1/16个Flcd周期，电阻改善  */
    LCDChargeQuick_PeriodDiv32_ResChg = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_32|LCD_LCDCON_FCSET2_CHANGER),  /*!< 快速充电模式，充电时间1/32个Flcd周期，电阻改善  */
    LCDChargeQuick_PeriodDiv64_ResChg = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_64|LCD_LCDCON_FCSET2_CHANGER),  /*!< 快速充电模式，充电时间1/64个Flcd周期，电阻改善  */
    LCDChargeQuick_PeriodDiv128_ResChg = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_128|LCD_LCDCON_FCSET2_CHANGER),/*!< 快速充电模式，充电时间1/128个Flcd周期，电阻改善 */
#else
    LCDChargeQuick_PeriodDiv8 = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_8),   /*!< 快速充电模式，充电时间1/8个Flcd周期 */
    LCDChargeQuick_PeriodDiv16 = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_16), /*!< 快速充电模式，充电时间1/16个Flcd周期*/
    LCDChargeQuick_PeriodDiv32 = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_32), /*!< 快速充电模式，充电时间1/32个Flcd周期*/
    LCDChargeQuick_PeriodDiv64 = (LCD_LCDCON_FC|LCD_LCDCON_LC|LCD_LCDCON_FCSET_64), /*!< 快速充电模式，充电时间1/64个Flcd周期*/
#endif
}LCDChargeMode_TypeDef;                    /*!< end of group LCDChargeMode_TypeDef  */

/* HT502x和HT6x3x 向下兼容重定义 */
#if  defined  HT502x  ||  defined  HT6x3x
#define LCDChargeQuick_PeriodDiv8       LCDChargeQuick_PeriodDiv16_ResDef
#define LCDChargeQuick_PeriodDiv16      LCDChargeQuick_PeriodDiv32_ResDef
#define LCDChargeQuick_PeriodDiv32      LCDChargeQuick_PeriodDiv64_ResDef
#define LCDChargeQuick_PeriodDiv64      LCDChargeQuick_PeriodDiv128_ResDef
#endif

#if  defined  HT6x3x
/*
* @brief  LCD chargepump时钟选择
*/
typedef enum
{
    LCDDriveMode_InsideRes   = LCD_LCDCPC_MODSET_InsideRESDrive,  /*!< 内部电阻分压驱动 */
    LCDChargePump_ChargePump = LCD_LCDCPC_MODSET_ChargePumpDrive, /*!< Charge Pump驱动 */
}LCDDriverMode_TypeDef;                    /*!< end of group LCDDriverMode_TypeDef  */

/*
* @brief  LCD chargepump时钟选择
*/
typedef enum
{
    LCDChargePump_Clk256 = LCD_LCDCPC_CCLKSEL_256,  /*!< Chargepump时钟为256Hz */
    LCDChargePump_Clk512 = LCD_LCDCPC_CCLKSEL_512,  /*!< Chargepump时钟为512Hz */
    LCDChargePump_Clk1K  = LCD_LCDCPC_CCLKSEL_1K,   /*!< Chargepump时钟为1kHz  */
    LCDChargePump_Clk2K  = LCD_LCDCPC_CCLKSEL_2K,   /*!< Chargepump时钟为2kHz  */
    LCDChargePump_Clk4K  = LCD_LCDCPC_CCLKSEL_4K,   /*!< Chargepump时钟为4kHz  */
    LCDChargePump_Clk8K  = LCD_LCDCPC_CCLKSEL_8K,   /*!< Chargepump时钟为8kHz  */
    LCDChargePump_Clk16K = LCD_LCDCPC_CCLKSEL_16K,  /*!< Chargepump时钟为16kHz */
    LCDChargePump_Clk32K = LCD_LCDCPC_CCLKSEL_32K,  /*!< Chargepump时钟为32kHz */
}LCDChargePumpClk_TypeDef;                    /*!< end of group LCDChargePumpClk_TypeDef  */
#endif

/*
* @brief  LCD模块初始化结构定义
*/
typedef struct
{
    LCDBias_TypeDef LCD_Bias;              /*!< LCD偏压驱动定义                     */
    LCDCom_TypeDef  LCD_Com;               /*!< LCDCom Number定义                   */
    LCDCLK_TypeDef  LCD_CLK;               /*!< LCD扫描频率定义                     */
    LCDChargeMode_TypeDef LCD_ChargeMode;  /*!< LCD充电模式定义                     */
    uint16_t LCD_Vrsel;                    /*!< LCD_Vrsel[3..0]: 对比度设置         */
}LCD_InitTypeDef;                          /*!< end of group LCD_InitTypeDef        */

#if  defined  HT6x3x
/*
* @brief  LCD chargepump初始化结构定义
*/
typedef struct
{
    LCDChargePumpClk_TypeDef LCD_ChargePumpClk; /*!< LCD偏压驱动定义                     */
    LCDDriverMode_TypeDef    LCD_DriverMode;    /*!< LCDCom Number定义                   */
    uint16_t LCD_Vcsel;                         /*!< LCD_Vcsel[4..0]: 对比度设置         */
}LCD_CPInitTypeDef;
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

void HT_LCD_Init(LCD_InitTypeDef* LCD_InitStruct);
void HT_LCD_Write(uint8_t SegNum, uint8_t SegValue);
uint8_t HT_LCD_Read(uint8_t SegNum);

#if  defined  HT502x  ||  defined  HT6x3x
void HT_LCD_OutConfig(FunctionalState NewState);
#endif

#if  defined  HT6x3x
void HT_LCD_CP_Init(LCD_CPInitTypeDef* LCD_CPInitStruct);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_LCD_H__ */
