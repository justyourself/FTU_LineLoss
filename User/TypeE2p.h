#include "TypeMeter.h"

//铁电存储器中保存当前所有电量，有一个备份(占前4页 4*2*128 )
//和当前有功正向、当前有功反向、当前视在、一、二、三、四象限需量缓存区（60分钟、120字节），无备份(后4页 4*2*128)
/***********************************************/

// 第一片24C512中存储的内容：
// 1：当前所有电量、需量及需量发生时间；
// 2：历史最近1-12个月的所有电量、需量及需量发生时间
// 3：节假日、周休日、季节、时段等电表参数
// 4：轮显项等参数
#define CMon_EC_Pp0                     0
#define	CMon_EC_Pn0                     CMon_EC_Pp0+5
#define CMon_EC_Qp0                     CMon_EC_Pn0+5
#define CMon_EC_Qn0                     CMon_EC_Qp0+5
#define CMon_EC_Pp1                     CMon_EC_Qn0+5
#define	CMon_EC_Pn1                     CMon_EC_Pp1+5
#define CMon_EC_Qp1                     CMon_EC_Pn1+5
#define CMon_EC_Qn1                     CMon_EC_Qp1+5
#define CMon_EC_Pp2                     CMon_EC_Qn1+5
#define	CMon_EC_Pn2                     CMon_EC_Pp2+5
#define CMon_EC_Qp2                     CMon_EC_Pn2+5
#define CMon_EC_Qn2                     CMon_EC_Qp2+5
#define CMon_EC_Pp3                     CMon_EC_Qn2+5
#define	CMon_EC_Pn3                     CMon_EC_Pp3+5
#define CMon_EC_Qp3                     CMon_EC_Pn3+5
#define CMon_EC_Qn3                     CMon_EC_Qp3+5
#define CMon_EC_Pp4                     CMon_EC_Qn3+5
#define	CMon_EC_Pn4                     CMon_EC_Pp4+5
#define CMon_EC_Qp4                     CMon_EC_Pn4+5
#define CMon_EC_Qn4                     CMon_EC_Qp4+5
#define CMon_EC_Pp5                     CMon_EC_Qn4+5
#define	CMon_EC_Pn5                     CMon_EC_Pp5+5
#define CMon_EC_Qp5                     CMon_EC_Pn5+5
#define CMon_EC_Qn5                     CMon_EC_Qp5+5
#define CMon_EC_Pp6                     CMon_EC_Qn5+5
#define	CMon_EC_Pn6                     CMon_EC_Pp6+5
#define CMon_EC_Qp6                     CMon_EC_Pn6+5
#define CMon_EC_Qn6                     CMon_EC_Qp6+5
#define CMon_EC_Pp7                     CMon_EC_Qn6+5
#define	CMon_EC_Pn7                     CMon_EC_Pp7+5
#define CMon_EC_Qp7                     CMon_EC_Pn7+5
#define CMon_EC_Qn7                     CMon_EC_Qp7+5
#define CMon_EC_Q01                     CMon_EC_Qn7+5
#define	CMon_EC_Q02                     CMon_EC_Q01+5
#define CMon_EC_Q03                     CMon_EC_Q02+5
#define CMon_EC_Q04                     CMon_EC_Q03+5
#define CMon_EC_Q11                     CMon_EC_Q04+5
#define	CMon_EC_Q12                     CMon_EC_Q11+5
#define CMon_EC_Q13                     CMon_EC_Q12+5
#define CMon_EC_Q14                     CMon_EC_Q13+5
#define CMon_EC_Q21                     CMon_EC_Q14+5
#define	CMon_EC_Q22                     CMon_EC_Q21+5
#define CMon_EC_Q23                     CMon_EC_Q22+5
#define CMon_EC_Q24                     CMon_EC_Q23+5
#define CMon_EC_Q31                     CMon_EC_Q24+5
#define	CMon_EC_Q32                     CMon_EC_Q31+5
#define CMon_EC_Q33                     CMon_EC_Q32+5
#define CMon_EC_Q34                     CMon_EC_Q33+5
#define CMon_EC_Q41                     CMon_EC_Q34+5
#define	CMon_EC_Q42                     CMon_EC_Q41+5
#define CMon_EC_Q43                     CMon_EC_Q42+5
#define CMon_EC_Q44                     CMon_EC_Q43+5
#define CMon_EC_Q51                     CMon_EC_Q44+5
#define	CMon_EC_Q52                     CMon_EC_Q51+5
#define CMon_EC_Q53                     CMon_EC_Q52+5
#define CMon_EC_Q54                     CMon_EC_Q53+5
#define CMon_EC_Q61                     CMon_EC_Q54+5
#define	CMon_EC_Q62                     CMon_EC_Q61+5
#define CMon_EC_Q63                     CMon_EC_Q62+5
#define CMon_EC_Q64                     CMon_EC_Q63+5
#define CMon_EC_Q71                     CMon_EC_Q64+5
#define	CMon_EC_Q72                     CMon_EC_Q71+5
#define CMon_EC_Q73                     CMon_EC_Q72+5
#define CMon_EC_Q74                     CMon_EC_Q73+5  //结束地址:0x0140

