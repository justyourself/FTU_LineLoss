/*
**********************************************************************************************************
*                                              HT60XX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT60xx
* File         : example.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
**********************************************************************************************************
*/

#ifndef __AES_EXAMPLE_H__
#define __AES_EXAMPLE_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
#include "ht6xxx_aes&rand.h"
	
#if defined HT6x2x  ||  defined  HT501x             /* This File Only support HT6x2x and HT501x */ 
    
/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/
	
#pragma pack(8)
/* 
* @brief  GCM加密输入变量结构定义
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//加密解密模式选择
			uint32_t *pKeyAddr;									//密钥地址指针
			uint8_t *pInitVectorAddr;						//初始向量地址指针
			uint64_t LenIV;											//初始向量数据长度（byte）
			uint8_t	*pPtextAddr;								//明文数据地址指针
			uint64_t LenPtext;									//明文数据长度（byte）
			uint8_t	*pAdataAddr;								//附加认证数据地址指针
			uint64_t LenAdata;									//附加认证数据长度（byte）
}GCM_Encrypt_InputTypedef;										/*!< end of group GCM_Encrypt_InputTypedef*/

/* 
* @brief  GCM加密输出变量结构定义
*/
typedef struct
{
			uint8_t	*pCtextAddr;									//已加密数据密文地址指针
			uint64_t LenCtext;										//已加密数据密文长度（byte）
			Parameter128bits_TypeDef*	AuthTag;		//认证标签数据地址指针
}GCM_Encrypt_OutputTypedef;											/*!< end of group GCM_Encrypt_OutputTypedef*/

/* 
* @brief  GCM解密输入变量结构定义
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//加密解密模式选择
			uint32_t *pKeyAddr;									//密钥地址指针
			uint8_t *pInitVectorAddr;						//初始向量地址指针
			uint64_t LenIV;											//初始向量数据长度（byte）
			uint8_t	*pCtextAddr;								//密文数据地址指针
			uint64_t LenCtext;									//密文数据长度（byte）
			uint8_t	*pAdataAddr;								//附加认证数据地址指针
			uint64_t LenAdata;									//附加认证数据长度（byte）
			Parameter128bits_TypeDef*	AuthTag;	//认证标签数据地址指针
}GCM_Decrypt_InputTypedef;										/*!< end of group GCM_Encrypt_InputTypedef*/


/* 
* @brief  GCM解密输出变量结构定义
*/
typedef struct
{
			uint8_t	*pPtextAddr;									//已解密明文数据地址指针
			uint64_t LenPtext;										//已解密明文数据长度（byte）
}GCM_Decrypt_OutputTypedef;									/*!< end of group GCM_Encrypt_OutputTypedef*/
#pragma pack()

/* 
* @brief  ECB加密输入变量结构定义
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//加密解密模式选择
			uint32_t *pKeyAddr;									//密钥地址指针
			uint8_t	*pPtextAddr;								//明文数据地址指针
			uint64_t LenPtext;									//明文数据长度（byte）
}ECB_Encrypt_InputTypedef;										/*!< end of group ECB_Encrypt_InputTypedef*/

/* 
* @brief  ECB加密输出变量结构定义
*/
typedef struct
{
			uint8_t	*pCtextAddr;									//已加密数据密文地址指针
			uint64_t LenCtext;										//已加密数据密文长度（byte）
}ECB_Encrypt_OutputTypedef;											/*!< end of group ECB_Encrypt_OutputTypedef*/

/* 
* @brief  ECB解密输入变量结构定义
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//加密解密模式选择
			uint32_t *pKeyAddr;									//密钥地址指针
			uint8_t	*pCtextAddr;								//密文数据地址指针
			uint64_t LenCtext;									//密文数据长度（byte）
}ECB_Decrypt_InputTypedef;										/*!< end of group ECB_Decrypt_InputTypedef*/

/* 
* @brief  ECB解密输出变量结构定义
*/
typedef struct
{
			uint8_t	*pPtextAddr;									//已解密明文地址指针
			uint64_t LenPtext;										//已解密明文长度（byte）
}ECB_Decrypt_OutputTypedef;											/*!< end of group ECB_Decrypt_OutputTypedef*/

/* 
* @brief  CBC加密输入变量结构定义
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//加密解密模式选择
			uint32_t *pKeyAddr;									//密钥地址指针
			uint8_t	*pPtextAddr;								//明文数据地址指针
			uint64_t LenPtext;									//明文数据长度（byte）
			Parameter128bits_TypeDef* pInitVectorAddr;				//初始向量地址指针
}CBC_Encrypt_InputTypedef;										/*!< end of group CBC_Encrypt_InputTypedef*/

/* 
* @brief  CBC加密输出变量结构定义
*/
typedef struct
{
			uint8_t	*pCtextAddr;									//已加密数据密文地址指针
			uint64_t LenCtext;										//已加密数据密文长度（byte）
}CBC_Encrypt_OutputTypedef;											/*!< end of group CBC_Encrypt_OutputTypedef*/

