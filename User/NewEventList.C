#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"

#include "RsComm.h"
#include "interrupt.h"
#include "Mem.h"
#include "Time.h"
#include "Power.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
//#include "Produce.h"
#include "cpucard.h"
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif

/*
#define NEW_EVENT_NUM0		30		//通讯口0新增事件列表容量（红外）
#define NEW_EVENT_NUM1		30		//通讯口1新增事件列表容量（4851）
#define NEW_EVENT_NUM2		30		//通讯口2新增事件列表容量（4852或载波）

#define NewEventL0FirstNo			EPhCBreakAmpHour+16				//通道0新增事件列表队首序号(1)
#define NewEventL0Recs				NewEventL0FirstNo+1*1			//通道0新增事件列表有效记录数(1)
#define NewEventL1FirstNo			NewEventL0FirstNo+1*2			//通道1新增事件列表队首序号(1)
#define NewEventL1Recs				NewEventL0FirstNo+1*3			//通道1新增事件列表有效记录数(1)
#define NewEventL2FirstNo			NewEventL0FirstNo+1*4			//通道2新增事件列表队首序号(1)
#define NewEventL2Recs				NewEventL0FirstNo+1*5			//通道2新增事件列表有效记录数(1)

#define NewEvent0List				NewEventL2Recs+1				//通道0新增事件列表(NEW_EVENT_NUM0*5)
#define NewEvent1List				NewEvent0List+NEW_EVENT_NUM0*5	//通道1新增事件列表(NEW_EVENT_NUM1*5)
#define NewEvent2List				NewEvent1List+NEW_EVENT_NUM1*5	//通道2新增事件列表(NEW_EVENT_NUM2*5)

typedef struct
{
	unsigned short ListAddr;		//队列首地址
	unsigned short FirstNoAddr;		//队首序号地址
	unsigned short RecsAddr;		//有效记录数地址
	unsigned short CapNumMax;		//队列最大项数(物理空间允许接收的项数)
}NEW_EVENT_LIST_TAB;	

const NEW_EVENT_LIST_TAB New_Event_ListTab[] = 
{					
	NewEvent0List, 	NewEventL0FirstNo,	NewEventL0Recs, 	NEW_EVENT_NUM0,		//COM0
	NewEvent1List, 	NewEventL1FirstNo,	NewEventL1Recs, 	NEW_EVENT_NUM1,		//COM1
	NewEvent2List, 	NewEventL2FirstNo,	NewEventL2Recs, 	NEW_EVENT_NUM2,		//COM2																																																																																																																																										
}
*/

////BIT0表示最近一次事件记录
//typedef struct
//{
//	unsigned short StartState;		//事件发生状态		0：未发生 1：已发生
//	unsigned short StartUpState;	//事件发生上报状态  0：未上报 1：已上报
//	unsigned short EndState;		//事件结束状态		0：未发生 1：已发生
//	unsigned short EndUpState;		//事件结束上报状态	0：未上报 1：已上报
//}EVENT_UPSTATE;	

