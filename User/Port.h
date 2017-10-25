/************************************************************
	Port Define
************************************************************/
/*
// Port 1

#define 	P_CARDIN	0x01
//#define 	P_AUXCHK	0x02
#define 	P_ESAMIO	0x08
//#define 	P_CARDIO	0x10
#define 	P_PSAMIO	0x10
#define 	P_CARDRST	0x20
#define 	P_ESAMCLK	0x40
#define 	P_ESAMRST	0x80

#define 	PDIR_CARDIN	P1DIR
//#define 	PDIR_AUXCHK	P1DIR
#define 	PDIR_ESAMIO	P1DIR
//#define 	PDIR_CARDIO	P1DIR
#define 	PDIR_PSAMIO	P1DIR
#define 	PDIR_CARDRST	P1DIR
#define 	PDIR_ESAMCLK	P1DIR
#define 	PDIR_ESAMRST	P1DIR

#define 	PIN_CARDIN	P1IN
//#define 	PIN_AUXCHK	P1IN
#define 	PIN_ESAMIO	P1IN
//#define 	PIN_CARDIO	P1IN
#define 	PIN_PSAMIO	P1IN
#define 	PIN_CARDRST	P1IN
#define 	PIN_ESAMCLK	P1IN
#define 	PIN_ESAMRST	P1IN

#define 	POUT_ESAMIO	P1OUT
#define 	POUT_PSAMIO	P1OUT
#define 	POUT_ESAMCLK	P1OUT

#define 	PSEL_ESAMIO	P1SEL
#define 	PSEL_PSAMIO	P1SEL
#define 	PSEL_ESAMCLK	P1SEL

#define		P_TXDHW		0x02
#define		P_RXDHW		0x04
#define		PDIR_TXDHW	P1DIR
#define		PDIR_RXDHW	P1DIR
#define		PIN_TXDHW	P1IN
#define		PIN_RXDHW	P1IN
#define		PIFG_RXDHW	P1IFG
#define		PIE_RXDHW	P1IE

#define 	PSEL_HW		P1SEL

#define 	P_CARDINPUT	0x02
#define 	P_CARDOUT	0x04

#define		POUT_TXDHW	P1OUT

// Port 2

#if ( InfraredCarry38k == YesCheck )		//15.01.22
#define		P_HW38K		0x08
#define		PDIR_HW38K	P2DIR
#define		PSEL_HW38K	P2SEL
#define		POUT_HW38K	P2OUT
#endif

#define		P_CARDPW	0x01
#define		P_SDA16		0x02
//#define		P_CLRWD		0x04
#define		P_WP		0x04
#define		P_SECINT	0x08
#define		P_SECOUT	0x10
#define		P_LREV		0x20
#define		P_YG		0x40
#define		P_WG		0x80

#define		PDIR_CARDPW	P2DIR
#define		PDIR_SDA16	P2DIR
#define		PDIR_CLRWD	P2DIR
#define		PDIR_SECINT	P2DIR
#define		PDIR_SECOUT	P2DIR
#define		PDIR_LREV	P2DIR
#define		PDIR_YG		P2DIR
#define		PDIR_WG		P2DIR

#define		PIN_CARDPW	P2IN
#define		PIN_SDA16	P2IN
#define		PIN_CLRWD	P2IN
#define		PIN_SECINT	P2IN
#define		PIN_SECOUT	P2IN
#define		PIN_LREV	P2IN
#define		PIN_YG		P2IN
#define		PIN_WG		P2IN

#define		POUT_LREV	P2OUT

// Port 3

#define		P_CTLQSY	0x01
//#define		P_BJOUT		0x02
#define		P_ESAMPW	0x02			//ESAM电源控制 10.12.01
#define		P_LEDBJ2	0x04
//#define		P_LEDBJ1	0x08
#define		P_LEDTZ		0x08
//#define		P_TXDHW		0x10
//#define		P_RXDHW		0x20
#define		P_TXDZB		0x10
#define		P_RXDZB		0x20
#define		P_SCL		0x40
#define		P_SDA		0x80

#define		PDIR_CTLQSY	P3DIR
#define		PDIR_BJOUT	P3DIR
#define		PDIR_LEDBJ2	P3DIR
//#define		PDIR_LEDBJ1	P3DIR
#define		PDIR_LEDTZ	P3DIR
//#define		PDIR_TXDHW	P3DIR
//#define		PDIR_RXDHW	P3DIR
#define		PDIR_TXDZB	P3DIR
#define		PDIR_RXDZB	P3DIR
#define		PDIR_SCL	P3DIR
#define		PDIR_SDA	P3DIR

#define		PDIR_ESAMPW	P3DIR			//ESAM电源控制 10.12.01

#define		PIN_CTLQSY	P3IN
#define		PIN_BJOUT	P3IN
#define		PIN_LEDBJ2	P3IN
//#define		PIN_LEDBJ1	P3IN
#define		PIN_LEDTZ	P3IN
//#define		PIN_TXDHW	P3IN
//#define		PIN_RXDHW	P3IN
#define		PIN_TXDZB	P3IN
#define		PIN_RXDZB	P3IN
#define		PIN_SCL		P3IN
#define		PIN_SDA		P3IN

//#define		POUT_LEDBJ1	P3OUT
#define		POUT_LEDTZ	P3OUT
#define		POUT_CTLQSY P3OUT		//10.11.11

#define		POUT_ESAMPW	P3OUT			//ESAM电源控制 10.12.01

#define		POUT_LEDBJ2	P3OUT

//#define 	PSEL_HW		P3SEL
// Port 4
//#define		P_HRPWM		0x01
#define		P_EVENTOUT	0x01					//新国网
#define		P_FMQBJ		0x02
//#define		P_YGOUT_	0x04				//10.12.07	
#define		P_CTLYWG	0x04					//10.12.07
//#define		P_WGOUT_	0x08
#define		P_RTCSEL	0x08					//新国网
#define		P_YGOUT		0x10
//#define		P_WGOUT		0x20
#define		P_STAZB		0x20
#define		P_RTCSCL	0x40
#define		P_RTCSDA	0x80

//#define		PDIR_HRPWM	P4DIR
#define		PDIR_EVENTOUT	P4DIR
#define		PDIR_FMQBJ	P4DIR
#define		PDIR_YGOUT_	P4DIR
//#define		PDIR_WGOUT_	P4DIR
#define		PDIR_RTCSEL	P4DIR					//新国网
#define		PDIR_YGOUT	P4DIR
//#define		PDIR_WGOUT	P4DIR
#define		PDIR_STAZB	P4DIR
#define		PDIR_RTCSCL	P4DIR
#define		PDIR_RTCSDA	P4DIR

#define		PDIR_CTLYWG	P4DIR					//10.12.07	

//#define		PIN_HRPWM	P4IN
#define		PIN_EVENTOUT	P4IN
#define		PIN_FMQBJ	P4IN
#define		PIN_YGOUT_	P4IN
//#define		PIN_WGOUT_	P4IN
#define		PIN_RTCSEL	P4IN					//新国网
#define		PIN_YGOUT	P4IN
//#define		PIN_WGOUT	P4IN
#define		PIN_STAZB	P4IN
#define		PIN_RTCSCL	P4IN
#define		PIN_RTCSDA	P4IN

#define		POUT_FMQBJ	P4OUT
#define		POUT_EVENTOUT	P4OUT

#define		POUT_CTLYWG	P4OUT					//10.12.07

// Port 5

//#define			0x01
//#define			0x02
//#define			0x04
//#define		 	0x08
#define		P_BGLED	 	0x10
#define		P_SW4851	0x20
#define		P_TXD4851	0x40
#define		P_RXD4851	0x80

#define		PDIR_BGLED	P5DIR
#define		PDIR_SW4851	P5DIR
#define		PDIR_TXD4851	P5DIR
#define		PDIR_RXD4851	P5DIR

#define		PIN_BGLED	P5IN
#define		PIN_SW4851	P5IN
#define		PIN_TXD4851	P5IN
#define		PIN_RXD4851	P5IN

#define		POUT_BGLED	P5OUT

// Port 6

#define		P_IAAD		0x01	//A相电流 	 
#define		P_IBAD		0x02	//B相电流 	 
#define		P_ICAD		0x04	//C相电流 	 
//#define		P_VASY		0x08	//A相电压 	 
//#define		P_VBSY		0x10	//B相电压 	 
//#define		P_VCSY		0x20	//C相电压 	 
#define		P_BATCH		0x40	//V2 
#define		P_BATCHB	0x80	//V2

#define		P_MAGNET	0x20	//磁场检测			//新国网

#define		PDIR_IAAD	P6DIR	//A相电流 	 
#define		PDIR_IBAD	P6DIR	//B相电流 	 
#define		PDIR_ICAD	P6DIR	//C相电流 	 
//#define		PDIR_VASY	P6DIR	//A相电压 	 
//#define		PDIR_VBSY	P6DIR	//B相电压 	 
//#define		PDIR_VCSY	P6DIR	//C相电压 	 
#define		PDIR_MAGNET	P6DIR	//磁场检测			//新国网
#define		PDIR_BATCH	P6DIR	//V2 
#define		PDIR_BATCHB	P6DIR	//V2

#define		PIN_IAAD		P6IN	//A相电流 	 
#define		PIN_IBAD		P6IN	//B相电流 	 
#define		PIN_ICAD		P6IN	//C相电流 	 
//#define		PIN_VASY		P6IN	//A相电压 	 
//#define		PIN_VBSY		P6IN	//B相电压 	 
//#define		PIN_VCSY		P6IN	//C相电压 	 
#define		PIN_MAGNET	P6IN	//磁场检测			//新国网
#define		PIN_BATCH	P6IN	//V2 
#define		PIN_BATCHB	P6IN	//V2



//#define		P_PULSSW	0x02
//#define		P_GT609		0x04	//不用放大器
//#define		P_LOWPOW	0x40
//#define		P_BATCHB	0x80
//#define		P_LEDBJ		0x80	//报警灯和停抄电池复用


// Port 7
//#define		P_KEYBC		0x04	
//#define		P_KEYG		0x04	
//#define		P_FlsCS2	0x08		//新国网	//13.10.16
#define		P_RSTZB		0x08			//载波复位脚	//11.05.27
#define		P_BATCTL	0x10	
//#define		P_RELAY1	0x20	
#define		P_RELBJ	0x20	
//#define		P_RELAY2	0x40	
#define		P_DISCTL	0x80	

#define		PDIR_RSTZB	P7DIR			//载波复位脚	//11.05.27

//#define		PDIR_KEYBC	P7DIR	
//#define		PDIR_KEYG	P7DIR	
#define		PDIR_FlsCS2	P7DIR	
#define		PDIR_BATCTL	P7DIR	
//#define		PDIR_RELAY1	P7DIR	
#define		PDIR_RELBJ	P7DIR	
//#define		PDIR_RELAY2	P7DIR	
#define		PDIR_DISCTL	P7DIR	

//#define		PIN_KEYBC	P7IN	
//#define		PIN_KEYG	P7IN	
#define		PIN_FlsCS2	P7IN	
#define		PIN_BATCTL	P7IN	
//#define		PIN_RELAY1	P7IN	
#define		PIN_RELBJ	P7IN	
//#define		PIN_RELAY2	P7IN	
#define		PIN_DISCTL	P7IN	

#define		POUT_BATCTL	P7OUT	
#define		POUT_RSTZB	P7OUT				//新国网	//13.10.16

#define		POUT_RELBJ	P7OUT	


#define		P_JDQA		0x40				//费控表磁保持继电器A口
#define		PDIR_JDQA	P7DIR
#define		POUT_JDQA	P7OUT

#define		P_JDQCHC	0x04				//继电器检测C

#define		PDIR_JDQCHC	P10DIR	
#define		POUT_JDQCHC	P10OUT	
#define		PIN_JDQCHC	P10IN	

//#define		PDS_JDQA		P7DS				//费控表磁保持继电器A口

// Port 8
#define		P_EECTL		0x01
#define		P_FlsCS		0x02
#define		P_FlsSDO	0x04
#define		P_FlsSCK	0x08
#define		P_FlsSDI	0x10
#define		P_SETEN 	0x20	
#define		P_KEYXF		0x40	
#define		P_KEYSF		0x80	

#define		PDIR_EECTL	P8DIR
#define		PDIR_FlsCS	P8DIR
#define		PDIR_FlsSDO	P8DIR
#define		PDIR_FlsSCK	P8DIR
#define		PDIR_FlsSDI	P8DIR
#define		PDIR_SETEN 	P8DIR
#define		PDIR_KEYXF	P8DIR
#define		PDIR_KEYSF	P8DIR

#define		PIN_EECTL	P8IN
#define		PIN_FlsCS	P8IN
#define		PIN_FlsSDO	P8IN
#define		PIN_FlsSCK	P8IN
#define		PIN_FlsSDI	P8IN
#define		PIN_SETEN 	P8IN
#define		PIN_KEYXF	P8IN
#define		PIN_KEYSF	P8IN

// Port 9
#define		P_PRGRM		0x01	
#define		P_KEYG		0x02	
#define		P_KEYD		0x04	
#define		P_SW4852	0x08	
#define		P_TXD4852	0x10	
#define		P_RXD4852	0x20	
#define		P_LOWPOW	0x40	
#define		P_ADCS		0x80	

#define		PDIR_PRGRM	P9DIR	
#define		PDIR_KEYG	P9DIR	
#define		PDIR_KEYD	P9DIR	
#define		PDIR_SW4852	P9DIR	
#define		PDIR_TXD4852	P9DIR	
#define		PDIR_RXD4852	P9DIR	
#define		PDIR_LOWPOW	P9DIR	
#define		PDIR_ADCS	P9DIR	

#define		PIN_PRGRM	P9IN	
#define		PIN_KEYG	P9IN	
#define		PIN_KEYD	P9IN	
#define		PIN_SW4852	P9IN	
#define		PIN_TXD4852	P9IN	
#define		PIN_RXD4852	P9IN	
#define		PIN_LOWPOW	P9IN	
#define		PIN_ADCS	P9IN	

#define		POUT_ADCS	P9OUT	


// Port 10

#define		P_ADREST	0x01	
#define		P_ADDIN		0x02	
#define		P_MDOUT		0x04	
#define		P_ADSCLK	0x08	
//#define		P_KBPWR		0x10				//仅作为清需量标志用，与端口无关        //新国网

//#define		P_TXD4853	0x10	
//#define		P_RXD4853	0x20	
//#define		P_SW4853	0x40	
#define		P_SWZB		0x40	
//#define		P_LCDSCL	0x80			//新国网
//#define		P_LCDDI		0x80			//新国网
#define		P_JDQCHK	0x80				//继电器检测	//新国网

#define		P_JDQCHA	0x20				//继电器检测A
#define		P_JDQCHB	0x10				//继电器检测A

#define		PDIR_JDQCHK	P10DIR				//新国网
	
#define		PDIR_JDQCHA	P10DIR	
#define		PDIR_JDQCHB	P10DIR	

#define		POUT_JDQCHK	P10OUT				//新国网
#define		POUT_JDQCHA	P10OUT	
#define		POUT_JDQCHB	P10OUT	

#define		PIN_JDQCHK	P10IN				//新国网

#define		PIN_JDQCHA	P10IN	
#define		PIN_JDQCHB	P10IN	

#define		PDIR_ADREST	P10DIR	
#define		PDIR_ADDIN	P10DIR
#define		PDIR_MDOUT	P10DIR
#define		PDIR_ADSCLK	P10DIR
//#define		PDIR_KBPWR	P10DIR
//#define		PDIR_TXD4853	P10DIR
//#define		PDIR_RXD4853	P10DIR
//#define		PDIR_SW4853	P10DIR
#define		PDIR_SWZB	P10DIR
#define		PDIR_LCDSCL	P10DIR
#define		PDIR_LCDDI	P10DIR


#define		PIN_ADREST	P10IN	
#define		PIN_ADDIN	P10IN
#define		PIN_MDOUT	P10IN
#define		PIN_ADSCLK	P10IN
//#define		PIN_KBPWR	P10IN
//#define		PIN_TXD4853	P10IN
//#define		PIN_RXD4853	P10IN
//#define		PIN_SW4853	P10IN
#define		PIN_SWZB	P10IN
#define		PIN_LCDSCL	P10IN
#define		PIN_LCDDI	P10IN

#define		POUT_ADREST	P10OUT	
#define		POUT_ADDIN	P10OUT
#define		POUT_MDOUT	P10OUT
#define		POUT_ADSCLK	P10OUT
//#define		POUT_KBPWR	P10OUT
//#define		POUT_SW4853	P10OUT
#define		POUT_SWZB	P10OUT
#define		POUT_LCDSCL	P10OUT
#define		POUT_LCDDI	P10OUT

#define 	P_AUXCHK	0x20
#define 	PDIR_AUXCHK	P10DIR
#define 	PIN_AUXCHK	P10IN


// Port 11

#define		P_LCDSDA	0x01
#define		P_LCDCLK	0x02
//#define		P_XLZQOUT	0x04

#define		P_LCDSTR	0x01

#define		PDIR_LCDSDA	P11DIR
#define		PDIR_LCDCLK	P11DIR
#define		PDIR_LCDSTR	P11DIR
//#define		PDIR_XLZQOUT	P11DIR

#define		PIN_LCDSDA	P11IN
#define		PIN_LCDCLK	P11IN
#define		PIN_LCDSTR	P11IN

#define		POUT_LCDSDA	P11OUT
#define		POUT_LCDCLK	P11OUT
#define		POUT_LCDSTR	P11OUT
//#define		POUT_XLZQOUT	P11OUT

#define		P_JDQB		0x04				//费控表磁保持继电器B口
#define		PDIR_JDQB	P11DIR
#define		POUT_JDQB	P11OUT

//#define		PDS_JDQB	P11DS				//费控表磁保持继电器B口


// Macro Define
#define		SCLEN			P3DIR
#define		SDAIN			P3IN
#define		SDAEN			P3DIR

//#define     ADCS    0x01
//#define     ADDIN   0x02
//#define     ADCLK   0x04
//#define     MDOUT   0x08

#define		DF_CS2		P7DIR
#define		DF_CS		P8DIR
#define		DF_RST		P8DIR
#define		DF_SCK		P8DIR
#define		DF_SDI		P8DIR
#define		DF_SDO		P8IN

*/

