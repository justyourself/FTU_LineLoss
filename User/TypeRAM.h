#include "TypeMeter.h"

// *******************************************************************
// Data type define
// *******************************************************************
typedef unsigned char BCD1[1];
typedef unsigned char BCD2[2];
typedef unsigned char BCD3[3];
typedef unsigned char BCD4[4];
typedef unsigned char BCD5[5];
typedef unsigned char BCD6[6];
typedef unsigned char BCD7[7];
typedef unsigned char BCD8[8];
typedef unsigned char BCD9[9];

/*状态字定义------------------------*/
typedef union{
  unsigned short word;
  struct{
    unsigned char b0  :1;
    unsigned char b1  :1;
    unsigned char b2  :1;
    unsigned char b3  :1;
    unsigned char b4  :1;
    unsigned char b5  :1;
    unsigned char b6  :1;
    unsigned char b7  :1;
    unsigned char b8  :1;
    unsigned char b9  :1;
    unsigned char b10 :1;
    unsigned char b11 :1;
    unsigned char b12 :1;
    unsigned char b13 :1;
    unsigned char b14 :1;
    unsigned char b15 :1;
  }bit;
}WORD_STRUCT;

typedef union{
  unsigned char byte;
  struct{
    unsigned char b0  :1;
    unsigned char b1  :1;
    unsigned char b2  :1;
    unsigned char b3  :1;
    unsigned char b4  :1;
    unsigned char b5  :1;
    unsigned char b6  :1;
    unsigned char b7  :1;
  }bit;
}BYTE_STRUCT;
/*
typedef struct
{
	unsigned char* SDA_DIR;
	unsigned char* SCL_DIR;
	unsigned char* SDA_IN;
	unsigned char SDA;
	unsigned char SCL;
}I2CPARA;							//I2C接口芯片在CPU上的IO口配置参数
*/

typedef struct
{
	unsigned long* SDA_DIR;
	unsigned long* SCL_DIR;
	unsigned long* SDA_IN;
	unsigned long SDA;
	unsigned long SCL;
}I2CPARA;							//I2C接口芯片在CPU上的IO口配置参数

typedef struct
{
	unsigned char RCMon_EC_P0[ECDIGIT+1];	//当前正向有功总电量(5+1)
	unsigned char RCMon_EC_P1[ECDIGIT+1];	//当前正向有功尖电量(5+1)
	unsigned char RCMon_EC_P2[ECDIGIT+1];	//当前正向有功峰电量(5+1)
	unsigned char RCMon_EC_P3[ECDIGIT+1];	//当前正向有功平电量(5+1)
	unsigned char RCMon_EC_P4[ECDIGIT+1];	//当前正向有功谷电量(5+1)
	unsigned char RCMon_EC_P5[ECDIGIT+1];	//当前正向有功尖电量(5+1)
	unsigned char RCMon_EC_P6[ECDIGIT+1];	//当前正向有功峰电量(5+1)
	unsigned char RCMon_EC_P7[ECDIGIT+1];	//当前正向有功平电量(5+1)
	unsigned char RCMon_EC_P8[ECDIGIT+1];	//当前正向有功谷电量(5+1)

	unsigned char RCMon_EC_Pp0[ECDIGIT+1];	//当前正向有功总电量(5+1)
	unsigned char RCMon_EC_Pp1[ECDIGIT+1];	//当前正向有功尖电量(5+1)
	unsigned char RCMon_EC_Pp2[ECDIGIT+1];	//当前正向有功峰电量(5+1)
	unsigned char RCMon_EC_Pp3[ECDIGIT+1];	//当前正向有功平电量(5+1)
	unsigned char RCMon_EC_Pp4[ECDIGIT+1];	//当前正向有功谷电量(5+1)
	unsigned char RCMon_EC_Pp5[ECDIGIT+1];	//当前正向有功尖电量(5+1)
	unsigned char RCMon_EC_Pp6[ECDIGIT+1];	//当前正向有功峰电量(5+1)
	unsigned char RCMon_EC_Pp7[ECDIGIT+1];	//当前正向有功平电量(5+1)
	unsigned char RCMon_EC_Pp8[ECDIGIT+1];	//当前正向有功谷电量(5+1)

	unsigned char RCMon_EC_Pn0[ECDIGIT+1];	//当前反向有功总电量(5+1)
	unsigned char RCMon_EC_Pn1[ECDIGIT+1];	//当前反向有功尖电量(5+1)
	unsigned char RCMon_EC_Pn2[ECDIGIT+1];	//当前反向有功峰电量(5+1)
	unsigned char RCMon_EC_Pn3[ECDIGIT+1];	//当前反向有功平电量(5+1)
	unsigned char RCMon_EC_Pn4[ECDIGIT+1];	//当前反向有功谷电量(5+1)
	unsigned char RCMon_EC_Pn5[ECDIGIT+1];	//当前反向有功尖电量(5+1)
	unsigned char RCMon_EC_Pn6[ECDIGIT+1];	//当前反向有功峰电量(5+1)
	unsigned char RCMon_EC_Pn7[ECDIGIT+1];	//当前反向有功平电量(5+1)
	unsigned char RCMon_EC_Pn8[ECDIGIT+1];	//当前反向有功谷电量(5+1)

	unsigned char RCMon_EC_Qp0[ECDIGIT+1];	//当前正向无功总电量(5+1)
	unsigned char RCMon_EC_Qp1[ECDIGIT+1];	//当前正向无功尖电量(5+1)
	unsigned char RCMon_EC_Qp2[ECDIGIT+1];	//当前正向无功峰电量(5+1)
	unsigned char RCMon_EC_Qp3[ECDIGIT+1];	//当前正向无功平电量(5+1)
	unsigned char RCMon_EC_Qp4[ECDIGIT+1];	//当前正向无功谷电量(5+1)
	unsigned char RCMon_EC_Qp5[ECDIGIT+1];	//当前正向无功尖电量(5+1)
	unsigned char RCMon_EC_Qp6[ECDIGIT+1];	//当前正向无功峰电量(5+1)
	unsigned char RCMon_EC_Qp7[ECDIGIT+1];	//当前正向无功平电量(5+1)
	unsigned char RCMon_EC_Qp8[ECDIGIT+1];	//当前正向无功谷电量(5+1)

	unsigned char RCMon_EC_Qn0[ECDIGIT+1];	//当前反向无功总电量(5+1)
	unsigned char RCMon_EC_Qn1[ECDIGIT+1];	//当前反向无功尖电量(5+1)
	unsigned char RCMon_EC_Qn2[ECDIGIT+1];	//当前反向无功峰电量(5+1)
	unsigned char RCMon_EC_Qn3[ECDIGIT+1];	//当前反向无功平电量(5+1)
	unsigned char RCMon_EC_Qn4[ECDIGIT+1];	//当前反向无功谷电量(5+1)
	unsigned char RCMon_EC_Qn5[ECDIGIT+1];	//当前反向无功尖电量(5+1)
	unsigned char RCMon_EC_Qn6[ECDIGIT+1];	//当前反向无功峰电量(5+1)
	unsigned char RCMon_EC_Qn7[ECDIGIT+1];	//当前反向无功平电量(5+1)
	unsigned char RCMon_EC_Qn8[ECDIGIT+1];	//当前反向无功谷电量(5+1)

#if(MeterPhase==Threephase)
	unsigned char RCMon_EC_Q10[ECDIGIT+1];	//当前1象限无功总电量(5+1)
	unsigned char RCMon_EC_Q11[ECDIGIT+1];	//当前1象限无功尖电量(5+1)
	unsigned char RCMon_EC_Q12[ECDIGIT+1];	//当前1象限无功峰电量(5+1)
	unsigned char RCMon_EC_Q13[ECDIGIT+1];	//当前1象限无功平电量(5+1)
	unsigned char RCMon_EC_Q14[ECDIGIT+1];	//当前1象限无功谷电量(5+1)
	unsigned char RCMon_EC_Q15[ECDIGIT+1];	//当前1象限无功尖电量(5+1)
	unsigned char RCMon_EC_Q16[ECDIGIT+1];	//当前1象限无功峰电量(5+1)
	unsigned char RCMon_EC_Q17[ECDIGIT+1];	//当前1象限无功平电量(5+1)
	unsigned char RCMon_EC_Q18[ECDIGIT+1];	//当前1象限无功谷电量(5+1)

	unsigned char RCMon_EC_Q20[ECDIGIT+1];	//当前2象限无功总电量(5+1)
	unsigned char RCMon_EC_Q21[ECDIGIT+1];	//当前2象限无功尖电量(5+1)
	unsigned char RCMon_EC_Q22[ECDIGIT+1];	//当前2象限无功峰电量(5+1)
	unsigned char RCMon_EC_Q23[ECDIGIT+1];	//当前2象限无功平电量(5+1)
	unsigned char RCMon_EC_Q24[ECDIGIT+1];	//当前2象限无功谷电量(5+1)
	unsigned char RCMon_EC_Q25[ECDIGIT+1];	//当前2象限无功尖电量(5+1)
	unsigned char RCMon_EC_Q26[ECDIGIT+1];	//当前2象限无功峰电量(5+1)
	unsigned char RCMon_EC_Q27[ECDIGIT+1];	//当前2象限无功平电量(5+1)
	unsigned char RCMon_EC_Q28[ECDIGIT+1];	//当前2象限无功谷电量(5+1)

	unsigned char RCMon_EC_Q30[ECDIGIT+1];	//当前3象限无功总电量(5+1)
	unsigned char RCMon_EC_Q31[ECDIGIT+1];	//当前3象限无功尖电量(5+1)
	unsigned char RCMon_EC_Q32[ECDIGIT+1];	//当前3象限无功峰电量(5+1)
	unsigned char RCMon_EC_Q33[ECDIGIT+1];	//当前3象限无功平电量(5+1)
	unsigned char RCMon_EC_Q34[ECDIGIT+1];	//当前3象限无功谷电量(5+1)
	unsigned char RCMon_EC_Q35[ECDIGIT+1];	//当前3象限无功尖电量(5+1)
	unsigned char RCMon_EC_Q36[ECDIGIT+1];	//当前3象限无功峰电量(5+1)
	unsigned char RCMon_EC_Q37[ECDIGIT+1];	//当前3象限无功平电量(5+1)
	unsigned char RCMon_EC_Q38[ECDIGIT+1];	//当前3象限无功谷电量(5+1)

	unsigned char RCMon_EC_Q40[ECDIGIT+1];	//当前4象限无功总电量(5+1)
	unsigned char RCMon_EC_Q41[ECDIGIT+1];	//当前4象限无功尖电量(5+1)
	unsigned char RCMon_EC_Q42[ECDIGIT+1];	//当前4象限无功峰电量(5+1)
	unsigned char RCMon_EC_Q43[ECDIGIT+1];	//当前4象限无功平电量(5+1)
	unsigned char RCMon_EC_Q44[ECDIGIT+1];	//当前4象限无功谷电量(5+1)
	unsigned char RCMon_EC_Q45[ECDIGIT+1];	//当前4象限无功尖电量(5+1)
	unsigned char RCMon_EC_Q46[ECDIGIT+1];	//当前4象限无功峰电量(5+1)
	unsigned char RCMon_EC_Q47[ECDIGIT+1];	//当前4象限无功平电量(5+1)
	unsigned char RCMon_EC_Q48[ECDIGIT+1];	//当前4象限无功谷电量(5+1)

	unsigned char RCMon_EC_Sp0[ECDIGIT+1];	//当前正向视在总电量(5+1)
	unsigned char RCMon_EC_Sp1[ECDIGIT+1];	//当前正向视在尖电量(5+1)
	unsigned char RCMon_EC_Sp2[ECDIGIT+1];	//当前正向视在峰电量(5+1)
	unsigned char RCMon_EC_Sp3[ECDIGIT+1];	//当前正向视在平电量(5+1)
	unsigned char RCMon_EC_Sp4[ECDIGIT+1];	//当前正向视在谷电量(5+1)
	unsigned char RCMon_EC_Sp5[ECDIGIT+1];	//当前正向视在尖电量(5+1)
	unsigned char RCMon_EC_Sp6[ECDIGIT+1];	//当前正向视在峰电量(5+1)
	unsigned char RCMon_EC_Sp7[ECDIGIT+1];	//当前正向视在平电量(5+1)
	unsigned char RCMon_EC_Sp8[ECDIGIT+1];	//当前正向视在谷电量(5+1)

	unsigned char RCMon_EC_Sn0[ECDIGIT+1];	//当前反向视在总电量(5+1)
	unsigned char RCMon_EC_Sn1[ECDIGIT+1];	//当前反向视在尖电量(5+1)
	unsigned char RCMon_EC_Sn2[ECDIGIT+1];	//当前反向视在峰电量(5+1)
	unsigned char RCMon_EC_Sn3[ECDIGIT+1];	//当前反向视在平电量(5+1)
	unsigned char RCMon_EC_Sn4[ECDIGIT+1];	//当前反向视在谷电量(5+1)	
	unsigned char RCMon_EC_Sn5[ECDIGIT+1];	//当前反向视在尖电量(5+1)
	unsigned char RCMon_EC_Sn6[ECDIGIT+1];	//当前反向视在峰电量(5+1)
	unsigned char RCMon_EC_Sn7[ECDIGIT+1];	//当前反向视在平电量(5+1)
	unsigned char RCMon_EC_Sn8[ECDIGIT+1];	//当前反向视在谷电量(5+1)
	#endif
/*
	unsigned char RCMon_EC_G0[ECDIGIT+1];	//当前关联总电量(5+1)
	unsigned char RCMon_EC_Bp0[ECDIGIT+1];	//当前正向有功基波总电量(5+1)				
	unsigned char RCMon_EC_Bn0[ECDIGIT+1];	//当前反向有功基波总电量(5+1)				
	unsigned char RCMon_EC_Hp0[ECDIGIT+1];	//当前正向有功谐波总电量(5+1)				
	unsigned char RCMon_EC_Hn0[ECDIGIT+1];	//当前反向有功谐波总电量(5+1)				
	unsigned char RCMon_EC_Cu0[ECDIGIT+1];	//当前铜损有功总电能补偿量(5+1)								
	unsigned char RCMon_EC_Fe0[ECDIGIT+1];	//当前铁损有功总电能补偿量(5+1)				
	
	unsigned char RCMon_EC_PpA0[ECDIGIT+1];	//当前A相正向有功总电量(5+1)
	unsigned char RCMon_EC_PpA1[ECDIGIT+1];	//当前A相正向有功尖电量(5+1)
	unsigned char RCMon_EC_PpA2[ECDIGIT+1];	//当前A相正向有功峰电量(5+1)
	unsigned char RCMon_EC_PpA3[ECDIGIT+1];	//当前A相正向有功平电量(5+1)
	unsigned char RCMon_EC_PpA4[ECDIGIT+1];	//当前A相正向有功谷电量(5+1)

	unsigned char RCMon_EC_PnA0[ECDIGIT+1];	//当前A相反向有功总电量(5+1)
	unsigned char RCMon_EC_PnA1[ECDIGIT+1];	//当前A相反向有功尖电量(5+1)
	unsigned char RCMon_EC_PnA2[ECDIGIT+1];	//当前A相反向有功峰电量(5+1)
	unsigned char RCMon_EC_PnA3[ECDIGIT+1];	//当前A相反向有功平电量(5+1)
	unsigned char RCMon_EC_PnA4[ECDIGIT+1];	//当前A相反向有功谷电量(5+1)

	unsigned char RCMon_EC_Q1A0[ECDIGIT+1];	//当前A相1象限无功总电量(5+1)
	unsigned char RCMon_EC_Q2A0[ECDIGIT+1];	//当前A相2象限无功总电量(5+1)
	unsigned char RCMon_EC_Q3A0[ECDIGIT+1];	//当前A相3象限无功总电量(5+1)
	unsigned char RCMon_EC_Q4A0[ECDIGIT+1];	//当前A相4象限无功总电量(5+1)

	unsigned char RCMon_EC_SpA0[ECDIGIT+1];	//当前A相正向视在总电量(5+1)
	unsigned char RCMon_EC_SnA0[ECDIGIT+1];	//当前A相反向视在总电量(5+1)

	unsigned char RCMon_EC_GA0[ECDIGIT+1];	//当前A相关联总电量(5+1)
	unsigned char RCMon_EC_BpA0[ECDIGIT+1];	//当前A相正向有功基波总电量(5+1)				
	unsigned char RCMon_EC_BnA0[ECDIGIT+1];	//当前A相反向有功基波总电量(5+1)				
	unsigned char RCMon_EC_HpA0[ECDIGIT+1];	//当前A相正向有功谐波总电量(5+1)				
	unsigned char RCMon_EC_HnA0[ECDIGIT+1];	//当前A相反向有功谐波总电量(5+1)				
	unsigned char RCMon_EC_CuA0[ECDIGIT+1];	//当前A相铜损有功总电能补偿量(5+1)								
	unsigned char RCMon_EC_FeA0[ECDIGIT+1];	//当前A相铁损有功总电能补偿量(5+1)				

	unsigned char RCMon_EC_PpB0[ECDIGIT+1];	//当前B相正向有功总电量(5+1)
	unsigned char RCMon_EC_PpB1[ECDIGIT+1];	//当前B相正向有功尖电量(5+1)
	unsigned char RCMon_EC_PpB2[ECDIGIT+1];	//当前B相正向有功峰电量(5+1)
	unsigned char RCMon_EC_PpB3[ECDIGIT+1];	//当前B相正向有功平电量(5+1)
	unsigned char RCMon_EC_PpB4[ECDIGIT+1];	//当前B相正向有功谷电量(5+1)

	unsigned char RCMon_EC_PnB0[ECDIGIT+1];	//当前B相反向有功总电量(5+1)
	unsigned char RCMon_EC_PnB1[ECDIGIT+1];	//当前B相反向有功尖电量(5+1)
	unsigned char RCMon_EC_PnB2[ECDIGIT+1];	//当前B相反向有功峰电量(5+1)
	unsigned char RCMon_EC_PnB3[ECDIGIT+1];	//当前B相反向有功平电量(5+1)
	unsigned char RCMon_EC_PnB4[ECDIGIT+1];	//当前B相反向有功谷电量(5+1)

	unsigned char RCMon_EC_Q1B0[ECDIGIT+1];	//当前B相1象限无功总电量(5+1)
	unsigned char RCMon_EC_Q2B0[ECDIGIT+1];	//当前B相2象限无功总电量(5+1)
	unsigned char RCMon_EC_Q3B0[ECDIGIT+1];	//当前B相3象限无功总电量(5+1)
	unsigned char RCMon_EC_Q4B0[ECDIGIT+1];	//当前B相4象限无功总电量(5+1)

	unsigned char RCMon_EC_SpB0[ECDIGIT+1];	//当前B相正向视在总电量(5+1)
	unsigned char RCMon_EC_SnB0[ECDIGIT+1];	//当前B相反向视在总电量(5+1)

	unsigned char RCMon_EC_GB0[ECDIGIT+1];	//当前B相关联总电量(5+1)
	unsigned char RCMon_EC_BpB0[ECDIGIT+1];	//当前B相正向有功基波总电量(5+1)				
	unsigned char RCMon_EC_BnB0[ECDIGIT+1];	//当前B相反向有功基波总电量(5+1)				
	unsigned char RCMon_EC_HpB0[ECDIGIT+1];	//当前B相正向有功谐波总电量(5+1)				
	unsigned char RCMon_EC_HnB0[ECDIGIT+1];	//当前B相反向有功谐波总电量(5+1)				
	unsigned char RCMon_EC_CuB0[ECDIGIT+1];	//当前B相铜损有功总电能补偿量(5+1)								
	unsigned char RCMon_EC_FeB0[ECDIGIT+1];	//当前B相铁损有功总电能补偿量(5+1)				

	unsigned char RCMon_EC_PpC0[ECDIGIT+1];	//当前C相正向有功总电量(5+1)
	unsigned char RCMon_EC_PpC1[ECDIGIT+1];	//当前C相正向有功尖电量(5+1)
	unsigned char RCMon_EC_PpC2[ECDIGIT+1];	//当前C相正向有功峰电量(5+1)
	unsigned char RCMon_EC_PpC3[ECDIGIT+1];	//当前C相正向有功平电量(5+1)
	unsigned char RCMon_EC_PpC4[ECDIGIT+1];	//当前C相正向有功谷电量(5+1)

	unsigned char RCMon_EC_PnC0[ECDIGIT+1];	//当前C相反向有功总电量(5+1)
	unsigned char RCMon_EC_PnC1[ECDIGIT+1];	//当前C相反向有功尖电量(5+1)
	unsigned char RCMon_EC_PnC2[ECDIGIT+1];	//当前C相反向有功峰电量(5+1)
	unsigned char RCMon_EC_PnC3[ECDIGIT+1];	//当前C相反向有功平电量(5+1)
	unsigned char RCMon_EC_PnC4[ECDIGIT+1];	//当前C相反向有功谷电量(5+1)

	unsigned char RCMon_EC_Q1C0[ECDIGIT+1];	//当前C相1象限无功总电量(5+1)
	unsigned char RCMon_EC_Q2C0[ECDIGIT+1];	//当前C相2象限无功总电量(5+1)
	unsigned char RCMon_EC_Q3C0[ECDIGIT+1];	//当前C相3象限无功总电量(5+1)
	unsigned char RCMon_EC_Q4C0[ECDIGIT+1];	//当前C相4象限无功总电量(5+1)

	unsigned char RCMon_EC_SpC0[ECDIGIT+1];	//当前C相正向视在总电量(5+1)
	unsigned char RCMon_EC_SnC0[ECDIGIT+1];	//当前C相反向视在总电量(5+1)

	unsigned char RCMon_EC_GC0[ECDIGIT+1];	//当前C相关联总电量(5+1)
	unsigned char RCMon_EC_BpC0[ECDIGIT+1];	//当前C相正向有功基波总电量(5+1)				
	unsigned char RCMon_EC_BnC0[ECDIGIT+1];	//当前C相反向有功基波总电量(5+1)				
	unsigned char RCMon_EC_HpC0[ECDIGIT+1];	//当前C相正向有功谐波总电量(5+1)				
	unsigned char RCMon_EC_HnC0[ECDIGIT+1];	//当前C相反向有功谐波总电量(5+1)				
	unsigned char RCMon_EC_CuC0[ECDIGIT+1];	//当前C相铜损有功总电能补偿量(5+1)								
	unsigned char RCMon_EC_FeC0[ECDIGIT+1];	//当前C相铁损有功总电能补偿量(5+1)				
*/
}ECRAM;			//696字节
/*
typedef struct
{
	unsigned short MDBuf_Pp[60];		//当前正向有功需量缓存区(2)
	unsigned short MDBuf_Pn[60];		//当前反向有功需量缓存区(2)
	unsigned short MDBuf_Q1[60];		//当前1象限无功需量缓存区(2)
	unsigned short MDBuf_Q2[60];		//当前2象限无功需量缓存区(2)
	unsigned short MDBuf_Q3[60];		//当前3象限无功需量缓存区(2)
	unsigned short MDBuf_Q4[60];		//当前4象限无功需量缓存区(2)
	unsigned short MDBuf_Sp[60];		//当前正向视在需量缓存区(2)
	unsigned short MDBuf_Sn[60];		//当前反向视在需量缓存区(2)
}MDBUFZ;

typedef struct
{
	unsigned short MDBuf_PpA[60];		//当前A相正向有功需量缓存区(2)
	unsigned short MDBuf_PnA[60];		//当前A相反向有功需量缓存区(2)
	unsigned short MDBuf_Q1A[60];		//当前A相1象限无功需量缓存区(2)
	unsigned short MDBuf_Q2A[60];		//当前A相2象限无功需量缓存区(2)
	unsigned short MDBuf_Q3A[60];		//当前A相3象限无功需量缓存区(2)
	unsigned short MDBuf_Q4A[60];		//当前A相4象限无功需量缓存区(2)
	unsigned short MDBuf_SpA[60];		//当前A相正向视在需量缓存区(2)
	unsigned short MDBuf_SnA[60];		//当前A相反向视在需量缓存区(2)
}MDBUFA;

typedef struct
{
	unsigned short MDBuf_PpB[60];		//当前A相正向有功需量缓存区(2)
	unsigned short MDBuf_PnB[60];		//当前A相反向有功需量缓存区(2)
	unsigned short MDBuf_Q1B[60];		//当前A相1象限无功需量缓存区(2)
	unsigned short MDBuf_Q2B[60];		//当前A相2象限无功需量缓存区(2)
	unsigned short MDBuf_Q3B[60];		//当前A相3象限无功需量缓存区(2)
	unsigned short MDBuf_Q4B[60];		//当前A相4象限无功需量缓存区(2)
	unsigned short MDBuf_SpB[60];		//当前A相正向视在需量缓存区(2)
	unsigned short MDBuf_SnB[60];		//当前A相反向视在需量缓存区(2)
}MDBUFB;

typedef struct
{
	unsigned short MDBuf_PpC[60];		//当前C相正向有功需量缓存区(2)
	unsigned short MDBuf_PnC[60];		//当前C相反向有功需量缓存区(2)
	unsigned short MDBuf_Q1C[60];		//当前C相1象限无功需量缓存区(2)
	unsigned short MDBuf_Q2C[60];		//当前C相2象限无功需量缓存区(2)
	unsigned short MDBuf_Q3C[60];		//当前C相3象限无功需量缓存区(2)
	unsigned short MDBuf_Q4C[60];		//当前C相4象限无功需量缓存区(2)
	unsigned short MDBuf_SpC[60];		//当前C相正向视在需量缓存区(2)
	unsigned short MDBuf_SnC[60];		//当前C相反向视在需量缓存区(2)
}MDBUFC;

typedef struct
{
	unsigned long MD_CumPp;			//1分钟内有功正向功率			//PWMD			
	unsigned long MD_CumPn;
	unsigned long MD_CumQ1;
	unsigned long MD_CumQ2;
	unsigned long MD_CumQ3;
	unsigned long MD_CumQ4;
	unsigned long MD_CumSp;			
	unsigned long MD_CumSn;
	
	unsigned long MD_CumPpA;		//1分钟内A相有功正向功率			//PWMD			
	unsigned long MD_CumPnA;
	unsigned long MD_CumQ1A;
	unsigned long MD_CumQ2A;
	unsigned long MD_CumQ3A;
	unsigned long MD_CumQ4A;
	unsigned long MD_CumSpA;		
	unsigned long MD_CumSnA;

	unsigned long MD_CumPpB;		//1分钟内B相有功正向功率			//PWMD			
	unsigned long MD_CumPnB;
	unsigned long MD_CumQ1B;
	unsigned long MD_CumQ2B;
	unsigned long MD_CumQ3B;
	unsigned long MD_CumQ4B;
	unsigned long MD_CumSpB;		
	unsigned long MD_CumSnB;

	unsigned long MD_CumPpC;		//1分钟内C相有功正向功率			//PWMD			
	unsigned long MD_CumPnC;
	unsigned long MD_CumQ1C;
	unsigned long MD_CumQ2C;
	unsigned long MD_CumQ3C;
	unsigned long MD_CumQ4C;
	unsigned long MD_CumSpC;		
	unsigned long MD_CumSnC;

	unsigned short MD_CumPCnt;			//1分钟内有功功率累加次数	//PWMD			
	unsigned short MD_CumQCnt;			//1分钟内无功功率累加次数	//PWMD			
	unsigned short MD_CumSCnt;			//1分钟内视在功率累加次数	//PWMD			

//总需量缓存区计数器在费率切换时不清零，费率需量缓存区计数器在费率切换时清零
	unsigned char MD_Cunt;				//整分钟计算需量时需要向前调取整分钟脉冲缓存区个数
	unsigned char MD_CuntT;				//整分钟计算费率需量时需要向前调取整分钟脉冲缓存区个数
	unsigned char MD_BufAds;			//当前需量缓存区地址

	unsigned char MDPCT;				//整个需量周期内完成需量滑差时间次数：滑差片数
	unsigned char MD_PCT;				//滑差片数计数器	
	unsigned char MDPMinCT;				//需量滑差时间内分钟计数器
	unsigned char MD_60T;				//60分钟/需量周期

	unsigned char MDInitialTime;		//上电、清零、时钟调整、需量周期变更
	unsigned char FeeMDInitialTime;		//时段切换后费率需量周期变更

	unsigned char MD_Sec;				//秒
	unsigned char MD_Sec_Third;			//1/3秒
	unsigned char MD_Min;				//分钟
	unsigned char MD_Fee;				//当前需量费率
	
	unsigned char AvePowerTime;			//1分钟平均功率计算起始时间
	BCD3		AvePower_P;				//1分钟平均功率			

	BCD3		MD_P;					//当前有功需量			
	BCD3		MD_Q;					//当前无功需量			
	BCD3		MD_S;					//当前视在需量			
	            
	BCD3		MD_Pp;					//正向有功需量
	BCD3		MD_Pn;					//反向有功需量
	BCD3		MD_Q1;					//第一象限无功需量
	BCD3		MD_Q2;					//第二象限无功需量
	BCD3		MD_Q3;					//第三象限无功需量
	BCD3		MD_Q4;					//第四象限无功需量
	BCD3		MD_Sp;					//正向视在需量			
	BCD3		MD_Sn;					//反向视在需量			

}MDVAR;					//3840字节
*/

