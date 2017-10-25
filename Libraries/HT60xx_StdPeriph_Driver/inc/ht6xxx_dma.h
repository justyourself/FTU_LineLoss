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
* File         : ht6xxx_dma.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT6x2x, HT501x and HT502x
**********************************************************************************************************
*/

#ifndef __HT6XXX_DMA_H__
#define __HT6XXX_DMA_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"

#if defined HT6x2x  ||  defined  HT501x  ||  defined  HT502x             /* This File Only support HT6x2x, HT501x and HT502x */
    
/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/
  
/* 
* @brief  DMA模块初始化结构定义
*/
typedef struct
{
  uint32_t DMA_Request;
  uint32_t DMA_SourceAddr;                  
  uint32_t DMA_DestinationAddr;  
  uint32_t DMA_TransferNum;    
  uint32_t DMA_BulkSize;                    
  uint32_t DMA_SourceAddrInc;                                         
  uint32_t DMA_DestinationAddrInc;                                                       
  uint32_t DMA_MemoryDataSize;                                            
  uint32_t DMA_TransferMode;                                                      
  uint32_t DMA_CycleMode;                  
}DMA_InitTypeDef;                                /*!< end of group TMR_InitTypeDef   */



/* 
* @brief  DMA_SourceAddrInc自增设置
*/
#define DMA_SourceAddrInc_NoIncrease                ((uint32_t)0x00000000)
#define DMA_SourceAddrInc_AutoIncrease              ((uint32_t)0x00000020)
#define DMA_SourceAddrInc_BulkInternalIncrease      ((uint32_t)0x00000040)

/* 
* @brief  DMA_DestinationAddrInc自增设置
*/
#define DMA_DestinationAddrInc_NoIncrease           ((uint32_t)0x00000000)
#define DMA_DestinationAddrInc_AutoIncrease         ((uint32_t)0x00000080)
#define DMA_DestinationAddrInc_BulkInternalIncrease ((uint32_t)0x00000100)

/* 
* @brief  DMA_MemoryDataSize大小设置
*/

#define DMA_MemoryDataSize_Byte                     ((uint32_t)0x00000000)
#define DMA_MemoryDataSize_HalfWord                 ((uint32_t)0x00000002)
#define DMA_MemoryDataSize_Word                     ((uint32_t)0x00000004)  

/* 
* @brief  DMA_TransferMode类型设置
*/

#define DMA_TransferMode_SingleTransfer             ((uint32_t)0x00000000)
#define DMA_TransferMode_BulkTransfer               ((uint32_t)0x00000008)

/* 
* @brief  DMA_CycleMode类型设置
*/
#define DMA_CycleMode_NoCycleTransfer               ((uint32_t)0x00000000)
#define DMA_CycleMode_CycleTransfer                 ((uint32_t)0x00000010)

/* 
* @brief  DMA_Channel_En通道使能设置
*/
#define DMA_Channel_Enable                          ((uint32_t)0x00000001)
#define DMA_Channel_Disable                         ((uint32_t)0xFFFFFFFE)

/* 
* @brief  DMA_Request设置
*/
#define DMA_Request_Software                         ((uint32_t)0x00000000 << 9)
#define DMA_Request_UART0_TX                         ((uint32_t)0x00000001 << 9)
#define DMA_Request_UART0_RX                         ((uint32_t)0x00000002 << 9)
#define DMA_Request_UART1_TX                         ((uint32_t)0x00000003 << 9)
#define DMA_Request_UART1_RX                         ((uint32_t)0x00000004 << 9)
#define DMA_Request_UART2_TX                         ((uint32_t)0x00000005 << 9)
#define DMA_Request_UART2_RX                         ((uint32_t)0x00000006 << 9)
#define DMA_Request_UART3_TX                         ((uint32_t)0x00000007 << 9)
#define DMA_Request_UART3_RX                         ((uint32_t)0x00000008 << 9)
#define DMA_Request_UART4_TX                         ((uint32_t)0x00000009 << 9)
#define DMA_Request_UART4_RX                         ((uint32_t)0x0000000A << 9)
#define DMA_Request_UART5_TX                         ((uint32_t)0x0000000B << 9)
#define DMA_Request_UART5_RX                         ((uint32_t)0x0000000C << 9)
#define DMA_Request_ISO7816_0_TX                     ((uint32_t)0x0000000D << 9)
#define DMA_Request_ISO7816_0_RX                     ((uint32_t)0x0000000E << 9)
#define DMA_Request_ISO7816_1_TX                     ((uint32_t)0x0000000F << 9)
#define DMA_Request_ISO7816_1_RX                     ((uint32_t)0x00000010 << 9)
#define DMA_Request_SPI_TX                           ((uint32_t)0x00000011 << 9)
#define DMA_Request_SPI_RX                           ((uint32_t)0x00000012 << 9)
#define DMA_Request_I2C_TX                           ((uint32_t)0x00000013 << 9)
#define DMA_Request_I2C_RX                           ((uint32_t)0x00000014 << 9)
#define DMA_Request_EMU_UWave                        ((uint32_t)0x00000015 << 9)
#define DMA_Request_EMU_I1Wave                       ((uint32_t)0x00000016 << 9)
#define DMA_Request_EMU_I2Wave                       ((uint32_t)0x00000017 << 9)
#define DMA_Request_TIMER0                           ((uint32_t)0x00000018 << 9)
#define DMA_Request_TIMER1                           ((uint32_t)0x00000019 << 9)
#define DMA_Request_TIMER2                           ((uint32_t)0x0000001A << 9)
#define DMA_Request_TIMER3                           ((uint32_t)0x0000001B << 9)
#define DMA_Request_TIMER4                           ((uint32_t)0x0000001C << 9)
#define DMA_Request_TIMER5                           ((uint32_t)0x0000001D << 9)
#define DMA_Request_UART6_TX                         ((uint32_t)0x0000001E << 9)
#define DMA_Request_UART6_RX                         ((uint32_t)0x0000001F << 9)
#define DMA_Request_SPI1_TX                          ((uint32_t)0x00000020 << 9)
#define DMA_Request_SPI1_RX                          ((uint32_t)0x00000021 << 9)


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

void HT_DMA_Init(HT_DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_Cmd(HT_DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState);
void HT_DMA_ITConfig(uint8_t ITEn, FunctionalState NewState);
ITStatus HT_DMA_ITFlagStatusGet(uint8_t ITFlag);
void HT_DMA_ClearITPendingBit(uint8_t ITFlag);



#endif                                        /* This File Only support HT6x2x and HT501x */

#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_DMA_H__ */

