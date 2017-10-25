#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"

#include "RsComm.h"
#include "interrupt.h"
#include "Mem.h"
#include "Time.h"
#include "Power.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
//#include "Produce.h"
#include "cpucard.h"
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif

//取Class7类数据
//short GetParameterClassData( unsigned char* Dest, unsigned char* Source ) 
short GetParameterClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 	//17.03.24
{
	short Len;

	RAM_Write( Dest, Source, 4 );

//	Len = GetParameterClassDataTab( Dest+4, Source );
	Len = GetParameterClassDataTab( COM_No, Dest+4, Source );
	if( Len == -1 ) 
	{
		*(Dest+4) = Get_data_access_result;						//异常标志
		*(Dest+5) = object_undefined; 						//无对应的OBIS码
		Len = 2;
	}
	else if( Len == -2 )										//17.04.09
	{
		*(Dest+4) = Get_data_access_result;						//异常标志
		*(Dest+5) = read_write_denied; 							//操作权限不足
		Len = 2;
	}	
	Len += 4;
	return Len;		
}		

//取PhaseEvent类数据
//short GetParameterClassDataTab( unsigned char* Dest, unsigned char* Source ) 
short GetParameterClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short i;
//	unsigned char* Addr;
	short Len;
//	unsigned short OICode;
//	short Temp;
//	short Start,End;
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
//	unsigned char* Ptr;
	unsigned char* OAD;
	Len = -1;
	
	OAD = Source;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
				
	i = GetParameterClassTabItemNo( OAD );
	if( i != DL698_ClassId8Sum )
	{
		if(( Attribute >= 1 )&&( Attribute <= 5 ))			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_ClassId8_Tab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09
		}
		*Dest = Get_data;								//正常数据返回	
//		Attribute = *(Source+2+6);						//请求的属性
	
		switch( Attribute )
		{
			case 1:										//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x02;
				RAM_Write( Dest+3, OAD, 2 );
				Len = 5;
				break;	
			case 2:										//关联对象属性表	
//				Len = GetClass8Data( Dest, OAD );
				Len = GetClass8Data( COM_No, Dest, OAD );	//17.03.24
				break;
			case 3:										//关联对象属性表	
//				Len = GetClass8Data( Dest, OAD );
				Len = GetClass8Data( COM_No, Dest, OAD );	//17.03.24
				break;
			case 4:										//关联对象属性表	
//				Len = GetClass8Data( Dest, OAD );
				Len = GetClass8Data( COM_No, Dest, OAD );	//17.03.24
			case 5:										//关联对象属性表	
//				Len = GetClass8Data( Dest, OAD );
				Len = GetClass8Data( COM_No, Dest, OAD );	//17.03.24
				break;
			default:
				break;	
		}	
	}		
	return Len;
}	

unsigned short GetParameterClassTabItemNo( unsigned char* OAD )
{
	unsigned short i;
	unsigned char Buff[4];
	unsigned char* NewOAD;
	
	NewOAD = Buff;
	RAM_Write( NewOAD, OAD, 4 );

//节假日、时区、时段、阶梯、费率电价等需特殊处理		
	if(( *NewOAD == 0x40 )&&((( *(NewOAD+1) >= 0x14 )&&( *(NewOAD+1) <= 0x1B ))||( *(NewOAD+1) == 0x11 ))) *(NewOAD+3) = 0x00;	
	if(( *NewOAD == 0x33 )&&( *(NewOAD+1) == 0x20 )&&( *(NewOAD+2) == 0x02 )) *(NewOAD+3) = 0x00;		//17.05.19
//	if(( *NewOAD == 0xF1 )&&( *(NewOAD+1) == 0x01 )&&( *(NewOAD+2) == 0x03 )) *(NewOAD+3) = 0x00;		//显式安全列表特殊处理
	for( i=0;i<DL698_ClassId8Sum;i++ )
	{
		if( Data_Comp( NewOAD, (unsigned char*)DL698_ClassId8_Tab[i].OAD, 4 ) == 0 ) break;				//查找对应的OBIS码
	}	
	return i;	
}

short GetZoneSegItemStructure( unsigned char* Dest, unsigned char* Source )
{
	*Dest = Data_structure;
	*(Dest+1) = 3;
	*(Dest+2) = D_Unsigned8;
	*(Dest+3) = BCD_Byte(*(Source+2));				//月
	*(Dest+4) = D_Unsigned8;
	*(Dest+5) = BCD_Byte(*(Source+1));				//日	
	*(Dest+6) = D_Unsigned8;	
//	*(Dest+7) = *(Source);							//时段表号
	*(Dest+7) = BCD_Byte(*Source);					//时段表号、时（年阶梯）	//17.05.12

	return 8;
}	

short GetSaveDayItemStructure( unsigned char* Dest, unsigned short Addr )
{
	unsigned char Buff[4];
	unsigned char* Point;
	
	Point = Buff;
	E2P_RData( Point, Addr, 2 );
	*Dest = Data_structure;
	*(Dest+1) = 2;
	*(Dest+2) = D_Unsigned8;
	*(Dest+3) = BCD_Byte(*(Point+1)) ;		//月
	*(Dest+4) = D_Unsigned8;
	*(Dest+5) = BCD_Byte(*Point);			//日	
	
	return 6;
}

short GetPositionItemStructure( unsigned char* Dest, unsigned short Addr )
{
	unsigned char Buff[10];
	unsigned char* Point;
	
	Point = Buff;
	E2P_RData( Point, Addr, 4 );
	*Dest = Data_structure;
	*(Dest+1) = 4;
	*(Dest+2) = D_ENUM;
	*(Dest+3) = *Point;
	*(Dest+4) = D_Unsigned8;
	*(Dest+5) = *(Point+1) ;				//度
	*(Dest+6) = D_Unsigned8;
	*(Dest+7) = *(Point+2);					//分	
	*(Dest+8) = D_Unsigned8;
	*(Dest+9) = *(Point+3);					//秒	
	
	return 10;
}

#if ( LocalSmartMeter == YesCheck )
unsigned short GetStepArray( unsigned char* Dest, unsigned short Addr, unsigned short Items )
{
	short i;
	unsigned short Len=2;

	*Dest = Data_array;
	*(Dest+1) = Items;
	Dest += 2;
	for( i=0;i<Items;i++ )
	{	
		*Dest = D_Unsigned32;
		E2P_RData( Dest+1, Addr, 4 );
		LongToSwapChar( Dest+1, BCD4_Long( Dest+1 ) );
		Dest += 5;
		Len += 5;
		Addr += 5;
	}				
	return Len;	
}	

unsigned short GetStepSaveDayArray( unsigned char* Dest, unsigned short Addr )
{
	unsigned char Buff[4];
	short i;
	unsigned short Len=2;
	unsigned short Length;

	*Dest = Data_array;
	*(Dest+1) = 4;
	Dest += 2;
	for( i=0;i<4;i++ )
	{	
		E2P_RData( Buff, Addr, 3 );
		Length = GetZoneSegItemStructure( Dest, Buff );							
		Len += Length;
		Dest += Length;
		Addr += 4;
	}	
    return Len;
}	
#endif