typedef struct
{
	unsigned short PL_CumPp;				//当前正向有功电量脉冲计数器(2)
	unsigned short PL_CumPn;				//当前反向有功电量脉冲计数器(2)
	unsigned short PL_CumQp;				//当前正向无功电量脉冲计数器(2)
	unsigned short PL_CumQn;				//当前反向无功电量脉冲计数器(2)
	unsigned short PL_CumQ1;				//当前1象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ2;				//当前2象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ3;				//当前3象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ4;				//当前4象限无功电量脉冲计数器(2)
	unsigned short PL_CumSp;				//当前正向视在电量脉冲计数器(2)
	unsigned short PL_CumSn;				//当前反向视在电量脉冲计数器(2)
/*	
	unsigned short PL_CumG;				//当前关联电量脉冲计数器(2)
	unsigned short PL_CumBp;				//当前正向有功基波电量脉冲计数器(2)
	unsigned short PL_CumBn;				//当前反向有功基波电量脉冲计数器(2)
	unsigned short PL_CumHp;				//当前正向有功谐波电量脉冲计数器(2)
	unsigned short PL_CumHn;				//当前反向有功谐波电量脉冲计数器(2)
	unsigned short PL_CumCu;				//当前铜损有功电量脉冲计数器(2)
	unsigned short PL_CumFe;				//当前铁损有功电量脉冲计数器(2)
	
	unsigned short PL_CumPpA;				//当前A相正向有功电量脉冲计数器(2)
	unsigned short PL_CumPnA;				//当前A相反向有功电量脉冲计数器(2)
	unsigned short PL_CumQ1A;				//当前A相1象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ2A;				//当前A相2象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ3A;				//当前A相3象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ4A;				//当前A相4象限无功电量脉冲计数器(2)
	unsigned short PL_CumSpA;				//当前A相正向视在电量脉冲计数器(2)
	unsigned short PL_CumSnA;				//当前A相反向视在电量脉冲计数器(2)
	
	unsigned short PL_CumGA;				//当前A相关联电量脉冲计数器(2)
	unsigned short PL_CumBpA;				//当前A相正向有功基波电量脉冲计数器(2)
	unsigned short PL_CumBnA;				//当前A相反向有功基波电量脉冲计数器(2)
	unsigned short PL_CumHpA;				//当前A相正向有功谐波电量脉冲计数器(2)
	unsigned short PL_CumHnA;				//当前A相反向有功谐波电量脉冲计数器(2)
	unsigned short PL_CumCuA;				//当前A相铜损有功电量脉冲计数器(2)
	unsigned short PL_CumFeA;				//当前A相铁损有功电量脉冲计数器(2)
	
	unsigned short PL_CumPpB;				//当前B相正向有功电量脉冲计数器(2)
	unsigned short PL_CumPnB;				//当前B相反向有功电量脉冲计数器(2)
	unsigned short PL_CumQ1B;				//当前B相1象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ2B;				//当前B相2象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ3B;				//当前B相3象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ4B;				//当前B相4象限无功电量脉冲计数器(2)
	unsigned short PL_CumSpB;				//当前B相正向视在电量脉冲计数器(2)
	unsigned short PL_CumSnB;				//当前B相反向视在电量脉冲计数器(2)
	
	unsigned short PL_CumGB;				//当前B相关联电量脉冲计数器(2)
	unsigned short PL_CumBpB;				//当前B相正向有功基波电量脉冲计数器(2)
	unsigned short PL_CumBnB;				//当前B相反向有功基波电量脉冲计数器(2)
	unsigned short PL_CumHpB;				//当前B相正向有功谐波电量脉冲计数器(2)
	unsigned short PL_CumHnB;				//当前B相反向有功谐波电量脉冲计数器(2)
	unsigned short PL_CumCuB;				//当前B相铜损有功电量脉冲计数器(2)
	unsigned short PL_CumFeB;				//当前B相铁损有功电量脉冲计数器(2)
	
	unsigned short PL_CumPpC;				//当前C相正向有功电量脉冲计数器(2)
	unsigned short PL_CumPnC;				//当前C相反向有功电量脉冲计数器(2)
	unsigned short PL_CumQ1C;				//当前C相1象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ2C;				//当前C相2象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ3C;				//当前C相3象限无功电量脉冲计数器(2)
	unsigned short PL_CumQ4C;				//当前C相4象限无功电量脉冲计数器(2)
	unsigned short PL_CumSpC;				//当前C相正向视在电量脉冲计数器(2)
	unsigned short PL_CumSnC;				//当前C相反向视在电量脉冲计数器(2)
	
	unsigned short PL_CumGC;				//当前C相关联电量脉冲计数器(2)
	unsigned short PL_CumBpC;				//当前C相正向有功基波电量脉冲计数器(2)
	unsigned short PL_CumBnC;				//当前C相反向有功基波电量脉冲计数器(2)
	unsigned short PL_CumHpC;				//当前C相正向有功谐波电量脉冲计数器(2)
	unsigned short PL_CumHnC;				//当前C相反向有功谐波电量脉冲计数器(2)
	unsigned short PL_CumCuC;				//当前C相铜损有功电量脉冲计数器(2)
	unsigned short PL_CumFeC;				//当前C相铁损有功电量脉冲计数器(2)
	*/

	unsigned char PL_ChkPp;				//当前正向有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkPn;				//当前反向有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQp;				//当前正向无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQn;				//当前反向无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ1;				//当前1象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ2;				//当前2象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ3;				//当前3象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ4;				//当前4象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkSp;				//当前正向视在电量脉冲计数器校验和(1)
	unsigned char PL_ChkSn;				//当前反向视在电量脉冲计数器校验和(1)
/*	
	unsigned char PL_ChkG;				//当前关联电量脉冲计数器校验和(1)
	unsigned char PL_ChkBp;				//当前正向有功基波电量脉冲计数器校验和(1)
	unsigned char PL_ChkBn;				//当前反向有功基波电量脉冲计数器校验和(1)
	unsigned char PL_ChkHp;				//当前正向有功谐波电量脉冲计数器校验和(1)
	unsigned char PL_ChkHn;				//当前反向有功谐波电量脉冲计数器校验和(1)
	unsigned char PL_ChkCu;				//当前铜损有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkFe;				//当前铁损有功电量脉冲计数器校验和(1)
	
	unsigned char PL_ChkPpA;			//当前A相正向有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkPnA;			//当前A相反向有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ1A;			//当前A相1象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ2A;			//当前A相2象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ3A;			//当前A相3象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ4A;			//当前A相4象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkSpA;			//当前A相正向视在电量脉冲计数器校验和(1)
	unsigned char PL_ChkSnA;			//当前A相反向视在电量脉冲计数器校验和(1)
	
	unsigned char PL_ChkGA;				//当前A相关联电量脉冲计数器校验和(1)
	unsigned char PL_ChkBpA;			//当前A相正向有功基波电量脉冲计数器校验和(1)
	unsigned char PL_ChkBnA;			//当前A相反向有功基波电量脉冲计数器校验和(1)
	unsigned char PL_ChkHpA;			//当前A相正向有功谐波电量脉冲计数器校验和(1)
	unsigned char PL_ChkHnA;			//当前A相反向有功谐波电量脉冲计数器校验和(1)
	unsigned char PL_ChkCuA;			//当前A相铜损有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkFeA;			//当前A相铁损有功电量脉冲计数器校验和(1)
	
	unsigned char PL_ChkPpB;			//当前B相正向有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkPnB;			//当前B相反向有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ1B;			//当前B相1象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ2B;			//当前B相2象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ3B;			//当前B相3象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ4B;			//当前B相4象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkSpB;			//当前B相正向视在电量脉冲计数器校验和(1)
	unsigned char PL_ChkSnB;			//当前B相反向视在电量脉冲计数器校验和(1)
	
	unsigned char PL_ChkGB;				//当前B相关联电量脉冲计数器校验和(1)
	unsigned char PL_ChkBpB;			//当前B相正向有功基波电量脉冲计数器校验和(1)
	unsigned char PL_ChkBnB;			//当前B相反向有功基波电量脉冲计数器校验和(1)
	unsigned char PL_ChkHpB;			//当前B相正向有功谐波电量脉冲计数器校验和(1)
	unsigned char PL_ChkHnB;			//当前B相反向有功谐波电量脉冲计数器校验和(1)
	unsigned char PL_ChkCuB;			//当前B相铜损有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkFeB;			//当前B相铁损有功电量脉冲计数器校验和(1)
	
	unsigned char PL_ChkPpC;			//当前C相正向有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkPnC;			//当前C相反向有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ1C;			//当前C相1象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ2C;			//当前C相2象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ3C;			//当前C相3象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ4C;			//当前C相4象限无功电量脉冲计数器校验和(1)
	unsigned char PL_ChkSpC;			//当前C相正向视在电量脉冲计数器校验和(1)
	unsigned char PL_ChkSnC;			//当前C相反向视在电量脉冲计数器校验和(1)
	
	unsigned char PL_ChkGC;				//当前C相关联电量脉冲计数器校验和(1)
	unsigned char PL_ChkBpC;			//当前C相正向有功基波电量脉冲计数器校验和(1)
	unsigned char PL_ChkBnC;			//当前C相反向有功基波电量脉冲计数器校验和(1)
	unsigned char PL_ChkHpC;			//当前C相正向有功谐波电量脉冲计数器校验和(1)
	unsigned char PL_ChkHnC;			//当前C相反向有功谐波电量脉冲计数器校验和(1)
	unsigned char PL_ChkCuC;			//当前C相铜损有功电量脉冲计数器校验和(1)
	unsigned char PL_ChkFeC;			//当前C相铁损有功电量脉冲计数器校验和(1)
*/
	unsigned short PL_CumPp_Dis;		//当前正向有功电量脉冲计数器--显示进度条用
	unsigned short PL_CumPn_Dis;		//当前反向有功电量脉冲计数器--显示进度条用

}ECPULSE;					//180字节

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
typedef struct
{
	unsigned short PL_CumPp1;				//当前正向有功费率1电量脉冲计数器(2)
	unsigned short PL_CumPp2;				//当前正向有功费率2电量脉冲计数器(2)
	unsigned short PL_CumPp3;				//当前正向有功费率3电量脉冲计数器(2)
	unsigned short PL_CumPp4;				//当前正向有功费率4电量脉冲计数器(2)

	unsigned short PL_CumPn1;				//当前反向有功费率1电量脉冲计数器(2)
	unsigned short PL_CumPn2;				//当前反向有功费率2电量脉冲计数器(2)
	unsigned short PL_CumPn3;				//当前反向有功费率3电量脉冲计数器(2)
	unsigned short PL_CumPn4;				//当前反向有功费率4电量脉冲计数器(2)

	unsigned short PL_CumQp1;				//当前正向无功费率1电量脉冲计数器(2)
	unsigned short PL_CumQp2;				//当前正向无功费率2电量脉冲计数器(2)
	unsigned short PL_CumQp3;				//当前正向无功费率3电量脉冲计数器(2)
	unsigned short PL_CumQp4;				//当前正向无功费率4电量脉冲计数器(2)

	unsigned short PL_CumQn1;				//当前反向无功费率1电量脉冲计数器(2)
	unsigned short PL_CumQn2;				//当前反向无功费率2电量脉冲计数器(2)
	unsigned short PL_CumQn3;				//当前反向无功费率3电量脉冲计数器(2)
	unsigned short PL_CumQn4;				//当前反向无功费率4电量脉冲计数器(2)

	unsigned short PL_CumQ11;				//当前1象限无功费率1电量脉冲计数器(2)
	unsigned short PL_CumQ12;				//当前1象限无功费率2电量脉冲计数器(2)
	unsigned short PL_CumQ13;				//当前1象限无功费率3电量脉冲计数器(2)
	unsigned short PL_CumQ14;				//当前1象限无功费率4电量脉冲计数器(2)

	unsigned short PL_CumQ21;				//当前2象限无功费率1电量脉冲计数器(2)
	unsigned short PL_CumQ22;				//当前2象限无功费率2电量脉冲计数器(2)
	unsigned short PL_CumQ23;				//当前2象限无功费率3电量脉冲计数器(2)
	unsigned short PL_CumQ24;				//当前2象限无功费率4电量脉冲计数器(2)

	unsigned short PL_CumQ31;				//当前3象限无功费率1电量脉冲计数器(2)
	unsigned short PL_CumQ32;				//当前3象限无功费率2电量脉冲计数器(2)
	unsigned short PL_CumQ33;				//当前3象限无功费率3电量脉冲计数器(2)
	unsigned short PL_CumQ34;				//当前3象限无功费率4电量脉冲计数器(2)

	unsigned short PL_CumQ41;				//当前4象限无功费率1电量脉冲计数器(2)
	unsigned short PL_CumQ42;				//当前4象限无功费率2电量脉冲计数器(2)
	unsigned short PL_CumQ43;				//当前4象限无功费率3电量脉冲计数器(2)
	unsigned short PL_CumQ44;				//当前4象限无功费率4电量脉冲计数器(2)

	unsigned char PL_ChkPp1;			//当前正向有功费率1电量脉冲计数器校验和(1)
	unsigned char PL_ChkPp2;			//当前正向有功费率2电量脉冲计数器校验和(1)
	unsigned char PL_ChkPp3;			//当前正向有功费率3电量脉冲计数器校验和(1)
	unsigned char PL_ChkPp4;			//当前正向有功费率4电量脉冲计数器校验和(1)

	unsigned char PL_ChkPn1;			//当前反向有功费率1电量脉冲计数器校验和(1)
	unsigned char PL_ChkPn2;			//当前反向有功费率2电量脉冲计数器校验和(1)
	unsigned char PL_ChkPn3;			//当前反向有功费率3电量脉冲计数器校验和(1)
	unsigned char PL_ChkPn4;			//当前反向有功费率4电量脉冲计数器校验和(1)

	unsigned char PL_ChkQp1;			//当前正向无功费率1电量脉冲计数器校验和(1)
	unsigned char PL_ChkQp2;			//当前正向无功费率2电量脉冲计数器校验和(1)
	unsigned char PL_ChkQp3;			//当前正向无功费率3电量脉冲计数器校验和(1)
	unsigned char PL_ChkQp4;			//当前正向无功费率4电量脉冲计数器校验和(1)

	unsigned char PL_ChkQn1;			//当前反向无功费率1电量脉冲计数器校验和(1)
	unsigned char PL_ChkQn2;			//当前反向无功费率2电量脉冲计数器校验和(1)
	unsigned char PL_ChkQn3;			//当前反向无功费率3电量脉冲计数器校验和(1)
	unsigned char PL_ChkQn4;			//当前反向无功费率4电量脉冲计数器校验和(1)

	unsigned char PL_ChkQ11;			//当前1象限无功费率1电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ12;			//当前1象限无功费率2电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ13;			//当前1象限无功费率3电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ14;			//当前1象限无功费率4电量脉冲计数器校验和(1)

	unsigned char PL_ChkQ21;			//当前2象限无功费率1电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ22;			//当前2象限无功费率2电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ23;			//当前2象限无功费率3电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ24;			//当前2象限无功费率4电量脉冲计数器校验和(1)

	unsigned char PL_ChkQ31;			//当前3象限无功费率1电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ32;			//当前3象限无功费率2电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ33;			//当前3象限无功费率3电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ34;			//当前3象限无功费率4电量脉冲计数器校验和(1)

	unsigned char PL_ChkQ41;			//当前4象限无功费率1电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ42;			//当前4象限无功费率2电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ43;			//当前4象限无功费率3电量脉冲计数器校验和(1)
	unsigned char PL_ChkQ44;			//当前4象限无功费率4电量脉冲计数器校验和(1)

}FeeECPULSE;					//72字节
#endif

#if ( MEASCHIP == IDT90E32 )
typedef struct
{
	unsigned short PL_RegPT;				//当前有功寄存器脉冲计数器(2)
	unsigned short PL_RegPA;				//当前A相有功寄存器脉冲计数器(2)
	unsigned short PL_RegPB;				//当前B相有功寄存器脉冲计数器(2)
	unsigned short PL_RegPC;				//当前C相有功寄存器脉冲计数器(2)
//	unsigned short PL_RegPn;				//当前反向有功寄存器脉冲计数器(2)
//	unsigned short PL_RegPnA;				//当前A相反向有功寄存器脉冲计数器(2)
//	unsigned short PL_RegPnB;				//当前B相反向有功寄存器脉冲计数器(2)
//	unsigned short PL_RegPnC;				//当前C相反向有功寄存器脉冲计数器(2)

	unsigned short PL_RegQT;				//当前无功寄存器脉冲计数器(2)
	unsigned short PL_RegQA;				//当前A相无功寄存器脉冲计数器(2)
	unsigned short PL_RegQB;				//当前B相无功寄存器脉冲计数器(2)
	unsigned short PL_RegQC;				//当前C相无功寄存器脉冲计数器(2)
//	unsigned short PL_RegQn;				//当前反向无功寄存器脉冲计数器(2)
//	unsigned short PL_RegQnA;				//当前A相反向无功寄存器脉冲计数器(2)
//	unsigned short PL_RegQnB;				//当前B相反向无功寄存器脉冲计数器(2)
//	unsigned short PL_RegQnC;				//当前C相反向无功寄存器脉冲计数器(2)

	unsigned short PL_RegST;				//当前视在寄存器脉冲计数器(2)
	unsigned short PL_RegSA;				//当前A相视在寄存器脉冲计数器(2)
	unsigned short PL_RegSB;				//当前B相视在寄存器脉冲计数器(2)
	unsigned short PL_RegSC;				//当前C相视在寄存器脉冲计数器(2)

	unsigned char PL_SparePp;			//当前正向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SparePn;			//当前反向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ1;			//当前1象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ2;			//当前2象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ3;			//当前3象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ4;			//当前4象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareSp;			//当前正向视在0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareSn;			//当前反向视在0.1CF以下剩余脉冲计数器(2)

	unsigned char PL_SparePpA;			//当前A相正向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SparePnA;			//当前A相反向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ1A;			//当前A相1象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ2A;			//当前A相2象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ3A;			//当前A相3象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ4A;			//当前A相4象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareSpA;			//当前A相正向视在0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareSnA;			//当前A相反向视在0.1CF以下剩余脉冲计数器(2)

	unsigned char PL_SparePpB;			//当前B相正向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SparePnB;			//当前B相反向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ1B;			//当前B相1象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ2B;			//当前B相2象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ3B;			//当前B相3象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ4B;			//当前B相4象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareSpB;			//当前B相正向视在0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareSnB;			//当前B相反向视在0.1CF以下剩余脉冲计数器(2)

	unsigned char PL_SparePpC;			//当前C相正向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SparePnC;			//当前C相反向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ1C;			//当前C相1象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ2C;			//当前C相2象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ3C;			//当前C相3象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareQ4C;			//当前C相4象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareSpC;			//当前C相正向视在0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_SpareSnC;			//当前C相反向视在0.1CF以下剩余脉冲计数器(2)

	unsigned char PL_ChkSparePp;		//当前正向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSparePn;		//当前反向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ1;		//当前1象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ2;		//当前2象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ3;		//当前3象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ4;		//当前4象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareSp;		//当前正向视在0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareSn;		//当前反向视在0.1CF以下剩余脉冲计数器(2)

	unsigned char PL_ChkSparePpA;		//当前A相正向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSparePnA;		//当前A相反向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ1A;		//当前A相1象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ2A;		//当前A相2象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ3A;		//当前A相3象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ4A;		//当前A相4象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareSpA;		//当前A相正向视在0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareSnA;		//当前A相反向视在0.1CF以下剩余脉冲计数器(2)
                        
	unsigned char PL_ChkSparePpB;		//当前B相正向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSparePnB;		//当前B相反向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ1B;		//当前B相1象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ2B;		//当前B相2象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ3B;		//当前B相3象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ4B;		//当前B相4象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareSpB;		//当前B相正向视在0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareSnB;		//当前B相反向视在0.1CF以下剩余脉冲计数器(2)
                        
	unsigned char PL_ChkSparePpC;		//当前C相正向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSparePnC;		//当前C相反向有功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ1C;		//当前C相1象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ2C;		//当前C相2象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ3C;		//当前C相3象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareQ4C;		//当前C相4象限无功0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareSpC;		//当前C相正向视在0.1CF以下剩余脉冲计数器(2)
	unsigned char PL_ChkSpareSnC;		//当前C相反向视在0.1CF以下剩余脉冲计数器(2)

}REGPULSE;
#endif

typedef struct
{
#if ( MEASCHIP == ADE7878 )	
 	unsigned long EcPSumA;				//A相有功电量累计缓存
	unsigned long EcPSumB;				//B相有功电量累计缓存
	unsigned long EcPSumC;				//C相有功电量累计缓存
	unsigned long EcQSumA;				//A相无功电量累计缓存
	unsigned long EcQSumB;				//B相无功电量累计缓存
	unsigned long EcQSumC;				//C相无功电量累计缓存
#endif

	unsigned long SMEcSumA;				//0.01度A相视在电量累计缓存
	unsigned long SMEcSumB;				//0.01度B相视在电量累计缓存
	unsigned long SMEcSumC;				//0.01度C相视在电量累计缓存
	unsigned long SMEcSum;				//0.01度总视在电量累计缓存
	
	unsigned char YPulse_Mode;			//有功脉冲检测移位寄存器
	unsigned char WPulse_Mode;			//无功脉冲检测移位寄存器

	unsigned char YPulse_Cum;			//中断有功脉冲计数器
	unsigned char WPulse_Cum;			//中断无功脉冲计数器
	
#if ( MEASCHIP == ADE7878 )	
 	unsigned char EcPSumAChk;			//A相有功电量累计缓存校验和
	unsigned char EcPSumBChk;			//B相有功电量累计缓存校验和
	unsigned char EcPSumCChk;			//C相有功电量累计缓存校验和
	unsigned char EcQSumAChk;			//A相无功电量累计缓存校验和
	unsigned char EcQSumBChk;			//B相无功电量累计缓存校验和
	unsigned char EcQSumCChk;			//C相无功电量累计缓存校验和
#endif

	unsigned char SMEcSumAChk;			//0.01度A相视在电量累计缓存
	unsigned char SMEcSumBChk;			//0.01度B相视在电量累计缓存
	unsigned char SMEcSumCChk;			//0.01度C相视在电量累计缓存
	unsigned char SMEcSumChk;			//0.01度总视在电量累计缓存
	
	unsigned char YPulse_CumChk;		//中断有功脉冲计数器
	unsigned char WPulse_CumChk;		//中断无功脉冲计数器
	
}INTPULSE;					

#if ( MEASCHIP == ADE7878 )	
typedef struct
{
	unsigned long* PhaseECSum;			//ADE7878相电量累加缓存区
	unsigned char* PhaseECSumChk;		//ADE7878相电量累加缓存区校验和
}PHASE_ECBUF;	
#endif
/*
typedef struct
{
	unsigned char Cunt;		//事件稳定时间
	unsigned char State;	//事件前1秒状态			//33：未发生	55：事件发生
	unsigned char OldState;	//事件当前状态
	unsigned char Sec;		//事件时标
	
}EVENT_STATE;				//事件状态变量
	
typedef struct
{
	EVENT_STATE PhFailVar;			//电表总失压状态字结构    
	EVENT_STATE PhAFailVar;			//电表A相失压状态字结构    
	EVENT_STATE PhBFailVar;			//电表B相失压状态字结构 
	EVENT_STATE PhCFailVar;			//电表C相失压状态字结构 

	EVENT_STATE PhLowerVar;			//电表总欠压状态字结构 
	EVENT_STATE PhALowerVar;		//电表A相欠压状态字结构 
	EVENT_STATE PhBLowerVar;		//电表B相欠压状态字结构 
	EVENT_STATE PhCLowerVar;		//电表C相欠压状态字结构 

	EVENT_STATE PhOverVar;			//电表总过压状态字结构 
	EVENT_STATE PhAOverVar;			//电表A相过压状态字结构 
	EVENT_STATE PhBOverVar;			//电表B相过压状态字结构 
	EVENT_STATE PhCOverVar;			//电表C相过压状态字结构 

	EVENT_STATE PhBreakVar;			//电表总断相状态字结构 
	EVENT_STATE PhABreakVar;		//电表A相断相状态字结构 
	EVENT_STATE PhBBreakVar;		//电表B相断相状态字结构 
	EVENT_STATE PhCBreakVar;		//电表C相断相状态字结构 

	EVENT_STATE VoltRevPhaseVar;	//电表电压逆相序状态字结构 
	EVENT_STATE CuRevPhaseVar;		//电表电压逆相序状态字结构 

	EVENT_STATE VoltImbalanceVar;	//电表电压不平衡状态字结构 
	EVENT_STATE CuImbalanceVar;		//电表电流不平衡状态字结构 
	EVENT_STATE CuBestImbalanceVar;	//电表电流严重不平衡状态字结构 

	EVENT_STATE CuFailVar;			//电表总失流状态字结构 
	EVENT_STATE CuAFailVar;			//电表A相失流状态字结构 
	EVENT_STATE CuBFailVar;			//电表B相失流状态字结构 
	EVENT_STATE CuCFailVar;			//电表C相失流状态字结构 

	EVENT_STATE CuOverVar;			//电表总过流状态字结构 
	EVENT_STATE CuAOverVar;			//电表A相过流状态字结构 
	EVENT_STATE CuBOverVar;			//电表B相过流状态字结构 
	EVENT_STATE CuCOverVar;			//电表C相过流状态字结构 

	EVENT_STATE CuBreakVar;			//电表总断流状态字结构 
	EVENT_STATE CuABreakVar;		//电表A相断流状态字结构 
	EVENT_STATE CuBBreakVar;		//电表B相断流状态字结构 
	EVENT_STATE CuCBreakVar;		//电表C相断流状态字结构 

	EVENT_STATE RevPowerVar;		//电表总潮流状态字结构 
	EVENT_STATE RevPowerAVar;		//电表A相潮流状态字结构 
	EVENT_STATE RevPowerBVar;		//电表B相潮流状态字结构 
	EVENT_STATE RevPowerCVar;		//电表C相潮流状态字结构 
	
	EVENT_STATE PowerOverVar;		//电表总过载状态字结构 
	EVENT_STATE PowerOverAVar;		//电表A相过载状态字结构 
	EVENT_STATE PowerOverBVar;		//电表B相过载状态字结构 
	EVENT_STATE PowerOverCVar;		//电表C相过载状态字结构 

	EVENT_STATE PftOverVar;			//电表功率因素超限状态字结构 
	
	EVENT_STATE TideSwitchVar;		//电表潮流状态字结构 				//新国网

//	EVENT_STATE PftAOverVar;		//电表A相功率因素超限状态字结构 	//新国网
//	EVENT_STATE PftBOverVar;		//电表B相功率因素超限状态字结构 	//新国网
//	EVENT_STATE PftCOverVar;		//电表C相功率因素超限状态字结构 	//新国网

	EVENT_STATE MagnetDisturbVar;	//恒定磁场干扰状态字结构 			//新国网
	EVENT_STATE RelayFaultVar;		//负荷开关误动作状态字结构 			//新国网

	EVENT_STATE RevPowerZVar;		//电表总潮流状态字结构 
	EVENT_STATE AllFailVar;			//全失压状态字结构 
	EVENT_STATE PdownVar;			//掉电状态字结构 

	EVENT_STATE MDOverPpVar;		//正向有功需量超限状态				//新国网	//13.10.23 
	EVENT_STATE MDOverPnVar;		//反向有功需量超限状态				//新国网	//13.10.23 
	EVENT_STATE MDOverQ1Var;		//第一象限无功需量超限状态			//新国网	//13.10.23 
	EVENT_STATE MDOverQ2Var;		//第二象限无功需量超限状态			//新国网	//13.10.23 
	EVENT_STATE MDOverQ3Var;		//第三象限无功需量超限状态			//新国网	//13.10.23 
	EVENT_STATE MDOverQ4Var;		//第四象限无功需量超限状态			//新国网	//13.10.23 

	EVENT_STATE reserveVar1;		//保留状态字结构 					//新国网	//13.10.23 
	EVENT_STATE reserveVar2;		//保留状态字结构 					//新国网	//13.10.23 	

//																		//新国网	//13.10.23 		
//	unsigned char 	MD_PpOverState;	//正向有功需量超限状态		
//	unsigned char 	MD_PnOverState;	//反向有功需量超限状态		      
//	unsigned char 	MD_Q1OverState;	//第一象限无功需量超限状态		  
//	unsigned char 	MD_Q2OverState;	//第二象限无功需量超限状态		  
//	unsigned char 	MD_Q3OverState;	//第三象限无功需量超限状态		  
//	unsigned char 	MD_Q4OverState;	//第四象限无功需量超限状态		  
	
	unsigned char	AuxPWFail_State;//辅助电源状态
	unsigned char	reserve;		//保留		
	
}EVENTVAR;							//93字节			

typedef struct
{
	unsigned long EventAmpHour;			//电压事件期间总累加安时数 = L1 xxx.xxxA * 1秒 + L2 xxx.xxxA * 1秒 + L3 xxx.xxxA * 1秒
	unsigned long EventAmpHourA;        //电压事件期间A相累加安时数 = L1 xxx.xxxA * 1秒                                       
	unsigned long EventAmpHourB;        //电压事件期间B相累加安时数 = L2 xxx.xxxA * 1秒                                       
	unsigned long EventAmpHourC;        //电压事件期间C相累加安时数 = L3 xxx.xxxA * 1秒                                       
	
	unsigned char EventAmpHourChk;		//电压事件期间总累加安时数校验和	
	unsigned char EventAmpHourChkA;		//电压事件期间A相累加安时数校验和	
	unsigned char EventAmpHourChkB;		//电压事件期间B相累加安时数校验和	
	unsigned char EventAmpHourChkC;		//电压事件期间C相累加安时数校验和	

}EVEAHOUR;

typedef struct
{
	unsigned short	EventNo;
	EVEAHOUR*		EveAHour;
	unsigned short 	AmpHourEAddr;		//事件中安时数在EEPROM中的首址
}EVENTAMPERHOUR;

typedef struct
{
	EVEAHOUR	PhFailAmpHour;			//总失压期间总及各相累加安时数
	EVEAHOUR	PhAFailAmpHour;			//A相失压期间总及各相累加安时数
	EVEAHOUR	PhBFailAmpHour;			//B相失压期间总及各相累加安时数
	EVEAHOUR	PhCFailAmpHour;			//C相失压期间总及各相累加安时数

	EVEAHOUR	PhLowerAmpHour;			//总欠压期间总及各相累加安时数
	EVEAHOUR	PhALowerAmpHour;		//A相欠压期间总及各相累加安时数
	EVEAHOUR	PhBLowerAmpHour;		//B相欠压期间总及各相累加安时数
	EVEAHOUR	PhCLowerAmpHour;		//C相欠压期间总及各相累加安时数
	
	EVEAHOUR	PhOverAmpHour;			//总过压期间总及各相累加安时数
	EVEAHOUR	PhAOverAmpHour;			//A相过压期间总及各相累加安时数
	EVEAHOUR	PhBOverAmpHour;			//B相过压期间总及各相累加安时数
	EVEAHOUR	PhCOverAmpHour;			//C相过压期间总及各相累加安时数
	
	EVEAHOUR	PhBreakAmpHour;			//总断相期间总及各相累加安时数
	EVEAHOUR	PhABreakAmpHour;		//A相断相期间总及各相累加安时数
	EVEAHOUR	PhBBreakAmpHour;		//B相断相期间总及各相累加安时数
	EVEAHOUR	PhCBreakAmpHour;		//C相断相期间总及各相累加安时数

}AMPERHOUR;

*/

typedef struct
{
  unsigned int HDLC_ADDR;
} PARA;

