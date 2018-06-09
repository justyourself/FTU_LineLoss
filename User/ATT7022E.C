#include "TypeMeter.h"
#include "TypeRAM.h"
#include "TypeE2p.h"
#include "TypeFRAM.h"
#include "TypeFLASH.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
#include "Port.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Power.h"
#include "Display.h"
#include "Rscomm.h"
#include "math.h"					
#if ( AdjustErrorCheck == YesCheck )	//15.09.05
#include "AdjustError.h"
#endif
#if ( MEASCHIP == ATT7022E )
#include "ATT7022E.h"					

#define E7022_E2_W E2P_WAdj
#define E7022_E2_R E2P_RAdj


unsigned short GetATT7022ECalibDataEAddr( unsigned char RegNo )  
{
  unsigned short EpAds=0;
  if(( RegNo <= 0x39 )||(( RegNo >= 0x60 )&&( RegNo <= 0x71)))
  {
    EpAds = RegNo;
    if( RegNo >= 0x60 )	EpAds -= 0x20;
    EpAds *= 3;	
  }
  return EpAds;
}	

#if( AllFail7022Chk == YesCheck )							
void ATT7022AmperInit(void)
{
	unsigned char Buff[6];
	unsigned char* Point;
	short RegAds,EpAds;
	unsigned long Value2=0;			//ATChk
//	short i,FaultFlag;						//10.11.02
	short i;						//10.11.02
	short FlashState = 0;	
	
	Point = Buff;

	Delay(5000);					//新国网	//13.10.18
//	WDTCTL = WDT_ARST_1000;			//新国网	//13.10.18
	HT_FreeDog();
	
	HT_GPIOC->PTCLR |= GPIOC_EMU_CLK;//POUT_ADSCLK &= ~P_ADSCLK;
	HT_GPIOA->PTCLR |= GPIOA_EMU_RST;//POUT_ADREST &= ~P_ADREST;
	HT_GPIOC->PTSET |= GPIOC_EMU_DIN;//POUT_ADDIN |= P_ADDIN;
	HT_GPIOC->PTSET |= GPIOC_EMU_CS;//POUT_ADCS |= P_ADCS;
	HT_GPIOC->PTCLR |= GPIOC_EMU_DIN;//POUT_ADDIN &= ~P_ADDIN;
	HT_GPIOC->PTDIR &= ~GPIOC_EMU_DOUT;//PDIR_MDOUT &= ~P_MDOUT;

	for( RegAds=0;RegAds<20*4;RegAds++ )
	{
		__NOP();	
	}		
	
	HT_GPIOA->PTSET |= GPIOA_EMU_RST;//POUT_ADREST |= P_ADREST;

	for( RegAds=0;RegAds<300*4;RegAds++ )
	{
		__NOP();	
	}		

	*(Point+3) =0;
	*(Point+4) =0;
	*(Point+5) =0;
	
	*(Point+3) =0x5A;
	ATT7022WtReg( 0xC9, Point+3 );
	*(Point+3) =0;
	ATT7022WtReg( 0xC3, Point+3 );
	ATT7022WtReg( 0x80, Point+3 );

	*Point = 0;
	*(Point+1) = 0;
	*(Point+2) = 0;
	ATT7022WR( 0xC6, Point, 3 );	   	

	*(Point+2) = 0x0;	

//	*Point = 0xFE;					
	*Point = 0x7E;						//ADC chop关闭	//Test
//	*(Point+1) = 0xB9;					//有效值慢速模式，稳定	//13.09.12
	*(Point+1) = 0x89;					//有效值快速模式		//13.09.12
//	*(Point+1) = 0x88;					//Femu=1.8432M			//15.09.06
	ATT7022WtReg( ModeCfg+128, Point );	//模式配置寄存器

	*Point = 0x00;						//电压通道2倍增益、电流通道1倍增益
//	*(Point+1) = 0x01;
	*(Point+1) = 0x00;				//12..01.18  ADC1倍
	ATT7022WtReg( PGACtrl+128, Point );	//ADC增益配置寄存器

#if( LinkMode == Phase3Wire4 )			//ATChk
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
	*Point = 0xCC;						//代数和、能量寄存器读后清0	//视在采用RMS		//14.09.04
#else
	*Point = 0xC4;						//代数和、能量寄存器读后清0
#endif
#else
	*Point = 0x84;						//代数和、能量寄存器读后清0	
#endif
	*(Point+1) = 0xF8;					//选择功率作为潜动起动判断依据
	ATT7022WtReg( EMUCfg+128, Point );	//EMU单元配置

//	*Point = HFConstL;													//新国网		//13.08.30
//	*(Point+1) = HFConstH;												//新国网		//13.08.30
	RAM_Write( Point, (unsigned char*)&MSpec.R7022E_HFConst, 2 );		//新国网		//13.08.30	
	ATT7022WtReg( HFConst+128, Point );	//高频脉冲输出

	*Point = 0x08;							//缺省值=8						
	*(Point+1) = 0x00;
	ATT7022WtReg( IaRmsoffse+128, Point );	//A相电流有效值offset校正

	*Point = 0x08;							//缺省值=8						
	*(Point+1) = 0x00;
	ATT7022WtReg( IbRmsoffse+128, Point );	//B相电流有效值offset校正

	*Point = 0x08;							//缺省值=8						
	*(Point+1) = 0x00;
	ATT7022WtReg( IcRmsoffse+128, Point );	//C相电流有效值offset校正

	*Point = 0x00;
	*(Point+1) = 0x00;
	ATT7022WtReg( QPhscal+128, Point );	//无功相位校正寄存器

//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T用7022内部温度传感器
	*Point = 0x37;							//7022E必须增加温度补偿
//#else
//	*Point = 0x27;
//#endif
	*(Point+1) = 0x34;
	ATT7022WtReg( ModuleCFG+128, Point );	//模拟模块使能寄存器

//  *Point = Pstartup % 256;											//新国网		//13.08.30		
//  *(Point+1) = Pstartup / 256;										//新国网		//13.08.30	
	RAM_Write( Point, (unsigned char*)&MSpec.R7022E_PStartup, 2 );		//新国网		//13.08.30	
	ATT7022WtReg( Pstart+128, Point );	//模拟模块使能寄存器

//  *Point = Istart % 256;							//13.07.01			//新国网		//13.08.30	
//  *(Point+1) = Istart / 256;											//新国网		//13.08.30		
	RAM_Write( Point, (unsigned char*)&MSpec.R7022E_IStart, 2 );		//新国网		//13.08.30	
	ATT7022WtReg( Istartup+128, Point );	//电流启动阀值寄存器

#if ( NEW7022E == YesCheck )
//	*Point = 0xF7;												
//	*(Point+1) = 0xFE;
	*Point = 0x11;												
	*(Point+1) = 0xFF;
	ATT7022WtReg( TCcoffA+128, Point );		//调整温度补偿系数		//13.08.09

//	*Point = 0x1B;												
//	*(Point+1) = 0x2F;
	*Point = 0x53;												
	*(Point+1) = 0x2B;
	ATT7022WtReg( TCcoffB+128, Point );		//调整温度补偿系数		//13.08.09

//	*Point = 0xDB;												
//	*(Point+1) = 0x10;
	*Point = 0x83;												
	*(Point+1) = 0xD4;
	ATT7022WtReg( TCcoffC+128, Point );		//调整温度补偿系数		//13.08.09

    *Point = 0x02;						//VrefAotu_en = 1  //温度自动补偿使能
    *(Point+1) = 0x00;
	ATT7022WtReg( EMCfg+128, Point );	//新增加的算法控制寄存器
#endif

//	for( RegAds=IgainA;RegAds<=IgainC;RegAds++ )
	for( RegAds=UgainA;RegAds<=IgainC;RegAds++ )						//新国网
	{
#if ( NEW7022E == YesCheck )
          EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
          EpAds = RegAds * 3;
#endif
          EpAds += E2P_PGLEN;
          EpAds -= 0x100;
          E7022_E2_R( Point, EpAds, 2 );
          if(( Flag.BatState & F_E2PCheck ) != 0 )
          {													
            if(( HT_GPIOB->PTDAT & GPIOB_VMEM_CTL ) != 0 )	//if(( P8OUT & P_EECTL ) != 0 )			//若Flash处于关闭状态，打开FLASH，并增加若干延时。
            {
              FlashState = 1;	
              OpenFlashIO();
              HT_FreeDog();
              Delay(10000);
            }	
            EpAds = RegAds * 3;						
            for( i=0;i<2;i++ )															
            {																				
              Value2 = (unsigned long)(AT7022DataZonePage+i)*4096;
              Read_Flash( Point, Value2+EpAds, 3 );									
              if( *(Point+2) == ChkNum( Point, 2 )) 									
              {
                break;	
              }	
            }
            if( i == 2 ) 
            {
              EpAds += E2P_PGLEN;
              EpAds -= 0x100;											
              E2P_RData( Point, EpAds, 2 );
            }	
            if(FlashState != 0 )			//若原Flash处于关闭状态，操作完延时后关闭FLASH电源。
            {
              HT_FreeDog();
              Delay(10000);
              HT_FreeDog();
              Delay(10000);
              CloseFlashIO();	
            }
          }
          ATT7022WtReg( RegAds+128, Point );
	} 
}
#endif

