#include "ht6xxx_gpio.h"
#include "ht6xxx.h"
#include "TypeRAM.h"
#include "Port.h"
#include "data.h"
#include "TypeE2p.h"
#define	E2P_PAGE	32
//#define MAX_TMP 	50
#define MAX_TMP 	150
/*
void GetI2CPara( I2CPARA* I2CPara, unsigned short Devads )	//获取该I2C接口芯片在CPU上的IO口配置
{
	switch( Devads )
	{
		case DataEAds:
		case AdjEAds:
		case ProfEAds:		
			I2CPara->SDA_DIR = (unsigned char*)&PDIR_SDA;
			I2CPara->SCL_DIR = (unsigned char*)&PDIR_SCL;
			I2CPara->SDA_IN = (unsigned char*)&PIN_SDA;			
			I2CPara->SDA = P_SDA;
			I2CPara->SCL = P_SCL;
			break;
		case FMAds:
			I2CPara->SDA_DIR = (unsigned char*)&PDIR_SDA16;
			I2CPara->SCL_DIR = (unsigned char*)&PDIR_SCL;
			I2CPara->SDA_IN = (unsigned char*)&PIN_SDA16;			
			I2CPara->SDA = P_SDA16;
			I2CPara->SCL = P_SCL;
			break;
		case RTCAds:
		case DS3231Ads:
			I2CPara->SDA_DIR = (unsigned char*)&PDIR_RTCSDA;
			I2CPara->SCL_DIR = (unsigned char*)&PDIR_RTCSCL;
			I2CPara->SDA_IN = (unsigned char*)&PIN_RTCSDA;					
			I2CPara->SDA = P_RTCSDA;
			I2CPara->SCL = P_RTCSCL;
			break;
		case PCF8578Ads:                                   //PCF8578	//10.08.03
		case BU97950Ads:                                   //10.11.11
#if ( LCDDrive_BR8564 == YesCheck )
		case BR8564Ads:                                    //12.07.18
#endif		
			I2CPara->SDA_DIR = (unsigned char*)&PDIR_LCDSTR;
			I2CPara->SCL_DIR = (unsigned char*)&PDIR_LCDCLK;
			I2CPara->SDA_IN = (unsigned char*)&PIN_LCDSTR;					
			I2CPara->SDA = P_LCDSTR;
			I2CPara->SCL = P_LCDCLK;
			break;			
		default: 
			break;				
	}
}	
*/

void GetI2CPara( I2CPARA* I2CPara, unsigned short Devads )	//获取该I2C接口芯片在CPU上的IO口配置
{
	switch( Devads )
	{
		case DataEAds:
		case AdjEAds:
		case ProfEAds:		
			I2CPara->SDA_DIR = (unsigned long*)&HT_GPIOC->PTDIR;//(unsigned char*)&PDIR_SDA;
			I2CPara->SCL_DIR = (unsigned long*)&HT_GPIOC->PTDIR;//(unsigned char*)&PDIR_SCL;
			I2CPara->SDA_IN = (unsigned long*)&HT_GPIOC->PTDAT;//(unsigned char*)&PIN_SDA;			
			I2CPara->SDA = GPIOC_EE_SDA;//P_SDA;
			I2CPara->SCL = GPIOC_EE_SCL;//P_SCL;
			break;
		case FMAds:
			I2CPara->SDA_DIR = (unsigned long*)&HT_GPIOE->PTDIR;//(unsigned char*)&PDIR_SDA16;
			I2CPara->SCL_DIR = (unsigned long*)&HT_GPIOC->PTDIR;//(unsigned char*)&PDIR_SCL;
			I2CPara->SDA_IN = (unsigned long*)&HT_GPIOE->PTDAT;//(unsigned char*)&PIN_SDA16;			
			I2CPara->SDA = GPIO_Pin_0;//P_SDA16;
			I2CPara->SCL = GPIOC_EE_SCL;//P_SCL;
			break;
		case PCF8578Ads:                                   //PCF8578	//10.08.03
		case BU97950Ads:                                   //10.11.11
#if ( LCDDrive_BR8564 == YesCheck )
		case BR8564Ads:                                    //12.07.18
#endif		
			I2CPara->SDA_DIR = (unsigned long*)&HT_GPIOB->PTDIR;//(unsigned char*)&PDIR_LCDSTR;
			I2CPara->SCL_DIR = (unsigned long*)&HT_GPIOB->PTDIR;//(unsigned char*)&PDIR_LCDCLK;
			I2CPara->SDA_IN = (unsigned long*)&HT_GPIOB->PTDAT;//(unsigned char*)&PIN_LCDSTR;					
			I2CPara->SDA = GPIOB_LCDSDA;//P_LCDSTR;
			I2CPara->SCL = GPIOB_LCDSCL;//P_LCDCLK;
			break;			
		default: 
			break;				
	}
}	