typedef struct
{
#if 0  
  unsigned long	Pt; 					//合相有功功率(4)
  unsigned long	Pa; 					//A相有功功率(4)	
  unsigned long	Pb; 					//B相有功功率(4)	
  unsigned long	Pc; 					//C相有功功率(4)	

  unsigned long	Qt; 					//合相无功功率(4)
  unsigned long	Qa; 					//A相无功功率(4)	
  unsigned long	Qb; 					//B相无功功率(4)	
  unsigned long	Qc; 					//C相无功功率(4)	
  	
  unsigned long	St; 					//合相视在功率(4)
  unsigned long	Sa; 					//A相视在功率(4)	
  unsigned long	Sb; 					//B相视在功率(4)	
  unsigned long	Sc; 					//C相视在功率(4)	
  
  unsigned long        Ia; 			//A相电流(4)		XXXX.XXXX A
  unsigned long	Ib; 			//B相电流(4)		XXXX.XXXX A
  unsigned long	Ic; 			//C相电流(4)		XXXX.XXXX A
  
  unsigned long        Ua; 			//A相电压(4)
  unsigned long	Ub; 			//B相电压(4)
  unsigned long	Uc; 			//C相电压(4)
  
  unsigned long	AFreq;			//A相频率(4)
  unsigned long	BFreq;			//B相频率(4)
  unsigned long	CFreq;			//C相频率(4)
  
  unsigned long	Pft;			//合相功率因数(4)
  unsigned long	Pfa;			//A相功率因数(4)
  unsigned long	Pfb;			//B相功率因数(4)
  unsigned long	Pfc;			//C相功率因数(4)
  
  unsigned long        Angle_Ia; 			//A相电流角度
  unsigned long        Angle_Ib; 			//B相电流角度
  unsigned long        Angle_Ic; 			//C相电流角度
  
  unsigned long        Angle_Ua; 			//A相电流角度
  unsigned long        Angle_Ub; 			//B相电流角度
  unsigned long        Angle_Uc; 			//C相电流角度
#else
  unsigned long	AFreq;			//A相频率(4)
  
  unsigned long        Ia; 			//A相电流(4)		XXXX.XXXX A
  unsigned long	Ib; 			//B相电流(4)		XXXX.XXXX A
  unsigned long	Ic; 			//C相电流(4)		XXXX.XXXX A
 
  unsigned long        Ua; 			//A相电压(4)
  unsigned long	Ub; 			//B相电压(4)
  unsigned long	Uc; 			//C相电压(4)
  
  unsigned long	Pa; 					//A相有功功率(4)	
  unsigned long	Pb; 					//B相有功功率(4)	
  unsigned long	Pc; 					//C相有功功率(4)	
  unsigned long	Pt; 					//合相有功功率(4)
  
  
  unsigned long	Qa; 					//A相无功功率(4)	
  unsigned long	Qb; 					//B相无功功率(4)	
  unsigned long	Qc; 					//C相无功功率(4)	
  unsigned long	Qt; 					//合相无功功率(4)
  
  
  unsigned long	Sa; 					//A相视在功率(4)	
  unsigned long	Sb; 					//B相视在功率(4)	
  unsigned long	Sc; 					//C相视在功率(4)	
  unsigned long	St; 					//合相视在功率(4)
  
  
  unsigned long	Pfa;			//A相功率因数(4)
  unsigned long	Pfb;			//B相功率因数(4)
  unsigned long	Pfc;			//C相功率因数(4)
  unsigned long	Pft;			//合相功率因数(4)
  
  unsigned long	BFreq;			//B相频率(4)
  unsigned long	CFreq;			//C相频率(4)
#endif  
}REAL;	

typedef struct
{
  unsigned long	Pp; 					//正向有功
  unsigned long	Qp; 					//正向无功	
  unsigned long	Q1; 					//1象限
  unsigned long	Q4; 					//4象限
  unsigned long	Pn; 					//反向有功
  unsigned long	Qn; 					//反向无功
  unsigned long	Q2; 					//2象限
  unsigned long	Q3; 					//3象限
  
}ENERGY;

//	Clock Timer Variable
typedef struct
{
//	long	AdjCT;
//	long 	Adj_Fact;
	short		Temp;
//	char	RTCOffset;

	char		Sec_64;
   	BCD1 		Sec;
   	BCD1 		Min;
   	BCD1 		Hour;
   	BCD1 		Day;
   	BCD1 		Mon;
   	BCD1 		Year;
   	BCD1		Week;

	unsigned char YearH;		//HEX时间	
	unsigned char YearL;
	unsigned char Month;
	unsigned char DayH;
	
	unsigned char WeekH;
	unsigned char HourH;
	unsigned char MinH;
	unsigned char SecH;
	unsigned char SecPer;
	unsigned char DeviatH;
	unsigned char DeviatL;
	unsigned char State;
	
//	unsigned char	Check;
	unsigned char daylightsaving;		//夏令时状态标志(1:开始进行;2:结束;0:无)..

} CLK;
	
typedef struct
{
	unsigned char Type;			//显示项类型 0：OAD;1：ROAD
	unsigned char OAD[4];		//主显示项内容
	unsigned char ROAD[4];		//副显示项内容
	unsigned char No;			//屏序号
}CURDISOBJ;
	
// Display  & Key registers
typedef struct
{
	enum {
		AutoMode,				// 轮显模式
		KeyMode,				// 按显模式
		LookMode,				// 液晶查看模式
	}TouchDisMode;				// 触发显示模式

	enum {
		FaultType,				// 故障类
		EventType,				// 事件类
		AutoType,				// 轮显类
		KeyType,				// 按显类
	}ContentDisType;			// 触发显示模式

	unsigned short FaultDisFlag;	//故障显示标志位
	unsigned short EventDisFlag;	//事件显示标志位

   unsigned short HWBatOnTime;			//红外触发停抄唤醒时间
   unsigned short HWBGDisTime;			//红外触发背光后点亮时间
	short ManuCode;				//厂家编码
	BCD2 UserCode;				//用户编码	
	BCD2 OldUserCode;				//用户编码					//V30
//	unsigned char TouchDisMode;			//触发显示模式 0：轮显 1：按显 2：液晶查看
//	unsigned char ContentDisMode;		//内容显示模式 0：故障类异常 1：事件类异常 2：轮显类 3：按显类

	unsigned char FullDisDelay;			// 上电显示20秒全屏
    signed char FaultItemPtr;			// 故障类表指针
    signed char EventItemPtr;			// 事件类表指针
    unsigned char FlautItemSum;			// 故障类显示项总数
    unsigned char EventItemSum;			// 事件类显示项总数

//	unsigned char Mode;					// 模式字
//	unsigned char KeyTmr;				// 按键扫描定时
//	unsigned char KeyLPCT;				// 按键长延时计数器
    unsigned char FlsCT;				// 闪烁计数器
    unsigned char DisInv;				// 轮显间隔计数器
    signed char KeyItemPtr;				// 按显表指针
    signed char AutoItemPtr;			// 轮显表指针
    unsigned char KeyItemSum;			// 显示项总数
    unsigned char AutoItemSum;			// 轮显项总数
//    unsigned char ItemSum;			// 显示项总数
//    unsigned char LXItemSum;			// 轮显项总数
	unsigned char DisFeeNo;
    unsigned char LCDDisEnable;			//LCD显示使能
   
    CURDISOBJ CurDisObj;				//当前显示项内容
    
	unsigned char CurDisID[7];			// 当前显示项ID码、其内部序号、显示用户号
//    unsigned char LXTab[16];			// LCD段位缓存
    unsigned char LCDSeg[32];			// LCD段位缓存
//    unsigned char FLCDSeg[32];			// LCD段位缓存

	unsigned char DayFreezeDelay;		//修改时间后延时判断补日冻结时间	//新国网
	BCD5 BeforeDayFreezeTime;			//补日冻结前时间					//新国网	

//#if( ECDisDecDigitsValid ==	YesCheck )			//13.03.03 当前电量显示小数位真实有效
	unsigned char DigitsValidFlg;		//当前电量显示小数位真实有效标记，显示前置位0x55，显示后清零
//	BCD7	DisReserve;					//保留8字节空间		
	unsigned char 	RHighZeroFlag;	//高位零显示标志(2)	0：高位显示0；其他：高位不显示0；	//新国网	//14.01.13
	BCD6	DisReserve;					//保留8字节空间										//新国网	//14.01.13	
//#else
//	BCD8	DisReserve;					//保留8字节空间		
//	unsigned char 	RHighZeroFlag;	//高位零显示标志(2)	0：高位显示0；其他：高位不显示0；	//新国网	//14.01.13
//	BCD7	DisReserve;					//保留8字节空间										//新国网	//14.01.13	
//#endif
	unsigned char Mode;					// 模式字

	unsigned char Mode1ItemPtr; 		// 模式1轮显表指针
	unsigned char Mode2ItemPtr; 		// 模式2轮显表指针
	unsigned char Mode3ItemPtr; 		// 模式3轮显表指针
	unsigned short Class_Id;				//当前显示项的类
	unsigned char Logical_Name[6];		//当前显示项的OBIS吗
	unsigned char Attribute;			//当前显示项在该类中的属性	
	unsigned short  Data_Index;	  //显示刷新..
//	unsigned char LCDSeg[24];			// LCD段位缓存
     unsigned short ItemPtr;				// 轮显表指针   
//    unsigned char ItemSum;			// 显示项总数
    unsigned char ItemNo;				// 显示项序号

} DISKEY;
/*
typedef struct
{
	BCD4	STime;
	BCD4	ETime;
	BCD4	CTime;		//用于抄日负荷表
	unsigned char Mode;		//0:不抄负荷、1:抄日负荷表、2:抄窃电记录
} DLOAD;	

typedef struct
{
	BCD3	STime;
	BCD3	ETime;
	unsigned char Mode;		//0:不抄、1:按日期抄收、2:全抄
	short		Ptr;			//当前记录指针
} TRECORD;	
*/
/*******************************************************************
	Even Log Profile Information Structure Define
*******************************************************************/
// Bit Flag
	// Power flags define
#define  F_PwrUp     	0x02
#define  F_IrmsCheck	0x01		//0:测电流；1：电流校准
#define  F_AllFail		0x04		//0:非全失压;1:全失压
#define  F_ByPass 		0x40		//电流旁路报警 
//#define  F_ErrMeas		0x08		//计量芯片出错
//#define  F_FailIrmsMeas	0x10		//校完表后第一次电流检测 0：已检测过；1：未检测
//#define	 F_NCheckLoss	0x20		//零线检测。0：正常，1：掉零线

#define  F_IbIrmsCheck	0x08		//0:测电流；1：全失压100%Ib电流校准
#define  F_Ib10IrmsCheck	0x80	//0:测电流；1：全失压10%Ib电流校准
#define  F_Ib00IrmsCheck	0x10	//0:测电流；1：全失压0%Ib电流校准


	// Run flags define
#define  F_ComOK1   0x01
//#define	 F_THW		0x02	//红外和485独立
#define  F_ComOK3   0x02
#define  F_Batt     0x04
//#define	 F_T485		0x08	//红外和485独立
#define  F_Hold		0x08		//16.10.20
#define	 F_AdsOk	0x10
#define	 F_MimaOk	0x20
#define  F_ComOK2   0x40
#define  F_Parity   0x80

	// Clk flags define
#define  F_Sec       0x01
#define  F_Min       0x02
#define  F_Hour      0x04
#define  F_Day       0x08

#define  F_DisSec    0x10
#define  F_HalfSec   0x20		//V1001		//PWMD
#define  F_ThirdSec  0x40		//PWMD		//三分之一秒、用于计算需量

	// DisK Flag Bit
#define  F_KeyInt    	0x80
#define  F_DspALT		0x40
//#define	 F_HWKeyInt    	0x20	//V30
#define  F_KeyStart    	0x20

#define  F_Flash		0x08
//#define  F_KeyS	    	0x04		//新国网	//13.10.17	启用于液晶驱动芯片首次上电标记
#define  F_KeyS	    	0x04		//保留

#define  F_Key       	0x03
#define  F_Key1      	0x01		//V30 已利用，作为实时费率是否显示的标记 ＝1 显示；＝0不显示。上下电用圆圈1指示
#define  F_Key2      	0x02

	// E.C. Direction
//#define  F_Ph     0x70
//#define  F_PhA    0x10
//#define  F_PhB    0x20
//#define  F_PhC    0x40

//#define  F_Ph     0x10	
//#define  F_PhA    0x01
//#define  F_PhB    0x02
//#define  F_PhC    0x04

//#define  F_Dir    0x03
//#define  F_PDir	0x02
//#define  F_QDir	0x01

//#define  F_TamTmr    0xC0

	// TamFlg Register flags define
#define  F_TamStop   0x01
#define  F_TamDrop   0x02
#define  F_TamResv   0x04
#define  F_TamPole   0x08
#define  F_TamPass   0x10
#define  F_TamUout   0x20
#define  F_TamLoad   0x40
#define  F_TamSequ   0x80

	// ErrFlg Register flags define
	
#define  F_AlarmCoverOpen	0x01		//开盖报警//#define  F_AlarmPhFail	0x01		//断相报警
#define  F_AlarmTCoverOpen	0x02		//开盖报警//#define  F_AlarmAllCuFail	0x02		//全断流报警
#define  F_AlarmMagnet	0x04		//磁场报警//#define  F_AlarmMeas	0x04		//计量芯片出错
#define  F_AlarmCRCFail	0x08		//CRC校验错误
#define  F_AlarmPhs 	0x10		//逆相序报警 
#define  F_AlarmReverse	0x20		//功率反向报警
#define  F_AlarmClock	0x40		// 32768Hz Crystal Error
#define  F_AlarmE2P   	0x80

//#define  F_ErrMeas		0x01		//计量芯片出错
#define  F_ErrBatLow   	0x02
#define  F_ErrDir   	0x04
#define  F_ErrPhs 		0x08		// 
#define  F_ErrBatOver	0x10		// 
#define  F_WatchDog		0x20
//#define  F_ErrClock		0x40		// 32768Hz Crystal Error
//#define  F_ErrE2P   	0x80

	// AlarmFlg[0] Register flags define
#define  F_AlarmPhFail	0x01		//失压报警
#define  F_AlarmCuFail	0x02		//失流报警
#define  F_AlarmMeas	0x04		//计量芯片出错
#define  F_AlarmBatLow  0x08		//电池低电压报警
#define  F_AlarmPhs 	0x10		//逆相序报警 
//#define  F_AlarmNLine 	0x20	//掉零线报警 
#define  F_AlarmRevPower 0x20		//功率反向报警 
#define  F_AlarmClock	0x40		//DS3231
//#define  F_AlarmByPass 	0x40	//电流旁路报警 
#define  F_AlarmE2P   	0x80

	// AlarmFlg[1] Register flags define
#define  F_AlarmPhBreak	0x01		//断相报警
#define  F_AlarmPhLower	0x02		//欠压报警
#define  F_AlarmPhOver	0x04		//过压报警
#define  F_AlarmCuBreak 0x08		//断流报警
#define  F_AlarmCuOver 	0x10		//过流报警 
#define  F_AlarmCuPhs 	0x20		//电流逆相序报警	//10.06.02 
#define  F_AlarmMDOver	0x40		//需量超限报警
#define  F_AlarmCover  	0x80		//开表盖报警

	// AlarmFlg[2] Register flags define
#define  F_AlarmFlash		0x01	//FLASH报警
#define  F_AlarmXT1			0x02	//XT1晶振报警		//V4
#define  F_AlarmCuBestIm	0x04	//电流严重不平衡
#define  F_AlarmCuImbalance	0x08	//电流不平衡
#define  F_AlarmPWOver		0x10	//过载
#define  F_AlarmCommBatLow	0x20	//停抄电池电压低
#define  F_AlarmTerminal  	0x40	//开端钮盖报警
#define  F_AlarmVoltImbalance	0x80//电压不平衡

	// AlarmFlg[3] Register flags define
#define  F_AlarmRemoteTrip	0x80	//远程拉闸
#define  F_AlarmRemote		0x40	//远程报警	
#define  F_AlarmBuyEC		0x20	//剩余金额小于报警金额1
#define  F_AlarmRelayOFF	0x10	//继电器断开

#define  F_AlarmPftOver		0x01	//功率因素超下限
#define  F_AlarmFRAM	   	0x08	//铁电故障

	// AlarmFlg[4] Register flags define
#define  F_AlarmRelayErr	0x01	//控制回路错误		//山东	//新国网	//13.10.08
#define  F_AlarmESAMErr		0x02	//ESAM错误			//山东	//新国网	//13.10.08
#define  F_AlarmAllFail		0x10	//全失压报警		//上海	//新国网	//13.09.21
#define  F_AlarmPhRevPower 	0x20	//分相功率反向报警  //湖北  //新国网	//13.09.21

	// AlarmFlg[5] Register flags define
#define  F_AlarmEEPROM   	0x01	//EEPROM故障	 	//15.01.30

	// AlarmFlg[6] Register flags define
	// AlarmFlg[7] Register flags define

	// FirstFlg Register flags define
//#define  F_PhAFail	0x01		//第一次A相断相未记录
//#define  F_PhBFail	0x02		//第一次B相断相未记录
//#define  F_PhCFail	0x04		//第一次C相断相未记录
//#define  F_CuAFail	0x08		//第一次A相断流未记录
//#define  F_CuBFail	0x10		//第一次A相断流未记录
//#define  F_CuCFail	0x20		//第一次A相断流未记录
//#define  F_PhFail	0x40		//第一次断相未记录			//V1000

/*
#define  F_ErrRAM    0x01
#define  F_ErrMeas   0x02		//计量芯片出错
#define  F_ErrE2P    0x04
#define  F_ErrDFls   0x08
#define  F_ErrCry1 	0x10		// 32768Hz Crystal Error
#define  F_ErrCry2 	0x20		// 8MHz Crystal Error
#define  F_WatchDog	0x40
#define  F_ErrANY    0x80
*/
//LoadFlag Register flags define   
#define  F_LoadCom	0x01		//通讯模式3中下一次分块数据发送为日负荷数据
#define  F_TampCom	0x02  		//通讯模式3中下一次分块数据发送为窃电记录数据
   
//BatState Register flags define   
//#define  F_ClockBat	0x01		//时钟电池状态  0:欠压；1：正常 
//#define  F_CommBat	0x02		//停抄电池状态   
#define	 F_E2PCheck	0x04		//读三次E2PROM是否正确。0：正确，1：三次校验和均错。	//EC	

/*   
#if ( CD4051 == YesCheck )
//4051的输入口状态   
#define		P_BATCH		0x01	//V2 0:非AD采样状态；1：AD采样状态；
#define		P_BATCHB	0x02	//V2
#define		P_LOWPOW	0x04	//V2
#define		P_PRGRM		0x08	//V2
#define		P_KEYSF		0x10	//V2
#define		P_KEYXF		0x20	//V2
#define		P_SETEN 	0x40	//V2
#define		P_VDD 		0x80	//V2
//4067的输入口状态   
#elif ( CD4067 == YesCheck )
#define		P_VASY		0x0001	//A相电压 	 
#define		P_VBSY		0x0002	//B相电压 	 
#define		P_VCSY		0x0004	//C相电压 	 
#define		P_IAAD		0x0008	//A相电流 	 
#define		P_IBAD		0x0010	//B相电流 	 
#define		P_ICAD		0x0020	//C相电流 	 
#define		P_BATCH		0x0040	//V2 0:非AD采样状态；1：AD采样状态；
#define		P_BATCHB	0x0080	//V2
#define		P_KEYD		0x0100	//V2
#define		P_KEYG		0x0200	//V2
#define		P_LOWPOW	0x0400	//V2
#define		P_PRGRM		0x0800	//V2
#define		P_KEYSF		0x1000	//V2
#define		P_KEYXF		0x2000	//V2
#define		P_SETEN 	0x4000	//V2
#define		P_VDD 		0x8000	//V2
#else
#endif   
*/

//MeterState1 Register flags define   
#define 	F_MDJSMode	0x0002		//需量积算方式	0：滑差、1：区间	   
#define 	F_ClockBat	0x0004		//时钟电池		0：正常、1：欠压	   
#define 	F_CommBat	0x0008		//停抄电池		0：正常、1：欠压	   
#define 	F_PDir		0x0010		//有功功率方向	0：正向、1：反向	   
#define 	F_QDir		0x0020		//无功功率方向	0：正向、1：反向	   

#define 	F_ESAMErr	0x0200		//ESAM状态		0：正常、1：故障

#define 	F_RelayErr		0x0100		//控制回路错误	0：正常、1：故障		//新国网
#define 	F_ProgramErr	0x1000		//内部程序错误	0：正常、1：故障		//新国网
#define 	F_MemErr		0x2000		//存储器故障	0：正常、1：故障		//新国网
#define 	F_Overdraft		0x4000		//透支状态		0：正常、1：故障		//新国网
#define 	F_ClkErr		0x8000		//时钟故障		0：正常、1：故障		//新国网

   
//MeterState2 Register flags define   
#define 	F_PDirA		0x0001		//A相有功功率方向	0：正向、1：反向	   
#define 	F_PDirB		0x0002		//B相有功功率方向	0：正向、1：反向	   
#define 	F_PDirC		0x0004		//C相有功功率方向	0：正向、1：反向	   
#define 	F_QDirA		0x0010		//A相无功功率方向	0：正向、1：反向	   
#define 	F_QDirB		0x0020		//B相无功功率方向	0：正向、1：反向	   
#define 	F_QDirC		0x0040		//C相无功功率方向	0：正向、1：反向	   

//MeterState3 Register flags define   
//#define 	F_Zone		0x0001		//当前运行时段	0：第一套、1：第二套	   
#define 	F_Seg		0x0001		//当前运行时段	0：第一套、1：第二套	   
#define 	F_MainPW	0x0000		//主电源
#define 	F_AuxPW		0x0002		//辅助电源
#define 	F_BatPW		0x0004		//电池电源
#define 	F_PrgAllow	0x0008		//编程允许		0：禁止、1：许可
#define 	F_RelayOpen	0x0010		//继电器状态	0：通、	 1：断
#define 	F_Zone		0x0020		//当前运行时区	0：第一套、1：第二套	//参照天津  
#define 	F_RelayOdState	0x0040	//继电器命令状态 0：通、 1：断
#define 	F_PreTrip	0x0080		//预跳闸报警状态 0：无、 1：有
#define 	F_EC		0x0100		//电量型预付费表
#define 	F_Cash		0x0200		//电费型预付费表
//#define 	F_Fee		0x0400		//当前运行分时费率	0：第一套、1：第二套	//新国网保留   
//#define 	F_Step		0x0800		//当前运行阶梯	0：第一套、1：第二套		//新国网保留   

#define 	F_Safe			0x1000	//保电状态									//新国网
#define 	F_IdentValid	0x2000	//身份认证状态								//新国网
#define 	F_LocalOpen		0x4000	//本地开户									//新国网
#define 	F_RemoteOpen	0x8000	//远程开户									//新国网	

//MeterState4、5、6 Register flags define //A、B、C相故障状态   
#define 	F_PhFail	0x0001		//失压
#define 	F_PhLower	0x0002		//欠压
#define 	F_PhOver	0x0004		//过压
#define 	F_CuFail	0x0008		//失流
#define 	F_CuOver	0x0010		//过流
#define 	F_PWOver	0x0020		//过载
#define 	F_PWRev		0x0040		//潮流反向
#define 	F_PhBreak	0x0080		//断相
#define 	F_CuBreak	0x0100		//断流

//MeterState7 Register flags define //合相故障状态 
#define 	F_VoltRevPhase	0x0001	//电压逆相序
#define 	F_CuRevPhase	0x0002	//电流逆相序
#define 	F_VoltImbalance	0x0004	//电压不平衡
#define 	F_CuImbalance	0x0008	//电流不平衡
#define 	F_AuxPWFail		0x0010	//辅助电源失电
#define 	F_Pdown			0x0020	//掉电
#define 	F_MDOver		0x0040	//需量超限
#define 	F_PftOver		0x0080	//功率因素超下限
#define 	F_CuBestImbalance	0x0100	//电流严重不平衡

#define 	F_Cover			0x0200	//开表盖									//新国网	
#define 	F_Terminal		0x0400	//开端钮盖									//新国网	

//PrgState Register flags define //编程记录状态 
#define 	F_PrgRec		0x0001		//编程记录
#define 	F_SegPrg		0x0002		//时段编程记录
#define 	F_ZonePrg		0x0004		//时区编程记录
#define 	F_HolidayPrg	0x0008		//节假日编程记录
#define 	F_SaveDayPrg	0x0010		//结算日编程记录
#define 	F_RemotePrgRec	0x0020		//远程编程记录(通过安全认证置位)

//EventOutState Register flags define 	//事件报警输出状态 
#define 	F_CoverOpen		0x0001		//开表盖

//KeyStateFlg Register flags define 	//按键状态 			//11.07.19
#define 	F_KeySFlag		0x01		//上翻键
#define 	F_KeyXFlag		0x02		//下翻键
#define 	F_KeyPFlag		0x04		//编程键
#define 	F_KeyCFlag		0x08		//开盖键
#define 	F_KeyTFlag		0x10		//开端键

#define 	F_KeyRFlag		0x20		//0：合闸 1:拉闸 监测内置继电器状态	//新国网	//13.09.27

typedef struct
{
  unsigned char 	Power;
  unsigned char 	Run;
  unsigned char	Clk;
  unsigned char	Disk;
  unsigned char	Error;
  unsigned char 	BatState;
  unsigned char AlarmFlg[4];
} FLAG;



typedef struct
{
  unsigned char PowerUpTime;
  unsigned short TestDisCnt;
  unsigned short BatUpTime;
  unsigned short TestCnt;
  unsigned long BatteryWTime;
  unsigned long ClockBatAD;  
} SRAM;

//总及分相电压检测记录结构
typedef struct
{
	unsigned long CheckTime;		//电压检测时间(4)
	unsigned short NormalRate;		//电压合格率(2)
	unsigned short AbnormalRate;	//电压超限率(2)
	unsigned long UpperLimitTime;	//电压超上限时间(4)
	unsigned long LowerLimitTime;	//电压超下限时间(4)
}VOLT_CHECK_REC;					//16字节

typedef struct
{
//	VOLT_CHECK_REC	MonVoltChk_Rec;	//本月电压合格率统计数据(16)
	VOLT_CHECK_REC	MonVoltChk_RecA;//本月A相电压合格率统计数据(16)
	VOLT_CHECK_REC	MonVoltChk_RecB;//本月B相电压合格率统计数据(16)
	VOLT_CHECK_REC	MonVoltChk_RecC;//本月C相电压合格率统计数据(16)

//	VOLT_CHECK_REC	DayVoltChk_Rec;	//当日电压合格率统计数据(16)
	VOLT_CHECK_REC	DayVoltChk_RecA;//当日A相电压合格率统计数据(16)
	VOLT_CHECK_REC	DayVoltChk_RecB;//当日B相电压合格率统计数据(16)
	VOLT_CHECK_REC	DayVoltChk_RecC;//当日C相电压合格率统计数据(16)

//	unsigned long SumVolt;			//合相1分钟内电压累加值
	unsigned long SumVoltA;			//A相1分钟内电压累加值
	unsigned long SumVoltB;			//B相1分钟内电压累加值
	unsigned long SumVoltC;			//C相1分钟内电压累加值

//	unsigned short AverageVolt;		//合相1分钟内电压平均值
	unsigned short AverageVoltA;	//A相1分钟内电压平均值
	unsigned short AverageVoltB;	//B相1分钟内电压平均值
	unsigned short AverageVoltC;	//C相1分钟内电压平均值

//	unsigned char VoltSecCnt;		//1分钟内电压检测秒数
	unsigned char VoltSecCntA;		//1分钟内A相电压检测秒数
	unsigned char VoltSecCntB;		//1分钟内B相电压检测秒数
	unsigned char VoltSecCntC;		//1分钟内C相电压检测秒数

}VOLTPARA;	

typedef struct
{
	VOLT_CHECK_REC*	MonVoltChk;
	VOLT_CHECK_REC*	DayVoltChk;
	unsigned long* SumV;
	unsigned short* AverVolt;
	unsigned char* SecCnt;
	unsigned short CurMonAddr;
	unsigned short CurDayAddr;
}VOLTCHECKPARA;


/*
typedef struct
{
	unsigned short ECBufAds;
	unsigned short ECBufAdsPtr;
	unsigned short ECRgAds;
	unsigned char* ECRamBufAds;
} ECRg;
*/
//#if ( MEASCHIP == IDT90E32 )
//typedef struct
//{
//	unsigned short ECEAds;
//	unsigned short ECFAds;
//	unsigned char* ECRAds;
//	unsigned short* ECRamBufAds;
//	unsigned char* ECRamBufChkAds;
//	unsigned char* SparePLRamBufAds;		//0.1CF以下剩余脉冲保留
//	unsigned char* SparePLRamBufChkAds;		//0.1CF以下剩余脉冲保留校验
//	unsigned short ECRgMode;		//0:电量包含费率；1：不包含费率。
//} ECRg;
//#else
typedef struct
{
	unsigned short ECEAds;
	unsigned short ECFAds;
	unsigned char* ECRAds;
//	unsigned short* ECRamBufAds;
//	unsigned char* ECRamBufChkAds;
	unsigned short ECRgMode;		//0:电量包含费率；1：不包含费率。
} ECRg;
//#endif

typedef struct
{
	unsigned short StartAds;		//区间起始地址
	unsigned short EndAds;		//区间结束地址
	unsigned short ECFAds;		//电量FRAM首地址
	unsigned char* ECRAds;		//电量RAM首地址
	unsigned char* Mode;		//组合模式字	
	unsigned short ECRgMode;		//0:电量包含费率；1：不包含费率。

}COMBECPARA;					//组合电量参数 		


typedef struct
{
	short DevAds;
	short NextPage;
	unsigned char BakNum;
	unsigned char CorrectMode;
} E2Para;

/*
typedef struct
{
	//short (*p)(void);
	unsigned char (*p)(void);
	unsigned char* Cunt;	
	unsigned char* State;		// 
	unsigned char* OldState;	
}TAMPER;	
*/

//typedef struct						//N645
//{
//	unsigned short PtAddr;
//	unsigned short StartAddr;
//	unsigned short RecdCunt;
//	unsigned short RecdLenth;	
//} NPTPARA;							//645


#if( ZjProtocol	== YesCheck )		//有浙江省通讯规约
// 	ZjState					// 浙江省通讯规约处理过程状态
#define zjIdle			0	// 通讯空闲
#define zjBufBusy		1	// 开始收数据，通讯缓存忙
#define zjRead			2	// 收完数据
#define zjDataSend1Wait	3	// 数据包1发送等待
#define zjDataSend2		4	// 数据包2发送
#define zjDataSend2Wait	5	// 数据包2发送等待
#define zjDataSend3		6	// 数据包3发送
#define zjDataSend3Wait	7	// 数据包3发送等待
#define zjDataSend4		8	// 数据包4发送
#define zjDataSend4Wait	9	// 数据包4发送等待
#define zjExit			10	// 通讯退出
#else
#endif
/*
#if( N645Protocol	== YesCheck )	//有645-2007通讯规约		//N645
// 	NState					// 通讯规约处理过程状态				//N645
#define NIdle			0	// 通讯空闲							//N645	
#define NBufBusy		1	// 开始收数据，通讯缓存忙			//N645
#define NRead			2	// 收完数据							//N645		
#define NDataSend1Wait	3	// 数据包1发送等待					//N645
#define NDataSend2		4	// 数据包2发送						//N645	
#define NExit			5	// 通讯退出							//N645

#define NEW645PRO		0	//645-2007							//...
#define OLD645PRO		1	//645-1997
#define ZJPRO			2	//浙江省规约

#endif															//N645	
*/

	// Run flags define
