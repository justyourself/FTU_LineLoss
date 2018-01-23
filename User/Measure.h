#ifndef __MEASURE_H__
#define __MEASURE_H__
#include "TypeRAM.h"

#if ( MEASCHIP == ATT7022E )

#define ModeCfg		0x01
#define PGACtrl		0x02
#define EMUCfg		0x03

#define PgainA		0x04
#define PgainB		0x05
#define PgainC		0x06
#define QgainA		0x07
#define QgainB		0x08
#define QgainC		0x09
#define SgainA		0x0A
#define SgainB		0x0B
#define SgainC		0x0C
#define PhsregA0	0x0D
#define PhsregB0	0x0E
#define PhsregC0	0x0F
#define PhsregA1	0x10
#define PhsregB1	0x11
#define PhsregC1	0x12

#define PoffsetA	0x13
#define PoffsetB	0x14
#define PoffsetC	0x15
#define QPhscal		0x16
#define UgainA		0x17
#define UgainB		0x18
#define UgainC		0x19
#define IgainA		0x1A
#define IgainB		0x1B
#define IgainC		0x1C

#define Istartup	0x1D
#define REGHFConst	0x1E
#define FailVoltage	0x1F

#define GainADC7	0x20

#define QoffsetA	0x21
#define QoffsetB	0x22
#define QoffsetC	0x23

#define UaRmsoffse	0x24
#define UbRmsoffse	0x25
#define UcRmsoffse	0x26
#define IaRmsoffse	0x27
#define IbRmsoffse	0x28
#define IcRmsoffse	0x29

#define UoffsetA	0x2A
#define UoffsetB	0x2B
#define UoffsetC	0x2C

#define IoffsetA	0x2D
#define IoffsetB	0x2E
#define IoffsetC	0x2F

#define EMUIE		0x30
#define ModuleCFG	0x31
#define AllGain		0x32
#define HFDouble	0x33
#define LineGain	0x34
#define PinCtrl		0x35
#define Pstart		0x36
#define Iregion		0x37

#if ( NEW7022E == YesCheck )
#define Iregion1	0x60

#define PhsregA2	0x61
#define PhsregB2	0x62
#define PhsregC2	0x63

#define PoffsetAL	0x64
#define PoffsetBL	0x65
#define PoffsetCL	0x66

#define QoffsetAL	0x67
#define QoffsetBL	0x68
#define QoffsetCL	0x69

#define ItRmsoffse	0x6A

#define TPSoffset	0x6B
#define TPSgain		0x6C

#define TCcoffA		0x6D
#define TCcoffB		0x6E
#define TCcoffC		0x6F

#define EMCfg		0x70
#define OILVL		0x71
#endif

//#define GCtrlTADC7	0x01
//#define UADCPga		0x02			
//#define EnergyAddMode	0x03


#define ATPWPA		0x01
#define ATPWPB		0x02
#define ATPWPC		0x03
#define ATPWPZ		0x04
#define ATPWQA		0x05
#define ATPWQB		0x06
#define ATPWQC		0x07
#define ATPWQZ		0x08
#define ATPWSA		0x09
#define ATPWSB		0x0A
#define ATPWSC		0x0B
#define ATPWSZ		0x0C
#define ATUrmsA		0x0D
#define ATUrmsB		0x0E
#define ATUrmsC		0x0F
#define ATIrmsA		0x10
#define ATIrmsB		0x11
#define ATIrmsC		0x12
#define ATIrmsZ		0x13
#define ATFactorA	0x14
#define ATFactorB	0x15
#define ATFactorC	0x16
#define ATFactorZ	0x17
#define ATAngleA	0x18
#define ATAngleB	0x19
#define ATAngleC	0x1A
//#define ATAngleZ	0x1B
#define ATFreq		0x1C
//#define ATSPIRBuff	0x28
#define ATSPIRBuff	0x2D
#define ATVoltFlag	0x2C
#define ATSPIWBuffA	0x2D
//#define ATSPIWBuffB	0x2E
#define ATState		0x1D
#define ATPA		0x1E
#define ATPB		0x1F
#define ATPC		0x20
#define ATPZ		0x21
#define ATQA		0x22
#define ATQB		0x23
#define ATQC		0x24
#define ATQZ		0x25

#define RmsADC7		0x29
#define ATTempD		0x2A		//10.08.03

