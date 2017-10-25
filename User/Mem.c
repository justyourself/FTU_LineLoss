#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
//#include "DLT698.h"					
#include "Measure.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
//#include "Mem.h"

#include	"Ram.c"
#include	"E2PROM.c"
//#include	"Flash_CPU.c"
#if ( LoadMFlash == YesCheck )			//V2
#include	"Flash_Data.c"
#else
#endif