short I2CMtData( I2CPARA* I2CPara, unsigned char SendData )			//I2C总线发送1字节	
{
	short i;
	
	for( i=0;i<8;i++ )
	{
//		Delay(5);				//10.08.17
		Delay(3);				//14.09.03
		if(( SendData & ByteBit[7-i] ) != 0 ) *I2CPara->SDA_DIR &= ~I2CPara->SDA;
		else
			{
			*I2CPara->SDA_DIR |= I2CPara->SDA;
			HT_GPIOE->PTCLR |= I2CPara->SDA;
			}
		Delay(5);
//		Delay(10);				//10.08.17
		*I2CPara->SCL_DIR &= ~I2CPara->SCL;
		Delay(5);
//		Delay(10);				//10.08.17	
		*I2CPara->SCL_DIR |= I2CPara->SCL;
	}
	Delay(5);
	*I2CPara->SDA_DIR &= ~I2CPara->SDA;		
	Delay(5);
	*I2CPara->SCL_DIR &= ~I2CPara->SCL;
	Delay(5);
	if(( *I2CPara->SDA_IN & I2CPara->SDA ) != 0 ) i = 1;		//检测ACK信号
	else  i = 0;
	*I2CPara->SCL_DIR |= I2CPara->SCL;
	Delay(3);				//14.09.03
	return i;	
}

short I2CStart( I2CPARA* I2CPara, unsigned short E2P_Addr, unsigned short Devads )	//I2C总线Start
{
	unsigned short E2PAddr;
	unsigned char CtlByte; 
	
	E2PAddr = E2P_Addr;
	CtlByte = Devads;
	Devads &= ~0x01;					//去掉读标志位 
	if( Devads == AdjEAds )				//AT24C04
	{
		if( E2PAddr >= 0x100 ) CtlByte |= 0x02;
	}
	else if( Devads == FMAds )			//FM24CL16
	{
		E2PAddr = ( E2PAddr / 128 ) & 0x0E ;
		CtlByte &= 0xF1;
		CtlByte |= E2PAddr;
	}		
	Delay(3);				//14.09.03
	*I2CPara->SDA_DIR |= I2CPara->SDA;	
	HT_GPIOE->PTCLR |= I2CPara->SDA;
	Delay(5);
	*I2CPara->SCL_DIR |= I2CPara->SCL;
	return I2CMtData( I2CPara, CtlByte );				
}

short I2CErr( I2CPARA* I2CPara )			//I2C总线发送1字节	
{
	Delay(3);				//14.09.03
	*I2CPara->SDA_DIR |= I2CPara->SDA;	
	HT_GPIOE->PTCLR |= I2CPara->SDA;
	Delay(5);
	*I2CPara->SCL_DIR &= ~I2CPara->SCL;
	Delay(5);
	*I2CPara->SDA_DIR &= ~I2CPara->SDA;		
	Delay(3);				//14.09.03
	
	return -1;	
}	

short I2CStartAddr( I2CPARA* I2CPara, unsigned short E2P_Addr, unsigned short Devads )	//I2C总线Start+地址
{
	unsigned short i;
	
	for( i=0;i<150;i++ )
	{
		if( I2CStart( I2CPara, E2P_Addr, Devads ) == 0 ) break;	
		*I2CPara->SCL_DIR &= ~I2CPara->SCL;
	}	
	if( i == 150 ) return I2CErr( I2CPara );

	if(( Devads == DataEAds )||( Devads == ProfEAds ))
	{
		if( I2CMtData( I2CPara, (unsigned char)(E2P_Addr/256) ) != 0 ) return I2CErr( I2CPara );			
	}	
	if( I2CMtData( I2CPara, (unsigned char)E2P_Addr ) != 0 ) return I2CErr( I2CPara );			
	
	return 0;
}	


