#include "TypeRAM.h"
#include "TypeE2p.h"
#include "ht6xxx_lib.h"
#include "Port.h"
#include "TypeFRAM.h"
#include "Data.h"
#include "Mem.h"
#include "IECComm.h"
#include "Measure.h"					
#include "Display.h"
#include "COSEMComm.h"
#include "Demand.h"
#include "General.h"

unsigned char Lcdram_buf[18];

typedef struct
{
   unsigned char 	PINNUM;
   unsigned char 	SEGBIT;
} DISPLAYPIN;

typedef struct
{
   DISPLAYPIN 	SEGB[7];
   
} DISPLAYSEG;


//----------------------------
#define LCDRAMBUFF Lcdram_buf

const DISPLAYSEG LCD_PINBIT[6+3] = 
{
//	    a	     b	      c	       d	     e	      f	       g
 	 0,0x01,  1,0x01,  1,0x04,  0,0x08,  0,0x04,  0,0x02,  1,0x02,
	 2,0x01,  3,0x01,  3,0x04,  2,0x08,	 2,0x04,  2,0x02,  3,0x02,
	 4,0x01,  5,0x01,  5,0x04,  4,0x08,	 4,0x04,  4,0x02,  5,0x02,
	12,0x01, 13,0x01, 13,0x04, 12,0x08,	12,0x04, 12,0x02, 13,0x02,
	14,0x01, 15,0x01, 15,0x04, 14,0x08,	14,0x04, 14,0x02, 15,0x02,
	16,0x01, 17,0x01, 17,0x04, 16,0x08,	16,0x04, 16,0x02, 17,0x02,

	 6,0x08,  7,0x08,  7,0x02,  6,0x01,	 6,0x02,  6,0x04,  7,0x04,
 	 8,0x08,  9,0x08,  9,0x02,  8,0x01,  8,0x02,  8,0x04,  9,0x04,
	10,0x08, 11,0x08, 11,0x02, 10,0x01,	10,0x02, 10,0x04, 11,0x04,
};


void DISPLAYSEGX( unsigned char NUM, unsigned char BIT)
{
//	Lcdram_buf[LCD_PINBIT[NUM].SEGB[BIT].PINNUM/2] |= LCD_PINBIT[NUM].SEGB[BIT].SEGBIT;	//4comm
	LCDRAMBUFF[LCD_PINBIT[NUM].SEGB[BIT].PINNUM] |= LCD_PINBIT[NUM].SEGB[BIT].SEGBIT;	//6comm or 8com
		
}

void DISPLAYNUMX0( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBa);
	DISPLAYSEGX(NUM,SEGBb);
	DISPLAYSEGX(NUM,SEGBc);
	DISPLAYSEGX(NUM,SEGBd);
	DISPLAYSEGX(NUM,SEGBe);
	DISPLAYSEGX(NUM,SEGBf);
}
void DISPLAYNUMX1( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBb);
	DISPLAYSEGX(NUM,SEGBc);
}

void DISPLAYNUMX2( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBa);
	DISPLAYSEGX(NUM,SEGBb);	
	DISPLAYSEGX(NUM,SEGBd);
	DISPLAYSEGX(NUM,SEGBe);
	DISPLAYSEGX(NUM,SEGBg);
}

void DISPLAYNUMX3( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBa);
	DISPLAYSEGX(NUM,SEGBb);
	DISPLAYSEGX(NUM,SEGBc);
	DISPLAYSEGX(NUM,SEGBd);
	DISPLAYSEGX(NUM,SEGBg);
}

void DISPLAYNUMX4( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBb);
	DISPLAYSEGX(NUM,SEGBc);	
	DISPLAYSEGX(NUM,SEGBf);
	DISPLAYSEGX(NUM,SEGBg);
}

void DISPLAYNUMX5( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBa);
	DISPLAYSEGX(NUM,SEGBc);	
	DISPLAYSEGX(NUM,SEGBd);
	DISPLAYSEGX(NUM,SEGBf);
	DISPLAYSEGX(NUM,SEGBg);
}
void DISPLAYNUMX6( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBa);
	DISPLAYSEGX(NUM,SEGBc);	
	DISPLAYSEGX(NUM,SEGBd);
	DISPLAYSEGX(NUM,SEGBe);
	DISPLAYSEGX(NUM,SEGBf);
	DISPLAYSEGX(NUM,SEGBg);
}
void DISPLAYNUMX7( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBa);
	DISPLAYSEGX(NUM,SEGBb);
	DISPLAYSEGX(NUM,SEGBc);
}
void DISPLAYNUMX8( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBa);
	DISPLAYSEGX(NUM,SEGBb);
	DISPLAYSEGX(NUM,SEGBc);
	DISPLAYSEGX(NUM,SEGBd);
	DISPLAYSEGX(NUM,SEGBe);
	DISPLAYSEGX(NUM,SEGBf);
	DISPLAYSEGX(NUM,SEGBg);
}
void DISPLAYNUMX9( unsigned char NUM)
{
	DISPLAYSEGX(NUM,SEGBa);
	DISPLAYSEGX(NUM,SEGBb);
	DISPLAYSEGX(NUM,SEGBc);
	DISPLAYSEGX(NUM,SEGBd);
	DISPLAYSEGX(NUM,SEGBf);
	DISPLAYSEGX(NUM,SEGBg);
}

void DISPLAYNUMXX( unsigned char NUM,unsigned char VALUE)
{
	switch(VALUE)
	{
		case 0:	DISPLAYNUMX0( NUM);break;
		case 1:	DISPLAYNUMX1( NUM);break;
		case 2:	DISPLAYNUMX2( NUM);break;		
		case 3:	DISPLAYNUMX3( NUM);break;	
		case 4:	DISPLAYNUMX4( NUM);break;
		case 5:	DISPLAYNUMX5( NUM);break;
		case 6:	DISPLAYNUMX6( NUM);break;
		case 7:	DISPLAYNUMX7( NUM);break;
		case 8:	DISPLAYNUMX8( NUM);break;
		case 9:	DISPLAYNUMX9( NUM);break;
		
		default:	DISPLAYNUMX9( NUM);break;							
	}
}
void Display_Point( unsigned char Num)
{
	switch(Num)
	{
		case 0:	
			break;
		case 1:	
			LCDRAMBUFF[15] |= TW_DisP4;
			break;
		case 2:	
			LCDRAMBUFF[13] |= TW_DisP3;
			break;
		case 3:	
			LCDRAMBUFF[5] |= TW_DisP2;
			break;		
		case 4:	
			LCDRAMBUFF[3] |= TW_DisP1;
			break;	
		
		default:break;							
	}
}
void Display_KW( void)
{
	LCDRAMBUFF[3] |= TW_DisK9;
	LCDRAMBUFF[4] |= TW_DisK10|TW_DisK11;
	LCDRAMBUFF[5] |= TW_DisK12;
}
void Display_W( void)
{
	LCDRAMBUFF[4] |= TW_DisK10|TW_DisK11;
	LCDRAMBUFF[5] |= TW_DisK12;
}

