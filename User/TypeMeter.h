/*
#define U220_I1A3200	0x01
#define U220_I1A6400	0x02
#define U220_I5A800		0x03
#define U220_I5A1600	0x04
#define U220_I10A400	0x05
#define U220_I10A800	0x06
#define U220_I15A400	0x07
#define U220_I15A800	0x08
#define U220_I20A200	0x09
#define U220_I20A400	0x10
#define U220_I30A200	0x11
#define U220_I30A400	0x12
#define U57_I1A12800	0x13
#define U100_I1A6400	0x14
#define U100_I1A12800	0x15
#define U57_I1A20000	0x16
*/
#define U220_I0_3A__1_2A25600   0x01
#define U220_I1A__6A6400	    0x02
#define U220_I1_5A__6A6400	    0x03
#define U220_I5A__20A800	    0x04
#define U220_I5A__30A800	    0x05
#define U220_I5A__40A400	    0x06
#define U220_I10A__40A400	    0x07
#define U220_I10A__60A400	    0x08
#define U220_I20A__80A200	    0x09
#define U57_I0_3A__1_2A100000   0x10
#define U57_I1A__6A20000	    0x11
#define U57_I1_5A__6A20000	    0x12
#define U100_I0_3A__1_2A100000  0x13
#define U100_I1A__6A20000	    0x14
#define U100_I1_5A__6A20000	    0x15

#define U220_I5A__20A1200	    0x16
#define U220_I5A__40A600	    0x17
#define U220_I10A__40A600	    0x18
#define U220_I10A__50A600	    0x19
#define U220_I20A__80A320	    0x20
#define U220_I20A__100A240	    0x21
#define U220_I20A__120A240	    0x22
#define U220_I15A__60A400	    0x23

#define U220_I5A__50A400	    0x24
#define U220_I5A__60A400	    0x25
#define U220_I5A__50A600	    0x26

#define U220_I2_5A__10A3200	    0x27
#define U220_I30A__100A240	    0x28

#define U220_I10A__100A240	    0x29

#define U220_I5A__6A6400	    0x30

#define U220_I5A__40A800	    0x31

#define U220_I10A__100A300	    0x32


#define DAYLIGHTSAVE YesCheck 	//夏令时功能
#define ACTIVE_FEE   NoCheck 		//主副费率功能

#define Phase3Wire4	0x00		//三相四线
#define Phase3Wire3	0x01		//三相三线
#define SinglePhase	0x02	    //单相
#define Threephase 	0x03		//三相
//#define Crypt_Soft     0x04
//#define Crypt_Hardware 0x05
#define MeterPhase  SinglePhase
#define METERTYPE   U220_I10A__100A300

//#define Crypt_Base Crypt_Hardware

#define LinkMode 	Phase3Wire4		////三相四线
//#define LinkMode 	Phase3Wire3		////三相三线

//#if ( LinkMode == Phase3Wire4 )							//新国网		//13.08.30	

//#define METERTYPE	U220_I0_3A__1_2A25600   
//#define METERTYPE	U220_I1A__6A6400	    
//#define METERTYPE	U220_I1_5A__6A6400	    
//#define METERTYPE	U220_I5A__20A800	    
//#define METERTYPE	U220_I5A__30A800	    
//#define METERTYPE	U220_I5A__40A400	    
//#define METERTYPE	U220_I10A__40A400	    
//#define METERTYPE	U220_I10A__60A400	    
//#define METERTYPE	U220_I20A__80A200	    
//#define METERTYPE	U57_I0_3A__1_2A100000   
//#define METERTYPE	U57_I1A__6A20000	    
//#define METERTYPE	U57_I1_5A__6A20000	    
//#define METERTYPE	U220_I5A__40A600	    

//#define METERTYPE	U220_I2_5A__10A3200	    
//#define METERTYPE	U220_I5A__50A600	    

//#define METERTYPE	U220_I1_5A__6A6400	    //外置		//CMC
//#define METERTYPE	U220_I5A__20A1200	    
//#define METERTYPE	U220_I5A__30A800	    
//#define METERTYPE	U220_I5A__40A600	    
//#define METERTYPE	U220_I5A__60A400	    
//#define METERTYPE	U220_I10A__40A600	    
//#define METERTYPE	U220_I10A__60A400	    
//#define METERTYPE	U220_I15A__60A400	    
////#define METERTYPE	U220_I10A__100A300  	//外置  
//#define METERTYPE	U220_I10A__100A240  	//外置  
//#define METERTYPE	U220_I20A__80A320	    //外置
//#define METERTYPE	U220_I20A__100A240	    //外置

//#define METERTYPE	U220_I5A__60A400	    //新国网智能表		//新国网		//13.08.30
//#define METERTYPE	U220_I1_5A__6A6400	    //新国网智能表

//#define METERTYPE	U220_I5A__40A800	    //湖北智能表
//#define METERTYPE	U220_I15A__60A400	    //湖北智能表
//#define METERTYPE	U220_I20A__80A320	    //湖北智能表

//#define METERTYPE	U220_I10A__40A600	    //安徽智能表
//#define METERTYPE	U220_I1_5A__6A6400	    //安徽智能表
//#define METERTYPE	U220_I10A__60A400	    

//#define METERTYPE	U220_I10A__40A600	    
//#define METERTYPE	U220_I10A__60A400	    
//#define METERTYPE	U220_I15A__60A400	    
//#define METERTYPE	U220_I20A__100A240	    
//#define METERTYPE	U220_I20A__80A320	    
//#define METERTYPE	U220_I30A__100A240	    
//#define METERTYPE	U220_I10A__100A240    
//#define METERTYPE	U220_I10A__100A300    

//#define METERTYPE	U220_I5A__6A6400	    //美国

//#else															//新国网		//13.08.30		
//#define METERTYPE	U100_I0_3A__1_2A100000  
//#define METERTYPE	U100_I1A__6A20000	    
//#define METERTYPE	U100_I1_5A__6A20000	    //安徽智能表		//新国网		//13.08.30
//#define METERTYPE	U100_I1_5A__6A20000	    //山东智能表
//#endif														//新国网		//13.08.30	

//#define CMonLength		560			//历史月长度
#define CMonECLength		822			//历史月电量长度
//#define CMonMDLength		999			//历史月需量长度
#define CMonMDLength		936			//历史月需量长度
//#define FreezeVarLength		74			//瞬时量冻结长度
#define FreezeVarLength		77			//瞬时量冻结长度
#define FREEZE_LEN			CMonECLength+CMonMDLength+FreezeVarLength	//冻结长度
//#define E2P_PGLEN		0x2800		//数据备份页长度		//PWMD
//#define E2P_PGLEN		0x7000		//数据备份页长度		
#define FRAM_PGLEN		0x400		//数据备份页长度		

#define ProfEAds    0xA8	//负荷曲线 + 事件记录 EEPROM 24C512
//国网三相表配置
#define DataEAds    0xA4	//数据 EEPROM 24C512
//#define AdjEAds     0xA0	//校表参数 EEPROM 24C02
//国网单相表配置
//#define DataEAds    0xA0	//#define DataEAds    0xA4	//数据 EEPROM 24C64
#define AdjEAds     0xA6	//#define AdjEAds     0xA0	//校表参数 EEPROM 24C04

#define	FMAds		0xA0	//当前电量和需量缓存 FM24C16	
#define RTCAds		0x64	//RTC8025时钟芯片地址
#define TCN75Ads	0x9E	//温度传感器地址
#define DS3231Ads	0xD0	//时钟芯片DS3231地址
#define PCF8578Ads  0x78    //PCF8578地址			//10.08.03
#define BU97950Ads  0x7C    //BU97950地址			//10.11.11
#define BR8564Ads   0x70    //BR8564地址			//12.07.18

#define BYTE4		0x04
#define BYTE5		0x05
#define BYTE6		0x06

#define ECDIGIT		BYTE4//电量数据位数
#define DATETIME	0x0C

//#define PowerLoadCurveSta	0x00000000
//#define PowerLoadCurveEnd	0x00037000		//220K  6个月有功功率
//#define PowerLoadCurveEnd	0x00005000		//220K  6个月有功功率