/* 
* @brief  CBC解密输入变量结构定义
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//加密解密模式选择
			uint32_t *pKeyAddr;									//密钥地址指针
			uint8_t	*pCtextAddr;								//密文数据地址指针
			uint64_t LenCtext;									//密文数据长度（byte）
			Parameter128bits_TypeDef* pInitVectorAddr;				//初始向量地址指针
}CBC_Decrypt_InputTypedef;										/*!< end of group CBC_Decrypt_InputTypedef*/

/* 
* @brief  CBC解密输出变量结构定义
*/
typedef struct
{
			uint8_t	*pPtextAddr;									//已解密明文地址指针
			uint64_t LenPtext;										//已解密明文长度（byte）
}CBC_Decrypt_OutputTypedef;											/*!< end of group CBC_Decrypt_OutputTypedef*/

/* 
* @brief  CTR加/解密输入变量结构定义
*/
typedef struct
{
			AESKeyMode_TypeDef AESKeyMode;			//加密解密模式选择
			uint32_t *pKeyAddr;									//密钥地址指针
			Parameter128bits_TypeDef* Counter0;	//Counter0地址指针
			uint8_t	*pInttextAddr;							//待加解密数据地址指针
			uint64_t LenInttext;								//待加解密数据长度（byte）
}CTR_Encrypt_InputTypedef;										/*!< end of group CTR_Encrypt_InputTypedef*/


/* 
* @brief  GCM加密输出变量结构定义
*/
typedef struct
{
			uint8_t	*pOuttextAddr;							//已加/解密数据密文地址指针
			uint64_t LenOuttext;								//已加/解密数据密文长度（byte）
}CTR_Encrypt_OutputTypedef;									/*!< end of group GCM_Encrypt_OutputTypedef*/

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
void GCM_GhashHeader_init(AESKeyMode_TypeDef AESKeyMode,uint32_t *pKeyAddr,uint32_t *pOutDataAddr);
void GCM_GhashAuth(uint32_t *GhashHeaderAddr,uint8_t *pADataAddr,uint64_t LenAData,uint8_t *pCtextAddr,uint64_t LenCtext,Parameter128bits_TypeDef* OutData);
void GCM_Counter0_init(uint8_t *pInitVectorAddr,uint64_t LenIV,uint32_t *GhashHeaderAddr,Parameter128bits_TypeDef* Counter0);
void GCM_Counter_increase(Parameter128bits_TypeDef* Counter);
void GCM_CTR_Encrypt(uint8_t *pInttextAddr,uint64_t LenInttext,Parameter128bits_TypeDef* Counter0,AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr,uint8_t *pOuttextAddr);
void GCM_TagAuth(Parameter128bits_TypeDef* GhashAuthData,Parameter128bits_TypeDef* Counter0,AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr,Parameter128bits_TypeDef* AuthTag);

void GCM_Encrypt(int COM_No,GCM_Encrypt_InputTypedef* GCM_InputStruct, GCM_Encrypt_OutputTypedef* GCM_OutputStruct);
Bool GCM_Decrypt(int COM_No,GCM_Decrypt_InputTypedef* GCM_InputStruct, GCM_Decrypt_OutputTypedef* GCM_OutputStruct);

void ECB_Encrypt(ECB_Encrypt_InputTypedef* ECB_InputStruct,ECB_Encrypt_OutputTypedef* ECB_OutputStruct);
Bool ECB_Decrypt(ECB_Decrypt_InputTypedef* ECB_InputStruct,ECB_Decrypt_OutputTypedef* ECB_OutputStruct);

void CBC_Encrypt(CBC_Encrypt_InputTypedef* CBC_InputStruct,CBC_Encrypt_OutputTypedef* CBC_OutputStruct);
Bool CBC_Decrypt(CBC_Decrypt_InputTypedef* CBC_InputStruct,CBC_Decrypt_OutputTypedef* CBC_OutputStruct);

void CTR_Encrypt(CTR_Encrypt_InputTypedef* CTR_InputStruct,CTR_Encrypt_OutputTypedef* CTR_OutputStruct);

void AES_GCM_Example(uint8_t TestCase);
void GCM_TestCase_Init(uint8_t TestCase);

void AES_CTR_Example(void);
void CTR_TestCase_Init(void);

void AES_CBC_Example(void);
void CBC_TestCase_Init(void);

void AES_ECB_Example(void);
void ECB_TestCase_Init(void);
int AES_GCM_Decrypt_OR_Encrypt_H( unsigned short COM_No,unsigned char* Ciphert,unsigned short *nCtextLen,unsigned char use_crypt );   //判断解密类型，做类型标识输出加解密密文,use_crypt:0--输入，1--加密..
int Fun_Key_Wrap_128Bits(unsigned char* key,unsigned char* Ciphert,unsigned char* Plaint);
int Fun_Key_Unwrap_128Bits(unsigned char* key,unsigned char* Ciphert,unsigned char* Plaint);
void Test_KeyWrap(void);

#endif                                        /* This File Only support HT6x2x and HT501x */

#ifdef __cplusplus
}
#endif

#endif /* __EXAMPLE_H__ */ 
