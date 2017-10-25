//铁电内存储的当前电量
#define	FRCMon_EC_P0 	0x00						//有功绝对值电量
#define	FRCMon_EC_P1	FRCMon_EC_P0 + (ECDIGIT+1)*1	  
#define	FRCMon_EC_P2	FRCMon_EC_P0 + (ECDIGIT+1)*2
#define	FRCMon_EC_P3	FRCMon_EC_P0 + (ECDIGIT+1)*3
#define	FRCMon_EC_P4	FRCMon_EC_P0 + (ECDIGIT+1)*4  
#define	FRCMon_EC_P5	FRCMon_EC_P0 + (ECDIGIT+1)*5	  
#define	FRCMon_EC_P6	FRCMon_EC_P0 + (ECDIGIT+1)*6
#define	FRCMon_EC_P7	FRCMon_EC_P0 + (ECDIGIT+1)*7
#define	FRCMon_EC_P8	FRCMon_EC_P0 + (ECDIGIT+1)*8  

#define	FRCMon_EC_Pp0 	FRCMon_EC_P8 + (ECDIGIT+1)*1						//当前总电量
#define	FRCMon_EC_Pp1	FRCMon_EC_Pp0 + (ECDIGIT+1)*1	  
#define	FRCMon_EC_Pp2	FRCMon_EC_Pp0 + (ECDIGIT+1)*2
#define	FRCMon_EC_Pp3	FRCMon_EC_Pp0 + (ECDIGIT+1)*3
#define	FRCMon_EC_Pp4	FRCMon_EC_Pp0 + (ECDIGIT+1)*4  
#define	FRCMon_EC_Pp5	FRCMon_EC_Pp0 + (ECDIGIT+1)*5	  
#define	FRCMon_EC_Pp6	FRCMon_EC_Pp0 + (ECDIGIT+1)*6
#define	FRCMon_EC_Pp7	FRCMon_EC_Pp0 + (ECDIGIT+1)*7
#define	FRCMon_EC_Pp8	FRCMon_EC_Pp0 + (ECDIGIT+1)*8  

#define	FRCMon_EC_Pn0 	FRCMon_EC_Pp8 + (ECDIGIT+1)
#define	FRCMon_EC_Pn1	FRCMon_EC_Pn0 + (ECDIGIT+1)*1
#define	FRCMon_EC_Pn2 	FRCMon_EC_Pn0 + (ECDIGIT+1)*2
#define	FRCMon_EC_Pn3	FRCMon_EC_Pn0 + (ECDIGIT+1)*3
#define	FRCMon_EC_Pn4 	FRCMon_EC_Pn0 + (ECDIGIT+1)*4
#define	FRCMon_EC_Pn5	FRCMon_EC_Pn0 + (ECDIGIT+1)*5
#define	FRCMon_EC_Pn6	FRCMon_EC_Pn0 + (ECDIGIT+1)*6
#define	FRCMon_EC_Pn7	FRCMon_EC_Pn0 + (ECDIGIT+1)*7
#define	FRCMon_EC_Pn8	FRCMon_EC_Pn0 + (ECDIGIT+1)*8

#define	FRCMon_EC_Qp0 	FRCMon_EC_Pn8 + (ECDIGIT+1)						//当前总电量
#define	FRCMon_EC_Qp1	FRCMon_EC_Qp0 + (ECDIGIT+1)*1	  
#define	FRCMon_EC_Qp2	FRCMon_EC_Qp0 + (ECDIGIT+1)*2
#define	FRCMon_EC_Qp3	FRCMon_EC_Qp0 + (ECDIGIT+1)*3
#define	FRCMon_EC_Qp4	FRCMon_EC_Qp0 + (ECDIGIT+1)*4  
#define	FRCMon_EC_Qp5	FRCMon_EC_Qp0 + (ECDIGIT+1)*5	  
#define	FRCMon_EC_Qp6	FRCMon_EC_Qp0 + (ECDIGIT+1)*6
#define	FRCMon_EC_Qp7	FRCMon_EC_Qp0 + (ECDIGIT+1)*7
#define	FRCMon_EC_Qp8	FRCMon_EC_Qp0 + (ECDIGIT+1)*8  

