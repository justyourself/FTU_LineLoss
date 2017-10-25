#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
#include "TypeFRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"

#include "RsComm.h"
#include "interrupt.h"
#include "Mem.h"
#include "Time.h"
#include "Power.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
//#include "Produce.h"
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif

//取Class9类数据
short GetRecordFreezeClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short Len;

	RAM_Write( Dest, Source, 4 );

	Len = GetRecordFreezeClassDataTab( COM_No, Dest+4, Source );
	if( Len == -1 )  
	{
//		*(Dest+4) = 0x00;									//SEQUENCE OF RCSD == 0
//		*(Dest+5) = Get_data_access_result;					//异常标志
//		*(Dest+6) = object_undefined; 						//无对应的OBIS码
////		Len = 7;										//长度不对
//		Len = 3;											//17.04.30	
		Len = GetRecordAbnormalResponse( Dest, Source, object_undefined );	//17.04.30
	}
	else if( Len == -2 )										//17.04.09
	{
//		*(Dest+4) = 0x00;									//SEQUENCE OF RCSD == 0
//		*(Dest+5) = Get_data_access_result;					//异常标志
//		*(Dest+6) = read_write_denied; 						//操作权限不足
////		Len = 7;										//长度不对
//		Len = 3;											//17.04.30	
		Len = GetRecordAbnormalResponse( Dest, Source, read_write_denied );	//17.04.30
	}	
	Len += 4;
	return Len;		
}		

//取Freeze类数据
short GetRecordFreezeClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
//	short i,j;
	short i;
//	unsigned char* Addr;
	short Len;
//	unsigned short OICode;
//	short Temp;
//	short Start,End;
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
//	unsigned char* Ptr;
	unsigned char* OAD;
	Len = -1;
	
	OAD = Source;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
					
	i = GetFreezeClassTabItemNo( OAD );
	if( i != DL698_Freeze_ProfileSum )
	{
		if( Attribute == 2 )			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_Freeze_ProfileTab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09		
		}	
		*Dest = Get_data;								//正常数据返回	
		switch( Attribute )
		{
			case 1:										//logical_name
				break;	
			case 2:										//	
				Len = GetFreezeBufferData( COM_No, Dest, Source, i );
				break;	
			case 3:										//关联对象属性表
				break;	
			case 4:										//配置参数	
				break;	
			default:
				break;	
		}	
	}		
	return Len;
}	
/*
unsigned char JudgeRCSD( short COM_No, unsigned char* Source, short* Len )
{
	unsigned char ItemAs,ItemBs;
	unsigned char ErrorByte=0;
	unsigned char* Ptr;
	short i,j;
	
	Items = *Source;
	Ptr = Source+1;
	for( i=0;i<ItemAs;i++ )						//sequence of csd
	{
		*Len += 1;
		if( *Ptr == 0 )							//OAD			
		{
			*Len += 4;
			Ptr += 4;			
		}
		else if( *Ptr == 1 )					//RCSD		
		{
			ErrorByte = type_unmatched;			//不支持RCSD,但需计算RCSD长度
			*Len += 5;							//OAD				
			Ptr += 5;						
			ItemBs = *Ptr;
			for( i=0;i<ItemBs;i++ )				//sequence of OAD
			{
				*Len += 4;
				Ptr += 4;			
			}
		}
		else 
		{
			ErrorByte = type_unmatched;			//不明数据类型
			NoResponseFrame( COM_No );	
			return ErrorByte;	
		}	
	}
	
	return ErrorByte;
}

unsigned char JudgeSelectorOAD( unsigned char* OAD )
{
	if(( *OAD == 0x20 )&&( *(OAD+1) == 0x21)&&( *(OAD+2) == 0x02)&&( *(OAD+3) == 0x00)) return 0;
	else return 1;
}	

unsigned char IsFreezeNoOAD( unsigned char* OAD )
{
	if(( *OAD == 0x20 )&&( *(OAD+1) == 0x23)&&( *(OAD+2) == 0x02)&&( *(OAD+3) == 0x00)) return 0;
	else return 1;
}

void NoResponseFrame( short COM_No )
{
	*CommPara[COM_No].Buf = 0xFF;
}	
*/

unsigned short FillAllFreezeOADToRcsd( unsigned char* Dest, short ItemNo )		//16.10.22
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned char* DestPtr;
    unsigned short Items;
	unsigned short Addr;
    unsigned short Len;
    short i;
	
	Ptr = Buff;
	
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	*Dest = Items+2;
	DestPtr = Dest + 1;
	RAM_Fill( DestPtr, 10 );
	*(DestPtr+1) = 0x20;
	*(DestPtr+2) = 0x23;
	*(DestPtr+3) = 0x02;
	*(DestPtr+6) = 0x20;
	*(DestPtr+7) = 0x21;
	*(DestPtr+8) = 0x02;
	DestPtr += 10;
	Len = 11;
	for( i=0;i<Items;i++ )
	{
		*DestPtr = 0;
		E2P_RData( Ptr, Addr, 9 );
		RAM_Write( DestPtr+1, Ptr+2, 4 );
		DestPtr += 5;
		Addr += 10;
	}	
	Len += Items * 5;	

	return Len;	
}	


short GetFreezeBufferData( short COM_No, unsigned char* Dest, unsigned char* Source, short ItemNo )
{
	unsigned short Len=0;
	unsigned short RSDLen=0;
	unsigned short RCSDLen=0;
	unsigned char* RSD;
	unsigned char* RCSD;
//	unsigned char* Ptr;
//	unsigned char ErrorByte=0;
//	unsigned char OAD[4];
//	unsigned char StartData[8];
//	unsigned char EndData[8];
	unsigned char Space[4];
//	unsigned short* BlockNumber;
//	unsigned char* SubsBlockPara;	

//	BlockNumber = CommPara[COM_No].BlockNumber;
//	SubsBlockPara = CommPara[COM_No].SubsBlockPara;

	RSD = Source+4;
	RAM_Fill( Space, 4 );		//数据间隔
	RSDLen = GetRSDLen( COM_No, RSD );
	RCSDLen = GetRCSDLen( COM_No, RSD+RSDLen );
    RCSD = RSD+RSDLen;
/*	switch( *RSD )
	{
		case 1:
			if(( JudgeSelectorOAD( RSD+1 ) != 0 )||( *(RSD+5) != D_date_time_s )) 	//非冻结时间选项不支持
			{
				NoResponseFrame( COM_No );
				return 0;
			}
			RCSD = RSD + 13;
			RAM_Write( OAD, RSD+1, 4 );
			RAM_Write( StartData, RSD+6, 7 );			
			break;
		case 2:
			if(( JudgeSelectorOAD( RSD+1 ) != 0 )||( *(RSD+5) != D_date_time_s )||( *(RSD+13) != D_date_time_s )
				||((*(RSD+21) != null_data )&&(*(RSD+21) != D_TI ))) 				//非冻结时间选项不支持,数据间隔需为空或TI
			{
				NoResponseFrame( COM_No );
				return 0;
			}	
			RAM_Write( OAD, RSD+1, 4 );
			RAM_Write( StartData, RSD+6, 7 );
			RAM_Write( EndData, RSD+14, 7 );
			Ptr = RSD + 21;
			if( *Ptr == null_data ) RCSD = Ptr + 1;
			else 
			{
				RAM_Write( Space+1, Ptr+1, 7 );	
				RCSD = Ptr + 8;
			}
			break;
		case 9:
			if( *(RSD+1) != D_Unsigned8 )
			{
				NoResponseFrame( COM_No );
				return 0;
			}	
			else
			{
				RCSD = RSD + 2;
				Space[0] = *(RSD+1);
			}	
			break;
		default:
			NoResponseFrame( COM_No );
			return 0;
			break;			
	}	

	Len = 0;
	ErrorByte = JudgeRCSD( COM_No, RCSD, &Len );
	RAM_Write( Dest, RCSD, Len );
	if( ErrorByte != 0 )										//不支持RCSD中的ROAD			
	{
		*(Dest+Len) = Get_data_access_result;					//异常标志
		*(Dest+Len+1) = object_undefined; 						//无对应的OBIS码
		Len += 2;
		return Len;
	}	
	Ptr = RCSD+Len;
*/
	RAM_Write( Dest, RCSD, RCSDLen );
	if( *RCSD == 0x00 )											//当RCSD为0，即全选时，需要把全部OAD调入Dest中		//16.10.22
	{																												//16.10.22
		RCSDLen = FillAllFreezeOADToRcsd( Dest, ItemNo );															//16.10.22
	}																												//16.10.22
	Len = RCSDLen;
	switch( *RSD )
	{
		case 1:
			Len += GetFreezeBufferDataSelector1( COM_No, Dest+RCSDLen, RSD, RCSD, ItemNo );
			break;
		case 2:
			
			//if( (*(RSD+21) == null_data )
			//{
			//	Len += GetFreezeBufferDataSelector2_Null( COM_No, Dest+RCSDLen, RSD, RCSD, ItemNo );
			//}
			//else
			//{			
			//	Len += GetFreezeBufferDataSelector2_TI( COM_No, Dest+RCSDLen, RSD, RCSD, ItemNo );
			//}
			Len += GetFreezeBufferDataSelector2( COM_No, Dest+RCSDLen, RSD, RCSD, ItemNo ); 	
			break;
		case 9:
			Len += GetFreezeBufferDataSelector9( COM_No, Dest+RCSDLen, RSD, RCSD, ItemNo );
			break;
		default:
			break;			
	}

	return Len;	
}	

short GetFreezeBufferDataSelector1( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo )
{
	unsigned short Len;
	
	if( JudgeSelectorTimeOAD( RSD+1, 0 ) == 0 )		//按冻结时间提取
	{
		Len = GetFreezeBufferDataSelector( COM_No, Dest, RSD, RCSD, ItemNo, 0 );	
	}
	else											//按冻结序号提取
	{
		Len = GetFreezeBufferDataSelector( COM_No, Dest, RSD, RCSD, ItemNo, 1 );	
	}
	
	return Len;
}	

short GetFreezeBufferDataSelector9( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo )
{
	unsigned char Buff[12];
	unsigned char SelectorData[8];
	unsigned char FreezeAux[14];			//辅助参数，时标或冻结序号
	unsigned char* Ptr;
	unsigned char* DestPtr;
	unsigned char* OAD;
        unsigned short EFinalRecNoAddr;
        unsigned short Items;
	unsigned short Addr;
	unsigned char* FreezeTimeAddr;
	unsigned char* FreezeNoAddr;
	unsigned short GetLen=0;
	unsigned char OADItems;
	unsigned char SelectorLen;
	unsigned char AuxLen;
	unsigned long FinalRecNo;
	short SelectType;
	short i,j;
	
	Ptr = Buff;
	
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	EFinalRecNoAddr = DL698_Freeze_ProfileTab[ItemNo].FinalRecNoAddr;
	
	OADItems = *RCSD;
	SelectType = 1;											//以冻结序号查询
	SelectorLen = 4;
	AuxLen = 11;
	E2P_RData( (unsigned char*)&FinalRecNo, EFinalRecNoAddr, 4 );
	FinalRecNo -= (*(RSD+1) - 1);
	RAM_Write( SelectorData, (unsigned char*)&FinalRecNo, SelectorLen );
	RAM_DataFill( FreezeAux, AuxLen,0xFF);

	GetDirTo_FBuff();							//将文件目录表放入FBuff,加速后续文件打开操作	//17.03.08

	OAD = RCSD + 2;	
	*Dest = Get_data;
//	*(Dest+1) = *RCSD;										//RecordRow数量
	*(Dest+1) = 0x01;										//RecordRow数量
	GetLen = 2;
	DestPtr = Dest+2;
	if( OADItems == 0 )
	{
//		*DestPtr = Data_structure;							//16.10.22
//		*(DestPtr+1) = Items + 2;							//16.10.22	
//		DestPtr += 2;                                       //16.10.22
//		GetLen += 2;			                            //16.10.22
		
		*DestPtr = D_Unsigned32;
		FreezeNoAddr = DestPtr + 1;
		DestPtr += 5;
		GetLen += 5;			
		
		*DestPtr = D_date_time_s;
		FreezeTimeAddr = DestPtr+1;
		DestPtr += 8;
		GetLen += 8;

		for( i=0;i<Items;i++ )
		{
			Get_single_freeze_content( DestPtr, &GetLen, SelectorData, FreezeAux, ItemNo, i, SelectType );
			DestPtr = Dest + GetLen;
		}
	}
	else
	{	
		FreezeTimeAddr = 0;
		FreezeNoAddr = 0;
		for(j=0;j<OADItems;j++)
		{
//			FreezeTimeAddr = 0;
//			FreezeNoAddr = 0;
			if( JudgeSelectorTimeOAD( OAD, 0 ) == 0 )
			{
//				if( SelectType == 0 )		//按时标
//				{
//					RAM_Write( DestPtr+1, SelectorData, SelectorLen );
//				}
//				else
//				{
//					*FreezeAux = D_Unsigned32;
//					RAM_DataFill( FreezeAux+1, AuxLen); 
//					*DestPtr = D_Unsigned32;
//					FreezeNoAddr = DestPtr;
//				}
				*DestPtr = D_date_time_s;
				FreezeTimeAddr = DestPtr+1;
				DestPtr += 8;
				GetLen += 8;
			}
			else if( JudgeSelectorNo_OAD( OAD, 0 ) == 0 )
			{
//				if( SelectType == 0 )		//按时标
//				{
//					*FreezeAux = D_date_time_s;
//					RAM_DataFill( FreezeAux+1, AuxLen); 
//					*DestPtr = D_date_time_s;
//					FreezeNoAddr = DestPtr;
//				}
//				else
//				{
//					RAM_Write( DestPtr+1, SelectorData, SelectorLen );
//				}
				*DestPtr = D_Unsigned32;
				FreezeNoAddr = DestPtr + 1;
				DestPtr += 5;
				GetLen += 5;			
			}
			else
			{
				for( i=0;i<Items;i++ )
				{
					E2P_RData( Ptr, Addr+i*10, 9 );
					if( Data_Comp( Ptr+2, OAD, 4 ) == 0 )  
					{
						Get_single_freeze_content( DestPtr, &GetLen, SelectorData, FreezeAux, ItemNo, i, SelectType );
						DestPtr = Dest + GetLen;
						break;
					}	
//					else
//					{
//						Fill_null_data( DestPtr, &GetLen );
//						DestPtr = Dest + GetLen;
//					}
				}
				if( i == Items )							//无此0AD项		//17.05.05
				{
					Fill_null_data( DestPtr, &GetLen );						//17.05.05
					DestPtr = Dest + GetLen;								//17.05.05
				}	
			}	
			OAD += 5;
		}
	}				
//	if(( *FreezeAux == D_Unsigned32 )||( *FreezeAux == D_date_time_s ))
//	{
//		RAM_Write( FreezeNoAddr+1, FreezeAux+1, AuxLen );
//	}	
	if( IsAllData( FreezeAux, 11, 0xff ) != 0 )
	{
		if( FreezeTimeAddr != 0 ) 
		{
			RAM_Write( FreezeTimeAddr, FreezeAux, 7 );
			SwapData( FreezeTimeAddr, 7 );
		}
		if( FreezeNoAddr != 0 ) 
		{
			RAM_Write( FreezeNoAddr, FreezeAux+7, 4 );
			SwapData( FreezeNoAddr, 4 );
		}
	}	
	else
	{
		GetLen = 2;	
		*(Dest+1) = 0x00;								//RecordRow数量	无冻结数据
		*CommPara[COM_No].SubsBlockPara = 0;				//无后续帧
	}
	return GetLen;
}

//SelectType 数据选择类型 0：按时标；1：按冻结序号
short GetFreezeBufferDataSelector( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo, short SelectType )
{
	unsigned char Buff[12];
	unsigned char SelectorData[8];
	unsigned char FreezeAux[14];			//辅助参数，时标或冻结序号
	unsigned char* Ptr;
	unsigned char* DestPtr;
	unsigned char* OAD;
	unsigned short Items;
	unsigned short Addr;
	unsigned char* FreezeTimeAddr;
	unsigned char* FreezeNoAddr;
	unsigned short GetLen=0;
	unsigned char OADItems;
	unsigned char SelectorLen;
	unsigned char AuxLen;
	short i,j;
	
	Ptr = Buff;
	
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	
	OADItems = *RCSD;
	if( SelectType == 0 )
	{
		SelectorLen = 7;
		AuxLen = 11;
	}
	else
	{
		SelectorLen = 4;
		AuxLen = 11;
	}	
	RAM_Write( SelectorData, RSD+6, SelectorLen );
	SwapData( SelectorData, SelectorLen );					//高低位交换
	RAM_DataFill( FreezeAux, AuxLen,0xFF);

	GetDirTo_FBuff();							//将文件目录表放入FBuff,加速后续文件打开操作	//17.03.08

	OAD = RCSD + 2;	
	*Dest = Get_data;
//	*(Dest+1) = *RCSD;										//RecordRow数量
	*(Dest+1) = 0x01;										//RecordRow数量
	GetLen = 2;
	DestPtr = Dest+2;
	if( OADItems == 0 )
	{
//		*DestPtr = Data_structure;							//16.10.22
//		*(DestPtr+1) = Items + 2;							//16.10.22
//		DestPtr += 2;                                       //16.10.22
//		GetLen += 2;			                            //16.10.22

		*DestPtr = D_Unsigned32;
		FreezeNoAddr = DestPtr + 1;
		DestPtr += 5;
		GetLen += 5;			
		
		*DestPtr = D_date_time_s;
		FreezeTimeAddr = DestPtr+1;
		DestPtr += 8;
		GetLen += 8;

		for( i=0;i<Items;i++ )
		{
			Get_single_freeze_content( DestPtr, &GetLen, SelectorData, FreezeAux, ItemNo, i, SelectType );
			DestPtr = Dest + GetLen;
		}
	}
	else
	{	
		FreezeTimeAddr = 0;
		FreezeNoAddr = 0;
		for(j=0;j<OADItems;j++)
		{
			if( JudgeSelectorTimeOAD( OAD, 0 ) == 0 )
			{
//				if( SelectType == 0 )		//按时标
//				{
//					RAM_Write( DestPtr+1, SelectorData, SelectorLen );
    	
//				}
//				else
//				{
//					*FreezeAux = D_Unsigned32;
//					RAM_DataFill( FreezeAux+1, AuxLen); 
//					*DestPtr = D_Unsigned32;
//					FreezeNoAddr = DestPtr+1;
//				}
    	
				*DestPtr = D_date_time_s;
				FreezeTimeAddr = DestPtr+1;
				DestPtr += 8;
				GetLen += 8;
			}
			else if( JudgeSelectorNo_OAD( OAD, 0 ) == 0 )
			{
//				if( SelectType == 0 )		//按时标
//				{
//					*FreezeAux = D_date_time_s;
//					RAM_DataFill( FreezeAux+1, AuxLen); 
//					*DestPtr = D_date_time_s;
//					FreezeNoAddr = DestPtr;
//				}
//				else
//				{
//					RAM_Write( DestPtr+1, SelectorData, SelectorLen );
//				}
				*DestPtr = D_Unsigned32;
				FreezeNoAddr = DestPtr + 1;
				DestPtr += 5;
				GetLen += 5;			
			}
			else
			{
				for( i=0;i<Items;i++ )
				{
					E2P_RData( Ptr, Addr+i*10, 9 );
					if( Data_Comp( Ptr+2, OAD, 4 ) == 0 )  
					{
						Get_single_freeze_content( DestPtr, &GetLen, SelectorData, FreezeAux, ItemNo, i, SelectType );
						DestPtr = Dest + GetLen;
						break;
					}	
//					else
//					{
//						Fill_null_data( DestPtr, &GetLen );
//						DestPtr = Dest + GetLen;
//					}
				}
				if( i == Items )							//无此0AD项		//17.05.05
				{
					Fill_null_data( DestPtr, &GetLen );						//17.05.05
					DestPtr = Dest + GetLen;								//17.05.05
				}	
			}	
			OAD += 5;
		}
	}				
//	if(( *FreezeAux == D_Unsigned32 )||( *FreezeAux == D_date_time_s ))
//	{
//		RAM_Write( FreezeNoAddr+1, FreezeAux+1, AuxLen );
//	}
	if( IsAllData( FreezeAux, 11, 0xff ) != 0 )
	{
		if( FreezeTimeAddr != 0 ) 
		{
			RAM_Write( FreezeTimeAddr, FreezeAux, 7 );
			SwapData( FreezeTimeAddr, 7 );
		}
		if( FreezeNoAddr != 0 ) 
		{
			RAM_Write( FreezeNoAddr, FreezeAux+7, 4 );
			SwapData( FreezeNoAddr, 4 );
		}
	}
	else
	{
		GetLen = 2;	
		*(Dest+1) = 0x00;								//RecordRow数量	无冻结数据
		*CommPara[COM_No].SubsBlockPara = 0;				//无后续帧
	}
	
	return GetLen;
}

