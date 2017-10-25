#include "TypeMeter.h"
#include "TypeE2p.h"
#include "Port.h"
//#include "Type.h"
#include "TypeRAM.h"

#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Measure.h"

#include "COSEMComm.h"
#include "ProfileGeneric.h"
#include "Imagetranfer.h"

//取image_transfer类数据..
/*
	状态：(Image_Transferred_Status)
	0 = transfer not initiated (not active; default state)
	1 = transfer initiated
	2 = verification initiated
	3 = verification successful
	4 = verification failed
	5 = activation initiated
	6 = activation successful
	7 = activation failed
	标志:(Image_Tranfer_Flag)
	修改状态标志:
	Image_Tranfer_Flag=0x06; ----需要执行校验升级程序是否正常..
	Image_Tranfer_Flag=0x70; ----需要执行激活升级程序..	
	
	//Image_Tranfer_Flag 标志位
	#define IMAGE_FLS4K_NEED    		 (0x01)		//FLASH(4K)需要进行先读出再写入操作,为零则ACTION2直接写入..
	#define ACTION1_INTI_FLAG        (0x02)		//ACTION1执行标志..
	#define ACTION2_TRAN_FLAG        (0x04)		//ACTION2执行标志..
	#define ACTION3_VERIFY_FLAG      (0x08)		//ACTION3执行标志..
	#define ACTION4_ACTIVATE_FLAG    (0x10)		//ACTION1执行操作通过标志..
	#define ACTIVATE_TIME_FLAG       (0x20)		//时间到激活标志..
	#define FLAG_BIT6                (0x40)
	#define FLAG_BIT7                (0x80)
	//	== 0; //标志清零..//初始化..
	
*/
short GetImage_transferClassData( unsigned char* Dest, unsigned char* Source ) 
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned short Len=2;
	unsigned char Attribute;
	unsigned long long_temp;///Long_Addr;
	unsigned short i,j;
	
	Ptr = Buff;
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
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
		if((Attribute!=0x11)&&(Attribute!=0x12)){
				if(( Attribute>7 )||(Attribute<=0)) return Len;
				*(Dest+1) = Image_transfer_ParaTab[Attribute-1].DataType;
	  }
	  
		switch( Attribute )
		{
			case 1:														//logical_name
				//*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x06;
				RAM_Write( Dest+3, Ptr, 6 );
				Len = 9;
				break;	
			case 2:
				//*(Dest+1) = D_Unsigned32;								//image_block_size			D_Unsigned32
				long_temp = IMAGE_BLOCK_SIZE;							//程序可接受的数据块长度，程序固定..ImageBlockSize shall not exceed the 
																		//ServerMaxReceivePduSize negotiated.
				FillLongToBuff( Dest+2, long_temp );
				Len = 6;
				break;	
			case 3:
				//*(Dest+1) = Data_bit_string;									//image_transferred_block_status				Data_bit_string
				*(Dest+2) = 0x82;
//				*(Dest+3) = 0x04;				//按照位进行算长度..
//				*(Dest+4) = 0x00;
//				for(i=0;i<128;i++)
//				{				
//					E2P_RData( Dest+5+i, IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, 1 );
//				}								
//				Len = 133;
				*(Dest+3) = 0x03;				//按照位进行算长度.. //TEST SKY2014
				*(Dest+4) = 0x98;
				for(i=0;i<128;i++)
				{				
					E2P_RData( Dest+5+i, IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, 1 );
				}								
//				Len = 120;
			  Len = 133;
				break;	
			case 4:
				//*(Dest+1) = D_Unsigned32;								//image_firstnot_transferred_block_number			D_Unsigned32
				//每次需要在程序中计算好写入EEPROM..
				//E2P_RData( Dest+2, IMAGE_FIRSTNOT_TRANSFERRED_BLOCK_NUMBER, 4 );

//				E2P_RData( Dest+5, IMAGE_TRANSFERRED_BLOCK_STATUS, 128 );
//				Ptr = Dest+5;
//				long_temp = 0xEEEE;				
//				for(i=0;i<128;i++)
//				{
//					for(j=0;j<8;j++)
//					{
//						if((*Ptr)&(0x80>>j)){
//							long_temp = i*8 + j;
//							break;
//						}						
//					}
//					if(long_temp != 0xEEEE) break;					
//					Ptr++;
//				}

				Ptr = Dest+5;
				long_temp = 0xEEEE;				
				for(i=0;i<128;i++)
				{
					E2P_RData( Dest+5+i, IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, 1  );
					Ptr = Dest+5+i;					
					for(j=0;j<8;j++)
					{
						if(!((*Ptr)&(0x80>>j))){
							long_temp = i*8 + j;
							break;
						}						
					}
					if(long_temp != 0xEEEE) break;					
					Ptr++;
				}

/*				if(long_temp == 0xEEEE){
						E2P_RData( Dest+125, IMAGE_TRANSFERRED_BLOCK_STATUS+120, 120 );														
						Ptr = Dest+125;
						long_temp = 0xEEEE;				
						for(i=120;i<240;i++)
						{
							for(j=0;j<8;j++)
							{
									if((*Ptr)&(0x80>>j)){
									long_temp = i*8 + j;
									break;
								}
							}
							if(long_temp != 0xEEEE) break;
							Ptr++;
						}
						if(long_temp == 0xEEEE){				
							E2P_RData( Dest+245, IMAGE_TRANSFERRED_BLOCK_STATUS+240, 79 );
							Ptr = Dest+245;
							long_temp = 0xEEEE;				
							for(i=240;i<319;i++)
							{
								for(j=0;j<8;j++)
								{
									if((*Ptr)&(0x80>>j)){
										long_temp = i*8 + j;
										break;
									}
								}
								if(long_temp != 0xEEEE) break;
								Ptr++;
							}											
						}
				}
*/				
//				if(long_temp != 0xEEEE)
//				{                                  
//				   long_temp++;								//起始数据块从1开始传输..
//         }
//修改数据块从0开始传输..
				FillLongToBuff( Dest+2, long_temp );
				Len = 6;
				break;	
			case 5:
				//*(Dest+1) = Data_boolean;								//image_transferred_enabled boolean
				E2P_RData( Dest+2, IMAGE_TRANSFERRED_ENABLED, 1 );
				if(*(Dest+2)!=0) *(Dest+2) = 1;
				Len = 3;
				break;	
			case 6:
				//*(Dest+1) = D_Unsigned16;								//image_transferred_block_status		enum
				//升级状态..
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );
				*(Dest+2) = Flag.Image_Transferred_Status;				
				Len = 3;
				break;
			case 7:
				//*(Dest+1) = Data_array;								//image_to_activate_info  array ---time	4+5			
				*(Dest+2) = 0x01;		//1个结构体					//1个程序激活..
				*(Dest+3) = 0x02;		//结构体..
				*(Dest+4) = 0x03;		//3个元素..
				*(Dest+5) = D_Unsigned32;	
				
				// E2P_RData( (unsigned char*)&long_temp, IMAGE_TO_ACTIVATE_INFO, 4 );
				// long_temp = (unsigned long)252*1024;  //临时增加sky2013..
				
				E2P_RData( Dest+6, IMMAGE_TRANSFER_SIZE, 4 );				

				*(Dest+10) = D_OctetStr;
				//*(Dest+11) = 0x10;				
				// E2P_RData( Dest+2, IMAGE_TO_ACTIVATE_INFO, 9 );
				//修改版本号显示..				
				// RAM_Write( Dest+12, (unsigned char *)firmware_version1, 16 );
				*(Dest+11) = 0x06;	
				E2P_RData( Dest+12, IMMAGE_TRANSFER_IDENTIFIER, 6 );
											
				*(Dest+18) = D_OctetStr;
				*(Dest+19) = 0x0B;				
				E2P_RData( Dest+20, IMAGE_TO_ACTIVATE_INFO+20, 11 );
				Len = 31;
				break;
		/*	
			//测试使用..
			case 0x11:		//跟01---更新的程序；
				if(*(Source+10)!=D_Unsigned32){
					// *Dest = *(Source+9);
					*(Dest+1) = Get_data_access_result;					//异常标志;
					Len = 2;		
					return 2;
				}
				long_temp = GetBuffToLong(Source+11);
				if(long_temp==0){
					// *Dest = *(Source+9);
					*(Dest+1) = Get_data_access_result;					//异常标志;
					Len = 2;		
					return 2;					
				}
				*(Dest+1) = D_OctetStr;
				
//				if(IMAGE_BLOCK_SIZE<=127){
//					 	*(Dest+2) =  IMAGE_BLOCK_SIZE;
//					 		Len = 3;
//							Ptr = Dest+3;
//				}
//				else{
					 	*(Dest+2) = 0x82;
					 	FillIntegerToBuff( Dest+3, IMAGE_BLOCK_SIZE );					 	
					 	Len = 5;
						Ptr = Dest+5;					
//				}
				
				//读取数据..
//					if(long_temp==0){	//前128个字节独立保存..
//							Long_Addr	= UPGRADE_NEW_START;			
//							//Long_Addr	= (unsigned long)896*(FLASH_4K);
//					}else{
//							Long_Addr	= UPGRADE_START_PROGRAM;					
//							//Long_Addr	= (unsigned long)896*(FLASH_4K)+FLASH_4K;
//							Long_Addr	+= (long_temp-1)*IMAGE_BLOCK_SIZE;
//					}

					
					Long_Addr	= UPGRADE_NEW_START;			
//					Long_Addr	+= long_temp*IMAGE_BLOCK_SIZE;		//Block_number从零开始..			
					Long_Addr	+= (long_temp-1)*IMAGE_BLOCK_SIZE;		//Block_number从1开始..			
					
					Read_Flash(  Ptr,Long_Addr, IMAGE_BLOCK_SIZE );
					
					Len += IMAGE_BLOCK_SIZE;
				break;	
			//测试使用..
			case 0x12:		//02--CPU运行的程序..
				if(*(Source+10)!=D_Unsigned32){
					// *Dest = *(Source+9);
					*(Dest+1) = Get_data_access_result;					//异常标志;
					return 2;
				}
				long_temp = GetBuffToLong(Source+11);
				*(Dest+1) = D_OctetStr;
				
//				if(IMAGE_BLOCK_SIZE<=127){
//					 	*(Dest+2) =  IMAGE_BLOCK_SIZE;
//					 		Len = 3;
//							Ptr = Dest+3;
//				}
//				else{
					 	*(Dest+2) = 0x82;
					 	FillIntegerToBuff( Dest+3, IMAGE_BLOCK_SIZE );					 	
					 	Len = 5;
						Ptr = Dest+5;					
//				}
//								if(long_temp==0){		//前128个字节独立保存..
//										Long_Addr	= CURRENT_NEW_START;	
//								}else{
//										Long_Addr	= CURRENT_START_PROGRAM;
//										Long_Addr	+= (long_temp-1)*IMAGE_BLOCK_SIZE;
//								}
				 Long_Addr	= CURRENT_NEW_START;			
//				 Long_Addr	+= long_temp*IMAGE_BLOCK_SIZE;		//Block_number从零开始..																
				 Long_Addr	+= (long_temp-1)*IMAGE_BLOCK_SIZE;		//Block_number从1开始..			
				 
				 Read_Flash(  Ptr,Long_Addr, IMAGE_BLOCK_SIZE );
							
				 Len += IMAGE_BLOCK_SIZE;		
				break;	
				*/
			default: break;			
		}
	}		
	return Len;
}	

