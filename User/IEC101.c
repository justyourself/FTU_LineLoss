
#include "iec101.h"
#include "TypeRAM.h"
#include "data.h"
#define justinit 1
#define initend 2
#define notinit 0

#define yxbw   1
#define substinit   2
#define readcmd   3
#define substchange 4
#define nofirstdata 5 

#define FIXDMSG  "LINELOSS/FIXD/fixd01.msg"
#define FIXDXML  "LINELOSS/FIXD/fixd01.xml"
#define FRZDMSG  "LINELOSS/FRZD/frzd01.msg"
#define FRZDXML  "LINELOSS/FRZD/frzd01.xml"
#define SHARPMSG "LINELOSS/SHARPD/sharpd01.msg"
#define SHARPXML "LINELOSS/SHARPD/sharpd01.xml"
#define MONMSG   "LINELOSS/MONTHD/monthd01.msg"
#define MONXML   "LINELOSS/MONTHD/monthd01.xml"
#define EVTMSG   "LINELOSS/EVENTD/eventd01.msg"
#define EVTXML   "LINELOSS/EVENTD/eventd01.xml"


typedef struct {
  u16 info_addr;
  u16 data_len;
  unsigned char *info_data;
}S_SNINFO;
unsigned char s_Devtype[]="FTU";
unsigned char s_Operation[]="N/A";
unsigned char s_Manufacture[]="炬华";
unsigned char s_Hardwarever[]="A";
unsigned char s_Firmwarever[]="00.01";
unsigned char s_FirmwareCrc[]="0x1234";
unsigned char s_Protocolver[]="V1.000";
unsigned char s_Model[]="JH4000";
unsigned char s_Id[]="201710280001";
S_SNINFO s_sninfo[]=
{
  0x8001,3,s_Devtype,
  0x8002,3,s_Operation,
  0x8003,4,s_Manufacture,
  0x8004,1,s_Hardwarever,
  0x8005,5,s_Firmwarever,
  0x8006,6,s_FirmwareCrc,
  0x8007,6,s_Protocolver,
  0x8008,6,s_Model,
  0x8009,12,s_Id,
};


unsigned char *File_List[]={FIXDMSG,FIXDXML,FRZDMSG,FRZDXML,SHARPMSG,SHARPXML,MONMSG,MONXML,EVTMSG,EVTXML};

static struct IEC101_STRUCT m_IEC101;
struct IEC101_STRUCT *lpIEC101=&m_IEC101;	//IEC101规约私用数据指针

unsigned long GetYc(unsigned short ycno)
{
  unsigned long *Ptr;
  Ptr=(unsigned long*)&Real_Data[ycno/31];
  return Ptr[ycno%31];
}
short GetDd(unsigned short kwhno,unsigned char *buf)
{
  int m_ch,ptr_v;
  unsigned long *Ptr;
  unsigned long m_Lval;
  unsigned char tmp_buf[48];
  float f_val;
  m_ch=kwhno/16;
  ptr_v=kwhno%16;
  if(ptr_v<8)
  {
   // Ptr=(unsigned long*)&Energy_Data[m_ch];
    //memcpy(buf,&Ptr[ptr_v],4);
    Ptr=(unsigned long*)&Energy_Data[m_ch];
    f_val = Ptr[ptr_v]; 
    f_val = f_val/1000;
    memcpy(buf,(unsigned char *)&f_val,4);
    //*(buf) = tmp_buf[0];//(u8)nVal;	// 遥测值
//		*(lpby + byMsgNum ++) = tmp_buf[1];//(u8)(nVal >> 8);
  //                      *(lpby + byMsgNum ++) = tmp_buf[2];//(u8)(nVal >> 16);
    //                    *(lpby + byMsgNum ++) = tmp_buf[3];//(u8)(nVal >> 24);
    return 4;
  }
  else
  {
    Read_LastData(m_ch,tmp_buf);
    memset(buf,0,12);
   // memcpy(buf,tmp_buf+6+4*(ptr_v-8),4);
    //Ptr = (unsigned long*)tmp_buf[6+4*(ptr_v-8)];
    //f_val = Ptr[0];
    memcpy(&m_Lval,tmp_buf+6+4*(ptr_v-8),4);
    f_val = m_Lval;
    f_val = f_val/1000;
    memcpy(buf,(unsigned char *)&f_val,4);
    memcpy(buf+6,tmp_buf,6);
    return 12;
  }
}
//功能：接收错误处理
//输入：void
//输出：void
void IEC101RErrorProcess(void)
{
	lpIEC101->wRecvNum = 0;
	lpIEC101->RErrorN++;	   	//规约接收错误字数
	if(lpIEC101->RErrorN>=MaxErrorN)
	{
		lpIEC101->RErrorN=0;	   //规约接收错误字数
		if((lpIEC101->initstatus == initend) &&(lpIEC101->FtuSccReset == FALSE))
	   	{
	    		//fnInitSCC(ChanlNo,SCCERROR); //复位通道
	   		lpIEC101->FtuSccReset = TRUE; //子站状态改变
		}
	}
}
//链路层接收函数
//功能：链路接收，把串口接收缓冲区中的数据解出一桢
//      并放入lpIEC101->PRecvFrame.byLinkBuf
//      取出链路层地址
//输入：void
//输出：void

void Iec101LinkRecv(void)
{
  u8 Count,byRecv,bySum = 0;
  int i,j;
  Count=Serial_Read(2,lpIEC101->byRecvBuf+lpIEC101->wRecvNum,128);
  if(Count)
  {
    lpIEC101->wRecvNum +=Count;
    for(i=0;i<lpIEC101->wRecvNum;++i)
    {  
      if(lpIEC101->byRecvBuf[i]!=F_STARTCODE && lpIEC101->byRecvBuf[i]!=V_STARTCODE)
      {
        continue;
      }
      if(lpIEC101->byRecvBuf[i]==F_STARTCODE)
      {
        if(lpIEC101->wRecvNum-i>=5 && (lpIEC101->byRecvBuf[lpIEC101->wRecvNum-i-1]==0x16))
        {  
          memcpy(lpIEC101->PRecvFrame.byLinkBuf,lpIEC101->byRecvBuf+i,lpIEC101->wRecvNum-i);
          lpIEC101->wRecvCmmAdd.Byte.l = lpIEC101->PRecvFrame.byLinkBuf[2];
          if(lpIEC101->TypeLinkAdd==2)
          {
            lpIEC101->wRecvCmmAdd.Byte.h = lpIEC101->PRecvFrame.byLinkBuf[3];
          }
          lpIEC101->wRecvNum=0;
          lpIEC101->PRecvFrame.byFull=1;
        }
        break;
      }
      if(lpIEC101->byRecvBuf[i]==V_STARTCODE)
      {
        if((lpIEC101->byRecvBuf[i]==lpIEC101->byRecvBuf[i+3])
					&& (lpIEC101->byRecvBuf[i+1]==lpIEC101->byRecvBuf[i+2]))
        {
          lpIEC101->wRecvLen = lpIEC101->byRecvBuf[i+1] + 6;
          if((lpIEC101->wRecvNum-i)>=lpIEC101->wRecvLen)
          {
            //Serial_Write(2,"Long Frame\r\n",12);
            memcpy(lpIEC101->PRecvFrame.byLinkBuf,lpIEC101->byRecvBuf+i,lpIEC101->wRecvNum-i);
            lpIEC101->wRecvCmmAdd.Byte.l = lpIEC101->PRecvFrame.byLinkBuf[5];
            if(lpIEC101->TypeLinkAdd==2)
            {
              lpIEC101->wRecvCmmAdd.Byte.h = lpIEC101->PRecvFrame.byLinkBuf[6];
            }
            lpIEC101->wRecvNum=0;
            lpIEC101->PRecvFrame.byFull=1;
          }
          break;
        }
      }
    }
  }
}
//从动链路层接收处理函数
//功能：根据链路控制字执行相应的操作：
//      1）根据功能码执行动作
//      2）FCB位判断和设置
//输入：链路控制字
//输出：void

void PLinkRecvProcessF(u8 byConField)
{
  lpIEC101->PRecvFrame.byFunCode = byConField & 0x0F;
  if (lpIEC101->initstatus == notinit)	    
  {
    if ( (lpIEC101->PRecvFrame.byFunCode == CALL_LINK)
              ||(lpIEC101->PRecvFrame.byFunCode == RESET_LINK) )
    {
      if (lpIEC101->PRecvFrame.byFunCode == CALL_LINK)
      {
        lpIEC101->PSeAppLayer.LinkFunCode = 0x0B;
        if (lpIEC101->haveset == TRUE)
        {
          lpIEC101->initstatus = justinit;
        }
      }		
      else if (lpIEC101->PRecvFrame.byFunCode == RESET_LINK)
      {
        lpIEC101->PfcbC = 0;
        lpIEC101->PSeAppLayer.LinkFunCode = 0x00;
	lpIEC101->haveset = TRUE;
        lpIEC101->initstatus = justinit;
        lpIEC101->PWinTimer=0;
      }
      lpIEC101->PSeAppLayer.byMsgNum = 0;
      lpIEC101->PSeAppLayer.byFull = 1;
    }
    return;
  }
  
  if ( ((byConField & 0x20)==(lpIEC101->PfcbC<<5)) && (byConField & 0x10))
  {
    lpIEC101->PSendFrame.byFull = 1;
    return;
  }
  
  if((byConField & 0x40)==0)
  {
    switch(lpIEC101->PRecvFrame.byFunCode)
    {
    case RESET_LINK:
      if (lpIEC101->initstatus == justinit)
        {
          lpIEC101->PReFrameType = CALL_DATA1;
          lpIEC101->firstData = substinit;		// 子站初始化
          lpIEC101->wCmmAdd.Byte.l = 1;
          lpIEC101->wCmmAdd.Byte.h = 0;
          lpIEC101->byCOI = 2;
          lpIEC101->Pacd = 3;
          lpIEC101->Pdfc = 1;
          lpIEC101->PSendFrame.byFunCode = 3;
	}
       // else
     //   {
      //    lpIEC101->PReFrameType = 0x3;
      //    lpIEC101->PSendFrame.byFunCode = 3;
     //   }
      
      break;
    case LINK_GOOD:
      lpIEC101->PReFrameType = lpIEC101->PRecvFrame.byFunCode;
      lpIEC101->PSeAppLayer.LinkFunCode = 0x00;
      lpIEC101->Pacd = 2;
    //  lpIEC101->PSendFrame.byFull=1;
      break;
    default:
      lpIEC101->PSendFrame.byFull=0;
      break;
    }
    
    return;
  }
 
  if ( (lpIEC101->PRecvFrame.byFunCode == CALL_LINK) 
		||(lpIEC101->PRecvFrame.byFunCode == RESET_LINK)
		||(lpIEC101->PRecvFrame.byFunCode == CALL_ACD) )
  {
    if (lpIEC101->PRecvFrame.byFunCode == RESET_LINK)
    {
      lpIEC101->PfcbC = 0;
      lpIEC101->PSeAppLayer.LinkFunCode = 0x00;
      lpIEC101->initstatus = justinit;
      lpIEC101->PWinTimer=0;
    }
    if (lpIEC101->PRecvFrame.byFunCode == CALL_LINK)
    {
      lpIEC101->PSeAppLayer.LinkFunCode = 0x0B;
      if (lpIEC101->haveset == TRUE)
        {
          lpIEC101->initstatus = justinit;
        }
    }
    if (lpIEC101->PRecvFrame.byFunCode == CALL_ACD)
      lpIEC101->PSeAppLayer.LinkFunCode = 0x0B;
		
    lpIEC101->PSeAppLayer.byMsgNum = 0;
    lpIEC101->PSeAppLayer.byFull = 1;
  }
  
  if ((lpIEC101->PRecvFrame.byFunCode == CALL_DATA1)||(lpIEC101->PRecvFrame.byFunCode == CALL_DATA2))
  {
    lpIEC101->PReFrameType = lpIEC101->PRecvFrame.byFunCode;
    lpIEC101->PfcbC = byConField & 0x20 ? 1 : 0;	// FCV位有效,则保存FCB位
  }       
}
//从动应用层接收处理函数
//功能：根据链路控制字及类型标识作相应的处理
//输入：void
//输出：void

