#include <math.h>
#include <stdio.h>
#include <string.h>
#include "ht6xxx_reset.h"
#include "TypeMeter.h"
#include "TypeRAM.h"
//#include <stdlib.h>
#include "TypeE2p.h"
//#include "msp430x54x.h"
//#include "In430.h"
#include "ht6xxx_lib.h"
//#include "DLT698.h"					
#include "Port.h"
#include "TypeFRAM.h"
#include "TypeFlash.h"
#include "Data.h"
#include "Interrupt.h"
#include "General.h"
#include "Time.h"
#include "Mem.h"
#include "Power.h"
#include "Initial.h"
#include "serial.h"
#include "IEC101.h"

#define EC_E2_W E2P_WAdj
#define EC_E2_R E2P_RAdj

//第一个字节指针
//第二个字节数量
void Pn_Event_Save(int ch,int phase,unsigned char flag)
{
  unsigned short m_e2_ptr,m_e2_buf,year;
  unsigned char ptr_buf[4],tmp_buf[48];
  MoveCurrentTimeBCD_Hex();
  tmp_buf[0]=Clk.SecH;
  tmp_buf[1]=Clk.MinH;
  tmp_buf[2]=Clk.HourH;
  tmp_buf[3]=Clk.DayH;
  tmp_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  tmp_buf[5]=year-2000;
  
  if(flag)
  {
    memcpy(tmp_buf+6,&SM.PQNum[ch][phase],4);
    memcpy(tmp_buf+10,&Energy_Data[ch],32);
    LoadRecord(CH0_PAP_USEADDR+ch*78+phase*3,tmp_buf);
  }
  else
  {
    memcpy(tmp_buf+6,&Energy_Data[ch],32);
    LoadRecord(CH0_PAN_USEADDR+ch*78+phase*3,tmp_buf);
  }
}

int GetPn_Event_num(int ch,int phase)
{
  int Num;
  Num = Get_Record_Num(CH0_PAP_USEADDR+ch*78+phase*3);
  return Num;
}

int GetPn_Event_Record(int ch,int phase,int No,unsigned char *buf)
{
  ReadRecord(CH0_PAP_USEADDR+ch*78+phase*3,buf,No);
  ReadRecord(CH0_PAN_USEADDR+ch*78+phase*3,buf+42,No);
  return 88;
}

void Pt_Event_Save(int ch)
{
  unsigned short m_e2_ptr,m_e2_buf,year;
  unsigned char ptr_buf[4],tmp_buf[48];
  MoveCurrentTimeBCD_Hex();
  tmp_buf[0]=Clk.SecH;
  tmp_buf[1]=Clk.MinH;
  tmp_buf[2]=Clk.HourH;
  tmp_buf[3]=Clk.DayH;
  tmp_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  tmp_buf[5]=year-2000;
  memcpy(tmp_buf+6,&SM.PQNum[ch][3],4);
  memcpy(tmp_buf+10,&Energy_Data[ch],32);
  LoadRecord(CH0_PTD_USEADDR+ch*78,tmp_buf);
}

int GetPt_Event_num(int ch)
{
  int Num;
  Num = Get_Record_Num(CH0_PTD_USEADDR+ch*78);
  return Num;
}

int GetPt_Event_Record(int ch,int No,unsigned char *buf)
{
  ReadRecord(CH0_PTD_USEADDR+ch*78,buf,No);
  return 42;
}

short Save_Data(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  memcpy(tmp_buf,Time_buf,6);
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    LoadRecord(LOAD0_USEADDR+i*30,tmp_buf);
  }
  return 0;
}

short Get_LoadData(int No,int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(LOAD0_USEADDR+30*ch,buf,No);
  return ONE_RECORD_LEN;
}

int Load_Record_Num()
{
  int Num;
  Num = Get_Record_Num(LOAD0_USEADDR);
  return Num;
}

short Read_LastData(int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(LOAD0_USEADDR+30*ch,buf,0);
  return ONE_RECORD_LEN;
}

