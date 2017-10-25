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
* File         : ht6xxx_gpio.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_GPIO_H__
#define __HT6XXX_GPIO_H__

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
* @brief  GPIO管脚模式定义
*/
typedef enum
{ 
    GPIO_Mode_IOIN  = 0x00,       /*!< GPIO输入                      */
    GPIO_Mode_IOOUT = 0x01,       /*!< GPIO输出                      */
    GPIO_Mode_AF1 = 0x10,         /*!< 复用功能1                     */
    GPIO_Mode_AF2 = 0x11          /*!< 复用功能2                     */
}GPIOMode_TypeDef;                /*!<end of group GPIOMode_TypeDef  */

/* 
* @brief  GPIO数字输入上拉定义
*/
typedef enum
{ 
    GPIO_Input_Up   = 0x00,      /*!< GPIO数字输入上拉使能           */
    GPIO_Input_Floating = 0x01,  /*!< GPIO数字输入上拉关闭           */
}GPIOInput_TypeDef;              /*!< end of group GPIOInput_TypeDef */

/* 
* @brief  GPIO数字输出开漏定义
*/
typedef enum
{ 
    GPIO_Output_OD = 0x00,       /*!< GPIO数字开漏输出               */
    GPIO_Output_PP = 0x01,       /*!< GPIO数字推挽输出               */
}GPIOOutput_TypeDef;             /*!< end of group GPIOOutput_TypeDef*/

/* 
* @brief  GPIO初始化结构定义
*/
typedef struct
{
    uint16_t GPIO_Pin;                    /*!< GPIO管脚                     */
    
    GPIOMode_TypeDef GPIO_Mode;           /*!< GPIO模式                     */  
    
    GPIOInput_TypeDef GPIO_InputStruct;   /*!< GPIO数字输入上拉功能配置     */
    
    GPIOOutput_TypeDef GPIO_OutputStruct; /*!< GPIO数字输出开漏功能配置     */
    
}GPIO_InitTypeDef;                        /*!< end of group GPIO_InitTypeDef*/

/* 
* @brief  GPIO初始化为复用管脚结构定义
*/
typedef struct
{
    uint16_t GPIO_Pin;                    /*!< GPIO管脚                     */
    
    uint16_t GPIO_AFMode;                 /*!< GPIO模式                     */  
    
    GPIOInput_TypeDef GPIO_InputStruct;   /*!< GPIO数字输入上拉功能配置     */
    
    GPIOOutput_TypeDef GPIO_OutputStruct; /*!< GPIO数字输出开漏功能配置     */
    
}GPIO_InitAFTypeDef;                        /*!< end of group GPIO_InitTypeDef*/

/* 
* @brief  GPIO管脚定义
*/
#define GPIO_Pin_0                 ((uint16_t)0x0001)     /*!< Pin 0 selected    */
#define GPIO_Pin_1                 ((uint16_t)0x0002)     /*!< Pin 1 selected    */
#define GPIO_Pin_2                 ((uint16_t)0x0004)     /*!< Pin 2 selected    */
#define GPIO_Pin_3                 ((uint16_t)0x0008)     /*!< Pin 3 selected    */
#define GPIO_Pin_4                 ((uint16_t)0x0010)     /*!< Pin 4 selected    */
#define GPIO_Pin_5                 ((uint16_t)0x0020)     /*!< Pin 5 selected    */
#define GPIO_Pin_6                 ((uint16_t)0x0040)     /*!< Pin 6 selected    */
#define GPIO_Pin_7                 ((uint16_t)0x0080)     /*!< Pin 7 selected    */
#define GPIO_Pin_8                 ((uint16_t)0x0100)     /*!< Pin 8 selected    */
#define GPIO_Pin_9                 ((uint16_t)0x0200)     /*!< Pin 9 selected    */
#define GPIO_Pin_10                ((uint16_t)0x0400)     /*!< Pin 10 selected   */
#define GPIO_Pin_11                ((uint16_t)0x0800)     /*!< Pin 11 selected   */
#define GPIO_Pin_12                ((uint16_t)0x1000)     /*!< Pin 12 selected   */
#define GPIO_Pin_13                ((uint16_t)0x2000)     /*!< Pin 13 selected   */
#define GPIO_Pin_14                ((uint16_t)0x4000)     /*!< Pin 14 selected   */
#define GPIO_Pin_15                ((uint16_t)0x8000)     /*!< Pin 15 selected   */
#define GPIO_Pin_All               ((uint16_t)0xFFFF)     /*!< All pins selected */

