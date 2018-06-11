#include "TypeMeter.h"
#include "TypeRAM.h"
#include "TypeE2p.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "General.h"
#include "Initial.h"
//#include "RsComm.h"
#include "Display.h"
#include "Time.h"
#include "Port.h"
#include "Power.h"

void Temperature(void)
{
    return;
}  

void GetICTime()
{
	unsigned char Buff[8];
	unsigned char* Point;
		
	Point = Buff;
	
	__disable_irq();//_DINT();
	*Point	   = Clk.Sec[0];	
	*(Point+1) = Clk.Min[0];	
	*(Point+2) = Clk.Hour[0];	
	*(Point+3) = Clk.Week[0];	
	*(Point+4) = Clk.Day[0];	
	*(Point+5) = Clk.Mon[0];	
	*(Point+6) = Clk.Year[0];		
	__enable_irq();//_EINT();
	if(( TimeValueCheck( Point ) != 0 )||( DateValueCheck( Point+3 ) != 0 ))
	{
		GetTime();
	}	 	
}	

void GetTime( void )
{
  unsigned char Buff[8];
  unsigned char* Point;
  Point = Buff;
  HT_RTC_Read(Point);
  Clk.Sec[0] = Byte_BCD(*(Point+6));	
  Clk.Min[0] = Byte_BCD(*(Point+5));	
  Clk.Hour[0] = Byte_BCD(*(Point+4));	
  Clk.Day[0] = Byte_BCD(*(Point+3));	
  Clk.Mon[0] = Byte_BCD(*(Point+2));	
  Clk.Year[0] = Byte_BCD(*(Point+1));		
  Clk.Week[0] = Byte_BCD(*Point);
}	
void MoveTimeBCD_Hex( unsigned char *Dest, unsigned char *source )
{
	unsigned short Temp;
	
//	*Dest = BCD_Byte( *source ); 
	Temp = BCD_Byte( *(source + 4));
	Temp += 2000;
	*Dest = Temp/256;	//yearH
	*(Dest + 1) = Temp%256;  //yearL
	*(Dest + 2) = BCD_Byte( *(source + 3) );//month
	*(Dest + 3) = BCD_Byte( *(source + 2) );//day
	*(Dest + 4) = 0;//week暂时不管，值考虑年月日时分
	*(Dest + 5) = BCD_Byte( *(source + 1) );//hour
	*(Dest + 6) = BCD_Byte( *(source ) );//min
	RAM_Fill(Dest + 7,5);	//剩余都填充0
}

void MoveCurrentTimeBCD_Hex( void )
{
	unsigned short Temp;
	
	Temp = BCD_Byte( Clk.Year[0] );
	Temp += 2000;
	Clk.YearH = Temp/256;	
	Clk.YearL = Temp%256;  
	Clk.Month = BCD_Byte( Clk.Mon[0] );  
	Clk.DayH = BCD_Byte( Clk.Day[0] );   
	Clk.WeekH = BCD_Byte( Clk.Week[0] );   
	Clk.HourH = BCD_Byte( Clk.Hour[0] );    
	Clk.MinH  = BCD_Byte( Clk.Min[0] );     
	Clk.SecH  = BCD_Byte( Clk.Sec[0] );  
	Clk.SecPer = 0xFF;  
	Clk.DeviatH = 0x80;
	Clk.DeviatL= 0x00;
	Clk.State= 0x00;
}	

void MoveCurrentTimeHex_BCD( void )
{
	unsigned short Temp;
	 
	Clk.Sec[0] = Byte_BCD( Clk.SecH );	
	Clk.Min[0] = Byte_BCD( Clk.MinH );		
	Clk.Hour[0] = Byte_BCD( Clk.HourH );		
	Clk.Day[0] = Byte_BCD( Clk.DayH );		
	Clk.Mon[0] = Byte_BCD( Clk.Month );		
	Temp = Clk.YearH;
	Temp *= 256;
	Temp += Clk.YearL;
	Clk.Year[0] = Byte_BCD( Temp%100 );			
//	Clk.Week[0] = Clk.WeekH;
}	

void ChangeTimeBCDToHex( unsigned char* Dest, unsigned char* Source )
{
	unsigned short Temp;
	 
	Temp = BCD_Byte( *(Source+5) );
	Temp += 2000;
	*(Dest+6) = Temp/256;	
	*(Dest+5) = Temp%256;  
	*(Dest+4) = BCD_Byte( *(Source+4) );  
	*(Dest+3) = BCD_Byte( *(Source+3) );   
	*(Dest+2) = BCD_Byte( *(Source+2) );    
	*(Dest+1)  = BCD_Byte( *(Source+1) );     
	*Dest  = BCD_Byte( *Source );
		
}	

void ChangeTimeHexToBCD( unsigned char* Dest, unsigned char* Source )
{
	unsigned short Temp;
	 
	*Dest = Byte_BCD( *Source );	
	*(Dest+1) = Byte_BCD( *(Source+1) );		
	*(Dest+2) = Byte_BCD( *(Source+2) );		
	*(Dest+3) = Byte_BCD( *(Source+3) );		
	*(Dest+4) = Byte_BCD( *(Source+4) );		
	Temp = *(Source+6);
	Temp *= 256;
	Temp += *(Source+5);
	*(Dest+5) = Byte_BCD( Temp%100 );			
}	


short TimeValueCheck( unsigned char* Source )
{
	short i;
	
	for( i=0;i<3;i++ )
	{
		if( IsBCD( *(Source+i) ) != 0 ) return 1;
		if( *(Source+i) > TimeMaxTab[i] ) return 1;	
	}	 
	return 0;
}	

