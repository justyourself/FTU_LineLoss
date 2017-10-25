#include "TypeE2p.h"
#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
//#include "Measure.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "TypeFRAM.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Port.h"
#include "Display.h"
#include "RsComm.h"
#include "Initial.h"
#include "Time.h"
#include "Measure.h"
#include "Power.h"
#include "FreezeData.h"
#include "math.h"					//SmallCurrent
#include "cpucard.h"
#include "YearSave.h"									//新国网	//13.11.29

#if ( LocalSmartMeter == YesCheck )
short JudgeYearSaveDay( unsigned char* Source )					//新国网	//13.11.28
{
	if(( AllIsBCD( Source, 3 ) != 0 )||( JudgeDayH( Source ) != 0 )||( *( Source+2 ) == 0 )||( *( Source+2 ) > 0x12 ))	//日期非法
	{
		return 1;	
	}	
	else return 0;	
}	

unsigned char GetYearMonSaveMode( unsigned char* YearSaveDay )	//新国网	//13.11.28	
{
	short i;
	unsigned char YearMonSaveMode;
	
	YearMonSaveMode = 0x00;									//月结算
	for( i=0;i<4;i++ )
	{
		if( JudgeYearSaveDay( YearSaveDay + i*3 ) == 0 )
		{
			YearMonSaveMode = 0x55;							//年结算	
			break;	
		}	
	}	
	return YearMonSaveMode;
}	

void MonSaveBarely( void )												//新国网	//13.11.29
{
	unsigned char Buff[4];
	unsigned char* SaveTime;
	
	SaveTime = Buff;

	RAM_Write( SaveTime, Clk.Hour, 4 );	
	E2P_WData( LastSaveTime, SaveTime, 4 );								//保存上次年结算时间为当前时间
//	ECSave();
	RecordFreeze( SaveDayFreeze_No, DIRECT );			//结算日冻结
	MD_Rest(0);															//需量第一次结算需保存当前需量
//	CreateMonthSaveTimeRec( MonthSaveTime_No, SaveTime );	
	RecordFreeze( StepSaveSWFreeze_No, DIRECT );						//阶梯结算冻结
	MoveSaveDay1EC();													//保存当前和上月结算电量					
}

void YearSaveBarely( void )												//新国网	//13.11.29
{
	unsigned char Buff[4];
	unsigned char* SaveTime;
	
	SaveTime = Buff;
	
	RAM_Write( SaveTime, Clk.Hour, 4 );	
	E2P_WData( LastYearSaveTime, SaveTime, 4 );							//保存上次年结算时间为当前时间
//	CreateYearSaveTimeRec( YearSaveTime_No, SaveTime );			
	RecordFreeze( StepSaveSWFreeze_No, DIRECT );						//阶梯结算冻结
	MoveYearSaveDayEC();												//清零当前年结算周期电量
}	

void YearMonSaveDaySwitchProcess( unsigned char* OldData, unsigned char* NewData )		//新国网	//13.11.28	
{
	unsigned char OldYearMonSaveMode;
	unsigned char NewYearMonSaveMode;
	
	OldYearMonSaveMode = GetYearMonSaveMode( OldData );	
	NewYearMonSaveMode = GetYearMonSaveMode( NewData );	
	ICComm.YearMonSaveMode = NewYearMonSaveMode;
	
	if( OldYearMonSaveMode == NewYearMonSaveMode )						//切换前后阶梯类型不变
	{
		if( NewYearMonSaveMode	== 0x00 ) return;						//切换前后均为月阶梯
		else															//切换前后均为年阶梯
		{
			if( Data_Comp( OldData, NewData, 12 ) == 0 ) return;		//切换后年结算日不变
			else														//切换前后年结算日不一致，	
			{
				YearSaveBarely();										//进行年结算。			
			}				
		}	
	}
	else if( OldYearMonSaveMode == 0x00 )								//切换前为月阶梯、切换后为年阶梯
	{
		MonSaveBarely();												//保存当前和上月结算电量,同时转存12个月电量
		MoveYearSaveDayEC();											//清零当前年结算周期电量
	}
	else																//切换前为年阶梯、切换后为月阶梯	
	{
		YearSaveBarely();												//进行年结算。
	}		
}	

void YearDec( unsigned char *Data )								//新国网	//13.11.28
{
	unsigned char Temp;

	Temp = BCD_Byte( *Data );		

	if( Temp == 0x00 ) Temp = 99;			//00年
	else Temp -= 1;							//年减1	
	
	*Data = Byte_BCD( Temp );		

	return;
}

void YearInc( unsigned char *Data )								//新国网	//13.11.28
{
	unsigned char Temp;

	Temp = BCD_Byte( *Data );		

	Temp += 1;
	if( Temp >= 100 ) Temp = 0;
	*Data = Byte_BCD( Temp );		

	return;
}