void PLinkRecvProcessV(u8 byConField)
{
  u8 byFrameCount,i; 
#if 0       
  if ( ((byConField & 0x20)==(lpIEC101->PfcbC<<5)) && (byConField & 0x10))
  {
    lpIEC101->PSendFrame.byFull = 1;
    return;
  }
#endif       
  lpIEC101->PReAppLayer.lpByBuf = lpIEC101->PRecvFrame.byLinkBuf;
  lpIEC101->PRecvFrame.byFunCode = byConField & 0x0F;
  if(3==lpIEC101->PRecvFrame.byFunCode)
  {
    if(3==lpIEC101->UnsolTimeInterval)    //先发链路确认再等待召唤二级数据
    {
      lpIEC101->PReFrameType = 3;
      lpIEC101->PSeAppLayer.LinkFunCode = 0x00;
      lpIEC101->PSeAppLayer.byMsgNum = 0;
      lpIEC101->PSeAppLayer.byFull = 1;
    }
  }
  lpIEC101->PfcbC = byConField & 0x20 ? 1 : 0;	// FCV位有效,则保存FCB位
  byFrameCount=5+lpIEC101->TypeLinkAdd; //指向类型标识
  lpIEC101->PReMsgType = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];	// 类型标识
  lpIEC101->byQualify = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];		// 可变结构限定词
  lpIEC101->byReason  = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];		// 传送原因
  if(lpIEC101->TypeSeReason==2)
    lpIEC101->bySourceAdd = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];  //传送原因为两个字节时取出源地址 fu 2005.9.1
  lpIEC101->wCmmAdd.Byte.l = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];		// 应用服务数据单元的公共地址
  if(lpIEC101->TypeCmmAdd==2)
    lpIEC101->wCmmAdd.Byte.h = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];		// 应用服务数据单元的公共地址
  lpIEC101->dwInfAdd.Dword = 0;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    lpIEC101->dwInfAdd.Byte[i] =  lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]; 
  lpIEC101->byPSGenStep = 0;
  lpIEC101->byPSDdStep = 0;
  switch(lpIEC101->PReMsgType)
  {
  case GeneralCall: //总召唤
	lpIEC101->byQOI = lpIEC101->PReAppLayer.lpByBuf[byFrameCount];//[lpIEC101->PRecvFrame.wFrameLen-3]; //指向QOI
        if ( (lpIEC101->byReason == ACT) || (lpIEC101->byReason == DEACT) )
          lpIEC101->byPSGenStep = 0;
        else if (lpIEC101->byReason == REQ)
          lpIEC101->byPSGenStep = lpIEC101->byQOI - 20;
    //  lpIEC101->PReFrameType = CALL_DATA2; //zzl 测试青云子站
  //    lpIEC101->PSeAppLayer.byFull=0;
        lpIEC101->frameno = 0;
        break;
  case C_CS_NA_1:  //校时
	{
                if(lpIEC101->byReason == ACT)
                  SettimeToCan(lpIEC101->PReAppLayer.lpByBuf+byFrameCount); //fulianqiang 2005.9.12,较时函数不能再使用常数来寻址
	}
        break;
  case C_CI_NA_1: //召唤电度
	{
          lpIEC101->byQCC = lpIEC101->PReAppLayer.lpByBuf[byFrameCount];//[lpIEC101->PRecvFrame.wFrameLen-3];
	//lpIEC101->byPSDdStep = (lpIEC101->byQCC & 0x3F);  //取实际接收到的值，原来减去一。
          break;
	}
  case C_RD_NA_1: //读数据
	{
		lpIEC101->dwReadAd.Dword = lpIEC101->dwInfAdd.Dword;
	}
        break;
  case C_RC_NA_1:
  case C_DC_NA_1:
  case C_SC_NA_1: //单、双点遥控	
	{
		lpIEC101->byDCO = lpIEC101->PReAppLayer.lpByBuf[byFrameCount];//[lpIEC101->PRecvFrame.wFrameLen-3];
		lpIEC101->WaitYkXzRet = 0;
		lpIEC101->WaitYkZxRet = 0;
		lpIEC101->SendYkZxAck = 0;
	}
        break;
  case C_TS_NA_1:  //测试
	{
		lpIEC101->wTester.Byte.l =lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
		lpIEC101->wTester.Byte.h =lpIEC101->PReAppLayer.lpByBuf[byFrameCount]; 
	}
        break;
  case C_RR_NA_1:
    break;
  case C_RS_NA_1:
          byFrameCount -= lpIEC101->TypeInfAdd;
          lpIEC101->Sn = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
          lpIEC101->Sn = (lpIEC101->Sn) | (lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]<<8);
          if(lpIEC101->byQualify) 
          {
            for(i=0;i<lpIEC101->byQualify;++i)
            {
              lpIEC101->List_para[i]=lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
              lpIEC101->List_para[i]=(lpIEC101->List_para[i]) | (lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]<<8);
            }
          }
          else //定值区间的所有参数
          {
          }
    break;
  case F_FR_NA_1:
    if(lpIEC101->PReAppLayer.lpByBuf[byFrameCount++]==0x02) //附加数据包类型
    {
      lpIEC101->Fop = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
      if(lpIEC101->Fop!=3)
      {  
        memcpy(&(lpIEC101->FId),lpIEC101->PReAppLayer.lpByBuf+byFrameCount,4);
        byFrameCount += 4;
      }
      memset(lpIEC101->Fname,0,32);
      if(lpIEC101->PReAppLayer.lpByBuf[byFrameCount])
      {
        memcpy(lpIEC101->Fname,lpIEC101->PReAppLayer.lpByBuf+byFrameCount+1,lpIEC101->PReAppLayer.lpByBuf[byFrameCount]);
       // byFrameCount += lpIEC101->PReAppLayer.lpByBuf[byFrameCount];
      }
      byFrameCount += lpIEC101->PReAppLayer.lpByBuf[byFrameCount]+1;
      lpIEC101->byDCO = lpIEC101->PReAppLayer.lpByBuf[byFrameCount++];
    }
    else
    {
      lpIEC101->PReMsgType = 0;
    }
    break;
  default:
    {
      lpIEC101->PReMsgType = 0;
    }
    break;
  }
}

//链路层接收处理函数
//功能：取出链路控制字，调用处理函数
//输入：void
//输出：void

void Iec101LinkRecvPro(void)
{
  u8 byConField;
  if (lpIEC101->PRecvFrame.byFull == 1)
  {
    lpIEC101->Pacd = 0;
    lpIEC101->Pdfc = 0;
    if(lpIEC101->PRecvFrame.byLinkBuf[0] == F_STARTCODE)
    {
      if(lpIEC101->TypeLinkAdd==2)
      {
	       if( (lpIEC101->wRecvCmmAdd.Word == lpIEC101->wLinkAdd.Word)||(lpIEC101->wRecvCmmAdd.Word ==0xFFFF))
	        {   
	           byConField = lpIEC101->PRecvFrame.byLinkBuf[1];
	           PLinkRecvProcessF(byConField);
		   lpIEC101->OrgnizeFrame = 1 ;
		}
		else
		   lpIEC101->OrgnizeFrame = 0 ;
	    }
	    else
	    {
	       if( (lpIEC101->wRecvCmmAdd.Byte.l == lpIEC101->wLinkAdd.Byte.l)||(lpIEC101->wRecvCmmAdd.Byte.l ==0xFF))
 		{
		   byConField = lpIEC101->PRecvFrame.byLinkBuf[1];
 		   PLinkRecvProcessF(byConField);
		   lpIEC101->OrgnizeFrame = 1 ;
		}
		else
 		   lpIEC101->OrgnizeFrame = 0 ;
	    }
	 }
	 else if(lpIEC101->PRecvFrame.byLinkBuf[0] == V_STARTCODE)
	 {
	    if(lpIEC101->TypeLinkAdd==2)
	    {
	       if( (lpIEC101->wRecvCmmAdd.Word == lpIEC101->wLinkAdd.Word)||(lpIEC101->wRecvCmmAdd.Word ==0xFFFF))
	        {   
	           byConField = lpIEC101->PRecvFrame.byLinkBuf[4];
	           PLinkRecvProcessV(byConField);
		   lpIEC101->OrgnizeFrame = 1;
		}
		else
		   lpIEC101->OrgnizeFrame = 0 ;
	    }
	    else
	    {
	       if( (lpIEC101->wRecvCmmAdd.Byte.l == lpIEC101->wLinkAdd.Byte.l)||(lpIEC101->wRecvCmmAdd.Byte.l ==0xFF))
 		{
		   byConField = lpIEC101->PRecvFrame.byLinkBuf[4];
 		   PLinkRecvProcessV(byConField);
		   lpIEC101->OrgnizeFrame =1;
		}
		else
		   lpIEC101->OrgnizeFrame = 0;
	    }
	}
	lpIEC101->PRecvFrame.byFull=0;
	if((lpIEC101->FtuSccReset ==TRUE)&&(lpIEC101->OrgnizeFrame == 1))
	{
		lpIEC101->FtuSccReset = FALSE;	  //通道恢复良好
	}
    }   
}
//向CAN网校时
void SettimeToCan(u8* pbyBuf)
{ 
#if 0
        unsigned short wTotalMs;
	wTotalMs = (unsigned short)pbyBuf[1];
	wTotalMs = pbyBuf[0]|(wTotalMs<<8);
	sClock.wMS = wTotalMs % 1000;
	sClock.bySecond = (u8)(wTotalMs / 1000);
	//校时帧的分钟字节最高位时间有效标志未判
	sClock.byMinute = pbyBuf[2] & 0x3F;
	sClock.byHour = pbyBuf[3] & 0x1F;
	sClock.byDay = pbyBuf[4] & 0x1F; 
	lpIEC101->byDayofWeek  = (pbyBuf[4] & 0xE0) >> 5;
	sClock.byMonth = pbyBuf[5] & 0x0F;
	sClock.wYear = (pbyBuf[6] & 0x7F)+2000;     
#else
        unsigned char pRTCTime[8];
        unsigned short wTotalMs;
	wTotalMs = (unsigned short)pbyBuf[1];
	wTotalMs = pbyBuf[0]|(wTotalMs<<8);
        pRTCTime[0] = (pbyBuf[4] & 0xE0) >> 5; //week
        pRTCTime[1] = (pbyBuf[6] & 0x7F); //year
        pRTCTime[2] = pbyBuf[5] & 0x0F;//month
        pRTCTime[3] = pbyBuf[4] & 0x1F; //day
        pRTCTime[4] = pbyBuf[3] & 0x1F;
        pRTCTime[5] = pbyBuf[2] & 0x3F;
        pRTCTime[6] = (u8)(wTotalMs / 1000);
        HT_RTC_Write(pRTCTime);
#endif        
}
//从动应用层接收处理函数
//功能：根据链路控制字及类型标识作相应的处理
//输入：void
//输出：void

