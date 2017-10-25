#include "Measure.h"

#define OPEN_STATE	0x01	//打开
#define CLOSE_STATE	0x02	//关闭
#define PWUPTIME	64		//EC
#define MS500		1000//1000//500MS
#define TM485		6		//500MS
#define MS4000		1000		//4000MS
//#define MS3000		192		//128*16 = 2S
#define MS3000		240		//128*16 = 2S

#define RS_LEN		0x00FF
#define CALIBCOUNT	60
#define CALIBCOUNT1	61
#define BGDISDELAY 	30
#define KEYDISDELAY 20
#define TESTDISCNT	8
#define BATONDLY	30
#if 0
#define	NAK		0x15
#define	ACK		0x06
#define	SOH		0x01
#define LF		0x0A
#define	CR		0x0D
#define STX		0x02
#define ETX		0x03
#define EOT		0x04
#define StartCode 	0x2F	//"/"
#define EndCode	0x21		//"!"
//#define REQ		0x3F		//"?"


#define	READ	0x4000
#define	WRITE	0x8000		//READ WRITE与DLMS移植部分的正好呼唤，之前对应的是0x4000 0x8000
#define PARAM	0x0300		//状态、参数、时间
#define OTHER	0x0400		//RAM内数据
#endif
#define Write_A2		0x0100
#define Write_A3		0x0200
#define Write_A4		0x0400
#define Write_A5		0x0800

#define Read_A2		0x1000
#define Read_A3		0x2000
#define Read_A4		0x4000
#define Read_A5		0x8000

#define Action_A2	0x0010
#define Action_A3	0x0020
#define Action_A4	0x0040
#define Action_A5	0x0080

#define Read_All		Read_A2+Read_A3+Read_A4+Read_A5
#define Write_All		Write_A2+Write_A3+Write_A4+Write_A5
#define Read_Write_All	Read_All+Write_All
#define Read_Write_A345	Read_A3+Read_A4+Read_A5+Write_A3+Write_A4+Write_A5
#define Read_Write_A45	Read_A4+Read_A5+Write_A4+Write_A5
#define Write_A345		Write_A3+Write_A4+Write_A5
#define Read_A345		Read_A3+Read_A4+Read_A5
#define Write_A45		Write_A4+Write_A5

#define Action_All	Action_A2+Action_A3+Action_A4+Action_A5
#define Action_A345	Action_A3+Action_A4+Action_A5

#define MM1		0x0100
#define MM2		0x0200
#define MM3		0x0300
#define MM4		0x0400
#define MM5		0x0500
////////////////////////////
#define BIT0                (0x0001)
#define BIT1                (0x0002)
#define BIT2                (0x0004)
#define BIT3                (0x0008)
#define BIT4                (0x0010)
#define BIT5                (0x0020)
#define BIT6                (0x0040)
#define BIT7                (0x0080)
#define BIT8                (0x0100)
#define BIT9                (0x0200)
#define BITA                (0x0400)
#define BITB                (0x0800)
#define BITC                (0x1000)
#define BITD                (0x2000)
#define BITE                (0x4000)
#define BITF                (0x8000)

//ZPA Event Code/////////////////
#define Open_Cover 	1
#define Close_Cover	2
#define Open_terminal_Box_Cover	3
#define Close_terminal_Box_Cover	4
#define L1_Switch_Off	5
#define L1_Switch_On		6
#define L2_Switch_Off	7
#define L2_Switch_On		8
#define L3_Switch_Off	9
#define L3_Switch_On		10
#define Meter_Restart	11
#define Magnet_on_the_Meter			12
#define Magnet_Removed				13
#define Magnet_Filed_Detector_Clear	64
#define Open_Terminal_Cover_Detector_Clear	71
#define Open_Meter_Cover_Detector_Clear		20
#define Change_Tariff_Program_by_SW			15
#define Setting_RTC							66
#define Change_Pmax_Limit					17
#define Change_of_the_Accountng_Period_length	18
#define Set_Tariff_List							19
#define Energy_Profile_Integration_Peroid_Change	22
#define LCD_Energy_Registers_Decimal_Change 		24
#define Meter_Switch_Off							25
#define Low_Battery_Voltage 						26
#define Closing_of_Billing_Period_Auto 			43
#define Closing_of_Billing_Period_Manual 		45
#define Closing_of_Billing_Period_Mannuly_SW 	27
#define Summer_Winter_Switch						28

