#include "TypeMeter.h"
#include "TypeE2p.h"
//#include "Measure.h"
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
//#include <RN8213.h>

//取Disconnect_control类数据
short GetDisconnect_controlClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x60;
	Buff[3] = 0x03;
	Buff[4] = 0x0A;
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
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Source+2, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_BOOLEAN;	
				/*
				if( Para.RMeterRelayState == 0x1A  ) *(Dest+2) = 0;		//已跳闸	TRUE
				else *(Dest+2) = 1;										//未跳闸	FLASE
				Len = 3;
				*/
				E2P_RData(&Para.RMeterRelayState,MeterRelayState,1);
				if(Para.RMeterRelayState==1)
					*(Dest+2) = 1;//TRUE = (BOOLEAN 1) Connected
				else
					*(Dest+2) = 0;//FALSE = (BOOLEAN 0) Disconnected
					Len = 3;
				break;	
			case 3:
				*(Dest+1) = D_ENUM;	
//				if( Para.RMeterTripState == 0x1A  ) *(Dest+2) = 0;		//已跳闸	(0) Disconnected,
//				else if( Para.RMeterTripState == 0x55  ) *(Dest+2) = 2;	//合闸允许  (2) Ready_for_reconnection
//				else *(Dest+2) = 1;										//已合闸    (1) Connected,                   

	/*			if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState != 0x55 ))
				{
					if( ICComm.RelayONState != 0xAA ) *(Dest+2) = 0;	//已跳闸	(0) Disconnected,
					else *(Dest+2) = 2;									//合闸允许  (2) Ready_for_reconnection
				}	
				else if( Para.RMeterRelayState != 0x1A )
				{
					*(Dest+2) = 1;										//已合闸    (1) Connected,
				}	
*/
				E2P_RData(Dest+2,MeterRelayState,1);
				Len = 3;                                                                           
				break;	                                                            
			case 4:
				*(Dest+1) = D_ENUM;	
				E2P_RData(&Para.RMeterControlMode,MeterControlMode,1);
				*(Dest+2) = Para.RMeterControlMode;
				Len = 3;
				break;	
			default: break;			
		}
	}		
	return Len;
}	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char SetDisconnect_controlClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x60;
	Buff[3] = 0x03;
	Buff[4] = 0x0A;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else if( *DataType != D_ENUM )
	{
		ErrorByte = type_unmatched; 									
	}	 
	else
	{
		switch( *Attribute )
		{
			case 4:
				if( *(WriteBufAds+1) > 6 )									
				{
					ErrorByte = other_reason;
					break;
				}
				Para.RMeterControlMode = *(WriteBufAds+1);
				E2P_WData( MeterControlMode, WriteBufAds+1, 1 );	
				break;	
			default: 
				ErrorByte = type_unmatched;			
				break;			
		}		
	}		
	return ErrorByte;
}		

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*
#if ( MagnKeepRelay	== YesCheck )
void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	ICComm.RelayDrivePulseWidth = 5;
	
	if( DriveType == RELAY_ON ) 
	{
		PDIR_JDQA |= P_JDQA; 
		PDIR_JDQB |= P_JDQB; 
		POUT_JDQA &= ~P_JDQA;  
		Delay(30);							//11.05.06
		POUT_JDQB |= P_JDQB;
		POUT_JDQA &= ~P_JDQA;    
	}	
	if( DriveType == RELAY_OFF ) 
	{
		PDIR_JDQA |= P_JDQA; 
		PDIR_JDQB |= P_JDQB; 
		POUT_JDQB &= ~P_JDQB; 	//11.05.06
		Delay(30);							//11.05.06
		POUT_JDQA |= P_JDQA;  
		POUT_JDQB &= ~P_JDQB;  
	}			
}	
#else
*/
void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	ICComm.RelayDrivePulseWidth = 5;
	
	if( DriveType == RELAY_ON ) 
	{
		HT_GPIOC->PTCLR |= P_RELAY1ON;
		HT_GPIOC->PTSET |= P_RELAY1OFF;
		
		HT_GPIOH->PTCLR |= P_RELAY2ON;
		HT_GPIOH->PTSET |= P_RELAY2OFF;
	}	
	if( DriveType == RELAY_OFF ) 
	{	
		HT_GPIOC->PTCLR |= P_RELAY1OFF;
		HT_GPIOC->PTSET |= P_RELAY1ON;
		
		HT_GPIOH->PTCLR |= P_RELAY2OFF;
		HT_GPIOH->PTSET |= P_RELAY2ON;
	}			
}

