//#include "Event.h"
//伊朗定义的事件类型：

//各对应时间的事件代号..
//标准事件：
#if 0
// EVENT CODE..
#define Power_Down									    1
#define Power_Up										    2
#define Day_light_saving_time_Activated_regularly 	3
#define Clock_Adjust_old_time_date					4
#define Clock_Adjust_new_time_date					5
#define Clock_Invalid												6
#define RTC_Fail                            7
#define Replace_Battery_need                8
#define Replace_Battery											9
#define Battery_Low_Voltage									10

#define Passive_TOU_Activated								11
#define Error_Register_Cleared							12
#define Alarm_Register_Cleared							13

#define Meter_Program_Memory_Error					14
#define RAM_Error														15 
#define NV_Memory_Error											16
#define Watchdog_Error											17
#define Measurement_System_Error						18
 
#define Firmware_verification_failed				19			// 19	Firmware Check Fault..
#define Firmware_ready_for_activation				20      // 20	Firmware ready for activation..
#define Firmware_activated									21

#define Tariff_Parameter_Changed            22      
#define Passive_TOU_programmed							23 

#define Meter_Software_Error								24

#define Time_Shift_Limit_No                 25     // 广播对时超过校表限制时间做标准事件上报处理..
//安全事件：
//#define Security_Event_No							6			
//#define Security_Event_BIT9						0x0200
		
#define Terminal_Cover_removed						40			 //40	Terminal Cover opened..
#define Terminal_Cover_closed							41			 //
#define Strong_DC_field_detect						42       //42	Strong DC field detected..
#define No_strong_DC_field_anymore				43       //43	Strong DC field removed..
#define Meter_cover_removed								44       //44	Meter cover opened..
#define Meter_cover_closed								45
#define Association_authentication_failure_after_n_times	46
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

#define Fuse_supervision_L1_threshold_exceeded		70
#define Fuse_supervision_L1_threshold_OK					71
#define Fuse_supervision_L2_threshold_exceeded		72
#define Fuse_supervision_L2_threshold_OK					73
#define Fuse_supervision_L3_threshold_exceeded		74
#define Fuse_supervision_L3_threshold_OK					75
//#define Disconnector_Control_No				7		
//#define Disconnector_Control_BIT10		0x0400		

//76	Undervoltage (voltage SAG) L1
//77	Undervoltage (voltage SAG) L2
//78	Undervoltage (voltage SAG) L3
//79	Overvoltage (voltage SWELL) L1
//80	Overvoltage (voltage SWELL) L2
//81	Overvoltage (voltage SWELL) L3

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
// Standard_Event

//事件分类..
//============================================================
#define General_display_No						0
#define Alternate_display_No					1
#define Service_display_No						2

#define Ins_Readout_No								3
#define Ins_Energy_Readout_No					4

#define Standard_Event_No							5
#define Security_Event_No							6
#define Disconnector_Control_No				7																
#define M_Bus_Event_No								8																
#define Power_Quality_Event1_No				9	
//--欠压..															
#define Power_Quality_Event2_No				10																
#define Power_Quality_Event3_No				11																
#define Power_Quality_Event4_No				12
//--过压..																
#define Power_Quality_Event5_No				13																
#define Power_Quality_Event6_No				14																
#define Power_Quality_Event7_No				15
//--失压(没有电压)..
#define Power_Quality_Event8_No				16																
#define Power_Quality_Event9_No				17																
#define Power_Quality_Event10_No			18
//故障电压--即没电压又无电流..																
#define Power_Failure_Event_No				19
#define Power_Failure_EventL1_No			20		
#define Power_Failure_EventL2_No			21		
#define Power_Failure_EventL3_No			22		
																
#define LongPower_Failure_Event_No		23											
#define LongPower_Failure_EventL1_No	24				
#define LongPower_Failure_EventL2_No	25				
#define LongPower_Failure_EventL3_No	26				

#define Billing_period1_No						27
#define Billing_period2_No						28
                            				
#define Load_profile1_No							29
#define Load_profile2_No							30

#define PowerQuality_profile_No				31
#define Harmonic_profile_No						32
//--------------2016-5-19-------------
#define Average_profile_No						33
#define Max_profile_No						34
#define Min_profile_No						35
//-------end---2016-5-5-19---------------
//事件状态:  
// SM.Event_all_flag	--事件传输高位在前..															
//#define Standard_Event_BIT8						0x0001
//#define Security_Event_BIT9						0x0002																
//#define Disconnector_Control_BIT10		0x0004																
//#define M_Bus_Event_BIT11							0x0008																
//#define Power_Quality_Event1_BIT12		0x0010																
//#define Power_Quality_Event2_BIT13		0x0020																
//#define Power_Quality_Event3_BIT14		0x0040																
//#define Power_Quality_Event4_BIT15		0x0080																
//#define Power_Quality_Event5_BIT0			0x0100																
//#define Power_Quality_Event6_BIT1			0x0200																
//#define Power_Quality_Event7_BIT2			0x0400																
//#define Power_Failure_Event1_BIT3			0x0800																
//#define Power_Failure_Event2_BIT4			0x1000

