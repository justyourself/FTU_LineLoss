#include "ht6xxx_lib.h"
#include "General.h"
#include "TypeE2P.h"
#include "TypeMeter.h"
#include "TypeRAM.h"
#include "Mem.h"

#include "Data.h"
#include "MeterDataType.h"
#include "Initial.h"
#include "GetMeterData.h"

//* RAM_Dest :读取数据返回RAM指针
//SYN_Src: 读取数据ID号
//Lenth ：读取数据长度
//返回0 :同步读写正常
//返回1: 同步读写错误
int SYNData_Read( unsigned char* RAM_Dest,unsigned int SYN_Src, int Lenth )
{
	short i;
	unsigned char* ptr;	
	unsigned char CheckNum;
	
	for(i=0;i<3;i++)
	{
		HT_FreeDog();

		Comm8213.CommDly=RD8213DLY;//通信等待时间//
		Comm8213.SendBuf8213[0]= 0xA6;//将要发送的数据起始位//
		Comm8213.SendBuf8213[1]=F_DRS_ReadEN;//读写控制字//
		Comm8213.SendBuf8213[2]= SYN_Src&0x00FF;	//读写地址
		Comm8213.SendBuf8213[3]= SYN_Src>>8;
		Comm8213.SendBuf8213[4]= 0x00;	//数据长度
		Comm8213.SendBuf8213[5]= ChkNum(Comm8213.SendBuf8213,5);
		Comm8213.SendBuf8213[6]= 0xAB;	//通讯结束字
		Comm8213.NeedRecCnt= Lenth+7;//需要接收的数据长度//
		Comm8213.NeedSendCnt=6;//需要发送的数据长度(除去发的第一字节)//
		Comm8213.CommOk=0;//通信完成标志清0//
		Comm8213.Rx_Ptr=0;

	//	RAM_Fill(Comm8213.RecBuf8213,100);

		HT_UART5->UARTCON &=~ (UART_UARTCON_RXIE + UART_UARTCON_RXEN);
		HT_UART5->SBUF= Comm8213.SendBuf8213[0];//发送一字节//
		HT_UART5->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
		while(Comm8213.CommDly>0)//当等待时间没完时一直等待//
		{
			if(Comm8213.CommOk == READ8213OK)
			{
				RAM_Write(&(Comm8213.SendBuf8213[0]),Comm8213.RecBuf8213,Lenth+7);
				CheckNum = ChkNum(Comm8213.RecBuf8213,Lenth+5);
				
				if(Comm8213.RecBuf8213[Lenth+5] == CheckNum)
				{	
				ptr = Comm8213.RecBuf8213;
					RAM_Write(RAM_Dest,ptr+5,Lenth);							
					return 0;
				}
			}
			HT_FreeDog();
		}
		
		Comm8213.Ptr8213=0;
	}
	
	return 1;
}
//主动

//* RAM_Dest :读取数据返回RAM指针
//SYN_Src: 读取数据ID号
//Lenth ：读取数据长度
//返回0 :同步读写正常
//返回1: 同步读写错误
int SYNData_Write( unsigned int SYN_Dest, unsigned char* RAM_Src, int Lenth)
{
	short i;
	unsigned char CheckNum;
	
	for(i=0;i<3;i++)
	{
		HT_FreeDog();

		Comm8213.CommDly=20;//通信等待时间//
		Comm8213.SendBuf8213[0]= 0xA6;//将要发送的数据起始位//
		Comm8213.SendBuf8213[1]=F_DRS_WriteEN;//读写控制字//
		Comm8213.SendBuf8213[2]= SYN_Dest&0xFF;	//读写地址
		Comm8213.SendBuf8213[3]= SYN_Dest>>8;
		Comm8213.SendBuf8213[4]= Lenth;	//数据长度
		RAM_Write(Comm8213.SendBuf8213+5,RAM_Src,Lenth);
		Comm8213.SendBuf8213[5+Lenth]= ChkNum(Comm8213.SendBuf8213,5+Lenth);
		Comm8213.SendBuf8213[6+Lenth]= 0xAB;	//通讯结束字
		Comm8213.NeedRecCnt= 1;//需要接收的数据长度//
		Comm8213.NeedSendCnt=6+Lenth;//需要发送的数据长度(除去发的第一字节)//
		Comm8213.CommOk=0;//通信完成标志清0//
		Comm8213.Tx_Ptr=0;

		HT_UART5->UARTCON &=~ (UART_UARTCON_RXIE + UART_UARTCON_RXEN);		
		HT_UART5->SBUF= Comm8213.SendBuf8213[0];//发送一字节//
		HT_UART5->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);

		while(Comm8213.CommDly>0)//当等待时间没完时一直等待//
		{
			if(Comm8213.CommOk == READ8213OK)
			{
				if(Comm8213.RecBuf8213[0] == 0x06)
				{	
					return 0;
				}
			}

			HT_FreeDog();
		}		
	}
	
	return 1;
}
void SynData500ms (void)
{
		
}
//主动
void SynData1s (void)
{
	unsigned char Buff[30]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,
					0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,};
	unsigned char stater;
	