void Display_KWH( void)
{
	Display_KW();
	LCDRAMBUFF[17] |= TW_DisK14|TW_DisK15;
}
void Display_Kvar( void)
{
	LCDRAMBUFF[3] |= TW_DisK9;
	LCDRAMBUFF[4] |= TW_DisK10;
	LCDRAMBUFF[5] |= TW_DisK12|TW_DisK13;
	LCDRAMBUFF[17] |= TW_DisK15;
}
void Display_VA( void)
{
	LCDRAMBUFF[4] |= TW_DisK10;
	LCDRAMBUFF[5] |= TW_DisK12|TW_DisK13;
}
void Display_VAH( void)
{
	LCDRAMBUFF[4] |= TW_DisK10;
	LCDRAMBUFF[5] |= TW_DisK12|TW_DisK13;	
	LCDRAMBUFF[17] |= TW_DisK14|TW_DisK15;
}

void Display_KvarH( void)
{
	Display_Kvar();
	LCDRAMBUFF[17] |= TW_DisK16;

}
void Display_V( void)
{
	LCDRAMBUFF[4] |= TW_DisK10;

}
void Display_A( void)
{
	LCDRAMBUFF[5] |= TW_DisK12|TW_DisK13;
}
void Display_Angle( void)
{
	LCDRAMBUFF[15] |= TW_DisP5;
}
/*
void Display_HZ( void)
{
	
	LCDRAMBUFF[36] |= 0x38;
}
*/
void DisplayUnit(unsigned char unit)
{
	switch(unit)
		{
			case Unit_Wh:
				Display_KWH();
				break;
			case Unit_varh:
				Display_KvarH();
				break;
			case Unit_VAh:
				Display_VAH();
				break;
			case Unit_wk:
				Display_KW();
				break;
			case Unit_W:
				Display_W();
				break;
			case Unit_var:
				Display_Kvar();
				break;
			case Unit_VA:
				Display_VA();
				break;
			case Unit_V:
				Display_V();
				break;
			case Unit_A:
				Display_A();
				break;
			case Unit_deg:
				Display_Angle();
				break;
		}
}


void Display_LA( void)
{
	LCDRAMBUFF[11] |= TW_DisA;
}
void Display_LB( void)
{
	LCDRAMBUFF[11] |= TW_DisB;
}
void Display_LC( void)
{
	LCDRAMBUFF[11] |= TW_DisC;
}
/*
void Display_GPRSSEG( void)
{
	switch(Disk.DisGPRS)
	{
		case 4:	
			LCDRAMBUFF[33] |= 0x20;
		case 3:	
			LCDRAMBUFF[33] |= 0x10;

		case 2:	
			LCDRAMBUFF[33] |= 0x08;
		case 1:	
			LCDRAMBUFF[32] |= 0x08;

		default:
			LCDRAMBUFF[32] |= 0x10;
			break;							
	}
}
*/
void Display_RELAYSTATE( void)
{
	if(Para.RMeterRelayState == 0x1A)	//跳闸
		{
		LCDRAMBUFF[1] |= TW_DisK4;
		LCDRAMBUFF[2] |= TW_DisK6;
		}
	else	//合闸
		{
		LCDRAMBUFF[1] |= TW_DisK4|TW_DisK5;
		}
}
void Display_COMMSTATE( void)
{
	if(Comm.RecTmr0!=0)	//载波通讯
		LCDRAMBUFF[0] |= TW_DisK2;
	else
		LCDRAMBUFF[0] &=~ TW_DisK2;
	
	if(Comm.RecTmr1!=0)	//红外通讯
		LCDRAMBUFF[1] |= TW_DisK3;
	else
		LCDRAMBUFF[1] &=~ TW_DisK3;
}