//#define GeneralLoadCurveSta	0x00037000		//220K
//#define GeneralLoadCurveEnd	0x00100000		//804K  普通负荷曲线范围，内容可设
//#define GeneralLoadCurveEnd	0x0003B000		//512K  普通负荷曲线范围，内容可设

#define YesCheck	0x00
#define NoCheck		0x01

#define ADE7758		0x00	
#define ATT7022		0x01
#define ADE7878		0x02	
#define ATT7022E	0x03								//11.09.24
#define IDT90E32	0x04								//11.09.24
#define RN8207		0x05
#define RN8209		0x06
#define HT7017		0x07

//#define ATT7022C	0x03
//#define ATT7022D	0x04
//#define MEASCHIP	ADE7758	//选择计量芯片类型	
//#define MEASCHIP	ATT7022	//选择计量芯片类型	
//#define MEASCHIP	RN8209//选择计量芯片类型		
#define MEASCHIP	HT7017//选择计量芯片类型		

//#define MEASCHIP	IDT90E32//选择计量芯片类型	
//#define MEASCHIP	ADE7878	//选择计量芯片类型	
//#define MEASCHIP	ATT7022D//选择计量芯片类型	

#if ( MEASCHIP == IDT90E32 )							//11.09.25
#define IDT90E32AS	YesCheck		//IDT芯片为90E32AS
//#define IDT90E32AS	NoCheck		//IDT芯片为90E32
#else
#define IDT90E32AS	NoCheck			//IDT芯片为90E32AS
#endif

#if ( MEASCHIP == ATT7022E )
#define NEW7022E	YesCheck		//新版7022E芯片		//新国网
#else
#define NEW7022E	NoCheck			//非新版7022E芯片	//新国网
#endif

//#define ZjProtocol	YesCheck		//有浙江省通讯规约
#define ZjProtocol	NoCheck		//无浙江省通讯规约

#define N645Protocol	YesCheck		//有645-2007通讯规约	//N645
//#define N645Protocol	NoCheck		//有645-2007通讯规约		//N645

#define PwOnLCDControl	YesCheck		//上电打开液晶5V电源	
//#define PwOnLCDControl	NoCheck
	
#define LCD8259		YesCheck			//8259液晶		国网新液晶
//#define LCD8259	NoCheck				//非8259液晶

//#define LCD7862		YesCheck		//7862液晶		国网老液晶
#define LCD7862	NoCheck					//非7862液晶

//#define LCDDrive_PCF8578		YesCheck   //PCF8578	//10.08.03
#define LCDDrive_PCF8578		NoCheck   //PCF8578

//#define LCDDrive_BU97950		YesCheck   //BU97950	//10.11.11
#define LCDDrive_BU97950		NoCheck   //BU97950

#define LCDDrive_BR8564			YesCheck   //BR8564	//2012.07.03
//#define LCDDrive_BR8564			NoCheck   //BR8564	//2012.07.03

//#define AuxPWFailCheck	YesCheck		//辅助电源检测0.2S、0.5S级表有
#define AuxPWFailCheck	NoCheck			//1级表无		费控表均无辅助电源

#define LoadMFlash	YesCheck		//负荷曲线使用Flash芯片

//#define FlashAT26DF321	YesCheck	//不再需要此条件编译	//17.03.27
//#define FlashAT26DF321	NoCheck

#define MagnKeepRelay YesCheck
//#define MagnKeepRelay NoCheck

#define CarryComm YesCheck			//带载波通讯
//#define CarryComm NoCheck				//无载波通讯
                       
#if ( CarryComm == YesCheck )             	
#define GPRSComm YesCheck				//带GPRS通讯
#else
#define GPRSComm NoCheck				//无GPRS通讯
#endif                     

#define LocalSmartMeter	YesCheck	//本地费控智能表	
//#define LocalSmartMeter	NoCheck			//本地费控智能表	

//#define ISO7816Config	YesCheck	//卡按7816协议通讯
#define ISO7816Config	NoCheck		//卡按Uart协议通讯

//#define GeneralSmartMeter	YesCheck	//普通智能表	
#define GeneralSmartMeter	NoCheck		//非普通智能表	
               	
//#define JiangSuRequire YesCheck			//江苏要求：
#define JiangSuRequire NoCheck		//非江苏要求
                                    	
//#define BeiJingRequire YesCheck		//北京要求：选择收到远程允许命令后立即合闸，选择双485口。
#define BeiJingRequire NoCheck			//非江苏要求

//#define AnHuiRequire YesCheck			//安徽要求：时段表、时区表允许设置为全零，选择双485口。
#define AnHuiRequire NoCheck			//非安徽要求

//#define ShanDongRequire YesCheck		//山东要求：与国网一样，无具体要求
#define ShanDongRequire NoCheck		//非山东要求

//#define HuBeiRequire YesCheck			//湖北要求：与国网一样，双485 永远选中
#define HuBeiRequire NoCheck			//非湖北要求

//#define HuaBeiRequire YesCheck			//冀北要求		//费控表显示不包含组合无功1和组合无功2	//17.03.25
#define HuaBeiRequire NoCheck			//非冀北要求
                                    	
#if ( BeiJingRequire == YesCheck )   	
#define RelayONImmediately YesCheck		//立即自动合闸
//#define RelayONImmediately NoCheck	//允许人工合闸

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//单485口
//#else
#define Two485Ports YesCheck			//双485口
//#endif

#elif ( AnHuiRequire == YesCheck )   	//	
//#define RelayONImmediately YesCheck	//立即自动合闸
#define RelayONImmediately NoCheck		//允许人工合闸

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//单485口
//#else
#define Two485Ports YesCheck			//双485口
//#endif

#elif ( ShanDongRequire == YesCheck )   //	
#define RelayONImmediately NoCheck		//允许人工合闸

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//单485口
//#else
#define Two485Ports YesCheck			//双485口
//#endif

#elif ( HuBeiRequire == YesCheck )   	//	
#if ( MagnKeepRelay == YesCheck )
#define RelayONImmediately NoCheck		//允许人工合闸
#else
#define RelayONImmediately YesCheck		//立即合闸
#endif

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//单485口
//#else
#define Two485Ports YesCheck			//双485口
//#endif

#else
//#define RelayONImmediately YesCheck	//立即自动合闸
#if ( MagnKeepRelay == YesCheck )
#define RelayONImmediately NoCheck		//允许人工合闸
#else
#define RelayONImmediately YesCheck		//立即自动合闸
#endif

//#if ( GPRSComm == YesCheck )
//#define Two485Ports NoCheck				//单485口
//#else
#define Two485Ports YesCheck			//双485口
//#endif

#endif

#if ( JiangSuRequire == YesCheck )
#define RemoteFeeMeter	YesCheck		//远程费率智能表	//15.12.14
#else
#define RemoteFeeMeter	NoCheck			//远程费率智能表	//15.12.14
#endif


#define RX8025		0x00	//
#define DS3231		0x01	//
#define RX8025T		0x02	//V1000
////////#define RTCCHIP		RX8025	// 选择时钟芯片类型
//#define RTCCHIP		DS3231	// 选择时钟芯片类型
//#define RTCCHIP		RX8025T	// 选择时钟芯片类型			//V1000		//新国网

//#define CarryCommWithOC 	YesCheck	//载波与主板光耦隔离，STA信号脚逻辑反向					//10.11.23
#define CarryCommWithOC 	NoCheck		//载波与主板光耦无隔离，用三极管切换电平，STA逻辑不反向		//10.11.23

#define PRODUCTTYPE1	'S'				//型号SD32DH	
//#define PRODUCTTYPE2	'D'
#if ( CarryComm == YesCheck )
//#define PRODUCTTYPE2	'Z'				//新国网
//#elif ( GPRSComm == YesCheck )		//新国网
#define PRODUCTTYPE2	'N'
#else
#define PRODUCTTYPE2	'D'
#endif
//#define PRODUCTTYPE3	'0'
//#define PRODUCTTYPE3	'1'				//新硬件方案升级为1
#if ( MEASCHIP == ATT7022E )
#define PRODUCTTYPE3	'3'				//新硬件方案升级为3		//7022E
#elif ( MEASCHIP == IDT90E32 )
#define PRODUCTTYPE3	'4'				//新硬件方案升级为4		//90E32
#else
#define PRODUCTTYPE3	'2'				//新硬件方案升级为2
#endif
//#define PRODUCTTYPE4	'2'
#define PRODUCTTYPE4	'3'
#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#define PRODUCTTYPE5	'A'													//新国网	//13.11.06		
#else																		//新国网	//13.11.06
#define PRODUCTTYPE5	'C'
#endif
//#define PRODUCTTYPE6	'K'
#if ( MagnKeepRelay == YesCheck )
#define PRODUCTTYPE6	'N'
#else
#define PRODUCTTYPE6	'W'
#endif

