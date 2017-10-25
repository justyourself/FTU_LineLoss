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
* File         : ecc_example.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
**********************************************************************************************************
*/

#ifndef __ECC_EXAMPLE_H__
#define __ECC_EXAMPLE_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
#include "ht6xxx_aes&rand.h"
#include "ht6xxx_ecc.h"	
	
#if defined  HT502x             /* This File Only support HT502x */
    
/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/

/* Various logical functions */
#define Ch(x,y,z)	((x & y) ^ (~x & z))
#define Maj(x,y,z)  ((x & y) ^ (x & z) ^ (y & z))
#define S(x, n)		(((x)>>((n)&31))|((x)<<(32-((n)&31))))
#define R(x, n)		((x)>>(n))
#define Sigma0(x)	(S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x)	(S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define Gamma0(x)	(S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x)	(S(x, 17) ^ S(x, 19) ^ R(x, 10))

/* 
* @brief  大数比较结果定义
*/
typedef enum
{ 
    GREATER = 0x1,       	/*!< 大于                     */
    LESS = 0x0,       		/*!< 小于                     */
    EQUAL = 0x2,       			/*!< 等于                 		*/
}BigdatCompare_TypeDef;                										/*!<end of group BigdatCompare_TypeDef  */	
	
/* 
* @brief  SHA-256 输入变量结构定义
*/
typedef struct
{
			uint8_t *pMtestAddr;									//消息地址指针
			uint64_t LenMtest;								//消息长度（byte）
}SHA256_InputTypedef;										/*!< end of group SHA256_InputTypedef*/	
	
typedef struct {
    uint32_t HashValue[8],MnCurlen;
		uint64_t MtestLen;
    uint8_t MnBuf[64];
} SHA256_TempTypedef;

typedef struct
{
		Parameter256bits_TypeDef	Piont_x;
		Parameter256bits_TypeDef	Piont_y;
}ECC_PointTypedef;						/*!< end of group ECC_PointTypedef*/


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
Bool ECDSA_Signature(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Signature_InputTypedef* ECDSA_S_InputStruct,ECC_PointOperate_OutputTypedef* ECDSA_S_OutputStruct);
Bool ECDSA_Verification(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Verification_InputTypedef* ECDSA_V_InputStruct);

BigdatCompare_TypeDef BigDatCompare(uint32_t *Data1,uint32_t *Data2,uint16_t	DataLen);
Bool BigDatNotZero(uint32_t *Data1,uint16_t	DataLen);
Bool ECDSA_Verification_HalfSoft(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Verification_InputTypedef* ECDSA_V_InputStruct);

void SHA256_Init(SHA256_TempTypedef* SHA256_Temp);
void SHA256_Process(SHA256_TempTypedef* SHA256_Temp,SHA256_InputTypedef* SHA256_InputStruct,uint8_t *pHashAddr);
void SHA256_Compress(SHA256_TempTypedef* SHA256_Temp);
void SHA256(SHA256_InputTypedef* SHA256_InputStruct,uint8_t *pHashAddr);
void ECDSA_TestCase_Init(void);
void SHA256_TestCase_Init(void);
uint8_t ECC_ECDSA_Example(void);





#endif                                        /* This File Only support HT6x2x ,HT501x and HT502x */

#ifdef __cplusplus
}
#endif

#endif /* __EXAMPLE_H__ */ 
