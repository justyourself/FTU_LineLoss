#include "ht6xxx.h"
#include "ht6xxx_lib.h"
#include "port.h"
#include "General.h"
#include "Mem.h"
/**********************************************************************
	Asm Subroutine
	Source File in Asms.s43
**********************************************************************/
//extern char _ReadByte(void);
//extern void _WriteByte(unsigned char Byte);
//extern void _WriteWord(unsigned short word);

/*		//16.09.26
unsigned char ReadByte(void)
{
	unsigned short i;
	unsigned char Temp;
	
	Temp = 0;
	for( i=0;i<8;i++ )
	{
//		Delay(5);
		Delay(3);
		PDIR_FlsSCK |= P_FlsSCK;
//		Delay(5);
		Delay(3);
		if(( PIN_FlsSDO & P_FlsSDO ) != 0 ) Temp |= ByteBit[7-i];
		PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	return Temp;
}

void WriteByte(unsigned char Byte)
{
	unsigned short i;
	
	for( i=0;i<8;i++ )
	{
//		Delay(5);
		Delay(3);
		PDIR_FlsSCK |= P_FlsSCK;
		if(( Byte & ByteBit[7-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
		else PDIR_FlsSDI |= P_FlsSDI;
//		Delay(5);
		Delay(3);
		PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	Delay(3);
	PDIR_FlsSDI &= ~P_FlsSDI;
}

void WriteWord(unsigned short Word)
{
	unsigned short i;
	
	for( i=0;i<16;i++ )
	{
//		Delay(5);
		Delay(3);
		PDIR_FlsSCK |= P_FlsSCK;
		if(( Word & ByteBit[15-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
		else PDIR_FlsSDI |= P_FlsSDI;
//		Delay(5);
		Delay(3);
		PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	PDIR_FlsSDI &= ~P_FlsSDI;
}
*/
/*
unsigned char ReadByte(void)
{
	unsigned short i;
	unsigned char Temp;
	
	Temp = 0;
	for( i=0;i<8;i++ )
	{
//		Delay(5);
		Delay(3);
		HT_GPIOB->PTDIR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
//		Delay(5);
		Delay(3);
		if(( HT_GPIOB->PTDAT & GPIOB_FSO ) != 0 ) Temp |= ByteBit[7-i];//if(( PIN_FlsSDO & P_FlsSDO ) != 0 ) Temp |= ByteBit[7-i];
		HT_GPIOB->PTDIR &= ~GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	return Temp;
}

void WriteByte(unsigned char Byte)
{
	unsigned short i;
	
	for( i=0;i<8;i++ )
	{
//		Delay(5);
		Delay(3);
		HT_GPIOB->PTDIR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
		if(( Byte & ByteBit[7-i] ) != 0 ) HT_GPIOB->PTDIR &= ~GPIOB_FSI;//if(( Byte & ByteBit[7-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
		else HT_GPIOB->PTDIR |= GPIOB_FSI;//else PDIR_FlsSDI |= P_FlsSDI;
//		Delay(5);
		Delay(3);
		HT_GPIOB->PTDIR &= ~GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	Delay(3);
	HT_GPIOB->PTDIR &= ~GPIOB_FSI;//PDIR_FlsSDI &= ~P_FlsSDI;
}

void WriteWord(unsigned short Word)
{
	unsigned short i;
	
	for( i=0;i<16;i++ )
	{
//		Delay(5);
		Delay(3);
		HT_GPIOB->PTDIR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
		if(( Word & ByteBit[15-i] ) != 0 ) HT_GPIOB->PTDIR &= ~GPIOB_FSI;//if(( Word & ByteBit[15-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
		else HT_GPIOB->PTDIR |= GPIOB_FSI;//else PDIR_FlsSDI |= P_FlsSDI;
//		Delay(5);
		Delay(3);
		HT_GPIOB->PTDIR &= ~GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	HT_GPIOB->PTDIR &= ~GPIOB_FSI;//PDIR_FlsSDI &= ~P_FlsSDI;
}

#define	FLS_PAGE		   0x0100	    	// 0x0100 For AT25DF041A

#define	Array_Read		0x03		
#define	Block_Erase		0x20		
#define BytePage_Prg	0x02
#define Status_Read		0x05
#define Status_Write	0x01
#define Write_Enable	0x06
#define Read_ID			0x9F				//11.02.23

//	Status Busy Check		CMD=0xD7/0x57
short Fls_WriteBusy(void)
{
	unsigned short	i;
	short Count=0;				
	unsigned char Temp;						//V4			

//	DF_CS |= P_FlsCS;
	DF_CS |= GPIOB_FCS1;
	WriteByte( Status_Read );
	for( i=0; i<7500; i++ ) {		
//		WDTCTL = WDT_ARST_1000;		
		HT_FreeDog();;		
		Temp = ReadByte();								//V4		
		if(( Temp & 0x0f ) == 0x02 )	//V4					
		{
			Count++;
			if( Count >= 2 ) break;		
		}	
		else Count = 0;				
	}
//	DF_CS &= ~P_FlsCS;
	DF_CS &= ~GPIOB_FCS1;
	
	if( i<7500 ) 
	{
		Flag.AlarmFlg[2] &= ~F_AlarmFlash;			//V4
		return 0;			
	}	
	else
	{
		Flag.AlarmFlg[2] |= F_AlarmFlash;			//V4
		return -1;
	}	
}

short Fls_Busy(void)
{
	unsigned short	i;
	short Count=0;				
	unsigned char Temp;						//V4			

//	DF_CS |= P_FlsCS;
	DF_CS |= GPIOB_FCS1;
	WriteByte( Status_Read );
//	for( i=0; i<1000; i++ ) {		
	for( i=0; i<7500; i++ ) {		
//		WDTCTL = WDT_ARST_1000;		
		HT_FreeDog();;		
		Temp = ReadByte();								//V4		
//		if((( Temp & 0x01 ) == 0 )&&( Temp != 0x00 ))	//V4
		if(( Temp & 0x01 ) == 0 )	
		{
			Count++;
			if( Count >= 2 ) break;		
		}	
		else Count = 0;				
	}
//	DF_CS &= ~P_FlsCS;
	DF_CS &= ~GPIOB_FCS1;
	
//	if( i<1000 ) 
	if( i<7500 ) 
	{
		Flag.AlarmFlg[2] &= ~F_AlarmFlash;			//V4
		return 0;			
	}	
	else
	{
		Flag.AlarmFlg[2] |= F_AlarmFlash;			//V4
		return -1;
	}	
	
}

//	Status WriteEnable		CMD=0x06
short WriteEnable(void)
{
	if( Fls_Busy() == 0 ) {
//		DF_CS |= P_FlsCS;
		DF_CS |= GPIOB_FCS1;
		WriteByte( Write_Enable );	
//		DF_CS &= ~P_FlsCS;
		DF_CS &= ~GPIOB_FCS1;
		return 0;
	} else return -1;
}

//	Read Status 	CMD=0x06
unsigned char ReadStatus(void)
{
	short i;
	unsigned char Temp,Temp1;

//	DF_CS |= P_FlsCS;
	DF_CS |= GPIOB_FCS1;
	WriteByte( Status_Read );
//	WDTCTL = WDT_ARST_1000;		
	HT_FreeDog();;		
	for( i=0;i<10;i++ )
	{
		Temp = ReadByte();										
		Temp1 = ReadByte();
		if( Temp == Temp1 ) break;										
	}	
//	DF_CS &= ~P_FlsCS;
	DF_CS &= ~GPIOB_FCS1;
	return Temp;
}
//	Status WriteEnable		CMD=0x06
short StatusWrite(void)
{
	WriteEnable();
	if( Fls_Busy() == 0 ) {
//		DF_CS |= P_FlsCS;
		DF_CS |= GPIOB_FCS1;
		WriteByte( Status_Write );	
		WriteByte( 0x00 );	
//		DF_CS &= ~P_FlsCS;
		DF_CS &= ~GPIOB_FCS1;
		return 0;
	} else return -1;
}

//	Block Erase		CMD=0x20
short BlockErase(unsigned long Fls_Src)
{
	unsigned char Buff[4];
	unsigned char Temp;
	
	RAM_Write( Buff, (unsigned char*)&Fls_Src, 4 );
//	WDTCTL = WDT_ARST_1000;	
	HT_FreeDog();;		
//	StatusWrite();	
	Temp = ReadStatus();
	if(( Temp & 0x0C ) != 0 ) StatusWrite();	
	WriteEnable();
//	if( Fls_Busy() == 0 ) {
	if( Fls_WriteBusy() == 0 ) {
//		DF_CS |= P_FlsCS;
		DF_CS |= GPIOB_FCS1;
		WriteByte( Block_Erase );
		WriteByte( Buff[2] );
		WriteByte( Buff[1] );
		WriteByte( Buff[0] );
		
//		DF_CS &= ~P_FlsCS;
		DF_CS &= ~GPIOB_FCS1;
		return 0;
	} else return -1;
	
}

//	Read from DataFlashID 	CMD=0x9F
short Read_FlashID( unsigned char *RAM_Dest )										//11.02.23
{	
	if( Fls_Busy() == 0 ) 
	{
//		DF_CS |= P_FlsCS;
		DF_CS |= GPIOB_FCS1;
		WriteByte( Read_ID );
		*RAM_Dest = ReadByte();										
		*(RAM_Dest+1) = ReadByte();										
		*(RAM_Dest+2) = ReadByte();										
		
//		DF_CS &= ~P_FlsCS;
		DF_CS &= ~GPIOB_FCS1;
		return 0;
	} else return -1;
}

//	Read from DataFlash	CMD=0x03
short Read_Flash( unsigned char *RAM_Dest, unsigned long Fls_Src, unsigned short Lenth )
{
	unsigned char Buff[4];
	
	RAM_Write( Buff, (unsigned char*)&Fls_Src, 4 );
	
	if( Fls_Busy() == 0 ) {
//		DF_CS |= P_FlsCS;
		DF_CS |= GPIOB_FCS1;
		WriteByte( Array_Read );
		WriteByte( Buff[2] );
		WriteByte( Buff[1] );
		WriteByte( Buff[0] );
		
		while(Lenth>0){
			*RAM_Dest++ = ReadByte();
			Lenth--;
		}
//		DF_CS &= ~P_FlsCS;
		DF_CS &= ~GPIOB_FCS1;
		return 0;
	} else return -1;
}

// Read DataFlash
short DataFlash_Read( unsigned char *RAM_Dest, unsigned long Fls_Src, unsigned long Fls_Sta, unsigned long Fls_End, unsigned short Lenth )
{
    unsigned short Len;	

	if( ( Fls_Src + Lenth ) >= Fls_End )
	{
		Len = Fls_End - Fls_Src ;	
		Read_Flash( RAM_Dest, Fls_Src, Len );
		Read_Flash( RAM_Dest+Len, Fls_Sta, Lenth-Len );		
	}	
	else Read_Flash( RAM_Dest, Fls_Src, Lenth );
	return 0;
}

//	Write To DataFlash	CMD=0x02
short Write_Flash( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth )
{
	unsigned char Buff[4];
	unsigned char Temp;
	
	RAM_Write( Buff, (unsigned char*)&Fls_Dest, 4 );
	
//	StatusWrite();
	Temp = ReadStatus();
	if(( Temp & 0x0C ) != 0 ) StatusWrite();	
	WriteEnable();
//	if( Fls_Busy() == 0 ) {
	if( Fls_WriteBusy() == 0 ) {
//		DF_CS |= P_FlsCS;
		DF_CS |= GPIOB_FCS1;
		WriteByte( BytePage_Prg );
		WriteByte( Buff[2] );
		WriteByte( Buff[1] );
		WriteByte( Buff[0] );
		
		while(Lenth>0){
			WriteByte( *RAM_Src++ );
			Lenth--;
		}
//		DF_CS &= ~P_FlsCS;
		DF_CS &= ~GPIOB_FCS1;
		return 0;
	} else return -1;
}

short DataFlash_Write( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth )
{
	unsigned short Buf_Addr;
	unsigned short Len;
	unsigned long Value;
	
//	Value = Fls_Dest + Lenth;
	Value = Fls_Dest;
	if(( Value % 4096 ) == 0 )			//写数据空间为4K页的首地址
	{
		BlockErase( Value );
	}		
	while( Lenth > 0 ) 
	{
		Buf_Addr = Fls_Dest % FLS_PAGE;
		if( Buf_Addr != 0 ) Len = FLS_PAGE - Buf_Addr;
		else Len = FLS_PAGE;
		if( Lenth < Len ) Len = Lenth;
		Write_Flash( Fls_Dest, RAM_Src, Len );
		Fls_Dest += Len; 
		RAM_Src += Len;
		Lenth -= Len;
	}
	return 0;
}
*/