void Display_Bar( void)
{
	unsigned short Plus_Num;
	//---------清掉进度条，便于重新显示-------
	LCDRAMBUFF[12] &=~ TW_DisL2;
	LCDRAMBUFF[13] &=~ TW_DisL3;
	LCDRAMBUFF[14] &=~ TW_DisL4;
	LCDRAMBUFF[15] &=~ TW_DisL5;
	if((GetMQuad( Real.PWFlag,3 ) == 2)||(GetMQuad( Real.PWFlag,3 ) == 3))//判断条件
		{
		LCDRAMBUFF[9] |= TW_DisL1;//有功反向
		LCDRAMBUFF[16] &=~ TW_DisL6;
		Plus_Num = Measur_Const*6;
		Plus_Num/=10;
		if(ECP.PL_CumPn_Dis>Plus_Num)//72*0.6
		LCDRAMBUFF[15] |= TW_DisL5;
		Plus_Num = Measur_Const*7;
		Plus_Num/=10;
		if(ECP.PL_CumPn_Dis>Plus_Num)//72*0.7
			LCDRAMBUFF[14] |= TW_DisL4;
		Plus_Num = Measur_Const*8;
		Plus_Num/=10;
		if(ECP.PL_CumPn_Dis>Plus_Num)//72*0.8
			LCDRAMBUFF[13] |= TW_DisL3;
		Plus_Num = Measur_Const*9;
		Plus_Num/=10;
		if(ECP.PL_CumPn_Dis>Plus_Num)//72*0.9
			LCDRAMBUFF[12] |= TW_DisL2;
		if(ECP.PL_CumPn_Dis>Measur_Const)
			{
			ECP.PL_CumPn_Dis = 0;
			LCDRAMBUFF[12] &=~ TW_DisL2;
			LCDRAMBUFF[13] &=~ TW_DisL3;
			LCDRAMBUFF[14] &=~ TW_DisL4;
			LCDRAMBUFF[15] &=~ TW_DisL5;
			}
		}
	else
		{
		LCDRAMBUFF[16] |= TW_DisL6;	//有功正向
		LCDRAMBUFF[9] &=~ TW_DisL1;
		Plus_Num = Measur_Const*6;
		Plus_Num/=10;
		if(ECP.PL_CumPp_Dis>Plus_Num)//72*0.6
			LCDRAMBUFF[12] |= TW_DisL2;
		Plus_Num = Measur_Const*7;
		Plus_Num/=10;
		if(ECP.PL_CumPp_Dis>Plus_Num)//72*0.7
			LCDRAMBUFF[13] |= TW_DisL3;
		Plus_Num = Measur_Const*8;
		Plus_Num/=10;
		if(ECP.PL_CumPp_Dis>Plus_Num)//72*0.8
			LCDRAMBUFF[14] |= TW_DisL4;
		Plus_Num = Measur_Const*9;
		Plus_Num/=10;
		if(ECP.PL_CumPp_Dis>Plus_Num)//72*0.9
			LCDRAMBUFF[15] |= TW_DisL5;
		if(ECP.PL_CumPp_Dis>Measur_Const)
			{
			ECP.PL_CumPp_Dis = 0;
			LCDRAMBUFF[12] &=~ TW_DisL2;
			LCDRAMBUFF[13] &=~ TW_DisL3;
			LCDRAMBUFF[14] &=~ TW_DisL4;
			LCDRAMBUFF[15] &=~ TW_DisL5;
			}
		}
			
}
void DspNum( unsigned char *Addr, unsigned char Lenth, unsigned char* SegPtr ,unsigned char Digit)
{
	unsigned char i;
	unsigned char Len,offset;
	unsigned char* ptr;
	Len = Lenth;
	ptr = Addr;
	if(Lenth>6){
	//	offset = Lenth-6;
	//	RAM_Write(ptr,ptr+offset,6);
		Lenth = 6;
		}
	
	//	offset = 6-Lenth;
    for(i=0;i < Len; i++)
    {	
		DISPLAYNUMXX(5-i,((*(ptr+5-i))& 0x0F));		
    
/*	if((Lenth&0x01)&&(i==0))
		DISPLAYNUMXX(i+offset,(((*ptr) >>(4*((i)&0x01)))& 0x0F));		
	else
		DISPLAYNUMXX(i+offset,(((*ptr) >>(4*((i+1)&0x01)))& 0x0F));
		if(i&0x01)
			ptr++; 
*/
    }	
	Display_Point(Digit);
}
void NonDisplaySEG1(void)
{
	LCDRAMBUFF[0] &=~ (0x01+0x02+0x04+0x08);
	LCDRAMBUFF[1] &=~ (0x01+0x02+0x04);

}
void Display_FeeNo( void)
{
	LCDRAMBUFF[12] &=~ TW_DisRateA;
	LCDRAMBUFF[13] &=~ TW_DisRateB;
	LCDRAMBUFF[14] &=~ TW_DisRateC;
	LCDRAMBUFF[16] &=~ TW_DisRateD;
	
	if(SM.FeeNo==1)
		LCDRAMBUFF[12] |= TW_DisRateA;
	else if(SM.FeeNo==2)
		LCDRAMBUFF[13] |= TW_DisRateB;
	else if(SM.FeeNo==3)
		LCDRAMBUFF[14] |= TW_DisRateC;
	else if(SM.FeeNo==4)
		LCDRAMBUFF[16] |= TW_DisRateD;
}
void Display_Cum( void)
{
	LCDRAMBUFF[3] |= TW_DisK8;
}
void Display_ContCum( void)
{
	LCDRAMBUFF[2] |= TW_DisK7;
	LCDRAMBUFF[3] |= TW_DisK8;
}
void Display_ALT( void)
{
	LCDRAMBUFF[10] |= TW_DisALT;
}
void Display_EOI( void)
{
	LCDRAMBUFF[10] |= TW_DisEOI;
}
void Display_Cover( void)
{
	unsigned short Open_Num;
	E2P_RData((unsigned char *)&Open_Num,KaiGai_Num,2);
		if(Open_Num>0)
			LCDRAMBUFF[9] |= TW_DisCover;
}

void HEX4_BCDn(long tt,unsigned char *temp,unsigned char len)
{
	unsigned char i,j,n;

	for (i=0;i<len;i++)
	{	
		j=tt%100;
		n=j%10;
		j/=10;
		j=j<<4;
		j|=n;
		tt/=100;
		*temp=j;
		++temp;			
	}	 
}

short JudgeDisItemType( void )
{
	if(( Disk.Class_Id == Class_id_Data )||( Disk.Class_Id == Class_id_Register ))
	{
		if( Disk.Attribute == 2 ) return 0;
	}	
	else if( Disk.Class_Id == Class_id_Extended_register )		
	{
		if(( Disk.Attribute == 2 )||( Disk.Attribute == 5 )) return 0;
	}	
	else if( Disk.Class_Id == Class_id_IEC_HDLC_setup )
	{
		if( Disk.Attribute == 9 ) return 0;
	}		
	return -1;
}	
//读电量Register数据，返回值：-1，无此数据项，非-1：返回数据长度
short IsLocalTimeDisplayItem( void )
{
	unsigned char Buff[6];
	
	Buff[0] = 0x01;
	Buff[1] = 0x00;
	Buff[2] = 0x00;
	Buff[3] = 0x09;
	Buff[4] = 0x01;
	Buff[5] = 0xff;
	
	if(( Disk.Class_Id != Class_id_Data)||( Disk.Attribute != 2 )) return -1;
	if( Data_Comp( Buff, Disk.Logical_Name, 6 ) == 0 ) return 0;
	else return -1;	
}	
void DisLocalTime( unsigned char* LCDSeg )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	
	Ptr = Buff;
	
