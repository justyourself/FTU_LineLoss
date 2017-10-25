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
#include "ht6xxx_lib.h"

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
unsigned short GetActivity_calendar_AddrOffset( unsigned char Attribute ) 
{
	unsigned char Buff[2];
	unsigned char* Ptr;
	unsigned short AddrOffset;
	
	Ptr = Buff;
	*Ptr = 1;
	
	E2P_RData( Ptr, EActivityCalendarNo, 1 );								//0:当前时段参数指向ECalendar_name_active	
	#if(ACTIVE_FEE ==YesCheck)				//复制备用套到激活套EEPROM中..
		 if( *Ptr != 0 ){
		 	 *Ptr = 0;
		 }										
        #endif	
		
	if( *Ptr != 0 )															//1:当前时段参数指向ECalendar_name_passive	
	{
		if(( Attribute >= 2 )&&(  Attribute <= 5 ))
		{
					 AddrOffset = ECalendar_name_passive;
           AddrOffset -= ECalendar_name_active; 
		}
		else AddrOffset = 0;	
	}	 
	else 
	{
		if(( Attribute >= 2 )&&(  Attribute <= 5 )) AddrOffset = 0;	
		else
		{
					AddrOffset = ECalendar_name_passive;
          AddrOffset -= ECalendar_name_active; 
		}
	}		

	return AddrOffset;
}	

//取Activity_calendar类数据
short GetActivity_calendarClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[6];
	unsigned char Buff2[16];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned short Len=2;
	unsigned char Attribute;
	unsigned short AddrOffset;
	unsigned short Addr;
	unsigned short i,j;
	unsigned short Cunt1;
	unsigned long* BlockNumber;
		
	Ptr = Buff2;
	BlockNumber = CommPara[COM_No].BlockNumber;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0D;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Buff, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;								//异常标志
		*(Dest+1) = object_undefined; 								//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;											//正常数据返回	
		Attribute = *(Source+2+6);									//请求的属性

	//	AddrOffset = GetActivity_calendar_AddrOffset( Attribute );
		
	if(Attribute>5)
		{
		AddrOffset = ECalendar_name_passive;
		AddrOffset -= ECalendar_name_active;
		}
	else
		AddrOffset = 0;
	
		switch( Attribute )
		{
			case 1:													//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Buff, 6 );
				Len = 9;
				break;	
			case 2:													//calendar_name_active (static) 		octet-string
			case 6:													//calendar_name_passive (static) 		octet-string
	#if(ACTIVE_FEE ==YesCheck)				//复制备用套到激活套EEPROM中..
				if(Attribute==2) Addr = ECalendar_name_activeNEW;
				else  Addr = ECalendar_name_passiveNEW;
                                Cunt1 =0 ;
				E2P_RData( (unsigned char *)&Cunt1, Addr, 1 );
		    if(Cunt1>12) Cunt1 =12;
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = Cunt1;  	
		    E2P_RData( Dest+3, Addr+2, Cunt1 );
				Len = 3+Cunt1;
				break;
	#else
				*(Dest+1) = D_OctetStr;
	            Addr = AddrOffset + ECalendar_name_active;
				E2P_RData( Dest+2, Addr, 1 );
				Buff[0] = *(Dest+2);
				if(Buff[0]>4) 
					{
					*Dest = Get_data_access_result; 							//异常标志
					*(Dest+1) = object_undefined;								//无对应的OBIS码
					Len = 2;	
					}
				E2P_RData( Dest+3, Addr+2, *(Dest+2) );
				Len = 5;
				break;
  #endif  	
			case 3:													//season_profile_active (static) 		array		
			case 7:													//season_profile_passive (static) 		array		
				*(Dest+1) = Data_array;
				Addr = AddrOffset + ESeasonNumber_active;
				E2P_RData( Dest+2, Addr, 1 );
				if( *(Dest+2) > 6 ) *(Dest+2) = 6;					//最多6个时区
				Cunt1 = *(Dest+2);
				Len = 0;
				Point = Dest+3;
				for( i=0;i<Cunt1;i++ )
				{
					Addr = AddrOffset + Season_profile1_active;
					Addr += i*15;
					E2P_RData( Ptr, Addr, 14 );
					*Point = Data_structure;
					*(Point+1) = 0x03;
					*(Point+2) = D_OctetStr;						//season_profile_name: 	octet-string
					*(Point+3) = 0x01;                          	
					*(Point+4) = *Ptr;                          	
					*(Point+5) = D_OctetStr;						//season_start: 		octet-string
					*(Point+6) = 0x0C;                          	
					RAM_Write( Point+7, Ptr+1, 12 );            	
					*(Point+19) = D_OctetStr;						//week_name: 			octet-string
					*(Point+20) = 0x01;
					*(Point+21) = *(Ptr+13);
					Point += 22;
					Len += 22;					
				}
				Len += 3;
				break;
			case 4:													//week_profile_table_active (static) 	array
			case 8:													//week_profile_table_passive (static) 	array
				*(Dest+1) = Data_array;
				Addr = AddrOffset + EWeekNumber_active;
				E2P_RData( Dest+2, Addr, 1 );
				if( *(Dest+2) > 8 ) *(Dest+2) = 8;					//最多8个周历表
				Cunt1 = *(Dest+2);
				Len = 0;
				Point = Dest+3;
				for( i=0;i<Cunt1;i++ )
				{
					Addr = AddrOffset + Week_profile1_active;
					Addr += i*9;
					E2P_RData( Ptr, Addr, 8 );
					*Point = Data_structure;
					*(Point+1) = 0x08;
					*(Point+2) = D_OctetStr;						//week_profile_name: 	octet-string
					*(Point+3) = 0x01;
					*(Point+4) = *Ptr;
					Point += 5;
					for( j=0;j<7;j++ )
					{
						*Point = D_Unsigned8;						//monday ~ sunday: 		day_id          
//						*(Point+1) = 0x01;                            
//						*(Point+2) = *(Ptr+j+1);                           
//						Point += 3;
						*(Point+1) = *(Ptr+j+1);                           
						Point += 2;
					}	
//					Len += 26;					
					Len += 19;					
				}
				Len += 3;
				break;
			case 5:													//day_profile_table_active (static) 	array
			case 9:													//day_profile_table_passive (static) 	array
				Addr = AddrOffset + EDay_ProfileNumber_active;
				E2P_RData( Dest+2, Addr, 1 );
				if( *(Dest+2) > 8 ) *(Dest+2) = 0;					//最多8个日历表
				Cunt1 = *(Dest+2); 
				if( *(Dest+2) == 0 )								//0个日历表
				{
					*(Dest+1) = Data_array;							//array day_profile
					Len = 3;
					*BlockNumber = 0;
				}
				else if( *(Dest+2) == 1 )							//1个日历表
				{
					*(Dest+1) = Data_array;							//array day_profile
					Len = 3;
					Get_sigle_day_profile( Dest+3, AddrOffset, 0, &Len );
					*BlockNumber = 0;
				}
				else
				{
					*Dest = 0;										//last-block = 0表示不是最后一帧数据，后续还有数据块。
					*BlockNumber = 1;
					RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
					SwapData( Ptr, 4 );
					RAM_Write( Dest+1, Ptr, 4 );					//block-number 块数，=1为传输的第一块。
					*(Dest+5) = 0x00;								//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
					*(Dest+6) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//					*(Dest+7) = 0x01;
//					*(Dest+8) = 0xBF;								//数据长度为0x1BF=447字节 
					*(Dest+9) = Data_array;							//Data数据类型1,指其后为SEQUENCE OF Data数组
					*(Dest+10) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//					*(Dest+11) = 0x00;								//数据长度为0x1BF=447字节 
//					*(Dest+12) = 0x80;								//共128个数组，对应于128个object_list_element
					FillIntegerToBuff( Dest+11, Cunt1 );
//					Len = 13;
					Len = 4;
					Get_sigle_day_profile( Dest+13, AddrOffset, 0, &Len );
					FillIntegerToBuff( Dest+7, Len );
					Len += 9;
				}
						
//				*(Dest+1) = Data_array;								//array day_profile
//				Addr = AddrOffset + EDay_ProfileNumber_active;
//				E2P_RData( Dest+2, Addr, 1 );
//				if( *(Dest+2) > 8 ) *(Dest+2) = 8;					//最多8个日历表
//				Cunt1 = *(Dest+2);									//日时段表数
//				Len = 0;
//				Point = Dest+3;
//				for( i=0;i<Cunt1;i++ )
//				{
//					Get_sigle_day_profile( Point, AddrOffset, i, &Len );
//					if( Len >= (MAX_PDU_SIZE-4) ) break;			//数据帧超长
//					*Point = Data_structure;						//day_profile ::= structure
//					*(Point+1) = 0x02;
//					Addr = AddrOffset + Day_id1_active;
//					Addr += i*2;
//					E2P_RData( Ptr, Addr, 1 );
//					*(Point+2) = D_Unsigned8;						//day_id          unsigned 
//					*(Point+3) = *Ptr;                            										
//					Addr = AddrOffset + EDay_Schedule1_Number_active;
//					Addr += i*2;
//					E2P_RData( Ptr, Addr, 1 );
//					if( *Ptr > 12 ) *Ptr = 12;
//					*(Point+4) = Data_array;						//day_schedule: array day_profile_action
//					*(Point+5) = *Ptr;
//					Cunt2 = *Ptr;									//日时段数
//					Point += 6;
//					Len += 6;
//					if( Len >= (MAX_PDU_SIZE-20) ) break;			//数据帧超长
//					Addr = AddrOffset + day_profile_action1_schedule1_active+i*12*13;
//					Get_sigle_day_profile_schedule( Point, Addr, Cunt2, &Len );
//					for( j=0;j<Cunt2;j++ )
//					{
//						*Point = Data_structure;					//day_profile_action ::= structure
//						*(Point+1) = 0x03;
////						Addr = AddrOffset + day_profile_action1_schedule1_active;
////						Addr += j*13;
//						E2P_RData( Ptr, Addr, 12 );
//						*(Point+2) = D_OctetStr;					//start_time: octet-string
//						*(Point+3) = 0x04;                      	
//						RAM_Write( Point+4, Ptr, 4 );           	
//						*(Point+8) = D_OctetStr;					//script_logical_name: octet-string
//						*(Point+9) = 0x06;                      	
//						RAM_Write( Point+10, Ptr+4, 6 );        	
//						*(Point+16) = D_Unsigned16;					//script_selector: long-unsigned
//						RAM_Write( Point+17, Ptr+10, 2 );												
//						Addr += 13;
//						Point += 19;
//						Len += 19;
//					}			
//				}
				//Len += 3;
				break;
			case 10:
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x0C;
				E2P_RData( Dest+3, EActivate_passive_calendar_time, 12 );
				Len = 15;
				break;
			default: break;			
		}
	}		
	return Len;
}	

