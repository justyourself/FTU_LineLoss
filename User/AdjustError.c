#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Rscomm.h"
#include "AdjustError.h"
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#elif ( MEASCHIP == ATT7022E )
#include "ATT7022E.h"					
#endif

#if ( AdjustErrorCheck == YesCheck )	//15.09.05

void InitErrorPara( void )
{				
	E2P_RData( (unsigned char*)&ErrRAM.RSpeAdjustPermit,ESpeAdjustPermit, 1 ); 
	E2P_RData( (unsigned char*)&ErrRAM.RErrorMode,EErrorMode, 1 ); 
	if( ErrRAM.RErrorMode > SelfHeat_Mode )
	{
		ErrRAM.RErrorMode = NormalMode; 	
		E2P_WData( EErrorMode, (unsigned char*)&ErrRAM.RErrorMode,1 ); 
	}
	else 
	{
		if( ErrRAM.RSpeAdjustPermit != 0x67 )		//不允许补偿，切换到正常模式
		{
		    ErrRAM.VoltAmperFtFreqState = 0;
			initialErrorStateVar();
			if(( Flag.Power & F_PwrUp ) != 0 ) SwitchErrorMode( NormalMode );		//避免掉电后操作计量芯片
		}				
	}		 
}

void GetVoltAmperState( void )
{
	unsigned long AImax100;			
	unsigned long AIb100;			
	unsigned long V100Un;		
	
#if( LinkMode == Phase3Wire4 )	
	unsigned short VoltA,VoltB,VoltC;
	unsigned long AmperA;
	unsigned long AmperB;
	unsigned long AmperC;
#else
	unsigned short VoltA,VoltC;
	unsigned long AmperA;
	unsigned long AmperC;
#endif
	unsigned long AUpperValve;
	unsigned long ALowerValve;	    
	unsigned short VoltUpperValve;
	unsigned short VoltLowerValve;	    
	unsigned short i;
	unsigned char Buff[4];
	unsigned char* Ptr; 

	if( ErrRAM.RSpeAdjustPermit != 0x67 ) return;
   
    Ptr = Buff;
   
	AImax100 = MSpec.RMaxCurrent;			//最大电流(4)   //HEX 	0.0001A             
   	AIb100 = MSpec.RBaseCurrent;			//基本电流(4)	//HEX	0.0001A					
	V100Un = MSpec.RRatedVolt;				//额定电压(2)	//HEX	0.1V						

    ErrRAM.VoltAmperFtFreqState = 0;

	VoltA = BCD2_Word( Real.Ua );
	VoltC = BCD2_Word( Real.Uc );
	
	AmperA = BCD4_Long( Real.Ia );
	AmperC = BCD4_Long( Real.Ic );

#if( LinkMode == Phase3Wire4 )
	VoltB = BCD2_Word( Real.Ub );
	AmperB = BCD4_Long( Real.Ib );    
#endif

	for( i=0;i<5;i++ )
	{
		switch( i )
		{
			case 0:
				VoltUpperValve = V100Un * 117 / 100;		//115%Un
				VoltLowerValve = V100Un * 113 / 100;
				break;	                        
			case 1:                             
				VoltUpperValve = V100Un * 112 / 100;		//110%Un
				VoltLowerValve = V100Un * 108 / 100;
				break;	                        
			case 2:                             
				VoltUpperValve = V100Un * 102 / 100;		//100%Un
				VoltLowerValve = V100Un * 98  / 100;
				break;	                        
			case 3:                             
				VoltUpperValve = V100Un * 92  / 100;		//90%Un
				VoltLowerValve = V100Un * 88  / 100;
				break;	                        
			case 4:                             
				VoltUpperValve = V100Un * 82  / 100;		//80%Un
				VoltLowerValve = V100Un * 78  / 100;
				break;	
			default: break;						
		}	
		if(( VoltA >= VoltLowerValve )&&( VoltA <= VoltUpperValve )
#if( LinkMode == Phase3Wire4 )	
		&&( VoltB >= VoltLowerValve )&&( VoltB <= VoltUpperValve )
#endif
		&&( VoltC >= VoltLowerValve )&&( VoltC <= VoltUpperValve ))
		{
			ErrRAM.VoltAmperFtFreqState |= ByteBit[i];
		}		
	}	
	
	for( i=0;i<5;i++ )
	{
		switch( i )
		{
			case 0:
				AUpperValve = AImax100 * 102 / 100;			//Imax
				ALowerValve = AImax100 * 98 / 100;
				break;	
			case 1:
				AUpperValve = AIb100 * 102 / 100;			//Ib	
				ALowerValve = AIb100 * 98 / 100;
				break;	
			case 2:
				AUpperValve = AIb100 * 102 / 1000; 			//10%Ib
				ALowerValve = AIb100 * 98 / 1000;   
				break;	
			case 3:
				AUpperValve = AIb100 * 102 / 2000; 			//5%Ib
				ALowerValve = AIb100 * 98 / 2000;   
				break;	
			case 4:
				AUpperValve = AIb100 * 102 / 5000; 			//2%Ib
				ALowerValve = AIb100 * 98 / 5000;   
				break;	
			default: break;						
		}	
		if(( AmperA >= ALowerValve )&&( AmperA <= AUpperValve )
#if( LinkMode == Phase3Wire4 )	
		&&( AmperB >= ALowerValve )&&( AmperB <= AUpperValve )
#endif
		&&( AmperC >= ALowerValve )&&( AmperC <= AUpperValve ))
		{
			ErrRAM.VoltAmperFtFreqState |= ByteBit[i+5];
		}		
	}		
	
//	VoltLowerValve = BCD2_Word( Real.Pft );
	*Ptr = Real.Pft[0];
	*(Ptr+1) = Real.Pft[1];
	*(Ptr+1) &= ~0x80;
	VoltLowerValve = BCD2_Word( Ptr );
	if( VoltLowerValve >= 990 ) ErrRAM.VoltAmperFtFreqState |= Ft1_0Flag;
	if(( VoltLowerValve >= 490 )&&( VoltLowerValve <= 510 )) ErrRAM.VoltAmperFtFreqState |= Ft0_5Flag;
	
	VoltLowerValve = BCD2_Word( Real.Freq );
	if(( VoltLowerValve >= 5080 )&&( VoltLowerValve <= 5120 )) ErrRAM.VoltAmperFtFreqState |= F51HzFlag;
	if(( VoltLowerValve >= 4940 )&&( VoltLowerValve <= 5060 )) ErrRAM.VoltAmperFtFreqState |= F50HzFlag;
	if(( VoltLowerValve >= 4880 )&&( VoltLowerValve <= 4920 )) ErrRAM.VoltAmperFtFreqState |= F49HzFlag;
	
}	

