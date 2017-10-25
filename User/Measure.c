#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
#include "ht6xxx_lib.h"
#include "TypeFRAM.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"
#include "Port.h"
#include "Display.h"
#include "Initial.h"
#include "Time.h"
#include "Power.h"
#include "ECProcess.h"
#include "Measure.h"
#include "COSEMComm.h"
#include <math.h>


#if( MEASCHIP == HT7017)

unsigned char UART_Write(  unsigned char Cmd, unsigned char* Data )
{
//	unsigned char Checksum;
	unsigned char Temp;
	int i,Delaysum;
	
	HT_FreeDog();
	Com_HT7017.Comm_Enable = DISABLE;
//	NVIC_DisableIRQ(TIMER_2_IRQn);
	for(i=0;i<3;i++)
	{//每一个写操作数据帧都是 6 个字节固定长度
		HT_UART3->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);

		Com_HT7017.MeasBuf_Tx[4] = 0x6A;//传输字节帧头，固定为 0x6A
		Com_HT7017.MeasBuf_Tx[0] = 0x6A;
		Com_HT7017.MeasBuf_Tx[1] = Cmd + 0x80;
		Com_HT7017.MeasBuf_Tx[4] += Cmd + 0x80;
		Com_HT7017.MeasBuf_Tx[2] = *Data;//写寄存器是固定 2 字节传输，高字节在前
		Com_HT7017.MeasBuf_Tx[4] += *Data;
		Com_HT7017.MeasBuf_Tx[3] = *(Data + 1);
		Com_HT7017.MeasBuf_Tx[4] += *(Data + 1);
		Com_HT7017.MeasBuf_Tx[4] = ~Com_HT7017.MeasBuf_Tx[4];//将命令帧的各数据相加，抛弃进位，最后的结果按位取反
		Com_HT7017.MeasState |= ATT7053Set;
		Com_HT7017.MeasState &=~ (ATT7053CommOK + ATT7053Read);
		Delaysum = 3000; //22M 26us  580
		Com_HT7017.Tx_Ptr = 1;
		Com_HT7017.Rx_Ptr = 0;
		Com_HT7017.MeasBuf_Rx[0] = 0;
	//	HT_UART3->UARTCON &=~ UART_UARTCON_RXEN;
		HT_UART3->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;
		HT_UART3->SBUF = Com_HT7017.MeasBuf_Tx[0];			
		HT_UART_ClearITPendingBit(HT_UART3, UART_UARTSTA_TXIF); 
	//	RAM_Fill(Com_HT7017.MeasBuf_Rx, 10 );
	
	Delay(Measur_Delay);
	Delay(Measur_Delay);
	if(Com_HT7017.MeasBuf_Rx[0] == 0x54)
		{	
		Com_HT7017.MeasState &=~ ATT7053Set;
			return 0;
		}
/*
		while((Com_HT7017.MeasState & ATT7053CommOK) != ATT7053CommOK)
		{
//			Delay(10);
			Delay(1);
			Delaysum--;
			if(Delaysum == 0)
			{	
				break;
//				return 1;
			}	
		}
		if(Com_HT7017.MeasBuf_Rx[0] == 0x54)
		{	
//			Delaysum = 30000;
			Temp = 0;
		//	break;
		}
		else 
		{	
//			Delaysum = 30000;
			Temp = 1;
		}
//		NVIC_EnableIRQ(TIMER_2_IRQn);
	*/

	}
	
//	Com_HT7017.MeasState &=~ ATT7053Set;
	return Temp;
}


unsigned char UART_Read(  unsigned char Cmd, unsigned char* Data )
{
	unsigned char Checksum;
	int i,Delaysum;
	
	HT_FreeDog();
	for(i=0;i<2;i++)
	{
		//UCA2IE &=~ (UCRXIE + UCTXIE);
		HT_UART3->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
		Checksum = 0x6A;
		Com_HT7017.MeasBuf_Tx[0] = 0x6A;
		Checksum = 0x6A;
		Com_HT7017.MeasBuf_Tx[1] = Cmd;
		Com_HT7017.Cmd = Cmd;
		Checksum += Cmd;
		Com_HT7017.MeasBuf_Tx[5] = Checksum;
		Com_HT7017.MeasBuf_Tx[5] = ~Com_HT7017.MeasBuf_Tx[5];
		Com_HT7017.MeasState |= ATT7053Read;
		Com_HT7017.MeasState &=~ (ATT7053CommOK + ATT7053Set);
//		Delaysum = 2600;	//4M	2012.8.11
		Delaysum = 3000;	//580
		Com_HT7017.Tx_Ptr = 1;
	//	Com_HT7017.Rx_Ptr = 0;
	//	HT_UART3->UARTCON &=~ UART_UARTCON_RXEN;
		HT_UART3->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
		HT_UART3->SBUF = Com_HT7017.MeasBuf_Tx[0];			
/*		RAM_Fill(Com_HT7017.MeasBuf_Rx, 10 );
		while((Com_HT7017.MeasState & ATT7053CommOK) != ATT7053CommOK)
		{
//			Delay(30);
			Delay(1);
			Delaysum--;
			if(Delaysum == 0)
			{
//				Com_HT7017.Rx_Ptr=0;
//				RAM_Fill(Com_HT7017.MeasBuf_Rx, 10 );
//				RAM_Fill(Data, 3 );
//				return 1;	
				break;
			}
		}
			
		Checksum += (Com_HT7017.MeasBuf_Rx[0] + Com_HT7017.MeasBuf_Rx[1] + Com_HT7017.MeasBuf_Rx[2]);
		Checksum = ~Checksum;
		if(Checksum == Com_HT7017.MeasBuf_Rx[3])
			break;
		else
			{
			Com_HT7017.Rx_Ptr=0;
			RAM_Fill(Com_HT7017.MeasBuf_Rx, 5 ); 
			}
		*/
	}
/*	//数据帧输出时，高字节在前，低字节在后。对于不足 3 个字节的寄存器， HT7017 的内部寄存器与数据帧的低位对齐。
	*Data = Com_HT7017.MeasBuf_Rx[2];
	*(Data + 1)= Com_HT7017.MeasBuf_Rx[1];
	*(Data + 2)= Com_HT7017.MeasBuf_Rx[0];
	Com_HT7017.MeasState &=~ ATT7053Read;
	*/
	return 0;
}
unsigned char UART_ReadInstan(  unsigned char Cmd )
{
	unsigned char Checksum,i;
	
	HT_FreeDog();
//	for(i=0;i<2;i++)
//	{
		//UCA2IE &=~ (UCRXIE + UCTXIE);
		HT_UART3->UARTCON &=~ (UART_UARTCON_TXIE + UART_UARTCON_TXEN);
		Checksum = 0x6A;
		Com_HT7017.MeasBuf_Tx[0] = 0x6A;
		Checksum = 0x6A;
		Com_HT7017.MeasBuf_Tx[1] = Cmd;
		Com_HT7017.Cmd = Cmd;
		Checksum += Cmd;
		Com_HT7017.MeasBuf_Tx[5] = Checksum;
		Com_HT7017.MeasBuf_Tx[5] = ~Com_HT7017.MeasBuf_Tx[5];
		Com_HT7017.MeasState |= ATT7053Read;
		Com_HT7017.MeasState &=~ (ATT7053CommOK + ATT7053Set);
//		Delaysum = 2600;	//4M	2012.8.11
//		Delaysum = 3000;	//580
		Com_HT7017.Tx_Ptr = 1;
		Com_HT7017.Rx_Ptr = 0;
		HT_UART3->UARTCON |= (UART_UARTCON_TXIE + UART_UARTCON_TXEN);//UCA2IE |= UCTXIE;			
		HT_UART3->SBUF = Com_HT7017.MeasBuf_Tx[0];			

//	}
	return 0;
}

unsigned char MasureData_Get(void)
{
	unsigned char Checksum,i;
	unsigned char Error_Byte;
	unsigned char buff[4];
	unsigned char* Data_ptr;
	Data_ptr = buff;
	RAM_Fill(Data_ptr,4);
	Checksum = 0x6A;
	Checksum += Com_HT7017.Cmd;

	Checksum += (Com_HT7017.MeasBuf_Rx[0] + Com_HT7017.MeasBuf_Rx[1] + Com_HT7017.MeasBuf_Rx[2]);
	Checksum = ~Checksum;
	if(Checksum == Com_HT7017.MeasBuf_Rx[3])
		{
		*(Data_ptr + 3) = Com_HT7017.MeasBuf_Rx[2];
		*(Data_ptr + 2)= Com_HT7017.MeasBuf_Rx[1];
		*(Data_ptr + 1)= Com_HT7017.MeasBuf_Rx[0];
		Error_Byte = 0;
		for(i=0; i<Measur_Num; i++)
			{
			if(HT7017_DATATab[i].ReadCmd == Com_HT7017.Cmd) 
				break;
			}
		if(i==Measur_Num) 
			Error_Byte = 1;
		else
			{
			RAM_Write(HT7017_DATATab[i].ReadData,Data_ptr,4);
			}
		Com_HT7017.MeasState |= ATT7053CommOK;
		Com_HT7017.MeasState &=~ ATT7053Read;
		Com_HT7017.Rx_Ptr=0;
		RAM_Fill(Com_HT7017.MeasBuf_Rx,10);
		}
	else
		{
		Error_Byte = 1;
		}
	return Error_Byte;
}
#if 0
void ChkMeasChipRST(unsigned int phase)
{
	unsigned char *Point;
	
	Point = (unsigned char *)&MeasureHT7017.APowerPulP;	//2012.12.19
	
	if( ChkMeasChkSum(phase) == 1 )	
	{
		SoftResetMeas(phase);
		ATT7053Int(phase);
		RAM_Fill( Point+4*phase, 4);
	}
}