short GetNextActivity_calendarClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	//unsigned long* BlockNumber;
  	unsigned char* Logical_name;
	unsigned char* SubsBlockPara;
  	unsigned char Attribute;
	unsigned short Addr;
	unsigned short AddrOffset;
	unsigned short Length;
	unsigned short Cunt1;
	unsigned short Cunt2;
	
	//BlockNumber = CommPara[COM_No].BlockNumber;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	Logical_name = SubsBlockPara+MAC_Get_Set_Logical_name;
	Attribute = *(SubsBlockPara+MAC_Get_Set_Attribute);
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0D;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	Ptr = Buff;
	
	if( Data_Comp( Buff, Logical_name, 6 ) != 0 ) 
	{
		Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
	}
	switch( Attribute )
	{
		case 5:																//day_profile_table_active
		case 9:																//day_profile_table_passive
			//AddrOffset = GetActivity_calendar_AddrOffset( Attribute );
			
			if(Attribute>5)
				{
				AddrOffset = ECalendar_name_passive;
				AddrOffset -= ECalendar_name_active;
				}
			else
				AddrOffset = 0;
			Addr = AddrOffset + EDay_ProfileNumber_active;
			E2P_RData( Ptr, Addr, 1 );
			if( *Ptr > 8 )
			{
				Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );	//最多8个日历表
				break;
			}	
			Cunt1 = *Ptr;
			//Cunt2 =(*BlockNumber); 
			Cunt2 = HComm.BIGBlockNumber[COM_No]; 
			CreatSubObjectListHead( COM_No, SendBuff );
			//HComm.BIGBlockNumber[COM_No]++;  
			Length = 0;
			Get_sigle_day_profile( SendBuff+9, AddrOffset, Cunt2, &Length );
			FillIntegerToBuff( SendBuff+7, Length );
			Length += 9;
//			if( Cunt1 == (Cunt2+1) ) *SendBuff = 0x01;						//last-block=1表示是最后一帧数据，后续没有数据块。
	    if( Cunt1 == (Cunt2+1) )
			//if( Temp == 0 )
			{
				 *SendBuff = 0x01;								//last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
			break;
		default:
			Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
			break;		
	}	

	return Length;
}		
	


short Get_sigle_day_profile_schedule( unsigned char* Dest, unsigned short Addr, unsigned short Items, unsigned short* Length )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short i;
	unsigned char* Point;
	
	Ptr = Buff;
	Point = Dest;
	for( i=0;i<Items;i++ )
	{
		*Point = Data_structure;		
		*(Point+1) = 0x03;
		E2P_RData( Ptr, Addr, 12 );
		*(Point+2) = D_OctetStr;					//start_time: octet-string
		*(Point+3) = 0x04;                      	
		RAM_Write( Point+4, Ptr, 4 );           	
		*(Point+8) = D_OctetStr;					//script_logical_name: octet-string
		*(Point+9) = 0x06;                      	
		RAM_Write( Point+10, Ptr+4, 6 );        	
		*(Point+16) = D_Unsigned16;					//script_selector: long-unsigned
		RAM_Write( Point+17, Ptr+10, 2 );												
		Addr += 13;
		*Length += 19;
		Point += 19;
	}	
    return 0;
}

short Get_sigle_day_profile( unsigned char* Dest, unsigned short AddrOffset, unsigned short ItemNo, unsigned short* Len )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short Addr;
	unsigned char* Point;
	unsigned short Cunt;
	
	Ptr = Buff;
	Point = Dest;

	*Point = Data_structure;						//day_profile ::= structure
	*(Point+1) = 0x02;
	Addr = AddrOffset + Day_id1_active;
	Addr += ItemNo*2;
	E2P_RData( Ptr, Addr, 1 );
	*(Point+2) = D_Unsigned8;						//day_id          unsigned 
	*(Point+3) = *Ptr;                            										
	Addr = AddrOffset + EDay_Schedule1_Number_active;
	Addr += ItemNo*2;
	E2P_RData( Ptr, Addr, 1 );
	if( *Ptr > 12 ) *Ptr = 12;
	*(Point+4) = Data_array;						//day_schedule: array day_profile_action
	*(Point+5) = *Ptr;
	Cunt = *Ptr;									//日时段数
	Point += 6;
	*Len += 6;
//	if( Len >= (MAX_PDU_SIZE-20) ) break;			//数据帧超长
	Addr = AddrOffset + day_profile_action1_schedule1_active+ItemNo*12*13;
	Get_sigle_day_profile_schedule( Point, Addr, Cunt, Len );

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

short Judge_calendar_name( unsigned char* Data )
{
	if( *Data != D_OctetStr ) 
	{
		return type_unmatched;
	}			
//	if( *(Data+1) > 2 ) 
	if( *(Data+1) > 12 ) 
	{
		return other_reason;
	}
	return 0;	
}	

short Judge_season_profile( unsigned char* Data )
{
	short i;
	unsigned short Cunt1;
	unsigned char* Ptr;
	
//	if(( *Data != Data_array )||( *(Data+1) > 4 )) 
//	{
//		return other_reason;
//	}
//	Cunt1 = *(Data+1);
//	Ptr = Data+2;

	if( *Data != Data_array )
	{
		return other_reason;
	}
	if(( *(Data+1) & 0x80 ) != 0 )
	{
		if( *(Data+1) == 0x81 )										//设置数据长度是否为1字节 
		{
			Cunt1 = *(Data+2);										//有效设置数据长度
			Ptr = Data+3;											//有效设置数据起始地址			
		}			
		else if( *(Data+1) != 0x82 ) return other_reason;
		else 
		{
			Cunt1 = GetBuffToInteger( Data+2 );
			Ptr = Data+4;
		}	
	}	
	else
	{
		Cunt1 = *(Data+1);
		Ptr = Data+2;
	}	
	if( Cunt1 > 6 ) return other_reason;
	
	for( i=0;i<Cunt1;i++ )
	{
		if((*Ptr!=Data_structure)||(*(Ptr+1)!= 3)					//season ::= structure
			||(*(Ptr+2)!=D_OctetStr)||(*(Ptr+3)!=1)					//season_profile_name: 	octet-string
			||(*(Ptr+5)!=D_OctetStr)||(*(Ptr+6)!=0x0C)				//season_start: 		octet-string
			||(*(Ptr+19)!=D_OctetStr)||(*(Ptr+20)!=1))				//week_name: 			octet-string
		{
			return other_reason;
		}		
		Ptr += 22;	
	}
	
	return 0;	
}	

short Judge_week_profile_table( unsigned char* Data )
{
	short i,j;
	unsigned short Cunt1;
	unsigned char* Ptr;
	
//	if(( *Data != Data_array )||( *(Data+1) > 4 )) 
//	{
//		return other_reason;
//	}
//	Cunt1 = *(Data+1);
//	Ptr = Data+2;

	if( *Data != Data_array )
	{
		return other_reason;
	}
	if(( *(Data+1) & 0x80 ) != 0 )
	{
		if( *(Data+1) == 0x81 )										//设置数据长度是否为1字节 
		{
			Cunt1 = *(Data+2);										//有效设置数据长度
			Ptr = Data+3;											//有效设置数据起始地址			
		}			
		else if( *(Data+1) != 0x82 ) return other_reason;
		else 
		{
			Cunt1 = GetBuffToInteger( Data+2 );
			Ptr = Data+4;
		}	
	}	
	else
	{
		Cunt1 = *(Data+1);
		Ptr = Data+2;
	}	
	if( Cunt1 > 8 ) return other_reason;

	for( i=0;i<Cunt1;i++ )
	{
		if((*Ptr!=Data_structure)||(*(Ptr+1)!=8)					//week_profile ::= structure
			||(*(Ptr+2)!=D_OctetStr)||(*(Ptr+3)!=1))				//week_profile_name: 	octet-string
		{
			return other_reason;
		}		
		Ptr += 5;
		for( j=0;j<7;j++ )
		{
//			if((*Ptr!=D_Unsigned8)||(*(Ptr+1)!=1))					//monday ~ sunday: 		day_id
			if(*Ptr!=D_Unsigned8)					//monday ~ sunday: 		day_id
			{
				return other_reason;
			}
//			Ptr += 3; 		
			Ptr += 2; 		
		}
	}
	
	return 0;	
}	


