#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
//#include "DLT698.h"					
#include "Measure.h"
#include "TypeFRAM.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Port.h"
#include "Display.h"
//#include "RsComm.h"
#include "Initial.h"
#include "Time.h"
#include "Power.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
//#include "cpucard.h"
#include "ECProcess.h"			//10.12.08
#include "COSEMComm.h"

short RAMECIsOK( unsigned char* ECRAds )
{
//	if(( ChkNum( ECRAds, ECDIGIT ) == *(ECRAds+ECDIGIT) )&&( AllIsBCD( ECRAds, ECDIGIT ) == 0 )) return 0;
	if( ChkNum( ECRAds, ECDIGIT ) == *(ECRAds+ECDIGIT) )
		return 0;
	else
		return 1;
}	

short E2PECIsOK( unsigned char* ECRAds )
{
//	if((( Flag.BatState & F_E2PCheck ) == 0 )&&( AllIsBCD( ECRAds, ECDIGIT ) == 0 )) return 0;
	if(( Flag.BatState & F_E2PCheck ) == 0 )
		return 0;
	else
		return 1;
}


//取当前铁电电量地址在ECRgTab表内对应的行号
short GetECRgTabRowNo( unsigned short ECFAds )		
{
	short i;
	unsigned short StartNo,EndNo;
	
	for( i=0;i<ECUnitNum;i++ )
	{
		StartNo = ECRgTab[i].ECFAds;
		EndNo = StartNo + 5*(ECDIGIT+1);
		if( ECRgTab[i].ECRgMode == 0 )			//电量包含费率
		{
			if(( ECFAds >= StartNo )&&( ECFAds < EndNo )&&((( ECFAds-StartNo)%(ECDIGIT+1)) == 0 )) break;			
		}
		else
		{
			if( StartNo == ECFAds ) break;		//不包含费率
		}		
	}	
	if( i == ECUnitNum ) return -1;
	else return i;
}	

void ECHEXAdd( unsigned char* Dest, unsigned char* Source )
{
	unsigned long value1;
	unsigned long value2;

#if ( ECDIGIT == BYTE4 )
	value1 = GetBuffToLong(Dest);
	value2 = GetBuffToLong(Source);
	value1+=value2;
	FillLongToBuff(Dest,value1);
#endif

}	

void ECBCDAdd( unsigned char* Dest, unsigned char* Source )
{
#if ( ECDIGIT == BYTE5 )
	_BCD5ADD( Dest, Source );	
#else
	_BCD4ADD( Dest, Source );	
#endif
}	

void ECBCDSub( unsigned char* Dest, unsigned char* Source )
{
#if ( ECDIGIT == BYTE5 )
	_BCD5SUB( Dest, Source );	
#else
	_BCD4SUB( Dest, Source );	
#endif
}
//检查RAM中的分时电量之和是否等于总
//Source: 存放电量的RAM区
//Type: 0：RAM区不包含每个电量的校验和； 1：每个电量后跟一个校验和。
short TotalFeeECCheck( unsigned char* Source, short Type )
{
	unsigned char Buff[8];
	unsigned char* Ptr;
	unsigned short Len;
	short i;
#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
	unsigned long Value1;
	unsigned long Value2;
#endif
	
	Ptr = Buff;
	
	if( Type == 0 ) Len = ECDIGIT;
	else Len = ECDIGIT + 1;
	
	RAM_Fill( Ptr, ECDIGIT );
	for( i=0;i<Total_Fee;i++ )
	{
		//ECBCDAdd( Ptr, Source+(i+1)*Len );
		ECHEXAdd( Ptr, Source+(i+1)*Len );//计算分费率之和
	}	
#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
#if ( ECDIGIT == BYTE5 )
	Value1 = BCD4_Long( Source+1 );
	Value2 = BCD4_Long( Ptr+1 );
#else
	Value1 = BCD4_Long( Source );
	Value2 = BCD4_Long( Ptr );
	//Value1 = GetBuffToLong(Source);
	//Value2 = GetBuffToLong( Ptr );
#endif
	if(( Value1 >= Value2 )&&(( Value1 - Value2 ) <= 4 )) return 0;		//总等于分时电量之和 
	else return 1;
#else
	if( Data_Comp( Ptr, Source, ECDIGIT ) != 0 ) return 1;		//总不等于分时电量之和 
	else return 0;
#endif
}	

//累加RAM中的分时电量
//Source: 存放电量的RAM区
//Type: 0：RAM区不包含每个电量的校验和； 1：每个电量后跟一个校验和。
void FeeECAdd( unsigned char* Source, short Type )
{
	unsigned short Len;
	short i;
	
	if( Type == 0 ) Len = ECDIGIT;
	else Len = ECDIGIT + 1;
	
	RAM_Fill( Source, ECDIGIT );
	for( i=0;i<Total_Fee;i++ )
	{
		ECBCDAdd( Source, Source+(i+1)*Len );
	}	
}
	