void initialErrorStateVar( void )
{
	short i;
	
	for( i=0;i<(ErrorModeNum*3);i=i+3 )
	{
		*ErrorComTab[i].StateDelay = 0;	
	}	
}
	

void ErrorModeJudge( void )
{   
	short i;
	unsigned char* StateDelay;
	unsigned short ModeState;
	unsigned short CurState;
	unsigned char CurMode;
	unsigned char Mode;
	unsigned char DelayValve;
	
	if( ErrRAM.RSpeAdjustPermit != 0x67 ) return;

	CurMode = 0;
	for( i=0;i<(ErrorModeNum*3);i=i+3 )
	{
		CurState = ErrRAM.VoltAmperFtFreqState;
		StateDelay = ErrorComTab[i].StateDelay;
		ModeState = ErrorComTab[i].ModeState;
		Mode = ErrorComTab[i].Mode;
		if( Mode == SelfHeat_Mode ) 
		{
			DelayValve = SelfHeatConst;
			CurState &= ~( Ft1_0Flag + Ft0_5Flag );			//自热状态不判功率因数
		}	
		else DelayValve = 3;
		if( CurState == ModeState )	
		{
			*StateDelay += 1;
			if( *StateDelay >= DelayValve )
			{
				initialErrorStateVar();						//清除别的状态计数器	
				*StateDelay = DelayValve;
				CurMode = Mode;
				SwitchErrorMode( Mode );	
			}				
		}
		else
		{
			if( Mode == SelfHeat_Mode ) 
			{
				if( ErrRAM.RErrorMode == SelfHeat_Mode ) 
				{
					if( *StateDelay > 20 ) *StateDelay = 20;
				}
			}	

			if( *StateDelay != 0 ) *StateDelay -= 1;			
		}		
	}	

	if( CurMode == 0 )
	{
		for( i=0;i<(ErrorModeNum*3);i=i+3 )
		{
			StateDelay = ErrorComTab[i].StateDelay;
			if( *StateDelay != 0 ) break;
		}
		if( i == (ErrorModeNum*3) )	SwitchErrorMode( NormalMode );	
	}	
}

short NGetErrorComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID )
{
	short i;
	unsigned short ErrorID;
  	
  	RAM_Write( (unsigned char*)&ErrorID, ComID, 2 );
  	
  	for( i=0;i<ErrorComNum;i++ )
  	{
		if( ErrorID == ErrorComTab[i].ComID ) 
		{
			*Addr = ErrorComTab[i].Addr;
			*Info = ErrorComTab[i].ComInfo;
			*Len = *Info & RS_LEN;
			return i;
		}
  	}
	return -1;
}


//根据标准校表命令计算相应寄存器的EEPROM地址
unsigned short GetErrorAddr( unsigned short ErrorID )
{
	unsigned short Addr;

	Addr = ErrorID;	
	Addr &= 0x00ff;
#if ( MEASCHIP == ATT7022E )
//	Addr *= 3; 
	Addr = GetATT7022ECalibDataEAddr( Addr );
#elif(( MEASCHIP == IDT90E32 )&&( IDT90E32AS	== YesCheck ))
	Addr = GetIDTCalibDataEAddr( Addr ); 
#endif
	Addr += ErrorRegAddr;
	
	return Addr;	
}	

//根据块命令将标准校表数据放入相应的特殊寄存器中
short BlockInitialAdjustErrorData( unsigned short ErrorID )
{
	unsigned char 	Buff[10];
	unsigned char* 	Ptr;
	unsigned short i;
	unsigned short Start=0;
	unsigned short End=0;
	unsigned short SourceAddr;
	unsigned short Addr;
	
	Ptr = Buff;
	
	switch( ErrorID )
	{
		case 0xFFFF: Start = 0; 	End =126; break;	
#if ( MEASCHIP == ATT7022E )
		case 0xFCFF: Start = 0; 	End =48; break;	
		case 0xFDFF: Start = 48; 	End =120; break;	
		case 0xFEFF: Start = 120; 	End =126;  break;	
#elif(( MEASCHIP == IDT90E32 )&&( IDT90E32AS	== YesCheck ))
		case 0xF8FF: Start = 0; 	End =48; break;	
		case 0xF9FF: Start = 48; 	End =120; break;	
		case 0xFAFF: Start = 120; 	End =126;  break;	
#endif
		default: break;	
	}	

	for( i=Start;i<End;i++ )
	{
		Addr = ErrorComTab[i].Addr;
		SourceAddr = GetErrorAddr( ErrorComTab[i].ComID1 );	
		E2P_RData( Ptr, SourceAddr, 2 );	
		E2P_WData( Addr, Ptr, 2 );	
	}	
	
	if( End == 0 ) return -1;
	else return 0;
}

