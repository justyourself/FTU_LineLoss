


//定义FLASH存储空间，该空间可以为一片独立的AT25DF041A或AT26DF161的前一部分空间。
//每一页为4K即4096字节，每一数据类型独立存放，单独占用2页或多页空间
//每一类型数据都多分配一页空间，作为数据轮换用。
//历史电量和历史需量有双备份，其余事件类数据无备份。																					至少记录数	
//#define AT7022DataZonePage			1800						// 校表数据存放区(1)		//10.11.02
#define AT7022DataZonePage				FLASH_SECTOR_COUNT			// 校表数据存放区(1)		//10.11.02
#define AT7022DataZonePageBak			AT7022DataZonePage+1			// 校表数据存放备份区(1)	//10.11.02

/*		//17.04.15																																//至少记录数	
#define Event_log0_1_Page				AT7022DataZonePageBak+1		//事件记录0记录表1首页码(2)	//失压 (class_id=24) 					以下全部为10条
#define Event_log0_2_Page				Event_log0_1_Page+2*1		//事件记录0记录表2首页码(2)				
#define Event_log0_3_Page				Event_log0_1_Page+2*2		//事件记录0记录表3首页码(2)				
#define Event_log0_4_Page				Event_log0_1_Page+2*3		//事件记录0记录表4首页码(2)				
#define Event_log1_1_Page				Event_log0_1_Page+2*4		//事件记录1记录表1首页码(2)	//欠压 (class_id=24) 					
#define Event_log1_2_Page				Event_log0_1_Page+2*5		//事件记录1记录表2首页码(2)				
#define Event_log1_3_Page				Event_log0_1_Page+2*6		//事件记录1记录表3首页码(2)				
#define Event_log1_4_Page				Event_log0_1_Page+2*7		//事件记录1记录表4首页码(2)				
#define Event_log2_1_Page				Event_log0_1_Page+2*8		//事件记录2记录表1首页码(2)	//过压 (class_id=24) 		
#define Event_log2_2_Page				Event_log0_1_Page+2*9		//事件记录2记录表2首页码(2)				
#define Event_log2_3_Page				Event_log0_1_Page+2*10		//事件记录2记录表3首页码(2)				
#define Event_log2_4_Page				Event_log0_1_Page+2*11		//事件记录2记录表4首页码(2)				
#define Event_log3_1_Page				Event_log0_1_Page+2*12		//事件记录3记录表1首页码(2)	//断相 (class_id=24) 		
#define Event_log3_2_Page				Event_log0_1_Page+2*13		//事件记录3记录表2首页码(2)				
#define Event_log3_3_Page				Event_log0_1_Page+2*14		//事件记录3记录表3首页码(2)				
#define Event_log3_4_Page				Event_log0_1_Page+2*15		//事件记录3记录表4首页码(2)				
#define Event_log4_1_Page				Event_log0_1_Page+2*16		//事件记录4记录表1首页码(2)	//失流 (class_id=24) 		
#define Event_log4_2_Page				Event_log0_1_Page+2*17		//事件记录4记录表2首页码(2)				
#define Event_log4_3_Page				Event_log0_1_Page+2*18		//事件记录4记录表3首页码(2)				
#define Event_log4_4_Page				Event_log0_1_Page+2*19		//事件记录4记录表4首页码(2)				
#define Event_log5_1_Page				Event_log0_1_Page+2*20		//事件记录5记录表1首页码(2)	//过流 (class_id=24) 		
#define Event_log5_2_Page				Event_log0_1_Page+2*21		//事件记录5记录表2首页码(2)				
#define Event_log5_3_Page				Event_log0_1_Page+2*22		//事件记录5记录表3首页码(2)				
#define Event_log5_4_Page				Event_log0_1_Page+2*23		//事件记录5记录表4首页码(2)				
#define Event_log6_1_Page				Event_log0_1_Page+2*24		//事件记录6记录表1首页码(2)	//断流 (class_id=24) 		
#define Event_log6_2_Page				Event_log0_1_Page+2*25		//事件记录6记录表2首页码(2)				
#define Event_log6_3_Page				Event_log0_1_Page+2*26		//事件记录6记录表3首页码(2)				
#define Event_log6_4_Page				Event_log0_1_Page+2*27		//事件记录6记录表4首页码(2)				
#define Event_log7_1_Page				Event_log0_1_Page+2*28		//事件记录7记录表1首页码(2)	//潮流反向 (class_id=24) 		
#define Event_log7_2_Page				Event_log0_1_Page+2*29		//事件记录7记录表2首页码(2)				
#define Event_log7_3_Page				Event_log0_1_Page+2*30		//事件记录7记录表3首页码(2)				
#define Event_log7_4_Page				Event_log0_1_Page+2*31		//事件记录7记录表4首页码(2)				
#define Event_log8_1_Page				Event_log0_1_Page+2*32		//事件记录8记录表1首页码(2)	//过载 (class_id=24) 		    
#define Event_log8_2_Page				Event_log0_1_Page+2*33		//事件记录8记录表2首页码(2)				
#define Event_log8_3_Page				Event_log0_1_Page+2*34		//事件记录8记录表3首页码(2)				
#define Event_log8_4_Page				Event_log0_1_Page+2*35		//事件记录8记录表4首页码(2)				
#define Event_log9_Page					Event_log0_1_Page+2*36		//事件记录9首页码(2)		//正向有功需量超限 (class_id=7) 		    
#define Event_logA_Page					Event_log0_1_Page+2*37		//事件记录A首页码(2)		//反向有功需量超限 (class_id=24) 		    
#define Event_logB_1_Page				Event_log0_1_Page+2*38		//事件记录B记录表1首页码(2)	//无功需量超限 (class_id=24) 		    
#define Event_logB_2_Page				Event_log0_1_Page+2*39		//事件记录B记录表1首页码(2)	//无功需量超限 (class_id=24) 		    
#define Event_logB_3_Page				Event_log0_1_Page+2*40		//事件记录B记录表1首页码(2)	//无功需量超限 (class_id=24) 		    
#define Event_logB_4_Page				Event_log0_1_Page+2*41		//事件记录B记录表1首页码(2)	//无功需量超限 (class_id=24) 		    
#define Event_logC_Page					Event_log0_1_Page+2*42		//事件记录C首页码(2)		//功率因数超限 (class_id=7) 		    
#define Event_logD_Page					Event_log0_1_Page+2*43		//事件记录D首页码(2)		//全失压 (class_id=7) 		    
#define Event_logE_Page					Event_log0_1_Page+2*44		//事件记录E首页码(2)		//辅助电源失压 (class_id=7) 		    
#define Event_logF_Page					Event_log0_1_Page+2*45		//事件记录F首页码(2)		//电压逆相序 (class_id=7) 		    
#define Event_log10_Page				Event_log0_1_Page+2*46		//事件记录10首页码(2)		//电流逆相序 (class_id=7) 		    
#define Event_log11_Page				Event_log0_1_Page+2*47		//事件记录11首页码(2)		//掉电 (class_id=7) 		    
#define Event_log12_Page				Event_log0_1_Page+2*48		//事件记录12首页码(2)		//电能表编程 (class_id=7) 		    
#define Event_log13_Page				Event_log0_1_Page+2*49		//事件记录13首页码(2)		//电表清零 (class_id=7) 		    
#define Event_log14_Page				Event_log0_1_Page+2*50		//事件记录14首页码(2)		//需量清零 (class_id=7) 		    
#define Event_log15_Page				Event_log0_1_Page+2*51		//事件记录15首页码(2)		//事件清零 (class_id=7) 		    
#define Event_log16_Page				Event_log0_1_Page+2*52		//事件记录16首页码(2)		//校时 (class_id=7) 		    
#define Event_log17_Page				Event_log0_1_Page+2*53		//事件记录17首页码(7)		//时段表编程 (class_id=7) 		    
#define Event_log18_Page				Event_log17_Page+7			//事件记录18首页码(2)		//时区表编程 (class_id=7) 		    
#define Event_log19_Page				Event_log18_Page+2*1		//事件记录19首页码(2)		//周休日编程 (class_id=7) 		    
#define Event_log1A_Page				Event_log18_Page+2*2		//事件记录1A首页码(2)		//结算日编程 (class_id=7) 		    
#define Event_log1B_Page				Event_log18_Page+2*3		//事件记录1B首页码(2)		//开盖 (class_id=7) 		    
#define Event_log1C_Page				Event_log18_Page+2*4		//事件记录1C首页码(2)		//开端钮盒 (class_id=7) 		    
#define Event_log1D_Page				Event_log18_Page+2*5		//事件记录1D首页码(2)		//电压不平衡 (class_id=7) 		    
#define Event_log1E_Page				Event_log18_Page+2*6		//事件记录1E首页码(2)		//电流不平衡 (class_id=7) 		    
#define Event_log1F_Page				Event_log18_Page+2*7		//事件记录1F首页码(2)		//跳闸 (class_id=7) 		    
#define Event_log20_Page				Event_log18_Page+2*8		//事件记录20首页码(2)		//合闸 (class_id=7) 		    
#define Event_log21_Page				Event_log18_Page+2*8		//事件记录21首页码(7)		//节假日编程 (class_id=7) 		    
#define Event_log22_Page				Event_log21_Page+7			//事件记录22首页码(2)		//有功组合方式编程 (class_id=7) 		    
#define Event_log23_Page				Event_log22_Page+2*1		//事件记录23首页码(2)		//无功组合方式编程 (class_id=7) 		    
#define Event_log24_Page				Event_log22_Page+2*2		//事件记录24首页码(2)		//费率参数编程 (class_id=7) 		    
#define Event_log25_Page				Event_log22_Page+2*3		//事件记录25首页码(2)		//阶梯表编程 (class_id=7) 		    
#define Event_log26_Page				Event_log22_Page+2*4		//事件记录26首页码(2)		//密钥更新 (class_id=7) 		    
#define Event_log27_Page				Event_log22_Page+2*5		//事件记录27首页码(2)		//异常插卡 (class_id=7) 		    
#define Event_log28_Page				Event_log22_Page+2*6		//事件记录28首页码(2)		//购电记录 (class_id=7) 		    
#define Event_log29_Page				Event_log22_Page+2*7		//事件记录29首页码(2)		//退费记录 (class_id=7) 		    
#define Event_log2A_Page				Event_log22_Page+2*8		//事件记录2A首页码(2)		//恒定磁场干扰 (class_id=7) 		    
#define Event_log2B_Page				Event_log22_Page+2*9		//事件记录2B首页码(2)		//负荷开关误动作 (class_id=7) 		    
#define Event_log2C_Page				Event_log22_Page+2*10		//事件记录2C首页码(2)		//电源异常 (class_id=7) 		    
#define Event_log2D_Page				Event_log22_Page+2*11		//事件记录2D首页码(2)		//电流严重不平衡 (class_id=7) 		    
#define Event_log2E_Page				Event_log22_Page+2*12		//事件记录2E首页码(2)		//时钟故障 (class_id=7) 		    
#define Event_log2F_Page				Event_log22_Page+2*13		//事件记录2F首页码(2)		//计量芯片故障 (class_id=7) 		    
#define Event_log30_Page				Event_log22_Page+2*14		//事件记录30首页码(2)		//通讯模块变更 (class_id=7) 		    
*/