//设置，只有属性5是可以设置升级使能状态需要设置的，其余都是只读..
unsigned char SetImage_transferClassData( unsigned char *Source , unsigned short DataLen )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char ErrorByte=0;	
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
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
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
			case 5:														//升级程序是否允许升级使能boolen..					
				if( *DataType != Data_boolean ) { ErrorByte = type_unmatched; break; }
				// DataLen -= 1;				
				if( DataLen != 2 ) { return type_unmatched; }					//长度错	
				E2P_WData( IMAGE_TRANSFERRED_ENABLED, WriteBufAds+1, 1 );
				break;	
			default: 
				ErrorByte = type_unmatched;
				break;			
		}
	}		
	return ErrorByte;
}	

#if 0
//远程升级的方法命令操作,<程序操作期间不能断电，否则不进行升级操作或者可能操作错误>.. ACTION..
//=====================================================================================================
unsigned char ActionImage_transferClassData( unsigned char *Source, unsigned short DataLen )
{
 	unsigned char Buff[320];
	unsigned char* Ptr;
	unsigned char* read_Ptr;
	unsigned char ErrorByte=0;	
	unsigned char* Logical_Name;
	unsigned char Method,status,BIT_status;
	unsigned short  BIT_status_byte;
  unsigned long Block_number,long_temp,Long_Addr;
  		      short WriteErrorByte=0;
	unsigned  int Lenth;  					
	unsigned long Crc32_Temp,Crc32_Data;
	unsigned  int i;
	unsigned char FW_BUFF[16];
	unsigned char *FW;
		
	Ptr = Buff;
  FW  = FW_BUFF;
  
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		return object_undefined; 															//无对应的OBIS码
	}
		
	//	判断状态 strategyimage_transfer_status =(1) image transfer initiatedimage				
	E2P_RData( Ptr, IMAGE_TRANSFERRED_ENABLED, 1 );					//读升级许可标志..
	if(*Ptr == 0) 	return temporary_failure;								//升级未使能，不执行ACTION..
	
	Ptr = Source+9;
	if(*Ptr==0x01)
        {
          Ptr = Source+10;
          DataLen--; 
        }
         
	switch( Method )
		{
			case 1:			// 不能进行多次初始化.. //2013.6.20修改 参数一样则不再进行初始化操作..
				//ACTION 1:ACTION image_transfer_init(image_identifier, image_size)..
				//				data ::= structure
				//{
				//image_identifier: octet-string,
				//image_size: double-long-unsigned
				//}			
				//includes data in format
				//{ 
				//	image_identifier
				//	image_size
				//}
				if( DataLen != 15 ) { return type_unmatched; }					//长度错	
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != Data_octet_string )||( *(Ptr+3) != 0x06 )||( *(Ptr+10) != D_Unsigned32 )) return other_reason;
				//if((*(Ptr+4)!=0x61)||(*(Ptr+5)!=0x01))	return other_reason; 		//判断image_identifier 0110 0001 00000001				
        //if((*(Ptr+8)&0xE0)!=0x60)	 return other_reason; 		//判断image_identifier 0110 0001 00000001
        
        //20130626改..
				/*
				FGI:Firmware (Image) Group Identification
				000 Not-used 
				001 PRIME parts 
				010 Communication Module (Non-PRIME) 
				011 Meter parts(single) 
				100 Data Concentrator 
				//101 – 111
				 Reserved for Future Use 
				101 3P Direct connection meter 
				110 3P CT meter 
				111 3P CT/PT meter
				*/
				if((*(Ptr+8)&0xE0)!=0x60)	 return other_reason;       //Single meter 
			///	#if( METERTYPE	 == U230_I10A__100A300 )
			///	    if((*(Ptr+8)&0xE0)!=0xA0)	 return other_reason; 		//判断image_identifier 101 3P Direct connection meter 
			///	#endif				
			///	#if( METERTYPE	 == U230_I2_5A__10A3200 )
			///	    if((*(Ptr+8)&0xE0)!=0xC0)	 return other_reason; 		//判断image_identifier 110 3P CT meter
			///	#endif				    
			///	#if( METERTYPE	 == U57_I2_5A__10A12000 )
			///	    if((*(Ptr+8)&0xE0)!=0xE0)	 return other_reason; 		//判断image_identifier 111 3P CT/PT meter
			///	#endif        

				long_temp = GetBuffToLong(Ptr+11);					
				if((long_temp>(FLASH_184K + 128))||(long_temp<0xa3ff)) 	return other_reason; 		//数据太短或超长..
				
				//条件通过,判断是否已经进行过初始化操作..
				read_Ptr = Buff;
				E2P_RData( read_Ptr, IMMAGE_TRANSFER_IDENTIFIER, 6 );			
				E2P_RData( read_Ptr+6, IMMAGE_TRANSFER_SIZE, 4 );			
				if(( Data_Comp( read_Ptr, Ptr+4, 6 ) == 0 )&&(Data_Comp( read_Ptr+6, Ptr+11, 4 ) == 0 )){
					//初始化完成标志..
		                         Flag.Image_Transferred_Status = TRANSFER_INITIATED;                                
				        //*Ptr = TRANSFER_INITIATED;
				          E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
					 return 0;
				}
					
				//条件通过，做初始化..
				Flag.Image_Tranfer_Flag = 0; 			//标志清零..
        		Flag.Image_Transferred_Status = 0;  		//
        		//写入新的版本号及Size
				E2P_WData( IMMAGE_TRANSFER_IDENTIFIER, Ptr+4, 6 );				
				E2P_WData( IMMAGE_TRANSFER_SIZE, Ptr+11, 4 );		//文件总的传输字节数..							
				

				//檫除升级程序存放的FLASH..
				for(long_temp=UPGRADE_NEW_START;long_temp<UPGRADE_END_PROGRAM;long_temp=long_temp+FLASH_4K)
				{
						WriteErrorByte = BlockErase( long_temp );	
						if(WriteErrorByte!=0) return  WriteErrorByte;
				}
				