//short GetSpecClassId8( unsigned char* Dest, unsigned char* OAD )	
short GetSpecClassId8( short COM_No, unsigned char* Dest, unsigned char* OAD )	//17.03.24
{
	short i,j;
	unsigned short Addr;
	short Len=0;
	short Temp;
//	short Start,End;
//	unsigned char DataType;
//	unsigned char Element_Len;			
	unsigned char* Ptr;
//	unsigned short ComInfo;
//	unsigned long ComID;
	unsigned char Buff[4];
	unsigned char Buff2[50];
	unsigned char* NewOAD;
	unsigned char* Point;
	unsigned char Sum1,Sum2;
	short Length;
	
	
	NewOAD = Buff;
	Point = Buff2;
	RAM_Write( NewOAD, OAD, 4 );
//	ComID = SwapCharToLong( OAD );

	if(( *NewOAD == 0x40 )&&((( *(NewOAD+1) >= 0x14 )&&( *(NewOAD+1) <= 0x1B ))||( *(NewOAD+1) == 0x11 ))) 	
	{
		switch( *(NewOAD+1) )	
		{
			case 0x11:						//节假日
				E2P_RData( (unsigned char*)&Sum1, HolidaySum, 1 );
				if( Sum1 > HOLIDAYNUMBER ) Sum1 = HOLIDAYNUMBER;			//17.04.21
				if( *(NewOAD+3) == 0 )
				{
					*Dest = Data_array;
					*(Dest+1) = Sum1;
					Ptr = Dest+2;
					Len += 2;
					for( i=0;i<Sum1;i++ )
					{	
						*(NewOAD+3) = i+1;
//						Length = GetSpecClassId8( Ptr, NewOAD );	
						Length = GetSpecClassId8( COM_No, Ptr, NewOAD );	//17.03.24
						Ptr += Length;
						Len += Length;
					}				
				}
				else if( *(NewOAD+3) <= Sum1 )
				{
					Ptr = Dest;
					*Ptr = Data_structure;
					*(Ptr+1) = 2;
					Addr = *(NewOAD+3) - 1;
					Addr *= 7;
					Addr += Holiday1;
					E2P_RData( Point, Addr, 6 );
					*(Ptr+2) = D_date;
					RAM_Write( Ptr+3, Point, 5 );
					*(Ptr+8) = D_Unsigned8;	
					*(Ptr+9) = *(Point+5);
					Len = 10;
				}
				else Len = -1;	
				break;	
			case 0x14:						//当前套时区
			case 0x15:						//备用套时区
				E2P_RData( (unsigned char*)&Sum1, ZoneSum, 1 );
				if( Sum1 > ZONENUMBER ) Sum1 = ZONENUMBER;
				if( *(NewOAD+1) == 0x14 ) Addr = ESeason1;
				else Addr = ESeason2;
				if( *(NewOAD+3) == 0 )
				{
					*Dest = Data_array;
					*(Dest+1) = Sum1;
					Ptr = Dest+2;
					Len += 2;
					E2P_RData( Point, Addr, ZONENUMBER*3 );
					for( i=0;i<Sum1;i++ )
					{	
						Temp = i*3;
						GetZoneSegItemStructure( Ptr, Point+Temp );
						Ptr += 8;
						Len += 8;
					}				
				}
				else if( *(NewOAD+3) <= Sum1 )
				{
					Ptr = Dest;
					Temp = *(NewOAD+3) - 1;
					Temp *= 3;					
					E2P_RData( Point, Addr, ZONENUMBER*3 );
					GetZoneSegItemStructure( Ptr, Point+Temp );
					Len = 8;
				}
				else Len = -1;	
				break;	
			case 0x16:						//当前套时段表
			case 0x17:						//备用套时段表
				E2P_RData( (unsigned char*)&Sum1, SegTabSum, 1 );
				if( Sum1 > 8 ) Sum1 = 8;
				E2P_RData( (unsigned char*)&Sum2, SegSum, 1 );
				if( Sum2 > SEGNUMBER ) Sum2 = SEGNUMBER;
				if( *(NewOAD+1) == 0x16 ) Addr = Period1_Tab1;
				else Addr = Period2_Tab1;
				if( *(NewOAD+3) == 0 )
				{
					*Dest = Data_array;
					*(Dest+1) = Sum1;
					Ptr = Dest+2;
					Len += 2;
					for( j=0;j<Sum1;j++ )
					{
						*Ptr = Data_array;
						*(Ptr+1) = Sum2;
						Ptr += 2;
						Len += 2;
						E2P_RData( Point, Addr+j*(SEGNUMBER*3+1), SEGNUMBER*3 );
						for( i=0;i<Sum2;i++ )
						{	
							Temp = i*3;
							GetZoneSegItemStructure( Ptr, Point+Temp );
							Ptr += 8;
							Len += 8;
						}
					}					
				}
				else if( *(NewOAD+3) <= Sum1 )
				{
					Ptr = Dest;
					*Ptr = Data_array;
					*(Ptr+1) = Sum2;
                    Ptr += 2;
					Len += 2;
					Temp = *(NewOAD+3) - 1;
					Addr += Temp * (SEGNUMBER*3+1); 
					E2P_RData( Point, Addr, SEGNUMBER*3 );
					for( i=0;i<Sum2;i++ )
					{	
						Temp = i*3;
						GetZoneSegItemStructure( Ptr, Point+Temp );
						Ptr += 8;
						Len += 8;
					}
				}
				else Len = -1;	
				break;
#if ( LocalSmartMeter == YesCheck )
			case 0x18:						//当前套费率电价
			case 0x19:						//备用套费率电价
				if( *(NewOAD+1) == 0x18 ) Addr = Case1Fee1Price;
				else Addr = Case2Fee1Price;
				if( *(NewOAD+3) == 0 )
				{
					*Dest = Data_array;
					*(Dest+1) = 4;
					Ptr = Dest+2;
					Len += 2;
//					for( i=0;i<Sum1;i++ )
					for( i=0;i<4;i++ )							//17.05.11
					{	
						*Ptr = D_Unsigned32;
						E2P_RData( Ptr+1, Addr, 4 );
						GetRAMData_BCDToHEX( Ptr+1, Ptr+1, D_Unsigned32, 4, 0 );
						Ptr += 5;
						Len += 5;
						Addr += 5;
					}				
				}
				else if( *(NewOAD+3) <= 4 )
				{
					*Dest = D_Unsigned32;
					E2P_RData( Dest+1, Addr, 4 );
					GetRAMData_BCDToHEX( Dest+1, Dest+1, D_Unsigned32, 4, 0 );
					Len = 5;
				}
				else Len = -1;					
				break;
			case 0x1A:						//当前套阶梯电价
			case 0x1B:						//备用套阶梯电价
				switch( *(NewOAD+3) )
				{
					case 0:
						*Dest = Data_array;
						*(Dest+1) = 3;
						if( *(NewOAD+1) == 0x1A ) Addr = Case1Step1EC;
						else Addr = Case2Step1EC;
						Len = GetStepArray( Dest+2, Addr, 6 );
						if( *(NewOAD+1) == 0x1A ) Addr = Case1Step1Price;
						else Addr = Case2Step1Price;
						Len += GetStepArray( Dest+2+Len, Addr, 7 );
						if( *(NewOAD+1) == 0x1A ) Addr = Case1YearSaveDay1;
						else Addr = Case2YearSaveDay1;					
						Len += GetStepSaveDayArray( Dest+2+Len, Addr );
						Len += 2;
						break;
					case 1:
						if( *(NewOAD+1) == 0x1A ) Addr = Case1Step1EC;
						else Addr = Case2Step1EC;
						Len = GetStepArray( Dest, Addr, 6 );
						Len += 2;
						break;
					case 2:
						if( *(NewOAD+1) == 0x1A ) Addr = Case1Step1Price;
						else Addr = Case2Step1Price;
						Len = GetStepArray( Dest, Addr, 7 );
						Len += 2;
						break;
					case 3:
						if( *(NewOAD+1) == 0x1A ) Addr = Case1YearSaveDay1;
						else Addr = Case2YearSaveDay1;					
						Len = GetStepSaveDayArray( Dest, Addr );
						Len += 2;
						break;
					default:
						Len = -1;					
						break;					
				}
				break;
#endif
			default:
				break;		
		}
	}	
	else if(( *NewOAD == 0x41 )&&( *(NewOAD+1) == 0x16 ))						//结算日
	{
		switch( *(NewOAD+3) )
		{
			case 0:
				*Dest = Data_array;
				*(Dest+1) = 3;
				Len = GetSaveDayItemStructure( Dest+2, ESave_Day1 );
				Len += GetSaveDayItemStructure( Dest+2+Len, ESave_Day2 );
				Len += GetSaveDayItemStructure( Dest+2+Len, ESave_Day3 );
				Len += 2;
				break;
			case 1:
				Len = GetSaveDayItemStructure( Dest, ESave_Day1 );
				break;
			case 2:
				Len = GetSaveDayItemStructure( Dest, ESave_Day2 );
				break;
			case 3:
				Len = GetSaveDayItemStructure( Dest, ESave_Day3 );
				break;
			default:
				break;					
		}
	}
	else if(( *NewOAD == 0x41 )&&( *(NewOAD+1) == 0x17 ))						//期间需量冻结周期
	{
		switch( *(NewOAD+3) )
		{
			case 0:
				*Dest = D_TI;
				E2P_RData( Dest+1, EFreezeMaxT, 3 );
				SwapData( Dest+2, 2 );
				Len = 4; 
				break;
			default:
				break;					
		}
	}
	else if(( *NewOAD == 0x40 )&&( *(NewOAD+1) == 0x04 ))						//经纬度
	{
		switch( *(NewOAD+3) )
		{
			case 0:
				*Dest = Data_structure;
				*(Dest+1) = 3;
				Len = GetPositionItemStructure( Dest+2, Longitude );
				Len += GetPositionItemStructure( Dest+2+Len, Latitude );
				*(Dest+2+Len) = D_Unsigned32;
				E2P_RData( Dest+2+Len+1, Altitude, 4 );
				SwapData( Dest+2+Len+1, 4 );
				Len += 7;
				break;
			case 1:
				Len = GetPositionItemStructure( Dest, Longitude );
				break;
			case 2:
				Len = GetPositionItemStructure( Dest, Latitude );
				break;
			case 3:
				*Dest = D_Unsigned32;
				E2P_RData( Dest+1, Altitude, 4 );
				SwapData( Dest+1, 4 );
				Len = 5;
				break;
			default:
				break;					
		}
	}
	else if(( *NewOAD == 0x40 )&&( *(NewOAD+1) == 0x06 ))						//时钟源
	{
		switch( *(NewOAD+3) )
		{
			case 0:
				*Dest = Data_structure;
				*(Dest+1) = 2;
				*(Dest+2) = D_ENUM;
				*(Dest+3) = 0x01;
				*(Dest+4) = D_ENUM;
				*(Dest+5) = 0x00;
				Len = 6;
				break;
			case 1:
				*Dest = D_ENUM;
				*(Dest+1) = 0x01;
				Len = 2;
				break;
			case 2:
				*Dest = D_ENUM;
				*(Dest+1) = 0x00;
				Len = 2;
				break;
			default:
				break;					
		}
	}
#if ( LocalSmartMeter == YesCheck )
	else if(( *NewOAD == 0x20 )&&( *(NewOAD+1) == 0x2C ))						//钱包文件
	{
		switch( *(NewOAD+3) )
		{
			case 0:
				*Dest = Data_structure;
				*(Dest+1) = 2;
				*(Dest+2) = D_Unsigned32;
				GetReOverEC( Dest+3, Dest+7 );								//剩余金额
				SwapData( Dest+3, 4 );
				*(Dest+7) = D_Unsigned32;
				E2P_RData( Dest+8, BuyECCnt, 4 );
				SwapData( Dest+8, 4 );
				Len = 12;
				break;
			case 1:
				*Dest = D_Unsigned32;
				GetReOverEC( Dest+1, Dest+5 );								//购电次数
				SwapData( Dest+1, 4 );
				Len = 5;
				break;
			case 2:
				*Dest = D_Unsigned32;
				E2P_RData( Dest+1, BuyECCnt, 4 );
				SwapData( Dest+1, 4 );
				Len = 5;
				break;
			default:
				break;					
		}
	}
	else if(( *NewOAD == 0x40 )&&( *(NewOAD+1) == 0x1E ))						//报警金额限值
	{
		switch( *(NewOAD+3) )
		{
			case 0:
				*Dest = Data_structure;
				*(Dest+1) = 2;
				*(Dest+2) = D_Unsigned32;
				E2P_RData( Dest+3, AlarmEC1, 4 );
				LongToSwapChar( Dest+3, BCD4_Long( Dest+3 ) );
				*(Dest+7) = D_Unsigned32;
				E2P_RData( Dest+8, AlarmEC2, 4 );
				LongToSwapChar( Dest+8, BCD4_Long( Dest+8 ) );
				Len = 12;
				break;
			case 1:
			case 2:
				*Dest = D_Unsigned32;
				if( *(NewOAD+3) == 0x01 ) Addr = AlarmEC1;
				else Addr = AlarmEC2;
				E2P_RData( Dest+1, Addr, 4 );
				LongToSwapChar( Dest+11, BCD4_Long( Dest+11 ) );
				Len = 5;
				break;
			default:
				break;					
		}
	}
	else if(( *NewOAD == 0x40 )&&( *(NewOAD+1) == 0x1F ))						//其他金额限值
	{
		switch( *(NewOAD+3) )
		{
			case 0:
				*Dest = Data_structure;
				*(Dest+1) = 3;
				*(Dest+2) = D_Unsigned32;
				E2P_RData( Dest+3, OverdraftECLimt, 4 );
				LongToSwapChar( Dest+3, BCD4_Long( Dest+3 ) );
				*(Dest+7) = D_Unsigned32;
				E2P_RData( Dest+8, RegrateECLimt, 4 );
				LongToSwapChar( Dest+8, BCD4_Long( Dest+8 ) );
				*(Dest+12) = D_Unsigned32;
				E2P_RData( Dest+13, PermitTripONLimt, 4 );
				LongToSwapChar( Dest+13, BCD4_Long( Dest+13 ) );
				Len = 17;
				break;
			case 1:
			case 2:
			case 3:
				*Dest = D_Unsigned32;
				if( *(NewOAD+3) == 0x01 ) Addr = OverdraftECLimt;
				else if( *(NewOAD+3) == 0x02 ) Addr = RegrateECLimt;
				else Addr = PermitTripONLimt;
				E2P_RData( Dest+1, Addr, 4 );
				LongToSwapChar( Dest+1, BCD4_Long( Dest+1 ) );
				Len = 5;
				break;
			default:
				break;					
		}
	}
#endif	
	else if(( *NewOAD == 0xF1 )&&( *(NewOAD+1) == 0x01 )&&( *(NewOAD+2) == 0x03 )&&( *(NewOAD+3) == 0x00 ))
	{
		*Dest = Data_array;
		E2P_RData( Dest+1, SafeModeParaSum, 1 );
		if( *(Dest+1) > 100 ) *(Dest+1) = 0;
		Len = 2;
//		Ptr = Dest+1;
		Ptr = Dest+2;							//17.04.12
		for( i=0;i<*(Dest+1);i++ )
		{
			*Ptr = Data_structure;
			*(Ptr+1) = 2;	
			*(Ptr+2) = D_OI;				
			E2P_RData( Ptr+4, SafeModePara1+5*i, 4 );	
			*(Ptr+3) = *(Ptr+4);
			*(Ptr+4) = *(Ptr+5);
			*(Ptr+5) = D_Unsigned16;
			
			Ptr += 8;
			Len += 8;
		}			
	}	
//	else if(( *NewOAD == 0x33 )&&( *(NewOAD+1) == 0x20 )&&( *(NewOAD+2) == 0x02 )&&( *(NewOAD+3) == 0x00 ))		//17.03.24
	else if(( *NewOAD == 0x33 )&&( *(NewOAD+1) == 0x20 )&&( *(NewOAD+2) == 0x02 ))		//17.05.19
	{
//		Len = GetNewEventList( COM_No, Dest );	
		Len = GetNewReportEventList( COM_No, Dest, NewOAD );								//17.05.19
	}
	else if(( *NewOAD == 0x33 )&&( *(NewOAD+1) == 0x20 )&&( *(NewOAD+2) == 0x03 )&&( *(NewOAD+3) == 0x00 ))		//17.05.19
	{
		Len = GetNewReportEventListAttribute3( Dest );								//17.05.19
	}
		
	return Len;
}	