void SleaveIec101Frame(void)
{
	//链路层接收函数
	Iec101LinkRecv();
	//链路层接收处理函数
	Iec101LinkRecvPro();
}
//监视从动窗口
void WatchPWindow(void)
{ 
	if (lpIEC101->PReMsgType || lpIEC101->PReFrameType
		|| lpIEC101->PSeAppLayer.byFull || lpIEC101->PSendFrame.byFull)
	{
		lpIEC101->PWindow = 1;
	}
        if(((lpIEC101->PRecvFrame.byFunCode == RESET_LINK) && lpIEC101->FlagPingH && (lpIEC101->initstatus == justinit)) || lpIEC101->wTester.Byte.l)
        {
                lpIEC101->PWindow = 1;
        }
}
//搜索一级数据
void SearchFirstData(void)
{     
    	short wYxChang,wSendNo;
    	if(lpIEC101->FlagPingH)	  //如果是平衡方式
    	{
          if((lpIEC101->PRecvFrame.byFunCode == RESET_LINK) && (lpIEC101->PWinTimer>200) && (lpIEC101->initstatus == justinit))
          {
            lpIEC101->PWinTimer = 0;
            lpIEC101->PSeAppLayer.byFull = 1;
            lpIEC101->PSeAppLayer.LinkFunCode = CALL_LINK;
            lpIEC101->Pacd = 2;
          }
          if(lpIEC101->PRecvFrame.byFunCode == LINK_GOOD)
          {
            lpIEC101->PSeAppLayer.byFull = 1;
            lpIEC101->PSeAppLayer.LinkFunCode = RESET_LINK;
            lpIEC101->PRecvFrame.byFunCode = 0;
            lpIEC101->Pacd = 2;
          }
        /*  if(lpIEC101->wTester.Byte.l && lpIEC101->PWinTimer>200)
          {
            lpIEC101->PWinTimer = 0;
            lpIEC101->PSeAppLayer.LinkFunCode=TRAN_CONFIRM_DATA;
            lpIEC101->PSendFrame.byFunCode = TRAN_CONFIRM_DATA;
            lpIEC101->PReMsgType = C_TS_NA_1; 
          }*/
          return;
        }
	if (lpIEC101->firstData != nofirstdata)	// 已有1级用户数据,暂停搜索
   		return;
	if (lpIEC101->initstatus == justinit)
	{
		lpIEC101->firstData = substinit;		// 子站初始化
	}
#if 0       
	else if((lpIEC101->YxChangeC != sHostSoe.bySoeWrite)&&(initend==lpIEC101->initstatus))
	{
		wYxChang=lpIEC101->YxChangeC ;
		while (wYxChang != sHostSoe.bySoeWrite)
		{
			//本机SOE对应的遥信变位
			wSendNo = sHostSoe.SoeQueue[wYxChang].byYXSendSN;
			//增加判断目的是按照发送表发遥信变位fulianqiang 2006.1.10
			if((wSendNo <= lpIEC101->SendYxN)&&(pYXSendTab[wSendNo].byUnitType==FTU))    //判断发送顺序表中是否有此遥信,避免空遥信变位
			{
				lpIEC101->firstData = yxbw;		// 遥信变位
				lpIEC101->YxChangeC= wYxChang;
				break;
			}
			if((++wYxChang) >= MAX_SOE_NUM )   //移动soe读指针
			      	wYxChang = 0;
		 }
	}
#endif        
    	if(nofirstdata==lpIEC101->firstData)
	{
		if (lpIEC101->dwReadAd.Dword)
	    	{
	   		lpIEC101->firstData = readcmd;		// 由读数命令所寻址的信息体的数据
	   	}
	}
	if (lpIEC101->firstData != nofirstdata)	// 搜索到1级用户数据
	{
		lpIEC101->Pacd = 1;			// ACD置位
	}
	else
		lpIEC101->Pacd=0;        
}
//组遥信变位信息体
u8 OrgnizeYxbwMsg(u8 *lpby)
{
#if 0 
        WORD wSendNo;
	FOUR_BYTE_TO_DWORD  dwInfoAd;
	u8 i,byMsgNum = 0;
	u8 * lpInfoNum;		   	// 信息体数目地址 没必要加个指针fulianqiang 2005.9.2
	*(lpby + byMsgNum ++) = 1;   //类型标识	
	lpInfoNum = lpby + byMsgNum ++;	   //结构限定词lpby+1对应的地址
	*lpInfoNum = 0;
	*(lpby + byMsgNum ++) = 3;  //传送原因
	if(lpIEC101->TypeSeReason==2)		 //是否两个字节的传送原因
	   *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;		
       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
	//针对SOE队列的遥信变位读计数
	while (lpIEC101->YxChangeC != sHostSoe.bySoeWrite)
	{
		//本机SOE对应的遥信变位
		wSendNo = sHostSoe.SoeQueue[lpIEC101->YxChangeC].byYXSendSN;
		//增加判断目的是按照发送表发遥信变位fulianqiang 2006.1.10
		if(wSendNo <= lpIEC101->SendYxN)		//判断发送顺序表中是否有此遥信
		{
                	dwInfoAd.Dword = wSendNo + IEC101_YXSA;
			dwInfoAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
	        	for(i=0;i<lpIEC101->TypeInfAdd;i++)		//信息体地址
	           		*(lpby + byMsgNum ++) = dwInfoAd.Byte[i];
			if (sHostSoe.SoeQueue[lpIEC101->YxChangeC].byOnOff == 0x10)
				*(lpby + byMsgNum ++) = 0;	
			else
				*(lpby + byMsgNum ++) = 1;
			if( ++(*lpInfoNum) >= IEC101_YXBWNPF)
				break;
		}
		if((++lpIEC101->YxChangeC) >= MAX_SOE_NUM )   //移动soe读指针
		      	lpIEC101->YxChangeC = 0;

	}
	return byMsgNum;
#else
        return 0;
#endif        
}
//组初始化结束信息体
u8 OrgnizeInitEndMsg(u8* pbyMsg)
{
 
	u8 i,byMsgNum = 0;         
	*(pbyMsg + byMsgNum++) = M_EI_NA_1;   //类型标识
	*(pbyMsg + byMsgNum++) = 1;
	*(pbyMsg + byMsgNum++) = INIT;
	if(lpIEC101->TypeSeReason==2)		 //是否两个字节的传送原因
	   *(pbyMsg + byMsgNum ++) = lpIEC101->bySourceAdd;		
       	*(pbyMsg + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
           *(pbyMsg + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
        for(i=0;i<lpIEC101->TypeInfAdd;i++)		//信息体地址 fulianqiang 2005.9.2
              *(pbyMsg + byMsgNum ++) = 0; 
	*(pbyMsg + byMsgNum++) = lpIEC101->byCOI;        
	return byMsgNum;
}
//组读数据信息体
u8 OrgnizeReadDataMsg(u8* lpby)
{
	int wYxTNo;
	u8 byYxVal,i;
	int wYcTNo;
	int  iYcVal;
	int wDdTNo;
	int dwDdVal;
	u8 byMsgNum = 1;
#if 1       
	*(lpby + byMsgNum ++) = 1;	// 结构限定词
	*(lpby + byMsgNum ++) = SPONT;	//突发
	if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;      //fulianqiang 2005.9.2
       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
        for(i=0;i<lpIEC101->TypeInfAdd;i++)
	    *(lpby + byMsgNum ++) = lpIEC101->dwReadAd.Byte[i];
	if (lpIEC101->TypeProtocol)	//2002版101规约
	{
#if 0          
		if ( (lpIEC101->dwReadAd.Word[0] >= IEC101_YXSA) && (lpIEC101->dwReadAd.Word[0] <= IEC101_YXEA_2002) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 1;	 //类型标识
			wYxTNo = lpIEC101->dwReadAd.Word[0] - 1;
			byYxVal = GetYx(wYxTNo,YX_BIT);
			if (byYxVal)
				*(lpby + byMsgNum ++) = 1;
			else
				*(lpby + byMsgNum ++) = 0;
		}
		else
#endif                  
                if ( (lpIEC101->dwReadAd.Word[0] >= IEC101_YCSA_2002) && (lpIEC101->dwReadAd.Word[0] <= IEC101_YCEA_2002) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 21;
			wYcTNo = lpIEC101->dwReadAd.Word[0] - IEC101_YCSA_2002;
			iYcVal = GetYc(wYcTNo);
			*(lpby + byMsgNum ++) = (u8)iYcVal;
			*(lpby + byMsgNum ++) = (u8)(iYcVal >> 8);
		}
		else if( (lpIEC101->dwReadAd.Word[0] >= IEC101_DDSA_2002) && (lpIEC101->dwReadAd.Word[0] <= IEC101_DDEA_2002) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 15;
			wDdTNo = lpIEC101->dwReadAd.Word[0] - IEC101_DDSA_2002;
			//dwDdVal = GetDd(wDdTNo);
                        dwDdVal = 0x12345678;
			*(lpby + byMsgNum ++) = (u8)dwDdVal;
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 8);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 16);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 24);
			*(lpby + byMsgNum ++) = 0;
		}
		else
		{
			byMsgNum = 0;
		}
	}
	else
	{
#if 0          
		if ( (lpIEC101->dwReadAd.Word[0] >= IEC101_YXSA) && (lpIEC101->dwReadAd.Word[0] <= IEC101_YXEA) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 1;
			wYxTNo = lpIEC101->dwReadAd.Word[0] - 1;
			byYxVal = GetYx(wYxTNo,YX_BIT);
			if (byYxVal)
				*(lpby + byMsgNum ++) = 1;
			else
				*(lpby + byMsgNum ++) = 0;
		}
		else
#endif                 
                if ( (lpIEC101->dwReadAd.Word[0] >= IEC101_YCSA) && (lpIEC101->dwReadAd.Word[0] <= IEC101_YCEA) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 21;
			wYcTNo = lpIEC101->dwReadAd.Word[0] - IEC101_YCSA;
			iYcVal = GetYc(wYcTNo);
			*(lpby + byMsgNum ++) = (u8)iYcVal;
			*(lpby + byMsgNum ++) = (u8)(iYcVal >> 8);
		}
		else if( (lpIEC101->dwReadAd.Word[0] >= IEC101_DDSA) && (lpIEC101->dwReadAd.Word[0] <= IEC101_DDEA) )//fulianqiang 2005.9.2
		{
			*(lpby + 0) = 15;
			wDdTNo = lpIEC101->dwReadAd.Word[0] - IEC101_DDSA;	   //fulianqiang 2005.9.2
			dwDdVal = 0x12345678;//GetDd(wDdTNo);
			*(lpby + byMsgNum ++) = (u8)dwDdVal;
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 8);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 16);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 24);
			*(lpby + byMsgNum ++) = 0;
		}
		else
		{
			byMsgNum = 0;
		}
	}
#endif        
	return byMsgNum;
}
//组子站状态变化信息体
u8 OrgnizeSubChgMsg(void)
{
	return 0;
}

//上送一级数据
void SendData1(void)
{
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 byMsgNum = 0;
        
	switch (lpIEC101->firstData)
	{
		case yxbw:
			byMsgNum = OrgnizeYxbwMsg(lpby);
		   	lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
			lpIEC101->PSeAppLayer.LinkFunCode = 8;
			break;
		case substinit:
			byMsgNum = OrgnizeInitEndMsg(lpby);
			lpIEC101->initstatus = initend;
			lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
			lpIEC101->PSeAppLayer.LinkFunCode = 3;
			break;
	       	case readcmd:
	     		byMsgNum = OrgnizeReadDataMsg(lpby);
	       		lpIEC101->dwReadAd.Dword = 0;	     
	       		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
	      		lpIEC101->PSeAppLayer.LinkFunCode = 8;
                        break;
		case substchange:
			byMsgNum = OrgnizeSubChgMsg();
			lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
			lpIEC101->PSeAppLayer.LinkFunCode = 8;
			break;
		case nofirstdata:
			byMsgNum = 0;
			lpIEC101->PSeAppLayer.byMsgNum = 0;
			lpIEC101->PSeAppLayer.LinkFunCode = 9;
			break;
	}
  	lpIEC101->Pacd = 0;
 	lpIEC101->firstData = nofirstdata;        
}
//组SOE信息体
u8 OrgnizeSoeMsg(void)
{
	int wSendNo;
	int  dwInfoAd;
	int wMs;
//	u8 *lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 i,byMsgNum = 0,bySendSoeNum = 0;
	u8 * lpInfoNum;
#if 0        
	if( lpIEC101->PC56Time2a )
		*(lpby + byMsgNum ++) = 30;
	else
		*(lpby + byMsgNum ++) = 2;	 //类型标识	
	lpInfoNum = lpby + byMsgNum ++;
	*(lpby + byMsgNum ++) = 3;				
	if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;

       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
	
	//从SOE实时区取SOE事项组帧（组信息体）
	while (lpChanl[ChanlNo].ReadSoeC!=sHostSoe.bySoeWrite)
	{
		wSendNo = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].byYXSendSN;
		if(wSendNo <= lpIEC101->SendYxN)
		{
			dwInfoAd.Dword = IEC101_YXSA + wSendNo ;
			dwInfoAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
	        	for(i=0;i<lpIEC101->TypeInfAdd;i++)
	           		*(lpby + byMsgNum ++) = dwInfoAd.Byte[i];
			if (sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].byOnOff == 0x10)
				*(lpby + byMsgNum ++) = 0;	
			else
				*(lpby + byMsgNum ++) = 1;	
			wMs =(sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.bySecond)*1000
				+sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.wMS;
			*(lpby + byMsgNum ++) = (u8)wMs;
			*(lpby + byMsgNum ++) = (u8)(wMs >> 8);
			*(lpby + byMsgNum ++) = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.byMinute;
			if(lpIEC101->PC56Time2a)
			{
		        	*(lpby + byMsgNum ++) = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.byHour;
		 		*(lpby + byMsgNum ++) = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.byDay;
		 		*(lpby + byMsgNum ++) = sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.byMonth;
		 		*(lpby + byMsgNum ++) = ( sHostSoe.SoeQueue[lpChanl[ChanlNo].ReadSoeC].Clock.wYear)-2000;
				if (++bySendSoeNum >= IEC101_SOENPF_TIME56)
					break;

		 	}
		 	else
		 	{
				if (++bySendSoeNum >= IEC101_SOENPF_TIME24)
					break;
		 	}
		}
		//移动soe读指针  
		if (++lpChanl[ChanlNo].ReadSoeC >= MAX_SOE_NUM)
			lpChanl[ChanlNo].ReadSoeC = 0;
	}
	if (bySendSoeNum)		
		*lpInfoNum = bySendSoeNum;	
	else
		byMsgNum = 0;			// 有SOE但发送序号为不发送
