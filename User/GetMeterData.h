#ifndef _GETMETERDATA_H
#define _GETMETERDATA_H

#define	RD8213DLY	10//设定时间x毫秒，如果xMs之内都没数据接收，则认为数据已接收完成,延时在0.5毫秒定时器进行
#define	READ8213OK	0x01
#define	WRITE8213OK 0x02
#define Comm8213OK	0x04
#define WriteSuccess 0x06

extern void SynData1s (void);
extern int ID_Read( unsigned char* RAM_Dest,unsigned int ID_Src, int Lenth );
extern int ID_Write( unsigned int ID_Dest, unsigned char* RAM_Src, int Lenth);
extern int ID_Read2(void);
extern unsigned char SYNData_Get(void);

#endif