//读参数变量接口类属性数据，返回值：-1，无此数据项，非-1：返回数据长度
//short GetClass8Data( unsigned char* Dest, unsigned char* OAD )
short GetClass8Data( short COM_No, unsigned char* Dest, unsigned char* OAD )	//17.03.24
{
	short i,j;
	unsigned short Addr;
	short Len;
//	short Temp;
	short Start,End;
	unsigned char DataType;
	unsigned char Element_Len;			
//	unsigned char BCD_Len;			//原BCD数据长度
//	char		  Multiple;			//放大倍数				负数表示缩小
	unsigned char* Ptr;
	unsigned short ComInfo;
	unsigned long ComID;
//	unsigned long Value;
	Len = -1;
	
	i = GetParameterClassTabItemNo( OAD );
	if( i != DL698_ClassId8Sum )
	{
		ComID = SwapCharToLong( OAD );
		*Dest = Get_data;								//正常数据返回	
		DataType = DL698_ClassId8_Tab[i].DataType;
		ComInfo = DL698_ClassId8_Tab[i].ComInfo;
		if(( DataType == Data_array )||( DataType == Data_structure )||( DataType == D_TI )) 					
		{	
			if((ComInfo & Comm_Type ) == SPECFUN )			//节假日、时区、时段、阶梯、费率电价、结算日、显式安全模式等需特殊处理
			{
				Len = 1;
//				Len += GetSpecClassId8( Dest+1, OAD );
				Len += GetSpecClassId8( COM_No, Dest+1, OAD );		//17.03.24
                return Len;
			}
			else		
			{
				Start = 0;
				Element_Len = DL698_ClassId8_Tab[i].Element_Len;
				End = Element_Len;
				*(Dest+1) = DataType;
				*(Dest+2) = End;
				Ptr = Dest+3;
				Len = 3;
				i += 1;
			}
		}	
		else
		{
			Start = 0;
			End = 1;					
			Ptr = Dest+1;
			Len = 1;
		}	 
		for( j=Start;j<End;j++ )
		{
			DataType = DL698_ClassId8_Tab[i].DataType;
			Element_Len = DL698_ClassId8_Tab[i].Element_Len;
			ComInfo = DL698_ClassId8_Tab[i].ComInfo;
			switch( ComInfo & Comm_Type )
			{
				case OTHER:
					*Ptr = DataType;
					if( ComID == 0x40000200 ) 
					{
						RAM_Write( Ptr+1, &Clk.YearH, Element_Len );		//当前时间
					}	
					else if( ComID == 0x40100200 ) 						//计量元件数	
					{
#if ( LinkMode == Phase3Wire4 )
						*(Ptr+1) = 0x03;		
#else
						*(Ptr+1) = 0x02;		
#endif						
					}	
					else if( ComID == 0x41020200 )
					{
						*(Ptr+1) = 80; 									//校表脉冲宽度时间	
					}	
					else if( ComID == 0x80000300 ) 						//继电器输出状态
					{
						*(Ptr+1) = 0x08; 								//D_Bit_String长度
						if( Para.RMeterRelayState == 0x1A ) *(Ptr+2) = 0x80; 									
						else *(Ptr+2) = 0x00; 									
						Element_Len = 2;						
					}
					else if( ComID == 0x80000400 ) 						//继电器告警状态
					{
						*(Ptr+1) = 0x08; 								//D_Bit_String长度
						if( Para.RMeterAlarmState == 0x2A ) *(Ptr+2) = 0x80;
						else *(Ptr+2) = 0x00; 									
						Element_Len = 2;
					}
					else if( ComID == 0x80000500 ) 						//继电器命令状态
					{
						*(Ptr+1) = 0x08; 								//D_Bit_String长度
						if(( Flag.MeterState3 & F_RelayOdState ) != 0 ) *(Ptr+2) = 0x80; 										
						else *(Ptr+2) = 0x00; 										
						Element_Len = 2;
					}
					else if( ComID == 0x80010200 ) 						//继电器保电状态
					{
						if( Para.RMeterSafeState == 0x3A ) *(Ptr+1) = 0x01;
						else *(Ptr+1) = 0x00; 									
					}
					else 
					{
						RAM_Fill( Ptr+1, Element_Len);					//缺省填0
					}	
					break;
				case PARAM:
					Addr = DL698_ClassId8_Tab[i].Addr;
					if(( DataType == D_Bit_String )||( DataType == D_VisibleStr )||( DataType == D_OctetStr))
					{
						*Ptr = DataType;
						*(Ptr+1) = Element_Len;	
						if( DataType == D_Bit_String ) 	Element_Len /= 8;		
						E2P_RData( Ptr+2, Addr, Element_Len );
						if( DataType == D_OctetStr ) SwapData( Ptr+2, Element_Len );
						else if( DataType == D_Bit_String ) SwapBitString( Ptr+2, Element_Len );
						Element_Len += 1;
					}
//					else if(( ComID == 0x401C0200 )||( ComID == 0x401D0200 )) 	//电压变比、电流变比
//					{
//						*Ptr = DataType;
//						E2P_RData( Ptr+1, Addr, 3 );	
//						Value = BCD3_Long( Ptr+1 );			
//						RAM_Write( Ptr+1, (unsigned char*)&Value, 4 );			
//						SwapData( Ptr+1, Element_Len );
//					}
					else
					{
						*Ptr = DataType;
#if ( LocalSmartMeter == YesCheck )
						if( ComID == 0x400A0200 )
						{
							ReadFeeStepSwitchTime( Ptr+1, Addr, OldFeeSwitchTime );
						}
						else if( ComID == 0x400B0200 )
						{
							ReadFeeStepSwitchTime( Ptr+1, Addr, OldStepSwitchTime );
						}
						else
						{	
							E2P_RData( Ptr+1, Addr, Element_Len );
							SwapData( Ptr+1, Element_Len );
						}	
#else
						E2P_RData( Ptr+1, Addr, Element_Len );
						SwapData( Ptr+1, Element_Len );
#endif
					}		
					
					break;
				default: break;
			}	
			Ptr += Element_Len + 1;
			Len += Element_Len + 1;
			i++;
		}		
	}
	return Len;	
}

//-------------------------------------------------------------------------------------------------------
unsigned char SetParameterClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = SetClass8DataTab( COM_No, Dest+4, Source, DataLen, ItemLen  );
	return ErrorByte;		

}		

//取PhaseEvent类数据
unsigned char SetClass8DataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	short i;
//	unsigned char* Addr;
	unsigned char ErrorByte=object_class_inconsistent;
