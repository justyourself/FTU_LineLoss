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
* File         : ht6xxx_flash.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.3
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
* @brief  flash H256K and L256K block
*/
#if  defined  HT6x3x
typedef enum
{
    FlashBlock1   = 0,                           /*!< First  256k byte                 */
    FlashBlock2   = 1,                           /*!< Second 256k byte                 */
}FLASH_BlockEraseTypeDef;                        /*!< end of group FLASH_BlockEraseTypeDef      */
#endif

#if  defined  HT501x
#define FLASH_MAX_ADDRESS       0x00020000
#elif  defined  HT502x
#define FLASH_MAX_ADDRESS       0x00040000
#elif  defined  HT6x1x
#define FLASH_MAX_ADDRESS       0x00020000
#elif  defined  HT6x2x
#define FLASH_MAX_ADDRESS       0x00040000
#elif  defined  HT6x3x
#define FLASH_MAX_ADDRESS       0x00080000
#else
#define FLASH_MAX_ADDRESS       0x00000000
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

void HT_Flash_ByteWrite(const uint8_t* pWriteByte, uint32_t Address, uint32_t Num);
void HT_Flash_HalfWordWrite(const uint16_t* pWriteHalfWord, uint32_t Address, uint32_t Num);
void HT_Flash_WordWrite(const uint32_t* pWriteWord, uint32_t Address, uint32_t Num);
void HT_Flash_ByteRead(uint8_t* pReadByte, uint32_t Address, uint32_t Num);
void HT_Flash_HalfWordRead(uint16_t* pReadHalfWord, uint32_t Address, uint32_t Num);
void HT_Flash_WordRead(uint32_t* pReadWord, uint32_t Address, uint32_t Num);
void HT_Flash_PageErase(uint32_t EraseAddress);
void HT_Flash_ChipErase(void);
#if defined  HT6x3x
void HT_Flash_BlockChipErase(FLASH_BlockEraseTypeDef EraseBlock);
#endif
#if defined  HT6x3x  ||  defined  HT502x
void HT_Flash_PageEraseWithSectionUnlock(uint32_t EraseAddress);
#endif


#ifdef __cplusplus
}
#endif

#endif /* __HT60xx_FLASH_H__ */