//'-'

//#define MANUFACTURER1	'J'				//上海金陵
//#define MANUFACTURER2	'L'
#if ( JiangSuRequire == YesCheck )
//#define MANUFACTURER1	'D'				//东汇
//#define MANUFACTURER2	'H'
#define MANUFACTURER1	'J'				//炬华
#define MANUFACTURER2	'H'
#elif ( BeiJingRequire == YesCheck ) 
#define MANUFACTURER1	'J'				//炬华
#define MANUFACTURER2	'H'
#elif ( AnHuiRequire == YesCheck )   	//	
#define MANUFACTURER1	'J'				//炬华
#define MANUFACTURER2	'H'
#elif ( ShanDongRequire == YesCheck )   //	
#define MANUFACTURER1	'J'				//炬华
#define MANUFACTURER2	'H'
#else
#define MANUFACTURER1	'J'				//炬华
#define MANUFACTURER2	'H'
#endif

//#define USERREGION1		'J'			//吉林
//#define USERREGION2		'L'
#if ( JiangSuRequire == YesCheck )
#define USERREGION1		'J'				//江苏
#define USERREGION2		'S'
#elif ( BeiJingRequire == YesCheck ) 
#define USERREGION1		'B'				//北京
#define USERREGION2		'J'
#elif ( AnHuiRequire == YesCheck )   		
#define USERREGION1		'A'				//安徽
#define USERREGION2		'H'
#elif ( ShanDongRequire == YesCheck )   		
#define USERREGION1		'S'				//山东
#define USERREGION2		'D'
#elif ( HuBeiRequire == YesCheck )   	//	
#define USERREGION1		'H'				//湖北
#define USERREGION2		'B'
#elif ( HuaBeiRequire == YesCheck )		//冀北要求		//费控表显示不包含组合无功1和组合无功2	//17.03.25
#define USERREGION1		'N'				//冀北
#define USERREGION2		'C'
#else
#define USERREGION1		'G'				//国网
#define USERREGION2		'W'
#endif

//'-'

/*
#if ( METERTYPE == U220_I1A6400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'C'				//1.5(6)A
#elif ( METERTYPE == U220_I20A400 )
#define VOLTTYPE		'2'				//3*220V
#define CURRENTTYPE		'K'				//20(80)A
#elif ( METERTYPE == U220_I10A800 )
#define VOLTTYPE		'2'				//3*220V
#define CURRENTTYPE		'K'				//10(80)A
#elif ( METERTYPE == U220_I10A400 )
#define VOLTTYPE		'2'				//3*220V
#define CURRENTTYPE		'K'				//10(80)A
#elif ( METERTYPE == U100_I1A12800 )
#define VOLTTYPE		'2'				//3*220V
#define CURRENTTYPE		'K'				//10(80)A
#endif
*/

/*															//新国网		//13.08.30
#if ( METERTYPE == U220_I0_3A__1_2A25600 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'A'				//0.3(1.2)A
#elif ( METERTYPE == U220_I1A__6A6400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'D'				//1.0(6)A
#elif ( METERTYPE == U220_I1_5A__6A6400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'E'				//1.5(6)A
#elif ( METERTYPE == U220_I2_5A__10A3200 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'F'				//1.5(6)A
#elif ( METERTYPE == U220_I5A__6A6400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'H'				//5(6)A
#elif (( METERTYPE == U220_I5A__20A800 )||( METERTYPE == U220_I5A__20A1200 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'J'				//5(20)A
#elif ( METERTYPE == U220_I5A__30A800 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'K'				//5(30)A
#elif (( METERTYPE == U220_I5A__40A400 )||( METERTYPE == U220_I5A__40A600 )||( METERTYPE == U220_I5A__40A800 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'L'				//5(40)A
#elif (( METERTYPE == U220_I5A__50A400 )||( METERTYPE == U220_I5A__50A600 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'M'				//5(30)A
#elif ( METERTYPE == U220_I5A__60A400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'N'				//5(30)A
#elif (( METERTYPE == U220_I10A__40A400 )||( METERTYPE == U220_I10A__40A600 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'P'				//10(40)A
#elif ( METERTYPE == U220_I10A__60A400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'Q'				//10(60)A
#elif (( METERTYPE == U220_I20A__80A200 )||( METERTYPE == U220_I20A__80A320 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'U'				//20(80)A
#elif ( METERTYPE == U220_I15A__60A400 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'S'				//10(60)A
#elif ( METERTYPE == U220_I20A__100A240 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'V'				//20(100)A
#elif ( METERTYPE == U220_I20A__120A240 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'W'				//20(120)A
#elif (( METERTYPE == U220_I10A__100A240 )||( METERTYPE == U220_I10A__100A300 ))
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'R'				//20(100)A
#elif ( METERTYPE == U220_I30A__100A240 )
#define VOLTTYPE		'2'				//3*220V	
#define CURRENTTYPE		'X'				//20(120)A
#elif ( METERTYPE == U57_I0_3A__1_2A100000 )
#define VOLTTYPE		'5'				//3*57.7V	
#define CURRENTTYPE		'A'				//0.3(1.2)A
#elif ( METERTYPE == U57_I1A__6A20000 )
#define VOLTTYPE		'5'				//3*57.7V	
#define CURRENTTYPE		'D'				//1.0(6)A
#elif ( METERTYPE == U57_I1_5A__6A20000 )
#define VOLTTYPE		'5'				//3*57.7V	
#define CURRENTTYPE		'E'				//1.5(6)A
#elif ( METERTYPE == U100_I0_3A__1_2A100000 )
#define VOLTTYPE		'1'				//3*100V
#define CURRENTTYPE		'A'				//0.3(1.2)A
#elif ( METERTYPE == U100_I1A__6A20000 )
#define VOLTTYPE		'1'				//3*100V
#define CURRENTTYPE		'D'				//1(6)A
#elif ( METERTYPE == U100_I1_5A__6A20000 )
#define VOLTTYPE		'1'				//3*100V
#define CURRENTTYPE		'E'				//1.5(6)A
#endif
*/

//'-'

//#define VERSION1		'1'				//新国网
#define VERSION1		'3'				//698协议
//#define VERSION2		'0'
#define VERSION2		'1'				//DLT645-2007-14	//新国网	//14.05.03
#define VERSION3		'0'
//#define VERSION2		'1'				//改正写ESAM、卡时数据区多发送一个字节问题		//12.12.30
//#define VERSION3		'1'
//#define VERSION3		'2'				//国网2011年第三次招标		//11.07.19
//#define VERSION3		'3'				//国网2011年第五次招标		//11.12.14
//#define VERSION3		'4'				//国网2012年第1次招标		//12.04.12	//组合电量显示负号，全失压下记录分相失压事件
//#define VERSION3		'5'				//国网2012年第2次招标要求(实际在第三次招标才改)		//12.10.04	//当前结算周期、编程记录、公钥状态定义
//#define VERSION3		'6'				//国网2012年第4次招标要求	//12.12.20		//取消操作者代码更换后，新产生一条编程记录
#define VERSION4		'.'
//#define VERSION5		'0'
//#define VERSION5		'1'				//增加国网第7号补遗			//15.03.04
//#define VERSION5		'2'				//增加特殊点校表			//15.09.11
#define VERSION5		'0'				//
//'-'

#if ( LinkMode == Phase3Wire4 )
#define LINEMODE		'A'				//三相四线
#else
#define LINEMODE		'B'				//三相三线
#endif