//	short Len;
//	unsigned short OICode;
//	short Temp;
//	short Start,End;
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
//	unsigned char* Ptr;
	unsigned char* OAD;
//	Len = -1;
	
	OAD = Source;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
				
	i = GetParameterClassTabItemNo( OAD );
	if( i != DL698_ClassId8Sum )
	{
//		*Dest = Get_data;								//正常数据返回		
		switch( Attribute )
		{
			case 1:										//logical_name
				break;	
			case 2:										//	
//				ErrorByte = SetClass8_2Data( Source, DataLen, ItemLen );
				ErrorByte = SetClass8_2Data( COM_No, Source, DataLen, ItemLen );		//17.04.09
				if( ErrorByte == 0 )
				{
					CreatPrgRecEvent( OAD ); 
					InitPara1();
					InitPara4();
					InitPara5();
					InitPara6();
				}	
				break;	
			case 3:										//
//				ErrorByte = SetClass8_2Data( Source, DataLen, ItemLen );
				ErrorByte = SetClass8_2Data( COM_No, Source, DataLen, ItemLen );		//17.04.09
				if( ErrorByte == 0 ) 
				{
					CreatPrgRecEvent( OAD ); 
					InitPara1();
					InitPara4();
					InitPara5();
					InitPara6();
				}	
				break;	
			default:
				break;	
		}	
	}		
	return ErrorByte;
}	