/*
void UpStateStringToShortType( EVENT_UPSTATE* EventUpState, unsigned char* Source )
{
	unsigned short Temp;
	
	RAM_Write( (unsigned char*)&Temp, Source, 2 );
	Temp &= 0x03FF;
	EventUpState->StartState = Temp;

	RAM_Write( (unsigned char*)&Temp, Source+1, 2 );
	Temp &= 0x0FFC;
	Temp >>= 2;
	EventUpState->StartUpState = Temp;
	
	RAM_Write( (unsigned char*)&Temp, Source+2, 2 );
	Temp &= 0x3FF0;
	Temp >>= 4;
	EventUpState->EndState = Temp;
	
	RAM_Write( (unsigned char*)&Temp, Source+3, 2 );
	Temp &= 0xFFC0;
	Temp >>= 6;
	EventUpState->EndUpState = Temp;
}	

void UpStateShortTypeToString( unsigned char* Dest, EVENT_UPSTATE* EventUpState )
{
	unsigned short Temp;
	
	RAM_Fill( Dest, 5 );
	
	RAM_Write( Dest, (unsigned char*)&EventUpState->StartState, 2 );
	*(Dest+1) &= 0x03;
	
	Temp = EventUpState->StartUpState;
	Temp <<= 2;
	*(Dest+1) |= Temp & 0xFC;
	*(Dest+2) |= ( Temp & 0x0F00 ) >> 8;
	
	Temp = EventUpState->EndState;
	Temp <<= 4;
	*(Dest+2) |= Temp & 0x00F0;
	*(Dest+3) |= (Temp & 0x3F00) >> 8;
	
	Temp = EventUpState->EndUpState;
	Temp <<= 6;
	*(Dest+3) |= Temp & 0x00C0;
	*(Dest+4) |= (Temp & 0xFF00) >> 8;
}	

unsigned short GetSignleNewEventList( unsigned char* Dest, EVENT_UPSTATE* EventUpState, unsigned short ItemNo )
{
	unsigned char Buff[6];
	unsigned char* Point;
	unsigned short Items;
	unsigned short Temp1;
	unsigned short Temp2;
	unsigned short StartState;		//事件发生状态		0：未发生 1：已发生
	unsigned short StartUpState;	//事件发生上报状态  0：未上报 1：已上报
	unsigned short EndState;		//事件结束状态		0：未发生 1：已发生
	unsigned short EndUpState;		//事件结束上报状态	0：未上报 1：已上报
	short i;
	unsigned char* Ptr;
		
	Ptr = Dest;	
	Point = Buff;
	StartState = EventUpState->StartState;	
	StartUpState = EventUpState->StartUpState;	
	EndState = EventUpState->EndState;	
	EndUpState = EventUpState->EndUpState;	
		
	Temp1 = StartState | EndState;
	if( Temp1 == 0 ) return 0;				//无事件产生
	Temp1 = StartState & (~StartUpState); 	//是否有已发生而未上报的记录
	Temp2 = EndState & (~EndUpState); 		//是否有已恢复而未上报的记录
	Temp1 |= Temp2;							//有发生或结束任意一种未上报
	if( Temp1 == 0 ) return 0;				//无新增未上报事件
	
	Items = 0;
	RAM_Write( Point, (unsigned char*)DL698_Event_ProfileTab[ItemNo].OAD, 3 );			
	for( i=0;i<10;i++ )
	{
		if(( Temp1 & ByteBit[i] ) != 0 )
		{
			*Ptr = Data_structure;
			*(Ptr+1) = 2;
			*(Ptr+2) = D_OAD;
			RAM_Write( Ptr+3, Point, 3 );
//			*(Ptr+6) = i+1;
			*(Ptr+6) = 0x00;				//只返回1条	
			*(Ptr+7) = D_BOOLEAN;
			*(Ptr+8) = 0x01;						
			Ptr += 9;
			Items += 1;
			break;							//只返回1条
		}		
	}	
	
	return Items;	
}	

unsigned short GetNewEventList( short COM_No, unsigned char* Dest )
{
	unsigned short ShortBuff[6];
	EVENT_UPSTATE* EventUpState;
	unsigned char Buff[6];
	unsigned char* Point;
	unsigned short Items;
	unsigned short Len;
	unsigned char* Ptr;
	short i;
	unsigned char ReportFlag;	//上报标识
	
    if( COM_No >= 3 )           //非本通道数据
    {
        *Dest = 0x00;
        Len = 1;
        return Len;
    }  
    
	EventUpState = (EVENT_UPSTATE*)&ShortBuff;
	Point = Buff;
	*Dest = Data_array;
	Ptr = Dest+4;				//前3字节为长度字节预留	
	Items = 0;
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{
		E2P_RData( (unsigned char*)&ReportFlag, DL698_Event_ProfileTab[i].EReportFlagAddr, 1 ); 
		if(( ReportFlag == 0 )||( ReportFlag > 3 )) continue;		//不上报
		E2P_RData( Point, DL698_Event_ProfileTab[i].UpState+COM_No*5, 5 ); 
		UpStateStringToShortType( EventUpState, Point );
		Len = GetSignleNewEventList( Ptr, EventUpState, i );
		Ptr += Len * 9;
		Items += Len;
		if( Items > 200 ) break;	//可能超出APDU长度，退出
	}	
	
	Len = Items * 9;
	if( Items < 128 )
	{
		*(Dest+1) = Items;
		RAM_Write( Dest+2, Dest+4, Len );	
		Len += 1;
	}
	else if( Items < 256 )
	{
		*(Dest+1) = 0x81;
		*(Dest+2) = Items;
		RAM_Write( Dest+3, Dest+4, Len );	
		Len += 2;		
	}		
	else 
	{
		*(Dest+1) = 0x82;
		*(Dest+2) = ( Items & 0xff00 ) >> 8;
		*(Dest+3) = ( Items & 0x00ff );
		Len += 3;		
	}		
	
	Len += 1;	//Data_array类型
	
	return Len;
}

void AddSignleNewEvent( EVENT_UPSTATE* EventUpState, unsigned char ReportFlag, short End )
{	

	EventUpState->StartState <<= 1;	
	EventUpState->StartUpState <<= 1;	
	EventUpState->EndState <<= 1;	
	EventUpState->EndUpState <<= 1;	
	
	if( End == 0 )
	{
		if(( ReportFlag == 1 )||( ReportFlag == 3 )) EventUpState->StartState |= 0x01;
	}	 
	else 
	{
		if(( ReportFlag == 2 )||( ReportFlag == 3 )) EventUpState->EndState |= 0x01;		
	}	

	EventUpState->StartState &= 0x03FF ;	
	EventUpState->StartUpState &= 0x03FF ;	
	EventUpState->EndState &= 0x03FF ;	
	EventUpState->EndUpState &= 0x03FF ;	

	return;	
}	

unsigned short GetEventClassTabItemNo_Event_No( unsigned short Event_No )
{
	unsigned short i;
		
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{
		if( Event_No == DL698_Event_ProfileTab[i].EventNo ) break;
	}	
	return i;	
}



//End: 0：事件起始 1：事件结束
void AddToNewEventList( unsigned short Event_No, short End )
{
	unsigned short ShortBuff[6];
	EVENT_UPSTATE* EventUpState;
	unsigned char Buff1[6];
	unsigned char Buff2[6];
	unsigned char* Point;
	unsigned char* Ptr;
	short i;
	unsigned char ReportFlag;	//上报标识
	unsigned short ItemNo;
	
	EventUpState = (EVENT_UPSTATE*)&ShortBuff;
	Point = Buff1;
	Ptr = Buff2;				

	ItemNo = GetEventClassTabItemNo_Event_No( Event_No );
	if( ItemNo == DL698_Event_ProfileSum ) return;
	E2P_RData( (unsigned char*)&ReportFlag, DL698_Event_ProfileTab[ItemNo].EReportFlagAddr, 1 ); 
	if(( ReportFlag == 0 )||( ReportFlag > 3 )) return;		//不上报
	if(( ReportFlag == 1 )&&( End != 0 )) return;		//配置为发生上报，结束事件不产生变化
	if(( ReportFlag == 2 )&&( End == 0 )) return;		//配置为结束上报，发生事件不产生变化
	for( i=0;i<3;i++ )
	{
		E2P_RData( Point, DL698_Event_ProfileTab[ItemNo].UpState+i*5, 5 ); 
		UpStateStringToShortType( EventUpState, Point );
		AddSignleNewEvent( EventUpState, ReportFlag, End );
		UpStateShortTypeToString( Ptr, EventUpState );
		E2P_WData( DL698_Event_ProfileTab[ItemNo].UpState+i*5, Ptr, 5 ); 
	}
	
}

//标注对应的上报位
//void DelSignleNewEvent( EVENT_UPSTATE* EventUpState, unsigned char ReportNo, unsigned char ReportFlag )
void DelSignleNewEvent( EVENT_UPSTATE* EventUpState, unsigned char ReportFlag )
{	
	int i;
	
	for( i=0;i<10;i++ )
	{
		if(( EventUpState->StartState & ByteBit[i] ) != 0 )
		{
			if(( ReportFlag == 1 )||( ReportFlag == 3 )) EventUpState->StartUpState |= ByteBit[i]; 	
		}	
		
		if(( EventUpState->EndState & ByteBit[i] ) != 0 )
		{
			if(( ReportFlag == 2 )||( ReportFlag == 3 )) EventUpState->EndUpState |= ByteBit[i]; 	
		}	
	}	
	return;	
}	

//标注对应的上报位，以从新增事件列表中删除
void DelFromNewEventList( short COM_No, unsigned char* OAD )
{
	unsigned short ShortBuff[6];
	EVENT_UPSTATE* EventUpState;
	unsigned char Buff1[6];
	unsigned char Buff2[6];
	unsigned char* Point;
	unsigned char* Ptr;
	unsigned char ReportFlag;	//上报标识
	unsigned short ItemNo;
	unsigned short UpStateAddr;
	
	EventUpState = (EVENT_UPSTATE*)&ShortBuff;
	Point = Buff1;
	Ptr = Buff2;					

//	if(( *(OAD+3) == 0 )||( *(OAD+3) > 10 )) return;		//超出记录范围
	if( *(OAD+3) != 0 ) return;		//超出记录范围
	ItemNo = GetEventClassTabItemNo_OAD3( OAD );
	if( ItemNo == DL698_Event_ProfileSum ) return;
	E2P_RData( (unsigned char*)&ReportFlag, DL698_Event_ProfileTab[ItemNo].EReportFlagAddr, 1 ); 
	if(( ReportFlag == 0 )||( ReportFlag > 3 )) return;		//不上报不处理

	UpStateAddr = DL698_Event_ProfileTab[ItemNo].UpState+COM_No*5;
	E2P_RData( Point, UpStateAddr, 5 ); 
	UpStateStringToShortType( EventUpState, Point );
//	DelSignleNewEvent( EventUpState, *(OAD+3), ReportFlag );
	DelSignleNewEvent( EventUpState, ReportFlag );
	UpStateShortTypeToString( Ptr, EventUpState );
	if( Data_Comp( Point, Ptr, 5 ) != 0 )					//有差异需要更新
	{
		E2P_WData( UpStateAddr, Ptr, 5 ); 
	}	
}	

//Type: 0:全部清除；1：除了电表清零记录；2：除了电表清零记录和事件记录
void InitialNewEventList( short Type )
{
	if( Type == 0 ) Clr_E2PData( PhFail_UpState, 15, DL698_Event_ProfileSum );	
	else if( Type == 1 ) 
	{
		Clr_E2PData( PhFail_UpState, 15, 49 );
		Clr_E2PData( MDClr_UpState, 15, DL698_Event_ProfileSum-50 );
	}	
	else if( Type == 2 ) 
	{
		Clr_E2PData( PhFail_UpState, 15, 49 );
		Clr_E2PData( MDClr_UpState, 15, 1 );
		Clr_E2PData( TimeSet_UpState, 15, DL698_Event_ProfileSum-52 );
	}	
}	
*/
//================================================================================================
unsigned short GetEventClassTabItemNo_Event_No( unsigned short Event_No )
{
	unsigned short i;
		
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{
		if( Event_No == DL698_Event_ProfileTab[i].EventNo ) break;
	}	
	return i;	
}

