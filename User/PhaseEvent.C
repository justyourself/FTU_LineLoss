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

//#include "RsComm.h"
#include "interrupt.h"
#include "Mem.h"
#include "Time.h"
#include "Power.h"
//#include "Display.h"
//#include "FreezeData.h"
//#include "Load.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
//#include "Produce.h"
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif
//取Class6类数据
short GetPhaseEventData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short Len;

	RAM_Write( Dest, Source, 4 );

	Len = GetPhaseEventClassDataTab( COM_No, Dest+4, Source );
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

unsigned short GetEventClassTabItemNo( unsigned char* OAD )
{
	unsigned short i;
		
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{
		if( Data_Comp( OAD, (unsigned char*)DL698_Event_ProfileTab[i].OAD, 2 ) == 0 ) break;				//查找对应的OBIS码
	}	
	return i;	
}

//取PhaseEvent类数据
short GetPhaseEventClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source ) 
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
	unsigned short Temp;
	unsigned char* OAD;
	Len = -1;
	
    OAD = Source;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
				
	i = GetEventClassTabItemNo( OAD );
	if( i != DL698_Event_ProfileSum )
	{
		if((( Attribute >= 1 )&&( Attribute <= 4 ))||(( Attribute >= 10 )&&( Attribute <= 12 )))			//17.04.09
		{	
			if( Judge_operating_authority( COM_No, OAD, DL698_Event_ProfileTab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09
		}
		*Dest = Get_data;								//正常数据返回		
		switch( Attribute )
		{
			case 1:										//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x02;
				RAM_Write( Dest+3, OAD, 2 );
				Len = 5;
				break;	
			case 2:										//关联对象属性表	
				Len = GetEvent_capture_objectsData( COM_No, Dest, i );
				break;	
			case 3:										//
				Len = GetClass24_3Data( Dest, OAD, i );
				break;	
			case 4:										//最大记录数	
				*(Dest+1) = D_Unsigned16;
//				E2P_RData( Dest+2, DL698_Event_ProfileTab[i].EntriesAddr, 2 );
				E2P_RData( (unsigned char*)&Temp, DL698_Event_ProfileTab[i].EntriesAddr, 2 );
				if( Temp > 10 ) Temp = 10;
				RAM_Write( Dest+2, (unsigned char*)&Temp, 2 );
				
				SwapData( Dest+2, 2 );
				Len = 4;
				break;
				break;	
			case 5:										//配置参数	
//				Len = GetClass24_5_13Data( Dest, OAD );
				Len = GetClass24_5_13Data( COM_No, Dest, OAD );		//17.04.09
				break;
			case 6:										//	
				break;
			case 7:										//	
				break;
			case 8:										//
				break;
			case 9:										//
				break;
			case 10:									//
				Len = GetClass24_10Data( Dest, OAD, i );
				break;
			case 11:									//上报标识
				*(Dest+1) = D_ENUM;
				E2P_RData( Dest+2, DL698_Event_ProfileTab[i].EReportFlagAddr,  1 );	
				Len = 3;
				break;
			case 12:									//有效标识
				*(Dest+1) = D_BOOLEAN;
				E2P_RData( Dest+2, DL698_Event_ProfileTab[i].EValidFlagAddr,  1 );	
				Len = 3;
				break;
			case 13:									//
//				Len = GetClass24_5_13Data( Dest, OAD );
				Len = GetClass24_5_13Data( COM_No, Dest, OAD );		//17.04.09
				break;
			default:
				break;	
		}	
	}		
	return Len;
}	

//取Class7类数据
short GetRecordPhaseEventClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short Len;

	RAM_Write( Dest, Source, 4 );

	Len = GetRecordPhaseEventClassDataTab( COM_No, Dest+4, Source );
	if( Len == -1 )  
	{
//		*(Dest+4) = 0x00;									//SEQUENCE OF RCSD == 0
//		*(Dest+5) = Get_data_access_result;					//异常标志
//		*(Dest+6) = object_undefined; 						//无对应的OBIS码
////		Len = 7;										//长度不对
//		Len = 3;											//17.04.30	
		Len = GetRecordAbnormalResponse( Dest, Source, object_undefined );	//17.04.30
	}
	else if( Len == -2 )										//17.04.09
	{
//		*(Dest+4) = 0x00;									//SEQUENCE OF RCSD == 0
//		*(Dest+5) = Get_data_access_result;					//异常标志
//		*(Dest+6) = read_write_denied; 						//操作权限不足
////		Len = 7;										//长度不对
//		Len = 3;											//17.04.30	
		Len = GetRecordAbnormalResponse( Dest, Source, read_write_denied );	//17.04.30
	}	
	Len += 4;
	return Len;		
}		