//unsigned char SetClass8_2Data( unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen )
unsigned char SetClass8_2Data( unsigned short COM_No, unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen )		//17.04.09
{
	unsigned char Buff[100];
	unsigned char *Ptr;
	unsigned char ErrorByte=0;
	short i,j;
	unsigned short Addr;
//	short Len;
//	short Temp;
	short Start,End;
	unsigned char DataType;
	unsigned char Element_Len;			
//	unsigned char BCD_Len;			//原BCD数据长度
//	char		  Multiple;			//放大倍数				负数表示缩小
	unsigned short ComInfo;
	unsigned long ComID;
//	unsigned char* Ptr;
	unsigned char OAD[4];
//    unsigned long Value;
//	Len = -1;
	unsigned char Len;
	
	Ptr = Buff;
//	for( i=0;i<DL698_ClassId8Sum;i++ )
//	{
//		if( Data_Comp( Source, (unsigned char*)DL698_ClassId8_Tab[i].OAD, 4 ) == 0 ) break;				//查找对应的OAD码
//	}	
//	if( i == DL698_ClassId8Sum ) { return object_undefined; }

	i = GetParameterClassTabItemNo( Source );

	if( Judge_operating_authority( COM_No, Source, DL698_ClassId8_Tab[i].SaftInfo, write_authority ) != 0 ) return read_write_denied;	//17.04.09

	DataType = *(Source+4);
//	if( DL698_ClassId8_Tab[i].DataType != DataType ) { return type_unmatched; }
	ComInfo = DL698_ClassId8_Tab[i].ComInfo;
	RAM_Write( OAD, Source, 4 );
	ComID = SwapCharToLong( OAD );

	Source += 4;
	*ItemLen += 4;
	if(( DataType == Data_array )||( DataType == Data_structure )||( DataType == D_TI )) 					
	{
		if((ComInfo & Comm_Type ) == SPECFUN )			//节假日、时区、时段、阶梯、费率电价、结算日、显式安全模式等需特殊处理
		{
			ComInfo = DL698_ClassId8_Tab[i].ComInfo;
			if(( ComInfo & WRITE ) == 0 )
			{ 
				return scope_of_access_violated; 
			}
			ErrorByte = SetSpecClassId8( Source, OAD, DataLen, ItemLen );	
			return ErrorByte;
		}
		else		
		{
			Start = 0;
			Element_Len = DL698_ClassId8_Tab[i].Element_Len;
	        if( DataType == Data_array ) 
    	    {
        		if( *(Source+1) <= Element_Len ) Element_Len = *(Source+1); 
          		else return type_unmatched;  
        	}  
			End = Element_Len;
           	Source += 2;
            *ItemLen += 2;
			i += 1;
			if( End == 0 ) return object_class_inconsistent;
		}
	}	
	else
	{
		if( DL698_ClassId8_Tab[i].DataType != DataType ) { return type_unmatched; }
		Start = 0;
		End = 1;					
	}	 
	for( j=Start;j<End;j++ )
	{
		DataType = DL698_ClassId8_Tab[i].DataType;
		if( *Source != DataType ) { return type_unmatched; }
		DataLen -= 1;
		Element_Len = DL698_ClassId8_Tab[i].Element_Len;
		if( DataType == D_Bit_String ) Element_Len /= 8;
//		if(( DataType == D_Bit_String )||( DataType == D_VisibleStr )||( DataType == D_OctetStr))
//		{
//			if( DataLen > (Element_Len+2) ) { return type_unmatched; }
//		}
//		else
//		{
//			if( DataLen < Element_Len ) { return type_unmatched; }			
//		}		
		ComInfo = DL698_ClassId8_Tab[i].ComInfo;
		Addr = DL698_ClassId8_Tab[i].Addr;
		if(( ComInfo & WRITE ) == 0 )
		{ 
			if(( *OAD == 0x41 )&&((( *(OAD+1) >= 0x04 )&&( *(OAD+1) <= 0x08 ))||(*(OAD+1) == 0x0B))&&(((HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 )))
			{
				__NOP();	//额定电压等可在厂内状态设置
			}
			else
			{			
				return scope_of_access_violated; 
			}	
		}

		switch( ComInfo & Comm_Type )
		{
			case OTHER:
				if( ComID == 0x40000200 ) 
				{
#if ( RemoteFeeMeter ==	YesCheck )		//远程费率智能表	
					if( ICComm.RPrivateKeyFlag == 0 ) return scope_of_access_violated;				//江苏远程费率表私钥下不允许修改时间
#endif
					if( Check_date_time_s( Source+1 ) != 0 ) return object_class_inconsistent;	//17.04.21
					RecordStartEvent( TimeSet_No );
					RAM_Write( &Clk.YearH, Source+1, Element_Len );		//当前时间
                    MoveCurrentTimeHex_BCD();
                    RAM_Write( Source+1, Clk.Sec, 3 );
					*(Source+1+3) = (unsigned char)ByteBit[Clk.Week[0]];					//新国网	//13.09.28
                    RAM_Write( Source+5, Clk.Day, 3 );
					E2Pwrite( 0x0, Source+1, 7, RTCAds );	//写时钟芯片秒分时
					TimeChangeUpdateAllLastSaveTime();							//时间向前修改，更新上次结算日结算和月结算时间
					RecordEndEvent( TimeSet_No, 0 );
					GetDaySeg();				
					GetFeeNo();		
					MD_InitialCalPara( Para.RMaxT );		
				}	
				else 
				{
					return scope_of_access_violated;					
				}	
				break;
			case PARAM:
				Addr = DL698_ClassId8_Tab[i].Addr;
				if(( DataType == D_Bit_String )||( DataType == D_VisibleStr )||( DataType == D_OctetStr))
				{
//					Element_Len = *(Source+1);	
//					if( DataType == D_Bit_String ) 	Element_Len /= 8;		
//					E2P_WData( Addr, Source+2, Element_Len );
//					Element_Len += 1;

					Len = *(Source+1);
					if( DataType == D_Bit_String ) 	Len /= 8;
					if(( Len > 100 )||( Len > Element_Len )) return read_write_denied;					//超过100不允许设置	
					
					if( ComID == 0x41120200 ) RecordStartEvent( ECPModePrg_No );
					else if( ComID == 0x41130200 ) { SM.PrgRactiveModeType = 0; RecordStartEvent( ECQModePrg_No ); }
					else if( ComID == 0x41140200 ) { SM.PrgRactiveModeType = 1; RecordStartEvent( ECQModePrg_No ); }
						
					RAM_Write( Ptr, Source+2, Len );
					if( DataType == D_OctetStr ) SwapData( Ptr, Len );
					else if( DataType == D_Bit_String ) SwapBitString( Ptr, Len );
					RAM_Fill( Ptr+Len, Element_Len - Len );					
//					if( DataType == D_OctetStr ) SwapData( Ptr, Element_Len );
					E2P_WData( Addr, Ptr, Element_Len );
					Element_Len = Len + 1;						
				}
//				else if(( ComID == 0x401C0200 )||( ComID == 0x401D0200 ))			//电压变比、电流变比
//				{
//					SwapData( Source+1, Element_Len );
//					RAM_Write( (unsigned char*)&Value, Source+1, 4 );			
//					Long_BCD3( Source+1, Value );
//					E2P_WData( Addr, Source+1, 3 );	
//				}
				else
				{
					if(( *OAD == 0x40 )&&( *(OAD+1) == 0x0C )&&( *(OAD+2) == 0x02 ))				//17.04.21
					{
						switch( DL698_ClassId8_Tab[i].OAD[3] )	
						{
							case 1:								//年时区数
								if( *(Source+1) > ZONENUMBER ) ErrorByte = zone_sum_over;
								break;
							case 2:								//日时段表数 	
								if( *(Source+1) > 8 ) ErrorByte = object_class_inconsistent;
								break;
							case 3:								//日时段数
								if( *(Source+1) > SEGNUMBER ) ErrorByte = seg_sum_over;
								break;
							case 4:								//费率数
								if( *(Source+1) > 4 ) ErrorByte = fee_sum_over;
								break;
							case 5:								//公共假日数
								if( *(Source+1) > HOLIDAYNUMBER ) ErrorByte = object_class_inconsistent;
								break;
							default:
								break;							
						}	
					}	
					
					if( ErrorByte == 0 )
					{
						if( ComID == 0x40130200 ) { RecordStartEvent( WeekCDPrg_No ); }
						SwapData( Source+1, Element_Len );
						E2P_WData( Addr, Source+1, Element_Len );
					}	
				}		
				break;
			default: 
				break;
		}	
		Source += Element_Len + 1;
		DataLen -= Element_Len;
		*ItemLen += Element_Len + 1;
		i++;
	}
			
	return ErrorByte;
}	

void TimeChangeUpdateLastSaveTime( unsigned short Addr, unsigned char* CurTime )
{
	unsigned char Buff[4];
	unsigned char* LastTime;

	LastTime = Buff;

	E2P_RData( LastTime, Addr, 4 );									//上次结算时间
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  
	{
		E2P_WData( Addr, CurTime, 4 );								//当前时间若小于上次结算时间，则上次结算时间=当前时间	
	}	
}
	
void TimeChangeUpdateAllLastSaveTime( void )
{
	unsigned char Buff[4];
	unsigned char* CurTime;
	
	CurTime = Buff;
	
	RAM_Write( CurTime, Clk.Hour, 4 );								//当前时间
	if( DateTime4ValueCheck( CurTime ) != 0 ) return;				
	
	TimeChangeUpdateLastSaveTime( LastSaveTime, CurTime );
	TimeChangeUpdateLastSaveTime( VoltLastSaveTime, CurTime );

}	


void SetZoneSegItemStructure( unsigned char* Dest, unsigned char* Source )
{
	*Dest = *(Source+7);					//时段表号
	*(Dest+1) = Byte_BCD(*(Source+5));		//日	
	*(Dest+2) = Byte_BCD(*(Source+3));		//月
}	

void SetSaveDayItemStructure( unsigned short Addr, unsigned char* Source  )
{
	unsigned char Buff[4];
	unsigned char* Point;
	
	Point = Buff;
	
	*Point = Byte_BCD(*(Source+5));			//日	
	*(Point+1) = Byte_BCD(*(Source+3));		//月	
	
	E2P_WData( Addr, Point, 2 );
}

void SetPositionItemStructure( unsigned short Addr, unsigned char* Source  )
{
	unsigned char Buff[4];
	unsigned char* Point;
	
	Point = Buff;

	*Point = *(Source+3);					//方位
	*(Point+1) = *(Source+5);				//度	
	*(Point+2) = *(Source+7);				//分	
	*(Point+3) = *(Source+9);				//秒		
	
	E2P_WData( Addr, Point, 4 );
}

void FillDefaultSegData( unsigned char* Point, unsigned short Start, unsigned short End )
{
	short j;
	
	for( j=Start;j<End;j++ )
	{
		RAM_Write( Point+j*3, Point+(Start-1)*3, 3 );			//补齐空白时区、时段表
	}		
}

unsigned char SetSpecClassId8( unsigned char* Source, unsigned char* OAD, unsigned short DataLen, unsigned short* ItemLen )	
{
	unsigned char ErrorByte=0;
	short i,j;
	unsigned short Addr;
//	short Len=0;
	short Temp;
//	short Start,End;
//	unsigned char DataType;
//	unsigned char Element_Len;			
	unsigned char* Ptr;
//	unsigned short ComInfo;
//	unsigned long ComID;
	unsigned char Buff[4];
	unsigned char Buff2[50];
	unsigned char* NewOAD;
	unsigned char* Point;
	unsigned char Sum1,Sum2;
//	short Length;
	
	NewOAD = Buff;
	Point = Buff2;
	RAM_Write( NewOAD, OAD, 4 );
//	ComID = SwapCharToLong( OAD );

	if(( *NewOAD == 0x40 )&&((( *(NewOAD+1) >= 0x14 )&&( *(NewOAD+1) <= 0x1B ))||( *(NewOAD+1) == 0x11 ))) 	
	{
		switch( *(NewOAD+1) )	
		{
			case 0x11:						//节假日
//				E2P_RData( (unsigned char*)&Sum1, HolidaySum, 1 );
//				if( Sum1 > 20 ) Sum1 = 20;   							//最多20个节假日
				Sum1 = HOLIDAYNUMBER;   							//最多20个节假日
				if( *(NewOAD+3) == 0 )
				{
					return read_write_denied;								//不允许大包设置时段表数数据		//17.04.15
/*																												//17.04.15
//					*Dest = Data_array;
//					*(Dest+1) = Sum1;
					if( *(Source+1) > Sum1 ) return type_unmatched; 
					Sum1 = *(Source+1);
					RecordStartEvent( HolidayPrg_No );
					Ptr = Source+2;
					*ItemLen += 2;
					for( i=0;i<Sum1;i++ )
					{	
//						Length = *ItemLen;
//						*(NewOAD+3) = i+1;
//						ErrorByte = SetSpecClassId8( Ptr, NewOAD, DataLen, ItemLen );	
//						Ptr += *ItemLen - Length;
	 					if((*Ptr) != Data_structure ) return type_unmatched;
						if(*(Ptr+1) != 2 ) return type_unmatched;				
						Addr = Holiday1 + i*7;
						*(Point+5) = *(Ptr+9);
						RAM_Write( Point, Ptr+3, 5 );
						E2P_WData( Addr, Point, 6 );
						DataLen -= 10;
						*ItemLen += 10;
						Ptr += 10;
					}
					E2P_WData( HolidaySum, (unsigned char*)&Sum1, 1 );				
*/
				}
				else if( *(NewOAD+3) <= Sum1 )
				{
//					Ptr = Dest;
					if((*Source) != Data_structure ) return type_unmatched;
					if(*(Source+1) != 2 ) return type_unmatched;				
					RAM_Write( SM.PrgOAD, NewOAD, 4 );						//17.04.15
					RecordStartEvent( HolidayPrg_No );
					Addr = *(NewOAD+3) - 1;
					Addr *= 7;
					Addr += Holiday1;
					*(Point+5) = *(Source+9);
					RAM_Write( Point, Source+3, 5 );
					E2P_WData( Addr, Point, 6 );
					DataLen -= 10;
					*ItemLen += 10;
					
					E2P_RData( (unsigned char*)&Sum1, HolidaySum, 1 );
					if( *(NewOAD+3) > Sum1 )
					{
						Sum1 = *(NewOAD+3);
						E2P_WData( HolidaySum, (unsigned char*)&Sum1, 1 );
					}	
				}
				else return object_undefined;	
				break;	
			case 0x14:						//当前套时区
			case 0x15:						//备用套时区
				E2P_RData( (unsigned char*)&Sum1, ZoneSum, 1 );
				if( Sum1 > ZONENUMBER ) Sum1 = ZONENUMBER;
				if( *(NewOAD+1) == 0x14 ) Addr = ESeason1;
				else Addr = ESeason2;
				if( *(NewOAD+3) == 0 )
				{
//					*Dest = Data_array;
//					*(Dest+1) = Sum1;
					if( Sum1 < *(Source+1) ) return type_unmatched; 
					RecordStartEvent( ZonePrg_No );
					Sum1 = *(Source+1);
					Ptr = Source+2;
					*ItemLen += 2;
					for( i=0;i<Sum1;i++ )
					{	
						Temp = i*8;
						SetZoneSegItemStructure( Point+i*3, Ptr+Temp );
						*ItemLen += 8;
					}
					FillDefaultSegData( Point, i, ZONENUMBER );					//补齐空白时区表
					E2P_WData( Addr, Point, ZONENUMBER*3 );		
				}
				else return read_write_denied;								//不允许设置单项时区数据
				break;	
			case 0x16:						//当前套时段表
			case 0x17:						//备用套时段表
				E2P_RData( (unsigned char*)&Sum1, SegTabSum, 1 );
				if( Sum1 > 8 ) Sum1 = 8;
				E2P_RData( (unsigned char*)&Sum2, SegSum, 1 );
				if( Sum2 > SEGNUMBER ) Sum2 = SEGNUMBER;
				if( *(NewOAD+1) == 0x16 ) Addr = Period1_Tab1;
				else Addr = Period2_Tab1;
				if( *(NewOAD+3) == 0 )
				{
					return read_write_denied;								//不允许大包设置时段表数数据		//17.04.15
/*																												//17.04.15
					if( Sum1 < *(Source+1) ) return type_unmatched; 		//不允许设置超出时段表数数据
					RecordStartEvent( SegTabPrg_No );
					Sum1 = *(Source+1);
					Ptr = Source+2;
					*ItemLen += 2;
					for( j=0;j<Sum1;j++ )
					{
						if( *Ptr != Data_array ) return type_unmatched;
						if( Sum2 < *(Ptr+1) ) return type_unmatched; 		//不允许设置超出时段数数据
						Sum2 = *(Ptr+1);
						Ptr += 2;
						*ItemLen += 2;
						for( i=0;i<Sum2;i++ )
						{	
							SetZoneSegItemStructure( Point+i*3, Ptr );
							Ptr += 8;
							*ItemLen += 8;
						}
						FillDefaultSegData( Point, i, SEGNUMBER );					//补齐空白时段表
						E2P_WData( Addr+j*(SEGNUMBER*3+1), Point, SEGNUMBER*3 );
					}					
*/
				}
				else if( *(NewOAD+3) <= Sum1 )
				{
					if( Sum2 < *(Source+1) ) return type_unmatched; 		//不允许设置超出时段数数据
					RAM_Write( SM.PrgOAD, NewOAD, 4 );						//17.04.15
					RecordStartEvent( SegTabPrg_No );
					Sum2 = *(Source+1);
					Ptr = Source+2;
					*ItemLen += 2;
					for( i=0;i<Sum2;i++ )
					{	
//						if( *Ptr != Data_array ) return type_unmatched;
						SetZoneSegItemStructure( Point+i*3, Ptr );
						Ptr += 8;
						*ItemLen += 2;
					}
					FillDefaultSegData( Point, i, SEGNUMBER );						//补齐空白时段表
					Temp = *(NewOAD+3) - 1;
					Addr += Temp * (SEGNUMBER*3+1); 
					E2P_WData( Addr, Point, SEGNUMBER*3 );		
				}
				else return read_write_denied;								//不允许设置超出时段表数数据
				break;
			case 0x18:						//当前套费率电价
			case 0x19:						//备用套费率电价
				break;
			case 0x1A:						//当前套阶梯电价
			case 0x1B:						//备用套阶梯电价
				break;
			default:
				break;		
		}
	}	
	else if(( *NewOAD == 0x41 )&&( *(NewOAD+1) == 0x16 ))					//结算日
	{
		RecordStartEvent( SaveDayPrg_No );
		Ptr = Source;
		switch( *(NewOAD+3) )
		{
			case 0:
				*ItemLen += 2;
				SetSaveDayItemStructure( ESave_Day1, Ptr+2 );
				*ItemLen += 6;
				SetSaveDayItemStructure( ESave_Day2, Ptr+8 );
				*ItemLen += 6;
				SetSaveDayItemStructure( ESave_Day3, Ptr+14 );
				*ItemLen += 6;
				break;
			case 1:
				SetSaveDayItemStructure( ESave_Day1, Ptr );
				*ItemLen += 6;
				break;
			case 2:
				SetSaveDayItemStructure( ESave_Day2, Ptr );
				*ItemLen += 6;
				break;
			case 3:
				SetSaveDayItemStructure( ESave_Day3, Ptr );
				*ItemLen += 6;
				break;
			default:
				ErrorByte = object_undefined;
				break;					
		}
	}
	else if(( *NewOAD == 0x41 )&&( *(NewOAD+1) == 0x17 ))						//期间需量冻结周期
	{
		Ptr = Source;
		switch( *(NewOAD+3) )
		{
			case 0:
				SwapData( Ptr+2, 2 );
				E2P_WData( EFreezeMaxT, Ptr+1, 3 );
				*ItemLen += 4;
				break;
			default:
				break;					
		}
	}
	else if(( *NewOAD == 0x40 )&&( *(NewOAD+1) == 0x04 ))						//经纬度
	{
		Ptr = Source;
		switch( *(NewOAD+3) )
		{
			case 0:
				*ItemLen += 2;
				SetPositionItemStructure( Longitude, Ptr+2 );
				*ItemLen += 10;
				SetPositionItemStructure( Latitude, Ptr+12 );
				*ItemLen += 10;
				SwapData( Ptr+23, 4 );
				E2P_WData( Altitude, Ptr+23, 4 );
				*ItemLen += 5;
				break;
			case 1:
				SetPositionItemStructure( Longitude, Ptr );
				*ItemLen += 10;
				break;
			case 2:
				SetPositionItemStructure( Latitude, Ptr+12 );
				*ItemLen += 10;
				break;
			case 3:
				SwapData( Ptr+1, 4 );
				E2P_WData( Altitude, Ptr+1, 4 );
				*ItemLen += 5;
				break;
			default:
				ErrorByte = object_undefined;
				break;					
		}
	}
#if ( LocalSmartMeter == YesCheck )
	else if(( *NewOAD == 0x40 )&&( *(NewOAD+1) == 0x1F ))						//其他金额限值
	{
		Ptr = Source;
		switch( *(NewOAD+3) )
		{
			case 0:
//				if(( *Ptr != Data_array )||( *(Ptr+1) > 3 )) return type_unmatched;
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 3 )) return type_unmatched;		//17.05.15
				Ptr += 2;
				Long_BCD4( Point, SwapCharToLong( Ptr+1 ) );
				E2P_WData( OverdraftECLimt, Point, 4 );
				Ptr += 5;
				Long_BCD4( Point, SwapCharToLong( Ptr+1 ) );
				E2P_WData( RegrateECLimt, Point, 4 );
				Ptr += 5;
				Long_BCD4( Point, SwapCharToLong( Ptr+1 ) );
				E2P_WData( PermitTripONLimt, Point, 4 );
				break;
			case 1:
			case 2:
			case 3:
				if( *Ptr != D_Unsigned32 ) return type_unmatched;
				if( *(NewOAD+3) == 0x01 ) Addr = OverdraftECLimt;
				else if( *(NewOAD+3) == 0x02 ) Addr = RegrateECLimt;
				else Addr = PermitTripONLimt;
				Long_BCD4( Point, SwapCharToLong( Ptr+1 ) );
				E2P_WData( Addr, Point, 4 );
				break;
			default:
				break;					
		}
	}