//根据命令将标准校表数据放入相应的特殊寄存器中
short InitialAdjustErrorData( unsigned short ErrorId )
{
	unsigned char 	Buff[10];
	unsigned char* 	Ptr;
	unsigned short	Addr, Info, Len;
	unsigned short	SourceAddr;
 	short ItemAds;

	Ptr = Buff;
	
	if( BlockInitialAdjustErrorData( ErrorId ) == 0 ) return 0;
	
	ItemAds = NGetErrorComVar( &Addr, &Info, &Len, (unsigned char*)&ErrorId );
	
	if( ItemAds == -1 ) return -1;
	
//	if( ErrorId == 0xF0FF ) return -1;
	if(( ErrorId == 0xF0FF )||( ErrorId == 0xF0FC )) return -1;
	
	SourceAddr = GetErrorAddr( ErrorComTab[ItemAds].ComID1 );	
	E2P_RData( Ptr, SourceAddr, 2 );	
	E2P_WData( Addr, Ptr, 2 );	
	
	return 0;		
}	

#if ( MEASCHIP == ATT7022E )
//切换误差模式时，将相应校表数据填入7022，同时修正校表数据校验和。
short SwitchErrorMode( unsigned char NewErrorMode )	
{
	unsigned short OldStart,OldEnd;
	unsigned short NewStart,NewEnd;
	unsigned short Start,End;
	unsigned long OldCheckSum;
	unsigned long NewCheckSum;
	unsigned long Value;
	unsigned char OldErrorMode;
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned short i;
	unsigned short Addr;
	unsigned short RegAds;
	
	Ptr = Buff;

	E2P_RData( (unsigned char*)&OldErrorMode, EErrorMode, 1 );	
	if( NewErrorMode == OldErrorMode ) return 0;
		
	for( i=0;i<(ErrorModeNum*3);i=i+3 )
	{
		if( OldErrorMode == ErrorComTab[i].Mode )
		{
			OldStart = i;	
			if( OldErrorMode == SelfHeat_Mode ) OldEnd = i+6;
			else OldEnd = i+3;
			break;
		}	
	}	
	if(( i == (ErrorModeNum*3) )||( OldErrorMode == NormalMode ))
	{
		OldStart = 0;
		OldEnd = 0;
	}	

	for( i=0;i<(ErrorModeNum*3);i=i+3 )
	{
		if( NewErrorMode == ErrorComTab[i].Mode )
		{
			NewStart = i;	
			if( NewErrorMode == SelfHeat_Mode ) NewEnd = i+6;
			else NewEnd = i+3;
			break;
		}	
	}	
	if(( i == (ErrorModeNum*3) )||( NewErrorMode == NormalMode ))
	{
		NewStart = 0;
		NewEnd = 0;
	}	
//-------------------------------------------------------------------------------

	Start = OldStart;
	End = OldEnd;
	RAM_Fill( Ptr, 4);
	OldCheckSum = 0;
	Value = 0;
	if( OldErrorMode == NormalMode )						//原状态为标准校表状态
	{
		Start = NewStart;
		End = NewEnd;
		for( i=Start;i<End;i++ )							//计算原校表数据校验和
		{
			Addr = GetErrorAddr( ErrorComTab[i].ComID1 );	//取新模式将要替换掉的标准校表值
			E2P_RData( Ptr, Addr, 2 );
			RAM_Write( (unsigned char*)&Value, Ptr, 2 );
			OldCheckSum += Value;
		}	
	}	
	else													//原状态为特殊状态
	{	
		if( End != 0 )
		{		
			*(Ptr+2) = 0x00;								//开读校表数据功能
			*(Ptr+1) = 0x00;
			*Ptr = 0x5A;
			ATT7022WtReg( 0xC6, Ptr );

			for( i=Start;i<End;i++ )							//计算原校表数据校验和
			{
//				Addr = ErrorComTab[i].Addr;	
//				E2P_RData( Ptr, Addr, 2 );
				Addr = ErrorComTab[i].ComID1;
				Addr &= 0x00FF;
				ATT7022Rd( Addr, Ptr, 3 );
				RAM_Write( (unsigned char*)&Value, Ptr, 2 );
				OldCheckSum += Value;
			}
			*(Ptr+2) = 0x00;								//关读校表数据功能
			*(Ptr+1) = 0x00;
			*Ptr = 0x00;
			ATT7022WtReg( 0xC6, Ptr );			
		}		
	}

//-------------------------------------------------------------------------------

	Start = NewStart;
	End = NewEnd;
	RAM_Fill( Ptr, 4);
	NewCheckSum = 0;
	Value = 0;

	*(Ptr+2) = 0x00;					//开写校表数据功能
	*(Ptr+1) = 0x00;
	*Ptr = 0x5A;
	ATT7022WtReg( 0xC9, Ptr );

	if( NewErrorMode == NormalMode )						//原状态为标准校表状态
	{
		Start = OldStart;
		End = OldEnd;
		for( i=Start;i<End;i++ )							//计算原校表数据校验和
		{
			Addr = GetErrorAddr( ErrorComTab[i].ComID1 );	//取新模式将要替换掉的标准校表值
			E2P_RData( Ptr, Addr, 2 );

			RegAds = ErrorComTab[i].ComID1;					//第一份校表数据
			RegAds &= 0x00ff;
			ATT7022WtReg( RegAds+128, Ptr );

			RAM_Write( (unsigned char*)&Value, Ptr, 2 );
			NewCheckSum += Value;
		}	
	}	
	else													//原状态为特殊状态
	{	
		for( i=Start;i<End;i++ )							//计算原校表数据校验和
		{
			Addr = ErrorComTab[i].Addr;	
			E2P_RData( Ptr, Addr, 2 );

			RegAds = ErrorComTab[i].ComID1;					//第一份校表数据	
			RegAds &= 0x00ff;
			ATT7022WtReg( RegAds+128, Ptr );

			RAM_Write( (unsigned char*)&Value, Ptr, 2 );
			NewCheckSum += Value;
		}	
	}
		
	*(Ptr+2) = 0x00;					//关闭写校表数据功能
	*(Ptr+1) = 0x00;
	*Ptr = 0x01;
	ATT7022WtReg( 0xC9, Ptr );

	E2P_RData( Ptr, AT7022ChkSum, 3 );
	Value = 0;
	RAM_Write( (unsigned char*)&Value, Ptr, 3 );
	Value += NewCheckSum - OldCheckSum;						//改变新旧校验和之差
	RAM_Write( Ptr, (unsigned char*)&Value, 3 );
    if( Data_Comp( Ptr, Para.RAT7022ChkSum, 3 ) != 0 ) 
    {
    	RAM_Write( Para.RAT7022ChkSum, Ptr,  3 );
		E2P_WData( AT7022ChkSum, Ptr, 3 );
	}	

	SM.AT7022ChkSumCnt = 2;									//ATChk

	E2P_WData( EErrorMode, (unsigned char*)&NewErrorMode, 1 );	
	ErrRAM.RErrorMode = NewErrorMode;
    
    return 0;
}	