//#if ( RTCCHIP == DS3231 )								//新国网
//#define CLOCKTYPE		'A'				//DS3231		//新国网
//#else													//新国网
//#define CLOCKTYPE		'B'				//8025T			//新国网
//#endif												//新国网

#define LCDTYPE			'A'				//8259
////#define FLASHTYPE		'A'				//26DF161
//#if ( FlashAT26DF321 ==	YesCheck )
//#define FLASHTYPE		'B'				//26DF321
//#else
//#define FLASHTYPE		'C'				//S25FL032P
//#endif
//#define FLASHTYPE		'C'				//698协议FLASH芯片起码用64
#define FLASHTYPE		'D'				//698协议FLASH芯片统一用128		//17.05.08

#if ( LCDDrive_PCF8578 == NoCheck )
#if ( LCDDrive_BU97950 == YesCheck )
#define LCDDRVTYPE		'C'				//BU97950
#elif ( LCDDrive_BR8564	== YesCheck )
#define LCDDRVTYPE		'D'				//BR8564
#else
#define LCDDRVTYPE		'A'				//EM1160
#endif
#else
#define LCDDRVTYPE		'B'				//PCF8578
#endif

#if ( AuxPWFailCheck ==	YesCheck )
#define AUXPWTYPE		'A'				//带辅助电源
#else
#define AUXPWTYPE		'B'				//无辅助电源
#endif

#if ( MagnKeepRelay == YesCheck )
#define RELAYTYPE		'A'				//负荷开关内置
#else
#define RELAYTYPE		'B'				//负荷开关外置或没有
#endif

#if (( CarryComm == NoCheck )&&( GPRSComm == NoCheck ))
#define COMMTYPE		'A'				//非载波及GPRS通讯	
#elif ( CarryComm == YesCheck )
//#define COMMTYPE		'B'				//带载波通讯	//新国网
//#elif ( GPRSComm == YesCheck )						//新国网
#define COMMTYPE		'C'				//带GPRS通讯	
#else
#endif

#if ( BeiJingRequire == YesCheck )   	
#define VERSION6		'0'				//北京：立即跳闸、双485口
#elif ( AnHuiRequire == YesCheck )   		
#define VERSION6		'0'				//安徽：时区时段允许设成全零、双485口
#elif ( ShanDongRequire == YesCheck )   		
#define VERSION6		'0'				//山东：与国网一致
#elif ( HuBeiRequire == YesCheck )   	//	
#define VERSION6		'0'				//湖北：与国网一致
#elif ( JiangSuRequire == YesCheck )
#if ( ClockBatResisterIs2M == YesCheck	)			//时钟电池采样电阻由5M改为2M //17.02.23
#define VERSION6		'1'				//江苏
#else
#define VERSION6		'0'				//江苏前期40只698表
#endif
#else
//#define VERSION6		'0'				//新方案		10.08.04
//#define VERSION6		'1'				//新方案		//新国网	//13.10.28
//#define VERSION6		'2'				//14年1月送电科院		//新国网	//14.01.11
//#define VERSION6		'3'				//14年1月送电科院		//新国网	//14.01.24	//校表及规格数据从FLASH恢复
//#define VERSION6		'4'				//14年2月送电科院		//新国网	//14.02.11	//15分钟时段间隔
//#define VERSION6		'5'				//14年4月送电科院		//新国网	//14.03.22	//置远程本地未开户、DS3231初始值校准

//#define VERSION6		'0'				//14年4月电科院要求		//新国网	//14.05.06	//主动上报、负荷曲线等
//#define VERSION6		'1'				//14年6月电科院要求		//新国网	//14.07.09	//增加信息FLASH参与程序CRC、增加参数CRC内容
//#define VERSION6		'2'				///14.07.14		//增加自定义命令加密设置备案号等
//#define VERSION6		'3'				///14.07.25		//7022E没短校表点的校表方式下，读电压值缺少一位小数
//#define VERSION6		'4'				///14.09.03		//增加电池工作时间的“时间”符号等，送电科院

//#define VERSION6		'0'				///14.03.04		//增加国网第7号补遗			
//#define VERSION6		'1'				///15.09.30		//增加密钥更新延时及重复更新一次,增加FLASH型号辨识
//#define VERSION6		'2'				//16.01.22		//负荷间隔大于等于60时优化抄读出错
//#define VERSION6		'3'				//16.04.12		//复位时脉冲尾数、红外背光唤醒处理
//#define VERSION6		'4'				//16.06.06		//避免停抄电池欠压后误报存储器故障及存储器故障后误报电池欠压

#define VERSION6		'0'				//698协议

//#define VERSION6		'0'				//八字节报警控制字，调整EEPROM  江苏
//#define VERSION6		'1'				//IO口
//#define VERSION6		'2'				//时段切换，电池电压
//#define VERSION6		'3'				//当前结算周期组合电量、增备份RAM等  10.04.12
//#define VERSION6		'3'				//10.07.24 远程
//#define VERSION6		'4'				//10.07.21
//#define VERSION6		'3'				//便于观察是否刷新，其余不变
//#define VERSION6		'4'				//八字节报警控制字，调整EEPROM
//#define VERSION6		'4'				//晶振失效等

#endif