//				//128*8个状态都置1..
//				Ptr = Buff;
//				RAM_DataFill(Ptr,128,0xff);				
//				WriteErrorByte = E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS, Ptr, 128 );
//				if(WriteErrorByte!=0) return  hardware_fault;
				
//				//128*8个状态都置1..
//				Ptr = Buff;
//				*Ptr = 0xff;
//				for(i=0;i<128;i++)
//				{				
//						WriteErrorByte = E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, Ptr, 1 );
//						if(WriteErrorByte!=0) return  hardware_fault;				
//				}	
				//128*8个状态都置0..The image_transferred_blocks_status   attribute shall be reset
				Ptr = Buff;
				*Ptr = 0x00;
				for(i=0;i<128;i++)
				{				
						WriteErrorByte = E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, Ptr, 1 );
						if(WriteErrorByte!=0) return  hardware_fault;				
				}								
													
				//初始化完成标志..
        Flag.Image_Transferred_Status = TRANSFER_INITIATED;                                
				//*Ptr = TRANSFER_INITIATED;
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );
			
				break;
					
			case 2:		//image_block_transfer:ACTION 2 ---数据传输数据块，重要..
				//	data ::= structure
				//	{
				//		image_block_number: double-long-unsigned,
				//		image_block_value: octet-string
				//	}
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != D_Unsigned32 )||( *(Ptr+7) != Data_octet_string )) return other_reason;

				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );							//读升级状态标志..	 					 
//			if((status ==0 )||(status >7))  return temporary_failure;				//未初始化则不进行任何操作..
//			if((Flag.Image_Transferred_Status != TRANSFER_INITIATED )&&(Flag.Image_Transferred_Status != VERIFICATION_FAILED))  return temporary_failure;		//未初始化或进入校验，校验通过则不进行重新写FLASH操作..
				if((Flag.Image_Transferred_Status ==0 )||(Flag.Image_Transferred_Status >7))  return temporary_failure;				//未初始化则不进行任何操作..
												
				//数据块及长度..
				Block_number = GetBuffToLong(Ptr+3);
					
        long_temp = FLASH_184K + 128;
        long_temp = long_temp/IMAGE_BLOCK_SIZE + 1;                                       
                                        
				//if(Block_number>2017) 		return other_reason; 		//数据块超出(FLASH_252K + 128)/128=0..
//			if(Block_number>long_temp) 		return other_reason; 		//数据块超出(FLASH_252K + 128)/128=0..
				if(Block_number>long_temp) 		return other_reason; 		//数据块超出(FLASH_252K + 128)/128=0..

				//if(Block_number==0) 		return other_reason; 		//Block_number修改从1开始传输,需要增加..数据块为零,则退出操作..	
					
					Block_number ++;   //修改按照从0开始计算sky20160301..
										                                        
					if(( *(Ptr+8) & 0x80 ) != 0 )
					{
							if( *(Ptr+8) == 0x81 )										//设置数据长度是否为1字节 
							{
								Lenth = *(Ptr+9);										//有效设置数据长度
								//检查接收的总字节数和数据块是否一致..
								if( DataLen != Lenth + 10 ) { return type_unmatched; }					//长度错
								Ptr = Ptr+10;											    //有效设置数据起始地址			
							}			
							else if( *(Ptr+8) != 0x82 ) return other_reason;
							else 
							{
								Lenth = GetBuffToInteger( Ptr+9 );
								//检查接收的总字节数和数据块是否一致..
								if( DataLen != Lenth + 11 ) { return type_unmatched; }					//长度错								
								Ptr = Ptr+11;
							}	
					}	
					else
					{
								Lenth = *(Ptr+8);										//有效设置数据长度
								//检查接收的总字节数和数据块是否一致..
								if( DataLen != Lenth + 9 ) { return type_unmatched; }					//长度错								
								Ptr = Ptr+9;											    //有效设置数据起始地址			
					}	
					
					//sky20141218增加软件版本号的判断..
					if(Block_number==1){
						FW = (unsigned char *)((unsigned char *)firmware_version0);
						if(Data_CompHL(FW,Ptr,10)!=0) return hardware_fault;
					}
						
					if(Block_number<=200){					//50K以内空间，增加判断,可以在这里增加最后一帧判断..
						 if(Lenth!=IMAGE_BLOCK_SIZE)	return  temporary_failure;			//判断每帧的数据长度..
						 Flag.Image_Tranfer_Flag = 0x02;
					}
					else
					{
	               	 if(Lenth!=IMAGE_BLOCK_SIZE)
					 	{
	               	 	  Flag.Image_Tranfer_Flag = 0x04;	 //当最后一帧来判断..
	               	 	  Flag.Image_Transferred_Status = VERIFICATION_INITIATED;   //校验初始化..
	               	 	  E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );
	               		}
   					}
					
//					//写数据..						
//					if(Block_number==0){		//前128个字节单独分开保存..
//							Long_Addr	= UPGRADE_NEW_START;			
//							//Long_Addr	= (unsigned long)896*(FLASH_4K);
//					}else{
//							Long_Addr	= UPGRADE_START_PROGRAM;					
//							//Long_Addr	= (unsigned long)896*(FLASH_4K)+FLASH_4K;
//							Long_Addr	+= (Block_number-1)*IMAGE_BLOCK_SIZE;
//					}

//					//传输成功状态位置1..
//					BIT_status_byte =(unsigned short )(Block_number/8);
//					if(BIT_status_byte>319)  return  hardware_fault;
//					BIT_status = (unsigned char )(Block_number - (Block_number/8)*8);
//					BIT_status =	(0x80>>BIT_status);					
//					E2P_RData( &status, IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte, 1 );
//					if(status&BIT_status) return other_reason;			//已经写成功则不再重复写..
//					
//					Long_Addr	= UPGRADE_NEW_START;			
//					Long_Addr	+= Block_number*IMAGE_BLOCK_SIZE;			//Block_number从零开始..			
//					
//					WriteErrorByte = DataFlash_Write_NOT_Erase( Long_Addr, Ptr, Lenth );
//					if(WriteErrorByte!=0) return  hardware_fault;		
//
//					status |=	BIT_status;
//					E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte, &status, 1 );	

					//传输成功状态位置0..
					Block_number = Block_number -1;			//Block_number修改从1开始传输,需要增加..
					
					BIT_status_byte =(unsigned short )(Block_number/8);
