#include "TypeMeter.h"
#include "TypeE2p.h"
//#include "msp430x54x.h"
//#include "Measure.h"
//#include "In430.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

//#include "TypeRAM.h"
//#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"

//#include "RsComm.h"
//#include "interrupt.h"
#include "Mem.h"
#include "Time.h"
#include "Power.h"
//#include "Display.h"
//#include "FreezeData.h"
//#include "Load.h"

#include "Initial.h"
//#include "Produce.h"
#include "AssociationLN.h"
#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "ScriptTable.h"

/*
#include "ActCalendar.h"
#include "RegisterMonitor.h"
#include "SingleAction.h"
#include "ProfileGeneric.h"
#include "Demand.h"
#include "DisconnectControl.h"
#include "ScriptTable.h"

//#include "Esamcpu.h"
 */

unsigned char GetDataValueAccessPurview( unsigned char* Logical_Name )
{
	short i;
	unsigned short Temp;
	unsigned char AccessPurview;
	
	i = GetDataClassTabItemNo( Logical_Name );						//查找对应的OBIS码
	if( i == DLMSDataSum ) { return read_only; }
	Temp = ( DLMS_DATATab[i].ComInfo & ( WRITE + READ ));
	switch( Temp)
	{
		case READ: 
			AccessPurview = read_only; 	
			break;
		case WRITE: 
			AccessPurview = write_only; 	
			break;
		case (READ+WRITE): 
			AccessPurview = read_and_write; 	
			break;
		default:
		   	AccessPurview = no_access;
		   	break; 			
	}	 
	return AccessPurview;
}	

unsigned char GetRegisterValueAccessPurview( unsigned char* Logical_Name )
{
	short i;
	unsigned short Temp;
	unsigned char AccessPurview;
	
	i = GetRegTabItemNo( Logical_Name );						//查找对应的OBIS码
	if( i == DLMS_RegSum ) { return read_only; }
	Temp = ( DLMS_RegTab[i].ComInfo & ( WRITE + READ ));
	switch( Temp )
	{
		case READ: 
			AccessPurview = read_only; 	
			break;
		case WRITE: 
			AccessPurview = write_only; 	
			break;
		case (READ+WRITE): 
			AccessPurview = read_and_write; 	
			break;
		default:
		   	AccessPurview = no_access;
		   	break; 			
	}	 
	return AccessPurview;	
}				

unsigned char GetAttributeAccessPurview( short COM_No, unsigned short ClassId, unsigned char* LogicalName, unsigned char Attribute )
{
	unsigned char AccessPurview;
	
	switch( ClassId )
	{
		case Class_id_Data:								//1
			if( Attribute == 2 )
			{
				AccessPurview = GetDataValueAccessPurview( LogicalName );
			}	
			else AccessPurview = read_only;
			break;
		case Class_id_Register:							//3
			if( Attribute == 2 )
			{
				AccessPurview = GetRegisterValueAccessPurview( LogicalName );			
			}	
			else AccessPurview = read_only;
			break;
		case Class_id_Extended_register:				//4
			AccessPurview = read_only;
			break;
		case Class_id_Demand_register:					//5
			if(( Attribute == 0x08 )||( Attribute == 0x09 )) AccessPurview = read_and_write;
			else AccessPurview = read_only;			              
			break;
		case Class_id_Register_activation:				//6
			if( Attribute == 0x01 ) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
		case Class_id_Profile_generic:					//7
			if(( Attribute == 0x01 )||( Attribute == 0x02 )||( Attribute == 0x05 )||( Attribute == 0x06 )||( Attribute == 0x07 )||( Attribute == 0x08 )) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
//		case Class_id_Utility_tables:					//26
//			break;
//		case Class_id_Register_table:					//61
//			break;
//		case Class_id_Status_mapping:					//63
//			break;
//		case Class_id_Association_SN:					//12
//			break;
		case Class_id_Association_LN:					//15
			if(*(CommPara[COM_No].Conformance+3) == 0 )						//无身份认证方式
			{
				if( Attribute == 0x06 ) AccessPurview = no_access;			//access_mode=0(no_access)	              
				else if( Attribute == 0x07 ) AccessPurview = no_access;		//LLS密码
				else AccessPurview = read_only;				
			}
			else
			{	
				if( Attribute == 0x07 ) AccessPurview = write_only;			//LLS密码	
				else AccessPurview = read_only;				
			}	
			break;
		case Class_id_SAP_assignment:					//17
			AccessPurview = SAP_assignmentAttribute_ParaTab[Attribute-1].AccessMode;
			break;
		case Class_id_Clock:							//8
			AccessPurview = ClockClassAttribute_ParaTab[Attribute-1].AccessMode;
			break;
		case Class_id_Script_table:						//9
			if( Attribute == 0x01 ) AccessPurview = read_only;			              
			//else AccessPurview = no_access;
			if( Attribute == 0x02 )
			{
				AccessPurview = GetScript_tableAccessPurview( LogicalName );//2016-6-15-fire
			}	
			break;
//		case Class_id_Schedule:							//10
//			break;
		case Class_id_Special_days_table:				//11
			if( Attribute == 0x01 ) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;			
		case Class_id_Activity_calendar:				//20
			if(( Attribute >= 1 )&&(Attribute <= 5)) AccessPurview = read_only;			              
			//if( Attribute == 0x01 ) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
		case Class_id_Register_monitor:					//21
			if( Attribute == 0x01 ) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
		case Class_id_Single_action_schedule:			//22
			if( Attribute == 0x01 ) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
		case Class_id_Disconnect_control:				//70
			if(( Attribute >= 0x01 )&&( Attribute <= 0x03 )) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
		case Class_id_Limiter:							//71
			if(( Attribute == 0x01 )||( Attribute == 0x03 )||( Attribute == 10 )) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
		case Class_id_Image_transfer:					//18		
			AccessPurview = Image_transfer_ParaTab[Attribute-1].AccessMode;
			break;			
		case Class_id_IEC_local_port_setup:				//19
			AccessPurview = IECLocalPortSetupClassAttribute_ParaTab[Attribute-1].AccessMode;
			break;
		case Class_id_IEC_HDLC_setup:					//23		
			AccessPurview = IECHDLCSetupClassAttribute_ParaTab[Attribute-1].AccessMode;
			break;
		case Class_id_Auto_answer_setup:					//28 -- 2
	//		if(( Attribute == 0x03 )||(Attribute == 0x08 )) AccessPurview = read_and_write;	   
		  if( Attribute == 0x03 )  AccessPurview = read_and_write;	            
			else AccessPurview = read_only;		
			break;
		case Class_id_Auto_connect_setup:					//29 -- 2
			if( Attribute == 0x01 ) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;			
		case Class_id_Push_Setup:              //40
			if( Attribute == 0x01 ) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
				//暂时不考虑其它的只读情况..		
		case Class_id_TCP_UDP_setup:					//41
		case Class_id_IPv4_setup:					//42
		case Class_id_MACaddr_GPRS:					//43						
		case Class_id_GPRS_modem_setup:					//45
		case Class_id_SMTP_setup:
			if( Attribute == 0x01 ) AccessPurview = read_only;			              
			else AccessPurview = read_and_write;
			break;
		case Class_id_Security_setup:				//64类
			if(( Attribute == 0x01 )||(( Attribute == 0x05 ))) AccessPurview = read_only;			   //电表逻辑设备名只读..           
			else AccessPurview = read_and_write;
			break;
		default: 
			AccessPurview = read_only;
			break;			
	}
    return AccessPurview;	
}	