short DateValueCheck( unsigned char* Source )
{
	short i;
	
	for( i=0;i<4;i++ )
	{
		if( IsBCD( *(Source+i) ) != 0 ) return 1;
		if( *(Source+i) > DateMaxTab[i] ) return 1;	
	}	 
	if(( *(Source+1) == 0 )||(*(Source+2) == 0)) return 1;
//	if( ((*(Source+3) & 0x03 )== 0)&&(*(Source+2) == 2)&&(*(Source+1) == 0x29)) return 0;	//闰年2月29日	
	if( ((BCD_Byte(*(Source+3)) & 0x03 )== 0)&&(*(Source+2) == 2)&&(*(Source+1) == 0x29)) return 0;	//闰年2月29日	//V40
	if( BCD_Byte(*(Source+1)) > MonTab[BCD_Byte(*(Source+2))] ) return 1; //判大月小月
	return 0;
}	

short DateTime5ValueCheck( unsigned char* Source )
{
	unsigned char Buff[8];
	unsigned char* Point;

	Point = Buff;
	*Point = 0;
	RAM_Write( Point+1, Source, 2 );	
	*(Point+3) = 0;
	RAM_Write( Point+4, Source+2, 3 );	
	if(( TimeValueCheck( Point ) != 0 )||( DateValueCheck( Point+3 ) != 0 ))
	{
		return 1;					//非BCD码返回错误
	}
	else return 0;
}	

short DateTime4ValueCheck( unsigned char* Source )
{
	unsigned char Buff[8];
	unsigned char* Point;

	Point = Buff;
	
	Point = Buff;
	*Point = 0;
	*(Point+1) = 0;
	*(Point+2) = *Source;
	*(Point+3) = 0;
	RAM_Write( Point+4, Source+1, 3 );	
	if(( TimeValueCheck( Point ) != 0 )||( DateValueCheck( Point+3 ) != 0 ))
	{
		return 1;					//非BCD码返回错误
	}
	else return 0;

}	

short DateTime6ValueCheck( unsigned char* Source )
{
	unsigned char Buff[8];
	unsigned char* Point;

	Point = Buff;
	RAM_Write( Point, Source, 3 );	
	*(Point+3) = 0;
	RAM_Write( Point+4, Source+3, 3 );	
	if(( TimeValueCheck( Point ) != 0 )||( DateValueCheck( Point+3 ) != 0 ))
	{
		return 1;					//非BCD码返回错误
	}
	else return 0;
}

//检测date_time_s合法性
short Check_date_time_s( unsigned char* Source )	//17.04.17
{
	unsigned char Buff[7];
	unsigned char* Ptr;
	
	Ptr = Buff;
	 
	RAM_Write( Ptr, Source, 7 );
	SwapData( Ptr, 7 );
	ChangeTimeHexToBCD( Ptr, Ptr );
	return DateTime6ValueCheck( Ptr );
}

/*-----------------------------------------------

2000年~2099年星期算法
--------------------------------------------------------*/
unsigned char WeekDay20( unsigned char* Addr )
{
	unsigned char week, day,d,m,y; 
	
	d = BCD_Byte(*Addr);
	m = BCD_Byte(*(Addr+1));
	y = BCD_Byte(*(Addr+2));
	
	week = m - 1;
	day = WeekTab[week];			//月表   
	week = day >> 5;				//月星期数 
	day &= 0x1f;					//月天数 
	if ((m < 3) && (y & 0x03))
	{								//平年   
		if (m == 2) day--;			//平年月天数   
		week++;						//平年月表+1  
	}  
	y = y + (y >> 2);				//年+年/4 
	week = (week +  y + d + 2) % 7;	//(星期=年+年/4+月表+2日)%7 
	return week;					//返回星期
}
/*
short JustWeek( unsigned char* Source )
{	
	unsigned char Week;
	
	Week = *Source;
	if( Week == 7 ) Week = 0;
	if( Week == WeekDay20( Source+1 ) ) return 0;
	else return 1;
}	
*/
/*
short TimeValueCheck( unsigned char* Source )
{
	short i;
	
	for( i=0;i<7;i++ )
	{
		if( *(Source+i) > TimeMaxTab[i] ) return 1;	
	}	 
	return 0;
}	

void ClkChk(void)
{
	unsigned char Buff[12];
	unsigned char* Point;
	short i;

	Point = Buff;

	//RAM_Fill( Point, 12, 0);
	RAM_Fill( Point, 12 );
	_DINT();
	if((( Flag.Clk & F_Min ) == 0 )&&( Clk.Check != ChkNum( Clk.Min , 6 ))) 		
	{
		E2P_RData( Point, ClkBuf1, 6 );
		for( i=1;i<4;i++ )
		{
			E2P_RData( Point+6, ClkBuf1 + i*7, 6 );
			if( Data_Comp( Point+6, Point, 6 ) > 0 )
				RAM_Write( Point, Point+6, 6 );
			else break;
		}		
		RAM_Write( &Clk.Min[0], Point, 6 );
		Clk.Check = ChkNum( (unsigned char*)&Clk.Min[0] , 6 );		
	}	
	_EINT();
}

void SetClkBak( void )
{
	short Temp;
	
	Clk.Check = ChkNum( (unsigned char*)&Clk.Min[0] , 6 );
	
	if((Flag.Power & F_PwrUp) != 0)
	{
		Temp = (( BCD_Byte( Clk.Min[0] ) & 0x03 ) * 7 ) + ClkBuf1  ;
		E2P_WData( Temp, (unsigned char*)&Clk.Min[0], 6 );
	}	
}	
*/
unsigned long GetSec( unsigned char* Addr )
{
	return (unsigned long)BCD_Byte(*Addr) 
		+ (unsigned long)BCD_Byte(*(Addr+1))*60
		+ (unsigned long)BCD_Byte(*(Addr+2))*3600;
}