/*
{
	ICComm.RelayDriveType = DriveType;
	if(( Para.RMeterRunMode1 & 0x01 ) != 0 ) ICComm.RelayDrivePulseWidth = 192;		//脉宽3S
	else ICComm.RelayDrivePulseWidth = 0;
	
//	if( DriveType == RELAY_ON ) POUT_JDQA &= ~P_JDQA;  			//11.03.29
//	if( DriveType == RELAY_OFF ) POUT_JDQA |= P_JDQA;  			//11.03.29
	if( DriveType == RELAY_ON ) 
		{
		POUT_RELAY1ON|=P_RELAY1ON;///POUT_RELBJ &= ~P_RELBJ;  			//11.03.29
		POUT_RELAY1OFF&=~P_RELAY1OFF;
		}
	if( DriveType == RELAY_OFF )
		{
		POUT_RELAY1OFF|=P_RELAY1OFF;/// POUT_RELBJ |= P_RELBJ;  			//11.03.29
		POUT_RELAY1ON&=~P_RELAY1ON;
		}
}
*/


///#endif

void OperateRelay( void )
{
	if( Para.RMeterTripState == 0x1A )	//远程跳闸..
	{			
		if( Para.RMeterRelayState != 0x1A ) ICComm.RelayOFFState = 0x55;//准备跳闸..																											
		ICComm.RelayONState = 0x0;
	}	

	if(( Para.RMeterRelayState == 0x1A )&&( Para.RMeterTripState == 0x1B ))
	{
		if( ICComm.RelayONState	== 0 )
		{
//			if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x05 )||( Para.RMeterControlMode == 0x06 ))
			if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x02 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x04 )||( Para.RMeterControlMode == 0x05 )||( Para.RMeterControlMode == 0x06 ))   //SKY20140218..
			{
				if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0xAA;	//允许合闸(可能刚上电)..
			}
			else if( Para.RMeterControlMode == 0x00 )   //在模式4的基础上增加当前和手动的设置内容..//SKY20140218..
//			else if(( Para.RMeterControlMode == 0x00 )||( Para.RMeterControlMode == 0x02 )||( Para.RMeterControlMode == 0x04 ))     
			{
				if( Para.RMeterRelayState == 0x1A ) 
				{
					ICComm.RelayONState = 0x55;	//立即合闸(可能刚上电).. 			
				}	
			}
		}		
	}	

	if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 ))	//执行合闸..		
	{
		limiter.LAST_OVER_Event.byte = 0;    //sky2013.12.24..
		ICComm.RelayDriveCnt = 5;	
		RelayDrive( RELAY_ON );	
		ICComm.RelayONState = 0x0;
		Para.RMeterRelayState = 0x55;			//已经合闸..
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
		if( SM.DisconnectorControlCode == Manual_connection ){
			 LoadRecord( Disconnector_Control_No );
			 	SM.Event_all_flag |= Disconnector_Control_BIT;
			// 清除需量事件判断标志..
			Clr_local_control_para_monitor();			 	
		}
	}				

	if(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))	//执行跳闸..		
	{
		ICComm.RelayDriveCnt = 5;	
		RelayDrive( RELAY_OFF );	
		ICComm.RelayOFFState = 0x0;
		Para.RMeterRelayState = 0x1A;			//已经跳闸..
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
	}				
}		

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
unsigned char ActionDisconnect_controlClassData( unsigned char *Source, unsigned short DataLen )
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
	Buff[2] = 0x60;
	Buff[3] = 0x03;
	Buff[4] = 0x0A;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		return object_undefined; 									//无对应的OBIS码..
	}	
	
