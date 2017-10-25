#include "TypeE2p.h"
#include "TypeRAM.h"
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
#include "cpucard.h"
#include "RemoteComm.h"

#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif

unsigned short GetDisplayTabItemNo( unsigned char* OAD )
{
	short i;

	for( i=0;i<DL698_Display_ParaSum;i++ )							//显示表格			
	{
		if( Data_Comp( OAD, (unsigned char*)DL698_Display_ParaTab[i].OAD, 2 ) == 0 ) break;				//查找对应的OAD码
	}	

	return i;	
}

//读显示类数据，返回值：-1，无此数据项，非-1：返回数据长度
//short GetDisplayDataTab( unsigned char* Dest, unsigned char* OAD )
short GetDisplayDataTab( short COM_No, unsigned char* Dest, unsigned char* OAD )		//17.04.06
{
	unsigned char Buff[12];
	unsigned char* Ptr;
	short i,j;
	unsigned short Addr;
	short Len;
//	unsigned short OICode;
//	short Temp;
	short Start,End;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
	unsigned char Num;
	unsigned char* Point;
	unsigned short ItemLen;
	
	Len = -1;
	
	Point = Buff;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	Index = *(OAD+3);
	
	i = GetDisplayTabItemNo( OAD );
	if( i != DL698_Display_ParaSum )
	{
		if(( Attribute >= 1 )&&( Attribute <= 4 ))			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_Display_ParaTab[i].SaftInfo, read_authority ) != 0 ) return -2;	//17.04.09
		}
		*Dest = Get_data;								//正常数据返回	
		switch( Attribute )
		{
			case 1:									//logical_name
				*(Dest+1) = D_OctetStr;
				*(Dest+2) = 0x02;
				RAM_Write( Dest+3, OAD, 2 );
				Len = 5;
				break;	
			case 2:									//value
				Addr = DL698_Display_ParaTab[i].CapNumAddr;
				E2P_RData( (unsigned char*)&Num, Addr, 1);	
				if( Index == 0 ) 					//总尖峰平谷
				{
					Start = 0;
					End = Num;
					*(Dest+1) = Data_array;
					*(Dest+2) = End;
					Ptr = Dest+3;
					Len = 3;
				}	
				else
				{
					Start = Index-1;
					End = Index;					
					Ptr = Dest+1;
					Len = 1;
				}	 
				Addr = DL698_Display_ParaTab[i].CapObjAddr;
				for( j=Start;j<End;j++ )
				{
					ItemLen = 0;
					E2P_RData( Point, Addr+j*11, 10);
					*Ptr = Data_structure;
					*(Ptr+1) = 0x02;
					*(Ptr+2) = D_CSD;
					if( *Point == 0 )
					{
						*(Ptr+3) = 0;								//OAD 
						RAM_Write( Ptr+4, Point+1, 4 );	
						ItemLen = 8;
					}
					else
					{
						*(Ptr+3) = 1;								//ROAD 
						RAM_Write( Ptr+4, Point+1, 4 );
						*(Ptr+8) = 1;								//SEQUENCE OF OAD 
						RAM_Write( Ptr+9, Point+5, 4 );
						ItemLen = 13;				
					}
					*(Ptr+ItemLen) = D_Unsigned8;
					*(Ptr+ItemLen+1) = *(Point+9);
					ItemLen += 2;
					Ptr += ItemLen;
					Len += ItemLen;	
				}
				break;	
			case 3:									//显示时间
				*(Dest+1) = D_Unsigned16;
				E2P_RData( Dest+2, DL698_Display_ParaTab[i].PeriodAddr, 2);	
				SwapData( Dest+2, 2 );
				Len = 4;
				break;	
			case 4:									
				E2P_RData( (unsigned char*)&Num, DL698_Display_ParaTab[i].CapNumAddr, 1);
				if( Index == 0 ) 					
				{
					*(Dest+1) = Data_array;
//					*(Dest+2) = End;
					*(Dest+2) = 0x02;
					*(Dest+3) = D_Unsigned8;
					*(Dest+4) = Num;
					*(Dest+5) = D_Unsigned8;
					*(Dest+6) = 99;
					Len = 7;
				}	
				else if( Index == 1 ) 					
				{
					*(Dest+1) = D_Unsigned8;
					*(Dest+2) = Num;
					Len = 3;
				}
				else if( Index == 2 ) 	
				{
					*(Dest+1) = D_Unsigned8;
					*(Dest+2) = 99;
					Len = 3;
				}
				break;	
			default: break;			
		}	
	}
	return Len;	
}	

//取Display类数据
//short GetDisplayClassData( unsigned char* Dest, unsigned char* Source ) 
short GetDisplayClassData( short COM_No, unsigned char* Dest, unsigned char* Source ) 		//17.04.06
{
	short Len;

	RAM_Write( Dest, Source, 4 );

//	Len = GetDisplayDataTab( Dest+4, Source );
	Len = GetDisplayDataTab( COM_No, Dest+4, Source );				//17.04.06
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

//-------------------------------------------------------------------------------------------------------
unsigned char SetDisplayClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = SetDisplayDataTab( COM_No, Dest+4, Source, DataLen, ItemLen  );
	if( ErrorByte == 0 ) InitPara5();
	return ErrorByte;		

}		

//取PhaseEvent类数据
unsigned char SetDisplayDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ) 
{
	short i,j;
//	unsigned short Addr;
	unsigned char ErrorByte=object_class_inconsistent;
	short Len;
//	unsigned short OICode;
	short Temp;
        unsigned short Item;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
	unsigned char* WriteBufAds;
	unsigned char* Ptr;
	unsigned char* OAD;
	Len = -1;
	
	OAD = Source;
	WriteBufAds = Source+4;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	Index = *(OAD+3);
				
	i = GetDisplayTabItemNo( OAD );
	if( i != DL698_Display_ParaSum )
	{
		if(( Attribute >= 2 )&&( Attribute <= 4 ))			//17.04.09
		{
			if( Judge_operating_authority( COM_No, OAD, DL698_Display_ParaTab[i].SaftInfo, write_authority ) != 0 ) return read_write_denied;	//17.04.09
		}
		switch( Attribute )
		{
			case 1:										//logical_name
				break;	
			case 2:										//	
				if( Index == 0 )
				{
					ErrorByte = Judge_display_capture_objects( WriteBufAds, i, &DataLen, ItemLen );
					if( ErrorByte == 0 )
					{
						Item = *(WriteBufAds+1);								//元素个数
						Ptr = WriteBufAds+2;									//冻结周期的数据类型			
						for( j=0;j<Item;j++ )
						{
							Len = Set_display_capture_object( Ptr, i, j );
							Ptr += Len;
						}
					}
				}
				else
				{
					if( Index > DL698_Display_ParaTab[i].CapNumMax ) return other_reason;
					ErrorByte = Judge_display_capture_object( WriteBufAds, i, &DataLen, ItemLen );
					if( ErrorByte == 0 )
					{
						Set_display_capture_object( WriteBufAds, i, Index-1 );
					}
				}									
				break;	
			case 3:										//
				if( *WriteBufAds != D_Unsigned16 ) return other_reason;
				Temp = SwapCharToShort( WriteBufAds+1 );				
				if( Temp == 0 ) Temp = 5;
				E2P_WData( DL698_Display_ParaTab[i].PeriodAddr, (unsigned char*)&Temp, 2);
				*ItemLen += 3;
                ErrorByte = 0;
				break;	
			case 4:										//
				if( Index == 0 )
				{
					if( *WriteBufAds != Data_structure ) return other_reason;
					if( *(WriteBufAds+1) != 2 ) return other_reason;
					if( *(WriteBufAds+2) != D_Unsigned8 ) return other_reason;
					E2P_WData( DL698_Display_ParaTab[i].CapNumAddr, WriteBufAds+3, 1 );
                    ErrorByte = 0;
				}
				else if( Index == 1 )
				{
					if( *WriteBufAds != D_Unsigned8 ) return other_reason;
					E2P_WData( DL698_Display_ParaTab[i].CapNumAddr, WriteBufAds+1, 1 );					
                    ErrorByte = 0;
				}						
				break;	
			default:
				break;	
		}	
	}		
	return ErrorByte;
}	

short Set_display_capture_object( unsigned char* WriteBufAds, short ProfileNo, short Index )
{
	unsigned char Buff[12];
	unsigned char* Point;
	unsigned short Addr;
	short Len=0;

	Point = Buff;

	RAM_Fill( Point, 10);
	*Point = *(WriteBufAds+3);
	RAM_Write( Point+1, WriteBufAds+4, 4 );
	if( *Point == 0x01 ) 
	{
		RAM_Write( Point+5, WriteBufAds+9, 4 );
		*(Point+9) = *(WriteBufAds+14);	 
		Len = 15;
	}
	else	
	{
		*(Point+9) = *(WriteBufAds+9);	 
		Len = 10;
	}	
//	E2P_RData( (unsigned char* )&Addr, DL698_Display_ParaTab[ProfileNo].CapObjAddr, 2 );
	Addr = DL698_Display_ParaTab[ProfileNo].CapObjAddr;
    Addr += Index * 11;
	E2P_WData( Addr, Point, 10 );	

	return Len;
}	

unsigned char Judge_display_capture_object( unsigned char* Data, short ProfileNo, unsigned short* Length, unsigned short* ItemLen )
{
//	unsigned short CapItems;
//	short i;
	unsigned char* Ptr;
//	unsigned short Temp;
	unsigned short Len;
								
	Len = 0;
        Ptr = Data;
	if( *Ptr != Data_structure ) return other_reason;
	if( *(Ptr+1) != 2 ) return other_reason;
	if( *(Ptr+2) != D_CSD ) return other_reason;
	if( *(Ptr+3) == 0x00 ) Len = 8;							//OAD	
	else if( *(Ptr+3) != 0x01 ) return other_reason;		//ROAD
	else
	{
		if( *(Ptr+8) != 0x01 ) return other_reason;
		Len = 13;
	}
	if( *(Ptr+Len) != D_Unsigned8 ) return other_reason;
    Len += 2;
	*ItemLen += Len;	
	*Length += Len;	 
											
    return 0;
}

unsigned char Judge_display_capture_objects( unsigned char* Data, short ProfileNo, unsigned short* Length, unsigned short* ItemLen )
{
	unsigned char ErrorByte;
	unsigned short CapItems;
	short i;
	unsigned char* Ptr;
	unsigned short Temp;
    unsigned short Len;
					
	CapItems = DL698_Display_ParaTab[ProfileNo].CapNumMax;
					
	if( *Data != Data_array ) return other_reason;
	Temp = *(Data+1);	
	if( Temp > CapItems ) return other_reason;			//捕获对象数大于额定限数
	Ptr = Data+2;
    Len = *ItemLen;
	for( i=0;i<Temp;i++ )
	{
		ErrorByte = Judge_display_capture_object( Ptr, ProfileNo, Length, ItemLen );
        Ptr += *ItemLen - Len;
        Len = *ItemLen;        
		if( ErrorByte != 0 ) return ErrorByte; 
	}	
											
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char ActionDisplayClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=0;

	RAM_Write( Dest, Source, 4 );

	ErrorByte = ActionDisplayClassDataTab( COM_No, Dest+4, Source, DataLen, ItemLen, BackLen  );
	return ErrorByte;		

}	

unsigned char ActionDisplayClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ) 
{
	unsigned char ErrorByte=object_class_inconsistent;
	unsigned short Temp;
	unsigned char* OMD;
	unsigned char Method;

	OMD = Source;
	Method = *(OMD+2);
	
	if(( Method >= 3 )&&( Method <= 6 ))			//17.04.09
	{
		if( Judge_operating_authority( COM_No, OMD, 0x4110, active_authority ) != 0 ) return read_write_denied;	//17.04.09
	}
	switch( Method )
	{
		case 1:										//复位
			break;	
		case 2:										//执行	
			break;	
		case 3:										//下翻
		case 4:										//上翻
			if( Disk.TouchDisMode == AutoMode ) 		
			{												
				Disk.TouchDisMode = KeyMode;					
                Disk.ContentDisType = KeyType;
				Disk.FaultItemPtr = -1;						
				Disk.EventItemPtr = -1;							
				Disk.KeyItemPtr = -1;						
			}												
			Disk.FullDisDelay = 0;	
			if( Method == 3) TouchDis( KeyMode, 1 );//下翻
			else TouchDis( KeyMode, 0 );			//上翻
			SM.KeyDly = DISDELAY;
			if((Flag.Power & F_PwrUp) != 0) SM.BJGDly = DISDELAY;
//			SM.KbCount = 0;
			Disk.DisInv = DISDELAY;	
			Flag.Disk |= F_KeyInt;
	
			*ItemLen += 5;
			ErrorByte = 0;
			break;	
		case 5:										//显示查看
		case 6:										//全显
			if( Method == 6 )
			{
				if( *(Source+4) != D_Unsigned16 ) return ErrorByte;
				Temp = SwapCharToShort( Source+5 );	
				if( Temp > 255 ) Temp = 255;
				RAM_DataFill( Disk.CurDisObj.OAD, 4, 0xFE );
				*ItemLen += 3;
			}
			else
			{
				if( *(Source+4) != Data_structure ) return ErrorByte;
				if( *(Source+5) != 0x03 ) return ErrorByte;
				
				if( *(Source+6) != D_CSD ) return ErrorByte;
				if( *(Source+7) == 0x00 )
				{
					if( *(Source+12) != D_Unsigned8 ) return ErrorByte;
					Disk.CurDisObj.No = *(Source+13);
					if( *(Source+14) != D_Unsigned16 ) return ErrorByte;
					Temp = SwapCharToShort( Source+15 );	
					if( Temp > 255 ) Temp = 255;
					Disk.CurDisObj.Type = 0;
					RAM_Write( Disk.CurDisObj.OAD, Source+8, 4 );	
					*ItemLen += 12;
				}
				else
				{
					if( *(Source+7) != 0x01 ) return ErrorByte;
					if( *(Source+17) != D_Unsigned8 ) return ErrorByte;
					Disk.CurDisObj.No = *(Source+18);
					if( *(Source+19) != D_Unsigned16 ) return ErrorByte;
					Temp = SwapCharToShort( Source+20 );	
					if( Temp > 255 ) Temp = 255;
					Disk.CurDisObj.Type = 1;
					RAM_Write( Disk.CurDisObj.OAD, Source+8, 4 );	
					RAM_Write( Disk.CurDisObj.ROAD, Source+13, 4 );	
					*ItemLen += 17;
				}						
			}	

			Flag.Disk |= F_KeyInt;
			Disk.DisInv = Temp;
			Disk.FullDisDelay = 0;
			if((Flag.Power & F_PwrUp) != 0) SM.BJGDly = Temp;
			Disk.TouchDisMode = LookMode;

			*ItemLen += 5;
			ErrorByte = 0;
			break;
		default:
			break;	
	}	
	return ErrorByte;
}

//========================================================================================================================================
void DspNum( unsigned char *Addr, short Lenth, unsigned char *SegPtr )
{
	if( Lenth > 4) return;
	while( Lenth>0 )
	{
		*SegPtr-- |= NumSeg[*Addr & 0x0F];
 		*SegPtr-- |= NumSeg[((*Addr)>>4) & 0x0F];
//		*SegPtr++ |= NumSeg[*Addr & 0x0F];		
// 		*SegPtr++ |= NumSeg[((*Addr)>>4) & 0x0F];
		Addr++;
		Lenth--;
    }			
}

void ZeroDark( unsigned char *LCDSeg )
{
	short i,Temp;
	unsigned short Item;
//	unsigned long ComID;											//新国网	//13.10.21
	
//	RAM_Write( (unsigned char*)&ComID, Disk.CurDisID, 4 );			//新国网	//13.10.21
	Temp = 0;														//新国网	//13.10.23
	
	if(( *LCDSeg & 0x08 ) != 0 )
	{
		Temp = 1;
		*LCDSeg &= ~0x08;
	}	
	for( i=0;i<6;i++ )
	{
		if( *(LCDSeg+i) == DS_Chr_Neg )
		{
			Temp = 1;	
			*(LCDSeg+i) = 0;
		}	
	}	
	for( i=0;i<7;i++ )
	{
		if( i == 6 )
        {
          if(( *(LCDSeg+15) & 0x08 ) != 0 ) break;			
        }  
		else
		{
			if(( *(LCDSeg+i+1) & 0x08 ) != 0 ) break;			
		}	
	}	
//	if( i == 7 ) i = 7;				//无小数点
	Item = i;
	for( i=0;i<Item;i++ )
	{
//		if( Disk.RHighZeroFlag == 0x00 ) 			//高位零显示	//新国网	//14.01.13
		if(( Disk.RHighZeroFlag & 0x01 ) == 0x00 ) 	//高位零显示	//新国网	//14.01.13	//16.06.07
		{
			if( *(LCDSeg+i) != 0 ) break;	
		}
		else										//新国网	//14.01.13
		{											//新国网	//14.01.13	
			if(( *(LCDSeg+i) == DS_Chr_0 )||(*(LCDSeg+i) == 0 )) 
			{
				*(LCDSeg+i) = 0;
			}
			else break;
		}											//新国网	//14.01.13
	}	
	if( Temp == 1 )
	{
#if ( MinusSignDisplay == YesCheck )
		if( i == 0 ) *LCDSeg |= 0x08;	//带负号		
		else *(LCDSeg+i-1) = DS_Chr_Neg;		//带负号
#else
		if(( *(LCDSeg+21) & 0x03 ) == 0x03 )		//显示电量时若为负，需显示负号 2012年第一次招标要求
		{
			if( i == 0 ) *LCDSeg |= 0x08;			//带负号		
			else *(LCDSeg+i-1) = DS_Chr_Neg;		//带负号			
		}	 
//		else if(( Disk.CurDisID[0] == 0x07 )&&( Disk.CurDisID[1] == 0x00 )&&( Disk.CurDisID[2] == 0x80 )&&( Disk.CurDisID[3] == 0x02 ))
/*		else if( ComID == 0x02800007 )
		{
			*(LCDSeg+i-1) = DS_Chr_Neg;		//带负号	//温度	
		}
		else if( ComID == 0x0400040F )				//新国网	//13.10.21
		{
			if( i == 0 ) *LCDSeg |= 0x08;			//带负号		
			else *(LCDSeg+i-1) = DS_Chr_Neg;		//带负号			
		}
*/		else
		{	
			*(LCDSeg+20) |= 0x48;			//"反向"
		}	
#endif				
	}
}