short Judge_day_profile_table( unsigned char* Data )
{
	short i,j;
	unsigned short Cunt1,Cunt2;
	unsigned char* Ptr;
	
//	if(( *Data != Data_array )||( *(Data+1) > 8 )) 					//array day_profile
//	{
//		return other_reason;
//	}
//	Cunt1 = *(Data+1);
//	Ptr = Data+2;
	
	if( *Data != Data_array )
	{
		return other_reason;
	}
	if(( *(Data+1) & 0x80 ) != 0 )
	{
		if( *(Data+1) == 0x81 )										//设置数据长度是否为1字节 
		{
			Cunt1 = *(Data+2);										//有效设置数据长度
			Ptr = Data+3;											//有效设置数据起始地址			
		}			
		else if( *(Data+1) != 0x82 ) return other_reason;
		else 
		{
			Cunt1 = GetBuffToInteger( Data+2 );
			Ptr = Data+4;
		}	
	}	
	else
	{
		Cunt1 = *(Data+1);
		Ptr = Data+2;
	}	
	if( Cunt1 > 8 ) return other_reason;
	
	for( i=0;i<Cunt1;i++ )
	{
		if((*Ptr!=Data_structure)||(*(Ptr+1)!=2)					//day_profile ::= structure
			||(*(Ptr+2)!=D_Unsigned8)								//day_id          unsigned
			||(*(Ptr+4)!=Data_array)||(*(Ptr+5)>12))			    //day_profile ::= structure
		{
			return other_reason;
		}		
		Cunt2 = *(Ptr+5);
		Ptr += 6;
		for( j=0;j<Cunt2;j++ )
		{
			if((*Ptr!=Data_structure)||(*(Ptr+1)!=3)				//day_profile_action ::= structure
				||(*(Ptr+2)!=D_OctetStr)||(*(Ptr+3)!=0x04)			//start_time: octet-string
				||(*(Ptr+8)!=D_OctetStr)||(*(Ptr+9)!=0x06)			//script_logical_name: octet-string
				||(*(Ptr+16)!=D_Unsigned16))						//script_selector: long-unsigned
			{
				return other_reason;
			}		
			if((*(Ptr+4)==0xff)||(*(Ptr+5)==0xff)||(*(Ptr+6)==0xff)||(*(Ptr+7)==0xff)) return other_reason;  //start_time: 不能使用通配符
			Ptr += 19;
		}
	}
	
	return 0;	
}	

unsigned char SetActivity_calendarClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char Buff2[16];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char ErrorByte=0;
//	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
//	unsigned char* DataType;
	unsigned char AccessMode;
	unsigned short AddrOffset;
	unsigned short Addr;
	unsigned short i,j;
	unsigned short Cunt1,Cunt2;
		
	Ptr = Buff2;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
//	DataType = WriteBufAds;
	if( *Attribute == 1 ) AccessMode = read_only;
	//if(( *Attribute >= 1 )&&(*Attribute <= 5)) AccessMode = read_only;
	else AccessMode = read_and_write;
	
	Buff[0] = 0x00;				//时段表..
	Buff[1] = 0x00;
	Buff[2] = 0x0D;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Buff, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else if(( AccessMode != write_only )&&( AccessMode != read_and_write ))
	{
		ErrorByte = read_write_denied; 									//无写入权限
	}	 
//	else if( *DataType != IECHDLCSetupClassAttribute_ParaTab[(*Attribute)-1].DataType )
//	{
//		ErrorByte = type_unmatched; 									//无写入权限
//	}	 
	else
	{	
		//AddrOffset = GetActivity_calendar_AddrOffset( *Attribute );
		if(*Attribute>5)
			{
			AddrOffset = ECalendar_name_passive;
            AddrOffset -= ECalendar_name_active;
			}
		else
			AddrOffset = 0;
		switch( *Attribute )
		{
			case 1:														//logical_name
				ErrorByte = read_write_denied; 							//无写入权限
				break;	                                                
			case 2:														//calendar_name_active (static) 		octet-string
			case 6: 
			//	ErrorByte = read_write_denied; 							//无写入权限
			    ErrorByte = Judge_calendar_name( WriteBufAds );
				if( ErrorByte == 0 )                                	
					{                                                   	
					Addr = AddrOffset + ECalendar_name_active;      	
//					E2P_WData( Addr, WriteBufAds+2, 2 );  
					//固定2个字节修改成1---12个字节可变..
				 	E2P_WData( Addr, WriteBufAds+1, 1 );				   //替换激活ECalendar_name_active..
	 	            E2P_WData( Addr+2, WriteBufAds+2, *(WriteBufAds+1) );				//替换激活ECalendar_name_active.. 
					}	    
				break;	
				/*
			case 6:														//calendar_name_passive (static) 		octet-string
				ErrorByte = Judge_calendar_name( WriteBufAds );     	
				if( ErrorByte == 0 )                                	
				{                                                   	
//					Addr = AddrOffset + ECalendar_name_active;      	
//					E2P_WData( Addr, WriteBufAds+2, 2 );  
					//固定2个字节修改成1---12个字节可变..
				 	E2P_WData( ECalendar_name_passiveNEW, WriteBufAds+1, 1 );				   //替换激活ECalendar_name_active..
	 	            E2P_WData( ECalendar_name_passiveNEW+2, WriteBufAds+2, *(WriteBufAds+1) );				//替换激活ECalendar_name_active.. 
				}	                                                	
				break;	 
				*/
			case 3:														//season_profile_active (static) 		array		
			case 7:														//season_profile_passive (static) 		array		
				ErrorByte = Judge_season_profile( WriteBufAds );
				if( ErrorByte == 0 )
				{
					Addr = AddrOffset + ESeasonNumber_active;
					Cunt1 = *(WriteBufAds+1);
					if( Cunt1 > 6 ) return other_reason;
					E2P_WData( Addr, WriteBufAds+1, 1 );				//时区数
					Point = WriteBufAds+2;
					Ptr = Buff2; 
					for( i=0;i<Cunt1;i++ )
					{
					HT_FreeDog();
						Addr = AddrOffset + Season_profile1_active;
						Addr += i*15;
						*Ptr = *(Point+4);								//season_profile_name: 	octet-string		
						RAM_Write( Ptr+1, Point+7, 12 );				//season_start: 		octet-string
						*(Ptr+13) = *(Point+21);						//week_name: 			octet-string
						E2P_WData( Addr, Ptr, 14 );
						Point += 22;
					}
					/*
					if(Cunt1!=0)
						{
				  	///	SM.StandardEventCode = Passive_TOU_programmed;
							LoadRecord( Standard_Event_No );
							SM.Event_all_flag |= Standard_Event_BIT;
							SM.Error_all_flag |= Standard_Event_BIT;
						}	
					*/
				}	
				break;
			case 4:														//week_profile_table_active (static) 	array
			case 8:														//week_profile_table_passive (static) 	array
				ErrorByte = Judge_week_profile_table( WriteBufAds );
				if( ErrorByte == 0 )
				{
					Addr = AddrOffset + EWeekNumber_active;
					Cunt1 = *(WriteBufAds+1);
					if( Cunt1 > 8 ) return other_reason;					
					E2P_WData( Addr, WriteBufAds+1, 1 );				//周历表数
					// Cunt1 = *(WriteBufAds+1);
					Point = WriteBufAds+2;
					Ptr = Buff2; 
					for( i=0;i<Cunt1;i++ )
					{
					HT_FreeDog();
						Addr = AddrOffset + Week_profile1_active;
						Addr += i*9;	
						*Ptr = *(Point+4);								//week_profile_name: 	octet-string		
						Point += 5;
						for( j=0;j<7;j++ )
						{
//							*(Ptr+j+1) = *(Point+2);					//monday ~ sunday: 		day_id
							*(Ptr+j+1) = *(Point+1);					//monday ~ sunday: 		day_id
//							Point += 3;							
							Point += 2;							
						}																	
						E2P_WData( Addr, Ptr, 8 );
					}
					/*
					 if(Cunt1!=0)
						{
				  		///SM.StandardEventCode = Passive_TOU_programmed;
							LoadRecord( Standard_Event_No );
							SM.Event_all_flag |= Standard_Event_BIT;
							SM.Error_all_flag |= Standard_Event_BIT;
						}	
					 */
				}	
				break;
			case 5:														//day_profile_table_active (static) 	array
			case 9:														//day_profile_table_passive (static) 	array
				ErrorByte = Judge_day_profile_table( WriteBufAds );
				if( ErrorByte == 0 )
				{
					//sky20130711..
					if(( *(WriteBufAds+1) & 0x80 ) != 0 )
					{
						if( *(WriteBufAds+1) == 0x81 )										//设置数据长度是否为1字节 
						{
							Cunt1 = *(WriteBufAds+2);										//有效设置数据长度
							Point = WriteBufAds+3;											//有效设置数据起始地址			
						}			
						else if( *(WriteBufAds+1) != 0x82 ) return other_reason;
						else 
						{	
							Cunt1 = GetBuffToInteger( WriteBufAds+2 );
							Point = WriteBufAds+4;
						}	
					}	
					else
					{
						Cunt1 = *(WriteBufAds+1);
						Point = WriteBufAds+2;
					}	
					Addr = AddrOffset + EDay_ProfileNumber_active;
					if( Cunt1 > 8 ) return other_reason;					
												
					E2P_WData( Addr, (unsigned char *)&Cunt1, 1 );	////日时段表数
										
//					Addr = AddrOffset + EDay_ProfileNumber_active;
//					Cunt1 = *(WriteBufAds+1);
//					if( Cunt1 > 8 ) return other_reason;					
					//E2P_WData( Addr, WriteBufAds+1, 1 );				//日时段表数
					//Cunt1 = *(WriteBufAds+1);
					//Point = WriteBufAds+2;
					Ptr = Buff2; 
					for( i=0;i<Cunt1;i++ )
					{
					HT_FreeDog();
						Addr = AddrOffset + Day_id1_active;
						Addr += i*2;	
						*Ptr = *(Point+3);								
						E2P_WData( Addr, Ptr, 1 );						//day_id          unsigned
						
						Addr = AddrOffset + EDay_Schedule1_Number_active;
						Addr += i*2;	
						*Ptr = *(Point+5);										
						E2P_WData( Addr, Ptr, 1 );						//日时段数
						
						Cunt2 = *Ptr;
						if(Cunt2>12) ErrorByte = other_reason;	  //日时段数限制..
						Point += 6;
						Addr = AddrOffset + day_profile_action1_schedule1_active+i*12*13;
						for( j=0;j<Cunt2;j++ )
						{
//							Addr = AddrOffset + day_profile_action1_schedule1_active;
//							Addr += j*13;
							RAM_Write( Ptr, Point+4, 4 );				//start_time: octet-string
							RAM_Write( Ptr+4, Point+10, 6 );			//script_logical_name: octet-string	
							RAM_Write( Ptr+10, Point+17, 2 );			//script_selector: long-unsigned
							E2P_WData( Addr, Ptr, 12 );
							Addr += 13;
							Point += 19;							
						}																	
					}
					HT_FreeDog();
					/*
					if(Cunt1!=0)
						{
//				  		SM.StandardEventCode = Passive_TOU_programmed;
//							LoadRecord( Standard_Event_No );
				  		///SM.FraudDetectionCode = Tariff_Parameter_Changed;
				  		SM.FraudDetectionCode = Change_Tariff_Program_by_SW;
							LoadRecord( Security_Event_No );
							SM.Event_all_flag |= Security_Event_BIT;
							SM.Error_all_flag |= Security_Event_BIT;
						}	
					*/
				}	
				break;
			case 10:													//Activate_passive_calendar_time
//				if((*WriteBufAds!=D_OctetStr)||(*(WriteBufAds+1)!=12))	
				if( Judge_Activate_passive_calendar_time( WriteBufAds ) != 0 )
				{
					ErrorByte = other_reason;	
				}	
				else 
				{
					E2P_WData( EActivate_passive_calendar_time, WriteBufAds+2, 12 );
					E2P_RData( Para.RActivate_passive_calendar_time, EActivate_passive_calendar_time, 12 );
				}	
				break;	
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}		
	return ErrorByte;
}	