//取Event类数据
short GetRecordPhaseEventClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short i;
	short Len;
	unsigned char Attribute;
	unsigned char* OAD;
	Len = -1;
	
	OAD = Source;
	Attribute = *(OAD+2) & 0x1F;
					
	i = GetEventClassTabItemNo( OAD );
	if( i != DL698_Event_ProfileSum )
	{
		if(( Attribute >= 6 )&&( Attribute <= 9 ))			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_Event_ProfileTab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09		
		}	
		*Dest = Get_data;								//正常数据返回	
		switch( Attribute )
		{
			case 6:										//总	
			case 7:										//A	
			case 8:										//B	
			case 9:										//C	
				Len = GetEventBufferData( COM_No, Dest, Source, i+Attribute-6 );
//				DelFromNewEventList( COM_No, OAD );		//17.03.24
				DelFromNewReportEventList( COM_No, OAD );		//17.05.19
				break;	
			default:
				break;	
		}	
	}		
	return Len;
}	

//读分相事件接口类属性3数据，返回值：-1，无此数据项，非-1：返回数据长度
short GetClass24_3Data( unsigned char* Dest, unsigned char* OAD, unsigned short ItemNo )
{
	short i,j;
//	unsigned short Addr;
	short Len;
	short Start,End;
	unsigned char* Ptr;
	unsigned short Temp;

	Len = -1;
	
	if( *(OAD+3) > 4 ) return Len;
	
	i = ItemNo;
	if( *(OAD+3) == 0 )
	{
		Start = 0;
		End = 4;
		*(Dest+1) = Data_structure;
		*(Dest+2) = End;
		Ptr = Dest+3;
		Len = 3;
	}	
	else
	{
		Start = 0;
		End = 1;					
		Ptr = Dest+1;
		Len = 1;
		i += *(OAD+3) - 1;
	}	 
	for( j=Start;j<End;j++ )
	{
		*Ptr = D_Unsigned16;
//		Addr = DL698_Event_ProfileTab[i].EntriesUseAddr;
//		E2P_RData( Ptr+1, Addr, 2 );
		E2P_RData( (unsigned char*)&Temp, DL698_Event_ProfileTab[i].EntriesUseAddr, 2 );
		if( Temp > 10 ) Temp = 10;
		RAM_Write( Ptr+1, (unsigned char*)&Temp, 2 );
		
		SwapData( Ptr+1, 2 );
		Ptr += 3;
		Len += 3;
		i++;
	}		

	return Len;	
}

//读分相事件接口类属性10数据，返回值：-1，无此数据项，非-1：返回数据长度
short GetClass24_10Data( unsigned char* Dest, unsigned char* OAD, unsigned short ItemNo )
{
	short i,j;
	unsigned short Addr;
	short Len;
	short Start,End;
	unsigned char* Ptr;

	Len = -1;
	
	if( *(OAD+3) > 4 ) return Len;
	
	i = ItemNo;
	if( *(OAD+3) == 0 )
	{
		Start = 0;
		End = 4;
		*(Dest+1) = Data_array;
		*(Dest+2) = End;
		Ptr = Dest+3;
		Len = 3;
	}	
	else
	{
		Start = 0;
		End = 1;					
		Ptr = Dest+1;
		Len = 1;
		i += *(OAD+3) - 1;
	}	 
	for( j=Start;j<End;j++ )
	{
		*Ptr = Data_structure;
		*(Ptr+1) = 2;
		*(Ptr+2) = D_Unsigned32;
		Addr = DL698_Event_ProfileTab[i].CuntAddr;
		E2P_RData( Ptr+3, Addr, 4 );
		SwapData( Ptr+3, 4 );
		*(Ptr+7) = D_Unsigned32;
//		Addr = DL698_Event_ProfileTab[i].TimeAddr;
//		E2P_RData( Ptr+8, Addr, 4 );
//		SwapData( Ptr+8, 4 );
		GetEventCumTime( Ptr+8, i );
		Ptr += 12;
		Len += 12;
		i++;
	}		

	return Len;	
}