void ActiveDisp(  unsigned char *LCDSeg, unsigned char *FLCDSeg )
{	
	unsigned char Temp2;
	unsigned char Buff[10];
	unsigned char* Point;
//	short i;      //新国网    //13.10.15
		
	Point = Buff;

	RAM_Fill( FLCDSeg, 32 );
		
	Temp2 = 0;

#if ( MEASCHIP == ADE7758 )	  
#elif ( MEASCHIP == ADE7878 )	
#elif ( MEASCHIP == IDT90E32 )							//12.08.29	
#else
	if((Flag.Power & F_PwrUp) != 0)		
	{	
		ATT7022RdReg( ATVoltFlag, Point );
		Temp2 = *Point & 0x80;	
	}
#endif		
	if((Flag.Power & F_PwrUp) != 0 )
	{
		if( SM.CalibCount >= CALIBCOUNT)  
		{
			*(LCDSeg + 16 ) |= 0xA0;		//校表用全象限表示
			*(LCDSeg + 17 ) |= 0x0C;		//校表用全象限表示		
		}	
		else		 
		{	
			switch( GetStablyQuad() )		
			{
				case 1: *(LCDSeg + 17 ) |= 0x08; 
                  Disk.LCDSeg[16] &= ~0xA0;
                  Disk.LCDSeg[17] &= ~0x04;
                  break;		
				case 2: *(LCDSeg + 16 ) |= 0x80; 
                  Disk.LCDSeg[16] &= ~0x20;
                  Disk.LCDSeg[17] &= ~0x0C;
                  break;
				case 3: *(LCDSeg + 16 ) |= 0x20; 
                  Disk.LCDSeg[16] &= ~0x80;
                  Disk.LCDSeg[17] &= ~0x0C;
                  break;
				case 4: *(LCDSeg + 17 ) |= 0x04; 
                  Disk.LCDSeg[16] &= ~0xA0;
                  Disk.LCDSeg[17] &= ~0x08;
                break;
				default:
					*(LCDSeg + 16 ) |= Disk.LCDSeg[16] & 0xA0;
					*(LCDSeg + 17 ) |= Disk.LCDSeg[17] & 0x0C;
					break;			
			}	
			if( Temp2 == 0 )
			{
				if(( Disk.LCDSeg[16] & 0xA0 ) == 0xA0) 
				{	
					Disk.LCDSeg[16] &= ~0xA0;
					Disk.LCDSeg[17] &= ~0x0C;
				}	
			}
			else *(LCDSeg + 17 ) |= 0x08;	
		}
		
		*(LCDSeg + 16 ) |= 0x40;  				//坐标常亮	
	}
	else
	{
		*(LCDSeg + 17 ) &= ~( 0x08 + 0x04 );
		*(LCDSeg + 16 ) &= ~( 0x80 + 0x20 );
		*(LCDSeg + 16 ) &= ~0x40;  				//坐标		
	}	

	if((Flag.Power & F_PwrUp) != 0 )
	{
#if( LCD8259 ==	YesCheck )
#if ( LocalSmartMeter == YesCheck )
		if(( Para.RFeeSum != 0x00 )&&( Para.RFeeMode != 0x01 ))		//12.12.22     费率为零不显示费率号     //10.07.29		//单费率不显示费率号
#else
		if( Para.RFeeMode != 0x01 )					//10.07.29		//单费率不显示费率号
#endif
		{
		switch( Disk.DisFeeNo )
		{
			case 1: *(LCDSeg + 16 ) |= 0x08; 
				Disk.LCDSeg[16] &= ~0x14;  
				Disk.LCDSeg[25] &= ~0x01;  
				break;
			case 2: *(LCDSeg + 16 ) |= 0x04; 
				Disk.LCDSeg[16] &= ~0x18;  
				Disk.LCDSeg[25] &= ~0x01;  
				break;
			case 3: *(LCDSeg + 25 ) |= 0x01; 
				Disk.LCDSeg[16] &= ~0x1C;  
				break;
			case 4: *(LCDSeg + 16 ) |= 0x10; 
				Disk.LCDSeg[16] &= ~0x0C;  
				Disk.LCDSeg[25] &= ~0x01;  
				break;
			default:break;	
		}	
		}
#else	
		switch( Disk.DisFeeNo )
		{
			case 1: *(LCDSeg + 16 ) |= 0x02; 
				Disk.LCDSeg[16] &= ~0x0d;  
				break;
			case 2: *(LCDSeg + 16 ) |= 0x01; 
				Disk.LCDSeg[16] &= ~0x0e;  
				break;
			case 3: *(LCDSeg + 16 ) |= 0x08; 
				Disk.LCDSeg[16] &= ~0x07;  
				break;
			case 4: *(LCDSeg + 16 ) |= 0x04; 
				Disk.LCDSeg[16] &= ~0x0b;  
				break;
			default:break;	
		}	
#endif
	}	
/*
	if(((Flag.Power & F_PwrUp) == 0)&&(Comm.BatOnDly == 0))
	{
		switch( Disk.DisFeeNo )
		{
			case 1: *(FLCDSeg + 16 ) |= 0x02; break;
			case 2: *(FLCDSeg + 16 ) |= 0x01; break;
			case 3: *(FLCDSeg + 16 ) |= 0x08; break;
			case 4: *(FLCDSeg + 16 ) |= 0x04; break;
			default:break;	
		}	
	}	
*/	
	if(( Flag.Power & F_PwrUp ) == 0 )
	{
		if(Comm.BatOnDly != 0) *(LCDSeg + 17 ) |= 0x08; 				//停抄唤醒标志
		else Disk.LCDSeg[17] &= ~0x08;
	}	
	
	
	if( (Flag.Power & F_PwrUp) != 0)
	{
//		if( VoltRevPhase() == 1 )
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		if(( EVar.VoltRevPhaseVar.OldState != 0 )||(( EVar.CuRevPhaseVar.OldState != 0 )&&( Para.RCuRevPhaseDisFlag != 0x55 )))		//14.09.03
#else
		if(( EVar.VoltRevPhaseVar.OldState != 0 )||( EVar.CuRevPhaseVar.OldState != 0 ))
#endif
		{
			*(FLCDSeg + 30 ) |= 0x01;	//逆相序闪
		}
		else Disk.LCDSeg[30] &= ~0x01;
		RAM_Fill( Point, 10 );	

		if( EVar.PhAFailVar.OldState != 0 ) *(FLCDSeg + 30 ) |= 0x10;	//Ua闪烁
#if( LinkMode == Phase3Wire4 )			//新国网
		if( EVar.PhBFailVar.OldState != 0 ) *(FLCDSeg + 30 ) |= 0x20;	//Ub闪烁
#endif
		if( EVar.PhCFailVar.OldState != 0 ) *(FLCDSeg + 30 ) |= 0x02;	//Uc闪烁

		if( EVar.PhABreakVar.OldState == 0 ) *(LCDSeg + 30 ) |= 0x10;	//Ua常亮
#if( LinkMode == Phase3Wire4 )			//新国网
		if( EVar.PhBBreakVar.OldState == 0 ) *(LCDSeg + 30 ) |= 0x20;	//Ub常亮
#endif
		if( EVar.PhCBreakVar.OldState == 0 ) *(LCDSeg + 30 ) |= 0x02;	//Uc常亮

		if( EVar.CuAFailVar.OldState != 0 ) *(FLCDSeg + 29 ) |= 0x10;	//Ia闪烁
#if( LinkMode == Phase3Wire4 )			//新国网
		if( EVar.CuBFailVar.OldState != 0 ) *(FLCDSeg + 29 ) |= 0x01;	//Ib闪烁
#endif
		if( EVar.CuCFailVar.OldState != 0 ) *(FLCDSeg + 28 ) |= 0x10;	//Ic闪烁

		if( EVar.AllFailVar.OldState != 0 )						//新国网	//13.10.12
		{														
			*(FLCDSeg + 30 ) |= 0x10;	//Ua闪烁
#if( LinkMode == Phase3Wire4 )			//新国网
			*(FLCDSeg + 30 ) |= 0x20;	//Ub闪烁
#endif
			*(FLCDSeg + 30 ) |= 0x02;	//Uc闪烁
		}														//新国网	//13.10.11	
			
	
		if( EVar.CuABreakVar.OldState == 0 ) *(LCDSeg + 29 ) |= 0x10;	//Ia常亮	//新国网	//13.10.12
		else Disk.LCDSeg[29] &= ~0x10;											//新国网	//13.10.12
#if( LinkMode == Phase3Wire4 )			//新国网
		if( EVar.CuBBreakVar.OldState == 0 ) *(LCDSeg + 29 ) |= 0x01;	//Ib常亮	//新国网	//13.10.12
		else Disk.LCDSeg[29] &= ~0x01;												//新国网	//13.10.12			
#endif
		if( EVar.CuCBreakVar.OldState == 0 ) *(LCDSeg + 28 ) |= 0x10;	//Ic常亮	//新国网	//13.10.12	
		else Disk.LCDSeg[28] &= ~0x10;												//新国网	//13.10.12
		
//		if( SM.PrgDly != 0 ) *(LCDSeg + 26 ) |= 0x10;				//编程按键

		if(( EVar.RevPowerAVar.OldState != 0 )&&( *(LCDSeg + 29 ) & 0x10 )) *(LCDSeg + 29 ) |= 0x20;	//"-Ia"	//新国网	//13.10.12
		else Disk.LCDSeg[29] &= ~0x20;																		//新国网	//13.10.12
#if( LinkMode == Phase3Wire4 )			//新国网
		if(( EVar.RevPowerBVar.OldState != 0 )&&( *(LCDSeg + 29 ) & 0x01 )) *(LCDSeg + 29 ) |= 0x02;	//"-Ib"	//新国网	//13.10.12
		else Disk.LCDSeg[29] &= ~0x02;																			//新国网	//13.10.12			
#endif
		if(( EVar.RevPowerCVar.OldState != 0 )&&( *(LCDSeg + 28 ) & 0x10 )) *(LCDSeg + 28 ) |= 0x20;	//"-Ic"	//新国网	//13.10.12	
		else Disk.LCDSeg[28] &= ~0x20;																			//新国网	//13.10.12
		
		if( EVar.PhABreakVar.OldState != 0 )					//新国网	//13.10.12
		{
			*(LCDSeg + 30 ) &= ~0x10;	//Ua不亮
			*(LCDSeg + 29 ) &= ~0x30;	//Ia、"-Ia"不亮	
		}	
#if( LinkMode == Phase3Wire4 )			//新国网
		if( EVar.PhBBreakVar.OldState != 0 )					//新国网	//13.10.12
		{
			*(LCDSeg + 30 ) &= ~0x20;	//Ub不亮
			*(LCDSeg + 29 ) &= ~0x03;	//Ib、"-Ib"不亮
		}	
#endif
		if( EVar.PhCBreakVar.OldState != 0 )					//新国网	//13.10.12
		{
			*(LCDSeg + 30 ) &= ~0x02;	//Uc不亮
			*(LCDSeg + 28 ) &= ~0x30;	//Ic、"-Ic"常亮
		}	

		if( EVar.PdownVar.OldState != 0 )						//新国网	//13.10.12
		{														//新国网	//13.10.12
			*(LCDSeg + 28 ) &= ~0x30;							//新国网	//13.10.12
			*(LCDSeg + 29 ) &= ~0x33;							//新国网	//13.10.12
			*(LCDSeg + 30 ) &= ~0x33;							//新国网	//13.10.12
			*(FLCDSeg + 28 ) &= ~0x30;							//新国网	//13.10.12
			*(FLCDSeg + 29 ) &= ~0x33;							//新国网	//13.10.12
			*(FLCDSeg + 30 ) &= ~0x33;							//新国网	//13.10.12
			Disk.LCDSeg[28] &= ~0x30;							//新国网	//13.10.12
			Disk.LCDSeg[29] &= ~0x33;							//新国网	//13.10.12	
			Disk.LCDSeg[30] &= ~0x33;							//新国网	//13.10.12	
		}														//新国网	//13.10.12		
	}
	else 
	{	
		Disk.LCDSeg[28] &= ~0x30;
		Disk.LCDSeg[29] &= ~0x33;
		Disk.LCDSeg[30] &= ~0x33;
	}
	
//	if( SM.PrgDly != 0 ) *(LCDSeg + 26 ) |= 0x10;				//编程按键		//新国网
	if( ICEC.InfraredValidDelay != 0 ) *(LCDSeg + 26 ) |= 0x10;	//红外认证有效	//新国网
	else Disk.LCDSeg[26] &= ~0x10;

	if( (Flag.MeterState1 & F_ClockBat ) != 0 ) *(LCDSeg + 27 ) |= 0x10;	// 时钟电池电压检测
	else Disk.LCDSeg[27] &= ~0x10;
	if( (Flag.MeterState1 & F_CommBat ) != 0 ) *(LCDSeg + 27 ) |= 0x20;	// 停抄电池电压检测
	else Disk.LCDSeg[27] &= ~0x20;
	
	if( Comm.RecTmr0 > 0 ) *(LCDSeg + 26 ) |= 0x04;			//"话机"
	else Disk.LCDSeg[26] &= ~0x04;
	if( Comm.RecTmr1 > 0 ) { *(LCDSeg + 26 ) |= 0x04; *(LCDSeg + 26 ) |= 0x02; }	//"话机1"
	else Disk.LCDSeg[26] &= ~0x02;
#if( CarryComm == YesCheck )							//带载波通讯
	if( Comm.RecTmr2 > 0 ) { *(LCDSeg + 26 ) |= 0x08; }								//"载波信号"
	else Disk.LCDSeg[26] &= ~0x08;
#else
	if( Comm.RecTmr2 > 0 ) { *(LCDSeg + 26 ) |= 0x04; *(LCDSeg + 26 ) |= 0x01; }	//"话机2"
	else Disk.LCDSeg[26] &= ~0x01;
#endif
//	if( Comm.RecTmr3 > 0 ) { *(LCDSeg + 26 ) |= 0x08; }								//"载波信号"
//	else Disk.LCDSeg[26] &= ~0x08;
	
	if(( Comm.RecTmr0 == 0 )&&( Comm.RecTmr1 == 0 )&&( Comm.RecTmr2 == 0 )) Disk.LCDSeg[26] &= ~0x04; //"话机"不显 
	
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
	if( ICEC.IdentityState == 0x55 ) *(LCDSeg + 26) |= 0x20;								//新国网	
#else
	if(( SM.SetCloseDelay2 != 0 )||( SM.SetCloseDelay4 != 0 )) *(LCDSeg + 26) |= 0x20;		//新国网	//14.06.20	
	else if( ICEC.IdentityState == 0x55 ) *(LCDSeg + 26) |= 0x20;							//新国网	//14.06.20	
#endif
#else
//	if(( SM.SetCloseDelay2 != 0 )||( SM.SetCloseDelay4 != 0 )) *(LCDSeg + 26) |= 0x20;		//新国网
	if( ICEC.IdentityState == 0x55 ) *(LCDSeg + 26) |= 0x20;								//新国网	
#endif
	else Disk.LCDSeg[26] &= ~0x20;
	
	if((Flag.Power & F_PwrUp) != 0 )		//停电后不显示
	{
#if ( LocalSmartMeter == YesCheck )
		if(( Para.RFeeSum != 0x00 )&&( Para.RFeeMode != 0x01 ))		//12.12.22     费率为零不显示费率号     //10.07.29		//单费率不显示费率号
#else
		if( Para.RFeeMode != 0x01 )					//10.07.29		//单费率不显示费率号
#endif
		{
			if( SM.SegTabNo == 0 ) 
			{
				*(LCDSeg + 28 ) |= 0x02;		//第1套时段表
				Disk.LCDSeg[28] &= ~0x01;
			}	
			else 
			{
				*(LCDSeg + 28 ) |= 0x01;		//第2套时段表
				Disk.LCDSeg[28] &= ~0x02;		
			}
		}
#if( LCD8259 ==	YesCheck )
											//停电后不显示，要显示全失压校表步骤
//		if( SM.StepTabNo == 0 ) 			//新国网	
//		{
//			*(LCDSeg + 16 ) |= 0x02;		//第1套阶梯电价
//			Disk.LCDSeg[16] &= ~0x01;
//		}
//		else
//		{
//			*(LCDSeg + 16 ) |= 0x01;		//第2套阶梯电价
//			Disk.LCDSeg[16] &= ~0x02;		
//		}		

		if( SM.StepNo == 0 ) 				//11.05.28
		{
			*(LCDSeg + 16 ) &= ~0x03;
			Disk.LCDSeg[16] &= ~0x03;		
		}	

		switch( SM.StepNo )					//当前阶梯号
		{
			case 1:
				*(LCDSeg + 17 ) |= 0x02;	//阶梯1
				Disk.LCDSeg[17] &= ~0x31;		
				break;	
			case 2:
				*(LCDSeg + 17 ) |= 0x01;	//阶梯2
				Disk.LCDSeg[17] &= ~0x32;				
				break;	
			case 3:
				*(LCDSeg + 17 ) |= 0x10;	//阶梯3
				Disk.LCDSeg[17] &= ~0x23;						
				break;	
			case 4:
				*(LCDSeg + 17 ) |= 0x20;	//阶梯4
				Disk.LCDSeg[17] &= ~0x13;								
				break;	
			default: 
				Disk.LCDSeg[17] &= ~0x33;	//超限								
				break;
		}		
#endif

	}		
	else
	{
		if(( SM.HWCAllFailState & 0xF0 ) == 0xE0 ) 									//10.10.06	
		{
			if(( SM.HWCAllFailState & 0x01 ) != 0 ) *(LCDSeg + 17 ) |= 0x02;		//10.10.06
			if(( SM.HWCAllFailState & 0x02 ) != 0 ) *(LCDSeg + 17 ) |= 0x01;		//10.10.06
			if(( SM.HWCAllFailState & 0x04 ) != 0 ) *(LCDSeg + 17 ) |= 0x10;		//10.10.06
			if(( SM.HWCAllFailState & 0x08 ) != 0 ) *(LCDSeg + 17 ) |= 0x20;		//10.10.06			
		}
		else Disk.LCDSeg[17] &= ~0x33;	//超限										//10.10.06			
	}			

#if ( GPRSComm == YesCheck	)
#if( LCD8259 ==	YesCheck )
	Disk.LCDSeg[25] &= ~0x76;	
	
	if((Flag.Power & F_PwrUp) != 0)			//停电后不显示
	{
		Temp2 = Para.NetIntensity;
//		if( Temp2 & 0x80 ) *(LCDSeg + 25 ) |= 0x04;				//"天线"
//		*(LCDSeg + 25 ) |= 0x04;				//"天线"
		if(( Temp2 & 0x07 ) != 0 ) *(LCDSeg + 25 ) |= 0x04;		//"天线"		//新国网
		Temp2 &= 0x07;	
		if( Temp2 >= 0x01 )
		{
			if( Temp2 < 0x02 )  
			{
				*(LCDSeg + 25 ) |= 0x02;						
			}
			else if( Temp2 < 0x3 )
			{
				*(LCDSeg + 25 ) |= 0x12;									
			}	  	
			else if( Temp2 < 0x4 )
			{
				*(LCDSeg + 25 ) |= 0x32;												
			}
			else *(LCDSeg + 25 ) |= 0x72;												
		}	
	}	
#else
	Disk.LCDSeg[25] &= ~0x73;	
	
	if((Flag.Power & F_PwrUp) != 0)			//停电后不显示
	{
		Temp2 = Para.NetIntensity;
//		if( Temp2 & 0x80 ) *(LCDSeg + 25 ) |= 0x02;				//"天线"
		*(LCDSeg + 25 ) |= 0x02;				//"天线"
		Temp2 &= 0x07;	
		if( Temp2 >= 0x01 )
		{
			if( Temp2 < 0x02 )  
			{
				*(LCDSeg + 25 ) |= 0x01;						
			}
			else if( Temp2 < 0x4 )
			{
				*(LCDSeg + 25 ) |= 0x11;									
			}	  	
			else if( Temp2 < 0x6 )
			{
				*(LCDSeg + 25 ) |= 0x31;												
			}
			else *(LCDSeg + 25 ) |= 0x71;												
		}
	}		
#endif
#endif

#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
	Disk.LCDSeg[28] &= ~0x08;												//新国网	//13.11.06
	Disk.LCDSeg[27] &= ~0x40;												//新国网	//13.11.06
#else																		//新国网	//13.11.06
//	if( Para.RMeterTripState == 0x1A ) *(LCDSeg + 28 ) |= 0x08;		//"拉闸"	
//	if( Para.RMeterRelayState == 0x1A ) *(LCDSeg + 28 ) |= 0x08;		//"拉闸"	
#if ( LocalSmartMeter == YesCheck )
	if( Para.RMeterRelayState == 0x1A ) *(LCDSeg + 28 ) |= 0x08;		//"拉闸"	
#else
	if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayONState != 0xAA )&&( ICComm.RelayONState != 0x55 )) *(LCDSeg + 28 ) |= 0x08;		//"拉闸"	
