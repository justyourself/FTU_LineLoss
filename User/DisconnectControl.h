#include "ht6xxx_gpio.h"

//继电器操作事件..

#define 	P_RELAY1OFF		GPIO_Pin_9
#define 	P_RELAY1ON		GPIO_Pin_10
#define 	P_RELAY2OFF		GPIO_Pin_0
#define 	P_RELAY2ON		GPIO_Pin_1

#define RELAY_ON	0x00	//磁保持继电器闭合
#define RELAY_OFF	0x01	//磁保持继电器断开
#define Route_A 0x80
#define Route_B 0x40
#define Route_C 0x20
#define Route_D 0x10
#define Route_E 0x08
#define Route_F 0x04
#define Route_G 0x02
#define Route_H 0x01

short GetDisconnect_controlClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char SetDisconnect_controlClassData( unsigned char *Source , unsigned short DataLen );
unsigned char ActionDisconnect_controlClassData( unsigned char *Source, unsigned short DataLen );
void RelayDrive( unsigned char DriveType );
void OperateRelay( void );

//(本地需量)普通模式.. //紧急模式.. ----- 2选1..
void Relay_localControl_Event( void );
void localControl_EventStart( void );
void localControl_EventEnd( void );
void Clr_local_control_para_monitor( void );
void Relay_Ctr(void);

