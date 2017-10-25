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

//取Class22类数据
//short GetIOClassData( unsigned char* Dest, unsigned char* Source ) 
short GetIOClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 		//17.04.06
{
	short Len;

	RAM_Write( Dest, Source, 4 );

//	Len = GetIOClassDataTab( Dest+4, Source );
	Len = GetIOClassDataTab( COM_No, Dest+4, Source );				//17.04.06
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
//short GetIOClassDataTab( unsigned char* Dest, unsigned char* Source ) 
short GetIOClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ) 		//17.04.06
{
	short i;
	short Len;
	unsigned char Attribute;
	unsigned char* OAD;
	Len = -1;
	
	OAD = Source;
	Attribute = *(OAD+2) & 0x1F;
				
	i = GetIOClassTabItemNo( OAD );
	if( i != DL698_ClassId22Sum )
	{
		if(( Attribute >= 1 )&&( Attribute <= 2 ))			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_ClassId22_Tab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09
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
				Len = GetClass22Data( Dest, OAD );
				break;
			default:
				break;	
		}	
	}		
	return Len;
}	

unsigned short GetIOClassTabItemNo( unsigned char* OAD )
{
	unsigned short i;
	unsigned char Buff[4];
	unsigned char* NewOAD;
	
	NewOAD = Buff;
	RAM_Write( NewOAD, OAD, 4 );

	for( i=0;i<DL698_ClassId22Sum;i++ )
	{
		if( Data_Comp( NewOAD, (unsigned char*)DL698_ClassId22_Tab[i].OAD, 4 ) == 0 ) break;				//查找对应的OBIS码
	}	
	return i;	
}