void AddPhFailCuntZ( void )
{
	unsigned long Value;	

	E2P_RData( (unsigned char*)&Value, PhFailCuntZ, 4 );
	Value += 1;
	E2P_WData( PhFailCuntZ, (unsigned char*)&Value, 4 );
}	

void AddPhFailTimeZ( unsigned long Value )
{
	unsigned long Value1;	

	E2P_RData( (unsigned char*)&Value1, PhFailTimeZ, 4 );
	Value1 += Value;
	E2P_WData( PhFailTimeZ, (unsigned char*)&Value1, 4 );
}	

void SavePhFailStartEndTime( unsigned short Addr, unsigned char* Time )
{
	unsigned char Buff[8];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, Time, 7 ); 
	SwapData( Ptr, 7 );
	E2P_WData( Addr, Ptr, 7 );
}	 

void ClearPhFailData( void )
{
	unsigned char Buff[8];
	unsigned char* Ptr;
	
	Ptr = Buff;

	RAM_Fill( Ptr, 8 );

	E2P_WData( PhFailCuntZ, Ptr, 4 );				
	E2P_WData( PhFailTimeZ, Ptr, 4 );				
	E2P_WData( PhFailStartTime, Ptr, 7 );				
	E2P_WData( PhFailEndTime, Ptr, 7 );				
}	

//读分相事件接口类属性5-13数据，返回值：-1，无此数据项，非-1：返回数据长度
//short GetClass24_5_13Data( unsigned char* Dest, unsigned char* OAD )
short GetClass24_5_13Data( short COM_No, unsigned char* Dest, unsigned char* OAD )				//17.04.09
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
	Len = -1;
	
	for( i=0;i<DL698_ClassId24_5Sum;i++ )
	{
		if( Data_Comp( OAD, (unsigned char*)DL698_ClassId24_5_Tab[i].OAD, 4 ) == 0 ) break;				//查找对应的OAD码
	}	
	if( i != DL698_ClassId24_5Sum )
	{
		if( Judge_operating_authority( COM_No, OAD, DL698_ClassId24_5_Tab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09
		*Dest = Get_data;								//正常数据返回	
		DataType = DL698_ClassId24_5_Tab[i].DataType;
		if(( DataType == Data_array )||( DataType == Data_structure )) 					
		{
			Start = 0;
			Element_Len = DL698_ClassId24_5_Tab[i].Element_Len;
			End = Element_Len;
			*(Dest+1) = DataType;
			*(Dest+2) = End;
			Ptr = Dest+3;
			Len = 3;
			i += 1;
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
			DataType = DL698_ClassId24_5_Tab[i].DataType;
			Element_Len = DL698_ClassId24_5_Tab[i].Element_Len;
//			if( DataType == D_Bit_String ) Element_Len /= 8;
//			BCD_Len = DL698_ClassId24_5_Tab[i].BCD_Len;
//			Multiple = DL698_ClassId24_5_Tab[i].Multiple;
			Addr = DL698_ClassId24_5_Tab[i].Addr;
//			E2P_RData( Ptr+1, Addr, BCD_Len );
			E2P_RData( Ptr+1, Addr, Element_Len );
			SwapData( Ptr+1, Element_Len );
			*Ptr = DataType;
//			GetRAMData_BCDToHEX( Ptr+1, Ptr+1, DataType, BCD_Len, Multiple );		
			Ptr += Element_Len + 1;
			Len += Element_Len + 1;
			i++;
		}		
	}
	return Len;	
}
//-------------------------------------------------------------------------------------------------------
unsigned char SetPhaseEventData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = SetPhaseEventClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen );
	return ErrorByte;		

}		

//取PhaseEvent类数据
unsigned char SetPhaseEventClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	short i,j;
	unsigned short Addr;
	unsigned char ErrorByte=object_class_inconsistent;
