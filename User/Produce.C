#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Port.h"
#include "TypeRAM.h"
//#include "RsComm.h"
#include "Data.h"
///#include "interrupt.h"
#include "Mem.h"
#include "Measure.h"
#include "General.h" 
#include "Time.h"
#include "Power.h"
#include "Display.h"
#include "Initial.h"
#include "ECProcess.h"
#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "ActCalendar.h"
#include "RegisterMonitor.h"
#include "SingleAction.h"
#include "ProfileGeneric.h"
#include "Demand.h"
#include "Produce.h"
#include "RegisterActivation.h"
#include "ScriptTable.h"
///#include "quality_event.h"
#include "ht6xxx_lib.h"


void InitialDemandPara( void )
{
	unsigned long Value;
	unsigned short Temp;
	unsigned short i;
	
	Value = 60;
	Temp = 15;	
	for( i=0;i<3;i++ )
	{
		E2P_WData( Eperiod_Pp+i*5, (unsigned char*)&Value, 4 );	
		E2P_WData( Enumber_of_periods_Pp+i*3, (unsigned char*)&Temp, 2 );	
	}	
}

void InitialProfile_genericLog( void )
{
	unsigned short i;
	unsigned char Buff[4];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	RAM_Fill( Ptr, 4 );
	for( i=Standard_Event_No;i<=PowerQuality_profile_No;i++ )
	{
		E2P_WData( DLMS_ProfileTab[i].EntriesUseAddr, Ptr, 4 );
	}	
}


void InitialEventLog( void )
{
	unsigned long Value;
	unsigned short i;
	
	Value = 0;
	E2P_WData( No_of_config_program, (unsigned char*)&Value, 2 );	

	SM.StandardEventCode = 0xff;				
	SM.FraudDetectionCode = 0xff;				
	SM.DisconnectorControlCode = 0xff;						
	SM.M_BusCode = 0xff;										
	SM.PowerQuality1Code = 0xff;				
	SM.PowerQuality2Code = 0xff;				
	SM.PowerQuality3Code = 0xff;				
	SM.PowerQuality4Code = 0xff;				
	SM.PowerQuality5Code = 0xff;				
	SM.PowerQuality6Code = 0xff;				
	SM.PowerQuality7Code = 0xff;				
///	SM.PowerQuality8Code = 0xff;				
///	SM.PowerQuality9Code = 0xff;				
///	SM.PowerQuality10Code = 0xff;	
	
///	SM.PowerQuality11Code = 0xff;				
///	SM.PowerQuality12Code = 0xff;	
	//记录开始临时时间位置..					
	E2P_WData( Power_Failure_Start_temp, &Clk.YearH, 12 );
	//fire 2016-6-27
	//清事件记录..
	quality_event.flgEvent1.word = 0;
  	quality_event.flgEvent1BK.word = 0;
  	    
	  quality_event.PowerFailureEvent.byte = 0; 
	  quality_event.PowerFailurelongEvent.byte =0;
	  
  	quality_event.duanxianglongCNT_a = 0;       
  ///	quality_event.duanxianglongCNT_b = 0;     
 /// 	quality_event.duanxianglongCNT_c = 0;  
  	quality_event.Ram_Duration_of_long_power_failure_in_any_phase = 0;
  	quality_event.Delay_TimeFlag[0] = 0;  //超过长时间开始标志..
  	quality_event.Delay_TimeFlag[1] = 0;  
  	quality_event.Delay_TimeFlag[2] = 0;  
				
  for( i=Standard_Event_No;i<=Power_Quality_Event4_No;i++ )
   {
    	LoadRecord( i );	
   }	                                
                                                                             
	SM.Error_register = 0; 
	SM.Alarm_register = 0; 
	SM.Event_all_flag = 0; 		//南瑞事件状态..
	SM.Error_all_flag = 0;
	
//	Clr_E2PData( Number_of_power_failures_in_phase_L1, 3, 8 );				
	
}	

