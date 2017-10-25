#include "TypeMeter.h"
#include "TypeE2p.h"
//#include "Measure.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"
#include "Mem.h"

//#include "interrupt.h"
#include "Time.h"
#include "Power.h"
#include "Initial.h"

//#include "RN8213.h"

#include "IECComm.h"
#include "ApplComm.h"
#include "COSEMComm.h"
//#include "Esamcpu.h"
//#include "AESGCM.H"
#include "aes_example.h"

void Creat_config_program_record( void )
{
	unsigned long Value;
	
	E2P_RData( (unsigned char*)&Value, No_of_config_program, 4 );	
	Value += 1;
	E2P_WData( No_of_config_program, (unsigned char*)&Value, 4 );	
	E2P_WData( Date_of_last_config_program, &Clk.YearH, 12 );			
}	

unsigned short CreatExceptionResponseAPDU( unsigned char* Dest )
{
	*Dest = APDU_EXCEPTION_response;
	*(Dest+1) = service_error;
	*(Dest+2) = service_not_supported;	
	return 3;
}	

short IsUIFrame( short COM_No )
{
  	unsigned char* ComBuf;
  	unsigned char ControlByte;
  	unsigned char ClientAddr;
	
  	ComBuf = CommPara[COM_No].Buf;
        
    ClientAddr = *(ComBuf+HDLC_RClientAddr);
  	
		ControlByte = *(ComBuf+HDLC_CtlPtr);
	
		if(ClientAddr==0x53) return 0x53; 				//GPRS模块读表数据特定客户端地址sky20160106..
			
		if(( ControlByte == HDLC_UI )||( ControlByte == HDLC_UI_N )) return 0;
		else return -1;
}	

unsigned short APPLProcess( unsigned char COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short i;
	unsigned char Buff[10];
	unsigned char *Ptr;
	short Len = 0;
	
	Ptr = Buff;
	
	switch( *RecBuff )
	{
	    case APDU_AARQ:   //分低级及高级加密..
	    			crypt.aarqHLS[COM_No] = 0;
	    		  Len = AARQHProcess( COM_No, RecBuff, SendBuff );
//						Len = AARQProcess( COM_No, RecBuff, SendBuff );				
						if( Len == -1 )
						{
							Len = CreatExceptionResponseAPDU( SendBuff );				
						}	
						break;
	    case RLRQ_APDU:			
					Len = RLRQProcess( COM_No, RecBuff, SendBuff );				
					if( Len == -1 )
					{
						Len = CreatExceptionResponseAPDU( SendBuff );				
					}	
					break;			
			case APDU_GET_Request:					
				//	if(( IsUIFrame( COM_No ) != 0 )&&(( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 )))		//GET功能是否支持	//UI帧直通
//				if(( IsUIFrame( COM_No ) != 0x53 )&&(( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 )))		//GET功能是否支持	//0x53直通..
//				if(( IsUIFrame( COM_No ) != 0x53 )&&(( *CommPara[COM_No].APPLState == APPLIdle )||(((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 )&&(HComm.ClientAddr[COM_No]!=0x10)&&(*CommPara[COM_No].APPLState == APPLON ))))		//GET功能是否支持	//0x53直通..
			//	if(( IsUIFrame( COM_No ) != 0x53 )&&(( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 )))		//GET功能是否支持	//0x53直通..
			if(( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x10 ) == 0 ))
					{
						Len = CreatExceptionResponseAPDU( SendBuff );				
					}	
					else 
					{
						//查读是否允许读..
						/* Ptr = RecBuff+3;
						
						if(( *(RecBuff+1)== get_request_normal)&&((PIN_SETEN & P_SETEN ) != 0 )){ 
								if((HComm.ClientAddr[COM_No]==0x66)&&(GetRPR_clientNO( Ptr )==0xffff))
								{							  
										Len = CreatExceptionResponseAPDU( SendBuff );			
								}else
								{
									//查读是否允许读..
									//Ptr = RecBuff+3;
									if(HComm.ClientAddr[COM_No]==0x10){
											i = GetRPublic_clientNO( Ptr );										
									}
									if(i==0xffff){
										 Len = CreatExceptionResponseAPDU( SendBuff );
										 break;
									}else{
												if(HComm.ClientAddr[COM_No]==0x02)
												{
														i = GetRReading_clientNO( Ptr );										
												}
												if(i==0xffff)
													Len = CreatExceptionResponseAPDU( SendBuff );	
												else 	
													Len = GETProcess( COM_No, RecBuff, SendBuff );
											}
									}
								}
						else
							{ 
								 Len = GETProcess( COM_No, RecBuff, SendBuff );
							}
							*/
							 Len = GETProcess( COM_No, RecBuff, SendBuff );
					}	
					break;
				case APDU_SET_Request:					
		//			if( *CommPara[COM_No].APPLState == APPLIdle )
					//	if(HComm.ClientAddr[COM_No]==0x10)..
					if((HComm.ClientAddr[COM_No]==0x10)||(HComm.ClientAddr[COM_No]==0x02)||( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x08 ) == 0 ))		//SET功能是否支持
					{
						Len = CreatExceptionResponseAPDU( SendBuff);				
					}	
					else 
					{
							if(( *(RecBuff+1)==set_request_normal))
						///	if(( *(RecBuff+1)==set_request_normal)&&((PIN_SETEN & P_SETEN ) != 0 ))
							{   
								//查读是否允许写..
							/*	Ptr = RecBuff+3;
								if(HComm.ClientAddr[COM_No]==0x66){
									i = GetWPR_clientNO( Ptr );										
								}
								if(i==0xffff){
									 Len = CreatExceptionResponseAPDU( SendBuff );
									 break;
								}
								*/
								Len = SETProcess( COM_No, RecBuff, SendBuff );
								if( *(SendBuff+3) == success ) Creat_config_program_record();
						}else{
								Len = SETProcess( COM_No, RecBuff, SendBuff );
								if( *(SendBuff+3) == success ) Creat_config_program_record();
						}	
						
					///	Len = SETProcess( COM_No, RecBuff, SendBuff );
					///			if( *(SendBuff+3) == success ) Creat_config_program_record();
		//				Len = CreatExceptionResponseAPDU( SendBuff );				
					}	
					break;
			 case APDU_EVENT_NOTIFICATION_Request:
				///	EVENT_NOTIFICATIONProcess( COM_No, RecBuff, SendBuff );					
					break;	
			 case APDU_ACTION_Request: 				
		//			if( *CommPara[COM_No].APPLState == APPLIdle )
					//15类读取列表对所有权限均支持..
					if((*(RecBuff+1)==action_request_normal)&&(*(RecBuff+3)==0)&&(*(RecBuff+4)==0x0F))	
					{
							Len = ACTIONProcess( COM_No, RecBuff, SendBuff );	
							//return Len;
					} // sky20150309..
					else
					{
								if((HComm.ClientAddr[COM_No]==0x10)||(HComm.ClientAddr[COM_No]==0x02)||( *CommPara[COM_No].APPLState == APPLIdle )||((*(CommPara[COM_No].Conformance+2) & 0x01 ) == 0 ))		//ACTION功能是否支持
								{
									Len = CreatExceptionResponseAPDU( SendBuff);				
								}	
								else 
								{			if(( *(RecBuff+1)==action_request_normal))
										///	if(( *(RecBuff+1)==action_request_normal)&&((PIN_SETEN & P_SETEN ) != 0 ))
												{ 	
													//查读是否允许Action..
												/*	Ptr = RecBuff+3;
													if(HComm.ClientAddr[COM_No]==0x66){
															i = GetAPR_clientNO( Ptr );										
													}
													if(i==0xffff){
														 Len = CreatExceptionResponseAPDU( SendBuff );
														 break;
													}		
													*/
													Len = ACTIONProcess( COM_No, RecBuff, SendBuff );	
										  }else Len = ACTIONProcess( COM_No, RecBuff, SendBuff );					
								}
					}	
					break;
				case GLO_GET_Request:

					break;
				case GLO_SET_Request:
				
					break;
				case GLO_EVENT_NOTIFICATION_Request:
				
					break;
				case GLO_ACTION_Request:
				
					break;
				case General_SIGNING:
				
					break;
					
		//		case APDU_AARE;							
		//		case APDU_GET_Response;   				
		//		case APDU_SET_Response;   				
		//		case APDU_ACTION_Response;
				default: 
					Len = CreatExceptionResponseAPDU( SendBuff );				
					break;								
	}	
	
	return Len;
}