//					if(BIT_status_byte>319)  return  hardware_fault;
					if(BIT_status_byte>128)  return  hardware_fault;//超出传输范围了
					BIT_status = (unsigned char )(Block_number - (Block_number/8)*8);
					
					//BIT_status =	~(0x80>>BIT_status);				

					BIT_status =	(0x80>>BIT_status);			//2013.6.20..置最新一次传输的状态=1

					E2P_RData( &status, IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte*2, 1 );
//					if((status&BIT_status)==0) return other_reason;			//已经写成功则不再重复写..
//					if((status&BIT_status)==0) return   ErrorByte;			//已经写成功则不再重复写(返回正确应答)..
					if((status&BIT_status)==1) return   ErrorByte;			////20160229 改状态0--未传输,1--已传输..	..、///相等说明已传输过该块,直接返回
						
					//BIT_status =	~BIT_status;
					//依次存储接收的数据块内容		
					Long_Addr	= UPGRADE_NEW_START;			
					Long_Addr	+= Block_number*IMAGE_BLOCK_SIZE;			//Block_number从零开始..			
					
				///	WriteErrorByte = DataFlash_Write_NOT_Erase( Long_Addr, Ptr, Lenth );
					WriteErrorByte = Write_Flash( Long_Addr, Ptr, Lenth );//接收数据从缓存中写入flash中
					if(WriteErrorByte!=0) return  hardware_fault;		

					//status &=	BIT_status;
					status |=	BIT_status;    //20160229 改状态0--未传输,1--已传输..	
					E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte*2, &status, 1 );	          
				break;	
			case 3:			
				//image_verify:ACTION 3  Verifies the integrity of the Image before activation. data::= integer (0)
				//(0) success, if the verification could be completed;
				//(2) temporary-failure if the verification has not been completed;
				//(250) other-reason if the verification failed.
				if( DataLen != 2 ) { return type_unmatched; }					//长度错					
				if(( *Ptr != D_Integer8 )||( *(Ptr+1) != 0)) 	return other_reason;
				
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );							//读升级状态标志..	 			
				if((Flag.Image_Transferred_Status ==0 )||(Flag.Image_Transferred_Status >5))  return temporary_failure;				//未初始化则不进行校验..
					
				E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//文件总的传输字节数..
				long_temp = GetBuffToLong(Ptr+1);
				if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;

				//头部128个字节计算CRC32..
				Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
				
				//读取文件CRC32..
			///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );
				Read_FlashD( Ptr+5,UPGRADE_NEW_START+124 ,4 );
				Crc32_Data = GetBuffToLong(Ptr+5);
				if(Crc32_Temp!=Crc32_Data) return temporary_failure;	

				//计算烧录程序文件CRC32..
				Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
				
				//读取文件CRC32..
			///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
				Read_FlashD( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );	

				Crc32_Data = GetBuffToLong(Ptr+5);
				if(Crc32_Temp==Crc32_Data){
  				//校验函数..
				  //*Ptr = VERIFICATION_SUCCESSFUL;				
				  //E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
				  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
					
//				*Ptr = 0xa5;
////			*(Ptr+1) = 0xa5;				
//				E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
				  Flag.Image_Tranfer_Flag = 0x26;	
				  
				  //升级校验成功记录..
					SM.StandardEventCode = Firmware_ready_for_activation;
					LoadRecord( Standard_Event_No );
					SM.Event_all_flag |= Standard_Event_BIT;
					SM.Error_all_flag |= Standard_Event_BIT;
					//-------------				  					
				}else{
								Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
								E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );	
							  //升级校验失败记录..
								SM.StandardEventCode = Firmware_verification_failed;
								LoadRecord( Standard_Event_No );
								SM.Event_all_flag |= Standard_Event_BIT;
								SM.Error_all_flag |= Standard_Event_BIT;									
					 			return temporary_failure;		
							}
				break;	
			case 4:			
				//image_activate:ACTION 4 Activates the Image(s). data::= integer (0) --同Action 3
				//(0) success, if the verification could be completed;
				//(2) temporary-failure if the verification has not been completed;
				//(250) other-reason if the verification failed.
				if( DataLen != 2 ) { return type_unmatched; }					//长度错..
				if(( *Ptr != D_Integer8 )||( *(Ptr+1) != 0)) 	return other_reason;
				Ptr = Buff;
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );					//读升级许可标志..
				// if(Flag.Image_Transferred_Status >= 5 ) 	return other_reason;
				if((Flag.Image_Transferred_Status < 2 )||(Flag.Image_Transferred_Status >= 5 )) 	return other_reason;
				else{							
				// if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL)&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED)){
					if(*Ptr ==0) return other_reason;
						E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//文件总的传输字节数..
						long_temp = GetBuffToLong(Ptr+1);
						if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;
		
						//头部128个字节计算CRC32..
						Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
						
						//读取文件CRC32..
					///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );	
						Read_FlashD( Ptr+5,UPGRADE_NEW_START+124 ,4 );
						Crc32_Data = GetBuffToLong(Ptr+5);
						if(Crc32_Temp!=Crc32_Data) return temporary_failure;	
		
						//计算烧录程序文件CRC32..
						Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
						
						//读取文件CRC32..
					///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
						Read_FlashD( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );	
						Crc32_Data = GetBuffToLong(Ptr+5);
						if(Crc32_Temp==Crc32_Data){
		  				  //校验函数..
						  //*Ptr = VERIFICATION_SUCCESSFUL;				
						  //E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
						  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
							E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
							
						  *Ptr = 0xa5;
		//				  *(Ptr+1) = 0xa5;				
						  E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
						  Flag.Image_Tranfer_Flag = 0x26;					  

						}else{
										Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
										E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
							 			return temporary_failure;		
									}

				}	 					 
				//if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED))  return temporary_failure;				//未校验则不进行激活操作..
			if(Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )  return temporary_failure;				//未校验则不进行激活操作..				

				Flag.Image_Transferred_Status = ACTIVATION_INITIATED;	
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
				//激活函数..			  
			  *Ptr = 0xb5;
				*(Ptr+1) = 0xc9;				
				E2P_WData( IMMAGE_TRANSFER_ACTIVATE, Ptr, 2 );
				Flag.Image_Tranfer_Flag = 0x70;
				
			///	Disk.FullDisDelay = 0x06;			//显示6s全屏..
				
//					//升级激活成功记录..
//					SM.StandardEventCode = Firmware_activated;
//					LoadRecord( Standard_Event_No );
//					SM.Event_all_flag |= Standard_Event_BIT;
					//-------------				  									  																