//	stater =SYNData_Read( Buff,1, 6 );
	stater =SYNData_Write( 0x1001,Buff, 30 );
	if(stater == 0)
		stater = 1;
	else
		stater = 0;	
}

////被动
//int SYNDataDown_Read( unsigned char* RAM_Dest,unsigned int SYN_Src, int Lenth )
//{
//	
//	
//	return 0;
//}
////被动
//int SYNDataDown_Write( unsigned int SYN_Dest, unsigned char* RAM_Src, int Lenth)
//{
//	
//	return 0;
//}

//* RAM_Dest :读取数据返回RAM指针
//ID_Src: 读取数据ID号
//Lenth ：读取数据长度

//返回0 :读写正常
//返回1:读写错误
int ID_Read( unsigned char* RAM_Dest,unsigned int ID_Src, int Lenth )
{
	unsigned int 	i;
	unsigned int	DataID;			//ID码
	unsigned long 	DataAdd;		//地址
	unsigned char 	DataLen;		//数据长度
	unsigned char 	DataRWENStorage;	//数据读写权限和存储类型
	
	for( i=0;i<MeterDataTabNum;i++)
	{
		if( MeterDataTab[i].DataID == ID_Src )
		{
			DataAdd = MeterDataTab[i].DataAdd;
			DataLen = MeterDataTab[i].DataLen;	
			DataRWENStorage = MeterDataTab[i].DataRWENStorage;
			break;
		}				
	}
	
	if((DataRWENStorage & F_DRS_ReadEN) == 0)
		return 1;
	if((DataRWENStorage & F_DRS_SYNData) == F_DRS_SYNData)	
	{
		if(SYNData_Read(RAM_Dest, ID_Src, DataLen ) == 0)
			return 0;
		else if(SYNData_Read(RAM_Dest, ID_Src, DataLen ) == 0)
			return 0;
		else return 1;	
	}	
	switch( DataRWENStorage & 0x1F )
	{	
		case DRS_Energy:
			Read_CMonEC( RAM_Dest, DataAdd );
			break;
		case DRS_instantaneous:
			
			break;
		case DRS_Eeprom1:
			if(E2P_RAdj( RAM_Dest,DataAdd,  DataLen ) != 0)
				return 1;
			break;
		case DRS_Eeprom2:
			if(E2P_RData( RAM_Dest,DataAdd,  DataLen ) != 0)
				return 1;
			break;
		case DRS_Eeprom3:
	//		if(E2P_RECData( RAM_Dest,DataAdd,  DataLen ) != 0)
	//			return 1;
			break;
		case DRS_Flash:
			
			break;
		case DRS_Ram:
			RAM_Write( RAM_Dest,(unsigned char*)DataAdd,DataLen);
			break;	
		case DRS_ferroelectric:
			
			break;
		case DRS_TimeIC:
		//	fnSysClock_Read(RAM_Dest);
			break;			
		default:
			break;		
	}
	return 0;
}