//	RAM_Write( Ptr, Clk.Sec, 3 );	
//	SwapData(Ptr,3);
	*Ptr=(*Clk.Hour>>4)&0x0f;
	*(Ptr+1)=(*Clk.Hour&0x0f);
	*(Ptr+2)=(*Clk.Min>>4)&0x0f;
	*(Ptr+3)=(*Clk.Min&0x0f);
	*(Ptr+4)=(*Clk.Sec>>4)&0x0f;
	*(Ptr+5)=(*Clk.Sec&0x0f);

	DspNum( Ptr, 6, LCDSeg ,0 );
///	DspNum( Ptr, 3, LCDSeg );
	Display_Point(2);
	Display_Point(4);

	Flag.Disk |= F_DspALT;
}	
short IsLocalDateDisplayItem( void )
{
	unsigned char Buff[6];
	
	Buff[0] = 0x01;
	Buff[1] = 0x00;
	Buff[2] = 0x00;
	Buff[3] = 0x09;
	Buff[4] = 0x02;
	Buff[5] = 0xff;
	
	///if(( Disk.Class_Id != Class_id_Register )||( Disk.Attribute != 2 )) return -1;
	if(( Disk.Class_Id != Class_id_Data)||( Disk.Attribute != 2 )) return -1;
	if( Data_Comp( Buff, Disk.Logical_Name, 6 ) == 0 ) return 0;
	else return -1;	
}

void DisLocalDate( unsigned char* LCDSeg )
{
	unsigned char Buff[6];
	unsigned char* Ptr;
	Ptr = Buff;
	
	//RAM_Write( Ptr, Clk.Day, 3 );
	*Ptr=(*Clk.Year>>4)&0x0f;
	*(Ptr+1)=(*Clk.Year&0x0f);
	*(Ptr+2)=(*Clk.Mon>>4)&0x0f;
	*(Ptr+3)=(*Clk.Mon&0x0f);
	*(Ptr+4)=(*Clk.Day>>4)&0x0f;
	*(Ptr+5)=(*Clk.Day&0x0f);
	DspNum( Ptr, 6, LCDSeg ,0 );	
	
	Display_Point(2);
	Display_Point(4);
///	DspNum( Ptr, 4, LCDSeg );	
	Flag.Disk |= F_DspALT;
}

//显示Data类数据
short DisDataRAMClassData( unsigned char* LCDSeg ) 
{
	unsigned char Buff[10],Buff1[6];
	unsigned char i;
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char Len;
	unsigned short temp_data;
	Ptr = Buff;
	Point = Buff1;

	i = GetRAMDataClassTabItemNo( Disk.Logical_Name );						//查找对应的OBIS码
	if( i == DLMS_RAM_DataSum ) return -1;
	else 
	{
		Ptr = DLMS_RAM_DATATab[i].Addr;
		Len = DLMS_RAM_DATATab[i].ComInfo & RS_LEN;

		if((DLMS_RAM_DATATab[i].DataType==D_Unsigned16)||(DLMS_RAM_DATATab[i].DataType==D_Integer16))
			{
		//	Word_BCD( Point,GetBuffToInteger(Ptr));
			if(i==4)
			temp_data = SM.Demand_Interval_Remain;
			Long_BCD4( Point, (unsigned long)temp_data );
			SwapData(Point,4);		
			Ptr = Buff;
			*Ptr=(*(Point+1)>>4)&0x0f;
			*(Ptr+1)=(*(Point+1)&0x0f);
			*(Ptr+2)=(*(Point+2)>>4)&0x0f;
			*(Ptr+3)=(*(Point+2)&0x0f);
			*(Ptr+4)=(*(Point+3)>>4)&0x0f;
			*(Ptr+5)=(*(Point+3)&0x0f);
			Len=6;
			
			}
		DspNum( Ptr, Len, LCDSeg ,0 );
	}		
	return 0;
}