#endif	
	else if(( *NewOAD == 0xF1 )&&( *(NewOAD+1) == 0x01 )&&( *(NewOAD+2) == 0x03 )&&( *(NewOAD+3) == 0x00 ))
	{
		Ptr = Source;
		if(( *Ptr != Data_array )||( *(Ptr+1) > 100 )) return type_unmatched;
		Ptr += 2;
		for( i=0;i<*(Source+1);i++ )
		{
			if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != D_OI )||( *(Ptr+5) != D_Unsigned16 )) return type_unmatched;
			Ptr += 8;
		}	
//		Ptr = Source;
		Ptr = Source+2;							//17.05.12
		*ItemLen += 2;
		for( i=0;i<*(Source+1);i++ )
		{
			*Point = *(Ptr+3);
			*(Point+1) = *(Ptr+4);
			*(Point+2) = *(Ptr+6);
			*(Point+3) = *(Ptr+7);
			E2P_WData( SafeModePara1+5*i, Point, 4 );	
			Ptr += 8;
			*ItemLen += 8;		
		}	
		Para.RSafeModeParaSum = *(Source+1);
		E2P_WData( SafeModeParaSum, (unsigned char*)&Para.RSafeModeParaSum, 1 );
	}			
	
	return ErrorByte;
}	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ActionParameterClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = ActionParameterClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen, BackLen  );
	return ErrorByte;		

}	

unsigned char ActionParameterClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
//	unsigned char Buff[4];
//	short i,j;
//	unsigned char* Addr;
	unsigned char ErrorByte=object_class_inconsistent;
//	short Len;
	unsigned short OICode;
//	short Temp;
//	unsigned short GetLen;
//	short Start,End;
//	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
//	unsigned char* Ptr;
	unsigned char* OMD;
	unsigned char* WriteBufAds;
	unsigned char Method;
#if ( GeneralSmartMeter	== NoCheck )
	unsigned char OperateType;
#endif
	
//	Len = -1;
	
	OMD = Source;
	WriteBufAds = Source+4;
	OICode = SwapCharToShort( OMD );
	Method = *(OMD+2);
	