#elif(( MEASCHIP == IDT90E32 )&&( IDT90E32AS	== YesCheck ))
//切换误差模式时，将相应校表数据填入90E32AS，同时重新计算校表数据校验和。
short SwitchErrorMode( unsigned char NewErrorMode )	
{
	unsigned short DataBuff[115];
	unsigned short OldStart,OldEnd;
	unsigned short NewStart,NewEnd;
	unsigned short Start,End;
	unsigned char OldErrorMode;
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned short i;
	unsigned short Addr;
	unsigned short RegAds;
	
	Ptr = Buff;

	E2P_RData( (unsigned char*)&OldErrorMode, EErrorMode, 1 );	
	if( NewErrorMode == OldErrorMode ) return 0;
	
	Get90E32AS_CalibData( DataBuff );
		
	for( i=0;i<(ErrorModeNum*3);i=i+3 )
	{
		if( OldErrorMode == ErrorComTab[i].Mode )
		{
			OldStart = i;	
			if( OldErrorMode == SelfHeat_Mode ) OldEnd = i+6;
			else OldEnd = i+3;
			break;
		}	
	}	
	if(( i == (ErrorModeNum*3) )||( OldErrorMode == NormalMode ))
	{
		OldStart = 0;
		OldEnd = 0;
	}	

	for( i=0;i<(ErrorModeNum*3);i=i+3 )
	{
		if( NewErrorMode == ErrorComTab[i].Mode )
		{
			NewStart = i;	
			if( NewErrorMode == SelfHeat_Mode ) NewEnd = i+6;
			else NewEnd = i+3;
			break;
		}	
	}	
	if(( i == (ErrorModeNum*3) )||( NewErrorMode == NormalMode ))
	{
		NewStart = 0;
		NewEnd = 0;
	}	

	Start = NewStart;
	End = NewEnd;
	RAM_Fill( Ptr, 4);

	*Ptr	= 0xAA;
	*(Ptr+1) = 0x55;
	IDT90E32WtReg(CfgRegAccEn,Ptr);						//Register Access Configuration Enable

	if( NewErrorMode == NormalMode )						//原状态为标准校表状态
	{
		Start = OldStart;
		End = OldEnd;
		for( i=Start;i<End;i++ )							//计算原校表数据校验和
		{
			Addr = GetErrorAddr( ErrorComTab[i].ComID1 );	//取新模式将要替换掉的标准校表值
			E2P_RData( Ptr, Addr, 2 );

			RegAds = ErrorComTab[i].ComID1;					//第一份校表数据
			RegAds &= 0x00ff;
			IDT90E32WtReg( RegAds, Ptr );	
		}	
	}	
	else													//原状态为特殊状态
	{	
		for( i=Start;i<End;i++ )							//计算原校表数据校验和
		{
			Addr = ErrorComTab[i].Addr;	
			E2P_RData( Ptr, Addr, 2 );

			RegAds = ErrorComTab[i].ComID1;					//第一份校表数据	
			RegAds &= 0x00ff;
			IDT90E32WtReg( RegAds, Ptr );	

			if( RegAds < 0x70 )
			{
				RAM_Write( (unsigned char*)&DataBuff[RegAds], Ptr, 2 );
			}	
		}	
	}
		
	*Ptr = 0x55;
	*(Ptr+1) = 0xAA;
	IDT90E32WtReg(CfgRegAccEn,Ptr);						//Register Access Configuration DisEnable

	WDTCTL = WDT_ARST_1000;					
	CRCINIRES = 0x0000;											
	for( i=0;i<0x70;i++ )						
	{										
		if( DataBuff[i] != 0 ) DataBuff[i] = CCITT( DataBuff[i] );
		CRCDI = DataBuff[i];				
	}										

	i = CRCINIRES;
	RAM_Write( Ptr, (unsigned char*)&i, 2 );
    if( Data_Comp( Ptr, Para.RAT7022ChkSum, 2 ) != 0 ) 
    {
		RAM_Write( Para.RAT7022ChkSum, Ptr, 2 );			
		E2P_WData( AT7022ChkSum, Ptr, 2 );				
	}	

	IDT90E32WtReg( CRCDigest, Ptr );	
	*Ptr	= 0x34;											//避免和复位后的值一样		//新国网	//14.01.16
	*(Ptr+1) = 0x12;
	IDT90E32WtReg(CfgRegAccEn,Ptr);							//Register Access Configuration DisEnable

	SM.AT7022ChkSumCnt = 2;									//ATChk

	E2P_WData( EErrorMode, (unsigned char*)&NewErrorMode, 1 );	
	ErrRAM.RErrorMode = NewErrorMode;
    
    return 0;
}	