#endif	
//	else if(( Para.RMeterTripState == 0x1A )&&( Para.RTripOFFAlarmTime != 0 ))
	else if(( Para.RMeterTripState == 0x1A )&&( ICComm.TripOFFDelay != 0 ))
	{
		*(FLCDSeg + 28 ) |= 0x08;		//"拉闸"闪烁
	}	
#if ( IOver30ALimitTrip	== YesCheck )		//新国网	//14.04.25
	else if(( Para.RMeterRelayState != 0x1A )&&( ICComm.RelayOFFState == 0x55 ))
	{
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		if(( IOver30AJudge() != 0 )&&( SM.Up30ATimeCount < Para.RUp30AClose24HOUR ))	//新国网	//14.06.19	
#else
		if(( IOver30AJudge() != 0 )&&( SM.Up30ATimeCnt != UP30A24HOUR ))		
#endif
		{
			*(FLCDSeg + 28 ) |= 0x08;		//"拉闸"闪烁
		}				
	}	
#endif
	else Disk.LCDSeg[28] &= ~0x08;

#if ( LocalSmartMeter == YesCheck )
	if(((( Para.RPreTripState != 0x55 )&&( ICComm.RJudgePermitFlag == 0x00 ))||(( ICComm.RJudgePermitFlag == 0x55 )&&( ICEC.RRemainEC > ICEC.RPermitTripONLimt ))			//新国网	//14.01.14
		||(( Para.RPreTripState == 0x55 )&&(( ICComm.ICECState == F_ECNormal )||( ICComm.ICECState == F_ECAlarm1 )))					//新国网	//14.01.14
	  	||(Para.RMeterSafeState == 0x3A))&&(( ICComm.RelayONState == 0xAA )||( ICComm.RelayONState == 0x55 )))			//允许合闸且剩余金额大于允许值时取消拉闸提示		//新国网	//14.01.13
	{
		*(LCDSeg + 28 ) &= ~0x08;
		Disk.LCDSeg[28] &= ~0x08;
	}	

//	if(( Para.RMeterTripState == 0x1A )||( Para.RMeterTripState == 0xAA ))		//拉闸或保电
//	if(( Para.RMeterTripState == 0x1A )||( Para.RMeterAlarmState == 0x2A )		//拉闸或报警
	if(((Flag.Power & F_PwrUp) != 0)&&(( Para.RMeterAlarmState == 0x2A )		//报警				//新国网	//14.01.07
		||( ICComm.ICECState != F_ECNormal )))									//小于报警电量1
	{
		*(FLCDSeg + 27 ) |= 0x40;		//"请购电"			
	}	
	else Disk.LCDSeg[27] &= ~0x40;
#else
//	if(( Para.RMeterTripState == 0x1A )||( Para.RMeterTripState == 0xAA ))		//拉闸或保电
//	if(( Para.RMeterTripState == 0x1A )||( Para.RMeterAlarmState == 0x2A ))		//拉闸或报警
	if( Para.RMeterAlarmState == 0x2A )		//报警
	{
		*(FLCDSeg + 27 ) |= 0x40;		//"请购电"			
	}	
	else Disk.LCDSeg[27] &= ~0x40;
#endif	
#endif																		//新国网	//13.11.06

	if( ICComm.RPrivateKeyFlag != 0x00 ) *(LCDSeg + 26 ) |= 0x40;				//"公钥"		//2011.08.01
	else Disk.LCDSeg[26] &= ~0x40;												//2011.08.01

#if ( LocalSmartMeter == YesCheck )
	if( ICComm.ICRDStateDelay != 0 )
	{
//		if( ICComm.ICAlarmFlg != 0 ) *(LCDSeg + 27 ) |= 0x09;		//"读卡失败"
		if( ICComm.ICDisAlarmFlg != 0 ) *(LCDSeg + 27 ) |= 0x09;		//"读卡失败"	//新国网	//13.12.20
		else *(LCDSeg + 27 ) |= 0x0A;		//"读卡成功"
	}	 
	if( ICComm.ICDisState == ICDisCardReading ) 
	{
		*(LCDSeg + 27 ) |= 0x0C;		//"读卡中"
		*(LCDSeg + 27 ) &= ~0x03;		//不显示"成功"、"失败"			
		Disk.LCDSeg[27] &= ~0x03;		//不显示"成功"、"失败"			
	}	
	else Disk.LCDSeg[27] &= ~0x0C;

//	if(( ICComm.ICOutState & F_ECRelay ) != 0 ) *(LCDSeg + 28 ) |= 0x08;		//"跳闸"
	if(( ICComm.ICInState & F_RegECLimt ) != 0 ) *(LCDSeg + 28 ) |= 0x80;		//"囤积"
	else Disk.LCDSeg[28] &= ~0x80;
//	if(( ICComm.ICECState == F_ECOverdraft )||( ICComm.ICECState == F_ECOverEnd )) *(LCDSeg + 27 ) |= 0x80;		//透支		
	if( ICComm.ICECState == F_ECOverdraft )											//11.06.27
	{
		*(FLCDSeg + 27 ) |= 0x80;		//“透支”闪烁			
	}	
	else if(( ICComm.ICECState == F_ECOverEnd )||( ICComm.ICECState == F_ECExceed ))	//11.06.27	
	{
		if( Para.RMeterSafeState == 0x3A ) *(FLCDSeg + 27 ) |= 0x80;		//“透支”闪烁					
		else *(LCDSeg + 27 ) |= 0x80;		//“透支”常亮					
	}	
	else 
	{
//		if(( *(LCDSeg + 27 ) & 0x80 ) == 0 ) Disk.LCDSeg[27] &= ~0x80;	//当前不是显示透支电费		
//		if((Disk.CurDisID[3]==0x00)&&(Disk.CurDisID[2]==0x90)&&(Disk.CurDisID[1]==0x02)&&(Disk.CurDisID[0]==0x01)) ;	//11.04.25
//		else if(( *(LCDSeg + 27 ) & 0x80 ) == 0 ) Disk.LCDSeg[27] &= ~0x80;	//当前不是显示透支电费		
		if(( *(LCDSeg + 27 ) & 0x80 ) == 0 ) Disk.LCDSeg[27] &= ~0x80;		//15.07.22
	}	
#endif

	if((Flag.Power & F_PwrUp) != 0)			//新国网
	{
		RAM_Write( Point, Flag.AlarmFlg, 8 );
//		*(Point+3) &= 0x0f;								//远程报警、跳闸、继电器断开不进行屏幕报警提示
		*(Point+3) &= 0x2f;								//远程报警、跳闸、继电器断开不进行屏幕报警提示
//		if( JustState( Point, Para.RAlarm_Control2 ) != 0 ) *(FLCDSeg + 26 ) |= 0x80;	//警铃符号闪烁		
#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
		if( JustState( Point, Para.RAlarm_Control2 ) != 0 ) *(FLCDSeg + 26 ) |= 0x80;	//警铃符号闪烁		//新国网	//13.10.19
#else																		//新国网	//13.11.06
		if(( Para.RMeterAlarmState == 0x2A )||( JustState( Point, Para.RAlarm_Control2 ) != 0 )) *(FLCDSeg + 26 ) |= 0x80;	//警铃符号闪烁		//新国网	//13.10.19
#endif																		//新国网	//13.11.06
		else Disk.LCDSeg[26] &= ~0x80;															
	}

}

short TouchDis( unsigned char TouchMode, unsigned char Dir )
{
//	signed char* ItemPtr;
//	unsigned short DisFlag;
//	short State;
	short ReturnData=0;
	unsigned short Addr;

//	switch( Disk.ContentDisType )
//	{
//		case AutoType:
//			ItemPtr = &Disk.AutoItemPtr;
//			break;	
//		case KeyType:
//			ItemPtr = &Disk.KeyItemPtr;
//			break;	
//		default: break;									
//	}	

	switch(TouchMode)
	{
		case AutoMode:
			switch( Disk.ContentDisType )
			{
				case AutoType:
					Disk.AutoItemPtr++;
					if( Disk.AutoItemPtr >= Disk.AutoItemSum ) 
					{
						Disk.AutoItemPtr = 0; 
					}	
					Addr = Disk.AutoItemPtr & 0x7f;
//					E2P_RData( Disk.CurDisID, Dis_Item1 + Addr*6, 5 );		
					E2P_RData( (unsigned char*)&Disk.CurDisObj.Type, Alternate_display_capture_objects1 + Addr*11, 10 );		
					break;	
				case KeyType:
					Disk.ContentDisType = AutoType;
					TouchDis( TouchMode, 1 );
					break;	
				default: break;									
			}	
			break;
		case KeyMode:
			switch( Disk.ContentDisType )
			{
				case AutoType:
					Disk.ContentDisType = KeyType;
					TouchDis( TouchMode, Dir );
					break;
				case KeyType:
					if( Dir == 0 )						//上翻
					{
						Disk.KeyItemPtr--;
						if( Disk.KeyItemPtr == -2 ) Disk.KeyItemPtr = -1;
						if( Disk.KeyItemPtr == -1 ) 
						{
							Disk.KeyItemPtr = Disk.KeyItemSum - 1; 
						}	
					}
					else								//下翻
					{
						Disk.KeyItemPtr++;
						if( Disk.KeyItemPtr >= Disk.KeyItemSum ) 
						{
							Disk.KeyItemPtr = 0; 
						}	
					}		 
					Addr = Disk.KeyItemPtr & 0x7f;
//					E2P_RData( Disk.CurDisID, KeyDis_Item1 + Addr*6, 5 );		
					E2P_RData( (unsigned char*)&Disk.CurDisObj.Type, Service_display_capture_objects1 + Addr*11, 10 );		
					break;	
				default: break;									
			}	
			break;
		case LookMode:
			Disk.ContentDisType = AutoType;
			TouchDis( AutoMode, 1 );
			break;
		default: break;				
	}		
	return ReturnData;
}

//返回值 0：不刷新显示  1：刷新显示
short Display(void)
{
//	short 	Temp=0;
   	unsigned char LCDSeg[32], FLCDSeg[40];
//   	unsigned char* Point;
   	unsigned long DisID;
   	unsigned char Buff[4];
   	
//   	Point = FLCDSeg;
   	
   	if( Disk.FullDisDelay != 0 )
   	{
		RAM_DataFill( Disk.LCDSeg, 32, 0xff );
		return 1;
   	}	
   	
//	if(( IsAllData( Disk.CurDisID, 4, 0xFE ) == 0 )&&( Disk.DisInv != 0)) 
//	if((( IsAllData( Disk.CurDisID, 4, 0xFE ) == 0 )||( IsAllData( Disk.CurDisID, 5, 0xFF ) == 0 ))&&( Disk.DisInv != 0)) 		//15.02.03
	if((( IsAllData( Disk.CurDisObj.OAD, 4, 0xFE ) == 0 )||( IsAllData( Disk.CurDisObj.OAD, 5, 0xFF ) == 0 ))&&( Disk.DisInv != 0)) 		//15.02.03
	{	
		RAM_DataFill( Disk.LCDSeg, 32, 0xff );
		return 1;
   	}
	
//	RAM_Write( (unsigned char*)&DisID, Disk.CurDisID, 4 );
//	RAM_Write( (unsigned char*)&DisID, Disk.CurDisObj.OAD, 4 );
	DisID = SwapCharToLong( Disk.CurDisObj.OAD );
//	ItemNo = Disk.CurDisID[4];
	
	if(( DisID == 0x04B00817 )&&( Disk.DisInv != 0))   //液晶0~9显示	//10.11.05
	{																	//10.11.05
//		NDis_ReadZdyScreen( Disk.LCDSeg, Disk.CurDisID[4] );			//10.11.05
		NDis_ReadZdyScreen( Disk.LCDSeg, Disk.CurDisObj.No );			//10.11.05
		return 1;														//10.11.05	
	}																	//10.11.05	

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
	if(( DisID == 0xFAFCFDFE )&&( Disk.DisInv != 0)) return 1;  		//液晶任意BIT位显示	//14.09.03
#endif

	RAM_Fill( LCDSeg, 32 );
     	   
	if(( Disk.DisInv != 0) && ( (Flag.Disk & F_KeyInt) == 0))
	{
		if(((Flag.Power & F_PwrUp) == 0)&&(( Flag.Clk & F_DisSec ) != 0 )&&( DisID != 0x40000200 )) return 0;	//掉电后除了时间实时刷新，其余到循显时间后再刷新
		
		if((( Flag.Clk & F_DisSec ) != 0 ) && (( Flag.Disk & F_DspALT ) != 0 )) 
		{
			if((Flag.Power & F_PwrUp) == 0)
			{
//				if(( DisID != 0x04000101 )&&( DisID != 0x04000102 )) 
//				if(( DisID != 0x04000101 )&&( DisID != 0x04000102 )&&( DisID != 0x02020100 )&&( DisID != 0x02020200 )&&( DisID != 0x02020300 )) //10.09.03
//				if(( DisID != 0x04000101 )&&( DisID != 0x04000102 )&&( DisID != 0x0400010C )&&( DisID != 0x02020100 )&&( DisID != 0x02020200 )&&( DisID != 0x02020300 )) //10.09.03	//新国网
				if( DisID != 0x40000200 ) 								//10.09.03	//新国网
				{
					LcdStateDisplay( LCDSeg, FLCDSeg );									
					return 1;
				}			
			}				
		}
		else
		{
			if(( Flag.Clk & F_DisSec ) == 0 ) return 0; 
			LcdStateDisplay( LCDSeg, FLCDSeg );									
			return 1;
		}
	}

	if(((Flag.Disk & F_KeyInt) == 0) && ( Flag.Clk & F_DisSec ) == 0 )   return 0;
	Flag.Disk &= ~F_KeyInt;
   	if( Disk.DisInv == 0 )
   	{
   		Flag.Disk &= ~F_DspALT;			//Test
   		
	   	Disk.DisInv = Para.RDspInv;	

//		if( Disk.TouchDisMode == KeyMode )
		if(( Disk.TouchDisMode == KeyMode )||( Disk.TouchDisMode == LookMode ))			//新国网		//13.09.14
		{
			Disk.TouchDisMode = AutoMode;
//			Disk.ContentDisType = FaultType;			
			Disk.ContentDisType = AutoType;
            Disk.FaultItemPtr = -1;
			Disk.EventItemPtr = -1;
		}	
	   	
// 		Temp = GetAutoDisNextCode();
   		if( TouchDis( AutoMode, 1 ) == -1 ) 
   		{
			LcdStateDisplay( LCDSeg, FLCDSeg );	
			return 1; 			
   		}	
//		RAM_Write( (unsigned char*)&DisID, Disk.CurDisID, 4 );
		RAM_Write( (unsigned char*)&DisID, Disk.CurDisObj.OAD, 4 );
//		ItemNo = Disk.CurDisID[4];	
   	}	

//	if( IsAllData( Disk.CurDisID, 4, 0xFF ) == 0 ) 
	if( IsAllData( Disk.CurDisObj.OAD, 4, 0xFF ) == 0 ) 
	{
		LcdStateDisplay( LCDSeg, FLCDSeg );	
		return 1;
	}	

/*   	
	if(( Temp == -1 )||( IsAllData( Disk.CurDisID, 4, 0xFF ) == 0))
	{ 
//		GetErrCode( Point );
		GetBitCode( Point, Flag.AlarmFlg[0] );
		DspNum( Point, 4, &LCDSeg[7] );
		LCDSeg[15] = LCDSeg[7];
		
		LCDSeg[7] = 0;
		LCDSeg[8] = DS_Chr_E;	//"E"
		LCDSeg[9] = DS_Chr_r;	//"r"
		LCDSeg[10] = DS_Chr_r;	//"r"
		LcdTxtDisplay( LCDSeg, FLCDSeg );		
		return 1; 
	}	
*/

/*	if( NDis_Read( LCDSeg ) != 0 )
	{
		Disk.CurDisID[0] = 0x00;
		Disk.CurDisID[1] = 0x01;
		Disk.CurDisID[2] = 0x01;
		Disk.CurDisID[3] = 0x02;
		Disk.CurDisID[4] = 0x00;
		NDis_Read( LCDSeg );			//显示A相电压
	}	
*/
	if( Dis_GetData( LCDSeg ) != 0 )
	{
		Disk.CurDisObj.Type = 0x00;
		Disk.CurDisObj.OAD[0] = 0x20;
		Disk.CurDisObj.OAD[1] = 0x00;
		Disk.CurDisObj.OAD[2] = 0x02;
		Disk.CurDisObj.OAD[3] = 0x01;
		RAM_Fill( Disk.CurDisObj.ROAD, 4 );
		Disk.CurDisObj.No = 0x00;
		Dis_GetData( LCDSeg );			//显示A相电压
	}	

#if ( JiangSuRequire == YesCheck )
	if(( Disk.CurDisObj.OAD[0] == 0x50 )&&( Disk.CurDisObj.OAD[1] == 0x03 )) ;		//上24整点冻结小数码显示冻结时间
	else
	{
		LCDSeg[7] = 0;
//		DspNum( Disk.CurDisID, 4, &LCDSeg[14] );
		RAM_Write( Buff, Disk.CurDisObj.OAD, 4 );
		SwapData( Buff, 4 );
		DspNum( Buff, 4, &LCDSeg[14] );
		DisPoint( &LCDSeg[7], Fdisi );
	}	
#else
	LCDSeg[7] = 0;
//	DspNum( Disk.CurDisID, 4, &LCDSeg[14] );
	RAM_Write( Buff, Disk.CurDisObj.OAD, 4 );
	SwapData( Buff, 4 );
	DspNum( Buff, 4, &LCDSeg[14] );
	DisPoint( &LCDSeg[7], Fdisi );
#endif
	 
	LcdTxtDisplay( LCDSeg, FLCDSeg );	

#if ( CombinationFlagDisplay ==	NoCheck )			//11.07.19
//	Disk.LCDSeg[19] &= ~0x80;				//取消“组合”显示	
	if(( Disk.LCDSeg[21] & 0x04 ) != 0 )  Disk.LCDSeg[19] &= ~0x80;			//有功取消“组合”显示，无功继续显示	
#endif
	return 1;
}   	

void LcdStateDisplay( unsigned char *LCDSeg, unsigned char *FLCDSeg )
{
	short i;
	
	ActiveDisp( LCDSeg, FLCDSeg );
	for( i=0; i<32; i++)				//刷新显示
	{
		LCDSeg[i] |= FLCDSeg[i];
		Disk.LCDSeg[i] |= LCDSeg[i];
		if(( Flag.Disk & F_Flash ) == 0) Disk.LCDSeg[i] ^=  FLCDSeg[i];
	}	
}	
void LcdTxtDisplay( unsigned char *LCDSeg, unsigned char *FLCDSeg )
{
	short i;
	
	ActiveDisp( LCDSeg, FLCDSeg );
	for( i=0; i<32; i++)				//刷新显示
	{
		LCDSeg[i] |= FLCDSeg[i];
		if(( Flag.Disk & F_Flash ) == 0)  LCDSeg[i] ^=  FLCDSeg[i];
		Disk.LCDSeg[i] = LCDSeg[i];
	}	
}

void GetBitCode( unsigned char* Dest, unsigned char Temp )		
{
	short i;
	
	RAM_Fill( Dest, 4 );
	for( i=0;i<8;i++ )
	{
		if(( Temp & ByteBit[i] ) != 0 )
		{
			if( (i % 2) != 0 )	*( Dest + i/2 ) |= 0x10;
			else *( Dest + i/2 ) |= 0x01;
		}	
	}	
}	