#define	FRCMon_EC_Qn0 	FRCMon_EC_Qp8 + (ECDIGIT+1)
#define	FRCMon_EC_Qn1	FRCMon_EC_Qn0 + (ECDIGIT+1)*1
#define	FRCMon_EC_Qn2 	FRCMon_EC_Qn0 + (ECDIGIT+1)*2
#define	FRCMon_EC_Qn3	FRCMon_EC_Qn0 + (ECDIGIT+1)*3
#define	FRCMon_EC_Qn4 	FRCMon_EC_Qn0 + (ECDIGIT+1)*4
#define	FRCMon_EC_Qn5	FRCMon_EC_Qn0 + (ECDIGIT+1)*5
#define	FRCMon_EC_Qn6 	FRCMon_EC_Qn0 + (ECDIGIT+1)*6
#define	FRCMon_EC_Qn7	FRCMon_EC_Qn0 + (ECDIGIT+1)*7
#define	FRCMon_EC_Qn8 	FRCMon_EC_Qn0 + (ECDIGIT+1)*8

#define	FRCMon_EC_Q10 	FRCMon_EC_Qn8 + (ECDIGIT+1)
#define	FRCMon_EC_Q11 	FRCMon_EC_Q10 + (ECDIGIT+1)*1
#define	FRCMon_EC_Q12 	FRCMon_EC_Q10 + (ECDIGIT+1)*2
#define	FRCMon_EC_Q13 	FRCMon_EC_Q10 + (ECDIGIT+1)*3
#define	FRCMon_EC_Q14 	FRCMon_EC_Q10 + (ECDIGIT+1)*4
#define	FRCMon_EC_Q15 	FRCMon_EC_Q10 + (ECDIGIT+1)*5
#define	FRCMon_EC_Q16 	FRCMon_EC_Q10 + (ECDIGIT+1)*6
#define	FRCMon_EC_Q17 	FRCMon_EC_Q10 + (ECDIGIT+1)*7
#define	FRCMon_EC_Q18 	FRCMon_EC_Q10 + (ECDIGIT+1)*8

#define	FRCMon_EC_Q20 	FRCMon_EC_Q18 + (ECDIGIT+1)
#define	FRCMon_EC_Q21 	FRCMon_EC_Q20 + (ECDIGIT+1)*1
#define	FRCMon_EC_Q22 	FRCMon_EC_Q20 + (ECDIGIT+1)*2
#define	FRCMon_EC_Q23 	FRCMon_EC_Q20 + (ECDIGIT+1)*3
#define	FRCMon_EC_Q24 	FRCMon_EC_Q20 + (ECDIGIT+1)*4
#define	FRCMon_EC_Q25 	FRCMon_EC_Q20 + (ECDIGIT+1)*5
#define	FRCMon_EC_Q26 	FRCMon_EC_Q20 + (ECDIGIT+1)*6
#define	FRCMon_EC_Q27 	FRCMon_EC_Q20 + (ECDIGIT+1)*7
#define	FRCMon_EC_Q28 	FRCMon_EC_Q20 + (ECDIGIT+1)*8

#define	FRCMon_EC_Q30 	FRCMon_EC_Q28 + (ECDIGIT+1)
#define	FRCMon_EC_Q31 	FRCMon_EC_Q30 + (ECDIGIT+1)*1
#define	FRCMon_EC_Q32 	FRCMon_EC_Q30 + (ECDIGIT+1)*2
#define	FRCMon_EC_Q33 	FRCMon_EC_Q30 + (ECDIGIT+1)*3
#define	FRCMon_EC_Q34 	FRCMon_EC_Q30 + (ECDIGIT+1)*4
#define	FRCMon_EC_Q35 	FRCMon_EC_Q30 + (ECDIGIT+1)*5
#define	FRCMon_EC_Q36 	FRCMon_EC_Q30 + (ECDIGIT+1)*6
#define	FRCMon_EC_Q37 	FRCMon_EC_Q30 + (ECDIGIT+1)*7
#define	FRCMon_EC_Q38 	FRCMon_EC_Q30 + (ECDIGIT+1)*8