/*
short GetFreezeBufferDataSelector1( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo )
{
	unsigned char Buff[12];
	unsigned char FreezeTime[8];
	unsigned char FreezeNo[5];
	unsigned char* Ptr;
	unsigned char* DestPtr;
	unsigned char* OAD;
	unsigned short Items;
	unsigned short Addr;
	unsigned short FreezeNoAddr;
	unsigned short GetLen=0;
	unsigned char OADItems;
	short i,j;
	
	Ptr = Buff;
	
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	
	OADItems = *RCSD;
	SwapData( RSD+6, 7 );									//时间的高低位交换
	RAM_Write( FreezTime, RSD+6, 7 );
	RAM_Fill( FreezeNo, 5);
	OAD = RCSD + 2;	
	*Dest = Get_data;
	*(Dest+1) = *RCSD;										//RecordRow数量
	GetLen = 2;
	DestPtr = Dest+2;
	for(j=0;j<OADItems;j++)
	{
		if(( JudgeSelectorOAD( OAD ) ) == 0 )
		{
			*DestPtr = D_date_time_s;
			RAM_Write( DestPtr, FreezTime, 7 );
			DestPtr += 8;
			GetLen += 8;
		}	
		else if( IsFreezeNoOAD( OAD ) == 0 )
		{
			FreezeNo[0] = D_Unsigned32;
			*DestPtr = D_Unsigned32;
			RAM_DataFill( FreezeNo+1, 4);
			FreezeNoAddr = DestPtr;
			DestPtr += 5;
			GetLen += 5;
		}
		else
		{
			for( i=0;i<Items;i++ )
			{
				E2P_RData( Ptr, Addr+i*10, 9 );
				if( Data_Comp( Ptr+2, OAD, 4 ) == 0 )  
				{
//					Get_single_freeze_content( DestPtr, &GetLen, OAD, FreezTime, FreezeNo, ItemNo, i );
					Get_single_freeze_content( DestPtr, &GetLen, FreezTime, FreezeNo, ItemNo, i );
					DestPtr = Dest + GetLen;
					break;
				}	
				else
				{
					Fill_null_data( DestPtr, &GetLen );
					DestPtr = Dest + GetLen;
				}
			}
		}	
		OAD += 4;
	}	
	if( FreezeNo[0] == D_Unsigned32 )
	{
		RAM_Write( FreezeNoAddr+1, FreezTime+1, 4 );
	}	
}
*/

//short GetFreezeBufferDataSelector2_TI( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo )
//{
//	unsigned char Buff[12];
//	unsigned char FreezeTime[16];
////	unsigned char* FreezeTime;
//	unsigned char FreezeNo[5];
//	unsigned char* Ptr;
//	unsigned char* DestPtr;
//	unsigned char* OAD;
//	unsigned short Items;
//	unsigned short Addr;
//	unsigned short FreezeNoAddr;
//	unsigned short GetLen=0;
//	unsigned char OADItems;
//	unsigned char Space[4];
//	unsigned char FreezeTimeFlag=0;
//	unsigned char FreezeNoFlag=0;	
//	unsigned short Len;
//	short i,j;
//	
//	Ptr = Buff;
//	
//	Items = GetFreezeCaptureItems( ItemNo );
//	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
//	
//	OADItems = *RCSD;
////	SwapData( RSD+6, 7 );									//时间的高低位交换
////	SwapData( RSD+14, 7 );									//时间的高低位交换
//	RAM_Write( FreezTime, RSD+6, 7 );
//	RAM_Write( FreezTime+7, RSD+14, 7 );
//	SwapData( FreezTime, 7 );									//时间的高低位交换
//	SwapData( FreezTime+7, 7 );									//时间的高低位交换
//	RAM_Fill( FreezeNo, 5);
////	RAM_Fill( Space, 4);
//	RAM_Write( Space, RSD+21, 4 );
//
//	OAD = RCSD + 2;	
//	*Dest = Get_data;
//	*(Dest+1) = *RCSD;										//RecordRow数量
//	GetLen = 2;
//	DestPtr = Dest+2;
//	
//	while( Data_Comp( FreezeTime+7, FreezeTime, 7 ) > 0 )
//	{
//		for(j=0;j<OADItems;j++)
//		{
//			if(( JudgeSelectorOAD( OAD ) ) == 0 )
//			{
//				*DestPtr = D_date_time_s;
//				RAM_Write( DestPtr, FreezTime, 7 );
//				DestPtr += 8;
//				GetLen += 8;
//				FreezeTimeFlag = 1;
//			}	
//			else if( IsFreezeNoOAD( OAD ) == 0 )
//			{
//				FreezeNo[0] = D_Unsigned32;
//				*DestPtr = D_Unsigned32;
//				RAM_DataFill( FreezeNo+1, 4);
//				FreezeNoAddr = DestPtr;
//				DestPtr += 5;
//				GetLen += 5;
//				FreezeNoFlag = 1;
//			}
//			else
//			{
//				for( i=0;i<Items;i++ )
//				{
//					E2P_RData( Ptr, Addr+i*10, 9 );
//					if( Data_Comp( Ptr+2, OAD, 4 ) == 0 )  
//					{
////						Get_single_freeze_content( DestPtr, &GetLen, OAD, FreezTime, FreezeNo, ItemNo, i );
//						Get_single_freeze_content( DestPtr, &GetLen, FreezTime, FreezeNo, ItemNo, i );
//						DestPtr = Dest + GetLen;
//						break;
//					}	
//					else
//					{
//						Fill_null_data( DestPtr, &GetLen );
//						DestPtr = Dest + GetLen;
//					}
//				}
//			}	
//			OAD += 4;
//		}	
//		if( FreezeNo[0] == D_Unsigned32 )
//		{
//			RAM_Write( FreezeNoAddr+1, FreezTime+1, 4 );
//		}
//		
//		Datetimes_Add_TI( FreezTime, FreezTime, Space[1], SwapCharToShort( Space+2 ) );
//
//		Len = GetLen - 2;
//		if( FreezeTimeFlag == 1 ) Len -= 8;
//		if( FreezeNoFlag == 1 ) Len -= 5;
//		Len /= 2;
//		if( Len <= OADItems )							//判断除了冻结时间和序号外，其他项是否都是空白
//		{
//			Datetimes_Add_TI( FreezTime, FreezTime, Space[1], SwapCharToShort( Space+2 ) );
//			GetLen = 2;
//			DestPtr = Dest+2;
//		}
//		else 
//		{
//			CommPara[COM_No].SubsBlockPara = 1;			//还有后续帧
//			break;										//已至少有一组数据
//		}
//	}
//	if( GetLen == 2 )	
//	{
//		*(Dest+1) = 0x00;								//RecordRow数量	无冻结数据
//	}
//
//	SwapData( FreezTime, 7 );							//时间的高低位交换
//	SwapData( FreezTime+7, 7 );							//时间的高低位交换
//	RAM_Write( RSD+6, FreezTime, , 7 );					//将更新后的冻结起始时间放回RSD,以备下次使用
//	
//	return GetLen;
//}	

unsigned char JudgeSearchTimeout( unsigned long StartTime )
{
	unsigned long EndTime;
	
	EndTime = SM.TimeTest;							//取一轮搜索后时间										//14.06.23
	if( StartTime > EndTime )
	{
		EndTime = 0xFFFFFFFF - StartTime + EndTime;	//可能定时器已翻转										//14.06.23
	}	
	else EndTime -= StartTime;						//计算搜索耗时											//14.06.23
//	if( EndTime > 8000 ) return 1;					//为保证500ms响应时间，搜索超过400.5ms就分帧			//14.06.23
	if( EndTime > 40000 ) return 1;					//为搜索完所有数据，搜索超过10秒就分帧			//14.06.23
	
	return 0;
}	
/*
short GetFreezeBufferDataSelector2( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo )
{

	unsigned char Buff[12];
	unsigned char SelectorData[18];
	unsigned char FreezeAux[14];			//辅助参数，时标或冻结序号
	unsigned char* Ptr;
	unsigned char* DestPtr;
	unsigned char* OAD;
	unsigned short Items;
	unsigned short Addr;
	unsigned char* FreezeTimeAddr;
	unsigned char* FreezeNoAddr;
	unsigned short GetLen=0;
	unsigned char OADItems;
	unsigned char SelectorLen;
	unsigned short SelectType;
	unsigned char AuxLen;
	unsigned char Space[5];
//	unsigned char FreezeTimeFlag=0;
//	unsigned char FreezeNoFlag=0;	
	unsigned long RecNo;
	unsigned long RecNoInc;
	short i,j;
	unsigned long StartTime;						//14.06.23
	
	Ptr = Buff;
	
	
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	
	OADItems = *RCSD;
	if( JudgeSelectorTimeOAD( RSD+1, 0 ) == 0 ) SelectType = 0;		//按冻结时间提取
	else SelectType = 1;											//按冻结序号提取	
	
	if( SelectType == 0 )
	{
		SelectorLen = 7;
		AuxLen = 11;
	}
	else
	{
		SelectorLen = 4;
		AuxLen = 11;
	}	
	RAM_Write( SelectorData, RSD+6, SelectorLen );
	RAM_Write( SelectorData+SelectorLen, RSD+6+SelectorLen+1, SelectorLen );
	SwapData( SelectorData, SelectorLen );					//高低位交换
	SwapData( SelectorData+SelectorLen, SelectorLen );		//高低位交换
	RAM_DataFill( FreezeAux, AuxLen,0xFF);
	RAM_Write( Space, RSD+5+(SelectorLen+1)*2, 5 );			//可能为TI或unsigned32,至少取5字节
	
	if(( SelectType == 0 )&&( Space[0] == null_data ))		//以时标选择下，间隔为空不支持
	{
		*CommPara[COM_No].SubsBlockPara = 0;			//无后续帧
		*Dest = Get_data_access_result;
		if( GetLen == 2 )	
		{
			*(Dest+1) = type_unmatched;								//RecordRow数量	无冻结数据
		}
		return GetLen;
	}

	if( SelectType == 1 )
	{
		RAM_Write( (unsigned char*)&RecNo, SelectorData, SelectorLen );
		if( Space[0] == null_data ) RecNoInc = 1;
		else 
		{
			SwapData( Space+1, SelectorLen );
			RAM_Write( (unsigned char*)&RecNoInc, Space+1, SelectorLen );
		}	
	}

	OAD = RCSD + 2;	
	*Dest = Get_data;
//	*(Dest+1) = *RCSD;										//RecordRow数量
	*(Dest+1) = 0x01;										//RecordRow数量
	GetLen = 2;
	DestPtr = Dest+2;
	StartTime = SM.TimeTest;								//保存搜索前时间	//14.06.23
	while( Data_Comp( SelectorData+SelectorLen, SelectorData, SelectorLen ) > 0 )
	{
		OAD = RCSD + 2;										//重置OAD位置
		if( OADItems == 0 )
		{
			*DestPtr = Data_structure;						//16.10.22
			*(DestPtr+1) = Items + 2;
			DestPtr += 2;
			GetLen += 2;			

			*DestPtr = D_Unsigned32;
			FreezeNoAddr = DestPtr + 1;
			DestPtr += 5;
			GetLen += 5;			
			
			*DestPtr = D_date_time_s;
			FreezeTimeAddr = DestPtr+1;
			DestPtr += 8;
			GetLen += 8;
	
			for( i=0;i<Items;i++ )
			{
				Get_single_freeze_content( DestPtr, &GetLen, SelectorData, FreezeAux, ItemNo, i, SelectType );
				DestPtr = Dest + GetLen;
			}
		}
		else
		{	
			FreezeTimeAddr = 0;
			FreezeNoAddr = 0;
			for(j=0;j<OADItems;j++)
			{
				if( JudgeSelectorTimeOAD( OAD, 0 ) == 0 )
				{
//					if( SelectType == 0 )		//按时标
//					{
//						RAM_Write( DestPtr+1, SelectorData, SelectorLen );
//					}
//					else
//					{
//						*FreezeAux = D_Unsigned32;
//						RAM_DataFill( FreezeAux+1, AuxLen); 
//						*DestPtr = D_Unsigned32;
//						FreezeNoAddr = DestPtr;
//					}
					*DestPtr = D_date_time_s;
					FreezeTimeAddr = DestPtr+1;
					DestPtr += 8;
					GetLen += 8;
//					FreezeTimeFlag = 1;
				}
				else if( JudgeSelectorNo_OAD( OAD, 0 ) == 0 )
				{
//					if( SelectType == 0 )		//按时标
//					{
//						*FreezeAux = D_date_time_s;
//						RAM_DataFill( FreezeAux+1, AuxLen); 
//						*DestPtr = D_date_time_s;
//						FreezeNoAddr = DestPtr;
//					}
//					else
//					{
//						RAM_Write( DestPtr+1, SelectorData, SelectorLen );
//					}
					*DestPtr = D_Unsigned32;
					FreezeNoAddr = DestPtr + 1;
					DestPtr += 5;
					GetLen += 5;	
//					FreezeNoFlag = 1;		
				}
				else
				{
					for( i=0;i<Items;i++ )
					{
						E2P_RData( Ptr, Addr+i*10, 9 );
						if( Data_Comp( Ptr+2, OAD, 4 ) == 0 )  
						{
							Get_single_freeze_content( DestPtr, &GetLen, SelectorData, FreezeAux, ItemNo, i, SelectType );
							DestPtr = Dest + GetLen;
							break;
						}	
//						else
//						{
//							Fill_null_data( DestPtr, &GetLen );
//							DestPtr = Dest + GetLen;
//						}
					}
				}	
				OAD += 5;
			}
		}					
//		if(( *FreezeAux == D_Unsigned32 )||( *FreezeAux == D_date_time_s ))
//		{
//			RAM_Write( FreezeNoAddr+1, FreezeAux+1, AuxLen );
//		}	
		if( IsAllData( FreezeAux, 11, 0xff ) != 0 )
		{
			if( FreezeTimeAddr != 0 ) 
			{
				RAM_Write( FreezeTimeAddr, FreezeAux, 7 );
				SwapData( FreezeTimeAddr, 7 );
			}
			if( FreezeNoAddr != 0 ) 
			{
				RAM_Write( FreezeNoAddr, FreezeAux+7, 4 );
				SwapData( FreezeNoAddr, 4 );
			}
		}

		if(( SelectType == 0 )&&( Space[0] == D_TI ))
		{
			Datetimes_Add_TI( SelectorData, SelectorData, Space[1], SwapCharToShort( Space+2 ) );
		}
		else if( SelectType == 1 )
		{
			RecNo += RecNoInc;
			RAM_Write( SelectorData, (unsigned char*)&RecNo, SelectorLen );
		}
		else break;

//		Len = GetLen - 2;
//		if( FreezeTimeFlag == 1 ) Len -= 8;
//		if( FreezeNoFlag == 1 ) Len -= 5;
//		Len /= 2;
//		if( Len <= OADItems )							//判断除了冻结时间和序号外，其他项是否都是空白
		if( IsAllData( FreezeAux, 11, 0xff ) == 0 )		//无有效数据
		{
//			Datetimes_Add_TI( FreezTime, FreezTime, Space[1], SwapCharToShort( Space+2 ) );
			GetLen = 2;
			DestPtr = Dest+2;
		}
		else 
		{
			if( Data_Comp( SelectorData+SelectorLen, SelectorData, SelectorLen ) > 0 )
			{				
				*CommPara[COM_No].SubsBlockPara = 1;			//还有后续帧
			}else *CommPara[COM_No].SubsBlockPara = 0;			//无后续帧				
			break;										//已至少有一组数据
		}

		if( JudgeSearchTimeout( StartTime ) != 0 ) break;		//超时，退出搜索

	}
	if( GetLen == 2 )	
	{
		*(Dest+1) = 0x00;								//RecordRow数量	无冻结数据
		*CommPara[COM_No].SubsBlockPara = 0;				//无后续帧
	}

	SwapData( SelectorData, SelectorLen );				//选择数据的高低位交换
	RAM_Write( RSD+6, SelectorData, SelectorLen );		//将更新后的冻结起始时间放回RSD,以备下次使用
	
	return GetLen;

}
*/

//取一条冻结记录的最大长度（包括RCSD），用于通讯缓存判断，以免溢出
unsigned short GetFreezeMaxRcsd_RecLen( short ItemNo )		//17.05.12
{
    unsigned short Items;
	unsigned short Addr;
    unsigned short Len;
    unsigned short Length;
    short i;
	
	Len = 1;
	Items = GetFreezeCaptureItems( ItemNo );
	Len += Items*5;																//全部RCSD长度	
	Addr = DL698_Freeze_ProfileTab[ItemNo].RecdLenAddr;
	for( i=0;i<Items;i++ )
	{
		E2P_RData( (unsigned char*)&Length, Addr, 2 );
		if( Length >= 11 ) Length -= 11;										//去掉每项的时标、序号
		Len += Length;	
	}
	Len += 11;																	//增加一项时标、序号
	
	if( Len > (MAX_PDU_SIZE - 50) ) Len = MAX_PDU_SIZE - 50;					//无效数据长度	
	
	return Len;
}	