//void DisNumber( unsigned char* LCDSeg )
void DisNumber( unsigned char* LCDSeg, unsigned char Number )			//新国网
{
	unsigned char Temp;
	
//	Temp = Disk.CurDisID[0];
	Temp = Number;
	if( Temp <= 9 ) *(LCDSeg+18) = NumSeg[Temp];		//上n
	else
	{
		Temp -= 10;
		*(LCDSeg+18) = NumSeg[Temp];					//上n
		*(LCDSeg+18) |= 0x08;							//上10以上的十位数
	}	
}

void DisLastN( unsigned char* LCDSeg )
{
	unsigned char Temp;
	
	if( Disk.CurDisObj.OAD[3] > 19 ) return;			//19以上，数值太大，液晶无法表示
	
	*(LCDSeg + 17) |= 0x40;								//"上"
//	Temp = Disk.CurDisID[0];
	Temp = Disk.CurDisObj.OAD[3];
	if( Temp <= 9 ) *(LCDSeg+18) = NumSeg[Temp];		//上n
	else
	{
		Temp -= 10;
		*(LCDSeg+18) = NumSeg[Temp];					//上n
		*(LCDSeg+18) |= 0x08;							//上10以上的十位数
	}	
}	

void DisPoint( unsigned char* LCDSeg, unsigned char Digit )
{
	short i;
	
	if( Digit != 0 ) 
	{
		if( Digit & 0x01 ) *(LCDSeg + 20) |= 0x01;		//COL1
		if( Digit & 0x02 ) *(LCDSeg + 22) |= 0x01;		//COL2
		for(i=2;i<8;i++)
		{
			if(( Digit & ByteBit[i] ) != 0 ) *(LCDSeg + i) |= 0x08;		//小数点
		}	
	}	
}	

void GetDigitEC( unsigned char* Source )
{
	unsigned long Value;
	unsigned char Temp;
	
//	E2P_RData( (unsigned char*)&Temp, ECDisDecDigits, 1 );
	Temp = Para.RECDisDecDigits;
	if( Temp == 2 ) return;								//两位小数返回	
	Value = BCD4_Long( Source );
	switch( Temp )
	{
		case 0: Value /= 100; break;	
		case 1: Value /= 10; break;	
		case 3: Value *= 10; break;	
		case 4: Value *= 100; break;	
		default: break;		
	}	
	Long_BCD4( Source, Value );
}	

void ECDigitSituation( unsigned char* LCDSeg )
{
	unsigned char Temp;

//	E2P_RData( (unsigned char*)&Temp, ECDisDecDigits, 1 );
	Temp = Para.RECDisDecDigits;
	switch( Temp )
	{
		case 0: break;	
		case 1: *(LCDSeg+15) |= 0x08; break;	
		case 3: *(LCDSeg+5) |= 0x08; break;	
		case 4: *(LCDSeg+4) |= 0x08; break;	
		default: *(LCDSeg+6) |= 0x08; break;	//缺省两位小数	
	}	
}	

void GetDigitMD( unsigned char* Source )
{
	unsigned long Value;
	unsigned char Buff[4];
	unsigned char* Ptr;
	unsigned char Temp;
	
	Ptr = Buff;
//	E2P_RData( (unsigned char*)&Temp, MDDisDecDigits, 1 );
	Temp = Para.RMDDisDecDigits;  
	if( Temp == 4 ) return;								//两位小数返回	
	RAM_Write( Ptr, Source, 3 );
	*(Ptr+3) = 0;
	Value = BCD4_Long( Ptr );
	switch( Temp )
	{
		case 0: Value /= 10000; break;	
		case 1: Value /= 1000; break;	
		case 2: Value /= 100; break;	
		case 3: Value /= 10; break;	
		default: break;		
	}	
	Long_BCD4( Ptr, Value );
	RAM_Write( Source, Ptr, 3 );
}	

void MDDigitSituation( unsigned char* LCDSeg )
{
	unsigned char Temp;

//	E2P_RData( (unsigned char*)&Temp, MDDisDecDigits, 1 );
	Temp = Para.RMDDisDecDigits;  

	switch( Temp )
	{
		case 0: break;	
		case 1: *(LCDSeg+15) |= 0x08; break;	
		case 2: *(LCDSeg+6) |= 0x08; break;
		case 3: *(LCDSeg+5) |= 0x08; break;	
		default: *(LCDSeg+4) |= 0x08; break;	//缺省四位小数	
	}	
}	

////////////////////////////////////////////////////////////////////////////////////变量
void GetDisUValue( unsigned char* Source, unsigned short ItemNumber )
{
	ItemNumber = ItemNumber * 2;
//	if( Data_Comp( Source, Para.RPhFail_StU, 2) >= 0 ) return;
	if( Data_Comp( Source, Para.RPhFail_Valve, 2) >= 0 ) return;
//	if( Data_Comp( Real.Ua+ItemNumber, Para.RPhFail_StU, 2) >= 0 ) 
	if( Data_Comp( Real.Ua+ItemNumber, Para.RPhFail_Valve, 2) >= 0 ) 
	{
		RAM_Write( Source, Real.Ua+ItemNumber, 2 );
		return;	
	}	
//	if( Data_Comp( Real.PreUa+ItemNumber, Para.RPhFail_StU, 2) >= 0 ) 
	if( Data_Comp( Real.PreUa+ItemNumber, Para.RPhFail_Valve, 2) >= 0 ) 
		RAM_Write( Source, Real.PreUa+ItemNumber, 2 );
}	

void GetDisAValue( unsigned char* Source, unsigned short ItemNumber )
{
	unsigned char Buff[8];
	unsigned char* Ptr;
	unsigned long Value;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, Source, 3 );
	*(Ptr+2) &= ~0x80;
	Value = BCD3_Long( Ptr );
	Value *= 10;
	Long_BCD4( Ptr, Value );
//	RAM_Write( Ptr+4, Para.RCuBreak_I, 3 );
//	*(Ptr+7) &= ~0x80;
	Long_BCD4( Ptr+4, Para.RCuBreak_I );
	
	if( Data_Comp( Ptr, Ptr+4, 4) < 0 ) return;						//V1000
	if( Data_Comp( Real.Ia+ItemNumber*4, Ptr+4, 4) < 0 ) 
	{
		RAM_Write( Source, Real.SIa+ItemNumber*3, 3 );
		return;	
	}	
	if( Data_Comp( Real.PreIa+ItemNumber*4, Ptr+4, 4) < 0 ) 
		RAM_Write( Source, Real.PreSIa+ItemNumber*3, 3 );
		
//	ItemNumber = ItemNumber * 3;
//	if( Data_Comp( Source, Para.RCuBreak_I, 3) < 0 ) return;						//V1000
//	if( Data_Comp( Real.Ia+ItemNumber, Para.RCuBreak_I, 3) < 0 ) 
//	{
//		RAM_Write( Source, Real.SIa+ItemNumber, 3 );
//		return;	
//	}	
//	if( Data_Comp( Real.PreIa+ItemNumber, Para.RCuBreak_I, 3) < 0 ) 
//		RAM_Write( Source, Real.PreSIa+ItemNumber, 3 );

}

void GetDisUA( unsigned long ComID, unsigned char* Source )
{
	unsigned char Buff[4];
	unsigned short Temp;
	
	RAM_Write( Buff, (unsigned char*)&ComID, 4 );
	
	if(( Buff[0] !=0 )||(Buff[3]!= 2 )) return;
	if(( Buff[1] < 1 )||( Buff[1] > 3 )) return;
	Temp = Buff[1];
	if( Buff[2] == 0x01 ) GetDisUValue( Source, Temp-1 );		//电压
	if( Buff[2] == 0x02 ) GetDisAValue( Source, Temp-1 );		//电流
	
}	

void GetDisPQValue( unsigned char* Source, unsigned char* Predata, unsigned short ItemNumber )
{
	unsigned char Buff[3];
	unsigned char* Ptr;
	unsigned long Value=0;
	unsigned long Value1;
	unsigned long PreValue=0;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, Source, 3 );
	*(Ptr+2) &= ~0x80;
	Value = BCD3_Long( Ptr );

	ItemNumber = ItemNumber * 3;
	RAM_Write( Ptr, Predata+ItemNumber, 3 );
	*(Ptr+2) &= ~0x80;
	PreValue = BCD3_Long( Ptr );

//	Value1 = ( PW010IbL / 2 );              //新国网		//13.08.30
	Value1 = ( GetPW100Ib() / 20 );        	//新国网		//13.08.30
	if( Value >= Value1 ) return;
	if( PreValue >= Value1 ) 
	{
		RAM_Write( Source, Predata+ItemNumber, 3 );
	}	
}	

void GetDisPQ( unsigned long ComID, unsigned char* Source )		//11.08.03
{
	unsigned char Buff[4];
	unsigned short Temp;
	
	RAM_Write( Buff, (unsigned char*)&ComID, 4 );
	
	if(( Buff[0] !=0 )||(Buff[3]!= 2 )) return;
	if( Buff[1] > 3 ) return;
	Temp = Buff[1];
	if(Temp == 0 ) Temp = 3;
	else Temp -= 1;
	if( Buff[2] == 0x03 ) GetDisPQValue( Source, (unsigned char*)Real.PrePa, Temp );		//有功功率
	if( Buff[2] == 0x04 ) GetDisPQValue( Source, (unsigned char*)Real.PreQa, Temp );		//无功功率
	
}	
/*
short IsPowerItem( unsigned long ComID )							//10.09.23
{
	unsigned char Buff[4];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
	RAM_Write( Ptr, (unsigned char*)&ComID, 4 );
	
	if((( ComID >= 0x02800003 )&&( ComID <= 0x02800006 ))
		||((*(Ptr+3) == 0x02 )&&(*(Ptr+2)>=0x03)&&(*(Ptr+2)<=0x05)&&(*(Ptr+1)<4)&&(*Ptr==0)))
	{
		return 0;
	}	
	
	return	1;		
	
}	
*/

//显示0~9
void NDis_ReadZdyScreen( unsigned char* LCDSeg, unsigned char Data )					//10.11.05
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	unsigned char Temp;
	
	Ptr = Buff;

	RAM_Fill( LCDSeg, 16 );
	RAM_DataFill( LCDSeg+16, 16, 0xff );
	*(LCDSeg+23) = 0;
	*(LCDSeg+24) = 0;
	*(LCDSeg+20) &= ~0x01;
	*(LCDSeg+22) &= ~0x01;
	
	Temp = Data & 0x0f;
	Temp += Temp * 16;	
	RAM_DataFill( Ptr, 4, Temp);
	DspNum( Ptr, 4, LCDSeg+7 );			//主显示内容
	switch( Data & 0x0f )
	{
		case 1: *(LCDSeg+7) |= 0x08; break;	
		case 2: *(LCDSeg+6) |= 0x08; break;	
		case 3: *(LCDSeg+5) |= 0x08; break;	
		case 4: *(LCDSeg+4) |= 0x08; break;	
		case 5: *(LCDSeg+3) |= 0x08; break;	
		case 6: *(LCDSeg+2) |= 0x08; break;	
		case 7: *(LCDSeg+1) |= 0x08; break;	
		case 8: *(LCDSeg+22) |= 0x01; break;	
		case 9: *(LCDSeg+20) |= 0x01; break;	
		default:
			break;		
	}	
	*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位
	DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
	DspNum( Ptr, 4, LCDSeg+14 );		//项内序号
	*(LCDSeg+18) = NumSeg[Data & 0x0f];
	*(LCDSeg+18) |= 0x08;
	*(LCDSeg+9) |= 0x08;
	*(LCDSeg+11) |= 0x08;
	*(LCDSeg+13) |= 0x08;
}	

void KeyProce( void )
{
	unsigned char Buff[4];
	unsigned char* Point;
	//short i,Temp;
	
	Point = Buff;
	RAM_Fill( Point, 4 );

	switch( SM.KbCount )
	{
//		case P_KEYXF: 	
		case GPIOA_KEY_DispDn: 	
						if( Disk.TouchDisMode == AutoMode ) 		//11.01.05				//新国网	
						{											//11.01.05				//新国网		
							Disk.TouchDisMode = KeyMode;			//11.01.05				//新国网			
//							Disk.ContentDisType = FaultType;		//11.01.05				//新国网			
                            Disk.ContentDisType = KeyType;
							Disk.FaultItemPtr = -1;					//11.01.05				//新国网		
							Disk.EventItemPtr = -1;					//11.01.05				//新国网			
							Disk.KeyItemPtr = -1;					//11.01.05				//新国网		
						}											//11.01.05				//新国网		

						Disk.FullDisDelay = 0;	
						TouchDis( KeyMode, 1 );	

						SM.RelayBJEnable = 0xAA;											//新国网	
						RAM_Write( Flag.OLDRelayOFFAlarmFlg, Flag.RelayOFFAlarmFlg, 8 );	//新国网

						break;
//		case P_KEYSF: 	
		case GPIOA_KEY_DispUp: 	
						if( Disk.TouchDisMode == AutoMode ) 		//11.01.05				//新国网	
						{											//11.01.05				//新国网		
							Disk.TouchDisMode = KeyMode;			//11.01.05				//新国网			
//							Disk.ContentDisType = FaultType;		//11.01.05				//新国网			
                            Disk.ContentDisType = KeyType;
							Disk.FaultItemPtr = -1;					//11.01.05				//新国网		
							Disk.EventItemPtr = -1;					//11.01.05				//新国网			
							Disk.KeyItemPtr = -1;					//11.01.05				//新国网		
						}											//11.01.05				//新国网		

						Disk.FullDisDelay = 0;	
						TouchDis( KeyMode, 0 ); 

						SM.RelayBJEnable = 0xAA;											//新国网	
						RAM_Write( Flag.OLDRelayOFFAlarmFlg, Flag.RelayOFFAlarmFlg, 8 );	//新国网
						break;	
#if ( LocalSmartMeter == YesCheck )
		case P_CARDINPUT:
						Flag.PrgState = 0;										//新国网	//13.12.10
#if ( PwOnLCDControl ==	YesCheck )		
 			  			if((Flag.Power & F_PwrUp) != 0 ) HT_GPIOE->PTSET |= GPIOE_Disp_CTL;//P7OUT |= P_DISCTL;
#endif
						ICComm.InsertCardState = 0;								//新国网	//13.12.09
			    		if( JudgeMeterVolt80Un() != 0 )							//电压过低	//新国网	//13.12.05
			    		{
			    			InitialAbnormalInsertCardMeassage();			//新国网	//13.12.02
			    			SM.KbCount &= ~P_CARDINPUT;
							ICComm.ICAlarmFlg = ICALARM_31;
							ICComm.ICDisAlarmFlg = ICALARM_31;					//新国网	//13.12.20
			    			ICComm.CardFaultFMQDelay = 41;						//读卡失败（错误）三声连续短鸣 短鸣为125ms，短鸣间隔也为125ms		
			    			ICComm.ICDisState = ICDisErr;
			    			ICComm.ErrorMessage = ICERROR_01; 
			    			Flag.Disk &= ~F_KeyInt;
							ICComm.ICDisDelay = 3;
							ICDisplay();
							w8576_byte( 0, 0 );
//							CreateCardInFaultRec( CardInFault_No, ICComm.ErrorMessage );		//生成插卡异常记录	//新国网	//13.12.02
			    			RecordStartEvent( CardInFault_No );
			    			ICComm.InsertCardState = 0x02;						//插卡失败		//新国网	//13.12.09
			    			return; 
			    		}		
			    		
			    		HT_GPIOB->PTCLR	|= GPIOB_VMEM_CTL;	//P8OUT &= ~P_EECTL;			
    					Disk.LCDDisEnable = 0x55;		//显示打开 
						SM.BJGDly = 60+1;
						SM.KbCount &= ~P_CARDINPUT;
						ICComm.ICDisState = ICDisCardReading;
						Display();					//新国网	//13.12.10
						w8576_byte( 0, 0 );
						HT_GPIOB->PTSET |= GPIOB_Backlight;//POUT_BGLED |= P_BGLED;
						SM.FMQOnDelay = 0;
						SM.FMQOffDelay = 0;
						ICProce();
						ProcessICAlarmFlg();		//新国网	//13.12.03
			
						KeyRelayOn();									//11.06.27	
						
						Card_Release( PSAM );
						if( GetIdentityValidState() != 0 ) Card_Release( ESAM );	//身份认证命令无效关闭ESAM
						ICComm.ICRDStateDelay = 3;	
						if( ICComm.ICOperateState == 0x03 ) { ICComm.ICAlarmFlg = ICALARM_35; ICComm.ICDisAlarmFlg = ICALARM_35; ICComm.ErrorMessage = ICERROR_20; }	//提前拔卡	//新国网	//13.12.03		
						if( ICComm.ICDisAlarmFlg != 0 ) 		//新国网	//13.12.20
						{
							ICComm.ICDisState = ICDisErr;
							ICComm.CardFaultFMQDelay = 41;			//读卡失败（错误）三声连续短鸣 短鸣为125ms，短鸣间隔也为125ms		//新国网	//13.12.05
			    			ICComm.InsertCardState = 0x02;			//插卡失败				//新国网	//13.12.09
						}
						else
						{
							ICComm.CardFaultFMQDelay = 41;		//读卡成功长鸣为625ms												//新国网	//13.12.05
			    			ICComm.InsertCardState = 0x01;		//插卡成功					//新国网	//13.12.09
						}	
							
						if( ICComm.ICDisState == ICDisCardReading )	ICComm.ICDisState = ICDisIdle;					
						if( ICComm.ICDisState != ICDisIdle )
						{
							Flag.Disk &= ~F_KeyInt;
							ICComm.ICDisDelay = 3;
							ICDisplay();
							w8576_byte( 0, 0 );
							
							if(( ICComm.CardType == CARD_PREPARE )&&( ICComm.ICDisState == ICDisExit ))
							{
								//if(( PIN_SETEN & P_SETEN ) == 0 ) ClrMeterClrRec();			//清除电表清零记录
        		 				if((HT_GPIOD->PTDAT & GPIOD_SETEN ) == 0 ) ResetSignleClass7Event( ECClr_No );		//清除电表清零记录
        		 				Comm.OperateCode[0] = ICComm.ParaUpdateFlag;				//11.05.24
        		 				Comm.OperateCode[1] = CARD_PREPARE;
        		 				Comm.OperateCode[2] = 0x99;
        		 				Comm.OperateCode[3] = 0x99;
//        		 				CreateECClrRec();		//电量清零前记录
         						RecordStartEvent( ECClr_No );
        						SM.MeterClrState = MeterClrStep2;							//进入电量清零流程		//11.01.14
         						SM.MeterClrStateChk = MeterClrStep2 + 0xA5A5;				//11.01.14	
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
				   				ClearAllEventUpState();						//新国网	//14.04.28    
#endif
							}								
						}
						else
						{
							Disk.DisInv = 0;	
							Flag.Disk |= F_KeyInt;
							Display();
							w8576_byte( 0, 0 );							
						}		
						if( ICComm.ICDisAlarmFlg == 0 ) 		//新国网	//13.12.20
						{
							if( ICComm.CardType == CARD_BUYEC )												//新国网	//13.12.10
							{
								if( ICComm.ParaUpdateFlag != 0 ) CreatICPrgRec( ICComm.CardType );			//编程记录
							}	
						}
//						else CreateCardInFaultRec( CardInFault_No, ICComm.ErrorMessage );		//生成插卡异常记录	//新国网	//13.12.02
						else RecordStartEvent( CardInFault_No );
						ICComm.ParaUpdateFlag = 0;
						Flag.PrgState = 0;										//新国网	//13.12.10
						SM.StepNo = GetCurrentStepNo( 0 );						//新国网	//14.01.07
						OperateRelay();						//11.06.27
						return;
		case P_CARDOUT:
						SM.KbCount &= ~P_CARDOUT;
						ICComm.ICInState &= ~F_BuyECOK;					
						return;
#endif
		default:
				SM.KbCount = 0;		
		return; 	
	}
	if((Flag.Power & F_PwrUp) == 0)
	{		
		SM.KeyDly = 30;
		SM.KbCount = 0;
		Disk.DisInv = 30;	
		Flag.Disk |= F_KeyInt;
		return;
	}	
	else
	{
		SM.KeyDly = DISDELAY;
		SM.BJGDly = DISDELAY;
	}	
	SM.KbCount = 0;
	Disk.DisInv = DISDELAY;	
	Flag.Disk |= F_KeyInt;
}	