//////////////////////////
// EVENT CODE..
///#define Power_Down									    1
///#define Power_Up										    2
#define Day_light_saving_time_Activated_regularly 	Summer_Winter_Switch///3 //夏时令切换
///#define Clock_Adjust_old_time_date					4
///#define Clock_Adjust_new_time_date					5
///#define Clock_Invalid												6
///#define RTC_Fail                            7
///#define Replace_Battery_need                8
///#define Replace_Battery											9
///#define Battery_Low_Voltage									10

///#define Passive_TOU_Activated								11
///#define Error_Register_Cleared							12
///#define Alarm_Register_Cleared							13

///#define Meter_Program_Memory_Error					14
///#define RAM_Error														15 
///#define NV_Memory_Error											16
///#define Watchdog_Error											17
///#define Measurement_System_Error						18
 
#define Firmware_verification_failed				19			// 19	Firmware Check Fault..
#define Firmware_ready_for_activation				20      // 20	Firmware ready for activation..
#define Firmware_activated									21

///#define Tariff_Parameter_Changed            22      
///#define Passive_TOU_programmed							23 

///#define Meter_Software_Error								24

///#define Time_Shift_Limit_No                 25     // 广播对时超过校表限制时间做标准事件上报处理..
//安全事件：
//#define Security_Event_No							6			
//#define Security_Event_BIT9						0x0200
		
#define Terminal_Cover_removed						Open_terminal_Box_Cover 			 //40	Terminal Cover opened..
#define Terminal_Cover_closed						Close_terminal_Box_Cover			 //41
#define Strong_DC_field_detect						Magnet_on_the_Meter       //42	Strong DC field detected..
#define No_strong_DC_field_anymore				Magnet_Removed       //43	Strong DC field removed..
#define Meter_cover_removed								Open_Cover      //44	Meter cover opened..
#define Meter_cover_closed								Close_Cover		//45
///#define Association_authentication_failure_after_n_times	46
#define Configuration_changed							47
#define Global_key_changed								48
#define Decryption_or_authentication_failure		49
#define Replay_attack											50

#define Cryptographic_General_Fault       51

#define Phase_and_null_current_not_equal	52					//52	Phase and null current not equal (difference> n%)..
#define Phase_and_null_current_are_equal_again		53
#define Communication_module_removed			54
#define Communication_module_insert				55
// Security_Event

//Disconnect or Event Log：
//#define Disconnector_Control_No				7		
//#define Disconnector_Control_BIT10		0x0400		
#define Manual_disconnection							60
#define Manual_connection									61
#define Remote_disconnection							62
#define Remote_connection									63
#define Local_disconnection								64
#define Local_reconnection								65      //以下代码改..

#define Limiter_threshold_exceed					66
#define Limiter_threshold_OK							67
#define Limiter_threshold_changed					68
#define Disconnect_or_Reconnect_failure		69

///#define Fuse_supervision_L1_threshold_exceeded		70
///#define Fuse_supervision_L1_threshold_OK					71
///#define Fuse_supervision_L2_threshold_exceeded		72
///#define Fuse_supervision_L2_threshold_OK					73
///#define Fuse_supervision_L3_threshold_exceeded		74
///#define Fuse_supervision_L3_threshold_OK					75


// Power Quality Event Log2
#define Undervoltage_L1								76
// Power Quality Event Log3
#define Undervoltage_L2								77
// Power Quality Event Log4
#define Undervoltage_L3								78
// Power Quality Event Log5
#define Overvoltage_L1								79
// Power Quality Event Log6
#define Overvoltage_L2								80
// Power Quality Event Log7
#define Overvoltage_L3								81

//Power Quality Event Log1
//#define Power_Quality_Event1_No				9		

#define Missing_voltage_L1						82
#define Missing_voltage_L2						83
#define Missing_voltage_L3						84

#define Normal_Voltage_L1							85
#define Normal_Voltage_L2							86
#define Normal_Voltage_L3							87

#define Phase_sequence_reversal				88 
#define Missing_Neutral								89 

