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
* File         : ht6xxx_spi.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  :
**********************************************************************************************************
*/

#ifndef __HT6XXX_SPI_H__
#define __HT6XXX_SPI_H__

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
* @brief  SPI时钟相位定义
*/
typedef enum
{
    SPI_CPHA_1Edge = 0x00,                      /*!< 第一个动态沿之前半周期数据被发送  */
    SPI_CPHA_2Edge = SPI_SPICON_SSDIS,          /*!< 动态沿通过后数据被发送            */
}SPICPHA_TypeDef;                               /*!< end of group SPICPHA_TypeDef      */

/*
* @brief  SPI时钟极性定义
*/
typedef enum
{
    SPI_CPOL_Low  = 0x00,                       /*!< 空闲状态SCK被置为低电平           */
    SPI_CPOL_high = SPI_SPICON_CPOL,            /*!< 空闲状态SCK被置为高电平           */
}SPICPOL_TypeDef;                               /*!< end of group SPICPOL_TypeDef      */

/*
* @brief  SPI时钟频率定义
*/
typedef enum
{
    SPI_BaudRatePrescaler_2   = 0x00,           /*!< Fspi = Fsys/2                     */
    SPI_BaudRatePrescaler_4   = 0x10,           /*!< Fspi = Fsys/4                     */
    SPI_BaudRatePrescaler_8   = 0x20,           /*!< Fspi = Fsys/8                     */
    SPI_BaudRatePrescaler_16  = 0x30,           /*!< Fspi = Fsys/16                    */
    SPI_BaudRatePrescaler_32  = 0x40,           /*!< Fspi = Fsys/32                    */
    SPI_BaudRatePrescaler_64  = 0x50,           /*!< Fspi = Fsys/64                    */
    SPI_BaudRatePrescaler_128 = 0x60,           /*!< Fspi = Fsys/128                   */
    SPI_CLK_None              = 0x70,           /*!< 不产生主时钟                      */
#if  defined  HT6x3x
    SPI_BaudRatePrescaler_3   = 0x01,           /*!< Fspi = Fsys/3                     */
    SPI_BaudRatePrescaler_5   = 0x05,           /*!< Fspi = Fsys/5                     */
    SPI_BaudRatePrescaler_6   = 0x06,           /*!< Fspi = Fsys/6                     */
    SPI_BaudRatePrescaler_7   = 0x07,           /*!< Fspi = Fsys/7                     */
    SPI_BaudRatePrescaler_9   = 0x09,           /*!< Fspi = Fsys/9                     */
#endif
}SPIBaudrate_TypeDef;                           /*!< end of group SPIBaudrate_TypeDef  */

/*
* @brief  SPIMode定义
*/
typedef enum
{
    SPI_Slave  = 0x00,                          /*!< SPI从机模式                       */
    SPI_Master = SPI_SPICON_MSTR,               /*!< SPI主机模式                       */
}SPIMode_TypeDef;                               /*!< end of group SPIMode_TypeDef      */

#if  defined  HT6x3x
/*
* @brief  SPI 时钟源选择定义（目前仅限SPI2）
*/
typedef enum
{
    SPI_CLKSEL_OSC  = 0x00,                     /*!< OSC晶振                           */
    SPI_CLKSEL_LRC  = 0x10,                     /*!< LRC晶振                           */
    SPI_CLKSEL_HRC  = 0x20,                     /*!< HRC晶振                           */
    SPI_CLKSEL_PLL  = 0x30,                     /*!< PLL晶振                           */
}SPIClkSel_TypeDef;                             /*!< end of group SPIClkSel_TypeDef    */
#endif

/*
* @brief  SPI模块扩展初始化结构定义
*/
typedef struct
{
    FunctionalState SPI_CSInCtrol;              /*!< 做从机时，是否打开SPI_CS输入控制  */
    SPIBaudrate_TypeDef SPI_Baudrate;           /*!< SPI时钟设置                       */
    SPICPHA_TypeDef SPI_CPHA;                   /*!< 时钟相位设置                      */
    SPICPOL_TypeDef SPI_CPOL;                   /*!< 时钟极性设置                      */
    SPIMode_TypeDef SPI_Mode;                   /*!< SPI_Mode设置                      */
#if  defined  HT6x3x
    SPIClkSel_TypeDef SPI_CLK;                  /*!< SPI时钟源设置(仅限SPI2，无扩展)   */
#endif
}SPI_InitTypeDef;                               /*!< end of group SPI_InitTypeDef      */

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

void HT_SPI_Init(HT_SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct);
ITStatus HT_SPI_ITFlagStatusGet(HT_SPI_TypeDef* SPIx, uint8_t ITFlag);
void HT_SPI_ClearITPendingBit(HT_SPI_TypeDef* SPIx, uint8_t ITFlag);
uint8_t HT_SPI_SendByte(HT_SPI_TypeDef* SPIx, uint16_t halfword);
void HT_SPI_CSHigh(HT_SPI_TypeDef* SPIx);
void HT_SPI_CSLow(HT_SPI_TypeDef* SPIx);


#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_SPI_H__ */