short DisDataRegClassData( unsigned char* LCDSeg ) 
{
	unsigned char Buff[10],Buff1[6];
	unsigned char i,j;
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned short Addr,year;
	unsigned char Len;
	unsigned long Asii_Value;
	Ptr = Buff;
	if( IsLocalTimeDisplayItem() == 0 )
	{
		DisLocalTime( LCDSeg );
		return 0;		
	}
	else if( IsLocalDateDisplayItem() == 0 )
	{
		DisLocalDate( LCDSeg );
		return 0;
	}		
	//////////
	i = GetDataClassTabItemNo( Disk.Logical_Name );						//查找对应的OBIS码
	if( i == DLMSDataSum ) return -1;
	else 
	{
		Addr = DLMS_DATATab[i].Addr;
		Len = DLMS_DATATab[i].ComInfo & RS_LEN;
		if( Len > 8 ) Len = 8;
		E2P_RData( Ptr, Addr, Len );
		if(DLMS_DATATab[i].DataType==Data_Date)
			{
			//year highbyte,
			//year lowbyte,
			//month,
			//day of month,
			//day of week
			Point = Buff1;
			
			year = GetBuffToInteger(Ptr)-2000;
			*Point = Byte_BCD(year);//year
			*(Point+1) = Byte_BCD(*(Ptr+2));//month
			*(Point+2) = Byte_BCD(*(Ptr+3));//day		
			//RAM_Write(Ptr,Point,3);
			
			*Ptr=(*Point>>4)&0x0f;
			*(Ptr+1)=(*Point&0x0f);
			*(Ptr+2)=(*(Point+1)>>4)&0x0f;
			*(Ptr+3)=(*(Point+1)&0x0f);
			*(Ptr+4)=(*(Point+2)>>4)&0x0f;
			*(Ptr+5)=(*(Point+2)&0x0f);
			Display_Point(2);
			Display_Point(4);
			Len=6;
		//	SwapData(Ptr,Len);		
			}
		else if(DLMS_DATATab[i].DataType==Data_Time)
			{
			//hour,
			//minute,
			//second,
			//hundredths
			Point = Buff1;
			*Point = Byte_BCD(*(Ptr));//hour
			*(Point+1) = Byte_BCD(*(Ptr+1));//minute
			*(Point+2) = Byte_BCD(*(Ptr+2));//second
		//	RAM_Write(Ptr,Point,3);
		
		*Ptr=(*Point>>4)&0x0f;
		*(Ptr+1)=(*Point&0x0f);
		*(Ptr+2)=(*(Point+1)>>4)&0x0f;
		*(Ptr+3)=(*(Point+1)&0x0f);
		*(Ptr+4)=(*(Point+2)>>4)&0x0f;
		*(Ptr+5)=(*(Point+2)&0x0f);
			Display_Point(2);
			Display_Point(4);
			Len=6;
		//	SwapData(Ptr,Len);
			}
		else if(DLMS_DATATab[i].DataType==D_VisibleStr)
			{
			if(Len>6) 
				{
				RAM_Write(Ptr,Ptr+Len-6,6);
				Len=6;
				}
			else
				RAM_Write(Ptr+6-Len,Ptr,Len);
		/*	Asii_Value=0;
			if(Len>9) Len=9;
			for(j=0;j<Len;j++)
				{
				if(*Ptr<=0x39 && *Ptr>=0x30)
					{
						Asii_Value+=(*Ptr-0x30);
						if(j<(Len-1))
							{
							Asii_Value*=10;
							Ptr++;			
							}
					}
				}
			Ptr = Buff;
			RAM_Fill(Ptr,Len);
			if(Len>2)
				{
				Long_BCD4( Ptr,Asii_Value);
				SwapData(Ptr,4);	
				RAM_Write(Ptr,Ptr+1,3);
				}
			else if(Len==2)
				{
				Word_BCD( Ptr,(unsigned short)Asii_Value);
				SwapData(Ptr,2);
				
				}
			else
				*Ptr=Byte_BCD( (unsigned char)Asii_Value);
		//	Len/=2;
		*/
			}
		else
			{
			Point = Buff1;
			RAM_Fill(Point,6);
				if((DLMS_DATATab[i].DataType==D_Unsigned32)||(DLMS_DATATab[i].DataType==D_Integer32))
				{
				Long_BCD4( Point,GetBuffToLong(Ptr));
				//SwapData(Ptr,4);
				//RAM_Write(Ptr,Ptr+1,3);
				RAM_Fill(Ptr,6);
				*Ptr=(*(Point+1)>>4)&0x0f;
				*(Ptr+1)=(*(Point+1)&0x0f);
				*(Ptr+2)=(*(Point+2)>>4)&0x0f;
				*(Ptr+3)=(*(Point+2)&0x0f);
				*(Ptr+4)=(*(Point+3)>>4)&0x0f;
				*(Ptr+5)=(*(Point+3)&0x0f);
				Len=6;
				
					}
				if((DLMS_DATATab[i].DataType==D_Unsigned16)||(DLMS_DATATab[i].DataType==D_Integer16))
					{
					Word_BCD( Point,GetBuffToInteger(Ptr));
					//SwapData(Ptr,2);	
					RAM_Fill(Ptr,6);
					*(Ptr+2)=(*(Point)>>4)&0x0f;
					*(Ptr+3)=(*(Point)&0x0f);
					*(Ptr+4)=(*(Point+1)>>4)&0x0f;
					*(Ptr+5)=(*(Point+1)&0x0f);
					Len=6;
					
					}
			}
			DspNum( Ptr, Len, LCDSeg ,0 );
	}		
	return 0;
}

short DisDataClassData( unsigned char* LCDSeg ) 
{
  	short Err_Byte=-1;

	Err_Byte = DisDataRegClassData( LCDSeg );
	if( Err_Byte == 0 ) return 0; 
	Err_Byte = DisDataRAMClassData( LCDSeg );
	if( Err_Byte == 0 ) return 0;
	return Err_Byte;

}
void ECDisplayMove( unsigned char* Data )
{
	unsigned char Databuff;
	unsigned char i;
	Databuff = 0;
	for(i=0;i<5;i++)
	{
		if(i == 4)
			Databuff = 0;
		else	
			Databuff = *(Data + i + 1);
		Databuff <<= 4;
		*(Data + i) >>= 4;
		*(Data + i) |= Databuff;
	}

}

