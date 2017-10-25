#ifndef __PROFILEGENERIC_H__
#define __PROFILEGENERIC_H__

extern short GetProfile_genericClassData( short COM_No, unsigned char* Dest, unsigned char* Source ); 
extern unsigned char SetProfile_genericClassData( short COM_No, unsigned char *Source , unsigned short DataLen );
extern short GetProfile_bufferData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item );
extern short GetProfile_capture_objectsData( short COM_No, unsigned char* Dest, short ProfItemNo );
extern unsigned char Judge_capture_objects_content( unsigned char* Data, unsigned short* Length );
extern short Get_sigle_capture_objectsPara( unsigned char* Dest, unsigned short Addr, unsigned short CaptureItems, unsigned short* Length );
extern short GetNextProfile_genericClassData( short COM_No, unsigned char* RecBuff, unsigned char* SendBuff );
extern unsigned short GetProfile_genericClassTabItemNo( unsigned char* Logical_Name );
extern short GetProfile_bufferEntryData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item );
extern short GetProfile_bufferRangeData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item );
extern short GetProfile_bufferAllData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item );
extern short GetCaptureItems( short ProfItemNo );
extern void Get_sigle_curve_entry_data( short COM_No, unsigned char* Dest, CURVEPARA* CurvePara, long LoadRecNo, unsigned short* Length );
extern void Get_sigle_curve_range_data( short COM_No, unsigned char* Dest, CURVEPARA* CurvePara, long LoadRecNo, unsigned short* Length );	
extern void Get_sigle_curve_data( unsigned char* Dest, CURVEPARA* CurvePara, long LoadRecNo, unsigned short* Length );	
extern short Judge_capture_objects( unsigned char* Data, short ProfileNo, unsigned short* Length );
extern void InitialProfilePara( unsigned short ItemNo, unsigned short ItemLen );
extern void GetCurvePara( CURVEPARA* CurvePara );
extern long LoadPtrChk( CURVEPARA* CurvePara, long Type );
extern void LoadRecord( unsigned short ChannelNo );
extern short CreatCurveData( unsigned char* Dest, unsigned short ChannelNo );
extern void IncSubsLoadPtr( CURVEPARA* CurvePara );
extern short Recuce_capture_object_definition( unsigned char* Dest, unsigned char* Source );
extern short Judge_restricting_object( unsigned short ChannelNo, unsigned char* Source, unsigned short* StartAddr, unsigned short* DataLen );
extern short Judge_restricting_Value( unsigned char* Source );
extern short Judge_range_descriptor( unsigned short ChannelNo, unsigned char* Dest, unsigned char* Source, unsigned short* RestrictAddr );
extern short Judge_entry_descriptor( unsigned char* Source );
extern short GetEntryLoadRecNo( unsigned char* Dest, CURVEPARA* CurvePara, unsigned char* Source );
//extern short Date_time_Less_than( unsigned char *Dest, unsigned char *Source );
//extern short Date_time_Rather_than( unsigned char *Dest, unsigned char *Source );
extern short Date_time_Comp( unsigned char *Dest, unsigned char *Source );
extern short GetAppointTimeLoadRecNo( unsigned char* Dest, unsigned char* STime, unsigned char* ETime, CURVEPARA* CurvePara, unsigned short StartAddr );
extern long GetLoadPWAds( unsigned char* Dest, CURVEPARA* CurvePara, unsigned char* STime, unsigned char* ETime, unsigned short StartAddr, short Type );
extern short LoadPWJustDay( long LoadRecNo, CURVEPARA* CurvePara, unsigned char* STime, unsigned char* ETime, unsigned short StartAddr, short Type );

extern unsigned char ActionProfile_genericClassData( unsigned char *Source , unsigned short DataLen );

//extern void InitialProfClassPara_Billing_period1( void );
//extern void InitialProfClassPara_Load_profile1( void );
//extern void InitialProfClassPara_Event_log1( void );

extern void Get_sigle_readout_data( unsigned char* Dest, short ChannelNo, unsigned short* Length );
extern short GetProfile_bufferAllReadoutData( short COM_No, unsigned char* Dest, unsigned char* Source, short Item );
extern void Getcapture_period( void );    //²¶»ñÖÜÆÚ..
extern void Getcapture_period_cnt( void );
extern void PushIncNum( short ChannelNo, unsigned long CurRecNo,unsigned char Inc_Or_Dec );
unsigned char PDN_Profile_24hours_Chek(void);
///extern void Load_Profile_15min(unsigned char* Profile_Status);
///extern void Load_Profile_24hours(unsigned char* Profile_Status);
extern void Load_Profile_15min(void);
extern void Load_Profile_24hours(void);

extern unsigned char PDN_Profile_15min_Chek(void);
extern void HexToASCII(unsigned char*  data_hex, unsigned char*  data_ascii);
extern void ASCIIToHex(unsigned char*  data_ascii, unsigned char* buff_hex);
extern short GetDemand_registerClassData( unsigned char* Dest, unsigned char* Source ) ;
extern unsigned long BCDToHex(unsigned char* data);
extern unsigned short GetDemandRegTabItemNo( unsigned char* Logical_Name );
#endif

