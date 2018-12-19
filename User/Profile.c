#include <math.h>
#include <stdio.h>
#include <string.h>
#include "ht6xxx_reset.h"
#include "TypeMeter.h"
#include "TypeRAM.h"
#include "TypeE2p.h"
#include "ht6xxx_lib.h"					
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

#define _R_PROFILE_RECORD   E2P_RAdj
#define _W_PROFILE_RECORD   E2P_WAdj

void GetCurvePara( CURVEPARA* CurvePara )
{
  unsigned char Buff[4];
  unsigned char* Ptr;
  
  Ptr = Buff;
  CurvePara->StartPageNo = ProfileTab[CurvePara->ChannelNo].StartAddr;		//负荷曲线存储空间起始页号
  CurvePara->Pages = ProfileTab[CurvePara->ChannelNo].EndAddr;				//负荷曲线存储空间总页数			
  CurvePara->RecdLenth = ProfileTab[CurvePara->ChannelNo].RecdLenAddr; //负荷曲线每条记录长度
  CurvePara->RecsInPage = 4096 / CurvePara->RecdLenth;							//负荷曲线每页记录数
  CurvePara->MaxCunt = CurvePara->Pages * CurvePara->RecsInPage;					//负荷曲线存储空间内最大记录数
  CurvePara->RecdCunt = ( CurvePara->Pages - 1 ) * CurvePara->RecsInPage;			//负荷曲线存储空间内最大有效记录数	//1页作为轮转  
  _R_PROFILE_RECORD( (unsigned char*)&CurvePara->AvailRecs, ProfileTab[CurvePara->ChannelNo].EntriesUseAddr, 2 );	//已存记录数
  if( CurvePara->AvailRecs > CurvePara->RecdCunt ) CurvePara->AvailRecs = 0;										//已存记录数超过允许记录数，非法，已存记录数清零。表示没有有效记录
  _R_PROFILE_RECORD( (unsigned char*)&CurvePara->CurRecNo,ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, 2 );
}	

//CurvePara:负荷曲线参数
//type: -1:新产生一条记录，其他：指定记录号
//返回值：指定曲线的指定记录号在FLASH芯片中的物理地址。 -1：该记录不存在
 long LoadPtrChk( CURVEPARA* CurvePara, long Type )
{
  unsigned char* ptr;
  unsigned char buff[4];
  unsigned long Temp,Temp1;
  unsigned long CurRecNo;			//当前记录号
  ptr=buff;
  HT_FreeDog();
  if( Type == -1 )
  {
    CurvePara->CurRecNo += 1;													//新分配一条记录
    if( CurvePara->CurRecNo >= CurvePara->MaxCunt ) CurvePara->CurRecNo = 0;	//超过总记录数，指向第一条。
    if( CurvePara->AvailRecs == 0 ) CurvePara->CurRecNo = 0;			//该通道第一次产生记录，指向第一条。
																					//可配合用于当目标记录指向页的逻辑零地址时，将当前页先擦除后再写入数据，与写FLASH程序配合。		
    _W_PROFILE_RECORD(ProfileTab[CurvePara->ChannelNo].ECurRecNoAddr, (unsigned char*)&CurvePara->CurRecNo, 2 );//保存当前记录号		
    if( CurvePara->AvailRecs < CurvePara->RecdCunt )							//已存记录数小于抄收限制记录数
    { 												
      CurvePara->AvailRecs++;													//已存记录数加1后存入EEPROM	
      _W_PROFILE_RECORD( ProfileTab[CurvePara->ChannelNo].EntriesUseAddr, (unsigned char*)&CurvePara->AvailRecs, 2 );	
      _R_PROFILE_RECORD( ptr,ProfileTab[CurvePara->ChannelNo].EntriesUseAddr,  2 );
    }
    CurRecNo = CurvePara->CurRecNo;			//当前记录号		
  }
  else
  {
    if( Type >= CurvePara->AvailRecs) return -1;									//所抄记录超限，返回空。	
    if( CurvePara->CurRecNo >= Type ) 
      CurRecNo = CurvePara->CurRecNo - Type;		//抄收记录号小于当前记录号，直接从当前记录向前递推找到目标记录
    else 
      CurRecNo = CurvePara->CurRecNo + CurvePara->MaxCunt - Type;
  }
  
  Temp1 = (CurRecNo % CurvePara->RecsInPage)*CurvePara->RecdLenth;	//目标记录首地址在其所处页内的逻辑地址
  Temp = CurvePara->StartPageNo;					//负荷记录存储区在FLASH芯片内的起始页号	
  Temp += CurRecNo/CurvePara->RecsInPage;				//目标记录在FLASH芯片内的页号
  Temp = Temp * 4096 + Temp1;						//目标记录首地址在FLASH芯片内物理地址
  return Temp;								//返回当前或指向的记录号
}
void LoadRecord( unsigned short E2_Addr,unsigned char *Buff)
{
  unsigned short ChannelNo;
  unsigned char* Point;
  unsigned long LongBuff[9];
  CURVEPARA* CurvePara;
  unsigned long Value;
  Point = Buff;
  
  for(ChannelNo=0;ChannelNo<15;++ChannelNo)
  {
      if(E2_Addr == ProfileTab[ChannelNo].PtAddr)
        break;
  }
  if(ChannelNo>14)
    return;
  CurvePara = (CURVEPARA*)LongBuff;
  HT_FreeDog();
  CurvePara->ChannelNo = ChannelNo;
  GetCurvePara( CurvePara );
  if( CurvePara->Pages == 0 ) return;		//该曲线分配的空间为空
  Value = LoadPtrChk( CurvePara, -1 );
  DataFlash_Write( Value, Point, CurvePara->RecdLenth);
  return;
}

