
void ClkAdjFact(void);
void Temperature(void);
//void Clock(void);
//void ClkChk(void);
//void SetClkBak( void );
//unsigned short GetBillMon( char* Addr );
unsigned short GetGmDay( unsigned char* Addr );
//#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
//#if ( ModifyTimeSkipDelay == YesCheck )						//新国网	//14.06.19
unsigned long GetGmSec( unsigned char* Addr );
//#endif
//#else
//unsigned long GetGmSec( char* Addr );
//#endif
unsigned long GetGmMin( unsigned char* Addr );
//char GetWeek( void );
//void Min_Date( char* Sec, unsigned long Min );
//char GetFeeNo( void );
unsigned long GetSec( unsigned char* Addr );
//short TimeValueCheck( unsigned char* Source );
void GetTime( void );
short TimeValueCheck( unsigned char* Source );
short DateValueCheck( unsigned char* Source );
unsigned char WeekDay20( unsigned char* Addr );
void GetICTime();
short DateTime5ValueCheck( unsigned char* Source );
short DateTime4ValueCheck( unsigned char* Source );
short DateTime6ValueCheck( unsigned char* Source );
void MoveTimeBCD_Hex( unsigned char *Dest, unsigned char *source );

void MoveCurrentTimeBCD_Hex( void );
void MoveCurrentTimeHex_BCD( void );
void ChangeTimeBCDToHex( unsigned char* Dest, unsigned char* Source );
void ChangeTimeHexToBCD( unsigned char* Dest, unsigned char* Source );
void DateTimeAddMin( unsigned char* Dest, unsigned char* Source, unsigned short Interval );
void YearInc_Hex( unsigned char* YearL, unsigned short Interval );
void MonInc_Hex( unsigned char* MonL, unsigned short Interval );
void DayInc_Hex( unsigned char* DayL, unsigned short Interval );
void HourInc_Hex( unsigned char* HourL, unsigned short Interval );
void MinInc_Hex( unsigned char* MinL, unsigned short Interval );
void SecInc_Hex( unsigned char* SecL, unsigned short Interval );
void Datetimes_Add_TI( unsigned char* Dest, unsigned char* Source, unsigned char DataType, unsigned short Interval );

short Check_date_time_s( unsigned char* Source );	//17.04.17
void DayInc( unsigned char* Data );