#if ( LocalSmartMeter == YesCheck )
//返回值 0：不刷新显示  1：刷新显示
void ICDisplay(void)
{
   	unsigned char LCDSeg[32], FLCDSeg[40];
   	unsigned char* Ptr;
   	unsigned long Value,Value2; 
   	
   	Ptr = FLCDSeg;

	RAM_Fill( LCDSeg, 32 );
   	switch( ICComm.ICDisState )
   	{
   		case ICDisIdle:							// 正常轮显或按显	
   			break;	
   		case ICDisCardReading:					// 显示"--------"
   			break;
   		case ICDisBfRemainEC:					// 显示电量叠加前剩余电量
   			E2P_RData( (unsigned char*)&Value, BfOverdraftEC, 4 );
   			if( Value != 0 )
   			{
				*LCDSeg |= 0x08; 				//显示剩余电费为负数		//15.07.22			
				*(LCDSeg+20) |= 0x02;			//"剩余"					//15.07.22			
   			}
   			else
   			{
	   			E2P_RData( (unsigned char*)&Value, BfERemainEC, 4 );
				*(LCDSeg+20) |= 0x02;			//"剩余"
   			}		
			Long_BCD4( Ptr, Value );
			DspNum( Ptr, 4, LCDSeg+7 );			//主显示内容
			*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位
			*(LCDSeg+7) = 0;
   			*(LCDSeg+6) |= 0x08;				//"."
  			*(LCDSeg+21) |= 0x21;				//"电费"
  			*(LCDSeg+29) |= 0x08;				//"元"
   			ZeroDark( LCDSeg );
   			LcdTxtDisplay( LCDSeg, FLCDSeg );
   			break;
   		case ICDisRemainEC:				// 显示电量叠加后剩余电量	
   			if( ICComm.CardType == CARD_PREPARE )
   			{
   				E2P_RData( (unsigned char*)&Value, CurrentBuyEC, 4 );
				*(LCDSeg+20) |= 0x02;			//"剩余"
   				ICComm.ICDisState = ICDisExit;
   			}
   			else
   			{	
	   			GetReOverEC( (unsigned char*)&Value2, (unsigned char*)&Value );
   				if( Value != 0 )
   				{
					*LCDSeg |= 0x08; 				//显示剩余电费为负数	//15.07.22				
					*(LCDSeg+20) |= 0x02;			//"剩余"				//15.07.22				
   				}
   				else
   				{
					Value = Value2;
					*(LCDSeg+20) |= 0x02;			//"剩余"
   				}		
   			}	
			Long_BCD4( Ptr, Value );
			DspNum( Ptr, 4, LCDSeg+7 );			//主显示内容
			*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位
			*(LCDSeg+7) = 0;
   			*(LCDSeg+6) |= 0x08;				//"."
  			*(LCDSeg+21) |= 0x21;				//"电费"
  			*(LCDSeg+29) |= 0x08;				//"元"
			*(LCDSeg + 17) |= 0x80;			//"当前"
   			ZeroDark( LCDSeg );
   			LcdTxtDisplay( LCDSeg, FLCDSeg );
   			break;
   		case ICDisErr:							// 显示错误号
			*(LCDSeg+2) = DS_Chr_E;
			*(LCDSeg+3) = DS_Chr_r;
			*(LCDSeg+4) = DS_Chr_r;
			*(LCDSeg+5) = DS_Chr_Neg;
			*Ptr = ICComm.ICDisAlarmFlg;		//新国网	//13.12.20
			DspNum( Ptr, 1, LCDSeg+7 );			//主显示内容
			*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位
			*(LCDSeg+7) = 0;
   			LcdTxtDisplay( LCDSeg, FLCDSeg );
   			break;
		default: 
			ICComm.ICDisState = ICDisIdle;
			break;   				
   	}	
   	
}   	

void ICDisplayCourse(void)
{
	if( ICComm.ICDisDelay != 0 )
	{
		ICComm.ICDisDelay--;
		if( ICComm.ICDisDelay == 0 )
		{
   	   	   	ICComm.ICDisAlarmFlg = 0;					//新国网	//13.12.20
   			ICComm.ICInState &= ~F_RegECLimt;			// 取消囤积显示
   	   	   	switch( ICComm.ICDisState )
   	   	   	{
   	   	   		case ICDisIdle:							// 正常轮显或按显	
   	   	   			ICComm.ICInState &= ~F_RegECLimt;	// 取消囤积显示
   	   	   			break;	
   	   	   		case ICDisCardReading:					// 显示"--------"
   	   	   			ICComm.ICDisState = ICDisIdle;
   	   	   			break;
   	   	   		case ICDisBfRemainEC:					// 显示电量叠加前剩余电量
   	   	   			ICComm.ICDisDelay = 3;
   	   	   			ICComm.ICDisState = ICDisRemainEC;
   	   	   			ICDisplay();
   	   	   			w8576_byte( 0, 0 );
   	   	   			break;
   	   	   		case ICDisRemainEC:						// 显示电量叠加后剩余电量	
   	   	   			ICComm.ICDisState = ICDisIdle;
   	   	   			ICComm.ICInState &= ~F_RegECLimt;	// 取消囤积显示
   	   	   			break;
   	   	   		case ICDisErr:							// 显示错误号
   	   	   			ICComm.ICDisState = ICDisIdle;
					Disk.TouchDisMode = AutoMode;		//新国网	//13.12.10							
					Disk.ContentDisType = FaultType;	//新国网	//13.12.10						
					Disk.FaultItemPtr = -1;				//新国网	//13.12.10					
					Disk.EventItemPtr = -1;				//新国网	//13.12.10						
					Disk.KeyItemPtr = -1;				//新国网	//13.12.10					
   	   	   			break;
   	   	   		case ICDisExit:
   	   	   			ICComm.ICDisState = ICDisIdle;	
					break;
				default: 
   	   	   			ICComm.ICDisState = ICDisIdle;	
					break;   				
   	   	   	}	
		}	
	}	
	
	if( ICComm.ICRDStateDelay != 0 ) ICComm.ICRDStateDelay--;
	
}
#endif

void Dis_PhaseFailEventFormat( unsigned char* LCDSeg )		//17.04.12
{
	unsigned char Temp1;
	unsigned char Temp2;

	if(( Disk.CurDisObj.OAD[0] == 0x30 )&&( Disk.CurDisObj.OAD[1] == 0x00 )&&( Disk.CurDisObj.ROAD[0] == 0x00 ))	//失压电量	//17.04.12
	{
		Temp1 = Disk.CurDisObj.OAD[2];
		if(( Temp1 >= 7 )&&( Temp1 <= 9 ))
		{
			*(LCDSeg + 22) |= 0x12;							//"失压"	
			if( Temp1 == 7 ) *(LCDSeg + 19) |= 0x04;		//"A"
			else if( Temp1 == 8 ) *(LCDSeg + 19) |= 0x02;	//"B"
			else *(LCDSeg + 19) |= 0x01;					//"C" 
			Temp2 = Disk.CurDisObj.ROAD[2];
			Temp2 >>= 5;
			if( Temp2 <= 2 ) Temp2 = 0x00;
			else Temp2 = 0x01;
			*(LCDSeg+24) = 0;					//重置项内序号
			DspNum( &Temp2, 1, LCDSeg+24 );		//项内序号		
		}	
	}						
}	


unsigned char Dis_GetData( unsigned char* LCDSeg )
{
	unsigned short ClassId;
  	unsigned char ErrorByte=1;
	
	ClassId = GetOADClassId( Disk.CurDisObj.OAD );
	
	switch( ClassId )
	{
		case Class_id_EC:
			ErrorByte = Dis_GetECClassData( LCDSeg ); 
			break;					
		case Class_id_MD:					
			ErrorByte = Dis_GetMDClassData( LCDSeg ); 
			break;				
		case Class_id_PhaseVar:				
			ErrorByte = Dis_GetClass3Data( LCDSeg ); 
			break;				
		case Class_id_Power:					
			ErrorByte = Dis_GetClass4Data( LCDSeg ); 
			break;				
		case Class_id_HarmonicVar:			
			break;				
		case Class_id_DataVar:				
			ErrorByte = Dis_GetClass6Data( LCDSeg ); 
			break;				
		case Class_id_Event:					
			ErrorByte = Dis_GetEventClassData( LCDSeg ); 
			break;				
		case Class_id_Parameter:				
			ErrorByte = Dis_GetParameterClassData( LCDSeg ); 
			break;				
		case Class_id_Freeze:
			ErrorByte = Dis_GetFreezeClassData( LCDSeg  );			
			break;				
		case Class_id_Display:				
//			ErrorByte = Dis_GetDisplayClassData( LCDSeg ); 
			break;				
		case Class_id_File:					
			break;				
		case Class_id_Device:				
			break;				
		case Class_id_APP_Connect:			
			break;				
		case Class_id_ESAM:					
			break;				
		case Class_id_IO:		
			ErrorByte = Dis_GetClass22Data( LCDSeg );			
			break;				
		case Class_id_PhaseEvent:			
			ErrorByte = Dis_GetPhaseEventClassData( LCDSeg ); 
			break;				
		case Class_id_GPRS:					
			break;				
		case Class_id_Ethernet:
			break;				
		default: 
			break;			
	}
	
	if( ErrorByte == 0 )
	{
		if( Disk.CurDisObj.Type == 0 )
		{
			Dis_DataFormat( LCDSeg, Disk.CurDisObj.OAD );		
		}
		else
		{
			Dis_DataFormat( LCDSeg, Disk.CurDisObj.ROAD );		//事件、冻结类	
			Dis_PhaseFailEventFormat( LCDSeg );		//17.04.12
		}					
	}	
    return ErrorByte;	
}	

unsigned char Dis_DataFormat( unsigned char* LCDSeg, unsigned char* OAD )
{
	unsigned short ClassId;
  	unsigned char ErrorByte=1;
	
	ClassId = GetOADClassId( OAD );
	
	switch( ClassId )
	{
		case Class_id_EC:
			ErrorByte = Dis_ECClassFormat( LCDSeg, OAD ); 
			break;					
		case Class_id_MD:					
			ErrorByte = Dis_MDClassFormat( LCDSeg, OAD ); 
			break;				
		case Class_id_PhaseVar:				
			ErrorByte = Dis_Class3Format( LCDSeg, OAD ); 
			break;				
		case Class_id_Power:					
			ErrorByte = Dis_Class4Format( LCDSeg, OAD ); 
			break;				
		case Class_id_HarmonicVar:			
			break;				
		case Class_id_DataVar:				
			ErrorByte = Dis_Class6Format( LCDSeg, OAD ); 
			break;				
		case Class_id_Event:					
//			ErrorByte = Dis_EventClassFormat( LCDSeg, OAD ); 
			break;				
		case Class_id_Parameter:				
//			ErrorByte = Dis_ParameterClassFormat( LCDSeg, OAD ); 
			break;				
		case Class_id_Freeze:
//			ErrorByte = Dis_FreezeClassFormat( LCDSeg, OAD  );			
			break;				
		case Class_id_Display:				
//			ErrorByte = Dis_DisplayClassFormat( LCDSeg, OAD ); 
			break;				
		case Class_id_File:					
			break;				
		case Class_id_Device:				
			break;				
		case Class_id_APP_Connect:			
			break;				
		case Class_id_ESAM:					
			break;				
		case Class_id_IO:					
			break;				
		case Class_id_PhaseEvent:			
//			ErrorByte = Dis_PhaseEventFormat( LCDSeg, OAD ); 
			break;				
		case Class_id_GPRS:					
			break;				
		case Class_id_Ethernet:
			break;				
		default: 
			break;			
	}
    return ErrorByte;	
}	


//读电量类数据，返回值：1，无此数据项，0：正常
unsigned char Dis_GetECClassData( unsigned char* LCDSeg )
{
	short i;
	unsigned short Addr;
	unsigned short OICode;
	short Temp;
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
	unsigned char FeeNum;
	unsigned char* OAD;
  	unsigned char ErrorByte=1;
	
	Ptr = Buff;
	OAD = Disk.CurDisObj.OAD;
	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	Index = *(OAD+3);

	i = GetECTabItemNo( Disk.CurDisObj.OAD );
	if( i != DL698_ECSum )
	{
		switch( Attribute )
		{
			case 2:									//value
				Addr = DL698_ECTab[i].Addr;
				FeeNum = DL698_ECTab[i].FeeNum;
				FeeNum &= 0x0F;						//分费率数	
				if( Index > (FeeNum+1) )			//超过费率数不支持
				{
					return ErrorByte;
				}	
				if( Index == 0 ) return ErrorByte;				//总尖峰平谷
				else 
				{
					if( Para.RECDisDecDigits == 3 ) Disk.DigitsValidFlg = 0x55;		//3位小数显示时才添加0.01kWh以下尾数		//新国网	//13.10.25
					Temp = (Index-1)*(ECDIGIT+1);
					Read_CMonEC( Ptr, Addr+Temp );
					Disk.DigitsValidFlg = 0x00;
					Temp = OICode & 0x00F0;
					if(( OICode == 0x0000 )||( Temp == 0x0030 )||( Temp == 0x0040 ))
					{
						if( *(Ptr+(ECDIGIT-1)) & 0x80 )
						{
							*(Ptr+(ECDIGIT-1)) &= ~0x80; //屏蔽组合的符号位
							*LCDSeg |= 0x08; 	//显示组合的符号位	
						}	
					}
					LeftShiftData( Ptr, ECDIGIT, (4-Para.RECDisDecDigits) );
				}
				*(LCDSeg + 17) |= 0x80;			//"当前"
//				if( Attribute == 2 ) ECDigitSituation( LCDSeg );	
//				else *(LCDSeg+4) |= 0x08;		//精确电能4位小数	
   				if((Flag.Power & F_PwrUp) != 0)	Flag.Disk |= F_DspALT;		//上电且是当前月需及时刷新
   				else Flag.Disk &= ~F_DspALT;
   				ErrorByte = 0;
				break;	
			case 4:									//精确电能
				Addr = DL698_ECTab[i].Addr;
				FeeNum = DL698_ECTab[i].FeeNum;
				FeeNum &= 0x0F;						//分费率数	
				if((( OICode & 0x000f ) != 0 )||(( OICode & 0x00f0 ) >= 9 )||( Index > (FeeNum+1) ))		//视在电能、分相电量、超过费率数不支持精确电能
				{
					return ErrorByte;
				}	
				if( Index == 0 ) return ErrorByte;					//总尖峰平谷
				else
				{
					Disk.DigitsValidFlg = 0x55;		//3位小数显示时才添加0.01kWh以下尾数		//新国网	//13.10.25
					Read_CMonEC( Ptr, Addr );
					Disk.DigitsValidFlg = 0x00;
					Temp = OICode & 0x00F0;
					if(( OICode == 0x0000 )||( Temp == 0x0030 )||( Temp == 0x0040 ))
					{
						if( *(Ptr+(ECDIGIT-1)) & 0x80 )
						{
							*(Ptr+(ECDIGIT-1)) &= ~0x80; //屏蔽组合的符号位
							*LCDSeg |= 0x08; 	//显示组合的符号位	
						}	
					}			
				}	
				*(LCDSeg + 17) |= 0x80;			//"当前"
   				*(LCDSeg+4) |= 0x08;		//精确电能4位小数	
   				if((Flag.Power & F_PwrUp) != 0)	Flag.Disk |= F_DspALT;		//上电且是当前月需及时刷新
   				else Flag.Disk &= ~F_DspALT;
   				ErrorByte = 0;
				break;	
			default: break;			
		}
		
		if( ErrorByte == 0 )
		{
			DspNum( Ptr, 4, LCDSeg+7 );			//主显示内容				
			*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
			*Ptr = 0;
			DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
		}	
	}
	return ErrorByte;	
}	

//读电量类数据，返回值：1，无此数据项，0：正常
unsigned char Dis_ECClassFormat( unsigned char* LCDSeg, unsigned char* OAD )
{
	short i;
//	unsigned short OICode;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
 	unsigned char ErrorByte=1;
	
//	Ptr = Buff;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	Index = *(OAD+3);
	
	i = GetECTabItemNo( OAD );
	if( i != DL698_ECSum )
	{
		switch( Attribute )
		{
			case 2:									//value
			case 4:									//精确电能
				*(LCDSeg + 19) |= DL698_ECTab[i].State1;
				*(LCDSeg + 20) |= DL698_ECTab[i].State2;
				*(LCDSeg + 21) |= DL698_ECTab[i].State3;
				*(LCDSeg + 22) |= DL698_ECTab[i].State4;
				*(LCDSeg + 30) |= DL698_ECTab[i].State5;
				*(LCDSeg + 31) |= DL698_ECTab[i].State6;
				if(( *(LCDSeg + 30) & 0x01 ) != 0 ) 
				{
					*(LCDSeg + 30) &= ~0x01;				//"h(2)"	
					*(LCDSeg + 29) |= 0x04;					//"h(2)"					
				}	
				if(( *(LCDSeg + 30) & 0x02 ) != 0 ) 
				{
					*(LCDSeg + 30) &= ~0x02;				//"A"	
					*(LCDSeg + 29) |= 0x40;					//"A"					
				}	

				switch( Index-1 )
				{
					case 0:
						*(LCDSeg + 22) |= 0x80;			//"总"
						break;
					case 1:
						*(LCDSeg + 22) |= 0x40;			//"尖"
						break;
					case 2:
						*(LCDSeg + 31) |= 0x08;			//"峰"
						break;
					case 3:
						*(LCDSeg + 31) |= 0x04;			//"平"
						break;
					case 4:
						*(LCDSeg + 31) |= 0x40;			//"谷"
						break;
					default: break;												
				}					

				if( Attribute == 2 ) ECDigitSituation( LCDSeg );	
				else *(LCDSeg+4) |= 0x08;		//精确电能4位小数	
				ZeroDark( LCDSeg );			//高位去零
				ErrorByte = 0;		
				break;	
			default: break;			
		}	
	}
	return ErrorByte;	
}	

