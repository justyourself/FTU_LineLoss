#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Port.h"
#include "Display.h"
//#include "RsComm.h"
#include "Initial.h"
#include "Time.h"

//#define scll      		p6_1  
//#define sdal      		p6_0 
//#define lcdstr          p0_3
//#define sdr_scll  		pd6_1
//#define sdr_sdal  		pd6_0
//#define sdr_lcdstr 		pd0_3

const unsigned char LCD_const[8]={0x03,0x23,0x13,0x33,0x0b,0x2b,0x1b,0x3b};//EM6110双RAM模式，数据传输时的8ROW的控制字，BLANK SET和W位为0，MUX1和MUX0位为1（8com模式）8位分别代表row1(0x03)--row8(0x3b);
//const unsigned char LCD_const[8]={0x07,0x27,0x17,0x37,0x0f,0x2f,0x1f,0x3f};
//unsigned char reeprom_status;
typedef union Union_ByteDef
{
	struct
	{ 
		unsigned char b0:1;
		unsigned char b1:1;
		unsigned char b2:1;
		unsigned char b3:1;
		unsigned char b4:1;
		unsigned char b5:1;
		unsigned char b6:1;
		unsigned char b7:1;
	}bit;
	unsigned char byte;//定义数据每字节的8个位；
}UNION_BYTE;


void delay_6110()   //延时程序；
{
   unsigned char a;
//   for(a=0;a<10;a++)
   for(a=0;a<5;a++)
        {;}
}

#if ( LCDDrive_PCF8578 == YesCheck )
unsigned char wr8578(unsigned char *p_xsbuf,unsigned char lddtpt,unsigned char xslong)
{
	unsigned char i;
	unsigned short Buff[10];
	I2CPARA* I2CPara;

	delay_6110();
	I2CPara = (I2CPARA*)Buff;
	GetI2CPara( I2CPara, PCF8578Ads );	
   
	for(i=0;i<150;i++)
	{
	 	if( I2CStart( I2CPara,0x0000, 0x78 ) == 0 ) break;		
	}	
	if( i == 150 ) return I2CErr(I2CPara);	

  	for(i=0;i<xslong;i++)
  	{
    	if( I2CMtData(I2CPara,*p_xsbuf)!= 0 )return I2CErr(I2CPara);
    	p_xsbuf++;     	
 	}
 	
	delay_6110();
	I2CErr(I2CPara);		//STOP
        
  	return 0;
}

void open8578(void)
{	
	unsigned char Buff[2];
 
    RAM_Fill(&Buff[0], 2);
     
//    WDTCTL = WDT_ARST_1000; 
    HT_FreeDog(); 
              
	Buff[0]=0xD5;          //  SET  MODE   d5    全屏d9                       
 	Buff[1]=0xFC;          //  SET  START BANK    FC
                  
	wr8578(&Buff[0],0xd5,2);				
}

unsigned char w8576_byte( unsigned char a, unsigned char  addr)//依次传8行，每行的数据，W8576_byte(a,addr)是为了和主函数保持一致；
{
	unsigned char i;
	unsigned char Buff[70];

//	WDTCTL = WDT_ARST_1000;
    HT_FreeDog(); 

	RAM_Fill( &Buff[0], 70); 
    Buff[0]=0xE0;
    Buff[1]=0x88;
    Buff[2]=0x74;

	for(i=0;i<16;i++)
	{
	  Buff[4*i+3]=Disk.LCDSeg[i]&0x0F;
	  Buff[4*i+5]=(Disk.LCDSeg[i]&0xF0)/16;
	}	
		
	for(i=0;i<16;i++)
	{  
	  Buff[4*i+3]|=(Disk.LCDSeg[i+16]&0x0F)*16;
	  Buff[4*i+5]|=Disk.LCDSeg[i+16]&0xF0;
	}
  		 					   	     
	wr8578(&Buff[0],0xd5,67);   
	open8578();   
	
	return  1;
}

#elif(LCDDrive_BU97950==YesCheck )
unsigned char wr97950(unsigned char *p_xsbuf,unsigned char lddtpt,unsigned char xslong)
{
	unsigned char i;
	unsigned short Buff[10];
	I2CPARA* I2CPara;

	delay_6110();
	I2CPara = (I2CPARA*)Buff;
	GetI2CPara( I2CPara, BU97950Ads );	

//	if(( Flag.Disk & F_KeyS ) != 0 ) 			//液晶驱动芯片首次上电		//新国网	//13.10.16	
//	{
		Delay(5);
		I2CErr(I2CPara);		//STOP			//13.10.05
//		Flag.Disk &= ~F_KeyS;					//液晶驱动芯片首次上电		//新国网	//13.10.16
		Delay(5);
//	}	
   
	for(i=0;i<150;i++)
	{
	 	if( I2CStart( I2CPara,0x0000, 0x7C ) == 0 ) break;		
	}	
	if( i == 150 ) return I2CErr(I2CPara);	

  	for(i=0;i<xslong;i++)
  	{
    	if( I2CMtData(I2CPara,*p_xsbuf)!= 0 )return I2CErr(I2CPara);
    	p_xsbuf++;     	
 	  }
	delay_6110();
	I2CErr(I2CPara);		//STOP
        
  	return 0;
}

