#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Measure.h"
#include "Port.h"
#include "TypeRAM.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "ht6xxx_lib.h"

#include "Mem.h"
#include "Time.h"
#include "Power.h"
#include "Initial.h"
#include "RegisterActivation.h"

#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "ActCalendar.h"
#include "RegisterMonitor.h"
#include "SingleAction.h"
#include "ProfileGeneric.h"
//#include "Demand.h"
#include "DisconnectControl.h"
#include "ScriptTable.h"
#include "AssociationLN.h"
#include "Limiter.h"
#include "ComSetup.h"
/*
//void InitialProfile_genericClassPara( void )
void InitialProfClassPara_Billing_period1( void )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short ItemNo; 
	unsigned short ItemLen;
    unsigned long Value;
	short i;
	
	Ptr = Buff;

	E2P_WData( Time_of_recent_billing_period, (unsigned char*)&Clk.YearH, 12 );
	Value = 0;
	E2P_WData( Billing_period_counter, (unsigned char*)&Value, 4 );
	
	*Ptr = 11;
	E2P_WData( Billing_period1_number, Ptr, 1 );
	*Ptr = 0x40;
	*(Ptr+1) = 0x00;
	E2P_WData( Billing_period1_RecdLenth, Ptr, 2 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x08;
	*(Ptr+2) = 0x00;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x00;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Billing_period1_capture_objects1, Ptr, 11 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	for( i=0;i<5;i++ )
	{
		*(Ptr+6) = i;
		E2P_WData( Billing_period1_capture_objects2+i*12, Ptr, 11 );	
	}	
	
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x02;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	for( i=0;i<5;i++ )
	{
		*(Ptr+6) = i;
		E2P_WData( Billing_period1_capture_objects7+i*12, Ptr, 11 );	
	}	

	ItemNo = BILLING_PERIOD1_NO;
	ItemLen = 64;
	InitialProfilePara( ItemNo, ItemLen );
	
	RAM_Fill( Ptr, 4 ); 	
	E2P_WData( Billing_period1_CurRecNo, Ptr, 4 );	
	E2P_WData( Billing_period1_Pt, Ptr, 4 );		
	E2P_WData( Billing_period1_entries_in_use, Ptr, 4 );		
	
}	

void InitialProfClassPara_Billing_period2( void )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short ItemNo; 
	unsigned short ItemLen;
    unsigned long Value;
	short i;
	
	Ptr = Buff;

//	E2P_WData( Time_of_recent_billing_period, (unsigned char*)&Clk.YearH, 12 );
//	Value = 0;
//	E2P_WData( Billing_period_counter, (unsigned char*)&Value, 4 );
	
	*Ptr = 11;
	E2P_WData( Billing_period2_number, Ptr, 1 );
	*Ptr = 0x40;
	*(Ptr+1) = 0x00;
	E2P_WData( Billing_period2_RecdLenth, Ptr, 2 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x08;
	*(Ptr+2) = 0x00;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x00;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Billing_period2_capture_objects1, Ptr, 11 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	for( i=0;i<5;i++ )
	{
		*(Ptr+6) = i;
		E2P_WData( Billing_period2_capture_objects2+i*12, Ptr, 11 );	
	}	
	
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x02;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	for( i=0;i<5;i++ )
	{
		*(Ptr+6) = i;
		E2P_WData( Billing_period2_capture_objects7+i*12, Ptr, 11 );	
	}	

	ItemNo = BILLING_PERIOD2_NO;
	ItemLen = 64;
	InitialProfilePara( ItemNo, ItemLen );
	
	RAM_Fill( Ptr, 4 ); 	
	E2P_WData( Billing_period2_CurRecNo, Ptr, 4 );	
	E2P_WData( Billing_period2_Pt, Ptr, 4 );		
	E2P_WData( Billing_period2_entries_in_use, Ptr, 4 );		
	
}	



void InitialProfClassPara_Load_profile1( void )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short ItemNo; 
	unsigned short ItemLen;
	
	Ptr = Buff;
	
	*Ptr = 0x03;
	E2P_WData( Load_profile1_number, Ptr, 1 );
	*Ptr = 0x18;
	*(Ptr+1) = 0x00;
	E2P_WData( Load_profile1_RecdLenth, Ptr, 2 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x08;
	*(Ptr+2) = 0x00;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x00;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Load_profile1_capture_objects1, Ptr, 11 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Load_profile1_capture_objects2, Ptr, 11 );	
	
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x02;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Load_profile1_capture_objects3, Ptr, 11 );	

	ItemNo = LOAD_PROFILE1_NO;
	ItemLen = 24;
	InitialProfilePara( ItemNo, ItemLen );
	
//	SM.Load_profile1_capture_period_Cnt = 60;
//	SM.Load_profile1_capture_period_Cnt = 300;
	SM.Load_profile1_capture_period_Cnt = 900;
	E2P_WData( Load_profile1_capture_period, (unsigned char*)&SM.Load_profile1_capture_period_Cnt, 4 );	
	
	RAM_Fill( Ptr, 4 ); 	
	E2P_WData( Load_profile1_CurRecNo, Ptr, 4 );	
	E2P_WData( Load_profile1_Pt, Ptr, 4 );		
	E2P_WData( Load_profile1_entries_in_use, Ptr, 4 );		
	
}	

void InitialProfClassPara_Load_profile2( void )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short ItemNo; 
	unsigned short ItemLen;
	
	Ptr = Buff;
	
	*Ptr = 0x03;
	E2P_WData( Load_profile2_number, Ptr, 1 );
	*Ptr = 0x18;
	*(Ptr+1) = 0x00;
	E2P_WData( Load_profile2_RecdLenth, Ptr, 2 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x08;
	*(Ptr+2) = 0x00;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x00;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Load_profile2_capture_objects1, Ptr, 11 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Load_profile2_capture_objects2, Ptr, 11 );	
	
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x02;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Load_profile2_capture_objects3, Ptr, 11 );	

	ItemNo = LOAD_PROFILE2_NO;
	ItemLen = 24;
	InitialProfilePara( ItemNo, ItemLen );
	
//	SM.Load_profile1_capture_period_Cnt = 60;
//	SM.Load_profile1_capture_period_Cnt = 300;
	SM.Load_profile1_capture_period_Cnt = 86400;
	E2P_WData( Load_profile2_capture_period, (unsigned char*)&SM.Load_profile2_capture_period_Cnt, 4 );	
	
	RAM_Fill( Ptr, 4 ); 	
	E2P_WData( Load_profile2_CurRecNo, Ptr, 4 );	
	E2P_WData( Load_profile2_Pt, Ptr, 4 );		
	E2P_WData( Load_profile2_entries_in_use, Ptr, 4 );		
	
}	


void InitialProfClassPara_Event_log1( void )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short ItemNo; 
	unsigned short ItemLen;
	short i;
    
	Ptr = Buff;
	
	*Ptr = 0x09;
	E2P_WData( Event_log1_number, Ptr, 1 );
	*Ptr = 54;
	*(Ptr+1) = 0x00;
	E2P_WData( Event_log1_RecdLenth, Ptr, 2 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x08;
	*(Ptr+2) = 0x00;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x00;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Event_log1_capture_objects1, Ptr, 11 );
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x01;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Event_log1_capture_objects2, Ptr, 11 );	
	
	*Ptr = 0x00;
	*(Ptr+1) = 0x03;
	*(Ptr+2) = 0x01;
	*(Ptr+3) = 0x00;
	*(Ptr+4) = 0x02;
	*(Ptr+5) = 0x08;
	*(Ptr+6) = 0x00;
	*(Ptr+7) = 0xff;
	*(Ptr+8) = 0x02;
	*(Ptr+9) = 0x00;
	*(Ptr+10) = 0x00;
	E2P_WData( Event_log1_capture_objects3, Ptr, 11 );	

	for( i=0;i<6;i++ )
	{
		RAM_Write( Ptr+2, (unsigned char*)DLMS_REALTab[i].Logical_Name, 6 );
		E2P_WData( Event_log1_capture_objects4+i*12, Ptr, 11 );	
	}	

	ItemNo = EVENT_LOG1_NO;
	ItemLen = 54;
	InitialProfilePara( ItemNo, ItemLen );
	
	RAM_Fill( Ptr, 4 ); 	
	E2P_WData( Event_log1_CurRecNo, Ptr, 4 );	
	E2P_WData( Event_log1_Pt, Ptr, 4 );		
	E2P_WData( Event_log1_entries_in_use, Ptr, 4 );		
	
}	
*/


unsigned short GetProfile_genericClassTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;
		
	for( i=0;i<DLMS_ProfileSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_ProfileTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	return i;	
}

//取Profile_generic类数据
short GetProfile_genericClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	unsigned short i = 0;
//	unsigned char Buff[10];
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	unsigned short Addr;
//	unsigned char Len=2;
	unsigned short Len=2;
	unsigned char Attribute;
				
	Ptr = Logical_Name;
	RAM_Write( Ptr, Source+2, 6 );
	
	i = GetProfile_genericClassTabItemNo( Ptr );
	if( i == DLMS_ProfileSum )
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}
	else 
	{
		*Dest = Get_data;											//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
	HT_FreeDog();
		switch( Attribute )
		{
			case 1:										//logical_name
				*(Dest+1) = Data_octet_string;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:										//buffer	
				Len = GetProfile_bufferData( COM_No, Dest, Source, i );
				break;	
			case 3:										//capture_objects
				Len = GetProfile_capture_objectsData( COM_No, Dest, i );
				break;	
			case 4:										//capture_period	
				Addr = DLMS_ProfileTab[i].PeriodAddr;
				*(Dest+1) = D_Unsigned32;
				E2P_RData( Dest+2, Addr, 4 );
				SwapData( Dest+2, 4 );
				Len = 6;
				break;	
			case 5:										//sort_method	
				*(Dest+1) = D_ENUM;
				*(Dest+2) = 0x01;						//fifo					
				Len = 3;
				break;
			case 6:										//sort_object	
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x04;
				*(Dest+3) = D_Unsigned16;
				*(Dest+4) = 0x00;
				*(Dest+5) = 0x00;		
				*(Dest+6) = Data_octet_string;
				*(Dest+7) = 0x06;
				RAM_Fill( Dest+8, 6);
				*(Dest+14) = D_Integer8;
				*(Dest+15) = 0x00;		
				*(Dest+16) = D_Unsigned16;
				*(Dest+17) = 0x00;
				*(Dest+18) = 0x00;		
				Len = 19;
				break;
			case 7:										//entries_in_use	
				Addr = DLMS_ProfileTab[i].EntriesUseAddr;
				*(Dest+1) = D_Unsigned32;
				E2P_RData( Dest+2, Addr, 4 );
				SwapData( Dest+2, 4 );
				Len = 6;
				break;
			case 8:										//profile_entries	
				Addr = DLMS_ProfileTab[i].EntriesAddr;
				*(Dest+1) = D_Unsigned32;
				E2P_RData( Dest+2, Addr, 4 );
				SwapData( Dest+2, 4 );
				Len = 6;
				break;
			default:
				break;	
		}	
	}		
	return Len;
}	

short GetProfile_bufferData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item )
{
	unsigned short Len;
	unsigned char Access_flag;
	unsigned char Access_selector;
	
	Access_flag = *(Source+MAC_Get_Set_Selection_Access_flag-3);
	Access_selector = *(Source+MAC_Get_Selection_Access_selector-3);
	
	if( Access_flag == 0 )
	{
		Len = GetProfile_bufferAllData( COM_No, Dest, Source, Item ); 
	}
	else
	{
		if(( Access_selector == 1 )&&( DLMS_ProfileTab[Item].Logical_Name[2] != 21 ))			//readout不支持选择性访问
		{
			Len = GetProfile_bufferRangeData( COM_No, Dest, Source, Item ); //按时间范围读取
		}
		else if(( Access_selector == 2 )&&( DLMS_ProfileTab[Item].Logical_Name[2] != 21 ))		//readout不支持选择性访问
		{
			Len = GetProfile_bufferEntryData( COM_No, Dest, Source, Item ); 			
		}			
		else
		{
				*Dest = Get_data_access_result;					//异常标志
			//	*(Dest+1) = other_reason; 		
				*(Dest+1) = object_unavailable;					
				Len = 2;					
//			*(Dest+1) = other_reason; 							//选择性访问参数错
//			Len = 2;		
		}			
	}	
	return Len;	
}	

short GetProfile_bufferEntryData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item )
{ 
    unsigned long LongBuff[8];
//	unsigned long From_entry;
//	unsigned long To_entry;
	unsigned long GetCurRecNo;	
	unsigned long TotalGetRecNumber;
	CURVEPARA* CurvePara;
	unsigned long* BlockNumber;
	unsigned char* SubsBlockPara;	
	unsigned short Length,temp_lenth;
	unsigned short i;
	unsigned char *Ptr;		

	BlockNumber = CommPara[COM_No].BlockNumber;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	CurvePara = (CURVEPARA*)LongBuff;
	
	CurvePara->ChannelNo = Item;
	GetCurvePara( CurvePara );
	
	PushSetup.PReadCurRecNo = CurvePara->CurRecNo;     //skyblue20140930..
		
	if( CurvePara->Pages == 0 )
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = other_reason; 		
	//	*(Dest+1) = object_unavailable;					
		Length = 2;		
		return Length;										//该曲线分配的空间为空
	}	
	if( Judge_entry_descriptor( Source+MAC_Get_Profile_Para-3 ) != 0 )//判断数据格式
	{
		*Dest = Get_data_access_result;					//异常标志
	//	*(Dest+1) = other_reason; 		
		*(Dest+1) = object_unavailable;					
		Length = 2;		
		return Length;										//GET格式有误
	}	
	if( GetEntryLoadRecNo( SubsBlockPara+MAC_Get_Profile_Para, CurvePara, Source+MAC_Get_Profile_Para-3 ) != 0 )
	{
		*(Dest+1) = Data_array; 							
		*(Dest+2) = 0x00; 							
		Length = 3;		
		return Length;										//无有效记录
//		*Dest = Get_data_access_result;					//异常标志
//	//	*(Dest+1) = other_reason; 		
//		*(Dest+1) = object_unavailable;					
//		Length = 2;		
//		return Length;										//GET格式有误
	}

		
