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
* File         : ht6xxx_ecc.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT502x and HT6x3x
*********************************************************************************************************
*/

#define  __HT6XXX_ECC_C

#include "ht6xxx_ecc.h"

#if defined  HT502x  ||  defined  HT6x3x             /* This File Only support HT502x */

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
*                              ECDSA ECC参数初始化
*
* 函数说明: ECDSA ECC参数初始化
*
* 入口参数: ECC_ECDSA_InitStruct    ECDSA ECC参数初始化 变量结构体指针，主要包含5个参数:
*                           1) pECCpAddr        uint32_t *型变量，其指向对象存储素域的阶p : pECCpAddr[0]为低位
*                           2) pECCaAddr        uint32_t *型变量，其指向对象存储素域上的曲线参数A : pECCaAddr[0]为低位
*                           3) pECCnAddr        uint32_t *型变量，其指向对象存储基点的阶n : pECCnAddr[0]为低位
*                           4) pECCGxAddr       uint32_t *型变量，其指向对象存储基点x坐标 : pECCGxAddr[0]为低位
*                           5) pECCGyAddr       uint32_t *型变量，其指向对象存储基点y坐标 : pECCGyAddr[0]为低位
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_ECC_ECDSA_Init(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct)
{
    uint8_t i;

    for(i=0;i<8;i++)
    {
        HT_ECC->PREG = *ECC_ECDSA_InitStruct->pECCpAddr++;      //加载ECC_p
        HT_ECC->AREG = *ECC_ECDSA_InitStruct->pECCaAddr++;      //加载ECC_a
        HT_ECC->PXREG = *ECC_ECDSA_InitStruct->pECCGxAddr++;    //加载ECC_Gx
        HT_ECC->PYREG = *ECC_ECDSA_InitStruct->pECCGyAddr++;    //加载ECC_Gy
        HT_ECC->SXREG = *ECC_ECDSA_InitStruct->pECCnAddr++;     //加载ECC_n
    }
}

/*
*********************************************************************************************************
*                              ECC PointOperate 参数初始化
*
* 函数说明: ECC PointOperate参数初始化
*
* 入口参数: ECC_PointOperate_InitStruct    PointOperate ECC参数初始化 变量结构体指针，主要包含2个参数:
*                           1) pECCpAddr        uint32_t *型变量，其指向对象存储素域的阶p : pECCpAddr[0]为低位
*                           2) pECCaAddr        uint32_t *型变量，其指向对象存储素域上的曲线参数a : pECCaAddr[0]为低位
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_ECC_PointOperate_Init(ECC_PointOperate_InitTypedef* ECC_PointOperate_InitStruct)
{
    uint8_t i;

    for(i=0;i<8;i++)
    {
        HT_ECC->PREG = *ECC_PointOperate_InitStruct->pECCpAddr++;     //加载ECC_p
        HT_ECC->AREG = *ECC_PointOperate_InitStruct->pECCaAddr++;     //加载ECC_a
    }
}

/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Signature operation
*
* 函数说明: 椭圆曲线数字签名算法，ECDSA-S
*
* 入口参数: ECC_ECDSA_S_InputStruct    ECDSA-S运算输入变量结构体指针，主要包含3个参数:
*                           1) pdAddr           uint32_t *型变量，其指向对象存储私钥d : pdAddr为低位
*                           2) pkAddr           uint32_t *型变量，其指向对象存储随机大整数k: pkAddr[0]为低位
*                           3) pMAddr           uint32_t *型变量，其指向对象存储HASH后的消息摘要m: pMAddr[0]为低位
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_ECC_ECDSA_S(ECC_ECDSA_Signature_InputTypedef* ECC_ECDSA_S_InputStruct)
{
    uint8_t i;
    uint32_t tempreg = 0;

    tempreg = HT_ECC->ECCCON;
    tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_ECCCON_OP_SEL_ECDSA_S);
                                                      /*!< 配置运算模式为：ECDSA-S        */
    for(i=0;i<8;i++)
    {
        HT_ECC->KEYREG = *ECC_ECDSA_S_InputStruct->pdAddr++;      //加载 d
        HT_ECC->SYREG = *ECC_ECDSA_S_InputStruct->pkAddr++;       //加载 k
        HT_ECC->MREG = *ECC_ECDSA_S_InputStruct->pMAddr++;        //加载 M
    }
    HT_ECC->ECCSTA = 0x0;                             /*!< 清零标志                       */
    HT_ECC->ECCCON = tempreg;
    HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;              /*!< 启动运算                       */
}