//读需量类数据，返回值：-1，无此数据项，非-1：返回数据长度
unsigned char Dis_GetMDClassData( unsigned char* LCDSeg )
{
	unsigned short Addr;
	short Len=0;
	unsigned short OICode;
	short i;
	short Temp;
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
	unsigned char FeeNum;
	unsigned char* OAD;
  	unsigned char ErrorByte=1;
	
	Ptr = Buff;
	OAD = Disk.CurDisObj.OAD;
	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	Index = *(OAD+3);
	
	i = GetMDTabItemNo( OAD );
	if( i != DL698_MDSum )
	{
		switch( Attribute )
		{
			case 2:									//value
				Addr = DL698_MDTab[i].Addr;
				FeeNum = DL698_MDTab[i].FeeNum;
				FeeNum &= 0x0F;						//分费率数	
				if( Index > (FeeNum+1) )					//超过4费率不支持
				{
					return ErrorByte;
				}	
				if( Index == 0 ) return ErrorByte;	//总尖峰平谷
				else
				{
					ErrorByte = 0;
					Temp = (Index-1)*9;
					E2P_RData( Ptr, Addr+Temp, 8 );
					switch( Disk.CurDisObj.No )
					{
						case 0x00:
							Temp = OICode & 0x00F0;
							if(( OICode == 0x0000 )||( Temp == 0x0030 )||( Temp == 0x0040 ))
							{
								if( *(Ptr+2) & 0x80 )
								{
									*(Ptr+2) &= ~0x80; //屏蔽组合的符号位
									*LCDSeg |= 0x08; 	//显示组合的符号位	
								}	
							}
							GetDigitMD( Ptr );
							Len = 3;	
							break;	
						case 0x01:
							RAM_Write( Ptr, Ptr+5, 3 );			//年月日
							Len = 3;
							break;	
						case 0x02:
							RAM_Write( Ptr, Ptr+3, 2 );			//年月日
							Len = 2;
							break;	
						default:
							ErrorByte = 1;
							break;													
					}	
				}
			default:
				break;
		}	

		if( ErrorByte == 0 )
		{
			DspNum( Ptr, Len, LCDSeg+7 );			//主显示内容				
			*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
			*Ptr = Disk.CurDisObj.No;
			DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
			*(LCDSeg + 17) |= 0x80;			//"当前"
   			if((Flag.Power & F_PwrUp) != 0)	Flag.Disk |= F_DspALT;		//上电且是当前月需及时刷新
   			else Flag.Disk &= ~F_DspALT;
		}	
	}
	return ErrorByte;
}	

//读电量类数据，返回值：1，无此数据项，0：正常
unsigned char Dis_MDClassFormat( unsigned char* LCDSeg, unsigned char* OAD )
{
	short i;
//	unsigned short OICode;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
 	unsigned char ErrorByte=1;
	
//	Ptr = Buff;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	Index = *(OAD+3);
	
	i = GetMDTabItemNo( OAD );
	if( i != DL698_MDSum )
	{
		switch( Attribute )
		{
			case 2:									//value
				*(LCDSeg + 19) |= DL698_MDTab[i].State1;
				*(LCDSeg + 20) |= DL698_MDTab[i].State2;
				*(LCDSeg + 21) |= DL698_MDTab[i].State3;
				*(LCDSeg + 22) |= DL698_MDTab[i].State4;
				*(LCDSeg + 31) |= DL698_MDTab[i].State6;
//				State = DL698_MDTab[i].State1 & D_Sa7;	//"组合"	
				if( Disk.CurDisObj.No == 0 ) 
				{
					*(LCDSeg + 30) = DL698_MDTab[i].State5;			//需量单位
					if(( *(LCDSeg + 30) & 0x02 ) != 0 ) 
					{
						*(LCDSeg + 30) &= ~0x02;				//"A"	
						*(LCDSeg + 29) |= 0x40;					//"A"					
					}	
					MDDigitSituation( LCDSeg );			
					ZeroDark( LCDSeg );			//高位去零	
				}	
				else if( Disk.CurDisObj.No == 1 ) 			//日期
				{
					*(LCDSeg + 31) |= 0x11;				//"时间"
					DisPoint( LCDSeg, Fymd );			//年月日
					*(LCDSeg+15) = *(LCDSeg+7);				//数码9移位
				}	
				else
				{
					*(LCDSeg + 31) |= 0x11;				//"时间"
					DisPoint( LCDSeg, Fhm );			//时分	
					*(LCDSeg+15) = *(LCDSeg+7);				//数码9移位
				}	

				switch( Index-1 )
				{
					case 0:
						*(LCDSeg + 22) |= 0x80;			//"总"
						break;
					case 1:
						*(LCDSeg + 22) |= 0x40;			//"尖"
						break;
					case 2:
						*(LCDSeg + 31) |= 0x08;			//"峰"
						break;
					case 3:
						*(LCDSeg + 31) |= 0x04;			//"平"
						break;
					case 4:
						*(LCDSeg + 31) |= 0x40;			//"谷"
						break;
					default: break;												
				}					
				ErrorByte = 0;		
				break;	
			default: break;			
		}	
	}
	return ErrorByte;	
}	

//读分相变量接口类数据，返回值：-1，无此数据项，非-1：返回数据长度
unsigned char Dis_GetClass3Data( unsigned char* LCDSeg )
{
	short i;
	unsigned char* Addr;
//	short Len;
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
	unsigned char DataType;
	unsigned char BCD_Len;			//原BCD数据长度
	unsigned char* OAD;
  	unsigned char ErrorByte=1;
	
	Ptr = Buff;
	OAD = Disk.CurDisObj.OAD;
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	Index = *(OAD+3);
	
	i = GetClassId3TabItemNo( OAD );
	if( i != DL698_ClassId3Sum )
	{
		switch( Attribute )
		{
			case 2:									//value
				if( Index == 0 ) return ErrorByte;	//总ABC
				else
				{
					Addr = DL698_ClassId3Tab[i].Addr;
					DataType = DL698_ClassId3Tab[i].DataType;
					BCD_Len = DL698_ClassId3Tab[i].BCD_Len;
					RAM_Write( Ptr, Addr, BCD_Len );
					if( DataType == D_Integer32 )
					{
						if( *(Ptr+2) & 0x80 )
						{
							*(Ptr+2) &= ~0x80; 	//屏蔽符号位
							*LCDSeg |= 0x08; 	//显示符号位	
						}	
					}	
					
					DspNum( Ptr, BCD_Len, LCDSeg+7 );	//主显示内容				
					*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
					*Ptr = 0;
					DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
   					if((Flag.Power & F_PwrUp) != 0)	Flag.Disk |= F_DspALT;		
   					else Flag.Disk &= ~F_DspALT;
					ErrorByte = 0;
				}	
				break;
			default: break;			
		}	
	}
	return ErrorByte;	
}	


void DisplayUnit( unsigned char *LCDSeg, unsigned char Unit )
{
	switch( Unit )
	{
		case U_W:
		case U_kW:
			*(LCDSeg + 21) |= 0x0C;			//“有功”
			*(LCDSeg + 31) |= 0x02;			//“功率”
			*(LCDSeg + 30) = 0x88;			//"KW"
			break;      
    	case U_VA:
    	case U_kVA:     
			*(LCDSeg + 31) |= 0x02;			//“功率”
			*(LCDSeg + 29) = 0x40;			//"A"
			*(LCDSeg + 30) = 0x80;			//"K"
			break;      
    	case U_var:    
    	case U_kvar:    
			*(LCDSeg + 20) |= 0x80;			//“无”
			*(LCDSeg + 21) |= 0x08;			//“功”
			*(LCDSeg + 31) |= 0x02;			//“功率”
			*(LCDSeg + 30) = 0x80;			//"kvar"
			break;      
		case U_A:      
			*(LCDSeg + 21) = 0x01;			//"电"
			*(LCDSeg + 22) = 0x20;			//"流"
			*(LCDSeg + 29) = 0x40;			//"A"
			*(LCDSeg + 29) = 0x40;			//"A"
			break;      
		case U_V:      
			*(LCDSeg + 21) = 0x01;			//"电"
			*(LCDSeg + 22) = 0x10;			//"压"
			*(LCDSeg + 30) = 0x08;			//"V"
			break;      
		default: break;
	}	
}	

void DisplayPhase( unsigned char *LCDSeg, unsigned char Phase )
{
	switch( Phase )
	{
		case 1:
			*(LCDSeg + 19) = 0x04;			//"A"
			break;
		case 2:
			*(LCDSeg + 19) = 0x02;			//"B"
			break;
		case 3:
			*(LCDSeg + 19) = 0x01;			//"C"
			break;
		case 4:
			*(LCDSeg + 19) = 0x10;			//"N"
			break;
		default:
			break;
	}		
}

unsigned char Dis_Class3Format( unsigned char* LCDSeg, unsigned char* OAD )
{
	short i;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Unit;
	unsigned char Index;
 	unsigned char ErrorByte=1;
	
//	Ptr = Buff;
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	*(OAD+2) &= ~0xE0;								//去掉特征值，以利于冻结事件数据采用
	Index = *(OAD+3);
	
	i = GetClassId3TabItemNo( OAD );
	if( i != DL698_ClassId3Sum )
	{
		switch( Attribute )
		{
			case 2:									//value
				Unit = DL698_ClassId3Tab[i].Unit;
				DisplayUnit( LCDSeg, Unit );
				if( *(OAD+1) != 0x02 ) DisplayPhase( LCDSeg, Index );		//电压夹角除外
				if( Unit == U_A ) *(LCDSeg+5) |= 0x08; 	//三位小数
				else *(LCDSeg+15) |= 0x08;				//一位小数
				ZeroDark( LCDSeg );			//高位去零	
				ErrorByte = 0;		
				break;	
			default: break;			
		}	
	}
	return ErrorByte;	
}	

//读功率接口类数据，返回值：-1，无此数据项，非-1：返回数据长度
unsigned char Dis_GetClass4Data( unsigned char* LCDSeg )
{
	short i;
	unsigned char Buff[10];
	unsigned char* Addr;
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
	unsigned char DataType;
	unsigned char BCD_Len;			//原BCD数据长度
	unsigned char* Ptr;
	unsigned char* OAD;
  	unsigned char ErrorByte=1;
	
	Ptr = Buff;
	OAD = Disk.CurDisObj.OAD;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
	
	i = GetClassId4TabItemNo( OAD );
	if( i != DL698_ClassId4Sum )
	{
		switch( Attribute )
		{
			case 2:									//value
					Addr = DL698_ClassId4Tab[i].Addr;
					DataType = DL698_ClassId4Tab[i].DataType;
					BCD_Len = DL698_ClassId4Tab[i].BCD_Len;
					RAM_Write( Ptr, Addr, BCD_Len );
					if( DataType == D_Integer32 )
					{
						if( *(Ptr+2) & 0x80 )
						{
							*(Ptr+2) &= ~0x80; 	//屏蔽符号位
							*LCDSeg |= 0x08; 	//显示符号位	
						}	
					}
					else if( DataType == D_Integer16 )
					{
						if( *(Ptr+1) & 0x80 )
						{
							*(Ptr+1) &= ~0x80; 	//屏蔽符号位
							*LCDSeg |= 0x08; 	//显示符号位	
						}	
					}		
					
					if(( *(OAD+1) >= 0x04 )&&( *(OAD+1) <= 0x09 )) GetDigitMD( Ptr );
					
					DspNum( Ptr, BCD_Len, LCDSeg+7 );	//主显示内容				
					*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
					*Ptr = 0;
					DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
   					if((Flag.Power & F_PwrUp) != 0)	Flag.Disk |= F_DspALT;		
   					else Flag.Disk &= ~F_DspALT;
					ErrorByte = 0;
				break;	
			default: break;			
		}	
	}
	return ErrorByte;	
}

void DisplayPhasePW( unsigned char *LCDSeg, unsigned char Phase )
{
	switch( Phase )
	{
		case 1:
			*(LCDSeg + 22) = 0x80;			//"总"
			break;
		case 2:
			*(LCDSeg + 19) = 0x04;			//"A"
			break;
		case 3:
			*(LCDSeg + 19) = 0x02;			//"B"
			break;
		case 4:
			*(LCDSeg + 19) = 0x01;			//"C"
			break;
		default:
			break;
	}		
}

unsigned char Dis_Class4Format( unsigned char* LCDSeg, unsigned char* OAD )
{
	short i;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
	unsigned char Unit;
  	unsigned char ErrorByte=1;
	
//	Ptr = Buff;
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	*(OAD+2) &= ~0xE0;								//去掉特征值，以利于冻结事件数据采用
	Index = *(OAD+3);
	
	i = GetClassId4TabItemNo( OAD );
	if( i != DL698_ClassId4Sum )
	{
		switch( Attribute )
		{
			case 2:									//value
				Unit = DL698_ClassId4Tab[i].Unit;
				DisplayUnit( LCDSeg, Unit );
				if( *(OAD+1) != 0x02 ) DisplayPhasePW( LCDSeg, Index );		//电压夹角除外
				if(( *(OAD+1) >= 0x04 )&&( *(OAD+1) <= 0x09 )) //功率
				{
					MDDigitSituation( LCDSeg );	
				}
				else
				{	
					if( Unit == U_FF ) *(LCDSeg+5) |= 0x08; 	//三位小数
					else *(LCDSeg+4) |= 0x08;				//四位小数
				}	
				ZeroDark( LCDSeg );			//高位去零	
				ErrorByte = 0;		
				break;	
			default: break;			
		}	
	}
	return ErrorByte;	
}	

//读数据变量接口类数据，返回值：-1，无此数据项，非-1：返回数据长度
unsigned char Dis_GetClass6Data( unsigned char* LCDSeg )
{
	short i;
	unsigned char* Addr;
//	short Len;
//	unsigned short OICode;
//	short Temp;
//	short Start,End;
	unsigned char Buff[10];
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
	unsigned char DataType;
//	unsigned char Element_Len;			
	unsigned char BCD_Len;			//原BCD数据长度
//	char		  Multiple;			//放大倍数				负数表示缩小
	unsigned char* Ptr;
	unsigned char* OAD;
  	unsigned char ErrorByte=1;
	unsigned long ComID;
	
//	Len = -1;
	
	Ptr = Buff;
	OAD = Disk.CurDisObj.OAD;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
	ComID = SwapCharToLong( OAD );
	
	i = GetClassId6TabItemNo( OAD );
	if( i != DL698_ClassId6Sum )
	{
		switch( Attribute )
		{
			case 2:									//value
				Addr = DL698_ClassId6Tab[i].Addr;
				DataType = DL698_ClassId6Tab[i].DataType;
				BCD_Len = DL698_ClassId6Tab[i].BCD_Len;

				if(( DataType == Data_array )||( DataType == D_OctetStr )||( DataType == D_Bit_String )) return ErrorByte;		//复合型数据不支持显示
				if( ComID == 0x20310200 )													//月度用电量
				{
					GetCycleInsideEC( Ptr, 0, 1 );
					*( Ptr+3 ) &= ~0x80;
				}	
#if ( LocalSmartMeter == YesCheck )
				else if( ComID == 0x201A0200 )												//当前电价
				{
					Long_BCD4( Ptr, GetCurrentECPrice() );
					*(LCDSeg + 29) |= 0x08;		//"元"	
					*(LCDSeg + 21) |= 0x11;		//"电价"	
					*(LCDSeg + 17) |= 0x80;			//"当前"
				}	
				else if( ComID == 0x201B0200 )												//当前费率电价
				{
					Long_BCD4( Ptr, GetCurrentFeePrice() );
					*(LCDSeg + 29) |= 0x08;		//"元"	
					*(LCDSeg + 21) |= 0x11;		//"电价"	
				}	
				else if( ComID == 0x201C0200 )												//当前阶梯电价
				{
					Long_BCD4( Ptr, GetCurrentStepPrice() );
					*(LCDSeg + 29) |= 0x08;		//"元"	
					*(LCDSeg + 20) |= 0x04;		//"阶梯"	
					*(LCDSeg + 21) |= 0x11;		//"电价"	
					*(LCDSeg + 17) |= 0x80;		//"当前"					
				}	
				else if( ComID == 0x202D0200 )												//当前透支金额
				{
					GetReOverEC( Ptr+4, Ptr );
					*(LCDSeg+20) |= 0x02;			//"剩余"					//15.07.22			
					if( IsAllData( Ptr, 4, 0x00 ) != 0 ) *LCDSeg |= 0x08; 	//显示剩余电费为负数	
  					*(LCDSeg+21) |= 0x21;			//"电费"
			  		*(LCDSeg+29) |= 0x08;			//"元"
					*(LCDSeg + 17) |= 0x80;			//"当前"
				}	
				else if( ComID == 0x202E0200 )												//累计购电金额
				{
					Long_BCD4( Ptr, GetBuyECSum() );
			  		*(LCDSeg+29) |= 0x08;			//"元"
				}	
				else if( ComID == 0x20300200 )												//阶梯结算用电量
				{
					if( ICComm.YearMonSaveMode == 0 ) GetCycleInsideEC( Ptr, 0, 1 );
					else GetYearCycleInsideEC( Ptr, 0, 1 );
					*( Ptr+3 ) &= ~0x80;
				}	
#endif
				else
				{
					RAM_Write( Ptr, Addr, BCD_Len );
				}	

				if( DataType == D_Integer32 )
				{
					if( *(Ptr+2) & 0x80 )
					{
						*(Ptr+2) &= ~0x80; 	//屏蔽符号位
						*LCDSeg |= 0x08; 	//显示符号位	
					}	
				}
				else if( DataType == D_Integer16 )
				{
					if( *(Ptr+1) & 0x80 )
					{
						*(Ptr+1) &= ~0x80; 	//屏蔽符号位
						*LCDSeg |= 0x08; 	//显示符号位	
					}	
				}		
				
				DspNum( Ptr, BCD_Len, LCDSeg+7 );	//主显示内容				
				*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
				*Ptr = 0;
				DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
   				if((Flag.Power & F_PwrUp) != 0)	Flag.Disk |= F_DspALT;		
   				else Flag.Disk &= ~F_DspALT;
				ErrorByte = 0;				
			default: break;			
		}	
	}
	return ErrorByte;	
}

void DigitScaler( unsigned char* LCDSeg, unsigned char Scaler )
{
	switch( Scaler )
	{
		case 0xFF: *(LCDSeg+15) |= 0x08; break;	//一位小数	
		case 0XFE: *(LCDSeg+6) |= 0x08; break;	//二位小数	
		case 0xFD: *(LCDSeg+5) |= 0x08; break;	//三位小数	
		case 0xfc: *(LCDSeg+4) |= 0x08; break;	//四位小数	
		default: break;	
	}	
}	

unsigned char Dis_Class6Format( unsigned char* LCDSeg, unsigned char* OAD )
{
	short i;
	unsigned char Attribute;
//	unsigned char Feature;
	unsigned char Index;
	unsigned char Unit;
 	unsigned char ErrorByte=1;
	
//	Ptr = Buff;
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
	*(OAD+2) &= ~0xE0;								//去掉特征值，以利于冻结事件数据采用
	Index = *(OAD+3);
	
	i = GetClassId6TabItemNo( OAD );
	if( i != DL698_ClassId6Sum )
	{
		switch( Attribute )
		{
			case 2:									//value
				Unit = DL698_ClassId6Tab[i].Unit;
				DisplayUnit( LCDSeg, Unit );
				if( *(OAD+1) != 0x02 ) DisplayPhasePW( LCDSeg, Index );		//电压夹角除外
				DigitScaler( LCDSeg, DL698_ClassId6Tab[i].Scaler );
				ZeroDark( LCDSeg );			//高位去零	
				ErrorByte = 0;		
				break;	
			default: break;			
		}	
	}
	return ErrorByte;	
}	

