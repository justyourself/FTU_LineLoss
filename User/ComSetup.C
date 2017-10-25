#include "TypeMeter.h"
#include "TypeE2p.h"
//#include "Measure.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "TypeE2p.h"
#include "Data.h"
#include "General.h"

//#include "RsComm.h"
//#include "interrupt.h"
#include "Mem.h"
#include "Time.h"
#include "Power.h"
#include "Initial.h"
//#include "Produce.h"

#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
#include "ActCalendar.h"
#include "RegisterMonitor.h"
#include "SingleAction.h"
#include "ProfileGeneric.h"
#include "DisconnectControl.h"
#include "ScriptTable.h"
#include "AssociationLN.h"
#include "Limiter.h"
#include "ComSetup.h"
//#include "Esamcpu.h"
//#include "Gprs.h"



//读取制造厂家自定义类型..
/*
电表主动发送给模块：
服务器模式下，GPRS上电推送IP设置：0x01[GPRS推送设置] ---8字节
采用特殊的主站（客户端）地址CLIENT：0x45;[告警事件]   --- 8字节
采用特殊的主站（客户端）地址CLIENT：0x46; [推送事件]  --- 8字节
采用特殊的主站（客户端）地址CLIENT：0x47;【备用事件】 --- 8字节
MSM判断报文：XXXXXXXX ---8个字节+;
通讯报文格式如下：
传输报文:
0x00,0x80,  --按照128类(厂家自定义来定义).
OBIS:0.1.128.128.70.255
格式定义：02 05（结构体，2个元素）
09 08 XX XX XX XX  -- XX XX – XX XX
09 08 XX XX XX XX  -- XX XX – XX XX
09 08 XX XX XX XX  -- XX XX – XX XX
09 08 XX XX XX XX  -- XX XX – XX XX
09 08 XX XX XX XX  -- XX XX – XX XX

*/
short GetManufacture_setupData( unsigned char* Dest, unsigned char* Source ) 
{

	unsigned char Buff[50];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
//	unsigned short Temp;	

	*Dest = Get_data_access_result;					//异常标志
	*(Dest+1) = object_undefined; 					//无对应的OBIS码
	Len = 2;		
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x01;
	Buff[2] = 0x80;
	Buff[3] = 0x80;
	Buff[4] = 0x46;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				//读取参数..
				//端口号处理和推送地址处理(固定HDLC中的地址处理:0x45、0x46、0x47、0x48、0x49)..
				//E2P_RData( (unsigned char*)&Temp, TCP_UDP_port, 2 );
				//FillIntegerToBuff( Dest+2, Temp );
				// E2P_RData( Ptr, Device_ID9, 48 );  //test..				
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x05;
				*(Dest+3) = 0x09;												
				*(Dest+4) = 0x08;	
				//主站注册..
				E2P_RData( Dest+5, Push_Setup_REGIS_destination+2, 8 );
															
       // RAM_Write( Dest+5, Ptr, 8 );   //主站GPRS模块推送地址..
				*(Dest+13) = 0x09;												
				*(Dest+14) = 0x08;												
        //RAM_Write( Dest+15, Ptr+8, 8 );   //告警..
				E2P_RData( Dest+15, Push_Setup_Alarm_destination+2, 8 );
 				*(Dest+21) = 0x00;												
				*(Dest+22) = 0x45;		
				      
 				*(Dest+23) = 0x09;												
				*(Dest+24) = 0x08;												
        //RAM_Write( Dest+25, Ptr+16, 8 );   //推送..  
				E2P_RData( Dest+25, Push_Setup_Scheduler2_destination+2, 8 );
 				*(Dest+31) = 0x00;												
				*(Dest+32) = 0x46;		
				             
 				*(Dest+33) = 0x09;												
				*(Dest+34) = 0x08;												
        //RAM_Write( Dest+35, Ptr+24, 8 );   //备用..   
				E2P_RData( Dest+35, Push_Setup_Scheduler1_destination+2, 8 );
 				*(Dest+41) = 0x00;												
				*(Dest+42) = 0x47;		
				               
 				*(Dest+43) = 0x09;												
				*(Dest+44) = 0x08;												
        //RAM_Write( Dest+45, Ptr+32, 8 );   //msm.. 
				E2P_RData( Dest+45, Push_Setup_SMS_destination+2, 8 );
 				// *(Dest+51) = 0x00;												
				// *(Dest+52) = 0x48;		
	                        
				Len = 53;
				break;
			default:
					*Dest = Get_data_access_result;					//异常标志
					*(Dest+1) = object_undefined; 					//无对应的OBIS码
					Len = 2;		
				 break;			
		}
	}		
	return Len;
	
}	

//取TCP_UDP_setup类数据
short GetTCP_UDP_setupClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned short Temp;
		
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_Unsigned16;								//端口号(3)					unsigned16
				E2P_RData( (unsigned char*)&Temp, TCP_UDP_port, 2 );
				FillIntegerToBuff( Dest+2, Temp );
				Len = 4;
				break;	
			case 3:
				*(Dest+1) = D_OctetStr;									//主站IP地址(5)				octet-string
				*(Dest+2) = 4;											
				E2P_RData( Dest+3, TCP_IP_reference, 4 );
				Len = 7;
				break;	
			case 4:
				*(Dest+1) = D_Unsigned16;								//最大接收字节数(3)			unsigned16
				E2P_RData( (unsigned char*)&Temp, TCP_MSS, 2 );
				FillIntegerToBuff( Dest+2, Temp );
				Len = 4;
				break;	
			case 5:
				*(Dest+1) = D_Unsigned8;								//最大的并发连接数(3)		unsigned8
				E2P_RData( Dest+2, TCP_nb_of_sim_conn, 1 );
				Len = 3;
				break;	
			case 6:
				*(Dest+1) = D_Unsigned16;								//TCP断开连接时间(3)		unsigned16
				E2P_RData( (unsigned char*)&Temp, TCP_inactivity_time_out, 2 );
				FillIntegerToBuff( Dest+2, Temp );
				Len = 4;
				break;	
			default: break;			
		}
	}		
	return Len;
}	

unsigned char SetTCP_UDP_setupClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
//	unsigned char AccessMode;
	