#define	FRCMon_EC_Q40 	FRCMon_EC_Q38 + (ECDIGIT+1)
#define	FRCMon_EC_Q41 	FRCMon_EC_Q40 + (ECDIGIT+1)*1
#define	FRCMon_EC_Q42 	FRCMon_EC_Q40 + (ECDIGIT+1)*2
#define	FRCMon_EC_Q43	FRCMon_EC_Q40 + (ECDIGIT+1)*3
#define	FRCMon_EC_Q44 	FRCMon_EC_Q40 + (ECDIGIT+1)*4
#define	FRCMon_EC_Q45	FRCMon_EC_Q40 + (ECDIGIT+1)*5
#define	FRCMon_EC_Q46	FRCMon_EC_Q40 + (ECDIGIT+1)*6
#define	FRCMon_EC_Q47	FRCMon_EC_Q40 + (ECDIGIT+1)*7
#define	FRCMon_EC_Q48	FRCMon_EC_Q40 + (ECDIGIT+1)*8

#define	FRCMon_EC_Sp0 	FRCMon_EC_Q48 + (ECDIGIT+1)						//当前正向视在总电量
#define	FRCMon_EC_Sp1 	FRCMon_EC_Sp0 + (ECDIGIT+1)*1					
#define	FRCMon_EC_Sp2 	FRCMon_EC_Sp0 + (ECDIGIT+1)*2					
#define	FRCMon_EC_Sp3 	FRCMon_EC_Sp0 + (ECDIGIT+1)*3					
#define	FRCMon_EC_Sp4 	FRCMon_EC_Sp0 + (ECDIGIT+1)*4					
#define	FRCMon_EC_Sp5 	FRCMon_EC_Sp0 + (ECDIGIT+1)*5					
#define	FRCMon_EC_Sp6 	FRCMon_EC_Sp0 + (ECDIGIT+1)*6					
#define	FRCMon_EC_Sp7 	FRCMon_EC_Sp0 + (ECDIGIT+1)*7					
#define	FRCMon_EC_Sp8 	FRCMon_EC_Sp0 + (ECDIGIT+1)*8					

#define	FRCMon_EC_Sn0 	FRCMon_EC_Sp8 + (ECDIGIT+1)						//当前反向视在总电量
#define	FRCMon_EC_Sn1 	FRCMon_EC_Sn0 + (ECDIGIT+1)*1					
#define	FRCMon_EC_Sn2 	FRCMon_EC_Sn0 + (ECDIGIT+1)*2					
#define	FRCMon_EC_Sn3 	FRCMon_EC_Sn0 + (ECDIGIT+1)*3					
#define	FRCMon_EC_Sn4 	FRCMon_EC_Sn0 + (ECDIGIT+1)*4					
#define	FRCMon_EC_Sn5 	FRCMon_EC_Sn0 + (ECDIGIT+1)*5					
#define	FRCMon_EC_Sn6 	FRCMon_EC_Sn0 + (ECDIGIT+1)*6					
#define	FRCMon_EC_Sn7 	FRCMon_EC_Sn0 + (ECDIGIT+1)*7					
#define	FRCMon_EC_Sn8 	FRCMon_EC_Sn0 + (ECDIGIT+1)*8					

#define	FRCMon_EC_G0 	FRCMon_EC_Sn8 + (ECDIGIT+1)						//当前关联总电量
#define	FRCMon_EC_Bp0 	FRCMon_EC_G0 + (ECDIGIT+1)*1					//当前正向有功基波总电量
#define	FRCMon_EC_Bn0 	FRCMon_EC_G0 + (ECDIGIT+1)*2					//当前反向有功基波总电量
#define	FRCMon_EC_Hp0 	FRCMon_EC_G0 + (ECDIGIT+1)*3					//当前正向有功谐波总电量
#define	FRCMon_EC_Hn0 	FRCMon_EC_G0 + (ECDIGIT+1)*4					//当前反向有功谐波总电量
#define	FRCMon_EC_Cu0 	FRCMon_EC_G0 + (ECDIGIT+1)*5					//当前铜损有功总电能补偿量
#define	FRCMon_EC_Fe0 	FRCMon_EC_G0 + (ECDIGIT+1)*6					//当前铁损有功总电能补偿量
//282
#define	FRCMon_EC_PpA0 	FRCMon_EC_Fe0 + (ECDIGIT+1)						//当前A相有功正向总电量
#define	FRCMon_EC_PpA1 	FRCMon_EC_PpA0 + (ECDIGIT+1)*1					
#define	FRCMon_EC_PpA2 	FRCMon_EC_PpA0 + (ECDIGIT+1)*2					
#define	FRCMon_EC_PpA3 	FRCMon_EC_PpA0 + (ECDIGIT+1)*3					
#define	FRCMon_EC_PpA4 	FRCMon_EC_PpA0 + (ECDIGIT+1)*4					