short DisRegisterECTab( unsigned char* LCDSeg )
{
	unsigned char Buff[6],Buff1[6];
	unsigned char* Point;
	unsigned char* Ptr;
	unsigned char i,j;
	unsigned short Addr;
	unsigned char Digit,Vol_Type;
	Point = Buff;
	Ptr = Buff1;
	for( i=0;i<DLMS_ECSum;i++ )						//电量表格
	{
		if( Data_Comp( Disk.Logical_Name, (unsigned char*)DLMS_ECTab[i].Logical_Name, 6 ) == 0 ) break;				//查找对应的OBIS码
	}
	if( i != DLMS_ECSum )
	{
		RAM_Fill(Point,6);
		Addr = DLMS_ECTab[i].Addr;
		Disk.DigitsValidFlg = 0x55;//显示到0.001kwh
		Read_CMonEC4( Point, Addr );
		SwapData(Point,4);
	/*	E2P_RData((unsigned char *)&Vol_Type,Meter_Vol_Type,1);
		if(Vol_Type==0)
			Digit=0;
		else
			*/
			Digit=2;	
	//	for(j=0;j<(3-Digit);j++)
	//		ECDisplayMove(Point);
		//RAM_Write(Point,Point+1,3);
	*Ptr=(*(Point+1)>>4)&0x0f;
	*(Ptr+1)=(*(Point+1)&0x0f);		
	*(Ptr+2)=(*(Point+2)>>4)&0x0f;
	*(Ptr+3)=(*(Point+2)&0x0f);
	*(Ptr+4)=(*(Point+3)>>4)&0x0f;
	*(Ptr+5)=(*(Point+3)&0x0f);
		DspNum( Ptr, 6, LCDSeg, Digit );
		//--------第一个大字形不显示----
		NonDisplaySEG1();
		DisplayUnit(DLMS_ECTab[i].Unit);	//显示单位
   		if((Flag.Power & F_PwrUp) != 0)	Flag.Disk |= F_DspALT;		//上电且是当前月需及时刷新
   		else Flag.Disk &= ~F_DspALT;
	}else return -1;	

	return 0;
}
//显示实时变量Register数据，返回值：-1，无此数据项，非-1：返回数据长度
short DisRegisterRealTab( unsigned char* LCDSeg )
{
	unsigned char Buff[6],Buff1[6];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char i,j;
	unsigned char*  Addr;
	unsigned short Length;
	unsigned char DataType;
	 long Value=0;
	unsigned char Digit,Vol_Type;
	Ptr = Buff;
	Point = Buff1;

	i = GetREALTabItemNo( Disk.Logical_Name );			//实时变量表格		//查找对应的OBIS码
	if( i != DLMS_RealSum )
	{
		DataType = DLMS_REALTab[i].DataType;
		Addr = DLMS_REALTab[i].Addr;
		Length = DLMS_REALTab[i].ComInfo & RS_LEN;				
		RAM_Write( (unsigned char *)&Value, Addr, Length );
	/*	E2P_RData((unsigned char *)&Vol_Type,Meter_Vol_Type,1);
		if(Vol_Type==0)
			Digit=2;
		else
			Digit=3;
		*/
		if(Value<0) Value = -Value;
	if(DLMS_REALTab[i].Scaler==0xFF)
		Digit=1;
	else if(DLMS_REALTab[i].Scaler==0xFE)
		Digit=2;
	else if(DLMS_REALTab[i].Scaler==0xFD)
		Digit=3;
	else if(DLMS_REALTab[i].Scaler==0xFC)
		Digit=4;
	else if(DLMS_REALTab[i].Scaler==0x00)	
		Digit=0;

	if(Value>99999)
		Length = 6;
	else if(Value>9999)
		Length = 5;
	else if(Value>999)
		Length = 4;
	else 
		Length = 3;
	
	//	for(j=0;j<(3-Digit);j++)
	//		Value/=10;
		RAM_Fill(Ptr,6);
		RAM_Fill(Point,6);
		if((DataType==D_Unsigned32)||(DataType==D_Integer32))
			{
			/*for(j=0;j<2;j++)
					{
					if(Value>999999)
					Value/=10;
					}
			*/
			Value/=100;
			Long_BCD4( Point, (unsigned long)Value );
			SwapData(Point,4);		
			
			*Ptr=(*(Point+1)>>4)&0x0f;
			*(Ptr+1)=(*(Point+1)&0x0f);
			*(Ptr+2)=(*(Point+2)>>4)&0x0f;
			*(Ptr+3)=(*(Point+2)&0x0f);
			*(Ptr+4)=(*(Point+3)>>4)&0x0f;
			*(Ptr+5)=(*(Point+3)&0x0f);
			//Length = 6;
			}
		if((DataType==D_Unsigned16)||(DataType==D_Integer16))
			{
			/*	for(j=0;j<2;j++)
					{
					if(Value>9999)
					Value/=10;
					}
				*/
			//Word_BCD( Point, (unsigned short)Value );
			//SwapData(Ptr,2);			
			Long_BCD4( Point, (unsigned long)Value );
			SwapData(Point,4);		
			
			*Ptr=(*(Point+1)>>4)&0x0f;
			*(Ptr+1)=(*(Point+1)&0x0f);
			*(Ptr+2)=(*(Point+2)>>4)&0x0f;
			*(Ptr+3)=(*(Point+2)&0x0f);
			*(Ptr+4)=(*(Point+3)>>4)&0x0f;
			*(Ptr+5)=(*(Point+3)&0x0f);
			}
		if((DataType==D_Unsigned8)||(DataType==D_Integer8))
			{
	/*		for(j=0;j<2;j++)
				{
				if(Value>99)
				Value/=10;
				}
	*/
		//	*Ptr=Byte_BCD( (unsigned char)Value );
		    *Point=Byte_BCD( (unsigned char)Value );
		
			*(Ptr+4)=(*(Point)>>4)&0x0f;
			*(Ptr+5)=(*(Point)&0x0f);
			Length = 3;
			}
		DspNum( Ptr, Length, LCDSeg, Digit );
		
		//--------第一个大字形不显示----
		NonDisplaySEG1();
		DisplayUnit( DLMS_REALTab[i].Unit );				//显示单位												
   	if((Flag.Power & F_PwrUp) != 0)	Flag.Disk |= F_DspALT;		//上电需及时刷新
   	else Flag.Disk &= ~F_DspALT;
	}else return -1;	
	
	return 0;	
}
//显示普通Register数据，返回值：-1，无此数据项，非-1：返回数据长度
short DisRegisterRegTab( unsigned char* LCDSeg )
{
	unsigned char Buff[6],Buff1[6];
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned char i,j;
	unsigned short Addr;
	unsigned short Length;
	unsigned char DataType;
	unsigned char Unit;
	long Value;
	unsigned char Digit,Vol_Type;

	Ptr = Buff;
	i = GetRegTabItemNo( Disk.Logical_Name );			//实时变量表格		//查找对应的OBIS码
	if( i != DLMS_RegSum )
	{
		DataType = DLMS_RegTab[i].DataType;
		Unit = DLMS_RegTab[i].Unit;
		Addr = DLMS_RegTab[i].Addr;
		Length = DLMS_RegTab[i].ComInfo & RS_LEN;
		if( Length > 20 ) return -1;
		E2P_RData( Ptr, Addr, Length );
		if(( DataType == D_VisibleStr )||( DataType == D_OctetStr ))								
		{
			return -1;
		}
		else 
		{		
		/*E2P_RData((unsigned char *)&Vol_Type,Meter_Vol_Type,1);
		
		if(Vol_Type==0)
			Digit=2;
		else
			Digit=3;
		*/
		if(DLMS_RegTab[i].Scaler==0xFF)
			Digit=1;
		else if(DLMS_RegTab[i].Scaler==0xFE)
			Digit=2;
		else if(DLMS_RegTab[i].Scaler==0xFD)
			Digit=3;
		else if(DLMS_RegTab[i].Scaler==0xFC)
			Digit=4;
		else if(DLMS_RegTab[i].Scaler==0x00)
			Digit=0;
		for(j=0;j<(3-Digit);j++)
			Value/=10;
		
		Point = Buff1;
			if(( DataType == D_Unsigned32 )||( DataType == D_Integer32 ))
			{
				RAM_Fill(Point,6);
				RAM_Write( (unsigned char*)&Value, Ptr, 4 );
				Long_BCD4( Point, Value );
			//	SwapData(Ptr,4);
			//	RAM_Write(Ptr,Ptr+1,3);
			
			*Ptr=(*(Point+1)>>4)&0x0f;
			*(Ptr+1)=(*(Point+1)&0x0f);
			*(Ptr+2)=(*(Point+2)>>4)&0x0f;
			*(Ptr+3)=(*(Point+2)&0x0f);
			*(Ptr+4)=(*(Point+3)>>4)&0x0f;
			*(Ptr+5)=(*(Point+3)&0x0f);
				DspNum( Ptr, 6, LCDSeg , Digit );
				DisplayUnit( Unit );				//显示单位																
			}	
			
			//--------第一个大字形不显示----
			NonDisplaySEG1();
		}	
	}else return -1;	
	return 0;	
}

