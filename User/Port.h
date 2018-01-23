
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

#define 	GPIOC_EE_SCL				GPIO_Pin_9
#define 	GPIOC_EE_SDA				GPIO_Pin_3

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


#define		DF_CS		HT_GPIOE->PTDAT
#define		DF_SCK		HT_GPIOC->PTDAT
#define		DF_SDI		HT_GPIOC->PTDAT
#define		DF_SDO		HT_GPIOE->PTDAT
#define 	FCS1		GPIO_Pin_2
#define 	FSO		GPIO_Pin_1
#define 	FSI		GPIO_Pin_13
#define 	FSCLK		GPIO_Pin_14

#define 	GPIOG_EMU_CLK				GPIO_Pin_7
#define 	GPIOG_EMU_DOUT				GPIO_Pin_8
#define 	GPIOG_EMU_DIN				GPIO_Pin_9
#define 	GPIOB_EMU_RST8				GPIO_Pin_4
#define 	GPIOA_EMU_RST1				GPIO_Pin_10
#define 	GPIOA_EMU_CS1				GPIO_Pin_9
#define 	GPIOA_EMU_RST6				GPIO_Pin_3
#define 	GPIOA_EMU_CS5				GPIO_Pin_5
#define 	GPIOA_EMU_RST7				GPIO_Pin_8
#define 	GPIOD_EMU_RST2				GPIO_Pin_3
#define 	GPIOD_EMU_CS2				GPIO_Pin_4
#define 	GPIOD_EMU_RST3				GPIO_Pin_8
#define 	GPIOD_EMU_CS3				GPIO_Pin_9
#define 	GPIOD_EMU_RST4				GPIO_Pin_11
#define 	GPIOD_EMU_RST5				GPIO_Pin_13
#define 	GPIOG_EMU_CS4				GPIO_Pin_10
#define 	GPIOH_EMU_CS6				GPIO_Pin_3
#define 	GPIOH_EMU_CS8				GPIO_Pin_0
#define 	GPIOB_EMU_CS7				GPIO_Pin_0


#define		PIN_SETEN   HT_GPIOD->PTDAT
#define		P_SETEN     GPIO_Pin_3

#define		P_Reset		0x0100
#define		P_Scroll	0x0200
#define		P_Program	0x0300

