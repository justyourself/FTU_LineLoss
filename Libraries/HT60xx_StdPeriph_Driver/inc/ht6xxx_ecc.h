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
* File         : ht6xxx_ecc.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.2
* Description  : Only support HT502x and HT6x3x
**********************************************************************************************************
*/

#ifndef __HT6XXX_ECC_H__
#define __HT6XXX_ECC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ht6xxx.h"

#if defined  HT502x  ||  defined  HT6x3x             /* This File Only support HT502x */


/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/

/*
* @brief  大数模运算模式定义
*/
typedef enum
{
    ModAdd_Mode = ECC_ECCCON_OP_SEL_MA,       /*!< 模加运算                     */
    ModSub_Mode = ECC_ECCCON_OP_SEL_MS,       /*!< 模减运算                     */
    ModMul_Mode = ECC_ECCCON_OP_SEL_MM,       /*!< 模乘运算                     */
    ModDiv_Mode = ECC_ECCCON_OP_SEL_MD,       /*!< 模除运算                     */
    ModInv_Mode = ECC_ECCCON_OP_SEL_MI,       /*!< 模逆运算                     */
}ECC_ModMode_TypeDef;                         /*!<end of group ECC_ModMode_TypeDef  */

/*
* @brief  ECC点运算模式定义
*/
typedef enum
{
    EccAdd_Mode = ECC_ECCCON_OP_SEL_ECA,       /*!< ECC点加运算                   */
    EccDou_Mode = ECC_ECCCON_OP_SEL_ECD,       /*!< ECC倍加运算                   */
    EccMul_Mode = ECC_ECCCON_OP_SEL_ECSM,      /*!< ECC点乘运算                   */
}ECC_PointMode_TypeDef;                        /*!<end of group ECC_PointMode_TypeDef  */

/*
* @brief  ECC中断标志定义
*/
typedef enum
{
    ECC_IF = ECC_ECCSTA_ECCFLG,                     /*!< ECC运算完成中断标志              */
    ECC_FLG_BUSY = ECC_ECCSTA_BUSY,                 /*!< ECC运算busy标志                  */
    ECC_FLG_ECDSA_V = ECC_ECCSTA_ECDSA_V,           /*!< ECDSA认证标志                    */
    ECC_FLG_ECDSA_S = ECC_ECCSTA_ECDSA_S,           /*!< ECDSA签名重试标志                */
    ECC_FLG= ECC_ECCSTA_PKV,                        /*!< ECDSA公钥验证标志                */
}ECC_ITFlagTypeDef;

/*
* @brief  Modular operation运算 输入变量结构定义
*/
typedef struct
{
      ECC_ModMode_TypeDef ModMode;        //模运算模式
      uint32_t *pnAddr;                   //大数模n
      uint32_t *pPxAddr;                  //第1个标量PX
      uint32_t *pPyAddr;                  //第2个标量PY
}ECC_ModOperate_InputTypedef;             /*!< end of group ECC_ModOperate_InputTypedef*/

/*
* @brief  ECC PointOperate Initial变量结构定义
*/
typedef struct
{
      uint32_t *pECCpAddr;                //素域Fp的阶p
      uint32_t *pECCaAddr;                //ECC曲线参数a
}ECC_PointOperate_InitTypedef;            /*!< end of group ECC_PointOperate_InitTypedef*/
/*
* @brief  ECC Point operation运算 输入变量结构定义
*/
typedef struct
{
      ECC_PointMode_TypeDef PointMode;    //ECC点运算模式
      uint32_t *pECCPxAddr;               //ECC曲线第1点x坐标
      uint32_t *pECCPyAddr;               //ECC曲线第1点y坐标
      uint32_t *pECCSxAddr;               //ECC曲线第2点x坐标
      uint32_t *pECCSyAddr;               //ECC曲线第2点y坐标
      uint32_t *pKscalAddr;               //ECC点乘标量k
}ECC_PointOperate_InputTypedef;           /*!< end of group ECC_PointOperate_InputTypedef*/

