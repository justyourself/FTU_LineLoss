//#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
#include "Data.h"
//#include "Display.h"
//#include "RsComm.h"
//#include "Measure.h"
//#include "Power.h"
//#include "Port.h"
#include "MeterDataType.h"
//#include "Display.h"


const METERDATATAB MeterDataTab[] =
{
	0x0001,/*0x01,0x00,0x01,0x08,0x00,0xFF,*/   CMon_EC_Pp0,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy,  /*	DLen_9,	DLen_3,		DDU_kwh,*/ //0
	0x0002,/*0x01,0x00,0x01,0x08,0x01,0xFF,*/	CMon_EC_Pp1,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, 	/*  DLen_9,	DLen_3,		DDU_kwh,*/	//1
	0x0003,/*0x01,0x00,0x01,0x08,0x02,0xFF,*/	CMon_EC_Pp2,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy,  /*	DLen_9,	DLen_3,		DDU_kwh,*/	//2
	0x0004,/*0x01,0x00,0x01,0x08,0x03,0xFF,*/	CMon_EC_Pp3,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, 	/*  DLen_9,	DLen_3,		DDU_kwh,*/	//3
	0x0005,/*0x01,0x00,0x01,0x08,0x04,0xFF,*/	CMon_EC_Pp4,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy,  /*	DLen_9,	DLen_3,		DDU_kwh,*/	//4
//	0x0006,	0x0100010805FF,	CMon_EC_Pp5,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kwh,	//5
//	0x0007,	0x0100010806FF,	CMon_EC_Pp6,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kwh,	//6
//	0x0008,	0x0100010807FF,	CMon_EC_Pp7,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kwh,	//7
//	0x0009,	0x0100010808FF,	CMon_EC_Pp8,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kwh,	//8
	
	0x000A,/*	0x01,0x00,0x02,0x08,0x00,0xFF,*/	CMon_EC_Pn0,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kwh,*/	//9
	0x000B,/*	0x01,0x00,0x02,0x08,0x01,0xFF,*/	CMon_EC_Pn1,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kwh,*/	//10
	0x000C,/*	0x01,0x00,0x02,0x08,0x02,0xFF,*/	CMon_EC_Pn2,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kwh,*/	//11
	0x000D,/*	0x01,0x00,0x02,0x08,0x03,0xFF,*/	CMon_EC_Pn3,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kwh,*/	//12
	0x000E,/*	0x01,0x00,0x02,0x08,0x04,0xFF,*/	CMon_EC_Pn4,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kwh,*/	//13
//	0x000F,/*	0x0100020805FF,	CMon_EC_Pn5,*/	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kwh,	//14
//	0x0010,	0x0100020806FF,	CMon_EC_Pn6,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kwh,	//15
//	0x0011,	0x0100020807FF,	CMon_EC_Pn7,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kwh,	//16
//	0x0012,	0x0100020808FF,	CMon_EC_Pn8,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kwh,	//17
	
	0x0013,/*	0x0100030800FF,*/	CMon_EC_Qp0,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功总电量,	1,	$15
	0x0014,/*	0x0100030801FF,*/	CMon_EC_Qp1,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功T1,	1,	$15
	0x0015,/*	0x0100030802FF,*/	CMon_EC_Qp2,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功T2,	1,	$15
	0x0016,/*	0x0100030803FF,*/	CMon_EC_Qp3,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功T3,	1,	$15
	0x0017,/*	0x0100030804FF,*/	CMon_EC_Qp4,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功T4,	,	$15
//	0x0018,	0x0100030805FF,	CMon_EC_Qp5,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功T5,	,	$15
//	0x0019,	0x0100030806FF,	CMon_EC_Qp6,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功T6,	,	$15
//	0x001A,	0x0100030807FF,	CMon_EC_Qp7,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功T7,	,	$15
//	0x001B,	0x0100030808FF,	CMon_EC_Qp8,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功T8,	,	$15
	0x001C,/*	0x0100040800FF,*/	CMon_EC_Qn0,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向总电量,	,	$15
	0x001D,/*	0x0100040801FF,*/	CMon_EC_Qn1,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向T1,	,	$15
	0x001E,/*	0x0100040802FF,*/	CMon_EC_Qn2,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向T2,	,	$15
	0x001F,/*	0x0100040803FF,*/	CMon_EC_Qn3,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向T3,	,	$15
	0x0020,/*	0x0100040804FF,*/	CMon_EC_Qn4,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向T4,	,	$15
//	0x0021,	0x0100040805FF,	CMon_EC_Qn5,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向T5,	,	$15
//	0x0022,	0x0100040806FF,	CMon_EC_Qn6,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向T6,	,	$15
//	0x0023,	0x0100040807FF,	CMon_EC_Qn7,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向T7,	,	$15
//	0x0024,	0x0100040808FF,	CMon_EC_Qn8,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功反向T8,	,	$15
	0x0025,/*	0x0100050800FF,*/	CMon_EC_Q10,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限总电量,	,	$15
	0x0026,/*	0x0100050801FF,*/	CMon_EC_Q11,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限T1,	,	$15
	0x0027,/*	0x0100050802FF,*/	CMon_EC_Q12,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限T2,	,	$15
	0x0028,/*	0x0100050803FF,*/	CMon_EC_Q13,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限T3,	,	$15
	0x0029,/*	0x0100050804FF,*/	CMon_EC_Q14,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限T4,	,	$15
//	0x002A,	0x0100050805FF,	CMon_EC_Q15,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限T5,	,	$15
//	0x002B,	0x0100050806FF,	CMon_EC_Q16,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限T6,	,	$15
//	0x002C,	0x0100050807FF,	CMon_EC_Q17,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限T7,	,	$15
//	0x002D,	0x0100050808FF,	CMon_EC_Q18,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第一象限T8,	,	$15
	0x002E,/*	0x0100060800FF,*/	CMon_EC_Q20,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限总电量,	,	$15
	0x002F,/*	0x0100060801FF,*/	CMon_EC_Q21,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限T1,	,	$15
	0x0030,/*	0x0100060802FF,*/	CMon_EC_Q22,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限T2,	,	$15
	0x0031,/*	0x0100060803FF,*/	CMon_EC_Q23,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限T3,	,	$15
	0x0032,/*	0x0100060804FF,*/	CMon_EC_Q24,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限T4,	,	$15
//	0x0033,	0x0100060805FF,	CMon_EC_Q25,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限T5,	,	$15
//	0x0034,	0x0100060806FF,	CMon_EC_Q26,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限T6,	,	$15
//	0x0035,	0x0100060807FF,	CMon_EC_Q27,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限T7,	,	$15
//	0x0036,	0x0100060808FF,	CMon_EC_Q28,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第二象限T8,	,	$15
	0x0037,/*	0x0100070800FF,*/	CMon_EC_Q30,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限总电量,	,	$15
	0x0038,/*	0x0100070801FF,*/	CMon_EC_Q31,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限T1,	,	$15
	0x0039,/*	0x0100070802FF,*/	CMon_EC_Q32,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限T2,	,	$15
	0x003A,/*	0x0100070803FF,*/	CMon_EC_Q33,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限T3,	,	$15
	0x003B,/*	0x0100070804FF,*/	CMon_EC_Q34,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限T4,	,	$15
//	0x003C,	0x0100070805FF,	CMon_EC_Q35,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限T5,	,	$15
//	0x003D,	0x0100070806FF,	CMon_EC_Q36,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限T6,	,	$15
//	0x003E,	0x0100070807FF,	CMon_EC_Q37,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限T7,	,	$15
//	0x003F,	0x0100070808FF,	CMon_EC_Q38,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第三象限T8,	,	$15
	0x0040,/*	0x0100080800FF,*/	CMon_EC_Q40,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限总电量,	,	$15
	0x0041,/*	0x0100080801FF,*/	CMon_EC_Q41,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限T1,	,	$15
	0x0042,/*	0x0100080802FF,*/	CMon_EC_Q42,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限T2,	,	$15
	0x0043,/*	0x0100080803FF,*/	CMon_EC_Q43,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限T3,	,	$15
	0x0044,/*	0x0100080804FF,*/	CMon_EC_Q44,	DLen_6,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Energy, /*	DLen_9,	DLen_3,		DDU_kvarh,*/	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限T4,	,	$15
//	0x0045,	0x0100080805FF,	CMon_EC_Q45,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限T5,	,	$15
//	0x0046,	0x0100080806FF,	CMon_EC_Q46,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限T6,	,	$15
//	0x0047,	0x0100080807FF,	CMon_EC_Q47,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限T7,	,	$15
//	0x0048,	0x0100080808FF,	CMon_EC_Q48,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	无功第四象限T8,	,	$15
//	0x0049,	0x0100210800FF,	CMon_EC_PpA,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	A相有功总电量,	,	$15
//	0x004A,	0x0100220800FF,	CMon_EC_PnA,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	A相有功反向电量,	,	$15
//	0x004B,	0x0100410800FF,	CMon_EC_PpB,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	B相有功总电量,	,	$15
//	0x004C,	0x0100420800FF,	CMon_EC_PnB,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	B相有功反向电量,	,	$15
//	0x004D,	0x0100610800FF,	CMon_EC_PpC,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	C相有功总电量,	,	$15
//	0x004E,	0x0100620800FF,	CMon_EC_PnC,	DLen_6,		F_DRS_ReadEN+DRS_Energy, 	DLen_9,	DLen_3,		DDU_kvarh,	//EEPROM:000000000.0;RAM:,	<0.1,	R,	实时显示，Kwh,	C相有功反向电量,	,	$15
	
	0x004F,	/*0x0100010800FF,*/	CMon_MD_Pp0,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	 */	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量总,	,	$14,	$15
	0x0050,	/*0x0100010800FF,*/	CMon_MD_Pp1,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量T1,	,	$14,	$15
	0x0051,	/*0x0100010800FF,*/	CMon_MD_Pp2,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量T2,	,	$14,	$15
	0x0052, /*0x0100010800FF,*/	CMon_MD_Pp3,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量T3,	,	$14,	$15
	0x0053,	/*0x0100010800FF,*/	CMon_MD_Pp4,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量T4,	,	$14,	$15
	0x0054,	/*0x0100010800FF,*/	CMon_MD_Pp5,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量T5,	,	$14,	$15
	0x0055,	/*0x0100010800FF,*/	CMon_MD_Pp6,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量T6,	,	$14,	$15
	0x0056,	/*0x0100010800FF,*/	CMon_MD_Pp7,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量T7,	,	$14,	$15
	0x0057,	/*0x0100010800FF,*/	CMon_MD_Pp8,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功最大需量T8,	,	$14,	$15
	0x0058,	/*0x0100010800FF,*/	CMon_MD_Qp0,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量总,	,	$14,	$15
	0x0059,	/*0x0100010800FF,*/	CMon_MD_Qp1,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量T1,	,	$14,	$15
	0x005A,	/*0x0100010800FF,*/	CMon_MD_Qp2,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量T2,	,	$14,	$15
	0x005B,	/*0x0100010800FF,*/	CMon_MD_Qp3,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量T3,	,	$14,	$15
	0x005C,	/*0x0100010800FF,*/	CMon_MD_Qp4,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量T4,	,	$14,	$15
	0x005D,	/*0x0100010800FF,*/	CMon_MD_Qp5,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量T5,	,	$14,	$15
	0x005E,	/*0x0100010800FF,*/	CMon_MD_Qp6,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量T6,	,	$14,	$15
//	0x005F,	/*0x0100010800FF,*/	CMon_MD_Qp7,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量T7,	,	$14,	$15
//	0x0060,	/*0x0100010800FF,*/	CMon_MD_Qp8,	DLen_3,		F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kvar,*/	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功最大需量T8,	,	$14,	$15
//	0x0061,	0x0100010800FF,	CMon_MD_Q10,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量总,	,	$14,	$15
//	0x0062,	0x0100010800FF,	CMon_MD_Q11,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量T1,	,	$14,	$15
//	0x0063,	0x0100010800FF,	CMon_MD_Q12,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量T2,	,	$14,	$15
//	0x0064,	0x0100010800FF,	CMon_MD_Q13,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量T3,	,	$14,	$15
//	0x0065,	0x0100010800FF,	CMon_MD_Q14,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量T4,	,	$14,	$15
//	0x0066,	0x0100010800FF,	CMon_MD_Q15,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量T5,	,	$14,	$15
//	0x0067,	0x0100010800FF,	CMon_MD_Q16,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量T6,	,	$14,	$15
//	0x0068,	0x0100010800FF,	CMon_MD_Q17,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量T7,	,	$14,	$15
//	0x0069,	0x0100010800FF,	CMon_MD_Q18,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q1最大需量T8,	,	$14,	$15
//	0x006A,	0x0100010800FF,	CMon_MD_Q20,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量总,	,	$14,	$15
//	0x006B,	0x0100010800FF,	CMon_MD_Q21,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量T1,	,	$14,	$15
//	0x006C,	0x0100010800FF,	CMon_MD_Q22,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量T2,	,	$14,	$15
//	0x006D,	0x0100010800FF,	CMon_MD_Q23,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量T3,	,	$14,	$15
//	0x006E,	0x0100010800FF,	CMon_MD_Q24,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量T4,	,	$14,	$15
//	0x006F,	0x0100010800FF,	CMon_MD_Q25,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量T5,	,	$14,	$15
//	0x0070,	0x0100010800FF,	CMon_MD_Q26,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量T6,	,	$14,	$15
//	0x0071,	0x0100010800FF,	CMon_MD_Q27,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量T7,	,	$14,	$15
//	0x0072,	0x0100010800FF,	CMon_MD_Q28,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q2最大需量T8,	,	$14,	$15
//	0x0073,	0x0100010800FF,	CMon_MD_Q30,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量总,	,	$14,	$15
//	0x0074,	0x0100010800FF,	CMon_MD_Q31,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量T1,	,	$14,	$15
//	0x0075,	0x0100010800FF,	CMon_MD_Q32,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量T2,	,	$14,	$15
//	0x0076,	0x0100010800FF,	CMon_MD_Q33,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量T3,	,	$14,	$15
//	0x0077,	0x0100010800FF,	CMon_MD_Q34,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量T4,	,	$14,	$15
//	0x0078,	0x0100010800FF,	CMon_MD_Q35,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量T5,	,	$14,	$15
//	0x0079,	0x0100010800FF,	CMon_MD_Q36,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量T6,	,	$14,	$15
//	0x007A,	0x0100010800FF,	CMon_MD_Q37,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量T7,	,	$14,	$15
//	0x007B,	0x0100010800FF,	CMon_MD_Q38,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q3最大需量T8,	,	$14,	$15
//	0x007C,	0x0100010800FF,	CMon_MD_Q40,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量总,	,	$14,	$15
//	0x007D,	0x0100010800FF,	CMon_MD_Q41,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量T1,	,	$14,	$15
//	0x007E,	0x0100010800FF,	CMon_MD_Q42,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量T2,	,	$14,	$15
//	0x007F,	0x0100010800FF,	CMon_MD_Q43,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量T3,	,	$14,	$15
//	0x0080,	0x0100010800FF,	CMon_MD_Q44,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量T4,	,	$14,	$15
//	0x0081,	0x0100010800FF,	CMon_MD_Q45,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量T5,	,	$14,	$15
//	0x0082,	0x0100010800FF,	CMon_MD_Q46,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量T6,	,	$14,	$15
//	0x0083,	0x0100010800FF,	CMon_MD_Q47,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量T7,	,	$14,	$15
//	0x0084,	0x0100010800FF,	CMon_MD_Q48,	DLen_4,		F_DRS_ReadEN+DRS_Eeprom2, 		DLen_5,		DLen_3,		DDU_kvar,	//EEPROM:00000.000,	R,	定时刷新，Kw,	无功Q4最大需量T8,	$13,	$14,	$15
	0x0085,/*	0x01,0x00,0x20,0x07,0x00,0xff,*/	(unsigned long)&Real.Ua,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_4,		DLen_2,		DDU_V,*/	//RAM:0000.00,	R,	定时刷新，V,	A相电压,	,	$14,	$15
	0x0086,/*	0x01,0x00,0x34,0x07,0x00,0xff,*/	(unsigned long)&Real.Ub,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_4,		DLen_2,		DDU_V,*/	//RAM:0000.00,	R,	定时刷新，V,	B相电压,	,	$14,	$15
	0x0087,/*	0x01,0x00,0x48,0x07,0x00,0xff,*/	(unsigned long)&Real.Uc,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_4,		DLen_2,		DDU_V,*/	//RAM:0000.00,	R,	定时刷新，V,	C相电压,	,	$14,	$15
	0x0088,/*	0x01,0x00,0x1F,0x07,0x00,0xff,*/	(unsigned long)&Real.Ia,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_3,		DLen_3,		DDU_A,*/	//RAM:000.000,	R,	定时刷新，A,	A相电流,	,	$14,	$15
	0x0089,/*	0x01,0x00,0x33,0x07,0x00,0xff,*/	(unsigned long)&Real.Ib,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_3,		DLen_3,		DDU_A,*/	//RAM:000.000,	R,	定时刷新，A,	B相电流,	,	$14,	$15
	0x008A,/*	0x01,0x00,0x47,0x07,0x00,0xff,*/	(unsigned long)&Real.Ic,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_3,		DLen_3,		DDU_A,*/	//RAM:000.000,	R,	定时刷新，A,	C相电流,	,	$14,	$15
	0x008B,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Freq,	DLen_2,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_2,		DLen_2,		DDU_Hz,*/	//RAM:00.00,	R,	定时刷新，Hz,	合相频率,	,	$14,	$15
	0x008C,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Freq,	DLen_2,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_2,		DLen_2,		DDU_Hz,*/	//RAM:00.00,	R,	定时刷新，Hz,	A相频率,	,	$14,	$15
	0x008D,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Freq,	DLen_2,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_2,		DLen_2,		DDU_Hz,*/	//RAM:00.00,	R,	定时刷新，Hz,	B相频率,	,	$14,	$15
//	0x008E,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Pa,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_2,		DLen_2,		DDU_Hz,*/	//RAM:00.00,	R,	定时刷新，Hz,	C相频率,	,	$14,	$15
	0x008F,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Pa,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_5,		DLen_3,		DDU_kw,*/	//RAM:00000.000,	R,	定时刷新，Kw,	A相功率,	,	$14,	$15
	0x0090,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Pb,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_5,		DLen_3,		DDU_kw,*/	//RAM:00000.000,	R,	定时刷新，Kw,	B相功率,	,	$14,	$15
	0x0091,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Pc,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_5,		DLen_3,		DDU_kw,*/	//RAM:00000.000,	R,	定时刷新，Kw,	C相功率,	,	$14,	$15
//	0x0092,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&eal.Pfa,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_5,		DLen_3,		DDU_kw,	*///RAM:00000.000,	R,	定时刷新,	合相功率,	,	$14,	$15
	0x0093,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Pfa,	DLen_2,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_2,		DLen_2,		0,*/	//RAM:00.00,	R,	定时刷新,	A相功率因素,	,	$14,	$15
	0x0094,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Pfb,	DLen_2,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_2,		DLen_2,		0,*/	//RAM:00.00,	R,	定时刷新,	B相功率因素,	,	$14,	$15
	0x0095,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Pfc,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_2,		DLen_2,		0,*/	//RAM:00.00,	R,	定时刷新,	C相功率因素,	,	$14,	$15
	0x0096,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	(unsigned long)&Real.Pfc,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, /*	DLen_2,		DLen_2,		0,*/	//RAM:00.00,	R,	定时刷新,	合相功率因素,	,	$14,	$15
	
	0x0097,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R,	,	A相每秒脉冲数,	,	$14,	$15
	0x0098,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/    0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R,	,	B相每秒脉冲数,	,	$14,	$15
	0x0099,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R,	,	C相每秒脉冲数,	,	$14,	$15
	0x00A0,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R,	,	合相每秒脉冲数,	,	$14,	$15
	0x00A1,/*   0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R,	,	逆相序状态字,	,	$14,	$15
	0x00A2,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R,	,	电压状态字,	,	$14,	$15
	0x00A3,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R,	,	电流状态字（断流和方向）,	,	$14,	$15
	0x00A4,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_instantaneous, /*	DLen_3,		DLen_1,		0,*/	//RAM:000.0,	R,	,	计量芯片温度,	,	$14,	$15
	0x00A5,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_20,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2,/*	DLen_20,	DLen_0,		0,*/	//EEPROM:,	R/W,	,	表号,	,	$14,	$15
	0x00A6,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R,	,	电表运行状态字1,	,	$14,	$15
	0x00A7,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R,	,	电表运行状态字2,	,	$14,	$15
	0x00A8,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R/W,	,	电表运行状态字3,	,	$14,	$15
	0x00A9,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R,	,	电网状态字1,	,	$14,	$15
	0x00AA,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_4,		DLen_0,		0,*/	//RAM:0000,	R,	,	电网状态字2,	,	$14,	$15
	0x00AB,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R,	,	按键状态字,	,	$14,	$15
	0x00AC,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+DRS_Ram, 		/*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R,	,	开盖开端状态字,	,	$14,	$15
	0x00AD,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_7,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Ram+F_DRS_SYNData, /*	DLen_6,		DLen_0,		DDU_Time,*/	//RAM:SSMMHH,	R/W,	,	时间,	,	$14,	$15
	0x00AE,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_3,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Ram+F_DRS_SYNData, /*	DLen_6,		DLen_0,		DDU_Date,*/	//RAM:DDMMYY,	R/W,	,	日期,	,	$14,	$15
	0x00AF,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2, /*	DLen_2,		DLen_0,		0,*/	//RAM:00-(00冬，01夏）,	R/W,	,	冬夏令时状态字,	,	$14,	$15
	0x00B0,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2+F_DRS_SYNData, /*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R/W,	,	费率状态字,	,	$14,	$15
	0x00B1,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_30,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2, /*	DLen_30,	DLen_0,		0,*/	//RAM:,	R/W,	,	简易时段表,	,	$14,	$15
	0x00B2,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2, /*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R/W,	,	需量运行模式,	,	$14,	$15
	0x00B3,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2, /*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R/W,	,	需量周期,	,	$14,	$15
	0x00B4,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2, /*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R/W,	,	需量滑差时间,	,	$14,	$15
	0x00B5,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2, /*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R/W,	,	计量计算模式,	,	$14,	$15
	0x1007,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2+F_DRS_SYNData, /*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R/W,	,	显示模式,	,	$14,	$15
	0x1008,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2+F_DRS_SYNData, /*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R/W,	,	显示位数,	,	$14,	$15
	0x1009,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_1,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2+F_DRS_SYNData, /*	DLen_2,		DLen_0,		0,*/	//RAM:00,	R/W,	,	显示轮休时间,	,	$14,	$15
	0x100a,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_30,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2+F_DRS_SYNData, /*	DLen_30,	DLen_0,		0,*/	//RAM,	R/W,	,	显示列表,	,	$14,	$15
	0x00C0,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_40,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2, /*	DLen_40,	DLen_0,		0,*/	//RAM,	R/W,	,	特殊数据显示串,	,	$14,	$15
	0x00C1,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_40,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2, /*	DLen_40,	DLen_0,		0,*/	//RAM,	R/W,	,	特殊符号显示,	,	$14,	$15
//	0x00C2,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_30,	F_DRS_ReadEN+DRS_Eeprom2, 	/*	DLen_30,	DLen_0,		0,*/	//RAM,	R,	,	事件记录(计量模块相关的事件）,	,	$14,	$15


	0x00C2,	/*0x0100010800FF,*/	0x0000,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, 	/*	DLen_5,		DLen_3,		DDU_kw,	 */	//当前频率
	0x00C3,	/*0x0100010800FF,*/	0x0000,	DLen_4,		F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //ActiveImportPower
	0x00C4, /*0x0100010800FF,*/ 0x0000, DLen_4, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //ActiveExportPower
	0x00C5, /*0x0100010800FF,*/ 0x0000, DLen_4, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //ReactiveImportPower
	0x00C6, /*0x0100010800FF,*/ 0x0000, DLen_4, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //ReactiveExportPower

	//----------需量----------------------
	0x00D9, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向有功总需量
	0x00DA, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向有功总需量
	0x00DB, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向有功总需量
	0x00DC, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向有功总需量		
	0x00DD, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向有功总需量

	0x00E2, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向无功总需量
	0x00E3, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向无功总需量
	0x00E4, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向无功总需量
	0x00E5, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向无功总需量
	0x00E6, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //正向无功总需量

	0x00EB, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向有功总需量
	0x00EC, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向有功总需量
	0x00ED, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向有功总需量
	0x00EE, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向有功总需量
	0x00EF, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向有功总需量

	0x00F4, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向无功总需量
	0x00F5, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向无功总需量
	0x00F6, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向无功总需量
	0x00F7, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向无功总需量
	0x00F8, /*0x0100010800FF,*/ 0x0000, DLen_3, 	F_DRS_ReadEN+DRS_instantaneous+F_DRS_SYNData, //反向无功总需量

	//--------------有功反向最大需量-------
	0x00C7,	/*0x0100020600FF,*/	CMon_MD_Pn0, DLen_3,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	 */	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功反向最大需量总,	,	$14,	$15
	0x00C8, /*0x0100010800FF,*/ CMon_MD_Pn1, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 
	0x00C9, /*0x0100010800FF,*/ CMon_MD_Pn2, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00CA, /*0x0100010800FF,*/ CMon_MD_Pn3, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00CB, /*0x0100010800FF,*/ CMon_MD_Pn4, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00CC, /*0x0100010800FF,*/ CMon_MD_Pn5, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00CD, /*0x0100010800FF,*/ CMon_MD_Pn6, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00CE, /*0x0100010800FF,*/ CMon_MD_Pn7, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00CF, /*0x0100010800FF,*/ CMon_MD_Pn8, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,

	//--------------无功反向最大需量-------
	0x00D0,	/*0x0100040600FF,*/	CMon_MD_Qn0, DLen_3,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 	/*	DLen_5,		DLen_3,		DDU_kw,	 */	//EEPROM:00000.000,	R,	定时刷新，Kw,	有功反向最大需量总,	,	$14,	$15
	0x00D1, /*0x0100010800FF,*/ CMon_MD_Qn1, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2, 
	0x00D2, /*0x0100010800FF,*/ CMon_MD_Qn2, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00D3, /*0x0100010800FF,*/ CMon_MD_Qn3, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00D4, /*0x0100010800FF,*/ CMon_MD_Qn4, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00D5, /*0x0100010800FF,*/ CMon_MD_Qn5, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00D6, /*0x0100010800FF,*/ CMon_MD_Qn6, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00D7, /*0x0100010800FF,*/ CMon_MD_Qn7, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00D8, /*0x0100010800FF,*/ CMon_MD_Qn8, DLen_3, 	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,

	//-------------最大需量发生时间-------
	0x00FD, /*0x0100010600FF,*/ CMon_MDT_Pp0, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00FE, /*0x0100010600FF,*/ CMon_MDT_Pp1, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x00FF, /*0x0100010600FF,*/ CMon_MDT_Pp2, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0100, /*0x0100010600FF,*/ CMon_MDT_Pp3, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0101, /*0x0100010600FF,*/ CMon_MDT_Pp4, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0102, /*0x0100010600FF,*/ CMon_MDT_Pp5, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0103, /*0x0100010600FF,*/ CMon_MDT_Pp6, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0104, /*0x0100010600FF,*/ CMon_MDT_Pp7, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0105, /*0x0100010600FF,*/ CMon_MDT_Pp8, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,

	0x0106, /*0x0100020600FF,*/ CMon_MDT_Pn0, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0107, /*0x0100020600FF,*/ CMon_MDT_Pn1, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0108, /*0x0100020600FF,*/ CMon_MDT_Pn2, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0109, /*0x0100020600FF,*/ CMon_MDT_Pn3, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x010A, /*0x0100020600FF,*/ CMon_MDT_Pn4, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x010B, /*0x0100020600FF,*/ CMon_MDT_Pn5, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x010C, /*0x0100020600FF,*/ CMon_MDT_Pn6, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x010D, /*0x0100020600FF,*/ CMon_MDT_Pn7, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x010E, /*0x0100020600FF,*/ CMon_MDT_Pn8, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,

	0x010F, /*0x0100030600FF,*/ CMon_MDT_Qp0, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0110, /*0x0100030600FF,*/ CMon_MDT_Qp1, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0111, /*0x0100030600FF,*/ CMon_MDT_Qp2, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0112, /*0x0100030600FF,*/ CMon_MDT_Qp3, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0X0113, /*0x0100030600FF,*/ CMon_MDT_Qp4, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0114, /*0x0100030600FF,*/ CMon_MDT_Qp5, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0115, /*0x0100030600FF,*/ CMon_MDT_Qp6, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0116, /*0x0100030600FF,*/ CMon_MDT_Qp7, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0117, /*0x0100030600FF,*/ CMon_MDT_Qp8, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,

	0x0118, /*0x0100030600FF,*/ CMon_MDT_Qn0, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0119, /*0x0100030600FF,*/ CMon_MDT_Qn1, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x011A, /*0x0100030600FF,*/ CMon_MDT_Qn2, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x011B, /*0x0100030600FF,*/ CMon_MDT_Qn3, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x011C, /*0x0100030600FF,*/ CMon_MDT_Qn4, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x011D, /*0x0100030600FF,*/ CMon_MDT_Qn5, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x011E, /*0x0100030600FF,*/ CMon_MDT_Qn6, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x011F, /*0x0100030600FF,*/ CMon_MDT_Qn7, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
	0x0120, /*0x0100030600FF,*/ CMon_MDT_Qn8, DLen_5,	F_DRS_ReadEN+F_DRS_SYNData+DRS_Eeprom2,
		
	0x0D13, /*0x0100010800FF,*/ 0x0000, DLen_4, 	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2+F_DRS_SYNData, //超最大电流事件电流阈值
	0x0D14, /*0x0100010800FF,*/ 0x0000, DLen_1, 	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2+F_DRS_SYNData, //继电器拉合闸(0xaa表示合闸，0x55表示拉闸)
	0x0D15, /*0x0100010800FF,*/ 0x0000, DLen_1, 	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom2+F_DRS_SYNData, //信号强度(0,1,2,3,4)


	///-------校表---------------------
	#if 0
//匈牙利表校表，

	//----------1p meter---------
	0xF1C1,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////  电压
	0xF1C2,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////电流有效值偏置       
	0xF1C3,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////电流            
	0xF1C4,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////功率增益          
	0xF1C5,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////功率偏置                    
	0xF1C6,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////相位            
	0xF1C7,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////第一段非线性的分界点    
	0xF1C8,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////第二段非线性的分界点
	0xF1C9,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////第三段非线性的分界点  
	0xF1CA,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////第一个分界点以下的相位校准            
	0xF1CB,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////第一个分界点以上与第二个分界点            
	0xF1CC,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////第二个分界点以上与第三个分界点          
	0xF1CD,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////第三个分界点以上的相位校准值                    
	0xF1CE,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////校验和           
	///---------3p meter+F_DRS_SYNData
	0xF3C1,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_6,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_6,	DLen_0,		0,*/	////A相B相C相电压校准基准(7)   
	0xF3C2,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_6,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_6,	DLen_0,		0,*/	////校表电流系数(7)            
	0xF3C3,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_6,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_6,	DLen_0,		0,*/	////校表电压系数(7)            
	0xF3C4,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_6,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_6,	DLen_0,		0,*/	////校表有功offset(7)          
	0xF3C5,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_6,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_6,	DLen_0,		0,*/	////相位(7)                    
	0xF3C6,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_6,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_6,	DLen_0,		0,*/	////校表无功offset(7)            
	0xF3C7,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_6,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_6,	DLen_0,		0,*/	////无功相位(7)    
	0xF3C8,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_2,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	////时钟精度
#endif
	0xF4C1,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4C2,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4C3,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4C4,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4C5,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4C6,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4C7,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4C8,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4C9,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4CA,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4CB,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4CC,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4CD,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4CE,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4CF,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D0,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D1,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D2,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D3,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D4,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D5,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D6,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D7,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////
	0xF4D8,/*	0x01,0x00,0x01,0x08,0x00,0xFF,*/	0x0000,		DLen_4,		F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, /*		DLen_4,	DLen_0,		0,*/	////

	0x1001,/*0x01,0x01,0x00,0x00,0x01,0x80,*/	0x0000,	DLen_30,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	//轮显第一屏显示 
	0x1002,/*0x01,0x02,0x00,0x00,0x01,0x80,*/	0x0000,	DLen_30,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	//轮显第二屏显示 
	0x1003,/*0x01,0x03,0x00,0x00,0x01,0x80,*/	0x0000,	DLen_30,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	//轮显第三屏显示 
	0x1004,/*0x01,0x04,0x00,0x00,0x01,0x80,*/	0x0000,	DLen_30,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	//按显第一屏 ,
	0x1005,/*0x01,0x05,0x00,0x00,0x01,0x80,*/	0x0000,	DLen_30,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	//按显第二屏 ,
	0x1006,/*0x01,0x06,0x00,0x00,0x01,0x80,*/	0x0000,	DLen_30,	F_DRS_ReadEN+F_DRS_WriteEN+DRS_Eeprom1+F_DRS_SYNData, 	/*	DLen_2,	DLen_0,		0,*/	//按显第三屏 ,
};

