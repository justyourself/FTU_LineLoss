#include "TypeMeter.h"
#include "TypeE2p.h"
//#include "msp430f6736.h"
#include "Measure.h"
//#include "Type.h"
#include "TypeRAM.h"
#include "TypeFRAM.h"

#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Port.h"
#include "Display.h"
#include "Power.h"
//#include "RsComm.h"
#include "IECComm.h"
#include "Initial.h"
#include "Time.h"
//#include <Drive_Lib.h>
#include "ProfileGeneric.h"
#include "ECProcess.h"

void GetBatteryWorkTime(void)
{
	unsigned char Buff[12];
	unsigned char* Point;
	unsigned long TempTime,TempTime1;
	
	Point = Buff;
	//E2P_RData( Point, PowerOffTime, 6 );	

	if( Data_Comp( Clk.Hour,Point+1, 4) == 1 )
	{
	TempTime = BCD_Byte(Clk.Year[0])*8760+BCD_Byte(Clk.Mon[0])*720+BCD_Byte(Clk.Day[0])*24+BCD_Byte(Clk.Hour[0]);
//	TempTime1 = BCD_Byte(Buff[4])*8760+BCD_Byte(Buff[3])*720+BCD_Byte(Buff[2])*24+BCD_Byte(Buff[1]);
	TempTime1 = BCD_Byte(Buff[5])*8760+BCD_Byte(Buff[4])*720+BCD_Byte(Buff[3])*24+BCD_Byte(Buff[2]);	

	TempTime = TempTime - TempTime1;
	
	//E2P_RData( Point, BatteryWorkTime, 4 );
	
	TempTime1=BCD4_Long(Point);
	
	TempTime = TempTime+TempTime1;
	
	Long_BCD4(Point, TempTime);

	//E2P_WData(BatteryWorkTime,Point,4);	
	//E2P_RData( SM.BatteryWTime, BatteryWorkTime, 4 );

	}
		
}	



short PowerCheck(void)
{
  if( ((Flag.Power & F_PwrUp) != 0) && ( SM.PowerUpTime < 50 ))
  {
    Flag.Power &= ~F_PwrUp;
    SM.BatUpTime = 49;
    //Bat_on(1);
   // Battery_on(0);
  }	

  if( ((Flag.Power & F_PwrUp) == 0) && ( SM.PowerUpTime >= 50 ))
  {	
    SM.TestDisCnt = TESTDISCNT;
    Flag.Power |= F_PwrUp;	
  }	
  
  if( SM.PowerUpTime >= 50)
  {	
    Flag.Power |= F_PwrUp;	
    return 1;
  }	
  else 
  {	
    if( SM.BatUpTime >= 50)
    {
      Flag.BatState |= F_CommBat;
    }
    else 
    {
      if(Flag.BatState&F_CommBat)
        PwrDnInit();
      Flag.BatState &=~ F_CommBat;
    }
    Flag.Power &= ~F_PwrUp;
    return 0;	
  }	
}	

unsigned char PhAFail(void)
{
    return 0;		
}	

unsigned char PhBFail(void)
{
 return 0;	
}	

unsigned char PhCFail(void)
{
  return 0;	
}		