//	S_GPRS_PARA gprs_pa;
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
			case 2:														//端口号(3)					unsigned16		
				if( *DataType != D_Unsigned16 ) { ErrorByte = type_unmatched; break; }
				Temp = GetBuffToInteger( WriteBufAds+1 );
				E2P_WData( TCP_UDP_port, (unsigned char*)&Temp, 2 );
				
		//		CmGprs_GetPara((uint8_t*)&gprs_pa,GPRS_PARA_ALL);
		//		gprs_pa.Host_port=Temp;
				//CmGprs_SetPara((uint8_t*)&gprs_pa.Host_port,GPRS_PARA_PORT);
		//		CmGprs_SetPara((uint8_t*)&gprs_pa,GPRS_PARA_ALL);
				break;	
			case 3:														//主站IP地址(5)				octet-string
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) != 4 ) { ErrorByte = type_unmatched; break; }
				E2P_WData( TCP_IP_reference, WriteBufAds+2, 4 );
				
		//		CmGprs_GetPara((uint8_t*)&gprs_pa,GPRS_PARA_ALL);
		//		RAM_Write(gprs_pa.Host_ip,WriteBufAds+2,4);
		//		CmGprs_SetPara((uint8_t*)&gprs_pa,GPRS_PARA_ALL);
				break;	
			case 4:														//最大接收字节数(3)			unsigned16	
				if( *DataType != D_Unsigned16 ) { ErrorByte = type_unmatched; break; }
				Temp = GetBuffToInteger( WriteBufAds+1 );
				E2P_WData( TCP_MSS, (unsigned char*)&Temp, 2 );
				break;	
			case 5:														//最大接收字节数(3)			unsigned16	
				if( *DataType != D_Unsigned8 ) { ErrorByte = type_unmatched; break; }
				E2P_WData( TCP_nb_of_sim_conn, WriteBufAds+1, 1 );
				break;	
			case 6:														//最大接收字节数(3)			unsigned16	
				if( *DataType != D_Unsigned16 ) { ErrorByte = type_unmatched; break; }
				Temp = GetBuffToInteger( WriteBufAds+1 );
				E2P_WData( TCP_inactivity_time_out, (unsigned char*)&Temp, 2 );
				break;	
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}		
	return ErrorByte;
}	

//===================================================================================================================================
//取GPRS_modem_setup类数据
short GetGPRS_modem_setupClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned short Temp;
	unsigned short i,j;
	unsigned char* Point;
		
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x04;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_OctetStr;									//网络名(33)				octet-string
				*(Dest+2) = 32;											
				E2P_RData( Dest+3, GPRS_APN, 32 );
				Len = 35;
				break;	
			case 3:
				*(Dest+1) = D_Unsigned16;								//个人识别号码(3)			unsigned16
				E2P_RData( (unsigned char*)&Temp, GPRS_PIN_code, 2 );
				FillIntegerToBuff( Dest+2, Temp );
				Len = 4;
				break;	
			case 4:
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x02;
				for( i=0;i<2;i++ )
				{
					Point = Dest+i*12;
					*(Point+3) = Data_structure;
					*(Point+4) = 0x05;
					E2P_RData( Ptr, GPRS_quality_of_service_default+i*6, 5 );
					for( j=0;j<5;j++ )
					{
						*(Point+5+j*2) = D_Unsigned8;
						*(Point+5+j*2+1) = *(Ptr+j);												
					}	
				}
				Len = 27;	
				break;
			default: break;			
		}
	}		
	return Len;
}	

short Judge_GPRS_quality_of_service( unsigned char* Data )
{
	short i,j;
	unsigned char* Point;
	
	if(( *Data != Data_structure )||( *(Data+1) != 2 )) 			
	{
		return other_reason;
	}
	for( i=0;i<2;i++ )
	{
		Point = Data+i*12;
		if(( *(Point+2) != Data_structure )||( *(Point+3) != 0x05 )) return other_reason;
		for( j=0;j<5;j++ )
		{
			if( *(Point+4+j*2) != D_Unsigned8 ) return other_reason;
		}	
	}	
    return 0;
}

unsigned char SetGPRS_modem_setupClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
//	unsigned char AccessMode;
    unsigned char* Point;
	short i,j;
    
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x04;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
			case 2:														//主站IP地址(5)				octet-string
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) > 32 ) { ErrorByte = type_unmatched; break; }
				Temp = *(WriteBufAds+1);
				RAM_Fill( WriteBufAds+2+Temp, 32-Temp);
				E2P_WData( GPRS_APN, WriteBufAds+2, 32 );
				break;	
			case 3:														//个人识别号码(3)			unsigned16		
				if( *DataType != D_Unsigned16 ) { ErrorByte = type_unmatched; break; }
				Temp = GetBuffToInteger( WriteBufAds+1 );
				E2P_WData( GPRS_PIN_code, (unsigned char*)&Temp, 2 );
				break;	
			case 4:
				if( Judge_GPRS_quality_of_service( WriteBufAds ) != 0 )	{ ErrorByte = type_unmatched; break; }				
				for( i=0;i<2;i++ )
				{
					Point = WriteBufAds+i*12;		
					for( j=0;j<5;j++ )
					{
						*(Ptr+j) = *(Point+4+j*2+1);												
					}
					E2P_WData( GPRS_quality_of_service_default+i*6, Ptr, 5 );	
				}	
				break;				
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}		
	return ErrorByte;
}


//4.8.6 SMTP setup (class_id: 46, version: 0)
//This IC allows setting up data exchange using the SMTP protocol.
//SMTP modem setup 0…n class_id = 46, version = 0 Attributes Data type Min. Max. Def. Short name 
//1. logical_name(static) octet-string
//2. server_port(static)  long-unsigned 
//3. user_name(static) octet-string 
//4. login_password(static) octet-string 
//5. server_address(static) octet-string 
//6. sender_address(static) octet-string 
//Specific methods  m/o Attribute description
//===================================================================================================================================
//取SMTP setup类数据
short GetSMTP_setupClassData( unsigned char* Dest, unsigned char* Source ) 
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned short Temp;	

		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;
		
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x05;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_Unsigned16;									//2. server_port(static)  long-unsigned 
				//先按照取41类错误的来进行，设置成一样..		//主站端口号..
				E2P_RData( (unsigned char*)&Temp, TCP_UDP_port, 2 );
				//E2P_RData( (unsigned char*)&Temp, SMTP_setup_server_port, 2 );
				FillIntegerToBuff( Dest+2, Temp );
				Len = 4;
				break;	
			case 3:
				*(Dest+1) = D_OctetStr;										//3. user_name(static) octet-string 
				*(Dest+2) = 10;											
				E2P_RData( Dest+3, SMTP_setup_user_name, 10 );
				Len = 13;
				break;
			case 4:																			//4. login_password(static) octet-string 
				*(Dest+1) = D_OctetStr;									
				*(Dest+2) = 10;											
				E2P_RData( Dest+3, SMTP_setup_login_password, 10 );
				Len = 13;
				break;
			case 5:
				*(Dest+1) = D_OctetStr;										//服务器（主站）IP地址(5)				octet-string
				*(Dest+2) = 4;
				//先按照取41类错误的来进行，把TCP_IP_reference先设置成主站IP地址..												
				E2P_RData( Dest+3, TCP_IP_reference, 4 );				
				//E2P_RData( Dest+3, SMTP_setup_server_address, 4 );
				Len = 7;
				break;
			case 6:
				*(Dest+1) = D_OctetStr;										//发送端IP地址(5)				octet-string
				*(Dest+2) = 4;											
				E2P_RData( Dest+3, SMTP_setup_sender_address, 4 );
				Len = 7;
				break;									
			default: break;			
		}
	}			
	return Len;

}	

