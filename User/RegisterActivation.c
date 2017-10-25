#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Measure.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"
#include "Mem.h"

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
///#include "Demand.h"
#include "DisconnectControl.h"
#include "ScriptTable.h"
#include "AssociationLN.h"
#include "Limiter.h"
#include "ComSetup.h"
//取Register_Activation类数据:包括电能和需量..
short GetRegister_ActivationClassData( unsigned char* Dest, unsigned char* Source ) 
{
	short i,j,k;
	unsigned char Buff[20];
	unsigned char Logical_Name[6];
	unsigned char* Ptr;	
	unsigned short Addr;
	         short Len=2;
	unsigned char Attribute;
	
	
	Ptr = Logical_Name;
	
	RAM_Write( Ptr, Source+2, 6 );
	
	for( i=0;i<DLMS_Reg_ActivationSum;i++ )
	{
		if( Data_Comp( Ptr, (unsigned char*)DLMS_Reg_ActivationTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	if( i == DLMS_Reg_monitorSum )
	{
		*Dest = Get_data_access_result;					//异常标志
		*(Dest+1) = object_undefined; 					//无对应的OBIS码
		Len = 2;		
	}
	else 
	{
		Addr = DLMS_Reg_ActivationTab[i].Addr;
		*Dest = Get_data;								//正常数据返回	
		Attribute = *(Source+2+6);			//请求的属性
		switch( Attribute )
		{
			case 1:										//logical_name
				*(Dest+1) = Data_octet_string;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:										//register_assignment	
				*(Dest+1) = Data_array;
				E2P_RData( Buff, Addr, 1 );
				if(Buff[0]>=32){
					 Buff[0]=32;
				}
				*(Dest+2) = Buff[0];
				Ptr = Dest+3;
				Len =3;
				for(j=0;j<Buff[0];j++ )
				{
					*Ptr = Data_structure;
					*(Ptr+1) = 0x02;
					*(Ptr+2) = D_Unsigned16;
					E2P_RData( Ptr+3, Addr+10*j+2, 2 );
					*(Ptr+5) = D_OctetStr;
					*(Ptr+6) = 0x06;
					E2P_RData( Ptr+7, Addr+10*j+5, 6 );
					Ptr += 13;		
					Len += 13;
				}				
				break;	
			case 3:										//register_mask_list
				Addr = DLMS_Reg_ActivationTab[i].State1;
				*(Dest+1) = Data_array;
				E2P_RData( Buff, Addr, 1 );
				if(Buff[0]>=8){
					 Buff[0]=8;
				}
				*(Dest+2) = Buff[0];
				Ptr = Dest+3;
				Len =3;
				for(j=0;j<Buff[0];j++ )
				{
					*Ptr = Data_structure;
					*(Ptr+1) = 0x02;
					*(Ptr+2) = D_OctetStr;
					*(Ptr+3) = 0x01;//0x05;
					E2P_RData( Ptr+4, Addr+23*j+2, 1 );
					
					*(Ptr+5) = Data_array;
					E2P_RData( &Buff[8], Addr+23*j+8, 1 );
					if(Buff[8]>=8){
					 	Buff[8]=8;
					}
					*(Ptr+6) = Buff[8];
					E2P_RData( &Buff[10], Addr+23*j+10, Buff[8] );								
					for(k=0;k<Buff[8];k++ )
					{										
						*(Ptr+7+k*2) = D_Unsigned8;
						*(Ptr+8+k*2) = Buff[10+k];
					}					
					Ptr += (7+Buff[8]*2);		
					Len += (7+Buff[8]*2);
				}			
				break;	
			case 4:										//当前激活费率号..
				Addr = DLMS_Reg_ActivationTab[i].State2;	
				*(Dest+1) = D_OctetStr;
			//	*(Dest+2) = 0x05;
				*(Dest+2) = 0x01;	//台湾			
				// E2P_RData( Dest+3, Addr, 5 );								
			//	RAM_Write( Dest+3, (unsigned char *)DLMS_ACTIVATION_FEE[SM.Currently_Active_tariff[0]-1].FEE_OCTET_STRING, 5 );
								
			//	Len = 8;
				///*(Dest+3) = 0x01;	//台湾
				*(Dest+3) = SM.Fee;
				Len = 4;
				break;	
			default:
				break;	
		}	
	}		
	return Len;
}	

unsigned char SetRegister_ActivationClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[20];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;

	//unsigned short Temp;
	unsigned char* Logical_Name;
	unsigned char* Attribute;
	unsigned char* WriteBufAds;

	unsigned short Addr;
        unsigned short i,j;
	
	Ptr = Buff;
	
	Logical_Name = Source+2;
	Attribute = Source+8;
	WriteBufAds = Source+10;
	// DataType = WriteBufAds;
	
	for( i=0;i<DLMS_Reg_ActivationSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_Reg_ActivationTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	if( i == DLMS_Reg_ActivationSum ) { return object_undefined; }
	
	if(( *Attribute == 1 )||( DLMS_Reg_ActivationTab[i].ComInfo & WRITE ) == 0 ) { return read_write_denied; }

	Addr = DLMS_Reg_ActivationTab[i].Addr;

	switch( *Attribute )
	{
//		case 1:														//logical_name			
//			break;	
			case 2:										//register_assignment	
				//if(*WriteBufAds != Data_array) return type_unmatched;
				ErrorByte = Judge_Register_Activation_objects( WriteBufAds,i,2 );	
				if(ErrorByte != 0 )	return ErrorByte;
				
				Buff[0] = *( WriteBufAds+1 );	
				E2P_WData(  Addr, Buff, 1 );
		
				Ptr = WriteBufAds+2;
				
				for(j=0;j<Buff[0];j++ )
				{
				//	*Ptr = Data_structure;
				//	*(Ptr+1) = 0x02;
				//	*(Ptr+2) = D_Unsigned16;
					E2P_WData( Addr+10*j+2,Ptr+3, 2 );
				//	*(Ptr+5) = D_OctetStr;
				//	*(Ptr+6) = 0x06;
					E2P_WData(  Addr+10*j+5,Ptr+7, 6 );
					Ptr += 13;		
				}				
				break;	
			case 3:										//register_mask_list
				ErrorByte = Judge_Register_Activation_objects(WriteBufAds,i,3);	
				if(ErrorByte != 0 )	return ErrorByte;	
								
				Addr = DLMS_Reg_ActivationTab[i].State1;
				
				Buff[0] = *( WriteBufAds+1 );	
				E2P_WData(  Addr, Buff, 1 );

				Ptr = WriteBufAds+2;
				
				for(j=0;j<Buff[0];j++ )
				{
					//*Ptr = Data_structure;
				  //	*(Ptr+1) = 0x02;
					//*(Ptr+2) = D_OctetStr;
					//*(Ptr+3) = 0x05;
					E2P_WData( Addr+23*j+2,Ptr+4, 1 );
					
					//*(Ptr+9) = Data_array;
					Buff[8] = *(Ptr+6);
					if(Buff[8]>=8){
					 	Buff[8]=8;
					}										
					E2P_WData( Addr+23*j+8,&Buff[8], 1 );
					for(i=0;i<Buff[8];i++ )
					{										
						//*(Ptr+11+k*2) = D_Unsigned8;
						 Buff[10+i]= *(Ptr+8+i*2);
					}
					E2P_WData( Addr+23*j+10, &Buff[10], Buff[8] );
																							
					Ptr += (7+Buff[8]*2);		
	
				}			
				break;		
			case 4:										//当前激活费率号..
				Addr = DLMS_Reg_ActivationTab[i].State2;
				if((*WriteBufAds != D_OctetStr)||(*(WriteBufAds+1)!=0x01)) return other_reason;
				E2P_WData( Addr, WriteBufAds+2, 1 );				
				break;
		default: 
			ErrorByte = type_unmatched;
			break;			
	}
 
	return ErrorByte;
}	


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

short Judge_Register_Activation_objects( unsigned char* Data, unsigned char Tab_No, unsigned char Attribute_No )
{
	unsigned char Buff[12];
	
	unsigned short Items;
	short i,j,k;
	unsigned char* Ptr;
	unsigned short Temp;
			
	Ptr = Buff;
        
	if( *Data != Data_array ) return other_reason;	
			Items = *(Data+1);
	if(Attribute_No==2){						
		 if( Items > (DLMS_Reg_ActivationTab[Tab_No].ComInfo&0xff) ) return other_reason;			//寄存器分配表超过额定限数..
		 	Ptr = Data +2;
			for( i=0;i<Items;i++ )
			{
					if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )) return other_reason;		
					if(( *(Ptr+2) != D_Unsigned16 )||( *(Ptr+3) != 0 )||( *(Ptr+5) != Data_octet_string )||( *(Ptr+6) != 6 )) return other_reason;
					if(Tab_No==0){
						if( *(Ptr+4) != 3 ) return other_reason;	
					}else{
						if( *(Ptr+4) != 4 ) return other_reason;	
					}			
					Ptr += 13;		
			}
}else{		//属性3..
				 if( Items > ((DLMS_Reg_ActivationTab[Tab_No].ComInfo&0xff)/4) ) return other_reason;			//寄存器分配表超过额定限数..
				 Ptr = Data +2;
				for( i=0;i<Items;i++ )
				{
					if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )) return other_reason;		
					if(( *(Ptr+2) != Data_octet_string )||( *(Ptr+3) != 1 )||( *(Ptr+5) != Data_array )) return other_reason;
					for(k=0;k<4;k++)
					{
							if( Data_Comp( Ptr+4, (unsigned char*)DLMS_ACTIVATION_FEE[k].FEE_OCTET_STRING, 1 ) == 0 ) break;				//查找对应的OBIS码					  						
					}
					if( k == 4 ) return other_reason;		//判断是否设置的是定义的4个费率号Rate1,2,3,4..
					  								
					Temp = *(Ptr+6);
					if( Temp > 8) return other_reason;
					for(j=0;j<Temp;j++)
					{
						if(*(Ptr+7+j*2)!= D_Unsigned8) return other_reason;
						if(*(Ptr+8+j*2)> (DLMS_Reg_ActivationTab[Tab_No].ComInfo&0xff)) return other_reason;	
					}					
					Ptr += (7 +Temp*2);		
				}	
			}	
  return 0;
}

