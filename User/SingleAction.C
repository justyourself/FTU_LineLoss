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
#include "Demand.h"

#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "ActCalendar.h"
#include "RegisterMonitor.h"
#include "SingleAction.h"
#include "ProfileGeneric.h"
//#include "GetData.h"

//#include "DisconnectControl.h"
#include "ScriptTable.h"
#include "AssociationLN.h"
#include "Limiter.h"
#include "ComSetup.h"
//取Single_action_schedule类数据-class 22:
short GetSingle_action_scheduleClassData( unsigned char* Dest, unsigned char* Source ) 
{
	short i;
	unsigned char Buff[10];
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	unsigned char* Point;	
	unsigned short Addr,Addr1;
	unsigned char Len=2;
	unsigned char Attribute,Cunt1;
	
	Ptr = Logical_Name;
	RAM_Write( Ptr, Source+2, 6 );
	
	for( i=0;i<DLMS_Sigle_actSum;i++ )
	{
		if( Data_Comp( Ptr, (unsigned char*)DLMS_Sigle_actTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	if( i == DLMS_Sigle_actSum )
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}
	else 
	{
		Addr = DLMS_Sigle_actTab[i].Addr;
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
			case 2:										//executed_script	
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x02;
				*(Dest+3) = Data_octet_string;
				*(Dest+4) = 0x06;
				E2P_RData( Dest+5, Addr, 6 );
				*(Dest+11) = D_Unsigned16;
                Addr += 7;
				E2P_RData( Dest+12, Addr, 2 );
				Len = 14;					
				break;	
			case 3:										//type
				*(Dest+1) = D_ENUM;
				Addr += Single1_type;
				Addr -= Single1_script_logical_name;
				E2P_RData( Dest+2, Addr, 1 );
				if( *(Dest+2) > 5 ) *(Dest+2) = 3;		//all time values are the same, wildcards in date are allowed,
				Len = 3;					
				break;	
			case 4:										//execution_time	
//				*(Dest+1) = Data_array;
//				*(Dest+2) = 0x01;
//				*(Dest+3) = Data_structure;
//				*(Dest+4) = 0x02;
//				Addr += Single1_execution_time1;
//				Addr -= Single1_script_logical_name;
//				E2P_RData( Buff, Addr, 9 );
//				*(Dest+5) = Data_octet_string;
//				*(Dest+6) = 0x04;
//				RAM_Write( Dest+7, Buff, 4 );
//				*(Dest+11) = Data_octet_string;
//				*(Dest+12) = 0x05;
//				RAM_Write( Dest+13, Buff+4, 5 );
//				Len = 18;

				*(Dest+1) = Data_array;
				Addr1 = Addr;
				Addr1 +=Single1_execution_time_Number;  //数量..
				Addr1 -= Single1_script_logical_name;
				E2P_RData( Dest+2, Addr1, 1 );
				if( *(Dest+2) > 5 ) *(Dest+2) = 5;					//最多5个时间
				Cunt1 = *(Dest+2);
				Len = 3;
				Point = Dest+3;
				Addr += Single1_execution_time1;
				Addr -= Single1_script_logical_name;							
				for( i=0;i<Cunt1;i++ )
				{				
					*Point  = Data_structure;
					*(Point+1) = 0x02;					
					E2P_RData( Buff, Addr, 9 );
					*(Point+2) = Data_octet_string;
					*(Point+3) = 0x04;
					RAM_Write( Point+4, Buff, 4 );
					*(Point+8) = Data_octet_string;
					*(Point+9) = 0x05;
					RAM_Write( Point+10, Buff+4, 5 );
					Addr += 10;
					Len += 15;
					Point += 15;
			  }
				break;	
			default:
				break;	
		}	
	}		
	return Len;
}	

short Judge_executed_script( unsigned char* Data )
{
	if(( *Data != Data_structure )||( *(Data+1) != 2 )) 
	{
		return other_reason;
	}
	if(( *(Data+2) != Data_octet_string )||( *(Data+3) != 6 )||( *(Data+10) != D_Unsigned16 ))
	{
		return other_reason;
	}
	return 0;
}

//short Judge_execution_time( unsigned char* Data )
//{
//	if(( *Data != Data_array )||( *(Data+1) != 1 )) 			//只接受一组执行时间
//	{
//		return other_reason;
//	}
//	if(( *(Data+2) != Data_structure )||( *(Data+3) != 2 )) 		
//	{
//		return other_reason;
//	}
//	if(( *(Data+4) != Data_octet_string )||( *(Data+5) != 4 )||( *(Data+10) != Data_octet_string )||( *(Data+11) != 5 )) 
//	{
//		return other_reason;
//	}		
//    return 0;
//}

short Judge_execution_time( unsigned char* Data )
{
	short i;
	unsigned char Item;
	unsigned char* Ptr;
	
	if(( *Data != Data_array )||( *(Data+1) > 5 )) 			//只接受小于5组执行时间
	{
		return other_reason;
	}
	Item = *(Data+1);
	Ptr = Data+2;
	for( i=0;i<Item;i++ )
	{
		if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 ))
		{
			return other_reason;
		}	
		if(( *(Ptr+2) != Data_octet_string )||( *(Ptr+3) != 4 )||( *(Ptr+8) != Data_octet_string )||( *(Ptr+9) != 5 )) 
		{
			return other_reason;
		}		
		Ptr += 15;		
	}	
    return 0;
}


