#ifndef __PRODUCE_H__
#define __PRODUCE_H__

extern void InitialEventLog( void );
//extern void InitialProfClassPara_display( void );
extern void MeterClrCycle( void );
extern void InitialProfile_genericClassPara( void );
extern void InitialSingle_action_scheduleClassPara( void );

extern void InitialDemandPara( void );
//水表、气表初始化..
extern void Mobus_meter_clr(void);
extern void	Clr_Volt_qulityevent(void);
#endif
