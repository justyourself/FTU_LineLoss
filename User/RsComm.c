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
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
#include "RemoteComm.h"
#include "cpucard.h"
#include "Produce.h"
#include "TypeFlash.h"					//10.11.02
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif
#if ( MEASCHIP == ATT7022E )
#include "ATT7022E.h"					
#endif
//#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
//#include "NewLoad.h"
//#endif
#if ( AdjustErrorCheck == YesCheck )	//15.09.05
#include "AdjustError.h"
#endif
/*
short GetBandRate( unsigned char BandRate )
{
	short i;
	
//	for( i=0;i<6;i++ )
	for( i=0;i<7;i++ )
	{
//		if(( BandRate & ByteBit[i+1] )!= 0 ) return i*3;		
		if(( BandRate & ByteBit[i] )!= 0 ) return i*3;		
	}			
	return 2*3;
}

unsigned char GetTXDelay( unsigned char* Source )
{
	unsigned char Temp;
	unsigned char TXDelay;
	
	Temp = *Source;
	
	switch( Temp )
	{
		case Baut_600H:		TXDelay = 96;	break;	
		case Baut_1200H:	TXDelay = 48; 	break;	
		case Baut_4800H:	TXDelay = 13;	break;	
		case Baut_9600H:	TXDelay = 7;	break;	
		case Baut_19200H:	TXDelay = 4;	break;	
		default: 			TXDelay = 24; 	break;				
	}		
	return TXDelay;
}	

//COM_No == 0 : 红外
//COM_No == 0 : 485口1
//COM_No == 0 : 485口2
void CommInit( short COM_No )
{
	short Temp;
	switch( COM_No )
	{
		case 0: 
//				if( Comm.TX_Delay0 == 0 ) 
//				{	
					Comm.Buf0[0] = 0xff;
					Comm.Mode0 = 1;
					Comm.Ptr0 = 0;
					Comm.BTime0 = 0;					// 字符间定时
					Comm.TX_FESum0 = 0;
//					UCA0STAT = 0;
//					UCA0BR0 = Baut_1200L;
//					UCA0BR1 = Baut_1200H;
//					UCA0MCTL = Baut_1200M;
//					UCA0IE = UCRXIE;
					Comm.Run &= ~FComOK0;
			        HwRxInitial();	
//    			}
				break;
		case 1: if( Comm.TX_Delay1 == 0 ) 
				{	
                    PDIR_SW4851 &= ~P_SW4851;			//接收状态
					Comm.Buf1[0] = 0xff;
					Comm.Mode1 = 1;
					Comm.Ptr1 = 0;
					Comm.BTime1 = 0;					// 字符间定时
					Comm.TX_FESum1 = 0;
					UCA1STAT = 0;
//					if( Comm.BandRateMode == 1 )		//波特率自适应
//					{
//						UCA1CTL1 |= UCRXEIE + UCBRKIE;
//						UCA1BR0 = Baut_9600L;
//						UCA1BR1 = Baut_9600H;
//						UCA1MCTL = Baut_9600M;	
//					}
//					else 
//					{
//						UCA1CTL1 &= ~( UCRXEIE + UCBRKIE );
//						Temp = GetBandRate( Comm.BandRate1 );					
//						UCA1BR0 = BautTab[Temp];
//						UCA1BR1 = BautTab[Temp+1];
//						UCA1MCTL = BautTab[Temp+2];
//					}
					Temp = GetBandRate( Comm.BandRate1 );					
					UCA1BR0 = BautTab[Temp];
					UCA1BR1 = BautTab[Temp+1];
					UCA1MCTL = BautTab[Temp+2];
					UCA1IE = UCRXIE;
					Comm.Run &= ~FComOK1;
    			}
				break;
		case 2: if( Comm.TX_Delay2 == 0 ) 
				{	
                    PDIR_SW4852 &= ~P_SW4852;			//接收状态
					Comm.Buf2[0] = 0xff;
					Comm.Mode2 = 1;
					Comm.Ptr2 = 0;
					Comm.BTime2 = 0;					// 字符间定时
					Comm.TX_FESum2 = 0;
					UCA2STAT = 0;
//					if( Comm.BandRateMode == 1 )		//波特率自适应
//					{
//						UCA2CTL1 |= UCRXEIE + UCBRKIE;
//						UCA2BR0 = Baut_9600L;
//						UCA2BR1 = Baut_9600H;
//						UCA2MCTL = Baut_9600M;	
//					}
//					else 
//					{
//						UCA2CTL1 &= ~( UCRXEIE + UCBRKIE );
//						Temp = GetBandRate( Comm.BandRate2 );					
//						UCA2BR0 = BautTab[Temp];
//						UCA2BR1 = BautTab[Temp+1];
//						UCA2MCTL = BautTab[Temp+2];
//					}
					Temp = GetBandRate( Comm.BandRate2 );					
					UCA2BR0 = BautTab[Temp];
					UCA2BR1 = BautTab[Temp+1];
					UCA2MCTL = BautTab[Temp+2];
					UCA2IE = UCRXIE;
					Comm.Run &= ~FComOK2;
    			}
				break;
		case 3: if( Comm.TX_Delay3 == 0 ) 
				{	
#if ( CarryComm == YesCheck )
#else
                    PDIR_SWZB &= ~P_SWZB;			//接收状态
#endif
					Comm.Buf3[0] = 0xff;
					Comm.Mode3 = 1;
					Comm.Ptr3 = 0;
					Comm.BTime3 = 0;					// 字符间定时
					Comm.TX_FESum3 = 0;
//					UCA3STAT = 0;
					UCA0STAT = 0;
//					if( Comm.BandRateMode == 1 )		//波特率自适应
//					{
//						UCA0CTL1 |= UCRXEIE + UCBRKIE;
//						UCA0BR0 = Baut_9600L;
//						UCA0BR1 = Baut_9600H;
//						UCA0MCTL = Baut_9600M;	
//					}
//					else 
//					{
//						UCA0CTL1 &= ~( UCRXEIE + UCBRKIE );
//						Temp = GetBandRate( Comm.BandRate3 );					
//						UCA0BR0 = BautTab[Temp];
//						UCA0BR1 = BautTab[Temp+1];
//						UCA0MCTL = BautTab[Temp+2];
//					}
					Temp = GetBandRate( Comm.BandRate3 );					
					UCA0BR0 = BautTab[Temp];
					UCA0BR1 = BautTab[Temp+1];
					UCA0MCTL = BautTab[Temp+2];
					UCA0IE = UCRXIE;
					Comm.Run &= ~FComOK3;
    			}
				break;
		default: break;	
	}	
}				

void CommRest( short COM_No )  		//恢复至初始状态
{
	switch( COM_No )
	{
//		case 0:	
//			UCA0CTL1 |= UCSWRST;
//			UCA0CTL0 = UCPEN + UCPAR;
//			UCA0CTL1 = UCSSEL1 + UCSSEL0;	//SMCLK
//			Comm.TX_Delay0 = 0;				
//			break;	
		case 1:	
			UCA1CTL1 |= UCSWRST;
			UCA1CTL0 = UCPEN + UCPAR;
			UCA1CTL1 = UCSSEL1 + UCSSEL0;	//SMCLK
			Comm.TX_Delay1 = 0;				
			break;	
		case 2:	
			UCA2CTL1 |= UCSWRST;
			UCA2CTL0 = UCPEN + UCPAR;
			UCA2CTL1 = UCSSEL1 + UCSSEL0;	//SMCLK
			Comm.TX_Delay2 = 0;				
			break;	
		case 3:	
//			UCA3CTL1 |= UCSWRST;
//			UCA3CTL0 = UCPEN + UCPAR;
//			UCA3CTL1 = UCSSEL1 + UCSSEL0;	//SMCLK
			UCA0CTL1 |= UCSWRST;
			UCA0CTL0 = UCPEN + UCPAR;
			UCA0CTL1 = UCSSEL1 + UCSSEL0;	//SMCLK
			Comm.TX_Delay3 = 0;				
			break;	
		default: break;
	}
	CommInit( COM_No );
}	
*/

void GetEventUpMode( unsigned char* Dest )		//16.05.17
{
	E2P_RData( Dest, EEventUpMode, 8 ); 
	if(( Flag.BatState & F_E2PCheck ) != 0 )
	{
		E2P_RFM( Dest, FEventUpMode, 8 );									
		if(( Flag.BatState & F_E2PCheck ) != 0 )
		{
			RAM_Write( Dest, (unsigned char*)&Para.REventUpMode, 8 );	
		}	
	}	
}	

short MeterAds_Comp( unsigned char* Source )
{
	unsigned char Buff[6];
	unsigned char* Point;
	short i;
	
	Point = Buff;

//	if( Addr_9_9(Source) == 0 ) return 0;			//DKYCE
//	if((PIN_SETEN & P_SETEN ) != 0 )
//	if(((PIN_SETEN & P_SETEN ) != 0 )&&( IsFactoryTestState() != 0 )) 	//新国网	//14.04.09
	if(((HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&( IsFactoryTestState() != 0 )) 	//新国网	//14.04.09
	{
		if( IsAllData( Source, 6, 0xAA ) == 0 ) return 1;		//不支持全AA写
	}
//	E2P_RData( Point, EMeter_No, 6 );				
    E2P_RData( Point, EMeter_Addr, 6 );	     	//17.03.16			    				
	for( i=0;i<6;i++)
	{
		if(( *Source != 0xAA )&&( *Source != *Point))
		{
			return 1;	
		}	
		Point++;
		Source++;
	}	
	return 0;	
}	

short MeterAds_Match( unsigned char* Source )						//V8
{
	unsigned char Buff[6];
	unsigned char* Point;
	
	Point = Buff;

//	if((PIN_SETEN & P_SETEN ) == 0 )
//	if(((PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) 	//新国网	//14.04.09
	if(((HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) 	//新国网	//14.04.09
	{
		if( MeterAds_Comp( Source ) == 0 ) return 0;			
	}	
//	E2P_RData( Point, EMeter_No, 6 );	
    E2P_RData( Point, EMeter_Addr, 6 );	     	//17.03.16			    				
	if( Data_Comp( Point, Source, 6 ) != 0 ) return 1;

	return 0; 	
}	

short Addr_9_9( unsigned char* Source )
{
	short i;
	for( i=0;i<6;i++ )
	{
		if( *Source != 0x99 )
		 	return 1;
		Source++;			
	}		
	return 0;	
}
	
void WritePassWd_Err( unsigned short Addr, unsigned char Temp )
{
	unsigned char Buff[2];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	E2P_RData( Ptr, Addr, 1 );
	if(( *Ptr == Temp )&&(( Flag.BatState & F_E2PCheck ) == 0 )) return;
	else
	{
		*Ptr = Temp;	
		E2P_WData( Addr, Ptr, 1 );
	}	
}

short Password_Comp( unsigned char* Source )
{
	unsigned char Buff[4];
	unsigned char* Point;  	
	
	Point = Buff;
//	if((PIN_SETEN & P_SETEN ) == 0 ) 
//	if(((PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) 	//新国网	//14.04.09
	if(((HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) 	//新国网	//14.04.09
	{
		SM.SetCloseDelay2 = 0;
		SM.SetCloseDelayChk2 = 0xA5;
		SM.PassWd_ErrCnt2 = 0;
		SM.PassWd_ErrCntChk2 = 0xA5;
		SM.SetCloseDelay4 = 0;
		SM.SetCloseDelayChk4 = 0xA5;
		SM.PassWd_ErrCnt4 = 0;
		SM.PassWd_ErrCntChk4 = 0xA5;

		WritePassWd_Err( EPassWd_ErrCnt2, 0 );
		WritePassWd_Err( EPassWd_ErrCnt4, 0 );
		WritePassWd_Err( ESetCloseDelay2, 0 );
//		WritePassWd_Err( ESetCloseDelay2, 0 );
		WritePassWd_Err( ESetCloseDelay4, 0 );		//10.07.09
		return 0;	
	}	

	if( *Source == 0x02 )
	{
		if( SM.PassWd_ErrCnt2 < PASSWDERRCNT ) 
		{
			E2P_RData( Point, Password0+(*Source)*5, 4 );
			if( Data_Comp( Source,Point,4) == 0 )
			{
				SM.SetCloseDelay2 = 0;
				SM.SetCloseDelayChk2 = 0xA5;
				SM.PassWd_ErrCnt2 = 0;
				SM.PassWd_ErrCntChk2 = 0xA5;
				WritePassWd_Err( EPassWd_ErrCnt2, 0 );
				WritePassWd_Err( ESetCloseDelay2, 0 );
				return 0;	
			}	
			else 
			{
				SM.PassWd_ErrCnt2++;
				SM.PassWd_ErrCntChk2 = 0xA5 + SM.PassWd_ErrCnt2;
				WritePassWd_Err( EPassWd_ErrCnt2, SM.PassWd_ErrCnt2 );
				if( SM.PassWd_ErrCnt2 == PASSWDERRCNT ) 
				{
					SM.SetCloseDelay2 = 24;
					SM.SetCloseDelayChk2 = SM.SetCloseDelay2 + 0xA5;
					WritePassWd_Err( ESetCloseDelay2, 24 );
				}	
				return 1;
			}	
		}
	}
	else if( *Source == 0x04 )
	{
		if( SM.PassWd_ErrCnt4 < PASSWDERRCNT ) 
		{
			E2P_RData( Point, Password0+(*Source)*5, 4 );
			if( Data_Comp( Source,Point,4) == 0 )
			{
				SM.SetCloseDelay4 = 0;
				SM.SetCloseDelayChk4 = 0xA5;
				SM.PassWd_ErrCnt4 = 0;
				SM.PassWd_ErrCntChk4 = 0xA5;
				WritePassWd_Err( EPassWd_ErrCnt4, 0 );
				WritePassWd_Err( ESetCloseDelay4, 0 );
				return 0;	
			}	
			else 
			{
				SM.PassWd_ErrCnt4++;
				SM.PassWd_ErrCntChk4 = 0xA5 + SM.PassWd_ErrCnt4;
				WritePassWd_Err( EPassWd_ErrCnt4, SM.PassWd_ErrCnt4 );
				if( SM.PassWd_ErrCnt4 == PASSWDERRCNT ) 
				{
					SM.SetCloseDelay4 = 24;
					SM.SetCloseDelayChk4 = SM.SetCloseDelay4 + 0xA5;
					WritePassWd_Err( ESetCloseDelay4, 24 );
				}	
				return 1;
			}	
		}
	}

	return 1;	
//#endif
}	

short ExtPassword_Comp( unsigned char* Source )
{
	unsigned char Buff[8];
	unsigned char* Point;  	
	
	Point = Buff;
//	if((PIN_SETEN & P_SETEN ) == 0 ) 
//	if(((PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) 	//新国网	//14.04.09
	if(((HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) 	//新国网	//14.04.09
	{
		SM.SetCloseDelay2 = 0;
		SM.SetCloseDelayChk2 = 0xA5;
		SM.PassWd_ErrCnt2 = 0;
		SM.PassWd_ErrCntChk2 = 0xA5;
		SM.SetCloseDelay4 = 0;
		SM.SetCloseDelayChk4 = 0xA5;
		SM.PassWd_ErrCnt4 = 0;
		SM.PassWd_ErrCntChk4 = 0xA5;
		WritePassWd_Err( EPassWd_ErrCnt2, 0 );
		WritePassWd_Err( EPassWd_ErrCnt4, 0 );
		WritePassWd_Err( ESetCloseDelay2, 0 );
//		WritePassWd_Err( ESetCloseDelay2, 0 );
		WritePassWd_Err( ESetCloseDelay4, 0 );		//10.07.09
		return 0;	
	}	

	if( *Source == 2 )
	{
		if( SM.PassWd_ErrCnt2 < PASSWDERRCNT ) 
		{
			E2P_RData( Point, Password0+(*Source)*5, 4 );
			E2P_RData( Point+4, PasswordDiff, 3 );
			_BCD3ADD( Point+1, Point+4 );
			if( Data_Comp( Source,Point,4) == 0 )
			{
				SM.SetCloseDelay2 = 0;
				SM.SetCloseDelayChk2 = 0xA5;
				SM.PassWd_ErrCnt2 = 0;
				SM.PassWd_ErrCntChk2 = 0xA5;
				WritePassWd_Err( EPassWd_ErrCnt2, 0 );
				WritePassWd_Err( ESetCloseDelay2, 0 );
				return 0;	
			}	
			else 
			{
				SM.PassWd_ErrCnt2++;
				SM.PassWd_ErrCntChk2 = 0xA5 + SM.PassWd_ErrCnt2;
				WritePassWd_Err( EPassWd_ErrCnt2, SM.PassWd_ErrCnt2 );
				if( SM.PassWd_ErrCnt2 == PASSWDERRCNT ) 
				{
					SM.SetCloseDelay2 = 24;
					SM.SetCloseDelayChk2 = SM.SetCloseDelay2 + 0xA5;
					WritePassWd_Err( ESetCloseDelay2, 24 );
				}	
				return 1;
			}	
		}
	}
	else if( *Source == 4 )
	{
		if( SM.PassWd_ErrCnt4 < PASSWDERRCNT ) 
		{
			E2P_RData( Point, Password0+(*Source)*5, 4 );
			E2P_RData( Point+4, PasswordDiff, 3 );
			_BCD3ADD( Point+1, Point+4 );
			if( Data_Comp( Source,Point,4) == 0 )
			{
				SM.SetCloseDelay4 = 0;
				SM.SetCloseDelayChk4 = 0xA5;
				SM.PassWd_ErrCnt4 = 0;
				SM.PassWd_ErrCntChk4 = 0xA5;
				WritePassWd_Err( EPassWd_ErrCnt4, 0 );
				WritePassWd_Err( ESetCloseDelay4, 0 );
				return 0;	
			}	
			else 
			{
				SM.PassWd_ErrCnt4++;
				SM.PassWd_ErrCntChk4 = 0xA5 + SM.PassWd_ErrCnt4;
				WritePassWd_Err( EPassWd_ErrCnt4, SM.PassWd_ErrCnt4 );
				if( SM.PassWd_ErrCnt4 == PASSWDERRCNT ) 
				{
					SM.SetCloseDelay4 = 24;
					SM.SetCloseDelayChk4 = SM.SetCloseDelay4 + 0xA5;
					WritePassWd_Err( ESetCloseDelay4, 24 );
				}	
				return 1;
			}	
		}
	}
	return 1;	
}	

/*
short BroadC_Time( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;		
	unsigned long Lng1,Lng2;
	unsigned short i;
	
	Ptr = Buff;
	
	if(( SM.BroadC_Mins != 0 )||( Data_Comp( Source+3,&Clk.Day[0],3 ) != 0 )) return 1;

	if( DateTime6ValueCheck( Source ) != 0 ) return 1;			//12.01.16

	RAM_Fill( Ptr, 12 );

	if( Clk.Hour[0] == 0x23 )						
	{												
		if( Clk.Min[0] >= 0x55 ) return 1;			
	}													
	if( Clk.Hour[0] == 0x0 )						
	{												
		if( Clk.Min[0] < 0x05 ) return 1;			
	}												

	Lng1 = GetSec(Clk.Sec);
	for( i=0;i<3;i++ )
	{
		E2P_RData( Ptr+i*4+2, ESave_Day1+i*3, 2 );
		if( *(Ptr+i*4+3) != Clk.Day[0] ) continue;
		if(( AllIsBCD( Ptr+i*4+2, 2 ) == 0 )&&( JudgeDayH( Ptr+i*4+2 ) == 0 ))	
		{
		    Lng2 = GetSec(Ptr+i*4)-Lng1;
		    if( Lng2 & 0x80000000 ) Lng2=~Lng2+1;
		    if( Lng2 <= 300 ) return 1;
		}	
	}	

	E2P_RData( Ptr+1, DayFreezeTime, 2 );								//12.10.16
	*Ptr = 0;
	if(( AllIsBCD( Ptr, 3 ) == 0 )&&( JudgeHourM( Ptr+1 ) == 0 ))	
	{
	    Lng2 = GetSec(Ptr)-Lng1;
	    if( Lng2 & 0x80000000 ) Lng2=~Lng2+1;
	    if( Lng2 <= 300 ) return 1;
	}	

    Lng2 = GetSec(Source)-Lng1;
    if( Lng2 & 0x80000000 ) Lng2=~Lng2+1;
    if( Lng2 <= 300 ) return 0;
    else return 1;    	
}
*/
#if ( RemoteFeeMeter ==	YesCheck )		//15.12.14		
int GetTimeMinZone( unsigned char Min )
{
	unsigned char Temp;
	
	Temp = BCD_Byte( Min );
	Temp /= 15;
	return Temp;	
}	
#endif

short BroadC_Time( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;		
	unsigned long Lng1,Lng2;
	unsigned short i;
	
	Ptr = Buff;
	
	if(( SM.BroadC_Mins != 0 )||( Data_Comp( Source+3,&Clk.Day[0],3 ) != 0 )) return 1;

	if( DateTime6ValueCheck( Source ) != 0 ) return 1;			//12.01.16

	RAM_Fill( Ptr, 12 );

	if( Clk.Hour[0] == 0x23 )						
	{												
		if( Clk.Min[0] >= 0x55 ) return 1;			
	}													
	if( Clk.Hour[0] == 0x0 )						
	{												
		if( Clk.Min[0] < 0x05 ) return 1;			
	}												

	Lng1 = GetSec(Clk.Sec);
	for( i=0;i<3;i++ )
	{
		E2P_RData( Ptr+i*4+2, ESave_Day1+i*3, 2 );
		if( *(Ptr+i*4+3) != Clk.Day[0] ) continue;
		if(( AllIsBCD( Ptr+i*4+2, 2 ) == 0 )&&( JudgeDayH( Ptr+i*4+2 ) == 0 ))	
		{
		    Lng2 = GetSec(Ptr+i*4)-Lng1;
		    if( Lng2 & 0x80000000 ) Lng2=~Lng2+1;
		    if( Lng2 <= 300 ) return 1;
		}	
	}	

//	E2P_RData( Ptr+1, DayFreezeTime, 2 );								//12.10.16
//	*Ptr = 0;
//	if(( AllIsBCD( Ptr, 3 ) == 0 )&&( JudgeHourM( Ptr+1 ) == 0 ))	
//	{
//	    Lng2 = GetSec(Ptr)-Lng1;
//	    if( Lng2 & 0x80000000 ) Lng2=~Lng2+1;
//	    if( Lng2 <= 300 ) return 1;
//	}	

#if ( RemoteFeeMeter ==	YesCheck )		//15.12.14		
	if( GetTimeMinZone( Clk.Min[0] ) != GetTimeMinZone( *(Source+1) )) return 1;		//不在同一个15分钟区间内，不允许广播校时	//16.01.19
#endif

    Lng2 = GetSec(Source)-Lng1;
    if( Lng2 & 0x80000000 ) Lng2=~Lng2+1;
    if( Lng2 <= 300 ) return 0;
    else return 1;    	
}


/*
void CreatTimeRec( unsigned char* Source )
{
//	unsigned short LastPt;	
	long LastPt;	
	unsigned char Buff[20];
	unsigned char* Ptr;
	
	Ptr = Buff;

//	IncEventCunt( TimeSet_No );
	LastPt = PtAddrChk( TimeSet_No, Next );				//定位新一次需量清零记录首址		
	if( LastPt == -1 ) return;
	IncEventCunt( TimeSet_No );
	RAM_DataFill( Ptr, 4, 0xFF );			//操作者代码	
	RAM_Write( Ptr+4, Clk.Sec, 6 );					
	RAM_Write( Ptr+10, Source, 6 );					
	
	E2P_WData( LastPt, Ptr, 16 );
	
}	

void MeterClrCycle( void )
{
	unsigned char Buff[4];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	switch( SM.MeterClrState )	
	{
		case MeterClrIdle:			// 电表清零空闲
			SM.MeterClrStateChk = MeterClrIdle;			
			break;
		case MeterClrStep1:			// 电表清零步骤1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
			CreateECClrRec();		//电量清零前记录					//10.04.26				
			SM.MeterClrState = MeterClrStep2;
			SM.MeterClrStateChk = SM.MeterClrState + 0xA5A5;
			break;
		case MeterClrStep2:			// 电表清零步骤1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
			SM.MeterClrState = MeterClrIdle;
			SM.MeterClrStateChk = MeterClrIdle;

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			Flag.RSwitchOldState |= 0x03;		//不报拉合闸成功事件						//新国网	//14.06.28		
//			//后续总清事件有写EEPROM操作													//新国网	//14.06.28		
//			E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );			//新国网	//14.06.28	
#endif
//			CreateECClrRec();		//电量清零前记录						
			MD_Rest(1);		//清当前需量
			AllECClr();		//清除当前电量、历史电量指针、项数
			AllMDClr();		//清除历史需量指针、项数
			RAM_DataFill( Ptr, 4, 0xff);		//事件全清
			AllEventClr( Ptr, 0 );
			AllDJClr();
			Clr_E2PData( LoadRecState1, 4, 6 );  //清负荷曲线
//			if(( PIN_SETEN & P_SETEN ) == 0 )
//			if((( PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))	//新国网	//14.04.09
			if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))	//新国网	//14.04.09  
			{
				Clr_E2PData( Bat_WorkTime, 4, 1 );   //清电池工作时间
				RAM_Fill( Real.RBat_WorkTime, 4 );
				Real.BatWorkTimeChk = 0xA5;
				ClrMeterClrRec();		//清除电表清零记录 10.05.11
//				ClrPrivateKeyFlag();			//10.07.29		//新国网
			}	
#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
			Clr_E2PData( L1_SaveDay1EC_Pp0, 4, 4 );  //最近第一、二次第一结算日结算时正反向有功电量		//12.10.04
#endif																									//12.10.04
			Clr_E2PData( L1_YearSaveDayEC_Pp0, 4, 4 );  //最近第一、二次年结算日结算时正反向有功电量	//新国网
			Clr_E2PData( EFstCntPA, 2, 3 );  			//分相正向有功电量掉电残余电量					//新国网	//13.12.28
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			Flag.MeterState1 &= ~( F_ClockBat+F_CommBat );		//电表清零后，电池状态需重新判断		//新国网	//14.06.22
			*Ptr = 0x00;																				//新国网	//14.06.22		
			Para.RBatState = *Ptr;						//16.05.09
			E2P_WData( EBatState, Ptr, 1 );																//新国网	//14.06.22	
#endif
			break;
		default: 
			SM.MeterClrState = MeterClrIdle;
			SM.MeterClrStateChk = MeterClrIdle;			
			break;	
	}
}

void MDClrCycle( void )
{
	switch( SM.MDClrState )	
	{
		case MDClrIdle:			// 需量清零空闲
			SM.MDClrStateChk = MDClrIdle;			
			break;
//		case MDClrStep0:			// 需量清零步骤1
//			if( SM.MDClrStateChk != ( SM.MDClrState +0xA5A5 ) ) break;
//			RAM_Fill( Comm.OperateCode, 4 );
//			CreateMDClrRec();								
//			SM.MDClrState = MDClrStep1;
//			SM.MDClrStateChk = SM.MDClrState +0xA5A5;
//			break;
		case MDClrStep1:			// 需量清零步骤1
			if( SM.MDClrStateChk != ( SM.MDClrState +0xA5A5 ) ) break;
			SM.MDClrState = MDClrIdle;
			SM.MDClrStateChk = MDClrIdle;
			MD_Rest(1);		//清当前需量
			break;
		default: 
			SM.MDClrState = MDClrIdle;
			SM.MDClrStateChk = MDClrIdle;			
			break;	
	}
}
*/

//*****************************************************************
// Communication control code describe
//*****************************************************************
void Comm_Ctrl(unsigned char* ComBuf)
{
  	unsigned char Temp;
//  	unsigned short Temp3;
//  	unsigned char Temp2;
  	unsigned short ComID;
//	unsigned char* ComRecTmr;
	unsigned short* BTime;
	unsigned long LComID;													//液晶查看
	
//    ComRecTmr = CommPara[GetCOM_No(ComBuf)].RecTmr;
	BTime = CommPara[GetCOM_No(ComBuf)].BTime;
	
	ComID = (short)*(ComBuf+Rs_IDHPtr)*256 + (short)*(ComBuf+Rs_IDLPtr);
	RAM_Write( (unsigned char*)&LComID, ComBuf+NRs_ID0Ptr, 4 );				//液晶查看

	if( ~(*(ComBuf+Rs_CtlPtr)) & 0x80 )
	{
   		switch (*(ComBuf+Rs_CtlPtr) & 0x1F)
   		{
      		case RS_CTL_Read:
//				if( HHTDisplay( ComBuf ) != 0 )
//				{
//					if( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)
					if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 ))
					{
#if ( AdjustErrorCheck == YesCheck )	//15.09.05
						Temp = *(ComBuf+Rs_IDHPtr);
#if ( MEASCHIP == IDT90E32 )							
						if(( Temp == 0xE8 )||( Temp == 0xE9 )||( Temp == 0xEA )||(ComID == 0xF0FF)||(ComID == 0xF0FC))						
#elif ( MEASCHIP == ATT7022E )							
						if(( Temp == 0xEC )||( Temp == 0xED )||( Temp == 0xEE )||(ComID == 0xF0FF)||(ComID == 0xF0FC))						
#endif
						{
							Temp = Spe_Com_Read(ComBuf);								
						}
						else Temp = Com_Read(ComBuf);							
#else
						Temp = Com_Read(ComBuf);
#endif
						if( Temp == 0 ) *(ComBuf+Rs_CtlPtr) = 0x81;
						else
						{
            				*(ComBuf+Rs_CtlPtr) = 0xC1;
            				*(ComBuf+Rs_LenPtr) = 1;            			
            				*(ComBuf+Rs_IDLPtr) = Temp;            			            			
         				}
         			} 
         			else *ComBuf = 0xFF;
//         		}	
				break;   
      		case RS_CTL_Write:    		
     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
//																			//新国网
//    			if( (( PIN_SETEN & P_SETEN ) != 0 )&&(((SM.SetCloseDelay2!=0)&&( *(ComBuf+Rs_DataPtr) == 2 ))||((SM.SetCloseDelay4!=0)&&( *(ComBuf+Rs_DataPtr) == 4 ))))
//     			{
//					if( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0 )
//					{
//						*(ComBuf+Rs_CtlPtr) = 0xC4;
//            			*(ComBuf+Rs_LenPtr) = 1;            			
//            			*(ComBuf+Rs_IDLPtr) = RS_State_PswE;           			            			
//					}
//					else *ComBuf = 0xFF;		//各级密码单独闭锁	
//     				break;
//     			}
     				
//				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&((( PIN_SETEN & P_SETEN ) == 0 )||( SM.PrgDly != 0 )		//新国网
//					||(ComID==0xC117)))																						//新国网	
//				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&(( PIN_SETEN & P_SETEN ) == 0 ))							//新国网
//				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&((( PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )))	//新国网	//14.04.09
				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 )))  
				{
					if( Password_Comp( ComBuf+Rs_DataPtr ) == 0 )	
					{	
#if ( MEASCHIP == ADE7758 )
						Temp = Com_Write(ComBuf);
#elif ( MEASCHIP == ADE7878 )	  
						Temp = Com_Write(ComBuf);
#elif ( MEASCHIP == IDT90E32 )							//12.08.29	
						if( *(ComBuf+Rs_IDHPtr) == 0xF6 ) Temp = ComAdjWrite( ComBuf );
#if ( AdjustErrorCheck == YesCheck )	//15.09.05
						else if(( *(ComBuf+Rs_IDHPtr) == 0xE8 )||( *(ComBuf+Rs_IDHPtr) == 0xE9 )||( *(ComBuf+Rs_IDHPtr) == 0xEA )||(ComID == 0xF0FF)||(ComID == 0xF0FC))
						{
							*BTime =MS4000;						
							Temp = TestComAdjWrite( ComBuf );						
							*BTime =MS500;						
						}	
#endif						
						else Temp = RS_State_PswE;	
#elif ( MEASCHIP == ATT7022E )									//11.09.24
						if( *(ComBuf+Rs_IDHPtr) == 0xF8 ) Temp = ComAdjWrite( ComBuf );
#if ( AdjustErrorCheck == YesCheck )	//15.09.05
						else if(( *(ComBuf+Rs_IDHPtr) == 0xEC )||( *(ComBuf+Rs_IDHPtr) == 0xED )||( *(ComBuf+Rs_IDHPtr) == 0xEE )||(ComID == 0xF0FF)||(ComID == 0xF0FC))
						{
							*BTime =MS4000;						
							Temp = TestComAdjWrite( ComBuf );						
							*BTime =MS500;						
						}	
#endif						
						else
						{
//							Temp = Com_Write(ComBuf);
							Temp = RS_State_PswE;	
						}
#else
						if( *(ComBuf+Rs_IDHPtr) == 0xFF ) Temp = ComAdjWrite( ComBuf );
						else
						{
//							Temp = Com_Write(ComBuf);
							Temp = RS_State_PswE;	
						}
#endif						
					}
					else Temp = RS_State_PswE;
      	   			if( Temp == 0 )
      	   			{
//	  					Comm.PrgTmr = 4;
         				*(ComBuf+Rs_CtlPtr) = 0x84;
         				*(ComBuf+Rs_LenPtr) = 0;            			
         			}
         			else
         			{
            			*(ComBuf+Rs_CtlPtr) = 0xC4;
            			*(ComBuf+Rs_LenPtr) = 1;            			
            			*(ComBuf+Rs_IDLPtr) = Temp;            			            			
         			}
         		} else *ComBuf = 0xFF;
      			break;      
      		case RS_CTL_AdjTime:
/*     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
     			if( IsInfraredValid( ComBuf ) != 0 ) { *ComBuf = 0xFF; break; }			//新国网
         		*ComBuf = 0xFF;
//         		if(( Addr_9_9(ComBuf+Rs_Addr0Ptr) == 0 )&&( BroadC_Time( ComBuf+Rs_IDLPtr ) == 0 ))
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
         		if((( Addr_9_9(ComBuf+Rs_Addr0Ptr) == 0 )||( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0))&&( BroadC_Time( ComBuf+Rs_IDLPtr ) == 0 )&&(*(ComBuf+Rs_LenPtr) == 0x06))	//14.09.03
#else
         		if((( Addr_9_9(ComBuf+Rs_Addr0Ptr) == 0 )&&( BroadC_Time( ComBuf+Rs_IDLPtr ) == 0 ))&&(*(ComBuf+Rs_LenPtr) == 0x06))
#endif
         		{		
//					CreatTimeRec( ComBuf+Rs_IDLPtr );		//山东要求

					Clk.Sec_64 = 32;
         			RAM_Write( &Clk.Sec[0], ComBuf+Rs_IDLPtr, 6 );
//#if(( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))																	//V1000
					if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
					{
						E2Pwrite( 0x0, Clk.Sec, 3, RTCAds );	//写RX8025时钟芯片
					}	
//#else
					else
					{
						E2Pwrite( 0x0, Clk.Sec, 3, DS3231Ads );	//写DS3231时钟芯片
					}	
//#endif				
					SM.TimeSet_Days = 0;
					SM.BroadC_Mins = 0x05A0;
// 					Comm.PrgTmr = 4;
					*ComRecTmr = 2;
					GetDaySeg();				
					GetFeeNo();		
					MD_InitialCalPara( Para.RMaxT );		

					SM.TimeSetLoadAdjDelay = 0;							//cru		//广播校时后立即根据当前时间进行负荷曲线调整		
					SM.LoadAdjMode = 0x3F;
//					E2P_WData( EBandRateMode, (unsigned char*)&SM.LoadAdjMode, 1 ); 		//cru	//需进行调整的负荷曲线
					E2P_WData( ELoadAdjMode, (unsigned char*)&SM.LoadAdjMode, 1 ); 		//cru	//需进行调整的负荷曲线
					SM.LoadAdjState = LoadAdjCruve1;	
					SM.LoadAdjStateChk = LoadAdjCruve1 + 0xA5A5;	
				}      	
*/
				*ComBuf = 0xFF;
	      		break;
     		case RS_CTL_ChgPswd:
/*			if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0 )&&((( PIN_SETEN & P_SETEN ) == 0 )||(( SM.PrgDly != 0 )&&(SM.SetCloseDelay==0)))&&( Password_Comp( ComBuf+Rs_IDLPtr ) == 0 )&&( *(ComBuf+Rs_IDLPtr) == 0 ))		//V2
			{	
//				GetPrgRecd( 0x000F );		//编程记录		
         		if( *(ComBuf+Rs_IDLPtr+4) > 9 ) *(ComBuf+Rs_IDLPtr+4) = 9;
				E2P_WData( Password0+*(ComBuf+Rs_IDLPtr+4)*5, ComBuf+Rs_IDLPtr+4, 4 ); 
				RAM_Write( ComBuf+Rs_IDLPtr, ComBuf+Rs_IDLPtr+4, 4 );
            	*(ComBuf+Rs_CtlPtr) = 0x8F;
           		*(ComBuf+Rs_LenPtr) = 4;            			
  				Comm.PrgTmr = 4;
         	}else *ComBuf = 0xFF; 	
*/
				*ComBuf = 0xFF;
				break; 
		case RS_CTL_RstMD:
/*
				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&((( PIN_SETEN & P_SETEN ) == 0 )||(( SM.PrgDly != 0 )&&(SM.SetCloseDelay==0))))	
				{
					if(( Password_Comp( ComBuf+Rs_IDLPtr ) == 0 )&&( *(ComBuf+Rs_IDLPtr) == 0 ))
					{	
//						GetPrgRecd( 0x0010 );		//编程记录		
         				Temp = 0;
//						MD_Rest(1);		//全清
					}
					else Temp = RS_State_PswE;
					*(ComBuf+Rs_LenPtr) = 0;
      	   			if( Temp == 0 )
      	   			{
	  					Comm.PrgTmr = 4;
         				*(ComBuf+Rs_CtlPtr) = 0x90;
         			}
         			else *(ComBuf+Rs_CtlPtr) = 0xD0;            		
         		} else *ComBuf = 0xFF;
*/
				*ComBuf = 0xFF;
				break;		
      	case RS_CTL_ChgBaut:
/*      		if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||(( PIN_SETEN & P_SETEN ) == 0 ))	
      		{
      			*(ComBuf+Rs_CtlPtr) = 0x8C;
      			switch( *(ComBuf+Rs_IDLPtr))
      			{
      				case Baut300:  
      				case Baut600:  
      				case Baut2400: 
      				case Baut4800: 
      				case Baut9600: 
      					if( ComBuf == Comm.Buf1 )
      					{ 
      						Comm.BandRate1 = *(ComBuf+Rs_IDLPtr); 
//     						Comm.BandRateTime1 = MS3000; 	
							break; 
						}      								
      					else if( ComBuf == Comm.Buf2 )
      					{ 
      						Comm.BandRate2 = *(ComBuf+Rs_IDLPtr); 
//     						Comm.BandRateTime2 = MS3000; 	
							break; 
						}      								
      					else if( ComBuf == Comm.Buf3 )
      					{ 
      						Comm.BandRate3 = *(ComBuf+Rs_IDLPtr); 
//					    	Comm.BandRateTime3 = MS3000; 	
      						break; 
      					}
					default:*(ComBuf+Rs_IDLPtr) = 0xFF;break;		
				}	
      		}else *ComBuf = 0xFF;	
*/      		
			*ComBuf = 0xFF;
      		break;		      							
     	case NRS_CTL_ExtRead:		//自定义读
//        	  if( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)
        	  if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 )
        	  		||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0x99 ) == 0 ))
				{
					if(( LComID == 0x04B00847 )&&( *(ComBuf+Rs_LenPtr) == 0x0B )) *(ComBuf+NRs_ID0Ptr) = 0x44;		//液晶查看	 
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
					*BTime =MS4000;						//新国网	//14.05.06
					Temp = NCom_ExtRead(ComBuf);
					*BTime =MS500;						//新国网	//14.05.06
#else
					Temp = NCom_ExtRead(ComBuf);
#endif
					if( Temp == 0 ) *(ComBuf+Rs_CtlPtr) = 0x8D;
					else
					{
           				*(ComBuf+Rs_CtlPtr) = 0xCD;
           				*(ComBuf+Rs_LenPtr) = 1;            			
           				*(ComBuf+Rs_IDLPtr) = Temp;            			            			
       				}
					RAM_Write( (unsigned char*)&LComID, ComBuf+NRs_ID0Ptr, 4 );										//液晶查看
					if( LComID == 0x04B00844 ) *ComBuf = 0xFF;														//液晶查看无回码
     			} 
       			else *ComBuf = 0xFF;
      			break;					
     	case NRS_CTL_ExtWrite:		//自定义写
     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
//																			//新国网		
//     			if( (( PIN_SETEN & P_SETEN ) != 0 )&&(((SM.SetCloseDelay2!=0)&&( *(ComBuf+NRs_PassWPtr) == 2 ))||((SM.SetCloseDelay4!=0)&&( *(ComBuf+NRs_PassWPtr) == 4 ))))
//     			{
//					if( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0 )
//					{
//						*(ComBuf+Rs_CtlPtr) = 0xCE;
//            			*(ComBuf+Rs_LenPtr) = 1;            			
//            			*(ComBuf+Rs_IDLPtr) = RS_State_PswE;           			            			
//					}
//					else *ComBuf = 0xFF;		//各级密码单独闭锁	
//     				break;
//     			}	

//				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&((( PIN_SETEN & P_SETEN ) == 0 )||( SM.PrgDly != 0 )))		//新国网
//				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&(( PIN_SETEN & P_SETEN ) == 0 ))							//新国网
//				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&((( PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )))	//新国网	//14.04.09
				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)&&((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 )))
				{
					if( ExtPassword_Comp( ComBuf+NRs_PassWPtr ) == 0 )	
					{	
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
						*BTime =MS4000;						//新国网	//14.05.06
#else
						*BTime =MS500+10;						//11.03.01
#endif
						Temp = NCom_ExtWrite(ComBuf);
						*BTime =MS500;							//11.03.01
					}
					else Temp = RS_State_PswE;
      	   			if( Temp == 0 )
      	   			{
         				*(ComBuf+Rs_CtlPtr) = 0x8E;
         				*(ComBuf+Rs_LenPtr) = 0;            			
         			}
         			else
         			{
            			*(ComBuf+Rs_CtlPtr) = 0xCE;
            			*(ComBuf+Rs_LenPtr) = 1;            			
            			*(ComBuf+Rs_IDLPtr) = Temp;            			            			
         			}
         		} else *ComBuf = 0xFF;
      			break;            		
     	case NRS_CTL_Read:				
				RAM_Write( (unsigned char*)&LComID, ComBuf+NRs_ID0Ptr, 4 );												//11.01.12	浙江计量中心 //液晶查看	//新国网
/*				if( LComID == 0x04040300 )								                                                //11.05.13  //新国网      
				{																										//11.01.12
    	        	if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 )	//11.01.12
        	  			||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0x99 ) == 0 ))											//11.01.12
					{																									//11.01.12	
						E2P_RData( ComBuf+NRs_WDataPtr, EProductState, 2 ); 											//新国网	//14.02.13	
						if(( *(ComBuf+NRs_WDataPtr) == 0x5A )&&( *(ComBuf+NRs_WDataPtr+1) == 0xA5 )) ;					//新国网	//14.02.13	
						else																							//新国网	//14.02.13	
						{																								//新国网	//14.02.13			
							if( IsInfraredIdentityValidComID( ComBuf, 0xD1, (unsigned char*)&LComID ) != 0 ) break;		//新国网
						}																								//新国网	//14.02.13		

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
						RAM_Write( (unsigned char*)&Disk.CurDisID[0], ComBuf+NRs_RDataPtr, 5 );														//14.09.03	
						if(( Disk.CurDisID[3] == 0xFA )&&( Disk.CurDisID[2] == 0xFC )&&( Disk.CurDisID[1] == 0xFD )&&( Disk.CurDisID[0] == 0xFE ))	//14.09.03	
						{																															//14.09.03			
							RAM_Write( Disk.LCDSeg, ComBuf+NRs_RDataPtr+5, 32 );																	//14.09.03	
						}																															//14.09.03	
						else if( *(ComBuf+Rs_LenPtr) != 0x09 ) 																						//14.09.03
#else
						if( *(ComBuf+Rs_LenPtr) != 0x09 ) 																//11.05.13
#endif
						{ 																								//11.05.13
		       				*(ComBuf+Rs_CtlPtr) = 0xD1;																	//11.05.13
    	  	   				*(ComBuf+Rs_LenPtr) = 1;            														//11.05.13
   							*(ComBuf+Rs_IDLPtr) = RS_State_IVData;            			            					//11.05.13				
							break;																						//11.05.13
						} 																								//11.05.13	

						if( IsAllData( ComBuf+NRs_RDataPtr, 5, 0xFF ) == 0 )
						{
							RAM_DataFill( (unsigned char*)&Disk.CurDisID[0], 4, 0xFE );									//11.01.12
							Disk.CurDisID[4] = 0;				//小项序号由BCD->HEX									//11.01.12
//							SM.LCDAllDisplayDelay = 10;			//LCD全屏查看报警等和跳闸灯点亮
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
							Disk.FullDisDelay = Para.RCommLCDDisplayTime;	//LCD全屏查看报警等和跳闸灯点亮				//14.09.03
#else
							Disk.FullDisDelay = 10;				//LCD全屏查看报警等和跳闸灯点亮
#endif
						}	
						else
						{
							RAM_Write( (unsigned char*)&Disk.CurDisID[0], ComBuf+NRs_RDataPtr, 5 );								
//							SM.LCDAllDisplayDelay = 0;			//LCD全屏查看报警等和跳闸灯点亮
							Disk.FullDisDelay = 0;				//LCD全屏查看报警等和跳闸灯点亮		
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
							SM.BJGDly = Para.RCommLCDDisplayTime;			//LCD查看背光点亮10秒			//新国网 	//14.09.03
#else
							SM.BJGDly = 10;						//LCD查看背光点亮10秒						//新国网 	//13.10.12
#endif
						}	
						
						Flag.Disk |= F_KeyInt;																			//11.01.12
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
						Disk.DisInv = Para.RCommLCDDisplayTime;															//14.09.03
#else
						Disk.DisInv = 10;																				//11.01.12		
#endif
//						Disk.FullDisDelay = 0;																			//11.01.12		
//						if((Flag.Power & F_PwrUp) != 0) SM.BJGDly = 10;			//12.12.28										//11.01.12	
						Disk.TouchDisMode = LookMode;																	//11.01.12

         				*(ComBuf+Rs_CtlPtr) = 0x91;																		//11.05.10	
   	     				*(ComBuf+Rs_LenPtr) = 0x09;            															//11.05.13	

						if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )								//新国网	//13.10.21
						{
							if( IsOccurEventUpSubsFrame() != 0 ) *(ComBuf+Rs_CtlPtr) = 0xB1;		//新国网	//13.10.21	//有主动上报内容	
						}		

//						if(( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 )||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0x99 ) == 0 ))	//11.05.10
						if( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0x99 ) == 0 )	//11.05.10
						{																											//11.05.10
						 	*ComBuf = 0xFF;														//广播液晶查看无回码				//11.05.10
						}																											//11.05.10		
					}																									//11.01.12
					else *ComBuf = 0xFF;														//液晶查看无回码		//11.01.12	
								
					break;																								//11.01.12
				}																										//11.01.12
*/
//     		   if( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)
       		   if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 ))
				{
/*					if( IsInfraredIdentityValidComID( ComBuf, 0xD1, (unsigned char*)&LComID ) != 0 ) break;				//新国网

					if(( *(ComBuf+NRs_ID3Ptr) == 0x06 )&&(*(ComBuf+NRs_ID2Ptr) <= 0x06 )&&( *(ComBuf+NRs_ID1Ptr) == 0x00 )&&( *(ComBuf+NRs_ID0Ptr) <= 0x02 ))
					{
						if(  *(ComBuf+NRs_ID0Ptr) == 0x01 )
						{
							if(*(ComBuf+Rs_LenPtr) != 0x0A) 
							{ 
//								*ComBuf = 0xFF; 
	    	       				*(ComBuf+Rs_CtlPtr) = 0xD1;
    	    	   				*(ComBuf+Rs_LenPtr) = 1;            			
           						*(ComBuf+Rs_IDLPtr) = RS_State_IVData;            			            			
								break;
							} 		//读负荷曲线长度不对
						}
						else if(*(ComBuf+Rs_LenPtr) != 0x05 ) 
						{ 
//							*ComBuf = 0xFF; 
	   	       				*(ComBuf+Rs_CtlPtr) = 0xD1;
   	    	   				*(ComBuf+Rs_LenPtr) = 1;            			
       						*(ComBuf+Rs_IDLPtr) = RS_State_IVData;            			            			
							break;
						} 	//读负荷曲线长度不对					
					}
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
					else if( IsNewLoadParaCom( ComBuf+NRs_ID0Ptr ) != -1 )		//新方法读负荷曲线		//新国网	//14.05.05
					{
						if(*(ComBuf+Rs_LenPtr) != 0x0A) 
						{ 
	    	       			*(ComBuf+Rs_CtlPtr) = 0xD1;
    	    	   			*(ComBuf+Rs_LenPtr) = 1;            			
           					*(ComBuf+Rs_IDLPtr) = RS_State_IVData;            			            			
							break;
						} 		//读负荷曲线长度不对
					}	
#endif
					else
*/					 if(*(ComBuf+Rs_LenPtr) != 0x04 ) 
					{ 
//						*ComBuf = 0xFF; 
	       				*(ComBuf+Rs_CtlPtr) = 0xD1;
      	   				*(ComBuf+Rs_LenPtr) = 1;            			
   						*(ComBuf+Rs_IDLPtr) = RS_State_IVData;            			            			
						break;
					} 		//读数据长度不对											

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
					*BTime =MS4000;						//新国网	//14.05.06
					Temp = NCom_Read(ComBuf);			
					*BTime =MS500;
#else
					Temp = NCom_Read(ComBuf);
#endif
					if( Temp == 0 )
					{
						*(ComBuf+Rs_CtlPtr) = 0x91;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25								//当响应帧为主动上报状态字时，不再提示还有后续数据		//14.05.28
						if(( IsOccurEventUpSubsFrame() != 0 )&&( LComID != 0x04001501 )) *(ComBuf+Rs_CtlPtr) = 0xB1;		//新国网		//有主动上报内容
#else
						if( IsOccurEventUpSubsFrame() != 0 ) *(ComBuf+Rs_CtlPtr) = 0xB1;		//新国网		//有主动上报内容
#endif
					}	
					else if( Temp == 0xB0 ) *(ComBuf+Rs_CtlPtr) = 0xB1;
					else
					{
           				*(ComBuf+Rs_CtlPtr) = 0xD1;
           				*(ComBuf+Rs_LenPtr) = 1;            			
           				*(ComBuf+Rs_IDLPtr) = Temp;            			            			
       				}
     			} 
       			else *ComBuf = 0xFF;
      			break;					//N645

     	case NRS_CTL_ReadSubs:			//N645
/*
//     		    if( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)
       		    if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 ))
				{
					if( IsInfraredIdentityValidComID( ComBuf, 0xD2, ComBuf+NRs_ID0Ptr ) != 0 ) break;			//新国网

					if(*(ComBuf+Rs_LenPtr) != 0x05)						//帧长度不对
					{
	       				*(ComBuf+Rs_CtlPtr) = 0xD2;
      	   				*(ComBuf+Rs_LenPtr) = 1;            			
   						*(ComBuf+Rs_IDLPtr) = RS_State_IVData;            			            			
						break;
					}

					Temp2 = *(ComBuf+NRs_RDataPtr);						//帧序列号
					Temp = NCom_ReadSubs(ComBuf);
					if( Temp == 0 ) 
					{
						*(ComBuf+Rs_CtlPtr) = 0x92;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
						RAM_Write( (unsigned char*)&LComID, ComBuf+NRs_ID0Ptr, 4 );				//当响应帧为主动上报状态字时，不再提示还有后续数据		//14.05.28
						if(( IsOccurEventUpSubsFrame() != 0 )&&( LComID != 0x04001501 )) *(ComBuf+Rs_CtlPtr) = 0xB2;		//新国网		//有主动上报内容
#else
						if( IsOccurEventUpSubsFrame() != 0 ) *(ComBuf+Rs_CtlPtr) = 0xB2;		//新国网		//有主动上报内容
#endif
						*(ComBuf+*(ComBuf+Rs_LenPtr)+10) = Temp2;			//帧序列号
						*(ComBuf+Rs_LenPtr) += 1;
					}	
					else if( Temp == 0xB0 ) 
					{					
						*(ComBuf+Rs_CtlPtr) = 0xB2;
						*(ComBuf+*(ComBuf+Rs_LenPtr)+10) = Temp2;			//帧序列号
						*(ComBuf+Rs_LenPtr) += 1;						
					}	
					else
					{
           				*(ComBuf+Rs_CtlPtr) = 0xD2;
           				*(ComBuf+Rs_LenPtr) = 1;            			
           				*(ComBuf+Rs_IDLPtr) = Temp;            			            			
       				}
     			} 
       			else *ComBuf = 0xFF;
*/
				*ComBuf = 0xFF;
      			break;					//N645
     	case NRS_CTL_ReadAdr:			//N645
//     			if( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 )
     			if(( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 )&&(*(ComBuf+Rs_LenPtr) == 0x00))
     			{
     				*(ComBuf+Rs_CtlPtr) = 0x93;
           			*(ComBuf+Rs_LenPtr) = 6;            			
//     				E2P_RData( ComBuf+Rs_IDLPtr, EMeter_No, 6 );	     				    				
     				E2P_RData( ComBuf+Rs_IDLPtr, EMeter_Addr, 6 );	     	//17.03.16			    				
     			}else *ComBuf = 0xFF;	
      			break;					//N645
     	case NRS_CTL_ZBReadAdr:			//N645
     			if(( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0x99 ) == 0 )&&(*(ComBuf+Rs_LenPtr) == 0x02)&&(*(ComBuf+Rs_IDLPtr) == 0xAF)&&(*(ComBuf+Rs_IDHPtr) == 0x02))
     			{
     				*(ComBuf+Rs_CtlPtr) = 0x9F;
           			*(ComBuf+Rs_LenPtr) = 8;            			
//     				E2P_RData( ComBuf+Rs_DataPtr, EMeter_No, 6 );	     				    				
     				E2P_RData( ComBuf+Rs_DataPtr, EMeter_Addr, 6 );	     	//17.03.15			    				
     			}else *ComBuf = 0xFF;	
      			break;					//N645
     	case NRS_CTL_Write:				//N645
     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
     			
     			RAM_Write( (unsigned char*)&LComID, ComBuf+NRs_ID0Ptr, 4 );		//新国网
/*				if( LComID == 0x04001301 )										//新国网	
				{
					if(( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )																					//新国网	//13.10.23			
						||(( ComBuf == Comm.Buf3 )&&(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 ))))	//新国网	//13.10.23
					{																																	//新国网	//13.10.23					
#if ( GPRSComm == YesCheck	 ) 				//带GPRS通讯
//																				//新国网	//14.06.23
//						if(( *(ComBuf+NRs_PassWPtr) == 0x98 )||( *(ComBuf+NRs_PassWPtr) == 0x99 ))	//用一类或2类数据方式不能设置无线场强				//新国网	//13.10.24												
//						{																																//新国网	//13.10.24												
//	            			*(ComBuf+Rs_CtlPtr) = 0xD4;																									//新国网	//13.10.24			
//    	        			*(ComBuf+Rs_LenPtr) = 1;            																						//新国网	//13.10.24					
//        	    			*(ComBuf+Rs_IDLPtr) = RS_State_PswE;            			            													//新国网	//13.10.24												
//						}																																//新国网	//13.10.24												
//						else																															//新国网	//13.10.24													
//						{																																//新国网	//13.10.24												
							Para.NetIntensity = *(ComBuf+NRs_WDataPtr);		//无线信号强弱																//新国网	//13.10.24
         					*(ComBuf+Rs_CtlPtr) = 0x94;																									//新国网	//13.10.23
         					*(ComBuf+Rs_LenPtr) = 0;            																						//新国网	//13.10.23			
//         				}														//新国网	//14.06.23													//新国网	//13.10.24												
#else																																					//新国网	//13.10.24			
            			*(ComBuf+Rs_CtlPtr) = 0xD4;																										//新国网	//13.10.24			
            			*(ComBuf+Rs_LenPtr) = 1;            																							//新国网	//13.10.24					
            			*(ComBuf+Rs_IDLPtr) = RS_State_IVID;            			            														//新国网	//13.10.24					
#endif																																					//新国网	//13.10.24					
					}else *ComBuf = 0xFF;																												//新国网	//13.10.23
					
//																		//新国网	//13.10.23	
//					if( ComBuf == Comm.Buf3 ) 
//					{
//						Para.NetIntensity = *(ComBuf+NRs_WDataPtr);		//无线信号强弱	
//         				*(ComBuf+Rs_CtlPtr) = 0x94;
//         				*(ComBuf+Rs_LenPtr) = 0;            			
//         			}
//         			else
//         			{
//            			*(ComBuf+Rs_CtlPtr) = 0xD4;
//            			*(ComBuf+Rs_LenPtr) = 1;            			
//            			*(ComBuf+Rs_IDLPtr) = RS_State_IVID;            			            			
//         			}
					break;	
				}
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
				else if( LComID == 0x04001503 )					//复位主动上报状态位	//新国网	//14.04.26
				{
					if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )																						//新国网	//14.04.26			
					{																																	//新国网	//14.04.26					
						if( IsInfraredIdentityValid( ComBuf, 0xD4 ) != 0 ) break;		//新国网	//14.06.21

//																				//新国网	//14.06.23
//						if(( *(ComBuf+NRs_PassWPtr) == 0x98 )||( *(ComBuf+NRs_PassWPtr) == 0x99 ))	//用一类或2类数据方式不能复位主动上报状态位			//新国网	//14.04.26												
//						{																																//新国网	//14.04.26												
//	            			*(ComBuf+Rs_CtlPtr) = 0xD4;																									//新国网	//14.04.26			
//    	        			*(ComBuf+Rs_LenPtr) = 1;            																						//新国网	//14.04.26					
//        	    			*(ComBuf+Rs_IDLPtr) = RS_State_PswE;            			            													//新国网	//14.04.26												
//						}																																//新国网	//14.04.26												
//						else																															//新国网	//14.04.26													
//						{																																//新国网	//14.04.26												
							if(*(ComBuf+Rs_LenPtr) != 24)	
							{
		           				*(ComBuf+Rs_CtlPtr) = 0xD4;
    		       				*(ComBuf+Rs_LenPtr) = 1;            			
    	    	   				*(ComBuf+Rs_IDLPtr) = RS_State_IVData; 	//长度不对
								break;						
							}
							
//							if( AllResetBitIsAlreadyUpState( ComBuf+NRs_WDataPtr ) != 0 )	//新国网	//14.06.21
//							if( SM.ClearUpStateDelay == 0 )				//是否已读过主动上报状态字		//新国网	//14.06.28	
							if(( SM.ClearUpStateDelay == 0 )||( AllResetBitIsAlreadyUpState( ComBuf+NRs_WDataPtr ) != 0 ))		//7号补遗				//新国网	//15.03.05
							{
		           				*(ComBuf+Rs_CtlPtr) = 0xD4;
    		       				*(ComBuf+Rs_LenPtr) = 1;            			
    	    	   				*(ComBuf+Rs_IDLPtr) = RS_State_PswE; 	//复位标志位未上报过
								break;						
							}

							CommResetEventUpState( ComBuf+NRs_WDataPtr );		//复位主动上报状态位													//新国网	//14.04.26
         					ResetEventUpStateSend( ComBuf+NRs_WDataPtr );		//新国网	//14.06.21
//         					SM.ClearUpStateDelay = 0;																									//新国网	//14.04.28
							E2P_RData( ComBuf+NRs_WDataPtr+12, EventUpStateSend, 12 );											//7号补遗				//新国网	//15.03.05
							if( IsAllData( ComBuf+NRs_WDataPtr+12, 12, 0x00 ) == 0 ) SM.ClearUpStateDelay = 0;					//7号补遗				//新国网	//15.03.05		//未复位完全，剩余位继续延时
         					*(ComBuf+Rs_CtlPtr) = 0x94;																									//新国网	//14.04.26
         					*(ComBuf+Rs_LenPtr) = 0;            																						//新国网	//14.04.26			
//         				}														//新国网	//14.06.23																		//新国网	//14.04.26												
					}else *ComBuf = 0xFF;																												//新国网	//14.04.26				
					break;	
				}
#endif
*/     			

//#if ( GPRSComm == YesCheck	 ) 				//带GPRS通讯		//新国网
//				if(( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )||((IsWriteGPRSNetState(ComBuf) == 0)&&(IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 )))
//#else																
				if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )		
//#endif															
				{
/*
					if( IsInfraredIdentityValid( ComBuf, 0xD4 ) != 0 ) break;		//新国网	 					
					
//#if ( GPRSComm == YesCheck	 ) 				//带GPRS通讯		//新国网
//					if(( *(ComBuf+NRs_PassWPtr) == 0x98 )&&(IsWriteGPRSNetState(ComBuf) != 0))		//该权限用于费控表的第二类数据
//#else																
					if( *(ComBuf+NRs_PassWPtr) == 0x98 )		//该权限用于费控表的第二类数据
//#endif																	
					{
						
						*BTime =MS4000;
						Temp = NCom_RemoteWrite(ComBuf);
						*BTime =MS500;
	      	   			if( Temp == 0 )
	      	   			{
	         				*(ComBuf+Rs_CtlPtr) = 0x94;
	         				*(ComBuf+Rs_LenPtr) = 0;            			
	         			}
	         			else
	         			{
	            			*(ComBuf+Rs_CtlPtr) = 0xD4;
	            			*(ComBuf+Rs_LenPtr) = 1;            			
	            			*(ComBuf+Rs_IDLPtr) = Temp;            			            			
	         			}
            			break;        			            				         				
					}

//#if ( GPRSComm == YesCheck	 ) 				//带GPRS通讯		//新国网
//					if(( *(ComBuf+NRs_PassWPtr) == 0x99 )&&(IsWriteGPRSNetState(ComBuf) != 0))		//该权限用于费控表的第一类数据
//#else																
					if( *(ComBuf+NRs_PassWPtr) == 0x99 )		//该权限用于费控表的第一类数据
//#endif															
					{
						*BTime =MS4000;
						Temp = NCom_RemoteWrite1(ComBuf);
						*BTime =MS500;
	      	   			if( Temp == 0 )
	      	   			{
	         				*(ComBuf+Rs_CtlPtr) = 0x94;
	         				*(ComBuf+Rs_LenPtr) = 0;            			
	         			}
	         			else
	         			{
	            			*(ComBuf+Rs_CtlPtr) = 0xD4;
	            			*(ComBuf+Rs_LenPtr) = 1;            			
	            			*(ComBuf+Rs_IDLPtr) = Temp;            			            			
	         			}
            			break;        			            				         				
					}
*/
				
//#if ( GPRSComm == YesCheck	 ) 				//带GPRS通讯		//新国网
//					if(( IsWriteGPRSNetState(ComBuf) == 0 )||(( Password_Comp( ComBuf+NRs_PassWPtr ) == 0 )&&(( *(ComBuf+NRs_PassWPtr) == 2 )||( *(ComBuf+NRs_PassWPtr) == 4 ))))	
//#else
//					if(( Password_Comp( ComBuf+NRs_PassWPtr ) == 0 )&&(( *(ComBuf+NRs_PassWPtr) == 2 )||( *(ComBuf+NRs_PassWPtr) == 4 )))	
//#endif
//					if(( PIN_SETEN & P_SETEN ) == 0 )				//新国网
//					if((( PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))	//新国网	//14.04.09
					if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))
					{	
//						Temp = NCom_Write(ComBuf);
						Temp = RS_State_PswE;
					}
					else Temp = RS_State_PswE;
      	   			if( Temp == 0 )
      	   			{
         				*(ComBuf+Rs_CtlPtr) = 0x94;
         				*(ComBuf+Rs_LenPtr) = 0;            			
         			}
         			else
         			{
            			*(ComBuf+Rs_CtlPtr) = 0xD4;
            			*(ComBuf+Rs_LenPtr) = 1;            			
            			*(ComBuf+Rs_IDLPtr) = Temp;            			            			
         			}
         		} else *ComBuf = 0xFF;
      			break;            		//N645
     	case NRS_CTL_WrMetID:			//N645
//																	//新国网     	
//     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
//     			if((( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 )&&((( PIN_SETEN & P_SETEN ) == 0 )||(( SM.PrgDly != 0 )&&((SM.SetCloseDelay2==0)||(SM.SetCloseDelay4==0)))))&&(*(ComBuf+Rs_LenPtr) == 0x06))
//     			{
//     				*(ComBuf+Rs_CtlPtr) = 0x95;
//     				E2P_WData( EMeter_No, ComBuf+Rs_IDLPtr, 6 );	     				    				
//     				*(ComBuf+Rs_LenPtr) = 0;
//     			}else *ComBuf = 0xFF;	
				*ComBuf = 0xFF;										//新国网
      			break;					//N645
     	case NRS_CTL_Freeze:			//N645
/*     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
				if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||( Addr_9_9(ComBuf+Rs_Addr0Ptr) == 0 ))		
				{
					if( IsInfraredIdentityValid( ComBuf, 0xD6 ) != 0 ) 					//新国网
					{
						if( Addr_9_9(ComBuf+Rs_Addr0Ptr) == 0 ) *ComBuf = 0xFF;	            				
						break;		//新国网	 					
					}	
					
					if((*(ComBuf+Rs_LenPtr) != 0x04)||( AllIsBCD( ComBuf+Rs_IDLPtr, 4 ) != 0 )
						||( JudgeFreezeDateTime( ComBuf+Rs_IDLPtr ) != 0 ))
					{
            			*(ComBuf+Rs_CtlPtr) = 0xD6;
            			*(ComBuf+Rs_LenPtr) = 1;            			
            			*(ComBuf+Rs_IDLPtr) = RS_State_IVData;            		
            			if( Addr_9_9(ComBuf+Rs_Addr0Ptr) == 0 ) *ComBuf = 0xFF;	            				
            			break;
					}
					
					if( IsAllData( ComBuf+Rs_IDLPtr, 4, 0x99 ) == 0 ) CreateFreezeRec( InstantFreeze_No );
					else E2P_WData( FreezeTimePara, ComBuf+Rs_IDLPtr, 4 );
       				*(ComBuf+Rs_CtlPtr) = 0x96;
       				*(ComBuf+Rs_LenPtr) = 0; 
       				if( Addr_9_9(ComBuf+Rs_Addr0Ptr) == 0 ) 
       				{
       					*ComRecTmr = 2;				
       					*ComBuf = 0xFF;          			
       				}
         		} else *ComBuf = 0xFF;
*/
      			break;					//N645
     	case NRS_CTL_ChgBaut:			//N645
/*     			
			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
//     		if(( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||(( PIN_SETEN & P_SETEN ) == 0 ))	
      		if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0)	
      		{
				if(*(ComBuf+Rs_LenPtr) != 0x01)
				{
           			*(ComBuf+Rs_CtlPtr) = 0xD7;
           			*(ComBuf+Rs_LenPtr) = 1;            			
           			*(ComBuf+Rs_IDLPtr) = RS_State_IVData;            			            				
           			break;
				}

      			if( JustBandRate( *(ComBuf+Rs_IDLPtr) ) != 0 )
      			{
             		*(ComBuf+Rs_CtlPtr) = 0xD7;
            		*(ComBuf+Rs_LenPtr) = 1;            			
            		*(ComBuf+Rs_IDLPtr) = RS_State_IVData; 	//通讯速率不能修改
            		break;           			            			
      			}	

//#if ( CarryComm == YesCheck )											//新国网
//     			if(( ComBuf == Comm.Buf0 )||( ComBuf == Comm.Buf3 ))	//远红外及载波口不能修改波特率	
//#else
      			if( ComBuf == Comm.Buf0 )
//#endif
      			{
             		*(ComBuf+Rs_CtlPtr) = 0xD7;
            		*(ComBuf+Rs_LenPtr) = 1;            			
            		*(ComBuf+Rs_IDLPtr) = NRS_State_Baut; 	//通讯速率不能修改
            		break;           			            			
      			}	

      			*(ComBuf+Rs_CtlPtr) = 0x97;
      			switch( *(ComBuf+Rs_IDLPtr))
      			{
      				case Baut600:  
      				case Baut1200: 
      				case Baut2400: 
      				case Baut4800: 
      				case Baut9600: 
      				case Baut19200:
      					if( ComBuf == Comm.Buf1 )
      					{ 
      						Comm.BandRate1 = *(ComBuf+Rs_IDLPtr); 
      						E2P_WData( RS485BaudRate1, ComBuf+Rs_IDLPtr, 1 );
							break; 
						}      								
      					else if( ComBuf == Comm.Buf2 )
      					{ 
      						Comm.BandRate2 = *(ComBuf+Rs_IDLPtr); 
      						E2P_WData( RS485BaudRate2, ComBuf+Rs_IDLPtr, 1 );
							break; 
						}      								
      					else if( ComBuf == Comm.Buf3 )
      					{ 
      						Comm.BandRate3 = *(ComBuf+Rs_IDLPtr); 
      						E2P_WData( RS485BaudRate3, ComBuf+Rs_IDLPtr, 1 );
      						break; 
      					}
					default:*(ComBuf+Rs_IDLPtr) = 0xFF;break;		
				}	
      		}else *ComBuf = 0xFF;	
*/
				*ComBuf = 0xFF;
      			break;					//N645
     	case NRS_CTL_ChgPswd:			//N645
     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
			if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )
			{	         		
//																//新国网	
//				if(*(ComBuf+Rs_LenPtr) != 0x0C )				//新国网			
//				{
//           			*(ComBuf+Rs_CtlPtr) = 0xD8;
//           			*(ComBuf+Rs_LenPtr) = 1;            			
//           			*(ComBuf+Rs_IDLPtr) = RS_State_IVData; 	//长度不对
//           			break;           			            			
//				}	
//
//				if((( PIN_SETEN & P_SETEN ) != 0 )&&( SM.PrgDly == 0 ))
//				{
//           			*(ComBuf+Rs_CtlPtr) = 0xD8;
//           			*(ComBuf+Rs_LenPtr) = 1;            			
//           			*(ComBuf+Rs_IDLPtr) = RS_State_PswE; 	//无编程权限
//           			break;           			            			
//				}					
//				
//				if( AllIsBCD( ComBuf+NRs_PassWPtr, 8 ) != 0 )
//				{
//           			*(ComBuf+Rs_CtlPtr) = 0xD8;
//           			*(ComBuf+Rs_LenPtr) = 1;            			
//           			*(ComBuf+Rs_IDLPtr) = RS_State_IVData; 	//非法数据
//           			break;           			            			
//				}	
//
//				if(( *(ComBuf+NRs_PassWPtr+4) <= 0x09 )&&( (*(ComBuf+NRs_PassWPtr+4)+1) == *(ComBuf+NRs_ID0Ptr) )
//				&&( *(ComBuf+NRs_ID1Ptr) == 0x0C )&&( *(ComBuf+NRs_ID2Ptr) == 0x00 )&&( *(ComBuf+NRs_ID3Ptr) == 0x04 )
//				&&( Password_Comp( ComBuf+NRs_PassWPtr ) == 0 )&&( *(ComBuf+NRs_PassWPtr) <= *(ComBuf+NRs_PassWPtr+4) ))		//V2
//				{				
//					E2P_WData( Password0+*(ComBuf+NRs_PassWPtr+4)*5, ComBuf+NRs_PassWPtr+4, 4 ); 
//					RAM_Write( ComBuf+NRs_ID0Ptr, ComBuf+NRs_PassWPtr+4, 4 );
//        	    	*(ComBuf+Rs_CtlPtr) = 0x98;
//           			*(ComBuf+Rs_LenPtr) = 4;            			
//           		}
//           		else
//           		{
	           			*(ComBuf+Rs_CtlPtr) = 0xD8;				//新国网
    	       			*(ComBuf+Rs_LenPtr) = 1;            			
        	   			*(ComBuf+Rs_IDLPtr) = RS_State_PswE; 	//无编程权限
//           		}		
         	}else *ComBuf = 0xFF; 	
      			break;					//N645
     	case NRS_CTL_RstMD:				//N645
/*     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
     	
				if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )
				{
					if( IsInfraredIdentityValid( ComBuf, 0xD9 ) != 0 ) break;		//新国网

					if( *(ComBuf+NRs_ID0Ptr) == 0x98 )					//新国网
					{
						*BTime =MS4000;
						Temp = ClearCodeDecrypt( ComBuf );
						*BTime =MS500;
	      	   			if( Temp == 0 )
	      	   			{
							CreateMDClrRec( ComBuf+NRs_RDataPtr );								
    	     				SM.MDClrState = MDClrStep1;							//进入需量清零流程	
	         			}
					}													//新国网
					else												//新国网		
					{													//新国网	
						if(*(ComBuf+Rs_LenPtr) != 0x08)	
						{
		           			*(ComBuf+Rs_CtlPtr) = 0xD9;
    		       			*(ComBuf+Rs_LenPtr) = 1;            			
    	    	   			*(ComBuf+Rs_IDLPtr) = RS_State_IVData; 	//长度不对
							break;						
						}
								
//						if((( PIN_SETEN & P_SETEN ) != 0 )&&( SM.PrgDly == 0 ))		//新国网
//						if(( PIN_SETEN & P_SETEN ) != 0 )							//新国网
//						if((( PIN_SETEN & P_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))	//新国网	//14.04.09
						if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))
									{
		           			*(ComBuf+Rs_CtlPtr) = 0xD9;
    		       			*(ComBuf+Rs_LenPtr) = 1;            			
    	    	   			*(ComBuf+Rs_IDLPtr) = RS_State_PswE; 	//无编程权限
							break;
						}	
    	
						if(( Password_Comp( ComBuf+NRs_ID0Ptr ) == 0 )&&(( *(ComBuf+NRs_ID0Ptr) == 2 )||( *(ComBuf+NRs_ID0Ptr) == 4 )))
						{	
    	     				Temp = 0;
							CreateMDClrRec( ComBuf+NRs_RDataPtr );								
    	     				SM.MDClrState = MDClrStep1;							//进入需量清零流程	
						}
						else Temp = RS_State_PswE;
					}																//新国网
					*(ComBuf+Rs_LenPtr) = 0;
      	   			if( Temp == 0 )
      	   			{
         				*(ComBuf+Rs_CtlPtr) = 0x99;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
           				UpdateEventUpState( 82 );					//新国网	//14.04.28        				
#endif
         			}
         			else
         			{
         				*(ComBuf+Rs_CtlPtr) = 0xD9;            		
         				*(ComBuf+Rs_LenPtr) = 1;            					//新国网		//国网
            			*(ComBuf+Rs_IDLPtr) = Temp;            			        //新国网    	//国网			
         			}	
         		} else *ComBuf = 0xFF;
*/
				*ComBuf = 0xFF;
				break;		
     	case NRS_CTL_MeterClr:			//N645
/*
     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
   	
				if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )
				{
					if( IsInfraredIdentityValid( ComBuf, 0xDA ) != 0 ) break;		//新国网

					if( *(ComBuf+NRs_ID0Ptr) == 0x98 )					//新国网
					{
						*BTime =MS4000;
						Temp = ClearCodeDecrypt( ComBuf );
						*BTime =MS500;
	      	   			if( Temp == 0 )
	      	   			{
//         					if(( PIN_SETEN & P_SETEN ) == 0 ) ClrMeterClrRec();			//清除电表清零记录
         					if((( PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) ClrMeterClrRec();	//清除电表清零记录	//新国网	//14.04.09
         					RAM_Write( Comm.OperateCode, ComBuf+NRs_RDataPtr, 4 );
							SM.LoadAdjMode = 0x00;												//cru
							E2P_WData( ELoadAdjMode, (unsigned char*)&SM.LoadAdjMode, 1 );		//cru		
            	
#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#else																		//新国网	//13.11.06
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
							if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0x55;	//立即合闸		//新国网	//14.06.28
							ICComm.TripOFFDelay = 0;														//新国网	//14.06.28	
							ICComm.RelayOFFState = 0x00;		//收到远程直接合闸命令后，取消允许拉闸状态	//新国网	//14.06.28
							Para.RMeterTripState = 0x1B;													//新国网	//14.06.28		
							E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );			//新国网	//14.06.28		

							Para.RMeterAlarmState = 0x2B;													//新国网	//14.06.28				
							E2P_WData( MeterAlarmState, (unsigned char*)&Para.RMeterAlarmState, 1 );		//新国网	//14.06.28		
#endif
#endif
         					SM.MeterClrState = MeterClrStep1;							//进入电量清零流程	
         					SM.SumPluseECP0 = 0;				//10.10.06
	         			}
					}													//新国网
					else												//新国网		
					{													//新国网	
						if(*(ComBuf+Rs_LenPtr) != 0x08)
						{
	        	   			*(ComBuf+Rs_CtlPtr) = 0xDA;
    	    	   			*(ComBuf+Rs_LenPtr) = 1;            			
        		   			*(ComBuf+Rs_IDLPtr) = RS_State_IVData; 	//长度不对
							break;						
						}
						
//						if((( PIN_SETEN & P_SETEN ) != 0 )&&( SM.PrgDly == 0 ))		//新国网
//						if(( PIN_SETEN & P_SETEN ) != 0 )							//新国网
//						if((( PIN_SETEN & P_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))	//新国网	//14.04.09
						if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))
						{
	        	   			*(ComBuf+Rs_CtlPtr) = 0xDA;
    	    	   			*(ComBuf+Rs_LenPtr) = 1;            			
        		   			*(ComBuf+Rs_IDLPtr) = RS_State_PswE; 	//无编程权限
							break;
						}						
						
						if(( Password_Comp( ComBuf+NRs_ID0Ptr ) == 0 )&&( *(ComBuf+NRs_ID0Ptr) == 2 ))
						{	
         					Temp = 0;
//         					if(( PIN_SETEN & P_SETEN ) == 0 ) ClrMeterClrRec();			//清除电表清零记录
//         					if((( PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) ClrMeterClrRec();			//清除电表清零记录	//新国网	//14.04.09
	         				if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) ClrMeterClrRec();
         					RAM_Write( Comm.OperateCode, ComBuf+NRs_RDataPtr, 4 );
							SM.LoadAdjMode = 0x00;												//cru
							E2P_WData( ELoadAdjMode, (unsigned char*)&SM.LoadAdjMode, 1 );		//cru		
            	
#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#else																		//新国网	//13.11.06
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
							if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0x55;	//立即合闸		//新国网	//14.06.28
							ICComm.TripOFFDelay = 0;														//新国网	//14.06.28	
							ICComm.RelayOFFState = 0x00;		//收到远程直接合闸命令后，取消允许拉闸状态	//新国网	//14.06.28
							Para.RMeterTripState = 0x1B;													//新国网	//14.06.28		
							E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );			//新国网	//14.06.28		

							Para.RMeterAlarmState = 0x2B;													//新国网	//14.06.28				
							E2P_WData( MeterAlarmState, (unsigned char*)&Para.RMeterAlarmState, 1 );		//新国网	//14.06.28		
#endif
#endif
         					SM.MeterClrState = MeterClrStep1;							//进入电量清零流程	
         					SM.SumPluseECP0 = 0;				//10.10.06
						}
						else Temp = RS_State_PswE;
					}													//新国网
					*(ComBuf+Rs_LenPtr) = 0;
      	   			if( Temp == 0 )
      	   			{
         				*(ComBuf+Rs_CtlPtr) = 0x9A;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
            				ClearAllEventUpState();						//新国网	//14.04.28    
            				if( ICComm.RPrivateKeyFlag == 0) UpdateEventUpState( 81 );	//私钥		//新国网	//14.04.28        				
#endif
         			}
         			else 
         			{
         				*(ComBuf+Rs_CtlPtr) = 0xDA;            		
            			*(ComBuf+Rs_LenPtr) = 1;            			
            			*(ComBuf+Rs_IDLPtr) = Temp;            			            				
         			}	
         		} else *ComBuf = 0xFF;
*/				
				*ComBuf = 0xFF;
				break;		
     	case NRS_CTL_EventClr:			//N645
/*   			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
//																		//新国网			     			
//    			if( (( PIN_SETEN & P_SETEN ) != 0 )&&(SM.SetCloseDelay2!=0)&&( *(ComBuf+NRs_ID0Ptr) == 2 ))
//     			{
//					if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )
//					{
//						*(ComBuf+Rs_CtlPtr) = 0xDB;
//            			*(ComBuf+Rs_LenPtr) = 1;            			
//            			*(ComBuf+Rs_IDLPtr) = RS_State_PswE;           			            			
//					}
//					else *ComBuf = 0xFF;		//各级密码单独闭锁	
//     				break;
//     			}	
     	
				if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )	
				{
					if( IsInfraredIdentityValid( ComBuf, 0xDB ) != 0 ) break;		//新国网

					if( *(ComBuf+NRs_ID0Ptr) == 0x98 )					//新国网
					{
						*BTime =MS4000;
						Temp = ClearCodeDecrypt( ComBuf );
						*BTime =MS500;
	      	   			if( Temp == 0 )
	      	   			{
	         				RAM_Write( Comm.OperateCode, ComBuf+NRs_RDataPtr, 4 );
							Temp = AllEventClr( ComBuf+NRs_OPCodePtr+8, 1 );
	         			}
					}													//新国网
					else												//新国网		
					{													//新国网	
						if(*(ComBuf+Rs_LenPtr) != 0x0C)
						{
		           			*(ComBuf+Rs_CtlPtr) = 0xDB;
	    	       			*(ComBuf+Rs_LenPtr) = 1;            			
	        	   			*(ComBuf+Rs_IDLPtr) = RS_State_IVData; 	//长度不对
							break;						
						}	
											
//						if((( PIN_SETEN & P_SETEN ) != 0 )&&( SM.PrgDly == 0 ))		//新国网
//						if(( PIN_SETEN & P_SETEN ) != 0 )							//新国网
//						if((( PIN_SETEN & P_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))			//新国网	//14.04.09
						if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))
						{
		           			*(ComBuf+Rs_CtlPtr) = 0xDB;
	    	       			*(ComBuf+Rs_LenPtr) = 1;            			
	        	   			*(ComBuf+Rs_IDLPtr) = RS_State_PswE; 	//无编程权限
							break;
						}	
						
						if(( Password_Comp( ComBuf+NRs_ID0Ptr ) == 0 )&&( *(ComBuf+NRs_ID0Ptr) == 2 ))
						{	
	         				RAM_Write( Comm.OperateCode, ComBuf+NRs_RDataPtr, 4 );
							Temp = AllEventClr( ComBuf+NRs_OPCodePtr, 1 );
						}
						else Temp = RS_State_PswE;
					}													//新国网		
					*(ComBuf+Rs_LenPtr) = 0;
      	   			if( Temp == 0 )
      	   			{
         				*(ComBuf+Rs_CtlPtr) = 0x9B;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
            				UpdateEventUpState( 83 );					//新国网	//14.04.28        				
#endif
         			}
         			else 
         			{
         				*(ComBuf+Rs_CtlPtr) = 0xDB;            		
            			*(ComBuf+Rs_LenPtr) = 1;            			
            			*(ComBuf+Rs_IDLPtr) = Temp;            			            				         				
         			}	
         		} else *ComBuf = 0xFF;
*/	
				*ComBuf = 0xFF;
				break;		
#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#else																		//新国网	//13.11.06
		case NRS_CTL_Trip:										//跳合闸、报警、保电	
/*
     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
//																		//新国网			     			
//    			if( (( PIN_SETEN & P_SETEN ) != 0 )&&(SM.SetCloseDelay2!=0)&&( *(ComBuf+NRs_ID0Ptr) == 2 ))
//     			{
//					if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )
//					{
//						*(ComBuf+Rs_CtlPtr) = 0xDC;
//            			*(ComBuf+Rs_LenPtr) = 1;            			
//            			*(ComBuf+Rs_IDLPtr) = RS_State_PswE;           			            			
//					}
//					else *ComBuf = 0xFF;		//各级密码单独闭锁	
//     				break;
//     			}	

				if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )
				{
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
					if(( *(ComBuf+NRs_ID0Ptr) == 0x98 )&&( IsInfraredIdentityValid( ComBuf, 0xDC ) != 0 )) break;		//新国网
#else
					if( IsInfraredIdentityValid( ComBuf, 0xDC ) != 0 ) break;		//新国网
#endif

					if( *(ComBuf+NRs_ID0Ptr) == 0x98 )		//该权限用于费控表
					{
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
						*BTime =MS4000;
						switch( ControlCodeDecrypt( ComBuf, (unsigned short*)&Temp3 ) )
						{
							case -2:
		         				*(ComBuf+Rs_CtlPtr) = 0xDC;            		
		            			*(ComBuf+Rs_LenPtr) = 2;            			
	    	        			RAM_Write( ComBuf+Rs_IDLPtr, (unsigned char*)&Temp3, 2 );            			            				         				
								break;	
							case -1:
		         				*(ComBuf+Rs_CtlPtr) = 0xDC;            		
		            			*(ComBuf+Rs_LenPtr) = 1;            			
	    	        			*(ComBuf+Rs_IDLPtr) = Temp3;
								break;	
							case 0:							
		         				*(ComBuf+Rs_CtlPtr) = 0x9C;
		         				*(ComBuf+Rs_LenPtr) = 0;
								break;	
							case 2:
		         				*(ComBuf+Rs_CtlPtr) = 0x9C;            		
		            			*(ComBuf+Rs_LenPtr) = 2;            			
	    	        			RAM_Write( ComBuf+Rs_IDLPtr, (unsigned char*)&Temp3, 2 );            			            				         											
								break;	
							default: break;							
						}	
						*BTime =MS500;
#else
						*BTime =MS4000;
						Temp = ControlCodeDecrypt( ComBuf );
						*BTime =MS500;
						*(ComBuf+Rs_LenPtr) = 0;
	      	   			if( Temp == 0 )
	      	   			{
	         				*(ComBuf+Rs_CtlPtr) = 0x9C;
	         			}
	         			else 
	         			{
	         				*(ComBuf+Rs_CtlPtr) = 0xDC;            		
	            			*(ComBuf+Rs_LenPtr) = 1;            			
	            			*(ComBuf+Rs_IDLPtr) = Temp;            			            				         				
	         			}	
#endif
					}
					else
					{			
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
//    					if(((( PIN_SETEN & P_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))&&(((SM.SetCloseDelay2!=0)&&( *(ComBuf+NRs_ID0Ptr) == 2 ))||((SM.SetCloseDelay4!=0)&&( *(ComBuf+NRs_ID0Ptr) == 4 ))))		//14.06.19
//    					if((( PIN_SETEN & P_SETEN ) != 0 )&&(((SM.SetCloseDelay2!=0)&&( *(ComBuf+NRs_ID0Ptr) == 2 ))||((SM.SetCloseDelay4!=0)&&( *(ComBuf+NRs_ID0Ptr) == 4 ))))		//14.06.19	//14.06.23
    					if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&(((SM.SetCloseDelay2!=0)&&( *(ComBuf+NRs_ID0Ptr) == 2 ))||((SM.SetCloseDelay4!=0)&&( *(ComBuf+NRs_ID0Ptr) == 4 ))))		//14.06.19	//14.06.23
     					{
							*(ComBuf+Rs_CtlPtr) = 0xDC;
            				*(ComBuf+Rs_LenPtr) = 1;            			
            				*(ComBuf+Rs_IDLPtr) = RS_State_PswE;           			            			
	   						break;
     					}	

						Temp2 = SM.FactoryState;					//保留原来的厂内或厂外状态			//14.06.23
						SM.FactoryState = 0x00;						//暂时置为厂外状态，以使密码有效	//14.06.23
						if(( Password_Comp( ComBuf+NRs_ID0Ptr ) == 0 )&&(( *(ComBuf+NRs_ID0Ptr) == 2 )||( *(ComBuf+NRs_ID0Ptr) == 4 )))	//14.06.16
						{	
							SM.FactoryState = Temp2;				//恢复原来的厂内或厂外状态		//14.06.23						

							Temp = 0x00;									//14.06.16
							if( JudgeContrlTime( ComBuf+NRs_OPCodePtr+2 ) != 0 )
							{ 
								Temp = RS_State_PswE;						//14.06.16	
							}	
							else
							{
								switch( *(ComBuf+NRs_OPCodePtr) )
								{
									case 0x1B:										//合闸允许			//新国网	//14.06.16
									case 0x1C:										//直接合闸			//新国网
										ICComm.PreTripOFFKeepTime = 0;
										if( *(ComBuf+NRs_OPCodePtr) == 0x1C )		//直接合闸
										{
											if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0x55;	//立即合闸
										}
										else										//合闸允许		
										{
#if ( RelayONImmediately == YesCheck )
											if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0x55;	//立即合闸
#else
											if( Para.RMeterRelayState == 0x1A ) ICComm.RelayONState = 0xAA;	//允许合闸
#endif											
										}		
										ICComm.RelayOFFState = 0x00;				//收到远程直接合闸命令后，取消允许拉闸状态	//新国网	//13.10.25
										ICComm.TripOFFDelay = 0;
										Para.RMeterTripState = 0x1B;				//新国网		//13.09.05
										E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );	//新国网		//13.09.05
										RAM_Write( Comm.TripONOperateCode, ComBuf+NRs_RDataPtr, 4 ); 	//新国网	//13.09.05
										E2P_WData( ETripONOperateCode, Comm.TripONOperateCode, 4 );		//新国网	//14.05.12
										Temp = 0;								//14.06.16
										break;
									default: 
										Temp = RS_State_PswE;
										break;			
								}	
							}	
						}
						else
						{
							SM.FactoryState = Temp2;					//恢复原来的厂内或厂外状态		//14.06.23						
							Temp = RS_State_PswE;								//14.06.16
						}	

//																				//14.06.16						
//						switch( Temp )
//						{
//							case 0xFE:
//		         				*(ComBuf+Rs_CtlPtr) = 0xDC;            		
//		            			*(ComBuf+Rs_LenPtr) = 2;            			
//	    	        			RAM_Write( ComBuf+Rs_IDLPtr, (unsigned char*)&Temp3, 2 );            			            				         				
//								break;	
//							case 0xFF:
//		         				*(ComBuf+Rs_CtlPtr) = 0xDC;            		
//		            			*(ComBuf+Rs_LenPtr) = 1;            			
//	    	        			*(ComBuf+Rs_IDLPtr) = Temp3;
//								break;	
//							case 0:							
//		         				*(ComBuf+Rs_CtlPtr) = 0x9C;
//		         				*(ComBuf+Rs_LenPtr) = 0;
//								break;	
//							case 2:
//		         				*(ComBuf+Rs_CtlPtr) = 0x9C;            		
//		            			*(ComBuf+Rs_LenPtr) = 2;            			
//	    	        			RAM_Write( ComBuf+Rs_IDLPtr, (unsigned char*)&Temp3, 2 );            			            				         											
//								break;	
//							default: break;							
//						}	
						*(ComBuf+Rs_LenPtr) = 0;								//14.06.16
	      	   			if( Temp == 0 )
	      	   			{
	         				*(ComBuf+Rs_CtlPtr) = 0x9C;
	         			}
	         			else 
	         			{
	         				*(ComBuf+Rs_CtlPtr) = 0xDC;            		
	            			*(ComBuf+Rs_LenPtr) = 1;            			
	            			*(ComBuf+Rs_IDLPtr) = Temp;            			            				         				
	         			}						
#else
//						if( (( PIN_SETEN & P_SETEN ) != 0 )
//						{									
         					*(ComBuf+Rs_CtlPtr) = 0xDC;            		
            				*(ComBuf+Rs_LenPtr) = 1;            			
            				*(ComBuf+Rs_IDLPtr) = RS_State_PswE;            			            				         				
//            			}	
//						else 
//						{
//							if(( Password_Comp( ComBuf+NRs_ID0Ptr ) == 0 )&&( *(ComBuf+NRs_ID0Ptr) == 2 ))
//							{	
//		         				RAM_Write( Comm.OperateCode, ComBuf+NRs_RDataPtr, 4 );
//		         				Temp = 0;
//
//								if( JudgeContrlTime( *(ComBuf+NRs_OPCodePtr+2 ) != 0 )
//								{ 
//									Temp |= RS_State_IVData;
//								}	
//								
//								switch( *(ComBuf+NRs_OPCodePtr) )
//								{
//									case 0x1A:										//跳闸
//										Para.RMeterTripState = *(ComBuf+NRs_OPCodePtr);
//										Addr = MeterTripState;
//										if( Para.RTripOFFAlarmTime == 0 )
//										{
//											ICComm.TripOFFDelay = 0;
//											ICComm.RelayDriveCnt = 3;
//											RelayDrive( RELAY_OFF );	
//											CreateTripRec( TripOFF_No );
//										}
//										else ICComm.TripOFFDelay = Para.RTripOFFAlarmTime;	
//										break;
//									case 0x1B:										//合闸允许	
//										Para.RMeterTripState = *WriteBufAds;
//										Addr = MeterTripState;
//										break;
//									case 0x2A:										//报警	
//										PDIR_RELBJ |= P_RELBJ;
//										Para.RMeterAlarmState = *WriteBufAds;
//										Addr = MeterAlarmState;
//										break;
//									case 0x2B:										//报警解除
//										PDIR_RELBJ &= ~P_RELBJ;
//										Para.RMeterAlarmState = *WriteBufAds;
//										Addr = MeterAlarmState;
//										break;
//									case 0x3A:										//保电
//									case 0x3B:										//保电解除
//										Para.RMeterSafeState = *WriteBufAds;
//										Addr = MeterSafeState;
//										break;			
//									default: 
//										Err_Byte |= RS_State_IVData; 
//										break;			
//								}	
//								if( Err_Byte == 0 ) E2P_WData( Addr, WriteBufAds, 1 );			
//							
//								if( *WriteBufAds == 0x1A ) RAM_Write( Comm.TripOFFOperateCode, ComBuf+NRs_RDataPtr, 4 ); 
//								else if( *WriteBufAds == 0x1B ) RAM_Write( Comm.TripONOperateCode, ComBuf+NRs_RDataPtr, 4 ); 
//								else RAM_Write( Comm.OperateCode, ComBuf+NRs_RDataPtr, 4 ); 
//								return Err_Byte;
//
//
//		         				switch( *(ComBuf+NRs_OPCodePtr) )
//		         				{
//		         					case 0x2A:						//报警
//		         						PDIR_RELAY1 |= P_RELAY1;
//		         						break;
//		         					case 0x2B:						//报警解除
//		         						PDIR_RELAY1 &= ~P_RELAY1;	         						
//		         						break;
//		         					case 0x1A:						//跳闸
//		         					case 0x1B:						//合闸允许
//		         					case 0x3A:						//保电
//		         					case 0x3B:						//保电解除	
//		         					default: 
//		         						Temp = RS_State_IVData;
//		         						break;	
//		         				}		         				
//							}
//							else Temp = RS_State_PswE;
//							*(ComBuf+Rs_LenPtr) = 0;
//		      	   			if( Temp == 0 )
//		      	   			{
//		         				*(ComBuf+Rs_CtlPtr) = 0x9C;
//		         			}
//		         			else 
//		         			{
//		         				*(ComBuf+Rs_CtlPtr) = 0xDC;            		
//		            			*(ComBuf+Rs_LenPtr) = 1;            			
//		            			*(ComBuf+Rs_IDLPtr) = Temp;            			            				         				
//		         			}	
//						}
#endif
	         		}	
				}else *ComBuf = 0xFF; 		
*/
				*ComBuf = 0xFF; 
#endif																		//新国网	//13.11.06			
				break;		
		case NRS_CTL_MutiPort:
/*
     			if( Comm.BatOnDly != 0 ) { *ComBuf = 0xFF; break; }
				if(( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0)||( Addr_9_9(ComBuf+Rs_Addr0Ptr) == 0 ))
				{
					if( IsInfraredIdentityValid( ComBuf, 0xDD ) != 0 ) break;		//新国网

//					if(((*(ComBuf+Rs_LenPtr) != 0x01)||( *(ComBuf+Rs_IDLPtr) > 2 ))&&( Addr_9_9(ComBuf+Rs_Addr0Ptr) != 0 ))
					if((*(ComBuf+Rs_LenPtr) != 0x01)||( *(ComBuf+Rs_IDLPtr) > 2 ))
					{
	           			*(ComBuf+Rs_CtlPtr) = 0xDD;
    	       			*(ComBuf+Rs_LenPtr) = 1;            			
        	   			*(ComBuf+Rs_IDLPtr) = RS_State_IVData; 	//长度不对数据非法
						break;						
					}	
					SM.RPulseOutMode = *(ComBuf+Rs_IDLPtr);
					E2P_WData( PulseOutMode, (unsigned char*)&SM.RPulseOutMode,1 );
					if( SM.RPulseOutMode == 0 ) 
					{
//						ComID = EN32K1Hz;		
						if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
						{
							ComID = EN32K1Hz_8025T;		
						}
						else ComID = EN32K1Hz_DS3231;			
#if ( NewHardScheme2 == YesCheck )		//10.12.25
#else
						PDIR_SECOUT |= P_SECOUT;		//V1
#endif
						SM.PulseWidthCnt = 0;
					}	
					else 
					{
//						ComID = EN32K;
						if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
						{
							ComID = EN32K_8025T;		
						}
						else ComID = EN32K_DS3231;			
					}
					HWTime( ComID );				//EC
					if( Addr_9_9(ComBuf+Rs_Addr0Ptr) != 0 )
					{
         				*(ComBuf+Rs_CtlPtr) = 0x9D;            		
            			*(ComBuf+Rs_LenPtr) = 1;            			
//            			*(ComBuf+Rs_IDLPtr) = 0x00;            			            				         				
            			*(ComBuf+Rs_IDLPtr) = SM.RPulseOutMode;            			            				         				
					}	
					else
					{
       					*ComRecTmr = 2;				
						*ComBuf = 0xFF; 												
					}	
				}else *ComBuf = 0xFF; 	
*/					
				*ComBuf = 0xFF; 
				break;		
     	case NRS_CTL_SAuRW:										//远程安全认证读、写数据
/*
//       		    if(( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )&&( ComBuf != Comm.Buf0 ))
//       		    if( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )
				if(( MeterAds_Match( ComBuf+Rs_Addr0Ptr ) == 0 )															//新国网	//13.10.12	
					||( (( MeterAds_Comp( ComBuf+Rs_Addr0Ptr ) == 0)||( IsAllData( ComBuf+Rs_Addr0Ptr, 6, 0xAA ) == 0 ))	//新国网	//13.10.12	
						&&(( LComID == 0x078001FF )||( LComID == 0x078002FF )||( LComID == 0x078102FF ))))					//新国网	//13.10.12	
				{
					if(( IsInfraredValid( ComBuf ) != 0 )&&( NeedInfraredIdentity( LComID ) == 0 ))			//新国网
					{
						Temp3 = 0x0001;							//其他错误
           				*(ComBuf+Rs_CtlPtr) = 0xC3;
           				*(ComBuf+Rs_LenPtr) = 2;            			
           				RAM_Write( ComBuf+Rs_IDLPtr, (unsigned char*)&Temp3, 2 );         			            			
						break;						
					}	
					
					*BTime =MS4000;
					Temp3 = NCom_SAuRW(ComBuf);
       				*BTime =MS500;
					if( Temp3 == 0 ) *(ComBuf+Rs_CtlPtr) = 0x83;
					else
					{
           				*(ComBuf+Rs_CtlPtr) = 0xC3;
           				*(ComBuf+Rs_LenPtr) = 2;            			
           				RAM_Write( ComBuf+Rs_IDLPtr, (unsigned char*)&Temp3, 2 );         			            			
       				}
     			} 
       			else *ComBuf = 0xFF;
*/
      			break;					//N645
   		default:	*ComBuf = 0xFF; 	
      			break;
	   	}
	} else 	*ComBuf = 0xFF; 	
}
////////////////////////////////////////////////////////////////////////////////////
/*
void GroupSetHoliday( unsigned char* Source )
{
	unsigned char Buff[210];
	unsigned short i;
	unsigned char* Ptr;
	
	Ptr = Buff;
	*(Source+4) = ChkNum( Source, 4 );
	for( i=0;i<40;i++ )
	{
		RAM_Write( Ptr+i*5, Source, 5 );		
	}	
	for( i=0;i<6;i++ )
	{
		WDTCTL = WDT_ARST_1000;
		E2Pwrite( HoliDAY1+i*200, Ptr, 200, DataEAds );	
		WDTCTL = WDT_ARST_1000;
		E2Pwrite( HoliDAY1+i*200+E2P_PGLEN, Ptr, 200, DataEAds );		
	}	
	WDTCTL = WDT_ARST_1000;
	E2Pwrite( HoliDAY1+1200, Ptr, 70, DataEAds );	
	WDTCTL = WDT_ARST_1000;
	E2Pwrite( HoliDAY1+1200+E2P_PGLEN, Ptr, 70, DataEAds );		
}	
*/

unsigned char NCom_ExtWrite(unsigned char* ComBuf )
{
	unsigned short	Addr, Info, Len;
  	unsigned char Err_Byte=0;
	short ItemAds;

	if( *(ComBuf+NRs_ID3Ptr) != 0x04 ) return RS_State_IVID;	//非参变量类不能写。
	
	ItemAds = NGetExtComVar( &Addr, &Info, &Len, ComBuf );
	if( ItemAds == -1 ) 
	{
		Err_Byte |= RS_State_IVID;
		return	Err_Byte;
	}

	if( ~Info & EN_W ) 
	{
		Err_Byte |= RS_State_IVData;
		*ComBuf = 0xFF;
		return	Err_Byte;
	}
	*(ComBuf + Rs_Head0Ptr) = 12;		//标识码+密码长度+操作者代码   第二个变量暂做地址缓存
	Err_Byte = NProc_WriteExt( ItemAds, Addr, Info, Len, ComBuf );
	*(ComBuf + Rs_Head0Ptr) = 0x68;
	return	Err_Byte;
}

unsigned char NProc_WriteExt( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned char* WriteBufAds;
  	unsigned char Err_Byte=0;
  	unsigned long ComID;
//  	unsigned short i;

	WriteBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_Head0Ptr);
	
	ComID = ExtComTab[ ItemAds ].ComID;
	
	switch( Info & Comm_Type )
	{
		case OTHER:														//10.10.06	
/*				 if( ComID == 0x04B00509 )	//10.10.06
				{
					RAM_Write( (unsigned char*)&SM.Ib00IrmsDelay, WriteBufAds, 4 );	//10.10.06
					SM.IbIrmsChk = ChkNum( (unsigned char*)&SM.Ib00IrmsDelay, 4 );	//10.10.06
       				RAM_Fill( WriteBufAds, 9 );
       				E2P_WData( EIbMesuValue, WriteBufAds, 9 );
       				*(ComBuf + Rs_LenPtr) -= Len;					
				}
				else
*/				if(( ComID == 0x04B00832 )||( ComID == 0x04B00818 ))	//10.10.06
				{
					RAM_Write( (unsigned char*)&SM.SumPluseECP0, WriteBufAds, 4 );	//10.10.06
       				*(ComBuf + Rs_LenPtr) -= Len;					
				}			
				break;
		case PARAM:		
				if( ComID == 0x04B00845 )
				{
//					GroupSetHoliday( WriteBufAds );
					;	
				}		
#if ( MEASCHIP == ADE7758 )	
#elif ( MEASCHIP == ADE7878 )	
				else if( ComID == 0x04B00840 ) Err_Byte = ComAdjWrite( ComBuf );
#else
#endif
//				else if( ComID == 0x04B00842 )		//写EEPROM数据		//测试用
//				{
//					if( *WriteBufAds == 0 ) Dev_Ads = AdjEAds; 
//					else if( *WriteBufAds == 1 ) Dev_Ads = FMAds; 
//					else Dev_Ads = DataEAds; 
//					EAddr = *(WriteBufAds+2)*256+*(WriteBufAds+1);
//					Len = *(WriteBufAds+3);
//					if( Len >= 196 ) Len = 196;
//					E2Pwrite( EAddr, WriteBufAds+4, Len, Dev_Ads );
//				}	
				else if( ComID == 0x04B00830 )				//厂内、厂外状态	//10.05.13
				{
					E2P_RData( WriteBufAds+4, EProductState, 2 ); 					
//					if((( PIN_SETEN & P_SETEN ) != 0 )&&( *WriteBufAds == 0x5A )&&( *(WriteBufAds+1) == 0xA5 ))
					if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&( *WriteBufAds == 0x5A )&&( *(WriteBufAds+1) == 0xA5 ))
					{
						Err_Byte |= RS_State_IVData;	
					}
					else
					{
						if(( *(WriteBufAds+4) == 0x5A )&&( *(WriteBufAds+5) == 0xA5 )&&(Data_Comp( WriteBufAds, WriteBufAds+4, 2 ) != 0 ))
						{
//							ClrMeterClrRec();		//清除电表清零记录 10.05.13
						}		
						E2P_WData( EProductState, WriteBufAds, 2 );
					}		
				}	
#if ( RemoteEnterCalib == YesCheck )								//新国网	//14.05.23
				else if( ComID == 0x04B00835 )				//进入或退出校表状态	0x5566为进入，非0x5566为退出校表状态
				{
					if(( *WriteBufAds == 0x66 )&&( *(WriteBufAds+1) == 0x55 ))
					{
						if( ICComm.RPrivateKeyFlag == 0 ) Err_Byte |= RS_State_PswE;	//私钥下不能进入远程校表状态
						else
						{						
							SM.RemoteCalibState = 0x55;
							SM.CalibCount = CALIBCOUNT;	
						}	
					}
					else 
					{
						SM.CalibCount = 0;	
						SM.RemoteCalibState = 0;
					}			
				}
#endif
#if ( FlashTypeSelect == YesCheck )		//支持Flash类型设置		//新国网	//14.06.04	
				else if( ComID == 0x04B00836 )								//V4
				{
					if((( *WriteBufAds == 0x64 )&&( IsFlashType32AndBelow() == 0 ))
						||(( *WriteBufAds == 0x32 )&&( IsFlashType64() == 0 )))
					{
						Err_Byte |= RS_State_IVData;						//10.07.02
						break;												//10.07.02		
					}
					E2P_WData( Addr, WriteBufAds, 1 );		
				}	
#endif
				else if( ComID == 0x04B00847 )								//V4
				{
					SM.ZoneNo = *WriteBufAds;
					SM.SegTabNo = *(WriteBufAds+1);
//					SM.FeeTabNo = *(WriteBufAds+2);
//					SM.StepTabNo = *(WriteBufAds+3);
					SM.FeeTabNo = 0;                                        //新国网
					SM.StepTabNo = 0;                                       //新国网
					E2P_WData( EZoneNo, (unsigned char*)&SM.ZoneNo, 1 );
					E2P_WData( ESegTabNo, (unsigned char*)&SM.SegTabNo, 1 );
					E2P_WData( EFeeTabNo, (unsigned char*)&SM.FeeTabNo, 1 );
					E2P_WData( EStepTabNo, (unsigned char*)&SM.StepTabNo, 1 );
				}			
				else if( ComID == 0x04B00838 )	//文件系统格式化															
				{
					if(((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))&&( *WriteBufAds == 0x01 ))
					{
						File_Format();			//文件系统格式化
						DeletAllFreezeFile();	//删除曲线文件参数
					}	
					else
					{
						Err_Byte |= RS_State_PswE;						
					}
					break;
				}	
				else if( ComID == 0x04B000B7 )																
				{
//					if(( PIN_SETEN & P_SETEN ) != 0 )						//10.07.02
//					if((( PIN_SETEN & P_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))	//新国网	//14.04.09
					if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))
					{
						Err_Byte |= RS_State_IVData;						//10.07.02
						break;												//10.07.02		
					}
#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#else																		//新国网	//13.11.06
					if( *WriteBufAds == 0x01 )	RelayDrive( RELAY_OFF );
#endif																		//新国网	//13.11.06
					if( *WriteBufAds == 0x00 )
					{
						Para.RMeterTripState = 0x55;		//已合闸
						E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );
						Para.RMeterRelayState = 0x55;		//已合闸
						E2P_WData( MeterRelayState, (unsigned char*)&Para.RMeterRelayState, 1 );
						Para.RMeterAlarmState = 0x2B;
						E2P_WData( MeterAlarmState, (unsigned char*)&Para.RMeterAlarmState, 1 );
#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#else																		//新国网	//13.11.06
						RelayDrive( RELAY_ON );
#endif																		//新国网	//13.11.06
					}	
				}	
//				else if(( ComID >= 0x04B0FF4A )&&( ComID <= 0x04B0FF4F ))	//批设按显和轮显项	
//				{
//					for( i=0;i<33;i++ )
//					{
//						E2P_WData( Addr+i*6, WriteBufAds+i*5, 5 );
//					}	
//				}			
//				else if( ComID == 0x04B0FF50 )				//参数包
//				{
//					Err_Byte = SetManuPara1( WriteBufAds );						
//				}	
//				else if( ComID == 0x04B0FF51 )				//事件阀值参数包
//				{
//					Err_Byte = SetManuPara2( WriteBufAds, Len );						
//				}	
//				else if( ComID == 0x04B0FF52 )				//阶梯参数包	//新国网	
//				{
//					Err_Byte = SetManuPara3( WriteBufAds, Len );						
//				}	
//				else if(( ComID >= 0x04B0FF02 )&&( ComID <= 0x04B0FF05 ))	//时段表参数包1~4
//				{
//					Err_Byte = SetManuSeg( WriteBufAds, Addr );						
//				}	
				else if( ComID == 0x04B006FF )				//电表规格参数包	//新国网	//13.09.16
				{
					Err_Byte = SetMeterSpec( WriteBufAds, Len );						
				}					
//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T用7022内部温度传感器		//10.08.03		//新国网
				else if( ComID == 0x04B00505 ) 
				{
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#elif ( MEASCHIP == ATT7022E )							//新国网
#else
//#if ( NEW7022E == YesCheck )
//					UpdateTPSoffset();					//新国网
//#endif
					SM.RTempDelta = BCD2_Word( WriteBufAds );				
					ATT7022RdReg( ATTempD, WriteBufAds );					
					*(WriteBufAds+1) = 0;									
					i = (short)*WriteBufAds * 10;								
					SM.RTempDelta += i;
		       		E2P_WData( ETempDelta, (unsigned char*)&SM.RTempDelta, 2 );	
#endif
				}	
				else if( ComID == 0x04B000A9 )				//DS3231晶体老化偏置  			//新国网	//14.03.22
				{
   	   				if(( *WriteBufAds > 64 )&&( *WriteBufAds < 192 )) Err_Byte |= RS_State_IVData;		//新国网	//14.03.22
   	   				else
   	   				{
   	   					E2P_WData( Addr, WriteBufAds, Len );	//2的补码
//						if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
//						{
//							;
//						}
//						else
//						{
//		            		E2Pwrite( 0x10, WriteBufAds, 1, DS3231Ads );
//		            		E2PRead( WriteBufAds+1, 0x0E, 1, DS3231Ads );			
//      			    		*(WriteBufAds+1) |= 0x20;
//							E2Pwrite( 0x0E, WriteBufAds+1, 1, DS3231Ads );			
//						}
					}		
				}					
//#endif
//				else if( ComID == 0x04B00508 )				//写远程密钥状态	//12.05.28		//新国网
//				{
//					if(( PIN_SETEN & P_SETEN ) != 0 )						
//					{
//						Err_Byte |= RS_State_IVData;						
//						break;												
//					}					
//					E2P_WData( PassWord4State, WriteBufAds, 4 );		//0:公钥；1：私钥
//				}	
				else
				{
//#if ( MEASCHIP == ADE7758 )	
//#elif ( MEASCHIP == ADE7878 )	
//#else
//   					if( ComID == 0x04B00061 ) ReCalFailVoltageChk( WriteBufAds );				//ATChk
//#endif
   	   				E2P_WData( Addr, WriteBufAds, Len );
//#if ( MEASCHIP == ADE7758 )		
//#elif ( MEASCHIP == ADE7878 )	
//#else
//   					if( ComID == 0x04B00061 ) { SM.AT7022ChkSumCnt = 2; SetFailVoltage(); }		//ATChk
//#endif
				}
    
       			*(ComBuf + Rs_LenPtr) -= Len;
				break;
		default:break;	
	}
	return	Err_Byte;	
}


//////////////////////////////////////////////////////////////////////////
//自定义读
unsigned char NCom_ExtRead( unsigned char* ComBuf )
{
	unsigned short 	Addr, Info, Len;
  	unsigned char Err_Byte=0;
	short ItemAds;

	switch( *(ComBuf+NRs_ID3Ptr) )
	{
		case 0x04:
			ItemAds = NGetExtComVar( &Addr, &Info, &Len, ComBuf );
			if( ItemAds == -1 ) 
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;
			}
			if( ~Info & EN_R ) 
			{
				Err_Byte |= RS_State_IVData;
				return	Err_Byte;
			}
			*(ComBuf+Rs_LenPtr) = 4;
			NProc_ReadExt( ItemAds, Addr, Info, Len, ComBuf );				
			break;
//		case 0x05:						//冻结量		//自定义冻结数据不支持包抄
//			if(( *(ComBuf+NRs_ID0Ptr) == 0x00 )||( *(ComBuf+NRs_ID1Ptr) == 0xFF )||(( *(ComBuf+NRs_ID2Ptr) == 0xB0 )&&( *(ComBuf+NRs_ID0Ptr) > 0x3C )))
//			{
//				Err_Byte |= RS_State_IVID;
//				return	Err_Byte;			
//			}	
//			RAM_Write( ComBuf+NRs_ID0Ptr+4, ComBuf+NRs_ID0Ptr, 4 );
//			*(ComBuf+NRs_ID2Ptr+4) = 0x00;
//			ItemAds = NGetDJComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr+4 );			
//			if( ItemAds == -1 ) 
//			{
//				Err_Byte |= RS_State_IVID;
//				return	Err_Byte;
//			}
//			if( ~Info & EN_R ) 
//			{
//				Err_Byte |= RS_State_IVData;
//				return	Err_Byte;
//			}
//			*(ComBuf+Rs_LenPtr) = 4;
//			Err_Byte = NProc_ReadDJ( ItemAds, Addr, Info, Len, ComBuf );		
//			break;		
		default: 
			Err_Byte |= RS_State_IVID;
			break;	
	}
    return	Err_Byte;
}

//自定义参变量
short NGetExtComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComBuf )
{
	short i;
	unsigned long Value,Value2;
  	
  	RAM_Write( (unsigned char*)&Value, ComBuf+NRs_ID0Ptr, 4 );
  	Value2 = Value & 0xffffff00;
  	if(( Value2 == 0x04B00200 )||( Value2 == 0x04B00300 )) 		//循环显示项、按键显示项需特殊处理
  	{
  		if(( *(ComBuf+NRs_ID0Ptr) == 0x00 )||( *(ComBuf+NRs_ID0Ptr) == 0xFF )) return -1;
  		Value = Value2 + 1;
  	}	
  	
  	for( i=0;i<ExtComNum;i++ )
  	{
		if( Value == ExtComTab[i].ComID ) 
		{
			*Addr = ExtComTab[i].Addr;
			*Info = ExtComTab[i].ComInfo;
			*Len = *Info & RS_LEN;
			return i;
		}
  	}
	return -1;
}

/*
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
//禁止读取CPU的FLASH内容
void ProhibitReadCPU_FLASH( unsigned char* RAM_Addr, unsigned char* CPU_Addr, unsigned short Lenth )		//新国网	//14.06.21
{
//	unsigned short StAddr1,StAddr2;
//	unsigned short EndAddr1;
	unsigned short Addr;
	short i;
	
//	StAddr1 = 0x1000;			//5438的信息FLASH起始地址	
//	EndAddr1 = 0x1C00;
//	StAddr2 = 0x5C00;			//5438的FLASH起始地址
	
//	RAM_Write( RAM_Addr, CPU_Addr, Lenth );
	
	Addr = (unsigned short)CPU_Addr;
	for( i=0;i<Lenth;i++ )
	{
//		if((( Addr >= StAddr1 )&&( Addr < EndAddr1 ))||( Addr >= StAddr2 )) 
//		if((( Addr >= 0x1800 )&&( Addr < 0x1A00 ))||(( Addr >= 0x1C00 )&&( Addr < 0x5C00 ))) 
		if(( Addr < 0x1000 )||(( Addr >= 0x1800 )&&( Addr < 0x1A00 ))||(( Addr >= 0x1C00 )&&( Addr < 0x5C00 ))) 	//新国网	//14.06.28
		{
			*( RAM_Addr + i ) = *(CPU_Addr+i);
		}
		else *( RAM_Addr + i ) = 0xFF;		
		Addr += 1;
	}		
}	
#else
//禁止读取CPU的FLASH内容
void ProhibitReadCPU_FLASH( unsigned char* RAM_Addr, unsigned char* CPU_Addr, unsigned short Lenth )
{
	unsigned short StAddr1,StAddr2;
	unsigned short EndAddr1;
	unsigned short Addr;
	short i;
	
	StAddr1 = 0x1000;			//5438的信息FLASH起始地址	
	EndAddr1 = 0x1C00;
	StAddr2 = 0x5C00;			//5438的FLASH起始地址
	
	RAM_Write( RAM_Addr, CPU_Addr, Lenth );
	
	Addr = (unsigned short)CPU_Addr;
	for( i=0;i<Lenth;i++ )
	{
		if((( Addr >= StAddr1 )&&( Addr < EndAddr1 ))||( Addr >= StAddr2 )) 
		{
			*( RAM_Addr + i ) = 0xFF;
		}		
		Addr += 1;
	}		
}	
#endif
*/

//禁止读取密码内容
void ProhibitReadPassword( unsigned char* RAM_Addr, unsigned short E2P_Addr, unsigned short Lenth )
{
	unsigned short StAddr1,StAddr2;
	unsigned short EndAddr1,EndAddr2;
	unsigned short Addr;
	short i;
	
	StAddr1 = Password0;
	EndAddr1 = StAddr1 + 50;
	StAddr2 = Password0 + E2P_PGLEN;
	EndAddr2 = StAddr2 + 50;
	
	Addr = E2P_Addr;
	for( i=0;i<Lenth;i++ )
	{
		if((( Addr >= StAddr1 )&&( Addr < EndAddr1 ))||(( Addr >= StAddr2 )&&( Addr < EndAddr2 ))) 
		{
			*( RAM_Addr + i ) = 0xFF;
		}		
		Addr += 1;
	}		
}	

//读取内部状态
void GetInsideState( unsigned char* Source )					//10.08.25
{
	unsigned char Buff[2];											//11.07.19
	unsigned char* Ptr;												//11.07.19	
	
	Ptr = Buff;														//11.07.19

	RAM_Fill( Source, 8 );
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
	if(( Para.RMeterTripState == 0x1A )||( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E )) *Source |= 0x01;			//跳闸
#else
	if( Para.RMeterTripState == 0x1A ) *Source |= 0x01;			//跳闸
#endif
	if( Para.RMeterAlarmState == 0x2A ) *Source |= 0x02;		//报警
	if( Para.RMeterSafeState == 0x3A ) *Source |= 0x04;			//保电
//	if( SM.PrgDly != 0 ) *Source |= 0x08;						//编程
	if( ICEC.InfraredValidDelay != 0 ) *Source |= 0x08;			//红外认证有效	//新国网	//13.10.11
	if( SM.CoverOldState == OPEN_STATE ) *Source |= 0x10;       //开盖
	if( SM.TerminalOldState == OPEN_STATE ) *Source |= 0x20;    //开端
	if( ICEC.IdentityValidDelay != 0 ) *Source |= 0x40;			//身份认证有效
//	if( ICEC.IdentityState != 0x0 ) *Source |= 0x80;            //安全认证挂起
	if( ICEC.IdentityState == 0x55 ) *Source |= 0x80;           //安全认证挂起	//15.02.11
                                                                
	if( ICComm.RPrivateKeyFlag == 0 ) *(Source+1) |= 0x01;		//私钥
//	if( ( PIN_SETEN & P_SETEN ) == 0 ) *(Source+1) |= 0x06;     //厂内状态、校表EEPROM写保护解除
//	if( ( PIN_SETEN & P_SETEN ) == 0 ) *(Source+1) |= 0x04;     //校表EEPROM写保护解除				//11.07.19	
	if( ( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 ) *(Source+1) |= 0x04;  //if( ( PIN_SETEN & P_SETEN ) == 0 ) *(Source+1) |= 0x04;     //校表EEPROM写保护解除				//11.07.19	
	E2P_RData( Ptr, EProductState, 2 ); 						//11.07.18
	if(( *Ptr == 0x5A )&&( *(Ptr+1) == 0xA5 )) *(Source+1) |= 0x02;		//厂内状态					//11.07.19

	if( SM.SetCloseDelay2 != 0 ) *(Source+1) |= 0x08;           //02级密码闭锁
	if( SM.SetCloseDelay4 != 0 ) *(Source+1) |= 0x10;			//04级密码闭锁

    if( ICComm.RelayONState == 0xAA ) *(Source+1) |= 0x20;		//允许手动合闸状态       								//10/11.05
    if(( Para.RMeterTripState == 0x1A )&&( ICComm.TripOFFDelay != 0 )) *(Source+1) |= 0x40;		//远程拉闸延时状态      //10/11.05                                                                                                               

	*(Source+3) |= (Flag.KeyStateFlg & 0x1f );					//按键状态	//11.07.19
                                                                
}

/*
void Read_CPU_CRC( unsigned char* Dest )						//11.04.30
{
	unsigned short i;
	unsigned short j;
    unsigned long addr;
	
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
	CRCINIRES = 0xFFFF;											
    addr = 0x1800;												//新国网	//14.07.08
	WDTCTL = WDT_ARST_1000;										//新国网	//14.07.08
	_DINT();													//新国网	//14.07.08
	for( i=0;i<256;i++ )										//新国网	//14.07.08	
	{															//新国网	//14.07.08
		CRCDI = __data20_read_short(addr);						//新国网	//14.07.08	
        addr += 2;												//新国网	//14.07.08
	}															//新国网	//14.07.08
	_EINT();													//新国网	//14.07.08
    addr = 0x5C00;
#else
    addr = 0x5C00;
	CRCINIRES = 0xFFFF;
#endif
	for( j=0;j<128;j++ )
	{
		WDTCTL = WDT_ARST_1000;
  		_DINT();
		for( i=0;i<1024;i++ )
		{
			CRCDI = __data20_read_short(addr);
            addr += 2;
		}
		_EINT();
	}			

	i = CRCINIRES;
	RAM_Write( Dest, (unsigned char*)&i, 2 );
	
}	
*/

void Read_CPU_CRC( unsigned char* Dest )						//11.04.30
{
	unsigned short CRC_Value;
	unsigned long i;
    unsigned long addr;
		
	HT_CRC->CRCCON |=0x00000004;		/*复位CRC计算*/
	HT_CMU->FLASHCON = CMU_FLASHCON_FOP_READ;   /*Flash读*/
	addr=0x00000000;
//	for( i=0;i<0x10100;i++ )
	for( i=0;i<0x10000;i++ )			//17.01.16
	{
		HT_FreeDog();
  		__disable_irq();//_DINT();			                                     
        HT_CRC->CRCDAT = M32(addr+i*4);		 /*从Flash读出数据放入CRCDAT数据寄存器*/
		__enable_irq();//_EINT();
	}			

	CRC_Value = HT_CRC->CRCDAT;
	RAM_Write( Dest, (unsigned char*)&CRC_Value, 2 );
	
}

//自定义参变量
void NProc_ReadExt( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
//	unsigned short Length;
	unsigned char* ReadBufAds;
	unsigned long ComID;
	short Dev_Ads;
	unsigned short EAddr;
	unsigned long Value;
//	unsigned short i;
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	ComID = ExtComTab[ItemAds].ComID;
	
	switch( Info & Comm_Type )
	{
		case OTHER:
			if( ComID == 0x04B00813 )
			{
				RAM_Write( ReadBufAds, (unsigned char*)VerType, 32 );	//正华自定义版本号
//				if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
//				{
					*(ReadBufAds+22) = 'B';				//8025T		
//				}
//				else *(ReadBufAds+22) = 'A';			//DS3231	

				if( MSpec.RRatedVolt == 2200 ) *(ReadBufAds+12) = '2';			//220V	//新国网		//13.08.30
				else if( MSpec.RRatedVolt == 1000 ) *(ReadBufAds+12) = '1';		//100V	//新国网		//13.08.30
				else if( MSpec.RRatedVolt == 577 ) *(ReadBufAds+12) = '5';		//57.7V	//新国网		//13.08.30
				
				if(( MSpec.RBaseCurrent == 3000 )&&( MSpec.RMaxCurrent == 12000 )) *(ReadBufAds+13) = 'A';	//0.3(1.2)A		//新国网		//13.08.30
				else if(( MSpec.RBaseCurrent == 15000 )&&( MSpec.RMaxCurrent == 60000 )) *(ReadBufAds+13) = 'E';	//1.5(6)A		//新国网		//13.08.30
				else if(( MSpec.RBaseCurrent == 50000 )&&( MSpec.RMaxCurrent == 600000 )) *(ReadBufAds+13) = 'N';	//1.5(6)A		//新国网		//13.08.30
				else if(( MSpec.RBaseCurrent == 100000 )&&( MSpec.RMaxCurrent == 1000000 )) *(ReadBufAds+13) = 'R';	//1.5(6)A		//新国网		//13.08.30

#if ( FlashTypeSelect == YesCheck )		//支持Flash类型设置		//新国网	//14.06.04	
				E2P_RData( ReadBufAds+32, EFlashType, 1 ); 
				if(( *(ReadBufAds+32) == 0x64 )||( IsFlashType64() == 0 ))
				{
					if((( Flag.BatState & F_E2PCheck ) == 0 )&&( IsFlashType32AndBelow() != 0 ))
					{
						*(ReadBufAds+24) = 'C';						
					}
					else *(ReadBufAds+24) = '0';						
				}
				else
				{
					if((( Flag.BatState & F_E2PCheck ) == 0 )||( IsFlashType32AndBelow() == 0 ))
					{
						*(ReadBufAds+24) = 'B';						
					}
					else *(ReadBufAds+24) = '0';						
				}	
#endif

				SwapData( ReadBufAds, 32 );
			}
			else if( ComID == 0x04B00095 )								//V4
			{
//				RAM_Write( ReadBufAds, Flag.AlarmFlg, 4 );				//报警状态字
				RAM_Write( ReadBufAds, Flag.AlarmFlg, Len );			//报警状态字
			}	
			else if( ComID == 0x04B00506 )								//读取内部状态		//10.08.25
			{
				GetInsideState( ReadBufAds );												//10.08.25
			}		
			else if( ComID == 0x04B00839 )								//读取总剩余扇区数
			{
				Dev_Ads = GetRemainSectors( &EAddr );					//总剩余扇区数		
				RAM_Write( ReadBufAds, (unsigned char*)&Dev_Ads, 2 );	//报警状态字
			}		
			else if(( ComID == 0x04B00832 )||( ComID == 0x04B00818 ))	//10.10.06
			{
				RAM_Write( ReadBufAds, (unsigned char*)&SM.SumPluseECP0, 4 );	//10.10.04
			}			
//			else if( ComID == 0x04B00509 )	//10.10.06
//			{
//				RAM_Write( ReadBufAds, (unsigned char*)&SM.Ib00IrmsDelay, 4 );	//10.10.11
//			}	
			*(ComBuf + Rs_LenPtr) += Len;
			break;
		case PARAM:
//			if(( ComID == 0x04B00201 )||( ComID == 0x04B00301 )) 	//循环显示项、按键显示项需特殊处理
//			{
//				Length = *(ComBuf+NRs_ID0Ptr)-1;
//				E2P_RData( ReadBufAds, Addr+Length*8, 7 ); 
//			}	
//			else 
			if( ComID == 0x04B00842 )		//读EEPROM数据
			{
				EAddr = *(ReadBufAds+2)*256+*(ReadBufAds+1);
				Len = *(ReadBufAds+3);
				if( Len >= 196 ) Len = 196;
				if( *ReadBufAds == 0 ) Dev_Ads = AdjEAds; 
				else if( *ReadBufAds == 1 ) Dev_Ads = FMAds; 
				else if( *ReadBufAds == 3 )
				{
//					ProhibitReadCPU_FLASH( ReadBufAds, (unsigned char*)EAddr, Len );
					*(ComBuf + Rs_LenPtr) += Len;
					break;		
				}	
				else Dev_Ads = DataEAds; 
//				EAddr = *(ReadBufAds+2)*256+*(ReadBufAds+1);
//				Len = *(ReadBufAds+3);
//				if( Len >= 196 ) Len = 196;
//				E2P_Read( ReadBufAds, EAddr, Len, Dev_Ads );
				E2PRead( ReadBufAds, EAddr, Len, Dev_Ads );
				if( Dev_Ads == DataEAds ) ProhibitReadPassword( ReadBufAds, EAddr, Len );
			}	
			else if( ComID == 0x04B00843 )		//读FLASH数据
			{
				RAM_Write( (unsigned char*)&ComID, ReadBufAds, 4 );
				Len = *(ReadBufAds+4);
				if( Len >= 196 ) Len = 196;
				Read_Flash( ReadBufAds, ComID, Len );				
			}	
			else if( ComID == 0x04B00844 )		//通讯查询显示
			{
//				RAM_Write( (unsigned char*)&Value, ReadBufAds, 4 );				
//				if( Value == 0x04B00848 )		//显示自定义编号				
//				{
//					E2P_RData( ReadBufAds+8, ZdyClient_No, 4 );
//					if( IsAllData( ReadBufAds+8, 4, 0x00 ) == 0 )
//					{
//						*ComBuf = 0xff;			//显示自定义编号为0，无反应。
//						break;
//					}	
//				}	
//				RAM_Write( (unsigned char*)&Disk.CurDisID[0], ReadBufAds, 7 );
//				Disk.CurDisID[4] = BCD_Byte( Disk.CurDisID[4] );				//小项序号由BCD->HEX				

				Disk.CurDisObj.Type = 0; 
				Disk.CurDisObj.No = 0; 
				RAM_Write( (unsigned char*)&Value, ReadBufAds, 4 );				
				if( Value == 0xFEFEFEFE ) RAM_DataFill( Disk.CurDisObj.OAD, 4, 0xFE );
				else if(( Value == 0x04000101 )||( Value == 0x04000102 ))		//日期、时间
				{
					Disk.CurDisObj.OAD[0] = 0x40;
					Disk.CurDisObj.OAD[1] = 0x00;
					Disk.CurDisObj.OAD[2] = 0x02;
					Disk.CurDisObj.OAD[3] = 0x00;
					if( Value == 0x04000102 ) Disk.CurDisObj.No = 1; 		//时间					
				}	
				else if( Value == 0x04000409 )		//有功常数
				{
					Disk.CurDisObj.OAD[0] = 0x41;
					Disk.CurDisObj.OAD[1] = 0x09;
					Disk.CurDisObj.OAD[2] = 0x02;
					Disk.CurDisObj.OAD[3] = 0x00;
				}
				else if(( Value == 0x02010100 )||( Value == 0x02010200 )||( Value == 0x02010300 ))		//电压
				{
					Disk.CurDisObj.OAD[0] = 0x20;
					Disk.CurDisObj.OAD[1] = 0x00;
					Disk.CurDisObj.OAD[2] = 0x02;
					Disk.CurDisObj.OAD[3] = *(ReadBufAds+1);
				}	
				else if(( Value == 0x02020100 )||( Value == 0x02020200 )||( Value == 0x02020300 ))		//电流
				{
					Disk.CurDisObj.OAD[0] = 0x20;
					Disk.CurDisObj.OAD[1] = 0x01;
					Disk.CurDisObj.OAD[2] = 0x02;
					Disk.CurDisObj.OAD[3] = *(ReadBufAds+1);
				}
				else if( Value == 0x04000401 )	//表地址
				{
					Disk.CurDisObj.OAD[0] = 0x40;
					Disk.CurDisObj.OAD[1] = 0x01;
					Disk.CurDisObj.OAD[2] = 0x02;
					Disk.CurDisObj.OAD[3] = 0x00;
					Disk.CurDisObj.No = 1; 
				}
				else if( Value == 0x00010000 )	//正向有功总电量
				{
					Disk.CurDisObj.OAD[0] = 0x00;
					Disk.CurDisObj.OAD[1] = 0x10;
					Disk.CurDisObj.OAD[2] = 0x02;
					Disk.CurDisObj.OAD[3] = 0x01;
				}
				else if( Value == 0x01010000 )	//正向有功总需量
				{
					Disk.CurDisObj.OAD[0] = 0x10;
					Disk.CurDisObj.OAD[1] = 0x10;
					Disk.CurDisObj.OAD[2] = 0x02;
					Disk.CurDisObj.OAD[3] = 0x01;
				}
				else								//A相电压
				{
					Disk.CurDisObj.OAD[0] = 0x20;
					Disk.CurDisObj.OAD[1] = 0x00;
					Disk.CurDisObj.OAD[2] = 0x02;
					Disk.CurDisObj.OAD[3] = 0x01;
				}	

				Flag.Disk |= F_KeyInt;
				Disk.DisInv = 30;
				Disk.FullDisDelay = 0;
				if((Flag.Power & F_PwrUp) != 0) SM.BJGDly = 30;
				Len = 0;
				Disk.TouchDisMode = LookMode;
			}				
#if ( MEASCHIP == ADE7758 )	
#elif ( MEASCHIP == ADE7878 )	
			else if( ComID == 0x04B00840 ) ComAdjRead( ComBuf );
#else
#endif
			else if( ComID == 0x04B00847 )								//V4
			{
				*ReadBufAds = SM.ZoneNo;
				*(ReadBufAds+1) = SM.SegTabNo;
				*(ReadBufAds+2) = 0;
				*(ReadBufAds+3) = 0;
			}			
			else if(( ComID >= 0x04B0FF4A )&&( ComID <= 0x04B0FF4F ))	//批读按显和轮显项	
			{
//				for( i=0;i<33;i++ )
//				{
//					E2P_RData( ReadBufAds+i*5, Addr+i*6, 5 );
//				}	
			}			
			else if( ComID == 0x04B0FF50 )				//参数包
			{
//				GetManuPara1( ReadBufAds );						
				;
			}	
			else if( ComID == 0x04B0FF51 )				//事件阀值参数包
			{
//				GetManuPara2( ReadBufAds );						
				;
			}	
//			else if( ComID == 0x04B0FF52 )				//阶梯参数包	//新国网	
//			{
//				GetManuPara3( ReadBufAds );						
//			}	
			else if(( ComID >= 0x04B0FF02 )&&( ComID <= 0x04B0FF05 ))	//时段表参数包1~4
			{
//				GetManuSeg( ReadBufAds, Addr );
				;	
			}	
			else if( ComID == 0x04B00830 )				//厂内、厂外状态
			{
//				if( ( PIN_SETEN & P_SETEN ) == 0 ) 
//				{
//					*ReadBufAds = 0x5A;
//					*(ReadBufAds+1) = 0xA5;
//					*(ReadBufAds+2) = 0x55;
//				}
//				else
//				{
//					*ReadBufAds = 0x00;
//					*(ReadBufAds+1) = 0x00;
//					*(ReadBufAds+2) = 0x00;
//				}		
				E2P_RData( ReadBufAds, EProductState, 2 ); 					//10.05.13
//				if( ( PIN_SETEN & P_SETEN ) == 0 ) *(ReadBufAds+2) = 0x55;
				if( ( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 ) *(ReadBufAds+2) = 0x55;//if( ( PIN_SETEN & P_SETEN ) == 0 ) *(ReadBufAds+2) = 0x55;	
				else *(ReadBufAds+2) = 0x00;
			}	
#if ( RemoteEnterCalib == YesCheck )								//新国网	//14.05.23
			else if( ComID == 0x04B00835 )				//进入或退出校表状态	0x5566为进入，非0x5566为退出校表状态
			{
				if( SM.RemoteCalibState == 0x55 )		//远程校表状态
				{
					*ReadBufAds = 0x66;
					*(ReadBufAds+1) = 0x55;
				}
				else
				{
					*ReadBufAds = 0x00;
					*(ReadBufAds+1) = 0x00;
				}		
			}	
			else if( ComID == 0x04B0081C )				//读校表参数的CRC校验码	//11.04.30	
			{
//				Read_CalibPara_CRC( ReadBufAds );						
			}	
#endif
#if ( FlashTypeSelect == YesCheck )		//支持Flash类型设置		//新国网	//14.06.04	
			else if( ComID == 0x04B00836 )								//V4
			{
				E2P_RData( ReadBufAds, EFlashType, 1 ); 
				if(( *ReadBufAds == 0x64 )||( IsFlashType64() == 0 ))
				{
					if((( Flag.BatState & F_E2PCheck ) == 0 )&&( IsFlashType32AndBelow() != 0 ))
					{
						*(ReadBufAds+1)	= 0x64;						
					}
					else *(ReadBufAds+1) = 0x00;						
				}
				else
				{
					if((( Flag.BatState & F_E2PCheck ) == 0 )||( IsFlashType32AndBelow() == 0 ))
					{
						*(ReadBufAds+1)	= 0x32;						
					}
					else *(ReadBufAds+1) = 0x00;						
				}	
				Len = 2;		
			}	
#endif
			else if( ComID == 0x04B006FF )				//电表规格参数包	//新国网	//13.09.16
			{
				GetMeterSpec( ReadBufAds );						
			}					
//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T用7022内部温度传感器		//10.08.03		//新国网
			else if( ComID == 0x04B00505 ) 				//计量芯片温度
			{
//				*ReadBufAds = Real.Wendu[0];
//				*(ReadBufAds+1) = Real.Wendu[1];
				*ReadBufAds = Real.MChipWendu[0];		//新国网
				*(ReadBufAds+1) = Real.MChipWendu[1];	//新国网
			}
//#endif				
			else if( ComID == 0x04B00850 )				//读FLASH的ID号		//11.02.23	
			{
				Read_FlashID( ReadBufAds );						
			}	
			else if( ComID == 0x04B0081A )				//读程序的CRC校验码	//11.04.30	
			{
				Read_CPU_CRC( ReadBufAds );						
			}	
			else if( ComID == 0x04B0081B )				//读参数的CRC校验码	//11.11.05	
			{
				*CommPara[GetCOM_No(ComBuf)].BTime = MS500 + MS500;
//				Read_Para_CRC( ReadBufAds );
				*CommPara[GetCOM_No(ComBuf)].BTime = MS500;				
			}	
			else if( ComID == 0x04B00508 )				//读远程密钥状态	//12.05.28	
			{
				E2P_RData( ReadBufAds, ESecretKeyState, 4 );	//0:公钥；1：私钥		//新国网
				if( ICComm.RPrivateKeyFlag != 0 ) *(ReadBufAds+4) = 0; 		//当前处于公钥状态
				else *(ReadBufAds+4) = 1; 		//当前处于私钥状态
			}	
			else
			{
				E2P_RData( ReadBufAds, Addr, Len ); 					
			}
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		default: break; 								
	}	
}	

////////////////////////////////////////////////////////////////////////////////////
/*
unsigned char NCom_ReadSubs(unsigned char* ComBuf)
{
	unsigned char Err_Byte=0;
	short ItemAds;
	
	ItemAds = GetCOM_No( ComBuf );
//	if( Data_Comp( ComBuf+NRs_ID0Ptr, CommLoadPara[ItemAds].SubsPara, 4 ) != 0 ) 		//新国网
	if(( Data_Comp( ComBuf+NRs_ID0Ptr, CommLoadPara[ItemAds].SubsPara, 4 ) != 0 )||( IsAllData( ComBuf+NRs_ID0Ptr, 4, 0x00 ) == 0 )) 	//新国网
	{
		if( IsOccurEventUpSubsFrame() != 0 )			//新国网
		{												//新国网
			CreatEventUpSubsFrame( ComBuf );			//新国网	
			return Err_Byte;							//新国网	
		}												//新国网	
		else											//新国网
		{												//新国网		
			Err_Byte |= RS_State_IVID;
			return Err_Byte;	
		}												//新国网
	}
		
	switch( *(ComBuf+NRs_ID3Ptr) )
	{
		case 0x03:						//事件记录
#if ( TakeMDClrRecFrameApart ==	YesCheck )			//11.02.17
			if(( *(ComBuf+NRs_ID2Ptr) != 0x30 )||(( *(ComBuf+NRs_ID1Ptr) != 0x05 )&&( *(ComBuf+NRs_ID1Ptr) != 0x08 )&&( *(ComBuf+NRs_ID1Ptr) != 0x02 ))
#else
			if(( *(ComBuf+NRs_ID2Ptr) != 0x30 )||(( *(ComBuf+NRs_ID1Ptr) != 0x05 )&&( *(ComBuf+NRs_ID1Ptr) != 0x08 ))
#endif			
				||( *(ComBuf+NRs_ID0Ptr) > 0x0A )||( *(ComBuf+NRs_ID0Ptr) == 0x00 ))
			{
				Err_Byte |= RS_State_IVID;		
				return	Err_Byte;			
			}	
			*(ComBuf+Rs_LenPtr) = 4;
			Err_Byte = NProc_ReadSubsEve( ComBuf );		
			break;		
		case 0x05:						//冻结量
			if(( *(ComBuf+NRs_ID0Ptr) == 0x00 )||( *(ComBuf+NRs_ID1Ptr) != 0xFF )||(( *(ComBuf+NRs_ID2Ptr) == 0x00 )&&( *(ComBuf+NRs_ID0Ptr) > 0x3C ))	//定时冻结记录号        
				||(( *(ComBuf+NRs_ID2Ptr) == 0x01 )&&( *(ComBuf+NRs_ID0Ptr) > 0x03 ))                                                                   //瞬时冻结记录号 		
				||(( *(ComBuf+NRs_ID2Ptr) == 0x02 )&&( *(ComBuf+NRs_ID0Ptr) > 0x02 ))                                                                   //时区表切换冻结记录号 	
				||(( *(ComBuf+NRs_ID2Ptr) == 0x03 )&&( *(ComBuf+NRs_ID0Ptr) > 0x02 ))                                                                   //时段表切换冻结记录号 	
//				||(( *(ComBuf+NRs_ID2Ptr) == 0x05 )&&( *(ComBuf+NRs_ID0Ptr) > 0x02 ))                               	                               	//费率电价切换冻结记录号
				||(( *(ComBuf+NRs_ID2Ptr) == 0x06 )&&( *(ComBuf+NRs_ID0Ptr) > 0x3E )))					                                                //日冻结记录号 			
//				||(( *(ComBuf+NRs_ID2Ptr) == 0x07 )&&( *(ComBuf+NRs_ID0Ptr) > 0x02 )))		            			                                    //阶梯切换冻结记录号
			{                                                                                                                                            	
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;			
			}	
			*(ComBuf+Rs_LenPtr) = 4;
			Err_Byte = NProc_ReadSubsDJ( ComBuf );		
			break;		
		case 0x06:						//负荷记录
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			if( IsNewLoadParaCom( ComBuf+NRs_ID0Ptr ) != -1 )			//新定义的负荷曲线抄收命令		//新国网	//14.05.05
			{
				*(ComBuf+Rs_LenPtr) = 4;
				Err_Byte = NProc_ReadSubsNewLoad( ComBuf );		
				break;			
			}	
#endif
			if(( *(ComBuf+NRs_ID0Ptr) > 0x02 )||(*(ComBuf+NRs_ID2Ptr) > 0x06 )||( *(ComBuf+NRs_ID1Ptr) != 0x00 ))
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;			
			}	
			*(ComBuf+Rs_LenPtr) = 4;
			Err_Byte = NProc_ReadSubsLoad( ComBuf );		
			break;		
		default:
			Err_Byte |= RS_State_IVID;
			break;
	}	
	return Err_Byte;
}

//返回值：0：无后续数据	0xB0: 有后续数据
unsigned char NProc_ReadSubsDJ( unsigned char* ComBuf )
{
	short COM_No; 
  	unsigned char Back_Byte=0;
  	unsigned char BlockSum;
	unsigned char* Ptr;
	
	COM_No = GetCOM_No( ComBuf );
	Ptr = CommLoadPara[COM_No].SubsPara;

	BlockSum = *(Ptr+4);
	if( BlockSum == 0 )
	{
		RAM_Fill( Ptr, 6);							//无后续数据帧，清空后继数据帧参数
		return Back_Byte;
	}	

	GetSubsDJBlockRec( ComBuf );		
	
	BlockSum = *(Ptr+4);							//后继记录块数
//	*(Ptr+5) += 1;									//帧序列号
//	*(ComBuf + Rs_LenPtr) += 1;						//增加帧序列号字节数
//	*(ComBuf + *(ComBuf + Rs_LenPtr) ) = *(Ptr+5);	//帧序列号
	if( BlockSum != 0 ) Back_Byte = 0xB0;			//有后续数据帧
	else RAM_Fill( Ptr, 6);							//无后续数据帧，清空后继数据帧参数
	return Back_Byte;		
}

//返回值：0：无后续数据	0xB0: 有后续数据
unsigned char NProc_ReadSubsEve( unsigned char* ComBuf )
{
	short COM_No; 
 	unsigned char Back_Byte=0;
  	unsigned char BlockSum;
	unsigned char* ReadBufAds;
	unsigned char* Ptr;
	
	COM_No = GetCOM_No( ComBuf );
	Ptr = CommLoadPara[COM_No].SubsPara;

	BlockSum = *(Ptr+4);
	ReadBufAds = ComBuf+NRs_RDataPtr;
	if( BlockSum == 0 )
	{
		RAM_Fill( Ptr, 6);							//无后续数据帧，清空后继数据帧参数
		return Back_Byte;
	}	

	if( *(ComBuf+NRs_ID1Ptr) == 0x05 ) 				//时段表编程记录
	{
		*(ComBuf + Rs_LenPtr) += GetSubsSegTabPrgRec( ReadBufAds, ComBuf );
	}
#if ( TakeMDClrRecFrameApart ==	YesCheck )			//11.02.17
	else if( *(ComBuf+NRs_ID1Ptr) == 0x02 ) 		//节假日编程记录
	{
		*(ComBuf + Rs_LenPtr) += GetSubsMDClrRec( ReadBufAds, ComBuf );
	}	
#endif
	else if( *(ComBuf+NRs_ID1Ptr) == 0x08 ) 		//节假日编程记录
	{
		*(ComBuf + Rs_LenPtr) += GetSubsHolidayPrgRec( ReadBufAds, ComBuf );
	}	
		
//	*(Ptr+4) = BlockSum;							//后继记录块数
	BlockSum = *(Ptr+4);							//后继记录块数
//	*(Ptr+5) += 1;									//帧序列号
//	*(ComBuf + Rs_LenPtr) += 1;						//增加帧序列号字节数
//	*(ComBuf + *(ComBuf + Rs_LenPtr) ) = *(Ptr+5);	//帧序列号
	if( BlockSum != 0 ) Back_Byte = 0xB0;			//有后续数据帧
	else RAM_Fill( Ptr, 6);							//无后续数据帧，清空后继数据帧参数
	return Back_Byte;		
}

//返回值：0：无后续数据	0xB0: 有后续数据
unsigned char NProc_ReadSubsLoad( unsigned char* ComBuf )
{
	short COM_No; 
	short Length;
  	unsigned char Back_Byte=0;
  	unsigned char BlockSum;
	unsigned char* ReadBufAds;
	unsigned char* Ptr;
	
	COM_No = GetCOM_No( ComBuf );
	Ptr = CommLoadPara[COM_No].SubsPara;

	BlockSum = *(Ptr+4);
	ReadBufAds = ComBuf+NRs_RDataPtr;
	if( BlockSum == 0 )
	{
		RAM_Fill( Ptr, 6);							//无后续数据帧，清空后继数据帧参数
		return Back_Byte;
	}	
	
#if( LASTBLOCKProcess == YesCheck )							//13.03.16	
	Length = GetSubsLoadData( ReadBufAds, ComBuf, 1 );	//剩余记录读取
#else
	Length = GetSubsLoadData( ReadBufAds, ComBuf );	//剩余记录读取
#endif
	if( Length == 0 ) BlockSum = 0;					//无相应的负荷曲线数据
	else
	{
		*(ComBuf + Rs_LenPtr) += Length;
		BlockSum -= 1;
	}	
	if( *(ComBuf+NRs_ID2Ptr) != 0 )					//单通道负荷曲线考虑在一帧中打包几块负荷数据块以增加通讯效率。
	{
		while(( BlockSum > 0 )&&(( *(ComBuf + Rs_LenPtr) + Length ) < 200 )&&( Length != 0 ))	
		{
			ReadBufAds += Length;
#if( LASTBLOCKProcess == YesCheck )							//13.03.16	
			Length = GetSubsLoadData( ReadBufAds, ComBuf, 1 );	//剩余记录读取
#else
			Length = GetSubsLoadData( ReadBufAds, ComBuf );	//剩余记录读取
#endif
			if( Length == 0 ) BlockSum = 0;					//无后继数据
			else
			{	
				*(ComBuf + Rs_LenPtr) += Length;
				BlockSum -= 1;					
			}	
		}	
	}	
	*(Ptr+4) = BlockSum;							//后继记录块数
//	*(Ptr+5) += 1;									//帧序列号
//	*(ComBuf + Rs_LenPtr) += 1;						//增加帧序列号字节数
//	*(ComBuf + *(ComBuf + Rs_LenPtr) ) = *(Ptr+5);	//帧序列号
#if( LASTBLOCKProcess == YesCheck )							//13.03.16	
	if( BlockSum != 0 )
	{
		Length = GetSubsLoadData( ReadBufAds, ComBuf, 0 );	//判断是否还有后续剩余记录可以读取
		if( Length == 0 ) BlockSum = 0;	
	}	
#endif
	if( BlockSum != 0 ) Back_Byte = 0xB0;			//有后续数据帧
	else RAM_Fill( Ptr, 6);							//无后续数据帧，清空后继数据帧参数
	return Back_Byte;		
}		
*/

////////////////////////////////////////////////////////////////////////////////////

unsigned char NCom_Read(unsigned char* ComBuf)
{
	unsigned short 	Addr, Info, Len;
	unsigned char* RAddr;
  	unsigned char Err_Byte=0;
	short ItemAds;
//	short i;
	unsigned char Class_Id;				//新国网
	unsigned long LComID;				//新国网									

	RAM_Write( (unsigned char*)&LComID, ComBuf+NRs_ID0Ptr, 4 );				//新国网									

	if((( *(ComBuf+NRs_ID3Ptr) == 0x03 )&&( *(ComBuf+NRs_ID2Ptr) == 0x33))			//备份规约数据读取
//		||((*(ComBuf+NRs_ID3Ptr) >= 0x10 )&&( *(ComBuf+NRs_ID3Ptr) <= 0x1F)))
		||((*(ComBuf+NRs_ID3Ptr) >= 0x10 )&&( *(ComBuf+NRs_ID3Ptr) <= 0x20)))		//新国网
	{
/*		if(( *(ComBuf+NRs_ID0Ptr) > 0x0A )&&( *(ComBuf+NRs_ID0Ptr) != 0xFF ))
		{
			Err_Byte |= RS_State_IVID;		//历史记录不超过10次
			return	Err_Byte;			
		}	
		if(( *(ComBuf+NRs_ID1Ptr) == 0xFF )&&( *(ComBuf+NRs_ID0Ptr) == 0xFF ))		//不能两种集合方式一起抄收
		{
			Err_Byte |= RS_State_IVID;
			return	Err_Byte;			
		}	
		ItemAds = NB1GetEveComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
		if( ItemAds == -1 ) 
		{
			Err_Byte |= RS_State_IVID;
			return	Err_Byte;
		}
		*(ComBuf+Rs_LenPtr) = 4;
		if( *(ComBuf+NRs_ID0Ptr) == 0xFF )					//当前及历史数据一起抄收
		{
			for( i=1;i<11;i++ )								//共10个记录数据
			{
				*(ComBuf+NRs_ID0Ptr) = (unsigned char)i;	
				NB1Proc_ReadEve( ItemAds, Addr, Info, Len, ComBuf );
			}			
			*(ComBuf+NRs_ID0Ptr) = 0xFF;					//数据标识恢复
		}	
		else NB1Proc_ReadEve( ItemAds, Addr, Info, Len, ComBuf );		
*/
		Err_Byte |= RS_State_IVID;
		return Err_Byte;	
	}		

	Class_Id = *(ComBuf+NRs_ID3Ptr);	//新国网
	if(( *(ComBuf+NRs_ID3Ptr) == 0x00 )&&(( *(ComBuf+NRs_ID2Ptr) == 0x0D )||( *(ComBuf+NRs_ID2Ptr) == 0xFE ))&&( *(ComBuf+NRs_ID1Ptr) == 0x00 )&&( *(ComBuf+NRs_ID0Ptr) != 0x00 ))		//新国网
	{									//新国网
		Class_Id = 0x03;				//新国网	
	}									//新国网
	if(( LComID & 0xFFFFFF00 ) == 0x21000000 ) Class_Id = 0x03;	//潮流反向事件记录	//新国网	

//	switch( *(ComBuf+NRs_ID3Ptr) )
	switch( Class_Id )					//新国网
	{
		case 0x00:						//电量
			if(( *(ComBuf+NRs_ID0Ptr) > 0x0C )&&( *(ComBuf+NRs_ID0Ptr) != 0xFF ))
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;			
			}	
			if(( *(ComBuf+NRs_ID1Ptr) == 0xFF )&&( *(ComBuf+NRs_ID0Ptr) == 0xFF ))		//不能两种集合方式一起抄收
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;			
			}	
			ItemAds = NGetECComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
			if( ItemAds == -1 ) 
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;
			}
			if( ~Info & EN_R ) 
			{
				Err_Byte |= RS_State_IVData;
				return	Err_Byte;
			}
			*(ComBuf+Rs_LenPtr) = 4;
			if( *(ComBuf+NRs_ID0Ptr) == 0xFF )					//当前及历史电量一起抄收
			{
//				for( i=0;i<13;i++ )								//共13个月数据
//				{
//					*(ComBuf+NRs_ID0Ptr) = (unsigned char)i;	
//					NProc_ReadEC( ItemAds, Addr, Info, Len, ComBuf );
//				}			
//				*(ComBuf+NRs_ID0Ptr) = 0xFF;					//数据标识恢复
				Err_Byte |= RS_State_IVData;
				return	Err_Byte;
			}	
			else NProc_ReadEC( ItemAds, Addr, Info, Len, ComBuf );		
			break;		
		case 0x01:						//需量
			if(( *(ComBuf+NRs_ID0Ptr) > 0x0C )&&( *(ComBuf+NRs_ID0Ptr) != 0xFF ))
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;			
			}	
			if(( *(ComBuf+NRs_ID1Ptr) == 0xFF )&&( *(ComBuf+NRs_ID0Ptr) == 0xFF ))		//不能两种集合方式一起抄收
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;			
			}	
			ItemAds = NGetMDComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
			if( ItemAds == -1 ) 
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;
			}
			if( ~Info & EN_R ) 
			{
				Err_Byte |= RS_State_IVData;
				return	Err_Byte;
			}
			*(ComBuf+Rs_LenPtr) = 4;
			if( *(ComBuf+NRs_ID0Ptr) == 0xFF )					//当前及历史需量一起抄收
			{
//				for( i=0;i<13;i++ )								//共13个月数据
//				{
//					*(ComBuf+NRs_ID0Ptr) = (unsigned char)i;	
//					NProc_ReadMD( ItemAds, Addr, Info, Len, ComBuf );
//				}			
//				*(ComBuf+NRs_ID0Ptr) = 0xFF;					//数据标识恢复
				Err_Byte |= RS_State_IVData;
				return	Err_Byte;
			}	
			else NProc_ReadMD( ItemAds, Addr, Info, Len, ComBuf );		
		
			break;		
		case 0x02:						//变量
			ItemAds = NGetRealComVar( &RAddr, &Info, &Len, ComBuf+NRs_ID0Ptr );
			if( ItemAds == -1 ) 
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;
			}
			if( ~Info & EN_R ) 
			{
				Err_Byte |= RS_State_IVData;
				return	Err_Byte;
			}
			*(ComBuf+Rs_LenPtr) = 4;
			NProc_ReadReal( ItemAds, RAddr, Info, Len, ComBuf );		
		
			break;		
		case 0x03:						//事件记录
/*
//			if(( *(ComBuf+NRs_ID0Ptr) > 0x0C )||(( *(ComBuf+NRs_ID2Ptr) != 0x10 )&&( *(ComBuf+NRs_ID0Ptr) > 0x0A )))
			if( LComID != 0x00FE00FF )	//新国网
			{							//新国网
				if(( *(ComBuf+NRs_ID0Ptr) > 0x0C )||((( LComID & 0xFFFFFF00 ) == 0x000D0000 )&&( *(ComBuf+NRs_ID0Ptr) > 0x04 ))||(( *(ComBuf+NRs_ID2Ptr) != 0x10 )&&( *(ComBuf+NRs_ID2Ptr) != 0xFE )&&( *(ComBuf+NRs_ID0Ptr) > 0x0A )))		//新国网	00FE0001月结算时间为12次
				{
					Err_Byte |= RS_State_IVID;		//电压合格率数据为12月，其余为上10次数据
					return	Err_Byte;			
				}	
			}							//新国网
			ItemAds = NGetEveComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
			if( ItemAds == -1 ) 
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;
			}
			*(ComBuf+Rs_LenPtr) = 4;
//			Err_Byte = NProc_ReadEve( ItemAds, Addr, Info, Len, ComBuf );		
			if( *(ComBuf+NRs_ID0Ptr) == 0xFF )					//12个结算日结算时间一起抄收	//新国网	
			{																					//新国网	
				for( i=1;i<13;i++ )								//共12个月数据					//新国网	
				{																				//新国网		
					*(ComBuf+NRs_ID0Ptr) = (unsigned char)i;									//新国网	
					NProc_ReadEve( ItemAds, Addr, Info, Len, ComBuf );							//新国网	
				}																				//新国网	
				*(ComBuf+NRs_ID0Ptr) = 0xFF;					//数据标识恢复					//新国网		
			}																					//新国网	
			else Err_Byte = NProc_ReadEve( ItemAds, Addr, Info, Len, ComBuf );					//新国网	
*/
			Err_Byte |= RS_State_IVID;
			break;		
		case 0x04:						//参变量
/*
			ItemAds = NGetParaComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
			if( ItemAds == -1 ) 
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;
			}
			if( ~Info & EN_R ) 
			{
//				Err_Byte |= RS_State_IVData;
				Err_Byte |= RS_State_PswE;		//读密码返回"密码错/无授权"	
				return	Err_Byte;
			}
			*(ComBuf+Rs_LenPtr) = 4;
			NProc_ReadPara( ItemAds, Addr, Info, Len, ComBuf );		
*/		
			Err_Byte |= RS_State_IVID;
			break;		
		case 0x05:						//冻结量
/*			if(( *(ComBuf+NRs_ID0Ptr) == 0x00 )||(( *(ComBuf+NRs_ID2Ptr) == 0x00 )&&( *(ComBuf+NRs_ID0Ptr) > 0x3C ))	//定时冻结记录号        
				||(( *(ComBuf+NRs_ID2Ptr) == 0x01 )&&( *(ComBuf+NRs_ID0Ptr) > 0x03 ))                                   //瞬时冻结记录号 		
				||(( *(ComBuf+NRs_ID2Ptr) == 0x02 )&&( *(ComBuf+NRs_ID0Ptr) > 0x02 ))                                   //时区表切换冻结记录号 	
				||(( *(ComBuf+NRs_ID2Ptr) == 0x03 )&&( *(ComBuf+NRs_ID0Ptr) > 0x02 ))                                   //时段表切换冻结记录号 	
				||(( *(ComBuf+NRs_ID2Ptr) == 0x04 )&&( *(ComBuf+NRs_ID0Ptr) > 0xFE ))                                   //整点冻结记录号 	
				||(( *(ComBuf+NRs_ID2Ptr) == 0x05 )&&( *(ComBuf+NRs_ID0Ptr) > 0x02 ))                                   //费率电价切换冻结记录号
				||(( *(ComBuf+NRs_ID2Ptr) == 0x06 )&&( *(ComBuf+NRs_ID0Ptr) > 0x3E ))				                    //日冻结记录号 			
				||(( *(ComBuf+NRs_ID2Ptr) == 0x07 )&&( *(ComBuf+NRs_ID0Ptr) > 0x02 )))                                  //阶梯切换冻结记录号 	 			
			{                                                                                                           
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;			
			}	
			if( *(ComBuf+NRs_ID2Ptr) == 0x04 )			//整点冻结
			{
				if(( *(ComBuf+NRs_ID1Ptr) > 0x02 )&&( *(ComBuf+NRs_ID1Ptr) != 0xff )) return RS_State_IVID;
				*(ComBuf+Rs_LenPtr) = 4;
				Err_Byte = NProc_ReadHourDJ( ComBuf );						
			}
			else
			{				
				ItemAds = NGetDJComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
				if( ItemAds == -1 ) 
				{
					Err_Byte |= RS_State_IVID;
					return	Err_Byte;
				}
				if( ~Info & EN_R ) 
				{
					Err_Byte |= RS_State_IVData;
					return	Err_Byte;
				}
				*(ComBuf+Rs_LenPtr) = 4;
				Err_Byte = NProc_ReadDJ( ItemAds, Addr, Info, Len, ComBuf );		
			}		
*/
			Err_Byte |= RS_State_IVID;
			break;		
		case 0x06:						//负荷记录
/*
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			if( IsNewLoadParaCom( ComBuf+NRs_ID0Ptr ) != -1 )			//新定义的负荷曲线抄收命令		//新国网	//14.05.05
			{
				ItemAds = GetCOM_No( ComBuf );
				RAM_Write( CommLoadPara[ItemAds].SubsPara, ComBuf+NRs_ID0Ptr, 10 );		//保留读取负荷记录的时标
				*( CommLoadPara[ItemAds].SubsPara+11 ) = BCD_Byte( *(ComBuf+NRs_RDataPtr));
				*(ComBuf+Rs_LenPtr) = 4;
				Err_Byte = NProc_ReadNewLoad( ComBuf );		
				break;			
			}	
#endif

			if(( *(ComBuf+NRs_ID0Ptr) > 0x02 )||(*(ComBuf+NRs_ID2Ptr) > 0x06 )||( *(ComBuf+NRs_ID1Ptr) != 0x00 ))
			{
				Err_Byte |= RS_State_IVID;
				return	Err_Byte;			
			}	
			ItemAds = GetCOM_No( ComBuf );
//			RAM_Write( CommLoadPara[ItemAds].SubsPara, ComBuf+NRs_ID0Ptr, 5 );
			RAM_Write( CommLoadPara[ItemAds].SubsPara, ComBuf+NRs_ID0Ptr, 10 );		//保留读取负荷记录的时标
			*( CommLoadPara[ItemAds].SubsPara+11 ) = BCD_Byte( *(ComBuf+NRs_RDataPtr));
			if( *(ComBuf+NRs_ID0Ptr) != 0x01 ) RAM_Fill( CommLoadPara[ItemAds].SubsPara+5, 6 );  //非读给定记录块，时标清零
			*(ComBuf+Rs_LenPtr) = 4;
			Err_Byte = NProc_ReadLoad( ComBuf );		
*/
			Err_Byte |= RS_State_IVID;
			break;		
		default:
			Err_Byte |= RS_State_IVID;
			break;
	}	
	return Err_Byte;
}

//////////////////////////////////////////////////////////////////////////
//电量
short NGetECComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID )
{
	short i;
	unsigned long Value;
	
  	RAM_Write( (unsigned char*)&Value, ComID, 4 );
//	Value &= 0xffffff00;
//  	if(( Value == 0x000B0000 )||( Value == 0x000B0001 )) ;
//  	else Value &= 0xffffff00;
  	if(( Value == 0x000B0000 )||( Value == 0x000B0001 )||( Value == 0x000C0000 )||( Value == 0x000C0001 )) ;	//新国网	//13.10.25
#if ( ExactECRead == YesCheck )			//读取精确电能	//新国网 	//14.04.25				
//	else if(( *(ComID+3) == 0x00 )&&((*(ComID+2) == 0x60 )||(*(ComID+2) == 0x61 ))&&( *ComID == 0xFF )) return -1;		//当前精确电能没有历史电量	//新国网 	//14.04.25
	else if(( *(ComID+3) == 0x00 )&&((*(ComID+2) == 0x60 )||(*(ComID+2) == 0x61 ))&&( *ComID != 0x00 )) return -1;		//当前精确电能没有历史电量	//新国网 	//14.07.11
#endif
  	else 																										//新国网	//13.10.25	
  	{																											//新国网	//13.10.25		
  		Value &= 0xffffff00;																					//新国网	//13.10.25
  		if(( Value == 0x000B0000 )||( Value == 0x000C0000 )) return -1;											//新国网	//13.10.25	
  	}																											//新国网	//13.10.25		
  	
  	for( i=0;i<ECComNum;i++ )
  	{
		if( Value == ECComTab[i].ComID ) 
		{
			*Addr = ECComTab[i].Addr;
			*Info = ECComTab[i].ComInfo;
			*Len = *Info & RS_LEN;
			return i;
		}
  	}
	return -1;
}

/*
void GetLSCycleEC( unsigned char* Dest, unsigned short Addr, unsigned char Cycle )
{
	long Value;
	unsigned char Buff[ECDIGIT+1];
	unsigned char* Ptr;
	
	Ptr = Buff;
	Value = PtAddrChk( LastEC_No, Cycle );					
	if( Value != -1 )
	{
		Value += Addr;
		Read_Flash( Ptr, (unsigned long)Value, ECDIGIT+1 );			//读历史电量及校验和
		if( ChkNum( Ptr, ECDIGIT ) != *(Ptr+ECDIGIT) )				//历史电量校验和不对，取备份数据
		{
			Read_Flash( Ptr, (unsigned long)Value+4*4096, ECDIGIT );
		}		
//#if ( ECDIGIT == BYTE5 )
//		RAM_Write( Ptr, Ptr+1, 4 );									//取4字节
//#endif
		RAM_Write( Ptr, Ptr+(ECDIGIT-4), 4 );						//取4字节
	}	
	else
	{
		RAM_Fill( Ptr, 4);											//无历史数据，返回全零	
	}	
	RAM_Write( Dest, Ptr, 4 );
}	

//每个结算周期
void GetEveryCycleInsideEC( unsigned char* Dest, unsigned char L1Cycle, unsigned char L2Cycle )			//新国网
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char Temp = 0;
	unsigned long ValuePp;
	unsigned long ValuePn;
	unsigned long L1ValuePp;
	unsigned long L1ValuePn;
	long Value;
	
	Ptr = Buff;

	if( L1Cycle == 0 ) Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	else GetLSCycleEC( Ptr, CMon_EC_Pp0, L1Cycle-1 );
	ValuePp = BCD4_Long( Ptr );
	if( L2Cycle == 0 ) Read_CMonEC4( Ptr+4, CMon_EC_Pp0 );
	else GetLSCycleEC( Ptr+4, CMon_EC_Pp0, L2Cycle-1 );
	L1ValuePp = BCD4_Long( Ptr+4 );
	
	if( ValuePp < L1ValuePp ) ValuePp += 100000000;
	ValuePp -= L1ValuePp;

	if( L1Cycle == 0 ) Read_CMonEC4( Ptr, CMon_EC_Pn0 );
	else GetLSCycleEC( Ptr, CMon_EC_Pn0, L1Cycle-1 );
	ValuePn = BCD4_Long( Ptr );
	if( L2Cycle == 0 ) Read_CMonEC4( Ptr+4, CMon_EC_Pn0 );
	else GetLSCycleEC( Ptr+4, CMon_EC_Pn0, L2Cycle-1 );
	L1ValuePn = BCD4_Long( Ptr+4 );

	if( ValuePn < L1ValuePn ) ValuePn += 100000000;
	ValuePn -= L1ValuePn;
	
	Value = 0;
	if(( Para.RActiveMode & 0x01 ) != 0 ) Value += ValuePp;
	if(( Para.RActiveMode & 0x02 ) != 0 ) Value -= ValuePp;
	if(( Para.RActiveMode & 0x04 ) != 0 ) Value += ValuePn;
	if(( Para.RActiveMode & 0x08 ) != 0 ) Value -= ValuePn;
	
	if( Value < 0 ) 
	{
		Temp = 1;
		Value = 0 - Value;
//		Value += 1;							//11.06.13
	}	
	
	Long_BCD4( Ptr, (unsigned long)Value );
	*(Ptr+3) &= 0x7F;
	if( Temp != 0 ) *(Ptr+3) |= 0x80;
	
	RAM_Write( Dest, Ptr, 4 );
}
*/
void GetCycleInsideEC( unsigned char* Dest, unsigned char L1Cycle, unsigned char L2Cycle )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char Temp = 0;
	unsigned long ValuePp;
	unsigned long ValuePn;
	unsigned long L1ValuePp;
	unsigned long L1ValuePn;
	long Value;
	
	Ptr = Buff;

#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
	if( L1Cycle == 0 ) Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	else 
	{
		E2P_RData( Ptr, L1_SaveDay1EC_Pp0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr, 4 ) != 0 )) RAM_Fill( Ptr, 4 );
	}	
	ValuePp = BCD4_Long( Ptr );
	if( L2Cycle == 0 ) Read_CMonEC4( Ptr+4, CMon_EC_Pp0 );
	else
	{
		if( L2Cycle == 1 ) E2P_RData( Ptr+4, L1_SaveDay1EC_Pp0, 4 );
		if( L2Cycle == 2 ) E2P_RData( Ptr+4, L2_SaveDay1EC_Pp0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
	}	
	L1ValuePp = BCD4_Long( Ptr+4 );
	
	if( ValuePp < L1ValuePp ) ValuePp += 100000000;
	ValuePp -= L1ValuePp;

	if( L1Cycle == 0 ) Read_CMonEC4( Ptr, CMon_EC_Pn0 );
	else 
	{
		E2P_RData( Ptr, L1_SaveDay1EC_Pn0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr, 4 ) != 0 )) RAM_Fill( Ptr, 4 );
	}	
	ValuePn = BCD4_Long( Ptr );
	if( L2Cycle == 0 ) Read_CMonEC4( Ptr+4, CMon_EC_Pn0 );
	else 
	{
		if( L2Cycle == 1 ) E2P_RData( Ptr+4, L1_SaveDay1EC_Pn0, 4 );
		if( L2Cycle == 2 ) E2P_RData( Ptr+4, L2_SaveDay1EC_Pn0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );		
	}	
	L1ValuePn = BCD4_Long( Ptr+4 );
#else
	if( L1Cycle == 0 ) Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	else GetLSCycleEC( Ptr, CMon_EC_Pp0, L1Cycle-1 );
	ValuePp = BCD4_Long( Ptr );
	if( L2Cycle == 0 ) Read_CMonEC4( Ptr+4, CMon_EC_Pp0 );
	else GetLSCycleEC( Ptr+4, CMon_EC_Pp0, L2Cycle-1 );
	L1ValuePp = BCD4_Long( Ptr+4 );
	
	if( ValuePp < L1ValuePp ) ValuePp += 100000000;
	ValuePp -= L1ValuePp;

	if( L1Cycle == 0 ) Read_CMonEC4( Ptr, CMon_EC_Pn0 );
	else GetLSCycleEC( Ptr, CMon_EC_Pn0, L1Cycle-1 );
	ValuePn = BCD4_Long( Ptr );
	if( L2Cycle == 0 ) Read_CMonEC4( Ptr+4, CMon_EC_Pn0 );
	else GetLSCycleEC( Ptr+4, CMon_EC_Pn0, L2Cycle-1 );
	L1ValuePn = BCD4_Long( Ptr+4 );
#endif

	if( ValuePn < L1ValuePn ) ValuePn += 100000000;
	ValuePn -= L1ValuePn;
	
	Value = 0;
	if(( Para.RActiveMode & 0x01 ) != 0 ) Value += ValuePp;
	if(( Para.RActiveMode & 0x02 ) != 0 ) Value -= ValuePp;
	if(( Para.RActiveMode & 0x04 ) != 0 ) Value += ValuePn;
	if(( Para.RActiveMode & 0x08 ) != 0 ) Value -= ValuePn;
	
	if( Value < 0 ) 
	{
		Temp = 1;
		Value = 0 - Value;
//		Value += 1;							//11.06.13
	}	
	
	Long_BCD4( Ptr, (unsigned long)Value );
	*(Ptr+3) &= 0x7F;
	if( Temp != 0 ) *(Ptr+3) |= 0x80;
	
	RAM_Write( Dest, Ptr, 4 );
}

#if ( LocalSmartMeter == YesCheck )
void GetYearCycleInsideEC( unsigned char* Dest, unsigned char L1Cycle, unsigned char L2Cycle )			//新国网
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char Temp = 0;
	unsigned long ValuePp;
	unsigned long ValuePn;
	unsigned long L1ValuePp;
	unsigned long L1ValuePn;
	long Value;
	
	Ptr = Buff;

	if( L1Cycle == 0 ) Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	else 
	{
		E2P_RData( Ptr, L1_YearSaveDayEC_Pp0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr, 4 ) != 0 )) RAM_Fill( Ptr, 4 );
	}	
	ValuePp = BCD4_Long( Ptr );
	if( L2Cycle == 0 ) Read_CMonEC4( Ptr+4, CMon_EC_Pp0 );
	else
	{
		if( L2Cycle == 1 ) E2P_RData( Ptr+4, L1_YearSaveDayEC_Pp0, 4 );
		if( L2Cycle == 2 ) E2P_RData( Ptr+4, L2_YearSaveDayEC_Pp0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
	}	
	L1ValuePp = BCD4_Long( Ptr+4 );
	
	if( ValuePp < L1ValuePp ) ValuePp += 100000000;
	ValuePp -= L1ValuePp;

	if( L1Cycle == 0 ) Read_CMonEC4( Ptr, CMon_EC_Pn0 );
	else 
	{
		E2P_RData( Ptr, L1_YearSaveDayEC_Pn0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr, 4 ) != 0 )) RAM_Fill( Ptr, 4 );
	}	
	ValuePn = BCD4_Long( Ptr );
	if( L2Cycle == 0 ) Read_CMonEC4( Ptr+4, CMon_EC_Pn0 );
	else 
	{
		if( L2Cycle == 1 ) E2P_RData( Ptr+4, L1_YearSaveDayEC_Pn0, 4 );
		if( L2Cycle == 2 ) E2P_RData( Ptr+4, L2_YearSaveDayEC_Pn0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );		
	}	
	L1ValuePn = BCD4_Long( Ptr+4 );

	if( ValuePn < L1ValuePn ) ValuePn += 100000000;
	ValuePn -= L1ValuePn;
	
	Value = 0;
	if(( Para.RActiveMode & 0x01 ) != 0 ) Value += ValuePp;
	if(( Para.RActiveMode & 0x02 ) != 0 ) Value -= ValuePp;
	if(( Para.RActiveMode & 0x04 ) != 0 ) Value += ValuePn;
	if(( Para.RActiveMode & 0x08 ) != 0 ) Value -= ValuePn;
	
	if( Value < 0 ) 
	{
		Temp = 1;
		Value = 0 - Value;
//		Value += 1;							//11.06.13
	}	
	
	Long_BCD4( Ptr, (unsigned long)Value );
	*(Ptr+3) &= 0x7F;
	if( Temp != 0 ) *(Ptr+3) |= 0x80;
	
	RAM_Write( Dest, Ptr, 4 );
}
#endif

//电量
void NProc_ReadEC( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i,Length;
	unsigned char* ReadBufAds;
//	long Value;
//	unsigned long ComID;
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	
	switch( Info & Comm_Type )
	{
		case 0x0000:
			if( *(ComBuf+NRs_ID0Ptr) == 0 )
			{
				Read_CMonEC4( ReadBufAds, Addr );
			}
			else
			{
//				GetLSCycleEC( ReadBufAds, Addr, *(ComBuf+NRs_ID0Ptr)-1 ); 
				RAM_Fill( ReadBufAds, Len);										//无历史数据，返回全零	
			}					
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case PARAM:
/*			RAM_Write( (unsigned char*)&ComID, ComBuf+NRs_ID0Ptr, 4 );
			if( ComID == 0x000B0000 )						//当前结算周期总累计用电量			//新国网
			{
				GetEveryCycleInsideEC( ReadBufAds, 0, 1 );				
			}	
			else if( ComID == 0x000C0000 )					//当前月结算周期总累计用电量			//新国网
			{
				GetCycleInsideEC( ReadBufAds, 0, 1 );
			}	
			else if( ComID == 0x000B0001 )					//上1结算周期总累计用电量			//新国网
			{
				GetEveryCycleInsideEC( ReadBufAds, 1, 2 );				
			}	
			else if( ComID == 0x000C0001 )					//上1月结算周期总累计用电量			//新国网
			{
				GetCycleInsideEC( ReadBufAds, 1, 2 );
			}
			else if( ComID == 0x000D0000 )					//当前日期时间、当前年结算周期组合有功总累计用电量		//新国网
			{
				RAM_Write( ReadBufAds, Clk.Hour, 4 );
				GetYearCycleInsideEC( ReadBufAds+4, 0, 1 );
			}	
#if ( ExactECRead == YesCheck )			//读取精确电能	//新国网 	//14.04.25				
			else if( IsExactECID( ComID ) == 0 )			//当前精确电能	//新国网 	//14.04.25
			{
				Disk.DigitsValidFlg = 0x55;		//3位小数显示时才添加0.01kWh以下尾数		//新国网	//13.10.25
				Read_CMonEC( ReadBufAds, Addr );
				Disk.DigitsValidFlg = 0x00;
			}
#endif
*/
			RAM_Fill( ReadBufAds, Len);										//无数据，返回全零	
			*(ComBuf + Rs_LenPtr) += Len;
			break;	
		case DATABLK:
			Length = Len;
			for( i=1;i<Length+1;i++ )
			{
				Addr = ECComTab[ ItemAds+i ].Addr;
				Info = ECComTab[ItemAds+i].ComInfo;
				Len = Info & RS_LEN;
				NProc_ReadEC( ItemAds+i, Addr, Info, Len, ComBuf );
			}
			break;
	
		default: break; 								
	}	
}	

//////////////////////////////////////////////////////////////////////////
//需量
short NGetMDComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID )
{
	short i;
	unsigned long Value;
	
  	RAM_Write( (unsigned char*)&Value, ComID, 4 );
	Value &= 0xffffff00;
  	
  	for( i=0;i<MDComNum;i++ )
  	{
		if( Value == MDComTab[i].ComID ) 
		{
			*Addr = MDComTab[i].Addr;
			*Info = MDComTab[i].ComInfo;
			*Len = *Info & RS_LEN;
			return i;
		}
  	}
	return -1;
}

//需量
void NProc_ReadMD( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i,Length;
	unsigned char* ReadBufAds;
//	long Value;
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	
	switch( Info & Comm_Type )
	{
		case 0x0000:
			if( *(ComBuf+NRs_ID0Ptr) == 0 )
			{
				E2P_RData( ReadBufAds, Addr, 8 );
			}
			else
			{
/*
				Value = PtAddrChk( LastMD_No, *(ComBuf+NRs_ID0Ptr)-1 );					
				if( Value != -1 )
				{
					Value += Addr;
                    Value -= CMon_MD_Pp0;
					Read_Flash( ReadBufAds, (unsigned long)Value, Len+1 );			//读历史需量及校验和
					if( ChkNum( ReadBufAds, Len ) != *(ReadBufAds+Len) )			//历史需量校验和不对，取备份数据
					{
						Read_Flash( ReadBufAds, (unsigned long)Value+4*4096, Len );
					}		
				}	
				else
				{
					RAM_Fill( ReadBufAds, Len);										//无历史数据，返回全零	
				}	
*/
			}		
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case DATABLK:
			Length = Len;
			for( i=1;i<Length+1;i++ )
			{
				Addr = MDComTab[ ItemAds+i ].Addr;
				Info = MDComTab[ItemAds+i].ComInfo;
				Len = Info & RS_LEN;
				NProc_ReadMD( ItemAds+i, Addr, Info, Len, ComBuf );
			}
			break;
		default: break; 								
	}	
}	

//////////////////////////////////////////////////////////////////////////
//变量
short NGetRealComVar( unsigned char** RAddr, unsigned short* Info, unsigned short* Len, unsigned char* ComID )
{
	short i;
	unsigned long Value;
	
  	RAM_Write( (unsigned char*)&Value, ComID, 4 );
  	
  	for( i=0;i<RealComNum;i++ )
  	{
		if( Value == RealComTab[i].ComID ) 
		{
//			*Addr = (unsigned short)RealComTab[i].Addr;
			*RAddr = RealComTab[i].Addr;
			*Info = RealComTab[i].ComInfo;
			*Len = *Info & RS_LEN;
			return i;
		}
  	}
	return -1;
}

//变量
void NProc_ReadReal( short ItemAds, unsigned char* Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i,Length;
	unsigned char* ReadBufAds;
	unsigned long ComID;					//新国网	
	unsigned short Temp;						//新国网	
	unsigned long Value;
    
	ComID = RealComTab[ ItemAds].ComID;		//新国网		
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	
	switch( Info & Comm_Type )
	{
		case 0x0000:
			if( ComID == 0x02800022 )		//身份认证时效剩余时间	//新国网			
			{
				Word_BCD( ReadBufAds, ICEC.IdentityValidDelay );
			}	
			else if( ComID == 0x02800023 )		//红外认证时效剩余时间	//新国网			
			{
				Temp = ICEC.InfraredValidDelay;
				Length = Temp % 60;
				Temp /= 60;
				if( Length != 0 ) Temp += 1;	//不足1分钟按1分钟处理
				*ReadBufAds = Temp;
				*ReadBufAds = Byte_BCD( *ReadBufAds );
			}	
			else if( ComID == 0x02800024 )		//本月总电压合格时间	//新国网			
			{
//				E2P_RFM( ReadBufAds, FVoltNormalTime, 3 );				
				E2P_RFM( (unsigned char*)&Value, FMonVoltNormalTime, 4 );				
                Long_BCD4( ReadBufAds, Value );
			}	
			else
			{	
				RAM_Write( ReadBufAds, (unsigned char*)Addr, Len );	
			}
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case DATABLK:
			Length = Len;
			for( i=1;i<Length+1;i++ )
			{
				Addr = RealComTab[ ItemAds+i ].Addr;
				Info = RealComTab[ItemAds+i].ComInfo;
				Len = Info & RS_LEN;
				NProc_ReadReal( ItemAds+i, Addr, Info, Len, ComBuf );
			}
			break;
		default: break; 								
	}	
}	

/*			//16.09.24
//////////////////////////////////////////////////////////////////////////
//事件
short NGetEveComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID )
{
	short i;
	unsigned long Value;
	unsigned char Buff[4];
	
  	RAM_Write( Buff, ComID, 4 );
	if( *ComID != 0 ) Buff[0] = 0x01;		//有历史记录
	
  	RAM_Write( (unsigned char*)&Value, Buff, 4 );
  	
  	for( i=0;i<EveComNum;i++ )
  	{
		if( Value == EveComTab[i].ComID ) 
		{
			*Addr = EveComTab[i].Addr;
			*Info = EveComTab[i].ComInfo;
			*Len = *Info & RS_EVLEN;
			return i;
		}
  	}
	return -1;
}

//事件
unsigned char NProc_ReadEve( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
//	unsigned short i;
	unsigned char* ReadBufAds;
  	unsigned char Err_Byte=0;
//	unsigned char State=0;
	long Value;
	unsigned char* Ptr;
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	
	switch( Info & EVComm_Type )
	{
		case EVOTHER:
			RAM_Write( ReadBufAds, (unsigned char*)Addr, Len );		//本月电压合格率统计数据
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case EVPARAM:
			E2P_RData( ReadBufAds, Addr, Len ); 					//次数、累计时间等
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case EVENO:
				if(( Info & FLASH ) != 0 )							//历史数据存放于FLASH
				{
						Value = PtAddrChk( Addr, *(ComBuf+NRs_ID0Ptr)-1 );	//取该事件该次记录在FLASH中的首地址
						if( Value != -1 )
						{
							switch( Addr )
							{
								case ECClr_No:
									GetECClrRec( ReadBufAds, Value );
									break;
								case MDClr_No:
#if ( TakeMDClrRecFrameApart ==	YesCheck )			//11.02.17
									Len = GetMDClrRec( ReadBufAds, Value, ComBuf );
									Err_Byte = 0xB0;						//有后续数据帧		
#else
									GetMDClrRec( ReadBufAds, Value );
#endif									
									break;
								case SegTabPrg_No:
									Len = GetSegTabPrgRec( ReadBufAds, Value, ComBuf );
									Err_Byte = 0xB0;						//有后续数据帧		
									break;
								case HolidayPrg_No:
									Len = GetHolidayPrgRec( ReadBufAds, Value, ComBuf );
//									Err_Byte = 0xB0;						//有后续数据帧		
									Ptr = CommLoadPara[GetCOM_No( ComBuf )].SubsPara;
									if( *(Ptr+4) != 0 )	Err_Byte = 0xB0;				//有后续数据帧		
									break;
								default: 
									Read_Flash( ReadBufAds, Value, Len );						
									break;
							}	
						}
						else
						{
							if( Addr == SegTabPrg_No )
							{
//								Len = 178;		//
								Len = GetSegTabPrgRec( ReadBufAds, Value, ComBuf );
								Err_Byte = 0xB0;						//有后续数据帧										 
							}	 
#if ( TakeMDClrRecFrameApart ==	YesCheck )			//11.02.17
							else if( Addr == MDClr_No )
							{
								Len = GetMDClrRec( ReadBufAds, Value, ComBuf );
								Err_Byte = 0xB0;						//有后续数据帧										 
							}	 
#endif
							else if( Addr == HolidayPrg_No )
							{
								Len = GetHolidayPrgRec( ReadBufAds, Value, ComBuf );
								Ptr = CommLoadPara[GetCOM_No( ComBuf )].SubsPara;
								if( *(Ptr+4) != 0 )	Err_Byte = 0xB0;				//有后续数据帧		
							}	
							RAM_Fill( ReadBufAds, Len);					//无历史数据，返回全零								
						}	
				}
				else
				{
					Value = PtAddrChk( Addr, *(ComBuf+NRs_ID0Ptr)-1 );		//取该事件该次记录在EEPROM中的首地址
					if( Value != -1 )
					{
						E2P_RData( ReadBufAds, (unsigned short)Value, Len );						
					}
					else RAM_Fill( ReadBufAds, Len);						//无历史数据，返回全零	
				}		
				*(ComBuf + Rs_LenPtr) += Len;
			break;		
		default: break; 								
	}
  return Err_Byte;	
}	

//////////////////////////////////////////////////////////////////////////
//新备案事件
short NB1GetEveComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID )
{
	short i;
	unsigned long Value;
	unsigned char Buff[4];
	unsigned short EventNo;
	unsigned short IdH;
	
  	RAM_Write( (unsigned char*)&Value, ComID, 4 );
	if(( Value == 0x10000101 )||( Value == 0x10000201 )||(( Value & 0x0000ff00 ) == 0x00 )) ;
	else
	{
  		RAM_Write( Buff, ComID, 4 );
		if( *ComID != 0 ) 
		{
			Buff[0] = 0x01;		//有历史记录
		  	RAM_Write( (unsigned char*)&IdH, Buff+2, 2 );
		  	for( i=0;i<IdEventNum;i++ )
		  	{
				if( IdH == IdEventNoTab[i].ID23 )
				{
					EventNo = IdEventNoTab[i].EventNo;					
					break;
				}	 
		  	}	
			if( i == IdEventNum ) return -1;	
			if(( Buff[1] == 0xff )||(( Buff[1] > 0 )&&( Buff[1] <= PointTab[EventNo].Items ))) 
			{
				Buff[1] = 0x01;		//ID1合法，置为1，用于查表
			}	
			else return -1;			
		}			
	  	RAM_Write( (unsigned char*)&Value, Buff, 4 );
	}	
  	
  	for( i=0;i<B1EveComNum;i++ )
  	{
		if( Value == B1EveComTab[i].ComID ) 
		{
			*Addr = B1EveComTab[i].Addr;
			*Info = B1EveComTab[i].ComInfo;
			*Len = *Info & RS_EVLEN;
			return i;
		}
  	}
	return -1;
}

//取各事件内小项起始地址
unsigned short GetRecItemStartAddr( unsigned short EventNo, unsigned short ItemNo )
{
	short StartAddr=0;
	unsigned char* Ptr;
	short i;
	
	Ptr = PointTab[EventNo].InArrayAddr;
	for( i=1;i<ItemNo;i++ )
	{
//		StartAddr += *(Ptr+i);				
		StartAddr += *(Ptr+i-1);				
	}	
	return StartAddr;
}

//清除上一次记录还未结束对应的结束时刻数据
void ClrRecNullItemContent( unsigned char* Dest, unsigned short EventNo, unsigned char SItemNo )
{
	unsigned short i;
	unsigned short StartAddr=0;
	unsigned short Len;
	unsigned short ItemNo;	
	unsigned char* ParaBuff; 
	unsigned short Items=17;
	
	if(( EventNo >= PhAFail_No )&&( EventNo <= PhCBreak_No ))
	{
		ParaBuff = (unsigned char*)Volt_Rec_BlankItem;   
		Items = 21;
	}
	else if(( EventNo >= CuAFail_No )&&( EventNo <= CuCBreak_No ))
	{
		ParaBuff = (unsigned char*)Current_Rec_BlankItem;   
	}		
	else if(( EventNo == VoltRevPhase_No )||( EventNo == CuRevPhase_No )
		||(( EventNo >= RevPowerA_No )&&( EventNo <= PowerOverC_No )))
	{
		ParaBuff = (unsigned char*)Phase_Power_Rec_BlankItem;   
	}
	else if(( EventNo == VoltImbalance_No )||( EventNo == CuImbalance_No )||( EventNo == CuBestImbalance_No ))		//新国网
	{
		ParaBuff = (unsigned char*)Imbalance_Rec_BlankItem;   
		Items = 18;
	}

	for( i=0;i<Items;i++ )
	{
		if(( SItemNo != 0xff )&&( *(ParaBuff+i) != SItemNo )) continue; 
		ItemNo = *(ParaBuff+i);
		if( SItemNo != 0xff ) StartAddr = 0;
		else StartAddr = GetRecItemStartAddr( EventNo, ItemNo );	
		Len = *(PointTab[EventNo].InArrayAddr+ItemNo-1);
		RAM_Fill( Dest+StartAddr, Len);								//历史数据还未产生，返回全零
		if( SItemNo != 0xff ) break;								//单条清零后退出
	}		
}	


//事件
unsigned char NB1Proc_ReadEve( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i;
	unsigned char* ReadBufAds;
  	unsigned char Err_Byte=0;
	unsigned char State=0;
	long Value;
	unsigned short StartAddr;
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	
	switch( Info & EVComm_Type )
	{
		case EVPARAM:
			E2P_RData( ReadBufAds, Addr, Len ); 					//次数、累计时间等
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case EVENO:
			for( i=0;i<TamperNum;i++ )
			{
				if( TamperTab[i].EventNo == Addr )
				{
					if((Flag.Power & F_PwrUp) != 0) State = TamperTab[i].Event_State->OldState;	//取该事件的当前状态是否结束，决定最近一次的事件记录数据是从EEPROM中还是FLASH中读取						
					else E2P_RData( (unsigned char*)&State, TamperTab[i].EOldState, 1 );
					break;										//是否为电表状态异常事件记录
				}	
			}	
							
			if( ( *(ComBuf+NRs_ID1Ptr) != 0xff ) )
			{		
				Len = *(PointTab[Addr].InArrayAddr+*(ComBuf+NRs_ID1Ptr) - 1 );							
				StartAddr = GetRecItemStartAddr( Addr, *(ComBuf+NRs_ID1Ptr) );			
			}				
			if(( Info & FLASH ) != 0 )							//历史数据存放于FLASH
			{
															        //所有事件记录都放在FLASH中或历史记录放在FLASH中											
				Value = PtAddrChk( Addr, *(ComBuf+NRs_ID0Ptr)-1 );	//取该事件该次记录在FLASH中的首地址
				if( Value != -1 )
				{
					if( *(ComBuf+NRs_ID1Ptr) == 0xff )
					{
						Read_Flash( ReadBufAds, Value, Len );						
						if(( *(ComBuf+NRs_ID0Ptr) == 0x01 )&&( State != 0 )) 	//最近一次未结束事件放在FLASH中
						{												
							ClrRecNullItemContent( ReadBufAds, Addr, *(ComBuf+NRs_ID1Ptr) );
						}
					}
					else
					{
						Read_Flash( ReadBufAds, Value+StartAddr, Len );
						if(( *(ComBuf+NRs_ID0Ptr) == 0x01 )&&( State != 0 )) 	//最近一次未结束事件放在FLASH中
						{												
							ClrRecNullItemContent( ReadBufAds, Addr, *(ComBuf+NRs_ID1Ptr) );
						}
					}		
				}
				else
				{
					RAM_Fill( ReadBufAds, Len);					//无历史数据，返回全零								
				}						
			}
			else
			{
				Value = PtAddrChk( Addr, *(ComBuf+NRs_ID0Ptr)-1 );		//取该事件该次记录在EEPROM中的首地址
				if( Value != -1 )
				{
					if( *(ComBuf+NRs_ID1Ptr) == 0xff )
					{
						E2P_RData( ReadBufAds, (unsigned short)Value, Len );						
					}
					else
					{
						E2P_RData( ReadBufAds, (unsigned short)Value+StartAddr, Len );												
					}		
				}
				else RAM_Fill( ReadBufAds, Len);						//无历史数据，返回全零	
			}		
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		default: break; 								
	}
  return Err_Byte;	
}	


//////////////////////////////////////////////////////////////////////////
//参变量
short NGetParaComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID )
{
	short i;
	unsigned long Value,Value2;
  	
  	RAM_Write( (unsigned char*)&Value, ComID, 4 );
  	Value2 = Value & 0xffffff00;
//  	if(( Value2 == 0x04030000 )||( Value2 == 0x04040100 )||( Value2 == 0x04040200 )) //公假日、循环显示项、按键显示项需特殊处理
  	if( Value2 == 0x04030000 ) 								//公假日需特殊处理,254项
  	{
  		if(( *ComID == 0x00 )||( *ComID == 0xFF )) return -1;
  		Value = Value2 + 1;
  	}	

  	if(( Value2 == 0x04040100 )||( Value2 == 0x04040200 )) //循环显示项、按键显示项需特殊处理,最多99项
  	{
  		if(( *ComID == 0x00 )||( *ComID > 99 )) return -1;
  		Value = Value2 + 1;
  	}	

  	
  	for( i=0;i<ParaComNum;i++ )
  	{
		if( Value == ParaComTab[i].ComID ) 
		{
			*Addr = ParaComTab[i].Addr;
			*Info = ParaComTab[i].ComInfo;
			*Len = *Info & RS_LEN;
			return i;
		}
  	}
	return -1;
}
//参变量
void NProc_ReadPara( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i,Length;
	unsigned char* ReadBufAds;
	unsigned long ComID;
	unsigned char Temp;					//新国网	//13.09.26
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	ComID = ParaComTab[ItemAds].ComID;
	
	switch( Info & Comm_Type )
	{
		case OTHER:
			if(( ComID >= 0x04000501 )&&( ComID <= 0x04000508 ))		//新国网
			{
				RAM_Write( ReadBufAds, (unsigned char*)Addr, Len );		//运行状态字
			}
			else if( ComID == 0x04000101 )
			{
				*ReadBufAds = Clk.Week[0];
				RAM_Write( ReadBufAds+1, Clk.Day, 3 );		//日期
			}	
			else if( ComID == 0x04000102 )
			{
				RAM_Write( ReadBufAds, Clk.Sec, 3 );		//时间			
			}	
			else if( ComID == 0x0400010C )					//日期时间	//新国网		
			{
				RAM_Write( ReadBufAds, Clk.Sec, 3 );		//时间			
				*(ReadBufAds+3) = Clk.Week[0];
				RAM_Write( ReadBufAds+4, Clk.Day, 3 );		//日期
			}	
			else if( ComID == 0x04000105 )
			{
				*ReadBufAds = 0x80;							//脉冲宽度	
				*(ReadBufAds+1) = 0x00;						
			}
			else if( ComID == 0x04001301 )
			{
				*ReadBufAds = Para.NetIntensity;			//无线信号强弱
			}		
			else if( ComID == 0x04001501 )					//主动上报状态字	//新国网
			{
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
				CreatEventUpSubsFrame( ComBuf );
				break;
#else
				MatchEventUpMode();							//新国网	//13.10.10	
				RAM_Write( ReadBufAds, Flag.EventUpState, 10 );
				RAM_Fill( Flag.EventUpState, 10 );
				E2P_WData( EEventUpState, Flag.EventUpState, 10 );	
				POUT_EVENTOUT &= ~P_EVENTOUT;				//取消事件报警脚报警状态  				
#endif
			}				
							
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case PARAM:
			if( ComID == 0x04030001 ) 									//公假日需特殊处理
			{
				Length = *(ComBuf+NRs_ID0Ptr)-1;
				E2P_RData( ReadBufAds, Addr+Length*5, Len ); 
			}	
			else if( ComID == 0x04001104 )					//主动上报模式字	//16.05.17
			{
				GetEventUpMode( ReadBufAds );	
			}		
			else if(( ComID == 0x04040101 )||( ComID == 0x04040201 )) 	//循环显示项、按键显示项需特殊处理
			{
				Length = *(ComBuf+NRs_ID0Ptr)-1;
				E2P_RData( ReadBufAds, Addr+Length*6, 5 ); 
			}	
			else if(( ComID == 0x04010000 )||( ComID == 0x04020000 ))	//两套时区				//新国网	//13.09.26
			{
				E2P_RData( ReadBufAds, Addr, Len ); 					
				E2P_RData( (unsigned char*)&Temp, ZoneSum, 1 );
				Temp = BCD_Byte( Temp );	
				if( Temp > ZONENUMBER ) Temp = ZONENUMBER;
				Temp *= 3;
				if( Len > Temp ) Len = Temp;
			}	
			else if((( ComID >= 0x04010001 )&&( ComID <= 0x04010008 ))			//两套时段		//新国网	//13.09.26
					||(( ComID >= 0x04020001 )&&( ComID <= 0x04020008 ))) 
			{
				E2P_RData( ReadBufAds, Addr, Len ); 					
				E2P_RData( (unsigned char*)&Temp, SegSum, 1 );
				Temp = BCD_Byte( Temp );	
				if( Temp > SEGNUMBER ) Temp = SEGNUMBER;
				Temp *= 3;
				if( Len > Temp ) Len = Temp;
			}			
			else if(( ComID == 0x040604FF )||( ComID == 0x040605FF )) 	//当前和备用套阶梯数据		//新国网
			{
				for( i=0;i<13;i++ )
				{
					E2P_RData( ReadBufAds+i*4, Addr+i*5, 4 );					
				}	
				for( i=0;i<4;i++ )
				{
					E2P_RData( ReadBufAds+52+i*3, Addr+65+i*4, 3 );					
				}					
			}	
			else
			{
				E2P_RData( ReadBufAds, Addr, Len ); 					
			}
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case DATABLK:
			Length = Len;
			for( i=1;i<Length+1;i++ )
			{
				Addr = (unsigned short)ParaComTab[ ItemAds+i ].Addr;
				Info = ParaComTab[ItemAds+i].ComInfo;
				Len = Info & RS_LEN;
				NProc_ReadPara( ItemAds+i, Addr, Info, Len, ComBuf );
			}
			break;
		default: break; 								
	}	
}	

//整点冻结量
unsigned char NProc_ReadHourDJ( unsigned char* ComBuf )
{	
	unsigned char* ReadBufAds;
	long Value;
	unsigned short Len;
  	unsigned char Err_Byte=0;
	unsigned char DJMode;
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);

	E2P_RData( (unsigned char*)&DJMode, HourFreezeMode, 1 );
	DJMode &= 0x03;
	if(( DJMode == 0 )||(( *(ComBuf+NRs_ID1Ptr) == 0x01 )&&(( DJMode & 0x01 ) == 0 ))
		||(( *(ComBuf+NRs_ID1Ptr) == 0x02 )&&(( DJMode & 0x02 ) == 0 )))
	{ 
		Err_Byte |= RS_State_IVID; 
		return Err_Byte;	
	}

	switch( *(ComBuf+NRs_ID1Ptr))
	{
		case 0x00:							//整点冻结时间
			Len = 5;
			break;	
		case 0x01:							//整点冻结正向有功总电量	
		case 0x02:							//整点冻结反向有功总电量	
			Len = 4;
			break;
		default:
			Len = 13;				
			break;	
	}

	Value = PtAddrChk( HourFreeze_No, *(ComBuf+NRs_ID0Ptr)-1 );					
	if( Value != -1 )
	{
		switch( *(ComBuf+NRs_ID1Ptr))
		{
			case 0x01:							//整点冻结正向有功总电量	
				Value += 5;
				break;	
			case 0x02:							//整点冻结反向有功总电量	
				Value += 9;
				break;
			default:
				break;	
		}
		Read_Flash( ReadBufAds, (unsigned long)Value, Len );	//读冻结数据
	}	
	else
	{
		RAM_Fill( ReadBufAds, Len);								//无冻结数据，返回全零	
	}	
	if( Len == 13 )
	{
		RAM_Write( ReadBufAds+20, ReadBufAds+5, 8 );
		*(ReadBufAds+5) = 0xAA;
		RAM_Write( ReadBufAds+6, ReadBufAds+20, 4  );
		*(ReadBufAds+10) = 0xAA;
		RAM_Write( ReadBufAds+11, ReadBufAds+24, 4  );
		*(ReadBufAds+15) = 0xAA;
		Len = 16;
		switch( DJMode )
		{
			case 0x01:
				*(ReadBufAds+11) = 0xAA;
				Len = 12;
				break;	
			case 0x02:
				*(ReadBufAds+6) = 0xAA;
				RAM_Write( ReadBufAds+7, ReadBufAds+24, 4  );
				*(ReadBufAds+11) = 0xAA;
				Len = 12;
				break;	
			default:
				break;			
		}	
	}
	*(ComBuf + Rs_LenPtr) += Len;

	return Err_Byte;
}

//冻结量
short NGetDJComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID )
{
	short i;
	unsigned long Value;
	unsigned char Buff[4];
	
  	RAM_Write( Buff, ComID, 4 );
  	if(( Buff[2] > 0x03 )&&( Buff[2] != 0x06 )) return -1;			//不支持阶梯切换冻结数据的读取。		//新国网	//13.09.21
  	Buff[2] = 0;
  	RAM_Write( (unsigned char*)&Value, Buff, 4 );
	Value &= 0xffffff00;
	Value += 1;
  	
  	for( i=0;i<DJComNum;i++ )
  	{
		if( Value == DJComTab[i].ComID ) 
		{
			*Addr = DJComTab[i].Addr;
			*Info = DJComTab[i].ComInfo;
			*Len = *Info & RS_LEN;
			return i;
		}
  	}
	return -1;
}

//冻结量
unsigned char NProc_ReadDJ( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i,Length;
	unsigned char* ReadBufAds;
	long Value;
	unsigned short EventNo;
  	unsigned char Err_Byte=0;
	unsigned char DJMode;
	
	switch( *(ComBuf+NRs_ID2Ptr))
	{
		case 0x00:
			E2P_RData( (unsigned char*)&DJMode, TimeFreezeMode, 1 );
			break;	
		case 0x01:
			E2P_RData( (unsigned char*)&DJMode, InstantFreezeMode, 1 );
			break;	
		case 0x02:							//时区
		case 0x03:							//时段
		case 0x05:							//费率电价
		case 0x07:							//阶梯电价
			E2P_RData( (unsigned char*)&DJMode, PromiseFreezeMode, 1 );
			break;	
		case 0x06:							//日冻结
			E2P_RData( (unsigned char*)&DJMode, DayFreezeMode, 1 );
			break;	
		default: break;				
	}	
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	
	Length = 0;
	switch( Info & Comm_Type )
	{
		case PARAM:
			switch( *(ComBuf+NRs_ID1Ptr))
			{
				case 0x01:
					if(( DJMode & 0x01 ) == 0 )	Length = Len;	
					break;	
				case 0x02:
					if(( DJMode & 0x02 ) == 0 )	Length = Len;	
					break;	
				case 0x03:
					if(( DJMode & 0x04 ) == 0 )	Length = Len;	
					break;
				case 0x04:
					if(( DJMode & 0x08 ) == 0 )	Length = Len;	
					break;
				case 0x05:
				case 0x06:
				case 0x07:
				case 0x08:
					if(( DJMode & 0x10 ) == 0 )	Length = Len;	
					break;
				case 0x09:
					if(( DJMode & 0x20 ) == 0 )	Length = Len;	
					break;	
				case 0x0A:
					if(( DJMode & 0x40 ) == 0 )	Length = Len;	
					break;	
				case 0x10:
					if(( DJMode & 0x80 ) == 0 )	Length = Len;	
					break;	
				default: break;				
			}	

			if( Length != 0 )
			{
//				*ReadBufAds = 0xAA;
//				RAM_Fill( ReadBufAds, Len);
//				*(ComBuf + Rs_LenPtr) += Len;
				Err_Byte |= RS_State_IVID;
				break;		
			}

			switch( *(ComBuf+NRs_ID2Ptr))
			{
				case 0x00:
					EventNo = AppointFreeze_No;
					break;	
				case 0x01:
					EventNo = InstantFreeze_No;
					break;	
				case 0x02:
					EventNo = ZoneSWFreeze_No;
					break;	
				case 0x03:
					EventNo = SegTabSWFreeze_No;
					break;
				case 0x05:
					EventNo = FeeSWFreeze_No;
					break;	
//				case 0xB0:
				case 0x06:							//日冻结
//					EventNo = ZdyFreeze_No;
					EventNo = DayFreeze_No;
					break;	
				case 0x07:
					EventNo = StepSWFreeze_No;
					break;	
				default: break;				
			}	
		
			Value = PtAddrChk( EventNo, *(ComBuf+NRs_ID0Ptr)-1 );					
			if( Value != -1 )
			{
				Value += Addr;
				if( Len == 20 ) Length = 5*(ECDIGIT+1);					//取全部电量数据，包括校验和
				else if( Len == 40 ) Length = 5*9;						//取全部需量数据，包括校验和	
				else Length = Len;
				Read_Flash( ReadBufAds, (unsigned long)Value, Length );	//读冻结数据
				if( Len == 20 )
				{
					for( i=0;i<5;i++ ) 
					{
						RAM_Write( ReadBufAds+i*4, ReadBufAds+i*(ECDIGIT+1)+ECDIGIT-4, 4 );		//取4字节电量				
					}							
				}	
				else if( Len == 40 )
				{
					for( i=0;i<5;i++ ) 
					{
						RAM_Write( ReadBufAds+i*8, ReadBufAds+i*9, 8 );							//取8字节需量				
					}							
				}	
			}	
			else
			{
				RAM_Fill( ReadBufAds, Len);								//无冻结数据，返回全零	
			}	
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case DATABLK:
//			Err_Byte = GetDJBlockRec( ComBuf );					
			Err_Byte = GetDJBlockRec( ComBuf, DJMode );					
			break;
		default: break; 								
	}	
	return Err_Byte;
}
	
short GetCOM_No( unsigned char* ComBuf )
{
	short COM_No=0;
	
	if( ComBuf == Comm.Buf0 ) COM_No = 0;
	if( ComBuf == Comm.Buf1 ) COM_No = 1;
	if( ComBuf == Comm.Buf2 ) COM_No = 2;
	if( ComBuf == Comm.Buf3 ) COM_No = 3;
	
	return COM_No;
}		

//返回值：0：无后续数据	0xB0: 有后续数据
unsigned char NProc_ReadLoad( unsigned char* ComBuf )
{
	short COM_No; 
	short Length;
  	unsigned char Back_Byte=0;
  	unsigned char BlockSum;
	unsigned char* ReadBufAds;
	unsigned char* Ptr;
	
	COM_No = GetCOM_No( ComBuf );
	Ptr = CommLoadPara[COM_No].SubsPara;

	BlockSum = BCD_Byte( *(ComBuf+NRs_RDataPtr) );
	ReadBufAds = ComBuf+NRs_RDataPtr;
	if( BlockSum == 0 )
	{
		RAM_Fill( Ptr, 6);							//无后续数据帧，清空后继数据帧参数
		return Back_Byte;
	}	
	
	switch( *(ComBuf+NRs_ID0Ptr) )
	{
		case 0x00:							//最早记录块
			Length = GetFirstLoadData( ReadBufAds, ComBuf, 1 );
			break;	
		case 0x01:							//给定时间记录块
			Length = GetTimeLoadData( ReadBufAds, ComBuf );
			break;	
		case 0x02:							//最近一个记录块
			Length = GetFirstLoadData( ReadBufAds, ComBuf, 0 );
			break;	
		default: break;		
	}	

	if( Length == 0 ) BlockSum = 0;			//无相应的负荷曲线数据
	else
	{
		*(ComBuf + Rs_LenPtr) += Length;
		BlockSum -= 1;
	}	
	if( *(ComBuf+NRs_ID2Ptr) != 0 )			//单通道负荷曲线考虑在一帧中打包几块负荷数据块以增加通讯效率。
	{
		while(( BlockSum > 0 )&&(( *(ComBuf + Rs_LenPtr) + Length ) < 200 )&&( Length != 0 ))	
		{
			ReadBufAds += Length;
#if( LASTBLOCKProcess == YesCheck )							//13.03.16	
			Length = GetSubsLoadData( ReadBufAds, ComBuf, 1 );	//剩余记录读取
#else
			Length = GetSubsLoadData( ReadBufAds, ComBuf );	//剩余记录读取
#endif
			if( Length == 0 ) BlockSum = 0;					//无后继数据
			else
			{	
				*(ComBuf + Rs_LenPtr) += Length;
				BlockSum -= 1;					
			}	
		}	
	}	
	*(Ptr+4) = BlockSum;			//后继记录块数
//	*(Ptr+5) = 1;					//帧序列号
//	*(Ptr+5) = 0;					//帧序列号  //09.12.20	影响读时标
#if( LASTBLOCKProcess == YesCheck )							//13.03.16	
	if( BlockSum != 0 )
	{
		Length = GetSubsLoadData( ReadBufAds, ComBuf, 0 );	//判断是否还有后续剩余记录可以读取
		if( Length == 0 ) BlockSum = 0;	
	}	
#endif
	if( BlockSum != 0 ) Back_Byte = 0xB0;		//有后续数据帧
	else RAM_Fill( Ptr, 6);						//无后续数据帧，清空后继数据帧参数
			
	return Back_Byte;		
}		


unsigned char NCom_Write(unsigned char* ComBuf )
{
	unsigned short	Addr, Info, Len;
  	unsigned char Err_Byte=0;
	short ItemAds;
	unsigned long Value;

	if( *(ComBuf+NRs_ID3Ptr) != 0x04 ) return RS_State_IVID;	//非参变量类不能写。
	
	ItemAds = NGetParaCom1Var( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );			//第1类数据不能用普通方式写
	if( ItemAds != -1 )
	{
		Err_Byte |= RS_State_IVID;
		return	Err_Byte;
	}
	
	ItemAds = NGetParaComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
	if(( ItemAds == -1 )||((( PIN_SETEN & P_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))) 		//新国网	//14.04.09
	{
		Err_Byte |= RS_State_IVID;
		return	Err_Byte;
	}

	RAM_Write( (unsigned char*)&Value, ComBuf+NRs_ID0Ptr, 4 );
//	if( ((( PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))&&((( Value >= 0x04000404 )&&( Value <= 0x0400040D ))||(( Value >= 0x04800001 )&&( Value <= 0x04800004 )))) ;	//写额定电压电流等		//新国网  	//13.09.10	//14.04.09
	if( ((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( IsFactoryTestState() == 0 ))&&((( Value >= 0x04000404 )&&( Value <= 0x0400040D ))||(( Value >= 0x04800001 )&&( Value <= 0x04800004 )))) ;
	else
	{
		if( ~Info & EN_W ) 
		{
//			Err_Byte |= RS_State_IVData;
			Err_Byte |= RS_State_PswE;		//写无写权限的数据，返回"密码错/无授权"	
//			*ComBuf = 0xFF;					//
			return	Err_Byte;
		}
	}	
	if(( Value >= 0x04000C01 )&&( Value <= 0x04000C0A ))
	{
		if( *(ComBuf+NRs_PassWPtr) >= *(ComBuf+NRs_ID0Ptr) )
		{
			Err_Byte |= RS_State_PswE;		//密码权限有误
			return Err_Byte;
		}				
	}			

	*(ComBuf + Rs_Head0Ptr) = 12;		//标识码+密码长度+操作者代码   第二个变量暂做地址缓存
	Err_Byte = JudgeValid( ItemAds, Addr, Info, Len, ComBuf );	//判断设置数据有效性
	*(ComBuf + Rs_Head0Ptr) = 0x68;
	if( Err_Byte != 0 ) return Err_Byte;
	
	*(ComBuf + Rs_Head0Ptr) = 12;		//标识码+密码长度+操作者代码   第二个变量暂做地址缓存
	Err_Byte = NProc_Write( ItemAds, Addr, Info, Len, ComBuf );
	*(ComBuf + Rs_Head0Ptr) = 0x68;
	return	Err_Byte;
}
*/
//判断时间是否合法 时：00～23点、分：00～59分
unsigned char JudgeHourM( unsigned char* Source )
{
	if(( *Source > 0x59 )||(*(Source+1) > 0x23)) return RS_State_IVData;
	else return 0;
}	

//判断时间是否合法 日：1～28点、时：00～23时
unsigned char JudgeDayH( unsigned char* Source )
{
	if(( *Source > 0x23 )||(*(Source+1) > 0x28)||(*(Source+1) < 0x01)) return RS_State_IVData;
	else return 0;
}	

//判断日期是否合法 月：1～12月、日：根据各月定义
unsigned char JudgeMonD( unsigned char* Source )
{
	if(( *(Source+1) > 0x12 )||(*(Source+1) == 0)) return RS_State_IVData;
	else 
	{
		if(( *Source == 0 )||(BCD_Byte(*Source) > MonTab[BCD_Byte(*(Source+1))])) return RS_State_IVData;				
	}	
	return 0;
}
/*
unsigned char JudgeFreezeDateTime( unsigned char* Source )
{
	if( IsAllData( Source, 4, 0x99 ) == 0 ) return 0;	
	if( *(Source+3) == 0x99 )
	{
		if( *(Source+2) == 0x99 )
		{
			if( *(Source+1) == 0x99 )
			{
				if( *Source <= 0x59 ) return 0; 	
			}
			else
			{
				if( JudgeHourM( Source ) == 0 ) return 0;
			}				
		}
		else 
		{
			if(( *( Source+2 ) > 0 )&&( *( Source+2 ) <= 0x28 )&&( JudgeHourM( Source ) == 0 )) return 0;
		}	
	}	
	else
	{
		if(( JudgeMonD( Source+2 ) == 0 )&&( JudgeHourM( Source ) == 0 )) return 0;
	}	
	return 1;
}

unsigned char JudgeSegInterval( unsigned char* Source, unsigned short Length )		//新国网	//13.10.18
{
	short i;
	unsigned char Err_Byte=0;
	unsigned char* Ptr;
	unsigned short Time1 = 0;
	unsigned short Time2 = 0;
	
	for( i=0;i<(Length/3);i++ )
	{
		Ptr = Source+i*3;

		if( (i+1) < (Length/3) )											//后面至少还有一项
		{
//			if( Data_Comp( Ptr, Ptr+3, 3 ) == 0 ) return 0;
			if( Data_Comp( Ptr, Ptr+3, 3 ) == 0 ) continue;					//新国网	//13.12.27
			Time1 = BCD_Byte( *(Ptr+2) );
			Time1 *= 60;
			Time1 += BCD_Byte( *(Ptr+1) );
			Time2 = BCD_Byte( *(Ptr+3+2) );
			Time2 *= 60;
			Time2 += BCD_Byte( *(Ptr+3+1) );
			
			if(( Time2 - Time1 ) < 15 ) 									//时段间隔小于15分钟
			{
				Err_Byte |= RS_State_IVData;
//				break;								
				return Err_Byte;											//新国网	//14.01.26
			}				
		}	
	}

	if( (Length/3) == 1 ) return Err_Byte;									//新国网	//13.12.27	
	else																	//新国网	//13.12.27
	{
//		if( *Source == *(Ptr+3) ) return 0;									//头尾费率相同
		if( *Source == *Ptr ) return 0;	    								//头尾费率相同  //新国网	//14.01.26
		Time1 = BCD_Byte( *(Source+2) );
		Time1 *= 60;
		Time1 += BCD_Byte( *(Source+1) );
		Time1 += 1440;														//加一整天时间
		if(( Time1 - Time2 ) < 15 ) 										//时段间隔小于15分钟
		{
			Err_Byte |= RS_State_IVData;
		}				
	}	

	return Err_Byte;
}	

unsigned char JudgeSeg( unsigned char* Source, unsigned short Length )
{
//	unsigned char Buff[3];
//	unsigned char* Point;
	short i;
	unsigned char Err_Byte=0;
	unsigned char* Ptr;
//	Point = Buff;
	
	if((( Length % 3 ) != 0 )||( AllIsBCD( Source, Length ) != 0 )) Err_Byte |= RS_State_IVData;
	else
	{
		if(( Length == 0 )||( Length > ( SEGNUMBER*3 ))) Err_Byte |= RS_State_IVData;
#if ( AnHuiRequire == YesCheck )												//安徽允许时段设成全零			
		else if( IsAllData( Source, Length, 0x00 ) == 0 ) Err_Byte = 0;		
#endif
		else
		{
			for( i=0;i<(Length/3);i++ )
			{
				Ptr = Source+i*3;
				if(( JudgeHourM( Ptr+1 ) != 0 )||( *Ptr == 0 )||( *Ptr > 4 ))
				{
					Err_Byte |= RS_State_IVData;
					break;										
				}										//时间非法
				if( (i+1) < (Length/3) )											//后面至少还有一项
				{
					if(( Data_Comp( Ptr+1, Ptr+3+1, 2 ) == 1 )						//从小到大排列
						||(( Data_Comp( Ptr+1, Ptr+3+1, 2 ) == 0 )&&(*Ptr!=*(Ptr+3))))	//日期相等但时段表号不等
					{
						Err_Byte |= RS_State_IVData;
						break;
					}
				}	
			}
			if( Err_Byte == 0 )							//新国网	//13.10.18
			{
				Err_Byte |= JudgeSegInterval( Source, Length );
			}	
		}			
	}	
	return Err_Byte;
}

unsigned char JudgeZone( unsigned char* Source, unsigned short Length )
{
	short i;
	unsigned char Err_Byte=0;
	unsigned char* Ptr;
	
	if((( Length % 3 ) != 0 )||( AllIsBCD( Source, Length ) != 0 )) Err_Byte |= RS_State_IVData;
	else
	{
		if(( Length == 0 )||( Length > (ZONENUMBER*3))) Err_Byte |= RS_State_IVData;
#if ( AnHuiRequire == YesCheck )												//安徽允许时区设成全零			
		else if( IsAllData( Source, Length, 0x00 ) == 0 ) Err_Byte = 0;		
#endif
		else
		{
			for( i=0;i<(Length/3);i++ )
			{
				Ptr = Source+i*3;

				if(( JudgeMonD( Ptr+1 ) != 0 )||( *Ptr == 0 )||( *Ptr > 8 ))		//日期非法或时段表号非法
				{
					Err_Byte |= RS_State_IVData;
					break;
				}
				if( (i+1) < (Length/3) )											//后面至少还有一项
				{
					if(( Data_Comp( Ptr+1, Ptr+3+1, 2 ) == 1 )						//从小到大排列
						||(( Data_Comp( Ptr+1, Ptr+3+1, 2 ) == 0 )&&(*Ptr!=*(Ptr+3))))	//日期相等但时段表号不等
					{
						Err_Byte |= RS_State_IVData;
						break;
					}
				}	
			}
		}			
	}	
	return Err_Byte;				
}

unsigned char JudgeValid( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned char* WriteBufAds;
  	unsigned char Buff[20];
  	unsigned char* Point;
  	unsigned char Err_Byte=0;
  	unsigned long ComID;

  	Point = Buff;

	WriteBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_Head0Ptr);
	
	ComID = ParaComTab[ ItemAds ].ComID;
	
	if(( Info & BCD ) != 0 ) 
	{
		if( AllIsBCD( ComBuf+NRs_WDataPtr, Len ) != 0 )
		{
			Err_Byte |= RS_State_IVData;					//非BCD码返回错误
			return	Err_Byte;
		}	
	}	
	
	if((( *(ComBuf + NRs_ID2Ptr) == 0x02 )||( *(ComBuf + NRs_ID2Ptr) == 0x01 ))&&( *(ComBuf + NRs_ID1Ptr) == 0x00 )) ;		//时区时段单独判长度
	else
	{
		if(*(ComBuf + Rs_LenPtr) != (Len + 12))
		{
//			if((( ComID == 0x04040101 )||( ComID == 0x04040201 ))&&( *(ComBuf + Rs_LenPtr) == 16 )) ;		//显示码需5字节
//			else
//			{
				Err_Byte |= RS_State_IVData;					//数据长度不对返回错误
				return	Err_Byte;			
//			}	
		}	
	}	
	
	switch( Info & Comm_Type )
	{
		case PARAM:		
				if( ComID == 0x04000103 )								//最大需量周期
				{
					if((*WriteBufAds != 0x5 )&&(*WriteBufAds != 0x10 )&&(*WriteBufAds != 0x15 )&&(*WriteBufAds != 0x30 )&&(*WriteBufAds != 0x60 ))
					{	
						Err_Byte |= RS_State_IVData;
					}
					else
					{
//						if( *WriteBufAds != Para.RMaxT ) MD_InitialCalPara( *WriteBufAds );	//需量周期变化，头一个周期内不计算需量
						if( BCD_Byte( *WriteBufAds ) != Para.RMaxT ) MD_InitialCalPara( BCD_Byte( *WriteBufAds ) );	//需量周期变化，头一个周期内不计算需量
					}						
				}
				else if( ComID == 0x04000104 )							//滑差时间
				{
					if((*WriteBufAds > 0x60 )||(*WriteBufAds < 1 )) Err_Byte |= RS_State_IVData;
				}		
				if(( ComID >= 0x04000106 )&&( ComID <= 0x04000109 ))	//切换时间
				{
					if( IsAllData( WriteBufAds, 0x05, 0x00 ) != 0 )
					{
						if( DateTime5ValueCheck( WriteBufAds ) != 0 ) Err_Byte |= RS_State_IVData;
					}		
				}
				else if( ComID == 0x04001201 )							//整点冻结起始时间	
				{
					if( DateTime5ValueCheck( WriteBufAds ) != 0 ) Err_Byte |= RS_State_IVData;
				}	
				else if( ComID == 0x04000201 )								//年时区数
				{
					if(( BCD_Byte(*WriteBufAds) > ZONENUMBER )||(*WriteBufAds == 0x00 )) Err_Byte |= RS_State_ZoneOver;	
				}	
				else if( ComID == 0x04000202 )								//日时段表数
				{
					if(( *WriteBufAds > 0x08 )||(*WriteBufAds == 0x00 )) Err_Byte |= RS_State_IVData;	
				}	
				else if( ComID == 0x04000203 )								//日时段数
				{
					if(( BCD_Byte(*WriteBufAds) > SEGNUMBER )||(*WriteBufAds == 0x00 )) Err_Byte |= RS_State_SegOver;	
				}	
				else if( ComID == 0x04000204 )								//费率数 = 4
				{
//					if( *WriteBufAds != 0x04 ) Err_Byte |= RS_State_FeeOver;	
					if( *WriteBufAds > 0x04 ) Err_Byte |= RS_State_FeeOver;	//新国网
				}	
				else if( ComID == 0x04000207 )								//梯度数 <= 10
				{
//					if( BCD_Byte( *WriteBufAds ) > 10 ) Err_Byte |= RS_State_IVData;	
					if( BCD_Byte( *WriteBufAds ) > 6 ) Err_Byte |= RS_State_IVData;			//新国网
				}	
				else if( ComID == 0x04000205 )								//公共假日数
				{
					if( BCD2_Word(WriteBufAds) > 254 ) Err_Byte |= RS_State_IVData;	
				}	
				else if( ComID == 0x04000206 )								//谐波分析次数 = 1次
				{
					if( *WriteBufAds != 0x01 ) Err_Byte |= RS_State_IVData;	
				}	
				else if( ComID == 0x04000302 )								//每屏显示时间 != 0		//10.09.21
				{
					if( *WriteBufAds == 0x00 ) Err_Byte |= RS_State_IVData;	
				}	
				else if( ComID == 0x04000303 )								//显示电能小数位数 = 2
				{
//					if( *WriteBufAds != 0x02 ) Err_Byte |= RS_State_IVData;	
//					if( *WriteBufAds > 0x04 ) Err_Byte |= RS_State_IVData;	
					if( *WriteBufAds > 0x03 ) Err_Byte |= RS_State_IVData;	//新国网	13.10.19
				}	
				else if( ComID == 0x04000304 )								//显示功率小数位数 = 4
				{
//					if( *WriteBufAds != 0x04 ) Err_Byte |= RS_State_IVData;	
					if( *WriteBufAds > 0x04 ) Err_Byte |= RS_State_IVData;	
				}	
				else if(( ComID >= 0x04000701 )&&(( ComID <= 0x04000705 )))								//显示功率小数位数 = 4
				{					
					if( JustBandRate( *WriteBufAds ) != 0 ) Err_Byte |= RS_State_IVData;
					if((*(ComBuf + NRs_ID0Ptr) == 0x01)&&(*WriteBufAds != 0x04 )) Err_Byte |= NRS_State_Baut;  //调制型红外只能1200						
				}	
				else if( ComID == 0x04000802 )								//周休日采用的日时段表号
				{
					E2P_RData( Point, SegTabSum, 1 );
					if( *Point > 8 ) *Point = 8;
					if(( *WriteBufAds == 0x00 )||( *WriteBufAds > *Point )) Err_Byte |= RS_State_IVData;	
				}	
				else if( ComID == 0x04000A01 )								//负荷记录起始时间
				{
					if(( JudgeMonD(WriteBufAds+2) !=0 )||( JudgeHourM( WriteBufAds ) !=0 )) Err_Byte |= RS_State_IVData;
				}	
				else if(( ComID >= 0x04000A02 )&&( ComID <= 0x04000A07 ))				//负荷间隔
				{
					if(( *WriteBufAds == 0x00 )&&( *(WriteBufAds+1) == 0x00 )) Err_Byte |= RS_State_IVData;
				}	
				else if(( ComID >= 0x04000B01 )&&( ComID <= 0x04000B03 ))				//结算日
				{
					if((( *WriteBufAds == 0x99 )&&( *(WriteBufAds+1) == 0x99 ))||( JudgeDayH( WriteBufAds ) == 0 )) ;
					else Err_Byte |= RS_State_IVData;					
				}	
				else if(( ComID >= 0x04000C01 )&&( ComID <= 0x04000C0A ))				//密码
				{
					if( *WriteBufAds != (*(ComBuf+NRs_ID0Ptr)-1) ) Err_Byte |= RS_State_IVData;
				}				
				else if( ComID == 0x04090E01 )								//功率因数限值
				{
					if( BCD2_Word(WriteBufAds) > 1000 ) Err_Byte |= RS_State_IVData;
				}			
				else if( ComID == 0x04030001 ) 								//公假日需特殊处理
				{
					if( IsAllData( WriteBufAds, 4, 0x00 ) != 0 )
					{
						E2P_RData( Point, SegTabSum, 1 );
						if( *Point > 8 ) *Point = 8;
						RAM_Write( Point+3, WriteBufAds+1, 3 );
						*(Point+2) = 0x00;
						if((( IsAllData( WriteBufAds+1, 3, 0x99 ) == 0 )||( DateValueCheck( Point+2 ) == 0 ))&&(*WriteBufAds<=*Point)&&(*WriteBufAds!=0)) ;
						else Err_Byte |= RS_State_IVData;					
					}	
				}
				else if(( ComID == 0x04010000 )||( ComID == 0x04020000 ))	//时区表
				{
					Err_Byte = JudgeZone( ComBuf+NRs_WDataPtr, *(ComBuf+Rs_LenPtr) - 12 );

				}
				else if((( ComID >= 0x04010001 )&&( ComID <= 0x04010008 ))||(( ComID >= 0x04020001 )&&( ComID <= 0x04020008 )))	//时段表
				{
					Err_Byte = JudgeSeg( ComBuf+NRs_WDataPtr, *(ComBuf+Rs_LenPtr) - 12 );
				
				}				

			break;
		case OTHER:
	    		if( ComID == 0x04000101 )
	    		{
					if( DateValueCheck( WriteBufAds ) != 0 ) Err_Byte |= RS_State_IVData;
				}
				else if( ComID == 0x04000102 )
	    		{
					if( TimeValueCheck( WriteBufAds ) != 0 ) Err_Byte |= RS_State_IVData;
				}
				else if( ComID == 0x0400010C )			//新国网
	    		{
					if(( TimeValueCheck( WriteBufAds ) != 0 )&&( DateValueCheck( WriteBufAds+3 ) != 0 )) Err_Byte |= RS_State_IVData;
				}
			break;
		default: break;
	}
	return Err_Byte;		
}
			
short CreatSinglePrgRec( unsigned long ComID, unsigned short Addr )
{
	unsigned char Buff[20];
	unsigned char* Ptr;
//	unsigned short LastPt;
	long LastPt;
	unsigned short EventNo;
	
	Ptr = Buff;	
	
	if( ComID == 0x04000802 ) EventNo = WeekCDPrg_No;
	else if( ComID == 0x04000601 ) EventNo = ECPModePrg_No;
	else if( ComID == 0x04000602 ) EventNo = ECQ1ModePrg_No;
	else if( ComID == 0x04000603 ) EventNo = ECQ2ModePrg_No;
	else return 1;	
	
//	IncEventCunt( EventNo );
	LastPt = PtAddrChk( EventNo, Next );				//定位新一次需量清零记录首址		
	if( LastPt == -1 ) return 0;
	IncEventCunt( EventNo );
	RAM_Write( Ptr, Clk.Sec, 6 );					
	RAM_Write( Ptr+6, Comm.OperateCode, 4 );					
	E2P_RData( Ptr+10, Addr, 1 );
	
	E2P_WData( LastPt, Ptr, 11 );

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
	if( EventNo == WeekCDPrg_No ) UpdateEventUpState( 87 );			//新国网	//14.04.26
	if( EventNo == ECPModePrg_No ) UpdateEventUpState( 89 );		//新国网	//14.04.26
	if( EventNo == ECQ1ModePrg_No ) UpdateEventUpState( 90 );		//新国网	//14.04.26
	if( EventNo == ECQ2ModePrg_No ) UpdateEventUpState( 91 );		//新国网	//14.04.26
#endif

	return 0;
	
}	 


//Type: 0: 通过按键的普通编程
//		1: 通过安全认证的远程编程
//		2: 通过卡的编程
void CreatPrgRec( unsigned char* Source, unsigned short Info, unsigned char* OperateCode, short Type )
{
	unsigned char Buff[50];
	unsigned char* Ptr;
	long LastPt;
	short i;
	
	Ptr = Buff;	
	
	if(( Info != PARAM )&&( Info != OTHER )) return;
	
	if( Type == 2 ) 
	{
		Flag.PrgState &= ~(F_PrgRec+F_RemotePrgRec);
	}	
//	if((( Flag.PrgState & F_PrgRec ) == 0 )&&(( Flag.PrgState & F_RemotePrgRec ) == 0 )) 
	if(( Flag.PrgState & F_PrgRec ) == 0 )					//10.08.25
	{
		LastPt = PtAddrChk( PrgRec_No, Next );				
		if( LastPt == -1 ) return;
		IncEventCunt( PrgRec_No );							//编程键按下期间第一次编程
		RAM_Write( Ptr, Clk.Sec, 6 );					
		RAM_Write( Ptr+6, OperateCode, 4 );					
		RAM_DataFill( Ptr+14, 36, 0xff );
		RAM_Write( Ptr+10, Source, 4 );					
		E2P_WData( LastPt, Ptr, 50 );
		if( Type == 0 ) Flag.PrgState |= F_PrgRec+F_RemotePrgRec;
		if( Type == 1 ) Flag.PrgState |= F_PrgRec+F_RemotePrgRec;
		if( Type == 2 ) 
		{
//			Flag.PrgState |= F_PrgRec;
//			Flag.PrgState |= F_RemotePrgRec;
		}			
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		UpdateEventUpState( 80 );			//新国网	//14.04.26
#endif		
	}	
	else
	{
		LastPt = PtAddrChk( PrgRec_No, Current );			
		if( LastPt == -1 ) return;
		E2P_RData( Ptr, LastPt+10, 40 );
		for( i=0;i<9;i++ )
		{
			RAM_Write( Ptr+(9-i)*4, Ptr+(8-i)*4, 4 );
		}	
		RAM_Write( Ptr, Source, 4 );					
		E2P_WData( LastPt+10, Ptr, 40 );

	}	
}	

void CreatSegPrgRec( void )
{
	unsigned char Buff[200];
	unsigned char* Ptr;
	unsigned short Addr;
	long LastPt;
	unsigned short i,j;
	
	Ptr = Buff;	
	
	Flag.PrgState &= ~F_SegPrg;

	if(( Flag.PrgState & F_SegPrg ) == 0 ) 
	{
//		IncEventCunt( SegTabPrg_No );							//编程键按下期间第一次编程
		LastPt = PtAddrChk( SegTabPrg_No, Next );				
		if( LastPt == -1 ) return;
		IncEventCunt( SegTabPrg_No );							//编程键按下期间第一次编程
		RAM_Write( Ptr, Clk.Sec, 6 );					
		RAM_Write( Ptr+6, Comm.OperateCode, 4 );
		DataFlash_Write( LastPt, Buff, 10 );

//		Addr = SEGNUMBER*3*4;
		Addr = (SEGNUMBER*3+1)*4;
		for( j=0;j<4;j++ )
		{
			for( i=0;i<4;i++ )
			{
//				E2P_RData( Ptr+SEGNUMBER*3*i, Period1_Tab1+(SEGNUMBER*3+1)*i*j, SEGNUMBER*3 );
				E2P_RData( Ptr+SEGNUMBER*3*i, Period1_Tab1+Addr*j+(SEGNUMBER*3+1)*i, SEGNUMBER*3 );
			}	
			DataFlash_Write( LastPt+10+j*SEGNUMBER*3*4, Buff, SEGNUMBER*3*4 );
		}	
//		Flag.PrgState |= F_SegPrg;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		UpdateEventUpState( 85 );			//新国网	//14.04.26
#endif		
	}	
}	

void CreatZonePrgRec( void )
{
	unsigned char Buff[100];
	unsigned char* Ptr;
	long LastPt;
	short i;
	
	Ptr = Buff;	
	
	Flag.PrgState &= ~F_ZonePrg;
	
	if(( Flag.PrgState & F_ZonePrg ) == 0 ) 
	{
//		IncEventCunt( ZonePrg_No );							//编程键按下期间第一次编程
		LastPt = PtAddrChk( ZonePrg_No, Next );				
		if( LastPt == -1 ) return;
		IncEventCunt( ZonePrg_No );							//编程键按下期间第一次编程
		RAM_Write( Ptr, Clk.Sec, 6 );					
		RAM_Write( Ptr+6, Comm.OperateCode, 4 );
		for( i=0;i<2;i++ )
		{
			E2P_RData( Ptr+10+ZONENUMBER*3*i, ESeason1+(ZONENUMBER*3+1)*i, ZONENUMBER*3 );
		}	
		DataFlash_Write( LastPt, Buff, ZONENUMBER*3*2+10 );
//		Flag.PrgState |= F_ZonePrg;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		UpdateEventUpState( 86 );			//新国网	//14.04.26
#endif		
	}	
}	

void CreatHolidayPrgRec( void )
{
	unsigned char Buff[210];
	unsigned char* Ptr;
	long LastPt;
	short i,j;
	
	Ptr = Buff;	
	
	Flag.PrgState &= ~F_HolidayPrg;
	
	if(( Flag.PrgState & F_HolidayPrg ) == 0 ) 
	{
//		IncEventCunt( HolidayPrg_No );							//编程键按下期间第一次编程
		LastPt = PtAddrChk( HolidayPrg_No, Next );				
		if( LastPt == -1 ) return;
		IncEventCunt( HolidayPrg_No );							//编程键按下期间第一次编程
		RAM_Write( Ptr, Clk.Sec, 6 );					
		RAM_Write( Ptr+6, Comm.OperateCode, 4 );
		DataFlash_Write( LastPt, Buff, 10 );
		for( j=0;j<6;j++ )
		{ 
			WDTCTL = WDT_ARST_1000;
			E2PRead( Ptr, HoliDAY1+200*j, 200, DataEAds );
			for( i=0;i<39;i++ )
			{
				RAM_Write( Ptr+4+i*4, Ptr+5+i*5, 4 );					
			}
			DataFlash_Write( LastPt+10+160*j, Buff, 160 );
		}
		WDTCTL = WDT_ARST_1000;
		E2PRead( Ptr, HoliDAY1+240*5, 14*5, DataEAds );
		for( i=0;i<13;i++ )
		{
			RAM_Write( Ptr+4+i*4, Ptr+5+i*5, 4 );					
		}
		DataFlash_Write( LastPt+10+960, Buff, 56 );
//		Flag.PrgState |= F_HolidayPrg;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		UpdateEventUpState( 88 );			//新国网	//14.04.26
#endif		
	}	
}

void CreatSaveDayPrgRec( void )
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	long LastPt;
	
	Ptr = Buff;	

	Flag.PrgState &= ~F_SaveDayPrg;
	
	if(( Flag.PrgState & F_SaveDayPrg ) == 0 ) 
	{
//		IncEventCunt( SaveDayPrg_No );							//编程键按下期间第一次编程
		LastPt = PtAddrChk( SaveDayPrg_No, Next );				
		if( LastPt == -1 ) return;
		IncEventCunt( SaveDayPrg_No );							//编程键按下期间第一次编程
		RAM_Write( Ptr, Clk.Sec, 6 );					
		RAM_Write( Ptr+6, Comm.OperateCode, 4 );
		E2P_RData( Ptr+10, ESave_Day1, 2 );
		E2P_RData( Ptr+12, ESave_Day2, 2 );
		E2P_RData( Ptr+14, ESave_Day3, 2 );
		E2P_WData( LastPt, Buff, 16 );
//		Flag.PrgState |= F_SaveDayPrg;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		UpdateEventUpState( 92 );			//新国网	//14.04.26
#endif		
	}	
}	

unsigned char NProc_Write( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i,Length;
	unsigned char* WriteBufAds;
  	unsigned char Buff[20];
  	unsigned char* Point;
  	unsigned char Err_Byte=0;
  	unsigned long ComID;
	long LastPt;

  	Point = Buff;

	WriteBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_Head0Ptr);
	
	ComID = ParaComTab[ ItemAds ].ComID;
	
#if( PrgRecRestWithOperateCode == YesCheck )																			//12.10.05
		if( Data_Comp( Comm.OperateCode, ComBuf+NRs_OPCodePtr, 4 ) != 0 )															//12.10.05
		{																												//12.10.05	
			RAM_Write( Comm.OperateCode, ComBuf+NRs_OPCodePtr, 4 );														//12.10.05
			Flag.PrgState &= ~F_PrgRec;											//操作者代码变化，产生新一次编程记录	//12.10.05
		}	
#else
		RAM_Write( Comm.OperateCode, ComBuf+NRs_OPCodePtr, 4 );
#endif
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
	if( ComID == 0x04001104 ) RAM_Write( (unsigned char*)&Para.REventUpMode, WriteBufAds, 8 );		//7号补遗，设置主动上报模式字即时生效	//15.03.19
#endif	
	CreatPrgRec( ComBuf+NRs_ID0Ptr, Info & Comm_Type, Comm.OperateCode, 0 );//编程记录
	
	switch( Info & Comm_Type )
	{
		case PARAM:		
				if( ComID == 0x04030001 ) 									//公假日需特殊处理
				{
					CreatHolidayPrgRec();
					Length = *(ComBuf+NRs_ID0Ptr)-1;
					E2P_WData( Addr+Length*5, WriteBufAds, Len ); 
				}	
				else if(( ComID == 0x04040101 )||( ComID == 0x04040201 )) 	//循环显示项、按键显示项需特殊处理
				{
//					*(WriteBufAds+4) = 0;									//数据项内部序号
//					if( *(ComBuf + Rs_LenPtr) == 16 ) *(WriteBufAds+4) = 0;	//显示项长度为4字节，数据项内部序号=0
//					Length = *(ComBuf+NRs_ID0Ptr);							
//					Word_BCD( WriteBufAds+5, Length );						//客户显示代码	
					Length = *(ComBuf+NRs_ID0Ptr)-1;					
//					E2P_WData( Addr+Length*8, WriteBufAds, 7 ); 
					E2P_WData( Addr+Length*6, WriteBufAds, 5 ); 
				}	
				else
				{					
					if( CreatSinglePrgRec( ComID, Addr ) != 0 )				//编程前记录
					{
						if(( ComID == 0x04010000 )||( ComID == 0x04020000 )) CreatZonePrgRec();
						else if((( ComID >= 0x04010001 )&&( ComID <= 0x04010008 ))
							||(( ComID >= 0x04020001 )&&( ComID <= 0x04020008 ))) CreatSegPrgRec();
						else if(( ComID >= 0x04000B01 )&&( ComID <= 0x04000B03 )) CreatSaveDayPrgRec();	
					}	
					
					if(( *(ComBuf+NRs_ID2Ptr) == 0x01 )||(( *(ComBuf+NRs_ID2Ptr) == 0x02 )))		//时区、时段
					{
						for( i=(*(ComBuf+Rs_LenPtr) - 12)/3; i<14; i++ )
						{
							RAM_Write( ComBuf+NRs_WDataPtr+i*3, ComBuf+NRs_WDataPtr+(i-1)*3, 3 );	
						}
					}			

					if( ComID == 0x04000703 ) 
					{
						Comm.BandRate1 = *(ComBuf+NRs_WDataPtr); 
						if(( Comm.Buf1[0] == 0xff )&&( Comm.Mode1 == 1 )&&( Comm.Ptr1 == 0 )) CommInit( 1 );		//V4				
					}	
					else if( ComID == 0x04000704 )
					{
						Comm.BandRate2 = *(ComBuf+NRs_WDataPtr); 
						if(( Comm.Buf2[0] == 0xff )&&( Comm.Mode2 == 1 )&&( Comm.Ptr2 == 0 )) CommInit( 2 );		//V4								
					}	
					else if( ComID == 0x04000705 )
					{
						Comm.BandRate3 = *(ComBuf+NRs_WDataPtr); 
						if(( Comm.Buf3[0] == 0xff )&&( Comm.Mode3 == 1 )&&( Comm.Ptr3 == 0 )) CommInit( 3 );		//V4								
					}	
#if ( MEASCHIP == ADE7758 )	
#elif ( MEASCHIP == ADE7878 )	
#elif ( MEASCHIP == IDT90E32 )							//12.08.29	
#else
   					if( ComID == 0x04090101 ) ReCalFailVoltageChk( WriteBufAds );				//ATChk
#endif
					
					E2P_WData( Addr, WriteBufAds, Len ); 					

					if( ComID == 0x04001104 )					//主动上报模式字	//16.05.17
					{
						E2P_WFM( FEventUpMode, WriteBufAds, 8 ); 					
					}		

#if ( MEASCHIP == ADE7758 )		
#elif ( MEASCHIP == ADE7878 )	
#elif ( MEASCHIP == IDT90E32 )							//12.08.29	
#else
   					if( ComID == 0x04090101 ) { SM.AT7022ChkSumCnt = 2; SetFailVoltage(); }		//ATChk
#endif
				}
    
       			*(ComBuf + Rs_LenPtr) -= Len;
				break;
		case OTHER:
	    		if( ComID == 0x04000101 )
	    		{
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
#if ( ModifyTimeSkipDelay == YesCheck )						//新国网	//14.06.19
					SaveTimeBeforeModification();						//新国网	//14.05.28
#endif
#endif
					IncEventCunt( TimeSet_No );
					LastPt = PtAddrChk( TimeSet_No, Next );				//定位新一次需量清零记录首址		
					RAM_Write( Point, Comm.OperateCode, 4 );			//操作者代码	
					RAM_Write( Point+4, Clk.Sec, 6 );					
					
					*WriteBufAds = WeekDay20( WriteBufAds+1 );

					if( Data_Comp( WriteBufAds+1, Clk.Day, 3 ) != 0 ) SM.BroadC_Mins = 0;	//新国网
//					*Point = Clk.Week[0];
//	    			RAM_Write( Point+1, Clk.Day, 3 );
					Clk.Week[0] = *WriteBufAds;
					Clk.Day[0] = *(WriteBufAds+1);
					Clk.Mon[0] = *(WriteBufAds+2);
					Clk.Year[0] = *(WriteBufAds+3);
//#if( RTCCHIP == RX8025 )
//					E2Pwrite( 0x30, WriteBufAds, 4, RTCAds );	//写时钟芯片日期
//#elif( RTCCHIP == RX8025T )																	//V1000				
					if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
					{
						*WriteBufAds = (unsigned char)ByteBit[*WriteBufAds];						//V1000
						E2Pwrite( 0x03, WriteBufAds, 4, RTCAds );	//写时钟芯片日期	//8025T
					}	
//#else
					else
					{
						if( Clk.Week[0] == 0 ) Clk.Week[0] = 0x07;
						E2Pwrite( 0x03, WriteBufAds, 4, DS3231Ads );	//写时钟芯片日期
					}	
//#endif
					if( Clk.Week[0] == 7 ) Clk.Week[0] = 0x0;
					RAM_Write( Point+10, Clk.Sec, 6 );					//操作者代码
					if( LastPt != -1 ) E2P_WData( (unsigned short)LastPt, Point, 16 );
//   					SM.BroadC_Mins = 0;									//新国网
					MD_InitialCalPara( Para.RMaxT );		
					SM.TimeSetLoadAdjDelay = 3;							//cru		//4秒后根据当前时间进行负荷曲线调整
					E2P_WData( LastSaveTime, Clk.Hour, 4 );				//设置时间跨过结算日，不结算	//12.09.06	//新国网
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
					UpdateEventUpState( 84 );			//新国网	//14.04.26
#endif		
				}
				else if( ComID == 0x04000102 )
				{
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
#if ( ModifyTimeSkipDelay == YesCheck )						//新国网	//14.06.19
					SaveTimeBeforeModification();						//新国网	//14.05.28
#endif
#endif
					IncEventCunt( TimeSet_No );
					LastPt = PtAddrChk( TimeSet_No, Next );				//定位新一次需量清零记录首址		
					RAM_Write( Point, Comm.OperateCode, 4 );			//操作者代码
					RAM_Write( Point+4, Clk.Sec, 6 );					

//					SM.BroadC_Mins = 0;									//新国网		
					Clk.Sec_64 = 32;
					Clk.Sec[0] = *WriteBufAds;
					Clk.Min[0] = *(WriteBufAds+1);
					Clk.Hour[0] = *(WriteBufAds+2);				
//#if(( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))												//V1000
					if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
					{
						E2Pwrite( 0x0, Clk.Sec, 3, RTCAds );	//写时钟芯片秒分时
					}	
//#else
					else
					{
						E2Pwrite( 0x0, Clk.Sec, 3, DS3231Ads );	//写时钟芯片秒分时
					}	
//#endif
					RAM_Write( Point+10, Clk.Sec, 6 );					//操作者代码
					if( LastPt != -1 ) E2P_WData( (unsigned short)LastPt, Point, 16 );
					MD_InitialCalPara( Para.RMaxT );	
					SM.TimeSetLoadAdjDelay = 3;							//cru		//4秒后根据当前时间进行负荷曲线调整
					E2P_WData( LastSaveTime, Clk.Hour, 4 );				//设置时间跨过结算日，不结算	//12.09.06	//新国网
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
					UpdateEventUpState( 84 );			//新国网	//14.04.26
#endif		
				}
	    		else if( ComID == 0x0400010C )								//新国网
	    		{
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
#if ( ModifyTimeSkipDelay == YesCheck )						//新国网	//14.06.19
					SaveTimeBeforeModification();						//新国网	//14.05.28
#endif
#endif
					IncEventCunt( TimeSet_No );
					LastPt = PtAddrChk( TimeSet_No, Next );				//定位新一次需量清零记录首址		
					RAM_Write( Point, Comm.OperateCode, 4 );			//操作者代码	
					RAM_Write( Point+4, Clk.Sec, 6 );					
					
					*(WriteBufAds+3) = WeekDay20( WriteBufAds+4 );

					if( Data_Comp( WriteBufAds+4, Clk.Day, 3 ) != 0 ) SM.BroadC_Mins = 0;	//新国网
					Clk.Sec_64 = 32;
					Clk.Sec[0] = *WriteBufAds;
					Clk.Min[0] = *(WriteBufAds+1);
					Clk.Hour[0] = *(WriteBufAds+2);				
					Clk.Week[0] = *(WriteBufAds+3);
					Clk.Day[0] = *(WriteBufAds+4);
					Clk.Mon[0] = *(WriteBufAds+5);
					Clk.Year[0] = *(WriteBufAds+6);

//#if( RTCCHIP == RX8025 )
//					E2Pwrite( 0x0, WriteBufAds, 7, RTCAds );	//写时钟芯片秒分时
//#elif( RTCCHIP == RX8025T )												//V1000
					if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
					{
//						*(WriteBufAds+3) = (unsigned char)ByteBit[*WriteBufAds+3];						//V1000
						*(WriteBufAds+3) = (unsigned char)ByteBit[*(WriteBufAds+3)];					//新国网	//13.09.28
						E2Pwrite( 0x0, WriteBufAds, 7, RTCAds );	//写时钟芯片秒分时
					}	
//#else
					else
					{
						if( *(WriteBufAds+3) == 0 ) *(WriteBufAds+3) = 0x07;
						E2Pwrite( 0x0, WriteBufAds, 7, DS3231Ads );	//写时钟芯片秒分时
					}	
//#endif
					RAM_Write( Point+10, Clk.Sec, 6 );					//操作者代码
					if( LastPt != -1 ) E2P_WData( (unsigned short)LastPt, Point, 16 );
					MD_InitialCalPara( Para.RMaxT );		
					SM.TimeSetLoadAdjDelay = 3;							//cru		//4秒后根据当前时间进行负荷曲线调整
					E2P_WData( LastSaveTime, Clk.Hour, 4 );				//设置时间跨过结算日，不结算	//12.09.06	//新国网
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
					UpdateEventUpState( 84 );			//新国网	//14.04.26
#endif		
				}
				else if( ComID == 0x04001301 )
				{
					Para.NetIntensity = *WriteBufAds;		//无线信号强弱
				}		
			break;	
		default:break;	
	}
	return	Err_Byte;	
}
*/		//16.09.24

/*****************************************************************
	The state of communication access
*****************************************************************/
void CommMode( short COM_No )
{
  	unsigned char Temp;
  	unsigned char* ComBuf;
  	unsigned char* ComMode;
  	unsigned short* ComPtr;
  	unsigned short* ComBTime;
  	unsigned char* ComSendDTime;
  	unsigned char* ComRecTmr;
  	unsigned char* ComFESum;
	unsigned char* IECState;			//17.05.05
  	
  	ComBuf = CommPara[COM_No].Buf;
  	ComMode = CommPara[COM_No].Mode;
  	ComPtr = CommPara[COM_No].Ptr;
  	ComBTime = CommPara[COM_No].BTime;
    ComSendDTime = CommPara[COM_No].SendDTime;
    ComRecTmr = CommPara[COM_No].RecTmr;
    ComFESum = CommPara[COM_No].FESum;

  	IECState = CommPara[COM_No].IECState;		//17.05.05
    
	switch( *ComMode )
	{
		case 0:	CommInit( COM_No );	break;
		case 1:
			if( *ComPtr != 0 )
			{
				if(((Comm.Run & FComOK0)&&(COM_No==0))||((Comm.Run & FComOK1)&&(COM_No==1))
//					||((Comm.Run & FComOK2)&&(COM_No==2))||((Comm.Run & FComOK3)&&(COM_No==3)))				
					||((Comm.Run & FComOK2)&&(COM_No==2)))				
				{
					Temp = ChkNum( ComBuf,*(ComBuf+Rs_LenPtr)+10 ) - 0xA5;
					if( *(ComBuf+*(ComBuf+Rs_LenPtr)+10) == Temp )
					{
						for( Temp=0;Temp<*(ComBuf+Rs_LenPtr);Temp++ ) *(ComBuf+10+Temp) -= 0x33;
						Comm_Ctrl(ComBuf);
						if(*ComBuf == 0x68)
						{
							if((Comm.BatOnDly != 0)&&(COM_No==0)) 
							{
//								Comm.BatOnDly = BATONDLY-1;		//停抄时间延长
								Comm.BatOnDly = Disk.HWBatOnTime-1;		//停抄时间延长	//10.10.06
//								SM.KeyDly = Para.RDspInv * Disk.LXItemSum;					
							}	
//							E2P_RData( ComBuf+Rs_Addr0Ptr, EMeter_No, 6 );	
    						E2P_RData( ComBuf+Rs_Addr0Ptr, EMeter_Addr, 6 );	     	//17.03.16			    				
							for(Temp=0; Temp<*(ComBuf+Rs_LenPtr); Temp++) *(ComBuf+10+Temp) += 0x33;
							*(ComBuf+*(ComBuf+Rs_LenPtr)+10) = ChkNum( ComBuf,*(ComBuf+Rs_LenPtr)+10 ) - 0xA5;
							*(ComBuf+*(ComBuf+Rs_LenPtr)+10+1) = 0x16;
							*ComMode = 2;
//							*ComSendDTime = 2;		//V1000
//							if( COM_No == 0 )		//V3
							if(( COM_No == 0 )||(*(ComBuf+Rs_CtlPtr) == 0x83 ))		//新国网
							{	
								*ComSendDTime = 0;		//V1000		//V3
								CommMode( COM_No );		//V1001		//V3
							}
							else 
							{
								*ComSendDTime = 1;	//V3						
							}	
							*ComBTime = MS500;		//V3
						} 
						else
						{
							Temp = *(ComBuf+Rs_LenPtr) + 12;								//17.05.05
							if( *ComPtr > Temp )											//17.05.05
							{																//17.05.05
								RAM_Write( ComBuf, ComBuf+Temp, (*ComPtr)-Temp );			//17.05.05
								*ComPtr -= Temp;											//17.05.05	
								if( *ComPtr != 0 )                                      	//17.05.05
								{								                        	//17.05.05
									if( ComBuf == Comm.Buf0 ) Comm.Run &= ~FComOK0;     	//17.05.05
									else if( ComBuf == Comm.Buf1 ) Comm.Run &= ~FComOK1;	//17.05.05
									else if( ComBuf == Comm.Buf2 ) Comm.Run &= ~FComOK2;	//17.05.05
									*IECState = IECBufBusy;									//17.05.05
									SetUart_IE( ComBuf, ENABLE );                       	//17.05.05
								}                                                       	//17.05.05
								else                                                    	//17.05.05
								{                                                       	//17.05.05
									*ComMode = 0;						                	//17.05.05
									CommInit( COM_No );                                 	//17.05.05
								}		                                                	//17.05.05
							}																//17.05.05
							else															//17.05.05
							{																//17.05.05	
                            	*ComMode = 0;                                           	
                            	CommInit( COM_No );											//17.05.05
                            }																//17.05.05		
						}	
					}
					else
					{
						*ComMode = 0;	
                       	CommInit( COM_No );													//17.05.05
					}	
				}
				else if( *ComBTime == 0 ) 
					  {
							*ComMode = 0;	
	                       	CommInit( COM_No );												//17.05.05
					  }					
			}
			break;
		case 2:
			if( *ComSendDTime == 0 )
			{
//				Comm.RecTmr = 2;
				*ComFESum = 3;
				*ComRecTmr = 2;
				*ComMode = 3;	
				*ComPtr = 0;
				switch( COM_No )
				{
					case 0: 
	                      	Comm.Run &= ~FComOK0; 
//        					UCA0IE |= UCTXIE;     		
//                     		UCA0TXBUF = 0xFE;     		
//							Comm.RxTx_Data = 0xFE;
//							HwTxInitial();
							HT_UART2->UARTSTA &= ~UART_UARTSTA_TXIF;//UCA1IFG &= ~UCTXIFG;				//13.05.17
        					HT_UART2->UARTCON |= UART_UARTCON_TXIE + UART_UARTCON_TXEN;//UCA1IE |= UCTXIE;  		
                      		HT_UART2->SBUF = 0xFE;//UCA1TXBUF = 0xFE;
							break;
					case 1: 
//	                      	PDIR_SW4851 |= P_SW4851;	
//	                      	HT_GPIOD->PTDIR |= GPIOD_485_CTL1;//PDIR_SW4851 |= P_SW4851;	
	                      	HT_GPIOD->PTCLR |= GPIOD_485_CTL1;//PDIR_SW4851 |= P_SW4851;	
							Comm.Run &= ~FComOK1; 
//							UCA1IFG &= ~UCTXIFG;				//13.05.17
//        					UCA1IE |= UCTXIE;     		
//                     		UCA1TXBUF = 0xFE;     		
							HT_UART1->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);//UCA1IFG &= ~UCTXIFG;				//13.05.17
//        					HT_UART1->UARTCON |= UART_UARTCON_TXIE;//UCA1IE |= UCTXIE;     		
        					HT_UART1->UARTCON |= UART_UARTCON_TXIE + UART_UARTCON_TXEN;//UCA1IE |= UCTXIE;     		
                      		HT_UART1->SBUF = 0xFE;//UCA1TXBUF = 0xFE;     		
							break;
#if ( CarryComm == YesCheck )			//带载波通道与485口2共用通讯缓存
					case 2: 
//	                      	HT_GPIOC->PTCLR |= GPIOC_485_CTL2;//PDIR_SW4852 |= P_SW4852;	
							Comm.Run &= ~FComOK2; 
							HT_GPIOA->PTDIR &= ~GPIOA_PLC_SET;//PDIR_SWZB |= P_SWZB;	
							HT_UART0->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);//UCA2IFG &= ~UCTXIFG;				//13.05.17
        					HT_UART0->UARTCON |= UART_UARTCON_TXIE + UART_UARTCON_TXEN;//UCA2IE |= UCTXIE;     		
                      		HT_UART0->SBUF = 0xFE;//UCA2TXBUF = 0xFE;     		
							break;
#else
					case 2: 
//	                      	PDIR_SW4852 |= P_SW4852;	
//	                      	HT_GPIOC->PTDIR |= GPIOC_485_CTL2;//PDIR_SW4852 |= P_SW4852;	
	                      	HT_GPIOC->PTCLR |= GPIOC_485_CTL2;//PDIR_SW4852 |= P_SW4852;	
							Comm.Run &= ~FComOK2; 
//							UCA2IFG &= ~UCTXIFG;				//13.05.17
//        					UCA2IE |= UCTXIE;     		
//                     		UCA2TXBUF = 0xFE;     		
							HT_UART5->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);//UCA2IFG &= ~UCTXIFG;				//13.05.17
//        					HT_UART5->UARTCON |= UART_UARTCON_TXIE;//UCA2IE |= UCTXIE;     		
        					HT_UART5->UARTCON |= UART_UARTCON_TXIE + UART_UARTCON_TXEN;//UCA2IE |= UCTXIE;     		
                      		HT_UART5->SBUF = 0xFE;//UCA2TXBUF = 0xFE;     		
							break;
#endif
					case 3: 
//#if ( CarryComm == YesCheck )
//#else
////	                      	PDIR_SWZB |= P_SWZB;	
//	                      	HT_GPIOA->PTDIR &= ~GPIOA_PLC_SET;//PDIR_SWZB |= P_SWZB;	
//#endif
//							Comm.Run &= ~FComOK3; 
////							UCA0IFG &= ~UCTXIFG;				//13.05.17
////       					UCA0IE |= UCTXIE;     		
////                     		UCA0TXBUF = 0xFE;     		
//							HT_UART0->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);//UCA0IFG &= ~UCTXIFG;				//13.05.17
//        					HT_UART0->UARTCON |= UART_UARTCON_TXIE;//UCA0IE |= UCTXIE;     		
//                     		HT_UART0->SBUF = 0xFE;//UCA0TXBUF = 0xFE;     		
							break;
					default:break;				
				}
			}	
			if(( *(ComBuf+Rs_CtlPtr) == 0x84 )||(*(ComBuf+Rs_CtlPtr) == 0x94)||(*(ComBuf+Rs_CtlPtr) == 0x8E)) InitPara();
			if( *(ComBuf+Rs_CtlPtr) == 0x9A )			//电表清零
			{
   				if( SM.MeterClrState == MeterClrStep1 )
   				{
   					SM.MeterClrStateChk = MeterClrStep1 + 0xA5A5;						
   				}	
			}			
			if( *(ComBuf+Rs_CtlPtr) == 0x99 )			//需量清零
			{
   				if( SM.MDClrState == MDClrStep1 )
   				{
   					SM.MDClrStateChk = MDClrStep1 + 0xA5A5;						
   				}	
			}			
			break;
		case 3:
			if(((Comm.Run & FComOK0)&&(COM_No==0))||((Comm.Run & FComOK1)&&(COM_No==1))
//				||((Comm.Run & FComOK2)&&(COM_No==2))||((Comm.Run & FComOK3)&&(COM_No==3)))				
				||((Comm.Run & FComOK2)&&(COM_No==2)))				
			{
				*ComMode = 0;
				CommInit( COM_No );					
			}
			break;
		default:
			*ComMode = 0;
			break;
	}
}

//short GetComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComBuf )
short GetComVar( unsigned long* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComBuf )
{
	short i;
    unsigned short Temp;
	
  	Temp = ((unsigned short)*(ComBuf+Rs_IDHPtr)<<8)+ (unsigned short)*(ComBuf+Rs_IDLPtr);
  	for( i=0;i<ComNum;i++ )
  	{
		if( Temp == ComTab[i].ComID ) 
		{
			*Addr = ComTab[i].Addr;
			*Info = ComTab[i].ComInfo;
			*Len = *Info & RS_LEN;
			return i;
		}
  	}
	return -1;
}

short GetSegNum( unsigned char* Source, short Number )
{
	unsigned char* Point;
	short i;
	
	Point = Source;
	for( i=1;i<Number;i++ )
	{
		if( Data_Comp( Point, Point+3, 3 ) != 0 )
		{
			Point = Point + 3;	
		}
		else break;
	}		 	
	return i;	
}	

unsigned char Com_Read(unsigned char* ComBuf)
{
//	unsigned short 	Addr, Info, Len;
	unsigned short 	Info, Len;
	unsigned long 	Addr;
  	unsigned char Err_Byte=0;
	short ItemAds;
//	short i;

	Addr = (short)*(ComBuf+Rs_IDHPtr)*256+ (short)*(ComBuf+Rs_IDLPtr);
	
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
	if(( Addr >= 0xF600 )&&( Addr <= 0xF800 ))			//
	{
		Addr -= 0xF600;
#if ( IDT90E32AS ==	YesCheck )	//12.09.05	
		if( Addr == 0x30 )
		{
//			*(ComBuf + Rs_DataPtr) = AmperMult;					//电流放大倍数		//新国网		//13.08.30
			*(ComBuf + Rs_DataPtr) = MSpec.R90E32_AmperMult;	//电流放大倍数		//新国网		//13.08.30
			*(ComBuf+Rs_LenPtr) = 3;
			return Err_Byte;			
		}	
#endif
//		if(( PIN_SETEN & P_SETEN ) != 0 )
		if(( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )
		{
			if((( Addr >= 0x80 )&&( Addr <= 0x94 ))||(( Addr >= 0xA0 )&&( Addr <= 0xAF ))) 		//读后清零寄存器正常时不允许读 
			{
              Err_Byte |= RS_State_IVData;
			    return	Err_Byte;
            }    
		}	

		IDT90E32RdReg( (unsigned short)Addr, ComBuf + Rs_DataPtr );	
		*(ComBuf+Rs_LenPtr) = 4;
		return Err_Byte;
	}		
#elif ( MEASCHIP == ATT7022E )							//13.07.15	
	if( GetATT7022ECalibrateReg( ComBuf ) == 0 ) return 0;						//读ATT7022E校表寄存器数据
#endif

		ItemAds = GetComVar( &Addr, &Info, &Len, ComBuf );
		if( ItemAds == -1 ) 
		{
			Err_Byte |= RS_State_IVID;
			return	Err_Byte;
		}
		if( ~Info & EN_R ) 
		{
			Err_Byte |= RS_State_IVData;
			return	Err_Byte;
		}
		*(ComBuf+Rs_LenPtr) = 2;
		Proc_Read( ItemAds, Addr, Info, Len, ComBuf );
//	}
	return Err_Byte;
}

//void Proc_Read( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
void Proc_Read( short ItemAds, unsigned long Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i,Length,ComID;
	unsigned char* ReadBufAds;
	unsigned char* RamAds;
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
	ComID = ComTab[ ItemAds ].ComID;
	
	switch( Info & Comm_Type )
	{
		case 0x0000:
			Read_CMonEC( ReadBufAds, (unsigned short)Addr );
			*(ComBuf + Rs_LenPtr) += Len;
			break;		
		case PARAM:
#if ( MEASCHIP == ADE7758 )		
			if((( ComID >= 0xC098 )&&( ComID <= 0xC09B))
				||( ComID == 0xC09D )||( ComID == 0xC09E))
			{
				E2P_RAdj( ReadBufAds,Addr, Len );	
				Swap7758Para( ReadBufAds, ComID );		//交换数据相高低字节
			}	
			else if( ComID == 0xC09C )
			{
				for( i=0;i<3;i++)
				{
					ADE7758RdReg( AVRMS+i, ReadBufAds+i*3 );
					ADE7758RdReg( AIRMS+i, ReadBufAds+9+i*3 );
				}					
				Swap7758Para( ReadBufAds, ComID );		//交换数据相高低字节
			}	 
			else
			{
		   			E2P_RData( ReadBufAds, (unsigned short)Addr, Len);
//		   		}
		   	}	
#else

				if(( ComID == 0xB634 )||( ComID == 0xB635 ))
				{
					E2P_RData( ReadBufAds, (unsigned short)Addr, 3 );
					RAM_Write( ReadBufAds, ReadBufAds+1, 2 );
				}	
				else E2P_RData( ReadBufAds, (unsigned short)Addr, Len);
#endif		   	
		   	*(ComBuf + Rs_LenPtr) += Len;
			break;
		case OTHER:	
			RamAds = (unsigned char*)Addr;
			RAM_Write( ReadBufAds, RamAds, Len );
			if(( ComID <= 0xB643 )&&( ComID >= 0xB640 ))		//无功功率
			{
				RAM_Write( ReadBufAds, RamAds, 3 );
				*ReadBufAds = *(ReadBufAds+1);
				*(ReadBufAds+1) = *(ReadBufAds+2);				 	
				*(ReadBufAds+1) &= 0x7F;				 	
			}	
			if(( ComID <= 0xB633 )&&( ComID >= 0xB630 ))		//有功功率
			{
				*(ReadBufAds+2) &= 0x7F;				 	
			}	
			if(( ComID <= 0xB653 )&&( ComID >= 0xB650 ))		//功率因素
			{
				*(ReadBufAds+1) &= 0x7F;				 	
			}	
			if(( ComID <= 0xB613 )&&( ComID >= 0xB611 ))		//电压
			{
#if ( MEASCHIP == IDT90E32 )							//12.09.08	
#if ( RemoteEnterCalib == YesCheck )								//新国网	//14.05.23
//				if(((( PIN_SETEN & P_SETEN ) == 0 )||( SM.RemoteCalibState == 0x55 ))&&(SM.CalibCount == CALIBCOUNT1)) 		//远程校表状态
				if(((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( SM.RemoteCalibState == 0x55 ))&&(SM.CalibCount == CALIBCOUNT1)) 		//远程校表状态
#else
//				if((( PIN_SETEN & P_SETEN ) == 0 )&&(SM.CalibCount == CALIBCOUNT1)) 
				if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )&&(SM.CalibCount == CALIBCOUNT1)) 		//远程校表状态
#endif
				{
					if( ComID == 0xB611 ) i = UrmsA;
					if( ComID == 0xB612 ) i = UrmsB;
					if( ComID == 0xB613 ) i = UrmsC;
					IDT90E32RdReg( i, ReadBufAds );
				}	
				else
				{
					Word_BCD( ReadBufAds, BCD2_Word(ReadBufAds)/10 );					
				}	
#else
#if ( RemoteEnterCalib == YesCheck )								//新国网	//14.05.23
//				if(((( PIN_SETEN & P_SETEN ) == 0 )||( SM.RemoteCalibState == 0x55 ))&&(SM.CalibCount == CALIBCOUNT1)) 
				if(((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( SM.RemoteCalibState == 0x55 ))&&(SM.CalibCount == CALIBCOUNT1)) 
				{
					__NOP();		//远程校表状态
				}
				else 	
				{
					Word_BCD( ReadBufAds, BCD2_Word(ReadBufAds)/10 );
				}
#else
//				if((( PIN_SETEN & P_SETEN ) != 0 )||(SM.CalibCount != CALIBCOUNT1)) 
				if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&(SM.CalibCount != CALIBCOUNT1)) 		//远程校表状态
				{
					Word_BCD( ReadBufAds, BCD2_Word(ReadBufAds)/10 );
				}
#endif
#endif			
			}
			if(( ComID <= 0xB623 )&&( ComID >= 0xB621 ))		//电流
			{
//				RAM_Write( ReadBufAds, RamAds, 3 );
				RAM_Write( ReadBufAds, RamAds, 4 );
//				*(ReadBufAds+3) = 0;
//				Long_BCD4( ReadBufAds, BCD4_Long(ReadBufAds)/10 );
#if ( MEASCHIP == IDT90E32 )							//12.09.08	
#if ( RemoteEnterCalib == YesCheck )								//新国网	//14.05.23
//				if(((( PIN_SETEN & P_SETEN ) == 0 )||( SM.RemoteCalibState == 0x55 ))&&(SM.CalibCount == CALIBCOUNT1)) 		//远程校表状态
				if(((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )||( SM.RemoteCalibState == 0x55 ))&&(SM.CalibCount == CALIBCOUNT1)) 
#else
//				if((( PIN_SETEN & P_SETEN ) == 0 )&&(SM.CalibCount == CALIBCOUNT1)) 
				if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 )&&(SM.CalibCount == CALIBCOUNT1)) 		//远程校表状态
#endif
				{
					Len = 4;
				}	
				else
				{
					Long_BCD4( ReadBufAds, BCD4_Long(ReadBufAds)/100 );
				}					
#else
				Long_BCD4( ReadBufAds, BCD4_Long(ReadBufAds)/100 );
#endif
			}
			if( ComID == 0xC010 )
	    	{
				*ReadBufAds = Clk.Week[0];
				*(ReadBufAds+1) = Clk.Day[0];
				*(ReadBufAds+2) = Clk.Mon[0];
				*(ReadBufAds+3) = Clk.Year[0];
			}		
			if( ComID == 0xFE01 )													//10.10.08		
			{																		//10.10.08		
				for( i=1;i<5;i++ )													//10.10.08
				{																	//10.10.08
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
					Length = i;
					if( Length == 4 ) Length = 0;	
					Long_BCD4( ReadBufAds+i*4, (unsigned long )GetPhasePW(PmeanT+Length) );		//10.10.08
#else
					Long_BCD4( ReadBufAds+i*4, (unsigned long )GetPhasePW(i) );		//10.10.08
#endif
				}																	//10.10.08	
				RAM_Write( ReadBufAds, ReadBufAds+16, 4 );							//10.10.08
#if( LinkMode == Phase3Wire4 )														//10.10.08
#else																				//10.10.08
				RAM_Fill( ReadBufAds+8, 4 );										//10.10.08
#endif																				//10.10.08
			}																		//10.10.08
			*(ComBuf + Rs_LenPtr) += Len;
			break;
		case DATABLK:
			Length = Len;
			for( i=1;i<Length+1;i++ )
			{
				Addr = ComTab[ ItemAds+i ].Addr;
				Info = ComTab[ItemAds+i].ComInfo;
				Len = Info & RS_LEN;
				Proc_Read( ItemAds+i, Addr, Info, Len, ComBuf );
			}
			break;
		default: break; 								
	}	
}	

/*			//16.09.24
long PtAddrChk( unsigned short PtNum, short Type )
{
	unsigned short CurAddr;
	unsigned short PtAddr;
	unsigned short StorageRecs;	
	unsigned short StartAddr;
	unsigned short EndAddr;
	unsigned short RecdCunt;
	unsigned short RecdLenth;	
	unsigned char MemType;
	unsigned short Page;
	unsigned short PageItems;
	short MaxPtr;
    unsigned char Recs;
	long Temp,Temp1,Temp2,Temp3;

	PtAddr = PointTab[PtNum].PtAddr;
	StorageRecs = PointTab[PtNum].StorageRecs;
	StartAddr = PointTab[PtNum].StartAddr;
	EndAddr = PointTab[PtNum].EndAddr;
	RecdCunt = PointTab[PtNum].RecdCunt;
	RecdLenth = PointTab[PtNum].RecdLenth;
	MemType = PointTab[PtNum].MemType;

	Recs = 0;
	E2P_RData( (unsigned char*)&Recs, StorageRecs, 1 );		//已存记录数
	if( Recs > RecdCunt ) Recs = 0;							//已存记录数超过允许记录数，非法，已存记录数清零。表示没有有效记录
	if( Type >= Recs ) return -1;							//所抄记录超限，返回空。	
	
	E2P_RData( (unsigned char*)&CurAddr, PtAddr, 2 );		//EEPROM为直接EEPROM地址、FLASH为事件当前记录号
	if( MemType == 0x55 )									//存储空间为FLASH，记录从地地址向高地址排列，且实际存储记录数大于记录逻辑限制数
	{														//    可减少FLASH擦除次数
//		StartAddr = 0;										//事件存储空间内起始逻辑地址	
//		Page = EndAddr/256;									//存储空间内总页数	
		Page = EndAddr;										//存储空间内总页数	
		if(( PtNum == LastEC_No )||( PtNum == LastMD_No ))  //历史电量、历史需量双备份 
		{
			Page = Page/2;									//单份空间占用FLASH页数
		}	
		PageItems = 4096 / RecdLenth;						//每一页内的记录数
		MaxPtr = PageItems * Page;							//存储空间内可以存储的总记录数
		if( Type == -1 )
		{
			CurAddr += 1;									//新分配一条记录
			if( CurAddr >= MaxPtr ) CurAddr = 0;			//超过总记录数，指向第一条。
			if( Recs == 0 ) CurAddr = 0;						//事件第一次产生记录，指向第一条。
															//可配合用于当目标记录指向页的逻辑零地址时，将当前页先擦除后再写入数据，与写FLASH程序配合。		
			E2P_WData( PtAddr, (unsigned char*)&CurAddr, 2 );//保存当前事件记录号	
			if( Recs < RecdCunt )							//已存记录数小于抄收限制记录数
			{ 												
				Recs++;										//已存记录数加1后存入EEPROM	
				E2P_WData( StorageRecs, (unsigned char*)&Recs, 1 );	
			}	
		}
		else
		{
			if( CurAddr >= Type ) CurAddr -= Type;			//抄收记录号小于当前记录号，直接从当前记录向前递推找到目标记录
			else CurAddr = CurAddr + MaxPtr - Type;			//否则转到存储区末端。
		}	
		Temp1 = (CurAddr % PageItems)*RecdLenth;			//目标记录首地址在其所处页内的逻辑地址
//		Temp = EndAddr % 256;								//事件存储区在FLASH芯片内的起始页号	
		Temp = StartAddr;									//事件存储区在FLASH芯片内的起始页号	
		Temp += CurAddr/PageItems;							//目标记录在FLASH芯片内的页号
		Temp = Temp * 4096 + Temp1;							//目标记录首地址在FLASH芯片内物理地址
	}	

	if( MemType == 0x00 )									//存储空间为EEPROM，记录从高地址向低地址排列。
	{
		Temp = CurAddr;
		Temp2 = StartAddr;
		Temp3 = EndAddr;
	
		if( (Temp < Temp2)||(Temp >= Temp3)) 
		{	
			Temp = Temp2; 
			E2P_WData( PtAddr, (unsigned char*)&Temp, 2 );							
		}	
		if( Type != 0 )
		{
//			Temp += Type * RecdLenth;
			Temp1 = Type * (short)RecdLenth;
			Temp += Temp1;
			if( Type > 0 ) 
			{
				if( Temp >= Temp3 )
                {  
//					Temp += Temp2 - Temp3;
					Temp += Temp2;
					Temp -= Temp3;
                }    
			}
			else 
			{
				if( Temp < Temp2 )	
                {   
//					Temp += Temp3 - Temp2;	
					Temp += Temp3;
					Temp -= Temp2;	
                }    
			}
			if( Type < 0 )
			{
				E2P_WData( PtAddr, (unsigned char*)&Temp, 2 );							
				if( Recs < RecdCunt )							//已存记录数小于抄收限制记录数
				{ 												
					Recs++;										//已存记录数加1后存入EEPROM	
					E2P_WData( StorageRecs, (unsigned char*)&Recs, 1 );	
				}					
			}	
		}	
	}
	
	return Temp;

}	
*/			//16.09.24
		
void Clr_E2PData( unsigned short Addr, unsigned short DataLenth, unsigned short Cylcle )
{
  	unsigned char Buff[130];
  	unsigned char* Point;
  	unsigned short i,Len,DestAddr;
  	unsigned short CylcleCnt;
  	
  	if( Addr >= (E2P_PGLEN * 2) )
  	{
  		DataLenth = DataLenth * Cylcle;
  		while( DataLenth > 0 )
  		{
	  		if( DataLenth >= 130 ) Len = 130;	
  			else Len = DataLenth;	
			RAM_Fill( Buff, Len );  			

//			WDTCTL = WDT_ARST_1000;
			HT_FreeDog();
  			E2Pwrite( Addr, Buff, Len, DataEAds );
  			Addr += Len;
  			DataLenth -= Len;
  		}		  		
  	}	
  	else
  	{  	  	
		CylcleCnt = 0;
		while( CylcleCnt < Cylcle )
		{ 
			Len = 0;
			for( i=CylcleCnt;i<Cylcle;i++ )
			{
				Len += DataLenth + 1;
				if( Len >= 130 ) break; 			
			}	
			if( i == Cylcle ) CylcleCnt = Cylcle;
			else 
			{
				Len = Len - (DataLenth + 1);		
				CylcleCnt = i;
			}	
    
  	  		Point = Buff;
			RAM_Fill( Point, Len );
			for( i=0;i<Len; )
			{
				Point += DataLenth;
				*Point = 0xA5;
				Point += 1;
				i += DataLenth + 1;	
				if( Point >= ( Buff+130 )) break;
			}	
    
			DestAddr = Addr;
			for( i=0;i<2;i++ )
			{		
//				WDTCTL = WDT_ARST_1000;
				HT_FreeDog();
//				_E2Pwrite( DestAddr, Buff, Len, DataEAds );
				E2Pwrite( DestAddr, Buff, Len, DataEAds );
				DestAddr += E2P_PGLEN;		
			}	
//			WDTCTL = WDT_ARST_1000;
			HT_FreeDog();
			Addr += Len;
		}
	}
}	

void Clr_FRAMData( unsigned short Addr, unsigned short DataLenth, unsigned short Cylcle )
{
  	unsigned char Buff[130];
  	unsigned char* Point;
  	unsigned short i,Len,DestAddr;
  	unsigned short CylcleCnt;
  	
	CylcleCnt = 0;
	while( CylcleCnt < Cylcle )
	{ 
		Len = 0;
		for( i=CylcleCnt;i<Cylcle;i++ )
		{
			Len += DataLenth + 1;
			if( Len >= 130 ) break; 			
		}	
		if( i == Cylcle ) CylcleCnt = Cylcle;
		else 
		{
			Len = Len - (DataLenth + 1);		
			CylcleCnt = i;
		}	

  		Point = Buff;
		RAM_Fill( Point, Len );
		for( i=0;i<Len; )
		{
			Point += DataLenth;
			*Point = 0xA5;
			Point += 1;
			i += DataLenth + 1;	
			if( Point >= ( Buff+130 )) break;
		}	

		DestAddr = Addr;
		for( i=0;i<2;i++ )
		{		
//			WDTCTL = WDT_ARST_1000;
			HT_FreeDog();
//			_E2Pwrite( DestAddr, Buff, Len, DataEAds );
			E2Pwrite( DestAddr, Buff, Len, FMAds );
//			DestAddr += 0x400;		
			DestAddr += FRAM_PGLEN;		
		}	
//		WDTCTL = WDT_ARST_1000;
		HT_FreeDog();
		Addr += Len;
	}
}	

	
#if ( MEASCHIP == ADE7758 )
#elif ( MEASCHIP == IDT90E32 )							//12.08.29	
#elif ( MEASCHIP == ADE7878 )	
unsigned char ComAdjWrite(unsigned char* ComBuf )
{
	unsigned short Reg,EAds;	
	short i;
	unsigned short Len;

//	Temp = *(ComBuf+Rs_IDLPtr);
	if((( PIN_SETEN & P_SETEN ) != 0 )||(SM.CalibCount != CALIBCOUNT1)) 
	{
		*ComBuf = 0xFF;
		return RS_State_IVData;		
	}	
	RAM_Write( (unsigned char*)&Reg, ComBuf+NRs_WDataPtr, 2 );					
	for( i=0;i<Par_7878cnt;i++ )
	{
		if( Addr7878[i].Ad7878_Addr != Reg ) continue;
		EAds = Addr7878[i].Eeprom_Addr;
		Len = Addr7878[i].DataLen;
		E2P_WAdj( EAds, ComBuf+NRs_WDataPtr+2, Len );		
       	E2P_RAdj( ComBuf+NRs_WDataPtr, EAds, Len );		
		WriteADEMeasureChip( Reg, Len, ComBuf+NRs_WDataPtr );		
//#if( AllFailChk == YesCheck )
#if(( AllFailChk == YesCheck )&&( AllFail7022Chk == NoCheck ))				//10.11.11
		if( Reg == CIGAIN ) 
		{	
			Flag.Power |= F_IrmsCheck;
			ADCStart( CURRENTAD );
		}
#else
#endif
		return 0;
	}	
	return 1;
}	

unsigned char ComAdjRead(unsigned char* ComBuf )
{
	unsigned short Reg,EAds;	
	short i;
	unsigned short Len;

//	Temp = *(ComBuf+Rs_IDLPtr);
	RAM_Write( (unsigned char*)&Reg, ComBuf+NRs_RDataPtr, 2 );					
	for( i=0;i<Par_7878cnt;i++ )
	{
		if( Addr7878[i].Ad7878_Addr != Reg ) continue;
		EAds = Addr7878[i].Eeprom_Addr;
		Len = Addr7878[i].DataLen;
       	E2P_RAdj( ComBuf+NRs_RDataPtr, EAds, Len );		
		return 0;
	}	
	return 1;
}	

#elif ( MEASCHIP == ATT7022E )									//11.09.24
#else
unsigned char ComAdjWrite(unsigned char* ComBuf )
{
	short Temp,Temp2;		
	unsigned char Buff[4]; 
	unsigned char* Point;
	unsigned short Temp3;				//10.07.30
	unsigned long Value;			//10.11.02
	short i;							//10.11.02

    Point = Buff;
	Temp = *(ComBuf+Rs_IDLPtr);
//	if((( PIN_SETEN & P_SETEN ) != 0 )||((SM.CalibCount != CALIBCOUNT1)&&(Temp != HFDouble))  ) 
	if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )||((SM.CalibCount != CALIBCOUNT1)&&(Temp != HFDouble))) 		//远程校表状态
	{
		*ComBuf = 0xFF;
		return RS_State_IVData;		
	}						
	if((( Temp >= Iregion1 )&&(Temp <= UgainC))||(( Temp >= IgainA )&&(Temp <= IgainC))||(Temp == GainADC7)||(Temp == HFDouble)) 
	{	
		RAM_Write( Point, ComBuf+Rs_WDataPtr, 3 );		//10.07.30

		Temp2 = Temp * 4;								

       	Temp3 = E2P_PGLEN - 0x100;						//10.07.30
       	Temp3 += Temp2;									//10.07.30		
       	E2P_WData( Temp3, ComBuf+Rs_WDataPtr, 3 );		//10.07.30
       	E2P_RData( ComBuf+Rs_WDataPtr, Temp3, 3 );		//10.07.30
		if( Data_Comp( Point, ComBuf+Rs_WDataPtr, 3 ) != 0 ) return RS_State_IVData;		//10.07.30

/*		for( i=0;i<2;i++ )																//10.11.02					
		{																				//10.11.02						
			Value = (unsigned long)(AT7022DataZonePage+i)*4096;							//13.09.16		//新国网	//13.09.16
			Read_Flash( FBuff1.Buff1, Value, 256+80 );									//13.09.16
			*(ComBuf+Rs_WDataPtr+3) = ChkNum( ComBuf+Rs_WDataPtr, 3 );					//13.09.16
			if( Data_Comp( ComBuf+Rs_WDataPtr, FBuff1.Buff1+Temp2, 4 ) == 0 ) continue;	//13.09.16	
			RAM_Write( FBuff1.Buff1+Temp2, ComBuf+Rs_WDataPtr, 4 );						//13.09.16
			DataFlash_Write( Value, FBuff1.Buff1, 256+80 );								//13.09.16	
			RAM_Fill( FBuff2.Buff1, 256+80 );											//13.09.16		
			Read_Flash( FBuff2.Buff1, Value, 256+80 );									//13.09.16
			if(( Data_Comp( FBuff1.Buff1, FBuff2.Buff1, 256+80 ) != 0 )					//13.09.16
				||( Data_Comp( Point, FBuff1.Buff1+Temp2, 3 ) != 0 )) 					//13.09.16		//新国网	//13.09.16
			{																			//10.11.02					
				return RS_State_IVData;													//10.11.02
			}																			//10.11.02							
		}																				//10.11.02						
*/
		*(Point+2) = 0x00;					
		*(Point+1) = 0x00;
		*Point = 0x00;
		ATT7022WtReg( 0xC9, Point );
		ATT7022WtReg( Temp+128, ComBuf+Rs_WDataPtr );	
		*(Point+2) = 0x00;					
		*(Point+1) = 0x00;
		*Point = 0x01;						//关闭写校表数据功能
		ATT7022WtReg( 0xC9, Point );
//#if( AllFailChk == YesCheck )
#if(( AllFailChk == YesCheck )&&( AllFail7022Chk == NoCheck ))				//10.11.11
		if(Temp == IgainC ) 
		{	
			Flag.Power |= F_IrmsCheck;
			ADCStart( CURRENTAD );
		}
#else
#endif		
	}	
	return 0;
}	
#endif

short BuffDataIsZero( unsigned char* Source, short Length )
{
	short i;
	
	for( i=0;i<Length;i++ )
	{
		if( *(Source+i) != 0 ) return 1;	
	}	
	return 0;		
}	

unsigned char Com_Write(unsigned char* ComBuf )
{
//	unsigned short	Addr, Info, Len;
	unsigned short 	Info, Len;
	unsigned long 	Addr;
  	unsigned char Err_Byte=0;
	short ItemAds;

	ItemAds = GetComVar( &Addr, &Info, &Len, ComBuf );
	if( ItemAds == -1 ) 
	{
		Err_Byte |= RS_State_IVID;
		return	Err_Byte;
	}

	if( ~Info & EN_W ) 
	{
		Err_Byte |= RS_State_IVData;
		*ComBuf = 0xFF;
		return	Err_Byte;
	}
	*(ComBuf + Rs_Head0Ptr) = 6;		//标识码+密码长度   第二个变量暂做地址缓存
	Err_Byte = Proc_Write( ItemAds, Addr, Info, Len, ComBuf );
	*(ComBuf + Rs_Head0Ptr) = 0x68;
	return	Err_Byte;
}	

//unsigned char Proc_Write( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
unsigned char Proc_Write( short ItemAds, unsigned long Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
	unsigned short i,Length,ComID;
	unsigned char* WriteBufAds;
  	unsigned char Buff[4];
  	unsigned char* Point;
  	unsigned char Err_Byte=0;
  	
  	Point = Buff;

	WriteBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_Head0Ptr);
	
	ComID = ComTab[ ItemAds ].ComID;
	
	switch( Info & Comm_Type )
	{
		case 0x0000:
			switch( ComID )
			{
				case 0xC818:		//清电量
					break;
				case 0xC119:		//清有功电量
					break;
				case 0xC11A:			//无功电量清零
				case 0xC81B:		//清断相
				case 0xC11B:		//清断相
					break;
				case 0xC81C: 	//清过程
					break;
				default: break;
			}	
			break;
		case PARAM:		
#if ( MEASCHIP == ADE7758 )		
			if((( ComID >= 0xC098 )&&(ComID <= 0xC09B))
				||( ComID == 0xC09D )||(ComID == 0xC09E))
			{				
				if((( PIN_SETEN & P_SETEN ) != 0 )||(SM.CalibCount != CALIBCOUNT1)) *ComBuf = 0xFF;	
				else 
				{
					Swap7758Para( WriteBufAds, ComID );		//交换数据相高低字节
					E2P_WAdj( (unsigned short)Addr, WriteBufAds, Len );	
					
					if( ComID == 0xC09A )
					{
						for( i=0;i<3;i++ )
						{
							ADE7758WtReg( AWATTOS+128+i, WriteBufAds+i*2 ); //A、B、C相电能漂移			
						}	
					}
					else if( ComID == 0xC098 )
					{
						for( i=0;i<6;i++ )
						{
							ADE7758WtReg( AWG+128+i, WriteBufAds+1+i*2 ); //A、B、C相有功无功电能增益			
						}	
					}
					else if( ComID == 0xC099 )
					{
						for( i=0;i<3;i++ )
						{
							ADE7758WtReg( APHCAL+128+i, WriteBufAds+i ); 			
						}	
					}
					else if( ComID == 0xC09B )
					{
						RAM_Write( (unsigned char*)&Para.RefUa, WriteBufAds, 12 );
					}			
				}	
			}
			else
			{		
#else
#endif								
    	   		E2P_WData( (unsigned short)Addr, WriteBufAds, Len );
       			*(ComBuf + Rs_LenPtr) -= Len;
#if ( MEASCHIP == ADE7758 )		
			}		
#else
#endif
			break;
		case OTHER:
	    		if( ComID == 0xC010 )
	    		{
					*WriteBufAds = WeekDay20( WriteBufAds+1 );

					*Point = Clk.Week[0];
	    			RAM_Write( Point+1, Clk.Day, 3 );
					Clk.Week[0] = *WriteBufAds;
					Clk.Day[0] = *(WriteBufAds+1);
					Clk.Mon[0] = *(WriteBufAds+2);
					Clk.Year[0] = *(WriteBufAds+3);
//#if( RTCCHIP == RX8025 )																	
//					E2Pwrite( 0x30, WriteBufAds, 4, RTCAds );	//写时钟芯片日期
//#elif( RTCCHIP == RX8025T )																	//V1000				
//					if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
//					{
						*WriteBufAds = (unsigned char)ByteBit[*WriteBufAds];						//V1000
						E2Pwrite( 0x03, WriteBufAds, 4, RTCAds );	//写时钟芯片日期	//8025T
//					}	
//#else
//					else
//					{
//						if( Clk.Week[0] == 0 ) Clk.Week[0] = 0x07;
//						E2Pwrite( 0x03, WriteBufAds, 4, DS3231Ads );	//写时钟芯片日期
//					}	
//#endif
					if( Clk.Week[0] == 7 ) Clk.Week[0] = 0x0;
   					SM.BroadC_Mins = 0;
				}
				else if( ComID == 0xC011 )
				{
					SM.BroadC_Mins = 0;
					Clk.Sec_64 = 32;
					Clk.Sec[0] = *WriteBufAds;
					Clk.Min[0] = *(WriteBufAds+1);
					Clk.Hour[0] = *(WriteBufAds+2);				
//#if(( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))																	//V1000
//					if(( PIN_RTCSEL & P_RTCSEL ) == 0 )		//低电平为8025T				//新国网
//					{
						E2Pwrite( 0x0, Clk.Sec, 3, RTCAds );	//写时钟芯片秒分时
//					}	
//#else
//					else
//					{
//						E2Pwrite( 0x0, Clk.Sec, 3, DS3231Ads );	//写时钟芯片秒分时
//					}	
//#endif
				}
			break;	
		case DATABLK:
			Length = Len;
			*(ComBuf + Rs_Head0Ptr) = 6;		//标识码+密码长度
			for( i=1;i<Length+1;i++ )
			{
				Addr = ComTab[ ItemAds+i ].Addr;
				Info = ComTab[ItemAds+i].ComInfo;
				Len = Info & RS_LEN;
				Err_Byte = Proc_Write( ItemAds+i, Addr, Info, Len, ComBuf );
				*(ComBuf + Rs_Head0Ptr) += Len;
				if( Err_Byte != 0 ) break;
			}
			break;		
		default:break;	
	}
	return	Err_Byte;	
}

#if ( MEASCHIP == ADE7758 )
#elif ( MEASCHIP == IDT90E32 )							//12.08.29	
#elif ( MEASCHIP == ADE7878 )	
#elif ( MEASCHIP == ATT7022E )
#else
short Read_ATTValue( unsigned char Cmd, unsigned char* Data )
{
	unsigned short	i=1,Temp;
//	unsigned long Value;
	float Value;
	unsigned long A0002Ib;			//新国网		//13.08.30
	unsigned long PW00002Ib;		//新国网		//13.08.30
	unsigned char Buff[4];
	unsigned char* Point;
	unsigned char Temp2=0;				//11.08.03
	
	Point = Buff;
	
	if( (Flag.Power & F_PwrUp) == 0)	//停电以后返回值为零。
	{
		*Data = 0;
		*(Data + 1) = 0;	
		*(Data + 2) = 0;	
		return 0;
	}	

	A0002Ib = MSpec.RBaseCurrent / 500;	//新国网		//13.08.30
	PW00002Ib = MSpec.RPW00002Ib;		//新国网		//13.08.30	
//	ATT7022RdReg( Cmd, Data );
	Temp2 = ATT7022RdReg( Cmd, Data );		//11.08.03	
	if(( *(Data +2) & 0x80 ) == 0) i = 0;
	else    {
	            *(Data+2) ^= 0xff;     
	            *(Data+1) ^= 0xff;     
	            *Data ^= 0xff;     	                	                
	        }   
	Temp = *(Data + 1) * 256 + *Data;
	Value = (float)Temp + (float)*(Data + 2) * 65536;

	if( i == 1 ) 	Value += 1;
	Value = Value * 100;
	switch( Cmd )
	{
		case ATPWPA:		
		case ATPWPB:		
		case ATPWPC:		
		case ATPWQA:		
		case ATPWQB:	
		case ATPWQC:		
		case ATPWSA:		
		case ATPWSB:		
		case ATPWSC:	
//新国网		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//				Value =  ( Value * 25 ) / ( 2 * PS32 ) ;  //PS32大了10倍
//#else
				Value =  ( Value * 5 ) / ( 4 * PS32 ) ;   //Value = ( Value * 10 * 32 ) / (256 * ps32); 
//#endif
				if( Value < PW00002Ib ) {Value = 0; i =0;}               //0.5W       
						break;	
		case ATPWPZ:		
		case ATPWQZ:		
		case ATPWSZ:	 
//新国网		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//				Value = ( Value * 50 ) / PS32 ;            //PS32大了10倍
//#else
				Value = ( Value * 5 ) / PS32 ;            //Value = ( Value * 10 * 32 ) / ( 64 * PS32 );
//#endif
				if( Value < PW00002Ib ) {Value = 0; i =0;}               //0.5W       
						break;
		case ATYUaUb:
		case ATYUaUc:
		case ATYUbUc:
		case ATUrmsA:		
		case ATUrmsB:		
		case ATUrmsC:	Value = ( Value * 10 ) / 8192;
						break;
		case ATIrmsA:		
		case ATIrmsB:		
		case ATIrmsC:		
		case ATIrmsZ:	
		case RmsADC7:
//						Value = ( Value * 1000 ) / 8192;
						Value = ( Value * 125 ) / 1024;
//						Value = ( Value * 10000 ) / 8192;
//						Value = ( Value * 625 ) / 512;
//						if( Value < A002Ib ) Value = 0;
						if( Value < A0002Ib ) Value = 0;

						Temp = (unsigned short)((unsigned long)Value%10);
						Value /= 10;
						if( Temp >= 5 ) Value += 1;	
//						if( Value < 100 ) Value = 0;
						Long_BCD4( Point, (unsigned long)Value );
//						Temp = *Point;
//						RAM_Write( Point, Point+1, 3 );
//						if( Temp >= 0x50 ) _BCD3INC( Point );
						RAM_Write( Data, Point, 4 );
						return i;
						break;	

		case ATFreq:	Value = ( Value * 100 ) / 8192;
						break;	
		case ATFactorA:	
		case ATFactorB:	
		case ATFactorC:	
		case ATFactorZ:	//Value = ((Value/4) * 1000 ) / 2097152;          //先除4防溢出
		                Value = ( Value * 250 ) / 2097152;          //先除4防溢出
		                //if( Value == 999 ) Value = 1000;
						break;
		case ATAngleA:	
		case ATAngleB:	
		case ATAngleC:	
		case ATAngleZ:	Value = (Value * 360 ) / 2635359;           
						break;
		default: break;
	}
	if( Value < 100 ) Value = 0;
	Long_BCD4( Point, (unsigned long)Value );
	Temp = *Point;
	RAM_Write( Point, Point+1, 3 );
	if( Temp >= 0x50 ) _BCD3INC( Point );
	RAM_Write( Data, Point, 3 );
	//Long_BCD4( Data, Value);
//	return i;	
	if( Temp2 != 0 ) return 2;			//11.08.03		读底层数据有误
	else return i;						//11.08.03		读底层数据有误
}	
#endif


void DayInc( unsigned char* Data )
{
	_BCD1INC( Data );
	if( BCD_Byte(*Data) > ((( *(Data+1)==2 ) && ( BCD_Byte(*(Data+2)) & 0x03 )==0 )? 29:MonTab[BCD_Byte(*(Data+1))]))
	{
		*Data = 1;
		_BCD1INC( Data+1 );
		if( *(Data+1) > 0x12 )
		{						// Month
			*(Data+1) = 1;
			_BCD1INC( Data+2);
			if( *(Data+2) > 0x99 ) *(Data+2) = 0;	// Year
		}
	}		
}

void DayDec( unsigned char* Data )						//新国网
{
	unsigned char Buff[3];
	short i;

	for( i=0;i<3;i++ )
	{
		Buff[i] = BCD_Byte(*(Data+i));		
	}	

	if( Buff[0] == 0 ) Buff[0] = 1;
	if( Buff[0] == 1 )									//1日		
	{
		if( Buff[1] == 0 ) Buff[1] = 1;					//1月
		if( Buff[1] == 1 )								//1月
		{
			if( Buff[2] == 0x00 ) Buff[2] = 99;			//00年
			else Buff[2] -= 1;							//年减1	
			Buff[1] = 12;								//月份为12月
			Buff[0] = 31;								//31日		
		}
		else
		{	
			Buff[1] -= 1;								//月减1		
			if(( Buff[1] == 2 )&&(( Buff[2]%4 ) == 0 )) Buff[0] = 29;		//2月闰年为29天
			else Buff[0] = MonTab[Buff[1]];
		}					
	}
	else Buff[0] -= 1;									//非当前月第一天，直接日减1
	
	for( i=0;i<3;i++ )
	{
		*(Data+i) = Byte_BCD( Buff[i] );		
	}	
}

void MonthDec( unsigned char* Data )					//新国网
{
	unsigned char Buff[2];
	short i;

	for( i=0;i<2;i++ )
	{
		Buff[i] = BCD_Byte(*(Data+i));		
	}	

	if( Buff[0] == 0 ) Buff[0] = 1;
	if( Buff[0] == 1 )									//1月		
	{
		if( Buff[1] == 0x00 ) Buff[1] = 99;			//00年
		else Buff[1] -= 1;							//年减1	
		Buff[0] = 12;								//月份为12月
	}
	else Buff[0] -= 1;									//非当前月第一天，直接日减1
	
	for( i=0;i<2;i++ )
	{
		*(Data+i) = Byte_BCD( Buff[i] );		
	}	
}

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
short IsMatchEventUpMode( short ByteNo, short BitNo )				//新国网	//14.06.21
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	RAM_Fill( Ptr, 12 );
	*(Ptr+ByteNo) |= ByteBit[BitNo];			
	
	*(Ptr+0) &= ( Para.REventUpMode[0] & 0xBF );
	*(Ptr+1) &= Para.REventUpMode[1];
	*(Ptr+2) &= Para.REventUpMode[2];
	*(Ptr+3) &= ( Para.REventUpMode[3] & 0x01 );
	*(Ptr+4) &= Para.REventUpMode[2];
	*(Ptr+5) &= ( Para.REventUpMode[3] & 0x01 );
	*(Ptr+6) &= Para.REventUpMode[2];
	*(Ptr+7) &= ( Para.REventUpMode[3] & 0x01 );
	*(Ptr+8) &= Para.REventUpMode[4];
	*(Ptr+9) &= ( Para.REventUpMode[5] & 0x07 );

	*(Ptr+10) &= Para.REventUpMode[6];
	*(Ptr+11) &= Para.REventUpMode[7];
#if ( AuxPWFailCheck ==	NoCheck	)
	*(Ptr+8) &= ~0x10;										//辅助电源失电主动上报仅限于带辅助电源的电能表		//13.05.13
#endif

	if( IsAllData( Ptr, 12, 0x00 ) != 0 ) return 0;
	else return -1;	
}	

void UpdateEventUpState( unsigned short BitSite )				//新国网	//14.04.26
{
	unsigned char Buff[16];
	unsigned char* OldState;
	unsigned char Temp;
	short i,j;
	
	OldState = Buff;
	
	if( BitSite == 0xff ) return;
	
	i = BitSite / 8;
	j = BitSite % 8;
 	
 	if( IsMatchEventUpMode( i, j ) != 0 ) return;				//是否匹配主动上报模式字	//14.06.21

	RAM_Write( OldState, Flag.EventUpState, 12 );

	Flag.EventUpState[i] |= ByteBit[j];			
//	MatchEventUpMode();											//14.06.21	
	if(( Flag.EventUpState[i] & ByteBit[j] ) != 0 )
	{
		if( Data_Comp( OldState, Flag.EventUpState, 12 ) != 0 )		//新事件产生，更新EEPROM内的主动上报状态字
		{
//			E2P_WData( EEventUpState, Flag.EventUpState, 12 );			
			E2P_WFM( FEventUpState, Flag.EventUpState, 12 );	//新国网	//14.05.08
		}	
		if(( BitSite == 0 )||( BitSite >= 10 ))
		{
			E2P_RData( (unsigned char*)&Temp, EventUpAddCnt+i*8+j, 1 );			//读取历史次数
			if(( *(OldState+i) & ByteBit[j] ) == 0 ) Temp = 0;	//第一次产生的事件，次数为1			
			if( Temp != 0xff ) 
			{
				Temp += 1;										//事件新增1次
				E2P_WData( EventUpAddCnt+i*8+j, (unsigned char*)&Temp, 1 );
			}
		}	
	}	
}	

//单事件清零
void ClearEventUpState( unsigned short BitSite )				//新国网	//14.04.26
{
	unsigned char Buff[12];									//14.06.21
	unsigned char* Ptr;										//14.06.21
	short i,j;
	unsigned char Temp;
	
	Ptr = Buff;												//14.06.21

	if( BitSite == 0xff ) return;
	
	i = BitSite / 8;
	j = BitSite % 8;
 	
 	if(( Flag.EventUpState[i] & ByteBit[j] ) != 0 )
 	{
		Flag.EventUpState[i] &= ~ByteBit[j];
//		E2P_WData( EEventUpState, Flag.EventUpState, 12 );	
		E2P_WFM( FEventUpState, Flag.EventUpState, 12 );	//新国网	//14.05.08
	}	
	if(( BitSite == 0 )||( BitSite >= 10 ))
	{
		Temp = 0;
		E2P_WData( EventUpAddCnt+i*8+j, (unsigned char*)&Temp, 1 );			
	}	

	RAM_DataFill( Ptr, 12, 0xff );							//14.06.21
	*(Ptr+i) &= ~ByteBit[j];								//14.06.21
	ResetEventUpStateSend( Ptr );				//新国网	//14.06.21

}	

//7号补遗				//新国网	//15.03.05
//所有准备复位的位都曾被主站抄读走，即要复位的位应是已经上报过的位的子集
															//新国网	//14.06.28				
short AllResetBitIsAlreadyUpState( unsigned char* Source )	//新国网	//14.06.21
{
	unsigned char Buff[12];
	unsigned char Buff2[12];
	unsigned char Buff3[12];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char* UpState;
	short i;
	
	Ptr = Buff;
	Point = Buff2;
	UpState = Buff3;
	
	E2P_RData( Ptr, EventUpStateSend, 12 );					//置1的为已经上报过的位
	if(( Flag.BatState & F_E2PCheck ) != 0 ) return 0;		//EEPROM故障后不判已上报内容
//	if( IsAllData( Source, 12, 0x00 ) == 0 ) 				//7号补遗				//新国网	//15.03.05
//	{
//		if( Data_Comp( Ptr, Flag.EventUpState, 12 ) == 0 ) return 0;	//当复位命令为全零时，必须是当前主动上报状态已全被读走的的情况下才支持清零。
//	}	
	RAM_Write( UpState, Flag.EventUpState, 12 );
	RAM_Write( Point, Source, 12 );
	for( i=0;i<12;i++ )
	{
		*(UpState+i) = ~*(UpState+i);						//将当前状态位取反
		*(Ptr+i) |= *(UpState+i);							//当前状态位为0的，作为已上报过的为，允许复位。		
		*(Ptr+i) = ~*(Ptr+i);								//将已经上报过的位置零
		*(Point+i) &= *(Ptr+i); 							//和要复位的位相与
	}	
	if( Data_Comp( Ptr, Point, 12 ) == 0 ) return 0;		//要复位的位应是已经上报过的位的子集
	else return -1;
}	

//命令12字节清零
void CommResetEventUpState( unsigned char* Source )		//新国网	//14.04.26
{
	unsigned char Buff[100];
	unsigned char Buff2[12];								//14.05.28
	unsigned char* Point;									//14.05.28
	unsigned char* Ptr;
	short i,j;

	Ptr = Buff;
	Point = Buff2;											//14.05.28	
	
	if( IsAllData( Source, 12, 0xff ) == 0 ) return; 		//均不复位
	E2P_RData( Ptr, EventUpAddCnt, 96 );
	for( i=0;i<12;i++ )
	{
		Flag.EventUpState[i] &= *(Source+i); 				//复位主动上报状态位
		for( j=0;j<8;j++ )
		{
			if(( *(Source+i) & ByteBit[j] ) == 0 )
			{
				*(Ptr+i*8+j) = 0x00;						//复位新增次数
			}	
		}					
	}	
//	E2P_WData( EEventUpState, Flag.EventUpState, 12 );		
	E2P_WFM( FEventUpState, Flag.EventUpState, 12 );	//新国网	//14.05.08
	RAM_DataFill( Ptr+1, 9, 0xff );
//	E2P_WData( EventUpAddCnt, Ptr, 96 );			
	for( i=0;i<12;i++ )										//相等部分不再重复写	//14.05.28
	{
		E2P_RData( Point, EventUpAddCnt+i*8, 8 );
		if( Data_Comp( Ptr+i*8, Point, 8 ) != 0 )			
		{
			E2P_WData( EventUpAddCnt+i*8, Ptr+i*8, 8 );			
		}			
	}	
}

//根据复位命令内容清除相应的已上报主动上报状态字中的BIT位
void ResetEventUpStateSend( unsigned char* Source )					//新国网	//14.06.21
{
	unsigned char Buff[12];
	unsigned char Buff2[12];
	unsigned char* Ptr;
	unsigned char* Point;
	short i;
	
	Ptr = Buff;
	Point = Buff2;
	
	E2P_RData( Ptr, EventUpStateSend, 12 );
	RAM_Write( Point, Ptr, 12 );
	for( i=0;i<12;i++ )
	{
		*(Ptr+i) &= *(Source+i);			
	}	
	if( Data_Comp( Ptr, Point, 12 ) != 0 ) 
	{
		E2P_WData( EventUpStateSend, Ptr, 12 );
	}	
}	

//清零全部主动上报状态字及其新增次数
void ClearAllEventUpState( void )		//新国网	//14.04.26
{
	unsigned char Buff[100];
	unsigned char* Ptr;

	Ptr = Buff;
	
	RAM_Fill( Flag.EventUpState, 12 );
//	E2P_WData( EEventUpState, Flag.EventUpState, 12 );		
	E2P_WFM( FEventUpState, Flag.EventUpState, 12 );	//新国网	//14.05.08
	E2P_WData( EventUpStateSend, Flag.EventUpState, 12 );	//新国网	//14.04.28
	RAM_Fill( Ptr, 96 );
	RAM_DataFill( Ptr+1, 9, 0xff );						//无发生次数的填FF
	E2P_WData( EventUpAddCnt, Ptr, 96 );			
}

//上报30分钟后未收到复位命令自动清除已上报的主动上报状态字
void ClearEventUpStateSendBit( void )					//新国网	//14.04.28
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	short i;
	
	Ptr = Buff;
	
	E2P_RData( Ptr, EventUpStateSend, 12 );
	for( i=0;i<12;i++ )
	{
		*(Ptr+i) = ~(*(Ptr+i));
	}				
	if( IsAllData( Ptr, 12, 0xff ) != 0 )
	{
		CommResetEventUpState( Ptr );
		RAM_Fill( Ptr, 12 );
		E2P_WData( EventUpStateSend, Ptr, 12 );				
	}	
}	

short GetEventUpAddCntFrame( unsigned char* Dest )		//新国网	//14.04.26
{
//	unsigned char Buff[10];
	unsigned char Buff[16];				//14.05.28
	unsigned char* Ptr;
	unsigned char* Point;
	short Len;
	short i,j;
	
	Ptr = Buff;
	Point = Dest;
	
	Len = 0;
	
//	MatchEventUpMode();					//14.06.21
	for( i=0;i<12;i++ )
	{
		if( Flag.EventUpState[i] == 0 ) continue;
		E2P_RData( Ptr, EventUpAddCnt+i*8, 8 );
		for(j=0;j<8;j++)	
		{
			if(( Flag.EventUpState[i] & ByteBit[j] ) != 0 )	
			{
				if((( i == 0 )&&( j != 0 ))||(( i == 1 )&&( j<2 ))) *Point = 0xff;	//无发生次数的填FF
				else *Point = *(Ptr+j);
				Point += 1;
				Len += 1;
			}	
		}			
	}		
	*Point = 0xAA;
	Len += 1;
	
//	if( Len != 1 ) 														//新国网	//14.06.29			
//	{																	//新国网	//14.06.29	
//		E2P_WData( EventUpStateSend, Flag.EventUpState, 12 );			//保存已上报的主动上报状态字	
		E2P_RData( Ptr, EventUpStateSend, 12 );							
		if( Data_Comp( Ptr, Flag.EventUpState, 12 ) != 0 )				//需保存的已上报主动上报状态字有差异才重写EEPROM	//14.05.28		
		{
			E2P_WData( EventUpStateSend, Flag.EventUpState, 12 );		//保存已上报的主动上报状态字	
		}	
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		SM.ClearUpStateDelay = Para.RClearUpState30MIN;				//若30分钟后未收到复位主动上报状态字命令，则自动清除已经上报过的主动上报状态字中的相应位	//14.06.20
#else
		SM.ClearUpStateDelay = 30;										//若30分钟后未收到复位主动上报状态字命令，则自动清除已经上报过的主动上报状态字中的相应位
#endif
		if( SM.ClearUpStateDelay == 0 ) ClearEventUpStateSendBit();		//若自动复位延时时间为0，直接清除已上报的标志位		//新国网	//14.06.28
//	}																	//新国网	//14.06.29	
	
	return Len;
}	

//生成事件主动上报响应帧		//0x04001501	
void CreatEventUpSubsFrame( unsigned char* ComBuf )		//新国网	//14.04.26
{
	*(ComBuf+Rs_LenPtr) = 17;
	*(ComBuf+NRs_ID0Ptr) = 0x01;
	*(ComBuf+NRs_ID1Ptr) = 0x15;
	*(ComBuf+NRs_ID2Ptr) = 0x00;
	*(ComBuf+NRs_ID3Ptr) = 0x04;
	
//	MatchEventUpMode();				//新国网	//13.10.10		//14.06.21
	RAM_Write( ComBuf+NRs_RDataPtr, Flag.EventUpState, 12 );
	*(ComBuf+NRs_RDataPtr+12) = 0xAA;
	*(ComBuf+Rs_LenPtr) += GetEventUpAddCntFrame( ComBuf+NRs_RDataPtr+13 );
}	

//出现需要事件上报状态	1：上报 0：无上报
short IsOccurEventUpState( void )						//新国网		
{
//	MatchEventUpMode();				//新国网	//13.10.10		//14.06.21

	if( IsAllData( Flag.EventUpState, 12, 0x00 ) == 0 ) return 0;			//新国网	//14.04.26
	else return 1;
}
#else
//生成事件主动上报响应帧		//0x04001501	
void CreatEventUpSubsFrame( unsigned char* ComBuf )		//新国网
{
	*(ComBuf+Rs_LenPtr) = 14;
	*(ComBuf+NRs_ID0Ptr) = 0x01;
	*(ComBuf+NRs_ID1Ptr) = 0x15;
	*(ComBuf+NRs_ID2Ptr) = 0x00;
	*(ComBuf+NRs_ID3Ptr) = 0x04;
	
	MatchEventUpMode();				//新国网	//13.10.10	
	RAM_Write( ComBuf+NRs_RDataPtr, Flag.EventUpState, 10 );
	
	RAM_Fill( Flag.EventUpState, 10 );
	E2P_WData( EEventUpState, Flag.EventUpState, 10 );	
	POUT_EVENTOUT &= ~P_EVENTOUT;				//取消事件报警脚报警状态  				
}	

void MatchEventUpMode( void )					//新国网	//13.10.10
{
	Flag.EventUpState[0] &= ( Para.REventUpMode[0] & 0xBF );
	Flag.EventUpState[1] &= ( Para.REventUpMode[1] & 0x0F );
	Flag.EventUpState[2] &= Para.REventUpMode[2];
	Flag.EventUpState[3] &= ( Para.REventUpMode[3] & 0x01 );
	Flag.EventUpState[4] &= Para.REventUpMode[2];
	Flag.EventUpState[5] &= ( Para.REventUpMode[3] & 0x01 );
	Flag.EventUpState[6] &= Para.REventUpMode[2];
	Flag.EventUpState[7] &= ( Para.REventUpMode[3] & 0x01 );
	Flag.EventUpState[8] &= ( Para.REventUpMode[4] & 0xEF );
	Flag.EventUpState[9] &= ( Para.REventUpMode[5] & 0x07 );
#if ( AuxPWFailCheck ==	NoCheck	)
	Flag.EventUpState[8] &= ~0x20;											//掉电和全失压主动上报仅限于带辅助电源的电能表
	Flag.EventUpState[9] &= ~0x04;
#endif
}	

//出现需要事件上报状态	1：上报 0：无上报
short IsOccurEventUpState( void )						//新国网		
{
	MatchEventUpMode();				//新国网	//13.10.10	

	if( IsAllData( Flag.EventUpState, 10, 0x00 ) == 0 ) return 0;
	else return 1;

}
#endif

//出现需要后续帧事件上报状态	1：上报 0：无上报
short IsOccurEventUpSubsFrame( void )						//新国网		
{
	if(( Para.RMeterRunMode1 & 0x04 ) == 0 ) return 0;			//不启用事件上报后续帧标志
	if( IsOccurEventUpState() == 0 ) return 0;
	else return 1;
}

/*
//红外认证是否有效
short IsInfraredValid( unsigned char* ComBuf )						//新国网
{
//	if((PIN_SETEN & P_SETEN ) == 0 ) return 0;
	if(((PIN_SETEN & P_SETEN ) == 0 )||( IsFactoryTestState() == 0 )) return 0;		//厂内状态	//新国网	//14.04.09
	if(( ComBuf == Comm.Buf0 )&&( ICEC.InfraredValidDelay == 0 )) return -1;
	return 0;	
}	

//该ID码是否需要红外认证
//只能读出表号、通信地址、备案号、当前日期、当前时间、当前电能、当前剩余金额、红外认证查询命令，其它信息不允许读出，所有信息均不允许设置
short NeedInfraredIdentity( unsigned long ComID )								//新国网
{
	unsigned char Buff[4];
	
//	if(( ComID == 0x04000401 )||( ComID == 0x04000402 )||( ComID == 0x04800004 )||( ComID == 0x04000101 )
	if(( ComID == 0x04000401 )||( ComID == 0x04000402 )||( ComID == 0x04800004 )||( ComID == 0x04000101 )||( ComID == 0x000B0000 )||( ComID == 0x000C0000 )		//新国网	//13.10.28
		||( ComID == 0x04000102 )||( ComID == 0x0400010C )||( ComID == 0x00900200 )||( ComID == 0x078003FF )||( ComID == 0x070003FF )) return -1;
	RAM_Write( Buff, (unsigned char*)&ComID , 4 );
	if(( Buff[0] == 0x00 )&&( Buff[3] == 0x00 )&&(( Buff[2] <= 0x0A )||((Buff[2] >= 0x15 )&&(Buff[2] <= 0x1E ))		//当前电能
		||((Buff[2] >= 0x29 )&&(Buff[2] <= 0x32 ))||((Buff[2] >= 0x3D )&&(Buff[2] <= 0x46 ))))	return -1;
#if ( ExactECRead == YesCheck )			//读取精确电能	//新国网 	//14.04.25				
	else if( IsExactECID( ComID ) == 0 ) return -1;			//当前精确电能	//新国网 	//14.04.25
#endif
	else return 0;	
}	

//红外口的ID码是否需要红外认证
short IsInfraredIdentityValid( unsigned char* ComBuf, unsigned char CtlByte )				//新国网
{
	if( IsInfraredValid( ComBuf ) != 0 )
	{
		*(ComBuf+Rs_CtlPtr) = CtlByte;				
  		*(ComBuf+Rs_LenPtr) = 1;         
  		*(ComBuf+Rs_IDLPtr) = RS_State_PswE;  				
  		return -1;
	}	
	else return 0;
}	

//红外口的ID码是否需要红外认证
short IsInfraredIdentityValidComID( unsigned char* ComBuf, unsigned char CtlByte, unsigned char* ComID  )				//新国网
{
	unsigned long Value;
	
	RAM_Write( (unsigned char*)&Value, ComID , 4 );
	
	if(( IsInfraredValid( ComBuf ) != 0 )&&( NeedInfraredIdentity( Value ) == 0 ))
	{
		*(ComBuf+Rs_CtlPtr) = CtlByte;				
  		*(ComBuf+Rs_LenPtr) = 1;         
  		*(ComBuf+Rs_IDLPtr) = RS_State_PswE;  				
  		return -1;
	}	
	else return 0;
}	
*/

//厂内且处于公钥状态，开放设置
short IsFactoryTestState( void )							//新国网	//14.04.09		
{
	if(( SM.FactoryState == 0x55 )&&( ICComm.RPrivateKeyFlag == 0x55 )) return 0;
	else return -1;
}	

#if ( ExactECRead == YesCheck )			//读取精确电能	//新国网 	//14.04.25				
short IsExactECID( unsigned long ComID )					//新国网 	//14.04.25				
{
	unsigned char Buff[4];
	
	RAM_Write( Buff, (unsigned char*)&ComID , 4 );
	if(( Buff[0] == 0x00 )&&( Buff[3] == 0x00 )&&(( Buff[1] <= 0x04 )||(Buff[1] == 0xff ))&&((Buff[2] == 0x60 )||(Buff[2] == 0x61 ))) return 0;		//当前精确电能	//新国网 	//14.04.25
	else return -1;
}	

#endif

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
#if ( ModifyTimeSkipDelay == YesCheck )						//新国网	//14.06.19

#endif
#endif

#if ( FlashTypeSelect == YesCheck )		//支持Flash类型设置		//新国网	//14.06.04	
short IsFlashType32AndBelow( void )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	short i;
	
	Ptr = Buff;
			
	Read_FlashID( Ptr );
//	for( i=0;i<8;i++ )
	for( i=0;i<9;i++ )					//15.09.30
	{
		if(( FlashType32AndBelow[i*3] == *Ptr )&&( FlashType32AndBelow[i*3+1] == *(Ptr+1) )&&( FlashType32AndBelow[i*3+2] == *(Ptr+2) )) break;			
	}	
	
//	if( i != 8 ) return 0;
	if( i != 9 ) return 0;				//15.09.30	
	else return 1;	
}	

short IsFlashType64( void )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	short i;
	
	Ptr = Buff;
			
	Read_FlashID( Ptr );
//	for( i=0;i<2;i++ )
	for( i=0;i<3;i++ )					//15.09.30
	{
		if(( FlashType64[i*3] == *Ptr )&&( FlashType64[i*3+1] == *(Ptr+1) )&&( FlashType64[i*3+2] == *(Ptr+2) )) break;			
	}	
	
//	if( i != 2 ) return 0;
	if( i != 3 ) return 0;				//15.09.30	
	else return 1;	
}	
#endif