unsigned long SetFailVoltage(unsigned short Devads)				//ATChk
{
	unsigned char Buff[6];
	unsigned char* Point;
	unsigned long Value;
	
	Point = Buff;
	
//	WDTCTL = WDT_ARST_1000;	
	HT_FreeDog();
	E2P_RData( Point, EPhFail_Valve, 2 ); 
	 *(Point+2) = 0;
	 *(Point+3) = 0;
	Value = BCD2_Word(Point);
	Value = Value * 64;		
	Value /= 10;

	RAM_Fill( Point, 4 );
	*Point = 0x5A;
	ATT7022WtReg( 0xC9, Point , Devads);
	*Point = 0x00;
	ATT7022WtReg( 0x80, Point , Devads);
	ATT7022WtReg( FailVoltage+128, (unsigned char*)&Value , Devads);  	
	
#if ( NEW7022E == YesCheck )
#else
	*(Point+2) = 0x00;					//关闭写校表数据功能
	*(Point+1) = 0x00;
	*Point = 0x01;
	ATT7022WtReg( 0xC9, Point , Devads);
#endif
	return Value;				//ATChk

}


void SetIDefault(unsigned short Devads)
{
  short RegAds,EpAds;
  unsigned char Buff[6];
  for( RegAds=PgainA;RegAds<=QgainC;RegAds++ )
  {
#if ( NEW7022E == YesCheck )
    EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
    EpAds = RegAds * 3;
#endif
    EpAds += E2P_PGLEN;
    EpAds -= (0x100*Devads); //10.07.30
    Buff[0]=0x2e;
    Buff[1]=0xf2;
   //Buff[0]=0;
   //Buff[1]=0;
    E7022_E2_W( EpAds,Buff,2);	
  }
  
  for( RegAds=SgainA;RegAds<=SgainC;RegAds++ )
  {
#if ( NEW7022E == YesCheck )
    EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
    EpAds = RegAds * 3;
#endif
    EpAds += E2P_PGLEN;
    EpAds -= (0x100*Devads); //10.07.30
    Buff[0]=0x8f;
    Buff[1]=0xf8;
   //Buff[0]=0;
   //Buff[1]=0;
    E7022_E2_W( EpAds,Buff,2);	
  }
  for( RegAds=IgainA;RegAds<=IgainC;RegAds++ )
  {
#if ( NEW7022E == YesCheck )
    EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
    EpAds = RegAds * 3;
#endif
    EpAds += E2P_PGLEN;
    EpAds -= (0x100*Devads); //10.07.30
    switch(RegAds)
    {
    case IgainA:
      Buff[0]=0xB8;
      break;
    case IgainB:
      Buff[0]=0x99;
      break;
    default:
      Buff[0]=0xA8;
      break;
    }
   // Buff[0]=0xee;
    Buff[1]=0xF1;//0xEE;
    //Buff[0]=0;
    //Buff[1]=0;
    E7022_E2_W( EpAds,Buff,2);	
  }
  for( RegAds=UgainA;RegAds<=UgainC;RegAds++ )
  {
#if ( NEW7022E == YesCheck )
    EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
    EpAds = RegAds * 3;
#endif
    EpAds += E2P_PGLEN;
    EpAds -= (0x100*Devads); //10.07.30
    
    switch(RegAds)
    {
    case UgainC:
      Buff[0]=0x1F;
      Buff[1]=0xFF;
      break;
    case UgainB:
      Buff[0]=0xC8;
      Buff[1]=0xFE;
      break;
    default:
      Buff[0]=0xC0;
      Buff[1]=0xFE;
      break;
    }
    
   // Buff[0]=0;
   // Buff[1]=0;
    E7022_E2_W( EpAds,Buff,2);	
  }
  
  for( RegAds=PhsregA0;RegAds<=PhsregC1;RegAds++ )
  {
#if ( NEW7022E == YesCheck )
    EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
    EpAds = RegAds * 3;
#endif
    EpAds += E2P_PGLEN;
    EpAds -= (0x100*Devads); //10.07.30
    
    switch(RegAds)
    {
    case PhsregA0:
      Buff[0]=0x00;
      Buff[1]=0x00;
      break;
    case PhsregA1:
      Buff[0]=0x00;
      Buff[1]=0x00;
      break;
    case PhsregB0:
      Buff[0]=00;
      Buff[1]=00;
      break;
    case PhsregC0:
      Buff[0]=00;
      Buff[1]=00;
      break;
    default:
      Buff[0]=0x0;
      Buff[1]=0x00;
      break;
    }
    E7022_E2_W( EpAds,Buff,2);	
  }
  
  for( RegAds=PoffsetA;RegAds<=PoffsetC;RegAds++ )
  {
#if ( NEW7022E == YesCheck )
    EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
    EpAds = RegAds * 3;
#endif
    EpAds += E2P_PGLEN;
    EpAds -= (0x100*Devads); //10.07.30
    
    switch(RegAds)
    {
    case PoffsetA:
      Buff[0]=0xFF;
      Buff[1]=0x7F;
      break;
    case PoffsetB:
      Buff[0]=0x99;
      Buff[1]=0x7F;
      break;
    case PoffsetC:
      Buff[0]=0xAA;
      Buff[1]=0x7F;
      break;
    default:
      Buff[0]=0x0;
      Buff[1]=00;
      break;
    }
    E7022_E2_W( EpAds,Buff,2);	
  }
  
  #if ( NEW7022E == YesCheck )
    EpAds = GetATT7022ECalibDataEAddr( Iregion );
#else
    EpAds = Iregion * 3;
#endif
    EpAds += E2P_PGLEN;
    EpAds -= (0x100*Devads); //10.07.30
    Buff[0]=0x20;
    Buff[1]=0x01;
    E7022_E2_W( EpAds,Buff,2);	
}