//取Register类数据
short DisRegisterClassData( unsigned char* LCDSeg ) 
{
  	short Err_Byte=-1;

	Err_Byte = DisRegisterECTab( LCDSeg );						//读电量数据
	if( Err_Byte == 0 ) return 0; 
//	Err_Byte = DisRegisterCUMDTab( LCDSeg );					//读累积需量数据
//	if( Err_Byte == 0 ) return 0;             				
	Err_Byte = DisRegisterRealTab( LCDSeg );					//读实时变量数据
	if( Err_Byte == 0 ) return 0;             				
	Err_Byte = DisRegisterRegTab( LCDSeg );						//读普通数据
	if( Err_Byte == 0 ) return 0; 
	return Err_Byte;		
}

//OBIS码只显示CDE三位
void DisplayOBISCode( unsigned char *Logical_Name )
{
	unsigned char i;
	unsigned char buff[2];
	unsigned char* Ptr;
	
	Ptr = buff;
	if(Disk.Mode == ManualDisMode)
		{
		for(i=0;i<Normal_Display_Num;i++)
			{
			if(Data_Comp(Logical_Name,(unsigned char*)&Normal_Display[i].OBIS_Num,6)==0) break;
			}
		if(i!=Normal_Display_Num)
			{
				FillIntegerToBuff(Ptr,Normal_Display[i].Dis_Num);
				if((Normal_Display[i].Dis_Num==0x12)||(Normal_Display[i].Dis_Num==0x112)
				||(Normal_Display[i].Dis_Num==0x16)||(Normal_Display[i].Dis_Num==0x116)
				||(Normal_Display[i].Dis_Num==0x20)||(Normal_Display[i].Dis_Num==0x120)
				||(Normal_Display[i].Dis_Num==0x24)||(Normal_Display[i].Dis_Num==0x124)
				)
				{
				Display_Cum();
			//	Display_KW();
				}
				if((Normal_Display[i].Dis_Num==0x13)||(Normal_Display[i].Dis_Num==0x113)
				||(Normal_Display[i].Dis_Num==0x17)||(Normal_Display[i].Dis_Num==0x117)
				||(Normal_Display[i].Dis_Num==0x21)||(Normal_Display[i].Dis_Num==0x121)
				||(Normal_Display[i].Dis_Num==0x25)||(Normal_Display[i].Dis_Num==0x125)
				)
				{
				Display_ContCum();
			//	Display_KW();
					}
			}
		}
	else 
		{
		for(i=0;i<ALT_Display_Num;i++)
			{
			if(Data_Comp(Logical_Name,(unsigned char*)&ALT_Display[i].OBIS_Num,6)==0) break;
			}
		
		if(i!=ALT_Display_Num)
			{
			FillIntegerToBuff(Ptr,ALT_Display[i].Dis_Num);
			}
		else if(Data_Comp(Logical_Name,(unsigned char*)&Normal_Display[3].OBIS_Num,6)==0)
			FillIntegerToBuff(Ptr,Normal_Display[3].Dis_Num);
			
		}
	//for(j=0;j<3;j++)
	//	{
	LCDRAMBUFF[6] &=~ 0x0f;
	LCDRAMBUFF[7] &=~ 0x0f;
	LCDRAMBUFF[8] &=~ 0x0f;
	LCDRAMBUFF[9] &=~ 0x0e;
	LCDRAMBUFF[10] &=~ 0x0f;
	LCDRAMBUFF[11] &=~ 0x0e;
		DISPLAYNUMXX(6,(*Ptr& 0x0F));//seg7
		DISPLAYNUMXX(7,((*(Ptr+1)>>4)& 0x0F));//seg8
		DISPLAYNUMXX(8,(*(Ptr+1)& 0x0F));//seg9
	//	}

}

short DLMSDis_Read( unsigned char* LCDSeg )
{
  	short Err_Byte=0;
	
	if( JudgeDisItemType() != 0 ) return -1;
	
	switch( Disk.Class_Id )
	{
		case Class_id_Data:								//1
			Err_Byte = DisDataClassData( LCDSeg ); 
			break;
		case Class_id_Register:							//3
			Err_Byte = DisRegisterClassData( LCDSeg ); 
			break;
	//	case Class_id_Extended_register:				//4
	//		Err_Byte = DisExtendedRegisterClassData( LCDSeg ); 
	//		break;
	//	case Class_id_IEC_HDLC_setup:					//4
	//		Err_Byte = DisIEC_HDLC_setupClassData( LCDSeg ); 
	//		break;
		default: break;	
	}
	///DisplayOBISCode( LCDSeg, Disk.Logical_Name );
	return Err_Byte;
}

short GetNextCode( unsigned char Dir )		//Type == 0 递增显示，type=1 倒着显示
{
	unsigned char* ItemPtr;
	unsigned char ItemSum;
	unsigned short Addr;
	unsigned short TempInt;
	unsigned char Buff[12];
	unsigned char* Ptr;
	
	Ptr = Buff;
	switch( Disk.Mode )
	{
	    case AutoDisMode: 
			ItemPtr = &Disk.Mode2ItemPtr; 
			E2P_RData( (unsigned char*)&ItemSum, ALT_display_num, 1 );
			Addr = ALT_display_objects1;
			break;		
		case ManualDisMode: 
			ItemPtr = &Disk.Mode1ItemPtr; 
			E2P_RData( (unsigned char*)&ItemSum, General_display_num, 1 );
			Addr = General_display_objects1;
			break;
		case PwDownDisMode: 
			ItemPtr = &Disk.Mode3ItemPtr; 
			E2P_RData( (unsigned char*)&ItemSum, General_display_num, 1 );
			Addr = General_display_objects1;
			break;
		default: 
			ItemPtr = &Disk.Mode2ItemPtr; 
			E2P_RData( (unsigned char*)&ItemSum, ALT_display_num, 1 );
			Addr = ALT_display_objects1;
			break;		
	}	

	//if( ItemSum >= 60 ) ItemSum = 0;
//	if( ItemSum > 60 ) ItemSum = 40;  //
	if( Dir == 0 )
 	{
		*ItemPtr += 1;
		if(*ItemPtr >= ItemSum ) *ItemPtr = 0; 
	}
	else
	{
		if( *ItemPtr == 0 ) *ItemPtr = ItemSum;
		if( *ItemPtr >= ItemSum )    
		{
			if( ItemSum == 0 ) *ItemPtr = 0;
			else *ItemPtr = ItemSum - 1;				
		}
		else *ItemPtr -= 1;	
	}		
	TempInt = *ItemPtr;
	Addr += TempInt * 12;
	E2P_RData( Ptr, Addr, 11 );
	Disk.Class_Id = GetBuffToInteger( Ptr );
	RAM_Write( Disk.Logical_Name, Ptr+2, 6 );
	Disk.Attribute = *(Ptr+8);
    Disk.Data_Index = GetBuffToInteger( Ptr+9 );
  
	return 0;
}

