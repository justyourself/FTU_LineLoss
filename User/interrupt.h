#define	TM_20ms		4+1
#define	TM_500ms		100+1
#define	TM_1000ms	200+1

/*
interrupt [NMI_VECTOR]         	void Int_NMI(void);
interrupt [TIMERB0_VECTOR]     	void Int_TimerB0(void);
interrupt [TIMERB1_VECTOR]     	void Int_TimerB1(void);
interrupt [COMPARATORA_VECTOR]	void Int_CompA(void);
interrupt [WDT_VECTOR]				void Int_WDT(void);
interrupt [UART0RX_VECTOR]			void Int_Uart0Rx(void);
interrupt [UART0TX_VECTOR]			void Int_Uart0Tx(void);
interrupt [ADC_VECTOR]				void Int_ADC(void);
interrupt [TIMERA0_VECTOR]			void Int_TimerA0(void);
interrupt [TIMERA1_VECTOR]			void Int_TimerA1(void);
interrupt [PORT1_VECTOR]			void Int_Port1(void);
#if ( CPUCHIP == MSPF449 )
interrupt [UART1RX_VECTOR]			void Int_Uart1Rx(void);
interrupt [UART1TX_VECTOR]			void Int_Uart1Tx(void);
#else
#endif
interrupt [PORT2_VECTOR]			void Int_Port2(void);
interrupt [BASICTIMER_VECTOR]   	void Int_BasicTm(void);
*/
/*
__interrupt void Int_NMI(void);
__interrupt void Int_TimerB0(void);
__interrupt void Int_TimerB1(void);
__interrupt void Int_CompA(void);
__interrupt void Int_WDT(void);
__interrupt void Int_Uart0Rx(void);
__interrupt void Int_Uart0Tx(void);
__interrupt void Int_ADC(void);
__interrupt void Int_TimerA0(void);
__interrupt void Int_TimerA1(void);
__interrupt void Int_Port1(void);
__interrupt void Int_Uart1Rx(void);
__interrupt void Int_Uart1Tx(void);
__interrupt void Int_Port2(void);
*/
/*
__interrupt void Int_UNMI(void);
__interrupt void Int_SYSNMI(void);
__interrupt void Int_TimerB0(void);
__interrupt void Int_TimerB1(void);
__interrupt void Int_WDT(void);
__interrupt void Int_UartA0(void);
__interrupt void Int_USCIB0(void);
__interrupt void Int_ADC(void);
__interrupt void Int_Timer0A0(void);
__interrupt void Int_Timer0A1(void);
__interrupt void Int_UartA2(void);
__interrupt void Int_USCIB2(void);
__interrupt void Int_DMA(void);
__interrupt void Int_Timer1A0(void);
__interrupt void Int_Timer1A1(void);
__interrupt void Int_Port1(void);
__interrupt void Int_UartA1(void);
__interrupt void Int_USCIB1(void);
__interrupt void Int_UartA3(void);
__interrupt void Int_USCIB3(void);
__interrupt void Int_Port2(void);
__interrupt void Int_RTC(void);
*/

extern void _WKUP(void);

void HwTxInitial(void);
void HwRxInitial(void);

void PushKey( void );

void ICTxInitial1(void);
void ICRxInitial1(void);
void ICTxInitial2(void);
void ICRxInitial2(void);