/* 
* @brief  GPIO管脚复用功能定义
*/
#define PA0_SEG28                  ((uint16_t)0x0000<<0)  /*!< PA0_AF1: seg28    */  
#define PA1_SEG29                  ((uint16_t)0x0000<<1)  /*!< PA1_AF1: seg29    */  
#define PA2_SEG30                  ((uint16_t)0x0000<<2)  /*!< PA2_AF1: seg30    */  
#define PA3_SEG31                  ((uint16_t)0x0000<<3)  /*!< PA3_AF1: seg31    */  
#define PA4_SEG32                  ((uint16_t)0x0000<<4)  /*!< PA4_AF1: seg32    */  
#define PA5_INT0                   ((uint16_t)0x0000<<5)  /*!< PA5_AF1: int0     */ 
#if defined  HT6x2x 
#define PA5_CLKIN                  ((uint16_t)0x0001<<5)  /*!< PA5_AF2: CLK IN   */
#elif defined HT501x  ||  defined  HT502s
#define PA5_SCANIN0                ((uint16_t)0x0001<<5)  /*!< PA5_AF2: scann0   */
#endif
#define PA6_INT1                   ((uint16_t)0x0000<<6)  /*!< PA6_AF1: int1     */ 
#if defined  HT501x  ||  defined  HT502x
#define PA6_SCANIN1                ((uint16_t)0x0001<<6)  /*!< PA6_AF2: scann1   */ 
#endif
#define PA7_INT2                   ((uint16_t)0x0000<<7)  /*!< PA7_AF1: int2     */  
#define PA7_SEG33                  ((uint16_t)0x0001<<7)  /*!< PA7_AF2: seg33    */  
#define PA8_INT3                   ((uint16_t)0x0000<<8)  /*!< PA8_AF1: int3     */  
#define PA8_SEG34                  ((uint16_t)0x0001<<8)  /*!< PA8_AF2: seg34    */  
#define PA9_INT4                   ((uint16_t)0x0000<<9)  /*!< PA9_AF1: int4     */  
#define PA10_INT5                  ((uint16_t)0x0000<<10) /*!< PA10_AF1: int5    */  
#define PA11_INT6                  ((uint16_t)0x0000<<11) /*!< PA11_AF1: int6    */
#if defined  HT502x
#define PA11_SEG50                 ((uint16_t)0x0001<<11) /*!< PA11_AF2: seg50   */
#endif
#define PA12_SEG35                 ((uint16_t)0x0000<<12) /*!< PA12_AF1: seg35   */  
#define PA12_ADCIN0                ((uint16_t)0x0001<<12) /*!< PA12_AF2: adcin0  */  
#define PA13_SEG36                 ((uint16_t)0x0000<<13) /*!< PA13_AF1: seg36   */  
#define PA13_ADCIN1                ((uint16_t)0x0001<<13) /*!< PA13_AF2: adcin1  */ 