void CheckMeasChip(void)
{
	if((Flag.HT7017PowerOn&F_PhaATT7053) == F_PhaATT7053)
	{
	 	ATT7053Int(0); 
		MeasureHT7017.APowerPulP=0;   	 	
	}
	else
	{
		ChkMeasChipRST(0);
	}

/*
	if((Flag.ATT7053PowerOn&F_PhaATT7053) == F_PhaATT7053)
	{
	 	ATT7053Int(0); 
		Flag.APowerPulP=0;   	 	
	}
	else
	{
		ChkMeasChipRST(0);
	}
	
	if((Flag.ATT7053PowerOn&F_PhbATT7053) == F_PhbATT7053)
	{
	  	ATT7053Int(1); 
		Flag.BPowerPulP=0;      	  	
	}
	else
	{
		ChkMeasChipRST(1);		
	}

	if((Flag.ATT7053PowerOn&F_PhcATT7053) == F_PhcATT7053)
	{
		ATT7053Int(2);
		Flag.CPowerPulP=0;            		
	}
	else
	{
		ChkMeasChipRST(2);		
	}  
	*/
}

unsigned char ChkMeasChkSum(unsigned int phase)
{
	unsigned char Buff[4];
	unsigned char *Point;
	
	Point = Buff;
	/*
	switch(phase)
	{
		case 0:
	        PDIR_CSA |= P_CSA;
            PDIR_CSC &= ~P_CSC;
	        PDIR_CSB &= ~P_CSB;
			break;
		case 1:
			PDIR_CSA &= ~P_CSA;
			PDIR_CSB |= P_CSB;
			PDIR_CSC &= ~P_CSC;				
			break;
		default:	
			PDIR_CSA &= ~P_CSA;
			PDIR_CSB &= ~P_CSB;
			PDIR_CSC |= P_CSC;				
			break;	
	}	
	*/
	ATT7053RdReg(SUMChecksum,Point);	
	
	if(Data_Comp(Point, Para.MeasChkSum, 3)!=0)
	{
		E2P_RData( Para.MeasChkSum,MeasChkSum, 3 );
		if(Data_Comp(Point, Para.MeasChkSum, 3)!=0)
		{
			ATT7053RdReg(SUMChecksum,Point);
			if(Data_Comp(Point, Para.MeasChkSum, 3)!=0)
			{
			/*
				PDIR_CSA &= ~P_CSA;
				PDIR_CSC &= ~P_CSC;
				PDIR_CSB &= ~P_CSB;
				*/
				return 1;
			}
		}			
	}
/*	
	PDIR_CSA &= ~P_CSA;
	PDIR_CSC &= ~P_CSC;
	PDIR_CSB &= ~P_CSB;
	*/
	return 0;	
}
#endif
void SoftResetMeas(unsigned int phase)
{
	unsigned char Buff[3];
/*	
	switch(phase)
	{
		case 0:
	        PDIR_CSA |= P_CSA;
            PDIR_CSC &= ~P_CSC;
	        PDIR_CSB &= ~P_CSB;
			break;
		case 1:
			PDIR_CSA &= ~P_CSA;
			PDIR_CSB |= P_CSB;
			PDIR_CSC &= ~P_CSC;				
			break;
		default:	
			PDIR_CSA &= ~P_CSA;
			PDIR_CSB &= ~P_CSB;
			PDIR_CSC |= P_CSC;				
			break;	
	}	
*/
	Buff[0]=0x00;
	Buff[1]=0x55;
	UART_Write(SRSTREG_Address,Buff);	//软件复位ATT7053	
/*	
	PDIR_CSA &= ~P_CSA;
	PDIR_CSC &= ~P_CSC;
	PDIR_CSB &= ~P_CSB;	
	*/
	Delay(300);
}

short HT7017Init(void)
	{
		unsigned long Value;
		unsigned int  Temp;
		unsigned char datbuff[5];
		unsigned char* Point;
		
		Point = datbuff;
		HT_FreeDog();
	/*
			UART_ReadInstan(IDCode);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			MasureData_Get();
			RAM_Write(Point,(unsigned char *)&Measurement.HT7017_ID[1],3);

			if((datbuff[2]!= 0xB0)||(datbuff[1]!= 0x53)||(datbuff[0]!= 0x70))
			{		
			Flag.HT7017PowerOn |= F_PhaATT7053;
				return 1;			
			}
			else
			{
			Flag.HT7017PowerOn &= ~F_PhaATT7053;
			}	
			
			datbuff[0]=0x7E;
			datbuff[1]=0x7F;
			UART_Write(EMUIE_Address,Point);			
			Delay(Measur_Delay);
			Delay(Measur_Delay);
	//	//	WPCFG = 0xBC：表示写保护打开，只能操作40H到45H的校表参数寄存器，不可操作50H到71H的校表参数寄存器//
	//	//	WPCFG = 0xA6：表示写保护打开，只能操作50H到71H的校表参数寄存器，不可操作40H到45H的校表参数寄存器。//
			datbuff[0]=0x00;
			datbuff[1]=0xBC;
			UART_Write(WPREG_Address,Point);	//使能40-45写操作//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value = 0x0000BC;	//EMUCFG
			
		//	Value += 0x000088;	//FreqCFG
	
		//	Value += 0x00007E;	//ModuleEn
					
			//datbuff[0]=0x40;//设置是否能量寄存器读后清 0（0：读后不清 0； 1：读后清 0）
			//datbuff[1]=0x12;//单相三线模式控制： 1=矢量和,单相三线累加模式选择（1：电流累加和模式）
			datbuff[0]=0xBE;//设置是否能量寄存器读后清 0（0：读后不清 0； 1：读后清 0）
			//datbuff[0]=0x9E;
			datbuff[1]=0x12;
			UART_Write(EMUCFG_Address,Point);	//EMU 配置寄存器
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x004012;		
		//	datbuff[0]=0x00;
		//	datbuff[1]=0xC8;
			datbuff[0]=0x01;
			datbuff[1]=0x48;
			UART_Write(FreqCFG_Address,Point);	//时钟/更新频率配置寄存器
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x000148;
			
			datbuff[0]=0x00;
			datbuff[1]=0x7E;
			UART_Write(ModuleEn_Address,Point);	//EMU 模块使能寄存器
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x00007E;

			datbuff[0]=0x00;
			datbuff[1]=0x0F;
			UART_Write(ANAEN,Point);	//ADC开关寄存器//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x000007;	//ADC开关寄存器

			Value += 0x0000A6;	//IOCFG
					
			datbuff[0]=0x00;
			datbuff[1]=0xA6;
			UART_Write(WPREG_Address,Point);	//使能50-7C写操作//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x0000A6;	

	//		datbuff[0]=0xFE;
	//		datbuff[1]=0x98;
	//		UART_Write(QPhsCal_Address,Point);	//当 femu=1MHz 时， 60Hz 信号频率下此寄存器写入 FE98H 即可，也不需要额外校准
	//		Delay(Measur_Delay);

			E2P_RAdj( datbuff, I2Gain, 2 );//电流通道 2 增益设置
			SwapData(Point,2);
			UART_Write(I2Gain_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			
			E2P_RAdj( datbuff, HFCONST, 2 );//高频脉冲常数设置
			SwapData(Point,2);
			UART_Write(HFConst_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			
	//		ATT7053RdReg(WPREG,Point);	//使能50-71写操作//
	//-----------电压电流有效值偏置校正----------
			E2P_RAdj( datbuff, I1RMSOFFSET, 2 );
			UART_Write(I1RMSOFFSET_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			E2P_RAdj( datbuff, I2RMSOFFSET, 2 );
			UART_Write(I2RMSOFFSET_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			E2P_RAdj( datbuff, URMSOFFSET, 2 );
			UART_Write(URMSOFFSET_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			
			E2P_RAdj( datbuff, AGPBase, 2 );
			SwapData(Point,2);
	//		datbuff[0]=0x13;
	//		datbuff[1]=0x20;
			UART_Write(GP1,Point);	//eeprom 3字节，实际使用2字节//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
	//		ATT7053RdReg(GP1,Point);	//eeprom 3字节，实际使用2字节//
			UART_Write(GQ1,Point);	//eeprom 3字节，实际使用2字节//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(GS1,Point);	//eeprom 3字节，实际使用2字节//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
	//		ATT7053WtReg(GP2,Point);	//eeprom 3字节，实际使用2字节//
			Temp = (*Point*256 + *(Point+1));
			
			Value += Temp;	//GP1		
			Value += Temp;	//GQ1
			Value += Temp;	//GS1

			E2P_RAdj( datbuff, AGPBase2, 2 );
			SwapData(Point,2);
			UART_Write(GP2_Address,Point);	//eeprom 3字节，实际使用2字节//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(GQ2_Address,Point);	//eeprom 3字节，实际使用2字节//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(GS2_Address,Point);	//eeprom 3字节，实际使用2字节//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = (*Point*256 + *(Point+1));
			
			Value += Temp;	//GP1		
			Value += Temp;	//GQ1
			Value += Temp;	//GS1
			
			Value += 0x00FF00;	//QPhsCal

			//需要定死的寄存器//
			datbuff[0]=0x00;
			datbuff[1]=0x00;
			UART_Write(ADCCON,Point); //电流1放大16倍，电压放大1倍,电流2放大4倍//
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Value += 0x00003C;	//ADCCON

			Value += 0x000040 + 0x000040;	//PQStart,HFConst
	
			Value += 0x000010 + 0x000020 + 0x000004;	//CHk,IPTAMP,ZCrossCurrent
	
			E2P_RAdj( Point, E2AP1OFFSET, 2 );		//2012.12.19
			UART_Write(P1OFFSET,Point+1);	//P1OFFSET
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(P1OFFSETH_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = *(Point+1);
			Value += Temp;
			Value += *Point;
			
			E2P_RAdj( Point, E2AP2OFFSET, 2 );		//2012.12.19
			UART_Write(P2OFFSET,Point+1);	//P1OFFSET
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			UART_Write(P2OFFSETH_Address,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = *(Point+1);
			Value += Temp;
			Value += *Point;
					
			E2P_RAdj( datbuff, AGPhs, 2 );
			SwapData(Point,2);
			UART_Write(GPhs1,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = (*Point*256 + *(Point+1));
			Value += Temp;	
			
			E2P_RAdj( datbuff, AGPhs2, 2 );
			SwapData(Point,2);
			UART_Write(GPhs2,Point);
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			Temp = (*Point*256 + *(Point+1));
			Value += Temp;	
				
	//		readep(&rambuf[1],EE_PHSA_7053,3,eplxjbcs);
	//		rambuf[0]=0;
	//		jchkprg(&rambuf[1]);
	//		wr7053(rambuf,0x6d+0X80,3); //相位校准
				
			datbuff[0]=0x00;
			datbuff[1]=0x00;
			datbuff[2]=0x00;
			UART_Write(WPREG_Address,Point);	//关闭使能			
			Delay(Measur_Delay);
			Delay(Measur_Delay);
			RAM_Write( Para.MeasCheckSum, (unsigned char*)&Value, 3 ); 		//ATChk 
			E2P_WData( MeasChkSum, (unsigned char *)&Value, 3 );
			
	//	}
*/	
	//	ATT7053RdReg(ADCCON,Point);
	//	ATT7053RdReg(GP1,Point);
	//	ATT7053RdReg(Rms_U,Point);
		/*
		PDIR_CSA &= ~P_CSA;
		PDIR_CSC &= ~P_CSC;
		PDIR_CSB &= ~P_CSB;
		*/
		Flag.HT7017PowerOn &=~ F_Reset7053;	
		return 0;
	}