short Save_DayData(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    LoadRecord(FRZD0_USEADDR+i*30,tmp_buf);
  }
  return 0;
}

short Get_DayData(int No,int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(FRZD0_USEADDR+30*ch,buf,No);
  return ONE_RECORD_LEN;
}

int Day_Record_Num()
{
  int Num;
  Num = Get_Record_Num(FRZD0_USEADDR);
  return Num;
}

short Save_HourData(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    LoadRecord(SHRP0_USEADDR+i*30,tmp_buf);
  } 
  return 0;
}

short Get_HourData(int No,int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(SHRP0_USEADDR+30*ch,buf,No);
  return ONE_RECORD_LEN;
}

int Hour_Record_Num()
{       
  int Num;
  Num = Get_Record_Num(SHRP0_USEADDR);
  return Num;
}

short Save_MonthData(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
    LoadRecord(MOND0_USEADDR+i*30,tmp_buf);
  } 
  return 0;
}

short Get_MonthData(int No,int ch,unsigned char *buf)
{
  if(ch>8)
    return 0;
  ReadRecord(MOND0_USEADDR+30*ch,buf,No);
  return E2ONE_RECORD_LEN;
}

int Month_Record_Num()
{
  int Num;
  Num = Get_Record_Num(MOND0_USEADDR);
  return Num;
}

short Save_RandData(unsigned char *Time_buf)
{
  int i;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<MAX_CH_NUM;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
    LoadRecord(RAND0_USEADDR+i*30,tmp_buf);
  } 
  return 0;
}
short Get_RandData(int No,int ch,unsigned char *buf)
{
  if(ch>MAX_CH_NUM)
    return 0;
  ReadRecord(RAND0_USEADDR+30*ch,buf,No);
  return E2ONE_RECORD_LEN;
}
int Rand_Record_Num()
{
  int Num;
  Num = Get_Record_Num(RAND0_USEADDR);
  return Num;
}

  
    
int Get_LoadFile_Msg_Len(void)
{
  int Len,rec_ptr;
  rec_ptr=Load_Record_Num();
  Len = 51+rec_ptr*189; 
  return Len; 
}

int Get_LoadFile_Xml_Len(void)
{
  int Len,rec_ptr;
  rec_ptr=Load_Record_Num();
  Len = 100+rec_ptr*256; 
  return Len; 
}

void EC_MeasA(void)
{
  int i;
  unsigned short* ECRamBufAdsPtr;
  unsigned char* ECRamBufChkAdsPtr;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  unsigned int Ps2;
  Ps2 = MSpec.RMeterConst/1000;
  for( i=0;i<ECUnitNum;++i)
  {
    ECRamBufAdsPtr = ECRgTab[i].ECRamBufAds;
    ECRamBufChkAdsPtr = ECRgTab[i].ECRamBufChkAds;
    ECRAds = ECRgTab[i].ECRAds;
    if( *ECRamBufChkAdsPtr != ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 ) )
    {
      *ECRamBufAdsPtr = 0;
      *ECRamBufChkAdsPtr = 0;
    }
    if(*ECRamBufAdsPtr>=Ps2)
    {
      *ECRAds += (*ECRamBufAdsPtr/Ps2);//*ECRamBufAdsPtr;
      ECEAds = ECRgTab[i].ECEAds;
      EC_E2_W(ECEAds,(unsigned char*)ECRAds,4);
      *ECRamBufAdsPtr %= Ps2;
      *ECRamBufChkAdsPtr = ChkNum((unsigned char*)ECRamBufAdsPtr,2);
    }
  }
}
void EC_ClearA(void)
{
  int i;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  unsigned char tmpbuf[8];
  memset(tmpbuf,0,8);
  for( i=0;i<ECUnitNum;++i)
  {
    ECRAds = ECRgTab[i].ECRAds;
    ECEAds = ECRgTab[i].ECEAds;
    EC_E2_W(ECEAds,tmpbuf,4);
    *ECRAds = 0;
  }
}
void Read_E2R()
{
  int i;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  for( i=0;i<ECUnitNum;++i)
  {
    ECRAds = ECRgTab[i].ECRAds;
    ECEAds = ECRgTab[i].ECEAds;
    EC_E2_R((unsigned char*)ECRAds,ECEAds,4);
  }
}

