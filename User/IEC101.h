#ifndef __IEC101_H__
#define __IEC101_H__
typedef unsigned char 	u8;
typedef unsigned int  	u16;
typedef unsigned long  	u32;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif
struct bb
{
  u8 l;
  u8 h;
};
union IEC101_ADD
{
  struct 
  {
    u16 l:8;
    u16 h:8;
  }Byte;
  u16 Word;
};

typedef union
{
  u8 Byte[4];
  u16 Word[2];
  u32 Dword;
  
}FOUR_BYTE_TO_DWORD;
union IEC101_DADD
{
  u8 Byte[4];
  u16 Word[2];
  u32 Dword;
};
struct APP_STRUCT
{
  u8 byFull;
  u8 byFunCode;
  u8 LinkFunCode;
  u8 byMsgNum;
  u8 wFrameLen;
  u8 *lpByBuf;
  u8 byLinkBuf[256];
};
struct REAPP_STRUCT
{
   u8 *lpByBuf;
};
struct IEC101_STRUCT
{
  u32 wRecvNum;
  u32 RErrorN;
  u32 initstatus;
  u16 Sn;
  u8  Fop;  //文件操作标识
  u8  Fname[32];
  u32 FId;
  u16 List_para[32];
  union IEC101_ADD wRecvCmmAdd;
  u32 FtuSccReset;
  u32 byFrameIntval;
  u32 wPSendNum;
  u32 PWindow;
  u32 PWinTimer;
  u32 OrgnizeFrame;
  u8  byRecvBuf[256];
  u32 wRecvLen;
  u8 PReMsgType;
  u8 byReason;
  u8 byPSGenStep;
  u8 frameno;
  u8 TypeProtocol;
  u8 byPSDdStep;
  u8 WaitYkXzRet;
  u8 WaitYkZxRet;
  u8 SendYkZxAck;
  u8 PReFrameType;
  u8 UnsolTimeInterval;
  u8 Pacd;
  u8 Pdfc;
  u8 PfcbC;
  u8 haveset;
  u8 byQualify;
  u8 TypeSeReason;
  u8 bySourceAdd;
  u8 TypeCmmAdd;
  u8 TypeInfAdd;
  u8 byQOI;
  u8 byQCC;
  u8 byDCO;
  u8 byCOI;
  u8 firstData;
  u8 FlagPingH;
  u8 YcFN;
  u8 DdFN;
  u8 YcNPF[32];
  u8 DdNPF[32];
  union IEC101_ADD wTester;
  union IEC101_DADD dwReadAd;
  union IEC101_DADD dwInfAdd;
  union IEC101_ADD wCmmAdd;
  union IEC101_ADD wLinkAdd;
  u8 TypeLinkAdd;
  struct APP_STRUCT PSeAppLayer;
  struct APP_STRUCT PSendFrame;
  struct APP_STRUCT PRecvFrame;
  struct REAPP_STRUCT PReAppLayer; 
};

/***************************************************************************
					IEC-870-5-101预定义头文件
***************************************************************************/

//启动方向 链路层功能码定义
#define RESET_LINK		0		// 复位远方链路
#define RESET_SESSION		1     		// 复位远动终端的用户进程
#define TRAN_CONFIRM_DATA       3  
#define TRAN_DATA		4	      // 传送数据/无回答帧
#define CALL_ACD		8	      		// 响应帧应说明访问要求
#define CALL_LINK		9		// 召唤链路状态
#define CALL_DATA1		10		// 召唤用户1级数据
#define CALL_DATA2		11		// 召唤用户2级数据
#define IEC101_SWITCH_PROTOCOL  15	//由101规约转换为维护规约
//从动方向 链路层功能码定义
#define YES_ACK                 0     //肯定确认
#define NO_ACK                  1     //否定确认
#define RESP_DATA               8     //以数据响应
#define RESP_NO_DATA            9     //无数据
#define LINK_GOOD               11    //链路良好
#define LINK_NO_WORK            14    //链路服务未工作
#define LINK_NO_OVER            15    //链路服务未完成
#define LINK_BUSY               1     //链路忙
// 变长帧类型标识定义

