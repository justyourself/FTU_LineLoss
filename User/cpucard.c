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
#include "YearSave.h"									//新国网	//13.11.29

#if ( LocalSmartMeter == YesCheck )
#else
void GetAndCompareESAMPara( void )
{
	unsigned char Buff[50];
	unsigned char Buff2[10];
	unsigned char* Ptr;
	unsigned char* Point;	
	unsigned long Value1;
	unsigned long Value2;
	unsigned short Len;
	unsigned short Length;
    
	Ptr = Buff;
	Point = Buff2;

	GetCurSecretKeyState();
	
//	if( ReadBinary( ESAM, Ptr, FILE_No2, 0x04, 38 ) == 0 ) 
	if( ReadBinary( ESAM, Ptr, FILE_No4, 0x04, 38 ) == 0 ) 
	{	
//		E2P_RData( Point, UserType, 1 );						//用户类型
//		if( *Ptr != *Point ) E2P_WData( UserType, Ptr, 1 );		
		
//		SwapData( Ptr+6, 5 );
//		E2P_RData( Point, FeeSwitchTime, 5 );					//分时费率切换时间
//		if( Data_Comp( Ptr+6, Point, 5 ) != 0 )
//		{
//			E2P_WData( FeeSwitchTime, Ptr+6, 5 );					
//		}	
		
//		SwapData( Ptr+20, 3 );
//		E2P_RData( Point, CurrentRate, 3 );						
//		if( Data_Comp( Ptr+20, Point, 3 ) != 0 )
//		{
//			E2P_WData( CurrentRate, Ptr+20, 3 );					
//		}	

		SwapData( Ptr+20, 3 );
		Value1 = BCD3_Long( Ptr+20 );
		Value1 *= 100;
		E2P_RData( (unsigned char*)&Value2, CurrentRate, 4 );						
		if( Value1 != Value2 )
		{
			E2P_WData( CurrentRate, (unsigned char*)&Value1, 4 );					
		}	

//		SwapData( Ptr+23, 3 );
//		E2P_RData( Point, VoltRate, 3 );						
//		if( Data_Comp( Ptr+23, Point, 3 ) != 0 )
//		{
//			E2P_WData( VoltRate, Ptr+23, 3 );					
//		}	

		SwapData( Ptr+23, 3 );
		Value1 = BCD3_Long( Ptr+23 );
		Value1 *= 100;
		E2P_RData( (unsigned char*)&Value2, VoltRate, 4 );						
		if( Value1 != Value2 )
		{
			E2P_WData( VoltRate, (unsigned char*)&Value1, 4 );					
		}	
	
//		SwapData( Ptr+26, 6 );
//		E2P_RData( Point, EUser_No, 6 );						
//		if( Data_Comp( Ptr+26, Point, 6 ) != 0 )
//		{
//			E2P_WData( EUser_No, Ptr+26, 6 );					
//		}	
	
//		SwapData( Ptr+32, 6 );
//		E2P_RData( Point, Client_No, 6 );						
//		if( Data_Comp( Ptr+32, Point, 6 ) != 0 )
//		{
//			E2P_WData( Client_No, Ptr+32, 6 );					
//		}	
	}
	
//	if( ReadBinary( ESAM, Ptr, FILE_No1, 0x05, 8 ) == 0 ) 		
//	{
//		SwapData( Ptr, 8 );
//		E2P_RData( Point, EMeter_No, 8 );						
//		if( Data_Comp( Ptr, Point, 8 ) != 0 )
//		{
//			E2P_WData( EMeter_No, Ptr, 8 );					
//		}	
//	}	

	if( ReadBinary( ESAM, Ptr, FILE_No1, 0x04, 9 ) == 0 ) 	//多取1字节长度字节
	{
		Len = *Ptr;
		if( Len < 8 ) Length = 8 - Len;
		else 
		{
			Length = 0;
			Len = 8;
		}	
		SwapData( Ptr+1, Len );
		RAM_Fill( Ptr+1+Len, Length );
		
		E2P_RData( Point, EMeter_No, 8 );						
		if( Data_Comp( Ptr+1, Point, 8 ) != 0 )
		{
			E2P_WData( EMeter_No, Ptr+1, 8 );					
		}	
	}	
	
	if( ReadRecord( ESAM, Ptr ) == 0 ) 					//返回4字节剩余金额、4字节购电次数、6字节客户编号	
	{
		SwapData( Ptr+8, 6 );
		E2P_RData( Point, Client_No, 6 );						
		if( Data_Comp( Ptr+8, Point, 6 ) != 0 )
		{
			E2P_WData( Client_No, Ptr+8, 6 );					
		}	
	}		
}

short Card_CoolRST( short CARD_TYPE )
{

	return 0;
}

void Card_Release( short CARD_TYPE )
{
	return;
}
#endif

#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#else																		//新国网	//13.11.06

#if ( MagnKeepRelay	== YesCheck )
void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	ICComm.RelayDrivePulseWidth = 5;
	
	if( DriveType == RELAY_ON ) 
	{
//		POUT_JDQA &= ~P_JDQA;  
		HT_GPIOD->PTCLR |= GPIOD_RLYON;//POUT_JDQA &= ~P_JDQA;  			//11.03.29
		Delay(30);						//11.05.06
//		POUT_JDQB |= P_JDQB;  
		HT_GPIOD->PTSET |= GPIOD_RLYOFF;//POUT_JDQB |= P_JDQB;  
	}	
	if( DriveType == RELAY_OFF ) 
	{
//		POUT_JDQB &= ~P_JDQB;  			//11.05.06
		HT_GPIOD->PTCLR |= GPIOD_RLYOFF;//POUT_JDQB &= ~P_JDQB;  
		Delay(30);						//11.05.06
//		POUT_JDQA |= P_JDQA;  
		HT_GPIOD->PTSET |= GPIOD_RLYON;//POUT_JDQA |= P_JDQA;  			//11.03.29
//		POUT_JDQB &= ~P_JDQB;  
		HT_GPIOD->PTCLR |= GPIOD_RLYOFF;//POUT_JDQB &= ~P_JDQB;  
	}			
}	
#else
void RelayDrive( unsigned char DriveType )
{
	ICComm.RelayDriveType = DriveType;
	if(( Para.RMeterRunMode1 & 0x01 ) != 0 ) ICComm.RelayDrivePulseWidth = SM.RRelayPulseWide;		//脉宽3S
	else ICComm.RelayDrivePulseWidth = 0;
	
	if( DriveType == RELAY_ON ) 
	{
#if ( NewHardScheme2 == YesCheck )		//10.12.25
//		POUT_JDQA &= ~P_JDQA;  			//11.03.29
		HT_GPIOD->PTCLR |= GPIOD_RLYON;//POUT_JDQA &= ~P_JDQA;  			//11.03.29
#else
//		PDIR_JDQA &= ~P_JDQA;  
		HT_GPIOD->PTDIR &= ~GPIOD_RLYON;//PDIR_JDQA &= ~P_JDQA;  
#endif
	}	
	if( DriveType == RELAY_OFF ) 
	{
#if ( NewHardScheme2 == YesCheck )		//10.12.25
//		POUT_JDQA |= P_JDQA;  			//11.03.29
		HT_GPIOD->PTSET |= GPIOD_RLYON;//POUT_JDQA |= P_JDQA;  			//11.03.29
#else
//		PDIR_JDQA |= P_JDQA;  
		HT_GPIOD->PTDIR |= GPIOD_RLYON;//PDIR_JDQA |= P_JDQA;  
#endif
	}			
}	
#endif

//每三分之一秒执行一次
void JudgeSTA_State( void )					//新国网			
{
#if ( CarryCommWithOC == YesCheck )			//10.11.23
//老方案使用光耦反向，载波常态为低，而单片机端常态为高																			//10.11.23
//	if(( PIN_STAZB & P_STAZB ) != 0 ) SM.CarrySTADelay = 0;		//允许执行	
	if(( HT_GPIOA->PTDAT & GPIOA_PLC_STA ) != 0 ) SM.CarrySTADelay = 0;		//允许执行
	else 
	{
		if( SM.CarrySTADelay < 7 )	SM.CarrySTADelay++;			//不允许执行最多2秒		//新国网	//13.10.25
	}	 	
#else
//新方案取消光耦，用三极管切换电平，逻辑电平不反向，载波常态为低，单片机端常态也为低											//10.11.23
//	if(( PIN_STAZB & P_STAZB ) == 0 ) SM.CarrySTADelay = 0;		//允许执行
	if(( HT_GPIOA->PTDAT & GPIOA_PLC_STA ) == 0 ) SM.CarrySTADelay = 0;		//允许执行
	else 
	{
		if( SM.CarrySTADelay < 7 )	SM.CarrySTADelay++;			//不允许执行最多2秒		//新国网	//13.10.25
	}	 	
#endif	
}

#if ( LocalSmartMeter == YesCheck )
#else
void OperateRelay( void )
{
	if( Para.RMeterSafeState == 0x3A )									//保电
	{
#if ( RelayONImmediately == YesCheck )
		if(( Para.RMeterRelayState == 0x1A )&&(ICComm.RelayONState != 0x55)) ICComm.RelayONState = 0x55;	//立即合闸
#else
		if(( Para.RMeterRelayState == 0x1A )&&(ICComm.RelayONState != 0x55)) ICComm.RelayONState = 0xAA;	//允许合闸
#endif
	}	
	if(( ICComm.TripOFFDelay == 0 )&&( Para.RMeterTripState == 0x1A ))	//远程跳闸
	{			
		if( Para.RMeterRelayState != 0x1A ) ICComm.RelayOFFState = 0x55;//准备跳闸																											
		ICComm.RelayONState = 0x0;
	}	

	if(( Para.RMeterRelayState == 0x1A )&&( Para.RMeterTripState == 0x1B ))
	{
#if ( RelayONImmediately == YesCheck )
		if( ICComm.RelayONState	== 0 ) ICComm.RelayONState = 0x55;		//立即合闸(可能刚上电)
#else
		if( ICComm.RelayONState	== 0 ) ICComm.RelayONState = 0xAA;		//允许合闸(可能刚上电)
#endif		
	}	

#if ( CarryComm == YesCheck )
	if((( SM.CarrySTADelay == 0 )||( SM.CarrySTADelay >= 6 ))&&( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 ))	//执行合闸			//10.11.23	//新国网 	/13.10.25
#else
	if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 ))	//执行合闸		
#endif
	{
			RecordStartEvent( TripON_No );
			ICComm.RelayDriveCnt = 5;	
			RelayDrive( RELAY_ON );	
			ICComm.RelayONState = 0x0;
			ICComm.RelayOFFState = 0x00;			//合闸后，取消允许拉闸状态	//新国网	//13.10.25
			Para.RMeterRelayState = 0x55;			//已经合闸
			E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
//			CreateTripRec( TripON_No );	 		
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			EVar.RelayFaultVar.Cunt = 1;														//新国网	//14.05.03	
			EVar.RelayFaultVar.OldState = 0xff;													//新国网	//14.05.03
			E2P_WData( RelayFault_OldState, (unsigned char*)&EVar.RelayFaultVar.OldState, 1 );	//新国网	//14.05.12	
			Flag.RSwitchOldState &= 0x04;														//新国网	//14.05.03	
			Flag.RSwitchOldState |= 0x40;			//允许记录异常事件							//新国网	//14.05.12				
			E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//新国网	//14.05.03	
#endif
			RecordEndEvent( TripON_No, 0 );
	}				

#if ( CarryComm == YesCheck )
	if((( SM.CarrySTADelay == 0 )||( SM.CarrySTADelay >= 6 ))&&( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))	//执行跳闸		//10.11.23	//新国网	//13.10.25
#else
	if(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))	//执行跳闸		
#endif
	{
#if ( IOver30ALimitTrip	== YesCheck )		//新国网	//14.04.25
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		if(( IOver30AJudge() == 0 )||( SM.Up30ATimeCount >= Para.RUp30AClose24HOUR ))			//新国网	//14.06.19
#else
		if(( IOver30AJudge() == 0 )||( SM.Up30ATimeCnt == UP30A24HOUR ))
#endif
		{
#endif
			RecordStartEvent( TripOFF_No );
			ICComm.RelayDriveCnt = 5;	
			RelayDrive( RELAY_OFF );	
			ICComm.RelayOFFState = 0x0;
			ICComm.RelayONState = 0x00;				//拉闸后，取消允许合闸状态	//新国网	//13.10.25
			Para.RMeterRelayState = 0x1A;			//已经跳闸
			E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
//			CreateTripRec( TripOFF_No );	 		
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			EVar.RelayFaultVar.Cunt = 1;														//新国网	//14.05.03
			EVar.RelayFaultVar.OldState = 0xff;													//新国网	//14.05.03	
			E2P_WData( RelayFault_OldState, (unsigned char*)&EVar.RelayFaultVar.OldState, 1 );	//新国网	//14.05.12	
			Flag.RSwitchOldState &= 0x04;														//新国网	//14.05.03	
			Flag.RSwitchOldState |= 0x40;			//允许记录异常事件							//新国网	//14.05.12							
			E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//新国网	//14.05.03	
#endif
			RecordEndEvent( TripOFF_No, 0 );
#if ( IOver30ALimitTrip	== YesCheck )		//11.06.29
		}
#endif
	}				
}		
#endif
#endif


#if ( LocalSmartMeter == YesCheck )

void CYCDelay(unsigned short cycle) /*输入cycle为希望的周期数的1/6。*/ 
{
    unsigned short t;
    t = cycle;
    while(t > 1)
    {
        t--;
    }
}
/*
void CommInitIC( short CARD_TYPE )
{
	if( CARD_TYPE == PSAM )
	{
		ICComm.ICBuf1[0] = 0xff;
		ICComm.ICPtr1 = 0;
		ICComm.ICBTime1 = 0;					// 字符间定时
		ICComm.ICSendLength1 = 0;				// IC口1软件接收发送字节数
		ICComm.Run &= ~F_ICComOK1;
   		ICRxInitial1();
   	}
   	else
   	{
		SM.ESAMByteCnt = 0;						//10.11.13
		ICComm.ICBuf2[0] = 0xff;
		ICComm.ICPtr2 = 0;
		ICComm.ICBTime2 = 0;					// 字符间定时
		ICComm.ICSendLength2 = 0;	        // IC口2软件接收发送字节数
		ICComm.Run &= ~F_ICComOK2;
   		ICRxInitial2();	
   	}			
}
*/
ISO7816_InitTypeDef ISO7816_InitStruct;
#if ( ISO7816Config	== YesCheck )
void ICRxInitial1( void )
{
	/***** 以下代码用于配置ISO7816通讯*****/
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);    /*!< 使能UART37816EN模块              */
			
	ISO7816_InitStruct.ISO7816_Parity = ISO7816Parity_EVEN;		//ISO7816奇偶校验位选择   
//    ISO7816_InitStruct.ISO7816_Parity = ISO7816Parity_ODD;		//ISO7816奇偶校验位选择   
	ISO7816_InitStruct.ISO7816_RepeatTime = Repeat_Three;       //ISO7816重发重收次数设定 
	ISO7816_InitStruct.AutoRxEN = ENABLE;                 		//ISO7816自动接收使能控制 
	ISO7816_InitStruct.AutoTxEN = ENABLE;                 		//ISO7816自动发送使能控制 
	ISO7816_InitStruct.ISO7816_AckLen = ACKLen_1Bit;            //ISO7816响应位长度设定   
//    ISO7816_InitStruct.ISO7816_AckLen = ACKLen_1P5Bit;            //ISO7816响应位长度设定   
//    ISO7816_InitStruct.ISO7816_AckLen = ACKLen_2Bit;            //ISO7816响应位长度设定   
//	ISO7816_InitStruct.ISO7816_BaudRate = 11275;         		//ISO7816波特率设定     10753(4.000M)、11275(4.096M)  
	ISO7816_InitStruct.ISO7816_BaudRate = 10753;         		//ISO7816波特率设定     10753(4.000M)、11275(4.096M)  

	HT_ISO7816_Init( HT_ISO7816_1, &ISO7816_InitStruct );
	HT_ISO7816_ITConfig(HT_ISO7816_1, UART_ISO7816CON_RXIE, ENABLE );
}	

void ICTxInitial1( void )
{
	HT_ISO7816_1->ISO7816STA &= ~(UART_ISO7816STA_RXIF|UART_ISO7816STA_TXIF);
  	HT_ISO7816_1->ISO7816CON |= UART_ISO7816CON_TXIE;     		
    HT_ISO7816_1->SBUF = ICComm.ICBuf1[0];     		
}	
#else				//用UART方式通讯
void ICRxInitial1( void )
{
    UART_InitTypeDef UART_InitStructure; 
		    /***** 以下代码用于配置UART3通讯*****/
	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);        /*!< 使能UART3模块              */
				
	/*!< UART1配置信息               */
	UART_InitStructure.UART_Logic = UartLogicPositive;          /*!< UART逻辑为正               */
//	UART_InitStructure.UART_StopBits = OneStopBits;             /*!< 1位停止位                  */
	UART_InitStructure.UART_StopBits = TwoStopBits;             /*!< 1位停止位                  */
	UART_InitStructure.UART_WordLength = WordLength_8Bits;      /*!< 8位数据位                  */
	UART_InitStructure.UART_Parity = UartParity_EVEN;        	/*!< 偶校验                     */
	UART_InitStructure.UART_BaudRate = 10753;          			/*!< 波特率2400                 */
	UART_InitStructure.ReceiveEN = ENABLE;                      /*!< 接收使能   注：非中断使能  */
	UART_InitStructure.SendEN = DISABLE;                         /*!< 发送使能   注：非中断使能  */		
	
	HT_UART_Init(HT_UART3, &UART_InitStructure);	
	HT_UART_ITConfig(HT_UART3, UART_UARTCON_RXIE, ENABLE);
}	

void ICTxInitial1( void )
{
	HT_UART_ITConfig(HT_UART3, UART_UARTCON_RXIE, DISABLE);
	HT_UART3->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);
	HT_UART3->UARTCON |= UART_UARTCON_TXIE + UART_UARTCON_TXEN;
    HT_UART3->SBUF = ICComm.ICBuf1[0];     		
}	

#endif

void CommInitIC( short CARD_TYPE )
{
	if( CARD_TYPE == PSAM )
	{
		ICComm.ICBuf1[0] = 0xff;
		ICComm.ICPtr1 = 0;
		ICComm.ICBTime1 = 0;					// 字符间定时
		ICComm.ICSendLength1 = 0;				// IC口1软件接收发送字节数
		ICComm.Run &= ~F_ICComOK1;
   		ICRxInitial1();
   	}
//   	else
//   	{
//		SM.ESAMByteCnt = 0;						//10.11.13
//		ICComm.ICBuf2[0] = 0xff;
//		ICComm.ICPtr2 = 0;
//		ICComm.ICBTime2 = 0;					// 字符间定时
//		ICComm.ICSendLength2 = 0;	        // IC口2软件接收发送字节数
//		ICComm.Run &= ~F_ICComOK2;
//   		ICRxInitial2();	
//   	}			
}

short RecDataDelay( short CARD_TYPE, unsigned long Cycle, unsigned short RecLength )
{
	unsigned long i;
	unsigned short ICComOK;
	unsigned short* ICBTime;				//V6
	unsigned short* RecLen;				//V6
	
	if( CARD_TYPE == PSAM ) 
	{
		ICComOK = F_ICComOK1;
		ICBTime = &ICComm.ICBTime1;		//V6
		RecLen = &ICComm.ICRecLength1;
	}	
//	else 
//	{
//		ICComOK = F_ICComOK2;
//		ICBTime = &ICComm.ICBTime2;		//V6
//		RecLen = &ICComm.ICRecLength2;
//	}	
	
	*RecLen = RecLength;
	
	i=Cycle;
	while( i>0)
	{
// 		WDTCTL = WDT_ARST_1000;
		HT_FreeDog();
		if(( ICComm.Run & ICComOK ) != 0 ) break;	
		i--;
	}

	*RecLen = 0;

	if( i == Cycle ) return -1;
	if( i == 0 ) *ICBTime = 0;			//V6
	return 0;	
}	