/*
//版本信息：
//低第一字节： 版本号
//低第二字节： 附加规约
//高第二字节： 使用地区
//高第一字节： 用户厂家

//用户厂家
#define HUALI	0x01	//华立	
#define WANSHENG 0x02	//万胜
#define SANXING	0x03	//三星
#define NEWERGO	0x04	//新艺高
#define DONGYA	0x05	//北京东亚
#define HENGYE	0x06	//嘉兴恒业
#define XILI	0x09	//西力
#define JHBADA	0x13	//金华八达
#define DONGHUI	0x14	//东汇
//使用地区
#define HENAN	0x01	//河南
#define BEIJING	0x02	//北京
#define ZHEJIANG	0x03	//浙江
#define ZJSHAOXING	0x07	//浙江绍兴
#define ZJXIAOSHAN	0x20	//浙江萧山
#define ZJQVZHOU	0x19	//浙江衢州

//附加规约
#define BUGUI	0x01	//纯部规
#define BU_ZHENGZHOU	0x02	//部规+郑州规约
//#define BU_ZHEJIANG		0x13	//部规+浙江规约		//07年8月招标
#define BU_ZHEJIANG		0x23	//部规+浙江规约		//08年4月招标

//版本号
#define VER0	0x00	//0.0版本	06.02.15
#define VER1	0x01	//1.0版本	06.02.15
#define VER2	0x02	//2.0版本
#define VER3	0x03	//3.0版本	06.05.13  浙江	三相三线全失压电压电流判断修正
#define VER4	0x04	//4.0版本	06.05.13  绍兴  三相三线全失压电压电流判断修正

#define VER11	0x11	//1.0版本	//4067	四线 万胜
#define VER13	0x13	//1.0版本	//4067	三线 万胜
#define VER14	0x14	//1.0版本	//4051  四线 万胜
#define VER16	0x16	//1.0版本	//4051  三线 万胜

#define VER21	0x21	//2.0版本	//4067	四线 万胜
#define VER23	0x23	//2.0版本	//4067	三线 万胜
#define VER24	0x24	//2.0版本	//4051  四线 万胜
#define VER26	0x26	//2.0版本	//4051  三线 万胜

#define VER31	0x31	//3.0版本	//4067	四线 万胜
#define VER33	0x33	//3.0版本	//4067	三线 万胜
#define VER34	0x34	//3.0版本	//4051  四线 万胜
#define VER36	0x36	//3.0版本	//4051  三线 万胜


//#define MANUFACTURER	NEWERGO		//新艺高
//#define MANUFACTURER	DONGHUI		//东汇
//#define MANUFACTURER	XILI		//西力
#define MANUFACTURER	WANSHENG	//万胜			//恒业
//#define MANUFACTURER	SANXING		//三星
//#define MANUFACTURER	HENGYE		//嘉兴恒业		//作废
//#define MANUFACTURER	JHBADA		//金华八达

//#define USERREGION		ZJSHAOXING	//浙江绍兴
#define USERREGION		ZHEJIANG	//浙江
//#define USERREGION		ZJXIAOSHAN	//浙江萧山
//#define USERREGION		ZJQVZHOU	//浙江衢州

#if ( ZjProtocol	== YesCheck )	//有浙江通讯规约)
#define PROTOCOL	BU_ZHEJIANG
#else
#endif

//以下万胜
//#define SOFTVERSION VER14		//4051  四线 万胜
//#define SOFTVERSION VER16		//4051  三线 万胜

//#define SOFTVERSION VER24		//4051  四线 万胜
//#define SOFTVERSION VER26		//4051  三线 万胜		//断相不判角度

//#define SOFTVERSION VER34		//4051  四线 万胜
//#define SOFTVERSION VER36		//4051  三线 万胜

//以下西力
#define SOFTVERSION VER0
//#define SOFTVERSION VER1
//#define SOFTVERSION VER2		//恒业0.5S
//#define SOFTVERSION VER3
//#define SOFTVERSION VER4		//西力三相三线断相不判角度

//以下恒业
//#define SOFTVERSION VER0
//#define SOFTVERSION VER1
//#define SOFTVERSION VER2		//恒业0.5S
//#define SOFTVERSION VER3
//#define SOFTVERSION VER4		//西力三相三线断相不判角度

//#define SMALLSOFTVER	VER2
#define SMALLSOFTVER	VER3	//时钟BUG修改



#if ( MANUFACTURER == XILI )
//#define MEASCHIP	ADE7758	//选择计量芯片类型	
#define MEASCHIP	ATT7022	//选择计量芯片类型	
//#define LCD4041		YesCheck		//4041液晶
#define LCD4041		NoCheck			//3713液晶
#define LCD4712		YesCheck	//4712液晶
//#define LCD4712		NoCheck			//非4712液晶
//#define LCD4773		YesCheck	//4773液晶		//与4712的差异为 N->万胜
#define LCD4773		NoCheck			//非4773液晶
//#define LCD4713		YesCheck	//4713液晶
#define LCD4713		NoCheck			//非4713液晶
//#define LCD3713		YesCheck		//3713液晶
#define LCD3713		NoCheck			//非3713液晶
#define CD4051		YesCheck	//使用八选一转换
//#define CD4051		NoCheck				//不使用八选一转换
//#define CD4067		YesCheck		//使用16选一转换
#define CD4067		NoCheck		//不使用16选一转换


#elif ( MANUFACTURER == WANSHENG )							//万胜
//#define MEASCHIP	ADE7758	//选择计量芯片类型	
//#define MEASCHIP	ATT7022	//选择计量芯片类型	
#define MEASCHIP	ADE7878	//选择计量芯片类型	
//#define LCD4041		YesCheck		//4041液晶
#define LCD4041		NoCheck			//3713液晶
#define LCD4712		YesCheck	//4712液晶
//#define LCD4712		NoCheck			//非4712液晶
#define LCD4773		YesCheck	//4773液晶		//与4712的差异为 N->万胜
//#define LCD4773		NoCheck			//非4773液晶
//#define LCD4713		YesCheck	//4713液晶
#define LCD4713		NoCheck			//非4713液晶
//#define LCD3713		YesCheck		//3713液晶
#define LCD3713		NoCheck			//非3713液晶
#define CD4051		YesCheck	//使用八选一转换
//#define CD4051		NoCheck				//不使用八选一转换
//#define CD4067		YesCheck		//使用16选一转换
#define CD4067		NoCheck		//不使用16选一转换

#endif


#define MSPF449		0x00
#define MSPF437		0x01
//#define CPUCHIP		MSPF437	// 选择单片机芯片类型
#define CPUCHIP		MSPF449	// 选择单片机芯片类型
*/

#define UCLOCKBATL	0x20
#define UCLOCKBATH	0x03	// 时钟电池阀值3.2V

#define BatVolt6V	0x00		// 停抄电池电压 = 6V	
#define BatVolt3V	0x01		// 停抄电池电压 = 3V	

#define CommBatVolt	BatVolt6V		//停抄电池电压 = 6V	
//#define CommBatVolt	BatVolt3V		//停抄电池电压 = 3V	

#if ( CommBatVolt == BatVolt6V )	//停抄电池电压 = 6V	
#define UCOMMBATL	0x20	
#define UCOMMBATH	0x05	//停抄电池阀值5.2V
#else
//#define UCOMMBATL	0x50	
//#define UCOMMBATH	0x02	// 停抄电池阀值2.5V
#define UCOMMBATL	0x20							//3.6V停抄电池
#define UCOMMBATH	0x03	//停抄电池阀值5.2V
#endif

//#define YesCheck	0x00
//#define NoCheck		0x01
#define AllFailChk 	YesCheck	//检测全失压检测
//#define AllFailChk 	NoCheck		//不检测全失压

#define AllFail7022Chk 		YesCheck	//通过7022检测全失压检测	//10.11.11
//#define AllFail7022Chk 	NoCheck		//不通过7022检测全失压		//10.11.11

#define ESAMPowerControl 	YesCheck	//ESAM电源控制				//10.12.02
//#define ESAMPowerControl 	NoCheck	//ESAM电源控制				//10.12.02

#define NewHardScheme2		YesCheck	//新方案2硬件			//10.12.25

#define FeeMDIndependence	YesCheck	//费率需量独立计量			//11.01.28
//#define FeeMDIndependence	NoCheck		//费率需量独立计量			//11.01.28

//#define FeeECIndependence	YesCheck	//费率电量独立计量			//11.01.15
#define FeeECIndependence	NoCheck		//费率电量独立计量			//11.01.15

#define TakeMDClrRecFrameApart		YesCheck		//需量清零事件帧拆分	//11.02.17
//#define TakeMDClrRecFrameApart	NoCheck

#define PowerOnMonSaveMAX12		YesCheck	//上电最多结算12次		//11.06.27
//#define PowerOnMonSaveMAX12	NoCheck		//上电不结算			//11.06.27

//#define CombinationFlagDisplay	YesCheck	//“组合”字符显示	//11.07.19
#define CombinationFlagDisplay	NoCheck			//“组合”字符不显示	//11.07.19

//#define MinusSignDisplay		YesCheck	//负号显示				//11.07.19
#define MinusSignDisplay		NoCheck		//负号不显示			//11.07.19

#define SaveDay1CycleEC		YesCheck	//当前周期组合有功累计用电量结算方式是按设置的第1个结算日结算	//2012.10.04
//#define SaveDay1CycleEC	NoCheck		//当前周期组合有功累计用电量结算方式是按设置的3个结算日结算

//#define PrgRecRestWithOperateCode YesCheck	//操作者代码更换后，新产生一条编程记录		//2012.10.04
#define PrgRecRestWithOperateCode NoCheck	//操作者代码更换后，维持原有编程记录		//2012.10.04

#define SaveDaySort			YesCheck		//三个结算日时间不按照顺序设置也能正确结算，需量结算跟随第一结算日	//12.12.05
//#define SaveDaySort		NoCheck			//

//#define PwDownKeepPrgDelay	YesCheck		//无电池情况下掉电后维持编程有效时间准确计时	//12.12.21		//新国网
//#define PwDownKeepPrgDelay	NoCheck		//

#if ( FeeECIndependence	== YesCheck )
#define ECDisDecDigitsValid		YesCheck	//当前电量显示小数位真实有效
#else
#define ECDisDecDigitsValid		NoCheck		//显示当前电量第3、4位小数内容为补零
#endif

#define LASTBLOCKProcess	YesCheck		//负荷曲线的最后一条判断是否有无后续帧，不要多发一条空的后续帧	//13.03.16
//#define LASTBLOCKProcess	NoCheck			//

#define DLT645_2007_14		YesCheck		//DL/T645-2007-14的新备案要求	//14.04.25
//#define DLT645_2007_14	NoCheck			//DL/T645-2007-14的新备案要求

#define InfraredCarry38k	YesCheck		//远红外用38K载波			//15.01.22	
//#define InfraredCarry38k	NoCheck			//远红外用32K载波	

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
#define ExactECRead			YesCheck		//精确电能读取
#if ( MagnKeepRelay == YesCheck )
#define IOver30ALimitTrip		YesCheck	//电流大于30A限制跳闸	//新国网	//14.04.25	
#else
#define IOver30ALimitTrip		NoCheck		//电流大于30A不限制跳闸	
#endif

