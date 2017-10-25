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
* File         : aes_example.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __AES_EXAMPLE_C
//#undef   __AES_EXAMPLE_C

#ifdef   __AES_EXAMPLE_C

#include "aes_example.h"
#include "ht6xxx_lib.h"
#include "TypeRAM.h"
#include "Data.h"
#include "COSEMComm.h"
#include "Mem.h"
#include "General.h"

#if defined HT6x2x  ||  defined  HT501x             /* This File Only support HT6x2x and HT501x */ 

/*
*********************************************************************************************************
*                                           ±æµÿ∫Í/Ω·ππÃÂ
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             ±æµÿ±‰¡ø
*********************************************************************************************************
*/
	/*±‰¡ø∂®“Â*/		
			Parameter128bits_TypeDef	AesKey; 						//√‹‘ø
		//	Parameter256bits_TypeDef	AesKey; 						//√‹‘ø
			AESKeyMode_TypeDef	AESKeyMode; 							//º”√‹∑Ω Ω
		//	Parameter512bits_TypeDef	Plaintext;					//√˜Œƒ	
		    Parameter1024bits_TypeDef	Plaintext;					//√˜Œƒ
			uint64_t	LenPtext;														//√˜ŒƒŒª≥§
		//	Parameter512bits_TypeDef	Ciphertext; 				//√‹Œƒ
			Parameter1024bits_TypeDef	Ciphertext; 				//√‹Œƒ
			uint64_t	LenCtext;														//√‹ŒƒŒª≥§
		//	Parameter256bits_TypeDef	AddAuthData;				//∏Ωº”»œ÷§ ˝æ›
			Parameter1024bits_TypeDef	AddAuthData;				//∏Ωº”»œ÷§ ˝æ›
			uint64_t	LenAdata;														//∏Ωº”»œ÷§ ˝æ›Œª≥§
		//	Parameter512bits_TypeDef	InitVector; 				//≥ı ºœÚ¡ø
			Parameter128bits_TypeDef	InitVector; 				//≥ı ºœÚ¡ø
			uint64_t	LenIV;															//≥ı ºœÚ¡øŒª≥§
			Parameter128bits_TypeDef	AuthTag;						//»œ÷§±Í«©
			Bool DecryptSta;															//Ω‚√‹≥…π¶◊¥Ã¨
			
			Parameter128bits_TypeDef	CTR_Counter0;				//CTR≥ı ºº∆ ˝∆˜÷µ


/*
*********************************************************************************************************
*                                           ±æµÿ∫Ø ˝…Í√˜
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                GHASH H≤Œ ˝≥ı ºªØº∆À„
*
* ∫Ø ˝Àµ√˜: H≤Œ ˝≥ı ºªØº∆À„
*						À„∑®£∫	H=E(K,0[128]),    E()Œ™aesº”√‹£¨0[128]Œ™128Œª'0'£¨KŒ™key√‹‘ø
* »Îø⁄≤Œ ˝: 
*						AESKeyMode  		AESKeyMode_TypeDef–Õ±‰¡ø£¨º”√‹ƒ£ Ω
*														KEY_Mode_128bits
*														KEY_Mode_192bits
*														KEY_Mode_256bits
*						pKeyAddr		  	uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢√‹‘ø
*						pOutDataAddr  	uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢H≥ı º÷µ: pIntDataAddr[0]Œ™µÕŒª
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
void GCM_GhashHeader_init(AESKeyMode_TypeDef AESKeyMode,uint32_t *pKeyAddr,uint32_t *pOutDataAddr)
{
		uint8_t i;
		Parameter128bits_TypeDef	Aes_ghash_temp;										/*!< ∂®“Â128bitsµƒ¡Ÿ ±±‰¡ø				*/
		
		for(i=0;i<4;i++)
		{
				Aes_ghash_temp.u32[i]=0x0000;	
		}
		HT_AES_Encrypt(AESKeyMode,pKeyAddr,Aes_ghash_temp.u32);			/*!< E(K,0[128])£¨aesº”√‹‘ÀÀ„			*/
		
		while(HT_AES_Busy_StatusGet());															/*!< AESº”√‹Ω· ¯≈–∂œ							*/

		HT_AES_Read(pOutDataAddr);																	/*!< º”√‹‘ÀÀ„Ω·π˚–¥»Î	*pOutDataAddr	*/
}

/*
*********************************************************************************************************
*                                GHASH£®H,A,C) »œ÷§∫Ø ˝º∆À„
*
* ∫Ø ˝Àµ√˜: GHASH£®H,A,C) »œ÷§∫Ø ˝º∆À„,
*						À„∑®£∫	GHASH(H,A,C)	= Xm+n+1,    Xm+n+1Œ™ghash»œ÷§Ω·π˚£¨mŒ™Aµƒ∑÷∂Œ ˝£®128bitsŒ™1∂Œ£©£¨nŒ™cµƒ∑÷∂Œ ˝,AŒ™auth data£®»œ÷§ ˝æ›£©£¨CŒ™ciphertext£®√‹Œƒ£©
*										Xi = 	0																,for i=0		
*											 =	(Xi-1 ^ Ai) . H									,for i=1,...,m-1
*											 =	(Xm-1 ^ Am||0[128-v]) . H				,for i=m												
*											 =	(Xi-1 ^ Ci-m) . H								,for i=m+1,...,m+n-1
*											 =	(Xm+n-1 ^ Cn||0[128-u]) . H			,for i=m+n
*											 =	(Xm+n	^ (len(A)||len(C))) . H		,for i=m+n+1
*													◊¢:Am||0[128-v]£¨Œ™A◊Ó∫Û∑÷∂Œ≤ª◊„128bits≤π¡„£¨vŒ™◊Ó∫Û∑÷∂ŒŒª≥§
*														 Cn||0[128-u]£¨Œ™C◊Ó∫Û∑÷∂Œ≤ª◊„128bits≤π¡„£¨uŒ™◊Ó∫Û∑÷∂ŒŒª≥§
*														 len(A),∑µªÿ“ª∏ˆ64-bits ◊÷∑˚¥Æ£¨±Ì æ ˝æ›AµƒŒª≥§£®bit£©
* »Îø⁄≤Œ ˝: 
*						GhashHeaderAddr		uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢GhashHeader≥ı÷µ
*						pADataAddr  			uint8_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢A÷µ
*						LenAData					uint64_t –Õ±‰¡ø£¨±Ì æA÷µ≥§∂»£®µ•Œªbyte£©
*						*pCtextAddr  			uint8_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢C÷µ
*						LenCtext					uint64_t –Õ±‰¡ø£¨±Ì æC÷µ≥§∂»£®µ•Œªbyte£©
*						OutData						Parameter128bits_TypeDef*±‰¡ø£z∆‰÷∏œÚ∂‘œÛ¥Ê¥¢ghash∫Ø ˝‘ÀÀ„Ω·π˚
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
void GCM_GhashAuth(uint32_t *GhashHeaderAddr,uint8_t *pADataAddr,uint64_t LenAData,uint8_t *pCtextAddr,uint64_t LenCtext,Parameter128bits_TypeDef* OutData)
{
		uint8_t i; 
		Parameter128bits_TypeDef	Aes_ghash_temp,LenData_temp;
	
		for(i=0;i<4;i++)
		{
				OutData->u32[i]=0x0000;																/*!< OutData±‰¡ø	∏≥÷µ0x0000				*/
		}
	
		/*(len(A)||len(C))‘ÀÀ„  */
		LenData_temp.u64[0] = LenCtext * 8;												/*!<(len(A)||len(C))‘ÀÀ„Ω·π˚ª∫¥Ê		*/
		LenData_temp.u64[1] = LenAData * 8;
	
		/*Ai‘ÀÀ„≤ø∑÷  */
		for(i=0;i<4;i++)
		{
				OutData->u32[i]=0x0000;																/*!< OutData±‰¡ø	∏≥÷µ0x0000				*/
				Aes_ghash_temp.u32[i]=0x0000;													/*!< ¡Ÿ ±±‰¡ø	∏≥÷µ0x0000						*/
		}
		//(Xi-1 ^ Ai) . H							,for i=1,...,m-1
		while(LenAData>=16)																				/*!< AŒª≥§≈–∂œ	16bytes£®128bits£©	*/
		{
				for(i=0;i<16;i++)
				{
						OutData->u8[i] ^= *pADataAddr++;										/*!< (Xi-1 ^ Ai), 128bits “ÏªÚ≤Ÿ◊˜	*/
				}
				LenAData -= 16;																				/*!< œ¬∏ˆ128bit∑÷∂Œ									*/
				HT_GHASH_Multiply(OutData->u32, GhashHeaderAddr);			/*!< £®OutData . H£©≤Ÿ◊˜						*/
				while(HT_GHASH_Busy_StatusGet());											/*!< Ghash≥À∑®‘ÀÀ„ÕÍ≥…≈–∂œ					*/
				HT_GHASH_Multiply_Read(OutData->u32);									/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	OutData		*/
		}
		//(Xm-1 ^ Am||0[128-v]) . H			,for i=m
		if(LenAData)
		{
				for(i=0;i<4;i++)
				{
						Aes_ghash_temp.u32[i]=0x0000;											/*!< ¡Ÿ ±±‰¡ø	∏≥÷µ0x0000						*/
				}
				for(i=16-LenAData;i<16;i++)
				{
						Aes_ghash_temp.u8[i] = *pADataAddr++;							/*!< ≤ª◊„128bitsµƒADataÀÕµΩAes_ghash_temp∏ﬂŒª	£¨µÕŒª≤π¡„	*/
				}
				HT_AES_Xor128bits(OutData->u32,Aes_ghash_temp.u32, OutData->u32);
																															/*!< ≤π¡„∫ÛµƒAData◊Ó∫Û∂Œ”ÎOutData“ÏªÚ‘ÀÀ„£¨Ω·π˚¥Ê”⁄OutData	*/
				HT_GHASH_Multiply(OutData->u32, GhashHeaderAddr);			/*!< £®OutData . H£©≤Ÿ◊˜						*/
				while(HT_GHASH_Busy_StatusGet());											/*!< Ghash≥À∑®‘ÀÀ„ÕÍ≥…≈–∂œ					*/
				HT_GHASH_Multiply_Read(OutData->u32);									/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	OutData		*/
		}	
		
	/*Ci‘ÀÀ„≤ø∑÷*/
		//(Xi-1 ^ Ci-m) . H								,for i=m+1,...,m+n-1
		while(LenCtext>=16)																				/*!< CŒª≥§≈–∂œ	16bytes£®128bits£©	*/
		{
				for(i=0;i<16;i++)
				{
						OutData->u8[i] ^= *pCtextAddr++;										/*!< (Xi-1 ^ Ci-m), 128bits “ÏªÚ≤Ÿ◊˜	*/
				}
				LenCtext -= 16;																				/*!< œ¬∏ˆ128bit∑÷∂Œ									*/
				HT_GHASH_Multiply(OutData->u32, GhashHeaderAddr);			/*!< £®OutData . H£©≤Ÿ◊˜						*/
				while(HT_GHASH_Busy_StatusGet());											/*!< Ghash≥À∑®‘ÀÀ„ÕÍ≥…≈–∂œ					*/
				HT_GHASH_Multiply_Read(OutData->u32);									/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	OutData		*/
		}
		//(Xm+n-1 ^ Cn||0[128-u]) . H			,for i=m+n
		if(LenCtext)
		{
				for(i=0;i<4;i++)
				{
						Aes_ghash_temp.u32[i]=0x0000;											/*!< ¡Ÿ ±±‰¡ø	∏≥÷µ0x0000						*/
				}
				for(i=16-LenCtext;i<16;i++)
				{
						Aes_ghash_temp.u8[i] = *pCtextAddr++;							/*!< ≤ª◊„128bitsµƒCDataÀÕµΩAes_ghash_temp∏ﬂŒª	£¨µÕŒª≤π¡„	*/
				}
				HT_AES_Xor128bits(OutData->u32,Aes_ghash_temp.u32, OutData->u32);
																															/*!< ≤π¡„∫ÛµƒAData◊Ó∫Û∂Œ”ÎOutData“ÏªÚ‘ÀÀ„£¨Ω·π˚¥Ê”⁄OutData	*/
				HT_GHASH_Multiply(OutData->u32, GhashHeaderAddr);			/*!< £®OutData . H£©≤Ÿ◊˜						*/
				while(HT_GHASH_Busy_StatusGet());											/*!< Ghash≥À∑®‘ÀÀ„ÕÍ≥…≈–∂œ					*/
				HT_GHASH_Multiply_Read(OutData->u32);									/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	OutData		*/
		}	
		
	/*◊Ó∫Û‘ÀÀ„≤ø∑÷*/
		//(Xm+n	^ (len(A)||len(C))) . H		,for i=m+n+1
		HT_AES_Xor128bits(OutData->u32,LenData_temp.u32, OutData->u32);	
																															/*!< (len(A)||len(C))”ÎOutData“ÏªÚ‘ÀÀ„£¨Ω·π˚¥Ê”⁄OutData	*/
		HT_GHASH_Multiply(OutData->u32, GhashHeaderAddr);					/*!< £®OutData . H£©≤Ÿ◊˜						*/
		while(HT_GHASH_Busy_StatusGet());													/*!< Ghash≥À∑®‘ÀÀ„ÕÍ≥…≈–∂œ					*/
		HT_GHASH_Multiply_Read(OutData->u32);											/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	OutData		*/	
}

/*
*********************************************************************************************************
*                                GCM Counter0(Y0)≤Œ ˝≥ı ºªØº∆À„
*
* ∫Ø ˝Àµ√˜: Counter0(Y0)≤Œ ˝≥ı ºªØº∆À„
*						À„∑®£∫Y0 = IV || 0[31]1   ,if len(IV)=96 bits(12 bytes)
*										 = GHASH(H,{},IV)	,otherwise
*									IVŒ™≥ı ºœÚ¡ø£¨0[31]1Œ™31Œª'0'∫Õ1Œª'1',GHASH(H,{},IV)Œ™ghash‘ÀÀ„∫Ø ˝
* »Îø⁄≤Œ ˝: 
*						pInitVectorAddr  	uint8_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢IV≥ı º÷µ: pInitVectorAddr[0]Œ™µÕŒª
*						LenIV							uint64_t	≥ı ºœÚ¡øŒª≥§£¨µ•Œªbyte
*						GhashHeaderAddr		uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢GhashHeader£®H£©÷µ
*						Counter0					Parameter128bits_TypeDef*±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢ICounter0(y0)≥ı ºªØΩ·π˚
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
void GCM_Counter0_init(uint8_t *pInitVectorAddr,uint64_t LenIV,uint32_t *GhashHeaderAddr,Parameter128bits_TypeDef* Counter0)
{
		uint8_t i;
		uint8_t	*ptemp;																							/*!< ¡Ÿ ±÷∏’Î±‰¡ø								*/															
		
		for(i=0;i<4;i++)
		{
				Counter0->u32[i] = 0x0000;																/*!< counter0	∏≥÷µ0x0000				*/
		}
		
		//Y0 = IV || 0[31]1				,if len(IV)=96 bits(12 bytes)
		if(LenIV == 12)																							/*!< IV≥§∂»12 bytes	(96 bits)		*/
		{
				for(i=4;i<16;i++)
				{
						Counter0->u8[i] = *pInitVectorAddr++;								/*!< IVÀÕµΩCounter0∏ﬂ96Œª				*/
				}
				Counter0->u32[0]=0x0001;																	/*!< Counter0µÕ32Œª	≤π0º”1			 */
		}
		
		//Y0 = GHASH(H,{},IV)			,otherwise
		else
		{
				GCM_GhashAuth(GhashHeaderAddr,ptemp,0x0,pInitVectorAddr,LenIV,Counter0);
																																/*!< Ghash»œ÷§‘ÀÀ„£¨GHASH(H,{},IV)	 */
		}
}