unsigned char SetSMTP_setupClassData( unsigned char *Source , unsigned short DataLen )
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
//	unsigned char AccessMode;

    
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x05;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
			case 2:										//2. server_port(static)  long-unsigned 
				if( *DataType != D_Unsigned16 ) { ErrorByte = type_unmatched; break; }
				Temp = GetBuffToInteger( WriteBufAds+1 );
				//先按照取41类错误的来进行，设置成一样..		//主站端口号..				
				E2P_WData( TCP_UDP_port, (unsigned char*)&Temp, 2 );
				//E2P_WData( SMTP_setup_server_port, (unsigned char*)&Temp, 2 );
				break;					
			case 3:										//3. user_name(static) octet-string 
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) > 10 ) { ErrorByte = type_unmatched; break; }
				Temp = *(WriteBufAds+1);
				RAM_Fill( WriteBufAds+2+Temp, 10-Temp);
				E2P_WData( SMTP_setup_user_name, WriteBufAds+2, 10 );
				break;	
			case 4:										//4. login_password(static) octet-string 
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) > 10 ) { ErrorByte = type_unmatched; break; }
				Temp = *(WriteBufAds+1);
				RAM_Fill( WriteBufAds+2+Temp, 10-Temp);
				E2P_WData( SMTP_setup_login_password, WriteBufAds+2, 10 );
				break;	
			case 5:											//服务器（主站）IP地址(5)				octet-string
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) != 4 ) { ErrorByte = type_unmatched; break; }				
				//先按照取41类错误的来进行，把TCP_IP_reference先设置成主站IP地址..												
				E2P_WData( TCP_IP_reference, WriteBufAds+2, 4 );				
				//E2P_WData( SMTP_setup_server_address, WriteBufAds+2, 4 );
				break;	
			case 6:											//服务器（主站）IP地址(5)				octet-string
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) != 4 ) { ErrorByte = type_unmatched; break; }				
				E2P_WData( SMTP_setup_sender_address, WriteBufAds+2, 4 );
				break;
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}	
	return ErrorByte;
	
}

//Class_id_MACaddr_GPRS (class_id: 43, version: 0)
//MAC address setup-GPRS (0-0:25.2.0.255 Class ID: 43)
//logical_name	octet_string[6]	0000190200FF
//MAC_address	Octet-string	IMEI of GPRS modem
//PLC模块地址设置PRIME PLC MAC address (0-0:28.6.0.255 Class ID: 43)..
//7E A0 24 FE FE FE FF 03 13 63 E2 E6 E6 00 C1 01 C1 00 2B 00 00 1C 06 00 FF 02 00 
//09 06 00 80 E1 06 3E 24 BD F8 7E
//备注:00 80 E1 06 3E 24 每个模块的物理地址不同，这是其中一块载波模块的物理地址
//===================================================================================================================================
//取SMTP setup类数据
short GetMACaddr_GPRSClassData( unsigned char* Dest, unsigned char* Source ) 
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Buff1[6];
	unsigned char* Ptr1;
	unsigned char Len=2;
	unsigned char Attribute;
//	unsigned short Temp;	
*Dest = Get_data_access_result; 				//异常标志
*(Dest+1) = object_undefined;					//无对应的OBIS码
Len = 2;

		
	Ptr = Buff;
	Ptr1 = Buff1;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x02;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	Buff1[0] = 0x00;				
	Buff1[1] = 0x00;
	Buff1[2] = 0x1c;    //28
	Buff1[3] = 0x06;
	Buff1[4] = 0x00;
	Buff1[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 )
	{
			if( Data_Comp( Ptr1, Source+2, 6 ) != 0 )   //PRIME
			{
				*Dest = Get_data_access_result;					//异常标志
				*(Dest+1) = object_undefined; 					//无对应的OBIS码
				Len = 2;		
			}	
			else
			{
				*Dest = Get_data;								//正常数据返回	
				Attribute = *(Source+2+6);						//请求的属性
				switch( Attribute )
				{
					case 1:														//logical_name
						*(Dest+1) = D_OctetStr;
						*(Dest+2) = 0x06;
						RAM_Write( Dest+3, Ptr1, 6 );
						Len = 9;
						break;	
					case 2:
						*(Dest+1) = D_OctetStr;										//MAC_address	Octet-string	PRIME 
						*(Dest+2) = 6;											
						E2P_RData( Dest+3, MACad_PRIME, 6 );
						Len = 9;
						break;
					default: break;			
				}
			}		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_OctetStr;										//MAC_address	Octet-string	IMEI of GPRS modem 
				*(Dest+2) = 15;											
				E2P_RData( Dest+3, MACad_GPRS, 15 );
				Len = 18;
				break;
			default: break;			
		}
	}		
	return Len;
	
}	

unsigned char SetMACaddr_GPRSClassData( unsigned char *Source , unsigned short DataLen )
{

	unsigned char Buff[6];
	unsigned char* Ptr;	
	unsigned char Buff1[6];
	unsigned char* Ptr1;
	unsigned char ErrorByte=0;
//	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
    
	Ptr = Buff;
	Ptr1 = Buff1;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x02;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	Buff1[0] = 0x00;				
	Buff1[1] = 0x00;
	Buff1[2] = 0x1c;    //28
	Buff1[3] = 0x06;
	Buff1[4] = 0x00;
	Buff1[5] = 0xff;
			
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
			if( Data_Comp( Ptr1, Logical_Name, 6 ) != 0 ) 
			{
				ErrorByte = object_undefined; 									//无对应的OBIS码
			}	
			else
			{
				switch( *Attribute )
				{
		//			case 1:														//logical_name
		//				break;	
					case 2:										//3. user_name(static) octet-string 
						if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
						if( *(WriteBufAds+1) != 6 ) { ErrorByte = type_unmatched; break; }
						E2P_WData( MACad_PRIME, WriteBufAds+2, 6 );
						break;	
					default: 
						ErrorByte = type_unmatched;
						break;			
				}
			}		
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
			case 2:										//3. user_name(static) octet-string 
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) != 15 ) { ErrorByte = type_unmatched; break; }
				E2P_WData( MACad_GPRS, WriteBufAds+2, 15 );
				break;	
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}	
	return ErrorByte;
	
}