//				Activate_Image_Transfer(0);
//				
//				Delay(2000);
//				asm(" mov &0xFFFE, PC;");            									
				
				break;	
			default: 
				ErrorByte = other_reason;		
				break;
		}			
  return ErrorByte;
}
#endif
//远程升级的方法命令操作,<程序操作期间不能断电，否则不进行升级操作或者可能操作错误>.. ACTION..
//测试使用，把前128个字节信息都屏蔽掉，只单纯接收升级程序文件，屏蔽掉CRC校验、软硬件版本号等信息-----2016-9-14
//=====================================================================================================
unsigned char ActionImage_transferClassData( unsigned char *Source, unsigned short DataLen )
{
 	unsigned char Buff[320];
	unsigned char* Ptr;
	unsigned char* read_Ptr;
	unsigned char ErrorByte=0;	
	unsigned char* Logical_Name;
	unsigned char Method,status,BIT_status;
	unsigned short  BIT_status_byte;
  unsigned long Block_number,long_temp,Long_Addr;
  		      short WriteErrorByte=0;
	unsigned short Lenth;  					
	unsigned long Crc32_Temp,Crc32_Data;
	unsigned short i;
	unsigned char FW_BUFF[16];
	unsigned char *FW;
		
	Ptr = Buff;
  FW  = FW_BUFF;
  
	Logical_Name = Source+2;
	Method = *(Source+8);
	
	//0.0.44.0.0.255
	Buff[0] = 0x00;
	Buff[1] = 0x00;
	Buff[2] = 0x2c;		//44
	Buff[3] = 0x00;
	Buff[4] = 0x00;
	Buff[5] = 0xff;
		
	if( Data_Comp( Ptr, Logical_Name, 6 ) != 0 ) 
	{
		return object_undefined; 															//无对应的OBIS码
	}
		
	//	判断状态 strategyimage_transfer_status =(1) image transfer initiatedimage				
	E2P_RData( Ptr, IMAGE_TRANSFERRED_ENABLED, 1 );					//读升级许可标志..
	if(*Ptr == 0) 	return temporary_failure;								//升级未使能，不执行ACTION..
	
	Ptr = Source+9;
	if(*Ptr==0x01)
        {
          Ptr = Source+10;
          DataLen--; 
        }
         
	switch( Method )
		{
			case 1:			// 不能进行多次初始化.. //2013.6.20修改 参数一样则不再进行初始化操作..
				//ACTION 1:ACTION image_transfer_init(image_identifier, image_size)..
				//				data ::= structure
				//{
				//image_identifier: octet-string,
				//image_size: double-long-unsigned
				//}			
				//includes data in format
				//{ 
				//	image_identifier
				//	image_size
				//}
				if( DataLen != 15 ) { return type_unmatched; }					//长度错	
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != Data_octet_string )||( *(Ptr+3) != 0x06 )||( *(Ptr+10) != D_Unsigned32 )) return other_reason;
				//if((*(Ptr+4)!=0x61)||(*(Ptr+5)!=0x01))	return other_reason; 		//判断image_identifier 0110 0001 00000001				
        //if((*(Ptr+8)&0xE0)!=0x60)	 return other_reason; 		//判断image_identifier 0110 0001 00000001
        
        //20130626改..
				/*
				FGI:Firmware (Image) Group Identification
				000 Not-used 
				001 PRIME parts 
				010 Communication Module (Non-PRIME) 
				011 Meter parts(single) 
				100 Data Concentrator 
				//101 – 111
				 Reserved for Future Use 
				101 3P Direct connection meter 
				110 3P CT meter 
				111 3P CT/PT meter
				*/
				#if( MeterPhase== SinglePhase)
					if((*(Ptr+8)&0xE0)!=0x60)	 return other_reason;       //Single meter 
				#else
					#if( METERTYPE	 == U230_I10A__100A300 )
					    if((*(Ptr+8)&0xE0)!=0xA0)	 return other_reason; 		//判断image_identifier 101 3P Direct connection meter 
					#elif( METERTYPE	 == U230_I2_5A__10A3200 )				
					    if((*(Ptr+8)&0xE0)!=0xC0)	 return other_reason; 		//判断image_identifier 110 3P CT meter
					#elif( METERTYPE	 == U57_I2_5A__10A12000 )				    
					    if((*(Ptr+8)&0xE0)!=0xE0)	 return other_reason; 		//判断image_identifier 111 3P CT/PT meter
					#endif   
				#endif
				#if 0
				long_temp = GetBuffToLong(Ptr+11);					
				if((long_temp>(FLASH_184K + 128))||(long_temp<0xa3ff)) 	return other_reason; 		//数据太短或超长..
				
				//条件通过,判断是否已经进行过初始化操作..
				read_Ptr = Buff;
				E2P_RData( read_Ptr, IMMAGE_TRANSFER_IDENTIFIER, 6 );			
				E2P_RData( read_Ptr+6, IMMAGE_TRANSFER_SIZE, 4 );			
				if(( Data_Comp( read_Ptr, Ptr+4, 6 ) == 0 )&&(Data_Comp( read_Ptr+6, Ptr+11, 4 ) == 0 )){
					//初始化完成标志..
		                         Flag.Image_Transferred_Status = TRANSFER_INITIATED;                                
				          E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
					 return 0;
				}
				#endif	
				//条件通过，做初始化..
				Flag.Image_Tranfer_Flag = 0; 			//标志清零..
        		Flag.Image_Transferred_Status = 0;  		//
				
				//写入新的版本号及Size
				E2P_WData( IMMAGE_TRANSFER_IDENTIFIER, Ptr+4, 6 );				
				E2P_WData( IMMAGE_TRANSFER_SIZE, Ptr+11, 4 );		//文件总的传输字节数..							
				

				//檫除升级程序存放的FLASH..
				for(long_temp=UPGRADE_NEW_START;long_temp<UPGRADE_END_PROGRAM;long_temp=long_temp+FLASH_4K)
				{
						WriteErrorByte = BlockErase( long_temp );	
						if(WriteErrorByte!=0) return  WriteErrorByte;
				}
				//128*8个状态都置0..The image_transferred_blocks_status   attribute shall be reset
				Ptr = Buff;
				*Ptr = 0x00;
				for(i=0;i<128;i++)
				{				
						WriteErrorByte = E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+i*2, Ptr, 1 );
						if(WriteErrorByte!=0) return  hardware_fault;				
				}								
													
				//初始化完成标志..
        Flag.Image_Transferred_Status = TRANSFER_INITIATED;                                
				//*Ptr = TRANSFER_INITIATED;
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );
			
				break;
					
			case 2:		//image_block_transfer:ACTION 2 ---数据传输数据块，重要..
				//	data ::= structure
				//	{
				//		image_block_number: double-long-unsigned,
				//		image_block_value: octet-string
				//	}
				if(( *Ptr != Data_structure )||( *(Ptr+1) != 2 )||( *(Ptr+2) != D_Unsigned32 )||( *(Ptr+7) != Data_octet_string )) return other_reason;

				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );							//读升级状态标志..	 					 
				if((Flag.Image_Transferred_Status ==0 )||(Flag.Image_Transferred_Status >7))  return temporary_failure;				//未初始化则不进行任何操作..
												
				//数据块及长度..
				Block_number = GetBuffToLong(Ptr+3);
					
       			long_temp = FLASH_184K + 128;
        		///long_temp = long_temp/IMAGE_BLOCK_SIZE + 1;                                       
                long_temp = long_temp/IMAGE_BLOCK_SIZE;                          
				if(Block_number>long_temp) 		return other_reason; 		//数据块超出(FLASH_252K + 128)/128=0..
					
					Block_number ++;   //修改按照从0开始计算sky20160301..
										                                        
					if(( *(Ptr+8) & 0x80 ) != 0 )
					{
							if( *(Ptr+8) == 0x81 )										//设置数据长度是否为1字节 
							{
								Lenth = *(Ptr+9);										//有效设置数据长度
								//检查接收的总字节数和数据块是否一致..
								if( DataLen != Lenth + 10 ) { return type_unmatched; }					//长度错
								Ptr = Ptr+10;											    //有效设置数据起始地址			
							}			
							else if( *(Ptr+8) != 0x82 ) return other_reason;
							else 
							{	//0x82
								Lenth = GetBuffToInteger( Ptr+9 );
								//检查接收的总字节数和数据块是否一致..
								if( DataLen != Lenth + 11 ) { return type_unmatched; }					//长度错								
								Ptr = Ptr+11;
							}	
					}	
					else
					{
								Lenth = *(Ptr+8);										//有效设置数据长度
								//检查接收的总字节数和数据块是否一致..
								if( DataLen != Lenth + 9 ) { return type_unmatched; }					//长度错								
								Ptr = Ptr+9;											    //有效设置数据起始地址			
					}	
					
					//sky20141218增加软件版本号的判断..
					if(Block_number==1){
						//E2P_RData(FW,Device_ID4,16);
						FW = (unsigned char *)((unsigned char *)firmware_version0);
						if(Data_CompHL(FW,Ptr,16)!=0) return hardware_fault;
					}
						
					if(Block_number<=200){					//50K以内空间，增加判断,可以在这里增加最后一帧判断..
						 if(Lenth!=IMAGE_BLOCK_SIZE)	return  temporary_failure;			//判断每帧的数据长度..
						 Flag.Image_Tranfer_Flag = 0x02;
					}
					else
					{
	               	 if(Lenth!=IMAGE_BLOCK_SIZE)
					 	{
	               	 	  Flag.Image_Tranfer_Flag = 0x04;	 //当最后一帧来判断..
	               	 	  Flag.Image_Transferred_Status = VERIFICATION_INITIATED;   //校验初始化..
	               	 	  E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );
	               		}
   					}
					
					//传输成功状态位置0..
					Block_number = Block_number -1;			//Block_number修改从1开始传输,需要增加..
					
					BIT_status_byte =(unsigned short )(Block_number/8);
					if(BIT_status_byte>128)  return  hardware_fault;//超出传输范围了
					BIT_status = (unsigned char )(Block_number - (Block_number/8)*8);
					
					BIT_status =	(0x80>>BIT_status);			//2013.6.20..置最新一次传输的状态=1

					E2P_RData( &status, IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte*2, 1 );
					if((status&BIT_status)==1) return   ErrorByte;			////20160229 改状态0--未传输,1--已传输..	..、///相等说明已传输过该块,直接返回
						
					//依次存储接收的数据块内容		
					Long_Addr	= UPGRADE_NEW_START;	///	UPGRADE_START_PROGRAM;	
					Long_Addr	+= Block_number*IMAGE_BLOCK_SIZE;			//Block_number从零开始..			
					
					WriteErrorByte = Write_Flash( Long_Addr, Ptr, Lenth );//接收数据从缓存中写入flash中
					if(WriteErrorByte!=0) return  hardware_fault;		

					status |=	BIT_status;    //20160229 改状态0--未传输,1--已传输..	
					E2P_WData( IMAGE_TRANSFERRED_BLOCK_STATUS+BIT_status_byte*2, &status, 1 );	          
				break;	
			case 3:			
				//image_verify:ACTION 3  Verifies the integrity of the Image before activation. data::= integer (0)
				//(0) success, if the verification could be completed;
				//(2) temporary-failure if the verification has not been completed;
				//(250) other-reason if the verification failed.
				if( DataLen != 2 ) { return type_unmatched; }					//长度错					
				if(( *Ptr != D_Integer8 )||( *(Ptr+1) != 0)) 	return other_reason;
				
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );							//读升级状态标志..	 			
				if((Flag.Image_Transferred_Status ==0 )||(Flag.Image_Transferred_Status >5))  return temporary_failure;				//未初始化则不进行校验..
				E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//文件总的传输字节数..
				long_temp = GetBuffToLong(Ptr+1);
				if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;

				//头部128个字节计算CRC32..
				Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
				
				//读取文件CRC32..
			///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );
				Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );
				Crc32_Data = GetBuffToLong(Ptr+5);
				if(Crc32_Temp!=Crc32_Data) return temporary_failure;	

				//计算烧录程序文件CRC32..
				Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
				
				//读取文件CRC32..
			///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
				Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );	

				Crc32_Data = GetBuffToLong(Ptr+5);
				if(Crc32_Temp==Crc32_Data){
  				//校验函数..
				  //*Ptr = VERIFICATION_SUCCESSFUL;				
				  //E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
				  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
					
//				*Ptr = 0xa5;
////			*(Ptr+1) = 0xa5;				
//				E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
				  Flag.Image_Tranfer_Flag = 0x26;	
				  
				  //升级校验成功记录..
					SM.StandardEventCode = Firmware_ready_for_activation;
					LoadRecord( Standard_Event_No );
					SM.Event_all_flag |= Standard_Event_BIT;
					SM.Error_all_flag |= Standard_Event_BIT;
					//-------------				  					
				}else{
								Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
								E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );	
							  //升级校验失败记录..
								SM.StandardEventCode = Firmware_verification_failed;
								LoadRecord( Standard_Event_No );
								SM.Event_all_flag |= Standard_Event_BIT;
								SM.Error_all_flag |= Standard_Event_BIT;									
					 			return temporary_failure;		
							}

					//	  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
					//		E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
					//	  Flag.Image_Tranfer_Flag = 0x26;	
						  
						  //升级校验成功记录..
						//	SM.StandardEventCode = Firmware_ready_for_activation;
						//	LoadRecord( Standard_Event_No );
						//	SM.Event_all_flag |= Standard_Event_BIT;
						//	SM.Error_all_flag |= Standard_Event_BIT;
				break;	
			case 4:			
				//image_activate:ACTION 4 Activates the Image(s). data::= integer (0) --同Action 3
				//(0) success, if the verification could be completed;
				//(2) temporary-failure if the verification has not been completed;
				//(250) other-reason if the verification failed.
				if( DataLen != 2 ) { return type_unmatched; }					//长度错..
				if(( *Ptr != D_Integer8 )||( *(Ptr+1) != 0)) 	return other_reason;
				Ptr = Buff;
				E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );					//读升级许可标志..
		#if 0
				// if(Flag.Image_Transferred_Status >= 5 ) 	return other_reason;
				if((Flag.Image_Transferred_Status < 2 )||(Flag.Image_Transferred_Status >= 5 )) 	return other_reason;
				else{							
				// if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL)&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED)){
					if(*Ptr ==0) return other_reason;
						E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//文件总的传输字节数..
						long_temp = GetBuffToLong(Ptr+1);
						if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;
		
						//头部128个字节计算CRC32..
						Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
						
						//读取文件CRC32..
					///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );	
						Read_FlashD( Ptr+5,UPGRADE_NEW_START+124 ,4 );
						Crc32_Data = GetBuffToLong(Ptr+5);
						if(Crc32_Temp!=Crc32_Data) return temporary_failure;	
		
						//计算烧录程序文件CRC32..
						Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
						
						//读取文件CRC32..
					///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
						Read_FlashD( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );	
						Crc32_Data = GetBuffToLong(Ptr+5);
						if(Crc32_Temp==Crc32_Data){
		  				  //校验函数..
						  //*Ptr = VERIFICATION_SUCCESSFUL;				
						  //E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
						  Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
							E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
							
						  *Ptr = 0xa5;
		//				  *(Ptr+1) = 0xa5;				
						  E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
						  Flag.Image_Tranfer_Flag = 0x26;					  

						}else{
										Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
										E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
							 			return temporary_failure;		
									}

				}
		#endif
				//if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED))  return temporary_failure;				//未校验则不进行激活操作..
			if(Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL )  return temporary_failure;				//未校验则不进行激活操作..				

				Flag.Image_Transferred_Status = ACTIVATION_INITIATED;	
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );						
				//激活函数..			  
			  *Ptr = 0xb5;
				*(Ptr+1) = 0xc9;				
				E2P_WData( IMMAGE_TRANSFER_ACTIVATE, Ptr, 2 );
				Flag.Image_Tranfer_Flag = 0x70;
				
				Disk.FullDisDelay = 0x06;			//显示6s全屏..
				break;	
			default: 
				ErrorByte = other_reason;		
				break;
		}			
  return ErrorByte;
}