#define ADJStart	0x80		//特殊命令
#define ClrADJData	0xC3
#define ATRest		0xD3

#define ATYUaUb		0x26
#define ATYUaUc		0x27
#define ATYUbUc		0x28

#define ATFstCntA	0x39		//新国网	//13.12.28
#define ATFstCntB	0x3A		//新国网	//13.12.28
#define ATFstCntC	0x3B		//新国网	//13.12.28

#define ATChkSum1	0x3E		//ATChk

#if ( NEW7022E == YesCheck )
#define ATChkSum2	0x5E		//ATChk
#endif

#else
#define Iregion1	0x02
#define Iregion2	0x03
#define Iregion3	0x04
#define Iregion4	0x05
#define PgainA0		0x06
#define PgainB0		0x07
#define PgainC0		0x08
#define PgainA1		0x09
#define PgainB1		0x0A
#define PgainC1		0x0B
#define PhsregA0	0x0C
#define PhsregA1	0x0D
#define PhsregA2	0x0E
#define PhsregA3	0x0F
#define PhsregA4	0x10
#define PhsregB0	0x11
#define PhsregB1	0x12
#define PhsregB2	0x13
#define PhsregB3	0x14
#define PhsregB4	0x15
#define PhsregC0	0x16
#define PhsregC1	0x17
#define PhsregC2	0x18
#define PhsregC3	0x19
#define PhsregC4	0x1A
#define UgainA		0x1B
#define UgainB		0x1C
#define UgainC		0x1D
#define Iregchg		0x1E
#define Istartup	0x1F
#define HFConst		0x20
#define IgainA		0x26
#define IgainB		0x27
#define IgainC		0x28
#define FailVoltage	0x29
#define EnergyAddMode	0x2A

#define GainADC7	0x2B
#define GCtrlTADC7	0x2C

#define HFDouble	0x3E
#define UADCPga		0x3F			//ATT7022B
#define PFlag		0x3D
#define EnUAngle	0x2E

#define EPgainA0	( 0x06 - 6 ) * 4
#define EPgainB0	( 0x07 - 6 ) * 4
#define EPgainC0	( 0x08 - 6 ) * 4
#define EPgainA1	( 0x09 - 6 ) * 4
#define EPgainB1	( 0x0A - 6 ) * 4
#define EPgainC1	( 0x0B - 6 ) * 4
#define EPhsregA0	( 0x0C - 6 ) * 4
#define EPhsregA1	( 0x0D - 6 ) * 4
#define EPhsregA2	( 0x0E - 6 ) * 4
#define EPhsregA3	( 0x0F - 6 ) * 4
#define EPhsregA4	( 0x10 - 6 ) * 4
#define EPhsregB0	( 0x11 - 6 ) * 4
#define EPhsregB1	( 0x12 - 6 ) * 4
#define EPhsregB2	( 0x13 - 6 ) * 4
#define EPhsregB3	( 0x14 - 6 ) * 4
#define EPhsregB4	( 0x15 - 6 ) * 4
#define EPhsregC0	( 0x16 - 6 ) * 4
#define EPhsregC1	( 0x17 - 6 ) * 4
#define EPhsregC2	( 0x18 - 6 ) * 4
#define EPhsregC3	( 0x19 - 6 ) * 4
#define EPhsregC4	( 0x1A - 6 ) * 4
#define EUgainA		( 0x1B - 6 ) * 4
#define EUgainB		( 0x1C - 6 ) * 4
#define EUgainC		( 0x1D - 6 ) * 4
#define EIgainA		( 0x26 - 14 ) * 4
#define EIgainB		( 0x27 - 14 ) * 4
#define EIgainC		( 0x28 - 14 ) * 4

