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
* Version      : V1.0.1
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
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
    
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flash写                */
    
    for(i=0; i<Num; i++)
    {
        M8(Address+i) = pWriteByte[i];                                     /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
    }
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
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
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    
    for(i=0; i<Num; i++)
    {
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
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
    
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flash写                */
    
    Address &= 0xFFFFFFFE;                                                 /*!< 保证半字对齐           */
    for(i=0; i<Num; i++)
    {
        M16(Address+i*2) = pWriteHalfWord[i];                              /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
    }
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
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
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    Address &= 0xFFFFFFFE;                                                 /*!< 保证半字对齐           */
    for(i=0; i<Num; i++)
    {
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
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
    
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_WRITE;                             /*!< Flash写                */
    
    Address &= 0xFFFFFFFC;                                                 /*!< 保证字对齐             */   
    for(i=0; i<Num; i++)
    {
        M32(Address+i*4) = pWriteWord[i];                                  /*!< 执行Flash写            */
        while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                       /*!< 等待写完成             */
    }
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
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
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    Address &= 0xFFFFFFFC;                                                 /*!< 保证字对齐             */     
    for(i=0; i<Num; i++)
    {
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
    
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
    
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_UnLocked;                            /*!< Flash解锁              */
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_PAGEERASE;                         /*!< Flash页擦除            */  
    
    EraseAddress &= 0xFFFFFFFC;                                            /*!< 保证字对齐             */
    M32(EraseAddress) = 0xFF;                                              /*!< 执行Flash页擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
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
    
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_CHIPERASE;                         /*!< Flash页擦除            */  

    M32(0x1000) = 0xFF;                                                    /*!< 执行Flash全擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */
    
    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
     
}

#if defined  HT502x
/*
*********************************************************************************************************
*                                         FLASH PAGE ERASE WITH SECTION UNLOCK
*
* 函数说明: Flash页擦除(仅Section解锁)
*
* 入口参数: EraseAddress    页擦除地址
*
* 返回参数: 无                                      
* 
* 特殊说明: 用户应保证函数执行过程中寄存器写保护状态以及Flash解锁状态不变，1K bytes/page
*********************************************************************************************************
*/
void HT_Flash_PageEraseWithSectionUnlock(uint32_t EraseAddress)
{
    /*  assert_param  */
    
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */
	
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
		else if ((EraseAddress >= 0x0003FC00) && (EraseAddress <= 0x0003FFFF ))
		{
			 HT_CMU->FSBLOCK = CMU_FSBLOCK_UnLocked;
		}
				
    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_PAGEERASE;                         /*!< Flash页擦除            */  
    
    EraseAddress &= 0xFFFFFFFC;                                            /*!< 保证字对齐             */
    M32(EraseAddress) = 0xFF;                                              /*!< 执行Flash页擦除        */
    while(HT_CMU->FLASHCON & CMU_FLASHCON_BUSY);                           /*!< 等待写完成             */

    HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;                              /*!< Flash读                */
    HT_CMU->FLASHLOCK = CMU_FLASHLOCK_Locked;                              /*!< Flash锁定              */
    
    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
     
}
#endif