//	short Len;
//	unsigned short OICode;
//	short Temp;
    short Item;
	unsigned short GetLen;
//	short Start,End;
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
	unsigned char* Ptr;
	unsigned char* OAD;
	unsigned char* WriteBufAds;
//	Len = -1;
	
	OAD = Source;
	WriteBufAds = Source+4;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
				
	i = GetEventClassTabItemNo( OAD );
	if( i != DL698_Event_ProfileSum )
	{
		if(( Attribute == 2 )||( Attribute == 11 )||( Attribute == 12 ))			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_Event_ProfileTab[i].SaftInfo, write_authority ) != 0 ) return read_write_denied;	//17.04.09
		}
//		*Dest = Get_data;								//正常数据返回		
		switch( Attribute )
		{
			case 1:										//logical_name
				break;	
			case 2:										//关联对象属性表	
				ErrorByte = Judge_capture_objects( WriteBufAds, i, &GetLen, ItemLen );
				if( ErrorByte == 0 )
				{
					InitialEventProfilePara( i, GetLen );					//初始化事件曲线的部分参数
					Item = *(WriteBufAds+1);
					Ptr = WriteBufAds+2;
					Addr = DL698_Event_ProfileTab[i].CapObjAddr;
					for( j=0;j<Item;j++ )
					{
						E2P_WData( Addr, Ptr+1, 4 );						
						Addr += 5;
						Ptr += 5;
					}		
					E2P_WData( DL698_Event_ProfileTab[i].CapNumAddr, (unsigned char*)&Item, 1 );						
					ResetSignleClass24Event( DL698_Event_ProfileTab[i].EventNo );					//17.05.10
					CreatPrgRecEvent( OAD ); 
				}	
				break;	
			case 3:										//
				break;	
			case 4:										//	
				break;	
			case 5:										//配置参数	
//				ErrorByte = SetClass24_5Data( Source, DataLen, ItemLen );
				ErrorByte = SetClass24_5Data( COM_No, Source, DataLen, ItemLen );		//17.04.09
				if( ErrorByte == 0 )
				{
					CreatPrgRecEvent( OAD ); 
					InitPara2();
					InitPara3();
				}	
				break;
			case 6:										//	
				break;
			case 7:										//	
				break;
			case 8:										//
				break;
			case 9:										//
				break;
			case 10:									//
				break;
			case 11:									//上报标识
				if( *WriteBufAds != D_ENUM ) return type_unmatched;
				E2P_WData( DL698_Event_ProfileTab[i].EReportFlagAddr, WriteBufAds+1, 1 );						
				ErrorByte = 0;
				CreatPrgRecEvent( OAD ); 
				break;
			case 12:									//有效标识
				if( *WriteBufAds != D_BOOLEAN ) return type_unmatched;
				E2P_WData( DL698_Event_ProfileTab[i].EValidFlagAddr, WriteBufAds+1, 1 );						
				ErrorByte = 0;
				CreatPrgRecEvent( OAD ); 
				break;
			case 13:									//
				break;
			default:
				break;	
		}	
	}		
	return ErrorByte;
}
	
/*
void SetRAMData_HEXToBCD( unsigned char* Dest, unsigned char* Source, unsigned char DataType, unsigned char Element_Len, unsigned char BCD_Len, signed char Multiple )		
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char Temp=0;
	unsigned short Temp_U;
	short Temp_S;
	unsigned long Value_U;
	long Value_S;
//	unsigned char DataLen;
	
	Ptr = Buff;
	RAM_Fill( Ptr, 10);
	RAM_Write( Ptr, Source, Element_Len );
	if( BCD_Len == 0 ) return;
//	DataLen = BCD_Len - 1;
	switch( DataType )
	{
		case D_date_time_s:
			break;
		case D_Bit_String:
			break;	
		case D_Unsigned8:
			*Dest = Byte_BCD( *Ptr );
			break;
		case D_Integer16:		
			Temp_S = SwapCharToShort( Ptr );
			if( Temp_S < 0 ) 
			{
				Temp_S = 0 - Temp_S;
				Temp = 1;
			}
			Word_BCD( Dest, Temp_S );
			if( Temp == 1 ) *Dest |= 0x80;
			break;
		case D_Unsigned16:
			Temp_U = SwapCharToShort(Ptr);
			Word_BCD( Dest, Temp_U );
			break;	
		case D_Integer32:		
			Value_S = SwapCharToLong( Ptr );
			if( Value_S < 0 ) 
			{
				Value_S = 0 - Value_S;
				Temp = 1;
			}
			if( Multiple > 0 ) Value_S /= Multiple;
			else if( Multiple < 0 ) 
			{
				Multiple = 0 - Multiple;
				Value_S *= Multiple;
			}	
			Long_BCD4( Dest, Value_S );
			if( Temp == 1 ) *(Dest+BCD_Len-1) |= 0x80;		
			break;
		case D_Unsigned32:
			Value_U = SwapCharToLong( Ptr );
			if( Multiple > 0 ) Value_U /= Multiple;
			else if( Multiple < 0 ) 
			{
				Multiple = 0 - Multiple;
				Value_U *= Multiple;
			}	
			Long_BCD4( Dest, Value_U );
			break;			
		default:
			break;	
	}	
}	
*/

