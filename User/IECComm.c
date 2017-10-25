#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
//#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "IECComm.h"

//#include "RsComm.h"
#include "interrupt.h"
#include "Time.h"
#include "Power.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
//#include "Produce.h"
#include "ApplComm.h"
#include "COSEMComm.h"
//#include "AESGCM.H"

// 可打印字符串转换为字节数据
// 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
// pSrc: 源字符串指针
// pDst: 目标数据指针
// nSrcLength: 源字符串长度
// 返回: 目标数据长度
//short gsmString2Bytes(const char* pSrc, unsigned char* pDst, short nSrcLength)
short ASC_BCD(unsigned char* pDst, unsigned char* pSrc, short nSrcLength)
{
	short i;
	
	if( nSrcLength == 0 ) return 0;
	pDst += nSrcLength - 1;
	
//	for( i=0; i<nSrcLength; i+=2 )
	for( i=0; i<nSrcLength; i++ )
	{
		// 输出高4位
		if(*pSrc>='0' && *pSrc<='9')
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		// 输出低4位
		if(*pSrc>='0' && *pSrc<='9')
		{
			*pDst |= *pSrc - '0';
		}
		else
		{
			*pDst |= *pSrc - 'A' + 10;
		}

		pSrc++;
//		pDst++;
		pDst--;
	}

	// 返回目标数据长度
	return nSrcLength / 2;
}

// 字节数据转换为可打印字符串
// 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
// pSrc: 源数据指针
// pDst: 目标字符串指针
// nSrcLength: 源数据长度
// 返回: 目标字符串长度
//short gsmBytes2String(const unsigned char* pSrc, char* pDst, short nSrcLength)
short BCD_ASC(unsigned char* pDst, unsigned char* pSrc, short nSrcLength)
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf的字符查找表
        short i;        

	pDst += nSrcLength*2 -1;

	for( i=0; i<nSrcLength; i++)
	{
		*pDst-- = tab[*pSrc & 0x0f];	// 输出高4位
//		*pDst++ = tab[*pSrc >> 4];		// 输出低4位
		*pDst-- = tab[*pSrc >> 4];		// 输出低4位
//		*pDst++ = tab[*pSrc & 0x0f];	// 输出高4位
		pSrc++;
	}

	// 输出字符串加个结束符
//	*pDst = '\0';

	// 返回目标字符串长度
	return nSrcLength * 2;
}

//short GetBandRate( unsigned char BandRate )
//{
//	short i;
//	
////	for( i=0;i<6;i++ )
//	for( i=0;i<7;i++ )
//	{
////		if(( BandRate & ByteBit[i+1] )!= 0 ) return i*3;		
//		if(( BandRate & ByteBit[i] )!= 0 ) return i*3;		
//	}			
//	return 2*3;
//}

short GetBandRate( unsigned char BandRate )
{
	unsigned short Temp;
	
	switch( BandRate )
	{
		case Baut300: Temp = 300; break; 	
		case Baut600: Temp = 600; break; 	
		case Baut1200: Temp = 1200; break; 	
		case Baut4800: Temp = 4800; break; 	
	//	case Baut7200: Temp = 7200; break; 	
		case Baut9600: Temp = 9600; break; 
		case Baut19200: Temp = 19200; break;
		case Baut38400: Temp = 38400; break;
		case Baut57600: Temp = 57600; break;
	//	case Baut115200: Temp = 115200; break;
		default: Temp = 57600; break; 	
	}	
	return Temp;
}


unsigned char GetTXDelay( unsigned char* Source )
{
	unsigned char Temp;
	unsigned char TXDelay;
	
	Temp = *Source;
	
	switch( Temp )
	{
		case Baut_300H:		TXDelay = 160;	break;	
		case Baut_600H:		TXDelay = 64;	break;	
		case Baut_1200H:	TXDelay = 32; 	break;	
		case Baut_2400H:	TXDelay = 16; 	break;	
		case Baut_4800H:	TXDelay = 8;	break;	
		case Baut_9600H:	TXDelay = 5;	break;	
		case Baut_19200H:	TXDelay = 4;	break;
	//	case Baut_57600H:	TXDelay = 500;	break;
		default: 			TXDelay = 7; 	break;				
	}		
	return TXDelay;
}	

short GetCOM_No( unsigned char* ComBuf )
{
	short COM_No=0;
	
	if( ComBuf == Comm.Buf0 ) COM_No = 0;
	if( ComBuf == Comm.Buf1 ) COM_No = 1;
	if( ComBuf == Comm.Buf2 ) COM_No = 2;
//	if( ComBuf == Comm.Buf3 ) COM_No = 3;

	return COM_No;
}		

unsigned char* GetComBuf( unsigned short COM_No )
{
	unsigned char* ComBuf=Comm.Buf0;
	
	if( COM_No == 0 ) ComBuf = Comm.Buf0;
	else if( COM_No == 1 ) ComBuf = Comm.Buf1;
	else if( COM_No == 2 ) ComBuf = Comm.Buf2;
//	else if( COM_No == 3 ) ComBuf = Comm.Buf3;

	return ComBuf;
}		

UART_InitTypeDef UART_InitStructure; 
void fnUSART_ZBCOM_Init(unsigned char Cfg)
{
	unsigned short Temp;
	Temp = GetBandRate( Cfg );					

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART0EN, ENABLE);		/*!< 使能UART0模块				*/
					
		/*!< UART0配置信息				 */
		UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART逻辑为正				*/
		UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1位停止位					*/
		UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8位数据位					*/
		UART_InitStructure.UART_Parity = UartParity_Disable;			/*!< 偶校验*/
		UART_InitStructure.UART_BaudRate = Temp;					/*!< 波特率9600 				*/
		UART_InitStructure.ReceiveEN = ENABLE;						/*!< 接收使能	注：非中断使能	*/
		//UART_InitStructure.SendEN = ENABLE;						/*!< 发送使能	注：非中断使能	*/		
		UART_InitStructure.SendEN = DISABLE;						/*!< 发送使能	注：非中断使能	*/		
		
		HT_UART_Init(HT_UART0, &UART_InitStructure);
		
		HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_RXIF);
		HT_UART_ITConfig(HT_UART0, UART_UARTCON_RXIE, ENABLE);

	return;
}

//===(红外)================================
void fnUSART_IRCom_Init(unsigned char Cfg)
{  
	unsigned short Temp;
	Temp = GetBandRate( Cfg );

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART1EN, ENABLE);		/*!< 使能UART1模块				*/
							  
	 /*!< UART1配置信息	,300,7,E,1		   */
	 UART_InitStructure.UART_Logic = UartLogicPositive;		  /*!< UART逻辑为正 			  */
	 UART_InitStructure.UART_StopBits = OneStopBits;			  /*!< 1位停止位				  */
	 UART_InitStructure.UART_WordLength = WordLength_8Bits ;	  /*!< 8位数据位				  */
	 UART_InitStructure.UART_Parity = UartParity_EVEN;		  /*!< 无校验 */
	 UART_InitStructure.UART_BaudRate = Temp;					  /*!< 红外初始波特率300		  */
	 UART_InitStructure.ReceiveEN = ENABLE;					  /*!< 接收使能   注：非中断使能  */
	 //UART_InitStructure.SendEN = ENABLE; 					  /*!< 发送使能   注：非中断使能  */	  
	 UART_InitStructure.SendEN = DISABLE;						  /*!< 发送使能   注：非中断使能  */	  
	 
	 HT_UART_Init(HT_UART1, &UART_InitStructure);
	 
	// HT_UART1->IRDUTY = 0x00000000;								  //占空比50%
	// HT_UART1->IRCON = 0x00000001;   //红外38k来源于PLL,红外调制正极性，使能TX红外调制
	 HT_UART_ClearITPendingBit(HT_UART1, UART_UARTSTA_RXIF);
	 HT_UART_ITConfig(HT_UART1, UART_UARTCON_RXIE, ENABLE);

	return;
}
void fnUART2_Init(unsigned char Cfg)
{
	unsigned short Temp;

	Temp = GetBandRate( Cfg );

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART2EN, ENABLE);		/*!< 使能UART2模块				*/
		//红外通讯初始波特率4800,7，e,1
		/*!< UART2配置信息	debugge			 */
		UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART逻辑为正				*/
		UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1位停止位					*/
		UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8位数据位					*/
		UART_InitStructure.UART_Parity = UartParity_EVEN;			/*!< 偶校验*/
		UART_InitStructure.UART_BaudRate = Temp;					/*!< 波特率  				    */
		UART_InitStructure.ReceiveEN = ENABLE;						/*!< 接收使能	注：非中断使能	*/
		UART_InitStructure.SendEN = ENABLE;						    /*!< 发送使能	注：非中断使能	*/		
		//UART_InitStructure.SendEN = DISABLE;						/*!< 发送使能	注：非中断使能	*/		
		
		HT_UART_Init(HT_UART2, &UART_InitStructure);
		HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_RXIF);
		HT_UART_ITConfig(HT_UART2, UART_UARTCON_RXIE, ENABLE);//接收中断使能
		HT_UART_ITConfig(HT_UART2, UART_UARTCON_TXIE, ENABLE);//发送中断使能

	return;
}

void fnUSART_RS485_1_Init(unsigned char Cfg)
{
	unsigned short Temp;
		Temp = GetBandRate( Cfg );					
	/***** 以下代码用于配置UART2通讯*****/
			HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART3_7816_1EN, ENABLE);		/*!< 使能UART3模块				*/
		
			/*!< UART2配置信息				 */
			UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART逻辑为正				*/
			UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1位停止位					*/
			UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8位数据位					*/
			UART_InitStructure.UART_Parity = UartParity_Disable;		/*!< 无校验 					*/
			UART_InitStructure.UART_BaudRate = Temp;					/*!< 波特率57600 				*/
			UART_InitStructure.ReceiveEN = ENABLE;						/*!< 接收使能	注：非中断使能	*/
			//UART_InitStructure.SendEN = ENABLE; 						/*!< 发送使能	注：非中断使能	*/	
			UART_InitStructure.SendEN = DISABLE;
			
			HT_UART_Init(HT_UART3, &UART_InitStructure);
			HT_UART_ClearITPendingBit(HT_UART3, UART_UARTSTA_RXIF);
			HT_UART_ITConfig(HT_UART3, UART_UARTCON_RXIE, ENABLE);

	return;
}
void fnUSART_RS485_2_Init(unsigned char Cfg)
{
	unsigned short Temp;
		Temp = GetBandRate( Cfg );					
	/***** 以下代码用于配置UART2通讯*****/
			HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART4_7816_0EN, ENABLE);		/*!< 使能UART4模块				*/
		
			/*!< UART2配置信息				 */
			UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART逻辑为正				*/
			UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1位停止位					*/
			UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8位数据位					*/
			UART_InitStructure.UART_Parity = UartParity_Disable;		/*!< 无校验 					*/
			UART_InitStructure.UART_BaudRate = Temp;					/*!< 波特率57600 				*/
			UART_InitStructure.ReceiveEN = ENABLE;						/*!< 接收使能	注：非中断使能	*/
			//UART_InitStructure.SendEN = ENABLE; 						/*!< 发送使能	注：非中断使能	*/	
			UART_InitStructure.SendEN = DISABLE;
			
			HT_UART_Init(HT_UART4, &UART_InitStructure);
			HT_UART_ClearITPendingBit(HT_UART4, UART_UARTSTA_RXIF);
			HT_UART_ITConfig(HT_UART4, UART_UARTCON_RXIE, ENABLE);

	return;
}

void fnUART5_Init(unsigned char Cfg)
{
	unsigned short Temp;

	Temp = GetBandRate( Cfg );

	HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_UART5EN, ENABLE);		/*!< 使能UART5模块				*/
		//红外通讯初始波特率9600,7，e,1
		/*!< UART3配置信息				 */
		UART_InitStructure.UART_Logic = UartLogicPositive;			/*!< UART逻辑为正				*/
		UART_InitStructure.UART_StopBits = OneStopBits; 			/*!< 1位停止位					*/
		UART_InitStructure.UART_WordLength = WordLength_8Bits;		/*!< 8位数据位					*/
		UART_InitStructure.UART_Parity = UartParity_EVEN;			/*!< 偶校验*/
		UART_InitStructure.UART_BaudRate = Temp;					/*!< 波特率  				    */
		UART_InitStructure.ReceiveEN = ENABLE;						/*!< 接收使能	注：非中断使能	*/
		UART_InitStructure.SendEN = ENABLE;						    /*!< 发送使能	注：非中断使能	*/		
		//UART_InitStructure.SendEN = DISABLE;						/*!< 发送使能	注：非中断使能	*/		
		
		HT_UART_Init(HT_UART5, &UART_InitStructure);
		HT_UART_ClearITPendingBit(HT_UART5, UART_UARTSTA_RXIF);
		HT_UART_ITConfig(HT_UART5, UART_UARTCON_RXIE, ENABLE);//接收中断使能
		HT_UART_ITConfig(HT_UART5, UART_UARTCON_TXIE, ENABLE);//发送中断使能

	return;
}

short GetXorCheck( unsigned char* ComBuf, unsigned char* XorCheck, short* CheckAds)
{
	short	i,j;
	*XorCheck = 0;
	for( i=0;i<RRx_MaxSize;i++)
	{
		if(( *(ComBuf+i) == SOH ) || ( *(ComBuf+i) == STX )) 
		break;
	}
	for( j=i+1;j<RRx_MaxSize;j++ )
	{
		*XorCheck ^= *(ComBuf+j); 
		if(( *(ComBuf+j) == ETX ) || ( *(ComBuf+j) == EOT ))
		{
			*CheckAds = j + 1;
			return 0;	
		}	
	}	
	return -1;
}	