short CreatObjectListElement( short COM_No, unsigned char* Dest, unsigned short ClassId, unsigned char* LogicalName )
{
	unsigned short i;
  	unsigned char 	AttributeNum;
	unsigned char 	MethodNum;
        unsigned char* Ptr;
	short Len;

	for( i=0;i<CosemParaSum;i++ )
	{
		if( Cosem_ParaTab[i].ClassId == ClassId )
		{
			AttributeNum = Cosem_ParaTab[i].AttributeNum;
			MethodNum = Cosem_ParaTab[i].MethodNum;
			break;
		}	
	}	
	if( i == CosemParaSum ) return -1;

													//	Object_list_element
													//	{
													//		Class_id: long_unsigned;
													//		Version: unsigned;
													//		Logical_name:octet-string;
													//		Access_rights:access_right;
													//	}

	*Dest = Data_structure;							//object_list的内容为结构型SEQUENCE OF Data
	*(Dest+1) = 0x04;								//包含4个元素
	*(Dest+2) = D_Unsigned16;						//Data数据类型18(Unsigned16)
	*(Dest+3) = ClassId / 256;
	*(Dest+4) = ClassId % 256;						//Class_id
	*(Dest+5) = D_Unsigned8;						//Data数据类型17 (Unsigned8)	

	if(( ClassId == Class_id_Profile_generic )||( ClassId == Class_id_Association_LN )
		||( ClassId == Class_id_IEC_local_port_setup )||( ClassId == Class_id_IEC_HDLC_setup )||( ClassId == Class_id_Security_setup ))
	{
		*(Dest+6) = 0x01;	   //version = 1
	}	
	else if(( ClassId == Class_id_Auto_answer_setup )||( ClassId == Class_id_Auto_connect_setup ))
	{
		*(Dest+6) = 0x02;	   //version = 2
	}
	else *(Dest+6) = 0x00;							//version
	*(Dest+7) = D_OctetStr;							//Data数据类型9（OCTET STRING）
	*(Dest+8) = 0x06;								   //OCTET STRING的长度为6
	RAM_Write( Dest+9, LogicalName, 6 );

													//	Access_right::=structre                               
													//	{                                                 
													//		Attribute_access: attribute_access_descriptor;
													//		Method_access:method_access_descriptor;       
													//	}                                                 
	*(Dest+15) = Data_structure;					//object_list的内容为结构型SEQUENCE OF Data
	*(Dest+16) = 0x02;								//包含2个元素
//以下为第一块Data型数据Attribute_access
//	*(Dest+17) = Data_structure;					//Data数据类型1(数组),指其后为SEQUENCE OF Data数组
	*(Dest+17) = Data_array;						//Data数据类型1(数组),指其后为SEQUENCE OF Data数组
	*(Dest+18) = AttributeNum;
	
	Ptr = Dest+19;
	Len = 0;
	for( i=1;i<=AttributeNum;i++ )
	{
		*Ptr = Data_structure;						//Data数据类型2（结构）,其后为SEQUENCE OF Data 型数据      
		*(Ptr+1) = 0x03;							//元素为3个                                                  
		*(Ptr+2) = D_Integer8;						//Data数据类型15，Integer8                                  	
		*(Ptr+3) = i;								//attribute_id
		*(Ptr+4) = D_ENUM;							//Data数据类型22，ENUMERATED                                  	
//		*(Ptr+5) = 0x01;							//access_mode=1(read_only)
//		if(( ClassId == Class_id_Association_LN )&&( i == 0x06))              
//		{
//			*(Ptr+5) = 0x00;						//access_mode=0(no_access)	
//		}	
//		if(( ClassId == Class_id_IEC_local_port_setup )&&(i>=7)&&(i<=9))
//		{
//			*(Ptr+5) = 0x02;						//access_mode=2(write_only)	
//		}	
		*(Ptr+5) = GetAttributeAccessPurview( COM_No, ClassId, LogicalName, i );
		if((*(CommPara[COM_No].Conformance+2) & 0x08 ) == 0 )						//不支持SET功能	
		{
			if( *(Ptr+5) == read_and_write ) *(Ptr+5) = read_only;	
			else if( *(Ptr+5) == write_only ) *(Ptr+5) = no_access;				
		}	
		if(( ClassId == Class_id_Profile_generic )&&( i == 2 )&&( *(LogicalName+2) != 21 ))	//buff属性允许选择性访问(readout只有一条数据，不支持选择性访问)
		{
//			*(Ptr+6) = 0x01;							//access_selectors = 1
//			*(Ptr+7) = Data_array;
//			*(Ptr+8) = 0x02;
//			*(Ptr+9) = D_Integer8;
//			*(Ptr+10) = 0x01;
//			*(Ptr+11) = D_Integer8;
//			*(Ptr+12) = 0x02;			
//			Len += 13;
//			Ptr += 13;			

			*(Ptr+6) = Data_array;
			*(Ptr+7) = 0x02;
			*(Ptr+8) = D_Integer8;
			*(Ptr+9) = 0x01;
			*(Ptr+10) = D_Integer8;
			*(Ptr+11) = 0x02;			
			Len += 12;
			Ptr += 12;			
		}			
		else
		{
				*(Ptr+6) = 0x00;							//access_selectors = 0(null_data)                                  	
				Len += 7;
				Ptr += 7;
		}	
	}	
	
//以下为第二块Data型数据Method_access
//	*Ptr = Data_structure;							//Data数据类型1(数组),指其后为SEQUENCE OF Data数组
	*Ptr = Data_array;								//Data数据类型1(数组),指其后为SEQUENCE OF Data数组
	*(Ptr+1) = MethodNum;
	Ptr += 2;
	for( i=1;i<=MethodNum;i++ )
	{
		*Ptr = Data_structure;						//Data数据类型2（结构）,其后为SEQUENCE OF Data 型数据      
		*(Ptr+1) = 0x02;							//元素为2个                                                  
		*(Ptr+2) = D_Integer8;						//Data数据类型15，Integer8                                  	
		*(Ptr+3) = i;								//method_id
//		*(Ptr+4) = D_BOOLEAN;						//Data数据类型3: BOOLEAN        //VERSION = 0                          	
		*(Ptr+4) = D_ENUM;							//Data数据类型22: ENUMERATED    //VERSION = 1                          	                              	
		*(Ptr+5) = 0x00;							//access_mode = FALSE                                  	
		if((*(CommPara[COM_No].Conformance+2) & 0x01 ) != 0 )						//支持ACTION功能
		{
			if((( ClassId == Class_id_Demand_register )&&( i == 1 ))
				||(( ClassId == Class_id_Profile_generic )&&(( i == 1 )||( i == 2 )))
				||(( ClassId == Class_id_Script_table )&&( i == 1 ))
				||(( ClassId == Class_id_Activity_calendar )&&( i == 1 )))
			{
				*(Ptr+5) = 0x01;					//access_mode = access;                                  				
			}
		}	
			
		Len += 6;
		Ptr += 6;
	}	

	Len += 21;
	
	return Len;	
}	
#if 0
short GetFirstObjectListDataGrade0( short COM_No, unsigned char* Dest ) 
{
	unsigned char Buff[10];
	unsigned char* Point;
	unsigned char* Ptr;
	unsigned long* BlockNumber;
//	unsigned char* SubsBlockPara;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
//	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	
//	FillIntegerToBuff( SubsBlockPara, Class_id_Association_LN );	
	
//	*(SubsBlockPara+2) = 0x00;
//	*(SubsBlockPara+3) = 0x00;
//	*(SubsBlockPara+4) = 0x28;
//	*(SubsBlockPara+5) = 0x00;
//	*(SubsBlockPara+6) = 0x00;
//	*(SubsBlockPara+7) = 0xff;
	
	*Dest = 0;										//last-block=0表示不是最后一帧数据，后续还有数据块。
	*BlockNumber = 1;
	RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( Dest+1, Ptr, 4 );					//block-number 块数，=1为传输的第一块。
	*(Dest+5) = 0x00;								//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
	*(Dest+6) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	*(Dest+7) = 0x01;
//	*(Dest+8) = 0xBF;								//数据长度为0x1BF=447字节 
	*(Dest+9) = Data_array;							//Data数据类型1,指其后为SEQUENCE OF Data数组
	*(Dest+10) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	*(Dest+11) = 0x00;								//数据长度为0x1BF=447字节 
//	*(Dest+12) = 0x80;								//共128个数组，对应于128个object_list_element
//	FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_CUMDSum+DLMS_MDSum+DLMS_ECSum+2 );	//258+2
//	FillIntegerToBuff( Dest+11, 14 );	//5        //test
//	FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_CUMDSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+7 );	//258+2
/*
#if ( DisListCUMMDReg== YesCheck)
		#if ( IranRequire == YesCheck )
		    #if ( IDTHarmonic == YesCheck )        
				#if ( VERSIONLISTDIS == YesCheck )
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_HRSum+8+ADD_IMAGENUMBER+DLMS_CUMDSum +DLMSPushADD2Sum );	//258+2
				#else
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_HRSum+4+ADD_IMAGENUMBER+DLMS_CUMDSum+DLMSPushADD2Sum  );	//258+2                
				#endif
		    #else                                
				#if ( VERSIONLISTDIS == YesCheck )
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+8+ADD_IMAGENUMBER+DLMS_CUMDSum+DLMSPushADD2Sum  );	//258+2
				#else
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+4+ADD_IMAGENUMBER+DLMS_CUMDSum+DLMSPushADD2Sum  );	//258+2                
				#endif
		    #endif                                
		#else
		    #if ( IDTHarmonic == YesCheck )        
				#if ( VERSIONLISTDIS == YesCheck )
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_HRSum+7+ADD_IMAGENUMBER+DLMS_CUMDSum+DLMSPushADD2Sum  );	//258+2        	
				#else		
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_HRSum+3+ADD_IMAGENUMBER+DLMS_CUMDSum+DLMSPushADD2Sum  );	//258+2        	
				#endif		
		    #else                                
				#if ( VERSIONLISTDIS == YesCheck )
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+7+ADD_IMAGENUMBER+DLMS_CUMDSum+DLMSPushADD2Sum  );	//258+2        	
				#else		
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+3+ADD_IMAGENUMBER+DLMS_CUMDSum+DLMSPushADD2Sum  );	//258+2        	
				#endif		
		    #endif                                                                
		#endif
#else
		#if ( IranRequire == YesCheck )
		    #if ( IDTHarmonic == YesCheck )        
				#if ( VERSIONLISTDIS == YesCheck )
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_HRSum+8+ADD_IMAGENUMBER+DLMSPushADD2Sum  );	//258+2
				#else
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_HRSum+4+ADD_IMAGENUMBER+DLMSPushADD2Sum  );	//258+2                
				#endif
		    #else                                
				#if ( VERSIONLISTDIS == YesCheck )
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+8+ADD_IMAGENUMBER+DLMSPushADD2Sum  );	//258+2
				#else
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+4+ADD_IMAGENUMBER+DLMSPushADD2Sum  );	//258+2                
				#endif
		    #endif                                
		#else
		    #if ( IDTHarmonic == YesCheck )        
				#if ( VERSIONLISTDIS == YesCheck )
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_HRSum+7+ADD_IMAGENUMBER+DLMSPushADD2Sum  );	//258+2        	
				#else		
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_HRSum+3+ADD_IMAGENUMBER+DLMSPushADD2Sum  );	//258+2        	
				#endif		
		    #else                                
				#if ( VERSIONLISTDIS == YesCheck )
						FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+7+ADD_IMAGENUMBER+DLMSPushADD2Sum  );	//258+2        	
				#else		

				///FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+3+ADD_IMAGENUMBER+DLMSPushADD2Sum  );	//258+2 		
				FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+6  );	//258+2        	
				#endif		
		    #endif                                                                
		#endif
#endif
*/
	//数组个数
	///FillIntegerToBuff( Dest+11, DLMS_RealSum+DLMSDataSum+DLMS_RAM_DataSum+DLMS_ECSum+DLMS_MDSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+3+DLMS_ScriptParaSum+DLMS_PortdataSum+ADD_IMAGENUMBER+DLMS_Reg_ActivationSum+DLMS_RegSum+6  );	//258+2 		
///	FillIntegerToBuff( Dest+11, DLMS_RealSum+DLMSDataSum+DLMS_RAM_DataSum+DLMS_ECSum+DLMS_MDSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+3+DLMS_ScriptParaSum+DLMS_PortdataSum+ADD_IMAGENUMBER+DLMS_Reg_ActivationSum+DLMS_RegSum+DLMSGPRS2829Sum+DLMSPushSum+5);
	FillIntegerToBuff( Dest+11, Public_AssociationSum+3);
	///FillIntegerToBuff( Dest+11, 135+3);

	Length = 4;

	Point = Dest+13;

//以下为第一个object_list_element
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x01;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Clock, Ptr );		//21+7*9 +6*6= 120
	Length += Len;
	Point += Len;

////sky2013..
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//	
//sky2013..

//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
////sky2013..
//        
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;

////sky2013..
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//	
////sky2013..
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x29;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_SAP_assignment, Ptr );		//21+7*2 +6*1 = 41
	Length += Len;
	Point += Len;
	