short GetFreezeBufferDataSelector2( short COM_No, unsigned char* Dest, unsigned char* RSD, unsigned char* RCSD, short ItemNo )
{

	unsigned char Buff[12];
	unsigned char MinD[10];
	unsigned char SelectorData[18];
	unsigned char FreezeAux[14];			//辅助参数，时标或冻结序号
	unsigned char* Ptr;
	unsigned char* DestPtr;
	unsigned char* OAD;
	unsigned char* MinData;
	unsigned short Items;
	unsigned short Addr;
	unsigned char* FreezeTimeAddr;
	unsigned char* FreezeNoAddr;
	unsigned short GetLen=0;
	unsigned char OADItems;
	unsigned char SelectorLen;
	unsigned short SelectType;
	unsigned char AuxLen;
	unsigned char Space[5];
//	unsigned char FreezeTimeFlag=0;
//	unsigned char FreezeNoFlag=0;	
	unsigned long RecNo;
	unsigned long RecNoInc;
	short i,j;
	unsigned long StartTime;						//14.06.23
	unsigned short ItemSTLen;						//每一轮搜索起始时的总长度		//16.10.27
	unsigned char* ItemSTDestPtr;					//每一轮搜索起始时的数据首址	//16.10.27
	unsigned short NullItems=0;						//不存在OAD数量					//17.05.05
	unsigned short MaxRcsd_RecLen=0;				//冻结记录的最大长度（包括RCSD）//17.05.12
	
	Ptr = Buff;
	MinData = MinD;
	
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	
	OADItems = *RCSD;
	if( JudgeSelectorTimeOAD( RSD+1, 0 ) == 0 ) SelectType = 0;		//按冻结时间提取
	else SelectType = 1;											//按冻结序号提取	
	
	if( SelectType == 0 )
	{
		SelectorLen = 7;
		AuxLen = 11;
	}
	else
	{
		SelectorLen = 4;
		AuxLen = 11;
	}	
	RAM_Write( SelectorData, RSD+6, SelectorLen );
	RAM_Write( SelectorData+SelectorLen, RSD+6+SelectorLen+1, SelectorLen );
	SwapData( SelectorData, SelectorLen );					//高低位交换
	SwapData( SelectorData+SelectorLen, SelectorLen );		//高低位交换
	RAM_DataFill( FreezeAux, AuxLen,0xFF);
	RAM_Write( Space, RSD+5+(SelectorLen+1)*2, 5 );			//可能为TI或unsigned32,至少取5字节
	
	if(( SelectType == 0 )&&( Space[0] == null_data ))		//以时标选择下，间隔为空不支持
	{
//		*CommPara[COM_No].SubsBlockPara = 0;			//无后续帧
//		*Dest = Get_data_access_result;
//		if( GetLen == 2 )	
//		{
//			*(Dest+1) = type_unmatched;								//RecordRow数量	无冻结数据
//		}
//		return GetLen;
		Space[0] = D_TI;
		Space[1] = 0x00;
		Space[2] = 0x00;
		Space[3] = 0x00;		
	}

	if( SelectType == 1 )
	{
		RAM_Write( (unsigned char*)&RecNo, SelectorData, SelectorLen );
		if( Space[0] == null_data ) RecNoInc = 1;
		else 
		{
			SwapData( Space+1, SelectorLen );
			RAM_Write( (unsigned char*)&RecNoInc, Space+1, SelectorLen );
			if( RecNoInc == 0 ) RecNoInc = 1;				//间隔为0时步距为1						//16.10.27		
		}	
	}

	GetDirTo_FBuff();							//将文件目录表放入FBuff,加速后续文件打开操作	//17.03.08

	OAD = RCSD + 2;	
	*Dest = Get_data;
//	*(Dest+1) = *RCSD;										//RecordRow数量
//	*(Dest+1) = 0x01;										//RecordRow数量
	*(Dest+1) = 0x00;										//RecordRow数量		//16.10.27
	GetLen = 2;
	DestPtr = Dest+2;
	StartTime = SM.TimeTest;								//保存搜索前时间	//14.06.23
	
	if( SelectType == 1 )																			//16.10.27		//迅速找到最贴近目标的最小值，跨过大量的无效搜寻
	{
		Get_freeze_First_content( MinData, SelectorData, OAD, OADItems, ItemNo, SelectType );	//16.10.27
		RAM_Write( SelectorData, MinData, SelectorLen );											//16.10.27		
	}	
	
	MaxRcsd_RecLen = GetFreezeMaxRcsd_RecLen( ItemNo );				//17.05.12
	
	while( Data_Comp( SelectorData+SelectorLen, SelectorData, SelectorLen ) > 0 )
	{
		ItemSTLen = GetLen;						//每一轮搜索起始时的总长度		//16.10.27
		ItemSTDestPtr = DestPtr;				//每一轮搜索起始时的数据首址	//16.10.27
		RAM_DataFill( FreezeAux, AuxLen,0xFF);				//准备下一轮初始化

		OAD = RCSD + 2;										//重置OAD位置
		
		if(( SelectType == 0 )&&( SwapCharToShort( Space+2 ) == 0 ))									//16.10.27	//时间间隔为0时才搜索最小一个，非零时搜指定时间
		{
			Get_freeze_First_content( MinData, SelectorData, OAD, OADItems, ItemNo, SelectType );		//16.10.27
			if( IsAllData( MinData, SelectorLen, 0xff ) == 0 ) 
			{
				*CommPara[COM_No].SubsBlockPara = 0;		//无后续帧				
				break;									//16.10.27
			}	
			else
			{
				RAM_Write( SelectorData, MinData, SelectorLen );										//16.10.27
			}
		}		
		
		if( OADItems == 0 )
		{
//			*DestPtr = Data_structure;						//16.10.22
//			*(DestPtr+1) = Items + 2;						//16.10.22
//			DestPtr += 2;                                   //16.10.22
//			GetLen += 2;			                        //16.10.22

			*DestPtr = D_Unsigned32;
			FreezeNoAddr = DestPtr + 1;
			DestPtr += 5;
			GetLen += 5;			
			
			*DestPtr = D_date_time_s;
			FreezeTimeAddr = DestPtr+1;
			DestPtr += 8;
			GetLen += 8;
	
			for( i=0;i<Items;i++ )
			{
				Get_single_freeze_content( DestPtr, &GetLen, SelectorData, FreezeAux, ItemNo, i, SelectType );
				DestPtr = Dest + GetLen;
			}
		}
		else
		{	
			FreezeTimeAddr = 0;
			FreezeNoAddr = 0;
			for(j=0;j<OADItems;j++)
			{
				if( JudgeSelectorTimeOAD( OAD, 0 ) == 0 )
				{
					*DestPtr = D_date_time_s;
					FreezeTimeAddr = DestPtr+1;
					DestPtr += 8;
					GetLen += 8;
				}
				else if( JudgeSelectorNo_OAD( OAD, 0 ) == 0 )
				{
					*DestPtr = D_Unsigned32;
					FreezeNoAddr = DestPtr + 1;
					DestPtr += 5;
					GetLen += 5;	
				}
				else
				{
					for( i=0;i<Items;i++ )
					{
						E2P_RData( Ptr, Addr+i*10, 9 );
						if( Data_Comp( Ptr+2, OAD, 4 ) == 0 )  
						{
							Get_single_freeze_content( DestPtr, &GetLen, SelectorData, FreezeAux, ItemNo, i, SelectType );
							DestPtr = Dest + GetLen;
							break;
						}	
					}
					if( i == Items )							//无此0AD项		//17.05.05
					{
						Fill_null_data( DestPtr, &GetLen );						//17.05.05
						DestPtr = Dest + GetLen;								//17.05.05
						NullItems += 1;											//17.05.05	
					}	
				}	
				OAD += 5;
			}
		}					
		if( IsAllData( FreezeAux, 11, 0xff ) != 0 )
		{
			if( FreezeTimeAddr != 0 ) 
			{
				RAM_Write( FreezeTimeAddr, FreezeAux, 7 );
				SwapData( FreezeTimeAddr, 7 );
			}
			if( FreezeNoAddr != 0 ) 
			{
				RAM_Write( FreezeNoAddr, FreezeAux+7, 4 );
				SwapData( FreezeNoAddr, 4 );
			}
		}

		if(( SelectType == 0 )&&( Space[0] == D_TI ))
		{
			if( SwapCharToShort( Space+2 ) == 0 )
			{
				Datetimes_Add_TI( SelectorData, SelectorData, 0x00, 0x0001 );							//间隔为0时增加1秒	
			}
			else Datetimes_Add_TI( SelectorData, SelectorData, Space[1], SwapCharToShort( Space+2 ) );
		}
		else if( SelectType == 1 )
		{
			RAM_Write( (unsigned char*)&RecNo, SelectorData, SelectorLen );
			RecNo += RecNoInc;
			RAM_Write( SelectorData, (unsigned char*)&RecNo, SelectorLen );
		}
		else break;

		if( IsAllData( FreezeAux, 11, 0xff ) == 0 )		//无有效数据
		{
//			GetLen = 2;
//			DestPtr = Dest+2;
			GetLen = ItemSTLen;							//回归至本轮搜索起始时的总长度		//16.10.27
			DestPtr = ItemSTDestPtr;					//回归至本轮搜索起始时的数据首址	//16.10.27
			*CommPara[COM_No].SubsBlockPara = 0;		//无后续帧				
		}
		else 
		{
//			if( Data_Comp( SelectorData+SelectorLen, SelectorData, SelectorLen ) > 0 )
//			{				
//				*CommPara[COM_No].SubsBlockPara = 1;			//还有后续帧
//			}else *CommPara[COM_No].SubsBlockPara = 0;			//无后续帧				
//			break;										//已至少有一组数据
			*(Dest+1) += 1;										//RecordRow数量		//16.10.27
			if( Data_Comp( SelectorData+SelectorLen, SelectorData, SelectorLen ) > 0 )
			{				
				*CommPara[COM_No].SubsBlockPara = 1;			//还有后续帧
//				if( GetLen > ( MAX_PDU_SIZE - 150 ) )			//超长，用后续帧
				if( GetLen > ( MAX_PDU_SIZE - 50 - MaxRcsd_RecLen ) )		//超长，用后续帧
				{
					break;
				}	
			}else
			{
				*CommPara[COM_No].SubsBlockPara = 0;			//无后续帧				
				break;										//已至少有一组数据
			}	
		}

		if( JudgeSearchTimeout( StartTime ) != 0 ) break;		//超时，退出搜索

	}
//	if( GetLen == 2 )	
	if( GetLen == (NullItems+2) )								//17.05.05
	{
		*(Dest+1) = 0x00;								//RecordRow数量	无冻结数据
		*CommPara[COM_No].SubsBlockPara = 0;				//无后续帧
	}

	SwapData( SelectorData, SelectorLen );				//选择数据的高低位交换
	RAM_Write( RSD+6, SelectorData, SelectorLen );		//将更新后的冻结起始时间放回RSD,以备下次使用
	
	return GetLen;

}


void Fill_null_data( unsigned char* Dest, unsigned short* GetLen )
{
	*Dest = null_data;
	*GetLen += 1;
}

/*
//void Get_single_freeze_content( unsigned char* DestPtr, unsigned short* GetLen, unsigned char* OAD, unsigned char* FreezTime, unsigned char* FreezeNo, unsigned short ItemNo, unsigned short CurveNo )
void Get_single_freeze_content( unsigned char* DestPtr, unsigned short* GetLen, unsigned char* SelectorData, unsigned char* FreezeAux, unsigned short ItemNo, unsigned short CurveNo, short SelectType )
{
	unsigned char Buff2[20];
//	unsigned char* Point;
	unsigned char* FileName;
    unsigned long LongBuff[8];
//    unsigned long WriteLen;
	FILECURVEPARA* FileCurvePara;
	unsigned char ErrorByte=0;
	long LoadAddr;
	long LoadRecNo;
        short Len;
	
//	Point = Buff;
	FileName = Buff2;
	FileCurvePara = (FILECURVEPARA*)LongBuff;
	
	FileCurvePara->ChannelNo = ItemNo;
	FileCurvePara->ChannelNo <<= 8;
	FileCurvePara->ChannelNo += CurveNo;
	GetFileCurvePara( FileCurvePara );

	if( FileCurvePara->AvailRecs == 0 ) 
	{
		Fill_null_data( DestPtr, GetLen );
		return;							//没有有效记录,返回空数据。	
	}
	
	CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );
	ErrorByte = f_open( &fsrc, (char const*)FileName, FA_READ);						//此处打开文件后，在Read_File中不再每次打开，避免多次打开，除非读取备用文件后恢复打开
	if( ErrorByte == FR_OK )
	{
		LoadRecNo = GetFreezeLoadPWAds( DestPtr, FileCurvePara, SelectorData, SelectType );
		if( LoadRecNo == -1 ) Fill_null_data( DestPtr, GetLen );
		else
		{
			LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
			if( LoadAddr != -1 )
			{
//				if(( SelectType == 0 )&&( *FreezeAux == D_Unsigned32 )&&( IsAllData( FreezeAux+1, 4, 0xFF ) == 0))	//以时标选择，冻结序号尚空白
//				{
//					Len = 7;
//					if( FileCurvePara->BakFlg == 1 ) Len += 1;
//					Read_File( DestPtr, FileCurvePara, LoadAddr+Len, 4 );
//					RAM_Write( FreezeAux+1, DestPtr, 4 );	//取该时间点的冻结序号
//				}
//				else if(( SelectType == 1 )&&( *FreezeAux == D_date_time_s )&&( IsAllData( FreezeAux+1, 7, 0xFF ) == 0))	//以序号选择，冻结时间尚空白
//				{
//					Read_File( DestPtr, FileCurvePara, LoadAddr, 7 );
//					RAM_Write( FreezeAux+1, DestPtr, 7 );	//取该时间点的冻结时间
//				}	
				
				if( IsAllData( FreezeAux, 11, 0xFF ) == 0 )	//时标、冻结序号尚空白
				{
					if( FileCurvePara->BakFlag == 1 )
					{
						Read_File( DestPtr, FileCurvePara, LoadAddr, 7 );
						Read_File( DestPtr+7, FileCurvePara, LoadAddr+8, 4 );
						RAM_Write( FreezeAux, DestPtr, 11 );	//取该时间点的冻结时间、冻结序号				
					}
					else   
					{
						Read_File( DestPtr, FileCurvePara, LoadAddr, 11 );
						RAM_Write( FreezeAux, DestPtr, 11 );	//取该时间点的冻结时间、冻结序号				
					}
				}
				
				LoadAddr += 11;								//数据起始地址	
				Len = FileCurvePara->RecdLenth - 11;		//有效数据长度（带校验的包含3个校验字节）
				if( FileCurvePara->BakFlag == 1 )			//带校验
				{
					LoadAddr += 2;							//加冻结时间和冻结序号的校验和
					Len -= 3;								//减去三项数据的校验和
				}	
				if( Read_File( DestPtr, FileCurvePara, LoadAddr, Len ) == 0 )
				{
					*GetLen += Len;					
				}
				else Fill_null_data( DestPtr, GetLen );
			}
			else Fill_null_data( DestPtr, GetLen );
		}			
	}	
	else Fill_null_data( DestPtr, GetLen );
	ErrorByte = f_close(&fsrc);
	return;								
}
*/

void Get_single_freeze_content( unsigned char* DestPtr, unsigned short* GetLen, unsigned char* SelectorData, unsigned char* FreezeAux, unsigned short ItemNo, unsigned short CurveNo, short SelectType )
{
	unsigned char Buff2[20];
//	unsigned char* Point;
	unsigned char* FileName;
    unsigned long LongBuff[8];
//    unsigned long WriteLen;
	FILECURVEPARA* FileCurvePara;
	unsigned char ErrorByte=0;
	long LoadAddr;
	long LoadRecNo;
	long Offset;							//16.10.12
        short Len;
	
//	Point = Buff;
	FileName = Buff2;
	FileCurvePara = (FILECURVEPARA*)LongBuff;
	
	FileCurvePara->ChannelNo = ItemNo;
	FileCurvePara->ChannelNo <<= 8;
	FileCurvePara->ChannelNo += CurveNo;
	GetFileCurvePara( FileCurvePara );

	if( FileCurvePara->AvailRecs == 0 ) 
	{
		Fill_null_data( DestPtr, GetLen );
		return;							//没有有效记录,返回空数据。	
	}
	
	Offset = 0;							//16.10.12
	CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );
//	ErrorByte = f_open( &fsrc, (char const*)FileName, FA_READ);						//此处打开文件后，在Read_File中不再每次打开，避免多次打开，除非读取备用文件后恢复打开
//	ErrorByte = Open_File( FileCurvePara, FileName );						//此处打开文件后，在Read_File中不再每次打开，避免多次打开，除非读取备用文件后恢复打开
	ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
	if( ErrorByte == FR_OK )
	{
		LoadRecNo = GetFreezeLoadPWAds( DestPtr, FileCurvePara, SelectorData, SelectType );
		if( LoadRecNo == -1 ) Fill_null_data( DestPtr, GetLen );
		else
		{
			LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
			if( LoadAddr != -1 )
			{			
				if( IsAllData( FreezeAux, 11, 0xFF ) == 0 )	//时标、冻结序号尚空白
				{
					if( FileCurvePara->BakFlag == 1 )
					{
						Read_File( DestPtr, FileCurvePara, LoadAddr, Offset, 7 );
						Offset += 8;
						Read_File( DestPtr+7, FileCurvePara, LoadAddr, Offset, 4 );
						RAM_Write( FreezeAux, DestPtr, 11 );	//取该时间点的冻结时间、冻结序号				
					}
					else   
					{
						Read_File( DestPtr, FileCurvePara, LoadAddr, Offset, 11 );
						RAM_Write( FreezeAux, DestPtr, 11 );	//取该时间点的冻结时间、冻结序号				
					}
				}
				
//				LoadAddr += 11;								//数据起始地址	
//				Offset += 11;								//数据起始地址	
				Offset = 11;								//数据起始地址	//16.10.13
				Len = FileCurvePara->RecdLenth - 11;		//有效数据长度（带校验的包含3个校验字节）
				if( FileCurvePara->BakFlag == 1 )			//带校验
				{
//					LoadAddr += 2;							//加冻结时间和冻结序号的校验和
					Offset += 2;							//加冻结时间和冻结序号的校验和
					Len -= 3;								//减去三项数据的校验和
				}	
				if( Read_File( DestPtr, FileCurvePara, LoadAddr, Offset, Len ) == 0 )
				{
					*GetLen += Len;					
				}
				else Fill_null_data( DestPtr, GetLen );
			}
			else Fill_null_data( DestPtr, GetLen );
		}			
	}	
	else Fill_null_data( DestPtr, GetLen );
//	ErrorByte = f_close(&fsrc);
	return;								
}


//Type = 0:查找冻结时间  1：查找冻结序号 	
long GetFreezeLoadPWAds( unsigned char* Dest, FILECURVEPARA* FileCurvePara, unsigned char* Data, short Type )
{
	long nStart = 0;
	long nMiddle;
	long nFound=-1;
	short Temp;
	long nEnd;
	
	nEnd = FileCurvePara->AvailRecs-1;
	
	while(nStart<=nEnd)
    {
        nMiddle=(nStart+nEnd)/2;
		Temp = FreezeLoadPWJustDay( nMiddle, FileCurvePara, Data, Type );
		switch( Temp )
		{
			case 1:  nEnd = nMiddle - 1;
					 break;
			case -1: nStart = nMiddle + 1;
					 break;
			case 0:	 
				 	 nFound = nMiddle;
//				 	 nStart = nMiddle+1;					
					 return nFound;	
					 break;
			default: break;			
		}	
	}	
	return nFound;	
}		

