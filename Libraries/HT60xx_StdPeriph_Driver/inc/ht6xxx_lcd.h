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
* File         : ht6xxx_lcd.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
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
    LCDBias_OneThird = 0x80,               /*!< 1/3 bias 偏压驱动              */
    LCDBias_OneFourth = 0x00,              /*!< 1/4 bias 偏压驱动              */
}LCDBias_TypeDef;                          /*!< end of group LCDBias_TypeDef   */  

/* 
* @brief  LCDCom定义
*/
typedef enum
{ 
    LCD_4COM = 0x00,                       /*!< 4Com                           */
    LCD_6COM = 0x08,                       /*!< 6Com                           */
    LCD_8COM = 0x10,                       /*!< 8Com                           */
}LCDCom_TypeDef;                           /*!< end of group LCDCom_TypeDef    */ 

/* 
* @brief  LCD扫描频率选择位定义
*/
typedef enum
{ 
    LCDCLK_Div1 = 0x04,                    /*!< LCD扫描频率最高                */
    LCDCLK_Div2 = 0x00,                    /*!< LCD扫描频率/2                  */
    LCDCLK_Div4 = 0x01,                    /*!< LCD扫描频率/4                  */
    LCDCLK_Div8 = 0x02,                    /*!< LCD扫描频率/8                  */
    LCDCLK_Div16 = 0x03,                   /*!< LCD扫描频率/16                 */
}LCDCLK_TypeDef;                           /*!< end of group LCDCLK_TypeDef    */

/* 
* @brief  LCD充电模式定义
*/
typedef enum
{ 
    LCDCharge_HighCurrent = 0x00,          /*!< LCD大电流充电模式                   */
    LCDCharge_LittleCurrent = 0x10,        /*!< LCD小电流充电模式                   */
    LCDChargeQuick_PeriodDiv8 = 0x30,      /*!< 快速充电模式，充电时间1/8个Flcd周期 */
    LCDChargeQuick_PeriodDiv16 = 0x70,     /*!< 快速充电模式，充电时间1/16个Flcd周期*/
    LCDChargeQuick_PeriodDiv32 = 0xb0,     /*!< 快速充电模式，充电时间1/32个Flcd周期*/
    LCDChargeQuick_PeriodDiv64 = 0xf0,     /*!< 快速充电模式，充电时间1/64个Flcd周期*/
}LCDChargeMode_TypeDef;                    /*!< end of group LCDChargeMode_TypeDef  */  

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





#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_LCD_H__ */ 
