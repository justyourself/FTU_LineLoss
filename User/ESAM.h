
unsigned char ESAM_ReadByte(void);
void ESAM_WriteByte(unsigned char Byte);
unsigned char GetESAM_LRC( unsigned char* Source, unsigned short Len );
short ESAM_Operation( unsigned char* Dest, unsigned char* Source, unsigned short SendLen, unsigned short* RecLen  );
short ReadWriteESAM( unsigned char* Dest, unsigned char CLA, unsigned char INS, unsigned short Addr, unsigned short SendLen );
short GetESAMSaftInfo( unsigned char* Dest, unsigned short Addr );
short GetESAMClassData( short COM_No, unsigned char* Dest, unsigned char* Source );
short GetESAMClassDataTab( short COM_No, unsigned char* Dest, unsigned char* Source ); 
unsigned char SetESAMClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ); 
unsigned char SetESAMClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen ); 
unsigned char ActionESAMClassData( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 
unsigned char ActionESAMClassDataTab( unsigned short COM_No, unsigned char* Dest, unsigned char* Source, unsigned short DataLen, unsigned short* ItemLen, unsigned short* BackLen ); 

void GetCurSecretKeyState( void );
void WriteVoltCurrentRate( unsigned short Addr, unsigned char* Source );

#if ( LocalSmartMeter == YesCheck )
unsigned char WriteWalletFile( unsigned char* Source );
unsigned char RefundmentProcss698( unsigned char* Source );
unsigned char OpenAndRecharge( unsigned char OperateType, unsigned char* Source );
void ReadFeeStepSwitchTime( unsigned char* Dest, unsigned short Addr, unsigned short OldAddr );
#endif