/*
* @brief  ECC Point operation运算 输出变量结构定义
*/
typedef struct
{
      uint32_t *pRxAddr;                  //ECC点运算结果x坐标
      uint32_t *pRyAddr;                  //ECC点运算结果y坐标
}ECC_PointOperate_OutputTypedef;          /*!< end of group ECC_ModOperate_InputTypedef*/

/*
* @brief  ECDSA Initial变量结构定义
*/
typedef struct
{
      uint32_t *pECCpAddr;                //素域Fp的阶p
      uint32_t *pECCaAddr;                //ECC曲线参数a
      uint32_t *pECCGxAddr;               //ECC曲线基点x坐标
      uint32_t *pECCGyAddr;               //ECC曲线基点y坐标
      uint32_t *pECCnAddr;                //ECC曲线基点的阶n
}ECC_ECDSA_InitTypedef;                   /*!< end of group ECC_ECDSA_InitTypedef*/

/*
* @brief  ECDSA Signature operation运算 输入变量结构定义
*/
typedef struct
{
      uint32_t *pdAddr;                   //私钥d
      uint32_t *pkAddr;                   //随机大整数k
      uint32_t *pMAddr;                   //HASH后的消息摘要M
}ECC_ECDSA_Signature_InputTypedef;        /*!< end of group ECC_ECDSA_Signature_InputTypedef*/

/*
* @brief  ECDSA Verification operation运算 输入变量结构定义
*/
typedef struct
{
      uint32_t *pECCDxAddr;               //ECC曲线公钥x坐标
      uint32_t *pECCDyAddr;               //ECC曲线公钥y坐标
      uint32_t *pMAddr;                   //HASH后的消息摘要M
      uint32_t *prAddr;                   //签名结果r参数
      uint32_t *psAddr;                   //签名结果s参数
}ECC_ECDSA_Verification_InputTypedef;     /*!< end of group ECC_ECDSA_Verification_InputTypedef*/

/*
* @brief  ECC Public Key Validation公钥验证 运算 输入变量结构定义
*/
typedef struct
{
      uint32_t *pECCpAddr;                //素域Fp的阶p
      uint32_t *pECCaAddr;                //ECC曲线参数a
      uint32_t *pECCbAddr;                //ECC曲线参数b
      uint32_t *pECCPxAddr;               //ECC曲线点x坐标
      uint32_t *pECCPyAddr;               //ECC曲线点y坐标
}ECC_PKV_InputTypedef;                    /*!< end of group ECC_PKV_InputTypedef*/



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
void HT_ECC_PointOperate_Init(ECC_PointOperate_InitTypedef* ECC_PointOperate_InitStruct);
void HT_ECC_ECDSA_Init(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct);
void HT_ECC_ECDSA_S(ECC_ECDSA_Signature_InputTypedef* ECC_ECDSA_S_InputStruct);
void HT_ECC_ECDSA_V(ECC_ECDSA_Verification_InputTypedef* ECC_ECDSA_V_InputStruct);
void HT_ECC_PointOperate(ECC_PointOperate_InputTypedef* ECC_PointOperate_InputStruct);
void HT_ECC_PublickeyValidate(ECC_PKV_InputTypedef* ECC_PKV_InputStruct);
void HT_ECC_ModOperate(ECC_ModOperate_InputTypedef* ECC_ModOperate_InputStruct);
void HT_ECC_PointOperate_Read(ECC_PointOperate_OutputTypedef* ECC_PointOperate_OutputStruct);
void HT_ECC_ModOperate_Read(uint32_t *pModOperateOutAddr);
void HT_ECC_ITConfig(FunctionalState NewState);
ITStatus HT_ECC_ITFlagStatusGet(ECC_ITFlagTypeDef ITFlag);
void HT_ECC_ClearITPendingBit(ECC_ITFlagTypeDef ITFlag);
ITStatus HT_ECC_Busy_StatusGet(void);


#endif                                        /* This File Only support HT6x2x ，HT501xand HT502x */

#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_ECC_H__ */