void ATT7022Init(unsigned short Devads)
{
  unsigned char Buff[6];
  unsigned char* Point;
  short RegAds,EpAds;
  unsigned long Value=0;
  unsigned long Value2=0;			//ATChk
  unsigned long HFConstHL;		//新国网		//13.08.30
  short i,FaultFlag;						//10.11.02
  unsigned char Buff1[32];
  SPIPARA* SPIPara;	
  
  if(SM.CalibCount == CALIBCOUNT1) return;	//处于校表状态，上电不进行AT7022初始化
  
  SPIPara = (SPIPARA*)Buff1;
  GetSPIPara( SPIPara, Devads );
  Point = Buff;
  HT_FreeDog();
#if 0  
  HT_GPIOG->IOCFG &=~(GPIOG_EMU_CLK|GPIOG_EMU_DIN|GPIOG_EMU_DOUT);
  HT_GPIOG->PTUP  &=~(GPIOG_EMU_CLK|GPIOG_EMU_DIN|GPIOG_EMU_DOUT);
  HT_GPIOG->PTDIR |= GPIOG_EMU_CLK;
  HT_GPIOG->PTCLR = GPIOG_EMU_CLK;
#endif
  HT_GPIOC->IOCFG &=~(GPIOG_EMU_CLK|GPIOG_EMU_DIN|GPIOG_EMU_DOUT);
  HT_GPIOC->PTUP  &=~(GPIOG_EMU_CLK|GPIOG_EMU_DIN|GPIOG_EMU_DOUT);
  HT_GPIOC->PTDIR |= GPIOG_EMU_CLK;
 // HT_GPIOC->PTCLR = GPIOG_EMU_CLK;
  
  *SPIPara->AD_RST_PTDIR |= SPIPara->AD_RST;
  *SPIPara->AD_RST_PTCLR = SPIPara->AD_RST;
#if 0 
  HT_GPIOG->PTSET |= GPIOG_EMU_DIN;
#endif
//  HT_GPIOC->PTSET |= GPIOG_EMU_DIN;  
  *SPIPara->AD_CS_PTSET |= SPIPara->AD_CS;
  *SPIPara->AD_CS_PTDIR |= SPIPara->AD_CS;
//  HT_GPIOC->PTCLR |= GPIOG_EMU_DIN;
  HT_GPIOC->PTDIR &= ~GPIOG_EMU_DOUT;
  SM.CalibCount = 0;
  udelay(500); 
  *SPIPara->AD_RST_PTSET |= SPIPara->AD_RST;	
  udelay(4500);
  //SetIDefault(Devads);//zzltest
  HFConstHL = MSpec.R7022E_HFConst;	//新国网		//13.08.30
  
  *(Point+3) =0;
  *(Point+4) =0;
  *(Point+5) =0;
  *(Point+3) = 0x5A;
  ATT7022WtReg( 0xC9, Point+3 ,Devads);
  
  *(Point+3) = 0x0;
  ATT7022WtReg( 0xC3, Point+3 ,Devads);
  ATT7022WtReg( 0x80, Point+3 ,Devads);

  *Point = 0;											//15.09.06		
  *(Point+1) = 0;										//15.09.06		
  *(Point+2) = 0;										//15.09.06		
  ATT7022WR( 0xC6, Point, 3 ,Devads);	   					//15.09.06			

//    Value = 0x00000160+0x00000001 + 0x00002C59 + 0x0000000F + 0x00001000 + 0x00004500;						//HFConst有缺省值0x10			//ATChk
//    Value = 0x000082C9;						//HFConst有缺省值0x10			//ATChk
//    Value = 0x00000001 + 0x00002C59 + 0x0000000F + 0x00001000 + 0x00004500;						//启动电流阀值要另外填
    Value = 0x00008169;					//HFConst有缺省值0x10			//ATChk
	Value += 24;						//增加三相电流有效值offset校正

	*(Point+2) = 0x0;	

//	*Point = 0xFE;
	*Point = 0x7E;						//ADC chop关闭	//Test
	*(Point+1) = 0xB9;
//	*(Point+1) = 0xB8;					//Femu=1.8432M			//15.09.06
	ATT7022WtReg( ModeCfg+128, Point ,Devads);	//模式配置寄存器

	*Point = 0x00;						//电压通道2倍增益、电流通道1倍增益
//	*(Point+1) = 0x01;
	*(Point+1) = 0x00;					//12..01.18  ADC1倍
	ATT7022WtReg( PGACtrl+128, Point ,Devads);	//ADC增益配置寄存器

#if( LinkMode == Phase3Wire4 )			//ATChk
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
	*Point = 0xCC;						//代数和、能量寄存器读后清0	//视在采用RMS		//14.09.04
#else
	*Point = 0xC4;						//代数和、能量寄存器读后清0
#endif	
#else
	*Point = 0x84;						//代数和、能量寄存器读后清0	
#endif
	*(Point+1) = 0xF8;			//选择功率作为潜动起动判断依据
	ATT7022WtReg( EMUCfg+128, Point ,Devads);	//EMU单元配置

//	*Point = HFConstL;													//新国网		//13.08.30
//	*(Point+1) = HFConstH;												//新国网		//13.08.30
	RAM_Write( Point, (unsigned char*)&MSpec.R7022E_HFConst, 2 );		//新国网		//13.08.30	
	ATT7022WtReg( REGHFConst+128, Point ,Devads);	//高频脉冲输出

	*Point = 0x08;				//缺省值=8						
	*(Point+1) = 0x00;
	ATT7022WtReg( IaRmsoffse+128, Point ,Devads);	//A相电流有效值offset校正

	*Point = 0x08;							//缺省值=8						
	*(Point+1) = 0x00;
	ATT7022WtReg( IbRmsoffse+128, Point ,Devads);	//B相电流有效值offset校正

	*Point = 0x08;							//缺省值=8						
	*(Point+1) = 0x00;
	ATT7022WtReg( IcRmsoffse+128, Point ,Devads);	//C相电流有效值offset校正

	*Point = 0x00;
	*(Point+1) = 0x00;
	ATT7022WtReg( QPhscal+128, Point ,Devads);	//无功相位校正寄存器

//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T用7022内部温度传感器
	*Point = 0x37;							//7022E必须增加温度补偿	
//#else
//	*Point = 0x27;
//#endif
	*(Point+1) = 0x34;
	ATT7022WtReg( ModuleCFG+128, Point ,Devads);	//模拟模块使能寄存器

//  *Point = Pstartup % 256;											//新国网		//13.08.30		
//  *(Point+1) = Pstartup / 256;										//新国网		//13.08.30	
	RAM_Write( Point, (unsigned char*)&MSpec.R7022E_PStartup, 2 );		//新国网		//13.08.30	
	ATT7022WtReg( Pstart+128, Point ,Devads);	//模拟模块使能寄存器

//  *Point = Istart % 256;							//13.07.01			//新国网		//13.08.30	
//  *(Point+1) = Istart / 256;											//新国网		//13.08.30		
	RAM_Write( Point, (unsigned char*)&MSpec.R7022E_IStart, 2 );		//新国网		//13.08.30	
	ATT7022WtReg( Istartup+128, Point ,Devads);	//电流启动阀值寄存器

#if ( NEW7022E == YesCheck )
    *Point = 0x02;						//VrefAotu_en = 1  //温度自动补偿使能
    *(Point+1) = 0x00;
    ATT7022WtReg( EMCfg+128, Point ,Devads);	//新增加的算法控制寄存器
#endif
        FaultFlag = 0;														//10.11.02	
	for( RegAds=PgainA;RegAds<=Iregion;RegAds++ )
	{
          if(( RegAds >= QgainA )&&( RegAds <= SgainC )) continue; 
          if(( RegAds >= PoffsetA )&&( RegAds <= QPhscal )) continue; 
          if(( RegAds >= Istartup )&&( RegAds <= Pstart )) continue; 

#if ( NEW7022E == YesCheck )
          EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
          EpAds = RegAds * 3;
#endif
          EpAds += E2P_PGLEN;
          EpAds -= (0x100*Devads); //10.07.30
          E7022_E2_R( Point, EpAds, 2 );				//10.07.30
          ATT7022WtReg( RegAds+128, Point ,Devads);
          Value2 = 0;														//10.11.02
          RAM_Write( (unsigned char*)&Value2, Point, 2 );		//ATChk
          Value += Value2;									//ATChk
          if(( RegAds >= PgainA )&&( RegAds <= PgainC ))
          {
            for( i=1;i<3;i++ )								//无功和视在设成和有功一样	
            {
              ATT7022WtReg( RegAds+i*3+128, Point ,Devads);
              Value2 = 0;														//10.11.02
              RAM_Write( (unsigned char*)&Value2, Point, 2 );		//ATChk
              Value += Value2;									//ATChk
            }
          }
        } 
        Value += 0xB97E;										//ADC chop关闭	//Test
        Value += 0x0000 + 0xF884;							//12..01.18  ADC1倍			//2倍						//新国网		//13.08.30
        Value += HFConstHL + 0x3427 + MSpec.R7022E_PStartup + MSpec.R7022E_IStart;  //ATChk			//2倍	//新国网		//13.08.30
#if( LinkMode == Phase3Wire4 )			//ATChk
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
	Value += 0x0048;					//视在采用RMS		//14.09.04
#else
	Value += 0x0040;
#endif	
#endif
//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T用7022内部温度传感器
	Value += 0x0010;										//7022E必须增加温度补偿
//#endif

	Value2 = SetFailVoltage(Devads);			//ATChk
    Value += Value2;
	
	RAM_Write( Point, (unsigned char*)&Value, 3 );			//ATChk		
	if(( Data_Comp(Point, Para.RAT7022ChkSum, 3) != 0 )&&( FaultFlag == 0 ))		//ATChk		//10.11.02
	{														//ATChk	
		RAM_Write( Para.RAT7022ChkSum, Point, 3 );			//ATChk		
		E2P_WData( AT7022ChkSum, Point, 3 );				//ATChk
	}														//ATChk	

#if ( NEW7022E == YesCheck )
//    Value = 0x0000FEFF+0x0000EF7A + 0x0000047C+0x000002;			//带温度补偿
//    Value = 0x0001F2F7;	    									//
//    Value = 0x0000FEF7+0x00002F1B + 0x000010DB+0x000002;			//调整温度补偿系数		//13.08.09
 //   Value = 0x00013EEF;	
//    Value = 0x0000FF11+0x00002B53 + 0x0000D483+0x000002;			//调整温度补偿系数		//13.08.09
   Value = 0x0001FEE9;	
    
//	*Point = 0xF7;												
//	*(Point+1) = 0xFE;
	*Point = 0x11;												
	*(Point+1) = 0xFF;
	ATT7022WtReg( TCcoffA+128, Point ,Devads);		//调整温度补偿系数		//13.08.09

//	*Point = 0x1B;												
//	*(Point+1) = 0x2F;
	*Point = 0x53;												
	*(Point+1) = 0x2B;
	ATT7022WtReg( TCcoffB+128, Point ,Devads);		//调整温度补偿系数		//13.08.09

//	*Point = 0xDB;												
//	*(Point+1) = 0x10;
	*Point = 0x83;												
	*(Point+1) = 0xD4;
	ATT7022WtReg( TCcoffC+128, Point ,Devads);		//调整温度补偿系数		//13.08.09

//    Value = 0x0000FEFF+0x0000EF7A + 0x0000047C;			//
//    Value = 0x0001F2F5;						//
	Value += 8;								//增加零线电流有效值offset校正

	*Point = 0x08;							//缺省值=8						
	*(Point+1) = 0x00;
	ATT7022WtReg( ItRmsoffse+128, Point ,Devads);	//零线电流有效值offset校正

	FaultFlag = 0;														//10.11.02	
	for( RegAds=Iregion1;RegAds<=OILVL;RegAds++ )
	{
		if((( RegAds >= PoffsetAL )&&( RegAds <= OILVL ))&&( RegAds != TPSoffset )) continue; 

#if ( NEW7022E == YesCheck )
		EpAds = GetATT7022ECalibDataEAddr( RegAds );
#else
		EpAds = RegAds * 3;
#endif
		EpAds += E2P_PGLEN;	
		EpAds -= (0x100*Devads);
		E7022_E2_R( Point, EpAds, 2 );
		ATT7022WtReg( RegAds+128, Point ,Devads);
		Value2 = 0;														//10.11.02
		RAM_Write( (unsigned char*)&Value2, Point, 2 );		//ATChk
		Value += Value2;									//ATChk
	} 

	*(Point+2) = 0x00;					//关闭写校表数据功能
	*(Point+1) = 0x00;
	*Point = 0x01;
	ATT7022WtReg( 0xC9, Point ,Devads);

	RAM_Write( Point, (unsigned char*)&Value, 3 );			//ATChk		
	if(( Data_Comp(Point, Para.RAT7022ChkSum2, 3) != 0 )&&( FaultFlag == 0 ))		//ATChk		//10.11.02
	{														//ATChk	
		RAM_Write( Para.RAT7022ChkSum2, Point, 3 );			//ATChk		
		E2P_WData( AT7022ChkSum2, Point, 3 );				//ATChk
	}														//ATChk		   	


#endif

	SM.AT7022ChkSumCnt = 2;									//ATChk

#if ( AdjustErrorCheck == YesCheck )	//15.09.05
	ErrRAM.VoltAmperFtFreqState = 0;
	initialErrorStateVar();
	E2P_RData( (unsigned char*)&ErrRAM.RErrorMode, EErrorMode, 1 ); 
	if( ErrRAM.RErrorMode != NormalMode )
	{
		ErrRAM.RErrorMode = NormalMode; 	
		E2P_WData( EErrorMode, (unsigned char*)&ErrRAM.RErrorMode,1 ); 
	}	
#endif

}