#define RemoteEnterCalib	YesCheck		//支持远程进入校表状态	//新国网	//14.04.25	
//#define FlashTypeSelect		YesCheck		//支持Flash类型设置		//新国网	//14.06.04	
#define FlashTypeSelect		NoCheck		//支持Flash类型设置		//新国网	//14.06.04	
#define ModifyTimeSkipDelay	NoCheck			//不支持修改时间跳过大安培拉闸和30分钟上报后复位延时		//新国网	//14.06.19	

#else
#define ExactECRead		NoCheck			//无精确电能读取
#define IOver30ALimitTrip		NoCheck		//电流大于30A不限制跳闸	//11.06.29	

#define RemoteEnterCalib	YesCheck		//支持远程进入校表状态	//新国网	//14.04.25	
//#define FlashTypeSelect		YesCheck		//支持Flash类型设置		//新国网	//14.06.04	
#define FlashTypeSelect		NoCheck		//支持Flash类型设置		//新国网	//14.06.04	
#define ModifyTimeSkipDelay	NoCheck			//不支持修改时间跳过大安培拉闸和30分钟上报后复位延时		//新国网	//14.06.19	

#endif

#if ( GeneralSmartMeter	== YesCheck )		//普通智能表	//新国网	//13.11.06
#define HardWareB40Exist		NoCheck		//硬件中B40电源集成块不存在	//11.06.29
#elif ( CarryComm == NoCheck )			
#define HardWareB40Exist		NoCheck		//硬件中B40电源集成块不存在	//11.06.29
#else
#define HardWareB40Exist		YesCheck	//硬件中B40电源集成块存在	//11.06.29	//卡表都有B40
#endif

//#define AdjustErrorCheck	YesCheck	//特殊校表选中		//15.09.05
#define AdjustErrorCheck	NoCheck		//特殊校表不选中	//15.09.05

#define FRAMPulseValidCheck YesCheck		//掉电后FRAM中脉冲是否有效监测		//16.01.26
//#define FRAMPulseValidCheck NoCheck		//掉电后FRAM中脉冲是否有效监测		//16.01.26

#define ClockBatResisterIs2M YesCheck		//时钟电池采样电阻由5M改为2M
//#define ClockBatResisterIs2M NoCheck		//时钟电池采样电阻5M

#if ( LinkMode == Phase3Wire4 )
//#define ZeroCuChk	YesCheck   //零线电流检测
#define ZeroCuChk	NoCheck   //零线电流不检测

//#define NLineCheck	YesCheck  //掉零线检测
#define NLineCheck	NoCheck   //无掉零线检测
#else
#define ZeroCuChk	NoCheck   //零线电流不检测
#define NLineCheck	NoCheck   //无掉零线检测
#endif

#define PhotoDiode	0x00		//光敏二极管触发
#define PhotoTriode	0x01		//光敏三极管触发
//#define IRTrigger	PhotoDiode 	//光敏二极管触发
#define IRTrigger	PhotoTriode //光敏三极管触发

#define LoadRecCheck	YesCheck 	//有负荷曲线记录
//#define LoadRecCheck	NoCheck 	//无负荷曲线记录

#if ( LoadRecCheck == YesCheck )
#define GeneralLoadCurve 	YesCheck	//普通负荷曲线	
//#define GeneralLoadCurve 	NoCheck		//普通负荷曲线	
#else
#define GeneralLoadCurve 	NoCheck		//普通负荷曲线	
#endif

#define	SecurityCheck	YesCheck 	//有安全认证
//#define	SecurityCheck	NoCheck 	//无安全认证

//#define HWKeyAllDis		YesCheck 		//红外遥控器可查看所有项	//HW	//万胜
#define HWKeyAllDis		NoCheck 	//红外遥控器可查看按显项					//西力

/*
#define ZjProtocol	YesCheck		//有浙江省通讯规约
//#define ZjProtocol	NoCheck		//无浙江省通讯规约

//#define LCD4041		YesCheck		//4041液晶
#define LCD4041	NoCheck			//3713液晶

#define LCD4712		YesCheck	//4712液晶
//#define LCD4712		NoCheck			//非4712液晶

//#define LCD4773		YesCheck	//4773液晶		//与4712的差异为 N->万胜
#define LCD4773		NoCheck			//非4773液晶

//#define LCD4713		YesCheck	//4713液晶
#define LCD4713		NoCheck			//非4713液晶

//#define LCD3713		YesCheck		//3713液晶
#define LCD3713		NoCheck			//非3713液晶

//#define CD4051		YesCheck	//使用八选一转换
#define CD4051	NoCheck				//不使用八选一转换

#define CD4067		YesCheck		//使用16选一转换
//#define CD4067		NoCheck		//不使用16选一转换
*/
#define CrawlLimit	YesCheck		//潜动电流限制		//ADE7758的1级表需要
//#define CrawlLimit	NoCheck		//无潜动电流限制	//ADE7758的0.5级表需要

//#if (( CD4051 == YesCheck )||( CD4067 == YesCheck )) 
//#define LoadMFlash	YesCheck		//负荷曲线使用Flash芯片
//#define LoadMFlash	NoCheck		//负荷曲线使用E2PROM芯片
//#else
//#define LoadMFlash	NoCheck			//负荷曲线使用E2PROM芯片
//#endif

//#if ( USERREGION == ZJQVZHOU )		//浙江衢州		//V03
//#define RunOverCheck	YesCheck
//#else
//#define RunOverCheck	NoCheck
//#endif

#define RunOverCheck	NoCheck

//DS3231
#define RS1			0x0008
#define RS2			0x0010
#define EN32kHz		0x0800
#define INTCN		0x0004

//8025
#define	Hour24		0x0020
#define CLEN1		0x0800
#define CLEN2		0x0010
#define CT0			0x0001
#define CT1			0x0002
#define CT2			0x0003
#define	XST			0x2000

//RX8025T
#define CSEL0		0x4000
#define UIE			0x2000

//#if ( RTCCHIP == RX8025 )																//新国网
//#define EN32K1Hz	XST+Hour24+CT0+CT1		//0x2023	32kHz+秒脉冲输出                
//#define EN32K		XST+Hour24				//0x2020	32kHz+无脉冲输出				
//#define DisEn32K1Hz	XST+Hour24+CLEN1+CLEN2	//0x2830	无输出						
//
//#elif ( RTCCHIP == RX8025T )				//V1000										//新国网
//#define EN32K1Hz	CSEL0+UIE				//0x6000	32kHz+秒脉冲输出 2秒刷新温度系数
//#define EN32K		CSEL0					//0x4000	32kHz+无脉冲输出
//
//#else
//
//#define EN32K1Hz	EN32kHz				//0x0800		32kHz+秒脉冲输出
//#define EN32K		EN32kHz+INTCN		//0x0804		32kHz+无脉冲输出
//#define DisEn32K1Hz	INTCN				//0x0004		无输出					
//#endif																				//新国网		

#define EN32K1Hz_8025T		CSEL0+UIE			//0x6000	32kHz+秒脉冲输出 2秒刷新温度系数		//新国网		
#define EN32K_8025T			CSEL0				//0x4000	32kHz+无脉冲输出						//新国网			

#define EN32K1Hz_DS3231		EN32kHz				//0x0800		32kHz+秒脉冲输出					//新国网			
#define EN32K_DS3231		EN32kHz+INTCN		//0x0804		32kHz+无脉冲输出					//新国网		
#define DisEn32K1Hz_DS3231	INTCN				//0x0004		无输出								//新国网		

#define RecordSAds	0xC000	//24C512中
#define RecordEAds	0xD5E0	//24C512中
#define LoadBLen	26			//2+2+2+2+2+2+2+4+4+3=25 +1(校验) = 26
#define RecordBLen	28			//6+1(校验)+1+1+2+2+2+2+2+2+2+4+1(校验) = 28	

#if ( MEASCHIP == ADE7758 )