void open97950(void)
{	
	unsigned char Buff[4];
 	short i;												//新国网	//13.10.17	
 
    RAM_Fill(&Buff[0], 4);
     
//    WDTCTL = WDT_ARST_1000; 
    HT_FreeDog(); 
              
	Buff[0]=0xF1;          //  ICSET                     
// 	Buff[1]=0xE6;          //  DISCTL
// 	Buff[2]=0xC0;          //  EVRSET
// 	Buff[3]=0x00;          //  ADSET
 	//buff[3]=0xFA             //APCTL
                  
//	wr97950(&Buff[0],0xF1,1);	
	for( i=0;i<3;i++ )									//新国网	//13.10.17
	{													//新国网	//13.10.17		
		if( wr97950(&Buff[0],0xF1,1) == 0 ) break;		//新国网	//13.10.17
	}													//新国网	//13.10.17	
				
}

unsigned char w8576_byte( unsigned char a, unsigned char  addr)//依次传8行，每行的数据，W8576_byte(a,addr)是为了和主函数保持一致；
{
	unsigned char i,j;
	unsigned char Buff[70];
	unsigned char Temp;

//	WDTCTL = WDT_ARST_1000;
    HT_FreeDog(); 

	RAM_Fill( &Buff[0],36); 
//    Buff[0]=0xE0;
//    Buff[1]=0x88;
//    Buff[2]=0x74;
  	Buff[0]=0xF1;
   	Buff[1]=0xE2;
   	Buff[2]=0xC0;
	Buff[3]=0x00;
	for(i=0;i<16;i++)
	{
	  Buff[i*2+4]=Disk.LCDSeg[i]&0x0F;
	  Buff[i*2+5]=(Disk.LCDSeg[i]&0xF0)/16;
	}	
		
	for(i=0;i<16;i++)
	{  
	  Buff[i*2+4]|=(Disk.LCDSeg[i+16]&0x0F)*16;
	  Buff[i*2+5]|=Disk.LCDSeg[i+16]&0xF0;
	}
	
	for( i=0;i<32;i++ )
	{
		Temp = 0;
		for( j=0;j<8;j++ )
		{
			if(( Buff[i+4]&ByteBit[7-j] ) != 0 ) Temp |= ByteBit[j];	
		}		
		Buff[i+4] = Temp; 
	}	
//for(i=4;i<36;i++)                           
//{                                           
//  Buff[i]|=0xFF;      
//}                                           
  
  //open97950(); 		 					   	     
//	wr97950(&Buff[0],0xE2,36);   
	for( i=0;i<3;i++ )									//新国网	//13.10.17
	{													//新国网	//13.10.17		
		if( wr97950(&Buff[0],0xE2,36) == 0 ) break;		//新国网	//13.10.17
	}													//新国网	//13.10.17	
  open97950(); 
	
	return  1;
}
#elif(LCDDrive_BR8564==YesCheck )			//2012.07.03
unsigned char wrBR8564(unsigned char *p_xsbuf,unsigned char lddtpt,unsigned char xslong)
{
	unsigned char i;
	unsigned short Buff[10];
	I2CPARA* I2CPara;

	delay_6110();
	I2CPara = (I2CPARA*)Buff;
	GetI2CPara( I2CPara, BU97950Ads );	

//	if(( Flag.Disk & F_KeyS ) != 0 ) 			//液晶驱动芯片首次上电		//新国网	//13.10.16	
//	{
		Delay(5);
		I2CErr(I2CPara);		//STOP			//13.10.05
//		Flag.Disk &= ~F_KeyS;					//液晶驱动芯片首次上电		//新国网	//13.10.16
		Delay(5);
//	}	
   
	for(i=0;i<150;i++)
	{
	 	if( I2CStart( I2CPara,0x0000, BR8564Ads ) == 0 ) break;		
	}	
	if( i == 150 ) return I2CErr(I2CPara);	

  	for(i=0;i<xslong;i++)
  	{
    	if( I2CMtData(I2CPara,*p_xsbuf)!= 0 )return I2CErr(I2CPara);
    	p_xsbuf++;     	
 	  }
	delay_6110();
	I2CErr(I2CPara);		//STOP
        
  	return 0;
}