void KeyProce( void )
{
	switch( SM.KbCount )
	{
		case P_Scroll: 	//GetNextCode( 1 );
				if(Disk.Mode == AutoDisMode)		 
                   // if( Disk.Mode != ManualDisMode )
                    {
                    	Disk.Mode = ManualDisMode;
                    	Disk.Mode1ItemPtr = 0xff;
                    }	
                    GetNextCode( 0 ); 
                break;
		case P_Reset: 	
			if((Flag.Power & F_PwrUp) != 0)
					  {						
						MaxDemand_Reset();
						Flag.Disk |= F_KeyInt;
						SM.KbCount &= ~P_Reset;
					  }	
			 break;
		case P_Program:
			if((Flag.Power & F_PwrUp) != 0)
			{
				if( SM.PrgDly == 0 )
				{
					SM.PrgDly = Para.RPrgDly;
				}	
				else 
				{
					SM.PrgDly = 0;
				}	
				Flag.Disk |= F_KeyInt;
				SM.KbCount &= ~P_Program;
			 }
			 break;
		default:
				SM.KbCount = 0;		
		return; 	
	}
	if((Flag.Power & F_PwrUp) == 0)
	{		
//		SM.KeyDly = 30 + (int)Para.RDspInv * Disk.AutoItemSum;	
		SM.KeyDly = 30;
		SM.KbCount = 0;
		Disk.DisInv = 6;	
		Flag.Disk |= F_KeyInt;
		return;
	}	
	else
	{
	SM.KeyDly = Para.RPrgDly;
		
	E2P_RData((unsigned char *)&SM.BJGDly,LCD_back_light_time,2);//背光点亮时间
		
	//	SM.BJGDly = temp_data;
//		SM.BJGDly = (unsigned int)Para.RDspInv * Disk.AutoItemSum * 2;
	}	
	SM.KbCount = 0;
//	Disk.DisInv = 30;	
	
 //   E2P_RData((unsigned char *)&Disk.DisInv,LCD_auto_scroll_time,1);
	Disk.DisInv = Para.RDspInv;

	//Disk.DisInv = DISDELAY;	
	Flag.Disk |= F_KeyInt;
}

void fnLcd_RefreshLcdBuf(void)
{
	unsigned char i;
  	for(i=0;i<18;i++) // HT_LCD->LCDBUF[i] = LCDRAMBUFF[i];
		HT_LCD_Write(i,LCDRAMBUFF[i]);
	for(i=18;i<41;i++)
		HT_LCD_Write(i,0x00);
  	return;
}
void ActiveDisp(void )
{
	Display_RELAYSTATE();
	Display_COMMSTATE();
	Display_Bar();
	Display_FeeNo();
	Display_Cover();
	//if()
	// 	Display_LA();
	//if()
	//	Display_LB();
	if(Disk.Mode == AutoDisMode)
		Display_ALT();
	if(SM.Demand_Interval_Remain==0)
	Display_EOI();

}
void LcdTxtDisplay(void )
{
	ActiveDisp();
	fnLcd_RefreshLcdBuf();	
}	

short Display(void)
 {
 unsigned char buff1[2];
 unsigned char* LCDSeg;
 
// unsigned char i;
 LCDSeg = buff1;
 *LCDSeg=0;

	 if( Disk.FullDisDelay != 0 )
	 {
		 RAM_DataFill( LCDRAMBUFF, 18, 0xff );
		 fnLcd_RefreshLcdBuf();
		 return 1;
	 }	 
	 RAM_Fill( LCDRAMBUFF, 18 );
 ///-------------------------------------------------
/*		 if(( Disk.DisInv != 0) && ( (Flag.Disk & F_KeyInt) == 0))
		 {
			 if((( Flag.Clk & F_DisSec ) != 0 ) && (( Flag.Disk & F_DspALT ) != 0 )) 
			 {
				 if((Flag.Power & F_PwrUp) == 0)
				 {
					 if(( IsLocalTimeDisplayItem() != 0 )&&( IsLocalDateDisplayItem() != 0 ))
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
	*/	 
 ///---------------------------------------------------
		if(((Flag.Disk & F_KeyInt) == 0) && ( Flag.Clk & F_DisSec ) == 0 )   return 0;
		Flag.Disk &= ~F_KeyInt;
		
	   	if( Disk.DisInv == 0 )
	   	{
	   		Flag.Disk &= ~F_DspALT;			//Test
	   		
		   	Disk.DisInv = Para.RDspInv;	

			GetNextCode( 0 );
	   	}	

	if( DLMSDis_Read( LCDSeg ) != 0 )
	{
		Disk.Class_Id = Class_id_Data;
		Disk.Attribute = 2;
		Disk.Logical_Name[0] = 0x01;
		Disk.Logical_Name[1] = 0x00;
		Disk.Logical_Name[2] = 0x00;
		Disk.Logical_Name[3] = 0x09;
		Disk.Logical_Name[4] = 0x01;
		Disk.Logical_Name[5] = 0xff;
		DisLocalTime( LCDSeg );
	}	
	
	DisplayOBISCode( Disk.Logical_Name );
	LcdTxtDisplay();
return 1;
 }


