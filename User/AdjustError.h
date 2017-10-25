
//#define NormalMode				0
//#define SelfHeatMode			1
//#define Un90Mode				2
//#define Un110Mode				3
//#define OverCu10IbMode			4
//#define OverCuIbMode			5

#define NormalMode				0

#define F49HzImax1_0_Mode		1	
#define F49HzIb1_0_Mode			2
#define F49Hz05Ib1_0_Mode		3	                         	
#define F49Hz02Ib1_0_Mode		4	                            	
#define F49HzImax0_5L_Mode		5	
#define F49HzIb0_5L_Mode		6	
#define F49Hz10Ib0_5L_Mode		7	
#define F49Hz05Ib0_5L_Mode		8	
                            	
#define F51HzImax1_0_Mode		9	
#define F51HzIb1_0_Mode			10
#define F51Hz05Ib1_0_Mode		11	
#define F51Hz02Ib1_0_Mode		12	
#define F51HzImax0_5L_Mode		13	
#define F51HzIb0_5L_Mode		14	
#define F51Hz10Ib0_5L_Mode		15	
#define F51Hz05Ib0_5L_Mode		16	

#define Un115Imax1_0_Mode		17	
#define Un115Ib1_0_Mode			18
#define Un115_05Ib1_0_Mode		19	
#define Un115_02Ib1_0_Mode		20	

#define Un110Imax1_0_Mode		21	
#define Un110Ib1_0_Mode			22
#define Un110_05Ib1_0_Mode		23	
#define Un110_02Ib1_0_Mode		24	
#define Un110Imax0_5L_Mode		25	
#define Un110Ib0_5L_Mode		26	
#define Un110_10Ib0_5L_Mode		27
#define Un110_05Ib0_5L_Mode		28

#define Un90Imax1_0_Mode		29	
#define Un90Ib1_0_Mode			30	
#define Un90_05Ib1_0_Mode		31	
#define Un90_02Ib1_0_Mode		32	
#define Un90Imax0_5L_Mode		33	
#define Un90Ib0_5L_Mode			34
#define Un90_10Ib0_5L_Mode		35	
#define Un90_05Ib0_5L_Mode		36	

#define Un80Imax1_0_Mode		37	
#define Un80Ib1_0_Mode			38	
#define Un80_05Ib1_0_Mode		39	
#define Un80_02Ib1_0_Mode		40	
                                                  
#define SelfHeat_Mode			41	                               

#define ErrorModeNum			41

#define SelfHeatConst			240
//#define OverCu10IbConst			300

//#define SPE_CALIB_STATE 		0x4567				//复位7022，进入特殊校表状态

//#if ( METERTYPE == U220_I10A__40A600 )
//#define	A10Ib98		980000		//100*0.98*10000=980000			
//#define	A10Ib102	1020000		//100*1.02*10000=1020000			
//#define	A98Ib		98000		//10*0.98*10000=98000			
//#define	A102Ib		102000		//10*1.02*10000=102000			
//#define	A98Imax		392000		//40*0.98*10000=392000			
//#define	A102Imax	408000		//40*1.02*10000=408000			
//#define V98Un		2156		//0.98*2200=215.6V		
//#define V102Un		2244		//1.02*2200=224.4V		
//#define V88Un		1936		//0.88*2200=193.6V		
//#define V92Un		2024		//0.92*2200=202.4V		
//#define V108Un		2376		//1.08*2200=237.6V		
//#define V112Un		2464		//1.12*2200=246.4V		
//#elif ( METERTYPE == U220_I10A__60A400 )
//#define	A10Ib98		980000		//100*0.98*10000=980000			
//#define	A10Ib102	1020000		//100*1.02*10000=1020000			
//#define	A98Ib		98000		//10*0.98*10000=98000			
//#define	A102Ib		102000		//10*1.02*10000=102000			
//#define	A98Imax		588000		//60*0.98*10000=588000			
//#define	A102Imax	612000		//60*1.02*10000=612000			
//#define V98Un		2156		//0.98*2200=215.6V		
//#define V102Un		2244		//1.02*2200=224.4V		
//#define V88Un		1936		//0.88*2200=193.6V		
//#define V92Un		2024		//0.92*2200=202.4V		
//#define V108Un		2376		//1.08*2200=237.6V		
//#define V112Un		2464		//1.12*2200=246.4V		
//#elif ( METERTYPE == U57_I1_5A__6A20000 )
//#define	A10Ib98		147000		//15*0.98*10000=147000			
//#define	A10Ib102	153000		//15*1.02*10000=153000			
//#define	A98Ib		14700		//1.5*0.98*10000=14700			
//#define	A102Ib		15300		//1.5*1.02*10000=15300			
//#define	A98Imax		58800		//6*0.98*10000=58800			
//#define	A102Imax	61200		//6*1.02*10000=61200			
//#define V98Un		564			//0.98*575=56.4V		
//#define V102Un		586			//1.02*575=58.6V		
//#define V88Un		506			//0.88*575=50.6V		
//#define V92Un		529			//0.92*575=52.9V		
//#define V108Un		621			//1.08*575=62.1V		
//#define V112Un		644			//1.12*575=64.4V		
//#elif ( METERTYPE == U100_I1_5A__6A20000 )
//#define	A10Ib98		147000		//15*0.98*10000=147000			
//#define	A10Ib102	153000		//15*1.02*10000=153000			
//#define	A98Ib		14700		//1.5*0.98*10000=14700			
//#define	A102Ib		15300		//1.5*1.02*10000=15300			
//#define	A98Imax		58800		//6*0.98*10000=58800			
//#define	A102Imax	61200		//6*1.02*10000=61200			
//#define V98Un		980			//0.98*1000=98.0V		
//#define V102Un		1020		//1.02*1000=102.0V		
//#define V88Un		880			//0.88*1000=88.0V		
//#define V92Un		920			//0.92*1000=92.0V		
//#define V108Un		1080		//1.08*1000=108.0V		
//#define V112Un		1120		//1.12*1000=112.0V		
//#endif


void InitErrorPara( void );
void GetVoltAmperState( void );
void initialErrorStateVar( void );
void ErrorModeJudge( void );
short NGetErrorComVar( unsigned short* Addr, unsigned short* Info, unsigned short* Len, unsigned char* ComID );
unsigned short GetErrorAddr( unsigned short ErrorID );
short BlockInitialAdjustErrorData( unsigned short ErrorID );
short InitialAdjustErrorData( unsigned short ErrorId );
short SwitchErrorMode( unsigned char NewErrorMode );	
unsigned char TestComAdjWrite(unsigned char* ComBuf );
void EnterSpeCalibState( void );
unsigned char Spe_Com_Read(unsigned char* ComBuf);
void Spe_Proc_Read( short ItemAds, unsigned short Addr, unsigned short Info, unsigned short Len, unsigned char* ComBuf );
short ManuEnterSpeState( unsigned char State );