/*
short FreezeLoadPWJustDay( long LoadRecNo, FILECURVEPARA* FileCurvePara, unsigned char* Data, short Type )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	long LoadAddr; 
	unsigned char Len;
	
	Ptr = Buff;
	
	if( LoadRecNo < 0 ) return 1;					//非法

	LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
	if( LoadAddr != -1 )
	{
		if( Type == 0 )	Len = 7;					//冻结时间
		else 										//冻结序号
		{
			LoadAddr += 7; 	
			Len = 4;
			if( FileCurvePara->BakFlag == 1 ) LoadAddr += 1;		//加1字节冻结时间的校验和
		}	
		Read_File( Ptr, FileCurvePara, (unsigned long)LoadAddr, Len );         
	}
	else return 1;
	if( Data_Comp( Data, Ptr, Len ) == -1 ) return -1;
	if( Data_Comp( Data, Ptr, Len ) == 1 ) return 1;

	return 0; 
}		
*/

short FreezeLoadPWJustDay( long LoadRecNo, FILECURVEPARA* FileCurvePara, unsigned char* Data, short Type )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	long LoadAddr; 
	long Offset; 
	unsigned char Len;
	
	Ptr = Buff;
	
	if( LoadRecNo < 0 ) return 1;					//非法

	Offset = 0;
	LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
	if( LoadAddr != -1 )
	{
		if( Type == 0 )	Len = 7;					//冻结时间
		else 										//冻结序号
		{
//			LoadAddr += 7; 	
			Offset += 7; 
			Len = 4;
//			if( FileCurvePara->BakFlag == 1 ) LoadAddr += 1;		//加1字节冻结时间的校验和
			if( FileCurvePara->BakFlag == 1 ) Offset += 1;			//加1字节冻结时间的校验和
		}	
//		Read_File( Ptr, FileCurvePara, (unsigned long)LoadAddr, Len );         
		Read_File( Ptr, FileCurvePara, (unsigned long)LoadAddr, Offset, Len );         
	}
	else return 1;
	if( Data_Comp( Data, Ptr, Len ) == -1 ) return -1;
	if( Data_Comp( Data, Ptr, Len ) == 1 ) return 1;

	return 0; 
}		

//================================================================================================================
void Get_freeze_First_content( unsigned char* DestPtr, unsigned char* SData, unsigned char* OAD, unsigned char OADItems, unsigned short ItemNo, short SelectType )
{
	unsigned char Buff[10];
	unsigned char Buff2[10];
	unsigned short Addr;
	unsigned char* MinData; 
	unsigned char* Ptr; 
	unsigned char* EData; 
    short Items;
	short Len;
	short i,j;
	
	MinData = Buff;
	Ptr = Buff2;
	
	if( SelectType == 0 ) Len = 7;
	else Len = 4;
	EData = SData + Len;
	
	RAM_DataFill( MinData, 10, 0xFF);
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	
	if( OADItems == 0 )
	{
		for( i=0;i<Items;i++ )
		{
			if( Get_single_freeze_First_content( DestPtr, SData, EData, ItemNo, i, SelectType ) == 0 ) 
			{
				if( Data_Comp( MinData, DestPtr, Len) > 0 )
				{
					RAM_Write( MinData, DestPtr, Len );
				}		
			}	
		}
	}
	else
	{	
		for(j=0;j<OADItems;j++)
		{
			if(( JudgeSelectorTimeOAD( OAD, 0 ) != 0 )&&( JudgeSelectorNo_OAD( OAD, 0 ) != 0 ))		//冻结时间和冻结序号项除外
			{
				for( i=0;i<Items;i++ )
				{
					E2P_RData( Ptr, Addr+i*10, 9 );
					if( Data_Comp( Ptr+2, OAD, 4 ) == 0 )  
					{
						if( Get_single_freeze_First_content( DestPtr, SData, EData, ItemNo, i, SelectType ) == 0 ) 
						{
							if( Data_Comp( MinData, DestPtr, Len) > 0 )
							{
								RAM_Write( MinData, DestPtr, Len );
							}		
						}	
						break;
					}	
				}
			}	
			OAD += 5;
		}
	}		
	RAM_Write( DestPtr, MinData, Len );
				
}

short Get_single_freeze_First_content( unsigned char* DestPtr, unsigned char* SData, unsigned char* EData, unsigned short ItemNo, unsigned short CurveNo, short SelectType )
{
	unsigned char Buff2[20];
	unsigned char* FileName;
    unsigned long LongBuff[8];
	FILECURVEPARA* FileCurvePara;
	unsigned char ErrorByte=0;
	long LoadAddr;
	long LoadRecNo;
	long Offset;							//16.10.12
    short Len;
	
	FileName = Buff2;
	FileCurvePara = (FILECURVEPARA*)LongBuff;
	
	FileCurvePara->ChannelNo = ItemNo;
	FileCurvePara->ChannelNo <<= 8;
	FileCurvePara->ChannelNo += CurveNo;
	GetFileCurvePara( FileCurvePara );

	if( FileCurvePara->AvailRecs == 0 ) 
	{
		return -1;							//没有有效记录,返回空数据。	
	}
	
	Offset = 0;							//16.10.12
	CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );
//	ErrorByte = Open_File( FileCurvePara, FileName );						//此处打开文件后，在Read_File中不再每次打开，避免多次打开，除非读取备用文件后恢复打开
	ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
	if( ErrorByte == FR_OK )
	{
		LoadRecNo = GetFirstFreezeLoadPWAds( DestPtr, FileCurvePara, SData, EData, SelectType );
		if( LoadRecNo == -1 ) return -1;
		else
		{
			LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
			if( LoadAddr != -1 )
			{			
				Offset = 0;
				if( SelectType == 0 )	Len = 7;			//冻结时间
				else 										//冻结序号
				{
					Offset += 7; 
					Len = 4;
					if( FileCurvePara->BakFlag == 1 ) Offset += 1;			//加1字节冻结时间的校验和
				}	
				if( Read_File( DestPtr, FileCurvePara, (unsigned long)LoadAddr, Offset, Len ) == 0 )
				{
					return 0;						
				}
				else return -1;	         
			}
			else return -1;	         
		}
	}
	else return -1;	         		
}

//根据范围取该区间内的第一条记录
//Type = 0:查找冻结时间  1：查找冻结序号 	
//SData:起始数据	//包含
//EData:结束数据	//不包含？
long GetFirstFreezeLoadPWAds( unsigned char* Dest, FILECURVEPARA* FileCurvePara, unsigned char* SData, unsigned char* EData, short Type )
{
	long nStart = 0;
	long nMiddle;
	long nFound=-1;
	short Temp;
	long nEnd;
	
	nEnd = FileCurvePara->AvailRecs-1;
	
	while(nStart<=nEnd)
    {
        nMiddle=(nStart+nEnd)/2;
		Temp = FirstFreezeLoadPWJustDay( nMiddle, FileCurvePara, SData, EData, Type );
		switch( Temp )
		{
			case 1:  nEnd = nMiddle - 1;
					 break;
			case -1: nStart = nMiddle + 1;
					 break;
			case 0:	 
				 	 nFound = nMiddle;
				 	 nStart = nMiddle+1;					
					 break;
			default: break;			
		}	
	}	
	return nFound;	
}		

short FirstFreezeLoadPWJustDay( long LoadRecNo, FILECURVEPARA* FileCurvePara, unsigned char* SData, unsigned char* EData, short Type )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	long LoadAddr; 
	long Offset; 
	unsigned char Len;
	
	Ptr = Buff;
	
	if( LoadRecNo < 0 ) return 1;					//非法

	Offset = 0;
	LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
	if( LoadAddr != -1 )
	{
		if( Type == 0 )	Len = 7;					//冻结时间
		else 										//冻结序号
		{
//			LoadAddr += 7; 	
			Offset += 7; 
			Len = 4;
//			if( FileCurvePara->BakFlag == 1 ) LoadAddr += 1;		//加1字节冻结时间的校验和
			if( FileCurvePara->BakFlag == 1 ) Offset += 1;			//加1字节冻结时间的校验和
		}	
//		Read_File( Ptr, FileCurvePara, (unsigned long)LoadAddr, Len );         
		Read_File( Ptr, FileCurvePara, (unsigned long)LoadAddr, Offset, Len );         
	}
	else return 1;
	if( Data_Comp( EData, Ptr, Len ) == -1 ) return -1;
	if( Data_Comp( SData, Ptr, Len ) == 1 ) return 1;

	return 0; 
}		

/*
unsigned char Read_File( unsigned char* Dest, FILECURVEPARA* FileCurvePara, long Addr, short Len )
{
	unsigned char Buff[20];
	unsigned char* FileName;
    unsigned long ReadLen;
	unsigned char ErrorByte=0;
 
	FileName = Buff;
	
	CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );
	if( FileCurvePara->BakFlag == 1 ) Len += 1;												//带备份数据都带校验和
	ErrorByte = f_lseek (&fsrc, Addr);
	if( ErrorByte == FR_OK )
	{
   		ErrorByte = f_read(&fsrc, Dest, Len, (UINT*)&ReadLen);     
   	}	      	
    if( FileCurvePara->BakFlag == 1 )
	{
		if( *(Dest+Len-1) != ChkNum( Dest, Len ) )
		{
			ErrorByte = f_close(&fsrc);      		
			CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 1 );	//打开备份文件
			ErrorByte = f_open( &fsrc, (char const*)FileName, FA_READ);
	    	if( ErrorByte == FR_OK )
	    	{ 
				ErrorByte = f_lseek (&fsrc, Addr);
				if( ErrorByte == FR_OK )
				{
	    			ErrorByte = f_read(&fsrc, Dest, Len, (UINT*)&ReadLen);     
	    		}	      	
				ErrorByte = f_close(&fsrc);      		
	    	}
			CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );	//恢复打开原始文件
			ErrorByte = f_open( &fsrc, (char const*)FileName, FA_READ);
	    }		
	}
        return ErrorByte;
}
*/

//备份文件中的文件起始扇区参数不一样，操作完备份文件后要重置当前文件参数			//16.10.12
unsigned char Read_File( unsigned char* Dest, FILECURVEPARA* FileCurvePara, long CurRecNo, long Offset, short Len )
{
	unsigned char Buff[20];
	unsigned char* FileName;
//    unsigned long ReadLen;
	unsigned char ErrorByte=0;
 	long Addr;
 
	FileName = Buff;
	
	CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );
	if( FileCurvePara->BakFlag == 1 ) Len += 1;												//带备份数据都带校验和
	Addr = GetFileRecRealFlashAddr( FileCurvePara, CurRecNo );
	Addr += Offset;
	Read_Flash( Dest, Addr, Len );      	
    if( FileCurvePara->BakFlag == 1 )
	{
//		if( *(Dest+Len-1) != ChkNum( Dest, Len ) )
		if( *(Dest+Len-1) != ChkNum( Dest, Len-1 ) )		//16.10.13
		{
			CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 1 );	//打开备份文件
//			ErrorByte = Open_File( FileCurvePara, FileName );
			ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
	    	if( ErrorByte == FR_OK )
	    	{ 
				Addr = GetFileRecRealFlashAddr( FileCurvePara, CurRecNo );
				Addr += Offset;
				Read_Flash( Dest, Addr, Len );      	
	    	}
			CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );	//恢复打开原始文件
//			ErrorByte = Open_File( FileCurvePara, FileName );								//需恢复原文件参数		
			ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
	    }		
	}
    return ErrorByte;
}


//取Class9类数据
short GetFreezeClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short Len;

	RAM_Write( Dest, Source, 4 );

	Len = GetFreezeClassDataTab( COM_No, Dest+4, Source );
	if( Len == -1 )  
	{
		*(Dest+4) = Get_data_access_result;						//异常标志
		*(Dest+5) = object_undefined; 						//无对应的OBIS码
		Len = 2;
	}
	else if( Len == -2 )										//17.04.09
	{
		*(Dest+4) = Get_data_access_result;						//异常标志
		*(Dest+5) = read_write_denied; 							//操作权限不足
		Len = 2;
	}	
	Len += 4;
	return Len;		
}		

unsigned short GetFreezeClassTabItemNo( unsigned char* OAD )
{
	unsigned short i;
		
	for( i=0;i<DL698_Freeze_ProfileSum;i++ )
	{
		if( Data_Comp( OAD, (unsigned char*)DL698_Freeze_ProfileTab[i].OAD, 2 ) == 0 ) break;				//查找对应的OBIS码
	}	
	return i;	
}

//取Freeze类数据
short GetFreezeClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ) 
{
	short i;
//	unsigned char* Addr;
	short Len;
//	unsigned short OICode;
//	short Temp;
//	short Start,End;
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
//	unsigned char* Ptr;
	unsigned char* OAD;
	Len = -1;
	
	OAD = Source;	
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
					
	i = GetFreezeClassTabItemNo( OAD );
	if( i != DL698_Freeze_ProfileSum )
	{
		if(( Attribute == 1 )||( Attribute == 3 ))			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_Freeze_ProfileTab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09
		}
		*Dest = Get_data;								//正常数据返回	
		switch( Attribute )
		{
			case 1:										//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x02;
				RAM_Write( Dest+3, OAD, 2 );
				Len = 5;
				break;	
			case 2:										//	
				break;	
			case 3:										//关联对象属性表
				Len = GetFreeze_capture_objectsData( COM_No, Dest, i );
				break;	
			case 4:										//配置参数	
				break;	
			default:
				break;	
		}	
	}		
	return Len;
}	

short GetFreezeCaptureItems( short ProfItemNo )
{
	unsigned short Addr;
	unsigned short Temp;
    unsigned short CaptureItems;
	
	Addr = DL698_Freeze_ProfileTab[ProfItemNo].CapNumAddr;
	CaptureItems = 0;
	E2P_RData( (unsigned char*)&CaptureItems, Addr, 1 );
	Temp = DL698_Freeze_ProfileTab[ProfItemNo].CapNumMax;
	if( CaptureItems > Temp ) CaptureItems = Temp;					//不超出最大物理存储项数
	
	return CaptureItems;
}	

short Get_sigle_freeze_capture_objectsPara( unsigned char* Dest, unsigned short Addr, unsigned short CaptureItems, unsigned short* Length )
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	unsigned short i;
	unsigned char* Point;
	
	Point = Dest;
	Ptr = Buff;
//	*Point = Data_structure;
//	*(Point+1) = 0x03;
//	*Length += 2;
//	Point += 2;
	for( i=0;i<CaptureItems;i++ )
	{
		*Point = Data_structure;
		*(Point+1) = 0x03;
		E2P_RData( Ptr, Addr, 9 );
		*(Point+2) = D_Unsigned16;
		*(Point+3) = *(Ptr+1);									//高位在前
		*(Point+4) = *Ptr;
		*(Point+5) = D_OAD;
		RAM_Write( Point+6, Ptr+2, 4 );
		*(Point+10) = D_Unsigned16;
		*(Point+11) = *(Ptr+7); 
		*(Point+12) = *(Ptr+6); 
		Point += 13;					
		*Length += 13;
		Addr += 10;
	}		
    return 0;
}

short GetFreeze_capture_objectsData( short COM_No, unsigned char* Dest, short ProfItemNo )
{
	unsigned short Addr;
	unsigned short CaptureItems;
	unsigned short Length;
	
	CaptureItems = GetFreezeCaptureItems( ProfItemNo );

	*(Dest+1) = Data_array;
	*(Dest+2) = CaptureItems;											
	Length = 3;
	Addr = DL698_Freeze_ProfileTab[ProfItemNo].CapObjAddr;
	Get_sigle_freeze_capture_objectsPara( Dest+3, Addr, CaptureItems, &Length );	
	return Length;	
}	

//-------------------------------------------------------------------------------------------------------
unsigned char GetBakFlag( short ProfileNo, unsigned char* OAD )
{
	unsigned char Buff[4];
	unsigned char BakFlag=0;
	
	RAM_Write( Buff, OAD, 4 );
//	if(( ProfileNo == 0x05 )||( ProfileNo == 0x06 )) BakFlag = 1;		//结算日冻结和月冻结需要全体备份
	if( ProfileNo == 0x05 ) BakFlag = 1;								//结算日冻结需要全体备份
	else if( ProfileNo == 0x02 )										//分钟冻结		
	{
		Buff[2] &= 0x1F;												//屏蔽掉属性特征
		if(( Buff[0] == 0x00 )&&( Buff[2] == 0x04 )&&( Buff[3] == 0x01 ))	//总电量的属性4(精确电量)	
		{
			switch( Buff[1] )
			{
				case 0x10:												//有功正向	
				case 0x20:												//有功反向	
				case 0x50:												//1象限无功
				case 0x60:												//2象限无功
				case 0x70:												//3象限无功
				case 0x80:												//4象限无功
					BakFlag = 1;										//针对江苏15分钟精确电量要求带备份	
					break;
				default:
					break;					
			}	
		}	
	}	
	
	return BakFlag;
}	

/*
unsigned char Judge_freeze_capture_objects( unsigned char* Data, short ProfileNo, unsigned short* Length, unsigned short* ItemLen )
{
	unsigned short CapItems;
	short i;
	unsigned char* Ptr;
	unsigned short Temp;
			
	CapItems = DL698_Freeze_ProfileTab[ProfileNo].CapNumMax;
					
	if( *Data != Data_array ) return other_reason;
	Temp = *(Data+1);	
	if( Temp > CapItems ) return other_reason;			//捕获对象数大于额定限数
	
	Ptr = Data+2;										
	
	*Length = 0;
	*ItemLen = 0;
	for( i=0;i<Temp;i++ )
	{
        if( *Ptr != Data_structure ) return other_reason;
        if( *(Ptr+1) != 0x03 ) return other_reason;
        if( *(Ptr+2) != D_Unsigned16 ) return other_reason;
        if( *(Ptr+5) != D_OAD ) return other_reason;
        if( *(Ptr+10) != D_Unsigned16 ) return other_reason;
        
		if( Judge_capture_objects_content( Ptr+6, Length, GetBakFlag( ProfileNo, Ptr+6 ) ) != 0 ) return other_reason; 
		Ptr += 13;		
		*ItemLen += 13;		
	}	
    return 0;
}
*/