//	RAM_Write( (unsigned char*)&From_entry, SubsBlockPara+MAC_Get_Profile_Para, 4 );			//准备返回的最早记录号
//	RAM_Write( (unsigned char*)&To_entry, SubsBlockPara+MAC_Get_Profile_Para+4, 4 );			//准备返回的最迟记录号
	RAM_Write( (unsigned char*)&GetCurRecNo, SubsBlockPara+MAC_Get_Profile_Para, 4 );			//准备返回的记录号
	RAM_Write( (unsigned char*)&TotalGetRecNumber, SubsBlockPara+MAC_Get_Profile_Para+4, 4 );	//返回的总记录数
	
 	if(TotalGetRecNumber==0)
	{
		*(Dest+1) = Data_array; 							
		*(Dest+2) = 0x00; 							
		Length = 3;		
		return Length;										//无有效记录
	}		
        
	//填加BLOCK读取多条纪录..
	Length = 5;
  temp_lenth = Length;
  Ptr = Dest+20;
	for(i=0;i<TotalGetRecNumber;i++)
	{
		Get_sigle_curve_entry_data( COM_No, Ptr, CurvePara, GetCurRecNo, &Length );
		GetCurRecNo -= 1;
		temp_lenth = Length - temp_lenth;
		if(temp_lenth<=4){		//结束..
			 i = TotalGetRecNumber;
			 break;
		}
		Ptr = Ptr + temp_lenth;
		temp_lenth = Length + temp_lenth;
		// MAX_INFO_SIZE ---测试修改sky2014.5.4.
		//if(temp_lenth>=MAX_INFO_SIZE ){
		if(temp_lenth>=MAX_PDU_SIZE - 40)
		///if(temp_lenth>=MAX_PDU_SIZE - 25)
		{
			 //GetCurRecNo += 1;
			  i++;
			 break;
		}
		temp_lenth = Length;
//		GetCurRecNo -=1;			
	}
	
	if( i == TotalGetRecNumber )  				//捕获对象参数一个PDU能返回完毕，不需要使用块操作
	{
		*(Dest+1) = Data_array;
//		*(Dest+2) = 0x84;											//长度字节为4字节
//		FillIntegerToBuff( Dest+3, TotalGetRecNumber );				
//		Length = 7;
		*(Dest+2) = 0x82;											//长度字节为4字节
		FillIntegerToBuff( Dest+3, TotalGetRecNumber );				
//		Length = 5;
//		Get_sigle_curve_data( Dest+5, CurvePara, GetCurRecNo, &Length );
		RAM_Write( Dest+5, Dest+20, (Length-5) );
		*BlockNumber = 0;
	}
	else															//捕获对象参数一个PDU不能返回完毕，需要使用块操作
	{	
		RAM_Write( Dest+13, Dest+20, (Length-5) );
		*Dest = 0;													//last-block=0表示不是最后一帧数据，后续还有数据块。
		*BlockNumber = 1;
		FillLongToBuff( Dest+1, *BlockNumber );						//block-number 块数，=1为传输的第一块。
		*(Dest+5) = 0x00;											//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
		*(Dest+6) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
		Length -= 1;													//记录数2字节
		FillIntegerToBuff( Dest+7, Length );
		Length += 9;
									
	//	*(Dest+7) = 0x01;                               			
	//	*(Dest+8) = 0xBF;											//数据长度为0x1BF=447字节 
		*(Dest+9) = Data_array;										//Data数据类型1,指其后为SEQUENCE OF Data数组
//		*(Dest+10) = 0x84;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
		*(Dest+10) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
	//	*(Dest+11) = 0x00;											//数据长度为0x1BF=447字节 
	//	*(Dest+12) = 0x80;											//共128个数组，对应于128个object_list_element
//		FillLongToBuff( Dest+11, TotalGetRecNumber );
		FillIntegerToBuff( Dest+11, TotalGetRecNumber );			//记录数必须为2字节，不能为4字节，否则CTT出错
//		Length = 13;
//		Length = 6;													//记录数4字节	
	//	GetCurRecNo -= 1;
		RAM_Write( SubsBlockPara+MAC_Get_Profile_Para, (unsigned char*)&GetCurRecNo, 4 );	//下一次准备返回的记录号
		TotalGetRecNumber -= i;
		RAM_Write( SubsBlockPara+MAC_Get_Profile_Para+4, (unsigned char*)&TotalGetRecNumber, 4 );	
	}	
	return Length;	
}	

short GetProfile_bufferRangeData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item )
{ 
    unsigned long LongBuff[8];
//	unsigned long From_entry;
//	unsigned long To_entry;
	unsigned long GetCurRecNo;	
	unsigned long TotalGetRecNumber;
	CURVEPARA* CurvePara;
	unsigned long* BlockNumber;
	unsigned char* SubsBlockPara;	
	unsigned short Length,temp_lenth;
		 unsigned short i;
	unsigned char *Ptr;		
	unsigned short RestrictAddr;
	short ErrorNo;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	CurvePara = (CURVEPARA*)LongBuff;
	
	CurvePara->ChannelNo = Item;
	GetCurvePara( CurvePara );
	PushSetup.PReadCurRecNo = CurvePara->CurRecNo;     //skyblue20140930..
		
	if( CurvePara->Pages == 0 )
	{
		*Dest = Get_data_access_result;						//异常标志
		*(Dest+1) = other_reason; 							
		Length = 2;		
		return Length;										//该曲线分配的空间为空
	}	
	if( Judge_range_descriptor( Item, SubsBlockPara+MAC_Get_Profile_Para+8, Source+MAC_Get_Profile_Para-3, &RestrictAddr ) != 0 )
	{
		*Dest = Get_data_access_result;					//异常标志
	//	*(Dest+1) = other_reason; 		
		*(Dest+1) = object_unavailable;					
		Length = 2;		
		return Length;									     	//该曲线分配的空间为空
	}	
	if( (ErrorNo=GetAppointTimeLoadRecNo( SubsBlockPara+MAC_Get_Profile_Para, Source+MAC_Get_Profile_Para-3+20+2, Source+MAC_Get_Profile_Para-3+34+2, CurvePara, RestrictAddr )) != 0 )
	{
	  if(ErrorNo!=3)
	  {
			*(Dest+1) = Data_array; 							
		*(Dest+2) = 0x00; 							
		Length = 3;		
		return Length;										//无有效记录
	}
	else
		{
		*Dest = Get_data_access_result;					//异常标志
	//	*(Dest+1) = other_reason; 		
		*(Dest+1) = object_unavailable;					
		Length = 2;		
		return Length;									     	//该曲线分配的空间为空
	  }
	}
			

//	RAM_Write( (unsigned char*)&From_entry, SubsBlockPara+MAC_Get_Profile_Para, 4 );			//准备返回的最早记录号
//	RAM_Write( (unsigned char*)&To_entry, SubsBlockPara+MAC_Get_Profile_Para+4, 4 );			//准备返回的最迟记录号
	RAM_Write( (unsigned char*)&GetCurRecNo, SubsBlockPara+MAC_Get_Profile_Para, 4 );			//准备返回的记录号
	RAM_Write( (unsigned char*)&TotalGetRecNumber, SubsBlockPara+MAC_Get_Profile_Para+4, 4 );	//返回的总记录数

	if(TotalGetRecNumber==0)
	{
		*(Dest+1) = Data_array; 							
		*(Dest+2) = 0x00; 							
		Length = 3;		
		return Length;										//无有效记录
	}
		
	//填加BLOCK读取多条纪录..
	Length = 5;
  temp_lenth = Length;
  Ptr = Dest+20;
  HT_FreeDog();
	for(i=0;i<TotalGetRecNumber;i++)
	{//逐步递减，读取所有记录，TotalGetRecNumber是读取记录的数量，GetCurRecNo记录号最大值
		Get_sigle_curve_range_data( COM_No, Ptr, CurvePara, GetCurRecNo, &Length );
		GetCurRecNo -= 1;
		temp_lenth = Length - temp_lenth;
		if(temp_lenth<=4){		//结束..
			 i = TotalGetRecNumber;
			 break;
		}
		Ptr = Ptr + temp_lenth;
		temp_lenth = Length + temp_lenth;
		// MAX_INFO_SIZE ---测试修改sky2014.5.4.
		//if(temp_lenth>=MAX_INFO_SIZE ){
		if(temp_lenth>=MAX_PDU_SIZE - 40){
			 //GetCurRecNo += 1;
			 i++;   //sky2014..
			 break;
		}
		temp_lenth = Length;
//		GetCurRecNo -=1;			
	}
	
	if( i == TotalGetRecNumber )  				//捕获对象参数一个PDU能返回完毕，不需要使用块操作
	{
		*(Dest+1) = Data_array;
//		*(Dest+2) = 0x84;											//长度字节为4字节
//		FillIntegerToBuff( Dest+3, TotalGetRecNumber );				
//		Length = 7;
		*(Dest+2) = 0x82;											//长度字节为4字节
		FillIntegerToBuff( Dest+3, TotalGetRecNumber );				
//		Length = 5;
//		Get_sigle_curve_data( Dest+5, CurvePara, GetCurRecNo, &Length );
		RAM_Write( Dest+5, Dest+20, (Length-5) );
		*BlockNumber = 0;
	}
	else															//捕获对象参数一个PDU不能返回完毕，需要使用块操作
	{	
		RAM_Write( Dest+13, Dest+20, (Length-5) );
		*Dest = 0;													//last-block=0表示不是最后一帧数据，后续还有数据块。
		*BlockNumber = 1;
		FillLongToBuff( Dest+1, *BlockNumber );						//block-number 块数，=1为传输的第一块。
		*(Dest+5) = 0x00;											//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
		*(Dest+6) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
		Length -= 1;													//记录数2字节
		FillIntegerToBuff( Dest+7, Length );
		Length += 9;
									
	//	*(Dest+7) = 0x01;                               			
	//	*(Dest+8) = 0xBF;											//数据长度为0x1BF=447字节 
		*(Dest+9) = Data_array;										//Data数据类型1,指其后为SEQUENCE OF Data数组
//		*(Dest+10) = 0x84;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
		*(Dest+10) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
	//	*(Dest+11) = 0x00;											//数据长度为0x1BF=447字节 
	//	*(Dest+12) = 0x80;											//共128个数组，对应于128个object_list_element
//		FillLongToBuff( Dest+11, TotalGetRecNumber );
		FillIntegerToBuff( Dest+11, TotalGetRecNumber );			//记录数必须为2字节，不能为4字节，否则CTT出错
//		Length = 13;
//		Length = 6;													//记录数4字节	
	//	GetCurRecNo -= 1;
		RAM_Write( SubsBlockPara+MAC_Get_Profile_Para, (unsigned char*)&GetCurRecNo, 4 );	//下一次准备返回的记录号
		TotalGetRecNumber -= i;
		RAM_Write( SubsBlockPara+MAC_Get_Profile_Para+4, (unsigned char*)&TotalGetRecNumber, 4 );	
				
	}	
	return Length;	
}	

short GetProfile_bufferAllReadoutData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item )
{ 
	unsigned long* BlockNumber;
	unsigned char* SubsBlockPara;	
	unsigned short TotalGetRecNumber;
	unsigned short Length;	

	BlockNumber = CommPara[COM_No].BlockNumber;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	
	TotalGetRecNumber = 1;
	RAM_Write( SubsBlockPara+MAC_Get_Profile_Para+4, (unsigned char*)&TotalGetRecNumber, 4 );//准备返回的总记录数
	
	*(Dest+1) = Data_array;
	*(Dest+2) = 0x01;											//长度字节为1字节
	Length = 3;
	Get_sigle_readout_data( Dest+3, Item, &Length );	
	*BlockNumber = 0;

	return Length;	
}	

short GetProfile_bufferAllData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item )
{ 
    unsigned short LongBuff[8];
	CURVEPARA* CurvePara;
	unsigned long* BlockNumber;
	unsigned char* SubsBlockPara;	
	unsigned long GetCurRecNo;	
	unsigned short TotalGetRecNumber;
	unsigned short Length,temp_lenth;
		 long i;
	unsigned char *Ptr;	

	BlockNumber = CommPara[COM_No].BlockNumber;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	CurvePara = (CURVEPARA*)LongBuff;
	
	if(( Item >= General_display_No )&&( Item <= Ins_Energy_Readout_No ))
	{
		Length = GetProfile_bufferAllReadoutData( COM_No, Dest, Source, Item );
		return Length;										
	}	
	
	CurvePara->ChannelNo = Item;
	GetCurvePara( CurvePara );
	
	PushSetup.PReadCurRecNo = CurvePara->CurRecNo;     //skyblue20140930..
	
	if( CurvePara->Pages == 0 )
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = other_reason; 							
		Length = 2;		
		return Length;										//该曲线分配的空间为空
	}	
	if( CurvePara->AvailRecs == 0 )
	{
		*(Dest+1) = Data_array; 							
		*(Dest+2) = 0x00; 							
		Length = 3;		
		return Length;										//无有效记录
	}
	
	GetCurRecNo = CurvePara->AvailRecs;
	//GetCurRecNo -= 1;
	TotalGetRecNumber = CurvePara->AvailRecs;
	RAM_Write( SubsBlockPara+MAC_Get_Profile_Para+4, (unsigned char*)&TotalGetRecNumber, 4 );//准备返回的总记录数

	//填加BLOCK读取多条纪录..
	Length = 5;
  temp_lenth = Length;
  Ptr = Dest+20;
	for(i=GetCurRecNo;i>0;i--)
	{
		GetCurRecNo -= 1;
		Get_sigle_curve_data( Ptr, CurvePara, GetCurRecNo, &Length );
		temp_lenth = Length - temp_lenth;
		if(temp_lenth<=4){		//结束..
			 i = 0;
			 break;
		}
		Ptr = Ptr + temp_lenth;
		temp_lenth = Length + temp_lenth;
		// MAX_INFO_SIZE ---测试修改sky2014.5.4.
		//if(temp_lenth>=MAX_INFO_SIZE ){
		if(temp_lenth>=MAX_PDU_SIZE - 40){
			 //GetCurRecNo += 1;
			 if(i>0)i--;
			 break;
		}
		temp_lenth = Length;
//		GetCurRecNo -=1;			
	}
	
	if(( i == 0 )||(GetCurRecNo==0))  				//捕获对象参数一个PDU能返回完毕，不需要使用块操作
	{
		*(Dest+1) = Data_array;
//		*(Dest+2) = 0x84;											//长度字节为4字节
//		FillIntegerToBuff( Dest+3, TotalGetRecNumber );				
//		Length = 7;
		*(Dest+2) = 0x82;											//长度字节为2字节
		FillIntegerToBuff( Dest+3, TotalGetRecNumber );				
//		Length = 5;
//		Get_sigle_curve_data( Dest+5, CurvePara, GetCurRecNo, &Length );
		RAM_Write( Dest+5, Dest+20, (Length-5) );
		*BlockNumber = 0;
	}
	else															//捕获对象参数一个PDU不能返回完毕，需要使用块操作
	{	
		RAM_Write( Dest+13, Dest+20, (Length-5) );
		*Dest = 0;													//last-block=0表示不是最后一帧数据，后续还有数据块。
		*BlockNumber = 1;
		FillLongToBuff( Dest+1, *BlockNumber );						//block-number 块数，=1为传输的第一块。
		*(Dest+5) = 0x00;											//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
		*(Dest+6) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
		Length -= 1;													//记录数2字节
		FillIntegerToBuff( Dest+7, Length );
		Length += 9;
									
	//	*(Dest+7) = 0x01;                               			
	//	*(Dest+8) = 0xBF;											//数据长度为0x1BF=447字节 
		*(Dest+9) = Data_array;										//Data数据类型1,指其后为SEQUENCE OF Data数组
//		*(Dest+10) = 0x84;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
		*(Dest+10) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
	//	*(Dest+11) = 0x00;											//数据长度为0x1BF=447字节 
	//	*(Dest+12) = 0x80;											//共128个数组，对应于128个object_list_element
//		FillLongToBuff( Dest+11, TotalGetRecNumber );
		FillIntegerToBuff( Dest+11, TotalGetRecNumber );			//记录数必须为2字节，不能为4字节，否则CTT出错
//		Length = 13;
//		Length = 6;													//记录数4字节	
	//	GetCurRecNo -= 1;
		RAM_Write( SubsBlockPara+MAC_Get_Profile_Para, (unsigned char*)&GetCurRecNo, 4 );	//下一次准备返回的记录号
	}	
	return Length;	
}	