unsigned char ComAdjWrite(unsigned char* ComBuf ,unsigned short Devads)
{
  short Temp,Temp2;		
  unsigned char Buff[4]; 
  unsigned char* Point;
  unsigned short Temp3;				//10.07.30
  short i;					//10.11.02
  Point = Buff;	
  Temp = *(ComBuf+Rs_IDLPtr);
//#if ( RemoteEnterCalib == YesCheck )								//新国网	//14.05.23
//  if((((HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )&&( IsFactoryTestState() != 0 ))||((SM.CalibCount != CALIBCOUNT1)&&(Temp != HFDouble))  )
//#else
//  if((( HT_GPIOD->PTDAT & GPIOD_SETEN ) != 0 )||((SM.CalibCount != CALIBCOUNT1)&&(Temp != HFDouble))  ) 
//#endif
//  {
//    *ComBuf = 0xFF;
//    return RS_State_IVData;
//  }
  if((( Temp >= PgainA )&&(Temp <= PgainC))||(( Temp >= PhsregA0 )&&(Temp <= PhsregC1))
#if ( NEW7022E == YesCheck )
     ||(( Temp >= PhsregA2 )&&(Temp <= PhsregC2))||( Temp == Iregion1 )||( Temp == TPSoffset )
#endif
       ||(( Temp >= UgainA )&&(Temp <= IgainC))||(Temp == GainADC7)||(Temp == HFDouble)||( Temp == Iregion)) 
  {
    RAM_Write( Point, ComBuf+Rs_WDataPtr, 2 );		//10.07.30

#if ( NEW7022E == YesCheck )
    Temp2 = GetATT7022ECalibDataEAddr( Temp );
#else
    Temp2 = Temp * 3;								
#endif
    Temp3 = Temp2 + E2P_PGLEN;
    Temp3 -= (0x100*Devads);
//    Temp3 = E2P_PGLEN - 0x100;	(0x100*Devads); //10.07.30
//    Temp3 += Temp2;									//10.07.30
    E7022_E2_W( Temp3, ComBuf+Rs_WDataPtr, 2 );		//10.07.30
    E7022_E2_R( ComBuf+1, Temp3, 2 );		//10.07.30
    if( Data_Comp( Point, ComBuf+Rs_WDataPtr, 2 ) != 0 ) return RS_State_IVData;		//10.07.30
    *(Point+2) = 0x00;
    *(Point+1) = 0x00;
    *Point = 0x5A;
    ATT7022WtReg( 0xC9, Point ,Devads);
    ATT7022WtReg( Temp+128, ComBuf+Rs_WDataPtr ,Devads);
    if(( Temp >= PgainA )&&(Temp <= PgainC))					//无功和视在使用有功数据
    {
      for(i=1;i<3;i++ )
      {
        ATT7022WtReg( Temp+i*3+128, ComBuf+Rs_WDataPtr ,Devads);
      }
    }
    *(Point+2) = 0x00;
    *(Point+1) = 0x00;
    *Point = 0x01;						//关闭写校表数据功能
    ATT7022WtReg( 0xC9, Point ,Devads);
  }
  return 0;
}	