#endif        
	return byMsgNum;
}
//组遥测越限值信息体
u8 OrgnizeYcOverMsg(void)
{
	int i;
	int  dwYcAdd;
	int  nTempYc;
	int  iThreshold;	//门限值
	int  iDifference;	//差值
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8  j,byMsgNum = 0,bySendYcOverNum = 0;
	u8* lpInfoNum;
#if 0        
	*(lpby + byMsgNum ++) = 21;			// 类型标识21
	lpInfoNum = lpby + byMsgNum ++;		//信息体数目
	*(lpby + byMsgNum ++) = SPONT;				// 传送原因<3>:突发
	if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
	if(lpIEC101->TypeProtocol)	 //判断是否2002版的101规约
 	     dwYcAdd.Dword =  IEC101_YCSA_2002;
 	else 
 	     dwYcAdd.Dword =  IEC101_YCSA;
	dwYcAdd.Word[1] = lpIEC101->dwInfAdd.Word[1];
	for (i=lpIEC101->nStartYcOver;i < lpIEC101->SendYcN;i++)
	{
		nTempYc = GetYc(i);
		iDifference=nTempYc-lpIEC101->nLastYcVal[i];	//取两次遥测的差值
		if (iDifference<0)	//差值为负数
			iDifference=-iDifference;	//将负整数变为正整数
		iThreshold=(INT)((LONG)lpIEC101->nLastYcVal[i]*(LONG)lpIEC101->byRange/1000);	//计算门限值				
		if (iThreshold<0)	//门限值为负数
			iThreshold=-iThreshold;	//将负整数变为正整数
		if (iDifference>iThreshold)	//差的绝对值超过设定的门限值
		{
			lpIEC101->nLastYcVal[i] = nTempYc;
			dwYcAdd.Dword += i;
	        	for(j=0;j<lpIEC101->TypeInfAdd;j++)		//信息体地址 fulianqiang 2005.9.2
	          	    *(lpby + byMsgNum ++) = dwYcAdd.Byte[j];		    
			dwYcAdd.Dword -= i;
			*(lpby + byMsgNum ++) = (u8)nTempYc;	// 遥测值
			*(lpby + byMsgNum ++) = (u8)(nTempYc >> 8);
			if(++bySendYcOverNum >= IEC101_YCCNPF)
				break;
		}
	}
	if (i == lpIEC101->SendYcN)
		lpIEC101->nStartYcOver = 0;
	else
		lpIEC101->nStartYcOver = i;
	if (bySendYcOverNum)
		*lpInfoNum = bySendYcOverNum;
	else
		byMsgNum = 0;
#endif        
	return byMsgNum;
}
//上送二级数据
void SendData2(void)
{
	u8 byMsgNum = 0;
#if 1        
       	SearchFirstData();
	if (lpIEC101->firstData != nofirstdata)
	{
		lpIEC101->PSeAppLayer.LinkFunCode = 9;
		lpIEC101->PSeAppLayer.byMsgNum = 0;
		return;
	}
#if 0       
	//事件顺序纪录数据
	if (lpChanl[ChanlNo].ReadSoeC!=sHostSoe.bySoeWrite)
	{
		byMsgNum = OrgnizeSoeMsg();
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 8;
	}
#endif        
   	if (!byMsgNum)					// 超过门限值的遥测
	{
		byMsgNum = OrgnizeYcOverMsg();
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 8;
	}      	    
	if (!byMsgNum)
	{
		lpIEC101->PSeAppLayer.byMsgNum = 0;
		//lpIEC101->PSeAppLayer.LinkFunCode = 0xE5;
                lpIEC101->PSeAppLayer.LinkFunCode = YES_ACK;
	} 
#endif        
}
//组总召唤确认信息体
u8 OrgnizeGenAck(u8 bySendReason)
{
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 i,byMsgNum = 0;        
	*(lpby + byMsgNum ++) = GeneralCall;		// 100
	*(lpby + byMsgNum ++) = 0x01;
	*(lpby + byMsgNum ++) = bySendReason;
	if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;

       //	*(lpby + byMsgNum ++) = lpIEC101->byCmmAdd;
       	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
        if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
           *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
        for(i=0;i<lpIEC101->TypeInfAdd;i++)
	   *(lpby + byMsgNum ++) = 0;
	*(lpby + byMsgNum ++) = lpIEC101->byQOI;       
	return byMsgNum;
}
//组全遥信信息体
u8 OrgnizeYxMsg(u8 *lpby,u8 bySendReason,u8 byFrameNo)
{
	u8 j,byYxVal;
	u8 byMsgNum = 0;
#if 0        
    //WORD wStartAd;
 FOUR_BYTE_TO_DWORD  dwStartAd;
    int nYxNo;
	int i;
	u8  byYxNPF;	//每帧遥信数
    static WORD wStartYxAd[16] = {0x001, 0x041, 0x081, 0x0C1, 0x101, 0x141, 0x181, 0x1C1, 
						   0x201, 0x241, 0x281, 0x2C1, 0x301, 0x341, 0x381, 0x3C1};
	byYxNPF=lpIEC101->YxNPF[byFrameNo];	//取得实际的每帧遥信数,最后一帧可能不是64
	if (byFrameNo<lpIEC101->YxFN)	//帧数小于需要发送的遥信帧数
	{
		*(lpby + byMsgNum ++) = 1;		// 1类型标识
		*(lpby + byMsgNum ++) = 0x80 | byYxNPF;	  //可变结构限定词
		*(lpby + byMsgNum ++) = bySendReason;
		if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
        	   *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;

		//*(lpby + byMsgNum ++) = lpIEC101->byCmmAdd;
       		*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
       		if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
           	    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;

		dwStartAd.Word[0] = wStartYxAd[byFrameNo];
		dwStartAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
		for(j=0;j<lpIEC101->TypeInfAdd;j++)	  //信息体地址 fulianqiang 2005.9.2
		   *(lpby + byMsgNum ++) = dwStartAd.Byte[j];
		nYxNo = byFrameNo * IEC101_YXNPF;
		for (i = nYxNo; i < nYxNo + byYxNPF; i++)
		{
			byYxVal = GetYx(i,YX_BIT);
			if (byYxVal)
				*(lpby + byMsgNum ++) = 1;		// 带品质描述的单点信息 6.4.1
			else
				*(lpby + byMsgNum ++) = 0;
		}
	}
#endif        
	return byMsgNum;
}
//组全遥测信息体
u8 OrgnizeYcMsg(u8* lpby,u8 bySendReason,u8 byFrameNo)
{
	u8 byMsgNum = 0;
	int i;
	int wYcNo;
       //	WORD wStartAd;      
        union IEC101_DADD dwStartAd;
        unsigned char databuf[8];
	unsigned long  nVal;
        float f_val;
	u8 j,byYcNPF;	//每帧遥测数
	static unsigned short wStartYcAd[8] = {0x701, 0x741, 0x781, 0x7C1, 0x801, 0x841, 0x881, 0x8C1};
	static unsigned short wStartYcAd2002[8] = {0x4001, 0x4020, 0x403f, 0x404e, 0x406c, 0x407b, 0x4099, 0x40a8};
	byYcNPF=lpIEC101->YcNPF[byFrameNo];	//取得实际的每帧遥测数,最后一帧可能不是64
	if (byFrameNo<lpIEC101->YcFN)	//帧数小于需要发送的遥测帧数
	{
		*(lpby + byMsgNum ++) = 0x0d;		// 21
		*(lpby + byMsgNum ++) = 0x80 | byYcNPF;
		*(lpby + byMsgNum ++) = bySendReason;
         	if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
                   *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
		*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
       		if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
           	    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;

		if (lpIEC101->TypeProtocol)	//2002版101规约
			dwStartAd.Word[0] = wStartYcAd2002[byFrameNo];
		else
			dwStartAd.Word[0] = wStartYcAd[byFrameNo];
		dwStartAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
	        for(j=0;j<lpIEC101->TypeInfAdd;j++)
	            *(lpby + byMsgNum ++) = dwStartAd.Byte[j];
		wYcNo = byFrameNo * IEC101_YCNPF;
		for (i = wYcNo; i < wYcNo + byYcNPF; i++)
		{
			//为每一路遥测赋值
			f_val = GetYc(i);
                        switch(i/31)
                        {
                        case 0:
                        case 1:
                        case 2:
                        case 3:
                        case 7:
                        case 8:
                        case 9:
                          f_val = f_val/1000;
                          break;
                        case 4:
                        case 5:
                        case 6:
                          f_val = f_val/10;
                          break;
                        default:
                          break;
                        }
                        memcpy(databuf,&f_val,4);
			*(lpby + byMsgNum ++) = databuf[0];//(u8)nVal;	// 遥测值
			*(lpby + byMsgNum ++) = databuf[1];//(u8)(nVal >> 8);
                        *(lpby + byMsgNum ++) = databuf[2];//(u8)(nVal >> 16);
                        *(lpby + byMsgNum ++) = databuf[3];//(u8)(nVal >> 24);
                        *(lpby + byMsgNum ++) = 0;
		}
	}        
	return byMsgNum;
}
//上送总召唤数据
void SendGeneralData(void)
{ 
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 byMsgNum = 0;
  u8 bySendReason;
  u8 byFrameNo;
  if (lpIEC101->byPSGenStep == 0)
  {
    if (lpIEC101->byReason == ACT)
      bySendReason = ACTCON;
    else if (lpIEC101->byReason == DEACT)
      bySendReason = DEACTCON;
    byMsgNum = OrgnizeGenAck(bySendReason);
    lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
    lpIEC101->PSeAppLayer.LinkFunCode = 3;//8;
    lpIEC101->byPSGenStep++;
  }
  else if ( (lpIEC101->byPSGenStep >= 1) && (lpIEC101->byPSGenStep <= 8) )
  {
    bySendReason = lpIEC101->byQOI;
    byFrameNo = (lpIEC101->byPSGenStep - 1) * 2;
    if (lpIEC101->frameno)
    {
      byFrameNo ++;
      lpIEC101->byPSGenStep++;
    }
    lpIEC101->frameno = lpIEC101->frameno ? 0 : 1;
    byMsgNum = OrgnizeYxMsg(lpby,bySendReason,byFrameNo);
    lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
    lpIEC101->PSeAppLayer.LinkFunCode = 3;
    if (!byMsgNum)	//没有实际信息体内容
    {
      if (lpIEC101->byQOI==INTROGEN)
			//总召唤时需要发送的遥信已发完,直接跳到发送遥测帧,但对于分组召唤命令,继续发送空信息响应帧
      {
        lpIEC101->byPSGenStep = 9;	//跳至发送遥测帧
        lpIEC101->frameno = 0;		//从一组内的第一帧开始发送
				//执行下一步的程序
        bySendReason = lpIEC101->byQOI;
	byFrameNo = (lpIEC101->byPSGenStep - 9) * 2;
        if (lpIEC101->frameno)	//两帧为一组,步进1
        {
          byFrameNo ++;
          lpIEC101->byPSGenStep++;
	}
	lpIEC101->frameno = lpIEC101->frameno ? 0 : 1;
	byMsgNum = OrgnizeYcMsg(lpby,bySendReason,byFrameNo);
	lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
	lpIEC101->PSeAppLayer.LinkFunCode = 3;
        if (!byMsgNum)	//没有实际信息体内容
        {
          if (lpIEC101->byQOI==INTROGEN)	
          //总召唤时需要发送的遥测已发完,直接跳到结束帧,但对于分组召唤命令,继续发送空信息响应帧
          {
            lpIEC101->byPSGenStep = 16;	//跳至总召唤结束帧
            bySendReason = ACTTERM;
            byMsgNum = OrgnizeGenAck(bySendReason);
            lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
            lpIEC101->PSeAppLayer.LinkFunCode = 3;	//总召唤结束帧功能码为8
          }
          else	//分组召唤
		lpIEC101->PSeAppLayer.LinkFunCode = 9;	//无所请求数据的确认帧
        }
      }
      else	//分组召唤
        lpIEC101->PSeAppLayer.LinkFunCode = 9;	//无所请求数据的确认帧
    }
  }
  else if (lpIEC101->TypeProtocol==0)//97-101
  {
    if((lpIEC101->byPSGenStep >= 9) && (lpIEC101->byPSGenStep <= 12))
    {
      bySendReason = lpIEC101->byQOI;
      if(lpIEC101->byReason==1)
        bySendReason=lpIEC101->byReason;
      byFrameNo = (lpIEC101->byPSGenStep - 9) * 2;
      if (lpIEC101->frameno)	//两帧为一组,步进1
      {
        byFrameNo ++;
        lpIEC101->byPSGenStep++;
      }
      if(lpIEC101->byPSGenStep == 13)
        lpIEC101->byPSGenStep = 15;
      lpIEC101->frameno = lpIEC101->frameno ? 0 : 1;
      byMsgNum = OrgnizeYcMsg(lpby,bySendReason,byFrameNo);
      lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
      lpIEC101->PSeAppLayer.LinkFunCode = 8;
      if(lpIEC101->byReason==1)
        lpIEC101->PSeAppLayer.LinkFunCode = 4;
      if (!byMsgNum)	//没有实际信息体内容
      {
        if (lpIEC101->byQOI==INTROGEN)	
        //总召唤时需要发送的遥测已发完,直接跳到结束帧,但对于分组召唤命令,继续发送空信息响应帧
        {
          lpIEC101->byPSGenStep = 16;	//跳至总召唤结束帧
          bySendReason = ACTTERM;
          byMsgNum = OrgnizeGenAck(bySendReason);
          lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
          lpIEC101->PSeAppLayer.LinkFunCode = 8;	//总召唤结束帧功能码为8
        }
        else	//分组召唤
          lpIEC101->PSeAppLayer.LinkFunCode = 9;	//无所请求数据的确认帧
      }
    }
  }
  else if(lpIEC101->TypeProtocol!=0)//2002-101
  {
    if((lpIEC101->byPSGenStep >= 9) && (lpIEC101->byPSGenStep <= 14))
    {
      bySendReason = lpIEC101->byQOI;
      if(lpIEC101->byReason==1)
        bySendReason=lpIEC101->byReason;
      byFrameNo = (lpIEC101->byPSGenStep - 9) * 2;
      if (lpIEC101->frameno)	//两帧为一组,步进1
      {
        byFrameNo ++;
        lpIEC101->byPSGenStep++;
      }
      lpIEC101->frameno = lpIEC101->frameno ? 0 : 1;
      byMsgNum = OrgnizeYcMsg(lpby,bySendReason,byFrameNo);
      lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
      lpIEC101->PSeAppLayer.LinkFunCode = 3;
      if(lpIEC101->byReason==1)
        lpIEC101->PSeAppLayer.LinkFunCode = 4;
      if (!byMsgNum)	//没有实际信息体内容
      {
        if (lpIEC101->byQOI==INTROGEN)	
          //总召唤时需要发送的遥测已发完,直接跳到结束帧,但对于分组召唤命令,继续发送空信息响应帧
        {
          lpIEC101->byPSGenStep = 16;	//跳至总召唤结束帧
          bySendReason = ACTTERM;
          byMsgNum = OrgnizeGenAck(bySendReason);
          lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
          lpIEC101->PSeAppLayer.LinkFunCode = 3;	//总召唤结束帧功能码为8
        }
	else	//分组召唤
          lpIEC101->PSeAppLayer.LinkFunCode = 9;	//无所请求数据的确认帧
      }
    }
  }
  else if (lpIEC101->byPSGenStep == 15)
  {
    bySendReason = ACTTERM;
    byMsgNum = OrgnizeGenAck(bySendReason);
    lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
    lpIEC101->PSeAppLayer.LinkFunCode = 8;	//总召唤结束帧功能码为8
    lpIEC101->byPSGenStep++;
  }       
}
//组时钟信息体
u8 OrgnizeTimeMsg(void)
{ 
	unsigned short wMS;
        unsigned char Buff[8];
        unsigned char* Point;
        Point = Buff;
        HT_RTC_Read(Point);
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 j,byMsgNum = 0;
	u8 byTimeValid = 0;

	*(lpby + byMsgNum ++) = 103;	// 103
	*(lpby + byMsgNum ++) = 1;
        if(lpIEC101->byReason == ACT)
          *(lpby + byMsgNum ++) = ACTCON;
        else
          *(lpby + byMsgNum ++) = REQ;
	if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;

	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
       	if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
             *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;

	for(j=0;j<lpIEC101->TypeInfAdd;j++)
	    *(lpby + byMsgNum ++) = 0;

	//取系统时间
	wMS = *(Point+6) * 1000;
	*(lpby + byMsgNum ++) = (u8)wMS;					// 毫秒
	*(lpby + byMsgNum ++) = (u8)(wMS >> 8);
	*(lpby + byMsgNum ++) = 0 << 7 | *(Point+5);	// IV = 0 有效 RES1 = 0
	*(lpby + byMsgNum ++) = *(Point+4);					// SU = 0 标准时间 RES2 = 0
	*(lpby + byMsgNum ++) = *(Point+3) | (*Point) << 5;
	*(lpby + byMsgNum ++) = *(Point+2);					// RES3 = 0
	*(lpby + byMsgNum ++) = *(Point+1);
       	return byMsgNum;       
}