#define CMon_DAY0                       CMon_EC_Q74+5  
#define CDay_HOUR0                      CMon_DAY0+5
#define CDay_InvertVal0                 CDay_HOUR0+5
#define CDay_InvertTime0                CDay_InvertVal0+5
#define CDay_TrendVal0                  CDay_InvertTime0+5
#define CDay_TrendTime0                 CDay_TrendVal0+5

#define CMon_DAY1                       CDay_TrendTime0+5  
#define CDay_HOUR1                      CMon_DAY1+5
#define CDay_InvertVal1                 CDay_HOUR1+5
#define CDay_InvertTime1                CDay_InvertVal1+5
#define CDay_TrendVal1                  CDay_InvertTime1+5
#define CDay_TrendTime1                 CDay_TrendVal1+5

#define CMon_DAY2                       CDay_TrendTime1+5  
#define CDay_HOUR2                      CMon_DAY2+5
#define CDay_InvertVal2                 CDay_HOUR2+5
#define CDay_InvertTime2                CDay_InvertVal2+5
#define CDay_TrendVal2                  CDay_InvertTime2+5
#define CDay_TrendTime2                 CDay_TrendVal2+5

#define CMon_DAY3                       CDay_TrendTime2+5  
#define CDay_HOUR3                      CMon_DAY3+5
#define CDay_InvertVal3                 CDay_HOUR3+5
#define CDay_InvertTime3                CDay_InvertVal3+5
#define CDay_TrendVal3                  CDay_InvertTime3+5
#define CDay_TrendTime3                 CDay_TrendVal3+5

#define CMon_DAY4                       CDay_TrendTime3+5  
#define CDay_HOUR4                      CMon_DAY4+5
#define CDay_InvertVal4                 CDay_HOUR4+5
#define CDay_InvertTime4                CDay_InvertVal4+5
#define CDay_TrendVal4                  CDay_InvertTime4+5
#define CDay_TrendTime4                 CDay_TrendVal4+5

#define CMon_DAY5                       CDay_TrendTime4+5  
#define CDay_HOUR5                      CMon_DAY5+5
#define CDay_InvertVal5                 CDay_HOUR5+5
#define CDay_InvertTime5                CDay_InvertVal5+5
#define CDay_TrendVal5                  CDay_InvertTime5+5
#define CDay_TrendTime5                 CDay_TrendVal5+5

#define CMon_DAY6                       CDay_TrendTime5+5  
#define CDay_HOUR6                      CMon_DAY6+5
#define CDay_InvertVal6                 CDay_HOUR6+5
#define CDay_InvertTime6                CDay_InvertVal6+5
#define CDay_TrendVal6                  CDay_InvertTime6+5
#define CDay_TrendTime6                 CDay_TrendVal6+5

#define CMon_DAY7                       CDay_TrendTime6+5  
#define CDay_HOUR7                      CMon_DAY7+5
#define CDay_InvertVal7                 CDay_HOUR7+5
#define CDay_InvertTime7                CDay_InvertVal7+5
#define CDay_TrendVal7                  CDay_InvertTime7+5
#define CDay_TrendTime7                 CDay_TrendVal7+5