/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Verification operation
*
* 函数说明: 椭圆曲线数字签名算法，ECDSA-V
*
* 入口参数: ECC_ECDSA_V_InputStruct    ECDSA-V运算输入变量结构体指针，主要包含5个参数:
*                           1) pECCDxAddr       uint32_t *型变量，其指向对象存储公钥x坐标 : pECCDxAddr[0]为低位
*                           2) pECCDyAddr       uint32_t *型变量，其指向对象存储公钥y坐标 : pECCDyAddr[0]为低位
*                           3) pMAddr           uint32_t *型变量，其指向对象存储HASH后的消息摘要m: pMAddr[0]为低位
*                           4) prAddr;          uint32_t *型变量，其指向对象存储签名结果r参数:prAddr[0]为低位
*                           5) psAddr;          uint32_t *型变量，其指向对象存储签名结果s参数:psAddr[0]为低位
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_ECC_ECDSA_V(ECC_ECDSA_Verification_InputTypedef* ECC_ECDSA_V_InputStruct)
{
    uint8_t i;
    uint32_t tempreg = 0;

    tempreg = HT_ECC->ECCCON;
    tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_ECCCON_OP_SEL_ECDSA_V);
                                                      /*!< 配置运算模式为：ECDSA-v        */
    for(i=0;i<8;i++)
    {
        HT_ECC->KEYREG = *ECC_ECDSA_V_InputStruct->pECCDxAddr++;  //加载ECC_Dx
        HT_ECC->SYREG = *ECC_ECDSA_V_InputStruct->pECCDyAddr++;   //加载ECC_Dy
        HT_ECC->MREG = *ECC_ECDSA_V_InputStruct->pMAddr++;        //加载 M
        HT_ECC->RXREG = *ECC_ECDSA_V_InputStruct->prAddr++;       //加载 r
        HT_ECC->RYREG = *ECC_ECDSA_V_InputStruct->psAddr++;       //加载 s
    }
    HT_ECC->ECCSTA = 0x0;                             /*!< 清零标志                       */
    HT_ECC->ECCCON = tempreg;
    HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;              /*!< 启动运算                       */
}
/*
*********************************************************************************************************
*                              ECC Point operation运算
*
* 函数说明: 椭圆曲线点运算
*
* 入口参数: ECC_PointOperate_InputStruct    椭圆曲线点运算输入变量结构体指针，主要包含10个参数:
*                           1) PointMode        ECC_PointMode_TypeDef变量，ECC点运算模式
*                               EccAdd_Mode ：P(X1,Y1) + S(X2,Y2) = R(X3,Y3)
*                               EccDou_Mode ：2 *P(X1,Y1) = R(X3,Y3)
*                               EccMul_Mode ：k*P(X1,Y1) = R(X3,Y3)
*                           2) pECCPxAddr       uint32_t *型变量，ECC曲线第1点x坐标: pECCPxAddr[0]为低位
*                           3) pECCPyAddr       uint32_t *型变量，ECC曲线第1点y坐标: pECCPyAddr[0]为低位
*                           4) pECCSxAddr       uint32_t *型变量，ECC曲线第2点x坐标: pECCSxAddr[0]为低位
*                           5) pECCSyAddr       uint32_t *型变量，ECC曲线第2点y坐标 : pECCSyAddr[0]为低位
*                           6) pKscalAddr       uint32_t *型变量，ECC点乘标量k: pKscalAddr[0]为低位
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_ECC_PointOperate(ECC_PointOperate_InputTypedef* ECC_PointOperate_InputStruct)
{
    uint8_t i;
    uint32_t tempreg = 0;

    tempreg = HT_ECC->ECCCON;
    tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_PointOperate_InputStruct->PointMode);
                                                      /*!< 配置运算模式为：ECC 点加       */
    switch(ECC_PointOperate_InputStruct->PointMode)
    {
        case EccAdd_Mode:
          for(i=0;i<8;i++)
          {
              HT_ECC->PXREG = *ECC_PointOperate_InputStruct->pECCPxAddr++;    //加载ECC_Px
              HT_ECC->PYREG = *ECC_PointOperate_InputStruct->pECCPyAddr++;    //加载ECC_Py
              HT_ECC->SXREG = *ECC_PointOperate_InputStruct->pECCSxAddr++;    //加载ECC_Sx
              HT_ECC->SYREG = *ECC_PointOperate_InputStruct->pECCSyAddr++;    //加载ECC_Sy
          }
          break;
        case EccDou_Mode:
          for(i=0;i<8;i++)
          {
              HT_ECC->PXREG = *ECC_PointOperate_InputStruct->pECCPxAddr++;    //加载ECC_Px
              HT_ECC->PYREG = *ECC_PointOperate_InputStruct->pECCPyAddr++;    //加载ECC_Py
          }
          break;
        case EccMul_Mode:
          for(i=0;i<8;i++)
          {
              HT_ECC->PXREG = *ECC_PointOperate_InputStruct->pECCPxAddr++;    //加载ECC_Px
              HT_ECC->PYREG = *ECC_PointOperate_InputStruct->pECCPyAddr++;    //加载ECC_Py
              HT_ECC->KEYREG = *ECC_PointOperate_InputStruct->pKscalAddr++;   //加载 k
          }
          break;
        default:
          break;
    }
    HT_ECC->ECCSTA = 0x0;                             /*!< 清零标志                       */
    HT_ECC->ECCCON = tempreg;
    HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;              /*!< 启动运算                       */
}