#define FLASH_DELAY1		0

/*
unsigned char ReadByte(void)
{
	unsigned short i;
	unsigned char Temp;
	
	Temp = 0;
	for( i=0;i<8;i++ )
	{
//		Delay(3);
		Delay(FLASH_DELAY1);			//16.10.16
//		HT_GPIOB->PTDIR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
		HT_GPIOB->PTCLR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
//		Delay(3);
		Delay(FLASH_DELAY1);			//16.10.16
		if(( HT_GPIOB->PTDAT & GPIOB_FSO ) != 0 ) Temp |= ByteBit[7-i];//if(( PIN_FlsSDO & P_FlsSDO ) != 0 ) Temp |= ByteBit[7-i];
//		HT_GPIOB->PTDIR &= ~GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
		HT_GPIOB->PTSET |= GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	return Temp;
}
*/
unsigned char ReadByte(void)
{
	unsigned short i;
	unsigned char Temp;
	
	Temp = 0;
	for( i=0;i<8;i++ )
	{
		Delay(FLASH_DELAY1);			//16.10.16
		DF_SCK &= (~FSCLK);//HT_GPIOC->PTCLR |= FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
		Delay(FLASH_DELAY1);			//16.10.16
		__NOP();						//16.10.17
		DF_SCK |= (FSCLK);//HT_GPIOC->PTSET |= FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
		Delay(FLASH_DELAY1);			//16.10.16
//		__NOP();						//16.10.17
//		if(( HT_GPIOB->PTDAT & GPIOB_FSO ) != 0 ) Temp |= ByteBit[7-i];//if(( PIN_FlsSDO & P_FlsSDO ) != 0 ) Temp |= ByteBit[7-i];
		Temp <<= 1;
		if(( DF_SDO & FSO ) != 0 ) Temp |= 0x01;	//if(( PIN_FlsSDO & P_FlsSDO ) != 0 ) Temp |= ByteBit[7-i];
//		HT_GPIOB->PTSET |= GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	return Temp;
}

/*
void WriteByte(unsigned char Byte)
{
	unsigned short i;
	
	for( i=0;i<8;i++ )
	{
//		Delay(3);
//		Delay(FLASH_DELAY1);			//16.10.16
		__NOP();						//16.10.17
//		HT_GPIOB->PTDIR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
		HT_GPIOB->PTCLR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
//		if(( Byte & ByteBit[7-i] ) != 0 ) HT_GPIOB->PTDIR &= ~GPIOB_FSI;//if(( Byte & ByteBit[7-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
		if(( Byte & ByteBit[7-i] ) != 0 ) HT_GPIOB->PTSET |= GPIOB_FSI;//if(( Byte & ByteBit[7-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
//		else HT_GPIOB->PTDIR |= GPIOB_FSI;//else PDIR_FlsSDI |= P_FlsSDI;
		else HT_GPIOB->PTCLR |= GPIOB_FSI;//else PDIR_FlsSDI |= P_FlsSDI;
//		Delay(3);
//		Delay(FLASH_DELAY1);			//16.10.16
		__NOP();						//16.10.17
//		HT_GPIOB->PTDIR &= ~GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
		HT_GPIOB->PTSET |= GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
//	Delay(3);
//	Delay(FLASH_DELAY1);			//16.10.16
	__NOP();						//16.10.17
//	HT_GPIOB->PTDIR &= ~GPIOB_FSI;//PDIR_FlsSDI &= ~P_FlsSDI;
	HT_GPIOB->PTSET |= GPIOB_FSI;//PDIR_FlsSDI &= ~P_FlsSDI;
}
*/
void WriteByte(unsigned char Byte)
{
	unsigned short i;
	
	for( i=0;i<8;i++ )
	{
                Delay(FLASH_DELAY1);
		DF_SCK &= (~FSCLK);//HT_GPIOC->PTCLR |= FSCLK;
                Delay(FLASH_DELAY1);
                if(( Byte & 0x80 ) == 0x80 ) DF_SDI |= FSI;//HT_GPIOC->PTSET |= FSI;//if(( Byte & ByteBit[7-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
		else DF_SDI &= (~FSI);//HT_GPIOC->PTCLR |= FSI;//else PDIR_FlsSDI |= P_FlsSDI;
		__NOP();
                 Delay(FLASH_DELAY1);
		DF_SCK |= (FSCLK);//HT_GPIOC->PTSET |= FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
		Byte <<= 1;
	}	
//	__NOP();						//16.10.17
	DF_SDI |= FSI;//HT_GPIOC->PTSET |= FSI;//PDIR_FlsSDI &= ~P_FlsSDI;
}

/*
void WriteWord(unsigned short Word)
{
	unsigned short i;
	
	for( i=0;i<16;i++ )
	{
//		Delay(3);
//		Delay(FLASH_DELAY1);			//16.10.16
		__NOP();						//16.10.17
//		HT_GPIOB->PTDIR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
		HT_GPIOB->PTCLR |= GPIOB_FSCLK;//PDIR_FlsSCK |= P_FlsSCK;
//		if(( Word & ByteBit[15-i] ) != 0 ) HT_GPIOB->PTDIR &= ~GPIOB_FSI;//if(( Word & ByteBit[15-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
		if(( Word & ByteBit[15-i] ) != 0 ) HT_GPIOB->PTSET |= GPIOB_FSI;//if(( Word & ByteBit[15-i] ) != 0 ) PDIR_FlsSDI &= ~P_FlsSDI;
//		else HT_GPIOB->PTDIR |= GPIOB_FSI;//else PDIR_FlsSDI |= P_FlsSDI;
		else HT_GPIOB->PTCLR |= GPIOB_FSI;//else PDIR_FlsSDI |= P_FlsSDI;
//		Delay(3);
//		Delay(FLASH_DELAY1);			//16.10.16
		__NOP();						//16.10.17
//		HT_GPIOB->PTDIR &= ~GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
		HT_GPIOB->PTSET |= GPIOB_FSCLK;//PDIR_FlsSCK &= ~P_FlsSCK;		
	}	
	__NOP();						//16.10.17
//	HT_GPIOB->PTDIR &= ~GPIOB_FSI;//PDIR_FlsSDI &= ~P_FlsSDI;
	HT_GPIOB->PTSET |= GPIOB_FSI;//PDIR_FlsSDI &= ~P_FlsSDI;
}
*/

#define	FLS_PAGE		   0x0100	    	// 0x0100 For AT25DF041A

#define	Array_Read		0x03		
#define	Block_Erase		0x20		
#define BytePage_Prg	0x02
#define Status_Read		0x05
#define Status_Write	0x01
#define Write_Enable	0x06
#define Read_ID			0x9F				//11.02.23

//	Status Busy Check		CMD=0xD7/0x57
short Fls_WriteBusy(void)
{
  unsigned short i;
  short Count=0;				
  unsigned char Temp;						//V4			
  DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
  WriteByte( Status_Read );
  for( i=0; i<7500; i++ ) 
  {
    HT_FreeDog();
    Temp = ReadByte();								//V4		
    if(( Temp & 0x0f ) == 0x02 )	//V4
    {
      Count++;
      if( Count >= 2 ) 
        break;		
    }	
    else 
      Count = 0;
  }
  DF_CS |=(FCS1);//HT_GPIOA->PTDAT |= FCS1;
  return !Count;
}

short Fls_Busy(void)
{
  unsigned short	i;
  short Count=0;
  unsigned char Temp;						//V4			
  DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
  WriteByte( Status_Read );
  for( i=0; i<7500; i++ ) 
  {
    HT_FreeDog();
    Temp = ReadByte();
    if(( Temp & 0x01 ) == 0 )
    {
      Count++;
      if( Count >= 2 ) break;
    }
    else 
      Count = 0;
  }
  DF_CS |=(FCS1);//HT_GPIOA->PTSET |= FCS1;
  return !Count;
}

//	Status WriteEnable		CMD=0x06
short WriteEnable(void)
{
	if( Fls_Busy() == 0 ) {
//		DF_CS |= P_FlsCS;
//		DF_CS |= GPIOB_FCS1;
		DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
		WriteByte( Write_Enable );	
//		DF_CS &= ~P_FlsCS;
//		DF_CS &= ~GPIOB_FCS1;
		DF_CS |=(FCS1);//HT_GPIOA->PTSET |= FCS1;
		return 0;
	} else return -1;
}

//	Read Status 	CMD=0x06
unsigned char ReadStatus(void)
{
	short i;
	unsigned char Temp,Temp1;

//	DF_CS |= P_FlsCS;
//	DF_CS |= GPIOB_FCS1;
	DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
	WriteByte( Status_Read );
//	WDTCTL = WDT_ARST_1000;		
	HT_FreeDog();;		
	for( i=0;i<10;i++ )
	{
		Temp = ReadByte();										
		Temp1 = ReadByte();
		if( Temp == Temp1 ) break;										
	}	
//	DF_CS &= ~P_FlsCS;
//	DF_CS &= ~GPIOB_FCS1;
	DF_CS |=(FCS1);//HT_GPIOA->PTSET |= FCS1;
	return Temp;
}
//	Status WriteEnable		CMD=0x06
short StatusWrite(void)
{
	WriteEnable();
	if( Fls_Busy() == 0 ) {
		DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
		WriteByte( Status_Write );	
		WriteByte( 0x00 );	
		DF_CS |=(FCS1);//HT_GPIOA->PTSET |= FCS1;
		return 0;
	} else return -1;
}

//	Block Erase		CMD=0x20
short BlockErase(unsigned long Fls_Src)
{
  unsigned char Buff[4];
  unsigned char Temp;
  RAM_Write( Buff, (unsigned char*)&Fls_Src, 4 );
  HT_FreeDog();
  Temp = ReadStatus();
  if(( Temp & 0x0C ) != 0 ) StatusWrite();
  WriteEnable();
  if( Fls_WriteBusy() == 0 ) {
    DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
    WriteByte( Block_Erase );
    WriteByte( Buff[2] );
    WriteByte( Buff[1] );
    WriteByte( Buff[0] );
    DF_CS |=(FCS1);//HT_GPIOA->PTSET |= FCS1;
    return 0;
  } 
  else 
    return -1;
}

//	Read from DataFlashID 	CMD=0x9F
short Read_FlashID( unsigned char *RAM_Dest )										//11.02.23
{
  if( Fls_Busy() == 0 )
  {
    DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
    WriteByte( Read_ID );
    *RAM_Dest = ReadByte();
    *(RAM_Dest+1) = ReadByte();	
    *(RAM_Dest+2) = ReadByte();	
    DF_CS |=(FCS1);//HT_GPIOA->PTSET |= FCS1;
    return 0;
  } 
  else 
    return -1;
}

//	Read from DataFlash	CMD=0x03
short Read_Flash( unsigned char *RAM_Dest, unsigned long Fls_Src, unsigned short Lenth )
{
  unsigned char Buff[4];
  RAM_Write( Buff, (unsigned char*)&Fls_Src, 4 );
  if( Fls_Busy() == 0 ) {
    DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
    WriteByte( Array_Read );
    WriteByte( Buff[2] );
    WriteByte( Buff[1] );
    WriteByte( Buff[0] );
    while(Lenth>0){
      *RAM_Dest++ = ReadByte();
      Lenth--;
    }
    DF_CS |=(FCS1);//HT_GPIOA->PTSET |= FCS1;
    return 0;
  } 
  else 
    return -1;
}

// Read DataFlash
short DataFlash_Read( unsigned char *RAM_Dest, unsigned long Fls_Src, unsigned long Fls_Sta, unsigned long Fls_End, unsigned short Lenth )
{
    unsigned short Len;	

	if( ( Fls_Src + Lenth ) >= Fls_End )
	{
		Len = Fls_End - Fls_Src ;	
		Read_Flash( RAM_Dest, Fls_Src, Len );
		Read_Flash( RAM_Dest+Len, Fls_Sta, Lenth-Len );		
	}	
	else Read_Flash( RAM_Dest, Fls_Src, Lenth );
	return 0;
}

//	Write To DataFlash	CMD=0x02
short Write_Flash( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth )
{
  unsigned char Buff[4];
  unsigned char Temp;
  RAM_Write( Buff, (unsigned char*)&Fls_Dest, 4 );
  Temp = ReadStatus();
  if(( Temp & 0x0C ) != 0 ) StatusWrite();
  WriteEnable();
  if( Fls_WriteBusy() == 0 ) {
    DF_CS &=(~FCS1);//HT_GPIOA->PTCLR |= FCS1;
    WriteByte( BytePage_Prg );
    WriteByte( Buff[2] );
    WriteByte( Buff[1] );
    WriteByte( Buff[0] );
    while(Lenth>0){
      WriteByte( *RAM_Src++ );
      Lenth--;
    }
    DF_CS |=(FCS1);//HT_GPIOA->PTSET |= FCS1;
    return 0;
  } 
  else 
    return -1;
}
#if 0
static char m_Databuf[4];
short DataFlash_Write( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned long Fls_Sta, unsigned long Fls_End,unsigned short Lenth )
{
  unsigned short Buf_Addr;
  unsigned short Len;
  unsigned long Value;
#if 0  
  Value = Fls_Dest+Lenth;
  if((( Value / 4096 ) != ( Fls_Dest / 4096 )) || ((Fls_Dest%4096)==0))			//写数据空间超过4K分界线
  {
    if( Value >= Fls_End ) Value = Value - Fls_End + Fls_Sta; 
    BlockErase( Value );
  }
#else
  Read_Flash(m_Databuf,Fls_Dest&(~0xfff),4096);
  memcpy(m_Databuf+(Fls_Dest&(0xfff)),RAM_Src,Lenth);
  BlockErase(Fls_Dest&(~0xfff));
  Fls_Dest &= (~0xfff);
  Lenth=4096;
  RAM_Src=m_Databuf;
#endif  
  while( Lenth > 0 )
  {
    Buf_Addr = Fls_Dest % FLS_PAGE;
    if( Buf_Addr != 0 ) 
      Len = FLS_PAGE - Buf_Addr;
    else 
      Len = FLS_PAGE;
    if( Lenth < Len ) 
      Len = Lenth;
    Write_Flash( Fls_Dest, RAM_Src, Len );
    Fls_Dest += Len; 
    RAM_Src += Len;
    Lenth -= Len;
  }
  return 0;
}
#else
short DataFlash_Write( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth )
{
	unsigned short Buf_Addr;
	unsigned short Len;
	unsigned long Value;
	
	Value = Fls_Dest;
	if(( Value % 4096 ) == 0 )			//写数据空间为4K页的首地址
	{
		BlockErase( Value );
	}		
	while( Lenth > 0 ) 
	{
		Buf_Addr = Fls_Dest % FLS_PAGE;
		if( Buf_Addr != 0 ) Len = FLS_PAGE - Buf_Addr;
		else Len = FLS_PAGE;
		if( Lenth < Len ) Len = Lenth;
		Write_Flash( Fls_Dest, RAM_Src, Len );
		Fls_Dest += Len; 
		RAM_Src += Len;
		Lenth -= Len;
	}
	return 0;
}
#endif

short Write_Flash_Direct( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth )
{
	unsigned short Buf_Addr;
	unsigned short Len;
	while( Lenth > 0 ) 
	{
		Buf_Addr = Fls_Dest % FLS_PAGE;
		if( Buf_Addr != 0 ) Len = FLS_PAGE - Buf_Addr;
		else Len = FLS_PAGE;
		if( Lenth < Len ) Len = Lenth;
		Write_Flash( Fls_Dest, RAM_Src, Len );
		Fls_Dest += Len; 
		RAM_Src += Len;
		Lenth -= Len;
	}
	return 0;
}		