/*
short Card_CoolRST( short CARD_TYPE )
{
	unsigned short* ComPtr;
	unsigned char* ComBuf;
	short RST_Port;
	
	WDTCTL = WDT_ARST_1000;
	if( CARD_TYPE == PSAM )
	{
		PDIR_CARDPW |= P_CARDPW;
		RST_Port = P_CARDRST;
		ComPtr = &ICComm.ICPtr1;
		ComBuf = ICComm.ICBuf1;		
	}	
	else
	{
#if( ESAMPowerControl == YesCheck )			//10.12.02			
		POUT_ESAMPW &= ~P_ESAMPW;			//打开ESAM电源
		CYCDelay(6500*4);					//电源稳定时间
#endif
		RST_Port = P_ESAMRST;
		ComPtr = &ICComm.ICPtr2;
		ComBuf = ICComm.ICBuf2;		
	}	

	PDIR_ESAMCLK |= P_ESAMCLK;
	PSEL_ESAMCLK |= P_ESAMCLK;
	CYCDelay(6500*4);						//>= 400/f	//约6.5ms
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	WDTCTL = WDT_ARST_1000;
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	WDTCTL = WDT_ARST_1000;
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	WDTCTL = WDT_ARST_1000;

#if ( CardIOTwoLine == YesCheck )		     
	if( CARD_TYPE == PSAM )					//PIO	
	{										//PIO	
		P4OUT &= ~RST_Port;					//PIO	
		P4DIR |= RST_Port;					//PIO	
		CYCDelay(650*4);					//PIO		//>= 400/f
		P4DIR &= ~RST_Port;					//PIO	
	}										//PIO	
	else									//PIO	
	{										//PIO	
		P1OUT &= ~RST_Port;
		P1DIR |= RST_Port;
		CYCDelay(650*4);								//>= 400/f
		P1DIR &= ~RST_Port;		
	}										//PIO	
#else
	P1OUT &= ~RST_Port;
	P1DIR |= RST_Port;
	CYCDelay(650*4);						//>= 400/f
	P1DIR &= ~RST_Port;
#endif

	CommInitIC( CARD_TYPE );
	RecDataDelay( CARD_TYPE, 65000*4, 13 );	
	if(( *ComPtr < 13 )||( *ComBuf != 0x3B)||( *(ComBuf+1) != 0x69 )) 
	{
		if( CARD_TYPE == ESAM )																//新国网
		{																					//新国网
			if(( Flag.MeterState1 & F_ESAMErr ) == 0 ) UpdateEventUpState( 1 );				//新国网	//13.10.08
		}																					//新国网		
		return -1; 
	}	
	if( CARD_TYPE == PSAM ) RAM_Write( Para.CardNo, ComBuf+5, 8 );
	else 
	{
		RAM_Write( Para.ECardNo, ComBuf+4, 9 );
		SwapData( Para.ECardNo, 9 );
		E2P_WData( ESAMCardNo, Para.ECardNo, 8 );		
	}	
	return 0;
}	
*/

short Card_CoolRST( short CARD_TYPE )
{
	unsigned short* ComPtr;
	unsigned char* ComBuf;
	
	HT_FreeDog();
	if( CARD_TYPE == PSAM )
	{
		VCARD_CLR;
		ComPtr = &ICComm.ICPtr1;
		ComBuf = ICComm.ICBuf1;		
	}	
	else
	{
#if( ESAMPowerControl == YesCheck )			//10.12.02			
		VESAM_CLR;							//打开ESAM电源
		CYCDelay(6500*4);					//电源稳定时间
#endif
	}	

	CYCDelay(6500*4);						//>= 400/f	//约6.5ms
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	HT_FreeDog();
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	HT_FreeDog();
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	CYCDelay(6500*4);						//>= 400/f
	HT_FreeDog();

	if( CARD_TYPE == PSAM )					//PIO	
	{										//PIO	
		CARDRST_CLR;
		CYCDelay(650*4);					//PIO		//>= 400/f
		CARDRST_SET;
	}										//PIO	

	if( CARD_TYPE == PSAM )
	{
		CommInitIC( CARD_TYPE );
		RecDataDelay( CARD_TYPE, 65000*4, 13 );	
		if(( *ComPtr < 13 )||( *ComBuf != 0x3B)||( *(ComBuf+1) != 0x69 )) 
		{
			return -1; 
		}
		RAM_Write( Para.CardNo, ComBuf+5, 8 );			
	}
	else
	{
		if( GetESAMSaftInfo( Para.ECardNo, 0x02 ) != -1 )
		{
			SwapData( Para.ECardNo, 8 );
			E2P_WData( ESAMCardNo, Para.ECardNo, 8 );			
		}	
		else
		{
			if(( Flag.MeterState1 & F_ESAMErr ) == 0 ) UpdateEventUpState( 1 );				//新国网	//13.10.08	
			return -1;
		}	
	}			
	return 0;
}	

/*	
void Card_Release( short CARD_TYPE )
{
	if( CARD_TYPE == ESAM )
	{	
		PDIR_ESAMRST |= P_ESAMRST;
		CYCDelay(6500);						//>= 400/f
		PSEL_ESAMCLK &= ~P_ESAMCLK;
		PDIR_ESAMCLK &= ~P_ESAMCLK;
#if( ESAMPowerControl == YesCheck )			//10.12.02			
		TA0CCTL2 = 0;
		PDIR_ESAMIO &= ~P_ESAMIO; 
		PSEL_ESAMIO &= ~P_ESAMIO; 

		POUT_ESAMIO &= ~P_ESAMIO; 
		PDIR_ESAMIO |= P_ESAMIO; 

		POUT_ESAMCLK &= ~P_ESAMCLK;
		PDIR_ESAMCLK |= P_ESAMCLK;
		CYCDelay(1000);						//>= 400/f
		POUT_ESAMPW |= P_ESAMPW;			//关闭ESAM电源
#endif
	}
	else
	{
		PDIR_CARDRST |= P_CARDRST;					
		CYCDelay(6500);						//>= 400/f
		PDIR_CARDPW &= ~P_CARDPW;
	}			
}
*/

void Card_Release( short CARD_TYPE )
{
	if( CARD_TYPE == ESAM )
	{	
#if( ESAMPowerControl == YesCheck )			//10.12.02			
//		ESAMCS_SET;
//		CYCDelay(1000);						//>= 400/f
//		ESAMCLK_CLR;
//		CYCDelay(1000);						//>= 400/f
//		ESAMMOSI_CLR;
//		CYCDelay(1000);						//>= 400/f
//		VESAM_SET;
//		ESAMCS_CLR;
#endif
	}
	else
	{
		CARDRST_CLR;
		CYCDelay(6500);						//>= 400/f
//		PDIR_CARDPW &= ~P_CARDPW;
		VCARD_SET;
	}			
}

//保存2字节错误响应状态
void SaveErrorResponse( short CARD_TYPE )							//新国网	//13.12.02
{
	if( CARD_TYPE == PSAM )
	{
		if( ICComm.ICPtr1 != 0 ) 
		{
			RAM_Write( ICComm.ErrorResponse, ICComm.ICBuf1, 2 );			
			if(( ICComm.ErrorResponse[0] == 0x6B )&&( ICComm.ErrorResponse[1] == 0x00 )) ICComm.ErrorMessage = ICERROR_07;
			else ICComm.ErrorMessage = ICERROR_19;
		}else ICComm.ErrorMessage = ICERROR_19;					//新国网	//14.01.06	
	}
	else
	{
//		if( ICComm.ICPtr2 != 0 ) RAM_Write( ICComm.ErrorResponse, ICComm.ICBuf2, 2 );
		ICComm.ErrorMessage = ICERROR_02;						//操作ESAM错			
	}			
}	 

//保存5字节操作命令头
void SaveOperateCommHead( short CARD_TYPE )							//新国网	//13.12.02
{
	RAM_Fill( ICComm.ErrorResponse, 2 ); 
	if( CARD_TYPE == PSAM )
	{
		RAM_Write( ICComm.OperateCommHead, ICComm.ICBuf1, 5 );			
	}
//	else RAM_Write( ICComm.OperateCommHead, ICComm.ICBuf2, 5 );			
}	 

//清除5字节操作命令头
void ClearOperateCommHead_ErrorResponse( void )							//新国网	//13.12.02
{
	RAM_Fill( ICComm.OperateCommHead, 5 );
	RAM_Fill( ICComm.ErrorResponse, 2 ); 
}

void ICDataSend( short CARD_TYPE, short Length, unsigned long Cycle, unsigned short RecLength )
{
	if( RecLength == 1 ) SaveOperateCommHead( CARD_TYPE );			//等待返回数据长度为1时，均为命令帧		//新国网	//13.12.02
	
	if( CARD_TYPE == PSAM )
	{
		ICComm.ICPtr1 = 0;
		ICComm.ICSendLength1= Length;
		ICComm.Run &= ~F_ICComOK1; 
		ICComm.ICRxTx_Data1 = ICComm.ICBuf1[0];
//		TA0CCR3 = TA0R;
		ICTxInitial1();
	}
//	else
//	{
//		ICComm.ICPtr2 = 0;
//		ICComm.ICSendLength2= Length;
//		ICComm.Run &= ~F_ICComOK2; 
//		ICComm.ICRxTx_Data2 = ICComm.ICBuf2[0];
//		TA0CCR2 = TA0R;
//		ICTxInitial2();
//	}		
	RecDataDelay( CARD_TYPE, Cycle, RecLength );	
}		

/*
//(程序比对加密用)
short CompareEncrypt( unsigned char* Dest, unsigned char* Source, unsigned short Length )		//新国网
{
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;						//读写出错
	
		ICComm.ICBuf2[0] = 0x80;						//主密钥分散认证	
		ICComm.ICBuf2[1] = 0xFA;
		ICComm.ICBuf2[2] = 0x01;
		ICComm.ICBuf2[3] = 0x00;
		ICComm.ICBuf2[4] = Length;
	
		ICDataSend( ESAM, 4, CYC5000, 1 );				//V6		
		if( ICComm.ICBuf2[0] != 0xFA ) { SaveErrorResponse( ESAM ); continue; }				//保存2字节错误响应状态 //新国网	//13.12.02
	
		RAM_Write( ICComm.ICBuf2, Source, Length );

		ICDataSend( ESAM, Length-1, CYC60000, 2 );			//V6
		
		if(( ICComm.ICBuf2[0] == 0x61 )&&( ICComm.ICBuf2[1] == Length )) 
		{
			if( GetResponse( ESAM, ICComm.ICBuf2, Length ) != 0 ) return -1;
			RAM_Write( Dest, ICComm.ICBuf2, Length );	
			ICComm.ICAlarmFlg = 0x00;
			return 0;	
		}
		else SaveErrorResponse( ESAM );					//保存2字节错误响应状态 //新国网	//13.12.02
	}	
	return -1;
		
}			
*/

/*
//主密钥分散认证(卡号)
//DisperseGene: 分散因子
short DisperseAuthenticationCardNo( short FileNo, unsigned char* DisperseGene )
{
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;						//读写出错
	
		ICComm.ICBuf2[0] = 0x80;						//主密钥分散认证	
		ICComm.ICBuf2[1] = 0xFA;
		ICComm.ICBuf2[2] = 0x00;
		ICComm.ICBuf2[3] = FileNo;
		ICComm.ICBuf2[4] = 0x08;
	
		ICDataSend( ESAM, 4, CYC5000, 1 );				//V6		
		if( ICComm.ICBuf2[0] != 0xFA ) { SaveErrorResponse( ESAM ); continue; }				//保存2字节错误响应状态 //新国网	//13.12.02
	
		RAM_Write( ICComm.ICBuf2, DisperseGene, 8 );

		ICDataSend( ESAM, 7, CYC60000, 2 );			//V6
		
		if((ICComm.ICBuf2[0] != 0x90)||(ICComm.ICBuf2[1] != 0x00)) { ICComm.ICAlarmFlg = 0x10; SaveErrorResponse( ESAM ); return -1;	}//卡号分散认证出错		//保存2字节错误响应状态 //新国网	//13.12.02
		ICComm.ICAlarmFlg = 0x0;						//密钥认证出错
		return 0;	
	}	
	return -1;
		
}			

//主密钥分散认证(随机数)
short DisperseAuthenticationRand( unsigned char* Dest, unsigned char* Source )
{
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;						//读写出错
	
		ICComm.ICBuf2[0] = 0x80;						//主密钥分散认证	
		ICComm.ICBuf2[1] = 0xFA;
		ICComm.ICBuf2[2] = 0x00;
		ICComm.ICBuf2[3] = 0x00;
		ICComm.ICBuf2[4] = 0x08;
	
		ICDataSend( ESAM, 4, CYC5000, 1 );				//V6		
		if( ICComm.ICBuf2[0] != 0xFA ) { SaveErrorResponse( ESAM ); continue; }				//保存2字节错误响应状态 //新国网	//13.12.02
	
		RAM_Write( ICComm.ICBuf2, Source, 8 );

		ICDataSend( ESAM, 7, CYC60000, 2 );			//V6
		
		if(( ICComm.ICBuf2[0] == 0x61 )&&( ICComm.ICBuf2[1] == 8 )) 
		{
			if( GetResponse( ESAM, ICComm.ICBuf2, 8 ) != 0 ) return -1;
			RAM_Write( Dest, ICComm.ICBuf2, 8 );	
			ICComm.ICAlarmFlg = 0x00;
			return 0;	
		}
		else SaveErrorResponse( ESAM );							//保存2字节错误响应状态		//新国网	//13.12.02
	}	
	return -1;
		
}			  
*/

//主密钥分散认证(卡号)
//DisperseGene: 分散因子
short DisperseAuthenticationCardNoRand( short FileNo, unsigned char* Dest, unsigned char* DisperseGene, unsigned char* Rand )
{
	unsigned char* ESAMSendBuff;

	ESAMSendBuff = FBuff.Buff;
	ICComm.ICAlarmFlg = ICALARM_28;						//读写出错

	RAM_Write( ESAMSendBuff+6, DisperseGene, 8 );
	RAM_Write( ESAMSendBuff+6+8, Rand, 8 );
	if( ReadWriteESAM( Dest, 0x80, 0x08, 0x0800+FileNo, 0x0010 ) == -1 ) { SaveErrorResponse( ESAM ); return -1; }				//保存2字节错误响应状态

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}			



//取随机数
/*
short GetChallenge( short CARD_TYPE, unsigned char* Dest, short Length )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
	if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
	else ICBuf = ICComm.ICBuf2;
	*ICBuf = 0x00;
	*(ICBuf+1) = 0x84;
	*(ICBuf+2) = 0x00;
	*(ICBuf+3) = 0x00;
	*(ICBuf+4) = Length;

	SaveOperateCommHead( CARD_TYPE );			//新国网	//13.12.02

	ICDataSend( CARD_TYPE, 4, CYC60000, Length+1 );		//V6	
	if( *ICBuf != 0x84 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态		//新国网	//13.12.02
	RAM_Write( Dest, ICBuf+1, Length );
	ICComm.ICAlarmFlg = 0x00;
	return 0;
}	
*/

short GetChallenge( short CARD_TYPE, unsigned char* Dest, unsigned short Length )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
	if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
	else
	{
		if( ReadWriteESAM( Dest, 0x80, 0x04, Length, 0x0000 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态		//新国网	//13.12.02
		ICComm.ICAlarmFlg = 0x00;
		return 0;
	}	 
	*ICBuf = 0x00;
	*(ICBuf+1) = 0x84;
	*(ICBuf+2) = 0x00;
	*(ICBuf+3) = 0x00;
	*(ICBuf+4) = Length;

	SaveOperateCommHead( CARD_TYPE );			//新国网	//13.12.02

	ICDataSend( CARD_TYPE, 4, CYC60000, Length+1 );		//V6	
	if( *ICBuf != 0x84 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态		//新国网	//13.12.02
	RAM_Write( Dest, ICBuf+1, Length );
	ICComm.ICAlarmFlg = 0x00;
	return 0;
}	


//取响应数据
short GetResponse( short CARD_TYPE, unsigned char* Dest, short Length )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) 
		{
			ICBuf = ICComm.ICBuf1;
		}	
		else 
		{
//			ICBuf = ICComm.ICBuf2;					//无ESAM操作
		}	
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xC0;
		*(ICBuf+2) = 0x00;
		*(ICBuf+3) = 0x00;
		*(ICBuf+4) = Length;

		SaveOperateCommHead( CARD_TYPE );			//新国网	//13.12.02

		ICDataSend( CARD_TYPE, 4, CYC120000, Length+3 );		//V6		
		if(( *ICBuf == 0xC0 )&&((( *(ICBuf+Length+1) == 0x90 )&&( *(ICBuf+Length+2) == 0x00 ))||( *(ICBuf+Length+1) == 0x61 )))
		{
			RAM_Write( Dest, ICBuf+1, Length );
			ICComm.ICAlarmFlg = 0x00;						//读写出错
			return 0;
		}	  			//收到响应数据！
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;
}	

//外部认证
short ExternalAuthentication( short CARD_TYPE, unsigned char* Source, short FileNo, short Length )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_10;						//密钥认证出错
	if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//	else ICBuf = ICComm.ICBuf2;							//无ESAM外部认证
	*ICBuf = 0x00;
	*(ICBuf+1) = 0x82;
	*(ICBuf+2) = 0x00;
	*(ICBuf+3) = FileNo;
	*(ICBuf+4) = Length;
	
	ICDataSend( CARD_TYPE, 4, CYC60000, 1 );		//V6
	if( *ICBuf != 0x82 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态		//新国网	//13.12.02

	RAM_Write( ICBuf, Source, Length );

	ICDataSend( CARD_TYPE, Length-1, CYC60000, 2 );		//V6		//12.12.30
	if((*ICBuf != 0x90)||(*(ICBuf+1) != 0x00)) { ICComm.ICAlarmFlg = 0x10; SaveErrorResponse( CARD_TYPE ); return -1;	}//外部认证出错		//保存2字节错误响应状态		//新国网	//13.12.02
	ICComm.ICAlarmFlg = 0x0;						//密钥认证出错
	return 0;	
}	


