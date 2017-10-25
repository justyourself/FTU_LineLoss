#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define SEGBa 0
#define SEGBb 1
#define SEGBc 2
#define SEGBd 3
#define SEGBe 4
#define SEGBf 5
#define SEGBg 6


#define NUMB0 0
#define NUMB1 1
#define NUMB2 2
#define NUMB3 3
#define NUMB4 4
#define NUMB5 5
#define NUMB6 6
#define NUMB7 7

//-------台湾圆表特殊字符宏定义--------
#define TW_DisK1 0x10
#define TW_DisK2 0x20
#define TW_DisK3 0x08
#define TW_DisK4 0x20
#define TW_DisK5 0x10
#define TW_DisK6 0x10
#define TW_DisK7 0x20
#define TW_DisK8 0x10
#define TW_DisK9 0x20
#define TW_DisK10 0x10
#define TW_DisK11 0x20
#define TW_DisK12 0x10
#define TW_DisK13 0x20
#define TW_DisK14 0x10
#define TW_DisK15 0x20
#define TW_DisK16 0x08

#define TW_DisCover 0x10
#define TW_DisALT 0x10
#define TW_DisEOI 0x20
#define TW_DisA 0x20
#define TW_DisB 0x10
#define TW_DisC 0x01

#define TW_DisRateA 0x20
#define TW_DisRateB 0x20
#define TW_DisRateC 0x20
#define TW_DisRateD 0x20

#define TW_DisL1 0x01
#define TW_DisL2 0x10
#define TW_DisL3 0x10
#define TW_DisL4 0x10
#define TW_DisL5 0x20
#define TW_DisL6 0x10

#define TW_DisP1 0x08
#define TW_DisP2 0x08
#define TW_DisP3 0x08
#define TW_DisP4 0x08
#define TW_DisP5 0x10

#define ManualDisMode		1	
#define AutoDisMode			2	
#define PwDownDisMode		3	


#define DISDELAY	60
void KeyProce( void );
short Display( void );

#endif
