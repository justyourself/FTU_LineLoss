/*
*********************************************************************************************************
*                                              HT60XX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT60xx
* File         : ecc_example.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __ECC_EXAMPLE_C
//#undef   __ECC_EXAMPLE_C

#ifdef   __ECC_EXAMPLE_C

#include "ecc_example.h"
#include "ht6xxx_lib.h"
#include "string.h"

#if defined  HT502x             /* This File Only support HT502x */ 

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
/* the K array  for SHA-256*/
const uint32_t SHA256_K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL,
		0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL,
		0x243185beUL, 0x550c7dc3UL, 0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL,
		0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
		0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL, 0x983e5152UL,
		0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
		0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL,
		0x53380d13UL, 0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
		0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL,
		0xd6990624UL, 0xf40e3585UL, 0x106aa070UL, 0x19a4c116UL, 0x1e376c08UL,
		0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL,
		0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
		0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Signature operation
*
* 函数说明: 椭圆曲线数字签名算法，ECDSA-S
*
* 入口参数: ECC_ECDSA_InitStruct    ECC曲线参数初始化变量结构体指针，主要包含5个参数: 
*														1) pECCpAddr				uint32_t *型变量，其指向对象存储素域的阶p : pECCpAddr[0]为低位
*														2) pECCaAddr				uint32_t *型变量，其指向对象存储素域上的曲线参数A : pECCaAddr[0]为低位
*														3) pECCnAddr				uint32_t *型变量，其指向对象存储基点的阶n : pECCnAddr[0]为低位
*														4) pECCGxAddr				uint32_t *型变量，其指向对象存储基点x坐标 : pECCGxAddr[0]为低位
*														5) pECCGyAddr  			uint32_t *型变量，其指向对象存储基点y坐标 : pECCGyAddr[0]为低位
*						ECDSA_S_InputStruct    ECDSA-S运算输入变量结构体指针，主要包含3个参数: 
*								           	6) pdAddr						uint32_t *型变量，其指向对象存储私钥d : pdAddr为低位            
*														7) pkAddr  					uint32_t *型变量，其指向对象存储随机大整数k: pkAddr[0]为低位
*									   				8) pMAddr						uint32_t *型变量，其指向对象存储HASH后的消息摘要m: pMAddr[0]为低位
*
*						ECDSA_S_OutputStruct   ECC运算结果数据结构体指针，主要包含10个参数: 
*													1) pRxAddr				uint32_t *型变量，ECC点运算结果x坐标/ECDSA签名结果r: pRxAddr[0]为低位
*													2) pRyAddr				uint32_t *型变量，ECC点运算结果y坐标/ECDSA签名结果s: pRyAddr[0]为低位
* 返回参数: Bool   = TRUE  ，签名完成
*									 = FALSE ，签名fail，满足重试条件
* 
* 特殊说明: 
*********************************************************************************************************
*/
Bool ECDSA_Signature(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Signature_InputTypedef* ECDSA_S_InputStruct,ECC_PointOperate_OutputTypedef* ECDSA_S_OutputStruct)
{
				
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,ENABLE);						/*打开ECC模块时钟									*/
	
		HT_ECC_ECDSA_Init(ECC_ECDSA_InitStruct);										/*ECC参数初始化										*/
	
		HT_ECC_ECDSA_S(ECDSA_S_InputStruct);												/*启动签名运算										*/
	
		while(!HT_ECC_ITFlagStatusGet(ECC_IF));											/*运算完成判断										*/
		HT_ECC_ClearITPendingBit(ECC_IF);														/*清零标志												*/
	
		if(RESET == HT_ECC_ITFlagStatusGet(ECC_FLG_ECDSA_S))				/*判断签名重试标志								*/
		{
				HT_ECC_PointOperate_Read(ECDSA_S_OutputStruct);					/*签名完成，输出签名标签					*/
				HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,DISABLE);			/*关闭ECC模块时钟									*/
				return(TRUE);
		}
		else
		{
				HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,DISABLE);			/*关闭ECC模块时钟									*/
				return(FALSE);																					/*签名失败，满足重试条件					*/
		}
}

