short GetTCP_UDP_setupClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char SetTCP_UDP_setupClassData( unsigned char *Source , unsigned short DataLen );
short GetGPRS_modem_setupClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char SetGPRS_modem_setupClassData( unsigned char *Source , unsigned short DataLen );
short GetSMTP_setupClassData( unsigned char* Dest, unsigned char* Source );
unsigned char SetSMTP_setupClassData( unsigned char *Source , unsigned short DataLen ); 
short GetMACaddr_GPRSClassData( unsigned char* Dest, unsigned char* Source );
unsigned char SetMACaddr_GPRSClassData( unsigned char *Source , unsigned short DataLen );
short GetIPv4_setupClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char SetIPv4_setupClassData( unsigned char *Source , unsigned short DataLen );
//Security_setup64..
short GetSecurity_setupClassData( unsigned char* Dest, unsigned char* Source );
unsigned char SetSecurity_setupClassData( unsigned char *Source , unsigned short DataLen );
unsigned char ActionSecurity_setupClassData( short COM_No,unsigned char *Source , unsigned short DataLen );
//厂家自定义..
short GetManufacture_setupData( unsigned char* Dest, unsigned char* Source );

//GPRS操作：
//40
short Get_PushsetupClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char Set_PushsetupClassData( unsigned char *Source , unsigned short DataLen );
//28
short Get_Auto_answer_setupClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char Set_Auto_answer_setupClassData( unsigned char *Source , unsigned short DataLen );
//29
short Get_Auto_connect_setupClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char Set_Auto_connect_setupClassData( unsigned char *Source , unsigned short DataLen );