/*
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
*/	
	///////////////fire-2016-7-2////////////
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
/*	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x02;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x03;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Buff[0] = 0x01;
//	Buff[1] = 0x01;
//	Buff[2] = 0x01;
//	Buff[3] = 0x04;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Demand_register, Ptr );		//21+7*9 +6*2 = 96
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x01;
//	Buff[1] = 0x00;
//	Buff[2] = 0x62;
//	Buff[3] = 0x01;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Profile_generic, Ptr );		//21+7*8 +6*2+6 = 95
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x01;
//	Buff[1] = 0x01;
//	Buff[2] = 0x20;
//	Buff[3] = 0x23;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Register_monitor, Ptr );		//21+7*4 +6*0 = 49
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x0F;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Single_action_schedule, Ptr );		//21+7*4 +6*0 = 49
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x0B;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Special_days_table, Ptr );	//21+7*2 +6*2 = 47
//	Length += Len;
//	Point += Len;


//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x0D;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Activity_calendar, Ptr );		//21+7*10+6*1 = 97
//	Length += Len;
//	Point += Len;

	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=347字节 

	Length += 9;        
	return Length;      //450
}	
#endif	

#if 0
short GetFirstObjectListDataGrade1( short COM_No, unsigned char* Dest ) 
{
	unsigned char Buff[10];
	unsigned char* Point;
	unsigned char* Ptr;
	unsigned long* BlockNumber;
//	unsigned char* SubsBlockPara;
	short Length;
	short Len;
	Ptr = Buff;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
//	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	
//	FillIntegerToBuff( SubsBlockPara, Class_id_Association_LN );	
	
//	*(SubsBlockPara+2) = 0x00;
//	*(SubsBlockPara+3) = 0x00;
//	*(SubsBlockPara+4) = 0x28;
//	*(SubsBlockPara+5) = 0x00;
//	*(SubsBlockPara+6) = 0x00;
//	*(SubsBlockPara+7) = 0xff;
	
	*Dest = 0;										//last-block=0表示不是最后一帧数据，后续还有数据块。
	*BlockNumber = 1;
	RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( Dest+1, Ptr, 4 );					//block-number 块数，=1为传输的第一块。
	*(Dest+5) = 0x00;								//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
	*(Dest+6) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	*(Dest+7) = 0x01;
//	*(Dest+8) = 0xBF;								//数据长度为0x1BF=447字节 
	*(Dest+9) = Data_array;							//Data数据类型1,指其后为SEQUENCE OF Data数组
	*(Dest+10) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	*(Dest+11) = 0x00;								//数据长度为0x1BF=447字节 
//	*(Dest+12) = 0x80;								//共128个数组，对应于128个object_list_element
//	FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_CUMDSum+DLMS_MDSum+DLMS_ECSum+2 );	//258+2
//	FillIntegerToBuff( Dest+11, 14 );	//5        //test
//	FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_CUMDSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+7 );	//258+2

	FillIntegerToBuff( Dest+11, RReading_AssociationSum + 3); //		

///FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+4  );	//258+2        	
	Length = 4;

	Point = Dest+13;

//以下为第一个object_list_element
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x01;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Clock, Ptr );		//21+7*9 +6*6= 120
	Length += Len;
	Point += Len;

////sky2013..
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//	
//sky2013..

//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
////sky2013..
//        
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;

////sky2013..
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//	
////sky2013..
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x29;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_SAP_assignment, Ptr );		//21+7*2 +6*1 = 41
	Length += Len;
	Point += Len;
/*
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
*/	
	///////////////fire-2016-7-2////////////
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x02;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	/*

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x02;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x03;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Buff[0] = 0x01;
//	Buff[1] = 0x01;
//	Buff[2] = 0x01;
//	Buff[3] = 0x04;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Demand_register, Ptr );		//21+7*9 +6*2 = 96
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x01;
//	Buff[1] = 0x00;
//	Buff[2] = 0x62;
//	Buff[3] = 0x01;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Profile_generic, Ptr );		//21+7*8 +6*2+6 = 95
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x01;
//	Buff[1] = 0x01;
//	Buff[2] = 0x20;
//	Buff[3] = 0x23;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Register_monitor, Ptr );		//21+7*4 +6*0 = 49
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x0F;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Single_action_schedule, Ptr );		//21+7*4 +6*0 = 49
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x0B;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Special_days_table, Ptr );	//21+7*2 +6*2 = 47
//	Length += Len;
//	Point += Len;


//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x0D;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Activity_calendar, Ptr );		//21+7*10+6*1 = 97
//	Length += Len;
//	Point += Len;

	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=347字节 

	Length += 9;        
	return Length;      //450
}	
#endif

short GetFirstObjectListDataGrade2( short COM_No, unsigned char* Dest ) 
{
	unsigned char Buff[10];
	unsigned char* Point;
	unsigned char* Ptr;
	unsigned long* BlockNumber;
//	unsigned char* SubsBlockPara;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
//	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	
//	FillIntegerToBuff( SubsBlockPara, Class_id_Association_LN );	
	
//	*(SubsBlockPara+2) = 0x00;
//	*(SubsBlockPara+3) = 0x00;
//	*(SubsBlockPara+4) = 0x28;
//	*(SubsBlockPara+5) = 0x00;
//	*(SubsBlockPara+6) = 0x00;
//	*(SubsBlockPara+7) = 0xff;
	
	*Dest = 0;										//last-block=0表示不是最后一帧数据，后续还有数据块。
	*BlockNumber = 1;
	RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( Dest+1, Ptr, 4 );					//block-number 块数，=1为传输的第一块。
	*(Dest+5) = 0x00;								//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
	*(Dest+6) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	*(Dest+7) = 0x01;
//	*(Dest+8) = 0xBF;								//数据长度为0x1BF=447字节 
	*(Dest+9) = Data_array;							//Data数据类型1,指其后为SEQUENCE OF Data数组
	*(Dest+10) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	*(Dest+11) = 0x00;								//数据长度为0x1BF=447字节 
//	*(Dest+12) = 0x80;								//共128个数组，对应于128个object_list_element
//	FillIntegerToBuff( Dest+11, DLMS_RealSum+DLMSDataSum+DLMS_RAM_DataSum+DLMS_ECSum+DLMS_MDSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+5+DLMS_PortdataSum+DLMS_Reg_ActivationSum+DLMS_ScriptParaSum+DLMS_RegSum+ADD_IMAGENUMBER+DLMSPushSum+3);
	FillIntegerToBuff( Dest+11, 3+DLMS_RealSum+DLMSDataSum+DLMS_RAM_DataSum+DLMS_ECSum+DLMS_ProfileSum+3+2+DLMS_RegSum+ADD_IMAGENUMBER+5+3+DLMS_MDSum+DLMS_Sigle_actSum+DLMS_Reg_monitorSum+2+DLMSPushSum+4+DLMS_ScriptParaSum);

	Length = 4;

	Point = Dest+13;

//以下为第一个object_list_element
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x01;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Clock, Ptr );		//21+7*9 +6*6= 120
	Length += Len;
	Point += Len;

////sky2013..
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//	
//sky2013..

//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;


//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x16;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x01;
	Buff[2] = 0x16;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
	Length += Len;
	Point += Len;
/*	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x29;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_SAP_assignment, Ptr );		//21+7*2 +6*1 = 41
	Length += Len;
	Point += Len;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0e;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Register_activation, Ptr );	//21+7*2 +6*2 = 47
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Special_days_table, Ptr );	//21+7*2 +6*2 = 47
	Length += Len;
	Point += Len;


	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0D;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Activity_calendar, Ptr );		//21+7*10+6*1 = 97
	Length += Len;
	Point += Len;
*/
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=347字节 

	Length += 9;        
	
	return Length;      //450
}	
#if 0
short GetFirstObjectListDataGrade3( short COM_No, unsigned char* Dest ) 
{
	unsigned char Buff[10];
	unsigned char* Point;
	unsigned char* Ptr;
	unsigned long* BlockNumber;
//	unsigned char* SubsBlockPara;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
//	SubsBlockPara = CommPara[COM_No].SubsBlockPara;
	
//	FillIntegerToBuff( SubsBlockPara, Class_id_Association_LN );	
	
//	*(SubsBlockPara+2) = 0x00;
//	*(SubsBlockPara+3) = 0x00;
//	*(SubsBlockPara+4) = 0x28;
//	*(SubsBlockPara+5) = 0x00;
//	*(SubsBlockPara+6) = 0x00;
//	*(SubsBlockPara+7) = 0xff;
	
	*Dest = 0;										//last-block=0表示不是最后一帧数据，后续还有数据块。
	*BlockNumber = 1;
	RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( Dest+1, Ptr, 4 );					//block-number 块数，=1为传输的第一块。
	*(Dest+5) = 0x00;								//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
	*(Dest+6) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	*(Dest+7) = 0x01;
//	*(Dest+8) = 0xBF;								//数据长度为0x1BF=447字节 
	*(Dest+9) = Data_array;							//Data数据类型1,指其后为SEQUENCE OF Data数组
	*(Dest+10) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
//	*(Dest+11) = 0x00;								//数据长度为0x1BF=447字节 
//	*(Dest+12) = 0x80;								//共128个数组，对应于128个object_list_element
//	FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_CUMDSum+DLMS_MDSum+DLMS_ECSum+2 );	//258+2
//	FillIntegerToBuff( Dest+11, 14 );	//5        //test
//	FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_CUMDSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+7 );	//258+2

	FillIntegerToBuff( Dest+11, DLMS_PortdataSum + 3); //		

///FillIntegerToBuff( Dest+11, DLMSDataSum+DLMS_RealSum+DLMS_MDSum+DLMS_ECSum+DLMS_RegSum+DLMS_Reg_monitorSum+DLMS_Sigle_actSum+DLMS_ProfileSum+DLMS_DemandRegSum+DLMS_RAM_DataSum+DLMS_ScriptParaSum+DLMS_PortdataSum+DLMS_Reg_ActivationSum+4  );	//258+2        	
	Length = 4;

	Point = Dest+13;

//以下为第一个object_list_element
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x01;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Clock, Ptr );		//21+7*9 +6*6= 120
	Length += Len;
	Point += Len;

////sky2013..
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//	
//sky2013..

//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
////sky2013..
//        
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;

////sky2013..
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x16;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;
//	
////sky2013..
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x29;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_SAP_assignment, Ptr );		//21+7*2 +6*1 = 41
	Length += Len;
	Point += Len;
/*
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
*/	
	///////////////fire-2016-7-2////////////
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x03;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	/*

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x02;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x03;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Buff[0] = 0x01;
//	Buff[1] = 0x01;
//	Buff[2] = 0x01;
//	Buff[3] = 0x04;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Demand_register, Ptr );		//21+7*9 +6*2 = 96
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x01;
//	Buff[1] = 0x00;
//	Buff[2] = 0x62;
//	Buff[3] = 0x01;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Profile_generic, Ptr );		//21+7*8 +6*2+6 = 95
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x01;
//	Buff[1] = 0x01;
//	Buff[2] = 0x20;
//	Buff[3] = 0x23;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Register_monitor, Ptr );		//21+7*4 +6*0 = 49
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x0F;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Single_action_schedule, Ptr );		//21+7*4 +6*0 = 49
//	Length += Len;
//	Point += Len;

//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x0B;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Special_days_table, Ptr );	//21+7*2 +6*2 = 47
//	Length += Len;
//	Point += Len;


//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x0D;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_Activity_calendar, Ptr );		//21+7*10+6*1 = 97
//	Length += Len;
//	Point += Len;

	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=347字节 

	Length += 9;        
	
	return Length;      //450
}	
#endif
short CreatSubObjectListHead( short COM_No, unsigned char* Dest ) 
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long* BlockNumber;
	
	Ptr = Buff;
	
	BlockNumber = CommPara[COM_No].BlockNumber;	
	
	*Dest = 0;										//last-block=0表示不是最后一帧数据，后续还有数据块。
	*BlockNumber += 1;
	HComm.BIGBlockNumber[COM_No] += 1;
	RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( Dest+1, Ptr, 4 );					//block-number 块数，=1为传输的第一块。
	*(Dest+5) = 0x00;								//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
	*(Dest+6) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
	*(Dest+7) = 0x00;
	*(Dest+8) = 0x00;								//数据长度为0x1BF=447字节 
    
    return 9;
}	

short GetObjectListDataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMSDataSum ) Satrt = DLMSDataSum;
	if( End > DLMSDataSum ) End = DLMSDataSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Data, (unsigned char*)DLMS_DATATab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

short GetObjectListPublicAssociation( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	unsigned short class_id;
	Length = 0;
/*
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > Public_AssociationSum ) Satrt = Public_AssociationSum;
	if( End > Public_AssociationSum ) End = Public_AssociationSum;
	if(End <=9)
		class_id = Class_id_Data;
	else
		class_id = Class_id_Register;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, class_id, (unsigned char*)Public_Association[i].OBIS_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	*/
	return Length;
}	

short GetObjectListReadingAssociation( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	unsigned short class_id;
	Length = 0;
/*
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > RReading_AssociationSum ) Satrt = RReading_AssociationSum;
	if( End > RReading_AssociationSum ) End = RReading_AssociationSum;
if(End <=9)
	class_id = Class_id_Data;
else
	class_id = Class_id_Register;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, class_id, (unsigned char*)RReading_Association[i].OBIS_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	*/
	return Length;
}

short GetObjectListManagerAssociation( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	/*
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > Public_AssociationSum ) Satrt = Public_AssociationSum;
	if( End > Public_AssociationSum ) End = Public_AssociationSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Data, (unsigned char*)Public_Association[i].OBIS_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	*/
	return Length;
}

// 40类Push..
short GetObjectListPushGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{

	unsigned char* Point;
	short Length;
	short Len;
  short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMSPushSum ) Satrt = DLMSPushSum;
	if( End > DLMSPushSum ) End = DLMSPushSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Push_Setup, (unsigned char*)DLMS_Push_Tab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为 XX字节 
	Length += 9;  
	return Length;
	
}	

short GetObjectListRAMDataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
  short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_RAM_DataSum ) Satrt = DLMS_RAM_DataSum;
	if( End > DLMS_RAM_DataSum ) End = DLMS_RAM_DataSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Data, (unsigned char*)DLMS_RAM_DATATab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

short GetObjectListRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_RegSum ) Satrt = DLMS_RegSum;
	if( End > DLMS_RegSum ) End = DLMS_RegSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Register, (unsigned char*)DLMS_RegTab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

short GetObjectListProfile_genericGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_ProfileSum ) Satrt = DLMS_ProfileSum;
	if( End > DLMS_ProfileSum ) End = DLMS_ProfileSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Profile_generic, (unsigned char*)DLMS_ProfileTab[i].Logical_Name );		
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

short GetObjectListDemand_registerGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_DemandRegSum ) Satrt = DLMS_DemandRegSum;
	if( End > DLMS_DemandRegSum ) End = DLMS_DemandRegSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Demand_register, (unsigned char*)DLMS_Demand_regTab[i].Logical_Name );		
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	
//class 21
short GetObjectListRegister_monitorGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{

	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_Reg_monitorSum ) Satrt = DLMS_Reg_monitorSum;
	if( End > DLMS_Reg_monitorSum ) End = DLMS_Reg_monitorSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Register_monitor, (unsigned char*)DLMS_Reg_monitorTab[i].Logical_Name );		
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9; 
	return Length;
	
}	
//class 6
short GetObjectListRegister_ActivationGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{

	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_Reg_ActivationSum ) Satrt = DLMS_Reg_ActivationSum;
	if( End > DLMS_Reg_ActivationSum ) End = DLMS_Reg_ActivationSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Register_activation, (unsigned char*)DLMS_Reg_ActivationTab[i].Logical_Name );		
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;  
	return Length;
	
}	