unsigned char Dis_GetParameterClassData( unsigned char* LCDSeg )
{
	short i;
//	unsigned short Addr;
	short Len;
	unsigned short Temp;
//	short Start,End;
	unsigned char Buff[20];
	unsigned char DataType;
//	unsigned char Element_Len;			
//	unsigned char BCD_Len;			//原BCD数据长度
//	char		  Multiple;			//放大倍数				负数表示缩小
	unsigned char* Ptr;
	unsigned short ComInfo;
	unsigned long ComID;
	unsigned long Value;
	unsigned char* OAD;
  	unsigned char ErrorByte=1;
	unsigned short OICode;
	Len = -1;
	
	Ptr = Buff;
	OAD = Disk.CurDisObj.OAD;
	OICode = SwapCharToShort( OAD );
	i = GetParameterClassTabItemNo( OAD );
	if( i != DL698_ClassId8Sum )
	{
		ComID = SwapCharToLong( OAD );
		DataType = DL698_ClassId8_Tab[i].DataType;
//		Element_Len = DL698_ClassId8_Tab[i].Element_Len;
		ComInfo = DL698_ClassId8_Tab[i].ComInfo;
		if(( DataType == Data_array )||( DataType == Data_structure )) 					
		{	
			if((ComInfo & Comm_Type ) == SPECFUN )			//节假日、时区、时段、阶梯、费率电价、结算日、显式安全模式等需特殊处理
			{
//				ErrorByte = Dis_GetSpecClassId8( LCDSeg, OAD );	
				if( ComID == 0x41160201 ) 							//结算日1
				{
					E2P_RData( Ptr, ESave_Day1, 2 );
					*(LCDSeg+6) |= 0x08; 							//二位小数
					Len = 2;			
					ErrorByte = 0;	
				}	
				else if( ( ComID & 0xFFFFFF00 ) == 0x40180200 )		//当前费率电价
				{
					if( *(OAD+3) > 4 ) return ErrorByte;
					Temp = *(OAD+3) - 1;
					E2P_RData( Ptr, Case1Fee1Price+Temp*5, 4 );
//					DisPoint( LCDSeg, Digit4 );		//四位小数
					*(LCDSeg+4) |= 0x08; 			//四位小数
					*(LCDSeg + 29) |= 0x08;		//"元"	
					*(LCDSeg + 21) |= 0x11;		//"电价"	
					*(LCDSeg + 17) |= 0x80;			//"当前"
					switch( Temp )
					{
						case 0:
							*(LCDSeg + 22) |= 0x40;			//"尖"
							break;
						case 1:
							*(LCDSeg + 31) |= 0x08;			//"峰"
							break;
						case 2:
							*(LCDSeg + 31) |= 0x04;			//"平"
							break;
						case 3:
							*(LCDSeg + 31) |= 0x40;			//"谷"
							break;
						default: break;												
					}
					Len = 4;											
					ErrorByte = 0;	
				}						
				else if( ( ComID & 0xFFFFFF00 ) == 0x401A0200 )		//当前阶梯电价
				{
					if( *(OAD+3) > 7 ) return ErrorByte;
					Temp = *(OAD+3) - 1;
					E2P_RData( Ptr, Case1Step1Price+Temp*5, 4 );
//					DisPoint( LCDSeg, Digit4 );		//四位小数
					*(LCDSeg+4) |= 0x08; 			//四位小数
					DisNumber( LCDSeg, Temp+1 );	//序号		
					*(LCDSeg + 29) |= 0x08;		//"元"	
					*(LCDSeg + 20) |= 0x04;		//"阶梯"	
					*(LCDSeg + 21) |= 0x11;		//"电价"	
					*(LCDSeg + 17) |= 0x80;		//"当前"					
					Len = 4;											
					ErrorByte = 0;	
				}						
				else return ErrorByte;
			}
			else return ErrorByte;
		}
		else
		{	
			switch( ComInfo & Comm_Type )
			{
#if ( LocalSmartMeter == YesCheck )
				case SPECFUN:
					if( ComID == 0x202C0201 ) 							//当前剩余金额
					{
						GetReOverEC( Ptr, Ptr+4 );						//剩余金额
						RAM_Write( (unsigned char*)&Value, Ptr, 4 );
						Long_BCD4( Ptr, Value );
//						DisPoint( LCDSeg, Digit2 );		//二位小数
						*(LCDSeg+20) |= 0x02;			//"剩余"					//15.07.22			
   						*(LCDSeg+6) |= 0x08;			//"."
  						*(LCDSeg+21) |= 0x21;			//"电费"
			  			*(LCDSeg+29) |= 0x08;			//"元"
						*(LCDSeg + 17) |= 0x80;			//"当前"
						Len = 4;					
					}
					else if( ComID == 0x401E0201 ) 						//报警金额1
					{
						E2P_RData( Ptr, AlarmEC1, 4 );
//						DisPoint( LCDSeg, Digit2 );		//二位小数
   						*(LCDSeg+6) |= 0x08;			//"."
						*(LCDSeg + 29) |= 0x08;		//"元"	
						Len = 4;					
					}
					else if( ComID == 0x401E0202 ) 						//报警金额2
					{
						E2P_RData( Ptr, AlarmEC2, 4 );
//						DisPoint( LCDSeg, Digit2 );		//二位小数
   						*(LCDSeg+6) |= 0x08;			//"."
						*(LCDSeg + 29) |= 0x08;		//"元"	
						Len = 4;					
					}
					else return ErrorByte;
					ErrorByte = 0;	
					break;
#endif
				case OTHER:
					if( ComID == 0x40000200 ) 							//当前时间
					{
						if(Disk.CurDisObj.No == 0x00 )
						{
							RAM_Write( Ptr, Clk.Day, 3 );				//当前日期	
							*(LCDSeg + 17) |= 0x80;			//"当前"
							*(LCDSeg + 31) |= 0x11;				//"时间"
							DisPoint( LCDSeg, Fymd );			//年月日
							*(LCDSeg+15) = *(LCDSeg+7);				//数码9移位
						}
						else if(Disk.CurDisObj.No == 0x01 )
						{
							RAM_Write( Ptr, Clk.Sec, 3 );				//当前时间	
							*(LCDSeg + 17) |= 0x80;			//"当前"
							*(LCDSeg + 31) |= 0x11;				//"时间"
							DisPoint( LCDSeg, Fhms );			//时分秒	
						}
						else return ErrorByte;			
						Len = 3;				
					}	
					else if( ComID == 0x40100200 ) 						//计量元件数	
					{
#if ( LinkMode == Phase3Wire4 )
						*Ptr = 0x03;		
#else
						*Ptr = 0x02;		
#endif						
						Len = 1;				
					}	
					else if( ComID == 0x41020200 )
					{
						*Ptr = 80;	 									//校表脉冲宽度时间	
						Len = 1;				
					}	
					else if( ComID == 0x80000300 ) 						//继电器输出状态
					{
						return ErrorByte;						
					}
					else if( ComID == 0x80000400 ) 						//继电器输出状态
					{
						return ErrorByte;
					}
					else if( ComID == 0x80000500 ) 						//继电器输出状态
					{
						return ErrorByte;
					}
					else 
					{
						return ErrorByte;
					}	
					ErrorByte = 0;	
					break;
				case PARAM:
//					Addr = DL698_ClassId8_Tab[i].Addr;
//					if(( DataType == D_Bit_String )||( DataType == D_VisibleStr )||( DataType == D_OctetStr))
					if(( DataType == D_Bit_String )||( DataType == D_VisibleStr ))
					{
						return ErrorByte;
					}
					else
					{
						if( ComID == 0x401C0200 )		//电压变比
						{
//							E2P_RData( Ptr, VoltRate, 3 );
							E2P_RData( (unsigned char*)&Value, VoltRate, 4 );
							Long_BCD4( Ptr, Value );
//							DisPoint( LCDSeg, Digit2 );		//二位小数
	   						*(LCDSeg+6) |= 0x08;			//"."
							Len = 4;
						}
						else if( ComID == 0x401D0200 )		//电流变比
						{
//							E2P_RData( Ptr, CurrentRate, 3 );
							E2P_RData( (unsigned char*)&Value, CurrentRate, 4 );
							Long_BCD4( Ptr, Value );
//							DisPoint( LCDSeg, Digit2 );		//二位小数
	   						*(LCDSeg+6) |= 0x08;			//"."
							Len = 4;
						}		
						else if( ComID == 0x41090200 )		//有功常数
						{
							E2P_RData( (unsigned char*)&Value, Meter_PConst, 4 );
							Long_BCD4( Ptr, Value );
							Len = 4;
						}		
						else if( ComID == 0x410A0200 )		//无功常数
						{
							E2P_RData( (unsigned char*)&Value, Meter_QConst, 4 );
							Long_BCD4( Ptr, Value );
							Len = 4;
						}		
						else if( ComID == 0x40010200 )		//通讯地址
						{
							E2P_RData( Ptr, EMeter_Addr, 6 );
							if(Disk.CurDisObj.No == 0x00 )
							{
								RAM_Write( Ptr, Ptr+4, 2 );
								Len = 2;	
							}
							else
							{
								Len = 4;									
							}
						}
						else return ErrorByte;	
						
//						DspNum( Ptr, Len, LCDSeg+7 );	//主显示内容				
//						*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
//						*Ptr = 0;
//						DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
//   						if( ComID == 0x40000200 )	Flag.Disk |= F_DspALT;		//时钟
//   						else Flag.Disk &= ~F_DspALT;
//						ErrorByte = 0;
					}		
					ErrorByte = 0;	
					break;
				default: 
					
					break;
			}	
		}		
		if( ErrorByte == 0 )
		{
			DspNum( Ptr, Len, LCDSeg+7 );	//主显示内容				
			*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
			*Ptr = Disk.CurDisObj.No;
			DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
			if(( ComID == 0x40000200 )||( ComID == 0x202C0201 )||( OICode == 0x4018 )||( OICode == 0x401A ))	Flag.Disk |= F_DspALT;		//时钟
			else Flag.Disk &= ~F_DspALT;
			
			if(( ComID == 0x401C0200 )||( ComID == 0x401D0200 )||( ComID == 0x41090200 )||( ComID == 0x410A0200 ))
			{
				ZeroDark( LCDSeg );			//高位去零
			}	
#if ( LocalSmartMeter == YesCheck )
			if(( OICode == 0x202C )||( OICode == 0x4018 )||( OICode == 0x401A )||( OICode == 0x401E ))
			{
				ZeroDark( LCDSeg );			//高位去零				
			}	
#endif			
		}	
	}
	return ErrorByte;	
}

//取PhaseEvent类数据
unsigned char Dis_GetEventClassData( unsigned char* LCDSeg ) 
{
	short i;
//	unsigned char* Addr;
//	short Len;
//	unsigned short OICode;
//	short Temp;
//	short Start,End;
//	unsigned char Buff[10];
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
//	unsigned char DataType;
//	unsigned char Element_Len;			
	unsigned char* OAD;
//	unsigned char* Ptr;
  	unsigned char ErrorByte=1;
//	unsigned long ComID;
//	unsigned long Value;
	
//	Len = -1;
	
//	Ptr = Buff;

	OAD = Disk.CurDisObj.OAD;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
//	ComID = SwapCharToLong( OAD );
				
	i = GetEventClassTabItemNo( OAD );
	if( i != DL698_Event_ProfileSum )
	{
		switch( Attribute )
		{
			case 1:										//logical_name
				break;	
			case 2:										//	
				ErrorByte = Dis_GetEventBufferData( LCDSeg, i );
				break;	
			case 3:										//关联对象属性表
				break;	
			case 4:										//	
				break;	
			case 5:										//配置参数	
				break;
			case 6:										//	
				break;
			case 7:										//	
				break;
			case 8:										//
				break;
			case 9:										//
				break;
/*			case 13:									//
				if( ComID == 0x30000D01 )				//失压发生总次数
				{
					E2P_RData( (unsigned char*)&Value, PhFailCuntZ, 4 );
					Long_BCD4( Ptr, Value );
					Len = 4;
				}
				else if( ComID == 0x30000D02 )			//失压总累计时间
				{
					E2P_RData( (unsigned char*)&Value, PhFailTimeZ, 4 );
					Long_BCD4( Ptr, Value );
					Len = 4;
				}		
				else if( ComID == 0x30000D03 )			//最后一次失压发生时间
				{
					E2P_RData( Ptr, PhFailStartTime, 7 );
					if( Disk.CurDisObj.No == 0 )		//日期
					{
						Len = *(Ptr+6);
						Len *= 256;
						Len += *(Ptr+5); 
						*(Ptr+5) = Len % 2000;						
						RAM_Write( Ptr, Ptr+3, 3 );
						*Ptr = Byte_BCD( *Ptr );
						*(Ptr+1) = Byte_BCD( *(Ptr+1) );
						*(Ptr+2) = Byte_BCD( *(Ptr+2) );
						*(LCDSeg + 31) |= 0x11;				//"时间"
						DisPoint( LCDSeg, Fymd );			//年月日
					}
					else
					{
						*Ptr = Byte_BCD( *Ptr );
						*(Ptr+1) = Byte_BCD( *(Ptr+1) );
						*(Ptr+2) = Byte_BCD( *(Ptr+2) );						
						*(LCDSeg + 31) |= 0x11;				//"时间"
						DisPoint( LCDSeg, Fhms );			//时分秒	
					}		
					Len = 3;
				}		
				else if( ComID == 0x30000D04 )			//最后一次失压结束时间
				{
					E2P_RData( Ptr, PhFailEndTime, 7 );
					if( Disk.CurDisObj.No == 0 )
					{
						RAM_Write( Ptr, Ptr+3, 3 );
						*(LCDSeg + 31) |= 0x11;				//"时间"
						DisPoint( LCDSeg, Fymd );			//年月日
					}
					else
					{
						*(LCDSeg + 31) |= 0x11;				//"时间"
						DisPoint( LCDSeg, Fhms );			//时分秒							
					}	
					Len = 3;
				}
									
				DspNum( Ptr, Len, LCDSeg+7 );	//主显示内容				
				*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
				*Ptr = Disk.CurDisObj.No;
				DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
   				if( ComID == 0x40000200 )	Flag.Disk |= F_DspALT;		//时钟
   				else Flag.Disk &= ~F_DspALT;
				ErrorByte = 0;
				break;
*/
			default:
				break;	
		}	
	}		
	return ErrorByte;
}	



unsigned char Dis_GetEventBufferData( unsigned char* LCDSeg, short ItemNo )
{
   short GetLen;
	short Len;
  unsigned char Buff[50];
	unsigned char Buff2[8];
	unsigned char* Ptr;	
    unsigned long LongBuff[8];
	CURVEPARA* CurvePara;
	long LoadAddr;
	unsigned char* OAD;
//	unsigned short GetLen=0;
	unsigned char OADItems;
  	unsigned char ErrorByte=1;
  	unsigned char DataType;
  	long Value;
	short FlashState = 0;	
	unsigned short OICode;
	unsigned short NullItems=0;						//不存在OAD数量					//17.05.05

	CurvePara = (CURVEPARA*)LongBuff;
	
	CurvePara->ChannelNo = ItemNo;
	GetCurvePara( CurvePara );
	
	Len = 4;
	
	Ptr = Buff;
	OAD = Buff2;
	RAM_Write( OAD, (unsigned char*)Disk.CurDisObj.ROAD, 4 );
	OICode = SwapCharToShort( OAD );
	if(( OICode == 0x201E )||( OICode == 0x2020 )) Len = 3;
//	*(OAD+2) &= ~0xE0;											//去掉属性特征			//事件类数据不能去掉属性特征，否则在事件记录中搜不到对应的OAD	//17.03.20
//	if( *OAD != 00 ) return ErrorByte;							//非电量不支持	
	if(( *OAD != 00 )&&( *OAD != 0x20 )) return ErrorByte;		//非电量或瞬时量不支持	//17.03.18			
//	if( *(OAD+3) == 00 ) return ErrorByte;						//大包电量不支持
	if(( *OAD == 00 )&&( *(OAD+3) == 00 )) return ErrorByte;	//大包电量不支持		//17.03.18
	
	if( Disk.CurDisObj.OAD[3] == 0 ) return ErrorByte;			//上几次次数非法

	if((Flag.Power & F_PwrUp) == 0)
	{
		if(( HT_GPIOB->PTDAT & GPIOB_VMEM_CTL ) != 0 )
		{
			__NOP();								//若Flash处于关闭状态，打开FLASH，并增加若干延时。
			FlashState = 1;	
			OpenFlashIO();
			HT_FreeDog();
			Delay(10000);
		}	
	}	

	LoadAddr = LoadPtrChk( CurvePara, Disk.CurDisObj.OAD[3]-1 );
	if( LoadAddr == -1 )
	{
//		RAM_Fill( Ptr, 4 );										//事件内容未产生
		RAM_Fill( Ptr, 20 );									//事件内容未产生
	}
	else
	{		
		OADItems = 1;											//只查找一项
//		GetLen = GetEventBufferRCSDData(  Ptr, OAD, OADItems, CurvePara, LoadAddr, ItemNo );
		GetLen = GetEventBufferRCSDData(  Ptr, OAD, OADItems, CurvePara, LoadAddr, ItemNo, &NullItems );		//17.05.05
//		if( *Ptr == null_data )
		if(( *Ptr == null_data )&&( *OAD == 00 ))				//电量数据当单项未找到，可通过包寻找
		{
			*(OAD+3) = 0x00;									//单项未找到，通过包寻找
//			GetLen = GetEventBufferRCSDData(  Ptr, OAD, OADItems, CurvePara, LoadAddr, ItemNo );
			GetLen = GetEventBufferRCSDData(  Ptr, OAD, OADItems, CurvePara, LoadAddr, ItemNo, &NullItems );	//17.05.05
			if( *Ptr == null_data ) DataType = 0xFF;
			else if( *Ptr != Data_array ) 
			{
				CloseFlashIOState( FlashState );	//17.03.18
				return ErrorByte;			//未找到电量项
			}	
			else
			{
				if( Disk.CurDisObj.ROAD[3] > *(Ptr+1) ) 
				{
					CloseFlashIOState( FlashState );	//17.03.18
					return ErrorByte;	//费率数超
				}	
				DataType = *(Ptr+1+(Disk.CurDisObj.ROAD[3]-1)*5);
				RAM_Write( Ptr, Ptr+2+(Disk.CurDisObj.ROAD[3]-1)*5, 4 );
			}	
		}
		else			
		{
			DataType = *Ptr;
//			RAM_Write( Ptr, Ptr+1, 4 );							
			RAM_Write( Ptr, Ptr+1, GetLen-1 );							
		}
		if( DataType == 0xFF ) 
		{
			RAM_Fill( Ptr, 4);				//事件内容未产生
			if( *OAD == 00 ) DataType = D_Unsigned32;
		}	
		if(( DataType == D_Integer32 )||( DataType == D_Unsigned32 ))
		{
			SwapData( Ptr, 4 );
			RAM_Write( (unsigned char*)&Value, Ptr, 4 );
			if( DataType == D_Integer32 )
			{
				if( Value < 0 )
				{
					Value = 0 - Value;	
					*LCDSeg |= 0x08; 			//显示符号位	
				}
			}
			Long_BCD4( Ptr, Value );
		}	
		else if( DataType == D_date_time_s )
		{
			Len = 3;
		}
		else 
		{
			CloseFlashIOState( FlashState );	//17.03.18
			return ErrorByte;									//类型不符
		}	

//		Long_BCD4( Ptr, Value );
//		if( DataType == D_Integer32 )
//		{
//			if( *(Ptr+3) & 0x80 )
//			{
//				*(Ptr+3) &= ~0x80; 			//屏蔽符号位
//				*LCDSeg |= 0x08; 			//显示符号位	
//			}	
//		}
	}			
	
//	if(FlashState != 0 )			//若原Flash处于关闭状态，操作完延时后关闭FLASH电源。
//	{
//		HT_FreeDog();
//		Delay(10000);
//		HT_FreeDog();
//		Delay(10000);
//		CloseFlashIO();	
//	}	
	CloseFlashIOState( FlashState );	//17.03.18

	if(( DataType == D_date_time_s )||( OICode == 0x201E )||( OICode == 0x2020 ))
	{
		SwapData( Ptr, 7 );
		if( Disk.CurDisObj.No == 0 )		//日期
		{					
			ChangeTimeHexToBCD( Ptr, Ptr );
			RAM_Write( Ptr, Ptr+3, 3 );
			*(LCDSeg + 31) |= 0x11;				//"时间"
			DisPoint( LCDSeg, Fymd );			//年月日
		}
		else
		{
			*Ptr = Byte_BCD( *Ptr );
			*(Ptr+1) = Byte_BCD( *(Ptr+1) );
			*(Ptr+2) = Byte_BCD( *(Ptr+2) );						
			*(LCDSeg + 31) |= 0x11;				//"时间"
			DisPoint( LCDSeg, Fhms );			//时分秒	
		}		
	}

	if( *OAD == 0 )
	{
		if(( *(OAD+2) & 0x1F ) == 2 ) GetDigitEC( Ptr );	//属性2电量	
	}	
	
//	DspNum( Ptr, 4, LCDSeg+7 );			//主显示内容				
	DspNum( Ptr, Len, LCDSeg+7 );			//主显示内容				
	*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
	*Ptr = Disk.CurDisObj.No;
	DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
	DisLastN( LCDSeg );
	if( *OAD == 0 )						//电量
	{
//		if(( *(OAD+2) & 0x1F ) == 2 ) DigitScaler( LCDSeg, 0xFE );	
//		else if(( *(OAD+2) & 0x1F ) == 4 ) *(LCDSeg+4) |= 0x08;		//精确电能4位小数	
		if(( *(OAD+2) & 0x1F ) == 4 ) *(LCDSeg+4) |= 0x08;		//精确电能4位小数	
	}	

	ErrorByte = 0;

	return ErrorByte;	
}