//unsigned char SetClass24_5Data( unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen )
unsigned char SetClass24_5Data( unsigned short COM_No, unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen )		//17.04.09
{
//	unsigned char Buff[20];
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
//	unsigned char* Ptr;
//	Len = -1;
	
//	Ptr = Buff;
	for( i=0;i<DL698_ClassId24_5Sum;i++ )
	{
		if( Data_Comp( Source, (unsigned char*)DL698_ClassId24_5_Tab[i].OAD, 4 ) == 0 ) break;				//查找对应的OAD码
	}	
	if( i == DL698_ClassId24_5Sum ) { return object_undefined; }

	if( Judge_operating_authority( COM_No, Source, DL698_ClassId24_5_Tab[i].SaftInfo, write_authority ) != 0 ) return read_write_denied;	//17.04.09

	DataType = *(Source+4);
	if( DL698_ClassId24_5_Tab[i].DataType != DataType ) { return type_unmatched; }
	
	Source += 4;
	*ItemLen += 4;
	if(( DataType == Data_array )||( DataType == Data_structure )) 					
	{
		Start = 0;
		Element_Len = DL698_ClassId24_5_Tab[i].Element_Len;
		End = Element_Len;
        Source += 2;
        *ItemLen += 2;
		i += 1;
		if( End == 0 ) ErrorByte = object_class_inconsistent;
	}	
	else
	{
		Start = 0;
		End = 1;					
	}	 
	for( j=Start;j<End;j++ )
	{
		DataType = DL698_ClassId24_5_Tab[i].DataType;
		if( *Source != DataType ) { return type_unmatched; }
		DataLen -= 1;
		Element_Len = DL698_ClassId24_5_Tab[i].Element_Len;
//		if( DataType == D_Bit_String ) Element_Len /= 8;
		if( DataLen < Element_Len ) { return type_unmatched; }
//		BCD_Len = DL698_ClassId24_5_Tab[i].BCD_Len;
//		Multiple = DL698_ClassId24_5_Tab[i].Multiple;
		Addr = DL698_ClassId24_5_Tab[i].Addr;
//		SetRAMData_HEXToBCD( Ptr+1, Source+1, DataType, Element_Len, BCD_Len, Multiple );		
		SwapData( Source+1, Element_Len );
		E2P_WData( Addr, Source+1, Element_Len );
		Source += Element_Len + 1;
		DataLen -= Element_Len;
		*ItemLen += Element_Len + 1;
		i++;
	}
			
	return ErrorByte;
}	
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ActionPhaseEventClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = ActionPhaseEventClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen, BackLen  );
	return ErrorByte;		

}	

unsigned char ActionPhaseEventClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
//	unsigned char Buff[4];
//	short i,j;
	short i;
//	unsigned char* Addr;
	unsigned char ErrorByte=object_class_inconsistent;
//	short Len;
//	unsigned short OICode;
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
//	unsigned char* WriteBufAds;
	unsigned char Method;
//	unsigned short SaftInfo;		//17.04.09
//	Len = -1;
	
	OMD = Source;
//	WriteBufAds = Source+4;
//	OICode = SwapCharToShort( OMD );
	Method = *(OMD+2);
	