#define	FRCMon_EC_PnA0 	FRCMon_EC_PpA4 + (ECDIGIT+1)					//当前A相有功反向总电量
#define	FRCMon_EC_PnA1 	FRCMon_EC_PnA0 + (ECDIGIT+1)*1					
#define	FRCMon_EC_PnA2 	FRCMon_EC_PnA0 + (ECDIGIT+1)*2					
#define	FRCMon_EC_PnA3 	FRCMon_EC_PnA0 + (ECDIGIT+1)*3					
#define	FRCMon_EC_PnA4 	FRCMon_EC_PnA0 + (ECDIGIT+1)*4					
//282+60=342
#define	FRCMon_EC_Q1A0 	FRCMon_EC_PnA4 + (ECDIGIT+1)
#define	FRCMon_EC_Q2A0 	FRCMon_EC_Q1A0 + (ECDIGIT+1)
#define	FRCMon_EC_Q3A0 	FRCMon_EC_Q2A0 + (ECDIGIT+1)
#define	FRCMon_EC_Q4A0 	FRCMon_EC_Q3A0 + (ECDIGIT+1)

#define	FRCMon_EC_SpA0 	FRCMon_EC_Q4A0 + (ECDIGIT+1)					//当前A相正向视在总电量
#define	FRCMon_EC_SnA0 	FRCMon_EC_SpA0 + (ECDIGIT+1)					//当前A相反向视在总电量
//282+60+36=378
#define	FRCMon_EC_GA0	FRCMon_EC_SnA0 + (ECDIGIT+1)					//当前A相关联总电量
#define	FRCMon_EC_BpA0 	FRCMon_EC_GA0 + (ECDIGIT+1)*1					//当前A相正向有功基波总电量
#define	FRCMon_EC_BnA0 	FRCMon_EC_GA0 + (ECDIGIT+1)*2					//当前A相反向有功基波总电量
#define	FRCMon_EC_HpA0 	FRCMon_EC_GA0 + (ECDIGIT+1)*3					//当前A相正向有功谐波总电量
#define	FRCMon_EC_HnA0 	FRCMon_EC_GA0 + (ECDIGIT+1)*4					//当前A相反向有功谐波总电量
#define	FRCMon_EC_CuA0 	FRCMon_EC_GA0 + (ECDIGIT+1)*5					//当前A相铜损有功总电能补偿量
#define	FRCMon_EC_FeA0 	FRCMon_EC_GA0 + (ECDIGIT+1)*6					//当前A相铁损有功总电能补偿量
//282+60+36+42=420
#define	FRCMon_EC_PpB0 	FRCMon_EC_FeA0 + (ECDIGIT+1)					//当前B相有功正向总电量
#define	FRCMon_EC_PpB1 	FRCMon_EC_PpB0 + (ECDIGIT+1)*1					
#define	FRCMon_EC_PpB2 	FRCMon_EC_PpB0 + (ECDIGIT+1)*2					
#define	FRCMon_EC_PpB3 	FRCMon_EC_PpB0 + (ECDIGIT+1)*3					
#define	FRCMon_EC_PpB4 	FRCMon_EC_PpB0 + (ECDIGIT+1)*4					