////CLASS ID 42: IPv4 setup-GPRS(0-0:25.1.0.255 Class ID: 42)..
//#define IPv4_logical_name 	MACad_GPRS_end		//logical_name: octet_string[6] 7..
//#define IPv4_DL_reference   IPv4_logical_name + 7  //DL_reference: octet_string[6] 7..
//#define IPv4_IP_address     IPv4_DL_reference + 7  //IP_address: double_long_unsigned 5..
////#define IPv4_multicast_IP_addrss   IPv4_IP_address + 5 //multicast_IP_addrss: Array double_long_unsigned--21..
////#define IPv4_IP_options            IPv4_multicast_IP_addrss + 21    //IP_options:array IP_options_element;21..
////#define IPv4_subnet_mask     			 IPv4_IP_options + 21             //subnet_mask:double_long_unsigned..
////#define IPv4_gateway_IP_address 	 IPv4_subnet_mask + 5 //gateway_IP_address:double_long_unsigned
////#define IPv4_use_DHCP_flag   			 IPv4_gateway_IP_address + 5  //use_DHCP_flag:boolean..
////#define IPv4_primary_DNS_address   IPv4_use_DHCP_flag + 2  //primary_DNS_address:double_long_unsigned
////#define IPv4_secondary_DNS_address IPv4_primary_DNS_address + 5  //secondary_DNS_address:double_long_unsigned
////#define IPv4_secondary_DNS_addressEnd  IPv4_secondary_DNS_address + 5
//===================================================================================================================================
//取SMTP setup类数据
short GetIPv4_setupClassData( unsigned char* Dest, unsigned char* Source ) 
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	//unsigned short Temp;	

	*Dest = Get_data_access_result;					//异常标志
	*(Dest+1) = object_undefined; 					//无对应的OBIS码
	Len = 2;		
	
	Ptr = Buff;

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x01;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:   //DL_reference: octet_string[6] 7..				
				*(Dest+1) = D_OctetStr;										//3. user_name(static) octet-string 
				*(Dest+2) = 0x06;											
				E2P_RData( Dest+3, IPv4_DL_reference, 6 );
				Len = 9;
				break;
			case 3:		
				*(Dest+1) = D_Unsigned32;	
				E2P_RData( Dest+2, IPv4_IP_address, 4 );
				Len = 6;
				break;	
			default: break;			
		}
	}	
	return Len;
	
}	

unsigned char SetIPv4_setupClassData( unsigned char *Source , unsigned short DataLen )
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	//unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
   
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x19;
	Buff[3] = 0x01;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
		  case 2:   //DL_reference: octet_string[6] 7..				
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) != 6 ) { ErrorByte = type_unmatched; break; }		
				E2P_WData( IPv4_DL_reference, WriteBufAds+2, 6 );				
				break;
			case 3:										//D_Unsigned32
				if( *DataType != D_Unsigned32 ) { ErrorByte = type_unmatched; break; }
				E2P_WData( IPv4_IP_address, WriteBufAds+1, 4 );
				break;
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}	
	return ErrorByte;
	
}

//64类安全..
//读取Class_id_Security_setup类数据
short GetSecurity_setupClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
//	unsigned short Temp;
		
	Ptr = Buff;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x2B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_ENUM;								//security_policy      enum
				E2P_RData(  Dest+2, E2P_security_policy, 1 );
				Len = 3;
				break;	
			case 3:
				*(Dest+1) = D_ENUM;									//security_suite
				E2P_RData(  Dest+2, E2P_security_suite, 1 );
				Len = 3;
				break;	
			case 4:
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x08;
				E2P_RData( Dest+3, E2P_client_system_title, 8 );
				Len = 11;
				break;	
			case 5:
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x08;
				E2P_RData( Dest+3, E2P_server_system_title, 8 );
				Len = 11;
				break;	
			default: break;			
		}
	}		
	return Len;
}

// 64Class_id_Security_setup..
unsigned char SetSecurity_setupClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
//	unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
	unsigned char* DataType;
//	unsigned char AccessMode;

    
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	DataType = WriteBufAds;
	
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x2B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
			case 2:										//2. server_port(static)  long-unsigned 
				if( *DataType != D_ENUM ) { ErrorByte = type_unmatched; break; }
				E2P_WData( E2P_security_policy, WriteBufAds+1, 1 );
				break;					
			case 3:										//3. user_name(static) octet-string 
				if( *DataType != D_ENUM ) { ErrorByte = type_unmatched; break; }
				E2P_WData( E2P_security_suite, WriteBufAds+1, 1 );
				break;	
			case 4:										// octet-string 
					if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) != 8 ) { ErrorByte = type_unmatched; break; }											
				E2P_WData( E2P_client_system_title, WriteBufAds+2, 8 );	
				break;	
			case 5:											//				octet-string
				if( *DataType != D_OctetStr ) { ErrorByte = type_unmatched; break; }
				if( *(WriteBufAds+1) != 8 ) { ErrorByte = type_unmatched; break; }											
				E2P_WData( E2P_server_system_title, WriteBufAds+2, 8 );	
				break;	
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}		
	return ErrorByte;
}