int ReadRecord( unsigned short E2_Addr,unsigned char *Buff,long LoadRecNo)
{
  unsigned long LongBuff[9];
  unsigned short ChannelNo;
  CURVEPARA* CurvePara;
  long LoadAddr;
  if( LoadRecNo < 0 ) return 1;					//非法
  for(ChannelNo=0;ChannelNo<15;++ChannelNo)
  {
      if(E2_Addr == ProfileTab[ChannelNo].PtAddr)
        break;
  }
  if(ChannelNo>14)
    return 1;
  CurvePara = (CURVEPARA*)LongBuff;
  HT_FreeDog();
  CurvePara->ChannelNo = ChannelNo;
  GetCurvePara( CurvePara );
  if(CurvePara->Pages == 0) 
    return 1;		//该曲线分配的空间为空
  LoadAddr = LoadPtrChk( CurvePara, LoadRecNo );
  if( LoadAddr != -1 )
  {
    Read_Flash( Buff, LoadAddr,CurvePara->RecdLenth);
    return 0;
  }
  return 2;
}

int Get_Record_Num(unsigned short E2_Addr)
{
  unsigned long LongBuff[9];
  unsigned short ChannelNo;
  CURVEPARA* CurvePara;	
  for(ChannelNo=0;ChannelNo<15;++ChannelNo)
  {
      if(E2_Addr == ProfileTab[ChannelNo].PtAddr)
        break;
  }
  if(ChannelNo>14)
    return 0;
  CurvePara = (CURVEPARA*)LongBuff;
  HT_FreeDog();
  CurvePara->ChannelNo = ChannelNo;
  GetCurvePara( CurvePara );
  return CurvePara->AvailRecs;
}

void Clear_Record(unsigned short E2_Addr)
{
  unsigned char LongBuff[9];
  unsigned short ChannelNo;
  for(ChannelNo=0;ChannelNo<15;++ChannelNo)
  {
      if(E2_Addr == ProfileTab[ChannelNo].PtAddr)
        break;
  }
  if(ChannelNo>14)
    return;
  memset(LongBuff,0,8);
  _W_PROFILE_RECORD( ProfileTab[ChannelNo].EntriesUseAddr, LongBuff, 2 );
  _W_PROFILE_RECORD( ProfileTab[ChannelNo].ECurRecNoAddr, LongBuff, 2 );
}

void Clear_All_Record(void)
{
  int i;
  for(i=0;i<15;++i)
  {
    Clear_Record(ProfileTab[i].PtAddr);
  }
}