#ifndef __MEASURE_H__
#define __MEASURE_H__

//校表参数寄存器列表
#define COMChecksum      0x17
#define SUMChecksum      0x18
#define EMUSR            0x19
#define SYSSTA           0x1A
#define IDCode           0x1B
//#define WPREG            0x32
#define ANAEN            0x43
#define	GP1              0x50
#define	GQ1              0x51
#define	GS1              0x52
#define	GPhs1            0x6D 
#define	GPhs2            0x6E
#define	ADCCON           0x59
#define	P1OFFSET         0x76
#define	P2OFFSET         0x77
#define	Rms_U            0x08
#define	Rms_I1           0x06
#define	Rms_I2           0x07
#define	PowerP1          0x0A
#define	PowerQ1          0x0B
#define	PowerS1          0x0C
#define	EnergyP          0x0D
#define	EnergyQ          0x0E

#define	PowerP2          0x10
#define	PowerQ2          0x11
#define	UFREQ            0x09



#define EMUIE_Address    0x30
#define EMUIF_Address    0x31
#define WPREG_Address    0x32
#define SRSTREG_Address  0x33

#define EMUCFG_Address   0x40
#define FreqCFG_Address  0x41
#define ModuleEn_Address 0x42
#define ANAEN_Address    0x43
#define IOCFG_Address    0x45

#define GP1_Address      0x50
#define GQ1_Address      0x51
#define GS1_Address      0x52
#define GP2_Address      0x54
#define GQ2_Address      0x55
#define GS2_Address      0x56

#define QPhsCal_Address  0x58
#define ADCCON_Address   0x59

#define I2Gain_Address   0x5B
#define I1Off_Address    0x5C
#define I2Off_Address    0x5D
#define UOff_Address     0x5E
#define PQStart_Address  0x5F
#define HFConst_Address  0x61
#define CHK_Address      0x62
#define IPTAMP_Address   0x63
#define Dec_Shift_Address  0x64
#define P1OFFSETH_Address  0x65
#define P2OFFSETH_Address  0x66
#define Q1OFFSETH_Address  0x67
#define Q2OFFSETH_Address  0x68
#define I1RMSOFFSET_Address  0x69
#define I2RMSOFFSET_Address  0x6A

#define URMSOFFSET_Address     0x6B
#define ZCrossCurrent_Address  0x6C
#define GPhs1_Address  0x6D
#define GPhs2_Address  0x6E
#define PFCnt_Address  0x6F
#define QFCnt_Address  0x70

#define ANACON_Address     0x72
#define SUMCHECKL_Address  0x73
#define SUMCHECKH_Address  0x74
#define MODECFG_Address    0x75
#define P1OFFSETL_Address  0x76
#define P2OFFSETL_Address  0x77
#define Q1OFFSETL_Address  0x78
#define Q2OFFSETL_Address  0x79
#define UPeakLvl_Address   0x7A
#define USagLvl_Address    0x7B
#define UCycLen_Address    0x7C

#define ATT7053Set	0x01	//ATT7053设置
#define ATT7053Read	0x02	//ATT7053读取
#define ATT7053CommOK	0x04	//ATT7053通信成功
#define F_PhaATT7053 0x01
#define F_Reset7053  0x02

#define PhPW4Imax    0x800000

#define Measur_Num 21
#define CommDelay  20  //10ms
#define Total_Fee 4
#define Measur_Delay 40000
#define Measur_Const 1000

#if( MEASCHIP == HT7017)
unsigned char UART_Write(  unsigned char Cmd, unsigned char* Data );

short HT7017Init(void);
void Energy_Read(void);
short Read_ATTValue( unsigned char cmd, unsigned char* Point , unsigned char length );
void Instan_Value(void);
unsigned char MasureData_Get(void);
unsigned char UART_ReadInstan(  unsigned char Cmd );

#endif
void Read_CMonEC( unsigned char* Dest, unsigned short ECRgAds );
void Read_CMonEC4( unsigned char* Dest, unsigned short ECRgAds );
void Read_CMonEC4_HEX_U( unsigned char* Dest, unsigned short ECRgAds );			//16.08.29
void Read_CMonEC4_HEX_S( unsigned char* Dest, unsigned short ECRgAds );			//16.08.29
void Read_CMonEC8_HEX_U( unsigned char* Dest, unsigned short ECRgAds );			//16.08.29
void Read_CMonEC8_HEX_S( unsigned char* Dest, unsigned short ECRgAds );			//16.08.29
short GetMQuad( unsigned char PWFlag, short Reg );

//void EC_Meas(void);

#endif