//	Ptr = Buff;
	
	i = GetEventClassTabItemNo( OMD );
	if( i == DL698_Event_ProfileSum ) { return object_undefined; }

	if(( Method == 1 )||(( Method >= 4 )&&( Method <= 5 )))			//17.04.09
	{
		if( Judge_operating_authority( COM_No, OMD, DL698_Freeze_ProfileTab[i].SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
	}
//	SaftInfo = DL698_Event_ProfileTab[i].SaftInfo;	//17.04.09
	switch( Method )
	{
		case 1:										//logical_name
//			*ItemLen += 6;
//			if( Judge_operating_authority( COM_No, OMD, SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//			RAM_Fill( Ptr, 4 );
//			for( j=0;j<4;j++ )
//			{
//				E2P_WData( DL698_Event_ProfileTab[i+j].CuntAddr, Ptr, 4 );
//				E2P_WData( DL698_Event_ProfileTab[i+j].TimeAddr, Ptr, 4 );
//				E2P_WData( DL698_Event_ProfileTab[i+j].EntriesUseAddr, Ptr, 2 );
//				E2P_WData( DL698_Event_ProfileTab[i+j].EFinalRecNoAddr, Ptr, 4 );
//				ClearEventVar( i+j );
//				ClearMDOverTamperTabVar( i+j );
//			}	
//			if( i == 0 ) ClearPhFailData();
//			UpdateMeterState4_7();			
			ResetSignleClass24Event( DL698_Event_ProfileTab[i].EventNo );					//17.05.10
			ErrorByte = 0;
//			*ItemLen += 6;
			break;	
		case 2:										//	
			break;	
		case 3:										
/*			//删除方法3		//17.04.14		
			*ItemLen += 8;
			if( Judge_operating_authority( COM_No, OMD, SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
			RecordStartEvent( i );
//			*ItemLen += 8;
			ErrorByte = 0;
*/
			break;	
		case 4:										//添加一个事件关联对象属性	
			ErrorByte = AddEventCapture_OAD( i, Source+5 );									//17.05.10
			if( ErrorByte != 0 ) ErrorByte = object_class_inconsistent;	
			break;	
		case 5:										//删除一个事件关联对象属性
			ErrorByte = DeleteEventCapture_OAD( i, Source+5 );								//17.05.10
			break;
		default:
			break;	
	}	
	return ErrorByte;
}

void ResetSignleClass24Event( unsigned short EventNo )					//17.05.10
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	short i,j;
	
	Ptr = Buff;
	
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{
		if( EventNo == DL698_Event_ProfileTab[i].EventNo ) break;
	}
	if( i == DL698_Event_ProfileSum ) return;
	
	RAM_Fill( Ptr, 4 );
	for( j=0;j<4;j++ )
	{
		E2P_WData( DL698_Event_ProfileTab[i+j].CuntAddr, Ptr, 4 );
		E2P_WData( DL698_Event_ProfileTab[i+j].TimeAddr, Ptr, 4 );
		E2P_WData( DL698_Event_ProfileTab[i+j].EntriesUseAddr, Ptr, 2 );
		E2P_WData( DL698_Event_ProfileTab[i+j].EFinalRecNoAddr, Ptr, 4 );
		E2P_WData( DL698_Event_ProfileTab[i+j].ECurRecNoAddr, Ptr, 2 );
		ClearEventVar( DL698_Event_ProfileTab[i+j].EventNo );
		ClearMDOverTamperTabVar( DL698_Event_ProfileTab[i+j].EventNo );
	}	
	if( i == 0 ) ClearPhFailData();
	UpdateMeterState4_7();		
}	

unsigned char EventOADIsAlreadyExist( unsigned short ItemNo, unsigned char* OAD )			//17.05.10
{
	short i;
	unsigned char Buff[10];
	unsigned char* Point;
	unsigned short CaptureItems;
	unsigned short CapObjAddr;
	
	Point = Buff;
	
	CaptureItems = GetEventCaptureItems( ItemNo );						//已存曲线数
    CapObjAddr = DL698_Event_ProfileTab[ItemNo].CapObjAddr;  			
    for( i=0;i<CaptureItems;i++ )
    {
    	E2P_RData( Point, CapObjAddr+i*5, 4 );
   		if( Data_Comp( Point, OAD, 4 ) == 0 )							//逐条判断已有的OAD曲线是否与准备添加的一致,只与OAD有关
   		{
   			return i+1;													//已有不允许添加
    	}
	}
	
	return 0;
}	