//总减RAM中的分时电量
//Source: 存放电量的RAM区
//Type: 0：RAM区不包含每个电量的校验和； 1：每个电量后跟一个校验和。
void FeeECSub( unsigned char* Source, short Type )
{
	unsigned char Buff[(ECDIGIT+1)*5];
	unsigned char* Ptr;
	unsigned short Len;
	short i;
	unsigned short FeeNo;

	unsigned char Buff2[ECDIGIT+1];					//11.06.13
	unsigned char* Point;							//11.06.13	

	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > Total_Fee )) return;				//当前费率号非法
	
	Ptr = Buff;
	Point = Buff2;									//11.06.13
	
	if( Type == 0 ) Len = ECDIGIT;
	else Len = ECDIGIT + 1;
	
	RAM_Write( Ptr, Source, (ECDIGIT+1)*5 );

	RAM_Fill( Point, Len );							//11.06.13
	for( i=1;i<(Total_Fee+1);i++ )								//11.06.13
	{												//11.06.13
		if( i == FeeNo ) continue;					//11.06.13
		ECBCDAdd( Point, Ptr+i*Len );				//11.06.13
	}												//11.06.13
	if( Data_Comp( Point, Ptr, Len ) > 0 ) return;	//11.06.13		//分时电量之和大于总电量，费率电量不修正。

	for( i=1;i<(Total_Fee+1);i++ )
	{
		if( i == FeeNo ) continue;
		ECBCDSub( Ptr, Ptr+i*Len );
	}	
	RAM_Write( Source+FeeNo*Len, Ptr, ECDIGIT );
}
/*
void ECRgSum( short ECItem, unsigned short Data )
{
	unsigned short ECFAds;
	unsigned char* ECRAds;
	unsigned char Buff[6];
	unsigned char Buff2[6];
	unsigned char* Ptr;
	unsigned char* Dest;
	unsigned short i;
	unsigned char ErrState=0;
	
	Ptr = Buff;
	Dest = Buff2;
	
	RAM_Fill( Ptr, 6 );
	Word_BCD( Ptr+1, Data );								//两位小数有效？
	ECRAds = ECRgTab[ECItem].ECRAds;
	ECFAds = ECRgTab[ECItem].ECFAds;
	for(i=0;i<5;i++)
    {
      	if(( i == 0 ) || ( i == SM.FeeNo ))
        {
			ECRAds += i*(ECDIGIT+1); 
			ECFAds += i*(ECDIGIT+1);
			
			E2P_RFM( Dest, ECFAds, ECDIGIT );
			if(( Flag.BatState & F_E2PCheck ) != 0 ) 		//铁电中电量校验和是否相等，铁电数据优先级高于RAM
			{
				if(( ChkNum( ECRAds, ECDIGIT ) == *(ECRAds+ECDIGIT) )&&( AllIsBCD( ECRAds, ECDIGIT ) == 0 ))
				{
					RAM_Write( Dest, ECRAds, ECDIGIT );		//铁电数据出错，用RAM数据，两者都错用铁电数据
				}
				else ErrState = 1;
			}	
			
			if( ErrState == 1 ) break;
			
#if ( ECDIGIT == BYTE5 )
			_BCD5ADD( Dest, Ptr );
#else
			_BCD4ADD( Dest, Ptr );
#endif
			RAM_Write( ECRAds, Dest, ECDIGIT );
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
			E2P_WFM( ECFAds, Dest, ECDIGIT );			
        }
        if( ECRgTab[ECItem].ECRgMode != 0 ) break;			//无费率电量
	}
}	
*/

//将当前脉冲电量加入铁电和RAM
void AddCurrentEC( unsigned char* ECRAds, unsigned short ECFAds, unsigned char* Source )
{
	unsigned char* TempPtr;
	unsigned short FeeNo;

	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > Total_Fee )) return;				//当前费率号非法
	
	ECBCDAdd( ECRAds, Source );								//取RAM内总电量为基准增加电量
	*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
	TempPtr = ECRAds+FeeNo*(ECDIGIT+1);						//取RAM内当前费率电量为基准增加电量
#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
	ECBCDAdd( TempPtr, Source+6 );
#else
	ECBCDAdd( TempPtr, Source );
#endif
	*(TempPtr+ECDIGIT) = ChkNum( TempPtr, ECDIGIT );
	E2P_WFM( ECFAds, ECRAds, ECDIGIT );						//做好校验后分别写入铁电
	E2P_WFM( ECFAds+FeeNo*(ECDIGIT+1), TempPtr, ECDIGIT );								
	
}	

//Source: 当前电量读出后在RAM区临时存放的首址
//AddData: 增加的电量
//Type: 0: 总出错 1：分时电量出错
short ModifyCurrentEC( unsigned char* ECRAds, unsigned short ECFAds, unsigned char* Source, unsigned char* AddData, short Type )
{
	unsigned short Addr;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned short FeeNo;
	
	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > Total_Fee)) return 1;					//当前费率号非法
	TempPtr1 = Source+ECDIGIT;
	for( i=1;i<(Total_Fee+1);i++ )
	{
		if( i == FeeNo )
		{
//			TempPtr1 += ECDIGIT+1;								//当前费率电量已正确，不再重读		
			TempPtr1 += ECDIGIT;								//当前费率电量已正确，不再重读		//10.05.12
			continue;
		}	
		E2P_RFM( TempPtr1, ECFAds+i*(ECDIGIT+1), ECDIGIT );		//读出其余各费率电量
		if( E2PECIsOK( TempPtr1 ) != 0 ) break;					//判断从铁电中读出是否合法	
//		TempPtr1 += ECDIGIT+1;																					
		TempPtr1 += ECDIGIT;									//10.05.12	
	}	
	if( Type == 0 ) 
	{
		TempPtr1 = Source;
		Addr = ECFAds;
	}
	else
	{
		TempPtr1 = Source + FeeNo*ECDIGIT;
		Addr = ECFAds+FeeNo*(ECDIGIT+1);		
	}		
	E2P_RFM( TempPtr1, Addr, ECDIGIT );							//再次读取铁电中的当前总电量或分时电量
