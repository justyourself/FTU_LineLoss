//short GetIOClassData( unsigned char* Dest, unsigned char* Source ); 
short GetIOClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 		//17.04.06
//short GetIOClassDataTab( unsigned char* Dest, unsigned char* Source ); 
short GetIOClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ); 	//17.04.06
unsigned short GetIOClassTabItemNo( unsigned char* OAD );
short GetSpecClassId22( unsigned char* Dest, unsigned char* OAD );	
short GetClass22Data( unsigned char* Dest, unsigned char* OAD );
unsigned char ActionIOClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
unsigned char ActionIOClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
short JudgeComDCB( unsigned char* Source );
void SetCOMBandRate( unsigned short Addr, unsigned char* BandRate, unsigned char Data );