//	兼容..
//	if(*(Source+9)==0x01)
//   {
//     if((*(Source+10)!=0x0f)&&(*(Source+11)!=0))  return other_reason;	
//   }
		
	if( Method == 1 )		//跳闸..
	{
		if( Para.RMeterControlMode > 0 )
		{
			Para.RMeterTripState=Route_B|Route_C;
			// 清除需量事件判断标志..
			Clr_local_control_para_monitor();
		}else ErrorByte = other_reason;							
	}
	else if( Method == 2 )
	{
		if((Para.RMeterControlMode==2)||(Para.RMeterControlMode==4))
			Para.RMeterTripState=Route_A;
		else
			Para.RMeterTripState=Route_D;

			// 清除需量事件判断标志..
			Clr_local_control_para_monitor();		
	}	
	else ErrorByte = other_reason;										

	return ErrorByte;
}	

//清除状态标志事件记录..
void Clr_local_control_para_monitor( void )
{
	// limiter.Limiter_RelayMode = 0; //..
	//limiter.Ram_Limiter_threshold_active = 0xFFFFFFFF;
	limiter.Threshold_MDEvent.byte = 0;
	limiter.flg_Threshold_MDEvent1.byte =0;
	limiter.flg_Threshold_MDEvent1BK.byte =0;
	
  limiter.min_under_threshold_duration_cnt = 0;
  limiter.Ram_Reconnection_timeout_emergency_cnt=0;
  
   limiter.Threshold_MDEvent.byte = 0;  
 	 limiter.over_threshold_duration_cnt = 0;
 	
 	limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt =0;
 	limiter.Ram_Auto_Reclosing_relay_Num_cnt =0; 

}
////------------------------------------------------------------------------------
////** void  local_control_para_monitor( void )
////** DESCRIPTION: 实时参数检测,每秒钟调用一次..
////** RETURNS: None..
////------------------------------------------------------------------------------
//void  local_control_para_monitor( void )
//{
//	BYTE_STRUCT   stateEvent1, stateEvent2;		//暂时先用第1位,预留..
//	unsigned char *Ptr_num_times;
//  unsigned char Threshod_num_times;
//	unsigned short *Ptr_time;
//	unsigned short Threshod_time;
//	unsigned char FLAG = 0;
//	 	 
//	//需量越限判断..
//	
//	Ave_Power();
//	if(limiter.Limiter_RelayMode == 0) return;		//还没判模式(刚上电)..
//	if(limiter.Limiter_RelayMode == 0xB2)					//紧急..
//	{	
//			Ptr_num_times = &limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt;
//			Threshod_num_times = limiter.Ram_Auto_Reclosing_relay_Num_emergency;
//			Ptr_time = &limiter.Ram_Reconnection_timeout_emergency_cnt;
//			Threshod_time = limiter.Ram_Reconnection_timeout_emergency;
//			//limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;
//	}else{
//	//				if((limiter.flg_Threshold_MDEvent1.byte&BIT0)&&(limiter.LastTime_Limiter_RelayMode == 0xB2))
//						
////									Ptr_time = &limiter.Ram_Reconnection_timeout_emergency_cnt;
////									Threshod_time = limiter.Ram_Reconnection_timeout_emergency;
////				 }else{
//				 					Ptr_num_times = &limiter.Ram_Auto_Reclosing_relay_Num_cnt;		//次数..
//				 					Threshod_num_times = limiter.Ram_Auto_Reclosing_relay_Num_Normal;  //次数判断值
//				 					Ptr_time = &limiter.Ram_Reconnection_timeout_Normal_cnt;
//									Threshod_time = limiter.Ram_Reconnection_timeout_Normal;
//									// limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;	//上次模式保存.																		
////							}
//			}
//			
//					
//		//次数判断..
//		if((*Ptr_num_times)<=Threshod_num_times)  //未超过次数
//		{		
//				if(limiter.Ram_Limiter_monitored_value>limiter.Ram_Limiter_threshold_active)
//					{
//						  if( limiter.over_threshold_duration_cnt<limiter.Ram_Limiter_min_over_threshold_duration ){
//						      limiter.over_threshold_duration_cnt++;				      
//						  }else{
//						      	limiter.flg_Threshold_MDEvent1.byte |= BIT0;
//						       }
//						   (*Ptr_time) = 0;				 				  
//						  limiter.min_under_threshold_duration_cnt = 0;
//					}else{
//						 	  //判断越限..
//						 	   if( limiter.over_threshold_duration_cnt>0)
//						 	   	      limiter.over_threshold_duration_cnt--;				      
//						     else{
//						      		 limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;		//清零..
//						      	}
//						      	
//						        	//再判断一个越限状态保持时间后才允许合闸..
//										  if(*Ptr_time<Threshod_time){
//										  	  (*Ptr_time) += 1;
//										  	  // FLAG = 0;
//										  }
//						      	
//						 	   if( limiter.min_under_threshold_duration_cnt<limiter.Ram_Limiter_min_under_threshold_duration ){
//								      limiter.min_under_threshold_duration_cnt++;				      						      
//								  }else{
//								  			  limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;		//清零..
//								  			 // *Ptr_num_times = 0;
//								  			  limiter.Ram_Auto_Reclosing_relay_Num_cnt =0;
//								  			  limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt =0;		
//		 						       }
//		           // limiter.over_threshold_duration_cnt = 0;  
//					    }
//						//------------------------------------------------------
//						//检测有无需量越限..
//					  stateEvent1.byte = limiter.flg_Threshold_MDEvent1.byte&BIT0;
//					  stateEvent2.byte = limiter.flg_Threshold_MDEvent1BK.byte&BIT0;
//						limiter.flg_Threshold_MDEvent1BK.byte = limiter.flg_Threshold_MDEvent1.byte;
//					//    record_eventFlg_MD( stateEvent1,stateEvent2,&quality_event.Threshold_MDEvent );	  
//					  if( stateEvent1.byte!=stateEvent2.byte )
//					  	{		  	
//								 if( stateEvent1.bit.b0!=stateEvent2.bit.b0 ){
//									  if( stateEvent1.bit.b0==1 ){				//检测到A相事件发生开始..
//									      if( limiter.Threshold_MDEvent.bit.b4==1 ){
//									          limiter.Threshold_MDEvent.bit.b0 = 0;
//									          limiter.Threshold_MDEvent.bit.b4 = 0;
//									      }
//									      else
//									          limiter.Threshold_MDEvent.bit.b0 = 1;
//									  }
//									  else{//检测到A相事件发生结束..
//									      if( limiter.Threshold_MDEvent.bit.b0==1 ){
//									          limiter.Threshold_MDEvent.bit.b0 = 0;
//									          limiter.Threshold_MDEvent.bit.b4 = 0;
//									      }
//									      else
//									          limiter.Threshold_MDEvent.bit.b4 = 1;
//									  }
//									}
//							}	
//	}else{
//					if(limiter.Limiter_RelayMode == 0xB2)		limiter.Threshold_MDEvent.bit.b0 = 1;			//紧急,跳闸..
//					else 	limiter.Threshold_MDEvent.bit.b4 = 1;			//合闸..
//				}	
//}