void DLMS_CommInit(unsigned char COM_No)
{
	switch( COM_No )
	{
		case 0:
			if( Comm.TX_Delay0 == 0 ) 
			{	
				Comm.Mode0 = 1;
				Comm.Ptr0 = 0;
			//	GPIO->PB |= P_SW4851;
			//	UART0->CTRL |=0x0c; 	//允许接收中断,允许接收错误中断
			//	UART0->STA = 0x3d;		//清接收中断标志及接收错误标志
			
				HT_UART0->UARTCON |= UART_UARTCON_RXIE + UART_UARTCON_RXEN;//允许接收中断	
				HT_UART0->UARTSTA &=~ (UART_UARTSTA_RXIF + UART_UARTSTA_TXIF);
				//HT_UART_ClearITPendingBit(HT_UART0, UART_UARTSTA_RXIF);
				Comm.Run0 &= ~F_ComOK;
			}
			break;
		case 1:
			if( Comm.TX_Delay1 == 0 ) 
			{
                          Comm.Ptr1 = 0;
                          HT_UART1->UARTCON |= UART_UARTCON_RXIE + UART_UARTCON_RXEN;//允许接收中断   
			  HT_UART1->UARTSTA &=~ (UART_UARTSTA_RXIF + UART_UARTSTA_TXIF);
                          Comm.Run1 &= ~F_ComOK;
			}
			break;	
		case 2:
			if( Comm.TX_Delay2 == 0 ) 
			{	
				Comm.Mode2 = 1;
				Comm.Ptr2 = 0;
			//	HT_GPIOE->PTSET |= GPIOE_Disp_CTL;
				//UART3->CTRL |=0x0c; 	//允许接收中断,允许接收错误中断
				//UART3->STA = 0x3d;		//清接收中断标志及接收错误标志
				
				HT_UART3->UARTCON |= UART_UARTCON_RXIE + UART_UARTCON_RXEN;//允许接收中断	
				HT_UART3->UARTSTA &=~ (UART_UARTSTA_RXIF + UART_UARTSTA_TXIF);
			//	HT_UART_ClearITPendingBit(HT_UART2, UART_UARTSTA_RXIF);
				Comm.Run2 &= ~F_ComOK;
			}

			break;			
		default: break;		
	}
}


void DLMS_CommMode( short COM_No )
{
  	unsigned char* ComMode;
  	unsigned short* ComPtr;
  	unsigned short* ComBTime;
  	unsigned char* ComRun;
	
  	ComMode = CommPara[COM_No].Mode;
  	ComPtr = CommPara[COM_No].Ptr;
  	ComBTime = CommPara[COM_No].BTime;
  	ComRun = CommPara[COM_No].Run;
	
	switch( *ComMode )
	{
		case 0: 
			DLMS_CommInit( COM_No ); 
			break;	
		case 1: if( *ComPtr == 0 ) break;
				if(( *ComRun & F_ComOK ) == 0 )
				{
					if( *ComBTime == 0 ) 
					{	
						DLMS_CommInit( COM_No );	
					}
				}	
				else 
					{

					}
				break;
		case 2: if(( *ComRun & F_ComOK ) != 0 )
				{
					DLMS_CommInit( COM_No );
				}					
		default: break;				
	}	
	
}

void CommRest( short COM_No )  		//恢复至初始状态
{	
	switch( COM_No )
	{
		case 0:												//载波
 			fnUSART_ZBCOM_Init(Baut57600);
			Comm.IECState0 = IECModeHDLC;
			Comm.MACState0 = NDMState;
			Comm.LLCState0 = LLCIdle;
			Comm.LastRecCommand0 = 0x00;
			HComm.APPLState0 = APPLIdle;					// 通讯口1APPL连接状态
			Comm.TX_Delay0 = 0;				
			Comm.BTime0 = 0;								// 字符间定时
			Comm.RecTmr0 = 0;
			Comm.Run0 &= ~F_ComOK;
			break;	
		case 1:												//			
			fnUSART_IRCom_Init(Baut9600);
			Comm.IECState1 = IECIdle;								//mode E
			Comm.MACState1 = NDMState;
			Comm.LLCState1 = LLCIdle;
			Comm.LastRecCommand1 = 0x00;
			HComm.APPLState1 = APPLIdle;					// 通讯口1APPL连接状态
			Comm.TX_Delay1 = 0;				
			Comm.BTime1 = 0;								// 字符间定时
			Comm.RecTmr1 = 0;
			Comm.Run1 &= ~F_ComOK;

			break;	
		case 2:												//uart3,485通道1
			fnUSART_RS485_1_Init(Baut57600);
			Comm.IECState2 = IECModeHDLC;
			Comm.MACState2 = NDMState;
			Comm.LLCState2 = LLCIdle;
			Comm.LastRecCommand2 = 0x00;
			HComm.APPLState2 = APPLIdle;					// 通讯口1APPL连接状态
			Comm.TX_Delay2 = 0;				
			Comm.BTime2 = 0;								// 字符间定时
			Comm.RecTmr2 = 0;
			Comm.Run2 &= ~F_ComOK;
			HT_GPIOE->PTSET |= SW485_1;
			break;
		default: break;
	}
	//CommInit( COM_No );
	DLMS_CommInit( COM_No );
}	

unsigned short MP_FCS(unsigned short fcs,unsigned short c)
{
	unsigned short FS1,FS2,FS3,FS;

	FS1=(fcs)^(c);
	FS2=FS1&0x00FF;
	FS3=MP_Fcstab[FS2];
	FS=(fcs>>8)^FS3;

	return FS;
}

unsigned short GetCheckCode_16BitFcs(unsigned char *pucData,unsigned short usDataLen)
{
	unsigned short crc=0;

//	crc = FCS_START;
	crc = 0xffff;
	while( usDataLen-- > 0 )
		crc = MP_FCS(crc,*pucData++);
	crc ^= 0xFFFF;

	return crc;
}

//unsigned short GetFrameLength( unsigned char* ComBuf )
//{
//	unsigned short Len;
//		
////	Len = *(ComBuf+1) & 0x1F;
//	Len = *(ComBuf+1) & 0x07;
//	Len *= 256;
//	Len += *(ComBuf+2);
//    
//    return Len;
//}	
/*
void FillHCS_FCS_Flag( unsigned char* ComBuf )
{
	unsigned short usCheckCode=0;
	unsigned short usDataLen=0;

	usDataLen = 10;
	usCheckCode = GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
	RAM_Write( ComBuf+usDataLen-1, (unsigned char*)&usCheckCode, 2 );
	
	usDataLen = GetFrameLength( ComBuf );

	if( usDataLen >= 12 ) 
	{
		usCheckCode = GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
		RAM_Write( ComBuf+usDataLen-1, (unsigned char*)&usCheckCode, 2 );
	}	
	*(ComBuf+usDataLen+1) = 0x7E;
}
*/

//unsigned short GetMeterAddress( unsigned char* ComBuf, unsigned char* ClientAddr, unsigned short* ServerAddrHigh, unsigned short* ServerAddrLow )
//{
//	short i;
//	unsigned short Len=0;
//	unsigned short SAPAddr;
//	unsigned short DevAddr;
//	
//	for( i=3;i<7;i++ )
//	{
//		Len++;
//		if(( *(ComBuf+i) & 0x01 ) != 0 ) break;			
//	}	
//	
////	if( Len == 2 )
////	{
////		if(( *(ComBuf+5) & 0x01 ) == 0 ) return 1;		// 客户机地址不为1字节
////		else *ClientAddr = *(ComBuf+5) / 2;
////		
////		*ServerAddrHigh = *(ComBuf+3);
////		*ServerAddrHigh /= 2; 	
////		*ServerAddrLow = *(ComBuf+4);
////		*ServerAddrLow /= 2; 	
////	}
////	else 
//	if( Len == 4 )
//	{
//		if(( *(ComBuf+7) & 0x01 ) == 0 ) return 1;		// 客户机地址不为1字节
//		else *ClientAddr = *(ComBuf+7) / 2;
//
//		*ServerAddrHigh = *(ComBuf+3);
//		*ServerAddrHigh *= 64;
//		*ServerAddrHigh += (*(ComBuf+4)/2);
//		*ServerAddrLow = *(ComBuf+5);
//		*ServerAddrLow *= 64; 	
//		*ServerAddrLow += (*(ComBuf+6)/2);
//
//		E2P_RData( (unsigned char*)&SAPAddr, ESAP, 2 );					//HDLC地址(3)				unsigned16	0x0010~0x3ffd:表号末4位加1000，例：1234+1000=2234=0x08BA
//		E2P_RData( (unsigned char*)&DevAddr, HDLC_device_address, 2 );	//服务器地址信息SAP地址(3)	0x0001
////		if( SAPAddr != 0x0001 ) SAPAddr = 0x0001;		//Test
////		if( DevAddr != 9743 ) DevAddr = 9743;			//Test
//		
////		if(( *ServerAddrHigh != 0x0001 )||( *ServerAddrLow != DevAddr )) return 1;
//		if((PIN_SETEN & P_SETEN ) == 0 ) return 0;
////		if(( *ServerAddrHigh != SAPAddr )||( *ServerAddrLow != DevAddr )) return 1;
//		if((( *ServerAddrHigh != SAPAddr )&&( *ServerAddrHigh != 0x3FFF ))||(( *ServerAddrLow != DevAddr )&&( *ServerAddrLow != 0x3FFF ))) return 1;		//13.03.15
//
//	}
//	else return 1;										//服务器地址长度不对
//	
//	return 0;	 	
//	
//}	

void Put16BitFcs( unsigned char* Source, unsigned char* Dest, unsigned short Len )
{
	unsigned short usCheckCode=0;
	
	usCheckCode=GetCheckCode_16BitFcs( Source,(unsigned short)(Len-2) );
	RAM_Write( Dest, (unsigned char*)&usCheckCode, 2 );
}	 

void ClrRSNumber( unsigned char* ComBuf )
{
	unsigned short COM_No;
	
	COM_No = GetCOM_No(ComBuf);
	
	*CommPara[COM_No].TxdFrameNo = 0;
	*CommPara[COM_No].RxdFrameNo = 0;
}	

void ClrRSBuffPara( unsigned char* ComBuf )
{
	unsigned short COM_No;
	
	COM_No = GetCOM_No(ComBuf);
	
	*CommPara[COM_No].RecDataPtr = 0;
	*CommPara[COM_No].SendDataPtr = 0;
	*CommPara[COM_No].SendByteSum = 0;
//	RAM_Fill( CommPara[COM_No].Conformance, 3);
	RAM_Fill( CommPara[COM_No].Conformance, 4);								//清除一致性块和LLS安全状态
	RAM_Fill( CommPara[COM_No].SubsBlockPara, 20);
	
}	
unsigned short GetInformationField( unsigned char* Dest )
{
	*Dest = 0x81;			//格式标识符
	*(Dest+1) = 0x80;		//组标识符
	*(Dest+2) = 0x12;		//组长(18字节)
	*(Dest+3) = 0x05;		//参数标识符(最大信息字段长度，发送)
	*(Dest+4) = 0x01;		//参数长度（1字节）
	*(Dest+5) = 0x80;		//参数值
	*(Dest+6) = 0x06;		//参数标识符(最大信息字段长度，接收)
	*(Dest+7) = 0x01;		//参数长度（1字节）
	*(Dest+8) = 0x80;		//参数值
	*(Dest+9) = 0x07;		//参数标识符(窗口大小,发送)
	*(Dest+10) = 0x04;		//参数长度（4字节）
	*(Dest+11) = 0x00;		//参数值(值高字节)
	*(Dest+12) = 0x00;		//参数值
	*(Dest+13) = 0x00;		//参数值
	*(Dest+14) = 0x01;		//参数值(值低字节)
	*(Dest+15) = 0x08;		//参数标识符(窗口大小, 接收)
	*(Dest+16) = 0x04;		//参数长度
	*(Dest+17) = 0x00;		//参数值(值高字节)
	*(Dest+18) = 0x00;		//参数值
	*(Dest+19) = 0x00;		//参数值
	*(Dest+20) = 0x01;		//参数值(值低字节)
	
	return 21;
/*	
    *Dest = 0x81;			//格式标识符
	*(Dest+1) = 0x80;		//组标识符
	*(Dest+2) = 0x14;		//组长(20字节)
	*(Dest+3) = 0x05;		//参数标识符(最大信息字段长度，发送)
	*(Dest+4) = 0x02;		//参数长度（1字节）
	*(Dest+5) = 0x03;		//参数值	
	*(Dest+6) = 0x00;		//参数值
	*(Dest+7) = 0x06;		//参数标识符(最大信息字段长度，接收)
	*(Dest+8) = 0x02;		//参数长度（1字节）
	*(Dest+9) = 0x03;		//参数值
	*(Dest+10) = 0x00;
	*(Dest+11) = 0x07;		//参数标识符(窗口大小,发送)
	*(Dest+12) = 0x04;		//参数长度（4字节）
	*(Dest+13) = 0x00;		//参数值(值高字节)
	*(Dest+14) = 0x00;		//参数值
	*(Dest+15) = 0x00;		//参数值
	*(Dest+16) = 0x01;		//参数值(值低字节)
	*(Dest+17) = 0x08;		//参数标识符(窗口大小, 接收)
	*(Dest+18) = 0x04;		//参数长度
	*(Dest+19) = 0x00;		//参数值(值高字节)
	*(Dest+20) = 0x00;		//参数值
	*(Dest+21) = 0x00;		//参数值
	*(Dest+22) = 0x01;		//参数值(值低字节)

return 23;
*/
}
//配置帧头
//FrameLen:帧长度
//SegSubFlag: 分段标志 0：最后一帧；1：还有后续帧
//ClientAddr: 客户机地址，目的地址
void CreatFrameHead( unsigned char* ComBuf, unsigned char ClientAddr, unsigned short FrameLen, unsigned char SegmentFlag )
{
	unsigned short Temp;
//	unsigned char Buff[4];
	
//	RAM_Write( (unsigned char*)&Buff, ComBuf+HDLC_SClientAddr, 4 );
	*ComBuf = 0x7E;
//	*(ComBuf+1) = FrameLen / 128;
	*(ComBuf+1) = FrameLen / 256;
	*(ComBuf+1) &= 0x07;						//取长度的高3BIT	
	*(ComBuf+1) |= 0xA0;						//帧类型
	if( SegmentFlag != 0 ) *(ComBuf+1) |= 0x08; //有后续帧
//	*(ComBuf+2) = FrameLen % 128;				//取长度的低字节	
	*(ComBuf+2) = FrameLen % 256;				//取长度的低字节	
	*(ComBuf+3) = ClientAddr;					
//	RAM_Write( ComBuf+4, (unsigned char*)&Buff, 4 );

	E2P_RData( (unsigned char*)&Temp, ESAP, 2 );					//服务器地址信息SAP地址(3)	0x0001
	*(ComBuf+4) = Temp / 128;					//每字节的最低位为0表示后续字节任然为地址，为1则结束地址构造
	*(ComBuf+4) *= 2;
	*(ComBuf+5) = Temp % 128;					//每字节只保存7BIT有效地址
	*(ComBuf+5) *= 2;
//	*(ComBuf+5) |= 0x01;						//结束地址构造

//	*(ComBuf+4) = 0x00;
//	*(ComBuf+5) = 0x02; 						//高端HDLC地址等于1
	E2P_RData( (unsigned char*)&Temp, HDLC_device_address, 2 ); //HDLC地址(3)				unsigned16	0x0010~0x3ffd:表号末4位加1000，例：1234+1000=2234=0x08BA
//	Temp = 9743;
	*(ComBuf+6) = Temp / 128;					//每字节的最低位为0表示后续字节任然为地址，为1则结束地址构造
	*(ComBuf+6) *= 2;
	*(ComBuf+7) = Temp % 128;					//每字节只保存7BIT有效地址
	*(ComBuf+7) *= 2;
	*(ComBuf+7) |= 0x01;						//结束地址构造
}
//生成UI-RR帧
void CreatUIRRFrame( unsigned char* ComBuf )
{
	unsigned short Len;
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
	Len = 10;									//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节FCS
	*(ComBuf+HDLC_CtlPtr) = HDLC_UI;
 	CreatFrameHead( ComBuf, ClientAddr, Len, LAST_FRAGMENT );
}	

