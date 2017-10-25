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
* File         : ht6xxx_flash.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_FLASH_H__
#define __HT6XXX_FLASH_H__

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

void HT_Flash_ByteWrite(const uint8_t* pWriteByte, uint32_t Address, uint32_t Num);
void HT_Flash_HalfWordWrite(const uint16_t* pWriteHalfWord, uint32_t Address, uint32_t Num);
void HT_Flash_WordWrite(const uint32_t* pWriteWord, uint32_t Address, uint32_t Num);
void HT_Flash_ByteRead(uint8_t* pReadByte, uint32_t Address, uint32_t Num);
void HT_Flash_HalfWordRead(uint16_t* pReadHalfWord, uint32_t Address, uint32_t Num);
void HT_Flash_WordRead(uint32_t* pReadWord, uint32_t Address, uint32_t Num);
void HT_Flash_PageErase(uint32_t EraseAddress);
void HT_Flash_ChipErase(void);
#if defined  HT502x
void HT_Flash_PageEraseWithSectionUnlock(uint32_t EraseAddress);
#endif





#ifdef __cplusplus
}
#endif

#endif /* __HT60xx_FLASH_H__ */ 