unsigned short GetEventClassTabItemNo_OAD3( unsigned char* OAD )		//17.05.19
{
	unsigned short i;
		
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{
		if( Data_Comp( OAD, (unsigned char*)DL698_Event_ProfileTab[i].OAD, 3 ) == 0 ) break;				//查找对应的OBIS码
	}	
	return i;	
}

//Type: 0:全部清除；1：除了电表清零记录；2：除了电表清零记录和事件记录
void InitialNewReportEventList( short Type )							//17.05.19
{
	unsigned char Buff[NEW_REPORT_EVENT_NUM];
	unsigned char* Ptr;
	unsigned char EventRecs;
	unsigned char Recs;
	unsigned short i,j;
	
	Ptr = Buff;
	
	for( i=0;i<3;i++ )
	{
		Recs = 0;
		if( Type != 0 )
		{	
			E2P_RData( (unsigned char*)&EventRecs, NewReportEventL0Recs+i, 1 );	
			if( EventRecs > NEW_REPORT_EVENT_NUM ) EventRecs = 0;
			E2P_RData( Ptr, NewReportEvent0List+i*NEW_REPORT_EVENT_NUM, EventRecs );
			for( j=0;j<EventRecs;j++ )
			{
				if( Type == 1 )
				{
					if( *(Ptr+j) == ECClr_No ) 
					{
						RAM_Write( Ptr, Ptr+j, EventRecs - j );	
						Recs = 1;
						break;
					}	
				}		
				else if( Type == 2 ) 
				{
					if(( *(Ptr+j) == ECClr_No )||( *(Ptr+j) == EventClr_No )) 
					{
						RAM_Write( Ptr, Ptr+j, EventRecs - j );	
						EventRecs -= j;
						j = 1;
						Recs += 1;
						if( Recs >= 2 ) break;
					}	
				}	
			}		
		}	
		E2P_WData( NewReportEventL0Recs+i, (unsigned char*)&Recs, 1 );	
		E2P_WData( NewReportEvent0List+i*NEW_REPORT_EVENT_NUM, Ptr, Recs );
#if ( CarryComm == YesCheck )			//载波通道
		if( i == 2 ) Para.RNewReportEventCarryRecs = Recs;						//17.05.20
#endif
	}	
	InitialClearFollowOADDelay();							//17.05.20
}	