#define	FRCMon_EC_PnB0 	FRCMon_EC_PpB4 + (ECDIGIT+1)					//当前B相有功反向总电量
#define	FRCMon_EC_PnB1 	FRCMon_EC_PnB0 + (ECDIGIT+1)*1					
#define	FRCMon_EC_PnB2 	FRCMon_EC_PnB0 + (ECDIGIT+1)*2					
#define	FRCMon_EC_PnB3 	FRCMon_EC_PnB0 + (ECDIGIT+1)*3					
#define	FRCMon_EC_PnB4 	FRCMon_EC_PnB0 + (ECDIGIT+1)*4					

#define	FRCMon_EC_Q1B0 	FRCMon_EC_PnB4 + (ECDIGIT+1)
#define	FRCMon_EC_Q2B0 	FRCMon_EC_Q1B0 + (ECDIGIT+1)
#define	FRCMon_EC_Q3B0 	FRCMon_EC_Q2B0 + (ECDIGIT+1)
#define	FRCMon_EC_Q4B0 	FRCMon_EC_Q3B0 + (ECDIGIT+1)

#define	FRCMon_EC_SpB0 	FRCMon_EC_Q4B0 + (ECDIGIT+1)					//当前B相正向视在总电量
#define	FRCMon_EC_SnB0 	FRCMon_EC_SpB0 + (ECDIGIT+1)					//当前B相反向视在总电量

#define	FRCMon_EC_GB0	FRCMon_EC_SnB0 + (ECDIGIT+1)					//当前B相关联总电量
#define	FRCMon_EC_BpB0 	FRCMon_EC_GB0 + (ECDIGIT+1)*1					//当前B相正向有功基波总电量
#define	FRCMon_EC_BnB0 	FRCMon_EC_GB0 + (ECDIGIT+1)*2					//当前B相反向有功基波总电量
#define	FRCMon_EC_HpB0 	FRCMon_EC_GB0 + (ECDIGIT+1)*3					//当前B相正向有功谐波总电量
#define	FRCMon_EC_HnB0 	FRCMon_EC_GB0 + (ECDIGIT+1)*4					//当前B相反向有功谐波总电量
#define	FRCMon_EC_CuB0 	FRCMon_EC_GB0 + (ECDIGIT+1)*5					//当前B相铜损有功总电能补偿量
#define	FRCMon_EC_FeB0 	FRCMon_EC_GB0 + (ECDIGIT+1)*6					//当前B相铁损有功总电能补偿量
//420+138=558 
#define	FRCMon_EC_PpC0 	FRCMon_EC_FeB0 + (ECDIGIT+1)					//当前C相有功正向总电量
#define	FRCMon_EC_PpC1 	FRCMon_EC_PpC0 + (ECDIGIT+1)*1					
#define	FRCMon_EC_PpC2 	FRCMon_EC_PpC0 + (ECDIGIT+1)*2					
#define	FRCMon_EC_PpC3 	FRCMon_EC_PpC0 + (ECDIGIT+1)*3					
#define	FRCMon_EC_PpC4 	FRCMon_EC_PpC0 + (ECDIGIT+1)*4					

#define	FRCMon_EC_PnC0 	FRCMon_EC_PpC4 + (ECDIGIT+1)					//当前C相有功反向总电量
#define	FRCMon_EC_PnC1 	FRCMon_EC_PnC0 + (ECDIGIT+1)*1					
#define	FRCMon_EC_PnC2 	FRCMon_EC_PnC0 + (ECDIGIT+1)*2					
#define	FRCMon_EC_PnC3 	FRCMon_EC_PnC0 + (ECDIGIT+1)*3					
#define	FRCMon_EC_PnC4 	FRCMon_EC_PnC0 + (ECDIGIT+1)*4					

#define	FRCMon_EC_Q1C0 	FRCMon_EC_PnC4 + (ECDIGIT+1)
#define	FRCMon_EC_Q2C0 	FRCMon_EC_Q1C0 + (ECDIGIT+1)
#define	FRCMon_EC_Q3C0 	FRCMon_EC_Q2C0 + (ECDIGIT+1)
#define	FRCMon_EC_Q4C0 	FRCMon_EC_Q3C0 + (ECDIGIT+1)

#define	FRCMon_EC_SpC0 	FRCMon_EC_Q4C0 + (ECDIGIT+1)					//当前C相正向视在总电量
#define	FRCMon_EC_SnC0 	FRCMon_EC_SpC0 + (ECDIGIT+1)					//当前C相反向视在总电量

