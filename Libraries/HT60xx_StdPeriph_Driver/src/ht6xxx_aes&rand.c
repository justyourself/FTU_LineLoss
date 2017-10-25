/*
*********************************************************************************************************
*                                              HT6XXXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx_aes&rand.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT6x2x, HT501x and HT502x 
*********************************************************************************************************
*/

#define  __HT6XXX_AES_RAND_C

#include "ht6xxx_aes&rand.h"

#if defined HT6x2x  ||  defined  HT501x  ||  defined  HT502x             /* This File Only support HT6x2x, HT501x and HT502x */

/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/



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
*                                      AES Encrypt MODULE
*
* 函数说明: AES加密模块
*
* 入口参数: AESKeyMode			枚举变量，选择加密模式
*														KEY_Mode_128bits
*														KEY_Mode_192bits
*														KEY_Mode_256bits
*						pKeyAddr  			uint32_t *型变量，其指向对象存储AES key : pKeyAddr[0]为高位
*                       
*						pIntDataAddr  	uint32_t *型变量，其指向对象存储待加密数据（明文）: pIntDataAddr[0]为低位
*   
*
* 返回参数: 无                                      
* 
* 特殊说明: 
*********************************************************************************************************
*/
void HT_AES_Encrypt(AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr, uint32_t *pIntDataAddr)
{
			if(AESKeyMode == KEY_Mode_128bits)
			{
					HT_AES->AESCON = 0x00; 										/*!< key-128bits,加密模式							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< 密钥写入密钥寄存器								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr;
			}
			else if(AESKeyMode == KEY_Mode_192bits)
			{
					HT_AES->AESCON = 0x02; 										/*!< key-192bits,加密模式							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< 密钥写入密钥寄存器								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr++;
					HT_AES->AESKEY4 = *pKeyAddr++;
					HT_AES->AESKEY5 = *pKeyAddr;
			}
			else if(AESKeyMode == KEY_Mode_256bits)
			{
					HT_AES->AESCON = 0x04; 										/*!< key-256bits,加密模式							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< 密钥写入密钥寄存器								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr++;
					HT_AES->AESKEY4 = *pKeyAddr++;
					HT_AES->AESKEY5 = *pKeyAddr++;
					HT_AES->AESKEY6 = *pKeyAddr++;
					HT_AES->AESKEY7 = *pKeyAddr;
			}
			
			HT_AES->AESINLL = *pIntDataAddr++;					/*!< 待加密数据（明文）写入寄存器				*/
			HT_AES->AESINML = *pIntDataAddr++;
			HT_AES->AESINHM =	*pIntDataAddr++;
			HT_AES->AESINHH = *pIntDataAddr;
			
			HT_AES->AESSTR = 0x8329;										/*!< 启动AES加密												*/
}

/*
*********************************************************************************************************
*                                      AES Decrypt MODULE
*
* 函数说明: AES解密模块
*
* 入口参数: AESKeyMode			枚举变量，选择解密模式
*														KEY_Mode_128bits
*														KEY_Mode_192bits
*														KEY_Mode_256bits
*						pKeyAddr  			uint32_t *型变量，其指向对象存储AES key : pKeyAddr[0]为高位
*                       
*						pIntDataAddr  	uint32_t *型变量，其指向对象存储待解密数据（密文）: pIntDataAddr[0]为低位
*   
*
* 返回参数: 无                                      
* 
* 特殊说明: 
*********************************************************************************************************
*/
void HT_AES_Decrypt(AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr, uint32_t *pIntDataAddr)
{
			if(AESKeyMode == KEY_Mode_128bits)
			{
					HT_AES->AESCON = 0x01; 										/*!< key-128bits,解密模式							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< 密钥写入密钥寄存器								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr;
			}
			else if(AESKeyMode == KEY_Mode_192bits)
			{
					HT_AES->AESCON = 0x03; 										/*!< key-192bits,解密模式							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< 密钥写入密钥寄存器								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr++;
					HT_AES->AESKEY4 = *pKeyAddr++;
					HT_AES->AESKEY5 = *pKeyAddr;
			}
			else if(AESKeyMode == KEY_Mode_256bits)
			{
					HT_AES->AESCON = 0x05; 										/*!< key-256bits,解密模式							*/
					HT_AES->AESKEY0 = *pKeyAddr++;						/*!< 密钥写入密钥寄存器								*/
					HT_AES->AESKEY1 = *pKeyAddr++;
					HT_AES->AESKEY2 = *pKeyAddr++;
					HT_AES->AESKEY3 = *pKeyAddr++;
					HT_AES->AESKEY4 = *pKeyAddr++;
					HT_AES->AESKEY5 = *pKeyAddr++;
					HT_AES->AESKEY6 = *pKeyAddr++;
					HT_AES->AESKEY7 = *pKeyAddr;
			}
			
			HT_AES->AESINLL = *pIntDataAddr++;					/*!< 待解密数据（密文）写入寄存器				*/
			HT_AES->AESINML = *pIntDataAddr++;
			HT_AES->AESINHM =	*pIntDataAddr++;
			HT_AES->AESINHH = *pIntDataAddr;
			
			HT_AES->AESSTR = 0x8329;										/*!< 启动AES解密												*/
}

/*
*********************************************************************************************************
*                                      GHASH 乘法运算 MODULE
*
* 函数说明: GHASH 乘法运算模块
*
* 入口参数: 
*						pIntDataAddr1		uint32_t *型变量，其指向对象存储乘数1数据 : pIntDataAddr1[0]为低位
*                       
*						pIntDataAddr2  	uint32_t *型变量，其指向对象存储乘数2数据 : pIntDataAddr2[0]为低位
*   
*
* 返回参数: 无                                      
* 
* 特殊说明: 
*********************************************************************************************************
*/
void HT_GHASH_Multiply(uint32_t *pIntDataAddr1, uint32_t *pIntDataAddr2)
{
		HT_GHASH->INPUT1LL = *pIntDataAddr1++;				/*!< 写入乘数1													*/
		HT_GHASH->INPUT1ML = *pIntDataAddr1++;
		HT_GHASH->INPUT1HM = *pIntDataAddr1++;
		HT_GHASH->INPUT1HH = *pIntDataAddr1;
		
		HT_GHASH->INPUT2LL = *pIntDataAddr2++;				/*!< 写入乘数2													*/
		HT_GHASH->INPUT2ML = *pIntDataAddr2++;
		HT_GHASH->INPUT2HM = *pIntDataAddr2++;
		HT_GHASH->INPUT2HH = *pIntDataAddr2;
	
		HT_GHASH->GHASHSTR = 0x0501;									/*!< 启动GHASH乘法											*/
		
}
/*
*********************************************************************************************************
*                                      AES/GHASH乘法输出数据读取 MODULE
*
* 函数说明: AES/GHASH乘法输出数据读取
*
* 入口参数: 
                     
*						pOutDataAddr  	uint32_t *型变量，其指向对象存储待解密数据（密文）: pIntDataAddr[0]为低位
*   
*
* 返回参数: 无                                      
* 
* 特殊说明: 
*********************************************************************************************************
*/

void HT_AES_Read(uint32_t *pOutDataAddr)
{
		*pOutDataAddr++ = HT_AES->AESOUTLL;
		*pOutDataAddr++ = HT_AES->AESOUTML;
		*pOutDataAddr++ = HT_AES->AESOUTHM;
		*pOutDataAddr 	= HT_AES->AESOUTHH;
}

void HT_GHASH_Multiply_Read(uint32_t *pOutDataAddr)
{
		*pOutDataAddr++ = HT_GHASH->OUTPUTLL;
		*pOutDataAddr++ = HT_GHASH->OUTPUTML;
		*pOutDataAddr++ = HT_GHASH->OUTPUTHM;
		*pOutDataAddr   = HT_GHASH->OUTPUTHH;
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE AES/GHASH/RAND INTERRUPT    
*
* 函数说明: 使能或者关闭AES/GHASH/RAND中断
*
* 入口参数: ITEn       AES/GHASH/RAND中断设置位，可以为以下参数或其组合
*                        @arg AES_IE
*                        @arg GHASH_IE
*                        @arg RAND_IE
*
*
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_AES_GHASH_RAND_ITConfig(AES_ITEnTypeDef ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_GHASH->AESGHASHIE |= (uint32_t)ITEn;            /*!< 使能AES/GHASH/RAND中断           */
    }
    else
    {
        HT_GHASH->AESGHASHIE &= ~(uint32_t)ITEn;           /*!< 关闭AES/GHASH/RAND中断           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED AES/GHASH/RAND INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应AES/GHASH/RAND中断标志状态
*
* 入口参数: ITFlag     想要检查的AES/GHASH/RAND中断，可以为以下参数:
*                        @arg AES_IF
*                        @arg GHASH_IF
*                        @arg RAND_IF
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
* 
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_AES_GHASH_RAND_ITFlagStatusGet(AES_ITFlagTypeDef ITFlag)
{
    /*  assert_param  */
    
    if (HT_GHASH->AESGHASHIF & ITFlag)
    {       
        return SET;                        /*!< AES/GHASH/RAND Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< AES/GHASH/RAND Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR AES/GHASH/RAND INTERRUPT FLAG
*
* 函数说明: 清除AES/GHASH/RAND中断标志
*
* 入口参数: ITFlag     想要清除的某个RTC中断标志，可以为以下参数或其组合:
*                        @arg AES_IF
*                        @arg GHASH_IF
*                        @arg RAND_IF
*
* 返回参数: 无
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_AES_GHASH_RAND_ClearITPendingBit(AES_ITFlagTypeDef ITFlag)
{
    /*  assert_param  */
    
      
    HT_GHASH->AESGHASHIF  &= ~((uint32_t)ITFlag);                  /*!< Clear AES/GHASH/RAND Interrupt Flag       */
    
}

/*
*********************************************************************************************************
*                            GET AES/GHASH/RAND BUSY STATUS
*
* 函数说明: 获取相应AES/GHASH/RAND中BUSY状态
*
* 入口参数: 无
*
* 返回参数: ITStatus    = SET：  busy忙碌
*                       = RESET：busy空闲
* 
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_AES_Busy_StatusGet(void)
{
    /*  assert_param  */
    return (ITStatus)HT_AES->AESFLG;                        		/*!< 返回AES busy status  				*/
}

ITStatus HT_GHASH_Busy_StatusGet(void)
{
    /*  assert_param  */
    return (ITStatus)HT_GHASH->GHASHFLG;                        /*!< 返回GHASHCH乘法 busy status  */
}

/*
*********************************************************************************************************
*                          128bits  xor function
*
* 函数说明: 128bits 异或函数
*
* 入口参数: 
*						pIntDataAddr1		uint32_t *型变量，其指向对象存储异或前数据1 : pIntDataAddr1[0]为低位
*                       
*						pIntDataAddr2  	uint32_t *型变量，其指向对象存储异或前数据2:  pIntDataAddr2[0]为低位
*
* 返回参数: 
*						pOutDataAddr  	uint32_t *型变量，其指向对象存储异或后数据 : pOutDataAddr[0]为低位
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_AES_Xor128bits(uint32_t *pIntDataAddr1, uint32_t *pIntDataAddr2, uint32_t *pOutDataAddr)
{
    /*  assert_param  */
		uint8_t i;
	
		for(i=0;i<4;i++)
		{
			*pOutDataAddr++ = (*pIntDataAddr1++) ^ (*pIntDataAddr2++);		/*!< 异或计算 				*/
		}
           		
}



#endif                                        /* This File Only support HT6x2x and HT501x */