//生成UA帧
void CreatUAFrame( unsigned char* ComBuf )
{
	unsigned short Len;
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
	Len = GetInformationField( ComBuf+HDLC_DataPtr );
	Len += 12;									//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节HCS+2字节FCS
	*(ComBuf+HDLC_CtlPtr) = HDLC_UA;
 	CreatFrameHead( ComBuf, ClientAddr, Len, LAST_FRAGMENT );
}	

//生成DM帧
void CreatDMFrame( unsigned char* ComBuf )
{
	unsigned short Len;
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
//	Len = GetInformationField( ComBuf+HDLC_DataPtr );
//	Len += 12;									//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节HCS+2字节FCS
	Len = 10;									//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节HCS+2字节FCS
	*(ComBuf+HDLC_CtlPtr) = HDLC_DM;
 	CreatFrameHead( ComBuf, ClientAddr, Len, LAST_FRAGMENT );
}	
//生成RR帧
void CreatRRFrame( unsigned char* ComBuf, unsigned char RxdFrameNo )
{
	unsigned short Len;
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
	Len = 10;									//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节FCS
	*(ComBuf+HDLC_CtlPtr) = RxdFrameNo;
	*(ComBuf+HDLC_CtlPtr) *= 32;
	*(ComBuf+HDLC_CtlPtr) |= HDLC_RR;
	CreatFrameHead( ComBuf, ClientAddr, Len, LAST_FRAGMENT );
}	

//生成I帧
void CreatIFrame( unsigned char* ComBuf, unsigned char RxdFrameNo, unsigned char TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag )
{
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
//	FrameLen += 10; 								//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节FCS
	FrameLen += 12; 								//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节FCS+2字节HCS
	*(ComBuf+HDLC_CtlPtr) = RxdFrameNo;
	*(ComBuf+HDLC_CtlPtr) *= 32;
	*(ComBuf+HDLC_CtlPtr) += TxdFrameNo*2;
	*(ComBuf+HDLC_CtlPtr) |= HDLC_I;
	CreatFrameHead( ComBuf, ClientAddr, FrameLen, SegmentFlag );
}
//生成UI帧
void CreatUIFrame( unsigned char* ComBuf, unsigned short FrameLen, unsigned char SegmentFlag )
{
	unsigned char ClientAddr;
	
	ClientAddr = *(ComBuf+HDLC_RClientAddr);
//	FrameLen += 10;									//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节FCS
	FrameLen += 12;									//2字节帧格式和长度+1字节目的地址+4字节源地址+1字节控制字节+2字节FCS+2字节HCS
	*(ComBuf+HDLC_CtlPtr) = HDLC_UI;
 	CreatFrameHead( ComBuf, ClientAddr, FrameLen, SegmentFlag );
}	

void HDLC_RNRFrameProcess( unsigned char* ComBuf )
{
     return; 
}  
/*
void HDLC_Ctrl(unsigned char* ComBuf)
{
	unsigned short COM_No;
//  	unsigned char Temp;
//  	unsigned char Temp2;
//  	unsigned short ComID;
//	unsigned char* ComRecTmr;
//	unsigned short* BTime;
	unsigned char* MACState;
//	unsigned char* RRRFrameNo;
//	unsigned char* SSSFrameNo;
	unsigned char SControlByte;
	unsigned char ControlByte;
//	unsigned char RespondCtlByte;
//	unsigned char RecFrameNo;
//	unsigned char SendFrameNo;

	COM_No = GetCOM_No(ComBuf);

	MACState = CommPara[COM_No].MACState;
	
//    ComRecTmr = CommPara[GetCOM_No(ComBuf)].RecTmr;
//	BTime = CommPara[GetCOM_No(ComBuf)].BTime;

	SControlByte = *(ComBuf+HDLC_CtlPtr);
	
	ControlByte = GetControlByte( SControlByte );
	if( *MACState == NDMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
				*MACState = NRMState;			//进入NRM模式
				ClrRSNumber( ComBuf );			//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_I:	
			case HDLC_RR:				
			case HDLC_RNR:		
			case HDLC_DISC:				
//			case HDLC_UI:
				ClrRSNumber( ComBuf );			//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
				CreatDMFrame( ComBuf );
				break;		
			case HDLC_UI:
			case HDLC_UI_N:
				HDLC_UIFrameProcess( ComBuf );			
				break;
			default: break;
					
		}
	}
	else if( *MACState == NRMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
				*MACState = NRMState;			//重复进入NRM模式	
				ClrRSNumber( ComBuf );			//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_I:	
				HDLC_InformationFrameProcess( ComBuf );			
				break;
			case HDLC_RR:				
				HDLC_RRFrameProcess( ComBuf );			
				break;
			case HDLC_RNR:		
				HDLC_RNRFrameProcess( ComBuf );			
				break;
			case HDLC_DISC:				
				*MACState = NDMState;
				ClrRSNumber( ComBuf );			//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
//				CreatDMFrame( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_UI:
			case HDLC_UI_N:
				HDLC_UIFrameProcess( ComBuf );			
				break;			
			default: break;
		}		
	}	
}
*/
/*
void HDLC_UIFrameProcess( unsigned char* ComBuf )
{
	unsigned short COM_No;
  	unsigned short Length;
//	unsigned char* RxdFrameNo;
//	unsigned char pSendFrameNo;
	unsigned short* RecDataPtr;
	unsigned short* SendDataPtr;
	unsigned char* RecBuff;
	unsigned char* SendBuff;
	unsigned char* LLCState;
	unsigned char ControlByte;
	unsigned short* SendByteSum;	// 485口LLC发送总数

	COM_No = GetCOM_No(ComBuf);	
	
//	pSendFrameNo = *(ComBuf+HDLC_CtlPtr) & 0x0E;
//	pSendFrameNo /= 2;
		
	ControlByte = *(ComBuf+HDLC_CtlPtr);
//	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	SendDataPtr = CommPara[COM_No].SendDataPtr;
	RecBuff = CommPara[COM_No].RecDataBuf;
	SendBuff = CommPara[COM_No].SendDataBuf;	
	LLCState = CommPara[COM_No].LLCState;
	SendByteSum = CommPara[COM_No].SendByteSum;
//	if( *RxdFrameNo != pSendFrameNo ) 
//	{
//		CreatRRFrame( ComBuf, *RxdFrameNo );				//收到的帧和期待的帧序号不一致，请求重发
//	}	
//	else
//	{
//		*RxdFrameNo += 1;
//		*RxdFrameNo &= 0x07;								//接收帧序号加1
		
		Length = GetFrameLength( ComBuf );
//		if((( *RecDataPtr + Length - 12 ) > MAX_PDU_SIZE )||(Length < 12 ) )	
		if((( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) )||(Length < 12 )||((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) )			//CTT测试软件的PDU包多发了一个字节	//UI帧不支持分帧
		{
			//返回receive_pdu_size错帧	
			_NOP();
			*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//初始化接收缓存指针
			*SendDataPtr = 0;
			if( ControlByte == HDLC_UI )
			{ 
				*LLCState = UISendNextState;
				LLCStateCycle( COM_No );	
			}	
		}
		else
		{
			Length -= 12;
			RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
			*RecDataPtr += Length;
//			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
//			{
//				*LLCState = WaitRecNextState;
//				CreatRRFrame( ComBuf, *RxdFrameNo );			//有后续帧，请求发下一帧
//			}	
//			else
//			{
				if((*RecDataPtr <= 3 )||( *RecBuff != 0xE6 )||( *(RecBuff+1) != 0xE6 )||( *(RecBuff+2) != 0x00 ))			//LLC帧格式出错
				{
					*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//初始化接收缓存指针
				}	
				else
				{
					RecBuff += 3;							//跳过LLC帧头 
					*SendBuff = 0xE6;
					*(SendBuff+1) = 0xE7;
					*(SendBuff+2) = 0x00;
					SendBuff += 3;
					*SendByteSum = 3 + APPLProcess( COM_No, RecBuff, SendBuff );		//进入正常帧处理
				}
				*SendDataPtr = 0;
				if( ControlByte == HDLC_UI )
				{
					*LLCState = UISendNextState;
					LLCStateCycle( COM_No );	
				}
				else
				{
					*SendByteSum = 0;	
					*RecDataPtr = 0;																						//初始化接收缓存指针
				}	
//			}	
		}		
//	}	
	if( ControlByte == HDLC_UI_N ) *ComBuf = 0;		//无应答码
}				
*/
/*
void HDLC_InformationFrameProcess( unsigned char* ComBuf )
{
	unsigned short COM_No;
  	unsigned short Length;
	unsigned char* RxdFrameNo;
	unsigned char pSendFrameNo;
	unsigned short* RecDataPtr;
	unsigned char* RecBuff;
	unsigned char* SendBuff;
//	unsigned char* LLCState;

	COM_No = GetCOM_No(ComBuf);	
	
	pSendFrameNo = *(ComBuf+HDLC_CtlPtr) & 0x0E;
	pSendFrameNo /= 2;
		
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	RecBuff = CommPara[COM_No].RecDataBuf;
	SendBuff = CommPara[COM_No].SendDataBuf;	
//	LLCState = CommPara[COM_No].LLCState;

	if( *RxdFrameNo != pSendFrameNo ) 
	{
		CreatRRFrame( ComBuf, *RxdFrameNo );				//收到的帧和期待的帧序号不一致，请求重发
	}	
	else
	{
		*RxdFrameNo += 1;
		*RxdFrameNo &= 0x07;								//接收帧序号加1
		
		Length = GetFrameLength( ComBuf );
//		if((( *RecDataPtr + Length - 12 ) > MAX_PDU_SIZE )||(Length < 12 ) )	
		if((( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) )||(Length < 12 ) )			//CTT测试软件的PDU包多发了一个字节
		{
			//返回receive_pdu_size错帧	
			_NOP();
			*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//初始化接收缓存指针
			*CommPara[COM_No].SendDataPtr = 0;
			*CommPara[COM_No].LLCState = SendNextState;
			LLCStateCycle( COM_No );	
		}
		else
		{
			Length -= 12;
			RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
			*RecDataPtr += Length;
			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
			{
				*CommPara[COM_No].LLCState = WaitRecNextState;
				CreatRRFrame( ComBuf, *RxdFrameNo );			//有后续帧，请求发下一帧
			}	
			else
			{
				if((*RecDataPtr <= 3 )||( *RecBuff != 0xE6 )||( *(RecBuff+1) != 0xE6 )||( *(RecBuff+2) != 0x00 ))			//LLC帧格式出错
				{
					*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//初始化接收缓存指针
				}	
				else
				{
					RecBuff += 3;							//跳过LLC帧头 
					*SendBuff = 0xE6;
					*(SendBuff+1) = 0xE7;
					*(SendBuff+2) = 0x00;
					SendBuff += 3;
					*CommPara[COM_No].SendByteSum = 3 + APPLProcess( COM_No, RecBuff, SendBuff );		//进入正常帧处理
				}
				*CommPara[COM_No].SendDataPtr = 0;
				*CommPara[COM_No].LLCState = SendNextState;
				LLCStateCycle( COM_No );	
			}	
		}		
	}	
}				
*/

/*
void HDLC_RRFrameProcess( unsigned char* ComBuf )
{
	unsigned short COM_No;
// 	unsigned short Length;
	unsigned char* RxdFrameNo;
//	unsigned char pSendFrameNo;
//	unsigned short* RecDataPtr;
//	unsigned char* RecBuff;
//	unsigned char* SendBuff;
	unsigned char* LLCState;

	COM_No = GetCOM_No(ComBuf);	
	
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	LLCState = CommPara[COM_No].LLCState;

//	*RxdFrameNo += 1;
//	*RxdFrameNo &= 0x07;									//接收帧序号加1
	
	if( *LLCState == LLCIdle )
	{
		*RxdFrameNo = 0;
		CreatRRFrame( ComBuf, *RxdFrameNo );				//电表没有准备上传帧
	}
	else if( *LLCState == WaitRecNextState )
	{
		*RxdFrameNo += 1;
		*RxdFrameNo &= 0x07;								//接收帧序号加1
		CreatRRFrame( ComBuf, *RxdFrameNo );				//请主站下发下一帧
	}
	else if( *LLCState == SendNextState )
	{
		*RxdFrameNo &= 0x07;								//接收帧序号维持不变
		LLCStateCycle( COM_No );							//电表上传帧
	}		 
}	
*/

/*
void LLCStateCycle( unsigned short COM_No )
{
	unsigned char* RxdFrameNo;
	unsigned char* TxdFrameNo;
	unsigned short* SendDataPtr;
	unsigned short* SendSum;
	unsigned char* SendBuff;
	unsigned char* LLCState;
	unsigned char* ComBuf;
	unsigned char SegmentFlag;
	unsigned short FrameLen;
	
    LLCState = CommPara[COM_No].LLCState;
    
	switch( *LLCState )	
	{
		case LLCIdle:				// LLC空闲
			break;
		case WaitRecNextState:		// LLC等待
			break;
		case SendNextState:			// 发送未完
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

			if( (*SendSum) > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			else FrameLen = *SendSum;
//			if( FrameLen > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				SegmentFlag = SUB_FRAGMENT;
				*LLCState = SendNextState;
			}	
			else 
			{
				SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
			}	
			RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
			*SendDataPtr += FrameLen;
			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			*TxdFrameNo += 1;
			*TxdFrameNo &= 0x07;
			break;
		case UISendNextState:			// 发送未完
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
//			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
//			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

			if( (*SendSum) > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			else FrameLen = *SendSum;
//			if( FrameLen > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				SegmentFlag = SUB_FRAGMENT;
				*LLCState = UISendNextState;
			}	
			else 
			{
				SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
			}	
			RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
			*SendDataPtr += FrameLen;
//			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			CreatUIFrame( ComBuf, FrameLen, SegmentFlag );
//			*TxdFrameNo += 1;
//			*TxdFrameNo &= 0x07;
			break;
		default:
		  	*LLCState = LLCIdle;
		  	break;	
	}	
}
*/
//=================================================================================================

