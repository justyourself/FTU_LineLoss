#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Measure.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"

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
//取Limiter类数据
short GetLimiterClassData( unsigned char* Dest, unsigned char* Source ) 
{
	short i,j;
	unsigned char Buff[20];
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned long Temp_long;
	
	Ptr = Logical_Name;
	Point = Buff;
	RAM_Write( Ptr, Source+2, 6 );
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x11;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;

	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}
	else 
	{
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
			case 2:										//monitored_value	
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x03;
				E2P_RData( Point, Limiter_monitored_value, 9 );
				*(Dest+3) = D_Unsigned16;
				RAM_Write( Dest+4, Point, 2 );
				*(Dest+6) = D_OctetStr;
				*(Dest+7) = 0x06;
				RAM_Write( Dest+8, Point+2, 6 );
				*(Dest+14) = D_Integer8;
				*(Dest+15) = *(Point+8);
				Len = 16;					
				break;
			case 3:										//threshold_active
//				E2P_RData( Dest+1, Limiter_emergency_profile_active, 1 );
//				if( *(Dest+1) == 1 ) E2P_RData( Dest+1, Limiter_threshold_emergency, 5 );
//				else E2P_RData( Dest+1, Limiter_threshold_normal, 5 );
//	//按照内部的功率单位进行换算..直通式是:KW,互感式是:W
//	#if ( METERTYPE == U230_I10A__100A300)		//直通式..
//		 		Temp_long = limiter.Ram_Limiter_threshold_active/10000;  //电表里功率是:0.1W
//	#else		//互感式..
//				Temp_long = limiter.Ram_Limiter_threshold_active/10;  //电表里功率是:0.1W
//	#endif
			//20140930后全部改为以W为单位，显示需要改为KW为单位..				
				Temp_long = limiter.Ram_Limiter_threshold_active/10;
				
				RAM_Write(  Dest+2, (unsigned char*)&Temp_long, 4 );		
				SwapData( Dest+2, 4 );		//高低位字节互换..
				*(Dest+1) = D_Unsigned32;
				Len = 6;					
				break;
			case 4:										//threshold_normal
			case 5:										//threshold_emergency	
				E2P_RData( Point, Limiter_threshold_normal+(Attribute-4)*6, 5 );
				*(Dest+1) = D_Unsigned32;
				RAM_Write( Dest+2, Point+1, 4 );
				Len = 6;					
				break;
			case 6:										//min_over_threshold_duration
				E2P_RData( Point, Limiter_min_over_threshold_duration, 4 );
				*(Dest+1) = D_Unsigned32;
				RAM_Write( Dest+2, Point, 4 );
				Len = 6;					
				break;
			case 7:										//min_under_threshold_duration	
				E2P_RData( Point, Limiter_min_under_threshold_duration, 4 );
				*(Dest+1) = D_Unsigned32;
				RAM_Write( Dest+2, Point, 4 );
				Len = 6;					
				break;
			case 8:										//emergency_profile	
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x03;
//				E2P_RData( Point, Limiter_emergency_profile, 12 );
				E2P_RData( Point, Limiter_emergency_profile, 18 );			//18个字节一起读写..
				*(Dest+3) = D_Unsigned16;
				RAM_Write( Dest+4, Point, 2 );			//2个字节紧急编号
				*(Dest+6) = D_OctetStr;
//				*(Dest+7) = 0x06;
				*(Dest+7) = 0x0C;
//				RAM_Write( Dest+8, Point+2, 6 );
				RAM_Write( Dest+8, Point+2, 12 );			//紧急起始时刻..
//				*(Dest+14) = D_Unsigned32;
				*(Dest+20) = D_Unsigned32;
//				RAM_Write( Dest+15, Point+8, 4 );
				RAM_Write( Dest+21, Point+14, 4 );		//紧急持续时间4个字节
//				Len = 19;					
				Len = 25;					
				break;
			case 9:										//emergency_profile_group_id_list	
				E2P_RData( Point, Limiter_emergency_profile_group_id_list, 12 );
				*(Dest+1) = Data_array;
//				*(Dest+2) = *Point;
				*(Dest+2) = *(Point+1);
				if( *(Dest+2) > 5 )
				{
					*(Dest+2) = 0;
					Len = 3;
					break;
				}
				else
				{
					Len = 3;
					for( i=0;i<*(Dest+2);i++ )
					{
						*(Dest+3+i*3) = D_Unsigned16;	
						RAM_Write( Dest+3+i*3+1, Point+2+i*2, 2 );
						Len += 3;
					}
				}		
				break;
			case 10:										//emergency_profile_active	
				*(Dest+1) = D_BOOLEAN;
//				E2P_RData( Dest+2, Limiter_emergency_profile_active, 1 );
//				if( *(Dest+2) != 1 ) 
//				{
//					if( *(Dest+2) != 0 )
//					{
//						*(Dest+2) = 0;
//						E2P_WData( Limiter_emergency_profile_active, Dest+2, 1 );
//					}						
//				}
				if(limiter.Limiter_RelayMode == 0xB2) *(Dest+2) = 1;	
				else *(Dest+2) = 0;
				Len = 3;
				break;
			case 11:										//actions	
//				*(Dest+1) = Data_array;
//				*(Dest+2) = 0x01;
//				*(Dest+3) = Data_structure;
//				*(Dest+4) = 0x02;
//				E2P_RData( Point, Limiter_action_Set1, 16 );
//				Ptr = Dest+5;

				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x02;
				E2P_RData( Point, Limiter_action_Set, 16 );
				Ptr = Dest+3;
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
//				Len = 31;
				Len = 29;				
				break;						
			default:
				break;	
		}	
	}		
	return Len;
}	