#define  FComOK0   0x01		//串口0操作完毕标志(发送或接收)
#define  FComOK1   0x02		//串口1操作完毕标志(发送或接收)
#define  FComOK2   0x04		//串口2操作完毕标志(发送或接收)
#define  FComOK3   0x08		//串口3操作完毕标志(发送或接收)
/*
typedef struct
{
	unsigned short	CommIntReserve1;	//保留整型空间1
	unsigned short	CommIntReserve2;	//保留整型空间2
	unsigned short	CommIntReserve3;	//保留整型空间3
	unsigned short	CommIntReserve4;	//保留整型空间4

	unsigned short   BatOnDly;			// 停抄电池供电时间延时

	unsigned short	HWYKRecL;			// 红外遥控接收低字节					//V30
	unsigned short	HWYKRecH;			// 红外遥控接收高字节(低字节的反码)		//V30
	unsigned short 	RecCount;			// 端口接收脉冲数			
	unsigned short 	BTime0;				// 红外口字符间定时
	unsigned short 	BTime1;				// 485口1字符间定时
	unsigned short 	BTime2;				// 485口2字符间定时
	unsigned short 	BTime3;				// 485口3字符间定时
	unsigned short	BandRateTime1;		//485口1波特率维持定时 2S		
	unsigned short	BandRateTime2;		//485口2波特率维持定时 2S			
	unsigned short	BandRateTime3;		//485口3波特率维持定时 2S			

  	unsigned char 	Buf0[220];			// 红外口通信缓存		
  	unsigned char 	Buf1[220];			// 485口1通信缓存		
  	unsigned char 	Buf2[220];			// 485口2通信缓存		
  	unsigned char 	Buf3[220];			// 485口3通信缓存		
  	unsigned char 	Mode0;				// 红外口通讯模式
  	unsigned char 	Mode1;				// 485口1通讯模式
  	unsigned char 	Mode2;				// 485口2通讯模式
  	unsigned char 	Mode3;				// 485口3通讯模式
  	unsigned char 	Ptr0;				// 红外口缓存器指针
  	unsigned char 	Ptr1;				// 485口1缓存器指针
  	unsigned char 	Ptr2;				// 485口2缓存器指针
  	unsigned char 	Ptr3;				// 485口3缓存器指针

	unsigned char	SendDTime0;			// 发送延时
	unsigned char	SendDTime1;			// 发送延时
	unsigned char	SendDTime2;			// 发送延时
	unsigned char	SendDTime3;			// 发送延时
	unsigned char 	TX_Delay0;			// 发送延时1
	unsigned char 	TX_Delay1;			// 发送延时1
	unsigned char 	TX_Delay2;			// 发送延时2
	unsigned char 	TX_Delay3;			// 发送延时1
	unsigned char 	TX_FESum0;			// 发送FE数量1
	unsigned char 	TX_FESum1;			// 发送FE数量2
	unsigned char 	TX_FESum2;			// 发送FE数量3
	unsigned char 	TX_FESum3;			// 发送FE数量4

	unsigned char	Run;				//运行标志

    //	unsigned char   T485DLY;            // 485状态保持时间
	unsigned char	RxTx_Data;			// 485口1软件接收发送字节
	signed char	BitCount;					// 485口1软件接收发送字节记数

	unsigned char 	BandRate1;	//485口2波特率
	unsigned char 	BandRate2;	//485口3波特率
	unsigned char 	BandRate3;	//485口4波特率
	unsigned char 	BandRateMode;	//波特率模式 0：可设；1：自适应   

//	unsigned char   BatOnDly;			// 停抄电池供电时间延时
	unsigned char   CPAOnDly;			// 停电期间比较器打开延时	
	
	unsigned char 	RecTmr0;			// 接收记录延时
	unsigned char 	RecTmr1;			// 接收记录延时
	unsigned char 	RecTmr2;			// 接收记录延时
	unsigned char 	RecTmr3;			// 接收记录延时
	unsigned char 	PrgTmr;				// 编程记录延时
	
	unsigned char	EdgeState;			// 红外遥控接收端电平状态				//V30
	unsigned char 	RecBitCnt;			// 红外遥控接收BIT记数					//V30
	unsigned char   BitWidthCnt;		// 红外遥控接收单BIT时长(以250us为单位)	//V30
	unsigned char   RecByte;			// 红外遥控接收到的字节					//V30
	unsigned char   RecByte1;			// 红外遥控接收到的字节					//V30
	unsigned char   RecByte2;			// 红外遥控接收到的字节					//V30
	unsigned char   RecByte3;			// 红外遥控接收到的字节					//V30
	unsigned char   RecByte4;			// 红外遥控接收到的字节					//HW
	unsigned char   RecByteCnt;			// 红外遥控接收到的字节记数				//V30	
	unsigned char   RecTimeDelay;		// 红外遥控接收到的字节间延时			//V30

#if( ZjProtocol	== YesCheck )		//有浙江省通讯规约
	unsigned char	ZjState1;		//1口浙江省通讯规约处理过程状态
	unsigned char	ZjState2;		//2口浙江省通讯规约处理过程状态
	unsigned char	ZjLength1;		//1口浙规发送长度
	unsigned char	ZjLength2;		//2口浙规发送长度
	unsigned char	ZjCheck1;		//1口浙规发送校验和
	unsigned char	ZjCheck2;		//2口浙规发送校验和
	unsigned char	ZjBTime1Rec;	//1口接收字符间隔定时 超过表示一帧结束
	unsigned char	ZjBTime2Rec;	//2口接收字符间隔定时 超过表示一帧结束
	unsigned char	ZjState3;		//3口浙规通讯规约处理过程状态
	unsigned char	ZjLength3;		//3口浙规发送长度
	unsigned char	ZjCheck3;		//3口浙规发送校验和
	unsigned char	ZjBTime3Rec;	//3口接收字符间隔定时 超过表示一帧结束
#else
#endif

	BCD4			OperateCode;	//操作者代码
	BCD4			TripONOperateCode;	//合闸操作者代码
	BCD4			TripOFFOperateCode;	//跳闸操作者代码

	unsigned char 	BatComBuff[3];	//停抄唤醒缓存区				//新国网

	BCD8			CommReserve;	//保留8字节空间

} COMM;
*/

//ICComm.Power flags define
#define	 F_FmqbjEn		0x08		//非F_ECAlarm1状态下蜂鸣器报警使能。0：报警正常；1：报警关闭			//V8
#define	 F_ECAlarm1FMQ	0x80		//F_ECAlarm1状态下 0:正常报警状态 1：插过用户卡或在快速测试状态下插过检查卡，不再声报警	 //V8

//ICComm.Run定义
#define  F_ICComOK1   	0x04
#define  F_ICParity1  	0x08
#define  F_ICComOK2   	0x10
#define  F_ICParity2   	0x20

//ICMeterState Register flags define   
#define  F_RELAY	0x01		//继电器状态
#define  F_EEPROM	0x02		//EEPROM状态
#define  F_BAT		0x04		//电池状态
#define  F_CLOCK	0x08		//时钟状态
#define  F_REVEC	0x10		//反向计量状态
#define  F_DRELAY	0x80		//显示用继电器状态		出故障后插检查卡，消除错误代码，其余不变。//V1000

//ICECState Register flags define   
#define F_ECNormal		0x00	//剩余电量大于报警电量1；
#define F_ECAlarm1		0x01	//剩余电量大于报警电量2，小于等于报警电量1；
#define F_ECAlarm20		0x02	//剩余电量大于0，小于等于报警电量2，					//还没有插过用户卡(跳闸报警)；
#define F_ECAlarm21		0x03	//剩余电量大于0，小于等于报警电量2，插过用户卡(恢复用电)；
#define F_ECOver		0x04	//剩余电量等于0，透支电量等于0；是否跳闸根据透支电量限额决定。
#define F_ECOverdraft	0x05	//透支；
#define F_ECOverEnd		0x06	//透支电量用完；		//V7
#define F_ECExceed		0x07	//超过透支电费限额；	//参照天津	//11.06.27
#define F_ECEnd			0x08	//剩余电量等于0，透支电量等于0；透支电量限额等于0。			//11.08.19

#define F_NULL			0x55	//空(用于继电器卡)							//V7
  
//ICOutState Register flags define   
#define F_ECRelay		0x01	//用电继电器开关状态 0: 合闸；1：跳闸
#define F_AlarmRelay	0x02	//报警继电器开关状态 0: 合闸；1：跳闸
#define F_ECRelayLED	0x04	//用电继电器开关状态指示灯 0: 熄灭；1：点亮
#define F_AlarmRelayLED	0x08	//报警继电器开关状态指示灯 0: 熄灭；1：点亮
  
//ICInState Register flags define   
#define F_BuyEC			0x01	//用户卡插卡状态 0: 未插入过；1：插过		//用于判断从小于等于报警2到透支电量前用电继电器是否跳闸
#define F_RegECLimt		0x02	//剩余电量状态 0: 剩余电量小于囤积电量限额；1：剩余电量小于囤积电量限额	//用于剩余电量闪烁显示
#define F_BuyECOK		0x04	//用户卡买电状态 0: 失败；1：成功		//用于判断从插用户卡是否要显示购电量		//V7
#define F_BuyECOver		0x08	//透支用户卡插卡状态 0: 未插入过；1：插过		//用于判断从小于等于0到透支电量前用电继电器是否跳闸		//V7
#define F_CheckEC		0x10	//检查卡插卡状态 0: 未插入过；1：插过		//用于判断从小于等于报警2到透支电量前用电继电器是否跳闸(快速测试卡状态)	//V7
#define F_CheckECOver	0x20	//检查卡插卡状态 0: 未插入过；1：插过		//用于判断从小于等于0到透支电量前用电继电器是否跳闸(快速测试卡状态)		//V7

#define F_Alarm2RelayONPermit	0x40	//用于判断从小于等于报警2到透支电量前用电继电器跳闸后插卡，允许合闸，
										//不受合闸允许金额限制,不保存在EEPROM中	//11.04.23

typedef struct
{
	unsigned long	TripOFFDelay;		// 跳闸延时时间
#if ( DLT645_2007_14 ==	YesCheck )	//新国网	//14.04.25
	unsigned long   PreTripOFFKeepTime; //远程预跳闸持续时间(单位：秒)		//新国网	//14.04.29
#endif
	unsigned long	LongReserve1;		// 保留					//新国网	//14.02.12

	enum {
		ICDisIdle,						// 正常轮显或按显
		ICDisCardReading,				// 显示"--------"
		ICDisBfRemainEC,				// 显示电量叠加前剩余电量
		ICDisRemainEC,					// 显示电量叠加后剩余电量
		ICDisMeter_No1,					// 显示表号高4位
		ICDisMeter_No2,					// 显示表号低6位
		ICDisErr,						// 显示错误号
		ICDisExit,						// 正常退出
	} ICDisState;						// 插卡后显示状态

	unsigned short	ControlComExeState;	// 控制命令执行状态
	unsigned short	ControlComErrState;	// 控制命令错误状态

//	unsigned short	TripOFFDelay;		// 跳闸延时时间
	unsigned short 	ICBTime1;			// IC口1字符间定时
	unsigned short 	ICBTime2;			// IC口1字符间定时
  	unsigned short 	ICPtr1;				// IC口1缓存器指针
  	unsigned short 	ICPtr2;				// IC口1缓存器指针
	unsigned short 	ICSendLength1;		// IC口1软件接收发送字节数
	unsigned short 	ICSendLength2;	    // IC口2软件接收发送字节数
	unsigned short 	ICRecLength1;		// IC口1软件接收字节数
	unsigned short 	ICRecLength2;	    // IC口2软件接收字节数
#if ( LocalSmartMeter == YesCheck )
	unsigned short	InsertCardState;	// 插卡状态字					//新国网	//13.12.05
#endif

	unsigned short 	RelayDrivePulseWidth;	// 磁保持继电器驱动脉冲宽度	150毫秒		//新国网	//14.02.12
	unsigned short	IntReserve1;		// 保留							//新国网	//14.02.12

  	unsigned char 	ICBuf1[330];		// PSAM口通信缓存
  	unsigned char 	ICBuf2[330];		// ESAM口通信缓存
	unsigned char	ICRxTx_Data1;		// IC口1软件接收发送字节
	signed char		ICBitCount1;		// IC口1软件接收发送字节记数
	unsigned char	ICRxTx_Data2;		// IC口2软件接收发送字节
	signed char		ICBitCount2;		// IC口2软件接收发送字节记数
	
	unsigned char	Power;				//
	unsigned char	Run;				// 运行状态
	unsigned char	ICMeterState;		//
	unsigned char	ICECState;			//
	unsigned char	ICOutState;			//
	unsigned char	ICInState;			//
	
	unsigned char	ICAlarmFlg;			// 报警标志字
	unsigned char	ICOperateState;		// IC卡插卡过程状态字 00：插卡正常结束，0x01：卡插入，0x02:正常流程结束，0x03:提前拔卡
	unsigned char	ICDisDelay;			// IC卡插卡显示项秒延时
	unsigned char	ICRDStateDelay;		// IC卡插卡是否成功显示秒延时
	
	unsigned char 	RPrivateKeyFlag;	// 私钥状态：00，公钥状态：非零
	unsigned char	ByteReCnt;			// 字节重复次数
	unsigned char	StrReCnt;			// 字符串重复次数	

	unsigned char 	CardType;			//卡类型,显示用
	unsigned char 	RelayErrDelay;		//继电器跳闸后延时。跳闸10秒后若还有脉冲采集到，则提示继电器故障。
	unsigned char 	SoundAlarmDelay;	//剩余电量小于报警电量1后，每采集到一个电量脉冲，报警3秒。
	
	unsigned char 	Alarm2State;		//剩余电量在0到报警电量2之间的状态 0：不在该区间，1：在该区间，继电器跳闸(跳闸报警)，
										//2：在该区间插入用户卡后恢复供电。
	unsigned char 	RelayTestDelay;		//继电器测试卡测试延时

	unsigned char   ModifyKeyClrDly;	//密钥修改卡在上电10分钟后不再能清除电量。
	
//#if ( MagnKeepRelay	== YesCheck )
	unsigned char 	RelayDriveType;		//磁保持继电器驱动类型	0x00：闭合，0x01：断开， 其余不动作。 
	unsigned char 	RelayDriveCnt;		//磁保持继电器驱动次数	3次
//	unsigned char 	RelayDrivePulseWidth;	//磁保持继电器驱动脉冲宽度	150毫秒
	unsigned char 	CharReserve1;		//保留							//新国网	//14.02.12
	unsigned char 	RelayONState;		//负荷开关执行合闸状态初始化	0：不执行	0xAA:允许接收合闸指令(按键或插卡) 0x55:执行
	unsigned char 	RelayOFFState;		//负荷开关执行跳闸闸状态初始化		0：不执行	0x55:执行
	unsigned char 	CardRelayDelay;		//继电器卡插入后有效时间延时。	//在该延时时间内执行继电器卡动作，延时到后执行正常的判断跳合闸流程

	unsigned char	ErrorMessage;		//错误信息字			//新国网	//13.12.02
	unsigned char	OperateCommHead[5];	//操作命令头			//新国网	//13.12.02	
	unsigned char	ErrorResponse[2];	//错误响应状态			//新国网	//13.12.02
	unsigned char	InsertCardECCnt[2];	//插卡时总购电次数		//新国网	//13.12.02	
	unsigned char	InsertCardRemainEC[4];	//插卡时剩余金额	//新国网	//13.12.02

	unsigned char	ICDisAlarmFlg;		//显示报警标志字		//新国网	//13.12.20
	
//#else
//#endif		
	unsigned char	ParaUpdateFlag;		//参数更新标志位
	unsigned char 	YearMonSaveMode;	//年月结算模式	0x00:月结算	其他：年结算											//新国网	//13.11.29
	unsigned char	RFeePriceNum;		//费率电价数	0：费率电价为全0	其他：有效费率电价								//新国网	//13.11.30
	unsigned char 	CardFaultFMQDelay;	//读卡失败（错误）三声连续短鸣		长鸣为625ms，短鸣为125ms，短鸣间隔也为125ms		//新国网	//13.12.05
	unsigned char	ROpenAccountFlag;	//开户成功标志   Bit0: 0:本地开户 1：本地未开户  Bit1: 0:远程开户 1：远程未开户  	//新国网	//13.12.05   
	unsigned char	RJudgePermitFlag;	//需判断合闸允许金额标志(2)	0x55:需判断，其他：不判断							  	//新国网	//13.12.10   

//	BCD8			IccommReserve;		//保留8字节空间
	unsigned char 	RDisPrivateKeyFlag;	//显示用私钥状态：00，公钥状态：非零		//12.08.04
	BCD7			IccommReserve;		//保留8字节空间
	
//#else
//#endif
	unsigned char   SystemTitle[8];     //系统名：由逻辑设备名转化而来--8字节固定..
	unsigned char   ClientSystemTitle[8];     //客户机系统名：由AARQ传给逻辑转化而来--8字节不固定..
	unsigned char	CommErrTime[4]; 	//通讯口错误次数(时间相对于锁定时间相差超过2个小时解锁)..
	unsigned char	SHEZHIErrTime;		//设置错误次数..

}ICCOMM;

typedef struct
{
	unsigned long	RRemainEC;				//剩余金额		
	unsigned long	ROverdraftEC;			//透支金额		
	unsigned long	RPermitTripONLimt;		//合闸允许金额		
//	unsigned long 	ECStep[10];
	unsigned long 	ECStep[6];				//新国网
	unsigned short 	IdentityValidDelay;		//身份认证有效时间				== 0 无效 分钟数
	unsigned short 	InfraredValidDelay;		//红外认证有效时间				== 0 无效 以秒计数		//新国网
	BCD8	RConsumECSumBCD;				//16进制累计用电金额(8)	HEX		XXXXXXXXXX.XXXXXX
	BCD4	Random1;						//随机数1(主站下发)		
	BCD4	Random2;						//随机数2(ESAM上传)		
	BCD8	DisperseGene;					//远程分散因子(表号+高两字节0)
	BCD8	LocalDisperseGene;				//本地分散因子(卡号或表号+高两字节0)
	
	unsigned char 	IdentityValidDelayChk;	//身份认证有效时间校验和		== 0 无效 分钟数
	unsigned char 	InfraredValidDelayChk;	//红外认证有效时间校验和		== 0 无效 以秒计数		//新国网
	unsigned char 	RConsumECSumBCDChk;		//10进制累计用电金额校验和	

	unsigned char 	DecDelay;				//15分钟ESAM扣钱延时
	unsigned char 	RemoteDecDelay;			//远程15分钟ESAM扣钱延时

	unsigned char   IdentityMACFaultCnt;	//MAC失败次数
	unsigned char   IdentityFaultCnt;		//身份认证失败次数
	unsigned char   IdentityMACFaultCntChk;	//MAC失败次数
	unsigned char   IdentityFaultCntChk;	//身份认证失败次数
	unsigned char   IdentityState;			//身份认证挂起状态
	unsigned char	ECStepChk;				//阶梯电量校验和

	unsigned char	InfraredRequestValid;	//用于红外认证后等待红外认证指令期间，ESAM保持有电，红外认证请求命令通过后置0x01;		//新国网
											//红外认证指令通过后、上下电后、身份认证后清零;	

	unsigned char use_DK;		//判断是否带DK..
	unsigned char use_HLSname;	//1--不采用；3--采用加密..
	unsigned char use_cipher_information; //使用加密用户信息传输..	
	unsigned char use_SHA256;	//0--不采用，1--采用..
	unsigned char d_key[16];	//16字节的dedicated key..
	unsigned char BAOLIU[16];

}RPESAM;

typedef struct
{
	unsigned char* TabPtr;	//帧数据项指针
	unsigned char Type;		//帧命令码		
	unsigned char Len;		//帧总长度		
	unsigned char ItemCount;//帧数据相数
}FramePARA;


typedef struct
{
	unsigned short 	DisID;
	unsigned short 	ComID;
	unsigned short 	Addr;
}COMDISTAB;

typedef struct
{
	unsigned short 	ComID;
	unsigned short 	Addr;
	unsigned short	State;
}CDPARATAB;

typedef struct
{
	unsigned short 	ComID;
	unsigned long 	Addr;
  	unsigned short 	ComInfo;
}COMTAB;

typedef struct					//N645
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
}NCOMTAB;						//N645
/*
typedef struct
{
	unsigned short 	DisID;
	unsigned short 	Addr;
	unsigned char 	Length;
	unsigned char	State1;
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
}DISTAB;
*/
typedef struct
{
	unsigned short 	ComID;
	unsigned char 	*Addr;
  	unsigned short 	ComInfo;
}BATCOMTAB;
/*
#if( N645Protocol	== YesCheck )	//有645-2007通讯规约		//N645
typedef struct
{
	unsigned char 	*Buf;			// 通讯缓存器
	unsigned char 	*Mode;			// 通讯模式
  	unsigned char 	*Ptr;			// 缓存器指针
//  	unsigned char 	*BTime;			// 字符间定时
  	unsigned short 	*BTime;			// 字符间定时
  	unsigned char 	*SendDTime;		// 发送延时
  	unsigned char 	*NState;		// 通讯状态
  	unsigned char 	*NLength;		// 发送长度
}COMMPARA;
#else
*/
/*
typedef struct
{
	unsigned char 	*Buf;			// 通讯缓存器
	unsigned char 	*Mode;			// 通讯模式
  	unsigned char 	*Ptr;			// 缓存器指针
//  	unsigned char 	*BTime;			// 字符间定时
  	unsigned short 	*BTime;			// 字符间定时
  	unsigned char 	*SendDTime;		// 发送延时
  	unsigned char 	*RecTmr;		// 通讯延时
  	unsigned char 	*FESum;			// 通讯FE数量
}COMMPARA;
*/
typedef struct
{
  unsigned char*	Buf;			// 通讯缓存器
  unsigned char*	Mode;			// 通讯模式
  unsigned char*	SendDTime;		// 发送延时
  unsigned char*	RecTmr; 		// 通讯延时
  unsigned short*	  Ptr;			// 缓存器指针
  unsigned short*		BTime;			// 字符间定时
  
	unsigned char*	IECState;		// 通讯口IEC通讯过程状态
	unsigned char*	MACState;		// 通讯口MAC通讯过程状态
	unsigned char*	LLCState;		// 通讯口MAC通讯过程状态
	unsigned char*	TX_Delay;		// 发送延时
	unsigned char*	Run;			// 485口运行标志
	unsigned char*	BandRate;		// 485口波特率
	unsigned char*	FTime;			// 帧间定时
//	unsigned char*	RxdCnt; 		// 接收帧计数器
//	unsigned char*	SendCnt;		// 发送帧计数器
	unsigned char*	RxdFrameNo; 	// 接收帧计数器
	unsigned char*	TxdFrameNo; 	// 发送帧计数器
	unsigned char*	LastRecCommand; // 最后收到的命令
	unsigned char*	APPLState;		// 通讯口APPL连接状态

	unsigned long*	BlockNumber;	// 串口GET块通讯块号
	unsigned short*	HBTime; 		// 485口字符间定时
    unsigned short*	RecDataPtr; 	// 485口缓存器指针
    unsigned short*	SendDataPtr;	// 485口缓存器指针
    unsigned short*	SendByteSum;	// 485口LLC发送总数
    unsigned short*	  SetBlockPtr;	  // 块设置缓存指针

    unsigned char*	RecDataBuf; 	// 485口接收通信缓存		
    unsigned char*	SendDataBuf;	// 485口发送通信缓存		
    unsigned char*	HMode;			// 485口通讯模式
    unsigned char*	HState; 		// 485口通信状态
	unsigned char*	HBandRate;		// 485口波特率
	unsigned char*	HRecTmr;		// 接收记录延时
	unsigned char*	HPrgTmr;		// 编程记录延时
	unsigned char*	Conformance;	// 通讯口应用连接的一致性块参数
	unsigned char*	SubsBlockPara;	// 串口后续帧块参数
	unsigned char*	SetBlockBuff;	// 块设置缓存首址
	//unsigned char reserv1;
	//unsigned char reserv2;
}COMMPARA;

//#endif														//N645

#if( ZjProtocol	== YesCheck )		// 有浙江通讯规约
typedef struct
{
	unsigned char 	*Buf;			// 通讯缓存器
	unsigned char 	*Mode;			// 部规约通讯模式
  	unsigned char 	*Ptr;			// 缓存器指针
  	unsigned short 	*BTime;			// 字符间定时
  	unsigned char 	*SendDTime;		// 发送延时
	unsigned char	*ZjState;		// 浙江规约通讯状态
	unsigned char	*ZjLength;		// 浙江规约每包数据发送长度
	unsigned char	*ZjCheck;		// 浙江规约每包数据校验和
}ZJCOMMPARA;
#else
#endif

/*									//PWMD	
typedef struct
{
	unsigned short 	MDBufAds;		// 需量缓存区首址
	unsigned short 	*MDCum;			// 1分钟累计脉冲数
}MDPARA;
*/
/*								
typedef struct	
{
	unsigned short 	MDEAds;
	unsigned short* 	MDBufAds;		// 需量缓存区首址
	unsigned long* 	MDCum;			// 1分钟累计功率
	unsigned short 	MDRgMode;		//0:需量包含费率；1：不包含费率。
}MDPARA;
*/
typedef struct
{
//	unsigned short	MDBufAds;		// 需量缓存区首址
//	unsigned short	*MDCum; 		// 1分钟累计脉冲数
	unsigned long		*MDCum; 		// 1分钟累计脉冲数	
	
}MDPARA;

typedef struct
{
	unsigned short 	MDEAds;
	unsigned short* 	MDBufAds;		// 需量缓存区首址
	unsigned char* 	Mode;			//组合模式字	
	unsigned short 	MDRgMode;		//0:电量包含费率；1：不包含费率。

}COMBMDPARA;						//组合电量参数 		


typedef struct
{
	unsigned long* 	SampleSum;		// n个周期内采样点数的累加和
	unsigned char* 	Irms;			// 相电流有效值
}IRMSPARA;

typedef struct
{
	unsigned short 	ECAddr;			//季节冻结首址
  	unsigned short 	EnterDay;		//进入季节日期
	unsigned short	OutDay;			//退出季节日期
	unsigned char*  ZoneNo;			//当前时区号
}SEASONPARA;

#if( SecurityCheck == YesCheck )				
typedef struct
{
	unsigned short 	CuLow;			//电流下限
  	unsigned short 	CuHigh;			//电流上限
	unsigned short	CosLow;			//功率因数上限
	unsigned short	CosHigh;		//功率因数下限
}SECURITYPARA;
#else
#endif

#if ( MEASCHIP == ADE7758 )
typedef struct
{
	unsigned char 	ADERgAddr;		//7758寄存器地址
  	unsigned char 	Length;			//寄存器字节长度
}ADEPara;

typedef struct
{
	unsigned char*  RamRgAddr;		//瞬时值RAM地址		
	unsigned char 	ATTRgAddr;		//ATT7022寄存器地址
  	unsigned char 	Length;			//寄存器字节长度
}RamPara;

typedef struct
{
	short CommID;		//通讯编码
	short Length;		//块长度
  	short Cycle;		//块数量
}ADESwapPara;
#else
#endif

#if ( ZjProtocol == YesCheck)
typedef struct
{
	unsigned short 	ComID;
	unsigned char* 	Addr;
  	unsigned short 	ComInfo;
}ZJCOMTAB;
#else
#endif




//A、B、C相失压、欠压、过压、断相、失流、过流、断流、潮流反向、过载总次数及总累计时间结构
typedef struct
{
	BCD3	ATimes;					//A相总次数
	BCD3	ATime;					//A相总累计时间
	BCD3	BTimes;					//B相总次数
	BCD3	BTime;					//B相总累计时间
	BCD3	CTimes;					//C相总次数
	BCD3	CTime;					//C相总累计时间

}PHASE_EVENT_TIMES;				//18字节


//全失压、辅助电源失电、电压、电流逆相序、电压、电流不平衡总次数及总累计时间结构
typedef struct
{
	BCD3	Times;					//总次数
	BCD3	Time;					//总累计时间

}EVENT_TIMES;					//6字节

/*
//A、B、C相失压、欠压、过压、断相记录结构
typedef struct
{
	BCD3	StaTime;		//异常起始时间(3)
	BCD3	StaDate;		//异常起始日期(3)
	BCD3	EndTime;		//异常结束时间(3)
	BCD3	EndDate;		//异常结束日期(3)
	BCD4	AmongECPp0;				//期间有功正向总电量增量(4)
	BCD4	AmongECPn0;				//期间有功反向总电量增量(4)
	BCD4	AmongECQp0;				//期间组合无功1总电量增量(4)
	BCD4	AmongECQn0;				//期间组合无功2总电量增量(4)
	BCD4	AmongECPpA;				//期间A相有功正向总电量增量(4)
	BCD4	AmongECPnA;				//期间A相有功反向总电量增量(4)
	BCD4	AmongECQpA;				//期间A相组合无功1总电量增量(4)
	BCD4	AmongECQnA;				//期间A相组合无功2总电量增量(4)
	BCD2	Ua;						//异常发生时刻A相电压(2)
	BCD3	Ia;						//异常发生时刻A相电流(3)
	BCD3	Pa;						//异常发生时刻A相有功功率(3)
	BCD3	Qa;						//异常发生时刻A相无功功率(3)
	BCD2	Pfa;					//异常发生时刻A相功率因素(2)
	BCD4	AmongECPpB;				//期间B相有功正向总电量增量(4)
	BCD4	AmongECPnB;				//期间B相有功反向总电量增量(4)
	BCD4	AmongECQpB;				//期间B相组合无功1总电量增量(4)
	BCD4	AmongECQnB;				//期间B相组合无功2总电量增量(4)
	BCD2	Ub;						//异常发生时刻B相电压(2)
	BCD3	Ib;						//异常发生时刻B相电流(3)
	BCD3	Pb;						//异常发生时刻B相有功功率(3)
	BCD3	Qb;						//异常发生时刻B相无功功率(3)
	BCD2	Pfb;					//异常发生时刻B相功率因素(2)
	BCD4	AmongECPpC;				//期间C相有功正向总电量增量(4)
	BCD4	AmongECPnC;				//期间C相有功反向总电量增量(4)
	BCD4	AmongECQpC;				//期间C相组合无功1总电量增量(4)
	BCD4	AmongECQnC;				//期间C相组合无功2总电量增量(4)
	BCD2	Uc;						//异常发生时刻C相电压(2)
	BCD3	Ic;						//异常发生时刻C相电流(3)
	BCD3	Pc;						//异常发生时刻C相有功功率(3)
	BCD3	Qc;						//异常发生时刻C相无功功率(3)
	BCD2	Pfc;					//异常发生时刻C相功率因素(2)
	BCD4	AmongAh;				//异常发生期间总安时数(4)
	BCD4	AmongAhA;				//异常发生期间A相安时数(4)
	BCD4	AmongAhB;				//异常发生期间B相安时数(4)
	BCD4	AmongAhC;				//异常发生期间C相安时数(4)

}VOLT_REC;					//131字节
*/
/*
//A、B、C相失压、欠压、过压、断相记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	EndECPpA;				//结束时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	EndECPnA;				//结束时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	EndECQpA;				//结束时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD4	EndECQnA;				//结束时刻A相组合无功2总电量(4)
	BCD2	Ua;						//异常发生时刻A相电压(2)
	BCD3	Ia;						//异常发生时刻A相电流(3)
	BCD3	Pa;						//异常发生时刻A相有功功率(3)
	BCD3	Qa;						//异常发生时刻A相无功功率(3)
	BCD2	Pfa;					//异常发生时刻A相功率因素(2)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	EndECPpB;				//结束时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	EndECPnB;				//结束时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	EndECQpB;				//结束时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD4	EndECQnB;				//结束时刻B相组合无功2总电量(4)
	BCD2	Ub;						//异常发生时刻B相电压(2)
	BCD3	Ib;						//异常发生时刻B相电流(3)
	BCD3	Pb;						//异常发生时刻B相有功功率(3)
	BCD3	Qb;						//异常发生时刻B相无功功率(3)
	BCD2	Pfb;					//异常发生时刻B相功率因素(2)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	EndECPpC;				//结束时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	EndECPnC;				//结束时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	EndECQpC;				//结束时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)
	BCD4	EndECQnC;				//结束时刻C相组合无功2总电量(4)
	BCD2	Uc;						//异常发生时刻C相电压(2)
	BCD3	Ic;						//异常发生时刻C相电流(3)
	BCD3	Pc;						//异常发生时刻C相有功功率(3)
	BCD3	Qc;						//异常发生时刻C相无功功率(3)
	BCD2	Pfc;					//异常发生时刻C相功率因素(2)
	BCD4	AmongAh;				//异常发生期间总安时数(4)
	BCD4	AmongAhA;				//异常发生期间A相安时数(4)
	BCD4	AmongAhB;				//异常发生期间B相安时数(4)
	BCD4	AmongAhC;				//异常发生期间C相安时数(4)

}VOLT_REC;					//195字节
*/