void initialRegister_ActivationPara( void )
{
	unsigned char Buff1[10];
	unsigned char Buff2[24]={0x01,0x08,0x01,0x01,0x06,0x01,0x01,0x02,0x01,0x01,0x80,0x01,0x02,0x08,0x01,0x02,0x06,0x01,0x02,0x02,0x01,0x02,0x80,0x01,};
	unsigned char Buff3[6]={0x01,0x00,0x01,0x08,0x01,0xff};

	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char* Point2;
	//unsigned long Value;	
	unsigned short Addr;
	short i,j,k;
	unsigned char max_array=32;
	
	//--------------台湾圆表,按照台湾标书内容填写--------------
	Addr = DLMS_Reg_ActivationTab[0].Addr;

	//-------register_assignment------
	E2P_WData( Addr,&max_array, 1 );
	for(j=0;j<max_array;j++ )
		{
		Ptr = Buff1;
		Point = Buff2;
		Point2 = Buff3;
		*Ptr = 0x00;
		*(Ptr+1) = 0x03;
			E2P_WData( Addr+10*j+2,Ptr, 2 );
			RAM_Write(Point2+2,Point+3*(j%8),3);
			*(Point2+4) = j/8;
			E2P_WData(	Addr+10*j+5,Point2, 6 );

		}
//-----------mask_list-----------------
	Addr = DLMS_Reg_ActivationTab[0].State1;
		max_array = 4;
		E2P_WData( Addr,&max_array, 1 );
		for(j=0;j<max_array;j++ )
				{	
		//			Ptr = Buff; 	
		//			Ptr =(unsigned char *)"Rate";
					Ptr = Buff1; 	
					if(j==0){
						*Ptr =0x41;
						*(Ptr+2) =1;
						*(Ptr+3) =2;				
						*(Ptr+4) =3;
						*(Ptr+5) =4;
						*(Ptr+6) =5;
						*(Ptr+7) =6;
						*(Ptr+8) =7;
						*(Ptr+9) =8;
					}
					if(j==1){
						*Ptr =0x42;
						*(Ptr+2) =9;
						*(Ptr+3) =10;				
						*(Ptr+4) =11;
						*(Ptr+5) =12;
						*(Ptr+6) =13;	
						*(Ptr+7) =14;
						*(Ptr+8) =15;
						*(Ptr+9) =16;
					}
					if(j==2){
						*Ptr =0x43;
						*(Ptr+2) =17;
						*(Ptr+3) =18;				
						*(Ptr+4) =19;
						*(Ptr+5) =20;
						*(Ptr+6) =21;
						*(Ptr+7) =22;
						*(Ptr+8) =23;
						*(Ptr+9) =24;
					}
					if(j==3){
						*Ptr =0x44;
						*(Ptr+2) =25;
						*(Ptr+3) =26;				
						*(Ptr+4) =27;
						*(Ptr+5) =28;
						*(Ptr+6) =29;
						*(Ptr+7) =30;
						*(Ptr+8) =31;
						*(Ptr+9) =32;
					}			
					E2P_WData( Addr+23*j+2, Ptr,1 );
		
					*(Ptr+1) = 0x08;
					E2P_WData( Addr+23*j+8, Ptr+1,1 );
					E2P_WData( Addr+23*j+10,Ptr+2,8 );
				}
		
		//active_mask	
		Addr = DLMS_Reg_ActivationTab[0].State2;
		Ptr = Buff1;	
		*Ptr = 0x41;
		E2P_WData( Addr, Ptr,1 );						

	//--------------------------------------
/*	
	for( i=0;i<2;i++ )
	{
		//寄存器分配..
		if(i==1) max_array = 16;			
		Addr = DLMS_Reg_ActivationTab[i].Addr;
		E2P_WData( Addr,&max_array, 1 );	//
		k =1;					
		for(j=0;j<max_array;j++ )
		{
					Ptr = Buff;
					*Ptr = 0x00;
					if(i==0) *(Ptr+1) = 0x03;
					else *(Ptr+1) = 0x04;
					E2P_WData( Addr+10*j+2, Ptr,2 );
					if(i==0){
							Ptr = (unsigned char *)&DLMS_ECTab[k].Logical_Name;					
							k++;
							switch( k )
							{
								case 5:
									k++;
									break;
								case 10:
									k ++;
									break;						
								case 15:
									k++;
									break;
								case 20:
									k = 51;
									break;
							}
					}else{
								Ptr = (unsigned char *)&DLMS_MDTab[k].Logical_Name;										
								k++;
								switch( k )
								{
									case 5:
										k++;
										break;
									case 10:
										k ++;
										break;						
									case 15:
										k++;
										break;
								}				
							}						
					E2P_WData( Addr+10*j+5,Ptr,6 );
		}

			
		//mask_list
		Addr = DLMS_Reg_ActivationTab[i].State1;
		max_array = 4;
		E2P_WData( Addr,&max_array, 1 );	//
		k = 1;							
		for(j=0;j<max_array;j++ )
		{	
//			Ptr = Buff;		
//			Ptr =(unsigned char *)"Rate";
			Ptr = Buff;		
			if(j==0){
				Ptr =(unsigned char *)"Rate1";
				Buff[10] =1;
				Buff[11] =5;				
				Buff[12] =9;
				Buff[13] =13;
				Buff[14] =17;															
				//octet-string(size(5))
				//index_array
				//Value
				//"Rate 1"
				//1, 5, 9, 13,17
				//"Rate 2"
				//2, 6, 10, 14,18
				//"Rate 3"
				//3, 7, 11, 15,19
				//"Rate 4"
				//4, 8, 12, 16,20
				//				
			}
			if(j==1){
				Ptr =(unsigned char *)"Rate2";
				Buff[10] =2;
				Buff[11] =6;				
				Buff[12] =10;
				Buff[13] =14;
				Buff[14] =18;																			
			}
			if(j==2){
				Ptr =(unsigned char *)"Rate3";
				Buff[10] =3;
				Buff[11] =7;				
				Buff[12] =11;
				Buff[13] =15;
				Buff[14] =19;																			
			}
			if(j==3){
				Ptr =(unsigned char *)"Rate4";
				Buff[10] =4;
				Buff[11] =8;				
				Buff[12] =12;
				Buff[13] =16;
				Buff[14] =20;																			
			}			
			E2P_WData( Addr+23*j+2, Ptr,5 );

			if(i==0) Buff[8] = 0x05;
			else Buff[8] = 0x04;
			E2P_WData( Addr+23*j+8, &Buff[8],1 );
			E2P_WData( Addr+23*j+10,&Buff[10],Buff[8] );
		}
	
			//active_mask	
			Addr = DLMS_Reg_ActivationTab[i].State2;
			Ptr = Buff;		
			Ptr = (unsigned char *)"Rate1";
			E2P_WData( Addr, Ptr,5 );						
	}		
	*/
}	


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
unsigned char ActionRegister_ActivationClassData( unsigned char *Source, unsigned short DataLen )
{
 	unsigned char Buff[22];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;
	unsigned char* Logical_Name;
	unsigned char Method;
	unsigned char i,j,k;
	unsigned short  Addr,Temp=0;
		
	
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	for( i=0;i<DLMS_Reg_ActivationSum;i++ )
	{
		if( Data_Comp( Logical_Name, (unsigned char*)DLMS_Reg_ActivationTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}	
	if( i == DLMS_Reg_ActivationSum ) { return object_undefined; }
	
	
	Ptr = Source+9;	
if( Method == 1 )
	{
				Addr = DLMS_Reg_ActivationTab[i].Addr;
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )) return other_reason;		
				if(( *(Ptr+2) != D_Unsigned16 )||( *(Ptr+3) != 0 )||( *(Ptr+5) != Data_octet_string )||( *(Ptr+6) != 6 )) return other_reason;
				if(i==0){
								if( *(Ptr+4) != 3 ) return other_reason;	
				}else{
								if( *(Ptr+4) != 4 ) return other_reason;	
						}
				E2P_RData( (unsigned char *)&Temp, Addr, 1 );
				if(Temp>=(DLMS_Reg_ActivationTab[i].ComInfo&0xff))  return other_reason;
				Temp++;
				E2P_WData( Addr, (unsigned char *)&Temp, 1 );
				//	*Ptr = Data_structure;
				//	*(Ptr+1) = 0x02;
				//	*(Ptr+2) = D_Unsigned16;
				E2P_WData( Addr+10*(Temp-1)+2,Ptr+3, 2 );
				//	*(Ptr+5) = D_OctetStr;
				//	*(Ptr+6) = 0x06;
				E2P_WData(  Addr+10*(Temp-1)+5,Ptr+7, 6 );
	}
	else if( Method == 2 )
	{			//add_mask
				Addr = DLMS_Reg_ActivationTab[i].State1;
				
				//if( Items > ((DLMS_Reg_ActivationTab[Tab_No].ComInfo&0xff)/4) ) return other_reason;			//寄存器分配表超过额定限数..
				
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )) return other_reason;		
				if(( *(Ptr+2) != Data_octet_string )||( *(Ptr+3) != 5 )||( *(Ptr+9) != Data_array )) return other_reason;

				for(k=0;k<4;k++)
				{
						if( Data_Comp( Ptr+4, (unsigned char*)DLMS_ACTIVATION_FEE[k].FEE_OCTET_STRING, 5 ) == 0 ) break;				//查找对应的OBIS码					  						
				}
				if( k == 4 ) return other_reason;		//判断是否设置的是定义的4个费率号Rate1,2,3,4..
											
				Buff[0] = *(Ptr+10);
				if( Buff[0] > 8) return other_reason;
				for(j=0;j<Buff[0];j++)
				{
						if(*(Ptr+11+j*2)!= D_Unsigned8) return other_reason;
						if(*(Ptr+12+j*2)> (DLMS_Reg_ActivationTab[i].ComInfo&0xff)) return other_reason;
						Buff[j+1] = *(Ptr+12+j*2);	
				}
								
				E2P_RData( (unsigned char *)&Temp, Addr, 1 );
				for(j=0;j<Temp;j++)
				{
					 E2P_RData( &Buff[10], Addr+23*j+2, 5 );					 
					 if( Data_Comp( Ptr+4, &Buff[10], 5 ) == 0 ){				//替换..	
					 	 	 E2P_WData( Addr+23*j+8, Buff, 1 );	 	
					 	 	 E2P_WData( Addr+23*j+10, &Buff[1], Buff[0] );
					 	 	 return 0;					 	 
					 	}
				}
				//增加..				
				if(Temp >((DLMS_Reg_ActivationTab[i].ComInfo&0xff)/4))  return other_reason;					
				E2P_WData( Addr+23*Temp+2, Ptr+4,  5 );					
