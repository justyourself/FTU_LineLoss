void ATT7022EStateCheckRun( void );
short Read_ATTAmper( unsigned char Cmd, unsigned char* Data );
short GetATT7022ECalibrateReg( unsigned char* ComBuf );
#if ( NEW7022E == YesCheck )
short UpdateTPSoffset(void);				
#endif
unsigned short GetATT7022ECalibDataEAddr( unsigned char RegNo );  