/************************************************************
	Port Define
************************************************************/

/****
GPIOA
****/
#define 	GPIOA_PLC_EVE				GPIO_Pin_0
#define 	GPIOA_PLC_RST				GPIO_Pin_1
#define 	GPIOA_PLC_SET				GPIO_Pin_3
#define 	GPIOA_PLC_STA				GPIO_Pin_2

#define 	GPIOA_PA4_NC				GPIO_Pin_4
#define 	GPIOA_LOWPOW				GPIO_Pin_5
#define 	GPIOA_ICP_JTAG				GPIO_Pin_6
#define 	GPIOA_KEY_Terminal_cover	GPIO_Pin_10//GPIO_Pin_7

#define 	GPIOA_KEY_Cover				GPIO_Pin_9//GPIO_Pin_8
#define 	GPIOA_KEY_DispDn			GPIO_Pin_9
#define 	GPIOA_KEY_DispUp			GPIO_Pin_10
#define 	GPIOA_Hall_INT				GPIO_Pin_11
                                		
#define 	GPIOA_BAT_ADC0				GPIO_Pin_12
#define 	GPIOA_BAT_ADC1				GPIO_Pin_13


/****
GPIOB
****/
#define 	GPIOB_FCS1					GPIO_Pin_0
#define 	GPIOB_FSO					GPIO_Pin_1
#define 	GPIOB_FSI					GPIO_Pin_2
#define 	GPIOB_FSCLK					GPIO_Pin_3

