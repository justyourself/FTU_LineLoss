#include "ht6xxx.h"

//#define TM_120s	120
//#define TM_120s	6
//#define TM_120s	15
/**********************************
	Baut rate Define (SMCLK=4M)
**********************************/	
//#define Baut_300L	0x9D	
#define Baut_300L	0x9f	
#define Baut_300H	0x36
//#define Baut_300M	0xFF
//#define Baut_300M	0x55
#define Baut_300M	0x00

#define Baut_600L	0x4E
#define Baut_600H	0x1B
//#define Baut_600M	0x55
#define Baut_600M	0x00

#define Baut_1200L	0xA6
#define Baut_1200H	0x0D
//#define Baut_1200M	0x0D
#define Baut_1200M	0x00

#define Baut_2400L	0xD3
#define Baut_2400H	0x06
//#define Baut_2400M	0xEF
#define Baut_2400M	0x00

//#define Baut_2400L	0x05
//#define Baut_2400H	0x0D
//#define Baut_2400M	0x6B

//#define Baut_4800L	0xD3
//#define Baut_4800H	0x06
//#define Baut_4800M	0xEF

#define Baut_4800L	0x69
#define Baut_4800H	0x03
//#define Baut_4800M	0xEF
#define Baut_4800M	0x00

#define Baut_9600L	0xB4
#define Baut_9600H	0x01
//#define Baut_9600M	0xFF
#define Baut_9600M	0x00

#define Baut_57600L	0x5A
#define Baut_57600H	0x00

#define Baut_19200L	0xDA
#define Baut_19200H	0x00
//#define Baut_19200M	0x55
#define Baut_19200M	0x00

#define Baut300  0
#define Baut600  1
#define Baut1200 2
#define Baut2400 3
#define Baut4800 4
//#define Baut7200 5
//#define Baut9600 6
//#define Baut19200 7
#define Baut9600  5
#define Baut19200 6
#define Baut38400 7
#define Baut57600 8
#define Baut115200 9

#define IECIdle				  0	  // IEC通讯空闲
#define IECStartCode		  1	  // IEC通讯起始
#define IECStartCodeOver	  2	  // IEC通讯起始帧结束
#define IECBandRate			  3	  // IEC通讯切换波特率
#define IECBandRateOver		  4	  // IEC通讯切换波特率帧结束
#define IECModeC			  5	  // IEC通讯模式C
#define IECModeCOver		  6	  // IEC通讯模式C帧结束
#define IECModeCRXOver		  7	  // IEC通讯模式C帧结束	
#define IECModeHDLC			  8	  // IEC通讯模式E,HDLC
#define IECModeHDLCOver		  9	  // IEC通讯模式E,HDLC帧结束
#define IECModeHDLCRXOver	  10	// IEC通讯模式E,HDLC帧结束
#define IECModeExit			  11	// IEC通讯退出

//MACState						      //通讯口1MAC通讯过程状态
#define	NDMState			  0	// MAC未连接
#define	NRMState			  1	// MAC已连接		

//LLCState						          // 通讯口1MAC通讯过程状态
#define	LLCIdle				  0	// LLC空闲
#define	WaitRecNextState	  1	// LLC等待
#define	SendNextState		  2	// 发送未完
#define	UISendNextState		  3	// UI帧发送未完
#define	UIBlockSendNextState  4 //UI帧分包发送未完..
//APPLState						// 通讯口1APPL连接状态
#define	APPLIdle			  0	// APPL空闲
#define	APPLON				  1	// APPL已连接

#define F_ComOK		0x01

#define HDLC_I 		0x10
#define HDLC_RR 	0x11
#define HDLC_RNR	0x15
#define HDLC_SNRM	0x93
#define HDLC_DISC	0x53
#define HDLC_UA		0x73
#define HDLC_DM		0x1F
#define HDLC_FRMR	0x97
#define HDLC_UI		0x13
#define HDLC_UI_N	0x03		//无回码