//17.04.15
#define Event_log0_1_Page				AT7022DataZonePageBak+1		//事件记录0记录表1首页码(3)	//失压 (class_id=24) 					以下全部为10条
#define Event_log0_2_Page				Event_log0_1_Page+3*1		//事件记录0记录表2首页码(3)				
#define Event_log0_3_Page				Event_log0_1_Page+3*2		//事件记录0记录表3首页码(3)				
#define Event_log0_4_Page				Event_log0_1_Page+3*3		//事件记录0记录表4首页码(3)				
#define Event_log1_1_Page				Event_log0_1_Page+3*4		//事件记录1记录表1首页码(3)	//欠压 (class_id=24) 					
#define Event_log1_2_Page				Event_log0_1_Page+3*5		//事件记录1记录表2首页码(3)				
#define Event_log1_3_Page				Event_log0_1_Page+3*6		//事件记录1记录表3首页码(3)				
#define Event_log1_4_Page				Event_log0_1_Page+3*7		//事件记录1记录表4首页码(3)				
#define Event_log2_1_Page				Event_log0_1_Page+3*8		//事件记录2记录表1首页码(3)	//过压 (class_id=24) 		
#define Event_log2_2_Page				Event_log0_1_Page+3*9		//事件记录2记录表2首页码(3)				
#define Event_log2_3_Page				Event_log0_1_Page+3*10		//事件记录2记录表3首页码(3)				
#define Event_log2_4_Page				Event_log0_1_Page+3*11		//事件记录2记录表4首页码(3)				
#define Event_log3_1_Page				Event_log0_1_Page+3*12		//事件记录3记录表1首页码(3)	//断相 (class_id=24) 		
#define Event_log3_2_Page				Event_log0_1_Page+3*13		//事件记录3记录表2首页码(3)				
#define Event_log3_3_Page				Event_log0_1_Page+3*14		//事件记录3记录表3首页码(3)				
#define Event_log3_4_Page				Event_log0_1_Page+3*15		//事件记录3记录表4首页码(3)				
#define Event_log4_1_Page				Event_log0_1_Page+3*16		//事件记录4记录表1首页码(3)	//失流 (class_id=24) 		
#define Event_log4_2_Page				Event_log0_1_Page+3*17		//事件记录4记录表2首页码(3)				
#define Event_log4_3_Page				Event_log0_1_Page+3*18		//事件记录4记录表3首页码(3)				
#define Event_log4_4_Page				Event_log0_1_Page+3*19		//事件记录4记录表4首页码(3)				
#define Event_log5_1_Page				Event_log0_1_Page+3*20		//事件记录5记录表1首页码(3)	//过流 (class_id=24) 		
#define Event_log5_2_Page				Event_log0_1_Page+3*21		//事件记录5记录表2首页码(3)				
#define Event_log5_3_Page				Event_log0_1_Page+3*22		//事件记录5记录表3首页码(3)				
#define Event_log5_4_Page				Event_log0_1_Page+3*23		//事件记录5记录表4首页码(3)				
#define Event_log6_1_Page				Event_log0_1_Page+3*24		//事件记录6记录表1首页码(3)	//断流 (class_id=24) 		
#define Event_log6_2_Page				Event_log0_1_Page+3*25		//事件记录6记录表2首页码(3)				
#define Event_log6_3_Page				Event_log0_1_Page+3*26		//事件记录6记录表3首页码(3)				
#define Event_log6_4_Page				Event_log0_1_Page+3*27		//事件记录6记录表4首页码(3)				
#define Event_log7_1_Page				Event_log0_1_Page+3*28		//事件记录7记录表1首页码(3)	//潮流反向 (class_id=24) 		
#define Event_log7_2_Page				Event_log0_1_Page+3*29		//事件记录7记录表2首页码(3)				
#define Event_log7_3_Page				Event_log0_1_Page+3*30		//事件记录7记录表3首页码(3)				
#define Event_log7_4_Page				Event_log0_1_Page+3*31		//事件记录7记录表4首页码(3)				
#define Event_log8_1_Page				Event_log0_1_Page+3*32		//事件记录8记录表1首页码(3)	//过载 (class_id=24) 		    
#define Event_log8_2_Page				Event_log0_1_Page+3*33		//事件记录8记录表2首页码(3)				
#define Event_log8_3_Page				Event_log0_1_Page+3*34		//事件记录8记录表3首页码(3)				
#define Event_log8_4_Page				Event_log0_1_Page+3*35		//事件记录8记录表4首页码(3)				
#define Event_log9_Page					Event_log0_1_Page+3*36		//事件记录9首页码(2)		//正向有功需量超限 (class_id=7) 		    
#define Event_logA_Page					Event_log9_Page+2*1			//事件记录A首页码(2)		//反向有功需量超限 (class_id=24) 		    
#define Event_logB_1_Page				Event_log9_Page+2*2			//事件记录B记录表1首页码(2)	//无功需量超限 (class_id=24) 		    
#define Event_logB_2_Page				Event_log9_Page+2*3			//事件记录B记录表1首页码(2)	//无功需量超限 (class_id=24) 		    
#define Event_logB_3_Page				Event_log9_Page+2*4			//事件记录B记录表1首页码(2)	//无功需量超限 (class_id=24) 		    
#define Event_logB_4_Page				Event_log9_Page+2*5			//事件记录B记录表1首页码(2)	//无功需量超限 (class_id=24) 		    
#define Event_logC_Page					Event_log9_Page+2*6			//事件记录C首页码(2)		//功率因数超限 (class_id=7) 		    
#define Event_logD_Page					Event_log9_Page+2*7			//事件记录D首页码(2)		//全失压 (class_id=7) 		    
#define Event_logE_Page					Event_log9_Page+2*8			//事件记录E首页码(2)		//辅助电源失压 (class_id=7) 		    
#define Event_logF_Page					Event_log9_Page+2*9			//事件记录F首页码(3)		//电压逆相序 (class_id=7) 		    
#define Event_log10_Page				Event_logF_Page+3*1			//事件记录10首页码(3)		//电流逆相序 (class_id=7) 		    
#define Event_log11_Page				Event_logF_Page+3*2			//事件记录11首页码(2)		//掉电 (class_id=7) 		    
#define Event_log12_Page				Event_log11_Page+2*1		//事件记录12首页码(2)		//电能表编程 (class_id=7) 		    
#define Event_log13_Page				Event_log11_Page+2*2		//事件记录13首页码(3)		//电表清零 (class_id=7) 		    
#define Event_log14_Page				Event_log13_Page+3*1		//事件记录14首页码(3)		//需量清零 (class_id=7) 		    
#define Event_log15_Page				Event_log13_Page+3*2		//事件记录15首页码(2)		//事件清零 (class_id=7) 		    
#define Event_log16_Page				Event_log15_Page+2*1		//事件记录16首页码(2)		//校时 (class_id=7) 		    
#define Event_log17_Page				Event_log15_Page+2*2		//事件记录17首页码(2)		//时段表编程 (class_id=7) 		    
#define Event_log18_Page				Event_log15_Page+2*3		//事件记录18首页码(2)		//时区表编程 (class_id=7) 		    
#define Event_log19_Page				Event_log15_Page+2*4		//事件记录19首页码(2)		//周休日编程 (class_id=7) 		    
#define Event_log1A_Page				Event_log15_Page+2*5		//事件记录1A首页码(2)		//结算日编程 (class_id=7) 		    
#define Event_log1B_Page				Event_log15_Page+2*6		//事件记录1B首页码(2)		//开盖 (class_id=7) 		    
#define Event_log1C_Page				Event_log15_Page+2*7		//事件记录1C首页码(2)		//开端钮盒 (class_id=7) 		    
#define Event_log1D_Page				Event_log15_Page+2*8		//事件记录1D首页码(3)		//电压不平衡 (class_id=7) 		    
#define Event_log1E_Page				Event_log1D_Page+3*1		//事件记录1E首页码(3)		//电流不平衡 (class_id=7) 		    
#define Event_log1F_Page				Event_log1D_Page+3*2		//事件记录1F首页码(2)		//跳闸 (class_id=7) 		    
#define Event_log20_Page				Event_log1F_Page+2*1		//事件记录20首页码(2)		//合闸 (class_id=7) 		    
#define Event_log21_Page				Event_log1F_Page+2*2		//事件记录21首页码(2)		//节假日编程 (class_id=7) 		    
#define Event_log22_Page				Event_log1F_Page+2*3		//事件记录22首页码(2)		//有功组合方式编程 (class_id=7) 		    
#define Event_log23_Page				Event_log1F_Page+2*4		//事件记录23首页码(2)		//无功组合方式编程 (class_id=7) 		    
#define Event_log24_Page				Event_log1F_Page+2*5		//事件记录24首页码(2)		//费率参数编程 (class_id=7) 		    
#define Event_log25_Page				Event_log1F_Page+2*6		//事件记录25首页码(2)		//阶梯表编程 (class_id=7) 		    
#define Event_log26_Page				Event_log1F_Page+2*7		//事件记录26首页码(2)		//密钥更新 (class_id=7) 		    
#define Event_log27_Page				Event_log1F_Page+2*8		//事件记录27首页码(2)		//异常插卡 (class_id=7) 		    
#define Event_log28_Page				Event_log1F_Page+2*9		//事件记录28首页码(2)		//购电记录 (class_id=7) 		    
#define Event_log29_Page				Event_log1F_Page+2*10		//事件记录29首页码(2)		//退费记录 (class_id=7) 		    
#define Event_log2A_Page				Event_log1F_Page+2*11		//事件记录2A首页码(2)		//恒定磁场干扰 (class_id=7) 		    
#define Event_log2B_Page				Event_log1F_Page+2*12		//事件记录2B首页码(2)		//负荷开关误动作 (class_id=7) 		    
#define Event_log2C_Page				Event_log1F_Page+2*13		//事件记录2C首页码(2)		//电源异常 (class_id=7) 		    
#define Event_log2D_Page				Event_log1F_Page+2*14		//事件记录2D首页码(3)		//电流严重不平衡 (class_id=7) 		    
#define Event_log2E_Page				Event_log2D_Page+3*1		//事件记录2E首页码(2)		//时钟故障 (class_id=7) 		    
#define Event_log2F_Page				Event_log2E_Page+2*1		//事件记录2F首页码(2)		//计量芯片故障 (class_id=7) 		    
#define Event_log30_Page				Event_log2F_Page+2*2		//事件记录30首页码(2)		//通讯模块变更 (class_id=7) 		    