//////////////////////////////////////////////////////////////////////////
//需要按照新的方法重新进行修改..
unsigned char ActionSecurity_setupClassData( short COM_No,unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[16];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method;
//	unsigned char* APPLState;
	
//	APPLState = CommPara[COM_No].APPLState;
		
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2B;
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	

 // if(( Method < 1 )||( Method > 6))  ErrorByte = other_reason;	
  //增加灵活ACTION 可选择..
  if(( *(Source+9) == 0x01 )&&( Method!=2)) Source = Source +1;
  else if(( *(Source+9) == 0x01 )&&( *(Source+10) == 0x01 )&&( *(Source+11) == 0x01 )) Source = Source +1;

 	switch( Method )
		{
			case 1:	
				if( *(Source+9) != D_ENUM ) { ErrorByte = type_unmatched; break; }
				E2P_RData(  Ptr, E2P_security_policy, 1 );
				if( *Ptr > *(Source+9)) { ErrorByte = type_unmatched; break; }
				E2P_WData( E2P_security_activate_data, Source+10, 1 );
				break;
#if 0
			case 2:	
				//
				if(( *(Source+9) == 0x01 )&&( *(Source+10) == 0x01 )&&( *(Source+11) == 0x02 )&&( *(Source+12) == 0x02 )&&( *(Source+13) == 0x16 )&&( *(Source+15) == 0x09) )
				{
					 		/*
							data ::= array key_transfer_data
							key_transfer_data ::= structure
							{
							key_id: enum:
							(0) global unicast encryption key,
							(1) global broadcast encryption key,
							(2) authentication key,
							(3) master key (KEK)
							key_wrapped: octet-string
							}
					 		*/
					 	/*					 
					 	if( *(Source+14) == 0x02)  E2P_WData( E2P_global_key_transfer_data, Source+17, 16 );      //原来错误,16--17..
					 	else 	ErrorByte = other_reason;	
					 	*/
					 	if( *(Source+14) == 0x02)
					 	{
					 		if( *(Source+16) == 0x10)
					 			  E2P_WData( E2P_global_key_transfer_data, Source+17, 16 );      //原来错误,16--17..
					 	  else ErrorByte = type_unmatched;
					 	}
					 	else
					 	{
					 	/*
					 		if(( *(Source+16) != 0x20 )||( *(Source+14) >= 4)) { ErrorByte = type_unmatched; break; }

							//操作ESAM芯片..
							if( Init_For_Process( ESAM ) != 0 ) 
							{ 
								ErrorByte = hardware_fault;		//芯片操作出错..	
								return ErrorByte;
							}
//							if( *(Source+14)==0) 	*(Source+14) = 0; 
//							else  if( *(Source+14)==1) 	*(Source+14) = 1; 
//							else if( *(Source+14)==3) 	*(Source+14) = 3; 
							ErrorByte = UpdateEnckey201603( Source+17, *(Source+14) ); //统一函数，fileno区分不同的密钥更新..
						 	if(ErrorByte!=0) 	 	ErrorByte = hardware_fault;		//芯片操作出错..			 	
						*/
					 	}	
					  
					  /*							 	
					 	switch( *(Source+14) )
						{
							case 0:	
								if( *(Source+16) != 0x20 ) { ErrorByte = type_unmatched; break; }
								
								break;
							case 2:			
					 			E2P_WData( E2P_global_key_transfer_data, Source+17, 16 );      //原来错误,16--17..
								break;
								
							default: 
								ErrorByte = other_reason;		
								break;	
						}
					*/		 	
				}else ErrorByte = other_reason;	
				break;
#endif
			case 2:	
				//
				if(( *(Source+9) == 0x01 )&&( *(Source+10) == 0x01 )&&( *(Source+11) == 0x02 )&&( *(Source+12) == 0x02 )&&( *(Source+13) == 0x16 )&&( *(Source+15) == 0x09)&&( *(Source+16) == 0x10) )
				{
				switch( *(Source+14) )
						{
							case 0: 	//(0) global unicast encryption key
								if( *(Source+16) != 0x20 ) { ErrorByte = type_unmatched; break; }
								
								break;
							case 1:  	//(1) global broadcast encryption key
								
								break;
							case 2:		// authentication key
								RAM_Fill(Ptr,16);
								if(Fun_Key_Unwrap_128Bits(crypt.EK,Source+17,Ptr)==0)
					 				E2P_WData( E2P_global_key_transfer_data, Ptr, 16 );      //原来错误,16--17..
					 			else
									ErrorByte = other_reason;
								break;
							case 3:		//(3) master key (KEK)

								break;
							default: 
								ErrorByte = other_reason;		
								break;	
						}
				}else ErrorByte = other_reason;	
				break;
	#if 0
			case 3:	   //更新 主站非对称密钥：2、	向芯片 -- 终端ESAM更新主站公钥 发送：80C9+00+91+81+Data (Data：92+公钥信息||签名值) 返回：9000
				//ErrorByte = 0x6A;
				if(( *(Source+9) == 0x09 )&&( *(Source+10) == 0x81 )&&( *(Source+11) == 0x80 ))    //长度>=128..
				{
						//操作ESAM芯片..
						if( Init_For_Process( ESAM ) != 0 ) 
						{ 
							ErrorByte = 0x69;		//芯片操作出错..	
							return ErrorByte;
						}
						ErrorByte = UpdateEnckey( Source+12, 2 ); //统一函数，fileno区分不同的密钥更新..
					 	if(ErrorByte!=0) 	 	ErrorByte = 0x69;		//芯片操作出错..			 	
				}else  ErrorByte = other_reason;			
				break;																	
			case 4:	//更新集中器中的非对称密钥对..
				//ErrorByte = 0x6A;	
				ErrorByte = 0x23;  //推到结果之后进行判断..
				break;
			case 5:	 //使用ECIES更新集中器中的全局密钥..
				//ErrorByte = 0x6A;	
				if(( *(Source+9) == 0x09 )&&( *(Source+10) == 0x81 )&&( *(Source+11) == 0x80 )) ErrorByte = 0x24;  //推到结果之后进行判断..
				else  ErrorByte = other_reason;	
				break;
			case 6:
				//0981C0
				if(( *(Source+9) == 0x09 )&&( *(Source+10) == 0x60 ))
				{
						//操作ESAM芯片..
						if( Init_For_Process( ESAM ) != 0 ) 
						{ 
							ErrorByte = 0x69;		//芯片操作出错..	
							return ErrorByte;
						}
						ErrorByte = UpdateKey( Source+11, 0 );
					 	if(ErrorByte!=0) 	 	ErrorByte = 0x69;		//芯片操作出错..			 	
				}else  ErrorByte = other_reason;			
				break;
			case 7:	   //更新 主控密钥：2、	向芯片 -- 发送：80C9030020+EncMasterkey；返回：0x9000				
				//ErrorByte = 0x6A;
				if(( *(Source+9) == 0x09 )&&( *(Source+10) == 0x20 ))
				{
						//操作ESAM芯片..
						if( Init_For_Process( ESAM ) != 0 ) 
						{ 
							ErrorByte = 0x69;		//芯片操作出错..	
							return ErrorByte;
						}
						ErrorByte = UpdateEnckey( Source+11, 1 ); //统一函数，fileno区分不同的密钥更新..
					 	if(ErrorByte!=0) 	 	ErrorByte = 0x69;		//芯片操作出错..			 	
				}else  ErrorByte = other_reason;			
				break;
	#endif														
			default: 
				ErrorByte = other_reason;		
				break;
		}
	return ErrorByte;
}	

short Judge_window_Date_time( unsigned char* Data )
{
	short i;
	unsigned char Item;
	unsigned char* Ptr;
	
	if(( *Data != Data_array )||( *(Data+1) > 5 )) 			//只接受小于5组执行时间
	{
		return other_reason;
	}
	Item = *(Data+1);
	Ptr = Data+2;
	for( i=0;i<Item;i++ )
	{
		if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 ))
		{
			return other_reason;
		}	
		if(( *(Ptr+2) != Data_octet_string )||( *(Ptr+3) != 12 )||( *(Ptr+16) != Data_octet_string )||( *(Ptr+17) != 12 )) 
		{
			return other_reason;
		}		
		Ptr += 30;		
	}	
    return 0;
}

