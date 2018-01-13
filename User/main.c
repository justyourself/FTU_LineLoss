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


short Save_Data(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
 // unsigned short year;
  
  E2P_RData(Record_buf,LoadRecord_Time,8); //记录指针 秒、分、时、日、月、年
  rec_ptr = Record_buf[0];
  rec_ptr %=LOAD_RECORD_NUM;
#if 0  
  tmp_buf[0]=Clk.SecH;
  tmp_buf[1]=Clk.MinH;
  tmp_buf[2]=Clk.HourH;
  tmp_buf[3]=Clk.DayH;
  tmp_buf[4]=Clk.Month;
  year = Clk.YearH;
  year = year*256 + Clk.YearL;
  tmp_buf[5]=year-2000;
#else
  memcpy(tmp_buf,Time_buf,6);
#endif  
  for(i=0;i<8;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    DataFlash_Write(i*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,tmp_buf,0,DAY_DATA_ADDR,ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=LOAD_RECORD_NUM)
    Record_buf[1]=1;
  rec_ptr %=LOAD_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+2,tmp_buf,6);
  E2P_WData(LoadRecord_Time,Record_buf,8); 
  return 0;
}

short Get_LoadData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[12];
  int rec_ptr;
  if(ch>8)
    return 0;
  E2P_RData(Record_buf,LoadRecord_Time,8); //记录指针 秒、分、时、日、月、年
  
  if(Record_buf[1])
  {
    rec_ptr = (Record_buf[0]+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=LOAD_RECORD_NUM;
  Read_Flash(buf,ch*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,ONE_RECORD_LEN);
  return ONE_RECORD_LEN;
}

int Load_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,LoadRecord_Time,8); //记录指针 秒、分、时、日、月、年
  if(Record_buf[1])
  {
    Num=LOAD_RECORD_NUM;
  }
  else
  {
    Num=Record_buf[0];
  }
  return Num;
}

short Read_LastData(int ch,unsigned char *buf)
{
  unsigned char Record_buf[12];
  int rec_ptr;
  if(ch>8)
    return 0;
  E2P_RData(Record_buf,LoadRecord_Time,8); //记录指针 秒、分、时、日、月、年
  rec_ptr = Record_buf[0];
  if(rec_ptr)
    rec_ptr--;
  else
    rec_ptr=LOAD_RECORD_NUM-1;
  rec_ptr %=LOAD_RECORD_NUM;
  Read_Flash(buf,ch*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,ONE_RECORD_LEN);
  return ONE_RECORD_LEN;
}

short Save_DayData(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  
  E2P_RData(Record_buf,FrzdRecord_Time,8); //记录指针 秒、分、时、日、月、年
  rec_ptr = Record_buf[0];
  rec_ptr %=DAY_RECORD_NUM;
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<8;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    DataFlash_Write(DAY_DATA_ADDR+i*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,tmp_buf,DAY_DATA_ADDR,HOUR_DATA_ADDR,ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=DAY_RECORD_NUM)
    Record_buf[1]=1;
  rec_ptr %=DAY_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+2,tmp_buf,6);
  E2P_WData(FrzdRecord_Time,Record_buf,8); 
  return 0;
}

short Get_DayData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[8];
  int rec_ptr;
  if(ch>8)
    return 0;
  E2P_RData(Record_buf,FrzdRecord_Time,8); //记录指针 秒、分、时、日、月、年
  
  if(Record_buf[1])
  {
    rec_ptr = (Record_buf[0]+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=DAY_RECORD_NUM;
  Read_Flash(buf,DAY_DATA_ADDR+ch*DAY_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,ONE_RECORD_LEN);
  return ONE_RECORD_LEN;
}

int Day_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,FrzdRecord_Time,8); //记录指针 秒、分、时、日、月、年
  if(Record_buf[1])
  {
    Num=DAY_RECORD_NUM;
  }
  else
  {
    Num=Record_buf[0];
  }
  return Num;
}

short Save_HourData(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  
  E2P_RData(Record_buf,ShrpdRecord_Time,9); //记录指针 秒、分、时、日、月、年
  rec_ptr = Record_buf[0];
  rec_ptr += Record_buf[1]*256;
  rec_ptr %=HOUR_RECORD_NUM;
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<8;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],ONE_RECORD_LEN-6);
    DataFlash_Write(DAY_DATA_ADDR+i*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,tmp_buf,DAY_DATA_ADDR,HOUR_DATA_ADDR,ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=HOUR_RECORD_NUM)
    Record_buf[2]=1;
  rec_ptr %=HOUR_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  Record_buf[1]=(rec_ptr>>8)&0xff;
  memcpy(Record_buf+3,tmp_buf,6);
  E2P_WData(ShrpdRecord_Time,Record_buf,9); 
  return 0;
}