/**********************************************************************/
// Change YYMMDD to Bill Month
/**********************************************************************/
/*
unsigned short GetBillMon( char* Addr )
{
   unsigned short Tmp;

   Tmp=BCD_Byte(*(Addr+1))+12*BCD_Byte(*(Addr+2));
   if( Para.ZeroDT[0]>0 && Para.ZeroDT[0]<=0x31 && *Addr>=Para.ZeroDT[0] ) Tmp++;   
   return Tmp;
}
*/
/**********************************************************************/
// Change YYMMDD to GmDay
/**********************************************************************/

unsigned short GetGmDay( unsigned char* Addr )
{
   	unsigned short Day,Mon,Year;

	Mon = BCD_Byte(*(Addr+1));
	Year= BCD_Byte(*(Addr+2));
	Day = BCD_Byte(*Addr) + DayTab[Mon] + Year*365 + (Year>>2) + 1;
	if( (Year&0x03)==0 && Mon<=2 ) Day--;
   	return Day;
}

/**********************************************************************
	Change YYMMDDhhmmss to GmSec
**********************************************************************/
/*
unsigned long GetGmSec( char* Addr )
{
	return (unsigned long)BCD_Byte(*Addr) 
		+ (unsigned long)BCD_Byte(*(Addr+1))*60
		+ (unsigned long)BCD_Byte(*(Addr+2))*60*60
		+ (unsigned long)GetGmDay(Addr+3)*60*60*24;
}
*/
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
//#if ( ModifyTimeSkipDelay == YesCheck )						//新国网	//14.06.19
/**********************************************************************
	Change YYMMDDhhmmss to GmSec
**********************************************************************/
unsigned long GetGmSec( unsigned char* Addr )
{
	return (unsigned long)BCD_Byte(*Addr) 
		+ (unsigned long)BCD_Byte(*(Addr+1))*60
		+ (unsigned long)BCD_Byte(*(Addr+2))*60*60
		+ (unsigned long)GetGmDay(Addr+3)*60*60*24;
}
//#endif
#endif
/**********************************************************************
	Change YYMMDDhhmmss to GmMin
**********************************************************************/

unsigned long GetGmMin( unsigned char* Addr )
{
	return (unsigned long)BCD_Byte(*Addr) 
		+ (unsigned long)BCD_Byte(*(Addr+1))*60
		+ (unsigned long)GetGmDay(Addr+2)*60*24;
}

/**********************************************************************
	Get the Week of Today
**********************************************************************/
/*char GetWeek( void )
{
   return ( GetGmDay( Clk.Day ) + 5 ) % 7;
}

*/
//**********************************************************************
// Change GmMin to YYMMDDhhmm
//**********************************************************************
/*
void Min_Date( char* Sec, unsigned long Min )
{
	unsigned long Tmp;
	short Y1,Y2;
	
	*(Sec+1) = Min % 60;				// Minute
	Tmp = Min / 60;
	*(Sec+2) = Tmp % 24;				// Hour
	Tmp /= 24;
	
	Y1 = Tmp / (365*4+1);
	Y2 = Tmp % (365*4+1);
	if( Y2 > 366 ){
	 	*(Sec+5) = Y1*4 + (Y2-1)/365;	// Year
	 	Y2 = (Y2-1)%365;
	 }
	else *(Sec+5) = Y1*4;
	for(Y1=13;Y1>0;Y1--) 
		if((Y2-DayTab[Y1])>0) break;
	*(Sec+4) = Y1-1;					// Month
	*(Sec+3) = Y2 - 1 - DayTab[Y1];		// Day
}
*/
/**********************************************************************
	Get the Fee rate Number
**********************************************************************/
/*
#define  MaxHoli	100
#define  MaxSeas	8
#define  MaxSeg	12

char GetFeeNo( void )
{
	unsigned short	Now, Int;
   char 	i, TabNo=12;
   char 	Num;
   
   Num = BCD_Byte( (char)FeeGrp.HoliNum );
   if( Num > MaxHoli ) Num = MaxHoli;
   for( i=0; i<Num; i++ ) {
      if( FeeGrp.Holi[i][0]==(char)Clk.Day && FeeGrp.Holi[i][1]==(char)Clk.Mon ) {
         TabNo = FeeGrp.Holi[i][2];
         break;
      }
   }
	
	if( TabNo>=12 ){
	   Num = (char)FeeGrp.SeasNum;
	   if( Num > MaxSeas ) Num = MaxSeas;
      ((char*)&Now)[0] = (char)Clk.Day;
      ((char*)&Now)[1] = (char)Clk.Mon;
	   for( i=0; i<Num; i++ ) {
         ((char*)&Int)[0] = (char)FeeGrp.Seas[i][0];
         ((char*)&Int)[1] = (char)FeeGrp.Seas[i][1];
         if( Now<Int ) break;
		}	   	
		if( i==0 || i>=Num ) i = Num-1;
		else i = i-1;
		TabNo = FeeGrp.Seas[i][2];
	}
	if((TabNo<8) && (FeeGrp.WeekEnd & ByteBit[(char)Clk.Week])) TabNo = (char)FeeGrp.Week[TabNo];
	if( TabNo<8 ) {
		Num = BCD_Byte( (char)FeeGrp.SegNum[TabNo] );	      
		if( Num>MaxSeg ) Num = MaxSeg;
	   ((char*)&Now)[0]=(char)Clk.Min;
  		((char*)&Now)[1]=(char)Clk.Hour;
		for( i=0; i<Num; i++ ){
      	((char*)&Int)[0]=FeeGrp.Seg[TabNo][i][0];
  	   	((char*)&Int)[1]=FeeGrp.Seg[TabNo][i][1];
  	   	if( Now<Int ) break;
  		}
		if( i==0 || i>=Num ) i = Num-1;
		else i = i-1;
		return ( FeeGrp.Seg[TabNo][i][2] & 3 );
	}
	return ( (TabNo-8) & 3 );
}

*/