//取PhaseEvent类数据
unsigned char Dis_GetPhaseEventClassData( unsigned char* LCDSeg ) 
{
	short i;
	short Len;
//	unsigned short OICode;
	unsigned char Buff[10];
	unsigned char Attribute;
//	unsigned char Feature;
//	unsigned char Index;
	unsigned char* OAD;
	unsigned char* Ptr;
  	unsigned char ErrorByte=1;
	unsigned long ComID;
	unsigned long Value;
	
	Len = -1;
	
	Ptr = Buff;

	OAD = Disk.CurDisObj.OAD;
//	OICode = SwapCharToShort( OAD );
	Attribute = *(OAD+2) & 0x1F;
//	Feature = (*(OAD+2) & 0xE0) >> 5;
//	Index = *(OAD+3);
	ComID = SwapCharToLong( OAD );
				
	i = GetEventClassTabItemNo( OAD );
	if( i != DL698_Event_ProfileSum )
	{
		switch( Attribute )
		{
			case 1:										//logical_name
				break;	
			case 2:										//	
				break;	
			case 3:										//关联对象属性表
				break;	
			case 4:										//	
				break;	
			case 5:										//配置参数	
				break;
			case 6:										//	
			case 7:										//	
			case 8:										//
			case 9:										//
//				ErrorByte = Dis_GetEventBufferData( LCDSeg, i );
				ErrorByte = Dis_GetEventBufferData( LCDSeg, i+Attribute-6 );
				break;
			case 13:									//
				if( ComID == 0x30000D01 )				//失压发生总次数
				{
					E2P_RData( (unsigned char*)&Value, PhFailCuntZ, 4 );
					Long_BCD4( Ptr, Value );
					Len = 4;
					*(LCDSeg + 22) |= 0x80;				//"总"
				}
				else if( ComID == 0x30000D02 )			//失压总累计时间
				{
					E2P_RData( (unsigned char*)&Value, PhFailTimeZ, 4 );
					Long_BCD4( Ptr, Value );
					Len = 4;
					*(LCDSeg + 22) |= 0x80;				//"总"
					*(LCDSeg + 31) |= 0x11;				//"时间"
				}		
				else if( ComID == 0x30000D03 )			//最后一次失压发生时间
				{
					E2P_RData( Ptr, PhFailStartTime, 7 );
					if( Disk.CurDisObj.No == 0 )		//日期
					{
//						Len = *(Ptr+6);
//						Len *= 256;
//						Len += *(Ptr+5); 
//						*(Ptr+5) = Len % 2000;						
//						RAM_Write( Ptr, Ptr+3, 3 );
//						*Ptr = Byte_BCD( *Ptr );
//						*(Ptr+1) = Byte_BCD( *(Ptr+1) );
//						*(Ptr+2) = Byte_BCD( *(Ptr+2) );
						
						ChangeTimeHexToBCD( Ptr, Ptr );
						RAM_Write( Ptr, Ptr+3, 3 );
						*(LCDSeg + 31) |= 0x11;				//"时间"
						DisPoint( LCDSeg, Fymd );			//年月日
					}
					else
					{
						*Ptr = Byte_BCD( *Ptr );
						*(Ptr+1) = Byte_BCD( *(Ptr+1) );
						*(Ptr+2) = Byte_BCD( *(Ptr+2) );						
						*(LCDSeg + 31) |= 0x11;				//"时间"
						DisPoint( LCDSeg, Fhms );			//时分秒	
					}		
					Len = 3;
				}		
				else if( ComID == 0x30000D04 )			//最后一次失压结束时间
				{
					E2P_RData( Ptr, PhFailEndTime, 7 );
					if( Disk.CurDisObj.No == 0 )
					{
						ChangeTimeHexToBCD( Ptr, Ptr );
						RAM_Write( Ptr, Ptr+3, 3 );
//						Len = *(Ptr+6);
//						Len *= 256;
//						Len += *(Ptr+5); 
//						*(Ptr+5) = Len % 2000;						
//						RAM_Write( Ptr, Ptr+3, 3 );
//						*Ptr = Byte_BCD( *Ptr );
//						*(Ptr+1) = Byte_BCD( *(Ptr+1) );
//						*(Ptr+2) = Byte_BCD( *(Ptr+2) );
						*(LCDSeg + 31) |= 0x11;				//"时间"
						DisPoint( LCDSeg, Fymd );			//年月日
					}
					else
					{
						*Ptr = Byte_BCD( *Ptr );
						*(Ptr+1) = Byte_BCD( *(Ptr+1) );
						*(Ptr+2) = Byte_BCD( *(Ptr+2) );
						*(LCDSeg + 31) |= 0x11;				//"时间"
						DisPoint( LCDSeg, Fhms );			//时分秒							
					}	
					Len = 3;
				}
									
				DspNum( Ptr, Len, LCDSeg+7 );	//主显示内容				
				*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
				*Ptr = Disk.CurDisObj.No;
				DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
				*(LCDSeg + 22) |= 0x12;			//"失压"	
				if(( ComID == 0x30000D01 )||( ComID == 0x30000D02 ))
				{
					Flag.Disk |= F_DspALT;
					ZeroDark( LCDSeg );			//高位去零
				}else Flag.Disk &= ~F_DspALT;
				ErrorByte = 0;
				break;
			default:
				break;	
		}	
	}		
	return ErrorByte;
}	

//取Freeze类数据
unsigned char Dis_GetFreezeClassData( unsigned char* LCDSeg  ) 
{
	short i;
	unsigned char Attribute;
	unsigned char* OAD;
  	unsigned char ErrorByte=1;
	
	OAD = Disk.CurDisObj.OAD;
	Attribute = *(OAD+2) & 0x1F;
					
	i = GetFreezeClassTabItemNo( OAD );
	if( i != DL698_Freeze_ProfileSum )
	{
		switch( Attribute )
		{
			case 1:										//logical_name
				break;	
			case 2:										//	
				ErrorByte = Dis_GetFreezeBufferData( LCDSeg, i );
				break;	
			case 3:										//关联对象属性表
				break;	
			case 4:										//配置参数	
				break;	
			default:
				break;	
		}	
	}		
	return ErrorByte;
}		

unsigned char  Dis_GetFreezeBufferData( unsigned char* LCDSeg, short ItemNo )
{
	unsigned char Buff[12];
	unsigned char Buff2[8];
	unsigned char Buff3[100];
	unsigned char Buff4[8];
	unsigned char FreezeTime[8];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char* OAD;
	unsigned char* NewOAD;
	unsigned short Items;
	unsigned short Addr;
	unsigned short GetLen=0;
  	unsigned char ErrorByte=1;
  	unsigned short Temp;
    unsigned char DataType;
    short Len;
  	long Value;
	short i;
	
	Point = Buff;
	NewOAD = Buff2;
	Ptr = Buff3;
	
    OAD = Buff4;
    RAM_Fill( FreezeTime, 8 );
	RAM_Write( OAD, Disk.CurDisObj.ROAD, 4 );
	*(OAD+2) &= ~0xE0;											//去掉属性特征
	RAM_Write( NewOAD, OAD, 4 );
	if( *(OAD+3) == 0x00 ) return ErrorByte;
	else *(NewOAD+3) = 0x00;

	Items = GetFreezeCaptureItems( ItemNo );
	Addr = DL698_Freeze_ProfileTab[ItemNo].CapObjAddr;

	if( Disk.CurDisObj.OAD[3] == 0 ) return ErrorByte;			//上几次次数非法
	for( i=0;i<Items;i++ )
	{
		E2P_RData( Point, Addr+i*10, 9 );
		if(( Data_Comp( Point+2, OAD, 4 ) == 0 )||( Data_Comp( Point+2, NewOAD, 4 ) == 0 ))  
		{
			Dis_Get_single_freeze_content( Ptr, (unsigned char*)FreezeTime, &GetLen, ItemNo, i, Disk.CurDisObj.OAD[3]-1 );
			ChangeTimeHexToBCD( FreezeTime, FreezeTime );
			break;
		}	
	}
	
	if( i == Items ) return ErrorByte;

	switch( GetOADClassId( OAD ) )
	{
		case Class_id_EC:
			if( *Ptr == null_data )
			{
				RAM_Fill( Ptr, 4);				//事件内容未产生
				DataType = D_Unsigned32;
			}
			else if( *Ptr == Data_array )
			{
				if( Disk.CurDisObj.ROAD[3] > *(Ptr+1) ) return ErrorByte;	//费率数超
//				DataType = *(Ptr+1+(Disk.CurDisObj.ROAD[3]-1)*5);
				DataType = *(Ptr+2+(Disk.CurDisObj.ROAD[3]-1)*5);
//				RAM_Write( Ptr, Ptr+2+(Disk.CurDisObj.ROAD[3]-1)*5, 4 );
				RAM_Write( Ptr, Ptr+3+(Disk.CurDisObj.ROAD[3]-1)*5, 4 );
			}
			else			
			{
				DataType = *Ptr;
				RAM_Write( Ptr, Ptr+1, 4 );							
			}
			if( DataType == 0xFF ) 
			{
				RAM_Fill( Ptr, 4);				//事件内容未产生
				DataType = D_Unsigned32;
			}	
			SwapData( Ptr, 4 );
			RAM_Write( (unsigned char*)&Value, Ptr, 4 );
			if( DataType == D_Integer32 )
			{
				if( Value < 0 )
				{
					Value = 0 - Value;	
				}
			}
			else if( DataType == D_Unsigned32 )
			{
				;
			}else return ErrorByte;									//类型不符
        	
			Long_BCD4( Ptr, Value );
			if( DataType == D_Integer32 )
			{
				if( *(Ptr+3) & 0x80 )
				{
					*(Ptr+3) &= ~0x80; 			//屏蔽符号位
					*LCDSeg |= 0x08; 			//显示符号位	
				}	
			}
			if( *(OAD+2) == 0x02 ) GetDigitEC( Ptr );	//属性2电量
			Len = 4;
			DspNum( Ptr, Len, LCDSeg+7 );			//主显示内容
//			DigitScaler( LCDSeg, 0xFE );				
			ErrorByte = 0;			
			break;					
		case Class_id_MD:					
			if( *Ptr == null_data )
			{
				RAM_Fill( Ptr, 15);				//事件内容未产生
//				DataType = D_Unsigned32;
			}
			else if( *Ptr == Data_array )
			{
				if( Disk.CurDisObj.ROAD[3] > *(Ptr+1) ) return ErrorByte;	//费率数超
				DataType = *(Ptr+4+(Disk.CurDisObj.ROAD[3]-1)*15);
				RAM_Write( Ptr, Ptr+4+(Disk.CurDisObj.ROAD[3]-1)*15, 13 );
			}
			else			
			{
				RAM_Write( Ptr, Ptr+2, 13 );							
				DataType = *Ptr;
			}
			if( DataType == 0xFF ) 
			{
				*Ptr = D_Unsigned32;
				RAM_Fill( Ptr+1, 14);				//事件内容未产生
			}	
			if( Disk.CurDisObj.No == 0 )		//需量值
			{
				DataType = *Ptr;
				SwapData( Ptr+1, 4 );
				RAM_Write( (unsigned char*)&Value, Ptr+1, 4 );
				if( DataType == D_Integer32 )
				{
					if( Value < 0 )
					{
						Value = 0 - Value;	
					}
					*LCDSeg |= 0x08; 			//显示符号位
				}
				Long_BCD4( Ptr, Value );	
				Len = 4;
			}
			else if( Disk.CurDisObj.No == 1 )	//需量发生日期	
			{
				Temp = SwapCharToShort( Ptr+6 ) % 100;
				*(Ptr+2) = Temp;
				*(Ptr+2) = Byte_BCD( *(Ptr+2) );	//17.05.05			
				*(Ptr+1) = Byte_BCD( *(Ptr+8) );
				*Ptr = Byte_BCD( *(Ptr+9) );
				Len = 3;
				DisPoint( LCDSeg, Fymd );			//年月日
			}
			else								//需量发生时间
			{
//				*(Ptr+2) = Byte_BCD( *(Ptr+10) );
//				*(Ptr+1) = Byte_BCD( *(Ptr+11) );
//				*Ptr = Byte_BCD( *(Ptr+12) );				
//				Len = 3;
//				DisPoint( LCDSeg, Fhms );			//时分秒							
				*(Ptr+1) = Byte_BCD( *(Ptr+10) );
				*Ptr = Byte_BCD( *(Ptr+11) );
				Len = 2;
				DisPoint( LCDSeg, Fhm );			//时分						
			}	
			DspNum( Ptr, Len, LCDSeg+7 );			//主显示内容	
			ErrorByte = 0;			
			break;				
		default:
		 	break;
	}	
	
	if( ErrorByte == 0 )
	{
		*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
		DisLastN( LCDSeg );
		*Ptr = Disk.CurDisObj.No;		
#if ( JiangSuRequire == YesCheck )
		if(( Disk.CurDisObj.OAD[0] == 0x50 )&&( Disk.CurDisObj.OAD[1] == 0x03 )) 		//上24整点冻结小数码显示冻结时间
		{
			LCDSeg[7] = 0;
//			RAM_Write( Buff, Disk.CurDisObj.OAD, 4 );
//			SwapData( Buff, 4 );
			DspNum( FreezeTime+1, 4, &LCDSeg[14] );
			DisPoint( &LCDSeg[7], Fdisi );			
		}
		else DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
#else
		DspNum( Ptr, 1, LCDSeg+24 );		//项内序号
						
#endif		
	}
	return ErrorByte;
}

/*
void Dis_Get_single_freeze_content( unsigned char* DestPtr, unsigned short* GetLen, unsigned short ItemNo, unsigned short CurveNo, long LoadRecNo )
{
	unsigned char Buff2[14];
//	unsigned char* Point;
	unsigned char* FileName;
    unsigned long LongBuff[8];
//    unsigned long WriteLen;
	FILECURVEPARA* FileCurvePara;
	unsigned char ErrorByte=0;
        short Len;
	long LoadAddr;
	
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
	ErrorByte = f_open( &fsrc, (char const*)FileName, FA_READ);		//此处打开文件后，在Read_File中不再每次打开，避免多次打开，除非读取备用文件后恢复打开
	if( ErrorByte == FR_OK )
	{
		LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
		if( LoadAddr != -1 )
		{				
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
	else Fill_null_data( DestPtr, GetLen );
	ErrorByte = f_close(&fsrc);
	return;								
}
*/

void Dis_Get_single_freeze_content( unsigned char* DestPtr, unsigned char* FreezeTime, unsigned short* GetLen, unsigned short ItemNo, unsigned short CurveNo, long LoadRecNo )
{
	unsigned char Buff2[14];
//	unsigned char* Point;
	unsigned char* FileName;
    unsigned long LongBuff[8];
//    unsigned long WriteLen;
	FILECURVEPARA* FileCurvePara;
	unsigned char ErrorByte=0;
        short Len;
	long LoadAddr;
	long Offset;							//16.10.12
	short FlashState = 0;	
	
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

	if((Flag.Power & F_PwrUp) == 0)
	{
		if(( HT_GPIOB->PTDAT & GPIOB_VMEM_CTL ) != 0 ) 
		{
			__NOP();								//若Flash处于关闭状态，打开FLASH，并增加若干延时。
			FlashState = 1;	
			OpenFlashIO();
			HT_FreeDog();
			Delay(10000);
		}	
	}	
	
	Offset = 0;							//16.10.12
	CreatFileName( FileName, FileCurvePara->ChannelNo>>8, FileCurvePara->OAD, 0 );
//	ErrorByte = f_open( &fsrc, (char const*)FileName, FA_READ);		//此处打开文件后，在Read_File中不再每次打开，避免多次打开，除非读取备用文件后恢复打开
//	ErrorByte = Open_File( FileCurvePara, FileName );						//此处打开文件后，在Read_File中不再每次打开，避免多次打开，除非读取备用文件后恢复打开
	GetDirTo_FBuff();							//将文件目录表放入FBuff,加速后续文件打开操作	//17.03.08
	ErrorByte = Open_File_In_FBuff( FileCurvePara, FileName );		//根据RAM(FBuff)中的文件目录表打开文件	//17.03.08
	if( ErrorByte == FR_OK )
	{
		LoadAddr = LoadFilePtrChk( FileCurvePara, LoadRecNo );
		if( LoadAddr != -1 )
		{	
			Read_File( FreezeTime, FileCurvePara, LoadAddr, Offset, 7 );			
//			LoadAddr += 11;								//数据起始地址	
			Offset = 11;								//数据起始地址	
			Len = FileCurvePara->RecdLenth - 11;		//有效数据长度（带校验的包含3个校验字节）
			if( FileCurvePara->BakFlag == 1 )			//带校验
			{
//				LoadAddr += 2;							//加冻结时间和冻结序号的校验和
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
	else Fill_null_data( DestPtr, GetLen );
//	ErrorByte = f_close(&fsrc);

	if(FlashState != 0 )			//若原Flash处于关闭状态，操作完延时后关闭FLASH电源。
	{
		HT_FreeDog();
		Delay(10000);
		HT_FreeDog();
		Delay(10000);
		CloseFlashIO();	
	}	

	return;								
}

//读数据输入输出设备类数据，返回值：-1，无此数据项，非-1：返回数据长度
unsigned char Dis_GetClass22Data( unsigned char* LCDSeg )
{
//	unsigned char Attribute;
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned char* OAD;
  	unsigned char ErrorByte=1;
	unsigned long ComID;
	
	Ptr = Buff;
	OAD = Disk.CurDisObj.OAD;
//	Attribute = *(OAD+2) & 0x1F;
	ComID = SwapCharToLong( OAD );
	
	if( ComID == 0xF2010201 )
	{
		Word_BCD( Ptr, GetBandRate( Comm.BandRate1 ) );
	}
#if ( CarryComm == YesCheck )			//带载波通道与485口2共用通讯缓存
	else if( ComID == 0xF2090201 )
	{
		Word_BCD( Ptr, GetBandRate( Comm.BandRate2 ) );			
	}		
#else
	else if( ComID == 0xF2010202 )
	{
		Word_BCD( Ptr, GetBandRate( Comm.BandRate2 ) );			
	}
#endif	
	else return ErrorByte;		
	
	DspNum( Ptr, 2, LCDSeg+7 );			//主显示内容				
	*(LCDSeg+15) = *(LCDSeg+7);			//数码9移位			
	*Ptr = 0;
	DspNum( Ptr, 1, LCDSeg+24 );		//项内序号

	return 0;
}	
	