//#define RS_MinSize   12
//#define RS_MinSize   14					//12.04.07
//#define RRx_MaxSize  RS_MinSize+128			

//#define HDLC_HeadPtr     	0
//#define HDLC_LenPtrH      	1
//#define HDLC_LenPtrL      	2
//#define HDLC_Addr0Ptr     	3
//#define HDLC_SClientAddr	3		//发送的客户地址
//#define HDLC_RClientAddr	7		//接收到的客户地址
//#define HDLC_CtlPtr      	8
//#define HDLC_HCSPtr	      	9
//#define HDLC_DataPtr    	11	
/*
#define HDLC_HeadPtr     	0
#define HDLC_LenPtrL      	1
#define HDLC_LenPtrH      	2
//#define HDLC_CtlPtr      	3
#define HDLC_SACtlPtr      	4
#define HDLC_SAPPtr      	5
#define HDLC_RClientAddr	7		//接收到的客户地址
#define HDLC_CtlPtr      	8
#define HDLC_HCSPtr	      	9
#define HDLC_DataPtr    	11	
*/
#define HDLC_HeadPtr     	0
#define HDLC_LenPtrH      	1
#define HDLC_LenPtrL      	2
#define HDLC_Addr0Ptr     	3
#define HDLC_SClientAddr	3		//发送的客户地址
#define HDLC_RClientAddr	7		//接收到的客户地址
#define HDLC_CtlPtr      	8
#define HDLC_HCSPtr	      	9
#define HDLC_DataPtr    	11	

#define LAST_FRAGMENT		0		//不分帧
#define SUB_FRAGMENT		1		//分帧

#define START_FRAME			0x00	//分帧中的起始帧
#define LAST_FRAME			0x40	//分帧中的最后帧
#define COMFIRM_FRAME		0x80	//分帧中的确认帧
#define SUB_FRAME			0xC0	//分帧中的中间帧

//#define MAX_INFO_SIZE		200
#define MAX_INFRA_INFO_SIZE	200
//#define MAX_INFRA_INFO_SIZE	100
#define MAX_485_INFO_SIZE	512
//#define MAX_485_INFO_SIZE	100
#define MAX_CARRY_INFO_SIZE	255
//#define MAX_CARRY_INFO_SIZE	100
#define MAX_INFO_SIZE		230//128//128
#define GPRS_MAX_INFO_SIZE	128//256 

#define RS_MinSize   14					//12.04.07
#define RRx_MaxSize  RS_MinSize+128			

#define MAX_485_SIZE	 128
#define RRx_485_MaxSize  RS_MinSize+MAX_485_SIZE			

#define MAX_PDU_SIZE		512//2000

#define MAX_BLOCK_SIZE		2048
//块传输标志..
#define	 DATABLOCKIDLE    		0     		// 无后续：0
#define	 FIRSTDATABLOCK   		1					// 最初第一块传输..
#define	 MIDDATABLOCK     		2					// 数据块中间传输..
#define	 ENDDATABLOCK     		3					// 最后一块传输..
#define	 NEW_BLOCK        		4     		// 无数据块分帧需要重新打成数据块包进行传输..
#define	 FIRSTMIDDATABLOCK      5					// 数据块中间传输..
#define	 ENDMIDDATABLOCK     	6					// 最后一块数据块中间传输..		