/**********************************************************************
	Asm Subroutine
	Source File in Asms.s43
**********************************************************************/
short E2PRead( unsigned char* RAM_Addr, unsigned short E2P_Addr, unsigned short Lenth, unsigned short Devads)
{
	unsigned short Buff[20];
	I2CPARA* I2CPara;
	unsigned short i,j;
	
//	WDTCTL = WDT_ARST_1000;
	HT_FreeDog();
	I2CPara = (I2CPARA*)Buff;
	GetI2CPara( I2CPara, Devads );				//获取该I2C接口芯片在CPU上的IO口配置

//	if(( I2CStartAddr( I2CPara, E2P_Addr, Devads )) != 0 ) return -1;  
	if(( I2CStartAddr( I2CPara, E2P_Addr, Devads )) != 0 ) 
	{
		memset( RAM_Addr,0, Lenth );
		return -1;  
	}	

	*I2CPara->SCL_DIR &= ~I2CPara->SCL;
//	if( I2CStart( I2CPara, E2P_Addr, Devads+0x01 ) != 0 ) return I2CErr( I2CPara );		//读命令
	if( I2CStart( I2CPara, E2P_Addr, Devads+0x01 ) != 0 ) 
	{
		memset( RAM_Addr,0, Lenth );
		return I2CErr( I2CPara );		//读命令
	}	

	for( i=0;i<Lenth;i++ )
	{
		*(RAM_Addr+i) = 0;
		*I2CPara->SDA_DIR &= ~I2CPara->SDA;
		Delay(5);
		for( j=0;j<8;j++ )
		{
//			if(( i == 0 )||( i == Lenth-1)) *I2CPara->SDA_DIR &= ~I2CPara->SDA;
//			else *I2CPara->SDA_DIR |= I2CPara->SDA;
			*I2CPara->SCL_DIR &= ~I2CPara->SCL;
			Delay(5);
			if(( *I2CPara->SDA_IN & I2CPara->SDA ) != 0 ) *(RAM_Addr+i) |= ByteBit[7-j];	
			*I2CPara->SCL_DIR |= I2CPara->SCL;
			Delay(5);
		}	
		if( i == (Lenth-1) ) *I2CPara->SDA_DIR &= ~I2CPara->SDA;
		else
			{*I2CPara->SDA_DIR |= I2CPara->SDA;
			HT_GPIOE->PTCLR |= I2CPara->SDA;
			}
		Delay(5);				//14.09.03
		*I2CPara->SCL_DIR &= ~I2CPara->SCL;	
		Delay(5);
		*I2CPara->SCL_DIR |= I2CPara->SCL;
		Delay(5);
	}
	
	*I2CPara->SDA_DIR |= I2CPara->SDA;		//STOP
	HT_GPIOE->PTCLR |= I2CPara->SDA;
	
	Delay(5);				//14.09.03
	*I2CPara->SCL_DIR &= ~I2CPara->SCL;		
	Delay(5);
	*I2CPara->SDA_DIR &= ~I2CPara->SDA;		//STOP
	Delay(3);				//14.09.03
	
	return 0;	
}	 

short E2Pwrite( unsigned short E2P_Addr, unsigned char* RAM_Addr, unsigned short Lenth,unsigned short Devads )
{
	unsigned short Buff[10];
	I2CPARA* I2CPara;
	unsigned short i,Len,PageLen;
	
//	WDTCTL = WDT_ARST_1000;
	HT_FreeDog();
	I2CPara = (I2CPARA*)Buff;
	GetI2CPara( I2CPara, Devads );				//获取该I2C接口芯片在CPU上的IO口配置
	switch( Devads )
	{
		case DataEAds:
		case ProfEAds:		
			PageLen = 64;						//页写的最大长度
			break;
		case FMAds:
			PageLen = 16;
			break;
		default: PageLen = 8;
			break;				
	}

	while( Lenth > 0 )
	{
		Len = E2P_Addr % PageLen;
		if( Len != 0 ) Len = PageLen - Len;
		else Len = PageLen;
		if( Lenth < Len ) Len = Lenth;
		
		if(( I2CStartAddr( I2CPara, E2P_Addr, Devads )) != 0 ) return -1;  

		for( i=0;i<Len;i++ )
		{
			if( I2CMtData( I2CPara, *(RAM_Addr+i) ) != 0 ) return I2CErr( I2CPara );			
		}	
		Delay(5);				//10.08.17
		*I2CPara->SDA_DIR |= I2CPara->SDA;		//STOP		
		HT_GPIOE->PTCLR |= I2CPara->SDA;
		Delay(5);
		*I2CPara->SCL_DIR &= ~I2CPara->SCL;
		Delay(5);
		*I2CPara->SDA_DIR &= ~I2CPara->SDA;		//STOP
		Delay(3);				//14.09.03

		RAM_Addr += Len;
		Lenth -= Len;
		E2P_Addr += Len;
	}
    return 0;
}