#define	ClockAdj			CDay_TrendTime7+4	//（2）//结束地址:0x088D

//事件参数均改为HEX码
#define	EPhFail_Valve		          ClockAdj+6		          //失压起始阀值(3)			yyy.y V 
#define AT7022ChkSum		          EPhFail_Valve+7		//7022校表数据校验和(4)		//ATChk		//V1000
#define AT7022ChkSum2		          AT7022ChkSum+6		//7022e校表数据校验和2(4)		//ATChk		//V1000

#define IEC_LINK_ADDR                    ClockAdj+6      //2
#define IEC_COMM_ADDR                    IEC_LINK_ADDR+4  //2
#define	LoadRecord_Time		         IEC_COMM_ADDR+4 //符合去的时间，指针记录
#define	RandRecord_Time                  LoadRecord_Time+10 //瞬时冻结
#define	FrzdRecord_Time                  RandRecord_Time+10  //日冻结
#define	ShrpdRecord_Time                 FrzdRecord_Time+10 //整点冻结
#define	MonthdRecord_Time                ShrpdRecord_Time+10  //考核日冻结

#define	CH0_PAN_EVT_PT                   MonthdRecord_Time+10
#define	CH0_PBN_EVT_PT                   CH0_PAN_EVT_PT+3
#define	CH0_PCN_EVT_PT                   CH0_PBN_EVT_PT+3
#define	CH0_PTD_EVT_PT                   CH0_PCN_EVT_PT+3
#define	CH0_CLR_EVT_PT                   CH0_PCN_EVT_PT+3

#define	CH1_PAN_EVT_PT                   CH0_CLR_EVT_PT+3
#define	CH1_PBN_EVT_PT                   CH1_PAN_EVT_PT+3
#define	CH1_PCN_EVT_PT                   CH1_PBN_EVT_PT+3
#define	CH1_PTD_EVT_PT                   CH1_PCN_EVT_PT+3
#define	CH1_CLR_EVT_PT                   CH1_PCN_EVT_PT+3

#define	CH2_PAN_EVT_PT                   CH1_CLR_EVT_PT+3
#define	CH2_PBN_EVT_PT                   CH2_PAN_EVT_PT+3
#define	CH2_PCN_EVT_PT                   CH2_PBN_EVT_PT+3
#define	CH2_PTD_EVT_PT                   CH2_PCN_EVT_PT+3
#define	CH2_CLR_EVT_PT                   CH2_PCN_EVT_PT+3

#define	CH3_PAN_EVT_PT                   CH2_CLR_EVT_PT+3
#define	CH3_PBN_EVT_PT                   CH3_PAN_EVT_PT+3
#define	CH3_PCN_EVT_PT                   CH3_PBN_EVT_PT+3
#define	CH3_PTD_EVT_PT                   CH3_PCN_EVT_PT+3
#define	CH3_CLR_EVT_PT                   CH3_PCN_EVT_PT+3

#define	CH4_PAN_EVT_PT                   CH3_CLR_EVT_PT+3
#define	CH4_PBN_EVT_PT                   CH4_PAN_EVT_PT+3
#define	CH4_PCN_EVT_PT                   CH4_PBN_EVT_PT+3
#define	CH4_PTD_EVT_PT                   CH4_PCN_EVT_PT+3
#define	CH4_CLR_EVT_PT                   CH4_PCN_EVT_PT+3

#define	CH5_PAN_EVT_PT                   CH4_CLR_EVT_PT+3
#define	CH5_PBN_EVT_PT                   CH5_PAN_EVT_PT+3
#define	CH5_PCN_EVT_PT                   CH5_PBN_EVT_PT+3
#define	CH5_PTD_EVT_PT                   CH5_PCN_EVT_PT+3
#define	CH5_CLR_EVT_PT                   CH5_PCN_EVT_PT+3

#define	CH6_PAN_EVT_PT                   CH5_CLR_EVT_PT+3
#define	CH6_PBN_EVT_PT                   CH6_PAN_EVT_PT+3
#define	CH6_PCN_EVT_PT                   CH6_PBN_EVT_PT+3
#define	CH6_PTD_EVT_PT                   CH6_PCN_EVT_PT+3
#define	CH6_CLR_EVT_PT                   CH6_PCN_EVT_PT+3