short GetObjectListSingle_actionGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_Sigle_actSum ) Satrt = DLMS_Sigle_actSum;
	if( End > DLMS_Sigle_actSum ) End = DLMS_Sigle_actSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Single_action_schedule, (unsigned char*)DLMS_Sigle_actTab[i].Logical_Name );		
		Length += Len;
		Point += Len;
	}	

	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	
	
short GetObjectListSpecial_days_tableGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	unsigned char i;
	Ptr = Buff;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 

	Point = Dest+9;

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Special_days_table, Ptr );	//21+7*2 +6*2 = 47
	Length += Len;
	Point += Len;

///Obis illegal 先注释掉 2016-8-23-fire
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0B;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Special_days_table, Ptr );	//21+7*2 +6*2 = 47
	Length += Len;
	Point += Len;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0D;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Activity_calendar, Ptr );		//21+7*10+6*1 = 97
	Length += Len;
	Point += Len;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x60;
	Buff[3] = 0x03;
	Buff[4] = 0x0A;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Disconnect_control, Ptr );	//21+7*4 +6*2 = 61
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x11;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Limiter, Ptr );				//21+7*11+6*0 = 98
	Length += Len;
	Point += Len;

	
//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x19;
//	Buff[3] = 0x00;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_TCP_UDP_setup, Ptr );			//21+7*6+6*0 = 63
//	Length += Len;
//	Point += Len;
//	
//	Buff[0] = 0x00;				
//	Buff[1] = 0x00;
//	Buff[2] = 0x19;
//	Buff[3] = 0x04;
//	Buff[4] = 0x00;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_GPRS_modem_setup, Ptr );		//21+7*4+6*0 = 49
//	Length += Len;
//	Point += Len;

////sky2013..
//	Buff[0] = 0x00;
//	Buff[1] = 0x00;
//	Buff[2] = 0x14;
//	Buff[3] = 0x00;
//	Buff[4] = 0x01;
//	Buff[5] = 0xff;
//	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
//	Length += Len;
//	Point += Len;      

	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

short GetObjectListScript_tableGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_ScriptParaSum ) Satrt = DLMS_ScriptParaSum;
	if( End > DLMS_ScriptParaSum ) End = DLMS_ScriptParaSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Script_table, (unsigned char*)DLMS_ScriptParaTab[i].Logical_Name );		
		Length += Len;
		Point += Len;
	}	

	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

short GetObjectListRealRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_RealSum ) Satrt = DLMS_RealSum;
	if( End > DLMS_RealSum ) End = DLMS_RealSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Register, (unsigned char*)DLMS_REALTab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

#if ( IDTHarmonic == YesCheck )
short GetObjectListHRRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_HRSum ) Satrt = DLMS_HRSum;
	if( End > DLMS_HRSum ) End = DLMS_HRSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Register, (unsigned char*)DLMS_HRTab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	
#endif

short GetObjectListECRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_ECSum ) Satrt = DLMS_ECSum;
	if( End > DLMS_ECSum ) End = DLMS_ECSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Register, (unsigned char*)DLMS_ECTab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

short GetObjectListCUMMDRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{

	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_CUMDSum ) Satrt = DLMS_CUMDSum;
	if( End > DLMS_CUMDSum ) End = DLMS_CUMDSum;
	
	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Register, (unsigned char*)DLMS_CUMDTab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;  
	return Length;
	
}	

short GetObjectListMDExtRegGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char* Point;
	short Length;
	short Len;
    short i;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	
	if( Satrt > DLMS_MDSum ) Satrt = DLMS_MDSum;
	if( End > DLMS_MDSum ) End = DLMS_MDSum;

	Point = Dest+9;
//以下为第一个object_list_element
	for( i=Satrt;i<End;i++ )
	{
		Len = CreatObjectListElement( COM_No, Point, Class_id_Extended_register, (unsigned char*)DLMS_MDTab[i].Logical_Name );		//21+7*9 +6*6= 120
		Length += Len;
		Point += Len;
	}	
	FillIntegerToBuff( Dest+7, Length );										//数据长度为0x1BF=447字节 
	Length += 9;
	return Length;
}	

//取SAP_assignment类数据
short GetAssociation_LNClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[30];
	unsigned char* Ptr;
	unsigned short Len=2;
	unsigned char Attribute;
	unsigned short DevAddr;
	Ptr = Buff;
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;

	Buff[6] = 0x00;
	Buff[7] = 0x00;
	Buff[8] = 0x28;
	Buff[9] = 0x00;
	Buff[10] = 0x01;
	Buff[11] = 0xff;

	Buff[12] = 0x00;
	Buff[13] = 0x00;
	Buff[14] = 0x28;
	Buff[15] = 0x00;
	Buff[16] = 0x02;
	Buff[17] = 0xff;

	Buff[18] = 0x00;
	Buff[19] = 0x00;
	Buff[20] = 0x28;
	Buff[21] = 0x00;
	Buff[22] = 0x03;
	Buff[23] = 0xff;

	Buff[24] = 0x00;
	Buff[25] = 0x00;
	Buff[26] = 0x28;
	Buff[27] = 0x00;
	Buff[28] = 0x04;
	Buff[29] = 0xff;
	Flag.Asso = 0;
/*
	if( Data_Comp( Ptr, Source+2, 6 ) == 0 ) 
		Flag.Asso = 0;
		else if( Data_Comp( Ptr+6, Source+2, 6 ) == 0 )
			Flag.Asso = 1;
			else if( Data_Comp( Ptr+12, Source+2, 6 ) == 0 ) 
				Flag.Asso = 2;
				else if( Data_Comp( Ptr+18, Source+2, 6 ) == 0 ) 
					Flag.Asso = 3;
				else if( Data_Comp( Ptr+24, Source+2, 6 ) == 0 ) 
					Flag.Asso = 4;
				else
*/					
///	if( Data_Comp( Ptr, Source+2, 6 ) != 0 )
	if(( Data_Comp( Ptr, Source+2, 6 ) != 0 )&&( Data_Comp( Ptr+6, Source+2, 6 ) != 0 )&&( Data_Comp( Ptr+12, Source+2, 6 ) != 0 )&&( Data_Comp( Ptr+18, Source+2, 6 ) != 0 )&&( Data_Comp( Ptr+24, Source+2, 6 ) != 0 ))

	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
		{
		if(HComm.ClientAddr[COM_No]==0x10)
			Flag.Asso = 1;
		else if(HComm.ClientAddr[COM_No]==0x11)
			Flag.Asso = 2;
		else if((HComm.ClientAddr[COM_No]==0x12)||(HComm.ClientAddr[COM_No]==0x01))///0x01是炬华软件发放的地址，在此兼容我们自己的测试软件
			Flag.Asso = 3;
		else if(HComm.ClientAddr[COM_No]==0x13)
			Flag.Asso = 4;
		else
			Flag.Asso = 0;
		}
				
	if( Flag.Asso !=0 )
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
			///	if(Flag.Asso==1)
			///	RAM_Write( Dest+3, Ptr, 6 );
			///	else
					if(Flag.Asso==1)
					RAM_Write( Dest+3, Ptr+6, 6 );
				else if(Flag.Asso==2)
					RAM_Write( Dest+3, Ptr+12, 6 );
				else if(Flag.Asso==3)
					RAM_Write( Dest+3, Ptr+18, 6 );
				Len = 9;
				break;	
			case 2:	
				//object_list
				/*
				if(Flag.Asso==1)
					Len = GetFirstObjectListDataGrade0( COM_No, Dest );
					else if(Flag.Asso==2)					
						///Len = GetFirstObjectListDataGrade1( COM_No, Dest );
						Len = GetFirstObjectListDataGrade1( COM_No, Dest );
					else if(Flag.Asso==3)					
					  ///  Len = GetFirstObjectListDataGrade2( COM_No, Dest );
					  */
					  Len = GetFirstObjectListDataGrade2( COM_No, Dest );
				///	else if(Flag.Asso==4)					
					
				///	Len = GetFirstObjectListDataGrade3( COM_No, Dest );
				
				break;	
			case 3:									//associated_partners_id
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x02;					//structure 内2元素
				*(Dest+3) = D_Integer8;
//				*(Dest+4) = 0x01;					//client_SAP=0x01
//				*(Dest+4) = 0x10;					//client_SAP=0x10
				*(Dest+4) = *(CommPara[COM_No].Buf+HDLC_RClientAddr);
				*(Dest+4) /= 2;
				*(Dest+5) = D_Unsigned16;			//sever_SAP
//				E2P_RData( (unsigned char*)&DevAddr, HDLC_device_address, 2 );	//服务器地址信息SAP地址(3)	0x0001
			//	E2P_RData( (unsigned char*)&DevAddr, ESAP, 2 );										//服务器地址信息SAP地址(3)	0x0001
//				if( DevAddr != 9743 ) DevAddr = 9743;			//Test
//				DevAddr = 0x0001;			//Test
//				RAM_Write( Dest+6, ( unsigned char*)&DevAddr, 2 );
//				SwapData( Dest+6, 2 );
			//	FillIntegerToBuff( Dest+6, DevAddr );
			    *(Dest+6)=0x00;
				*(Dest+7)=0x01;

				Len = 8;
				break;	
			case 4:									//application_context_name
//				FillApplicationContextName( Dest );
				*Dest = Get_data;					//正常数据返回	
				*(Dest+1) = 0x09;
				*(Dest+2) = 0x07;
				*(Dest+3) = 0x60;
				*(Dest+4) = 0x85;
				*(Dest+5) = 0x74;
				*(Dest+6) = 0x05;
				*(Dest+7) = 0x08;
				*(Dest+8) = 0x01;
				*(Dest+9) = 0x01;
				if(Flag.Asso > 1)
					*(Dest+9) = 0x03;
				Len = 10;		
				break;	
			case 5:									//xDLMS_context_info
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x06;					//structure 内6元素
				*(Dest+3) = Data_bit_string;
				*(Dest+4) = 0x18;					//BITSTRING字节长度		0x18=24位
				RAM_Write( Dest+5, CommPara[COM_No].Conformance, 3 );
				*(Dest+8) = D_Unsigned16;
				*(Dest+9) = 0x03;//768size        MAX_PDU_SIZE / 256;		//max_receive_pdu_size
				*(Dest+10) = 0x00;//MAX_PDU_SIZE % 256;
				*(Dest+11) = D_Unsigned16;
				*(Dest+12) = 0x03;//MAX_PDU_SIZE / 256;	//max_send_pdu_size
				*(Dest+13) = 0x00;//MAX_PDU_SIZE % 256;
				*(Dest+14) = D_Unsigned8;
				*(Dest+15) = 0x06;					//dlms_version_number
				*(Dest+16) = D_Integer8;
				*(Dest+17) = 0x00;					//quality_of_service
				*(Dest+18) = D_OctetStr;
				*(Dest+19) = 0x00;					//cyphering_info
				Len = 20;
				break;	
//			case 6:									//authentication_mechanism_name
//			case 7:									//LLS_secret
//				*Dest = Get_data_access_result;		//异常标志
//				*(Dest+1) = scope_of_access_violated; 					
//				Len = 2;		
			case 6:									//authentication_mechanism_name
//				FillApplicationContextName( Dest );
				if( *(CommPara[COM_No].Conformance+3) == 0 )	//无身份认证
				{
					*Dest = Get_data_access_result;		//异常标志
					*(Dest+1) = scope_of_access_violated; 					
					Len = 2;							
				}
				else	
				{
					*Dest = Get_data;					//authentication_mechanism_name
					*(Dest+1) = 0x09;
					*(Dest+2) = 0x07;
					*(Dest+3) = 0x60;
					*(Dest+4) = 0x85;
					*(Dest+5) = 0x74;
					*(Dest+6) = 0x05;
					*(Dest+7) = 0x08;
					*(Dest+8) = 0x01;
					*(Dest+9) = 0x01;
					///GREEN BOOK Page423
					//----按台湾要求----
					if((Flag.Asso == 2)||(Flag.Asso == 4))
						*(Dest+9) = 0x05;
					if(Flag.Asso == 3)
						{
						*(Dest+8) = 0x02;
						*(Dest+9) = 0x05;
						}
					Len = 10;		
				}	
				break;					
			case 7:									//LLS_secret