#define Standard_Event_BIT						0x00000001
#define Security_Event_BIT						0x00000002															
#define Power_Quality_Event1_BIT		  0x00000004
															
#define Power_Quality_Event2_BIT		  0x00000008																
#define Power_Quality_Event3_BIT		  0x00000010																
#define Power_Quality_Event4_BIT		  0x00000020
																
#define Power_Quality_Event5_BIT			0x00000040															
#define Power_Quality_Event6_BIT			0x00000080														
#define Power_Quality_Event7_BIT			0x00000100

#define Power_Quality_Event8_BIT			0x00000200															
#define Power_Quality_Event9_BIT			0x00000400														
#define Power_Quality_Event10_BIT			0x00000800
													
#define Power_Failure_Event_BIT			  0x00001000
#define Power_Failure_EventL1_BIT		  0x00002000
#define Power_Failure_EventL2_BIT			0x00004000
#define Power_Failure_EventL3_BIT			0x00008000
														
#define LongPower_Failure_Event_BIT		0x00010000
#define LongPower_Failure_EventL1_BIT	0x00020000
#define LongPower_Failure_EventL2_BIT	0x00040000
#define LongPower_Failure_EventL3_BIT	0x00080000
																
#define Disconnector_Control_BIT		  0x00100000																
#define M_Bus_Event_BIT 							0x00200000
//========================
#if 0
#define Global_meter_reset_ScriptNo					0
#define MDI_End_of_billing_period2_ScriptNo	1
#define Tariffication_Script1No							2
#define Tariffication_Script2No							3
#define Tariffication_Script3No							4
#define Tariffication_Script4No							5
#define Disconnect_control_Script1No				6
#define Disconnect_control_Script2No				7
#define MDI_End_of_billing_period1_ScriptNo	8
#define Image_activation_ScriptNo	        9
//--------------2016-5-5------
#define GPRS_Internal_Script1No	10
#define GPRS_Internal_Script2No	11
//--------------end--2016-5-5---
#endif
//------------2016-5-23---------
#define Global_meter_reset_ScriptNo					0

#define Tariffication_Script1No 		1		
#define Tariffication_Script2No 		2		
#define Tariffication_Script3No 		3		
#define Tariffication_Script4No 		4		

#define Passive_Tariffication_Script1No 		5
#define Passive_Tariffication_Script2No 		6
#define Passive_Tariffication_Script3No 		7
#define Passive_Tariffication_Script4No 		8

#define Disconnect_control_Script1No 		9	
#define Disconnect_control_Script2No 		10	
#define Disconnect_control_Script3No 		11	
#define Disconnect_control_Script4No 		12	

#define MDI_End_of_billing_period1_ScriptNo 	13

#define Limiter_Scrip1tNo 					14	
#define Limiter_Scrip2tNo 					15	
#define Limiter_Scrip3tNo 					16	
#define Limiter_Scrip4tNo 					17	

#define M_Bus_Disconnector_script1No 		18	
#define M_Bus_Disconnector_script2No 		19	
#define M_Bus_Disconnector_script3No 		20	
#define M_Bus_Disconnector_script4No 		21	
#define M_Bus_Disconnector_script5No 		22	
#define M_Bus_Disconnector_script6No 		23	
#define M_Bus_Disconnector_script7No 		24	
#define M_Bus_Disconnector_script8No 		25	
#define M_Bus_Disconnector_script9No 		26	
#define M_Bus_Disconnector_script10No		27	
#define M_Bus_Disconnector_script11No		28	
#define M_Bus_Disconnector_script12No		29	
#define M_Bus_Disconnector_script13No		30	
#define M_Bus_Disconnector_script14No		31	
#define M_Bus_Disconnector_script15No		32
#define M_Bus_Disconnector_script16No		33	

#define Image_activation_ScriptNo 			34	

#define Push_Setup_Interval_No1				35
#define Push_Setup_Interval_No2				36
#define Push_Setup_Interval_No3				37
#define Push_On_Alarm						38
#define Push_On_Connectivity					39
#define Push_On_Installation					40

#define MDI_End_of_billing_period2_ScriptNo	41

//-------end-----2016-5-23---------

//zzl 2015-11-20 add
#define GPRS_Internal_Trigger1                  10
#define GPRS_Internal_Trigger2                  11
//zzl add end