unsigned short GetFrameLength( unsigned char* ComBuf )
{
	unsigned short Len;
   
	Len = *(ComBuf+1) & 0x07;
	Len *= 256;
	Len += *(ComBuf+2);
    return Len;
}	

void FillHCS_FCS_Flag( unsigned char* ComBuf )
{
	unsigned short usCheckCode=0;
	unsigned short usDataLen=0;

	usDataLen = 10;
	usCheckCode = GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
	RAM_Write( ComBuf+usDataLen-1, (unsigned char*)&usCheckCode, 2 );

	usDataLen = GetFrameLength( ComBuf );

	if( usDataLen >= 12 ) 
	{
		usCheckCode = GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
		RAM_Write( ComBuf+usDataLen-1, (unsigned char*)&usCheckCode, 2 );
	}	
	*(ComBuf+usDataLen+1) = 0x7E;
}

unsigned short DLMS_GetMeterAddress( unsigned char* ComBuf, unsigned char* ClientAddr, unsigned short* ServerAddrHigh, unsigned short* ServerAddrLow )
{
	short i;
	unsigned short Len=0;
	unsigned short SAPAddr;
	unsigned short DevAddr;
	for( i=3;i<7;i++ )
	{
		Len++;
		if(( *(ComBuf+i) & 0x01 ) != 0 ) break;			
	}	
	
	if( Len == 4 )
	{
		if(( *(ComBuf+7) & 0x01 ) == 0 ) return 1;		// 客户机地址不为1字节
		else *ClientAddr = *(ComBuf+7) / 2;

		*ServerAddrHigh = *(ComBuf+3);
		*ServerAddrHigh *= 64;
		*ServerAddrHigh += (*(ComBuf+4)/2);
		*ServerAddrLow = *(ComBuf+5);
		*ServerAddrLow *= 64; 	
		*ServerAddrLow += (*(ComBuf+6)/2);

		E2P_RData( (unsigned char*)&SAPAddr, ESAP, 2 );					//HDLC地址(3)				unsigned16	0x0010~0x3ffd:表号末4位加1000，例：1234+1000=2234=0x08BA
		E2P_RData( (unsigned char*)&DevAddr, HDLC_device_address, 2 );	//服务器地址信息SAP地址(3)	0x0001
		
		if((PIN_SETEN & P_SETEN ) == 0 ) 	return 0;		
		if((( *ServerAddrHigh != SAPAddr )&&( *ServerAddrHigh != 0x3FFF ))||(( *ServerAddrLow != DevAddr )&&( *ServerAddrLow != 0x3FFF ))) 
			return 1;
	}
	else 
		return 1;										//服务器地址长度不对..
	
	return 0;	 	
	
}

unsigned char DLMS_HDLC_CheckFrame( unsigned char COM_No,unsigned char* ComBuf )
{//帧校验
	unsigned char ClientAddr;
	unsigned short usCheckCode=0;
	unsigned short usDataLen=0;
	unsigned short ServerAddrHigh;
	unsigned short ServerAddrLow;
	unsigned short Check=0;

	if(( *(ComBuf+1) & 0xf0 ) != 0xA0 )	
		return 1;				//MAC子层用HDLC帧格式类型3
	if( DLMS_GetMeterAddress( ComBuf, (unsigned char*)&ClientAddr, (unsigned short*)&ServerAddrHigh, (unsigned short*)&ServerAddrLow ) != 0 )
		return 1;
	
	HComm.ClientAddr[COM_No] = ClientAddr;
	
	usDataLen = 10;
	usCheckCode=GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
	RAM_Write( (unsigned char*)&Check, ComBuf+usDataLen-1, 2 );
	if( usCheckCode != Check )
		return 1;
	
	usDataLen = GetFrameLength( ComBuf );

	if( usDataLen == 10 ) return 0;
	if( usDataLen < 12 ) return 1;
	
	usCheckCode=GetCheckCode_16BitFcs( ComBuf+1,(unsigned short)(usDataLen-2) );
	RAM_Write( (unsigned char*)&Check, ComBuf+usDataLen-1, 2 );
	if( usCheckCode != Check ) 
		return 1;                        

	return 0;
}
unsigned char GetControlByte( unsigned char SControlByte )
{
	unsigned char ControlByte;
	
	ControlByte = SControlByte;
	
	if(( ControlByte & 0x11 ) == 0x10 ) ControlByte = HDLC_I;				//I 帧
	else if(( ControlByte & 0x1f ) == 0x11 ) ControlByte = HDLC_RR;			//RR 帧
	else if(( ControlByte & 0x1f ) == 0x15 ) ControlByte = HDLC_RNR;		//RNR 帧
	else
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
			case HDLC_DISC:				
			case HDLC_UI:
			case HDLC_UI_N:
				break;			
			default: 
				ControlByte = HDLC_FRMR;		
				break;		
		}	
	}	
	return ControlByte;
}
unsigned short GetHDLCParaValue( unsigned char* Source, unsigned short* Len, unsigned long* Value )
{
	short i;
	unsigned short ErrorByte=0;
	unsigned short Length=0;
	unsigned char* Data;
	
	*Value=0;
	Length = *(Source+1);
	if( Length > 4 ) return 1;			//每项数据长度最多4字节
	switch( *Source )
	{
		case 0x05:
		case 0x06:
			break;
		case 0x07:
			if(( Length != 0 )&&( Length != 4 )) ErrorByte = 1;		//接收窗口长度字节数固定为4
			break;
		case 0x08:
			if(( Length != 0 )&&( Length != 4 )) ErrorByte = 1;		//发送窗口长度字节数固定为4
			break;
    default: 
			ErrorByte = 1;
			break;					
	}	

	*Len = Length+2;					//各项标识符和长度字节本身		
	if( Length == 0 ) return 0;			

	if( ErrorByte == 0 )
	{	
		Data = (unsigned char*)Value; 
		for( i=0;i<Length;i++ )
		{
			*(Data+i) = *(Source+2+Length-i-1);
		}	
	}
	
	return ErrorByte;
}

unsigned char JudgeHDLCParaInformationField( unsigned char* Source, short Length )
{
	unsigned long Value;
	unsigned long HDLC[4];
	unsigned char* Ptr;
	unsigned short Len;

	HDLC[0] =128;
	HDLC[1] =128;
	HDLC[2] =1;
	HDLC[3] =1;

	if( Length == 0 ) return 0;					//SNRM无信息帧内容，使用缺省值	
	Length -= 2;								//去掉FCS	
	if( Length < 3 ) return 1;					//至少一项内容
	if( *Source != 0x81 ) return 1;				//格式标识符错
	if( *(Source+1) != 0x80 ) return 1;			//组标识符错
	if( *(Source+2) != (Length-3) ) return 1;	//组长度错
	Ptr = Source+3;
	Length -= 3;
	while( Length )
	{
		if( GetHDLCParaValue( Ptr, (unsigned short*)&Len, (unsigned long*)&Value ) != 0 ) return 1;
		else
		{
			if( Length >= Len ) Length -= Len;
			else return 1;
			if( Len > 2 )								//SNRM帧内有对应参数
			{
				switch( *Ptr )
				{
					case 5: HDLC[0] = Value; break;		//接收长度
					case 6: HDLC[1] = Value; break;		//发送长度
					case 7: HDLC[2] = Value; break;		//接收窗口数
					case 8: HDLC[3] = Value; break;		//发送窗口数
					default: break;
				}									
			}	
			Ptr += Len;
		}			
    }
	if( HDLC[0] > 128 ) HDLC[0] = 128;					//电表侧最多允许接收字节为128字节
	if( HDLC[1] > 128 ) HDLC[1] = 128;					//电表侧最多允许发送字节为128字节

	return 0;
}