short Read_ATTValue( unsigned char cmd, unsigned char* Point , unsigned char length )
{
	unsigned char buff[4];
	unsigned char* ptr;
	short return_value=0;
	ptr = buff;
	RAM_Fill(ptr,4);
	if(UART_Read(cmd,ptr)==0)
		{
			SwapData(ptr,4);
			RAM_Write(Point,ptr,4);
			return_value=0;
		}
	else
		return_value=-1;
	return return_value;
}

void ReadHT7017_V(void)
{
	unsigned char Buff[4];
	unsigned char *Point;
	unsigned long Temp;
	unsigned long long Temp2;
	Point = Buff;
/*	
	Temp = GetBuffToLong(Measurement.HT7017_U);
	if(MeasureHT7017.Urms_K!=0)
		Temp2 = MeasureHT7017.Urms_K;
	else
		Temp2 = 1;
	Temp2 *= Temp;
	Temp2 /= 100000;
	Temp2 /= 1000;//保留两位小数
	Real.Uab = (unsigned short)Temp2;
	Word_BCD(Para.Ua,Real.Uab);
*/	
}
void ReadHT7017_HZ(void)
{
	unsigned char Buff[4];
	unsigned char *Point;
	unsigned long Temp2;
	Point = Buff;
	
	Temp2 = GetBuffToLong(Measurement.HT7017_Hz);
	Temp2 = 1000000/Temp2;//EMU 时钟（femu）选择为 1MHz 
	Temp2/=2;

	Real.Freq = (unsigned short)Temp2;//hex
	Real.Freq*=100;//2位小数
	Word_BCD(Point,Real.Freq);
	
	RAM_Write( Para.Fre, Point, 2 );//bcd
}

void ReadHT7017_I(void)
{
	unsigned long Temp;	
	unsigned long long Temp2;
	/* 
	Temp = GetBuffToLong(Measurement.HT7017_Ia);
	if(MeasureHT7017.Irms1_K!=0)
		Temp2 = MeasureHT7017.Irms1_K;
	else
		Temp2 = 1;
	Temp2 *= Temp;
	Temp2 /= 100000;
	Temp2 /= 1000;//保留两位小数
	Real.Ia = (unsigned short)Temp2;
	Word_BCD((unsigned char*)&Para.Ia,Real.Ia);

	
	Temp = GetBuffToLong(Measurement.HT7017_Ib);	
	if(MeasureHT7017.Irms2_K!=0)
		Temp2 = MeasureHT7017.Irms2_K;
	else
		Temp2 = 1;
	Temp2 *= Temp;
	Temp2 /= 100000;
	Temp2 /= 1000;//保留两位小数
	Real.Ib = (unsigned short)Temp2;
	*/
}

void ReadHT7017_P(void)
{
	unsigned char Databuff[4];
	unsigned char* Point;
	int State;
//	unsigned long int Power; 
	long Power1,Power2,Power,angle,power_factor; 
	long Temp;
	long long temp2,temp3;
	double anglezA;
	Power1 = 0;
	Power2 = 0;
	Point = Databuff;
#if 0
	if(MeasureHT7017.Power1_K==0)
		MeasureHT7017.Power1_K=1;
	if(MeasureHT7017.Power2_K==0)
		MeasureHT7017.Power2_K=1;

		Temp = GetBuffToLong(Measurement.HT7017_P1);
		if(Temp<0x800000)
			Power1 = Temp;
		else
			{
			Power1 = -(0x1000000-Temp);
			}
		MeasureHT7017.APowerPulP= Power1;
		temp2 = MeasureHT7017.Power1_K;
		temp2 *= Power1;
	Power = temp2;
	//-----第一通道角度--------	
		temp2 = GetBuffToLong(Measurement.HT7017_U);
		temp3 = MeasureHT7017.APowerPulP;
		anglezA = (double)((temp3*100)/(temp2*GetBuffToLong(Measurement.HT7017_Ia)/100000));
		anglezA/=1000;
		if(anglezA > 100000) anglezA = 100000;
		Real.Angle_Ia = (unsigned short)(acos(anglezA)*1800/3.1416)*10;
		
/*	temp2 = Real.Uab;
	temp3 = Power;
	anglezA = (double)((temp3*100)/(temp2*Real.Ia));
	anglezA/=1000;
	if(anglezA > 100000) anglezA = 100000;
	Real.Angle_Ia = (unsigned short)(acos(anglezA)*1800/3.1416)*10;
*/
		Temp = GetBuffToLong(Measurement.HT7017_P2);
		if(Temp<0x800000)
			Power2 = Temp;
		else
			{
			Power2 = -(0x1000000-Temp);
			}
		MeasureHT7017.BPowerPulP= Power2;
		temp2 = MeasureHT7017.Power2_K;
		temp2 *= Power2;
		Power+=temp2;
		Power = Power/100000;
		Real.Pa = Power*100;//w位小数
		if(Real.Pa<0)
			Real.Pa = -Real.Pa;
		//-----第二通道角度--------
		
		temp2 = GetBuffToLong(Measurement.HT7017_U);
		temp3 = MeasureHT7017.BPowerPulP;
		anglezA = (double)((temp3*100)/(temp2*GetBuffToLong(Measurement.HT7017_Ib)/100000));
		anglezA/=1000;
		if(anglezA > 100000) anglezA = 100000;
		Real.Angle_Ib= (unsigned short)(acos(anglezA)*1800/3.1416)*10;
/*		temp3 = temp2;
		temp2 = Real.Uab;
		anglezA = (double)((temp3*100)/(temp2*Real.Ib));
		anglezA/=1000;
		if(anglezA > 100000) anglezA = 100000;
		Real.Angle_Ib= (unsigned short)(acos(anglezA)*1800/3.1416)*10;
*/
		Temp = GetBuffToLong(Measurement.HT7017_S1);
		if(Temp<0x800000)
			Power1 = Temp;
		else
			{
			Power1 = -(0x1000000-Temp);
			}
		MeasureHT7017.SPowerPulP = Power1;
		if(MeasureHT7017.PowerS_K!=0)
			temp2 =MeasureHT7017.PowerS_K;
		else
			temp2 = 1;
		temp2 *= Power1;
		temp2/=100000;
		Power1 = temp2;
		Real.St = Power1*100;;

		Temp = GetBuffToLong(Measurement.HT7017_Q1);
		if(Temp<0x800000)
			Power1 = Temp;
		else
			{
			Power1 = -(0x1000000-Temp);
			}
		MeasureHT7017.APowerPulQ= Power1;	
		temp2 = MeasureHT7017.Power2_K;
		temp2 *= Power1;

		Temp = GetBuffToLong(Measurement.HT7017_Q2);
		if(Temp<0x800000)
			Power2 = Temp;
		else
			{
			Power2 = -(0x1000000-Temp);
			}
		MeasureHT7017.BPowerPulQ= Power2;
		temp2 = MeasureHT7017.Power2_K;
		temp2 *= Power2;
	
	power_factor = ((MeasureHT7017.APowerPulP+MeasureHT7017.BPowerPulP)*100)/MeasureHT7017.SPowerPulP;
	//power_factor*=10;
	if(power_factor<0)
		power_factor = -power_factor;
	Real.Pfa = (unsigned char)power_factor;
	Word_BCD((unsigned char *)& Para.Pt,(unsigned short)power_factor);
#endif
}
void Instan_Value(void)
{
	ReadHT7017_V();
	ReadHT7017_I();
	ReadHT7017_HZ();
	ReadHT7017_P();

}