//	if( E2PECIsOK( TempPtr1 ) == 0 ) 							//电量正确
	if(( E2PECIsOK( TempPtr1 ) == 0 )||( i != 5 )) 				//电量正确或无法修复以铁电为准
	{
		RAM_Write( ECRAds, Source, ECDIGIT );
		RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Source+FeeNo*ECDIGIT, ECDIGIT );
		AddCurrentEC( ECRAds, ECFAds, AddData );
	}	
	else if( i == (Total_Fee+1) )
	{
		if( Type == 0 ) FeeECAdd( Source, 0 );					//总等于分时电量之和
		else FeeECSub( Source, 0 );								//分时电量等于总减去其余分时电量			
		TempPtr1 = ECRAds;
		for( i=0;i<(Total_Fee+1);i++ )
		{
			RAM_Write( TempPtr1, Source+i*ECDIGIT, ECDIGIT );
			*(TempPtr1+ECDIGIT) = ChkNum( TempPtr1, ECDIGIT );								
			TempPtr1 += ECDIGIT+1; 
		}	
		AddCurrentEC( ECRAds, ECFAds, AddData );
	}
//	else return 1;	 

	return 0;
}	

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
short ECRgSum( short ECItem, unsigned short Data, unsigned short FData )
{
	unsigned short ECFAds;
	unsigned char* ECRAds;
	unsigned char Buff[Total_Fee*6];
//	unsigned char Buff2[6];
	unsigned char Buff2[12];
	unsigned char* Ptr;
	unsigned char* Source;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned char FStateZ=0;
	unsigned char FStateF=0;
	unsigned char RState=0;
	unsigned short FeeNo;
	
	Ptr = Buff;
	Source = Buff2;
	
	RAM_Fill( Ptr, Total_Fee*6 );
//	RAM_Fill( Source, 6 );
	RAM_Fill( Source, 12 );
	Word_BCD( Source+1, Data );											//两位小数有效？
	Word_BCD( Source+7, FData );											//两位小数有效？
	ECRAds = ECRgTab[ECItem].ECRAds;
	ECFAds = ECRgTab[ECItem].ECFAds;
	FeeNo = SM.FeeNo;

	if(( FeeNo < 1 )||( FeeNo > Total_Fee )) return 1;							//当前费率号非法
	
	if( ECRgTab[ECItem].ECRgMode == 0 )									//带费率电量
	{	
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    	//总电量是否合法	
		TempPtr1 = Ptr+FeeNo*ECDIGIT;
		E2P_RFM( TempPtr1, ECFAds+FeeNo*(ECDIGIT+1), ECDIGIT );
		if( E2PECIsOK( TempPtr1 ) != 0 ) FStateF = 1;                   //分时电量是否合法 		
		
		if(( FStateZ == 0 )&&( FStateF == 0 ))							//总和当前费率电量读铁电均正确
		{
			RAM_Write( ECRAds, Ptr, ECDIGIT );
			RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
			AddCurrentEC( ECRAds, ECFAds, Source );
		}	
		else															//读总电量和分时电量至少1个不成功
		{
			if( TotalFeeECCheck( ECRAds, 1 ) == 0 )						//判断RAM区的电量总是否等于分时电量之和
			{
				for( i=0;i<(Total_Fee+1);i++ )
				{
					if( RAMECIsOK( ECRAds+i*(ECDIGIT+1) ) != 0 )		//RAM区的各电量自检是否正确
					{
						RState = 1;										//有电量非法，校验不对或为非BCD码	
						break;	
					}	
				}	
			}	
			else RState = 1;											//RAM区电量有误，总与分时之和不等
			if( RState == 0 )											//RAM内电量数据均合法。
			{				
				AddCurrentEC( ECRAds, ECFAds, Source );							
			}
			else														//RAM内电量有误	
			{
				if(( FStateZ == 1 )&&( FStateF == 0 ))					//总错误、当前费率电量读铁电正确
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 0 );
				}
				else if(( FStateZ == 0 )&&( FStateF == 1 ))				//总正确和当前费率电量读铁电错误
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 1 );
				}