/*
*********************************************************************************************************
*                                GCM Counter(Yi)¿€º”º∆À„
*
* ∫Ø ˝Àµ√˜: Counter(Yi)¿€º”º∆À„
*						À„∑®£∫Yi = Yi-1 + 1   ,i=1,2...n
*								Yiµƒ¿€º”º∆À„£¨Ωˆ∂‘◊Ó”“±ﬂ32bits”––ß
*									
* »Îø⁄≤Œ ˝: 
*						Counter					Parameter128bits_TypeDef*±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢ICounter(Yi)¿€º”µƒcounter ˝æ›
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
void GCM_Counter_increase(Parameter128bits_TypeDef* Counter)
{
		Counter->u32[0]++;													/*!< Yi = Yi-1 + 1	£¨◊Ó”“±ﬂ32bits”––ß			*/
}

/*
*********************************************************************************************************
*                                GCM_CTRº”√‹/Ω‚√‹º∆À„
*
* ∫Ø ˝Àµ√˜: GCM_CTRº”√‹/Ω‚√‹º∆À„
*						º”√‹À„∑®£∫Ci = Pi ^ E(K,Yi)   				,for i=1,2,...,n-1
*											 		 Pn ^ MSBu(E(K,Yn))			,for i=n
*						Ω‚√‹À„∑®£∫Pi = Ci ^ E(K,Yi)   				,for i=1,2,...,n-1
*											 		 Cn ^ MSBu(E(K,Yn))			,for i=n
*									PiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n-1)*128+u bits£¨1<= u <=128
*									CiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n-1)*128+u bits£¨1<= u <=128
*									E(K,Yi)Œ™∂‘YiΩ¯––AESº”√‹‘ÀÀ„
*									MSBu(S)Œ™∑µªÿ1◊÷∑˚¥Æ£¨◊÷∑˚¥Æ∞¸∫¨S ˝æ›µƒ◊Ó◊Û±ﬂu∏ˆbits
* »Îø⁄≤Œ ˝: 
*						pInttextAddr  	uint8_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢¥˝º”/Ω‚√‹ ˝æ›:
*						LenInttext			uint64_t	¥˝º”/Ω‚√‹ ˝æ›Œª≥§£¨µ•Œªbyte
*						Counter0				Parameter128bits_TypeDef*±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢Counter0(y0)≥ı÷µ
*						AESKeyMode			√∂æŸ±‰¡ø£¨—°‘Òº”√‹ƒ£ Ω
*														KEY_Mode_128bits
*														KEY_Mode_192bits
*														KEY_Mode_256bits
*						pKeyAddr  			uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢AES key : pKeyAddr[0]Œ™∏ﬂŒª
*						pOuttextAddr		uint8_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢º”/Ω‚√‹ÕÍ≥… ˝æ›
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: º”√‹≤Ÿ◊˜ ±£¨pInttextAddr∫ÕLenInttextŒ™√˜Œƒ ‰»Î£¨pOuttextAddrŒ™√‹Œƒ ‰≥ˆ
*						Ω‚√‹≤Ÿ◊˜ ±£¨pInttextAddr∫ÕLenInttextŒ™√‹Œƒ ‰»Î£¨pOuttextAddrŒ™√˜Œƒ ‰≥ˆ
*********************************************************************************************************
*/
void GCM_CTR_Encrypt(uint8_t *pInttextAddr,uint64_t LenInttext,Parameter128bits_TypeDef* Counter0,AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr,uint8_t *pOuttextAddr)
{
		uint8_t i; 
		Parameter128bits_TypeDef	Counter_temp,Aes_GCM_temp;
	
		for(i=0;i<4;i++)
		{
				Counter_temp.u32[i] = Counter0->u32[i];								/*!< counter0¥ÊµΩCounter_temp±∏”√		*/
		}
	//Ci = Pi ^ E(K,Yi)   				,for i=1,2,...,n-1
		while(LenInttext>=16)																				/*!< PŒª≥§≈–∂œ	16bytes£®128bits£©	*/
		{
				GCM_Counter_increase(&Counter_temp);										/*!< GCM Counter(Yi)¿€º”º∆À„				*/
				HT_AES_Encrypt(AESKeyMode,pKeyAddr,Counter_temp.u32);
																															/*!< ∂‘Counter(Yi)AESº”√‹≤Ÿ◊˜	£¨ E(K,Yi) 			*/
				while(HT_AES_Busy_StatusGet());												/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_GCM_temp.u32);												/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_GCM_temp		*/
				for(i=0;i<16;i++)
				{
						*pOuttextAddr++ = Aes_GCM_temp.u8[i] ^ *pInttextAddr++;	
																															/*!< Pi ^ E(K,Yi)‘ÀÀ„								*/
				}
				LenInttext -= 16;																				/*!< œ¬∏ˆ128bit∑÷∂Œ									*/			
		}
	//Cn = Pn ^ MSBu(E(K,Yn))			,for i=n
		if(LenInttext)
		{
				GCM_Counter_increase(&Counter_temp);										/*!< GCM Counter(Yi)¿€º”º∆À„				*/
				HT_AES_Encrypt(AESKeyMode,pKeyAddr,Counter_temp.u32);
																															/*!< ∂‘Counter(Yn)AESº”√‹≤Ÿ◊˜	£¨ E(K,Yn) 	*/
				while(HT_AES_Busy_StatusGet());												/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_GCM_temp.u32);												/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_GCM_temp1		*/
				for(i=16-LenInttext;i<16;i++)
				{
						*pOuttextAddr++ = Aes_GCM_temp.u8[i] ^ *pInttextAddr++;							
																															/*!< Pn ^ MSBu(E(K,Yn))‘ÀÀ„					*/
				}
		}		
}

/*
*********************************************************************************************************
*                                GCM »œ÷§±Í«©º∆À„
*
* ∫Ø ˝Àµ√˜: GCM »œ÷§±Í«©º∆À„
*						º”√‹À„∑®£∫T = MSBt(GHASH(H,A,C) ^ E(K,Y0))  	
*
*									AiŒ™»œ÷§ ˝æ›∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œv bits£¨◊‹Œª≥§ (m-1)*128+v bits£¨1<= v <=128
*									CiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n-1)*128+u bits£¨1<= u <=128
*									E(K,Y0)Œ™∂‘Y0Ω¯––AESº”√‹‘ÀÀ„
*									MSBu(S)Œ™∑µªÿ1◊÷∑˚¥Æ£¨◊÷∑˚¥Æ∞¸∫¨S ˝æ›µƒ◊Ó◊Û±ﬂu∏ˆbits
* »Îø⁄≤Œ ˝: 
*						GhashAuthData		Parameter128bits_TypeDef*±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢GHASH(H,A,C)‘ÀÀ„Ω·π˚
*						Counter0				Parameter128bits_TypeDef*±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢Counter0(y0)≥ı÷µ
*						AESKeyMode			√∂æŸ±‰¡ø£¨—°‘Òº”√‹ƒ£ Ω
*														KEY_Mode_128bits
*														KEY_Mode_192bits
*														KEY_Mode_256bits
*						pKeyAddr  			uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢AES key : pKeyAddr[0]Œ™∏ﬂŒª
*						AuthTag					Parameter128bits_TypeDef*±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢»œ÷§±Í«© ˝æ›
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: º”√‹≤Ÿ◊˜ ±£¨pInttextAddr∫ÕLenInttextŒ™√˜Œƒ ‰»Î£¨pOuttextAddrŒ™√‹Œƒ ‰≥ˆ
*						Ω‚√‹≤Ÿ◊˜ ±£¨pInttextAddr∫ÕLenInttextŒ™√‹Œƒ ‰»Î£¨pOuttextAddrŒ™√˜Œƒ ‰≥ˆ
*********************************************************************************************************
*/
void GCM_TagAuth(Parameter128bits_TypeDef* GhashAuthData,Parameter128bits_TypeDef* Counter0,AESKeyMode_TypeDef AESKeyMode, uint32_t *pKeyAddr,Parameter128bits_TypeDef* AuthTag)
{
		Parameter128bits_TypeDef	Aes_GCM_temp;
	
		HT_AES_Encrypt(AESKeyMode,pKeyAddr,Counter0->u32);					/*!< ∂‘Counter(Y0)AESº”√‹≤Ÿ◊˜	£¨ E(K,Y0) 					*/
		while(HT_AES_Busy_StatusGet());														/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ													*/
		HT_AES_Read(Aes_GCM_temp.u32);														/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_GCM_temp						*/	

		HT_AES_Xor128bits(Aes_GCM_temp.u32,GhashAuthData->u32, AuthTag->u32);	
																															/*!< GHASH(H,A,C) ^ E(K,Y0)‘ÀÀ„£¨Ω·π˚¥Ê”⁄AuthTag	*/
}

/*
*********************************************************************************************************
*                                GCM º”√‹∫Ø ˝
*
* ∫Ø ˝Àµ√˜: GCM º”√‹∫Ø ˝
*
* »Îø⁄≤Œ ˝: GCM_InputStruct    GCMº”√‹ ‰»Î±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨8∏ˆ≤Œ ˝: 
*                              1) AESKeyMode     		: º”√‹Ω‚√‹ƒ£ Ω—°‘Ò
*                              2) *pKeyAddr       	: √‹‘øµÿ÷∑÷∏’Î
*                              3) *pInitVectorAddr  : ≥ı ºœÚ¡øµÿ÷∑÷∏’Î
*                              4) LenIV      				: ≥ı ºœÚ¡ø ˝æ›≥§∂»£®byte£©
*                              5) *pPtextAddr  			: √˜Œƒ ˝æ›µÿ÷∑÷∏’Î
*                              6) LenPtext					: √˜Œƒ ˝æ›≥§∂»£®byte£©
*                              7) *pAdataAddr  			: ∏Ωº”»œ÷§ ˝æ›µÿ÷∑÷∏’Î
*                              8) LenAdata					: ∏Ωº”»œ÷§ ˝æ›≥§∂»£®byte£©
*						GCM_OutputStruct   GCMº”√‹ ‰≥ˆ±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨3∏ˆ≤Œ ˝: 
*                              1) *pCtextAddr     	: “—º”√‹ ˝æ›√‹Œƒµÿ÷∑÷∏’Î
*                              2) LenCtext       		: “—º”√‹ ˝æ›√‹Œƒ≥§∂»£®byte£©
*                              3) AuthTag  					: »œ÷§±Í«© ˝æ›µÿ÷∑÷∏’Î
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜:Œﬁ
*********************************************************************************************************
*/
void GCM_Encrypt(int COM_No,GCM_Encrypt_InputTypedef* GCM_InputStruct, GCM_Encrypt_OutputTypedef* GCM_OutputStruct)
{
		Parameter128bits_TypeDef	Header;																			//H÷µ
		Parameter128bits_TypeDef	Counter0;																		//Counter0÷µ
		Parameter128bits_TypeDef	Ghash_HAC;																	//GHASH(H,A,C)‘ÀÀ„Ω·π˚
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);									/*¥Úø™AESƒ£øÈ ±÷”				*/	

		GCM_GhashHeader_init(GCM_InputStruct->AESKeyMode,GCM_InputStruct->pKeyAddr,Header.u32);				
																																					/*!< º∆À„Header≥ı÷µ			*/
		GCM_Counter0_init(GCM_InputStruct->pInitVectorAddr,GCM_InputStruct->LenIV,Header.u32,&Counter0);		
																																					/*!< º∆À„Counter0≥ı÷µ		*/
		GCM_CTR_Encrypt(GCM_InputStruct->pPtextAddr,GCM_InputStruct->LenPtext,&Counter0,GCM_InputStruct->AESKeyMode,GCM_InputStruct->pKeyAddr,GCM_OutputStruct->pCtextAddr);
																																					/*!< gcm CTRº”√‹				*/
		GCM_OutputStruct->LenCtext = GCM_InputStruct->LenPtext;
    GCM_GhashAuth(Header.u32,GCM_InputStruct->pAdataAddr,GCM_InputStruct->LenAdata,GCM_OutputStruct->pCtextAddr,GCM_OutputStruct->LenCtext,&Ghash_HAC);
																																					/*!< GHASH(H.A.C)‘ÀÀ„			*/
		GCM_TagAuth(&Ghash_HAC,&Counter0,GCM_InputStruct->AESKeyMode,GCM_InputStruct->pKeyAddr,GCM_OutputStruct->AuthTag);			
																																					/*!< »œ÷§±Í«©‘ÀÀ„					*/		
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE);									/*πÿ±’AESƒ£øÈ ±÷”				*/		
}

/*
*********************************************************************************************************
*                                GCM Ω‚√‹∫Ø ˝
*
* ∫Ø ˝Àµ√˜:  GCM Ω‚√‹∫Ø ˝
*
* »Îø⁄≤Œ ˝: GCM_InputStruct    GCMº”√‹ ‰»Î±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨8∏ˆ≤Œ ˝: 
*                              1) AESKeyMode     		: º”√‹Ω‚√‹ƒ£ Ω—°‘Ò
*                              2) *pKeyAddr       	: √‹‘øµÿ÷∑÷∏’Î
*                              3) *pInitVectorAddr  : ≥ı ºœÚ¡øµÿ÷∑÷∏’Î
*                              4) LenIV      				: ≥ı ºœÚ¡ø ˝æ›≥§∂»£®byte£©
*                              5) *pCtextAddr     	:  ˝æ›√‹Œƒµÿ÷∑÷∏’Î
*                              6) LenCtext       		:  ˝æ›√‹Œƒ≥§∂»£®byte£
*                              7) *pAdataAddr  			: ∏Ωº”»œ÷§ ˝æ›µÿ÷∑÷∏’Î
*                              8) LenAdata					: ∏Ωº”»œ÷§ ˝æ›≥§∂»£®byte£©
*                              9) AuthTag  					: »œ÷§±Í«© ˝æ›µÿ÷∑÷∏’Î
*						GCM_OutputStruct   GCMº”√‹ ‰≥ˆ±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨3∏ˆ≤Œ ˝: 
*                              1) *pPtextAddr  			: √˜Œƒ ˝æ›µÿ÷∑÷∏’Î
*                              2) LenPtext					: √˜Œƒ ˝æ›≥§∂»£®byte£©
*
* ∑µªÿ≤Œ ˝: Bool    = TRUE£∫  »œ÷§≥…π¶
*                   = FALSE£∫	»œ÷§ ß∞‹                                   
* 
* Ãÿ ‚Àµ√˜:Œﬁ
*********************************************************************************************************
*/
Bool GCM_Decrypt(int COM_No,GCM_Decrypt_InputTypedef* GCM_InputStruct, GCM_Decrypt_OutputTypedef* GCM_OutputStruct)
{
		Parameter128bits_TypeDef	Header;																			//H÷µ
		Parameter128bits_TypeDef	Counter0;																		//Counter0÷µ
		Parameter128bits_TypeDef	Ghash_HAC;																	//GHASH(H,A,C)‘ÀÀ„Ω·π˚
		Parameter128bits_TypeDef	AuthTag_Temp;
		uint8_t i;
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);									/*¥Úø™AESƒ£øÈ ±÷”				*/	

		GCM_GhashHeader_init(GCM_InputStruct->AESKeyMode,GCM_InputStruct->pKeyAddr,Header.u32);				
																																					/*!< º∆À„Header≥ı÷µ			*/
		GCM_Counter0_init(GCM_InputStruct->pInitVectorAddr,GCM_InputStruct->LenIV,Header.u32,&Counter0);		/*!< º∆À„Counter0≥ı÷µ		*/
																																					
		if((crypt.type[COM_No] == 0x10)||(crypt.type[COM_No] == 0x30))																				/*!< GHASH(H.A.C)‘ÀÀ„			*/
		{
		    GCM_GhashAuth(Header.u32,GCM_InputStruct->pAdataAddr,GCM_InputStruct->LenAdata,GCM_InputStruct->pCtextAddr,GCM_InputStruct->LenCtext,&Ghash_HAC);
			GCM_TagAuth(&Ghash_HAC,&Counter0,GCM_InputStruct->AESKeyMode,GCM_InputStruct->pKeyAddr,&AuthTag_Temp);			
			for(i=0;i<3;i++)
			{
					if(AuthTag_Temp.u32[3-i] != GCM_InputStruct->AuthTag->u32[i] )
					{
							return	FALSE;																								/*!< »œ÷§ ß∞‹£¨∑µªÿ	FALSE	*/	
					}
			}
		}
		GCM_CTR_Encrypt(GCM_InputStruct->pCtextAddr,GCM_InputStruct->LenCtext,&Counter0,GCM_InputStruct->AESKeyMode,GCM_InputStruct->pKeyAddr,GCM_OutputStruct->pPtextAddr);
																																					/*!< gcm CTRΩ‚√‹				*/
		GCM_OutputStruct->LenPtext = GCM_InputStruct->LenCtext;
		
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE);								/*πÿ±’AESƒ£øÈ ±÷”				*/	
		
		return	TRUE;																													/*!< Ω‚√‹≥…π¶£¨∑µªÿ	TRUE	*/	
}