//新增事件列表中删除最新事件
void DelCurFromNewReportEventList( short COM_No )		//17.05.20
{
	unsigned char Buff[NEW_REPORT_EVENT_NUM];
	unsigned char* Ptr;
	unsigned short RecsAddr;
	unsigned short ListAddr;
	unsigned char EventRecs;
	
	Ptr = Buff;
	
	if( COM_No > 3 ) return;
	RecsAddr = NewReportEventL0Recs+COM_No;
	ListAddr = NewReportEvent0List+COM_No*NEW_REPORT_EVENT_NUM;
	E2P_RData( (unsigned char*)&EventRecs, RecsAddr, 1 );	
	if(( EventRecs > NEW_REPORT_EVENT_NUM )||( EventRecs == 0 )) 
	{
		EventRecs = 0;
	}
	else
	{	
		E2P_RData( Ptr, ListAddr, EventRecs );
		RAM_Write( Ptr, Ptr+1, EventRecs - 1 );
		
		EventRecs--;
		E2P_WData( ListAddr, Ptr, EventRecs );	
		E2P_WData( RecsAddr, (unsigned char*)&EventRecs, 1 );	
	}	
#if ( CarryComm == YesCheck )			//载波通道
	if( COM_No == 2 ) Para.RNewReportEventCarryRecs = EventRecs;		//17.05.20
#endif

	return;
}	