void Energy_Read(void)
{
	unsigned long Temp;

		Temp =	GetBuffToLong(Measurement.HT7017_P);
		ECInt.YPulse_Cum = (unsigned char)Temp;
		ECInt.YPulse_CumChk += ECInt.YPulse_Cum ;
	
		Temp = GetBuffToLong(Measurement.HT7017_Q);
		ECInt.WPulse_Cum = (unsigned char)Temp;
		ECInt.WPulse_CumChk += ECInt.WPulse_Cum ;	
		
		RAM_Fill(Measurement.HT7017_P,4);
		RAM_Fill(Measurement.HT7017_Q,4);
		if(Measurement.HT7017_EMU_Status[3]&0x02)
			Real.PWFlag |= 0x10;//有功反向
		else
			Real.PWFlag &=~ 0x10;
		if(Measurement.HT7017_EMU_Status[3]&0x01)
			Real.PWFlag |= 0x01;//无功反向
		else
			Real.PWFlag &=~ 0x01;
	
}

#endif

void Read_CMonEC( unsigned char* Dest, unsigned short ECRgAds )
	{
		unsigned short i;
		unsigned short ECEAds;
		unsigned short ECFAds;
		unsigned char* ECRAds;
		
	//	if( CombECJust( Dest, ECRgAds ) == 0 ) return;	//组合电量判断 
		
		for( i=0;i<ECUnitNum;i++ )
		{
			ECEAds = ECRgTab[i].ECEAds; 
			ECRAds = ECRgTab[i].ECRAds; 
			ECFAds = ECRgTab[i].ECFAds;
	
			if( ECEAds == ECRgAds ) break;				//总电量
			else if(( i < (ECUnitNum-1) )&&( ECEAds < ECRgAds )&&( ECRgTab[i+1].ECEAds > ECRgAds )) //费率电量
			{
				if(((( ECRgAds - ECEAds ) % (ECDIGIT+1) ) == 0 )&&( ECRgTab[i].ECRgMode == 0 )) //有费率电量且费率电量地址正确
				{
					ECRAds += ECRgAds - ECEAds; 	//费率电量RAM地址
					ECFAds += ECRgAds - ECEAds; 	//费率电量铁电地址
					break;
				}	
			}			
		}	
		if( i == ECUnitNum ) 
		{
			RAM_Fill( Dest, ECDIGIT );				//电量地址错误
			return; 		
		}	
		GetRamEC( Dest, ECRAds, ECFAds );			//读取RAM或FRAM中的电量
	
	}

void Read_CMonEC4( unsigned char* Dest, unsigned short ECRgAds )
{
	unsigned char Buff[6];
	
	Read_CMonEC( Buff, ECRgAds );	
	RAM_Write( Dest, Buff+1, 4 );
	
}	

void Read_CMonEC4_HEX_U( unsigned char* Dest, unsigned short ECRgAds )			//16.08.29
{
	unsigned long Value;
	
	Read_CMonEC4( Dest, ECRgAds );
	Value = BCD4_Long( Dest );
	RAM_Write( Dest, (unsigned char*)&Value, 4 );
}	

void Read_CMonEC4_HEX_S( unsigned char* Dest, unsigned short ECRgAds )			//16.08.29
{
	long Value;
	short Temp=0;
	
	Read_CMonEC4( Dest, ECRgAds );
	if(( *(Dest+3) & 0x80 ) != 0 ) 
	{
		*(Dest+3) &= ~0x80;
		Temp = 1;
	}	
	Value = BCD4_Long( Dest );
	if( Temp == 1 ) Value = 0 - Value;
	RAM_Write( Dest, (unsigned char*)&Value, 4 );
}	

void Read_CMonEC8_HEX_U( unsigned char* Dest, unsigned short ECRgAds )			//16.08.29
{
	unsigned long long LValue;
	
	Disk.DigitsValidFlg = 0x55;										//添加0.01kWh以下尾数		
	Read_CMonEC( Dest, ECRgAds );
	Disk.DigitsValidFlg = 0x00;
	LValue = *(Dest+4);
	LValue <<= 32;
	LValue += BCD4_Long( Dest );
	RAM_Write( Dest, (unsigned char*)&LValue, 8 );
}	

void Read_CMonEC8_HEX_S( unsigned char* Dest, unsigned short ECRgAds )			//16.08.29
{
	long long LValue;
	short Temp=0;
	
	Disk.DigitsValidFlg = 0x55;										//添加0.01kWh以下尾数		
	Read_CMonEC( Dest, ECRgAds );
	Disk.DigitsValidFlg = 0x00;
	if(( *(Dest+4) & 0x80 ) != 0 ) 
	{
		*(Dest+4) &= ~0x80;
		Temp = 1;
	}	
	LValue = *(Dest+4);
	LValue <<= 32; 
	LValue += BCD4_Long( Dest );
	if( Temp == 1 ) LValue = 0 - LValue;
	RAM_Write( Dest, (unsigned char*)&LValue, 8 );
}	

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
void FeeECPulseAdd( unsigned short* PL_Cum, unsigned char* PL_Chk, unsigned char Pulse_Cum )
{
	unsigned short* PL_CumPtr;
	unsigned char* PL_ChkPtr;
	unsigned short Temp;
	
	if(( SM.FeeNo < 1 )||( SM.FeeNo > Total_Fee )) return;				//当前费率号非法

	Temp = SM.FeeNo;
	Temp -= 1; 
	PL_CumPtr = PL_Cum;
	PL_CumPtr += Temp;
	PL_ChkPtr = PL_Chk;
	PL_ChkPtr += Temp;
//新国网		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//	*PL_CumPtr += (unsigned short)Pulse_Cum * 10;	
//#else
	*PL_CumPtr += Pulse_Cum;	
//#endif
	*PL_ChkPtr = ChkNum( (unsigned char*)PL_CumPtr, 2 );
}
#endif

short GetMQuad( unsigned char PWFlag, short Reg )
{
    short Quad=1;
        
//	PWFlag = PWFlag >> Reg;	
	PWFlag &= 0x11;
	switch( PWFlag )
	{
		case 0: Quad = 1; break;	
		case 0x10: Quad = 2; break;	
		case 0x11: Quad = 3; break;	
		case 0x01: Quad = 4; break;	
		default: break;
	}	
	return 	Quad;
}

#if 0
short RAMECIsOK( unsigned char* ECRAds )
{
	if(( ChkNum( ECRAds, ECDIGIT ) == *(ECRAds+ECDIGIT) )&&( AllIsBCD( ECRAds, ECDIGIT ) == 0 )) return 0;
	else return 1;
}	

short E2PECIsOK( unsigned char* ECRAds )
{
	if((( Flag.BatState & F_E2PCheck ) == 0 )&&( AllIsBCD( ECRAds, ECDIGIT ) == 0 )) return 0;
	else return 1;
}
//取当前铁电电量地址在ECRgTab表内对应的行号
short GetECRgTabRowNo( unsigned short ECFAds )		
{
	short i;
	unsigned short StartNo,EndNo;
	
	for( i=0;i<ECUnitNum;i++ )
	{
		StartNo = ECRgTab[i].ECFAds;
		EndNo = StartNo + 5*(ECDIGIT+1);
		if( ECRgTab[i].ECRgMode == 0 )			//电量包含费率
		{
			if(( ECFAds >= StartNo )&&( ECFAds < EndNo )&&((( ECFAds-StartNo)%(ECDIGIT+1)) == 0 )) break;			
		}
		else
		{
			if( StartNo == ECFAds ) break;		//不包含费率
		}		
	}	
	if( i == ECUnitNum ) return -1;
	else return i;
}
void ECBCDAdd( unsigned char* Dest, unsigned char* Source )
{
#if ( ECDIGIT == BYTE5 )
	_BCD5ADD( Dest, Source );	
#else
	_BCD4ADD( Dest, Source );	
#endif
}	

