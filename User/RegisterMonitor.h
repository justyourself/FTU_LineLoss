#ifndef __REGISTERMONITOR_H__
#define __REGISTERMONITOR_H__

short Judge_monitored_value( unsigned char* Data );
short Judge_actions( unsigned char* Data );
short GetRegister_monitorClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char SetRegister_monitorClassData( unsigned char *Source , unsigned short DataLen );
void initialRegister_monitorPara( void );
#endif