//------------------------------------------------------------------------------
//** void Relay_localControl_Event( void )  //放在每秒处理的任务中..
//** DESCRIPTION: 判断事件的发生并记录数据..
//** RETURNS: None..
//------------------------------------------------------------------------------

void Relay_localControl_Event( void )
{	
  	BYTE_STRUCT 	*eventFlg_Ptr;			  
		BYTE_STRUCT   stateEvent1, stateEvent2;		//暂时先用第1位,预留..
		unsigned char *Ptr_num_times;
  	unsigned char Threshod_num_times;
		unsigned short 	*Ptr_time;
		unsigned short 	Threshod_time;
		
	 
	  //前10秒内不做判断..
	  // quality_event.Deal_EVENT_NO ++;  	//此标志在电能质量事件中增加..
	  if(quality_event.Deal_EVENT_NO<10)  return;
	  	
		//每秒执行一次	 	 
		//需量越限判断..
	
	Ave_Power();
	if(limiter.Limiter_RelayMode == 0) return;		//还没判模式(刚上电)..
	
		if(limiter.LastTime_Limiter_RelayMode!=limiter.Limiter_RelayMode){	//模式切换
//				*Ptr_num_times = 0;
//				*Ptr_time = 0;
//				limiter.over_threshold_duration_cnt =0;
//				limiter.min_under_threshold_duration_cnt = 0;
//			//	if(limiter.LAST_OVER_Event.byte == 0x11) limiter.flg_Threshold_MDEvent1.byte |= BIT0;
//			//	else limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;
				//紧急模式到普通模式，则先进行合闸操作..
				if((limiter.LastTime_Limiter_RelayMode ==0xb2)&&( limiter.Limiter_RelayMode!=0xb2)){
						if( Para.RMeterRelayState == 0x1A ){
								if(limiter.LAST_OVER_Event.byte == 0x11){
											limiter.LAST_OVER_Event.byte = 0;
											limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt = 0;
									 		localControl_EventEnd();
									}
						}
				} 
//					 	// 清除需量事件判断标志..
						Clr_local_control_para_monitor();									
//				}				
		// 模式变化..
		limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;
	}
	
	if(limiter.Limiter_RelayMode == 0xB2)					//紧急..
	{	
			Ptr_num_times = &limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt;
			Threshod_num_times = limiter.Ram_Auto_Reclosing_relay_Num_emergency;
			Ptr_time = &limiter.Ram_Reconnection_timeout_emergency_cnt;
			Threshod_time = limiter.Ram_Reconnection_timeout_emergency;
			//limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;
	}else{
	//				if((limiter.flg_Threshold_MDEvent1.byte&BIT0)&&(limiter.LastTime_Limiter_RelayMode == 0xB2))
						
//									Ptr_time = &limiter.Ram_Reconnection_timeout_emergency_cnt;
//									Threshod_time = limiter.Ram_Reconnection_timeout_emergency;
//				 }else{
				 					Ptr_num_times = &limiter.Ram_Auto_Reclosing_relay_Num_cnt;		//次数..
				 					Threshod_num_times = limiter.Ram_Auto_Reclosing_relay_Num_Normal;  //次数判断值
				 					Ptr_time = &limiter.Ram_Reconnection_timeout_Normal_cnt;
									Threshod_time = limiter.Ram_Reconnection_timeout_Normal;
									// limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;	//上次模式保存.																		
//							}
			}

					
//		//次数判断..
//		if((*Ptr_num_times)<Threshod_num_times)  //未超过次数
//		{		
				if(limiter.Ram_Limiter_monitored_value>limiter.Ram_Limiter_threshold_active)
					{
						  if( limiter.over_threshold_duration_cnt<limiter.Ram_Limiter_min_over_threshold_duration ){
						      limiter.over_threshold_duration_cnt++;				      
						  }else{
						      	limiter.flg_Threshold_MDEvent1.byte |= BIT0;
						       }
						   (*Ptr_time) = 0;				 				  
						  limiter.min_under_threshold_duration_cnt = 0;
					}else{
						 	  //判断越限..
						 	   if( limiter.over_threshold_duration_cnt>0)
						 	   	      limiter.over_threshold_duration_cnt--;				      
						     else{
						      		 limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;		//清零..
						      	}
						      	
						   //再判断一个越限状态保持时间后才允许合闸..
						   if(limiter.LAST_OVER_Event.byte == 0x11)
						    { 
									  if(*Ptr_time<Threshod_time){
									  	  (*Ptr_time) += 1;										 
									  }else *Ptr_time = Threshod_time;
						   	}else{ 
						   				//		//次数判断..
											if((*Ptr_num_times)<Threshod_num_times)  //未超过次数
											{								      		
												 	   if( limiter.min_under_threshold_duration_cnt<limiter.Ram_Limiter_min_under_threshold_duration ){
														      limiter.min_under_threshold_duration_cnt++;				      						      
														  }else{
														  			  limiter.flg_Threshold_MDEvent1.byte &= ~BIT0;		//清零..
														  			 // *Ptr_num_times = 0;
														  			  limiter.Ram_Auto_Reclosing_relay_Num_cnt =0;
														  			  limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt =0;		
								 						       }
					 						  }
		 						   	}
		           // limiter.over_threshold_duration_cnt = 0;  
					    }
						//------------------------------------------------------
						//检测有无需量越限..
					  stateEvent1.byte = limiter.flg_Threshold_MDEvent1.byte&BIT0;
					  stateEvent2.byte = limiter.flg_Threshold_MDEvent1BK.byte&BIT0;
						limiter.flg_Threshold_MDEvent1BK.byte = limiter.flg_Threshold_MDEvent1.byte;
					//    record_eventFlg_MD( stateEvent1,stateEvent2,&quality_event.Threshold_MDEvent );	  
					  if( stateEvent1.byte!=stateEvent2.byte )
					  	{		  	
								 if( stateEvent1.bit.b0!=stateEvent2.bit.b0 ){
									  if( stateEvent1.bit.b0==1 ){				//检测到事件发生开始..
									      if( limiter.Threshold_MDEvent.bit.b4==1 ){
									          limiter.Threshold_MDEvent.bit.b0 = 0;
									          limiter.Threshold_MDEvent.bit.b4 = 0;
									      }
									      else
									          limiter.Threshold_MDEvent.bit.b0 = 1;
									  }
									  else{//检测到事件发生结束..
									      if( limiter.Threshold_MDEvent.bit.b0==1 ){
									          limiter.Threshold_MDEvent.bit.b0 = 0;
									          limiter.Threshold_MDEvent.bit.b4 = 0;
									      }
									      else
									          limiter.Threshold_MDEvent.bit.b4 = 1;
									  }
									}
							}	
//	}else{
//					if(limiter.Limiter_RelayMode == 0xB2)		limiter.Threshold_MDEvent.bit.b0 = 1;			//紧急,跳闸..
//					else 	limiter.Threshold_MDEvent.bit.b4 = 1;			//合闸..
//				}	
	   
	  eventFlg_Ptr = &limiter.Threshold_MDEvent;

	//----------------------------------------------------------------------------
	if( eventFlg_Ptr->bit.b4==1 ){     //如果结束事件发生(合闸)..
			//		//次数判断..
		if((*Ptr_num_times)<Threshod_num_times)  //未超过次数
			{			   	
			  	if(*Ptr_time==Threshod_time){
						 (*Ptr_time) = 0;										 
						 (*Ptr_num_times)++;		//次数..				  	
		        localControl_EventEnd( );
		        eventFlg_Ptr->bit.b4 = 0;
		      }
		 }else{		//次数超的情况..
		 					if(limiter.Limiter_RelayMode != 0xB2){
		 						 if(*Ptr_time==Threshod_time){
									 (*Ptr_time) = 0;										 
									 (*Ptr_num_times)++;		//次数..				  	
					         localControl_EventEnd( );
					         eventFlg_Ptr->bit.b4 = 0;
					       }		 						
		 					}	
					}
	}
	else if( eventFlg_Ptr->bit.b0==1 ){//如果开始事件发生(拉闸)..
				//		//次数判断..
		if((*Ptr_num_times)<Threshod_num_times)  //未超过次数
			{	
					*Ptr_time = 0;   
		      localControl_EventStart();
		      eventFlg_Ptr->bit.b0 = 0;
      }else{		//次数超
      		 if(limiter.Limiter_RelayMode == 0xB2){  //紧急模式--跳闸..
      		 		*Ptr_time = 0;   
				      localControl_EventStart();
				      eventFlg_Ptr->bit.b0 = 0;      		 	
      		 }
      }
	}		
}