//A、B、C相失压、欠压、过压、断相记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD2	Ua;						//异常发生时刻A相电压(2)
	BCD3	Ia;						//异常发生时刻A相电流(3)
	BCD3	Pa;						//异常发生时刻A相有功功率(3)
	BCD3	Qa;						//异常发生时刻A相无功功率(3)
	BCD2	Pfa;					//异常发生时刻A相功率因素(2)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD2	Ub;						//异常发生时刻B相电压(2)
	BCD3	Ib;						//异常发生时刻B相电流(3)
	BCD3	Pb;						//异常发生时刻B相有功功率(3)
	BCD3	Qb;						//异常发生时刻B相无功功率(3)
	BCD2	Pfb;					//异常发生时刻B相功率因素(2)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)
	BCD2	Uc;						//异常发生时刻C相电压(2)
	BCD3	Ic;						//异常发生时刻C相电流(3)
	BCD3	Pc;						//异常发生时刻C相有功功率(3)
	BCD3	Qc;						//异常发生时刻C相无功功率(3)
	BCD2	Pfc;					//异常发生时刻C相功率因素(2)
	BCD4	AmongAh;				//异常发生期间总安时数(4)
	BCD4	AmongAhA;				//异常发生期间A相安时数(4)
	BCD4	AmongAhB;				//异常发生期间B相安时数(4)
	BCD4	AmongAhC;				//异常发生期间C相安时数(4)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)
	BCD4	EndECPpA;				//结束时刻A相有功正向总电量(4)
	BCD4	EndECPnA;				//结束时刻A相有功反向总电量(4)
	BCD4	EndECQpA;				//结束时刻A相组合无功1总电量(4)
	BCD4	EndECQnA;				//结束时刻A相组合无功2总电量(4)
	BCD4	EndECPpB;				//结束时刻B相有功正向总电量(4)
	BCD4	EndECPnB;				//结束时刻B相有功反向总电量(4)
	BCD4	EndECQpB;				//结束时刻B相组合无功1总电量(4)
	BCD4	EndECQnB;				//结束时刻B相组合无功2总电量(4)
	BCD4	EndECPpC;				//结束时刻C相有功正向总电量(4)
	BCD4	EndECPnC;				//结束时刻C相有功反向总电量(4)
	BCD4	EndECQpC;				//结束时刻C相组合无功1总电量(4)
	BCD4	EndECQnC;				//结束时刻C相组合无功2总电量(4)

}VOLT_REC;					//195字节


//全失压记录结构
typedef struct
{
	BCD3	StaTime;		//异常起始时间(3)
	BCD3	StaDate;		//异常起始日期(3)
	BCD3	Amper;			//全失压电流值(3)
	BCD3	EndTime;		//异常结束时间(3)
	BCD3	EndDate;		//异常结束日期(3)

}ALLFAIL_REC;				//15字节

//辅助电源失电、掉电记录结构
typedef struct
{
	BCD3	StaTime;		//异常起始时间(3)
	BCD3	StaDate;		//异常起始日期(3)
	BCD3	EndTime;		//异常结束时间(3)
	BCD3	EndDate;		//异常结束日期(3)

}POWER_REC;					//12字节	
//}ASSISTPW_REC;				//12字节

//掉电记录结构
//typedef struct
//{
//	BCD3	StaTime;		//异常起始时间(3)
//	BCD3	StaDate;		//异常起始日期(3)
//	BCD3	EndTime;		//异常结束时间(3)
//	BCD3	EndDate;		//异常结束日期(3)

//} POWER_DOWN_REC;				//20字节

/*
//电压、电流不平衡记录结构
typedef struct
{
	BCD3	StaTime;		//异常起始时间(3)
	BCD3	StaDate;		//异常起始日期(3)
	BCD3	EndTime;		//异常结束时间(3)
	BCD3	EndDate;		//异常结束日期(3)
	BCD2	ImbalanceRate;			//不平衡率(2)
	BCD4	AmongECPp0;				//期间有功正向总电量增量(4)
	BCD4	AmongECPn0;				//期间有功反向总电量增量(4)
	BCD4	AmongECQp0;				//期间组合无功1总电量增量(4)
	BCD4	AmongECQn0;				//期间组合无功2总电量增量(4)
	BCD4	AmongECPpA;				//期间A相有功正向总电量增量(4)
	BCD4	AmongECPnA;				//期间A相有功反向总电量增量(4)
	BCD4	AmongECQpA;				//期间A相组合无功1总电量增量(4)
	BCD4	AmongECQnA;				//期间A相组合无功2总电量增量(4)
	BCD4	AmongECPpB;				//期间B相有功正向总电量增量(4)
	BCD4	AmongECPnB;				//期间B相有功反向总电量增量(4)
	BCD4	AmongECQpB;				//期间B相组合无功1总电量增量(4)
	BCD4	AmongECQnB;				//期间B相组合无功2总电量增量(4)
	BCD4	AmongECPpC;				//期间C相有功正向总电量增量(4)
	BCD4	AmongECPnC;				//期间C相有功反向总电量增量(4)
	BCD4	AmongECQpC;				//期间C相组合无功1总电量增量(4)
	BCD4	AmongECQnC;				//期间C相组合无功2总电量增量(4)

}IMBALANCE_REC;				//78字节
*/
/*
//电压、电流不平衡记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD3	ImbalanceRate;			//不平衡率(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	EndECPpA;				//结束时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	EndECPnA;				//结束时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	EndECQpA;				//结束时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD4	EndECQnA;				//结束时刻A相组合无功2总电量(4)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	EndECPpB;				//结束时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	EndECPnB;				//结束时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	EndECQpB;				//结束时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD4	EndECQnB;				//结束时刻B相组合无功2总电量(4)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	EndECPpC;				//结束时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	EndECPnC;				//结束时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	EndECQpC;				//结束时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)
	BCD4	EndECQnC;				//结束时刻C相组合无功2总电量(4)

}IMBALANCE_REC;				//143字节
*/

//电压、电流不平衡记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)
	BCD3	ImbalanceRate;			//不平衡率(3)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)
	BCD4	EndECPpA;				//结束时刻A相有功正向总电量(4)
	BCD4	EndECPnA;				//结束时刻A相有功反向总电量(4)
	BCD4	EndECQpA;				//结束时刻A相组合无功1总电量(4)
	BCD4	EndECQnA;				//结束时刻A相组合无功2总电量(4)
	BCD4	EndECPpB;				//结束时刻B相有功正向总电量(4)
	BCD4	EndECPnB;				//结束时刻B相有功反向总电量(4)
	BCD4	EndECQpB;				//结束时刻B相组合无功1总电量(4)
	BCD4	EndECQnB;				//结束时刻B相组合无功2总电量(4)
	BCD4	EndECPpC;				//结束时刻C相有功正向总电量(4)
	BCD4	EndECPnC;				//结束时刻C相有功反向总电量(4)
	BCD4	EndECQpC;				//结束时刻C相组合无功1总电量(4)
	BCD4	EndECQnC;				//结束时刻C相组合无功2总电量(4)

}IMBALANCE_REC;				//143字节


/*
//A、B、C相失流、过流、断流记录结构
typedef struct
{
	BCD3	StaTime;		//异常起始时间(3)
	BCD3	StaDate;		//异常起始日期(3)
	BCD3	EndTime;		//异常结束时间(3)
	BCD3	EndDate;		//异常结束日期(3)
	BCD4	AmongECPp0;		//期间有功正向总电量增量(4)
	BCD4	AmongECPn0;		//期间有功反向总电量增量(4)
	BCD4	AmongECQp0;		//期间组合无功1总电量增量(4)
	BCD4	AmongECQn0;		//期间组合无功2总电量增量(4)
	BCD4	AmongECPpA;		//期间A相有功正向总电量增量(4)
	BCD4	AmongECPnA;		//期间A相有功反向总电量增量(4)
	BCD4	AmongECQpA;		//期间A相组合无功1总电量增量(4)
	BCD4	AmongECQnA;		//期间A相组合无功2总电量增量(4)
	BCD2	Ua;				//异常发生时刻A相电压(2)
	BCD3	Ia;				//异常发生时刻A相电流(3)
	BCD3	Pa;				//异常发生时刻A相有功功率(3)
	BCD3	Qa;				//异常发生时刻A相无功功率(3)
	BCD2	Pfa;			//异常发生时刻A相功率因素(2)
	BCD4	AmongECPpB;		//期间B相有功正向总电量增量(4)
	BCD4	AmongECPnB;		//期间B相有功反向总电量增量(4)
	BCD4	AmongECQpB;		//期间B相组合无功1总电量增量(4)
	BCD4	AmongECQnB;		//期间B相组合无功2总电量增量(4)
	BCD2	Ub;				//异常发生时刻B相电压(2)
	BCD3	Ib;				//异常发生时刻B相电流(3)
	BCD3	Pb;				//异常发生时刻B相有功功率(3)
	BCD3	Qb;				//异常发生时刻B相无功功率(3)
	BCD2	Pfb;			//异常发生时刻B相功率因素(2)
	BCD4	AmongECPpC;		//期间C相有功正向总电量增量(4)
	BCD4	AmongECPnC;		//期间C相有功反向总电量增量(4)
	BCD4	AmongECQpC;		//期间C相组合无功1总电量增量(4)
	BCD4	AmongECQnC;		//期间C相组合无功2总电量增量(4)
	BCD2	Uc;				//异常发生时刻C相电压(2)
	BCD3	Ic;				//异常发生时刻C相电流(3)
	BCD3	Pc;				//异常发生时刻C相有功功率(3)
	BCD3	Qc;				//异常发生时刻C相无功功率(3)
	BCD2	Pfc;			//异常发生时刻C相功率因素(2)

}CURRENT_REC;				//115字节
*/
/*
//A、B、C相失流、过流、断流记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	EndECPpA;				//结束时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	EndECPnA;				//结束时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	EndECQpA;				//结束时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD4	EndECQnA;				//结束时刻A相组合无功2总电量(4)
	BCD2	Ua;						//异常发生时刻A相电压(2)
	BCD3	Ia;						//异常发生时刻A相电流(3)
	BCD3	Pa;						//异常发生时刻A相有功功率(3)
	BCD3	Qa;						//异常发生时刻A相无功功率(3)
	BCD2	Pfa;					//异常发生时刻A相功率因素(2)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	EndECPpB;				//结束时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	EndECPnB;				//结束时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	EndECQpB;				//结束时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD4	EndECQnB;				//结束时刻B相组合无功2总电量(4)
	BCD2	Ub;						//异常发生时刻B相电压(2)
	BCD3	Ib;						//异常发生时刻B相电流(3)
	BCD3	Pb;						//异常发生时刻B相有功功率(3)
	BCD3	Qb;						//异常发生时刻B相无功功率(3)
	BCD2	Pfb;					//异常发生时刻B相功率因素(2)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	EndECPpC;				//结束时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	EndECPnC;				//结束时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	EndECQpC;				//结束时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)
	BCD4	EndECQnC;				//结束时刻C相组合无功2总电量(4)
	BCD2	Uc;						//异常发生时刻C相电压(2)
	BCD3	Ic;						//异常发生时刻C相电流(3)
	BCD3	Pc;						//异常发生时刻C相有功功率(3)
	BCD3	Qc;						//异常发生时刻C相无功功率(3)
	BCD2	Pfc;					//异常发生时刻C相功率因素(2)

}CURRENT_REC;					//179字节
*/

//A、B、C相失流、过流、断流记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD2	Ua;						//异常发生时刻A相电压(2)
	BCD3	Ia;						//异常发生时刻A相电流(3)
	BCD3	Pa;						//异常发生时刻A相有功功率(3)
	BCD3	Qa;						//异常发生时刻A相无功功率(3)
	BCD2	Pfa;					//异常发生时刻A相功率因素(2)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD2	Ub;						//异常发生时刻B相电压(2)
	BCD3	Ib;						//异常发生时刻B相电流(3)
	BCD3	Pb;						//异常发生时刻B相有功功率(3)
	BCD3	Qb;						//异常发生时刻B相无功功率(3)
	BCD2	Pfb;					//异常发生时刻B相功率因素(2)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)
	BCD2	Uc;						//异常发生时刻C相电压(2)
	BCD3	Ic;						//异常发生时刻C相电流(3)
	BCD3	Pc;						//异常发生时刻C相有功功率(3)
	BCD3	Qc;						//异常发生时刻C相无功功率(3)
	BCD2	Pfc;					//异常发生时刻C相功率因素(2)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)
	BCD4	EndECPpA;				//结束时刻A相有功正向总电量(4)
	BCD4	EndECPnA;				//结束时刻A相有功反向总电量(4)
	BCD4	EndECQpA;				//结束时刻A相组合无功1总电量(4)
	BCD4	EndECQnA;				//结束时刻A相组合无功2总电量(4)
	BCD4	EndECPpB;				//结束时刻B相有功正向总电量(4)
	BCD4	EndECPnB;				//结束时刻B相有功反向总电量(4)
	BCD4	EndECQpB;				//结束时刻B相组合无功1总电量(4)
	BCD4	EndECQnB;				//结束时刻B相组合无功2总电量(4)
	BCD4	EndECPpC;				//结束时刻C相有功正向总电量(4)
	BCD4	EndECPnC;				//结束时刻C相有功反向总电量(4)
	BCD4	EndECQpC;				//结束时刻C相组合无功1总电量(4)
	BCD4	EndECQnC;				//结束时刻C相组合无功2总电量(4)

}CURRENT_REC;					//179字节

/*
//电压、电流逆相序；A、B、C相潮流反向、过载记录结构
typedef struct
{
	BCD3	StaTime;		//异常起始时间(3)
	BCD3	StaDate;		//异常起始日期(3)
	BCD3	EndTime;		//异常结束时间(3)
	BCD3	EndDate;		//异常结束日期(3)
	BCD4	AmongECPp0;				//期间有功正向总电量增量(4)
	BCD4	AmongECPn0;				//期间有功反向总电量增量(4)
	BCD4	AmongECQp0;				//期间组合无功1总电量增量(4)
	BCD4	AmongECQn0;				//期间组合无功2总电量增量(4)
	BCD4	AmongECPpA;				//期间A相有功正向总电量增量(4)
	BCD4	AmongECPnA;				//期间A相有功反向总电量增量(4)
	BCD4	AmongECQpA;				//期间A相组合无功1总电量增量(4)
	BCD4	AmongECQnA;				//期间A相组合无功2总电量增量(4)
	BCD4	AmongECPpB;				//期间B相有功正向总电量增量(4)
	BCD4	AmongECPnB;				//期间B相有功反向总电量增量(4)
	BCD4	AmongECQpB;				//期间B相组合无功1总电量增量(4)
	BCD4	AmongECQnB;				//期间B相组合无功2总电量增量(4)
	BCD4	AmongECPpC;				//期间C相有功正向总电量增量(4)
	BCD4	AmongECPnC;				//期间C相有功反向总电量增量(4)
	BCD4	AmongECQpC;				//期间C相组合无功1总电量增量(4)
	BCD4	AmongECQnC;				//期间C相组合无功2总电量增量(4)

}PHASE_POWER_REC;					//76字节		
*/

/*
//电压、电流逆相序；A、B、C相潮流反向、过载记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	EndECPpA;				//结束时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	EndECPnA;				//结束时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	EndECQpA;				//结束时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD4	EndECQnA;				//结束时刻A相组合无功2总电量(4)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	EndECPpB;				//结束时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	EndECPnB;				//结束时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	EndECQpB;				//结束时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD4	EndECQnB;				//结束时刻B相组合无功2总电量(4)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	EndECPpC;				//结束时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	EndECPnC;				//结束时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	EndECQpC;				//结束时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)
	BCD4	EndECQnC;				//结束时刻C相组合无功2总电量(4)

}PHASE_POWER_REC;				//140字节
*/

//电压、电流逆相序；A、B、C相潮流反向、过载记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)
	BCD4	EndECPpA;				//结束时刻A相有功正向总电量(4)
	BCD4	EndECPnA;				//结束时刻A相有功反向总电量(4)
	BCD4	EndECQpA;				//结束时刻A相组合无功1总电量(4)
	BCD4	EndECQnA;				//结束时刻A相组合无功2总电量(4)
	BCD4	EndECPpB;				//结束时刻B相有功正向总电量(4)
	BCD4	EndECPnB;				//结束时刻B相有功反向总电量(4)
	BCD4	EndECQpB;				//结束时刻B相组合无功1总电量(4)
	BCD4	EndECQnB;				//结束时刻B相组合无功2总电量(4)
	BCD4	EndECPpC;				//结束时刻C相有功正向总电量(4)
	BCD4	EndECPnC;				//结束时刻C相有功反向总电量(4)
	BCD4	EndECQpC;				//结束时刻C相组合无功1总电量(4)
	BCD4	EndECQnC;				//结束时刻C相组合无功2总电量(4)

}PHASE_POWER_REC;				//140字节


/*
//总及分相电压检测记录结构
typedef struct
{
	BCD3	CheckTime;				//电压检测时间(3)
	BCD3	NormalRate;				//电压合格率(3)
	BCD3	AbnormalRate;			//电压超限率(3)
	BCD3	UpperLimitTime;			//电压超上限时间(3)
	BCD3	LowerLimitTime;			//电压超下限时间(3)
	BCD2	MaxVolt;				//最高电压(2)
	BCD4	MaxVoltHpTime;			//最高电压出现时间(4)
	BCD2	LeastVolt;				//最低电压(2)
	BCD4	LeastVoltHpTime;		//最低电压出现时间(4)

}VOLT_CHECK_REC;				//27字节
*/

//需量超限记录结构
typedef struct
{
	BCD3	StaTime;		//异常起始时间(3)
	BCD3	StaDate;		//异常起始日期(3)
	BCD3	EndTime;		//异常结束时间(3)
	BCD3	EndDate;		//异常结束日期(3)
	BCD3	MaxDemand;				//异常期间最大需量(3)
	BCD5	MaxDemandHpTime;		//异常期间最大需量发生时间(5)

} MD_OVER_REC;				//20字节


//编程记录结构
typedef struct
{
	BCD3	HappenTime;				//编程发生时间(3)
	BCD3	HappenDate;				//编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD4	ID01;					//数据标识码1(4)
	BCD4	ID2;					//数据标识码2(4)
	BCD4	ID3;					//数据标识码3(4)
	BCD4	ID4;					//数据标识码4(4)
	BCD4	ID5;					//数据标识码5(4)
	BCD4	ID6;					//数据标识码6(4)
	BCD4	ID7;					//数据标识码7(4)
	BCD4	ID8;					//数据标识码8(4)
	BCD4	ID9;					//数据标识码9(4)
	BCD4	ID10;					//数据标识码10(4)

}PRG_REC;					//50字节	


//电量清零记录结构
typedef struct
{
	BCD3	HappenTime;				//电量清零发生时间(3)
	BCD3	HappenDate;				//电量清零发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD4	ECPp0;					//电量清零前正向有功总电量(4)
	BCD4	ECPn0;					//电量清零前反向有功总电量(4)
	BCD4	ECQ10;					//电量清零前1象限无功总电量(4)
	BCD4	ECQ20;					//电量清零前2象限无功总电量(4)
	BCD4	ECQ30;					//电量清零前3象限无功总电量(4)
	BCD4	ECQ40;					//电量清零前4象限无功总电量(4)
	BCD4	ECPpA0;					//电量清零前A相正向有功总电量(4)
	BCD4	ECPnA0;					//电量清零前A相反向有功总电量(4)
	BCD4	ECQ1A0;					//电量清零前A相1象限无功总电量(4)
	BCD4	ECQ2A0;					//电量清零前A相2象限无功总电量(4)
	BCD4	ECQ3A0;					//电量清零前A相3象限无功总电量(4)
	BCD4	ECQ4A0;					//电量清零前A相4象限无功总电量(4)
	BCD4	ECPpB0;					//电量清零前B相正向有功总电量(4)
	BCD4	ECPnB0;					//电量清零前B相反向有功总电量(4)
	BCD4	ECQ1B0;					//电量清零前B相1象限无功总电量(4)
	BCD4	ECQ2B0;					//电量清零前B相2象限无功总电量(4)
	BCD4	ECQ3B0;					//电量清零前B相3象限无功总电量(4)
	BCD4	ECQ4B0;					//电量清零前B相4象限无功总电量(4)
	BCD4	ECPpC0;					//电量清零前C相正向有功总电量(4)
	BCD4	ECPnC0;					//电量清零前C相反向有功总电量(4)
	BCD4	ECQ1C0;					//电量清零前C相1象限无功总电量(4)
	BCD4	ECQ2C0;					//电量清零前C相2象限无功总电量(4)
	BCD4	ECQ3C0;					//电量清零前C相3象限无功总电量(4)
	BCD4	ECQ4C0;					//电量清零前C相4象限无功总电量(4)

}EC_CLR_REC;				//106字节


//需量清零记录结构
typedef struct
{
	BCD3	HappenTime;				//电量清零发生时间(3)
	BCD3	HappenDate;				//电量清零发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD3	MDPp0;					//需量清零前正向有功总最大需量(3)
	BCD5	MDTPp0;					//需量清零前正向有功总最大需量发生时间(5)
	BCD3	MDPn0;					//需量清零前反向有功总最大需量(3)
	BCD5	MDTPn0;					//需量清零前反向有功总最大需量发生时间(5)
	BCD3	MDQ10;					//需量清零前1象限无功总最大需量(3)
	BCD5	MDTQ10;					//需量清零前1象限无功总最大需量发生时间(5)
	BCD3	MDQ20;					//需量清零前2象限无功总最大需量(3)
	BCD5	MDTQ20;					//需量清零前2象限无功总最大需量发生时间(5)
	BCD3	MDQ30;					//需量清零前3象限无功总最大需量(3)
	BCD5	MDTQ30;					//需量清零前3象限无功总最大需量发生时间(5)
	BCD3	MDQ40;					//需量清零前4象限无功总最大需量(3)
	BCD5	MDTQ40;					//需量清零前4象限无功总最大需量发生时间(5)

	BCD3	MDPpA0;					//需量清零前A相正向有功总最大需量(3)
	BCD5	MDTPpA0;				//需量清零前A相正向有功总最大需量发生时间(5)
	BCD3	MDPnA0;					//需量清零前A相反向有功总最大需量(3)
	BCD5	MDTPnA0;				//需量清零前A相反向有功总最大需量发生时间(5)
	BCD3	MDQ1A0;					//需量清零前A相1象限无功总最大需量(3)
	BCD5	MDTQ1A0;				//需量清零前A相1象限无功总最大需量发生时间(5)
	BCD3	MDQ2A0;					//需量清零前A相2象限无功总最大需量(3)
	BCD5	MDTQ2A0;				//需量清零前A相2象限无功总最大需量发生时间(5)
	BCD3	MDQ3A0;					//需量清零前A相3象限无功总最大需量(3)
	BCD5	MDTQ3A0;				//需量清零前A相3象限无功总最大需量发生时间(5)
	BCD3	MDQ4A0;					//需量清零前A相4象限无功总最大需量(3)
	BCD5	MDTQ4A0;				//需量清零前A相4象限无功总最大需量发生时间(5)

	BCD3	MDPpB0;					//需量清零前B相正向有功总最大需量(3)
	BCD5	MDTPpB0;				//需量清零前B相正向有功总最大需量发生时间(5)
	BCD3	MDPnB0;					//需量清零前B相反向有功总最大需量(3)
	BCD5	MDTPnB0;				//需量清零前B相反向有功总最大需量发生时间(5)
	BCD3	MDQ1B0;					//需量清零前B相1象限无功总最大需量(3)
	BCD5	MDTQ1B0;				//需量清零前B相1象限无功总最大需量发生时间(5)
	BCD3	MDQ2B0;					//需量清零前B相2象限无功总最大需量(3)
	BCD5	MDTQ2B0;				//需量清零前B相2象限无功总最大需量发生时间(5)
	BCD3	MDQ3B0;					//需量清零前B相3象限无功总最大需量(3)
	BCD5	MDTQ3B0;				//需量清零前B相3象限无功总最大需量发生时间(5)
	BCD3	MDQ4B0;					//需量清零前B相4象限无功总最大需量(3)
	BCD5	MDTQ4B0;				//需量清零前B相4象限无功总最大需量发生时间(5)

	BCD3	MDPpC0;					//需量清零前C相正向有功总最大需量(3)
	BCD5	MDTPpC0;				//需量清零前C相正向有功总最大需量发生时间(5)
	BCD3	MDPnC0;					//需量清零前C相反向有功总最大需量(3)
	BCD5	MDTPnC0;				//需量清零前C相反向有功总最大需量发生时间(5)
	BCD3	MDQ1C0;					//需量清零前C相1象限无功总最大需量(3)
	BCD5	MDTQ1C0;				//需量清零前C相1象限无功总最大需量发生时间(5)
	BCD3	MDQ2C0;					//需量清零前C相2象限无功总最大需量(3)
	BCD5	MDTQ2C0;				//需量清零前C相2象限无功总最大需量发生时间(5)
	BCD3	MDQ3C0;					//需量清零前C相3象限无功总最大需量(3)
	BCD5	MDTQ3C0;				//需量清零前C相3象限无功总最大需量发生时间(5)
	BCD3	MDQ4C0;					//需量清零前C相4象限无功总最大需量(3)
	BCD5	MDTQ4C0;				//需量清零前C相4象限无功总最大需量发生时间(5)

}MD_CLR_REC;				//202字节


//事件清零记录结构
typedef struct
{
	BCD3	HappenTime;				//事件清零发生时间(3)
	BCD3	HappenDate;				//事件清零发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD4	EventID;				//事件清零数据标识码1(4)
}EVENT_CLR_REC;			//14字节

//校时清零记录结构
typedef struct
{
	BCD4	OperatorCode;			//操作者代码(4)
	BCD3	BeforeTime;				//校时前时间(3)
	BCD3	BeforeDate;				//校时前日期(3)
	BCD3	AfterTime;				//校时后时间(3)
	BCD3	AfterDate;				//校时后日期(3)

}TIME_REC;			//14字节


//时段表编程记录结构
typedef struct
{
	BCD3	HappenTime;				//时段表编程发生时间(3)
	BCD3	HappenDate;				//时段表编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD3	Period1Tab1[14];		//时段表编程前第一套第1日时段表数据(3*14)
	BCD3	Period1Tab2[14];		//时段表编程前第一套第2日时段表数据(3*14)
	BCD3	Period1Tab3[14];		//时段表编程前第一套第3日时段表数据(3*14)
	BCD3	Period1Tab4[14];		//时段表编程前第一套第4日时段表数据(3*14)
	BCD3	Period1Tab5[14];		//时段表编程前第一套第5日时段表数据(3*14)
	BCD3	Period1Tab6[14];		//时段表编程前第一套第6日时段表数据(3*14)
	BCD3	Period1Tab7[14];		//时段表编程前第一套第7日时段表数据(3*14)
	BCD3	Period1Tab8[14];		//时段表编程前第一套第8日时段表数据(3*14)
	BCD3	Period2Tab1[14];		//时段表编程前第二套第1日时段表数据(3*14)
	BCD3	Period2Tab2[14];		//时段表编程前第二套第2日时段表数据(3*14)
	BCD3	Period2Tab3[14];		//时段表编程前第二套第3日时段表数据(3*14)
	BCD3	Period2Tab4[14];		//时段表编程前第二套第4日时段表数据(3*14)
	BCD3	Period2Tab5[14];		//时段表编程前第二套第5日时段表数据(3*14)
	BCD3	Period2Tab6[14];		//时段表编程前第二套第6日时段表数据(3*14)
	BCD3	Period2Tab7[14];		//时段表编程前第二套第7日时段表数据(3*14)
	BCD3	Period2Tab8[14];		//时段表编程前第二套第8日时段表数据(3*14)

}SEGTAB_PRG_REC;			//682字节