//#define L1PhAFailRecPage			L1Mon_MD_Pp0_PageBak+4			//上10次A相失压记录首页码(2)
//#define L1PhBFailRecPage			L1PhAFailRecPage+2*1			//上10次B相失压记录首页码(2)
//#define L1PhCFailRecPage			L1PhAFailRecPage+2*2			//上10次C相失压记录首页码(2)
//
//#define L1PhALowerRecPage			L1PhCFailRecPage+2				//上10次A相欠压记录首页码(2)
//#define L1PhBLowerRecPage			L1PhALowerRecPage+2*1			//上10次B相欠压记录首页码(2)
//#define L1PhCLowerRecPage			L1PhALowerRecPage+2*2			//上10次C相欠压记录首页码(2)
//
//#define L1PhAOverRecPage			L1PhCLowerRecPage+2				//上10次A相过压记录首页码(2)
//#define L1PhBOverRecPage			L1PhAOverRecPage+2*1			//上10次B相过压记录首页码(2)
//#define L1PhCOverRecPage			L1PhAOverRecPage+2*2			//上10次C相过压记录首页码(2)
//
//#define L1PhABreakRecPage			L1PhCOverRecPage+2				//上10次A相断相记录首页码(2)
//#define L1PhBBreakRecPage			L1PhABreakRecPage+2*1			//上10次B相断相记录首页码(2)
//#define L1PhCBreakRecPage			L1PhABreakRecPage+2*2			//上10次C相断相记录首页码(2)
//
//#define L1VoltRevPhaseRecPage		L1PhCBreakRecPage+2				//上10次电压逆相序记录首页码(2)
//#define L1CuRevPhaseRecPage			L1VoltRevPhaseRecPage+2			//上10次电流逆相序记录首页码(2)
//#define L1VoltImbalanceRecPage		L1CuRevPhaseRecPage+2			//上10次电压不平衡记录首页码(2)
//#define L1CuImbalanceRecPage		L1VoltImbalanceRecPage+2		//上10次电流不平衡记录首页码(2)
//#define L1CuBestImbalanceRecPage	L1CuImbalanceRecPage+2			//上10次电流严重不平衡记录首页码(2)
//
//#define L1CuAFailRecPage			L1CuBestImbalanceRecPage+2		//上10次A相失流记录首页码(2)
//#define L1CuBFailRecPage			L1CuAFailRecPage+2*1			//上10次B相失流记录首页码(2)
//#define L1CuCFailRecPage			L1CuAFailRecPage+2*2			//上10次C相失流记录首页码(2)
//
//#define L1CuAOverRecPage			L1CuCFailRecPage+2				//上10次A相过流记录首页码(2)
//#define L1CuBOverRecPage			L1CuAOverRecPage+2*1			//上10次B相过流记录首页码(2)
//#define L1CuCOverRecPage			L1CuAOverRecPage+2*2			//上10次C相过流记录首页码(2)
//
//#define L1CuABreakRecPage			L1CuCOverRecPage+2				//上10次A相断流记录首页码(2)
//#define L1CuBBreakRecPage			L1CuABreakRecPage+2*1			//上10次B相断流记录首页码(2)
//#define L1CuCBreakRecPage			L1CuABreakRecPage+2*2			//上10次C相断流记录首页码(2)
//
//#define L1RevPowerARecPage			L1CuCBreakRecPage+2				//上10次A相潮流反向记录首页码(2)
//#define L1RevPowerBRecPage			L1RevPowerARecPage+2*1			//上10次B相潮流反向记录首页码(2)
//#define L1RevPowerCRecPage			L1RevPowerARecPage+2*2			//上10次C相潮流反向记录首页码(2)
//
//#define L1PowerOverARecPage			L1RevPowerCRecPage+2			//上10次A相过载记录首页码(2)
//#define L1PowerOverBRecPage			L1PowerOverARecPage+2*1			//上10次B相过载记录首页码(2)
//#define L1PowerOverCRecPage			L1PowerOverARecPage+2*2			//上10次C相过载记录首页码(2)
//
//#define L1ECClrRecPage				L1PowerOverCRecPage+2			//上10次电量清零记录首页码(4)
//#define L1MDClrRecPage				L1ECClrRecPage+4				//上10次需量清零记录首页码(4)
//
//#define L1SegTabPrgRecPage			L1MDClrRecPage+4				//上10次时段表编程记录首页码(4)
//#define L1ZonePrgRecPage			L1SegTabPrgRecPage+4			//上10次时区表编程记录首页码(2)
//#define L1HolidayPrgRecPage			L1ZonePrgRecPage+2				//上10次节假日编程记录首页码(5)
//
////#define L1OpenCoverRecPage			L1HolidayPrgRecPage+5			//上10次开表盖记录首页码(2)
////#define L1OpenTerminalRecPage		L1OpenCoverRecPage+2			//上10次开端钮盖记录首页码(2)
//
//#define L1AppointFreezeRecPage		L1HolidayPrgRecPage+5			//上60次定时冻结记录首页码(31)
//#define L1InstantFreezeRecPage		L1AppointFreezeRecPage+31		//上3次瞬时冻结记录首页码(3)
//#define L1ZoneSWFreezeRecPage		L1InstantFreezeRecPage+3		//上2次时区切换冻结记录首页码(2)
//#define L1SegTabSWFreezeRecPage		L1ZoneSWFreezeRecPage+2			//上2次时段表切换冻结记录首页码(2)
//#define L1FeeSWFreezeRecPage		L1SegTabSWFreezeRecPage+2		//上2次费率切换冻结记录首页码(2)
//#define L1StepSWFreezeRecPage		L1FeeSWFreezeRecPage+2			//上2次阶梯切换冻结记录首页码(2) 				
//#define L1HourFreezeRecPage			L1StepSWFreezeRecPage+2			//上96次整点冻结记录首页码(2) 				
//#define L1DayFreezeRecPage			L1HourFreezeRecPage+2			//上62次日冻结记录首页码(32) 				
//
//#if ( NWProtocol ==	YesCheck )								//11.09.14			
//#define L1AllFailRecPage			L1DayFreezeRecPage+32			//上10次全失压记录首页码(2)
//#define L1PowerDownRecPage			L1AllFailRecPage+2				//上10次电流逆相序记录首页码(2)
//
//#define L1ECErrorRecPage			L1PowerDownRecPage+2			//上10次电量容错事件记录首页码(3)
//#define L1MDErrorRecPage			L1ECErrorRecPage+3				//上10次需量容错事件记录首页码(3)
//
//#else
//
//#define L1ECErrorRecPage			L1DayFreezeRecPage+32			//上10次电量容错事件记录首页码(3)
//#define L1MDErrorRecPage			L1ECErrorRecPage+3				//上10次需量容错事件记录首页码(3)
//
//#endif