#define 	GPIOB_VEMU_CTL				GPIO_Pin_4
#define 	GPIOB_VMEM_CTL				GPIO_Pin_5
#define 	GPIOB_Backlight				GPIO_Pin_6

#define 	GPIOE_FM_SDA				GPIO_Pin_0//GPIO_Pin_7
#define 	GPIOB_FM_SCL				GPIO_Pin_13//GPIO_Pin_8 

#define 	GPIOB_LCDSDA				GPIO_Pin_9 
#define 	GPIOB_LCDSCL				GPIO_Pin_10
#define 	GPIOB_VCARD_CTRL			GPIO_Pin_11

#define 	GPIOB_CARD_KEY				GPIO_Pin_12
#define 	GPIOB_ICP_TMS				GPIO_Pin_13
#define 	GPIOB_PB14_NC				GPIO_Pin_14
#define 	GPIOB_ICP_CLK				GPIO_Pin_15   


/****
GPIOC
****/
#define 	GPIOC_232_TXD				GPIO_Pin_11//GPIO_Pin_0
#define 	GPIOC_232_RXD				GPIO_Pin_12//GPIO_Pin_1
#define 	GPIOC_GPRS_RXD				GPIO_Pin_2
#define 	GPIOC_GPRS_TXD				GPIO_Pin_3