#define Short_power_failure_L1       	90
#define Short_power_failure_L2    	 	91
#define Short_power_failure_L3    	 	92

#define Inverse_Current              	93			//仅针对单相表..
#define Phase_and_Neutral_Swaping    	94      //仅针对单相表..

#define Long_power_failure_L1       	95
#define Long_power_failure_L2    	 	  96
#define Long_power_failure_L3    	 	  97

#define Phase_sequence_become_normal	98   //临时增加相序恢复正常..
/*
////#define M_Bus_Event_No								8		
#define M_bus_Channel_1_Communication_Error		100
#define M_bus_Channel_1_Communication_OK			101
#define M_bus_Channel_1_PermanentlyUnavailable  102
#define M_bus_Channel_1_Fraud_attempt         103
#define M_bus_Channel_1_Clock_adjusted        104
//#define M_bus_Channel_1_Old_reading_value			105

#define M_bus_Channel_2_Communication_Error		110
#define M_bus_Channel_2_Communication_OK			111
#define M_bus_Channel_2_PermanentlyUnavailable  112
#define M_bus_Channel_2_Fraud_attempt         113
#define M_bus_Channel_2_Clock_adjusted        114
//#define M_bus_Channel_2_Old_reading_value			115

#define M_bus_Channel_3_Communication_Error		120
#define M_bus_Channel_3_Communication_OK			121
#define M_bus_Channel_3_PermanentlyUnavailable  122
#define M_bus_Channel_3_Fraud_attempt         123
#define M_bus_Channel_3_Clock_adjusted        124
//#define M_bus_Channel_3_Old_reading_value			125

#define M_bus_Channel_4_Communication_Error		130
#define M_bus_Channel_4_Communication_OK			131
#define M_bus_Channel_4_PermanentlyUnavailable  132
#define M_bus_Channel_4_Fraud_attempt         133
#define M_bus_Channel_4_Clock_adjusted        134
//#define M_bus_Channel_4_Old_reading_value			135

#define Mbus_Channel_1_Manual_disconnection  160
#define Mbus_Channel_1_Manual_Connection     161
#define Mbus_Channel_1_Remote_disconnection  162
#define Mbus_Channel_1_Remote_Connection     163
#define Mbus_Channel_1_Valve_Alarm           164

#define Mbus_Channel_2_Manual_disconnection  170
#define Mbus_Channel_2_Manual_Connection     171
#define Mbus_Channel_2_Remote_disconnection  172
#define Mbus_Channel_2_Remote_Connection     173
#define Mbus_Channel_2_Valve_Alarm           174

#define Mbus_Channel_3_IHD_Alarm           	 184	

#define PLC_CommunicationTemporarilyUnavailable  200
#define PLC_CommunicationPermanentlyUnavailable  201
*/
/*
		#define Demand_parameters_changed						200
		#define Communication_parameters_changed		201
		#define Day_light_Saving_parameters_changed	202 
		#define Billing_date_changed								203
		#define Load_Profile_parameters_changed			204
		#define Transformer_ratio_changed						205
		#define Display_List_changed								206
		#define Time_Threshold_for_SAG_or_SWELL_changed		    207 
		#define Time_Threshold_for_long_power_failure_changed	208
		#define SWELL_voltage_level_changed					  209
		#define SAG_voltage_level_changed						  210
		#define Disconnection_parameters_changed			211
		#define Limiter_parameters_changed					  212
		#define Fuse_threshold_changed						    213
		#define Password_One_changed						      214
		#define Password_Two_changed						      215
		#define Password_Three_changed						    216
		#define Password_four_changed						      217
		#define Phase_sequence_become_normal				  218
		#define Passive_TOU_Activation_failed				  219
*/
#define Electricity_load_profiled_cleared			254       //254	Load profile cleared..
#define Any_of_event_logs_were_cleared	      255				

//事件分类..
//============================================================
#define General_display_No					0
#define Alternate_display_No					1
#define Service_display_No					2

#define Ins_Readout_No						3
#define Ins_Energy_Readout_No				4

