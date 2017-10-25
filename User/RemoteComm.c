#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
//#include "In430.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Port.h"
#include "TypeFRAM.h"
#include "RsComm.h"
#include "Data.h"
#include "interrupt.h"
#include "Mem.h"
#include "Measure.h"
#include "General.h" 
#include "Time.h"
#include "Power.h"
#include "Display.h"
#include "FreezeData.h"
//#include "Load.h"
#include "cpucard.h"
#include "RemoteComm.h"


void AddIdentityFaultCnt( void )					
{
	unsigned char Temp;															//新国网	//13.09.16
	
	Temp = ICEC.IdentityMACFaultCnt + 0xA5;										//新国网	//13.09.16
	if( ICEC.IdentityMACFaultCntChk != Temp )									//新国网	//13.09.16
	{
		ICEC.IdentityMACFaultCnt = 0;
		ICEC.IdentityMACFaultCntChk = 0xA5;
	}	
	
	if( ICEC.IdentityMACFaultCnt < 200 ) 
	{
		ICEC.IdentityMACFaultCnt++;
		ICEC.IdentityMACFaultCntChk = ICEC.IdentityMACFaultCnt + 0xA5;
	}	
	if( ICEC.IdentityMACFaultCnt >= 200 ) 
	{
		ICEC.IdentityState = 0x55;	
		E2P_WData( EIdentityState, (unsigned char* )&ICEC.IdentityState, 1 );	
	}
//	RemoteFaultDisplay( ICALARM_10 );		
}	


void InitFaultCnt( void )
{
//	if(((PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))			//新国网	//13.10.09	//14.04.09
	if(((HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))			//新国网	//13.10.09	//14.04.09
	{											//新国网	//13.10.09
		ICEC.IdentityMACFaultCnt = 0;			
		ICEC.IdentityMACFaultCntChk = 0xA5;		
	}											//新国网	//13.10.09			
	ICEC.IdentityFaultCnt = 0;
	ICEC.IdentityFaultCntChk = 0xA5;	
}	

int GetIdentityValidState( void )						
{
	if( ICEC.IdentityValidDelayChk == ChkNum( (unsigned char*)&ICEC.IdentityValidDelay, 2 ) )
	{
		if( ICEC.IdentityValidDelay != 0 ) return 0;			
	}
	else
	{
		ICEC.IdentityValidDelay = 0;
		ICEC.IdentityValidDelayChk = 0xA5;
	}			
	return 1;
}	