long GetPhasePW( unsigned char Cmd ,unsigned short Devads)								//SmallCurrent
{
  unsigned long long LValue;
  unsigned short	i=1,Temp;
  unsigned long Value=0;
  unsigned long PS32;					//新国网		//13.08.30	
  unsigned long HFConstHL;			//新国网		//13.08.30	
  float FValue;
  unsigned char Buff[4];
  unsigned char* Data;
  Data = Buff;
  if( (Flag.Power & F_PwrUp) == 0)	//停电以后返回值为零。
  {
    *Data = 0;
    *(Data + 1) = 0;	
    *(Data + 2) = 0;	
    return 0;
  }
  LValue = 0;
  PS32 = MSpec.RMeterConst / 100;		//新国网		//13.08.30
  HFConstHL = MSpec.R7022E_HFConst;	//新国网		//13.08.30	
  
  ATT7022RdReg( Cmd, Data ,Devads);
  if(( *(Data +2) & 0x80 ) == 0) i = 0;
  else 
  {
    *(Data+2) ^= 0xff;
    *(Data+1) ^= 0xff;
    *Data ^= 0xff; 
  }
  Temp = *(Data + 1) * 256 + *Data;
  Value = (unsigned long)*(Data + 2) * 65536;
  LValue = Temp;
  LValue += Value;
  if( i == 1 ) 	LValue += 1;
  LValue = LValue * 100;
  FValue = LValue;
  switch( Cmd )
  {
  case ATPWPA:
  case ATPWPB:		
  case ATPWPC:		
  case ATPWQA:		
  case ATPWQB:	
  case ATPWQC:		
//新国网		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
////				LValue = ( LValue * 1265625 * 10 ) / ( (unsigned long)HFConstHL * PS32 * 4096 );		//PS32大了10倍
//				LValue = ( LValue * 6328125 ) / ( (unsigned long)HFConstHL * PS32 * 2048 );		//PS32大了10倍
//#else
////				LValue = ( LValue * 253125 * 10 ) / ( (unsigned long)HFConstHL * PS32 * 8192 );			//Lvalue = Lvalue*2.592*10^10/(HFconst*EC*2^23)
    LValue = ( LValue * 1265625 ) / ( (unsigned long)HFConstHL * PS32 * 4096 );			//Lvalue = Lvalue*2.592*10^10/(HFconst*EC*2^23)
//#endif		
    FValue = LValue;		
    break;	
  case ATPWPZ:		
  case ATPWQZ:		
//新国网		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
////				LValue = ( LValue * 1265625 * 10 ) / ( (unsigned long)HFConstHL * PS32 * 2048 );		//PS32大了10倍
//				LValue = ( LValue * 6328125 ) / ( (unsigned long)HFConstHL * PS32 * 1024 );		//PS32大了10倍
//#else
////				LValue = ( LValue * 253125 * 10 ) / ( (unsigned long)HFConstHL * PS32 * 4096 );			//Lvalue = Lvalue*2*2.592*10^10/(HFconst*EC*2^23)
    LValue = ( LValue * 1265625 ) / ( (unsigned long)HFConstHL * PS32 * 2048 );			//Lvalue = Lvalue*2*2.592*10^10/(HFconst*EC*2^23)
//#endif
    FValue = LValue;		
    break;
  default: 
    break;
  }
  //return FValue;
  return LValue;
}