//GPRS操作：
//28 Auto answer (class_id = 28, version = 0)
// Auto answer 0...n  class_id = 28, version = 0
//
//1. logical_name (static) octet-string x
//2. mode (static) enum x + 0x08
//3. listening_window (static) array x + 0x10
//4. status(dyn.) enum x + 0x18
//5. number_of_calls (static) unsigned x + 0x20
//6. number_of_rings (static)nr_rings_type x + 0x28
//7. list_of_allowed_callers (static) array x + 0x30
short Get_Auto_answer_setupClassData( unsigned char* Dest, unsigned char* Source )
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned short Temp,Addr,i;
	unsigned char* Point;
	//没有GPRS fire
	*Dest = Get_data_access_result; 				//异常标志
	*(Dest+1) = object_undefined;					//无对应的OBIS码
	Len = 2;

	Ptr = Buff;

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x02;
	Buff[3] = 0x02;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_ENUM;								
        *(Dest+2) = 200;
				Len = 3;
				break;	
			case 3:
				*(Dest+1) = Data_array;
				Addr = Auto_answer_listening_window_Number;
				E2P_RData( Dest+2, Addr, 1 );
				if( *(Dest+2) > 5 ) *(Dest+2) = 5;					//最多5个时间组
				Len = 3;
				Point = Dest+3;
				Addr += 2;						
				for( i=0;i<*(Dest+2);i++ )
				{				
					*Point  = Data_structure;
					*(Point+1) = 0x02;					
					*(Point+2) = Data_octet_string;
					*(Point+3) = 0x0c;
					E2P_RData( Point+4,  Addr, 12 );
					*(Point+16) = Data_octet_string;
					*(Point+17) = 0x0c;
					E2P_RData( Point+18,  Addr+13, 12 );
					Addr += 26;
					Len += 30;
					Point += 30;
			  }
				break;	
			case 4:
			  *(Dest+1) = D_ENUM;								
        *(Dest+2) = 1;
				Len = 3;
				break;	
			case 5:
				*(Dest+1) = D_Unsigned8;								//
				*(Dest+2) = 0;    //0 (No limit)
				Len = 3;
				break;	
			case 6:
				*(Dest+1) = Data_structure;
				*(Dest+2) = 0x02;
				*(Dest+3) = D_Unsigned8;
				*(Dest+4) = 0x01;
				*(Dest+5) = D_Unsigned8;
				*(Dest+6) = 0x01;
				Len = 7;	
				break;
			case 7:
				*(Dest+1) = Data_array;
				*(Dest+2) = 0x01;
				*(Dest+3) = Data_structure;
				*(Dest+4) = 0x02;
				*(Dest+5) = Data_octet_string;
				*(Dest+6) = 0x01;
				*(Dest+7) = 0x01;				
				*(Dest+8) = D_ENUM;
				*(Dest+9) = 0x01;
				Len = 10;	
				break;
									
			default: break;			
		}
	}	
	return Len;
	
}

unsigned char Set_Auto_answer_setupClassData( unsigned char *Source , unsigned short DataLen )
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short  Addr,i;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
//	unsigned char* DataType;
//	unsigned char AccessMode;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
//	DataType = WriteBufAds;
	
	Ptr = Buff;
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x02;
	Buff[3] = 0x02;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
			case 3:														// 
				//3. listening_window (static) array x + 0x10
			ErrorByte = Judge_window_Date_time( WriteBufAds );		//只支持5组执行时间
			if( ErrorByte == 0 )
			{
				Addr = Auto_answer_listening_window_Number;
				E2P_WData( Addr, WriteBufAds+1, 1 );				//arrays				
				Addr += 2;
				if(*(WriteBufAds+1)>5) *(WriteBufAds+1) = 5;  //如果大于5超出则只取前5个时间2016-02-05 7:05:20..
        Ptr = WriteBufAds+2;
				for( i=0;i<*(WriteBufAds+1);i++ )
				{
					E2P_WData( Addr+i*26, Ptr+4, 12 );	   //起始..
					E2P_WData( Addr+i*26+13, Ptr+18, 12 );	   //结束..
					Ptr += 30;
				}
			}			
				break;
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}	
	return ErrorByte;
	
}

//29  "Auto Connect" object (29, 0-0:2.2.1.255) ----DLMS上面应该是0.0.2.1.0.255
short Get_Auto_connect_setupClassData( unsigned char* Dest, unsigned char* Source )
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Len=2;
	unsigned char Attribute;
	unsigned short Temp,Addr,i;
	unsigned char* Point;
        
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;	
	Ptr = Buff;

	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x02;
	Buff[3] = 0x01;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
	
	if( Data_Comp( Ptr, Source+2, 6 ) != 0 ) 
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}	
	else
	{
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		switch( Attribute )
		{
			case 1:														//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				*(Dest+1) = D_ENUM;	
				Addr = Auto_connect_Mode;
				E2P_RData( Dest+2, Addr, 1 );
				//101 --- 104
				Len = 3;
				break;	
		 case 3:
		 		*(Dest+1) = D_Unsigned8;								//
				Addr = Auto_connect_repetitions;
				E2P_RData( Dest+2, Addr, 1 );		 						
				Len = 3;
				break;	
		 case 4:
		 		*(Dest+1) = D_Unsigned16;								//
				Addr = Auto_connect_repetition_delay;
				E2P_RData( Dest+2, Addr, 2 );		 						
				Len = 4;
				break;	
			case 5:
				*(Dest+1) = Data_array;
				Addr = Auto_answer_Calling_window_Number;
				E2P_RData( Dest+2, Addr, 1 );
				if( *(Dest+2) > 5 ) *(Dest+2) = 5;					//最多5个时间组
				Len = 3;
				Point = Dest+3;
				Addr += 2;						
				for( i=0;i<*(Dest+2);i++ )
				{				
					*Point  = Data_structure;
					*(Point+1) = 0x02;					
					*(Point+2) = Data_octet_string;
					*(Point+3) = 0x0c;
					E2P_RData( Point+4,  Addr, 12 );
					*(Point+16) = Data_octet_string;
					*(Point+17) = 0x0c;
					E2P_RData( Point+18,  Addr+13, 12 );
					Addr += 26;
					Len += 30;
					Point += 30;
			  }
				break;	
			case 6:
			  *(Dest+1) = Data_array;						
        *(Dest+2) = 1;
        *(Dest+3) = Data_octet_string;
	     Addr = Auto_connect_destination_list1;
			 E2P_RData( Dest+4, Addr, 1 );			 	     
	     if(*(Dest+4)>30) 
	     	{
			    *(Dest+4) =0;
			     Len = 5;
		    }
		    else
		    {
		    	  E2P_RData( Dest+5, Addr+2, *(Dest+4) );			
		    	  Len = 5 + *(Dest+4);
		    }
				// *(Dest+4) = 0x00;
				break;	
			default: break;			
		}
	}	
	return Len;
	
}
 
 //  "Auto Connect" object (29, 0-0:2.2.1.255) ----DLMS上面应该是0.0.2.1.0.255