short Judge_emergency_profile( unsigned char* Data )
{
	if(( *Data != Data_structure )||( *(Data+1) != 3 )) 			
	{
		return other_reason;
	}
//	if(( *(Data+2) != D_Unsigned16 )||( *(Data+5) != Data_octet_string )||( *(Data+6) != 6 )||( *(Data+13) != D_Unsigned32 )) 
	if(( *(Data+2) != D_Unsigned16 )||( *(Data+5) != Data_octet_string )||( *(Data+6) != 12 )||( *(Data+19) != D_Unsigned32 )) 
	{
		return other_reason;
	}		
    return 0;
}

short Judge_emergency_profile_group_id_list( unsigned char* Data )
{
	short i;
	if(( *Data != Data_array )||( *(Data+1) > 5 )) 
	{
		return other_reason;
	}
	for( i=0;i<*(Data+1);i++ )
	{
		if( *(Data+2+i*3) != D_Unsigned16 ) return other_reason;				
	}	
	return 0;
}

short Judge_structure_actions( unsigned char* Data )
{
	short i;
	unsigned char* Source;
	
	if(( *Data != Data_structure )||( *(Data+1) != 2 )) 
	{
		return other_reason;
	}
	Source = Data+2;
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
/*
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
*/
unsigned char SetLimiterClassData( unsigned char *Source , unsigned short DataLen )
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
//	unsigned short Addr;
    unsigned short i;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
//	DataType = WriteBufAds;
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x11;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;

	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) { return object_undefined; }
	if(( *Attribute == 1 )||( *Attribute == 0x03 )||( *Attribute == 10 )) { return read_write_denied; }

	switch( *Attribute )
	{
		case 2:														//monitored_value		
			ErrorByte = Judge_monitored_value( WriteBufAds );
			if( ErrorByte == 0 )
			{				
				RAM_Write( Ptr, WriteBufAds+3, 2 );
				RAM_Write( Ptr+2, WriteBufAds+7, 6 );
				RAM_Write( Ptr+8, WriteBufAds+14, 1 );
				E2P_WData( Limiter_monitored_value, Ptr, 9 );				//一起读写..	
			}	
			break;	
//		case 3:														//threshold_active		//只读
//			break;
		case 4:														//threshold_normal		
		case 5:														//threshold_emergency		
//			if(( *WriteBufAds != D_Unsigned32 )&&( *WriteBufAds != D_Integer32 )) ErrorByte = other_reason;			
			if( *WriteBufAds != D_Unsigned32 ) ErrorByte = other_reason;			
			if( ErrorByte == 0 )
			{				
				E2P_WData( Limiter_threshold_normal+(*Attribute-4)*6, WriteBufAds, 5 );					
			}	
			break;	
		case 6:														//min_over_threshold_duration		
		case 7:														//min_under_threshold_duration		
			if( *WriteBufAds != D_Unsigned32 ) ErrorByte = other_reason;			
			if( ErrorByte == 0 )
			{				
//				E2P_WData( Limiter_min_over_threshold_duration+(*Attribute-6)*5, WriteBufAds, 4 );					
				E2P_WData( Limiter_min_over_threshold_duration+(*Attribute-6)*5, WriteBufAds+1, 4 );					
			}	
			break;	
		case 8:														//emergency_profile		
			ErrorByte = Judge_emergency_profile( WriteBufAds );
			if( ErrorByte == 0 )
			{				
				RAM_Write( Ptr, WriteBufAds+3, 2 );
//				RAM_Write( Ptr+2, WriteBufAds+7, 6 );
				RAM_Write( Ptr+2, WriteBufAds+7, 12 );				//13.04.09
//				RAM_Write( Ptr+8, WriteBufAds+14, 4 );
				RAM_Write( Ptr+14, WriteBufAds+20, 4 );
//				E2P_WData( Limiter_emergency_profile, Ptr, 12 );					
				E2P_WData( Limiter_emergency_profile, Ptr, 18 );		//设置的ID需要在LIST中..				
			}	
			break;	
		case 9:														//emergency_profile_group_id_list
			ErrorByte = Judge_emergency_profile_group_id_list( WriteBufAds );
			if( ErrorByte == 0 )
			{				
				RAM_Fill( Ptr, 12);
				*Ptr = Data_array;
				*(Ptr+1) = *(WriteBufAds+1);
				if(*(Ptr+1)>5) ErrorByte = other_reason;	 
				for( i=0;i<*(Ptr+1);i++ )
				{
					RAM_Write( Ptr+2+i*2, WriteBufAds+3+i*3, 2 );						
				}	
				E2P_WData( Limiter_emergency_profile_group_id_list, Ptr, 12 );	//最大支持预存5个LIST..				
			}	
			break;
	/*	case 10:										//emergency_profile_active				//只读..
			if( *WriteBufAds != D_BOOLEAN ) ErrorByte = other_reason;			
			if( ErrorByte == 0 )
			{	
					E2P_WData( Limiter_emergency_profile_active, WriteBufAds+1, 1 );
				}
				break;	
	*/		
		case 11:													//actions		
//		ErrorByte = Judge_actions( WriteBufAds );
			ErrorByte = Judge_structure_actions( WriteBufAds );
			if( ErrorByte == 0 )
			{
//				*Ptr = 0x01;
//				E2P_WData( Limiter_action_Number, Ptr, 1 );					
//				RAM_Write( Ptr, WriteBufAds+8, 6 );
//				RAM_Write( Ptr+6, WriteBufAds+15, 2 );
//				RAM_Write( Ptr+8, WriteBufAds+21, 6 );
//				RAM_Write( Ptr+8+6, WriteBufAds+28, 2 );
//				E2P_WData( Limiter_action_Set1, Ptr, 16 );					
				RAM_Write( Ptr, WriteBufAds+6, 6 );
				RAM_Write( Ptr+6, WriteBufAds+13, 2 );
				RAM_Write( Ptr+8, WriteBufAds+19, 6 );
				RAM_Write( Ptr+8+6, WriteBufAds+26, 2 );
				E2P_WData( Limiter_action_Set, Ptr, 16 );	
			}	
			break;	
		default: 
			ErrorByte = type_unmatched;
			break;			
	}
	
	if(*Attribute <= 10)	read_Threshold_for_MDEvent();  //更改LIMITER事件读取..
		
	return ErrorByte;
}	