#define	CH7_PAN_EVT_PT                   CH6_CLR_EVT_PT+3
#define	CH7_PBN_EVT_PT                   CH7_PAN_EVT_PT+3
#define	CH7_PCN_EVT_PT                   CH7_PBN_EVT_PT+3
#define	CH7_PTD_EVT_PT                   CH7_PCN_EVT_PT+3
#define	CH7_CLR_EVT_PT                   CH7_PCN_EVT_PT+3

#define ErrorRegAddr			 E2P_PGLEN-0x800

#define E2P_PGLEN		         0x1000//0x0B00//0xAAA//0x1554		//数据备份页长度

#define E2ONE_RECORD_LEN                 54
#define MONTH_RECORD_NUM                 12
#define E2ONE_RECORD_SIZE                60
#define MONTH_RECORD_SIZE                (MONTH_RECORD_NUM*E2ONE_RECORD_SIZE)

#define RAND_RECORD_NUM                   3
#define RAND_RECORD_SIZE                (RAND_RECORD_NUM*E2ONE_RECORD_SIZE)

#define MONTH_DATA_ADDR                  0
#define RAND_DATA_ADDR                   0x1680

#define CH0_PAN_EVT_ADDR                RAND_DATA_ADDR+0x05A0
#define CH0_PBN_EVT_ADDR                CH0_PAN_EVT_ADDR+840
#define CH0_PCN_EVT_ADDR                CH0_PBN_EVT_ADDR+840
#define CH0_PTD_EVT_ADDR                CH0_PCN_EVT_ADDR+840
#define CH0_CLR_ADDR                    CH0_PTD_EVT_ADDR+440

#define CH1_PAN_EVT_ADDR                CH0_CLR_ADDR+440
#define CH1_PBN_EVT_ADDR                CH1_PAN_EVT_ADDR+840
#define CH1_PCN_EVT_ADDR                CH1_PBN_EVT_ADDR+840
#define CH1_PTD_EVT_ADDR                CH1_PCN_EVT_ADDR+840
#define CH1_CLR_ADDR                    CH1_PTD_EVT_ADDR+440

#define CH2_PAN_EVT_ADDR                CH1_CLR_ADDR+440
#define CH2_PBN_EVT_ADDR                CH2_PAN_EVT_ADDR+840
#define CH2_PCN_EVT_ADDR                CH2_PBN_EVT_ADDR+840
#define CH2_PTD_EVT_ADDR                CH2_PCN_EVT_ADDR+840
#define CH2_CLR_ADDR                    CH2_PTD_EVT_ADDR+440

#define CH3_PAN_EVT_ADDR                CH2_CLR_ADDR+440
#define CH3_PBN_EVT_ADDR                CH3_PAN_EVT_ADDR+840
#define CH3_PCN_EVT_ADDR                CH3_PBN_EVT_ADDR+840
#define CH3_PTD_EVT_ADDR                CH3_PCN_EVT_ADDR+840
#define CH3_CLR_ADDR                    CH3_PTD_EVT_ADDR+440

#define CH4_PAN_EVT_ADDR                CH3_CLR_ADDR+440
#define CH4_PBN_EVT_ADDR                CH4_PAN_EVT_ADDR+840
#define CH4_PCN_EVT_ADDR                CH4_PBN_EVT_ADDR+840
#define CH4_PTD_EVT_ADDR                CH4_PCN_EVT_ADDR+840
#define CH4_CLR_ADDR                    CH4_PTD_EVT_ADDR+440

#define CH5_PAN_EVT_ADDR                CH4_CLR_ADDR+440
#define CH5_PBN_EVT_ADDR                CH5_PAN_EVT_ADDR+840
#define CH5_PCN_EVT_ADDR                CH5_PBN_EVT_ADDR+840
#define CH5_PTD_EVT_ADDR                CH5_PCN_EVT_ADDR+840
#define CH5_CLR_ADDR                    CH5_PTD_EVT_ADDR+440