/*
*********************************************************************************************************
*                              ECC Public Key Validation operation
*
* 函数说明: 椭圆曲线公钥验证，PKV
*
* 入口参数: ECC_PKV_InputStruct    PKV运算输入变量结构体指针，主要包含10个参数:
*                           1) pECCpAddr        uint32_t *型变量，其指向对象存储素域的阶p : pECCpAddr[0]为低位
*                           2) pECCaAddr        uint32_t *型变量，其指向对象存储曲线参数a : pECCaAddr[0]为低位
*                           3) pECCbAddr        uint32_t *型变量，其指向对象存储曲线参数b : pECCbAddr[0]为低位
*                           4) pECCPxAddr       uint32_t *型变量，其指向对象存储基点x坐标 : pECCPxAddr[0]为低位
*                           5) pECCPyAddr       uint32_t *型变量，其指向对象存储基点y坐标 : pECCPyAddr[0]为低位
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_ECC_PublickeyValidate(ECC_PKV_InputTypedef* ECC_PKV_InputStruct)
{
    uint8_t i;
    uint32_t tempreg = 0;

    tempreg = HT_ECC->ECCCON;
    tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_ECCCON_OP_SEL_PKV);
                                                      /*!< 配置运算模式为：PKV        */
    for(i=0;i<8;i++)
    {
        HT_ECC->PREG = *ECC_PKV_InputStruct->pECCpAddr++;     //加载ECC_p
        HT_ECC->AREG = *ECC_PKV_InputStruct->pECCaAddr++;     //加载ECC_a
        HT_ECC->PXREG = *ECC_PKV_InputStruct->pECCPxAddr++;   //加载ECC_Px
        HT_ECC->PYREG = *ECC_PKV_InputStruct->pECCPyAddr++;   //加载ECC_Py
        HT_ECC->SYREG = *ECC_PKV_InputStruct->pECCbAddr++;    //加载ECC_b
    }
    HT_ECC->ECCSTA = 0x0;                             /*!< 清零标志                   */
    HT_ECC->ECCCON = tempreg;
    HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;              /*!< 启动运算                   */
}


/*
*********************************************************************************************************
*                              Modular operation运算
*
* 函数说明: 大数模运算
*
* 入口参数: ECC_ModOperate_InputStruct    大数模运算输入变量结构体指针，主要包含10个参数:
*                           1) ModMode        ECC_ModMode_TypeDef变量，模运算模式
*                               ModAdd_Mode ：PX + PY ( mod n ) = RX
*                               ModSub_Mode ：PX - PY ( mod n ) = RX
*                               ModMul_Mode ：PX * PY ( mod n ) = RX
*                               ModDiv_Mode ：PY / PX ( mod n ) = RX
*                               ModInv_Mode ：PX-1 ( mod n ) = RX
*                           2) pnAddr       uint32_t *型变量，大数模n : pnAddr[0]为低位
*                           3) pPxAddr        uint32_t *型变量，第1个标量PX: pPxAddr[0]为低位
*                           4) pPyAddr        uint32_t *型变量，第2个标量PX: pPyAddr[0]为低位
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_ECC_ModOperate(ECC_ModOperate_InputTypedef* ECC_ModOperate_InputStruct)
{
    uint8_t i;
    uint32_t tempreg = 0;

    tempreg = HT_ECC->ECCCON;
    tempreg = ((tempreg & (~ECC_ECCCON_OP_SEL)) | ECC_ModOperate_InputStruct->ModMode);
                                                      /*!< 配置运算模式为：ECC 点加       */
    switch(ECC_ModOperate_InputStruct->ModMode)
    {
        case ModAdd_Mode:
        case ModSub_Mode:
        case ModMul_Mode:
        case ModDiv_Mode:
          for(i=0;i<8;i++)
          {
              HT_ECC->PREG = *ECC_ModOperate_InputStruct->pnAddr++;     //加载n
              HT_ECC->PXREG = *ECC_ModOperate_InputStruct->pPxAddr++;   //加载Px
              HT_ECC->PYREG = *ECC_ModOperate_InputStruct->pPyAddr++;   //加载Py
          }
          break;
        case ModInv_Mode:
          for(i=0;i<8;i++)
          {
              HT_ECC->PREG = *ECC_ModOperate_InputStruct->pnAddr++;     //加载n
              HT_ECC->PXREG = *ECC_ModOperate_InputStruct->pPxAddr++;   //加载Px
          }
          break;
        default:
          break;
    }
    HT_ECC->ECCSTA = 0x0;                             /*!< 清零标志                       */
    HT_ECC->ECCCON = tempreg;
    HT_ECC->ECCCON |= ECC_ECCCON_OP_STR;              /*!< 启动运算                       */
}