//#0C098H----	ADDRESSL,#00H---------LONG,#14t	
//(空1字节+A有功增益2字节+B有功增益2字节+C有功增益2字节+A无功增益2字节+B无功增益2字节+C无功增益2字节+校验字节）
//ADE7758校表数据
#define	YGAGN0	0x00	//空	
#define	YGAGN1	0x01	//有功无功校表参数存放区
#define	YGAGN2	0x02
#define	YGBGN1	0x03
#define	YGBGN2	0x04
#define	YGCGN1	0x05
#define	YGCGN2	0x06
#define	WGAGN1	0x07
#define	WGAGN2	0x08
#define	WGBGN1	0x09
#define	WGBGN2	0x0A
#define	WGCGN1	0x0B
#define	WGCGN2	0x0C

// #0C099H----	ADDRESSL,#XIANGJIAO---LONG,4T	
//（A相角补偿1字节+B相角补偿1字节+C相角补偿1字节）
#define XIANGJIAO 0x10

// #0C09AH----	ADDRESSL,#WTTOSD------LONG,7T	
//（A电能漂移2字节+B电能漂移2字节+C电能漂移2字节）
#define WTTOSD XIANGJIAO+4

// #0C09BH----	ADDRESSL,#zdydydlcs---LONG,13T	（
// A电压系数2字节+B电压系数2字节+C电压系数2字节+A电流系数2字节+B电流系数2字节+C电流系数2字节）
#define zdydydlcs WTTOSD+7

// #0C09CH		addressl,#DYAYUAN3-----long,18t	
//（A电压原始值3字节+B电压原始值3字节+C电压原始值3字节+A电流原始值3字节+B电流原始值3字节+C电流原始值3字节）
//		;读电压电流原始值（即从7758读出来的值）

//#0C09DH----	ADDRESSL,---------LONG,#14t	
//(空1字节+A有功增益2字节+B有功增益2字节+C有功增益2字节+A无功增益2字节+B无功增益2字节+C无功增益2字节+校验字节）
//ADE7758校表数据
#define	YGAGN048Hz	zdydydlcs+13	//48Hz参数	

//#0C09DH----	ADDRESSL,---------LONG,#14t	
//(空1字节+A有功增益2字节+B有功增益2字节+C有功增益2字节+A无功增益2字节+B无功增益2字节+C无功增益2字节+校验字节）
//ADE7758校表数据
#define	YGAGN052Hz	YGAGN048Hz+14	//52Hz参数	

#else
#endif

// Time Backup

/*		//2016.09.19
#define PhAFail_No			0			//A相失压记录号				
#define PhBFail_No			1			//B相失压记录号				
#define PhCFail_No			2			//C相失压记录号				
#define PhALower_No			3			//A相欠压记录号				
#define PhBLower_No			4			//B相欠压记录号				
#define PhCLower_No			5			//C相欠压记录号				
#define PhAOver_No			6			//A相过压记录号				
#define PhBOver_No			7			//B相过压记录号				
#define PhCOver_No			8			//C相过压记录号				
#define PhABreak_No			9			//A相断相记录号				
#define PhBBreak_No			10			//B相断相记录号				
#define PhCBreak_No			11			//C相断相记录号				
#define AllFail_No			12			//全失压记录号				
#define AuxPWFail_No		13			//辅助电源失压记录号				
#define VoltRevPhase_No		14			//电压逆相序记录号				
#define CuRevPhase_No		15			//电流逆相序记录号				
#define VoltImbalance_No	16			//电压不平衡记录号				
#define CuImbalance_No		17			//电流不平衡记录号
#define CuBestImbalance_No	18			//电流严重不平衡记录号				
#define CuAFail_No			19			//A相失流记录号				
#define CuBFail_No			20			//B相失流记录号				
#define CuCFail_No			21			//C相失流记录号				
#define CuAOver_No			22			//A相过流记录号				
#define CuBOver_No			23			//B相过流记录号
#define CuCOver_No			24			//C相过流记录号
#define CuABreak_No			25			//A相断流记录号				
#define CuBBreak_No			26			//B相断流记录号				
#define CuCBreak_No			27			//C相断流记录号				
#define RevPowerA_No		28			//A相潮流反向记录号
#define RevPowerB_No		29			//B相潮流反向记录号
#define RevPowerC_No		30			//C相潮流反向记录号
#define PowerOverA_No		31			//A相过载记录号
#define PowerOverB_No		32			//B相过载记录号
#define PowerOverC_No		33			//C相过载记录号
#define PftOver_No			34			//总功率因素超限记录号

#define VoltCheck_No		35			//电压合格率统计记录号 
#define VoltACheck_No		36			//A相电压合格率统计记录号 
#define VoltBCheck_No		37			//B相电压合格率统计记录号 
#define VoltCCheck_No		38			//C相电压合格率统计记录号 

#define PdownRec_No			39			//下电记录号

#define MDOverPp_No			40			//正向有功需量超限记录号
#define MDOverPn_No			41			//反向有功需量超限记录号
#define MDOverQ1_No			42			//1象限无功需量超限记录号
#define MDOverQ2_No			43			//2象限无功需量超限记录号
#define MDOverQ3_No			44			//3象限无功需量超限记录号
#define MDOverQ4_No			45			//4象限无功需量超限记录号

#define PrgRec_No			46			//编程记录号
#define ECClr_No			47			//电量清零记录号
#define MDClr_No			48			//需量清零记录号
#define EventClr_No			49			//事件清零记录号
#define TimeSet_No			50			//校时记录号
#define SegTabPrg_No		51			//时段表编程记录号
#define ZonePrg_No			52			//时区编程记录号
#define WeekCDPrg_No		53			//周休日编程记录号
#define HolidayPrg_No		54			//节假日编程记录号
#define ECPModePrg_No		55			//有功组合方式编程记录号
#define ECQ1ModePrg_No		56			//无功组合方式1编程记录号
#define ECQ2ModePrg_No		57			//无功组合方式2编程记录号
#define SaveDayPrg_No		58			//结算日编程记录号
#define OpenCover_No		59			//开表盖记录号
#define OpenTerminal_No		60			//开端钮盒记录号
#define BuyEC_No			61			//购电量记录号
#define BuyMoney_No			62			//购电金额记录号
#define TripOFF_No			63			//跳闸记录号
#define TripON_No			64			//合闸记录号

#define AppointFreeze_No	65			//定时冻结记录号
#define InstantFreeze_No	66			//瞬时冻结记录号 				
#define ZoneSWFreeze_No		67			//时区表切换冻结记录号 				
#define SegTabSWFreeze_No	68			//时段表切换冻结记录号 				
#define FeeSWFreeze_No		69			//费率电价切换冻结记录号 				
#define StepSWFreeze_No		70			//阶梯切换冻结记录号 				
#define HourFreeze_No		71			//整点冻结记录号 				
#define DayFreeze_No		72			//日冻结记录号 				

#define LastEC_No			73			//历史电量记录号
#define LastMD_No			74			//历史需量量记录号

#define TideSwitch_No		75			//潮流反向记录号			//新国网

#define PftAOver_No			76			//A相功率因素超限记录号		//新国网
#define PftBOver_No			77			//B相功率因素超限记录号		//新国网
#define PftCOver_No			78			//C相功率因素超限记录号		//新国网

#define YearSaveTime_No		79			//年结算时间记录号			//新国网
#define MonthSaveTime_No	80			//月结算时间记录号			//新国网

#define FeePricePrg_No		81			//费率电价编程记录号 		//新国网		
#define StepParaPrg_No		82			//阶梯参数编程记录号 		//新国网		
#define SecretKeyPrg_No		83			//密钥编程记录号 			//新国网	
#define CardInFault_No		84			//异常插卡记录号 			//新国网	
#define FeeBack_No			85			//退费记录号 				//新国网
#define MagnetDisturb_No	86			//恒定磁场干扰记录号 		//新国网		
#define RelayFault_No		87			//负荷开关误动作记录号 		//新国网		
#define PowerFault_No		88			//电源异常记录号 			//新国网			
*/