//				else return 1;		
				else 
				{
					ResumeEC( Ptr, ECRAds, ECFAds );					//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08
					ResumeEC( Ptr+FeeNo*ECDIGIT, ECRAds+FeeNo*(ECDIGIT+1), ECFAds+FeeNo*(ECDIGIT+1) );		//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08
					RAM_Write( ECRAds, Ptr, ECDIGIT );										//全错以铁电为准
					RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
					AddCurrentEC( ECRAds, ECFAds, Source );							
//					return 1;		
				}	
			}		
		}	
	}
	else
	{
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    		
		if( RAMECIsOK( ECRAds ) != 0 ) RState = 1;

		if(( FStateZ == 1 )&&( RState == 1 )) ResumeEC( Ptr, ECRAds, ECFAds );				//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08

//		if(( FStateZ == 0 )||( RState == 0 ))
//		{
//			if( FStateZ == 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//以铁电数据为准
			if( RState != 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//只要RAM不对均以铁电数据为准
			ECBCDAdd( ECRAds, Source );
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
			E2P_WFM( ECFAds, ECRAds, ECDIGIT );						
//		}	 		
//		else return 1;		
	}	
	return 0;
}	
#else
short ECRgSum( short ECItem, unsigned short Data )
{
	unsigned short ECFAds;
	unsigned char* ECRAds;
	unsigned char Buff[Total_Fee*6];
	unsigned char Buff2[6];
	unsigned char* Ptr;
	unsigned char* Source;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned char FStateZ=0;
	unsigned char FStateF=0;
	unsigned char RState=0;
	unsigned short FeeNo;
	
	Ptr = Buff;
	Source = Buff2;
	
	RAM_Fill( Ptr, Total_Fee*6 );
	RAM_Fill( Source, 6 );
	Word_BCD( Source+1, Data );											//两位小数有效？
	ECRAds = ECRgTab[ECItem].ECRAds;
	ECFAds = ECRgTab[ECItem].ECFAds;
	FeeNo = SM.FeeNo;

	if(( FeeNo < 1 )||( FeeNo > Total_Fee )) return 1;							//当前费率号非法
	
	if( ECRgTab[ECItem].ECRgMode == 0 )									//带费率电量
	{	
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    	//总电量是否合法	
		TempPtr1 = Ptr+FeeNo*ECDIGIT;
		E2P_RFM( TempPtr1, ECFAds+FeeNo*(ECDIGIT+1), ECDIGIT );
		if( E2PECIsOK( TempPtr1 ) != 0 ) FStateF = 1;                   //分时电量是否合法 		
		
		if(( FStateZ == 0 )&&( FStateF == 0 ))							//总和当前费率电量读铁电均正确
		{
			RAM_Write( ECRAds, Ptr, ECDIGIT );
			RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
			AddCurrentEC( ECRAds, ECFAds, Source );
		}	
		else															//读总电量和分时电量至少1个不成功
		{
			if( TotalFeeECCheck( ECRAds, 1 ) == 0 )						//判断RAM区的电量总是否等于分时电量之和
			{
				for( i=0;i<(Total_Fee+1);i++ )
				{
					if( RAMECIsOK( ECRAds+i*(ECDIGIT+1) ) != 0 )		//RAM区的各电量自检是否正确
					{
						RState = 1;										//有电量非法，校验不对或为非BCD码	
						break;	
					}	
				}	
			}	
			else RState = 1;											//RAM区电量有误，总与分时之和不等
			if( RState == 0 )											//RAM内电量数据均合法。
			{				
				AddCurrentEC( ECRAds, ECFAds, Source );							
			}
			else														//RAM内电量有误	
			{
				if(( FStateZ == 1 )&&( FStateF == 0 ))					//总错误、当前费率电量读铁电正确
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 0 );
				}
				else if(( FStateZ == 0 )&&( FStateF == 1 ))				//总正确和当前费率电量读铁电错误
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 1 );
				}
//				else return 1;		
				else 
				{
					ResumeEC( Ptr, ECRAds, ECFAds );					//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08
					ResumeEC( Ptr+FeeNo*ECDIGIT, ECRAds+FeeNo*(ECDIGIT+1), ECFAds+FeeNo*(ECDIGIT+1) );		//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08
					RAM_Write( ECRAds, Ptr, ECDIGIT );										//全错以铁电为准
					RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
					AddCurrentEC( ECRAds, ECFAds, Source );							
//					return 1;		
				}	
			}		
		}	
	}
	else
	{
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    		
		if( RAMECIsOK( ECRAds ) != 0 ) RState = 1;

		if(( FStateZ == 1 )&&( RState == 1 )) ResumeEC( Ptr, ECRAds, ECFAds );				//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08

//		if(( FStateZ == 0 )||( RState == 0 ))
//		{
//			if( FStateZ == 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//以铁电数据为准
			if( RState != 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//只要RAM不对均以铁电数据为准
			ECBCDAdd( ECRAds, Source );
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
			E2P_WFM( ECFAds, ECRAds, ECDIGIT );						
//		}	 		
//		else return 1;		
	}	
	return 0;
}	
#endif

