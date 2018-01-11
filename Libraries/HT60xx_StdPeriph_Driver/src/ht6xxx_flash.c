/*
*********************************************************************************************************
*                                              HT6XXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*
*
* Project      : HT6xxx
* File         : ht6xxx_flash.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.3
* Description  :
*********************************************************************************************************
*/

#define  __HT6XXX_FLASH_C

#include "ht6xxx_flash.h"

/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/
#define M8(adr)     (*((uint8_t * ) (adr)))
#define M16(adr)    (*((uint16_t *) (adr)))
#define M32(adr)    (*((uint32_t *) (adr)))

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};

/*
*********************************************************************************************************
*                                             本地变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         FLASH BYTE WRITE
*
* 函数说明: Flash字节写
*
* 入口参数: pWriteByte    指向被写数据的首地址
*
*           Address       数据将要写到Flash的地址
*
*           Num           写数据长度，以字节为单位
*
* 返回参数: 无
*
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*********************************************************************************************************
*/
void HT_Flash_ByteWrite(const uint8_t* pWriteByte, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */
    uint32_t i;
    uint32_t writeProtect;

    if (Address >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];             /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_WRITE;                           /*!< Flash写                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_WRITE;                            /*!< Flash写                */
#endif

    for(i=0; i<Num; i++)
    {
        if ((Address+i) >= FLASH_MAX_ADDRESS)
        {
            break;
        }
        M8(Address+i) = pWriteByte[i];                                     /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
    }

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}

/*
*********************************************************************************************************
*                                         FLASH BYTE READ
*
* 函数说明: Flash字节读
*
* 入口参数: pReadByte     指向存储读到的数据的首地址
*
*           Address       读Flash的首地址
*
*           Num           读数据长度，以字节为单位
*
* 返回参数: 无
*
* 特殊说明: 用户需注意读的数据不要超出保存的范围，以防溢出
*********************************************************************************************************
*/
void HT_Flash_ByteRead(uint8_t* pReadByte, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */
    uint32_t i;
    uint32_t writeProtect;

    if (Address >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];             /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */

    for(i=0; i<Num; i++)
    {
        if ((Address+i) >= FLASH_MAX_ADDRESS)
        {
            return;
        }
        pReadByte[i] = M8(Address+i);                                      /*!< 执行Flash读            */
    }
}

/*
*********************************************************************************************************
*                                         FLASH HALF WORD WRITE
*
* 函数说明: Flash半字写
*
* 入口参数: pWriteHalfWord    指向被写数据的首地址
*
*           Address           数据将要写到Flash的地址
*
*           Num               写数据长度，以半字为单位
*
* 返回参数: 无
*
* 特殊说明: 1)用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*           2)用户应保证传递给函数的地址为至少半字对齐
*********************************************************************************************************
*/
void HT_Flash_HalfWordWrite(const uint16_t* pWriteHalfWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */
    uint32_t i;
    uint32_t writeProtect;

    if (Address >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];             /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_WRITE;                           /*!< Flash写                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_WRITE;                            /*!< Flash写                */
#endif

    Address &= 0xFFFFFFFE;                                                 /*!< 保证半字对齐           */
    for(i=0; i<Num; i++)
    {
        if ((Address+i*2) >= FLASH_MAX_ADDRESS)
        {
            break;
        }
        M16(Address+i*2) = pWriteHalfWord[i];                              /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
    }

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}
/*
*********************************************************************************************************
*                                         FLASH HALF WORD READ
*
* 函数说明: Flash半字读
*
* 入口参数: pReadHalfWord 指向存储读到的数据的首地址
*
*           Address       读Flash的首地址
*
*           Num           读数据长度，以半字为单位
*
* 返回参数: 无
*
* 特殊说明: 1)用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*           2)用户应保证传递给函数的地址为至少半字对齐
*********************************************************************************************************
*/
void HT_Flash_HalfWordRead(uint16_t* pReadHalfWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */
    uint32_t i;
    uint32_t writeProtect;

    if (Address >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];             /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */

    Address &= 0xFFFFFFFE;                                                 /*!< 保证半字对齐           */
    for(i=0; i<Num; i++)
    {
        if ((Address+i*2) >= FLASH_MAX_ADDRESS)
        {
            return;
        }
        pReadHalfWord[i] = M16(Address+i*2);                               /*!< 执行Flash读            */
    }
}

