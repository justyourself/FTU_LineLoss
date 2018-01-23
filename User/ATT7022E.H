void ATT7022EStateCheckRun( unsigned short Devads );
short Read_ATTAmper( unsigned char Cmd, unsigned char* Data ,unsigned short Devads);
short GetATT7022ECalibrateReg( unsigned char* ComBuf ,unsigned short Devads);
#if ( NEW7022E == YesCheck )
short UpdateTPSoffset(void);				
#endif
unsigned short GetATT7022ECalibDataEAddr( unsigned char RegNo );  