#define ATPWPA		0x01
#define ATPWPB		0x02
#define ATPWPC		0x03
#define ATPWPZ		0x04
#define ATPWQA		0x05
#define ATPWQB		0x06
#define ATPWQC		0x07
#define ATPWQZ		0x08
#define ATPWSA		0x09
#define ATPWSB		0x0A
#define ATPWSC		0x0B
#define ATPWSZ		0x0C
#define ATUrmsA		0x0D
#define ATUrmsB		0x0E
#define ATUrmsC		0x0F
#define ATIrmsA		0x10
#define ATIrmsB		0x11
#define ATIrmsC		0x12
#define ATIrmsZ		0x13
#define ATFactorA	0x14
#define ATFactorB	0x15
#define ATFactorC	0x16
#define ATFactorZ	0x17
#define ATAngleA	0x18
#define ATAngleB	0x19
#define ATAngleC	0x1A
#define ATAngleZ	0x1B
#define ATFreq		0x1C
#define ATSPIRBuff	0x28
#define ATVoltFlag	0x2C
#define ATSPIWBuffA	0x2D
#define ATSPIWBuffB	0x2E
#define ATState		0x30
#define ATPA		0x31
#define ATPB		0x32
#define ATPC		0x33
#define ATPZ		0x34
#define ATQA		0x35
#define ATQB		0x36
#define ATQC		0x37
#define ATQZ		0x38
#define ATInPA		0x60
#define ATInPB		0x61
#define ATInPC		0x62
#define ATInPZ		0x63
#define ATOutPA		0x64
#define ATOutPB		0x65
#define ATOutPC		0x66
#define ATOutPZ		0x67
#define ATLPA		0x68
#define ATLPB		0x69
#define ATLPC		0x6A
#define ATLPZ		0x6B
#define ATCPA		0x6C
#define ATCPB		0x6D
#define ATCPC		0x6E
#define ATCPZ		0x6F

#define RmsADC7		0x29
#define ATTempD		0x2A		//10.08.03

#define ADJStart	0x80		//特殊命令
#define ClrADJData	0xC3
#define ATRest		0xD3

#define ATYUaUb		0x5C
#define ATYUaUc		0x5D
#define ATYUbUc		0x5E

#define ATChkSum1	0x3E		//ATChk

#endif
//ADE7758

#if ( MEASCHIP == ADE7758 )	  
#define	AWATTHR		0x01
#define BWATTHR		0x02
#define CWATTHR		0x03
#define AVARHR		0x04
#define BVARHR		0x05
#define CVARHR		0x06
#define AVAHR		0x07
#define BVAHR		0x08
#define CVAHR		0x09

#define AIRMS		0x0A
#define BIRMS		0x0B
#define CIRMS		0x0C

#define AVRMS		0x0D
#define BVRMS		0x0E
#define CVRMS		0x0F

#define FREQ		0x10
#define TEMP		0x11
#define WFORM		0x12
#define OPMODE		0x13
#define MMODE		0x14
#define WAVMODE		0x15
#define COMPMODE	0x16

#define LCYCMODE	0x17
#define MASK		0x18
#define STATUS		0x19
#define RSTATUS		0x1A
#define ZXTOUT		0x1B

#define LINCYC		0x1C
#define SAGCYC		0x1D
#define SAGLVL		0x1E
#define VPINTLVL	0x1F
#define IPINTLVL	0x20
#define VPEAK		0x21
#define IPEAK		0x22
#define GAIN		0x23


#define AVRMSGAIN	0x24
#define BVRMSGAIN	0x25
#define CVRMSGAIN	0x26

#define AIGAIN		0x27
#define BIGAIN		0x28
#define CIGAIN		0x29

#define AWG			0x2A
#define BWG			0x2B
#define CWG			0x2C

#define AVARG		0x2D
#define BVARG		0x2E
#define CVARG		0x2F

#define AVAG		0x30
#define BVAG		0x31
#define CVAG		0x32

#define AVRMSOS		0x33
#define BVRMSOS		0x34
#define CVRMSOS		0x35

#define AIRMSOS		0x36
#define BIRMSOS		0x37
#define CIRMSOS		0x38

#define AWATTOS		0x39
#define BWATTOS		0x3A
#define CWATTOS		0x3B

#define AVAROS		0x3C
#define BVAROS		0x3D
#define CVAROS		0x3E

#define APHCAL		0x3F
#define BPHCAL		0x40
#define CPHCAL		0x41

#define WDIV		0x42

#define VARDIV		0x43
#define VADIV		0x44

#define APCFNUM		0x45
#define APCFDEN		0x46

#define VARCFNUM	0x47
#define VARCFDEN	0x48
#define CHKSUM		0x7E
#define VERSION		0x7F

#endif

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
//#define	EnergyP          0x0D
//#define	EnergyQ          0x0E

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