//根据EEPROM中保存的值进行数据恢复
void ResumeEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds )
{
	short i;
	short ECRgTabRowNo; 
	unsigned char Buff[Total_Fee*6];
	unsigned char* Ptr;
	unsigned char Buff2[10];
	unsigned char* Point;
	unsigned short Addr;
	unsigned char State=0;
	unsigned char* TempPtr;
	
	Ptr = Buff;
	Point = Buff2;
	
	ECRgTabRowNo = GetECRgTabRowNo( ECFAds );
	if( ECRgTabRowNo == -1 ) return;									//正常FRAM电量地址
                                                                		
	Addr = ECRgTab[ECRgTabRowNo].ECEAds;								//取对应的EEPROM中电量的地址
	if( ECRgTab[ECRgTabRowNo].ECRgMode == 0 )							//电量包含费率
	{                                                           		
		TempPtr = Ptr;                                          		
		for( i=0;i<(Total_Fee+1);i++ )												//读取EEPROM中的总及各费率电量	
		{                                                       		
			E2P_RData( TempPtr, Addr, ECDIGIT );	            		
			if( E2PECIsOK( TempPtr ) != 0 )                     		
			{                                                   		
				State = 1;												//EEPROM数据有错	
				break;                                          		
			}	                                                		
			TempPtr += ECDIGIT;                                 		
			Addr += ECDIGIT+1;                                  		
		}	                                                    		
		if( State == 0 )                                        		
		{                                                       		
			if( TotalFeeECCheck( Ptr, 0 ) != 0 ) State = 1;				//总不等于分时电量之和 
		}	
		E2P_RFM( Point, ECFAds, ECDIGIT );								//再次读取FRAM数据。	
		if( E2PECIsOK( Point ) == 0 ) 
		{
			RAM_Write( Dest, Point, ECDIGIT );							//铁电中电量读取正确，以铁电电量为准，放弃从RRPROM中取得的数据
			RAM_Write( ECRAds, Dest, ECDIGIT );							//RAM中的值恢复为正确的铁电数据或备份EEPROM数据
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
		}	
		else
		{																//铁电数据还是有误	
			if( State == 0 ) 											//EEPROM数据正确，包括校验和、BCD码、总等于分时费率之和
			{
				TempPtr = ECRgTab[ECRgTabRowNo].ECRAds;
				Addr = ECRgTab[ECRgTabRowNo].ECFAds;					//取铁电中电量首址
				RAM_Write( Dest, Ptr+(((ECFAds-Addr)/(ECDIGIT+1))*ECDIGIT), ECDIGIT );	//恢复电量后需有当前项返回值 //10.12.08					
				for( i=0;i<(Total_Fee+1);i++ )
				{
					E2P_WFM( Addr, Ptr+i*ECDIGIT, ECDIGIT );			//将EEPROM中的备份数据替换掉铁电中的错误电量，但可能导致电量减少。	
					RAM_Write( TempPtr, Ptr+i*ECDIGIT, ECDIGIT );
					*(TempPtr+ECDIGIT) = ChkNum( TempPtr, ECDIGIT );
					TempPtr += ECDIGIT+1;
					Addr += ECDIGIT+1;
				}	
			}	
			else 
			{
				RAM_Write( Dest, Point, ECDIGIT );						//铁电、EEPROM数据均不对，以最近一次读取的铁电数据返回，但不刷新RAM数据
				return;					
			}	
		}	
	}
	else
	{																	//无费率电量
		E2P_RData( Ptr, Addr, ECDIGIT );	
		if( E2PECIsOK( Ptr ) != 0 ) 
		{
			State = 1;													//EEPROM中数据自检错误
		}	
		E2P_RFM( Point, ECFAds, ECDIGIT );								//再次读取FRAM数据。	
		if( E2PECIsOK( Point ) == 0 ) 
		{
			RAM_Write( Ptr, Point, ECDIGIT );							//铁电中电量读取正确，以铁电电量为准，放弃从RRPROM中取得的数据
		}	
		else
		{																//铁电数据还是有误	
			if( State == 0 ) E2P_WFM( ECFAds, Ptr, ECDIGIT );			//将EEPROM中的备份数据替换掉铁电中的错误电量，可能导致电量减少。		
			else 
			{
				RAM_Write( Dest, Point, ECDIGIT );						//铁电、EEPROM数据均不对，以最近一次读取的铁电数据返回，但不刷新RAM数据
				return;					
			}	
		}	
		RAM_Write( Dest, Ptr, ECDIGIT );								//返回正确的铁电数据或备份EEPROM数据
		RAM_Write( ECRAds, Dest, ECDIGIT );								//RAM中的值恢复为正确的铁电数据或备份EEPROM数据
		*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
	}		
	return;	
}	

#if( ECDisDecDigitsValid ==	YesCheck )		//13.03.03 当前电量显示小数位真实有效
//添加0.01kWh以下尾数
void AddECMantissa( unsigned char* Dest, unsigned char* ECRAds )
{
	short i;
	unsigned short* ECRamBufAdsPtr;
	unsigned char* ECRamBufChkAdsPtr;
	unsigned long Value;
	unsigned long PS32;					//新国网		//13.08.30	
	unsigned char Temp1;
	unsigned char Temp2;
	
	for( i=0;i<40;i++ )					//14.09.03
	{
		if( ECDisDigitRgTab[i].ECRAds == ECRAds ) break;	
	}	
	if( i == 40 ) return;				//14.09.03	
	
	PS32 = 10;//MSpec.RMeterConst / 100;		//新国网		//13.08.30	

	ECRamBufAdsPtr = ECDisDigitRgTab[i].ECRamBufAds;
	ECRamBufChkAdsPtr = ECDisDigitRgTab[i].ECRamBufChkAds;
	if( *ECRamBufChkAdsPtr != ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 ) ) return;