void Ave_Power( void )		//获取功率数据,上电15分钟内不进行操作,每秒处理一次..
{
	//RAM_Fill( LIMITER.AvePower_Total, 4 );
	//RAM_Fill( LIMITER.AvePower_Pp, 4 );
//	if( limiter.AvePowerTime < 900 ) 
//	if( limiter.AvePowerTime < 120 )  			//test
//	{
//		limiter.AvePower_Total = 0;
//		limiter.AvePower_Pp = 0;
//
//		limiter.AvePowerTime++;
//		return;
//	}else limiter.AvePowerTime = 120;			//test
////	}else limiter.AvePowerTime = 900;
		
	if( Real.Pt >= 0 ) 
	{
		limiter.AvePower_Total = Real.Pt;
		limiter.AvePower_Pp = Real.Pt;
	}	
	else 
	{
		limiter.AvePower_Total = -Real.Pt;
		limiter.AvePower_Pp = 0;
	}

	//继电器监视取值..
  if(limiter.Limiter_monitored_NO==0) 		limiter.Ram_Limiter_monitored_value = limiter.AvePower_Total;
  else if(limiter.Limiter_monitored_NO==1) limiter.Ram_Limiter_monitored_value = limiter.AvePower_Pp;
  else limiter.Ram_Limiter_monitored_value = limiter.AvePower_Total;		//默认..
}