//				*(Dest+1) = D_OctetStr;
//				*(Dest+2) = 0x00;					//cyphering_info
//				Len = 3;		
				*Dest = Get_data_access_result;		//异常标志
				*(Dest+1) = scope_of_access_violated; 					
				Len = 2;		
				break;
			case 8:									//association_status
				*(Dest+1) = D_ENUM;	
				*(Dest+2) = 0x02;					//连接
				Len = 3;
				break;					
			case 9:									//security_setup_reference	//version = 1
				*(Dest+1) = D_OctetStr;
				//*(Dest+2) = 0x00;					//cyphering_info
				//Len = 3;
				*(Dest+2) = 0x06;
				*(Dest+3) = 0x00;
				*(Dest+4) = 0x00;
				*(Dest+5) = 0x28;
				*(Dest+6) = 0x00;
				*(Dest+7) = 0x02;
				*(Dest+8) = 0xFF;
				Len = 9;			
				break;	
			default: break;			
		}
	}		
	return Len;
}	

unsigned char SetAssociation_LNClassData( short COM_No, unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[30];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
	unsigned char AccessMode;
	unsigned char Select_Flag;
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	if( *(CommPara[COM_No].Conformance+3) == 0 ) 						//无身份认证
	{
		AccessMode = no_access;		
	}	
	else
	{
		AccessMode = write_only;		
	}
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;

	Buff[6] = 0x00;
	Buff[7] = 0x00;
	Buff[8] = 0x28;
	Buff[9] = 0x00;
	Buff[10] = 0x01;
	Buff[11] = 0xff;

	Buff[12] = 0x00;
	Buff[13] = 0x00;
	Buff[14] = 0x28;
	Buff[15] = 0x00;
	Buff[16] = 0x02;
	Buff[17] = 0xff;

	Buff[18] = 0x00;
	Buff[19] = 0x00;
	Buff[20] = 0x28;
	Buff[21] = 0x00;
	Buff[22] = 0x03;
	Buff[23] = 0xff;

	Buff[24] = 0x00;
	Buff[25] = 0x00;
	Buff[26] = 0x28;
	Buff[27] = 0x00;
	Buff[28] = 0x04;
	Buff[29] = 0xff;
	
	
//	if( Data_Comp( Ptr, Logical_Name, 6 ) == 0 ) Select_Flag=1;
	if( Data_Comp( Ptr+6, Logical_Name, 6 ) == 0 ) Select_Flag=1;
	else if( Data_Comp( Ptr+12, Logical_Name, 6 ) == 0 ) Select_Flag=2;	
	else if( Data_Comp( Ptr+18, Logical_Name, 6 ) == 0 ) Select_Flag=3;
	else if( Data_Comp( Ptr+24, Logical_Name, 6 ) == 0 ) Select_Flag=4;
	else if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 )
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	
	if(( AccessMode != write_only )&&( AccessMode != read_and_write ))
	{
		ErrorByte = read_write_denied; 									//无写入权限
	}	 
	else if( *DataType != D_OctetStr )
	{
		ErrorByte = type_unmatched; 									//无写入权限
	}	 
	else
	{
		switch( *Attribute )
		{
			case 7:														//LLS_secret
				if( *(WriteBufAds+1) > 8 ) ErrorByte = other_reason;
				{
					RAM_Fill( Ptr, 10 );	
					RAM_Write( Ptr, WriteBufAds+2, *(WriteBufAds+1) );
				///	E2P_WData( ELLS_secret1, Ptr, 8 );
					
					if(Select_Flag==1)
					///if(HComm.ClientAddr[COM_No]==0x11)
					E2P_WData( ELLS_secret1, Ptr, 8 );
					else if(Select_Flag==2)
					///else if(HComm.ClientAddr[COM_No]==0x12)
					E2P_WData( ELLS_secret2, Ptr, 8 );
					else if(Select_Flag==3)
				///	else if(HComm.ClientAddr[COM_No]==0x12)
					E2P_WData( ELLS_secret3, Ptr, 8 );
					else if(Select_Flag==4)
					E2P_WData( ELLS_secret4, Ptr, 8 );
				
				}	
				break;	
			default:
				ErrorByte = type_unmatched;			
				break;			
		}
	}
	return ErrorByte;
}	
/*
short GetNextAssociation_LNClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned long* BlockNumber;
	short Len = 0;
    short Item;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
	
	switch( *BlockNumber )
	{
//		case 2:
		case 1:
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, 0, 9 );
//			Len = GetObjectListDataGrade0( COM_No, SendBuff, 6, DLMSDataSum );						//Test	
//			*SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。		//Test
			break;	
//		case 3:
		case 2:
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, 9, 18 );
			break;	
//		case 4:
		case 3:
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, 18, 27 );
			break;	
//		case 5:
		case 4:
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, 27, 36 );
			break;	
//		case 6:
		case 5:
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, 36, DLMS_RealSum );
			break;	
//		case 7:
		case 6:
			Len = GetObjectListDataGrade0( COM_No, SendBuff, 0, 6 );
			break;	
//		case 8:
		case 7:
			Len = GetObjectListDataGrade0( COM_No, SendBuff, 6, DLMSDataSum );
			break;	
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
//		case 18:
//			Item = (( *BlockNumber)-9)*10;
			Item = (( *BlockNumber)-8)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, Item+10 );
			break;	
//		case 19:			
		case 18:			
//			Item = (( *BlockNumber)-9)*10;
			Item = (( *BlockNumber)-8)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, DLMS_ECSum );
			break;	
		case 19:
		case 20:
		case 21:
		case 22:
//		case 23:
//		case 24:
//		case 25:
//		case 26:
//		case 27:
//		case 28:

//		case 29:
//			Item = (( *BlockNumber)-20)*10;
			Item = (( *BlockNumber)-19)*10;
			Len = GetObjectListCUMMDRegGrade0( COM_No, SendBuff, Item, Item+10 );
			break;	
		case 23:
		case 24:
		case 25:
		case 26:
//		case 27:
//		case 28:
//		case 29:

//		case 31:
//		case 32:
//		case 33:
//		case 34:
//		case 35:

//		case 36:
//			Item = (( *BlockNumber)-30)*8;
			Item = (( *BlockNumber)-23)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, Item+8 );
			break;
//		case 30:
		case 27:
			Item = (( *BlockNumber)-23)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, DLMS_MDSum );
//			if( *BlockNumber == 28 ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
			break;
		case 28:
		case 29:
		case 30:
			Item = (( *BlockNumber)-28);
			Len = GetObjectListProfile_genericGrade0( COM_No, SendBuff, Item, Item+1 );
			break;	
		case 31:
		case 32:
			Item = (( *BlockNumber)-31)*4;
			Len = GetObjectListDemand_registerGrade0( COM_No, SendBuff, Item, Item+4 );
			break;
		case 33:
		case 34:
		case 35:
//			Item = (( *BlockNumber)-33)*3;
			Item = (( *BlockNumber)-33)*1;
//			Len = GetObjectListRegister_monitorGrade0( COM_No, SendBuff, Item, Item+3 );
			Len = GetObjectListRegister_monitorGrade0( COM_No, SendBuff, Item, Item+1 );
			break;	
		case 36:
			Len = GetObjectListSingle_actionGrade0( COM_No, SendBuff, 0, DLMS_Sigle_actSum );
			break;	
		case 37:
			Len = GetObjectListRegGrade0( COM_No, SendBuff, 0, 10 );
			break;	
		case 38:
			Len = GetObjectListRegGrade0( COM_No, SendBuff, 10, 20 );
			break;	
		case 39:
			Len = GetObjectListRegGrade0( COM_No, SendBuff, 20, 30 );
			break;	
		case 40:
			Len = GetObjectListRegGrade0( COM_No, SendBuff, 30, 40 );
			break;	
		case 41:
			Len = GetObjectListRegGrade0( COM_No, SendBuff, 40, 50 );
			break;	
		case 42:
			Len = GetObjectListRegGrade0( COM_No, SendBuff, 50, DLMS_RegSum );
			if( *BlockNumber == 43 ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
			break;	
		default: break;				
	}	

	return Len;
}	
*/
/*
short GetNextAssociation_LNClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	// unsigned long* BlockNumber;
  unsigned char* BlockNumberN;
	short Len = 0;
  short Item;
	//unsigned short ADD_NUMBER = 1;   // 增加项目数..
	
//	BlockNumber = CommPara[COM_No].BlockNumber;
//	
//	switch( *BlockNumber )

	BlockNumberN = &HComm.BIGBlockNumber[COM_No];
	
	switch( *BlockNumber )	
	// switch( HComm.BIGBlockNumber[COM_No] )
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:						
			Item = (( *BlockNumber)-1)*9;
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, Item, Item+9 );			
			break;	
		case 7:			
			Item = (( *BlockNumber)-1)*9;
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, Item, DLMS_RealSum );
			break;	
		case 8:
		case 9:
		case 10:
			Item = (( *BlockNumber)-8)*12;
			Len = GetObjectListDataGrade0( COM_No, SendBuff, Item, Item+12 );
			break;	
		case 11:
			Item = (( *BlockNumber)-8)*12;
			Len = GetObjectListDataGrade0( COM_No, SendBuff, Item, DLMSDataSum );
			break;	
		case 12:
			Item = (( *BlockNumber)-12)*12;
			Len = GetObjectListRAMDataGrade0( COM_No, SendBuff, Item, Item+12 );
			break;	
		case 13:
			Item = (( *BlockNumber)-12)*12;
			#if ( VERSIONLISTDIS == YesCheck )
					Len = GetObjectListRAMDataGrade0( COM_No, SendBuff, Item, DLMS_RAM_DataSum );
			#else		
					Len = GetObjectListRAMDataGrade0( COM_No, SendBuff, Item, (DLMS_RAM_DataSum-1) );
			#endif					
			break;							
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
			Item = (( *BlockNumber)-14)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, Item+10 );
			break;	
		case 24:			
			Item = (( *BlockNumber)-14)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, DLMS_ECSum );
			break;	
//		case 23:
//		case 24:
//		case 25:
//		case 26:
//			Item = (( *BlockNumber)-23)*10;
//			Len = GetObjectListCUMMDRegGrade0( COM_No, SendBuff, Item, Item+10 );
//			break;	
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
			Item = (( *BlockNumber)-25)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, Item+8 );
			break;
//		case 29:
//			Item = (( *BlockNumber)-25)*8;
//			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, DLMS_MDSum );   //增加超出512..
//			break;
		case 30:
			Item = (( *BlockNumber)-25)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, DLMS_MDSum );   //单独一条进行打包..
			break;				
		case (30+ADD_NUMBER):  //30
		case (31+ADD_NUMBER):  //31:
		case (32+ADD_NUMBER):  //32:
		case (33+ADD_NUMBER):  //33:
		case (34+ADD_NUMBER)://34:
		case (35+ADD_NUMBER)://35:
		case (36+ADD_NUMBER)://36:
			Item = (( *BlockNumber)-(30+ADD_NUMBER))*3;
			Len = GetObjectListProfile_genericGrade0( COM_No, SendBuff, Item, Item+3 );
			break;	
		case (37+ADD_NUMBER)://37:
			Item = (( *BlockNumber)-(30+ADD_NUMBER))*3;
			Len = GetObjectListProfile_genericGrade0( COM_No, SendBuff, Item, DLMS_ProfileSum );
			break;	
		case (38+ADD_NUMBER): //38:
		case (39+ADD_NUMBER): //39:
			Item = (( *BlockNumber)-(38+ADD_NUMBER))*4;
			Len = GetObjectListDemand_registerGrade0( COM_No, SendBuff, Item, Item+4 );
			break;
		case (40+ADD_NUMBER): //40:
			Len = GetObjectListRegister_monitorGrade0( COM_No, SendBuff, 0, DLMS_Reg_monitorSum );
			break;	
		case (41+ADD_NUMBER): //41:
			Len = GetObjectListSingle_actionGrade0( COM_No, SendBuff, 0, DLMS_Sigle_actSum );
			break;	
		case (42+ADD_NUMBER): //42:
			#if( IranRequire == YesCheck )
				Len = GetObjectListSpecial_days_tableGrade0( COM_No, SendBuff, 0, 4 );
			#else
				Len = GetObjectListSpecial_days_tableGrade0( COM_No, SendBuff, 0, 3 );
			#endif			
			break;	
		case (43+ADD_NUMBER): //43:
			Len = GetObjectListScript_tableGrade0( COM_No, SendBuff, 0, DLMS_ScriptParaSum );
			break;	
//增加一条sky2013..
		case (44+ADD_NUMBER): //44:
			Len = GetObjectListPort_dataGrade0( COM_No, SendBuff, 0, DLMS_PortdataSum );
			break;
//单独增加一条升级sky2013..
		case (45+ADD_NUMBER)://45:
			Len = GetObjectListImage_transfer_dataGrade0( COM_No, SendBuff, 0, 2 );				//增加46类..
			break;
		case (46+ADD_NUMBER)://46:
			Len = GetObjectListRegister_ActivationGrade0( COM_No, SendBuff, 0, DLMS_Reg_ActivationSum );				//增加寄存器激活..												
			break;			
		case (47+ADD_NUMBER)://47:
		case (48+ADD_NUMBER)://48:
		case (49+ADD_NUMBER)://49:
		case (50+ADD_NUMBER)://50:
		case (51+ADD_NUMBER)://51:
		case (52+ADD_NUMBER)://52:
			Item = (( *BlockNumber)-(47+ADD_NUMBER))*8;
			Len = GetObjectListRegGrade0( COM_No, SendBuff, Item, Item+8 );
			break;	
		case (53+ADD_NUMBER)://53:
			Item = (( *BlockNumber)-(47+ADD_NUMBER))*8;
			Len = GetObjectListRegGrade0( COM_No, SendBuff, Item, DLMS_RegSum );
//			if( *BlockNumber == 52 ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
#if ( DisListCUMMDReg != YesCheck)
			#if ( IDTHarmonic == YesCheck )
			#else
						 if( *BlockNumber == (54+ADD_NUMBER) ) *SendBuff = 0x01;	//54			//last-block=1表示是最后一帧数据，后续没有数据块。
			#endif
						break;	
			#if ( IDTHarmonic == YesCheck )
					case (54+ADD_NUMBER)://54:
					case (55+ADD_NUMBER)://55:
					case (56+ADD_NUMBER)://56:
					case (57+ADD_NUMBER)://57:
					case (58+ADD_NUMBER)://58:
					case (59+ADD_NUMBER)://59:
					case (60+ADD_NUMBER)://60:
					case (61+ADD_NUMBER)://61:
					case (62+ADD_NUMBER)://62:
					case (63+ADD_NUMBER)://63:
					case (64+ADD_NUMBER)://64:
					case (65+ADD_NUMBER)://65:
					case (66+ADD_NUMBER)://66:
					case (67+ADD_NUMBER)://67:						
						Item = (( *BlockNumber)-(54+ADD_NUMBER))*8;
						Len = GetObjectListHRRegGrade0( COM_No, SendBuff, Item, Item+8 );
						break;	
					case (68+ADD_NUMBER)://68:
						Item = (( *BlockNumber)-(68+ADD_NUMBER))*8;
						Len = GetObjectListHRRegGrade0( COM_No, SendBuff, Item, DLMS_HRSum );
						if( *BlockNumber == (69+ADD_NUMBER) ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
						break;
			#endif				
#else
			break;     //接53上面..
			//增加累计需量LIST抄读..
			case (54+ADD_NUMBER)://54:
			case (55+ADD_NUMBER)://55:
			case (56+ADD_NUMBER)://56:
			Item = (( *BlockNumber)-(54+ADD_NUMBER))*10;
			Len = GetObjectListCUMMDRegGrade0( COM_No, SendBuff, Item, Item+10 );
			break;
			case (57+ADD_NUMBER)://57:
			Item = (( *BlockNumber)-(54+ADD_NUMBER))*10;
			Len = GetObjectListCUMMDRegGrade0( COM_No, SendBuff, Item, Item+10 );
		//	break;
			#if ( IDTHarmonic == YesCheck )
			#else
						 if( *BlockNumber == (58+ADD_NUMBER) ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
			#endif
						break;	
			#if ( IDTHarmonic == YesCheck )
					case (58+ADD_NUMBER)://58:
					case (59+ADD_NUMBER)://59:
					case (60+ADD_NUMBER)://60:
					case (61+ADD_NUMBER)://61:
					case (62+ADD_NUMBER)://62:
					case (63+ADD_NUMBER)://63:
					case (64+ADD_NUMBER)://64:
					case (65+ADD_NUMBER)://65:
					case (66+ADD_NUMBER)://66:
					case (67+ADD_NUMBER)://67:
					case (68+ADD_NUMBER)://68:
					case (69+ADD_NUMBER)://69:
					case (70+ADD_NUMBER)://70:
					case (71+ADD_NUMBER)://71:
					// case (72+ADD_NUMBER)://72:																		
						Item = (( *BlockNumber)-(58+ADD_NUMBER))*8;
						Len = GetObjectListHRRegGrade0( COM_No, SendBuff, Item, Item+8 );
						break;	
					//case (73+ADD_NUMBER)://73:
					case (72+ADD_NUMBER)://72:
						Item = (( *BlockNumber)-(58+ADD_NUMBER))*8;
						Len = GetObjectListHRRegGrade0( COM_No, SendBuff, Item, DLMS_HRSum );
						if( *BlockNumber == (73+ADD_NUMBER) ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
						//if( *BlockNumber == (74+ADD_NUMBER) ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
						break;
			#endif	 
#endif	
		default: break;				
	}	
  		if( *SendBuff == 0x01 )
			{
				 //last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
	return Len;
}	
*/ //sky20140520..