//标注对应的上报位，以从新增事件列表中删除
void DelFromNewReportEventList( short COM_No, unsigned char* OAD )		//17.05.19
{
	unsigned char Buff[NEW_REPORT_EVENT_NUM];
	unsigned char* Ptr;
	unsigned short ItemNo;
	unsigned char EventNo;
	unsigned short RecsAddr;
	unsigned short ListAddr;
	unsigned char EventRecs;
	unsigned short i;
	
	Ptr = Buff;
	
//	if(( *(OAD+3) == 0 )||( *(OAD+3) > 10 )) return;		//超出记录范围
	if( *(OAD+3) != 0 ) return;		//超出记录范围
	if( COM_No > 3 ) return;
	ItemNo = GetEventClassTabItemNo_OAD3( OAD );
	if( ItemNo == DL698_Event_ProfileSum ) return;
//	E2P_RData( (unsigned char*)&ReportFlag, DL698_Event_ProfileTab[ItemNo].EReportFlagAddr, 1 ); 
	EventNo = DL698_Event_ProfileTab[ItemNo].EventNo;
//	if(( ReportFlag == 0 )||( ReportFlag > 3 )) return;		//不上报不处理

	RecsAddr = NewReportEventL0Recs+COM_No;
	ListAddr = NewReportEvent0List+COM_No*NEW_REPORT_EVENT_NUM;
	E2P_RData( (unsigned char*)&EventRecs, RecsAddr, 1 );	
	if(( EventRecs > NEW_REPORT_EVENT_NUM )||( EventRecs == 0))
	{
		EventRecs = 0;
	}
	else
	{	
		E2P_RData( Ptr, ListAddr, EventRecs );
		for( i=0;i<EventRecs;i++ )
		{
			if( *(Ptr+i) == EventNo )
			{
				RAM_Write( Ptr+i, Ptr+i+1, EventRecs - i - 1 );
				break; 
			}				
		}	
		
		if( i != EventRecs )
		{
			EventRecs--;
			E2P_WData( ListAddr, Ptr, EventRecs );	
			E2P_WData( RecsAddr, (unsigned char*)&EventRecs, 1 );	
		}
	}
#if ( CarryComm == YesCheck )			//载波通道                                
	if( COM_No == 2 ) Para.RNewReportEventCarryRecs = EventRecs;		//17.05.20
#endif                                                                            
	return;			
}	