void InitialProfile_genericClassPara( void )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
//	unsigned short ItemNo; 
//	unsigned short ItemLen;
    unsigned long Value;
	short i=0,j=0;
	unsigned char Temp;
	unsigned char RecdLen;
	
//	unsigned short PtAddr;			//当前记录首地址指针(该记录所占用的FLASH空间内的相对地址)
//	unsigned char Logical_Name[6];
//	unsigned short StartAddr;			//记录起始地址:FLASH起始页号												
//	unsigned short EndAddr;			//记录结束地址(占用FLASH页数)												
	unsigned short CapNumAddr;		//曲线捕获项数地址
//	unsigned short CapNumMax;			//曲线最大捕获项数(物理空间允许接收的项数)
//	unsigned short RecdLenAddr;		//每条记录长度地址(根据捕获内容得到)
	unsigned short CapObjAddr;		//捕获项地址
//	unsigned short PeriodAddr;		//捕获周期地址
//	unsigned short SortMethodAddr;	//排序方法地址
//	unsigned short SortObjectAddr;	//排序对象地址
//	unsigned short EntriesUseAddr;	//有效条目数地址，该数最大为允许读取的总记录条数，超过后不再增加。记录清零时只需将其清零即可
//	unsigned short EntriesAddr;		//逻辑最大条目数地址(读取时最大条目数)
//	unsigned short ECurRecNoAddr;		//当前记录号地址
	unsigned short InitCapNumber;		//曲线初始化捕获项数
	INI_PROFILE_TAB* InitCapObjTabPtr;	//曲线初始化捕获项表格
	
	Ptr = Buff;
	HT_FreeDog();
	for( i=0;i<DLMS_ProfileSum;i++ )
	{
		RAM_Fill( Ptr, 16 );
		InitCapNumber = DLMS_ProfileTab[i].InitCapNumber;
		*Ptr = InitCapNumber;
		CapNumAddr = DLMS_ProfileTab[i].CapNumAddr;
		//E2P_WData( CapNumAddr, Ptr, 2 );
		E2P_WData( CapNumAddr, Ptr, 1 );
		
		InitCapObjTabPtr = (INI_PROFILE_TAB*)DLMS_ProfileTab[i].InitCapObjTab;
		CapObjAddr = DLMS_ProfileTab[i].CapObjAddr;
		RecdLen = 0;
		for( j=0;j<InitCapNumber;j++ )
		{
			RecdLen += InitCapObjTabPtr->DataLen;
			Temp = InitCapObjTabPtr->ClassId;
//			RAM_Write( Ptr, (unsigned char*)&Temp, 2 );
			FillIntegerToBuff( Ptr, Temp );
			RAM_Write( Ptr+2, InitCapObjTabPtr->Logical_Name, 6 );
			*(Ptr+8) = InitCapObjTabPtr->Attribute;
			E2P_WData( CapObjAddr+j*12, Ptr, 11 );
			InitCapObjTabPtr++;
		}	
		E2P_WData( DLMS_ProfileTab[i].RecdLenAddr, (unsigned char*)&RecdLen, 2 );			//每条记录长度
		
		if(( i >= General_display_No )&&( i <= Ins_Energy_Readout_No )) Value = 1;			//显示项及READOUT项每秒刷新
		else if(( i >= Standard_Event_No )&&( i <= Power_Quality_Event4_No )) Value = 0;	//事件随机触发
		else if(( i == Billing_period1_No )||(  i == Billing_period2_No)) Value = 0;		//结算单操作数类触发
		else if( i == Load_profile1_No ) Value = 900;		//15分钟负荷曲线
		else if( i == Load_profile2_No ) Value = 86400;		//小时冻结
		E2P_WData( DLMS_ProfileTab[i].PeriodAddr, (unsigned char*)&Value, 4 );				//捕获周期		
		
		*Ptr = 1;
		E2P_WData( DLMS_ProfileTab[i].SortMethodAddr, Ptr, 1 ); 		//排序方法1：FIFO
		RAM_Fill( Ptr, 16 );    
		E2P_WData( DLMS_ProfileTab[i].SortObjectAddr, Ptr, 11 ); 		//无排序对象
		  
		InitialProfilePara( i, RecdLen );
		
		if(( i>= General_display_No )&&( i<= Ins_Energy_Readout_No ))
		{
			RAM_Fill( Ptr, 4 );
			*Ptr = 1;	
			E2P_WData( DLMS_ProfileTab[i].EntriesUseAddr, Ptr, 4 );												
			E2P_WData( DLMS_ProfileTab[i].EntriesAddr, Ptr, 4 );	
		}	

		if( i >= Standard_Event_No )
		{
			RAM_Fill( Ptr, 4 ); 	
			E2P_WData( DLMS_ProfileTab[i].ECurRecNoAddr, Ptr, 4 );	
			E2P_WData( DLMS_ProfileTab[i].PtAddr, Ptr, 4 );		
			E2P_WData( DLMS_ProfileTab[i].EntriesUseAddr, Ptr, 4 );								
		}
	
	///--------------2016-10-20------------------------
		///if( i == Billing_period1_No )
		if( i == Billing_period1_No )
		{
			E2P_WData( Time_of_recent_billing_period, (unsigned char*)&Clk.YearH, 12 );
			Value = 0;
			E2P_WData( Billing_period_counter, (unsigned char*)&Value, 4 );			
		}	
	///------------------------------------------------------
		
	}	
}	