void Clear_E2R(int chan)
{
  int i,j;
  unsigned long* ECRAds;
  unsigned short ECEAds;
  unsigned char tmpbuf[8];
  memset(tmpbuf,0,8);
  if(chan==0)
  {
    chan=ECUnitNum;
    i = 0; 
  }
  else
  {
    i = 8*(chan-1);
    chan = 8*chan;
  }
  while(i<chan)
  {
    ECRAds = ECRgTab[i].ECRAds;
    ECEAds = ECRgTab[i].ECEAds;
    EC_E2_W(ECEAds,tmpbuf,4);
    ++i;
  }
}

void Clear_EVT2R(int chan)
{
  int i;
  unsigned char ptr_buf[4];
  memset(ptr_buf,0,4);
  if(chan==0)
  {
    chan=26*MAX_CH_NUM;
    i = 0; 
  }
  else
  {
    i = 26*(chan-1);
    chan = 26*chan;
  }
  while(i<chan)
  {
    EC_E2_W(LOAD0_USEADDR+3*i,ptr_buf,2);
    ++i;
  }
}

void Read_E2R1()
{
  int i;
  unsigned char ptr_buf[4];
  memset(ptr_buf,0,4);
  for( i=0;i<26*MAX_CH_NUM;++i)
  {
    EC_E2_W(LOAD0_USEADDR+3*i,ptr_buf,2);
  }
}