unsigned char JudgeHDLC_FRMR( unsigned char* ComBuf )
{
	short COM_No;
    unsigned short Length;
	unsigned char Buff[6];
	unsigned char Buff2[4];
	unsigned char* MACState;
	unsigned char* Ptr;
//	unsigned char* sRxdCnt;
//	unsigned char* sSendCnt;
	unsigned char pRxdFrameNo;
	unsigned char ErrState=0;
//	unsigned char Temp;
	unsigned char ControlByte;
    unsigned char TxdFrameNo;
	unsigned char RxdFrameNo;
    
	COM_No = GetCOM_No(ComBuf);

	MACState = CommPara[COM_No].MACState;
	
	Ptr = Buff;
	
	Length = GetFrameLength( ComBuf );
	//if( Length > 142 ) ErrState |= 0x04;									//信息帧超长
	if(COM_No==0)
		{
			if( Length > 350 )
				ErrState |= 0x04;									//信息帧超长
		}
		else
		{
				///	if( Length > 142 ) 
				if( Length > 140 )	//CTT2.6 发送141字节 2016-8-22-fire
						ErrState |= 0x04;									//信息帧超长
		}
		
//	if( GetControlByte( *(ComBuf+HDLC_CtlPtr) ) == HDLC_FRMR ) ErrState |= 0x01;	//控制字节非法

	ControlByte = GetControlByte( *(ComBuf+HDLC_CtlPtr) );
	
	if( ControlByte == HDLC_UI_N ) __NOP();			//无编号帧
	
	if( *MACState == NDMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
			case HDLC_UI:
			case HDLC_UI_N:
			case HDLC_DISC:				
			case HDLC_I:	
			case HDLC_RR:				
			case HDLC_RNR:		
				break;
			default: 
				ControlByte = HDLC_FRMR;									//NDM模式下，只接受上述3种命令
				break;
		}
	}

	*CommPara[COM_No].LastRecCommand = ControlByte;
	
	if(( ControlByte == HDLC_SNRM )&&( ErrState == 0 ))						//信息帧长度合法情况下判断SNRM参数
	{
		if( JudgeHDLCParaInformationField( ComBuf+HDLC_DataPtr, Length-10 ) != 0 )
			ErrState |= 0x01;	//SNRM参数内容非法	
	}	

	if( ControlByte == HDLC_FRMR ) 
		ErrState |= 0x01;						//控制字节非法
	
	TxdFrameNo = *CommPara[COM_No].TxdFrameNo;
	RxdFrameNo = *CommPara[COM_No].RxdFrameNo;

	if(( ControlByte == HDLC_UI )||( ControlByte == HDLC_UI_N ))			//无编号帧
	{
		if((HComm.APPL_A8_BUSY[COM_No]&0x0f)==0x05)
		{
		 	if( ControlByte == HDLC_UI )	
				ErrState |= 0x07;						//还在有序号分帧状态，不接受无序号的操作..		 				 	
		}
		TxdFrameNo = 0;
		RxdFrameNo = 0;
	}	

	if(( ControlByte == HDLC_I )||( ControlByte == HDLC_RR )||( ControlByte == HDLC_RNR ))	//带编号的帧
	{	
		pRxdFrameNo = *(ComBuf+HDLC_CtlPtr) & 0xE0;
		pRxdFrameNo /= 32;
			
		if( pRxdFrameNo != TxdFrameNo ) 
	//if(( pRxdFrameNo != TxdFrameNo )&&(SM.Module_Type!=Module_GPRS)) //gprs模式时候不判断帧序号	
			ErrState |= 0x08;							//主从站的发送接受序列号不符，即主站期望接收的帧编号和从站准备发送的帧编号不一致。
		if((HComm.APPL_A8_BUSY[COM_No]&0x0f)==0x0a)
		{
		 	ErrState |= 0x07;						//还在有序号分帧状态，不接受无序号的操作..
		}
//		else{
//			HComm.APPL_A8_BUSY[COM_No] = 0;					//操作..
//		}					
	}
	
	if( ErrState != 0 )														//帧格式异常
	{
		Buff2[0] = *(ComBuf+HDLC_CtlPtr);		
		Buff2[1] = TxdFrameNo * 2 + RxdFrameNo * 32 + (Buff2[0]&0x10);		
		Buff2[2] = ErrState;		

		*ComBuf = 0x7E;
		*(ComBuf+1) = 0xA0;
		*(ComBuf+2) = 15;
		RAM_Write( Ptr, ComBuf+3, 4 );
		*(ComBuf+3) = *(ComBuf+7);
		RAM_Write( ComBuf+4, Ptr, 4 );
		
		*(ComBuf+HDLC_CtlPtr) = HDLC_FRMR;
		RAM_Write( ComBuf+HDLC_DataPtr, (unsigned char*)&Buff2, 3 );		//异常帧信息内容
		
		HComm.APPL_A8_BUSY[COM_No] = 0;
	}
	
	return ErrState;
			
}
void HDLC_UIFrameProcess( unsigned char* ComBuf )
{
#if 0 
		unsigned short COM_No;
  		unsigned short Length;
//	unsigned char* RxdFrameNo;
//	unsigned char pSendFrameNo;
		unsigned short* RecDataPtr;
		unsigned short* SendDataPtr;
		unsigned char* RecBuff;
		unsigned char* SendBuff;
		unsigned char* LLCState;
		unsigned char ControlByte;
		unsigned short* SendByteSum;	// 485口LLC发送总数
		
		unsigned short FrameLen,i;
		unsigned char * RepeatPtr;				
		unsigned long* BlockNumber;
        unsigned long  RdFc_number=0;

		COM_No = GetCOM_No(ComBuf);
		    	
		BlockNumber = CommPara[COM_No].BlockNumber;
    	RepeatPtr = HComm.RepeatBuff;
        
//	pSendFrameNo = *(ComBuf+HDLC_CtlPtr) & 0x0E;
//	pSendFrameNo /= 2;
		
		ControlByte = *(ComBuf+HDLC_CtlPtr);
//	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
		RecDataPtr = CommPara[COM_No].RecDataPtr;
		SendDataPtr = CommPara[COM_No].SendDataPtr;
		RecBuff = CommPara[COM_No].RecDataBuf;
		SendBuff = CommPara[COM_No].SendDataBuf;	
		LLCState = CommPara[COM_No].LLCState;
		SendByteSum = CommPara[COM_No].SendByteSum;
//	if( *RxdFrameNo != pSendFrameNo ) 
//	{
//		CreatRRFrame( ComBuf, *RxdFrameNo );				//收到的帧和期待的帧序号不一致，请求重发
//	}	
//	else
//	{
//		*RxdFrameNo += 1;
//		*RxdFrameNo &= 0x07;								//接收帧序号加1
		
//	if(((HComm.APPL_A8_BUSY[COM_No]&0x0f)==0x05)||((HComm.APPL_A8_BUSY[COM_No]&0x80)!=0))
//	{
//		// 	if( ControlByte == HDLC_UI )	ErrState |= 0x01;						//还在有序号分帧状态，不接受无序号的操作..		 	
//		// HComm.APPL_A8_BUSY[COM_No] = 0;		
//		*ComBuf = 0;		//无应答码
//		//*RecDataPtr = 0;																						//初始化接收缓存指针		
//		return;
//	}
		
		Length = GetFrameLength( ComBuf );
		
		//增加全局（解密）判断，解密后执行同无加密状态..
	//	Decrypt_OR_Encrypt(COM_No,ComBuf+HDLC_DataPtr,&Length,0);   //判断解密类型，做类型标识输出加解密密文,use_crypt:0--输入，1--加密..
    //增加FC测试sky20160305..
     if(( crypt.glo_request[COM_No] == GLO_GET_Request )||( crypt.glo_request[COM_No] == GLO_SET_Request )||( crypt.glo_request[COM_No] == GLO_ACTION_Request ))   //全局加密模式..
    	{
    		//HComm.UK_FCRXCount++;
    		//判断对错..指针要大于接受指针才给予通过..
    			RAM_Write( (unsigned char*)&RdFc_number ,&crypt.glo_FC[1],4 );
	  	 		SwapData( (unsigned char*)&RdFc_number, 4 );
	  	 		if(HComm.UK_FCRXCount>RdFc_number)
	  	 		{
						//返回receive_pdu_size错帧	
						__NOP();
						*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
						*RecDataPtr = 0;																						//初始化接收缓存指针
						*SendDataPtr = 0;
						if( ControlByte == HDLC_UI )
						{ 
								*LLCState = UISendNextState;
								LLCStateCycle( COM_No );	
						}		  	 			
	  	 		} 
    	}
    	else
    	{
       if(( crypt.glo_request[COM_No] == GLO_GET_Request )||( crypt.glo_request[COM_No] == GLO_SET_Request )||( crypt.glo_request[COM_No] == GLO_ACTION_Request ))   //全局加密模式..
  		 {
   				//判断对错..指针要大于接受指针才给予通过..
    			RAM_Write( (unsigned char*)&RdFc_number ,&crypt.glo_FC[1],4 );
	  	 		SwapData( (unsigned char*)&RdFc_number, 4 );
	  	 		if(HComm.DK_FCRXCount>RdFc_number)
	  	 		{
						//返回receive_pdu_size错帧	
						__NOP();
						*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
						*RecDataPtr = 0;																						//初始化接收缓存指针
						*SendDataPtr = 0;
						if( ControlByte == HDLC_UI )
						{ 
								*LLCState = UISendNextState;
								LLCStateCycle( COM_No );	
						}		  	 			
	  	 		}   		 	
  		 }    		
    	}
    
		if((( *RecDataPtr + Length - 12 ) > MAX_PDU_SIZE )||(Length < 12 ) )	
//		if((( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) )||(Length < 12 )||((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) )			//CTT测试软件的PDU包多发了一个字节	//UI帧不支持分帧
		{
			//返回receive_pdu_size错帧	
			__NOP();
			*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//初始化接收缓存指针
			*SendDataPtr = 0;
			if( ControlByte == HDLC_UI )
			{ 
					*LLCState = UISendNextState;
					LLCStateCycle( COM_No );	
			}	
		}
		else
		{
			Length -= 12;
			if(HComm.APPL_A8_BUSY[COM_No] == 0x0A)
				{      //UI帧时的分帧操作..
					if(( *(ComBuf+HDLC_DataPtr) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+1) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+2) == 0x00 ))
					{
						if(Length>3)
						{
							Length -=3;
							RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+3, Length );
						}else{
							RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
						}
					}
					else
					{
	                 RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
	                }
				}else
				{				
			        RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
		        }
		  
			// RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
			*RecDataPtr += Length;
			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
			{
				*CommPara[COM_No].LLCState = WaitRecNextState;
				
				if( ControlByte == HDLC_UI_N ) *ComBuf = 0;		//无应答码
				else 	CreatUIRRFrame( ComBuf );		//有后续帧，请求发下一帧				

				// 增加判断，如果是分帧操作则不接受不是相同命令的操作  //2013.06.24..
				HComm.APPL_A8_BUSY[COM_No] = 0x0A;	//分帧操作状态..							
			}	
			else
			{
				HComm.APPL_A8_BUSY[COM_No] = 0x0;	//分帧操作状态..											
				if((*RecDataPtr <= 3 )||( *RecBuff != 0xE6 )||( *(RecBuff+1) != 0xE6 )||( *(RecBuff+2) != 0x00 ))			//LLC帧格式出错
				{
					*SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//初始化接收缓存指针
				}	
				else
				{
					RecBuff += 3;							//跳过LLC帧头
					PushSetup.SendNumber = 0;
					if((ControlByte == HDLC_UI )&&(*RecDataPtr == 10 )&&( *RecBuff == 0xc0 )&&( *(RecBuff+1) == 0x02 )&&( *(RecBuff+3) == 0x00 )&&( *(RecBuff+4) == 0x00 ))			//读后续数据块帧格式判断..
							{								
									PushSetup.SendNumber =  (*(RecBuff+5))*256 + *(RecBuff+6);	
							}		
					//			//增加打包情况下，进行重发的功能(判断BLOCK数据块)(配合南瑞集中器载波信号不少做的重发机制)..
					if((PushSetup.SendNumber!=0)&&(*BlockNumber!=0))   
					{
							if((PushSetup.SendNumber +1 == *BlockNumber )&&(*(RepeatPtr+148)!= 0)) 
							{ 
									FrameLen = *(RepeatPtr+148);
									for(i=0;i<FrameLen;i++) *(ComBuf+i) = *(RepeatPtr+i) ;  //chongfa
									*CommPara[COM_No].RecDataPtr = 0;
		      				return;  
		      		}
		      		if(PushSetup.SendNumber != *BlockNumber )
		      		{
		      				*CommPara[COM_No].RecDataPtr = 0;
		      				return;  
		      		}
		     	}
//					*SendBuff = 0xE6;
//					*(SendBuff+1) = 0xE7;
//					*(SendBuff+2) = 0x00;
//					SendBuff += 3;

//								  //======================================		
								  
 					//add UI-datablock trans  sky20140519..
					// if((HComm.COSEM_BlockState[COM_No]!=DATABLOCKIDLE)&&(*RecDataPtr <= 12 )&&(COM_No==1))
                                       // if((HComm.COSEM_BlockState[COM_No]!=DATABLOCKIDLE)&&(COM_No==1))
					//if((*LLCState) == UIBlockSendNextState)
 				if(((*LLCState) == UIBlockSendNextState)&&(HComm.COSEM_BlockState[COM_No]!=DATABLOCKIDLE)&&(*RecDataPtr == 10 ))
// 				if(((*LLCState) == UIBlockSendNextState)&&(HComm.COSEM_BlockState[COM_No]!=DATABLOCKIDLE)&&(*RecDataPtr == 10 )&&(COM_No==0)) 
					{
							if( ControlByte == HDLC_UI )
							{
								//7E A0 16 FE FE FE FF 23 13 A1 20 E6 E6 00 c0 02 c1 00 00 00 01 73 7F 7E
								//if((*RecDataPtr <= 12 )||( *RecBuff != 0xc0 )||( *(RecBuff+1) != 0x02 )||( *(RecBuff+4) != 0x00 )||( *(RecBuff+5) != 0x00 ))			//读后续数据块帧格式出错
									if((*RecDataPtr <= 10 )&&( *RecBuff == 0xc0 )&&( *(RecBuff+1) == 0x02 )&&( *(RecBuff+3) == 0x00 )&&( *(RecBuff+4) == 0x00 ))			//读后续数据块帧格式判断..
									{								
											*LLCState = UIBlockSendNextState;
									    PushSetup.SendNumber =  (*(RecBuff+5))*256 + *(RecBuff+6);									    									    
											LLCStateCycle( COM_No );
									}
									else
									{
											*SendBuff = 0xE6;
											*(SendBuff+1) = 0xE7;
											*(SendBuff+2) = 0x00;
											SendBuff += 3;										
											*SendByteSum = 3 + CreatExceptionResponseAPDU( SendBuff );
											*RecDataPtr = 0;																						//初始化接收缓存指针
											*LLCState = UIBlockSendNextState;
											HComm.COSEM_BlockState[COM_No] = DATABLOCKIDLE;
											LLCStateCycle( COM_No );
									}	
							}
							else
							{
									*SendByteSum = 0;	
									*RecDataPtr = 0;
								  HComm.COSEM_BlockState[COM_No] = DATABLOCKIDLE;																						//初始化接收缓存指针
							}
				 }
				 else	
				 {							
							*SendBuff = 0xE6;
							*(SendBuff+1) = 0xE7;
							*(SendBuff+2) = 0x00;
							SendBuff += 3;				 	
				 			*SendByteSum = 3 + APPLProcess( COM_No, RecBuff, SendBuff );		//进入正常帧处理
				 //}
				 //}
							*SendDataPtr = 0;
							if( ControlByte == HDLC_UI )
							{
								if(COM_No==0) *LLCState = UIBlockSendNextState;
//								if(COM_No==1) *LLCState = UIBlockSendNextState; //test..
								else *LLCState = UISendNextState;
//								//test..
//								 *LLCState = UIBlockSendNextState;
								LLCStateCycle( COM_No );	
							}
							else
							{
								*SendByteSum = 0;	
								*RecDataPtr = 0;																						//初始化接收缓存指针
							}
					}
				}	
			}	
	}		
//	}	
	if( ControlByte == HDLC_UI_N ) *ComBuf = 0;		//无应答码
#endif     
}
void HDLC_InformationFrameProcess( unsigned char* ComBuf )
{
#if 0 
	unsigned short COM_No;
    unsigned short Length;
	unsigned char* RxdFrameNo;
	unsigned char pSendFrameNo;
	unsigned short* RecDataPtr;
	unsigned char* RecBuff;
	unsigned char* SendBuff;
//	unsigned char* LLCState;
	unsigned char* SetBlockBuff;
	unsigned short* SetBlockPtr;
	

	COM_No = GetCOM_No(ComBuf);	
	
	pSendFrameNo = *(ComBuf+HDLC_CtlPtr) & 0x0E;
	pSendFrameNo /= 2;
		
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	RecBuff = CommPara[COM_No].RecDataBuf;
	SendBuff = CommPara[COM_No].SendDataBuf;	
//	LLCState = CommPara[COM_No].LLCState;

	//临时start..
	SetBlockBuff = CommPara[COM_No].SetBlockBuff;
	SetBlockPtr = CommPara[COM_No].SetBlockPtr;
	//临时end..

	if( *RxdFrameNo != pSendFrameNo ) 
	{
		CreatRRFrame( ComBuf, *RxdFrameNo );				//收到的帧和期待的帧序号不一致，请求重发
	}	
	else
	{
		*RxdFrameNo += 1;
		*RxdFrameNo &= 0x07;								//接收帧序号加1
		
		Length = GetFrameLength( ComBuf );
		//增加全局（解密）判断，解密后执行同无加密状态..
		Decrypt_OR_Encrypt(COM_No,ComBuf+HDLC_DataPtr,&Length,0);   //判断解密类型，做类型标识输出加解密密文,use_crypt:0--输入，1--加密..

    //自己增加FC测试sky20160306..
    if(( crypt.glo_request[COM_No] == GLO_GET_Request )||( crypt.glo_request[COM_No] == GLO_SET_Request )||( crypt.glo_request[COM_No] == GLO_ACTION_Request ))   //全局加密模式..
    {
        HComm.UK_FCRXCount++;
    }
              
		//临时START-增加分帧数据超出512处理sky20140711..
			 if(((HComm.APPL_A8_BUSY[COM_No] == 0x05)&&(( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) ))||(crypt.Special_A8[COM_No] == 0x06)){
			 		if(crypt.Special_A8[COM_No] != 0x06){
			 			*SetBlockPtr = 0;
			 			RAM_Write( SetBlockBuff,RecBuff,(*RecDataPtr));
			 		}
			 		crypt.Special_A8[COM_No] = 0x06;   			//超过512，save to datablock buff..	
			 		
		if((Length < 12 )||(( *RecDataPtr + Length - 12 ) > (MAX_BLOCK_SIZE+1)))			//帧长度出错..
		{	
			//返回receive_pdu_size错帧				
			__NOP();
			*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//初始化接收缓存指针
			*SetBlockPtr = 0;
			*CommPara[COM_No].SendDataPtr = 0;
			*CommPara[COM_No].LLCState = SendNextState;
			LLCStateCycle( COM_No );
			crypt.Special_A8[COM_No] = 0;  	
		}
		else			
		{ 		
			Length -= 12;
			if(HComm.APPL_A8_BUSY[COM_No] == 0x05){						//带序号帧的分帧操作..
				if(( *(ComBuf+HDLC_DataPtr) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+1) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+2) == 0x00 ))
				{
					if( Length>3 )
					{
						Length -=3;
						RAM_Write( SetBlockBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+3, Length );
					}else{
								RAM_Write( SetBlockBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
							}
				}else{
                RAM_Write( SetBlockBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
             }
			}else{				
			        RAM_Write( SetBlockBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
		        }
			*RecDataPtr += Length;
			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
			{
				*CommPara[COM_No].LLCState = WaitRecNextState;
				CreatRRFrame( ComBuf, *RxdFrameNo );		//有后续帧，请求发下一帧
				// 增加判断，如果是分帧操作则不接受不是相同命令的操作  //2013.06.24..
				HComm.APPL_A8_BUSY[COM_No] = 0x05;	//分帧操作状态..			
			}	
			else
			{
				HComm.APPL_A8_BUSY[COM_No] = 0;			//无分帧操作状态..
				crypt.Special_A8[COM_No] = 0;  
				if((*RecDataPtr <= 3 )||( *SetBlockBuff != 0xE6 )||( *(SetBlockBuff+1) != 0xE6 )||( *(SetBlockBuff+2) != 0x00 ))			//LLC帧格式出错
				{
					*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//初始化接收缓存指针
				}	
				else
				{
					SetBlockBuff += 3;							//跳过LLC帧头					 
					*SendBuff = 0xE6;
					*(SendBuff+1) = 0xE7;
					*(SendBuff+2) = 0x00;
					SendBuff += 3;
					*CommPara[COM_No].SendByteSum = 3 + APPLProcess( COM_No, SetBlockBuff, SendBuff );		//进入正常帧处理,要发送字节总数量
				}
				*CommPara[COM_No].SendDataPtr = 0;
				*CommPara[COM_No].LLCState = SendNextState;
				LLCStateCycle( COM_No );	
			}
		}	
			return;			 			
			 }
		//临时END...
		
//		if((( *RecDataPtr + Length - 12 ) > MAX_PDU_SIZE )||(Length < 12 ) )	
		if((( *RecDataPtr + Length - 12 ) > (MAX_PDU_SIZE+1) )||(Length < 12 ) )			//CTT测试软件的PDU包多发了一个字节
		{
			//返回receive_pdu_size错帧	
			__NOP();
			*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
			*RecDataPtr = 0;																						//初始化接收缓存指针
			*CommPara[COM_No].SendDataPtr = 0;
			*CommPara[COM_No].LLCState = SendNextState;
			LLCStateCycle( COM_No );
			crypt.Special_A8[COM_No] = 0;  	
		}
		else
		{
			Length -= 12;
			crypt.Special_A8[COM_No] = 0;
			if(HComm.APPL_A8_BUSY[COM_No] == 0x05){						//带序号帧的分帧操作..
				if(( *(ComBuf+HDLC_DataPtr) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+1) == 0xE6 )&&( *(ComBuf+HDLC_DataPtr+2) == 0x00 ))
				{
					if(Length>3)
					{
						Length -=3;
						RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+3, Length );
					}else{
								RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
							}
				}else{
                RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
             }
			}else{				
			        RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
		        }
			*RecDataPtr += Length;
			if((*(ComBuf+HDLC_LenPtrH) & 0x08 ) != 0 ) 
			{
				*CommPara[COM_No].LLCState = WaitRecNextState;
				CreatRRFrame( ComBuf, *RxdFrameNo );		//有后续帧，请求发下一帧
				// 增加判断，如果是分帧操作则不接受不是相同命令的操作  //2013.06.24..
				HComm.APPL_A8_BUSY[COM_No] = 0x05;	//分帧操作状态..			
			}	
			else
			{
				HComm.APPL_A8_BUSY[COM_No] = 0;			//分帧操作状态..
				if((*RecDataPtr <= 3 )||( *RecBuff != 0xE6 )||( *(RecBuff+1) != 0xE6 )||( *(RecBuff+2) != 0x00 ))			//LLC帧格式出错
				{
					*CommPara[COM_No].SendByteSum = CreatExceptionResponseAPDU( SendBuff );
					*RecDataPtr = 0;																						//初始化接收缓存指针
				}	
				else
				{
					RecBuff += 3;							//跳过LLC帧头					 
					*SendBuff = 0xE6;
					*(SendBuff+1) = 0xE7;
					*(SendBuff+2) = 0x00;
					SendBuff += 3;
					*CommPara[COM_No].SendByteSum = 3 + APPLProcess( COM_No, RecBuff, SendBuff );		//进入正常帧处理
				}
				*CommPara[COM_No].SendDataPtr = 0;
				*CommPara[COM_No].LLCState = SendNextState;
				LLCStateCycle( COM_No );	
			}	
		}		
	}