///2016-8-21-fire,
//因为每一类的OBIS码数量有变化，因此要根据每一类的数据量大小适当调整，以发回整个OBIS列表。。
short GetNextAssociation_LN1ClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	 unsigned long* BlockNumberN;
  //unsigned char* BlockNumberN;
	short Len = 0;
  short Item;
  /*
	//unsigned short ADD_NUMBER = 1;   // 增加项目数..
	
//	BlockNumber = CommPara[COM_No].BlockNumber;
//	
//	switch( *BlockNumber )

	// BlockNumberN = &HComm.BIGBlockNumber[COM_No];
		BlockNumberN = CommPara[COM_No].BlockNumber;
		
	switch( *BlockNumberN )	
	// switch( HComm.BIGBlockNumber[COM_No] )
	{
		case 1:
			Item = (( *BlockNumberN)-1)*9;
			Len = GetObjectListPublicAssociation( COM_No, SendBuff, Item, Item+9 );
			break;
		case 2:
			Item = (( *BlockNumberN)-2)*7+9;
			Len = GetObjectListPublicAssociation( COM_No, SendBuff, Item, Item+7 );
			break;
		case 3:	
			Item = (( *BlockNumberN)-2)*7+9;
			Len = GetObjectListPublicAssociation( COM_No, SendBuff, Item, Public_AssociationSum );
			if( *BlockNumberN == 4)  *SendBuff = 0x01;
			break;

		default: break;				
	}	
  		if( *SendBuff == 0x01 )
			{
				 //last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
	*/
	return Len;
}	

short GetNextAssociation_LN2ClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	 unsigned long* BlockNumberN;
  //unsigned char* BlockNumberN;
	short Len = 0;
  short Item;
	//unsigned short ADD_NUMBER = 1;   // 增加项目数..
	
//	BlockNumber = CommPara[COM_No].BlockNumber;
//	
//	switch( *BlockNumber )
/*
	// BlockNumberN = &HComm.BIGBlockNumber[COM_No];
		BlockNumberN = CommPara[COM_No].BlockNumber;
		
	switch( *BlockNumberN )	
	// switch( HComm.BIGBlockNumber[COM_No] )
	{
		case 1:
			Item = (( *BlockNumberN)-1)*9;
			Len = GetObjectListReadingAssociation( COM_No, SendBuff, Item, 9 );
			break;
		case 2:
		case 3:
		case 4:
			Item = (( *BlockNumberN)-2)*7+9;
			Len = GetObjectListReadingAssociation( COM_No, SendBuff, Item, Item+7 );
			break;
		case 5:	
			Item = (( *BlockNumberN)-2)*7+9;
			Len = GetObjectListReadingAssociation( COM_No, SendBuff, Item, RReading_AssociationSum );
			if( *BlockNumberN == 6)  *SendBuff = 0x01;
			break;
		default: break;				
	}	
  		if( *SendBuff == 0x01 )
			{
				 //last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
			*/
	return Len;
}	

short GetNextAssociation_LN4ClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
	{
		 unsigned long* BlockNumberN;
	  //unsigned char* BlockNumberN;
		short Len = 0;
	  short Item;
		//unsigned short ADD_NUMBER = 1;	// 增加项目数..
	/*	
	//	BlockNumber = CommPara[COM_No].BlockNumber;
	//	
	//	switch( *BlockNumber )
	
		// BlockNumberN = &HComm.BIGBlockNumber[COM_No];
			BlockNumberN = CommPara[COM_No].BlockNumber;
			
		switch( *BlockNumberN ) 
		// switch( HComm.BIGBlockNumber[COM_No] )
		{
			case 1:
			case 2:
			case 3:
				Item = (( *BlockNumberN)-1)*12;
				Len = GetObjectListReadingAssociation( COM_No, SendBuff, Item, 28 );
				break;
			case 4:
				Len = GetObjectListReadingAssociation( COM_No, SendBuff, 28, 33 );
				break;
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				Item = (( *BlockNumberN)-5)*7+33;
				Len = GetObjectListReadingAssociation( COM_No, SendBuff, Item, RReading_AssociationSum );
				
				if( *BlockNumberN == 10)  *SendBuff = 0x01;
					break;
	
			default: break; 			
		}	
			if( *SendBuff == 0x01 )
				{
					 //last-block=1表示是最后一帧数据，后续没有数据块。
					 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
				}
				else
				{
						HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
				}
				*/
		return Len;
	}	
#if 0
//Public association
short GetNextAssociation_LNClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	 unsigned long* BlockNumberN;
  //unsigned char* BlockNumberN;
	short Len = 0;
  short Item;
	//unsigned short ADD_NUMBER = 1;   // 增加项目数..
	
//	BlockNumber = CommPara[COM_No].BlockNumber;
//	
//	switch( *BlockNumber )

	// BlockNumberN = &HComm.BIGBlockNumber[COM_No];
		BlockNumberN = CommPara[COM_No].BlockNumber;
		
	switch( *BlockNumberN )	
	// switch( HComm.BIGBlockNumber[COM_No] )
	{
		case 1:
			Item = (( *BlockNumberN)-1)*7;
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, Item, DLMS_RealSum );	
			break;
		case 2:
			Item = (( *BlockNumberN)-2)*12;
			Len = GetObjectListDataGrade0( COM_No, SendBuff, Item, DLMSDataSum );
			break;
		case 3:
			Item = (( *BlockNumberN)-3);
			Len = GetObjectListRAMDataGrade0( COM_No, SendBuff, Item, DLMS_RAM_DataSum );
			break;	
		case 4:
		case 5:	
			Item = (( *BlockNumberN)-4)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, Item+10 );
			break;
		case 6:	
			Item = (( *BlockNumberN)-4)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, DLMS_ECSum );
			break;	
		case 7:
		case 8:
		case 9:
			Item = (( *BlockNumberN)-7)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, Item+8 );      //508..
			break;
		case 10:
			Item = (( *BlockNumberN)-7)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, DLMS_MDSum );   //单独一条进行打包..
			break;
		case 11:
		case 12:
		case 13:
		case 14:
			Item = (( *BlockNumberN)-11)*4;
			Len = GetObjectListProfile_genericGrade0( COM_No, SendBuff, Item, DLMS_ProfileSum );
			break;	
		case 15:
			Item = (( *BlockNumberN)-15)*4;
			Len = GetObjectListDemand_registerGrade0( COM_No, SendBuff, Item, DLMS_DemandRegSum );    // 8..
			break;
			case 16:
			///Item = (( *BlockNumberN)-18)*4;
			Len = GetObjectListRegister_monitorGrade0( COM_No, SendBuff, 0, DLMS_Reg_monitorSum );    // 8..
			break;
	///	case 19:
	///		Len = GetObjectListSingle_actionGrade0( COM_No, SendBuff, 0, DLMS_Sigle_actSum );
	///		break;	
		case 17:
			Len = GetObjectListSpecial_days_tableGrade0( COM_No, SendBuff, 0, 3 );
			break;	
		case 18:
			Len = GetObjectListScript_tableGrade0( COM_No, SendBuff, 0, DLMS_ScriptParaSum );
			break;	
		case 19:
			Len = GetObjectListPort_dataGrade0( COM_No, SendBuff, 0, DLMS_PortdataSum );   //
			break;
		case 20:
			Len = GetObjectListImage_transfer_dataGrade0( COM_No, SendBuff, 0, ADD_IMAGENUMBER );				//增加46类\42\43..
			break;
		case 21:	
			Len = GetObjectListRegister_ActivationGrade0( COM_No, SendBuff, 0, DLMS_Reg_ActivationSum );				//增加寄存器激活..												
			break;			
	    case 22:	                  
			Len = GetObjectListALNGrade0( COM_No, SendBuff, 0, 3);
		  break;  
		case 23:	                  
			///  Item = (( *BlockNumberN)-26)*7;
			Len = GetObjectListRegGrade0( COM_No, SendBuff, 0, DLMS_RegSum);
		  break; 
		case 24:			
			Len = GetObjectListGPRSPort_dataGrade0( COM_No, SendBuff, 0, DLMSGPRS2829Sum );   //

			 if( *BlockNumberN == 25)  *SendBuff = 0x01;	//54			//last-block=1表示是最后一帧数据，后续没有数据块。
			break;	