short GetSpecClassId22( unsigned char* Dest, unsigned char* OAD )	
{
	short i;
	unsigned short Addr;
	short Len=0;
	unsigned char* Ptr;
	unsigned char Buff[4];
	unsigned char* NewOAD;
	unsigned char Sum1;
	short Length;
	unsigned short PortDescriptor;
	unsigned short EComDCB;
	
	NewOAD = Buff;
	RAM_Write( NewOAD, OAD, 4 );

	if(( *NewOAD == 0xF2 )&&(( *(NewOAD+1) == 0x01 )||( *(NewOAD+1) == 0x02 ))||( *(NewOAD+1) == 0x09 )) 	
	{
		Sum1 = 1;
		if( *(NewOAD+1) == 0x01 )			//485口
		{
#if (( CarryComm == NoCheck )&&( GeneralSmartMeter == YesCheck))				
			Sum1 = 2;
#endif
			PortDescriptor = PortDescriptor1;
			EComDCB = EComDCB1;
		}
		else if( *(NewOAD+1) == 0x02 )		//红外口
		{
			PortDescriptor = PortDescriptor0;
			EComDCB = EComDCB0;
		}
		else								//模块
		{
			PortDescriptor = PortDescriptor3;
			EComDCB = EComDCB3;			
		}					
		
		if( *(NewOAD+3) == 0 )
		{
			*Dest = Data_array;
			*(Dest+1) = Sum1;
			Ptr = Dest+2;
			Len += 2;
			for( i=0;i<Sum1;i++ )
			{	
				*(NewOAD+3) = i+1;
				Length = GetSpecClassId22( Ptr, NewOAD );	
				Ptr += Length;
				Len += Length;
			}				
		}
		else if( *(NewOAD+3) <= Sum1 )
		{
			Ptr = Dest;
			*Ptr = Data_structure;
			if( *(NewOAD+1) == 0x02 ) *(Ptr+1) = 2;		//红外2元素
			else *(Ptr+1) = 3;							//485和模块3元素		
			
			*(Ptr+2) = D_VisibleStr;
			*(Ptr+3) = 8;
			Addr = *(NewOAD+3) - 1;
			Addr *= 9;
			Addr += PortDescriptor;
			E2P_RData( Ptr+4, Addr, 8 );
			
			*( Ptr+12 ) = D_COMBCD;
			if( *(NewOAD+1) == 0x01 )					//485口
			{
				Addr = *(NewOAD+3) - 1;
				Addr *= 6;
				Addr += EComDCB;
			}else Addr = EComDCB;						//红外或485	
			E2P_RData( Ptr+13, Addr, 5 );
			
			if( *(NewOAD+1) == 0x01 ) 					//485
			{
				*(Ptr+18) = D_ENUM;
				Addr = *(NewOAD+3) - 1;
				Addr *= 2;
				Addr += PortFunction1;
				E2P_RData( Ptr+19, Addr, 1 );					
				Len = 20;
			}
			else if( *(NewOAD+1) == 0x02 ) 				//红外
			{
				Len = 18;	
			}	
			else										//模块		
			{
				*(Ptr+18) = Data_structure;
				*(Ptr+19) = 4;
				*(Ptr+20) = D_VisibleStr;
				*(Ptr+21) = 2;
				E2P_RData( Ptr+22, ModuleMaunf_No, 2 );					
				*(Ptr+24) = D_VisibleStr;
				*(Ptr+25) = 2;
				E2P_RData( Ptr+26, ModuleChip_No, 2 );					
				*(Ptr+28) = D_date;
				E2P_RData( Ptr+29, ModuleVERDate, 5 );					
				*(Ptr+34) = D_Unsigned16;
				E2P_RData( Ptr+35, ModuleSoftVER, 2 );					
				Len = 37;
			}	
		}
		else Len = -1;	
	}	
#if( GeneralSmartMeter == NoCheck )
	else if(( *NewOAD == 0xF2 )&&( *(NewOAD+1) == 0x05 ))						//继电器输出
	{
		if( *(NewOAD+3) == 0 )
		{
			*Dest = Data_array;
			*(Dest+1) = 0x01;
			Ptr = Dest+2;
			Len += 2;
			*(NewOAD+3) = 1;
			Length = GetSpecClassId22( Ptr, NewOAD );	
			Ptr += Length;
			Len += Length;
		}
		else if( *(NewOAD+3) == 0x01 )
		{
			Ptr = Dest;
			*Ptr = Data_structure;
			*(Ptr+1) = 4;		//红外2元素
			*(Ptr+2) = D_VisibleStr;
			*(Ptr+3) = 8;
			E2P_RData( Ptr+4, RealyDescriptor, 8 );
			*(Ptr+12) = D_ENUM;
			if( Para.RMeterRelayState == 0x1A ) *(Ptr+13) = 0x01;
			else *(Ptr+13) = 0x00;
			*(Ptr+14) = D_ENUM;
			E2P_RData( Ptr+15, SwitchAttribute, 1 );
			*(Ptr+16) = D_ENUM;
			*(Ptr+17) = 0x00;
			Len = 18;	
		}
	}	
#endif	
	else if(( *NewOAD == 0xF2 )&&( *(NewOAD+1) == 0x06 ))						//告警输出
	{
		if( *(NewOAD+3) == 0 )
		{
			*Dest = Data_array;
			*(Dest+1) = 0x01;
			*(Dest+2) = D_ENUM;
//			if( Para.RMeterAlarmState == 0x2A ) *(Dest+3) = 0x01;
			if(( HT_GPIOD->PTDAT & GPIOD_RLY_ALM ) != 0 )  *(Dest+3) = 0x01;	//17.04.12
			else *(Dest+3) = 0x00;
			Len = 4;
		}
		else if( *(NewOAD+3) == 0x01 )
		{
			*Dest = D_ENUM;
//			if( Para.RMeterAlarmState == 0x2A ) *(Dest+1) = 0x01;
			if(( HT_GPIOD->PTDAT & GPIOD_RLY_ALM ) != 0 )  *(Dest+3) = 0x01;	//17.04.12
			else *(Dest+1) = 0x00;
			Len = 2;
		}
	}	
	else if(( *NewOAD == 0xF2 )&&( *(NewOAD+1) == 0x07 ))						//多功能端子
	{
		if( *(NewOAD+3) == 0 )
		{
			*Dest = Data_array;
			*(Dest+1) = 0x01;
			*(Dest+2) = D_ENUM;
			*(Dest+3) = SM.RPulseOutMode;
			Len = 4;
		}
		else if( *(NewOAD+3) == 0x01 )
		{
			*Dest = D_ENUM;
			*(Dest+1) = SM.RPulseOutMode;
			Len = 2;
		}
	}		

	return Len;
}	


//读参数变量接口类属性数据，返回值：-1，无此数据项，非-1：返回数据长度
short GetClass22Data( unsigned char* Dest, unsigned char* OAD )
{
	short i;
	short Len;
	
	Len = -1;
	
	i = GetIOClassTabItemNo( OAD );
	if( i != DL698_ClassId22Sum )
	{
		*Dest = Get_data;								//正常数据返回	
		Len = 1;
		Len += GetSpecClassId22( Dest+1, OAD );
        return Len;
	}
	return Len;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ActionIOClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = ActionIOClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen, BackLen  );
	return ErrorByte;		

}	