//取BIT STRING流的最低BIT位是否为1
unsigned char GetSTRINGLowestBIT( unsigned char* Data, short Len )
{
	unsigned char BitCt;
	unsigned char LowestByte;
	
	if( Len == 0 ) return 0;
	
//	BitCt = *Data;
	BitCt = 7;						//Test
		
//	LowestByte = *(Data+Len-1);
	LowestByte = *(Data+Len+1);
	
	LowestByte = LowestByte >> BitCt;
	if( LowestByte & 0x01 ) return 1;
	else return 0;
	
}	

void FillApplicationContextName( unsigned char* Dest )
{
	*Dest = 0xA1;
	*(Dest+1) = 0x09;
	*(Dest+2) = 0x06;
	*(Dest+3) = 0x07;
	*(Dest+4) = 0x60;
	*(Dest+5) = 0x85;
	*(Dest+6) = 0x74;
	*(Dest+7) = 0x05;
	*(Dest+8) = 0x08;
	*(Dest+9) = 0x01;
	*(Dest+10) = 0x01;
}	

short CompareApplicationContextName( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	
	Ptr = Buff;
	ICEC.use_HLSname = 0;
	FillApplicationContextName( Ptr );
	
	if( Data_Comp( Source, Ptr, 10 ) != 0 ) return 1;
	if(*(Source+10)==0x03)
		{
			ICEC.use_HLSname = 3;
			 //带加密认证name..
			 return 0;;
			}
//	if( Data_Comp( Source, Ptr, 11 ) != 0 ) return 1;
//	else return -1;
	if(*(Source+10)==0x01)
		{
			ICEC.use_HLSname = 1;
			//无加密..
			 return 0;;
			}
	else return 1;
	
}	

void FillMechanism_name( unsigned char* Dest )
{
	*Dest = 0x8B;
	*(Dest+1) = 0x07;
	*(Dest+2) = 0x60;
	*(Dest+3) = 0x85;
	*(Dest+4) = 0x74;
	*(Dest+5) = 0x05;
	*(Dest+6) = 0x08;
	*(Dest+7) = 0x02;
	*(Dest+8) = 0x01;
}	

short CompareMechanism_name( unsigned char* Source )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	
	Ptr = Buff;
	FillMechanism_name( Ptr );
	
	crypt.choice_algorithm = 0;
	
	if( Data_Comp( Source, Ptr, 8 ) != 0 ) return 1;
	
	ICEC.use_SHA256 = 0x0;
	crypt.choice_algorithm = *(Source+8);		
	//01--LLS; 02--HLS(ESAM硬加密);	05--HLS(软加密)	
	if(*(Source+8)==0x01) return 0;
	if((*(Source+8)==0x02)||(*(Source+8)==0x05)) return 0x55;
	if(*(Source+8)==0x04)
	{
		 ICEC.use_SHA256 = 0x1;
		 return 0x55;	
	}		
	if(*(Source+8)!=0x05) return 1;
	// crypt.aarqHLS = 0x55;
	else return 0x55;
			
//	if( Data_Comp( Source, Ptr, 9 ) != 0 ) return 1;
//	else return 0;
}	

//初始密码先统一改为123456
short CompareAuthentication_value( short COM_No,unsigned char* Source )
{
	unsigned char Buff[10];
	unsigned char Buff2[10];
	unsigned char Buff3[10];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char* password;
	short Len;

	Ptr = Buff;
	Point = Buff2;
	password = Buff3;
	
	Len = *(Source+1);
	if( Len > 10 ) return 1;
	if( *(Source+2) != 0x80 ) return 1;
	if( *(Source+3) != ( Len - 2 ) ) return 1;
	Len -= 2;
	
	RAM_Fill( Point, 10 );
	RAM_Write( Point, Source+4, Len );
///	#if ( IranDemo == YesCheck )
///		RAM_Write( Ptr, Point, 8 );	
///	#else
	//	if((PIN_SETEN & P_SETEN ) == 0 )			//超级跳线，内部使用..
	//	{
	//		RAM_Write( Ptr, Point, 8 );	
	//	}	
	//	else
	//	{
			//临时增加超级密码：88935860
			*password = '8';
			*(password+1) = '8';
			*(password+2) = '9';
			*(password+3) = '3';
			*(password+4) = '5';
			*(password+5) = '8';
			*(password+6) = '6';
			*(password+7) = '0';
			if( Data_Comp( Point, password, 8 ) == 0 ) return 0;		//超级密码，临时增加..
			// E2P_RData( Ptr, ELLS_secret1, 8 );
	//		if((HComm.ClientAddr[COM_No]==0x10)||(HComm.ClientAddr[COM_No]==0x11)||(HComm.ClientAddr[COM_No]==0x13)||(HComm.ClientAddr[COM_No]==0x01))
			//if((HComm.ClientAddr[COM_No]==0x01)||((HComm.ClientAddr[COM_No]==0x11)&&(COM_No==0)))
			if((HComm.ClientAddr[COM_No]==0x01)||(HComm.ClientAddr[COM_No]==0x11))
			{
				E2P_RData( Ptr, ELLS_secret2, 8 );
			}else{
				if(HComm.ClientAddr[COM_No]==0x13)
				{
				
				E2P_RData( Ptr, ELLS_secret4, 8 );
					//	 *Ptr = '1';
					//	*(Ptr+1) = '2';
					//	*(Ptr+2) = '3';
					//	*(Ptr+3) = '4';
					//	*(Ptr+4) = '5';
					//	*(Ptr+5) = '6';
					//	*(Ptr+6) = 0;
					//	*(Ptr+7) = 0;
				}else{
						if(HComm.ClientAddr[COM_No]==0x12)
							{
							E2P_RData( Ptr, ELLS_secret3, 8 );
							//		 *Ptr = '1';
							//		*(Ptr+1) = '2';
							//		*(Ptr+2) = '3';
							//		*(Ptr+3) = '4';
							//		*(Ptr+4) = '5';
							//		*(Ptr+5) = '6';
							//		*(Ptr+6) = '7';
							//		*(Ptr+7) = '8';
							}else{
										if(HComm.ClientAddr[COM_No]==0x10)
										{
										return 0;
										/*
												*Ptr = '1';
												*(Ptr+1) = '2';
												*(Ptr+2) = '3';
												*(Ptr+3) = '4';
												*(Ptr+4) = '5';
												*(Ptr+5) = '6';
												*(Ptr+6) = 0;
												*(Ptr+7) = 0;
											*/	
											}
										
									}
							} 
			}		
	//	}	
///	#endif
	if( Data_Comp( Point, Ptr, 8 ) != 0 )
		{
		E2P_RData((unsigned char*)&ICComm.SHEZHIErrTime,Wrong_password_times_disable_Com,1);
		
			if((COM_No==1)||(COM_No==0))
			{
				ICComm.CommErrTime[COM_No]++;
					if(COM_No==1)
					{//串口1 2016-8-17-fire 红外口
						E2P_WData( COMMERRTIMES1, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
						if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME1,(unsigned char*)&Clk.YearH,12 );
					}
					else
					{
						//串口0 2016-8-17-fire 485口
						E2P_WData( COMMERRTIMES2, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
						if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME2, (unsigned char*)&Clk.YearH,12 );
					}
					if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)
						{
							E2P_RData((unsigned char*)&Para.Comm_LockTime,Disable_Com_Duration,2);
							if(Para.Comm_LockTime==0xFFFF)
								Para.Comm_LockTime = 0;
						}
			}
		 return 1;
		}
	else
		{
			if((COM_No==1)||(COM_No==0))
			{
				if(ICComm.CommErrTime[COM_No]!=0)
				{
					ICComm.CommErrTime[COM_No] = 0;
					if(COM_No==1)
					{	//串口1 2016-8-17-fire 红外口
						E2P_WData( COMMERRTIMES1, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
						//if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME1,(unsigned char*)&Clk.YearH,12 );
					}
					else
					{	//串口0 2016-8-17-fire 485口
						E2P_WData( COMMERRTIMES2, (unsigned char*)&ICComm.CommErrTime[COM_No],1 );
						//if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)  E2P_WData( COMMERRDateTIME2, (unsigned char*)&Clk.YearH,12 );
					}
				}
			}
		 return 0;
		}
	
}