void ProcHalfSec(void)
{
  int i;
  unsigned int tmp_p;
  short Quad;
  Flag.Clk &= ~F_HalfSec;
  if((Flag.Power & F_PwrUp) == 0)
      return;
  HT_GPIO_BitsToggle(HT_GPIOA,GPIO_Pin_4);
  for(i=0;i<MAX_CH_NUM;++i)
  {
    ATT7022RdReg(ATVoltFlag,(unsigned char*)&(SM.State[i]),i);
    ATT7022RdReg(ATPZ,(unsigned char*)&tmp_p,i);
    tmp_p &= 0xffffff;
    if(tmp_p)
    if(SM.State[i]&0x1000)
    {
      ECP.PL_CumPn[i] += tmp_p;
      ECP.PL_ChkPn[i]=ChkNum((unsigned char*)&ECP.PL_CumPn[i],2);
    }
    else
    {
      ECP.PL_CumPp[i] += tmp_p;
      ECP.PL_ChkPp[i]=ChkNum((unsigned char*)&ECP.PL_CumPp[i],2);
    }
    ATT7022RdReg(ATQZ,(unsigned char*)&tmp_p,i);
    tmp_p &= 0xffffff;
    if(tmp_p)
    {
      if(SM.State[i]&0x2000)
      {
        ECP.PL_CumQn[i] += tmp_p;
        ECP.PL_ChkQn[i]=ChkNum((unsigned char*)&ECP.PL_CumQn[i],2);
      }
      else
      {
        ECP.PL_CumQp[i] += tmp_p;
        ECP.PL_ChkQp[i]=ChkNum((unsigned char*)&ECP.PL_CumQp[i],2);
      }
      Quad=GetMQuad((SM.State[i]>>12)&0x3,0);
      //Quad = (SM.State[i]>>12)&0x3;
      ECP.PL_CumQ[i][Quad] += tmp_p;
      ECP.PL_ChkQ[i][Quad]=ChkNum((unsigned char*)&ECP.PL_CumQ[i][Quad],2);
    }
  }
  EC_MeasA();
}
/***************************************************
	Working for Every Second
	1. Decreasing Second Timer
	2. Energy Measurement
	3. Maxium Demand Measurement
	4. Even Log File
***************************************************/
void ProcSec(void)
{
  char Buff[8];
  char* Point;
  int i,j,flag_p;
  signed long *si_val;

  Flag.Clk &= ~F_Sec;
  Point = Buff;
  
  memset( Point,0,6 );
  
  if((Flag.Power & F_PwrUp) != 0)
  {
    if(SM.TestCnt != 0)
    {
       SM.TestCnt--;
    }
    if(SM.TestDisCnt != 0)
    {
       SM.TestDisCnt--;
    }
    
    for(i=0;i<MAX_CH_NUM;++i)
    {
      ATT7022RdReg(PFlag,(unsigned char*)&(SM.PQFlag[i]),i);
      Read_ATTValue(ATPWPA,(unsigned char *)&Energy_Data[i].Pa,i);
      Read_ATTValue(ATPWPB,(unsigned char *)&Energy_Data[i].Pb,i);
      Read_ATTValue(ATPWPC,(unsigned char *)&Energy_Data[i].Pc,i);
      Read_ATTValue(ATPWPZ,(unsigned char *)&Energy_Data[i].Pt,i);
      if(SM.PQFlag[i]&0x1)
      {
        Energy_Data[i].Pa *=-1;
      }
      if(SM.PQFlag[i]&0x2)
      {
        Energy_Data[i].Pb *=-1;
      }
      if(SM.PQFlag[i]&0x4)
      {
        Energy_Data[i].Pc *=-1;
      }
      if(SM.PQFlag[i]&0x8)
      {
        Energy_Data[i].Pt *=-1;
      }
      
      Real_Data[i].Pa = Energy_Data[i].Pa;
      Real_Data[i].Pb = Energy_Data[i].Pb;
      Real_Data[i].Pc = Energy_Data[i].Pc;
      Real_Data[i].Pt = Energy_Data[i].Pt;

      Read_ATTValue(ATPWQA,(unsigned char *)&Real_Data[i].Qa,i);
      Read_ATTValue(ATPWQB,(unsigned char *)&Real_Data[i].Qb,i);
      Read_ATTValue(ATPWQC,(unsigned char *)&Real_Data[i].Qc,i);
      Read_ATTValue(ATPWQZ,(unsigned char *)&Real_Data[i].Qt,i);
      if(SM.PQFlag[i]&0x10)
      {
        Real_Data[i].Qa *=-1;
      }
      if(SM.PQFlag[i]&0x20)
      {
        Real_Data[i].Qb *=-1;
      }
      if(SM.PQFlag[i]&0x40)
      {
        Real_Data[i].Qc *=-1;
      }
      if(SM.PQFlag[i]&0x80)
      {
        Real_Data[i].Qt *=-1;
      }
    
      Read_ATTValue(ATPWSA,(unsigned char *)&Real_Data[i].Sa,i);
      Read_ATTValue(ATPWSB,(unsigned char *)&Real_Data[i].Sb,i);
      Read_ATTValue(ATPWSC,(unsigned char *)&Real_Data[i].Sc,i);
      Read_ATTValue(ATPWSZ,(unsigned char *)&Real_Data[i].St,i);
      Read_ATTValue(ATFreq,(unsigned char *)&Real_Data[i].AFreq,i);
      Read_ATTValue(ATUrmsA,(unsigned char *)&Real_Data[i].Ua,i);
      Read_ATTValue(ATUrmsB,(unsigned char *)&Real_Data[i].Ub,i);
      Read_ATTValue(ATUrmsC,(unsigned char *)&Real_Data[i].Uc,i);
      Read_ATTValue(ATIrmsA,(unsigned char *)&Real_Data[i].Ia,i);
      Read_ATTValue(ATIrmsB,(unsigned char *)&Real_Data[i].Ib,i);
      Read_ATTValue(ATIrmsC,(unsigned char *)&Real_Data[i].Ic,i);
      Read_ATTValue(ATFactorA,(unsigned char *)&Real_Data[i].Pfa,i);
      Read_ATTValue(ATFactorB,(unsigned char *)&Real_Data[i].Pfb,i);
      Read_ATTValue(ATFactorC,(unsigned char *)&Real_Data[i].Pfc,i);
      Read_ATTValue(ATFactorZ,(unsigned char *)&Real_Data[i].Pft,i);
      Read_ATTValue(ATAngleA,(unsigned char *)&SM.Angle_Ia[i],i);
      Read_ATTValue(ATAngleB,(unsigned char *)&SM.Angle_Ib[i],i);
      Read_ATTValue(ATAngleC,(unsigned char *)&SM.Angle_Ic[i],i);
    }
#if  1   
    for(i=0;i<MAX_CH_NUM;++i)
    {
      flag_p = SM.PQFlag[i]^SM.PQFlag_b[i];
      SM.PQFlag_b[i]=SM.PQFlag[i];
      if((flag_p&0xf))
      {
        si_val = &Real_Data[i].Pa;
        for(j=0;j<4;++j)
        {
          if(flag_p&(1<<j))
          {
            if(abs(si_val[j])>40)
            {
              SM.P_Time[i][j]=60;
            }
          }
        }
      }
      for(j=0;j<3;++j) //功率方向
      {
        if(SM.P_Time[i][j])
        {
          --SM.P_Time[i][j];
          if(SM.P_Time[i][j]==0)
          {
            Pn_Event_Save(i,j,SM.PQFlag[i]&(1<<j));
          }
        }
      }
      
      if(SM.P_Time[i][3]) //潮流
      {
        --SM.P_Time[i][3];
        if(SM.P_Time[i][3]==0)
        {
            Pt_Event_Save(i);
        }
      }
    }
#endif
  }
  else
  {
    HT_GPIO_BitsToggle(HT_GPIOA,GPIO_Pin_4);
  }
}