#define CH6_PAN_EVT_ADDR                CH5_CLR_ADDR+440
#define CH6_PBN_EVT_ADDR                CH6_PAN_EVT_ADDR+840
#define CH6_PCN_EVT_ADDR                CH6_PBN_EVT_ADDR+840
#define CH6_PTD_EVT_ADDR                CH6_PCN_EVT_ADDR+840
#define CH6_CLR_ADDR                    CH6_PTD_EVT_ADDR+440

#define CH7_PAN_EVT_ADDR                CH6_CLR_ADDR+440
#define CH7_PBN_EVT_ADDR                CH7_PAN_EVT_ADDR+840
#define CH7_PCN_EVT_ADDR                CH7_PBN_EVT_ADDR+840
#define CH7_PTD_EVT_ADDR                CH7_PCN_EVT_ADDR+840
#define CH7_CLR_ADDR                    CH7_PTD_EVT_ADDR+440

#define E2END_DATA_ADDR                 (CH7_CLR_ADDR+440)


#define ONE_RECORD_LEN                   54
#define LOAD_RECORD_NUM                  96
#define ONE_RECORD_SIZE                  64
#define LOAD_RECORD_SIZE                 (LOAD_RECORD_NUM*ONE_RECORD_SIZE)

#define DAY_RECORD_NUM                   40
#define DAY_RECORD_SIZE                  (DAY_RECORD_NUM*ONE_RECORD_SIZE)

#define HOUR_RECORD_NUM                  264
#define HOUR_RECORD_SIZE                 (HOUR_RECORD_NUM*ONE_RECORD_SIZE)


//FLASH
#define LOAD_DATA_ADDR                   0
#define DAY_DATA_ADDR                    0xC000 //负荷记录留48k空间
#define HOUR_DATA_ADDR                  (0xC000+0x5000) //20k空间
#define END_DATA_ADDR                  (HOUR_DATA_ADDR+0x21000) //132K空间


#define FlashChip		YesCheck

///////////   FLASH	//前300页为负荷记录
#if (FlashChip == YesCheck)
#define	Mon_Data_Addr	 0x25800//0x19000	//(1840)三页多
#else 
#define	Mon_Data_Addr	 0x7000///0x1554//0x19000	//(1840)三页多
#endif
#define	Tamper_Data_Addr	 Mon_Data_Addr + 0xA00	//(6000)



#define FlashAT45DB		NoCheck		//ATMEL的AT25DF041A系列芯片 AT26DF081、AT26DF161
#define FlashAT161Series	YesCheck	//ATMEL的AT45161芯片 容量2M字节

//#define GeneralLoadCurveSta		0x00000000		//0
//#define GeneralLoadCurveEnd		0x00090000		//384K  普通负荷曲线范围，
//#define GeneralMonCurveSta		0x00090000		//0
//#define GeneralMonCurveEnd		0x000A0000		//64K  历史数据范围，
//#define GeneralTamperCurveSta	0x000A0000		//0
//#define GeneralTamperCurveEnd	0x000B0000		//64K  事件记录范围，

//------------------DLMS Profile tab数据存储----------------
#define EventLog1Page					0x00000000
///#define EventLog1Page					GeneralTamperCurveEnd			//事件1记录首页码(6)
#define EventLog2Page					EventLog1Page+6*1						//事件2记录首页码(6)
#define EventLog3Page					EventLog1Page+6*2						//事件3记录首页码(6)
#define EventLog4Page					EventLog1Page+6*3						//事件4记录首页码(6)
#define EventLog5Page					EventLog1Page+6*4						//事件5记录首页码(6)
#define EventLog6Page					EventLog1Page+6*5						//事件6记录首页码(6)
#define EventLog7Page					EventLog1Page+6*6						//事件7记录首页码(6)
#define EventLog8Page					EventLog1Page+6*7						//事件8记录首页码(6)
#define EventLog9Page					EventLog1Page+6*8						//事件9记录首页码(6)
#define EventLog10Page					EventLog1Page+6*9						//事件10记录首页码(6)
#define EventLog11Page					EventLog1Page+6*10					//事件11记录首页码(6)

#define Billing_period1Page				EventLog11Page+6*1				//账单记录1首页码(6)		//12个月结算
#define Billing_period2Page				EventLog11Page+6*2			//账单记录1首页码(20)		//31天冻结
#define Load_profile1Page				EventLog11Page+6*3			//负荷曲线1首页码(20)		
#define Load_profile2Page				Load_profile1Page+40				//负荷曲线1首页码(20)	
#define PowerQuality_profilePage			Load_profile2Page+6				//负荷曲线2首页码(200)		

