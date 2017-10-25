#ifndef __DEMAND_H__
#define __DEMAND_H__

#define Interval_method 0x01
#define Rolling_method  0x02

void MD_ParaInitial( void );
short MD_Meas( void );
void MD_Rest( void );
void initialMeterConstPara( void );
void clr_event_Ram( void );
unsigned char SetDemand_registerClassData( unsigned char *Source , unsigned short DataLen );
///unsigned char ActionDisconnect_controlClassData( unsigned char *Source, unsigned short DataLen );
void Action_Demand_reg_Reset( short ItemNo );
unsigned char ActionClass3_register( unsigned char *Source, unsigned short DataLen );

unsigned char ActionDemand_registerClassData( unsigned char *Source, unsigned short DataLen );
void MaxDemand_Reset(void);
void Clr_E2PECData( short Addr, short DataLenth, short Cylcle );

#endif