//				E2P_WData( Addr+23*j+8, Buff, Buff[0]+1 );
		 	 	 E2P_WData( Addr+23*Temp+8, Buff, 1 );	 	
		 	 	 E2P_WData( Addr+23*Temp+10, &Buff[1], Buff[0] );
				Temp++;
				E2P_WData( Addr, (unsigned char *)&Temp, 1 );
			
	}else if( Method == 3 )
	{			//delete_mask..		
				Addr = DLMS_Reg_ActivationTab[i].State1;

 			  if(( *Ptr != Data_octet_string )||( *(Ptr+1) != 5 )) return other_reason;		
								
				E2P_RData( (unsigned char *)&Temp, Addr, 1 );
				for(j=0;j<Temp;j++)
				{
					 E2P_RData( &Buff[10], Addr+23*j+2, 5 );					 
					 if( Data_Comp( Ptr+2, &Buff[10], 5 ) == 0 ){				//找到相同..
//					 		if(Temp==j+1){		//最后一条相同..					 			
//					 			RAM_Fill( Buff, 20);
//								E2P_WData( Addr+20*j+2, Buff, 20 );
//					 		}else{						
									 		for(k=j+1;k<Temp;k++)
									 		{
//									 			E2P_RData( Buff, Addr+23*k+2, 22 );			
//									 			E2P_WData( Addr+23*j+2, Buff, 22 );
												E2P_RData( Buff, Addr+23*k+2, 5 );														
												E2P_WData( Addr+23*j+2, Buff,  5 );
												
												E2P_RData( Buff, Addr+23*k+8, 1 );												
										 	 	E2P_WData( Addr+23*j+8, Buff, 1 );	 	

												E2P_RData( &Buff[2], Addr+23*k+10, Buff[0]);												
										 	 	
										 	 	E2P_WData( Addr+23*j+10, &Buff[2], Buff[0] );		
									 			j++;					 	 	 
									 		}
									 		
							 				RAM_Fill( Buff, 22);
//											E2P_WData( Addr+23*j+2, Buff, 22 );
												E2P_WData( Addr+23*j+2, Buff,  5 );						
										 	 	E2P_WData( Addr+23*j+8, Buff, 1 );	 	
										 	 	E2P_WData( Addr+23*j+10, Buff, 8 );	//清除..										
//					 				 }
								Temp--;
								E2P_WData( Addr, (unsigned char *)&Temp, 1 );					 				 		 	
					 	 	 return 0;					 	 
					 	}
				}
				ErrorByte = other_reason;									//无此mask_name..				
	}	
	else ErrorByte = other_reason;										

	return ErrorByte;
}	