//日期时间加分钟
//Source: 源时间地址
//Dest: 目标时间地址
//Interval:增加的分钟数
void DateTimeAddMin( unsigned char* Dest, unsigned char* Source, unsigned short Interval )
{
	unsigned short TempInt1;
	unsigned short TempInt2;
	unsigned char Temp;
	unsigned char Temp2;

	if( Interval == 0 ) return;
	
	RAM_Write( Dest, Source, 5 );	
	TempInt1 = Interval;
	while( TempInt1 >= 1440 )
	{
		DayInc( Dest+2 );	
		TempInt1 -= 1440;
	}	
	if( TempInt1 >= 60 )
	{
		TempInt2 = TempInt1 / 60;
//		TempInt1 = TempInt2 % 60;	
		TempInt1 = TempInt1 % 60;				//16.01.22
		Temp = BCD_Byte( *(Dest+1) );
		Temp += TempInt2;
		if( Temp >= 24 )
		{
			DayInc( Dest+2 );
			Temp -= 24;
		}	 
		*(Dest+1) = Byte_BCD( Temp );		
	}	
	Temp = BCD_Byte( *Dest );
	Temp += TempInt1;
	if( Temp >= 60 )
	{
		Temp -= 60;
		Temp2 = BCD_Byte( *(Dest+1) );
		Temp2 += 1;
		if( Temp2 >= 24 )
		{
			DayInc( Dest+2 );
			Temp2 -= 24;
		}	 
		*(Dest+1) = Byte_BCD( Temp2 );		
	}	 
	*Dest = Byte_BCD( Temp );			
	
	return;
}	

void YearInc_Hex( unsigned char* YearL, unsigned short Interval )
{
	unsigned short Year;
	
	RAM_Write( (unsigned char*)&Year, YearL, 2 );
	Year += Interval;
	RAM_Write( YearL, (unsigned char*)&Year, 2 );
}	

void MonInc_Hex( unsigned char* MonL, unsigned short Interval )
{
	unsigned short Mon;
	unsigned short Year;
	
	Mon = (*MonL);
	Mon += Interval; 
	if( Mon >= 12 )
	{
		Year = Mon / 12;
		YearInc_Hex( MonL+1, Year );	
		*MonL = Year = Mon % 12;
 	}
 	else *MonL = Year;	
}	

void DayInc_Hex( unsigned char* DayL, unsigned short Interval )
{
	unsigned short Year;
	
	RAM_Write( (unsigned char*)&Year, DayL+2, 2 );

	while( Interval > 0 )
	{
		*DayL += 1;
		if((*DayL) > ((( *(DayL+1)==2 ) && ((Year/4)==0))? 29:MonTab[*(DayL+1)]))
		{
			*DayL = 1;
			MonInc_Hex( DayL+1, 1 );
		}	
		Interval--;
	}
}

void HourInc_Hex( unsigned char* HourL, unsigned short Interval )
{
	unsigned short Hour;
	unsigned short Day;
	
	Hour = Interval;
	Hour += *HourL;
	if( Hour >= 24 )
	{
		Day = Hour / 24;
		DayInc_Hex( HourL+1, Day );	
		*HourL = Hour % 24;	
	}
	else *HourL = Hour; 
}	

void MinInc_Hex( unsigned char* MinL, unsigned short Interval )
{
	unsigned short Min;
	unsigned short Hour;

	Min = Interval;
	Min += *MinL;
	if( Min >= 60 )
	{
		Hour = Min / 60;
		HourInc_Hex( MinL+1, Hour );	
		*MinL = Min % 60;	
	}
	else *MinL = Min; 
}	

void SecInc_Hex( unsigned char* SecL, unsigned short Interval )
{	
	unsigned short Sec;
	unsigned short Min;

	Sec = Interval;
	Sec += *SecL;
	if( Sec >= 60 )
	{
		Min = Sec / 60;
		MinInc_Hex( SecL+1, Min );	
		*SecL = Sec % 60;	
	}
	else *SecL = Sec; 
}		

void Datetimes_Add_TI( unsigned char* Dest, unsigned char* Source, unsigned char DataType, unsigned short Interval )
{
	
	RAM_Write( Dest, Source, 7 );
	
	switch( DataType )
	{
		case 0:											//秒
			SecInc_Hex( Dest, Interval );
			break;			
		case 1:											//分
			MinInc_Hex( Dest+1, Interval );
			break;			
		case 2:											//时
			HourInc_Hex( Dest+2, Interval );
			break;			
		case 3:											//日
			DayInc_Hex( Dest+3, Interval );
			break;			
		case 4:											//月
			MonInc_Hex( Dest+4, Interval );
			break;			
		case 5:											//年
			YearInc_Hex( Dest+5, Interval );
			break;			
		default:										
			break;			
	}	
}	
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

#if ( CPUOutputRTC_Pluse == YesCheck )			//CPU输出秒信号				//17.11.03
//MCU速率（用户可配置）---------------------------------------------------------
#define	SPD_22000K		0
#define	SPD_11000K		1
#define	SPD_5500K		2
#define	SPD_2750K		3
#define	SPD_1375K		4
#define	SPD_688K		5
#define	SPD_344K		6
#define	SPD_172K		7
#define	SPD_MCU			SPD_11000K


