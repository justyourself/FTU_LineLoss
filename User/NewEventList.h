/*
void UpStateStringToShortType( EVENT_UPSTATE* EventUpState, unsigned char* Source );
void UpStateShortTypeToString( unsigned char* Dest, EVENT_UPSTATE* EventUpState );
unsigned short GetSignleNewEventList( unsigned char* Dest, EVENT_UPSTATE* EventUpState, unsigned short ItemNo );
unsigned short GetNewEventList( short COM_No, unsigned char* Dest );
void AddSignleNewEvent( EVENT_UPSTATE* EventUpState, unsigned char ReportFlag, short End );
unsigned short GetEventClassTabItemNo_Event_No( unsigned short Event_No );
unsigned short GetEventClassTabItemNo_OAD3( unsigned char* OAD );
void AddToNewEventList( unsigned short Event_No, short End );
//void DelSignleNewEvent( EVENT_UPSTATE* EventUpState, unsigned char ReportNo, unsigned char ReportFlag );
void DelSignleNewEvent( EVENT_UPSTATE* EventUpState, unsigned char ReportFlag );
void DelFromNewEventList( short COM_No, unsigned char* OAD );
void InitialNewEventList( short Type );
*/

unsigned short GetEventClassTabItemNo_OAD3( unsigned char* OAD );		//17.05.19
void InitialNewReportEventList( short Type );							//17.05.19
void DelFromNewReportEventList( short COM_No, unsigned char* OAD );		//17.05.19
void AddToNewReportEventList( unsigned short Event_No, short End );		//17.05.19
unsigned short GetSignleNewReportEventList( unsigned char* Dest, unsigned short Event_No );			//17.05.19
unsigned short GetNewReportEventList( short COM_No, unsigned char* Dest, unsigned char* OAD );	//17.05.19
unsigned short GetNewReportEventListAttribute3( unsigned char* Dest );	//17.05.19

void CheckClearFollowOADDelay( void );									//17.05.20
void InitialClearFollowOADDelay( void );								//17.05.20
void DecClearFollowOADDelay( void );									//17.05.20
void StartClearFollowOADDelay( short COM_No );							//17.05.20
void DelCurFromNewReportEventList( short COM_No );						//17.05.20