unsigned char ActionIOClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=object_class_inconsistent;
	unsigned short OICode;
	unsigned long ComID;
	unsigned char* OMD;
	unsigned char* WriteBufAds;
	unsigned char Method;
	
	OMD = Source;
	WriteBufAds = Source+4;
	OICode = SwapCharToShort( OMD );
	Method = *(OMD+2);
	
	WriteBufAds += 2;	//添加结构体、结构元素两字节	//17.03.02	

	switch( Method )
	{
		case 1:										//复位
			break;	
		case 2:										//执行	
			break;	
		case 127:										
			if( OICode == 0xF201 )					//485
			{
				ComID = SwapCharToLong( WriteBufAds+1 );
				if( ComID == 0xF2010201 )
				{
					if( JudgeComDCB( WriteBufAds+6 ) != 0 ) return ErrorByte;
					if( Judge_operating_authority( COM_No, OMD, 0x8880, active_authority ) != 0 ) return read_write_denied;	//17.04.09
					E2P_WData( EComDCB1, WriteBufAds+6, 5 );
					E2P_WData( PortFunction1, WriteBufAds+13, 1 );
					SetCOMBandRate( RS485BaudRate1, (unsigned char*)&Comm.BandRate1, *(WriteBufAds+6) );
					ErrorByte = 0;
				}
#if (( CarryComm == NoCheck )&&( GeneralSmartMeter == YesCheck ))                                    	                        	
				else if( ComID == 0xF2010202 )
				{
					if( JudgeComDCB( WriteBufAds+6 ) != 0 ) return ErrorByte;
					if( Judge_operating_authority( COM_No, OMD, 0x8880, active_authority ) != 0 ) return read_write_denied;	//17.04.09
					E2P_WData( EComDCB2, WriteBufAds+6, 5 );
					E2P_WData( PortFunction2, WriteBufAds+13, 1 );										
					SetCOMBandRate( RS485BaudRate2, (unsigned char*)&Comm.BandRate2, *(WriteBufAds+6) );
					ErrorByte = 0;
				}	
#endif
			}	
			else if( OICode == 0xF205 )				//继电器输出
			{
				if( Judge_operating_authority( COM_No, OMD, 0x8880, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				E2P_WData( SwitchAttribute, WriteBufAds+6, 1 );
				if( *(WriteBufAds+6) == 0 ) Para.RMeterRunMode1 = 0x01;										
				else Para.RMeterRunMode1 = 0x00;
				E2P_WData( MeterRunMode1, (unsigned char*)&Para.RMeterRunMode1, 1 ); 		//电表运行特征字
				ErrorByte = 0;
			}
			else if( OICode == 0xF207 )				//多功能端子
			{
				if( Judge_operating_authority( COM_No, OMD, 0x8880, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				SM.RPulseOutMode = *(WriteBufAds+6);
				if( SM.RPulseOutMode > 2 ) return ErrorByte;							//防止设置非法值	//17.03.02
				E2P_WData( PulseOutMode, (unsigned char*)&SM.RPulseOutMode, 1 ); 
				InitPara1();
				ErrorByte = 0;
			}
			break;	
		case 128:										
#if ( CarryComm == YesCheck )
			if( OICode == 0xF209 )					//模块
			{
				if( JudgeComDCB( WriteBufAds+6 ) != 0 ) return ErrorByte;
				if( Judge_operating_authority( COM_No, OMD, 0x8880, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				E2P_WData( EComDCB3, WriteBufAds+6, 5 );								
				SetCOMBandRate( RS485BaudRate2, (unsigned char*)&Comm.BandRate2, *(WriteBufAds+6) );
				ErrorByte = 0;
			}		
#endif		
			break;	
		default:
			break;	
	}	
	return ErrorByte;
}

short JudgeComDCB( unsigned char* Source )
{
	if( *Source > Baut9600 ) return -1;						//300~9600
	if( *(Source+1) != 2 ) return -1;					//偶校验
	if( *(Source+2) != 8 ) return -1;					//数据位8位
	if( *(Source+3) != 1 ) return -1;					//1位停止位
	if( *(Source+4) > 2 ) return -1;	
    return 0;
}	

void SetCOMBandRate( unsigned short Addr, unsigned char* BandRate, unsigned char Data )
{
	*BandRate = Data;
	E2P_WData( Addr, BandRate, 1 );
}	