unsigned char Set_Auto_connect_setupClassData( unsigned char *Source , unsigned short DataLen )
{

	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned short  Addr,i;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;
//	unsigned char* DataType;
//	unsigned char AccessMode;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
//	DataType = WriteBufAds;
	
	Ptr = Buff;
	Buff[0] = 0x00;				
	Buff[1] = 0x00;
	Buff[2] = 0x02;
	Buff[3] = 0x01;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		ErrorByte = object_undefined; 									//无对应的OBIS码
	}	
	else
	{
		switch( *Attribute )
		{
//			case 1:														//logical_name
//				break;	
			case 2:
			if( *WriteBufAds != D_ENUM ) 		
	    {
	    		return other_reason;
	    }
	    if(*(WriteBufAds+1)==101 )  //永久在线
	    {
	    	*Ptr = 1; //永久在线..1
	    	E2P_WData( Configuration, Ptr, 1 );	 
	    }
	    else
	    {
	    	 if((*(WriteBufAds+1)>=102 )&&(*(WriteBufAds+1)<=104))  //触发模式..
	    	 	{
	    	 			*Ptr = 0; //永久在线..
	    				E2P_WData( Configuration, Ptr, 1 );	 
	    	 	}
	    }
	     Addr = Auto_connect_Mode; 
		   E2P_WData( Addr, WriteBufAds+1, 1 );				//mode				
				//101 --- 104 
				break;	
			case 3:
				if( *WriteBufAds != D_Unsigned8 ) 		
		    {
		    		return other_reason;
		    }
	     Addr = Auto_connect_repetitions; 
		   E2P_WData( Addr, WriteBufAds+1, 1 );				//			
				break;					
			case 4:
				if( *WriteBufAds != D_Unsigned16 ) 		
		    {
		    		return other_reason;
		    }
	     Addr = Auto_connect_repetition_delay; 
		   E2P_WData( Addr, WriteBufAds+1, 2 );				//			
			 break;				
			case 5:														// 
				//3. listening_window (static) array x + 0x10
			ErrorByte = Judge_window_Date_time( WriteBufAds );		//只支持5组执行时间
			if( ErrorByte == 0 )
			{
				Addr = Auto_answer_Calling_window_Number;
				E2P_WData( Addr, WriteBufAds+1, 1 );				//arrays				
				Addr += 2;
				if(*(WriteBufAds+1)>5) *(WriteBufAds+1) = 5;  //如果大于5超出则只取前5个时间2016-02-05 7:05:20..
        Ptr = WriteBufAds+2;
				for( i=0;i<*(WriteBufAds+1);i++ )
				{
					E2P_WData( Addr+i*26, Ptr+4, 12 );	   //起始..
					E2P_WData( Addr+i*26+13, Ptr+18, 12 );	   //结束..
					Ptr += 30;
				}
			}			
				break;
			case 6:
				if(( *WriteBufAds != Data_array )||( *(WriteBufAds+1) != 1 )||( *(WriteBufAds+2) != Data_octet_string )) 		
		    {
		    		return other_reason;
		    }
	     Addr = Auto_connect_destination_list1;
	     if(*(WriteBufAds+3)<30) 
	     	{
		   			E2P_WData( Addr, WriteBufAds+3,1 );													//	
		   		  E2P_WData( Addr+2, WriteBufAds+4,*(WriteBufAds+3) );				//
		    }else return other_reason;			
				break;	
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}	
	return ErrorByte;
	
}

//CLASS ID:40类
unsigned short Get_PushsetupClassTabItemNo( unsigned char* Logical_Name )
{

	unsigned short i;
	for( i=0;i<DLMSPushSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_Push_Tab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}
	return i;	
	
}

short Get_PushsetupClassData( unsigned char* Dest, unsigned char* Source )
{

	short i;
	unsigned char Buff[30];
	unsigned char Logical_Name[6];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned short Addr;
//	unsigned char Len=2;
	unsigned short Len=2;
	unsigned char Attribute;

		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;
	Ptr = Logical_Name;
	RAM_Write( Ptr, Source+2, 6 );
	
	i = Get_PushsetupClassTabItemNo( Ptr );
	if( i == DLMSPushSum  )
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}
	else 
	{
		*Dest = Get_data;											//正常数据返回	
		Attribute = *(Source+2+6);						//请求的属性
		
		Addr = DLMS_Push_Tab[i].Addr; 
		     
		switch( Attribute )
		{
			case 1:										//logical_name
				*(Dest+1) = Data_octet_string;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
				
			case 2:										//buffer
			*(Dest+1) = Data_array;
			//数量..
			E2P_RData( Dest+2, Addr, 1 );
			if(*(Dest+2)>8) *(Dest+2) = 8;	
	    Addr +=2;
	    Len = 3;		
			Ptr = Buff;
			Point = Dest+3;
			for( i=0;i<*(Dest+2);i++ )
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
				Len += 18;
				Point += 18;					
			}					
        //PUSH-OBJECT
				break;	
			case 3:										//目标地址IP..
					*(Dest+1)  = Data_structure;
					*(Dest+2) = 0x03;					
					*(Dest+3) = D_ENUM;
					*(Dest+4) = 0x00;	
					*(Dest+5) = Data_octet_string;
					*(Dest+6) = 0x08;	
					Addr += Push_Setup_Scheduler1_destination;
					Addr -= Push_Setup_Scheduler1_list_NUM;
					E2P_RData( Dest+7, Addr+2, 8 );					
					*(Dest+15) = D_ENUM;
					*(Dest+16) = 128;	
					Len = 17;																		
				break;	
			case 4:
				*(Dest+1) = Data_array;
				Addr += Push_Setup_Scheduler1_window_Number;
				Addr -= Push_Setup_Scheduler1_list_NUM;
				E2P_RData( Dest+2, Addr, 1 );
				if( *(Dest+2) > 5 ) *(Dest+2) = 5;					//最多5个时间组
				Len = 3;
				Point = Dest+3;
				Addr += 2;						
				for( i=0;i<*(Dest+2);i++ )
				{				
					*Point  = Data_structure;
					*(Point+1) = 0x02;					
					*(Point+2) = Data_octet_string;
					*(Point+3) = 0x0c;
					E2P_RData( Point+4,  Addr, 12 );
					*(Point+16) = Data_octet_string;
					*(Point+17) = 0x0c;
					E2P_RData( Point+18,  Addr+13, 12 );
					Addr += 26;
					Len += 30;
					Point += 30;
			  }
				break;
			case 5:										//
		 		*(Dest+1) = D_Unsigned16;								//
				Addr += Push_Setup_Scheduler1_interval;
				Addr -= Push_Setup_Scheduler1_list_NUM;
				E2P_RData( Dest+2, Addr, 2 );		 						
				Len = 4;
				break;						
			case 6:
		 		*(Dest+1) = D_Unsigned8;								//
				Addr += Push_Setup_Scheduler1_repititions;
				Addr -= Push_Setup_Scheduler1_list_NUM;
				E2P_RData( Dest+2, Addr, 1 );		 						
				Len = 3;
				break;					
			case 7:										//
		 		*(Dest+1) = D_Unsigned16;								//
				Addr += Push_Setup_Scheduler1_repetition_delay;
				Addr -= Push_Setup_Scheduler1_list_NUM;
				E2P_RData( Dest+2, Addr, 2 );		 						
				Len = 4;
				break;	
			default:
				break;	
		}	
	}		
	return Len;
	
}	
 