void SendTimeAck(void)
{
  u8 byMsgNum = 0;
  byMsgNum = OrgnizeTimeMsg();
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 8;       
}
//组召唤电度确认信息体
u8 OrgnizeDdAckMsg(u8 bySendReason)
{ 
	u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
	u8 j,byMsgNum = 0;
	*(lpby + byMsgNum ++) = C_CI_NA_1;		// 101
	*(lpby + byMsgNum ++) = 0x81;
	*(lpby + byMsgNum ++) = bySendReason;
	if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
           *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
	*(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
       	if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
             *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
	for(j=0;j<lpIEC101->TypeInfAdd;j++)
	    *(lpby + byMsgNum ++) = 0;

	*(lpby + byMsgNum ++) = lpIEC101->byQCC;
	return byMsgNum;      
}
//组电度数据信息体
u8 OrgnizeDdMsg(u8 bySendReason,u8 byFrameNo)
{
  int i;
  FOUR_BYTE_TO_DWORD  dwInfoAd;
  int dwDdVal;
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 j,byMsgNum = 0;
  u8 byDdNPF;	//每帧电度数
  int dwDdAdd;
  byDdNPF=lpIEC101->DdNPF[byFrameNo];	//取得实际的每帧电度数,最后一帧可能不是32
  if (byFrameNo<lpIEC101->DdFN)	//帧数小于需要发送的电度帧数
  {
    if(byFrameNo%2)
    {
      *(lpby + byMsgNum ++) = M_IT_TC_1;	
      *(lpby + byMsgNum ++) = 0x80|byDdNPF;
     }
     else
     {
      *(lpby + byMsgNum ++) = M_IT_NB_1;	
      *(lpby + byMsgNum ++) = 0x80|byDdNPF;
     }
      *(lpby + byMsgNum ++) = bySendReason;
      if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
        *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
      *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
      if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
      *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
      if(lpIEC101->TypeProtocol)
        dwDdAdd =  IEC101_DDSA_2002+31;
      else
        dwDdAdd =  IEC101_DDSA;
      dwInfoAd.Dword = dwDdAdd+(byFrameNo/2)*31;
      dwInfoAd.Word[1] = lpIEC101->dwInfAdd.Word[1];
      for(j=0;j<lpIEC101->TypeInfAdd;j++)
        *(lpby + byMsgNum ++) = dwInfoAd.Byte[j];
      for (i = byFrameNo * IEC101_DDNPF; i < byFrameNo * IEC101_DDNPF + byDdNPF; i++)
      {
#if 0                        
			dwDdVal = 0;			// 电度发送值
			dwDdVal = i;//GetDd(i);
			*(lpby + byMsgNum ++) = (u8)dwDdVal;
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 8);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 16);
			*(lpby + byMsgNum ++) = (u8)(dwDdVal >> 24);
			*(lpby + byMsgNum ++) = i - byFrameNo * IEC101_DDNPF;	// IV CA CY SQ--6.4.7
#else
                        dwDdVal=GetDd(i,lpby + byMsgNum);
                        byMsgNum +=dwDdVal;
                        if(byFrameNo%2==0)
                          *(lpby + byMsgNum ++) = 0;
                       // byMsgNum +=dwDdVal;
                       // *(lpby + byMsgNum ++) = i - byFrameNo * IEC101_DDNPF;
#if 0                       
                        if(dwDdVal==4)
                        {
                          byMsgNum +=dwDdVal;
                          *(lpby + byMsgNum ++) = i - byFrameNo * IEC101_DDNPF;
                        }
                        else
                        {
                           *(lpby + byMsgNum+4) = i - byFrameNo * IEC101_DDNPF;
                           byMsgNum +=dwDdVal;
                        }
#endif                        
                        
#endif                        
		}
	}
	return byMsgNum;
}
//为了兼容97和2002电度召唤，有两种解决方法：
//1）比较直观的解决办法，另写一电镀函数分别调用，本文用此方法
//2）另一种解决方案设bl2002为是否2002版的判别变量，在电度函数中用以下代码替换原来的代码，次方法可以减少代码量
		//if((（bl2000==TRUE)&&(lpIEC101->byQCC&0X3F)==5))||(（bl2000==FALSE)&&(lpIEC101->byQCC&0X3F)==1)))
		 //   bySendReason = REQCGCN;
		 //else
		 //   bySendReason = (lpIEC101->byQCC & 0x3f) - 1 + REQCOGCN;// 并且在lpIEC101->byPSDdStep赋值的时候有如下
		 //程序向配合
		 //if(bl2002)
		 //{
		 //  if(  lpIEC101->byQCC&0X3F)==5)
		 //  lpIEC101->byPSDdStep=0;
		 //  else
		 //  lpIEC101->byPSDdStep=lpIEC101->byQCC&0X3F;
		 //}
		 //else
		 // lpIEC101->byPSDdStep=lpIEC101->byQCC&0X3F-1; 
		 //其它的地方不需要改   
//上送电度数据
void SendDdData(void)
{
        
}
//2002版的电度召唤函数	  //付2005.8.30
void SendDdData2002(void)
{
        u8 byMsgNum = 0;
        u8 bySendReason;
        u8 byFrameNo;
	if (lpIEC101->byPSDdStep == 17 || lpIEC101->byPSDdStep == 0)
	{
		if (lpIEC101->byReason == ACT)
			bySendReason = ACTCON;
		else if (lpIEC101->byReason == DEACT)
			bySendReason = DEACTCON;
		byMsgNum = OrgnizeDdAckMsg(bySendReason);
		lpIEC101->byPSDdStep=0;
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 3;
	}
	else if ( (lpIEC101->byPSDdStep >= 1) && (lpIEC101->byPSDdStep <= 16) )
	{
		byFrameNo = lpIEC101->byPSDdStep - 1;
		if ( (lpIEC101->byQCC & 0x3f) == 5)
			bySendReason = REQCOGCN;		// 37响应帧的传送原因
		else if ( ((lpIEC101->byQCC & 0x3f) >= 1) 
			&& ((lpIEC101->byQCC & 0x3f) <= 4) )
			bySendReason = (lpIEC101->byQCC & 0x3f) - 1 + REQCOGCN;
		byMsgNum = OrgnizeDdMsg(bySendReason,byFrameNo);
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 3;
		if ( lpIEC101->byPSDdStep==lpIEC101->DdFN)
		     lpIEC101->byPSDdStep=17;
		if (!byMsgNum)	//没有实际电度信息体内容
		{
			if (bySendReason==REQCOGCN)
			//响应计数量召唤时需要发送的电度已发完,直接跳到结束帧,但对于分组召唤命令,继续发送空信息响应帧
			{
				lpIEC101->byPSDdStep = 17;	
				bySendReason = ACTTERM;
                               // lpIEC101->byReason=ACTTERM;
				byMsgNum = OrgnizeDdAckMsg(bySendReason);
				lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
				lpIEC101->PSeAppLayer.LinkFunCode = 3;
			}
			else	//分组召唤
				lpIEC101->PSeAppLayer.LinkFunCode = 9;	//无所请求数据的确认帧
		}	
	}       
	else if(lpIEC101->byPSDdStep == 18)
	{
               // lpIEC101->byReason=ACTTERM;
		bySendReason = ACTTERM;
		byMsgNum = OrgnizeDdAckMsg(bySendReason);
		lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
		lpIEC101->PSeAppLayer.LinkFunCode = 3;
	}     
	lpIEC101->byPSDdStep++;
        
}