//	Value = *ECRamBufAdsPtr;
//	Value *= 100;
//	Value /= PS32;
//	if( Value >= 10 )
//	{
//		Temp1 = Value%10;
//		Temp2 = Value/10;
//		if( Temp2 != 0 ) 
//		{
			Temp2 = *ECRamBufAdsPtr;
		//	Value = BCD4_Long( Dest+1 );
		Value = BCD4_Long( Dest );
		Value /= PS32;
			Value += Temp2;
			Long_BCD4( Dest+1, Value );
//		}	
//	}else Temp1 = Value;	
	
//	*Dest = Byte_BCD( Temp1 );
	
}	
#endif

//读取RAM或FRAM中的电量
void GetRamEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds ) 
{
	
	if( RAMECIsOK( ECRAds ) == 0 )
	{
		RAM_Write( Dest, ECRAds, ECDIGIT );					//RAM中电量校验和相等，提取RAM电量。
#if( ECDisDecDigitsValid ==	YesCheck )			//13.03.03 当前电量显示小数位真实有效
		if( Disk.DigitsValidFlg == 0x55) AddECMantissa( Dest, ECRAds ); 	//显示时才添加0.01kWh以下尾数
#endif
	}
	else 
	{
		E2P_RFM( Dest, ECFAds, ECDIGIT );					//RAM中校验和错误，读取FRAM数据。	
		if( E2PECIsOK( Dest ) == 0 ) 
		{
			RAM_Write( ECRAds, Dest, ECDIGIT );				//RAM中的值恢复为FRAM中的值
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
		}
		else
		{
			if((Flag.Power & F_PwrUp) != 0) 				//停电状态下不作数据恢复	
			{
				ResumeEC( Dest, ECRAds, ECFAds );
			}	
		}		
	}	
}	
void ECSave( unsigned short StartNo, unsigned short EndNo )
{
	unsigned char i,j;
	unsigned short EAddr;
	unsigned short FAddr;
	unsigned char* RAds;
	for( j=StartNo;j<EndNo;j++ )
		{
		FAddr = ECRgTab[j].ECFAds;		//取对应的FRAM中电量的地址
		RAds = ECRgTab[j].ECRAds;;		//取对应的RAM中电量的地址
		EAddr = ECRgTab[j].ECEAds;		//取对应的E2P中电量的地址
		for( i=0;i<(Total_Fee+1);i++ )
			{//备份总及分费率电量
			E2P_WData( EAddr, RAds, ECDIGIT );
			E2P_WFM( FAddr, RAds, ECDIGIT );
			EAddr += ECDIGIT+1;
			FAddr += ECDIGIT+1;
			RAds += ECDIGIT+1;
			}
		}

}