unsigned char Set_PushsetupClassData( unsigned char *Source , unsigned short DataLen )
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
	
	i = Get_PushsetupClassTabItemNo( Logical_Name );
	if( i == DLMSPushSum ) { return object_undefined; }
	Addr = DLMS_Push_Tab[i].Addr; 
	
	switch( *Attribute )
	{
//		case 1:														//logical_name			
//			break;	
		case 2:														//capture_objects			
			if( *WriteBufAds != Data_array ) return other_reason;
			if( *(WriteBufAds+1) > 8 ) return other_reason;			//捕获对象数大于额定限数
		 	Ptr = WriteBufAds+2;
			for( i=0;i<*(WriteBufAds+1);i++ )
			{
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 4 )) return other_reason;
				if(( *(Ptr+2) != D_Unsigned16 )||( *(Ptr+5) != Data_octet_string )||( *(Ptr+6) != 6 )) return other_reason;
				if(( *Ptr+13 != D_Integer8 )||( *(Ptr+15) != D_Unsigned16 )) return other_reason; 
				Ptr += 18;		
			}	
		
			//对象分析合格..
     	//捕获数量..
     		E2P_WData( Addr, WriteBufAds+1, 1 );			
     		Addr += 2;     		
     		Ptr = Buff;
     		Point = WriteBufAds+2;
				for( i=0;i<*(WriteBufAds+1);i++ )
				{
					RAM_Write( Ptr, Point+3, 2 );					//class_id: 		long-unsigned,   
					RAM_Write( Ptr+2, Point+7, 6 );					//logical_name: 	octet-string,
					RAM_Write( Ptr+8, Point+14, 1 );				//attribute_index: 	integer,  
					RAM_Write( Ptr+9, Point+16, 2 );				//data_index: 		long-unsigned  
					E2P_WData( Addr, Ptr, 11 );						
					Addr += 12;
					Point += 18;
				}
			break;
	  case 3:														//send_destination_and_method
	  	// {0,Destination address,128} Destination address: Central System IP address + TCP/UDP port number + COSEM Management client wPort number (SAP Address)(001) 
     	if(( *WriteBufAds != Data_structure )||(*(WriteBufAds+1) != 0x03 )||(*(WriteBufAds+2) != Data_enum)||(*(WriteBufAds+4) != D_OctetStr)||(*(WriteBufAds+5) != 0x08)||(*(WriteBufAds+14) != Data_enum))
     	{
     		    		return other_reason;
     	}
     	
     		Addr += Push_Setup_Scheduler1_destination;
				Addr -= Push_Setup_Scheduler1_list_NUM;
				//
				E2P_WData( Addr+2, WriteBufAds+6, 8 );				//Destination address					//
     	
     	break; 		
		case 4:														//capture_period		
			ErrorByte = Judge_window_Date_time( WriteBufAds );		//只支持5组执行时间
			if( ErrorByte == 0 )
			{
				Addr += Push_Setup_Scheduler1_window_Number;
				Addr -= Push_Setup_Scheduler1_list_NUM;
				E2P_WData( Addr, WriteBufAds+1, 1 );				//arrays				
				Addr += 2;
				if(*(WriteBufAds+1)>5) *(WriteBufAds+1) = 5;  //如果大于5超出则只取前5个时间2016-02-05 7:05:20..
        Ptr = WriteBufAds+2;
				for( i=0;i<*(WriteBufAds+1);i++ )
				{
					E2P_WData( Addr+i*26, Ptr+4, 12 );	   //起始..
					E2P_WData( Addr+i*26+13, Ptr+18, 12 );	   //结束..
					Ptr += 30;
				}
			}	
			break;	
	 // 5 - 6 -7 ..
		case 5:										//D_Unsigned16				
				// *(Dest+1) = D_ENUM;
			if( *WriteBufAds != D_Unsigned16 ) 		
		    {
		    		return other_reason;
		    }
			Addr += Push_Setup_Scheduler1_interval;
			Addr -= Push_Setup_Scheduler1_list_NUM;
	     
		   E2P_WData( Addr, WriteBufAds+1, 2 );				//			
			 break;	
		case 6:										//D_Unsigned8				
				// *(Dest+1) = D_ENUM;
			if( *WriteBufAds != D_Unsigned8 ) 		
		    {
		    		return other_reason;
		    }
			Addr += Push_Setup_Scheduler1_repititions;
			Addr -= Push_Setup_Scheduler1_list_NUM;
	     
		   E2P_WData( Addr, WriteBufAds+1, 1 );				//			
			 break;	
		case 7:										//D_Unsigned16				
				// *(Dest+1) = D_ENUM;
			if( *WriteBufAds != D_Unsigned16 ) 		
		    {
		    		return other_reason;
		    }
			Addr += Push_Setup_Scheduler1_repetition_delay;
			Addr -= Push_Setup_Scheduler1_list_NUM;
	     
		   E2P_WData( Addr, WriteBufAds+1, 2 );				//			
			 break;				 	
		default: 
			ErrorByte = read_write_denied;
			break;			
	} 
	return ErrorByte;
	
}	