//* RAM_Dest :读取数据返回RAM指针
//ID_Src: 读取数据ID号
//Lenth ：读取数据长度
//返回0 :读写正常
//返回1:读写错误
int ID_Write( unsigned int ID_Dest, unsigned char* RAM_Src, int Lenth)
{
	unsigned int 	i;
	unsigned int	DataID;			//ID码
	unsigned long 	DataAdd;		//地址
	unsigned char 	DataLen;		//数据长度
	unsigned char 	DataRWENStorage;	//数据读写权限和存储类型
	
	for( i=0;i<MeterDataTabNum;i++)
	{
		if( MeterDataTab[i].DataID == ID_Dest )
		{
			DataAdd = MeterDataTab[i].DataAdd;
			DataLen = MeterDataTab[i].DataLen;	
			DataRWENStorage = MeterDataTab[i].DataRWENStorage;
			break;
		}				
	}
	
	if((DataRWENStorage & F_DRS_WriteEN) == 0)
		return 1;
	if((DataRWENStorage & F_DRS_SYNData) == F_DRS_SYNData)	
	{
		if(SYNData_Write(ID_Dest, RAM_Src,DataLen ) == 0)
			return 0;
		else if(SYNData_Write(ID_Dest,RAM_Src,  DataLen ) == 0)
			return 0;
		else return 1;	
	}	
	switch( DataRWENStorage & 0x1F )
	{	
		case DRS_Energy:
//			Read_CMonEC( RAM_Dest, DataAdd );
			break;
		case DRS_instantaneous:
			
			break;
		case DRS_Eeprom1:
			if(E2P_WAdj( DataAdd,RAM_Src,  DataLen ) != 0)
				return 1;
			break;
		case DRS_Eeprom2:
			if(E2P_WData( DataAdd,RAM_Src,  DataLen ) != 0)
				return 1;
			break;
		case DRS_Eeprom3:
		//	if(E2P_WECData( DataAdd, RAM_Src, DataLen ) != 0)
		//		return 1;
			break;
		case DRS_Flash:
			
			break;
		case DRS_Ram:
			RAM_Write( (unsigned char*)DataAdd,RAM_Src,DataLen);
			break;	
		case DRS_ferroelectric:
			
			break;
		case DRS_TimeIC:
		//	fnSysClock_Write(&Clk.Sec[0]);
			break;			
		default:
			break;		
	}
	return 0;
	
}
unsigned short GetRealDataReadTabItemNo( unsigned short cmd )
{
	unsigned short i;
	unsigned char* ptr;
		
	for( i=0;i<MeterRealTabNum;i++ )
	{
		if( cmd== Double_RealTab[i].DataID) break;				//查找对应的OBIS码
	}	
	if(i==MeterRealTabNum) return 1;
	
	ptr = Comm8213.RecBuf8213;
	RAM_Write(Double_RealTab[i].ECRAds,ptr+5,Double_RealTab[i].len);
	return 0;	
}
unsigned short GetECDataReadTabItemNo( unsigned short cmd )
{
	unsigned short i;
	unsigned char* ptr;
		
	for( i=0;i<MeterECTabNum;i++ )
	{
		if( cmd== Double_ECTab[i].DataID) break;				//查找对应的OBIS码
	}	
	if(i==MeterECTabNum) return 1;
	
	ptr = Comm8213.RecBuf8213;
	RAM_Write(Double_ECTab[i].ECRAds,ptr+5,Double_ECTab[i].len);
	return 0;	
}
unsigned short GetDemandDataReadTabItemNo( unsigned short cmd )
{
	unsigned short i;
	unsigned char* ptr;
		
	for( i=0;i<MeterDemandTabNum;i++ )
	{
		if( cmd== Double_DemandTab[i].DataID) break;				//查找对应的OBIS码
	}	
	if(i==MeterDemandTabNum) return 1;
	
	ptr = Comm8213.RecBuf8213;
	RAM_Write(Double_DemandTab[i].ECRAds,ptr+5,Double_DemandTab[i].len);
	return 0;	
}