//short GetComformanceState( short COM_No, unsigned char* Source )
short GetComformanceState( short COM_No, unsigned char* Source, unsigned char SecretState )

{
	unsigned char Buff[3];
	unsigned char* Ptr;
	unsigned char* Conformance;
	
	Ptr = Buff;
	
	if( SecretState == 0x00 )					//无身份认证
	{
		Buff[0] = 0x00;
//		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11、BIT12
		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)];		//BIT11、BIT12
//		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)]+ByteBit[7-(Conformance_action%8)];		//BIT19、20、21、23
		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_selective_access%8)];		//BIT19、20、21、23
	}
	else										//LLS最低安全身份认证
	{
		if((HComm.ClientAddr[COM_No]==0x02)||(HComm.ClientAddr[COM_No]==0x10))
		{
				Buff[0] = 0x00;
				Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11
				Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)];		//BIT19、20、21
		}
	  else
	  {
				Buff[0] = 0x00;
				Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11
				Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)]+ ByteBit[7-(Conformance_action%8)];		//BIT19、20、21、23
		}
	}	
	*Source &= *Ptr;
	*(Source+1) &= *(Ptr+1);
	*(Source+2) &= *(Ptr+2);

	Conformance = CommPara[COM_No].Conformance;
	*(Conformance+3) = SecretState;
	
//	if( IsAllData( Ptr, 3, 0x00 ) == 0 )
	if( IsAllData( Source, 3, 0x00 ) == 0 )
	{
//		RAM_Write( Conformance, Ptr, 3 );
		RAM_Write( Conformance, Source, 3 );
		return 1;
	}
	else
	{	
		RAM_Write( Conformance, Source, 3 );
		return 0;	
	}	
}
#if 0
{
	unsigned char Buff[3];
	unsigned char* Ptr;
	unsigned char* Conformance;
	
	Ptr = Buff;

	if( SecretState == 0x00 )					//无身份认证
	{
		Buff[0] = 0x00;
//		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11、BIT12
		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)];		//BIT11、BIT12
//		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)]+ByteBit[7-(Conformance_action%8)];		//BIT19、20、21、23
		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_selective_access%8)];		//BIT19、20、21、23
	}
	else										//LLS最低安全身份认证
	{
		Buff[0] = 0x00;
		Buff[1] = ByteBit[7-(Conformance_block_transfer_with_get%8)]+ByteBit[7-(Conformance_block_transfer_with_set%8)];		//BIT11
		Buff[2] = ByteBit[7-(Conformance_get%8)]+ByteBit[7-(Conformance_set%8)]+ByteBit[7-(Conformance_selective_access%8)]+ByteBit[7-(Conformance_action%8)];		//BIT19、20、21、23
	}	
	
	*Source &= *Ptr;
	*(Source+1) &= *(Ptr+1);
	*(Source+2) &= *(Ptr+2);

	Conformance = CommPara[COM_No].Conformance;
	*(Conformance+3) = SecretState;
	
//	if( IsAllData( Ptr, 3, 0x00 ) == 0 )
	if( IsAllData( Source, 3, 0x00 ) == 0 )
	{
//		RAM_Write( Conformance, Ptr, 3 );
		RAM_Write( Conformance, Source, 3 );
		return 1;
	}
	else
	{	
		RAM_Write( Conformance, Source, 3 );
		return 0;	
	}	
}	 
#endif
//返回User_Systemtile长度
short	CreatUser_Systemtile( short COM_No, unsigned char* Dest, unsigned short ErrByte )	
{
	unsigned short Len=12;
	
//	Ptr = Dest;
	
	*Dest = 0xA4;					// user-information字段组件(component [30], Context-specific)标记的编码
	*(Dest+1) = 0x0A;					// 标记的组件值字段的长度的编码
									//-- application-context-name组件(OCTET STRING)的编码
	*(Dest+2) = 0x04;				// user-information (OCTET STRING, Universal)选择的编码
	*(Dest+3) = 0x08;				// OCTET STRING值字段的长度的编码(4个八位字节)
			
	RAM_Write( Dest+4, ICComm.SystemTitle, 8 );
	return Len;
}
	
//返回User_information长度
short	CreatUser_information( short COM_No, unsigned char* Dest, unsigned short ErrByte )	
{
	unsigned short Len=8;
	unsigned char Buff[50];
	unsigned char *Ptr;
	unsigned short Len_cipher = 0;
	Ptr = Buff;
//	Ptr = Dest;
	
	*Dest = 0xBE;					// user-information字段组件(component [30], Context-specific)标记的编码
	*(Dest+1) = 0x06;					// 标记的组件值字段的长度的编码
									//-- application-context-name组件(OCTET STRING)的编码
	*(Dest+2) = 0x04;				// user-information (OCTET STRING, Universal)选择的编码
	*(Dest+3) = 0x04;				// OCTET STRING值字段的长度的编码(4个八位字节)
	
	if( ErrByte == APPL_OPEN_9 )
	{
												//-- xDLMS-Initiate.response PDU的A-XDR编码
		*(Dest+4) = 0x0E;						// DLMS PDU选项表中confirmedServiceError的标记（显式标记）的编码
												//-- confirmedServiceError组件
		*(Dest+5) = CSError_initiateError;		//confirmedServiceError中的1   initiateError [1] ServiceError
		*(Dest+6) = SError_initiate;			//ServiceError 中的 6       inttiate [6]  初始化服务错误 
		*(Dest+7) = SError6_other;
	}
	else if( ErrByte == APPL_OPEN_12_S1 )
	{
		*(Dest+4) = 0x0E;
		*(Dest+5) = CSError_initiateError;
		*(Dest+6) = SError_initiate;
		*(Dest+7) = SError6_dlms_version_too_low;		
	}		
	else if( ErrByte == APPL_OPEN_14_S2 )
	{
		*(Dest+4) = 0x0E;
		*(Dest+5) = CSError_initiateError;
		*(Dest+6) = SError_initiate;
		*(Dest+7) = SError6_pdu_size_too_short;				
	}		
	else
	{
//			if(ICEC.useDK!=1)
//			{
					*(Dest+1) = 0x10;			// 标记的组件值字段的长度的编码
					*(Dest+3) = 0x0E;			// OCTET STRING值字段的长度的编码(14个八位字节)	
										//-- xDLMS-Initiate.response PDU的A-XDR编码
					*(Dest+4) = 0x08;			// DLMS PDU选项表中InitiateResponse的标记（显式标记）的编码
										//-- negotiated-quality-of-service组件(可选项, 不出现)
					*(Dest+5) = 0x00;			// proposed-quality-of-service组件的使用标记(FALSE, 不出现)
										//-- negotiated-dlms-version-number组件的编码(Unsigned8, value=6)
					*(Dest+6) = 0x06;			// 类型为Unsigned8的数据的A-XDR编码就是它的值
										//-- 一致性块[APPLICATION 31] IMPLICIT BITSTRING (SIZE(24))的编码
					*(Dest+7) = 0x5F;
					*(Dest+8) = 0x1F;			// [APPLICATION 31] 标记 (ASN.1 显式标记)的编码
					*(Dest+9) = 0x04;			// 'contents' 字段长度（用八位字节数表示）的编码（4个八位字节）
					*(Dest+10) = 0x00;			// 位串的最后一个八位字节没有使用的比特数的编码
					RAM_Write( Dest+11, CommPara[COM_No].Conformance, 3 );		// 固定长度位串值的编码
					if(COM_No==2)//2016-8-17-fire
					{
							//-- server-max-receive-pdu-size组件的编码(Unsigned16, value=128)
//							*(Dest+14) = MAX_INFO_SIZE / 256;	
//							*(Dest+15) = MAX_INFO_SIZE % 256;				// 类型为Unsigned16的数据的A-XDR编码就是它的值
							*(Dest+14) = GPRS_MAX_INFO_SIZE / 256;	
							*(Dest+15) = GPRS_MAX_INFO_SIZE % 256;				// 类型为Unsigned16的数据的A-XDR编码就是它的值
				  }
				  else
				  {
				  		//-- server-max-receive-pdu-size组件的编码(Unsigned16, value=512)
							*(Dest+14) = MAX_PDU_SIZE / 256;	
							*(Dest+15) = MAX_PDU_SIZE % 256;				// 类型为Unsigned16的数据的A-XDR编码就是它的值
				  }
			//							//-- server-max-receive-pdu-size组件的编码(Unsigned16, value=512)
			//		*(Dest+14) = MAX_PDU_SIZE / 256;	
			//		*(Dest+15) = MAX_PDU_SIZE % 256;	// 类型为Unsigned16的数据的A-XDR编码就是它的值
										//-- VAA-Name组件的编码 (Unsigned16, 使用LN引用时值=0x0007，使用SN引用时值= FA 00)
					*(Dest+16) = 0x00;
					*(Dest+17) = 0x07;			// 类型为Unsigned16的数据的A-XDR编码就是它的值
					Len = 18;
//			}
//			else
//			{
//		  }
	}
	/*不带加密，加密部分，先注释掉
	// cipher..
	if(ICEC.use_cipher_information == 0x21) //加密返回..
	{
//		 Len_cipher = Gcm128EnDecrypt( ESAM, Ptr,Dest+4, 0 ,crypt.glo_FC[0],1, 14 );
//		 *(Dest+1)  =  Len_cipher + 9;
//		 *(Dest+3)  =  Len_cipher + 7;
//		 *(Dest+4)  = 0x28;
//		 *(Dest+5)  = Len_cipher + 5;
//		 RAM_Write( Dest+6, crypt.glo_FC, 5 );
//		 RAM_Write( Dest+11, Ptr, Len_cipher );
//		 Len = Len_cipher+11;	
//
  unsigned char TEMP_DK;
  TEMP_DK = ICEC.use_DK;
  ICEC.use_DK = 0;
		 	if( Init_For_Process( ESAM ) != 0 ) 
			{ 
					     ICEC.EsamOK = 0;
					     //return 2;   
								//Flag.MeterState1 |= F_ESAMErr;				//电表运行状态1
			} 
		 Len_cipher = Gcm128EnDecrypt( ESAM, Ptr,Dest+4, 0 ,crypt.glo_FC[0],1, 14 );
		 *(Dest+1)  =  Len_cipher + 9;
		 *(Dest+3)  =  Len_cipher + 7;
		 *(Dest+4)  = 0x28;
		 *(Dest+5)  = Len_cipher + 5;
		 RAM_Write( Dest+6, crypt.glo_FC, 5 );
		 RAM_Write( Dest+11, Ptr, Len_cipher );
		 Len = Len_cipher+11;	

			ICEC.use_DK = TEMP_DK; 
			
			if((ICEC.use_DK ==1)&&(ICComm.SaveDk[0]<=50))
			{
			 	if( Init_For_Process( ESAM ) != 0 ) 
					{ 
							     ICEC.EsamOK = 0;
							     //return 2;   
										//Flag.MeterState1 |= F_ESAMErr;				//电表运行状态1
					}	

					Gcm128EnDecrypt( ESAM, &ICComm.SaveDk[1],&ICComm.SaveDk[1], 1 ,crypt.glo_FC[0],0x55, ICComm.SaveDk[0] );   //采用0x55表示采用DK模式..			  
		  }
	//	 Gcm128EnDecrypt( ESAM, Ptr,Ptr+7, 1 ,crypt.glo_FC[0],0x55, ((*(Ptr+1))-5) );   //采用0x55表示采用DK模式..
		 //保存DK传输时的密钥..下次建立DK连接使用..
	//	 ICComm.SaveDk[0] = ((*(Ptr+1))-5);
	//	 if(ICComm.SaveDk[0]<=50)  RAM_Write(&ICComm.SaveDk[1],Ptr+7,ICComm.SaveDk[0]);
			 		 
		 	 
	}
	*/
	return Len;
}	