/*
*********************************************************************************************************
*                                ECB º”√‹º∆À„
*
* ∫Ø ˝Àµ√˜: ECB º”√‹º∆À„
*
*						º”√‹À„∑®£∫Ci = E(K,Pi)   							,for i=1,2,...,n-1
*											 		 E(K,Pn||0[128-u]))			,for i=n
*									PiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n-1)*128+u bits£¨1<= u <=128
*									CiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n)*128 bits
*									E(K,Pi)Œ™∂‘PiΩ¯––AESº”√‹‘ÀÀ„
*									Pn||0[128-u]£¨Œ™P◊Ó∫Û∑÷∂Œ≤ª◊„128bits≤π¡„£¨uŒ™◊Ó∫Û∑÷∂ŒŒª≥§
*
* »Îø⁄≤Œ ˝: ECB_InputStruct    ECBº”√‹ ‰»Î±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨4∏ˆ≤Œ ˝: 
*                              1) AESKeyMode     		: º”√‹Ω‚√‹ƒ£ Ω—°‘Ò
*                              2) *pKeyAddr       	: √‹‘øµÿ÷∑÷∏’Î
*                              3) *pPtextAddr  			: √˜Œƒ ˝æ›µÿ÷∑÷∏’Î
*                              4) LenPtext					: √˜Œƒ ˝æ›≥§∂»£®byte£©
*						ECB_OutputStruct   ECBº”√‹ ‰≥ˆ±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨2∏ˆ≤Œ ˝: 
*                              1) *pCtextAddr     	: “—º”√‹ ˝æ›√‹Œƒµÿ÷∑÷∏’Î
*                              2) LenCtext       		: “—º”√‹ ˝æ›√‹Œƒ≥§∂»£®byte£©
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: 
*********************************************************************************************************
*/
void ECB_Encrypt(ECB_Encrypt_InputTypedef* ECB_InputStruct,ECB_Encrypt_OutputTypedef* ECB_OutputStruct)
{
		uint8_t i; 
		Parameter128bits_TypeDef	Aes_ECB_temp;
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);						/*¥Úø™AESƒ£øÈ ±÷”						*/	
	
		if(ECB_InputStruct->LenPtext%16 == 0)														/*!< º∆À„√‹Œƒ≥§∂»						*/
		{
				ECB_OutputStruct->LenCtext = ECB_InputStruct->LenPtext;
		}
		else
		{
				ECB_OutputStruct->LenCtext = ECB_InputStruct->LenPtext - (ECB_InputStruct->LenPtext%16) +16;
		}

	//Ci =  E(K,Pi)   				,for i=1,2,...,n-1
		while(ECB_InputStruct->LenPtext>=16)														/*!< PŒª≥§≈–∂œ	16bytes£®128bits£©	*/
		{
				for(i=0;i<16;i++)
				{
						Aes_ECB_temp.u8[i] = *ECB_InputStruct->pPtextAddr++;		/*!< Pi¥Ê»Î¡Ÿ ±±‰¡øAes_ECB_temp			*/
				}
				HT_AES_Encrypt(ECB_InputStruct->AESKeyMode,ECB_InputStruct->pKeyAddr,Aes_ECB_temp.u32);
																																		/*!< ∂‘Pi AESº”√‹≤Ÿ◊˜	£¨ E(K,Pi) 			*/
				while(HT_AES_Busy_StatusGet());															/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_ECB_temp.u32);															/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_ECB_temp	*/
				for(i=0;i<16;i++)
				{
						*ECB_OutputStruct->pCtextAddr++ = Aes_ECB_temp.u8[i];	
																																		/*!< Ω´√‹Œƒ±£¥Ê											*/
				}
				ECB_InputStruct->LenPtext -= 16;														/*!< œ¬∏ˆ128bit∑÷∂Œ									*/			
		}
	//Cn = E(K,Pn||0[128-u])			,for i=n
		if(ECB_InputStruct->LenPtext)
		{
				for(i=0;i<4;i++)
				{
						Aes_ECB_temp.u32[i] = 0x0000;														/*!< Aes_ECB_temp«Â¡„								*/
				}	
				for(i=16-ECB_InputStruct->LenPtext;i<16;i++)
				{
						Aes_ECB_temp.u8[i] = *ECB_InputStruct->pPtextAddr++;		/*!< Pi¥Ê»Î¡Ÿ ±±‰¡øAes_ECB_temp			*/
				}				
				HT_AES_Encrypt(ECB_InputStruct->AESKeyMode,ECB_InputStruct->pKeyAddr,Aes_ECB_temp.u32);
																																		/*!< ∂‘Pi AESº”√‹≤Ÿ◊˜	£¨ E(K,Pi) 		*/
				while(HT_AES_Busy_StatusGet());															/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_ECB_temp.u32);															/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_ECB_temp		*/
				for(i=0;i<16;i++)
				{
						*ECB_OutputStruct->pCtextAddr++ = Aes_ECB_temp.u8[i];	
				}																														/*!< Ω´√‹Œƒ±£¥Ê											*/
		}	
		
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE);					/*πÿ±’AESƒ£øÈ ±÷”										*/	
}	

/*
*********************************************************************************************************
*                                ECB Ω‚√‹º∆À„
*
* ∫Ø ˝Àµ√˜: ECB Ω‚√‹º∆À„
*
*						Ω‚√‹À„∑®£∫Pi = E'(K,Ci)   							,for i=1,2,...,n
*									PiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n)*128 bits
*									CiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n)*128 bits
*									E'(K,Pi)Œ™∂‘PiΩ¯––AESΩ‚√‹‘ÀÀ„
*
* »Îø⁄≤Œ ˝: ECB_InputStruct    ECBΩ‚√‹ ‰»Î±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨4∏ˆ≤Œ ˝: 
*                              1) AESKeyMode     		: º”√‹Ω‚√‹ƒ£ Ω—°‘Ò
*                              2) *pKeyAddr       	: √‹‘øµÿ÷∑÷∏’Î
*                              3) *pCtextAddr  			: √‹Œƒ ˝æ›µÿ÷∑÷∏’Î
*                              4) LenCtext					: √‹Œƒ ˝æ›≥§∂»£®byte£©
*						ECB_OutputStruct   ECBΩ‚√‹ ‰≥ˆ±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨2∏ˆ≤Œ ˝: 
*                              1) *pPtextAddr     	: “—Ω‚√‹√˜Œƒµÿ÷∑÷∏’Î
*                              2) LenPtext       		: “—Ω‚√‹√˜Œƒ≥§∂»£®byte£©
*
* ∑µªÿ≤Œ ˝: Bool    = TRUE£∫  Ω‚√‹≥…π¶
*                   = FALSE£∫	Ω‚√‹ ß∞‹£¨√‹Œƒ”–ŒÛ                                      
* 
* Ãÿ ‚Àµ√˜: 
*********************************************************************************************************
*/
Bool ECB_Decrypt(ECB_Decrypt_InputTypedef* ECB_InputStruct,ECB_Decrypt_OutputTypedef* ECB_OutputStruct)
{
		uint8_t i; 
		Parameter128bits_TypeDef	Aes_ECB_temp;
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);						/*¥Úø™AESƒ£øÈ ±÷”						*/
	
		if(ECB_InputStruct->LenCtext%16 != 0 || ECB_InputStruct->LenCtext == 0)
		{
				return	FALSE;																							/*!< √‹ŒƒŒª≥§¥ÌŒÛ										*/
		}
		ECB_OutputStruct->LenPtext = ECB_InputStruct->LenCtext;					/*!< LenPtext = LenCtext						*/
		
	//Pi = E'(K,Ci)   							,for i=1,2,...,n
		while(ECB_InputStruct->LenCtext >= 16)													/*!< PŒª≥§≈–∂œ	16bytes£®128bits£©	*/
		{
				for(i=0;i<16;i++)
				{
						Aes_ECB_temp.u8[i] = *ECB_InputStruct->pCtextAddr++;		/*!< Pi¥Ê»Î¡Ÿ ±±‰¡øAes_ECB_temp			*/
				}
				HT_AES_Decrypt(ECB_InputStruct->AESKeyMode,ECB_InputStruct->pKeyAddr,Aes_ECB_temp.u32);
																																		/*!< ∂‘Pi AESº”√‹≤Ÿ◊˜	£¨ E(K,Pi) 		*/
				while(HT_AES_Busy_StatusGet());															/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_ECB_temp.u32);															/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_ECB_temp	*/
				for(i=0;i<16;i++)
				{
						*ECB_OutputStruct->pPtextAddr++ = Aes_ECB_temp.u8[i];	
																																		/*!< Ω´√‹Œƒ±£¥Ê											*/
				}
				ECB_InputStruct->LenCtext -= 16;														/*!< œ¬∏ˆ128bit∑÷∂Œ									*/			
		}
		
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE);					/*πÿ±’AESƒ£øÈ ±÷”										*/
		
		return TRUE;																										/*!< Ω‚√‹≥…π¶												*/
}

/*
*********************************************************************************************************
*                                CBC º”√‹º∆À„
*
* ∫Ø ˝Àµ√˜: CBC º”√‹º∆À„
*
*						º”√‹À„∑®£∫Ci = E(K,(P1 ^ IV))									,for i=1
*												 = E(K,(Pi ^	Ci-1))   						,for i=2,3,...,n-1
*											 	 = E(K,((Pn||0[128-u])^Cn-1))			,for i=n
*									PiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n-1)*128+u bits£¨1<= u <=128
*									CiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n)*128 bits
*									IVŒ™≥ı ºœÚ¡ø£¨0~128 bits
*									E(K,P)Œ™∂‘PΩ¯––AESº”√‹‘ÀÀ„
*									Pn||0[128-u]£¨Œ™P◊Ó∫Û∑÷∂Œ≤ª◊„128bits≤π¡„£¨uŒ™◊Ó∫Û∑÷∂ŒŒª≥§
*
* »Îø⁄≤Œ ˝: CBC_InputStruct    CBCº”√‹ ‰»Î±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨4∏ˆ≤Œ ˝: 
*                              1) AESKeyMode     		: º”√‹Ω‚√‹ƒ£ Ω—°‘Ò
*                              2) *pKeyAddr       	: √‹‘øµÿ÷∑÷∏’Î
*                              3) *pPtextAddr  			: √˜Œƒ ˝æ›µÿ÷∑÷∏’Î
*                              4) LenPtext					: √˜Œƒ ˝æ›≥§∂»£®byte£©
*                              5) *pInitVectorAddr  : ≥ı ºœÚ¡øµÿ÷∑÷∏’Î
*						CBC_OutputStruct   CBCº”√‹ ‰≥ˆ±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨2∏ˆ≤Œ ˝: 
*                              1) *pCtextAddr     	: “—º”√‹ ˝æ›√‹Œƒµÿ÷∑÷∏’Î
*                              2) LenCtext       		: “—º”√‹ ˝æ›√‹Œƒ≥§∂»£®byte£©
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: 
*********************************************************************************************************
*/
void CBC_Encrypt(CBC_Encrypt_InputTypedef* CBC_InputStruct,CBC_Encrypt_OutputTypedef* CBC_OutputStruct)
{
		uint8_t i; 
		Parameter128bits_TypeDef	Aes_CBC_temp;
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);						/*¥Úø™AESƒ£øÈ ±÷”						*/
	
		if(CBC_InputStruct->LenPtext%16 == 0)														/*!< º∆À„√‹Œƒ≥§∂»						*/
		{
				CBC_OutputStruct->LenCtext = CBC_InputStruct->LenPtext;
		}
		else
		{
				CBC_OutputStruct->LenCtext = CBC_InputStruct->LenPtext - (CBC_InputStruct->LenPtext%16) +16;
		}
		