#define Standard_Event_No					5
#define Security_Event_No					6
#define Disconnector_Control_No				7																
///#define M_Bus_Event_No								8																
#define Power_Quality_Event1_No				8	
//--欠压..															
#define Power_Quality_Event2_No				9																
//--过压..																
#define Power_Quality_Event3_No				10	
#define Power_Quality_Event4_No				11
/*
//故障电压--即没电压又无电流..																
#define Power_Failure_EventL1_No				12		
#define Power_Failure_EventL2_No				13		
#define Power_Failure_EventL3_No				14		
#define Power_Failure_EventL4_No				15		
*/																
#define Billing_period1_No					12//16
#define Billing_period2_No					13//17
                            				
#define Load_profile1_No						14//18
#define Load_profile2_No						15//19

#define PowerQuality_profile_No				16//20

#define Standard_Event_BIT			  0x00000001
#define Security_Event_BIT			  0x00000002															
#define Power_Quality_Event1_BIT		  0x00000004
															
#define Power_Quality_Event2_BIT		  0x00000008																
#define Power_Quality_Event3_BIT		  0x00000010																
#define Power_Quality_Event4_BIT		  0x00000020
																
#define Power_Quality_Event5_BIT			0x00000040															
#define Power_Quality_Event6_BIT			0x00000080														
#define Power_Quality_Event7_BIT			0x00000100

#define Power_Quality_Event8_BIT			0x00000200															
#define Power_Quality_Event9_BIT			0x00000400														
#define Power_Quality_Event10_BIT		0x00000800
													
#define Power_Failure_Event_BIT			0x00001000
#define Power_Failure_EventL1_BIT		0x00002000
#define Power_Failure_EventL2_BIT		0x00004000
#define Power_Failure_EventL3_BIT		0x00008000
														
#define LongPower_Failure_Event_BIT		0x00010000
#define LongPower_Failure_EventL1_BIT	0x00020000
#define LongPower_Failure_EventL2_BIT	0x00040000
#define LongPower_Failure_EventL3_BIT	0x00080000
																
#define Disconnector_Control_BIT		  	0x00100000																
#define M_Bus_Event_BIT 					0x00200000

#if(MeterPhase == Threephase)
#define ECUnitNum	11		//电量计量项数
#define MDUnitNum	5		//需量计量项数			//反向需量不计
#elif (MeterPhase == SinglePhase) //单相
#define ECUnitNum	5		//电量计量项数
#define MDUnitNum	5		//需量计量项数			//反向需量不计
#endif


extern  PARA			Para;				// 电表参数
extern  COMM			Comm;

extern 	CLK		Clk;

extern 	FLAG		Flag;
extern 	Demand_Reg  Demand_Class5;
extern 	SRAM SM;
extern  const unsigned short MP_Fcstab[256];
extern const unsigned long crc_32_tab[256];
extern PUSHSTATE PushSetup;					//推送事件40参数..
//extern MDBUFF		MDBufPp;
//extern MDBUFF		MDBufPn;
//extern MDBUFF		MDBufP15;
extern QUALITY_EVENT  quality_event;		//电能事件..
extern  ECPULSE		ECP;
extern  INTPULSE	ECInt;
extern Double_Commu Double_ComPtr;

extern const unsigned char firmware_version0[16];

//extern		int32_t __initial_sp;
//#endif	//_DL645_GLOBALS_H

/*
DL645_EXT  PARA	Para;				// 电表参数
DL645_EXT  COMM	Comm;
DL645_EXT  COMM8207	Comm8207;

DL645_EXT 	CLK		Clk;
DL645_EXT 	ADJ		Adj;
DL645_EXT 	DISKEY		Disk;
DL645_EXT 	FLAG			Flag;
DL645_EXT 	SRAM SM;
DL645_EXT 	MEASURE7053	Measure7053;
DL645_EXT 	MEASURE8207	Measure8207;
//DL645_EXT		int32_t __initial_sp;
*/

//----- 

#define clrbit0 0xfe
#define	clrbit1 0xfd
#define	clrbit2 0xfb
#define	clrbit3 0xf7
#define	clrbit4 0xef
#define	clrbit5 0xdf
#define	clrbit6 0xbf
#define	clrbit7 0x7f

#define bitsetkey  (Flag.Key &0x1)
#define Set_bitsetkey {Flag.Key |=0x1;}
#define Clr_bitsetkey {Flag.Key &=clrbit0;}