unsigned char w8576_byte( unsigned char a, unsigned char  addr)//依次传8行，每行的数据，W8576_byte(a,addr)是为了和主函数保持一致；
{
	unsigned char i,j;
	unsigned char Buff[50];
	unsigned char Temp;
	
//	WDTCTL = WDT_ARST_1000;
    HT_FreeDog(); 

	RAM_Fill( &Buff[0],50); 
//	Buff[0]=0xE0;		//SLEEP控制命令:正常工作
// 	Buff[1]=0xF0;		//闪烁控制命令:不闪烁
// 	Buff[2]=0xF8;		//BANK选择命令:4COM时有效
// 	Buff[3]=0xCE;		//状态控制命令:1.允许显示,2.‘1’--1/4Bias,3.‘10’--8COM; 	
// 	Buff[4]=0x00;		//写RAM起始地址命令:初始值=“000000”

	Buff[0]=0xE0;		//SLEEP控制命令:正常工作
 	Buff[1]=0xF0;		//闪烁控制命令:不闪烁
 	Buff[2]=0xF8;		//BANK选择命令:4COM时有效
 	Buff[3]=0xCE;		//状态控制命令:1.允许显示,2.‘1’--1/4Bias,3.‘10’--8COM; 	
 	Buff[4]=0x00;		//写RAM起始地址命令:初始值=“000000”
	 	
	for(i=0;i<16;i++)
	{
	  Buff[i*2+5]=Disk.LCDSeg[i]&0x0F;
	  Buff[i*2+6]=(Disk.LCDSeg[i]&0xF0)/16;
	}	
		
	for(i=0;i<16;i++)
	{  
	  Buff[i*2+5]|=(Disk.LCDSeg[i+16]&0x0F)*16;
	  Buff[i*2+6]|=Disk.LCDSeg[i+16]&0xF0;
	}

	for( i=0;i<32;i++ )
	{
		Temp = 0;
		for( j=0;j<8;j++ )
		{
			if(( Buff[i+5]&ByteBit[7-j] ) != 0 ) Temp |= ByteBit[j];	
		}		
		Buff[i+5] = Temp; 
	}
                                         		 					   	     
//	wrBR8564(&Buff[0],0xE0,37); 
	for( i=0;i<3;i++ )									//新国网	//13.10.17
	{													//新国网	//13.10.17		
		if( wrBR8564(&Buff[0],0xE0,37) == 0 ) break;	//新国网	//13.10.17
	}													//新国网	//13.10.17	
	  	
	return  1;
}

#else
void p6110_start() //开始工作；
{
//	sdr_scll=1;  		
//	sdr_sdal=1;  		
//	sdr_lcdstr=1;
//	lcdstr=0;
//	scll=1;

//	PDIR_LCDCLK &= ~P_LCDCLK;				//Test
	POUT_LCDCLK |= P_LCDCLK;				//Test
//	PDIR_LCDDI &= ~P_LCDDI;					//Test
	POUT_LCDDI |= P_LCDDI;					//Test
//	PDIR_LCDSTR |= P_LCDSTR;				//Test	
	POUT_LCDSTR &= ~P_LCDSTR;				//Test	
	delay_6110();
	delay_6110();	
}

void p6110_stop()							//
{
//	sdr_scll=1;  		
//	PDIR_LCDCLK &= ~P_LCDCLK;				//Test
	POUT_LCDCLK |= P_LCDCLK;				//Test
//	sdr_sdal=1;  		
//	PDIR_LCDDI &= ~P_LCDDI;					//Test
	POUT_LCDDI |= P_LCDDI;					//Test
//	sdr_lcdstr=1;
//	lcdstr=1;
//	PDIR_LCDSTR &= ~P_LCDSTR;				//Test	
	POUT_LCDSTR |= P_LCDSTR;				//Test	
	delay_6110();
	delay_6110();
//	lcdstr=0;
//	PDIR_LCDSTR |= P_LCDSTR;				//Test	
	POUT_LCDSTR &= ~P_LCDSTR;				//Test	
/*	delay_6110();
	delay_6110();
	delay_6110();
	delay_6110();
	delay_6110();
	delay_6110();

	delay_6110();
	delay_6110();
	delay_6110();
	delay_6110();
	delay_6110();
	delay_6110();
*/
}
/*
void p6110_wb(unsigned char a)//每次传输6个字节的数据1-4个字节为32位数据，第5个字节为空，第6个字节为控制命令字；
{
	unsigned char a1,i;
	unsigned char b1;
//	UNION_BYTE data[6];
	unsigned char data[6];
	
//	WDTCTL = WDT_ARST_1000;
	HT_FreeDog();
	RAM_Fill( data, 4 );
//	for(i=0;i<4;i++)
//	{
//		data[i].byte=lcdsbuf[a*4+i];
//		data[i]=0xff;
//	}
	
	for(i=0;i<32;i++)
	{
		if( (Disk.LCDSeg[(a/4)*16+(31-i)/2] & ByteBit[(1-i%2)*4+a%4] ) != 0 ) data[i/8] |= ByteBit[7-(i%8)];
	}		
	
//	data[4].byte=0;
	data[4] = 0;
//	data[5].byte=LCD_const[a];
	data[5] = LCD_const[a];
	for(b1=0;b1<6;b1++)//传6个字节；
	{
		for(a1=0;a1<8;a1++)//传一个字节的8位；
		{
//			scll=1;
			PDIR_LCDCLK &= ~P_LCDCLK;					
			delay_6110();
//			sdal=data[b1].bit.b7;
			if(( data[b1] & ByteBit[7-a1] ) != 0 ) PDIR_LCDDI &= ~P_LCDDI;					
			else PDIR_LCDDI |= P_LCDDI;
			delay_6110();
//			scll=0;
			PDIR_LCDCLK |= P_LCDCLK;		
			delay_6110();
//			data[b1].byte=data[b1].byte<<1;      
		}
		delay_6110();
	}
}
*/