/* LenPtext>=16	*/
		if(CBC_InputStruct->LenPtext>=16)																/*!< PŒª≥§≈–∂œ	16bytes£®128bits£©	*/
		{
	//Ci = E(K,(P1 ^ IV))									,for i=1		
				for(i=0;i<16;i++)
				{
						Aes_CBC_temp.u8[i] = *CBC_InputStruct->pPtextAddr++ ^ CBC_InputStruct->pInitVectorAddr->u8[i];		
																																		/*!< (P1 ^ IV)‘ÀÀ„£¨Ω·π˚¥Ê”⁄Aes_CBC_temp			*/
				}
				HT_AES_Encrypt(CBC_InputStruct->AESKeyMode,CBC_InputStruct->pKeyAddr,Aes_CBC_temp.u32);
																																		/*!< ∂‘(P1 ^ IV) AESº”√‹≤Ÿ◊˜£¨E(K,(P1 ^ IV))	*/
				while(HT_AES_Busy_StatusGet());															/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_CBC_temp.u32);															/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_CBC_temp	*/
				for(i=0;i<16;i++)
				{
						*CBC_OutputStruct->pCtextAddr++ = Aes_CBC_temp.u8[i];	
																																		/*!< Ω´√‹ŒƒC1±£¥Ê										*/
				}
				CBC_InputStruct->LenPtext -= 16;														/*!< œ¬∏ˆ128bit∑÷∂Œ									*/
				CBC_OutputStruct->pCtextAddr -= 16;													/*!< ÷∏’Î÷∏œÚC1											*/	

	//Ci =  E(K,(Pi ^	Ci-1))  				,for i=2,3,...,n-1
				while(CBC_InputStruct->LenPtext>=16)												/*!< PŒª≥§≈–∂œ	16bytes£®128bits£©	*/
				{
						for(i=0;i<16;i++)
						{
								Aes_CBC_temp.u8[i] = *CBC_InputStruct->pPtextAddr++ ^ *CBC_OutputStruct->pCtextAddr++;		
																																		/*!< (Pi ^ Ci-1)‘ÀÀ„£¨Ω·π˚¥Ê”⁄Aes_CBC_temp			*/
						}
						HT_AES_Encrypt(CBC_InputStruct->AESKeyMode,CBC_InputStruct->pKeyAddr,Aes_CBC_temp.u32);
																																		/*!< ∂‘(Pi ^ Ci-1) AESº”√‹≤Ÿ◊˜£¨E(K,(Pi ^ Ci-1))	*/
						while(HT_AES_Busy_StatusGet());													/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
						HT_AES_Read(Aes_CBC_temp.u32);													/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_CBC_temp	*/
						for(i=0;i<16;i++)
						{
								*CBC_OutputStruct->pCtextAddr++ = Aes_CBC_temp.u8[i];	
																																		/*!< Ω´√‹Œƒ±£¥Ê											*/
						}
						CBC_InputStruct->LenPtext -= 16;												/*!< œ¬∏ˆ128bit∑÷∂Œ									*/	
						CBC_OutputStruct->pCtextAddr-=16;												/*!< ÷∏’Î÷∏œÚCi-1										*/
				}
	//E(K,((Pn||0[128-u])^Cn-1))			,for i=n
				if(CBC_InputStruct->LenPtext)
				{
						for(i=0;i<4;i++)
						{
								Aes_CBC_temp.u32[i] = 0x0000;														/*!< Aes_CBC_temp«Â¡„								*/
						}	
						for(i=16-CBC_InputStruct->LenPtext;i<16;i++)
						{
								Aes_CBC_temp.u8[i] = *CBC_InputStruct->pPtextAddr++;		/*!< Pi¥Ê»Î¡Ÿ ±±‰¡øAes_CBC_temp£¨≤π¡„	*/
						}		
						for(i=0;i<16;i++)
						{
								Aes_CBC_temp.u8[i] = Aes_CBC_temp.u8[i] ^ *CBC_OutputStruct->pCtextAddr++;		
																																				/*!< ((Pn||0[128-u])^Cn-1)‘ÀÀ„£¨Ω·π˚¥Ê”⁄Aes_CBC_temp			*/
						}
						HT_AES_Encrypt(CBC_InputStruct->AESKeyMode,CBC_InputStruct->pKeyAddr,Aes_CBC_temp.u32);
																																				/*!< ∂‘((Pn||0[128-u])^Ci-1) AESº”√‹≤Ÿ◊˜	£¨ E(K,Pi) 		*/
						while(HT_AES_Busy_StatusGet());															/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
						HT_AES_Read(Aes_CBC_temp.u32);															/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_CBC_temp		*/
						for(i=0;i<16;i++)
						{
								*CBC_OutputStruct->pCtextAddr++ = Aes_CBC_temp.u8[i];	
						}																														/*!< Ω´√‹Œƒ±£¥Ê											*/
				}	
		}
/* LenPtext<16 «“!=0	*/
		else if(CBC_InputStruct->LenPtext)
		{
	//Ci = E(K,((P1||0[128-U]) ^ IV))									,for i=n=1	
				for(i=0;i<4;i++)
				{
						Aes_CBC_temp.u32[i] = 0x0000;														/*!< Aes_CBC_temp«Â¡„								*/
				}	
				for(i=16-CBC_InputStruct->LenPtext;i<16;i++)
				{
						Aes_CBC_temp.u8[i] = *CBC_InputStruct->pPtextAddr++;		/*!< Pi¥Ê»Î¡Ÿ ±±‰¡øAes_CBC_temp,≤π¡„*/
				}		
				for(i=0;i<16;i++)
				{
						Aes_CBC_temp.u8[i] = Aes_CBC_temp.u8[i] ^ CBC_InputStruct->pInitVectorAddr->u8[i];		
																																		/*!< (P1 ^ IV)‘ÀÀ„£¨Ω·π˚¥Ê”⁄Aes_CBC_temp			*/
				}
				HT_AES_Encrypt(CBC_InputStruct->AESKeyMode,CBC_InputStruct->pKeyAddr,Aes_CBC_temp.u32);
																																		/*!< ∂‘(P1 ^ IV)AESº”√‹≤Ÿ◊˜	£¨ E(K,(P1 ^ IV)) 		*/
				while(HT_AES_Busy_StatusGet());															/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_CBC_temp.u32);															/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_CBC_temp		*/
				for(i=0;i<16;i++)
				{
						*CBC_OutputStruct->pCtextAddr++ = Aes_CBC_temp.u8[i];	
				}																														/*!< Ω´√‹Œƒ±£¥Ê											*/
		}
		
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE);					/*πÿ±’AESƒ£øÈ ±÷”										*/
}	

/*
*********************************************************************************************************
*                                CBC Ω‚√‹º∆À„
*
* ∫Ø ˝Àµ√˜: CBC Ω‚√‹º∆À„
*
*						º”√‹À„∑®£∫Pi = E'(K,C1) ^ IV									,for i=1
*												 = E'(K,Ci) ^	Ci-1								,for i=2,3,...,n
*									PiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n)*128 bits
*									CiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n)*128 bits
*									IVŒ™≥ı ºœÚ¡ø£¨0~128 bits
*									E'(K,C)Œ™∂‘CΩ¯––AESΩ‚√‹‘ÀÀ„
*
* »Îø⁄≤Œ ˝: CBC_InputStruct    CBCΩ‚√‹ ‰»Î±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨4∏ˆ≤Œ ˝: 
*                              1) AESKeyMode     		: º”√‹Ω‚√‹ƒ£ Ω—°‘Ò
*                              2) *pKeyAddr       	: √‹‘øµÿ÷∑÷∏’Î
*                              3) *pCtextAddr  			: √‹Œƒ ˝æ›µÿ÷∑÷∏’Î
*                              4) LenCtext					: √‹Œƒ ˝æ›≥§∂»£®byte£©
*                              5) *pInitVectorAddr  : ≥ı ºœÚ¡øµÿ÷∑÷∏’Î
*						CBC_OutputStruct   CBCΩ‚√‹ ‰≥ˆ±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨2∏ˆ≤Œ ˝: 
*                              1) *pPtextAddr     	: “—Ω‚√‹√˜Œƒµÿ÷∑÷∏’Î
*                              2) LenPtext       		: “—Ω‚√‹√˜Œƒ≥§∂»£®byte£©
*
* ∑µªÿ≤Œ ˝: Bool    = TRUE£∫  Ω‚√‹≥…π¶
*                   = FALSE£∫	Ω‚√‹ ß∞‹£¨√‹Œƒ”–ŒÛ   
* Ãÿ ‚Àµ√˜: 
*********************************************************************************************************
*/
Bool CBC_Decrypt(CBC_Decrypt_InputTypedef* CBC_InputStruct,CBC_Decrypt_OutputTypedef* CBC_OutputStruct)
{
		uint8_t i; 
		Parameter128bits_TypeDef	Aes_CBC_temp;
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);						/*¥Úø™AESƒ£øÈ ±÷”						*/
	
		if(CBC_InputStruct->LenCtext%16 != 0 || CBC_InputStruct->LenCtext == 0)
		{
				return	FALSE;																							/*!< √‹ŒƒŒª≥§¥ÌŒÛ										*/
		}
		CBC_OutputStruct->LenPtext = CBC_InputStruct->LenCtext;					/*!< LenPtext = LenCtext						*/
		
	//Pi = E'(K,C1) ^ IV									,for i=1
		for(i=0;i<16;i++)
		{
				Aes_CBC_temp.u8[i] = *CBC_InputStruct->pCtextAddr++;				/*!< C1¥Ê»Î¡Ÿ ±±‰¡øAes_CBC_temp			*/
		}
		HT_AES_Decrypt(CBC_InputStruct->AESKeyMode,CBC_InputStruct->pKeyAddr,Aes_CBC_temp.u32);
																																		/*!< ∂‘C1 AESΩ‚√‹≤Ÿ◊˜£¨E'(K,C1)			*/
		while(HT_AES_Busy_StatusGet());																	/*!< AESΩ‚√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
		HT_AES_Read(Aes_CBC_temp.u32);																	/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_CBC_temp	*/
		for(i=0;i<16;i++)
		{
				*CBC_OutputStruct->pPtextAddr++ = Aes_CBC_temp.u8[i] ^ CBC_InputStruct->pInitVectorAddr->u8[i];	
																																		/*!< Ω´√˜ŒƒP1±£¥Ê										*/
		}
		CBC_InputStruct->LenCtext -= 16;																/*!< œ¬∏ˆ128bit∑÷∂Œ									*/

	//Pi = E'(K,Ci) ^	Ci-1								,for i=2,3,...,n
		while(CBC_InputStruct->LenCtext>=16)														/*!< PŒª≥§≈–∂œ	16bytes£®128bits£©	*/
		{
				for(i=0;i<16;i++)
				{
						Aes_CBC_temp.u8[i] = *CBC_InputStruct->pCtextAddr++;		/*!< Ci¥Ê»Î¡Ÿ ±±‰¡øAes_CBC_temp			*/
				}
				CBC_InputStruct->pCtextAddr -= 32;														/*!< ÷∏’Î÷∏œÚCi-1										*/
				HT_AES_Decrypt(CBC_InputStruct->AESKeyMode,CBC_InputStruct->pKeyAddr,Aes_CBC_temp.u32);
																																		/*!< ∂‘Ci AESΩ‚√‹≤Ÿ◊˜£¨E'(K,Ci)			*/
				while(HT_AES_Busy_StatusGet());															/*!< AESΩ‚√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_CBC_temp.u32);															/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_CBC_temp	*/
				for(i=0;i<16;i++)
				{
						*CBC_OutputStruct->pPtextAddr++ = Aes_CBC_temp.u8[i] ^ *CBC_InputStruct->pCtextAddr++;	
																																		/*!< Ω´√˜ŒƒP1±£¥Ê,Pi = E'(K,Ci) ^	Ci-1	*/
				}
				CBC_InputStruct->pCtextAddr += 16;													/*!< ÷∏’Î÷∏œÚCi+1										*/
				CBC_InputStruct->LenCtext -= 16;														/*!< œ¬∏ˆ128bit∑÷∂Œ									*/
		}
		
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE);					/*πÿ±’AESƒ£øÈ ±÷”										*/
		
		return TRUE;																										/*!< Ω‚√‹≥…π¶												*/
}	

/*
*********************************************************************************************************
*                                CTRº”√‹/Ω‚√‹º∆À„
*
* ∫Ø ˝Àµ√˜: CTRº”√‹/Ω‚√‹º∆À„
*						º”√‹À„∑®£∫Ci = Pi ^ E(K,Yi)   				,for i=1,2,...,n-1
*											 		 Pn ^ MSBu(E(K,Yn))			,for i=n
*						Ω‚√‹À„∑®£∫Pi = Ci ^ E(K,Yi)   				,for i=1,2,...,n-1
*											 		 Cn ^ MSBu(E(K,Yn))			,for i=n
*									PiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n-1)*128+u bits£¨1<= u <=128
*									CiŒ™√˜Œƒ∑÷∂Œ£¨√ø∂Œ128 bits£¨◊Ó∫Û∂Œu bits£¨◊‹Œª≥§ (n-1)*128+u bits£¨1<= u <=128
*									E(K,Yi)Œ™∂‘YiΩ¯––AESº”√‹‘ÀÀ„
*									MSBu(S)Œ™∑µªÿ1◊÷∑˚¥Æ£¨◊÷∑˚¥Æ∞¸∫¨S ˝æ›µƒ◊Ó◊Û±ﬂu∏ˆbits
*
* »Îø⁄≤Œ ˝: CTR_InputStruct    CTRº”√‹ ‰»Î±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨5∏ˆ≤Œ ˝: 
*                              1) AESKeyMode     		: º”√‹Ω‚√‹ƒ£ Ω—°‘Ò
*                              2) *pKeyAddr       	: √‹‘øµÿ÷∑÷∏’Î
*                              3) *pInttextAddr  		: ¥˝º”Ω‚√‹ ˝æ›µÿ÷∑÷∏’Î
*                              4) LenInttext				: ¥˝º”Ω‚√‹ ˝æ›≥§∂»£®byte£©
*                              5) *Counter0  				: Counter0µÿ÷∑÷∏’Î
*						CTR_OutputStruct   CTRº”√‹ ‰≥ˆ±‰¡øΩ·ππÃÂ÷∏’Î£¨÷˜“™∞¸∫¨2∏ˆ≤Œ ˝: 
*                              1) *pOuttextAddr     : “—º”/Ω‚√‹ ˝æ›µÿ÷∑÷∏’Î
*                              2) LenOuttext       	: “—º”/Ω‚√‹ ˝æ›≥§∂»£®byte£©
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: º”√‹≤Ÿ◊˜ ±£¨pInttextAddr∫ÕLenInttextŒ™√˜Œƒ ‰»Î£¨pOuttextAddrŒ™√‹Œƒ ‰≥ˆ
*						Ω‚√‹≤Ÿ◊˜ ±£¨pInttextAddr∫ÕLenInttextŒ™√‹Œƒ ‰»Î£¨pOuttextAddrŒ™√˜Œƒ ‰≥ˆ
*********************************************************************************************************
*/
void CTR_Encrypt(CTR_Encrypt_InputTypedef* CTR_InputStruct,CTR_Encrypt_OutputTypedef* CTR_OutputStruct)
{
		uint8_t i; 
		Parameter128bits_TypeDef	Counter_temp,Aes_GCM_temp;
	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);						/*¥Úø™AESƒ£øÈ ±÷”						*/
	
		CTR_OutputStruct->LenOuttext = CTR_InputStruct->LenInttext;
																															/*!< LenOuttext = LenInttext				*/
		for(i=0;i<4;i++)
		{
				Counter_temp.u32[i] = CTR_InputStruct->Counter0->u32[i]; 
																															/*!< counter0¥ÊµΩCounter_temp±∏”√		*/
		}
	//Ci = Pi ^ E(K,Yi)   				,for i=1,2,...,n-1
		while(CTR_InputStruct->LenInttext>=16)										/*!< PŒª≥§≈–∂œ	16bytes£®128bits£©	*/
		{
				GCM_Counter_increase(&Counter_temp);									/*!< GCM Counter(Yi)¿€º”º∆À„				*/
				HT_AES_Encrypt(CTR_InputStruct->AESKeyMode,CTR_InputStruct->pKeyAddr,Counter_temp.u32);
																															/*!< ∂‘Counter(Yi)AESº”√‹≤Ÿ◊˜	£¨ E(K,Yi) 			*/
				while(HT_AES_Busy_StatusGet());												/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_GCM_temp.u32);												/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_GCM_temp*/
				for(i=0;i<16;i++)
				{
						*CTR_OutputStruct->pOuttextAddr++ = Aes_GCM_temp.u8[i] ^ *CTR_InputStruct->pInttextAddr++;	
																															/*!< Pi ^ E(K,Yi)‘ÀÀ„								*/
				}
				CTR_InputStruct->LenInttext -= 16;										/*!< œ¬∏ˆ128bit∑÷∂Œ									*/			
		}
	//Cn = Pn ^ MSBu(E(K,Yn))			,for i=n
		if(CTR_InputStruct->LenInttext)
		{
				GCM_Counter_increase(&Counter_temp);									/*!< GCM Counter(Yi)¿€º”º∆À„				*/
				HT_AES_Encrypt(CTR_InputStruct->AESKeyMode,CTR_InputStruct->pKeyAddr,Counter_temp.u32);
																															/*!< ∂‘Counter(Yn)AESº”√‹≤Ÿ◊˜	£¨ E(K,Yn) 	*/
				while(HT_AES_Busy_StatusGet());												/*!< AESº”√‹‘ÀÀ„ÕÍ≥…≈–∂œ						*/
				HT_AES_Read(Aes_GCM_temp.u32);												/*!< ∂¡»°‘ÀÀ„Ω·π˚£¨¥Ê¥¢µΩ	Aes_GCM_temp1		*/
				for(i=16-CTR_InputStruct->LenInttext;i<16;i++)
				{
						*CTR_OutputStruct->pOuttextAddr++ = Aes_GCM_temp.u8[i] ^ *CTR_InputStruct->pInttextAddr++;							
																															/*!< Pn ^ MSBu(E(K,Yn))‘ÀÀ„					*/
				}
		}	
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE);		/*πÿ±’AESƒ£øÈ ±÷”						*/
}
#if 0
/*
*********************************************************************************************************
*                                GCM_AES EXAMPLES FOR HT6x2x/HT501x
*
* ∫Ø ˝Àµ√˜: ¿˚”√ø‚∫Ø ˝ µœ÷GCM_AESº”/Ω‚√‹
*
* »Îø⁄≤Œ ˝: Œﬁ                            
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
		
void AES_GCM_Example(uint8_t TestCase)
{   
		CMU_InitTypeDef  CMU_InitStructure;
		GCM_Encrypt_InputTypedef GCM_Encrypt_InputStruct;	
		GCM_Encrypt_OutputTypedef	GCM_Encrypt_OutputStruct;
		GCM_Decrypt_InputTypedef GCM_Decrypt_InputStruct;	
		GCM_Decrypt_OutputTypedef	GCM_Decrypt_OutputStruct;
		uint8_t i;
	
/* ‰»Î≤Œ ˝≥ı ºªØ*/
		GCM_TestCase_Init(TestCase);