//对指定的数据进行加密或解密操作
//CARD_TYPE: 操作卡类型
//Dest : 操作后数据
//Source : 操作前数据
//OperateType：操作类型 0：加密、1：解密、2：进行MAC运算 
//FileNo : 采用的操作密钥文件号
//Length：请求加密或解密的数据长度
short EnDecrypt( short CARD_TYPE, unsigned char* Dest, unsigned char* Source, short OperateType ,short FileNo, short Length )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) 
		{
			ICBuf = ICComm.ICBuf1;
		}	
		else 
		{
//			ICBuf = ICComm.ICBuf2;					//无对应ESAM操作
		}	
		*ICBuf = 0x00;
		*(ICBuf+1) = 0x88;
		*(ICBuf+2) = OperateType;
		*(ICBuf+3) = FileNo;
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC60000, 1 );		//V6
		if( *ICBuf != 0x88 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态		//新国网	//13.12.02

		RAM_Write( ICBuf, Source, Length );

		ICDataSend( CARD_TYPE, Length-1, CYC60000, 2 );		//V6		//12.12.30
		if( *ICBuf == 0x61 )
		{
			if( GetResponse( CARD_TYPE, Dest, Length ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}	
	return -1;	
}

/*
//内部认证	
short InternalAuthentication( short FileNo, unsigned char* DisperseGene )
{
	unsigned char Buff[8];
	unsigned char *Ptr;
	
	Ptr = Buff;
	
	if( GetChallenge( ESAM, Ptr, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//向ESAM取随机数	//新国网	//14.01.06

	if( DisperseAuthenticationCardNo( FILE_No1, DisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//主密钥分散认证(卡号)   ESAM系统身份认证密钥的文件标识号为01	//新国网	//14.01.06
	if( DisperseAuthenticationRand( ICComm.ICBuf2, Ptr ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; } //主密钥分散认证(随机数)		//新国网	//14.01.06
	if( EnDecrypt( PSAM, ICComm.ICBuf1, Ptr, ENCRYPT , FileNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//PSAM DES加密	//新国网	//14.01.06
	if( Data_Comp( ICComm.ICBuf1, ICComm.ICBuf2, 8 ) == 0 ) return 0;	//加密数据相等，内部认证成功
	else { ICComm.ErrorMessage = ICERROR_05; ClearOperateCommHead_ErrorResponse(); return -1; }		//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.02

}
*/
//内部认证	
short InternalAuthentication( short FileNo, unsigned char* DisperseGene )
{
	unsigned char Buff[8];
	unsigned char Buff2[16];
	unsigned char *Ptr;
	unsigned char *Point;
	
	Ptr = Buff;
	Point = Buff2;
	
	if( GetChallenge( ESAM, Ptr, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//向ESAM取随机数	//新国网	//14.01.06
	if( DisperseAuthenticationCardNoRand( FILE_No1, Point, DisperseGene, Ptr ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//主密钥分散认证(卡号+随机数)   ESAM系统身份认证密钥的文件标识号为01	
	if( EnDecrypt( PSAM, ICComm.ICBuf1, Ptr, ENCRYPT , FileNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//PSAM DES加密	//新国网	//14.01.06
	if( Data_Comp( ICComm.ICBuf1, Point, 8 ) == 0 ) return 0;	//加密数据相等，内部认证成功
	else { ICComm.ErrorMessage = ICERROR_05; ClearOperateCommHead_ErrorResponse(); return -1; }		//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.02

}

/*		//取消
//ESAM外部认证
short ESAMExternalAuthentication( short FileNo, short FrameType )
{
	unsigned char Buff[16];
	unsigned char *Ptr;
	short ESAMFileNo;
	
	Ptr = Buff;
	
	if( GetChallenge( ESAM, Ptr, 8 ) != 0 ) return -1;					//向ESAM取随机数
	if( EnDecrypt( PSAM, Ptr, Ptr, ENCRYPT , FileNo, 8 ) != 0 ) return -1;	//PSAM DES加密	
	RAM_Write( Ptr+8, Para.CardNo, 8 );
	if( FrameType == 0x02 ) ESAMFileNo = FILE_No8;					//费率时段卡为参数外部认证密钥
	else ESAMFileNo = FILE_No3;	
	return ExternalAuthentication( ESAM, Ptr, ESAMFileNo, 16 );						//外部认证
}
*/

/*
//PSAM外部认证(CPU卡)
short PSAMExternalAuthentication( short FileNo, unsigned char* DisperseGene )
{
	unsigned char Buff[8];
	unsigned char *Ptr;
	
	Ptr = Buff;
	
	if( GetChallenge( PSAM, Ptr, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//向PSAM取随机数	//新国网	//14.01.06
	if( DisperseAuthenticationCardNo( FILE_No2, DisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }		//主密钥分散认证(卡号)  //ESAM中用户卡返写密钥文件标识号为02	//新国网	//14.01.06
	if( DisperseAuthenticationRand( Ptr, Ptr ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//主密钥分散认证(随机数)	//新国网	//14.01.06
	if( ExternalAuthentication( PSAM, Ptr, FileNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_06; return -1; } 	//外部认证	//新国网	//14.01.06
	return 0;
}		
*/
//PSAM外部认证(CPU卡)
short PSAMExternalAuthentication( short FileNo, unsigned char* DisperseGene )
{
	unsigned char Buff[8];
	unsigned char *Ptr;
	
	Ptr = Buff;
	
	if( GetChallenge( PSAM, Ptr, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//向PSAM取随机数	//新国网	//14.01.06
	if( DisperseAuthenticationCardNoRand( FILE_No2, Ptr, DisperseGene, Ptr ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }	//主密钥分散认证(卡号+随机数)   ESAM中用户卡返写密钥文件标识号为02	
	if( ExternalAuthentication( PSAM, Ptr, FileNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_06; return -1; } 	//外部认证	//新国网	//14.01.06
	return 0;
}		

short SelectDF( unsigned short DirFile )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		ICBuf = ICComm.ICBuf1;
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xA4;
		*(ICBuf+2) = 0x00;
		*(ICBuf+3) = 0x00;
		*(ICBuf+4) = 0x02;

		ICDataSend( PSAM, 4, CYC5000, 1 );				//V6		
		if( *ICBuf != 0xA4 ) { SaveErrorResponse( PSAM ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, (unsigned char*)&DirFile, 2 );

		ICDataSend( PSAM, 1, CYC60000, 2 );			//V6
		if( *ICBuf == 0x61 ) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( PSAM );							//保存2字节错误响应状态		//新国网	//13.12.02
	}	
	return -1;	
}

/*
short SelectDF_ESAM( unsigned short DirFile )			//新国网
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xA4;
		*(ICBuf+2) = 0x00;
		*(ICBuf+3) = 0x00;
		*(ICBuf+4) = 0x02;

		ICDataSend( ESAM, 4, CYC5000, 1 );				//V6		
		if( *ICBuf != 0xA4 ) { SaveErrorResponse( ESAM ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, (unsigned char*)&DirFile, 2 );

		ICDataSend( ESAM, 1, CYC60000, 2 );			//V6
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( ESAM );							//保存2字节错误响应状态		//新国网	//13.12.02
	}	
	return -1;	
}
*/

/*
//读二进制文件
//CARD_TYPE: 操作卡类型
//Dest : 数据目的地址
//FileNo : 二进制文件号
//StartAddr： 读取二进制文件中的起始地址
//Length：读取数据长度
short ReadBinary( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	
	short RepeatCnt=4; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) { ICBuf = ICComm.ICBuf1; }//ICPtr = &ICComm.ICPtr1; }
		else { ICBuf = ICComm.ICBuf2; }//ICPtr = &ICComm.ICPtr2; }
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;

		SaveOperateCommHead( CARD_TYPE );			//新国网	//13.12.02
	
		ICDataSend( CARD_TYPE, 4, CYC60000, Length+3 );			//V6
		if(( *ICBuf == 0xB0 )&&( *(ICBuf+Length+1) == 0x90 )&&( *(ICBuf+Length+2) == 0x00 ))
		{
			RAM_Write( Dest, ICBuf+1, Length );
			ICComm.ICAlarmFlg = 0x0;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;
}
*/

//读二进制文件
//CARD_TYPE: 操作卡类型
//Dest : 数据目的地址
//FileNo : 二进制文件号
//StartAddr： 读取二进制文件中的起始地址
//Length：读取数据长度
short ReadBinary( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	
	short RepeatCnt=4; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) { ICBuf = ICComm.ICBuf1; }
//		else { ICBuf = ICComm.ICBuf2; }
		else
		{
			ICBuf = FBuff.Buff;
			*(ICBuf+6) = 0x00; 
			ShortToSwapChar( ICBuf+7, StartAddr );
			ShortToSwapChar( ICBuf+7+2, Length );
			if( ReadWriteESAM( Dest, 0x80, 0x2C, FileNo, 0x0005 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

			ICComm.ICAlarmFlg = 0x00;
			return 0;		
		}	
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;

		SaveOperateCommHead( CARD_TYPE );			//新国网	//13.12.02
	
		ICDataSend( CARD_TYPE, 4, CYC60000, Length+3 );			//V6
		if(( *ICBuf == 0xB0 )&&( *(ICBuf+Length+1) == 0x90 )&&( *(ICBuf+Length+2) == 0x00 ))
		{
			RAM_Write( Dest, ICBuf+1, Length );
			ICComm.ICAlarmFlg = 0x0;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;
}

/*
//带MAC读二进制文件
//CARD_TYPE: 	卡类型
//SFileType:	源文件类型
//Dest:			另一卡中取得的随机数/读回数据的目的地址
//D_FileNo: 	目标文件号
//S_FileNo: 	源文件号
//D_StartAddr: 	目标起始地址
//S_StartAddr:  源起始地址
//Length: 		写数据长度
//R_Type: 		读类型：0：不带卡序列号分散，1：带卡序列号分散
//RLength:		读回数据长度，包含4字节MAC，允许读取的数据长度可能不止该长度，只取需要取的长度即可
//WLength:		后续往另一张卡（ESAM）中写数据长度，包含4字节MAC，可能含有8字节卡号
//R_Type: 		读类型：0：不带卡序列号分散，1：带卡序列号分散
//DisperseGene: 分散因子
short ReadBinaryMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short S_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = S_FileNo+0x80;
		*(ICBuf+3) = S_StartAddr;
		if( R_Type == 0 ) *(ICBuf+4) = 0x09;		//读时不需带8字节卡号
		else *(ICBuf+4) = 0x11;						//带8字节卡号
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4字节随机数
		
//		*(ICBuf+4) = 0x04;
		*(ICBuf+4) = 0x83;
//		*(ICBuf+5) = 0xD6;
		*(ICBuf+5) = 0x2A;
		*(ICBuf+6) = D_FileNo+0x80;
		*(ICBuf+7) = D_StartAddr;
		*(ICBuf+8) = WLength;

		if( R_Type == 1 )					//带卡序列号分散 
		{
			RAM_Write( ICBuf+9, DisperseGene, 8 );

			ICDataSend( CARD_TYPE, 0x10, CYC60000, 2 );		
		}
		else ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, RLength ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;	
}		
*/

//带MAC读二进制文件
//CARD_TYPE: 	卡类型
//SFileType:	源文件类型
//Dest:			另一卡中取得的随机数/读回数据的目的地址
//D_FileNo: 	目标文件号
//S_FileNo: 	源文件号
//D_StartAddr: 	目标起始地址
//S_StartAddr:  源起始地址
//Length: 		写数据长度
//R_Type: 		读类型：0：不带卡序列号分散，1：带卡序列号分散
//RLength:		读回数据长度，包含4字节MAC，允许读取的数据长度可能不止该长度，只取需要取的长度即可
//WLength:		后续往另一张卡（ESAM）中写数据长度，包含4字节MAC，可能含有8字节卡号
//R_Type: 		读类型：0：不带卡序列号分散，1：带卡序列号分散
//DisperseGene: 分散因子
short ReadBinaryMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short S_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//		else ICBuf = ICComm.ICBuf2;
		else
		{
			ICBuf = FBuff.Buff;
//			if( W_Type == 1 )
//			{
				*(ICBuf+6) = 0x48; 
				RAM_Write( ICBuf+7, DisperseGene, 8 );
				RAM_Write( ICBuf+7+8, Dest, 4 );
				ShortToSwapChar( ICBuf+7+8+4, S_StartAddr );
				ShortToSwapChar( ICBuf+7+8+4+2, RLength );
				*(ICBuf+23) = 0x04;
				*(ICBuf+24) = 0xD6;
				*(ICBuf+25) = D_FileNo+0x80;
				*(ICBuf+26) = D_StartAddr;
				*(ICBuf+27) = WLength;
//			}	
//			else RAM_Write( ICBuf+6, Source, Length );	
			if( ReadWriteESAM( Dest, 0x83, 0x2C, S_FileNo, 22 ) == -1 ) { SaveErrorResponse( ESAM ); return -1; }				//保存2字节错误响应状态

			ICComm.ICAlarmFlg = 0x00;
			return 0;		
		}	
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = S_FileNo+0x80;
		*(ICBuf+3) = S_StartAddr;
		if( R_Type == 0 ) *(ICBuf+4) = 0x09;		//读时不需带8字节卡号
		else *(ICBuf+4) = 0x11;						//带8字节卡号
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4字节随机数
		
//		*(ICBuf+4) = 0x04;
		*(ICBuf+4) = 0x83;
//		*(ICBuf+5) = 0xD6;
		*(ICBuf+5) = 0x2A;
		*(ICBuf+6) = D_FileNo+0x80;
		*(ICBuf+7) = D_StartAddr;
		*(ICBuf+8) = WLength;

		if( R_Type == 1 )					//带卡序列号分散 
		{
			RAM_Write( ICBuf+9, DisperseGene, 8 );

			ICDataSend( CARD_TYPE, 0x10, CYC60000, 2 );		
		}
		else ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, RLength ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;	
}		

/*
//带MAC读记录文件 写二进制文件
//CARD_TYPE: 	卡类型
//Dest:			另一卡中取得的随机数/读回数据的目的地址
//D_FileNo: 	目标文件号
//S_FileNo: 	源文件号(指定记录号)
//D_StartAddr: 	目标起始地址
//Length: 		写数据长度
//R_Type: 		读类型：0：不带卡序列号分散，1：带卡序列号分散
//RLength:		读回数据长度，包含4字节MAC，允许读取的数据长度可能不止该长度，只取需要取的长度即可
//WLength:		后续往另一张卡（ESAM）中写数据长度，包含4字节MAC，可能含有8字节卡号
//R_Type: 		读类型：0：不带卡序列号分散，1：带卡序列号分散
//DisperseGene: 分散因子
short ReadRecMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB2;
		*(ICBuf+2) = S_FileNo;						//指定记录号
		*(ICBuf+3) = 0x0C;							//按P1指定的记录号
		if( R_Type == 0 ) *(ICBuf+4) = 0x09;		//读时不需带8字节卡号
		else *(ICBuf+4) = 0x11;						//带8字节卡号
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB2 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4字节随机数
		
		*(ICBuf+4) = 0x04;
		*(ICBuf+5) = 0xD6;
		*(ICBuf+6) = D_FileNo+0x80;
		*(ICBuf+7) = D_StartAddr;
		*(ICBuf+8) = WLength;

		if( R_Type == 1 )					//带卡序列号分散 
		{
			RAM_Write( ICBuf+9, DisperseGene, 8 );
			ICDataSend( CARD_TYPE, 0x10, CYC60000, 2 );		
		}
		else ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, RLength ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;	
}		
*/

//带MAC读二进制文件,存入钱包文件
//CARD_TYPE: 	卡类型
//Dest:			另一卡中取得的随机数/读回数据的目的地址
//S_FileNo: 	源文件号
//S_StartAddr:  源起始地址
short ReadBinaryIncMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short S_StartAddr )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//		else ICBuf = ICComm.ICBuf2;					//无ESAM操作
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = 0x80+S_FileNo;
		*(ICBuf+3) = S_StartAddr;
		*(ICBuf+4) = 0x09;		
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4字节随机数
		
//		*(ICBuf+4) = 0x84;
//		*(ICBuf+5) = 0x32;
//		*(ICBuf+6) = 0x01;
//		*(ICBuf+7) = 0x0C;
//		*(ICBuf+8) = 0x0C;

		*(ICBuf+4) = 0x80;
		*(ICBuf+5) = 0x42;
		*(ICBuf+6) = 0x00;
		*(ICBuf+7) = 0x00;
		*(ICBuf+8) = 0x0C;

		ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, 12 ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;	
}		

/*									
//带MAC读二进制文件,写记录文件
//CARD_TYPE: 	卡类型
//Dest:			另一卡中取得的随机数/读回数据的目的地址
//S_FileNo: 	源文件号
//S_StartAddr:  源起始地址
short ReadBinaryRecMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short D_FileNo, short S_StartAddr )		//新国网	//13.11.23
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = 0x80+S_FileNo;
		*(ICBuf+3) = S_StartAddr;
		*(ICBuf+4) = 0x09;		
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4字节随机数
		
		*(ICBuf+4) = 0x04;
		*(ICBuf+5) = 0xDC;
		*(ICBuf+6) = D_FileNo;
		*(ICBuf+7) = 0x0C;
		*(ICBuf+8) = 0x08;

		ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, 8 ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;	
}		
*/
//带MAC读二进制文件,写记录文件
//CARD_TYPE: 	卡类型
//Dest:			另一卡中取得的随机数/读回数据的目的地址
//S_FileNo: 	源文件号
//S_StartAddr:  源起始地址
short ReadBinaryRecMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short D_FileNo, short S_StartAddr )		//新国网	//13.11.23
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//		else ICBuf = ICComm.ICBuf2;					//无ESAM操作
		
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xB0;
		*(ICBuf+2) = 0x80+S_FileNo;
		*(ICBuf+3) = S_StartAddr;
		*(ICBuf+4) = 0x09;		
		
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );				
		if( *ICBuf != 0xB0 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, Dest, 4 );				//4字节随机数
		
//		*(ICBuf+4) = 0x04;
//		*(ICBuf+5) = 0xDC;
//		*(ICBuf+6) = D_FileNo;
//		*(ICBuf+7) = 0x0C;
//		*(ICBuf+8) = 0x08;

		*(ICBuf+4) = 0x83;
		*(ICBuf+5) = 0x3E;
		*(ICBuf+6) = 0x00;
		*(ICBuf+7) = 0x00;
		*(ICBuf+8) = 0x08;

		ICDataSend( CARD_TYPE, 8, CYC60000, 2 );		
					
		if( *ICBuf == 0x61 ) 
		{
			if( GetResponse( CARD_TYPE, Dest, 8 ) != 0 ) return -1;
			ICComm.ICAlarmFlg = 0x00;						
			return 0;	
		}	
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;	
}		

/*
//写二进制文件
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
//FileNo : 二进制文件号
//StartAddr： 写二进制文件中的起始地址
//Length：写数据长度
short UpdateBinary( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xD6;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xD6 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, Source, Length );

		ICDataSend( CARD_TYPE, Length-1, CYC120000, 2 );	//V6		//12.12.30
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}	
	return -1;	
}
*/

//写ESAM二进制文件
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
//FileNo : 二进制文件号
//StartAddr： 写二进制文件中的起始地址
//Length：写数据长度
short UpdateBinary( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length )
{
    unsigned char Buff[10];
  unsigned char* ICBuf;

	ICBuf = FBuff.Buff;
//	RAM_Write( ICBuf+6, Source, Length );	
//	if( ReadWriteESAM( Buff, 0x83, 0x2A, 0x8000+(FileNo<<8)+StartAddr, Length ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态
	*(ICBuf+6) = 0x00;
	ShortToSwapChar( ICBuf+7, StartAddr );
	RAM_Write( ICBuf+9, Source, Length );
	if( ReadWriteESAM( Buff, 0x80, 0x2A, FileNo, Length+3 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

/*
short UpdateBinaryLA( short CARD_TYPE, unsigned char* Source, unsigned short StartAddr, short Length )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x00;
		*(ICBuf+1) = 0xD6;
		RAM_Write( ICBuf+3, (unsigned char*)&StartAddr, 2 );
		*(ICBuf+2) = *(ICBuf+4);
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xD6 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		RAM_Write( ICBuf, Source, Length );

		ICDataSend( CARD_TYPE, Length-1, CYC120000, 2 );		//V6		//12.12.30
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}	
	return -1;	
}
*/

/*
//带MAC写二进制文件
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
//FileNo : 二进制文件号
//StartAddr： 写二进制文件中的起始地址
//Length：写数据长度,传递下来时已经包含卡序列号的8字节
//W_Type: 		写类型：0：不带卡序列号分散，1：带卡序列号分散
//DisperseGene: 分散因子
short UpdateBinaryMAC( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length, short W_Type, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xD6;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xD6 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		if( W_Type == 1 )
		{
			RAM_Write( ICBuf, DisperseGene, 8 );
			RAM_Write( ICBuf+8, Source, Length-8 );
		}
		else RAM_Write( ICBuf, Source, Length );	

		ICDataSend( CARD_TYPE, Length-1, CYC240000, 2 );	//V6
		
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
		return -1; 
	}	
	return -1;	
}
*/

//带MAC写二进制文件
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
//FileNo : 二进制文件号
//StartAddr： 写二进制文件中的起始地址
//Length：写数据长度,传递下来时已经包含卡序列号的8字节
//W_Type: 		写类型：0：不带卡序列号分散，1：带卡序列号分散
//DisperseGene: 分散因子
short UpdateBinaryMAC( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length, short W_Type, unsigned char* DisperseGene )
{
	unsigned char Buff[10];
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
//		else ICBuf = ICComm.ICBuf2;
		else 
		{
			ICBuf = FBuff.Buff;
			if( W_Type == 1 )
			{
				RAM_Write( ICBuf+6, DisperseGene, 8 );
				RAM_Write( ICBuf+6+8, Source, Length - 8 );
			}	
			else RAM_Write( ICBuf+6, Source, Length );	
			if( ReadWriteESAM( Buff, 0x83, 0x2A, 0x8000+(FileNo<<8)+StartAddr, Length ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

			ICComm.ICAlarmFlg = 0x00;
			return 0;		
		}	
		*ICBuf = 0x04;
		*(ICBuf+1) = 0xD6;
		*(ICBuf+2) = FileNo+0x80;
		*(ICBuf+3) = StartAddr;
		*(ICBuf+4) = Length;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xD6 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02
		if( W_Type == 1 )
		{
			RAM_Write( ICBuf, DisperseGene, 8 );
			RAM_Write( ICBuf+8, Source, Length-8 );
		}
		else RAM_Write( ICBuf, Source, Length );	

		ICDataSend( CARD_TYPE, Length-1, CYC240000, 2 );	//V6
		
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
		return -1; 
	}	
	return -1;	
}

/*
//带MAC存钱
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
//DisperseGene: 分散因子
short UpdateBinaryIncMAC( short CARD_TYPE, unsigned char* Source, unsigned char* DisperseGene )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x84;
		*(ICBuf+1) = 0x32;
		*(ICBuf+2) = 0x01;
		*(ICBuf+3) = 0x0C;
		*(ICBuf+4) = 12;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0x32 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02

		RAM_Write( ICBuf, Source, 12 );	
		ICDataSend( CARD_TYPE, 11, CYC60000, 2 );	//V6
		if(( *ICBuf == 0x61 )&&( *(ICBuf+1) == 0x08 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
		return -1;	
	}	
	return -1;	
}
*/

//带MAC存钱
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
//DisperseGene: 分散因子
short UpdateBinaryIncMAC( short CARD_TYPE, unsigned char* Source, unsigned char* DisperseGene )
{
	unsigned char Buff[20];
	unsigned char* ESAMSendBuff;

	ESAMSendBuff = FBuff.Buff;
	ICComm.ICAlarmFlg = ICALARM_28;						//读写出错

	RAM_Write( ESAMSendBuff+6, Source, 12 );
	if( ReadWriteESAM( Buff, 0x80, 0x42, 0x0000, 0x000C ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

/*
//带MAC扣钱
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
short UpdateBinaryDecMAC( short CARD_TYPE, unsigned char* Source )				//新国网	//13.11.27
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;
		*ICBuf = 0x84;
		*(ICBuf+1) = 0x30;
		*(ICBuf+2) = 0x02;
		*(ICBuf+3) = 0x0C;
		*(ICBuf+4) = 0x08;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0x30 ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02

		RAM_Write( ICBuf, Source, 8 );	

		ICDataSend( CARD_TYPE, 7, CYC60000, 2 );	//V6
		if(( *ICBuf == 0x61 )&&( *(ICBuf+1) == 0x08 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
		return -1;	
	}	
	return -1;	
}
*/

/*
//带MAC写记录文件
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
short UpdateBinaryRecMAC( short CARD_TYPE, unsigned char* Source, short D_FileNo )
{
	unsigned char* ICBuf;
	short RepeatCnt=3; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
		else ICBuf = ICComm.ICBuf2;

		*ICBuf = 0x04;
		*(ICBuf+1) = 0xDC;
		*(ICBuf+2) = D_FileNo;
		*(ICBuf+3) = 0x0C;
		*(ICBuf+4) = 0x08;
	
		ICDataSend( CARD_TYPE, 4, CYC5000, 1 );		//V6		
		if( *ICBuf != 0xDC ) { SaveErrorResponse( CARD_TYPE ); continue; }				//保存2字节错误响应状态		//新国网	//13.12.02

		RAM_Write( ICBuf, Source, 8 );	

		ICDataSend( CARD_TYPE, 7, CYC60000, 2 );	//V6
		if(( *ICBuf == 0x90 )&&( *(ICBuf+1) == 0x00 )) { ICComm.ICAlarmFlg = 0x0; return 0; }
		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
		return -1;	
	}	
	return -1;	
}
*/
//带MAC写记录文件
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
short UpdateBinaryRecMAC( short CARD_TYPE, unsigned char* Source, short D_FileNo )
{
	unsigned char Buff[10];
	unsigned char* ICBuf;

	ICBuf = FBuff.Buff;
	ICComm.ICAlarmFlg = ICALARM_28;				//读写出错

	RAM_Write( ICBuf+6, Source, 8 );	
	if( ReadWriteESAM( Buff, 0x83, 0x3E, 0x0000, 0x0008 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

/*
//写密钥
//Source : 数据源地址
//FileNo : 密钥标识号
short UpdateKey( unsigned char* Source, short FileNo )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_16;						//读写出错
	ICBuf = ICComm.ICBuf2;
	*ICBuf = 0x84;
	*(ICBuf+1) = 0xD4;
	*(ICBuf+2) = 0x01;
	*(ICBuf+3) = FileNo;
	*(ICBuf+4) = 0x20;
	
	ICDataSend( ESAM, 4, CYC5000, 1 );			//V6
	if( *ICBuf != 0xD4 ) { SaveErrorResponse( ESAM ); return -1; }				//保存2字节错误响应状态		//新国网	//13.12.02
	RAM_Write( ICBuf, Source, 0x20 );

	ICDataSend( ESAM, (0x20-1), CYC60000, 2 );		//V6			//12.12.30
	if(( *ICBuf != 0x90 )||( *(ICBuf+1) != 0x00 )) { SaveErrorResponse( ESAM );  return -1; }		//保存2字节错误响应状态		//新国网	//13.12.02
	ICComm.ICAlarmFlg = 0x0;						//读写出错
	return 0;	
}
*/

/*
//读钱包文件
//CARD_TYPE: 操作卡类型
//Dest : 数据目的地址
short ReadRecord( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
	if( CARD_TYPE == PSAM ) ICBuf = ICComm.ICBuf1;
	else ICBuf = ICComm.ICBuf2;
	*ICBuf = 0x00;
	*(ICBuf+1) = 0xB2;
	*(ICBuf+2) = FileNo;
	*(ICBuf+3) = StartAddr;
	*(ICBuf+4) = Length;

	SaveOperateCommHead( CARD_TYPE );			//新国网	//13.12.02
	
	ICDataSend( CARD_TYPE, 4, CYC60000, Length+1 );		//V6	
	if( *ICBuf != 0xB2 ) { SaveErrorResponse( CARD_TYPE ); return -1; }		//保存2字节错误响应状态		//新国网	//13.12.02
	RAM_Write( Dest, ICBuf+1, Length );
	ICComm.ICAlarmFlg = 0x0;						//读写出错
	return 0;	
}
*/
//读ESAM钱包文件
//Dest : 数据目的地址
short ReadRecord( short CARD_TYPE, unsigned char* Dest )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
	ICBuf = FBuff.Buff;
	*(ICBuf+6) = 0x00;
	if( ReadWriteESAM( Dest, 0x80, 0x48, 0x0000, 0x0001 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

/*
//存款、扣款
//CARD_TYPE: 操作卡类型
//Source : 数据源地址
//Dest : 比对结果数据源地址
//Ins : 命令	0x32: 存款命令   0x30: 扣款命令
short InDecrease( short CARD_TYPE, unsigned char* Source, short StartAddr, unsigned char Ins )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//存扣款出错
	if( CARD_TYPE == PSAM ) 
	{
		ICBuf = ICComm.ICBuf1;
	} 
	else 
	{
	  	ICBuf = ICComm.ICBuf2;
	}  
	*ICBuf = 0x80;
	*(ICBuf+1) = Ins;
	*(ICBuf+2) = 0x00;
	*(ICBuf+3) = StartAddr;
	*(ICBuf+4) = 0x04;
	
	ICDataSend( CARD_TYPE, 4, CYC5000, 1 );			//V6
	if( *ICBuf != Ins ) { SaveErrorResponse( CARD_TYPE ); return -1; }		//保存2字节错误响应状态		//新国网	//13.12.02
	RAM_Write( ICBuf, Source, 0x04 );

	ICDataSend( CARD_TYPE, (0x04-1), CYC60000, 2 );	//V6	//12.12.30
	if(( *ICBuf == 0x61 )&&( *(ICBuf+1) == 8 )) 
	{
		if( GetResponse( CARD_TYPE, ICBuf, 8 ) != 0 ) return -1;
		ICComm.ICAlarmFlg = 0x00;
		return 0;	
	}	
	else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	return -1;	
}
*/

//明文扣款
//Source : 数据源地址
short InDecrease( short CARD_TYPE, unsigned char* Source )
{
	unsigned char Buff[20];
    unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
	ICBuf = FBuff.Buff;
	RAM_Write( ICBuf+6, Source, 0x04 );
	if( ReadWriteESAM( Buff, 0x80, 0x46, 0x0000, 0x0004 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}	

unsigned long GetDecData( unsigned char* Source )
{
	unsigned long Value1,Value2,Value3;
		
	RAM_Write( (unsigned char*)&Value1, Source, 4 );		//剩余金额
	RAM_Write( (unsigned char*)&Value2, Source+4, 4 );		//透支金额
	RAM_Write( (unsigned char*)&Value3, Source+8, 4 );		//ESAM内剩余金额

	if( Value2 == 0 ) 										
	{
		if( Value1 >= Value3 ) Value3 = 0;					//电表剩余金额>=ESAM内剩余金额  
		else
		{
			Value3 -= Value1; 
			return Value3;
		}		
	}														//已经透支，扣光	
	return Value3;
}	

short WriteESAMRunInfoReOver( unsigned char* ReDest, unsigned char* OverDest )
{
	unsigned char Buff1[5];
	unsigned char Buff2[5];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned long Value;
	
	Ptr = Buff1;
	Point = Buff2;
	
	RAM_Write( Ptr, ReDest, 4 );
	SwapData( Ptr, 4 );
	
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 23, 4 ) != 0 ) return 1;
	
	RAM_Write( (unsigned char*)&Value, OverDest, 4 );
	Long_BCD4( Point, Value );
	SwapData( Point, 4 );
	if( UpdateBinary( ESAM, Point, FILE_No7, 31, 4 ) != 0 ) return 1;
	RAM_Write( Ptr, Clk.Min, 5 );
	SwapData( Ptr, 5 );
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 42, 5 ) != 0 ) return 1;
	
	return 0;
}	


short Write_ESAMBurse( void )
{
//	unsigned char Buff[12];
	unsigned char Buff[30];
	unsigned char Buff2[10];
	unsigned char* Ptr;
	unsigned char* Point;
//	unsigned char Ins;
	unsigned long Value;
	
	Ptr = Buff;
	Point = Buff2;
	
	E2P_RFM( Ptr, FConsumECSumBCD, 8 );											//11.07.14
	if(( Flag.BatState & F_E2PCheck ) == 0 )									//11.07.14
	{																			//11.07.14	
		E2P_WData( ConsumECSumBCD, Ptr, 8 );									//11.07.14
	}																			//11.07.14
	else																		//11.07.14
	{																			//11.07.14
		if( ChkNum( ICEC.RConsumECSumBCD, 8 ) == ICEC.RConsumECSumBCDChk )		//11.07.14
		{																		//11.07.14	
			E2P_RFM( Ptr, FConsumECSumBCD, 8 );									//11.07.14
			if(( Flag.BatState & F_E2PCheck ) == 0 )							//11.07.14
			{																	//11.07.14	
				E2P_WData( ConsumECSumBCD, Ptr, 8 );							//11.07.14	
			}																	//11.07.14
			else																//11.07.14
			{																	//11.07.14
				E2P_WData( ConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );			//11.07.14
			}																	//11.07.14
		}																		//11.07.14
	}																			//11.07.14

//	RAM_Fill( Ptr, 12);
	RAM_Fill( Ptr, 30);
	
	GetReOverEC( Ptr, Ptr+4 );
	RAM_Write( Point, Ptr, 8 );

//	if( ReadRecord( ESAM, Ptr+8, FILE_No1, 0x0C, 0x04 ) != 0 ) return -1;
	if( ReadRecord( ESAM, Ptr+8 ) != 0 ) return -1;					//返回4字节剩余金额、4字节购电次数、6字节客户编号	

	SwapData( Ptr+8, 4 );														//4字节剩余金额	
	
	Value = GetDecData( Ptr );
	if( Value == 0 ) return 0;								//不需要扣钱	
	RAM_Write( Ptr, (unsigned char*)&Value, 4 );
	SwapData( Ptr, 4 );										//高低字节交换
//	Ins = 0x30;												//扣钱		
//	if( InDecrease( ESAM, Ptr, 0x0C, Ins ) != 0 ) return -1;
	if( InDecrease( ESAM, Ptr ) != 0 ) return -1;
	if( WriteESAMRunInfoReOver( Point, Point+4 ) != 0 ) return -1;
	return 0; 
}	

void InitialAbnormalInsertCardMeassage( void )			//新国网	//13.12.02
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value1,Value2;
	
	Ptr = Buff;
	
	RAM_Fill( (unsigned char*)&ICComm.ErrorMessage, 8 );
	RAM_DataFill( Para.CardNo, 8, 0xFF ); 									//新国网	//14.01.22
	E2P_RData( (unsigned char*)&Value1, BuyECCnt, 4 );
	Long_BCD4( Ptr, Value1 );				
	RAM_Write( ICComm.InsertCardECCnt, Ptr, 2 );
	GetReOverEC( (unsigned char*)&Value1, (unsigned char*)&Value2 );		//剩余电量
	Long_BCD4( ICComm.InsertCardRemainEC, Value1 );				
		
}	

//判断插卡流程完整性
void CardProceIntegrality( void )			
{
	if( ICComm.ICOperateState == 0x01 ) ICComm.ICOperateState = 0x02;
}	

short JudgeMeterVolt80Un( void )								//新国网	//13.12.03
{
	short i;
	unsigned short TempInt;
	unsigned short V80Un;						
	      
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
//	if(( PIN_SETEN & P_SETEN ) == 0 ) return 0;				//新国网	//14.07.08
	if(( HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 ) return 0;
#endif

	V80Un = MSpec.RRatedVolt;				
	V80Un *= 4;								
	V80Un /= 5;								
	
	TempInt = V80Un;
	for( i=0;i<3;i++ )
	{
#if ( LinkMode == Phase3Wire3 )
		if( i == 1 ) continue;
#endif			
		TempInt = BCD2_Word( Real.Ua+i*2 );
		if( TempInt >= V80Un ) break;			
	}		
	if( i == 3 ) return -1;			//电源异常
	else return 0;
}

void ProcessICAlarmFlg( void )		//新国网	//13.12.03
{
	if( ICComm.ErrorMessage == 0 ) ICComm.ICDisAlarmFlg = 0; 			//新国网	//13.12.20	//14.01.06
	else if(( ICComm.ErrorMessage >= ICERROR_01 )&&( ICComm.ErrorMessage <= ICERROR_03 )) { ICComm.ICAlarmFlg = ICALARM_31; ICComm.ICDisAlarmFlg = ICALARM_31; }
	else if(( ICComm.ErrorMessage >= ICERROR_04 )&&( ICComm.ErrorMessage <= ICERROR_10 )) { ICComm.ICAlarmFlg = ICALARM_32; ICComm.ICDisAlarmFlg = ICALARM_32; }
	else if(( ICComm.ErrorMessage >= ICERROR_11 )&&( ICComm.ErrorMessage <= ICERROR_13 )) { ICComm.ICAlarmFlg = ICALARM_33; ICComm.ICDisAlarmFlg = ICALARM_33; }
	else if(( ICComm.ErrorMessage >= ICERROR_14 )&&( ICComm.ErrorMessage <= ICERROR_18 )) { ICComm.ICAlarmFlg = ICALARM_34; ICComm.ICDisAlarmFlg = ICALARM_34; }
 	else if(( ICComm.ErrorMessage >= ICERROR_19 )&&( ICComm.ErrorMessage <= ICERROR_20 )) { ICComm.ICAlarmFlg = ICALARM_35; ICComm.ICDisAlarmFlg = ICALARM_35; }
	else if( ICComm.ErrorMessage == ICERROR_21 ) { ICComm.ICAlarmFlg = ICALARM_36; ICComm.ICDisAlarmFlg = ICALARM_36; }
}	

							//remote
void ICProce( void )
{
	unsigned char FrameType;
	unsigned char InterFileNo;
	
	InitialAbnormalInsertCardMeassage();			//新国网	//13.12.02
	
	ICComm.CardType = 0;
	if( Card_CoolRST( PSAM ) != 0 )
	{
		if( Card_CoolRST( PSAM ) != 0 ) 
		{ 
			ICComm.ICAlarmFlg = ICALARM_23; 
			Recd_ADD1( ILInsert_Cunt, 3 ); 			//新国网	//13.12.10
			RAM_DataFill( Para.CardNo, 8, 0xFF ); 	//新国网	//13.12.10
			ICComm.ErrorMessage = ICERROR_04; 		//新国网	//13.12.03
			return;  
		}		
	}
	ICEC.IdentityValidDelay = 0;					//新国网	//13.12.11
	ICEC.IdentityValidDelayChk = 0xA5;				//新国网	//13.12.11
	if( Card_CoolRST( ESAM ) != 0 ) 	//保持ESAM处于长期工作状态
	{ 
		ICComm.ICAlarmFlg = ICALARM_11; 
		Flag.MeterState1 |= F_ESAMErr;	//电表运行状态1
		ICComm.ErrorMessage = ICERROR_03; 		//新国网	//13.12.02
		return;  		
	}
	else Flag.MeterState1 &= ~F_ESAMErr;
	
	if( SelectDF( 0x01DF) != 0 ) { CardProceIntegrality(); return; }
	if( ReadBinary( PSAM, ICComm.ICBuf1, 0x01, 0x0, 4 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_28; CardProceIntegrality(); return;  }
	if( ICComm.ICBuf1[0] != 0x68) { ICComm.ICAlarmFlg = ICALARM_28; ICComm.ErrorMessage = ICERROR_14; CardProceIntegrality(); return;  }	//新国网	//13.12.03
	FrameType = ICComm.ICBuf1[1];
	InterFileNo = FILE_No1;						//系统身份认证密钥文件标识均为01
		if( InternalAuthentication(InterFileNo, Para.CardNo ) != 0 ) 
		{
			if(( FrameType == CARD_PREPARE )&&( ICComm.ErrorMessage == ICERROR_05 ))		//新国网	//14.01.02	//14.01.06
			{
//				if( GetTrueBitSum_32Bit( Flag.SecretKeyState, Para.RSecretKeySum ) != 0  )		//所有密钥不全部为测试密钥
				if( ICComm.RPrivateKeyFlag == 0x00 )		//私钥			
				{
					ICComm.ErrorMessage = ICERROR_09;
				}	
			}	
			
			ICComm.ICAlarmFlg = ICALARM_10;
			if( ICComm.ErrorMessage == ICERROR_05 ) Recd_ADD1( ILInsert_Cunt, 3 );		//新国网	//14.01.06
			CardProceIntegrality(); 
			return;
		}	
	if( Card_Purview( FrameType ) != 0 ) { CardProceIntegrality(); return; }			//新国网		//13.12.05	
	if( GetFullFrame( FrameType ) != 0 ) { CardProceIntegrality(); return; }
	ICEC.DecDelay = 15;
	if( Write_ESAMBurse() != 0 ) { ICComm.ErrorMessage = ICERROR_02; ICComm.ICAlarmFlg = ICALARM_28; CardProceIntegrality(); return; }		//扣ESAM内的剩余金额
	ICComm_Ctrl( FrameType );
	CardProceIntegrality(); 				//插卡流程正常结束判断
}		

							//remote
short GetFullFrame( short FrameType )
{
	unsigned char Buff[100];			//新国网		//13.12.12
	unsigned char *Ptr;					//新国网		//13.12.12
	short FrameNo;
	short Len;
	short FileNo;								
	
	Ptr = Buff; 						//新国网		//13.12.12
	
	FrameNo = FrameType;
	if(( FrameNo != 1 )&&( FrameNo != 6 )) { ICComm.ICAlarmFlg = ICALARM_26; ICComm.ErrorMessage = ICERROR_08; return -1;  }	//有效卡的命令码范围为1、9		//新国网		//13.12.03
	if( FrameNo == 1 ) FrameNo = 0;	//查表用		//新国网		//13.09.06
	else FrameNo = 1;					//查表用		//新国网		//13.09.06
																				
	ICComm.CardType = FrameType;
	Len = FramePara[FrameNo].Len;

	if( ReadBinary( PSAM, ICComm.ICBuf1, FILE_No1, 0x00, Len ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }	//新国网	//14.01.06

	SwapFrameData( ICComm.ICBuf1+0x04, FramePara[FrameNo].TabPtr, FramePara[FrameNo].ItemCount );	//新国网总共两张卡，都要倒数据		//新国网		//13.09.06

	if(( ICComm.ICBuf1[0] != 0x68 )||( ICComm.ICBuf1[3]!= (Len-6 ))		//帧格式是否相符		//长度占用2字节
		||( ICComm.ICBuf1[Len-1] != 0x16 )||( ICComm.ICBuf1[ Len-2] != ChkNumAdd( ICComm.ICBuf1+1,Len-3 ))) 
		{ 
			ICComm.ErrorMessage = ICERROR_14;							//新国网		//13.12.03
			ICComm.ICAlarmFlg = ICALARM_25; 
			return -1;	
		}
	if( FrameType == CARD_BUYEC )		//用户卡需要电表外部认证		//新国网		//13.12.05
	{														
		FileNo = FILE_No2;				//用户卡返写密钥文件标识为02	
		RAM_Write( Ptr, ICComm.ICBuf1, Len );							//新国网		//13.12.12
		if( PSAMExternalAuthentication( FileNo, Para.CardNo ) != 0 ) { return -1; }		//返写前对卡进行外部认证	//V8	//新国网		//13.12.03	//14.01.06
		RAM_Write( ICComm.ICBuf1, Ptr, Len );							//新国网		//13.12.12
	}										
		
	return 0;	
}

short Card_Purview( short FrameType )						//新国网		//13.12.03
{
//	unsigned char Sum; 
	
//	Sum = GetTrueBitSum_32Bit( Flag.SecretKeyState, Para.RSecretKeySum );	//取私钥数
//	if( Sum != 0  )										//所有密钥不全部为测试密钥
	if( ICComm.RPrivateKeyFlag == 0x00 )		//私钥			
	{
		if( FrameType == CARD_PREPARE )		
		{ 
			ICComm.ErrorMessage = ICERROR_09;
			return -1; 
		}
	}	
//	if( Sum != Para.RSecretKeySum  )					//所有密钥不是全部为正式密钥，不允许插购电卡	
	if( ICComm.RPrivateKeyFlag == 0x55 )		//公钥			
	{
		if( FrameType == CARD_BUYEC )		
		{ 
			ICComm.ErrorMessage = ICERROR_09;
			return -1; 
		}
	}	
	
	return 0;
}	
							//remote
void ICComm_Ctrl( short FrameType )
{
	switch( FrameType )
	{
		case CARD_BUYEC:	//电表购电信息文件
			IC_BuyEC();		//购电卡
			break;
		case CARD_PREPARE:	//预置卡指令信息文件
			IC_Prepare();	//预置卡
			break;
		default: break;
	}	
}	

						//remote
void GetECState( void )
{
	unsigned char Buff[8];
	unsigned char* Ptr;	
	
	Ptr = Buff;
	
	GetReOverEC( Ptr, Ptr+4 );
	GetICECState( Ptr, Ptr+4 );
}

void GetConsumECSum( unsigned char* Dest )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	if( ChkNum( ICEC.RConsumECSumBCD, 8 ) == ICEC.RConsumECSumBCDChk )
	{
		RAM_Write( Dest, ICEC.RConsumECSumBCD, 8 );
	}	
	else
	{
		E2P_RFM( Ptr, FConsumECSumBCD, 8 );
		RAM_Write( Dest, Ptr, 8 );														//11.07.14
		if(( Flag.BatState & F_E2PCheck ) == 0 )
		{
			RAM_Write( Dest, Ptr, 8 );
			RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
			ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
		}
		else
		{	
			E2P_RData( Ptr, ConsumECSumBCD, 8 );
			if(( Flag.BatState & F_E2PCheck ) == 0 )
			{
				E2P_RFM( Dest, FConsumECSumBCD, 8 );									//11.07.14
				if(( Flag.BatState & F_E2PCheck ) == 0 )								//11.07.14	
				{																		//11.07.14	
					RAM_Write( ICEC.RConsumECSumBCD, Dest, 8 );							//11.07.14		
					ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );		//11.07.14	
				}																		//11.07.14		
				else																	//11.07.14	
				{																		//11.07.14
					RAM_Write( Dest, Ptr, 8 );
					RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
					ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
					E2P_WFM( FConsumECSumBCD, Ptr, 8 );
				}																		//11.07.14
			}
		}
	}		
}

//取剩余和透支电量
//ReDest : 剩余电量目标地址		HEX
//OverDest : 透支电量目标地址	HEX
void GetReOverEC( unsigned char* ReDest, unsigned char* OverDest )
{
	unsigned long Temp;
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned long long LValue1,LValue2;
	
	Ptr = Buff;
	
	GetConsumECSum( Ptr+10 );
	RAM_Write( (unsigned char*)&LValue2, Ptr+10, 8 );	
	E2P_RData( Ptr, BuyECSumBCD, 8 );
	RAM_Write( (unsigned char*)&LValue1, Ptr, 8 );	

	if( LValue1 >= LValue2 )
	{
		RAM_Fill( OverDest, 4 );
		LValue1 -= LValue2;	
		Temp = LValue1 / 10000;						
		RAM_Write( ReDest, (unsigned char*)&Temp, 4 );		//十六进制的剩余金额 	XXXXXX.XX元
		ICEC.RRemainEC = Temp;				//剩余金额
		ICEC.ROverdraftEC = 0;              //透支金额
	}
	else
	{
		RAM_Fill( ReDest, 4 );
		LValue2 -= LValue1;	
		Temp = LValue2 / 10000;		
		RAM_Write( OverDest, (unsigned char*)&Temp, 4 );	//十六进制的透支金额 	XXXXXX.XX元
		ICEC.RRemainEC = 0;					//剩余金额
		ICEC.ROverdraftEC = Temp;           //透支金额
	}		
}	

void KeyRelayOn( void )									//11.06.27
{
	ICComm.ICInState |= ( F_BuyEC + F_BuyECOver );		//11.08.19		
	SaveBuyECState();									//11.08.19
	GetECState();										//11.08.19
	if( ICComm.RelayONState == 0xAA ) ICComm.RelayONState = 0x55;	//在允许合闸状态下，插卡直接合闸

}

//退出预跳闸状态
void ExitPreTripState( void )													//11.08.19
{																				//11.08.19
	if( Para.RPreTripState == 0x55 ) 											//11.08.19	
	{																			//11.08.19
		Para.RPreTripState = 0x00;												//11.08.19
		E2P_WData( EPreTripState, (unsigned char*)&Para.RPreTripState,1 );		//11.08.19
	}																			//11.08.19	
}

//进入预跳闸状态
void EnterPreTripState( void )													//11.08.19
{																				//11.08.19
	if( Para.RPreTripState != 0x55 ) 											//11.08.19	
	{																			//11.08.19
		Para.RPreTripState = 0x55;												//11.08.19
		E2P_WData( EPreTripState, (unsigned char*)&Para.RPreTripState,1 );		//11.08.19
	}																			//11.08.19	
}

//置位需判合闸允许金额标志
void SetJudgePermitFlag( void )										//新国网	//13.12.10			
{
	if( Para.RMeterSafeState == 0x3A ) 				//保电			//新国网	//14.01.09		
	{
		ClearJudgePermitFlag();										//新国网	//13.12.10			
		return;		
	}	
	if(( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed )||( ICComm.ICECState == F_ECEnd ))	//新国网	//14.01.09
	{	
		if( ICComm.RJudgePermitFlag != 0x55 )
		{
			ICComm.RJudgePermitFlag = 0x55;
			E2P_WData( JudgePermitFlag, (unsigned char*)&ICComm.RJudgePermitFlag,1 );				
		}
	}			
}	

//清除需判合闸允许金额标志
void ClearJudgePermitFlag( void )									//新国网	//13.12.10			
{
	if( ICComm.RJudgePermitFlag != 0x00 )
	{
		ICComm.RJudgePermitFlag = 0x00;
		E2P_WData( JudgePermitFlag, (unsigned char*)&ICComm.RJudgePermitFlag,1 );				
	}		
}	

void SaveBuyECState( void )					//参照天津	//11.06.27
{
	unsigned char Temp;
	unsigned char Temp2;
	
	E2P_RData( (unsigned char*)&Temp, EICInState, 1 );
	
	Temp &= (F_BuyEC + F_BuyECOver);
	Temp2 = ICComm.ICInState;
	Temp2 &= (F_BuyEC + F_BuyECOver);
	
	if( Temp != Temp2 )
	{
		E2P_WData( EICInState, (unsigned char*)&ICComm.ICInState, 1 );
	}	
}


//ReDest: 剩余电量
//OverDest: 透支电量	
//F_ECNormal	0x00	//剩余电量大于等于报警电量1；
//F_ECAlarm1	0x01	//剩余电量大于等于报警电量2，小于报警电量1；
//F_ECAlarm20	0x02	//剩余电量大于0，小于报警电量2，还没有插过用户卡(跳闸报警)；
//F_ECAlarm21	0x03	//剩余电量大于0，小于报警电量2，插过用户卡(恢复用电)；
//F_ECOver		0x04	//剩余电量等于0，透支电量等于0；是否跳闸根据透支电量限额决定。
//F_ECOverdraft	0x05	//透支；
//F_ECOverEnd	0x06	//透支电量用完；		//V7
//F_ECExceed	0x07	//超过透支电费限额；	//参照天津	//11.06.27
//F_ECEnd		0x08	//剩余电量等于0，透支电量等于0；透支电量限额等于0。			//11.08.19
//F_NULL		0x55	//空(用于继电器卡)		//V7

							//remote
void GetICECState( unsigned char* ReDest, unsigned char* OverDest )
{
	unsigned long RemainEC,Alarm1,Alarm2;
	unsigned char Buff[8];
	unsigned char* Ptr;
	unsigned char RelayState=0;
	unsigned char OldICECState;							//新国网	//14.01.07	
	Ptr = Buff;
	
	ICComm.Power &= ~F_FmqbjEn;					//V8				//蜂鸣器使能
	OldICECState = ICComm.ICECState;					//新国网	//14.01.07	
	RAM_Write( (unsigned char*)&RemainEC, ReDest, 4 );	//剩余电量
	RAM_Write( (unsigned char*)&Alarm1, OverDest, 4 );	//透支电量
	if( RemainEC == 0 )
	{
		if( Alarm1 == 0 ) ICComm.ICECState = F_ECOver;
		else ICComm.ICECState = F_ECOverdraft;
		E2P_RData( Ptr, OverdraftECLimt, 4 );
		Alarm2 = BCD4_Long( Ptr );		//透支电量限额	
		if( Alarm2 > Alarm1 )
		{
			if(( ICComm.ICInState & F_BuyECOver ) == 0 ) 	//新国网	//13.12.07
			{
				RelayState = 1;	//跳闸			//V7	//透支后是否插过用户卡	//参照天津	//11.06.27
				if(( ICComm.RJudgePermitFlag == 0x55 )&&( ICEC.RRemainEC <= ICEC.RPermitTripONLimt )) ;		//新国网	//14.01.09
				else if( Para.RMeterSafeState == 0x3A )	;									//保电			//新国网	//14.01.14
				else if(( ICComm.TripOFFDelay == 0 )&&( Para.RMeterTripState == 0x1A ))	;	//远程跳闸		//新国网	//14.01.14
				else																						//新国网	//14.01.09
				{
					EnterPreTripState();						//13.12.07
					ICComm.RelayONState = 0xAA;			//13.12.07		
				}	
			}	
			else
			{												//V8
				RelayState = 0;	//不跳闸
			}												//V8	
		}	
		else 
		{
			if( ICComm.ICECState != F_ECOver ) ICComm.ICECState = F_ECOverEnd;		//V7
			else ICComm.ICECState = F_ECEnd;		//剩余电量等于0，透支电量等于0；透支电量限额等于0。			//11.08.19
			RelayState = 1;	//跳闸			
			ICComm.ICInState &= ~( F_BuyECOver+ F_CheckECOver );		//参照天津	//11.06.27
			if( Alarm1 > Alarm2 ) ICComm.ICECState = F_ECExceed;		//超过透支限额，提示ERR-01		

			ICComm.RelayONState = 0x00;							//13.12.07
			ExitPreTripState();									//11.08.19
		}	
		
		if( OldICECState != 0xff )								//上电RAM乱不置主动上报透支状态		//新国网 	//14.01.07
		{
			if(( OldICECState != F_ECOverdraft )&&( OldICECState != F_ECOverEnd )&&( OldICECState != F_ECExceed ))
			{
				if(( ICComm.ICECState == F_ECOverdraft )||( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed ))
				{
					UpdateEventUpState( 9 );	
				}	
			}				
		}			
	}	
	else
	{
		ICComm.ICInState &= ~( F_BuyECOver+ F_CheckECOver );			//V7
		E2P_RData( Ptr, AlarmEC1, 4 );
		Alarm1 = BCD4_Long( Ptr );
		E2P_RData( Ptr, AlarmEC2, 4 );
		Alarm2 = BCD4_Long( Ptr );

		if( RemainEC > Alarm1 ) { ICComm.ICECState = F_ECNormal; ICComm.ICInState &= ~F_BuyEC; }	//参照天津	//11.06.27
		if(( RemainEC > Alarm2 )&&(RemainEC <= Alarm1)) ICComm.ICECState = F_ECAlarm1;
		if(( RemainEC > 0 )&&(RemainEC <= Alarm2))
		{
			if(( ICComm.ICInState & F_BuyEC ) == 0 ) { ICComm.ICECState = F_ECAlarm20; RelayState = 1; }	//跳闸 
			else { ICComm.ICECState = F_ECAlarm21; RelayState = 0; ICComm.Power |= F_FmqbjEn; }	//不跳闸		//V8	
		}
		if( RemainEC > Alarm2) 
		{
			ICComm.ICInState &= ~(F_BuyEC+F_CheckEC);		//V7
		}	
	}	
	
	SaveBuyECState();				//参照天津	//11.06.27

	SetJudgePermitFlag();										//置位需判合闸允许金额标志	//新国网	//13.12.10	//14.01.10
	
	if( ICComm.ICECState != F_ECAlarm21 ) ICComm.ICInState &= ~F_Alarm2RelayONPermit;		//11.04.23

	if( ICComm.RelayTestDelay == F_NULL )
	{
		if( Para.RMeterSafeState == 0x3A )									//保电
		{
#if ( RelayONImmediately == YesCheck )
			if(( Para.RMeterRelayState == 0x1A )&&(ICComm.RelayONState != 0x55)) ICComm.RelayONState = 0x55;	//立即合闸
#else
			if(( Para.RMeterRelayState == 0x1A )&&(ICComm.RelayONState != 0x55)) ICComm.RelayONState = 0xAA;	//允许合闸
#endif
			if( Para.RMeterRelayState != 0x1A ) ICComm.RelayONState = 0x00;//新国网	//13.01.09
			ExitPreTripState();									//11.08.19
			return;															//保电状态，不根据剩余金额进行跳合闸	
		}	
		if(( ICComm.TripOFFDelay == 0 )&&( Para.RMeterTripState == 0x1A ))	//远程跳闸
		{			
			if( Para.RMeterRelayState != 0x1A ) ICComm.RelayOFFState = 0x55;//准备跳闸																											
			ICComm.RelayONState = 0x00;
			ExitPreTripState();									//11.08.19
			return;															//远程跳闸状态，不根据剩余金额进行跳合闸
		}	
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		if(( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E ))	//远程预跳闸状态下
		{
			if( RelayState == 1 )											//要进入本地预跳闸或跳闸
			{
				ICComm.PreTripOFFKeepTime = 0;								//退出远程预跳闸
				Para.RMeterTripState = 0x1B;			//允许合闸			//退出远程预跳闸
				E2P_WData( MeterTripState, &Para.RMeterTripState, 1 );
				
				if( Para.RMeterRelayState == 0x1A )							//若已经被远程预跳闸拉到跳闸状态则维持跳闸状态	
				{
					if(( ICComm.ICECState == F_ECEnd )||( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed )) 	
					{																											 				
						ExitPreTripState();									//注明目前非本地预跳闸状态														
					}																												
					else EnterPreTripState();								//注明目前进入本地预跳闸状态，以利于本地合闸				
				}
			}	
			else															//远程预跳闸状态下本地没有预跳闸或跳闸，则执行远程预跳闸 
			{
				if( Para.RMeterRelayState != 0x1A ) ICComm.RelayOFFState = 0x55;//准备跳闸																											
				ICComm.RelayONState = 0x00;
				return;
			}	
		}	
#endif
		if(( RelayState == 1 )&&( Para.RMeterRelayState != 0x1A ))			//当前合闸状态需要跳闸
		{
			if(( ICComm.ICECState == F_ECEnd )||( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed )) 	//11.08.19	
			{																											 	//11.08.19				
				ExitPreTripState();																							//11.08.19	
			}																												//11.08.19	
			else EnterPreTripState();																						//11.08.19	

			ICComm.RelayOFFState = 0x55;									//准备跳闸
			ICComm.RelayONState = 0x00;
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			if( IsAllData( Comm.TripOFFOperateCode, 4, 0xFF ) != 0 )		//新国网	//14.05.12
			{
				RAM_DataFill( Comm.TripOFFOperateCode, 4, 0xFF );			//自动跳闸			
				E2P_WData( ETripOFFOperateCode, Comm.TripOFFOperateCode, 4 );//新国网	//14.05.12
			}	
#else
			RAM_DataFill( Comm.TripOFFOperateCode, 4, 0xFF );				//自动跳闸			
#endif
		}	
#if ( IOver30ALimitTrip	== YesCheck )		//新国网	//14.04.25
		else if( RelayState == 0 )
		{
			ICComm.RelayOFFState = 0x0;				//13.02.28				//避免充值回正常后还继续执行电流过载时的拉闸条件	//14.09.10			
			if( Para.RMeterRelayState == 0x1A )		//继电器状态需要变更  	//V7
			{ 
#if ( RelayONImmediately == YesCheck )
				ICComm.RelayONState = 0x55;										//立即合闸
#else
				ICComm.RelayONState = 0xAA;										//允许合闸
#endif
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
#else       	
				RAM_DataFill( Comm.TripONOperateCode, 4, 0xFF );				//自动合闸操作代码													
#endif
			}
		}	
#else
		else if(( RelayState == 0 )&&( Para.RMeterRelayState == 0x1A ))		//继电器状态需要变更  //V7
		{
#if ( RelayONImmediately == YesCheck )
			ICComm.RelayONState = 0x55;										//立即合闸
#else
			ICComm.RelayONState = 0xAA;										//允许合闸
#endif
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
#else
			RAM_DataFill( Comm.TripONOperateCode, 4, 0xFF );				//自动合闸操作代码									
#endif
		}	
#endif
	}	
}
	
void OperateRelay( void )
{
#if ( CarryComm == YesCheck )
	if((( SM.CarrySTADelay == 0 )||( SM.CarrySTADelay >= 6 ))&&((( Para.RPreTripState == 0x55 )||( ICComm.RJudgePermitFlag == 0x00 )||(( ICComm.RJudgePermitFlag == 0x55 )&&( ICEC.RRemainEC > ICEC.RPermitTripONLimt ))||(Para.RMeterSafeState == 0x3A))&&( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 )))	//执行合闸			//新国网	//13.12.10
#else
	if((( Para.RPreTripState == 0x55 )||( ICComm.RJudgePermitFlag == 0x00 )||(( ICComm.RJudgePermitFlag == 0x55 )&&( ICEC.RRemainEC > ICEC.RPermitTripONLimt ))||(Para.RMeterSafeState == 0x3A))&&( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState == 0x55 ))	//执行合闸			//新国网	//13.12.10
#endif
	{
		RecordStartEvent( TripON_No );
		ExitPreTripState();									//11.08.19
		ClearJudgePermitFlag();	//清除判合闸允许金额标志	//新国网	//13.12.10

		ICComm.ICInState |= ( F_BuyEC + F_BuyECOver );		//11.08.19		
		SaveBuyECState();									//11.08.19

		ICComm.ICInState &= ~F_Alarm2RelayONPermit;		//11.04.23
		ICComm.RelayDriveCnt = 5;	
		RelayDrive( RELAY_ON );	
		ICComm.RelayONState = 0x0;
		ICComm.RelayOFFState = 0x00;			//合闸后，取消允许拉闸状态	//新国网	//13.10.25
		Para.RMeterRelayState = 0x55;			//已经合闸
		E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
//		CreateTripRec( TripON_No );	 		
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		EVar.RelayFaultVar.Cunt = 1;														//新国网	//14.05.03	
		EVar.RelayFaultVar.OldState = 0xff;													//新国网	//14.05.03
		E2P_WData( RelayFault_OldState, (unsigned char*)&EVar.RelayFaultVar.OldState, 1 );	//新国网	//14.05.12	
		Flag.RSwitchOldState &= 0x04;														//新国网	//14.05.03	
		Flag.RSwitchOldState |= 0x40;			//允许记录异常事件							//新国网	//14.05.12				
		E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//新国网	//14.05.03	
#endif
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
//15.08.05
		if( IsAllData( Comm.TripONOperateCode, 4, 0xFF ) != 0 )			//新国网	//14.05.12
		{
			RAM_DataFill( Comm.TripONOperateCode, 4, 0xFF );			//自动跳闸			
			E2P_WData( ETripONOperateCode, Comm.TripONOperateCode, 4 );	//新国网	//14.05.12
		}	
#endif
		RecordEndEvent( TripON_No, 0 );
	}				

#if ( CarryComm == YesCheck )
	if((( SM.CarrySTADelay == 0 )||( SM.CarrySTADelay >= 6 ))&&(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 )))	//执行跳闸		//新国网	//13.10.25
#else
	if(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))	//执行跳闸		
#endif
	{
#if ( IOver30ALimitTrip	== YesCheck )		//新国网	//14.04.25
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		if(( IOver30AJudge() == 0 )||( SM.Up30ATimeCount >= Para.RUp30AClose24HOUR ))			//新国网	//14.06.19
#else
		if(( IOver30AJudge() == 0 )||( SM.Up30ATimeCnt == UP30A24HOUR ))
#endif
		{
#endif
			RecordStartEvent( TripOFF_No );
			ICComm.RelayDriveCnt = 5;				//改为5秒后再操作一次	
			RelayDrive( RELAY_OFF );	
			ICComm.RelayOFFState = 0x0;
			ICComm.RelayONState = 0x00;				//拉闸后，取消允许合闸状态	//新国网	//13.10.25
			Para.RMeterRelayState = 0x1A;			//已经跳闸
			E2P_WData( MeterRelayState, &Para.RMeterRelayState, 1 );		
//			CreateTripRec( TripOFF_No );	 		
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			EVar.RelayFaultVar.Cunt = 1;														//新国网	//14.05.03
			EVar.RelayFaultVar.OldState = 0xff;													//新国网	//14.05.03	
			E2P_WData( RelayFault_OldState, (unsigned char*)&EVar.RelayFaultVar.OldState, 1 );	//新国网	//14.05.12	
			Flag.RSwitchOldState &= 0x04;														//新国网	//14.05.03	
			Flag.RSwitchOldState |= 0x40;			//允许记录异常事件							//新国网	//14.05.12							
			E2P_WData( ESwitchOldState, (unsigned char*)&Flag.RSwitchOldState, 1 );				//新国网	//14.05.03	
#endif
			RecordEndEvent( TripOFF_No, 0 );
#if ( IOver30ALimitTrip	== YesCheck )		//11.06.29
		}
#endif
	}				
}		
	
void CreatICPrgRec( unsigned char CardType )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	
	Ptr = Buff;	
	
	*Ptr = ICComm.ParaUpdateFlag; 
	*(Ptr+1) = CardType;		
	*(Ptr+2) = 0x98;		
	*(Ptr+3) = 0x99;			
//	CreatPrgRec( Ptr, PARAM, Para.CardNo, 2 );	
	EndPrgRecEvent( 0 );
	CreatPrgRecEvent( Ptr );

}

void Clr_AccountFlag( void )										//新国网	//13.12.05	
{
	ICComm.ROpenAccountFlag = 0x03;									
	E2P_WData( OpenAccountFlag, (unsigned char*)&ICComm.ROpenAccountFlag, 1 );			//清开户标记
}

void Clr_ReOverEC( void )
{
	Clr_RemainEC();
	Clr_E2PData( OverdraftEC, 4, 1 );
}	
	
void Clr_RemainEC( void )
{	
	RAM_Fill( ICEC.RConsumECSumBCD, 8 );
	ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
	E2P_WFM( FConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );
	
	Clr_E2PData( BuyECSumBCD, 8, 2 );
	Clr_E2PData( CurrentBuyEC, 4, 1 );
}



void IC_Prepare( void )		//预置卡					//新国网		//13.09.10
{
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char RandomData[8];						//新国网	//13.11.23				
	unsigned char Para1Data[20];						//新国网	//13.11.23					
	unsigned char Para2Data[20];						//新国网	//13.11.23				
	unsigned char CPrice1Data[80];						//新国网	//13.11.23				
	unsigned char CPrice2Data[80];						//新国网	//13.11.23					
	unsigned char BPrice1Data[80];						//新国网	//13.11.23				
	unsigned char BPrice2Data[80];						//新国网	//13.11.23							
	unsigned char CStepData[80];						//新国网	//13.11.23				
	unsigned char BStepData[80];						//新国网	//13.11.23					

	unsigned char Buff[60];								//新国网	//13.11.23						
	unsigned char Buff2[10];
	unsigned char ParaUpdateFlag;
	short i;
    
	Point = Buff2;
	
	ParaUpdateFlag = ICComm.ICBuf1[5];
	ICComm.ParaUpdateFlag = ParaUpdateFlag;

//	if( ReadBinary( PSAM, Point, FILE_No5, 0x04, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; ICComm.ICAlarmFlg = ICALARM_28; return; }		//读PSAM钱包文件内的购电次数	//新国网	//14.01.06
//	if( IsAllData( Point, 4, 0x00 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_34; ICComm.ErrorMessage = ICERROR_17; ClearOperateCommHead_ErrorResponse(); return; }		//读卡文件内的购电次数不等于0			//新国网	//13.11.23	//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.02

	ICComm.RelayTestDelay = F_NULL; 	

	Ptr = Buff;

	Clr_AccountFlag();
	Clr_ReOverEC();

	RAM_Fill( Ptr, 8);
	E2P_WData( BfERemainEC, Ptr, 4 );																//初始化前剩余电量为0	//新国网	//13.11.26

	if( GetChallenge( ESAM, RandomData, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//新国网	//14.01.06
	RAM_Write( Ptr, RandomData, 4 );
//	if( ReadBinaryRecMAC( PSAM, Ptr, 0x05, 0x01, 0x00 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//读钱包文件余额		//新国网	//13.11.23	//14.01.06
	if( ReadBinaryRecMAC( PSAM, Ptr, 0x02, 0x01, 0x00 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//读钱包文件余额		//新国网	//13.11.23	//14.01.06
	if( UpdateBinaryRecMAC( ESAM, Ptr, 0x01 ) != 0 ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 );  return; }	//写钱包文件余额		//新国网	//13.12.10

	AddBuyECSumBCD( Ptr );						 //累加累计购电金额		

//预置卡的购电次数不用设？缺省为0？
//	RAM_Write( Ptr, RandomData, 4 );
//	if( ReadBinaryRecMAC( PSAM, Ptr, 0x05, 0x03, 0x04 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//读购电次数			//新国网	//13.11.23	//14.01.06
//	if( UpdateBinaryRecMAC( ESAM, Ptr, 0x03 ) != 0 ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return; }		//写购电次数			//新国网	//13.12.10

//	SwapData( Ptr, 4 );																				
	RAM_Fill( Ptr, 4);							//预置卡购电次数为0
	E2P_WData( BuyECCnt, Ptr, 4 );																	
			
	RAM_Fill( Ptr, 8);
	E2P_WData( UserCardNo, Ptr, 8 ); 						//清卡序列号
	E2P_WData( ILInsert_Cunt, Ptr, 3 );												//新国网	//13.11.26				
	E2P_WData( BuyECFlag, Ptr, 1 );								//购电成功			//新国网	//13.11.26		
	ClearJudgePermitFlag();					//清除判合闸允许金额标志	//新国网	//13.12.10

	Para.RMeterTripState = 0x1B;													//新国网	//13.11.26		
	E2P_WData( MeterTripState, (unsigned char*)&Para.RMeterTripState, 1 );			//新国网	//13.11.26		

	Para.RMeterAlarmState = 0x2B;													//新国网	//13.11.26				
	E2P_WData( MeterAlarmState, (unsigned char*)&Para.RMeterAlarmState, 1 );		//新国网	//13.11.26		

	RAM_Fill( ICEC.LocalDisperseGene, 8);											//新国网	//13.11.26		
//	E2P_RData( ICEC.LocalDisperseGene, EUser_No, 6 );								//新国网	//13.11.26		
	E2P_RData( ICEC.LocalDisperseGene, EMeter_No, 8 );								//新国网	//13.11.26		
	SwapData( ICEC.LocalDisperseGene, 8 );											//新国网	//13.11.26		

	if(( ParaUpdateFlag & 0x80 ) != 0 )				//更新其他参数					//新国网	//13.11.23				
	{
		RAM_Write( Para1Data, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, Para1Data, FILE_No2, FILE_No1, 16, 16, 14+4, 14+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					
		if( ReadBinaryMAC( PSAM, Para1Data, FILE_No4, FILE_No1, 16, 16, 14+4, 14+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					
	}	

	if(( ParaUpdateFlag & 0x01 ) != 0 )				//更新当前套费率				//新国网	//13.11.22
	{
		RAM_Write( CPrice1Data, RandomData, 4 );
		RAM_Write( CPrice2Data, RandomData, 4 );		
//		if( ReadBinaryMAC( PSAM, CPrice1Data, FILE_No3, FILE_No3, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					 
		if( ReadBinaryMAC( PSAM, CPrice1Data, FILE_No5, FILE_No3, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					 
//		if( ReadBinaryMAC( PSAM, CPrice2Data, FILE_No3, FILE_No3, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					  
		if( ReadBinaryMAC( PSAM, CPrice2Data, FILE_No5, FILE_No3, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					  
	}
	if(( ParaUpdateFlag & 0x02 ) != 0 )				//更新备用套费率				//新国网	//13.11.22
	{
		RAM_Write( BPrice1Data, RandomData, 4 );
		RAM_Write( BPrice2Data, RandomData, 4 );
		RAM_Write( Para2Data, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, BPrice1Data, FILE_No4, FILE_No4, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					   
		if( ReadBinaryMAC( PSAM, BPrice1Data, FILE_No6, FILE_No4, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					   
//		if( ReadBinaryMAC( PSAM, BPrice2Data, FILE_No4, FILE_No4, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
		if( ReadBinaryMAC( PSAM, BPrice2Data, FILE_No6, FILE_No4, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
//		if( ReadBinaryMAC( PSAM, Para2Data, FILE_No2, FILE_No1, 10, 10, 5+4, 5+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06  		//两套分时费率切换时间
		if( ReadBinaryMAC( PSAM, Para2Data, FILE_No4, FILE_No1, 10, 10, 5+4, 5+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06  		//两套分时费率切换时间
	}

	if(( ParaUpdateFlag & 0x04 ) != 0 )				//更新当前套阶梯				//新国网	//13.11.22
	{
		RAM_Write( CStepData, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, CStepData, FILE_No3, FILE_No3, 132, 132, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
		if( ReadBinaryMAC( PSAM, CStepData, FILE_No5, FILE_No3, 132, 132, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
	}

	if(( ParaUpdateFlag & 0x08 ) != 0 )				//更新当前套阶梯				//新国网	//13.11.22
	{
		RAM_Write( BStepData, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, BStepData, FILE_No4, FILE_No4, 132, 132, 69+4, 69+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06  
		if( ReadBinaryMAC( PSAM, BStepData, FILE_No6, FILE_No4, 132, 132, 69+4, 69+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06  
	}

	CardProceIntegrality(); 				//读卡数据正常结束，允许拔卡				//新国网	//14.01.04					

	if(( ParaUpdateFlag & 0x80 ) != 0 )				//更新其他参数
	{
//		if( UpdateBinaryMAC( ESAM, Para1Data, FILE_No2, 16, 14+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.03								
		if( UpdateBinaryMAC( ESAM, Para1Data, FILE_No4, 16, 14+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.03								

		SwapData( Para1Data, 4 );
		E2P_WData( AlarmEC1, Para1Data, 4 );
		SwapData( Para1Data+4, 4 );
		E2P_WData( AlarmEC2, Para1Data+4, 4 );
		SwapData( Para1Data+8, 3 );
//		E2P_WData( CurrentRate, Para1Data+8, 3 );
		WriteVoltCurrentRate( CurrentRate, Para1Data+8 );
		SwapData( Para1Data+11, 3 );
//		E2P_WData( VoltRate, Para1Data+11, 3 );
		WriteVoltCurrentRate( VoltRate, Para1Data+11 );
	}

	GetBackBuyECFile1( Ptr );
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 0, 49 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }		//全部数据写入ESAM的运行信息文件	//新国网	//14.01.06

	if(( ParaUpdateFlag & 0x01 ) != 0 )				//更新当前套费率				//新国网	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, CPrice1Data, FILE_No3, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CPrice1Data, FILE_No5, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, CPrice1Data+4+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Fee1Price+i*5, Point, 4 );									
		}	

//		if( UpdateBinaryMAC( ESAM, CPrice2Data, FILE_No3, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CPrice2Data, FILE_No5, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, CPrice2Data+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Fee1Price+16*5+i*5, Point, 4 );								
		}	
		GetFeePriceNum();													//新国网	//13.11.30	
	}

	if(( ParaUpdateFlag & 0x02 ) != 0 )				//更新备用套费率				//新国网	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, BPrice1Data, FILE_No4, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BPrice1Data, FILE_No6, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, BPrice1Data+4+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Fee1Price+i*5, Point, 4 );									
		}	

//		if( UpdateBinaryMAC( ESAM, BPrice2Data, FILE_No4, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BPrice2Data, FILE_No6, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, BPrice2Data+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Fee1Price+16*5+i*5, Point, 4 );								
		}	

//		if( UpdateBinaryMAC( ESAM, Para2Data, FILE_No2, 10, 5+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, Para2Data, FILE_No4, 10, 5+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		RAM_Write( Point, Para2Data, 5 );
//		SwapData( Point, 5 );
//		E2P_WData( FeeSwitchTime, Point, 5 );			
//		RAM_Fill( Point, 5);							//新设置分时费率切换时间后，不管时间点是否相等，都根据当前时间重新判断是否切换						
//		E2P_WData( OldFeeSwitchTime, Point, 5 );					
		WriteFeeStepSwitchTime( FeeSwitchTime, OldFeeSwitchTime, Point );
		FeeTabSwitch();

	}

	if(( ParaUpdateFlag & 0x04 ) != 0 )				//更新当前套阶梯				//新国网	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, CStepData, FILE_No3, 132, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CStepData, FILE_No5, 132, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		for( i=0;i<13;i++ )
		{
			RAM_Write( Point, CStepData+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Step1EC+i*5, Point, 4 );									
		}	

		for( i=0;i<4;i++ )
		{
			E2P_RData( Ptr+i*3, Case1YearSaveDay1+i*4, 3 );	
			RAM_Write( Point, CStepData+52+i*3, 3 );
			SwapData( Point, 3 );
			E2P_WData( Case1YearSaveDay1+i*4, Point, 3 );									
			RAM_Write( Ptr+12+i*3, Point, 3 );
		}
		YearMonSaveDaySwitchProcess( Ptr, Ptr+12 );			
	}

	if(( ParaUpdateFlag & 0x08 ) != 0 )				//更新备用套阶梯				//新国网	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, BStepData, FILE_No4, 132, 69+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BStepData, FILE_No6, 132, 69+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		for( i=0;i<13;i++ )
		{
			RAM_Write( Point, BStepData+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Step1EC+i*5, Point, 4 );									
		}	

		for( i=0;i<4;i++ )
		{
			RAM_Write( Point, BStepData+52+i*3, 3 );
			SwapData( Point, 3 );
			E2P_WData( Case2YearSaveDay1+i*4, Point, 3 );									
		}	

		RAM_Write( Point, BStepData+64, 5 );
//		SwapData( Point, 5 );
//		E2P_WData( StepSwitchTime, Point, 5 );			
//		RAM_Fill( Point, 5);							//新设置分时费率切换时间后，不管时间点是否相等，都根据当前时间重新判断是否切换						
//		E2P_WData( OldStepSwitchTime, Point, 5 );					
		WriteFeeStepSwitchTime( StepSwitchTime, OldStepSwitchTime, Point );
		StepTabSwitch();

	}

	GetECState();                                               		//11.07.22

	if(( ICComm.ICECState == F_ECAlarm20 )||( ICComm.ICECState == F_ECOver ))	//取消预跳闸	//新国网	//13.12.23	
	{
		ExitPreTripState();
		ICComm.RelayOFFState = 0;
		ICComm.RelayONState = 0x00;
	}
	
	ClearJudgePermitFlag();										//新国网	//13.12.10    					

	ICComm.ICDisState = ICDisRemainEC;					//需显示剩余电量
	ICComm.ICInState &= ~F_RegECLimt;
}	

/*
void CreatBuyECRec( void )						//产生购电记录
{
	unsigned char Buff[25];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue;
	long LastPt;
	
	Ptr = Buff;	
	
	LastPt = PtAddrChk( BuyMoney_No, Next );				
	if( LastPt == -1 ) return;
	RAM_Write( Ptr, Clk.Min, 5 );
	E2P_RData( (unsigned char*)&Value, BuyECCnt, 4 );
	Long_BCD4( Ptr+5, Value );				
	E2P_RData( (unsigned char*)&Value, CurrentBuyEC, 4 );
	Long_BCD4( Ptr+7, Value );				
	E2P_RData( (unsigned char*)&Value, BfERemainEC, 4 );
	Long_BCD4( Ptr+11, Value );				
	GetReOverEC( Ptr+15, Ptr+19 );
	RAM_Write( (unsigned char*)&Value, Ptr+15, 4 );
	Long_BCD4( Ptr+15, Value );				
	E2P_RData( (unsigned char*)&LValue, BuyECSumBCD, 8 );
	Value = LValue / 10000;
	Long_BCD4( Ptr+19, Value );				
	E2P_WData( LastPt, Ptr, 23 );
}	
*/

unsigned long GetBuyECSum( void ) 							//取累计购电金额
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue;
	
	Ptr = Buff;
	
	E2P_RData( Ptr, BuyECSumBCD, 8 );						//累计购电量 XXXXXXXXXX.XXXXXX 元
	RAM_Write( (unsigned char*)&LValue, Ptr, 8 );
	LValue /= 10000;
	Value = (unsigned long)LValue;							//返回累计购电量 XXXXXX.XX 元	

	return Value;	
}	


							//remote
void AddBuyECSumBCD( unsigned char* Source ) 		//累加累计购电金额
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue1;
	unsigned long long LValue2;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, Source, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( (unsigned char*)&Value, Ptr, 4 );
	E2P_WData( CurrentBuyEC, Ptr, 4 );						//本次购电量

	E2P_RData( Ptr, BuyECSumBCD, 8 );						//累计购电量 XXXXXXXXXX.XXXXXX 元
	RAM_Write( (unsigned char*)&LValue1, Ptr, 8 );
	LValue2 = Value;
	LValue2 *= 10000;
	LValue1 += LValue2;
	E2P_WData( BuyECSumBCD, (unsigned char*)&LValue1, 8 );	//累计购电量 XXXXXXXXXX.XXXXXX 元
	
}	

void DecBuyECSumBCD( unsigned char* Source ) 		//退费累计购电金额		//新国网	//13.11.27
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue1;
	unsigned long long LValue2;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, Source, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( (unsigned char*)&Value, Ptr, 4 );
//	Long_BCD4( Ptr, Value );
//	E2P_WData( EFeeBack, Ptr, 4 );							//本次退费金额

	E2P_RData( Ptr, BuyECSumBCD, 8 );						//累计购电量 XXXXXXXXXX.XXXXXX 元
	RAM_Write( (unsigned char*)&LValue1, Ptr, 8 );
	LValue2 = Value;
	LValue2 *= 10000;
	if( LValue1 >= LValue2 )
	{
		LValue1 -= LValue2;
		E2P_WData( BuyECSumBCD, (unsigned char*)&LValue1, 8 );	//累计购电量 XXXXXXXXXX.XXXXXX 元
	}		
}	

short WritePSAMBackInfoFile( unsigned char* Source )		//返写用户卡返写信息文件		//新国网	//14.01.06
{
	GetBackBuyECFile1( Source );
	if( UpdateBinary( ESAM, Source, FILE_No7, 0, 49 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }		//全部数据写入ESAM的运行信息文件	//新国网	//13.01.06

	if( GetChallenge( PSAM, Source, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return -1; }		//新国网	//13.01.06											
//	if( ReadBinaryMAC( ESAM, Source, FILE_No5, FILE_No7, 0x00, 0x00, 49+4, 49+4, 1, Para.CardNo ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }		//新国网	//13.01.06 
	if( ReadBinaryMAC( ESAM, Source, FILE_No5, FILE_No7, 0x00, 0x00, 49, 49+4, 1, Para.CardNo ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return -1; }		//新国网	//13.01.06 
	if( UpdateBinaryMAC( PSAM, Source, FILE_No5, 0x00, 49+4, 0, Para.CardNo ) ) { ICComm.ErrorMessage = ICERROR_10; return -1; }																	

	return 0;
}	
							//remote
void IC_BuyEC( void )		//购电卡					//新国网	//13.11.23				
{
	unsigned char Buff[60];								//新国网	//13.11.23	
	unsigned char Buff2[20];
	unsigned char RandomData[8];						//新国网	//13.11.23				
	unsigned char Para1Data[20];						//新国网	//13.11.23					
	unsigned char Para2Data[20];						//新国网	//13.11.23				
	unsigned char CPrice1Data[80];						//新国网	//13.11.23				
	unsigned char CPrice2Data[80];						//新国网	//13.11.23					
	unsigned char BPrice1Data[80];						//新国网	//13.11.23				
	unsigned char BPrice2Data[80];						//新国网	//13.11.23							
	unsigned char CStepData[80];						//新国网	//13.11.23				
	unsigned char BStepData[80];						//新国网	//13.11.23					
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned long Temp1=0,Temp2;
	unsigned long Value,Value2,Value3;
	unsigned char BuyState=0x55;
	unsigned char ParaUpdateFlag;
	unsigned char CardType;
	short i;
    
	Point = Buff2;
	Ptr = Buff;
	
	ParaUpdateFlag = ICComm.ICBuf1[5];
	ICComm.ParaUpdateFlag = ParaUpdateFlag;    
	    
	RAM_Write( Ptr, ICComm.ICBuf1, 44 );	//通讯缓存数据搬至内存
	CardType = *(Ptr+42);					//用户卡类型	

															//新国网	//13.12.05							
//	E2P_RData( Point, EUser_No, 6 );						
	E2P_RData( Point, EMeter_No, 8 );						
	if( Data_Comp( Point, Ptr+30, 6 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_12; ICComm.ErrorMessage = ICERROR_11; ClearOperateCommHead_ErrorResponse(); return; }	//核对表号错	//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.10

	if(( *(Ptr+42) != 0x01)&&( *(Ptr+42) != 0x02 )&&( *(Ptr+42) != 0x03 ))		//卡类型错		
	{ 
		ICComm.ErrorMessage = ICERROR_08;					//新国网		//13.12.03
		ICComm.ICAlarmFlg = ICALARM_26; 
		ClearOperateCommHead_ErrorResponse(); 				//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.02
		return; 
	}

															//新国网	//13.12.05									
	if( ReadBinary( PSAM, Point, FILE_No2, 0x04, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }		//读PSAM钱包文件内的购电次数	//新国网	//14.01.06
	SwapData( Point, 4 );
	RAM_Write( (unsigned char*)&Temp2, Point, 4 );			//PSAM的购电次数
			
	E2P_RData( (unsigned char*)&ICComm.ROpenAccountFlag, OpenAccountFlag, 1 );
	ICComm.ROpenAccountFlag &= 0x03;											//取本地、远程开户状态		//新国网		//13.12.05
	if(( ICComm.ROpenAccountFlag == 0x03 )&&( CardType != 0x01 ))				//未开户表只认开户卡		//新国网		//13.12.05
	{
		if( CardType == 0x02 )	ICComm.ErrorMessage = ICERROR_15;		//新国网		//13.12.03
		else ICComm.ErrorMessage = ICERROR_16;							//新国网		//13.12.03
		ICComm.ICAlarmFlg = ICALARM_22; 
		ClearOperateCommHead_ErrorResponse(); 				//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.02
		return; 	
	}	
	
	if(( CardType == 0x01)&&( ICComm.ROpenAccountFlag == 0x03 )) ;				//新开户卡对未开户表不核对客户编号				//新国网		//13.12.05
	else	
	{
		E2P_RData( Point, Client_No, 6 );
		if( Data_Comp( Point, Ptr+36, 6 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_12; ICComm.ErrorMessage = ICERROR_12; ClearOperateCommHead_ErrorResponse(); return; }				//核对客户编号错	//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.02
		if( CardType == 0x02 )								//已开户的购电卡要判卡序列号					//新国网		//13.12.10	
		{
			E2P_RData( Point, UserCardNo, 8 );
			if( Data_Comp( Point, Para.CardNo, 8 ) != 0 ) { ICComm.ErrorMessage = ICERROR_13; ClearOperateCommHead_ErrorResponse(); return; }				//核对卡序列号错	//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.10	
		}	
	}

	if(( CardType == 0x01 )&&( Temp2 > 1 ))					//开户卡购电次数不为0或1，报错？				//新国网		//13.12.05
	{
		ICComm.ErrorMessage = ICERROR_17; 						
		ClearOperateCommHead_ErrorResponse(); 
		return; 		
	}		
															//新国网	//13.12.05									
//	if( ReadRecord( ESAM, Point, FILE_No3, 0x0C, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//读ESAM钱包文件内的购电次数	//新国网	//14.01.06
	if( ReadRecord( ESAM, Point ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//读ESAM钱包文件内的购电金额、购电次数、客户编号	//新国网	//14.01.06
	RAM_Write( Point, Point+4, 4 );														//读ESAM钱包文件内的购电次数	//新国网	//14.01.06				
	SwapData( Point, 4 );
	RAM_Write( (unsigned char*)&Temp1, Point, 4 );			//ESAM的购电次数
	
	if( Temp2 < Temp1 ) 									//卡内购电次数小于表内购电次数，不报错			//新国网	//13.12.05
	{ 
		if(( CardType == 0x01 )||( CardType == 0x03 ))		//开户卡或补卡保存卡序列号	
		{
			E2P_WData( UserCardNo, Para.CardNo, 8 ); 		
		}
		if( CardType == 0x01 )
		{
			ICComm.ROpenAccountFlag &= ~0x01;									//置本地开户状态		
			E2P_WData( OpenAccountFlag, (unsigned char*)&ICComm.ROpenAccountFlag, 1 );
		}
		
		if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//返写用户卡返写信息文件				//新国网	//13.12.05	//14.01.06
		ICComm.ICDisState = ICDisRemainEC;			//正常插卡结束需显示剩余电量  			//新国网	//13.12.05
		ICComm.ParaUpdateFlag = 0;					//避免插用户卡后未更新参数却增加编程	//新国网	//14.01.06
		return; 
	}				

	if( Temp2 >= (Temp1+2) )	
	{
		if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//返写用户卡返写信息文件		//新国网	//13.12.05	//14.01.06				
		ICComm.ErrorMessage = ICERROR_17; 
		ClearOperateCommHead_ErrorResponse();
		return;			
	}	

//	if( ReadBinary( PSAM, Point, FILE_No5, 0x00, 3 ) != 0 ) return;		//读电表反馈信息文件判断是否开户卡
	if( ReadBinary( PSAM, Point, FILE_No5, 0x05, 3 ) != 0 ) return;		//读电表反馈信息文件判断是否开户卡	//发卡软件只清空了数据区，未整个文件清空	//Test
	if( IsAllData( Point, 3, 0x0 ) == 0 ) BuyState = 0; 				//返写文件为空					//新国网	//13.12.05		

//    BuyState = 0; 				//返写文件为空(发卡软件未清空)			//16.11.28      //Test
    
	if( Temp2 == (Temp1+1) )							//新国网	//13.12.05									
	{
		if( BuyState != 0 )
		{
			if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//返写用户卡返写信息文件		//新国网	//13.12.05	//14.01.06				
			ICComm.ErrorMessage = ICERROR_18; 
			ClearOperateCommHead_ErrorResponse();
			return;
		}	
	}
			
	if(( CardType == 0x01 )||( CardType == 0x03 ))												//新国网	//13.12.05
	{
		if(( Temp2 == Temp1 )&&( BuyState != 0 ))
		{
			E2P_RData( Point, UserCardNo, 8 );														
			if( Data_Comp( Point, Para.CardNo, 8 ) != 0 ) { ICComm.ICAlarmFlg = ICALARM_12; ICComm.ErrorMessage = ICERROR_13; ClearOperateCommHead_ErrorResponse(); return; }	//用户卡序列号错	//非通讯原因清除操作命令头、错误响应状态		//新国网	//13.12.02		
			else 
			{
				if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//返写用户卡返写信息文件		//新国网	//13.12.05		//14.01.06			
				ICComm.ICDisState = ICDisRemainEC;			//正常插卡结束需显示剩余电量  //新国网	//13.12.05
				ICComm.ParaUpdateFlag = 0;					//避免插开户卡、补卡后未更新参数却增加编程	//新国网	//14.01.06
				return;
			}	
		}						
	}	
	if( CardType == 0x02 )												//新国网	//13.12.05
	{
		if(( Temp2 == Temp1 )&&( BuyState != 0 ))
		{
			if( WritePSAMBackInfoFile( CPrice1Data ) != 0 ) return;		//返写用户卡返写信息文件		//新国网	//13.12.05	//14.01.06				
			ICComm.ICDisState = ICDisRemainEC;			//正常插卡结束需显示剩余电量  	//新国网	//13.12.05		
			ICComm.ParaUpdateFlag = 0;					//避免插用户卡后未更新参数却增加编程	//新国网	//14.01.06
			return;																				//新国网	//14.01.06	
		}
	}
	


	ICComm.RelayTestDelay = F_NULL; 					//V7
	if( Temp2 == (Temp1+1) )
	{

		GetReOverEC( Point, Point+4 );								//剩余电量

		if( ReadBinary( PSAM, Point+8, FILE_No2, 0x00, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }		//读PSAM钱包文件内的购电金额	//新国网	//14.01.06
		SwapData( Point+8, 4 );
		RAM_Write( (unsigned char*)&Value, Point, 4 );
		RAM_Write( (unsigned char*)&Value2, Point+8, 4 );
		Value += Value2;												//剩余电量加本次购电量
		
		E2P_RData( Point+12, RegrateECLimt, 4 );
		Value2 = BCD4_Long( Point+12 );	
		if( Value2 == 0 ) Value2 = 99999999;						//囤积限额=0，按照999999.99元处理	//新国网	//13.12.10
		RAM_Write( (unsigned char*)&Value3, Point+4, 4 );
		Value2 += Value3;											//囤积限额+透支电量	

		if( Value <= Value2 )
		{

			E2P_WData( BfERemainEC, Point, 4 );						//电量叠加前剩余电量
			E2P_WData( BfOverdraftEC, Point+4, 4 );					//电量叠加前透支电量
			
			ICComm.ICDisState = ICDisBfRemainEC;					//需显示叠加前电量
			ICComm.ICInState &= ~F_RegECLimt;

			if( GetChallenge( ESAM, Ptr, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//购电量、购电次数写入ESAM钱包文件	//新国网	//14.01.06
			if( ReadBinaryIncMAC( PSAM, Ptr, FILE_No2, 0x00 ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; } 	//新国网	//14.01.06
			if( UpdateBinaryIncMAC( ESAM, Ptr, Para.CardNo ) != 0 ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 );  return ; }		//新国网	//14.01.02

			RecordStartEvent( BuyMoney_No );
			AddBuyECSumBCD( Ptr );					 				//累加累计购电金额		
			
			Value = Temp1 + 1;
			E2P_WData( BuyECCnt, (unsigned char*)&Value, 4 );
			*Point = 0x0;
			E2P_WData( BuyECFlag, Point, 1 );							//购电成功
//			CreatBuyECRec();						//产生购电记录
			RecordEndEvent( BuyMoney_No, 0 );

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
			ICComm.PreTripOFFKeepTime = 0; 																			//新国网	//14.06.22	
			if(( Para.RMeterTripState == 0x1D )||( Para.RMeterTripState == 0x1E )) Para.RMeterTripState = 0x1B;		//新国网	//14.06.22	
#endif

		}
		else 
		{
			ICComm.ICInState |= F_RegECLimt;
			ICComm.ErrorMessage = ICERROR_21;					//新国网	//13.12.05
			return;												//新国网	//13.12.05
		}	
		ICComm.ICInState |= F_BuyECOK;				//V8	//无论购电是否成功，都显示当前购电量	
	}	

	if( CardType != 0x01 ) 
	{
		ICComm.ParaUpdateFlag &= 0x0A;	//非开户卡只支持修改备用套费率电价和备用套阶梯参数		//新国网	//13.11.28				
		ParaUpdateFlag &= 0x0A;			//非开户卡只支持修改备用套费率电价和备用套阶梯参数		//新国网	//14.01.06				
	}	

	if( GetChallenge( ESAM, RandomData, 4 ) != 0 ) { ICComm.ErrorMessage = ICERROR_02; return; }	//新国网	//14.01.06				
	if( CardType == 0x01 )							//更新客户编号					//新国网	//13.12.05				
	{
		RAM_Write( Ptr, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, Ptr, FILE_No2, FILE_No1, 36, 36, 6+4, 6+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 	
		if( ReadBinaryMAC( PSAM, Ptr, FILE_No15, FILE_No1, 6, 36, 6+4, 6+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 	
//		if( UpdateBinaryMAC( ESAM, Ptr, FILE_No2, 36, 6+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, Ptr, FILE_No15, 6, 6+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		
		SwapData( Ptr, 6 );
		E2P_WData( Client_No, Ptr, 6 );
	}	

	if((( ParaUpdateFlag & 0x80 ) != 0 )&&( CardType == 0x01 ))	//更新其他参数		//新国网	//13.11.23				
	{
		RAM_Write( Para1Data, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, Para1Data, FILE_No2, FILE_No1, 16, 16, 14+4, 14+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					
		if( ReadBinaryMAC( PSAM, Para1Data, FILE_No4, FILE_No1, 16, 16, 14+4, 14+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 					
	}	

	if((( ParaUpdateFlag & 0x01 ) != 0 )&&( CardType == 0x01 ))	//更新当前套费率	//新国网	//13.11.22
	{
		RAM_Write( CPrice1Data, RandomData, 4 );
		RAM_Write( CPrice2Data, RandomData, 4 );		
//		if( ReadBinaryMAC( PSAM, CPrice1Data, FILE_No3, FILE_No3, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
		if( ReadBinaryMAC( PSAM, CPrice1Data, FILE_No5, FILE_No3, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
//		if( ReadBinaryMAC( PSAM, CPrice2Data, FILE_No3, FILE_No3, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
		if( ReadBinaryMAC( PSAM, CPrice2Data, FILE_No5, FILE_No3, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
	}
	if(( ParaUpdateFlag & 0x02 ) != 0 )				//更新备用套费率				//新国网	//13.11.22
	{
		RAM_Write( BPrice1Data, RandomData, 4 );
		RAM_Write( BPrice2Data, RandomData, 4 );
		RAM_Write( Para2Data, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, BPrice1Data, FILE_No4, FILE_No4, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
		if( ReadBinaryMAC( PSAM, BPrice1Data, FILE_No6, FILE_No4, 0x00, 0x00, 68+4, 68+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
//		if( ReadBinaryMAC( PSAM, BPrice2Data, FILE_No4, FILE_No4, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
		if( ReadBinaryMAC( PSAM, BPrice2Data, FILE_No6, FILE_No4, 68, 68, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 
//		if( ReadBinaryMAC( PSAM, Para2Data, FILE_No2, FILE_No1, 10, 10, 5+4, 5+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 		//两套分时费率切换时间
		if( ReadBinaryMAC( PSAM, Para2Data, FILE_No4, FILE_No1, 10, 10, 5+4, 5+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06 		//两套分时费率切换时间
	}

	if((( ParaUpdateFlag & 0x04 ) != 0 )&&( CardType == 0x01 ))	//更新当前套阶梯	//新国网	//13.11.22
	{
		RAM_Write( CStepData, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, CStepData, FILE_No3, FILE_No3, 132, 132, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06
		if( ReadBinaryMAC( PSAM, CStepData, FILE_No5, FILE_No3, 132, 132, 64+4, 64+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06
	}

	if(( ParaUpdateFlag & 0x08 ) != 0 )				//更新备用套阶梯				//新国网	//13.11.22
	{
		RAM_Write( BStepData, RandomData, 4 );
//		if( ReadBinaryMAC( PSAM, BStepData, FILE_No4, FILE_No4, 132, 132, 69+4, 69+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06
		if( ReadBinaryMAC( PSAM, BStepData, FILE_No6, FILE_No4, 132, 132, 69+4, 69+4, 0, ICEC.LocalDisperseGene ) != 0 ) { ICComm.ErrorMessage = ICERROR_19; return; }	//新国网	//14.01.06
	}

	if((( ParaUpdateFlag & 0x80 ) != 0 )&&( CardType == 0x01 ))	//更新其他参数
	{
//		if( UpdateBinaryMAC( ESAM, Para1Data, FILE_No2, 16, 14+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, Para1Data, FILE_No4, 16, 14+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		SwapData( Para1Data, 4 );
		E2P_WData( AlarmEC1, Para1Data, 4 );
		SwapData( Para1Data+4, 4 );
		E2P_WData( AlarmEC2, Para1Data+4, 4 );
		SwapData( Para1Data+8, 3 );
//		E2P_WData( CurrentRate, Para1Data+8, 3 );
		WriteVoltCurrentRate( CurrentRate, Para1Data+8 );
		SwapData( Para1Data+11, 3 );
//		E2P_WData( VoltRate, Para1Data+11, 3 );
		WriteVoltCurrentRate( VoltRate, Para1Data+11 );
	}

	if( WritePSAMBackInfoFile( Ptr ) != 0 ) return;		//返写用户卡返写信息文件				//新国网	//14.01.04	//14.01.06				
	CardProceIntegrality(); 			//读卡数据正常结束，允许拔卡			//新国网	//14.01.04					

	if((( ParaUpdateFlag & 0x01 ) != 0 )&&( CardType == 0x01 ))					//更新当前套费率	//新国网	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, CPrice1Data, FILE_No3, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CPrice1Data, FILE_No5, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

//		CreateFeePricePrgRec( FeePricePrg_No );									//新国网	//13.11.26
		RecordStartEvent( FeePricePrg_No );

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, CPrice1Data+4+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Fee1Price+i*5, Point, 4 );									
		}	

//		if( UpdateBinaryMAC( ESAM, CPrice2Data, FILE_No3, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CPrice2Data, FILE_No5, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, CPrice2Data+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Fee1Price+16*5+i*5, Point, 4 );								
		}	
		GetFeePriceNum();													//新国网	//13.11.30	
	}

	if(( ParaUpdateFlag & 0x02 ) != 0 )				//更新备用套费率				//新国网	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, BPrice1Data, FILE_No4, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BPrice1Data, FILE_No6, 0x00, 68+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

//		CreateFeePricePrgRec( FeePricePrg_No );									//新国网	//13.11.26
		RecordStartEvent( FeePricePrg_No );
		
		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, BPrice1Data+4+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Fee1Price+i*5, Point, 4 );									
		}	

//		if( UpdateBinaryMAC( ESAM, BPrice2Data, FILE_No4, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BPrice2Data, FILE_No6, 68, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

		for( i=0;i<16;i++ )
		{
			RAM_Write( Point, BPrice2Data+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Fee1Price+16*5+i*5, Point, 4 );								
		}	

//		if( UpdateBinaryMAC( ESAM, Para2Data, FILE_No2, 10, 5+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, Para2Data, FILE_No4, 10, 5+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		RAM_Write( Point, Para2Data, 5 );
//		SwapData( Point, 5 );
//		E2P_WData( FeeSwitchTime, Point, 5 );			
//		RAM_Fill( Point, 5);							//新设置分时费率切换时间后，不管时间点是否相等，都根据当前时间重新判断是否切换						
//		E2P_WData( OldFeeSwitchTime, Point, 5 );					
		WriteFeeStepSwitchTime( FeeSwitchTime, OldFeeSwitchTime, Point );
		FeeTabSwitch();

	}

	if((( ParaUpdateFlag & 0x04 ) != 0 )&&( CardType == 0x01 ))					//更新当前套阶梯				//新国网	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, CStepData, FILE_No3, 132, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, CStepData, FILE_No5, 132, 64+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

//		CreateStepParaPrgRec( StepParaPrg_No );									//新国网	//13.11.26
		RecordStartEvent( StepParaPrg_No );
			
		for( i=0;i<13;i++ )
		{
			RAM_Write( Point, CStepData+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case1Step1EC+i*5, Point, 4 );									
		}	

		for( i=0;i<4;i++ )
		{
			E2P_RData( Ptr+i*3, Case1YearSaveDay1+i*4, 3 );	
			RAM_Write( Point, CStepData+52+i*3, 3 );
			SwapData( Point, 3 );
			E2P_WData( Case1YearSaveDay1+i*4, Point, 3 );									
			RAM_Write( Ptr+12+i*3, Point, 3 );
		}
		YearMonSaveDaySwitchProcess( Ptr, Ptr+12 );			
	}

	if(( ParaUpdateFlag & 0x08 ) != 0 )				//更新备用套阶梯			//新国网	//13.11.22
	{
//		if( UpdateBinaryMAC( ESAM, BStepData, FILE_No4, 132, 69+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								
		if( UpdateBinaryMAC( ESAM, BStepData, FILE_No6, 132, 69+4, 0, ICEC.LocalDisperseGene ) ) { ICComm.ErrorMessage = ICERROR_10; Recd_ADD1( ILInsert_Cunt, 3 ); return ; }		//新国网	//13.12.10								

//		CreateStepParaPrgRec( StepParaPrg_No );									//新国网	//13.11.26
		RecordStartEvent( StepParaPrg_No );
		
		for( i=0;i<13;i++ )
		{
			RAM_Write( Point, BStepData+i*4, 4 );
			SwapData( Point, 4 );
			E2P_WData( Case2Step1EC+i*5, Point, 4 );									
		}	

		for( i=0;i<4;i++ )
		{
			RAM_Write( Point, BStepData+52+i*3, 3 );
			SwapData( Point, 3 );
			E2P_WData( Case2YearSaveDay1+i*4, Point, 3 );									
		}	

		RAM_Write( Point, BStepData+64, 5 );
//		SwapData( Point, 5 );
//		E2P_WData( StepSwitchTime, Point, 5 );			
//		RAM_Fill( Point, 5);							//新设置分时费率切换时间后，不管时间点是否相等，都根据当前时间重新判断是否切换						
//		E2P_WData( OldStepSwitchTime, Point, 5 );					
		WriteFeeStepSwitchTime( StepSwitchTime, OldStepSwitchTime, Point );
		StepTabSwitch();

	}

	if(( ICComm.ICInState & F_RegECLimt ) != 0 ) ICComm.ICAlarmFlg = ICALARM_14;

	if(( CardType == 0x01 )||( CardType == 0x03 ))		//开户卡或补卡保存卡序列号		//新国网	//13.12.05					
	{
		E2P_WData( UserCardNo, Para.CardNo, 8 ); 		
	}
	if( CardType == 0x01 )																//新国网	//13.12.05						
	{
		ICComm.ROpenAccountFlag &= ~0x01;									//置本地开户状态		
		E2P_WData( OpenAccountFlag, (unsigned char*)&ICComm.ROpenAccountFlag, 1 );
	}
	
	GetReOverEC( Point, Point+4 );								//剩余电量
	GetICECState( Point, Point+4 );
	if( ICComm.RelayONState == 0xAA ) ICComm.RelayONState = 0x55;	//在允许合闸状态下，插卡直接合闸

	ICComm.ICInState |= ( F_BuyEC + F_BuyECOver );												//V7
	SaveBuyECState();
	if( ICComm.ICDisState != ICDisBfRemainEC ) ICComm.ICDisState = ICDisRemainEC;		//需显示剩余电量  //参照天津 11.06.27

}

//EEPROM内的现场参数设置卡版本号和身份认证时效性不起作用
//以ESAM内的参数为准,4费率
void GetAndCompareESAMPara( void )
{
	unsigned char Buff[50];
	unsigned char Buff2[10];
	unsigned char* Ptr;
	unsigned char* Point;	
	unsigned long Value1;
	unsigned long Value2;
	unsigned short Len;
	unsigned short Length;
    
	Ptr = Buff;
	Point = Buff2;

//	if( ReadBinary( ESAM, Ptr, FILE_No5, 00, 4 ) == 0 ) 		//新国网
//	{	
//		SwapData( Ptr, 4 );
//		E2P_RData( Point, ESecretKeyState, 4 );						
//		if( Data_Comp( Ptr, Point, 4 ) != 0 )
//		{
//			E2P_WData( ESecretKeyState, Ptr, 4 );				//保存密钥信息	
//			RAM_Write( Flag.SecretKeyState, Ptr, 4 );
//		}	
//	}
	
	GetCurSecretKeyState();
	
//	if( ReadBinary( ESAM, Ptr, FILE_No2, 0x04, 38 ) == 0 ) 
	if( ReadBinary( ESAM, Ptr, FILE_No4, 0x04, 38 ) == 0 ) 
	{	
		E2P_RData( Point, UserType, 1 );						//用户类型
		if( *Ptr != *Point ) E2P_WData( UserType, Ptr, 1 );		
		
//		SwapData( Ptr+6, 5 );
//		E2P_RData( Point, FeeSwitchTime, 5 );					//分时费率切换时间
//		if( Data_Comp( Ptr+6, Point, 5 ) != 0 )
//		{
//			E2P_WData( FeeSwitchTime, Ptr+6, 5 );					
//		}	

		SwapData( Ptr+6, 5 );
		E2P_RData( Point, FeeSwitchTime, 7 );					//分时费率切换时间
		ChangeTimeHexToBCD( Point, Point );
		if( Data_Comp( Ptr+6, Point+1, 5 ) != 0 )
		{
			RAM_Write( Point+1, Ptr+6, 5 );
			*Point = 0;
			if( IsAllData( Point+1, 5, 0x00 ) == 0 ) *(Point+6) = 0;
			else ChangeTimeBCDToHex( Point, Point );
			E2P_WData( FeeSwitchTime, Point, 7 );					
		}	
		
		SwapData( Ptr+12, 4 );
		E2P_RData( Point, AlarmEC1, 4 );						
		if( Data_Comp( Ptr+12, Point, 4 ) != 0 )
		{
			E2P_WData( AlarmEC1, Ptr+12, 4 );					
		}	
	
		SwapData( Ptr+16, 4 );
		E2P_RData( Point, AlarmEC2, 4 );						
		if( Data_Comp( Ptr+16, Point, 4 ) != 0 )
		{
			E2P_WData( AlarmEC2, Ptr+16, 4 );					
		}	
	
//		SwapData( Ptr+20, 3 );
//		E2P_RData( Point, CurrentRate, 3 );						
//		if( Data_Comp( Ptr+20, Point, 3 ) != 0 )
//		{
//			E2P_WData( CurrentRate, Ptr+20, 3 );					
//		}	

		SwapData( Ptr+20, 3 );
		Value1 = BCD3_Long( Ptr+20 );
		Value1 *= 100;
		E2P_RData( (unsigned char*)&Value2, CurrentRate, 4 );						
		if( Value1 != Value2 )
		{
			E2P_WData( CurrentRate, (unsigned char*)&Value1, 4 );					
		}	

//		SwapData( Ptr+23, 3 );
//		E2P_RData( Point, VoltRate, 3 );						
//		if( Data_Comp( Ptr+23, Point, 3 ) != 0 )
//		{
//			E2P_WData( VoltRate, Ptr+23, 3 );					
//		}	

		SwapData( Ptr+23, 3 );
		Value1 = BCD3_Long( Ptr+23 );
		Value1 *= 100;
		E2P_RData( (unsigned char*)&Value2, VoltRate, 4 );						
		if( Value1 != Value2 )
		{
			E2P_WData( VoltRate, (unsigned char*)&Value1, 4 );					
		}	
	
//		SwapData( Ptr+26, 6 );
//		E2P_RData( Point, EUser_No, 6 );						
//		if( Data_Comp( Ptr+26, Point, 6 ) != 0 )
//		{
//			E2P_WData( EUser_No, Ptr+26, 6 );					
//		}	
	
//		SwapData( Ptr+32, 6 );
//		E2P_RData( Point, Client_No, 6 );						
//		if( Data_Comp( Ptr+32, Point, 6 ) != 0 )
//		{
//			E2P_WData( Client_No, Ptr+32, 6 );					
//		}	
	}
	
//	if( ReadBinary( ESAM, Ptr, FILE_No1, 0x05, 8 ) == 0 ) 		
//	{
//		SwapData( Ptr, 8 );
//		E2P_RData( Point, EMeter_No, 8 );						
//		if( Data_Comp( Ptr, Point, 8 ) != 0 )
//		{
//			E2P_WData( EMeter_No, Ptr, 8 );					
//		}	
//	}	

	if( ReadBinary( ESAM, Ptr, FILE_No1, 0x04, 9 ) == 0 ) 	//多取1字节长度字节
	{
		Len = *Ptr;
		if( Len < 8 ) Length = 8 - Len;
		else 
		{
			Length = 0;
			Len = 8;
		}	
		SwapData( Ptr+1, Len );
		RAM_Fill( Ptr+1+Len, Length );
		
		E2P_RData( Point, EMeter_No, 8 );						
		if( Data_Comp( Ptr+1, Point, 8 ) != 0 )
		{
			E2P_WData( EMeter_No, Ptr+1, 8 );					
		}	
	}	
	
	if( ReadRecord( ESAM, Ptr ) == 0 ) 					//返回4字节剩余金额、4字节购电次数、6字节客户编号	
	{
		SwapData( Ptr+8, 6 );
		E2P_RData( Point, Client_No, 6 );						
		if( Data_Comp( Ptr+8, Point, 6 ) != 0 )
		{
			E2P_WData( Client_No, Ptr+8, 6 );					
		}	
	}	
}	


//增加总用电金额
//Num：0.01度的数量
//PriceType：0:计算阶梯电价	1：计算费率电价
void IncConsumECSum( unsigned short Num, short PriceType )		//15.07.31
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned long long LValue1,LValue2;
	
	Ptr = Buff;
	
	if( PriceType == 0 ) LValue2 = GetCurrentStepPrice();	//取当前阶梯电价，6位小数，16进制
	else LValue2 = GetCurrentFeePrice();					//取当前费率电价，6位小数，16进制
	if( LValue2 == 0 ) return;								//单价为0，不再计算

	GetFRAMConsumECSum( Ptr );
	RAM_Write( (unsigned char*)&LValue1, Ptr, 8 );	
	
	LValue2 *= Num;
	RAM_Fill( Ptr, 8 );
//	E2P_RData( Ptr, VoltRate, 3 );
//	Value = BCD4_Long( Ptr );
	E2P_RData( (unsigned char*)&Value, VoltRate, 4 );
	LValue2 *= Value;									//电压互感器变比
	LValue2 /= 100;										//电压互感器变比带2位小数
//	E2P_RData( Ptr, CurrentRate, 3 );
//	Value = BCD4_Long( Ptr );
	E2P_RData( (unsigned char*)&Value, CurrentRate, 4 );
	LValue2 *= Value;									//电流互感器变比
	LValue2 /= 100;										//电流互感器变比带2位小数
	LValue1 += LValue2;
	RAM_Write( ICEC.RConsumECSumBCD, (unsigned char*)&LValue1, 8 );
	ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
	E2P_WFM( FConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );
}	

//取当前已用金额
//第一优先级为FRAM，第二优先级为RAM，第三优先级为EEPROM，最后为FRAM的校验错的值
void GetFRAMConsumECSum( unsigned char* Dest )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	
	Ptr = Buff;

	E2P_RFM( Ptr, FConsumECSumBCD, 8 );
	RAM_Write( Dest, Ptr, 8 );
	if(( Flag.BatState & F_E2PCheck ) == 0 )
	{
		RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
		ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
	}
	else
	{	
		if( ChkNum( ICEC.RConsumECSumBCD, 8 ) == ICEC.RConsumECSumBCDChk )
		{
			E2P_RFM( Ptr, FConsumECSumBCD, 8 );											//11.07.14
			RAM_Write( Dest, Ptr, 8 );													//11.07.14
			if(( Flag.BatState & F_E2PCheck ) == 0 )									//11.07.14	
			{																			//11.07.14	
				RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );								//11.07.14		
				ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );			//11.07.14	
			}																			//11.07.14		
			else																		//11.07.14	
			{																			//11.07.14
				RAM_Write( Dest, ICEC.RConsumECSumBCD, 8 );                         	
				E2P_WFM( FConsumECSumBCD, ICEC.RConsumECSumBCD, 8 );			    	
			}																			//11.07.14
		}	
		else
		{
			E2P_RData( Ptr, ConsumECSumBCD, 8 );
			if(( Flag.BatState & F_E2PCheck ) == 0 )
			{
				E2P_RFM( Dest, FConsumECSumBCD, 8 );									//11.07.14
				if(( Flag.BatState & F_E2PCheck ) == 0 )								//11.07.14	
				{																		//11.07.14	
					RAM_Write( ICEC.RConsumECSumBCD, Dest, 8 );							//11.07.14		
					ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );		//11.07.14	
				}																		//11.07.14		
				else																	//11.07.14	
				{																		//11.07.14
					RAM_Write( Dest, Ptr, 8 );
					RAM_Write( ICEC.RConsumECSumBCD, Ptr, 8 );
					ICEC.RConsumECSumBCDChk = ChkNum( ICEC.RConsumECSumBCD, 8 );
					E2P_WFM( FConsumECSumBCD, Ptr, 8 );
				}																		//11.07.14
			}
		}		
	}
}

unsigned long GetCurrentFeePrice( void )                             //新国网    //13.12.12
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned short Addr;
	
	Ptr = Buff;
	
	Addr = Case1Fee1Price;											//新国网	//13.11.28	
	if(( SM.FeeNo == 0 )||(  SM.FeeNo > 4 )) SM.FeeNo = 1;			//新国网	//14.01.03	
	E2P_RData( Ptr, Addr+(SM.FeeNo-1)*5, 4 );
	Value = BCD4_Long( Ptr );

	return Value;	
}	

unsigned long GetCurrentECPrice( void )                             //新国网    //13.11.30
{
	unsigned long Value;
	
	if(( ICComm.RFeePriceNum == 0 )&&( SM.StepNo != 0 ))			//12.12.22		//费率数设置为0，阶梯数不为0，当前电价=当前阶梯电价
	{																//12.12.22
		Value = GetCurrentStepPrice(); 								//12.12.22	
		return Value;												//12.12.22
	}																//12.12.22
	
	Value = GetCurrentFeePrice();
	Value += GetCurrentStepPrice(); 

	return Value;	
}	

//Type: 0: 重读EEPROM内阶梯电量；1：利用RAM内阶梯电量
unsigned short GetCurrentStepNo( short Type )							//新国网	//13.11.30
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned short i;
	
	Ptr = Buff;
	
	SM.StepNo = 0;
	if(( Type == 0 )||( ICEC.ECStepChk != ChkNum( (unsigned char*)ICEC.ECStep, 24 ) ))		
	{
		for( i=0;i<6;i++ )
		{
			E2P_RData( Ptr, Case1Step1EC+i*5, 4 ); 	
			ICEC.ECStep[i] = BCD4_Long( Ptr );
		}	
		ICEC.ECStepChk = ChkNum( (unsigned char*)ICEC.ECStep, 24 );
	}	
	
	for( i=0;i<6;i++ )
	{
		if( ICEC.ECStep[i] != 0 ) break;								//所有阶梯电量为零表示不执行阶梯电价
	}	
	if( i == 6 ) return 0;
	
	if( ICComm.YearMonSaveMode == 0 ) GetCycleInsideECPp0( Ptr );		//月结算
	else GetYearCycleInsideECPp0( Ptr );									//年结算	
	Value = BCD4_Long( Ptr );
	for( i=0;i<6;i++ )
	{
		if( Value <= ICEC.ECStep[i] ) break;
		if( i != 5 )													//14.01.07
		{																//14.01.07
			if( ICEC.ECStep[i] >= ICEC.ECStep[i+1] ) 
			{
				i += 1;													//14.01.07	//避免连续相等
				break;													//14.01.07	//避免连续相等
			}	
		}																//14.01.07
	}	
	SM.StepNo = i+1;
	return i+1;	
}	

unsigned long GetCurrentStepPrice( void )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value;
	unsigned short StepNo;
	
	Ptr = Buff;
	
	StepNo = GetCurrentStepNo(1); 
	if( StepNo == 0 ) return 0;					//梯度数为零或超范围
	E2P_RData( Ptr, Case1Step1Price+(StepNo-1)*5, 4 );					//新国网
	Value = BCD4_Long( Ptr );
	
	return Value;	
}	

void GetCycleInsideECPp0( unsigned char* Dest  )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value1;
	unsigned long Value2;
	unsigned long Value3;
	unsigned long Value4;
	
	Ptr = Buff;

	Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	Value1 = BCD4_Long( Ptr );
#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
	E2P_RData( Ptr+4, L1_SaveDay1EC_Pp0, 4 );
	if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
#else
	GetLSCycleEC( Ptr+4, CMon_EC_Pp0, 0 );
#endif
	Value2 = BCD4_Long( Ptr+4 );
	
	if(( Para.RActiveMode & 0x0C ) == 0x04 )
	{
		Read_CMonEC4( Ptr, CMon_EC_Pn0 );
		Value3 = BCD4_Long( Ptr );
#if ( SaveDay1CycleEC == YesCheck )																		//12.10.04		
		E2P_RData( Ptr+4, L1_SaveDay1EC_Pn0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
#else
		GetLSCycleEC( Ptr+4, CMon_EC_Pn0, 0 );
#endif
		Value4 = BCD4_Long( Ptr+4 );
		
		Value1 += Value3;
		Value2 += Value4;
	}	

	if( Value1 < Value2 )
	{
		Value1 += 100000000;		//电量翻转
	}	
	Value1 -= Value2;
	Long_BCD4( Dest, Value1 );
}

void GetYearCycleInsideECPp0( unsigned char* Dest )			//新国网	//13.11.28
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long Value1;
	unsigned long Value2;
	unsigned long Value3;
	unsigned long Value4;
	
	Ptr = Buff;

	Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	Value1 = BCD4_Long( Ptr );
	E2P_RData( Ptr+4, L1_YearSaveDayEC_Pp0, 4 );
	if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
	Value2 = BCD4_Long( Ptr+4 );
	
	if(( Para.RActiveMode & 0x0C ) == 0x04 )
	{
		Read_CMonEC4( Ptr, CMon_EC_Pn0 );
		Value3 = BCD4_Long( Ptr );
		E2P_RData( Ptr+4, L1_YearSaveDayEC_Pn0, 4 );
		if(((Flag.BatState & F_E2PCheck) != 0 )||(AllIsBCD( Ptr+4, 4 ) != 0 )) RAM_Fill( Ptr+4, 4 );
		Value4 = BCD4_Long( Ptr+4 );
		
		Value1 += Value3;
		Value2 += Value4;
	}	

	if( Value1 < Value2 )
	{
		Value1 += 100000000;		//电量翻转
	}	
	Value1 -= Value2;
	Long_BCD4( Dest, Value1 );
}


							//remote
void SwapFrameData( unsigned char* Dest, unsigned char* Source, short ItemCount )
{
	unsigned char Buff[14];
	unsigned char* Ptr;
	short i,j,Len;
	
	Ptr = Buff;
	
	if( ItemCount == 0 ) return;
	RAM_Fill( Ptr, 14);

	for( i=0;i<ItemCount;i++ )
	{
		Len = *(Source+i);
		if( Len > 14 ) Len = 14;		//不超过14 
		for( j=0;j<Len;j++ )
		{
			*(Ptr+j) = *(Dest+Len-1-j);	
		}		
		RAM_Write( Dest, Ptr, Len );
		Dest += Len;
	}	
}	

void GetBackBuyECFile1( unsigned char* Dest )
{
	unsigned long Value;
	    
	*Dest = 0x68;
	*(Dest+1) = 0x11;
	*(Dest+2) = 0x00;
	*(Dest+3) = 0x2B;
	*(Dest+4) = 0;													//新国网		//13.09.06			
//	E2P_RData( Dest+5, CurrentRate, 3 );
	E2P_RData( (unsigned char*)&Value, CurrentRate, 4 );
	Value /= 100;													//变比带2位小数
	Long_BCD3( Dest+5, Value );
//	E2P_RData( Dest+8, VoltRate, 3 );
	E2P_RData( (unsigned char*)&Value, VoltRate, 4 );
	Value /= 100;													//变比带2位小数
	Long_BCD3( Dest+8, Value );
//	E2P_RData( Dest+11, EUser_No, 6 );
	E2P_RData( Dest+11, EMeter_No, 8 );
	E2P_RData( Dest+17, Client_No, 6 );
	GetReOverEC( Dest+23, Dest+31 );								//剩余金额
	RAM_Write( (unsigned char*)&Value, Dest+31, 4 );
	Long_BCD4( Dest+31, Value );
	E2P_RData( Dest+27, BuyECCnt, 4 );
	RAM_Fill( Dest+35, 4);											//新国网		//13.09.06			
	E2P_RData( Dest+39, ILInsert_Cunt, 3 );
	RAM_Write( Dest+42, Clk.Min, 5 );
	SwapFrameData( Dest+4, (unsigned char*)Back_BuyECTab1, 14 );
	*(Dest+47) = ChkNumAdd( Dest+1, 46 );
	*(Dest+48) = 0x16;

}

//写ESAM运行信息文件
short WriteESAMRunInfoFile( void )
{
	unsigned char Buff[50];
	unsigned char* Ptr;

	Ptr = Buff;
	
	GetBackBuyECFile1( Ptr );
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 0, 49 ) != 0 ) return 1;	//{ ICComm.ICAlarmFlg = ICALARM_60; return; }	//全部数据写入ESAM的运行信息文件	
	
	return 0;
}

//10.07.30
//写ESAM运行信息文件
short WriteESAMRunInfoFilePassWordState( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	CYCDelay(6500);						//6.5ms/4=1.26ms > 10ETU			//12.05.24

	RAM_Fill( Ptr, 12 );
	RAM_Write( Ptr, Source, 4 );
	RAM_Write( Ptr+7, Clk.Min, 5 );
	SwapData( Ptr+7, 5 );
	if( UpdateBinary( ESAM, Ptr, FILE_No7, 35, 12 ) != 0 ) return 1;		//密钥信息数据、日期写入ESAM的运行信息文件	
	return 0;
}


void IC_IOState( void )
{

	GetECState();												

}

#else

//保存2字节错误响应状态
void SaveErrorResponse( short CARD_TYPE )							//新国网	//13.12.02
{
	if( CARD_TYPE == PSAM )
	{
		if( ICComm.ICPtr1 != 0 ) 
		{
			RAM_Write( ICComm.ErrorResponse, ICComm.ICBuf1, 2 );			
			if(( ICComm.ErrorResponse[0] == 0x6B )&&( ICComm.ErrorResponse[1] == 0x00 )) ICComm.ErrorMessage = ICERROR_07;
			else ICComm.ErrorMessage = ICERROR_19;
		}else ICComm.ErrorMessage = ICERROR_19;					//新国网	//14.01.06	
	}
	else
	{
//		if( ICComm.ICPtr2 != 0 ) RAM_Write( ICComm.ErrorResponse, ICComm.ICBuf2, 2 );
		ICComm.ErrorMessage = ICERROR_02;						//操作ESAM错			
	}			
}	 

//保存5字节操作命令头
void SaveOperateCommHead( short CARD_TYPE )							//新国网	//13.12.02
{
	RAM_Fill( ICComm.ErrorResponse, 2 ); 
	if( CARD_TYPE == PSAM )
	{
		RAM_Write( ICComm.OperateCommHead, ICComm.ICBuf1, 5 );			
	}
//	else RAM_Write( ICComm.OperateCommHead, ICComm.ICBuf2, 5 );			
}	 

//读二进制文件
//CARD_TYPE: 操作卡类型
//Dest : 数据目的地址
//FileNo : 二进制文件号
//StartAddr： 读取二进制文件中的起始地址
//Length：读取数据长度
short ReadBinary( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length )
{
	unsigned char* ICBuf;
	
	short RepeatCnt=4; 
	
	while( RepeatCnt > 0 )
	{
		RepeatCnt--;
	
		ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
		if( CARD_TYPE == PSAM ) { ICBuf = ICComm.ICBuf1; }
//		else { ICBuf = ICComm.ICBuf2; }
		else
		{
			ICBuf = FBuff.Buff;
			*(ICBuf+6) = 0x00; 
			ShortToSwapChar( ICBuf+7, StartAddr );
			ShortToSwapChar( ICBuf+7+2, Length );
			if( ReadWriteESAM( Dest, 0x80, 0x2C, FileNo, 0x0005 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

			ICComm.ICAlarmFlg = 0x00;
			return 0;		
		}	
//		*ICBuf = 0x00;
//		*(ICBuf+1) = 0xB0;
//		*(ICBuf+2) = FileNo+0x80;
//		*(ICBuf+3) = StartAddr;
//		*(ICBuf+4) = Length;
//
//		SaveOperateCommHead( CARD_TYPE );			//新国网	//13.12.02
//	
//		ICDataSend( CARD_TYPE, 4, CYC60000, Length+3 );			//V6
//		if(( *ICBuf == 0xB0 )&&( *(ICBuf+Length+1) == 0x90 )&&( *(ICBuf+Length+2) == 0x00 ))
//		{
//			RAM_Write( Dest, ICBuf+1, Length );
//			ICComm.ICAlarmFlg = 0x0;						
//			return 0;	
//		}	
//		else SaveErrorResponse( CARD_TYPE );							//保存2字节错误响应状态		//新国网	//13.12.02
	}
	return -1;
}

//读ESAM钱包文件
//Dest : 数据目的地址
short ReadRecord( short CARD_TYPE, unsigned char* Dest )
{
	unsigned char* ICBuf;
	
	ICComm.ICAlarmFlg = ICALARM_28;				//读写出错
	ICBuf = FBuff.Buff;
	*(ICBuf+6) = 0x00;
	if( ReadWriteESAM( Dest, 0x80, 0x48, 0x0000, 0x0001 ) == -1 ) { SaveErrorResponse( CARD_TYPE ); return -1; }				//保存2字节错误响应状态

	ICComm.ICAlarmFlg = 0x00;
	return 0;		
}

#endif					// LocalSmartMeter == YesCheck



