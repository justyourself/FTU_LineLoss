
// BCD Calculation Subroutine
void _BCD1INC(unsigned char *Dest);
void _BCD2INC(unsigned char *Dest);
void _BCD3INC(unsigned char *Dest);
void _BCD4INC(unsigned char *Dest);
void _BCD1DEC(unsigned char *Dest);

void _BCD1ADD(unsigned char *Dest, unsigned char *Src);
void _BCD2ADD(unsigned char *Dest, unsigned char *Src);
void _BCD3ADD(unsigned char *Dest, unsigned char *Src);
void _BCD4ADD(unsigned char *Dest, unsigned char *Src);
void _BCD5ADD(unsigned char *Dest, unsigned char *Src);
void _BCD6ADD(unsigned char *Dest, unsigned char *Src);

void _BCD2SUB(unsigned char *Dest, unsigned char *Src);
void _BCD4SUB(unsigned char *Dest, unsigned char *Src);
void _BCD5SUB(unsigned char *Dest, unsigned char *Src);
void _BCD6SUB(unsigned char *Dest, unsigned char *Src);

extern void _BCD_ASC(unsigned char *Dest, unsigned char *Src, short Lenth);
extern void _ASC_BCD(unsigned char *Dest, unsigned char *Src, short Lenth);

extern void _BTOWADD(unsigned char *Dest, unsigned char *Src);
extern void _BCD2TO4ADD(unsigned char *Dest, unsigned char *Src);

extern void _SUB4(short Dest, short *Src);
extern void _ADD4(short Dest, short *Src);
extern void _ADD2(short *Dest, unsigned char *Src);
extern short _E2Pwrite( unsigned short E2P_Addr, unsigned char* RAM_Addr, short Lenth,short Devads );
extern short _E2PRead( unsigned char* RAM_Addr, unsigned short E2P_Addr, short Lenth, short Devads);
//*****************************************************************
// General.c
//*****************************************************************
unsigned char Byte_BCD( unsigned char Data );
void Word_BCD( unsigned char *Ptr, unsigned short Data );
void Long_BCD3( unsigned char *Ptr, unsigned long Data );
void Long_BCD4( unsigned char *Ptr, unsigned long Data );
short IsBCD(unsigned char Data);

unsigned char BCD_Byte( unsigned char Data );
unsigned short BCD2_Word( unsigned char *Ptr );
unsigned long BCD3_Long( unsigned char *Ptr );
unsigned long BCD4_Long( unsigned char *Ptr );

unsigned char ChkNum( unsigned char *Ptr, short Len );
unsigned char ChkNumAdd( unsigned char *Ptr, short Len );
unsigned short ChkNumShort( unsigned short *Ptr, short Len );

void Reset(void);
void WDTReset(void);

//void Delay(void);
void Delay( unsigned short i );
//void OpenADC12(void);
void OpenADC12( short Mode );
void CloseADC12(void);


unsigned char GetNo(unsigned char* Point,unsigned char* Source,short ItemNums);
short GetDaySeg(void);
void ClrMDBuf(void);
void GetFeeNo(void);
void RpaInit(void);
short Mon_Save(void);
void GetATAngle( unsigned char Addr, unsigned char* Data );
void GetVoltAmper( void );
//void SeasonFreeze( short Number );
//void FreezeEC( unsigned short Dest, short Cycle );
void ChangeSeason(void);
//void MD_Rest( short Type );
void Recd_ADD1( unsigned short CtAds, unsigned short DataLenth);
void E2P_DataMove( unsigned short Dest, unsigned short Source,short DataLenth, short Cycle );
void Recd_Ct_Tm( unsigned short CtAds, short Time);
void ADCEnd( void );
void ADCStart( short Type );
short SeasonJust( short StartTime, short EndTime, short Type );
void SeasonFreeze( short Season, short Type );
void Get24HourEC( void );
#if( LinkMode == Phase3Wire4 )	
void StartNCheck( void );
void EndNCheck( void );
#else
#endif
#if( SecurityCheck == YesCheck )				
void CheckLabState( void );
void OpenLabState( void );
void CloseLabState( void );
#else
#endif
void Freeze1EC( void );							
void Freeze23EC( unsigned short Dest );
void FreezeEC( void );

void E2P_MoveTwoCycle( unsigned short Source,short DataLenth );		//用于最近三次记录
void GetPrgRecd(unsigned short Com_ID);
void GetPdownRecd( void );

void SwapPara( unsigned char* Source, short Length, short Cycle );
void Swap7758Para( unsigned char* Source, short CommID );