/*
*********************************************************************************************************
*                                         FLASH WORD WRITE
*
* 函数说明: Flash字写
*
* 入口参数: pWriteWord    指向被写数据的首地址
*
*           Address       数据将要写到Flash的地址
*
*           Num           写数据长度，以字为单位
*
* 返回参数: 无
*
* 特殊说明: 1)用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*           2)用户应保证传递给函数的地址为至少字对齐
*********************************************************************************************************
*/
void HT_Flash_WordWrite(const uint32_t* pWriteWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */

    uint32_t i;
    uint32_t writeProtect;

    if (Address >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];             /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_WRITE;                           /*!< Flash写                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_WRITE;                            /*!< Flash写                */
#endif

    Address &= 0xFFFFFFFC;                                                 /*!< 保证字对齐             */
    for(i=0; i<Num; i++)
    {
        if ((Address+i*4) >= FLASH_MAX_ADDRESS)
        {
            break;
        }
        M32(Address+i*4) = pWriteWord[i];                                  /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
    }

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}

/*
*********************************************************************************************************
*                                         FLASH WORD READ
*
* 函数说明: Flash字读
*
* 入口参数: pReadWord     指向存储读到的数据的首地址
*
*           Address       读Flash的首地址
*
*           Num           读数据长度，以字为单位
*
* 返回参数: 无
*
* 特殊说明: 1)用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*           2)用户应保证传递给函数的地址为至少字对齐
*********************************************************************************************************
*/
void HT_Flash_WordRead(uint32_t* pReadWord, uint32_t Address, uint32_t Num)
{
    /*  assert_param  */
    uint32_t i;
    uint32_t writeProtect;

    if (Address >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];             /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */

    Address &= 0xFFFFFFFC;                                                 /*!< 保证字对齐             */
    for(i=0; i<Num; i++)
    {
        if ((Address+i*4) >= FLASH_MAX_ADDRESS)
        {
            return;
        }
        pReadWord[i] = M32(Address+i*4);                                   /*!< 执行Flash读            */
    }
}