//获取原有曲线进行删除后，将释放多少目录、扇区
//ItemNo:冻结曲线号
//返回:预删除原曲线多个文件后，共空余多少文件目录区
//*DeleteSectors：预删除原曲线多个文件后，可空余出来的扇区数
unsigned short GetPreDeleteDirSectors( unsigned short ItemNo, unsigned short* DeleteSectors )
{
	unsigned short i,j;
	unsigned long Fls_Src;
	unsigned short IdleDir;
	unsigned short DelDirs;
	unsigned short DelSectors;
	unsigned char* Ptr;
	unsigned char Buff[14]; 
	unsigned char* Point;
	unsigned short Items;
	unsigned short Addr;
	unsigned char Buff2[20];
	unsigned char BakFlag;
	unsigned char* FileName;
	unsigned short Temp;
	unsigned short RemainDir; 
	
	Fls_Src = FLASH_SECTOR_SIZE;
	Read_Flash( FBuff.Buff, Fls_Src, 4096 );

	IdleDir = 0;
	Ptr = FBuff.Buff;
	for( i=0;i<FAT_DIRS;i++ )
	{
		if( IsAllData( Ptr, FAT_DIR_LEN, 0xff ) == 0 )
		{
			IdleDir++;				
		}		
		Ptr += FAT_DIR_LEN;
	}	

	Point = Buff;
	FileName = Buff2;
	DelDirs = 0;
	DelSectors = 0;
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	for( i=0;i<Items;i++ )
	{
		E2P_RData( Point, Addr+i*10, 9 );
		BakFlag = GetBakFlag( ItemNo, Point+2 );
		CreatFileName( FileName, ItemNo, Point+2, 0 );
		Ptr = FBuff.Buff;
		for( j=0;j<FAT_DIRS;j++ )
		{
			if( Data_Comp( Ptr, FileName, 5 ) == 0 )
			{
				if( *(Ptr+9) != ChkNum( Ptr, 9 ) )
				{
					Read_Flash( Ptr, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE+Ptr-FBuff.Buff, FAT_DIR_LEN );
					if( *(Ptr+9) != ChkNum( Ptr, 9 ) ) 
					{
						Ptr += FAT_DIR_LEN;
						continue;	
					}	
				}	
				DelDirs++;	
				RAM_Write( (unsigned char*)&Temp, Ptr+7, 2 );
				DelSectors += Temp;											//该文件所包含的扇区数
				break;			
			}		
			Ptr += FAT_DIR_LEN;
		}	
		if( BakFlag != 0 )													//删除备份文件
		{	
			CreatFileName( FileName, ItemNo, Point+2, BakFlag );
			Ptr = FBuff.Buff;
			for( j=0;j<FAT_DIRS;j++ )
			{
				if( Data_Comp( Ptr, FileName, 5 ) == 0 )
				{
					if( *(Ptr+9) != ChkNum( Ptr, 9 ) )
					{
						Read_Flash( Ptr, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE+Ptr-FBuff.Buff, FAT_DIR_LEN );
						if( *(Ptr+9) != ChkNum( Ptr, 9 ) ) 
						{
							Ptr += FAT_DIR_LEN;
							continue;	
						}	
					}	
					DelDirs++;	
					RAM_Write( (unsigned char*)&Temp, Ptr+7, 2 );
					DelSectors += Temp;											//该文件所包含的扇区数
					break;			
				}		
				Ptr += FAT_DIR_LEN;
			}	
		}
	}	

	RemainDir = IdleDir + DelDirs;											//可用目录数为空余数加删除数
	*DeleteSectors = DelSectors;											//删除扇区数
	
	return RemainDir;
}

unsigned short GetFreezeRecordPags( unsigned char* Source, unsigned short RecdLenth )
{
	unsigned short RecdCunt;
	unsigned short RecsInPage;		//每页记录数
	unsigned short Pages;			//存储空间所占用页数

	RecdCunt = SwapCharToShort( Source );			//存储深度，即总需存储记录数
	RecsInPage = 4096/RecdLenth;					//每页存放记录数
	Pages = RecdCunt / RecsInPage;					
	if(( RecdCunt % RecsInPage ) != 0 ) Pages += 1; //有效数据占用页数
	Pages += 1;										//多保留一页轮转用

	return Pages;	
}	

//Type:返回的剩余扇区数是否包括即将删除的 0：包括；1：不包括 
//unsigned char Judge_freeze_capture_objects( unsigned char* Data, short ProfileNo, unsigned short* Length, unsigned short* ItemLen )
unsigned char Judge_freeze_capture_objects( unsigned char* Data, short ProfileNo, unsigned short* Length, unsigned short* ItemLen, short Type )
{
	unsigned short CapItems;
	short i;
	unsigned char* Ptr;
	unsigned short Temp;
	unsigned short TotalDir;
	unsigned short TotalSectors;
	unsigned short Remain;
	unsigned short RemainDir;
	unsigned short DeleteSectors;
	unsigned short RecdLenth;		//记录长度
	unsigned short Pages;			//存储空间所占用页数
	unsigned char BakFlag;
	
	CapItems = DL698_Freeze_ProfileTab[ProfileNo].CapNumMax;
					
	if( *Data != Data_array ) return other_reason;
	Temp = *(Data+1);	
	if( Temp > CapItems ) return other_reason;			//捕获对象数大于额定限数
	
	
	TotalDir = 0;										//需分配的目录数
	TotalSectors = 0;									//需分配的扇区数
	
	Ptr = Data+2;										
	*Length = 0;
	*ItemLen = 0;
	for( i=0;i<Temp;i++ )
	{
        if( *Ptr != Data_structure ) return other_reason;
        if( *(Ptr+1) != 0x03 ) return other_reason;
        if( *(Ptr+2) != D_Unsigned16 ) return other_reason;
        if( *(Ptr+5) != D_OAD ) return other_reason;
        if( *(Ptr+10) != D_Unsigned16 ) return other_reason;
        
        BakFlag = GetBakFlag( ProfileNo, Ptr+6 );
		RecdLenth = *Length;
		if( Judge_capture_objects_content( Ptr+6, Length, BakFlag ) != 0 ) return other_reason; 
		RecdLenth = *Length - RecdLenth;				//单条记录长度	
		RecdLenth += GetFreezeHeadLen( ProfileNo, BakFlag );
		Pages = GetFreezeRecordPags( Ptr+11, RecdLenth );	//每条冻结记录占用页数
		if( BakFlag != 0 )								//带备份数据空间翻倍
		{
			TotalDir += 1;
			Pages *= 2;
		}	
		TotalDir += 1;
		TotalSectors += Pages;
		
		Ptr += 13;		
		*ItemLen += 13;		
	}	
	
	if( Type == 0 )
	{
		RemainDir = GetPreDeleteDirSectors( ProfileNo, &DeleteSectors );		//总剩余目录数，包括即将删除的
	}
	else
	{
		RemainDir = GetRemainDirs( &DeleteSectors );	
		DeleteSectors = 0;
	}		
	DeleteSectors += GetRemainSectors( &Remain );							//总剩余扇区数
	
	if( TotalDir > RemainDir ) return other_reason;							//剩余目录数不足
	if( TotalSectors > DeleteSectors ) return other_reason;					//剩余扇区数不足
	
    return 0;
}



unsigned short GetFreezeHeadLen( unsigned short ItemNo, unsigned char BakFlag )
{
	unsigned short Length;	
	
	Length = 4+7;						//记录序号、发生时间(年月日时分秒7字节)
	if( BakFlag != 0 ) Length += 2; 	//带校验

	return Length;
}	
/*
//文件名命名规则：
//前三字符为“0：/”,后缀为“.TXT”
//正式文件名的第一字符为冻结类型，小于15的直接转换成ASCII数字，大于15的先减去15后再加上字符“G”
//OAD[0]、OAD[1]、OAD[2]直接转换成6个字符
//OAD[3]一般小于15，当BakFlag等于0时，直接转换成一个字符
//当BakFlag等于1时，把OAD[3]加上字符“G”作为1个字符
void CreatFileName( unsigned char* Dest, unsigned short ItemNo, unsigned char* OAD, unsigned char BakFlag )
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf的字符查找表
	unsigned char Buff[7];
	unsigned char* Ptr;
		
	Ptr = Buff;
	*Ptr= ItemNo & 0x00FF;
//	RAM_Write( Ptr+1, OAD, 4 );
//	HEX_ASC( Dest, Ptr, 5);
//	if( BakFlag == 0 ) *(Dest+10) = '0';
//	else *(Dest+10) = '1';
//	*(Dest+11) = 0;
	
	*Dest = '0';										//盘符
	*(Dest+1) = ':';
	*(Dest+2) = '/';
	if( *Ptr <= 15 ) *(Dest+3) = tab[*Ptr];				//冻结类别
	else
	{
		*Ptr -= 16;										//为以后扩展预留		
		*(Dest+3) = 'G' + *Ptr;	
		if( *(Dest+3) > 'Z') *(Dest+3) = 'Z';			//避免非法字符
	}
	RAM_Write( Ptr, OAD, 4 );
	HEX_ASC( Dest+4, Ptr, 3);							//前3个OAD直接转换
	if( *(Ptr+3) > 15 ) *(Ptr+3) = 15;					//索引号大于15，强制置为15，填‘F’
	if( BakFlag == 0 ) *(Dest+10) = tab[*(Ptr+3)];		//无校验
	else 
	{
		*(Dest+10) = 'G' + *(Ptr+3);					//带校验	
		if( *(Dest+10) > 'Z') *(Dest+10) = 'Z';			//避免非法字符
	}
	*(Dest+11) = '.';
	*(Dest+12) = 'T';
	*(Dest+13) = 'X';
	*(Dest+14) = 'T';
	*(Dest+15) = 0;

	return;
}	
*/

//文件名命名规则：
//第一字节为冻结曲线类别，最大0x7F
//后四字节为OAD
//当BakFlag等于1时，把第一字节的最高位置1
void CreatFileName( unsigned char* Dest, unsigned short ItemNo, unsigned char* OAD, unsigned char BakFlag )
{
	*Dest= ItemNo & 0x007F;
	RAM_Write( Dest+1, OAD, 4 );
	if( BakFlag != 0 ) *Dest |= 0x80;		//备份文件

	return;
}	

/*
void DeleteFreezeFile( unsigned short ItemNo, unsigned char* OAD )
{
	unsigned char Buff[20];
	unsigned char BakFlag;
	unsigned char* FileName;
        unsigned char ErrorByte;
        
	FileName = Buff;
	
	BakFlag = GetBakFlag( ItemNo, OAD );
	CreatFileName( FileName, ItemNo, OAD, 0 );
	ErrorByte = f_open( &fsrc, (char const*)FileName, FA_READ);					//以只读方式，试探文件是否存在？
    if( ErrorByte == FR_OK )
    { 
		f_unlink( (char const*)FileName );
	}
	if( BakFlag != 0 )											//删除备份文件
	{	
		CreatFileName( FileName, ItemNo, OAD, 1 );
		ErrorByte = f_open( &fsrc, (char const*)FileName, FA_READ);				//以只读方式，试探文件是否存在？
    	if( ErrorByte == FR_OK )
    	{ 
			f_unlink( (char const*)FileName );
		}
	}	
}	
*/

//16.10.12
void DeleteFreezeFile( unsigned short ItemNo, unsigned short CurveNo, unsigned char* OAD )
{
	unsigned char Buff[20];
	unsigned char BakFlag;
	unsigned char* FileName;
    unsigned long LongBuff[8];
	FILECURVEPARA* FileCurvePara;
	unsigned char ErrorByte=0;
	
	FileName = Buff;
	FileCurvePara = (FILECURVEPARA*)LongBuff;
	
	FileCurvePara->ChannelNo = ItemNo;
	FileCurvePara->ChannelNo <<= 8;
	FileCurvePara->ChannelNo += CurveNo;
	GetFileCurvePara( FileCurvePara );
        
	BakFlag = GetBakFlag( ItemNo, OAD );
	CreatFileName( FileName, ItemNo, OAD, 0 );
	ErrorByte = Open_File( FileCurvePara, FileName );					//试探文件是否存在？
    if( ErrorByte == FR_OK )
    { 
		DeletFile( FileCurvePara, FileName );
	}
	if( BakFlag != 0 )													//删除备份文件
	{	
		CreatFileName( FileName, ItemNo, OAD, 1 );
		ErrorByte = Open_File( FileCurvePara, FileName );				//试探文件是否存在？
    	if( ErrorByte == FR_OK )
    	{ 
			DeletFile( FileCurvePara, FileName );
		}
	}	
}	


//对原有曲线进行删除
void DeleteFreezeProfileFile( unsigned short ItemNo )
{
	unsigned char Buff[14]; 
	unsigned char* Ptr;
	unsigned short Items;
	unsigned short Addr;
	short i;
	
	Ptr = Buff;
	
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	for( i=0;i<Items;i++ )
	{
		E2P_RData( Ptr, Addr+i*10, 9 );
//		DeleteFreezeFile( ItemNo, Ptr+2 );
		DeleteFreezeFile( ItemNo, i, Ptr+2 );		//16.10.12
	}	
	*Ptr = 0;
	E2P_WData( DL698_Freeze_ProfileTab[ItemNo].CapNumAddr, Ptr, 1 );								//捕获项数清零
}	

unsigned char DeleteFreezeCapture_OAD( unsigned short ItemNo, unsigned char* OAD )
{
//	unsigned char BakFlag=0;
	unsigned char Buff[14]; 
	unsigned char* Ptr;
	unsigned short Items;
//	unsigned short Addr;
	short i,j;
	unsigned short PtAddr;			//当前记录首地址指针(该记录所占用的FLASH空间内的相对地址)
	unsigned short RecdLenAddr;		//每条记录长度首地址(逐条对应于捕获内容得到)
	unsigned short CapObjAddr;		//捕获项地址
	unsigned short EntriesUseAddr;	//有效条目数首地址，该地址列对应数最大为允许读取的总记录条数，超过后不再增加。记录清零时只需将其清零即可
	
	Items = 0;
	Ptr = Buff;

//	BakFlag = GetBakFlag( ItemNo, OAD );
	Items = GetFreezeCaptureItems( ItemNo );
	if( Items == 0 ) return object_class_inconsistent;
	CapObjAddr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	for( i=0;i<Items;i++ )
	{
		E2P_RData( Ptr, CapObjAddr+i*10, 9 );
		if( Data_Comp( Ptr+2, OAD, 4 ) == 0 )
		{
//			DeleteFreezeFile( ItemNo, Ptr+2 );
			DeleteFreezeFile( ItemNo, i, Ptr+2 );		//16.10.12
			break;
		}		
	}		
	if( i == Items ) return object_class_inconsistent;
	else
	{
		CapObjAddr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
		PtAddr = DL698_Freeze_ProfileTab[ItemNo].PtAddr;
		RecdLenAddr = DL698_Freeze_ProfileTab[ItemNo].RecdLenAddr;
		EntriesUseAddr = DL698_Freeze_ProfileTab[ItemNo].EntriesUseAddr;
		for( j=i+1;j<Items;j++ )											//删除一项后捕获曲线参数整体前移
		{
			E2P_RData( Ptr, CapObjAddr+j*10, 9 );
			E2P_WData( CapObjAddr+(j-1)*10, Ptr, 9 );

			E2P_RData( Ptr, PtAddr+j*3, 2 );
			E2P_WData( PtAddr+(j-1)*3, Ptr, 2 );

			E2P_RData( Ptr, RecdLenAddr+j*3, 2 );
			E2P_WData( RecdLenAddr+(j-1)*3, Ptr, 2 );

			E2P_RData( Ptr, EntriesUseAddr+j*3, 2 );
			E2P_WData( EntriesUseAddr+(j-1)*3, Ptr, 2 );
		}
		Items -= 1;
		*Ptr = Items;
		E2P_WData( DL698_Freeze_ProfileTab[ItemNo].CapNumAddr, Ptr, 1 );	//曲线总数减一
	}	
	
	return 0;
}		

/*
unsigned char AddFreezeCapture_OAD( unsigned short ItemNo, unsigned char* Source )
{
//	unsigned char BakFlag=0;
	unsigned char* Ptr;
	unsigned short Items;
	unsigned short GetLen;
	unsigned char ErrorByte;
	
	Ptr = Source+4;																		//冻结周期类型
	Items = GetFreezeCaptureItems( ItemNo );
	Items += 1;
	if( Items > DL698_Freeze_ProfileTab[ItemNo].CapNumMax ) return 1;					//OAD已全满，无法添加
	GetLen = 0;
//	if(	Judge_capture_objects_content( Ptr+3, &GetLen, GetBakFlag( ItemNo, Ptr+4 ) ) != 0 ) return 1;	//该OAD不支持 
	if(	Judge_capture_objects_content( Ptr+6, &GetLen, GetBakFlag( ItemNo, Ptr+6 ) ) != 0 ) return 1;	//该OAD不支持 
	ErrorByte = InitialFreezeProfilePara( ItemNo, Items-1, Ptr+2, GetLen );				//初始化事件曲线的部分参数
	if( ErrorByte == 0 ) 
	{
		E2P_WData( DL698_Freeze_ProfileTab[ItemNo].CapNumAddr, (unsigned char*)&Items, 1 );
	}	
	
	return ErrorByte;
}		
*/
unsigned char AllNewOADIsNotExist( unsigned short ItemNo, unsigned char* OAD, unsigned short NewItems )
{
	short i,j;
	unsigned char Buff[10];
	unsigned char* Point;
	unsigned char* Ptr;
	unsigned short CaptureItems;
	unsigned short CapObjAddr;
	
	Point = Buff;
	
	CaptureItems = GetFreezeCaptureItems( ItemNo );						//已存曲线数
    CapObjAddr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;  			
    for( i=0;i<CaptureItems;i++ )
    {
    	Ptr = OAD;
    	E2P_RData( Point, CapObjAddr+i*10, 9 );
    	for( j=0;j<NewItems;j++ )
    	{
   			if( Data_Comp( Point+2, Ptr, 4 ) == 0 )							//逐条判断已有的OAD曲线是否与准备添加的一致,只与OAD有关
   			{
   				return 1;													//已有不允许添加
    		}
    		Ptr += 13;
    	}	
	}
	
	return 0;
}	

unsigned char OADIsAlreadyExist( unsigned short ItemNo, unsigned char* OAD )
{
	short i;
	unsigned char Buff[10];
	unsigned char* Point;
	unsigned short CaptureItems;
	unsigned short CapObjAddr;
	
	Point = Buff;
	
	CaptureItems = GetFreezeCaptureItems( ItemNo );						//已存曲线数
    CapObjAddr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;  			
    for( i=0;i<CaptureItems;i++ )
    {
    	E2P_RData( Point, CapObjAddr+i*10, 9 );
   		if( Data_Comp( Point+2, OAD, 4 ) == 0 )							//逐条判断已有的OAD曲线是否与准备添加的一致,只与OAD有关
   		{
   			return 1;													//已有不允许添加
    	}
	}
	
	return 0;
}	

unsigned char AddFreezeCapture_OAD( unsigned short ItemNo, unsigned char* Source )
{
//	unsigned char BakFlag=0;
	unsigned char* Ptr;
	unsigned short Items;
	unsigned short GetLen;
	unsigned char ErrorByte;
	unsigned char BakFlag;
	unsigned short TotalDir;
	unsigned short TotalSectors;
	unsigned short Remain;
	unsigned short RemainDir;
	unsigned short RemainSectors;
	unsigned short RecdLenth;
	unsigned short Pages;
	
	Ptr = Source+4;																		//冻结周期类型
	Items = GetFreezeCaptureItems( ItemNo );
	Items += 1;
	if( Items > DL698_Freeze_ProfileTab[ItemNo].CapNumMax ) return 1;					//OAD已全满，无法添加
	GetLen = 0;
	BakFlag = GetBakFlag( ItemNo, Ptr+6 );
	if(	Judge_capture_objects_content( Ptr+6, &GetLen, BakFlag ) != 0 ) return 1;		//该OAD不支持 
	if( OADIsAlreadyExist( ItemNo, Ptr+6 ) != 0 ) return 1;								//该OAD已存在 


	RecdLenth = GetLen;
	RecdLenth += GetFreezeHeadLen( ItemNo, BakFlag );
	Pages = GetFreezeRecordPags( Ptr+11, RecdLenth );									//每条冻结记录占用页数
	TotalDir = 1;
	if( BakFlag != 0 )																	//带备份数据空间翻倍
	{
		TotalDir += 1;
		Pages *= 2;
	}	
	TotalSectors = Pages;
	RemainDir = GetRemainDirs( &Remain );
	if( TotalDir > RemainDir ) return 1;												//剩余目录数不足
	RemainSectors = GetRemainSectors( &Remain );										//总剩余扇区数
	if( TotalSectors > RemainSectors ) return 1;										//剩余扇区数不足

	ErrorByte = InitialFreezeProfilePara( ItemNo, Items-1, Ptr+2, GetLen );				//初始化事件曲线的部分参数
	if( ErrorByte == 0 ) 
	{
		E2P_WData( DL698_Freeze_ProfileTab[ItemNo].CapNumAddr, (unsigned char*)&Items, 1 );
	}	
	
	return ErrorByte;
	
}		