#define PB0_SEG0                   ((uint16_t)0x0000<<0)  /*!< PB0_AF1: seg0     */  
#define PB1_SEG1                   ((uint16_t)0x0000<<1)  /*!< PB1_AF1: seg1     */  
#define PB2_SEG2                   ((uint16_t)0x0000<<2)  /*!< PB2_AF1: seg2     */  
#if defined  HT502x
#define PB2_VLCD0                  ((uint16_t)0x0001<<2)  /*!< PB2_AF2: vlcd0    */  
#endif
#define PB3_SEG3                   ((uint16_t)0x0000<<3)  /*!< PB3_AF1: seg3     */  
#if defined  HT502x
#define PB3_VLCD1                  ((uint16_t)0x0001<<3)  /*!< PB3_AF2: vlcd1    */  
#endif
#define PB4_SEG4                   ((uint16_t)0x0000<<4)  /*!< PB4_AF1: seg4     */  
#if defined  HT502x
#define PB4_VLCD2                  ((uint16_t)0x0001<<4)  /*!< PB4_AF2: vlcd2    */  
#endif
#define PB5_SEG5                   ((uint16_t)0x0000<<5)  /*!< PB5_AF1: seg5     */  
#if defined  HT502x
#define PB5_VLCD3                  ((uint16_t)0x0001<<5)  /*!< PB5_AF2: vlcd3    */  
#endif
#define PB6_SEG6                   ((uint16_t)0x0000<<6)  /*!< PB6_AF1: seg6     */  
#define PB7_SEG7                   ((uint16_t)0x0000<<7)  /*!< PB7_AF1: seg7     */  
#define PB8_SEG8                   ((uint16_t)0x0000<<8)  /*!< PB8_AF1: seg8     */  
#define PB9_SEG9                   ((uint16_t)0x0000<<9)  /*!< PB9_AF1: seg9     */  
#define PB10_SEG10                 ((uint16_t)0x0000<<10) /*!< PB10_AF1: seg10   */  
#define PB11_SEG11                 ((uint16_t)0x0000<<11) /*!< PB11_AF1: seg11   */  
#define PB12_SEG12                 ((uint16_t)0x0000<<12) /*!< PB12_AF1: seg12   */  
#if defined  HT502x
#define PB12_INT8                  ((uint16_t)0x0001<<12) /*!< PB12_AF2: int8    */  
#endif
#define PB13_SEG13                 ((uint16_t)0x0000<<13) /*!< PB13_AF1: seg13   */  
#if defined  HT502x
#define PB13_SWIO                  ((uint16_t)0x0001<<13) /*!< PB13_AF2: swio    */  
#endif
#define PB14_SEG14                 ((uint16_t)0x0000<<14) /*!< PB14_AF1: seg14   */  
#if defined  HT502x
#define PB14_INT9                  ((uint16_t)0x0001<<14) /*!< PB14_AF2: int9    */  
#endif
#define PB15_SEG15                 ((uint16_t)0x0000<<15) /*!< PB15_AF1: seg15   */
#if defined  HT502x
#define PB13_SWCLK                 ((uint16_t)0x0001<<15) /*!< PB15_AF2: swclk   */  
#endif
                                                                                  