/*…Ë÷√œµÕ≥ ±÷”*/    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv1;
    HT_CMU_Init(&CMU_InitStructure);
	

/*º”√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/		
		for(i=0;i<16;i++)
		{
				Ciphertext.u32[i] = 0x0000;
				AuthTag.u8[i] = 0x00;
		}
		GCM_Encrypt_InputStruct.AESKeyMode = AESKeyMode;
		GCM_Encrypt_InputStruct.pKeyAddr = AesKey.u32;
		GCM_Encrypt_InputStruct.pInitVectorAddr = InitVector.u8;
		GCM_Encrypt_InputStruct.LenIV = LenIV;
		GCM_Encrypt_InputStruct.pPtextAddr = Plaintext.u8;
		GCM_Encrypt_InputStruct.LenPtext = LenPtext;
		GCM_Encrypt_InputStruct.pAdataAddr = AddAuthData.u8;
		GCM_Encrypt_InputStruct.LenAdata = LenAdata;
		GCM_Encrypt_OutputStruct.AuthTag = &AuthTag;
		GCM_Encrypt_OutputStruct.pCtextAddr = Ciphertext.u8;
		GCM_Encrypt(0,&GCM_Encrypt_InputStruct,&GCM_Encrypt_OutputStruct);										/*!< gcm º”√‹	*/
		
/*Ω‚√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/	
		for(i=0;i<16;i++)
		{
				Plaintext.u32[i] = 0x0000;
		}
		GCM_Decrypt_InputStruct.AESKeyMode = AESKeyMode;
		GCM_Decrypt_InputStruct.pKeyAddr = AesKey.u32;
		GCM_Decrypt_InputStruct.pInitVectorAddr = InitVector.u8;
		GCM_Decrypt_InputStruct.LenIV = LenIV;
		GCM_Decrypt_InputStruct.pCtextAddr = GCM_Encrypt_OutputStruct.pCtextAddr;
		GCM_Decrypt_InputStruct.LenCtext = GCM_Encrypt_OutputStruct.LenCtext;
		GCM_Decrypt_InputStruct.pAdataAddr = AddAuthData.u8;
		GCM_Decrypt_InputStruct.LenAdata = LenAdata;
		GCM_Decrypt_InputStruct.AuthTag = &AuthTag;
		GCM_Decrypt_OutputStruct.pPtextAddr = Plaintext.u8;
		DecryptSta = GCM_Decrypt(0,&GCM_Decrypt_InputStruct, &GCM_Decrypt_OutputStruct);			/*!< gcm Ω‚√‹	*/
} 

/*
*********************************************************************************************************
*                                CTR_AES EXAMPLES FOR HT6x2x/HT501x
*
* ∫Ø ˝Àµ√˜: ¿˚”√ø‚∫Ø ˝ µœ÷CTR_AESº”/Ω‚√‹
*
* »Îø⁄≤Œ ˝: Œﬁ                            
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
void AES_CTR_Example(void)
{   
		CMU_InitTypeDef  CMU_InitStructure;
		CTR_Encrypt_InputTypedef CTR_Encrypt_InputStruct;	
		CTR_Encrypt_OutputTypedef	CTR_Encrypt_OutputStruct;
		uint8_t i;
	
/* ‰»Î≤Œ ˝≥ı ºªØ*/
		CTR_TestCase_Init();
/*…Ë÷√œµÕ≥ ±÷”*/    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv1;
    HT_CMU_Init(&CMU_InitStructure);

/*º”√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/		
		for(i=0;i<16;i++)
		{
				Ciphertext.u32[i] = 0x0000;
		}
		CTR_Encrypt_InputStruct.AESKeyMode = AESKeyMode;
		CTR_Encrypt_InputStruct.pKeyAddr = AesKey.u32;
		CTR_Encrypt_InputStruct.pInttextAddr = Plaintext.u8;
		CTR_Encrypt_InputStruct.LenInttext= LenPtext;
		CTR_Encrypt_InputStruct.Counter0 = &CTR_Counter0;
		CTR_Encrypt_OutputStruct.pOuttextAddr = Ciphertext.u8;
		CTR_Encrypt(&CTR_Encrypt_InputStruct,&CTR_Encrypt_OutputStruct);										/*!< CTR º”√‹	*/
		
/*Ω‚√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/
		LenCtext = CTR_Encrypt_OutputStruct.LenOuttext;
		for(i=0;i<16;i++)
		{
				Plaintext.u32[i] = 0x0000;
		}
		CTR_Encrypt_InputStruct.AESKeyMode = AESKeyMode;
		CTR_Encrypt_InputStruct.pKeyAddr = AesKey.u32;
		CTR_Encrypt_InputStruct.pInttextAddr = Ciphertext.u8;
		CTR_Encrypt_InputStruct.LenInttext= LenCtext;
		CTR_Encrypt_InputStruct.Counter0 = &CTR_Counter0;
		CTR_Encrypt_OutputStruct.pOuttextAddr = Plaintext.u8;
		CTR_Encrypt(&CTR_Encrypt_InputStruct,&CTR_Encrypt_OutputStruct);										/*!< CTR Ω‚√‹	*/
} 
/*
*********************************************************************************************************
*                                CBC_AES EXAMPLES FOR HT6x2x/HT501x
*
* ∫Ø ˝Àµ√˜: ¿˚”√ø‚∫Ø ˝ µœ÷CBC_AESº”/Ω‚√‹
*
* »Îø⁄≤Œ ˝: Œﬁ                            
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
void AES_CBC_Example(void)
{   
		CMU_InitTypeDef  CMU_InitStructure;
		CBC_Encrypt_InputTypedef CBC_Encrypt_InputStruct;	
		CBC_Encrypt_OutputTypedef	CBC_Encrypt_OutputStruct;
		CBC_Decrypt_InputTypedef CBC_Decrypt_InputStruct;	
		CBC_Decrypt_OutputTypedef	CBC_Decrypt_OutputStruct;
		uint8_t i;
	
/* ‰»Î≤Œ ˝≥ı ºªØ*/
		CBC_TestCase_Init();
/*…Ë÷√œµÕ≥ ±÷”*/    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv1;
    HT_CMU_Init(&CMU_InitStructure);

/*º”√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/		
		for(i=0;i<16;i++)
		{
				Ciphertext.u32[i] = 0x0000;
		}
		CBC_Encrypt_InputStruct.AESKeyMode = AESKeyMode;
		CBC_Encrypt_InputStruct.pKeyAddr = AesKey.u32;
		CBC_Encrypt_InputStruct.pPtextAddr = Plaintext.u8;
		CBC_Encrypt_InputStruct.LenPtext= LenPtext;
		CBC_Encrypt_InputStruct.pInitVectorAddr = &CTR_Counter0;
		CBC_Encrypt_OutputStruct.pCtextAddr = Ciphertext.u8;
		CBC_Encrypt(&CBC_Encrypt_InputStruct,&CBC_Encrypt_OutputStruct);										/*!< CBC º”√‹	*/
		
/*Ω‚√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/
		LenCtext = CBC_Encrypt_OutputStruct.LenCtext;
		for(i=0;i<16;i++)
		{
				Plaintext.u32[i] = 0x0000;
		}
		CBC_Decrypt_InputStruct.AESKeyMode = AESKeyMode;
		CBC_Decrypt_InputStruct.pKeyAddr = AesKey.u32;
		CBC_Decrypt_InputStruct.pCtextAddr = Ciphertext.u8;
		CBC_Decrypt_InputStruct.LenCtext= LenCtext;
		CBC_Decrypt_InputStruct.pInitVectorAddr= &CTR_Counter0;
		CBC_Decrypt_OutputStruct.pPtextAddr = Plaintext.u8;
		DecryptSta = CBC_Decrypt(&CBC_Decrypt_InputStruct,&CBC_Decrypt_OutputStruct);										/*!< CBC Ω‚√‹	*/
} 

/*
*********************************************************************************************************
*                                ECB_AES EXAMPLES FOR HT6x2x/HT501x
*
* ∫Ø ˝Àµ√˜: ¿˚”√ø‚∫Ø ˝ µœ÷ECB_AESº”/Ω‚√‹
*
* »Îø⁄≤Œ ˝: Œﬁ                            
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
void AES_ECB_Example(void)
{   
		CMU_InitTypeDef  CMU_InitStructure;
		ECB_Encrypt_InputTypedef ECB_Encrypt_InputStruct;	
		ECB_Encrypt_OutputTypedef	ECB_Encrypt_OutputStruct;
		ECB_Decrypt_InputTypedef ECB_Decrypt_InputStruct;	
		ECB_Decrypt_OutputTypedef	ECB_Decrypt_OutputStruct;
		uint8_t i;
	
/* ‰»Î≤Œ ˝≥ı ºªØ*/
		ECB_TestCase_Init();
/*…Ë÷√œµÕ≥ ±÷”*/    
    CMU_InitStructure.SysClkSel = SysPLL;
    CMU_InitStructure.CPUDiv = CPUDiv1;
    HT_CMU_Init(&CMU_InitStructure);

/*º”√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/		
		for(i=0;i<16;i++)
		{
				Ciphertext.u32[i] = 0x0000;
		}
		ECB_Encrypt_InputStruct.AESKeyMode = AESKeyMode;
		ECB_Encrypt_InputStruct.pKeyAddr = AesKey.u32;
		ECB_Encrypt_InputStruct.pPtextAddr = Plaintext.u8;
		ECB_Encrypt_InputStruct.LenPtext= LenPtext;
		ECB_Encrypt_OutputStruct.pCtextAddr = Ciphertext.u8;
		ECB_Encrypt(&ECB_Encrypt_InputStruct,&ECB_Encrypt_OutputStruct);										/*!< ECB º”√‹	*/
		
/*Ω‚√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/
		LenCtext = ECB_Encrypt_OutputStruct.LenCtext;
		for(i=0;i<16;i++)
		{
				Plaintext.u32[i] = 0x0000;
		}
		ECB_Decrypt_InputStruct.AESKeyMode = AESKeyMode;
		ECB_Decrypt_InputStruct.pKeyAddr = AesKey.u32;
		ECB_Decrypt_InputStruct.pCtextAddr = Ciphertext.u8;
		ECB_Decrypt_InputStruct.LenCtext= LenCtext;
		ECB_Decrypt_OutputStruct.pPtextAddr = Plaintext.u8;
		DecryptSta = ECB_Decrypt(&ECB_Decrypt_InputStruct,&ECB_Decrypt_OutputStruct);										/*!< ECB Ω‚√‹	*/
} 

