
//short GetParameterClassData( unsigned char* Dest, unsigned char* Source );
short GetParameterClassData( short COM_No, unsigned char* Dest, unsigned char* Source );        //17.03.24
//short GetParameterClassDataTab( unsigned char* Dest, unsigned char* Source ); 
short GetParameterClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source );     //17.03.24
unsigned short GetParameterClassTabItemNo( unsigned char* OAD );
short GetZoneSegItemStructure( unsigned char* Dest, unsigned char* Source );
short GetSaveDayItemStructure( unsigned char* Dest, unsigned short Addr );
//short GetSpecClassId8( unsigned char* Dest, unsigned char* OAD );	
short GetSpecClassId8( short COM_No, unsigned char* Dest, unsigned char* OAD );	                //17.03.24
//short GetClass8Data( unsigned char* Dest, unsigned char* OAD );
short GetClass8Data( short COM_No, unsigned char* Dest, unsigned char* OAD );                   //17.03.24
unsigned char SetParameterClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ); 
unsigned char SetClass8DataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ); 
//unsigned char SetClass8_2Data( unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen );
unsigned char SetClass8_2Data( unsigned short COM_No, unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen );		//17.04.09
void SetZoneSegItemStructure( unsigned char* Dest, unsigned char* Source );
void SetSaveDayItemStructure( unsigned short Addr, unsigned char* Source );
void FillDefaultSegData( unsigned char* Point, unsigned short Start, unsigned short End );
unsigned char SetSpecClassId8( unsigned char* Source, unsigned char* OAD, unsigned short DataLen, unsigned short* ItemLen );

unsigned char ActionParameterClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
unsigned char ActionParameterClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen );
void TimeChangeUpdateAllLastSaveTime( void );
short BroadC_Time698( unsigned char* Source );

void AddIdentityFaultCnt( void );				
void InitFaultCnt( void );
void RemoteFaultDisplay( unsigned char AlarmCode );
short JudgeTripONPara( unsigned char* Source, unsigned char* Type );
short JudgeTripOFFPara( unsigned char* Source, unsigned char* Type );
short TripOFFProcess( unsigned char OperateType );

unsigned char Judge_SafeModeParaItem( unsigned char* Data, unsigned short* ItemLen );		//17.04.07
short NewOIIsNotExist( unsigned char* Source );				//17.04.07
short AllNewOIIsNotExist( unsigned char* Source );			//17.04.07
void AddAllSafeModeParaItems( unsigned char* Source );		//17.04.07
void AddSafeModeParaItem( unsigned char* Source );			//17.04.07
void DeleteSafeModeParaItem( unsigned char* Source );		//17.04.07

short TripONProcess( unsigned char OperateType );			//17.04.17
//short JudgeTimeTag( unsigned short COM_No, unsigned char* Source );				//17.04.17
short JudgeTimeTag( unsigned short COM_No );				//17.04.30