/**********************************************************************
	Write E2PROM	
**********************************************************************/
short E2P_Write( unsigned short E2P_Dest, unsigned char* RAM_Src, short Lenth, short Dev_Ads )
{
	unsigned char Buffer[MAX_TMP];
	unsigned char* Temp_Pt;
	short i=0;
	short NextPage;
	unsigned char BakNum;
	
	Temp_Pt = Buffer;
	HT_FreeDog();
        memcpy( Temp_Pt, RAM_Src, Lenth );
	if(( Dev_Ads == DataEAds )&&( E2P_Dest >= (E2P_PGLEN * 2) )) 
	{
		BakNum = 1;				//事件记录无备份	
		Lenth -= 1;			//无备份数据无校验和
	}	
	else
	{
		*(Temp_Pt + Lenth ) = ChkNum( Temp_Pt, Lenth );
		for( i=0;i<4;i++)
		{
			if( E2PTab[i].DevAds == Dev_Ads )
			{
				BakNum = E2PTab[i].BakNum;
				NextPage = E2PTab[i].NextPage;	
				break;
			}				
		}		 
	}
	for( i=0;i<BakNum;i++)
	{
		if( E2Pwrite( E2P_Dest, Temp_Pt, Lenth+1, Dev_Ads ) != 0) Flag.AlarmFlg[0] |= F_AlarmE2P;
		else Flag.AlarmFlg[0] &= ~F_AlarmE2P;
	}

	return 0;
}

/**********************************************************************
	Read E2PROM	
**********************************************************************/
short E2P_Read( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth, short Dev_Ads )
{
	unsigned char Buffer[MAX_TMP];
	unsigned char* Temp_Pt;
	short i=0;
	short NextPage;
	unsigned char BakNum;
	unsigned char CorrectMode;
    unsigned char Check;

	Temp_Pt = Buffer;
//	WDTCTL = WDT_ARST_1000;
	HT_FreeDog();
	for( i=0;i<4;i++)
	{
		if( E2PTab[i].DevAds == Dev_Ads )
		{
			BakNum = E2PTab[i].BakNum;
			NextPage = E2PTab[i].NextPage;	
			CorrectMode = E2PTab[i].CorrectMode;
			break;
		}				
	}		 
	if(( Dev_Ads == DataEAds )&&( E2P_Src >= (E2P_PGLEN * 2) )) 
	{
//		BakNum = 1;		//事件记录无备份
		if( E2PRead( Temp_Pt, E2P_Src, Lenth, Dev_Ads ) != 0 )
		{
			Flag.AlarmFlg[0] |= F_AlarmE2P;
			Flag.BatState |= F_E2PCheck;		//EC		//E2PROM校验和错
		}
		else Flag.BatState &= ~F_E2PCheck;		//E2PROM正常	//10.11.05
	    memcpy( RAM_Dest, Temp_Pt, Lenth );
		return 0;		
	}	

	for( i=0;i<BakNum;i++ )
	{
		if( E2PRead( Temp_Pt, E2P_Src, Lenth+1, Dev_Ads ) != 0 )
		{
			//Flag.ErrFlg |= F_ErrE2P;
			Flag.AlarmFlg[0] |= F_AlarmE2P;
			E2P_Src += NextPage;
			Flag.BatState |= F_E2PCheck;		//EC		//E2PROM校验和错
			continue;		
		}
		//else Flag.ErrFlg &= ~F_ErrE2P;
		else Flag.AlarmFlg[0] &= ~F_AlarmE2P;
        Check = *(Temp_Pt + Lenth );
//		if( *(Temp_Pt + Lenth ) != ChkNum( Temp_Pt, Lenth ) )
        if( Check != ChkNum( Temp_Pt, Lenth ) )
		{
			E2P_Src += NextPage;
			Flag.BatState |= F_E2PCheck;		//EC		//E2PROM校验和错
			continue;		
		}
		else Flag.BatState &= ~F_E2PCheck;		//EC
		break;
	}
//	WDTCTL = WDT_ARST_1000;
	HT_FreeDog();
	if( i != 0) 
	{
		//for(;i>=0;i--)
		for(;i>0;i--)
		{
			if( i == BakNum )
			{
				if( CorrectMode == 1 ) 
				{
					//RAM_Fill( Temp_Pt, Lenth, 0x00);
					memset( Temp_Pt, 0,Lenth);	
					*(Temp_Pt + Lenth ) = 0xA5;			
				}	
				else break;
			}	
			//E2P_Write( E2P_Src - NextPage, Temp_Pt, Lenth, Dev_Ads );
//			_E2Pwrite( E2P_Src - ( NextPage * ( BakNum - i )), Temp_Pt, Lenth+1, Dev_Ads );
//			E2Pwrite( E2P_Src - ( NextPage * ( BakNum - i )), Temp_Pt, Lenth+1, Dev_Ads );
			E2P_Src -= NextPage;
			E2Pwrite( E2P_Src, Temp_Pt, Lenth+1, Dev_Ads );
		}	
	}
	memcpy( RAM_Dest, Temp_Pt, Lenth );
	return	0;	
}