//取Special_days类数据
short GetSpecial_daysClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned short Len;
	unsigned char Attribute;
	unsigned short CaptureItems;
	unsigned long* BlockNumber;
	unsigned char Temp_FLAG = 0;
	unsigned short Addr1,Addr2;	
			
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	Buff[6] = 0x00;				
	Buff[7] = 0x00;
	Buff[8] = 0x0B;
	Buff[9] = 0x00;
	Buff[10] = 0x01;
	Buff[11] = 0xff;	
	
	if( Data_Comp( Ptr, Source+2, 6 ) == 0 ) Temp_FLAG = 1;
	else if( Data_Comp( Ptr+6, Source+2, 6 ) == 0 ) Temp_FLAG = 2; 
	else Temp_FLAG = 0;
//	if( Data_Comp( Buff, Source+2, 6 ) != 0 ) 
  if( Temp_FLAG == 0 )
	{
		*Dest = Get_data_access_result;											//异常标志
		*(Dest+1) = object_undefined; 											//无对应的OBIS码
		Len = 2;		                                            			
	}	                                                            			
	else                                                            			
	{                                                               			
		*Dest = Get_data;														//正常数据返回	
		Attribute = *(Source+2+6);												//请求的属性
		switch( Attribute )                                         			
		{                                                           			
			case 1:																//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				if( Temp_FLAG == 2 ) Ptr = Ptr + 6;
				RAM_Write( Dest+3, Ptr, 6 );	
				//RAM_Write( Dest+3, Buff, 6 );
				Len = 9;
				break;	
			case 2:																//entries (static) array
				if( Temp_FLAG == 1 )
				{
					 Addr1 =Espec_day_Number;
					 Addr2 =Spec_day_entry1;
				}
				/*
				else
				{
					 Addr1 = Espec_day_Number_passive;
					 Addr2 =Spec_day_entry_passive;					
				} 
				*/
				BlockNumber = CommPara[COM_No].BlockNumber;					
				CaptureItems = 0;
				E2P_RData( (unsigned char*)&CaptureItems, Addr1, 1 );
				//if( CaptureItems > 60 ) CaptureItems = 0;						//不超出最多60个节假日
				//if( CaptureItems > 200 ) CaptureItems = 0;						//不超出最多200个节假日
				if( CaptureItems > 200 ) CaptureItems = 0;						//台湾圆表节假日数开到200个	
				if( CaptureItems <= 56 )  		//56---768//20---size=280								//每包最多发送20个节假日参数一个PDU能返回完毕，不需要使用块操作
		//		if( CaptureItems <= 8 )  			//pdu = 128							//每包最多发送20个节假日参数一个PDU能返回完毕，不需要使用块操作
				{
					*(Dest+1) = Data_array;
					*(Dest+2) = 0x82;											//长度字节为2字节
					FillIntegerToBuff( Dest+3, CaptureItems );				
					Len = 5;
					Get_sigle_spec_dayPara( Dest+5, Addr2, CaptureItems, &Len );	
					*BlockNumber = 0;
				}
				else															//捕获对象参数一个PDU不能返回完毕，需要使用块操作
				{	
					*Dest = 0;													//last-block=0表示不是最后一帧数据，后续还有数据块。
					*BlockNumber = 1;
					RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
					SwapData( Ptr, 4 );
					RAM_Write( Dest+1, Ptr, 4 );								//block-number 块数，=1为传输的第一块。
					*(Dest+5) = 0x00;											//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
					*(Dest+6) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//					*(Dest+7) = 0x01;                               			
//					*(Dest+8) = 0xBF;											//数据长度为0x1BF=447字节 
					*(Dest+9) = Data_array;										//Data数据类型1,指其后为SEQUENCE OF Data数组
					*(Dest+10) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//					*(Dest+11) = 0x00;											//数据长度为0x1BF=447字节 
//					*(Dest+12) = 0x80;											//共128个数组，对应于128个object_list_element
					FillIntegerToBuff( Dest+11, CaptureItems );
//					Len = 13;
					Len = 4;
				//	Get_sigle_spec_dayPara( Dest+13, Spec_day_entry1, 20, &Len );	
					Get_sigle_spec_dayPara( Dest+13, Addr2, 8, &Len );
					FillIntegerToBuff( Dest+7, Len );
					Len += 9;
				}	
				break;
			default: break;			
		}
	}		
	return Len;
}	

short GetNextSpecial_daysClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	//u32* BlockNumber;
  	unsigned char* Logical_name;
	unsigned char* SubsBlockPara;
  	unsigned char Attribute;
	unsigned short Addr;
	unsigned short CaptureItems;
	unsigned short Length;
	unsigned short Temp;
	unsigned short SendItem;
	unsigned short PDUItem;
	unsigned char Temp_FLAG = 0;
	//BlockNumber = CommPara[COM_No].BlockNumber;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	Logical_name = SubsBlockPara+MAC_Get_Set_Logical_name;
	Attribute = *(SubsBlockPara+MAC_Get_Set_Attribute);
	Ptr = Buff;

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	Buff[6] = 0x00;				
	Buff[7] = 0x00;
	Buff[8] = 0x0B;
	Buff[9] = 0x00;
	Buff[10] = 0x01;
	Buff[11] = 0xff;	
	
	if( Data_Comp( Ptr, Logical_name, 6 ) == 0 ) Temp_FLAG = 1;
	else if( Data_Comp( Ptr+6, Logical_name, 6 ) == 0 ) Temp_FLAG = 2; 

	if( Temp_FLAG == 0 )
///	if( Data_Comp( Buff, Logical_name, 6 ) != 0 ) 
	{
		Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
		return Length;		
	}	
	switch( Attribute )
	{
		case 2:											//entries (static) array
		if( Temp_FLAG == 1 )
			E2P_RData( Ptr, Espec_day_Number, 1 );
		/*
		else if( Temp_FLAG == 2 )
			E2P_RData( Ptr, Espec_day_Number_passive, 1 );
		*/
			CaptureItems = *Ptr;
			//if( CaptureItems > 60 ) CaptureItems = 0;						//最多60个节假日
			if( CaptureItems > 200 ) CaptureItems = 0;						//最多200个节假日
			//PDUItem = 20;													//每包最多发送20个节假日	
			PDUItem = 8;													//每包最多发送8个节假日	
		  // SendItem = PDUItem * (*BlockNumber); 				 			//已发送项数
		  SendItem = PDUItem * HComm.BIGBlockNumber[COM_No];
			if( CaptureItems >= SendItem ) CaptureItems -= SendItem;		//剩余项数
			else 
			{
				Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
				break;
			}	
			if( CaptureItems > PDUItem )
			{
				CaptureItems = PDUItem;
				Temp =1;							
			}else Temp =0;
			CreatSubObjectListHead( COM_No, SendBuff ); 
			//HComm.BIGBlockNumber[COM_No]++;    //
			Length = 0;
			if( Temp_FLAG == 1 )
				Addr = Spec_day_entry1;
			/*
			else if( Temp_FLAG == 2 )
				Addr = Spec_day_entry_passive;
			*/
			Addr += SendItem*9;
			Get_sigle_spec_dayPara( SendBuff+9, Addr, CaptureItems, &Length );	
			FillIntegerToBuff( SendBuff+7, Length );
			Length += 9;
//			if( Temp == 0 ) *SendBuff = 0x01;								//last-block=1表示是最后一帧数据，后续没有数据块。
			if( Temp == 0 )
			{
				 *SendBuff = 0x01;								//last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
			break;
		default:
			Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
			break;		
	}	
	return Length;
}		

short Get_sigle_spec_dayPara( unsigned char* Dest, unsigned short Addr, unsigned short Items, unsigned short* Len )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short i;
	unsigned char* Point;
	
	Ptr = Buff;
	Point = Dest;
	for( i=0;i<Items;i++ )
	{
		E2P_RData( Ptr, Addr, 8 );
		*Point = Data_structure;
		*(Point+1) = 0x03;
		*(Point+2) = D_Unsigned16;						//index: long-unsigned
		*(Point+3) = *Ptr;                          	
		*(Point+4) = *(Ptr+1);                          	
		*(Point+5) = D_OctetStr;						//specialday_date: octet-string
		*(Point+6) = 0x05;                          	
		RAM_Write( Point+7, Ptr+2, 5 );            	
		*(Point+12) = D_Unsigned8;						//day_id: unsigned
		*(Point+13) = *(Ptr+7);
		Addr += 9;
		Point += 14;
		*Len += 14;					
	}
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