/***************************************************
	Working for Every Minute
	1. Decreasing Minute Timer
	2. Fee judgement
	3. Load Profile
***************************************************/
void ProcMin(void)
 {
    unsigned char Time_buf[8];
    unsigned short year;
    Flag.Clk &= ~F_Min;
    if((Flag.Power & F_PwrUp) == 0)
      return;
    //GetTime();
    MoveCurrentTimeBCD_Hex();
    Time_buf[0]=Clk.SecH;
    Time_buf[1]=Clk.MinH;
    Time_buf[2]=Clk.HourH;
    Time_buf[3]=Clk.DayH;
    Time_buf[4]=Clk.Month;
    year = Clk.YearH;
    year = year*256 + Clk.YearL;
    Time_buf[5]=year-2000;
    if((Clk.MinH%15)==0)
    {
      Save_Data(Time_buf);
    }
    
    Save_RandData(Time_buf);
}	

/***************************************************
	Working for Every Minute
	1. Decreasing Minute Timer
***************************************************/
void ProcHour(void)
{
  unsigned char Time_buf[8];
  unsigned short year;
  Flag.Clk &= ~F_Hour;
  if((Flag.Power & F_PwrUp) == 0)
      return;
  GetTime();
  MoveCurrentTimeBCD_Hex();
  Time_buf[0]=Clk.SecH;
  Time_buf[1]=Clk.MinH;
  Time_buf[2]=Clk.HourH;
  Time_buf[3]=Clk.DayH;
  Time_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  Time_buf[5]=year-2000;
  Save_HourData(Time_buf);
}

void ProcDay(void)
{
  unsigned char Time_buf[8];
  unsigned short year;
  Flag.Clk &= ~F_Day;
  if((Flag.Power & F_PwrUp) == 0)
      return;
  Time_buf[0]=Clk.SecH;
  Time_buf[1]=Clk.MinH;
  Time_buf[2]=Clk.HourH;
  Time_buf[3]=Clk.DayH;
  Time_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  Time_buf[5]=year-2000;
  Save_DayData(Time_buf);
  if(Clk.DayH==1)
  {
    Save_MonthData(Time_buf);
  }
}	