unsigned char SetSingle_action_scheduleClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[10];
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
	
	for( i=0;i<DLMS_Sigle_actSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_Sigle_actTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	if( i == DLMS_Sigle_actSum ) { return object_undefined; }
	
	if(( *Attribute == 1 )||( DLMS_Sigle_actTab[i].ComInfo & WRITE ) == 0 ) { return read_write_denied; }

	switch( *Attribute )
	{
//		case 1:														//logical_name			
//			break;	
		case 2:														//executed_script		
			ErrorByte = Judge_executed_script( WriteBufAds );
			if( ErrorByte == 0 )
			{
				Addr = DLMS_Sigle_actTab[i].Addr;
				E2P_WData( Addr, WriteBufAds+4, 6 );					
				Addr += 7;
				E2P_WData( Addr, WriteBufAds+11, 2 );									
			}	
			break;	
		case 3:														//type		
			if(( *WriteBufAds != D_ENUM )||( *(WriteBufAds+1) > 5 )||( *(WriteBufAds+1) == 0 ))
			{
				ErrorByte = other_reason;
				break;	
			}	
			Addr = DLMS_Sigle_actTab[i].Addr;
			Addr += Single1_type;
			Addr -= Single1_script_logical_name;
			E2P_WData( Addr, WriteBufAds+1, 1 );
			break;	
		case 4:														//execution_time		
			ErrorByte = Judge_execution_time( WriteBufAds );		//只支持5组执行时间
			if( ErrorByte == 0 )
			{
				Addr = DLMS_Sigle_actTab[i].Addr;
				Addr += Single1_execution_time_Number;
				Addr -= Single1_script_logical_name;
				E2P_WData( Addr, WriteBufAds+1, 1 );				//arrays		
				
				Addr = DLMS_Sigle_actTab[i].Addr;
				Addr += Single1_execution_time1;
				Addr -= Single1_script_logical_name;
				if(*(WriteBufAds+1)>5) *(WriteBufAds+1) = 5;  //如果大于5超出则只取前5个时间2016-02-05 7:05:20..
				for( i=0;i<*(WriteBufAds+1);i++ )
				{
					RAM_Write( Ptr, WriteBufAds+6+i*15, 4 );		//time
					RAM_Write( Ptr+4, WriteBufAds+12+i*15, 5 );		//date
					E2P_WData( Addr+i*10, Ptr, 9 );	
				}			
//				Addr = DLMS_Sigle_actTab[i].Addr;
//				Addr += Single1_execution_time1;
//				Addr -= Single1_script_logical_name;
//				RAM_Write( Ptr, WriteBufAds+6, 4 );					//time
//				RAM_Write( Ptr+4, WriteBufAds+12, 5 );				//date
//				E2P_WData( Addr, Ptr, 9 );	
//				
//				Addr = DLMS_Sigle_actTab[i].Addr;
//				Addr += Single1_execution_time_Number;
//				Addr -= Single1_script_logical_name;
//				*Ptr = 1;			
//				E2P_WData( Addr, Ptr, 1 );							//array = 1		
			}	
			break;	
		default: 
			ErrorByte = type_unmatched;
			break;			
	}
	return ErrorByte;
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Exec_MDI_reset_End_of_billing_period( void )
{
	unsigned long Value;
	
	LoadRecord( Billing_period1_No );			
  	MD_Rest( );	
	E2P_WData( Time_of_recent_billing_period, (unsigned char*)&Clk.YearH, 12 );
	
	E2P_RData( (unsigned char*)&Value, Billing_period_counter, 4 );
	Value += 1;
	E2P_WData( Billing_period_counter, (unsigned char*)&Value, 4 );
								
}	

short DateComp( unsigned char *Dest, unsigned char *Source )
{
	short i;

	for( i=0;i<4;i++ )
	{
		if( *Dest == 0xFF ) 
		{
			Source += 1;
			Dest += 1;
			continue;
		}	
		if( *Dest != *Source ) return 1;
		Source += 1;
		Dest += 1;
	}	
			
	return 0;	
}	

//十六进制函数文件,月增加一天，年相应增加..
void IncHexYearMonth( unsigned char* TMon )
{
  (*TMon)++;
	if( *TMon > 12 )   //12月
	{
		*TMon = 1; 			//月为1月		
		if(*(TMon-1)==0xFF)
		{
		   *(TMon-1) = 0;
		   *(TMon-2) = *(TMon-2) + 1;	//年加1
	       }
               else   *(TMon-1) = *(TMon-1) + 1;	//年加1
	}	 
}

//返回0：execution_time匹配， 非0：不匹配
/*
	unsigned char YearH;		//HEX时间	
	unsigned char YearL;
	unsigned char Month;
	unsigned char DayH;
	unsigned char WeekH;
	
	unsigned char HourH;
	unsigned char MinH;
	unsigned char SecH;
	unsigned char SecPer;
	unsigned char DeviatH;
	unsigned char DeviatL;
	unsigned char State;
*/

short Comp_execution_time( unsigned char* Source, unsigned char Type ,unsigned char checkOBISID )
{
	unsigned char Buff[12];
	unsigned char TBuff1[12];
	unsigned char Date_Buff[12];
	unsigned char* TPtr;   
	unsigned char* Ptr;
	unsigned char* Date;
	unsigned char* RealDate;	
	
	unsigned char Buff1[12];
	unsigned char* Ptr1;
	unsigned char  Flag;
	
//	Ptr = Buff;
//	Ptr1 = Buff1;
//	
//	RAM_Write( Ptr, &Clk.YearH, 9 );
//	Date = Source+4;
//	RealDate = Ptr;
	Ptr = Buff;
	Date =  TBuff1;
	TPtr = Date_Buff;
	
	Ptr1 = Buff1;
	
	RAM_Write( Ptr, &Clk.YearH, 12 );	  //比对日期yyyymmddWW+时间hh:mm	
	Date = Source+4;
	RealDate = Ptr;
	
	//结算日---停电后上电需要补..
	if((Type==1)&&(checkOBISID==0)){
			E2P_RData(  TPtr,Time_of_recent_billing_period, 12 );   //读上次结算日数据..
			if( (Data_CompHL( TPtr, RealDate, 4 ) == 1 )||((Data_CompHL( TPtr, RealDate, 4 ) == 0)&&(Data_CompHL( TPtr+5, RealDate+5, 2 ) == 1)))         //当前时间若小于上次结算时间
					{
						RAM_Write( TPtr, RealDate, 7 );				//当前时间若小于上次结算时间，则上次结算时间=当前时间
						E2P_WData( Time_of_recent_billing_period, RealDate, 12 );			//上次结算时间	
					}	
			if((Data_CompHL( TPtr, RealDate, 4 ) == 0)&&(Data_CompHL( TPtr+5, RealDate+5, 2 ) == 0)) return -1;							//若当前时间和上次结算时间为相同，则不结算
			
			switch( Type )
			{
				case 1:	
				case 3:	
				case 5:							//日期允许用通配符
					//if( DateComp( Date, RealDate ) != 0 ) return -1;			//只判断年月日
					  // 月冻结只允许年月用通配符或不用通配符..
						if(( *Date == 0xFF )&&( *(Date+1) == 0xFF )&&( *(Date+2) == 0xFF ))
						//if((( *Date == 0xFF )&&( *(Date+1) == 0xFF ))||( *(Date+2) == 0xFF ))
						{
								RAM_Write( Date, TPtr, 3 );
//								if(( *Date == 0xFF )&&( *(Date+1) == 0xFF )) RAM_Write( Date, TPtr, 2 );
//								if( *(Date+2) == 0xFF ) *(Date+2) = *(TPtr+2);
//								{
//									if(*(Date+2) != *(RealDate+2)) return -1;  //月份不在当月则不进行结算处理..									
//									//if((*(Date+3) < *(TPtr+3))||(( *(Date+3) == *(TPtr+3) )&&(Data_CompHL( Source, TPtr+5, 2 ) != 1))) IncHexYearMonth( Date+2 );	//若上次结算日与结算日相同，则测试时间加1个月。
//								}
								// 以下是：if(( *Date == 0xFF )&&( *(Date+1) == 0xFF )&&( *(Date+2) == 0xFF ))..		
								if((*(Date+3) < *(TPtr+3))||(( *(Date+3) == *(TPtr+3) )&&(Data_CompHL( Source, TPtr+5, 2 ) != 1))) IncHexYearMonth( Date+2 );	//若上次结算日与结算日相同，则测试时间加1个月。
								
								//if( *(Date+3) == *(TPtr+3) ) IncHexYearMonth( Date+3 );	//若上次结算日与结算日相同，则测试时间加1个月。
								if( (Data_CompHL( Date, RealDate, 4 ) == 1 )||((Data_CompHL( Date, RealDate, 4 ) == 0)&&(Data_CompHL( Source, RealDate+5, 2 ) == 1))) return -1;        //下次结算时间大于当前时间
								else
								{
	                 				 if( (Data_CompHL( Date, TPtr, 4 ) == 1 )||((Data_CompHL( Date, TPtr, 4 ) == 0)&&(Data_CompHL( Source, TPtr+5, 2 ) == 1))) return 0;        //下次结算时间大于上次结算时间..
										else return -1;			
								}		
						}
						else
						{
						   //if((Data_CompHL( TPtr, Date, 4 ) == 0)&&(Data_CompHL( TPtr+5, Source, 2 ) == 0)) return -1;							//若结算日时间和上次结算时间相同，不结算
						  if((Data_CompHL( TPtr, Date, 4 ) == 0)&&(Data_CompHL( TPtr+5, Source, 2 ) == 0)) return -1;
              if( (Data_CompHL( Date, RealDate, 4 ) == 1 )||((Data_CompHL( Date, RealDate, 4 ) == 0)&&(Data_CompHL( Source, RealDate+5, 2 ) == 1))) return -1;        //当前时间若小于上次结算时间
              if( (Data_CompHL( TPtr, RealDate, 4 ) == 1 )||((Data_CompHL( TPtr, RealDate, 4 ) == 0)&&(Data_CompHL( TPtr+5, RealDate+5, 2 ) == 1))) return 0;        //需要重新记录..
              if( (Data_CompHL( Date, TPtr, 4 ) == 1 )||((Data_CompHL( Date, TPtr, 4 ) == 0)&&(Data_CompHL( Source, TPtr+5, 2 ) == 1))) return 0;        //当前时间若小于上次结算时间
              return  -1;
						}								
					break;
				case 2:	
				case 4:							//日期不允许用通配符					
						  if((Data_CompHL( TPtr, Date, 4 ) == 0)&&(Data_CompHL( TPtr+5, Source, 2 ) == 0)) return -1;
              if( (Data_CompHL( Date, RealDate, 4 ) == 1 )||((Data_CompHL( Date, RealDate, 4 ) == 0)&&(Data_CompHL( Source, RealDate+5, 2 ) == 1))) return -1;        //当前时间若小于上次结算时间
              if( (Data_CompHL( TPtr, RealDate, 4 ) == 1 )||((Data_CompHL( TPtr, RealDate, 4 ) == 0)&&(Data_CompHL( TPtr+5, RealDate+5, 2 ) == 1))) return 0;        //需要重新记录..
              if( (Data_CompHL( Date, TPtr, 4 ) == 1 )||((Data_CompHL( Date, TPtr, 4 ) == 0)&&(Data_CompHL( Source, TPtr+5, 2 ) == 1))) return 0;        //当前时间若小于上次结算时间
              return  -1;
					break;
				default: 						
					break;						
			}					
		
		return -1;	
	//
	}
			
	//升级--立即升级判断.."FFFFFFF","FFFFFFFF" short IsAllData( unsigned char* Source, short Len, unsigned char Data )
	if((Type==1)&&(checkOBISID==3)){
			if(IsAllData(Source,9,0xff )==0) return 0;
			if(IsAllData(Source,9,0x00 )==0) return -1;
	}
	
	//停电后继电器拉合闸控制..
	if((checkOBISID==2)&&(crypt.CutPower == 0x0A)){
		//停电时间判断..
		 crypt.CutPower = 0x0;
		 if(IsAllData(Source,9,0xff )==0) return -1;
		 if(IsAllData(Source,9,0x00 )==0) return -1;    //非法错误时间..
		 	switch( Type )
			{
				case 1:	
				case 3:	
				case 5:							//日期允许用通配符
						if( *Date != 0xFF ) 
						{
						   if( *Date != *RealDate ) return 1;
						}												
					break;
				case 2:	
				case 4:							//日期不允许用通配符
					if( Data_Comp( Date, RealDate, 1 ) != 0 ) return -1;			
					break;
				//default: break;
					default: return -1;								
			}
			// 计算时间差异:
			RAM_Write( Ptr1, Source+4, 5 );	 //日期：XXXX年XX月XX日XX周
			RAM_Write( Ptr1+5, Source, 4 );  //时间：时分秒百秒
		 	{
	    		unsigned long Total_Sec1,Total_Sec2,Total_temp;
	    		unsigned char Buff_clk0[12];
	    		
	    		E2P_RData( Buff_clk0,Event_PowerDownRecord, 12 );		//..
	    		//先比较高位YY年份..
					if(Buff_clk0[0]!=Clk.YearH)	return -1;	
					if((Buff_clk0[1]==0xff)||(Buff_clk0[2]==0xff)||(Buff_clk0[3]==0xff)||(Buff_clk0[5]==0xff)||(Buff_clk0[6]==0xff)) return -1;
						
					Flag = 0;
					if((Type==1)||(Type==3)||(Type==5))
					{
							if(IsAllData(Ptr1,4,0xff )==0) Flag = 5;
					}
					if(Flag == 5)   //通配符..
					{
						  Total_Sec1 = (*(Ptr1+5))*60 + (*(Ptr1+6));
							Total_Sec2 = Clk.HourH*60 + Clk.MinH;
							if(Total_Sec1>Total_Sec2) return -1; //时间还没到..
							if(Total_Sec1==Total_Sec2) return 0; //时间正好到..								
							Total_temp = Buff_clk0[5]*60 + Buff_clk0[6];
							if(Total_Sec1>Total_temp) return 0; //时间已经过..							
				  }
				  else    //无通配符..
				  {
				  	if((*(Ptr1+1)==0xff)||(*(Ptr1+2)==0xff)||(*(Ptr1+3)==0xff)) return -1;
				  	
				  	Total_Sec1 = HexGetGmMin( Ptr1+1 );
						Total_Sec2 = HexGetGmMin( &Clk.YearL );
						if(Total_Sec1>Total_Sec2) return -1; //时间还没到..
						if(Total_Sec1==Total_Sec2) return 0; //时间正好到..				
						Total_temp =  HexGetGmMin( &Buff_clk0[1] );
						if(Total_Sec1>Total_temp) return 0; //时间已经过..
				  }

			}
			return -1;	 		
   }
	
	if( Data_Comp( Source, Ptr+5, 2 ) != 0 ) return -1;				//只判断时分		
	
	switch( Type )
	{
		case 1:	
		case 3:	
		case 5:							//日期允许用通配符
			if( DateComp( Date, RealDate ) != 0 ) return -1;			//只判断年月日															
			break;
		case 2:	
		case 4:							//日期不允许用通配符
			if( Data_Comp( Date, RealDate, 4 ) != 0 ) return -1;			
			break;
		//default: break;
			default: return -1;								
	}			
	return 0;
}	

void Exec_SingleAction( void )
{
	unsigned char script_logical_name[6];
	unsigned char execution_time[9];
	unsigned char Buff1[12];
	unsigned char* Ptr;
	short i,j;
	unsigned short Addr;
	unsigned short script_selector;		  	
	unsigned char Type;  					
	unsigned char execution_time_Number;  	
	
	Ptr = Buff1;
	
	for( i=0;i<DLMS_Sigle_actSum;i++ )
	{
		Addr = DLMS_Sigle_actTab[i].Addr;
		E2P_RData( script_logical_name, Addr, 6 );
		E2P_RData( Ptr, Addr+7, 2 );
		script_selector = GetBuffToInteger( Ptr );
		E2P_RData( (unsigned char*)&Type, Addr+7+3, 1 );
		E2P_RData( (unsigned char*)&execution_time_Number, Addr+7+3+2, 1 );
		//if( execution_time_Number > 4 ) execution_time_Number = 0;
		if( execution_time_Number > 5 ) execution_time_Number = 0;
		for( j=0;j<execution_time_Number;j++ )
		{
			E2P_RData( execution_time, Addr+7+3+2+2+10*j, 9 );	//Hex格式存储时间
			if( Comp_execution_time( execution_time, Type,i ) == 0 )
			{
			///	Exec_ScriptTable( script_logical_name, script_selector );
				Class9_ScriptTable( script_logical_name, script_selector );
			}	
		}	
	}
}	