//向EEPROM中保存的电量值
void ECSaveToE2P( unsigned short StartNo, unsigned short EndNo )
{
	short i,j;
	unsigned char Buff[Total_Fee*6];
	unsigned char* Ptr;
//	unsigned char Buff2[10];
//	unsigned char* Point;
	unsigned short EAddr;
	unsigned short FAddr;
	unsigned char FState;
	unsigned char RState;
	unsigned char* RAds;
	unsigned char* TempPtr;
	
	Ptr = Buff;
//	Point = Buff2;
	
	for( j=StartNo;j<EndNo;j++ )
	{
		FState=0;
		RState=0;
		FAddr = ECRgTab[j].ECFAds;											//取对应的FRAM中电量的地址
		RAds = ECRgTab[j].ECRAds;;											//取对应的RAM中电量的地址
		if( ECRgTab[j].ECRgMode == 0 )										//电量包含费率
		{
			TempPtr = Ptr;
			for( i=0;i<(Total_Fee+1);i++ )									
			{
				E2P_RFM( TempPtr, FAddr, ECDIGIT );	//取对应的FRAM中电量
				if( E2PECIsOK( TempPtr ) != 0 ) 
				{
					FState = 1;												//FRAM数据有错	
					break;
				}	
				TempPtr += ECDIGIT;
				FAddr += ECDIGIT+1;
			}	
			if( FState == 0 )
			{
				if( TotalFeeECCheck( Ptr, 0 ) != 0 ) FState = 1;			//判断FRAM总电量是否等于分时电量之和 
			}	
			TempPtr = RAds;
			for( i=0;i<(Total_Fee+1);i++ )									//判断RAM中的总及各费率电量	
			{
				if( RAMECIsOK( TempPtr ) != 0 ) 
				{
					RState = 1;												//RAM数据有错	
					break;
				}	
				TempPtr += ECDIGIT+1;
			}	
			if( RState == 0 )
			{
				if( TotalFeeECCheck( RAds, 1 ) != 0 ) RState = 1;			//总不等于分时电量之和 
			}	
			if(( FState == 0 )&&( RState == 0 ))							//若铁电数据和RAM数据不一致，以铁电为准
			{
				for( i=0;i<(Total_Fee+1);i++ )
				{
					if( Data_Comp( Ptr+i*ECDIGIT, RAds+i*(ECDIGIT+1), ECDIGIT ) != 0 ) { RState = 1; break; }	
				}	
			}	

			if(( FState == 0 )&&( RState == 1 ))							//铁电准确，RAM错误，用铁电数据覆盖RAM
			{
				TempPtr = RAds;
				for( i=0;i<(Total_Fee+1);i++ )
				{
					RAM_Write( TempPtr, Ptr+i*ECDIGIT, ECDIGIT );
					*(TempPtr+ECDIGIT) = ChkNum( TempPtr, ECDIGIT );
					TempPtr += ECDIGIT+1;
				}					
			}
			else if(( FState == 1 )&&( RState == 0 ))						//铁电错误，RAM正确，用RAM数据覆盖铁电
			{
//				FAddr = ECRgTab[j].ECFAds;									//取对应的FRAM中电量的地址
//				TempPtr = ECRgTab[j].ECRAds;								//取对应的RAM中电量的地址
//				for( i=0;i<5;i++ )
//				{
//					E2P_WFM( FAddr, TempPtr, ECDIGIT );
//					TempPtr += ECDIGIT+1;
//					FAddr += ECDIGIT+1;					
//				}					
			}
			else if(( FState == 1 )&&( RState == 1 )) return;				//FRAM和RAM均出错，不保存电量	
			
			EAddr = ECRgTab[j].ECEAds;										//取对应的EEPROM中电量的地址
			TempPtr = ECRgTab[j].ECRAds;;									//取对应的RAM中电量的地址
			for( i=0;i<(Total_Fee+1);i++ )												//向EEPROM中写入总及各费率电量	
			{
				E2P_WData( EAddr, TempPtr, ECDIGIT );
				TempPtr += ECDIGIT+1;
				EAddr += ECDIGIT+1;
			}	
		}
		else
		{
			E2P_RFM( Ptr, FAddr, ECDIGIT );									//读铁电数据
			if( E2PECIsOK( Ptr ) != 0 ) FState = 1;							//FRAM数据有错	
			if( RAMECIsOK( RAds ) != 0 ) RState = 1;						//RAM数据有错	
			if(( FState == 0 )&&( RState == 0 ))
			{
				if( Data_Comp( Ptr, RAds, ECDIGIT ) != 0 ) RState = 1;		//以铁电数据为准
			}	
			
			if(( FState == 0 )&&( RState == 1 ))							//铁电准确，RAM错误，用铁电数据覆盖RAM
			{
				RAM_Write( RAds, Ptr, ECDIGIT );
				*(RAds+ECDIGIT) = ChkNum( RAds, ECDIGIT );
			}
			else if(( FState == 1 )&&( RState == 0 ))						//铁电错误，RAM正确，用RAM数据覆盖铁电
			{
//				FAddr = ECRgTab[j].ECFAds;									//取对应的FRAM中电量的地址
//				RAds = ECRgTab[j].ECRAds;									//取对应的RAM中电量的地址
//				E2P_WFM( FAddr, RAds, ECDIGIT );
			}
			else if(( FState == 1 )&&( RState == 1 )) return;				//FRAM和RAM均出错，不保存电量	
			
			EAddr = ECRgTab[j].ECEAds;										//取对应的EEPROM中电量的地址
			E2P_WData( EAddr, RAds, ECDIGIT );
		}	
	}	
}		
/*
void ConsumECSumSaveToE2P( void )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char FState=0;
	unsigned char RState=0;
	
	Ptr = Buff;
	
	E2P_RFM( Ptr, FConsumECSumBCD, 8 );										//读铁电数据
	if(( Flag.BatState & F_E2PCheck ) != 0 ) FState = 1;					//FRAM数据有错	
	if( ChkNum( ICEC.RConsumECSumBCD, 8 ) != ICEC.RConsumECSumBCDChk ) RState = 1;
	if(( FState == 0 )&&( RState == 0 ))
	{
		if( Data_Comp( Ptr, ICEC.RConsumECSumBCD, 8 ) != 0 ) RState = 1;	//以铁电数据为准
	}	
	
	if(( FState == 0 )&&( RState == 1 ))									//铁电准确，RAM错误，用铁电数据覆盖RAM
	{
		RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
		ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
	}
	else if(( FState == 1 )&&( RState == 1 )) return;						//FRAM和RAM均出错，不保存累计用电金额	
	
	E2P_WData( ConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );
}
*/
void SaveECBakTime( void )
{
//	unsigned char Buff[6];	
//	unsigned char* Ptr;
	
//	Ptr = Buff;
	
//	RAM_Write( Ptr, Clk.Sec, 6 );
	E2P_WData( ECBakTime, (unsigned char*)&Clk.YearH, 12 );
	
}
void Clr_E2PData( unsigned short Addr, unsigned short DataLenth, unsigned short Cylcle )
{
  	unsigned char Buff[130];
  	unsigned char* Point;
  	unsigned short i,Len,DestAddr;
  	unsigned short CylcleCnt;
  	
  	if( Addr >= (E2P_PGLEN * 2) )
  	{
  		DataLenth = DataLenth * Cylcle;
  		while( DataLenth > 0 )
  		{
	  		if( DataLenth >= 130 ) Len = 130;	
  			else Len = DataLenth;	
			RAM_Fill( Buff, Len );  			

//			WDTCTL = WDT_ARST_1000;
			HT_FreeDog();
  			E2Pwrite( Addr, Buff, Len, DataEAds );
  			Addr += Len;
  			DataLenth -= Len;
  		}		  		
  	}	
  	else
  	{  	  	
		CylcleCnt = 0;
		while( CylcleCnt < Cylcle )
		{ 
			Len = 0;
			for( i=CylcleCnt;i<Cylcle;i++ )
			{
				Len += DataLenth + 1;
				if( Len >= 130 ) break; 			
			}	
			if( i == Cylcle ) CylcleCnt = Cylcle;
			else 
			{
				Len = Len - (DataLenth + 1);		
				CylcleCnt = i;
			}	
    
  	  		Point = Buff;
			RAM_Fill( Point, Len );
			for( i=0;i<Len; )
			{
				Point += DataLenth;
				*Point = 0xA5;
				Point += 1;
				i += DataLenth + 1;	
				if( Point >= ( Buff+130 )) break;
			}	
    
			DestAddr = Addr;
			for( i=0;i<2;i++ )
			{		
//				WDTCTL = WDT_ARST_1000;
				HT_FreeDog();
//				_E2Pwrite( DestAddr, Buff, Len, DataEAds );
				E2Pwrite( DestAddr, Buff, Len, DataEAds );
				DestAddr += E2P_PGLEN;		
			}	
//			WDTCTL = WDT_ARST_1000;
			HT_FreeDog();
			Addr += Len;
		}
	}
}	