short Read_ATTValue( unsigned char Cmd, unsigned char* Data ,unsigned short Devads)
{
  unsigned short	i=1,Temp;
  unsigned long Value;
  unsigned long long LValue;
  unsigned long A0001Ib;
  unsigned long A0002Ib;
  unsigned long PS32;				//新国网		//13.08.30
  unsigned long HFConstHL;		//新国网		//13.08.30
  unsigned long PW00002Ib;		//新国网		//13.08.30
  unsigned char Buff[4];
  unsigned char* Point;
  unsigned char Temp2=0;				//11.08.03
  
  Point = Buff;
  if( (Flag.Power & F_PwrUp) == 0)	//停电以后返回值为零。
  {
    *Data = 0;
    *(Data + 1) = 0;	
    *(Data + 2) = 0;	
    return 0;
  }	
  
  A0001Ib = MSpec.RBaseCurrent / 1000;	//新国网		//13.08.30
  A0002Ib = MSpec.RBaseCurrent / 500;		//新国网		//13.08.30
  PS32 = MSpec.RMeterConst / 100;			//新国网		//13.08.30	
  HFConstHL = MSpec.R7022E_HFConst;		//新国网		//13.08.30	
  PW00002Ib = MSpec.RPW00002Ib;			//新国网		//13.08.30
  LValue = 0;

  Temp2 = ATT7022RdReg( Cmd, Data ,Devads);		//11.08.03
  if(( *(Data +2) & 0x80 ) == 0) i = 0;
  else
  {
    *(Data+2) ^= 0xff;
    *(Data+1) ^= 0xff;
    *Data ^= 0xff;
  }
  Temp = *(Data + 1) * 256 + *Data;
  Value = (unsigned long)*(Data + 2) * 65536;
  LValue = Temp;
  LValue += Value;
  
  if( i == 1 && LValue) 	LValue += 1;
  
  LValue = LValue * 100;
  Value = LValue;
  switch( Cmd )
  {
  case ATPWPA:
  case ATPWPB:
  case ATPWPC:
  case ATPWQA:		
  case ATPWQB:	
  case ATPWQC:		
  case ATPWSA:		
  case ATPWSB:		
  case ATPWSC:	
//新国网		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
////LValue = ( LValue * 1265625 * 10 ) / ( (unsigned long)HFConstHL * PS32 * 4096 );		//PS32大了10倍
//LValue = ( LValue * 6328125 ) / ( (unsigned long)HFConstHL * PS32 * 2048 );		//PS32大了10倍
//#else
////LValue = ( LValue * 253125 * 10 ) / ( (unsigned long)HFConstHL * PS32 * 8192 );			//Lvalue = Lvalue*2.592*10^10/(HFconst*EC*2^23)
    LValue = ( LValue * 1265625 ) / ( (unsigned long)HFConstHL * PS32 * 4096 );			//Lvalue = Lvalue*2.592*10^10/(HFconst*EC*2^23)
//#endif		
    Value = LValue;		
#if ( LinkMode == Phase3Wire4 )
//				if( Value < (PW00002Ib/3) ) {Value = 0; i =0;}               //0.5W       
    if( Value < (PW00002Ib) ) {Value = 0; i =0;}               //0.5W  	//新国网		//13.08.30          
#else
//				if( Value < PW00002IbPh ) {Value = 0; i =0;}               //0.5W       
    //if( Value < PW00002Ib ) {Value = 0; i =0;}               //0.5W   	//新国网		//13.08.30    
#endif
    break;	
  case ATPWPZ:	
  case ATPWQZ:		
  case ATPWSZ:	 
//新国网		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
////LValue = ( LValue * 1265625 * 10 ) / ( (unsigned long)HFConstHL * PS32 * 2048 );		//PS32大了10倍
//LValue = ( LValue * 6328125 ) / ( (unsigned long)HFConstHL * PS32 * 1024 );		//PS32大了10倍
//#else
////				LValue = ( LValue * 253125 * 10 ) / ( (unsigned long)HFConstHL * PS32 * 4096 );			//Lvalue = Lvalue*2*2.592*10^10/(HFconst*EC*2^23)
    LValue = ( LValue * 1265625 ) / ( (unsigned long)HFConstHL * PS32 * 2048 );			//Lvalue = Lvalue*2*2.592*10^10/(HFconst*EC*2^23)
//#endif
    Value = LValue;		
    if( Value < PW00002Ib ) {Value = 0; i =0;}               //0.5W       
    break;
//		case ATYUaUb:
//		case ATYUaUc:
//		case ATYUbUc:
  case ATUrmsA:		
  case ATUrmsB:		
  case ATUrmsC:	
    //LValue = ( LValue * 10 ) / 8192;
    LValue = ( LValue*10) / 65536;
    Value = LValue;
    break;
  case ATIrmsZ:	
  //Value = ( Value * 1000 ) / 4096;
//						LValue = ( LValue * 125 ) / 512;
//						LValue = ( LValue * 125 ) / ( 512 * NConst ) ;

//						LValue *= 125; 
//						LValue /= 512;
//                       LValue /= NConst;

//						LValue *= A100Ib;									//新国网	//13.08
    LValue *= MSpec.RBaseCurrent;						//新国网	//13.08.30
    LValue /= 122880;		//2^12*30=4096*30=122880
    Value = LValue;
//                      if( Value < A0002Ib ) Value = 0;
    if( Value < (A0001Ib/3) ) Value = 0;
    Temp = (unsigned short)((unsigned long)Value%100);
    Value /= 100;
    if( Temp >= 50 ) Value += 1;
    //Long_BCD4( Point, (unsigned long)Value );
   // RAM_Write( Data, Point, 4 );
   // return i;
    break;
  case ATIrmsA:		
  case ATIrmsB:		
  case ATIrmsC:		
//case ATIrmsZ:	
  case RmsADC7:
//Value = ( Value * 1000 ) / 8192;
//LValue = ( LValue * 125 ) / 1024;
//LValue = ( LValue * 125 ) / (1024 * NConst );

//LValue *= 125;
//LValue /= 1024;
//LValue /= NConst;

//LValue *= A100Ib;									//新国网	//13.08.30
    LValue *= MSpec.RBaseCurrent;						//新国网	//13.08.30
    LValue /= 16384;
    //  LValue /= 245760;	//2^13*30=8192*30=245760  
    Value = LValue;
//Value = ( Value * 10000 ) / 8192;
//Value = ( Value * 625 ) / 512;
//if( Value < A002Ib ) Value = 0;
    if( Value < A0002Ib ) Value = 0;
    Temp = (unsigned short)((unsigned long)Value%100);
    Value /= 100;
    if( Temp >= 50 ) Value += 1;	
//if( Value < 100 ) Value = 0;
 //   Long_BCD4( Point, (unsigned long)Value );
//Temp = *Point;
//RAM_Write( Point, Point+1, 3 );
//if( Temp >= 0x50 ) _BCD3INC( Point );
    //RAM_Write( Data, Point, 4 );
    //return i;
    break;
  case ATFreq:	
    // LValue = ( LValue * 100 ) / 8192;
    LValue = ( LValue * 10 ) / 8192;
    Value = LValue;
    break;	
  case ATFactorA:	
  case ATFactorB:	
  case ATFactorC:	
  case ATFactorZ:	//Value = ((Value/4) * 1000 ) / 2097152;          //先除4防溢出
    LValue = ( LValue * 250 ) / 2097152;          //先除4防溢出
    Value = LValue;
    //if( Value == 999 ) Value = 1000;
    break;
  case ATAngleA:	
  case ATAngleB:	
  case ATAngleC:	
//case ATAngleZ:	
  case ATYUaUb:
  case ATYUaUc:
  case ATYUbUc:
    Value = (Value * 10 * 180 ) / 1048576;           
    LValue = (LValue * 225 ) / 131072;
    Value = LValue;
    break;
  default: 
    break;
  }
  if( Value < 50 ) Value = 0;
	//Long_BCD4( Point, (unsigned long)Value );
	//Temp = *Point;
	//RAM_Write( Point, Point+1, 3 );
	//if( Temp >= 0x50 ) _BCD3INC( Point );
	//RAM_Write( Data, Point, 3 );
  RAM_Write( Data, (char*)&Value, 4 );	
  if( Temp2 != 0 ) return 2;			//11.08.03		读底层数据有误
  else return i;						//11.08.03		读底层数据有误
}