#endif	
}

void HDLC_Ctrl(unsigned char* ComBuf)
	{
	unsigned short COM_No;
	unsigned char* MACState;
	unsigned char SControlByte;
	unsigned char ControlByte;
	COM_No = GetCOM_No(ComBuf);
/*	
///2016-8-26,去掉密码错误次数判断，为CTT测试
//处于接收状态，则事件推送，推送时间清零skyblue201410..
	if(COM_No==2)  PushSetup.Time = 30;
	
	//本地口密码错误次数超过3次，则退出..
	if(((COM_No==1)||(COM_No==0))&&((PIN_SETEN & P_SETEN ) != 0))
		{
			if(ICComm.CommErrTime[COM_No]>ICComm.SHEZHIErrTime)   //密码错误次数过多，则拒绝访问操作..
			{
				//			case HDLC_DISC:			
				ClrRSNumber( ComBuf );				//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
				CreatDMFrame( ComBuf );
				return;	
			} 
		}
	*/
	MACState = CommPara[COM_No].MACState;
	
//  ComRecTmr = CommPara[GetCOM_No(ComBuf)].RecTmr;
//	BTime = CommPara[GetCOM_No(ComBuf)].BTime;

	SControlByte = *(ComBuf+HDLC_CtlPtr);
	
	ControlByte = GetControlByte( SControlByte );
	if( *MACState == NDMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
				*MACState = NRMState;					//进入NRM模式
				ClrRSNumber( ComBuf );				//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_I:	
			case HDLC_RR:				
			case HDLC_RNR:		
			case HDLC_DISC:				
//			case HDLC_UI:
				ClrRSNumber( ComBuf );				//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
				CreatDMFrame( ComBuf );
				break;		
			case HDLC_UI:
			case HDLC_UI_N:
				HDLC_UIFrameProcess( ComBuf );			
				break;
			default: break;
					
		}
	}
	else if( *MACState == NRMState )
	{
		switch( ControlByte )
		{
			case HDLC_SNRM:	
				*MACState = NRMState;					//重复进入NRM模式	
				ClrRSNumber( ComBuf );				//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
				CreatUAFrame( ComBuf );
				break;
			case HDLC_I:	
				HDLC_InformationFrameProcess( ComBuf );			
				break;
			case HDLC_RR:				
				HDLC_RRFrameProcess( ComBuf );			
				break;
			case HDLC_RNR:		
				HDLC_RNRFrameProcess( ComBuf );			
				break;
			case HDLC_DISC:				
				*MACState = NDMState;
				ClrRSNumber( ComBuf );				//清除发送接收序列号
				ClrRSBuffPara( ComBuf );
//				CreatDMFrame( ComBuf );     //CTT通过需要用UA..P3无法通过..
				CreatUAFrame( ComBuf );        
				break;
			case HDLC_UI:
			case HDLC_UI_N:
				HDLC_UIFrameProcess( ComBuf );			
				break;			
			default: break;
		}		
	}	
}
/*
//fire--2017年6月5日21:25:23
{
////  	unsigned short Length;
//	unsigned short AddrLen;
//	unsigned short HDLC_DataPtr;
//	unsigned char FrameType;
//
////	Length = GetFrameLength( ComBuf );
//	AddrLen = GetAddrLength( ComBuf );
//	HDLC_DataPtr = AddrLen + 8;						//帧头(1)、长度(2)、控制码(1)、地址头码(1)、客户机地址(1)、HCS(2)
//
//	if((*(ComBuf+HDLC_CtlPtr) & 0x20 ) != 0 ) 								//分帧
//	{
//		FrameType = *(ComBuf+HDLC_DataPtr + 1) & 0xC0;
//
////		pSendFrameNo = *(ComBuf+HDLC_DataPtr + 1) & 0x3F;
////		pSendFrameNo *= 256;
////		pSendFrameNo += *(ComBuf+HDLC_DataPtr);
//		
//		if( FrameType == COMFIRM_FRAME ) HDLC_RRFrameProcess( ComBuf );
//		else HDLC_InformationFrameProcess( ComBuf );	
//	}		
//	else HDLC_InformationFrameProcess( ComBuf );
	HDLC_InformationFrameProcess( ComBuf );
}

void HDLC_InformationFrameProcess( unsigned char* ComBuf )
{
	unsigned short COM_No;
 
	unsigned short Length;
	unsigned short* TxdFrameNo;
	unsigned short* RxdFrameNo;
	unsigned short pSendFrameNo;
	unsigned short* RecDataPtr;
	unsigned char* RecBuff;
	unsigned char* SendBuff;
//	unsigned char* LLCState;
	unsigned short AddrLen;
	unsigned short HDLC_DataPtr;
//	unsigned char ControlByte;
	unsigned char FrameType;
	unsigned char* SecurityState;			
	unsigned char* TimeTag;			//17.04.18

	COM_No = GetCOM_No(ComBuf);	
	
//	ControlByte = *(ComBuf+HDLC_CtlPtr);
	
	TxdFrameNo = CommPara[COM_No].TxdFrameNo;
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	RecDataPtr = CommPara[COM_No].RecDataPtr;
	RecBuff = CommPara[COM_No].RecDataBuf;
	SendBuff = CommPara[COM_No].SendDataBuf;	
	SecurityState = CommPara[COM_No].SecurityState;	// 通讯口安全状态		0x00: 非安全认证操作 0x01：明文 0x02:密文
	TimeTag = CommPara[COM_No].TimeTag;				//通讯口时间标签	optional:1字节，data_time_s:7字节，TI:3字节		//17.04.18
	
	Length = GetFrameLength( ComBuf );
	AddrLen = GetAddrLength( ComBuf );
	HDLC_DataPtr = AddrLen + 8;						//帧头(1)、长度(2)、控制码(1)、地址头码(1)、客户机地址(1)、HCS(2)
	if((( *RecDataPtr + (Length - (HDLC_DataPtr+1)) ) > (MAX_PDU_SIZE+1) )||(Length < (HDLC_DataPtr+1) ) )			//CTT测试软件的PDU包多发了一个字节
	{
		*ComBuf = 0xFF;				//PDU超长。
		*RecDataPtr = 0;
	}
	else
	{
		if((*(ComBuf+HDLC_CtlPtr) & 0x20 ) != 0 ) 								//分帧
		{
			FrameType = *(ComBuf+HDLC_DataPtr + 1) & 0xC0;						//低字节在前
			pSendFrameNo = *(ComBuf+HDLC_DataPtr + 1) & 0x3F;
			pSendFrameNo *= 256;
			pSendFrameNo += *(ComBuf+HDLC_DataPtr);

//			FrameType = *(ComBuf+HDLC_DataPtr) & 0xC0;							//高字节在前
//			pSendFrameNo = *(ComBuf+HDLC_DataPtr) & 0x3F;
//			pSendFrameNo *= 256;
//			pSendFrameNo += *(ComBuf+HDLC_DataPtr+1);
			
			switch( FrameType )
			{
				case START_FRAME:								//起始帧
					*RxdFrameNo = pSendFrameNo;
					*RecDataPtr = 0;
					break;
				case LAST_FRAME:								//最后帧
				case SUB_FRAME:									//中间帧
					if( (*RxdFrameNo+1) != pSendFrameNo )		//要求新收到的比已收到的大1
					{
						*CommPara[COM_No].LLCState = WaitRecNextState;
						CreatRRFrame( ComBuf, *RxdFrameNo );				//收到的帧和期待的帧序号不一致，请求重发
						return;
					}else *RxdFrameNo += 1;	
					break;
				case COMFIRM_FRAME:								//确认帧	
					*RxdFrameNo += 1;							//接收帧序号加1
					break;
				default: 
					break;				
			}	
//			*RxdFrameNo += 1;
//			*RxdFrameNo &= 0x3F;								//接收帧序号加1
			*RxdFrameNo &= 0x3FFF;								

			if( FrameType == COMFIRM_FRAME )					//确认帧
			{
//				*ComBuf = 0xFF;				
				if( *CommPara[COM_No].SendByteSum == 0 )		//报文出错，无数据返回
				{
					*ComBuf = 0xFF;				
					*CommPara[COM_No].SendDataPtr = 0;
	                *RecDataPtr = 0;
					*CommPara[COM_No].LLCState = LLCIdle;
					*TxdFrameNo = 0;
					*RxdFrameNo = 0;
					LLCStateCycle( COM_No );	
				}
				else
				{
//					*CommPara[COM_No].SendDataPtr = 0;
					*CommPara[COM_No].LLCState = SendNextState;
//					*TxdFrameNo += 1;
					LLCStateCycle( COM_No );	
				}
				return;
			}	

			Length -= (HDLC_DataPtr+3);												//去掉地址部分及校验	
//			RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+3, Length );		//将本次收到的信息帧内容移入数据接收缓存区,滤掉分帧格式符信息
			RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr+2, Length );		//将本次收到的信息帧内容移入数据接收缓存区,滤掉分帧格式符信息
			*RecDataPtr += Length;
			
			if( FrameType == LAST_FRAME )						//最后帧
			{
				*SecurityState = 0;								// 通讯口安全状态		0x00: 非安全认证操作 0x01：明文 0x02:密文	//17.04.07
				*TimeTag = 0;									//通讯口时间标签	optional:1字节，data_time_s:7字节，TI:3字节		//17.04.18
				*CommPara[COM_No].SendByteSum = APPLProcess( COM_No, RecBuff, SendBuff );		//进入正常帧处理
				*SecurityState = 0;								// 通讯口安全状态		0x00: 非安全认证操作 0x01：明文 0x02:密文
				if(( *CommPara[COM_No].SendByteSum == 0 )||(( *(ComBuf+HDLC_SACtlPtr) & 0x80) != 0 ))		//报文出错或组地址、或广播地址，无数据返回
				{
					*ComBuf = 0xFF;				
					*CommPara[COM_No].SendDataPtr = 0;
	                *RecDataPtr = 0;
					*CommPara[COM_No].LLCState = LLCIdle;
					*TxdFrameNo = 0;
					*RxdFrameNo = 0;
					LLCStateCycle( COM_No );	
				}
				else
				{
					*CommPara[COM_No].SendDataPtr = 0;
					*CommPara[COM_No].LLCState = SendNextState;
					*TxdFrameNo = 0;
					LLCStateCycle( COM_No );	
				}
			}
			else
			{		
				*CommPara[COM_No].LLCState = WaitRecNextState;
				CreatRRFrame( ComBuf, *RxdFrameNo );			//有后续帧，请求发下一帧
			}	
		}	
		else
		{
			Length -= (HDLC_DataPtr+1);												//去掉地址部分及校验
			if(( *(ComBuf+HDLC_DataPtr) == APDU_SECURITY_Request )&&( *(ComBuf+HDLC_DataPtr+1) == 0x00 )&&(( *(ComBuf+HDLC_DataPtr+2) & 0x80 ) == 0x00 )
				&&( *(ComBuf+HDLC_DataPtr+3) == APDU_GET_Request )&&( *(ComBuf+HDLC_DataPtr+4) == get_request_next ))
			{
				*TimeTag = 0;									//通讯口时间标签	optional:1字节，data_time_s:7字节，TI:3字节		//17.04.18
				*CommPara[COM_No].SendByteSum = GetSECURITYRequestNextProcess( COM_No, RecBuff, SendBuff, ComBuf+HDLC_DataPtr );
			}	
			else if(( *(ComBuf+HDLC_DataPtr) == APDU_GET_Request )&&( *(ComBuf+HDLC_DataPtr+1) == get_request_next ))
			{
				*TimeTag = 0;									//通讯口时间标签	optional:1字节，data_time_s:7字节，TI:3字节		//17.04.18
				*CommPara[COM_No].SendByteSum = GetRequestNextProcess( COM_No, RecBuff, SendBuff, ComBuf+HDLC_DataPtr );
			}
			else
			{		
				*RecDataPtr = 0;														//无分帧，永远指向0
				RAM_Write( RecBuff+(*RecDataPtr), ComBuf+HDLC_DataPtr, Length );		//将本次收到的信息帧内容移入数据接收缓存区
				*RecDataPtr += Length;				
				*SecurityState = 0;								// 通讯口安全状态		0x00: 非安全认证操作 0x01：明文 0x02:密文	//17.04.07
				*TimeTag = 0;									//通讯口时间标签	optional:1字节，data_time_s:7字节，TI:3字节		//17.04.18
				*CommPara[COM_No].SendByteSum = APPLProcess( COM_No, RecBuff, SendBuff );		//进入正常帧处理
				*SecurityState = 0;								// 通讯口安全状态		0x00: 非安全认证操作 0x01：明文 0x02:密文
			}
			if(( *CommPara[COM_No].SendByteSum == 0 )||(( *(ComBuf+HDLC_SACtlPtr) & 0x80) != 0 ))		//报文出错或组地址、或广播地址，无数据返回
			{
				*ComBuf = 0xFF;				
				*CommPara[COM_No].SendDataPtr = 0;
                *RecDataPtr = 0;
				*CommPara[COM_No].LLCState = LLCIdle;
				*TxdFrameNo = 0;
				*RxdFrameNo = 0;
				LLCStateCycle( COM_No );	
			}
			else
			{
				*CommPara[COM_No].SendDataPtr = 0;
				*CommPara[COM_No].LLCState = SendNextState;
				*TxdFrameNo = 0;
				LLCStateCycle( COM_No );	
			}
		}	
	}		
	
}				


//配置帧头
//FrameLen:帧长度
//SegSubFlag: 分段标志 0：最后一帧；1：还有后续帧
//ClientAddr: 客户机地址，目的地址
//fire--2017年6月5日21:28:26
void CreatFrameHead( unsigned char* ComBuf, unsigned short FrameLen, unsigned char SegmentFlag )
{

	*ComBuf = 0x68;
	*(ComBuf+HDLC_LenPtrL) = FrameLen % 256;
	*(ComBuf+HDLC_LenPtrH) = FrameLen / 256;
	*(ComBuf+HDLC_CtlPtr) = 0xC3;							//帧类型
	if( SegmentFlag != 0 ) *(ComBuf+HDLC_CtlPtr) |= 0x20; 	//有后续帧
	*(ComBuf+HDLC_SACtlPtr) &= 0x3F;						//响应帧的地址类型只能是单地址
}	

//生成RR帧
void CreatRRFrame( unsigned char* ComBuf, unsigned short RxdFrameNo )
{
	unsigned char Buff[6];
	unsigned short FrameLen;
	unsigned short AddrLen;
	unsigned short HDLC_DataPtr;

	FrameLen = 0;
	AddrLen = GetAddrLength( ComBuf );
	E2P_RData( Buff, EMeter_Addr, 6 );
	RAM_Write( ComBuf+HDLC_SAPPtr, Buff, AddrLen );

	HDLC_DataPtr = AddrLen + 8;						//帧头(1)、长度(2)、控制码(1)、地址头码(1)、客户机地址(1)、HCS(2)
	FrameLen += HDLC_DataPtr + 1;
	*(ComBuf+HDLC_DataPtr) = RxdFrameNo % 256;				//低字节在前
	*(ComBuf+HDLC_DataPtr+1) = RxdFrameNo / 256;
	*(ComBuf+HDLC_DataPtr+1) |= COMFIRM_FRAME;	
//	*(ComBuf+HDLC_DataPtr+1) = RxdFrameNo % 256;			//高字节在前
//	*(ComBuf+HDLC_DataPtr) = RxdFrameNo / 256;
//	*(ComBuf+HDLC_DataPtr) |= COMFIRM_FRAME;	
    FrameLen += 2;                                          //加2字节分帧格式域    
 	CreatFrameHead( ComBuf, FrameLen, SUB_FRAGMENT );
}	

//生成I帧
void CreatIFrame( unsigned char* ComBuf, unsigned short TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag, unsigned char FrameFlag )
{
	unsigned char Buff[6];
	unsigned short AddrLen;
	unsigned short HDLC_DataPtr;

	AddrLen = GetAddrLength( ComBuf );
	E2P_RData( Buff, EMeter_Addr, 6 );
	RAM_Write( ComBuf+HDLC_SAPPtr, Buff, AddrLen );
	
	HDLC_DataPtr = AddrLen + 8;						//帧头(1)、长度(2)、控制码(1)、地址头码(1)、客户机地址(1)、HCS(2)
	FrameLen += HDLC_DataPtr + 1;
	if( SegmentFlag == SUB_FRAGMENT )
	{
		*(ComBuf+HDLC_DataPtr) = TxdFrameNo % 256;	            //低字节在前
		*(ComBuf+HDLC_DataPtr+1) = TxdFrameNo / 256;
		*(ComBuf+HDLC_DataPtr+1) |= FrameFlag;	
//		*(ComBuf+HDLC_DataPtr+1) = TxdFrameNo % 256;	        //高字节在前
//		*(ComBuf+HDLC_DataPtr) = TxdFrameNo / 256;
//		*(ComBuf+HDLC_DataPtr) |= FrameFlag;	
        FrameLen += 2;                                          //加2字节分帧格式域    
	}
 	CreatFrameHead( ComBuf, FrameLen, SegmentFlag );
}	
*/