short Judge_spec_day_entry( unsigned char* Data )
{
	short i;
	unsigned short Cunt1;
	unsigned char* Ptr;
	
//	if(( *Data != Data_array )||( *(Data+1) > 60 )) 
//	{
//		return other_reason;
//	}
//	Cunt1 = *(Data+1);
//	Ptr = Data+2;

	if( *Data != Data_array )
	{
		return other_reason;
	}
	if(( *(Data+1) & 0x80 ) != 0 )
	{
		if( *(Data+1) == 0x81 )										//设置数据长度是否为1字节 
		{
			Cunt1 = *(Data+2);										//有效设置数据长度
			Ptr = Data+3;											//有效设置数据起始地址			
		}			
		else if( *(Data+1) != 0x82 ) return other_reason;
		else 
		{	
			Cunt1 = GetBuffToInteger( Data+2 );
			Ptr = Data+4;
		}	
	}	
	else
	{
		Cunt1 = *(Data+1);
		Ptr = Data+2;
	}	
	//if( Cunt1 > 60 ) return other_reason;
		if( Cunt1 > 200 ) return other_reason;

	for( i=0;i<Cunt1;i++ )
	{
		if((*Ptr!=Data_structure)||(*(Ptr+1)!= 3)					//spec_day_entry ::= structure
			||(*(Ptr+2)!=D_Unsigned16)								//index: long-unsigned
			||((*(Ptr+5)!=D_OctetStr)||(*(Ptr+6)!=0x05))				//specialday_date: octet-stringg
			||(*(Ptr+12)!=D_Unsigned8))								//day_id: unsigned
		{
			return other_reason;
		}		
		Ptr += 14;	
	}
	return 0;	
}	

unsigned char SetSpecial_daysClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[12];
	unsigned char Buff2[16];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char ErrorByte=0;
//	unsigned short Temp;
	//unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
//	unsigned char* DataType;
	unsigned char AccessMode,Temp_FLAG=0;
	//unsigned short Addr;
	unsigned short i;
	unsigned short Cunt1;
	unsigned short Addr1,Addr2;	
		
	//Ptr = Buff2;
	
	//Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
//	DataType = WriteBufAds;
	if( *Attribute == 1 ) AccessMode = read_only;
	else AccessMode = read_and_write;
	
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	Buff[6] = 0x00;				
	Buff[7] = 0x00;
	Buff[8] = 0x0B;
	Buff[9] = 0x00;
	Buff[10] = 0x01;
	Buff[11] = 0xff;	
	
	if( Data_Comp( Ptr, Source+2, 6 ) == 0 )
	{
		 Temp_FLAG = 1;
		 ///CTT测试要求可读可写，在此放开写权限,并且只使用0.0.11.0.0.255一条 2015-8-23-fire
		/// AccessMode = read_only;   //激活类的只读，设置只能设置备用公共假日..
	}
	else if( Data_Comp( Ptr+6, Source+2, 6 ) == 0 ) Temp_FLAG = 2; 
	
//	if( Data_Comp( Buff, Source+2, 6 ) != 0 ) 
  if( Temp_FLAG == 0 )
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else if(( AccessMode != write_only )&&( AccessMode != read_and_write ))
	{
		ErrorByte = read_write_denied; 									//无写入权限
	}	 
//	else if( *DataType != IECHDLCSetupClassAttribute_ParaTab[(*Attribute)-1].DataType )
//	{
//		ErrorByte = type_unmatched; 									//无写入权限
//	}	 
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				ErrorByte = read_write_denied; 							//无写入权限
//				break;	                                                
			case 2:														//array spec_day_entry		
				ErrorByte = Judge_spec_day_entry( WriteBufAds );
				///ErrorByte = Judge_spec_day_entry_Test(Test_Sector+10);
				if( ErrorByte == 0 )
				{
					//sky20130711..
					if(( *(WriteBufAds+1) & 0x80 ) != 0 )
					{
						if( *(WriteBufAds+1) == 0x81 )										//设置数据长度是否为1字节 
						{
							Cunt1 = *(WriteBufAds+2);										//有效设置数据长度
							Point = WriteBufAds+3;											//有效设置数据起始地址			
						}			
						else if( *(WriteBufAds+1) != 0x82 ) return other_reason;
						else 
						{	
							Cunt1 = GetBuffToInteger( WriteBufAds+2 );
							Point = WriteBufAds+4;
						}	
					}	
					else
					{
						Cunt1 = *(WriteBufAds+1);
						Point = WriteBufAds+2;
					}	
					//if( Cunt1 > 60 ) return other_reason;		
					if( Cunt1 > 200 ) return other_reason;	
					if( Temp_FLAG == 1 )
					{
						 Addr1 =Espec_day_Number;
						 Addr2 =Spec_day_entry1;
					}
					/*
					else
					{
						 Addr1 = Espec_day_Number_passive;
						 Addr2 =Spec_day_entry_passive;					
					} 		
					*/
					E2P_WData( Addr1, (unsigned char *)&Cunt1, 1 );	//节假日数
				
					//E2P_WData( Espec_day_Number, WriteBufAds+1, 1 );	//节假日数
					//Cunt1 = *(WriteBufAds+1);
					//Point = WriteBufAds+2;
					Ptr = Buff2; 
					//Addr = Spec_day_entry1;
					for( i=0;i<Cunt1;i++ )
					{
						*Ptr = *(Point+3);									//index: long-unsigned		
						*(Ptr+1) = *(Point+4);							//index: long-unsigned		
						RAM_Write( Ptr+2, Point+7, 5 );			//specialday_date: octet-string
						*(Ptr+7) = *(Point+13);							//day_id: unsigned
						E2P_WData( Addr2, Ptr, 8 );
						Addr2 += 9;
           				Point += 14;
					}
					/*
					//公共假日改变..
					if( Cunt1 != 0 )
						{
							///SM.StandardEventCode = Tariff_Parameter_Changed;
							SM.StandardEventCode = Change_Tariff_Program_by_SW;//fire
							LoadRecord( Security_Event_No );
							SM.Event_all_flag |= Security_Event_BIT;
							SM.Error_all_flag |= Security_Event_BIT;
						}
					*/
				}	
				break;
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}		
	return ErrorByte;
}	

unsigned char Judge_Activate_passive_calendar_time( unsigned char* Source )
{
	short i;
	
	if((*Source!=D_OctetStr)||(*(Source+1)!=12))	
	{
		return other_reason;	
	}
	for( i=0;i<4;i++ )
	{
		if( *( Source+2+i ) == 0xFF ) return other_reason;	
	}		
	for( i=0;i<3;i++ )
	{
		if( *( Source+7+i ) == 0xFF ) return other_reason;	
	}
	return 0;		
}	

void Activated_Passive_TOU( void )
{
	unsigned char Buff[12];
	unsigned char *Ptr;
	
	Ptr = Buff;
	if(Clk.daylightsaving == 0x55)  //时区开始需要判断前面的时间进行切换
		{	
			E2P_RData( Ptr,Clock_daylight_savings_lasttime,  12 );	 //起始发生标志..
			
			if(( Data_CompHL( Para.RActivate_passive_calendar_time, Ptr, 4 ) != 0 )||( Data_CompHL( Para.RActivate_passive_calendar_time+5, Ptr+5, 2 ) != 0 ))
				{
					if( Data_CompHL( Para.RActivate_passive_calendar_time, &Clk.YearH, 4 ) != 0 ) return;	
					//if( Data_CompHL( Para.RActivate_passive_calendar_time+5, &Clk.HourH, 3 ) != 0 ) return;	
					if( Data_CompHL( Para.RActivate_passive_calendar_time+5, &Clk.HourH, 2 ) != 0 ) return;	
			  }
		}
	else
		{
			if( Data_CompHL( Para.RActivate_passive_calendar_time, &Clk.YearH, 4 ) != 0 ) return;	
			//if( Data_CompHL( Para.RActivate_passive_calendar_time+5, &Clk.HourH, 3 ) != 0 ) return;	
			if( Data_CompHL( Para.RActivate_passive_calendar_time+5, &Clk.HourH, 2 ) != 0 ) return;	
		}
			
	Execute_Passive_TOU_Activated();
}	