//End: 0：事件起始 1：事件结束
void AddToNewReportEventList( unsigned short Event_No, short End )			//17.05.19
{
	unsigned char Buff[NEW_REPORT_EVENT_NUM];
	unsigned char* Ptr;
	unsigned char ReportFlag;	//上报标识
	unsigned short ItemNo;
	unsigned char EventNo;
	unsigned short RecsAddr;
	unsigned short ListAddr;
	unsigned char EventRecs;
	unsigned short i,j;
	
	Ptr = Buff;
	
	ItemNo = GetEventClassTabItemNo_Event_No( Event_No );
	if( ItemNo == DL698_Event_ProfileSum ) return;
	E2P_RData( (unsigned char*)&ReportFlag, DL698_Event_ProfileTab[ItemNo].EReportFlagAddr, 1 ); 
	if(( ReportFlag == 0 )||( ReportFlag > 3 )) return;		//不上报
	if(( ReportFlag == 1 )&&( End != 0 )) return;		//配置为发生上报，结束事件不产生变化
	if(( ReportFlag == 2 )&&( End == 0 )) return;		//配置为结束上报，发生事件不产生变化

	EventNo = DL698_Event_ProfileTab[ItemNo].EventNo;
	for( i=0;i<3;i++ )
	{	
		RecsAddr = NewReportEventL0Recs+i;
		ListAddr = NewReportEvent0List+i*NEW_REPORT_EVENT_NUM;
		E2P_RData( (unsigned char*)&EventRecs, RecsAddr, 1 );	
		if( EventRecs > NEW_REPORT_EVENT_NUM ) EventRecs = 0;
		E2P_RData( Ptr, ListAddr, EventRecs );
		for( j=0;j<EventRecs;j++ )
		{
			if( *(Ptr+j) == EventNo )
			{
				RAM_Write( Ptr+j, Ptr+j+1, EventRecs - j - 1 );
				EventRecs--;
			}				
		}	
		RAM_Write_Rev( Ptr+1, Ptr, EventRecs );
		EventRecs++;
		*Ptr = EventNo;
		E2P_WData( ListAddr, Ptr, EventRecs );	
		E2P_WData( RecsAddr, (unsigned char*)&EventRecs, 1 );	
#if ( CarryComm == YesCheck )			//载波通道                                
		if( i == 2 ) Para.RNewReportEventCarryRecs = EventRecs;		//17.05.20
#endif                                                                            
	}		
	InitialClearFollowOADDelay();							//17.05.20
}

unsigned short GetSignleNewReportEventList( unsigned char* Dest, unsigned short Event_No )			//17.05.19
{
	unsigned char ReportFlag;	//上报标识
	unsigned short ItemNo;
	unsigned short Len;
	
	Len = 1;
	*Dest = 0;
	ItemNo = GetEventClassTabItemNo_Event_No( Event_No );
	if( ItemNo == DL698_Event_ProfileSum ) return Len;
	E2P_RData( (unsigned char*)&ReportFlag, DL698_Event_ProfileTab[ItemNo].EReportFlagAddr, 1 ); 
//	if(( ReportFlag == 0 )||( ReportFlag > 3 )) return;		//不上报

	*Dest = D_OAD;
	RAM_Write( Dest+1, (unsigned char*)DL698_Event_ProfileTab[ItemNo].OAD, 4 );
	*(Dest+4) = 0x01;
	Len = 5;
	
	return Len;
	
}	

unsigned short GetNewReportEventList( short COM_No, unsigned char* Dest, unsigned char* OAD )	//17.05.19
{
	unsigned char Buff[NEW_REPORT_EVENT_NUM];
	unsigned char* Point;
	unsigned short Len;
	unsigned short Length;
	unsigned char* Ptr;
	unsigned short RecsAddr;
	unsigned short ListAddr;
	unsigned char EventRecs;
	unsigned short i;
	
    Ptr = Buff;
    
    *Dest = 0x00;
    Len = 1;
    if( COM_No >= 3 ) return Len;          //非本通道数据
    
	RecsAddr = NewReportEventL0Recs+COM_No;
	ListAddr = NewReportEvent0List+COM_No*NEW_REPORT_EVENT_NUM;
	E2P_RData( (unsigned char*)&EventRecs, RecsAddr, 1 );	
	if( EventRecs > NEW_REPORT_EVENT_NUM ) 
	{
		EventRecs = 0;
		return Len;
	}	
	if( *(OAD+3) > EventRecs ) return Len;	//超出范围
	E2P_RData( Ptr, ListAddr, EventRecs );
	if( *(OAD+3) == 0 )
	{
		*Dest = Data_array;
		*(Dest+1) = EventRecs;
		Len = 2;
		Point = Dest+2;
		for( i=0;i<EventRecs;i++ )
		{
			Length = GetSignleNewReportEventList( Point, (unsigned short)*(Ptr+i) );
			Point += Length;
			Len += Length;
		}	
	}
	else 
	{
		Len = GetSignleNewReportEventList( Dest, (unsigned short)*(Ptr+(*(OAD+3)-1)) );
	}		

	return Len;
}