void ECBCDSub( unsigned char* Dest, unsigned char* Source )
{
#if ( ECDIGIT == BYTE5 )
	_BCD5SUB( Dest, Source );	
#else
	_BCD4SUB( Dest, Source );	
#endif
}

//检查RAM中的分时电量之和是否等于总
//Source: 存放电量的RAM区
//Type: 0：RAM区不包含每个电量的校验和； 1：每个电量后跟一个校验和。
short TotalFeeECCheck( unsigned char* Source, short Type )
{
	unsigned char Buff[8];
	unsigned char* Ptr;
	unsigned short Len;
	short i;
#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
	unsigned long Value1;
	unsigned long Value2;
#endif
	
	Ptr = Buff;
	
	if( Type == 0 ) Len = ECDIGIT;
	else Len = ECDIGIT + 1;
	
	RAM_Fill( Ptr, ECDIGIT );
	for( i=0;i<4;i++ )
	{
		ECBCDAdd( Ptr, Source+(i+1)*Len );
	}	
#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
#if ( ECDIGIT == BYTE5 )
	Value1 = BCD4_Long( Source+1 );
	Value2 = BCD4_Long( Ptr+1 );
#else
	Value1 = BCD4_Long( Source );
	Value2 = BCD4_Long( Ptr );
#endif
	if(( Value1 >= Value2 )&&(( Value1 - Value2 ) <= 4 )) return 0;		//总等于分时电量之和 
	else return 1;
#else
	if( Data_Comp( Ptr, Source, ECDIGIT ) != 0 ) return 1;		//总不等于分时电量之和 
	else return 0;
#endif
}

//根据EEPROM中保存的值进行数据恢复
void ResumeEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds )
{
	short i;
	short ECRgTabRowNo; 
	unsigned char Buff[30];
	unsigned char* Ptr;
	unsigned char Buff2[10];
	unsigned char* Point;
	unsigned short Addr;
	unsigned char State=0;
	unsigned char* TempPtr;
	
	Ptr = Buff;
	Point = Buff2;
	
	ECRgTabRowNo = GetECRgTabRowNo( ECFAds );
	if( ECRgTabRowNo == -1 ) return;									//正常FRAM电量地址
                                                                		
	Addr = ECRgTab[ECRgTabRowNo].ECEAds;								//取对应的EEPROM中电量的地址
	if( ECRgTab[ECRgTabRowNo].ECRgMode == 0 )							//电量包含费率
	{                                                           		
		TempPtr = Ptr;                                          		
		for( i=0;i<5;i++ )												//读取EEPROM中的总及各费率电量	
		{                                                       		
			E2P_RData( TempPtr, Addr, ECDIGIT );	            		
			if( E2PECIsOK( TempPtr ) != 0 )                     		
			{                                                   		
				State = 1;												//EEPROM数据有错	
				break;                                          		
			}	                                                		
			TempPtr += ECDIGIT;                                 		
			Addr += ECDIGIT+1;                                  		
		}	                                                    		
		if( State == 0 )                                        		
		{                                                       		
			if( TotalFeeECCheck( Ptr, 0 ) != 0 ) State = 1;				//总不等于分时电量之和 
		}	
		E2P_RFM( Point, ECFAds, ECDIGIT );								//再次读取FRAM数据。	
		if( E2PECIsOK( Point ) == 0 ) 
		{
			RAM_Write( Dest, Point, ECDIGIT );							//铁电中电量读取正确，以铁电电量为准，放弃从RRPROM中取得的数据
			RAM_Write( ECRAds, Dest, ECDIGIT );							//RAM中的值恢复为正确的铁电数据或备份EEPROM数据
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
		}	
		else
		{																//铁电数据还是有误	
			if( State == 0 ) 											//EEPROM数据正确，包括校验和、BCD码、总等于分时费率之和
			{
				TempPtr = ECRgTab[ECRgTabRowNo].ECRAds;
				Addr = ECRgTab[ECRgTabRowNo].ECFAds;					//取铁电中电量首址
				RAM_Write( Dest, Ptr+(((ECFAds-Addr)/(ECDIGIT+1))*ECDIGIT), ECDIGIT );	//恢复电量后需有当前项返回值 //10.12.08					
				for( i=0;i<5;i++ )
				{
					E2P_WFM( Addr, Ptr+i*ECDIGIT, ECDIGIT );			//将EEPROM中的备份数据替换掉铁电中的错误电量，但可能导致电量减少。	
					RAM_Write( TempPtr, Ptr+i*ECDIGIT, ECDIGIT );
					*(TempPtr+ECDIGIT) = ChkNum( TempPtr, ECDIGIT );
					TempPtr += ECDIGIT+1;
					Addr += ECDIGIT+1;
				}	
			}	
			else 
			{
				RAM_Write( Dest, Point, ECDIGIT );						//铁电、EEPROM数据均不对，以最近一次读取的铁电数据返回，但不刷新RAM数据
				return;					
			}	
		}	
	}
	else
	{																	//无费率电量
		E2P_RData( Ptr, Addr, ECDIGIT );	
		if( E2PECIsOK( Ptr ) != 0 ) 
		{
			State = 1;													//EEPROM中数据自检错误
		}	
		E2P_RFM( Point, ECFAds, ECDIGIT );								//再次读取FRAM数据。	
		if( E2PECIsOK( Point ) == 0 ) 
		{
			RAM_Write( Ptr, Point, ECDIGIT );							//铁电中电量读取正确，以铁电电量为准，放弃从RRPROM中取得的数据
		}	
		else
		{																//铁电数据还是有误	
			if( State == 0 ) E2P_WFM( ECFAds, Ptr, ECDIGIT );			//将EEPROM中的备份数据替换掉铁电中的错误电量，可能导致电量减少。		
			else 
			{
				RAM_Write( Dest, Point, ECDIGIT );						//铁电、EEPROM数据均不对，以最近一次读取的铁电数据返回，但不刷新RAM数据
				return;					
			}	
		}	
		RAM_Write( Dest, Ptr, ECDIGIT );								//返回正确的铁电数据或备份EEPROM数据
		RAM_Write( ECRAds, Dest, ECDIGIT );								//RAM中的值恢复为正确的铁电数据或备份EEPROM数据
		*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
	}		
	return;	
}

void GetRamEC( unsigned char* Dest, unsigned char* ECRAds, unsigned short ECFAds ) 
{
	
	if( RAMECIsOK( ECRAds ) == 0 )
	{
		RAM_Write( Dest, ECRAds, ECDIGIT );					//RAM中电量校验和相等，提取RAM电量。
#if( ECDisDecDigitsValid ==	YesCheck )			//13.03.03 当前电量显示小数位真实有效
		if( Disk.DigitsValidFlg == 0x55) AddECMantissa( Dest, ECRAds ); 	//显示时才添加0.01kWh以下尾数
#endif
	}
	else 
	{
		E2P_RFM( Dest, ECFAds, ECDIGIT );					//RAM中校验和错误，读取FRAM数据。	
		if( E2PECIsOK( Dest ) == 0 ) 
		{
			RAM_Write( ECRAds, Dest, ECDIGIT );				//RAM中的值恢复为FRAM中的值
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );		
		}
		else
		{
			if((Flag.Power & F_PwrUp) != 0) 				//停电状态下不作数据恢复	
			{
				ResumeEC( Dest, ECRAds, ECFAds );
			}	
		}		
	}	
}

void Read_CMonEC( unsigned char* Dest, unsigned short ECRgAds )
{
	unsigned short i;
	unsigned short ECEAds;
	unsigned short ECFAds;
	unsigned char* ECRAds;
	
///	if( CombECJust( Dest, ECRgAds ) == 0 ) return; 	//组合电量判断 
	
	for( i=0;i<ECUnitNum;i++ )
	{
		ECEAds = ECRgTab[i].ECEAds; 
		ECRAds = ECRgTab[i].ECRAds; 
		ECFAds = ECRgTab[i].ECFAds;

		if( ECEAds == ECRgAds ) break;				//总电量
		else if(( i < (ECUnitNum-1) )&&( ECEAds < ECRgAds )&&( ECRgTab[i+1].ECEAds > ECRgAds ))	//费率电量
		{
			if(((( ECRgAds - ECEAds ) % (ECDIGIT+1) ) == 0 )&&( ECRgTab[i].ECRgMode == 0 ))	//有费率电量且费率电量地址正确
			{
				ECRAds += ECRgAds - ECEAds;		//费率电量RAM地址
				ECFAds += ECRgAds - ECEAds;		//费率电量铁电地址
				break;
			}	
		}			
	}	
	if( i == ECUnitNum ) 
	{
		RAM_Fill( Dest, ECDIGIT );				//电量地址错误
		return;			
	}	
	GetRamEC( Dest, ECRAds, ECFAds );			//读取RAM或FRAM中的电量

}

