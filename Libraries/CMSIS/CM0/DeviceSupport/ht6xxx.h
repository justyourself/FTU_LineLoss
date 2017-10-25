/*
**********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2014, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.2
* Description  : HT6x2x, HT6x1x, HT501x, HT502x are supported
**********************************************************************************************************
*/

#ifndef __HT6XXX_H__
#define __HT6XXX_H__

#ifdef __cplusplus
extern "C" {
#endif     
  
/*
**********************************************************************************************************
*                                             全局宏/结构体
**********************************************************************************************************
*/
#if !defined (HT6x2x) && !defined (HT6x1x) && !defined (HT501x) && !defined (HT502x)
 #error "Please select one HT device used in your application"
#endif

#if defined (HT6x1x)
 #if defined (HT6x2x) || defined (HT501x) || defined (HT502x)
   #error "More than one HT device used in your application"
 #endif
#elif defined (HT6x2x)
 #if defined (HT501x) || defined (HT502x)
   #error "More than one HT device used in your application"
 #endif
#else
 #if defined (HT501x) &&  defined (HT502x)
   #error "More than one HT device used in your application"
 #endif
#endif

#define  CurrentVersion   Version1_7_20161226
    
/* 
* @brief  General Bool State  
*/ 
typedef enum {DISABLE = 0, ENABLE = !DISABLE}   FunctionalState;        /* 用于使能/非使能            */
typedef enum {RESET   = 0, SET    = !RESET  }   FlagStatus, ITStatus;   /* 用于状态或者中断标志置1置0 */
typedef enum {FALSE   = 0, TRUE   = !FALSE  }   Bool;                   /* 用于函数返回               */

    
/*
**********************************************************************************************************
*                                       Interrupt Number Definition
**********************************************************************************************************
*/  
typedef enum IRQn
{
/*****************************  Cortex-M0 Processor Exceptions Numbers  *********************************/
    NonMaskableInt_IRQn         = -14,      /*!< 2 Non Maskable Interrupt  CMU                    */
    HardFault_IRQn              = -13,      /*!< 3 Cortex-M0 Hard Fault Interrupt                 */
    SVCall_IRQn                 = -5,       /*!< 11 Cortex-M0 SV Call Interrupt                   */
    PendSV_IRQn                 = -2,       /*!< 14 Cortex-M0 Pend SV Interrupt                   */
    SysTick_IRQn                = -1,       /*!< 15 Cortex-M0 System Tick Interrupt               */

/*****************************  HT6xxx Specific Interrupt Numbers  **************************************/
    PMU_IRQn                    = 0,        /*!< PMU Interrupt                                    */
#if defined  HT6x1x
    TDES_IRQn                   = 1,        /*!< 3DES Interrupt                                   */
#else
    AES_IRQn                    = 1,        /*!< AES Interrupt                                    */
																						/*!<   HT6x2x: includ AES, RAND, GHASH                */
																						/*!<   HT501x: includ AES, GHASH                      */
																						/*!<   HT502x: includ AES, RAND, GHASH, ECC           */
#endif
    EXTI0_IRQn                  = 2,        /*!< External Interrupt 0 Interrupt                   */
    EXTI1_IRQn                  = 3,        /*!< External Interrupt 1 Interrupt                   */
    EXTI2_IRQn                  = 4,        /*!< External Interrupt 2 Interrupt                   */
    EXTI3_IRQn                  = 5,        /*!< External Interrupt 3 Interrupt                   */
    EXTI4_IRQn                  = 6,        /*!< External Interrupt 4 Interrupt                   */
    EXTI5_IRQn                  = 7,        /*!< External Interrupt 5 Interrupt                   */
    EXTI6_IRQn                  = 8,        /*!< External Interrupt 6 Interrupt                   */
    UART0_IRQn                  = 9,        /*!< UART0 Interrupt                                  */
    UART1_IRQn                  = 10,       /*!< UART1 Interrupt                                  */
    UART2_IRQn                  = 11,       /*!< UART2 Interrupt                                  */
    UART3_IRQn                  = 12,       /*!< UART3 Interrupt                                  */
    UART4_IRQn                  = 13,       /*!< UART4 Interrupt                                  */
    UART5_IRQn                  = 14,       /*!< UART5 Interrupt                                  */
    TIMER_0_IRQn                = 15,       /*!< Timer0 Interrupt                                 */
    TIMER_1_IRQn                = 16,       /*!< Timer1 Interrupt                                 */
    TIMER_2_IRQn                = 17,       /*!< Timer2 Interrupt                                 */
    TIMER_3_IRQn                = 18,       /*!< Timer3 Interrupt                                 */
    TBS_IRQn                    = 19,       /*!< TBS Interrupt                                    */
    RTC_IRQn                    = 20,       /*!< RTC Interrupt                                    */
    I2C_IRQn                    = 21,       /*!< I2C Converter Interrupt                          */
    SPI0_IRQn                   = 22,       /*!< SPI0 Interrupt                                   */
#if   defined HT6x1x
    WDT_IRQn                    = 23,       /*!< WDT Interrupt                                    */
#elif defined HT501x
    EMU_IRQn                    = 25,       /*!< EMU Interrupt                                    */
    DMA_IRQn                    = 26,       /*!< DMA Interrupt                                    */
    KEY_IRQn                    = 27,       /*!< KEY Scan Interrupt                               */
#elif defined HT502x
    EMU_IRQn                    = 25,       /*!< EMU Interrupt                                    */
    DMA_IRQn                    = 26,       /*!< DMA Interrupt                                    */
    KEY_IRQn                    = 27,       /*!< KEY Scan Interrupt                               */
    EXTI7_IRQn                  = 28,       /*!< External Interrupt 7 Interrupt                   */
    EXTI8_IRQn                  = 29,       /*!< External Interrupt 8 Interrupt                   */
    EXTI9_IRQn                  = 30,       /*!< External Interrupt 9 Interrupt                   */
#elif defined HT6x2x
    SPI1_IRQn                   = 23,       /*!< SPI1 Interrupt                                   */
    SelfTestF_IRQn              = 24,       /*!< SelfTestF Interrupt                              */
    TIMER_4_IRQn                = 25,       /*!< Timer4 Interrupt                                 */
    TIMER_5_IRQn                = 26,       /*!< Timer5 Interrupt                                 */
    UART6_IRQn                  = 27,       /*!< UART6 Interrupt                                  */
    EXTI7_IRQn                  = 28,       /*!< External Interrupt 7 Interrupt                   */
    EXTI8_IRQn                  = 29,       /*!< External Interrupt 8 Interrupt                   */
    EXTI9_IRQn                  = 30,       /*!< External Interrupt 9 Interrupt                   */
    DMA_IRQn                    = 31,	      /*!< DMA Interrupt                   	    	          */
#endif
} IRQn_Type;


/*
**********************************************************************************************************
*                                   Processor and Core Peripheral Section
**********************************************************************************************************
*/  

/********************  Configuration of the Cortex-M0 Processor and Core Peripherals  *******************/
#define __MPU_PRESENT             0         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */
                                            /* end of group HT60xx_CMSIS                          */

#include "core_cm0.h"                       /* Cortex-M0 processor and core peripherals           */
#include <stdint.h>                         /* standard types definitions                         */


/*
**********************************************************************************************************
*                                 Device Specific Peripheral Registers structures 
**********************************************************************************************************
*/  

/*****************************************  Timer Control Block  *****************************************/
/* 
* @brief  定时器寄存器定义
*/
typedef struct
{
    __IO uint32_t TMRCON;                   /*!< Offset: 0x000 通道控制寄存器      (R/W) */
    __IO uint32_t TMRDIV;                   /*!< Offset: 0x004 通道预分频寄存器    (R/W) */
    __IO uint32_t TMRPRD;                   /*!< Offset: 0x008 通道周期寄存器      (R/W) */
    __IO uint32_t TMRCAP;                   /*!< Offset: 0x00C 通道捕获数据寄存器  (R/W) */
    __IO uint32_t TMRCNT;                   /*!< Offset: 0x010 通道计数器寄存器    (R/W) */
    __IO uint32_t TMRCMP;                   /*!< Offset: 0x014 通道比较器寄存器    (R/W) */
    __IO uint32_t TMRIE;                    /*!< Offset: 0x018 通道中断使能寄存器  (R/W) */
    __IO uint32_t TMRIF;                    /*!< Offset: 0x01C 通道中断标志寄存器  (R/W) */
} HT_TMR_TypeDef;                           /* end of group HT_TMR_TypeDef               */


/*****************************************  UART Control Block  ******************************************/
/* 
* @brief  UART寄存器定义
*/
typedef struct
{          
    __IO uint32_t MODESEL;                  /*!< Offset: 0x000 UART/7816功能选择寄存器    (R/W) */
    __IO uint32_t UARTCON;                  /*!< Offset: 0x004 串口功能配置寄存器         (R/W) */
    __IO uint32_t ISO7816CON;               /*!< Offset: 0x008 7816功能配置寄存器         (R/W) */ 
    __IO uint32_t SREL;                     /*!< Offset: 0x00C 串口/7816波特率发生器重载值(R/W) */
    __IO uint32_t SBUF;                     /*!< Offset: 0x010 串口/7816数据缓冲寄存器    (R/W) */
    __IO uint32_t UARTSTA;                  /*!< Offset: 0x014 串口状态寄存器             (R/W) */
    __IO uint32_t ISO7816STA;               /*!< Offset: 0x018 7816状态寄存器             (R/W) */ 
    __I  uint32_t Reserved[5];  
    __IO uint32_t IRCON;                    /*!< Offset: 0x030 红外调制控制寄存器         (R/W) */ 
    __IO uint32_t IRDUTY;                   /*!< Offset: 0x034 红外调制脉宽调整寄存器     (R/W) */ 
#if defined  HT6x2x    
    __IO uint32_t LOGDETNUM;                /*!< Offset: 0x038 485正反逻辑判断数据长度设置(R/W) */ 
#endif   
} HT_UART_TypeDef, HT_ISO7816_TypeDef;      /* end of group HT_UART_TypeDef, HT_ISO7816_TypeDef */


/*****************************************  I2C Control Block  *******************************************/
/*
* @brief  I2C寄存器定义
*/
typedef struct
{
    __IO uint32_t I2CDAT;                   /*!< Offset: 0x000 I2C数据寄存器 (R/W) */
    __IO uint32_t I2CADR;                   /*!< Offset: 0x004 I2C地址寄存器 (R/W) */
    __IO uint32_t I2CCON;                   /*!< Offset: 0x008 I2C控制寄存器 (R/W) */
    __IO uint32_t I2CSTA;                   /*!< Offset: 0x00C I2C状态寄存器 (R/W) */
} HT_I2C_TypeDef;                           /* end of group HT_I2C_TypeDef         */


/*****************************************  SPI Control Block  *******************************************/
/*
* @brief  SPI寄存器定义
*/
typedef struct
{
    __IO uint32_t SPICON;                   /*!< Offset: 0x000 SPI控制寄存器     (R/W) */
    __IO uint32_t SPISTA;                   /*!< Offset: 0x004 SPI状态寄存器     (R/W) */
    __IO uint32_t SPIDAT;                   /*!< Offset: 0x008 SPI数据寄存器     (R/W) */
    __IO uint32_t SPISSN;                   /*!< Offset: 0x00C SPI从机选择寄存器 (R/W) */
} HT_SPI_TypeDef;                           /* end of group HT_SPI_TypeDef             */
        
        
/*****************************************  RTC Control Block  *******************************************/
/*
* @brief  RTC寄存器定义
*/
typedef struct
{
    __IO uint32_t RTCCON;                   /*!< Offset: 0x000 RTC控制寄存器 (R/W)                     */
    __IO uint32_t RTCIE;                    /*!< Offset: 0x004 RTC中断使能寄存器 (R/W)                 */
    __IO uint32_t RTCIF;                    /*!< Offset: 0x008 RTC中断标志寄存器 (R/W)                 */
    __IO uint32_t ALMR;                     /*!< Offset: 0x00C 闹钟寄存器 (R/W)                        */
    __IO uint32_t RTCTMR1;                  /*!< Offset: 0x010 RTC定时器1计数设置 (R/W)                */
    __IO uint32_t RTCTMR2;                  /*!< Offset: 0x014 RTC定时器2计数设置 (R/W)                */
    __IO uint32_t SECR;                     /*!< Offset: 0x018 秒寄存器 (R/W)                          */
    __IO uint32_t MINR;                     /*!< Offset: 0x01C 分钟寄存器 (R/W)                        */
    __IO uint32_t HOURR;                    /*!< Offset: 0x020 小时寄存器 (R/W)                        */
    __IO uint32_t DAYR;                     /*!< Offset: 0x024 天寄存器 (R/W)                          */
    __IO uint32_t MONTHR;                   /*!< Offset: 0x028 月寄存器 (R/W)                          */
    __IO uint32_t YEARR;                    /*!< Offset: 0x02C 年寄存器 (R/W)                          */
    __IO uint32_t WEEKR;                    /*!< Offset: 0x030 星期寄存器 (R/W)                        */
    __IO uint32_t RTCCNTH;                  /*!< Offset: 0x034 校时次数寄存器高16位 (R/W )             */
    __IO uint32_t RTCCNTL;                  /*!< Offset: 0x038 校时次数寄存器低16位 (R/W )             */
    __IO uint32_t RTCRD;                    /*!< Offset: 0x03C RTC读控制寄存器 (R/W )                  */
    __IO uint32_t RTCWR;                    /*!< Offset: 0x040 RTC写控制寄存器 (R/W)                   */
    __I  uint32_t Reserved[3];  
    __IO uint32_t DFAH;                     /*!< Offset: 0x050 Mems时钟频率时的常数项（高7位） (R/W )  */
    __IO uint32_t DFAL;                     /*!< Offset: 0x054 Mems时钟频率时的常数项（低16位）(R/W )  */
    __IO uint32_t DFBH;                     /*!< Offset: 0x058 Mems时钟频率时的一次项（高7位） (R/W )  */
    __IO uint32_t DFBL;                     /*!< Offset: 0x05C Mems时钟频率时的一次项（低16位）(R/W )  */
    __IO uint32_t DFCH;                     /*!< Offset: 0x060 Mems时钟频率时的二次项（高7位） (R/W )  */
    __IO uint32_t DFCL;                     /*!< Offset: 0x064 Mems时钟频率时的二次项（低16位）(R/W )  */
    __IO uint32_t DFDH;                     /*!< Offset: 0x068 Mems时钟频率时的三次项（高7位） (R/W )  */
    __IO uint32_t DFDL;                     /*!< Offset: 0x06C Mems时钟频率时的三次项（低16位）(R/W )  */
    __IO uint32_t DFEH;                     /*!< Offset: 0x070 Mems时钟频率时的四次项（高7位） (R/W )  */
    __IO uint32_t DFEL;                     /*!< Offset: 0x074 Mems时钟频率时的四次项（低16位）(R/W )  */ 
    __IO uint32_t Toff;                     /*!< Offset: 0x078 温度偏置寄存器 (R/W )                   */ 
    __IO uint32_t MCON01;                   /*!< Offset: 0x07C Mems控制寄存器01 (R/W )                 */
    __IO uint32_t MCON23;                   /*!< Offset: 0x080 Mems控制寄存器23 (R/W )                 */
    __IO uint32_t MCON45;                   /*!< Offset: 0x084 Mems控制寄存器45 (R/W )                 */
    __IO uint32_t DFIH;                     /*!< Offset: 0x088 频率误差寄存器（高5位） (R/W )          */
    __IO uint32_t DFIL;                     /*!< Offset: 0x08C 频率误差寄存器（低16位） (R/W )         */
    __IO uint32_t RTCRSTFLG;                /*!< Offset: 0x090 RTC模块复位标志寄存器 (R/W )            */
    __IO uint32_t RTCRSTSET;                /*!< Offset: 0x094 RTC写复位寄存器 (R/W )                  */
#if defined  HT501x  ||  HT502x
    __I  uint32_t Reserved2[22];
#elif defined  HT6x2x    
    __I  uint32_t RTCCHKSUM;                /*!< Offset: 0x098 RTC写复位寄存器 (R/W )                  */    
    __I  uint32_t Reserved2[13];    
    __IO uint32_t HRa;                      /*!< Offset: 0x0D0 HRC1Hz补偿常数项 (R/W)                  */
    __IO uint32_t HRb;                      /*!< Offset: 0x0D4 HRC1Hz补偿一次项 (R/W)                  */  
    __IO uint32_t HRc;                      /*!< Offset: 0x0D8 HRC1Hz补偿二次项 (R/W)                  */ 
    __IO uint32_t HRd;                      /*!< Offset: 0x0DC HRC1Hz补偿三次项 (R/W)                  */                    
    __IO uint32_t HRe;                      /*!< Offset: 0x0E0 HRC1Hz补偿四次项 (R/W)                  */ 
    __IO uint32_t HDFi;                     /*!< Offset: 0x0E4 HRC1Hz频率误差寄存器 (R/W)              */  
    __I  uint32_t Reserved3[2]; 
#endif
#if defined  HT501x  || HT502x  || HT6x2x
    __IO uint32_t HFCFG;                    /*!< Offset: 0x0F0 测频模块配置寄存器 对用户不开放 (R/W)   */ 
    __IO uint32_t HFNUM;                    /*!< Offset: 0x0F4 设置测频脉冲周期个数 不开放(R/W)        */
    __IO uint32_t HFCNTH;                   /*!< Offset: 0x0F8 测频个数高16位 不开放(R/W)              */
    __IO uint32_t HFCNTL;                   /*!< Offset: 0x0FC 测频个数低16位 不开放(R/W)              */                 
    __IO uint32_t CTRLBYFLASH;              /*!< Offset: 0x100 此寄存器对用户不开放 (R/W )             */
	  __IO uint32_t LRCCOMAND;                /*!< Offset: 0x104 此寄存器对用户不开放 (R/W )             */
    __I  uint32_t Reserved4[62];                
    __IO uint32_t SECR2;                    /*!< Offset: 0x200 秒寄存器 (R/W)                          */
    __IO uint32_t MINR2;                    /*!< Offset: 0x204 分钟寄存器 (R/W)                        */
    __IO uint32_t HOURR2;                   /*!< Offset: 0x208 小时寄存器 (R/W)                        */
    __IO uint32_t DAYR2;                    /*!< Offset: 0x20C 天寄存器 (R/W)                          */
    __IO uint32_t MONTHR2;                  /*!< Offset: 0x210 月寄存器 (R/W)                          */
    __IO uint32_t YEARR2;                   /*!< Offset: 0x214 年寄存器 (R/W)                          */
    __IO uint32_t WEEKR2;                   /*!< Offset: 0x218 星期寄存器 (R/W)                        */   
    __IO uint32_t RTC2CAL;                  /*!< Offset: 0x21C LRC计时调整寄存器 (R/W)                 */
		__IO uint32_t RTCRD2;                   /*!< Offset: 0x220 RTC读控制寄存器 (R/W )                  */ 
    __IO uint32_t RTCWR2;                   /*!< Offset: 0x224 RTC写控制寄存器 (R/W )                  */ 
#endif  
#if defined  HT501x  || HT502x
    __I  uint32_t FRE_LRC;                  /*!< Offset: 0x228 LRC频率测量值 (R)                       */
#endif
} HT_RTC_TypeDef;                           /* end of group HT_RTC_TypeDef                             */


/*****************************************  LCD Control Block  *******************************************/
/*
* @brief  LCD寄存器定义
*/
typedef struct
{
    __IO uint32_t LCDCLK;                   /*!< Offset: 0x000 LCD时钟频率选择寄存器 (R/W)  */
    __IO uint32_t LCDCON;                   /*!< Offset: 0x004 LCD驱动控制寄存器 (R/W)      */
    __IO uint32_t Reserved;
	  __IO uint32_t LCDOUT;                   /*!< Offset: 0x00C LCD输出控制寄存器 (R/W)      */
#if defined  HT6x1x
    __IO uint32_t LCDBUF[37];               /*!< Offset: 0x010 LCD显示数据寄存器 (R/W)      */
#elif defined  HT502x
    __IO uint32_t LCDBUF[53];               /*!< Offset: 0x010 LCD显示数据寄存器 (R/W)      */
#else
    __IO uint32_t LCDBUF[43];               /*!< Offset: 0x010 LCD显示数据寄存器 (R/W)      */
#endif
} HT_LCD_TypeDef;                           /* end of group HT_LCD_TypeDef                  */


/*****************************************  TBS Control Block  *******************************************/
/*
* @brief  TBS寄存器定义
*/
typedef struct
{
#if defined HT501x  || HT502x
    __IO uint32_t TBSCON;                   /*!< Offset: 0x000 TBS设置寄存器 (R/W)      */
    __IO uint32_t TBSPRD;                   /*!< Offset: 0x004 打开频率设置寄存器 (R/W) */    
    __IO uint32_t TBSIE;                    /*!< Offset: 0x008 TBS中断使能寄存器 (R/W)  */
    __IO uint32_t TBSIF;                    /*!< Offset: 0x00C TBS中断标志寄存器 (R/W)  */
    __I  uint32_t TMPDAT;                   /*!< Offset: 0x010 温度测量输出值 (R/)      */
    __I  uint32_t VBATDAT;                  /*!< Offset: 0x014 ADC通道2测量输出值 (R/)  */
    __I  uint32_t VCCDAT;                   /*!< Offset: 0x018 电源电压测量输出值(R/)   */
																						/*!<               HT502x对应手册中VDDDAT   */
    __I  uint32_t ADC0DAT;                  /*!< Offset: 0x01C ADC通道0测量输出值 (R/)  */
    __I  uint32_t ADC1DAT;                  /*!< Offset: 0x020 ADC通道1测量输出值 (R/)  */
    __I  uint32_t ADC2DAT;                  /*!< Offset: 0x024 ADC通道2测量输出值 (R/)  */    
    __IO uint32_t VBATCMP;                  /*!< Offset: 0x028 VBAT比较寄存器 (R/W)     */
    __I  uint32_t Reserved[2];             
    __IO uint32_t ADC0CMP;                  /*!< Offset: 0x034 ADC0比较寄存器 (R/W)     */
    __IO uint32_t ADC1CMP;                  /*!< Offset: 0x038 ADC1比较寄存器 (R/W)     */    
#else
    __IO uint32_t TBSCON;                   /*!< Offset: 0x000 TBS设置寄存器 (R/W)      */
    __IO uint32_t TBSIE;                    /*!< Offset: 0x004 TBS中断使能寄存器 (R/W)  */
    __IO uint32_t TBSIF;                    /*!< Offset: 0x008 TBS中断标志寄存器 (R/W)  */
    __I  uint32_t TMPDAT;                   /*!< Offset: 0x00C 温度测量输出值 (R/)      */
    __I  uint32_t VBATDAT;                  /*!< Offset: 0x010 Vbat测量输出值 (R/)      */
    __I  uint32_t ADC0DAT;                  /*!< Offset: 0x014 ADC通道0测量输出值 (R/)  */
    __I  uint32_t ADC1DAT;                  /*!< Offset: 0x018 ADC通道1测量输出值 (R/)  */
    __IO uint32_t VBATCMP;                  /*!< Offset: 0x01C VBAT比较寄存器 (R/W)     */
    __IO uint32_t TBSPRD;                   /*!< Offset: 0x020 打开频率设置寄存器 (R/W) */
    __IO uint32_t TBSTEST;                  /*!< Offset: 0x024 TBS测试寄存器 (R/W)      */
    __I  uint32_t VCCDAT;                   /*!< Offset: 0x028 电源电压测量输出值(R/)*/
#if defined  HT6x2x
    __IO uint32_t ADC0CMP;                  /*!< Offset: 0x02C ADC0比较寄存器 (R/)      */  
		__IO uint32_t ADC3DAT;                  /*!< Offset: 0x030 ADC3测量输出值 (R/)      */ 
		__IO uint32_t ADC4DAT;                  /*!< Offset: 0x034 ADC4测量输出值 (R/)      */ 
		__IO uint32_t ADC5DAT;                  /*!< Offset: 0x038 ADC5测量输出值 (R/)      */ 
		__IO uint32_t TBSPRD2;                  /*!< Offset: 0x03C 打开频率设置寄存器 (R/W) */
#endif  
#endif    
} HT_TBS_TypeDef;                           /* end of group HT_TBS_TypeDef              */


/*****************************************  PMU Control Block  *******************************************/
/*
* @brief  PMU寄存器定义
*/
typedef struct
{
    __IO uint32_t PMUCON;                   /*!< Offset: 0x000 PMU配置寄存器 (R/W)              */
    __IO uint32_t VDETCFG;                  /*!< Offset: 0x004 电源检测阈值配置寄存器 (R/W)     */
    __IO uint32_t VDETPCFG;                 /*!< Offset: 0x008 电源检测时间周期配置寄存器 (R/W) */
    __IO uint32_t PMUIE;                    /*!< Offset: 0x00C PMU中断使能寄存器 (R/W)          */
    __IO uint32_t PMUIF;                    /*!< Offset: 0x010 PMU中断标志寄存器 (R/W)          */
    __I  uint32_t PMUSTA;                   /*!< Offset: 0x014 PMU状态指示寄存器 (R)          */
    __IO uint32_t WAKEIF;                   /*!< Offset: 0x018 唤醒源标志寄存器 (R/W)           */
#if defined HT6x2x	
	__IO uint32_t PDTFLT;                   /*!< Offset: 0x01C POW数字滤波寄存器 (R/W)           */
    __I  uint32_t Reserved[4];
#else 
    __I  uint32_t Reserved[5];
#endif	
    __IO uint32_t RSTSTA;                   /*!< Offset: 0x030 复位标志寄存器 (R/W)             */
} HT_PMU_TypeDef;                           /* end of group HT_PMU_TypeDef                      */


/*****************************************  CMU Control Block  *******************************************/
/*
* @brief  CMU寄存器定义
*/
typedef struct
{
    __IO uint32_t WPREG;                    /*!< Offset: 0x000 写保护控制寄存器 (R/W)                    */
    __IO uint32_t SYSCLKCFG;                /*!< Offset: 0x004 系统时钟配置寄存器 (R/W)                  */
    __I  uint32_t JTAGSTA;                  /*!< Offset: 0x008 JTAG状态指示 (R)                          */
    __IO uint32_t LRCADJ;                   /*!< Offset: 0x00C 低频RC配置寄存器 (R/W )                   */
    __IO uint32_t HRCADJ;                   /*!< Offset: 0x010 高频RC配置寄存器 (R/W)                    */
    __IO uint32_t HRCDIV;                   /*!< Offset: 0x014 高频RC分频寄存器 (R/W)                    */
    __I  uint32_t CLKSTA;                   /*!< Offset: 0x018 时钟状态寄存器 (R)                        */
    __IO uint32_t SYSCLKDIV;                /*!< Offset: 0x01C 系统时钟分频寄存器 (R/W)                  */
    __I  uint32_t Reserved1[1];             
    __IO uint32_t CLKOUTSEL;                /*!< Offset: 0x024 CLKOUT时钟选择寄存器 (R/W)                */
    __IO uint32_t CLKOUTDIV;                /*!< Offset: 0x028 CLKOUT时钟分频寄存器 (R/W)                */
    __IO uint32_t CLKCTRL0;                 /*!< Offset: 0x02C 内部模块使能寄存器0 (R/W)                 */
    __IO uint32_t CLKCTRL1;                 /*!< Offset: 0x030 内部模块使能寄存器1 (R/W)                 */
    __IO uint32_t FLASHCON;                 /*!< Offset: 0x034 Flash访问控制寄存器 (R/W)                 */
    __O  uint32_t FLASHLOCK;                /*!< Offset: 0x038 Flash锁定寄存器 (W)                       */
#if defined  HT6x2x     
    __IO uint32_t PREFETCH;                 /*!< Offset: 0x03C 指令预取使能寄存器 (R/W)                  */  
    __I  uint32_t Reserved2[4]; 
#elif defined HT501x  ||  defined HT502x
    __IO uint32_t FLASHDLY;                 /*!< Offset: 0x03C Flash取指令等待 (R/W)                     */  
    __I  uint32_t Reserved2[4];     
#else
    __I  uint32_t Reserved2[5]; 
#endif      
    __IO uint32_t INFOLOCK;                 /*!< Offset: 0x050 Info Flash锁定寄存器(不开放给用户) (R/W)  */
    __IO uint32_t PORRSTSET;                /*!< Offset: 0x054 POR/LBOR复位寄存器(不开放给用户) (R/W)    */
#if defined  HT501x    
    __I  uint32_t Reserved4[2+10*4+14*64];             
    __I  uint32_t CHIPID;                   /*!< Offset: 0xF00 ChipID (R)                                */    
#elif defined  HT6x2x   
    __I  uint32_t Reserved3[2];
    __IO uint32_t RCCALICON;                /*!< Offset: 0x060 RC校正配置寄存器 (R/W)                    */
    __IO uint32_t RCCALIIE;                 /*!< Offset: 0x064 RC测频中断使能寄存器 (R/W)                */
    __IO uint32_t RCCALIIF;                 /*!< Offset: 0x068 RC测频中断标志寄存器 (R/W)                */
    __IO uint32_t HRCVALUE;                 /*!< Offset: 0x06C HRC测频值(24位无符号数) (R/W)             */     
    __IO uint32_t LRCVALUE;                 /*!< Offset: 0x070 LRC测频值(17位无符号数) (R/W)             */ 
    __I  uint32_t Reserved4[2+10*4+14*64-7];             
    __I  uint32_t CHIPID;                   /*!< Offset: 0xF00 ChipID (R)                                */
#elif defined  HT502x   
    __I  uint32_t Reserved3[2];
    __IO uint32_t FS1LOCK;                  /*!< Offset: 0x060 FlashSector1 锁定寄存器 (R/W)            */
    __IO uint32_t FS2LOCK;                  /*!< Offset: 0x064 FlashSector2 锁定寄存器 (R/W)            */
    __IO uint32_t FS3LOCK;                  /*!< Offset: 0x068 FlashSector3 锁定寄存器 (R/W)            */
    __IO uint32_t FS4LOCK;                  /*!< Offset: 0x06C FlashSector4 锁定寄存器 (R/W)            */     
    __IO uint32_t FS5LOCK;                  /*!< Offset: 0x070 FlashSector5 锁定寄存器 (R/W)            */ 
    __IO uint32_t FS6LOCK;                  /*!< Offset: 0x074 FlashSector6 锁定寄存器 (R/W)            */
    __IO uint32_t FS7LOCK;                  /*!< Offset: 0x078 FlashSector7 锁定寄存器 (R/W)            */
    __IO uint32_t FS8LOCK;                  /*!< Offset: 0x07C FlashSector8 锁定寄存器 (R/W)            */
    __IO uint32_t FS9LOCK;                  /*!< Offset: 0x080 FlashSector9 锁定寄存器 (R/W)            */     
    __IO uint32_t FSALOCK;                  /*!< Offset: 0x084 FlashSectorA 锁定寄存器 (R/W)            */ 
    __IO uint32_t FSBLOCK;                  /*!< Offset: 0x088 FlashSectorB 锁定寄存器 (R/W)            */
    __I  uint32_t Reserved4[1];
    __IO uint32_t FLASHCON2;                /*!< Offset: 0x090 Flash访问控制寄存器2 (R/W)               */
    __I  uint32_t Reserved5[2+10*4+14*64-7-8];             
    __I  uint32_t CHIPID;                   /*!< Offset: 0xF00 ChipID (R)                                */
#endif
} HT_CMU_TypeDef;                           /* end of group HT_CMU_TypeDef                               */


/*****************************************  WDT Control Block  *******************************************/
/*
* @brief  WDT寄存器定义
*/
typedef struct
{   
#if defined HT6x1x	
    __IO uint32_t WDTCFG;                   /*!< Offset: 0x000 看门狗配置寄存器 (R/W)           */ 
#else 
		__I  uint32_t Reserved;
#endif
    __IO uint32_t WDTCLR;                   /*!< Offset: 0x004 看门狗喂狗与时间配置寄存器 (R/W) */
    __I  uint32_t WDTCNT;                   /*!< Offset: 0x008 看门狗计数寄存器(R)              */
} HT_WDT_TypeDef;                           /* end of group HT_WDT_TypeDef                      */


/*****************************************  GPIO Control Block  ******************************************/
/*
* @brief GPIO寄存器定义
*/
typedef struct
{
    __IO uint32_t IOCFG;                    /*!< Offset: 0x000 端口功能配置寄存器1 (R/W)  */
    __IO uint32_t AFCFG;                    /*!< Offset: 0x004 端口功能配置寄存器2 (R/W)  */
    __IO uint32_t PTDIR;                    /*!< Offset: 0x008 端口方向配置寄存器 (R/W)   */
    __IO uint32_t PTUP;                     /*!< Offset: 0x00C 端口上拉配置寄存器 (R/W)   */
    __IO uint32_t PTDAT;                    /*!< Offset: 0x010 端口数据寄存器 (R/W)       */
    __O  uint32_t PTSET;                    /*!< Offset: 0x014 端口设置寄存器 ( /W)       */
    __O  uint32_t PTCLR;                    /*!< Offset: 0x018 端口复位寄存器 ( /W)       */
    __O  uint32_t PTTOG;                    /*!< Offset: 0x01C 端口翻转寄存器 ( /W)       */
    __IO uint32_t PTOD;                     /*!< Offset: 0x020 端口OD功能配置寄存器 (R/W) */
} HT_GPIO_TypeDef;                          /* end of group HT_GPIO_TypeDef               */


/*****************************************  INT Control Block  *******************************************/
/*
* @brief INT中断寄存器定义
*/
typedef struct
{
    __IO uint32_t EXTIE;                    /*!< Offset: 0x000 外部中断使能寄存器 (R/W) */
    __IO uint32_t EXTIF;                    /*!< Offset: 0x004 外部中断标志寄存器 (R/W) */
  	__IO uint32_t PINFLT;                    /*!< Offset: 0x008 引脚数字滤波使能寄存器 (R/W) */
#if defined  HT6x2x  || HT502x
    __IO uint32_t Reserved[1];                            
    __IO uint32_t EXTIE2;                   /*!< Offset: 0x010 外部中断使能寄存器 (R/W) */
    __IO uint32_t EXTIF2;                   /*!< Offset: 0x014 外部中断标志寄存器 (R/W) */  
	  __IO uint32_t PINFLT2;                  /*!< Offset: 0x0018 引脚数字滤波使能寄存器 (R/W) */
#endif    
} HT_INT_TypeDef;                           /* end of group HT_INT_TypeDef              */


/*****************************************  DMA Control Block  *******************************************/
/*
* @brief DMA寄存器定义
*/
#if defined HT501x  || defined HT502x || defined HT6x2x
typedef struct
{
    __IO uint32_t DMAIE;                    /*!< Offset: 0x000 中断使能寄存器 (R/W)       */
    __IO uint32_t DMAIF;                    /*!< Offset: 0x004 通道中断标志 (R/W)         */
    __IO uint32_t CHNSTA;                   /*!< Offset: 0x008 状态寄存器 (R/W)           */
} HT_DMA_TypeDef;                           /* end of group HT_DMA_TypeDef                */


typedef struct
{
    __IO uint32_t CHNCTL;                   /*!< Offset: 0x000 通道控制寄存器 (R/W)       */
    __IO uint32_t CHNSRC;                   /*!< Offset: 0x004 通道源地址寄存器 (R/W)     */
    __IO uint32_t CHNTAR;                   /*!< Offset: 0x008 通道目的地址寄存器 (R/W)   */ 
    __IO uint32_t CHNCNT;                   /*!< Offset: 0x00C 通道传输数量设置寄存器(R/W)*/
    __IO uint32_t CHNTCCNT;                 /*!< Offset: 0x010 通道传输完成数据个数 (R/W) */
    __IO uint32_t CHNBULKNUM;               /*!< Offset: 0x014 通道块传输内个数设置 (R/W) */               
} HT_DMA_Channel_TypeDef;                   /* end of group HT_DMA_Channel_TypeDef        */
#endif


/*****************************************  CRC Control Block  *******************************************/
/*
* @brief CRC寄存器定义
*/
#if defined  HT6x2x
typedef struct
{
    __IO uint32_t CRCCON;                    /*!< Offset: 0x000 CRC控制寄存器 (R/W)        */
    __IO uint32_t CRCDAT;                    /*!< Offset: 0x004 CRC数据寄存器 (R/W)        */          
} HT_CRC_TypeDef;                            /* end of group HT_CRC_TypeDef              	 */
#endif


/*****************************************  AES Control Block  *******************************************/
/*
* @brief AES寄存器定义
*/
#if defined HT501x  || defined HT502x || defined HT6x2x
typedef struct
{
    __IO uint32_t AESCON;                    /*!< Offset: 0x000 AES配置寄存器 (R/W)                       */
    __O  uint32_t AESSTR;                    /*!< Offset: 0x004 AES启动命令寄存器 (W)                     */   
    __IO uint32_t AESFLG;                    /*!< Offset: 0x008 AES标志寄存器 (R/W)                       */
    __IO uint32_t AESINLL;                   /*!< Offset: 0x00C AES输入待加密/解秘数据低位寄存器 (R/W)    */  
    __IO uint32_t AESINML;                   /*!< Offset: 0x010 AES输入待加密/解秘数据中低位寄存器 (R/W)  */  
    __IO uint32_t AESINHM;                   /*!< Offset: 0x014 AES输入待加密/解秘数据中高位寄存器 (R/W)  */
    __IO uint32_t AESINHH;                   /*!< Offset: 0x018 AES输入待加密/解秘数据高位寄存器 (R/W)    */ 
    __IO uint32_t AESOUTLL;                  /*!< Offset: 0x02C AES输出已加密/解秘数据低位寄存器 (R/W)    */
    __IO uint32_t AESOUTML;                  /*!< Offset: 0x020 AES输出已加密/解秘数据中低位寄存器 (R/W)  */ 
    __IO uint32_t AESOUTHM;                  /*!< Offset: 0x024 AES输出已加密/解秘数据中高位寄存器 (R/W)  */ 
    __IO uint32_t AESOUTHH;                  /*!< Offset: 0x028 AES输出已加密/解秘数据高位寄存器 (R/W)    */
    __IO uint32_t AESKEY0;                   /*!< Offset: 0x02C AES输入密钥寄存器 (R/W)                   */	
    __IO uint32_t AESKEY1;                   /*!< Offset: 0x030 AES输入密钥寄存器 (R/W)                   */
    __IO uint32_t AESKEY2;                   /*!< Offset: 0x034 AES输入密钥寄存器 (R/W)                   */
    __IO uint32_t AESKEY3;                   /*!< Offset: 0x038 AES输入密钥寄存器 (R/W)                   */
    __IO uint32_t AESKEY4;                   /*!< Offset: 0x03C AES输入密钥寄存器 (R/W)                   */
    __IO uint32_t AESKEY5;                   /*!< Offset: 0x040 AES输入密钥寄存器 (R/W)                   */
    __IO uint32_t AESKEY6;                   /*!< Offset: 0x044 AES输入密钥寄存器 (R/W)                   */
    __IO uint32_t AESKEY7;                   /*!< Offset: 0x048 AES输入密钥寄存器 (R/W)                   */
} HT_AES_TypeDef;                            /* end of group HT_AES_TypeDef                               */
#endif


#if defined  HT6x2x  || defined HT502x
typedef struct
{
    __IO uint32_t RANDSTR;                    /*!< Offset: 0x000 真随机数启动命令寄存器 (R/W)             */
    __IO uint32_t RANDDAT;                    /*!< Offset: 0x004 真随机数数据寄存器 (R/W)                 */   
} HT_RAND_TypeDef;                            /* end of group HT_RAND_TypeDef                             */
#endif


#if defined HT501x || defined HT6x2x  || defined HT502x
typedef struct
{
    __O  uint32_t GHASHSTR;                   /*!< Offset: 0x000 GHASH启动命令寄存器 (W)                   */
    __IO uint32_t GHASHFLG;                   /*!< Offset: 0x004 GHASH标志寄存器 (R/W)                     */   
    __IO uint32_t INPUT1LL;                   /*!< Offset: 0x008 GHASH输入数据1低位 (R/W)                  */
    __IO uint32_t INPUT1ML;                   /*!< Offset: 0x00C GHASH输入数据1中低位 (R/W)                */
    __IO uint32_t INPUT1HM;                   /*!< Offset: 0x010 GHASH输入数据1中高位 (R/W)                */		
    __IO uint32_t INPUT1HH;                   /*!< Offset: 0x014 GHASH输入数据1高位 (R/W)                  */
    __IO uint32_t INPUT2LL;                   /*!< Offset: 0x018 GHASH输入数据2低位 (R/W)                  */
    __IO uint32_t INPUT2ML;                   /*!< Offset: 0x01C GHASH输入数据2中低位 (R/W)                */
    __IO uint32_t INPUT2HM;                   /*!< Offset: 0x020 GHASH输入数据2中高位 (R/W)                */
    __IO uint32_t INPUT2HH;                   /*!< Offset: 0x024 GHASH输入数据2高位 (R/W)                  */
    __IO uint32_t OUTPUTLL;                   /*!< Offset: 0x028 GHASH输出数据低位 (R/W)                   */
    __IO uint32_t OUTPUTML;                   /*!< Offset: 0x02C GHASH输出数据中低位 (R/W)                 */
    __IO uint32_t OUTPUTHM;                   /*!< Offset: 0x030 GHASH输出数据中高位 (R/W)                 */
    __IO uint32_t OUTPUTHH;                   /*!< Offset: 0x034 GHASH输出数据高位 (R/W)                   */
    __IO uint32_t AESGHASHIE;                 /*!< Offset: 0x038 AES/GHASH中断使能位 (R/W)                 */
    __IO uint32_t AESGHASHIF;                 /*!< Offset: 0x03C AES/GHASH中断标志位 (R/W)                 */
} HT_GHASH_TypeDef;                           /* end of group HT_GHASH_TypeDef                             */
#endif



/*****************************************  EMU Control Block  *******************************************/
/*
* @brief EMU EPR定义
*/
#if defined  HT501x  ||  defined  HT502x
typedef struct
{										 
  __I uint32_t SPLI1;                       /*!< Offset: 0x000  电流通道1ADC采样数据 (R) */
  __I uint32_t SPLI2;                       /*!< Offset: 0x004  电流通道2ADC采样数据 (R) */
  __I uint32_t SPLU;                        /*!< Offset: 0x008  电压通道ADC采样数据 (R)  */
  __I uint32_t SPLP;                        /*!< Offset: 0x00C  有功功率波形数据 (R)     */
  __I uint32_t SPLQ;                        /*!< Offset: 0x010  无功功率波形数据 (R)     */
  __I uint32_t FASTRMSI1;                   /*!< Offset: 0x014  快速电流通道1的有效值 (R)*/
  __I uint32_t FASTRMSI2;                   /*!< Offset: 0x018  快速电流通道2的有效值 (R)*/
  __I uint32_t FASTRMSU;                    /*!< Offset: 0x01C  快速电压通道的有效值 (R) */
  __I uint32_t FREQU;                       /*!< Offset: 0x020  电压频率 (R)             */
  __I uint32_t FASTPOWERP1;                 /*!< Offset: 0x024  快速第一路有功功率 (R)   */
  __I uint32_t FASTPOWERQ1;                 /*!< Offset: 0x028  快速第一路无功功率 (R)   */
  __I uint32_t FASTPOWERP2;                 /*!< Offset: 0x02C  快速第二路有功功率 (R)   */
  __I uint32_t FASTPOWERQ2;                 /*!< Offset: 0x030  快速第二路无功功率 (R)   */
  __I uint32_t FASTPOWERS1;                 /*!< Offset: 0x034  快速第一路视在功率 (R)   */
  __I uint32_t FASTPOWERS2;                 /*!< Offset: 0x038  快速第二路视在功率 (R)   */
  __I uint32_t RMSI1;                       /*!< Offset: 0x03C  慢速电流通道1的有效值 (R)*/
  __I uint32_t RMSI2;                       /*!< Offset: 0x040  慢速电流通道2的有效值 (R)*/
  __I uint32_t RMSU;                        /*!< Offset: 0x044  慢速电压通道的有效值 (R) */
  __I uint32_t POWERP1;                     /*!< Offset: 0x048  慢速第一路有功功率 (R)   */
  __I uint32_t POWERQ1;                     /*!< Offset: 0x04C  慢速第一路无功功率 (R)   */
  __I uint32_t POWERP2;                     /*!< Offset: 0x050  慢速第二路有功功率 (R)   */
  __I uint32_t POWERQ2;                     /*!< Offset: 0x054  慢速第二路无功功率 (R)   */
  __I uint32_t POWERS1;                     /*!< Offset: 0x058  慢速第一路视在功率 (R)   */
  __I uint32_t POWERS2;                     /*!< Offset: 0x05C  慢速第二路视在功率 (R)   */ 
  __I uint32_t ENERGYP;                     /*!< Offset: 0x060  有功能量 (R)             */
  __I uint32_t ENERGYQ;                     /*!< Offset: 0x064  无功能量 (R)             */
  __I uint32_t ENERGYS;                     /*!< Offset: 0x068  视在能量 (R)             */
  __I uint32_t ENERGYPC;                    /*!< Offset: 0x06C  读后清0型有功能量 (R)    */
  __I uint32_t ENERGYQC;                    /*!< Offset: 0x070  读后清0型无功能量 (R)    */
  __I uint32_t ENERGYSC;                    /*!< Offset: 0x074  读后清0型视在能量 (R)    */
  __I uint32_t DC_UAVERAGE;                 /*!< Offset: 0x078  直流电压平均值 (R)       */
  __I uint32_t DC_I1AVERAGE;                /*!< Offset: 0x07C  直流电流通道1平均值 (R)  */
  __I uint32_t DC_I2AVERAGE;                /*!< Offset: 0x080  直流电流通道2平均值 (R)  */
  __I uint32_t CHECKSUM;                    /*!< Offset: 0x084  校验和寄存器 (R)         */
  __I uint32_t UPEAK;                       /*!< Offset: 0x088  电压波形峰值寄存器 (R)   */
  __I uint32_t I1PEAK;                      /*!< Offset: 0x08C  I1电流波形峰值寄存器 (R) */ 
  __I uint32_t I2PEAK;                      /*!< Offset: 0x090  I2电流波形峰值寄存器 (R) */  
  __I uint32_t PFCNT_PHOTO;                 /*!< Offset: 0x094  PFCNT快照 (R)            */
  __I uint32_t QFCNT_PHOTO;                 /*!< Offset: 0x098  QFCNT快照 (R)            */ 
  __I uint32_t SFCNT_PHOTO;                 /*!< Offset: 0x09C  SFCNT快照 (R)            */    
  __I uint32_t AUTOUGAIN;                   /*!< Offset: 0x0A0  U通道温度自动补偿系数 (R) */
  __I uint32_t AUTOI1GAIN;                  /*!< Offset: 0x0A4  I1通道温度自动补偿系数(R) */ 
  __I uint32_t AUTOI2GAIN;                  /*!< Offset: 0x0A8  I2通道温度自动补偿系数(R) */   
  __I uint32_t R_Buffer;                    /*!< Offset: 0x0AC  buffer读取数据 (R)       */ 
  __I uint32_t Reserved[2]; 
  __I uint32_t UdetCNT;                     /*!< Offset: 0x0B8  SAG/Peak工况持续时间计数寄存器(R) */ 
} HT_EMU_EPR_TypeDef;                       /* end of group HT_EMU_EPR_TypeDef           */
/*
* @brief EMU ECR定义
*/
typedef struct
{										 
  __IO uint32_t EMUSR;                     /*!< Offset: 0x000  EMU状态标志寄存器                 (R/W)              */
  __IO uint32_t EMUIE;                     /*!< Offset: 0x004  EMU中断使能寄存器                 (R/W)              */
  __IO uint32_t EMUIF;                     /*!< Offset: 0x008  EMU中断标志寄存器                 (R/W)              */
  __IO uint32_t GP1;                       /*!< Offset: 0x00C  通道1的有功功率校正               (R/W)              */
  __IO uint32_t GQ1;                       /*!< Offset: 0x010  通道1的无功功率校正               (R/W)              */
  __IO uint32_t GS1;                       /*!< Offset: 0x014  通道1的视在功率校正               (R/W)              */
  __IO uint32_t GPHS1;                     /*!< Offset: 0x018  通道1的相位校正                   (R/W)              */
  __IO uint32_t GP2;                       /*!< Offset: 0x01C  通道2的有功功率校正               (R/W)              */
  __IO uint32_t GQ2;                       /*!< Offset: 0x020  通道2的无功功率校正               (R/W)              */
  __IO uint32_t GS2;                       /*!< Offset: 0x024  通道2的视在功率校正               (R/W)              */
  __IO uint32_t GPHS2;                     /*!< Offset: 0x028  通道2的相位校正                   (R/W)              */
  __IO uint32_t QPHSCAL;                   /*!< Offset: 0x02C  无功相位补偿                      (R/W)              */
  __IO uint32_t I2GAIN;                    /*!< Offset: 0x030  电流通道2增益补偿                 (R/W)              */
  __IO uint32_t I1OFF;                     /*!< Offset: 0x034  电流通道1的偏置校正               (R/W)              */
  __IO uint32_t I2OFF;                     /*!< Offset: 0x038  电流通道2的偏置校正               (R/W)              */
  __IO uint32_t UOFF;                      /*!< Offset: 0x03C  电压通道的偏置校正                (R/W)              */
  __IO uint32_t PSTART;                    /*!< Offset: 0x040  起动功率设置                      (R/W)              */
  __IO uint32_t QSTART;                    /*!< Offset: 0x044  起动功率设置                      (R/W)              */
  __IO uint32_t SSTART;                    /*!< Offset: 0x048  起动功率设置                      (R/W)              */
  __IO uint32_t HFCONST;                   /*!< Offset: 0x04C  输出脉冲频率设置                  (R/W)              */
  __IO uint32_t ADCCFG;                    /*!< Offset: 0x050  ADC控制寄存器                     (R/W)              */
  __IO uint32_t CHNLCR;                    /*!< Offset: 0x054  通道控制寄存器                    (R/W)              */
  __IO uint32_t EMCON;                     /*!< Offset: 0x058  能量计量控制寄存器                (R/W)              */
  __IO uint32_t PFCNT;                     /*!< Offset: 0x05C  快速有功脉冲计数                  (R/W)              */
  __IO uint32_t QFCNT;                     /*!< Offset: 0x060  快速无功脉冲计数                  (R/W)              */   
  __IO uint32_t SFCNT;                     /*!< Offset: 0x064  快速视在脉冲计数                  (R/W)              */
  __IO uint32_t ADCCON;                    /*!< Offset: 0x068  ADC通道增益选择                   (R/W)              */
  __IO uint32_t IPTAMP;                    /*!< Offset: 0x06C  窃电检测域值                      (R/W)              */
  __IO uint32_t ICHK;                      /*!< Offset: 0x070  窃电阈值设置                      (R/W)              */
  __IO uint32_t EMUCTRL;                   /*!< Offset: 0x074  EMU控制寄存器                     (R/W)              */
  __IO uint32_t P1OFFSET;                  /*!< Offset: 0x078  通道1有功功率小信号偏置校正        (R/W)              */
  __IO uint32_t P2OFFSET;                  /*!< Offset: 0x07C  通道2有功功率小信号偏置校正        (R/W)              */
  __IO uint32_t Q1OFFSET;                  /*!< Offset: 0x080  通道1无功功率小信号偏置校正        (R/W)              */
  __IO uint32_t Q2OFFSET;                  /*!< Offset: 0x084  通道2无功功率小信号偏置校正        (R/W)              */
  __IO uint32_t I1RMSOFFSET;               /*!< Offset: 0x088  电流通道1有效值小信号校正寄存器    (R/W)              */
  __IO uint32_t I2RMSOFFSET;               /*!< Offset: 0x08C  电流通道2有效值小信号校正寄存器    (R/W)              */
  __IO uint32_t URMSOFFSET;                /*!< Offset: 0x090  电压通道有效值小信号校正寄存器     (R/W)              */
  __IO uint32_t ROSICTRL;                  /*!< Offset: 0x094  罗氏线圈使能控制位                (R/W)              */
  __IO uint32_t ANACTRL;                   /*!< Offset: 0x098  模拟控制寄存器                    (R/W)              */
  __IO uint32_t UCONST;                    /*!< Offset: 0x09C  失压情况下参与计量的电压，断相仿窃电         (R/W)     */
  __IO uint32_t LPIDELTIME;                /*!< Offset: 0x0A0  低功耗计量模式下，分时模式空闲时间设置寄存器  (R/W)    */
#if defined HT502x
  __IO uint32_t USAGLVL;                   /*!< Offset: 0x0A4  电压跌落检测阈值寄存器                      (R/W)    */
  __IO uint32_t IPEAKCYC;                  /*!< Offset: 0x0A8  电流跌落检测ADC半周波数设置寄存器            (R/W)    */
  __IO uint32_t UOVLVL;                    /*!< Offset: 0x0AC  电压峰值检测阈值寄存器                      (R/W)    */
	__IO uint32_t UdetCyc;                   /*!< Offset: 0x0B0  电压Peak Sag检测周期                        (R/W)   */
#else
  __IO uint32_t USAGLVL;                   /*!< Offset: 0x0A4  电压跌落检测阈值寄存器                      (R/W)    */
  __IO uint32_t USAGCYC;                   /*!< Offset: 0x0A8  电压跌落检测ADC半周波数设置寄存器            (R/W)    */
  __IO uint32_t UOVLVL;                    /*!< Offset: 0x0AC  电压峰值检测阈值寄存器                      (R/W)    */
  __IO uint32_t OVCYC;                     /*!< Offset: 0x0B0  峰值检测ADC半周波数设置寄存器                (R/W)   */
#endif
  __IO uint32_t IOVLVL;                    /*!< Offset: 0x0B4  电流峰值阈值寄存器                           (R/W)   */
  __IO uint32_t ZXILVL;                    /*!< Offset: 0x0B8  电流过零阈值寄存器                           (R/W)   */
  __IO uint32_t PDATCPH;                   /*!< Offset: 0x0BC  常数有功功率寄存器高16bit                    (R/W)   */
  __IO uint32_t PDATCPL;                   /*!< Offset: 0x0C0  常数有功功率寄存器低16bit                    (R/W)   */
  __IO uint32_t QDATCPH;                   /*!< Offset: 0x0C4  常数无功功率寄存器高16bit                    (R/W)   */
  __IO uint32_t QDATCPL;                   /*!< Offset: 0x0C8  常数无功功率寄存器低16bit                    (R/W)   */
  __IO uint32_t SDATCPH;                   /*!< Offset: 0x0CC  常数视在功率寄存器高16bit                    (R/W)   */
  __IO uint32_t SDATCPL;                   /*!< Offset: 0x0D0  常数功率寄存器低16bit                        (R/W)   */
  __IO uint32_t FILTERCTRL;                /*!< Offset: 0x0D4  高通低通等滤波器系数选择控制                 (R/W)   */ 
  __IO uint32_t TUGAIN;                    /*!< Offset: 0x0D8  电压通道手动温度补偿系数                     (R/W)   */   
  __IO uint32_t TI1GAIN;                   /*!< Offset: 0x0DC  电流I1通道手动温度补偿系数                   (R/W)   */
  __IO uint32_t TI2GAIN;                   /*!< Offset: 0x0E0  电流I2通道手动温度补偿系数                   (R/W)   */
  __IO uint32_t UTCCOFFA;                  /*!< Offset: 0x0E4  Vrefgain的二次系数                           (R/W)   */   
  __IO uint32_t UTCCOFFB;                  /*!< Offset: 0x0E8  Vrefgain的一次系数                           (R/W)   */
  __IO uint32_t UTCCOFFC;                  /*!< Offset: 0x0EC  Vrefgain的常数项                             (R/W)   */
  __IO uint32_t I1TCCOFFA;                 /*!< Offset: 0x0F0  Vrefgain的二次系数                           (R/W)   */   
  __IO uint32_t I1TCCOFFB;                 /*!< Offset: 0x0F4  Vrefgain的一次系数                           (R/W)   */
  __IO uint32_t I1TCCOFFC;                 /*!< Offset: 0x0F8  Vrefgain的常数项                             (R/W)   */
  __IO uint32_t I2TCCOFFA;                 /*!< Offset: 0x0FC  Vrefgain的二次系数                           (R/W)   */   
  __IO uint32_t I2TCCOFFB;                 /*!< Offset: 0x100  Vrefgain的一次系数                           (R/W)   */
  __IO uint32_t I2TCCOFFC;                 /*!< Offset: 0x104  Vrefgain的常数项                             (R/W)   */
  __IO uint32_t LOADDATACP;                /*!< Offset: 0x108  当用户向该寄存器写0x00BC时                    (R/W)   */    
#if defined HT502x
  __IO uint32_t BufferStart;               /*!< Offset: 0x10C  缓存数据启动（不参与校验和）                   (R/W)   */
	__IO uint32_t BufferCoff;                /*!< Offset: 0x110  缓存数据手动调整系数（不参与校验和）           (R/W)   */
#else
  __I  uint32_t Reserved[2];               /*!<                常数计量DATACP load进入累加源                        */                                 
#endif
  __IO uint32_t SRSTREG; 			   	   /*!< Offset: 0x114  软件复位寄存器,写0x55用于复位校表寄存器  (R/W)       */    								  	
#if defined  HT502x
  __IO uint32_t PFCNTN;                    /*!< Offset: 0x118  反向快速有功脉冲计数              (R/W)              */
  __IO uint32_t QFCNTN;                    /*!< Offset: 0x11C  反向快速无功脉冲计数              (R/W)              */   
#endif
} HT_EMU_ECR_TypeDef;                      /* end of group HT_EMU_ECR_TypeDef                                       */
#endif


/*****************************************  KEY Control Block  *******************************************/
#if defined  HT501x  ||  defined HT502x
typedef struct
{
    __IO uint32_t KEYSTA;                    /*!< Offset: 0x000 KEYSCAN状态寄存器 (R/W)       */
    __IO uint32_t KEYIF;                     /*!< Offset: 0x004 按键中断标志寄存器(R/W)       */   
} HT_KEY_TypeDef;                            /* end of group HT_KEY_TypeDef                   */
#endif


/*****************************************  ECC Control Block  *******************************************/
#if defined  HT502x
typedef struct
{
    __IO uint32_t ECCCON;                    /*!< Offset: 0x000 ECC模块控制寄存器      (R/W)  */
    __IO uint32_t ECCSTA;                    /*!< Offset: 0x004 ECC模块状态寄存器      (R/W)  */
	  __IO uint32_t PXREG;                     /*!< Offset: 0x008 基点X坐标寄存器        (R/W)  */
	  __IO uint32_t PYREG;                     /*!< Offset: 0x00C 基点Y坐标寄存器        (R/W)  */
	  __IO uint32_t KEYREG;                    /*!< Offset: 0x010 私钥寄存器             (R/W)  */
	  __IO uint32_t AREG;                      /*!< Offset: 0x014 椭圆曲线参数a          (R/W)  */
	  __IO uint32_t PREG;                      /*!< Offset: 0x018 大素数p                (R/W)  */
	  __IO uint32_t RXREG;                     /*!< Offset: 0x01C ECC运算输出点X坐标     (R/W)  */
	  __IO uint32_t RYREG;                     /*!< Offset: 0x020 ECC运算输出点Y坐标     (R/W)  */
	  __IO uint32_t SXREG;                     /*!< Offset: 0x024 ECC运算第二操作数X坐标  (R/W)  */
	  __IO uint32_t SYREG;                     /*!< Offset: 0x028 ECC运算第二操作数Y坐标  (R/W)  */
	  __IO uint32_t MREG;                      /*!< Offset: 0x02C Hash算法消息摘要        (R/W)  */
} HT_ECC_TypeDef;                            /* end of group HT_ECC_TypeDef                   */
#endif


/*****************************************  Info Control Block  ******************************************/
/*
* @brief  RTC系数在Info中定义
*/
typedef union
{
    struct
    {
        __I  uint32_t iDFAH;
        __I  uint32_t iDFAL;
        __I  uint32_t iDFBH;
        __I  uint32_t iDFBL;
        __I  uint32_t iDFCH;
        __I  uint32_t iDFCL;
        __I  uint32_t iDFDH;
        __I  uint32_t iDFDL;
        __I  uint32_t iDFEH;
        __I  uint32_t iDFEL;
        __I  uint32_t iToff;
        __I  uint32_t iMCON01;
        __I  uint32_t iMCON23;
        __I  uint32_t iMCON45;
        __I  uint32_t iChkSum;               /* iChkSum = iDFAH + iDFAL +...+iMCON45   */
    }Muster;
    __I  uint32_t DataArry[15];
} HT_INFO_TypeDef;                           /* end of group HT_INFO_TypeDef           */


/*
**********************************************************************************************************
*                                        Peripheral memory map  
*                                           Unit of Byte
**********************************************************************************************************
*/  

/*  Base Address  */
#define HT_FLASH_BASE        (0x00000000UL)
#define HT_RAM_BASE          (0x20000000UL)
#define HT_APB0_BASE         (0x40000000UL)


#if defined HT6x1x
#define HT_FLASH_SIZE        (0x00020000UL)
#define HT_RAM_SIZE          (0x00002000UL)
#elif defined HT6x2x
#define HT_FLASH_SIZE        (0x00040000UL)
#define HT_RAM_SIZE          (0x00008000UL)
#elif defined HT501x
#define HT_FLASH_SIZE        (0x00020000UL)
#define HT_RAM_SIZE          (0x00002000UL)
#elif defined HT502x
#define HT_FLASH_SIZE        (0x00040000UL)
#define HT_RAM_SIZE          (0x00008000UL)
#endif
#define HT_APB0_SIZE         (0x00080000UL)


#define HT_INFO_BASE         (HT_FLASH_BASE + 0x00040000)
#if defined HT6x1x
#define HT_INFO_SIZE         (0x00000200UL)
#define HT_INFO_BANK         (0x00000100UL)
#elif defined HT6x2x
#define HT_INFO_SIZE         (0x00000400UL)
#define HT_INFO_BANK         (0x00000400UL)
#elif defined HT501x
#define HT_INFO_SIZE         (0x00000200UL)
#define HT_INFO_BANK         (0x00000100UL)
#elif defined HT502x
#define HT_INFO_SIZE         (0x00000400UL)
#define HT_INFO_BANK         (0x00000400UL)
#endif


/*  APB0 Peripherals Address  */
#define HT_TMR0_BASE         (HT_APB0_BASE + 0x01000)
#define HT_TMR1_BASE         (HT_APB0_BASE + 0x02000)
#define HT_TMR2_BASE         (HT_APB0_BASE + 0x03000)
#define HT_TMR3_BASE         (HT_APB0_BASE + 0x04000)
#if defined  HT6x2x
#define HT_TMR4_BASE         (HT_APB0_BASE + 0x19000)
#define HT_TMR5_BASE         (HT_APB0_BASE + 0x1A000)
#endif


#define HT_UART0_BASE        (HT_APB0_BASE + 0x05000)
#define HT_UART1_BASE        (HT_APB0_BASE + 0x06000)
#define HT_UART2_BASE        (HT_APB0_BASE + 0x07000)
#define HT_UART3_BASE        (HT_APB0_BASE + 0x08000)
#define HT_UART4_BASE        (HT_APB0_BASE + 0x09000)
#define HT_UART5_BASE        (HT_APB0_BASE + 0x00000)
#if defined  HT6x2x
#define HT_UART6_BASE        (HT_APB0_BASE + 0x18000)
#endif


#define HT_I2C_BASE          (HT_APB0_BASE + 0x0A000)
#define HT_SPI0_BASE         (HT_APB0_BASE + 0x0B000)
#if defined  HT6x2x
#define HT_SPI1_BASE         (HT_APB0_BASE + 0x17000)
#endif


#define HT_RTC_BASE          (HT_APB0_BASE + 0x0C000)
#define HT_LCD_BASE          (HT_APB0_BASE + 0x0D000)
#define HT_TBS_BASE          (HT_APB0_BASE + 0x0E000)
#define HT_PMU_BASE          (HT_APB0_BASE + 0x0F400)
#define HT_CMU_BASE          (HT_APB0_BASE + 0x0F000)
#define HT_WDT_BASE          (HT_APB0_BASE + 0x10000)
#define HT_GPIO_BASE         (HT_APB0_BASE + 0x11000)
#define HT_INT_BASE          (HT_APB0_BASE + 0x11800)


/*  GPIO Address  */    
#define HT_GPIOA_BASE        (HT_GPIO_BASE  + 0x00000)
#define HT_GPIOB_BASE        (HT_GPIO_BASE  + 0x00100)
#define HT_GPIOC_BASE        (HT_GPIO_BASE  + 0x00200)
#define HT_GPIOD_BASE        (HT_GPIO_BASE  + 0x00300)
#define HT_GPIOE_BASE        (HT_GPIO_BASE  + 0x00400)
#if defined  HT501x  ||  defined  HT502x
#define HT_GPIOF_BASE        (HT_GPIO_BASE  + 0x00600)
#elif defined  HT6x2x
#define HT_GPIOG_BASE        (HT_GPIO_BASE  + 0x00600)
#define HT_GPIOH_BASE        (HT_GPIO_BASE  + 0x00700)
#endif


#if defined  HT501x
#define HT_AES_BASE          (HT_APB0_BASE  + 0x12000)
#define HT_GHASH_BASE        (HT_APB0_BASE  + 0x12100)
#define HT_EMU_EPA_BASE      (HT_APB0_BASE  + 0x13000)
#define HT_EMU_ECA_BASE      (HT_APB0_BASE  + 0x13800)
#define HT_KEY_BASE          (HT_APB0_BASE  + 0x14000)

#define HT_DMA_BASE          (HT_APB0_BASE  + 0x16000)
#define HT_DMA_Channel0_BASE (HT_APB0_BASE  + 0x1600C)
#define HT_DMA_Channel1_BASE (HT_APB0_BASE  + 0x16024)
#define HT_DMA_Channel2_BASE (HT_APB0_BASE  + 0x1603C)

#elif defined  HT6x2x
#define HT_CRC_BASE          (HT_APB0_BASE  + 0x21000)
#define HT_AES_BASE          (HT_APB0_BASE  + 0x12000)
#define HT_RAND_BASE         (HT_APB0_BASE  + 0x12080)
#define HT_GHASH_BASE        (HT_APB0_BASE  + 0x12100)

#define HT_DMA_BASE          (HT_APB0_BASE  + 0x20000)
#define HT_DMA_Channel0_BASE (HT_APB0_BASE  + 0x2000C)
#define HT_DMA_Channel1_BASE (HT_APB0_BASE  + 0x20024)
#define HT_DMA_Channel2_BASE (HT_APB0_BASE  + 0x2003C)

#elif defined  HT502x
#define HT_AES_BASE          (HT_APB0_BASE  + 0x12000)
#define HT_RAND_BASE         (HT_APB0_BASE  + 0x12080)
#define HT_GHASH_BASE        (HT_APB0_BASE  + 0x12100)
#define HT_EMU_EPA_BASE      (HT_APB0_BASE  + 0x13000)
#define HT_EMU_ECA_BASE      (HT_APB0_BASE  + 0x13800)
#define HT_KEY_BASE          (HT_APB0_BASE  + 0x14000)

#define HT_DMA_BASE          (HT_APB0_BASE  + 0x16000)
#define HT_DMA_Channel0_BASE (HT_APB0_BASE  + 0x1600C)
#define HT_DMA_Channel1_BASE (HT_APB0_BASE  + 0x16024)
#define HT_DMA_Channel2_BASE (HT_APB0_BASE  + 0x1603C)

#define HT_ECC_BASE          (HT_APB0_BASE  + 0x17000)
#endif

/*
**********************************************************************************************************
*                                        Peripheral declaration  
**********************************************************************************************************
*/  
#define HT_TMR0              ((HT_TMR_TypeDef    *) HT_TMR0_BASE )
#define HT_TMR1              ((HT_TMR_TypeDef    *) HT_TMR1_BASE )
#define HT_TMR2              ((HT_TMR_TypeDef    *) HT_TMR2_BASE )
#define HT_TMR3              ((HT_TMR_TypeDef    *) HT_TMR3_BASE )
#if defined  HT6x2x
#define HT_TMR4              ((HT_TMR_TypeDef    *) HT_TMR4_BASE )
#define HT_TMR5              ((HT_TMR_TypeDef    *) HT_TMR5_BASE )
#endif

#define HT_UART0             ((HT_UART_TypeDef   *) HT_UART0_BASE)
#define HT_UART1             ((HT_UART_TypeDef   *) HT_UART1_BASE)
#define HT_UART2             ((HT_UART_TypeDef   *) HT_UART2_BASE)
#define HT_UART3             ((HT_UART_TypeDef   *) HT_UART3_BASE)
#define HT_UART4             ((HT_UART_TypeDef   *) HT_UART4_BASE)
#define HT_UART5             ((HT_UART_TypeDef   *) HT_UART5_BASE)
#if defined  HT6x2x
#define HT_UART6             ((HT_UART_TypeDef   *) HT_UART6_BASE)
#endif

#define HT_ISO7816_0         ((HT_ISO7816_TypeDef*) HT_UART4_BASE)
#define HT_ISO7816_1         ((HT_ISO7816_TypeDef*) HT_UART3_BASE)


#define HT_I2C               ((HT_I2C_TypeDef    *) HT_I2C_BASE  )
#define HT_SPI0              ((HT_SPI_TypeDef    *) HT_SPI0_BASE )
#if defined  HT6x2x
#define HT_SPI1              ((HT_SPI_TypeDef    *) HT_SPI1_BASE )
#endif

#define HT_RTC               ((HT_RTC_TypeDef    *) HT_RTC_BASE  )
#define HT_LCD               ((HT_LCD_TypeDef    *) HT_LCD_BASE  )
#define HT_TBS               ((HT_TBS_TypeDef    *) HT_TBS_BASE  )
#define HT_PMU               ((HT_PMU_TypeDef    *) HT_PMU_BASE  )
#define HT_CMU               ((HT_CMU_TypeDef    *) HT_CMU_BASE  )
#define HT_WDT               ((HT_WDT_TypeDef    *) HT_WDT_BASE  )
#define HT_INT               ((HT_INT_TypeDef    *) HT_INT_BASE  )


#define HT_GPIOA             ((HT_GPIO_TypeDef   *) HT_GPIOA_BASE)
#define HT_GPIOB             ((HT_GPIO_TypeDef   *) HT_GPIOB_BASE)
#define HT_GPIOC             ((HT_GPIO_TypeDef   *) HT_GPIOC_BASE)
#define HT_GPIOD             ((HT_GPIO_TypeDef   *) HT_GPIOD_BASE)
#define HT_GPIOE             ((HT_GPIO_TypeDef   *) HT_GPIOE_BASE)
#define HT_GPIOHDPORT        (*((uint32_t        *) 0x40011500)  )            /*!< 大电流端口设置       */
#if defined  HT501x  ||  defined HT502x
#define HT_GPIOF             ((HT_GPIO_TypeDef   *) HT_GPIOF_BASE)
#elif defined  HT6x2x
#define HT_GPIOG             ((HT_GPIO_TypeDef   *) HT_GPIOG_BASE)
#define HT_GPIOH             ((HT_GPIO_TypeDef   *) HT_GPIOH_BASE)
#endif


#if defined  HT501x  ||  defined HT502x
#define HT_EMUEPA            ((HT_EMU_EPR_TypeDef*) HT_EMU_EPA_BASE)	
#define HT_EMUECA            ((HT_EMU_ECR_TypeDef*) HT_EMU_ECA_BASE)	
#define HT_KEY               ((HT_KEY_TypeDef    *) HT_KEY_BASE  )	
#elif defined  HT6x2x
#define HT_CRC               ((HT_CRC_TypeDef    *) HT_CRC_BASE  )
#elif defined  HT6x2x  ||  defined HT502x
#define HT_RAND              ((HT_RAND_TypeDef   *) HT_RAND_BASE )
#endif

#if defined HT501x  || defined HT6x2x  ||  defined HT502x
#define HT_AES               ((HT_AES_TypeDef    *) HT_AES_BASE  )
#define HT_GHASH             ((HT_GHASH_TypeDef  *) HT_GHASH_BASE)

#define HT_DMA               ((HT_DMA_TypeDef    *) HT_DMA_BASE  )
#define HT_DMA_Channel0      ((HT_DMA_Channel_TypeDef*) HT_DMA_Channel0_BASE  )
#define HT_DMA_Channel1      ((HT_DMA_Channel_TypeDef*) HT_DMA_Channel1_BASE  )
#define HT_DMA_Channel2      ((HT_DMA_Channel_TypeDef*) HT_DMA_Channel2_BASE  )
#endif

#if defined HT502x
#define HT_ECC               ((HT_ECC_TypeDef    *) HT_ECC_BASE  )
#endif

#define HT_INFO              ((HT_INFO_TypeDef   *) (HT_INFO_BASE + 0x104) )    /*!< RTC系数存储在Info中  */        
/*
**********************************************************************************************************
*                               Bits Definition For HT6x1x HT6x2x HT501x
**********************************************************************************************************
*/ 


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of Timer Module  
**********************************************************************************************************
*/  
/************************  Bit definition for TMRCON register of HT_TMR_TypeDef  ************************/
#if defined HT6x2x  
#define  TMR_TMRCON                        		 ((uint32_t)0x07ff)           /*!<  定时器控制寄存器	  */
#else
#define  TMR_TMRCON                        		 ((uint32_t)0x00ff)           /*!<  定时器控制寄存器	  */
#endif
#define  TMR_TMRCON_CNTEN                        ((uint32_t)0x0001)           /*!<  定时器计数功能使能  */

#define  TMR_TMRCON_MODE                         ((uint32_t)0x0006)           /*!<  定时器工作模式      */
#if defined HT6x1x  ||  defined HT501x  ||  defined HT502x                                                                            
#define  TMR_TMRCON_MODE_NONE                    ((uint32_t)0x0000)           /*!<Timer mode none       */
#define  TMR_TMRCON_MODE_PWM                     ((uint32_t)0x0002)           /*!<Timer mode pwm        */
#define  TMR_TMRCON_MODE_CAPTURE                 ((uint32_t)0x0004)           /*!<Timer mode capture    */
#define  TMR_TMRCON_MODE_COMPARE                 ((uint32_t)0x0006)           /*!<Timer mode compare    */
#elif defined  HT6x2x                                                         
#define  TMR_TMRCON_MODE_RECORD                  ((uint32_t)0x0000)           /*!<Timer mode EventRecord*/
#define  TMR_TMRCON_MODE_PWM                     ((uint32_t)0x0002)           /*!<Timer mode pwm        */
#define  TMR_TMRCON_MODE_CAPTURE                 ((uint32_t)0x0004)           /*!<Timer mode capture    */
#define  TMR_TMRCON_MODE_TIMING                  ((uint32_t)0x0006)           /*!<Timer mode timing only*/
#endif

#define  TMR_TMRCON_CCMODE                       ((uint32_t)0x0008)           /*!<捕获/事件电平沿选择   */
#define  TMR_TMRCON_CCMODE_RISE                  ((uint32_t)0x0000)           /*!<捕获/事件电平上升沿   */
#define  TMR_TMRCON_CCMODE_FALL                  ((uint32_t)0x0008)           /*!<捕获/事件电平下降沿   */

#define  TMR_TMRCON_PWMC                         ((uint32_t)0x0030)           /*!<PWM计数方式           */
#define  TMR_TMRCON_PWMC_UP                      ((uint32_t)0x0000)           /*!<PWM向上计数           */
#define  TMR_TMRCON_PWMC_DOWM                    ((uint32_t)0x0010)           /*!<PWM向下计数           */
#define  TMR_TMRCON_PWMC_CENTER                  ((uint32_t)0x0020)           /*!<PWM中央对齐方式       */

#define  TMR_TMRCON_PWMHL                        ((uint32_t)0x0040)           /*!<PWM初始电平选择       */
#define  TMR_TMRCON_PWMHL_HIGH                   ((uint32_t)0x0000)           /*!<PWM初始电平高         */
#define  TMR_TMRCON_PWMHL_LOW                    ((uint32_t)0x0040)           /*!<PWM初始电平低         */

#if defined  HT6x1x  ||  defined HT501x                                                          
#define  TMR_TMRCON_CMPMODE                      ((uint32_t)0x0080)           /*!<输出比较模式选择      */
#define  TMR_TMRCON_CMPMODE_0                    ((uint32_t)0x0000)           /*!<输出比较模式0         */
#define  TMR_TMRCON_CMPMODE_1                    ((uint32_t)0x0080)           /*!<输出比较模式1         */
#endif

#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TMR_TMRCON_CLKSEL                       ((uint32_t)0x0700)           /*!<TMR4&5时钟源选择      */
#define  TMR_TMRCON_CLKSEL_LRC                   ((uint32_t)0x0000)           /*!<选择LRC               */
#define  TMR_TMRCON_CLKSEL_LF                    ((uint32_t)0x0100)           /*!<选择LF                */
#define  TMR_TMRCON_CLKSEL_HRC                   ((uint32_t)0x0200)           /*!<选择HRC               */
#define  TMR_TMRCON_CLKSEL_PLL                   ((uint32_t)0x0300)           /*!<选择PLL               */
#define  TMR_TMRCON_CLKSEL_MEMS                  ((uint32_t)0x0400)           /*!<选择MEMS              */
#endif                                                                        /*!<Only for timer4&5     */

/************************  Bit definition for TMRDIV register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRDIV                              ((uint32_t)0xffff)           /*!<通道预分频寄存器      */

/************************  Bit definition for TMRPRD register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRPRD                              ((uint32_t)0xffff)           /*!<通道周期寄存器        */

/************************  Bit definition for TMRCAP register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRCAP                              ((uint32_t)0xffff)           /*!<通道捕获数据寄存器    */

/************************  Bit definition for TMRCNT register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRCNT                              ((uint32_t)0xffff)           /*!<通道计数器寄存器      */

/************************  Bit definition for TMRCMP register of HT_TMR_TypeDef  ************************/
#define  TMR_TMRCMP                              ((uint32_t)0xffff)           /*!<通道比较器寄存器      */

/************************  Bit definition for TMRIE register of HT_TMR_TypeDef   ************************/
#if  defined  HT6x2x
#define  TMR_TMRIE		                         ((uint32_t)0x000f)           /*!<TMR中断使能          */
#else
#define  TMR_TMRIE		                         ((uint32_t)0x0007)           /*!<TMR中断使能          */
#endif
#define  TMR_TMRIE_PRDIE                         ((uint32_t)0x0001)           /*!<周期中断使能          */
#define  TMR_TMRIE_CAPIE                         ((uint32_t)0x0002)           /*!<捕获中断使能          */
#define  TMR_TMRIE_CMPIE                         ((uint32_t)0x0004)           /*!<比较中断使能          */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TMR_TMRIE_ACIE                          ((uint32_t)0x0008)           /*!<事件计数中断使能      */
#endif

/************************  Bit definition for TMRIF register of HT_TMR_TypeDef   ************************/
#if  defined  HT6x2x
#define  TMR_TMRIF		                         ((uint32_t)0x000f)           /*!<TMR中断使能          */
#else
#define  TMR_TMRIF		                         ((uint32_t)0x0007)           /*!<TMR中断使能          */
#endif
#define  TMR_TMRIF_PRDIF                         ((uint32_t)0x0001)           /*!<周期中断标志          */
#define  TMR_TMRIF_CAPIF                         ((uint32_t)0x0002)           /*!<捕获中断标志          */
#define  TMR_TMRIF_CMPIF                         ((uint32_t)0x0004)           /*!<比较中断标志          */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TMR_TMRIF_ACIF                          ((uint32_t)0x0008)           /*!<事件计数中断标志      */
#endif

/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of UART Module  
**********************************************************************************************************
*/  
/************************  Bit definition for MODESEL register of HT_UART_TypeDef ***********************/
#define  UART_MODESEL                            ((uint32_t)0x0001)           /*!<串口功能选择控制位    */
#define  UART_MODESEL_UART                       ((uint32_t)0x0000)           /*!<选择Uart功能          */
#define  UART_MODESEL_7816                       ((uint32_t)0x0001)           /*!<选择7816功能          */
                                                                              /*!<Uart3&4 only          */
/************************  Bit definition for UARTCON register of HT_UART_TypeDef ***********************/                                                                            
#if  defined  HT6x2x
#define  UART_UARTCON		                     ((uint32_t)0x87ff)           /*!<Uart控制寄存器         */
#elif defined  HT502x
#define  UART_UARTCON		                     ((uint32_t)0x07ff)           /*!<Uart控制寄存器         */
#else
#define  UART_UARTCON		                     ((uint32_t)0x03ff)           /*!<Uart控制寄存器         */
#endif
#define  UART_UARTCON_TXEN                       ((uint32_t)0x0001)           /*!<Uart发送使能          */
#define  UART_UARTCON_RXEN                       ((uint32_t)0x0002)           /*!<Uart接收使能          */
#define  UART_UARTCON_TXIE                       ((uint32_t)0x0004)           /*!<Uart发送中断使能      */
#define  UART_UARTCON_RXIE                       ((uint32_t)0x0008)           /*!<Uart接收中断使能      */
#define  UART_UARTCON_PARITYEN                   ((uint32_t)0x0010)           /*!<Uart奇偶校验使能      */

#define  UART_UARTCON_PARITYSEL                  ((uint32_t)0x0060)           /*!<Uart奇偶校验选择      */
#define  UART_UARTCON_PARITYSEL_1                ((uint32_t)0x0060)           /*!<Uart奇偶校验恒为1     */
#define  UART_UARTCON_PARITYSEL_0                ((uint32_t)0x0000)           /*!<Uart奇偶校验恒为0     */
#define  UART_UARTCON_PARITYSEL_ODD              ((uint32_t)0x0020)           /*!<Uart奇偶校验为奇      */
#define  UART_UARTCON_PARITYSEL_EVEN             ((uint32_t)0x0040)           /*!<Uart奇偶校验为偶      */

#define  UART_UARTCON_LENSEL                     ((uint32_t)0x0080)           /*!<Uart长度选择位        */
#define  UART_UARTCON_LENSEL_7Bit                ((uint32_t)0x0080)           /*!<Uart通讯数据长度为7位 */
#define  UART_UARTCON_LENSEL_8Bit                ((uint32_t)0x0000)           /*!<Uart通讯数据长度为8位 */

#define  UART_UARTCON_STOPSEL                    ((uint32_t)0x0100)           /*!<Uart长度选择位        */
#define  UART_UARTCON_STOPSEL_2Bit               ((uint32_t)0x0100)           /*!<Uart通讯停止位为2位   */
#define  UART_UARTCON_STOPSEL_1Bit               ((uint32_t)0x0000)           /*!<Uart通讯停止位为1位   */

#define  UART_UARTCON_UNEG                       ((uint32_t)0x0200)           /*!<Uart通讯逻辑选择位    */
#define  UART_UARTCON_UNEG_NEGETIVE              ((uint32_t)0x0200)           /*!<Uart通讯为负逻辑      */
#define  UART_UARTCON_UNEG_POSITIVE              ((uint32_t)0x0000)           /*!<Uart通讯为正逻辑      */

#if  defined  HT6x2x                                                          
#define  UART_UARTCON_UNEG_AUTO                  ((uint32_t)0x0400)           /*!<Uart通讯逻辑判断控制  */ 
#define  UART_UARTCON_PRDIE                      ((uint32_t)0x8000)           /*!<Uart上溢中断使能      */
#endif

#if  defined  HT502x
#define  UART_UARTCON_NEGFIX                     ((uint32_t)0x0400)           /*!<Uart通讯逻辑锁定位    */
#define  UART_UARTCON_NEGFIX_BYHARDWARE          ((uint32_t)0x0400)           /*!<Uart通讯逻辑由硬件配置 */
#define  UART_UARTCON_NEGFIX_BYSOFTWARE          ((uint32_t)0x0000)           /*!<Uart通讯逻辑由软件配置 */
#endif

/**********************  Bit definition for ISO7816CON register of HT_UART_TypeDef **********************/
#define  UART_ISO7816CON	                     ((uint32_t)0x03ff)           /*!<ISO7816控制寄存器     */

#define  UART_ISO7816CON_IEALL                   ((uint32_t)0x0007)           /*!<ISO7816中断使能位     */
#define  UART_ISO7816CON_TXIE                    ((uint32_t)0x0001)           /*!<发送中断使能位        */
#define  UART_ISO7816CON_RXIE                    ((uint32_t)0x0002)           /*!<接收中断使能位        */
#define  UART_ISO7816CON_PRDIE                   ((uint32_t)0x0004)           /*!<上溢中断使能位        */

#define  UART_ISO7816CON_ACKLEN                  ((uint32_t)0x0018)           /*!<ACK响应长度控制位     */
#define  UART_ISO7816CON_ACKLEN_1Bit             ((uint32_t)0x0000)           /*!<ACK响应长度1Bit       */
#define  UART_ISO7816CON_ACKLEN_1P5Bit           ((uint32_t)0x0008)           /*!<ACK响应长度1.5Bit     */
#define  UART_ISO7816CON_ACKLEN_2Bit             ((uint32_t)0x0010)           /*!<ACK响应长度2Bit       */

#define  UART_ISO7816CON_REPTR                   ((uint32_t)0x0060)           /*!<自动重收重发次数上限  */
#define  UART_ISO7816CON_REPTR_0                 ((uint32_t)0x0000)           /*!<自动重收重发0次       */
#define  UART_ISO7816CON_REPTR_1                 ((uint32_t)0x0020)           /*!<自动重收重发1次       */
#define  UART_ISO7816CON_REPTR_2                 ((uint32_t)0x0040)           /*!<自动重收重发2次       */
#define  UART_ISO7816CON_REPTR_3                 ((uint32_t)0x0060)           /*!<自动重收重发3次       */

#define  UART_ISO7816CON_AUTOTXEN                ((uint32_t)0x0080)           /*!<自动重发使能位        */
#define  UART_ISO7816CON_AUTORXEN                ((uint32_t)0x0100)           /*!<自动重收使能位        */

#define  UART_ISO7816CON_7816PARITY              ((uint32_t)0x0200)           /*!<奇偶校验选择位        */
#define  UART_ISO7816CON_7816PARITY_ODD          ((uint32_t)0x0200)           /*!<奇校验                */
#define  UART_ISO7816CON_7816PARITY_EVEN         ((uint32_t)0x0000)           /*!<偶校验                */
                                                                              /*!<Uart3&4 only          */
/*************************  Bit definition for SREL register of HT_UART_TypeDef *************************/
#define  UART_SREL                               ((uint32_t)0xffff)           /*!<串口波特率发生器      */

/*************************  Bit definition for SBUF register of HT_UART_TypeDef *************************/
#define  UART_SBUF                               ((uint32_t)0x00ff)           /*!<串口数据缓冲寄存器    */

/************************  Bit definition for UARTSTA register of HT_UART_TypeDef ***********************/
#if  defined  HT6x2x
#define  UART_UARTSTA                       	 ((uint32_t)0x001f)           /*!<uart状态寄存器         */
#else
#define  UART_UARTSTA                       	 ((uint32_t)0x0007)           /*!<uart状态寄存器         */
#endif
#define  UART_UARTSTA_TXIF                       ((uint32_t)0x0001)           /*!<发送中断标志          */
#define  UART_UARTSTA_RXIF                       ((uint32_t)0x0002)           /*!<接收中断标志          */
#define  UART_UARTSTA_PARITY                     ((uint32_t)0x0004)           /*!<奇偶校验出错标志      */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  UART_UARTSTA_PRDIF                      ((uint32_t)0x0008)           /*!<接收上溢中断标志      */

#define  UART_UARTSTA_POLASTA                    ((uint32_t)0x0010)           /*!<极性状态位            */
#define  UART_UARTSTA_POLASTA_POSITIVE           ((uint32_t)0x0000)           /*!<极性状态为正          */
#define  UART_UARTSTA_POLASTA_NEGETIVE           ((uint32_t)0x0010)           /*!<极性状态为负          */
#endif   

/**********************  Bit definition for ISO7816STA register of HT_UART_TypeDef **********************/
#define  UART_ISO7816STA	                     ((uint32_t)0x001f)           /*!<7816状态寄存器         */
#define  UART_ISO7816STA_TXIF                    ((uint32_t)0x0001)           /*!<发送中断标志          */
#define  UART_ISO7816STA_RXIF                    ((uint32_t)0x0002)           /*!<接收中断标志          */
#define  UART_ISO7816STA_PRDIF                   ((uint32_t)0x0004)           /*!<接收上溢中断标志      */
#define  UART_ISO7816STA_TXERRSTAT               ((uint32_t)0x0008)           /*!<发送数据出错标志      */
#define  UART_ISO7816STA_RXERRSTAT               ((uint32_t)0x0010)           /*!<接收数据出错标志      */
                                                                              /*!<Uart3&4 only          */
/*************************  Bit definition for IRCON register of HT_UART_TypeDef ************************/
#define  UART_IRCON		                         ((uint32_t)0x0007)           /*!<红外调制控制寄存器	  */
#define  UART_IRCON_IRTX                         ((uint32_t)0x0001)           /*!<红外调制功能使能控制  */

#define  UART_IRCON_IRLVL                        ((uint32_t)0x0002)           /*!<红外调制输出极性选择  */
#define  UART_IRCON_IRLVL_NEGETIVE               ((uint32_t)0x0002)           /*!<负极性                */
#define  UART_IRCON_IRLVL_POSITIVE               ((uint32_t)0x0000)           /*!<正极性                */

#define  UART_IRCON_IR38KSOURCE                  ((uint32_t)0x0004)           /*!<红外调制信号来源选择  */
#define  UART_IRCON_IR38KSOURCE_HRC              ((uint32_t)0x0004)           /*!<来源于HRC             */
#define  UART_IRCON_IR38KSOURCE_PLL              ((uint32_t)0x0000)           /*!<来源于PLL             */


/*************************  Bit definition for IRDUTY register of HT_UART_TypeDef ***********************/
#define  UART_IRDUTY	                         ((uint32_t)0x0003)           /*!<红外调制脉宽寄存器    */
#define  UART_IRDUTY_IRDUTY                      ((uint32_t)0x0003)           /*!<调制波形占空比设置    */
#define  UART_IRDUTY_IRDUTY_50                   ((uint32_t)0x0000)           /*!<红外调制波形占空比50% */
#define  UART_IRDUTY_IRDUTY_25                   ((uint32_t)0x0001)           /*!<调制波形占空比25%     */
#define  UART_IRDUTY_IRDUTY_12P5                 ((uint32_t)0x0002)           /*!<调制波形占空比12.5%   */
#define  UART_IRDUTY_IRDUTY_6P25                 ((uint32_t)0x0003)           /*!<调制波形占空比6.25%   */

/************************  Bit definition for LOGDETNUM register of HT_UART_TypeDef *********************/
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  UART_NEGDETLEN                          ((uint32_t)0xffff)           /*!<逻辑判断时长设置      */
#endif
/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of I2C Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for I2CDAT register of HT_I2C_TypeDef ************************/
#define  I2C_I2CDAT                              ((uint32_t)0x00ff)           /*!<I2C数据寄存器         */

/*************************  Bit definition for I2CADR register of HT_I2C_TypeDef ************************/
#define  I2C_I2CADR                              ((uint32_t)0x00ff)           /*!<I2C地址寄存器         */

/*************************  Bit definition for I2CCON register of HT_I2C_TypeDef ************************/
#define  I2C_I2CCON   	                         ((uint32_t)0x7fff)           /*!<I2C控制寄存器         */
#define  I2C_I2CCON_AA                           ((uint32_t)0x0004)           /*!<I2C应答生成位         */
#define  I2C_I2CCON_SI                           ((uint32_t)0x0008)           /*!<I2C中断标志位         */
#define  I2C_I2CCON_STO                          ((uint32_t)0x0010)           /*!<I2C停止生成位         */
#define  I2C_I2CCON_STA                          ((uint32_t)0x0020)           /*!<I2C开始生成位         */
#define  I2C_I2CCON_ENS1                         ((uint32_t)0x0040)           /*!<I2C模块使能位         */
#define  I2C_I2CCON_CR                           ((uint32_t)0x7f83)           /*!<I2C时钟频率控制位     */

/*************************  Bit definition for I2CSTA register of HT_I2C_TypeDef ************************/
#define  I2C_I2CSTA                              ((uint32_t)0x00f8)           /*!<I2C状态寄存器         */


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of SPI Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for SPICON register of HT_SPI_TypeDef ************************/
#define  SPI_SPICON	  	                         ((uint32_t)0x00ff)           /*!<SPI控制寄存器         */
#define  SPI_SPICON_SPIEN                        ((uint32_t)0x0001)           /*!<SPI模块使能位         */
#define  SPI_SPICON_MSTR                         ((uint32_t)0x0002)           /*!<SPI主从模式选择位     */
#define  SPI_SPICON_CPOL                         ((uint32_t)0x0004)           /*!<SPI时钟极性选择       */
#define  SPI_SPICON_CPHA                         ((uint32_t)0x0008)           /*!<SPI时钟相位           */
#define  SPI_SPICON_SPR                          ((uint32_t)0x0070)           /*!<SPI时钟速率控制       */
#define  SPI_SPICON_SSDIS                        ((uint32_t)0x0080)           /*!<SS控制位              */

/*************************  Bit definition for SPISTA register of HT_SPI_TypeDef ************************/
#define  SPI_SPISTA		                         ((uint32_t)0x000f)           /*!<SPI状态寄存器    */
#define  SPI_SPISTA_MODF                         ((uint32_t)0x0001)           /*!<SPI模式故障标志位     */
#define  SPI_SPISTA_SSERR                        ((uint32_t)0x0002)           /*!<SPI同步从机错误标志位 */
#define  SPI_SPISTA_WCOL                         ((uint32_t)0x0004)           /*!<SPI写冲突标志位       */
#define  SPI_SPISTA_SPIF                         ((uint32_t)0x0008)           /*!<SPI数据传输完成标志位 */

/*************************  Bit definition for SPIDAT register of HT_SPI_TypeDef ************************/
#define  SPI_SPIDAT                              ((uint32_t)0x00ff)           /*!<SPI数据寄存器         */

/*************************  Bit definition for SPISSN register of HT_SPI_TypeDef ************************/
#define  SPI_SPISSN_CS                           ((uint32_t)0x0001)           /*!<SPI从机选择控制       */

/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of RTC Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for RTCCON register of HT_RTC_TypeDef ************************/
#if defined HT502x
#define  RTC_RTCCON		                         ((uint32_t)0x006f)           /*!<RTC控制寄存器  */
#else
#define  RTC_RTCCON		                         ((uint32_t)0x007f)           /*!<RTC控制寄存器  */
#endif
#define  RTC_RTCCON_AUTOC                        ((uint32_t)0x0001)           /*!<自动手动补偿选择控制  */

#if defined HT501x  ||  defined  HT502x
#define  RTC_RTCCON_TOUT                         ((uint32_t)0x000E)           /*!<Tout输出控制          */
#define  RTC_RTCCON_TOUT_LOW                     ((uint32_t)0x0000)           /*!<Tout输出低            */
#define  RTC_RTCCON_TOUT_HIGH                    ((uint32_t)0x0002)           /*!<Tout输出高            */
#define  RTC_RTCCON_TOUT_LF                      ((uint32_t)0x0004)           /*!<Tout输出低频时钟      */
#define  RTC_RTCCON_TOUT_1HZ                     ((uint32_t)0x0006)           /*!<Tout输出高频补偿1hz   */
#define  RTC_RTCCON_TOUT_2HZ                     ((uint32_t)0x0008)           /*!<Tout输出高频补偿2hz   */
#define  RTC_RTCCON_TOUT_4HZ                     ((uint32_t)0x000A)           /*!<Tout输出高频补偿4hz   */
#define  RTC_RTCCON_TOUT_8HZ                     ((uint32_t)0x000C)           /*!<Tout输出高频补偿8hz   */
#define  RTC_RTCCON_TOUT_RTC1HZ                  ((uint32_t)0x000E)           /*!<Tout输出内部RTC1hz    */
#else
#define  RTC_RTCCON_TOUT                         ((uint32_t)0x001E)           /*!<Tout输出控制          */
#define  RTC_RTCCON_TOUT_LOW                     ((uint32_t)0x0000)           /*!<Tout输出低            */
#define  RTC_RTCCON_TOUT_HIGH                    ((uint32_t)0x0002)           /*!<Tout输出高            */
#define  RTC_RTCCON_TOUT_LF                      ((uint32_t)0x0004)           /*!<Tout输出低频时钟      */
#define  RTC_RTCCON_TOUT_1HZ                     ((uint32_t)0x0006)           /*!<Tout输出高频补偿1hz   */
#define  RTC_RTCCON_TOUT_2HZ                     ((uint32_t)0x0008)           /*!<Tout输出高频补偿2hz   */
#define  RTC_RTCCON_TOUT_4HZ                     ((uint32_t)0x000A)           /*!<Tout输出高频补偿4hz   */
#define  RTC_RTCCON_TOUT_8HZ                     ((uint32_t)0x000C)           /*!<Tout输出高频补偿8hz   */
#define  RTC_RTCCON_TOUT_16HZ                    ((uint32_t)0x000E)           /*!<Tout输出高频补偿16hz  */
#define  RTC_RTCCON_TOUT_32HZ                    ((uint32_t)0x0010)           /*!<Tout输出高频补偿32hz  */
#define  RTC_RTCCON_TOUT_64HZ                    ((uint32_t)0x0012)           /*!<Tout输出高频补偿64hz  */
#define  RTC_RTCCON_TOUT_128HZ                   ((uint32_t)0x0014)           /*!<Tout输出高频补偿128hz */
#endif

#define  RTC_RTCCON_RTC1EN                       ((uint32_t)0x0020)           /*!<RTC定时器1使能位      */
#define  RTC_RTCCON_RTC2EN                       ((uint32_t)0x0040)           /*!<RTC定时器2使能位      */


/*************************  Bit definition for RTCIE register of HT_RTC_TypeDef *************************/
#define  RTC_RTCIE                               ((uint32_t)0x00ff)           /*!<RTC中断使能位         */
#define  RTC_RTCIE_SECIE                         ((uint32_t)0x0001)           /*!<RTC秒中断使能位       */
#define  RTC_RTCIE_MINIE                         ((uint32_t)0x0002)           /*!<RTC分钟中断使能位     */
#define  RTC_RTCIE_HRIE                          ((uint32_t)0x0004)           /*!<RTC小时中断使能位     */
#define  RTC_RTCIE_DAYIE                         ((uint32_t)0x0008)           /*!<RTC日中断使能位       */
#define  RTC_RTCIE_MTHIE                         ((uint32_t)0x0010)           /*!<RTC月中断使能位       */
#define  RTC_RTCIE_RTC1IE                        ((uint32_t)0x0020)           /*!<RTC定时器1中断使能位  */
#define  RTC_RTCIE_RTC2IE                        ((uint32_t)0x0040)           /*!<RTC定时器2中断使能位  */
#define  RTC_RTCIE_ALMIE                         ((uint32_t)0x0080)           /*!<RTC闹铃中断使能位     */

/*************************  Bit definition for RTCIF register of HT_RTC_TypeDef *************************/
#define  RTC_RTCIF                               ((uint32_t)0x00ff)           /*!<RTC中断标志位         */
#define  RTC_RTCIF_SECIF                         ((uint32_t)0x0001)           /*!<RTC秒中断标志位       */
#define  RTC_RTCIF_MINIF                         ((uint32_t)0x0002)           /*!<RTC分钟中断标志位     */
#define  RTC_RTCIF_HRIF                          ((uint32_t)0x0004)           /*!<RTC小时中断标志位     */
#define  RTC_RTCIF_DAYIF                         ((uint32_t)0x0008)           /*!<RTC日中断标志位       */
#define  RTC_RTCIF_MTHIF                         ((uint32_t)0x0010)           /*!<RTC月中断标志位       */
#define  RTC_RTCIF_RTC1IF                        ((uint32_t)0x0020)           /*!<RTC定时器1中断标志位  */
#define  RTC_RTCIF_RTC2IF                        ((uint32_t)0x0040)           /*!<RTC定时器2中断标志位  */
#define  RTC_RTCIF_ALMIF                         ((uint32_t)0x0080)           /*!<RTC闹铃中断标志位     */

/*************************  Bit definition for ALMR register of HT_RTC_TypeDef **************************/
#define  RTC_ALMR                                ((uint32_t)0x1f3f)           /*!<RTC闹铃寄存器         */

/************************  Bit definition for RTCTMR1 register of HT_RTC_TypeDef ************************/
#define  RTC_RTCTMR1                             ((uint32_t)0xffff)           /*!<RTC定时器1寄存器      */

/************************  Bit definition for RTCTMR2 register of HT_RTC_TypeDef ************************/
#define  RTC_RTCTMR2                             ((uint32_t)0xffff)           /*!<RTC定时器2寄存器      */

/**************************  Bit definition for SECR register of HT_RTC_TypeDef *************************/
#define  RTC_SECR                                ((uint32_t)0x003f)           /*!<RTC秒寄存器           */

/**************************  Bit definition for MINR register of HT_RTC_TypeDef *************************/
#define  RTC_MINR                                ((uint32_t)0x003f)           /*!<RTC分钟寄存器         */

/*************************  Bit definition for HOURR register of HT_RTC_TypeDef *************************/
#define  RTC_HOURR                               ((uint32_t)0x001f)           /*!<RTC小时寄存器         */

/**************************  Bit definition for DAYR register of HT_RTC_TypeDef *************************/
#define  RTC_DAYR                                ((uint32_t)0x001f)           /*!<RTC日寄存器           */

/*************************  Bit definition for MONTHR register of HT_RTC_TypeDef ************************/
#define  RTC_MONTHR                              ((uint32_t)0x000f)           /*!<RTC月寄存器           */

/*************************  Bit definition for YEARR register of HT_RTC_TypeDef *************************/
#define  RTC_YEARR                               ((uint32_t)0x007f)           /*!<RTC年寄存器           */

/*************************  Bit definition for WEEKR register of HT_RTC_TypeDef *************************/
#define  RTC_WEEKR                               ((uint32_t)0x0007)           /*!<RTC周寄存器           */

/************************  Bit definition for RTCCNTH register of HT_RTC_TypeDef ************************/
#define  RTC_RTCCNTH                             ((uint32_t)0xffff)           /*!<RTC校时次数高16位     */

/************************  Bit definition for RTCCNTL register of HT_RTC_TypeDef ************************/
#define  RTC_RTCCNTL                             ((uint32_t)0xffff)           /*!<RTC校时次数低16位     */

/*************************  Bit definition for RTCRD register of HT_RTC_TypeDef *************************/
#define  RTC_RTCRD_READFLAG                      ((uint32_t)0x0001)           /*!<RTC读控制位           */

/*************************  Bit definition for RTCWR register of HT_RTC_TypeDef *************************/
#define  RTC_RTCWR_UPDATE                        ((uint32_t)0x0001)           /*!<RTC写控制位           */
#define  RTC_RTCWR_CLEAR                         ((uint32_t)0x0000)           /*!<RTC写控制位清零       */

/***********************  Bit definition for RTCRSTFLG register of HT_RTC_TypeDef ***********************/
#define  RTC_RTCRSTFLG_SOFTReset                 ((uint32_t)0x0001)           /*!<RTC软复位标志         */
#define  RTC_RTCRSTFLG_PORReset                  ((uint32_t)0x0002)           /*!<RTCPOR复位标志        */

/***********************  Bit definition for RTCRSTSET register of HT_RTC_TypeDef ***********************/
#define  RTC_RTCRSTSET                           ((uint32_t)0xffff)           /*!<RTC写复位寄存器       */


/***********************  Bit definition for RTCCHKSUM register of HT_RTC_TypeDef ***********************/
#if  defined  HT6x2x                                                          
#define  RTC_RTCCHKSUM                           ((uint32_t)0xffff)           /*!<RTC校验和寄存器       */
#endif

#if  defined  HT6x2x || defined HT501x  ||  defined HT502x
/**************************  Bit definition for SECR2 register of HT_RTC_TypeDef ************************/
#define  RTC_SECR2                               ((uint32_t)0x003f)           /*!<RTC2秒寄存器          */

/**************************  Bit definition for MINR2 register of HT_RTC_TypeDef ************************/
#define  RTC_MINR2                               ((uint32_t)0x003f)           /*!<RTC2分钟寄存器        */

/*************************  Bit definition for HOURR2 register of HT_RTC_TypeDef ************************/
#define  RTC_HOURR2                              ((uint32_t)0x001f)           /*!<RTC2小时寄存器        */

/**************************  Bit definition for DAYR2 register of HT_RTC_TypeDef ************************/
#define  RTC_DAYR2                               ((uint32_t)0x001f)           /*!<RTC2日寄存器          */

/*************************  Bit definition for MONTHR2 register of HT_RTC_TypeDef ***********************/
#define  RTC_MONTHR2                             ((uint32_t)0x000f)           /*!<RTC2月寄存器          */

/*************************  Bit definition for YEARR2 register of HT_RTC_TypeDef ************************/
#define  RTC_YEARR2                              ((uint32_t)0x007f)           /*!<RTC2年寄存器          */

/*************************  Bit definition for WEEKR2 register of HT_RTC_TypeDef ************************/
#define  RTC_WEEKR2                              ((uint32_t)0x0007)           /*!<RTC2周寄存器          */

/*************************  Bit definition for RTC2CAL register of HT_RTC_TypeDef ***********************/
#define  RTC_RTC2CAL                             ((uint32_t)0x1ffff)          /*!<LRC RTC校正寄存器     */
#endif

/*************************  Bit definition for FRE_LRC register of HT_RTC_TypeDef ***********************/
#if  defined HT501x  ||  defined HT502x
#define  RTC_FRELRC                              ((uint32_t)0x1ffff)          /*!<LRC测频值             */
#endif

#if  defined  HT6x2x  ||  defined  HT502x
/*************************  Bit definition for RTCRD2 register of HT_RTC_TypeDef ************************/
#define  RTC_RTCRD2_READFLAG                     ((uint32_t)0x0001)           /*!<RTC2读控制位          */

/*************************  Bit definition for RTCWR2 register of HT_RTC_TypeDef ************************/
#define  RTC_RTCWR2_UPDATE                       ((uint32_t)0x0001)           /*!<RTC2写控制位          */
#define  RTC_RTCWR2_CLEAR                        ((uint32_t)0x0000)           /*!<RTC2写控制位清零      */
#endif

/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of LCD Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for LCDCLK register of HT_LCD_TypeDef ************************/
#define  LCD_LCDCLK                       	   ((uint32_t)0x009f)           /*!<LCD时钟寄存器		    */

#define  LCD_LCDCLK_LCLK                       ((uint32_t)0x0007)           /*!<LCD扫描频率选择位     */
#define  LCD_LCDCLK_LCLK_DIV64                   ((uint32_t)0x0004)           /*!<Divide Radio = 64     */
#define  LCD_LCDCLK_LCLK_DIV128                  ((uint32_t)0x0000)           /*!<Divide Radio = 128    */
#define  LCD_LCDCLK_LCLK_DIV256                  ((uint32_t)0x0001)           /*!<Divide Radio = 256    */
#define  LCD_LCDCLK_LCLK_DIV512                  ((uint32_t)0x0002)           /*!<Divide Radio = 512    */
#define  LCD_LCDCLK_LCLK_DIV1024                 ((uint32_t)0x0003)           /*!<Divide Radio = 1024   */
                                                                                                        
#define  LCD_LCDCLK_DUTY                       ((uint32_t)0x0018)           /*!<LCDDuty控制选择位     */
#define  LCD_LCDCLK_DUTY_COM4                    ((uint32_t)0x0000)           /*!<4Com                  */
#define  LCD_LCDCLK_DUTY_COM6                    ((uint32_t)0x0008)           /*!<6Com                  */
#define  LCD_LCDCLK_DUTY_COM8                    ((uint32_t)0x0010)           /*!<8Com                  */
                                                                                                        
#define  LCD_LCDCLK_BIASCTL                    ((uint32_t)0x0080)           /*!<偏压驱动选择          */
#define  LCD_LCDCLK_BIASCTL_3                    ((uint32_t)0x0080)           /*!<1/3 bias偏压驱动      */
#define  LCD_LCDCLK_BIASCTL_4                    ((uint32_t)0x0000)           /*!<1/4 bias偏压驱动      */

/*************************  Bit definition for LCDCON register of HT_LCD_TypeDef ************************/

#if defined HT502x
#define  LCD_LCDCON		                         ((uint32_t)0x01ff)           /*!<LCD控制寄存器         */
#else
#define  LCD_LCDCON		                         ((uint32_t)0x00ff)           /*!<LCD控制寄存器         */
#endif


#define  LCD_LCDCON_VRSEL                      ((uint32_t)0x000f)           /*!<LCD对比度调节         */
#define  LCD_LCDCON_LC                         ((uint32_t)0x0010)           /*!<LCD慢速充电模式选择位 */
#define  LCD_LCDCON_FC                         ((uint32_t)0x0020)           /*!<LCD快速充电模式选择位 */

#if defined HT502x
#define  LCD_LCDCON_FCSET                      ((uint32_t)0x00c0)           /*!<LCD快速充电时间选择位 */
#define  LCD_LCDCON_FCSET_16                     ((uint32_t)0x0000)           /*!<充电时间 = 1/16flcd    */
#define  LCD_LCDCON_FCSET_32                     ((uint32_t)0x0040)           /*!<充电时间 = 1/32flcd   */
#define  LCD_LCDCON_FCSET_64                     ((uint32_t)0x0080)           /*!<充电时间 = 1/64flcd   */
#define  LCD_LCDCON_FCSET_128                    ((uint32_t)0x00c0)           /*!<充电时间 = 1/128flcd  */
#else
#define  LCD_LCDCON_FCSET                      ((uint32_t)0x00c0)           /*!<LCD快速充电时间选择位 */
#define  LCD_LCDCON_FCSET_8                      ((uint32_t)0x0000)           /*!<充电时间 = 1/8flcd    */
#define  LCD_LCDCON_FCSET_16                     ((uint32_t)0x0040)           /*!<充电时间 = 1/16flcd   */
#define  LCD_LCDCON_FCSET_32                     ((uint32_t)0x0080)           /*!<充电时间 = 1/32flcd   */
#define  LCD_LCDCON_FCSET_64                     ((uint32_t)0x00c0)           /*!<充电时间 = 1/64flcd   */
#endif

#if defined HT502x
#define  LCD_LCDCON_FCSET2                     ((uint32_t)0x0100)           /*!<电阻分压结构控制位    */
#define  LCD_LCDCON_FCSET2_CHANGER               ((uint32_t)0x0100)           /*!<改善后电阻分压结构    */
#define  LCD_LCDCON_FCSET2_KEEPR                 ((uint32_t)0x0000)           /*!<保持原有电阻分压结构  */
#endif

/*************************  Bit definition for LCDOUT register of HT_LCD_TypeDef ************************/

#if defined HT502x

#define  LCD_LCDOUT                              ((uint32_t)0x0001)           /*!<LCD输出控制寄存器     */
#define  LCD_LCDOUT_ENABLE                       ((uint32_t)0x0001)           /*!<LCD输出打开           */
#define  LCD_LCDOUT_DISABLE                      ((uint32_t)0x0000)           /*!<LCD输出关闭           */

#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of TBS Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for TBSCON register of HT_TBS_TypeDef ************************/
#if  defined  HT501x
#define  TBS_TBSCON		                         ((uint32_t)0xff1b)           /*!<TBS控制寄存器	      */
#define  TBS_TBSCON_TMPEn                        ((uint32_t)0x0001)           /*!<温度测量功能使能      */
#define  TBS_TBSCON_VBATEn                       ((uint32_t)0x0002)           /*!<电池电压测量功能使能  */
#define  TBS_TBSCON_VCCEn                        ((uint32_t)0x0004)           /*!<电池电压测量功能使能  */
#define  TBS_TBSCON_ADC0En                       ((uint32_t)0x0008)           /*!<ADC0测量功能使能      */
#define  TBS_TBSCON_ADC1En                       ((uint32_t)0x0010)           /*!<ADC1测量功能使能      */
#define  TBS_TBSCON_ADC2En                       ((uint32_t)0x0020)           /*!<ADC2测量功能使能      */
#define  TBS_TBSCON_VBATCMPEn                    ((uint32_t)0x0040)           /*!<Vbat比较功能使能      */
#define  TBS_TBSCON_ADC0CMPEn                    ((uint32_t)0x0800)           /*!<ADC0比较功能使能      */
#define  TBS_TBSCON_ADC1CMPEn                    ((uint32_t)0x1000)           /*!<ADC1比较功能使能      */
#define  TBS_TBSCON_xEn                          ((uint32_t)0x183f)           /*!<TBS子模块使能位       */
#elif  defined HT502x
#define  TBS_TBSCON		                         ((uint32_t)0x1b7f)           /*!<TBS控制寄存器	      */
#define  TBS_TBSCON_TMPEn                        ((uint32_t)0x0001)           /*!<温度测量功能使能      */
#define  TBS_TBSCON_VBATEn                       ((uint32_t)0x0002)           /*!<电池电压测量功能使能  */
#define  TBS_TBSCON_VCCEn                        ((uint32_t)0x0004)           /*!<电池电压测量功能使能  */
																						                                  /*!<  对应手册中VDDEn     */
#define  TBS_TBSCON_ADC0En                       ((uint32_t)0x0008)           /*!<ADC0测量功能使能      */
#define  TBS_TBSCON_ADC1En                       ((uint32_t)0x0010)           /*!<ADC1测量功能使能      */
#define  TBS_TBSCON_ADC2En                       ((uint32_t)0x0020)           /*!<ADC2测量功能使能      */
#define  TBS_TBSCON_VBATCMPEn                    ((uint32_t)0x0040)           /*!<Vbat比较功能使能      */
#define  TBS_TBSCON_FILTER                       ((uint32_t)0x0300)           /*!<针对温度测量的Filter输出配置*/
#define  TBS_TBSCON_ADC0CMPEn                    ((uint32_t)0x0800)           /*!<ADC0比较功能使能      */
#define  TBS_TBSCON_ADC1CMPEn                    ((uint32_t)0x1000)           /*!<ADC1比较功能使能      */
#define  TBS_TBSCON_xEn                          ((uint32_t)0x183f)           /*!<TBS子模块使能位       */
#elif  defined HT6x2x
#define  TBS_TBSCON		                         ((uint32_t)0x3ffff)           /*!<TBS控制寄存器	      */
#define  TBS_TBSCON_TMPEn                        ((uint32_t)0x0001)           /*!<温度测量功能使能      */
#define  TBS_TBSCON_VBATEn                       ((uint32_t)0x0002)           /*!<电池电压测量功能使能  */
#define  TBS_TBSCON_ADC0En                       ((uint32_t)0x0004)           /*!<ADC0测量功能使能      */
#define  TBS_TBSCON_ADC1En                       ((uint32_t)0x0008)           /*!<ADC1测量功能使能      */
#define  TBS_TBSCON_FILTER                       ((uint32_t)0x0300)           /*!<针对温度测量的Filter输出配置*/
#define  TBS_TBSCON_ADC3En                       ((uint32_t)0x8000)           /*!<ADC3测量功能使能      */
#define  TBS_TBSCON_ADC4En                       ((uint32_t)0x10000)          /*!<ADC4测量功能使能      */
#define  TBS_TBSCON_ADC5En                       ((uint32_t)0x20000)          /*!<ADC5测量功能使能      */
#define  TBS_TBSCON_VCCEn                        ((uint32_t)0x0010)           /*!<VCC测量功能使能       */
#define  TBS_TBSCON_xEn                          ((uint32_t)0x001f)           /*!<TBS子模块使能位       */ 
#else
#define  TBS_TBSCON		                         ((uint32_t)0x3ffff)           /*!<TBS控制寄存器	      */
#define  TBS_TBSCON_TMPEn                        ((uint32_t)0x0001)           /*!<温度测量功能使能      */
#define  TBS_TBSCON_VBATEn                       ((uint32_t)0x0002)           /*!<电池电压测量功能使能  */
#define  TBS_TBSCON_ADC0En                       ((uint32_t)0x0004)           /*!<ADC0测量功能使能      */
#define  TBS_TBSCON_ADC1En                       ((uint32_t)0x0008)           /*!<ADC1测量功能使能      */
#define  TBS_TBSCON_VCCEn                        ((uint32_t)0x0010)           /*!<VCC测量功能使能       */
#define  TBS_TBSCON_xEn                          ((uint32_t)0x001f)           /*!<TBS子模块使能位       */  
#endif

/*************************  Bit definition for TBSIE register of HT_TBS_TypeDef *************************/
#if  defined  HT501x
#define  TBS_TBSIE                               ((uint32_t)0x03ff)           /*!<TBS中断使能           */
#define  TBS_TBSIE_TMPIE                         ((uint32_t)0x0001)           /*!<温度测量中断使能      */
#define  TBS_TBSIE_VBATIE                        ((uint32_t)0x0002)           /*!<电池电压测量中断使能  */
#define  TBS_TBSIE_VCCIE                         ((uint32_t)0x0004)           /*!<VCC测量中断使能       */
#define  TBS_TBSIE_ADC0IE                        ((uint32_t)0x0008)           /*!<ADC0测量中断使能      */
#define  TBS_TBSIE_ADC1IE                        ((uint32_t)0x0010)           /*!<ADC1测量中断使能      */
#define  TBS_TBSIE_ADC2IE                        ((uint32_t)0x0020)           /*!<ADC2测量中断使能      */
#define  TBS_TBSIE_VBATCMPIE                     ((uint32_t)0x0040)           /*!<电池电压比较中断使能  */
#define  TBS_TBSIE_VREFIE                        ((uint32_t)0x0080)           /*!<VREF测量中断使能      */
#define  TBS_TBSIE_ADC0CMPIE                     ((uint32_t)0x0100)           /*!<ADC0比较中断使能      */
#define  TBS_TBSIE_ADC1CMPIE                     ((uint32_t)0x0200)           /*!<ADC1比较中断使能      */
#elif defined  HT502x
#define  TBS_TBSIE                               ((uint32_t)0x03ff)           /*!<TBS中断使能           */
#define  TBS_TBSIE_TMPIE                         ((uint32_t)0x0001)           /*!<温度测量中断使能      */
#define  TBS_TBSIE_VBATIE                        ((uint32_t)0x0002)           /*!<电池电压测量中断使能  */
#define  TBS_TBSIE_VCCIE                         ((uint32_t)0x0004)           /*!<VCC测量中断使能       */
																						                                  /*!<  对应手册中VDDIE     */
#define  TBS_TBSIE_ADC0IE                        ((uint32_t)0x0008)           /*!<ADC0测量中断使能      */
#define  TBS_TBSIE_ADC1IE                        ((uint32_t)0x0010)           /*!<ADC1测量中断使能      */
#define  TBS_TBSIE_ADC2IE                        ((uint32_t)0x0020)           /*!<ADC2测量中断使能      */
#define  TBS_TBSIE_VBATCMPIE                     ((uint32_t)0x0040)           /*!<电池电压比较中断使能  */
#define  TBS_TBSIE_ADC0CMPIE                     ((uint32_t)0x0100)           /*!<ADC0比较中断使能      */
#define  TBS_TBSIE_ADC1CMPIE                     ((uint32_t)0x0200)           /*!<ADC1比较中断使能      */
#else
#if  defined HT6x1x
#define  TBS_TBSIE                               ((uint32_t)0x003f)           /*!<TBS中断使能           */
#elif defined HT6x2x
#define  TBS_TBSIE                               ((uint32_t)0x03ff)           /*!<TBS中断使能           */
#endif
#define  TBS_TBSIE_TMPIE                         ((uint32_t)0x0001)           /*!<温度测量中断使能      */
#define  TBS_TBSIE_VBATIE                        ((uint32_t)0x0002)           /*!<电池电压测量中断使能  */
#define  TBS_TBSIE_ADC0IE                        ((uint32_t)0x0004)           /*!<ADC0测量中断使能      */
#define  TBS_TBSIE_ADC1IE                        ((uint32_t)0x0008)           /*!<ADC1测量中断使能      */
#define  TBS_TBSIE_VBATCMPIE                     ((uint32_t)0x0010)           /*!<电池电压比较中断使能  */
#define  TBS_TBSIE_VCCIE                         ((uint32_t)0x0020)           /*!<VCC测量中断使能       */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TBS_TBSIE_ADC0CMPIE                     ((uint32_t)0x0040)           /*!<ADC0测量比较中断使能  */
#define  TBS_TBSIE_ADC3IE                        ((uint32_t)0x0080)           /*!<ADC3测量中断使能      */
#define  TBS_TBSIE_ADC4IE                        ((uint32_t)0x0100)           /*!<ADC4测量中断使能      */
#define  TBS_TBSIE_ADC5IE                        ((uint32_t)0x0200)           /*!<ADC5测量中断使能      */
#endif
#endif

/*************************  Bit definition for TBSIF register of HT_TBS_TypeDef *************************/
#if  defined  HT501x
#define  TBS_TBSIF                               ((uint32_t)0x03ff)           /*!<TBS中断标志           */
#define  TBS_TBSIF_TMPIF                         ((uint32_t)0x0001)           /*!<温度测量中断标志      */
#define  TBS_TBSIF_VBATIF                        ((uint32_t)0x0002)           /*!<电池电压测量中断标志  */
#define  TBS_TBSIF_VCCIF                         ((uint32_t)0x0004)           /*!<VCC测量中断标志       */
#define  TBS_TBSIF_ADC0IF                        ((uint32_t)0x0008)           /*!<ADC0测量中断标志      */
#define  TBS_TBSIF_ADC1IF                        ((uint32_t)0x0010)           /*!<ADC1测量中断标志      */
#define  TBS_TBSIF_ADC2IF                        ((uint32_t)0x0020)           /*!<ADC2测量中断标志      */
#define  TBS_TBSIF_VBATCMPIF                     ((uint32_t)0x0040)           /*!<电池电压比较中断标志  */
#define  TBS_TBSIF_VREFIF                        ((uint32_t)0x0080)           /*!<VREF测量中断标志      */
#define  TBS_TBSIF_ADC0CMPIF                     ((uint32_t)0x0100)           /*!<ADC0比较中断标志      */
#define  TBS_TBSIF_ADC1CMPIF                     ((uint32_t)0x0200)           /*!<ADC1比较中断标志      */
#elif defined  HT502x
#define  TBS_TBSIF_TMPIF                         ((uint32_t)0x0001)           /*!<温度测量中断标志      */
#define  TBS_TBSIF_VBATIF                        ((uint32_t)0x0002)           /*!<电池电压测量中断标志  */
#define  TBS_TBSIF_VCCIF                         ((uint32_t)0x0004)           /*!<VCC测量中断标志       */
																						                                  /*!<  对应手册中VDDIF     */
#define  TBS_TBSIF_ADC0IF                        ((uint32_t)0x0008)           /*!<ADC0测量中断标志      */
#define  TBS_TBSIF_ADC1IF                        ((uint32_t)0x0010)           /*!<ADC1测量中断标志      */
#define  TBS_TBSIF_ADC2IF                        ((uint32_t)0x0020)           /*!<ADC2测量中断标志      */
#define  TBS_TBSIF_VBATCMPIF                     ((uint32_t)0x0040)           /*!<电池电压比较中断标志  */
#define  TBS_TBSIF_ADC0CMPIF                     ((uint32_t)0x0100)           /*!<ADC0比较中断标志      */
#define  TBS_TBSIF_ADC1CMPIF                     ((uint32_t)0x0200)           /*!<ADC1比较中断标志      */
#else    /*!<HT6x1x && HT6x2x      */
#if  defined HT6x1x
#define  TBS_TBSIE                               ((uint32_t)0x003f)           /*!<TBS中断标志           */
#elif defined HT6x2x
#define  TBS_TBSIE                               ((uint32_t)0x03ff)           /*!<TBS中断标志           */
#endif
#define  TBS_TBSIF_TMPIF                         ((uint32_t)0x0001)           /*!<温度测量中断标志      */
#define  TBS_TBSIF_VBATIF                        ((uint32_t)0x0002)           /*!<电池电压测量中断标志  */
#define  TBS_TBSIF_ADC0IF                        ((uint32_t)0x0004)           /*!<ADC0测量中断标志      */
#define  TBS_TBSIF_ADC1IF                        ((uint32_t)0x0008)           /*!<ADC1测量中断标志      */
#define  TBS_TBSIF_VBATCMPIF                     ((uint32_t)0x0010)           /*!<电池电压比较中断标志  */
#define  TBS_TBSIF_VCCIF                         ((uint32_t)0x0020)           /*!<VCC测量中断标志       */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  TBS_TBSIF_ADC0CMPIF                     ((uint32_t)0x0040)           /*!<ADC0测量比较中断标志  */
#define  TBS_TBSIF_ADC3IF                        ((uint32_t)0x0080)           /*!<ADC3测量中断标志      */
#define  TBS_TBSIF_ADC4IF                        ((uint32_t)0x0100)           /*!<ADC4测量中断标志      */
#define  TBS_TBSIF_ADC5IF                        ((uint32_t)0x0200)           /*!<ADC5测量中断标志      */
#endif
#endif

/************************  Bit definition for TMPDAT register of HT_TBS_TypeDef *************************/
#define  TBS_TMPDAT                              ((uint32_t)0xffff)           /*!<温度测量输出寄存器    */

/************************  Bit definition for VBATDAT register of HT_TBS_TypeDef ************************/
#define  TBS_VBATDAT                             ((uint32_t)0xffff)           /*!<电池电压测量寄存器    */

/************************  Bit definition for ADC0DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC0DAT                             ((uint32_t)0xffff)           /*!<ADC0测量寄存器        */

/************************  Bit definition for ADC1DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC1DAT                             ((uint32_t)0xffff)           /*!<ADC1测量寄存器        */

/***********************  Bit definition for VBATCMP register of HT_TBS_TypeDef *************************/
#define  TBS_VBATCMP                             ((uint32_t)0xffff)           /*!<电池电压比较寄存器    */

#if  defined  HT501x ||  defined HT502x 
/***********************  Bit definition for ADC2DAT register of HT_TBS_TypeDef *************************/
#define  TBS_ADC2DAT                             ((uint32_t)0xffff)           /*!<ADC2测量寄存器        */
#endif

#if  defined  HT501x
/***********************  Bit definition for VREFDAT register of HT_TBS_TypeDef *************************/
#define  TBS_VREFDAT                             ((uint32_t)0xffff)           /*!<Vref测量寄存器        */
#endif

#if  defined  HT501x ||  defined HT502x 
/************************  Bit definition for ADC0CMP register of HT_TBS_TypeDef ************************/
#define  TBS_ADC0CMP                             ((uint32_t)0xffff)           /*!<ADC0比较寄存器        */

/************************  Bit definition for ADC1CMP register of HT_TBS_TypeDef ************************/                                                        
#define  TBS_ADC1CMP                             ((uint32_t)0xffff)           /*!<ADC1比较寄存器        */
#endif

#if defined HT6x2x
/************************  Bit definition for ADC3DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC3DAT                             ((uint32_t)0xffff)           /*!<ADC0测量寄存器        */

/************************  Bit definition for ADC4DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC4DAT                             ((uint32_t)0xffff)           /*!<ADC1测量寄存器        */

/************************  Bit definition for ADC5DAT register of HT_TBS_TypeDef ************************/
#define  TBS_ADC5DAT                             ((uint32_t)0xffff)           /*!<ADC0测量寄存器        */
#endif

/************************  Bit definition for TBSPRD register of HT_TBS_TypeDef *************************/
#if  defined  HT501x  ||  defined  HT502x
#define  TBS_TBSPRD_TMPPRD                       ((uint32_t)0x0003)           /*!<TMP周期设置           */
#define  TBS_TBSPRD_TMPPRD_1S                    ((uint32_t)0x0000)           /*!<TMP周期 = 1s          */
#define  TBS_TBSPRD_TMPPRD_8S                    ((uint32_t)0x0001)           /*!<TMP周期 = 8s          */
#define  TBS_TBSPRD_TMPPRD_32S                   ((uint32_t)0x0002)           /*!<TMP周期 = 32s         */
#define  TBS_TBSPRD_TMPPRD_125mS                 ((uint32_t)0x0003)           /*!<TMP周期 = 1/8s        */

#define  TBS_TBSPRD_VBATPRD                      ((uint32_t)0x000C)           /*!<Vbat检测周期设置      */
#define  TBS_TBSPRD_VBATPRD_1S                   ((uint32_t)0x0000)           /*!<Vbat检测周期 = 1s     */
#define  TBS_TBSPRD_VBATPRD_4S                   ((uint32_t)0x0004)           /*!<Vbat检测周期 = 4s     */
#define  TBS_TBSPRD_VBATPRD_8S                   ((uint32_t)0x0008)           /*!<Vbat检测周期 = 8s     */
#define  TBS_TBSPRD_VBATPRD_32S                  ((uint32_t)0x000C)           /*!<Vbat检测周期 = 32s    */

#define  TBS_TBSPRD_VCCPRD                       ((uint32_t)0x0030)           /*!<VCC检测周期设置       */
#define  TBS_TBSPRD_VCCPRD_1S                    ((uint32_t)0x0000)           /*!<VCC检测周期 = 1s      */
#define  TBS_TBSPRD_VCCPRD_4S                    ((uint32_t)0x0010)           /*!<VCC检测周期 = 4s      */
#define  TBS_TBSPRD_VCCPRD_8S                    ((uint32_t)0x0020)           /*!<VCC检测周期 = 8s      */
#define  TBS_TBSPRD_VCCPRD_32S                   ((uint32_t)0x0030)           /*!<VCC检测周期 = 32s     */
																										                          /*!<  HT502x此处均对应手册中VDDPRD  */
                                                                                                        
#define  TBS_TBSPRD_ADC0PRD                      ((uint32_t)0x00C0)           /*!<ADC0检测周期设置      */
#define  TBS_TBSPRD_ADC0PRD_1S                   ((uint32_t)0x0000)           /*!<ADC0检测周期 = 1s     */
#define  TBS_TBSPRD_ADC0PRD_4S                   ((uint32_t)0x0040)           /*!<ADC0检测周期 = 4s     */
#define  TBS_TBSPRD_ADC0PRD_8S                   ((uint32_t)0x0080)           /*!<ADC0检测周期 = 8s     */
#define  TBS_TBSPRD_ADC0PRD_32S                  ((uint32_t)0x00C0)           /*!<ADC0检测周期 = 32s    */
                                                                                                        
#define  TBS_TBSPRD_ADC1PRD                      ((uint32_t)0x0300)           /*!<ADC1检测周期设置      */
#define  TBS_TBSPRD_ADC1PRD_1S                   ((uint32_t)0x0000)           /*!<ADC1检测周期 = 1s     */
#define  TBS_TBSPRD_ADC1PRD_4S                   ((uint32_t)0x0100)           /*!<ADC1检测周期 = 4s     */
#define  TBS_TBSPRD_ADC1PRD_8S                   ((uint32_t)0x0200)           /*!<ADC1检测周期 = 8s     */
#define  TBS_TBSPRD_ADC1PRD_32S                  ((uint32_t)0x0300)           /*!<ADC1检测周期 = 32s    */
                                                                                                      
#define  TBS_TBSPRD_ADC2PRD                      ((uint32_t)0x0C00)           /*!<ADC2检测周期设置      */
#define  TBS_TBSPRD_ADC2PRD_1S                   ((uint32_t)0x0000)           /*!<ADC2检测周期 = 1s     */
#define  TBS_TBSPRD_ADC2PRD_4S                   ((uint32_t)0x0400)           /*!<ADC2检测周期 = 4s     */
#define  TBS_TBSPRD_ADC2PRD_8S                   ((uint32_t)0x0800)           /*!<ADC2检测周期 = 8s     */
#define  TBS_TBSPRD_ADC2PRD_32S                  ((uint32_t)0x0C00)           /*!<ADC2检测周期 = 32s    */

#if  defined  HT501x
#define  TBS_TBSPRD_VREFPRD                      ((uint32_t)0x3000)           /*!<Vref检测周期设置      */
#define  TBS_TBSPRD_VREFPRD_1S                   ((uint32_t)0x0000)           /*!<Verf检测周期 = 1s     */
#define  TBS_TBSPRD_VREFPRD_4S                   ((uint32_t)0x1000)           /*!<Vref检测周期 = 4s     */
#define  TBS_TBSPRD_VREFPRD_8S                   ((uint32_t)0x2000)           /*!<Vref检测周期 = 8s     */
#define  TBS_TBSPRD_VREFPRD_32S                  ((uint32_t)0x3000)           /*!<Vref检测周期 = 32s    */
#endif

#else    /*!<HT6x1x && HT6x2x      */

#define  TBS_TBSPRD_TMPPRD                       ((uint32_t)0x0007)           /*!<TMP周期设置           */
#define  TBS_TBSPRD_TMPPRD_OSC0P5S_MEM8S         ((uint32_t)0x0000)           /*!<TMP周期 = 0.5s / 8s   */
#define  TBS_TBSPRD_TMPPRD_OSC1S_MEM4S           ((uint32_t)0x0001)           /*!<TMP周期 = 1s   / 4s   */
#define  TBS_TBSPRD_TMPPRD_OSC2S_MEM2S           ((uint32_t)0x0002)           /*!<TMP周期 = 2s   / 2s   */
#define  TBS_TBSPRD_TMPPRD_OSC4S_MEM1S           ((uint32_t)0x0003)           /*!<TMP周期 = 4s   / 1s   */
#define  TBS_TBSPRD_TMPPRD_OSC8S_MEM0P5S         ((uint32_t)0x0004)           /*!<TMP周期 = 8s   / 0.5s */
#define  TBS_TBSPRD_TMPPRD_OSC16S_MEM125mS       ((uint32_t)0x0005)           /*!<TMP周期 = 16s  / 125mS*/
#define  TBS_TBSPRD_TMPPRD_OSC32S_MEM31mS        ((uint32_t)0x0006)           /*!<TMP周期 = 32s  / 31mS */
#define  TBS_TBSPRD_TMPPRD_OSC64S_MEM8mS         ((uint32_t)0x0007)           /*!<TMP周期 = 64s  / 8mS  */

#define  TBS_TBSPRD_VBATPRD                      ((uint32_t)0x0018)           /*!<Vbat检测周期设置      */
#define  TBS_TBSPRD_VBATPRD_1S                   ((uint32_t)0x0000)           /*!<Vbat检测周期 = 1s     */
#define  TBS_TBSPRD_VBATPRD_2S                   ((uint32_t)0x0008)           /*!<Vbat检测周期 = 2s     */
#define  TBS_TBSPRD_VBATPRD_8S                   ((uint32_t)0x0010)           /*!<Vbat检测周期 = 8s     */
#define  TBS_TBSPRD_VBATPRD_16S                  ((uint32_t)0x0018)           /*!<Vbat检测周期 = 16s    */
                                                                                                        
#define  TBS_TBSPRD_ADC0PRD                      ((uint32_t)0x0060)           /*!<ADC0检测周期设置      */
#define  TBS_TBSPRD_ADC0PRD_1S                   ((uint32_t)0x0000)           /*!<ADC0检测周期 = 1s     */
#define  TBS_TBSPRD_ADC0PRD_2S                   ((uint32_t)0x0020)           /*!<ADC0检测周期 = 2s     */
#define  TBS_TBSPRD_ADC0PRD_8S                   ((uint32_t)0x0040)           /*!<ADC0检测周期 = 8s     */
#define  TBS_TBSPRD_ADC0PRD_16S                  ((uint32_t)0x0060)           /*!<ADC0检测周期 = 16s    */
                                                                                                        
#define  TBS_TBSPRD_ADC1PRD                      ((uint32_t)0x0180)           /*!<ADC1检测周期设置      */
#define  TBS_TBSPRD_ADC1PRD_1S                   ((uint32_t)0x0000)           /*!<ADC1检测周期 = 1s     */
#define  TBS_TBSPRD_ADC1PRD_2S                   ((uint32_t)0x0080)           /*!<ADC1检测周期 = 2s     */
#define  TBS_TBSPRD_ADC1PRD_8S                   ((uint32_t)0x0100)           /*!<ADC1检测周期 = 8s     */
#define  TBS_TBSPRD_ADC1PRD_16S                  ((uint32_t)0x0180)           /*!<ADC1检测周期 = 16s    */

#define  TBS_TBSPRD_VCCPRD                       ((uint32_t)0x0600)           /*!<VCC检测周期设置       */
#define  TBS_TBSPRD_VCCPRD_1S                    ((uint32_t)0x0000)           /*!<VCC检测周期 = 1s      */
#define  TBS_TBSPRD_VCCPRD_2S                    ((uint32_t)0x0200)           /*!<VCC检测周期 = 2s      */
#define  TBS_TBSPRD_VCCPRD_8S                    ((uint32_t)0x0400)           /*!<VCC检测周期 = 8s      */
#define  TBS_TBSPRD_VCCPRD_16S                   ((uint32_t)0x0600)           /*!<VCC检测周期 = 16s     */
#if defined HT6x2x 
#define  TBS_TBSPRD_ADC3PRD                      ((uint32_t)0x1800)           /*!<ADC3检测周期设置      */
#define  TBS_TBSPRD_ADC3PRD_1S                   ((uint32_t)0x0000)           /*!<ADC3检测周期 = 1s     */
#define  TBS_TBSPRD_ADC3PRD_2S                   ((uint32_t)0x0800)           /*!<ADC3检测周期 = 2s     */
#define  TBS_TBSPRD_ADC3PRD_8S                   ((uint32_t)0x1000)           /*!<ADC3检测周期 = 8s     */
#define  TBS_TBSPRD_ADC3PRD_16S                  ((uint32_t)0x1800)           /*!<ADC3检测周期 = 16s    */

#define  TBS_TBSPRD_ADC4PRD                      ((uint32_t)0x6000)           /*!<ADC4检测周期设置      */
#define  TBS_TBSPRD_ADC4PRD_1S                   ((uint32_t)0x0000)           /*!<ADC4检测周期 = 1s     */
#define  TBS_TBSPRD_ADC4PRD_2S                   ((uint32_t)0x2000)           /*!<ADC4检测周期 = 2s     */
#define  TBS_TBSPRD_ADC4PRD_8S                   ((uint32_t)0x4000)           /*!<ADC4检测周期 = 8s     */
#define  TBS_TBSPRD_ADC4PRD_16S                  ((uint32_t)0x6000)           /*!<ADC4检测周期 = 16s    */

#define  TBS_TBSPRD_ADC5PRD                      ((uint32_t)0x18000)           /*!<ADC5检测周期设置      */
#define  TBS_TBSPRD_ADC5PRD_1S                   ((uint32_t)0x00000)           /*!<ADC5检测周期 = 1s     */
#define  TBS_TBSPRD_ADC5PRD_2S                   ((uint32_t)0x08000)           /*!<ADC5检测周期 = 2s     */
#define  TBS_TBSPRD_ADC5PRD_8S                   ((uint32_t)0x10000)           /*!<ADC5检测周期 = 8s     */
#define  TBS_TBSPRD_ADC5PRD_16S                  ((uint32_t)0x18000)           /*!<ADC5检测周期 = 16s    */
#endif
#endif

/*************************  Bit definition for VCCDAT register of HT_TBS_TypeDef ************************/
#define  TBS_VCCDAT                              ((uint32_t)0xffff)           /*!<VCC测量寄存器         */
                                                         /*!< HT6x2x               */
/************************  Bit definition for ADC0CMP register of HT_TBS_TypeDef ************************/
#if  defined  HT6x2x                                                          
#define  TBS_ADC0CMP                             ((uint32_t)0xffff)           /*!<ADC0比较寄存器        */
#define  TBS_ADC3CMP                             ((uint32_t)0xffff)           /*!<ADC3比较寄存器        */
#define  TBS_ADC4CMP                             ((uint32_t)0xffff)           /*!<ADC4比较寄存器        */
#define  TBS_ADC5CMP                             ((uint32_t)0xffff)           /*!<ADC5比较寄存器        */
#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of PMU Module  
**********************************************************************************************************
*/  
/*************************  Bit definition for PMUCON register of HT_PMU_TypeDef ************************/
#if defined HT6x2x
#define  PMU_PMUCON			                     ((uint32_t)0x803f)           /*!<PMU控制寄存器	      */
#elif defined  HT501x  ||  defined  HT502x
#define  PMU_PMUCON			                     ((uint32_t)0x801f)           /*!<PMU控制寄存器	      */
#elif defined HT6x1x
#define  PMU_PMUCON			                     ((uint32_t)0x0017)           /*!<PMU控制寄存器	      */
#endif
#define  PMU_PMUCON_BORDETEN                     ((uint32_t)0x0001)           /*!<BOR_DET 模块使能      */
#define  PMU_PMUCON_BORRST                       ((uint32_t)0x0002)           /*!<BOR中断/复位选择      */
#define  PMU_PMUCON_LVD0DETEN                    ((uint32_t)0x0004)           /*!<LVDIN0模块使能        */
#if  defined  HT6x2x  ||  defined  HT501x   ||  defined  HT502x                                                  
#define  PMU_PMUCON_LVD1DETEN                    ((uint32_t)0x0008)           /*!<LVDIN1模块使能        */
#endif
#define  PMU_PMUCON_HoldLDO                      ((uint32_t)0x0010)           /*!<Hold下大功耗LDO打开   */
#if  defined  HT6x2x  
#define  PMU_PMUCON_POWDETEN                     ((uint32_t)0x0020)           /*!<POW_DET模块使能       */
#endif
#if  defined  HT6x2x  ||  defined HT501x  ||  defined  HT502x
#define  PMU_PMUCON_DisChargeEN                  ((uint32_t)0x8000)           /*!<电池钝化功能使能      */
#endif

/*************************  Bit definition for VDETCFG register of HT_PMU_TypeDef ***********************/
#if defined HT6x2x
#define  PMU_VDETCFG		                     ((uint32_t)0xc1ff)           /*!<电源检测阀值配置寄存器  */
#elif  defined  HT501x
#define  PMU_VDETCFG		                     ((uint32_t)0x800f)           /*!<电源检测阀值配置寄存器  */
#elif  defined HT6x1x
#define  PMU_VDETCFG		                     ((uint32_t)0x003f)           /*!<电源检测阀值配置寄存器  */
#elif  defined HT502x
#define  PMU_VDETCFG		                     ((uint32_t)0x80bf)           /*!<电源检测阀值配置寄存器  */
#endif

#if  defined  HT501x  ||  defined  HT502x  ||  defined  HT6x1x
#define  PMU_VDETCFG_BORLVL                      ((uint32_t)0x0003)           /*!<BOR_DET 检测阈值设定  */
#define  PMU_VDETCFG_BORLVL_2V2                  ((uint32_t)0x0001)           /*!<检测阈值 = 2.2v       */
#define  PMU_VDETCFG_BORLVL_2V4                  ((uint32_t)0x0000)           /*!<检测阈值 = 2.4v       */
#define  PMU_VDETCFG_BORLVL_2V6                  ((uint32_t)0x0003)           /*!<检测阈值 = 2.6v       */
#define  PMU_VDETCFG_BORLVL_2V8                  ((uint32_t)0x0002)           /*!<检测阈值 = 2.8v       */
#elif  defined  HT6x2x
#define  PMU_VDETCFG_BORLVL                      ((uint32_t)0x0003)           /*!<BOR_DET 检测阈值设定  */
#define  PMU_VDETCFG_BORLVL_2V0                  ((uint32_t)0x0001)           /*!<检测阈值 = 2.0v       */
#define  PMU_VDETCFG_BORLVL_2V2                  ((uint32_t)0x0000)           /*!<检测阈值 = 2.2v       */
#define  PMU_VDETCFG_BORLVL_2V6                  ((uint32_t)0x0003)           /*!<检测阈值 = 2.6v       */
#define  PMU_VDETCFG_BORLVL_2V8                  ((uint32_t)0x0002)           /*!<检测阈值 = 2.8v       */
#endif

#if  defined  HT501x
#define  PMU_VDETCFG_VCCLVL                      ((uint32_t)0x000c)           /*!<VCC_DET 检测阈值设定  */
#define  PMU_VDETCFG_VCCLVL_2V4                  ((uint32_t)0x0000)           /*!<检测阈值 = 2.4V       */
#define  PMU_VDETCFG_VCCLVL_2V6                  ((uint32_t)0x0004)           /*!<检测阈值 = 2.6V       */
#define  PMU_VDETCFG_VCCLVL_2V8                  ((uint32_t)0x0008)           /*!<检测阈值 = 2.8V       */
#define  PMU_VDETCFG_VCCLVL_3V0                  ((uint32_t)0x000c)           /*!<检测阈值 = 3.0V       */
#else    /*!<HT6x1x && HT6x2x && HT502x      */
				 /*!<HT502x此处均对应手册中的VSYS_LVL */
#define  PMU_VDETCFG_VCCLVL                      ((uint32_t)0x003c)           /*!<VCC_DET 检测阈值设定  */
#define  PMU_VDETCFG_VCCLVL_2V4                  ((uint32_t)0x0000)           /*!<检测阈值 = 2.4V       */
#define  PMU_VDETCFG_VCCLVL_2V6                  ((uint32_t)0x0004)           /*!<检测阈值 = 2.6V       */
#define  PMU_VDETCFG_VCCLVL_2V8                  ((uint32_t)0x0008)           /*!<检测阈值 = 2.8V       */
#define  PMU_VDETCFG_VCCLVL_3V0                  ((uint32_t)0x000c)           /*!<检测阈值 = 3.0V       */
#define  PMU_VDETCFG_VCCLVL_3V2                  ((uint32_t)0x0010)           /*!<检测阈值 = 3.2V       */
#define  PMU_VDETCFG_VCCLVL_3V4                  ((uint32_t)0x0014)           /*!<检测阈值 = 3.4V       */
#define  PMU_VDETCFG_VCCLVL_3V6                  ((uint32_t)0x0018)           /*!<检测阈值 = 3.6V       */
#define  PMU_VDETCFG_VCCLVL_3V8                  ((uint32_t)0x001c)           /*!<检测阈值 = 3.8V       */
#define  PMU_VDETCFG_VCCLVL_4V0                  ((uint32_t)0x0020)           /*!<检测阈值 = 4.0V       */
#define  PMU_VDETCFG_VCCLVL_4V2                  ((uint32_t)0x0024)           /*!<检测阈值 = 4.2V       */
#define  PMU_VDETCFG_VCCLVL_4V4                  ((uint32_t)0x0028)           /*!<检测阈值 = 4.4V       */
#define  PMU_VDETCFG_VCCLVL_4V6                  ((uint32_t)0x002c)           /*!<检测阈值 = 4.6V       */
#define  PMU_VDETCFG_VCCLVL_4V8                  ((uint32_t)0x0030)           /*!<检测阈值 = 4.8V       */
#define  PMU_VDETCFG_VCCLVL_5V0                  ((uint32_t)0x0034)           /*!<检测阈值 = 5.0V       */
#if defined  HT6x2x                                                                                                      
#define  PMU_VDETCFG_POWLVL                      ((uint32_t)0x01c0)           /*!<POW_DET 检测阈值设定  */
#define  PMU_VDETCFG_POWLVL_1P25mV               ((uint32_t)0x0000)           /*!<检测阈值 = 1.25mV     */
#define  PMU_VDETCFG_POWLVL_2P5mV                ((uint32_t)0x0040)           /*!<检测阈值 = 2.5mV      */
#define  PMU_VDETCFG_POWLVL_3P75mV               ((uint32_t)0x0080)           /*!<检测阈值 = 3.75mV     */
#define  PMU_VDETCFG_POWLVL_5mV                  ((uint32_t)0x00c0)           /*!<检测阈值 = 5mV        */
#define  PMU_VDETCFG_POWLVL_6P25mV               ((uint32_t)0x0100)           /*!<检测阈值 = 6.25mV     */
#define  PMU_VDETCFG_POWLVL_7P5mV                ((uint32_t)0x0140)           /*!<检测阈值 = 7.5mV      */
#define  PMU_VDETCFG_POWLVL_8P75mV               ((uint32_t)0x0180)           /*!<检测阈值 = 8.75mV     */
#define  PMU_VDETCFG_POWLVL_10mV                 ((uint32_t)0x01c0)           /*!<检测阈值 = 10mV       */
#endif
#endif

#if  defined HT6x2x  
#define  PMU_VDETCFG_DisChargeCTRL               ((uint32_t)0xc000)           /*!<电池钝化电流大小选择   */
#define  PMU_VDETCFG_DisChargeCTRL_1mA           ((uint32_t)0x0000)           /*!<电池钝化电流1mA       */
#define  PMU_VDETCFG_DisChargeCTRL_2mA           ((uint32_t)0x8000)           /*!<电池钝化电流2mA       */
#define  PMU_VDETCFG_DisChargeCTRL_50uA          ((uint32_t)0x4000)           /*!<电池钝化电流50uA      */
#define  PMU_VDETCFG_DisChargeCTRL_100uA         ((uint32_t)0xc000)           /*!<电池钝化电流100uA     */
#elif  defined HT501x  ||  defined  HT502x
#define  PMU_VDETCFG_DisChargeCTRL               ((uint32_t)0x8000)           /*!<电池钝化电流大小选择   */
#define  PMU_VDETCFG_DisChargeCTRL_1mA           ((uint32_t)0x0000)           /*!<电池钝化电流1mA       */
#define  PMU_VDETCFG_DisChargeCTRL_2mA           ((uint32_t)0x8000)           /*!<电池钝化电流2mA       */
#endif

/************************  Bit definition for VDETPCFG register of HT_PMU_TypeDef ***********************/
#define  PMU_VDETPCFG		                     ((uint32_t)0x001f)           /*!<电源检测周期寄存器      */

#define  PMU_VDETPCFG_VDETPRD                    ((uint32_t)0x0007)           /*!<分时检测周期设定      */
#define  PMU_VDETPCFG_VDETPRD_16P5mS             ((uint32_t)0x0000)           /*!<分时检测周期= 16.5ms  */
#define  PMU_VDETPCFG_VDETPRD_33mS               ((uint32_t)0x0001)           /*!<分时检测周期= 33ms    */
#define  PMU_VDETPCFG_VDETPRD_67mS               ((uint32_t)0x0002)           /*!<分时检测周期= 67ms    */
#define  PMU_VDETPCFG_VDETPRD_134mS              ((uint32_t)0x0003)           /*!<分时检测周期= 134ms   */
#define  PMU_VDETPCFG_VDETPRD_268mS              ((uint32_t)0x0004)           /*!<分时检测周期= 268ms   */
#define  PMU_VDETPCFG_VDETPRD_536mS              ((uint32_t)0x0005)           /*!<分时检测周期= 536ms   */
#define  PMU_VDETPCFG_VDETPRD_1072mS             ((uint32_t)0x0006)           /*!<分时检测周期= 1072ms  */
#define  PMU_VDETPCFG_VDETPRD_2144mS             ((uint32_t)0x0007)           /*!<分时检测周期= 2144ms  */

#define  PMU_VDETPCFG_VDETTIME                   ((uint32_t)0x0018)           /*!<分时检测时间设定，实际检测时间以注释为准      */
#define  PMU_VDETPCFG_VDETTIME_300uS             ((uint32_t)0x0000)           /*!<分时检测时间 = 336us  */
#define  PMU_VDETPCFG_VDETTIME_360uS             ((uint32_t)0x0008)           /*!<分时检测时间 = 397us  */
#define  PMU_VDETPCFG_VDETTIME_480uS             ((uint32_t)0x0010)           /*!<分时检测时间 = 519us  */
#define  PMU_VDETPCFG_VDETTIME_1120uS            ((uint32_t)0x0018)           /*!<分时检测时间 = 1068us  */
#define  PMU_VDETPCFG_VDETTIME_560uS             ((uint32_t)0x0018)           /*!<分时检测时间 = 1068us，兼容旧版本定义  */

/**************************  Bit definition for PMUIE register of HT_PMU_TypeDef ************************/
#if defined  HT6x2x   
#define  PMU_PMUIE                               ((uint32_t)0x001f)           /*!<PMU中断使能           */
#elif defined  HT501x  ||  defined  HT502x
#define  PMU_PMUIE                               ((uint32_t)0x000f)           /*!<PMU中断使能           */
#elif defined  HT6x1x
#define  PMU_PMUIE                               ((uint32_t)0x0007)           /*!<PMU中断使能           */
#endif
#define  PMU_PMUIE_VCCIE                         ((uint32_t)0x0001)           /*!<VCC检测中断使能       */
																																							/*!<  HT502x此处对应手册中的VSYSIE */
#define  PMU_PMUIE_BORIE                         ((uint32_t)0x0002)           /*!<BOR检测中断使能       */
#define  PMU_PMUIE_LVD0IE                        ((uint32_t)0x0004)           /*!<LVDIN0检测中断使能    */
#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x                                    
#define  PMU_PMUIE_LVD1IE                        ((uint32_t)0x0008)           /*!<LVDIN1检测中断使能    */
#endif
#if  defined  HT6x2x
#define  PMU_PMUIE_POWIE                         ((uint32_t)0x0010)           /*!<POW检测中断使能       */
#endif
/**************************  Bit definition for PMUIF register of HT_PMU_TypeDef ************************/
#if defined  HT6x2x  
#define  PMU_PMUIF								 ((uint32_t)0x001f)           /*!<PMU中断标志     	  */
#elif defined  HT501x  ||  defined  Ht502x
#define  PMU_PMUIF								 ((uint32_t)0x000f)           /*!<PMU中断标志     	  */
#elif defined  HT6x1x
#define  PMU_PMUIF								 ((uint32_t)0x0007)           /*!<PMU中断标志     	  */
#endif
#define  PMU_PMUIF_VCCIF                         ((uint32_t)0x0001)           /*!<VCC检测中断标志       */
																																							/*!<  HT502x此处对应手册中的VSYSIF */
#define  PMU_PMUIF_BORIF                         ((uint32_t)0x0002)           /*!<BOR检测中断标志       */
#define  PMU_PMUIF_LVD0IF                        ((uint32_t)0x0004)           /*!<LVDIN0检测中断标志    */
#if  defined  HT6x2x   ||  defined  HT501x  ||  defined  HT502x                                                          
#define  PMU_PMUIF_LVD1IF                        ((uint32_t)0x0008)           /*!<LVDIN1检测中断标志    */
#endif
#if  defined  HT6x2x  
#define  PMU_PMUIF_POWIF                         ((uint32_t)0x0010)           /*!<POW检测中断标志       */
#endif
/*************************  Bit definition for PMUSTA register of HT_PMU_TypeDef ************************/
#if defined  HT6x2x 
#define  PMU_PMUSTA								 ((uint32_t)0x001f)           /*!<PMU状态标志     	  */
#elif defined  HT501x  ||  defined  HT502x
#define  PMU_PMUSTA								 ((uint32_t)0x000f)           /*!<PMU状态标志     	  */
#elif defined  HT6x1x
#define  PMU_PMUSTA								 ((uint32_t)0x0007)           /*!<PMU状态标志     	  */
#endif
#define  PMU_PMUSTA_VCCFLG                       ((uint32_t)0x0001)           /*!<VCC电压状态@VCCLVL    */
																																							/*!<  HT502x此处对应手册中的VSYS_FLG */
#define  PMU_PMUSTA_BORFLG                       ((uint32_t)0x0002)           /*!<VCC电压状态@BORLVL    */
#define  PMU_PMUSTA_LVD0FLG                      ((uint32_t)0x0004)           /*!<LVDIN0引脚状态        */
#if  defined  HT6x2x   ||  defined  HT501x  ||  HT502x
#define  PMU_PMUSTA_LVD1FLG                      ((uint32_t)0x0008)           /*!<LVDIN1引脚状态        */
#endif
#if  defined  HT6x2x   
#define  PMU_PMUSTA_POWFLG                       ((uint32_t)0x0010)           /*!<POWIN引脚状态         */
                                                                              /*!< bit=1：大于设定阈值  */
#endif
/*************************  Bit definition for WAKEIF register of HT_PMU_TypeDef ************************/
#define  PMU_WAKEIF                              ((uint32_t)0xffffff)         /*!<唤醒标志              */
#define  PMU_WAKEIF_PMUWKIF                      ((uint32_t)0x0001)           /*!<PMU唤醒标志           */
#define  PMU_WAKEIF_INT0WKIF                     ((uint32_t)0x0004)           /*!<INT0唤醒标志          */
#define  PMU_WAKEIF_INT1WKIF                     ((uint32_t)0x0008)           /*!<INT1唤醒标志          */
#define  PMU_WAKEIF_INT2WKIF                     ((uint32_t)0x0010)           /*!<INT2唤醒标志          */
#define  PMU_WAKEIF_INT3WKIF                     ((uint32_t)0x0020)           /*!<INT3唤醒标志          */
#define  PMU_WAKEIF_INT4WKIF                     ((uint32_t)0x0040)           /*!<INT4唤醒标志          */
#define  PMU_WAKEIF_INT5WKIF                     ((uint32_t)0x0080)           /*!<INT5唤醒标志          */
#define  PMU_WAKEIF_INT6WKIF                     ((uint32_t)0x0100)           /*!<INT6唤醒标志          */
#define  PMU_WAKEIF_RX0WKIF                      ((uint32_t)0x0200)           /*!<RX0唤醒标志           */
#define  PMU_WAKEIF_RX1WKIF                      ((uint32_t)0x0400)           /*!<RX1唤醒标志           */
#define  PMU_WAKEIF_RX2WKIF                      ((uint32_t)0x0800)           /*!<RX2唤醒标志           */
#define  PMU_WAKEIF_RX3WKIF                      ((uint32_t)0x1000)           /*!<RX3唤醒标志           */
#define  PMU_WAKEIF_RX4WKIF                      ((uint32_t)0x2000)           /*!<RX4唤醒标志           */
#define  PMU_WAKEIF_RX5WKIF                      ((uint32_t)0x4000)           /*!<RX5唤醒标志           */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  PMU_WAKEIF_TMR4WKIF                     ((uint32_t)0x20000)          /*!<Timer4唤醒标志        */
#define  PMU_WAKEIF_TMR5WKIF                     ((uint32_t)0x40000)          /*!<Timer5唤醒标志        */
#endif
#define  PMU_WAKEIF_TBSWKIF                      ((uint32_t)0x80000)          /*!<TBS唤醒标志           */
#define  PMU_WAKEIF_RTCWKIF                      ((uint32_t)0x100000)         /*!<RTC唤醒标志           */
#if  defined  HT6x1x ||  defined HT501x
#define  PMU_WAKEIF_WDTWKIF                      ((uint32_t)0x800000)         /*!<WDT唤醒标志           */
#endif
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  PMU_WAKEIF_RX6WKIF                      ((uint32_t)0x8000000)        /*!<RX6唤醒标志           */
#define  PMU_WAKEIF_INT7WKIF                     ((uint32_t)0x10000000)       /*!<INT7唤醒标志          */
#define  PMU_WAKEIF_INT8WKIF                     ((uint32_t)0x20000000)       /*!<INT8唤醒标志          */
#define  PMU_WAKEIF_INT9WKIF                     ((uint32_t)0x40000000)       /*!<INT9唤醒标志          */
#endif

#if  defined  HT502x
#define  PMU_WAKEIF_ARGEWKIF                     ((uint32_t)0x00000002)       /*!<AES/RAND/GHASH/ECC唤醒标志 */
#define  PMU_WAKEIF_TMR0WKIF                     ((uint32_t)0x00008000)       /*!<Timer0唤醒标志             */
#define  PMU_WAKEIF_TMR1WKIF                     ((uint32_t)0x00010000)       /*!<Timer1唤醒标志             */
#define  PMU_WAKEIF_TMR2WKIF                     ((uint32_t)0x00020000)       /*!<Timer2唤醒标志             */
#define  PMU_WAKEIF_TMR3WKIF                     ((uint32_t)0x00040000)       /*!<Timer3唤醒标志             */
#define  PMU_WAKEIF_IICWKIF                      ((uint32_t)0x00200000)       /*!<IIC唤醒标志                */
#define  PMU_WAKEIF_SPIWKIF                      ((uint32_t)0x00400000)       /*!<SPI唤醒标志                */
#define  PMU_WAKEIF_SELFTESTWKIF                 ((uint32_t)0x01000000)       /*!<SelfTest唤醒标志           */
#define  PMU_WAKEIF_EMUWKIF                      ((uint32_t)0x02000000)       /*!<EMU唤醒标志                */
#define  PMU_WAKEIF_DMAWKIF                      ((uint32_t)0x04000000)       /*!<DMA唤醒标志                */
#define  PMU_WAKEIF_KEYWKIF                      ((uint32_t)0x08000000)       /*!<KEY唤醒标志                */
#define  PMU_WAKEIF_INT7WKIF                     ((uint32_t)0x10000000)       /*!<INT7唤醒标志               */
#define  PMU_WAKEIF_INT8WKIF                     ((uint32_t)0x20000000)       /*!<INT8唤醒标志               */
#define  PMU_WAKEIF_INT9WKIF                     ((uint32_t)0x40000000)       /*!<INT9唤醒标志               */
#define  PMU_WAKEIF_NMIWKIF                      ((uint32_t)0x80000000)       /*!<NMI唤醒标志                */
#endif

/*************************  Bit definition for RSTSTA register of HT_PMU_TypeDef ************************/
#define  PMU_RSTSTA                              ((uint32_t)0xffff)           /*!<复位标志位            */
#define  PMU_RSTSTA_PORRST                       ((uint32_t)0x0001)           /*!<POR复位标志位         */
#define  PMU_RSTSTA_LBORRST                      ((uint32_t)0x0002)           /*!<LBOR复位标志位        */
#define  PMU_RSTSTA_WDTRST                       ((uint32_t)0x0004)           /*!<WDT复位标志位         */
#define  PMU_RSTSTA_WakeupRST                    ((uint32_t)0x0008)           /*!<WakeUp复位标志        */
#define  PMU_RSTSTA_ExtRST                       ((uint32_t)0x0020)           /*!<外部RST复位标志       */
#define  PMU_RSTSTA_SoftRST                      ((uint32_t)0x0040)           /*!<软复位标志            */
#define  PMU_RSTSTA_DebugRST                     ((uint32_t)0x0080)           /*!<调试复位标志          */
#define  PMU_RSTSTA_BORRST                       ((uint32_t)0x0100)           /*!<BOR复位标志           */
#define  PMU_RSTSTA_SleepFlag                    ((uint32_t)0x4000)           /*!<Sleep唤醒复位标志     */
#define  PMU_RSTSTA_HoldFlag                     ((uint32_t)0x8000)           /*!<Hold唤醒标志          */


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of CMU Module  
**********************************************************************************************************
*/  
/**************************  Bit definition for WPREG register of HT_CMU_TypeDef ************************/
#define  CMU_WPREG                               ((uint32_t)0xffff)           /*!<写保护寄存器          */
#define  CMU_WPREG_UnProtected                   ((uint32_t)0xA55A)           /*!<关闭写保护功能        */
#define  CMU_WPREG_Protected                     ((uint32_t)0x0000)           /*!<打开写保护功能        */
                                                                              /*!<0xA55A: Unprotected   */
                                                                              /*!<other : Protected     */
/************************  Bit definition for SYSCLKCFG register of HT_CMU_TypeDef **********************/
#if  defined  HT6x2x  ||  defined  HT6x1x  ||  defined  HT502x
#define  CMU_SYSCLKCFG_CLKSEL                    ((uint32_t)0x0007)           /*!<系统时钟选择控制位    */
#elif defined HT501x
#define  CMU_SYSCLKCFG_CLKSEL                    ((uint32_t)0x0003)           /*!<系统时钟选择控制位    */
#endif
#define  CMU_SYSCLKCFG_CLKSEL_LRC                ((uint32_t)0x0000)           /*!<选择lrc               */
#define  CMU_SYSCLKCFG_CLKSEL_LF                 ((uint32_t)0x0001)           /*!<选择低频osc           */
#define  CMU_SYSCLKCFG_CLKSEL_HRC                ((uint32_t)0x0002)           /*!<选择hrc               */
#define  CMU_SYSCLKCFG_CLKSEL_PLL                ((uint32_t)0x0003)           /*!<选择PLL               */
#if  defined  HT6x2x  ||  defined  HT6x1x
#define  CMU_SYSCLKCFG_CLKSEL_MEMS               ((uint32_t)0x0004)           /*!<选择mems              */
#endif
#define  CMU_SYSCLKCFG_WCLKEN                    ((uint32_t)0x0080)           /*!<时钟配置写保护位      */

/*************************  Bit definition for JTAGSTA register of HT_CMU_TypeDef ***********************/
#define  CMU_JTAGSTA                             ((uint32_t)0x0001)           /*!<JTAG状态指示          */

/*************************  Bit definition for LRCADJ register of HT_CMU_TypeDef ************************/
#if defined  HT6x1x ||  defined  HT501x  ||  defined  HT502x
#define  CMU_LRCADJ                              ((uint32_t)0x000f)           /*!<lrc输出频率调制位     */
#elif  defined  HT6x2x 
#define  CMU_LRCADJ                              ((uint32_t)0x00ff)           /*!<lrc输出频率调制位     */
#endif                                                                            /*!<HT6x2x长度待定        */
/*************************  Bit definition for HRCADJ register of HT_CMU_TypeDef ************************/
#if defined  HT6x1x ||  defined  HT501x
#define  CMU_HRCADJ                              ((uint32_t)0x003f)           /*!<hrc输出频率调制位     */
#elif  defined  HT6x2x  ||  defined  HT502x
#define  CMU_HRCADJ                              ((uint32_t)0x007f)           /*!<hrc输出频率调制位     */
#endif                                                                              /*!<HT6x2x长度待定        */
/*************************  Bit definition for HRCDIV register of HT_CMU_TypeDef ************************/
#if defined  HT6x1x ||  defined  HT6x2x 
#define  CMU_HRCDIV                              ((uint32_t)0x0003)           /*!<hrc时钟分频设置       */
#define  CMU_HRCDIV_1                            ((uint32_t)0x0000)           /*!<分频系数 = 1          */
#define  CMU_HRCDIV_2                            ((uint32_t)0x0001)           /*!<分频系数 = 2          */
#define  CMU_HRCDIV_4                            ((uint32_t)0x0002)           /*!<分频系数 = 4          */
#define  CMU_HRCDIV_8                            ((uint32_t)0x0003)           /*!<分频系数 = 8          */
#endif

/*************************  Bit definition for CLKSTA register of HT_CMU_TypeDef ************************/
#if  defined  HT6x2x
#define  CMU_CLKSTA		                         ((uint32_t)0x003b)           /*!<MCU时钟状态寄存器       */
#elif defined HT501x  ||  defined  HT502x
#define  CMU_CLKSTA		                         ((uint32_t)0x0039)           /*!<MCU时钟状态寄存器       */
#elif defined  HT6x1x
#define  CMU_CLKSTA		                         ((uint32_t)0x001b)           /*!<MCU时钟状态寄存器       */
#endif
#define  CMU_CLKSTA_LFFLAG                       ((uint32_t)0x0001)           /*!<LF停振标志            */
#if  defined  HT6x2x  ||  defined  HT6x1x
#define  CMU_CLKSTA_LFSEL                        ((uint32_t)0x0002)           /*!<0:osc 1:mems          */
#elif defined HT501x  ||  defined  HT502x
#define  CMU_CLKSTA_LFSEL                        ((uint32_t)0x0000)           /*!<0:osc osc only        */
#endif
#define  CMU_CLKSTA_HRCFLAG                      ((uint32_t)0x0008)           /*!<HRC停振标志           */
#define  CMU_CLKSTA_PLLFLAG                      ((uint32_t)0x0010)           /*!<PLL停振标志           */
#if defined  HT6x2x   ||  defined  HT501x  ||  defined  HT502x
#define  CMU_CLKSTA_PLLLOCK                      ((uint32_t)0x0020)           /*!<PLL锁定状态标志           */
#endif
/***********************  Bit definition for SYSCLKDIV register of HT_CMU_TypeDef ***********************/  
#define  CMU_SYSCLKDIV                           ((uint32_t)0x0007)           /*!<系统时钟分频设置      */
#if  defined  HT501x || HT6x2x  ||  HT502x
#define  CMU_SYSCLKDIV_SEL20M                    ((uint32_t)0x0000)           /*!<PLL输出20M时钟        */
#define  CMU_SYSCLKDIV_SEL40M                    ((uint32_t)0x0008)           /*!<PLL输出40M时钟        */
#endif
#define  CMU_CPUCLKDIV_1                         ((uint32_t)0x0000)           /*!<分频系数 = 1          */
#define  CMU_CPUCLKDIV_2                         ((uint32_t)0x0001)           /*!<分频系数 = 2          */
#define  CMU_CPUCLKDIV_4                         ((uint32_t)0x0002)           /*!<分频系数 = 4          */
#define  CMU_CPUCLKDIV_8                         ((uint32_t)0x0003)           /*!<分频系数 = 8          */
#define  CMU_CPUCLKDIV_16                        ((uint32_t)0x0004)           /*!<分频系数 = 16         */
#define  CMU_CPUCLKDIV_32                        ((uint32_t)0x0005)           /*!<分频系数 = 32         */
#define  CMU_CPUCLKDIV_64                        ((uint32_t)0x0006)           /*!<分频系数 = 64         */
#define  CMU_CPUCLKDIV_128                       ((uint32_t)0x0007)           /*!<分频系数 = 128        */

/***********************  Bit definition for CLKOUTSEL register of HT_CMU_TypeDef ***********************/
#define  CMU_CLKOUTSEL                           ((uint32_t)0x0007)           /*!<CLKOUT输出时钟设置    */
#define  CMU_CLKOUTSEL_LRC                       ((uint32_t)0x0000)           /*!<LRC --> CLKOUT        */
#define  CMU_CLKOUTSEL_LF                        ((uint32_t)0x0001)           /*!<OSC --> CLKOUT        */
#define  CMU_CLKOUTSEL_HRC                       ((uint32_t)0x0002)           /*!<HRC --> CLKOUT        */
#define  CMU_CLKOUTSEL_SYS                       ((uint32_t)0x0003)           /*!<SYS --> CLKOUT        */
#define  CMU_CLKOUTSEL_MEMS                      ((uint32_t)0x0004)           /*!<MEMS--> CLKOUT        */

/***********************  Bit definition for CLKOUTDIV register of HT_CMU_TypeDef ***********************/
#define  CMU_CLKOUTDIV                           ((uint32_t)0x000f)           /*!<CLKOUT输出时钟设置    */
                                                                              /*!<分频系数 = 2*(N+1)    */
/************************  Bit definition for CLKCTRL0 register of HT_CMU_TypeDef ***********************/
#if   defined  HT6x1x                                                         
#define  CMU_CLKCTRL0                            ((uint32_t)0x3bfe)           /*!<CLKCTRL0寄存器        */
#elif defined  HT501x
#define  CMU_CLKCTRL0                            ((uint32_t)0xfbfe)           /*!<CLKCTRL0寄存器        */
#elif defined  HT6x2x
#define  CMU_CLKCTRL0                            ((uint32_t)0x01f3fe)         /*!<CLKCTRL0寄存器        */
#elif defined  HT502x
#define  CMU_CLKCTRL0                            ((uint32_t)0x01fffe)         /*!<CLKCTRL0寄存器        */
#endif
#define  CMU_CLKCTRL0_LCDEN                      ((uint32_t)0x0002)           /*!<LCD模块时钟使能       */
#define  CMU_CLKCTRL0_SPI0EN                     ((uint32_t)0x0004)           /*!<SPI0模块时钟使能      */
#define  CMU_CLKCTRL0_I2CEN                      ((uint32_t)0x0008)           /*!<I2C模块时钟使能       */
#define  CMU_CLKCTRL0_PLLEN                      ((uint32_t)0x0010)           /*!<PLL模块时钟使能       */
#define  CMU_CLKCTRL0_HRCEN                      ((uint32_t)0x0020)           /*!<HRC模块时钟使能       */
#if defined HT6x2x || defined  HT501x  ||  defined  HT502x
#define  CMU_CLKCTRL0_PLLLOCKEN                  ((uint32_t)0x0040)           /*!<PLL锁定使能           */
#elif	defined HT6x1x
#define  CMU_CLKCTRL0_Reserved                   ((uint32_t)0x0040)           /*!<保留为1               */
#endif
#define  CMU_CLKCTRL0_LFDETEN                    ((uint32_t)0x0080)           /*!<LF时钟检测模块使能    */
#define  CMU_CLKCTRL0_PLLDETEN                   ((uint32_t)0x0100)           /*!<PLL时钟检测模块使能   */
#define  CMU_CLKCTRL0_HRCDETEN                   ((uint32_t)0x0200)           /*!<HRC时钟检测模块使能   */

#define  CMU_CLKCTRL0_OSC_SLP                    ((uint32_t)0x0400)           /*!<OSC低功耗使能控制信号   */
#define  CMU_CLKCTRL0_OSC_SLP_LOWPOWER           ((uint32_t)0x0400)           /*!<OSC小功耗              */
#define  CMU_CLKCTRL0_OSC_SLP_LARGEPOWER         ((uint32_t)0x0000)           /*!<OSC大功耗              */

#if  defined  HT6x1x                                                         
#define  CMU_CLKCTRL0_WDTEN                      ((uint32_t)0x0800)           /*!<WDT低功耗模式下使能位 */
#elif defined HT501x  ||  defined  HT502x
#define  CMU_CLKCTRL0_KEYEN                      ((uint32_t)0x0800)           /*!<WDT低功耗模式下使能位 */
#endif
#define  CMU_CLKCTRL0_CLKOUTEN                   ((uint32_t)0x1000)           /*!<CLKOUT输出使能位      */
#define  CMU_CLKCTRL0_1P5LBOREN                  ((uint32_t)0x2000)           /*!<低功耗1P5 LBOR使能位  */
#if   defined  HT6x1x                                                        
#define  CMU_CLKCTRL0_3DESRADEN                  ((uint32_t)0x4000)           /*!<3DES/随机数模块使能位 */
#else                                                                         
#define  CMU_CLKCTRL0_ADESRADEN                  ((uint32_t)0x4000)           /*!<AES/随机数模块使能位  */
#endif
#if  defined  HT6x2x 
#define  CMU_CLKCTRL0_CRCEN                      ((uint32_t)0x8000)           /*!<CRC模块使能位         */
#elif defined HT501x  ||  HT502x
#define  CMU_CLKCTRL0_EMUEN                      ((uint32_t)0x8000)           /*!<EMU模块使能位         */
#endif

#if defined  HT502x
#define  CMU_CLKCTRL0_ECCEN                      ((uint32_t)0x010000)         /*!<ECC模块使能位         */
#elif  defined  HT6x2x
#define  CMU_CLKCTRL0_HFRTC_SEL                  ((uint32_t)0x010000)         /*!<高频补偿1HZ来源选择位  */
#define  CMU_CLKCTRL0_HFRTC_SEL_HRC              ((uint32_t)0x010000)         /*!<HRC时钟               */
#define  CMU_CLKCTRL0_HFRTC_SEL_PLL              ((uint32_t)0x000000)         /*!<PLL时钟               */
#endif

/************************  Bit definition for CLKCTRL1 register of HT_CMU_TypeDef ***********************/
#if  defined  HT6x1x                                                        
#define  CMU_CLKCTRL1                            ((uint32_t)0x03ff)           /*!<CLKCTRL1寄存器        */
#elif defined HT6x2x
#define  CMU_CLKCTRL1                            ((uint32_t)0x01ffff)         /*!<CLKCTRL1寄存器        */
#elif defined HT501x
#define  CMU_CLKCTRL1                            ((uint32_t)0xc3ff)           /*!<CLKCTRL1寄存器        */
#elif defined HT502x
#define  CMU_CLKCTRL1                            ((uint32_t)0xc3ff)           /*!<CLKCTRL1寄存器        */
#endif
#define  CMU_CLKCTRL1_TMR0EN                     ((uint32_t)0x0001)           /*!<Timer0模块时钟使能    */
#define  CMU_CLKCTRL1_TMR1EN                     ((uint32_t)0x0002)           /*!<Timer1模块时钟使能    */
#define  CMU_CLKCTRL1_TMR2EN                     ((uint32_t)0x0004)           /*!<Timer2模块时钟使能    */
#define  CMU_CLKCTRL1_TMR3EN                     ((uint32_t)0x0008)           /*!<Timer3模块时钟使能    */
#define  CMU_CLKCTRL1_UART0EN                    ((uint32_t)0x0010)           /*!<UART0模块时钟使能     */
#define  CMU_CLKCTRL1_UART1EN                    ((uint32_t)0x0020)           /*!<UART1模块时钟使能     */
#define  CMU_CLKCTRL1_UART2EN                    ((uint32_t)0x0040)           /*!<UART2模块时钟使能     */
#define  CMU_CLKCTRL1_UART3_7816_1EN             ((uint32_t)0x0080)           /*!<UART37816模块时钟使能 */
#define  CMU_CLKCTRL1_UART4_7816_0EN             ((uint32_t)0x0100)           /*!<UART47816模块时钟使能 */
#define  CMU_CLKCTRL1_UART5EN                    ((uint32_t)0x0200)           /*!<UART5模块时钟使能     */
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
#define  CMU_CLKCTRL1_TMR4EN                     ((uint32_t)0x0400)           /*!<Timer4模块时钟使能    */
#define  CMU_CLKCTRL1_TMR5EN                     ((uint32_t)0x0800)           /*!<Timer5模块时钟使能    */
#define  CMU_CLKCTRL1_UART6EN                    ((uint32_t)0x1000)           /*!<UART6模块时钟使能     */
#define  CMU_CLKCTRL1_SPI1EN                     ((uint32_t)0x2000)           /*!<SPI1模块时钟使能      */
#define  CMU_CLKCTRL1_SOFTWDTEN                  ((uint32_t)0x4000)           /*!<SoftWDT模块时钟使能   */
#define  CMU_CLKCTRL1_LRCRTCEN                   ((uint32_t)0x8000)           /*!<LRCRTC模块时钟使能    */
#elif defined HT501x  ||  defined  HT502x
#define  CMU_CLKCTRL1_LRCTFREQEN                 ((uint32_t)0x4000)           /*!<LRC测频模块使能       */
#define  CMU_CLKCTRL1_LRCRTCEN                   ((uint32_t)0x8000)           /*!<LRCRTC模块时钟使能    */
#endif

#if  defined  HT6x2x 
#define  CMU_CLKCTRL1_AUTORCCAL_EN               ((uint32_t)0x010000)         /*!<HRC/LRC自动校正使能    */
#endif

/************************  Bit definition for FLASHCON register of HT_CMU_TypeDef ***********************/
#define  CMU_FLASHCON                      	 	 ((uint32_t)0x0007)           /*!<Flash控制寄存器         */
#define  CMU_FLASHCON_FOP                        ((uint32_t)0x0003)           /*!<Flash操作模式         */
#define  CMU_FLASHCON_FOP_READ                   ((uint32_t)0x0000)           /*!<Flash处于只读模式     */
#define  CMU_FLASHCON_FOP_WRITE                  ((uint32_t)0x0001)           /*!<Flash处于写模式       */
#define  CMU_FLASHCON_FOP_PAGEERASE              ((uint32_t)0x0002)           /*!<Flash处于页擦除模式   */
#define  CMU_FLASHCON_FOP_CHIPERASE              ((uint32_t)0x0003)           /*!<Flash处于全擦除模式   */
                                                                                                        
#define  CMU_FLASHCON_BUSY                       ((uint32_t)0x0004)           /*!<Flash状态 0:空闲 1:忙 */

/************************  Bit definition for FLASHLOCK register of HT_CMU_TypeDef **********************/
#define  CMU_FLASHLOCK                           ((uint32_t)0xffff)           /*!<Flash锁定寄存器       */
#define  CMU_FLASHLOCK_UnLocked                  ((uint32_t)0x7A68)           /*!<Flash写/擦除解锁      */
#define  CMU_FLASHLOCK_Locked                    ((uint32_t)0x0000)           /*!<Flash锁定             */
                                                                              /*!<0x7A68 : Unlock       */
                                                                              /*!<other  : Lock         */
                                                                              
/************************  Bit definition for PREFETCH register of HT_CMU_TypeDef ***********************/
#if  defined  HT6x2x                                                          
#define  CMU_PREFETCH                            ((uint32_t)0x0001)           /*!<指令预取使能控制      */
#define  CMU_PREFETCH_ENABLE                     ((uint32_t)0x0001)           /*!<指令预取使能          */
#define  CMU_PREFETCH_DISABLE                    ((uint32_t)0x0000)           /*!<指令预取禁止          */
#endif

/************************  Bit definition for FLASHDLY register of HT_CMU_TypeDef ***********************/
#if  defined  HT501x  ||  defined  HT502x
#define  CMU_FLASHDLY                            ((uint32_t)0x0001)           /*!<Flash取指令等待控制   */
#define  CMU_FLASHDLY_ENABLE                     ((uint32_t)0x0001)           /*!<Flash取指令等待       */
#define  CMU_FLASHDLY_DISABLE                    ((uint32_t)0x0000)           /*!<Flash取指令不等待     */
#endif

#if  defined  HT502x
/************************  Bit definition for FLASHCON2 register of HT_CMU_TypeDef **********************/
#define  CMU_FLASHCON2                     	 	 ((uint32_t)0xffff)           /*!<Flash控制寄存器2        */
#define  CMU_FLASHCON2_FOP                       ((uint32_t)0xffff)           /*!<Flash操作模式         */
#define  CMU_FLASHCON2_FOP_READ                  ((uint32_t)0x0000)           /*!<Flash处于只读模式     */
#define  CMU_FLASHCON2_FOP_WRITE                 ((uint32_t)0xca53)           /*!<Flash处于写模式       */
#define  CMU_FLASHCON2_FOP_PAGEERASE             ((uint32_t)0xac35)           /*!<Flash处于页擦除模式   */
#endif

#if  defined  HT6x2x                                                          
/************************  Bit definition for RCCALICON register of HT_CMU_TypeDef **********************/
#define  CMU_RCCALICON			                 ((uint32_t)0x001f)           /*!<HRC校正配置寄存器   */

#define  CMU_RCCALICON_HRCAutoCali               ((uint32_t)0x0001)           /*!<HRC自动测试是使能位   */
#define  CMU_RCCALICON_LRCAutoCali               ((uint32_t)0x0002)           /*!<LRC自动测试是使能位   */
#define  CMU_RCCALICON_HRCCaliStart              ((uint32_t)0x0004)           /*!<HRC测频使能           */
#define  CMU_RCCALICON_LRCCaliStart              ((uint32_t)0x0008)           /*!<LRC测频使能           */

#define  CMU_RCCALICON_FreqSource                ((uint32_t)0x0010)           /*!<被测1Hz时钟来源选择   */
#define  CMU_RCCALICON_FreqSourceInner           ((uint32_t)0x0000)           /*!<被测1Hz时钟来源内部1Hz*/
#define  CMU_RCCALICON_FreqSourceOut             ((uint32_t)0x0010)           /*!<被测1Hz时钟来源外部1Hz*/

/************************  Bit definition for RCCALIIE register of HT_CMU_TypeDef ***********************/
#define  CMU_RCCALIIE		                    ((uint32_t)0x0003)           /*!<HRC/LRC测频中断使能    */

#define  CMU_RCCALIIE_HRCCaliIE                  ((uint32_t)0x0001)           /*!<HRC测频中断使能位     */
#define  CMU_RCCALIIE_LRCCaliIE                  ((uint32_t)0x0002)           /*!<LRC测频中断使能位     */

/************************  Bit definition for RCCALIIF register of HT_CMU_TypeDef ***********************/
#define  CMU_RCCALIIF			                 ((uint32_t)0x0003)           /*!<HRC/LRC测频中断标志位     */

#define  CMU_RCCALIIF_HRCCaliIF                  ((uint32_t)0x0001)           /*!<HRC测频中断标志位     */
#define  CMU_RCCALIIF_LRCCaliIF                  ((uint32_t)0x0002)           /*!<LRC测频中断标志位     */

/************************  Bit definition for HRCVALUE register of HT_CMU_TypeDef ***********************/
#define  CMU_HRCVALUE                            ((uint32_t)0xffffff)         /*!<HRC测频值             */

/************************  Bit definition for LRCVALUE register of HT_CMU_TypeDef ***********************/
#define  CMU_LRCVALUE                            ((uint32_t)0x1ffff)          /*!<LRC测频值             */
#endif

#if defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/*************************  Bit definition for CHIPID register of HT_CMU_TypeDef ************************/
#define  CMU_CHIPID                              ((uint32_t)0xffff)           /*!<ChipID                */
#endif

/*************************  Bit definition for INFOLOCK register of HT_CMU_TypeDef ************************/
#define  CMU_INFOLOCK                            ((uint32_t)0xffff)           /*!<Info锁定寄存器        */
#define  CMU_INFOLOCK_UnLocked                   ((uint32_t)0xf998)           /*!<Info写/擦除解锁       */
#define  CMU_INFOLOCK_Locked                     ((uint32_t)0x0000)           /*!<Info锁定              */
                                                                              /*!<0xF998 : Unlock        */
                                                                              /*!<other  : Lock          */

#if defined HT502x
/*************************  Bit definition for FS1LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS1LOCK                             ((uint32_t)0xffff)           /*!<FlashSector1锁定寄存器    */
#define  CMU_FS1LOCK_UnLocked                    ((uint32_t)0x7161)           /*!<FlashSector1写/擦除解锁   */
#define  CMU_FS1LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector1锁定          */
                                                                              /*!<0x7161 : Unlock           */
                                                                              /*!<other  : Lock             */
																																							
/*************************  Bit definition for FS2LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS2LOCK                             ((uint32_t)0xffff)           /*!<FlashSector2锁定寄存器    */
#define  CMU_FS2LOCK_UnLocked                    ((uint32_t)0x7262)           /*!<FlashSector2写/擦除解锁   */
#define  CMU_FS2LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector2锁定          */
                                                                              /*!<0x7262 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS3LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS3LOCK                             ((uint32_t)0xffff)           /*!<FlashSector3锁定寄存器    */
#define  CMU_FS3LOCK_UnLocked                    ((uint32_t)0x7363)           /*!<FlashSector3写/擦除解锁   */
#define  CMU_FS3LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector3锁定          */
                                                                              /*!<0x7363 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS4LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS4LOCK                             ((uint32_t)0xffff)           /*!<FlashSector4锁定寄存器    */
#define  CMU_FS4LOCK_UnLocked                    ((uint32_t)0x7464)           /*!<FlashSector4写/擦除解锁   */
#define  CMU_FS4LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector4锁定          */
                                                                              /*!<0x7464 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS5LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS5LOCK                             ((uint32_t)0xffff)           /*!<FlashSector5锁定寄存器    */
#define  CMU_FS5LOCK_UnLocked                    ((uint32_t)0x7565)           /*!<FlashSector5写/擦除解锁   */
#define  CMU_FS5LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector5锁定          */
                                                                              /*!<0x7565 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS6LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS6LOCK                             ((uint32_t)0xffff)           /*!<FlashSector6锁定寄存器    */
#define  CMU_FS6LOCK_UnLocked                    ((uint32_t)0x7666)           /*!<FlashSector6写/擦除解锁   */
#define  CMU_FS6LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector6锁定          */
                                                                              /*!<0x7666 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS7LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS7LOCK                             ((uint32_t)0xffff)           /*!<FlashSector7锁定寄存器    */
#define  CMU_FS7LOCK_UnLocked                    ((uint32_t)0x7767)           /*!<FlashSector7写/擦除解锁   */
#define  CMU_FS7LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector7锁定          */
                                                                              /*!<0x7767 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS8LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS8LOCK                             ((uint32_t)0xffff)           /*!<FlashSector8锁定寄存器    */
#define  CMU_FS8LOCK_UnLocked                    ((uint32_t)0x7868)           /*!<FlashSector8写/擦除解锁   */
#define  CMU_FS8LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector8锁定          */
                                                                              /*!<0x7868 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FS9LOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FS9LOCK                             ((uint32_t)0xffff)           /*!<FlashSector9锁定寄存器    */
#define  CMU_FS9LOCK_UnLocked                    ((uint32_t)0x7969)           /*!<FlashSector9写/擦除解锁   */
#define  CMU_FS9LOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSector9锁定          */
                                                                              /*!<0x7969 : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FSALOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FSALOCK                             ((uint32_t)0xffff)           /*!<FlashSectorA锁定寄存器    */
#define  CMU_FSALOCK_UnLocked                    ((uint32_t)0x7a6a)           /*!<FlashSectorA写/擦除解锁   */
#define  CMU_FSALOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSectorA锁定          */
                                                                              /*!<0x7A6A : Unlock           */
                                                                              /*!<other  : Lock             */

/*************************  Bit definition for FSBLOCK register of HT_CMU_TypeDef ************************/
#define  CMU_FSBLOCK                             ((uint32_t)0xffff)           /*!<FlashSectorB锁定寄存器    */
#define  CMU_FSBLOCK_UnLocked                    ((uint32_t)0x7b6b)           /*!<FlashSectorB写/擦除解锁   */
#define  CMU_FSBLOCK_Locked                      ((uint32_t)0x0000)           /*!<FlashSectorB锁定          */
                                                                              /*!<0x7B6B : Unlock           */
                                                                              /*!<other  : Lock             */
#endif
 
/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of WDT Module  
**********************************************************************************************************
*/  
/**************************  Bit definition for WDTCFG register of HT_WDT_TypeDef ***********************/
#if   defined  HT6x1x 
#define  WDT_WDTCFG_INTRST                       ((uint32_t)0x0001)           /*!<WDT产生复位/中断选择  */  
#define  WDT_WDTCFG_INT                          ((uint32_t)0x0001)           /*!<WDT溢出产生中断       */
#define  WDT_WDTCFG_RST                          ((uint32_t)0x0000)           /*!<WDT产生复位           */ 
#endif

/**************************  Bit definition for WDTCLR register of HT_WDT_TypeDef ***********************/
#define  WDT_WDTCLR                              ((uint32_t)0xffff)           /*!<WDT喂狗及时间配置     */

/**************************  Bit definition for WDTCNT register of HT_WDT_TypeDef ***********************/
#define  WDT_WDTCNT                              ((uint32_t)0xffff)           /*!<WDT计数寄存器         */


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of GPIO Module  
**********************************************************************************************************
*/  
/**************************  Bit definition for IOCFG register of HT_GPIO_TypeDef ***********************/
#define  GPIO_IOCFG                              ((uint32_t)0xffff)           /*!<0:GPIO 1:FunctionPin  */

/**************************  Bit definition for AFCFG register of HT_GPIO_TypeDef ***********************/
#define  GPIO_AFCFG                              ((uint32_t)0xffff)           /*!<0:Func1 1:Func2       */

/**************************  Bit definition for PTDIR register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTDIR                              ((uint32_t)0xffff)           /*!<0:In 1:Out  GPIOOnly  */

/**************************  Bit definition for PTUP register of HT_GPIO_TypeDef ************************/
#define  GPIO_PTUP                               ((uint32_t)0xffff)           /*!<0:Pull-up 1:NoPull-up*/

/**************************  Bit definition for PTDAT register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTDAT                              ((uint32_t)0xffff)           /*!<Data                  */

/**************************  Bit definition for PTSET register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTSET                              ((uint32_t)0xffff)           /*!<Write 1 to Set High   */

/**************************  Bit definition for PTCLR register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTCLR                              ((uint32_t)0xffff)           /*!<Write 1 to Set Low    */

/**************************  Bit definition for PTTOG register of HT_GPIO_TypeDef ***********************/
#define  GPIO_PTTOG                              ((uint32_t)0xffff)           /*!<Write 1 to Toggle     */

/**************************  Bit definition for PTOD register of HT_GPIO_TypeDef ************************/
#define  GPIO_PTOD                               ((uint32_t)0xffff)           /*!<0:OD 1:Push-Pull      */

/*********************  Bit definition for HT_GPIOHDPORT register of HT_GPIO_TypeDef ********************/
#define  GPIO_HDPORT                             ((uint32_t)0x000f)           /*!<大电流模式设定        */
#define  GPIO_HDPORT_PC0                         ((uint32_t)0x0001)           /*!<PC0大电流模式设定     */
#define  GPIO_HDPORT_PA6                         ((uint32_t)0x0002)           /*!<PA6大电流模式设定     */
#define  GPIO_HDPORT_PA7                         ((uint32_t)0x0004)           /*!<PA7大电流模式设定     */
#define  GPIO_HDPORT_PA8                         ((uint32_t)0x0008)           /*!<PA8大电流模式设定     */


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of INT Module  
**********************************************************************************************************
*/  
/**************************  Bit definition for EXTIE register of HT_INT_TypeDef ************************/
#define  INT_EXTIE_FIE                           ((uint32_t)0x007f)           /*!<下降沿中断使能        */
#define  INT_EXTIE_FIE_INT0                      ((uint32_t)0x0001)           /*!<INT0下降沿中断使能    */
#define  INT_EXTIE_FIE_INT1                      ((uint32_t)0x0002)           /*!<INT1下降沿中断使能    */
#define  INT_EXTIE_FIE_INT2                      ((uint32_t)0x0004)           /*!<INT2下降沿中断使能    */
#define  INT_EXTIE_FIE_INT3                      ((uint32_t)0x0008)           /*!<INT3下降沿中断使能    */
#define  INT_EXTIE_FIE_INT4                      ((uint32_t)0x0010)           /*!<INT4下降沿中断使能    */
#define  INT_EXTIE_FIE_INT5                      ((uint32_t)0x0020)           /*!<INT5下降沿中断使能    */
#define  INT_EXTIE_FIE_INT6                      ((uint32_t)0x0040)           /*!<INT6下降沿中断使能    */

#define  INT_EXTIE_RIE                           ((uint32_t)0x7f00)           /*!<上升沿中断使能        */
#define  INT_EXTIE_RIE_INT0                      ((uint32_t)0x0100)           /*!<INT0上升沿中断使能    */
#define  INT_EXTIE_RIE_INT1                      ((uint32_t)0x0200)           /*!<INT1上升沿中断使能    */
#define  INT_EXTIE_RIE_INT2                      ((uint32_t)0x0400)           /*!<INT2上升沿中断使能    */
#define  INT_EXTIE_RIE_INT3                      ((uint32_t)0x0800)           /*!<INT3上升沿中断使能    */
#define  INT_EXTIE_RIE_INT4                      ((uint32_t)0x1000)           /*!<INT4上升沿中断使能    */
#define  INT_EXTIE_RIE_INT5                      ((uint32_t)0x2000)           /*!<INT5上升沿中断使能    */
#define  INT_EXTIE_RIE_INT6                      ((uint32_t)0x4000)           /*!<INT6上升沿中断使能    */

/**************************  Bit definition for EXTIF register of HT_INT_TypeDef ************************/
#define  INT_EXTIF_FIF                           ((uint32_t)0x007f)           /*!<下降沿中断标志        */
#define  INT_EXTIF_FIF_INT0                      ((uint32_t)0x0001)           /*!<INT0下降沿中断标志    */
#define  INT_EXTIF_FIF_INT1                      ((uint32_t)0x0002)           /*!<INT1下降沿中断标志    */
#define  INT_EXTIF_FIF_INT2                      ((uint32_t)0x0004)           /*!<INT2下降沿中断标志    */
#define  INT_EXTIF_FIF_INT3                      ((uint32_t)0x0008)           /*!<INT3下降沿中断标志    */
#define  INT_EXTIF_FIF_INT4                      ((uint32_t)0x0010)           /*!<INT4下降沿中断标志    */
#define  INT_EXTIF_FIF_INT5                      ((uint32_t)0x0020)           /*!<INT5下降沿中断标志    */
#define  INT_EXTIF_FIF_INT6                      ((uint32_t)0x0040)           /*!<INT6下降沿中断标志    */

#define  INT_EXTIF_RIF                           ((uint32_t)0x7f00)           /*!<上升沿中断标志        */
#define  INT_EXTIF_RIF_INT0                      ((uint32_t)0x0100)           /*!<INT0上升沿中断标志    */
#define  INT_EXTIF_RIF_INT1                      ((uint32_t)0x0200)           /*!<INT1上升沿中断标志    */
#define  INT_EXTIF_RIF_INT2                      ((uint32_t)0x0400)           /*!<INT2上升沿中断标志    */
#define  INT_EXTIF_RIF_INT3                      ((uint32_t)0x0800)           /*!<INT3上升沿中断标志    */
#define  INT_EXTIF_RIF_INT4                      ((uint32_t)0x1000)           /*!<INT4上升沿中断标志    */
#define  INT_EXTIF_RIF_INT5                      ((uint32_t)0x2000)           /*!<INT5上升沿中断标志    */
#define  INT_EXTIF_RIF_INT6                      ((uint32_t)0x4000)           /*!<INT6上升沿中断标志    */

/**************************  Bit definition for PINFLT register of HT_INT_TypeDef ***********************/
#define	 INT_PINFLT								 ((uint32_t)0x7f7f)    		   /*!<外部引脚数字滤波寄存器 */

#define	 INT_PINFLT_INTFLT						 	 				((uint32_t)0x007f)    		   /*!<INT0-6数字滤波使能   */
#define	 INT_PINFLT_INTFLT_INT0					 				((uint32_t)0x0001)    		   /*!<INT0数字滤波使能 	  */
#define	 INT_PINFLT_INTFLT_INT1					 				((uint32_t)0x0002)    		   /*!<INT1数字滤波使能 	  */
#define	 INT_PINFLT_INTFLT_INT2					 				((uint32_t)0x0004)    		   /*!<INT2数字滤波使能 	  */
#define	 INT_PINFLT_INTFLT_INT3					 				((uint32_t)0x0008)    		   /*!<INT3数字滤波使能 	  */
#define	 INT_PINFLT_INTFLT_INT4					 				((uint32_t)0x0010)    		   /*!<INT4数字滤波使能 	  */
#define	 INT_PINFLT_INTFLT_INT5					 				((uint32_t)0x0020)    		   /*!<INT5数字滤波使能 	  */
#define	 INT_PINFLT_INTFLT_INT6					 				((uint32_t)0x0040)    		   /*!<INT6数字滤波使能 	  */

#define	 INT_PINFLT_RXFLT						 		 				((uint32_t)0x7f00)    		   /*!<RX0-6数字滤波使能   */
#define	 INT_PINFLT_RXFLT_RX0					 					((uint32_t)0x0100)    		   /*!<RX0数字滤波使能 	  */
#define	 INT_PINFLT_RXFLT_RX1					 					((uint32_t)0x0200)    		   /*!<RX1数字滤波使能 	  */
#define	 INT_PINFLT_RXFLT_RX2					 					((uint32_t)0x0400)    		   /*!<RX2数字滤波使能 	  */
#define	 INT_PINFLT_RXFLT_RX3					 					((uint32_t)0x0800)    		   /*!<RX3数字滤波使能 	  */
#define	 INT_PINFLT_RXFLT_RX4					 					((uint32_t)0x1000)    		   /*!<RX4数字滤波使能 	  */
#define	 INT_PINFLT_RXFLT_RX5					 					((uint32_t)0x2000)    		   /*!<RX5数字滤波使能 	  */
#define	 INT_PINFLT_RXFLT_RX6					 					((uint32_t)0x4000)    		   /*!<RX6数字滤波使能 	  */

#if  defined  HT6x2x  ||  defined  HT502x
/**************************  Bit definition for EXTIE2 register of HT_INT_TypeDef ***********************/
#define  INT_EXTIE2_FIE                          ((uint32_t)0x0007<<16)       /*!<下降沿中断使能        */
#define  INT_EXTIE2_FIE_INT7                     ((uint32_t)0x0001<<16)       /*!<INT7下降沿中断使能    */
#define  INT_EXTIE2_FIE_INT8                     ((uint32_t)0x0002<<16)       /*!<INT8下降沿中断使能    */
#define  INT_EXTIE2_FIE_INT9                     ((uint32_t)0x0004<<16)       /*!<INT9下降沿中断使能    */

#define  INT_EXTIE2_RIE                          ((uint32_t)0x0700<<16)       /*!<上升沿中断使能        */
#define  INT_EXTIE2_RIE_INT7                     ((uint32_t)0x0100<<16)       /*!<INT7上升沿中断使能    */
#define  INT_EXTIE2_RIE_INT8                     ((uint32_t)0x0200<<16)       /*!<INT8上升沿中断使能    */
#define  INT_EXTIE2_RIE_INT9                     ((uint32_t)0x0400<<16)       /*!<INT9上升沿中断使能    */

/**************************  Bit definition for EXTIF2 register of HT_INT_TypeDef ***********************/
#define  INT_EXTIF2_FIF                          ((uint32_t)0x0007<<16)       /*!<下降沿中断标志        */
#define  INT_EXTIF2_FIF_INT7                     ((uint32_t)0x0001<<16)       /*!<INT7下降沿中断标志    */
#define  INT_EXTIF2_FIF_INT8                     ((uint32_t)0x0002<<16)       /*!<INT8下降沿中断标志    */
#define  INT_EXTIF2_FIF_INT9                     ((uint32_t)0x0004<<16)       /*!<INT9下降沿中断标志    */

#define  INT_EXTIF2_RIF                          ((uint32_t)0x0700<<16)       /*!<上升沿中断标志        */
#define  INT_EXTIF2_RIF_INT7                     ((uint32_t)0x0100<<16)       /*!<INT7上升沿中断标志    */
#define  INT_EXTIF2_RIF_INT8                     ((uint32_t)0x0200<<16)       /*!<INT8上升沿中断标志    */
#define  INT_EXTIF2_RIF_INT9                     ((uint32_t)0x0400<<16)       /*!<INT9上升沿中断标志    */

/**************************  Bit definition for PINFLT2 register of HT_INT_TypeDef ***********************/
#define	 INT_PINFLT2							 							((uint32_t)0x0007<<16)    		/*!<外部引脚数字滤波寄存器2 */
#define	 INT_PINFLT2_INTFLT						 					((uint32_t)0x0007<<16)    		/*!<INT7-9数字滤波使能  */
#define	 INT_PINFLT2_INTFLT_INT7				 				((uint32_t)0x0001<<16)    		/*!<INT7数字滤波使能    */
#define	 INT_PINFLT2_INTFLT_INT8				 				((uint32_t)0x0002<<16)    		/*!<INT8数字滤波使能    */
#define	 INT_PINFLT2_INTFLT_INT9				 				((uint32_t)0x0004<<16)    		/*!<INT9数字滤波使能    */
#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of DMA Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/**************************  Bit definition for DMAIE register of HT_DMA_TypeDef ************************/
#define  DMA_DMAIE		                         ((uint32_t)0x0777)           /*!<DMA中断使能寄存器	 */

#define  DMA_DMAIE_TCIE0                         ((uint32_t)0x0001)           /*!<通道0传输完成中断使能 */
#define  DMA_DMAIE_TCIE1                         ((uint32_t)0x0002)           /*!<通道1传输完成中断使能 */
#define  DMA_DMAIE_TCIE2                         ((uint32_t)0x0004)           /*!<通道2传输完成中断使能 */

#define  DMA_DMAIE_BCIE0                         ((uint32_t)0x0010)           /*!<通道0半传输中断使能   */
#define  DMA_DMAIE_BCIE1                         ((uint32_t)0x0020)           /*!<通道1半传输中断使能   */
#define  DMA_DMAIE_BCIE2                         ((uint32_t)0x0040)           /*!<通道2半传输中断使能   */

#define  DMA_DMAIE_TEIE0                         ((uint32_t)0x0100)           /*!<通道0传输出错中断使能 */
#define  DMA_DMAIE_TEIE1                         ((uint32_t)0x0200)           /*!<通道1传输出错中断使能 */
#define  DMA_DMAIE_TEIE2                         ((uint32_t)0x0400)           /*!<通道2传输出错中断使能 */

/**************************  Bit definition for DMAIF register of HT_DMA_TypeDef ************************/
#define  DMA_DMAIF		                         ((uint32_t)0x0777)           /*!<DMA中断标志寄存器     */

#define  DMA_DMAIF_TCIF0                         ((uint32_t)0x0001)           /*!<通道0传输完成标志     */
#define  DMA_DMAIF_TCIF1                         ((uint32_t)0x0002)           /*!<通道1传输完成标志     */
#define  DMA_DMAIF_TCIF2                         ((uint32_t)0x0004)           /*!<通道2传输完成标志     */

#define  DMA_DMAIF_BCIF0                         ((uint32_t)0x0010)           /*!<通道0半传输完成标志   */
#define  DMA_DMAIF_BCIF1                         ((uint32_t)0x0020)           /*!<通道1半传输完成标志   */
#define  DMA_DMAIF_BCIF2                         ((uint32_t)0x0040)           /*!<通道2半传输完成标志   */

#define  DMA_DMAIF_TEIF0                         ((uint32_t)0x0100)           /*!<通道0传输出错标志     */
#define  DMA_DMAIF_TEIF1                         ((uint32_t)0x0200)           /*!<通道1传输出错标志     */
#define  DMA_DMAIF_TEIF2                         ((uint32_t)0x0400)           /*!<通道2传输出错标志     */

/**************************  Bit definition for CHNSTA register of HT_DMA_TypeDef ***********************/
#define  DMA_CHNSTA                        		 ((uint32_t)0x0007)           /*!<DMA状态寄存器         */
#define  DMA_CHNSTA_BUSY0                        ((uint32_t)0x0001)           /*!<通道0忙标志           */
#define  DMA_CHNSTA_BUSY1                        ((uint32_t)0x0002)           /*!<通道1忙标志           */
#define  DMA_CHNSTA_BUSY2                        ((uint32_t)0x0004)           /*!<通道2忙标志           */
#endif

#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/*********************  Bit definition for CHNCTL register of HT_DMA_Channel_TypeDef ********************/
#define  DMA_CHNCTL		                         ((uint32_t)0x7fff)           /*!<DMA通道控制寄存器      */

#define  DMA_CHNCTL_CHNEN                        ((uint32_t)0x0001)           /*!<DMA通道使能           */

#define  DMA_CHNCTL_PSIZE                        ((uint32_t)0x0006)           /*!<传送位数设置          */
#define  DMA_CHNCTL_PSIZE_Byte                   ((uint32_t)0x0000)           /*!<传送位数 = Byte       */
#define  DMA_CHNCTL_PSIZE_HalfWord               ((uint32_t)0x0002)           /*!<传送位数 = HalfWord   */
#define  DMA_CHNCTL_PSIZE_Word                   ((uint32_t)0x0004)           /*!<传送位数 = Word       */

#define  DMA_CHNCTL_MODE                         ((uint32_t)0x0008)           /*!<传送模式设置          */
#define  DMA_CHNCTL_MODE_WordTransfer            ((uint32_t)0x0000)           /*!<单个传输              */
#define  DMA_CHNCTL_MODE_BlockTransfer           ((uint32_t)0x0008)           /*!<块传输                */

#define  DMA_CHNCTL_CYCLE                        ((uint32_t)0x0010)           /*!<循环传输设置          */
#define  DMA_CHNCTL_CYCLE_DISABLE                ((uint32_t)0x0000)           /*!<非循环模式            */
#define  DMA_CHNCTL_CYCLE_ENABLE                 ((uint32_t)0x0010)           /*!<循环模式              */

#define  DMA_CHNCTL_SOURCEINC                    ((uint32_t)0x0060)           /*!<源地址增量模式设置    */
#define  DMA_CHNCTL_SOURCEINC_NOINC              ((uint32_t)0x0000)           /*!<不增加                */
#define  DMA_CHNCTL_SOURCEINC_INC                ((uint32_t)0x0020)           /*!<增加                  */
#define  DMA_CHNCTL_SOURCEINC_BLOCKINC           ((uint32_t)0x0040)           /*!<数据块内循环增加      */

#define  DMA_CHNCTL_DESTININC                    ((uint32_t)0x0180)           /*!<目的地址增量模式设置  */
#define  DMA_CHNCTL_DESTININC_NOINC              ((uint32_t)0x0000)           /*!<不增加                */
#define  DMA_CHNCTL_DESTININC_INC                ((uint32_t)0x0080)           /*!<增加                  */
#define  DMA_CHNCTL_DESTININC_BLOCKINC           ((uint32_t)0x0100)           /*!<数据块内循环增加      */

#define  DMA_CHNCTL_CHANNEL                      ((uint32_t)0x3e00)           /*!<通道选择              */

/*********************  Bit definition for CHNSRC register of HT_DMA_Channel_TypeDef ********************/
#define  DMA_CHNSRC                              ((uint32_t)0xffffffff)       /*!<源地址寄存器          */

/*********************  Bit definition for CHNTAR register of HT_DMA_Channel_TypeDef ********************/
#define  DMA_CHNTAR                              ((uint32_t)0xffffffff)       /*!<目的地址寄存器        */

/*********************  Bit definition for CHNCNT register of HT_DMA_Channel_TypeDef ********************/
#define  DMA_CHNCNT                              ((uint32_t)0xffff)           /*!<传输数量设置          */

/********************  Bit definition for CHNTCCNT register of HT_DMA_Channel_TypeDef *******************/
#define  DMA_CHNTCCNT                            ((uint32_t)0xffff)           /*!<已完成传输数量        */

/*******************  Bit definition for CHNBULKNUM register of HT_DMA_Channel_TypeDef ******************/
#if defined HT6x2x
#define  DMA_CHNBULKNUM                          ((uint32_t)0xffff)           /*!<数据块内数据长度设置  */
#define  DMA_CHNBULKNUM_NUM                      ((uint32_t)0x00ff)           /*!<循环次数设置 */
#define  DMA_CHNBULKNUM_CYCLE                    ((uint32_t)0xff00)           /*!<数据块内数据长度设置  */
#else
#define  DMA_CHNBULKNUM                          ((uint32_t)0x00ff)           /*!<数据块内数据长度设置  */
#define  DMA_CHNBULKNUM_NUM                      ((uint32_t)0x00ff)           /*!<循环次数设置 */
#endif
#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of CRC Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x                                                          /*!< HT6x2x               */
/*************************  Bit definition for CRCCON register of HT_CRC_TypeDef ************************/
#define  CRC_CRCCON_MODE                         ((uint32_t)0x0003)           /*!<CRC模式设置           */
#define  CRC_CRCCON_MODE_CCITT                   ((uint32_t)0x0000)           /*!<CRC-CCITT             */
#define  CRC_CRCCON_MODE_CRC16                   ((uint32_t)0x0001)           /*!<CRC-16                */
#define  CRC_CRCCON_MODE_CRC32                   ((uint32_t)0x0002)           /*!<CRC-32                */

#define  CRC_CRCCON_RESET                        ((uint32_t)0x0004)           /*!<CRC计算复位           */

/*************************  Bit definition for CRCDAT register of HT_CRC_TypeDef ************************/
#define  CRC_CRCDAT                              ((uint32_t)0xffffffff)       /*!<CRC数据寄存器         */
#endif


/*
**********************************************************************************************************
*                          Peripheral Registers_Bits_Definition of AES Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/*************************  Bit definition for AESCON register of HT_AES_TypeDef ************************/
#define  AES_AESCON_MODE                         ((uint32_t)0x0001)           /*!<AES模式设置           */
#define  AES_AESCON_MODE_ENCRYPT                 ((uint32_t)0x0000)           /*!<AES模式：加密         */
#define  AES_AESCON_MODE_DECRYPT                 ((uint32_t)0x0001)           /*!<AES模式：解密         */

#define  AES_AESCON_KEYMODE                      ((uint32_t)0x0006)           /*!<秘钥长度设置          */
#define  AES_AESCON_KEYMODE_AES128               ((uint32_t)0x0000)           /*!<秘钥长度：128bits     */
#define  AES_AESCON_KEYMODE_AES192               ((uint32_t)0x0002)           /*!<秘钥长度：192bits     */
#define  AES_AESCON_KEYMODE_AES256               ((uint32_t)0x0004)           /*!<秘钥长度：256bits     */

/*************************  Bit definition for AESSTR register of HT_AES_TypeDef ************************/
#define  AES_AESSTR                              ((uint32_t)0xffff)           /*!<AES启动寄存器         */
#define  AES_AESSTR_NormalStart                  ((uint32_t)0x8329)           /*!<普通启动              */
#define  AES_AESSTR_KeyStart                     ((uint32_t)0x8581)           /*!<KEY不变启动           */

/*************************  Bit definition for AESFLG register of HT_AES_TypeDef ************************/
#define  AES_AESFLG_BUSY                         ((uint32_t)0x0001)           /*!<AES 忙标志            */

/*************************  Bit definition for AESINLL register of HT_AES_TypeDef ***********************/
#define  AES_AESINLL                             ((uint32_t)0xffffffff)       /*!<AES输入数据的bit0-31  */

/*************************  Bit definition for AESINML register of HT_AES_TypeDef ***********************/
#define  AES_AESINML                             ((uint32_t)0xffffffff)       /*!<AES输入数据的bit32-63 */

/*************************  Bit definition for AESINHM register of HT_AES_TypeDef ***********************/
#define  AES_AESINHM                             ((uint32_t)0xffffffff)       /*!<AES输入数据的bit64-95 */

/*************************  Bit definition for AESINHH register of HT_AES_TypeDef ***********************/
#define  AES_AESINHH                             ((uint32_t)0xffffffff)       /*!<AES输入数据的bit96-127*/

/************************  Bit definition for AESOUTLL register of HT_AES_TypeDef ***********************/
#define  AES_AESOUTLL                            ((uint32_t)0xffffffff)       /*!<AES输出数据的bit0-31  */

/************************  Bit definition for AESOUTML register of HT_AES_TypeDef ***********************/
#define  AES_AESOUTML                            ((uint32_t)0xffffffff)       /*!<AES输出数据的bit32-63 */

/************************  Bit definition for AESOUTHM register of HT_AES_TypeDef ***********************/
#define  AES_AESOUTHM                            ((uint32_t)0xffffffff)       /*!<AES输出数据的bit64-95 */

/************************  Bit definition for AESOUTHH register of HT_AES_TypeDef ***********************/
#define  AES_AESOUTHH                            ((uint32_t)0xffffffff)       /*!<AES输出数据的bit96-127*/

/*************************  Bit definition for AESKEY0 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY0                             ((uint32_t)0xffffffff)       /*!<KEY Part0             */

/*************************  Bit definition for AESKEY1 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY1                             ((uint32_t)0xffffffff)       /*!<KEY Part1             */

/*************************  Bit definition for AESKEY2 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY2                             ((uint32_t)0xffffffff)       /*!<KEY Part2             */

/*************************  Bit definition for AESKEY3 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY3                             ((uint32_t)0xffffffff)       /*!<KEY Part3             */

/*************************  Bit definition for AESKEY4 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY4                             ((uint32_t)0xffffffff)       /*!<KEY Part4             */

/*************************  Bit definition for AESKEY5 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY5                             ((uint32_t)0xffffffff)       /*!<KEY Part5             */

/*************************  Bit definition for AESKEY6 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY6                             ((uint32_t)0xffffffff)       /*!<KEY Part6             */

/*************************  Bit definition for AESKEY7 register of HT_AES_TypeDef ***********************/
#define  AES_AESKEY7                             ((uint32_t)0xffffffff)       /*!<KEY Part7             */
#endif


/*
**********************************************************************************************************
*                         Peripheral Registers_Bits_Definition of RAND Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT502x                                     /*!< HT6x2x               */
/************************  Bit definition for RANDSTR register of HT_RAND_TypeDef ***********************/
#define  RAND_RANDSTR_MODESEL                    ((uint32_t)0x0003)           /*!<随机数模式选择控制    */
#define  RAND_RANDSTR_MODESEL_SAMPLE             ((uint32_t)0x0000)           /*!<低频采样高频          */
#define  RAND_RANDSTR_MODESEL_DFF                ((uint32_t)0x0001)           /*!<D触发器               */
#define  RAND_RANDSTR_MODESEL_XOR                ((uint32_t)0x0002)           /*!<低频采样高频XORD触发器*/

#define  RAND_RANDSTR_START                      ((uint32_t)0x0004)           /*!<随机数启动控制位      */
#define  RAND_RANDSTR_BUSY                       ((uint32_t)0x0008)           /*!<随机数生成忙标志      */

#define  RAND_RANDSTR_BACKMODE                   ((uint32_t)0x0300)           /*!<随机数后处理模式      */
#define  RAND_RANDSTR_BACKMODE_LFSR              ((uint32_t)0x0000)           /*!<LFSR模式              */
#define  RAND_RANDSTR_BACKMODE_PSEUDO            ((uint32_t)0x0100)           /*!<伪随机混合方式        */
#define  RAND_RANDSTR_BACKMODE_ADD               ((uint32_t)0x0200)           /*!<上述两者和方式        */

#define  RAND_RANDSTR_BACKEN                     ((uint32_t)0x0400)           /*!<随机数后处理使能      */

/************************  Bit definition for RANDDAT register of HT_RAND_TypeDef ***********************/
#define  RAND_RANDDAT                            ((uint32_t)0xffffffff)       /*!<随机数数据寄存器      */
#endif

/*
**********************************************************************************************************
*                         Peripheral Registers_Bits_Definition of GHASH Module  
**********************************************************************************************************
*/
#if  defined  HT6x2x  ||  defined  HT501x  ||  defined  HT502x
/**********************  Bit definition for GHASHSTR register of HT_GHASH_TypeDef ***********************/
#define  GHASH_GHASHSTR                          ((uint32_t)0x0501)           /*!<GHASH计算启动寄存器   */

/**********************  Bit definition for GHASHFLG register of HT_GHASH_TypeDef ***********************/
#define  GHASH_GHASHFLG_BUSY                     ((uint32_t)0x0001)           /*!<GHASH计算忙标志       */

/**********************  Bit definition for INPUT1LL register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT1LL                          ((uint32_t)0xffffffff)       /*!<数据1 bit0-31         */

/**********************  Bit definition for INPUT1ML register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT1ML                          ((uint32_t)0xffffffff)       /*!<数据1 bit32-63        */

/**********************  Bit definition for INPUT1HM register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT1HM                          ((uint32_t)0xffffffff)       /*!<数据1 bit64-95        */

/**********************  Bit definition for INPUT1HH register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT1HH                          ((uint32_t)0xffffffff)       /*!<数据1 bit96-127       */

/**********************  Bit definition for INPUT2LL register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT2LL                          ((uint32_t)0xffffffff)       /*!<数据2 bit0-31         */

/**********************  Bit definition for INPUT2ML register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT2ML                          ((uint32_t)0xffffffff)       /*!<数据2 bit32-63        */

/**********************  Bit definition for INPUT2HM register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT2HM                          ((uint32_t)0xffffffff)       /*!<数据2 bit64-95        */

/**********************  Bit definition for INPUT2HH register of HT_GHASH_TypeDef ***********************/
#define  GHASH_INPUT2HH                          ((uint32_t)0xffffffff)       /*!<数据2 bit96-127       */

/**********************  Bit definition for OUTPUTLL register of HT_GHASH_TypeDef ***********************/
#define  GHASH_OUTPUTLL                          ((uint32_t)0xffffffff)       /*!<输出数据 bit0-31      */

/**********************  Bit definition for OUTPUTML register of HT_GHASH_TypeDef ***********************/
#define  GHASH_OUTPUTML                          ((uint32_t)0xffffffff)       /*!<输出数据 bit32-63     */

/**********************  Bit definition for OUTPUTHM register of HT_GHASH_TypeDef ***********************/
#define  GHASH_OUTPUTHM                          ((uint32_t)0xffffffff)       /*!<输出数据 bit64-95     */

/**********************  Bit definition for OUTPUTHH register of HT_GHASH_TypeDef ***********************/
#define  GHASH_OUTPUTHH                          ((uint32_t)0xffffffff)       /*!<输出数据 bit96-127    */

/*********************  Bit definition for AESGHASHIE register of HT_GHASH_TypeDef **********************/
#define  GHASH_AESGHASHIE_AESIE                  ((uint32_t)0x0001)           /*!<AES计算中断使能       */
#define  GHASH_AESGHASHIE_GHASHIE                ((uint32_t)0x0002)           /*!<GHASH计算中断使能     */
#define  GHASH_AESGHASHIE_RANDIE                 ((uint32_t)0x0004)           /*!<随机数生成中断使能    */

/*********************  Bit definition for AESGHASHIF register of HT_GHASH_TypeDef **********************/
#define  GHASH_AESGHASHIF_AESIF                  ((uint32_t)0x0001)           /*!<AES计算中断标志       */
#define  GHASH_AESGHASHIF_GHASHIF                ((uint32_t)0x0002)           /*!<GHASH计算中断标志     */
#define  GHASH_AESGHASHIF_RANDIF                 ((uint32_t)0x0004)           /*!<随机数生成中断标志    */
#endif


/*
**********************************************************************************************************
*                         Peripheral Registers_Bits_Definition of KEY Module  
**********************************************************************************************************
*/
#if  defined  HT501x  ||  defined  HT502x
/************************  Bit definition for KEYIF register of HT_KEY_TypeDef ***********************/
#define  KEY_KEYIF                               ((uint32_t)0x0001)           /*!<按键标志寄存器    */

/************************  Bit definition for KEYSTA register of HT_KEY_TypeDef ***********************/
#define	 KEY_KEYSTA								 ((uint32_t)0xffff)           /*!<按键状态寄存器    */
#define	 KEY_KEYSTA_STA0						 ((uint32_t)0x0001<<0)        /*!<按键状态STA0    */
#define	 KEY_KEYSTA_STA1						 ((uint32_t)0x0001<<1)        /*!<按键状态STA1    */
#define	 KEY_KEYSTA_STA2						 ((uint32_t)0x0001<<2)        /*!<按键状态STA2    */
#define	 KEY_KEYSTA_STA3						 ((uint32_t)0x0001<<3)        /*!<按键状态STA3    */
#define	 KEY_KEYSTA_STA4						 ((uint32_t)0x0001<<4)        /*!<按键状态STA4    */
#define	 KEY_KEYSTA_STA5						 ((uint32_t)0x0001<<5)        /*!<按键状态STA5    */
#define	 KEY_KEYSTA_STA6						 ((uint32_t)0x0001<<6)        /*!<按键状态STA6    */
#define	 KEY_KEYSTA_STA7						 ((uint32_t)0x0001<<7)        /*!<按键状态STA7    */
#define	 KEY_KEYSTA_STA8						 ((uint32_t)0x0001<<8)        /*!<按键状态STA8    */
#define	 KEY_KEYSTA_STA9						 ((uint32_t)0x0001<<9)        /*!<按键状态STA9    */
#define	 KEY_KEYSTA_STA10						 ((uint32_t)0x0001<<10)       /*!<按键状态STA10    */
#define	 KEY_KEYSTA_STA11						 ((uint32_t)0x0001<<11)       /*!<按键状态STA11   */
#define	 KEY_KEYSTA_STA12						 ((uint32_t)0x0001<<12)       /*!<按键状态STA12   */
#define	 KEY_KEYSTA_STA13						 ((uint32_t)0x0001<<13)       /*!<按键状态STA13   */
#define	 KEY_KEYSTA_STA14						 ((uint32_t)0x0001<<14)       /*!<按键状态STA14   */
#define	 KEY_KEYSTA_STA15						 ((uint32_t)0x0001<<15)       /*!<按键状态STA15   */

#endif


/*
**********************************************************************************************************
*                         Peripheral Registers_Bits_Definition of ECC Module  
**********************************************************************************************************
*/
#if  defined  HT502x
/************************  Bit definition for ECCCON register of HT_ECC_TypeDef ***********************/
#define	 ECC_ECCCON								 ((uint32_t)0x007f)           /*!<ECC模块控制寄存器       */

#define	 ECC_ECCCON_OP_STR	  			 ((uint32_t)0x0001)           /*!<开始运算                */

#define	 ECC_ECCCON_OP_SEL	  			 ((uint32_t)0x001e)           /*!<运算模式选择位          */
#define	 ECC_ECCCON_OP_SEL_ECSM	  	 ((uint32_t)0x0000)           /*!<点乘运算                */
#define	 ECC_ECCCON_OP_SEL_ECA 	  	 ((uint32_t)0x0002)           /*!<点加运算                */
#define	 ECC_ECCCON_OP_SEL_ECD 	  	 ((uint32_t)0x0004)           /*!<倍加运算                */
#define	 ECC_ECCCON_OP_SEL_MA  	  	 ((uint32_t)0x0006)           /*!<模加运算                */
#define	 ECC_ECCCON_OP_SEL_MS  	  	 ((uint32_t)0x0008)           /*!<模减运算                */
#define	 ECC_ECCCON_OP_SEL_MM  	  	 ((uint32_t)0x000a)           /*!<模乘运算                */
#define	 ECC_ECCCON_OP_SEL_MD  	  	 ((uint32_t)0x000c)           /*!<模除运算                */
#define	 ECC_ECCCON_OP_SEL_MI  	  	 ((uint32_t)0x000e)           /*!<模逆运算                */
#define	 ECC_ECCCON_OP_SEL_ECDSA_S	 ((uint32_t)0x0010)           /*!<ESDA签名运算            */
#define	 ECC_ECCCON_OP_SEL_ECDSA_V	 ((uint32_t)0x0012)           /*!<ESDA认证运算            */
#define	 ECC_ECCCON_OP_SEL_PKV    	 ((uint32_t)0x0014)           /*!<公钥曲线上验证           */

#define	 ECC_ECCCON_ECCIE          	 ((uint32_t)0x0020)           /*!<ECC中断使能控制          */

#define	 ECC_ECCCON_SE            	 ((uint32_t)0x0040)           /*!<私钥选择控制位           */
#define	 ECC_ECCCON_SE_SECRETKEY     ((uint32_t)0x0040)           /*!<将Secret Key作为私钥     */
#define	 ECC_ECCCON_SE_KEYREG        ((uint32_t)0x0000)           /*!<将KEYREG寄存器值作为私钥  */

/************************  Bit definition for ECCSTA register of HT_ECC_TypeDef ***********************/
#define	 ECC_ECCSTA								 ((uint32_t)0x001f)           /*!<ECC模块状态寄存器       */

#define	 ECC_ECCSTA_ECCFLG	  			 ((uint32_t)0x0001)           /*!<ECC运算完成标志位          */
#define	 ECC_ECCSTA_BUSY  	  			 ((uint32_t)0x0002)           /*!<计算模块状态位             */
#define	 ECC_ECCSTA_ECDSA_V	  			 ((uint32_t)0x0004)           /*!<ECDSA认证标志             */
#define	 ECC_ECCSTA_ECDSA_S   			 ((uint32_t)0x0008)           /*!<ECDSA签名重试标志         */
#define	 ECC_ECCSTA_PKV    	  			 ((uint32_t)0x0010)           /*!<公钥曲线上验证标志位       */

/************************  Bit definition for PXREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_PXREG								 ((uint32_t)0xffffffff)       /*!<基点X坐标寄存器         */

/************************  Bit definition for PYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_PYREG								 ((uint32_t)0xffffffff)       /*!<基点Y坐标寄存器         */

/************************  Bit definition for KEYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_KEYREG								 ((uint32_t)0xffffffff)       /*!<私钥寄存器              */

/************************  Bit definition for AREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_AREG 								 ((uint32_t)0xffffffff)       /*!<椭圆曲线参数A寄存器         */

/************************  Bit definition for PREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_PREG 								 ((uint32_t)0xffffffff)       /*!<大素数P寄存器               */

/************************  Bit definition for RXREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_RXREG								 ((uint32_t)0xffffffff)       /*!<ECC运算输出点X坐标数据入口       */

/************************  Bit definition for RYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_RYREG								 ((uint32_t)0xffffffff)       /*!<ECC运算输出点Y坐标数据入口        */

/************************  Bit definition for SXREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_SXREG								 ((uint32_t)0xffffffff)       /*!<ECC运算第二操作数X坐标           */

/************************  Bit definition for SYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_SYREG								 ((uint32_t)0xffffffff)       /*!<ECC运算输出点Y坐标数据入口        */

/************************  Bit definition for MYREG register of HT_ECC_TypeDef ***********************/
#define	 ECC_MYREG								 ((uint32_t)0xffffffff)       /*!<HASH算法消息摘要                 */



#endif

#ifdef __cplusplus
}
#endif

#endif  /* __HT6XXX_H__ */