////short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length )	
//short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char SecretState )	
//{
//	unsigned char Len;
//	unsigned short ErrByte = 0;
//	unsigned char* Ptr;
//	unsigned short Temp;
//
//	Ptr = Source;
//	Ptr++;
//	Len = *Ptr;
//	*Length = Len + 2;
//	Ptr++;
//	if( *Ptr != 0x04 ) return APPL_OPEN_9;		//04  // user-information (OCTET STRING, Universal)选择的编码
//	Len -= 2;
//	Ptr++;
//	if( *Ptr != Len ) return APPL_OPEN_9;		// OCTET STRING值字段的长度的编码(n个八位字节)
//	Ptr++; Len--;
//	if( *Ptr != 0x01 ) return APPL_OPEN_9;		//01   // DLMS PDU选项表中InitiateRequest的标记（显式标记）的编码
//	Ptr++; Len--;
////	if( *Ptr != 0 )	return APPL_OPEN_9;			//应为00    // dedicated-key组件的使用标志(FALSE, 应该不出现)
////	Ptr++; Len--;
////2015-03-12 2:15:39增加dedicated-key组件的使用标志判断..
//	if( *Ptr == 0x01 )
//	{
//		Ptr++; Len--;
//		if(Len<28) return APPL_OPEN_9;	  //DK为固定16字节..
//		if((*Ptr == 0x09 )&&(*(Ptr+1) == 0x10 ))
//		{
//				RAM_Write( ICEC.d_key, Ptr+2, 0x10 );
//				Ptr += 18;
//				Len -= 18;
//				ICEC.use_DK = 1;
//		}else return APPL_OPEN_9;	 
//		
//	}
//	else
//	{
//			if( *Ptr != 0 )	return APPL_OPEN_9;			//不为01 则应为00    // dedicated-key组件的使用标志(FALSE, 应该不出现)
//	    Ptr++; Len--;
//	    ICEC.use_DK =0;
//	}												// response-allowed组件的使用标志(TRUE)	
//	Ptr++; Len--;										
//	if( *Ptr != 0 ) { Ptr++; Len--; }			//01 proposed-quality-of-service组件的使用标志(TRUE,出现)
//	Ptr++; Len--;								//01 proposed-quality-of-service数据项的编码 (1)	
//	if( *Ptr < 6 ) return APPL_OPEN_12_S1;		//proposed-dlms-version-number组件的编码(Unsigned8, value=6)
//	Ptr++; Len--;
//	if( *Ptr != 0x5F ) return APPL_OPEN_9;
//	if( Len == 8 ) 
//	{
//		Ptr++;
//		if( *Ptr != 0x1F ) return APPL_OPEN_9;	
//	}
//	Ptr++;										 //4 'contents' 字段长度（用八位字节数表示）的编码（4个八位字节）
//	Ptr++;										 // 位串的最后一个八位字节没有使用的比特数的编码
//	Ptr++;										 // 位串
////	if( GetComformanceState( COM_No, Ptr) != 0 ) return APPL_OPEN_13_S3;
//	if( GetComformanceState( COM_No, Ptr, SecretState ) != 0 ) return APPL_OPEN_13_S3;
//	Ptr += 3;
//	Temp = *Ptr;
//	Ptr++;
//	Temp *= 256;
//	Temp += *Ptr;
//	if( Temp < 12 ) return APPL_OPEN_14_S2;		//测试跳过	//13.03.29	
//	return ErrByte;
//	
//}	