/*******************************************************************************
功能描述：	单字节BCD码转HEX码
输入参数：	bcd:		源数据(BCD码)
返回参数：	HEX码数据
函数说明：
*******************************************************************************/
INT8U BCD_HEX(INT8U bcd)
{
	INT8U	hex;
	
	hex = bcd & 0xF0;
	hex >>= 1;
	hex += (hex>>2);
	hex += bcd & 0x0F;
	return hex;
}

/*******************************************************************************
功能描述：	2~4字节BCD格式转换为INT32U型HEX数据
输入参数：	*pData:		源数据(BCD码)
			len:		转换字节数
返回参数：	INT32U型HEX数据
函数说明：
*******************************************************************************/
INT32U nBCD_LHEX(const INT8U *pData, INT8U len)
{
	INT32U	value = 0;
	while (--len)
	{
		value += BCD_HEX(pData[len]);
		value *= 100;
	}
	value += BCD_HEX(pData[0]);
	return value;
}

/*******************************************************************************
功能描述：	软件延时若干毫秒
输入参数：	mSec:		延时时间
返回参数：
函数说明：
*******************************************************************************/
void Delay_mSec(INT8U mSec)
{
	INT16U	i;
	
	if (HT_CMU->SYSCLKCFG & 0x0002)				//高频时钟:PLL或HRC
	{
		while (mSec--)
		{
			for (i=0; i<(2200>>SPD_MCU); i++)
			{
				__NOP();
				__NOP();
				__NOP();
				__NOP();
			}
		}
	}
	else
	{
		for (i=0; i<10*mSec; i++)				//31uS*(1+4)*7=1ms
		{
			__NOP();
		}
	}
}

static const INT16U TAB_DFx_waibu[10] = 
{
	0x0000, 0x0000,
	0x007F, 0xD64c,
	0x007E, 0xD708,
	0x0000, 0x546E,
	0x0000, 0x04B0,  
};

static const INT16U TAB_DFx_waibu_New[10] = 		//18.04.28
{
	0x0000, 0x0000,
	0x007F, 0xDA4B,
	0x007E, 0xD9AC,
	0x0000, 0x4A2E,
	0x007F, 0xFC90,  
};

//static const INT16U TAB_DFx_neibu[10] = 
//{
//	0x0000, 0x0705,															//系数A, 常数项
//	0x007F, 0xE80D,															//系数B, 一次项
//	0x007F, 0x060C,															//系数C, 二次项
//	0x0000, 0x7262,															//系数D, 三次项
//	0x0000, 0x1E26															//系数E, 四次项
//};

#define	C_Toff			0x0000												//温度偏置寄存器
//#define	C_MCON01		0x6077												//控制系数01
#define	C_MCON01		0x2000												//控制系数01
//#define	C_MCON23		0x2388												//控制系数23
#define	C_MCON23		0x0588												//控制系数23
//#define	C_MCON45		0x2688												//控制系数45
#define	C_MCON45		0x4488												//控制系数45

#define	EnWr_WPREG()		HT_CMU->WPREG = 0xA55A							//则关闭写保护功能，用户可以写操作被保护的寄存器
#define	DisWr_WPREG()		HT_CMU->WPREG = 0x0000							//则开启写保护功能，用户禁止写操作被保护的寄存器