#define bitlgset (Flag.Key &0x2)
#define Set_bitlgset {Flag.Key |=0x2;}
#define Clr_bitlgset {Flag.Key &=clrbit1;}

#define bitscroll (Flag.Key &0x4)
#define Set_bitscroll {Flag.Key |=0x4;}
#define Clr_bitscroll {Flag.Key &=clrbit2;}

#define bitUpRollKey (Flag.Key &0x8)
#define Set_bitUpRollKey {Flag.Key |=0x8;}
#define Clr_bitUpRollKey {Flag.Key &=clrbit3;}

#define bitDownRollKey (Flag.Key &0x10)
#define Set_bitDownRollKey {Flag.Key |=0x10;}
#define Clr_bitDownRollKey {Flag.Key &=clrbit4;}

#define bitstset (Flag.Key &0x20)
#define Set_bitstset {Flag.Key |=0x20;}
#define Clr_bitstset {Flag.Key &=clrbit5;}

#define bitsfsttolg (Flag.Key &0x40)
#define Set_bitsfsttolg {Flag.Key |=0x40;}
#define Clr_bitsfsttolg {Flag.Key &=clrbit6;}

#define bitxfsttolg (Flag.Key &0x80)
#define Set_bitxfsttolg {Flag.Key |=0x80;}
#define Clr_bitxfsttolg {Flag.Key &=clrbit7;}
//--------------------
#define bitlgsfkey  (Flag.Key1 &0x1)
#define Set_bitlgsfkey {Flag.Key1 |=0x1;}
#define Clr_bitlgsfkey {Flag.Key1 &=clrbit0;}

#define bitlgxfkey  (Flag.Key1 &0x2)
#define Set_bitlgxfkey {Flag.Key1 |=0x2;}
#define Clr_bitlgxfkey {Flag.Key1 &=clrbit1;}

#define bitwinter	(Flag.Key1 &0x4)
#define Set_bitwinter	{Flag.Key1 |=0x4;}
#define Clr_bitwinter	{Flag.Key1 &=~0x4;}

//---------------------
#define bitsetsdb (Flag.Event &0x01)
#define Set_bitsetsdb {Flag.Event |=0x01;}
#define Clr_bitsetsdb {Flag.Event &=clrbit0;}

#define bitsetsdbSW (Flag.Event &0x04)
#define Set_bitsetsdbSW {Flag.Event |=0x04;}
#define Clr_bitsetsdbSW {Flag.Event &=clrbit2;}

#define bitSetJsr (Flag.Event &0x02)
#define Set_bitSetJsr {Flag.Event |=0x02;}
#define Clr_bitSetJsr {Flag.Event &=clrbit1;}

#define bitSetJsrSW (Flag.Event &0x08)
#define Set_bitSetJsrSW {Flag.Event |=0x08;}
#define Clr_bitSetJsrSW {Flag.Event &=clrbit3;}

//extern 	#define bitsetkey  (Flag.Key &0x1)
//extern 	#define Set_bitsetkey {Flag.Key |=0x1;}
//extern 	#define Clr_bitsetkey {Flag.Key &=clrbit0;}
//
//extern 	#define bitlgset (Flag.Key &0x2)
//extern 	#define Set_bitlgset {Flag.Key |=0x2;}
//extern 	#define Clr_bitlgset {Flag.Key &=clrbit1;}
//
//extern 	#define bitscroll (Flag.Key &0x4)
//extern 	#define Set_bitscroll {Flag.Key |=0x4;}
//extern 	#define Clr_bitscroll {Flag.Key &=clrbit2;}
//
//extern 	#define bitUpRollKey (Flag.Key &0x8)
//extern 	#define Set_bitUpRollKey {Flag.Key |=0x8;}
//extern 	#define Clr_bitUpRollKey {Flag.Key &=clrbit3;}
//
//extern 	#define bitDownRollKey (Flag.Key &0x10)
//extern 	#define Set_bitDownRollKey {Flag.Key |=0x10;}
//extern 	#define Clr_bitDownRollKey {Flag.Key &=clrbit4;}
//
//extern 	#define bitstset (Flag.Key &0x20)
//extern 	#define Set_bitstset {Flag.Key |=0x20;}
//extern 	#define Clr_bitstset {Flag.Key &=clrbit5;}
//
//extern 	#define bitsfsttolg (Flag.Key &0x40)
//extern 	#define Set_bitsfsttolg {Flag.Key |=0x40;}
//extern 	#define Clr_bitsfsttolg {Flag.Key &=clrbit6;}
//
//extern 	#define bitxfsttolg (Flag.Key &0x80)
//extern 	#define Set_bitxfsttolg {Flag.Key |=0x80;}
//extern 	#define Clr_bitxfsttolg {Flag.Key &=clrbit7;}
//
//
//extern 	#define bitsetsdb (Flag.Event |=0x01)
//extern 	#define Set_bitsetsdb {Flag.Event |=0x01;}
//extern 	#define Clr_bitsetsdb {Flag.Event &=clrbit0;}
//
//extern 	#define bitSetJsr (Flag.Event |=0x02)
//extern 	#define Set_bitSetJsr {Flag.Event |=0x02;}
//extern 	#define Clr_bitSetJsr {Flag.Event &=clrbit1;}
//---------2016-6-12-------
#define Global_meter_reset_ScriptNo					0