//计算CRC校验..
unsigned long Flash_GenerateCRC32(unsigned long Addr,unsigned long len )
{
	unsigned long lenth;
	unsigned char Buff[520];
	unsigned char *Ptr;
	unsigned short i;
	unsigned long Crc32_check=0;
	
	i = 0;
	while( len > 0)
	{
		Ptr = Buff;		
		lenth = len/512;
		if(lenth==0){
				lenth = len;
		}else{
				lenth = 512;
		}
	///	Read_Flash( Ptr,(unsigned long)Addr+(unsigned long)i*512, lenth );
		Read_Flash( Ptr,(unsigned long)Addr+(unsigned long)i*512, lenth );
		Crc32_check = GenerateCRC32(Crc32_check, Ptr,lenth); 
		i++;
		len -=lenth;		
	}
	return Crc32_check; 
}

// 计算CRC32
// 超长计算CRC32，需要将 oldcrc32 = 0 付值后做计算,最先开始需要付初始值 
unsigned long GenerateCRC32(unsigned long oldcrc32, unsigned char * dataptr,unsigned short len) 
{ 
		// unsigned long oldcrc32; 
		unsigned long crc32; 
		unsigned long oldcrc; 
		// unsigned short charcnt; 
		unsigned char c,t; 
		// oldcrc32 = 0x00000000; //初值为0 
		while (len--) { 
				t= (oldcrc32 >> 24) & 0xFF; 		//要移出的字节的值 
				oldcrc=crc_32_tab[t]; 					//根据移出的字节的值查表 		
				c = *dataptr; 									//新移进来的字节值 
				oldcrc32= (oldcrc32 << 8) | c; 	//将新移进来的字节值添在寄存器末字节中 
				oldcrc32=oldcrc32^oldcrc; 			//将寄存器与查出的值进行xor运算 
				dataptr++;
			} 
			crc32=oldcrc32; 
			return crc32; 
}