//
//#define PhAFail_No			0			//A相失压记录号				
//#define PhBFail_No			1			//B相失压记录号				
//#define PhCFail_No			2			//C相失压记录号				
//#define PhALower_No			3			//A相欠压记录号				
//#define PhBLower_No			4			//B相欠压记录号				
//#define PhCLower_No			5			//C相欠压记录号				
//#define PhAOver_No			6			//A相过压记录号				
//#define PhBOver_No			7			//B相过压记录号				
//#define PhCOver_No			8			//C相过压记录号				
//#define PhABreak_No			9			//A相断相记录号				
//#define PhBBreak_No			10			//B相断相记录号				
//#define PhCBreak_No			11			//C相断相记录号				
//#define AllFail_No			12			//全失压记录号				
//#define AuxPWFail_No		13			//辅助电源失压记录号				
//#define VoltRevPhase_No		14			//电压逆相序记录号				
//#define CuRevPhase_No		15			//电流逆相序记录号				
//#define VoltImbalance_No	16			//电压不平衡记录号				
//#define CuImbalance_No		17			//电流不平衡记录号
//#define CuBestImbalance_No	18			//电流严重不平衡记录号				
//#define CuAFail_No			19			//A相失流记录号				
//#define CuBFail_No			20			//B相失流记录号				
//#define CuCFail_No			21			//C相失流记录号				
//#define CuAOver_No			22			//A相过流记录号				
//#define CuBOver_No			23			//B相过流记录号
//#define CuCOver_No			24			//C相过流记录号
//#define CuABreak_No			25			//A相断流记录号				
//#define CuBBreak_No			26			//B相断流记录号				
//#define CuCBreak_No			27			//C相断流记录号				
//#define RevPowerA_No		28			//A相潮流反向记录号
//#define RevPowerB_No		29			//B相潮流反向记录号
//#define RevPowerC_No		30			//C相潮流反向记录号
//#define PowerOverA_No		31			//A相过载记录号
//#define PowerOverB_No		32			//B相过载记录号
//#define PowerOverC_No		33			//C相过载记录号
//#define PftOver_No			34			//总功率因素超限记录号
//
//#define VoltCheck_No		35			//电压合格率统计记录号 
//#define VoltACheck_No		36			//A相电压合格率统计记录号 
//#define VoltBCheck_No		37			//B相电压合格率统计记录号 
//#define VoltCCheck_No		38			//C相电压合格率统计记录号 
//
//#define PdownRec_No			39			//下电记录号
//
//#define MDOverPp_No			40			//正向有功需量超限记录号
//#define MDOverPn_No			41			//反向有功需量超限记录号
//#define MDOverQ1_No			42			//1象限无功需量超限记录号
//#define MDOverQ2_No			43			//2象限无功需量超限记录号
//#define MDOverQ3_No			44			//3象限无功需量超限记录号
//#define MDOverQ4_No			45			//4象限无功需量超限记录号
//
//#define PrgRec_No			46			//编程记录号
//#define ECClr_No			47			//电量清零记录号
//#define MDClr_No			48			//需量清零记录号
//#define EventClr_No			49			//事件清零记录号
//#define TimeSet_No			50			//校时记录号
//#define SegTabPrg_No		51			//时段表编程记录号
//#define ZonePrg_No			52			//时区编程记录号
//#define WeekCDPrg_No		53			//周休日编程记录号
//#define HolidayPrg_No		54			//节假日编程记录号
//#define ECPModePrg_No		55			//有功组合方式编程记录号
//#define ECQ1ModePrg_No		56			//无功组合方式1编程记录号
//#define ECQ2ModePrg_No		57			//无功组合方式2编程记录号
//#define SaveDayPrg_No		58			//结算日编程记录号
//#define OpenCover_No		59			//开表盖记录号
//#define OpenTerminal_No		60			//开端钮盒记录号
//#define BuyEC_No			61			//购电量记录号
//#define BuyMoney_No			62			//购电金额记录号
//#define TripOFF_No			63			//跳闸记录号
//#define TripON_No			64			//合闸记录号
//
//#define AppointFreeze_No	65			//定时冻结记录号
//#define InstantFreeze_No	66			//瞬时冻结记录号 				
//#define ZoneSWFreeze_No		67			//时区表切换冻结记录号 				
//#define SegTabSWFreeze_No	68			//时段表切换冻结记录号 				
//#define FeeSWFreeze_No		69			//费率电价切换冻结记录号 				
//#define StepSWFreeze_No		70			//阶梯切换冻结记录号 				
//#define HourFreeze_No		71			//整点冻结记录号 				
//#define DayFreeze_No		72			//日冻结记录号 				
//
//#define LastEC_No			73			//历史电量记录号
//#define LastMD_No			74			//历史需量量记录号
#endif

void GetCoverTerminalState( void );