#define 	GPIOC_485_RXD2				GPIO_Pin_4
#define 	GPIOC_485_TXD2				GPIO_Pin_5
#define 	GPIOC_485_CTL2				GPIO_Pin_6
#define 	GPIOC_ALM_LED				GPIO_Pin_7

#define 	GPIOC_IPC_SEC				GPIO_Pin_8
#define 	GPIOC_EMU_CS				GPIO_Pin_9
#define 	GPIOC_EMU_DIN				GPIO_Pin_10

#define 	GPIOC_MODEM_TXD				GPIO_Pin_0//GPIO_Pin_11
#define 	GPIOC_MODEM_RXD				GPIO_Pin_1//GPIO_Pin_12

#define 	GPIOC_EE_SCL				GPIO_Pin_13
#define 	GPIOC_EE_SDA				GPIO_Pin_14

/****
GPIOD
****/
#define 	GPIOD_PD0_NC				GPIO_Pin_0
#define 	GPIOD_PD1_NC				GPIO_Pin_1
#define 	GPIOD_PD2_NC				GPIO_Pin_2
#define 	GPIOD_SECOUT				GPIO_Pin_3

#define 	GPIOD_LREV					GPIO_Pin_4
#define 	GPIOD_RLY_LED				GPIO_Pin_5
#define 	GPIOD_RLY_ALM				GPIO_Pin_6
#define 	GPIOD_SETEN					GPIO_Pin_7

