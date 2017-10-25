
#define ZONENUMBER 	6
#define WEEKNUMBER 	8
#define SEGNUMBER	12

short GetActivity_calendarClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 
unsigned char SetActivity_calendarClassData( unsigned char *Source , unsigned short DataLen );
short GetSpecial_daysClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 
unsigned char SetSpecial_daysClassData( unsigned char *Source , unsigned short DataLen );

short GetNextActivity_calendarClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short GetNextSpecial_daysClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
short Get_sigle_spec_dayPara( unsigned char* Dest, unsigned short Addr, unsigned short Items, unsigned short* Length );
short Get_sigle_day_profile( unsigned char* Dest, unsigned short AddrOffset, unsigned short ItemNo, unsigned short* Len );

unsigned char ActionActivity_calendarClassData( unsigned char *Source , unsigned short DataLen );
unsigned char ActionSpecial_DaysClassData( unsigned char *Source , unsigned short DataLen );

short CompSpecday( unsigned char* Source );
short CompSeason( unsigned char* Source );
unsigned short GetAddrOffset( void );
short GetDay_id( void );
void GetDay_schedule( unsigned char Day_id );
void GetFeeNo(void);
//unsigned char GetNo(unsigned char* Point, unsigned char* Source, short ItemNums);

unsigned char Judge_Activate_passive_calendar_time( unsigned char* Source );
void Activated_Passive_TOU( void );
void Execute_Passive_TOU_Activated( void );

unsigned char judge_daylightsaving( unsigned char change_time_flag );	//œƒ¡Ó ±≈–∂œ..


