#include <math.h>
#include "TypeMeter.h"
//#include <stdlib.h>
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
//#include "In430.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Port.h"
#include "TypeFRAM.h"
#include "TypeFlash.h"
#include "Data.h"
#include "Interrupt.h"
#include "General.h"
#include "Time.h"
#include "Mem.h"
#include "Power.h"
#include "RsComm.h"
#include "Display.h"
//#include "LoadEven.h"
#include "Initial.h"
#include "Measure.h"
//#include "FreezeData.h"
//#include "Load.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
//#include "cpucard.h"
//#include "RemoteComm.h"
#include "ECProcess.h"
#include "Hardware.h"

void HardwareChk( void )
{
/*
#if ( InfraredCarry38k == YesCheck )		//15.01.22
	P2OUT &= ~P_WP;		
	P2DIR |= P_WP + P_HW38K;		
	P2DIR &= ~(P_YG + P_WG);		
	P2SEL = P_HW38K;
#else
	P2OUT &= ~P_WP;		
	P2DIR |= P_WP;		
	P2DIR &= ~(P_YG + P_WG);		
	P2SEL = 0x00;
#endif

	P3DIR |= P_TXDZB;
	P3SEL = 0x30;
	
	P4SEL = 0x00;
    
	P5DIR |= P_TXD4851+0x01;
	P5SEL = 0xC0;
	
	P7SEL = 0x03;

	P8SEL = 0x00;

	P9DIR |= P_TXD4852;
	P9SEL = 0x30;

	P10SEL = 0x00;

	P11SEL = 0x00;

	if(( Comm.TX_Delay1 == 0 )&&( Comm.BTime1 == 0 )&&(( UCA1IE & UCRXIE ) == 0 )) 							
	{
  		_DINT();
		CommRest(1);
		_EINT();	
	}

	if(( Comm.TX_Delay2 == 0 )&&( Comm.BTime2 == 0 )&&(( UCA2IE & UCRXIE ) == 0 )) 							
	{
  		_DINT();
		CommRest(2);
		_EINT();	
	}

	if(( Comm.TX_Delay3 == 0 )&&( Comm.BTime3 == 0 )&&(( UCA0IE & UCRXIE ) == 0 )) 							
	{
  		_DINT();
		CommRest(3);
		_EINT();	
	}
*/	
}	