short Read_ATTAmper( unsigned char Cmd, unsigned char* Data ,unsigned short Devads )
{
	unsigned short	i=1,Temp;
	unsigned long Value;
	unsigned long long LValue;
	unsigned long A0002Ib;			//新国网		//13.08.30
	unsigned char Buff[4];
	unsigned char* Point;
	unsigned char Temp2=0;				//11.08.03
	
	Point = Buff;
	
	LValue = 0;

	A0002Ib = MSpec.RBaseCurrent / 500;	//新国网		//13.08.30
	Temp2 = ATT7022RdReg( Cmd, Data ,Devads);		//11.08.03	
	if(( *(Data +2) & 0x80 ) == 0) i = 0;
	else    {
	            *(Data+2) ^= 0xff;     
	            *(Data+1) ^= 0xff;     
	            *Data ^= 0xff;     	                	                
	        }   
	Temp = *(Data + 1) * 256 + *Data;
	Value = (unsigned long)*(Data + 2) * 65536;
	LValue = Temp;
	LValue += Value;

	if( i == 1 ) 	LValue += 1;
	LValue = LValue * 100;
	Value = LValue;
	switch( Cmd )
	{
		case ATUrmsA:		
		case ATUrmsB:		
		case ATUrmsC:	
						LValue = ( LValue * 10 ) / 8192;
						Value = LValue;
                        break;
		case ATIrmsA:		
		case ATIrmsB:		
		case ATIrmsC:		
		case RmsADC7:
//						LValue *= A100Ib;									//新国网	//13.08.30
						LValue *= MSpec.RBaseCurrent;						//新国网	//13.08.30
						LValue /= 245760;		//2^13*30=8192*30=245760  

                        Value = LValue;
						if( Value < A0002Ib ) Value = 0;

						Temp = (unsigned short)((unsigned long)Value%100);
						Value /= 100;
						if( Temp >= 50 ) Value += 1;	
						Long_BCD4( Point, (unsigned long)Value );
						RAM_Write( Data, Point, 4 );
						return i;
//						break;	
		default: break;
	}
	if( Value < 100 ) Value = 0;
	Long_BCD4( Point, (unsigned long)Value );
	Temp = *Point;
	RAM_Write( Point, Point+1, 3 );
	if( Temp >= 0x50 ) _BCD3INC( Point );
	RAM_Write( Data, Point, 3 );
	if( Temp2 != 0 ) return 2;			//11.08.03		读底层数据有误
	else return i;						//11.08.03		读底层数据有误
}

	
void ATT7022EStateCheckRun( unsigned short Devads )
{
	unsigned char Buff[50];
	unsigned char* Point;
//	unsigned short TempInt1;
//	unsigned short TempInt2;
	unsigned char Temp;

	Point = Buff;

    if(SM.CalibCount == CALIBCOUNT)				
    {
		*Point =0;
		*(Point+1) =0;
		*(Point+2) =0;
		
		*Point = 0x5A;
		ATT7022WtReg( 0xC9, Point ,Devads);
		*Point = 0x0;	
		ATT7022WtReg( 0xC3, Point ,Devads);
		ATT7022WtReg( 0x80, Point ,Devads);
		
		*(Point+2) = 0x0;	
		
//		*Point = 0xFE;
		*Point = 0x7E;						//ADC chop关闭	//Test
		*(Point+1) = 0xB9;
//		*(Point+1) = 0xB8;					//Femu=1.8432M			//15.09.06
		ATT7022WtReg( ModeCfg+128, Point ,Devads);	//模式配置寄存器
		
		*Point = 0x00;						//电压通道2倍增益、电流通道1倍增益
//		*(Point+1) = 0x01;
		*(Point+1) = 0x00;				//12..01.18  ADC1倍
		ATT7022WtReg( PGACtrl+128, Point ,Devads);	//ADC增益配置寄存器
		
#if( LinkMode == Phase3Wire4 )			//ATChk
#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
		*Point = 0xCC;						//代数和、能量寄存器读后清0	//视在采用RMS		//14.09.04
#else
		*Point = 0xC4;						//代数和、能量寄存器读后清0
#endif
#else
		*Point = 0x84;						//代数和、能量寄存器读后清0	
#endif
		*(Point+1) = 0xF8;					//选择功率作为潜动起动判断依据
		ATT7022WtReg( EMUCfg+128, Point ,Devads);	//EMU单元配置
		
//		*Point = HFConstL;													//新国网		//13.08.30	
//		*(Point+1) = HFConstH;												//新国网		//13.08.30	
		RAM_Write( Point, (unsigned char*)&MSpec.R7022E_HFConst, 2 );		//新国网		//13.08.30	
		ATT7022WtReg( REGHFConst+128, Point ,Devads);	//高频脉冲输出
		
		*Point = 0x00;
		*(Point+1) = 0x00;
		ATT7022WtReg( QPhscal+128, Point ,Devads);	//无功相位校正寄存器
		
//#if (( RTCCHIP == RX8025 )||( RTCCHIP == RX8025T ))	//8025T用7022内部温度传感器
		*Point = 0x37;							//7022E必须增加温度补偿
//#else
//		*Point = 0x27;
//#endif
		*(Point+1) = 0x34;
		ATT7022WtReg( ModuleCFG+128, Point ,Devads);	//模拟模块使能寄存器
		
//      *Point = Pstartup % 256;											//新国网		//13.08.30		
//      *(Point+1) = Pstartup / 256;										//新国网		//13.08.30	
		RAM_Write( Point, (unsigned char*)&MSpec.R7022E_PStartup, 2 );		//新国网		//13.08.30	
		ATT7022WtReg( Pstart+128, Point,Devads );	//模拟模块使能寄存器
		
//      *Point = Istart % 256;							//13.07.01			//新国网		//13.08.30	
//      *(Point+1) = Istart / 256;											//新国网		//13.08.30		
		RAM_Write( Point, (unsigned char*)&MSpec.R7022E_IStart, 2 );		//新国网		//13.08.30	
		ATT7022WtReg( Istartup+128, Point ,Devads);	//电流启动阀值寄存器

#if ( NEW7022E == YesCheck )
//	*Point = 0xF7;												
//	*(Point+1) = 0xFE;
	*Point = 0x11;												
	*(Point+1) = 0xFF;
	ATT7022WtReg( TCcoffA+128, Point ,Devads);		//调整温度补偿系数		//13.08.09

//	*Point = 0x1B;												
//	*(Point+1) = 0x2F;
	*Point = 0x53;												
	*(Point+1) = 0x2B;
	ATT7022WtReg( TCcoffB+128, Point ,Devads);		//调整温度补偿系数		//13.08.09

//	*Point = 0xDB;												
//	*(Point+1) = 0x10;
	*Point = 0x83;												
	*(Point+1) = 0xD4;
	ATT7022WtReg( TCcoffC+128, Point ,Devads);		//调整温度补偿系数		//13.08.09

        *Point = 0x02;						//VrefAotu_en = 1  //温度自动补偿使能
        *(Point+1) = 0x00;
		ATT7022WtReg( EMCfg+128, Point ,Devads);	//新增加的算法控制寄存器
#endif

		SM.CalibCount += 1;	
		*(Point+2) = 0x00;					//关闭写校表数据功能
		*(Point+1) = 0x00;
		*Point = 0x01;
		ATT7022WtReg( 0xC9, Point ,Devads);

		SM.AT7022ChkSumCnt = 0;				
	}	
	else  	
	{			
		if( SM.CalibCount != CALIBCOUNT1 )												
		{																				
			if(	SM.AT7022ChkSumCnt != 0 ) SM.AT7022ChkSumCnt--;				
			else 																		
			{									
				Temp = 0;
				ATT7022RdReg( ATChkSum1, Point ,Devads);												
				if( IsAllData( Point, 3, 0x00 ) == 0 ) ;
				else
				{																				
					if( Data_Comp(Point, Para.RAT7022ChkSum, 3) != 0 ) 
                    {
                      	ATT7022RdReg( ATChkSum1, Point+3 ,Devads);										
						if( IsAllData( Point+3, 3, 0x00 ) == 0 ) ;
                      	else
                      	{
                      		if( Data_Comp(Point+3, Para.RAT7022ChkSum, 3) != 0 ) Temp = 0x55;	
                      	}	
                    }
                    else Temp = 0;  
					if((Temp!=0 )&&(Data_Comp(Point, Point+3, 3) == 0))
					{
						ATT7022Init(Devads);		
						return;
					}	
				}	 																								
#if ( NEW7022E == YesCheck )
				Temp = 0;
				ATT7022RdReg( ATChkSum2, Point ,Devads);												
				if( IsAllData( Point, 3, 0x00 ) == 0 ) ;
				else
				{																				
					if( Data_Comp(Point, Para.RAT7022ChkSum2, 3) != 0 ) 
                    {
                      	ATT7022RdReg( ATChkSum2, Point+3 ,Devads);										
						if( IsAllData( Point+3, 3, 0x00 ) == 0 ) ;
                      	else
                      	{
                      		if( Data_Comp(Point+3, Para.RAT7022ChkSum2, 3) != 0 ) Temp = 0x55;	
                      	}	
                    }
                    else Temp = 0;  
					if((Temp!=0 )&&(Data_Comp(Point, Point+3, 3) == 0)) ATT7022Init( Devads);			
				}	 																								
#endif
			}																							
		}																								
	}			
}