/*
*********************************************************************************************************
*                                TEST CASE INITIAL
*
* ∫Ø ˝Àµ√˜: ∏˜º”√‹À„∑®—È÷§testcase≥ı ºªØ
*
* »Îø⁄≤Œ ˝: Œﬁ                            
*
* ∑µªÿ≤Œ ˝: Œﬁ                                      
* 
* Ãÿ ‚Àµ√˜: ¥À∫Ø ˝ƒ¨»œ≤ª≤Œ”Îπ§≥Ã±‡“Î
*********************************************************************************************************
*/
void GCM_TestCase_Init(uint8_t TestCase)
{
		switch(TestCase)
		{		
		case 1:
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0]		= 	0x00000000;
				AesKey.u32[1]		= 	0x00000000;
				AesKey.u32[2]		= 	0x00000000;
				AesKey.u32[3]		= 	0x00000000;
				
				LenPtext	=  0;
				
				LenIV = 12;
				InitVector.u32[0] = 0x00000000;
				InitVector.u32[1] = 0x00000000;
				InitVector.u32[2] = 0x00000000;
				
				LenAdata = 0;
				
				break;
		
		case 2:
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0]		= 	0x00000000;
				AesKey.u32[1]		= 	0x00000000;
				AesKey.u32[2]		= 	0x00000000;
				AesKey.u32[3]		= 	0x00000000;
				
				LenPtext	=  16;
				Plaintext.u32[0]		= 	0x00000000;
				Plaintext.u32[1]		= 	0x00000000;
				Plaintext.u32[2]		= 	0x00000000;
				Plaintext.u32[3]		= 	0x00000000;
				
				LenIV = 12;
				InitVector.u32[0] = 0x00000000;
				InitVector.u32[1] = 0x00000000;
				InitVector.u32[2] = 0x00000000;
						
				LenAdata = 0;	
				break;

		case 3:
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;
				
				LenPtext	=  16*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[13] 	= 	0xba637b39;
				Plaintext.u32[14] 	= 	0xaa0de657;
				Plaintext.u32[15] 	= 	0xb16aedf5;
				
				LenIV = 12;
				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[1] = 0xfacedbad;
				InitVector.u32[2] = 0xcafebabe;
						
				LenAdata = 0;	
				break;
				
		case 4:
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;
				
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				
				LenIV = 3*4;
				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[1] = 0xfacedbad;
				InitVector.u32[2] = 0xcafebabe;
				break;
				
		case 5:
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;
				
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				LenIV = 2*4;
//				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[0] = 0xfacedbad;
				InitVector.u32[1] = 0xcafebabe;
				break;

		case 6:
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;
				
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				LenIV = 15*4;
				InitVector.u32[0] = 0xff5269aa;
				InitVector.u32[1] = 0x55909c5a;
				InitVector.u32[2] = 0xf88406e5;
				InitVector.u32[3] = 0x9313225d;
				InitVector.u32[4] = 0xa318a728;
				InitVector.u32[5] = 0xe4c303d2;
				InitVector.u32[6] = 0x534f7da1;
				InitVector.u32[7] = 0x6a7a9538;
				InitVector.u32[8] = 0x9a6b5254;
				InitVector.u32[9] = 0xfcf0e242;
				InitVector.u32[10] = 0x56809539;
				InitVector.u32[11] = 0xc3c0c951;
				InitVector.u32[12] = 0xa637b39b;
				InitVector.u32[13] = 0xa0de6a57;
				InitVector.u32[14] = 0x16aedbf5;
				break;

		case 7:
				AESKeyMode = KEY_Mode_192bits;
				AesKey.u32[0]		= 	0x00000000;
				AesKey.u32[1]		= 	0x00000000;
				AesKey.u32[2]		= 	0x00000000;
				AesKey.u32[3]		= 	0x00000000;
				AesKey.u32[4]		= 	0x00000000;
				AesKey.u32[5]		= 	0x00000000;		
				
				LenPtext	=  0;
				
				LenIV = 12;
				InitVector.u32[0] = 0x00000000;
				InitVector.u32[1] = 0x00000000;
				InitVector.u32[2] = 0x00000000;
				
				LenAdata = 0;
				break;

		case 8:
				AESKeyMode = KEY_Mode_192bits;
				AesKey.u32[0]		= 	0x00000000;
				AesKey.u32[1]		= 	0x00000000;
				AesKey.u32[2]		= 	0x00000000;
				AesKey.u32[3]		= 	0x00000000;
				AesKey.u32[4]		= 	0x00000000;
				AesKey.u32[5]		= 	0x00000000;	
				
				LenPtext	=  16;
				Plaintext.u32[0]		= 	0x00000000;
				Plaintext.u32[1]		= 	0x00000000;
				Plaintext.u32[2]		= 	0x00000000;
				Plaintext.u32[3]		= 	0x00000000;
				
				LenIV = 12;
				InitVector.u32[0] = 0x00000000;
				InitVector.u32[1] = 0x00000000;
				InitVector.u32[2] = 0x00000000;
						
				LenAdata = 0;	
				break;	

	case 9:
				AESKeyMode = KEY_Mode_192bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
	
				LenPtext	=  16*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[13] 	= 	0xba637b39;
				Plaintext.u32[14] 	= 	0xaa0de657;
				Plaintext.u32[15] 	= 	0xb16aedf5;	
				
				LenAdata = 0;	
				
				LenIV = 3*4;
				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[1] = 0xfacedbad;
				InitVector.u32[2] = 0xcafebabe;				
				break;
	case 10:
				AESKeyMode = KEY_Mode_192bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
	
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				LenIV = 3*4;
				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[1] = 0xfacedbad;
				InitVector.u32[2] = 0xcafebabe;	
				break;
				
	case 11:
				AESKeyMode = KEY_Mode_192bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
	
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				LenIV = 2*4;
//				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[0] = 0xfacedbad;
				InitVector.u32[1] = 0xcafebabe;	
				break;
				
	case 12:
				AESKeyMode = KEY_Mode_192bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
	
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				LenIV = 15*4;
				InitVector.u32[0] = 0xff5269aa;
				InitVector.u32[1] = 0x55909c5a;
				InitVector.u32[2] = 0xf88406e5;
				InitVector.u32[3] = 0x9313225d;
				InitVector.u32[4] = 0xa318a728;
				InitVector.u32[5] = 0xe4c303d2;
				InitVector.u32[6] = 0x534f7da1;
				InitVector.u32[7] = 0x6a7a9538;
				InitVector.u32[8] = 0x9a6b5254;
				InitVector.u32[9] = 0xfcf0e242;
				InitVector.u32[10] = 0x56809539;
				InitVector.u32[11] = 0xc3c0c951;
				InitVector.u32[12] = 0xa637b39b;
				InitVector.u32[13] = 0xa0de6a57;
				InitVector.u32[14] = 0x16aedbf5;
				break;		

		case 13:
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0x00000000;
				AesKey.u32[1]		= 	0x00000000;
				AesKey.u32[2]		= 	0x00000000;
				AesKey.u32[3]		= 	0x00000000;
				AesKey.u32[4]		= 	0x00000000;
				AesKey.u32[5]		= 	0x00000000;		
				AesKey.u32[6]		= 	0x00000000;
				AesKey.u32[7]		= 	0x00000000;	
				
				LenPtext	=  0;
				
				LenIV = 12;
				InitVector.u32[0] = 0x00000000;
				InitVector.u32[1] = 0x00000000;
				InitVector.u32[2] = 0x00000000;
				
				LenAdata = 0;
				break;

		case 14:
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0x00000000;
				AesKey.u32[1]		= 	0x00000000;
				AesKey.u32[2]		= 	0x00000000;
				AesKey.u32[3]		= 	0x00000000;
				AesKey.u32[4]		= 	0x00000000;
				AesKey.u32[5]		= 	0x00000000;		
				AesKey.u32[6]		= 	0x00000000;
				AesKey.u32[7]		= 	0x00000000;	
				
				LenPtext	=  16;
				Plaintext.u32[0]		= 	0x00000000;
				Plaintext.u32[1]		= 	0x00000000;
				Plaintext.u32[2]		= 	0x00000000;
				Plaintext.u32[3]		= 	0x00000000;
				
				LenIV = 12;
				InitVector.u32[0] = 0x00000000;
				InitVector.u32[1] = 0x00000000;
				InitVector.u32[2] = 0x00000000;
						
				LenAdata = 0;	
				break;	

	case 15:
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
				AesKey.u32[6]		= 	0x6d6a8f94;
				AesKey.u32[7]		= 	0x67308308;	
	
				LenPtext	=  16*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[13] 	= 	0xba637b39;
				Plaintext.u32[14] 	= 	0xaa0de657;
				Plaintext.u32[15] 	= 	0xb16aedf5;	
				
				LenAdata = 0;	
				
				LenIV = 3*4;
				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[1] = 0xfacedbad;
				InitVector.u32[2] = 0xcafebabe;				
				break;
	case 16:
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
				AesKey.u32[6]		= 	0x6d6a8f94;
				AesKey.u32[7]		= 	0x67308308;
	
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				LenIV = 3*4;
				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[1] = 0xfacedbad;
				InitVector.u32[2] = 0xcafebabe;	
				break;
				
	case 17:
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
				AesKey.u32[6]		= 	0x6d6a8f94;
				AesKey.u32[7]		= 	0x67308308;
	
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				LenIV = 2*4;
//				InitVector.u32[0] = 0xdecaf888;
				InitVector.u32[0] = 0xfacedbad;
				InitVector.u32[1] = 0xcafebabe;	
				break;
				
	case 18:
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
				AesKey.u32[6]		= 	0x6d6a8f94;
				AesKey.u32[7]		= 	0x67308308;
	
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				LenAdata = 5*4;	
				AddAuthData.u32[0]		= 	0xdeadbeef;
				AddAuthData.u32[1]		= 	0xfeedface;
				AddAuthData.u32[2]		= 	0xdeadbeef;
				AddAuthData.u32[3]		= 	0xfeedface;
				AddAuthData.u32[4]		= 	0xabaddad2;
				
				LenIV = 15*4;
				InitVector.u32[0] = 0xff5269aa;
				InitVector.u32[1] = 0x55909c5a;
				InitVector.u32[2] = 0xf88406e5;
				InitVector.u32[3] = 0x9313225d;
				InitVector.u32[4] = 0xa318a728;
				InitVector.u32[5] = 0xe4c303d2;
				InitVector.u32[6] = 0x534f7da1;
				InitVector.u32[7] = 0x6a7a9538;
				InitVector.u32[8] = 0x9a6b5254;
				InitVector.u32[9] = 0xfcf0e242;
				InitVector.u32[10] = 0x56809539;
				InitVector.u32[11] = 0xc3c0c951;
				InitVector.u32[12] = 0xa637b39b;
				InitVector.u32[13] = 0xa0de6a57;
				InitVector.u32[14] = 0x16aedbf5;
				break;	
		default:
				break;
		}	
}

void CTR_TestCase_Init(void)
{
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
				AesKey.u32[6]		= 	0x6d6a8f94;
				AesKey.u32[7]		= 	0x67308308;
	
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				CTR_Counter0.u32[0] = 0x06bc8eb4;
				CTR_Counter0.u32[1] = 0x079f8e24;
				CTR_Counter0.u32[2] = 0x140a5976;
				CTR_Counter0.u32[3] = 0x0cd953e2;
}