unsigned char RealDataRead(void)
{
	unsigned char Error=0;
	if(Double_ComPtr.Real_Ptr>=MeterRealTabNum) return 1;
	Double_ComPtr.Cmd = Double_RealTab[Double_ComPtr.Real_Ptr].DataID;
	Double_ComPtr.length = Double_RealTab[Double_ComPtr.Real_Ptr].len; 
		Double_ComPtr.Real_Ptr++;
	if(Double_ComPtr.Real_Ptr>=MeterRealTabNum) 
		Error=1;

	return Error;
}

unsigned char ECDataRead(void)
{
	unsigned char Error=0;
	if(Double_ComPtr.EC_Ptr>=MeterECTabNum)  return 1;
	Double_ComPtr.Cmd = Double_ECTab[Double_ComPtr.EC_Ptr].DataID;
	Double_ComPtr.length = Double_ECTab[Double_ComPtr.EC_Ptr].len; 
		Double_ComPtr.EC_Ptr++;
	if(Double_ComPtr.EC_Ptr>=(MeterECTabNum+(Total_Fee+1)+SM.FeeNo)) 
		Error=1;
	else if(Double_ComPtr.EC_Ptr>=(MeterECTabNum+(Total_Fee+1)))
		Double_ComPtr.EC_Ptr = SM.FeeNo;

	return Error;
}

unsigned char DemandDataRead(void)
{
	unsigned char Error=0;
	if(Double_ComPtr.Demand_Ptr>=MeterDemandTabNum)   return 1;
	Double_ComPtr.Cmd= Double_DemandTab[Double_ComPtr.Demand_Ptr].DataID;
	Double_ComPtr.length= Double_DemandTab[Double_ComPtr.Demand_Ptr].len; 
		Double_ComPtr.Demand_Ptr+=(Total_Fee+1);
			
	if(Double_ComPtr.Demand_Ptr>=(MeterDemandTabNum+(Total_Fee+1)+SM.FeeNo)) 
		Error=1;
	else if(Double_ComPtr.Demand_Ptr>=(MeterDemandTabNum+(Total_Fee+1)))
		Double_ComPtr.Demand_Ptr = SM.FeeNo;

	return Error;
}
int SYNData_Read2( unsigned int SYN_Src, unsigned char Lenth )
{
	if((Comm8213.CommOk&WRITE8213OK)==WRITE8213OK) return 1;

		//Comm8213.CommDly=RD8213DLY;//通信等待时间//
		Comm8213.SendBuf8213[0]= 0xA6;//将要发送的数据起始位//
		Comm8213.SendBuf8213[1]=F_DRS_ReadEN;//读写控制字//
		Comm8213.SendBuf8213[2]= SYN_Src&0x00FF;	//读写地址
		Comm8213.SendBuf8213[3]= SYN_Src>>8;
		Comm8213.SendBuf8213[4]= 0x00;	//数据长度
		Comm8213.SendBuf8213[5]= ChkNum(Comm8213.SendBuf8213,5);
		Comm8213.SendBuf8213[6]= 0xAB;	//通讯结束字
		Comm8213.NeedRecCnt= Lenth+7;//需要接收的数据长度//
		Comm8213.NeedSendCnt=6;//需要发送的数据长度(除去发的第一字节)//
		Comm8213.CommOk|=READ8213OK;//通信完成标志清0//
		Comm8213.CommOk&=~WRITE8213OK;
		Comm8213.Rx_Ptr=0;

	//	RAM_Fill(Comm8213.RecBuf8213,100);

		HT_UART5->UARTCON &=~ (UART_UARTCON_RXIE + UART_UARTCON_RXEN);
		HT_UART5->SBUF= Comm8213.SendBuf8213[0];//发送一字节//
		HT_UART5->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
	return 0;
}
int SYNData_Write2( unsigned int SYN_Dest, unsigned char* RAM_Src, int Lenth)
	{
		short i;
		unsigned char CheckNum;
		
		for(i=0;i<3;i++)
		{
			HT_FreeDog();
	
			Comm8213.CommDly=20;//通信等待时间//
			Comm8213.SendBuf8213[0]= 0xA6;//将要发送的数据起始位//
			Comm8213.SendBuf8213[1]=F_DRS_WriteEN;//读写控制字//
			Comm8213.SendBuf8213[2]= SYN_Dest&0xFF; //读写地址
			Comm8213.SendBuf8213[3]= SYN_Dest>>8;
			Comm8213.SendBuf8213[4]= Lenth; //数据长度
			RAM_Write(Comm8213.SendBuf8213+5,RAM_Src,Lenth);
			Comm8213.SendBuf8213[5+Lenth]= ChkNum(Comm8213.SendBuf8213,5+Lenth);
			Comm8213.SendBuf8213[6+Lenth]= 0xAB;	//通讯结束字
			Comm8213.NeedRecCnt= 1;//需要接收的数据长度//
			Comm8213.NeedSendCnt=6+Lenth;//需要发送的数据长度(除去发的第一字节)//
			Comm8213.CommOk|=WRITE8213OK;//通信完成标志清0//
			Comm8213.Tx_Ptr=0;
	
			HT_UART5->UARTCON &=~ (UART_UARTCON_RXIE + UART_UARTCON_RXEN);		
			HT_UART5->SBUF= Comm8213.SendBuf8213[0];//发送一字节//
			HT_UART5->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
	
			while(Comm8213.CommDly>0)//当等待时间没完时一直等待//
			{
					HT_FreeDog();
				}		
				if(Comm8213.CommOk == Comm8213OK)
				{
					if(Comm8213.RecBuf8213[0] == 0x06)
					{	
					Comm8213.CommOk&=~WRITE8213OK;
						return 0;
					}
				}
	
		}
		
		return 1;
	}