#endif

unsigned char TestComAdjWrite(unsigned char* ComBuf )
{
	unsigned short	Addr, Info, Len;
  	unsigned char Err_Byte=0;
	short ItemAds;
//	short Temp;
    short Temp2;		
	unsigned char Buff[4]; 
	unsigned char* Point;
	unsigned short ComID;

    Point = Buff;
//	Temp = *(ComBuf+Rs_IDLPtr);
	
	ItemAds = NGetErrorComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
	if( ItemAds == -1 )
	{
		Err_Byte |= RS_State_IVID;		
		return	Err_Byte;			
	}	
	
	*(Point+2) = Len+6;
	if( *(ComBuf+Rs_LenPtr) != *(Point+2) )
	{
		Err_Byte |= RS_State_IVData;		
		return	Err_Byte;					
	}	
	
	RAM_Write( (unsigned char*)&ComID, ComBuf+NRs_ID0Ptr, 2 );

	if( ComID == 0xF0FF )						//将标准校表数据搬入特殊校表寄存器	
	{
		RAM_Write( (unsigned char*)&Temp2, ComBuf+Rs_WDataPtr, 2 );
		if( InitialAdjustErrorData( Temp2 ) != 0 ) Err_Byte = RS_State_IVID;
		return	Err_Byte;				
	}	

	if( ComID == 0xF0FC )						//手工进入特殊校表状态
	{
		ItemAds = ManuEnterSpeState( *(ComBuf+Rs_WDataPtr) );
		if( ItemAds != 0 )
		{
			Err_Byte |= RS_State_IVID;		
		}	
		return	Err_Byte;			
	}	
	
       	E2P_WData( Addr, ComBuf+Rs_WDataPtr, 2 );		//10.07.30
       	E2P_RData( Point, Addr, 2 );		//10.07.30
		if( Data_Comp( Point, ComBuf+Rs_WDataPtr, 2 ) != 0 ) return RS_State_IVData;		//10.07.30


	return 0;
}	


