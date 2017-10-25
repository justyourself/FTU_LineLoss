#ifndef __ECPROCESS_H__
#define __ECPROCESS_H__

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
short ECRgSum( short ECItem, unsigned short Data, unsigned short FData );
#else
short ECRgSum( short ECItem, unsigned short Data );
#endif
void GetRamEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds );
void ECReserveCycle( void );
short E2PECIsOK( unsigned char* ECRAds );				//10.06.22
void Clr_FRAMData( unsigned short Addr, unsigned short DataLenth, unsigned short Cylcle );
void Clr_E2PData( unsigned short Addr, unsigned short DataLenth, unsigned short Cylcle );
void ResumeEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds );	//10.12.08
void ECSave( unsigned short StartNo, unsigned short EndNo );

#endif
