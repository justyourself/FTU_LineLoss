/*****************************************************************
	RAM.c
*****************************************************************/

void RAM_Write( unsigned char *Dest, unsigned char *Src, short Len );
//void RAM_Fill( char* Dest, short Len, char Data);
void RAM_Fill( unsigned char* Dest, short Len);
short Data_Comp(unsigned char *Data1, unsigned char *Data2, short Len);
short Data_CompHL( unsigned char *Data1, unsigned char *Data2, short Len);

void RAM_DataFill( unsigned char* Dest, short Len, unsigned char Data);
short IsAllData( unsigned char* Source, short Len, unsigned char Data );
void SwapData( unsigned char* Dest, short Len );
#if( ECDisDecDigitsValid ==	YesCheck )		//13.03.03 当前电量显示小数位真实有效
//数据串低字节在前，左移缩小
void LeftShiftData( unsigned char* Dest, unsigned char DataLen, unsigned char ShiftDigit );
#endif
void SwapDataBlock( unsigned char* Dest, unsigned char* TestBuff, short Items, short BlockLen );			//新国网
//unsigned char GetTrueBitSum_32Bit( unsigned char* Source, short BitRange );								//新国网
void SetOneBitTrue_32Bit( unsigned char* Source, short BitNo );											//新国网
void SetOneBitFalse_32Bit( unsigned char* Source, short BitNo );											//新国网
short OneBitIsTrue_32Bit( unsigned char* Source, short BitNo );												//新国网

void RAM_Write_Rev( unsigned char *Dest, unsigned char *Src, short Len );
unsigned short SwapCharToShort( unsigned char* Data );
unsigned long SwapCharToLong( unsigned char* Data );
void ShortToSwapChar( unsigned char* Dest, unsigned short Data );
void LongToSwapChar( unsigned char* Dest, unsigned long Data );
void BCDStringToHEX( unsigned char* Dest, unsigned char* Source, unsigned short Len );
void HEXStringToBCD( unsigned char* Dest, unsigned char* Source, unsigned short Len );
short HEX_ASC(unsigned char* pDst, unsigned char* pSrc, short nSrcLength);
void SwapBitString(unsigned char* Dest, short Len );

unsigned char GetTrueBitSum_128Bit( unsigned char* Source, short BitRange );									//新国网

/*****************************************************************
	Flash_CPU.c
*****************************************************************/
/*
void Fls_SegClr( char *Dest );
void Fls_wb( char *Dest, char Data );
void Fls_Write( char *Dest, char *Src, short Len );

void FlsInfo_Write( char *Dest, char *Src, short Len );
void Fls2_Write( char *Dest, char *Src, short Len );
void Flash_Write( char *Dest, char *Src, short Len );
void Flash_Fill( char *Dest, short Lenth, char Data );
*/
/*****************************************************************
	Flash_Data.c
*****************************************************************/
/*
void Flash_Reset(void);
char Read_Status(void);
short Read_Buffer1( char *RAM_Dest,  unsigned short Buf_Src, short Lenth);
short Read_Flash( char *RAM_Dest, unsigned short Page_Addr, unsigned short Buf_Addr, short Lenth);
short Write_Buffer1( unsigned short Buf_Dest, char *RAM_Src, short Lenth);
short Fls_Buf1( unsigned short Page_Add );
short Buf1_Fls( unsigned short Page_Add );
unsigned short Read_BufPg( void );
short Write_BufPg( unsigned short Page_Addr);

short DFlash_Read( char *RAM_Dest, unsigned long Fls_Src, short Lenth);
short DFlash_Write( unsigned long Fls_Dest, char *RAM_Src, short Lenth);
void DFlash_Fill( unsigned long Dest, short Lenth, char Data );
*/
/*****************************************************************
	E2PROM.c
*****************************************************************/
short E2PRead( unsigned char* RAM_Addr, unsigned short E2P_Addr, unsigned short Lenth, unsigned short Devads);
short E2Pwrite( unsigned short E2P_Addr, unsigned char* RAM_Addr, unsigned short Lenth,unsigned short Devads );

short E2P_Write( unsigned short E2P_Dest,unsigned char* RAM_Src, short Lenth, short Dev_Ads );
short E2P_Read( unsigned char* RAM_Dest, unsigned short E2P_Src, short Lenth, short Dev_Ads );
void E2P_E2P( unsigned short Dest, unsigned short Src, short Len, short Dev_Ads );
void E2P_Fill( unsigned short Dest, short Len, char Data, short Dev_Ads );
short E2P_RData( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth );
short E2P_WData( unsigned short E2P_Dest, unsigned char* RAM_Src, short Lenth );
short E2P_RMDBuf( unsigned char* RAM_Dest, unsigned short E2P_Src, short Lenth );
short E2P_WMDBuf( unsigned short E2P_Dest, unsigned char* RAM_Src, short Lenth );
short E2P_RAdj( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth );
short E2P_WAdj( unsigned short E2P_Src, unsigned char* RAM_Dest, short Lenth );
short E2P_RFM( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth );
short E2P_WFM( unsigned short E2P_Dest, unsigned char* RAM_Src, short Lenth );
short E2P_PRData( unsigned char* RAM_Dest,unsigned short E2P_Src, short Lenth );
short E2P_PWData( unsigned short E2P_Dest, unsigned char* RAM_Src,short Lenth );

/*****************************************************************
	Flash_Data.c
*****************************************************************/
#if ( LoadMFlash == YesCheck )
//short DataFlash_Read( unsigned char *RAM_Dest, unsigned long Fls_Src, unsigned short Lenth);
//short DataFlash_Write( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth );
short DataFlash_Read( unsigned char *RAM_Dest, unsigned long Fls_Src, unsigned long Fls_Sta, unsigned long Fls_End, unsigned short Lenth );
//short DataFlash_Write( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned long Fls_Sta, unsigned long Fls_End, unsigned short Lenth );
short DataFlash_Write( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth );

short Write_Flash_Direct( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth );

short Flash_On( void );		//V10
short Flash_Off( void );		//V10

short BlockErase(unsigned long Fls_Src);
short Read_Flash( unsigned char *RAM_Dest, unsigned long Fls_Src, unsigned short Lenth );
short Write_Flash( unsigned long Fls_Dest, unsigned char *RAM_Src, unsigned short Lenth );

short Read_FlashID( unsigned char *RAM_Dest );										//11.02.23
#else
#endif

extern short I2CErr( I2CPARA* I2CPara );
extern short I2CStart( I2CPARA* I2CPara, unsigned short E2P_Addr, unsigned short Devads );	//I2C总线Start
extern short I2CStartAddr( I2CPARA* I2CPara, unsigned short E2P_Addr, unsigned short Devads );	//I2C总线Start+地址
extern short I2CMtData( I2CPARA* I2CPara, unsigned char SendData );			//I2C总线发送1字节	
extern void GetI2CPara( I2CPARA* I2CPara, unsigned short Devads );
