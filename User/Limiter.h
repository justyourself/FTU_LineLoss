#ifndef __LIMITER_H__
#define __LIMITER_H__

short GetLimiterClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char SetLimiterClassData( unsigned char *Source , unsigned short DataLen );
void Ave_Power( void );
void read_Threshold_for_MDEvent( void );
unsigned short JudgeTwo_DateTime( unsigned char* Source, unsigned long ADD_source);
unsigned long HexGetGmSec( unsigned char* Addr );
unsigned long HexGetGmMin( unsigned char* Addr );
///short Judge_monitored_value( unsigned char* Data );
unsigned short ComparTwo_DateTime( unsigned char* Source);
#endif