/*******************************************************************************
功能描述：	装载InfoData数据
*******************************************************************************/
void Load_InfoData(void)
{
	INT8U	i;
	INT32U	chksum = 0;
	INT32U	toff;
	INT16S	temp, code;
	
	for (i=0; i<14; i++)
	{
		chksum += HT_INFO->DataArry[i];
	}
	if (chksum == HT_INFO->DataArry[14])		//已设置
	{
		if (HT_RTC->DFAH	!= (HT_INFO->Muster.iDFAH   & 0x007F))	HT_RTC->DFAH	= (HT_INFO->Muster.iDFAH   & 0x007F);
		if (HT_RTC->DFAL	!= (HT_INFO->Muster.iDFAL   & 0xFFFF))	HT_RTC->DFAL	= (HT_INFO->Muster.iDFAL   & 0xFFFF);
		if (HT_RTC->DFBH	!= (HT_INFO->Muster.iDFBH   & 0x007F))	HT_RTC->DFBH	= (HT_INFO->Muster.iDFBH   & 0x007F);
		if (HT_RTC->DFBL	!= (HT_INFO->Muster.iDFBL   & 0xFFFF))	HT_RTC->DFBL	= (HT_INFO->Muster.iDFBL   & 0xFFFF);
		if (HT_RTC->DFCH	!= (HT_INFO->Muster.iDFCH   & 0x007F))	HT_RTC->DFCH	= (HT_INFO->Muster.iDFCH   & 0x007F);
		if (HT_RTC->DFCL	!= (HT_INFO->Muster.iDFCL   & 0xFFFF))	HT_RTC->DFCL	= (HT_INFO->Muster.iDFCL   & 0xFFFF);
		if (HT_RTC->DFDH	!= (HT_INFO->Muster.iDFDH   & 0x007F))	HT_RTC->DFDH	= (HT_INFO->Muster.iDFDH   & 0x007F);
		if (HT_RTC->DFDL	!= (HT_INFO->Muster.iDFDL   & 0xFFFF))	HT_RTC->DFDL	= (HT_INFO->Muster.iDFDL   & 0xFFFF);
		if (HT_RTC->DFEH	!= (HT_INFO->Muster.iDFEH   & 0x007F))	HT_RTC->DFEH	= (HT_INFO->Muster.iDFEH   & 0x007F);
		if (HT_RTC->DFEL	!= (HT_INFO->Muster.iDFEL   & 0xFFFF))	HT_RTC->DFEL	= (HT_INFO->Muster.iDFEL   & 0xFFFF);
		if (HT_RTC->Toff	!= (HT_INFO->Muster.iToff   & 0xFFFF))	HT_RTC->Toff	= (HT_INFO->Muster.iToff   & 0xFFFF);
		if (HT_RTC->MCON01	!= (HT_INFO->Muster.iMCON01 & 0xFFFF))	HT_RTC->MCON01	= (HT_INFO->Muster.iMCON01 & 0xFFFF);
		if (HT_RTC->MCON23	!= (HT_INFO->Muster.iMCON23 & 0xFFFF))	HT_RTC->MCON23	= (HT_INFO->Muster.iMCON23 & 0xFFFF);
		if (HT_RTC->MCON45	!= (HT_INFO->Muster.iMCON45 & 0xFFFF))	HT_RTC->MCON45	= (HT_INFO->Muster.iMCON45 & 0xFFFF);
	}
	else
	{
//		if(yingclkdatatab[1]!=0x00)
//		{	
//		if (HT_RTC->DFAH	!= TAB_DFx_neibu[0])	HT_RTC->DFAH	= TAB_DFx_neibu[0];
//		if (HT_RTC->DFAL	!= TAB_DFx_neibu[1])	HT_RTC->DFAL	= TAB_DFx_neibu[1];
//		if (HT_RTC->DFBH	!= TAB_DFx_neibu[2])	HT_RTC->DFBH	= TAB_DFx_neibu[2];
//		if (HT_RTC->DFBL	!= TAB_DFx_neibu[3])	HT_RTC->DFBL	= TAB_DFx_neibu[3];
//		if (HT_RTC->DFCH	!= TAB_DFx_neibu[4])	HT_RTC->DFCH	= TAB_DFx_neibu[4];
//		if (HT_RTC->DFCL	!= TAB_DFx_neibu[5])	HT_RTC->DFCL	= TAB_DFx_neibu[5];
//		if (HT_RTC->DFDH	!= TAB_DFx_neibu[6])	HT_RTC->DFDH	= TAB_DFx_neibu[6];
//		if (HT_RTC->DFDL	!= TAB_DFx_neibu[7])	HT_RTC->DFDL	= TAB_DFx_neibu[7];
//		if (HT_RTC->DFEH	!= TAB_DFx_neibu[8])	HT_RTC->DFEH	= TAB_DFx_neibu[8];
//		if (HT_RTC->DFEL	!= TAB_DFx_neibu[9])	HT_RTC->DFEL	= TAB_DFx_neibu[9];
//		}
//		else 
//		{	
			if(((*(unsigned char*)0x40188 == 0xFF)&&(*(unsigned char*)0x4018C == 0xFF))				//18.04.28
  				||((*(unsigned char*)0x40188 == 0xA0)&&(*(unsigned char*)0x4018C == 0x5F)))         //18.04.28
//  			if((*(unsigned char*)0x40188 == 0xA0)&&(*(unsigned char*)0x4018C == 0x5F))         	//18.04.28
			{                                                                       				//18.04.28
				if (HT_RTC->DFAH	!= TAB_DFx_waibu[0])	HT_RTC->DFAH	= TAB_DFx_waibu[0];
				if (HT_RTC->DFAL	!= TAB_DFx_waibu[1])	HT_RTC->DFAL	= TAB_DFx_waibu[1];
				if (HT_RTC->DFBH	!= TAB_DFx_waibu[2])	HT_RTC->DFBH	= TAB_DFx_waibu[2];
				if (HT_RTC->DFBL	!= TAB_DFx_waibu[3])	HT_RTC->DFBL	= TAB_DFx_waibu[3];
				if (HT_RTC->DFCH	!= TAB_DFx_waibu[4])	HT_RTC->DFCH	= TAB_DFx_waibu[4];
				if (HT_RTC->DFCL	!= TAB_DFx_waibu[5])	HT_RTC->DFCL	= TAB_DFx_waibu[5];
				if (HT_RTC->DFDH	!= TAB_DFx_waibu[6])	HT_RTC->DFDH	= TAB_DFx_waibu[6];
				if (HT_RTC->DFDL	!= TAB_DFx_waibu[7])	HT_RTC->DFDL	= TAB_DFx_waibu[7];
				if (HT_RTC->DFEH	!= TAB_DFx_waibu[8])	HT_RTC->DFEH	= TAB_DFx_waibu[8];
				if (HT_RTC->DFEL	!= TAB_DFx_waibu[9])	HT_RTC->DFEL	= TAB_DFx_waibu[9];
			}																								//18.04.28		
			else                                                                                            //18.04.28
			{                                                                                               //18.04.28
				if (HT_RTC->DFAH	!= TAB_DFx_waibu_New[0])	HT_RTC->DFAH	= TAB_DFx_waibu_New[0];     //18.04.28
				if (HT_RTC->DFAL	!= TAB_DFx_waibu_New[1])	HT_RTC->DFAL	= TAB_DFx_waibu_New[1];     //18.04.28
				if (HT_RTC->DFBH	!= TAB_DFx_waibu_New[2])	HT_RTC->DFBH	= TAB_DFx_waibu_New[2];     //18.04.28
				if (HT_RTC->DFBL	!= TAB_DFx_waibu_New[3])	HT_RTC->DFBL	= TAB_DFx_waibu_New[3];     //18.04.28
				if (HT_RTC->DFCH	!= TAB_DFx_waibu_New[4])	HT_RTC->DFCH	= TAB_DFx_waibu_New[4];     //18.04.28
				if (HT_RTC->DFCL	!= TAB_DFx_waibu_New[5])	HT_RTC->DFCL	= TAB_DFx_waibu_New[5];     //18.04.28
				if (HT_RTC->DFDH	!= TAB_DFx_waibu_New[6])	HT_RTC->DFDH	= TAB_DFx_waibu_New[6];     //18.04.28
				if (HT_RTC->DFDL	!= TAB_DFx_waibu_New[7])	HT_RTC->DFDL	= TAB_DFx_waibu_New[7];     //18.04.28
				if (HT_RTC->DFEH	!= TAB_DFx_waibu_New[8])	HT_RTC->DFEH	= TAB_DFx_waibu_New[8];     //18.04.28
				if (HT_RTC->DFEL	!= TAB_DFx_waibu_New[9])	HT_RTC->DFEL	= TAB_DFx_waibu_New[9];     //18.04.28
			}		                                                                                        //18.04.28
//		}
//	}			//17.12.06
		toff = *(INT32U*)0x4012C;				//Toff低位
		temp = *(INT16S*)0x4015E;				//温度
		code = *(INT16S*)0x4015C;				//TPS code
		if (((toff > 0x0BB8) && (toff < 0xF448))
		|| (temp < 2000) || (temp > 3000)
		|| (code < -7000) || (code > -1000))
		{
			toff = C_Toff;
		}
		if (HT_RTC->Toff	!= toff)		HT_RTC->Toff	= toff;
		if (HT_RTC->MCON01	!= C_MCON01)	HT_RTC->MCON01	= C_MCON01;
		if (HT_RTC->MCON23	!= C_MCON23)	HT_RTC->MCON23	= C_MCON23;
		if (HT_RTC->MCON45	!= C_MCON45)	HT_RTC->MCON45	= C_MCON45;
	}			//17.12.06
}

