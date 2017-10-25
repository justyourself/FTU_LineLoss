short JudgeYearSaveDay( unsigned char* Source );									//新国网	//13.11.28
unsigned char GetYearMonSaveMode( unsigned char* YearSaveDay );					//新国网	//13.11.28	
void MonSaveBarely( void );														//新国网	//13.11.29
void YearSaveBarely( void );													//新国网	//13.11.29
void YearMonSaveDaySwitchProcess( unsigned char* OldData, unsigned char* NewData );	//新国网	//13.11.28	
void YearDec( unsigned char *Data );											//新国网	//13.11.28
void YearInc( unsigned char *Data );											//新国网	//13.11.28
void UpdateLastYearSaveTime( unsigned char* YearSaveDayBuff );					//新国网	//13.11.28
void CreateYearSaveTimeRec( unsigned short EventNo, unsigned char* SaveTime );	//新国网	//13.11.28
short AdjustYear( unsigned char* YearSaveDayBuff, short* YearSaveCnt );				//新国网	//13.11.28
void MoveYearSaveDayEC( void );													//新国网	//13.11.28
unsigned char GetCurYearMonSaveMode( unsigned char* Dest );						//新国网	//13.11.29
short Year_Save(void);															//新国网	//13.11.28
