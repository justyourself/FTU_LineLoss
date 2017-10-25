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