#define 	GPIOD_RLYOFF			GPIO_Pin_9//	GPIO_Pin_8 
#define 	GPIOD_RLYON				GPIO_Pin_10//	GPIO_Pin_9 
#define 	GPIOD_ChckRelay				GPIO_Pin_10
#define 	GPIOD_AUXChck				GPIO_Pin_10
#define 	GPIOD_VESAM_CTL				GPIO_Pin_11

#define 	GPIOD_EMU_PLS				GPIO_Pin_12 
#define 	GPIOD_PRGRM					GPIO_Pin_2//GPIO_Pin_13 
#define 	GPIOD_PD14_NC				GPIO_Pin_14 
#define 	GPIOD_485_CTL1				GPIO_Pin_15 


/****
GPIOE
****/
#define 	GPIOE_Disp_CTL				GPIO_Pin_0
#define 	GPIOE_PE1_NC				GPIO_Pin_1
#define 	GPIOE_PE2_NC				GPIO_Pin_2
#define 	GPIOE_BEEP				GPIO_Pin_3
                                         
#define 	SW485_2					GPIO_Pin_3
#define 	GPIOE_CARDIO				GPIO_Pin_5
#define 	SW485_1					GPIO_Pin_6
#define 	GPIOE_LVDIN0				GPIO_Pin_7