/*
short ASC_BCD(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
short BCD_ASC(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
short GetBandRate( unsigned char BandRate );
unsigned char GetTXDelay( unsigned char* Source );
void CommInit( short COM_No );
void CommRest( short COM_No );  		//恢复至初始状态
short GetXorCheck( unsigned char* ComBuf, unsigned char* XorCheck, short* CheckAds);
void CommMode( short COM_No );
short CommModeRec( short COM_No );
unsigned short MP_FCS(unsigned short fcs,unsigned short c);
unsigned short GetCheckCode_16BitFcs(unsigned char *pucData,unsigned short usDataLen);
unsigned short GetFrameLength( unsigned char* ComBuf );
void FillHCS_FCS_Flag( unsigned char* ComBuf );
unsigned short GetMeterAddress( unsigned char* ComBuf, unsigned char* ClientAddr, unsigned short* ServerAddrHigh, unsigned short* ServerAddrLow );
void Put16BitFcs( unsigned char* Source, unsigned char* Dest, unsigned short Len );
unsigned char HDLC_CheckFrame( unsigned char* ComBuf );
unsigned char GetControlByte( unsigned char SControlByte );
unsigned short GetInformationField( unsigned char* Dest );
void CreatFrameHead( unsigned char* ComBuf, unsigned char ClientAddr, unsigned short FrameLen, unsigned char SegmentFlag );
void CreatUAFrame( unsigned char* ComBuf );
void CreatDMFrame( unsigned char* ComBuf );
void CreatRRFrame( unsigned char* ComBuf, unsigned char RxdFrameNo );
void CreatIFrame( unsigned char* ComBuf, unsigned char RxdFrameNo, unsigned char TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag );
unsigned short GetHDLCParaValue( unsigned char* Source, unsigned short* Len, unsigned long* Value );
unsigned char JudgeHDLCParaInformationField( unsigned char* Source, short Length );
unsigned char JudgeHDLC_FRMR( unsigned char* ComBuf );
void ClrRSNumber( unsigned char* ComBuf );
void ClrRSBuffPara( unsigned char* ComBuf );
void HDLC_Ctrl(unsigned char* ComBuf);
void HDLC_InformationFrameProcess( unsigned char* ComBuf );
void HDLC_RRFrameProcess( unsigned char* ComBuf );
void LLCStateCycle( unsigned short COM_No );
void HDLC_UIFrameProcess( unsigned char* ComBuf );
*/

short ASC_BCD(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
short BCD_ASC(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
short GetBandRate( unsigned char BandRate );
unsigned char GetTXDelay( unsigned char* Source );
short GetCOM_No( unsigned char* ComBuf );
unsigned char* GetComBuf( unsigned short COM_No );
//void CommInit( short COM_No );
void CommRest( short COM_No );  		//恢复至初始状态
unsigned short MP_FCS(unsigned short fcs,unsigned short c);
unsigned short GetCheckCode_16BitFcs(unsigned char *pucData,unsigned short usDataLen);
void Put16BitFcs( unsigned char* Source, unsigned char* Dest, unsigned short Len );
void ClrRSNumber( unsigned char* ComBuf );
void ClrRSBuffPara( unsigned char* ComBuf );
void HDLC_RNRFrameProcess( unsigned char* ComBuf );
unsigned short GetFrameLength( unsigned char* ComBuf );
unsigned short GetAddrLength( unsigned char* ComBuf );
void FillHCS_FCS_Flag( unsigned char* ComBuf );
void HDLC_Ctrl(unsigned char* ComBuf);
void HDLC_InformationFrameProcess( unsigned char* ComBuf );
//void CreatFrameHead( unsigned char* ComBuf, unsigned short FrameLen, unsigned char SegmentFlag );
void CreatFrameHead( unsigned char* ComBuf, unsigned char ClientAddr, unsigned short FrameLen, unsigned char SegmentFlag );

//void CreatRRFrame( unsigned char* ComBuf, unsigned short RxdFrameNo );
void CreatRRFrame( unsigned char* ComBuf, unsigned char RxdFrameNo );
void CreatIFrame( unsigned char* ComBuf, unsigned char RxdFrameNo, unsigned char TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag );

//void CreatIFrame( unsigned char* ComBuf, unsigned short TxdFrameNo, unsigned short FrameLen, unsigned char SegmentFlag, unsigned char FrameFlag );
void HDLC_RRFrameProcess( unsigned char* ComBuf );
unsigned short GetMaxInfoSize( unsigned short COM_No );
void LLCStateCycle( unsigned short COM_No );

short DLMS_CommModeRec( short COM_No );
void DLMS_CommMode( short COM_No );
void DLMS_CommInit(unsigned char COM_No);
void fnUART5_Init(unsigned char Cfg);