//short GetProfile_bufferAllData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item )
//{ 
//    unsigned long LongBuff[8];
//	CURVEPARA* CurvePara;
//	unsigned long* BlockNumber;
//	unsigned char* SubsBlockPara;	
//	unsigned long GetCurRecNo;	
//	unsigned long TotalGetRecNumber;
//	unsigned short Length;	
//
//	BlockNumber = CommPara[COM_No].BlockNumber;
//	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
//	CurvePara = (CURVEPARA*)LongBuff;
//	
//	if(( Item >= General_display_No )&&( Item <= Ins_Energy_Readout_No ))
//	{
//		Length = GetProfile_bufferAllReadoutData( COM_No, Dest, Source, Item );
//		return Length;										
//	}	
//	
//	CurvePara->ChannelNo = Item;
//	GetCurvePara( CurvePara );
//	if( CurvePara->Pages == 0 )
//	{
//		*Dest = Get_data_access_result;					//异常标志
//		*(Dest+1) = other_reason; 							
//		Length = 2;		
//		return Length;										//该曲线分配的空间为空
//	}	
//	if( CurvePara->AvailRecs == 0 )
//	{
//		*(Dest+1) = Data_array; 							
//		*(Dest+2) = 0x00; 							
//		Length = 3;		
//		return Length;										//无有效记录
//	}
//	GetCurRecNo = CurvePara->AvailRecs;
//	GetCurRecNo -= 1;
////	RAM_Write( SubsBlockPara+MAC_Get_Profile_Para, (unsigned char*)&Value, 4 );			//准备返回的最早记录号
////	RAM_Fill( SubsBlockPara+MAC_Get_Profile_Para+4, 4 );								//准备返回的最迟记录号
//	TotalGetRecNumber = CurvePara->AvailRecs;
//	RAM_Write( SubsBlockPara+MAC_Get_Profile_Para+4, (unsigned char*)&TotalGetRecNumber, 4 );//准备返回的总记录数
//	
//	if( TotalGetRecNumber == 1 )  				//捕获对象参数一个PDU能返回完毕，不需要使用块操作
//	{
//		*(Dest+1) = Data_array;
////		*(Dest+2) = 0x84;											//长度字节为4字节
////		FillIntegerToBuff( Dest+3, TotalGetRecNumber );				
////		Length = 7;
//		*(Dest+2) = 0x82;											//长度字节为4字节
//		FillIntegerToBuff( Dest+3, TotalGetRecNumber );				
//		Length = 5;
////		Get_sigle_curve_data( Dest+7, CurvePara, Value, &Length );
////		Get_sigle_curve_data( Dest+7, CurvePara, GetCurRecNo, &Length );
//		Get_sigle_curve_data( Dest+5, CurvePara, GetCurRecNo, &Length );
//		*BlockNumber = 0;
//	}
//	else															//捕获对象参数一个PDU不能返回完毕，需要使用块操作
//	{	
//		*Dest = 0;													//last-block=0表示不是最后一帧数据，后续还有数据块。
//		*BlockNumber = 1;
//		FillLongToBuff( Dest+1, *BlockNumber );						//block-number 块数，=1为传输的第一块。
//		*(Dest+5) = 0x00;											//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
//		*(Dest+6) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	//	*(Dest+7) = 0x01;                               			
//	//	*(Dest+8) = 0xBF;											//数据长度为0x1BF=447字节 
//		*(Dest+9) = Data_array;										//Data数据类型1,指其后为SEQUENCE OF Data数组
////		*(Dest+10) = 0x84;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//		*(Dest+10) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	//	*(Dest+11) = 0x00;											//数据长度为0x1BF=447字节 
//	//	*(Dest+12) = 0x80;											//共128个数组，对应于128个object_list_element
////		FillLongToBuff( Dest+11, TotalGetRecNumber );
//		FillIntegerToBuff( Dest+11, TotalGetRecNumber );			//记录数必须为2字节，不能为4字节，否则CTT出错
////		Length = 13;
////		Length = 6;													//记录数4字节	
//		Length = 4;													//记录数2字节		
////		Get_sigle_curve_data( Dest+15, CurvePara, Value, &Length );
////		Get_sigle_curve_data( Dest+15, CurvePara, GetCurRecNo, &Length );
//		Get_sigle_curve_data( Dest+13, CurvePara, GetCurRecNo, &Length );
//		if( Length >= 128 )
//		{
//			FillIntegerToBuff( Dest+7, Length );
//			Length += 9;
//		}			
//		else
//		{
//			*(Dest+6) = Length;
//			RAM_Write( Dest+7, Dest+9, Length );
//			Length += 7;				
//		}
//
//		GetCurRecNo -= 1;
//		RAM_Write( SubsBlockPara+MAC_Get_Profile_Para, (unsigned char*)&GetCurRecNo, 4 );	//下一次准备返回的记录号
//	}	
//	return Length;	
//}	

short GetCaptureItems( short ProfItemNo )
{
	unsigned short Addr;
	unsigned short Temp;
    unsigned short CaptureItems=0;   //sky2014..
	
	Addr = DLMS_ProfileTab[ProfItemNo].CapNumAddr;
	CaptureItems = 0;
	E2P_RData( (unsigned char*)&CaptureItems, Addr, 1 );
	Temp = DLMS_ProfileTab[ProfItemNo].CapNumMax;
	if( CaptureItems > Temp ) CaptureItems = Temp;					//不超出最大物理存储项数
	
	return CaptureItems;
}	

void Get_sigle_curve_entry_data( short COM_No, unsigned char* Dest, CURVEPARA* CurvePara, long LoadRecNo, unsigned short* Length )	
{
	unsigned char Buff[12];
	long Value;		
	unsigned short CaptureItems;
	unsigned char* SubsBlockPara;
	unsigned char* Ptr;
	unsigned char* Point;	
	unsigned short From_selected_value;
	unsigned short To_selected_value;
	unsigned short Addr;
	unsigned short DataLen;
	unsigned short StartAddr;
	unsigned short EndAddr;
	short i;
	
	Point = Buff;
//	SubsBlockPara = CommPara[CurvePara->ChannelNo].SubsBlockPara;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	CaptureItems = *(SubsBlockPara+MAC_Get_Profile_Para+8);

	*Dest = Data_structure;		
	if( LoadRecNo == -1 )
	{
		*(Dest+1) = 0x00;
		*Length += 2;
		return;
	}	
	Value = LoadPtrChk( CurvePara, LoadRecNo );
	if( Value == -1 )												//所抄数据超限	
	{
		*(Dest+1) = 0x00;
		*Length += 2;
	}
	else
	{	
		if( CaptureItems >= 128 )									//必须自动调整长度字节结构，否则CTT提示出错
		{
			*(Dest+1) = 0x82;
			FillIntegerToBuff( Dest+2, CaptureItems );
			*Length += 4;
			Dest += 4;
		}
		else	
		{	
			*(Dest+1) = CaptureItems;
			*Length += 2;
			Dest += 2;
		}
		Ptr = SubsBlockPara+MAC_Get_Profile_Para+8+1;
		RAM_Write( (unsigned char*)&From_selected_value, Ptr, 2 );	
		RAM_Write( (unsigned char*)&To_selected_value, Ptr+2, 2 );	
		
		Addr = DLMS_ProfileTab[CurvePara->ChannelNo].CapObjAddr;
		EndAddr = 0;
		for( i=0;i<To_selected_value;i++ )
		{
			DataLen = 0;
			E2P_RData( Point, Addr, 11 );
			if( Judge_capture_objects_content( Point, &DataLen ) != 0 ) 
			{
				*(Dest+1) = 0x00;
				*Length += 2;
				return;
			}	
			Addr += 12;
//			EndAddr += DataLen;
			if( (i+1) == From_selected_value )
			{
				StartAddr = EndAddr;	
			}	
			EndAddr += DataLen;
		}
//		DataLen += ( EndAddr - StartAddr );	
		DataLen = ( EndAddr - StartAddr );	
		
//		if(( StartAddr + DataLen ) > CurvePara->RecdLenth )					//数据地址或长度超长
		if( EndAddr > CurvePara->RecdLenth )								//数据地址或长度超长
		{
			*(Dest+1) = 0x00;
			*Length += 2;
		}
		else
		{ 
			Read_Flash( Dest, Value+StartAddr, DataLen );//从Flash内记录存储地址处读出数据
			*Length += DataLen;
		}	
	}
}	

void Get_sigle_curve_range_data( short COM_No, unsigned char* Dest, CURVEPARA* CurvePara, long LoadRecNo, unsigned short* Length )	
{
	long Value;		
	///unsigned short CaptureItems;
	
	unsigned long Addr;
	unsigned short Len;
	unsigned short CaptureItems=0;
	unsigned char* SubsBlockPara;
	unsigned char* Ptr;
	unsigned char* point;
	unsigned char i;
	unsigned char buff[2];
	point=buff;
//	SubsBlockPara = CommPara[CurvePara->ChannelNo].SubsBlockPara;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	CaptureItems = *(SubsBlockPara+MAC_Get_Profile_Para+8);
	//point = (SubsBlockPara+MAC_Get_Profile_Para+8);
	//CaptureItems = buff[0];
	if( CaptureItems == 0 )
	{
		Get_sigle_curve_data( Dest, CurvePara, LoadRecNo, Length );
	}
	else
	{	
		*Dest = Data_structure;		
		if( LoadRecNo == -1 )
		{
			*(Dest+1) = 0x00;
			*Length += 2;
			return;
		}	
		Value = LoadPtrChk( CurvePara, LoadRecNo );//返回该记录号在flash中的物理地址
		if( Value == -1 )												//所抄数据超限	
		{
			*(Dest+1) = 0x00;
			*Length += 2;
		}
		else
		{	
			if( CaptureItems >= 128 )
			{
				*(Dest+1) = 0x82;
				FillIntegerToBuff( Dest+2, CaptureItems );
				*Length += 4;
				Dest += 4;
			}
			else
			{	
				*(Dest+1) = CaptureItems;
				*Length += 2;
				Dest += 2;
			}
			Ptr = SubsBlockPara+MAC_Get_Profile_Para+8+1;
			for( i=0;i<CaptureItems;i++ )
			{
				RAM_Write( (unsigned char*)&Addr, Ptr, 2 );	
				///RAM_Write( point, Ptr, 2 );	
				///Addr = buff[1] + buff[0]*0xff;
				Len = *(Ptr+2);
				Read_Flash( Dest, Value+Addr, Len );
				Dest += Len;
				*Length += Len;
				Ptr += 3;
			}	
		}
	}	
}	

void Get_sigle_curve_data( unsigned char* Dest, CURVEPARA* CurvePara, long LoadRecNo, unsigned short* Length )	
{
	 long Value;		
	unsigned short CaptureItems;

	CaptureItems = GetCaptureItems( CurvePara->ChannelNo );
	
	*Dest = Data_structure;		
	if( LoadRecNo == -1 )
	{
		*(Dest+1) = 0x00;
		*Length += 2;
		return;
	}	
	Value = LoadPtrChk( CurvePara, LoadRecNo );
	if( Value == -1 )												//所抄数据超限	
	{
		*(Dest+1) = 0x00;
		*Length += 2;
	}
	else
	{	
		if( CaptureItems >= 128 )
		{
			*(Dest+1) = 0x82;
			FillIntegerToBuff( Dest+2, CaptureItems );
			*Length += 4;
			Read_Flash( Dest+4, Value, CurvePara->RecdLenth );
		}
		else
		{	
			*(Dest+1) = CaptureItems;
			*Length += 2;
			Read_Flash( Dest+2, Value, CurvePara->RecdLenth );
		}
		*Length += CurvePara->RecdLenth;
	}
}	

void Get_sigle_readout_data( unsigned char* Dest, short ChannelNo, unsigned short* Length )	
{
	unsigned short CaptureItems = 0;    //sky2014.05
	unsigned short RecdLenth;

	//E2P_RData( (unsigned char*)&CaptureItems, DLMS_ProfileTab[ChannelNo].CapNumAddr, 2 );
	E2P_RData( (unsigned char*)&CaptureItems, DLMS_ProfileTab[ChannelNo].CapNumAddr, 1 );
	E2P_RData( (unsigned char*)&RecdLenth, DLMS_ProfileTab[ChannelNo].RecdLenAddr, 2 );
	
	*Dest = Data_structure;		
	if( CaptureItems >= 128 )
	{
		*(Dest+1) = 0x82;
		FillIntegerToBuff( Dest+2, CaptureItems );
		*Length += 4;
		if( CreatCurveData( Dest+4, ChannelNo ) == -1 ) 
		{
			*(Dest+1) = 0x00;
			*Length -= 2;
			return;			
		}	
	}
	else
	{	
		*(Dest+1) = CaptureItems;
		*Length += 2;
		if( CreatCurveData( Dest+2, ChannelNo ) == -1 ) 
		{
			*(Dest+1) = 0x00;
			return;			
		}	
	}
	*Length += RecdLenth;
}	

/*
函数名:short Get_sigle_capture_objectsPara( unsigned char* Dest, unsigned short Addr, unsigned short CaptureItems, unsigned short* Length )
参数1:unsigned char* Dest,指向发送缓冲区，SendBuff。
参数2:unsigned short Addr,曲线条目存储首地址,诸如General_display_capture_objects1之类。
参数3:unsigned short CaptureItems,要读取的曲线捕获项。
参数4:unsigned short* Length，存储读取数据的长度。
功能:读出曲线捕获项的内容到发送缓冲区SendBuff。
返回值:0
*/
short Get_sigle_capture_objectsPara( unsigned char* Dest, unsigned short Addr, unsigned short CaptureItems, unsigned short* Length )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned short i;
	unsigned char* Point;
	
	Ptr = Buff;
	Point = Dest;
	for( i=0;i<CaptureItems;i++ )
	{
		*Point = Data_structure;		
		*(Point+1) = 0x04;
		E2P_RData( Ptr, Addr, 11 );								
		*(Point+2) = D_Unsigned16;                //class_id: 		long-unsigned   
		RAM_Write( Point+3, Ptr, 2 );                           
		*(Point+5) = D_OctetStr;                  //logical_name: 	octet-string,   
		*(Point+6) = 0x06;                                      
		RAM_Write( Point+7, Ptr+2, 6 );                         
		*(Point+13) = D_Integer8;									//attribute_index: 	integer,        
		RAM_Write( Point+14, Ptr+8, 1 );                        
		*(Point+15) = D_Unsigned16;								//data_index: 		long-unsigned   
		RAM_Write( Point+16, Ptr+9, 2 );
		Addr += 12;
		*Length += 18;
		Point += 18;					
	}	
    return 0;
}

short GetProfile_capture_objectsData( short COM_No, unsigned char* Dest, short ProfItemNo )
{
    unsigned char Buff[4];
    unsigned char* Ptr;
    
	unsigned long* BlockNumber;
	unsigned short Addr;
	unsigned short CaptureItems;
	unsigned short Length;
	
    Ptr = Buff;
	BlockNumber = CommPara[COM_No].BlockNumber;		

	CaptureItems = GetCaptureItems( ProfItemNo );//捕获的项数
	HT_FreeDog();
	if( (CaptureItems*19) <= ( MAX_PDU_SIZE - 20) )  				//捕获对象参数一个PDU能返回完毕，不需要使用块操作
//	if( (CaptureItems*19) <= ( MAX_INFO_SIZE - 20) )  				//捕获对象参数一个PDU能返回完毕，不需要使用块操作

	{
		*(Dest+1) = Data_array;
		*(Dest+2) = 0x82;											//长度字节为2字节
		FillIntegerToBuff( Dest+3, CaptureItems );				
		Length = 5;
		Addr = DLMS_ProfileTab[ProfItemNo].CapObjAddr;//捕获项首地址 Object1 2 3 4....
		Get_sigle_capture_objectsPara( Dest+5, Addr, CaptureItems, &Length );	
		*BlockNumber = 0;
	}
	else															//捕获对象参数一个PDU不能返回完毕，需要使用块操作
	{	
		*Dest = 0;													//last-block=0表示不是最后一帧数据，后续还有数据块。
		*BlockNumber = 1;
		RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
		SwapData( Ptr, 4 );
		RAM_Write( Dest+1, Ptr, 4 );								//block-number 块数，=1为传输的第一块。
		*(Dest+5) = 0x00;											//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
		*(Dest+6) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
	//	*(Dest+7) = 0x01;                               			
	//	*(Dest+8) = 0xBF;											//数据长度为0x1BF=447字节 
		*(Dest+9) = Data_array;										//Data数据类型1,指其后为SEQUENCE OF Data数组
		*(Dest+10) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
	//	*(Dest+11) = 0x00;											//数据长度为0x1BF=447字节 
	//	*(Dest+12) = 0x80;											//共128个数组，对应于128个object_list_element
		FillIntegerToBuff( Dest+11, CaptureItems );
//		Length = 13;
		Length = 4;
		Addr = DLMS_ProfileTab[ProfItemNo].CapObjAddr;
		Get_sigle_capture_objectsPara( Dest+13, Addr, ( MAX_PDU_SIZE - 20)/19, &Length );	
//		Get_sigle_capture_objectsPara( Dest+13, Addr, ( MAX_INFO_SIZE - 20)/19, &Length );	//sky20140508..
		
		FillIntegerToBuff( Dest+7, Length );
		Length += 9;
	}	
	return Length;	
}	


