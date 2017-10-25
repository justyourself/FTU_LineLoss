#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Measure.h"
#include "Port.h"
//#include "Type.h"

#include "TypeRAM.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"

#include "Mem.h"
#include "Time.h"
#include "Power.h"
#include "Initial.h"
#include "RegisterActivation.h"

#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "ActCalendar.h"
#include "RegisterMonitor.h"
#include "SingleAction.h"
#include "ProfileGeneric.h"
///#include "Demand.h"
#include "DisconnectControl.h"
#include "ScriptTable.h"
#include "AssociationLN.h"
#include "Limiter.h"
#include "ComSetup.h"
//取Register_monitor类数据
short GetRegister_monitorClassData( unsigned char* Dest, unsigned char* Source ) 
{
	short i,j;
	unsigned char Buff[20];
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned short Addr;
	unsigned char Len=2;
	unsigned char Attribute;
	
	Ptr = Logical_Name;
	Point = Buff;
	RAM_Write( Ptr, Source+2, 6 );
	
	for( i=0;i<DLMS_Reg_monitorSum;i++ )
	{
		if( Data_Comp( Ptr, (unsigned char*)DLMS_Reg_monitorTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	if( i == DLMS_Reg_monitorSum )
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}
	else 
	{
		Addr = DLMS_Reg_monitorTab[i].Addr;
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:										//logical_name
				*(Dest+1) = Data_octet_string;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:										//thresholds	
				*(Dest+1) = Data_array;
				*(Dest+2) = 0x01;
                Addr += Register_monitor1_thresholds1;
                Addr -= Register_monitor1_thresholds_Number;
				E2P_RData( Buff, Addr, 5 );
				*(Dest+3) = Buff[0];
				RAM_Write( Dest+4, Point+1, 4 );
				Len = 8;					
				break;	
			case 3:										//monitored_value
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x03;
                Addr += Register_monitor1_value;
                Addr -= Register_monitor1_thresholds_Number;
				E2P_RData( Buff, Addr, 9 );
				*(Dest+3) = D_Unsigned16;
				RAM_Write( Dest+4, Buff, 2 );
				*(Dest+6) = D_OctetStr;
				*(Dest+7) = 0x06;
				RAM_Write( Dest+8, Point+2, 6 );
				*(Dest+14) = D_Integer8;
				*(Dest+15) = Buff[8];
				Len = 16;					
				break;	
			case 4:										//actions	
				*(Dest+1) = Data_array;
				*(Dest+2) = 0x01;
				*(Dest+3) = Data_structure;
				*(Dest+4) = 0x02;
                Addr += Register_monitor1_action_Set1;
                Addr -= Register_monitor1_thresholds_Number;
				E2P_RData( Point, Addr, 16 );
				Ptr = Dest+5;
				for(j=0;j<2;j++ )
				{
					*Ptr = Data_structure;
					*(Ptr+1) = 0x02;
					*(Ptr+2) = D_OctetStr;
					*(Ptr+3) = 0x06;
					RAM_Write( Ptr+4, Point, 6 );
					*(Ptr+10) = D_Unsigned16;
					RAM_Write( Ptr+11, Point+6, 2 );
					Ptr += 13;
					Point += 8;					
				}					
				Len = 31;
				break;	
			default:
				break;	
		}	
	}	

	return Len;
}	

short Judge_thresholds( unsigned char* Data )
{
	if(( *Data != Data_array )||( *(Data+1) != 1 )) 
	{
		return other_reason;
	}
	return 0;
}

short Judge_monitored_value( unsigned char* Data )
{
	if(( *Data != Data_structure )||( *(Data+1) != 3 )) 			
	{
		return other_reason;
	}
	if(( *(Data+2) != D_Unsigned16 )||( *(Data+5) != Data_octet_string )||( *(Data+6) != 6 )||( *(Data+13) != D_Integer8 )) 
	{
		return other_reason;
	}		
    return 0;
}

short Judge_actions( unsigned char* Data )
{
	short i;
	unsigned char* Source;
	
	if(( *Data != Data_array )||( *(Data+1) != 1 )||( *(Data+2) != Data_structure )||( *(Data+3) != 2 ))						//只支持1组操作 
	{
		return other_reason;
	}
	Source = Data+4;
	for( i=0;i<2;i++ )
	{
		if(( *Source != Data_structure )||( *(Source+1) != 2 )||( *(Source+2) != Data_octet_string )||( *(Source+3) != 6 )||( *(Source+10) != D_Unsigned16 )) 
		{
			return other_reason;
		}
		Source += 13;
	}	
	return 0;
}

unsigned char SetRegister_monitorClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
//	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
//	unsigned char* DataType;
//	unsigned char AccessMode;
	unsigned short Addr;
    unsigned short i;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
//	DataType = WriteBufAds;
	
	for( i=0;i<DLMS_Reg_monitorSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_Reg_monitorTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	if( i == DLMS_Reg_monitorSum ) { return object_undefined; }
	
	if(( *Attribute == 1 )||( DLMS_Reg_monitorTab[i].ComInfo & WRITE ) == 0 ) { return read_write_denied; }

	Addr = DLMS_Reg_monitorTab[i].Addr;

	switch( *Attribute )
	{
//		case 1:														//logical_name			
//			break;	
		case 2:														//thresholds		
			ErrorByte = Judge_thresholds( WriteBufAds );
			if( ErrorByte == 0 )
			{				
				*Ptr = 0x01;
				E2P_WData( Addr, Ptr, 1 );					
//				E2P_WData( Addr+Register_monitor1_action_Number-Register_monitor1_thresholds_Number, Ptr, 1 );					
                Addr += Register_monitor1_thresholds1;
                Addr -= Register_monitor1_thresholds_Number;
				E2P_WData( Addr, WriteBufAds+2, 5 );					
			}	
			break;	
		case 3:														//monitored_value		
			ErrorByte = Judge_monitored_value( WriteBufAds );
			if( ErrorByte == 0 )
			{				
				RAM_Write( Ptr, WriteBufAds+3, 2 );
				RAM_Write( Ptr+2, WriteBufAds+7, 6 );
				RAM_Write( Ptr+8, WriteBufAds+14, 1 );
                Addr += Register_monitor1_value;
                Addr -= Register_monitor1_thresholds_Number;
				E2P_WData( Addr, Ptr, 9 );					
			}	
			break;	
		case 4:														//actions		
			ErrorByte = Judge_actions( WriteBufAds );
			if( ErrorByte == 0 )
			{
				*Ptr = 0x01;
        Addr += Register_monitor1_action_Number;
        Addr -= Register_monitor1_thresholds_Number;
				E2P_WData( Addr, Ptr, 1 );					
				RAM_Write( Ptr, WriteBufAds+8, 6 );
				RAM_Write( Ptr+6, WriteBufAds+15, 2 );
				RAM_Write( Ptr+8, WriteBufAds+21, 6 );
				RAM_Write( Ptr+8+6, WriteBufAds+28, 2 );
//                Addr += Register_monitor1_action_Set1;
//                Addr -= Register_monitor1_thresholds_Number;
                Addr += 2;
				E2P_WData( Addr, Ptr, 16 );					
			}	
			break;	
		default: 
			ErrorByte = type_unmatched;
			break;			
	}
	return ErrorByte;
}	

void initialRegister_monitorPara( void )
{
	unsigned char Buff[18];
	unsigned char* Ptr;
	unsigned long Value;	
	unsigned short Addr;
	short i;
	
	Ptr = Buff;
	for( i=0;i<3;i++ )
	{
		Addr = DLMS_Reg_monitorTab[i].Addr;
		RAM_Fill( Ptr, 18 );
		*Ptr = 0x01;
		E2P_WData( Addr, Ptr, 1 );					
		Addr += 2;	
		*Ptr = D_Unsigned32;
		Value = 1760;
		FillLongToBuff( Ptr+1, Value );	
		E2P_WData( Addr, Ptr, 5 );
		Addr += 6;	
		*Ptr = 0x00;					
		*(Ptr+1) = 0x03;					
		*(Ptr+2) = 0x01;					
		*(Ptr+3) = 0x00;					
		*(Ptr+4) = 0x20+i*20;						
		*(Ptr+5) = 0x07;					
		*(Ptr+6) = 0x00;					
		*(Ptr+7) = 0xFF;					
		*(Ptr+8) = 0x02;							
		E2P_WData( Addr, Ptr, 9 );
		Addr += 10;
		*Ptr = 0x01;
		E2P_WData( Addr, Ptr, 1 );					
		Addr += 2;

		*Ptr = 0x00;					
		*(Ptr+1) = 0x00;					
		*(Ptr+2) = 0x0A;					
		*(Ptr+3) = 0x01;					
		*(Ptr+4) = 0x01;						
		*(Ptr+5) = 0xFF;					
		*(Ptr+6) = 0x00;					
		*(Ptr+7) = 0x01;					
		
		*(Ptr+8) = 0x00;					
		*(Ptr+9) = 0x00;					
		*(Ptr+10) = 0x0A;					
		*(Ptr+11) = 0x01;					
		*(Ptr+12) = 0x01;						
		*(Ptr+13) = 0xFF;					
		*(Ptr+14) = 0x00;					
		*(Ptr+15) = 0x02;					
		E2P_WData( Addr, Ptr, 16 );							
	}		
}
	