short GetATT7022ECalibrateReg( unsigned char* ComBuf ,unsigned short Devads)
{
	unsigned char Buff[10];
	unsigned char *Point;
	unsigned short 	Addr;
  	unsigned char Err_Byte=0;

	Point = Buff;
	Addr = (short)*(ComBuf+Rs_IDHPtr)*256+ (short)*(ComBuf+Rs_IDLPtr);

#if ( NEW7022E == YesCheck )
	if((( Addr >= 0xF800 )&&( Addr <= 0xF839 ))||(( Addr >= 0xF860 )&&( Addr <= 0xF871 )))
#else
	if(( Addr >= 0xF800 )&&( Addr <= 0xF839 ))
#endif
	{
		Addr &= 0x00ff;
		*Point = 0x5A;
		*(Point+1) = 0;
		*(Point+2) = 0;
		ATT7022WR( 0xC6, Point, 3 ,Devads);	   	
		ATT7022Rd( Addr, ComBuf + Rs_DataPtr, 3 ,Devads);
		*Point = 0x0;
		*(Point+1) = 0;
		*(Point+2) = 0;
		ATT7022WR( 0xC6, Point, 3 ,Devads);
		*(ComBuf+Rs_LenPtr) = 4;
		return Err_Byte;
	}
	else return -1;	
}	

//#if ( NEW7022E == YesCheck )
////重置TPSoffset值
//void UpdateTPSoffset(void)				
//{
//	unsigned char Buff[6];
//	unsigned char* Point;
//	unsigned long Value;
//	unsigned short TempInt1;
//	unsigned short TempInt2;
//	
//	Point = Buff;
//	
//	WDTCTL = WDT_ARST_1000;	
//	
//	Value = 0;
//	RAM_Write( (unsigned char*)&Value, Para.RAT7022ChkSum2, 3 );			
//	
//	ATT7022RdReg( ATTempD, Point );
//	RAM_Write( (unsigned char*)&TempInt2, Point, 2 );
//	
//	*Point = 0x5A;
//	*(Point+1) = 0;
//	*(Point+2) = 0;
//	ATT7022WR( 0xC6, Point, 3 );	   	
//	ATT7022Rd( TPSoffset, Point, 3 );
//	RAM_Write( (unsigned char*)&TempInt1, Point, 2 );
//	*Point = 0x0;
//	*(Point+1) = 0;
//	*(Point+2) = 0;
//	ATT7022WR( 0xC6, Point, 3 );
//	
//	*(Point+2) = 0x00;					
//	*(Point+1) = 0x00;
//	*Point = 0x5A;
//	ATT7022WtReg( 0xC9, Point );
//	RAM_Write( Point, (unsigned char*)&TempInt2, 2 );
//	ATT7022WtReg( TPSoffset+128, Point );	
//	*(Point+2) = 0x00;					
//	*(Point+1) = 0x00;
//	*Point = 0x01;						//关闭写校表数据功能
//	ATT7022WtReg( 0xC9, Point );
//		
//	Value += TempInt2;
//	Value -= TempInt1;
//	RAM_Write( Para.RAT7022ChkSum2, (unsigned char*)&Value, 3 );			
//	E2P_WData( AT7022ChkSum2, Para.RAT7022ChkSum2, 3 );				
//
//}
//#endif

#endif