short GetNextProfile_genericClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
    unsigned long LongBuff[8];
//	unsigned long From_entry;
//	unsigned long To_entry;
	unsigned long GetCurRecNo;	
	unsigned long TotalGetRecNumber;
	CURVEPARA* CurvePara;
	//unsigned long* BlockNumber;
  unsigned char* Logical_name;
	unsigned char* SubsBlockPara;
  unsigned short ProfItemNo;
	unsigned short Addr;
	unsigned short CaptureItems;
	unsigned short Length;
	unsigned short Temp;
	unsigned short SendItem;
	unsigned short PDUItem;
  unsigned char Attribute;
	unsigned short  temp_lenth;					//为读BLOCK增加..
		  long i;
	unsigned char *Ptr;	  	
  	
	
	CurvePara = (CURVEPARA*)LongBuff;
	//BlockNumber = CommPara[COM_No].BlockNumber;
	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	Logical_name = SubsBlockPara+MAC_Get_Set_Logical_name;
	Attribute = *(SubsBlockPara+MAC_Get_Set_Attribute);
	
	ProfItemNo = GetProfile_genericClassTabItemNo( Logical_name );
	if( ProfItemNo == DLMS_ProfileSum )
	{
		Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
		return Length;
	}
	switch( Attribute )
	{
		case 2:											//buffer		
			CurvePara->ChannelNo = ProfItemNo;
			GetCurvePara( CurvePara );
//			RAM_Write( (unsigned char*)&From_entry, SubsBlockPara+MAC_Get_Profile_Para, 4 );			//准备返回的最早记录号
//			RAM_Write( (unsigned char*)&To_entry, SubsBlockPara+MAC_Get_Profile_Para+4, 4 );			//准备返回的最迟记录号
			RAM_Write( (unsigned char*)&GetCurRecNo, SubsBlockPara+MAC_Get_Profile_Para, 4 );			//准备返回的记录号
			RAM_Write( (unsigned char*)&TotalGetRecNumber, SubsBlockPara+MAC_Get_Profile_Para+4, 4 );	//返回的总记录数
//			if( *BlockNumber > ( From_entry - To_entry )  )
//			if(( *BlockNumber > TotalGetRecNumber  )||(TotalGetRecNumber==0  )||(GetCurRecNo==0)||(GetCurRecNo==0xFFFFFFFF))			//改GetCurRecNo==0最后一帧定义..
//			if((TotalGetRecNumber==0  )||(GetCurRecNo==0)||(GetCurRecNo==0xFFFFFFFF))			//改GetCurRecNo==0最后一帧定义..
			if((TotalGetRecNumber==0  )||(GetCurRecNo==0xFFFFFFFF))			//改GetCurRecNo==0最后一帧定义..
			{
				Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
				break;
			}
										
			CreatSubObjectListHead( COM_No, SendBuff ); 
//			Value = From_entry+1-*BlockNumber;
			if( *(SubsBlockPara+MAC_Get_Set_Selection_Access_flag) == 0 )
			{
				Length = 0;							
				//Get_sigle_curve_data( SendBuff+9, CurvePara, GetCurRecNo, &Length );
					//填加BLOCK读取多条纪录..
					  temp_lenth = Length;
					  Ptr = SendBuff+9;
						for(i=GetCurRecNo;i>0;i--)
						{
							GetCurRecNo -= 1; //
							Get_sigle_curve_data( Ptr, CurvePara, GetCurRecNo, &Length );							
							temp_lenth = Length - temp_lenth;
							if(temp_lenth<=4){		//结束..
								 i = 0;
								 GetCurRecNo = 0;
								 break;
							}
							Ptr = Ptr + temp_lenth;
							temp_lenth = Length + temp_lenth;
							// MAX_INFO_SIZE ---测试修改sky2014.5.4.
						//	if(temp_lenth>=(MAX_INFO_SIZE-9) ){
							if(temp_lenth>=MAX_PDU_SIZE - 40){
			 						//GetCurRecNo += 1;
			 						if(i>0) i--;			 
								  break;
							}
							temp_lenth = Length;
					//		GetCurRecNo -=1;			
						}
				if(GetCurRecNo==0) GetCurRecNo=0xFFFFFFFF;    //终止标志..
			}
			else if( *(SubsBlockPara+MAC_Get_Selection_Access_selector) == 1 )
			{
					Length = 0;			
//				Get_sigle_curve_range_data( COM_No, SendBuff+9, CurvePara, GetCurRecNo, &Length );				
					//填加BLOCK读取多条纪录..
					  temp_lenth = Length;
					  Ptr = SendBuff+9;
					  
						for(i=0;i<TotalGetRecNumber;i++)
						{							
							Get_sigle_curve_range_data( COM_No, Ptr, CurvePara, GetCurRecNo, &Length );	
							GetCurRecNo -= 1;
							temp_lenth = Length - temp_lenth;
							if(temp_lenth<=4){		//结束..
								 i = TotalGetRecNumber;
								 break;
							}
							Ptr = Ptr + temp_lenth;
							temp_lenth = Length + temp_lenth;
							// MAX_INFO_SIZE ---测试修改sky2014.5.4.
				      //if(temp_lenth>=(MAX_INFO_SIZE-9) ){
						 	if(temp_lenth>=MAX_PDU_SIZE - 40){
							 //GetCurRecNo += 1;
							 i++;
								 break;
							}
							temp_lenth = Length;
					//		GetCurRecNo -=1;			
						}
					TotalGetRecNumber -= i;						
			}	
			else if( *(SubsBlockPara+MAC_Get_Selection_Access_selector) == 2 )
			{
				Length = 0;			
//				Get_sigle_curve_entry_data( COM_No, SendBuff+9, CurvePara, GetCurRecNo, &Length );				
					//填加BLOCK读取多条纪录..
					  temp_lenth = Length;
					  Ptr = SendBuff+9;
						for(i=0;i<TotalGetRecNumber;i++)
						{			
							Get_sigle_curve_entry_data( COM_No, Ptr, CurvePara, GetCurRecNo, &Length );				
							GetCurRecNo -= 1;
							temp_lenth = Length - temp_lenth;
							if(temp_lenth<=4){		//结束..
								 i = TotalGetRecNumber;
								 break;
							}
							Ptr = Ptr + temp_lenth;
							temp_lenth = Length + temp_lenth;
		// MAX_INFO_SIZE ---测试修改sky2014.5.4.
        //if(temp_lenth>=(MAX_INFO_SIZE-9) ){
		if(temp_lenth>=MAX_PDU_SIZE - 40){
			 //GetCurRecNo += 1;
			 i++;
								 break;
							}
							temp_lenth = Length;
					//		GetCurRecNo -=1;			
						}
						TotalGetRecNumber -= i;
			}
		
						
		//	if( Length >= 128 )
			if( Length >= 113 )			  //一帧大小，包括块传输字节数..
			{
				FillIntegerToBuff( SendBuff+7, Length );
				Length += 9;
			}
			else
			{
				*(SendBuff+6) = Length;
				RAM_Write( SendBuff+7, SendBuff+9, Length );
				Length += 7;				
			}		
//			if( Value == To_entry ) *SendBuff = 0x01;													//last-block=1表示是最后一帧数据，后续没有数据块。
//			if(( TotalGetRecNumber == *BlockNumber )||(TotalGetRecNumber ==0)||(GetCurRecNo==0)||(GetCurRecNo==0xFFFFFFFF)) *SendBuff = 0x01;									//last-block=1表示是最后一帧数据，后续没有数据块。
//			if((TotalGetRecNumber ==0)||(GetCurRecNo==0)||(GetCurRecNo==0xFFFFFFFF))
		if((TotalGetRecNumber ==0)||(GetCurRecNo==0xFFFFFFFF))
			{
				 *SendBuff = 0x01;								//last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}

//				if(GetCurRecNo==0) *SendBuff = 0x01;									//last-block=1表示是最后一帧数据，后续没有数据块。
			// GetCurRecNo -= 1;                                                                       	

// 		 if((TotalGetRecNumber ==0)||(GetCurRecNo==0)||(GetCurRecNo==0xFFFFFFFF)) *SendBuff = 0x01;									//last-block=1表示是最后一帧数据，后续没有数据块。

			RAM_Write( SubsBlockPara+MAC_Get_Profile_Para, (unsigned char*)&GetCurRecNo, 4 );			//下一次准备返回的记录号
			RAM_Write( SubsBlockPara+MAC_Get_Profile_Para+4, (unsigned char*)&TotalGetRecNumber, 4 );			//下一次准备返回的记录号			
			break;
		case 3:											//capture_objects
			CaptureItems = GetCaptureItems( ProfItemNo );
			PDUItem = ( MAX_PDU_SIZE - 20)/19;
		//	PDUItem = ( MAX_INFO_SIZE - 20)/19;
						
			//SendItem = PDUItem * (*BlockNumber); 				 			//已发送项数
			SendItem = PDUItem * HComm.BIGBlockNumber[COM_No];
			if( CaptureItems >= SendItem ) CaptureItems -= SendItem;		//剩余项数
			else 
			{
				Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
				break;
			}	
			if( CaptureItems > PDUItem )
			{
				CaptureItems = PDUItem;
				Temp =1;							
			}else Temp =0;
										
			CreatSubObjectListHead( COM_No, SendBuff ); 
//			*(SendBuff+9) = Data_array;										//Data数据类型1,指其后为SEQUENCE OF Data数组
//			*(SendBuff+10) = 0x82;											//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
////			*(SendBuff+11) = 0x00;											//数据长度为0x1BF=447字节 
////			*(SendBuff+12) = 0x80;											//共128个数组，对应于128个object_list_element
//			FillIntegerToBuff( SendBuff+11, CaptureItems );
//			Length = 13;
//			Length = 4;
			Length = 0;
			Addr = DLMS_ProfileTab[ProfItemNo].CapObjAddr;
			Addr += SendItem*12;
//			Get_sigle_capture_objectsPara( SendBuff+13, Addr, CaptureItems, &Length );	
			Get_sigle_capture_objectsPara( SendBuff+9, Addr, CaptureItems, &Length );	
			FillIntegerToBuff( SendBuff+7, Length );
			Length += 9;			
			if( Temp == 0 )
			{
				 *SendBuff = 0x01;								//last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
			break;
		default:
			Length = Get_with_datablock_Abnormity( SendBuff, no_long_get_in_progress );
			break;		
	}	

	return Length;
}		


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

short Judge_capture_objects( unsigned char* Data, short ProfileNo, unsigned short* Length )
{
	unsigned char Buff[12];
	unsigned char* Point;
	unsigned short CapItems;
	short i;
	unsigned char* Ptr;
	unsigned short Temp;
			
	Point = Buff;		
					
	CapItems = DLMS_ProfileTab[ProfileNo].CapNumMax;
					
	if( *Data != Data_array ) return other_reason;
	if(( *(Data+1) & 0x80 ) != 0 )
	{
		if( *(Data+1) == 0x81 )										//设置数据长度是否为1字节 
		{
			Temp = *(Data+2);										//有效设置数据长度
			Ptr = Data+3;
		}			
		else if( *(Data+1) != 0x82 )	return other_reason;
		else 
		{
			Temp = GetBuffToInteger( Data+2 ); 	
			Ptr = Data+4;
		}	
	}
	else 
	{
		Temp = *(Data+1);	
		Ptr = Data+2;		
	}
	if( Temp > CapItems ) return other_reason;			//捕获对象数大于额定限数
	
	*Length = 0;
//	Ptr = Data+2;
//	if(( *(Data+1) & 0x80 ) != 0 ) Ptr += 2;			//长度字节为两字节
	//for( i=0;i<CapItems;i++ )
	for( i=0;i<Temp;i++ )
	{
//		if(( *Ptr != Data_structure )||( *(Ptr+1) != 4 )) return other_reason;
//		if(( *(Ptr+2) != D_Unsigned16 )||( *(Ptr+5) != Data_octet_string )||( *(Ptr+6) != 6 )) return other_reason;
//		if(( *Ptr+13 != D_Integer8 )||( *(Ptr+15) != D_Unsigned16 )) return other_reason; 
//		if( Judge_capture_objects_content( Ptr, Length ) != 0 ) return other_reason; 
		if( Recuce_capture_object_definition( Point, Ptr ) != 0 ) return other_reason;	
		if( Judge_capture_objects_content( Point, Length ) != 0 ) return other_reason; 
		Ptr += 18;		
	}	
    return 0;
}
unsigned short GetDemandRegTabItemNo( unsigned char* Logical_Name )
{
	unsigned short i;				
	
	for( i=0;i<DLMS_DemandRegSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_Demand_regTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	return i;
}	

unsigned char Judge_capture_objects_content( unsigned char* Data, unsigned short* Length )
{
//	unsigned char Buff[6];
	unsigned char* Logical_Name;
	unsigned short Class_id;
	unsigned short Data_Index;
	unsigned char Attribute_Index;
	unsigned char ErrorByte=0;
	unsigned short i;
	unsigned char DataType;
	
//	Class_id =  GetBuffToInteger( Data+3 );
//	Logical_Name = Data+7;
//	Attribute_Index = *(Data+14);
//	Data_Index = GetBuffToInteger( Data+16 );

	Class_id =  GetBuffToInteger( Data );
	Logical_Name = Data+2;
	Attribute_Index = *(Data+8);
	Data_Index = GetBuffToInteger( Data+9 );

	if( Attribute_Index == 0 ) return other_reason;
	//if( Data_Index != 0 ) return other_reason;

	for( i=0;i<CosemParaSum;i++ )
	{
		if( Class_id == Cosem_ParaTab[i].ClassId ) break;
	}	
	if( i == CosemParaSum ) return other_reason;
	
	if( Attribute_Index > Cosem_ParaTab[i].AttributeNum ) return other_reason;
	
	switch( Class_id )
	{
		case Class_id_Data:			
//			i = GetDataClassTabItemNo( Logical_Name );							//查找对应的OBIS码
//			if( i == DLMS_RegSum ) return  other_reason; 
//			if( Attribute_Index != 2 ) return other_reason; 					//只支持属性2
//			*Length += DLMS_DATATab[i].ComInfo & RS_LEN;	
//			*Length += 2;														//加两字节，数据类型和长度
			if( Attribute_Index != 2 ) return other_reason; 					//只支持属性2
			i = GetDataClassTabItemNo( Logical_Name );							//查找对应的OBIS码
			if( i != DLMSDataSum ) 
			{
//				//change skyblue20140925..
//				DataType = DLMS_DATATab[i].DataType;
//				if(DataType==Data_array){				//仅判断电压合格率事件统计使用..
//							*Length += (DLMS_DATATab[i].ComInfo & RS_LEN)*5;
//							*Length += 2;	
//			  }else{	
//							*Length += DLMS_DATATab[i].ComInfo & RS_LEN;	
//							*Length += 1;													//加1字节数据类型				
//							if(( DataType == D_VisibleStr )||( DataType == D_OctetStr )) *Length += 1;	//加1字节数据长度
//				}

				*Length += DLMS_DATATab[i].ComInfo & RS_LEN;	
				*Length += 1;													//加1字节数据类型，enum
				DataType = DLMS_DATATab[i].DataType;
				if(( DataType == D_VisibleStr )||( DataType == D_OctetStr )) *Length += 1;	//加1字节数据长度
			}	
			else
			{
				i = GetRAMDataClassTabItemNo( Logical_Name );							//查找对应的OBIS码
				if( i != DLMS_RAM_DataSum ) 
				{
					*Length += DLMS_RAM_DATATab[i].ComInfo & RS_LEN;	
					*Length += 1;													//加1字节数据类型
					DataType = DLMS_RAM_DATATab[i].DataType;
					if(( DataType == D_VisibleStr )||( DataType == D_OctetStr )) *Length += 1;	//加1字节数据长度
				}
				else return other_reason;
			}					
			break;		
		case Class_id_Register:				
			if( Attribute_Index != 2 ) return other_reason; 					//只支持属性2	

			i = GetECTabItemNo( Logical_Name );				//电量表格			//查找对应的OBIS码
			if( i != DLMS_ECSum )
			{ 
				*Length += DLMS_ECTab[i].ComInfo & RS_LEN;	
				*Length += 1;													//加1字节，数据类型
				return 0; 
			}
		
			i = GetCUMDTabItemNo( Logical_Name );			//累积需量表格		//查找对应的OBIS码
		/*	if( i != DLMS_CUMDSum ) 
			{ 
				*Length += DLMS_CUMDTab[i].ComInfo & RS_LEN;	
				*Length += 1;													//加1字节，数据类型
				return 0; 
			}
		*/
			i = GetREALTabItemNo( Logical_Name );			//实时变量表格		//查找对应的OBIS码
			if( i != DLMS_RealSum ) 
			{ 
				*Length += DLMS_REALTab[i].ComInfo & RS_LEN;	
				*Length += 1;													//加1字节，数据类型
				return 0; 
			}
		
			i = GetRegTabItemNo( Logical_Name );			//实时变量表格		//查找对应的OBIS码
			if( i != DLMS_RegSum ) 
			{ 
				*Length += DLMS_RegTab[i].ComInfo & RS_LEN;	
				if(( DLMS_RegTab[i].DataType == D_VisibleStr )||( DLMS_RegTab[i].DataType == D_OctetStr ))
				{
					*Length += 2;												//加两字节，数据类型和长度	
				}
				else *Length += 1;										//加1字节，数据类型	
				return 0; 
			}
		/*	#if ( IDTHarmonic == YesCheck )							//skyblue20140925..
					i = GetHRTabItemNo( Logical_Name );			//谐波曲线表格		//查找对应的OBIS码
					if( i != DLMS_HRSum ) 
					{ 
						*Length += DLMS_HRTab[i].ComInfo & RS_LEN;	
						*Length += 1;													//加1字节，数据类型
						return 0; 
					}				
			#endif
			*/
			//无显示设置项则退出..
			return other_reason; 	
			break;		
		case Class_id_Extended_register:	
			i = GetMDTabItemNo( Logical_Name );									//查找对应的OBIS码
			if( i == DLMS_MDSum ) return other_reason; 									
			if( Attribute_Index == 2 )	*Length += 5;							//加1字节，数据类型	
			else if( Attribute_Index == 5 )	*Length += 14;						//加两字节，数据类型和长度	
			else ErrorByte = other_reason; 										//只支持属性2、5								
			break;		
		case Class_id_Demand_register:					//5
			i = GetDemandRegTabItemNo( Logical_Name );							//查找对应的OBIS码
			if( i == DLMS_DemandRegSum ) return other_reason; 																	
			switch( Attribute_Index )
			{
//				case 1: 									////logical_name (static) octet-string
//				 	*Length += 8;							//加2字节，数据类型和长度	
//					break;
				case 2: 									//current_average_value 
				case 3: 									//last_average_value 
				case 8: 									//period (static) double-long-unsigned 
				case 9: 									//number_of_periods -----2016-5-20---伊朗给出去的新版本中类型为long-unsigned
					*Length += 5;							//加1字节，数据类型	
					break;	
//				case 4: 									//scaler_unit (static) 
//					*Length += 6;							//加4字节，结构和数据类型等	
//					break;	
				case 5: 									//status (dyn.) 
					*Length += 2;							//加1字节，数据类型	
					break;
				case 6: 									//capture_time (dyn.) octet-string
				case 7: 									//start_time_current (dyn.) octet-string
					*Length += 14;							//加2字节，数据类型和长度	
					break;		
				default:
					ErrorByte = other_reason;
					break;
			}			
			break;
		case Class_id_Clock:	
			switch( Attribute_Index )
			{
//				case 1: 									//logical_name (static) octet-string
//				 	*Length += 8;							//加2字节，数据类型和长度	
//					break;
				case 3: 									//time_zone (static) long
				 	*Length += 3;							//加1字节，数据类型	
				 	break;
				case 4: 									//status (dyn.) unsigned
				 	*Length += 2;							//加1字节，数据类型	
					break;
				case 2: 									//time (dyn.) octet-string
				case 5: 									//daylight_savings_begin (static) octet-string
				case 6: 									//daylight_savings_end (static) octet-string
				 	*Length += 14;							//加2字节，数据类型和长度	
					break;
				case 7: 									//daylight_savings_deviation (static) integer
				case 8: 									//daylight_savings_enabled (static) boolean
				case 9: 									//clock_base (static) enum
				 	*Length += 2;							//加1字节，数据类型	
					break;
				default:
					ErrorByte = other_reason; 				
					break;
			}
			break;
		case Class_id_IEC_HDLC_setup:	
			switch( Attribute_Index )
			{
        case 2:                          
				case 3: 		
				case 4:
 				    *Length += 2;
				    break;                                  
				case 5:
        case 6:                          
				case 7: 		
				case 8:                                 
				case 9:                                  
				    *Length += 3;
				    break;
				default:
				    ErrorByte = other_reason; 
				    break;                    
                        }
			break;		                        
		case Class_id_Limiter:	
			switch( Attribute_Index )
			{
				case 3: 											//threshold_active
				case 4: 											//threshold_normal
				case 5: 											//threshold_emergency
				    *Length += 5;							//加1字节，数据类型	
				    break;
				default:
				    ErrorByte = other_reason; 				
				    break;
			}
    break;
		default:
			ErrorByte = other_reason;						//负荷曲线只支持上述6类。
			break;			
	}	
	
	return ErrorByte;
}	

void InitialProfilePara( unsigned short ItemNo, unsigned short ItemLen )
{
	unsigned short Pages;
	unsigned short Addr;
	unsigned long Value;
	
//	if( ItemNo < 3 ) return;								//显示项不保存长度		//Test
	
	Addr = DLMS_ProfileTab[ItemNo].RecdLenAddr;				//单条记录长度
	E2P_WData( Addr, (unsigned char*)&ItemLen, 2 );
	Pages = DLMS_ProfileTab[ItemNo].EndAddr;					
	if( Pages < 2 ) return;
	Pages -= 1;												//去掉备份页
	Value = 4096/ItemLen;									//每页记录数
	Value *= Pages;												
//	Addr = DLMS_ProfileTab[ItemNo].EntriesMaxAddr;			//总允许有效记录数	
	Addr = DLMS_ProfileTab[ItemNo].EntriesAddr;				//总允许有效记录数
	//总记录条数控制在12000条以内..
	if(Value>12000) Value = 12000;	
	E2P_WData( Addr, (unsigned char*)&Value, 4 );
}	

unsigned char SetProfile_genericClassData( short COM_No, unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char ErrorByte=0;
//	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
//	unsigned char* DataType;
//	unsigned char AccessMode;
	unsigned short Addr;
    unsigned short i,j;
	unsigned short ItemLen;
	unsigned short Item;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
//	DataType = WriteBufAds;
	
	i = GetProfile_genericClassTabItemNo( Logical_Name );
	if( i == DLMS_ProfileSum ) { return object_undefined; }
	
	switch( *Attribute )
	{
//		case 1:														//logical_name			
//			break;	
//		case 2:														//buffer		
//			break;	
		case 3:														//capture_objects		
			ErrorByte = Judge_capture_objects( WriteBufAds, i, &ItemLen );
			if( ErrorByte == 0 )
			{
				if( DLMS_ProfileTab[i].Logical_Name[2] != 21 ) InitialProfilePara( i, ItemLen );	//readout现读，不保存在存储器中
//				InitialProfilePara( i, ItemLen );					//readout现读，不保存在存储器中

				if(( *(WriteBufAds+1) & 0x80 ) != 0 )
				{
					if( *(WriteBufAds+1) == 0x81 )										//设置数据长度是否为1字节 
					{
						Item = *(WriteBufAds+2);										//有效设置数据长度
						Point = WriteBufAds+3;
					}			
					else
					{
						Item = GetBuffToInteger( WriteBufAds+2 );		//长度为2字节
						Point = WriteBufAds+4;
					}	
				}	
				else
				{
					Item = *(WriteBufAds+1);
					Point = WriteBufAds+2;
				}
				
				Addr = DLMS_ProfileTab[i].CapNumAddr;
				//E2P_WData( Addr, (unsigned char *)&Item, 2 );
				if(Item>=254) Item = 1;						
				E2P_WData( Addr, (unsigned char *)&Item, 1 );		
					
				Addr = DLMS_ProfileTab[i].CapObjAddr;
				for( j=0;j<Item;j++ )
				{
					RAM_Write( Ptr, Point+3, 2 );					//class_id: 		long-unsigned,   
					RAM_Write( Ptr+2, Point+7, 6 );					//logical_name: 	octet-string,
					RAM_Write( Ptr+8, Point+14, 1 );				//attribute_index: 	integer,  
					RAM_Write( Ptr+9, Point+16, 2 );				//data_index: 		long-unsigned  
					E2P_WData( Addr, Ptr, 11 );						
					Addr += 12;
					Point += 18;
				}

				// 增加sky2013
    		RAM_Fill( Ptr, 4 );
    		if(( i>= General_display_No )&&( i<= Ins_Energy_Readout_No ))
				{
					//RAM_Fill( Ptr, 4 );
					*Ptr = 1;	
					E2P_WData( DLMS_ProfileTab[i].EntriesUseAddr, Ptr, 4 );												
					E2P_WData( DLMS_ProfileTab[i].EntriesAddr, Ptr, 4 );	
				}//else{
				//			E2P_WData( DLMS_ProfileTab[i].EntriesUseAddr, Ptr, 4 );												
				//			E2P_WData( DLMS_ProfileTab[i].EntriesAddr, Ptr, 4 );	
				//		}
							
				if( i >= Standard_Event_No )
					{
						//RAM_Fill( Ptr, 4 ); 	
						E2P_WData( DLMS_ProfileTab[i].ECurRecNoAddr, Ptr, 4 );	
						E2P_WData( DLMS_ProfileTab[i].PtAddr, Ptr, 4 );		
						E2P_WData( DLMS_ProfileTab[i].EntriesUseAddr, Ptr, 4 );								
					}
					
					if( i == Billing_period1_No )
					{
						E2P_WData( Time_of_recent_billing_period, (unsigned char*)&Clk.YearH, 12 );
						//Value = 0;
						E2P_WData( Billing_period_counter, Ptr, 4 );			
					}								
			//for( i=Standard_Event_No;i<=Load_profile2_No;i++ )
			//{
			//	E2P_WData( DLMS_ProfileTab[i].EntriesUseAddr, Ptr, 4 );								
			//}
			
			//定义第1条记录255的状态..

//				Value = 0;
//				E2P_WData( No_of_config_program, (unsigned char*)&Value, 4 );	
			
                              
			   if(( i>=Standard_Event_No)&&(i<=Power_Quality_Event4_No))
			    {
			    	switch( i )			//..
						{
								case Standard_Event_No:
									SM.StandardEventCode = 0xff;							
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
								case Security_Event_No:
									SM.FraudDetectionCode = 0xff;							
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
								case Disconnector_Control_No:
									SM.DisconnectorControlCode = 0xff;							
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
								///case M_Bus_Event_No:
								///	SM.M_BusCode = 0xff;							
								///	//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
								///	break;																
								case Power_Quality_Event1_No:
									SM.PowerQuality2Code = 0xff;								
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;							
								case Power_Quality_Event2_No:
									SM.PowerQuality2Code = 0xff;		
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
								case Power_Quality_Event3_No:
									SM.PowerQuality3Code = 0xff;		
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
								case Power_Quality_Event4_No:
									SM.PowerQuality4Code = 0xff;
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
							/*	case Power_Quality_Event5_No:
									SM.PowerQuality5Code = 0xff;
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
								case Power_Quality_Event6_No:
									SM.PowerQuality6Code = 0xff;
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
								case Power_Quality_Event7_No:
									SM.PowerQuality7Code = 0xff;
									//SM.Event_all_flag |= quality_event_Tab[eventCode].evnet_State;
									break;
							*/
							default: break;
						}
			    	LoadRecord( i );	
			    }	                                                                        
				SM.Error_register = 0; 
			//	SM.Alarm_register1 = 0; 
				SM.Event_all_flag = 0; 		//南瑞事件状态..
				SM.Error_all_flag = 0;
      }
			//	
			break;
		case 4:														//capture_period		
			if( *WriteBufAds != D_Unsigned32 )
			{
				ErrorByte = other_reason;
				break;	
			}	
			Addr = DLMS_ProfileTab[i].PeriodAddr;
			SwapData( WriteBufAds+1, 4 );
			E2P_WData( Addr, WriteBufAds+1, 4 );
			Getcapture_period();
			break;	
/*		case 7:										//entries_in_use	
			if( *WriteBufAds != D_Unsigned32 )
			{
				ErrorByte = other_reason;
				break;	
			}	
			Addr = DLMS_ProfileTab[i].EntriesUseAddr;
			SwapData( WriteBufAds+1, 4 );
			E2P_WData( Addr, WriteBufAds+1, 4 );
			break;
		case 8:										//profile_entries	
			Addr = DLMS_ProfileTab[i].EntriesAddr;
			if( *WriteBufAds != D_Unsigned32 )
			{
				ErrorByte = other_reason;
				break;	
			}	
			SwapData( WriteBufAds+1, 4 );
			E2P_WData( Addr, WriteBufAds+1, 4 );
			break;
*/			
		default: 
			ErrorByte = read_write_denied;
			break;			
	}
	return ErrorByte;
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ActionProfile_genericClassData( unsigned char *Source , unsigned short DataLen )
{
		  unsigned short i;
			unsigned char Buff[6];
			unsigned char* Ptr;
			unsigned char ErrorByte=0;
			unsigned char* Logical_Name;
			unsigned char Method;
				
			Ptr = Buff;
			
			Logical_Name = Source+2;
			Method = *(Source+8);
			
			i = GetProfile_genericClassTabItemNo( Logical_Name );
			if( i == DLMS_ProfileSum ) { return object_undefined; }
		
			if( Method == 1 )
			{
				RAM_Fill( Ptr, 4 );
				if( *(Logical_Name+2) == 21 ) *Ptr = 1;										//readout永远为1
				E2P_WData( DLMS_ProfileTab[i].EntriesUseAddr, Ptr, 4 );
			}
			else if( Method == 2 )
			{
				LoadRecord( i );															//方法2	
			}
			else ErrorByte = other_reason;		
			
			return ErrorByte;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetCurvePara( CURVEPARA* CurvePara )
{
	unsigned char Buff[4];
	unsigned char* Ptr;

	Ptr = Buff;
	
	CurvePara->StartPageNo = DLMS_ProfileTab[CurvePara->ChannelNo].StartAddr;		//负荷曲线存储空间起始页号
	CurvePara->Pages = DLMS_ProfileTab[CurvePara->ChannelNo].EndAddr;				//负荷曲线存储空间总页数
	E2P_RData( Ptr, DLMS_ProfileTab[CurvePara->ChannelNo].RecdLenAddr, 2 );			//负荷曲线每条记录长度
	CurvePara->RecdLenth = *(Ptr+1)*256+ *Ptr;
	CurvePara->RecsInPage = 4096 / CurvePara->RecdLenth;							//负荷曲线每页记录数
	CurvePara->MaxCunt = CurvePara->Pages * CurvePara->RecsInPage;					//负荷曲线存储空间内最大记录数
	CurvePara->RecdCunt = ( CurvePara->Pages - 1 ) * CurvePara->RecsInPage;			//负荷曲线存储空间内最大有效记录数	//1页作为轮转池
	E2P_RData( (unsigned char*)&CurvePara->AvailRecs, DLMS_ProfileTab[CurvePara->ChannelNo].EntriesUseAddr, 4 );	//已存记录数
	if( CurvePara->AvailRecs > CurvePara->RecdCunt ) CurvePara->AvailRecs = 0;										//已存记录数超过允许记录数，非法，已存记录数清零。表示没有有效记录
	E2P_RData( (unsigned char*)&CurvePara->CurRecNo,DLMS_ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, 4 );
}	

//CurvePara:负荷曲线参数
//type: -1:新产生一条记录，其他：指定记录号
//返回值：指定曲线的指定记录号在FLASH芯片中的物理地址。 -1：该记录不存在
 long LoadPtrChk( CURVEPARA* CurvePara, long Type )
{
unsigned char* ptr;
unsigned char buff[4];
	unsigned long Temp,Temp1;
	unsigned long CurRecNo;			//当前记录号
	ptr=buff;
	HT_FreeDog();

	if( Type == -1 )
	{
		CurvePara->CurRecNo += 1;													//新分配一条记录
		if( CurvePara->CurRecNo >= CurvePara->MaxCunt ) CurvePara->CurRecNo = 0;	//超过总记录数，指向第一条。
		if( CurvePara->AvailRecs == 0 ) CurvePara->CurRecNo = 0;					//该通道第一次产生记录，指向第一条。
																					//可配合用于当目标记录指向页的逻辑零地址时，将当前页先擦除后再写入数据，与写FLASH程序配合。		
		E2P_WData( DLMS_ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, (unsigned char*)&CurvePara->CurRecNo, 4 );//保存当前记录号	
			///E2P_WData( (unsigned short)DLMS_ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, (unsigned char*)&CurvePara->CurRecNo, 4 );//保存当前记录号	
		if( CurvePara->AvailRecs < CurvePara->RecdCunt )							//已存记录数小于抄收限制记录数
		{ 												
			CurvePara->AvailRecs++;													//已存记录数加1后存入EEPROM	
			E2P_WData( DLMS_ProfileTab[CurvePara->ChannelNo].EntriesUseAddr, (unsigned char*)&CurvePara->AvailRecs, 4 );	
			E2P_RData( ptr,DLMS_ProfileTab[CurvePara->ChannelNo].EntriesUseAddr,  4 );	
		}	
		CurRecNo = CurvePara->CurRecNo;			//当前记录号		
	}
	else
	{
//      if( Type >= CurvePara->AvailRecs ) return -1;									//所抄记录超限，返回空。	
//      if( CurvePara->CurRecNo >= Type ) CurRecNo = CurvePara->CurRecNo - Type;		//抄收记录号小于当前记录号，直接从当前记录向前递推找到目标记录
//	  	else CurRecNo = CurvePara->CurRecNo + CurvePara->MaxCunt - Type;				//否则转到存储区末端。
//skyblue201410读取..
     		if( Type >= CurvePara->AvailRecs ) return -1;									//所抄记录超限，返回空。	
      	if( PushSetup.PReadCurRecNo >= Type ) CurRecNo = PushSetup.PReadCurRecNo - Type;		//抄收记录号小于当前记录号，直接从当前记录向前递推找到目标记录
	  		else CurRecNo = PushSetup.PReadCurRecNo + CurvePara->MaxCunt - Type;				//否则转到存储区末端。
	}	
	Temp1 = (CurRecNo % CurvePara->RecsInPage)*CurvePara->RecdLenth;				//目标记录首地址在其所处页内的逻辑地址
	Temp = CurvePara->StartPageNo;													//负荷记录存储区在FLASH芯片内的起始页号	
	Temp += CurRecNo/CurvePara->RecsInPage;											//目标记录在FLASH芯片内的页号
	Temp = Temp * 4096 + Temp1;														//目标记录首地址在FLASH芯片内物理地址

	return Temp;																	//返回当前或指向的记录号
}

void LoadRecord( unsigned short ChannelNo )
{
	unsigned char Buff[MAX_PDU_SIZE];
	unsigned char* Point;
    unsigned long LongBuff[9];
	CURVEPARA* CurvePara;
	unsigned long Value;
	
	Point = Buff;
	
	if(( ChannelNo >= General_display_No )&&( ChannelNo <= Ins_Energy_Readout_No )) return;
	
	CurvePara = (CURVEPARA*)LongBuff;
	HT_FreeDog();
	CurvePara->ChannelNo = ChannelNo;
	GetCurvePara( CurvePara );
	if( CurvePara->Pages == 0 ) return;											//该曲线分配的空间为空
	if( CreatCurveData( Point, ChannelNo ) == -1 ) return;						//捕获项参数非法
	Value = LoadPtrChk( CurvePara, -1 );
//	HT_FreeDog();
  //	PushIncNum( ChannelNo,CurvePara->CurRecNo, 1 );      //推送记录增加1;

	//Flash_Write( Value, Point, CurvePara->RecdLenth );		//带擦除
	DataFlash_Write( Value, Point, CurvePara->RecdLenth );

//	IncSubsLoadPtr( CurvePara );
}
//取Demand_register类数据
short GetDemand_registerClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned short Temp;
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	unsigned short Addr;
	unsigned char i,Len=2;
	unsigned char Attribute;
	unsigned char buff[12];
//	MDBUFF* MDBuffPtr; 
	
	Ptr = Logical_Name;
	RAM_Write( Ptr, Source+2, 6 );
	
	i = GetDemandRegTabItemNo( Ptr );				//查找对应的OBIS码
	if( i == DLMS_DemandRegSum )								
	{
		*Dest = Get_data_access_result;				//异常标志
		*(Dest+1) = object_undefined; 				//无对应的OBIS码
		Len = 2;		
	}
	else 
	{
		*Dest = Get_data;											//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
	//	MDBuffPtr = (MDBUFF*)DLMS_Demand_regTab[i].MDBufAds; 
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:									//current_average_value
			case 3:									//last_average_value
				*(Dest+1) = D_Unsigned32;
			//	if( Attribute == 2 ) Ptr = (unsigned char*)&MDBuffPtr->current_average_value;
			//	else Ptr = (unsigned char*)&MDBuffPtr->last_average_value;
			
				RAM_Write( Dest+2, DLMS_Demand_regTab[i].MDEAds, 4 );
				SwapData( Dest+2, 4 );
				if((i%(Total_Fee+1) != SM.FeeNo)||(i%(Total_Fee+1) != 0))
					RAM_Fill( Dest+2, 4 );
				Len = 6;
				break;	
			case 4:									//scaler_unit
				*(Dest+1) = Data_structure;	
				*(Dest+2) = 0x02;	
				*(Dest+3) = D_Integer8;	
				*(Dest+4) = 0xFF;								//scaler:-1				
				*(Dest+5) = D_ENUM;	
				if( i < 2 ) *(Dest+6) = Unit_W;
				else *(Dest+6) = Unit_var;			//unit: enum
				Len = 7;
				break;	
			case 5:									//status
				*(Dest+1) = D_Unsigned8;	
			//	*(Dest+2) = MDBuffPtr->Status;	
				*(Dest+2) = 0;
				Len = 3;
				break;	
			case 6:									//capture_time
		//	case 7:									//start_time_current
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x0C;
			//	if( Attribute == 6 ) Ptr = MDBuffPtr->capture_time;
			//	else Ptr = MDBuffPtr->start_time_current;
				Ptr = buff;
			RAM_Fill(Ptr,12);
			Temp = 2000+BCD_Byte(*DLMS_Demand_regTab[i].MDTEAds);
			*Ptr  = Temp>>4; 		
			*(Ptr+1) = Temp&0xff;
			*(Ptr+2) = BCD_Byte(*(DLMS_Demand_regTab[i].MDTEAds+1));
			*(Ptr+3) = BCD_Byte(*(DLMS_Demand_regTab[i].MDTEAds+2));
			*(Ptr+4) = 0xff;
			*(Ptr+5) = BCD_Byte(*(DLMS_Demand_regTab[i].MDTEAds+3));
			*(Ptr+6) = BCD_Byte(*(DLMS_Demand_regTab[i].MDTEAds+4));
			*(Ptr+7) = BCD_Byte(*(DLMS_Demand_regTab[i].MDTEAds+5));

				RAM_Write( Dest+3, Ptr, 12 );
				Len = 15;
				break;
			case 8:									//period
				*(Dest+1) = D_Unsigned32;
				Addr = DLMS_Demand_regTab[i].EperiodAds;				
				E2P_RData( Dest+2, Addr, 4 );
				SwapData( Dest+2, 4 );
				Len = 6;
				break;	
			case 9:									//number_of_periods
				*(Dest+1) = D_Unsigned16;
				Addr = DLMS_Demand_regTab[i].Enumber_of_periodsAds;
				E2P_RData( Dest+2, Addr, 2 );
				SwapData( Dest+2, 2 );
				Len = 4;
				break;	
			default: break;			
		}
	}		
	return Len;
}	

short CreatCurveData( unsigned char* Dest, unsigned short ChannelNo )
{
	unsigned char Buff[12];
	unsigned char* Ptr; 
	unsigned char* Point; 
	unsigned short ClassId;
	short Len;
	short i;
	unsigned char CapNum=0;
	unsigned short CapObjAddr;
	unsigned short Length;
	
	Ptr = Buff;
	Point = Dest;
	
	E2P_RData( (unsigned char*)&CapNum, DLMS_ProfileTab[ChannelNo].CapNumAddr, 1 );	
	if( CapNum > DLMS_ProfileTab[ChannelNo].CapNumMax ) return -1;
	Len = 0;
	Length = 0;
	HT_FreeDog();
	CapObjAddr = DLMS_ProfileTab[ChannelNo].CapObjAddr;	
	for( i=0;i<CapNum;i++ )
	{
		E2P_RData( Ptr, CapObjAddr, 11 );	
		ClassId = *(Ptr);
		ClassId *= 256;
		ClassId += *(Ptr+1);
	
		switch( ClassId )
		{
			case Class_id_Data:								//1
				Len = GetDataClassData( 0,Point, Ptr ); 		//通道无所谓，0--随机加,曲线不支持消息1024..
				break;
			case Class_id_Register:							//3
				Len = GetRegisterClassData( Point, Ptr ); 
				break;
			case Class_id_Extended_register:				//4
				Len = GetExtendedRegisterClassData( Point, Ptr ); 
				break;
			case Class_id_Demand_register:					//5
				Len = GetDemand_registerClassData( Point, Ptr ); 
				break;
			case Class_id_Clock:							//8
			   //if((ChannelNo==19)||(ChannelNo==20)){
			/*  
			if((ChannelNo>=Power_Failure_EventL1_No)&&(ChannelNo<=Power_Failure_EventL4_No)){
              *Point = 0;
              *(Point+1) = D_OctetStr;
              *(Point+2) = 0x0C;  
              E2P_RData( Point+3,Power_Failure_Start_temp, 12 );
              //记录开始临时时间位置..					
              Len = 15;
			  }else{
                                       //zzl added 
                              #if 0//( IranRequire == YesCheck )
                                      *Point = Get_data;
                                      *(Point+1) = D_OctetStr;
                                      *(Point+2) = 0x0C;  
                                      RAM_Write( Dest+3, (unsigned char*)&Clk.YearH, 12 );
                                      Len = 15;
                              #else
                                      Len = GetClockClassData( Point, Ptr );  
                              #endif                           
			         // Len = GetClockClassData( Point, Ptr ); 
				}
				*/
			  Len = GetClockClassData( Point, Ptr ); 
				break;
			case Class_id_Limiter:						//71
				Len = GetLimiterClassData( Point, Ptr );
				break; 
			default: 
				Len = 2;
                *(Point+1) = object_undefined;
				break;			
		}
		if(( Len == 2 )&&( *(Point+1) == object_undefined ))	//无对应的OBIS码
		{
			return -1;			
		}
					
		RAM_Write( Point, Point+1, Len );
		Len -= 1;
		Point += Len;
		CapObjAddr += 12;
		Length += Len;
		if( Length > ( MAX_PDU_SIZE - 10 ) ) return -1; 
	}	
   return Len;	
}	


//调整四个通道的后续帧参数
//void IncSubsLoadPtr( CURVEPARA* CurvePara )
//{
//	short i;
//	unsigned char* SubsParaPtr;
//	unsigned long* SubsRecNo;
//	
//	for( i=0;i<4;i++ )							//扫描四个通道的后续帧参数
//	{
//		SubsParaPtr = CommLoadPara[i].SubsPara;
//		SubsRecNo = CommLoadPara[i].FirstLoadNo;
//		if(( *SubsParaPtr < 2 )&&( *(SubsParaPtr+1) == 0 )&&(*(SubsParaPtr+3) == 6)		//后续帧为负荷曲线
////			&&((*(SubsParaPtr+2) == 0 )||(*(SubsParaPtr+2) == CurvePara->ChannelNo )))	//后续帧为抄本曲线或总负荷曲线
//			&&((*(SubsParaPtr+2) == 0 )||(*(SubsParaPtr+2) == (CurvePara->ChannelNo+1))))	//后续帧为抄本曲线或总负荷曲线
//		{
//			if( *(SubsParaPtr+4) != 0 )			//还有有效后续帧要发送
//			{
//				*(SubsRecNo+CurvePara->ChannelNo) += 1;		//上一次记录加一
//				if( *(SubsRecNo+CurvePara->ChannelNo) >= CurvePara->AvailRecs )	//超过有效记录数，最大为有效记录
//					*(SubsRecNo+CurvePara->ChannelNo) = CurvePara->AvailRecs-1;		
//			}		
//		}	
//	}	
//}	

//调整四个通道的后续帧参数
void IncSubsLoadPtr( CURVEPARA* CurvePara )
{
	unsigned long* BlockNumber;
  	unsigned char* Logical_name;
	unsigned char* SubsBlockPara;
//	unsigned long From_entry;
//	unsigned long To_entry;
	unsigned short GetCurRecNo;	
	unsigned short TotalGetRecNumber;
    unsigned short ProfItemNo;
	short i;
  	unsigned char Attribute;
	
//	for( i=0;i<2;i++ )							//扫描2个通道的后续帧参数
	for( i=0;i<4;i++ )							//扫描4个通道的后续帧参数
	{
		BlockNumber = CommPara[i].BlockNumber;
		SubsBlockPara = CommPara[i].SubsBlockPara;
		Logical_name = SubsBlockPara+MAC_Get_Set_Logical_name;
		Attribute = *(SubsBlockPara+MAC_Get_Set_Attribute);
		
		ProfItemNo = GetProfile_genericClassTabItemNo( Logical_name );
		if( ProfItemNo == CurvePara->ChannelNo )
		{
			if( Attribute == 2 )																			//buffer		//正在发送状态
			{
//				RAM_Write( (unsigned char*)&From_entry, SubsBlockPara+MAC_Get_Profile_Para, 4 );			//准备返回的最早记录号
//				RAM_Write( (unsigned char*)&To_entry, SubsBlockPara+MAC_Get_Profile_Para+4, 4 );			//准备返回的最迟记录号
				RAM_Write( (unsigned char*)&GetCurRecNo, SubsBlockPara+MAC_Get_Profile_Para, 4 );			//当前准备返回的记录号
				RAM_Write( (unsigned char*)&TotalGetRecNumber, SubsBlockPara+MAC_Get_Profile_Para+4, 4 );	//返回的总记录数
				if( TotalGetRecNumber >= *BlockNumber )
				{
					GetCurRecNo += 1;
					if( GetCurRecNo >= CurvePara->AvailRecs ) GetCurRecNo = CurvePara->AvailRecs - 1;
					RAM_Write( SubsBlockPara+MAC_Get_Profile_Para, (unsigned char*)&GetCurRecNo, 4 );		//当前准备返回的记录号
				}	
			}
    	}
	}
}


//判断capture_object_definition定义是否合法，若合法将其格式简化
short Recuce_capture_object_definition( unsigned char* Dest, unsigned char* Source )
{
	unsigned char* Ptr;
	
	Ptr = Source;
	
	if(( *Ptr != Data_structure )||( *(Ptr+1) != 4 )) return other_reason;
	if(( *(Ptr+2) != D_Unsigned16 )||( *(Ptr+5) != Data_octet_string )||( *(Ptr+6) != 6 )) return other_reason;
	if(( *Ptr+13 != D_Integer8 )||( *(Ptr+15) != D_Unsigned16 )) return other_reason; 

	RAM_Write( Dest, Ptr+3, 2 );					//class_id: 		long-unsigned,   
	RAM_Write( Dest+2, Ptr+7, 6 );					//logical_name: 	octet-string,
	RAM_Write( Dest+8, Ptr+14, 1 );					//attribute_index: 	integer,  
	RAM_Write( Dest+9, Ptr+16, 2 );					//data_index: 		long-unsigned  

	return 0;
}	

short Judge_restricting_object( unsigned short ChannelNo, unsigned char* Source, unsigned short* StartAddr, unsigned short* DataLen )
{
	unsigned char Buff[12];
	unsigned char Buff2[12];	
	unsigned char* Ptr;
	unsigned char* Point;	
	unsigned short Addr;
	unsigned char CapNum=0; 
	short i;
	
	Ptr = Buff;
	Point = Buff2;
	
	if( Recuce_capture_object_definition( Ptr, Source ) != 0 ) return -1;
	Addr = DLMS_ProfileTab[ChannelNo].CapObjAddr;
	E2P_RData( (unsigned char*)&CapNum, DLMS_ProfileTab[ChannelNo].CapNumAddr, 1 );	
	if( CapNum > DLMS_ProfileTab[ChannelNo].CapNumMax ) return -1;
	*StartAddr = 0;
	for( i=0;i<CapNum;i++ )//与所有的存储记录比较，找到相同的并返回0，否则返回-1
	{
		*DataLen = 0;
		E2P_RData( Point, Addr, 11 );
		if( Judge_capture_objects_content( Point, DataLen ) != 0 ) return -1;
		if( Data_Comp( Ptr, Point, 11 ) == 0 ) break;
		Addr += 12;
		*StartAddr += *DataLen;
	}	
	if( i == CapNum ) return -1;
	else return 0;
}	

short Judge_restricting_Value( unsigned char* Source )
{
	if( *Source != D_OctetStr ) return -1;
	if( *(Source+1) != 12 ) return -1;
	return 0;
}	

//Dest 的第一字节为selected_values
short Judge_range_descriptor( unsigned short ChannelNo, unsigned char* Dest, unsigned char* Source, unsigned short* RestrictAddr )
{
	unsigned short StartAddr;
	unsigned short DataLen;
	unsigned short Items;
	unsigned char* Ptr;
	unsigned short i;
	
	if( *Source != Data_structure ) return -1;
	if( *(Source+1) != 0x04 ) return -1;
	if( Judge_restricting_object( ChannelNo, Source+2, RestrictAddr, &DataLen ) != 0 ) return -1;//OBIS码是否在存储的内容之中
	if( Judge_restricting_Value( Source+20 ) != 0 ) return -1;//第一个时间点
	if( Judge_restricting_Value( Source+34 ) != 0 ) return -1;//第二个时间点
//	if( *(Source+48) != Data_structure ) return -1;
	if( *(Source+48) != Data_array ) return -1;
	if(( *(Source+49) & 0x80 ) != 0 )
	{
		if( *(Source+49) == 0x81 )										//设置数据长度是否为1字节 
		{
			Items = *(Source+50);										//有效设置数据长度
			Ptr = Source+51;
		}			
		else if( *(Source+49) == 0x82 ) 
		{
			Items = GetBuffToInteger( Source+50 );	
			Ptr = Source+52;
		}else return -1;	
	}else 
	{
		Items = *(Source+49);	
		Ptr = Source+50;
	}	
	
	*Dest = Items;
	Dest += 1;
	for( i=0;i<Items;i++ )
	{
		if( Judge_restricting_object( ChannelNo, Ptr, &StartAddr, &DataLen ) != 0 ) return -1;
		RAM_Write( Dest, (unsigned char*)&StartAddr, 2 );//StartAddr表示Ptr在捕获项中的位置，偏移量
		*(Dest+2) = DataLen;//单项的数据长度
		Dest += 3;
		Ptr += 18;
	}	
    return 0;
}	

short Judge_entry_descriptor( unsigned char* Source )
{
	if( *Source != Data_structure ) return -1;
	if( *(Source+1) != 0x04 ) return -1;
	if(( *(Source+2) != D_Unsigned32 )||( *(Source+7) != D_Unsigned32)) return -1;
	if(( *(Source+12) != D_Unsigned16 )||( *(Source+15) != D_Unsigned16)) return -1;
  return 0;
}	

//Dest: 读取据参数保存地址
//CurvePara:负荷曲线参数
//返回值：非零：异常，返回内容空
short GetEntryLoadRecNo( unsigned char* Dest, CURVEPARA* CurvePara, unsigned char* Source )
{
	unsigned long From_entry;
	unsigned long To_entry;
	unsigned long GetCurRecNo;	
	unsigned long TotalGetRecNumber;
	unsigned short CaptureItems;
	unsigned short From_selected_value;
	unsigned short To_selected_value;

	if( CurvePara->AvailRecs == 0 ) return -1;							//没有有效记录,返回空。	

//	From_entry = GetBuffToLong( Source+3 );
//	To_entry = GetBuffToLong( Source+8 );
//	if( From_entry > CurvePara->AvailRecs ) return -1;
//	else From_entry = CurvePara->AvailRecs - From_entry;
//	if( To_entry != 0 )													//等于零表示可能达到的最大记录
//	{
//		if( To_entry > CurvePara->AvailRecs ) To_entry = 0;
//		else To_entry = CurvePara->AvailRecs - To_entry;
//	}	

	From_entry = GetBuffToLong( Source+3 );
	To_entry = GetBuffToLong( Source+8 );
	if( From_entry == 0 ) return -1;									//非法记录号
	if( From_entry > CurvePara->AvailRecs ) return -1;
	else GetCurRecNo = CurvePara->AvailRecs - From_entry;
	if( To_entry != 0 )													//等于零表示可能达到的最大记录
	{
		if( To_entry < From_entry ) return -1;
		if( To_entry > CurvePara->AvailRecs ) TotalGetRecNumber = GetCurRecNo + 1;
		else TotalGetRecNumber = To_entry - From_entry + 1;
	}
	else TotalGetRecNumber = GetCurRecNo + 1;	
	

	CaptureItems = GetCaptureItems( CurvePara->ChannelNo );
	From_selected_value = GetBuffToInteger( Source+13 );
	To_selected_value = GetBuffToInteger( Source+16 );
	if( From_selected_value == 0 ) return -1;							//非法索引号
	if( From_selected_value > CaptureItems ) return -1;					//非法索引号
	if( To_selected_value != 0 )										//等于零表示可能达到的最大记录
	{
		if( From_selected_value > To_selected_value ) return -1;		//非法索引号
		if( To_selected_value > CaptureItems ) To_selected_value = CaptureItems;		
	}
	else To_selected_value = CaptureItems;								//等于零表示最大索引号
	
//	RAM_Write( Dest, (unsigned char*)&From_entry, 4 );
//	RAM_Write( Dest+4, (unsigned char*)&To_entry, 4 );
	RAM_Write( Dest, (unsigned char*)&GetCurRecNo, 4 );
	RAM_Write( Dest+4, (unsigned char*)&TotalGetRecNumber, 4 );
	*(Dest+8) = To_selected_value - From_selected_value + 1;
	RAM_Write( Dest+9, (unsigned char*)&From_selected_value, 2 );
	RAM_Write( Dest+11, (unsigned char*)&To_selected_value, 2 );
	
	return 0;
}	


//short Date_time_Less_than( unsigned char *Dest, unsigned char *Source )
short Date_time_Comp( unsigned char *Dest, unsigned char *Source )
{
	short i;

	for( i=0;i<8;i++ )
	{
		if( i==4 )
		{
			Source += 1;
			Dest += 1;
			continue;
		}	
		if( *Dest == 0xFF ) return 1;
		if( *Dest > *Source ) return 1;
		if( *Dest < *Source ) return -1;
		Source += 1;
		Dest += 1;
	}	
			
	return 0;
}	

//short Date_time_Rather_than( unsigned char *Dest, unsigned char *Source )
//{
//	short i;
//
//	for( i=0;i<8;i++ )
//	{
//		if( i==4 )
//		{
//			Source += 1;
//			Dest += 1;
//			continue;
//		}	
//		if( *Dest == 0xFF ) return -1;
//		if( *Dest > *Source ) return 1;
//		if( *Dest < *Source ) return -1;
//		Source += 1;
//		Dest += 1;
//	}	
//	return 0;
//}	

//Dest: 读取回来数据的目标地址
//STime: 从该时间起date_tinme格式
//ETime: 到该时间结束date_tinme格式
//CurvePara:负荷曲线参数
//StartAddr:搜索值位于该曲线单条记录中的首地址 
//返回值：符合指定时间的记录在曲线中的记录号。 -1：符合该条件的记录不存在
short GetAppointTimeLoadRecNo( unsigned char* Dest, unsigned char* STime, unsigned char* ETime, CURVEPARA* CurvePara, unsigned short StartAddr )
{
	unsigned char Buff[12];
	unsigned char* Time;
	long LoadRecNo;
	unsigned long GetCurRecNo;	
	unsigned long TotalGetRecNumber;

	Time = Buff;
	
	if( Date_time_Comp( STime, ETime ) == 1 ) return 3;//开始>结束时间相同
	if( CurvePara->AvailRecs == 0 ) return -1;							//没有有效记录,返回空。	
	//if( CurvePara->AvailRecs == 0 ) return 1;							//没有有效记录,返回空。	
	
//	if( Date_time_Rather_than( STime, ETime ) == 1 ) return -1;
//	if( Date_time_Comp( STime, ETime ) == 1 ) return 3;
	RAM_Write( Time, &Clk.YearH, 12 );
//	if( Date_time_Rather_than( STime, Time ) == 1 ) return -1;
	if( Date_time_Comp( STime, Time ) == 1 ) return -1;//开始时间大于当前时间，错误
//	if( Date_time_Rather_than( ETime, Time ) == 1 ) 
	if( Date_time_Comp( ETime, Time ) == 1 ) 
	{
		RAM_Write( ETime, Time, 12 );									//结束时间大于当前时间，已当前时间为结束时间	
	}	

	LoadRecNo = GetLoadPWAds( Dest, CurvePara, STime, ETime, StartAddr, 0 );//找到页数
//	if( LoadRecNo == -1 ) return -1;												//没有符合条件记录,返回空。
	if( LoadRecNo == -1 ) return 1;												//没有符合条件记录,返回空。
//	RAM_Write( Dest, (unsigned char*)&LoadRecNo, 4 );
	GetCurRecNo = LoadRecNo;
	LoadRecNo = GetLoadPWAds( Dest, CurvePara, STime, ETime, StartAddr, 1 );
//	if( LoadRecNo == -1 ) return -1;			
	if( LoadRecNo == -1 ) return 1;												//没有符合条件记录,返回空。
//	RAM_Write( Dest+4, (unsigned char*)&LoadRecNo, 4 );
	TotalGetRecNumber = GetCurRecNo - LoadRecNo + 1;
	RAM_Write( Dest, (unsigned char*)&GetCurRecNo, 4 );
	RAM_Write( Dest+4, (unsigned char*)&TotalGetRecNumber, 4 );
	return 0;
}	
	
//Type = 0:查找起始第一个记录  1：查找结束第一个记录	
long GetLoadPWAds( unsigned char* Dest, CURVEPARA* CurvePara, unsigned char* STime, unsigned char* ETime, unsigned short StartAddr, short Type )
{
	long nStart = 0;
	long nMiddle;
	long nFound=-1;
	short Temp;
	long nEnd;
	
	nEnd = CurvePara->AvailRecs-1;
	
	while(nStart<=nEnd)
    {
        nMiddle=(nStart+nEnd)/2;
		Temp = LoadPWJustDay( nMiddle, CurvePara, STime, ETime, StartAddr, Type );
		switch( Temp )
		{
			case 1:  nEnd = nMiddle - 1;
//					 if(( Type == 1 )&&( nFound == -1 ))
					 if( Type == 1 )
					 {
					 	nFound = nMiddle;				 	
					 }						  
					 break;
			case -1: nStart = nMiddle + 1;
					 break;
			case 0:	 
				 	 nFound = nMiddle;
				 	 if( Type == 0 ) nStart = nMiddle+1;					
				 	 else { return nFound; }
					 break;
			default: break;			
		}	
	}	
	return nFound;	
}		

short LoadPWJustDay( long LoadRecNo, CURVEPARA* CurvePara, unsigned char* STime, unsigned char* ETime, unsigned short StartAddr, short Type )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	long LoadAddr; 
	
	Ptr = Buff;
	
	if( LoadRecNo < 0 ) return 1;					//非法

	LoadAddr = LoadPtrChk( CurvePara, LoadRecNo );
	if( LoadAddr != -1 )
	{
//		Read_Flash( Ptr, (unsigned long)LoadAddr+StartAddr, 12 );
	///	Read_Flash( Ptr, (unsigned long)LoadAddr+StartAddr+2, 12 );         //跳过数据类型和长度,读出存储的一条记录的捕获时间
		Read_Flash( Ptr, (unsigned long)LoadAddr+StartAddr+2, 12 );         //跳过数据类型和长度,读出存储的一条记录的捕获时间
//		Ptr += 2;									//跳过数据类型和长度
	}
	else return 1;
	if( Type == 0 )
	{
		if( Date_time_Comp( ETime, Ptr ) == -1 ) return -1;
		if( Date_time_Comp( STime, Ptr ) == 1 ) return 1;
	}
	else
	{
//		if( Date_time_Comp( ETime, Ptr ) == -1 ) return -1;
//		if( Date_time_Comp( ETime, Ptr ) == 1 ) return 1;
		return Date_time_Comp( ETime, Ptr );
	}		
	return 0; 
}		


//获取捕获周期..
//	unsigned long Load_profile1_capture_period_Cnt;		//单位秒
//	unsigned long Load_profile2_capture_period_Cnt;
void Getcapture_period( void )
{
	unsigned char Buff[4];
	unsigned char* Ptr;

	Ptr = Buff;
	E2P_RData( Ptr,Load_profile1_capture_period, 4 );		//15分钟曲线捕获周期..
	RAM_Write( (unsigned char*)&SM.Load_profile1_capture_period_save, Ptr, 4 );			//低位在前..	
	//SM.Load_profile1_capture_period_save = GetBuffToLong(Ptr);	
	if((SM.Load_profile1_capture_period_save<60)||(SM.Load_profile1_capture_period_save>3600)) SM.Load_profile1_capture_period_save = 900;
			
	E2P_RData( Ptr,Load_profile2_capture_period, 4 );		//每天的曲线捕获周期..
	RAM_Write( (unsigned char*)&SM.Load_profile2_capture_period_save, Ptr, 4 );			//低位在前..		
	//SM.Load_profile2_capture_period_save = GetBuffToLong(Ptr);
	if((SM.Load_profile2_capture_period_save<7200)||(SM.Load_profile2_capture_period_save>86400)) SM.Load_profile2_capture_period_save = 86400;	
}		

//放在每秒钟执行里面..	
void Getcapture_period_cnt( void )
{
		SM.Load_profile1_capture_period_Cnt++;
		SM.Load_profile2_capture_period_Cnt++;
		#if ( IDTHarmonic == YesCheck )		
				SM.Harmonic_capture_period_Cnt++;
		#endif
		//显示气表和水表符号..
	/*	if((outmeter.Gasmeter_time!=0)&&(outmeter.Gasmeter_time<7)){
			outmeter.Gasmeter_time--;
		}else outmeter.Gasmeter_time = 0;
		//}else outmeter.Gas_meter = 0;

		if((outmeter.Watermeter_time!=0)&&(outmeter.Watermeter_time<7)){
			outmeter.Watermeter_time--;
		}else outmeter.Watermeter_time = 0;
		//}else outmeter.Watermeter = 0;
	*/
}		

//需要推送内容加1，减一:0x81，其余加1，最大5952..
void PushIncNum( short ChannelNo, unsigned long CurRecNo,unsigned char Inc_Or_Dec )
{
	unsigned short RecordNum,RecordMax;
	//推送部分(记录数加1)..
///	E2P_RData( (unsigned char *)&RecordNum,Push_Setup_Tab[ChannelNo].PushNumAddr, 2 );
	if(RecordNum>5955) RecordNum = 0;
	if(CurRecNo>5952)
	{
		RecordMax = 5952;
	}
	else
	{
		RecordMax = (unsigned short)CurRecNo;
	}
	if(Inc_Or_Dec==0x81)
	{
			if(RecordNum<=0 )  //最低为0..
			{
				RecordNum = 0;
			}
			else
			{
				RecordNum--;
		  }
	}
	else
	{
			if(RecordNum>=RecordMax)  //最大定义96*62..
			{
				RecordNum = RecordMax;
			}
			else
			{
				RecordNum++;
		  }
	}
///	E2P_WData( Push_Setup_Tab[ChannelNo].PushNumAddr, (unsigned char *)&RecordNum,2 );
	//==========================
}
//------------2016-5-20---------
/*
函数名:PDN_Profile_24hours_Chek(unsigned char* Profile_Status)
输入参数:unsigned char* Profile_Status,在此指SM.Profile_Status[x];
输出参数:返回值0表示时间上出错，1表示成功。
函数功能:停电时置profile_status 标志位bit7=1，上电时根据停电时间
				自行决定是否增加一条捕获。24 hours capture time
*/
unsigned char PDN_Profile_24hours_Chek(void)
{
	unsigned char Buff_old[12],Buff_temp[12];
	unsigned char* Ptr_old;
	unsigned short Temp;
	Ptr_old = Buff_old;

	if((Flag.Power & F_PwrUp) != 0 )//上电后执行
		{
		E2P_RData( Ptr_old, PowerOffTime, 6 );	 	//读取下电后保存掉电时间,BCD时间
		//BCD转HEX
	Temp = BCD_Byte( Buff_old[5] );//year
	Temp += 2000;
	Buff_temp[0] = Temp/256;	
	Buff_temp[1] = Temp%256;  
	Buff_temp[2] = BCD_Byte( Buff_old[4] ); //month 
	Buff_temp[3] = BCD_Byte( Buff_old[3] ); //day  
	Buff_temp[4] = BCD_Byte( Buff_old[2] ); //hour 
	Buff_temp[5] = BCD_Byte( Buff_old[1] ); //min   
	Buff_temp[6] = BCD_Byte( Buff_old[0] ); //sec 

		//先比较年月日时，如果其中一项不相等，说明跨越了捕获点，Power down across several capture periods
			if(Buff_temp[0]!=Clk.YearH) SM.poweroff_num[1]=2;
			if(Buff_temp[1]!=Clk.YearL) SM.poweroff_num[1]=2;
			if(Buff_temp[2]!=Clk.Month) SM.poweroff_num[1]=2;
			if(Buff_temp[3]!=Clk.DayH)  SM.poweroff_num[1]=2;
			if(Buff_temp[4]!=Clk.HourH) SM.poweroff_num[1]=2;
			else{				
					SM.poweroff_num[1]=1;
				}
	}
	return 1;		
}
/*
函数名:PDN_Profile_24hours(unsigned char* Profile_Status)
输入参数:unsigned char* Profile_Status,在此指SM.Profile_Status[x];
函数功能:用于捕获曲线，放置于ProcSec()中,上电后第二次捕获时清除bit7
*/
void Load_Profile2(void)
{
	unsigned char Buff[12];
	unsigned char* ptr;

	ptr = Buff;

	LoadRecord( Load_profile2_No );
		if(SM.poweroff_num[1]==2)
			 {
			   SM.capture_num[1]++;
			   if(SM.capture_num[1]==2)//上电后第二次捕获
				   {
				   SM.Profile2_status=0;  //profile status2
				   SM.poweroff_num[1]=0;
				   SM.capture_num[1]=0;
				   }
			   else
			   SM.Profile2_status&=~0x7f; 
			   }
		   else if(SM.poweroff_num[1]==1)
			   {
			   SM.Profile2_status&=~0x7f;
			   SM.poweroff_num[1]=0;
			   }
	 else
	 SM.Profile2_status = 0;
	 //---------清零最大需量及发生时间
	 RAM_Fill( ptr, 12 );

	 E2P_WData( DLMS_MDTab[0].Addr , ptr, 3);					   //写最大需量   1-0:15.6.0.255
	 E2P_WData( DLMS_MDTab[0].MDTAddr , &Clk.Min[0], 5);			//写需量发生时间
	/// E2P_WData( DLMS_MDTab[0].Addr , ptr, 4);					   //写最大需量  1-0:1.54.0.255
	/// E2P_WData( DLMS_MDTab[0].MDTAddr , ptr, 12);				   //写需量发生时间
}

void Load_Profile_24hours(void)
{		
	if((Flag.Power & F_PwrUp) != 0) 	
		{	
			if( SM.Load_profile2_capture_period_save == 86400)
			{
						if(( SM.Load_profile2_capture_period_save == 86400)&&( Clk.Hour[0] == 0 )&&( Clk.Min[0] == 0 ))
						{						
						SM.Load_profile2_capture_period_Cnt = 10;
								Load_Profile2();
						}
			}
			else
			{			
				if(SM.Load_profile2_capture_period_Cnt>=SM.Load_profile2_capture_period_save)			//需要满足整点..
					{
						if(( Clk.Min[0] == 0 )&&(( BCD_Byte(Clk.Hour[0]) % (SM.Load_profile2_capture_period_save/3600) ) == 0 ))
							{
							SM.Load_profile2_capture_period_Cnt = 10;
							Load_Profile2();
							}
					}
			}				
		}
}
//--------end--------2016-5-20----------
//-------------2016-5-10------------------
/*
函数名:PDN_Profile_Status1_Chek(unsigned char* Profile_Status)
输入参数:unsigned char* Profile_Status,在此指SM.Profile_Status[x];
输出参数:返回值0表示时间上出错，1表示成功。
函数功能:停电时置profile_status 标志位bit7=1，上电时根据停电时间
				自行决定是否增加一条捕获。15 mins capture time
*/
unsigned char PDN_Profile_15min_Chek(void)
{
	unsigned char Buff_old[12],Buff_temp[12];
	unsigned char* Ptr_old;
	unsigned short Temp;
	Ptr_old = Buff_old;

	if((Flag.Power & F_PwrUp) != 0 )//上电后执行
		{
		E2P_RData( Ptr_old, PowerOffTime, 6 );	 	//读取下电后保存掉电时间,BCD时间
		//BCD转HEX
	Temp = BCD_Byte( Buff_old[5] );//year
	Temp += 2000;
	Buff_temp[0] = Temp/256;	
	Buff_temp[1] = Temp%256;  
	Buff_temp[2] = BCD_Byte( Buff_old[4] ); //month 
	Buff_temp[3] = BCD_Byte( Buff_old[3] ); //day  
	Buff_temp[4] = BCD_Byte( Buff_old[2] ); //hour 
	Buff_temp[5] = BCD_Byte( Buff_old[1] ); //min   
	Buff_temp[6] = BCD_Byte( Buff_old[0] ); //sec 

		//先比较年月日时，如果其中一项不相等，说明跨越了捕获点，Power down across several capture periods
			if(Buff_temp[0]!=Clk.YearH) SM.poweroff_num[0]=2;
			if(Buff_temp[1]!=Clk.YearL) SM.poweroff_num[0]=2;
			if(Buff_temp[2]!=Clk.Month) SM.poweroff_num[0]=2;
			if(Buff_temp[3]!=Clk.DayH)  SM.poweroff_num[0]=2;
			if(Buff_temp[4]!=Clk.HourH) SM.poweroff_num[0]=2;
			else{				
				if(Buff_temp[5]<15&&Clk.MinH>15)
					SM.poweroff_num[0]=2;
				else if(Buff_temp[5]<30&&Clk.MinH>30)
					SM.poweroff_num[0]=2;
				else if(Buff_temp[5]<45&&Clk.MinH>45)
					SM.poweroff_num[0]=2;
				else
					SM.poweroff_num[0]=1;
				}
	}
	return 1;		
}
void Load_Profile1 (void )
{
		LoadRecord( Load_profile1_No );
		if(SM.poweroff_num[0]==2)
			{
				SM.capture_num[0]++;
				if(SM.capture_num[0]==2)//上电后第二次捕获
					{
					SM.Profile1_status=0;	//profile status1	
					SM.poweroff_num[0]=0;
					SM.capture_num[0]=0;
					}
				else
					{
					SM.Profile1_status&=~0x7f; 
					}
			}
		else if(SM.poweroff_num[0]==1)
			{
				SM.Profile1_status&=~0x7f;	
				SM.poweroff_num[0]=0;
			}
		else
		{
			SM.Profile1_status = 0;
		}
}

/*
函数名:PDN_Profile_Status1(unsigned char* Profile_Status)
输入参数:unsigned char* Profile_Status,在此指SM.Profile_Status[x];
函数功能:用于捕获曲线，放置于ProcSec()中,上电后第二次捕获时清除bit7
*/
void Load_Profile_15min(void)
{		
	if((Flag.Power & F_PwrUp) != 0) 	
		{	
	if((SM.Load_profile1_capture_period_Cnt>=SM.Load_profile1_capture_period_save)||((SM.Load_profile1_capture_period_save==900)&&(SM.Load_profile1_capture_period_Cnt>10)))			//分钟..
			{		
				if(( BCD_Byte(Clk.Min[0]) % (SM.Load_profile1_capture_period_save/60) ) == 0 )
				{					
				SM.Load_profile1_capture_period_Cnt = 3;
					Load_Profile1 ();
					HT_FreeDog();
				///	Clr_EC();	//清电量
				}
			}	
		}
}
unsigned long BCDToHex(unsigned char* data)
{
unsigned long  Hex_Data;
unsigned long temp=0;
//BCD转Hex,BCD数据低位在前，高位在后

 Hex_Data = BCD_Byte(*data);
  temp =  BCD_Byte(*(data+1));
  Hex_Data += temp*100;
  temp = BCD_Byte(*(data+2));
  Hex_Data += temp*10000;
  
 return Hex_Data;
}

unsigned char BCDToAscii(unsigned char data)
{
unsigned char  ASCII_Data;
//BCD转ACCII
 ASCII_Data=data & 0x0F;
 if(ASCII_Data<10) 
  ASCII_Data=ASCII_Data+0x30; //‘0--9’
 else  
  ASCII_Data=ASCII_Data+0x41-0x0A;       //‘A--F’
  
 return ASCII_Data;
}
void HexToASCII(unsigned char*  data_hex, unsigned char*  data_ascii)
{ 
 unsigned char L_Bit,H_Bit,HH_Bit;
 unsigned char i; 
 unsigned short temp_data;

 //十六进制转BCD
	 for(i=0; i<5; i++)
	 	{
	 	 if(i==4)
	 	{
		temp_data=*(data_hex);
		temp_data<<=8;
		temp_data|=*(data_hex+1);
		 *(data_ascii+3)=BCDToAscii(temp_data%10);//个位
		 temp_data/=10;
		 *(data_ascii+2)=BCDToAscii(temp_data%10);//十位
		 temp_data/=10;
		 *(data_ascii+1)=BCDToAscii(temp_data%10);//百位
		 temp_data/=10;
		 *data_ascii=BCDToAscii(temp_data);//千位
		 *(data_ascii-1)=0x3A;
	 	}
		 else
		 	{
	 	if(*data_hex<10)
			{
			*data_ascii=BCDToAscii(*data_hex);
			data_ascii+=2;
	 		}
	else if(*data_hex<100)
	 	{
		 L_Bit=*data_hex%10;
		 H_Bit=*data_hex/10;
		 	
		 //BCD转ACCII
		 *data_ascii=BCDToAscii(H_Bit);
		 *(data_ascii+1)=BCDToAscii(L_Bit);
		 *(data_ascii+2)=0x2E;
			data_ascii+=3;
		}
	 else if(*data_hex<1000)
	 	{
		 L_Bit=*data_hex%10;
		 H_Bit=*data_hex/10;		
		 H_Bit=H_Bit%10;
		 HH_Bit=*data_hex/100;	
		 //BCD转ACCII
		 *data_ascii=BCDToAscii(HH_Bit);
		 *(data_ascii+1)=BCDToAscii(H_Bit);
		 *(data_ascii+2)=BCDToAscii(L_Bit);
		 *(data_ascii+3)=0x2E;
			data_ascii+=4;
	 	}
		 	}
	 data_hex++;
	 
	}
	 
}
void ASCIIToHex(unsigned char*  data_ascii, unsigned char* buff_hex)
{
	unsigned short  hex_Data=0;
	unsigned char i=0,j=0;
while(i==0)
{
	if(*data_ascii<=0x39 && *data_ascii>=0x30)
		{
			hex_Data+=(*data_ascii-0x30);	
			hex_Data*=10;
		}
	else if(*data_ascii==0x2E||*data_ascii==0X3A )
		{
		hex_Data/=10;
			*buff_hex=hex_Data;	
			hex_Data=0;
			 buff_hex++;
			 if(*data_ascii==0X3A)
			 	{
					for(j=1; j<4;j++)
						{
						
						if(*(data_ascii+j)<=0x39 && *(data_ascii+j)>=0x30)
							{
							hex_Data+=(*(data_ascii+j)-0x30);
							hex_Data*=10;
							}
						}
					if(*(data_ascii+4)<=0x39 && *(data_ascii+4)>=0x30)
					hex_Data+=(*(data_ascii+4)-0x30);
					*buff_hex=hex_Data>>8;
					*(buff_hex+1)=hex_Data&0XFF;
					i=1;
			 	}
		}
	data_ascii++;

	}
}

//----------------end---2016-5-10--------