void Clr_FRAMData( unsigned short Addr, unsigned short DataLenth, unsigned short Cylcle )
{
  	unsigned char Buff[130];
  	unsigned char* Point;
  	unsigned short i,Len,DestAddr;
  	unsigned short CylcleCnt;
  	
	CylcleCnt = 0;
	while( CylcleCnt < Cylcle )
	{ 
		Len = 0;
		for( i=CylcleCnt;i<Cylcle;i++ )
		{
			Len += DataLenth + 1;
			if( Len >= 130 ) break; 			
		}	
		if( i == Cylcle ) CylcleCnt = Cylcle;
		else 
		{
			Len = Len - (DataLenth + 1);		
			CylcleCnt = i;
		}	

  		Point = Buff;
		RAM_Fill( Point, Len );
		for( i=0;i<Len; )
		{
			Point += DataLenth;
			*Point = 0xA5;
			Point += 1;
			i += DataLenth + 1;	
			if( Point >= ( Buff+130 )) break;
		}	

		DestAddr = Addr;
		for( i=0;i<2;i++ )
		{		
//			WDTCTL = WDT_ARST_1000;
			HT_FreeDog();
//			_E2Pwrite( DestAddr, Buff, Len, DataEAds );
			E2Pwrite( DestAddr, Buff, Len, FMAds );
//			DestAddr += 0x400;		
			DestAddr += FRAM_PGLEN;		
		}	
//		WDTCTL = WDT_ARST_1000;
		HT_FreeDog();
		Addr += Len;
	}
}

void ECReserveCycle( void )
{
	switch( SM.ECReserveState )	
	{
		case ECReserveIdle:								// 电量备份空闲
			SM.ECReserveStateChk = ECReserveIdle;			
			break;
		case ECReserveStep1:							// 电量备份步骤1、合相电量
			if( SM.ECReserveStateChk != ( SM.ECReserveState +0xA5A5 ) ) break;
			SaveECBakTime();
        //	ConsumECSumSaveToE2P();
		//	ECSaveToE2P( 0, 5 );
			ECSave( 0, 5 );
#if(MeterPhase==Threephase)
			SM.ECReserveState = ECReserveStep2;
			SM.ECReserveStateChk = SM.ECReserveState + 0xA5A5;
#else
			SM.ECReserveState = ECReserveIdle;
			SM.ECReserveStateChk = ECReserveIdle;			
#endif
			break;
			
		case ECReserveStep2:							// 电量备份步骤2、A相电量
			if( SM.ECReserveStateChk != ( SM.ECReserveState +0xA5A5 ) ) break;
			//ECSaveToE2P( 5, ECUnitNum );
			ECSave( 5, ECUnitNum );
			SM.ECReserveState = ECReserveIdle;
			SM.ECReserveStateChk = SM.ECReserveState + 0xA5A5;
			break;
			/*
		case ECReserveStep3:							// 电量备份步骤3、B相电量
			if( SM.ECReserveStateChk != ( SM.ECReserveState +0xA5A5 ) ) break;
			ECSaveToE2P( 30, 38 );
			SM.ECReserveState = ECReserveStep4;
			SM.ECReserveStateChk = SM.ECReserveState + 0xA5A5;
			break;
		case ECReserveStep4:							// 电量备份步骤4、C相电量
			if( SM.ECReserveStateChk != ( SM.ECReserveState +0xA5A5 ) ) break;
			ECSaveToE2P( 45, 53 );
			SM.ECReserveState = ECReserveIdle;
			SM.ECReserveStateChk = SM.ECReserveState + 0xA5A5;
			break;
			*/
		default: 
			SM.ECReserveState = ECReserveIdle;
			SM.ECReserveStateChk = ECReserveIdle;			
			break;	
	}
}