#define	FRCMon_EC_GC0	FRCMon_EC_SnC0 + (ECDIGIT+1)					//当前C相关联总电量
#define	FRCMon_EC_BpC0 	FRCMon_EC_GC0 + (ECDIGIT+1)*1					//当前C相正向有功基波总电量
#define	FRCMon_EC_BnC0 	FRCMon_EC_GC0 + (ECDIGIT+1)*2					//当前C相反向有功基波总电量
#define	FRCMon_EC_HpC0 	FRCMon_EC_GC0 + (ECDIGIT+1)*3					//当前C相正向有功谐波总电量
#define	FRCMon_EC_HnC0 	FRCMon_EC_GC0 + (ECDIGIT+1)*4					//当前C相反向有功谐波总电量
#define	FRCMon_EC_CuC0 	FRCMon_EC_GC0 + (ECDIGIT+1)*5					//当前C相铜损有功总电能补偿量
#define	FRCMon_EC_FeC0 	FRCMon_EC_GC0 + (ECDIGIT+1)*6					//当前C相铁损有功总电能补偿量
//420+138+138=696
#define FPL_CumPp		FRCMon_EC_FeC0+(ECDIGIT+1)						//所有电量脉冲(121)
//696+121=817
//#define FLoadCurRecNo1	FPL_CumPp+(ECUnitNum*2+1)						//第1类负荷当前记录号(5)
//#define FLoadCurRecNo2	FLoadCurRecNo1+5*1								//第2类负荷当前记录号(5)
//#define FLoadCurRecNo3	FLoadCurRecNo1+5*2								//第3类负荷当前记录号(5)
//#define FLoadCurRecNo4	FLoadCurRecNo1+5*3								//第4类负荷当前记录号(5)
//#define FLoadCurRecNo5	FLoadCurRecNo1+5*4								//第5类负荷当前记录号(5)
//#define FLoadCurRecNo6	FLoadCurRecNo1+5*5								//第6类负荷当前记录号(5)
//817
#define FPdownTime		FPL_CumPp+(ECUnitNum*2+1)						//下电时间(7)
#define FConsumECSumBCD	FPdownTime+7									//16进制累计用电金额(9)	HEX		XXXXXXXXXX.XXXXXX
//817+7+9=833
//#define FMonVoltCheckRec	FConsumECSumBCD+9							//本月电压合格率数据(17)
#define FMonVoltACheckRec	FConsumECSumBCD+9							//本月A相电压合格率数据(17)
#define FMonVoltBCheckRec	FMonVoltACheckRec+17*2						//本月B相电压合格率数据(17)
#define FMonVoltCCheckRec	FMonVoltACheckRec+17*3						//本月C相电压合格率数据(17)

//#define FDayVoltCheckRec	FMonVoltCCheckRec+17						//当日电压合格率数据(17)
#define FDayVoltACheckRec	FMonVoltCCheckRec+17*1						//当日A相电压合格率数据(17)
#define FDayVoltBCheckRec	FDayVoltACheckRec+17*2						//当日B相电压合格率数据(17)
#define FDayVoltCCheckRec	FDayVoltACheckRec+17*3						//当日C相电压合格率数据(17)
//833+102=935
#define FRAMCheck		FDayVoltCCheckRec+17							//FRAM故障检测字节(2)	正常：0x55 异常：非0x55 
//935+2=937
#define FMonVoltNormalTime FRAMCheck+2									//本月电压合格时间(5)	//新国网
#define FDayVoltNormalTime FMonVoltNormalTime+5							//当日电压合格时间(5)	//新国网
//947
#define FPWDownFlag 	FDayVoltNormalTime+5							//掉电标志(2)	0x55:掉过电；其他：未掉过电		//新国网	//13.10.26
//949
#define FEventUpState	FPWDownFlag+2									//主动上报状态字(13)		//新国网	//14.04.25
//963
#define FPulseValidFlag FEventUpState+13								//掉电脉冲有效标记(2)		有效：55H; 无效：其他：		//16.01.26
#define	FEventUpMode	FPulseValidFlag+2								//主动上报模式字(9)	//16.05.17