//刷新上次年结算时间
void UpdateLastYearSaveTime( unsigned char* YearSaveDayBuff )			//新国网	//13.11.28
{
	unsigned char Buff[100];
	unsigned char Buff1[4];
	unsigned char Buff2[4];
	unsigned char Buff3[4];
	unsigned char* LastTime;
	unsigned char* TestTime;
	unsigned char* CurTime;
	unsigned char* SaveTime;
	unsigned char* Ptr;	
	unsigned short i,j;
	unsigned char SaveDayBuff[12];
	unsigned char YearSaveCnt;
	unsigned char Year;
	
	LastTime = Buff1;
	CurTime = Buff2;
	SaveTime = Buff3;
	
	Ptr = Buff;
	
	E2P_RData( LastTime, LastYearSaveTime, 4 );						//上次年结算时间
	RAM_Write( CurTime, Clk.Hour, 4 );								//当前时间
	
	if( DateTime4ValueCheck( CurTime ) != 0 ) return;
	
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  
	{
		RAM_Write( LastTime, CurTime, 4 );							//当前时间若小于上次结算时间，则上次结算时间=当前时间
		E2P_WData( LastYearSaveTime, LastTime, 4 );							
		return;				
	}
	
	RAM_Write( SaveDayBuff, YearSaveDayBuff, 12 );

	RAM_Fill( Ptr, 100 );
	Year = Clk.Year[0];
	for( i=0;i<5;i++ )												//取得最近5年的结算点时间	
	{
		for( j=0;j<4;j++ )
		{
			RAM_Write( Ptr+(4-i)*16+j*4, SaveDayBuff+j*3, 3 );		//每年4个结算时间					//15.05.15
			*( Ptr+(4-i)*16+j*4+3 ) = Year;							//当前及上5年的结算时间				//15.05.15
		}	
		YearDec( (unsigned char*)&Year );							//倒推1年		
	}	
	
	YearSaveCnt = 0;
	for( i=0;i<20;i++ )
	{
		TestTime = Ptr+(19-i)*4;									//从当前年的第四个年结算日开始倒推	
		if( DateTime4ValueCheck( TestTime ) != 0 ) continue;		//测试区内的时间非法，主要用于某一结算日不采用时 	//是用9999或FFFF填充的。跳过
		if( Data_Comp( TestTime, CurTime, 4 ) > 0 ) continue;		//测试区内的时间大于当前时间，即未来时间，不参与结算判断
		
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 )				//测试区内的时间大于上次结算时间，是需要结算的时间点
		{
			RAM_Write( SaveTime, TestTime, 4 ); 					//保留该时间点	
			YearSaveCnt += 1;
			if( YearSaveCnt >= 4 ) break;							//最多补上4次结算
		}
		else break;													//属于已经结算过的时间了
	}
	
	if( YearSaveCnt != 0 )											
	{
		if( *SaveTime == 0 )
		{
			DayDec( SaveTime+1 ); 					
			*SaveTime = 0x23; 										//13.10.06
		}	
		else *SaveTime = Byte_BCD( BCD_Byte(*SaveTime)-1 );			//把上次结算时间保存为比测到的最早的结算点早一个小时，以有效进行补结算
		if( Data_Comp( SaveTime, LastTime, 4 ) == 1 )				//推算的最早结算点若晚于上次结算时间，把推算时间作为上次结算时间。
		{
			E2P_WData( LastYearSaveTime, SaveTime, 4 );
		}	
	}		
}

/*
void CreateYearSaveTimeRec( unsigned short EventNo, unsigned char* SaveTime )			//新国网	//13.11.28
{
	unsigned char Buff[sizeof(YEARSAVETIME_REC)];
	YEARSAVETIME_REC* RecPtr;
	long LastPt;
	
	RecPtr = (YEARSAVETIME_REC*)Buff;

	LastPt = PtAddrChk( EventNo, Next );			
	if( LastPt == -1 ) return;
	
	RAM_Write( RecPtr->HappenDate, SaveTime, 4 );
	GetYearCycleInsideEC( RecPtr->StaECPp0, 0, 1 );
	E2P_WData( (unsigned short)LastPt, RecPtr->HappenDate, sizeof(YEARSAVETIME_REC ));
}
*/