/*
*********************************************************************************************************
*                                      ECC运算结果数据读取 MODULE
*
* 函数说明: ECC运算结果数据读取
*
* 入口参数: ECC_PointOperate_OutputStruct   ECC运算结果数据结构体指针，主要包含10个参数:
*                   1) pRxAddr        uint32_t *型变量，ECC点运算结果x坐标/ECDSA签名结果r: pRxAddr[0]为低位
*                   2) pRyAddr        uint32_t *型变量，ECC点运算结果y坐标/ECDSA签名结果s: pRyAddr[0]为低位
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_ECC_PointOperate_Read(ECC_PointOperate_OutputTypedef* ECC_PointOperate_OutputStruct)
{
    uint8_t i;

    ECC_PointOperate_OutputStruct->pRxAddr += 7;
    ECC_PointOperate_OutputStruct->pRyAddr += 7;
    for(i=0;i<8;i++)
    {
        *ECC_PointOperate_OutputStruct->pRxAddr-- = HT_ECC->RXREG;
        *ECC_PointOperate_OutputStruct->pRyAddr-- = HT_ECC->RYREG;
    }
}

/*
*********************************************************************************************************
*                                      大数模运算结果数据读取 MODULE
*
* 函数说明: 大数模运算结果数据读取
*
* 入口参数: pModOperateOutAddr   uint32_t *型变量,大数模运算结果数据指针
*
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/

void HT_ECC_ModOperate_Read(uint32_t *pModOperateOutAddr)
{
    uint8_t i;

    pModOperateOutAddr += 7;
    for(i=0;i<8;i++)
    {
        *pModOperateOutAddr-- = HT_ECC->RXREG;
    }
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE ECC INTERRUPT
*
* 函数说明: 使能或者关闭ECC中断
*
* 入口参数:
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_ECC_ITConfig(FunctionalState NewState)
{
    /*  assert_param  */

    if (NewState != DISABLE)
    {
        HT_ECC->ECCCON |= (uint32_t)ECC_ECCCON_ECCIE;            /*!< 使能ECC中断           */
    }
    else
    {
        HT_ECC->ECCCON &= ~(uint32_t)ECC_ECCCON_ECCIE;           /*!< 关闭ECC中断           */
    }
}

/*
*********************************************************************************************************
*                            GET SPECIFIED ECC INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应ECC中断标志状态
*
* 入口参数: ITFlag     想要检查的ECC中断标志，可以为以下参数:
*                        @arg ECC_IF
*                        @arg ECC_FLG_BUSY
*                        @arg ECC_FLG_ECDSA_V
*                        @arg ECC_FLG_ECDSA_S
*                        @arg ECC_FLG_PKV
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
*
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_ECC_ITFlagStatusGet(ECC_ITFlagTypeDef ITFlag)
{
    /*  assert_param  */

    if (HT_ECC->ECCSTA & ITFlag)
    {
        return SET;                        /*!< ECC Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< ECC Interrupt Flag is reset */
    }
}

/*
*********************************************************************************************************
*                                   CLEAR ECC INTERRUPT FLAG
*
* 函数说明: 清除ECC中断标志
*
* 入口参数: ITFlag     想要清除的某个ECC中断标志，可以为以下参数或其组合:
*                        @arg ECC_IF
*                        @arg ECC_FLG_BUSY
*                        @arg ECC_FLG_ECDSA_V
*                        @arg ECC_FLG_ECDSA_S
*                        @arg ECC_FLG_PKV
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_ECC_ClearITPendingBit(ECC_ITFlagTypeDef ITFlag)
{
    /*  assert_param  */


    HT_ECC->ECCSTA  &= ~((uint32_t)ITFlag);                  /*!< Clear ECC Interrupt Flag       */

}

/*
*********************************************************************************************************
*                            GET ECC BUSY STATUS
*
* 函数说明: 获取相应ECC中BUSY状态
*
* 入口参数: 无
*
* 返回参数: ITStatus    = SET：  busy忙碌
*                       = RESET：busy空闲
*
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_ECC_Busy_StatusGet(void)
{
    /*  assert_param  */
    return (ITStatus)(HT_ECC->ECCSTA & ECC_FLG_BUSY);               /*!< 返回ECC busy status          */
}


#endif                                        /* This File Only support HT6x2x and HT501x */