//------------------------------------------------------------------------------
//  void read_Threshold_for_MDEvent( void )
//  DESCRIPTION: 上电读一次(或上电先不判模式)，接下来每分钟读一次(设置过程中都进行操作一次)
//  RETURNS: None..
//------------------------------------------------------------------------------
void read_Threshold_for_MDEvent( void )
 {
 	unsigned char Buff1[20];
 	unsigned char Buff2[20];
 	unsigned char *Ptr;
 	unsigned char *Point; 	
 	unsigned long  temp_long1;
 	unsigned short i;			
 	
 	Point=Buff1;
	Ptr = Buff2;
	limiter.Limiter_RelayMode = 0;
	//获取监视值代码
	E2P_RData( Point, Limiter_monitored_value, 9 );		// 2(ID)+6(NAME)+1(属性)
	
	for( i=0;i<Limiter_monitored_list_Sum;i++ )
	{
		if( Data_Comp( Point+2, (unsigned char*)Limiter_monitored_list[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
  if(i==0) limiter.Limiter_monitored_NO = 0;
  else if(i==1) limiter.Limiter_monitored_NO = 1;
  else limiter.Limiter_monitored_NO = 2;
  
  //判断紧急emergency模式/普通模式normal..
 	//E2P_RData( Ptr, Limiter_emergency_profile_active, 1 );  //只读..
 	limiter.Limiter_RelayMode = 0xA1;		//使能普通模式.. 0---无模式;0xa1---普通模式; 0xb2---紧急模式; 

		//emergency_profile_group_id_list	
		E2P_RData( Point, Limiter_emergency_profile_group_id_list, 12 );
	  //emergency_profile	
		E2P_RData( Ptr, Limiter_emergency_profile, 18 );			//18个字节一起读写..		
		
		for( i=0;i<*(Point+1);i++ )
		{
			if( Data_Comp( Point+2+i*2, Ptr, 2 ) == 0 ) break;
		}
		if(i!=*(Point+1))
		{
			//判断紧急模式的时间 --- 起始时间,按照闰年计算结束时间(单独存放结束时间)..
			SwapData( Ptr+14, 4 );		//高低位字节互换..
			RAM_Write( (unsigned char*)&temp_long1, Ptr+14, 4 );		//紧急持续时间4个字节
			if(JudgeTwo_DateTime( Ptr+2, temp_long1)==1) limiter.Limiter_RelayMode = 0xB2;	//比较时间			
			
		}			
	 
	//取值(激活限值)..
	if(limiter.Limiter_RelayMode == 0xB2)		//紧急模式..
		{
				E2P_RData((unsigned char*)&limiter.Ram_Auto_Reclosing_relay_Num_emergency, Auto_Reclosing_relay_Num_emergency, 1 );		//需量超限后自动重合闸次数(紧急)(2)		Data::Unsigned8
				limiter.Ram_Reconnection_timeout_emergency = 0;
				E2P_RData((unsigned char*)&limiter.Ram_Reconnection_timeout_emergency, Reconnection_timeout_emergency, 1 );					//需量超限后到重合闸延时时间(1)			Register::Unsigned8 --分钟..
				limiter.Ram_Reconnection_timeout_emergency = limiter.Ram_Reconnection_timeout_emergency*60;
			
				E2P_RData( Ptr, Limiter_threshold_emergency, 5 );
		}else{
				//除了71类中的属性,其余存储均为低位在前..
//				E2P_RData((unsigned char*)&limiter.Ram_Auto_Reclosing_relay_Num_emergency, Auto_Reclosing_relay_Num_emergency, 1 );		//需量超限后自动重合闸次数(紧急)(2)		Data::Unsigned8
//				limiter.Ram_Reconnection_timeout_emergency = 0;
//				E2P_RData((unsigned char*)&limiter.Ram_Reconnection_timeout_emergency, Reconnection_timeout_emergency, 1 );					//需量超限后到重合闸延时时间(1)			Register::Unsigned16 --分钟..
//				limiter.Ram_Reconnection_timeout_emergency = limiter.Ram_Reconnection_timeout_emergency*60;
			
				E2P_RData((unsigned char*)&limiter.Ram_Auto_Reclosing_relay_Num_Normal, Auto_Reclosing_relay_Num, 1 );	//需量超限后自动重合闸次数(2)			Data::Unsigned8
				limiter.Ram_Reconnection_timeout_Normal = 0;
				E2P_RData((unsigned char*)&limiter.Ram_Reconnection_timeout_Normal, Reconnection_timeout, 1 );					//需量超限后到重合闸延时时间(1)			Register::Unsigned16 --分钟..
			  limiter.Ram_Reconnection_timeout_Normal = limiter.Ram_Reconnection_timeout_Normal*60;
			  
				E2P_RData( Ptr, Limiter_threshold_normal, 5 );
		}
	SwapData( Ptr+1, 4 );		//高低位字节互换..
	RAM_Write( (unsigned char*)&limiter.Ram_Limiter_threshold_active, Ptr+1, 4 );
	
//	//按照内部的功率单位进行换算..直通式是:KW,互感式是:W
//	#if ( METERTYPE == U230_I10A__100A300)		//直通式..
//				limiter.Ram_Limiter_threshold_active = limiter.Ram_Limiter_threshold_active*10000;  //电表里功率是:0.1W
//	#else		//互感式..
//				limiter.Ram_Limiter_threshold_active = limiter.Ram_Limiter_threshold_active*10;  //电表里功率是:0.1W
//	#endif		
			//20140930后全部改为以W为单位，显示需要改为KW为单位..										
		limiter.Ram_Limiter_threshold_active = limiter.Ram_Limiter_threshold_active*10;  //电表里功率是:0.1W
	
	E2P_RData( Point, Limiter_threshold_active, 4 );		//读取存储激活值进行比较..
	if(Data_Comp( Point, Ptr+1, 4 ) != 0 ){		//限制产生变化..
				E2P_WData( Limiter_threshold_active,Ptr+1, 4 );
				//记录域值变化..
			 SM.DisconnectorControlCode = Limiter_threshold_changed;
			 LoadRecord( Disconnector_Control_No );
			 SM.Event_all_flag |= Disconnector_Control_BIT;
			 SM.Error_all_flag |= Disconnector_Control_BIT;
		}
	
		//获取判断持续时间..
	  E2P_RData( Ptr, Limiter_min_over_threshold_duration, 4 );
		E2P_RData( Ptr,Limiter_min_over_threshold_duration, 4 );
		SwapData( Ptr, 4 );		//高低位字节互换..
		RAM_Write( (unsigned char*)&limiter.Ram_Limiter_min_over_threshold_duration, Ptr, 4 );		
		
		E2P_RData( Ptr, Limiter_min_under_threshold_duration, 4 );
		SwapData( Ptr, 4 );		//高低位字节互换..
		RAM_Write( (unsigned char*)&limiter.Ram_Limiter_min_under_threshold_duration, Ptr, 4 );
			
 }
 

/**********************************************************************/
// Change YYMMDD to GmDay (高位在前)
/**********************************************************************/

unsigned long HexGetGmDay( unsigned char* Addr )
{
  unsigned short Day,Mon,Year;

	Mon = *(Addr+1);
	Year= *Addr;
	Day = *(Addr+2) + DayTab[Mon] + Year*365 + (Year>>2) + 1;
	if( (Year&0x03)==0 && Mon<=2 ) Day--;
  return Day;
}

/**********************************************************************
	Change YYMMDDxxhhmmss to GmSec   (xx表示:周--高位在前)
**********************************************************************/

unsigned long HexGetGmSec( unsigned char* Addr )
{
	return (unsigned long)(*(Addr+6)) 
		+ (unsigned long)(*(Addr+5))*60
		+ (unsigned long)(*(Addr+4))*60*60
		+ (unsigned long)(HexGetGmDay(Addr))*60*60*24;
}

/**********************************************************************
	Change YYMMDDxxhhmm to GmMin   (xx表示:周--高位在前)
**********************************************************************/

unsigned long HexGetGmMin( unsigned char* Addr )
{
	return (unsigned long)(*(Addr+5))
		+ (unsigned long)(*(Addr+4))*60
		+ (unsigned long)(HexGetGmDay(Addr))*60*24;
}

/**********************************************************************
	judge YYYYMMDDxxhhmmss 跟当前比较的大小    (xx表示:周--高位在前)
	Source: 判断日期及时间; Clk_Dest:当前电表的日期及时间;
	ADD_source:判断日期及时间需要需要增加的数值;  //不判秒按照分钟来计
	返回:1--表示当前在这段时间以内;0--表示在此之外,或者日期非法;
**********************************************************************/
unsigned short JudgeTwo_DateTime( unsigned char* Source,unsigned long ADD_source)
{
	unsigned long Total_Min1,Total_Min2;	
	//unsigned short add_second,Temp;
	
	//先比较高位YY年份..
	if((*Source!=Clk.YearH)&&(((*Source)+1)!=Clk.YearH))	 return 0;
		
//	if( ADD_source == 0xFFFFFFFF ) return 0;			//未初始化..

//	Total_Min1 = HexGetGmMin( Source+1 );
//	Total_Min2 = HexGetGmMin( &Clk.YearL );

	Total_Min1 = HexGetGmSec( Source+1 );
	Total_Min2 = HexGetGmSec( &Clk.YearL );

	//增加高位年份判断
	//if(((*Source)+1)==Clk.YearH) Total_Min2 +=(unsigned long)256*365*24*60;		//最高位256年????天数判断..
	
	if( Total_Min1 > Total_Min2 ) return 0;
	//add_second = (unsigned short)	(ADD_source%60); //不做秒判断..
	//Total_Min1 += (ADD_source/60);
	Total_Min1 += ADD_source;		//
	if(Total_Min1>Total_Min2) return 1;
	else return 0;		
}
///当前时间>时间，返回1，时间相等返回0，当前时间<时间,返回2
unsigned short ComparTwo_DateTime( unsigned char* Source)
{
	unsigned char Cur_time[12];
	unsigned char* ptr;
	unsigned char i=0;
	ptr = Cur_time;
	RAM_Write( ptr, &Clk.YearH, 12 );	  //比对日期yyyymmddWW+时间hh:mm	
	//----------比较年月日 时分秒，跳过周---------
	for(i=0;i<8;i++,i!=4)
		{
			if(*(Source+i)!=0xFF)
			{	
			///	if(Data_CompHL(ptr+i,Source+i,8)==0)	return 0;
				if(Data_CompHL(ptr+i,Source+i,1)==1)	return 1;
				if(Data_CompHL(ptr+i,Source+i,1)==-1) return 2;
			}
		}
	return 0;
}