#define Tariffication_Script1No 		1		
#define Tariffication_Script2No 		2		
#define Tariffication_Script3No 		3		
#define Tariffication_Script4No 		4		

#define Activate_Test_Script1No 		5
#define Activate_Test_Script2No 		6
#define Activate_Test_Script3No 		7
#define Activate_Test_Script4No 		8

#define Activate_Normal_Script1No 		9	
#define Activate_Normal_Script2No 		10	
#define Activate_Normal_Script3No 		11	
#define Activate_Normal_Script4No 		12	
#define MDI_End_of_billing_period1_ScriptNo	13
#define MDI_End_of_billing_period2_ScriptNo	14
///------------class 9 宏定义---------
#define Global_meter_reset_Script		0
#define Image_activation_Script	        1
#define Tariffication_Script				2
#define Disconnect_control_Script		3
#define Push_script						4
#define Passive_Tariffication_Script		5
#define MBus_Disconnector_script			6
#define Limiter_Script 					7
#define MDI_End_of_billing_period_Script	8

#define Push_Interval_1				1
#define Push_Interval_2				2
#define Push_Interval_3				3
#define Push_Alarm					4
#define Push_On_Connectivity			5
#define Push_On_Installation			6
#define Push_On_Power_Down			7
#define Push_Consumer_Installation	8

#define MDI_End_of_billing_period1 	1
#define MDI_End_of_billing_period2	2

///--------------------------------

//权限设置..
#define RPublic_clientSum		48
#define RReading_clientSum	173

#define RPR_clientSum				2
#define WPR_clientSum				71
#define APR_clientSum				7

#define DLMSDataSum	44+9
#if(MeterPhase == Threephase)
#define DLMS_MDSum		57
#define DLMS_CUMDSum	45
#define DLMS_DemandRegSum 45
#define DLMS_ECSum	55

#else
#define DLMS_MDSum		37
#define DLMS_CUMDSum	25
#define DLMS_DemandRegSum 25
#define DLMS_ECSum	25

#endif
#define DLMS_RealSum 7
#define DLMS_RAM_DataSum 15
#define DLMS_Sigle_actSum 5
#define DLMS_ScriptParaSum 9
#define DLMS_ScriptSum 13
#define CosemParaSum	29

#define Init_General_display_Sum				18			//52+3	
#define Init_Alternate_display_Sum			22	
#define Init_Service_display_Sum				7	
#define Init_Ins_Readout_Sum					15		
#define Init_Ins_Energy_Readout_Sum			20
#define Init_Standard_Event_Sum				2	
#define Init_Security_Event_Sum				2	
#define Init_Disconnector_Control_Sum		3	
#define Init_Billing_period1_Sum				20	
#define Init_Billing_period2_Sum				11	
#define Init_Load_profile1_Sum				7	
#define Init_Load_profile2_Sum				19
#define DLMS_RegSum				10
#define DLMS_Reg_monitorSum		3
#define DLMS_ProfileSum			17
#define DLMS_PortdataSum			6
#define DLMS_Reg_ActivationSum	2