void Execute_Passive_TOU_Activated( void )
{
	unsigned char Buff[150];
	unsigned char* Ptr;
	unsigned short  i;  
	unsigned char LenName,SLen;      
	//unsigned short  i,Len;
//	unsigned short  Addr1,Addr2;
//	unsigned short  Total_number;

	Ptr = Buff;

	E2P_RData( Ptr, EActivityCalendarNo, 1 );						//0:当前时段参数指向ECalendar_name_active
	#if(ACTIVE_FEE ==YesCheck)				//复制备用套到激活套EEPROM中..
		 if( *Ptr != 0 ){
		 	 *Ptr = 0;													
		 	 E2P_WData( EActivityCalendarNo, Ptr, 1 );						//0:当前时段参数指向ECalendar_name_active			
//		 	 Addr1 =ECalendar_name_passive;
//		 	 Addr2 =ECalendar_name_active;
//		 }else{
//		 		Addr1 =ECalendar_name_active;
//		 		Addr2 =ECalendar_name_passive;
		 }
//		 //复制数据.. 需要复制1451个字节数据..
//	 	Total_number = (ECalendar_name_passive) -(ECalendar_name_active)-1;
//	 	for(i=0;i<Total_number;i++)
//	 	{
//	 			Len = Total_number -i;
//	 			if(Len>128){
//	 				Len = 128;
//	 			} 
//	 			E2P_RData( Ptr, Addr2+i, Len );				//读备用..
//	 			E2P_WData( Addr1+i, Ptr, Len );				//替换激活..
//	 			i +=(Len-1);		
//	 	}
 //复制数据.. 
 //激活数据..
    E2P_RData( &LenName, ECalendar_name_passiveNEW, 1 );
    if(LenName>12) LenName =12;
    E2P_RData( Ptr, ECalendar_name_passiveNEW+2, LenName );
	 	E2P_WData( ECalendar_name_activeNEW, &LenName, 1 );				   //替换激活ECalendar_name_active..
	 	E2P_WData( ECalendar_name_activeNEW+2, Ptr, LenName );				//替换激活ECalendar_name_active.. 
	 	
	 	//预留执行主副费率时使用..    
 	 	E2P_RData( Ptr, ECalendar_name_passive, 2 );				//读备用ECalendar_name_passive..
	 	E2P_WData( ECalendar_name_active, Ptr, 2 );				//替换激活ECalendar_name_active..
	 	
	 	//ESeasonNumber_passive..
 	 	E2P_RData( Ptr, ESeasonNumber_passive, 1 );				//读备用..
	 	E2P_WData( ESeasonNumber_active, Ptr, 1 );				//替换激活..	 	
	 	//Season_profile1_passive
	 	for(i=0;i<6;i++)
	 	{
	 			E2P_RData( Ptr, ESeasonNumber_passive+2+i*15, 14 );				//读备用..
	 			E2P_WData( ESeasonNumber_active+2+i*15, Ptr, 14 );				//替换激活..
	 	}
	 	//EWeekNumber_passive
 	 	E2P_RData( Ptr, EWeekNumber_passive, 1 );				//读备用..
	 	E2P_WData( EWeekNumber_active, Ptr, 1 );				//替换激活..	 		 	
	 	 	for(i=0;i<8;i++)
	 	{
	 			E2P_RData( Ptr, EWeekNumber_passive+2+i*9, 8 );				//读备用..
	 			E2P_WData( EWeekNumber_active+2+i*9, Ptr, 8 );				//替换激活..
	 	}
	 	HT_FreeDog();
	 	//EDay_ProfileNumber_passive 	 	
	 	 	for(i=0;i<17;i++)
	 	{
	 			E2P_RData( Ptr, EDay_ProfileNumber_passive+i*2, 1 );				//读备用..
	 			E2P_WData( EDay_ProfileNumber_active+i*2, Ptr, 1 );				//替换激活..
	 	}			
	 	//day_profile_action1_schedule1_passive 	 	
	 	 	for(i=0;i<96;i++)
	 	{
	 			E2P_RData( Ptr, day_profile_action1_schedule1_passive+i*13, 12 );				//读备用..
	 			E2P_WData( day_profile_action1_schedule1_active+i*13, Ptr, 12 );				//替换激活..
	 	}
	 ///	WDTCTL = WDT_ARST_1000;	
	 HT_FreeDog();
	 //公共假日备用TO激活
	 /*
	 //台湾不用备用
	 	E2P_RData( &SLen, Espec_day_Number_passive, 1 );
    if(SLen>94) SLen = 0;
    E2P_WData( Espec_day_Number, &SLen, 1 );
 	 	for(i=0;i<SLen;i++)
	 	{
	 			E2P_RData( Ptr, Spec_day_entry_passive+i*9, 8 );				//读备用..
	 			E2P_WData( Spec_day_entry1+i*9, Ptr, 8 );				//替换激活..
	 	}
	*/
	/// 	WDTCTL = WDT_ARST_1000;	
	HT_FreeDog();
	#else
			if( *Ptr != 0 ) *Ptr = 0;														//1:当前时段参数指向ECalendar_name_passive	
			else *Ptr = 1;
			E2P_WData( EActivityCalendarNo, Ptr, 1 );						//0:当前时段参数指向ECalendar_name_active			
	#endif	
	
	GetDay_schedule( GetDay_id() );											//判断取当前费率
	GetFeeNo();
	///SM.StandardEventCode = Passive_TOU_Activated;
	SM.StandardEventCode = Change_Tariff_Program_by_SW;
	LoadRecord( Standard_Event_No );
	SM.Event_all_flag |= Standard_Event_BIT;
	SM.Error_all_flag |= Standard_Event_BIT;
	
}	


//////////////////////////////////////////////////////////////////////////
unsigned char ActionActivity_calendarClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method;
		
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0D;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	

	// if((( Method == 1 )&&(*(Source+9)==0x0f)&&(*(Source+10)==0x0))||(( Method == 1 )&&(*(Source+9)==0x00)))
	//ACTION后跟的数据格式问题，错误的适应修改20151215..
  if((( Method == 1 )&&(*(Source+9)==0x01)&&(*(Source+10)==0x0f)&&(*(Source+11)==0x0))||(( Method == 1 )&&(*(Source+9)==0x0f)&&(*(Source+10)==0x0))||(( Method == 1 )&&(*(Source+9)==0x00)))
	{
//		E2P_RData( Ptr, EActivityCalendarNo, 1 );						//0:当前时段参数指向ECalendar_name_active	
//		if( *Ptr != 0 ) *Ptr = 0;										//1:当前时段参数指向ECalendar_name_passive	
//		else *Ptr = 1;
//		E2P_WData( EActivityCalendarNo, Ptr, 1 );						//0:当前时段参数指向ECalendar_name_active	
//		GetDay_schedule( GetDay_id() );									//判断取当前费率
//		GetFeeNo();
//		SM.StandardEventCode = Passive_TOU_Activated;
//		LoadRecord( Standard_Event_No );
	//	Execute_Passive_TOU_Activated();
	}
	else ErrorByte = other_reason;		

	return ErrorByte;
}		

//////////////////////////////////////////////////////////////////////////
unsigned char ActionSpecial_DaysClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[20];
	unsigned char* Ptr;
        unsigned char* Ptr_Buff;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method,Items;
	unsigned char j,k;
	unsigned short  Addr;
		
	Ptr_Buff = Buff;
	
	Logical_Name = Source+2;
	Method = *(Source+8);
	
//	0.0.11.0.0.255
//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x0B; 
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//只能设置备用..		
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0B; 
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr_Buff, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	

	Ptr = Source+9;
	Addr = Espec_day_Number;
	//Addr = Espec_day_Number_passive;
	
	// 增加ACTION错误兼容性判断20151215..
	if(*Ptr==0x01)
   {
          Ptr = Source+10;
   }
        
	if( Method == 1 )					//add..
	{
		if((*Ptr!=Data_structure)||(*(Ptr+1)!= 3)					//spec_day_entry ::= structure
			||(*(Ptr+2)!=D_Unsigned16)											//index: long-unsigned
			||(*(Ptr+5)!=D_OctetStr)||(*(Ptr+6)!=0x05)				//specialday_date: octet-stringg
			||(*(Ptr+12)!=D_Unsigned8))												//day_id: unsigned
		{
			return other_reason;
		}
		
		E2P_RData( (unsigned char *)&Items, Addr, 1 );
		for(j=0;j<Items;j++)
		{
				 E2P_RData( Ptr_Buff, Addr+9*j+2, 7 );				 					 
				 if(( Data_Comp( Ptr+3, Ptr_Buff, 2 ) == 0 )||( Data_Comp( Ptr+7, Ptr_Buff+2, 5 ) == 0 )){				//替换..
				 	 	 E2P_WData( Addr+9*j+2, Ptr+3, 2 );				 		 		 	
				 	 	 E2P_WData( Addr+9*j+4, Ptr+7, 5 );
				 	 	 E2P_WData( Addr+9*j+9, Ptr+13, 1 );				 	 	 
				 	 	 return 0;					 	 
			 	}
		}
		//没有相同则增加1条..				
				//if(Items >60 )  return other_reason;
				//if(Items >200 )  return other_reason;	
				if(Items >94 )  return other_reason;												
				E2P_WData( Addr+9*Items+2, Ptr+3, 2 );				 		 		 	
				E2P_WData( Addr+9*Items+4, Ptr+7, 5 );
	 	 		E2P_WData( Addr+9*Items+9, Ptr+13, 1 );						
				Items++;
				E2P_WData( Addr, (unsigned char *)&Items, 1 );

	}else if( Method == 2 ){			//delete..
		
		 	  if(*Ptr != D_Unsigned16 )  return other_reason;		
				E2P_RData( (unsigned char *)&Items, Addr, 1 );		
				if(Items>200)  return other_reason;	
					
				for(j=0;j<Items;j++)
				{
					 E2P_RData( Ptr_Buff, Addr+9*j+2, 2 );					 
					 if( Data_Comp( Ptr+1, Ptr_Buff, 2 ) == 0 ){				//找到相同..
						 		for(k=j+1;k<Items;k++)
						 		{
						 			E2P_RData( Ptr_Buff, Addr+9*k+2, 9 );			
						 			E2P_WData( Addr+9*j+2, Ptr_Buff, 9 );		
						 			j++;					 	 	 
						 		}
				 				RAM_Fill( Ptr_Buff, 9);
								E2P_WData( Addr+9*j+2, Ptr_Buff, 9 );								

								//if(Items==1) Items--; 
								Items--;		//一定大于或等于1..
								E2P_WData( Addr, (unsigned char *)&Items, 1 );					 				 		 	
					 	 	 return 0;					 	 
					 	}
				}
				ErrorByte = other_reason;									//无此mask_name..		
	}	
	else ErrorByte = other_reason;		

	return ErrorByte;
}

		
//////////////////////////////////////////////////////////////////////////
short CompSpecday( unsigned char* Source )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	short i;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, &Clk.YearH, 5 );
	for( i=0;i<4;i++ )
	{
		if( *(Source+i) == 0xFF ) *(Ptr+i) = 0xFF; 	
	}	
	
	if( Data_Comp( Source, Ptr, 4 ) == 0 ) return 0;
	return 1;	   
	
}	

short CompSeason( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	short i;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, &Clk.YearH, 12 );
	for( i=0;i<12;i++ )
	{
		if( *(Source+i) == 0xFF ) *(Ptr+i) = 0xFF; 	
	}	
	
//	if( Data_Comp( Source, Ptr, 8 ) >= 0 ) return 0;
//	if( Data_CompHL( Source, Ptr, 8 ) >= 0 ) return 0;
//	return 1;	   

	if( Data_CompHL( Ptr, Source, 8 ) >= 0 ) return 1;
	return 0;	   
	
}	

unsigned short GetAddrOffset( void )
{
	unsigned char Buff[4];
	unsigned char* Ptr;
	unsigned short AddrOffset;
	unsigned char Temp;
	unsigned short PassiveAddr;
	
	Ptr = Buff;	
	E2P_RData( &Temp, EActivityCalendarNo, 1 );								//0:当前时段参数指向ECalendar_name_active	
	#if(ACTIVE_FEE ==YesCheck)				//复制备用套到激活套EEPROM中..
		 if( Temp != 0 ){
		 	 Temp = 0;
		 }
	#endif													
	if( Temp != 0 )															//1:当前时段参数指向ECalendar_name_passive	
	{
//		AddrOffset = ECalendar_name_passive - ECalendar_name_active; 
		AddrOffset = ECalendar_name_passive;
		AddrOffset -= ECalendar_name_active; 
		PassiveAddr = ECalendar_name_active;
	}
	else 
	{
		PassiveAddr = ECalendar_name_passive;
		AddrOffset = 0;
	}	
	
	E2P_RData( Ptr, PassiveAddr, 2 );								
	E2P_RData( Ptr+2, Passive_calendar_name, 2 );			
	if(( *Ptr != *(Ptr+2) )||( *(Ptr+1) != *(Ptr+3) ))
	{
		E2P_WData( Passive_calendar_name, Ptr, 2 );						
	}		
	
	return AddrOffset;
}	