/*
unsigned char InitialFreezeProfilePara( unsigned short ItemNo, unsigned short CurveNo, unsigned char* Source, unsigned short ItemLen )
{
	unsigned char Buff[20];
	unsigned char Buff2[20];
	unsigned char* FileName;
//	unsigned short Pages;
	unsigned short Addr;
	unsigned long  WriteLen;
	unsigned long  RealLen;
//	unsigned short Temp;
	unsigned short Entries;
	unsigned char* Ptr;
//	unsigned long CurFilePt;
	unsigned char ErrorByte1=FR_OK;
	unsigned char ErrorByte2=FR_OK;
	unsigned char BakFlag=0;
	
	Ptr = Buff;
	FileName = Buff2;
	
	BakFlag = GetBakFlag( ItemNo, Source+4 );
	ItemLen += GetFreezeHeadLen( ItemNo, BakFlag );

//以冻结记录序号和OAD作为文件名，在删除曲线而进行OAD挪移时，不需要进行文件名更名操作，带字符串结束符
	CreatFileName( FileName, ItemNo, Source+4, 0 );
	Entries = SwapCharToShort( Source+9 );
	WriteLen = Entries;
	WriteLen *= ItemLen;
	ErrorByte1 = f_open( &fsrc, (char const*)FileName, FA_CREATE_NEW | FA_WRITE);
    if( ErrorByte1 == FR_OK )
    { 
		ErrorByte1 = f_lseek (&fsrc, WriteLen );
		if( ErrorByte1 == FR_OK )
		{
			*Ptr = 0xff;
    		ErrorByte1 = f_write(&fsrc, Ptr, 1, (UINT*)&RealLen);     
    	}	      	
		ErrorByte1 = f_close(&fsrc);      		
    }
    if( BakFlag == 1 )		//带备份
    {
    	CreatFileName( FileName, ItemNo, Source+4, BakFlag );
		ErrorByte1 = f_open( &fsrc, (char const*)FileName, FA_CREATE_NEW | FA_WRITE);
		if( ErrorByte2 == FR_OK )
    	{ 
			ErrorByte2 = f_lseek (&fsrc, WriteLen );
			if( ErrorByte2 == FR_OK )
			{
				*Ptr = 0xff;
    			ErrorByte2 = f_write(&fsrc, Ptr, 1, (UINT*)&RealLen);     
    		}	      	
			ErrorByte2 = f_close(&fsrc);      		
    	}    	
    }	
	
	if(( ErrorByte1 == FR_OK )&&( ErrorByte2 == FR_OK ))
	{
		SwapData( Source+1, 2 );										//冻结周期
		RAM_Write( Ptr, Source+1, 2 );
		RAM_Write( Ptr+2, Source+4, 4 );								//冻结OAD
		RAM_Write( Ptr+6, (unsigned char *)&Entries, 2 );				//冻结深度
		*(Ptr+8) = BakFlag;												//是否带备份 0：无；1：带 每项数据带校验
		Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
		Addr += CurveNo * 10;
		E2P_WData( Addr, Ptr, 9 );
		
		Addr = DL698_Freeze_ProfileTab[ItemNo].RecdLenAddr;				//单条记录长度
		Addr += CurveNo * 3;
		E2P_WData( Addr, (unsigned char*)&ItemLen, 2 );

		RAM_Fill( Ptr, 4);
		Addr = DL698_Freeze_ProfileTab[ItemNo].PtAddr;					//当前记录指针
		Addr += CurveNo * 3;
		E2P_WData( Addr, Ptr, 2 );

		Addr = DL698_Freeze_ProfileTab[ItemNo].EntriesUseAddr;			//有效记录数
		Addr += CurveNo * 3;
		E2P_WData( Addr, Ptr, 2 );
	}	
	
	return ErrorByte1|ErrorByte2;
}	
*/

//16.10.12
unsigned char InitialFreezeProfilePara( unsigned short ItemNo, unsigned short CurveNo, unsigned char* Source, unsigned short ItemLen )
{
	unsigned char Buff[20];
	unsigned char Buff2[20];
	unsigned char* FileName;
	unsigned short Addr;
	unsigned short Entries;
	unsigned short ShortBuff[5];
	FILEPARA* FilePara;
	unsigned char* Ptr;
	unsigned char ErrorByte1=FR_OK;
	unsigned char ErrorByte2=FR_OK;
	unsigned char BakFlag=0;

	FilePara = (FILEPARA*)ShortBuff;
	
	Ptr = Buff;
	FileName = Buff2;
	
	BakFlag = GetBakFlag( ItemNo, Source+4 );
	ItemLen += GetFreezeHeadLen( ItemNo, BakFlag );
	FilePara->Sectors = GetFreezeRecordPags( Source+9, ItemLen );	//每条冻结记录占用页数
	Entries = SwapCharToShort( Source+9 );
//以冻结记录序号和OAD作为文件名，在删除曲线而进行OAD挪移时，不需要进行文件名更名操作
	CreatFileName( FileName, ItemNo, Source+4, 0 );
	ErrorByte1 = CreatFile( FilePara, FileName );
    if( BakFlag == 1 )		//带备份
    {
    	CreatFileName( FileName, ItemNo, Source+4, BakFlag );
		ErrorByte2 = CreatFile( FilePara, FileName );
    }	
	
	if(( ErrorByte1 == FR_OK )&&( ErrorByte2 == FR_OK ))
	{
		SwapData( Source+1, 2 );										//冻结周期
		RAM_Write( Ptr, Source+1, 2 );
		RAM_Write( Ptr+2, Source+4, 4 );								//冻结OAD
		RAM_Write( Ptr+6, (unsigned char *)&Entries, 2 );				//冻结深度
		*(Ptr+8) = BakFlag;												//是否带备份 0：无；1：带 每项数据带校验
		Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
		Addr += CurveNo * 10;
		E2P_WData( Addr, Ptr, 9 );
		
		Addr = DL698_Freeze_ProfileTab[ItemNo].RecdLenAddr;				//单条记录长度
		Addr += CurveNo * 3;
		E2P_WData( Addr, (unsigned char*)&ItemLen, 2 );

		RAM_Fill( Ptr, 4);
		Addr = DL698_Freeze_ProfileTab[ItemNo].PtAddr;					//当前记录指针
		Addr += CurveNo * 3;
		E2P_WData( Addr, Ptr, 2 );

		Addr = DL698_Freeze_ProfileTab[ItemNo].EntriesUseAddr;			//有效记录数
		Addr += CurveNo * 3;
		E2P_WData( Addr, Ptr, 2 );
	}	
	
	return ErrorByte1|ErrorByte2;
}	


unsigned char SetFreezeClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = SetFreezeClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen  );
	return ErrorByte;		

}	

//取PhaseEvent类数据
unsigned char SetFreezeClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	short i,j;
	unsigned char Buff[4];
	unsigned char* Ptr;
	unsigned char ErrorByte=object_class_inconsistent;
//	short Len;
//	unsigned short OICode;
//	short Temp;
	unsigned short GetLen;
//	short Start,End;
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
	unsigned char* OAD;
	unsigned char* WriteBufAds;
//	unsigned short CapNumMax;		//曲线最大捕获项数(物理空间允许接收的项数)
        unsigned short Item;  
//	Len = -1;
	unsigned short* ComBTime;

	Ptr = Buff;
	OAD = Source;
	WriteBufAds = Source+4;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
  	ComBTime = CommPara[COM_No].BTime;
			
	i = GetFreezeClassTabItemNo( OAD );
	if( i != DL698_Freeze_ProfileSum )
	{
		if( Attribute == 3 )			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_Freeze_ProfileTab[i].SaftInfo, write_authority ) != 0 ) return read_write_denied;	//17.04.09
		}
//		*Dest = Get_data;								//正常数据返回		
		switch( Attribute )
		{
			case 1:										//logical_name
				break;	
			case 2:										//	
				break;	
			case 3:										//关联对象属性表     
				
//				Read_Flash( Ptr, 1800*4096, 4 );           //Test FLASH 
//				RAM_Write( Ptr, OAD, 4 );
//				DataFlash_Write( 1800*4096, Ptr, 4 );
//				Read_Flash( Ptr, 1800*4096, 4 );
//				break;                                      //Test FLASH 
			        
//				ErrorByte = Judge_freeze_capture_objects( WriteBufAds, i, &GetLen, ItemLen );
				ErrorByte = Judge_freeze_capture_objects( WriteBufAds, i, &GetLen, ItemLen, 0 );
				if( ErrorByte == 0 )
				{
					Item = *(WriteBufAds+1);								//元素个数
					
					
					Ptr = WriteBufAds+4;									//冻结周期的数据类型
					*ComBTime = MS4000;			
					DeleteFreezeProfileFile( i );							//删除该冻结曲线原先的文件	
					for( j=0;j<Item;j++ )
					{
						*ComBTime = MS4000;			
                        GetLen = 0;
// 						Judge_capture_objects_content( Ptr+3, &GetLen, GetBakFlag( i, Ptr+4 ) ); 
 						Judge_capture_objects_content( Ptr+4, &GetLen, GetBakFlag( i, Ptr+4 ) ); 
						ErrorByte = InitialFreezeProfilePara( i, j, Ptr, GetLen );		//初始化事件曲线的部分参数
                   		if( ErrorByte != 0 ) break;
//						Ptr += 11;					
						Ptr += 13;					
					}
					*ComBTime = MS500;			
					if( ErrorByte != 0 ) 
					{
						*(WriteBufAds+1) = 0;
						ErrorByte = object_class_inconsistent;							
					}
					E2P_WData( DL698_Freeze_ProfileTab[i].CapNumAddr, WriteBufAds+1, 1 );		//捕获项数
					RAM_Fill( Ptr, 4 );
					E2P_WData( DL698_Freeze_ProfileTab[i].FinalRecNoAddr, Ptr, 4 );				//最新冻结项
				}							
				break;	
			case 4:										//配置参数	
				break;	
			default:
				break;	
		}	
	}		
	return ErrorByte;
}	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ActionFreezeClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = ActionFreezeClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen, BackLen  );
	return ErrorByte;		

}	

unsigned char ActionFreezeClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char Buff[4];
	short i,j;
//	unsigned char* Addr;
	unsigned char ErrorByte=object_class_inconsistent;
//	short Len;
//	unsigned short OICode;
	short Temp;
	unsigned short GetLen;
//	short Start,End;
//	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
	unsigned char* Ptr;
	unsigned char* OMD;
	unsigned char* WriteBufAds;
	unsigned char Method;
	unsigned short EntriesUseAddr;
	unsigned short PtAddr;
//	unsigned short FinalRecNoAddr;
//	Len = -1;
    unsigned short Item;  
    unsigned short ExistingItems;  
	unsigned short* ComBTime;
//	unsigned short SaftInfo;		//17.04.09
	
  	ComBTime = CommPara[COM_No].BTime;
	OMD = Source;
	WriteBufAds = Source+4;