unsigned short GetNewReportEventListAttribute3( unsigned char* Dest )		//17.05.19
{
	unsigned short Items;
	unsigned short Len;
	unsigned char* Ptr;
	unsigned short Class_Id;		//类别
	unsigned short i;
	unsigned char ReportFlag;	//上报标识
	 
	*Dest = Data_array;
	Ptr = Dest+2;				//前3字节为长度字节预留	
	Items = 0;
	Len = 2;
	for( i=0;i<DL698_Event_ProfileSum;i++ )
	{
		E2P_RData( (unsigned char*)&ReportFlag, DL698_Event_ProfileTab[i].EReportFlagAddr, 1 ); 
		Class_Id = DL698_Event_ProfileTab[i].Class_Id;
		if(( ReportFlag == 0 )||( ReportFlag > 3 )) 
		{
			if( Class_Id == Class_id_PhaseEvent ) i += 3;		//跳过相别
			continue;											//不上报
		}	
		*Ptr = D_OI;
		RAM_Write( Ptr+1, (unsigned char*)DL698_Event_ProfileTab[i].OAD, 2 );
		Items += 1;
		Len += 3;
		Ptr += 3;
		if( Class_Id == Class_id_PhaseEvent ) i += 3;			//跳过相别
	}	
	*(Dest+1) = Items;
	
	return Len;
}

void CheckClearFollowOADDelay( void )							//17.05.20
{
	unsigned char* Ptr;
	unsigned char* Check;
	unsigned short i;
	
	Ptr = (unsigned char*)&HComm.ClearFollowOADDelay0;
	Check = (unsigned char*)&HComm.ClearFollowOADDelayCheck0;
	for( i=0;i<3;i++ )
	{
		if( *Check != ChkNum( Ptr, 2 ) )
		{
			*Ptr = 0;
			*(Ptr+1) = 0;
			*Check = 0xA5;			
		}
		Ptr += 2;
		Check += 1;				
	}	
}	

void InitialClearFollowOADDelay( void )							//17.05.20
{
	unsigned short* ShortPtr;
	unsigned char* Check;
	unsigned short i;
	
	ShortPtr = &HComm.ClearFollowOADDelay0;
	Check = (unsigned char*)&HComm.ClearFollowOADDelayCheck0;
	for( i=0;i<3;i++ )
	{
		*ShortPtr = 0;
		*Check = 0xA5;			
		ShortPtr += 1;
		Check += 1;				
	}	
}	

void DecClearFollowOADDelay( void )								//17.05.20
{
	unsigned char* Ptr;
	unsigned short* ShortPtr;
	unsigned char* Check;
	unsigned short i;
	
	Ptr = (unsigned char*)&HComm.ClearFollowOADDelay0;
	Check = (unsigned char*)&HComm.ClearFollowOADDelayCheck0;
	ShortPtr = &HComm.ClearFollowOADDelay0;
	for( i=0;i<3;i++ )
	{
		if( *Check != ChkNum( Ptr, 2 ) )
		{
			*Ptr = 0;
			*(Ptr+1) = 0;
			*Check = 0xA5;			
		}
		else
		{
			if( *ShortPtr != 0 )
			{
				*ShortPtr -= 1;
				if( *ShortPtr == 0 )
				{
					DelCurFromNewReportEventList( i );					
				}		
				*Check = ChkNum( Ptr, 2 );
			}		
		}	
		Ptr += 2;
		Check += 1;	
		ShortPtr += 1;			
	}	
}

//启动30分钟延时计时
void StartClearFollowOADDelay( short COM_No )							//17.05.20
{
	unsigned char* Ptr;
	unsigned short* ShortPtr;
	unsigned char* Check;
	
	Ptr = (unsigned char*)&HComm.ClearFollowOADDelay0;
    Ptr += COM_No*2;
	ShortPtr = &HComm.ClearFollowOADDelay0;
    ShortPtr += COM_No;
	Check = (unsigned char*)&HComm.ClearFollowOADDelayCheck0;
    Check += COM_No;
	*ShortPtr = 1800;			//30分钟
	*Check = ChkNum( Ptr, 2 );
}	