#if ( MEASCHIP == ADE7758 )
void ADE7758WtReg(  unsigned char Cmd, unsigned char* Data );
void ADE7758RdReg(  unsigned char Cmd, unsigned char* Data );
void ADE7758Init(void);
void MeasVA( unsigned char Source, unsigned char* Dest, short* RefVA );
void MeasPW( unsigned char Source, unsigned char* Dest );
//void MeasPWZ( unsigned char Source, unsigned char* Dest );
//void MeasPWValue( unsigned short Temp, unsigned char* Dest );
void MeasPWSZ( void );
void MeasPWS( void );
void MeasPF( void);
short	GetFreqZone( unsigned char* Source );
void GetFreq( void );
void MeasFreq( void );
void NegRestrain( unsigned char* Source, unsigned char Num );
#else
//void ATT7022RdReg( unsigned char Cmd, unsigned char* Data  );
short ATT7022RdReg(  unsigned char Cmd, unsigned char* Data ,unsigned short Devads);				//11.08.03
void ATT7022WtReg( unsigned char Cmd, unsigned char* Data  ,unsigned short Devads);
unsigned long SetFailVoltage(unsigned short Devads);				//ATChk
void ATT7022Init(unsigned short Devads);
//void SetFailVoltage(void);
void SetEnergyAddMode(void);
#endif
void EC_Meas(void);
//void ECRgSum( short ECItem, short Type);
void Read_CMonEC( unsigned char* Dest, unsigned short ECRgAds );
//void FMBuf_RWAdd( short Dest, unsigned char* Src );
//short GetMQuad(void);
short MD_Meas( void );
//short CumBuffPls( short BuffAds );
//void MD_Comp( unsigned char* PointSrc, short MD_Ads, short MDT_Ads );
void MD_Comp( unsigned char* PointSrc, unsigned short MD_Ads );
void SMEcMease( void );
//void ECBuf_Read( unsigned char* Dest, unsigned short Src );
//void ECBuf_Write( unsigned short Dest, unsigned char* Scr );
void PhaseEcMease( void );
//short GetMQuad( unsigned char ActPwReg, unsigned char ReActPwReg);
short GetStablyQuad( void );
short GetMQuad( unsigned char PWFlag, short Reg );

void GetCompMode( void );
void ReCalFailVoltageChk( unsigned char* Source );				//ATChk
void GetPW( unsigned char ECType, unsigned char* Source, unsigned short PhaseNo );		//V70		//V1001

void GetCurrentPower(void);						//PWMD
void GetATT7022PQZ( void );							//PWMD
void GetAPW(void );							//PWMD

void WriteATByte( unsigned char Cmd );
void ATT7022Rd(unsigned char Cmd, unsigned char* Data, short Length ,unsigned short Devads);
void ATT7022WR(unsigned char Cmd, unsigned char* Data, short Length ,unsigned short Devads);
void GetSPIPara( SPIPARA* SPIPara, unsigned short Devads );
//void ECRgSum( short ECItem, unsigned short Data );
//void GetRamEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds );		 //读取RAM或FRAM中的电量
void CalculateCombEC( unsigned char* Dest, unsigned char* Source, unsigned char Mode, unsigned char Items ); //计算组合电量
short CombECJust( unsigned char* Dest, unsigned short ECRgAds ); 	//组合电量判断

void CombMD_Comp( unsigned char* PointSrc, unsigned short MD_Ads, unsigned char Dir );		//组合需量比较
void Read_CMonEC4( unsigned char* Dest, unsigned short ECRgAds );
void Read_CMonEC4_HEX_U( unsigned char* Dest, unsigned short ECRgAds );			//16.08.29
void Read_CMonEC4_HEX_S( unsigned char* Dest, unsigned short ECRgAds );			//16.08.29
void Read_CMonEC8_HEX_U( unsigned char* Dest, unsigned short ECRgAds );			//16.08.29
void Read_CMonEC8_HEX_S( unsigned char* Dest, unsigned short ECRgAds );			//16.08.29

void JustMDOverState( void );

void MDOverEventEnd( void );

#if( AllFail7022Chk == YesCheck )							//10.11.11
void ATT7022AmperInit(unsigned short Devads);
void GetAmper( void );
#endif
#define DELEY2		0x01
#define DELEY4		0x04
#define DELEY5		0x05
#endif