#define Profile_Page_end				PowerQuality_profilePage+40

#define GeneralMonCurveSta		Profile_Page_end		//0
#define GeneralMonCurveEnd		Profile_Page_end+0x10000		//64K  历史数据范围，
#define GeneralTamperCurveSta	GeneralMonCurveEnd		//0
//#define GeneralTamperCurveEnd	0x000E0000		//64K  事件记录范围，
#define GeneralTamperCurveEnd	GeneralTamperCurveSta+0x10000		//64K  事件记录范围，
#define GeneralLoadCurveSta		GeneralTamperCurveEnd		//0
#define GeneralLoadCurveEnd		GeneralLoadCurveSta+0x60000		//384K  普通负荷曲线范围，


//以下是按照4MFLASH进行操作的，升级部分放到FLASH的最后512K字节部分..
//定义FLASH存储空间，目前定义空间为AT25DF041A的最后（256*2+4）K =516K字节的空间。
//此FLASH每一页为4K即4096字节，檫除也只能是4K一檫除
// 2 BYTE
#define FLASH_512	 0x200  			//512..
#define FLASH_4K	 0x1000  			//4096  = 4*1024..
#define FLASH_16K	 0x4000  			//16384 = 16*1024..
// 3 BYTE
#define FLASH_64K	 0x10000
#define FLASH_252K	 0x3F000			//252K..
#define FLASH_256K	 0x40000			//256K..
#define FLASH_184K	 0x2E000	//8213内部flash存储用户程序的空间192K-8K=184K

//升级程序参数--备用..
//895 37F000h 37FFFFh
#define NEW_FIRMWARE_STATE			(unsigned long)(FLASH_4K)*383 				// 37F000h = 895*4K(1024) //升级初始化，前面4个字节初始化为：0xA5 B6 C7 D8
// page
// 4K = 4096
// 升级程序暂时定义为：128+252K字节（包括8个字节的CRC校验）如需要扩充到256K自己的升级，则需要在每块后面扩4K，即252K-->256K..
//升级程序改成可变长度，只要不小于104个字节一块就可以，地址则也需做灵活处理程序里认起始和结束地址..
// NEW FIRMWARE UPGRADE -..
#define UPGRADE_NEW_START				(unsigned long)(FLASH_4K)*384					// 0x180000 = 384*4K(1024) 
//4k --暂时定义存放128字节用，以及版本信息等..
#define UPGRADE_START_PROGRAM		UPGRADE_NEW_START + FLASH_4K 					// 388*4K(1024)
#define UPGRADE_VECTOR_START 	 	UPGRADE_NEW_START + 73*FLASH_512 			// 中断向量表起始地址..
#define UPGRADE_VECTOR_END 	    	UPGRADE_NEW_START + 74*FLASH_512 			// 中断向量结束也是0x10000地址的开始 <1---74>..
//<75---504>.. 
#define UPGRADE_END_PROGRAM			UPGRADE_START_PROGRAM + FLASH_252K 		// 898--959 //4K(1024)

// 保存升级前的程序 0LD FIRMWARE
#define CURRENT_NEW_START				(unsigned long)(FLASH_4K)*448					//(384+64)*FLASH_4K) 									
// 4k---存放当前文件，不使用128个字节头文件，直接存放当前512字节的应用程序中断向量表，檫除应用程序..
#define CURRENT_START_PROGRAM		CURRENT_NEW_START + FLASH_4K 					// 452*4K(1024)

#define CURRENT_VECTOR_START 	    CURRENT_NEW_START + 73*FLASH_512 			// 当前中断向量表起始地址..
#define CURRENT_VECTOR_END 	    	CURRENT_NEW_START + 74*FLASH_512 			// 当前中断向量结束也是0x10000地址的开始 <1---74>..
//<75---504> ，一共是504块512个字节数据..
#define CURRENT_END_PROGRAM			CURRENT_START_PROGRAM + FLASH_252K 		// 898--959 //4K(1024)