/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Verification operation
*
* 函数说明: 椭圆曲线数字签名算法，ECDSA-V
*
* 入口参数: ECC_ECDSA_InitStruct    ECC曲线参数初始化变量结构体指针，主要包含5个参数: 
*														1) pECCpAddr				uint32_t *型变量，其指向对象存储素域的阶p : pECCpAddr[0]为低位
*														2) pECCaAddr				uint32_t *型变量，其指向对象存储素域上的曲线参数A : pECCaAddr[0]为低位
*														3) pECCnAddr				uint32_t *型变量，其指向对象存储基点的阶n : pECCnAddr[0]为低位
*														4) pECCGxAddr				uint32_t *型变量，其指向对象存储基点x坐标 : pECCGxAddr[0]为低位
*														5) pECCGyAddr  			uint32_t *型变量，其指向对象存储基点y坐标 : pECCGyAddr[0]为低位
* 					ECC_ECDSA_V_InputStruct    ECDSA-V运算输入变量结构体指针，主要包含10个参数: 
*								           	6) pECCDxAddr				uint32_t *型变量，其指向对象存储公钥x坐标 : pECCDxAddr[0]为低位            
*														7) pECCDyAddr  			uint32_t *型变量，其指向对象存储公钥y坐标 : pECCDyAddr[0]为低位
*									   				8) pMAddr						uint32_t *型变量，其指向对象存储HASH后的消息摘要m: pMAddr[0]为低位
*														9) prAddr;					uint32_t *型变量，其指向对象存储签名结果r参数:prAddr[0]为低位
*														10)psAddr;					uint32_t *型变量，其指向对象存储签名结果s参数:psAddr[0]为低位
*
* 返回参数: Bool   = TRUE  ，认证通过
*									 = FALSE ，认证fail
* 
* 特殊说明: 
*********************************************************************************************************
*/
Bool ECDSA_Verification(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Verification_InputTypedef* ECDSA_V_InputStruct)
{
		
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,ENABLE);						/*打开ECC模块时钟									*/
	
		HT_ECC_ECDSA_Init(ECC_ECDSA_InitStruct);										/*ECC参数初始化										*/
	
		HT_ECC_ECDSA_V(ECDSA_V_InputStruct);												/*启动认证运算										*/
	
		while(!HT_ECC_ITFlagStatusGet(ECC_IF));											/*运算完成判断										*/
		HT_ECC_ClearITPendingBit(ECC_IF);														/*清零标志												*/
	
		if(SET == HT_ECC_ITFlagStatusGet(ECC_FLG_ECDSA_V))					/*判断认证标志										*/
		{
				HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,DISABLE);			/*关闭ECC模块时钟									*/
				return(TRUE);																						/*认证通过												*/
		}
		else
		{
				HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,DISABLE);			/*关闭ECC模块时钟									*/
				return(FALSE);																					/*认证失败												*/
		}
}
/*
*********************************************************************************************************
*                              EC Digital Signature Algorithm (ECDSA) Verification operation
*
* 函数说明: 椭圆曲线数字签名算法，ECDSA-V,软件算法for HT502XA
*
* 入口参数: ECC_ECDSA_InitStruct    ECC曲线参数初始化变量结构体指针，主要包含5个参数: 
*														1) pECCpAddr				uint32_t *型变量，其指向对象存储素域的阶p : pECCpAddr[0]为低位
*														2) pECCaAddr				uint32_t *型变量，其指向对象存储素域上的曲线参数A : pECCaAddr[0]为低位
*														3) pECCnAddr				uint32_t *型变量，其指向对象存储基点的阶n : pECCnAddr[0]为低位
*														4) pECCGxAddr				uint32_t *型变量，其指向对象存储基点x坐标 : pECCGxAddr[0]为低位
*														5) pECCGyAddr  			uint32_t *型变量，其指向对象存储基点y坐标 : pECCGyAddr[0]为低位
* 					ECC_ECDSA_V_InputStruct    ECDSA-V运算输入变量结构体指针，主要包含10个参数: 
*								           	6) pECCDxAddr				uint32_t *型变量，其指向对象存储公钥x坐标 : pECCDxAddr[0]为低位            
*														7) pECCDyAddr  			uint32_t *型变量，其指向对象存储公钥y坐标 : pECCDyAddr[0]为低位
*									   				8) pMAddr						uint32_t *型变量，其指向对象存储HASH后的消息摘要m: pMAddr[0]为低位
*														9) prAddr;					uint32_t *型变量，其指向对象存储签名结果r参数:prAddr[0]为低位
*														10)psAddr;					uint32_t *型变量，其指向对象存储签名结果s参数:psAddr[0]为低位
*
* 返回参数: Bool   = TRUE  ，认证通过
*									 = FALSE ，认证fail
* 
* 特殊说明: 
*********************************************************************************************************
*/
Bool ECDSA_Verification_HalfSoft(ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct,ECC_ECDSA_Verification_InputTypedef* ECDSA_V_InputStruct)
{
		uint8_t i;
		uint32_t *pTemp0,*pTemp1,*pTemp2,*pTemp3,*pTemp4,*pTemp5;
		ECC_ECDSA_InitTypedef* ECC_ECDSA_InitStruct_backup;
		ECC_ECDSA_Verification_InputTypedef* ECDSA_V_InputStruct_backup;
		ECC_ModOperate_InputTypedef ECCtemp;
		ECC_PointOperate_InitTypedef ECCPoint_Init;
		ECC_PointOperate_InputTypedef ECCPointtemp;
		Parameter256bits_TypeDef	ECDSAtemp_w,ECDSAtemp_u1,ECDSAtemp_u2,ECCmodtemp;
		ECC_PointOperate_OutputTypedef EccPointtemp_X1,EccPointtemp_X2;
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ECCEN,ENABLE);						/*打开ECC模块时钟									*/
	
		ECC_ECDSA_InitStruct_backup = ECC_ECDSA_InitStruct;					/*初始化结构体指针备份						*/
		ECDSA_V_InputStruct_backup = ECDSA_V_InputStruct;						/*输入变量结构体指针备份					*/
		pTemp0 = ECC_ECDSA_InitStruct_backup->pECCnAddr;
		pTemp1 = ECC_ECDSA_InitStruct_backup->pECCnAddr;
		pTemp2 = ECDSA_V_InputStruct_backup->prAddr;
		pTemp3 = ECDSA_V_InputStruct_backup->psAddr;
		pTemp4 = ECDSA_V_InputStruct_backup->prAddr;
		pTemp5 = ECDSA_V_InputStruct_backup->psAddr;
	
		/* 1)检验签名是否是区间[1, n-1]内的整数.若任何一个检验失败,则返回("拒绝该签名")*/
		if((GREATER == BigDatCompare(pTemp0,pTemp2,8)) && 
			 (GREATER == BigDatCompare(pTemp1,pTemp3,8)) &&
			 (TRUE == BigDatNotZero(pTemp4,8)) &&
			 (TRUE == BigDatNotZero(pTemp5,8)) )
		{
		/* 2)计算s的逆元s1,并计算w = s1 mod n.    */
				ECCtemp.ModMode = ModInv_Mode;
				ECCtemp.pnAddr = ECC_ECDSA_InitStruct_backup->pECCnAddr;
				ECCtemp.pPxAddr = ECDSA_V_InputStruct_backup->psAddr;
				HT_ECC_ModOperate(&ECCtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*运算完成判断										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*清零标志												*/
				HT_ECC_ModOperate_Read(ECDSAtemp_w.u32);
			
		/* 3)计算u1 = (M*w) mod n 和 u2 = (r*w) mod n. */	
				ECCtemp.ModMode = ModMul_Mode;
				ECCtemp.pnAddr = ECC_ECDSA_InitStruct_backup->pECCnAddr;
				ECCtemp.pPxAddr = ECDSA_V_InputStruct_backup->pMAddr;
				ECCtemp.pPyAddr = ECDSAtemp_w.u32;
				HT_ECC_ModOperate(&ECCtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*运算完成判断										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*清零标志												*/
				HT_ECC_ModOperate_Read(ECDSAtemp_u1.u32);
			
				ECCtemp.pnAddr = ECC_ECDSA_InitStruct_backup->pECCnAddr;
				ECCtemp.pPxAddr = ECDSA_V_InputStruct_backup->prAddr;
				ECCtemp.pPyAddr = ECDSAtemp_w.u32;
				HT_ECC_ModOperate(&ECCtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*运算完成判断										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*清零标志												*/
				HT_ECC_ModOperate_Read(ECDSAtemp_u2.u32);				
			
		/* 4)计算X = u1*G +u2*D.  */
				ECCPoint_Init.pECCaAddr = ECC_ECDSA_InitStruct_backup->pECCaAddr;
				ECCPoint_Init.pECCpAddr = ECC_ECDSA_InitStruct_backup->pECCpAddr;
				HT_ECC_PointOperate_Init(&ECCPoint_Init);
				
				ECCPointtemp.PointMode = EccMul_Mode;
				ECCPointtemp.pECCPxAddr = ECC_ECDSA_InitStruct_backup->pECCGxAddr;
				ECCPointtemp.pECCPyAddr = ECC_ECDSA_InitStruct_backup->pECCGyAddr;
				ECCPointtemp.pKscalAddr = ECDSAtemp_u1.u32;
				HT_ECC_PointOperate(&ECCPointtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*运算完成判断										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*清零标志												*/
				EccPointtemp_X1.pRxAddr = ECDSAtemp_w.u32;							/*输出结构体指针初始化						*/
				EccPointtemp_X1.pRyAddr = ECDSAtemp_u1.u32;
				HT_ECC_PointOperate_Read(&EccPointtemp_X1);							/*u1*G 存于ECDSAtemp_w&ECDSAtemp_u1中	*/

				ECCPointtemp.PointMode = EccMul_Mode;										/* 点乘														*/
				ECCPointtemp.pECCPxAddr = ECDSA_V_InputStruct_backup->pECCDxAddr;
				ECCPointtemp.pECCPyAddr = ECDSA_V_InputStruct_backup->pECCDyAddr;
				ECCPointtemp.pKscalAddr = ECDSAtemp_u2.u32;
				HT_ECC_PointOperate(&ECCPointtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*运算完成判断										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*清零标志												*/
				EccPointtemp_X2.pRxAddr = ECDSAtemp_u2.u32;							/*输出结构体指针初始化						*/
				EccPointtemp_X2.pRyAddr = ECCmodtemp.u32;				
				HT_ECC_PointOperate_Read(&EccPointtemp_X2);							/*u2*D 存于ECDSAtemp_u2&ECCmodtemp中	*/

				
				ECCPointtemp.PointMode = EccAdd_Mode;										/* 点加														*/
				ECCPointtemp.pECCPxAddr = ECDSAtemp_w.u32;							/*u1*G的x坐标											*/
				ECCPointtemp.pECCPyAddr = ECDSAtemp_u1.u32;							/*u1*G的y坐标											*/
				ECCPointtemp.pECCSxAddr = ECDSAtemp_u2.u32;							/*u2*D的x坐标											*/
				ECCPointtemp.pECCSyAddr = ECCmodtemp.u32;								/*u2*D的y坐标											*/
				HT_ECC_PointOperate(&ECCPointtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*运算完成判断										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*清零标志												*/
				EccPointtemp_X1.pRxAddr = ECDSAtemp_w.u32;							/*输出结构体指针初始化						*/
				EccPointtemp_X1.pRyAddr = ECDSAtemp_u1.u32;
				HT_ECC_PointOperate_Read(&EccPointtemp_X1);							/*u1*G +u2*D 存于ECDSAtemp_w&ECDSAtemp_u1中	*/
			
		/* 5)若X = 无穷大, 则返回("拒绝该签名") */	
		/* 6)将X的x1坐标转换为整数t, 计算 w = t mod n. */	
				for(i=0;i<8;i++)
				{
					ECCmodtemp.u32[i] = 0x0;
				}
				ECCtemp.ModMode = ModAdd_Mode;
				ECCtemp.pnAddr = ECC_ECDSA_InitStruct_backup->pECCnAddr;
				ECCtemp.pPxAddr = ECDSAtemp_w.u32;											/*X的x1坐标												*/
				ECCtemp.pPyAddr = ECCmodtemp.u32;
				HT_ECC_ModOperate(&ECCtemp);
				while(!HT_ECC_ITFlagStatusGet(ECC_IF));									/*运算完成判断										*/
				HT_ECC_ClearITPendingBit(ECC_IF);												/*清零标志												*/
				HT_ECC_ModOperate_Read(ECCmodtemp.u32);

		/* 7）若w= r, 则返回("接受该签名"); 否则,返回("拒绝该签名").  */
				pTemp0 = ECDSA_V_InputStruct_backup->prAddr;
				if( EQUAL ==BigDatCompare(pTemp0,ECCmodtemp.u32,8))
				{
					return(TRUE);
				}
				else
				{
					return(FALSE);
				}
		}
		else
		{
			return(FALSE);
		}
}

BigdatCompare_TypeDef BigDatCompare(uint32_t *Data1,uint32_t *Data2,uint16_t	DataLen)
{
		uint16_t i;
		uint32_t *pTemp1,*pTemp2;
	
		pTemp1=Data1;
		pTemp2=Data2;
		for(i=0; i<DataLen; i++)
		{
			if((*(pTemp1+DataLen-i-1)) > (*(pTemp2+DataLen-i-1)))
			{
				return(GREATER);
			}
			else if((*(pTemp1+DataLen-i-1)) < (*(pTemp2+DataLen-i-1)))
			{
				return(LESS);
			}
		}
		return(EQUAL);
}

Bool BigDatNotZero(uint32_t *Data1,uint16_t	DataLen)
{
		uint16_t i;
		uint32_t *pTemp1;
		
		pTemp1=Data1;
		for(i=0; i<DataLen; i++)
		{
			if((*(pTemp1+DataLen-i-1)) != 0)
			{
				return(TRUE);
			}
		}
		return(FALSE);
}
/*
*********************************************************************************************************
*                              SHA-256 算法软件实现
*
* 函数说明: HASH算法，SHA-256
*
* 入口参数: SHA256_InputTypedef    SHA256运算初始化变量结构体指针，主要包含5个参数: 
*														1) pMtestAddr				uint8_t *型变量，其指向对象待运算消息m : pMtestAddr[0]为低位
*														2) LenMtest					uint64_t 型变量，待运算的消息长度（byte）
* 					pHashAddr     uint8_t *型变量，其指向对象计算后的HASH结果: pHashAddr[0]为低位
*
* 特殊说明: 
*********************************************************************************************************
*/
void SHA256(SHA256_InputTypedef* SHA256_InputStruct,uint8_t *pHashAddr)
{
	SHA256_TempTypedef SHA256_Temp;
	
	SHA256_Init(&SHA256_Temp);
	SHA256_Process(&SHA256_Temp,SHA256_InputStruct,pHashAddr);
}

/* init the SHA256 HASH初值 */
void SHA256_Init(SHA256_TempTypedef* SHA256_Temp)
{
		SHA256_Temp->MtestLen = 0;																		//消息长度初始化（单位bit）
		SHA256_Temp->MnCurlen = 0;																		//每段消息（512-bits）实时长度（单位byte）
    SHA256_Temp->HashValue[0] = 0x6A09E667UL;											//SHA256 HASH初值Hn(0)，n=0~7
    SHA256_Temp->HashValue[1] = 0xBB67AE85UL;
    SHA256_Temp->HashValue[2] = 0x3C6EF372UL;
    SHA256_Temp->HashValue[3] = 0xA54FF53AUL;
    SHA256_Temp->HashValue[4] = 0x510E527FUL;
    SHA256_Temp->HashValue[5] = 0x9B05688CUL;
    SHA256_Temp->HashValue[6] = 0x1F83D9ABUL;
    SHA256_Temp->HashValue[7] = 0x5BE0CD19UL;
}

/* SHA256 运算		*/
/* 初始化sha-256消息结构, 若消息过长, 则压缩消息至最多64字节. */
void SHA256_Process(SHA256_TempTypedef* SHA256_Temp,SHA256_InputTypedef* SHA256_InputStruct,uint8_t *pHashAddr)
{
    uint8_t i;
	
		/* process （1~N-1） 512-bits message  处理前N-1次 迭代运算*/   
		while(SHA256_InputStruct->LenMtest--)
		{
				SHA256_Temp->MnBuf[SHA256_Temp->MnCurlen] = *SHA256_InputStruct->pMtestAddr++;					
																																					/*  copy message to temp  		*/
				SHA256_Temp->MnCurlen++;
				if(SHA256_Temp->MnCurlen == 64)
				{
					SHA256_Compress(SHA256_Temp);																		/*  Compress 512-bit message  */
					SHA256_Temp->MtestLen += 512;																		/*  消息长度累加 512 bits			*/
					SHA256_Temp->MnCurlen = 0;																			/*	分段计数长度清零					*/
				}
		}
		/* process the Nth 512-bits message  处理最后512-BIT 迭代运算 */
		SHA256_Temp->MtestLen += (SHA256_Temp->MnCurlen * 8);									/* 累加消息长度（单位bit） 	  */
		SHA256_Temp->MnBuf[SHA256_Temp->MnCurlen++] = 0x80;										/* 消息后补位append the '1' bit */
		/* if the length is currenlly above 56 bytes we append zeroshen compress. 
     * Then we can fall back to padding zeros and length ，encoding like normal.*/
		if(SHA256_Temp->MnCurlen >= 56)																				/* MnCurlen>=56,后4bytes，全部补零*/
		{
				for(; SHA256_Temp->MnCurlen < 64;)
				{
						SHA256_Temp->MnBuf[SHA256_Temp->MnCurlen++] = 0;							/* 最后4bytes，补零						*/
				}
				SHA256_Compress(SHA256_Temp);																			/*  Compress 512-bit message  */
				SHA256_Temp->MnCurlen = 0;																				/*	分段计数长度清零					*/
		}
		for(; SHA256_Temp->MnCurlen < 56;)
        SHA256_Temp->MnBuf[SHA256_Temp->MnCurlen++] = 0;  								/* pad upto 56 bytes of zeroes */
    for(i = 56; i < 64; i++)
        SHA256_Temp->MnBuf[i] = (uint8_t)((SHA256_Temp->MtestLen >> ((63 - i) * 8)) & 255);
																																					/* append length 							*/
		SHA256_Compress(SHA256_Temp);																					/*  Compress 512-bit message  */
    /* copy output */
    for(i = 0; i < 32; i++)
        *pHashAddr++ = (uint8_t)((SHA256_Temp->HashValue[(31-i) >> 2] >> ((i & 3) << 3)) & 255);
																																						
}

/*  Compress 512-bit message  */
void SHA256_Compress(SHA256_TempTypedef* SHA256_Temp)
{
		uint32_t	Sha_a, Sha_b,Sha_c,Sha_d,Sha_e,Sha_f,Sha_g,Sha_h,Sha_W[64], Sha_T1, Sha_T2;
		uint8_t	i;
	
		/* 初始化工作变量a，b,c,d,e,f,g,h	，Initialize the eight working variables	*/
		Sha_a = SHA256_Temp->HashValue[0];
		Sha_b = SHA256_Temp->HashValue[1];
		Sha_c = SHA256_Temp->HashValue[2];
		Sha_d = SHA256_Temp->HashValue[3];
		Sha_e = SHA256_Temp->HashValue[4];
		Sha_f = SHA256_Temp->HashValue[5];
		Sha_g = SHA256_Temp->HashValue[6];
		Sha_h = SHA256_Temp->HashValue[7];
		/* 准备消息调度表，Prepare the message schedule  */
    for(i=0; i<16; i++)																			    /* copy the state into 512-bits into W[i] */
        Sha_W[i] = (((uint32_t) SHA256_Temp->MnBuf[(4 * i) + 0]) << 24) |
									 (((uint32_t) SHA256_Temp->MnBuf[(4 * i) + 1]) << 16) |
									 (((uint32_t) SHA256_Temp->MnBuf[(4 * i) + 2]) << 8)  |
									 (((uint32_t) SHA256_Temp->MnBuf[(4 * i) + 3]));
    for(i=16; i<64; i++)																				/* fill W[16..63] */
        Sha_W[i] = Gamma1(Sha_W[i-2]) + Sha_W[i-7] + Gamma0(Sha_W[i-15]) + Sha_W[i-16];

    /* 64次迭代运算，Compress */
    for(i=0; i<64; i++) 
		{
        Sha_T1 = Sha_h + Sigma1(Sha_e) + Ch(Sha_e,Sha_f,Sha_g) + SHA256_K[i] + Sha_W[i];
        Sha_T2 = Sigma0(Sha_a) + Maj(Sha_a,Sha_b,Sha_c);
        Sha_h = Sha_g;
        Sha_g = Sha_f;
        Sha_f = Sha_e;
        Sha_e = Sha_d + Sha_T1;
        Sha_d = Sha_c;
				Sha_c = Sha_b;
        Sha_b = Sha_a;
        Sha_a = Sha_T1 + Sha_T2;
    }

    /* 计算hash值，feedback */
    SHA256_Temp->HashValue[0] += Sha_a;
		SHA256_Temp->HashValue[1] += Sha_b;
		SHA256_Temp->HashValue[2] += Sha_c;
		SHA256_Temp->HashValue[3] += Sha_d;
		SHA256_Temp->HashValue[4] += Sha_e;
		SHA256_Temp->HashValue[5] += Sha_f;
		SHA256_Temp->HashValue[6] += Sha_g;
		SHA256_Temp->HashValue[7] += Sha_h;
}

/*
*********************************************************************************************************
*                                ECDSA EXAMPLES FOR HT502x
*
* 函数说明: 利用库函数实现ECDSA签名与认证
*
* 入口参数: 无                            
*
* 返回参数: uint8_t
*						   			bit0：	0,signiture fail 
*														1,signiture pass 
*							 			bit1：	0,verification fail
*														1,verification pass
*							 			bit2：	0,halfsoftware verification fail
*														1,halfsoftware verification pass
*										bit3：	0,SHA256 fail
*														1,SHA256 pass
* 
* 特殊说明: 此函数默认不参与工程编译
*********************************************************************************************************
*/

/*变量定义*/	
Parameter256bits_TypeDef 	ECC_p,ECC_a,ECC_b,ECC_Gx,ECC_Gy,ECC_n;		//ECC曲线(Ep(a,b))参数，素域的阶p，曲线系数a,b，基点G(x,y)，基点的阶n
Parameter256bits_TypeDef	ECC_Dx,ECC_Dy,ECC_M,ECC_s,ECC_r;		//公钥D(x,y)，消息摘要M，签名结果(r,s)
Parameter256bits_TypeDef	ECC_d,ECC_k;												//私钥d，随机大整数k
Parameter1024bits_TypeDef	SHA_m;															//消息
Parameter256bits_TypeDef	SHA_M;															//消息摘要SHA_M=sha256(SHA_m)
uint64_t	LenSHA_m;																						//消息长度（byte）

uint8_t ECC_ECDSA_Example(void)
{
		CMU_InitTypeDef  CMU_InitStructure;
		ECC_ECDSA_InitTypedef ECC_ECDSA_InitStruct;
		ECC_ECDSA_Signature_InputTypedef ECDSA_S_InputStruct;
		ECC_PointOperate_OutputTypedef ECDSA_S_OutputStruct;
		ECC_ECDSA_Verification_InputTypedef ECDSA_V_InputStruct;
		Parameter256bits_TypeDef ECC_tmp0,ECC_tmp1; 
		SHA256_InputTypedef SHA256_InputStruct;
		uint8_t i,ReturnTemp;

/*ECDSA输入参数初始化*/
		ReturnTemp = 0x0;
		ECDSA_TestCase_Init();
/*设置系统时钟*/    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv16;				/*!<	HT502XA 此分频必须大于等于16分频，ECDSA认证运算和PKV运算才能工作正常，其余运算不受影响*/
    HT_CMU_Init(&CMU_InitStructure);
	
//----ESDSA签名运算-----------hardware实现--------		
/*ECDSA初始化结构体指针*/												/*此处每次重新运算都要初始化结构体指针*/
		ECC_ECDSA_InitStruct.pECCaAddr = ECC_a.u32;
		ECC_ECDSA_InitStruct.pECCpAddr = ECC_p.u32;
		ECC_ECDSA_InitStruct.pECCGxAddr = ECC_Gx.u32;
		ECC_ECDSA_InitStruct.pECCGyAddr = ECC_Gy.u32;
		ECC_ECDSA_InitStruct.pECCnAddr = ECC_n.u32;
/*ECDSA签名运算输入结构体初始化*/	
		ECDSA_S_InputStruct.pdAddr = ECC_d.u32;
		ECDSA_S_InputStruct.pkAddr = ECC_k.u32;
		ECDSA_S_InputStruct.pMAddr = ECC_M.u32;
/*ECDSA输出结构体初始化*/
		ECDSA_S_OutputStruct.pRxAddr = ECC_tmp0.u32;
		ECDSA_S_OutputStruct.pRyAddr = ECC_tmp1.u32;
		for(i=0;i<8;i++)
		{
			ECC_tmp0.u32[i] = 0x00000000;
			ECC_tmp1.u32[i] = 0x00000000;
		}
/*ECDSA签名运算*/		
		if(TRUE == ECDSA_Signature(&ECC_ECDSA_InitStruct,&ECDSA_S_InputStruct,&ECDSA_S_OutputStruct))
		{
			if( (EQUAL ==BigDatCompare(ECC_tmp0.u32,ECC_r.u32,8)) && 
					(EQUAL ==BigDatCompare(ECC_tmp1.u32,ECC_s.u32,8)))
			{
					ReturnTemp |= 0x1;						//PASS
			}
			else
			{
					ReturnTemp &= (~0x1);					//FAIL
			}				
		}
		else
		{
			ReturnTemp &= (~0x1);							//FAIL
		}
//----ESDSA签名运算-----------hardware实现--------END		
		
//----ECDSA认证运算-----------hardware实现--------	
/*ECDSA初始化结构体指针*/											/*此处每次重新运算都要初始化结构体指针*/
		ECC_ECDSA_InitStruct.pECCaAddr = ECC_a.u32;
		ECC_ECDSA_InitStruct.pECCpAddr = ECC_p.u32;
		ECC_ECDSA_InitStruct.pECCGxAddr = ECC_Gx.u32;
		ECC_ECDSA_InitStruct.pECCGyAddr = ECC_Gy.u32;
		ECC_ECDSA_InitStruct.pECCnAddr = ECC_n.u32;		
/*ECDSA认证运算输入结构体初始化*/
		ECDSA_V_InputStruct.pECCDxAddr = ECC_Dx.u32;
		ECDSA_V_InputStruct.pECCDyAddr = ECC_Dy.u32;
		ECDSA_V_InputStruct.pMAddr = ECC_M.u32;
		ECDSA_V_InputStruct.prAddr = ECC_r.u32;
		ECDSA_V_InputStruct.psAddr = ECC_s.u32;
/*ECDSA认证运算*/	
		if(TRUE == ECDSA_Verification(&ECC_ECDSA_InitStruct,&ECDSA_V_InputStruct))
		{
				ReturnTemp |= (0x2);							//PASS
		}
		else
		{
				ReturnTemp &= (~0x2);							//FAIL
		}
//----ECDSA认证运算-----------hardware实现--------END
		
//----ECDSA认证运算------half software实现--------	
/*ECDSA初始化结构体指针*/											/*此处每次重新运算都要初始化结构体指针*/
		ECC_ECDSA_InitStruct.pECCaAddr = ECC_a.u32;
		ECC_ECDSA_InitStruct.pECCpAddr = ECC_p.u32;
		ECC_ECDSA_InitStruct.pECCGxAddr = ECC_Gx.u32;
		ECC_ECDSA_InitStruct.pECCGyAddr = ECC_Gy.u32;
		ECC_ECDSA_InitStruct.pECCnAddr = ECC_n.u32;		
/*ECDSA认证运算输入结构体初始化*/
		ECDSA_V_InputStruct.pECCDxAddr = ECC_Dx.u32;
		ECDSA_V_InputStruct.pECCDyAddr = ECC_Dy.u32;
		ECDSA_V_InputStruct.pMAddr = ECC_M.u32;
		ECDSA_V_InputStruct.prAddr = ECC_r.u32;
		ECDSA_V_InputStruct.psAddr = ECC_s.u32;
/*ECDSA认证运算*/	
		if(TRUE == ECDSA_Verification_HalfSoft(&ECC_ECDSA_InitStruct,&ECDSA_V_InputStruct))
		{
				ReturnTemp |= (0x4);
		}
		else
		{
				ReturnTemp &= (~0x4);
		}		
//----ECDSA认证运算------half software实现--------END
		
//----SHA256运算--------------software实现--------			
/*SHA256 输入参数初始化*/
		SHA256_TestCase_Init();
/*SHA256初始化结构体指针*/											/*此处每次重新运算都要初始化结构体指针*/
		SHA256_InputStruct.pMtestAddr = SHA_m.u8;
		SHA256_InputStruct.LenMtest = LenSHA_m;
/*SHA256运算*/		
		SHA256(&SHA256_InputStruct,SHA_M.u8);
		if( EQUAL == BigDatCompare(ECC_M.u32,SHA_M.u32,8))
		{
				ReturnTemp |= (0x8);
		}
		else
		{
				ReturnTemp &= (~0x8);
		}
//----SHA256运算--------------software实现--------END	
		
		return(ReturnTemp);
}

/*
*********************************************************************************************************
*                              ECDSA test case initial
*
* 函数说明: ESDSA testcase 初始化
*
* 入口参数: 
*
* 特殊说明: 
*********************************************************************************************************
*/
void ECDSA_TestCase_Init(void)
{

		ECC_p.u64[3] = 0xFFFFFFFF00000001;			//p=2[256]-2[224]+2[192]+2[96]-1
		ECC_p.u64[2] = 0x0000000000000000;
		ECC_p.u64[1] = 0x00000000FFFFFFFF;
		ECC_p.u64[0] = 0xFFFFFFFFFFFFFFFF;
	
		ECC_a.u64[3] = 0xFFFFFFFF00000001;			//a=-3 mod p
		ECC_a.u64[2] = 0x0000000000000000;
		ECC_a.u64[1] = 0x00000000FFFFFFFF;
		ECC_a.u64[0] = 0xFFFFFFFFFFFFFFFC;
	
		ECC_b.u64[3] = 0x5AC635D8AA3A93E7;			//b
		ECC_b.u64[2] = 0xB3EBBD55769886BC;
		ECC_b.u64[1] = 0x651D06B0CC53B0F6;
		ECC_b.u64[0] = 0x3BCE3C3E27D2604B;
	
		ECC_Gx.u64[3] = 0x6B17D1F2E12C4247;			//Gx
		ECC_Gx.u64[2] = 0xF8BCE6E563A440F2;
		ECC_Gx.u64[1] = 0x77037D812DEB33A0;
		ECC_Gx.u64[0] = 0xF4A13945D898C296;
	
		ECC_Gy.u64[3] = 0x4FE342E2FE1A7F9B;			//Gy
		ECC_Gy.u64[2] = 0x8EE7EB4A7C0F9E16;
		ECC_Gy.u64[1] = 0x2BCE33576B315ECE;
		ECC_Gy.u64[0] = 0xCBB6406837BF51F5;		
	
		ECC_n.u64[3] = 0xFFFFFFFF00000000;			//n
		ECC_n.u64[2] = 0xFFFFFFFFFFFFFFFF;
		ECC_n.u64[1] = 0xBCE6FAADA7179E84;
		ECC_n.u64[0] = 0xF3B9CAC2FC632551;

		ECC_M.u64[3] = 0x76823489310398B8;			//hash(m)
		ECC_M.u64[2] = 0x30799E95A8A1819B;
		ECC_M.u64[1] = 0x0FC44253C1ED704C;
		ECC_M.u64[0] = 0xB6030C8248BA22FE;
		
		ECC_d.u64[3] = 0x101D35748FA6C773;			//私钥d
		ECC_d.u64[2] = 0xDEF6BBBC4E31D720;
		ECC_d.u64[1] = 0xD7CB07C369060856;
		ECC_d.u64[0] = 0xA4EE9F7A9905C315;
		
		ECC_k.u64[3] = 0x732B578FCD30BA63;			//随机大整数k
		ECC_k.u64[2] = 0x6965A177596065A1;
		ECC_k.u64[1] = 0x835C026B3BBCE990;
		ECC_k.u64[0] = 0xF360AD51AB6EC9CA;

		ECC_Dx.u64[3] = 0xCA9FDDF2F526D6A3;			//公钥D(x,y)
		ECC_Dx.u64[2] = 0x3D1CE42A5B1DDB74;
		ECC_Dx.u64[1] = 0xF1809CADAB9AC14C;
		ECC_Dx.u64[0] = 0x84B1AF00A3B7CE8E;	
		
		ECC_Dy.u64[3] = 0xC5F7CD9057622549;
		ECC_Dy.u64[2] = 0x17B1C6D5E6535613;
		ECC_Dy.u64[1] = 0xC2BB7079F54E7F2D;
		ECC_Dy.u64[0] = 0x9B4A8CFB91A2C035;	

		ECC_s.u64[3] = 0xFDAB2982C6FBE1A1;		//签名结果(r,s)
		ECC_s.u64[2] = 0x24C854F8C94FD52A;
		ECC_s.u64[1] = 0xE4B694D04554B84F;
		ECC_s.u64[0] = 0x74FF7D4FA543B25D;			

		ECC_r.u64[3] = 0x124C6384C629842A;
		ECC_r.u64[2] = 0x92ED98844EA505FF;
		ECC_r.u64[1] = 0x0BF33C456AA4ABA8;
		ECC_r.u64[0] = 0x9C3D84EC3ABDF10E;	
	
}
/*
*********************************************************************************************************
*                              SHA256 test case initial
*
* 函数说明: SHA256 testcase 初始化
*
* 入口参数: 
*
* 特殊说明: 
*********************************************************************************************************
*/
void SHA256_TestCase_Init(void)
{
			uint8_t i;
//			uint8_t Mtemp[] = "qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy87923523bjk";
																											/* 消息原文*/
			for(i=0;i<16;i++)
			{
				SHA_m.u64[i] = 0x0;														/*清零消息数组*/
			}
//			for(i=0;i<sizeof(temp);i++)
//			{
//				SHA_m.u8[i] = Mtemp[i];											 /*初始化消息数组*/
//			}

//case 0
		strcpy(SHA_m.u8,"qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy879qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy879");
																											/*初始化消息数组*/
		ECC_M.u64[3] = 0x99bf8093cd9c98db;			//hash(m)
		ECC_M.u64[2] = 0x84fe52e83ebe9a3c;
		ECC_M.u64[1] = 0xa959180f2cc96663;
		ECC_M.u64[0] = 0x0ea3ca5e65d55e52;
		LenSHA_m = 896/8;			
//case 1
//		strcpy(SHA_m.u8,"qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy87923523bjk");
//																											/*初始化消息数组*/
//		ECC_M.u64[3] = 0xb98b0260087cab2f;			//hash(m)
//		ECC_M.u64[2] = 0x0c04042874323779;
//		ECC_M.u64[1] = 0x9942fdbbc878ec7a;
//		ECC_M.u64[0] = 0x25e463312fefd726;
//		LenSHA_m = 512/8;
//case2
//		strcpy(SHA_m.u8,"qwertyuiopasdfghjklsdfhjknbsdjkfhwyer97234725wjnrlkjy879");
//																											/*初始化消息数组*/
//		ECC_M.u64[3] = 0x98c65f99b6613bc4;			//hash(m)
//		ECC_M.u64[2] = 0xa76a82abe6756b87;
//		ECC_M.u64[1] = 0xbf8f4fcd00fabeba;
//		ECC_M.u64[0] = 0x8173a25d7264e783;			
//		LenSHA_m = 448/8;

}


#endif                                        /* This File Only support HT6x2x and HT501x */


#endif  /* __EXAMPLE_C */