void HDLC_RRFrameProcess( unsigned char* ComBuf )
{

	unsigned short COM_No;
	unsigned char* RxdFrameNo;
	unsigned char* LLCState;

	COM_No = GetCOM_No(ComBuf);	
	
	RxdFrameNo = CommPara[COM_No].RxdFrameNo;
	LLCState = CommPara[COM_No].LLCState;

	if( *LLCState == LLCIdle )
	{
		*RxdFrameNo = 0;
		CreatRRFrame( ComBuf, *RxdFrameNo );				//电表没有准备上传帧
	}
	else if( *LLCState == WaitRecNextState )
	{
		*RxdFrameNo += 1;
		*RxdFrameNo &= 0x3F;								//接收帧序号加1
		CreatRRFrame( ComBuf, *RxdFrameNo );				//请主站下发下一帧
	}
	else if( *LLCState == SendNextState )
	{
		*RxdFrameNo &= 0x3F;								//接收帧序号维持不变
		LLCStateCycle( COM_No );							//电表上传帧
	}	
	
}	


unsigned short GetMaxInfoSize( unsigned short COM_No )
{
	if( COM_No == 0 ) return MAX_INFRA_INFO_SIZE-25;
	else if( COM_No == 1 ) return MAX_485_INFO_SIZE-25;
#if ( CarryComm == YesCheck )			//带载波通道与485口2共用通讯缓存
	else return MAX_CARRY_INFO_SIZE-25;
#else
	else return MAX_485_INFO_SIZE-25;
#endif
}	
// EndFlag==0;开始 EndFlag ==1 结束
//包头：E6 E7 00   C4 02 C1    00 00 00   00 01 00   82 XX XX
short CreatGetDataObjectListHead( short COM_No, unsigned char* Dest, unsigned char BlockEndFlag )   
{
	unsigned char Buff[10];
	unsigned char* Ptr;
	unsigned long* BlockNumber;
	
	Ptr = Buff;
	
	BlockNumber = CommPara[COM_No].BlockNumber;
	
	*Dest = 0xE6;
	*(Dest+1) = 0xE7;
	*(Dest+2) = 0;										
	*(Dest+3) = 0xc4;
	*(Dest+4) = 0x02;
	
	if(((HComm.Send_state_InvokeId&0xf0)==0xc0) || ((HComm.Send_state_InvokeId&0xf0)==0x80))
	{
			if(COM_No==0) *(Dest+5) =  HComm.Send_state_InvokeId;		
			else *(Dest+5) =  HComm.Send_state_InvokeId;			  //需要按照实际进行替换？？？？？test
  }
  else *(Dest+5) = 0xc1;
  		
	if(BlockEndFlag!=0) *(Dest+6) = 0x01;    //last-block=0表示不是最后一帧数据，后续还有数据块。
	else *(Dest+6) = 0;
		
	*BlockNumber += 1;
	RAM_Write( Ptr, (unsigned char*)BlockNumber, 4 );
	SwapData( Ptr, 4 );
	RAM_Write( Dest+7, Ptr, 4 );			//block-number 块数，=1为传输的第一块。
	*(Dest+11) = 0x00;								//result选择0（raw-data），以下数据采用A-XDR编码成为OCTETSTRING，因为在ASN.1中没有规定该OCTETSTRING的长度，所以以下的OCTETSTRING采用变长方式编码（A-XDR编码规定）
	*(Dest+12) = 0x82;								//最高BIT位1表示该字节为长度域编码，02表示其后两个字节为长度值
	*(Dest+13) = 0x00;
	*(Dest+14) = 0x00;								//数据长度 
    
   return 15;
}

void LLCStateCycle( unsigned short COM_No )
{
#if 0 
	unsigned char* RxdFrameNo;
	unsigned char* TxdFrameNo;
	unsigned short* SendDataPtr;
	unsigned short* SendSum;
	unsigned char* SendBuff;
	unsigned char* LLCState;
	unsigned char* ComBuf;
	unsigned char SegmentFlag;
	unsigned char* COSEM_BlockState; 
	unsigned short FrameLen,i;
	
	unsigned short RepeatLEN = MAX_INFO_SIZE + 20;
	unsigned short SendMAX_INFO;
	unsigned char * RepeatPtr;				
	//unsigned long* BlockNumber;


	//BlockNumber = CommPara[COM_No].BlockNumber;
    RepeatPtr = HComm.RepeatBuff;
    
    LLCState = CommPara[COM_No].LLCState;
    COSEM_BlockState = &HComm.COSEM_BlockState[COM_No];
      	
/*  if((COM_No==2)&&(crypt.GPRS_STAND == 0x0B)) //GPRS/PLC通道
  {
 		  //加密时，需要带加密参数:7个字节：0xCx +0xXX +0x20 + 0x01 + 0x23 +0x45 + 0x67..
		  if((crypt.type[COM_No] == 0x20)||(crypt.type[COM_No] == 0x30)||(crypt.type[COM_No] == 0x10)){
		  	if(crypt.type[COM_No] == 0x20) SendMAX_INFO = GPRS_MAX_INFO_SIZETX - 10;
		  	else SendMAX_INFO = GPRS_MAX_INFO_SIZETX - 22;
			}else{
							SendMAX_INFO = GPRS_MAX_INFO_SIZETX;
						}
	}else
	*/
	//{
		  //加密时，需要带加密参数:7个字节：0xCx +0xXX +0x20 + 0x01 + 0x23 +0x45 + 0x67..
		  if((crypt.type[COM_No] == 0x20)||(crypt.type[COM_No] == 0x30)||(crypt.type[COM_No] == 0x10)){
		  	if(crypt.type[COM_No] == 0x20) SendMAX_INFO = MAX_INFO_SIZE - 10;
		  	else SendMAX_INFO = MAX_INFO_SIZE - 22;
			}else{
							SendMAX_INFO = MAX_INFO_SIZE;
						}
	//}
     
	switch( *LLCState )	
	{
		case LLCIdle:				// LLC空闲
			break;
		case WaitRecNextState:		// LLC等待
			break;
		case SendNextState:			// 发送未完
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

		//	if( (*SendSum) > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			if( (*SendSum) > SendMAX_INFO ) FrameLen = SendMAX_INFO;		
			else FrameLen = *SendSum;
//			if( FrameLen > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				HComm.APPL_A8_BUSY[COM_No] |= 0x80;		//分帧发送操作状态..
				SegmentFlag = SUB_FRAGMENT;
				*LLCState = SendNextState;
			}	
			else 
			{
				HComm.APPL_A8_BUSY[COM_No] &= ~0x80;	//分帧发送分帧结束..						
				SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
			}	
			RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	//写入要发送的数据
			*SendDataPtr += FrameLen;
			//加密后发送操作..
			Decrypt_OR_Encrypt(COM_No,(ComBuf+HDLC_DataPtr),&FrameLen,1);  // 加密sky20140624..
			//函数里判断是否需要加密..
			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			*TxdFrameNo += 1;
			*TxdFrameNo &= 0x07;
			FrameLen +=14;                                                                
			for(i=0;i<FrameLen;i++) *(RepeatPtr+i) = *(ComBuf+i);  //chongfa
			*(RepeatPtr+RepeatLEN) = FrameLen;					
			break;
		case UISendNextState:			// UI发送未完
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
//			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
//			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

			if( (*SendSum) > SendMAX_INFO ) FrameLen = SendMAX_INFO;
			else FrameLen = *SendSum;
//			if( FrameLen > MAX_INFO_SIZE ) FrameLen = MAX_INFO_SIZE;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				HComm.APPL_A8_BUSY[COM_No] |= 0x80;		//分帧发送忙操作状态..				
				SegmentFlag = SUB_FRAGMENT;
				*LLCState = UISendNextState;
			}	
			else 
			{
				HComm.APPL_A8_BUSY[COM_No] &= ~0x80;	//分帧发送分帧结束..		
				SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
			}	
			RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
			*SendDataPtr += FrameLen;
//			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			//加密后发送操作..
			Decrypt_OR_Encrypt(COM_No,(ComBuf+HDLC_DataPtr),&FrameLen,1);  // 加密sky20140624..
			//函数里判断是否需要加密..
			CreatUIFrame( ComBuf, FrameLen, SegmentFlag );
