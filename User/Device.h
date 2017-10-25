
unsigned short GetClass19TabItemNo( unsigned char* OAD );
//short GetClass19Data( unsigned char* Dest, unsigned char* OAD );
short GetClass19Data( short COM_No, unsigned char* Dest, unsigned char* OAD );		//17.04.09
short GetDeviceClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 
short GetDeviceClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source );
unsigned char SetDeviceClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ); 
unsigned char SetDeviceClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen );
//unsigned char SetClass19Data( unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen );
unsigned char SetClass19Data( unsigned short COM_No, unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen );	//17.04.09
unsigned char ActionDeviceClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
unsigned char ActionDeviceClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
void ClrEventData( short Type );
void ClrFreezeData( void );
void InitDisplayTab( void );
void InitEventTab( void );
void InitComPara( void );

void MeterClrCycle( void );

void FillInitFreezeItemBuff( unsigned char* Dest, unsigned char* Period, unsigned char* OAD, unsigned char* Deep );
void InitFreezeTab( unsigned short ItemNo );
void InitFreezeCycle( void );
void DeletAllFreezeFile( void );
void EventClrCycle( void );								//17.04.14