//组读数据命令确认帧
void SendReadDataAck(void)
{
  lpIEC101->PSeAppLayer.byMsgNum = 0;
  lpIEC101->PSeAppLayer.LinkFunCode = 0;

}
//组遥控返校信息体
u8 OrgnizeYkRetMsg(u8 bySendReason)
{

	return 0;
}
//向CAN网下发遥控命令
void ReceiveYktoCan(void)
{
	
}
//组遥控返校
void SendYkReply(void)
{
	
}
//组通道测试信息体
u8 OrgnizeTestMsg(void)
{
  u8 *lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 j,byMsgNum = 0;
  *(lpby + byMsgNum ++) = 104;			// 104
  *(lpby + byMsgNum ++) = 0;//1;
  *(lpby + byMsgNum ++) = ACTCON;		// 7
  if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
    *(lpby + byMsgNum ++) = 0;//lpIEC101->bySourceAdd;
  *(lpby + byMsgNum ++) = 0x01;//lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
    *(lpby + byMsgNum ++) = 0x00;//lpIEC101->wCmmAdd.Byte.h;
  for(j=0;j<lpIEC101->TypeInfAdd;j++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = lpIEC101->wTester.Byte.l;
  *(lpby + byMsgNum ++) = lpIEC101->wTester.Byte.h;
  lpIEC101->wTester.Byte.l =0;
  lpIEC101->wTester.Byte.h =0;
  return byMsgNum;
}
//上送通道测试确认
void SendChanTestAck(void)
{
  u8 byMsgNum = 0;
  byMsgNum = OrgnizeTestMsg();
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 3;
  lpIEC101->Pacd = 0x2;
  lpIEC101->Pdfc = 1;
}

void Dir_Send(void)
{
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 i,byMsgNum = 0;        
  if(lpIEC101->byPSGenStep>1)
  {
    return;
  }
  *(lpby + byMsgNum ++) = F_FR_NA_1;
  *(lpby + byMsgNum ++) = 0x01;
  *(lpby + byMsgNum ++) = REQ;
  if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = 0x02; //
  *(lpby + byMsgNum ++) = 0x02;
  *(lpby + byMsgNum ++) = 0x00;
  memcpy(lpby + byMsgNum,&(lpIEC101->FId),4);byMsgNum+=4;
  if(lpIEC101->byPSGenStep)
  {
    *(lpby + byMsgNum ++) = 0x00;
  }
  else
  {
    *(lpby + byMsgNum ++) = 0x01;
  }
  *(lpby + byMsgNum ++) = 0x05;
  for(i=0;i<5;++i)
  {
    *(lpby + byMsgNum) = strlen(File_List[i+lpIEC101->byPSGenStep*5]);
    memcpy((lpby + byMsgNum + 1),File_List[i+lpIEC101->byPSGenStep*5],*(lpby + byMsgNum));
    byMsgNum += (*(lpby + byMsgNum)+1);
    *(lpby + byMsgNum++) = 0;
    *(lpby + byMsgNum++) = 0x88+i+lpIEC101->byPSGenStep*5;
    *(lpby + byMsgNum++) = 0;
    *(lpby + byMsgNum++) = 0;
    *(lpby + byMsgNum++) = 0;
    *(lpby + byMsgNum++) = 0x50;
    *(lpby + byMsgNum++) = 0x46; 
    *(lpby + byMsgNum++) = 0x20;
    *(lpby + byMsgNum++) = 0x0A;
    *(lpby + byMsgNum++) = 0x1B;
    *(lpby + byMsgNum++) = 0x0A;
    *(lpby + byMsgNum++) = 0x11;
  }
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 3;
  lpIEC101->byPSGenStep++;
}

void File_Recv(void)
{
}

//文件召唤确认信息体
u8 SendFileGenAck(u8 bySendReason)
{
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 i,byMsgNum = 0;
  u32 m_filelen;
  *(lpby + byMsgNum ++) = F_FR_NA_1;		// 210
  *(lpby + byMsgNum ++) = 0x01;
  *(lpby + byMsgNum ++) = bySendReason;
  if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = 0x2;    
  *(lpby + byMsgNum ++) = 0x4;
  *(lpby + byMsgNum ++) = 0x0;
  *(lpby + byMsgNum ++) = strlen(lpIEC101->Fname);
  strcpy(lpby + byMsgNum,lpIEC101->Fname);
  byMsgNum += strlen(lpIEC101->Fname);
  memcpy(lpby + byMsgNum,&(lpIEC101->FId),4);
  byMsgNum += 4;
  memcpy(lpby + byMsgNum,&(m_filelen),4);
  byMsgNum += 4;
  return byMsgNum;
}

//文件信息体
u8 SendFileInfo(u8 bySendReason)
{
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 i,byMsgNum = 0,pos,bySum=0,j;
  char *p_filename;
  u32 m_filelen;
  *(lpby + byMsgNum ++) = F_FR_NA_1;		// 210
  *(lpby + byMsgNum ++) = 0x01;
  *(lpby + byMsgNum ++) = 5;
  if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = 0x2;    
  *(lpby + byMsgNum ++) = 0x5;
  memcpy(lpby + byMsgNum,&(lpIEC101->FId),4);
  byMsgNum += 4;
  m_filelen = 0;
  memcpy(lpby + byMsgNum,&(m_filelen),4);
  byMsgNum += 4;
  *(lpby + byMsgNum ++) = 0x1;
  pos = byMsgNum;
#if 0  
  strcpy(lpby + byMsgNum,lpIEC101->Fname+14);
  byMsgNum += strlen(lpIEC101->Fname+14);
#else
  p_filename = lpIEC101->Fname;
  if(p_filename)
  {
    p_filename = (char *)strstr((const char *)p_filename,"/");
    if(p_filename)
    {
      p_filename += 1;
      p_filename = (char *)strstr(p_filename,"/");
    }
    p_filename += 1;
    strcpy(lpby + byMsgNum,p_filename);
    byMsgNum += strlen(p_filename);
  }
#endif  
  strcpy(lpby + byMsgNum,"  v1.0\r\n");
  byMsgNum += strlen("  v1.0\r\n");
  strcpy(lpby + byMsgNum,"201710300001,96,2\r\n");
  byMsgNum += strlen("201710300001,96,2\r\n");
  for(j=pos;j<byMsgNum;j++)
    bySum = bySum + *(lpby + j);
  *(lpby + byMsgNum ++) = bySum;
  return byMsgNum;
}


//文件数据
u8 SendFileData(u8 bySendReason)
{
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 i,byMsgNum = 0,pos,bySum=0,j;
  u32 m_filelen;
  *(lpby + byMsgNum ++) = F_FR_NA_1;		// 210
  *(lpby + byMsgNum ++) = 0x01;
  *(lpby + byMsgNum ++) = 5;
  if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  for(i=0;i<lpIEC101->TypeInfAdd;i++)
    *(lpby + byMsgNum ++) = 0;
  *(lpby + byMsgNum ++) = 0x2;    
  *(lpby + byMsgNum ++) = 0x5;
  memcpy(lpby + byMsgNum,&(lpIEC101->FId),4);
  byMsgNum += 4;
  m_filelen = (lpIEC101->byPSGenStep-1)*256;
  memcpy(lpby + byMsgNum,&(m_filelen),4);
  byMsgNum += 4;
  if(lpIEC101->byPSGenStep<9)
    *(lpby + byMsgNum ++) = 0x1;
  else
    *(lpby + byMsgNum ++) = 0x0;
  pos = byMsgNum;
  if(strstr(lpIEC101->Fname,".msg"))
  {
    strcpy(lpby + byMsgNum,"12,2017-10-24 15:15:00,6401,   10.71,6402,    0.23,6403,    0.23,6404,    0.08,6405,    0.00,6406,    0.08,6407,    0.00,6408,    0.00,4008,-    0.003,4009,-    0.003,400A,-    0.003,400B,-    0.006\r\n");
    byMsgNum += strlen("12,2017-10-24 15:15:00,6401,   10.71,6402,    0.23,6403,    0.23,6404,    0.08,6405,    0.00,6406,    0.08,6407,    0.00,6408,    0.00,4008,-    0.003,4009,-    0.003,400A,-    0.003,400B,-    0.006\r\n");
  }
  else if(strstr(lpIEC101->Fname,".xml"))
  {
    switch(lpIEC101->byPSGenStep)
    {
    case 2:
      strcpy(lpby + byMsgNum,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"FIXD\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"96\" interval=\"15min\">\r\n");
      byMsgNum += strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<DataFile>\r\n\t<Header fileType=\"FIXD\" fileVer=\"1.00\" devName=\"201709030019\" />\r\n\t<DataAttr dataNum=\"12\" sectNum=\"96\" interval=\"15min\">\r\n");
      break;
    case 3:
      strcpy(lpby + byMsgNum,"\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25602\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25603\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25604\" type=\"float\" unit=\"kVARh\" />\r\n");
      byMsgNum += strlen("\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25602\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25603\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25604\" type=\"float\" unit=\"kVARh\" />\r\n");
      break;
    case 4:
      strcpy(lpby + byMsgNum,"\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25605\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25606\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25607\" type=\"float\" unit=\"kVARh\" />\r\n");
      byMsgNum += strlen("\t\t<DI ioa=\"25601\" type=\"float\" unit=\"kWh\" />\r\n \
    \t\t<DI ioa=\"25605\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25606\" type=\"float\" unit=\"kVARh\" />\r\n \
    \t\t<DI ioa=\"25607\" type=\"float\" unit=\"kVARh\" />\r\n");
      break;
    case 5:
      strcpy(lpby + byMsgNum,"\t\t<DI ioa=\"16392\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16393\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16394\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16395\" type=\"float\" unit=\"W\" />\r\n \
  \t</DataAttr>\r\n");
      byMsgNum += strlen("\t\t<DI ioa=\"16392\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16393\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16394\" type=\"float\" unit=\"W\" />\r\n \
    \t\t<DI ioa=\"16395\" type=\"float\" unit=\"W\" />\r\n \
  \t</DataAttr>\r\n");
             break;
    default:
      if(lpIEC101->byPSGenStep%2==0)
      {
      strcpy(lpby + byMsgNum,"\t<DataRec sect=\"1\" tm=\"20171025_113000\">\r\n \
      \t\t<DI val=\"10.718\"/>\r\n \
      \t\t<DI val=\"0.236\"/>\r\n \
      \t\t<DI val=\"0.236\"/>\r\n \
      \t\t<DI val=\"0.082\"/>\r\n \
      \t\t<DI val=\"0.0\"/>\r\n \
      \t\t<DI val=\"0.082\"/>\r\n");
      byMsgNum += strlen("\t<DataRec sect=\"1\" tm=\"20171025_113000\">\r\n \
      \t\t<DI val=\"10.718\"/>\r\n \
      \t\t<DI val=\"0.236\"/>\r\n \
      \t\t<DI val=\"0.236\"/>\r\n \
      \t\t<DI val=\"0.082\"/>\r\n \
      \t\t<DI val=\"0.0\"/>\r\n \
      \t\t<DI val=\"0.082\"/>\r\n");
      }
      else
      {
        strcpy(lpby + byMsgNum,"\t\t<DI val=\"10.718\"/>\r\n \
        \t<DI val=\"0.236\"/>\r\n \
        \t<DI val=\"0.236\"/>\r\n \
        \t<DI val=\"0.082\"/>\r\n \
        \t<DI val=\"0.0\"/>\r\n \
        \t<DI val=\"0.082\"/>\r\n\t</DataRec>\r\n");
        byMsgNum += strlen("\t\t<DI val=\"10.718\"/>\r\n \
        \t<DI val=\"0.236\"/>\r\n \
        \t<DI val=\"0.236\"/>\r\n \
        \t<DI val=\"0.082\"/>\r\n \
        \t<DI val=\"0.0\"/>\r\n \
        \t<DI val=\"0.082\"/>\r\n\t</DataRec>\r\n");
        if(lpIEC101->byPSGenStep>=9)
        {
          strcpy(lpby + byMsgNum,"</DataFile>\r\n");
          byMsgNum += strlen("</DataFile>\r\n");
        }
      } 
      break;
    }
  }
   for(j=pos;j<byMsgNum;j++)
    bySum = bySum + *(lpby + j);
  *(lpby + byMsgNum ++) = bySum;
  return byMsgNum;
}

void File_Send(void)
{
  u8 bySendReason;
  u8 byMsgNum=0;
  if(lpIEC101->byPSGenStep)
  {
    if(lpIEC101->byPSGenStep==1)
    {
      byMsgNum=SendFileInfo(1);
      lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
      lpIEC101->PSeAppLayer.LinkFunCode = 3;
    }
    else if(lpIEC101->byPSGenStep<10)
    {
      byMsgNum=SendFileData(1);
      lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
      lpIEC101->PSeAppLayer.LinkFunCode = 3;
    }
  }
  else
  {
    if (lpIEC101->byReason == ACT)
      bySendReason = ACTCON;
    else if (lpIEC101->byReason == DEACT)
      bySendReason = DEACTCON;
    byMsgNum = SendFileGenAck(bySendReason);
    lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
    lpIEC101->PSeAppLayer.LinkFunCode = 3;
  }
  ++lpIEC101->byPSGenStep;
}