#define DisListCUMMDReg NoCheck		//无累计需量抄读LIST.. 20140212..
#define IranRequire NoCheck			//塞浦路斯不带拉合闸
#define IDTHarmonic	NoCheck		//IDT芯片为90E32AS
#define VERSIONLISTDIS  NoCheck 		// 炬华软件版本号定义不在抄LIST中显示..

#if ( IDTHarmonic == YesCheck )
//#define DLMS_HRSum	84
#define DLMS_HRSum	120
#endif
//DLMSPushSum--40类
#define DLMSPushSum    6
#define DLMSGPRS2829Sum    2
#define DLMSPushADD2Sum    (DLMSPushSum + DLMSGPRS2829Sum)   //增加28类和29类代码..
#define Limiter_monitored_list_Sum  3
///#define DLMS_Reg_ActivationSum	2

#define Init_Power_Quality_Event1_Sum		2	
#define Init_Power_Quality_Event2_Sum		4	
#define Init_Power_Quality_Event3_Sum		4	
#define Init_Power_Quality_Event4_Sum		4	
#define Init_Power_Failure_Event1_Sum		3
#define Init_Power_Failure_Event2_Sum	  	3
#define Init_Power_Failure_Event3_Sum	  	3		
#define Init_Power_Failure_Event4_Sum	  	4	
#define Init_PowerQuality_profile_Sum		3	

////test 2016-8-25-fire
#define Public_AssociationSum 19///22
#define RReading_AssociationSum 34
#define ALT_Display_Num     21 
#define Normal_Display_Num  49+2 


//---------------end--------------
extern 	const unsigned char TimeMaxTab[3];
extern 	const unsigned char DateMaxTab[4];

extern 	const char 			MonTab[13];
extern 	const unsigned char BCDMonTab[13];
extern 	const unsigned char WeekTab[12];
extern  const E2Para E2PTab[4]; 

extern  const ECRg 			ECRgTab[ECUnitNum];
//extern  const unsigned char BautTab[21];
//extern 	const unsigned char NumSeg[10];
//extern 	const unsigned char NumSeg_Obis[16];

//extern  const TAMPER TamperTab[TamperNum];
//extern  const PTPARA PointTab[7];
extern  const unsigned char ItemLenTab[32];
extern 	const unsigned short	ByteBit[16];
extern 	const unsigned char 	CByteBit[8];

//extern 	const DISTAB 	DisTab[DisNum];
//extern	const DISMENUTAB KeyMenuTab[16];
/*
extern	const unsigned short TariffTab[10];
extern	const COMPLEXSD Complexsd[3];
extern	const SIMPLESD Simplesd[7];
extern 	const COMTAB 	ComTab[ComNum];
*/

//#if ( CPUCHIP == MSPF449 )
//extern  const COMMPARA CommPara[3];
//#else
//extern  const COMMPARA CommPara[2];
//#endif
extern  const MDPARA MDPara[MDUnitNum];

extern  const COMMPARA CommPara[4];
///extern  const BLOCKCOMMPARA BlockCommPara[2];

extern	const MEASREG MeasReg[5];

//extern  const unsigned char LCDHWBrowseTab[26];
//extern  const unsigned char LCDM2BrowseTab[7];
//extern  const unsigned char LCDM4BrowseTab[7];
//extern  const unsigned long LCDSWTab[21];
//extern  const unsigned long YMPTab[6];

///extern 	const EVENTTAB EventTab[EventTabNum];//131228事件记录

///extern 	const COMDISTAB 	ComDisTab[3];			//V2.0
///extern  const LCDROLL		 LcdRoll[9];
extern	const LASTSUNDAY LastSunTab[36];
//#if ( ZjProtocol == YesCheck)
//extern const ZJCOMTAB ZjComTab[ZjComNum];
//#if ( CPUCHIP == MSPF449 )
//extern  const ZJCOMMPARA ZjCommPara[3];
//#else
//extern  const ZJCOMMPARA ZjCommPara[2];
//#endif
//#else
//#endif

extern  const unsigned char LCDRollOrderTab[21];
extern  const short DayTab[13];
//extern  const EXECUTETAB ExecuteTab[10];
extern  const unsigned short Crc16tab[256];
extern REAL            Real_Data[8];
extern ENERGY          Energy_Data[8];


