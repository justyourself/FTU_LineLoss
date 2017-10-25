#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Measure.h"
#include "Port.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"

#include "Mem.h"
#include "Time.h"
#include "Power.h"
#include "IECComm.h"
#include "ProfileGeneric.h"
#include "COSEMComm.h"

/*
#include "Initial.h"

#include "imagetranfer.h"
#include "RegisterActivation.h"
#include "ComSetup.h"
#include "ApplComm.h"
#include "AssociationLN.h"
#include "GetData.h"
#include "ScriptTable.h"
#include "ActCalendar.h"
#include "SingleAction.h"
#include "Limiter.h"
#include "RegisterMonitor.h"
#include "DisconnectControl.h"

*/

//开盖开端状态合法性判断
short IsAvailState( unsigned char State )														//10.11.05
{
	if(( State == 0 )||( State == OPEN_STATE )||( State == CLOSE_STATE )) return 0;
	else return 1;
}	

void GetCoverTerminalState( void )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
		
	Ptr = Buff;

	if( IsAvailState( SM.CoverOldState ) != 0 )											//10.11.05
	{																						//10.11.05	
		E2P_RData( Ptr, OpenCover_OldState, 1 );											//10.11.05
		if(( Flag.BatState & F_E2PCheck ) == 0 )											//10.11.05	
		{																					//10.11.05
			if( IsAvailState( *Ptr ) != 0 )	*Ptr = 0;										//10.11.05
		}																					//10.11.05
		SM.CoverOldState = *Ptr;																			//10.11.05	
	}																						//10.11.05			
	                                                                    					
	if( IsAvailState( SM.TerminalOldState ) != 0 )											//10.11.05
	{																						//10.11.05
		E2P_RData( Ptr, OpenTerminal_OldState, 1 );											//10.11.05	
		if(( Flag.BatState & F_E2PCheck ) == 0 )											//10.11.05	
		{																					//10.11.05
			if( IsAvailState( *Ptr ) != 0 )	*Ptr = 0;										//10.11.05
		}																					//10.11.05
		SM.TerminalOldState = *Ptr;															//10.11.05
	}																						//10.11.05
	
	if( IsAvailState( SM.CoverState ) != 0 ) SM.CoverState = SM.CoverOldState;				//10.11.05
	if( IsAvailState( SM.TerminalState ) != 0 ) SM.TerminalState = SM.TerminalOldState;		//10.11.05
	
	if( SM.CoverState != SM.CoverOldState )
	{
		if( SM.CoverState == OPEN_STATE )
		{
		///	if((Flag.Power & F_PwrUp) == 0 ) 	P8OUT &= ~P_EECTL;			//打开FLASH电源开关..
			E2P_WData( OpenCover_OldState, (unsigned char*)&SM.CoverState, 1 );								//10.11.05	

			SM.FraudDetectionCode = Meter_cover_removed;
			LoadRecord( Security_Event_No );
			SM.Event_all_flag |= Security_Event_BIT;
			SM.Error_all_flag |= Security_Event_BIT;
			SM.Profile1_status |=BIT0;
			SM.Profile2_status |=BIT0;
		}		
		else if( SM.CoverState == CLOSE_STATE )
		{
		///	if((Flag.Power & F_PwrUp) == 0 ) 	P8OUT &= ~P_EECTL;			//打开FLASH电源开关..
			E2P_WData( OpenCover_OldState, (unsigned char*)&SM.CoverState, 1 );								//10.11.05

			SM.FraudDetectionCode = Meter_cover_closed;
			LoadRecord( Security_Event_No );
			SM.Event_all_flag |= Security_Event_BIT;
			SM.Error_all_flag |= Security_Event_BIT;
		}			
	}	

	if( SM.TerminalState != SM.TerminalOldState )
	{
		if( SM.TerminalState == OPEN_STATE )
		{
		///	if((Flag.Power & F_PwrUp) == 0 ) 	P8OUT &= ~P_EECTL;			//打开FLASH电源开关..
			E2P_WData( OpenTerminal_OldState, (unsigned char*)&SM.TerminalState, 1 );						//10.11.05

			SM.FraudDetectionCode = Terminal_Cover_removed;
			LoadRecord( Security_Event_No );
			SM.Event_all_flag |= Security_Event_BIT;
			SM.Error_all_flag |= Security_Event_BIT;
			SM.Profile1_status |=BIT0;
			SM.Profile2_status |=BIT0;			
		}		
		else if( SM.TerminalState == CLOSE_STATE )
		{
	///		if((Flag.Power & F_PwrUp) == 0 ) 	P8OUT &= ~P_EECTL;			//打开FLASH电源开关..
			E2P_WData( OpenTerminal_OldState, (unsigned char*)&SM.TerminalState, 1 );						//10.11.05

			SM.FraudDetectionCode = Terminal_Cover_closed;
			LoadRecord( Security_Event_No );
			SM.Event_all_flag |= Security_Event_BIT;
			SM.Error_all_flag |= Security_Event_BIT;			
		}	
	}	
	
	SM.CoverOldState = SM.CoverState;
	SM.TerminalOldState = SM.TerminalState;
	
	if((Flag.Power & F_PwrUp) == 0) return;    //停电不执行操作..
	if( SM.ComModuleState != SM.ComModuleDis_State )
	{			
				//上次状态备份..
				E2P_RData( &SM.ComModuleDis_State,OpenComModule_OldState,  1 );						//15.07.31
		if( SM.ComModuleState != SM.ComModuleDis_State )
		{
			if( SM.ComModuleState == OPEN_STATE ) 
			{
				crypt.GPRS_STAND = 0x0c;   //打开..
				crypt.comm_online = 0;
		///		if((Flag.Power & F_PwrUp) == 0 ) 	P8OUT &= ~P_EECTL;			//打开FLASH电源开关..
				//SM.ComModuleState = 0;
				SM.ComModuleDis_State = OPEN_STATE;
				//上次状态备份..
				E2P_WData( OpenComModule_OldState, (unsigned char*)&SM.ComModuleDis_State, 1 );						//15.07.31
				SM.FraudDetectionCode = Communication_module_removed;
				LoadRecord( Security_Event_No );
				SM.Event_all_flag |= Security_Event_BIT;
				SM.Error_all_flag |= Security_Event_BIT;
				SM.Profile1_status |=BIT0;
				SM.Profile2_status |=BIT0;				
			}else if( SM.ComModuleState == CLOSE_STATE ) 	
			{
				crypt.GPRS_STAND = 0x0;   //闭合..
				crypt.comm_online = 0;
		///		if((Flag.Power & F_PwrUp) == 0 ) 	P8OUT &= ~P_EECTL;			//打开FLASH电源开关..
				//SM.ComModuleState = 0;
				SM.ComModuleDis_State = CLOSE_STATE;
				//上次状态备份..
				E2P_WData( OpenComModule_OldState, (unsigned char*)&SM.ComModuleDis_State, 1 );						//15.07.31				
				SM.FraudDetectionCode = Communication_module_insert;
				LoadRecord( Security_Event_No );
				SM.Event_all_flag |= Security_Event_BIT;
				SM.Error_all_flag |= Security_Event_BIT;				
			}
		}
	}	
	
	if( SM.MagneticState != SM.MagneticOldState )
	{
				//上次状态备份..
		E2P_RData( &SM.MagneticOldState,OpenMagnetic_OldState,  1 );						//15.07.31
		if(  SM.MagneticState != SM.MagneticOldState )
		{
		
			if( SM.MagneticState == OPEN_STATE ) 
			{
		///		if((Flag.Power & F_PwrUp) == 0 ) 	P8OUT &= ~P_EECTL;			//打开FLASH电源开关..
				//SM.MagneticState = 0;
				SM.MagneticOldState = OPEN_STATE;
					//上次状态备份..
					E2P_WData( OpenMagnetic_OldState, (unsigned char*)&SM.MagneticOldState, 1 );						//15.07.31
				
				SM.FraudDetectionCode = No_strong_DC_field_anymore;
				LoadRecord( Security_Event_No );	
				SM.Event_all_flag  |= Security_Event_BIT;
				SM.Error_all_flag  |= Security_Event_BIT;
				SM.Profile1_status |= BIT0;
				SM.Profile2_status |= BIT0;			
			}else if( SM.MagneticState == CLOSE_STATE ) 	
			{
		///		if((Flag.Power & F_PwrUp) == 0 ) 	P8OUT &= ~P_EECTL;			//打开FLASH电源开关..
				//SM.MagneticState = 0;
				SM.MagneticOldState = CLOSE_STATE;
				//上次状态备份..
				E2P_WData( OpenMagnetic_OldState, (unsigned char*)&SM.MagneticOldState, 1 );						//15.07.31
				SM.FraudDetectionCode = Strong_DC_field_detect;
				LoadRecord( Security_Event_No );
				SM.Event_all_flag |= Security_Event_BIT;
				SM.Error_all_flag |= Security_Event_BIT;				
			}
		}
	}	
}
