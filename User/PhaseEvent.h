

short GetPhaseEventData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source ); 
unsigned short GetEventClassTabItemNo( unsigned char* OAD );
short GetPhaseEventClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source ); 
short GetRecordPhaseEventClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 
short GetRecordPhaseEventClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ); 
short GetClass24_3Data( unsigned char* Dest, unsigned char* OAD, unsigned short ItemNo );
short GetClass24_10Data( unsigned char* Dest, unsigned char* OAD, unsigned short ItemNo );
//short GetClass24_5_13Data( unsigned char* Dest, unsigned char* OAD );
short GetClass24_5_13Data( short COM_No, unsigned char* Dest, unsigned char* OAD );				//17.04.09
unsigned char SetPhaseEventData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen );
unsigned char SetPhaseEventClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ); 
void SetRAMData_HEXToBCD( unsigned char* Dest, unsigned char* Source, unsigned char DataType, unsigned char Element_Len, unsigned char BCD_Len, signed char Multiple );		
//unsigned char SetClass24_5Data( unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen );
unsigned char SetClass24_5Data( unsigned short COM_No, unsigned char *Source , unsigned short DataLen, unsigned short* ItemLen );   //17.04.09
unsigned char ActionPhaseEventClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
unsigned char ActionPhaseEventClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 

void AddPhFailCuntZ( void );
void AddPhFailTimeZ( unsigned long Value );
void SavePhFailStartEndTime( unsigned short Addr, unsigned char* Time );
void ClearPhFailData( void );

void ResetSignleClass24Event( unsigned short EventNo );					//17.05.10
unsigned char EventOADIsAlreadyExist( unsigned short ItemNo, unsigned char* OAD );			//17.05.10
unsigned char AddEventCapture_OAD( unsigned short ItemNo, unsigned char* OAD );				//17.05.10
unsigned char DeleteEventCapture_OAD( unsigned short ItemNo, unsigned char* OAD );			//17.05.10