//总减RAM中的分时电量
//Source: 存放电量的RAM区
//Type: 0：RAM区不包含每个电量的校验和； 1：每个电量后跟一个校验和。
void FeeECSub( unsigned char* Source, short Type )
{
	unsigned char Buff[(ECDIGIT+1)*5];
	unsigned char* Ptr;
	unsigned short Len;
	short i;
	unsigned short FeeNo;

	unsigned char Buff2[ECDIGIT+1];					//11.06.13
	unsigned char* Point;							//11.06.13	

	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > 4 )) return;				//当前费率号非法
	
	Ptr = Buff;
	Point = Buff2;									//11.06.13
	
	if( Type == 0 ) Len = ECDIGIT;
	else Len = ECDIGIT + 1;
	
	RAM_Write( Ptr, Source, (ECDIGIT+1)*5 );

	RAM_Fill( Point, Len );							//11.06.13
	for( i=1;i<5;i++ )								//11.06.13
	{												//11.06.13
		if( i == FeeNo ) continue;					//11.06.13
		ECBCDAdd( Point, Ptr+i*Len );				//11.06.13
	}												//11.06.13
	if( Data_Comp( Point, Ptr, Len ) > 0 ) return;	//11.06.13		//分时电量之和大于总电量，费率电量不修正。

	for( i=1;i<5;i++ )
	{
		if( i == FeeNo ) continue;
		ECBCDSub( Ptr, Ptr+i*Len );
	}	
	RAM_Write( Source+FeeNo*Len, Ptr, ECDIGIT );
}

//将当前脉冲电量加入铁电和RAM
void AddCurrentEC( unsigned char* ECRAds, unsigned short ECFAds, unsigned char* Source )
{
	unsigned char* TempPtr;
	unsigned short FeeNo;

	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > 4 )) return;				//当前费率号非法
	
	ECBCDAdd( ECRAds, Source );								//取RAM内总电量为基准增加电量
	*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
	TempPtr = ECRAds+FeeNo*(ECDIGIT+1);						//取RAM内当前费率电量为基准增加电量
#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
	ECBCDAdd( TempPtr, Source+6 );
#else
	ECBCDAdd( TempPtr, Source );
#endif
	*(TempPtr+ECDIGIT) = ChkNum( TempPtr, ECDIGIT );
	E2P_WFM( ECFAds, ECRAds, ECDIGIT );						//做好校验后分别写入铁电
	E2P_WFM( ECFAds+FeeNo*(ECDIGIT+1), TempPtr, ECDIGIT );								
	
}

//Source: 当前电量读出后在RAM区临时存放的首址
//AddData: 增加的电量
//Type: 0: 总出错 1：分时电量出错
short ModifyCurrentEC( unsigned char* ECRAds, unsigned short ECFAds, unsigned char* Source, unsigned char* AddData, short Type )
{
	unsigned short Addr;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned short FeeNo;
	
	FeeNo = SM.FeeNo;
	
	if(( FeeNo < 1 )||( FeeNo > 4 )) return 1;					//当前费率号非法
	TempPtr1 = Source+ECDIGIT;
	for( i=1;i<5;i++ )
	{
		if( i == FeeNo )
		{
//			TempPtr1 += ECDIGIT+1;								//当前费率电量已正确，不再重读		
			TempPtr1 += ECDIGIT;								//当前费率电量已正确，不再重读		//10.05.12
			continue;
		}	
		E2P_RFM( TempPtr1, ECFAds+i*(ECDIGIT+1), ECDIGIT );		//读出其余各费率电量
		if( E2PECIsOK( TempPtr1 ) != 0 ) break;					//判断从铁电中读出是否合法	
//		TempPtr1 += ECDIGIT+1;																					
		TempPtr1 += ECDIGIT;									//10.05.12	
	}	
	if( Type == 0 ) 
	{
		TempPtr1 = Source;
		Addr = ECFAds;
	}
	else
	{
		TempPtr1 = Source + FeeNo*ECDIGIT;
		Addr = ECFAds+FeeNo*(ECDIGIT+1);		
	}		
	E2P_RFM( TempPtr1, Addr, ECDIGIT );							//再次读取铁电中的当前总电量或分时电量
//	if( E2PECIsOK( TempPtr1 ) == 0 ) 							//电量正确
	if(( E2PECIsOK( TempPtr1 ) == 0 )||( i != 5 )) 				//电量正确或无法修复以铁电为准
	{
		RAM_Write( ECRAds, Source, ECDIGIT );
		RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Source+FeeNo*ECDIGIT, ECDIGIT );
		AddCurrentEC( ECRAds, ECFAds, AddData );
	}	
	else if( i == 5 )
	{
		if( Type == 0 ) FeeECAdd( Source, 0 );					//总等于分时电量之和
		else FeeECSub( Source, 0 );								//分时电量等于总减去其余分时电量			
		TempPtr1 = ECRAds;
		for( i=0;i<5;i++ )
		{
			RAM_Write( TempPtr1, Source+i*ECDIGIT, ECDIGIT );
			*(TempPtr1+ECDIGIT) = ChkNum( TempPtr1, ECDIGIT );								
			TempPtr1 += ECDIGIT+1; 
		}	
		AddCurrentEC( ECRAds, ECFAds, AddData );
	}
//	else return 1;	 

	return 0;
}	

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
void FeeECPulseAdd( unsigned short* PL_Cum, unsigned char* PL_Chk, unsigned char Pulse_Cum )
{
	unsigned short* PL_CumPtr;
	unsigned char* PL_ChkPtr;
	unsigned short Temp;
	
	if(( SM.FeeNo < 1 )||( SM.FeeNo > 4 )) return;				//当前费率号非法

	Temp = SM.FeeNo;
	Temp -= 1; 
	PL_CumPtr = PL_Cum;
	PL_CumPtr += Temp;
	PL_ChkPtr = PL_Chk;
	PL_ChkPtr += Temp;
//新国网		//13.08.30
//#if (( METERTYPE == U220_I20A__80A320 )||( METERTYPE == U220_I20A__100A240 )||( METERTYPE == U220_I20A__120A240 )||( METERTYPE == U220_I30A__100A240 )||( METERTYPE == U220_I10A__100A240 ))
//	*PL_CumPtr += (unsigned short)Pulse_Cum * 10;	
//#else
	*PL_CumPtr += Pulse_Cum;	
//#endif
	*PL_ChkPtr = ChkNum( (unsigned char*)PL_CumPtr, 2 );
}
#endif
#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
short ECRgSum( short ECItem, unsigned short Data, unsigned short FData )
{
	unsigned short ECFAds;
	unsigned char* ECRAds;
	unsigned char Buff[30];
//	unsigned char Buff2[6];
	unsigned char Buff2[12];
	unsigned char* Ptr;
	unsigned char* Source;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned char FStateZ=0;
	unsigned char FStateF=0;
	unsigned char RState=0;
	unsigned short FeeNo;
	
	Ptr = Buff;
	Source = Buff2;
	
	RAM_Fill( Ptr, 30 );
//	RAM_Fill( Source, 6 );
	RAM_Fill( Source, 12 );
	Word_BCD( Source+1, Data );											//两位小数有效？
	Word_BCD( Source+7, FData );											//两位小数有效？
	ECRAds = ECRgTab[ECItem].ECRAds;
	ECFAds = ECRgTab[ECItem].ECFAds;
	FeeNo = SM.FeeNo;

	if(( FeeNo < 1 )||( FeeNo > 4 )) return 1;							//当前费率号非法
	
	if( ECRgTab[ECItem].ECRgMode == 0 )									//带费率电量
	{	
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    	//总电量是否合法	
		TempPtr1 = Ptr+FeeNo*ECDIGIT;
		E2P_RFM( TempPtr1, ECFAds+FeeNo*(ECDIGIT+1), ECDIGIT );
		if( E2PECIsOK( TempPtr1 ) != 0 ) FStateF = 1;                   //分时电量是否合法 		
		
		if(( FStateZ == 0 )&&( FStateF == 0 ))							//总和当前费率电量读铁电均正确
		{
			RAM_Write( ECRAds, Ptr, ECDIGIT );
			RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
			AddCurrentEC( ECRAds, ECFAds, Source );
		}	
		else															//读总电量和分时电量至少1个不成功
		{
			if( TotalFeeECCheck( ECRAds, 1 ) == 0 )						//判断RAM区的电量总是否等于分时电量之和
			{
				for( i=0;i<5;i++ )
				{
					if( RAMECIsOK( ECRAds+i*(ECDIGIT+1) ) != 0 )		//RAM区的各电量自检是否正确
					{
						RState = 1;										//有电量非法，校验不对或为非BCD码	
						break;	
					}	
				}	
			}	
			else RState = 1;											//RAM区电量有误，总与分时之和不等
			if( RState == 0 )											//RAM内电量数据均合法。
			{				
				AddCurrentEC( ECRAds, ECFAds, Source );							
			}
			else														//RAM内电量有误	
			{
				if(( FStateZ == 1 )&&( FStateF == 0 ))					//总错误、当前费率电量读铁电正确
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 0 );
				}
				else if(( FStateZ == 0 )&&( FStateF == 1 ))				//总正确和当前费率电量读铁电错误
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 1 );
				}