void localControl_EventStart( void )
{
//	unsigned char Buff[15];
//	unsigned char* Ptr;
//	
//	Ptr = Buff;

		limiter.LAST_OVER_Event.byte = 0x11;
		
		//limiter.LastTime_Limiter_RelayMode = limiter.Limiter_RelayMode;

		//本地拉闸..
		if( Para.RMeterControlMode > 0 )
		{
		/*
			if(( Para.RMeterTripState == 0x1A )&&( Para.RMeterRelayState == 0x1A )) return; 	//远程跳闸且已跳闸，该命令可忽略。
			else if( Para.RMeterRelayState != 0x1A )
			{
				ICComm.RelayOFFState = 0x55;									//准备跳闸
				Para.RMeterTripState = 0x1A;				
				// E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
				
				SM.DisconnectorControlCode = Limiter_threshold_exceed;
				//else 									
				LoadRecord( Disconnector_Control_No );
				OperateRelay();
				SM.Event_all_flag |= Disconnector_Control_BIT;
				SM.Error_all_flag |= Disconnector_Control_BIT;
			
				ICComm.RelayONState = 0xAA;	
			
				E2P_RData( &Para.RMeterTripState,MeterTripState,  1 );		
				if(Para.RMeterTripState != 0x1B){
						Para.RMeterTripState = 0x1B;
						E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
				 }				
			}		
			*/			
			Para.RMeterTripState = Route_G; 
		}
	
}