void CBC_TestCase_Init(void)
{
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
				AesKey.u32[6]		= 	0x6d6a8f94;
				AesKey.u32[7]		= 	0x67308308;
	
				LenPtext	=  15*4;
				Plaintext.u32[0]		= 	0xaff5269a;
				Plaintext.u32[1]		= 	0xa55909c5;
				Plaintext.u32[2]		= 	0xf88406e5;
				Plaintext.u32[3]		= 	0xd9313225;
				Plaintext.u32[4]		= 	0x8a318a72;
				Plaintext.u32[5]		= 	0x2e4c303d;
				Plaintext.u32[6]		= 	0x1534f7da;
				Plaintext.u32[7]		= 	0x86a7a953;
				Plaintext.u32[8] 		= 	0x49a6b525;
				Plaintext.u32[9] 		= 	0x2fcf0e24;
				Plaintext.u32[10] 	= 	0x95680953;
				Plaintext.u32[11] 	= 	0x1c3c0c95;
//				Plaintext.u32[12] 	= 	0x1aafd255;
				Plaintext.u32[12] 	= 	0xba637b39;
				Plaintext.u32[13] 	= 	0xaa0de657;
				Plaintext.u32[14] 	= 	0xb16aedf5;
			
				CTR_Counter0.u32[0] = 0xca6b0cba;
				CTR_Counter0.u32[1] = 0x52047196;
				CTR_Counter0.u32[2] = 0xf41a5514;
				CTR_Counter0.u32[3] = 0x83bcdd0a;
}
void ECB_TestCase_Init(void)
{
				AESKeyMode = KEY_Mode_256bits;
				AesKey.u32[0]		= 	0xfeffe992;
				AesKey.u32[1]		= 	0x8665731c;
				AesKey.u32[2]		= 	0x6d6a8f94;
				AesKey.u32[3]		= 	0x67308308;		
				AesKey.u32[4] 	= 	0xfeffe992;
				AesKey.u32[5] 	= 	0x8665731c;
				AesKey.u32[6]		= 	0x6d6a8f94;
				AesKey.u32[7]		= 	0x67308308;
	
				LenPtext	=  16*4;
				Plaintext.u32[0] 		= 	0x06bc8eb4;
				Plaintext.u32[1] 		= 	0x079f8e24;
				Plaintext.u32[2] 		= 	0x140a5976;
				Plaintext.u32[3] 		= 	0x0cd953e2;
				Plaintext.u32[4] 		= 	0x06bc8eb5;
				Plaintext.u32[5] 		= 	0x079f8e24;
				Plaintext.u32[6] 		= 	0x140a5976;
				Plaintext.u32[7] 		= 	0x0cd953e2;
				Plaintext.u32[8] 		= 	0x06bc8eb6;
				Plaintext.u32[9] 		= 	0x079f8e24;
				Plaintext.u32[10] 		= 	0x140a5976;
				Plaintext.u32[11] 		= 	0x0cd953e2;
				Plaintext.u32[12] 		= 	0x06bc8eb7;
				Plaintext.u32[13] 		= 	0x079f8e24;
				Plaintext.u32[14] 		= 	0x140a5976;
				Plaintext.u32[15] 		= 	0x0cd953e2;
}
#endif
unsigned char AES_GCM_Parameter_Init(unsigned short COM_No,unsigned char* Ciphert,unsigned short *nCtextLen,unsigned char use_crypt)
{//128bitsŒ™“ª◊È
	unsigned char *key;	
	unsigned short tempLen;
	unsigned char buff[128];
	unsigned char buff1[16];
	unsigned char *ptr;
	unsigned char *ptr1;
	unsigned char LLC_len = 0,i;
	unsigned char ErrorNO = 3;
	unsigned char Key[16]	= {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};	//º”√‹√‹‘ø..
	unsigned char A_Key[16] = {0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf};	//»œ÷§√‹‘ø..
	unsigned char IV[12]	= {0x4D,0x4D,0x4D,0x00,0x00,0xBC,0x61,0x4E,0x01,0x23,0x45,0x67};						// /*≥ı ºªØœÚ¡ø*/..

	RAM_Write(crypt.EK,Key,16);
	RAM_Write(crypt.AK,A_Key,16); 
	RAM_Write(crypt.IV,IV,12); 
	 
	 ptr = buff;
	 ptr1 = buff1;
	if(use_crypt==0)
		{//Ω‚√‹
			tempLen = *(Ciphert+4);
			switch(crypt.type[COM_No])
			{
			case 0x10:
				if(*nCtextLen<7) return ErrorNO;
				
				key = crypt.EK;
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0] = GetBuffToLong(key);
				AesKey.u32[1] = GetBuffToLong(key+4);
				AesKey.u32[2] = GetBuffToLong(key+8);
				AesKey.u32[3] = GetBuffToLong(key+12);
				LenPtext = 0;
				
				LenIV = 12;
				key = crypt.IV;
				for(i=0;i<4;i++)
					InitVector.u32[i] = 0x00;
				InitVector.u32[0] = GetBuffToLong(key+8);
				InitVector.u32[1] = GetBuffToLong(key+4);
				InitVector.u32[2] = GetBuffToLong(key);

				LenAdata = 17+tempLen-12-5; 		
				*ptr = crypt.type[COM_No];
				RAM_Write(ptr+1, crypt.AK, 16);
				RAM_Write(ptr+17, Ciphert+5+5, tempLen-12-5);
				for(i=0;i<(LenAdata/16 + 1);i++)
					{
					RAM_Fill(ptr1,16);
					if(tempLen>=16)
						{
						RAM_Write(ptr1,ptr,16);
						tempLen-=16;
						ptr+=16;
						}
					else
						{
					    RAM_Write(ptr1+16-(tempLen%16),ptr,tempLen);
						tempLen=0;
						}
					AddAuthData.u32[4*i+0]		=	GetBuffToLong(ptr1+12);//0XBC000000;//
					AddAuthData.u32[4*i+1]		=	GetBuffToLong(ptr1+8);//0X6827C467;//
					AddAuthData.u32[4*i+2]		=	GetBuffToLong(ptr1+4);//0X935A4756;//
					AddAuthData.u32[4*i+3]		=	GetBuffToLong(ptr1);//0X411312FF;//
				}
			/*	AddAuthData.u32[0] = GetBuffToLong(ptr+12);//0xDBDCDDDE;//
				AddAuthData.u32[1] = GetBuffToLong(ptr+8);//0xD7D8D9DA;//
				AddAuthData.u32[2] = GetBuffToLong(ptr+4);//0xD3D4D5D6;//
				AddAuthData.u32[3] = GetBuffToLong(ptr);//0x30D0D1D2;//
				RAM_Fill(ptr1,16);
				RAM_Write(ptr1+16-(LenAdata-16),ptr+16,LenAdata-16);
				AddAuthData.u32[4] = GetBuffToLong(ptr1+12);//0x000000DF;//
				AddAuthData.u32[5] = GetBuffToLong(ptr1+8);
				AddAuthData.u32[6] = GetBuffToLong(ptr1+4);
				AddAuthData.u32[7] = GetBuffToLong(ptr1);
				*/
			    RAM_Fill(ptr1,16);
				RAM_Write(ptr1, (Ciphert+5+*(Ciphert+4)-12),12);
				AuthTag.u32[0] = GetBuffToLong(ptr1);
				AuthTag.u32[1] = GetBuffToLong(ptr1+4);
				AuthTag.u32[2] = GetBuffToLong(ptr1+8);

				break;
			case 0x20:
				if(*nCtextLen<7) return ErrorNO;
				
				key = crypt.EK;
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0] = GetBuffToLong(key);
				AesKey.u32[1] = GetBuffToLong(key+4);
				AesKey.u32[2] = GetBuffToLong(key+8);
				AesKey.u32[3] = GetBuffToLong(key+12);

				tempLen-=5;
				LenPtext = tempLen;	
				RAM_Fill(ptr,128);
			    RAM_Write(ptr, Ciphert+10, LenPtext);
			for(i=0;i<32;i++)
				Ciphertext.u32[i] = 0x00;
			for(i=0;i<(LenPtext/16 + 1);i++)
				{
				RAM_Fill(ptr1,16);
					if(tempLen>=16)
						{
						RAM_Write(ptr1,ptr,16);
						tempLen-=16;
						ptr+=16;
						}
					else
						{
					    RAM_Write(ptr1+16-(tempLen%16),ptr,tempLen);
						tempLen=0;
						}
					Ciphertext.u32[4*i+0]		=	GetBuffToLong(ptr1+12);//0XBC000000;//
					Ciphertext.u32[4*i+1]		=	GetBuffToLong(ptr1+8);//0X6827C467;//
					Ciphertext.u32[4*i+2]		=	GetBuffToLong(ptr1+4);//0X935A4756;//
					Ciphertext.u32[4*i+3]		=	GetBuffToLong(ptr1);//0X411312FF;//
					//ptr+=16;
				}
				LenIV = 12;
				key = crypt.IV;
				for(i=0;i<4;i++)
					InitVector.u32[i] = 0x00;
				InitVector.u32[0] = GetBuffToLong(key+8);
				InitVector.u32[1] = GetBuffToLong(key+4);
				InitVector.u32[2] = GetBuffToLong(key);
				LenAdata = 0; 		

				break;
			case 0x30:
				if(*nCtextLen<7) return ErrorNO;
				
				key = crypt.EK;
				AESKeyMode = KEY_Mode_128bits;
				AesKey.u32[0] = GetBuffToLong(key);
				AesKey.u32[1] = GetBuffToLong(key+4);
				AesKey.u32[2] = GetBuffToLong(key+8);
				AesKey.u32[3] = GetBuffToLong(key+12);
				tempLen-=(5+12);
				LenPtext = tempLen;
				
				RAM_Fill(ptr,128);
				RAM_Write(ptr, Ciphert+10, LenPtext);
				for(i=0;i<32;i++)
				   Ciphertext.u32[i] = 0x00;
			
			for(i=0;i<(LenPtext/16 + 1);i++)
				{
				RAM_Fill(ptr1,16);
					if(tempLen>=16)
						{
						RAM_Write(ptr1,ptr,16);
						tempLen-=16;
						ptr+=16;
						}
					else
						{
					    RAM_Write(ptr1+16-(tempLen%16),ptr,tempLen);
						tempLen=0;
						}
					Ciphertext.u32[4*i+0]		=	GetBuffToLong(ptr1+12);//0XBC000000;//
					Ciphertext.u32[4*i+1]		=	GetBuffToLong(ptr1+8);//0X6827C467;//
					Ciphertext.u32[4*i+2]		=	GetBuffToLong(ptr1+4);//0X935A4756;//
					Ciphertext.u32[4*i+3]		=	GetBuffToLong(ptr1);//0X411312FF;//
				}
				LenIV = 12;
				key = crypt.IV;
				for(i=0;i<4;i++)
					InitVector.u32[i] = 0x00;
				InitVector.u32[0] = GetBuffToLong(key+8);
				InitVector.u32[1] = GetBuffToLong(key+4);
				InitVector.u32[2] = GetBuffToLong(key);
				
				LenAdata = 17; 		
				*ptr = crypt.type[COM_No];
				RAM_Write(ptr+1, crypt.AK, 16);
				AddAuthData.u32[0] = GetBuffToLong(ptr+12);//0xDBDCDDDE;//
				AddAuthData.u32[1] = GetBuffToLong(ptr+8);//0xD7D8D9DA;//
				AddAuthData.u32[2] = GetBuffToLong(ptr+4);//0xD3D4D5D6;//
				AddAuthData.u32[3] = GetBuffToLong(ptr);//0x30D0D1D2;//
				AddAuthData.u32[4] = *(ptr+16);//GetBuffToLong(ptr+16);0x000000DF;//

				RAM_Write(ptr, (Ciphert+5+ *(Ciphert+4) -12),12);
				AuthTag.u32[0] = GetBuffToLong(ptr);
				AuthTag.u32[1] = GetBuffToLong(ptr+4);
				AuthTag.u32[2] = GetBuffToLong(ptr+8);

				break;
			default:
				break;
			}
		}
	else if(use_crypt==1)
		{//º”√‹
		tempLen = *nCtextLen-3;
		switch(crypt.type[COM_No])
					{
					case 0x10:
						
						key = crypt.EK;
						AESKeyMode = KEY_Mode_128bits;
						AesKey.u32[0] = GetBuffToLong(key);
						AesKey.u32[1] = GetBuffToLong(key+4);
						AesKey.u32[2] = GetBuffToLong(key+8);
						AesKey.u32[3] = GetBuffToLong(key+12);
						LenPtext = 0;
		
						LenIV = 12;
						key = crypt.IV;
						for(i=0;i<4;i++)
					      InitVector.u32[i] = 0x00;
						InitVector.u32[0] = GetBuffToLong(key+8);
						InitVector.u32[1] = GetBuffToLong(key+4);
						InitVector.u32[2] = GetBuffToLong(key);
						
						LenAdata = 17+tempLen; 		
						*ptr = crypt.type[COM_No];
						RAM_Write(ptr+1, crypt.AK, 16);
						RAM_Write(ptr+17, Ciphert+3, tempLen);
						tempLen+=17;
						for(i=0;i<(tempLen/16 + 1);i++)
							{
							RAM_Fill(ptr1,16);
							if(tempLen>=16)
								{
								RAM_Write(ptr1,ptr,16);
								tempLen-=16;
								ptr+=16;
								}
							else
								{
							    RAM_Write(ptr1+16-(tempLen%16),ptr,tempLen);
								tempLen=0;
								}
							AddAuthData.u32[4*i+0]		=	GetBuffToLong(ptr1+12);
							AddAuthData.u32[4*i+1]		=	GetBuffToLong(ptr1+8);
							AddAuthData.u32[4*i+2]		=	GetBuffToLong(ptr1+4);
							AddAuthData.u32[4*i+3]		=	GetBuffToLong(ptr1);
							}
					/*	AddAuthData.u32[0] = GetBuffToLong(ptr+12);//0xDBDCDDDE;//
						AddAuthData.u32[1] = GetBuffToLong(ptr+8);//0xD7D8D9DA;//
						AddAuthData.u32[2] = GetBuffToLong(ptr+4);//0xD3D4D5D6;//
						AddAuthData.u32[3] = GetBuffToLong(ptr);//0x30D0D1D2;//
						RAM_Fill(ptr1,16);
						RAM_Write(ptr1+16-(LenAdata-16),ptr+16,LenAdata-16);
						AddAuthData.u32[4] = GetBuffToLong(ptr1+12);//0x000000DF;//
						AddAuthData.u32[5] = GetBuffToLong(ptr1+8);
						AddAuthData.u32[6] = GetBuffToLong(ptr1+4);
						AddAuthData.u32[7] = GetBuffToLong(ptr1);
						*/		
						break;
					case 0x20:
						
						key = crypt.EK;
						AESKeyMode = KEY_Mode_128bits;
						AesKey.u32[0] = GetBuffToLong(key);
						AesKey.u32[1] = GetBuffToLong(key+4);
						AesKey.u32[2] = GetBuffToLong(key+8);
						AesKey.u32[3] = GetBuffToLong(key+12);

						LenPtext = tempLen;
						RAM_Fill(ptr,128);
						RAM_Write(ptr, Ciphert+3, LenPtext);
		
						for(i=0;i<32;i++)
							Plaintext.u32[i] = 0x00;
						for(i=0;i<(LenPtext/16 + 1);i++)
							{
							RAM_Fill(ptr1,16);
							if(tempLen>=16)
								{
								RAM_Write(ptr1,ptr,16);
								tempLen-=16;
								ptr+=16;
								}
							else
								{
							    RAM_Write(ptr1+16-(tempLen%16),ptr,tempLen);
								tempLen=0;
								}
							Plaintext.u32[4*i+0]		=	GetBuffToLong(ptr1+12);
							Plaintext.u32[4*i+1]		=	GetBuffToLong(ptr1+8);
							Plaintext.u32[4*i+2]		=	GetBuffToLong(ptr1+4);
							Plaintext.u32[4*i+3]		=	GetBuffToLong(ptr1);
							}
						LenIV = 12;
						key = crypt.IV;
						for(i=0;i<4;i++)
					       InitVector.u32[i] = 0x00;
						InitVector.u32[0] = GetBuffToLong(key+8);
						InitVector.u32[1] = GetBuffToLong(key+4);
						InitVector.u32[2] = GetBuffToLong(key);
						LenAdata = 0;		
						
						break;
					case 0x30:
						
						key = crypt.EK;
						AESKeyMode = KEY_Mode_128bits;
						AesKey.u32[0] = GetBuffToLong(key);
						AesKey.u32[1] = GetBuffToLong(key+4);
						AesKey.u32[2] = GetBuffToLong(key+8);
						AesKey.u32[3] = GetBuffToLong(key+12);
						LenPtext = tempLen;
						
						RAM_Fill(ptr,128);
						RAM_Write(ptr, Ciphert+3, LenPtext);
						for(i=0;i<32;i++)
							Plaintext.u32[i] = 0x00;
				
					    for(i=0;i<(LenPtext/16 + 1);i++)
							{
							RAM_Fill(ptr1,16);
							if(tempLen>=16)
								{
								RAM_Write(ptr1,ptr,16);
								tempLen-=16;
								ptr+=16;
								}
							else
								{
							    RAM_Write(ptr1+16-(tempLen%16),ptr,tempLen);
								tempLen=0;
								}
							Plaintext.u32[4*i+0]		=	GetBuffToLong(ptr1+12);
							Plaintext.u32[4*i+1]		=	GetBuffToLong(ptr1+8);
							Plaintext.u32[4*i+2]		=	GetBuffToLong(ptr1+4);
							Plaintext.u32[4*i+3]		=	GetBuffToLong(ptr1);
							}
						LenIV = 12;
						key = crypt.IV;
						for(i=0;i<4;i++)
					        InitVector.u32[i] = 0x00;
						InitVector.u32[0] = GetBuffToLong(key+8);
						InitVector.u32[1] = GetBuffToLong(key+4);
						InitVector.u32[2] = GetBuffToLong(key);

						LenAdata = 17;		
						*ptr = crypt.type[COM_No];
						RAM_Write(ptr+1, crypt.AK, 16);
						for(i=0;i<32;i++)
							   AddAuthData.u32[i] = 0x00;
						AddAuthData.u32[0] = GetBuffToLong(ptr+12);
						AddAuthData.u32[1] = GetBuffToLong(ptr+8);
						AddAuthData.u32[2] = GetBuffToLong(ptr+4);
						AddAuthData.u32[3] = GetBuffToLong(ptr);
		                AddAuthData.u32[4] = *(ptr+16);//GetBuffToLong(ptr+16);
						break;
					default:
						break;
					}

		}
	return 0;
}