int Hour_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,ShrpdRecord_Time,9); //记录指针 秒、分、时、日、月、年
  if(Record_buf[2])
  {
    Num=HOUR_RECORD_NUM;
  }
  else
  {
    Num = Record_buf[0];
    Num += Record_buf[1]*256;
  }
  return Num;
}

short Save_MonthData(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  
  E2P_RData(Record_buf,MonthdRecord_Time,8); //记录指针 秒、分、时、日、月、年
  rec_ptr = Record_buf[0];
  if(rec_ptr>=MONTH_RECORD_NUM)
    rec_ptr = 0;
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<8;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
    E2P_PWData(i*MONTH_RECORD_SIZE+rec_ptr*E2ONE_RECORD_SIZE,tmp_buf,E2ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=MONTH_RECORD_NUM)
    Record_buf[1]=1;
  rec_ptr %=MONTH_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+2,tmp_buf,6);
  E2P_WData(MonthdRecord_Time,Record_buf,8); 
  return 0;
}

short Get_MonthData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[8];
  int rec_ptr;
  if(ch>8)
    return 0;
  E2P_RData(Record_buf,MonthdRecord_Time,8); //记录指针 秒、分、时、日、月、年
  
  if(Record_buf[1])
  {
    rec_ptr = (Record_buf[0]+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=MONTH_RECORD_NUM;
  E2P_PRData(buf,ch*MONTH_RECORD_SIZE+rec_ptr*E2ONE_RECORD_SIZE,E2ONE_RECORD_LEN);
  return E2ONE_RECORD_LEN;
}

int Month_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,MonthdRecord_Time,8); //记录指针 秒、分、时、日、月、年
  if(Record_buf[1])
  {
    Num=MONTH_RECORD_NUM;
  }
  else
  {
    Num = Record_buf[0];
  }
  return Num;
}

short Save_RandData(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  
  E2P_RData(Record_buf,RandRecord_Time,8); //记录指针 秒、分、时、日、月、年
  rec_ptr = Record_buf[0];
  if(rec_ptr>=RAND_RECORD_NUM)
    rec_ptr = 0;
  memcpy(tmp_buf,Time_buf,6);  
  for(i=0;i<8;++i)
  {
    memcpy(tmp_buf+6,&Energy_Data[i],E2ONE_RECORD_LEN-6);
    E2P_PWData(RAND_DATA_ADDR+i*RAND_RECORD_SIZE+rec_ptr*E2ONE_RECORD_SIZE,tmp_buf,E2ONE_RECORD_LEN);
  }
  rec_ptr++;
  if(rec_ptr>=RAND_RECORD_NUM)
    Record_buf[1]=1;
  rec_ptr %=RAND_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+2,tmp_buf,6);
  E2P_WData(RandRecord_Time,Record_buf,8); 
  return 0;
}
short Get_RandData(int No,int ch,unsigned char *buf)
{
  unsigned char Record_buf[8];
  int rec_ptr;
  if(ch>8)
    return 0;
  E2P_RData(Record_buf,RandRecord_Time,8); //记录指针 秒、分、时、日、月、年
  
  if(Record_buf[1])
  {
    rec_ptr = (Record_buf[0]+No);
  }
  else
  {
    rec_ptr=No;
  }
  rec_ptr %=RAND_RECORD_NUM;
  E2P_PRData(buf,RAND_DATA_ADDR+ch*RAND_RECORD_SIZE+rec_ptr*E2ONE_RECORD_SIZE,E2ONE_RECORD_LEN);
  return E2ONE_RECORD_LEN;
}
int Rand_Record_Num()
{
  int Num;
  unsigned char Record_buf[12];
  E2P_RData(Record_buf,RandRecord_Time,8); //记录指针 秒、分、时、日、月、年
  if(Record_buf[1])
  {
    Num=RAND_RECORD_NUM;
  }
  else
  {
    Num = Record_buf[0];
  }
  return Num;
}

  
    
int Get_LoadFile_Msg_Len(void)
{
  int Len,rec_ptr;
  unsigned char Record_buf[16];
  E2P_RData(Record_buf,LoadRecord_Time,8); //记录指针 秒、分、时、日、月、年
  if(Record_buf[1])
     rec_ptr = LOAD_RECORD_NUM;
  else
    rec_ptr = Record_buf[0];
  Len = 51+rec_ptr*189; 
  return Len; 
}