#define PC0_TX1                    ((uint16_t)0x0000<<0)  /*!< PC0_AF1: tx1      */  
#if defined  HT502x
#define PC0_SEG43                  ((uint16_t)0x0001<<0)  /*!< PC0_AF2: seg43    */  
#endif
#define PC1_RX1                    ((uint16_t)0x0000<<1)  /*!< PC1_AF1: rx1      */  
#if defined  HT502x
#define PC1_SEG44                  ((uint16_t)0x0001<<1)  /*!< PC1_AF2: seg44    */  
#endif
#define PC2_RX0                    ((uint16_t)0x0000<<2)  /*!< PC2_AF1: rx0      */  
#if defined  HT6x2x 
#define PC2_INT7                   ((uint16_t)0x0001<<2)  /*!< PC2_AF2: int7     */
#elif defined  HT502x
#define PC2_SEG45                  ((uint16_t)0x0001<<2)  /*!< PC2_AF2: seg45    */  
#endif
#define PC3_TX0                    ((uint16_t)0x0000<<3)  /*!< PC3_AF1: tx0      */  
#if defined  HT502x
#define PC3_SEG46                  ((uint16_t)0x0001<<3)  /*!< PC3_AF2: seg46    */  
#endif
#define PC4_SPIMOSI                ((uint16_t)0x0000<<4)  /*!< PC4_AF1: spi_mosi */  
#define PC4_RX5                    ((uint16_t)0x0001<<4)  /*!< PC4_AF2: rx5      */  
#define PC5_SPIMISO                ((uint16_t)0x0000<<5)  /*!< PC5_AF1: spi_miso */  
#define PC5_TX5                    ((uint16_t)0x0001<<5)  /*!< PC5_AF2: tx5      */  
#define PC6_SPICLK                 ((uint16_t)0x0000<<6)  /*!< PC6_AF1: spiclk   */ 
#if defined  HT502x
#define PC6_SEG51                  ((uint16_t)0x0001<<6)  /*!< PC6_AF2: seg51    */  
#endif
#define PC7_SPICS                  ((uint16_t)0x0000<<7)  /*!< PC7_AF1: spics    */ 
#if defined  HT502x
#define PC7_SEG52                  ((uint16_t)0x0001<<7)  /*!< PC7_AF2: seg52    */  
#endif
#define PC8_TOUT1                  ((uint16_t)0x0000<<8)  /*!< PC8_AF1: tout1    */  
#if defined  HT502x
#define PC8_INT7                   ((uint16_t)0x0001<<8)  /*!< PC8_AF2: int7     */  
#endif
#define PC9_TMR2                   ((uint16_t)0x0000<<9)  /*!< PC9_AF1: timer2   */ 
#if defined  HT501x  ||  defined  HT502x
#define PC9_SCANIN3                ((uint16_t)0x0001<<9)  /*!< PC9_AF2: scann3   */ 
#endif
#define PC10_TMR3                  ((uint16_t)0x0000<<10) /*!< PC10_AF1: timer3  */ 
#if defined  HT501x  ||  defined  HT502x
#define PC10_SCANIN2               ((uint16_t)0x0001<<10) /*!< PC10_AF2: scann2  */ 
#endif
#define PC11_TX2                   ((uint16_t)0x0000<<11) /*!< PC11_AF1: tx2     */  
#define PC12_RX2                   ((uint16_t)0x0000<<12) /*!< PC12_AF1: rx2     */  
#define PC13_SCL                   ((uint16_t)0x0000<<13) /*!< PC13_AF1: scl     */  
#define PC14_SDA                   ((uint16_t)0x0000<<14) /*!< PC14_AF1: sda     */  
                                                                                  
#define PD0_SEG16                  ((uint16_t)0x0000<<0)  /*!< PD0_AF1: seg16    */  
#define PD1_SEG17                  ((uint16_t)0x0000<<1)  /*!< PD1_AF1: seg17    */  
#define PD2_SEG18                  ((uint16_t)0x0000<<2)  /*!< PD2_AF1: seg18    */ 
#if defined  HT501x  ||  defined  HT502x
#define PD2_SCANOUT0               ((uint16_t)0x0001<<2)  /*!< PD2_AF2: scanout0 */ 
#endif
#define PD3_SEG19                  ((uint16_t)0x0000<<3)  /*!< PD3_AF1: seg19    */  
#if defined  HT501x  ||  defined  HT502x
#define PD3_SCANOUT1               ((uint16_t)0x0001<<3)  /*!< PD3_AF2: scanout1 */ 
#endif
#define PD4_SEG20                  ((uint16_t)0x0000<<4)  /*!< PD4_AF1: seg20    */  
#if defined  HT501x  ||  defined  HT502x
#define PD4_SCANOUT2               ((uint16_t)0x0001<<4)  /*!< PD4_AF2: scanout2 */ 
#endif
#define PD5_SEG21                  ((uint16_t)0x0000<<5)  /*!< PD5_AF1: seg21    */  
#if defined  HT501x  ||  defined  HT502x
#define PD5_SCANOUT3               ((uint16_t)0x0001<<5)  /*!< PD5_AF2: scanout3 */ 
#endif
#define PD6_SEG22                  ((uint16_t)0x0000<<6)  /*!< PD6_AF1: seg22    */  
#define PD7_SEG23                  ((uint16_t)0x0000<<7)  /*!< PD7_AF1: seg23    */  
#define PD8_COM0                   ((uint16_t)0x0000<<8)  /*!< PD8_AF1: com0     */  
#define PD9_COM1                   ((uint16_t)0x0000<<9)  /*!< PD9_AF1: com1     */  
#define PD10_COM2                  ((uint16_t)0x0000<<10) /*!< PD10_AF1: com2    */  
#define PD11_COM3                  ((uint16_t)0x0000<<11) /*!< PD11_AF1: com3    */  
#define PD12_COM4                  ((uint16_t)0x0000<<12) /*!< PD12_AF1: com4    */  
#define PD12_SEG24                 ((uint16_t)0x0001<<12) /*!< PD12_AF2: seg24   */  
#define PD13_COM5                  ((uint16_t)0x0000<<13) /*!< PD13_AF1: com5    */  
#define PD13_SEG25                 ((uint16_t)0x0001<<13) /*!< PD13_AF2: seg25   */  
#define PD14_COM6                  ((uint16_t)0x0000<<14) /*!< PD14_AF1: com6    */  
#define PD14_SEG26                 ((uint16_t)0x0001<<14) /*!< PD14_AF2: seg26   */  
#define PD15_COM7                  ((uint16_t)0x0000<<15) /*!< PD15_AF1: com7    */  
#define PD15_SEG27                 ((uint16_t)0x0001<<15) /*!< PD15_AF2: seg27   */  
                                                                             