//				else return 1;		
				else 
				{
					ResumeEC( Ptr, ECRAds, ECFAds );					//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08
					ResumeEC( Ptr+FeeNo*ECDIGIT, ECRAds+FeeNo*(ECDIGIT+1), ECFAds+FeeNo*(ECDIGIT+1) );		//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08
					RAM_Write( ECRAds, Ptr, ECDIGIT );										//全错以铁电为准
					RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
					AddCurrentEC( ECRAds, ECFAds, Source );							
//					return 1;		
				}	
			}		
		}	
	}
	else
	{
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    		
		if( RAMECIsOK( ECRAds ) != 0 ) RState = 1;

		if(( FStateZ == 1 )&&( RState == 1 )) ResumeEC( Ptr, ECRAds, ECFAds );				//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08

//		if(( FStateZ == 0 )||( RState == 0 ))
//		{
//			if( FStateZ == 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//以铁电数据为准
			if( RState != 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//只要RAM不对均以铁电数据为准
			ECBCDAdd( ECRAds, Source );
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
			E2P_WFM( ECFAds, ECRAds, ECDIGIT );						
//		}	 		
//		else return 1;		
	}	
	return 0;
}	
#else
short ECRgSum( short ECItem, unsigned short Data )
{
	unsigned short ECFAds;
	unsigned char* ECRAds;
	unsigned char Buff[30];
	unsigned char Buff2[6];
	unsigned char* Ptr;
	unsigned char* Source;
	unsigned char* TempPtr1;
	unsigned short i;
	unsigned char FStateZ=0;
	unsigned char FStateF=0;
	unsigned char RState=0;
	unsigned short FeeNo;
	
	Ptr = Buff;
	Source = Buff2;
	
	RAM_Fill( Ptr, 30 );
	RAM_Fill( Source, 6 );
	Word_BCD( Source+1, Data );											//两位小数有效？
	ECRAds = ECRgTab[ECItem].ECRAds;
	ECFAds = ECRgTab[ECItem].ECFAds;
	FeeNo = SM.FeeNo;

	if(( FeeNo < 1 )||( FeeNo > 4 )) return 1;							//当前费率号非法
	
	if( ECRgTab[ECItem].ECRgMode == 0 )									//带费率电量
	{	
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    	//总电量是否合法	
		TempPtr1 = Ptr+FeeNo*ECDIGIT;
		E2P_RFM( TempPtr1, ECFAds+FeeNo*(ECDIGIT+1), ECDIGIT );
		if( E2PECIsOK( TempPtr1 ) != 0 ) FStateF = 1;                   //分时电量是否合法 		
		
		if(( FStateZ == 0 )&&( FStateF == 0 ))							//总和当前费率电量读铁电均正确
		{
			RAM_Write( ECRAds, Ptr, ECDIGIT );
			RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
			AddCurrentEC( ECRAds, ECFAds, Source );
		}	
		else															//读总电量和分时电量至少1个不成功
		{
			if( TotalFeeECCheck( ECRAds, 1 ) == 0 )						//判断RAM区的电量总是否等于分时电量之和
			{
				for( i=0;i<5;i++ )
				{
					if( RAMECIsOK( ECRAds+i*(ECDIGIT+1) ) != 0 )		//RAM区的各电量自检是否正确
					{
						RState = 1;										//有电量非法，校验不对或为非BCD码	
						break;	
					}	
				}	
			}	
			else RState = 1;											//RAM区电量有误，总与分时之和不等
			if( RState == 0 )											//RAM内电量数据均合法。
			{				
				AddCurrentEC( ECRAds, ECFAds, Source );							
			}
			else														//RAM内电量有误	
			{
				if(( FStateZ == 1 )&&( FStateF == 0 ))					//总错误、当前费率电量读铁电正确
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 0 );
				}
				else if(( FStateZ == 0 )&&( FStateF == 1 ))				//总正确和当前费率电量读铁电错误
				{
					return ModifyCurrentEC( ECRAds, ECFAds, Ptr, Source, 1 );
				}
//				else return 1;		
				else 
				{
					ResumeEC( Ptr, ECRAds, ECFAds );					//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08
					ResumeEC( Ptr+FeeNo*ECDIGIT, ECRAds+FeeNo*(ECDIGIT+1), ECFAds+FeeNo*(ECDIGIT+1) );		//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08
					RAM_Write( ECRAds, Ptr, ECDIGIT );										//全错以铁电为准
					RAM_Write( ECRAds+FeeNo*(ECDIGIT+1), Ptr+FeeNo*ECDIGIT, ECDIGIT );
					AddCurrentEC( ECRAds, ECFAds, Source );							
//					return 1;		
				}	
			}		
		}	
	}
	else
	{
		E2P_RFM( Ptr, ECFAds, ECDIGIT );
		if( E2PECIsOK( Ptr ) != 0 ) FStateZ = 1;                    		
		if( RAMECIsOK( ECRAds ) != 0 ) RState = 1;

		if(( FStateZ == 1 )&&( RState == 1 )) ResumeEC( Ptr, ECRAds, ECFAds );				//若RAM和铁电均有错，则用EEPROM内电量恢复	//10.12.08

//		if(( FStateZ == 0 )||( RState == 0 ))
//		{
//			if( FStateZ == 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//以铁电数据为准
			if( RState != 0 ) RAM_Write( ECRAds, Ptr, ECDIGIT );		//只要RAM不对均以铁电数据为准
			ECBCDAdd( ECRAds, Source );
			*(ECRAds+ECDIGIT) = ChkNum( ECRAds, ECDIGIT );
			E2P_WFM( ECFAds, ECRAds, ECDIGIT );						
//		}	 		
//		else return 1;		
	}	
	return 0;
}	
#endif

void EC_Meas(void)
{
	unsigned char YPulse_Cum=0;		
	unsigned char WPulse_Cum=0;	
	unsigned char YPulse_CumChk;		
	unsigned char WPulse_CumChk;
	unsigned char Temp2;	
	unsigned short Temp;	
	short	Meter_Quad,i;
	unsigned short PS32;					//新国网		//13.08.30	
#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
	unsigned short* ECRamBufAdsPtr;
	unsigned char* ECRamBufChkAdsPtr;
	unsigned short* FeeECRamBufAdsPtr;
	unsigned char* FeeECRamBufChkAdsPtr;
	unsigned short Temp3;
#endif
#if ( MEASCHIP == IDT90E32 )
	unsigned short TempInt;	
#endif

//	WDTCTL = WDT_ARST_1000;
	HT_FreeDog();
	//先按1000脉冲常数算
	PS32 = (Measur_Const/100);//MSpec.RMeterConst / 10;						//新国网		//13.08.30	

	if(( ECInt.YPulse_Cum != 0 )||( ECInt.WPulse_Cum != 0 ))		
	{														
		__disable_irq();//_DINT();
		YPulse_Cum = ECInt.YPulse_Cum;
		WPulse_Cum = ECInt.WPulse_Cum;
		YPulse_CumChk = ECInt.YPulse_CumChk;
		WPulse_CumChk = ECInt.WPulse_CumChk;
		ECInt.YPulse_Cum = 0;
		ECInt.WPulse_Cum = 0;
		ECInt.YPulse_CumChk = 0xA5;
		ECInt.WPulse_CumChk = 0xA5;
		__enable_irq();//_EINT();
		Temp2 = YPulse_Cum + 0xA5;
		if( YPulse_CumChk != Temp2 ) YPulse_Cum = 0;						
		Temp2 = WPulse_Cum + 0xA5;
		if( WPulse_CumChk != Temp2 ) WPulse_Cum = 0;												

#if ( FRAMPulseValidCheck == YesCheck )			//16.01.26
		if(( YPulse_Cum != 0 )||( WPulse_Cum != 0 ))
		{
			if( SM.PulseFlag == 0 )		//已产生脉冲标记		//16.01.26
			{
				E2P_WFM( FPulseValidFlag, (unsigned char*)&SM.PulseFlag, 1 );	 	//下电后保存RAM内的不到0.01Kwh的脉冲个数有效标志				
			}
			SM.PulseFlag = 1;	
		}	
#endif		
	}															
	
	Meter_Quad = GetMQuad( Real.PWFlag,3 );		//取当前象限		
	if( YPulse_Cum !=0 )
	{
		switch (Meter_Quad )
		{
			case 2:
			case 3:	
					ECP.PL_CumPn +=  YPulse_Cum;
					ECP.PL_ChkPn = ChkNum( (unsigned char*)&ECP.PL_CumPn, 2 );
					MDBufPn.MD_Cum += YPulse_Cum;	//有功反向需量
					ECP.PL_CumPn_Dis += YPulse_Cum;

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
					FeeECPulseAdd( &FeeECP.PL_CumPn1, &FeeECP.PL_ChkPn1, YPulse_Cum );
#endif
					break;		//正反向合计？ 	
			case 1:
			case 4: 
					ECP.PL_CumPp +=  YPulse_Cum; 
					ECP.PL_ChkPp = ChkNum( (unsigned char*)&ECP.PL_CumPp, 2 );
					MDBufPp.MD_Cum += YPulse_Cum;	//有功正向需量
					ECP.PL_CumPp_Dis += YPulse_Cum;

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
					FeeECPulseAdd( &FeeECP.PL_CumPp1, &FeeECP.PL_ChkPp1, YPulse_Cum );
#endif
					break;
			default: break;
		}
	}	
	if( WPulse_Cum !=0 )
	{
		switch (Meter_Quad )
		{
			case 1:
				ECP.PL_CumQ1 += WPulse_Cum; ECP.PL_ChkQ1 = ChkNum( (unsigned char*)&ECP.PL_CumQ1, 2 );
				ECP.PL_CumQp += WPulse_Cum; ECP.PL_ChkQp = ChkNum( (unsigned char*)&ECP.PL_CumQp, 2 );
				break; 	
			case 2:
				ECP.PL_CumQ2 += WPulse_Cum; ECP.PL_ChkQ2 = ChkNum( (unsigned char*)&ECP.PL_CumQ2, 2 );
				ECP.PL_CumQp += WPulse_Cum; ECP.PL_ChkQp = ChkNum( (unsigned char*)&ECP.PL_CumQp, 2 );
				break; 	
			case 3:
				ECP.PL_CumQ3 += WPulse_Cum; ECP.PL_ChkQ3 = ChkNum( (unsigned char*)&ECP.PL_CumQ3, 2 );
				ECP.PL_CumQn+= WPulse_Cum; ECP.PL_ChkQn= ChkNum( (unsigned char*)&ECP.PL_CumQn, 2 );
				break; 	
			case 4:
				ECP.PL_CumQ4 += WPulse_Cum; ECP.PL_ChkQ4 = ChkNum( (unsigned char*)&ECP.PL_CumQ4, 2 );
				ECP.PL_CumQn+= WPulse_Cum; ECP.PL_ChkQn= ChkNum( (unsigned char*)&ECP.PL_CumQn, 2 );
				break;		
			default: break;
		}

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
		switch (Meter_Quad )
		{
			case 1:	FeeECPulseAdd( &FeeECP.PL_CumQ11, &FeeECP.PL_ChkQ11, WPulse_Cum ); break;
            case 2: FeeECPulseAdd( &FeeECP.PL_CumQ21, &FeeECP.PL_ChkQ21, WPulse_Cum ); break;
            case 3: FeeECPulseAdd( &FeeECP.PL_CumQ31, &FeeECP.PL_ChkQ31, WPulse_Cum ); break;
            case 4: FeeECPulseAdd( &FeeECP.PL_CumQ41, &FeeECP.PL_ChkQ41, WPulse_Cum ); break;
			default: break;
		}   
#endif      

	}	

#if ( FeeECIndependence	== YesCheck )		//11.01.15	费率电量独立计量
	for( i=0;i<ECUnitNum;i++ )						//大于0.01度写铁电存储器
	{
		ECRamBufAdsPtr = ECRgTab[i].ECRamBufAds;
		ECRamBufChkAdsPtr = ECRgTab[i].ECRamBufChkAds;
		if( *ECRamBufChkAdsPtr != ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 ) )
		{
			*ECRamBufAdsPtr = 0;
			*ECRamBufChkAdsPtr = 0;
		}	

		Temp = 0;
#if ( MEASCHIP == IDT90E32 )
		if( i < 8 ) TempInt = PS32;				
		else TempInt = PS32*10;						//分相有无功电量和视在电量脉冲是从IDT90E32中读出，单位为0.1CF	
		if( *ECRamBufAdsPtr >= TempInt )
		{
			Temp = *ECRamBufAdsPtr / TempInt;
			*ECRamBufAdsPtr -= Temp * TempInt;
			*ECRamBufChkAdsPtr = ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 );
		}