const ID_OBIS Double_ECTab[]=
{
	EC.RCMon_EC_Pp0,0x0001,  DLen_6,      //∑Li Import active energy (QI+QIV)
	EC.RCMon_EC_Pp1,0x0002,  DLen_6,             
	EC.RCMon_EC_Pp2,0x0003,  DLen_6,             
	EC.RCMon_EC_Pp3,0x0004,  DLen_6,             
	EC.RCMon_EC_Pp4,0x0005,  DLen_6,  
#if(Total_Fee==8)
	EC.RCMon_EC_Pp5,0x0006,  DLen_6,    		
	EC.RCMon_EC_Pp6,0x0007,  DLen_6,    	
	EC.RCMon_EC_Pp7,0x0008,  DLen_6,    		
	EC.RCMon_EC_Pp8,0x0009,  DLen_6,    		
#endif

	EC.RCMon_EC_Pn0,0x000a,  DLen_6,    //∑Li Export active energy (QII+QIII)            
	EC.RCMon_EC_Pn1,0x000b,  DLen_6,                
	EC.RCMon_EC_Pn2,0x000c,  DLen_6,                
	EC.RCMon_EC_Pn3,0x000d,  DLen_6,                
	EC.RCMon_EC_Pn4,0x000e,  DLen_6,
#if(Total_Fee==8)
	EC.RCMon_EC_Pn5,0x000f,  DLen_6,   			   
	EC.RCMon_EC_Pn6,0x0010,  DLen_6,   			   
	EC.RCMon_EC_Pn7,0x0011,  DLen_6,   			   
	EC.RCMon_EC_Pn8,0x0012,  DLen_6,   			   
#endif
          
	EC.RCMon_EC_Qp0,0x0013,  DLen_6,     //∑Li Import reactive energy (QI+QII)           
	EC.RCMon_EC_Qp1,0x0014,  DLen_6,                
	EC.RCMon_EC_Qp2,0x0015,  DLen_6,                
	EC.RCMon_EC_Qp3,0x0016,  DLen_6,                
	EC.RCMon_EC_Qp4,0x0017,  DLen_6,  
#if(Total_Fee==8)
	EC.RCMon_EC_Qp5,0x0018,  DLen_6,   			   
	EC.RCMon_EC_Qp6,0x0019,  DLen_6,   			   
	EC.RCMon_EC_Qp7,0x001a,  DLen_6,   			   
	EC.RCMon_EC_Qp8,0x001b,  DLen_6,   			   
#endif
                                      
	EC.RCMon_EC_Qn0,0x001c,  DLen_6,    //∑Li Export reactive energy (QIII+QIV)                   
	EC.RCMon_EC_Qn1,0x001d,  DLen_6,                       
	EC.RCMon_EC_Qn2,0x001e,  DLen_6,                       
	EC.RCMon_EC_Qn3,0x001f,  DLen_6,                       
	EC.RCMon_EC_Qn4,0x0020,  DLen_6,   
#if(Total_Fee==8)
	EC.RCMon_EC_Qn5,0x0021,  DLen_6,   					  
	EC.RCMon_EC_Qn6,0x0022,  DLen_6,   					  
	EC.RCMon_EC_Qn7,0x0023,  DLen_6,   					  
	EC.RCMon_EC_Qn8,0x0024,  DLen_6,   					  
#endif

#if(MeterPhase== Threephase)
	EC.RCMon_EC_Q10,0x0025,  DLen_6,     //∑Li Reactive energy (QI)                  
	EC.RCMon_EC_Q11,0x0026,  DLen_6,                       
	EC.RCMon_EC_Q12,0x0027,  DLen_6,                       
	EC.RCMon_EC_Q13,0x0028,  DLen_6,                       
	EC.RCMon_EC_Q14,0x0029,  DLen_6, 
#if(Total_Fee==8)	
	EC.RCMon_EC_Q15,0x002a,  DLen_6,   					  
	EC.RCMon_EC_Q16,0x002b,  DLen_6,   					  
	EC.RCMon_EC_Q17,0x002c,  DLen_6,   					  
	EC.RCMon_EC_Q18,0x002d,  DLen_6,   					  
#endif

	EC.RCMon_EC_Q20,0x002e,  DLen_6,     //∑Li Reactive energy (QII)                  
	EC.RCMon_EC_Q21,0x002f,  DLen_6,                       
	EC.RCMon_EC_Q22,0x0030,  DLen_6,                       
	EC.RCMon_EC_Q23,0x0031,  DLen_6,                       
	EC.RCMon_EC_Q24,0x0032,  DLen_6,  
#if(Total_Fee==8)
	EC.RCMon_EC_Q25,0x0033,  DLen_6,   					  
	EC.RCMon_EC_Q26,0x0034,  DLen_6,   					  
	EC.RCMon_EC_Q27,0x0035,  DLen_6,   					  
	EC.RCMon_EC_Q28,0x0036,  DLen_6,   					  
#endif

	EC.RCMon_EC_Q30,0x0037,  DLen_6,    //∑Li Reactive energy (QIII)                   
	EC.RCMon_EC_Q31,0x0038,  DLen_6,                       
	EC.RCMon_EC_Q32,0x0039,  DLen_6,                       
	EC.RCMon_EC_Q33,0x003a,  DLen_6,                       
	EC.RCMon_EC_Q34,0x003b,  DLen_6, 
#if(Total_Fee==8)
	EC.RCMon_EC_Q35,0x003c,  DLen_6,   					  
	EC.RCMon_EC_Q36,0x003d,  DLen_6,   					  
	EC.RCMon_EC_Q37,0x003e,  DLen_6,   					  
	EC.RCMon_EC_Q38,0x003f,  DLen_6,   					  
#endif
     
	EC.RCMon_EC_Q40,0x0040,  DLen_6,     //∑Li Reactive energy (QIV)                  
	EC.RCMon_EC_Q41,0x0041,  DLen_6,                       
	EC.RCMon_EC_Q42,0x0042,  DLen_6,                       
	EC.RCMon_EC_Q43,0x0043,  DLen_6,                       
	EC.RCMon_EC_Q44,0x0044,  DLen_6, 
#if(Total_Fee==8)
	EC.RCMon_EC_Q45,0x0045,  DLen_6,   					  
	EC.RCMon_EC_Q46,0x0046,  DLen_6,   					  
	EC.RCMon_EC_Q47,0x0047,  DLen_6,   					  
	EC.RCMon_EC_Q48,0x0048,  DLen_6,   	
#endif
#endif
 #if 0                                    
	CMon_EC_Sp0,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x00,0xff,    Unit_VAh,	0x00,  //∑Li Import apparent energy (QI+QIV)                   
	CMon_EC_Sp1,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x01,0xff,    Unit_VAh,	0x00,                     
	CMon_EC_Sp2,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x02,0xff,    Unit_VAh,	0x00,                     
	CMon_EC_Sp3,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x03,0xff,    Unit_VAh,	0x00,                     
	CMon_EC_Sp4,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x04,0xff,    Unit_VAh,	0x00,                     
	CMon_EC_Sp5,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x05,0xff,    Unit_VAh,	0x00,
	CMon_EC_Sp6,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x06,0xff,    Unit_VAh,	0x00,					  
	CMon_EC_Sp7,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x07,0xff,    Unit_VAh,	0x00,					  		
	CMon_EC_Sp8,	READ+BYTE8,	 0x01,0x00,0x09,0x08,0x08,0xff,    Unit_VAh,	0x00, 

	CMon_EC_Sn0,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x00,0xff,    Unit_VAh,	0x00,  //∑Li Export apparent energy (QII+QIII)                          
	CMon_EC_Sn1,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x01,0xff,    Unit_VAh,	0x00,                            
	CMon_EC_Sn2,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x02,0xff,    Unit_VAh,	0x00,                            
	CMon_EC_Sn3,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x03,0xff,    Unit_VAh,	0x00,                            
	CMon_EC_Sn4,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x04,0xff,    Unit_VAh,	0x00,                            
	CMon_EC_Sn5,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x05,0xff,    Unit_VAh,	0x00,							 
	CMon_EC_Sn6,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x06,0xff,    Unit_VAh,	0x00,							 
	CMon_EC_Sn7,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x07,0xff,    Unit_VAh,	0x00,							 
	CMon_EC_Sn8,	READ+BYTE8,	 0x01,0x00,0x0A,0x08,0x08,0xff,    Unit_VAh,	0x00,							 

	CMon_EC_P0,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x00,0xff,    Unit_Wh, 	0x00,  // ∑Li Active energy (abs(QI+QIV)+(abs(QII+QIII))
	CMon_EC_P1,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x01,0xff,    Unit_Wh, 	0x00,		   
	CMon_EC_P2,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x02,0xff,    Unit_Wh, 	0x00,		   
	CMon_EC_P3,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x03,0xff,    Unit_Wh, 	0x00,		   
	CMon_EC_P4,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x04,0xff,    Unit_Wh, 	0x00,		
	CMon_EC_P5,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x05,0xff,    Unit_Wh, 	0x00,		
	CMon_EC_P6,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x06,0xff,    Unit_Wh, 	0x00,		
	CMon_EC_P7,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x07,0xff,    Unit_Wh, 	0x00,		
	CMon_EC_P8,   READ+BYTE8,	 0x01,0x00,0x0F,0x08,0x08,0xff,    Unit_Wh, 	0x00,		

#if(CTPT_Meter==YesCheck)	
	CMon_EC_PpA0,	READ+0x04,	 0x01,0x00,0x15,0x08,0x00,0xff,    Unit_Wh,		0x00,              
	CMon_EC_PnA0,	READ+0x04,	 0x01,0x00,0x16,0x08,0x00,0xff,    Unit_Wh,		0x00,              
	CMon_EC_QpA0,	READ+0x04,	 0x01,0x00,0x17,0x08,0x00,0xff,    Unit_varh,	0x00,        		
	CMon_EC_QnA0,	READ+0x04,	 0x01,0x00,0x18,0x08,0x00,0xff,    Unit_varh,	0x00,       		
	CMon_EC_Q1A0,	READ+BYTE8,	 0x01,0x00,0x19,0x08,0x00,0xff,    Unit_varh,	0x00,       		
	CMon_EC_Q2A0,	READ+BYTE8,	 0x01,0x00,0x1A,0x08,0x00,0xff,    Unit_varh,	0x00,       		
	CMon_EC_Q3A0,	READ+BYTE8,	 0x01,0x00,0x1B,0x08,0x00,0xff,    Unit_varh,	0x00,       		
	CMon_EC_Q4A0,	READ+BYTE8,	 0x01,0x00,0x1C,0x08,0x00,0xff,    Unit_varh,	0x00,       		       		
	CMon_EC_SpA0,	READ+0x04,	 0x01,0x00,0x1D,0x08,0x00,0xff,    Unit_VAh,	0x00,                                   		
	CMon_EC_SnA0,	READ+0x04,	 0x01,0x00,0x1E,0x08,0x00,0xff,    Unit_VAh,	0x00,                                   		
                                         
	CMon_EC_PpB0,	READ+0x04,	 0x01,0x00,0x29,0x08,0x00,0xff,    Unit_Wh,		0x00,                                                                                            	
	CMon_EC_PnB0,	READ+0x04,	 0x01,0x00,0x2A,0x08,0x00,0xff,    Unit_Wh,		0x00,                     
	CMon_EC_QpB0,	READ+0x04,	 0x01,0x00,0x2B,0x08,0x00,0xff,    Unit_varh,	0x00,        		                                    
	CMon_EC_QnB0,	READ+0x04,	 0x01,0x00,0x2C,0x08,0x00,0xff,    Unit_varh,	0x00, 	
	CMon_EC_Q1B0,	READ+BYTE8,	 0x01,0x00,0x2d,0x08,0x00,0xff,    Unit_varh,	0x00,        		                                    
	CMon_EC_Q2B0,	READ+BYTE8,	 0x01,0x00,0x2e,0x08,0x00,0xff,    Unit_varh,	0x00,	
	CMon_EC_Q3B0,	READ+BYTE8,	 0x01,0x00,0x2f,0x08,0x00,0xff,    Unit_varh,	0x00,
	CMon_EC_Q4B0,	READ+BYTE8,	 0x01,0x00,0x30,0x08,0x00,0xff,    Unit_varh,	0x00,	
	CMon_EC_SpB0,	READ+0x04,	 0x01,0x00,0x31,0x08,0x00,0xff,    Unit_VAh,	0x00,                                   		         
	CMon_EC_SnB0,	READ+0x04,	 0x01,0x00,0x32,0x08,0x00,0xff,    Unit_VAh,	0x00,                                   		         
                                         
	CMon_EC_PpC0,	READ+0x04,	 0x01,0x00,0x3D,0x08,0x00,0xff,    Unit_Wh,		0x00,              
	CMon_EC_PnC0,	READ+0x04,	 0x01,0x00,0x3E,0x08,0x00,0xff,    Unit_Wh,		0x00,              
	CMon_EC_QpC0,	READ+0x04,	 0x01,0x00,0x3F,0x08,0x00,0xff,    Unit_varh,	0x00,        		                                           
	CMon_EC_QnC0,	READ+0x04,	 0x01,0x00,0x40,0x08,0x00,0xff,    Unit_varh,	0x00,       		                                        
	CMon_EC_Q1C0,	READ+BYTE8,	 0x01,0x00,0x41,0x08,0x00,0xff,    Unit_varh,	0x00,       		                                    
	CMon_EC_Q2C0,	READ+BYTE8,	 0x01,0x00,0x42,0x08,0x00,0xff,    Unit_varh,	0x00,       		                                    
	CMon_EC_Q3C0,	READ+BYTE8,	 0x01,0x00,0x43,0x08,0x00,0xff,    Unit_varh,	0x00,       		                                    
	CMon_EC_Q4C0,	READ+BYTE8,	 0x01,0x00,0x44,0x08,0x00,0xff,    Unit_varh,	0x00,       		       		                                                                                                                                                       
	CMon_EC_SpC0,	READ+0x04,	 0x01,0x00,0x45,0x08,0x00,0xff,    Unit_VAh,	0x00,                                   		        
	CMon_EC_SnC0,	READ+0x04,	 0x01,0x00,0x46,0x08,0x00,0xff,    Unit_VAh,	0x00, 
#endif
#endif
};