//激活函数..
unsigned char Activate_Image_Transfer(unsigned char Enter_Flag )
{
	//unsigned long lenth;
	unsigned char Buff[20];
	unsigned char *Ptr;
	unsigned char Error=0;
//	unsigned short Addr_data16;
//	unsigned long Addr_data20;
	//unsigned long Crc32_check=0;
  unsigned long long_temp;//long_ADDR;
	
	//激活前确认CRC32是否通过..
	
	//备份当前的程序（引导程序是否需要升级???暂时不考虑..）..	
	//先檫除保存当前程序的FLASH地址..(开辟出一块flash存放当前程序)
	for(long_temp=CURRENT_NEW_START;long_temp<CURRENT_END_PROGRAM;long_temp=long_temp+FLASH_4K)
	{
		Error = BlockErase( long_temp );	
		if(Error!=0) return  Error;
	}
/*        
 //拷贝程序(0x5c00 --- 0xFFFF)到FLASH..
 //0xF000 ---0xFFFF引导区不做考虑..
 Ptr = Buff;
 long_ADDR = CURRENT_NEW_START;        
 //for(Addr_data16=0x5c00;Addr_data16<=0xEfff;(Addr_data16+512))
 for(Addr_data16=0x5c00;Addr_data16<=0xEfff;(Addr_data16=Addr_data16+512))
  {
	  cpu_fls_int_Read_Flash( Ptr, Addr_data16, 512 );
		            
	///  Error = DataFlash_Write_NOT_Erase( long_ADDR, Ptr, 512 );
	  Error =  Write_Flash( long_ADDR, Ptr, 512 );
	  if(Error!=0) return  Error;   //sky20140714..
	  long_ADDR +=512;           
  }
  
  long_ADDR = CURRENT_NEW_START + 0x9400;				//(0xefff-0x5c00)+1;
  Ptr = Buff;
 
 ///Cortex-M0 Flash与MSP4305438不同，没有信息段空间，不备份---fire
  //存储data20数据..(0x10000----0x45bff)
  for(Addr_data20=0x10000;Addr_data20<=0x45bff;(Addr_data20=Addr_data20+512))
  {
	  cpu_fls_data20_Read_Flash( Ptr, Addr_data20, 512 );
		            
	///  Error = DataFlash_Write_NOT_Erase( long_ADDR, Ptr, 512 );
	   Error =  Write_Flash( long_ADDR, Ptr, 512 );
	  if(Error!=0) return  Error;   //sky20140714..
	  long_ADDR +=512;           
  }  
  */
  
  //计算备份当前程序的校验和..
	//校验程序对错..
	//给引导程序指示如何操作：
	// 0xa1a5---升级A--新程序；
	// 0xb3b7--升级程序B---老程序；
	// 0xc1c3XX---错误，XX--表示失败次数..
	//写CPU-INFO-FLASH C..
	
	//写信息数据区INFOC
///  cpu_Fls_int_SegClr( 0x55,(unsigned char *)CPU_INFOC  );//fire
///	*Ptr = 0xa1;
///	*(Ptr+1) = 0xa5;
///	*(Ptr+2) = 0xff;
	Ptr = Buff;
	E2P_RData( Ptr+3,IMMAGE_TRANSFER_SIZE, 4 );		//文件总的传输字节数..
	long_temp = GetBuffToLong(Ptr+3);
	if((long_temp<65536)||(long_temp>(FLASH_252K + 128))) 	return other_reason;
//	E2P_RData( Ptr+7,IMMAGE_TRANSFER_SIZE, 4 );		//文件总的传输字节数..
	
//	if(Data_Comp(Ptr+3,Ptr+7,4)!=0) 	return other_reason;
		
//	*Ptr = 0xb3;
//	*(Ptr+1) = 0xb7;
//	*(Ptr+2) = 0;	
	
///	 cpu_Fls_int_Write(0x0C, CPU_INFOC, Ptr, 12 );//fire
	 
////升级激活成功记录..
//	SM.StandardEventCode = Firmware_activated;
//	LoadRecord( Standard_Event_No );
//	SM.Event_all_flag |= Standard_Event_BIT;  
	
	 Flag.Image_Transferred_Status = 5;	
	 E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		

//版本号进行切换..
 	Ptr = Buff;
 	E2P_RData( Ptr,IMMAGE_TRANSFER_IDENTIFIER, 6 );		//读出升级的版本号..
 	
 	E2P_WData( Device_ID4, Ptr, 6 ); 	
 	*Ptr = 0;
 	*(Ptr+1) = 0;			
    *(Ptr+2) = 0;		
 	*(Ptr+3) = 0;	
 	*(Ptr+4) = 0;			
 	*(Ptr+5) = 0;	
 	*(Ptr+6) = 0;
 	*(Ptr+7) = 0;	
 	E2P_WData( ImageID7_1, Ptr, 7 );	
	Ptr = (unsigned char *)((unsigned char *)firmware_version0);	
 	E2P_WData( ImageID7_2, Ptr, 16 );
// 
//	//  test
//	cpu_fls_int_Read_Flash( Ptr+10, CPU_INFOC, 3 );		
	
///	cpu_Fls_int_SegClr( 0x55,(unsigned char *)0xee00  );//fire
	
	SM.Init_Flag = 0;			//初始化..  
	return Error; 
}

//校验并生成事件:
//激活函数..
unsigned char Verify_Image_Transfer( unsigned char Enter_Flag )
{
 	unsigned char  Buff[150];
	unsigned char* Ptr;
//	unsigned char* read_Ptr;
//	unsigned char ErrorByte=0;	
//	unsigned char* Logical_Name;
//	unsigned char Method,status,BIT_status;
//	unsigned short  BIT_status_byte;
//  unsigned long Block_number,long_temp,Long_Addr;
  unsigned long long_temp;        
//  		      short WriteErrorByte=0;
//	unsigned  int Lenth;  					
	unsigned long Crc32_Temp,Crc32_Data;
//	unsigned  int i;
//
	if(Flag.Image_Tranfer_Flag>= 0x04)
	{
		
	Ptr = Buff;
	E2P_RData( &Flag.Image_Transferred_Status, IMAGE_TRANSFERRED_STATUS, 1 );					//读升级许可标志..
	if((Flag.Image_Transferred_Status != VERIFICATION_SUCCESSFUL)&&(Flag.Image_Transferred_Status != ACTIVATION_INITIATED))
	{
			if(*Ptr ==0) return other_reason;
			E2P_RData( Ptr+1,IMMAGE_TRANSFER_SIZE, 4 );		//文件总的传输字节数..
			long_temp = GetBuffToLong(Ptr+1);
			if((long_temp<65536)||(long_temp>(FLASH_252K + 128)))		 	return other_reason;
				
			//头部128个字节计算CRC32..
			Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START,124 );	
								
			//读取文件CRC32..
		///	Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );	
			Read_Flash( Ptr+5,UPGRADE_NEW_START+124 ,4 );
			Crc32_Data = GetBuffToLong(Ptr+5);
			if(Crc32_Temp!=Crc32_Data){
					Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
					return temporary_failure;		
			}
				
			//计算烧录程序文件CRC32..
			Crc32_Temp = Flash_GenerateCRC32( UPGRADE_NEW_START+128,long_temp-136 );	
								
			//读取文件CRC32..
		///	Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );		
			Read_Flash( Ptr+5,UPGRADE_NEW_START+long_temp-8 ,4 );
			Crc32_Data = GetBuffToLong(Ptr+5);
			if(Crc32_Temp==Crc32_Data){
				//校验函数..
				//*Ptr = VERIFICATION_SUCCESSFUL;				
				//E2P_WData( IMAGE_TRANSFERRED_STATUS, Ptr, 1 );
				Flag.Image_Transferred_Status = VERIFICATION_SUCCESSFUL;	
				E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );				  
									
				*Ptr = 0xa5;
				//				  *(Ptr+1) = 0xa5;				
				E2P_WData( IMMAGE_TRANSFER_VERIFY, Ptr, 1 );
				Flag.Image_Tranfer_Flag = 0x26;					  
		
		  }else{
					Flag.Image_Transferred_Status = VERIFICATION_FAILED;	
					E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
		 			return temporary_failure;		
				}
  	}
	}
	return 0x55;      
}
#if 0