#else
		if( *ECRamBufAdsPtr >= PS32 )
		{
			Temp = *ECRamBufAdsPtr / PS32;
			*ECRamBufAdsPtr -= Temp * PS32;
			*ECRamBufChkAdsPtr = ChkNum( (unsigned char*)ECRamBufAdsPtr, 2 );
		}
#endif
		if( i < 8 )
		{
			Temp3 = 0;
			FeeECRamBufAdsPtr = FeeECRgTab[i].ECRamBufAds;
			FeeECRamBufAdsPtr += (SM.FeeNo-1);
			FeeECRamBufChkAdsPtr = FeeECRgTab[i].ECRamBufChkAds;
			FeeECRamBufChkAdsPtr += (SM.FeeNo-1);
			if( *FeeECRamBufChkAdsPtr != ChkNum( (unsigned char*)FeeECRamBufAdsPtr, 2 ) )
			{
				*FeeECRamBufAdsPtr = 0;
				*FeeECRamBufChkAdsPtr = 0;
			}	
			if( *FeeECRamBufAdsPtr >= PS32 )
			{
				Temp3 = *FeeECRamBufAdsPtr / PS32;
				*FeeECRamBufAdsPtr -= Temp3 * PS32;
				*FeeECRamBufChkAdsPtr = ChkNum( (unsigned char*)FeeECRamBufAdsPtr, 2 );
			}
		}
		else Temp3 = Temp;
		if(( Temp > 100 )||( Temp3 > 100 )) continue;				//数据超差，不计入电量。
		if(( Temp == 0 )&&( Temp3 == 0 )) continue;					//无脉冲，返回。		//11.03.01
		ECRgSum( i, Temp, Temp3 );
/*
#if( LocalSmartMeter ==	YesCheck )									//17.04.22
		if((( i == 0 )||(( i == 1 )&&(( Para.RActiveMode & 0x0C ) == 0x04 )))&&( Temp != 0 )) IncConsumECSum( Temp, 0 );	//增加正向有功阶梯用电金额							//15.07.31	
		if((( i == 0 )||(( i == 1 )&&(( Para.RActiveMode & 0x0C ) == 0x04 )))&&( Temp3 != 0 )) IncConsumECSum( Temp3, 1 );	//增加正向有功费率用电金额	//新国网	//13.11.30	//15.07.31	
#endif

#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#else																		//新国网	//13.11.06
#if ( MagnKeepRelay == NoCheck )											//10.08.07
		if((( i == 0 )||(( i == 1 )&&(( Para.RActiveMode & 0x0C ) == 0x04 )))&&( Temp != 0 ))							//继电器外置表在跳闸后总电量每走0.01kWh就再拉闸一次。	
		{
			if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayDrivePulseWidth == 0 )) 	//已拉闸且不是正在拉闸	//10.08.07
			{
				RelayDrive( RELAY_OFF );						
			}					
		}	
#endif
#endif					//新国网	//13.11.06
*/
	}

#else
	for( i=0;i<ECUnitNum;i++ )						//大于0.01度写铁电存储器
	{
		if( *ECRgTab[i].ECRamBufChkAds != ChkNum( (unsigned char*)ECRgTab[i].ECRamBufAds, 2 ) )
		{
			*ECRgTab[i].ECRamBufAds = 0;
			*ECRgTab[i].ECRamBufChkAds = 0;
		}	
		
#if ( MEASCHIP == IDT90E32 )
		if( i < 6 ) TempInt = PS32;				
		else TempInt = PS32*10;						//分相有无功电量和视在电量脉冲是从IDT90E32中读出，单位为0.1CF	
		if( *ECRgTab[i].ECRamBufAds >= TempInt )
		{
			Temp = *ECRgTab[i].ECRamBufAds / TempInt;
			*ECRgTab[i].ECRamBufAds -= Temp * TempInt;
#else
		if( *ECRgTab[i].ECRamBufAds >= PS32 )
		{
			Temp = *ECRgTab[i].ECRamBufAds / PS32;
			*ECRgTab[i].ECRamBufAds -= Temp * PS32;
#endif
			*ECRgTab[i].ECRamBufChkAds = ChkNum( (unsigned char*)ECRgTab[i].ECRamBufAds, 2 );
			if( Temp > 100 ) continue;				//数据超差，不计入电量。
			ECRgSum( i, Temp);

//			if( Temp > 100 ) 
//			{
//				*ECRgTab[i].ECRamBufAds -= Temp * PS32;
//				*ECRgTab[i].ECRamBufChkAds = ChkNum( (unsigned char*)ECRgTab[i].ECRamBufAds, 2 );			
//				continue;				//数据超差，不计入电量。				
//			}	
//			if( ECRgSum( i, Temp) == 0 )
//			{
//				*ECRgTab[i].ECRamBufAds -= Temp * PS32;
//				*ECRgTab[i].ECRamBufChkAds = ChkNum( (unsigned char*)ECRgTab[i].ECRamBufAds, 2 );			
//			}	
//			else continue;	

/*
#if( LocalSmartMeter ==	YesCheck )									//17.04.22
			if(( i == 0 )||(( i == 1 )&&(( Para.RActiveMode & 0x0C ) == 0x04 ))) IncConsumECSum( Temp );	//增加正向有功用电金额
#endif

#if ( GeneralSmartMeter	== YesCheck )						//普通智能表	//新国网	//13.11.06
#else																		//新国网	//13.11.06
//#if (( JiangSuRequire == YesCheck )&&( MagnKeepRelay == NoCheck))			//10.08.07
#if ( MagnKeepRelay == NoCheck )											//10.08.07
			if( i == 0 )							//继电器外置表在跳闸后每走0.01kWh就再拉闸一次。	
			{
//				if( Para.RMeterRelayState == 0x1A ) //已拉闸
				if(( Para.RMeterRelayState == 0x1A )&&( ICComm.RelayDrivePulseWidth == 0 )) 	//已拉闸且不是正在拉闸	//10.08.07
				{
					RelayDrive( RELAY_OFF );						
//					if( Un55VoltJudge() == 0 ) RelayDrive( RELAY_OFF );					//10.01.18					
				}					
			}	
#endif
#endif		
*/
		}						
	}		
#endif		
}	
#endif