//	OICode = SwapCharToShort( OMD );
	Method = *(OMD+2);
	
	Ptr = Buff;
	
	i = GetFreezeClassTabItemNo( OMD );
	if( i == DL698_Freeze_ProfileSum ) { return object_undefined; }

	if(( Method == 1 )||(( Method >= 3 )&&( Method <= 5 ))||(( Method >= 7 )&&( Method <= 8 )))			//17.04.09
	{
		if( Judge_operating_authority( COM_No, OMD, DL698_Freeze_ProfileTab[i].SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
	}
//	SaftInfo = DL698_Freeze_ProfileTab[i].SaftInfo;	//17.04.09
	switch( Method )
	{
		case 1:										//复位
			*ItemLen += 6;
//			if( Judge_operating_authority( COM_No, OMD, SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
			Temp = 0;
			E2P_RData( (unsigned char*)&Temp, DL698_Freeze_ProfileTab[i].CapNumAddr, 1 );
			if( Temp > DL698_Freeze_ProfileTab[i].CapNumMax ) Temp = DL698_Freeze_ProfileTab[i].CapNumMax;
			EntriesUseAddr = DL698_Freeze_ProfileTab[i].EntriesUseAddr;
			PtAddr = DL698_Freeze_ProfileTab[i].PtAddr;
			RAM_Fill( Ptr, 4 );
			for( j=0;j<Temp;j++ )
			{
				E2P_WData( EntriesUseAddr, Ptr, 2 );
				E2P_WData( PtAddr, Ptr, 2 );
				EntriesUseAddr += 3;
				PtAddr += 3;			
			}
			E2P_WData( DL698_Freeze_ProfileTab[i].FinalRecNoAddr, Ptr, 4 );
			ErrorByte = 0;
//			*ItemLen += 6;
			break;	
		case 2:										//	
			break;	
		case 3:	
			*ItemLen += 8;
//			if( Judge_operating_authority( COM_No, OMD, SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
//          E2P_RData( (unsigned char*)&FinalRecNoAddr, DL698_Freeze_ProfileTab[i].FinalRecNoAddr, 4 );
//			RecordSingleFreeze( i,FinalRecNoAddr, &Clk.YearH, 0 );
			RecordFreeze( i, 0 );
//			*ItemLen += 8;
			ErrorByte = 0;
			break;	
		case 4:										//
			*ItemLen += 13;
//			if( Judge_operating_authority( COM_No, OMD, SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
			ErrorByte = AddFreezeCapture_OAD( i, Source );
			if( ErrorByte != 0 ) ErrorByte = object_class_inconsistent;	
//			*ItemLen += 13;
			break;	
		case 5:										//配置参数
			*ItemLen += 5;
//			if( Judge_operating_authority( COM_No, OMD, SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
			ErrorByte = DeleteFreezeCapture_OAD( i, WriteBufAds+1 );	
//			*ItemLen += 5;
			break;
		case 7:										//配置关联对象属性表
			ExistingItems = GetFreezeCaptureItems( i );
//			ErrorByte = Judge_freeze_capture_objects( WriteBufAds, i, &GetLen, ItemLen );
			ErrorByte = Judge_freeze_capture_objects( WriteBufAds, i, &GetLen, ItemLen, 1 );
			if( ErrorByte == 0 )
			{
//				if( Judge_operating_authority( COM_No, OMD, SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
				Item = *(WriteBufAds+1);								//元素个数
				if( AllNewOADIsNotExist( i, WriteBufAds+8, Item ) != 0 ) return object_class_inconsistent;	//若配置内容已存在，返回出错
				Ptr = WriteBufAds+4;									//冻结周期的数据类型
				*ComBTime = MS4000;			
//				DeleteFreezeProfileFile( i );							//删除该冻结曲线原先的文件	
				for( j=0;j<Item;j++ )
				{
					*ComBTime = MS4000;			
                    GetLen = 0;
 					Judge_capture_objects_content( Ptr+4, &GetLen, GetBakFlag( i, Ptr+4 ) ); 
//					ErrorByte = InitialFreezeProfilePara( i, j+ExistingItems, Ptr, GetLen );		//初始化事件曲线的部分参数
					InitialFreezeProfilePara( i, j+ExistingItems, Ptr, GetLen );		//初始化事件曲线的部分参数
//             		if( ErrorByte != 0 ) break;
					Ptr += 13;					
				}
				*ComBTime = MS500;			
//				if( ErrorByte != 0 ) 
//				{
//					*(WriteBufAds+1) = 0;
//					ErrorByte = object_class_inconsistent;							
//				}
				*(WriteBufAds+1) += ExistingItems;
				E2P_WData( DL698_Freeze_ProfileTab[i].CapNumAddr, WriteBufAds+1, 1 );		//捕获项数
				RAM_Fill( Ptr, 4 );
				E2P_WData( DL698_Freeze_ProfileTab[i].FinalRecNoAddr, Ptr, 4 );				//最新冻结项
				ErrorByte = 0;
			}							
			break;
		case 8:										//删除关联对象属性表
			*ItemLen += 5;
//			if( Judge_operating_authority( COM_No, OMD, SaftInfo, active_authority ) != 0 ) return read_write_denied;	//17.04.09
			DeleteFreezeProfileFile( i );							//删除该冻结曲线原先的文件	
			ErrorByte = 0;
//			*ItemLen += 5;
			break;
		default:
			break;	
	}	
	return ErrorByte;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
typedef struct
{
	unsigned short CurRecNo;		//当前记录号
	unsigned short Space;			//冻结间隔
	unsigned char OAD[4];
	unsigned short RecdCunt;		//存储空间最大记录数
    unsigned short AvailRecs;		//当前已存储的有效记录数
	unsigned short RecdLenth;		//记录长度
	unsigned short ChannelNo;		//通道号
	unsigned char  BakFlag;			//是否有备份标志0：无备份；1有备份，每项数据带1字节校验和
}FILECURVEPARA;
*/
/*
typedef struct
{
	unsigned short CurRecNo;		//当前记录号
	unsigned short Space;			//冻结间隔
	unsigned char OAD[4];
	unsigned short MaxCunt;			//存储空间最大记录数
	unsigned short RecdCunt;		//存储空间最大记录数，去除4K轮转池后
    unsigned short AvailRecs;		//当前已存储的有效记录数
	unsigned short  Pages;			//存储空间所占用页数
	unsigned short  RecsInPage;		//每页记录数
	unsigned short RecdLenth;		//记录长度
	unsigned short ChannelNo;		//通道号
	unsigned short DirNo;			//文件目录序号
	unsigned short StartSectorNo;	//起始索引号
	unsigned char  BakFlag;			//是否有备份标志0：无备份；1有备份，每项数据带1字节校验和
}FILECURVEPARA;
*/					//16.10.12
void GetFileCurvePara( FILECURVEPARA* FileCurvePara )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned short ItemNo; 
	unsigned short CurveNo;
	unsigned short Addr;
	unsigned short GetLen = 0;
	
	Ptr = Buff;
	
	ItemNo = FileCurvePara->ChannelNo>>8;											//冻结号
	CurveNo = FileCurvePara->ChannelNo & 0xFF;										//冻结中曲线号
	
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	Addr += CurveNo * 10;
	E2P_RData( Ptr, Addr, 9 );
	RAM_Write( (unsigned char*)&FileCurvePara->Space, Ptr, 2 );						//冻结周期
	RAM_Write( (unsigned char*)FileCurvePara->OAD, Ptr+2, 4 );						//冻结OAD
	RAM_Write( (unsigned char*)&FileCurvePara->RecdCunt, Ptr+6, 2 );				//冻结深度
	Addr = DL698_Freeze_ProfileTab[ItemNo].EntriesUseAddr;
	Addr += CurveNo * 3;
	E2P_RData( Ptr, Addr, 2 );
	RAM_Write( (unsigned char*)&FileCurvePara->AvailRecs, Ptr, 2 );					//已存记录数
	
	if( FileCurvePara->AvailRecs > FileCurvePara->RecdCunt ) FileCurvePara->AvailRecs = 0;	//已存记录数超过允许记录数，非法，已存记录数清零。表示没有有效记录
	
	GetLen = 0;
	*Ptr = D_OAD;
	RAM_Write( Ptr+1, (unsigned char*)FileCurvePara->OAD, 4 );
	FileCurvePara->BakFlag = GetBakFlag( ItemNo, Ptr+1 );
//	Judge_capture_objects_content( Ptr, &GetLen, GetBakFlag( ProfileNo, FileCurvePara->BakFlag ) ); 	//返回OAD对应的数据长度：包括类型和1字节校验
	Judge_capture_objects_content( Ptr+1, &GetLen, GetBakFlag( ItemNo, FileCurvePara->OAD ) ); 	//返回OAD对应的数据长度：包括类型和1字节校验
//	GenLen += 7+4;																	//增加每条记录时钟+冻结号
	GetLen += 7+4;																	//增加每条记录时钟(7字节)+冻结号
	if( FileCurvePara->BakFlag != 0 ) GetLen += 2;								//增加每条记录时钟带校验+冻结号带校验								
	FileCurvePara->RecdLenth = GetLen;												//单条记录总长度	

	Addr = DL698_Freeze_ProfileTab[ItemNo].PtAddr;
	Addr += CurveNo * 3;
	E2P_RData( Ptr, Addr, 2 );
	RAM_Write( (unsigned char*)&FileCurvePara->CurRecNo, Ptr, 2 );
	
	FileCurvePara->RecsInPage = 4096 / FileCurvePara->RecdLenth;					//每页记录数				//16.10.12
	FileCurvePara->Pages = FileCurvePara->RecdCunt / FileCurvePara->RecsInPage;		//
	if(( FileCurvePara->RecdCunt % FileCurvePara->RecsInPage ) != 0 ) FileCurvePara->Pages += 1;
	FileCurvePara->Pages += 1;														//总页数增加一页轮转池	
	FileCurvePara->MaxCunt = FileCurvePara->Pages * FileCurvePara->RecsInPage;		//存储空间内最大记录数
	
}	
/*																		//16.10.12
//CurvePara:负荷曲线参数
//type: -1:新产生一条记录，其他：指定记录号
//返回值：指定曲线的指定记录号在文件的物理地址。 -1：该记录不存在
long LoadFilePtrChk( FILECURVEPARA* FileCurvePara, long Type )
{
	long Temp;
	unsigned short CurRecNo;			//当前记录号
	unsigned short ItemNo; 
	unsigned short CurveNo;
	unsigned short Addr;
	unsigned short RecNo;				//记录号
	
	ItemNo = FileCurvePara->ChannelNo>>8;											//冻结号
	CurveNo = FileCurvePara->ChannelNo & 0xFF;										//冻结中曲线号

	if( Type == -1 )
	{
		FileCurvePara->CurRecNo += 1;													//新分配一条记录
		if( FileCurvePara->CurRecNo >= FileCurvePara->RecdCunt ) FileCurvePara->CurRecNo = 0;	//超过总记录数，指向第一条。
		if( FileCurvePara->AvailRecs == 0 ) FileCurvePara->CurRecNo = 0;					//该通道第一次产生记录，指向第一条。
																					//可配合用于当目标记录指向页的逻辑零地址时，将当前页先擦除后再写入数据，与写FLASH程序配合。		
//		E2P_WData( DL698_Event_ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, (unsigned char*)&CurvePara->CurRecNo, 2 );//保存当前记录号	
		Addr = DL698_Freeze_ProfileTab[ItemNo].PtAddr;
		Addr += CurveNo * 3;
		E2P_WData( Addr, (unsigned char*)&FileCurvePara->CurRecNo, 2 );

		if( FileCurvePara->AvailRecs < FileCurvePara->RecdCunt )							//已存记录数小于抄收限制记录数
		{ 												
			FileCurvePara->AvailRecs++;													//已存记录数加1后存入EEPROM	
//			E2P_WData( DL698_Event_ProfileTab[CurvePara->ChannelNo].EntriesUseAddr, (unsigned char*)&CurvePara->AvailRecs, 2 );	
			Addr = DL698_Freeze_ProfileTab[ItemNo].EntriesUseAddr;
			Addr += CurveNo * 3;
			E2P_WData( Addr, (unsigned char*)&FileCurvePara->AvailRecs, 2 );
		}	
		CurRecNo = FileCurvePara->CurRecNo;			//当前记录号
	}
	else
	{
	  	RecNo = (unsigned short)Type;												//恢复为2字节	
      	if( RecNo >= FileCurvePara->AvailRecs ) return -1;								//所抄记录超限，返回空。	
      	if( FileCurvePara->CurRecNo >= RecNo ) CurRecNo = FileCurvePara->CurRecNo - RecNo;	//抄收记录号小于当前记录号，直接从当前记录向前递推找到目标记录
	  	else CurRecNo = FileCurvePara->CurRecNo + FileCurvePara->RecdCunt - RecNo;			//否则转到存储区末端。	  
	}
	Temp = CurRecNo;																//记录号
	Temp *= FileCurvePara->RecdLenth;													//数据文件中的地址

	return Temp;																	//返回当前或指向的记录号

}
*/

//16.10.12
//FileCurvePara:文件曲线参数
//type: -1:新产生一条记录，其他：指定记录号
//返回值：指定文件的指定记录号在本文件中顺序存储的记录号（具体物理地址需根据不同文件的扇区地址计算得到） -1：该记录不存在
long LoadFilePtrChk( FILECURVEPARA* FileCurvePara, long Type )
{
//	long Temp;
	unsigned short CurRecNo;			//当前记录号
	unsigned short ItemNo; 
	unsigned short CurveNo;
	unsigned short Addr;
	unsigned short RecNo;				//记录号

	ItemNo = FileCurvePara->ChannelNo>>8;											//冻结号
	CurveNo = FileCurvePara->ChannelNo & 0xFF;										//冻结中曲线号

	if( Type == -1 )
	{
		FileCurvePara->CurRecNo += 1;													//新分配一条记录
		if( FileCurvePara->CurRecNo >= FileCurvePara->MaxCunt ) FileCurvePara->CurRecNo = 0;	//超过总记录数，指向第一条。
		if( FileCurvePara->AvailRecs == 0 ) FileCurvePara->CurRecNo = 0;					//该通道第一次产生记录，指向第一条。
																					//可配合用于当目标记录指向页的逻辑零地址时，将当前页先擦除后再写入数据，与写FLASH程序配合。		
//		E2P_WData( DL698_Event_ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, (unsigned char*)&CurvePara->CurRecNo, 2 );//保存当前记录号	
		Addr = DL698_Freeze_ProfileTab[ItemNo].PtAddr;
		Addr += CurveNo * 3;
		E2P_WData( Addr, (unsigned char*)&FileCurvePara->CurRecNo, 2 );

		if( FileCurvePara->AvailRecs < FileCurvePara->RecdCunt )							//已存记录数小于抄收限制记录数
		{ 												
			FileCurvePara->AvailRecs++;													//已存记录数加1后存入EEPROM	
//			E2P_WData( DL698_Event_ProfileTab[CurvePara->ChannelNo].EntriesUseAddr, (unsigned char*)&CurvePara->AvailRecs, 2 );	
			Addr = DL698_Freeze_ProfileTab[ItemNo].EntriesUseAddr;
			Addr += CurveNo * 3;
			E2P_WData( Addr, (unsigned char*)&FileCurvePara->AvailRecs, 2 );
		}	
		CurRecNo = FileCurvePara->CurRecNo;			//当前记录号
	}
	else
	{
	  	RecNo = (unsigned short)Type;												//恢复为2字节	
      	if( RecNo >= FileCurvePara->AvailRecs ) return -1;								//所抄记录超限，返回空。	
      	if( FileCurvePara->CurRecNo >= RecNo ) CurRecNo = FileCurvePara->CurRecNo - RecNo;		//抄收记录号小于当前记录号，直接从当前记录向前递推找到目标记录
	 	else CurRecNo = FileCurvePara->CurRecNo + FileCurvePara->MaxCunt - RecNo;				//否则转到存储区末端。	  
	}	
//	Temp1 = (CurRecNo % CurvePara->RecsInPage)*CurvePara->RecdLenth;				//目标记录首地址在其所处页内的逻辑地址
//	Temp = CurvePara->StartPageNo;													//负荷记录存储区在FLASH芯片内的起始页号	
//	Temp += CurRecNo/CurvePara->RecsInPage;											//目标记录在FLASH芯片内的页号
//	Temp = Temp * 4096 + Temp1;														//目标记录首地址在FLASH芯片内物理地址

//	return Temp;																	//返回当前或指向的记录号

	return CurRecNo;
}

//读取文件记录的真正物理地址				//16.10.12
long GetFileRecRealFlashAddr( FILECURVEPARA* FileCurvePara, long CurRecNo )
{
	long Temp,Temp1;
	
	if( CurRecNo == -1 ) return -1;
	Temp1 = (CurRecNo % FileCurvePara->RecsInPage)*FileCurvePara->RecdLenth;		//目标记录首地址在其所处页内的逻辑地址
	Temp = GetAppointSector( FileCurvePara->StartSectorNo, CurRecNo/FileCurvePara->RecsInPage );	
	if( Temp == -1 ) return -1;
	Temp += Temp1;

	return Temp;
}

void ResetFreezeMD( short ChannelNo )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned short ItemNo; 
	unsigned short CurveNo;
	unsigned short Addr;
	unsigned long ComID;
	
	Ptr = Buff;
	
	ItemNo = ChannelNo>>8;											//冻结号
	CurveNo = ChannelNo & 0xFF;										//冻结中曲线号
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	Addr += CurveNo * 10;
	E2P_RData( Ptr, Addr, 9 );
	ComID = SwapCharToLong( Ptr+2 );
	RAM_Fill( Ptr, 10);
	if(( ComID == 0x11100200 )||( ComID == 0x11100201 ))
	{
		E2P_WData( FreezeMD_Pp0, Ptr, 9 );
	}
	else if(( ComID == 0x11200200 )||( ComID == 0x11200201 ))
	{
		E2P_WData( FreezeMD_Pn0, Ptr, 9 );
	}		
}	

unsigned short GetFreezeSpace( short ChannelNo )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned short ItemNo; 
	unsigned short CurveNo;
	unsigned short Addr;
	unsigned short Space;
	
	Ptr = Buff;
	
	ItemNo = ChannelNo>>8;											//冻结号
	CurveNo = ChannelNo & 0xFF;										//冻结中曲线号
	
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	Addr += CurveNo * 10;
	E2P_RData( Ptr, Addr, 9 );
	RAM_Write( (unsigned char*)&Space, Ptr, 2 );					//冻结周期

	return Space;
}

//Type: 0:直接冻结；1：按冻结周期冻结
unsigned char AllowFreeze( short ChannelNo, short Type )
{
	unsigned long Space;
	unsigned long Value;
	unsigned short ItemNo; 
	unsigned char Flag=0;
        
	
	if( Type == 0 ) return 0;						//直接冻结
    ItemNo = ChannelNo>>8;
	switch( ItemNo )
	{
		case InstantFreeze_No:						//0			//瞬时冻结记录号 			
			break;              					
		case SecFreeze_No:							//1			//秒冻结记录号
			Space = GetFreezeSpace( ChannelNo );
			Value = GetGmSec( Clk.Sec ); 				
			if(( Value % Space ) != 0 ) Flag = 1;
			break;              					
		case MinFreeze_No:							//2			//分钟冻结记录号 			
//			if( Clk.Sec[0] != 0 ) { Flag = 1; break; }
//			else
//			{								
				Space = GetFreezeSpace( ChannelNo );
//				Space *= 60;
				Value = GetGmMin( Clk.Min ); 				
				if(( Value % Space ) != 0 ) Flag = 1;
//           }
			break;              					
		case HourFreeze_No:							//3			//小时冻结记录号 			
			if( Clk.Min[0] != 0 ) { Flag = 1; break; }
			else
			{								
				Space = GetFreezeSpace( ChannelNo );
				Space *= 60;
//				Value = GetGmSec( Clk.Min ); 				
				Value = GetGmMin( Clk.Min ); 				    //17.01.20
				if(( Value % Space ) != 0 ) Flag = 1;
			}
			break;              					
		case DayFreeze_No:							//4			//日冻结记录号 				
			if(( Clk.Hour[0] != 0 )||( Clk.Min[0] != 0 )) { Flag = 1; break; }
			else
			{								
				Space = GetFreezeSpace( ChannelNo );
				Value = GetGmDay( Clk.Day ); 				
				if(( Value % Space ) != 0 ) Flag = 1;
			}
			break;              					
		case SaveDayFreeze_No:						//5			//结算日冻结记录号 			
			break;              					
		case MonthFreeze_No:						//6			//月冻结记录号 
			if(( Clk.Day[0] != 0x01 )||( Clk.Hour[0] != 0 )||( Clk.Min[0] != 0 )) { Flag = 1; break; }
			else
			{								
				Space = GetFreezeSpace( ChannelNo );
				Value = ( SwapCharToShort( &Clk.YearH ) - 2000 ) * 12 + Clk.Month; 				
				if(( Value % Space ) != 0 ) Flag = 1;
			}	
			break;              					
		case YearFreeze_No:							//8			//年冻结记录号 				
			if(( Clk.Mon[0] != 0x01 )||( Clk.Day[0] != 0x01 )||( Clk.Hour[0] != 0 )||( Clk.Min[0] != 0 )) { Flag = 1; break; }
			else
			{								
				Space = GetFreezeSpace( ChannelNo );
				Value = SwapCharToShort( &Clk.YearH ) - 2000; 				
				if(( Value % Space ) != 0 ) Flag = 1;
			}	
			break;              					
		case ZoneSWFreeze_No:						//9			//时区表切换冻结记录号 		
		case SegTabSWFreeze_No:						//10		//日时段表切换冻结记录号 	
		case FeeSWFreeze_No:						//11		//费率电价切换冻结记录号 	
		case StepSWFreeze_No:						//12		//阶梯切换冻结记录号 		
		case StepSaveSWFreeze_No:					//13		//阶梯结算冻结记录号 		
			break;
		default:
			break;		
	}
	
	return Flag;
}	

/*
//Type: 0:直接冻结；1：按冻结周期冻结
unsigned char RecordSingleFreeze( short ChannelNo, unsigned long FinalRecNo, short Type )
{
	unsigned char Buff2[20];
	unsigned char Buff[200];
	unsigned char* Point;
	unsigned char* FileName;
    unsigned long LongBuff[8];
    unsigned long WriteLen;
	FILECURVEPARA* FileCurvePara;
	unsigned char ErrorByte=1;
	long Value;
	
	Point = Buff;
	FileName = Buff2;
	FileCurvePara = (FILECURVEPARA*)LongBuff;
	
	if( AllowFreeze( ChannelNo, Type ) != 0 ) return ErrorByte;
	
	FileCurvePara->ChannelNo = ChannelNo;
	GetFileCurvePara( FileCurvePara );
	if( CreatFileCurveData( Point, FileCurvePara, FinalRecNo ) == -1 ) return ErrorByte;					//捕获项参数非法
	Value = LoadFilePtrChk( FileCurvePara, -1 );

	CreatFileName( FileName, ChannelNo>>8, FileCurvePara->OAD, 0 );
	ErrorByte = f_open( &fsrc, (char const*)FileName, FA_WRITE);
    if( ErrorByte == FR_OK )
    { 
		ErrorByte = f_lseek (&fsrc, Value);
		if( ErrorByte == FR_OK )
		{
    		ErrorByte = f_write(&fsrc, Point, FileCurvePara->RecdLenth, (UINT*)&WriteLen);     
    	}	      	
		ErrorByte = f_close(&fsrc);      		
    }
    if( FileCurvePara->BakFlag == 1 )
	{
		CreatFileName( FileName, ChannelNo>>8, FileCurvePara->OAD, 1 );
		ErrorByte = f_open( &fsrc, (char const*)FileName, FA_WRITE);
	    if( ErrorByte == FR_OK )
	    { 
			ErrorByte = f_lseek (&fsrc, Value);
			if( ErrorByte == FR_OK )
			{
	    		ErrorByte = f_write(&fsrc, Point, FileCurvePara->RecdLenth, (UINT*)&WriteLen);     
	    	}	      	
			ErrorByte = f_close(&fsrc);      		
	    }		
	}	    
    
    return 0;
//	IncSubsLoadPtr( CurvePara );
}
*/

//判断最近记录时间是否大于当前冻结时间，大于则不冻结，以免冻结时间顺序混乱		//17.03.29
short JudgeCurrentRecFreezeTime( FILECURVEPARA* FileCurvePara, unsigned char* FreezeTime )
{
	unsigned char Buff[20];
	unsigned char Buff2[20];
	unsigned char Buff3[10];
	unsigned char* Point;
	unsigned char* FileName;
	unsigned char ErrorByte=1;
	long Value;
	long Addr;
	short ItemNo;

	Point = Buff;
	FileName = Buff2;

	ItemNo = FileCurvePara->ChannelNo; 
	ItemNo >>= 8;
	if(( ItemNo < 2 )||( ItemNo > 4 )) return 0;		//只有分钟冻结、小时冻结、日冻结要判最近一条的时间
	Value = LoadFilePtrChk( FileCurvePara, 0 );
	if( Value == -1 ) return 0;							//还未有历史记录
	CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );
	ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
    if( ErrorByte == FR_OK )
    { 
		Addr = GetFileRecRealFlashAddr( FileCurvePara, Value );
	}
	else return 0;
	Read_Flash( (unsigned char*)Buff3, Addr, 7 ); 
	if(( Buff3[0] > 60 )||( Buff3[1] > 60 )||( Buff3[2] > 24 )||( Buff3[3] > 31 )||( Buff3[4] > 12 )) return 0;		//上次时间乱码，继续保存
	RAM_Write( Point, FreezeTime, 7 );
	SwapData( Point, 7 );						//时间低位在前
	if( Data_Comp( (unsigned char*)Buff3, Point, 7 ) > 0 ) return -1;
	
	return 0;
}	

//Type: 0:直接冻结；1：按冻结周期冻结
unsigned char RecordSingleFreeze( short ChannelNo, unsigned long FinalRecNo, unsigned char* FreezeTime, short Type )
{
	unsigned char Buff2[20];
	unsigned char Buff[500];
	unsigned char* Point;
	unsigned char* FileName;
    unsigned long LongBuff[8];
	FILECURVEPARA* FileCurvePara;
	unsigned char ErrorByte=1;
	long Value;
	long Addr;
	
	Point = Buff;
	FileName = Buff2;
	FileCurvePara = (FILECURVEPARA*)LongBuff;
	
	if( AllowFreeze( ChannelNo, Type ) != 0 ) return ErrorByte;
	
	FileCurvePara->ChannelNo = ChannelNo;
	GetFileCurvePara( FileCurvePara );

	if( JudgeCurrentRecFreezeTime( FileCurvePara, FreezeTime ) != 0 ) return ErrorByte; 	//记录最新时间大于当前时间	//17.03.29

	if( CreatFileCurveData( Point, FileCurvePara, FinalRecNo, FreezeTime ) == -1 ) return ErrorByte;					//捕获项参数非法
	Value = LoadFilePtrChk( FileCurvePara, -1 );

	CreatFileName( FileName, ChannelNo>>8, FileCurvePara->OAD, 0 );
//	ErrorByte = Open_File( FileCurvePara, FileName );
	ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
    if( ErrorByte == FR_OK )
    { 
		Addr = GetFileRecRealFlashAddr( FileCurvePara, Value );
		DataFlash_Write( Addr, Point, FileCurvePara->RecdLenth );
	}
    if( FileCurvePara->BakFlag == 1 )
	{
		CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 1 );	//打开备份文件
//		ErrorByte = Open_File( FileCurvePara, FileName );
		ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
    	if( ErrorByte == FR_OK )
	    { 
			Addr = GetFileRecRealFlashAddr( FileCurvePara, Value );
			DataFlash_Write( Addr, Point, FileCurvePara->RecdLenth );
    	}
		CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );	//恢复打开原始文件