void File_Server(void)
{
  switch(lpIEC101->Fop)
  {
  case F_FR_DA_1:
    Dir_Send();
    break;
  case F_FR_FA_1:
    File_Send();
    break;
  case F_FW_FA_1:
      File_Recv();
    break;
  default:
    break;
  }
}
void Sn_Read(void)
{
  u8* lpby = lpIEC101->PSeAppLayer.lpByBuf;
  u8 i,byMsgNum = 0,j;        
  *(lpby + byMsgNum ++) = C_RS_NA_1;
  *(lpby + byMsgNum ++) = lpIEC101->byQualify;
  *(lpby + byMsgNum ++) = ACTCON;
  if (lpIEC101->TypeSeReason==2)			//传送原因两个字节
    *(lpby + byMsgNum ++) = lpIEC101->bySourceAdd;
  *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.l;   //2005.9.2
  if(lpIEC101->TypeCmmAdd==2)	   //是否两个字节的应用服务单元地址
    *(lpby + byMsgNum ++) = lpIEC101->wCmmAdd.Byte.h;
  *(lpby + byMsgNum ++) = lpIEC101->Sn&0xff; 
  *(lpby + byMsgNum ++) = (lpIEC101->Sn>>8)&0xff; 
  *(lpby + byMsgNum ++) = 0x00;
 
  for(i=0;i<lpIEC101->byQualify;++i)
  {
    *(lpby + byMsgNum ++) = lpIEC101->List_para[i]&0xff; 
    *(lpby + byMsgNum ++) = (lpIEC101->List_para[i]>>8)&0xff; 
    *(lpby + byMsgNum ++) = 4;
    for(j=0;j<9;++j)
    {
      if(lpIEC101->List_para[i]==s_sninfo[j].info_addr)
        break;
    }
    if(j<9)
    {
      *(lpby + byMsgNum ++) = s_sninfo[j].data_len;
      memcpy(lpby + byMsgNum,s_sninfo[j].info_data,s_sninfo[j].data_len);
      byMsgNum += s_sninfo[j].data_len;
    }
    else
    {
      *(lpby + byMsgNum ++) = 0;
    }
  }
  lpIEC101->PSeAppLayer.byMsgNum = byMsgNum;
  lpIEC101->PSeAppLayer.LinkFunCode = 3;
  lpIEC101->byPSGenStep++;
  lpIEC101->PReMsgType = 0;
}
//组信息帧
void AppVFrame(void)
{
  switch (lpIEC101->PReMsgType)
  {
  case GeneralCall:
      SendGeneralData();
      lpIEC101->PSeAppLayer.byFull = 1;
      if ( (lpIEC101->byReason == ACT) && (lpIEC101->byPSGenStep == 16) )
        lpIEC101->PReMsgType = 0;
      else if( (lpIEC101->byReason == REQ) && (!lpIEC101->frameno) )
        lpIEC101->PReMsgType = 0;
      else if(lpIEC101->byReason == DEACT)
        lpIEC101->PReMsgType = 0;
      break;
    case C_CS_NA_1:				//校时确认
      SendTimeAck();
      lpIEC101->PSeAppLayer.byFull = 1;
      lpIEC101->PReMsgType = 0;
      break;
    case C_CI_NA_1:				//召唤电度
      if ( lpIEC101->TypeProtocol)//2002版的101规约//付2005.8.30
        SendDdData2002();
      else
        SendDdData();
      lpIEC101->PSeAppLayer.byFull = 1;
    
      if( (lpIEC101->byReason == ACT) && (lpIEC101->byPSDdStep == 17) )
        lpIEC101->PReMsgType = 0;
      else if(lpIEC101->byReason == REQ)
        lpIEC101->PReMsgType = 0;
    else if((lpIEC101->byReason == DEACT))
      lpIEC101->PReMsgType = 0;
    else if(lpIEC101->byPSDdStep > 18)
      lpIEC101->PReMsgType = 0;
    break;
   case C_RD_NA_1:				//读命令确认
     SendReadDataAck();
     lpIEC101->PSeAppLayer.byFull = 1;
     lpIEC101->PReMsgType = 0;
     break;
    case C_SC_NA_1:				//单点遥控
    case C_DC_NA_1:				//遥控
    case C_RC_NA_1:				//升降命令
      SendYkReply();
      if( (lpIEC101->WaitYkXzRet == 0) && (lpIEC101->WaitYkZxRet == 0) )
      {
        lpIEC101->PSeAppLayer.byFull = 1;
        lpIEC101->PReMsgType = 0;
      }
      if (lpIEC101->SendYkZxAck == 1)
      {
        lpIEC101->PSeAppLayer.byFull = 1;
        lpIEC101->SendYkZxAck = 0;
      }
      break;
  case  C_TS_NA_1:
    SendChanTestAck();
    lpIEC101->PReMsgType = 0;
    lpIEC101->PWinTimer=0;
    lpIEC101->PSeAppLayer.byFull = 1;
    break;
  case C_RS_NA_1:
    Sn_Read();
    if(lpIEC101->PSeAppLayer.byMsgNum)
      lpIEC101->PSeAppLayer.byFull = 1;
    else
    {
      lpIEC101->PReMsgType = 0;
    }
    break;
  case F_FR_NA_1: //文件传输
    File_Server();
    if(lpIEC101->PSeAppLayer.byMsgNum)
      lpIEC101->PSeAppLayer.byFull = 1;
    else
    {
      lpIEC101->PReMsgType = 0;
    }
    break;
  default:
    lpIEC101->PSeAppLayer.byFull = 0;
    lpIEC101->PReMsgType = 0;
    break;
  }
}
//从动应用层发送处理函数
void PAppSendProcess(void)
{
  if (lpIEC101->PReFrameType == CALL_DATA1)
  {
    SendData1();
    lpIEC101->PSeAppLayer.byFull = 1;
    lpIEC101->PReFrameType = 0;
  }
  else  
  {
    if(3==lpIEC101->UnsolTimeInterval)
    {
     // if(lpIEC101->PReFrameType)// == CALL_DATA2)
      {
        if( 0==lpIEC101->PReFrameType)
        {
          SendData2();
          lpIEC101->PSeAppLayer.byFull = 1;
          lpIEC101->PReFrameType = 0xff;
        }
        else
        {
          //if (((lpIEC101->PWinTimer >= 200)&& (lpIEC101->PReFrameType==0x3)))
          if((lpIEC101->OrgnizeFrame) && (lpIEC101->PWinTimer >= 100))
          { 
           // lpIEC101->PWinTimer =0 ;
            AppVFrame();
            lpIEC101->PReFrameType = 0xff;
          }
        }
      }
    }
    else
    {
      //if(lpIEC101->PReFrameType == CALL_DATA2)
      //if((lpIEC101->PReFrameType == YES_ACK) || (lpIEC101->PReFrameType == CALL_DATA2))
      if(lpIEC101->PReFrameType && lpIEC101->PReFrameType!=0xff)
      {
#if 0        
        if(lpIEC101->Pacd==0x2)
        {
          lpIEC101->PSeAppLayer.byFull=1;
         // AppVFrame();
          lpIEC101->PReFrameType = 0xff;
        }
        else
#endif          
        {
          SendData2();
          lpIEC101->PSeAppLayer.byFull = 1;
          lpIEC101->PReFrameType = 0xff;
          lpIEC101->Pacd=0x2;
        }
      }
      else if(lpIEC101->Pacd==0x2)
      {
        AppVFrame();
      }
 /*     
      else
      {
        if (lpIEC101->PWinTimer >= 5000)
        {
          
          AppVFrame();
          if(lpIEC101->PSeAppLayer.byFull)
            lpIEC101->PWinTimer=0;
        }
      }
      */
    }
  }

}
//应用层发送处理函数
void Iec101AppSendPro(void)
{
	if ( (lpIEC101->PReFrameType!=0xff || lpIEC101->PReMsgType)
		&& (lpIEC101->PSendFrame.byFull == 0)&&(lpIEC101->PSeAppLayer.byFull == 0))
	{
		PAppSendProcess();
	}
}
//组变长链路帧
u8 OrgnizeVarFrame(u8 *lpby,u8 byMsgNum,u8 byConField)	//fulianqiang 2005.9.2
{
	int i;
	u8 bySum = 0;
	//u8 byMsgNum = lpIEC101->PSeAppLayer.byMsgNum;
	*(lpby + 0) = V_STARTCODE;	// 变长帧起始字节
	*(lpby + 1) = byMsgNum + 1 + lpIEC101->TypeLinkAdd;
	*(lpby + 2) = byMsgNum + 1 + lpIEC101->TypeLinkAdd;
	*(lpby + 3) = V_STARTCODE;	// 变长帧起始字节
	*(lpby + 4) = byConField;
	*(lpby + 5) = lpIEC101->wLinkAdd.Byte.l;
	if( lpIEC101->TypeLinkAdd ==2)
	   *(lpby + 6) = lpIEC101->wLinkAdd.Byte.h;
	for (i = 4; i < byMsgNum+5+lpIEC101->TypeLinkAdd ; i++)
		bySum = bySum + *(lpby + i);
	*(lpby + byMsgNum + 5 + lpIEC101->TypeLinkAdd) = bySum;
	*(lpby + byMsgNum + 6 + lpIEC101->TypeLinkAdd) = STOPCODE;	// 停止字节
	return byMsgNum + 7 + lpIEC101->TypeLinkAdd;
}
//组定长链路帧
u8 OrgnizeFixFrame(u8 *lpby,u8 byConField)
{
  u8 j,bySum = 0,i=0;
  *(lpby + i++) = F_STARTCODE;	// 定长帧起始字节
  *(lpby + i++) = byConField;
  *(lpby + i++) = lpIEC101->wLinkAdd.Byte.l;
  if( lpIEC101->TypeLinkAdd ==2)
    *(lpby + i++) = lpIEC101->wLinkAdd.Byte.h;
  for(j=1;j<i;j++)
    bySum = bySum + *(lpby + j);
  *(lpby + i++) = bySum;
  *(lpby + i++) = STOPCODE;	// 停止字节
  return i;
}
//从动链路层发送处理函数
void PLinkSendProcess(void)
{
	u8 byConField;
	lpIEC101->PSendFrame.byFunCode = lpIEC101->PSeAppLayer.LinkFunCode;
        
       if(lpIEC101->FlagPingH)       
         byConField = 0x80 | (lpIEC101->Pacd << 5) | (lpIEC101->Pdfc << 4)
					| lpIEC101->PSendFrame.byFunCode;
       else
          byConField = (lpIEC101->Pacd << 5) | (lpIEC101->Pdfc << 4)
					| lpIEC101->PSendFrame.byFunCode;
	if (lpIEC101->PSeAppLayer.byMsgNum)
	{
		lpIEC101->PSendFrame.wFrameLen = 
                OrgnizeVarFrame(lpIEC101->PSendFrame.byLinkBuf,lpIEC101->PSeAppLayer.byMsgNum,byConField);
                lpIEC101->PSeAppLayer.byMsgNum=0;  //zzl add
	}
	else
	{
		if (lpIEC101->PSendFrame.byFunCode == 0xE5)
		{
			lpIEC101->PSendFrame.byLinkBuf[0] = 0xE5;
			lpIEC101->PSendFrame.wFrameLen = 1;
		}
		else
		{
			lpIEC101->PSendFrame.wFrameLen = 
                        OrgnizeFixFrame(lpIEC101->PSendFrame.byLinkBuf,byConField);
		}
	}
	lpIEC101->PSendFrame.byFull = 1;
}
//链路层发送处理函数
void Iec101LinkSendPro(void)
{
	if (lpIEC101->PSeAppLayer.byFull == 1)
	{
		PLinkSendProcess();
		lpIEC101->PSeAppLayer.byFull = 0;
	}
}
//链路层发送函数
void Iec101LinkSend(void)
{
        if (lpIEC101->PSendFrame.byFull == 1)
	{
 
          Serial_Write(2,lpIEC101->PSendFrame.byLinkBuf,lpIEC101->PSendFrame.wFrameLen);
          //if (lpIEC101->wPSendNum >= lpIEC101->PSendFrame.wFrameLen)
          {
            lpIEC101->wPSendNum = 0;
            lpIEC101->byFrameIntval = 0;
            lpIEC101->PSendFrame.byFull = 0;
#if 0
            if ( (lpIEC101->PReFrameType == 0) && (lpIEC101->PReMsgType == 0) )
            {
              lpIEC101->PWindow = 0;
              lpIEC101->PWinTimer = 0;
              lpIEC101->OrgnizeFrame = 0 ;
            }
#else
            if((lpIEC101->PReFrameType == 0xff))
            {
              lpIEC101->OrgnizeFrame = 0 ;
              lpIEC101->PWindow = 0;
            }
            lpIEC101->PWinTimer = 0;
#endif            
          }
	}
}
void OrgnizeIec101Frame(void)
{
	//应用层发送处理函数
	Iec101AppSendPro();
	SearchFirstData();  //搜索一级数据放在组应用帧后，组链路帧前fulianqiang 2005.10.14
	//链路层发送处理函数
	Iec101LinkSendPro();
	//链路层发送函数
   //	Iec101LinkSend();
}
//规约各定时器监测函数
void Iec101WatchTime(void)
{
  if(lpIEC101==0)
    return;
  if (lpIEC101->PWindow == 1)
  {
    lpIEC101->byFrameIntval++;
    lpIEC101->PWinTimer++;
  }
  if(3==lpIEC101->UnsolTimeInterval)
  {
    if(lpIEC101->OrgnizeFrame && (lpIEC101->PWinTimer>100))
    {
      if(lpIEC101->PReFrameType!=0xff && lpIEC101->PReFrameType!=0)
      {  
        lpIEC101->PWinTimer = 0;
        lpIEC101->PReFrameType = 0x3;
        lpIEC101->PSendFrame.byFull = 1;
      }
    }
  }
  if (lpIEC101->PWinTimer >= 60000) //大约60秒钟时间,必须保证低波特率时,总召唤信息能够发送完毕
  {
    lpIEC101->PWindow = 0;
    lpIEC101->PWinTimer = 0;
    lpIEC101->PReFrameType = 0xff;
    lpIEC101->PReMsgType = 0;
    lpIEC101->PSendFrame.byFull = 0;
    lpIEC101->wPSendNum = 0;
  }
}
//101规约帧处理函数
void IEC101Process(void)
{
	//解帧函数
	SleaveIec101Frame();
        
	//监视从动窗口
	WatchPWindow();

	//组帧函数
	OrgnizeIec101Frame();
	Iec101LinkSend();//链路层发送
#if 1
	//规约定时器处理
	//Iec101WatchTime();
#endif        

}
//初始化规约私有数据函数
void InitIEC101Func(u8 ChanNo)
{
    
#if 0 
	WORD i,n;
	lpIEC101=&(lpChanl[ChanNo].ChanlPrivM.IEC101ProtPrivM);	//初始化本通道规约私用数据指针
	lpIEC101->byCOI = 0;				// 初始化原因:当地电源开关合上
	lpIEC101->FtuSccReset = FALSE;//通道复位标志
	//初始化 SOE 的读指针
	lpChanl[ChanNo].ReadSoeC = sHostSoe.bySoeWrite;
	//针对SOE队列的遥信变位读计数
	lpIEC101->YxChangeC = sHostSoe.bySoeWrite;	
	lpIEC101->PWindow = 0;
	lpIEC101->PWinTimer = 0;
	lpIEC101->wRecvNum = 0;
	lpIEC101->wRecvLen = BUFLENGTH;
	lpIEC101->wPSendNum = 0;
	lpIEC101->byFrameIntval = 0;
	lpIEC101->firstData = nofirstdata;
	lpIEC101->TypeProtocol = pChannelPara[ChanNo].wChanPara2;
	if(lpIEC101->TypeProtocol == 0)	   //如果选择97版的101则其余的参数无效fulianqiang 2005.9.2
	{
	   lpIEC101->TypeLinkAdd = 1;
	   lpIEC101->TypeSeReason = 1;
	   lpIEC101->TypeCmmAdd = 1;
	   lpIEC101->TypeInfAdd = 2;
	}
	else				   //2002版的101需要设的参数
	{
		if(pChannelPara[ChanNo].wChanPara3==2)
	   	   lpIEC101->TypeLinkAdd = pChannelPara[ChanNo].wChanPara3; //由"通道参数3"设置站地址类型fu 2005.9.1
		else
		   lpIEC101->TypeLinkAdd = 1;
		if( pChannelPara[ChanNo].wChanPara4 ==2)
		   lpIEC101->TypeSeReason = pChannelPara[ChanNo].wChanPara4; //由"通道参数4"设置传送原因类型fu 2005.9.1
		else
		   lpIEC101->TypeSeReason = 1;
		if(pChannelPara[ChanNo].wChanPara5==2)
		   lpIEC101->TypeCmmAdd = pChannelPara[ChanNo].wChanPara5; //由"通道参数5"设置应用服务单元地址类型fu 2005.9.1
		else
		   lpIEC101->TypeCmmAdd = 1;
		if( pChannelPara[ChanNo].wChanPara6==3 ||  pChannelPara[ChanNo].wChanPara6 == 1)  //第三个字节仅用于结构化信息对象地址，在所有情况下不同信息对象地址的最大数目为65536（即两个八位位组）
		   lpIEC101->TypeInfAdd = pChannelPara[ChanNo].wChanPara6; //由"通道参数6"设置信息体地址类型fu 2005.9.1
		else
		   lpIEC101->TypeInfAdd = 2;
	}
	lpIEC101->wLinkAdd.Word =pChannelPara[ChanNo].wChanPara0;	//由"通道参数0"设置站地址
	lpIEC101->wCmmAdd.Word = lpIEC101->wLinkAdd.Word;

	if(pChannelPara[ChanNo].wChanPara7 > 0)
	       lpIEC101->PC56Time2a = TRUE;
	else
	       lpIEC101->PC56Time2a = FALSE;

	lpIEC101->initstatus = notinit;
	lpIEC101->haveset = FALSE;
	lpIEC101->PfcbC = 0;
	lpIEC101->OrgnizeFrame = 0 ;
	lpIEC101->Pfcv = 0;
	lpIEC101->Pacd = 0;
	lpIEC101->Pdfc = 0;
	lpIEC101->PRecvFrame.byFull = 0;
	lpIEC101->PReAppLayer.byFull = 0;
	lpIEC101->PSeAppLayer.byFull = 0;
	lpIEC101->PSendFrame.byFull = 0;
	lpIEC101->PSeAppLayer.lpByBuf = lpIEC101->PSendFrame.byLinkBuf + 5 + lpIEC101->TypeLinkAdd;
	lpIEC101->PReFrameType = 0;
	lpIEC101->PReMsgType = 0;
	lpIEC101->dwReadAd.Dword = 0;	//fulianqiang 2005.9.2
	lpIEC101->WaitYkXzRet = 0;
	lpIEC101->WaitYkZxRet = 0;
	lpIEC101->SendYkZxAck = 0;
	lpIEC101->byYKWaitTimes=0;	//遥控返校等待次数清零
	lpIEC101->nStartYcOver = 0;
	for (i=0;i<YCSendTabSize;i++)
	{
		lpIEC101->nLastYcVal[i]=GetYc(i);
	}
   	if (pChannelPara[ChanNo].wChanPara1)	//遥测越限千分比
	{
		if (pChannelPara[ChanNo].wChanPara1 < 1000)
			lpIEC101->byRange = (u8)pChannelPara[ChanNo].wChanPara1;
		else
			lpIEC101->byRange = 1;
	}
	else
	{ 
		lpIEC101->byRange = 1;
	} 
	//实际发送量初始化
	if (pChannelPara[ChanNo].wSendYCNum<=YCSendTabSize)	//判断参数有效性
		lpIEC101->SendYcN=pChannelPara[ChanNo].wSendYCNum;//初始化发送遥测数
	else
		lpIEC101->SendYcN=YCSendTabSize;
	if (pChannelPara[ChanNo].wSendYXNum<=YXSendTabSize)
		lpIEC101->SendYxN=pChannelPara[ChanNo].wSendYXNum;//初始化发送遥信数
	else
		lpIEC101->SendYxN=YXSendTabSize;
	if (pChannelPara[ChanNo].wSendYMNum<=YMSendTabSize)
		lpIEC101->SendDdN=pChannelPara[ChanNo].wSendYMNum;//初始化发送电度数
	else
		lpIEC101->SendDdN=YMSendTabSize;
	lpIEC101->SendYkN=IEC101_YKNUM;
	//初始化需要发送的遥测帧数,不足一帧的按一帧处理
	lpIEC101->YcFN=(u8)((lpIEC101->SendYcN+IEC101_YCNPF-1)/IEC101_YCNPF);
	//初始化需要发送的遥信帧数,不足一帧的按一帧处理
	lpIEC101->YxFN=(u8)((lpIEC101->SendYxN+IEC101_YXNPF-1)/IEC101_YXNPF);
	//初始化需要发送的电度帧数,不足一帧的按一帧处理
	lpIEC101->DdFN=(u8)((lpIEC101->SendDdN+IEC101_DDNPF-1)/IEC101_DDNPF);
	for(i=0;i<IEC101_YCNUM/IEC101_YCNPF;i++)
		lpIEC101->YcNPF[i]=0;
	for(i=0;i<IEC101_YXNUM/IEC101_YXNPF;i++)
		lpIEC101->YxNPF[i]=0;
	for(i=0;i<IEC101_DDNUM/IEC101_DDNPF;i++)
		lpIEC101->DdNPF[i]=0;
	//初始化每帧遥测数,最后一帧遥测数为实际需要发送的遥测数
	n=lpIEC101->YcFN;
	for(i=1;i<=n;i++)
		lpIEC101->YcNPF[i-1]=IEC101_YCNPF;
	if ((n>0)&&(lpIEC101->SendYcN%IEC101_YCNPF!=0))
		lpIEC101->YcNPF[n-1]=lpIEC101->SendYcN%IEC101_YCNPF;
	//初始化每帧遥信数,最后一帧遥信数为实际需要发送的遥信数
	n=lpIEC101->YxFN;
	for (i=1;i<=n;i++)
		lpIEC101->YxNPF[i-1]=IEC101_YXNPF;
	if ((n>0)&&(lpIEC101->SendYxN%IEC101_YXNPF!=0))
		lpIEC101->YxNPF[n-1]=lpIEC101->SendYxN%IEC101_YXNPF;
	//初始化每帧电度数,最后一帧电度数为实际需要发送的电度数
	n=lpIEC101->DdFN;
	for (i=1;i<=n;i++)
		lpIEC101->DdNPF[i-1]=IEC101_DDNPF;
	if ((n>0)&&(lpIEC101->SendDdN%IEC101_DDNPF!=0))
		lpIEC101->DdNPF[n-1]=lpIEC101->SendDdN%IEC101_DDNPF;
    //新增初始化量
     //	if(pChannelPara[ChanNo].wChanPara8==0)	      //主动上送时间间隔
     //		lpIEC101->UnsolTimeInterval=10;
     //	else
    //	        lpIEC101->UnsolTimeInterval = pChannelPara[ChanNo].wChanPara8; 
       lpIEC101->UnsolTimeInterval = pChannelPara[ChanNo].wChanPara8; //为3时采用国际召唤方式，其他为国内老方式
#endif      
}
//初始化IEC101规约描述结构
void InitIEC101Prot(void)
{
    int i;
#if 0    
    lpIEC101->TypeLinkAdd=1;
    lpIEC101->TypeCmmAdd=1;
    lpIEC101->TypeInfAdd=1;
    lpIEC101->TypeSeReason=1;
#else
    lpIEC101->TypeCmmAdd=2;
    lpIEC101->TypeLinkAdd=2;
    lpIEC101->TypeInfAdd=2;
    lpIEC101->TypeSeReason=2;
#endif    
    lpIEC101->wLinkAdd.Word=1;
    lpIEC101->initstatus = notinit;
     lpIEC101->haveset = FALSE;
    lpIEC101->FlagPingH = 1;
    lpIEC101->UnsolTimeInterval=3;
    lpIEC101->firstData = nofirstdata;
   
    
    lpIEC101->TypeProtocol=1;
    lpIEC101->PSeAppLayer.lpByBuf = lpIEC101->PSendFrame.byLinkBuf + 5 + lpIEC101->TypeLinkAdd;
    lpIEC101->YcFN = 8;
    for(i=0;i<20;++i)
    {
      lpIEC101->YcNPF[i]=23;//23;
    }
    lpIEC101->DdFN = 16;
    for(i=0;i<20;++i)
    {
      lpIEC101->DdNPF[i]=8;
    }
#if 0 
	ProtocolDisp[IEC101ProtNo].ProtocolType=IEC101ProtNo;	//规约类型
	ProtocolDisp[IEC101ProtNo].RecvBuffLen=RECEBUFSIZE; 	//接收缓冲区长度
	ProtocolDisp[IEC101ProtNo].TranBuffLen=TRANSBUFSIZE;	//发送缓冲区长度
	ProtocolDisp[IEC101ProtNo].SyncByte[0]=0xFF;	 		//同步字
	ProtocolDisp[IEC101ProtNo].SyncByte[1]=0x68;	 		//同步字
	ProtocolDisp[IEC101ProtNo].WordWidth=1;		 			//字宽度
	ProtocolDisp[IEC101ProtNo].FrameProcess=IEC101Process;	//帧处理函数指针
	ProtocolDisp[IEC101ProtNo].InitProcess=InitIEC101Func; 	//初始化处理函数指针
#endif        
}

void SetACD(void)
{
  lpIEC101->Pacd = 1; //zzl test
}