// 子站→主站过程信息
#define M_SP_NA_1	1		// 不带时标的单点信息
#define M_SP_TA_1	2		// 带时标的单点信息
#define M_DP_NA_1	3		// 不带时标的双点信息
#define M_DP_TA_1	4		// 带时标的双点信息
#define M_ST_NA_1	5		// 步位置信息(变压器分接头信息)
#define M_ST_TA_1	6		// 带时标步位置信息(变压器分接头)
#define M_BO_NA_1	7		// 子站远动终端状态
#define M_ME_NA_1	9		// 测量值
#define M_ME_TA_1	10		// 带时标的测量值
#define M_IT_NA_1	15		// 电能脉冲计数量
#define M_IT_TA_1	16		// 带时标的电能脉冲计数量
#define M_EP_TA_1	17		// 带时标的继电保护或重合闸设备单个事件
#define M_EP_TB_1	18		// 带时标的继电保护装置成组启动事件
#define M_EP_TC_1	19		// 带时标的继电保护装置成组输出电路信息事件
#define M_PS_NA_1	20		// 具有状态变位检出的成组单点信息
#define M_ME_ND_1	21		// 不带品质描述的测量值
#define M_BD_NA_1	232		// BCD码(水位值)

// 主站→子站 在控制方向的过程信息
#define C_SC_NA_1	45		// 单点遥控命令
#define C_DC_NA_1	46		// 双点遥控命令
#define C_RC_NA_1	47		// 升降命令
#define C_SE_NA_1	48		// 设定命令
#define C_BO_NA_1	51		// 32位的位串

// 子站→主站 在监视方向的系统信息
#define M_EI_NA_1	70		// 初始化结束

// 主站→子站 在控制方向的系统信息
#define GeneralCall	100		// 召唤命令C_IC_NA_1
#define C_CI_NA_1	101		// 电能脉冲召唤命令
#define C_RD_NA_1	102		// 读数据命令
#define C_CS_NA_1	103		// 时钟同步命令
#define C_TS_NA_1	104		// 测试命令
#define C_RP_NA_1	105		// 复位进程命令
#define C_CD_NA_1	106		// 延时获得命令

// 在控制方向的参数
#define P_ME_NA_1	110		// 装载参数命令
#define P_AC_NA_1	113		// 激活参数

#define C_RR_NA_1       201             //读定值区号
#define C_RS_NA_1       202             //读定值和参数
#define C_WS_NA_1       203             //写定值和参数
#define M_IT_NB_1       206
#define M_IT_TC_1       207 
#define F_SR_NA_1       211             //软件升级启动和结束

// 文件传输的类型标识
#define F_FR_NA_1       210             //文件传输
#define F_FR_DA_1       1   //读目录激活
#define F_FR_DA_2       2   //读目录确认
#define F_FR_FA_1       3    //读文件激活
#define F_FR_FA_2       4    //读文件确认
#define F_FR_FA_3       5    //读文件内容
#define F_FR_FA_4       6    //读文件内容确认
#define F_FW_FA_1       7    //写文件激活
#define F_FW_FA_2       8    //写文件确认
#define F_FW_FA_3       9    //写文件内容
#define F_FW_FA_4       10    //写文件内容确认
#if 0
#define F_FR_NA_1	120		// 文件已准备好
#define F_SR_NA_1	121		// 节已准备好
#define F_SC_NA_1	122		// 召唤目录、选择文件、召唤文件、召唤节
#define F_LS_NA_1	123		// 最后的节、最后的段
#define F_AF_NA_1	124		// 确认文件、确认节
#define F_SG_NA_1	125		// 段
#define F_DR_NA_1	126		// 目录
#endif

// 传送原因
#define PER_CYC		1		// 周期、循环
#define BACK		2		// 背景扫描
#define SPONT		3		// 突发
#define INIT		4		// 初始化
#define REQ		5		// 请求或被请求
#define ACT		6		// 激活
#define ACTCON		7		// 激活确认
#define DEACT		8		// 停止激活
#define DEACTCON	9		// 停止激活确认
#define ACTTERM		10		// 激活结束
#define RETREM		11		// 远程命令引起的返送信息
#define RETLOC		12		// 当地命令引起的返送信息
#define FILETRANS	13		// 文件传输