static const INT16S TAB_Temperature[] =
{
	22495,		//-50C
	4638,		//0C
	-720,		//+15C
	-7862,		//+35C
	-13220,		//+50C
	-31077,		//+100C
};
INT16S ADC_TempVolt(void)
{
	INT16S	temp, toff;
	
	toff = HT_RTC->Toff;
	temp = HT_TBS->TMPDAT;
	temp -= toff;
	return temp;
}
void Prog_InfoData(INT32U *info);
/*******************************************************************************
功能描述：	RTC误差校准
输入参数：	err:		秒脉冲误差
返回参数：
函数说明：
*******************************************************************************/
unsigned char  Sample_Error(INT8U *err)
{
	INT8U	i, j, num, buff[7];
	INT32U	info[15];
	INT32S	ratio[5];														//当前补偿系数
	INT16S	error, tps[3];
	INT64S	tmp64s;															//中间变量
	double	FN, tmp;
	double	add[3];															//修正补偿系数
    double	TT13, TT23, T13, T23;
	
	//RTC误差及温度采样---------------------------------------------------------
	error = nBCD_LHEX(&err[0], 3);
	if (err[3] & 0x80)
	{
		error = -error;
	}
	if ((error < -30000) || (error > 30000))								//-30.0s/d ~ +30.0s/d
	{
		return FALSE;
	}
	
	tps[0] = ADC_TempVolt();													//当前TpsCode
	for (i=0; i<3; i++)
	{
		if ((tps[0] <= TAB_Temperature[i*2+0])
		&& (tps[0] >= TAB_Temperature[i*2+1]))								//-50C~0C; +15C~+35C; +50C~+100C
		{
			break;
		}
	}
	if (i >= 3)
	{
		return FALSE;
	}
	j = i;																	//当前采样点
	num = 1;																//1个有效点
	//已采样数据收集------------------------------------------------------------
//	if (TRUE != Read_ParaData(ID_TpsCode, &buff[0]))						//读取温度采样数据
	//{
	//	return FALSE;
	//}
//	readep(buff,ID_TpsCode,6,eplxsj);
	E2P_RData( buff, ID_TpsCode, 6 );										//17.11.03
	for (i=0; i<3; i++)
	{
		if (i != j)															//不同点
		{
			tps[num] = buff[i*2] | buff[i*2+1]<<8;
			if ((tps[num] <= TAB_Temperature[i*2+0])
			&& (tps[num] >= TAB_Temperature[i*2+1]))						//-50C~0C; +15C~+35C; +50C~+100C
			{
				num++;														//有效点
			}
		}
		else																//同一点
		{
			buff[i*2]   = tps[0];
			buff[i*2+1] = tps[0]>>8;
		}
	}
//	wrep(buff,ID_TpsCode,6,eplxsj);
	E2P_WData( ID_TpsCode, buff, 6 );										//17.11.03

//	if (TRUE != Write_ParaData(ID_TpsCode, &buff[0]))						//更新温度采样数据
//	{
//		return FALSE;
//	}
	//读出InfoData--------------------------------------------------------------
	ratio[0] = HT_RTC->DFAH<<16 | HT_RTC->DFAL;
	ratio[1] = HT_RTC->DFBH<<16 | HT_RTC->DFBL;
	ratio[2] = HT_RTC->DFCH<<16 | HT_RTC->DFCL;
	ratio[3] = HT_RTC->DFDH<<16 | HT_RTC->DFDL;
	ratio[4] = HT_RTC->DFEH<<16 | HT_RTC->DFEL;
	for (i=0; i<5; i++)
	{
		if (ratio[i] & 0x00400000)
		{
			ratio[i] |= 0xFF800000;
		}
	}
	
	tmp64s  =  ratio[4];
	tmp64s *=  tps[0];
	tmp64s >>= 16;
	
	tmp64s +=  ratio[3];
	tmp64s *=  tps[0];
	tmp64s >>= 16;
	
	tmp64s +=  ratio[2];
	tmp64s *=  tps[0];
	tmp64s >>= 16;
	
	tmp64s +=  ratio[1];
	tmp64s *=  tps[0];
	tmp64s >>= 16;
	
	tmp64s +=  ratio[0];
	tmp64s +=  2;
	tmp64s >>= 2;
	
	FN  = (double)tmp64s/512.0 +32768;
	tmp = error / 1000.0;													//转换为double型
	FN  = FN*tmp/(86400-tmp);
	//曲线拟合------------------------------------------------------------------
	if (num == 3)															//3点拟合
	{
		TT13 = (double)(tps[0]+tps[2]) * (tps[0]-tps[2]);					//dTps[0]^2 - dTps[2]^2
		TT23 = (double)(tps[1]+tps[2]) * (tps[1]-tps[2]);					//dTps[1]^2 - dTps[2]^2
		
		T13  = (double)(tps[0]-tps[2]);										//dTps[0] - dTps[2]
		T23  = (double)(tps[1]-tps[2]);										//dTps[0] - dTps[2]
		
		TT23 *= T13;
		TT23 /= T23;
		TT23 = TT13 - TT23;
		
		add[2] = FN/TT23;													//2次系数修正
		add[1] = (FN-add[2]*TT13)/T13;										//1次系数修正
		add[0] = FN -add[2]*tps[0]*tps[0] -add[1]*tps[0];					//0次系数修正

		add[2] = add[2] *128 *16 *65536 *65536;
		add[1] = add[1] *128 *16 *65536;
		add[0] = add[0] *128 *16;
	}
	else if (num == 2)														//2点拟合
	{
		T23  = (double)(tps[1]-tps[0]);
		
		add[1] = -FN/T23;
		add[0] = -add[1]*tps[1];
		
		add[2] = 0;
		add[1] = add[1] *128 *16 *65536;
		add[0] = add[0] *128 *16;
	}
	else																	//1点拟合
	{
		add[2] = 0;
		add[1] = 0;
		add[0] = FN*128*16;
	}
    ratio[0] += (INT32S)add[0];												//0次补偿系数
    ratio[1] += (INT32S)add[1];												//1次补偿系数
    ratio[2] += (INT32S)add[2];												//2次补偿系数
	//写入InfoData数据----------------------------------------------------------
	info[0]  = (ratio[0]>>16) & 0x007F;
	info[1]  = ratio[0]       & 0xFFFF;
	info[2]  = (ratio[1]>>16) & 0x007F;
	info[3]  = ratio[1]       & 0xFFFF;
	info[4]  = (ratio[2]>>16) & 0x007F;
	info[5]  = ratio[2]       & 0xFFFF;
	info[6]  = (ratio[3]>>16) & 0x007F;
	info[7]  = ratio[3]       & 0xFFFF;
	info[8]  = (ratio[4]>>16) & 0x007F;
	info[9]  = ratio[4]       & 0xFFFF;
	info[10] = HT_RTC->Toff;
	info[11] = HT_RTC->MCON01;
	info[12] = HT_RTC->MCON23;
	info[13] = HT_RTC->MCON45;
	info[14] = 0;															//checksum
	for (i=0; i<14; i++)
	{
		info[14] += info[i];
	}
	Prog_InfoData(&info[0]);

	return TRUE;
}