unsigned char Spe_Com_Read(unsigned char* ComBuf)
{
	unsigned short 	Addr, Info, Len;
  	unsigned char Err_Byte=0;
	short ItemAds;

	Addr = (short)*(ComBuf+Rs_IDHPtr)*256+ (short)*(ComBuf+Rs_IDLPtr);

	ItemAds = NGetErrorComVar( &Addr, &Info, &Len, ComBuf+NRs_ID0Ptr );
	if( ItemAds == -1 )
	{
		Err_Byte |= RS_State_IVID;		
		return	Err_Byte;			
	}	

	if( ~Info & READ ) 
	{
		Err_Byte |= RS_State_IVData;
		return	Err_Byte;
	}
	*(ComBuf+Rs_LenPtr) = 2;
	Spe_Proc_Read( ItemAds, Addr, Info, Len, ComBuf );
	return Err_Byte;
}

void Spe_Proc_Read( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf )
{	
//	unsigned short i,Length,ComID;
	unsigned char* ReadBufAds;
	
	ReadBufAds =  ComBuf + Rs_IDLPtr + *(ComBuf + Rs_LenPtr);
//	ComID = ComTab[ ItemAds ].ComID;
	
	switch( Info & Comm_Type )
	{
		case PARAM:
			E2P_RData( ReadBufAds, Addr, Len);
		   	*(ComBuf + Rs_LenPtr) += Len;
			break;
		default:
			break;	
	}	
}	
	
short ManuEnterSpeState( unsigned char State )
{
	short Err_Byte = 0;

//	if( State > OverCuIbMode ) return -1;
	if( State > SelfHeat_Mode ) return -1;

	switch( State )
	{
		case NormalMode: 	
			initialErrorStateVar();
//			ErrRAM.OverCu10IbExitDelay = 0;
			SwitchErrorMode( NormalMode );
			break;	
		default: 
			Err_Byte = -1;
			break;	
	}	

	return Err_Byte; 
}	
	
#endif	