int AES_GCM_Decrypt_OR_Encrypt_H( unsigned short COM_No,unsigned char* Ciphert,unsigned short *nCtextLen,unsigned char use_crypt )   //≈–∂œΩ‚√‹¿‡–Õ£¨◊ˆ¿‡–Õ±Í ∂ ‰≥ˆº”Ω‚√‹√‹Œƒ,use_crypt:0-- ‰»Î£¨1--º”√‹..
{
	GCM_Encrypt_InputTypedef GCM_Encrypt_InputStruct;	
	GCM_Encrypt_OutputTypedef	GCM_Encrypt_OutputStruct;
	GCM_Decrypt_InputTypedef GCM_Decrypt_InputStruct;	
	GCM_Decrypt_OutputTypedef	GCM_Decrypt_OutputStruct;
	uint8_t i;
	unsigned short tempLen;
	unsigned char LLC_len = 0;
	unsigned char ErrorNO = 3;			  //÷°≥ˆ¥Ì..
	uint8_t buff[50];
	
	if(use_crypt==0)
		{
		if(( *(Ciphert+3) != 0xC8 )&&( *(Ciphert+3) != 0xC9 )&&( *(Ciphert+3) != 0xCA )&&( *(Ciphert+3) != 0xCB ))
			return 0;
		
		crypt.glo_request[COM_No] = *(Ciphert+3);
		crypt.type[COM_No] = *(Ciphert+5);
		tempLen = *(Ciphert+4);
		Test_KeyWrap();
		AES_GCM_Parameter_Init(COM_No,Ciphert,nCtextLen,use_crypt);
		
		/*Ω‚√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/	
			for(i=0;i<16;i++)
			{
					Plaintext.u32[i] = 0x0000;
			}
			
			GCM_Decrypt_InputStruct.AESKeyMode = AESKeyMode;
			GCM_Decrypt_InputStruct.pKeyAddr = AesKey.u32;
			GCM_Decrypt_InputStruct.pInitVectorAddr = InitVector.u8;
			GCM_Decrypt_InputStruct.LenIV = LenIV;
		//	GCM_Encrypt_OutputStruct.pCtextAddr=Ciphertext.u8;
			GCM_Decrypt_InputStruct.pCtextAddr = Ciphertext.u8;//GCM_Encrypt_OutputStruct.pCtextAddr;
			GCM_Decrypt_InputStruct.LenCtext = LenPtext;//tempLen - 5;//GCM_Encrypt_OutputStruct.LenCtext;
			GCM_Decrypt_InputStruct.pAdataAddr = AddAuthData.u8;
			GCM_Decrypt_InputStruct.LenAdata = LenAdata;
			GCM_Decrypt_InputStruct.AuthTag = &AuthTag;
			GCM_Decrypt_OutputStruct.pPtextAddr = Plaintext.u8;
			DecryptSta = GCM_Decrypt(COM_No,&GCM_Decrypt_InputStruct, &GCM_Decrypt_OutputStruct);			/*!< gcm Ω‚√‹	*/
		
			if(DecryptSta==TRUE)
				{
				if(crypt.type[COM_No]==0x20)
					{
					tempLen -= 5;
					(*nCtextLen) = (*nCtextLen) -7;
					RAM_Write( Ciphert+3,GCM_Decrypt_OutputStruct.pPtextAddr+LenPtext-tempLen, tempLen );
					SwapData(Ciphert+3,tempLen);
					return 0;	
					}
				else if(crypt.type[COM_No]==0x10)
					{
					RAM_Write( (Ciphert+3),(Ciphert+10), tempLen-5-12 );
					(*nCtextLen) = (*nCtextLen) -7-12;
					return 0;
					}
				else if(crypt.type[COM_No]==0x30)
					{
					tempLen -= (5+12);
					(*nCtextLen) = (*nCtextLen) -7-12;
					RAM_Write( Ciphert+3,GCM_Decrypt_OutputStruct.pPtextAddr+LenPtext-tempLen, tempLen );
					SwapData(Ciphert+3,tempLen);
					
					return 0;
					}
				}
		}
	else if(use_crypt==1)
		{	
		if((crypt.type[COM_No] != 0x10)&&(crypt.type[COM_No] != 0x20)&&(crypt.type[COM_No] != 0x30))
			return 0;
		AES_GCM_Parameter_Init(COM_No,Ciphert,nCtextLen,use_crypt);
		if(( *(Ciphert) == 0xE6 )&&( *(Ciphert+1) == 0xE7 )&&( *(Ciphert+2) == 0x00 ))
				{
					LLC_len = 3;
					if((*nCtextLen)<=3) return ErrorNO;
					else (*nCtextLen) -= 3; 					
				}
		/*º”√‹ ‰»ÎΩ·ππÃÂ≥ı ºªØ*/		
			for(i=0;i<16;i++)
			{
					Ciphertext.u32[i] = 0x0000;
					AuthTag.u8[i] = 0x00;
			}
			GCM_Encrypt_InputStruct.AESKeyMode = AESKeyMode;
			GCM_Encrypt_InputStruct.pKeyAddr = AesKey.u32;
			GCM_Encrypt_InputStruct.pInitVectorAddr = InitVector.u8;
			GCM_Encrypt_InputStruct.LenIV = LenIV;
			GCM_Encrypt_InputStruct.pPtextAddr = Plaintext.u8;
			GCM_Encrypt_InputStruct.LenPtext = LenPtext;
			GCM_Encrypt_InputStruct.pAdataAddr = AddAuthData.u8;
			GCM_Encrypt_InputStruct.LenAdata = LenAdata;
			GCM_Encrypt_OutputStruct.AuthTag = &AuthTag;
			GCM_Encrypt_OutputStruct.pCtextAddr = Ciphertext.u8;
			GCM_Encrypt(COM_No,&GCM_Encrypt_InputStruct,&GCM_Encrypt_OutputStruct);		/*!< gcm º”√‹	*/
			if(crypt.type[COM_No] == 0x20)
				{
					
					*Ciphert = 0xE6;
					*(Ciphert +1) = 0xE7;
					*(Ciphert +2) = 0;
					*(Ciphert +3) = crypt.glo_request[COM_No] + 4;		//..
					*(Ciphert +4) = (*nCtextLen)+5;	 //º”√‹ ˝æ›≥§∂»..
					*(Ciphert +5) = crypt.type[COM_No];		//..
					*(Ciphert +6) = 0x01;
					*(Ciphert +7) = 0x23;
					*(Ciphert +8) = 0x45;
					*(Ciphert +9) = 0x67;	
					tempLen = *nCtextLen;
					if(tempLen>16)
					{
					for(i=0;i<(tempLen/16);i++)
						{
						RAM_Write(Ciphert+10+16*i,GCM_Encrypt_OutputStruct.pCtextAddr+16*i,16);
						SwapData(Ciphert+10+16*i,16);
						}
					RAM_Write(Ciphert+10+16*i,GCM_Encrypt_OutputStruct.pCtextAddr+16*i,tempLen-16*i);
					SwapData(Ciphert+10+16*i,tempLen-16*i);
						}
					else
						{
					RAM_Write(Ciphert+10,GCM_Encrypt_OutputStruct.pCtextAddr,tempLen);	
					SwapData(Ciphert+10,tempLen);
						}
					tempLen +=10;
					*nCtextLen = tempLen;				
				}
			else if(crypt.type[COM_No] == 0x10)
				{
				
				tempLen = *nCtextLen; 				
				RAM_Write( buff,Ciphert+3, tempLen );
					*Ciphert = 0xE6;
					*(Ciphert+1) = 0xE7;
					*(Ciphert+2) = 0;
					*(Ciphert+3) = crypt.glo_request[COM_No] + 4;		//..
					*(Ciphert+4) = (*nCtextLen)+5+12;	//º”√‹ ˝æ›≥§∂»..
					*(Ciphert+5) = crypt.type[COM_No];		//..
					*(Ciphert+6) = 0x01;
					*(Ciphert+7) = 0x23;
					*(Ciphert+8) = 0x45;
					*(Ciphert+9) = 0x67;	
				   
					RAM_Write( Ciphert+10,buff, tempLen );
					tempLen +=10;
					RAM_Write( Ciphert+tempLen,AuthTag.u8, 12 );
					 *nCtextLen = tempLen+12;

				}
			else if(crypt.type[COM_No] == 0x30)
				{
				 *Ciphert = 0xE6;
				 *(Ciphert+1) = 0xE7;
				 *(Ciphert+2) = 0;
				 *(Ciphert+3) = crypt.glo_request[COM_No] + BIT2;		  //..
				 *(Ciphert+4) = (*nCtextLen)+5+12;   //º”√‹ ˝æ›≥§∂»..
				 *(Ciphert+5) = crypt.type[COM_No];	  //..
				 *(Ciphert+6) = 0x01;
				 *(Ciphert+7) = 0x23;
				 *(Ciphert+8) = 0x45;
				 *(Ciphert+9) = 0x67;	  
					 
				 tempLen = *nCtextLen;
				 if(tempLen>16)
					{
					for(i=0;i<(tempLen/16);i++)
						{
						RAM_Write(Ciphert+10+16*i,GCM_Encrypt_OutputStruct.pCtextAddr+16*i,16);
						SwapData(Ciphert+10+16*i,16);
						}
					RAM_Write(Ciphert+10+16*i,GCM_Encrypt_OutputStruct.pCtextAddr+16*i,tempLen-16*i);
					SwapData(Ciphert+10+16*i,tempLen-16*i);
						}
					else
						{
					RAM_Write(Ciphert+10,GCM_Encrypt_OutputStruct.pCtextAddr,tempLen);	
					SwapData(Ciphert+10,tempLen);
						}
				  tempLen += 10;
				RAM_Write( Ciphert+tempLen,AuthTag.u8, 12 );
				*nCtextLen = tempLen+12;

				}
			crypt.type[COM_No] = 0x00;
		}
	return 0;
}
/*
*********************************************************************************************************
*                          64bits  xor function
*
* ∫Ø ˝Àµ√˜: 64bits “ÏªÚ∫Ø ˝
*
* »Îø⁄≤Œ ˝: 
*						pIntDataAddr1		uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢“ÏªÚ«∞ ˝æ›1 : pIntDataAddr1[0]Œ™µÕŒª
*                       
*						pIntDataAddr2  	uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢“ÏªÚ«∞ ˝æ›2:  pIntDataAddr2[0]Œ™µÕŒª
*
* ∑µªÿ≤Œ ˝: 
*						pOutDataAddr  	uint32_t *–Õ±‰¡ø£¨∆‰÷∏œÚ∂‘œÛ¥Ê¥¢“ÏªÚ∫Û ˝æ› : pOutDataAddr[0]Œ™µÕŒª
* 
* Ãÿ ‚Àµ√˜: Œﬁ
*********************************************************************************************************
*/
void HT_AES_Xor64bits(uint32_t *pIntDataAddr1, uint32_t *pIntDataAddr2, uint32_t *pOutDataAddr)
{
    /*  assert_param  */
		uint8_t i;
	
		for(i=0;i<2;i++)
		{
			*pOutDataAddr++ = (*pIntDataAddr1++) ^ (*pIntDataAddr2++);		/*!< “ÏªÚº∆À„ 				*/
		}
           		
}
/*
*********************************************************************************************************
*                         Fun_Key_Wrap_128Bits
*
* ∫Ø ˝Àµ√˜: Wrap 128 bits of Key Data with a 128-bit KEK
*
* »Îø⁄≤Œ ˝: 
*						key		uint8_t *–Õ±‰¡ø£¨√ÿ‘ø
*                       
*						Plaint  	uint8_t *–Õ±‰¡ø£¨√˜Œƒ
* ∑µªÿ≤Œ ˝: 
*						Ciphert  	uint8_t *–Õ±‰¡ø£¨√‹Œƒ
* 
* Ãÿ ‚Àµ√˜: Œﬁ
*********************************************************************************************************
*/
int Fun_Key_Wrap_128Bits(unsigned char* key,unsigned char* Ciphert,unsigned char* Plaint)
{
 unsigned char i,j;
 int Error_Byte=0;
 unsigned char A[8];
 unsigned char IV[8]={0xA6,0xA6,0xA6,0xA6,0xA6,0xA6,0xA6,0xA6};
 unsigned char R[16];
 unsigned char AR[16];
// unsigned char* key;
 unsigned char* ptr;
 uint32_t *T;
 uint32_t T_Value[2];
 uint32_t Temp_Value[2];
 GCM_Encrypt_InputTypedef GCM_Encrypt_InputStruct;	 
 Parameter128bits_TypeDef	 Header;

  ptr = R;
  T = T_Value;

//step1:Initialize variables
	RAM_Write(A,IV,8);
 	RAM_Write(ptr,Plaint,16);

//Calculate intermediate values
//	key = crypt.EK;
	AESKeyMode = KEY_Mode_128bits;
	AesKey.u32[0] = GetBuffToLong(key);
	AesKey.u32[1] = GetBuffToLong(key+4);
	AesKey.u32[2] = GetBuffToLong(key+8);
	AesKey.u32[3] = GetBuffToLong(key+12);
	GCM_Encrypt_InputStruct.AESKeyMode = AESKeyMode;
	GCM_Encrypt_InputStruct.pKeyAddr = AesKey.u32;
	GCM_Encrypt_InputStruct.pPtextAddr = Plaintext.u8;

	for(j=0;j<6;j++)
	{
	 for(i=0;i<2;i++)
	 	{
	 	 RAM_Fill(Header.u8,16);
		 RAM_Write(AR ,A,8);
	 	 RAM_Write(&AR[8] ,ptr+8*i,8);
		 Header.u32[3] = GetBuffToLong(AR);
		 Header.u32[2] = GetBuffToLong(&AR[4]);
		 Header.u32[1] = GetBuffToLong(&AR[8]);
		 Header.u32[0] = GetBuffToLong(&AR[12]);
		 HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);									 /*¥Úø™AESƒ£øÈ ±÷”				 */  
		 
		 HT_AES_Encrypt(GCM_Encrypt_InputStruct.AESKeyMode,GCM_Encrypt_InputStruct.pKeyAddr,Header.u32);
		 while(HT_AES_Busy_StatusGet());														 /*!< AESº”√‹Ω· ¯≈–∂œ							 */
		 
		 HT_AES_Read(Header.u32);  /*!< º”√‹‘ÀÀ„Ω·π˚–¥»Î	 *pOutDataAddr	 */
		 HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE); 								 /*πÿ±’AESƒ£øÈ ±÷”				 */ 	 
	     RAM_Write(ptr+8*i,(unsigned char*)&Header.u8,8);
	     SwapData(ptr+8*i,8);
		 T_Value[1] = 0;
		 T_Value[0] = i+1+2*j;
		 HT_AES_Xor64bits(&Header.u32[2],T, Temp_Value);
		 
		 FillLongToBuff(A,Temp_Value[1]);
		 FillLongToBuff(&A[4],Temp_Value[0]);

	 	}
	}
	//Output the results
	
	RAM_Write(Ciphert,A,8);
	RAM_Write(Ciphert+8,ptr,16);
	return Error_Byte;
}
/*
*********************************************************************************************************
*                         Fun_Key_Unwrap_128Bits
*
* ∫Ø ˝Àµ√˜: Unwrap 128 bits of Key Data with a 128-bit KEK
*
* »Îø⁄≤Œ ˝: 
*						key		uint8_t *–Õ±‰¡ø£¨√ÿ‘ø
*                       
*						Ciphert  	uint8_t *–Õ±‰¡ø£¨√‹Œƒ
* ∑µªÿ≤Œ ˝: 
*						Plaint  	uint8_t *–Õ±‰¡ø£¨√˜Œƒ
* 
* Ãÿ ‚Àµ√˜: Œﬁ
*********************************************************************************************************
*/
int Fun_Key_Unwrap_128Bits(unsigned char* key,unsigned char* Ciphert,unsigned char* Plaint)
{
	unsigned char i,j;
	int Error_Byte=0;
	unsigned char A[8];
	unsigned char R[16];	
	unsigned char AR[16];	
	unsigned char Temp[8];
	
	unsigned char IV[8]={0xA6,0xA6,0xA6,0xA6,0xA6,0xA6,0xA6,0xA6};
	unsigned char* ptr;	
	uint32_t *T;
	uint32_t T_Value[2];	
	uint32_t Temp_Value[2];
	GCM_Encrypt_InputTypedef GCM_Encrypt_InputStruct;	
	Parameter128bits_TypeDef	Header;

	ptr = R;	
	T = T_Value;
//Initialize variables
	RAM_Write(A,Ciphert,8);
    RAM_Write(ptr,Ciphert+8,8);
	RAM_Write(ptr+8,Ciphert+16,8);

//Calculate the intermediate values
	AESKeyMode = KEY_Mode_128bits;
	AesKey.u32[0] = GetBuffToLong(key);
	AesKey.u32[1] = GetBuffToLong(key+4);
	AesKey.u32[2] = GetBuffToLong(key+8);
	AesKey.u32[3] = GetBuffToLong(key+12);
	GCM_Encrypt_InputStruct.AESKeyMode = AESKeyMode;
	GCM_Encrypt_InputStruct.pKeyAddr = AesKey.u32;
	GCM_Encrypt_InputStruct.pPtextAddr = Plaintext.u8;

	for(j=12;j>0;j--)
		{		
		T_Value[1] = 0;
		T_Value[0] = j;
		
		Header.u32[1] = GetBuffToLong(A);
		Header.u32[0] = GetBuffToLong(&A[4]);
		HT_AES_Xor64bits(&Header.u32[0],T, Temp_Value);
		
		FillLongToBuff(AR,Temp_Value[1]);
		FillLongToBuff(&AR[4],Temp_Value[0]);
		RAM_Write(&AR[8] ,ptr+8,8);
		RAM_Write(Temp ,ptr,8);
		
		Header.u32[3] = GetBuffToLong(AR);
		Header.u32[2] = GetBuffToLong(&AR[4]);
		Header.u32[1] = GetBuffToLong(&AR[8]);
		Header.u32[0] = GetBuffToLong(&AR[12]);
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,ENABLE);									/*¥Úø™AESƒ£øÈ ±÷”				*/	
		
		HT_AES_Decrypt(GCM_Encrypt_InputStruct.AESKeyMode,GCM_Encrypt_InputStruct.pKeyAddr,Header.u32);
		while(HT_AES_Busy_StatusGet()); 														/*!< AESΩ‚√‹Ω· ¯≈–∂œ							*/
		
		HT_AES_Read(Header.u32);  /*!< º”√‹‘ÀÀ„Ω·π˚–¥»Î 	*pOutDataAddr	*/
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_ADESRADEN,DISABLE);									/*πÿ±’AESƒ£øÈ ±÷”				*/		
		FillLongToBuff(A,Header.u32[3]);
		FillLongToBuff(&A[4],Header.u32[2]);//A[t-1]
		FillLongToBuff(ptr,Header.u32[1]);
		FillLongToBuff(ptr+4,Header.u32[0]);//R[t-1]
		RAM_Write(ptr+8,Temp,8);//R[t-1][i] = R[t][i-1]

		}
//Output the results
		if(Data_Comp(A,IV,8)==0)
			{//Key Data Integrity -- the Initial Value
				RAM_Write(Plaint,ptr,16);
			}
		else
			Error_Byte=1;
	return Error_Byte;

}
void Test_KeyWrap(void)
{
	unsigned char Key_Data[16]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};
	unsigned char Key[16]	= {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};	//º”√‹√‹‘ø..
	unsigned char Key_wrap[24];
	unsigned char Key_unwrap[16];
	
	Fun_Key_Wrap_128Bits(Key,Key_wrap,Key_Data);
	Fun_Key_Unwrap_128Bits(Key,Key_wrap,Key_unwrap);
}
#endif                                        /* This File Only support HT6x2x and HT501x */

#endif  /* __EXAMPLE_C */
