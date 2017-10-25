#include "TypeMeter.h"
//#define CMonItems	123		//当前月电量、需量、需量时间总项数
//#define	CMonLength	574		//当前月电量、需量、需量时间总长度

//铁电存储器中保存当前所有电量，有一个备份(占前4页 4*2*128 )
//和当前有功正向、当前有功反向、当前视在、一、二、三、四象限需量缓存区（60分钟、120字节），无备份(后4页 4*2*128)
/***********************************************/

// 第一片24C512中存储的内容：
// 1：当前所有电量、需量及需量发生时间；
// 2：历史最近1-12个月的所有电量、需量及需量发生时间
// 3：节假日、周休日、季节、时段等电表参数
// 4：轮显项等参数
//校表参数 放在前100字节
#define EE_CAL_IA			 0x0000			//IA(3)
#define EE_CAL_IB			 EE_CAL_IA+3		//IB(3)
#define EE_CAL_PA			 EE_CAL_IA+3*2		//PA(3)
#define EE_CAL_PB			 EE_CAL_IA+3*3		//PB(3)
#define EE_CAL_V			 EE_CAL_IA+3*4		//V(3)
#define EE_PHADJ_A			 EE_CAL_IA+3*5		//A相相位(3)
#define EE_PHADJ_B			 EE_CAL_IA+3*6		//B相相位(3)
#define EE_Offset_A			 EE_CAL_IA+3*7		//A相Offset(3)
#define EE_Offset_B			 EE_CAL_IA+3*8		//B相Offset(3)
#define EMUStatusJY			 EE_Offset_B+3		//校验和(5)  
#define	ClockAdj			 EMUStatusJY+5				//（2）//结束地址:0x088D


#define IEC_LINK_ADDR                    ClockAdj+6      //2
#define IEC_COMM_ADDR                    IEC_LINK_ADDR+4  //2
#define	LoadRecord_Time		         IEC_COMM_ADDR+4 //符合去的时间，指针记录

#define LOAD_RECORD_NUM                  96
#define ONE_RECORD_SIZE                  40
#define LOAD_RECORD_SIZE                 (LOAD_RECORD_NUM*ONE_RECORD_SIZE)



#define E2P_PGLEN		0x1000//0x0B00//0xAAA//0x1554		//数据备份页长度


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

#define Profile_Page_end						PowerQuality_profilePage+40

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