#define INTROGEN	20		// 响应总召唤
#define INTRO1		21		// 响应第1组召唤
#define INTRO2		22		// 响应第2组召唤
#define INTRO3		23		// 响应第3组召唤
#define INTRO4		24		// 响应第4组召唤
#define INTRO5		25		// 响应第5组召唤
#define INTRO6		26		// 响应第6组召唤
#define INTRO7		27		// 响应第7组召唤
#define INTRO8		28		// 响应第8组召唤
#define INTRO9		29		// 响应第9组召唤
#define INTRO10		30		// 响应第10组召唤
#define INTRO11		31		// 响应第11组召唤
#define INTRO12		32		// 响应第12组召唤
#define INTRO13		33		// 响应第13组召唤
#define INTRO14		34		// 响应第14组召唤
#define INTRO15		35		// 响应第15组召唤
#define INTRO16		36		// 响应第16组召唤

#define REQCOGCN	37		// 响应计数量总召唤
#define REQCO1		38		// 响应第1组计数量召唤
#define REQCO2		39		// 响应第2组计数量召唤
#define REQCO3		40		// 响应第3组计数量召唤
#define REQCO4		41		// 响应第4组计数量召唤
			 
// 信息体地址预定义

// 遥信--单点遥信占1个地址，双点遥信占2个地址
#define IEC101_YXSA		0x001		// 遥信信息体起始地址
#define IEC101_YXEA		0x400		// 遥信信息体结束地址
#define IEC101_YXNUM	        1024		// 遥信信息总个数
#define IEC101_YXEA_2002	0x1000		// 遥信信息体结束地址
#define IEC101_YXNUM_2002	4096		// 遥信信息总个数
// 继电保护单个事件--每个事件占2个地址
#define IEC101_SEPSA	0x401		// 继电保护单个事件起始地址
#define IEC101_SEPEA	0x500		// 继电保护单个事件结束地址
#define IEC101_SEPNUM	256 / 2		// 继电保护单个事件总个数

// 继电保护装置具有启动事件的事件顺序纪录--每个信息记录占8个地址
#define IEC101_SPESA	0x501		// 继电保护事件顺序记录起始地址
#define IEC101_SPEEA	0x600		// 继电保护事件顺序记录结束地址
#define IEC101_SPENUM	256 / 8		// 继电保护事件顺序记录总个数

// 继电保护装置成组输出电路信息--每个信息占4个地址
#define IEC101_PTOSA	0x601		// 继电保护成组输出电路信息起始地址
#define IEC101_PTOEA	0x700		// 继电保护成组输出电路信息结束地址
#define IEC101_PTONUM	256 / 4		// 继电保护成组输出电路信息总个数

// 遥测--每个遥测占1个地址
#define IEC101_YCSA		0x701		// 遥测信息体起始地址
#define IEC101_YCEA		0x900		// 遥测信息体结束地址
#define IEC101_YCNUM	512			// 遥测信息总个数
#define IEC101_YCSA_2002	0x4001		// 遥测信息体起始地址
#define IEC101_YCEA_2002	0x5000		// 遥测信息体结束地址
#define IEC101_YCNUM_2002	4096		// 遥测信息总个数

// 参数地址范围--和遥测信息体地址一一对应
#define IEC101_PARASA	0x901		// 参数起始地址
#define IEC101_PARAEA	0xB00		// 参数结束地址

// 遥控，升降--每个遥控对象占1个地址
#define IEC101_YKSA		0xB01		// 遥控信息体起始地址
#define IEC101_YKEA		0xB80		// 遥控信息体结束地址
#define IEC101_YKNUM	128			// 遥控信息总个数
#define IEC101_YKSA_2002		0x6001		// 遥控信息体起始地址
#define IEC101_YKEA_2002		0x6200		// 遥控信息体结束地址
// 设定--一个量占用一个信息体地址
#define IEC101_SETSA	0xB81		// 设定信息体起始地址
#define IEC101_SETEA	0xC00		// 设定信息体结束地址
#define IEC101_SETNUM	128			// 设定信息总个数