void Prog_InfoData(INT32U *info)
{
	INT8U	i;
	INT32U	data[64];														//256字节1页
	
	for (i=0; i<64; i++)
	{
//		data[i] = *((INT32U*)(HT_INFO_BASE+i*4));							//整页读取
		data[i] = *((INT32U*)(HT_INFO_BASE+0x100+i*4));						//整页读取		//17.11.03
	}
	for (i=0; i<15; i++)
	{
		data[i+1] = info[i];												//更新RTC参数
	}
	//擦除InfoData--------------------------------------------------------------
//关闭预读取指令模式//
//	HT_CMU->PREFETCH=0X00000;

	EnWr_WPREG();
	HT_CMU->FLASHLOCK = 0x7A68;												//unlock flash memory
	HT_CMU->INFOLOCK  = 0xF998;												//unlock information flash memory
	HT_CMU->FLASHCON  = 0x02;												//page erase
	*((INT32U*)HT_INFO_BASE) = 0xFFFFFFFF;									//data
	while (HT_CMU->FLASHCON & 0x04)											//FLASH_BSY
		;
	HT_CMU->FLASHCON  = 0x00;												//read only
	
	Delay_mSec(2);
	//更新InfoData--------------------------------------------------------------
	HT_CMU->FLASHCON  = 0x01;												//word write
	for (i=0; i<64; i++)
	{
//		*((INT32U *)(HT_INFO_BASE+i*4)) = data[i];							//program word
		*((INT32U *)(HT_INFO_BASE+0x100+i*4)) = data[i];					//program word		//17.11.03
		while (HT_CMU->FLASHCON  & 0x04)									//FLASH_BSY
			;
	}
	HT_CMU->FLASHLOCK = ~0x7A68;											//lock flash memory
	HT_CMU->INFOLOCK  = ~0xF998;											//lock information flash memory
	HT_CMU->FLASHCON  = 0x00;												//read only
	DisWr_WPREG();

//主频太高开启预读取指令模式//
//	HT_CMU->PREFETCH=0X00001;
}

unsigned char NCom_WriteCPU_RTC(unsigned char* ComBuf )
{
  unsigned char* WriteBufAds;
  WriteBufAds =  ComBuf;
  
  if (TRUE == Sample_Error(WriteBufAds))											
  {
    Load_InfoData();
    return 0;
  }
  else return 1;	
  return 1;
}
#endif
