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
* File         : ht6xxx_aes&rand.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT6x2x, HT501x and HT502x 
**********************************************************************************************************
*/

#ifndef __HT6XXX_AES_RAND_H__
#define __HT6XXX_AES_RAND_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
    
#if defined HT6x2x  ||  defined  HT501x ||  defined  HT502x             /* This File Only support HT6x2x, HT501x and HT502x */    

    
/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/
/* 
* @brief  AES_key长度模式定义
*/
typedef enum
{ 
    KEY_Mode_128bits = AES_AESCON_KEYMODE_AES128,       /*!< AES-128                     */
    KEY_Mode_192bits = AES_AESCON_KEYMODE_AES192,       /*!< AES-192                     */
    KEY_Mode_256bits = AES_AESCON_KEYMODE_AES256,       /*!< AES-256                  		*/
}AESKeyMode_TypeDef;                										/*!<end of group AESKeyMode_TypeDef  */	

/* 
* @brief  AES/GHASH/RAND中断使能定义
*/
typedef enum
{ 
    AES_IE = GHASH_AESGHASHIE_AESIE,       							/*!< AES中断使能                    */
    GHASH_IE = GHASH_AESGHASHIE_GHASHIE,       					/*!< GHASH中断使能                   */
		RAND_IE = GHASH_AESGHASHIE_RANDIE,       						/*!< RAND中断使能                   */
}AES_ITEnTypeDef;  																			/*!<end of group AES_ITEnTypeDef  */	

/* 
* @brief  AES/GHASH/RAND中断标志定义
*/
typedef enum
{ 
    AES_IF = GHASH_AESGHASHIF_AESIF,       							/*!< AES中断标志                    */
    GHASH_IF = GHASH_AESGHASHIF_GHASHIF,       					/*!< GHASH中断标志                   */
		RAND_IF = GHASH_AESGHASHIF_RANDIF,       						/*!< RAND中断标志                 */
}AES_ITFlagTypeDef;  			

typedef union
{
	uint8_t u8[16];
	uint32_t u32[4];
	uint64_t u64[2];
} Parameter128bits_TypeDef;

typedef union
{
	uint8_t u8[16*2];
	uint32_t u32[4*2];
	uint64_t u64[2*2];
} Parameter256bits_TypeDef;

typedef union
{
	uint8_t u8[16*4];
	uint32_t u32[4*4];
	uint64_t u64[2*4];
} Parameter512bits_TypeDef;

typedef union
{
	uint8_t u8[16*8];
	uint32_t u32[4*8];
	uint64_t u64[2*8];
} Parameter1024bits_TypeDef;
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
void HT_AES_Encrypt(AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr, uint32_t *pIntDataAddr);
void HT_AES_Decrypt(AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr, uint32_t *pIntDataAddr);
void HT_GHASH_Multiply(uint32_t *pIntDataAddr1, uint32_t *pIntDataAddr2);
void HT_AES_Read(uint32_t *pOutDataAddr);
void HT_GHASH_Multiply_Read(uint32_t *pOutDataAddr);
void HT_AES_GHASH_RAND_ITConfig(AES_ITEnTypeDef ITEn, FunctionalState NewState);
ITStatus HT_AES_GHASH_RAND_ITFlagStatusGet(AES_ITFlagTypeDef ITFlag);
void HT_AES_GHASH_RAND_ClearITPendingBit(AES_ITFlagTypeDef ITFlag);
ITStatus HT_AES_Busy_StatusGet(void);
ITStatus HT_GHASH_Busy_StatusGet(void);
void HT_AES_Xor128bits(uint32_t *pIntDataAddr1, uint32_t *pIntDataAddr2,uint32_t *pOutDataAddr);




#endif                                        /* This File Only support HT6x2x and HT501x */

#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_AES_RAND_H__ */ 