//		ErrorByte = Open_File( FileCurvePara, FileName );								//后续不再操作，可不打开
	}
    
    return 0;
//	IncSubsLoadPtr( CurvePara );
}


//Type: 0:直接冻结；1：按冻结周期冻结
void RecordFreeze( short ItemNo, short Type )
{
	unsigned char Buff[8];
	unsigned char* FreezeTime;
//	unsigned char Buff[200];
//	unsigned char* Point;
//	unsigned char* FileName;
//   unsigned long LongBuff[8];
//  unsigned long WriteLen;
//	FILECURVEPARA* FileCurvePara;
	unsigned long FinalRecNo;
	unsigned short FinalRecNoAddr;
	unsigned short Items;
	unsigned char Flag=0;
//	long Value;
	short i;
	short ChannelNo;
	
	FreezeTime = Buff;
	
	FinalRecNoAddr = DL698_Freeze_ProfileTab[ItemNo].FinalRecNoAddr;
	E2P_RData( (unsigned char*)&FinalRecNo, FinalRecNoAddr, 4 );
	FinalRecNo += 1;
	Items = GetFreezeCaptureItems( ItemNo );
	RAM_Write( FreezeTime, &Clk.YearH, 7 );
	GetDirTo_FBuff();							//将文件目录表放入FBuff,加速后续文件打开操作	//17.03.08
	for( i=0;i<Items;i++ )
	{
		ChannelNo = ( ItemNo & 0xFF ) << 8;
		ChannelNo += i & 0xff;
		if( RecordSingleFreeze( ChannelNo, FinalRecNo, FreezeTime, Type ) == 0 )
		{
			Flag = 1;		 		//至少冻结过一条
		}	
	}	
	if( Flag == 1 )
	{
		E2P_WData( FinalRecNoAddr, (unsigned char*)&FinalRecNo, 4 );
		if( ItemNo == MinFreeze_No ) ResetFreezeMD( ChannelNo );			//非分钟冻结无冻结需量
	}	
}	

short CreatFileCurveData( unsigned char* Dest, FILECURVEPARA* FileCurvePara, unsigned long FinalRecNo, unsigned char* FreezeTime )
{
//	unsigned char Buff[12];
	unsigned char OAD[4];
//	unsigned char* Ptr; 
	unsigned char* Point; 
	unsigned short ClassId;
	short Len;
//	unsigned short Length;
	unsigned short ItemNo;
	
//	Ptr = Buff;
	Point = Dest;
	
//	RAM_Write( Dest, &Clk.YearH, 7 );
	RAM_Write( Dest, FreezeTime, 7 );
	SwapData( Dest, 7 );						//时间低位在前
	ItemNo = ( FileCurvePara->ChannelNo & 0xFF ) >> 8;
	if(( ItemNo >= 0x0002 )&&( ItemNo <= 0x0007 )) *Dest = 0;		//从分钟冻结到年冻结，秒置0，方便搜索，以免同一记录中不同OAD时间不一致	
	
	Point = Dest+7;
	if( FileCurvePara->BakFlag == 1 ) 
	{
		*(Dest+7) = ChkNum( Dest, 7 );
		Point += 1;
	}
	RAM_Write( Point, (unsigned char*)&FinalRecNo, 4 );
	if( FileCurvePara->BakFlag == 1 ) 
	{
		*(Point+4) = ChkNum( Point, 4 );
		Point += 5;
	}
	else Point += 4;

	RAM_Write( OAD, FileCurvePara->OAD, 4 );
	
	Len = 0;

	OAD[2] &= 0x1F;											//屏蔽属性特征
	ClassId = GetOADClassId( OAD );
	
	switch( ClassId )
	{
		case Class_id_EC:
//			Len = GetECClassData( Point, OAD ); 
			Len = GetECClassData( 0xFF, Point, OAD ); 	//17.04.06
			break;					
		case Class_id_MD:					
//			Len = GetMDClassData( Point, OAD ); 
			Len = GetMDClassData( 0xFF, Point, OAD ); 	//17.04.06	
			break;				
		case Class_id_PhaseVar:				
//			Len = GetClass3Data( Point, OAD ); 
			Len = GetClass3Data( 0xFF, Point, OAD ); 		//17.04.06
			break;				
		case Class_id_Power:					
//			Len = GetClass4Data( Point, OAD ); 
			Len = GetClass4Data( 0xFF, Point, OAD ); 		//17.04.06
			break;				
		case Class_id_DataVar:				
//			Len = GetClass6Data( Point, OAD ); 
			Len = GetClass6Data( 0xFF, Point, OAD ); 		//17.04.06
			break;				
//		case Class_id_Parameter:				
//			Len = GetParameterClassData( Point, OAD ); 
			break;				
		default: 
			Len = 6;
               *(Point+5) = object_undefined;
			break;			
	}
	if(( Len == 6 )&&( *(Point+5) == object_undefined ))	//无对应的OBIS码
	{
		return -1;			
	}
				
	RAM_Write( Point, Point+5, Len );						//返回数据包含OAD
	Len -= 5;

	if( FileCurvePara->BakFlag == 1 ) 						//17.03.08
	{
		*(Point+Len) = ChkNum( Point, Len );
	}
	
	if( Len > (200 - 20 ) ) return -1; 

		
   return Len;	
}	

//=====================================================================================================
/*
void CreateComplementDayFreezeRec( unsigned char* DataTime, unsigned char MDState )		//补充日冻结数据		//新国网
{
	unsigned char Buff[10];
	unsigned char* Ptr;	
	long LastPt;
		
	Ptr = Buff;

	LastPt = PtAddrChk( DayFreeze_No, Current );				//定位新一次冻结记录首址	//14.11.27			//15.06.19				
	if( LastPt != -1 ) 																		//14.11.27			//15.06.19				
	{
		Read_Flash( Ptr, LastPt, 5 );														//14.11.27
		if( Data_Comp( Ptr, DataTime, 5 ) == 0 ) return;	//若最近一次日冻结时间同要补的日冻结时间相同，则不补日冻结数据	//14.11.27
	}	

	LastPt = PtAddrChk( DayFreeze_No, Next );				//定位新一次冻结记录首址	
	if( LastPt == -1 ) return;	
	RAM_Write( Ptr, DataTime, 5 );
	DataFlash_Write( LastPt, Ptr, 5 );
	ComplementFreezeECDataRecord( LastPt+5 );	
	ComplementFreezeMDDataRecord( LastPt+5+CMon_MD_Pp0, MDState );
	ComplementFreezeRealDataRecord( LastPt+5+CMon_MD_SnC0+9 );

}	

//取虚拟第一结算日结算时间：年月日时
short GetDummyTime_AdjustMon( unsigned char* SaveTime, unsigned char* CTime )						//新国网
{
	unsigned char Buff[8];
	unsigned char Buff1[4];
	unsigned char Buff2[4];
	unsigned char* LastTime;
	unsigned char* TestTime;
	unsigned char* CurTime;
	unsigned char* SixSaveDay;	
	unsigned short i;
	
	LastTime = Buff1;
	CurTime = Buff2;	
	
	SixSaveDay = Buff;
	
	E2P_RData( LastTime, LastSaveTime, 4 );							//上次结算时间
	RAM_Write( CurTime, CTime, 4 );									//当前时间
	
	if( DateTime4ValueCheck( CurTime ) != 0 ) return 0;				//当前时间非法，不结算。		
	
	if(( Data_Comp( LastTime, CurTime, 4 ) == 1 )||( DateTime4ValueCheck( LastTime ) != 0 ))  //当前时间若小于上次结算时间或上次结算时间非法，则不结算
	{
		return 0;				
	}	

	RAM_Write( SixSaveDay+2, LastTime+2, 2 );						//上次结算时间的年月放入测试区
	RAM_Write( SixSaveDay+6, LastTime+2, 2 );						//上次结算时间的年月放入测试区
	E2P_RData( SixSaveDay, ESave_Day1, 2 );							//第一结算日时放入测试区
	RAM_Write( SixSaveDay+4, SixSaveDay, 2 );
	IncYearMonth( SixSaveDay+6 );									//第二测试区月份加一

	TestTime = SixSaveDay;
	for( i=0;i<2;i++ )
	{
		if( DateTime4ValueCheck( TestTime ) != 0 ) 					//测试区内的时间非法
		{															//跳过
			TestTime += 4;
			continue;	
		}	
		if( Data_Comp( TestTime, LastTime, 4 ) == 1 )				//测试区内的时间大于上次结算时间
		{
			if( Data_Comp( CurTime, TestTime, 4 ) == -1 ) break;	//当前时间若小于测试时间，则不进行结算
			else 
			{
				RAM_Write( SaveTime, TestTime, 4 ); 				//以测试时间作为结算点时间返回
				return 1;											//返回1：有结算
			}	
		}	
		TestTime += 4;
	}	
	return 0;	
}

void ComplementDayFreeze( unsigned char* STime, unsigned char* ETime )							//新国网
{
	unsigned char Buff[5];
	unsigned char Buff2[5];
	unsigned char* Ptr;
	unsigned char* SaveTime;
	unsigned short FreezeCnt;
	unsigned short i;
	unsigned short SaveTRUE=0;
	unsigned char MDState;
	
	Ptr = Buff;	
	SaveTime = Buff2;
	
	if(( DateTime5ValueCheck( STime ) != 0 )||( DateTime5ValueCheck( ETime ) != 0 )) return;		//时间非法不补冻结
	
	RAM_Fill( SaveTime, 5 );
	SaveTRUE = GetDummyTime_AdjustMon( SaveTime+1, ETime+1 );			//判断是否有第一结算日的结算，若有则返回结算时间	
	
	E2P_RData( Ptr, DayFreezeTime, 2 );
	
	FreezeCnt = 0;
	RAM_Write( Ptr+2, ETime+2, 3 );
	if( Data_Comp( Ptr, ETime, 5 ) > 0 )							//14.12.03
	{																//14.12.03	
		DayDec( Ptr+2 );											//14.12.03
	}																//14.12.03
	for( i=0;i<7;i++ )
	{
		if((( Data_Comp( Ptr, STime, 5 ) ) > 0 )&&(( Data_Comp( Ptr, ETime, 5 ) ) <= 0 ))
		{
			FreezeCnt += 1;
			DayDec( Ptr+2 );
		}else break;	
	}	

	while( FreezeCnt > 0 )
	{
		FreezeCnt--;
		DayInc( Ptr+2 );
		if( SaveTRUE != 0 )
		{
			if( Data_Comp( Ptr, SaveTime, 5 ) > 0 ) MDState = 1;	//当前补冻结时间已经跨过第一结算日，所补的需量必须清零。
			else MDState = 0;											//当前补冻结时间未跨过第一结算日，所补的需量为当前需量。
		}
		else MDState = 0;												//补冻结期间不跨过第一结算日，所补的需量为当前需量。
		CreateComplementDayFreezeRec( Ptr, MDState );
	}	
}
*/

#if( JiangSuRequire == YesCheck )			//江苏要求
//读取单个冻结曲线的最近一次冻结时间
short Get_single_freeze_time( unsigned char* DestPtr, unsigned short ItemNo, unsigned short CurveNo, long LoadRecNo )
{
	unsigned char Buff2[20];
	unsigned char* FileName;
    unsigned long LongBuff[8];
	FILECURVEPARA* FileCurvePara;
	long LoadAddr;
	long Offset;							//16.10.12
    unsigned char ErrorByte;
	
	FileName = Buff2;
	FileCurvePara = (FILECURVEPARA*)LongBuff;
	
	FileCurvePara->ChannelNo = ItemNo;
	FileCurvePara->ChannelNo <<= 8;
	FileCurvePara->ChannelNo += CurveNo;
	GetFileCurvePara( FileCurvePara );

	if( FileCurvePara->AvailRecs == 0 ) return -1;							//没有有效记录,返回空数据。	
	
	Offset = 0;							//16.10.12
	CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );
//	ErrorByte = Open_File( FileCurvePara, FileName );						//此处打开文件后，在Read_File中不再每次打开，避免多次打开，除非读取备用文件后恢复打开
	GetDirTo_FBuff();							//将文件目录表放入FBuff,加速后续文件打开操作	//17.03.08
	ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
	if( ErrorByte == FR_OK )
	{
		LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
		if( LoadAddr != -1 )
		{			
			Read_File( DestPtr, FileCurvePara, LoadAddr, Offset, 7 );
		}
		else return -1;
	}	
	else return -1;
	return 0;								
}

//取分钟冻结中正向有功总精确电量的冻结间隔和最近一次冻结时间
//0:正常返回，非0：异常(可能为无正向有功总精确电量项或冻结时间不合法）
short GetMinFreezeTimeSpace( short ItemNo, unsigned short* Space, unsigned char* FreezeTime )
{
	unsigned char Buff[10];
	unsigned char* Ptr;
//	unsigned short ItemNo; 
//	unsigned short CurveNo;
	unsigned short Addr;
//	unsigned short Space;
	unsigned short Items;
	unsigned long ComID;
	short i;

	Ptr = Buff;
	
	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;
	for( i=0;i<Items;i++ )
	{
		E2P_RData( Ptr, Addr, 9 );
		ComID = SwapCharToLong( Ptr+2 );	
		if( ComID == 0x00100401 )						//正向有功精确电量
		{
			RAM_Write( (unsigned char*)Space, Ptr, 2 );	//冻结周期	
			break;
		}	
		Addr += 10;
	}	
	if( i == Items ) return -1;							//无正向有功总精确电量，不补冻
	
	Get_single_freeze_time( FreezeTime, ItemNo, i, 0 );	//读最近一次冻结时间
	ChangeTimeHexToBCD( Ptr, FreezeTime );
	if( DateTime6ValueCheck( Ptr ) != 0 ) return -1;	//最近一次冻结时间非法，不补
	
	return 0;
}

//取当前时间前的一次应该冻结时间
void GetSecondTime( unsigned char* Dest, unsigned char* Source, unsigned short Space )
{
	unsigned char Temp1,Temp2;
	
	RAM_Write( Dest, Source, 6 );	
	Temp1 = 0;											//从0分钟起计算	
	Temp2 = BCD_Byte( *(Dest+1) );						
	if( *Dest != 0 ) Temp2 += 1;						//若秒不为0，则分钟数加1
	*Dest = 0;											//秒清零	
	while( Temp1 < Temp2 )								//未超过当前分钟数	
	{
		Temp1 += Space;									//增加冻结间隔
	}	
	if( Temp1 > Space ) Temp1 -= Space;					//超过后退回前一次冻结时间
	*(Dest+1) = Byte_BCD( Temp1 );	
}	

//判断是否需要补精确电量的分钟冻结
short JudgeComplementMinFreeze( unsigned char* FirstTime, unsigned char* SecondTime )		//补充分钟冻结数据		
{
	unsigned char PowerdownTime[7];	
	unsigned char FreezeTime[7];	
	unsigned char CurTime[7];	
	unsigned short Space;
	
	unsigned char* PWTime;
	
	PWTime = PowerdownTime;
	 
	E2P_RFM( PWTime, FPdownTime, 6 );	
	if( DateTime6ValueCheck( PWTime ) != 0 ) return -1;					//掉电时间非法，不补
	RAM_Write( CurTime, Clk.Sec, 6 );	
	if( DateTime6ValueCheck( CurTime ) != 0 ) return -1;				//当前时间非法，不补
	
	if( GetMinFreezeTimeSpace( MinFreeze_No, &Space, (unsigned char*)FreezeTime ) != 0 ) return -1;
	if(( Space > 60 )||( Space == 0 )) return -1;											//只支持小于60
	GetSecondTime( SecondTime, CurTime, Space );
	ChangeTimeBCDToHex( SecondTime, SecondTime );
	Datetimes_Add_TI( FirstTime, FreezeTime, 0x01, Space );
	ChangeTimeBCDToHex( CurTime, CurTime );
	if( Data_Comp( FirstTime, CurTime, 7 ) > 0 ) return -1;				//第一补偿点大于当前时间，不补偿
	if( Data_Comp( FirstTime, SecondTime, 7 ) >= 0 ) RAM_Fill( SecondTime, 7);		//第一补偿点大于等于第二点补冻结当前时间，第二点补冻结点不补冻

    return 0;
}	

void ComplementRecordMinFreeze( void )
{
	unsigned char Buff[8];
	unsigned char Buff2[10];
	unsigned char FirstTime[8];
	unsigned char SecondTime[8];
	unsigned char* FreezeTime;
	unsigned char* Ptr;
	unsigned long FinalRecNo;
	unsigned long ComID;
	unsigned short FinalRecNoAddr;
	unsigned short Items;
	unsigned short Addr;
	unsigned char Flag=0;
	short i,j;
	short ChannelNo;
	
	FreezeTime = Buff;
	Ptr = Buff2;
	
	if( JudgeComplementMinFreeze( FirstTime, SecondTime ) != 0 ) return;
	FinalRecNoAddr = DL698_Freeze_ProfileTab[MinFreeze_No].FinalRecNoAddr;
	E2P_RData( (unsigned char*)&FinalRecNo, FinalRecNoAddr, 4 );
	RAM_Write( FreezeTime, FirstTime, 7 );
	for( j=0;j<2;j++ )
	{
		SwapData( FreezeTime, 7 );
		FinalRecNo += 1;
		Items = GetFreezeCaptureItems( MinFreeze_No );
		Addr = DL698_Freeze_ProfileTab[MinFreeze_No].CapObjAddr;
		for( i=0;i<Items;i++ )
		{
			E2P_RData( Ptr, Addr, 9 );
			ComID = SwapCharToLong( Ptr+2 );
			if(( ComID == 0x00100401 )||( ComID == 0x00200401 )||( ComID == 0x00500401 )||( ComID == 0x00600401 )||( ComID == 0x00700401 )||( ComID == 0x00800401 )||( ComID == 0x11100201 )||( ComID == 0x11200201 )||( ComID == 0x11100200 )||( ComID == 0x11200200 ))
			{
				ChannelNo = ( MinFreeze_No & 0xFF ) << 8;
				ChannelNo += i & 0xff;
				if( RecordSingleFreeze( ChannelNo, FinalRecNo, FreezeTime, 0 ) == 0 )
				{
					Flag = 1;		 		//至少冻结过一条			
				}
			}		
			Addr += 10;
		}	
		if( Flag == 1 )	
		{
			E2P_WData( FinalRecNoAddr, (unsigned char*)&FinalRecNo, 4 );
			if( j == 0 ) ResetFreezeMD( ChannelNo );			//补分钟冻结冻结需量后清零
		}
		if( IsAllData( SecondTime, 7, 0x00 ) == 0 ) break;
		RAM_Write( FreezeTime, SecondTime, 7 );
	}	
}	
#endif