//时区表编程记录结构
typedef struct
{
	BCD3	HappenTime;				//时区表编程发生时间(3)
	BCD3	HappenDate;				//时区表编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD3	Period1_Season1;		//时区表编程前第一套第1时区数据(3)
	BCD3	Period1_Season2;		//时区表编程前第一套第2时区数据(3)
	BCD3	Period1_Season3;		//时区表编程前第一套第3时区数据(3)
	BCD3	Period1_Season4;		//时区表编程前第一套第4时区数据(3)
	BCD3	Period1_Season5;		//时区表编程前第一套第5时区数据(3)
	BCD3	Period1_Season6;		//时区表编程前第一套第6时区数据(3)
	BCD3	Period1_Season7;		//时区表编程前第一套第7时区数据(3)
	BCD3	Period1_Season8;		//时区表编程前第一套第8时区数据(3)
	BCD3	Period1_Season9;		//时区表编程前第一套第9时区数据(3)
	BCD3	Period1_Season10;		//时区表编程前第一套第10时区数据(3)
	BCD3	Period1_Season11;		//时区表编程前第一套第11时区数据(3)
	BCD3	Period1_Season12;		//时区表编程前第一套第12时区数据(3)
	BCD3	Period1_Season13;		//时区表编程前第一套第13时区数据(3)
	BCD3	Period1_Season14;		//时区表编程前第一套第14时区数据(3)

	BCD3	Period2_Season1;		//时区表编程前第二套第1时区数据(3)
	BCD3	Period2_Season2;		//时区表编程前第二套第2时区数据(3)
	BCD3	Period2_Season3;		//时区表编程前第二套第3时区数据(3)
	BCD3	Period2_Season4;		//时区表编程前第二套第4时区数据(3)
	BCD3	Period2_Season5;		//时区表编程前第二套第5时区数据(3)
	BCD3	Period2_Season6;		//时区表编程前第二套第6时区数据(3)
	BCD3	Period2_Season7;		//时区表编程前第二套第7时区数据(3)
	BCD3	Period2_Season8;		//时区表编程前第二套第8时区数据(3)
	BCD3	Period2_Season9;		//时区表编程前第二套第9时区数据(3)
	BCD3	Period2_Season10;		//时区表编程前第二套第10时区数据(3)
	BCD3	Period2_Season11;		//时区表编程前第二套第11时区数据(3)
	BCD3	Period2_Season12;		//时区表编程前第二套第12时区数据(3)
	BCD3	Period2_Season13;		//时区表编程前第二套第13时区数据(3)
	BCD3	Period2_Season14;		//时区表编程前第二套第14时区数据(3)

}ZONE_PRG_REC;			//94字节

//周休日编程记录结构
typedef struct
{
	BCD3	HappenTime;				//时段表编程发生时间(3)
	BCD3	HappenDate;				//时段表编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	unsigned char Week_Code;		//周休日编程前采用的日时段表号

}WEEK_PRG_REC;				//11字节

//节假日编程记录结构
typedef struct
{
	BCD3	HappenTime;				//时段表编程发生时间(3)
	BCD3	HappenDate;				//时段表编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD4 	Holiday[254];			//节假日编程前第1~254节假日数据

}HOLIDAY_PRG_REC;			//1026字节

//组合方式编程记录结构
typedef struct
{
	BCD3	HappenTime;				//时段表编程发生时间(3)
	BCD3	HappenDate;				//时段表编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	unsigned char 	Mode;			//组合方式编程前的组合方式特征字

}ECMODE_PRG_REC;			//11字节

//结算日编程记录结构
typedef struct
{
	BCD3	HappenTime;				//时段表编程发生时间(3)
	BCD3	HappenDate;				//时段表编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD2	SaveDay1;				//结算日编程前每月第1结算日数据(2)
	BCD2	SaveDay2;				//结算日编程前每月第2结算日数据(2)
	BCD2	SaveDay3;				//结算日编程前每月第3结算日数据(2)

}SAVEDAY_PRG_REC;			//16字节


//开端盖记录结构
typedef struct
{
	BCD3	OpenStaTime;			//打开起始时间(3)
	BCD3	OpenStaDate;			//打开起始日期(3)
	BCD3	OpenEndTime;			//打开结束时间(3)
	BCD3	OpenEndDate;			//打开结束日期(3)
	BCD4	BeforeECPp0;			//打开前正向有功总电量(4)
	BCD4	BeforeECPn0;			//打开前反向有功总电量(4)
	BCD4	BeforeECQ10;			//打开前1象限无功总电量(4)
	BCD4	BeforeECQ20;			//打开前2象限无功总电量(4)
	BCD4	BeforeECQ30;			//打开前3象限无功总电量(4)
	BCD4	BeforeECQ40;			//打开前4象限无功总电量(4)
	BCD4	AfterECPp0;				//合上后正向有功总电量(4)
	BCD4	AfterECPn0;				//合上后反向有功总电量(4)
	BCD4	AfterECQ10;				//合上后1象限无功总电量(4)
	BCD4	AfterECQ20;				//合上后2象限无功总电量(4)
	BCD4	AfterECQ30;				//合上后3象限无功总电量(4)
	BCD4	AfterECQ40;				//合上后4象限无功总电量(4)

}OPEN_REC;					//60字节

//购电记录结构
typedef struct
{
	BCD5	HappenDate;				//购电日期(5)
	BCD2	AfterBuyCnt;			//购电后总购电次数(2) 
	BCD4	CurBuyEC;				//购电量(4)           
	BCD4	BeforeRemainEC;			//购电前剩余电量(4)   
	BCD4	AfterRemainEC;			//购电后剩余电量(4)   
	BCD4	AfterBuyECSum;			//购电后累计购电量(4) 												

}BUYEC_REC;					//23字节

//跳闸记录结构
typedef struct
{
	BCD3	HappenTime;				//发生时间(3)
	BCD3	HappenDate;				//发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD4	ECPp0;					//发生时有功正向总电量(4) 
	BCD4	ECPn0;					//发生时有功反向总电量(4) 
	BCD4	ECQ10;					//发生时第一象限无功总电量(4)
	BCD4	ECQ20;					//发生时第二象限无功总电量(4)
	BCD4	ECQ30;					//发生时第三象限无功总电量(4)
	BCD4	ECQ40;					//发生时第四象限无功总电量(4)

}TRIP_REC;					//34字节

/*
//功率因素超限记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)

}PFT_REC;					//44字节
*/

//功率因素超限记录结构
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD3	EndTime;				//异常结束时间(3)
	BCD3	EndDate;				//异常结束日期(3)
	BCD4	EndECPp0;				//结束时刻有功正向总电量(4)
	BCD4	EndECPn0;				//结束时刻有功反向总电量(4)
	BCD4	EndECQp0;				//结束时刻组合无功1总电量(4)
	BCD4	EndECQn0;				//结束时刻组合无功2总电量(4)

}PFT_REC;					//44字节


//安全认证读写通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
  	unsigned short 	FileNo;
  	unsigned short 	DataStAddr;
	unsigned char  	DisItems;	//内部显示小项数
	unsigned char  	DisPoint;   //小数点显示格式  	
}SAUCOMTAB;


//电量通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
//	unsigned char	State1;
//	unsigned char	State2;
//	unsigned char	State3;	
//	unsigned char	State4;
//	unsigned char	State5;
//	unsigned char	State6;
}ECCOMTAB;

//需量通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
//	unsigned char	State1;
//	unsigned char	State2;
//	unsigned char	State3;	
//	unsigned char	State4;
//	unsigned char	State5;
//	unsigned char	State6;
}MDCOMTAB;

//变量通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned char* 	Addr;
  	unsigned short 	ComInfo;
//	unsigned char	State1;
//	unsigned char	State2;
//	unsigned char	State3;	
//	unsigned char	State4;
//	unsigned char	State5;
//	unsigned char	State6;
}REALCOMTAB;

//事件通讯表格
//Addr:
//1)EEPROM地址
//2)事件号
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char  	DisItems;	//内部显示小项数
//	unsigned char  	DisPoint;   //小数点显示格式
	unsigned char  	TimeItems;  //发生、结束时间项数
}EVECOMTAB;

//参变量通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char  	DisItems;	//内部显示小项数
	unsigned char  	DisPoint;   //小数点显示格式
}PARACOMTAB;

//参变量通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
}MANUPARATAB;

//冻结通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;		//数据项该整个冻结数据块中的相对于数据块首地址的相对地址
  	unsigned short 	ComInfo;
	unsigned char	State1;
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char  	DisItems;	//内部显示小项数
}DJCOMTAB;

#if ( DLT645_2007_14 ==	YesCheck )			//新国网	//14.04.25
typedef struct						//新国网	//14.05.04
{
	unsigned long ComID;			//负荷记录抄收命令
	unsigned char ChannelNo;		//曲线类型
	unsigned char StartAddr;		//所取内容在本条记录内起始地址
	unsigned char Length;			//所取内容长度
	unsigned char FillInMode;		//不存在的记录填充方式				//0：补FF，非0：补上一数据点数据
}NEWLOADPARA;

typedef struct						//新国网	//14.05.05
{
	unsigned short LoadRecInterval;	//负荷记录间隔时间
	BCD5	StartTime;				//曲线起始时间
	BCD5	EndTime;				//曲线结束时间
	unsigned char ChannelNo;		//曲线类型
	unsigned char StartAddr;		//所取内容在本条记录内起始地址
	unsigned char Length;			//所取内容长度
	unsigned char LoadMode;			//曲线模式字
}NEWSEARCHPARA;

#endif

//自定义通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
}EXTCOMTAB;

//自定义事件通讯表格
typedef struct
{
	unsigned long 	ComID;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char	State1;
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char	State6;
	unsigned char  	State7;	
	unsigned char  	DisPoint;   //小数点显示格式
}ZDYEVECOMTAB;


//冻结通讯表格
typedef struct
{
	unsigned short 	ECAddr;		//该电量数据的来源地址
	unsigned char	State1;
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char	State6;
	unsigned char  	DisPoint;   //小数点显示格式
	unsigned char  	DSign;   	//显示负号
	unsigned char	Length;
	unsigned char  	StartAddr;	//该小项在数据记录内部起始地址
}PHFAILDISTAB;

typedef struct
{
	unsigned short ID23;
	unsigned short EventNo;	
}IDEVENO;
/*
typedef struct
{
	unsigned char (*p)(void);
	unsigned short	EventNo;
	EVENT_STATE*	Event_State;
	unsigned char* Delay;
	unsigned short	EOldState;
	unsigned short	UpBitSite;	//新国网
}TAMPER;	
*/
//历史记录结构参数
//记录存放于EEPROM的定义：
//EventNo：事件记录号
//PtAddr:当前记录首地址指针(EEPROM直接地址)
//StorageRecs：已存记录数，该数最大为允许读取的总记录条数，超过后不再增加。记录清零时只需将其清零即可
//StartAddr:记录起始地址(EEPROM直接地址)
//EndAddr:记录结束地址(EEPROM直接地址)
//记录存放于FLASH的定义：
//PtAddr:当前记录首地址指针(该记录所占用的FLASH空间内的相对地址)
//StorageRecs：已存记录数，该数最大为允许读取的总记录条数，超过后不再增加。记录清零时只需将其清零即可
//StartAddr:记录起始地址:FLASH起始页号												//(当前次记录放于EEPROM直接地址)
//EndAddr:记录结束地址(占用FLASH页数)												//(低字节FLASH起始页号、高字节：占用FLASH页数)
//RecdCunt:总记录数
//RecdLenth:每条记录长度
//InArrayAddr: 每条记录内每小项数据长度。
//Items: 每条记录内小项数。
//MemType: 记录存储类型。		0：EEPROM,	0x55:FLASH
//Cunt: 累计次数。		无此项数据时，值为0xFFFF
//SumTime: 累计时间。	无此项数据时，值为0xFFFF	

typedef struct
{
	unsigned short EventNo;
	unsigned short PtAddr;
	unsigned short StorageRecs;	
	unsigned short StartAddr;
	unsigned short EndAddr;
	unsigned short RecdCunt;
	unsigned short RecdLenth;	
	unsigned char* InArrayAddr;
	unsigned char Items;
	unsigned char MemType;
	unsigned short Cunt;
	unsigned short SumTime;	
} PTPARA;	

typedef struct
{

	unsigned long 	Comm0FirstLoadNo1;		//串口0第1类负荷最早记录号
	unsigned long 	Comm0FirstLoadNo2;		//串口0第2类负荷最早记录号
	unsigned long 	Comm0FirstLoadNo3;		//串口0第3类负荷最早记录号
	unsigned long 	Comm0FirstLoadNo4;		//串口0第4类负荷最早记录号
	unsigned long 	Comm0FirstLoadNo5;		//串口0第5类负荷最早记录号
	unsigned long 	Comm0FirstLoadNo6;		//串口0第6类负荷最早记录号
                                          
	unsigned long 	Comm1FirstLoadNo1;		//串口1第1类负荷最早记录号
	unsigned long 	Comm1FirstLoadNo2;		//串口1第2类负荷最早记录号
	unsigned long 	Comm1FirstLoadNo3;		//串口1第3类负荷最早记录号
	unsigned long 	Comm1FirstLoadNo4;		//串口1第4类负荷最早记录号
	unsigned long 	Comm1FirstLoadNo5;		//串口1第5类负荷最早记录号
	unsigned long 	Comm1FirstLoadNo6;		//串口1第6类负荷最早记录号
                                          
	unsigned long 	Comm2FirstLoadNo1;		//串口2第1类负荷最早记录号
	unsigned long 	Comm2FirstLoadNo2;		//串口2第2类负荷最早记录号
	unsigned long 	Comm2FirstLoadNo3;		//串口2第3类负荷最早记录号
	unsigned long 	Comm2FirstLoadNo4;		//串口2第4类负荷最早记录号
	unsigned long 	Comm2FirstLoadNo5;		//串口2第5类负荷最早记录号
	unsigned long 	Comm2FirstLoadNo6;		//串口2第6类负荷最早记录号
                                          
	unsigned long 	Comm3FirstLoadNo1;		//串口3第1类负荷最早记录号
	unsigned long 	Comm3FirstLoadNo2;		//串口3第2类负荷最早记录号
	unsigned long 	Comm3FirstLoadNo3;		//串口3第3类负荷最早记录号
	unsigned long 	Comm3FirstLoadNo4;		//串口3第4类负荷最早记录号
	unsigned long 	Comm3FirstLoadNo5;		//串口3第5类负荷最早记录号
	unsigned long 	Comm3FirstLoadNo6;		//串口3第6类负荷最早记录号

//	BCD5		 	Comm0Load1STime;		//串口0第1类负荷最早记录时间
//	BCD5		 	Comm0Load2STime;		//串口0第2类负荷最早记录时间
//	BCD5		 	Comm0Load3STime;		//串口0第3类负荷最早记录时间
//	BCD5		 	Comm0Load4STime;		//串口0第4类负荷最早记录时间
//	BCD5		 	Comm0Load5STime;		//串口0第5类负荷最早记录时间
//	BCD5		 	Comm0Load6STime;		//串口0第6类负荷最早记录时间

//	unsigned char	Comm0Load1Num;			//串口0第1类负荷剩余记录数
//	unsigned char	Comm0Load2Num;			//串口0第2类负荷剩余记录数
//	unsigned char	Comm0Load3Num;			//串口0第3类负荷剩余记录数
//	unsigned char	Comm0Load4Num;			//串口0第4类负荷剩余记录数
//	unsigned char	Comm0Load5Num;			//串口0第5类负荷剩余记录数
//	unsigned char	Comm0Load6Num;			//串口0第6类负荷剩余记录数

//	BCD5		 	Comm1Load1STime;		//串口1第1类负荷最早记录时间
//	BCD5		 	Comm1Load2STime;		//串口1第2类负荷最早记录时间
//	BCD5		 	Comm1Load3STime;		//串口1第3类负荷最早记录时间
//	BCD5		 	Comm1Load4STime;		//串口1第4类负荷最早记录时间
//	BCD5		 	Comm1Load5STime;		//串口1第5类负荷最早记录时间
//	BCD5		 	Comm1Load6STime;		//串口1第6类负荷最早记录时间

//	unsigned char	Comm1Load1Num;			//串口1第1类负荷剩余记录数
//	unsigned char	Comm1Load2Num;			//串口1第2类负荷剩余记录数
//	unsigned char	Comm1Load3Num;			//串口1第3类负荷剩余记录数
//	unsigned char	Comm1Load4Num;			//串口1第4类负荷剩余记录数
//	unsigned char	Comm1Load5Num;			//串口1第5类负荷剩余记录数
//	unsigned char	Comm1Load6Num;			//串口1第6类负荷剩余记录数

//	BCD5		 	Comm2Load1STime;		//串口2第1类负荷最早记录时间
//	BCD5		 	Comm2Load2STime;		//串口2第2类负荷最早记录时间
//	BCD5		 	Comm2Load3STime;		//串口2第3类负荷最早记录时间
//	BCD5		 	Comm2Load4STime;		//串口2第4类负荷最早记录时间
//	BCD5		 	Comm2Load5STime;		//串口2第5类负荷最早记录时间
//	BCD5		 	Comm2Load6STime;		//串口2第6类负荷最早记录时间

//	unsigned char	Comm2Load1Num;			//串口2第1类负荷剩余记录数
//	unsigned char	Comm2Load2Num;			//串口2第2类负荷剩余记录数
//	unsigned char	Comm2Load3Num;			//串口2第3类负荷剩余记录数
//	unsigned char	Comm2Load4Num;			//串口2第4类负荷剩余记录数
//	unsigned char	Comm2Load5Num;			//串口2第5类负荷剩余记录数
//	unsigned char	Comm2Load6Num;			//串口2第6类负荷剩余记录数

//	BCD5		 	Comm3Load1STime;		//串口3第1类负荷最早记录时间
//	BCD5		 	Comm3Load2STime;		//串口3第2类负荷最早记录时间
//	BCD5		 	Comm3Load3STime;		//串口3第3类负荷最早记录时间
//	BCD5		 	Comm3Load4STime;		//串口3第4类负荷最早记录时间
//	BCD5		 	Comm3Load5STime;		//串口3第5类负荷最早记录时间
//	BCD5		 	Comm3Load6STime;		//串口3第6类负荷最早记录时间

//	unsigned char	Comm3Load1Num;			//串口3第1类负荷剩余记录数
//	unsigned char	Comm3Load2Num;			//串口3第2类负荷剩余记录数
//	unsigned char	Comm3Load3Num;			//串口3第3类负荷剩余记录数
//	unsigned char	Comm3Load4Num;			//串口3第4类负荷剩余记录数
//	unsigned char	Comm3Load5Num;			//串口3第5类负荷剩余记录数
//	unsigned char	Comm3Load6Num;			//串口3第6类负荷剩余记录数

	BCD4			Comm0SubsFrameID[4];	//串口0后续帧通讯ID号
	unsigned char 	Comm0LoadNum;			//串口0后续帧剩余记录数		当非负荷曲线时，指分帧步骤号
//	BCD5		 	Comm0LoadSTime;			//串口0后续帧起始时间
	unsigned char 	Comm0FrameNo;			//串口0后续帧序号

	BCD4			Comm1SubsFrameID[4];	//串口1后续帧通讯ID号
	unsigned char 	Comm1LoadNum;			//串口1后续帧剩余记录数		当非负荷曲线时，指分帧步骤号
//	BCD5		 	Comm1LoadSTime;			//串口1后续帧起始时间
	unsigned char 	Comm1FrameNo;			//串口1后续帧序号
	
	BCD4			Comm2SubsFrameID[4];	//串口2后续帧通讯ID号
	unsigned char 	Comm2LoadNum;			//串口2后续帧剩余记录数		当非负荷曲线时，指分帧步骤号
//	BCD5		 	Comm2LoadSTime;			//串口2后续帧起始时间
	unsigned char 	Comm2FrameNo;			//串口2后续帧序号
	
	BCD4			Comm3SubsFrameID[4];	//串口3后续帧通讯ID号
	unsigned char 	Comm3LoadNum;			//串口3后续帧剩余记录数		当非负荷曲线时，指分帧步骤号
//	BCD5		 	Comm3LoadSTime;			//串口3后续帧起始时间
	unsigned char 	Comm3FrameNo;			//串口3后续帧序号
	
}LOADPARA;					//负荷曲线参数 		

typedef struct
{
	unsigned long*	FirstLoadNo;	// 最早记录号
//	unsigned char*	STime;			// 最早记录时间
//	unsigned char*	Sum;			// 剩余记录数
	unsigned char*	SubsPara;		// 后续帧参数
	
}COMMLOADPARA;

//typedef struct
//{
//	unsigned short CurRecNo;		//当前记录号
//   unsigned short AvailRecs;		//当前已存储的有效记录数
//	unsigned short MaxCunt;			//存储空间最大记录数
//	unsigned short RecdCunt;		//存储空间最大有效记录数，去除4K轮转池后
//	unsigned short  StartPageNo;	//存储空间起始页号
//	unsigned short  Pages;			//存储空间所占用页数
//	unsigned short  RecsInPage;		//每页记录数
//	unsigned short RecdLenth;		//记录长度
//	unsigned char ChannelNo;		//通道号
//}CURVEPARA;

typedef struct
{
	unsigned long CurRecNo;			//当前记录号
    unsigned long AvailRecs;		//当前已存储的有效记录数
	unsigned long MaxCunt;			//存储空间最大记录数
	unsigned long RecdCunt;			//存储空间最大有效记录数，去除4K轮转池后
	unsigned short  StartPageNo;		//存储空间起始页号
	unsigned short  Pages;			//存储空间所占用页数
	unsigned short  RecsInPage;		//每页记录数
	unsigned short  RecdLenth;		//记录长度
	unsigned short  ChannelNo;		//通道号
}CURVEPARA;

//typedef struct						//cru
//{
//	unsigned char Buff1[256];
//	unsigned char Buff2[256];
//}FLASHBUFF;

typedef struct						//cru
{
	unsigned char Buff[128];
}FBUFF;

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
typedef struct
{
	unsigned short* ECRamBufAds;
	unsigned char* ECRamBufChkAds;
}FeeECRg;
#endif

//费率电价编程记录结构				//新国网	
typedef struct
{
	BCD3	HappenTime;				//时段表编程发生时间(3)
	BCD3	HappenDate;				//时段表编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD4	ActiveFee1Price;		//当前套费率1单价(4)
	BCD4	ActiveFee2Price;		//当前套费率2单价(4)
	BCD4	ActiveFee3Price;		//当前套费率3单价(4)
	BCD4	ActiveFee4Price;		//当前套费率4单价(4)
	BCD4	PassiveFee1Price;		//备用套费率1单价(4)
	BCD4	PassiveFee2Price;		//备用套费率2单价(4)
	BCD4	PassiveFee3Price;		//备用套费率3单价(4)
	BCD4	PassiveFee4Price;		//备用套费率4单价(4)

}FEEPRICE_PRG_REC;					//42字节

//阶梯参数编程记录结构				//新国网
typedef struct
{
	BCD3	HappenTime;				//阶梯参数编程发生时间(3)
	BCD3	HappenDate;				//阶梯参数编程发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	BCD4	ActiveStep1EC;			//当前套第1阶梯值(4)
	BCD4	ActiveStep2EC;			//当前套第2阶梯值(4)
	BCD4	ActiveStep3EC;			//当前套第3阶梯值(4)
	BCD4	ActiveStep4EC;			//当前套第4阶梯值(4)
	BCD4	ActiveStep5EC;			//当前套第5阶梯值(4)
	BCD4	ActiveStep6EC;			//当前套第6阶梯值(4)
	BCD4	ActiveStep1Price;		//当前套第1阶梯电价(4)
	BCD4	ActiveStep2Price;		//当前套第2阶梯电价(4)
	BCD4	ActiveStep3Price;		//当前套第3阶梯电价(4)
	BCD4	ActiveStep4Price;		//当前套第4阶梯电价(4)
	BCD4	ActiveStep5Price;		//当前套第5阶梯电价(4)
	BCD4	ActiveStep6Price;		//当前套第6阶梯电价(4)
	BCD4	ActiveStep7Price;		//当前套第7阶梯电价(4)
	BCD3	ActiveYearSaveDay1;		//当前年第1结算日(3)
	BCD3	ActiveYearSaveDay2;		//当前年第2结算日(3)
	BCD3	ActiveYearSaveDay3;		//当前年第3结算日(3)
	BCD3	ActiveYearSaveDay4;		//当前年第4结算日(3)
	BCD4	PassiveStep1EC;			//备用套第1阶梯值(4)
	BCD4	PassiveStep2EC;			//备用套第2阶梯值(4)
	BCD4	PassiveStep3EC;			//备用套第3阶梯值(4)
	BCD4	PassiveStep4EC;			//备用套第4阶梯值(4)
	BCD4	PassiveStep5EC;			//备用套第5阶梯值(4)
	BCD4	PassiveStep6EC;			//备用套第6阶梯值(4)
	BCD4	PassiveStep1Price;		//备用套第1阶梯电价(4)
	BCD4	PassiveStep2Price;		//备用套第2阶梯电价(4)
	BCD4	PassiveStep3Price;		//备用套第3阶梯电价(4)
	BCD4	PassiveStep4Price;		//备用套第4阶梯电价(4)
	BCD4	PassiveStep5Price;		//备用套第5阶梯电价(4)
	BCD4	PassiveStep6Price;		//备用套第6阶梯电价(4)
	BCD4	PassiveStep7Price;		//备用套第7阶梯电价(4)
	BCD3	PassiveYearSaveDay1;	//备用年第1结算日(3)
	BCD3	PassiveYearSaveDay2;	//备用年第2结算日(3)
	BCD3	PassiveYearSaveDay3;	//备用年第3结算日(3)
	BCD3	PassiveYearSaveDay4;	//备用年第4结算日(3)

}STEPPARA_PRG_REC;					//138字节


//年结算记录结构					//新国网
typedef struct
{
	BCD4	HappenDate;				//年结算时间(3)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	
}YEARSAVETIME_REC;					//8字节

//月结算记录结构					//新国网
typedef struct
{
	BCD4	HappenDate;				//月结算时间(3)
	
}MONTHSAVETIME_REC;					//4字节

//密钥更新记录结构					//新国网
typedef struct
{
	BCD3	HappenTime;				//密钥更新发生时间(3)
	BCD3	HappenDate;				//密钥更新发生日期(3)
	BCD4	OperatorCode;			//操作者代码(4)
	unsigned char Cunt;				//密钥更新的总条数(1)
	BCD4	KeyState;				//更新前的密钥状态字(4)

}SECRETKEY_PRG_REC;					//15字节

//异常插卡记录结构					//新国网
typedef struct
{
	BCD3	HappenTime;				//密钥更新发生时间(3)
	BCD3	HappenDate;				//密钥更新发生日期(3)
	BCD8	CardNo;					//CPU卡序列号(8)	
	unsigned char ErrorByte;		//错误信息字(1)
	BCD5	CommandHead;			//操作命令头(5)
	BCD2	ErrorResponse;			//错误响应状态(2)
	BCD2	RBuyECCnt;				//插卡时总购电次数(2)
	BCD4	RemainEC;				//插卡时剩余金额(4)
	BCD4	ECPp0;					//插卡时正向有功总电能(4)
	BCD4	ECPn0;					//插卡时反向有功总电能(4)
	
}CARDINFAULT_REC;					//36字节

//退费记录结构						//新国网
typedef struct
{
	BCD3	HappenTime;				//退费发生时间(3)
	BCD3	HappenDate;				//退费发生日期(3)
	BCD2	RBuyECCnt;				//退费前购电次数(2)
	BCD4	FeeBackSum;				//退费金额(4)
	BCD4	BeforeRemainEC;			//退费前剩余金额(4)
	BCD4	AfterRemainEC;			//退费后剩余金额(4)

}FEEBACK_REC;						//20字节

//恒定磁场干扰记录结构				//新国网
typedef struct
{
	BCD3	StaTime;				//发生时间(3)
	BCD3	StaDate;				//发生日期(3)
	BCD3	EndTime;				//结束时间(3)
	BCD3	EndDate;				//结束日期(3)
	BCD4	StaECPp0;				//发生前正向有功总电能(4)
	BCD4	StaECPn0;				//发生前反向有功总电能(4)
	BCD4	EndECPp0;				//发生后正向有功总电能(4)
	BCD4	EndECPn0;				//发生后反向有功总电能(4)
	
}MAGNETDISTURB_REC;					//36字节

//负荷开关误动作记录结构			//新国网
typedef struct
{
	BCD3	StaTime;				//发生时间(3)
	BCD3	StaDate;				//发生日期(3)
	BCD3	EndTime;				//结束时间(3)
	BCD3	EndDate;				//结束日期(3)
	unsigned char EndState;			//结束状态(1)
	BCD4	StaECPp0;				//发生前正向有功总电能(4)
	BCD4	StaECPn0;				//发生前反向有功总电能(4)
	BCD4	EndECPp0;				//发生后正向有功总电能(4)
	BCD4	EndECPn0;				//发生后反向有功总电能(4)
	
}RELAYFAULT_REC;					//29字节

//电源异常记录结构					//新国网
typedef struct
{
	BCD3	StaTime;				//发生时间(3)
	BCD3	StaDate;				//发生日期(3)
	BCD3	EndTime;				//结束时间(3)
	BCD3	EndDate;				//结束日期(3)
	BCD4	ECPp0;					//发生时正向有功总电能(4)
	BCD4	ECPn0;					//发生时反向有功总电能(4)
	
}POWERFAULT_REC;					//20字节

//总潮流反向记录结构				//新国网
typedef struct
{
	BCD3	StaTime;				//异常起始时间(3)
	BCD3	StaDate;				//异常起始日期(3)
	unsigned char StaDir;			//发生时刻总有功功率方向(1)
	BCD4	StaECPp0;				//发生时刻有功正向总电量(4)
	BCD4	StaECPn0;				//发生时刻有功反向总电量(4)
	BCD4	StaECQp0;				//发生时刻组合无功1总电量(4)
	BCD4	StaECQn0;				//发生时刻组合无功2总电量(4)
	BCD4	StaECPpA;				//发生时刻A相有功正向总电量(4)
	BCD4	StaECPnA;				//发生时刻A相有功反向总电量(4)
	BCD4	StaECQpA;				//发生时刻A相组合无功1总电量(4)
	BCD4	StaECQnA;				//发生时刻A相组合无功2总电量(4)
	BCD4	StaECPpB;				//发生时刻B相有功正向总电量(4)
	BCD4	StaECPnB;				//发生时刻B相有功反向总电量(4)
	BCD4	StaECQpB;				//发生时刻B相组合无功1总电量(4)
	BCD4	StaECQnB;				//发生时刻B相组合无功2总电量(4)
	BCD4	StaECPpC;				//发生时刻C相有功正向总电量(4)
	BCD4	StaECPnC;				//发生时刻C相有功反向总电量(4)
	BCD4	StaECQpC;				//发生时刻C相组合无功1总电量(4)
	BCD4	StaECQnC;				//发生时刻C相组合无功2总电量(4)

}TIDESWITCH_REC;				//71字节

#if( ECDisDecDigitsValid ==	YesCheck )		//13.03.03 当前电量显示小数位真实有效
typedef struct
{
	unsigned char* ECRAds;
	unsigned short* ECRamBufAds;
	unsigned char* ECRamBufChkAds;
} ECDisDigitRg;
#endif