//	Ptr = Buff;
	
	switch( Method )
	{
		case 1:										//复位
			if( OICode == 0xF101 )					//清空安全模式参数属性3		//17.04.07
			{
				*ItemLen += 6;	
				if( Judge_operating_authority( COM_No, OMD, 0x8120, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				Para.RSafeModeParaSum = 0x00;													
				E2P_WData( SafeModeParaSum, (unsigned char*)&Para.RSafeModeParaSum, 1 );
				ErrorByte = 0;   					
			}	
			break;	
		case 2:										//执行	
			break;	
		case 127:										
			if( OICode == 0x4000 )					//广播校时
			{
				if( Judge_operating_authority( COM_No, OMD, 0x8180, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				if( BroadC_Time698( WriteBufAds ) != 0 ) return ErrorByte;
				else ErrorByte = 0;	
			}
			else if( OICode == 0xF101 )					//增加显示安全模式参数		//17.04.07
			{
				*ItemLen += 12;	
				if( Judge_operating_authority( COM_No, OMD, 0x8120, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				if(( *WriteBufAds != Data_structure )||( *(WriteBufAds+1) != 2 )||( *(WriteBufAds+2) != D_OI )||( *(WriteBufAds+5) != D_Unsigned16 )) ErrorByte = type_unmatched;
				else if( Para.RSafeModeParaSum >= 100 ) ErrorByte = other_reason; 					//最多100项OI
				else if( NewOIIsNotExist( WriteBufAds ) != 0 ) return object_class_inconsistent;	//若配置内容已存在，返回出错
				else
				{
					AddSafeModeParaItem( WriteBufAds );
					Para.RSafeModeParaSum += 1;													
					E2P_WData( SafeModeParaSum, (unsigned char*)&Para.RSafeModeParaSum, 1 );
					ErrorByte = 0;   					
				}	
			}	
#if ( GeneralSmartMeter	== NoCheck )
			else if( OICode == 0x8000 )						//报警
			{
				if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.30
				if( Judge_operating_authority( COM_No, OMD, 0x4110, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//				if( JudgeTimeTag( COM_No, WriteBufAds+1 ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.17
				HT_GPIOD->PTSET |= GPIOD_RLY_ALM;
				Flag.AlarmFlg[3] |= F_AlarmRemote;			//新国网	//13.09.14
				Flag.OLDRelayOFFAlarmFlg[3] &= ~F_AlarmRemote;//新国网	//13.10.19		//若已进行继电器报警过，则再次进行报警
				Para.RMeterAlarmState = 0x2A;
				ICComm.ControlComExeState = 0x0400;		//报警
				E2P_WData( MeterAlarmState, &Para.RMeterAlarmState, 1 );
				ErrorByte = 0;	
			}
			else if( OICode == 0x8001 )						//保电
			{
				if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.30
				if( Judge_operating_authority( COM_No, OMD, 0x4110, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//				if( JudgeTimeTag( COM_No, WriteBufAds+1 ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.17
				ICComm.PreTripOFFKeepTime = 0;
				if(( Para.RMeterTripState == 0x1A )||( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E ))		//新国网	//14.05.03
				{
					ICComm.RelayOFFState = 0x00;			//收到远程直接合闸命令后，取消允许拉闸状态	//新国网	//13.10.25
					ICComm.TripOFFDelay = 0;
					Para.RMeterTripState = 0x1B;			//允许合闸
					E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
				}	
				Para.RMeterSafeState = 0x3A;
				ICComm.ControlComExeState = 0x0100;		//保电
				E2P_WData( MeterSafeState, &Para.RMeterSafeState, 1 );
				ErrorByte = 0;	
			}		
#endif
			break;	
		case 128:										
			if( OICode == 0xF101 )					//删除显示安全模式参数		//17.04.07
			{
				if( Judge_operating_authority( COM_No, OMD, 0x8120, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				if( *WriteBufAds != D_OI ) ErrorByte = type_unmatched;
				else if( Para.RSafeModeParaSum == 0x00 ) ErrorByte = other_reason; 
				else
				{
					DeleteSafeModeParaItem( WriteBufAds );
					ErrorByte = 0;   					
				}
				*ItemLen += 7;	
			}	
#if ( GeneralSmartMeter	== NoCheck )
			else if( OICode == 0x8000 )							//报警解除
			{
				if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.30
				if( Judge_operating_authority( COM_No, OMD, 0x4110, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//				if( JudgeTimeTag( COM_No, WriteBufAds+1 ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.17
				Para.RMeterAlarmState = 0x2B;
				ICComm.ControlComExeState = 0x0800;		//报警解除
				E2P_WData( MeterAlarmState, &Para.RMeterAlarmState, 1 );
				ErrorByte = 0;	
			}
			else if( OICode == 0x8001 )						//保电解除
			{
				if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.30
				if( Judge_operating_authority( COM_No, OMD, 0x4110, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//				if( JudgeTimeTag( COM_No, WriteBufAds+1 ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.17
				Para.RMeterSafeState = 0x3B;
				ICComm.ControlComExeState = 0x0200;		//保电解除
				E2P_WData( MeterSafeState, &Para.RMeterSafeState, 1 );
				ErrorByte = 0;	
			}		
#endif
			break;	
		case 129:										
			if( OICode == 0xF101 )					//批量增加显示安全模式参数		//17.04.07
			{
				if( Judge_operating_authority( COM_No, OMD, 0x8120, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				if( Judge_SafeModeParaItem( WriteBufAds, ItemLen ) != 0 ) ErrorByte = type_unmatched;
				else if( AllNewOIIsNotExist( WriteBufAds ) != 0 ) ErrorByte = object_class_inconsistent; 
				else
				{
					AddAllSafeModeParaItems( WriteBufAds );					
					ErrorByte = 0;   					
				}	
			}	
#if ( GeneralSmartMeter	== NoCheck )
			else if( OICode == 0x8000 )							//跳闸
			{
				if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.30
				if( Judge_operating_authority( COM_No, OMD, 0x4110, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				ErrorByte = JudgeTripOFFPara( WriteBufAds, &OperateType );
				if( ErrorByte != 0 ) return ErrorByte;
//				if( JudgeTimeTag( COM_No, WriteBufAds+16 ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.17
				ErrorByte= TripOFFProcess( OperateType );
			}
			else if( OICode == 0x8001 )						//解除自动保电
			{
				
			}		
#endif
			break;
		case 130:										
#if ( GeneralSmartMeter	== NoCheck )
			if( OICode == 0x8000 )							//合闸
			{
				ErrorByte = JudgeTripONPara( WriteBufAds, &OperateType );
				if( ErrorByte != 0 ) return ErrorByte;
				
				if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.30
				if( *CommPara[COM_No].APPLState == APPLON2 )		//17.04.09
				{
					if( Judge_operating_authority( COM_No, OMD, 0x0080, active_authority ) != 0 ) return read_write_denied;	//17.04.09		//明文拉合闸			
				}
				else
				{	 
					if( Judge_operating_authority( COM_No, OMD, 0x4110, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				}	
//				if( JudgeTimeTag( COM_No, WriteBufAds+11 ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.17
				
				ErrorByte = TripONProcess( OperateType );			//17.04.17
			}			
#endif
			break;
		case 131:										
#if ( GeneralSmartMeter	== NoCheck )
			if( OICode == 0x8000 )							//明文合闸		//17.04.17
			{
				if( JudgeTimeTag( COM_No ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.30
				ErrorByte = JudgeTripONPara( WriteBufAds, &OperateType );
				if( ErrorByte != 0 ) return ErrorByte;
				if( *(WriteBufAds+11) != D_VisibleStr ) return type_unmatched;
				
				if( Judge_operating_authority( COM_No, OMD, 0x0080, active_authority ) != 0 ) return read_write_denied;	//17.04.09		//明文拉合闸			
				
				if( CheckConnectPassWord( WriteBufAds+12 ) != 0 ) 
				{
					ICComm.ControlComErrState = 0x04;		//密码错/未授权
					return scope_of_access_violated;					//密码错
				}	
//				if( JudgeTimeTag( COM_No, WriteBufAds+13+*(WriteBufAds+12) ) != 0 ) return time_tage_invalid;				//时间标签无效		//17.04.17
				
				ErrorByte = TripONProcess( OperateType );			//17.04.17
			}			
#endif
			break;
		default:
			break;	
	}	
	return ErrorByte;
}

//添加的显式安全模式格式是否正确
unsigned char Judge_SafeModeParaItem( unsigned char* Data, unsigned short* ItemLen )		//17.04.07
{
	short i;
	unsigned char* Ptr;
	unsigned short Temp;
			
	if( *Data != Data_array ) return type_unmatched;
	Temp = *(Data+1);
	Temp += Para.RSafeModeParaSum;	
	if( Temp > 100 ) return type_unmatched;			//添加后大于额定限数
	
	Ptr = Data+2;										
	*ItemLen += 2;
	Temp = *(Data+1);
	for( i=0;i<Temp;i++ )
	{
        if( *Ptr != Data_structure ) return type_unmatched;
        if( *(Ptr+1) != 0x02 ) return type_unmatched;
        if( *(Ptr+2) != D_OI ) return type_unmatched;
        if( *(Ptr+5) != D_Unsigned16 ) return type_unmatched;
      		
		Ptr += 8;		
		*ItemLen += 8;		
	}	
	
    return 0;
}


//是否已有相同OI存在
short NewOIIsNotExist( unsigned char* Source )			//17.04.07
{
	unsigned char Buff[4];
	unsigned char* Ptr;
	unsigned short Addr;
	unsigned short i;
	
	Ptr = Buff;
	
	Addr = SafeModePara1;
	for( i=0;i<Para.RSafeModeParaSum;i++ )
	{
		E2P_RData( Ptr, Addr, 4 );
		if(( *(Source+3) == *Ptr )&&(*(Source+4) == *(Ptr+1))) break;
		Addr += 5;	
	}	
	if( i == Para.RSafeModeParaSum ) return 0;
	else return -1;
}	

//是否已有相同OI存在
short AllNewOIIsNotExist( unsigned char* Source )			//17.04.07
{
	unsigned char Buff[420];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned short Addr;
	unsigned short Items;
	unsigned short i,j;	
	
	Ptr = Buff;
	
	Addr = SafeModePara1;
	if( Para.RSafeModeParaSum > 100 ) return -1;
	for( i=0;i<Para.RSafeModeParaSum;i++ )
	{
		E2P_RData( Ptr, Addr, 4 );
		Addr += 5;	
		Ptr += 4;
	}	

	Ptr = Buff;
	Items = *(Source+1);
	for( i=0;i<Para.RSafeModeParaSum;i++ )
	{
		Point = Source+2;					//结构体首址
		for( j=0;j<Items;j++ )
		{
			if(( *Ptr == *(Point+3) )&&( *(Ptr+1) == *(Point+4) )) return -1;
			Point += 8;						//跨过一个结构体
		}	
		Ptr += 4;							//已存在的下一条OI
	}
	
	return 0;	
}	

void AddAllSafeModeParaItems( unsigned char* Source )		//17.04.07
{
	unsigned short Items;
	unsigned short i;	
	unsigned char* Ptr;
	
	Items = *(Source+1);
	Ptr = Source+2;	
	for( i=0;i<Items;i++ )
	{
		AddSafeModeParaItem( Ptr );
		Para.RSafeModeParaSum += 1;
		Ptr += 8;
	}	
	E2P_WData( SafeModeParaSum, (unsigned char*)&Para.RSafeModeParaSum, 1 );

}	

//添加一项显式安全模式参数		//17.04.07
void AddSafeModeParaItem( unsigned char* Source )		//17.04.07
{
	unsigned char Buff[4];
	unsigned char* Ptr;
	unsigned short Addr;
	
	Ptr = Buff;
	
	*Ptr = *(Source+3); 			//OI
	*(Ptr+1) = *(Source+4); 	
	*(Ptr+2) = *(Source+6); 		//权限	//高位在前
	*(Ptr+3) = *(Source+7); 	
	
	Addr = Para.RSafeModeParaSum;
	Addr *= 5;
	Addr += SafeModePara1;
	
	E2P_WData( Addr, Ptr, 4 );	
}	

//删除一项显式安全模式参数		//17.04.07
void DeleteSafeModeParaItem( unsigned char* Source )	//17.04.07
{
	unsigned char Buff[4];
	unsigned char* Ptr;
	unsigned short Addr;
	unsigned short i,j;
	
	Ptr = Buff;
	
	Addr = SafeModePara1;
	for( i=0;i<Para.RSafeModeParaSum;i++ )
	{
		E2P_RData( Ptr, Addr, 4 );
		if(( *(Source+1) == *Ptr )&&(*(Source+2) == *(Ptr+1))) break;
		Addr += 5;	
	}	

	if( i == Para.RSafeModeParaSum ) return;
	
	j = i;
	Addr = SafeModePara1;
	for( j=(i+1);j<Para.RSafeModeParaSum;j++ )			//后续数据向前递进一位
	{
		E2P_RData( Ptr, Addr+j*5, 4 );
		E2P_WData( Addr+(j-1)*5, Ptr, 4 );
	}	

	Para.RSafeModeParaSum -= 1;
	E2P_WData( SafeModeParaSum, (unsigned char*)&Para.RSafeModeParaSum, 1 );

}	


short BroadC_Time698( unsigned char* Source )
{
	unsigned char Buff[8];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	if( *Source != D_date_time_s ) return 1;
	RAM_Write( Ptr, Source+1, 7 );
    SwapData( Ptr, 7 );
	ChangeTimeHexToBCD( Ptr, Ptr );	
	if( BroadC_Time( Ptr ) == 0 )
	{				
		RAM_Write( &Clk.YearH, Source+1, 7 );	//当前时间
		Clk.Sec_64 = 32;
       	RAM_Write( &Clk.Sec[0], Ptr, 6 );
		E2Pwrite( 0x0, Clk.Sec, 3, RTCAds );	//写RX8025时钟芯片
		SM.TimeSet_Days = 0;
		SM.BroadC_Mins = 0x05A0;
//		*ComRecTmr = 2;
		GetDaySeg();				
		GetFeeNo();		
		MD_InitialCalPara( Para.RMaxT );		

//		SM.TimeSetLoadAdjDelay = 0;							//cru		//广播校时后立即根据当前时间进行负荷曲线调整		
//		SM.LoadAdjMode = 0x3F;
//		E2P_WData( ELoadAdjMode, (unsigned char*)&SM.LoadAdjMode, 1 ); 		//cru	//需进行调整的负荷曲线
//		SM.LoadAdjState = LoadAdjCruve1;	
//		SM.LoadAdjStateChk = LoadAdjCruve1 + 0xA5A5;		
	}else return 1;
	
	return 0;
}	

/*
//判断时间标签						
short JudgeTimeTag( unsigned short COM_No, unsigned char* Source )				//17.04.17
{
	unsigned char Buff[8];
	unsigned char Buff2[8];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char* TimeTag;			//17.04.18
	
	Ptr = Buff; 
	Point = Buff2;
	TimeTag = CommPara[COM_No].TimeTag;				//通讯口时间标签	optional:1字节，data_time_s:7字节，TI:3字节		//17.04.18
	
	*TimeTag = 0;
	if( *Source != 1 ) return -1;
	if( *(Source+8) > 5 ) return -1;	
	if( Check_date_time_s( Source+1 ) != 0 ) return -1;		//时间非法

	RAM_Write( Ptr, Source+1, 7 );
	SwapData( Ptr, 7 );
	Datetimes_Add_TI( Ptr, Ptr, *(Source+8), SwapCharToShort( Source+9 ) );
	
	RAM_Write( Point, &Clk.YearH, 7 );
	SwapData( Point, 7 );
	if( Data_Comp( Point, Ptr, 7 ) > 0 ) return -1;		//时间加延时小于当前时间，无效
	
	RAM_Write( TimeTag, Source, 11 );					//17.04.18
	return 0;
}	
*/
//判断时间标签						
short JudgeTimeTag( unsigned short COM_No )				//17.04.30
{
	unsigned char* TimeTag;			//17.04.18
	
	TimeTag = CommPara[COM_No].TimeTag;				//通讯口时间标签	optional:1字节，data_time_s:7字节，TI:3字节		//17.04.18
	
	if( *TimeTag != 1 ) return -1;
	
	return 0;
}	

#if ( GeneralSmartMeter	== NoCheck )
short JudgeTripONPara( unsigned char* Source, unsigned char* Type )
{
	if(( *Source != Data_array )&&( *(Source+1) != 0x01 )) return type_unmatched;
	if(( *(Source+2) != Data_structure )&&( *(Source+3) != 0x02 )) return type_unmatched;
	if(( *(Source+4) != D_OAD )&&( *(Source+9) != D_ENUM )) return type_unmatched;
	if( *(Source+10) == 0 ) *Type = 0x1B;
	else *Type = 0x1C;
	
	return 0;
}	

//合闸处理
short TripONProcess( unsigned char OperateType )			//17.04.17
{
	ICComm.ControlComErrState = 0;

	if( OperateType == 0x1B )
	{											//合闸允许	
#if ( RelayONImmediately == YesCheck )
		if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0x55;	//立即合闸
#else       		
		if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0xAA;	//允许合闸
#endif      		
			ICComm.ControlComExeState = 0x0002;							//合闸允许
	}
	else								//0x1C:							//直接合闸			
	{	
		if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0x55;	//立即合闸
		ICComm.ControlComExeState = 0x0001;							//直接合闸
	}		
	ICComm.TripOFFDelay = 0;
	ICComm.PreTripOFFKeepTime = 0;
	ICComm.RelayOFFState = 0x00;				//收到远程直接合闸命令后，取消允许拉闸状态	//新国网	//13.10.25
	Para.RMeterTripState = 0x1B;
	E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );	
	
	return 0;
}

short JudgeTripOFFPara( unsigned char* Source, unsigned char* Type )
{	
	if(( *Source != Data_array )&&( *(Source+1) != 0x01 )) return type_unmatched;
	if(( *(Source+2) != Data_structure )&&( *(Source+3) != 0x04 )) return type_unmatched;
	if(( *(Source+4) != D_OAD )&&( *(Source+9) != D_Unsigned8 )&&( *(Source+11) != D_Unsigned16 )&&( *(Source+14) != D_BOOLEAN )) return type_unmatched;

	E2P_WData( TripOFFAlarmTime, Source+10, 1 );
	Para.RTripOFFAlarmTime = *(Source+10);
	Para.RTripOFFAlarmTime *= 60;

	ICComm.PreTripOFFKeepTime = SwapCharToShort( Source+12 );
	ICComm.PreTripOFFKeepTime *= 60;
	if( ICComm.PreTripOFFKeepTime == 0 ) *Type = 0x1A;				//跳闸
	else
	{
		if( *(Source+15) == 0 ) *Type = 0x1D;						//预跳闸（延时到直接合闸）		
		else *Type = 0x1E;                                          //预跳闸（延时到直接合闸允许）	
	}	
	return 0;
}	

//跳闸处理
short TripOFFProcess( unsigned char OperateType )		//新国网	//14.04.29
{
	ICComm.ControlComExeState = 0;
	ICComm.ControlComErrState = 0;
	
	if(( OperateType == 0x1D )||( OperateType == 0x1E)) 	//预跳闸
	{
		if( Para.RMeterSafeState == 0x3A )			//保电
		{
			ICComm.ControlComErrState = 0x40;			//预跳闸失败(保电)
			return -1;
		}	
		if(( Para.RMeterTripState == 0x1A )||(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0xAA )))			//跳闸、合闸允许	//14.06.05	
		{
			ICComm.ControlComErrState = 0x0100;			//预跳闸失败(跳闸)
			return -1;
		}	

		if( ICComm.PreTripOFFKeepTime == 0 )			//预跳闸持续时间
		{
			ICComm.ControlComErrState = 0x80;			//预跳闸持续时间无效
			return -1;
		}
	}		
	
	switch( OperateType )
	{
		case 0x1A:										//跳闸
			if( Para.RMeterSafeState != 0x3A )		//非保电
			{
				ICComm.PreTripOFFKeepTime = 0;
				ICComm.RelayONState = 0x00;			//收到远程拉闸命令后，取消允许合闸状态	//新国网	//13.10.25
				if(( Para.RMeterRelayState == 0x1A ) 	//远程跳闸且已跳闸，该命令可忽略。
					&&(( Para.RMeterTripState == 0x1A )||((( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E ))&&(ICComm.PreTripOFFKeepTime != 0))))
				{
					ICComm.TripOFFDelay = 0;
					ICComm.ControlComExeState = 0x0004;					//直接跳闸
#if ( MagnKeepRelay	== YesCheck )
#else
					if( ICComm.RelayDrivePulseWidth == 0 ) RelayDrive( RELAY_OFF );			//新国网	//13.10.17
#endif
				}	
				else if(( Para.RTripOFFAlarmTime == 0 )&&( Para.RMeterRelayState != 0x1A ))
				{
					ICComm.TripOFFDelay = 0;			//11.05.28
					ICComm.RelayOFFState = 0x55;									//准备跳闸
					OperateRelay();
#if ( IOver30ALimitTrip	== YesCheck )		//新国网	//14.04.25
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
					if(( IOver30AJudge() != 0 )&&( SM.Up30ATimeCount < Para.RUp30AClose24HOUR ))	//新国网	//14.06.19	
#else
					if(( IOver30AJudge() != 0 )&&( SM.Up30ATimeCnt != UP30A24HOUR ))
#endif
					{
						ICComm.ControlComExeState = 0x0020;					//延时跳闸(大电流)
					}
					else ICComm.ControlComExeState = 0x0004;					//直接跳闸
#else			
					ICComm.ControlComExeState = 0x0004;						//直接跳闸
#endif
				}
				else																							//新国网	//13.10.25
				{																								//新国网	//13.10.25
					if( Para.RMeterRelayState == 0x1A ) 
					{
						ICComm.TripOFFDelay = 0; 	//已跳闸，该命令可忽略。	//新国网	//13.10.25
						ICComm.ControlComExeState = 0x0004;					//直接跳闸
					}	
					else 
					{
						ICComm.TripOFFDelay = Para.RTripOFFAlarmTime;											//新国网	//13.10.25
						ICComm.ControlComExeState = 0x0008;					//延时跳闸（跳闸延时时间）
					}	
				}																								//新国网	//13.10.25	
			}	
			else
			{
				ICComm.ControlComErrState = 0x20;			//拉闸失败(保电)
				return -1;
			}		
			Flag.AlarmFlg[3] |= F_AlarmRemoteTrip;		//新国网	//13.09.14
			Para.RMeterTripState = OperateType;
			break;
		case 0x1D:										//预跳闸（延时到直接合闸）			//新国网
		case 0x1E:										//预跳闸（延时到直接合闸允许）			//新国网
			ICComm.TripOFFDelay = 0;			//11.05.28
			ICComm.RelayOFFState = 0x55;									//准备跳闸
			ICComm.RelayONState = 0x00;			//收到远程拉闸命令后，取消允许合闸状态	//新国网	//13.10.25
			Para.RMeterTripState = OperateType;		
			OperateRelay();
#if ( IOver30ALimitTrip	== YesCheck )		//新国网	//14.04.25
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			if(( IOver30AJudge() != 0 )&&( SM.Up30ATimeCount < Para.RUp30AClose24HOUR ))	//新国网	//14.06.19	
#else
			if(( IOver30AJudge() != 0 )&&( SM.Up30ATimeCnt != UP30A24HOUR ))
#endif
			{
				ICComm.ControlComExeState = 0x0020;					//延时跳闸(大电流)
			}
			else ICComm.ControlComExeState = 0x0010;					//预跳闸
#else			
			ICComm.ControlComExeState = 0x0010;						//预跳闸
#endif

			break;
		default: 
			ICComm.ControlComErrState = 0x04;		//密码错/未授权
			return -2;
			break;			
	}	
	E2P_WData( MeterTripState, &OperateType, 1 );			

	return 0;
}
#endif

