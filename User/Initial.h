
void HWTime( unsigned short ParaWord );
void fnTarget_Init(void);
void PwrOnInit(void);
void PwrDnInit(void);
void VarInit(void);
void Bat_On( void );
//void Bat_Off( void );
void SCFQ4M( void );
void SCFQ1M( void );
void PwrOnInitTest( void );			//V32

void GetFRamECPulse( void );

void HW_ON( void );
void HW_OFF( void );

void GetFRamEC( void );							//10.06.22

void CheckSetClose2( void );					//10.11.05
void CheckSetClose4( void );					//10.11.05
void CheckPassWd_ErrCnt2( void );				//10.11.05
void CheckPassWd_ErrCnt4( void );				//10.11.05
unsigned short DoCrc16(unsigned short RegInit, unsigned char *message, unsigned short len) ;

//#if( RTCCHIP == RX8025T )						//10.12.04
void RepeatHWTime( unsigned short Temp );
//#endif
void EnterHold(void);
void ExitHold(void);

#if ( LocalSmartMeter == YesCheck )		//17.04.22
void InitialRConsumECSum( void );									//11.07.14	
#endif