#define PE0_TMR0                   ((uint16_t)0x0000<<0)  /*!< PE0_AF1: timer0   */  
#define PE1_TX4                    ((uint16_t)0x0000<<1)  /*!< PE1_AF1: tx4      */  
#define PE1_TOUT2                  ((uint16_t)0x0001<<1)  /*!< PE1_AF2: tout2    */  
#define PE2_RX4                    ((uint16_t)0x0000<<2)  /*!< PE2_AF1: rx4      */  
#define PE2_ESAMIO                 ((uint16_t)0x0001<<2)  /*!< PE2_AF2: esam io  */
#if defined  HT502x
#define PE2_7816_0                 ((uint16_t)0x0001<<2)  /*!< PE2_AF2: 7816_0   */
#endif
#define PE3_CLKOUT                 ((uint16_t)0x0000<<3)  /*!< PE3_AF1: clkout   */
#if defined  HT502x
#define PE3_SEG53                  ((uint16_t)0x0001<<3)  /*!< PE3_AF2: seg53    */
#endif
#define PE4_RX3                    ((uint16_t)0x0000<<4)  /*!< PE4_AF1: rx3      */
#if defined  HT502x
#define PE4_7816_1                 ((uint16_t)0x0001<<4)  /*!< PE4_AF2: 7816_1   */
#endif
#define PE4_CARDIO                 ((uint16_t)0x0001<<4)  /*!< PE4_AF2: card io  */  
#define PE5_TX3                    ((uint16_t)0x0000<<5)  /*!< PE5_AF1: tx3      */
#if defined  HT502x
#define PE5_SEG49                  ((uint16_t)0x0001<<5)  /*!< PE5_AF2: seg49    */
#endif
#define PE6_TMR1                   ((uint16_t)0x0000<<6)  /*!< PE6_AF1: timer 1  */  
#define PE7_LVDIN0                 ((uint16_t)0x0000<<7)  /*!< PE7_AF1: lvdin0   */
#if defined  HT502x
#define PE7_SEG47                  ((uint16_t)0x0001<<7)  /*!< PE7_AF2: seg47    */
#endif
#if defined  HT501x  ||  defined  HT502x
#define PE8_SEG37                  ((uint16_t)0x0000<<8)  /*!< PE8_AF1: seg37    */ 
#define PE8_ADCIN2                 ((uint16_t)0x0001<<8)  /*!< PE8_AF2: adcin2   */ 
#define PE9_LVDIN1                 ((uint16_t)0x0000<<9)  /*!< PE9_AF1: lvdin1   */ 
#else
#define PE8_POWIN                  ((uint16_t)0x0000<<8)  /*!< PE8_AF1: powin    */   
#endif