///		case 28:
///			Len = GetObjectListPushGrade0( COM_No, SendBuff, 0, DLMSPushSum );
///	#if ( DisListCUMMDReg != YesCheck)
///						 if( *BlockNumberN == 29)  *SendBuff = 0x01;	//54			//last-block=1表示是最后一帧数据，后续没有数据块。
///						break;	
///	#else
///			break;
///			case 29:
///			Item = (( *BlockNumberN)-29)*10;
///			Len = GetObjectListCUMMDRegGrade0( COM_No, SendBuff, Item, DLMS_CUMDSum);
///				 if( *BlockNumberN == 30 ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
///				break;	
///	#endif	
		default: break;				
	}	
  		if( *SendBuff == 0x01 )
			{
				 //last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
	return Len;
}
#endif
#if 0
short GetNextAssociation_LNClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	 unsigned long* BlockNumberN;
  //unsigned char* BlockNumberN;
	short Len = 0;
  short Item;
	//unsigned short ADD_NUMBER = 1;   // 增加项目数..
	
//	BlockNumber = CommPara[COM_No].BlockNumber;
//	
//	switch( *BlockNumber )

	// BlockNumberN = &HComm.BIGBlockNumber[COM_No];
		BlockNumberN = CommPara[COM_No].BlockNumber;
		
	switch( *BlockNumberN )	
	// switch( HComm.BIGBlockNumber[COM_No] )
	{
		case 1:
			Item = (( *BlockNumberN)-1)*7;
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, Item, DLMS_RealSum );	
			break;
		case 2:
			Item = (( *BlockNumberN)-2)*12;
			Len = GetObjectListDataGrade0( COM_No, SendBuff, Item, DLMSDataSum );
			break;
		case 3:
			Item = (( *BlockNumberN)-3)*12;
			Len = GetObjectListRAMDataGrade0( COM_No, SendBuff, Item, Item+12 );
			break;	
		case 4:
			Item = (( *BlockNumberN)-3)*12;
			Len = GetObjectListRAMDataGrade0( COM_No, SendBuff, Item, DLMS_RAM_DataSum );
			break;	
		case 5:
		case 6:	
			Item = (( *BlockNumberN)-5)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, Item+10 );
			break;
		case 7:	
			Item = (( *BlockNumberN)-5)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, DLMS_ECSum );
			break;	
		case 8:
		case 9:
		case 10:
			Item = (( *BlockNumberN)-8)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, Item+8 );      //508..
			break;
		case 11:
			Item = (( *BlockNumberN)-8)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, DLMS_MDSum );   //单独一条进行打包..
			break;
		case 12:
		case 13:
		case 14:
		case 15:
			Item = (( *BlockNumberN)-12)*4;
			Len = GetObjectListProfile_genericGrade0( COM_No, SendBuff, Item, Item+4 );
			break;	
		case 16:
			Item = (( *BlockNumberN)-12)*4;
			Len = GetObjectListProfile_genericGrade0( COM_No, SendBuff, Item, DLMS_ProfileSum );
			break;	
		case 17:
			Item = (( *BlockNumberN)-17)*4;
			Len = GetObjectListDemand_registerGrade0( COM_No, SendBuff, Item, DLMS_DemandRegSum );    // 8..
			break;
			case 18:
			Item = (( *BlockNumberN)-18)*4;
			Len = GetObjectListRegister_monitorGrade0( COM_No, SendBuff, Item, DLMS_Reg_monitorSum );    // 8..
			break;
		case 19:
			Len = GetObjectListSingle_actionGrade0( COM_No, SendBuff, 0, DLMS_Sigle_actSum );
			break;	
		case 20:
			Len = GetObjectListSpecial_days_tableGrade0( COM_No, SendBuff, 0, 3 );
			break;	
		case 21:
			Len = GetObjectListScript_tableGrade0( COM_No, SendBuff, 0, DLMS_ScriptParaSum );
			break;	
		case 22:
			Len = GetObjectListPort_dataGrade0( COM_No, SendBuff, 0, DLMS_PortdataSum );   //
			break;
		case 23:
			Len = GetObjectListImage_transfer_dataGrade0( COM_No, SendBuff, 0, ADD_IMAGENUMBER );				//增加46类\42\43..
			break;
		case 24:	
			Len = GetObjectListRegister_ActivationGrade0( COM_No, SendBuff, 0, DLMS_Reg_ActivationSum );				//增加寄存器激活..												
			break;			
///	    case 25:	                  
///			Len = GetObjectListALNGrade0( COM_No, SendBuff, Item, 3);
///		  break;  
		case 25:	                  
			  Item = (( *BlockNumberN)-25)*7;
			Len = GetObjectListRegGrade0( COM_No, SendBuff, Item, DLMS_RegSum);
		  break; 
		case 26:			
			Len = GetObjectListGPRSPort_dataGrade0( COM_No, SendBuff, 0, DLMSGPRS2829Sum );   //
			break;	
		case 27:
			Len = GetObjectListPushGrade0( COM_No, SendBuff, 0, DLMSPushSum );
	#if ( DisListCUMMDReg != YesCheck)
						 if( *BlockNumberN == 28)  *SendBuff = 0x01;	//54			//last-block=1表示是最后一帧数据，后续没有数据块。
						break;	
	#else
			break;
			case 28:
			Item = (( *BlockNumberN)-28)*10;
			Len = GetObjectListCUMMDRegGrade0( COM_No, SendBuff, Item, DLMS_CUMDSum);
				 if( *BlockNumberN == 29 ) *SendBuff = 0x01;				//last-block=1表示是最后一帧数据，后续没有数据块。
				break;	
	#endif	
	
		default: break;				
	}	
  		if( *SendBuff == 0x01 )
			{
				 //last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
	return Len;
}
#endif
short GetObjectListASNGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	
	Ptr = Buff;	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	Point = Dest+9;
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x02;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
/*	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x03;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x04;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	*/
	FillIntegerToBuff( Dest+7, Length );										//数据长度为字节 
	Length += 9;
	return Length;

}
short GetObjectListASNGrade1( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	
	Ptr = Buff;	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	Point = Dest+9;
/*	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x02;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	*/
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x03;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x04;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*9 +6*4 = 108  //VERSION=1    //21+7*8 +6*4 = 101  VERSION = 0
	Length += Len;
	Point += Len;
	FillIntegerToBuff( Dest+7, Length );										//数据长度为字节 
	Length += 9;
	return Length;

}

short GetNextAssociation_LNClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	 unsigned long* BlockNumberN;
  //unsigned char* BlockNumberN;
	short Len = 0;
  short Item;
	//unsigned short ADD_NUMBER = 1;   // 增加项目数..
	
//	BlockNumber = CommPara[COM_No].BlockNumber;
//	
//	switch( *BlockNumber )

	// BlockNumberN = &HComm.BIGBlockNumber[COM_No];
		BlockNumberN = CommPara[COM_No].BlockNumber;
		
	switch( *BlockNumberN )	
	// switch( HComm.BIGBlockNumber[COM_No] )
	{
		case 1:
			Item = (( *BlockNumberN)-1)*7;
			Len = GetObjectListRealRegGrade0( COM_No, SendBuff, Item, DLMS_RealSum );	
			break;
		case 2:
		case 3:
		case 4:
		case 5:	
		case 6:	
			Item = (( *BlockNumberN)-2)*10;
			Len = GetObjectListDataGrade0( COM_No, SendBuff, Item, Item+10 );
			break;
		case 7:
			Item = (( *BlockNumberN)-2)*10;
			Len = GetObjectListDataGrade0( COM_No, SendBuff, Item, DLMSDataSum );
			break;
		case 8:
			Item = (( *BlockNumberN)-8)*12;
			Len = GetObjectListRAMDataGrade0( COM_No, SendBuff, Item, Item+12 );
			break;
		case 9:	
			Item = (( *BlockNumberN)-8)*12;
			Len = GetObjectListRAMDataGrade0( COM_No, SendBuff, Item, DLMS_RAM_DataSum );
			break;	
		case 10:
		case 11:
			Item = (( *BlockNumberN)-10)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, Item+10 );
			break;
		case 12:	
			Item = (( *BlockNumberN)-10)*10;
			Len = GetObjectListECRegGrade0( COM_No, SendBuff, Item, DLMS_ECSum );
			break;	
		case 13:
		case 14:	
		case 15:
		case 16:
			Item = (( *BlockNumberN)-13)*4;
			Len = GetObjectListProfile_genericGrade0( COM_No, SendBuff, Item, Item+4 );			
			break;
		case 17:
			Item = (( *BlockNumberN)-13)*4;
			Len = GetObjectListProfile_genericGrade0( COM_No, SendBuff, Item, DLMS_ProfileSum );			
			break;	
	    case 18:
			Len = GetObjectListASNGrade0( COM_No, SendBuff, Item, 3 );
			break;
		case 19:
			Len = GetObjectListASNGrade1( COM_No, SendBuff, Item, 2 );
			break;
		case 20:	
			  Item = (( *BlockNumberN)-20)*8;
			Len = GetObjectListRegGrade0( COM_No, SendBuff, Item, Item+8);
			break;
		case 21:	
			  Item = (( *BlockNumberN)-20)*8;
			Len = GetObjectListRegGrade0( COM_No, SendBuff, Item, DLMS_RegSum);
		  break;
		case 22:
		  	  Len = GetObjectListImage_transfer_dataGrade0( COM_No, SendBuff, 0, ADD_IMAGENUMBER ); 			  //增加46类\42\43..
		  break;
		case 23:
				Len = GetObjectListOther_transfer_dataGrade0( COM_No, SendBuff, 0, 5 );				//增加46类\42\43..
			break;
		case 24:
				Len = GetObjectListOther_transfer_dataGrade1( COM_No, SendBuff, 0, 3 ); 			//增加46类\42\43..
			break;
		case 25:
		case 26:
		case 27:
		case 28:	
			Item = (( *BlockNumberN)-25)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, Item+8 );
			break;
		case 29:
			Item = (( *BlockNumberN)-25)*8;
			Len = GetObjectListMDExtRegGrade0( COM_No, SendBuff, Item, DLMS_MDSum );
			break;
		case 30:
			Len = GetObjectListSingle_actionGrade0( COM_No, SendBuff, 0, DLMS_Sigle_actSum );
				break;		
		case 31:
			Len = GetObjectListRegister_monitorGrade0( COM_No, SendBuff, 0, DLMS_Reg_monitorSum );
				break;	
		case 32:
				Len = GetObjectListGPRSPort_dataGrade0( COM_No, SendBuff, 0, 2 );
				break;
		case 33:
			Len = GetObjectListPushGrade0( COM_No, SendBuff, 0, DLMSPushSum );
				break;
		case 34:
			Len = GetObjectListPort_dataGrade0( COM_No, SendBuff, 0, 4 );
			break;
	    case 35:
		case 36:
			Item = (( *BlockNumberN)-35)*4;
			Len = GetObjectListScript_tableGrade0( COM_No, SendBuff, Item, Item+4 );				//增加46类\42\43..
			break;
		case 37:
			Item = (( *BlockNumberN)-35)*4;
			Len = GetObjectListScript_tableGrade0( COM_No, SendBuff, Item, DLMS_ScriptParaSum );				//增加46类\42\43..
		  if( *BlockNumberN == 38)	*SendBuff = 0x01;
		  break;
		default: break;				
	}	
  		if( *SendBuff == 0x01 )
			{
				 //last-block=1表示是最后一帧数据，后续没有数据块。
				 HComm.COSEM_BlockState[COM_No] = ENDDATABLOCK;
			}
			else
			{
					HComm.COSEM_BlockState[COM_No] = MIDDATABLOCK;
			}
	return Len;
}