void InitialSingle_action_scheduleClassPara( void )
{
	unsigned char Buff[6];
	unsigned char Buff1[16];
	unsigned char* Ptr;
	
	Ptr = Buff1;
	
	RAM_Fill( Ptr, 16 );
	
	Buff[0] = 0x00;				//End of billing period1(12个月)
	Buff[1] = 0x00;
	Buff[2] = 0x0A;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	
	E2P_WData( Single1_script_logical_name, Buff, 6 );
	*Ptr = 0;
	*(Ptr+1) = 1;
	E2P_WData( Single1_script_selector, Ptr, 2 );
	*Ptr = 3;
	E2P_WData( Single1_type, Ptr, 1 );
	*Ptr = 1;
	E2P_WData( Single1_execution_time_Number, Ptr, 1 );
	RAM_Fill( Ptr, 16 );		//hour、min、sec			//每月1日0时0分
	*(Ptr+4) = 0xff;			//yearH
	*(Ptr+5) = 0xff;			//yearL
	*(Ptr+6) = 0xff;			//Month
	*(Ptr+7) = 0x01;			//Day	
	*(Ptr+8) = 0xff;			//Week
	E2P_WData( Single1_execution_time1, Ptr, 9 );
	
	Buff[0] = 0x00;				//End of billing period2(31天)
	Buff[1] = 0x00;
	Buff[2] = 0x0A;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	E2P_WData( Single2_script_logical_name, Buff, 6 );
	*Ptr = 0;
	*(Ptr+1) = 2;
	E2P_WData( Single2_script_selector, Ptr, 2 );
	*Ptr = 3;
	E2P_WData( Single2_type, Ptr, 1 );
	*Ptr = 1;
	E2P_WData( Single2_execution_time_Number, Ptr, 1 );
	RAM_Fill( Ptr, 16 );	//hour、min、sec			//每天0时0分
	*(Ptr+4) = 0xff;			//yearH
	*(Ptr+5) = 0xff;			//yearL
	*(Ptr+6) = 0xff;			//Month
	*(Ptr+7) = 0xff;			//Day	
	*(Ptr+8) = 0xff;			//Week
	E2P_WData( Single2_execution_time1, Ptr, 9 );

	Buff[0] = 0x00;				//Disconnect control scheduler
	Buff[1] = 0x00;
	Buff[2] = 0x0A;
	Buff[3] = 0x00;
	Buff[4] = 106;
	Buff[5] = 0xff;
	E2P_WData( Single3_script_logical_name, Buff, 6 );
	*Ptr = 0;
	*(Ptr+1) = 0;
	E2P_WData( Single3_script_selector, Ptr, 2 );
	*Ptr = 1;
	E2P_WData( Single3_type, Ptr, 1 );
	*Ptr = 1;
	E2P_WData( Single3_execution_time_Number, Ptr, 1 );
	RAM_Fill( Ptr, 16 );		//hour、min、sec			//每天0时0分	
	*(Ptr+4) = 0xff;			//yearH
	*(Ptr+5) = 0xff;			//yearL
	*(Ptr+6) = 0xff;			//Month
	*(Ptr+7) = 0xff;			//Day	
	*(Ptr+8) = 0xff;			//Week
	E2P_WData( Single3_execution_time1, Ptr, 9 );

	Buff[0] = 0x00;				//Image transfer 执行脚本0.0.10.0.107.255 执行:(0-0:44.0.0.255 Class ID: 18)..
	Buff[1] = 0x00;
	Buff[2] = 0x0A;
	Buff[3] = 0;
	Buff[4] = 107;
	Buff[5] = 0xff;
	E2P_WData( Single4_script_logical_name, Buff, 6 );
	*Ptr = 0;
	*(Ptr+1) = 0;
	E2P_WData( Single4_script_selector, Ptr, 2 );
	*Ptr = 1;
	E2P_WData( Single4_type, Ptr, 1 );
	*Ptr = 1;
	E2P_WData( Single4_execution_time_Number, Ptr, 1 );
	RAM_Fill( Ptr, 16 );		//hour、min、sec			//每天0时0分	
	*(Ptr+4) = 0xff;			//yearH
	*(Ptr+5) = 0xff;			//yearL
	*(Ptr+6) = 0xff;			//Month
	*(Ptr+7) = 0xff;			//Day	
	*(Ptr+8) = 0xff;			//Week
	E2P_WData( Single4_execution_time1, Ptr, 9 );	

}	