#define PhFail_No			0			//总失压记录号				
#define PhAFail_No			1			//A相失压记录号				
#define PhBFail_No			2			//B相失压记录号				
#define PhCFail_No			3			//C相失压记录号				
#define PhLower_No			4			//总欠压记录号				
#define PhALower_No			5			//A相欠压记录号				
#define PhBLower_No			6			//B相欠压记录号				
#define PhCLower_No			7			//C相欠压记录号				
#define PhOver_No			8			//总过压记录号				
#define PhAOver_No			9			//A相过压记录号				
#define PhBOver_No			10			//B相过压记录号				
#define PhCOver_No			11			//C相过压记录号				
#define PhBreak_No			12			//总断相记录号				
#define PhABreak_No			13			//A相断相记录号				
#define PhBBreak_No			14			//B相断相记录号				
#define PhCBreak_No			15			//C相断相记录号				
#define CuFail_No			16			//总失流记录号				
#define CuAFail_No			17			//A相失流记录号				
#define CuBFail_No			18			//B相失流记录号				
#define CuCFail_No			19			//C相失流记录号				
#define CuOver_No			20			//总过流记录号				
#define CuAOver_No			21			//A相过流记录号				
#define CuBOver_No			22			//B相过流记录号
#define CuCOver_No			23			//C相过流记录号
#define CuBreak_No			24			//总断流记录号				
#define CuABreak_No			25			//A相断流记录号				
#define CuBBreak_No			26			//B相断流记录号				
#define CuCBreak_No			27			//C相断流记录号				
#define RevPower_No			28			//总潮流反向记录号
#define RevPowerA_No		29			//A相潮流反向记录号
#define RevPowerB_No		30			//B相潮流反向记录号
#define RevPowerC_No		31			//C相潮流反向记录号
#define PowerOver_No		32			//总过载记录号
#define PowerOverA_No		33			//A相过载记录号
#define PowerOverB_No		34			//B相过载记录号
#define PowerOverC_No		35			//C相过载记录号

/*
#define PhAFail_No			0			//A相失压记录号				
#define PhBFail_No			1			//B相失压记录号				
#define PhCFail_No			2			//C相失压记录号				
#define PhFail_No			3			//总失压记录号				
#define PhALower_No			4			//A相欠压记录号				
#define PhBLower_No			5			//B相欠压记录号				
#define PhCLower_No			6			//C相欠压记录号				
#define PhLower_No			7			//总欠压记录号				
#define PhAOver_No			8			//A相过压记录号				
#define PhBOver_No			9			//B相过压记录号				
#define PhCOver_No			10			//C相过压记录号				
#define PhOver_No			11			//总过压记录号				
#define PhABreak_No			12			//A相断相记录号				
#define PhBBreak_No			13			//B相断相记录号				
#define PhCBreak_No			14			//C相断相记录号				
#define PhBreak_No			15			//总断相记录号				
#define CuAFail_No			16			//A相失流记录号				
#define CuBFail_No			17			//B相失流记录号				
#define CuCFail_No			18			//C相失流记录号				
#define CuFail_No			19			//总失流记录号				
#define CuAOver_No			20			//A相过流记录号				
#define CuBOver_No			21			//B相过流记录号
#define CuCOver_No			22			//C相过流记录号
#define CuOver_No			23			//总过流记录号				
#define CuABreak_No			24			//A相断流记录号				
#define CuBBreak_No			25			//B相断流记录号				
#define CuCBreak_No			26			//C相断流记录号				
#define CuBreak_No			27			//总断流记录号				
#define RevPowerA_No		28			//A相潮流反向记录号
#define RevPowerB_No		29			//B相潮流反向记录号
#define RevPowerC_No		30			//C相潮流反向记录号
#define RevPower_No			31			//总潮流反向记录号
#define PowerOverA_No		32			//A相过载记录号
#define PowerOverB_No		33			//B相过载记录号
#define PowerOverC_No		34			//C相过载记录号
#define PowerOver_No		35			//总过载记录号
*/
#define MDOverPp_No			36			//正向有功需量超限记录号
#define MDOverPn_No			37			//反向有功需量超限记录号
#define MDOverQ1_No			38			//1象限无功需量超限记录号
#define MDOverQ2_No			39			//2象限无功需量超限记录号
#define MDOverQ3_No			40			//3象限无功需量超限记录号
#define MDOverQ4_No			41			//4象限无功需量超限记录号
#define PftOver_No			42			//总功率因素超限记录号
                                            
#define AllFail_No			43			//全失压记录号				
#define AuxPWFail_No		44			//辅助电源失压记录号				
#define VoltRevPhase_No		45			//电压逆相序记录号				
#define CuRevPhase_No		46			//电流逆相序记录号				
#define PdownRec_No			47			//下电记录号
#define PrgRec_No			48			//编程记录号
                                            
#define ECClr_No			49			//电量清零记录号
#define MDClr_No			50			//需量清零记录号
#define EventClr_No			51			//事件清零记录号
#define TimeSet_No			52			//校时记录号
#define SegTabPrg_No		53			//时段表编程记录号
#define ZonePrg_No			54			//时区编程记录号
#define WeekCDPrg_No		55			//周休日编程记录号
#define SaveDayPrg_No		56			//结算日编程记录号
#define OpenCover_No		57			//开表盖记录号
#define OpenTerminal_No		58			//开端钮盒记录号
                                            
#define VoltImbalance_No	59			//电压不平衡记录号				
#define CuImbalance_No		60			//电流不平衡记录号
#define TripOFF_No			61			//跳闸记录号
#define TripON_No			62			//合闸记录号
                                            
#define HolidayPrg_No		63			//节假日编程记录号
#define ECPModePrg_No		64			//有功组合方式编程记录号
#define ECQModePrg_No		65			//无功组合方式编程记录号                            
#define FeePricePrg_No		66			//费率电价编程记录号 		//新国网		
#define StepParaPrg_No		67			//阶梯参数编程记录号 		//新国网		
#define SecretKeyPrg_No		68			//密钥编程记录号 			//新国网	
#define CardInFault_No		69			//异常插卡记录号 			//新国网	
#define BuyMoney_No			70			//购电金额记录号
#define FeeBack_No			71			//退费记录号 				//新国网
#define MagnetDisturb_No	72			//恒定磁场干扰记录号 		//新国网		
#define RelayFault_No		73			//负荷开关误动作记录号 		//新国网		
#define PowerFault_No		74			//电源异常记录号 			//新国网			
                                            
#define CuBestImbalance_No	75			//电流严重不平衡记录号				
#define ClockErr_No			76			//时钟故障
#define MeterICErr_No		77			//电能计量芯片故障
#define ComModuleChange_No	78			//通讯模块变更


/*
#define InstantFreeze_No	0			//瞬时冻结记录号 				
#define SecFreeze_No		1			//秒冻结记录号 				
#define MinFreeze_No		2			//分钟冻结记录号 				
#define HourFreeze_No		3			//小时冻结记录号 				
#define DayFreeze_No		4			//日冻结记录号 				
#define SaveDayFreeze_No	5			//结算日冻结记录号 				
#define MonthFreeze_No		6			//月冻结记录号 				
#define YearFreeze_No		8			//年冻结记录号 				
#define ZoneSWFreeze_No		9			//时区表切换冻结记录号 				
#define SegTabSWFreeze_No	10			//日时段表切换冻结记录号 				
#define FeeSWFreeze_No		11			//费率电价切换冻结记录号 				
#define StepSWFreeze_No		12			//阶梯切换冻结记录号 				
#define StepSaveSWFreeze_No	13			//阶梯结算冻结记录号 				
*/

#define InstantFreeze_No	0			//瞬时冻结记录号 				
#define SecFreeze_No		1			//秒冻结记录号 				
#define MinFreeze_No		2			//分钟冻结记录号 				
#define HourFreeze_No		3			//小时冻结记录号 				
#define DayFreeze_No		4			//日冻结记录号 				
#define SaveDayFreeze_No	5			//结算日冻结记录号 				
#define MonthFreeze_No		6			//月冻结记录号 				
#define YearFreeze_No		7			//年冻结记录号 				
#define ZoneSWFreeze_No		8			//时区表切换冻结记录号 				
#define SegTabSWFreeze_No	9			//日时段表切换冻结记录号 				
#define FeeSWFreeze_No		10			//费率电价切换冻结记录号 				
#define StepSWFreeze_No		11			//阶梯切换冻结记录号 				
#define StepSaveSWFreeze_No	12			//阶梯结算冻结记录号 				