//short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length )	
short	ProcessUser_information( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char SecretState )	
#if 0
{
	unsigned char Len;
	unsigned short ErrByte = 0;
	unsigned char* Ptr;
	unsigned short Temp;

	ICEC.use_DK = 0;
       ICEC.use_cipher_information = 0;
	
	Ptr = Source;
	Ptr++;
	Len = *Ptr;
	*Length = Len + 2;
	Ptr++;
	if( *Ptr != 0x04 ) return APPL_OPEN_9;		//04  // user-information (OCTET STRING, Universal)选择的编码
	Len -= 2;
	Ptr++;
	if( *Ptr != Len ) return APPL_OPEN_9;		// OCTET STRING值字段的长度的编码(n个八位字节)
	Ptr++; Len--;
	//带DK参数，用0x21---33..
/*	不带加密
if( *Ptr == 0x21)
	{
		 ICEC.use_cipher_information = 0x21;  //带加密..
			// 更新systemtitle和 AAD(AK)..
			RAM_Write( crypt.glo_FC,Ptr+2, 5 );
//			E2P_RData( ICComm.SystemTitle,E2P_server_system_title, 8 );   //服务器的systile..
			//传递AAD--AK
			E2P_RData( ICComm.SysAAD,E2P_global_key_transfer_data, 16 );   //AK..
		  //解密..
		 //====
		 //初始化..
		 	if( Init_For_Process( ESAM ) != 0 ) 
			{ 
				ICEC.EsamOK = 0;
				if( Card_CoolRST( ESAM ) != 0 ) 
				{ 
					ICEC.EsamOK = 0;
					//return 1;
				}				
			}
		 	if( Init_For_Process( ESAM ) != 0 ) 
			{ 
					     ICEC.EsamOK = 0;
					     //return 2;   
								//Flag.MeterState1 |= F_ESAMErr;				//电表运行状态1
			}
		 //保存DK传输时的密钥..下次建立DK连接使用..
		 ICComm.SaveDk[0] = ((*(Ptr+1))-5);
		 if(ICComm.SaveDk[0]<=50)  RAM_Write(&ICComm.SaveDk[1],Ptr+7,ICComm.SaveDk[0]);
		 	//建立AARQ,AARE都要用全局密钥要进行..
			  
		 Gcm128EnDecrypt( ESAM, Ptr,Ptr+7, 1 ,crypt.glo_FC[0],0x55, ((*(Ptr+1))-5) );   //采用0x55表示采用DK模式..
		 //Gcm128EnDecrypt( ESAM, Ptr,Ptr+7, 1 ,crypt.glo_FC[0],0x55, ((*(Ptr+1))-5) );   //采用0x55表示采用DK模式..
	}
	*/
	if( *Ptr != 0x01 ) return APPL_OPEN_9;		//01   // DLMS PDU选项表中InitiateRequest的标记（显式标记）的编码
	Ptr++; Len--;
//	if( *Ptr != 0 )	return APPL_OPEN_9;			//应为00    // dedicated-key组件的使用标志(FALSE, 应该不出现)
//	Ptr++; Len--;
//2015-03-12 2:15:39增加dedicated-key组件的使用标志判断..
	if( *Ptr == 0x01 )
	{
		Ptr++; Len--;
		if(Len<27) return APPL_OPEN_9;	  //DK为固定16字节..
		//if((*Ptr == 0x09 )&&(*(Ptr+1) == 0x10 ))
		if(*Ptr == 0x10 )
		{
				RAM_Write( ICEC.d_key, Ptr+1, 0x10 );
				Ptr += 17;
				Len -= 17;
				ICEC.use_DK = 1;
		}else return APPL_OPEN_9;
	}
	else
	{
			if( *Ptr != 0 )	return APPL_OPEN_9;			//不为01 则应为00    // dedicated-key组件的使用标志(FALSE, 应该不出现)
	    Ptr++; Len--;
	    ICEC.use_DK =0;
	}												// response-allowed组件的使用标志(TRUE)	
	Ptr++; Len--;										
	if( *Ptr != 0 ) { Ptr++; Len--; }			//01 proposed-quality-of-service组件的使用标志(TRUE,出现)
	Ptr++; Len--;								//01 proposed-quality-of-service数据项的编码 (1)	
	if( *Ptr < 6 ) return APPL_OPEN_12_S1;		//proposed-dlms-version-number组件的编码(Unsigned8, value=6)
	Ptr++; Len--;
	if( *Ptr != 0x5F ) return APPL_OPEN_9;
        //if( Len == 8 )
	if( Len!= 7 ) 
	{
		Ptr++;
		if( *Ptr != 0x1F ) return APPL_OPEN_9;	
	}
	Ptr++;										 //4 'contents' 字段长度（用八位字节数表示）的编码（4个八位字节）
	Ptr++;										 // 位串的最后一个八位字节没有使用的比特数的编码
	Ptr++;										 // 位串
//	if( GetComformanceState( COM_No, Ptr) != 0 ) return APPL_OPEN_13_S3;
	if( GetComformanceState( COM_No, Ptr, SecretState ) != 0 ) return APPL_OPEN_13_S3;
	Ptr += 3;
	Temp = *Ptr;
	Ptr++;
	Temp *= 256;
	Temp += *Ptr;
	if( Temp < 12 ) return APPL_OPEN_14_S2;		//测试跳过	//13.03.29	
	return ErrByte;
	
}
#endif

{
	unsigned char Len;
	unsigned short ErrByte = 0;
	unsigned char* Ptr;
	unsigned short Temp;

	Ptr = Source;
	Ptr++;
	Len = *Ptr;
	*Length = Len + 2;
	Ptr++;
	if( *Ptr != 0x04 ) return APPL_OPEN_9;		//04  // user-information (OCTET STRING, Universal)选择的编码
	Len -= 2;
	Ptr++;
	if( *Ptr != Len ) return APPL_OPEN_9;		// OCTET STRING值字段的长度的编码(n个八位字节)
	Ptr++; Len--;
	if( *Ptr != 0x01 ) return APPL_OPEN_9;		//01   // DLMS PDU选项表中InitiateRequest的标记（显式标记）的编码
	Ptr++; Len--;
	if( *Ptr != 0 )	return APPL_OPEN_9;			//应为00    // dedicated-key组件的使用标志(FALSE, 应该不出现)
	Ptr++; Len--;
												// response-allowed组件的使用标志(TRUE)	
	Ptr++; Len--;										
	if( *Ptr != 0 ) { Ptr++; Len--; }			//01 proposed-quality-of-service组件的使用标志(TRUE,出现)
	Ptr++; Len--;								//01 proposed-quality-of-service数据项的编码 (1)	
	if( *Ptr < 6 ) return APPL_OPEN_12_S1;		//proposed-dlms-version-number组件的编码(Unsigned8, value=6)
	Ptr++; Len--;
	if( *Ptr != 0x5F ) return APPL_OPEN_9;
	if( Len == 8 ) 
	{
		Ptr++;
		if( *Ptr != 0x1F ) return APPL_OPEN_9;	
	}
	Ptr++;										 //4 'contents' 字段长度（用八位字节数表示）的编码（4个八位字节）
	Ptr++;										 // 位串的最后一个八位字节没有使用的比特数的编码
	Ptr++;										 // 位串
//	if( GetComformanceState( COM_No, Ptr) != 0 ) return APPL_OPEN_13_S3;
	if( GetComformanceState( COM_No, Ptr, SecretState ) != 0 ) return APPL_OPEN_13_S3;
	Ptr += 3;
	Temp = *Ptr;
	Ptr++;
	Temp *= 256;
	Temp += *Ptr;
	if( Temp < 12 ) return APPL_OPEN_14_S2;		//测试跳过	//13.03.29	
	
	return ErrByte;
	
}	