//			*TxdFrameNo += 1;
//			*TxdFrameNo &= 0x07;
			FrameLen +=14;                                                                
			for(i=0;i<FrameLen;i++) *(RepeatPtr+i) = *(ComBuf+i);  //chongfa
			*(RepeatPtr+RepeatLEN) = FrameLen;							
			break;
				
	 case UIBlockSendNextState:			// 大于128个字节/或续包则进行-UI分包发送未完//修改扩充128到300..
	
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
			SendSum = CommPara[COM_No].SendByteSum;

			SegmentFlag = LAST_FRAGMENT;   //分包都采用A0，而非A8操作..
			
			if(((*SendSum) > SendMAX_INFO )&&(*COSEM_BlockState==DATABLOCKIDLE))
			{
				*COSEM_BlockState = NEW_BLOCK;
			}
			else
			{
				if(( (*SendSum) <= SendMAX_INFO )&&((*COSEM_BlockState==DATABLOCKIDLE)||(*COSEM_BlockState==FIRSTDATABLOCK)||(*COSEM_BlockState==MIDDATABLOCK)||(*COSEM_BlockState==ENDDATABLOCK)))  //一帧可以传输完，无需再分包..
						{
							  FrameLen = *SendSum;
							  
								*SendSum -= FrameLen;
			
						//		SegmentFlag = LAST_FRAGMENT;
								*LLCState = LLCIdle;
								*CommPara[COM_No].RecDataPtr = 0;
								RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
								*SendDataPtr += FrameLen;	
									//加密后发送操作..
								Decrypt_OR_Encrypt(COM_No,(ComBuf+HDLC_DataPtr),&FrameLen,1);  // 加密sky20140624..
								//函数里判断是否需要加密..							
								CreatUIFrame( ComBuf, FrameLen, SegmentFlag );
								*COSEM_BlockState = DATABLOCKIDLE;
								FrameLen +=14;                                                                
								for(i=0;i<FrameLen;i++) *(RepeatPtr+i) = *(ComBuf+i);  //chongfa
								*(RepeatPtr+RepeatLEN) = FrameLen;							
								return;			//返回..
						}
			}
			
	 //分包分标志看是否需要填加包头，重新计算包的字节数..
	 if( (*SendSum) > (SendMAX_INFO-15) )       //再包头：C4 02 C1 00 00 00 00 01 00 82 XX XX
	 		{
				 FrameLen = (SendMAX_INFO-15);
				 *SendSum -= FrameLen;
				 
//				 switch( *COSEM_BlockState )	
//					{
//						case FIRSTDATABLOCK:				// 第一块首传..
//						case MIDDATABLOCK:          // 中间块首传..
//						case ENDDATABLOCK:					// 最后一块传输..														
//							break;
//						case FIRSTMIDDATABLOCK:				// 非最后一块中间传输..
//						case ENDMIDDATABLOCK:					// 最后一块中间传输..
//						case NEW_BLOCK:							  // 单独的数据块..
//							break;							
//					default:
//					  	*COSEM_BlockState = 0;
//					  	break;	
//					}
						//包头：E6 E7 00   C4 02 C1    00 00 00   00 01 00   82 XX XX
				 if((*COSEM_BlockState==FIRSTDATABLOCK)||(*COSEM_BlockState==MIDDATABLOCK)||(*COSEM_BlockState==ENDDATABLOCK))   //首块传输..
				 {
				 	  if(*COSEM_BlockState==ENDDATABLOCK)
				 	  {
				 	  		*COSEM_BlockState = ENDMIDDATABLOCK;				 	  		
				 	  }
				 	  else
				 	  {
				 	  		*COSEM_BlockState = FIRSTMIDDATABLOCK;				 	  		
				 	  }
				 	  // 标志改变..
					 	if(*(SendBuff+*SendDataPtr+6) != 0) *(SendBuff+*SendDataPtr+6) = 0;    // 结束标志未完..
					 	//帧长度计算..
					 	if(( *(SendBuff+*SendDataPtr+12) & 0x80 ) != 0 )
							{
								if( *(SendBuff+*SendDataPtr+12) == 0x81 )										//数据长度是否为1字节 
								{
									*(SendBuff+*SendDataPtr+13) = (FrameLen-14);							//有效设置数据长度									
								}			
								else 
								{
//									*(SendBuff+*SendDataPtr+13) = 0;   //超过255，出错..
//									*(SendBuff+*SendDataPtr+14) = (FrameLen-15);

									*(SendBuff+*SendDataPtr+13) = (FrameLen-15)/ 256;   //修改20160305iran..
									*(SendBuff+*SendDataPtr+14) = (FrameLen-15)% 256;

								}	
							}	
							else
							{
								*(SendBuff+*SendDataPtr+12) = FrameLen - 13;
							}	
						RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );
				    *SendDataPtr += FrameLen;
				 }
				 else
				 	{
				 		//创建数据块包头..
				 		CreatGetDataObjectListHead( COM_No, ComBuf+HDLC_DataPtr,0 );
				 		//*(ComBuf+HDLC_DataPtr+14) = FrameLen; 
				 		// if(*COSEM_BlockState == NEW_BLOCK) *COSEM_BlockState = ENDMIDDATABLOCK;    //错误更正20140620..
				 		//   RAM_Write( ComBuf+HDLC_DataPtr+15, SendBuff+*SendDataPtr, FrameLen );
				 		if(*COSEM_BlockState == NEW_BLOCK){
				 				*COSEM_BlockState = ENDMIDDATABLOCK; 
				 				*(ComBuf+HDLC_DataPtr+14) = FrameLen-7;   
				 				RAM_Write( ComBuf+HDLC_DataPtr+15, SendBuff+*SendDataPtr+7, FrameLen-7 );
				 				*SendDataPtr += FrameLen;
				     		//FrameLen += 15;
				     		FrameLen += 8;
				 		}else{
				 					*(ComBuf+HDLC_DataPtr+14) = FrameLen;   				 		
				 	   			RAM_Write( ComBuf+HDLC_DataPtr+15, SendBuff+*SendDataPtr, FrameLen );
				 	   			*SendDataPtr += FrameLen;
				     			FrameLen += 15;
				 	   		 }

				 	}
			}					
			else
			{
				 FrameLen = *SendSum;				 
				 *SendSum -= FrameLen;
				 				 		
				 //结束帧--创建数据块包头..
				 if(*COSEM_BlockState == FIRSTMIDDATABLOCK) 	CreatGetDataObjectListHead( COM_No, ComBuf+HDLC_DataPtr,0 );
				 else 	CreatGetDataObjectListHead( COM_No, ComBuf+HDLC_DataPtr,1 );
				 *(ComBuf+HDLC_DataPtr+14) = FrameLen; 
				 
				 RAM_Write( ComBuf+HDLC_DataPtr+15, SendBuff+*SendDataPtr, FrameLen );
				 *SendDataPtr += FrameLen;
				 FrameLen += 15;
			}
			
			if( *SendSum > 0 ) 
			{	
				// SegmentFlag = LAST_FRAGMENT;   							//分包都采用A0，而非A8操作..
				*CommPara[COM_No].RecDataPtr = 0;
				*LLCState = UIBlockSendNextState;
			}	
			else          //*SendSum == 0
			{
				//SegmentFlag = LAST_FRAGMENT;
				*LLCState = LLCIdle;
				*CommPara[COM_No].RecDataPtr = 0;
				*COSEM_BlockState = DATABLOCKIDLE;
			}
			//加密后发送操作..
			Decrypt_OR_Encrypt(COM_No,(ComBuf+HDLC_DataPtr),&FrameLen,1);  // 加密sky20140624..
			//函数里判断是否需要加密..											
			CreatUIFrame( ComBuf, FrameLen, SegmentFlag );    //构建UI帧..
			FrameLen += 14;
			for(i=0;i<FrameLen;i++) *(RepeatPtr+i) = *(ComBuf+i);  //chongfa
			*(RepeatPtr+RepeatLEN) = FrameLen;
			PushSetup.SendNumber = 0;	
			break;
		default:
		  	*LLCState = LLCIdle;
		  	PushSetup.SendNumber  = 0;
		  	break;	
	}
#endif	
}
short DLMS_CommModeRec( short COM_No )
{
	unsigned char Buff[35];
	unsigned char* Point;
	unsigned char XorCheck;
	short CheckAds;
	unsigned short i;
  	short Temp;
	unsigned long Value;

  	unsigned char* ComBuf;
  	unsigned char* ComMode;
  	unsigned short* ComPtr;
  	unsigned char* ComRun;
  	unsigned char* ComState;
  	unsigned char* MACState;
  	ComBuf = CommPara[COM_No].Buf;
  	ComMode = CommPara[COM_No].Mode;
  	ComPtr = CommPara[COM_No].Ptr;
  	ComRun = CommPara[COM_No].Run;
  	ComState = CommPara[COM_No].IECState;
	MACState = CommPara[COM_No].MACState;
	Point = Buff;
	switch( *ComState )
	{
		case IECModeHDLCRXOver:		// IEC通讯模式E,HDLC帧结束
				if( DLMS_HDLC_CheckFrame( COM_No,ComBuf ) != 0 ) 
				{
					*ComState = IECModeHDLC;
					DLMS_CommInit( COM_No );
				 	return 0;					
				}
				
				if( JudgeHDLC_FRMR( ComBuf ) != 0 )
				{
					FillHCS_FCS_Flag( ComBuf );
					break;						
				}	
				HDLC_Ctrl( ComBuf );
				if( *ComBuf == 0x7E )
				{
					FillHCS_FCS_Flag( ComBuf );
				}
				else
				{
					*ComState = IECModeHDLC;
					DLMS_CommInit( COM_No );
				 	return 0;					
				}
				
				break;		
		default: DLMS_CommInit( COM_No );
				 return 0;
	}	
	*ComMode += 1;
	*ComRun &= ~F_ComOK; 
	*ComPtr = 1;
	HT_FreeDog();
	switch( COM_No )
	{
		case 0: 
		//	HT_GPIOA->PTDIR &= ~GPIOA_PLC_SET;//PDIR_SWZB |= P_SWZB;	
		//	HT_UART0->UARTSTA &= ~UART_UARTSTA_RXIF;//UCA2IFG &= ~UCTXIFG;				//13.05.17
			HT_UART0->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
			HT_UART0->SBUF = *ComBuf;//UCA2TXBUF = 0xFE;			
			break;			
		case 1:	
			HT_UART1->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);	
			HT_UART1->SBUF = *ComBuf;//UCA1TXBUF = 0xFE;

			break;			
		case 2:	
			HT_GPIOE->PTCLR |= SW485_1;//PDIR_SWZB |= P_SWZB;	
		//	HT_UART3->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);//UCA2IFG &= ~UCTXIFG;				//13.05.17
			HT_UART3->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
			HT_UART3->SBUF = *ComBuf;//UCA2TXBUF = 0xFE;			
			break;		
		case 3:	
			HT_GPIOE->PTCLR |= SW485_2;//PDIR_SWZB |= P_SWZB;	
		//	HT_UART4->UARTSTA &= ~(UART_UARTSTA_RXIF|UART_UARTSTA_TXIF);//UCA2IFG &= ~UCTXIFG;				//13.05.17
			HT_UART4->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
			HT_UART4->SBUF = *ComBuf;//UCA2TXBUF = 0xFE;			
			break;			
		default:break;				
	}	
	return 0;
}
#if 0
void LLCStateCycle( unsigned short COM_No )
{
//	unsigned short* RxdFrameNo;
	unsigned short* TxdFrameNo;
	unsigned short* SendDataPtr;
	unsigned short* SendSum;
	unsigned char* SendBuff;
	unsigned char* LLCState;
	unsigned char* ComBuf;
	unsigned char SegmentFlag;
	unsigned char FrameFlag;
	unsigned short FrameLen;
	unsigned short HDLC_DataPtr;
	unsigned short MaxInfoSize;
	unsigned short AddrLen;
    
    LLCState = CommPara[COM_No].LLCState;
    
	switch( *LLCState )	
	{
		case LLCIdle:				// LLC空闲
			break;
		case WaitRecNextState:		// LLC等待
			break;
		case SendNextState:			// 发送未完
			ComBuf = CommPara[COM_No].Buf;
			SendDataPtr = CommPara[COM_No].SendDataPtr;
			SendBuff = CommPara[COM_No].SendDataBuf;	
			TxdFrameNo = CommPara[COM_No].TxdFrameNo;
//			RxdFrameNo = CommPara[COM_No].RxdFrameNo;
			SendSum = CommPara[COM_No].SendByteSum;

			MaxInfoSize = GetMaxInfoSize( COM_No );
			if( (*SendSum) > MaxInfoSize ) FrameLen = MaxInfoSize;
			else FrameLen = *SendSum;
			*SendSum -= FrameLen;
			if( *SendSum > 0 ) 
			{
				SegmentFlag = SUB_FRAGMENT;						//分帧返回
				if( *TxdFrameNo == 0 )
				{
					FrameFlag = START_FRAME;					//起始帧
				}	
				else FrameFlag = SUB_FRAME;						//中间帧
				*LLCState = SendNextState;
			}	
			else 
			{
				if( *TxdFrameNo == 0 )
				{ 
					SegmentFlag = LAST_FRAGMENT;				//不分帧
					*LLCState = LLCIdle;
					*CommPara[COM_No].RecDataPtr = 0;
				}
				else
				{
					SegmentFlag = SUB_FRAGMENT;					//分帧
					FrameFlag = LAST_FRAME;						//最后一帧
					*CommPara[COM_No].RecDataPtr = 0;
				}		
			}	
			
			AddrLen = GetAddrLength( ComBuf );
			HDLC_DataPtr = AddrLen + 8;						//帧头(1)、长度(2)、控制码(1)、地址头码(1)、客户机地址(1)、HCS(2)
			if( SegmentFlag == SUB_FRAGMENT )
			{
				RAM_Write( ComBuf+HDLC_DataPtr+2, SendBuff+*SendDataPtr, FrameLen );	//带分帧格式域2字节
			}
			else	
			{
				RAM_Write( ComBuf+HDLC_DataPtr, SendBuff+*SendDataPtr, FrameLen );	
			}	
			*SendDataPtr += FrameLen;
//			CreatIFrame( ComBuf, *RxdFrameNo, *TxdFrameNo, FrameLen, SegmentFlag );
			CreatIFrame( ComBuf, *TxdFrameNo, FrameLen, SegmentFlag, FrameFlag );
			*TxdFrameNo += 1;
			*TxdFrameNo &= 0x3F;
			break;
		default:
		  	*LLCState = LLCIdle;
		  	break;	
	}	
}
#endif