/*
//FLASH中的历史电量、需量，电量清零前电量、需量清零前需量、
//定时冻结、瞬时冻结、时区和时段冻结的电量和需量数据同EEPROM中定义的电量和需量格式。各数据均带校验和。
//以下定义冻结数据中的（电压、电流、有无功视在功率、功率因素、相角、零线电流、电网频率），无校验
#define FreezeTime		0x0					//冻结时间(5)

#define FreezeUa		0x0					//A相电压(2)
#define FreezeUb		FreezeUa+2*1		//B相电压(2)
#define FreezeUc		FreezeUa+2*2		//C相电压(2)

#define FreezeIa		FreezeUc+2			//A相电流(3)
#define FreezeIb		FreezeIa+3*1		//B相电流(3)
#define FreezeIc		FreezeIa+3*2		//C相电流(3)
#define FreezeIt		FreezeIa+3*3		//N相电流(3)

#define FreezePt		FreezeIt+3			//总有功功率(3)
#define FreezePa		FreezePt+3*1		//A相有功功率(3)
#define FreezePb		FreezePt+3*2		//B相有功功率(3)
#define FreezePc		FreezePt+3*3		//C相有功功率(3)

#define FreezeQt		FreezePc+3			//总无功功率(3)
#define FreezeQa		FreezeQt+3*1		//A相无功功率(3)
#define FreezeQb		FreezeQt+3*2		//B相无功功率(3)
#define FreezeQc		FreezeQt+3*3		//C相无功功率(3)

#define FreezeSt		FreezeQc+3			//总视在功率(3)
#define FreezeSa		FreezeSt+3*1		//A相视在功率(3)
#define FreezeSb		FreezeSt+3*2		//B相视在功率(3)
#define FreezeSc		FreezeSt+3*3		//C相视在功率(3)

#define FreezePft		FreezeSc+3			//总功率因素(2)
#define FreezePfa		FreezePft+2*1		//A相功率因素(2)
#define FreezePfb		FreezePft+2*2		//B相功率因素(2)
#define FreezePfc		FreezePft+2*3		//C相功率因素(2)

#define FreezeAngleA	FreezePfc+2			//A相相角(2)
#define FreezeAngleB	FreezeAngleA+2*1	//B相相角(2)
#define FreezeAngleC	FreezeAngleA+2*2	//C相相角(2)
#define FreezeAngleZ	FreezeAngleA+2*3	//总相角(2)

#define FreezeFreq		FreezeAngleZ+2		//频率(2)
*/