// 电能脉冲计数量--每一个量占用1个信息体地址
#define IEC101_DDSA		0xC01		// 电度信息体起始地址
#define IEC101_DDEA		0xC80		// 电度信息体结束地址
#define IEC101_DDNUM	        128			// 电度信息总个数
#define IEC101_DDSA_2002	0x6401		// 电度信息体起始地址
#define IEC101_DDEA_2002	0x6600		// 电度信息体结束地址
#define IEC101_DDNUM_2002	512			// 电度信息总个数

// 步位置信息(变压器分接头)--一个量占用一个信息体地址
#define IEC101_STEPSA	0xC81		// 步位置信息起始地址
#define IEC101_STEPEA	0xCA0		// 步位置信息结束地址
#define IEC101_STEPSA_2002	0x6601		// 步位置信息起始地址
#define IEC101_STEPEA_2002	0x6620		// 步位置信息结束地址

#define IEC101_STEPNUM	32			// 步位置信息总个数

// 二进制状态信息
#define IEC101_BINSA	0xCA1		// 二进制状态信息起始地址
#define IEC101_BINEA	0xCC0		// 二进制状态信息结束地址
#define IEC101_BINNUM	32			// 二进制状态信息总个数

// BCD码--一个量占用一个信息体地址
#define IEC101_BCDSA	0xCC1		// BCD信息起始地址
#define IEC101_BCDEA	0xCE0		// BCD信息结束地址
#define IEC101_BCDNUM	32			// BCD信息总个数

// 子站远动终端状态地址
#define IEC101_RTUSA	0xCE9

// 文件传输地址
#define IEC101_FTA		0xCEA		

// 标志字节
#define F_STARTCODE		0x10		// 定长帧起始字节
#define V_STARTCODE		0x68		// 变长帧起始字节
#define STOPCODE		0x16		// 停止字节

//遥控所用常量
#define YK_WAIT_TIMES 100	//遥控返校等待次数

//发送数量定义
//每帧长度最大为255个，去掉控制字，链路地址，类型标识，限定词，传送原因，公共地址。这些最大为
//9个字节，因此数据为255-9=246个，因此：
#define IEC101_YXBWNPF 		60	//64 //每帧最大遥信变位数 246/4=61.5
//#define IEC101_SOENPF 32		//每帧最大SOE数
#define IEC101_SOENPF_TIME56	 22		//每帧最大SOE数	 246/11= 22.36
#define IEC101_SOENPF_TIME24	 32		//每帧最大SOE数	 246/7=	35.1

#define IEC101_YCCNPF 16		//每帧最大变化遥测数
#define IEC101_YCNPF 31			//每帧最大遥测数(每组遥测分2帧传送)
#define IEC101_YXNPF 64			//每帧最大遥信数(每组遥信分2帧传送)
#define IEC101_DDNPF 8			//每帧最大电度数   246/8=30.75
#define IEC101_YCNPG 128		//每组最大遥测数(每组遥测分2帧传送)
#define IEC101_YXNPG 128	   	//每组最大遥信数(每组遥信分2帧传送)
//#define IEC101_DDNPG 32			//每组最大电度数

//处理接收错误所用常量
#define MaxErrorN 5	//最大接收错误数
#define MaxDogN 256	//最大dog数
#define MAXRESENDTIME 25 //全数据循环传送时间

//函数声明
void SearchFirstData(void);
void SendData1(void);
void SendData2(void);
void InitIEC101Prot(void);
void InitIEC101Func(u8 ChanNo);
void SettimeToCan(u8* pbyBuf);
void SendGeneralData(void);
void SendTimeAck(void);
void SendDdData(void);
void SendDdData2002(void);
void SendReadDataAck(void);
void SendYkReply(void);
void SendChanTestAck(void);
void PAppSendProcess(void);
void Iec101LinkRecvProPH(void);
u8 OrgnizeYxbwMsg(u8 *lpby);
u8 OrgnizeSoeMsg(void);
u8 OrgnizeYcOverMsg(void);
u8 OrgnizeInitEndMsg(u8*);
u8 OrgnizeReadDataMsg(u8* lpby);
u8 OrgnizeSubChgMsg(void);
u8 OrgnizeVarFrame(u8 *lpby,u8 byMsgNum,u8 byConField);
u8 OrgnizeFixFrame(u8 *lpby,u8 byConField) ;
//全局变量声明
extern struct IEC101_STRUCT *lpIEC101;	//IEC101规约私用数据指针
#endif