//电表规格参数						//新国网
typedef struct
{
	unsigned long	RBaseCurrent;			//基本电流(4)					//HEX	0.0001A				//新国网		
	unsigned long	RMaxCurrent;			//最大电流(4)                   //HEX 	0.0001A             //新国网
	unsigned long	RMeterConst;			//电表有功常数(3)               //HEX                       //新国网
	unsigned long	LongReserve1;			//保留1		
	unsigned long	LongReserve2;			//保留2
	
	unsigned short	RRatedVolt;				//额定电压(2)					//HEX	0.1V				//新国网		
	unsigned short	RMDPS32;				//需量计算系数(2)               //HEX                       //新国网
	unsigned short	R7022C_HFConst;			//7022C高频脉冲常数(2)          //HEX                       //新国网
	unsigned short	R7022E_HFConst;			//7022E高频脉冲常数(2)          //HEX                       //新国网
	unsigned short	R7022E_PStartup;		//7022E功率启动阀值(2)          //HEX                       //新国网
	unsigned short	R7022E_IStart;			//7022E电流启动阀值(2)          //HEX                       //新国网
	unsigned short	R90E32_PStartup;		//90E32AS功率启动阀值(2)        //HEX                       //新国网
	unsigned short	R90E32_PCreeping;		//90E32AS功率潜动阀值(2)        //HEX                       //新国网
	unsigned short	RPW00002Ib;				//最小功率限值(2)               //HEX 	0.001W              //新国网
	unsigned short	RDisA0001Ib;			//最小显示电流限值(2)           //HEX 0.0001A               //新国网	//13.10.23

	unsigned short	IntReserve1;			//保留1		
	unsigned short	IntReserve2;			//保留2
	unsigned short	IntReserve3;			//保留3		
	unsigned short	IntReserve4;			//保留4
	
	unsigned char	R90E32_AmperMult;		//90E32AS电流功率放大倍数(1)    //HEX 最高位置1表示缩小(除) //新国网

	unsigned char	CharReserve1;			//保留1		
	unsigned char	CharReserve2;			//保留2
	
}METERSPEC;

typedef struct						//新国网	//13.09.16
{
	unsigned long ComID;
	unsigned short Addr;
	unsigned char* RAds;
	unsigned char DataLen;
	unsigned char FlashAddr;	
}METERPARA;

#if ( AdjustErrorCheck == YesCheck )	//15.09.05
//VoltAmperFtFreqState;			//电压电流功率因数频率状态
#define Un115Flag	0x0001		//电压处于115%Un状态
#define Un110Flag	0x0002		//电压处于110%Un状态
#define Un100Flag	0x0004		//电压处于100%Un状态
#define Un90Flag	0x0008		//电压处于90%Un状态
#define Un80Flag	0x0010		//电压处于80%Un状态
#define ImaxFlag	0x0020		//电流处于Imax状态
#define Ib100Flag	0x0040		//电流处于100%Ib状态
#define Ib10Flag	0x0080		//电流处于10%Ib状态
#define Ib05Flag	0x0100		//电流处于5%Ib状态
#define Ib02Flag	0x0200		//电流处于2%Ib状态
#define Ft1_0Flag	0x0400		//功率因数处于1.0状态
#define Ft0_5Flag	0x0800		//功率因数处于0.5状态
#define F51HzFlag	0x1000		//频率处于51Hz状态
#define F50HzFlag	0x2000		//频率处于50Hz状态
#define F49HzFlag	0x4000		//频率处于49Hz状态

#define F49Hz1_0	F49HzFlag+Un100Flag+Ft1_0Flag
#define F49Hz0_5L	F49HzFlag+Un100Flag+Ft0_5Flag
#define F51Hz1_0	F51HzFlag+Un100Flag+Ft1_0Flag
#define F51Hz0_5L	F51HzFlag+Un100Flag+Ft0_5Flag

#define Un115_1_0	F50HzFlag+Un115Flag+Ft1_0Flag
#define Un110_1_0	F50HzFlag+Un110Flag+Ft1_0Flag
#define Un110_0_5L	F50HzFlag+Un110Flag+Ft0_5Flag
#define Un90_1_0	F50HzFlag+Un90Flag+Ft1_0Flag
#define Un90_0_5L	F50HzFlag+Un90Flag+Ft0_5Flag
#define Un80_1_0	F50HzFlag+Un80Flag+Ft1_0Flag

#define SelfHeat	F50HzFlag+Un100Flag+ImaxFlag

//误差变量
typedef struct
{
//	unsigned short	RSpeCalibPermit;			//0x1234:允许特殊校表
//	unsigned short 	RSpeCalibState;				//0x4567:复位7022，进入特殊校表状态
	unsigned short 	VoltAmperFtFreqState;		//电压电流功率因数频率状态

//	unsigned short 	SelfHeatEnterDelay;			//自热模式进入延时	秒
	
	unsigned char 	F49HzImax1_0_Delay;			//符合状态的3秒延时
	unsigned char 	F49HzIb1_0_Delay;			//符合状态的3秒延时
	unsigned char 	F49Hz05Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	F49Hz02Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	F49HzImax0_5L_Delay;		//符合状态的3秒延时
	unsigned char 	F49HzIb0_5L_Delay;			//符合状态的3秒延时
	unsigned char 	F49Hz10Ib0_5L_Delay;		//符合状态的3秒延时
	unsigned char 	F49Hz05Ib0_5L_Delay;		//符合状态的3秒延时
	
	unsigned char 	F51HzImax1_0_Delay;			//符合状态的3秒延时
	unsigned char 	F51HzIb1_0_Delay;			//符合状态的3秒延时
	unsigned char 	F51Hz05Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	F51Hz02Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	F51HzImax0_5L_Delay;		//符合状态的3秒延时
	unsigned char 	F51HzIb0_5L_Delay;			//符合状态的3秒延时
	unsigned char 	F51Hz10Ib0_5L_Delay;		//符合状态的3秒延时
	unsigned char 	F51Hz05Ib0_5L_Delay;		//符合状态的3秒延时

	unsigned char 	Un115Imax1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un115Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un115_05Ib1_0_Delay;		//符合状态的3秒延时
	unsigned char 	Un115_02Ib1_0_Delay;		//符合状态的3秒延时
	
	unsigned char 	Un110Imax1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un110Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un110_05Ib1_0_Delay;		//符合状态的3秒延时
	unsigned char 	Un110_02Ib1_0_Delay;		//符合状态的3秒延时
	unsigned char 	Un110Imax0_5L_Delay;		//符合状态的3秒延时
	unsigned char 	Un110Ib0_5L_Delay;			//符合状态的3秒延时
	unsigned char 	Un110_10Ib0_5L_Delay;		//符合状态的3秒延时
	unsigned char 	Un110_05Ib0_5L_Delay;		//符合状态的3秒延时

	unsigned char 	Un90Imax1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un90Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un90_05Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un90_02Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un90Imax0_5L_Delay;			//符合状态的3秒延时
	unsigned char 	Un90Ib0_5L_Delay;			//符合状态的3秒延时
	unsigned char 	Un90_10Ib0_5L_Delay;		//符合状态的3秒延时
	unsigned char 	Un90_05Ib0_5L_Delay;		//符合状态的3秒延时

	unsigned char 	Un80Imax1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un80Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un80_05Ib1_0_Delay;			//符合状态的3秒延时
	unsigned char 	Un80_02Ib1_0_Delay;			//符合状态的3秒延时

	unsigned char 	SelfHeat_Delay;				//符合状态的240秒延时

	unsigned char	RErrorMode;					//0:正常 
	
	unsigned char	RSpeAdjustPermit;			//特殊误差点补偿允许(2)		允许：0x67;不允许：其他
	
}ERRORRAM;

//误差转换表格
typedef struct
{
	unsigned short 	ComID;
	unsigned short 	Addr;		//该特殊误差项在EEPROM中地址
	unsigned char*  StateDelay;
  	unsigned short 	ComInfo;
	unsigned short	ComID1;
	unsigned short	ModeState;
	unsigned char	Mode;
	unsigned char	Resve;	
}ERRORCOMTAB;

#endif

/*
typedef struct
{
	unsigned char*	Buf;			// 通讯缓存器
	unsigned char*	Mode;			// 通讯模式
  	unsigned short*	Ptr;			// 缓存器指针
  	unsigned short*	BTime;			// 字符间定时
  	unsigned char*	SendDTime;		// 发送延时
  	unsigned char*	RecTmr;			// 通讯延时
  	unsigned char*	FESum;			// 通讯FE数量
	unsigned char* 	PrgTmr;			// 编程记录延时
	unsigned char*	IECState;		// 通讯口IEC通讯过程状态
	unsigned char*	MACState;		// 通讯口MAC通讯过程状态
	unsigned char*	LLCState;		// 通讯口MAC通讯过程状态
	unsigned char*	TX_Delay;		// 发送延时
	unsigned char*	Run;			// 485口运行标志
//	unsigned char* 	BandRate;		// 485口波特率
	unsigned char* 	RComDCB;		// 485口波特率等参数		[0]:波特率、[1]:校验位、[2]:数据位、[3]:停止位、[4]:流控
	unsigned char* 	FTime;			// 帧间定时
	unsigned short* RxdFrameNo;		// 接收帧计数器
	unsigned short* TxdFrameNo;		// 发送帧计数器
	unsigned char* 	LastRecCommand;	// 最后收到的命令
	unsigned short*	MaxInfoSize;	// 收发帧最大尺寸

	unsigned char* 	ClientAddr;		// 客户机地址						//17.04.11
	unsigned long* 	ConnectValidTime;	//串口应用连接有效时间			//17.04.11
	unsigned long* 	ConnectValidDelay;	//串口应用连接有效时间延时
	unsigned short* BlockNumber;	// 串口GET块通讯块号
	unsigned char*	APPLState;		// 通讯口APPL连接状态
	unsigned short* HBTime;			// 485口字符间定时
  	unsigned short* RecDataPtr;		// 485口缓存器指针
  	unsigned short* SendDataPtr;	// 485口缓存器指针
  	unsigned short* SendByteSum;	// 485口LLC发送总数
  	unsigned char* 	RecDataBuf;		// 485口接收通信缓存		
  	unsigned char* 	SendDataBuf;	// 485口发送通信缓存		
  	unsigned char* 	HMode;			// 485口通讯模式
  	unsigned char*	HState;			// 485口通信状态
//	unsigned char* 	HBandRate;		// 485口波特率
	unsigned char* 	HRecTmr;		// 接收记录延时
	unsigned char* 	HPrgTmr;		// 编程记录延时
	unsigned char*	Conformance;	// 通讯口应用连接的一致性块参数
	unsigned char*	FunConformance;	// 通讯口应用连接的功能一致性块参数	//17.04.11
	unsigned char*	SubsBlockPara;	// 串口后续帧块参数
//	unsigned char*	SetBlockBuff;	// 块设置缓存首址
//	unsigned short*	SetBlockPtr;	// 块设置缓存指针
  	unsigned short*	MaxApduSize;	// 串口最大APDU尺寸

	unsigned char* SecurityState;		// 通讯口安全状态		0x00: 非安全认证操作 0x01：明文 0x02:密文	
	unsigned char* SecurityVerifyType;	// 通讯口安全验证类型	0：SIG_MAC 1：RN 2：RN_MAC 3：SID
	unsigned char* TimeTag;				// 通讯口时间标签	optional:1字节，data_time_s:7字节，TI:3字节
	
}DL698COMMPARA;
*/

////IECState						//通讯口1IEC通讯过程状态
//#define IECIdle				0	// IEC通讯空闲
//#define IECStartCode		1	// IEC通讯起始
//#define IECStartCodeOver	2	// IEC通讯起始帧结束
//#define IECBandRate			3	// IEC通讯切换波特率
//#define IECBandRateOver		4	// IEC通讯切换波特率帧结束
//#define IECModeC			5	// IEC通讯模式C
//#define IECModeCOver		6	// IEC通讯模式C帧结束
//#define IECModeCRXOver		7	// IEC通讯模式C帧结束	
//#define IECModeHDLC			8	// IEC通讯模式E,HDLC
//#define IECModeHDLCOver		9	// IEC通讯模式E,HDLC帧结束
//#define IECModeHDLCRXOver	10	// IEC通讯模式E,HDLC帧结束
//#define IECModeExit			11	// IEC通讯退出
/*
// 	IECState					// Modbus通讯规约处理过程状态
#define IECIdle				0	// 通讯空闲
#define IECBufBusy			1	// 开始收数据，通讯缓存忙
#define IECRead				2	// 收完数据
#define IECDataSend1Wait	3	// 数据包1发送等待
#define IECDataSend2		4	// 数据包2发送
#define IECDataSend2Wait	5	// 数据包2发送等待
#define IECDataSend3		6	// 数据包3发送
#define IECDataSend3Wait	7	// 数据包3发送等待
#define IECDataSend4		8	// 数据包4发送
#define IECDataSend4Wait	9	// 数据包4发送等待
#define IECExit				10	// 通讯退出

//MACState						//通讯口1MAC通讯过程状态
#define	NDMState			0	// MAC未连接
#define	NRMState			1	// MAC已连接		

//LLCState						// 通讯口1MAC通讯过程状态
#define	LLCIdle				0	// LLC空闲
#define	WaitRecNextState	1	// LLC等待
#define	SendNextState		2	// 发送未完
#define	UISendNextState		3	// UI帧发送未完
*/
typedef struct
{
  unsigned short	CommIntReserve1;	//保留整型空间1
  unsigned short	CommIntReserve2;	//保留整型空间2
  unsigned short	CommIntReserve3;	//保留整型空间3

  unsigned short   BatOnDly;			// 停抄电池供电时间延时
  unsigned short 	RecCount;			// 端口接收脉冲数			

  unsigned short 	Frame_BTime0;		// 485口1帧内字符间定时 1/2ms
  unsigned short 	Frame_BTime1;		// 485口2帧内字符间定时 1/2ms
  unsigned short 	Frame_BTime2;		// 485口2帧内字符间定时 1/2ms
                                                        
  unsigned short 	BTime0;				// 485口1字符间定时
  unsigned short 	BTime1;				// 485口2字符间定时
  unsigned short 	BTime2;				// 485口3字符间定时

  unsigned char 	RxdCnt0;			// 接收帧计数器1
  unsigned char 	RxdCnt1;			// 接收帧计数器2
  unsigned char 	RxdCnt2;			// 接收帧计数器3


	unsigned char	SendCnt0;			// 发送帧计数器1
	unsigned char	SendCnt1;			// 发送帧计数器2
	unsigned char	SendCnt2;			// 发送帧计数器2




  	unsigned short 	Ptr0;				// 485口1缓存器指针
  	unsigned short 	Ptr1;				// 485口2缓存器指针
  	unsigned short 	Ptr2;				// 485口3缓存器指针


  	unsigned char 	Buf0[256];			// 485口1通信缓存		
  	unsigned char 	Buf1[256];			// 485口2通信缓存		
 	unsigned char 	Buf2[256];			// 485口3通信缓存		
		

	unsigned char 	Mode0;				// 485口1通讯模式
  	unsigned char 	Mode1;				// 485口2通讯模式
  	unsigned char 	Mode2;				// 485口3通讯模式

	unsigned char	SendDTime0;			// 发送延时1
	unsigned char	SendDTime1;			// 发送延时2
	unsigned char	SendDTime2;			// 发送延时3

	unsigned char 	TX_Delay0;			// 发送延时1
	unsigned char 	TX_Delay1;			// 发送延时2
	unsigned char 	TX_Delay2;			// 发送延时3

	unsigned char	Run0;				//485口1运行标志
	unsigned char	Run1;				//485口2运行标志
	unsigned char	Run2;				//485口3运行标志


	unsigned char 	BandRate0;			//485口1波特率
	unsigned char 	BandRate1;			//485口2波特率
	unsigned char 	BandRate2;			//485口3波特率


	unsigned char 	RecTmr0;			// 接收记录延时
	unsigned char 	RecTmr1;			// 接收记录延时
	unsigned char 	RecTmr2;			// 接收记录延时
	unsigned char 	RecTmr3;			// 接收记录延时


	unsigned char 	FTime0;				// 帧间定时1
	unsigned char 	FTime1;				// 帧间定时2
	unsigned char 	FTime2;				// 帧间定时3


	unsigned char IECState0;	// 通讯口1IEC通讯过程状态
	unsigned char IECState1;	// 通讯口2IEC通讯过程状态
	unsigned char IECState2;	// 通讯口3IEC通讯过程状态

	unsigned char MACState0;	// 通讯口1MAC通讯过程状态                                                         
	unsigned char MACState1;	// 通讯口2MAC通讯过程状态                                                         
	unsigned char MACState2;	// 通讯口3MAC通讯过程状态                                                         
                                                                                                                 

    unsigned char LLCState0;	// 通讯口1MAC通讯过程状态                                                     
    unsigned char LLCState1;	// 通讯口2MAC通讯过程状态                                                     
    unsigned char LLCState2;	// 通讯口2MAC通讯过程状态                                                     
                                                    

    unsigned char LastRecCommand0;	// 通讯口1最后收到的命令
    unsigned char LastRecCommand1;	// 通讯口2最后收到的命令
    unsigned char LastRecCommand2;	// 通讯口3最后收到的命令


	unsigned char	Run;				//运行标志

	unsigned char 	BandRateMode;	//波特率模式 0：可设；1：自适应   
	unsigned char   CPAOnDly;			// 停电期间比较器打开延时	

	BCD4			OperateCode;	//操作者代码
	BCD4			TripONOperateCode;	//合闸操作者代码
	BCD4			TripOFFOperateCode;	//跳闸操作者代码

	unsigned char 	BatComBuff[3];	//停抄唤醒缓存区				//新国网

	unsigned char 	TX_CardDelay;		//卡发送延时
	BCD8			CommReserve;		//保留8字节空间

}COMM;

//APPLState						// 通讯口1APPL连接状态
#define	APPLIdle			0	// APPL空闲
#define	APPLON1				1	// APPL已连接1	//公共连接		
#define	APPLON2				2	// APPL已连接2	//一般密码	
#define	APPLON3				3	// APPL已连接3	//对称加密	//17.04.07
#define	APPLON4				4	// APPL已连接4	//数字签名	//17.04.07

#define SUBS_PARA_LEN		100		//块传输后续帧参数缓存长度

typedef struct
{
	unsigned long	BlockNumber0;		//串口0GET块通讯块号
	unsigned long	BlockNumber1;		//串口1GET块通讯块号
	unsigned long	BlockNumber2;		//串口1GET块通讯块号
	unsigned long	BlockNumber3;		//串口1GET块通讯块号

	unsigned short CommIntReserve1;	//保留整型空间1
	unsigned short CommIntReserve2;	//保留整型空间2

	unsigned short SetBlockPtr0;		// 485口1块设置缓存指针
	unsigned short SetBlockPtr1;		// 485口2块设置缓存指针
	unsigned short SetBlockPtr2;		// 485口2块设置缓存指针
	unsigned short SetBlockPtr3;		// 485口2块设置缓存指针

	unsigned short 	HBTime0;			// 485口1字符间定时
	unsigned short 	HBTime1;			// 485口2字符间定时
	unsigned short 	HBTime2;			// 485口2字符间定时
	unsigned short 	HBTime3;			// 485口2字符间定时

	unsigned short 	RecDataPtr0;		// 485口1缓存器指针
	unsigned short 	SendDataPtr0;		// 485口1缓存器指针
	unsigned short 	RecDataPtr1;		// 485口2缓存器指针
	unsigned short 	SendDataPtr1;		// 485口2缓存器指针
	unsigned short 	RecDataPtr2;		// 485口2缓存器指针
	unsigned short 	SendDataPtr2;		// 485口2缓存器指针
	unsigned short 	RecDataPtr3;		// 485口2缓存器指针
	unsigned short 	SendDataPtr3;		// 485口2缓存器指针

	unsigned short 	SendByteSum0;		// 485口1LLC发送总数
	unsigned short 	SendByteSum1;		// 485口2LLC发送总数
	unsigned short 	SendByteSum2;		// 485口2LLC发送总数
	unsigned short 	SendByteSum3;		// 485口2LLC发送总数

	unsigned char	RecDataBuf0[256];	// 485口1接收通信缓存		//530---2016-6-16-fire
	unsigned char	SendDataBuf0[256];	// 485口1发送通信缓存		//530---2016-6-16-fire
	unsigned char	RecDataBuf1[256];	// 485口2接收通信缓存		//150
	unsigned char	SendDataBuf1[256];	// 485口2发送通信缓存		//150
	unsigned char	RecDataBuf2[256];	// 485口2接收通信缓存		//150
	unsigned char	SendDataBuf2[256];	// 485口2发送通信缓存		//150


	unsigned char	HMode0; 			// 485口1通讯模式
	unsigned char	HMode1; 			// 485口2通讯模式	
	unsigned char	HMode2; 			// 485口2通讯模式
	unsigned char	HMode3; 			// 485口2通讯模式
	
	unsigned char		HState0;			// 485口1通信状态
	unsigned char		HState1;			// 485口2通信状态
	unsigned char		HState2;			// 485口2通信状态
	unsigned char		HState3;			// 485口2通信状态

//	unsigned char	HRun;					// 运行标志
										   
	unsigned char	HBandRate0; 	// 485口1波特率
	unsigned char	HBandRate1; 		// 485口2波特率
	unsigned char	HBandRate2; 		// 485口2波特率
	unsigned char	HBandRate3; 		// 485口2波特率

	unsigned char	HRecTmr0;			// 接收记录延时
	unsigned char	HRecTmr1;			// 接收记录延时
	unsigned char	HRecTmr2;			// 接收记录延时
	unsigned char	HRecTmr3;			// 接收记录延时

	unsigned char	HPrgTmr0;			// 编程记录延时
	unsigned char	HPrgTmr1;			// 编程记录延时
	unsigned char	HPrgTmr2;			// 编程记录延时
	unsigned char	HPrgTmr3;			// 编程记录延时
	
	unsigned char APPLState0;			// 通讯口1APPL连接状态
	unsigned char APPLState1;			// 通讯口2APPL连接状态
	unsigned char APPLState2;			// 通讯口2APPL连接状态
	unsigned char APPLState3;			// 通讯口2APPL连接状态

	BCD4 Conformance_block0;			// 通讯口1应用连接的一致性块参数	//前三字节为一致性块参数，后一字节为安全级别。0：无安全级别 0x07: 最低安全级别	
	BCD4 Conformance_block1;			// 通讯口2应用连接的一致性块参数	//前三字节为一致性块参数，后一字节为安全级别。0：无安全级别 0x07: 最低安全级别	
	BCD4 Conformance_block2;			// 通讯口2应用连接的一致性块参数	//前三字节为一致性块参数，后一字节为安全级别。0：无安全级别 0x07: 最低安全级别	
	BCD4 Conformance_block3;			// 通讯口2应用连接的一致性块参数	//前三字节为一致性块参数，后一字节为安全级别。0：无安全级别 0x07: 最低安全级别	
	unsigned char	SubsBlockPara0[SUBS_PARA_LEN];	//串口0后续帧块参数
	unsigned char	SubsBlockPara1[SUBS_PARA_LEN];	//串口1后续帧块参数
	unsigned char	SubsBlockPara2[SUBS_PARA_LEN];	//串口1后续帧块参数
	unsigned char	SubsBlockPara3[SUBS_PARA_LEN];	//串口1后续帧块参数

//	enum {
//		APPLIdle,				// APPL空闲
//		APPLON, 				// APPL已连接		
//	} APPLState0;				// 通讯口1APPL连接状态

//add sky20140519..
	unsigned char APPL_A8_BUSY[4];		   // 判断接收是否处于分帧操作状态..
//	enum {
//	  DATABLOCKIDLE,		// 无：0
//		FIRSTDATABLOCK, 			// 最初第一块传输..
//		MIDDATABLOCK,					// 数据块中间传输..
//		ENDDATABLOCK,					// 最后一块传输..
//	  NEW_BLOCK,				// 无数据块分帧需要重新打成数据块包进行传输..
//	} COSEM_BlockState; 			// 数据块传输状态	
  unsigned char COSEM_BlockState[4];   // 判断需要分块后再分包操作的数据块传输的状态..
//	unsigned char BIGBlockNumber[4];	 // 大的数据块数量---参与应用层的数据计算..
  unsigned short BIGBlockNumber[4];	 // 大的数据块数量---参与应用层的数据计算..
//参与打包操作的前期字符;
 unsigned char Send_state_InvokeId;   // 
 
	//客户机地址--判断权限；
	  unsigned char ClientAddr[4];
	BCD8			CommReserve;	//保留8字节空间
	unsigned short  BK_FCRXCount;	// 接收..
	unsigned short  DK_FCRXCount;	// 接收..	
	unsigned short  DK_FCTXCount;	//
	
	unsigned short  UK_FCRXCount;	// 接收..
	unsigned short  UK_FCTXCount;	//
		 
	unsigned char  RepeatBuff[256];
	
}HCOMM;

#pragma pack(8) 
//密码加密认证变量..加解密的变量要4个字节对齐方式存储于内存中
typedef struct
{
  unsigned char 	Special_A8[4];  		 //特殊分帧处理--即超过512字节（泰国临时）..
  unsigned char		aarqHLS[4];					 //不同通讯口，分别处理..
  unsigned char 	type[4];          	 //加密类型：10--认证加密；20--应用层数据加密；30--认证加密
  unsigned char 	glo_request[4];   	 //全部加密控制识别标识..
  
  unsigned char   glo_FC[6];					 //
  unsigned char   choice_algorithm;    //选择何种算法..
  unsigned char   Challenge_Len;  
  unsigned char   S_Challenge[16];		 //服务器的随机数..
  unsigned char   C_Challenge[16];     //客户端的随机数..
   unsigned char 	AK[16];
   unsigned char 	EK[16]; 
   unsigned char 	IV[12];
  unsigned char 	SH[5];
  // 增加停电判断时间..
  unsigned char    CutPower;            //0x0A---表示停电过一次，刚上电，处理过之后则不再进行判断..  
  unsigned char    GPRS_STAND;          //GPRS模式插上标志0x0B.. 模块拔掉则复位..
  unsigned char    comm_online;         //1--在线； 0-- 不在线;
  
}CRYPT;						//字节	
#pragma pack()
//推送40类--告警事件..
typedef struct
{
	//修改读取曲线出错增加(读取期间记录数正好在增加)..
	unsigned long   PReadCurRecNo;	 // skyblue20141001..
	unsigned short  SendNumber;
	unsigned char   RepeatSendFlag;	 //块传输是否需要重发标志,0x22表示需要重发..
	//判断..			
	unsigned char Time; 					//间隔发送..
	unsigned char Comm_Flag;			//通讯标识..
			
}PUSHSTATE;
/*
//电量通讯表格
typedef struct
{
	unsigned short 	OI;
	unsigned short 	Addr;
  	unsigned short 	SaftInfo;		//安全模式参数
	unsigned char	FeeNum;			//费率数+是否支持精确电量
	unsigned char 	Unit;			//单位
	unsigned char	State1;			//液晶段码1
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char	State6;
}DL698_ECTAB;

typedef struct
{
	unsigned short 	OI;
	unsigned short 	Addr;
  	unsigned short 	SaftInfo;		//安全模式参数
	unsigned char	FeeNum;			//费率数
	unsigned char 	Unit;			//单位
	unsigned char	State1;			//液晶段码1
	unsigned char	State2;
	unsigned char	State3;	
	unsigned char	State4;
	unsigned char	State5;
	unsigned char	State6;
}DL698_MDTAB;

typedef struct
{
	unsigned char* 	Addr;
	unsigned char 	OAD[4];
  	unsigned short 	SaftInfo;		//安全模式参数
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	Element_Len;			
	unsigned char	BCD_Len;			//原BCD数据长度
	signed char			Multiple;			//放大倍数				负数表示缩小
}DL698_CLASSID3_TAB;

typedef struct
{
	unsigned char* 	Addr;
	unsigned char 	OAD[4];
  	unsigned short 	SaftInfo;		//安全模式参数
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	Element_Len;			//数组或结构时表示元素个数
	unsigned char	BCD_Len;			//原BCD数据长度
	signed 	 char	Multiple;			//放大倍数				负数表示缩小
}DL698_CLASSID6_TAB;

typedef struct
{
	unsigned short 	Addr;
	unsigned char 	OAD[4];
  	unsigned short 	SaftInfo;		//安全模式参数
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	Element_Len;			//数组或结构时表示元素个数
	unsigned char	BCD_Len;			//原BCD数据长度
	signed char			Multiple;			//放大倍数				负数表示缩小
}DL698_CLASSID24_5_TAB;

typedef struct
{
	unsigned short 	Addr;
	unsigned char 	OAD[4];
  	unsigned short 	ComInfo;
  	unsigned short 	SaftInfo;		//安全模式参数
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	Element_Len;			//数组或结构时表示元素个数
}DL698_CLASSID8_TAB;

//初始化显示类捕获参数表格
typedef struct
{
	unsigned char 	Data_Type;				//CHOICE 0：OAD 1：ROAD
	unsigned char 	OAD1[4];				//显示项OAD
	unsigned char 	OAD2[4];				//ROAD中的OAD
  	unsigned char 	Dis_Number;				//屏序号
}INI_DISPLAY_TAB;

//初始化显示类捕获参数表格
typedef struct
{
	unsigned char 	OAD[4];				//事件记录项OAD
}INI_EVENT_TAB;

//初始化冻结类捕获参数表格
typedef struct
{
	unsigned short	Period;				//冻结周期
	unsigned char 	OAD[4];				//事件记录项OAD
	unsigned short	Deep;				//冻结深度
}INI_FREEZE_TAB;

typedef struct
{
	unsigned short	BufAddr;
	unsigned short	BufLen;
	unsigned char 	OAD[4];				//事件记录项OAD	
}INI_EVENT_HEAD_TAB;

typedef struct
{
	unsigned short EventNo;			//事件号
	unsigned short UpState;			//事件产生及上报状态	//17.03.23
	unsigned short CuntAddr;		//事件总次数地址
	unsigned short TimeAddr;		//事件累计时间地址
  	unsigned short SaftInfo;		//安全模式参数			//17.04.09	
	unsigned short TimeValidFlag;	//事件累计时间有效标志	0：无效，永远返回0	1：有效，实时累加
	unsigned short PtAddr;			//当前记录首地址指针(该记录所占用的FLASH空间内的相对地址)
	unsigned char OAD[4];
	unsigned short Class_Id;		//类别
	unsigned short StartAddr;		//记录起始地址:FLASH起始页号												
	unsigned short EndAddr;			//记录结束地址(占用FLASH页数)												
	unsigned short CapNumAddr;		//曲线捕获项数地址
	unsigned short CapNumMax;		//曲线最大捕获项数(物理空间允许接收的项数)
	unsigned short RecdLenAddr;		//每条记录长度地址(根据捕获内容得到)
	unsigned short CapObjAddr;		//捕获项地址
	unsigned short EntriesUseAddr;	//有效条目数地址，该数最大为允许读取的总记录条数，超过后不再增加。记录清零时只需将其清零即可
	unsigned short EntriesAddr;		//逻辑最大条目数地址(读取时最大条目数)
	unsigned short EReportFlagAddr;	//上报标识地址
	unsigned short EValidFlagAddr;	//有效标识地址
	unsigned short ECurRecNoAddr;	//当前记录号地址
	unsigned short EFinalRecNoAddr;	//最新记录号地址,内容用于表示事件记录序号
	unsigned short InitCapNumber;	//曲线初始化捕获项数
	unsigned char* InitCapObjTab;	//曲线初始化捕获项表格
	unsigned short EventHeadNumber;	//事件头捕获项数
	INI_EVENT_HEAD_TAB* EventHeadTab;	//事件头捕获项表格
}DL698_EVENT_PROFILE_TAB;	

typedef struct
{
	unsigned short PtAddr;			//当前记录首地址指针(该记录所占用的FLASH空间内的相对地址)
	unsigned short CapNumAddr;		//曲线捕获项数地址
  	unsigned short SaftInfo;		//安全模式参数			//17.04.09	
	unsigned char OAD[4];
	unsigned short CapNumMax;		//曲线最大捕获项数(物理空间允许接收的项数)
	unsigned short RecdLenAddr;		//每条记录长度首地址(逐条对应于捕获内容得到)
	unsigned short CapObjAddr;		//捕获项地址
	unsigned short EntriesUseAddr;	//有效条目数首地址，该地址列对应数最大为允许读取的总记录条数，超过后不再增加。记录清零时只需将其清零即可
	unsigned short FinalRecNoAddr;	//最新记录号地址,内容用于表示事件记录序号
	unsigned short InitCapNumber;	//曲线初始化捕获项数
	unsigned char* InitCapObjTab;	//曲线初始化捕获项表格
}DL698_FREEZE_PROFILE_TAB;	

typedef struct
{
	unsigned short CapNumAddr;		//显示项数地址
	unsigned short CapNumMax;		//显示最大项数(物理空间允许接收的项数)
  	unsigned short SaftInfo;		//安全模式参数			//17.04.09	
	unsigned char OAD[4];
	unsigned short CapObjAddr;		//显示项地址
	unsigned short PeriodAddr;
	unsigned short InitCapNumber;	//显示初始化项数
//	unsigned char* InitCapObjTab;	//显示初始化项表格
	const INI_DISPLAY_TAB* InitCapObjTab;	//显示初始化项表格
}DL698_DISPLAY_PARA_TAB;	
*/
/*
typedef struct
{
	unsigned short CurRecNo;		//当前记录号
	unsigned short Space;			//冻结间隔
	unsigned char OAD[4];
	unsigned short RecdCunt;		//存储空间最大记录数
    unsigned short AvailRecs;		//当前已存储的有效记录数
	unsigned short RecdLenth;		//记录长度
	unsigned short ChannelNo;		//通道号
	unsigned char  BakFlag;			//是否有备份标志0：无备份；1有备份，每项数据带1字节校验和
}FILECURVEPARA;
*/				//16.10.12
typedef struct
{
	unsigned short CurRecNo;		//当前记录号
	unsigned short Space;			//冻结间隔
	unsigned char OAD[4];
	unsigned short MaxCunt;			//存储空间最大记录数
	unsigned short RecdCunt;		//存储空间最大记录数，去除4K轮转池后
    unsigned short AvailRecs;		//当前已存储的有效记录数
	unsigned short  Pages;			//存储空间所占用页数
	unsigned short  RecsInPage;		//每页记录数
	unsigned short RecdLenth;		//记录长度
	unsigned short ChannelNo;		//通道号
	unsigned short DirNo;			//文件目录序号
	unsigned short StartSectorNo;	//起始索引号
	unsigned char  BakFlag;			//是否有备份标志0：无备份；1有备份，每项数据带1字节校验和
}FILECURVEPARA;