//YearSaveDayBuff: 存放四个年结算日首址
//YearSaveCnt：允许结算次数
short AdjustYear( unsigned char* YearSaveDayBuff, short* YearSaveCnt )					//新国网	//13.11.28
{
	unsigned char Buff[32];
	unsigned char Buff1[4];
	unsigned char Buff2[4];
	unsigned char* LastTime;
	unsigned char* TestTime;
	unsigned char* CurTime;
	unsigned char* SixSaveDay;	
	unsigned short i;
	
	LastTime = Buff1;
	CurTime = Buff2;
	
	SixSaveDay = Buff;
	
	E2P_RData( LastTime, LastYearSaveTime, 4 );							//上次结算时间
	RAM_Write( CurTime, Clk.Hour, 4 );								//当前时间
	
	if( DateTime4ValueCheck( CurTime ) != 0 ) 
	{
		*YearSaveCnt = 0;											//当前时间非法，不结算。		
		return 0;				
	}	
	
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  
	{
		RAM_Write( LastTime, CurTime, 4 );							//当前时间若小于上次结算时间，则上次结算时间=当前时间
		E2P_WData( LastYearSaveTime, LastTime, 4 );							
		*YearSaveCnt = 0;											
		return 0;				
	}	

	for( i=0;i<8;i++ )
	{
		RAM_Write( SixSaveDay+3+i*4, LastTime+3, 1 );				//上次年结算时间的年放入测试区
	}	

	for( i=0;i<4;i++ )
	{
		RAM_Write( SixSaveDay+i*4, YearSaveDayBuff+i*3, 3 );		//上次结算时间的年月和三个结算日组成前三个测试区
		RAM_Write( SixSaveDay+16+i*4, YearSaveDayBuff+i*3, 3 );		//上次结算时间的年月的下一个月和三个结算日组成后三个测试区
		YearInc(SixSaveDay+19+i*4);
	}	
	
	TestTime = SixSaveDay;
	for( i=0;i<8;i++ )
	{
		if( DateTime4ValueCheck( TestTime ) != 0 ) 					//测试区内的时间非法，主要用于某一结算日不采用时
		{															//是用9999或FFFF填充的。跳过
			TestTime += 4;
			continue;	
		}	
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 )				//测试区内的时间大于上次结算时间
		{
			if( Data_Comp( CurTime, TestTime, 4 ) == -1 ) break;	//当前时间若小于测试时间，则不进行结算
			else 
			{
//				CreateYearSaveTimeRec( YearSaveTime_No, TestTime );	//新国网				
				RecordFreeze( StepSaveSWFreeze_No, DIRECT );		//阶梯结算冻结
				if( *YearSaveCnt != 0 )
				{
					*YearSaveCnt -= 1;
					if( *YearSaveCnt == 0 ) 						//最多连续年结算4次，4次后，将当前时间存入上次结算时间
					{												//以终止结算。	
						RAM_Write( TestTime, CurTime, 4 ); 
					}
				}	
				E2P_WData( LastYearSaveTime, TestTime, 4 );			//结算，该测试区内的时间作为上次结算时间	
				return (i%4)+1;										//返回1：第一年结算日、2：第二年结算日、3：第三年结算日、3：第三年结算日
			}	
		}	
		TestTime += 4;
	}	
	*YearSaveCnt = 0;												//不符合结算条件。		
	return 0;	
}

void MoveYearSaveDayEC( void )											//新国网	//13.11.28
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	
	Ptr = Buff;
	RAM_Fill( Ptr, 6 );
	
	E2P_RData( Ptr, L1_YearSaveDayEC_Pp0, 4 );
	E2P_WData( L2_YearSaveDayEC_Pp0, Ptr, 4 );
	E2P_RData( Ptr, L1_YearSaveDayEC_Pn0, 4 );
	E2P_WData( L2_YearSaveDayEC_Pn0, Ptr, 4 );
	Read_CMonEC4( Ptr, CMon_EC_Pp0 );
	E2P_WData( L1_YearSaveDayEC_Pp0, Ptr, 4 );
	Read_CMonEC4( Ptr, CMon_EC_Pn0 );
	E2P_WData( L1_YearSaveDayEC_Pn0, Ptr, 4 );
	
}	

unsigned char GetCurYearMonSaveMode( unsigned char* Dest )						//新国网	//13.11.29
{
	short i;
	unsigned char YearMonSaveMode;
	
	for( i=0;i<4;i++ )
	{
		E2P_RData( Dest+i*3, Case1YearSaveDay1+i*4, 3 );		
	}	
	YearMonSaveMode = GetYearMonSaveMode( Dest );
	
	return YearMonSaveMode;
}	

short Year_Save(void)															//新国网	//13.11.28
{
	short i,k;
	unsigned char Buff[12];
	short Year_SaveCnt;
	
	ICComm.YearMonSaveMode = GetCurYearMonSaveMode( Buff );
	if( ICComm.YearMonSaveMode == 0 ) return 0;
	
	UpdateLastYearSaveTime( Buff );											

	Year_SaveCnt = 4;														//最多允许年结算4次
	for( k=0;k<4;k++ )														//最多允许年结算4次
	{	
		if((Flag.Power & F_PwrUp) == 0 ) break;								//检测到掉电后不再判断结算
		i = AdjustYear( Buff, &Year_SaveCnt );
		if( i != 0 )
		{
			MoveYearSaveDayEC();											
		} 	
		if( Year_SaveCnt == 0 ) break;
	}

	return 0;
}

#endif

