/*
void InitialProfClassPara_display( void )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	short i;
	
	Ptr = Buff;

	*Ptr = 22;
	E2P_WData( General_display_number, Ptr, 2 );
	RAM_Fill( Ptr, 11);
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x00;
	*(Ptr+5) = 0x09;
	*(Ptr+6) = 0x02;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	E2P_WData( General_display_capture_objects1, Ptr, 11 );
	
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x00;
	*(Ptr+5) = 0x09;
	*(Ptr+6) = 0x01;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	E2P_WData( General_display_capture_objects2, Ptr, 11 );

	RAM_Fill( Ptr, 11);
	*(Ptr+1) = Class_id_Register;
	for( i=0;i<20;i++ )
	{
		RAM_Write( Ptr+2, (unsigned char*)DLMS_ECTab[i].Logical_Name, 6 ); 
		*(Ptr+8) = 0x02;
		E2P_WData( General_display_capture_objects3+i*12, Ptr, 11 );
	}	

/////////////////////////////////////////////////////////////////////////////

	*Ptr = 40;
	E2P_WData( Alternate_display_number, Ptr, 2 );
	RAM_Fill( Ptr, 11);
	*(Ptr+1) = Class_id_Extended_register;
	for( i=0;i<20;i++ )
	{
		RAM_Write( Ptr+2, (unsigned char*)DLMS_MDTab[i].Logical_Name, 6 ); 
		*(Ptr+8) = 0x02;
		E2P_WData( Alternate_display_capture_objects1+i*2*12, Ptr, 11 );
		*(Ptr+8) = 0x05;
		E2P_WData( Alternate_display_capture_objects1+(i*2+1)*12, Ptr, 11 );
	}	
	
/////////////////////////////////////////////////////////////////////////////
	
	*Ptr = 42;
	E2P_WData( Service_display_number, Ptr, 2 );
	RAM_Fill( Ptr, 11);
	*(Ptr+1) = Class_id_Register;
	for( i=0;i<DLMS_RealSum;i++ )
	{
		RAM_Write( Ptr+2, (unsigned char*)DLMS_REALTab[i].Logical_Name, 6 ); 
		*(Ptr+8) = 0x02;
		E2P_WData( Service_display_capture_objects1+i*12, Ptr, 11 );
	}	
	
}	
*/
/*
void MeterClrCycle( void )
{
	switch( SM.MeterClrState )	
	{
		case MeterClrIdle:			// 电表清零空闲
			SM.MeterClrStateChk = MeterClrIdle;			
			break;
		case MeterClrStep1:			// 电表清零步骤1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
			AllECClr();		//清除当前电量、历史电量指针、项数
			SM.MeterClrState = MeterClrStep2;
			SM.MeterClrStateChk = SM.MeterClrState + 0xA5A5;
			break;
		case MeterClrStep2:			// 电表清零步骤1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
			SM.MeterClrState = MeterClrIdle;
			SM.MeterClrStateChk = MeterClrIdle;
			MD_Rest(1);		//清当前需量
//			AllECClr();		//清除当前电量、历史电量指针、项数
	
	#if ( IranDemo_JUMP == YesCheck )
	   if(1)
	   //if( SM.PrgDly != 0 )
	#else
			if(( PIN_SETEN & P_SETEN ) == 0 )
	#endif			
			{
				Clr_E2PData( Bat_WorkTime, 4, 1 );   //清电池工作时间
				Real.RBat_WorkTime = 0;
				Real.BatWorkTimeChk = 0xA5;
				InitialProfile_genericClassPara();
				InitialSingle_action_scheduleClassPara();
//				InitialEventLog();
				initialMeterConstPara();
				InitialDemandPara();
				initialRegister_ActivationPara();		//初始化激活状态..
				initialTariffication_Script_TablePara(); 	//初始化费率激活对应表..
				initialRegister_monitorPara();			//寄存器监视..
			}	
//
//			RAM_Fill((unsigned char*)&SM.CoverState, 2 );			//清除开盖状态字
//			RAM_Fill((unsigned char*)&SM.TerminalState, 2 );		//清除开端状态字
			
			//清除MOBUS事件参数..
			Mobus_meter_clr();
			//事件清零..
			clr_event_Ram();
			//清电压合格率事件..
			Clr_Volt_qulityevent();
			SM.Profile1_status = 0;				
			SM.Profile2_status = 0;
			//初始化事件..				
			InitialProfile_genericLog();
			InitialEventLog();
			
			//事件清零事件..
			SM.StandardEventCode = Electricity_load_profiled_cleared;
			LoadRecord( Standard_Event_No );
			SM.Event_all_flag |= Standard_Event_BIT;
			SM.Error_all_flag |= Standard_Event_BIT;    		                                   
			break;
		default: 
			SM.MeterClrState = MeterClrIdle;
			SM.MeterClrStateChk = MeterClrIdle;			
			break;	
	}
}
*/
	void Clr_EC( void )
	{
		short i;
		unsigned char* Databuff;
		unsigned char*	ptr;
		unsigned char buff[6];
		ptr=buff;
	/*	RAM_Fill( (unsigned char*)&SM.PL_CumPp, ECUnitNum*2 );
		SM.PL_CumCheck = DoCrc16(0xFFFF,(unsigned char*)&SM.PL_CumPp,ECUnitNum*2);	
		E2P_WData( PowerDownSave, (unsigned char*)&SM.PL_CumPp, 12 );	
		
		RAM_Fill((unsigned char*)&SM.YPulse_Cum, 12 );	
		SM.PL_PowerOffCheck = ChkNumShort((unsigned short*)&SM.YPulse_Cum,6);	
		E2P_WData( PowerDownSave1, (unsigned char*)&SM.YPulse_Cum, 14 );	
		RAM_Fill(ptr,6);
		for(i = 0;i<ECUnitNum;i++)
		{
			ECRgSum( i,1);
			E2P_WData( ECRgTab[i].ECRAds, ptr, 2 );
		}
		*/	
		Clr_E2PECData( CMon_EC_P0, ECDIGIT, 12*9 );  //电量//Clr_E2PData( CMon_EC_Pp0, 4, 8 );  //电量
		///Clr_E2PECData( CMon_EC_Ap0, 6, 20 );  //清除A+ A- R+ R-总及分费率电量 fire-2016-6-29
	//	Clr_E2PData( L1Mon_EC_Pp0, 4, 8 );	//电量
	//	Clr_E2PData( L2Mon_EC_Pp0, 4, 8 );	//电量
	/// Clr_E2PData( Opencover_Time, 5, 2 );
	//	Clr_E2PData( User_ID, 8, 1 );
	/// Clr_E2PData( LastEC_Pt, 2, 1 );
	/// Clr_E2PData( GeneralMonCurAddr, 4, 1 );
	/*//fire
#if (FlashChip == YesCheck)	
		BlockErase( GeneralMonCurveSta );
#else
#endif	
	*/
	/// E2P_WData( Last_Mon_Tm, &Clk.Day[0], 2 );
	//	E2P_WData( Last_Mon_Tm, &Clk.Hour[0], 4 );
	//	RAM_Fill((unsigned char*)&SM.MD_CumPp, 22 );
	
		RAM_Fill(SM.BatteryWTime, 5 );
		Clr_E2PData( BatteryWorkTime, 4, 1 ); //清电池工作时间
	//	E2P_WData(PowerOffTime,Clk.Min,5);	
		E2P_WData(PowerOffTime,Clk.Sec,6);	//停电时间	
		
		///MD_Rest_RS();//fire
	/// SetLoadPara();			//fire
	/// SetTamperPara();		
	/// Clr_E2PData(JsCount,1,1);
		Clr_E2PData( MagFlag, 1, 3 );
	//	Clr_E2PData( BatteryWorkTime, 5, 1 );	//清电池工作时间
	/*
	///--------------2016-10-20----------------
		Clr_E2PData( MeterRestCnt, 3, 1 );
		Clr_E2PData( PhaseL1Cnt, 2, 3 );
		Clr_E2PData( OpenTCoverCnt, 2, 3 );
		Clr_E2PData( FristOTCover_Time, 6, 3 );
	///----------------------------------
	*/
		E2P_WData( Time_of_last_reset, (unsigned char*)&Clk.HourH, 4 );
		E2P_WData( Date_of_last_reset, (unsigned char*)&Clk.YearH, 5 );
	
	
	//	*Databuff = 0x30; 
	//	E2P_WData( Opencover_Stata, Databuff, 1 );	//清开盖闪烁	

	//Flag.ErrFlg &=~ F_ErrMeasFlashDis;
		
		RAM_Fill(Flag.AlarmFlg, 8 );
		
//		SM.ENCLRCount = 0;
//		SM.ILoadPulseSumP = 0;		
	
//		Comm.PassWordErrCnt = 0;
//		Comm.LockCnt = 0;	
	}