short E2P_RData( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth )
{
	if( Lenth == 0 ) return 0;		//17.05.20
	E2P_Read( RAM_Dest, E2P_Src, Lenth, DataEAds );
	return 0;
}	
short E2P_WData( unsigned short E2P_Dest, unsigned char* RAM_Src, short Lenth )
{
	if( Lenth == 0 ) return 0;		//17.05.20
	E2P_Write( E2P_Dest, RAM_Src, Lenth, DataEAds );
	return	0;
}

short E2P_RFM( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth )
{
	if( Lenth == 0 ) return 0;		//17.05.20
	E2P_Read( RAM_Dest, E2P_Src, Lenth, FMAds );
	return 0;
}
short E2P_WFM( unsigned short E2P_Dest, unsigned char* RAM_Src, short Lenth )
{
	if( Lenth == 0 ) return 0;		//17.05.20
	E2P_Write( E2P_Dest, RAM_Src, Lenth, FMAds );
	return	0;
}

short E2P_RAdj( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth )
{
	E2P_Read( RAM_Dest, E2P_Src, Lenth, DataEAds );
	return 0;
}	
short E2P_WAdj( unsigned short E2P_Dest, unsigned char* RAM_Src, short Lenth )
{
	E2P_Write( E2P_Dest, RAM_Src, Lenth, DataEAds );
	return 0;
}	
short E2P_PRData( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth )
{
	E2P_Read( RAM_Dest, E2P_Src, Lenth, ProfEAds );
	return 0;
}	
short E2P_PWData( unsigned short E2P_Dest, unsigned char* RAM_Src,short Lenth )
{
	E2P_Write( E2P_Dest, RAM_Src, Lenth, ProfEAds );
	return 0;
}



/**********************************************************************
	Move Data in E2PROM	
**********************************************************************/
/*
void E2P_E2P( unsigned short Dest, unsigned short Src, short Len, short Dev_Ads )
{
	char	Buffer[MAX_TMP];
	short	Size;
	
	while( Len > 0 ){
		if( Len > MAX_TMP ) Size = MAX_TMP;
		else Size = Len;
		
		E2P_Read( (char*)&Buffer, Src, Size, Dev_Ads );
		E2P_Write( Dest, (char*)&Buffer, Size, Dev_Ads );
		Dest += Size;
		Src += Size;
		Len -= Size;
	}
}
*/
/**********************************************************************
	Fill Data to E2PROM	
**********************************************************************/
/*
void E2P_Fill( unsigned short Dest, short Len, char Data, short Dev_Ads )
{
	char	Buffer[MAX_TMP];
	short	Size;
	
	RAM_Fill( (char*)&Buffer, MAX_TMP, Data );	

	while( Len > 0 ){
		if( Len > MAX_TMP ) Size = MAX_TMP;
		else Size = Len;
		E2P_Write( Dest, (char*)&Buffer, Size, Dev_Ads );
		Dest += Size;
		Len -= Size;
	}
}
*/