/*
*********************************************************************************************************
*                                         FLASH PAGE ERASE
*
* 函数说明: Flash页擦除
*
* 入口参数: EraseAddress    页擦除地址
*
* 返回参数: 无
*
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变，1K bytes/page
*********************************************************************************************************
*/
void HT_Flash_PageErase(uint32_t EraseAddress)
{
    /*  assert_param  */
    uint32_t writeProtect;

    if (EraseAddress >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];             /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_PAGEERASE;                       /*!< Flash页擦除            */
#else
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_PAGEERASE;                         /*!< Flash页擦除            */
#endif

    EraseAddress &= 0xFFFFFFFC;                                            /*!< 保证字对齐             */
    M32(EraseAddress) = 0xFF;                                              /*!< 执行Flash页擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}

/*
*********************************************************************************************************
*                                         FLASH CHIP ERASE
*
* 函数说明: Flash全擦除
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*********************************************************************************************************
*/
void HT_Flash_ChipErase(void)
{
    /*  assert_param  */
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_CHIPERASE;                         /*!< Flash擦除              */

    M32(0x00000000) = 0xFF;                                                /*!< 执行Flash全擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}

#if defined  HT6x3x
/*
*********************************************************************************************************
*                                         FLASH BLOCK CHIP ERASE
*
* 函数说明: Flash 256K擦除 (for HT6x3x)
*
* 入口参数: EraseBlock    块擦除区域
*
* 返回参数: 无
*
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变
*********************************************************************************************************
*/
void HT_Flash_BlockChipErase(FLASH_BlockEraseTypeDef EraseBlock)
{
    /*  assert_param  */
    uint32_t writeProtect;

    if (EraseBlock > FlashBlock2)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];             /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    if (EraseBlock == FlashBlock1)
    {
        HT_CMU->FLASHLOCK_L256 = CMU_FLASHLOCK_L256_UnLocked;              /*!< Flash解锁              */
    }
    else
    {
        HT_CMU->FLASHLOCK_H256 = CMU_FLASHLOCK_H256_UnLocked;              /*!< Flash解锁              */
    }
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_CHIPERASE;                         /*!< Flash擦除              */

    if (EraseBlock == FlashBlock1)
    {
        M32(0x00000000) = 0xFF;                                            /*!< 执行Flash 256k擦除     */
    }
    else
    {
        M32(0x00040000) = 0xFF;                                            /*!< 执行Flash 256k擦除     */
    }
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    if (EraseBlock == FlashBlock1)
    {
        HT_CMU->FLASHLOCK_L256 = CMU_FLASHLOCK_L256_Locked;                /*!< Flash锁定              */
    }
    else
    {
        HT_CMU->FLASHLOCK_H256 = CMU_FLASHLOCK_H256_Locked;                /*!< Flash锁定              */
    }

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}
#endif

/*
*********************************************************************************************************
*                                         FLASH PAGE ERASE WITH SECTION UNLOCK
*
* 函数说明: Flash页擦除(仅Section解锁)    (for HT6x3x, HT502x)
*
* 入口参数: EraseAddress    页擦除地址
*
* 返回参数: 无
*
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变，1K bytes/page
*********************************************************************************************************
*/
#if defined  HT6x3x
void HT_Flash_PageEraseWithSectionUnlock(uint32_t EraseAddress)
{
    /*  assert_param  */
    uint32_t writeProtect;

    if (EraseAddress >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];              /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                  /*!< 关闭写保护功能         */

    if (EraseAddress <= 0x00001FFF )                                        /*!< Flash解锁              */
    {
        HT_CMU->FSA1LOCK = CMU_FSA1LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00002000) && (EraseAddress <= 0x00007FFF ))
    {
        HT_CMU->FSA2LOCK = CMU_FSA2LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00008000) && (EraseAddress <= 0x0000FFFF ))
    {
        HT_CMU->FSA3LOCK = CMU_FSA3LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00010000) && (EraseAddress <= 0x00017FFF ))
    {
        HT_CMU->FSA4LOCK = CMU_FSA4LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00018000) && (EraseAddress <= 0x0001FFFF ))
    {
        HT_CMU->FSA5LOCK = CMU_FSA5LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00020000) && (EraseAddress <= 0x00027FFF ))
    {
        HT_CMU->FSA6LOCK = CMU_FSA6LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00028000) && (EraseAddress <= 0x0002FFFF ))
    {
        HT_CMU->FSA7LOCK = CMU_FSA7LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00030000) && (EraseAddress <= 0x00037FFF ))
    {
        HT_CMU->FSA8LOCK = CMU_FSA8LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00038000) && (EraseAddress <= 0x0003FBFF ))
    {
        HT_CMU->FSA9LOCK = CMU_FSA9LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x0003FC00) && (EraseAddress <= 0x0003FFFF ))
    {
        HT_CMU->FSAALOCK = CMU_FSAALOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00040000) && (EraseAddress <= 0x00041FFF ))
    {
        HT_CMU->FSB1LOCK = CMU_FSB1LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00042000) && (EraseAddress <= 0x00047FFF ))
    {
        HT_CMU->FSB2LOCK = CMU_FSB2LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00048000) && (EraseAddress <= 0x0004FFFF ))
    {
        HT_CMU->FSB3LOCK = CMU_FSB3LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00050000) && (EraseAddress <= 0x00057FFF ))
    {
        HT_CMU->FSB4LOCK = CMU_FSB4LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00058000) && (EraseAddress <= 0x0005FFFF ))
    {
        HT_CMU->FSB5LOCK = CMU_FSB5LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00060000) && (EraseAddress <= 0x00067FFF ))
    {
        HT_CMU->FSB6LOCK = CMU_FSB6LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00068000) && (EraseAddress <= 0x0006FFFF ))
    {
        HT_CMU->FSB7LOCK = CMU_FSB7LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00070000) && (EraseAddress <= 0x00077FFF ))
    {
        HT_CMU->FSB8LOCK = CMU_FSB8LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00078000) && (EraseAddress <= 0x0007FBFF ))
    {
        HT_CMU->FSB9LOCK = CMU_FSB9LOCK_UnLocked;
    }
    else
    {
        HT_CMU->FSBBLOCK = CMU_FSBBLOCK_UnLocked;
    }

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_PAGEERASE;                       /*!< Flash页擦除            */
#else
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_PAGEERASE;                         /*!< Flash页擦除            */
#endif

    EraseAddress &= 0xFFFFFFFC;                                            /*!< 保证字对齐             */
    M32(EraseAddress) = 0xFF;                                              /*!< 执行Flash页擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif

    if (EraseAddress <= 0x00001FFF )                                       /*!< Flash解锁              */
    {
        HT_CMU->FSA1LOCK = CMU_FSA1LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00002000) && (EraseAddress <= 0x00007FFF ))
    {
        HT_CMU->FSA2LOCK = CMU_FSA2LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00008000) && (EraseAddress <= 0x0000FFFF ))
    {
        HT_CMU->FSA3LOCK = CMU_FSA3LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00010000) && (EraseAddress <= 0x00017FFF ))
    {
        HT_CMU->FSA4LOCK = CMU_FSA4LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00018000) && (EraseAddress <= 0x0001FFFF ))
    {
        HT_CMU->FSA5LOCK = CMU_FSA5LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00020000) && (EraseAddress <= 0x00027FFF ))
    {
        HT_CMU->FSA6LOCK = CMU_FSA6LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00028000) && (EraseAddress <= 0x0002FFFF ))
    {
        HT_CMU->FSA7LOCK = CMU_FSA7LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00030000) && (EraseAddress <= 0x00037FFF ))
    {
        HT_CMU->FSA8LOCK = CMU_FSA8LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00038000) && (EraseAddress <= 0x0003FBFF ))
    {
        HT_CMU->FSA9LOCK = CMU_FSA9LOCK_Locked;
    }
    else if ((EraseAddress >= 0x0003FC00) && (EraseAddress <= 0x0003FFFF ))
    {
        HT_CMU->FSAALOCK = CMU_FSAALOCK_Locked;
    }
    else if ((EraseAddress >= 0x00040000) && (EraseAddress <= 0x00041FFF ))
    {
        HT_CMU->FSB1LOCK = CMU_FSB1LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00042000) && (EraseAddress <= 0x00047FFF ))
    {
        HT_CMU->FSB2LOCK = CMU_FSB2LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00048000) && (EraseAddress <= 0x0004FFFF ))
    {
        HT_CMU->FSB3LOCK = CMU_FSB3LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00050000) && (EraseAddress <= 0x00057FFF ))
    {
        HT_CMU->FSB4LOCK = CMU_FSB4LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00058000) && (EraseAddress <= 0x0005FFFF ))
    {
        HT_CMU->FSB5LOCK = CMU_FSB5LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00060000) && (EraseAddress <= 0x00067FFF ))
    {
        HT_CMU->FSB6LOCK = CMU_FSB6LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00068000) && (EraseAddress <= 0x0006FFFF ))
    {
        HT_CMU->FSB7LOCK = CMU_FSB7LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00070000) && (EraseAddress <= 0x00077FFF ))
    {
        HT_CMU->FSB8LOCK = CMU_FSB8LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00078000) && (EraseAddress <= 0x0007FBFF ))
    {
        HT_CMU->FSB9LOCK = CMU_FSB9LOCK_Locked;
    }
    else
    {
        HT_CMU->FSBBLOCK = CMU_FSBBLOCK_Locked;
    }

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}