void main(void)
{
  unsigned int i;
  unsigned char flash_id[16];
  HT_FreeDog();
  //i = 5000;
  i=E2END_DATA_ADDR;
  while(i--);
  fnTarget_Init();
 // Flag.BatState=0;
  while(1)
  {	
    HT_FreeDog();
    for(i=0;i<5000;i++)
    {
      HT_FreeDog();
      __NOP();
    }
    VarInit();
    GetTime();
    MoveCurrentTimeBCD_Hex();																									//10.11.22
    while (1)
    {
      HT_FreeDog();						
      if(((Flag.Power & F_PwrUp) == 0) && ( PowerCheck() == 1 ))		
      {
        PwrOnInit();	
        InitPara();			
        InitPara5();
        Serial_Open(0,9600,8,UartParity_Disable);
        //Serial_Open(0,9600,8,UartParity_EVEN);
        Load_InfoData();
	InitPara6();   
        InitIEC101Prot();
        Read_FlashID(flash_id);
#if 0        
        memset(flash_id,0x51,16);
        E2P_WFM(0,flash_id,16);
        memset(flash_id,0,10);
        E2P_RFM(flash_id,0,16);
        memset(flash_id,0x52,16);
        E2P_WFM(0,flash_id,16);
        memset(flash_id,0,10);
        E2P_RFM(flash_id,0,16);
        memset(flash_id,0,10);
        E2P_RFM(flash_id,0,16);
        memset(flash_id,0,10);
        E2P_RFM(flash_id,0,16);
       
        BlockErase(0);
        Read_Flash(flash_id,0,10);
        memset(flash_id,0xaa,10);
        Write_Flash(0,flash_id,10);
        memset(flash_id,0,10);
        Read_Flash(flash_id,0,10);
        memset(flash_id,0,10);
        Read_Flash(flash_id,0,10);
        memset(flash_id,0,10);
        Read_Flash(flash_id,0,10);
 
        memset(flash_id,0x55,16);
        E2P_PWData(0,flash_id,16);
        memset(flash_id,0,10);
        E2P_PRData(flash_id,0,16);
        memset(flash_id,0x33,16);
        E2P_WData(0,flash_id,16);
        memset(flash_id,0,10);
        E2P_RData(flash_id,0,16);
        memset(flash_id,0,10);
        E2P_WData(LoadRecord_Time,flash_id,8);
        E2P_WData(RandRecord_Time,flash_id,8);
        E2P_WData(FrzdRecord_Time,flash_id,8);
        E2P_WData(ShrpdRecord_Time,flash_id,8);
        E2P_WData(MonthdRecord_Time,flash_id,8);
#endif
        //EC_ClearA();
        //Read_E2R1();
        Read_E2R();
        for(i=0;i<MAX_CH_NUM;i++)
        {
          ATT7022Init(i);	//Test
        }
        break;
      }	
      if(((Flag.Power & F_PwrUp) != 0) && ( PowerCheck() == 0 ))
      {
//       // Flag.BatState=1;
//        //PwrDnInit();
      }	
    
      
//      if(( Flag.Clk & F_ThirdSec )) 	
//      {		
//        Flag.Clk &= ~F_ThirdSec;								
//      }
      if(Flag.Clk& F_HalfSec)
      {
        E2P_RFM(flash_id,0,16);
        ProcHalfSec();
      }
      if (Flag.Clk & F_Sec) 
      {
        ProcSec();	
      }
      if (Flag.Clk & F_Min) ProcMin();
      if (Flag.Clk & F_Hour) ProcHour();
      if (Flag.Clk & F_Day) ProcDay();
      
      if(Flag.Power & F_IrmsCheck)
      {
        Serial_Open(0,115200,8,UartParity_Disable);
        xmodemReceive();
        Serial_Open(0,9600,8,UartParity_Disable);
      }
      else
      {	
        IEC101Process();
      }			
      
      if(((Flag.Power & F_PwrUp) == 0))
      { 
        Flag.Run |= F_Hold;
        EnterHold();						
        if((HT_PMU->WAKEIF&0x001000f4))
        {	
          if((Flag.Run & F_Hold)!=0)
          {
            Flag.Run &= ~F_Hold;
            ExitHold();
            HT_FreeDog();						
          }
        }				
      }		
    }
  }
}	