short GetDay_id( void )
{
	unsigned char Buff[16];
	unsigned char* Point;
	short i;
	unsigned short Sum;
	unsigned short AddrOffset;
	unsigned short Addr;
	unsigned char Week_Name;
	unsigned char Day_id;
	
	Point = Buff;
	
//	AddrOffset = GetAddrOffset();
	AddrOffset = 0;
	E2P_RData( Point,Espec_day_Number, 1 ); 
	if(( Flag.BatState & F_E2PCheck ) != 0 ) Sum = 0; 		
	else Sum = *Point;
	
	//if( Sum > 60 ) Sum = 0;											//节假日数非法
	if( Sum > 200 ) Sum = 0;											//节假日数非法
	
	for( i=0;i<Sum;i++ )
	{
		E2P_RData( Point,Spec_day_entry1+i*9, 8 ); 
		if( CompSpecday( Point+2 ) == 0 )
		{
			Day_id = *(Point+7);
			//if(( Day_id > 8 )||(Day_id < 1)) Day_id = 1;				//Day_id:1--8
			if( Day_id >= 8 ) Day_id = 1;										//Day_id:0--7
			return Day_id;				
		}				
	}	

	Addr = ESeasonNumber_active + AddrOffset;
	E2P_RData( Point,Addr, 1 ); 
	if(( Flag.BatState & F_E2PCheck ) != 0 ) Sum = 0; 		
	else Sum = *Point;
	if(( Sum > ZONENUMBER )||( Sum == 0 )) 
	{
		Day_id = 1;					//季历表数非法
		return Day_id;
	}	

	Addr = Season_profile1_active + AddrOffset;
	if( Sum == 1 )
	{
		E2P_RData( Point,Addr, 14 );
		Week_Name = *(Point+13);		
	}	
	else
	{
		E2P_RData( Point,Addr+(Sum-1)*15, 14 );
		Week_Name = *(Point+13);				
		for( i=0;i<Sum;i++ )
		{
//			E2P_RData( Point,Addr+i*15, 14 ); 
			E2P_RData( Point,Addr+(Sum-i-1)*15, 14 ); 
			if( CompSeason( Point+1 ) != 0 ) break;
		}
		if(i!=Sum) Week_Name = *(Point+13);    //时间不是从00:00开始，会遇到错误，进行修改20160312..
	}		
	
	Addr = EWeekNumber_active + AddrOffset;
	E2P_RData( Point,Addr, 1 ); 
	if(( Flag.BatState & F_E2PCheck ) != 0 ) Sum = 0; 		
	else Sum = *Point;
	if(( Sum > WEEKNUMBER )||( Sum == 0 )) 
	{
		Day_id = 1;					//周历表数非法
		return Day_id;
	}	

	Addr = Week_profile1_active + AddrOffset;
	for( i=0;i<Sum;i++ )
	{
		E2P_RData( Point,Addr+i*9, 8 );
		if( Week_Name == *Point ) break;
	}
	if( i == Sum ) 
	{
		Day_id = 1;					//周历表内容非法
		return Day_id;
	}
	else Day_id = *(Point+Clk.WeekH);
		
	return Day_id;	
}	

void GetDay_schedule( unsigned char Day_id )
{
	unsigned char Buff[16];
	unsigned char* Point;
	unsigned char Temp;
	short i;
	unsigned short Sum;
	unsigned short AddrOffset;
	unsigned short Addr;
	unsigned short ScheduleNo;

	Point = Buff;

	//AddrOffset = GetAddrOffset();
	AddrOffset = 0;
	HT_FreeDog();
	Addr = EDay_ProfileNumber_active + AddrOffset;
	E2P_RData( Point,Addr, 1 ); 
	if(( Flag.BatState & F_E2PCheck ) != 0 ) Sum = 0; 		
	else Sum = *Point;
	if(( Sum > 8 )||( Sum == 0 )) 
	{
		Day_id = 1;					//日时段表数非法
	}	

	for( i=0;i<Sum;i++ )
	{
		Addr = Day_id1_active + AddrOffset;
    Addr += i*2;
		E2P_RData( Point,Addr, 1 ); 
		if(( Flag.BatState & F_E2PCheck ) != 0 ) continue; 		
		else Temp = *Point;
		if( Temp == Day_id ) break;		
	}	
	if( i == Sum ) ScheduleNo = 0;
	else ScheduleNo = i;
	
	Addr = EDay_Schedule1_Number_active + AddrOffset;
	Addr += ScheduleNo*2;
     E2P_RData( Point,Addr, 1 ); 
	if(( Flag.BatState & F_E2PCheck ) != 0 ) Sum = 0;
	else Sum = *Point;
	//if(( Sum == 0 )||( Sum >= SEGNUMBER )) Sum = 1;
	if(( Sum == 0 )||( Sum > SEGNUMBER )) Sum = 1;
	
	Addr = day_profile_action1_schedule1_active + ScheduleNo*SEGNUMBER*13 + AddrOffset;
	for( i=0;i<SEGNUMBER;i++ )
	{
		if( i < Sum )
		{
			E2P_RData( Point, Addr, 12 ); 
			RAM_Write( Para.Seg[i], Point, 3 );								//时分秒
			Para.Seg[i][3] = GetBuffToInteger( Point+10 );					//费率号
			Addr += 13;
		}
		else
		{
//			RAM_Write( Para.Seg[i], Para.Seg[i-1], 3 );
			RAM_Write( Para.Seg[i], Para.Seg[i-1], 4 );
		}		
	}	
	
}	

//伊朗表方法
void GetFeeNo(void)
{
	unsigned char Temp;
	unsigned char Temp2;		
	
	Temp = GetNo( (unsigned char*)&Para.Seg[0], (unsigned char*)&Clk.HourH, SEGNUMBER );
	
	if(( Temp > 4 )||(Temp < 1)) Temp = 1;					//EC
	Disk.DisFeeNo  = Temp;									//EC
	
	E2P_RData( (unsigned char*)&Temp2, EFeeNo, 1 ); 		//EC
	if(( Temp2 <= 4 )&&(Temp2 >= 1)&&((Flag.BatState & F_E2PCheck) == 0 )) //EC		
	{														//EC		
		SM.FeeNo = Temp2;									//EC
	}														//EC
	if(( SM.FeeNo > 4 )||(SM.FeeNo < 1)) SM.FeeNo = Temp;	//EC
	if((Flag.Power & F_PwrUp) == 0)	return;					//EC
	
	RAM_Fill( SM.Currently_Active_tariff, 4 );
	SM.Currently_Active_tariff[0] = Temp;
	
	SM.FeeNo = Temp;//A B C D
	if( SM.FeeNo != Temp2 )	E2P_WData( EFeeNo, (unsigned char*)&Temp, 1 ); 	//EC	写入新的费率号
	SM.Fee= SM.FeeNo+0x40;
	
}	