//short	ProcessAARQItem( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length )	
short	ProcessAARQItem( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char* SecretState )	
{
	unsigned char Len;
	unsigned short ErrByte = 0;
	//unsigned char *AAPtr;
		
	switch( *Source )
	{
		case AARQ_protocol_version:					//0x80	//IMPLICIT BIT STRING {version1 (0)} DEFAULT {version1}
			Len = *(Source+1); 
			if( GetSTRINGLowestBIT( Source, Len ) == 0 )	//最低BIT为0，异常
			{
				ErrByte = APPL_OPEN_4_S2;				
			}
			*Length = *(Source+1) + 2; 
			break;
		case AARQ_application_context_name:			//0xA1
			if( CompareApplicationContextName( Source ) != 0 ) ErrByte = APPL_OPEN_5_S2;				
//			else
//			{
//				if(*(Source+10)==3) *(Dest+10) = *(Source+10);
//			}
			*Length = 11;
			break;
		case AARQ_called_AP_title:					//0xA2	//OPTIONAL
		case AARQ_called_AE_qualifier:				//0xA3	//OPTIONAL	
		case AARQ_called_AP_invocation_id:			//0xA4	//OPTIONAL
		case AARQ_called_AE_invocation_id:			//0xA5	//OPTIONAL
		case AARQ_calling_AE_qualifier:				//0xA7	//OPTIONAL	
		case AARQ_calling_AP_invocation_id:			//0xA8	//OPTIONAL
		case AARQ_calling_AE_invocation_id:			//0xA9	//OPTIONAL
			*Length = *(Source+1) + 2;
			break;
	 case AARQ_calling_AP_title:					//0xA6	//OPTIONAL
	 	  //AAPtr = ICComm.ClientSystemTitle;
	 		E2P_RData( ICComm.ClientSystemTitle, E2P_client_system_title, 8 );			//读取服务器的SYSTEM TITLE ..
		   if( Data_Comp( ICComm.ClientSystemTitle, Source+4, 8 ) != 0 )
		   {
		   	   RAM_Write(ICComm.ClientSystemTitle,Source+4, 8 );
		   	   E2P_WData( E2P_client_system_title, ICComm.ClientSystemTitle,8 );
		   } 
	 		*Length = *(Source+1) + 2;
	 		// 错误处理需要增加..
	 		
			break;		
		//-- 如果只使用内核，下面的字段不出现。 		
		case AARQ_sender_acse_requirements:			//0x8A	//IMPLICIT OPTIONAL
			Len = *(Source+1); 
			if( GetSTRINGLowestBIT( Source, Len ) == 0 )	//最低BIT为1，使用安全认证
		///	if( GetSTRINGLowestBIT( Source, Len ) == 1 )	//最低BIT为1，使用安全认证  2016-8-25-fire 改为1 APPL_OPEN_7 第1条通过
			{
				ErrByte = APPL_OPEN_7_S1;				
			}
			else 
				{
				*SecretState |= 0x01;
			///	ErrByte = APPL_OPEN_7_S3;//2016-8-25-fire
				}
//			*SecretState |= 0x80;
			*Length = *(Source+1) + 2;
			break;
		 //-- 如果选用身份验证单元，下面的字段应出现
		case AARQ_mechanism_name:					//0x8B	//IMPLICIT OPTIONAL
			if(( *SecretState & 0x01 ) != 0 ) 
			{
				//if( CompareMechanism_name( Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;				
				// else *SecretState |= 0x02;
				ErrByte = CompareMechanism_name( Source );
				if(ErrByte == 0x55){
					 crypt.aarqHLS[COM_No] = 0x55;
					 ErrByte = 0;
				 }
				if( ErrByte != 0 )
				{
					 ErrByte = APPL_OPEN_7_S1;
					 crypt.choice_algorithm = 0;
				}				
				else *SecretState |= 0x02;
				// HLS..
							
			}	
			else ErrByte = APPL_OPEN_7_S1; 				
			*Length = *(Source+1) + 2;
			break;
		 //-- 如果选用身份验证单元，下面的字段应出现
		case AARQ_calling_authentication_value:		//0xAC	//EXPLICIT OPTIONAL
			if(( *SecretState & 0x03 ) == 0x03 ) 
			{
				if(crypt.aarqHLS[COM_No] != 0x55){
						if( CompareAuthentication_value( COM_No,Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;
						else *SecretState |= 0x04;
				}else{
						if( CompareAuthentication_HLS( Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;
						else *SecretState |= 0x04;
				}					
			}
			else ErrByte = APPL_OPEN_7_S1;				
			*Length = *(Source+1) + 2;
			break;
		case AARQ_implementation_information:		//0xBD	//IMPLICIT OPTIONAL
			ErrByte = APPL_OPEN_8;				
			*Length = *(Source+1) + 2;
			break;
		case AARQ_user_information:					//0xBE	//IMPLICIT OPTIONAL
//			if(( *SecretState != 0x80 )&&( *SecretState != 0x0F )) ErrByte = APPL_OPEN_7_S1;				
//			if(( *SecretState != 0x00 )&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;				
			if((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x21 )&&( *SecretState != 0x07 )) 
				ErrByte = APPL_OPEN_7_S1;		//客户机地址为0x10,	MechanismName = NO_SECURITY	
		///	if((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x23 )&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;		//客户机地址为0x11,	MechanismName = NO_SECURITY	 2016-8-25-fire
//			if(((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x03 )||(*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x21 ))&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;		//客户机地址为0x10,	MechanismName = NO_SECURITY	
			else
			{
//				ErrByte = ProcessUser_information( COM_No, Dest, Source, Length );
				ErrByte = ProcessUser_information( COM_No, Dest, Source, Length, *SecretState );
			}	
			break;
    default:
    		ErrByte = APPL_OPEN_8;
    		break;	
	}	
	return ErrByte;
}	

/*
	short ProcessAARQItem( short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short* Length, unsigned char* SecretState )	
	{
	unsigned char Len;
	unsigned short ErrByte = 0;
	
	switch( *Source )
	{
		case AARQ_protocol_version:					//0x80	//IMPLICIT BIT STRING {version1 (0)} DEFAULT {version1}
			Len = *(Source+1); 
			if( GetSTRINGLowestBIT( Source, Len ) == 0 )	//最低BIT为0，异常
			{
				ErrByte = APPL_OPEN_4_S2;				
			}
			*Length = *(Source+1) + 2;
			break;
		case AARQ_application_context_name:			//0xA1
			if( CompareApplicationContextName( Source ) != 0 ) ErrByte = APPL_OPEN_5_S2;				
			*Length = 11;
			break;
		case AARQ_called_AP_title:					//0xA2	//OPTIONAL
		case AARQ_called_AE_qualifier:				//0xA3	//OPTIONAL	
		case AARQ_called_AP_invocation_id:			//0xA4	//OPTIONAL
		case AARQ_called_AE_invocation_id:			//0xA5	//OPTIONAL
		case AARQ_calling_AP_title:					//0xA6	//OPTIONAL
		case AARQ_calling_AE_qualifier:				//0xA7	//OPTIONAL	
		case AARQ_calling_AP_invocation_id:			//0xA8	//OPTIONAL
		case AARQ_calling_AE_invocation_id:			//0xA9	//OPTIONAL
			*Length = *(Source+1) + 2;
			break;
		//-- 如果只使用内核，下面的字段不出现。 		
		case AARQ_sender_acse_requirements:			//0x8A	//IMPLICIT OPTIONAL
			Len = *(Source+1); 
			if( GetSTRINGLowestBIT( Source, Len ) == 0 )	//最低BIT为1，使用安全认证
			{
				ErrByte = APPL_OPEN_7_S1;				
			}
			else *SecretState |= 0x01;
//			*SecretState |= 0x80;
			*Length = *(Source+1) + 2;
			break;
		 //-- 如果选用身份验证单元，下面的字段应出现
		case AARQ_mechanism_name:					//0x8B	//IMPLICIT OPTIONAL
			if(( *SecretState & 0x01 ) != 0 ) 
			{
				if( CompareMechanism_name( Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;
				else *SecretState |= 0x02;		
			}	
			else ErrByte = APPL_OPEN_7_S1; 				
			*Length = *(Source+1) + 2;
			break;
		 //-- 如果选用身份验证单元，下面的字段应出现
		case AARQ_calling_authentication_value:		//0xAC	//EXPLICIT OPTIONAL
			if(( *SecretState & 0x03 ) == 0x03 ) 
			{
				if( CompareAuthentication_value( COM_No,Source ) != 0 ) ErrByte = APPL_OPEN_7_S1;
				else *SecretState |= 0x04;					
			}
			else ErrByte = APPL_OPEN_7_S1;				
			*Length = *(Source+1) + 2;
			break;
		case AARQ_implementation_information:		//0xBD	//IMPLICIT OPTIONAL
			ErrByte = APPL_OPEN_8;				
			*Length = *(Source+1) + 2;
			break;
		case AARQ_user_information:					//0xBE	//IMPLICIT OPTIONAL
//			if(( *SecretState != 0x80 )&&( *SecretState != 0x0F )) ErrByte = APPL_OPEN_7_S1;				
//			if(( *SecretState != 0x00 )&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;				
			if((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x21 )&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;		//客户机地址为0x10,	MechanismName = NO_SECURITY	
//			if(((*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x03 )||(*(CommPara[COM_No].Buf+HDLC_RClientAddr) != 0x21 ))&&( *SecretState != 0x07 )) ErrByte = APPL_OPEN_7_S1;		//客户机地址为0x10,	MechanismName = NO_SECURITY	
			else
			{
//				ErrByte = ProcessUser_information( COM_No, Dest, Source, Length );
				ErrByte = ProcessUser_information( COM_No, Dest, Source, Length, *SecretState );
			}	
			break;
    default:
    		ErrByte = APPL_OPEN_8;
    		break;	
	}	
	return ErrByte;
}	
*/
short GetAAREResult_Diagnostic(  short COM_No, unsigned char* Dest, unsigned short RejectType )
{	
	unsigned char Buff[20];
	unsigned char *Ptr;
	short len;
	Ptr = Buff;
	
	*Dest = 0xA2;   	// result 组件 (component [2], Context-specific)的标记和长度的编码
	*(Dest+1) = 0x03;   // 标记的组件值字段的长度的编码
						//-- result-component (INTEGER)的编码
	*(Dest+2) = 0x02;   // result (INTEGER, Universal)选择的编码
	*(Dest+3) = 0x01;  	// result值字段的长度的编码(1个八位字节)
	*(Dest+4) = Result_accepted_0;  	// Result (accepted (0))值的编码
						//-- result-source-diagnostic (tagged component [3])的编码
	*(Dest+5) = 0xA3;   // result-source-diagnostic组件(component [3], Context-specific)标记的编码
	*(Dest+6) = 0x05;   // 标记的组件值字段的长度的编码
	*(Dest+7) = Diagnostic_acse_service_user;  	// acse-service-user 选项(1)标记的编码 
	*(Dest+8) = 0x03;  	// 标记的组件值字段的长度的编码
						//-- result-source-diagnostics组件(INTEGER)的编码
	*(Dest+9) = 0x02;  	// result-source-diagnostics (INTEGER, Universal)选择的编码
	*(Dest+10) = 0x01;  // 值字段长度的编码 (1个八位字节)
	*(Dest+11) = Diagnostic_null_0;  // 值的编码：null (0)
	switch( RejectType )
	{
		case APPL_OPEN_4_S2:	
			*(Dest+4) = Result_rejected_permanent_1;  				// Result (rejected-permanent (1))值的编码
			*(Dest+7) = Diagnostic_acse_service_provider;  			// acse-service-provider 选项(2)标记的编码 
			*(Dest+11) = Diagnostic_no_common_acse_version_2;  		// 值的编码：no-common-acse-version (2)
			break;
		case APPL_OPEN_5_S2:	
		case APPL_OPEN_5_S3:	
			*(Dest+4) = Result_rejected_permanent_1;  				// Result (rejected-permanent (1))值的编码
			*(Dest+7) = Diagnostic_acse_service_user;  				// acse-service-user 选项(1)标记的编码 
			*(Dest+11) = Diagnostic_application_context_name_not_supported_2;  	// 值的编码：application-context-name-not-supported (2)
			break;
		case APPL_OPEN_7_S1:	
		case APPL_OPEN_7_S2:	
		case APPL_OPEN_7_S3:	
		case APPL_OPEN_7_S4:	
		case APPL_OPEN_7_S5:	
		case APPL_OPEN_7_S6:	
		case APPL_OPEN_7_S7:	
		case APPL_OPEN_7_S8:	
			*(Dest+4) = Result_rejected_permanent_1;  				// Result (rejected-permanent (1))值的编码
			*(Dest+7) = Diagnostic_acse_service_user;  				// acse-service-user 选项(1)标记的编码 
			*(Dest+11) = Diagnostic_authentication_failure_13;  	// 值的编码：authentication-failure (13)
			break;
		case APPL_OPEN_8:				
		case APPL_OPEN_9:			
		case APPL_OPEN_12_S1:
		case APPL_OPEN_13_S3:
		case APPL_OPEN_14_S2:
			*(Dest+4) = Result_rejected_permanent_1;  				// Result (rejected-permanent (1))值的编码
			*(Dest+7) = Diagnostic_acse_service_user;  				// acse-service-user 选项(1)标记的编码 
			*(Dest+11) = Diagnostic_no_reason_given_1;  			// 值的编码：no-reason-given (1)
			break;
        default: break;
	}
	len = 12;
	if((crypt.aarqHLS[COM_No]== 0x55)&&(RejectType==0)){
			*(Dest+11) = 0x0e;  
			 // 0e 88 02 07 80 89 07 60 85 74 05 08 02 05 aa 0a 80 08 50 36 77 52 4a 32 31 46
			 *(Dest+12) = 0x88; 
			 *(Dest+13) = 0x02; 
			 *(Dest+14) = 0x07; 
			 *(Dest+15) = 0x80; 
			 *(Dest+16) = 0x89; 
			 *(Dest+17) = 0x07; 
			 *(Dest+18) = 0x60; 
			 *(Dest+19) = 0x85; 
			 *(Dest+20) = 0x74; 
			 *(Dest+21) = 0x05; 
			 *(Dest+22) = 0x08; 			 
			 *(Dest+23) = 0x02; 
			 //*(Dest+24) = 0x05;
			 *(Dest+24) = crypt.choice_algorithm;
			  			 
			 *(Dest+25) = 0xaa; 
			 *(Dest+26) = 0x0a; 
			 *(Dest+27) = 0x80; 
			 *(Dest+28) = 0x08;
			 // 随机数暂时固定..以下是8个字节的随机数..
			 if(crypt.Challenge_Len==8)
			 {
			 /*
					//取ESAM产生的随机数..
					GetChallenge( ESAM, Ptr, 8 );
				  RAM_Write( Dest+29, Ptr,  8 );  
				  //保存服务器随机数..
				  RAM_Write( crypt.S_Challenge, Ptr,  8 );
			 		///return 37;
			 		 	len = 37;		
			 	  return len;
			 		*/
		   }
		   else
		   {  //Challenge_Len==16
		   		*(Dest+26) = 0x12; 
		   		*(Dest+28) = 0x10; 
		   		RAM_Write( Dest+29, crypt.C_Challenge,  16 );  
				  //保存服务器随机数..
				  RAM_Write( crypt.S_Challenge, Dest+29,  16 );
				  	len = 45;		
			 	  return len;
		   }  			 
			 /* 
			 *(Dest+29) = 0x50; 
			 *(Dest+30) = 0x36; 
			 *(Dest+31) = 0x77; 
			 *(Dest+32) = 0x52; 			 
			 *(Dest+33) = 0x4a; 
			 *(Dest+34) = 0x32; 
			 *(Dest+35) = 0x31; 
			 *(Dest+36) = 0x46;
			*/
	}
	else 
		len = 12;
			return len;
}	

//short AARQProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
//{
//	unsigned short* RecDataPtr;
//	short Length;
//	unsigned char* Source;
//	unsigned short SendLength=0;
//	unsigned char* Dest;
//	unsigned short ItemLen;
//	unsigned short ErrByte;
//	unsigned char* APPLState;
//	unsigned char SecretState;
//	
//	RecDataPtr = CommPara[COM_No].RecDataPtr;
//	APPLState = CommPara[COM_No].APPLState;
//	Length = *RecDataPtr - 3;
//	Length -= 2;
//	
//	if( *(RecBuff+1) != Length ) return -1;			//收到的长度和信息帧中的长度不等，非法
//
//	*SendBuff = APDU_AARE;
//	*(SendBuff+1) = 0;
//	FillApplicationContextName( SendBuff+2 );
//	SendLength = 11;
//	
//	Dest = SendBuff+13;
//	
//	Source = RecBuff+2;
//	ItemLen = 0;
//	SecretState = 0;
//	while( Length > 0 )
//	{
////		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen );	
//		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen, &SecretState );	
//		if( ErrByte != 0 )
//		{
//			GetAAREResult_Diagnostic( Dest, ErrByte );
//			Dest += 12;
//			SendLength += 12;
//			break;
//		}	
//		Length -= ItemLen;
//		Source += ItemLen;
//	}
//	if( ErrByte == 0 )
//	{
//		GetAAREResult_Diagnostic( Dest, ErrByte );
//		Dest += 12;
//		SendLength += 12;
//	}		
//	SendLength += CreatUser_information( COM_No, Dest, ErrByte );
//	*(SendBuff+1) = SendLength;
//	
//	if( ErrByte != 0 ) *APPLState = APPLIdle;			//未建立应用连接
//	else *APPLState = APPLON; 							//建立应用连接	
//
//	return SendLength+2;	
//}					

short RLRQProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short* RecDataPtr;
	short Length;
	//unsigned char* Source;
	unsigned short SendLength=0;
	//unsigned char* Dest;
	//unsigned short ItemLen;
	unsigned short ErrByte=0;
	unsigned char* APPLState;
	//unsigned char SecretState;
	
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	APPLState = CommPara[COM_No].APPLState;
	Length = *RecDataPtr - 3;
	Length -= 2;
	
	//if( *(RecBuff+1) != Length ) return -1;			//收到的长度和信息帧中的长度不等，非法

///----------------------------------------------
	*SendBuff = RLRE_APDU;
		*(SendBuff+1) = 0x03;
		*(SendBuff+2) = 0x80;	
		*(SendBuff+3) = 0x01;		
		*(SendBuff+4) = 0x00;
    SendLength = 3;
    
	SendLength += CreatUser_information( COM_No, SendBuff+5, ErrByte );
	*(SendBuff+1) = SendLength;
	
	//SendLength = 0;
//	*(SendBuff+1) = 0;
//	FillApplicationContextName( SendBuff+2 );
//	SendLength = 11;
//	
//	Dest = SendBuff+13;
//	
//	Source = RecBuff+2;
//	ItemLen = 0;
//	SecretState = 0;
//	while( Length > 0 )
//	{
////		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen );	
//		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen, &SecretState );	
//		if( ErrByte != 0 )
//		{
//			GetAAREResult_Diagnostic( Dest, ErrByte );
//			Dest += 12;
//			SendLength += 12;
//			break;
//		}	
//		Length -= ItemLen;
//		Source += ItemLen;
//	}
//	if( ErrByte == 0 )
//	{
//		GetAAREResult_Diagnostic( Dest, ErrByte );
//		Dest += 12;
//		SendLength += 12;
//	}		
//	SendLength += CreatUser_information( COM_No, Dest, ErrByte );
//	*(SendBuff+1) = SendLength;
//	
//	if( ErrByte != 0 ) *APPLState = APPLIdle;			//未建立应用连接
//	else *APPLState = APPLON; 							//建立应用连接	
//
//	return SendLength+2;

	ICEC.use_DK = 0;
	*APPLState = APPLIdle;

	return SendLength+2;	
}					
/*
//权限管理。。
//公共权限0x10..
unsigned short GetRPublic_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<RPublic_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)RPublic_client[i].OBIS_Name, 9 ) == 0 ) break;				//查找对应的OBIS码
	}
	if(i==RPublic_clientSum) i = 0xFFFF;	 //总数不能超过0xffff..
	return i;	
}	
//
//只读权限02..
unsigned short GetRReading_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<RReading_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)RReading_client[i].OBIS_Name, 9 ) == 0 ) break;				//查找对应的OBIS码
	}
	if(i==RReading_clientSum) i = 0xFFFF;	 //总数不能超过0xffff..
	return i;	
}	


unsigned short GetRPR_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<RPR_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)RPR_client[i].OBIS_Name, 9 ) == 0 ) break;				//查找对应的OBIS码
	}
	if(i==RPR_clientSum) i = 0xFFFF;	 //总数不能超过0xffff..
	return i;	
}
	
//只可设权限..
unsigned short GetWPR_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<WPR_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)WPR_client[i].OBIS_Name, 9 ) == 0 ) break;				//查找对应的OBIS码
	}
	if(i==WPR_clientSum) i = 0xFFFF;	 //总数不能超过0xffff..
	return i;	
}

//只可ACTION权限..
unsigned short GetAPR_clientNO( unsigned char* OBIS_Name )
{
	unsigned short i;
		
	for( i=0;i<APR_clientSum;i++ )
	{
		if( Data_Comp( OBIS_Name, (unsigned char*)APR_client[i].OBIS_Name, 9 ) == 0 ) break;				//查找对应的OBIS码
	}
	if(i==APR_clientSum) i = 0xFFFF;	 //总数不能超过0xffff..
	return i;	
}	
*/

short AARQHProcess( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff )
{
	unsigned short* RecDataPtr;
	short Length;
	unsigned char* Source;
	unsigned short SendLength=0;
	unsigned char* Dest;
	unsigned short ItemLen;
	unsigned short ErrByte=0;
	unsigned char* APPLState;
	unsigned char SecretState;
	
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	APPLState = CommPara[COM_No].APPLState;
	Length = *RecDataPtr - 3;
	Length -= 2;
	
	if( *(RecBuff+1) != Length ) return -1;			//收到的长度和信息帧中的长度不等，非法

	*SendBuff = APDU_AARE;
	*(SendBuff+1) = 0;
	FillApplicationContextName( SendBuff+2 );
	SendLength = 11;
	
	Dest = SendBuff+13;
	
	//if(HComm.ClientAddr[COM_No]!=0x10)  // 非公共权限读取项目..
//		{
				Source = RecBuff+2;
				ItemLen = 0;
				SecretState = 0;
				while( Length > 0 )
				{
			//		ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen );	
					ErrByte = ProcessAARQItem( COM_No, Dest, Source, &ItemLen, &SecretState );	
					if( ErrByte != 0 )
					{
						GetAAREResult_Diagnostic( COM_No,Dest, ErrByte );
						Dest += 12;
						SendLength += 12;
						break;
					}	
					Length -= ItemLen;
					Source += ItemLen;
				}		
				if( ErrByte == 0 )
				{
					GetAAREResult_Diagnostic( COM_No,Dest, ErrByte );
					if(crypt.aarqHLS[COM_No]== 0x55){
						 if(ICEC.use_SHA256 == 0x1)
						 	{
								Dest += 45;
								SendLength += 45;
						  }
						  else
							{
								Dest += 37;
								SendLength += 37;
							}
				 }else{
				 			Dest += 12;
							SendLength += 12;
					}
				}	
//	}

	//增加systemtitle	
	if(ICEC.use_HLSname == 3)
	{
		  *(SendBuff+12) = 0x03;
		  SendLength += CreatUser_Systemtile( COM_No, Dest, ErrByte );   //03--增加
      Dest += 12;       
  }
  else
  {
  	  // AARQ结束读一下CLIENT_SYSTEMTILE：
  		E2P_RData( ICComm.ClientSystemTitle, E2P_client_system_title, 8 );			
  }
			
	SendLength += CreatUser_information( COM_No, Dest, ErrByte );
	*(SendBuff+1) = SendLength;
	
	if( ErrByte != 0 ) *APPLState = APPLIdle;			//未建立应用连接
	else *APPLState = APPLON; 							//建立应用连接	
  
  //DK-FC AARQ之后复位后从0开始..
  HComm.DK_FCRXCount = 0;
  HComm.DK_FCTXCount = 0;
  
	return SendLength+2;	
}	

short CompareAuthentication_HLS( unsigned char* Source )
{
	unsigned char Buff[20];
	unsigned char Buff2[10];
	//unsigned char Buff3[10];
	unsigned char* Ptr;
	unsigned char* Point;
	// unsigned char* password;
	short Len;

	Ptr = Buff;
	Point = Buff2;
	//password = Buff3;
	
	Len = *(Source+1);
	//if( Len != 10 ) return 1;
	if( Len > 18 ) return 1;
	
	if( *(Source+2) != 0x80 ) return 1;
	if( *(Source+3) != ( Len - 2 ) ) return 1;
	Len -= 2;
	
	crypt.Challenge_Len = Len;
	//目前只支持8和16字节..
	if(Len == 8 )
	{
		//保存客户机随机数..
		RAM_Write( crypt.C_Challenge, Source+4, 8 );   //保存随机数,计算TAG进行比对..
	}
	else
	{
		  if(Len != 16 ) return 1;		
			RAM_Write( crypt.C_Challenge, Source+4, 16 );   //保存随机数,计算TAG进行比对..
  }
	
	/*	//取ESAM产生的随机数..
		GetChallenge( ESAM, Ptr, 8 );
		
	  RAM_Write( Source+4, Ptr,  8 );  
	  //保存服务器随机数..
	  RAM_Write( crypt.S_Challenge, Ptr,  8 );  
	*/
	
	/*
		RAM_Fill( Point, 10 );

		RAM_Write( Point, Source+4, Len );   //保存随机数,计算TAG进行比对..
		//计算TAG..
	  //TEMP NULL
		//

		// 返回8位随机数.. 50 36 77 52 4a 32 31 46 
	  *(Ptr) = 0x50;
	  *(Ptr+1) = 0x36;  
	  *(Ptr+2) = 0x77;
	  *(Ptr+3) = 0x52;
	  *(Ptr+4) = 0x4a;
	  *(Ptr+5) = 0x32;
	  *(Ptr+6) = 0x31;
	  *(Ptr+7) = 0x46;
	  RAM_Write( Source+4, Ptr,  8 );  
  */      
	return 0;
}

// 加密解密调用程序..
// 长度的地方不能超过0x80，规则还是要遵守20160309修改于伊朗(长帧)..
int Decrypt_OR_Encrypt( unsigned short COM_No,unsigned char* Ciphertext,unsigned short *nCtextLen,unsigned char use_crypt )
{
//use_crypt:0解密，1加密
	return AES_GCM_Decrypt_OR_Encrypt_H(COM_No,Ciphertext,nCtextLen,use_crypt);

}