//sky2013..
short GetObjectListPort_dataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 
	Point = Dest+9;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x14;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x14;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_local_port_setup, Ptr );		//21+7*9 +6*0 = 84
	Length += Len;
	Point += Len;
/*
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x16;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x01;
	Buff[2] = 0x16;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_IEC_HDLC_setup, Ptr );		//21+7*9 +6*0 = 84
	Length += Len;
	Point += Len;
*/	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_TCP_UDP_setup, Ptr );			//21+7*6+6*0 = 63
	Length += Len;
	Point += Len;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x04;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_GPRS_modem_setup, Ptr );		//21+7*4+6*0 = 49
	Length += Len;
	Point += Len;

/*
	// 28类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x02;
	Buff[3] = 0x02;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Auto_answer_setup, Ptr );		//21+7*4+6*0 = 49
	Length += Len;
	Point += Len;

	//29类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x02;
	Buff[3] = 0x01;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Auto_connect_setup, Ptr );		//21+7*4+6*0 = 49
	Length += Len;
	Point += Len;
*/			
	FillIntegerToBuff( Dest+7, Length );										//数据长度为字节 
	Length += 9;
	return Length;
}
	
//sky2013..
short GetObjectListGPRSPort_dataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 

	Point = Dest+9;
	// 28类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x02;
	Buff[3] = 0x02;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Auto_answer_setup, Ptr );		//21+7*4+6*0 = 49
	Length += Len;
	Point += Len;

	//29类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x02;
	Buff[3] = 0x01;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Auto_connect_setup, Ptr );		//21+7*4+6*0 = 49
	Length += Len;
	Point += Len;

//////////////////////////////////				
	FillIntegerToBuff( Dest+7, Length );										//数据长度为字节 
	Length += 9;
	return Length;
}
#if 0
short GetObjectListALNGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 

	Point = Dest+9;
	// 15类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*4+6*0 = 49
	Length += Len;
	Point += Len;

	//15类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x02;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*4+6*0 = 49
	Length += Len;
	Point += Len;
	

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x03;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Association_LN, Ptr );		//21+7*4+6*0 = 49
	Length += Len;
	Point += Len;
			
	FillIntegerToBuff( Dest+7, Length );										//数据长度为字节 
	Length += 9;
	return Length;

}
#endif
//升级18类sky2013.. + 46类.. + 64类..
short GetObjectListImage_transfer_dataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 

	Point = Dest+9;
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Image_transfer, Ptr );		//21+7*9 +6*0 = 84
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x29;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_SAP_assignment, Ptr );		//21+7*2 +6*1 = 41
	Length += Len;
	Point += Len;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0e;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Register_activation, Ptr );	//21+7*2 +6*2 = 47
	Length += Len;
	Point += Len;

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Special_days_table, Ptr );	//21+7*2 +6*2 = 47
	Length += Len;
	Point += Len;


	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x0D;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Activity_calendar, Ptr );		//21+7*10+6*1 = 97
	Length += Len;
	Point += Len;
/*
	//增加46类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x05;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_SMTP_setup, Ptr );		//21+7*6+6*0 = 63
	Length += Len;
	Point += Len;

	//增加43类-1..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x02;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_MACaddr_GPRS, Ptr );		
	Length += Len;
	Point += Len;

	//增加43类-2..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x1c;
	Buff[3] = 0x06;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_MACaddr_GPRS, Ptr );		
	Length += Len;
	Point += Len;
	
	//增加42类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x01;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_IPv4_setup, Ptr );		
	Length += Len;
	Point += Len;
	//CLASS ID 42: IPv4 setup-GPRS (0-0:25.1.0.255 Class ID: 42)..

//增加64类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x2B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Security_setup, Ptr );		
	Length += Len;
	Point += Len;
*/			
	FillIntegerToBuff( Dest+7, Length );										//数据长度为字节 
	Length += 9;
	return Length;
}	
short GetObjectListOther_transfer_dataGrade0( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 

	Point = Dest+9;
	
	//增加46类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x05;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_SMTP_setup, Ptr );		//21+7*6+6*0 = 63
	Length += Len;
	Point += Len;

	//增加43类-1..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x02;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_MACaddr_GPRS, Ptr );		
	Length += Len;
	Point += Len;

	//增加43类-2..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x1c;
	Buff[3] = 0x06;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_MACaddr_GPRS, Ptr );		
	Length += Len;
	Point += Len;
	
	//增加42类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x01;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_IPv4_setup, Ptr );		
	Length += Len;
	Point += Len;
	//CLASS ID 42: IPv4 setup-GPRS (0-0:25.1.0.255 Class ID: 42)..

//增加64类..
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x2B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Security_setup, Ptr );			
	Length += Len;
	Point += Len;

	FillIntegerToBuff( Dest+7, Length );										//数据长度为字节 
	Length += 9;
	return Length;
}
short GetObjectListOther_transfer_dataGrade1( short COM_No, unsigned char* Dest, short Satrt, short End ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char* Point;
	short Length;
	short Len;
	
	Ptr = Buff;
	
	Length = 0;
	CreatSubObjectListHead( COM_No, Dest ); 

	Point = Dest+9;

//class70	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x60;
	Buff[3] = 0x03;
	Buff[4] = 0x0A;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Disconnect_control, Ptr );	
	Length += Len;
	Point += Len;
	
//class71
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x11;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Limiter, Ptr );			
	Length += Len;
	Point += Len;
//class71
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x11;
	Buff[3] = 0x00;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	Len = CreatObjectListElement( COM_No, Point, Class_id_Limiter, Ptr );		
	
	Length += Len;
	Point += Len;

	FillIntegerToBuff( Dest+7, Length );										//数据长度为字节 
	Length += 9;
	return Length;
}

//////////////////////////////////////////////////////////////////////////
unsigned char ActionAssociation_LNClassData( short COM_No,unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method;
	unsigned char* APPLState;
	unsigned char Tag[12];	
	
	APPLState = CommPara[COM_No].APPLState;
		
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x28;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	
	//判断是否是SHA-256..
	//ACTION通用性设置.，带01 和不带01.
//	if((ICEC.use_SHA256 == 0x01)&&( Method == 1 )&&(*(Source+9)==0x01)&&(*(Source+10)==0x09)) Source = Source+1;
/*
  if((ICEC.use_SHA256 == 0x01)&&( Method == 1 )&&(*(Source+9)==0x09)&&(*(Source+10)==0x24))
	{
		 //验证SHA-256 -- TAG算法正确性..
		 RAM_Write( &crypt.glo_FC[1],Source+11, 4 );
		 //校验及取随机数..
		 if(SHA_256EnDecrypt(ESAM,Source+80,Source+15)!=32) ErrorByte = other_reason;
			
			//成功则建立连接..	//失败则推出连接..
	    if( ErrorByte != 0 ) *APPLState = APPLIdle;			//未建立应用连接
	    else *APPLState = APPLON; 							//建立应用连接	
		
      if( ErrorByte == 0 )
      {
      	 ErrorByte = 0x22;  //表示验证通过，需要做特殊处理..
      }
		return ErrorByte;
	}
	*/	
	if(( Method == 1 )&&(*(Source+9)==0x01)&&(*(Source+10)==0x09)&&(*(Source+11)==0x11)) Source = Source+1;
		if(( Method == 1 )&&(*(Source+9)==0x09)&&(*(Source+10)==0x11))
		{
	//		E2P_RData( Ptr, EActivityCalendarNo, 1 );						//0:当前时段参数指向ECalendar_name_active	
	//		if( *Ptr != 0 ) *Ptr = 0;										//1:当前时段参数指向ECalendar_name_passive	
	//		else *Ptr = 1;
	//		E2P_WData( EActivityCalendarNo, Ptr, 1 );						//0:当前时段参数指向ECalendar_name_active	
	//		GetDay_schedule( GetDay_id() ); 								//判断取当前费率
	//		GetFeeNo();
	//		SM.StandardEventCode = Passive_TOU_Activated;
	//		LoadRecord( Standard_Event_No );
	//		Execute_Passive_TOU_Activated();
				//需要增加验证..
	//-------------计算Tag验证----------
		RAM_Write((unsigned char *)&crypt.SH[1],(unsigned char *)&crypt.IV[8],4);
	//	Encrypt_ByteData(crypt.EK,16,crypt.IV,12,crypt.AK,16,Ptr,0,Ptr,Tag);
		  ErrorByte = 0x22;  //表示验证通过，需要做特殊处理..
		}
		else ErrorByte = other_reason;		
	
		return ErrorByte;

	if(( Method == 1 )&&(*(Source+9)==0x09)&&(*(Source+10)==0x11)&&(*(Source+11)==0x10))
	{		
		//计算TAG进行比对..
	/*		if( Init_For_Process( ESAM ) != 0 ) 
			{ 
				ICEC.EsamOK = 0;
        ErrorByte = 1;
				if( Card_CoolRST( ESAM ) != 0 ) 
				{ 
					ICEC.EsamOK = 0;
					return 1;
				}				
			}
			if(( Init_For_Process( ESAM ) != 0 )&&(ErrorByte == 0)) 
			{ 
				ICEC.EsamOK = 0;
				return 1;
			}
			else 
			{
					ICEC.EsamOK = 5;
					RAM_Write( crypt.glo_FC,Source+11, 5 );
					// 更新systemtitle和 AAD(AK)..//test sky20150310..
					E2P_RData( ICComm.SystemTitle,E2P_server_system_title, 8 );   //服务器的systile..
					//传递AAD--AK
					E2P_RData( ICComm.SysAAD,E2P_global_key_transfer_data, 16 );   //AK..
					//====
					Gcm128EnDecrypt( ESAM, Ptr,crypt.S_Challenge, 0 ,0x10,0x08, 8 );
					if( Data_Comp( Ptr, Source+16, 12 ) != 0 ) 
						{
							ErrorByte = 0x01; 									//TAG验证没通过..
						}	
			}	
			*/
		//成功则建立连接..	//失败则推出连接..
			if( ErrorByte != 0 ) *APPLState = APPLIdle;			//未建立应用连接
	    else *APPLState = APPLON; 							//建立应用连接	
		
      if( ErrorByte == 0 )
      {
      	 ErrorByte = 0x22;  //表示验证通过，需要做特殊处理..
      	 if((COM_No==1)||(COM_No==0))
						{
							if(ICComm.CommErrTime[COM_No]!=0)
							{
								ICComm.CommErrTime[COM_No] = 0;
								if(COM_No==1)
								{
									E2P_WData( COMMERRTIMES1, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
									//if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME1,(unsigned char*)&Clk.YearH,12 );
								}
								else
								{
									E2P_WData( COMMERRTIMES2, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
									//if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME2, (unsigned char*)&Clk.YearH,12 );
								}
							}
						}
      }
      else{
      				if((COM_No==1)||(COM_No==0))
							{
								ICComm.CommErrTime[COM_No]++;
									if(COM_No==1)
									{
										E2P_WData( COMMERRTIMES1, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
										if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME1,(unsigned char*)&Clk.YearH,12 );
									}
									else
									{
										E2P_WData( COMMERRTIMES2, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
										if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME2, (unsigned char*)&Clk.YearH,12 );
									}
							}
      }
	}
	else ErrorByte = other_reason;		

	return ErrorByte;
}	
	