int ID_Read2(void)
{	
	if(Double_ComPtr.Comm_Enable==DISABLE) return 1;
	switch(Double_ComPtr.Data_Select)
	{//瞬时量 电量 需量值一次更新，瞬时量及电量每秒刷新一次，需量一分钟刷新一次
	case 1:
		if(RealDataRead()==1)	
			Double_ComPtr.Data_Select=2;

		break;
	case 2:
		if(ECDataRead()==1)
			Double_ComPtr.Data_Select=3;
		break;
	case 3:
		if(Clk.Sec[0]==0)
			{
			if(DemandDataRead()==1)
				{
				Double_ComPtr.Data_Select=1;
				Double_ComPtr.Real_Ptr=0;
				Double_ComPtr.EC_Ptr=0;
				Double_ComPtr.Demand_Ptr=0;
				Double_ComPtr.Comm_Enable=DISABLE;
				Double_ComPtr.Data_Select=1;
				}
			}
		else
			{
			Double_ComPtr.Real_Ptr=0;
			Double_ComPtr.EC_Ptr=0;
			Double_ComPtr.Demand_Ptr=0;
			Double_ComPtr.Comm_Enable=DISABLE;
			Double_ComPtr.Data_Select=1;
			}
		break;
	default:
		Double_ComPtr.Data_Select=1;
		break;
	}

	SYNData_Read2(Double_ComPtr.Cmd,Double_ComPtr.length);
	return 0;
}
unsigned char SYNData_Get(void)
{
	unsigned char Checksum,i;
	unsigned char Error_Byte=0;
	unsigned short cmd;

	Checksum = ChkNum(Comm8213.RecBuf8213,Double_ComPtr.length+5);
	
	if(Comm8213.RecBuf8213[Double_ComPtr.length+5] == Checksum)
	{	
	
	//Comm8213.CommOk |= Comm8213OK;
	Comm8213.CommOk &=~ READ8213OK;
	cmd = Comm8213.RecBuf8213[3];
	cmd<<=8;
	cmd+=Comm8213.RecBuf8213[2];
	if(GetRealDataReadTabItemNo(cmd)==0)   return Error_Byte;
	if(GetECDataReadTabItemNo(cmd)==0)     return Error_Byte;
	if(GetDemandDataReadTabItemNo(cmd)==0) return Error_Byte;
	}

	return Error_Byte;
}