int Get_LoadFile_Xml_Len(void)
{
  int Len,rec_ptr;
  unsigned char Record_buf[16];
  E2P_RData(Record_buf,LoadRecord_Time,8); //记录指针 秒、分、时、日、月、年
  if(Record_buf[1])
     rec_ptr = LOAD_RECORD_NUM;
  else
    rec_ptr = Record_buf[0];
  Len = 100+rec_ptr*256; 
  return Len; 
}

void ProcHalfSec(void)
{
  Flag.Clk &= ~F_HalfSec;
  HT_GPIO_BitsToggle(HT_GPIOB,GPIO_Pin_5);
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
    for(Buff[0]=0;Buff[0]<8;++Buff[0])
    {
     // Energy_Data[Buff[0]].Pn++;
      Energy_Data[Buff[0]].Pp = Buff[0];
      Energy_Data[Buff[0]].Pn +=Buff[0];
      Energy_Data[Buff[0]].Qn++;
      Energy_Data[Buff[0]].Q1++;
      Energy_Data[Buff[0]].Q2++;
      Energy_Data[Buff[0]].Pa = rand();
      Energy_Data[Buff[0]].Pb = rand();
      Energy_Data[Buff[0]].Pc = rand();
      Energy_Data[Buff[0]].Pt = Energy_Data[Buff[0]].Pa+Energy_Data[Buff[0]].Pb+Energy_Data[Buff[0]].Pc;
      Real_Data[Buff[0]].Qa++;
    }
#if 0    
    if(SM.TestDisCnt==0)
    {
        SM.TestDisCnt=2;
        //sprintf(Buff,"%05d\r\n",SM.ClockBatAD);
        Buff[0]=0xaa;
        Buff[1]=0x55;
        Buff[2]=0x00;
        Buff[3]=0x11;
        Buff[4]=0x22;
        Serial_Write(2,Buff,5);
    } 
#endif    
  }
  else
  {
    HT_GPIO_BitsToggle(HT_GPIOC,GPIO_Pin_7);
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
    //GetTime();
    MoveCurrentTimeBCD_Hex();
    //if((Clk.MinH%15)==0)
    {
      Time_buf[0]=Clk.SecH;
      Time_buf[1]=Clk.MinH;
      Time_buf[2]=Clk.HourH;
      Time_buf[3]=Clk.DayH;
      Time_buf[4]=Clk.Month;
      year = Clk.YearH;
      year = year*256 + Clk.YearL;
      Time_buf[5]=year-2000;
      Save_Data(Time_buf);
    }
    
    Save_RandData(Time_buf);
    Save_MonthData(Time_buf);
    Flag.Clk &= ~F_Min;
}	

/***************************************************
	Working for Every Minute
	1. Decreasing Minute Timer
***************************************************/
void ProcHour(void)
{
  unsigned char Time_buf[8];
  unsigned short year;
  
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
  Flag.Clk &= ~F_Hour;
}

void ProcDay(void)
{
  unsigned char Time_buf[8];
  unsigned short year;
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
  Flag.Clk &= ~F_Day;
}	

void main(void)
{
  unsigned short i;
  unsigned char flash_id[16];
  HT_FreeDog();
  i = 5000;
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
       // Flag.BatState=0;
        PwrOnInit();	
        InitPara();			
        InitPara5();
        Serial_Open(2,9600,8,UartParity_Disable);
	InitPara6();
       // Serial_Write(2,"Start Debug\r\n",13);   
        InitIEC101Prot();
        Read_FlashID(flash_id);
#if 0        
        BlockErase(0);
        Read_Flash(flash_id,0,10);
        memset(flash_id,0xaa,10);
        Write_Flash(0,flash_id,10);
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
        break;
      }	
      if(((Flag.Power & F_PwrUp) != 0) && ( PowerCheck() == 0 ))
      {
       // Flag.BatState=1;
        //PwrDnInit();
      }	
    
      
      if(( Flag.Clk & F_ThirdSec )) 	
      {		
        Flag.Clk &= ~F_ThirdSec;								
      }
      if(Flag.Clk& F_HalfSec)
      {
        ProcHalfSec();
      }
      if (Flag.Clk & F_Sec) ProcSec();		
      if (Flag.Clk & F_Min) ProcMin();
      if (Flag.Clk & F_Hour) ProcHour();
      if (Flag.Clk & F_Day) ProcDay();
      
      if(((Flag.Power & F_PwrUp) != 0))	
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