unsigned char AddEventCapture_OAD( unsigned short ItemNo, unsigned char* OAD )					//17.05.10
{
	unsigned char Buff[4];
	unsigned char* Point; 
	unsigned short Items;
	unsigned short GetLen;
	unsigned short Addr;
	unsigned short ClassId;
	unsigned short EventNo;
    unsigned short i;
	
	Point = Buff;
																			//冻结周期类型
	Items = GetEventCaptureItems( ItemNo );
	Items += 1;
	if( Items > DL698_Event_ProfileTab[ItemNo].CapNumMax ) return 1;					//OAD已全满，无法添加
	GetLen = 0;
	if(	Judge_capture_objects_content( OAD, &GetLen, 0 ) != 0 ) return 1;				//该OAD不支持 
	if( EventOADIsAlreadyExist( ItemNo, OAD ) != 0 ) return 1;						//该OAD已存在 

	Addr = DL698_Event_ProfileTab[ItemNo].CapObjAddr;
	for( i=0;i<(Items-1);i++ )
	{
		E2P_RData( Point, Addr, 4 );
		if( Judge_capture_objects_content( Point, &GetLen, 0 ) != 0 ) return 1; 
		Addr += 5;
	}	
	E2P_WData( Addr, OAD, 4 );
	InitialEventProfilePara( ItemNo, GetLen );					//初始化事件曲线的部分参数
	E2P_WData( DL698_Event_ProfileTab[ItemNo].CapNumAddr, (unsigned char*)&Items, 1 );						
	ClassId = DL698_Event_ProfileTab[ItemNo].Class_Id;
	EventNo = DL698_Event_ProfileTab[ItemNo].EventNo;
	if( ClassId == Class_id_Event )	ResetSignleClass7Event( EventNo );				//17.05.10
	else if( ClassId == Class_id_PhaseEvent ) ResetSignleClass24Event( EventNo );	

	return 0;
}	

unsigned char DeleteEventCapture_OAD( unsigned short ItemNo, unsigned char* OAD )					//17.05.10
{
	unsigned char Buff[14]; 
	unsigned char* Ptr;
	unsigned short Items;
	unsigned short GetLen;
	unsigned short INo;
	short i;
	unsigned short CapObjAddr;		//捕获项地址
	unsigned short ClassId;
	unsigned short EventNo;
	
	Ptr = Buff;

	Items = GetEventCaptureItems( ItemNo );
	if( Items == 0 ) return object_class_inconsistent;
	INo = EventOADIsAlreadyExist( ItemNo, OAD );
	if( INo == 0 ) return object_class_inconsistent;					//该OAD不存在 

	CapObjAddr = DL698_Event_ProfileTab[ItemNo].CapObjAddr;
	for( i=INo;i<Items;i++ )											//删除一项后捕获曲线参数整体前移
	{
		E2P_RData( Ptr, CapObjAddr+i*5, 4 );
		E2P_WData( CapObjAddr+(i-1)*5, Ptr, 4 );
	}
	Items -= 1;
	E2P_WData( DL698_Event_ProfileTab[ItemNo].CapNumAddr, (unsigned char*)&Items, 1 );		
	GetLen = 0;
	for( i=0;i<Items;i++ )
	{
		E2P_RData( Ptr, CapObjAddr, 4 );
		if( Judge_capture_objects_content( Ptr, &GetLen, 0 ) != 0 ) break; 
		CapObjAddr += 5;
	}	
	InitialEventProfilePara( ItemNo, GetLen );					//初始化事件曲线的部分参数
					
	ClassId = DL698_Event_ProfileTab[ItemNo].Class_Id;
	EventNo = DL698_Event_ProfileTab[ItemNo].EventNo;
	if( ClassId == Class_id_Event )	ResetSignleClass7Event( EventNo );				//17.05.10
	else if( ClassId == Class_id_PhaseEvent ) ResetSignleClass24Event( EventNo );	

	return 0;
}		
	