const ID_OBIS Double_RealTab[]=
{
	(unsigned char*)&Real.Ua,0x0085,	/*0x01,0x00,0x20,0x07,0x00,0xff,*/DLen_4,//A相电压
	(unsigned char*)&Real.Ia,0x0088,	/*0x01,0x00,0x1F,0x07,0x00,0xff,*/DLen_4,//A相电流
	#if(MeterPhase==Threephase)
	(unsigned char*)&Real.Ub,0x0086,	/*0x01,0x00,0x34,0x07,0x00,0xff,*/DLen_4,//B相电压
	(unsigned char*)&Real.Uc,0x0087,	/*0x01,0x00,0x48,0x07,0x00,0xff,*/DLen_4,//C相电压
	(unsigned char*)&Real.Ib,0x0089,	/*0x01,0x00,0x33,0x07,0x00,0xff,*/DLen_4,//B相电流
	(unsigned char*)&Real.Ic,0x008A,	/*0x01,0x00,0x47,0x07,0x00,0xff,*/DLen_4,//C相电流
	#endif
	(unsigned char*)&Real.Pft,    0x0096,	/*0x01,0x00,0x0D,0x07,0x00,0xff,*/DLen_4,//合相功率因素
	(unsigned char*)&Real.Freq,   0x00C2,	/*0x01,0x00,0x0E,0x07,0x00,0xff,*/DLen_4,//当前频率

#if(MeterPhase==SinglePhase)
	(unsigned char*)&Real.PowerPp,0x00C3,	/*0x01,0x00,0x01,0x07,0x00,0xff,*/DLen_3,//ActiveImportPower
	(unsigned char*)&Real.PowerPn,0x00C4,	/*0x01,0x00,0x02,0x07,0x00,0xff,*/DLen_3,//ActiveExportPower
	(unsigned char*)&Real.PowerQp,0x00C5,	/*0x01,0x00,0x03,0x07,0x00,0xff,*/DLen_3,//ReactiveImportPower
	(unsigned char*)&Real.PowerQn,0x00C6,	/*0x01,0x00,0x04,0x07,0x00,0xff,*/DLen_3,//ReactiveExportPower
#endif

};
const ID_OBIS Double_DemandTab[]=
{
	//----------需量----------------------
	Demand_Class5.Demand_Pp,   0x00D9,	/*0x01,0x00,0x01,0x04,0x00,0xff,*/DLen_3,//正向有功总需量
	Demand_Class5.Demand_PpFee,0x00DA,	/*0x01,0x00,0x01,0x04,0x01,0xff,*/DLen_3,//正向有功总需量T1
	Demand_Class5.Demand_PpFee,0x00DB,	/*0x01,0x00,0x01,0x04,0x02,0xff,*/DLen_3,//正向有功总需量T2
	Demand_Class5.Demand_PpFee,0x00DC,	/*0x01,0x00,0x01,0x04,0x03,0xff,*/DLen_3,//正向有功总需量T3	
	Demand_Class5.Demand_PpFee,0x00DD,	/*0x01,0x00,0x01,0x04,0x04,0xff,*/DLen_3,//正向有功总需量T4
#if(Total_Fee==8)
	Demand_Class5.Demand_PpFee,0x00DE,	/*0x01,0x00,0x01,0x04,0x05,0xff,*/DLen_3,//正向有功总需量T5
	Demand_Class5.Demand_PpFee,0x00DF,	/*0x01,0x00,0x01,0x04,0x06,0xff,*/DLen_3,//正向有功总需量T6
	Demand_Class5.Demand_PpFee,0x00E0,	/*0x01,0x00,0x01,0x04,0x07,0xff,*/DLen_3,//正向有功总需量T7 
	Demand_Class5.Demand_PpFee,0x00E1,	/*0x01,0x00,0x01,0x04,0x08,0xff,*/DLen_3,//正向有功总需量T8
#endif
	
	Demand_Class5.Demand_Qp,   0x00E2,	/*0x01,0x00,0x03,0x04,0x00,0xff,*/DLen_3,//正向无功总需量
	Demand_Class5.Demand_QpFee,0x00E3,	/*0x01,0x00,0x03,0x04,0x01,0xff,*/DLen_3,//正向无功总需量T1
	Demand_Class5.Demand_QpFee,0x00E4,	/*0x01,0x00,0x03,0x04,0x02,0xff,*/DLen_3,//正向无功总需量T2
	Demand_Class5.Demand_QpFee,0x00E5,	/*0x01,0x00,0x03,0x04,0x03,0xff,*/DLen_3,//正向无功总需量T3
	Demand_Class5.Demand_QpFee,0x00E6,	/*0x01,0x00,0x03,0x04,0x04,0xff,*/DLen_3,//正向无功总需量T4
#if(Total_Fee==8)
	Demand_Class5.Demand_QpFee,0x00E7,	/*0x01,0x00,0x03,0x04,0x05,0xff,*/DLen_3,//正向无功总需量T5
	Demand_Class5.Demand_QpFee,0x00E8,	/*0x01,0x00,0x03,0x04,0x06,0xff,*/DLen_3,//正向无功总需量T6
	Demand_Class5.Demand_QpFee,0x00E9,	/*0x01,0x00,0x03,0x04,0x07,0xff,*/DLen_3,//正向无功总需量T7
	Demand_Class5.Demand_QpFee,0x00EA,	/*0x01,0x00,0x03,0x04,0x08,0xff,*/DLen_3,//正向无功总需量T8
#endif
	
	Demand_Class5.Demand_Pn,   0x00EB,	/*0x01,0x00,0x02,0x04,0x00,0xff,*/DLen_3,//反向有功总需量
	Demand_Class5.Demand_PnFee,0x00EC,	/*0x01,0x00,0x02,0x04,0x01,0xff,*/DLen_3,//反向有功总需量T1
	Demand_Class5.Demand_PnFee,0x00ED,	/*0x01,0x00,0x02,0x04,0x02,0xff,*/DLen_3,//反向有功总需量T2
	Demand_Class5.Demand_PnFee,0x00EE,	/*0x01,0x00,0x02,0x04,0x03,0xff,*/DLen_3,//反向有功总需量T3
	Demand_Class5.Demand_PnFee,0x00EF,	/*0x01,0x00,0x02,0x04,0x04,0xff,*/DLen_3,//反向有功总需量T4
#if(Total_Fee==8)
	Demand_Class5.Demand_PnFee,0x00F0,	/*0x01,0x00,0x02,0x04,0x05,0xff,*/DLen_3,//反向有功总需量T5
	Demand_Class5.Demand_PnFee,0x00F1,	/*0x01,0x00,0x02,0x04,0x06,0xff,*/DLen_3,//反向有功总需量T6
	Demand_Class5.Demand_PnFee,0x00F2,	/*0x01,0x00,0x02,0x04,0x07,0xff,*/DLen_3,//反向有功总需量T7
	Demand_Class5.Demand_PnFee,0x00F3,	/*0x01,0x00,0x02,0x04,0x08,0xff,*/DLen_3,//反向有功总需量T8
#endif
	
	Demand_Class5.Demand_Qn,   0x00F4,	/*0x01,0x00,0x04,0x04,0x00,0xff,*/DLen_3,//反向无功总需量
	Demand_Class5.Demand_QnFee,0x00F5,	/*0x01,0x00,0x04,0x04,0x01,0xff,*/DLen_3,//反向无功总需量T1
	Demand_Class5.Demand_QnFee,0x00F6,	/*0x01,0x00,0x04,0x04,0x02,0xff,*/DLen_3,//反向无功总需量T2
	Demand_Class5.Demand_QnFee,0x00F7,	/*0x01,0x00,0x04,0x04,0x03,0xff,*/DLen_3,//反向无功总需量T3
	Demand_Class5.Demand_QnFee,0x00F8,	/*0x01,0x00,0x04,0x04,0x04,0xff,*/DLen_3,//反向无功总需量T4
#if(Total_Fee==8)
	Demand_Class5.Demand_QnFee,0x00F9,	/*0x01,0x00,0x04,0x04,0x05,0xff,*/DLen_3,//反向无功总需量T5
	Demand_Class5.Demand_QnFee,0x00FA,	/*0x01,0x00,0x04,0x04,0x06,0xff,*/DLen_3,//反向无功总需量T6
	Demand_Class5.Demand_QnFee,0x00FB,	/*0x01,0x00,0x04,0x04,0x07,0xff,*/DLen_3,//反向无功总需量T7
	Demand_Class5.Demand_QnFee,0x00FC,	/*0x01,0x00,0x04,0x04,0x08,0xff,*/DLen_3,//反向无功总需量T8
#endif
};
const ID_MAXDemand_OBIS MaxDemandTab[]=
{
/*
	CMon_MD_Pp0,0x004F,	CMon_MDT_Pp0,0x00FD,0x01,0x00,0x01,0x06,0x00,0xff,	DLen_3,
	CMon_MD_Pp1,0x0050,	CMon_MDT_Pp1,0x00FE,0x01,0x00,0x01,0x06,0x01,0xff,	DLen_3,
	CMon_MD_Pp2,0x0051,	CMon_MDT_Pp2,0x00FF,0x01,0x00,0x01,0x06,0x02,0xff,	DLen_3,
 	CMon_MD_Pp3,0x0052,	CMon_MDT_Pp3,0x0100,0x01,0x00,0x01,0x06,0x03,0xff,	DLen_3,
	CMon_MD_Pp4,0x0053,	CMon_MDT_Pp4,0x0101,0x01,0x00,0x01,0x06,0x04,0xff,	DLen_3,
	CMon_MD_Pp5,0x0054,	CMon_MDT_Pp5,0x0102,0x01,0x00,0x01,0x06,0x05,0xff,	DLen_3,
	CMon_MD_Pp6,0x0055,	CMon_MDT_Pp6,0x0103,0x01,0x00,0x01,0x06,0x06,0xff,	DLen_3,
	CMon_MD_Pp7,0x0056,	CMon_MDT_Pp7,0x0104,0x01,0x00,0x01,0x06,0x07,0xff,	DLen_3,
	CMon_MD_Pp8,0x0057,	CMon_MDT_Pp8,0x0105,0x01,0x00,0x01,0x06,0x08,0xff,	DLen_3,	

	//--------------新补充的ID填充-------
	CMon_MD_Pn0,0x00C7,	CMon_MDT_Pn0,0x0106,0x01,0x00,0x02,0x06,0x00,0xff, 	DLen_3,
	CMon_MD_Pn1,0x00C8,	CMon_MDT_Pn1,0x0107,0x01,0x00,0x02,0x06,0x01,0xff, 	DLen_3,
	CMon_MD_Pn2,0x00C9,	CMon_MDT_Pn2,0x0108,0x01,0x00,0x02,0x06,0x02,0xff, 	DLen_3,
 	CMon_MD_Pn3,0x00CA,	CMon_MDT_Pn3,0x0109,0x01,0x00,0x02,0x06,0x03,0xff, 	DLen_3,
	CMon_MD_Pn4,0x00CB,	CMon_MDT_Pn4,0x010A,0x01,0x00,0x02,0x06,0x04,0xff, 	DLen_3,
	CMon_MD_Pn5,0x00CC,	CMon_MDT_Pn5,0x010B,0x01,0x00,0x02,0x06,0x05,0xff, 	DLen_3,
	CMon_MD_Pn6,0x00CD,	CMon_MDT_Pn6,0x010C,0x01,0x00,0x02,0x06,0x06,0xff, 	DLen_3,
	CMon_MD_Pn7,0x00CE,	CMon_MDT_Pn7,0x010D,0x01,0x00,0x02,0x06,0x07,0xff, 	DLen_3,
	CMon_MD_Pn8,0x00CF,	CMon_MDT_Pn8,0x010E,0x01,0x00,0x02,0x06,0x08,0xff, 	DLen_3,	

	CMon_MD_Qp0,0x0058,	CMon_MDT_Qp0,0x010F,0x01,0x00,0x03,0x06,0x00,0xff,	DLen_3,
	CMon_MD_Qp1,0x0059,	CMon_MDT_Qp1,0x0110,0x01,0x00,0x03,0x06,0x01,0xff,	DLen_3,
	CMon_MD_Qp2,0x005A,	CMon_MDT_Qp2,0x0111,0x01,0x00,0x03,0x06,0x02,0xff,	DLen_3,
	CMon_MD_Qp3,0x005B,	CMon_MDT_Qp3,0x0112,0x01,0x00,0x03,0x06,0x03,0xff,	DLen_3,
	CMon_MD_Qp4,0x005C,	CMon_MDT_Qp4,0X0113,0x01,0x00,0x03,0x06,0x04,0xff,	DLen_3,
	CMon_MD_Qp5,0x005D,	CMon_MDT_Qp5,0x0114,0x01,0x00,0x03,0x06,0x05,0xff,	DLen_3,
	CMon_MD_Qp6,0x005E,	CMon_MDT_Qp6,0x0115,0x01,0x00,0x03,0x06,0x06,0xff,	DLen_3,
	CMon_MD_Qp7,0x005F,	CMon_MDT_Qp7,0x0116,0x01,0x00,0x03,0x06,0x07,0xff,	DLen_3,
	CMon_MD_Qp8,0x0060,	CMon_MDT_Qp8,0x0117,0x01,0x00,0x03,0x06,0x08,0xff,	DLen_3,

	//--------------新补充的ID填充-------
	CMon_MD_Qn0,0x00D0,	CMon_MDT_Qn0,0x0118,0x01,0x00,0x04,0x06,0x00,0xff,	DLen_3,
	CMon_MD_Qn1,0x00D1,	CMon_MDT_Qn1,0x0119,0x01,0x00,0x04,0x06,0x01,0xff,	DLen_3,
	CMon_MD_Qn2,0x00D2,	CMon_MDT_Qn2,0x011A,0x01,0x00,0x04,0x06,0x02,0xff,	DLen_3,
	CMon_MD_Qn3,0x00D3,	CMon_MDT_Qn3,0x011B,0x01,0x00,0x04,0x06,0x03,0xff,	DLen_3,
	CMon_MD_Qn4,0x00D4,	CMon_MDT_Qn4,0x011C,0x01,0x00,0x04,0x06,0x04,0xff,	DLen_3,
	CMon_MD_Qn5,0x00D5,	CMon_MDT_Qn5,0x011D,0x01,0x00,0x04,0x06,0x05,0xff,	DLen_3,
	CMon_MD_Qn6,0x00D6,	CMon_MDT_Qn6,0x011E,0x01,0x00,0x04,0x06,0x06,0xff,	DLen_3,
	CMon_MD_Qn7,0x00D7,	CMon_MDT_Qn7,0x011F,0x01,0x00,0x04,0x06,0x07,0xff,	DLen_3,
	CMon_MD_Qn8,0x00D8,	CMon_MDT_Qn8,0x0120,0x01,0x00,0x04,0x06,0x08,0xff,	DLen_3,	
	*/
	CMon_MD_Pp0,0x00D9,	CMon_MDT_Pp0,0x00FD,0x01,0x00,0x01,0x06,0x00,0xff,	DLen_3,
	CMon_MD_Pp1,0x00DA,	CMon_MDT_Pp1,0x00FE,0x01,0x00,0x01,0x06,0x01,0xff,	DLen_3,
	CMon_MD_Pp2,0x00DB,	CMon_MDT_Pp2,0x00FF,0x01,0x00,0x01,0x06,0x02,0xff,	DLen_3,
 	CMon_MD_Pp3,0x00DC,	CMon_MDT_Pp3,0x0100,0x01,0x00,0x01,0x06,0x03,0xff,	DLen_3,
	CMon_MD_Pp4,0x00DD,	CMon_MDT_Pp4,0x0101,0x01,0x00,0x01,0x06,0x04,0xff,	DLen_3,
	CMon_MD_Pp5,0x00DE,	CMon_MDT_Pp5,0x0102,0x01,0x00,0x01,0x06,0x05,0xff,	DLen_3,
	CMon_MD_Pp6,0x00DF,	CMon_MDT_Pp6,0x0103,0x01,0x00,0x01,0x06,0x06,0xff,	DLen_3,
	CMon_MD_Pp7,0x00E0,	CMon_MDT_Pp7,0x0104,0x01,0x00,0x01,0x06,0x07,0xff,	DLen_3,
	CMon_MD_Pp8,0x00E1,	CMon_MDT_Pp8,0x0105,0x01,0x00,0x01,0x06,0x08,0xff,	DLen_3,	

	//--------------新补充的ID填充-------
	CMon_MD_Pn0,0x00EB,	CMon_MDT_Pn0,0x0106,0x01,0x00,0x02,0x06,0x00,0xff, 	DLen_3,
	CMon_MD_Pn1,0x00EC,	CMon_MDT_Pn1,0x0107,0x01,0x00,0x02,0x06,0x01,0xff, 	DLen_3,
	CMon_MD_Pn2,0x00ED,	CMon_MDT_Pn2,0x0108,0x01,0x00,0x02,0x06,0x02,0xff, 	DLen_3,
 	CMon_MD_Pn3,0x00EE,	CMon_MDT_Pn3,0x0109,0x01,0x00,0x02,0x06,0x03,0xff, 	DLen_3,
	CMon_MD_Pn4,0x00EF,	CMon_MDT_Pn4,0x010A,0x01,0x00,0x02,0x06,0x04,0xff, 	DLen_3,
	CMon_MD_Pn5,0x00F0,	CMon_MDT_Pn5,0x010B,0x01,0x00,0x02,0x06,0x05,0xff, 	DLen_3,
	CMon_MD_Pn6,0x00F1,	CMon_MDT_Pn6,0x010C,0x01,0x00,0x02,0x06,0x06,0xff, 	DLen_3,
	CMon_MD_Pn7,0x00F2,	CMon_MDT_Pn7,0x010D,0x01,0x00,0x02,0x06,0x07,0xff, 	DLen_3,
	CMon_MD_Pn8,0x00F3,	CMon_MDT_Pn8,0x010E,0x01,0x00,0x02,0x06,0x08,0xff, 	DLen_3,	

	CMon_MD_Qp0,0x00E2,	CMon_MDT_Qp0,0x010F,0x01,0x00,0x03,0x06,0x00,0xff,	DLen_3,
	CMon_MD_Qp1,0x00E3,	CMon_MDT_Qp1,0x0110,0x01,0x00,0x03,0x06,0x01,0xff,	DLen_3,
	CMon_MD_Qp2,0x00E4,	CMon_MDT_Qp2,0x0111,0x01,0x00,0x03,0x06,0x02,0xff,	DLen_3,
	CMon_MD_Qp3,0x00E5,	CMon_MDT_Qp3,0x0112,0x01,0x00,0x03,0x06,0x03,0xff,	DLen_3,
	CMon_MD_Qp4,0x00E6,	CMon_MDT_Qp4,0X0113,0x01,0x00,0x03,0x06,0x04,0xff,	DLen_3,
	CMon_MD_Qp5,0x00E7,	CMon_MDT_Qp5,0x0114,0x01,0x00,0x03,0x06,0x05,0xff,	DLen_3,
	CMon_MD_Qp6,0x00E8,	CMon_MDT_Qp6,0x0115,0x01,0x00,0x03,0x06,0x06,0xff,	DLen_3,
	CMon_MD_Qp7,0x00E9,	CMon_MDT_Qp7,0x0116,0x01,0x00,0x03,0x06,0x07,0xff,	DLen_3,
	CMon_MD_Qp8,0x00EA,	CMon_MDT_Qp8,0x0117,0x01,0x00,0x03,0x06,0x08,0xff,	DLen_3,

	//--------------新补充的ID填充-------
	CMon_MD_Qn0,0x00F4,	CMon_MDT_Qn0,0x0118,0x01,0x00,0x04,0x06,0x00,0xff,	DLen_3,
	CMon_MD_Qn1,0x00F5,	CMon_MDT_Qn1,0x0119,0x01,0x00,0x04,0x06,0x01,0xff,	DLen_3,
	CMon_MD_Qn2,0x00F6,	CMon_MDT_Qn2,0x011A,0x01,0x00,0x04,0x06,0x02,0xff,	DLen_3,
	CMon_MD_Qn3,0x00F7,	CMon_MDT_Qn3,0x011B,0x01,0x00,0x04,0x06,0x03,0xff,	DLen_3,
	CMon_MD_Qn4,0x00F8,	CMon_MDT_Qn4,0x011C,0x01,0x00,0x04,0x06,0x04,0xff,	DLen_3,
	CMon_MD_Qn5,0x00F9,	CMon_MDT_Qn5,0x011D,0x01,0x00,0x04,0x06,0x05,0xff,	DLen_3,
	CMon_MD_Qn6,0x00FA,	CMon_MDT_Qn6,0x011E,0x01,0x00,0x04,0x06,0x06,0xff,	DLen_3,
	CMon_MD_Qn7,0x00FB,	CMon_MDT_Qn7,0x011F,0x01,0x00,0x04,0x06,0x07,0xff,	DLen_3,
	CMon_MD_Qn8,0x00FC,	CMon_MDT_Qn8,0x0120,0x01,0x00,0x04,0x06,0x08,0xff,	DLen_3,	
};

