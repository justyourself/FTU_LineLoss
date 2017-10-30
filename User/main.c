#include <math.h>
#include <stdio.h>
#include <string.h>
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
#include "serial.h"


short Save_Data(unsigned char *Time_buf)
{
  unsigned char Record_buf[16];
  int i,rec_ptr;
  unsigned char tmp_buf[64];  //秒、分、时、日、月、年、记录 
  unsigned short year;
  
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
    memcpy(tmp_buf+6,&Energy_Data[0],32);
    E2P_PWData(i*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,tmp_buf,38);
  }
  rec_ptr++;
  rec_ptr %=LOAD_RECORD_NUM;
  Record_buf[0]=rec_ptr;
  memcpy(Record_buf+1,tmp_buf,6);
  E2P_WData(LoadRecord_Time,Record_buf,8); 
  return 0;
}

short Read_LastData(int ch,unsigned char *buf)
{
  unsigned char Record_buf[8];
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
  E2P_PRData(buf,ch*LOAD_RECORD_SIZE+rec_ptr*ONE_RECORD_SIZE,38);
  return 38;
}



void ProcHalfSec(void)
{
  Flag.Clk &= ~F_HalfSec;
  HT_GPIO_BitsToggle(HT_GPIOC,GPIO_Pin_7);
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
    }
#if 0    
    if(SM.TestDisCnt==0)
    {
        SM.TestDisCnt=2;
        sprintf(Buff,"%05d\r\n",SM.ClockBatAD);
        Serial_Write(2,Buff,7);
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
    Flag.Clk &= ~F_Min;
    GetTime();
    MoveCurrentTimeBCD_Hex();
    if((Clk.MinH%15)==0)
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
}	

/***************************************************
	Working for Every Minute
	1. Decreasing Minute Timer
***************************************************/
void ProcHour(void)
{

    Flag.Clk &= ~F_Hour;
}

void ProcDay(void)
{

    Flag.Clk &= ~F_Day;
}	

void main(void)
{
  unsigned short i;
  unsigned char flash_id[16];
  HT_FreeDog();
  i = 30000;
  while(i--);
  fnTarget_Init();
 // Flag.BatState=0;
  while(1)
  {	
    HT_FreeDog();
    for(i=0;i<4400;i++)
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
       // Read_FlashID(flash_id);
      //  BlockErase(0);
      //  Read_Flash(flash_id,0,10);
      //  memset(flash_id,0xaa,10);
       // E2P_WData(0,flash_id,10);
       // memset(flash_id,0,10);
     //   E2P_RData(flash_id,0,10);
     //   memset(flash_id,0x55,16);
     //   E2P_PWData(0,flash_id,16);
     //   memset(flash_id,0,10);
     //   E2P_PRData(flash_id,0,16);
        
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