/*
unsigned char GetNo(unsigned char* Point, unsigned char* Source, short ItemNums)
{
	unsigned char Temp;
	short	i,j;
	
//	Temp = *(Point+(ItemNums-1)*4);
//	Temp = *(Point+(ItemNums-1)*4) + 3;
	Temp = *(Point+(ItemNums-1)*4 + 3);

	for( i=ItemNums;i>0;i-- )
	{
		j = (i-1)*4;
//		if( Data_Comp( Source, Point+j, 3) >= 0 )
		if( Data_CompHL( Source, Point+j, 3) >= 0 )
		{
			Temp = *(Point+j+3);	
			break;				
		}	
	}
	return Temp;
}
*/
/*************************************************************************************************
 Function name: unsigned char judge_daylightsaving()
 Description: 判断夏令时开始和结束，并做调整（不判秒跟其它设置）..
 							备注：由于伊朗都是有日期设置的夏令时，故先只做有固定日期的夏令时，暂时不考虑按周设置;
 										（需要按照只要在判断的地方加上判断条件即可），
 							      需要设置明确的月日（结束时间的月份需要大于起始时间）等时间，考虑伊朗的24小时设置；							      
 Return: daylight_deviation ---夏令时偏差的全局变量..
 				 0x55 --> 开始
         0xaa --> 结束    
         0x0  --> 出错或无夏令时，不做夏令时判断
         
         change_time_flag =0xa6 --时间修改后判断..   
**************************************************************************************************/
unsigned char judge_daylightsaving( unsigned char change_time_flag ) 
{
	unsigned char in_flag =0;
	unsigned char tell_flag = 0;
	unsigned short temp_curr,temp_start,temp_end,temp_last,temp;
	unsigned char* start_Ptr;
	unsigned char* end_Ptr;
	unsigned char* last_Ptr;
	         short  int_i;								//做提取数据判断用..
	  signed char daylight_deviation;		//夏令时偏差..
  //使用CLK结构定义..
  					CLK 	clk_start;				 	 
  					CLK 	clk_end;				 	 
						CLK   clk_last;						//上次夏令时记录时间..
											
	start_Ptr = &clk_start.YearH;
	end_Ptr = &clk_end.YearH;
	last_Ptr = &clk_last.YearH;
	
	daylight_deviation = 0;		//偏差..
		
	E2P_RData( &in_flag, Clock_daylight_savings_enabled, 1 );				//判夏令时使能TURE = 0xff..
	if( in_flag == 0 ) return 0;		//未使能..
	
	
	E2P_RData( &in_flag, Clock_daylight_savings_deviation, 1 );			//读取夏令时偏差,范围需要在-120到120之间,单位：分钟..
	if(( in_flag == 0xff )||( in_flag == 0x00 )) return 0;	
	daylight_deviation = ( signed char )in_flag;
	if((daylight_deviation>120)||(daylight_deviation<-120)) return 0;
		
	
	E2P_RData( start_Ptr, Clock_daylight_savings_begin, 12 );				// 判断夏令时开始和结束设置数据对错..
	E2P_RData( end_Ptr, Clock_daylight_savings_end, 12 );
	
	E2P_RData( last_Ptr, Clock_daylight_savings_lasttime, 12 );			// 读取上次执行夏令时的时间..
	
	//判年..
	int_i = Data_CompHL(start_Ptr,end_Ptr,2);
	if( int_i != 0 ){ 
			return 0;
	}else{
				if((clk_start.YearH==0xff)&&(clk_start.YearL==0xff)){
					 in_flag = 1;			//年份未指定..					 
				}else{
						  //in_flag = 0;
						  if((Clk.YearH==clk_start.YearH)&&(Clk.YearL==clk_start.YearL)) in_flag = 2; //年份一致..
						 // 指定年份没意义，故年份一定要0xffff未指定..
						 }					
				}
	if(in_flag == 0) return 0;
	
	//年份未指定情况下，判月(月份也不能在同一个月)..
	//设置不对，则不进行判断..
	if(( clk_start.Month>=clk_end.Month )||(clk_start.Month>12)||(clk_end.Month>12)||(clk_start.Month==0)||(clk_end.Month==0)) return 0;
	if((clk_start.DayH>31)||(clk_start.HourH>24)||(clk_end.DayH>31)||(clk_end.HourH>24)) return 0;	
	
	//判内部存储的日期时间跟当前时间比较..
	
	//年比对..
	// if((Clk.YearH>clk_last.YearH+1)||(Clk.YearH<clk_last.YearH)) return 0;
	
	temp_curr = Clk.YearL;
	temp_last = clk_last.YearL;		
	if(Clk.YearH==clk_last.YearH+1)
	{	
		temp_curr += 0xff;		
	}
	if((temp_curr < temp_last )||(Clk.YearH<clk_last.YearH)||(Clk.YearH>clk_last.YearH+1))		
	{
	  	E2P_WData( Clock_daylight_savings_lasttime, &Clk.YearH, 12 );		
	  	return change_time_flag;
	}else{
				if( temp_curr >( temp_last+1) ) temp = 1;		//停电年份超过两年都当做跨1年来处理，不判月的情况..
				else temp = 0;						//年份一致情况下..

				clk_start.YearH = Clk.YearH;		
				clk_start.YearL = Clk.YearL;	
				clk_end.YearH = Clk.YearH;		
				clk_end.YearL = Clk.YearL;
				
				//不判周，留待以后增加判断用..
				clk_start.WeekH = Clk.WeekH;
				clk_end.WeekH = Clk.WeekH;
				clk_last.WeekH = Clk.WeekH;					
		}	
	
	//月比对..
	//上次修改夏令时的时间不是夏令时切换的时间,跟夏令时起始和结束时间先进行比对..
	int_i  =  Data_CompHL(&clk_start.Month,&clk_last.Month,5);	//一直比对到分钟..
	if(int_i!=0){
			if(int_i>0){		// ==1;大于..		
				 // E2P_WData( Clock_daylight_savings_oldtime, &Clk_end.YearH, 12 );	 //结束标志..
				 tell_flag = 2;		
			}else{			   //if(int_i<0){	//==-1 小于..		
					int_i  = Data_CompHL(&clk_last.Month,&clk_end.Month,5);	//一直比对到分钟..
					if(int_i!=0){
						if(int_i<0){
								 tell_flag = 1;		
						}else  tell_flag = 2;	
					}else 	tell_flag = 0xAA;			//夏令时开始已经做调整..			
			}
	}else  tell_flag = 0x55;			//夏令时开始已经做调整..


	if(tell_flag!=0){
		if(change_time_flag==0xa6){
				if((tell_flag == 1)||(tell_flag == 2)) return 0;			//设置时间，原来没调整过..
		}
		if(tell_flag == 1){
					E2P_WData( Clock_daylight_savings_lasttime, &clk_start.YearH, 12 );	 //起始发生标志..
					tell_flag = 0x55;			//夏令时开始已经做调整..
		}else{
					if(tell_flag == 2){
						 E2P_WData( Clock_daylight_savings_lasttime, &clk_end.YearH, 12 );	 //结束发生标志..		
						 tell_flag = 0xAA;			//夏令时开始已经做调整..
					}
		}
	}
	
	in_flag  = 0;		//转为设置时间标志，0x55夏令时开始时间需要做调整；0xAA夏令时结束时间需要做调整..
	//月比对，不支持跨月的夏令时..
	if((Clk.Month!=clk_start.Month)&&(Clk.Month!=clk_end.Month)){	
		if(temp==1){
			if(Clk.Month<clk_start.Month){					
						if(tell_flag == 0x55 ){
								in_flag = 0xAA;	
						}
					} 			
			if((Clk.Month>clk_start.Month)&&(Clk.Month<clk_end.Month)){
			 			if(tell_flag == 0xAA ){
								in_flag = 0x55;	
						}
				} 	
			if(Clk.Month>clk_end.Month){
						if(tell_flag == 0x55 ){
								in_flag = 0xAA;	
						}
			} 	
	  }
 }
 if(in_flag==0){
		if(Clk.Month==clk_start.Month){		//起始和结束不能在同一个月，否则需要在此加入判断程序..
				//日*1440+时*60+分
				temp_curr = (unsigned short)Clk.DayH*1440 +(unsigned short)Clk.HourH*60 +Clk.MinH;
				temp_start = (unsigned short)clk_start.DayH*1440 +(unsigned short)clk_start.HourH*60 +clk_start.MinH;
				if(change_time_flag==0xa6){
						if(	tell_flag == 0x55	){		//调整夏令时开始调整..
								if(daylight_deviation>0){
									if(temp_start <= temp_curr <= (temp_start+daylight_deviation)) return  change_time_flag;										
								}else if(daylight_deviation<0){
									if((temp_start +daylight_deviation) <= temp_curr <= temp_start) return  change_time_flag;										
								}
						}
						return 0;
				}									
				if(temp_curr>=temp_start){
						if(tell_flag != 0x55 ){
								in_flag = 0x55;
								E2P_WData( Clock_daylight_savings_lasttime, &clk_start.YearH, 12 );	 //起始发生标志..									
						}
				}
		}else{	
				if(Clk.Month==clk_end.Month){		//月份等于结束..
						//日*1440+时*60+分
						temp_curr = (unsigned short)Clk.DayH*1440 +(unsigned short)Clk.HourH*60 +Clk.MinH;
						temp_end = (unsigned short)clk_end.DayH*1440 +(unsigned short)clk_end.HourH*60 +clk_end.MinH;
						if(change_time_flag==0xa6){
								if(	tell_flag == 0xAA	){		//调整夏令时结束调整..
										if(daylight_deviation>0){
											if(temp_end-daylight_deviation <= temp_curr <= temp_end ) return  change_time_flag;										
										}else if(daylight_deviation<0){
											if(temp_end <= temp_curr <= (temp_end-daylight_deviation)) return  change_time_flag;										
										}
								}
								return 0;
						}							
						if(temp_curr>=temp_end){
								if(tell_flag != 0xAA ){
										in_flag = 0xAA;
 										E2P_WData( Clock_daylight_savings_lasttime, &clk_end.YearH, 12 );	 //结束发生标志..													
								}
							}
				}
			}
	}
	
if( in_flag !=0 ){
				if(	in_flag == 0x55	){		//调整夏令时开始调整..		
			//		if(daylight_deviation>0){
			//				temp_curr += daylight_deviation;
			//		}else{
			//				temp_curr += daylight_deviation;
			//		}
							temp_curr += daylight_deviation;				
				}else{
				//		if(	in_flag == 0xAA	){		//调整夏令时结束调整..
							temp_curr -= daylight_deviation;
				//		}
				}
				
				//夏令时切换的事件记录(时钟修改前记录)..
				SM.StandardEventCode = Day_light_saving_time_Activated_regularly;
				LoadRecord( Standard_Event_No );
				SM.Event_all_flag |= Standard_Event_BIT;
					SM.Error_all_flag |= Standard_Event_BIT;
					
			//调整到日期时间修改<日时分>上..  
				temp = (temp_curr/1440);
				if(Clk.DayH!=temp){			//跨日..
					if(Clk.DayH == temp+1) Clk.WeekH -= 1;
					else Clk.WeekH += 1;
					if(Clk.WeekH==0) Clk.WeekH = 7;
					if(Clk.WeekH==8) Clk.WeekH = 1;	
				}
				Clk.DayH = temp;		
				// Clk.DayH = (temp_curr/1440);  //跨日需要判周..
				temp = temp_curr%1440;
				Clk.HourH = temp/60;
				Clk.MinH = temp%60;
				
			// 这里可以记录修改夏令时的时间的事件记录..
				//E2P_WData( XXXX, &Clk.YearH, 12 );
				MoveCurrentTimeHex_BCD();					
				RAM_Write( start_Ptr , Clk.Sec, 3 );
				*(start_Ptr+3) = Clk.Week[0];
				RAM_Write( start_Ptr+4, Clk.Day, 3 );
		/*
		///用表自身的RTC，硬件时钟
		#if( RTCCHIP == RX8025 )
				 E2Pwrite( 0x00, start_Ptr, 7, RTCAds );		//写时钟芯片日期
			#elif( RTCCHIP == RX8025T )																	
					*(start_Ptr+3) = (unsigned char)ByteBit[*(start_Ptr+3)];					
					E2Pwrite( 0x00, start_Ptr, 7, RTCAds );		//写时钟芯片日期	//8025T
			#else
					if( *(start_Ptr+3) == 0 ) *(start_Ptr+3) = 0x07;
					E2Pwrite( 0x00, start_Ptr, 7, DS3231Ads );	//写时钟芯片日期
			#endif
			*/
	}
	return in_flag;
}