#if defined  HT501x || HT502x
#define PF0_PFOUT                  ((uint16_t)0x0000<<0)  /*!< PF0_AF1: pf       */
#if defined  HT502x
#define PF0_TOUT3                  ((uint16_t)0x0001<<0)  /*!< PF0_AF2: tout3    */
#endif
#define PF1_QFOUT                  ((uint16_t)0x0000<<1)  /*!< PF1_AF1: qf       */ 
#define PF2_SFOUT                  ((uint16_t)0x0000<<2)  /*!< PF2_AF1: sf       */
#if defined  HT502x
#define PF2_SEG48                  ((uint16_t)0x0001<<2)  /*!< PF2_AF2: seg48    */
#endif
#define PF3_SEG38                  ((uint16_t)0x0000<<3)  /*!< PF3_AF1: seg38    */
#define PF4_SEG39                  ((uint16_t)0x0000<<4)  /*!< PF4_AF1: seg39    */
#define PF5_SEG40                  ((uint16_t)0x0000<<5)  /*!< PF5_AF1: seg40    */
#define PF6_SEG41                  ((uint16_t)0x0000<<6)  /*!< PF6_AF1: seg41    */
#define PF7_SEG42                  ((uint16_t)0x0000<<7)  /*!< PF7_AF1: seg42    */ 
#endif


#if defined  HT6x2x 
#define PG0_SEG37                  ((uint16_t)0x0000<<0)  /*!< PG0_AF1: seg37    */  
#define PG1_SEG38                  ((uint16_t)0x0000<<1)  /*!< PG1_AF1: seg38    */
#define PG2_SEG39                  ((uint16_t)0x0000<<2)  /*!< PG2_AF1: seg39    */
#define PG3_SEG40                  ((uint16_t)0x0000<<3)  /*!< PG3_AF1: seg40    */
#define PG4_SEG41                  ((uint16_t)0x0000<<4)  /*!< PG4_AF1: seg41    */
#define PG5_LVDIN1                 ((uint16_t)0x0000<<5)  /*!< PG5_AF1: lvdin1   */
#define PG6_SPI1CS                 ((uint16_t)0x0000<<6)  /*!< PG6_AF1: spi1cs   */
#define PG7_SPI1CLK                ((uint16_t)0x0000<<7)  /*!< PG7_AF1: spi1clk  */
#define PG8_SPI1MISO               ((uint16_t)0x0000<<8)  /*!< PG8_AF1: spi1miso */
#define PG9_SPI1MOSI               ((uint16_t)0x0000<<9)  /*!< PG9_AF1: spi1mosi */
#define PG11_RX6                   ((uint16_t)0x0000<<11) /*!< PG11_AF1: rx6     */
#define PG12_TX6                   ((uint16_t)0x0000<<12) /*!< PG12_AF1: tx6     */

#define PH0_TMR4                   ((uint16_t)0x0000<<0)  /*!< PH0_AF1: timer 4  */
#define PH1_TMR5                   ((uint16_t)0x0000<<1)  /*!< PH1_AF1: timer 5  */
#define PH2_INT8                   ((uint16_t)0x0000<<2)  /*!< PH2_AF1: int 8    */
#define PH3_INT9                   ((uint16_t)0x0000<<3)  /*!< PH3_AF1: int 9    */
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

void HT_GPIO_Init(HT_GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void HT_GPIO_AFInit(HT_GPIO_TypeDef* GPIOx, GPIO_InitAFTypeDef* GPIO_InitAFStruct);
void HT_GPIO_BitsSet(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HT_GPIO_BitsReset(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HT_GPIO_BitsToggle(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t HT_GPIO_BitsRead(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void HT_GPIO_HighCurrentSet(uint16_t GPIO_Pin, FunctionalState NewState);




#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_GPIO_H__ */ 