void p6110_wb(unsigned char COM_No, unsigned char* Source )//每次传输6个字节的数据1-4个字节为32位数据，第5个字节为空，第6个字节为控制命令字；
{
	unsigned char i,j;
	unsigned char Data;
	
//	WDTCTL = WDT_ARST_1000;
    HT_FreeDog(); 
//	RAM_Fill( data, 4 );
	
//	for(i=0;i<32;i++)
//	{
//		if( (Disk.LCDSeg[(a/4)*16+(31-i)/2] & ByteBit[(1-i%2)*4+a%4] ) != 0 ) data[i/8] |= ByteBit[7-(i%8)];
//	}		
	
//	data[4] = 0;
//	data[5] = LCD_const[a];
	for( i=0;i<6;i++ )			//传6个字节；
	{
		Data = *(Source+COM_No*6+i);
		for( j=0;j<8;j++ )		//传一个字节的8位；
		{
//			PDIR_LCDCLK &= ~P_LCDCLK;					//Test
			POUT_LCDCLK |= P_LCDCLK;					//Test
			delay_6110();
//			if(( data[i] & ByteBit[7-j] ) != 0 ) PDIR_LCDDI &= ~P_LCDDI;					
//			if(( Data & ByteBit[7-j] ) != 0 ) PDIR_LCDDI &= ~P_LCDDI;					//Test
			if(( Data & ByteBit[7-j] ) != 0 ) POUT_LCDDI |= P_LCDDI;					//Test
//			else PDIR_LCDDI |= P_LCDDI;			//Test
			else POUT_LCDDI &= ~P_LCDDI;			//Test
			delay_6110();
//			PDIR_LCDCLK |= P_LCDCLK;				//Test
			POUT_LCDCLK &= ~P_LCDCLK;				//Test
			delay_6110();
		}
		delay_6110();
	}
}

unsigned char powerdown_8576()//下电退出；
{  
//   p6110_stop();     
	POUT_LCDCLK &= ~P_LCDCLK;				//Test
	POUT_LCDDI &= ~P_LCDDI;					//Test
	POUT_LCDSTR &= ~P_LCDSTR;				//Test	

   return 1;
}

// kou ling
unsigned char w8576_byte( unsigned char a, unsigned char  addr)//依次传8行，每行的数据，W8576_byte(a,addr)是为了和主函数保持一致；
{
   unsigned char i,j;
   unsigned char Buff[50];
   unsigned char* Ptr;
   
   Ptr = Buff;
   RAM_Fill( Ptr, 50); 
   for( j=0;j<8;j++ )
   {
//		WDTCTL = WDT_ARST_1000;
    	HT_FreeDog(); 
		for(i=0;i<32;i++)
		{
			if( (Disk.LCDSeg[(j/4)*16+(31-i)/2] & ByteBit[(1-i%2)*4+j%4] ) != 0 ) *(Ptr+(i/8)+j*6) |= ByteBit[7-(i%8)];
		}		
		*(Ptr+j*6+4) = 0;
		*(Ptr+j*6+5) = LCD_const[j];				   	
   }	
   
   p6110_start();
   for(i=0;i<8;i++)//调用8次p6110_wb(i),传完8行的数据；
   {
//		p6110_wb(i);
		p6110_wb(i,Ptr);
		p6110_stop();
   }
   powerdown_8576();
   return  1;
}

#endif

  
  
  
  
  