//初始化安全时间等..
void Image_tranfer_init(void)
{
	unsigned char INFO_buff[20];
	unsigned char *INFO_buff_Ptr;

	//给引导程序指示如何操作：
	// 0xa1a5---升级A--新程序；
	// 0xb3b7--升级程序B---老程序；
	// XXXXXXXX --四个字节做长度..
	// 0xc1c3XX---错误，XX--表示失败次数..
	//写CPU-INFO-FLASH C..

	INFO_buff_Ptr = INFO_buff;
	cpu_fls_int_Read_Flash( INFO_buff_Ptr, CPU_INFOC, 12 );  
  if((*(INFO_buff_Ptr)==0xa1)&&(*(INFO_buff_Ptr+1)==0xa5)&&(*(INFO_buff_Ptr+2)==0x55)) 	Flag.New_Test_Flag = 0x57;
  else 	Flag.New_Test_Flag = 0;
	
	// 是否需要判断安全时间..
	Flag.New_Image_safetime = 99999999;
	Flag.New_check_ok = 0;	
	//safe time
	if(Flag.New_Test_Flag==0x57) 	E2P_RData( (unsigned char *)&Flag.New_Image_safetime, New_firmware_safety_time, 4 );
	if(Flag.New_Image_safetime ==0)  Flag.New_check_ok = 0x58;	   //确认采用新程序..		
}

// 等安全时间过了后，判断是否回到老的程序或者新程序..
void Image_tranfer_NewOrOld(void)
{
	unsigned char INFO_buff[20];
	unsigned char *INFO_buff_Ptr;
	unsigned char ImageBuff[7];
	unsigned char *Ptr;	
	
	if(Flag.New_Test_Flag != 0x57) return;
  if(Flag.New_Image_safetime>0) Flag.New_Image_safetime--;

  //伊朗暂时不做回退功能..(不需要切回原来程序)..
   Flag.New_check_ok = 0x58;
   Flag.New_Image_safetime = 0xFFFFFFFF;
   // =============2016-01-14 13:22:12..
   
//  if(Flag.New_check_ok != 0x58)	   //确认是否执行新程序..
//  {
//  	//读取EEPROM，ID7_CONFIG来判断..
//  }	  
  if(Flag.New_check_ok == 0x58)	   //执行新程序..
  {
	  	INFO_buff_Ptr = INFO_buff;
			cpu_fls_int_Read_Flash( INFO_buff_Ptr, CPU_INFOC, 12 );
			*INFO_buff_Ptr=0x0;  
			*(INFO_buff_Ptr+1)=0x0; 
		  *(INFO_buff_Ptr+2)=0x0;
		  //写信息数据区INFOC
  		cpu_Fls_int_SegClr( 0x55,(unsigned char *)CPU_INFOC  );
  	 	cpu_Fls_int_Write(0x0C, CPU_INFOC, INFO_buff_Ptr, 12 );	
  	 	
  	 	//升级激活成功记录..
  	 	Flag.Image_Transferred_Status = 6;	
			E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
					
			SM.StandardEventCode = Firmware_activated;
			LoadRecord( Standard_Event_No );
			SM.Event_all_flag |= Standard_Event_BIT;
			SM.Error_all_flag |= Standard_Event_BIT;
			
			Flag.New_check_ok = 0;			//清零..
			Flag.New_Test_Flag = 0;  	
			
			Ptr = ImageBuff;			
 			E2P_RData( Ptr,IMMAGE_TRANSFER_IDENTIFIER, 6 );		//读出升级的版本号..
			E2P_WData( Device_ID7, Ptr, 6 ); 	
		 	*(Ptr+6) = 0x01;
		 	E2P_WData( ImageID7_1, Ptr, 7 );						
  }else{
  			 if(Flag.New_Image_safetime==0)  //执行老程序..
  			 	{
						  //计算备份当前程序的校验和..
							//校验程序对错..
							//给引导程序指示如何操作：
							// 0xa1a5---升级A--新程序；
							// 0xb3b7--升级程序B---老程序；
							// 0xc1c3XX---错误，XX--表示失败次数..
							//写CPU-INFO-FLASH C..
							
							//写信息数据区INFOC
							INFO_buff_Ptr = INFO_buff;
							cpu_fls_int_Read_Flash( INFO_buff_Ptr, CPU_INFOC, 12 ); 
						  cpu_Fls_int_SegClr( 0x55,(unsigned char *)CPU_INFOC  );
							*INFO_buff_Ptr = 0xb3;
							*(INFO_buff_Ptr+1) = 0xb7;
							*(INFO_buff_Ptr+2) = 0xff;
							
							//文件传输的字节数#define FLASH_256K	 0x40000		//256K..
							*(INFO_buff_Ptr+3) = 0;
							*(INFO_buff_Ptr+4) = 0x03;
							*(INFO_buff_Ptr+5) = 0xf0;
							*(INFO_buff_Ptr+6) = 0;
							 
							*(INFO_buff_Ptr+7) = 0;
							*(INFO_buff_Ptr+8) = 0x03;
							*(INFO_buff_Ptr+9) = 0xf0;
							*(INFO_buff_Ptr+10) = 0;
	 					 
	 					  cpu_Fls_int_Write(0x0C, CPU_INFOC, INFO_buff_Ptr, 12 );
							 
							Flag.Image_Transferred_Status = 7;	
							E2P_WData( IMAGE_TRANSFERRED_STATUS, &Flag.Image_Transferred_Status, 1 );		
						
						//写版本号..
							Ptr = ImageBuff;			
				 			E2P_RData( Ptr,Device_ID7, 6 );		//读出升级的版本号..
						 	E2P_WData( ImageID7_0, Ptr, 6 ); 	
						 	E2P_WData( ImageID7_2, Ptr, 6 );
						 	*Ptr = 0;
						 	*(Ptr+1) = 0;			
						  *(Ptr+2) = 0;		
						 	*(Ptr+3) = 0;	
						 	*(Ptr+4) = 0;			
						 	*(Ptr+5) = 0;	
						 	*(Ptr+6) = 0;
						 	*(Ptr+7) = 0;	
						 	E2P_WData( ImageID7_1, Ptr, 7 );								 
						//	//  test
						//	cpu_fls_int_Read_Flash( Ptr+10, CPU_INFOC, 3 );		
							
							cpu_Fls_int_SegClr( 0x55,(unsigned char *)0xee00  );
							
							SM.Init_Flag = 0;			//初始化.. 
							asm(" mov &0xFFFE, PC;");   	//执行引导区 		 
  			 	}
  		}
}
#endif