void InitPara(void);

void SetConstPara(void);
void AveragePower( void );

void GetCMon_ECZ( unsigned char* Dest );
void GetLMon_ECZ( unsigned char* Dest, unsigned short Addr );

void CreateECClrRec( void );
void ECDataRecord( unsigned long Fls_Dest, unsigned short BackSpace );
void MDSave( void );
//void CreateMDClrRec( void );
void CreateMDClrRec( unsigned char* OperateCode );
void MDDataRecord( unsigned long Fls_Dest, unsigned short BackSpace );
short AllEventClr( unsigned char* EventIDAdr, short Type );
void ClrEventCuntTime( short Type );
short SingleEventClr( unsigned char* Source, unsigned short EventClrNo );
void AllDJClr( void );
void AllECClr( void );
void AllMDClr( void );

void GetSeg( unsigned char Num, short ZoneNo );

void GetECClrRec( unsigned char* Dest, unsigned long Fls_Src );
#if ( TakeMDClrRecFrameApart ==	YesCheck )			//11.02.17
#else
void GetMDClrRec( unsigned char* Dest, unsigned long Fls_Src );
#endif

void ClrMeterClrRec( void );					//清除电表清零记录

void GetSumVolt( void );						//累加每秒总电压			
void GetAverageVolt( void );					//计算每分钟平均电压			
void GetQualifiyRate( void );
//void InitQualifiyRateData( short PhaseNo );
//void InitAllQualifiyRateData( void );
void InitQualifiyRateData( short PhaseNo, short Type );
void InitMonQualifiyRateData( void );
void InitDayQualifiyRateData( void );
void GetCoverTerminalState( void );

void GetBatVolt( void );
void IncBatWorkTime( void );
short JustState( unsigned char* Source, unsigned char* Dest );
short AllIsBCD(unsigned char* Data, unsigned short Len );

void GetSFlag( void );
short JustBandRate( unsigned char BandRate );

short B1SingleEventClr( unsigned char* Source, unsigned short EventClrNo );
void MD_InitialCalPara( unsigned char NewMaxT );
void CreatFMQOFFAlarmFlg( void );
void CheckFRAMState( void );

#if ( PowerOnMonSaveMAX12 == YesCheck )						//11.06.27
void MDZeroSave( void );
#endif

void EveryMinInitPara(void);			//10.06.11
void InitPara1(void);			
void InitPara2(void);			
void InitPara3(void);			
void InitPara4(void);			
void InitPara5(void);			
void InitPara6(void);			
void InitPara7(void);			

//10.07.30
void GetPrivateKeyFlag( void );
//void SetPassWState( unsigned char State, unsigned char Type );		//新国网
//void ClrPrivateKeyFlag( void );										//新国网

void AllFailCalibrateDelay( void );			//10.10.06
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#else
float GetPhasePW( unsigned char Cmd );		//10.10.08
#endif

#if ( NewHardScheme2 == YesCheck )		//11.04.29
#if ( MagnKeepRelay == YesCheck  )		//11.04.29
short Un68_78VoltJudge( void ); 			//11.04.29
#endif									//11.04.29
#endif									//11.04.29

//#if ( PwDownKeepPrgDelay == YesCheck )							//12.12.21		//新国网
//void initialPrgDly( void ); 
//#endif

void IncYearMonth( unsigned char* TMon );											//新国网

void CreateMonthSaveTimeRec( unsigned short EventNo, unsigned char* SaveTime );		//新国网
void MoveSaveDay1EC( void );														//新国网	
short IsAwakeBatCom( void );															//新国网

void ComparRelayAlarm( void );														//新国网		
void CreatRelayOFFAlarmFlg( void );													//新国网

#if ( IOver30ALimitTrip	== YesCheck )		//新国网	//14.04.25	
short IOver30AJudge( void ); 
#endif

int Un55VoltJudge( void ); 								//新国网	//13.11.06

void InitialBat_WorkTime( unsigned char PwFlag ); 

#if ( LocalSmartMeter == YesCheck )
void FeeDataSwitch( void );			//费率数据切换			//新国网	//13.11.22
void GetFeePriceNum( void );									//新国网	//13.11.30	
void FeeTabSwitch( void );									//新国网	//13.11.22	
void StepDataSwitch( void );		//阶梯数据切换			//新国网	//13.11.29	
void StepTabSwitch( void );									//新国网	//13.11.22	
#endif

void GetSumPower( void );						//累加每秒总功率			
void GetAveragePower( void );					//计算每分钟平均功率			