#elif  defined  HT502x
void HT_Flash_PageEraseWithSectionUnlock(uint32_t EraseAddress)
{
    /*  assert_param  */
    uint32_t writeProtect;

    if (EraseAddress >= FLASH_MAX_ADDRESS)
    {
        return;
    }

    writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];              /*!< 保存当前写保护状态     */
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                  /*!< 关闭写保护功能         */

    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                               /*!< Flash锁定              */
    if (EraseAddress <= 0x00001FFF )                                        /*!< Flash解锁              */
    {
        HT_CMU->FS1LOCK = CMU_FS1LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00002000) && (EraseAddress <= 0x00007FFF ))
    {
        HT_CMU->FS2LOCK = CMU_FS2LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00008000) && (EraseAddress <= 0x0000FFFF ))
    {
        HT_CMU->FS3LOCK = CMU_FS3LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00010000) && (EraseAddress <= 0x00017FFF ))
    {
        HT_CMU->FS4LOCK = CMU_FS4LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00018000) && (EraseAddress <= 0x0001FBFF ))
    {
        HT_CMU->FS5LOCK = CMU_FS5LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x0001FC00) && (EraseAddress <= 0x0001FFFF ))
    {
        HT_CMU->FSALOCK = CMU_FSALOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00020000) && (EraseAddress <= 0x00027FFF ))
    {
        HT_CMU->FS6LOCK = CMU_FS6LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00028000) && (EraseAddress <= 0x0002FFFF ))
    {
        HT_CMU->FS7LOCK = CMU_FS7LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00030000) && (EraseAddress <= 0x00037FFF ))
    {
        HT_CMU->FS8LOCK = CMU_FS8LOCK_UnLocked;
    }
    else if ((EraseAddress >= 0x00038000) && (EraseAddress <= 0x0003FBFF ))
    {
        HT_CMU->FS9LOCK = CMU_FS9LOCK_UnLocked;
    }
    else
    {
        HT_CMU->FSBLOCK = CMU_FSBLOCK_UnLocked;
    }

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_PAGEERASE;                       /*!< Flash页擦除            */
#else
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_PAGEERASE;                         /*!< Flash页擦除            */
#endif

    EraseAddress &= 0xFFFFFFFC;                                            /*!< 保证字对齐             */
    M32(EraseAddress) = 0xFF;                                              /*!< 执行Flash页擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */

#if  defined  CMU_FLASHCON2
    HT_CMU->FLASHCON2 = CMU_FLASHCON2_FOP_READ;                            /*!< Flash读                */
#else
    HT_CMU->FLASHCON  = CMU_FLASHCON_FOP_READ;                             /*!< Flash读                */
#endif

    if (EraseAddress <= 0x00001FFF )                                       /*!< Flash锁定              */
    {
        HT_CMU->FS1LOCK = CMU_FS1LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00002000) && (EraseAddress <= 0x00007FFF ))
    {
        HT_CMU->FS2LOCK = CMU_FS2LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00008000) && (EraseAddress <= 0x0000FFFF ))
    {
        HT_CMU->FS3LOCK = CMU_FS3LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00010000) && (EraseAddress <= 0x00017FFF ))
    {
        HT_CMU->FS4LOCK = CMU_FS4LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00018000) && (EraseAddress <= 0x0001FBFF ))
    {
        HT_CMU->FS5LOCK = CMU_FS5LOCK_Locked;
    }
    else if ((EraseAddress >= 0x0001FC00) && (EraseAddress <= 0x0001FFFF ))
    {
        HT_CMU->FSALOCK = CMU_FSALOCK_Locked;
    }
    else if ((EraseAddress >= 0x00020000) && (EraseAddress <= 0x00027FFF ))
    {
        HT_CMU->FS6LOCK = CMU_FS6LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00028000) && (EraseAddress <= 0x0002FFFF ))
    {
        HT_CMU->FS7LOCK = CMU_FS7LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00030000) && (EraseAddress <= 0x00037FFF ))
    {
        HT_CMU->FS8LOCK = CMU_FS8LOCK_Locked;
    }
    else if ((EraseAddress >= 0x00038000) && (EraseAddress <= 0x0003FBFF ))
    {
        HT_CMU->FS9LOCK = CMU_FS9LOCK_Locked;
    }
    else
    {
        HT_CMU->FSBLOCK = CMU_FSBLOCK_Locked;
    }

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}
#endif