#define 	GPIOE_CARDRST				GPIO_Pin_8


/****
GPIOG
****/
#define 	GPIOG_VIR_CTL				GPIO_Pin_0
#define 	GPIOG_PG0_NC				GPIO_Pin_1
#define 	GPIOG_PG1_NC				GPIO_Pin_2
#define 	GPIOG_SCLRTC				GPIO_Pin_3

#define 	GPIOG_SDARTC				GPIO_Pin_4
#define 	GPIOG_PG5_NC				GPIO_Pin_5
#define 	GPIOG_ESAM_CS				GPIO_Pin_6
#define 	GPIOG_ESAM_CLK				GPIO_Pin_7

#define 	GPIOG_ESAM_MISO				GPIO_Pin_8
#define 	GPIOG_ESAM_MOSI				GPIO_Pin_9
#define 	GPIOG_PG10_NC				GPIO_Pin_10
#define 	GPIOG_PG11_NC				GPIO_Pin_11

#define 	GPIOG_PG12_NC				GPIO_Pin_12
#define 	GPIOG_EMU_PM0				GPIO_Pin_13
#define 	GPIOG_EMU_PM1				GPIO_Pin_14


/****
GPIOH
****/
#define 	GPIOH_KEY_DispDn			GPIO_Pin_3
#define 	GPIOH_KEY_DispUp			GPIO_Pin_2

#define 	GPIOH_EMU_DOUT				GPIO_Pin_0
#define 	GPIOH_EMU_CLK				GPIO_Pin_1
#define 	GPIOH_WG					GPIO_Pin_2
#define 	GPIOH_YG					GPIO_Pin_3

#define 	GPIOH_EMU_RST				GPIO_Pin_4


#define 	P_CARDINPUT	0x02
#define 	P_CARDOUT	0x04


//#define		DF_CS2		P7DIR
#define		DF_CS		HT_GPIOB->PTDIR
//#define		DF_RST		HT_GPIOB->PTDIR
#define		DF_SCK		HT_GPIOB->PTDIR
#define		DF_SDI		HT_GPIOB->PTDIR
#define		DF_SDO		HT_GPIOB->PTDAT
#define 	FCS1		GPIO_Pin_5
#define 	FSO		GPIO_Pin_2
#define 	FSI		GPIO_Pin_3
#define 	FSCLK		GPIO_Pin_9


#define		PIN_SETEN   HT_GPIOD->PTDAT
#define		P_SETEN     GPIO_Pin_3

#define		P_Reset		0x0100
#define		P_Scroll	0x0200
#define		P_Program	0x0300