typedef struct
{
	unsigned short DirAddr;
	unsigned short StartSector;
	unsigned short Sectors;
}FILEPARA;

typedef struct
{
	long* SumPower;
	unsigned char* RealPower;
	unsigned char* AverPower;
}POWERCHECKPARA;

//BIT0表示最近一次事件记录
typedef struct						//17.03.24					
{
	unsigned short StartState;		//事件发生状态		0：未发生 1：已发生
	unsigned short StartUpState;	//事件发生上报状态  0：未上报 1：已上报
	unsigned short EndState;		//事件结束状态		0：未发生 1：已发生
	unsigned short EndUpState;		//事件结束上报状态	0：未上报 1：已上报
}EVENT_UPSTATE;
//-----------------------------------------------------------------------------
//---------------------------------------------------------------------------------
//------------------DLMS---------------------------------------------------------------
//DATA通讯表格
typedef struct
{
	unsigned char* 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	DataType;
	unsigned char	State;				//保留
}DLMS_RAM_DATATAB;

//DATA通讯表格
typedef struct
{
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	DataType;
	//unsigned char	State;				//保留
	unsigned short 	State;
	unsigned short	Acess;
}DLMS_DATATAB;
//需量通讯表格
typedef struct
{
	unsigned short 	Addr;
	unsigned short 	MDTAddr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	Unit;
	unsigned char	State;				//保留
}DLMS_MDTAB;

typedef struct
{
	unsigned short 	Time_Addr;
	unsigned short 	Date_Addr;
  	unsigned short 	MaxVal_Addr;
	unsigned short 	Cum_Addr;
	unsigned short	ContCum_Addr;
}DLMS_MAXKW;

typedef struct
{
	unsigned char	Demand_P[5];
	unsigned char	Demand_PFee[5];	
	unsigned char	Demand_Pp[5];
	unsigned char	Demand_PpFee[5];
	unsigned char	Demand_Pn[5];
	unsigned char	Demand_PnFee[5];
	unsigned char	Demand_Qp[5];
	unsigned char	Demand_QpFee[5];
	unsigned char	Demand_Qn[5];
	unsigned char	Demand_QnFee[5];
#if(MeterPhase==Threephase)
	unsigned char	Demand_Q10[5];
	unsigned char	Demand_Q10Fee[5];
	unsigned char	Demand_Q20[5];
	unsigned char	Demand_Q20Fee[5];
	unsigned char	Demand_Q30[5];
	unsigned char	Demand_Q30Fee[5];
	unsigned char	Demand_Q40[5];
	unsigned char	Demand_Q40Fee[5];
#endif
	unsigned char	Demand_PT[7];
	unsigned char	Demand_PTFee[7];	
	unsigned char	Demand_PpT[7];
	unsigned char	Demand_PpTFee[7];
	unsigned char	Demand_PnT[7];
	unsigned char	Demand_PnTFee[7];
	unsigned char	Demand_QpT[7];
	unsigned char	Demand_QpTFee[7];
	unsigned char	Demand_QnT[7];
	unsigned char	Demand_QnTFee[7];
#if(MeterPhase==Threephase)
	unsigned char	Demand_Q10T[7];
	unsigned char	Demand_Q10TFee[7];
	unsigned char	Demand_Q20T[7];
	unsigned char	Demand_Q20TFee[7];
	unsigned char	Demand_Q30T[7];
	unsigned char	Demand_Q30TFee[7];
	unsigned char	Demand_Q40T[7];
	unsigned char	Demand_Q40TFee[7];
#endif

}Demand_Reg;

//需量寄存器通讯表格
typedef struct
{
//	unsigned short 		MDEAds;					//需量值地址
//	unsigned short 		MDTEAds;				//需量发生时间地址	
    unsigned char*  MDEAds;
	unsigned char*	MDTEAds;
	unsigned char 	Logical_Name[6];
//	unsigned short* 	MDBufAds;				//需量结构首址
	unsigned short 		EperiodAds;				//滑差时间地址
	unsigned short 		Enumber_of_periodsAds;  //周期数地址 
}DLMS_DEMAND_REGTAB;

typedef struct
{
	unsigned short MDBuf[60];			//滑差时间内脉冲数缓存区
	unsigned short MD_Cum;				//滑差时间内脉冲数						
	
	unsigned long current_average_value;
	unsigned long last_average_value;
	unsigned char capture_time[12];
	unsigned char start_time_current[12];
	unsigned short period;				//滑差时间参数
	unsigned short period_counter;		//滑差时间内秒计数器
	unsigned char number_of_periods;
	
//总需量缓存区计数器在费率切换时不清零，费率需量缓存区计数器在费率切换时清零
	unsigned char MD_Cunt;				//计算需量时需要向前调取整滑差区间脉冲缓存区个数
	unsigned char MD_CuntT;				//计算费率需量时需要向前调取整滑差区间脉冲缓存区个数
	unsigned char MD_BufAds;			//当前需量缓存区地址
	unsigned char Status;
	unsigned char FeeNo;				//需量费率号
	unsigned char State1;
	unsigned char State2;
}MDBUFF;
//电量通讯表格
typedef struct
{
	unsigned char* ECRAds;
	unsigned short ECFAds;
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	Unit;
	unsigned char	State;				//保留
}DLMS_ECTAB;

//变量通讯表格
typedef struct
{
	unsigned char* 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned char	State;				//保留
}DLMS_REALTAB;
//初始化负荷曲线捕获参数表格
typedef struct
{
	unsigned short 	  ClassId;
	unsigned char 	Logical_Name[6];
  unsigned char 	Attribute;
  unsigned char 	DataLen;				//该项数据长度
}INI_PROFILE_TAB;

//单操作时间表通讯表格
typedef struct
{
	unsigned short 	Addr;
  unsigned short 	ComInfo;
	unsigned char Logical_Name[6];
	unsigned char	State1;				//保留
	unsigned char	State2;				//保留
}DLMS_SIGLE_ACTTAB;
//脚本通讯表格
typedef struct
{
	unsigned short 	NumAddr;			//脚本数
  	unsigned short 	ComInfo;			
	unsigned char 	Logical_Name[6];
	unsigned short 	IdentAddr;			//脚本标识符1首址
	unsigned short 	ActsNumAddr;		//脚本标识符1操作说明数首址
	unsigned short 	ActsAddr;			//脚本标识符1操作说明区
  	unsigned short 	Len;				//脚本标识符操作说明区长度
	unsigned char	State1;				//保留
	unsigned char	State2;				//保留
}DLMS_SCRIPTPARATAB;
//脚本通讯表格
typedef struct
{
	unsigned short 	Script_No;			//脚本号
	unsigned char 	Logical_Name[6];
	unsigned short 	Script_Ident;		//脚本标识符
}DLMS_SCRIPTTAB;
//Cosem参数表格
typedef struct
{
	unsigned short 	ClassId;
  	unsigned char 	AttributeNum;
	unsigned char 	MethodNum;
  	unsigned char 	RListLen;
  	unsigned char 	WListLen;
}COSEM_PARATAB;

typedef struct
{
  	unsigned char 	AccessMode;			//属性访问方式
  	unsigned char 	DataType;			//属性数据类型
  	unsigned char 	AllDataLen;			//属性数据长度，包括数据类型等
  	unsigned char 	DataLen;			//纯粹数据长度
}ATTRIBUTE_PARATAB;
//寄存器激活类通讯表格
typedef struct
{
	unsigned short 	Addr;
  unsigned short  ComInfo;
	unsigned char Logical_Name[6];
	unsigned short 	State1; 			//保留
	unsigned short 	State2; 			//保留
}DLMS_REG_ACTIVATIONTAB;
typedef struct
{
	unsigned  short PtAddr;			//当前记录首地址指针(该记录所占用的FLASH空间内的相对地址)
	unsigned char Logical_Name[6];
	unsigned long StartAddr;			//记录起始地址:FLASH起始页号												
	unsigned short EndAddr;			//记录结束地址(占用FLASH页数)												
	unsigned short CapNumAddr;		//曲线捕获项数地址
	unsigned short CapNumMax;			//曲线最大捕获项数(物理空间允许接收的项数)
	unsigned short RecdLenAddr;		//每条记录长度地址(根据捕获内容得到)
//	unsigned short EntriesMaxAddr;	//物理允许最大条目数地址(根据物理空间和每条记录长度计算得到)
	unsigned short CapObjAddr;		//捕获项地址
	unsigned short PeriodAddr;		//捕获周期地址
	unsigned short SortMethodAddr;	//排序方法地址
	unsigned short SortObjectAddr;	//排序对象地址
	unsigned short EntriesUseAddr;	//有效条目数地址，该数最大为允许读取的总记录条数，超过后不再增加。记录清零时只需将其清零即可
	unsigned short EntriesAddr;		//逻辑最大条目数地址(读取时最大条目数)
	unsigned short ECurRecNoAddr;		//当前记录号地址
	unsigned short InitCapNumber;		//曲线初始化捕获项数
	unsigned char* InitCapObjTab;	//曲线初始化捕获项表格
}DLMS_PROFILETAB;	
//寄存器通讯表格
typedef struct
{
	unsigned short 	Addr;
  	unsigned short 	ComInfo;
	unsigned char 	Logical_Name[6];
	unsigned char	DataType;
	unsigned char	Scaler;
	unsigned char	Unit;
	unsigned short	State;				//保留
}DLMS_REGISTERTAB;

//初始化费率号定义..
typedef struct
{	
	unsigned char 	FEE_OCTET_STRING[5];
//	unsigned char	FEE_OCTET_STRING;
	unsigned char   state1;						//保留..	
 }INI_FEE_TAB;
 //寄存器监视通讯表格
 typedef struct
 {
	 unsigned short	 Addr;
	 unsigned short	 ComInfo;
	 unsigned char	 Logical_Name[6];
	 unsigned char	 State1;			 //保留
	 unsigned char	 State2;			 //保留
 }DLMS_REG_MONITORTAB;
 
 //累计需量通讯表格
 typedef struct
 {
	 unsigned short	 Addr;
	 unsigned short	 ComInfo;
	 unsigned char	 Logical_Name[6];
	 unsigned char	 Unit;
	 unsigned char	 State; 			 //保留
 }DLMS_CUMDTAB;
 typedef struct
 {
	 //判断..			 
	 unsigned char	Ram_Auto_Reclosing_relay_Num_Normal;					 //需量超限后自动重合闸次数(2)			 Data::Unsigned8
	 unsigned short   Ram_Reconnection_timeout_Normal; 							 //需量超限后到重合闸延时时间(1)		 Register::Unsigned8 --分钟..
	 unsigned char	Ram_Auto_Reclosing_relay_Num_emergency; 			 //需量超限后自动重合闸次数(紧急)(2)	 Data::Unsigned8
	 unsigned short   Ram_Reconnection_timeout_emergency;						 //需量超限后到重合闸延时时间(紧急)(1)	 Register::Unsigned8--分钟..
	 //实际运行记录..
	 unsigned char	Ram_Auto_Reclosing_relay_Num_cnt;							 //需量超限后自动重合闸次数(2)
	 unsigned short   Ram_Reconnection_timeout_Normal_cnt; 					 //需量超限后到重合闸延时时间(3)		 Register::Unsigned16 --分钟*60--秒做判断
	 unsigned char	Ram_Auto_Reclosing_relay_Num_emergency_cnt; 	 //需量超限后自动重合闸次数(紧急)(2)	 Data::Unsigned8
	 unsigned short   Ram_Reconnection_timeout_emergency_cnt;				 //需量超限后到重合闸延时时间(紧急)(3)	 Register::Unsigned16--分钟*60--秒做判断
 
 
	 unsigned long	 Ram_Limiter_monitored_value;			 //取监视值..
	 
	 //激活限制值..
	 unsigned char	 Limiter_monitored_NO;							 //判断取哪个值做为判断条件 0--总有功(正+反)功率.. 1--正向有功功率
	 
	 unsigned short		 Limiter_RelayMode; 							 //继电器模式使用判断..
	 unsigned short	LastTime_Limiter_RelayMode; 			 //保存上次的继电器模式使用判断..
	 unsigned long	 Ram_Limiter_threshold_active;			 //激活域值..
	 unsigned long	 Ram_Limiter_min_over_threshold_duration;	 //4字节越限判断时间..	 
	 unsigned long	 Ram_Limiter_min_under_threshold_duration;	 //4字节恢复判断时间..		 
	 
	 //功率,需量..
	 unsigned short	 AvePowerTime;			 //1分钟平均功率计算起始时间(上电后15分钟后才开始计算)
	 unsigned long	 AvePower_Total;	 //有功总功率
	 unsigned long	 AvePower_Pp;			 //有功正向功率
	 
	 //事件判断产生..
	 //事件判断寄存器..
	 unsigned long	 over_threshold_duration_cnt;				 //判断需量越限时间..
	 unsigned long	 min_under_threshold_duration_cnt;		 //判断需量恢复越限时间..
	 
   BYTE_STRUCT	 flg_Threshold_MDEvent1;	   //发生限制值事件..
	 BYTE_STRUCT   flg_Threshold_MDEvent1BK;	 //发生限制值上次状态..
	 
	 BYTE_STRUCT   Threshold_MDEvent;			 //发生限制值事件(标注开始和结束)..
	 
	 BYTE_STRUCT   LAST_OVER_Event; 				 //判断事件合闸持续时间是否到0x11--开始,其余结束..
   
 
 }LIMITER;
 typedef struct
{	
	//判断值//quality_threshold_judge..
	unsigned short 	Ram_Threshold_for_voltage_sag;    		//1,	0,	12,	31,	0,	255,D_Unsigned16,	0xFF,	Unit_V,	0,//欠压阀值(3)	Register::Unsigned16  
	unsigned short 	Ram_Threshold_for_voltage_swell;			//1,	0,	12,	35,	0,	255,D_Unsigned16,	0xFF,	Unit_V,	0,//过压阀值(3)	Register::Unsigned16  
	unsigned short 	Ram_Threshold_for_missing_voltage;		//1,	0,	12,	39,	0,	255,D_Unsigned16,	0xFF,	Unit_V,	0,//失压阀值(3) Register::Unsigned16  
	
	unsigned short 		Ram_Time_threshold_for_voltage_sag;		//1,	0,	12,	43,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,//欠压判断持续时间(3)   	Register::Unsigned16  
	unsigned short 		Ram_Time_threshold_for_voltage_swell;	//1,	0,	12,	44,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,//过压判断持续时间(3)       Register::Unsigned16
	unsigned short 		Ram_Time_threshold_for_voltage_cut;		//1,	0,	12,	45,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,//失压判断持续时间(3)       Register::Unsigned16
	unsigned short	 	Ram_Time_threshold_for_long_power_failure;	//0,0,96,7,20,255,D_Unsigned16,	0x00,	Unit_s,	0,	//长时间电源故障持续时间阀值(5)	Register::Unsigned32 //2013.6.19改2个字节..   
	
	//事件记录..                                                	                                               
	unsigned short Ram_Duration_of_last_voltage_sag_in_phase_L1;				//1,	0,	32,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//A相欠压持续时间(3)    	Register::Unsigned16  
	unsigned short Ram_Duration_of_last_voltage_swell_in_phase_L1;			//1,	0,	32,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//A相过压持续时间(3)    	Register::Unsigned16  
	                                                	                                               
///	unsigned short Ram_Duration_of_last_voltage_sag_in_phase_L2;				//1,	0,	52,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//B相欠压持续时间(3)    	Register::Unsigned16   
///	unsigned short Ram_Duration_of_last_voltage_swell_in_phase_L2;			//1,	0,	52,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,  //B相过压持续时间(3)    	Register::Unsigned16  
	                                                	                                               
///	unsigned short Ram_Duration_of_last_voltage_sag_in_phase_L3;				//1,	0,	72,	33,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,	//C相欠压持续时间(3)    	Register::Unsigned16   
///	unsigned short Ram_Duration_of_last_voltage_swell_in_phase_L3;			//1,	0,	72,	37,	0,	255,D_Unsigned16,	0x00,	Unit_s,	0,  //C相过压持续时间(3)    	Register::Unsigned16  
	
	unsigned short Ram_Short_power_failure_L1;							//A相短时间电源故障持续时间(5)			Register::Unsigned32         
///	unsigned short Ram_Short_power_failure_L2;							//B相短时间电源故障持续时间(5)			Register::Unsigned32           
///	unsigned short Ram_Short_power_failure_L3;							//C相短时间电源故障持续时间(5)			Register::Unsigned32       

	unsigned short Ram_Short_power_failure_any;						//任意相短时间电源故障持续时间(5)		Register::Unsigned32
	
	unsigned short Ram_Duration_of_long_power_failure_in_phase_L1;							//0,	0,	96,	7,	16,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //A相长时间电源故障持续时间(5)	Register::Unsigned32         
///	unsigned short Ram_Duration_of_long_power_failure_in_phase_L2;							//0,	0,	96,	7,	17,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //B相长时间电源故障持续时间(5)	Register::Unsigned32         
///	unsigned short Ram_Duration_of_long_power_failure_in_phase_L3;							//0,	0,	96,	7,	18,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //C相长时间电源故障持续时间(5)	Register::Unsigned32     
	
	unsigned short Ram_Duration_of_long_power_failure_in_any_phase;						//0,	0,	96,	7,	19,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //任意相长时间电源故障持续时间(5)Register::Unsigned32
		
	unsigned short Ram_Total_duration_of_long_power_failure_in_phase_L1;				//0,	0,	96,	7,	22,	255,D_Unsigned32,	0x00,	Unit_s,	0,	//A相长时间电源故障总持续时间(5)Register::Unsigned32   
///	unsigned short Ram_Total_duration_of_long_power_failure_in_phase_L2;				//0,	0,	96,	7,	23,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //B相长时间电源故障总持续时间(5)Register::Unsigned32   
///	unsigned short Ram_Total_duration_of_long_power_failure_in_phase_L3;				//0,	0,	96,	7,	24,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //C相长时间电源故障总持续时间(5)Register::Unsigned32   单位：s

	unsigned short Ram_Total_Duration_of_long_power_failure_in_any_phase;			//0,	0,	96,	7,	19,	255,D_Unsigned32,	0x00,	Unit_s,	0,  //任意相长时间电源故障持续时间(5)Register::Unsigned32

	//纪录电压值..
	unsigned short Ram_Magnitude_of_last_voltage_sag_in_phase_L1;						//欠压最低电压统计
	unsigned short Ram_Magnitude_of_last_voltage_swell_in_phase_L1;					//欠压最高电压统计
///	unsigned short Ram_Magnitude_of_last_voltage_sag_in_phase_L2;						//欠压最低电压统计
///	unsigned short Ram_Magnitude_of_last_voltage_swell_in_phase_L2;					//欠压最高电压统计
///	unsigned short Ram_Magnitude_of_last_voltage_sag_in_phase_L3;						//欠压最低电压统计
///	unsigned short Ram_Magnitude_of_last_voltage_swell_in_phase_L3;					//欠压最高电压统计
		
	//事件判断寄存器..
	unsigned short    phsreverseCNT;													//电压逆向序..
	unsigned short    MissingNeutralCNT;       								//掉零线电流检测..
	unsigned short    guoyaCNT_a,duanxiangCNT_a,qianyaCNT_a,duanxianglongCNT_a;
/// 	unsigned short    guoyaCNT_b,duanxiangCNT_b,qianyaCNT_b,duanxianglongCNT_b;
///	unsigned short    guoyaCNT_c,duanxiangCNT_c,qianyaCNT_c,duanxianglongCNT_c;
    unsigned char   Deal_EVENT_NO;
  
  //继电器跳闸失败检测:是否还会有电流产生..
    unsigned short    DisconnectFailureCNT;
  
  //增加超过长时间掉电恢复才做事件1记录..
    unsigned char   Delay_TimeFlag[3];   //代表不同的三相定义..
  
  //电压合格率固定累计记录时间--10分钟..
    unsigned short Volt_RecordTime;
  
/*-----------------------------------------------------------------------------------------------------*/
  WORD_STRUCT    flgEvent1;		         //过压事件记录控制字..A--BIT0;B--BIT1;C--BIT2;

  WORD_STRUCT    flgEvent1BK;          //事件记录控制字..

  BYTE_STRUCT    guoyaEvent,qianyaEvent,duanxiangEvent,PowerFailureEvent,PowerFailurelongEvent;
 
  WORD_STRUCT    flgEvent2;		         //其它事件：掉零线等..
  WORD_STRUCT    flgEvent2BK;          //事件记录控制字..
 
  //电压平均值..
    unsigned char  AveVoltTime;
 	unsigned short   AveVoltUa;   //A相电压平均值..
/// 	unsigned short   AveVoltUb;   //B相电压平均值..
///	unsigned short   AveVoltUc;   //C相电压平均值..
	//电压平均值计算BUFF..
 	unsigned short   AveVoltUaBuff[10];   //A相电压平均值..
/// 	unsigned short   AveVoltUbBuff[10];   //B相电压平均值..
///	unsigned short   AveVoltUcBuff[10];   //C相电压平均值..
	
	//短长停电事件次数及时间临时记录变量..
	unsigned short   fail_short_times;
	unsigned short   fail_long_times;			 //			
	unsigned short  fail_total_seconds;
	unsigned short  temp_long_powerfailure_any;
	unsigned short  temp_long_powerfailure_L1;
///	unsigned short  temp_long_powerfailure_L2;
///	unsigned short  temp_long_powerfailure_L3;		
}QUALITY_EVENT;
 typedef struct
 {
	 unsigned char RegAddr;
	 unsigned char Length;
 }MEASREG;
 typedef struct
 {
	 unsigned char MarSun;
	 unsigned char OctSun;	 
 }LASTSUNDAY;
 
 typedef struct
 {
	unsigned short Dis_Num;
	unsigned short Class_id;
	unsigned char  OBIS_Num[6];
	unsigned char  Attribute;
	unsigned short Index;
 } DISPLAYNUM;
 
 typedef struct
{
//	unsigned char NeedRecCnt;			//需要接收的数据长度//
//	unsigned char NeedSendCnt;		//需要发送的长度//
	unsigned char MeasBuf_Tx[10];	//收发送缓冲区//	
	unsigned char MeasBuf_Rx[10];	//收发送缓冲区//
//	unsigned char CommOk;			//通信完成标志，0X55=通信完成//
	unsigned char Tx_Ptr;			//8207通信时的指针//		
	unsigned char Cmd;			//8207通信时的指针//	
	unsigned char Rx_Ptr;			//8207通信时的指针//
	unsigned char CommDly;			//通信定时，超过这个时间表明通信出错//
//	unsigned char ReadPowerState;
	unsigned char MeasState;
	unsigned char Comm_Enable;
	unsigned char Comm_Ptr;

}COMM_Measure;
 //双核的Uart通信//
 typedef struct
 {
	 unsigned char NeedRecCnt;			 //需要接收的数据长度//
	 unsigned char NeedSendCnt; 	 //需要发送的长度//
	 unsigned char RecBuf8213[100];  //接收缓冲区//
	 unsigned char SendBuf8213[100]; //发送缓冲区//
	 unsigned char PhaType; 		 //相别，1=A相，2=B相，3=C相//
	 unsigned char Tx_Ptr;			 //8207通信时的指针//		 
	 unsigned char Rx_Ptr;			 //8207通信时的指针//
	 
	 unsigned char Cmd; 		 //8207通信时的指针//	 
	 unsigned char CommOk;			 //通信完成标志，0X55=通信完成//
	 unsigned char Ptr8213; 		 //8207通信时的指针//
	 unsigned char CommDly; 		 //通信定时，超过这个时间表明通信出错//
	 unsigned char ReadPowerState;
	 
 }COMM8213;

 typedef struct
 {
	 unsigned char ReadCmd;			 //寄存器地址
	 unsigned char* ReadData; 	 //返回数据值
 }HT7017_Measure;

 typedef struct
 {
 	unsigned short 	Cmd;
	unsigned char length;
	unsigned char Comm_Enable;
	unsigned char Real_Ptr;			
	unsigned char EC_Ptr; 
	unsigned char Demand_Ptr; 
	unsigned char Data_Select; 
 }Double_Commu;

  typedef struct
 {
	unsigned char* ECRAds;
 //	unsigned short 	Addr;
	unsigned short  DataID;		 //ID码
//	unsigned char 	Logical_Name[6];
	unsigned char	len;
 } ID_OBIS;
 typedef struct
 {
 	unsigned short 	Addr;
	unsigned short  DataID;		 //ID码	
	unsigned short 	Time_Addr;
	unsigned short  TimeID;		 //ID码
	unsigned char 	Logical_Name[6];
	unsigned char	len;
 } ID_MAXDemand_OBIS;
 typedef struct
 {
	 unsigned char HT7017_U[4]; 
	 unsigned char HT7017_Ia[4]; 
	 unsigned char HT7017_Ib[4]; 	 
	 unsigned char HT7017_Hz[4]; 
	 unsigned char HT7017_P1[4]; 
	 unsigned char HT7017_Q1[4]; 
	 unsigned char HT7017_S1[4]; 
	 unsigned char HT7017_P2[4]; 
	 unsigned char HT7017_Q2[4]; 
	 
	 unsigned char HT7017_P[4]; 
	 unsigned char HT7017_Q[4]; 
	 unsigned char HT7017_EMU_Status[4]; 

	 unsigned char HT7017_ID[4]; 
	 unsigned char HT7017_I2Gain[4]; 
	 unsigned char HT7017_HFConst[4]; 
	 unsigned char HT7017_P1Gain[4]; 
	 unsigned char HT7017_Gphs1[4]; 
	 unsigned char HT7017_OffsetH[4]; 
	 unsigned char HT7017_OffsetL[4]; 
	 unsigned char HT7017_P2Gain[4]; 
	 unsigned char HT7017_Gphs2[4]; 
	 
 }HT7017_Meas;
 
typedef struct
{
	 long APowerPulP;
	 long BPowerPulP;
	 long SPowerPulP;
	 long APowerPulQ;
	 long BPowerPulQ;

	unsigned long Ia;
	unsigned long Ib;
//---------增益系数-----------
	unsigned long Urms_K;
	unsigned long Irms1_K;
	unsigned long Irms2_K;
	unsigned long Power1_K;
	unsigned long Power2_K;
	unsigned long PowerS_K;

}MEASURE7053;
//用户权限管理脚本表格..
typedef struct
{
	unsigned char		Class[2];
	unsigned char 	OBIS_Name[6];     //例如：00 01（类：ID） 00 00 2A 00 00 FF （逻辑设备名：6个字节） 02（属性）
	unsigned char	  State_Reserve1;	  //保留1字节空间..
	unsigned char	  State_Reserve2;	  //保留1字节空间..
}OBIS_TAB;
//用户权限管理脚本表格..
typedef struct
{
	unsigned char 	OBIS_Name[9];     //例如：00 01（类：ID） 00 00 2A 00 00 FF （逻辑设备名：6个字节） 02（属性）
	unsigned char	  State_Reserve1;	  //保留1字节空间..
}CLIENT_TAB;