void localControl_EventEnd( void )
{
//	unsigned char Buff[15];
//	unsigned char* Ptr;
//	
//	Ptr = Buff;
	
/*		//增加次数限制..
	if(limiter.Limiter_RelayMode == 0xB2)		//紧急模式..
		{
				if( limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt<limiter.Ram_Auto_Reclosing_relay_Num_emergency ){
						//limiter.Ram_Auto_Reclosing_relay_Num_emergency_cnt++;				      
				 }else{
								//if(limiter.Ram_Auto_Reclosing_relay_Num_cnt)
								//需要处于跳闸状态不合闸..
								return;
					 	  }
		}else{		//普通模式..
						if( limiter.Ram_Auto_Reclosing_relay_Num_cnt<limiter.Ram_Reconnection_timeout_Normal ){
									//limiter.Ram_Auto_Reclosing_relay_Num_cnt++;
									_NOP();				      
				 		}else{
								//if(limiter.Ram_Auto_Reclosing_relay_Num_cnt)
								//次数到需要一直处于合闸状态，等待主站发跳闸命令..								
								// return;
					 	  }
					}
	*/
	
	limiter.LAST_OVER_Event.byte = 0x22;
	if( Para.RMeterControlMode > 4 )
		Para.RMeterTripState = Route_H;

/*				
//	if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x05 )||( Para.RMeterControlMode == 0x06 ))
	if(( Para.RMeterControlMode == 0x01 )||( Para.RMeterControlMode == 0x03 )||( Para.RMeterControlMode == 0x05 ))
		{
			if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0xAA;	//允许合闸
		}
		else if(( Para.RMeterControlMode == 0x02 )||( Para.RMeterControlMode == 0x04 )||( Para.RMeterControlMode == 0x06 ))
		{
//			if(( Para.RMeterRelayState == 0x1A )&&(Para.RMeterTripState != 0x1A)) 
		if( Para.RMeterRelayState == 0x1A ) 
			{
				ICComm.RelayONState = 0x55;	//立即合闸 			
				SM.DisconnectorControlCode = Limiter_threshold_OK;
				LoadRecord( Disconnector_Control_No );
				OperateRelay();
				SM.Event_all_flag |= Disconnector_Control_BIT;
				SM.Error_all_flag |= Disconnector_Control_BIT;
			}	
		}	
		*/
//		Para.RMeterTripState = 0x1B;
//		E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
//			E2P_RData( &Para.RMeterTripState,MeterTripState,  1 );		
//			if(Para.RMeterTripState != 0x1B){
//					Para.RMeterTripState = 0x1B;
//					E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
//			}								
}
void Relay_Ctr(void)
{
		if(Para.RMeterControlMode==1)
			Para.RMeterTripState&=0x7e; //bcfgde
		else if(Para.RMeterControlMode==2)
			Para.RMeterTripState&=0xee; //bcfgae
		else if(Para.RMeterControlMode==3)
			Para.RMeterTripState&=0x7a; //bcgde
		else if(Para.RMeterControlMode==4)
			Para.RMeterTripState&=0xea; //bcgae
		else if(Para.RMeterControlMode==5)
			Para.RMeterTripState&=0x7f; //bcfgdeh
		else if(Para.RMeterControlMode==6)
			Para.RMeterTripState&=0x7b; //bcgdeh
		else{
				if(Para.RMeterRelayState!=1)
				{
				RelayDrive( RELAY_ON );//继电器合闸
				Para.RMeterRelayState=1;
				E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
				 SM.DisconnectorControlCode = Local_reconnection;
				LoadRecord( Disconnector_Control_No );
				}
			}
	if(Para.RMeterControlMode==0) return;
	if(Para.RMeterRelayState==0)//Disconected
	{
		if(Para.RMeterTripState&Route_A)
		{
		RelayDrive( RELAY_ON );//继电器合闸
		Para.RMeterRelayState=1;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Remote_connection;
		LoadRecord( Disconnector_Control_No );
		}
		else if(Para.RMeterTripState&Route_D)
		{	
		Para.RMeterRelayState=2;
		SM.DisconnectorControlCode = Remote_connection;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		}
	}
	else if(Para.RMeterRelayState==1)//Connected
	{
		if(Para.RMeterTripState&Route_B)
		{
		RelayDrive( RELAY_OFF );//继电器跳闸
		Para.RMeterRelayState=0;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Remote_disconnection;
		LoadRecord( Disconnector_Control_No );
		}
		else if(Para.RMeterTripState&Route_F)
		{
		RelayDrive( RELAY_OFF );//继电器跳闸
		Para.RMeterRelayState=2;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Manual_disconnection;
		LoadRecord( Disconnector_Control_No );
		}
		else if(Para.RMeterTripState&Route_G)
		{
		RelayDrive( RELAY_OFF );//继电器跳闸
		Para.RMeterRelayState=2;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Local_disconnection;
		LoadRecord( Disconnector_Control_No );
		}
	}
	else if(Para.RMeterRelayState==2)//Ready for reconnection
	{
		if(Para.RMeterTripState&Route_C)
		{
		Para.RMeterRelayState=0;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		}
		else if(Para.RMeterTripState&Route_E)
		{
		RelayDrive( RELAY_ON );//继电器合闸
		Para.RMeterRelayState=1;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Manual_connection;
		LoadRecord( Disconnector_Control_No );
		}
		else if(Para.RMeterTripState&Route_H)
		{
		RelayDrive( RELAY_ON );//继电器合闸
		Para.RMeterRelayState=1;
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );
		SM.DisconnectorControlCode = Local_reconnection;
		LoadRecord( Disconnector_Control_No );
		}
	}
	Para.RMeterTripState=0;

}