//电表清零操作
void MeterClrCycle( void )
{
	switch( SM.MeterClrState )	
	{
		case MeterClrIdle:			// 电表清零空闲
			SM.MeterClrStateChk = MeterClrIdle;			
			break;
		case MeterClrStep1:			// 电表清零步骤1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
			///AllECClr();		//清除当前电量、历史电量指针、项数
			Clr_EC();
			SM.MeterClrState = MeterClrStep2;
			SM.MeterClrStateChk = SM.MeterClrState + 0xA5A5;
			break;
		case MeterClrStep2:			// 电表清零步骤1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
		//	if( SM.PrgDly != 0 )
			if(( PIN_SETEN & P_SETEN ) == 0 )
				{
				MD_Rest();		//清当前需量
					//initialMeterConstPara();
				}
			SM.MeterClrState = MeterClrStep3;
			SM.MeterClrStateChk = SM.MeterClrState + 0xA5A5;
			break;
		case MeterClrStep3: 		// 电表清零步骤1
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
		//	if( SM.PrgDly != 0 )
		if(( PIN_SETEN & P_SETEN ) == 0 )
				{
				InitialDemandPara();
				initialRegister_ActivationPara();		//初始化激活状态..
				initialTariffication_Script_TablePara(); 	//初始化费率激活对应表..
				}
			SM.MeterClrState = MeterClrStep4;
			SM.MeterClrStateChk = SM.MeterClrState + 0xA5A5;
			break;
		case MeterClrStep4: 		// 电表清零步骤2
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
			InitialProfile_genericClassPara();
			
			SM.MeterClrState = MeterClrStep5;
			SM.MeterClrStateChk = SM.MeterClrState + 0xA5A5;
			break;
		case MeterClrStep5:			// 电表清零步骤2
			if( SM.MeterClrStateChk != ( SM.MeterClrState +0xA5A5 ) ) break;
			SM.MeterClrState = MeterClrIdle;
			SM.MeterClrStateChk = MeterClrIdle;
	//		MD_Rest();		//清当前需量
//			AllECClr();		//清除当前电量、历史电量指针、项数
	
	//   if( SM.PrgDly != 0 )
			if(( PIN_SETEN & P_SETEN ) == 0 )
			{
			///	Clr_E2PData( Bat_WorkTime, 4, 1 );   //清电池工作时间
			///	Real.RBat_WorkTime = 0;
			///	Real.BatWorkTimeChk = 0xA5;
				//InitialProfile_genericClassPara();
				//InitialSingle_action_scheduleClassPara();
//				InitialEventLog();
				//HT_FreeDog();
				//initialMeterConstPara();
				//InitialDemandPara();
				//initialRegister_ActivationPara();		//初始化激活状态..
				//initialTariffication_Script_TablePara(); 	//初始化费率激活对应表..
				
				InitialSingle_action_scheduleClassPara();
				initialRegister_monitorPara();			//寄存器监视..
			}	

//			RAM_Fill((unsigned char*)&SM.CoverState, 2 );			//清除开盖状态字
//			RAM_Fill((unsigned char*)&SM.TerminalState, 2 );		//清除开端状态字
			
			//清除MOBUS事件参数..
		///	Mobus_meter_clr();
			//事件清零..
			clr_event_Ram();
			//清电压合格率事件..
			Clr_Volt_qulityevent();
			SM.Profile1_status = 0;				
			SM.Profile2_status = 0;
			//初始化事件..				
			InitialProfile_genericLog();
			InitialEventLog();
			
			//事件清零事件..
			SM.StandardEventCode = Electricity_load_profiled_cleared;
			LoadRecord( Standard_Event_No );
			SM.Event_all_flag |= Standard_Event_BIT;
			SM.Error_all_flag |= Standard_Event_BIT;   
			
			break;
		default: 
			SM.MeterClrState = MeterClrIdle;
			SM.MeterClrStateChk = MeterClrIdle;			
			break;	
	}
}
/*
void Mobus_meter_clr(void)
{
	unsigned long Value=0;
	//气表..
	E2P_WData( GasMeter_SAP, (unsigned char*)&Value, 2 );
	E2P_WData( GasMeter_HDLC_device_address, (unsigned char*)&Value, 2 );
	E2P_WData( GasMeter_ReadValue, (unsigned char*)&Value, 4 );			
	//水表..
	E2P_WData( WaterMeter_SAP, (unsigned char*)&Value, 2 );
	E2P_WData( WaterMeter_HDLC_device_address, (unsigned char*)&Value, 2 );
	E2P_WData( WaterMeter_ReadValue, (unsigned char*)&Value, 4 );											
}
*/
//清电压合格率事件..
void	Clr_Volt_qulityevent(void)
{
	  unsigned char i;
		unsigned long Value=0;
		
		for( i=0;i<15;i++ )										//清电压合格率事件..
		{
				E2P_WData( voltageL1Counter1 + i*5, (unsigned char*)&Value, 4 );
		}	
}
