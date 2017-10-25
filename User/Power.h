

//====================================================
short PowerCheck(void);

void EventCntADD(unsigned short Add);
void CNTAddOne( short CNT ,short E2PADD,short E2PPtr);

void TamperCheck( void );
short PhaseRev( void );
//s16 CBypasChk( void );
//s16 NeutralChk( void );
//s16 MissCurrent( void );
unsigned char MissPotent(void);
//s16 CurrentRev( void );
void GetCurrent( short* Ia,short* Ib, short* Ic);
//void GetVA( unsigned char*Dest );
//void TampRecord( s16 Temp,unsigned char Bytebit );
void TampRecord( short i, unsigned char Temp,short End );
//void TampRecord( s16 Tamp_No, unsigned char Temp,unsigned char End );
//void LoadRecord( void );
unsigned char ECRev( void );
void Data_Save( short Source, short DataLength, short CycleCnt );
unsigned char PhFail(void);
unsigned char CUFail(void);
unsigned char AmperOver(void);
unsigned char Rev( void );
unsigned char APhBk(void);
unsigned char BPhBk(void);
unsigned char CPhBk(void);
unsigned char PhBreak( short PhaseNo );
void TamperSumTime( void );
short VACompare( unsigned char* Source, unsigned char* Dest );
/*#if( AllFailChk == YesCheck )		
void AllFailRec( void );
#else
#endif
*/
void SumAmpHour( void );
void SumPhFailEC( void );
void GetMaxCurrent( void );
///short LoadRecord_RS( void );//void LoadRecord( void );
//short LoadPtrChk( long Type );//s16 LoadPtrChk( unsigned char* LoadPara, s16 Type );
short LoadPtrChk_RS( long Type ,short Ptr );//s16 LoadPtrChk( unsigned char* LoadPara, s16 Type );

///short ReadLoadRecord( unsigned char* Dest ,long Ptr);
///void SetLoadPara( void );//void SetLoadPara( unsigned char* Source );
void GetMeterState(void);
unsigned char PhFailState( short PhaseNo );
unsigned char CuFailState( short PhaseNo );
/*
unsigned char PhAFail(void);
unsigned char PhBFail(void);
unsigned char PhCFail(void);
unsigned char VFailState( void );
unsigned char CuAFail(void);
unsigned char CuBFail(void);
unsigned char CuCFail(void);

unsigned char CoverOpen(void);
unsigned char TCoverOpen(void);
unsigned char MeterResetOpen(void);
unsigned char MagnetOnOpen(void);
*/
//short ReadTamperRecord( unsigned char* Dest ,short Ptr);
short ReadTamperRecord( unsigned char* Dest ,short Ptr,unsigned char Number);
//short ReadLoadRecordByDate( unsigned char* Dest ,unsigned char* StartDate ,unsigned char* CloseDate );

void GetRunOverState(void);
//short TamperPtrChk( short Type );
/*
short TamperPtrChk( short Type ,short TamperRecState,short MAX_TAMPER_PTR);

//short TamperRecord( unsigned char Tamper_no );
short TamperRecord( unsigned char Tamper_no, unsigned char UseID);
	
void SetTamperPara( void );
void GetGLoadCurAdr( unsigned long* LoadCurAdr ,unsigned char Type );
void GetGEventCurAdr( unsigned long* LoadCurAdr ,unsigned char Number );
*/


