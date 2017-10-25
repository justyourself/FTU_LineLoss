/*
*********************************************************************************************************
*                                              HT6XXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx_uart&7816.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_UART7816_C

#include "ht6xxx_uart&7816.h"
#include "ht6xxx_cmu.h"

/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                             本地变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         INITIAL UART MODULE
*
* 函数说明: 初始化UART模块
*
* 入口参数: UARTx               只能是HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5中一个
*
*           UART_InitStruct     UART初始化结构体指针，主要包含7个参数: 
*                               1) UART_Logic      : UART通讯逻辑设定
*                               2) UART_StopBits   : UART通讯停止位设定
*                               3) UART_WordLength : UART通讯数据位长度设定
*                               4) UART_Parity     : UART通讯奇偶校验设定 
*                               5) UART_BaudRate   : UART通讯波特率
*                               6) ReceiveEN       : UART接收使能控制
*                               7) SendEN          : UART发送使能控制
*
* 返回参数: 无                                      
* 
* 特殊说明: 1) HT_UART3/HT_UART4与ISO7816功能复用，如果这边有对HT_UART3/HT_UART4初始化，则不能再使用
*              HT_ISO7816_0/HT_ISO7816_1功能，其对应关系为：
*              HT_UART3 <----->HT_ISO7816_1             
*              HT_UART4 <----->HT_ISO7816_0
*           2) 由于涉及到波特率计算，所以用户在调用此函数时应先配置好系统时钟。否则如果已经配置好波特率，
*              再改变系统时钟，则波特率配置将失效
*           3) 用户在配置UART寄存器前应先使能UART模块，具体参见HT_CMU_ClkCtrl1Config()函数
*********************************************************************************************************
*/
void HT_UART_Init(HT_UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg,sysclk;
    
    UARTx->MODESEL = UART_MODESEL_UART;                                   /*!< 选择为UART功能         */
                                                                          /*!< 只对Uart3/4有效        */

    tempreg = UARTx->UARTCON & (UART_UARTCON_TXIE | UART_UARTCON_RXIE);   /*!< 保留TXIE，RXIE不变     */
    
    if(UART_InitStruct->UART_Parity != UartParity_Disable )
    {
        tempreg |= UART_UARTCON_PARITYEN;
        tempreg |= UART_InitStruct->UART_Parity;                          /*!< 配置奇偶校验位         */
    }
    tempreg |= UART_InitStruct->UART_Logic;                               /*!< 配置UART通讯逻辑       */
    tempreg |= UART_InitStruct->UART_StopBits;                            /*!< 配置停止位             */
    tempreg |= UART_InitStruct->UART_WordLength;                          /*!< 配置通讯数据长度       */
    
    if(UART_InitStruct->ReceiveEN != DISABLE)
    {
        tempreg |= UART_UARTCON_RXEN;                                     /*!< 配置接收使能           */
    }
    if(UART_InitStruct->SendEN != DISABLE)
    {
        tempreg |= UART_UARTCON_TXEN;                                     /*!< 配置发送使能           */
    }
    
    UARTx->UARTCON = tempreg;                                             /*!< 配置寄存器             */   
        
    sysclk = HT_CMU_SysClkGet()>>1;                                       /*!< 获取当前系统时钟频率/2 */
    UARTx->SREL = sysclk/UART_InitStruct->UART_BaudRate-1;                /*!< 计算波特率             */
    
    UARTx->IRCON &= ~UART_IRCON_IRTX;                                     /*!< 关闭红外调制输出功能   */
} 

/*
*********************************************************************************************************
*                                         INITIAL IR MODULE
*
* 函数说明: 初始化红外模块
*
* 入口参数: UARTx               只能是HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5中一个
*
*           IR_InitStruct       IR模块初始化结构体指针，主要包含3个参数: 
*                               1) UART_Module   : UART通讯模块初始化，其为UART_InitTypeDef类型结构体
*                               2) IR_Polarity   : 红外调制输出极性配置 
*                               3) IR_Duty       : 红外调制波形占空比设置
*
* 返回参数: 无                                      
* 
* 特殊说明: 1) HT_UART3/HT_UART4与ISO7816功能复用，如果这边有对HT_UART3/HT_UART4初始化，则不能再使用
*              HT_ISO7816_0/HT_ISO7816_1功能，其对应关系为：
*              HT_UART3 <----->HT_ISO7816_1             
*              HT_UART4 <----->HT_ISO7816_0
*           2) 由于涉及到波特率计算，所以用户在调用此函数时应先配置好系统时钟。否则如果已经配置好波特率，
*              再改变系统时钟，则波特率配置将失效
*           3) 用户在配置UART寄存器前应先使能UART模块，具体参见HT_CMU_ClkCtrl1Config()函数
*********************************************************************************************************
*/
void HT_IR_Init(HT_UART_TypeDef* UARTx, IR_InitTypeDef* IR_InitStruct)
{
    /*  assert_param  */
    
    HT_UART_Init(UARTx, &IR_InitStruct->UARTModule);                    /*!< 初始化Uart模块           */
    
    UARTx->IRCON = (IR_InitStruct->IRPolarity | UART_IRCON_IRTX);       /*!< 使能红外以及调制极性配置 */
    
    UARTx->IRDUTY = IR_InitStruct->IRDuty;                              /*!< 配置占空比               */
} 

/*
*********************************************************************************************************
*                                      INITIAL ISO7816 MODULE
*
* 函数说明: 初始化ISO7816模块
*
* 入口参数: ISO7816x            只能是HT_ISO7816_0/HT_ISO7816_1中一个
*
*           ISO7816_InitStruct  ISO7816初始化结构体指针，主要包含6个参数: 
*                               1) ISO7816_Parity     : ISO7816奇偶校验位选择
*                               2) ISO7816_RepeatTime : ISO7816重发重收次数设定
*                               3) AutoRxEN           : ISO7816自动接收使能控制
*                               4) AutoTxEN           : ISO7816自动发送使能控制
*                               5) ISO7816_AckLen     : ISO7816响应位长度设定
*                               6) ISO7816_BaudRate   : ISO7816波特率设定
*
* 返回参数: 无                                      
* 
* 特殊说明: 1) HT_UART3/HT_UART4与ISO7816功能复用，如果这边有对HT_ISO7816_0/HT_ISO7816_1初始化，则不能再使用
*              HT_UART3/HT_UART4功能，其对应关系为：
*              HT_UART3 <----->HT_ISO7816_1             
*              HT_UART4 <----->HT_ISO7816_0 
*           2) 由于涉及到波特率计算，所以用户在调用此函数时应先配置好系统时钟。否则如果已经配置好波特率，
*              再改变系统时钟，则波特率配置将失效
*           3) 用户在配置ISO7816寄存器前应先使能ISO7816模块，具体参见HT_CMU_ClkCtrl1Config()函数
*********************************************************************************************************
*/
void HT_ISO7816_Init(HT_ISO7816_TypeDef* ISO7816x, ISO7816_InitTypeDef* ISO7816_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg, sysclk;
    
    ISO7816x->MODESEL = UART_MODESEL_7816;                                   /*!< 选择为ISO7816功能      */
    
    tempreg = ISO7816x->ISO7816CON & UART_ISO7816CON_IEALL;                  /*!< 保留中断使能不变       */
    
    if(ISO7816_InitStruct->AutoRxEN != DISABLE)
    {
        tempreg |= UART_ISO7816CON_AUTORXEN;                                 /*!< 使能自动重收功能       */ 
    }
    if(ISO7816_InitStruct->AutoTxEN != DISABLE)
    {
        tempreg |= UART_ISO7816CON_AUTOTXEN;                                 /*!< 使能自动重发功能       */ 
    }    
    
    tempreg |= ISO7816_InitStruct->ISO7816_Parity;                           /*!< 配置奇偶性             */
    tempreg |= ISO7816_InitStruct->ISO7816_RepeatTime;                       /*!< 配置重发重收次数       */
    tempreg |= ISO7816_InitStruct->ISO7816_AckLen;                           /*!< 配置响应位长度         */
    
    ISO7816x->ISO7816CON = tempreg;                                          /*!< 配置寄存器             */ 
        
    sysclk = HT_CMU_SysClkGet()>>1;                                          /*!< 获取当前系统时钟频率/2 */
    ISO7816x->SREL = sysclk/ISO7816_InitStruct->ISO7816_BaudRate-1;          /*!< 计算波特率             */
    
    ISO7816x->IRCON &= ~UART_IRCON_IRTX;                                     /*!< 关闭红外调制输出功能   */
} 

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE UART INTERRUPT    
*
* 函数说明: 使能或者关闭UART中断
*
* 入口参数: UARTx      只能是HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5中一个
*
            ITEn       UART中断设置位，可以为以下参数或其组合
*                        @arg UART_UARTCON_TXIE
*                        @arg UART_UARTCON_RXIE
*
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_UART_ITConfig(HT_UART_TypeDef* UARTx, uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    uint32_t it;
    
    it = (uint32_t)ITEn & (UART_UARTCON_RXIE | UART_UARTCON_TXIE);
    
    if (NewState != DISABLE)
    {       
        UARTx->UARTCON |= it;                                             /*!< 使能UART中断           */
    }
    else
    {
        UARTx->UARTCON &= ~it;                                            /*!< 关闭UART中断           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED UART INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应UART中断标志
*
* 入口参数: UARTx      只能是HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5中一个
*
*           ITFlag     想要清除的某个UART中断，可以为以下参数:
*                        @arg UART_UARTSTA_TXIF
*                        @arg UART_UARTSTA_RXIF
*                        @arg UART_UARTSTA_PARITY    注：此标志位不是中断标志位
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
* 
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_UART_ITFlagStatusGet(HT_UART_TypeDef* UARTx, uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (UARTx->UARTSTA & ITFlag)
    {       
        return SET;                                            /*!< UART Interrupt Flag is set   */
    }
    else
    {
        return RESET;                                          /*!< UART Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                CLEAR UART INTERRUPT FLAG
*
* 函数说明: 清除UART中断标志
*
* 入口参数: UARTx      只能是HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5中一个
*
*           ITFlag     想要清除的某个UART中断标志，可以为以下参数或其组合:
*                        @arg UART_UARTSTA_TXIF
*                        @arg UART_UARTSTA_RXIF
*                        @arg UART_UARTSTA_PARITY    注：此标志位不是中断标志位
*
* 返回参数: 无
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_UART_ClearITPendingBit(HT_UART_TypeDef* UARTx, uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    UARTx->UARTSTA &= ~((uint32_t)ITFlag);                /*!< Clear UART Interrupt Flag    */
    
}


/*
*********************************************************************************************************
*                              ENABLE OR DISABLE ISO7816 INTERRUPT    
*
* 函数说明: 使能或者关闭ISO7816中断
*
* 入口参数: ISO7816x  只能是HT_ISO7816_0/HT_ISO7816_1中一个
*
*           ITEn       ISO7816中断设置位，可以为以下参数或其组合
*                        @arg UART_ISO7816CON_TXIE
*                        @arg UART_ISO7816CON_RXIE
*                        @arg UART_ISO7816CON_PRDIE
*
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_ISO7816_ITConfig(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    uint32_t it;
    
    it = (uint32_t)ITEn & UART_ISO7816CON_IEALL;
    
    if (NewState != DISABLE)
    {       
        ISO7816x->ISO7816CON |= it;                                   /*!< 使能ISO7816中断         */
    }
    else
    {
        ISO7816x->ISO7816CON &= ~it;                                  /*!< 关闭ISO7816中断         */
    } 
}

/*
*********************************************************************************************************
*                         GET SPECIFIED ISO7816 INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应ISO7816中断标志状态
*
* 入口参数: ISO7816x  只能是HT_ISO7816_0/HT_ISO7816_1中一个
*
*           ITFlag     想要检查的某个ISO7816中断，可以为以下参数:
*                        @arg UART_ISO7816STA_TXIF
*                        @arg UART_ISO7816STA_RXIF
*                        @arg UART_ISO7816STA_PRDIF
*                        @arg UART_ISO7816STA_TXERRSTAT    注：此标志位不是中断标志位
*                        @arg UART_ISO7816STA_RXERRSTAT    注：此标志位不是中断标志位
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
* 
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_ISO7816_ITFlagStatusGet(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (ISO7816x->ISO7816STA & ITFlag)
    {       
        return SET;                                        /*!< ISO7816 Interrupt Flag is set   */
    }
    else
    {
        return RESET;                                      /*!< ISO7816 Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                CLEAR ISO7816 INTERRUPT FLAG
*
* 函数说明: 清除ISO7816中断标志
*
* 入口参数: ISO7816x  只能是HT_ISO7816_0/HT_ISO7816_1中一个
*
*           ITFlag     想要清除的某个ISO7816中断标志，可以为以下参数或其组合:
*                        @arg UART_ISO7816STA_TXIF
*                        @arg UART_ISO7816STA_RXIF
*                        @arg UART_ISO7816STA_PRDIF
*                        @arg UART_ISO7816STA_TXERRSTAT    注：此标志位不是中断标志位
*                        @arg UART_ISO7816STA_RXERRSTAT    注：此标志位不是中断标志位
*
* 返回参数: 无
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_ISO7816_ClearITPendingBit(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    ISO7816x->ISO7816STA &= ~((uint32_t)ITFlag);             /*!< Clear ISO7816 Interrupt Flag    */
    
}

/*
*********************************************************************************************************
*                                        WRITE DATA TO UART BUFFER
*
* 函数说明: 往Uart buffer中写数据
*
* 入口参数: UARTx      只能是HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5中一个
*
*           Data       往Uart Buffer中要写的数据
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_UART_SendData(HT_UART_TypeDef* UARTx, uint16_t Data)
{
    /*  assert_param  */
    
    
    UARTx->SBUF = Data;
}

/*
*********************************************************************************************************
*                                         READ UART BUFFER
*
* 函数说明: 读Uart中数据
*
* 入口参数: UARTx      只能是HT_UART0/HT_UART1/HT_UART2/HT_UART3/HT_UART4/HT_UART5中一个
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
uint8_t HT_UART_ReceiveData(HT_UART_TypeDef* UARTx)
{
    /*  assert_param  */
        
    return UARTx->SBUF;
}

/*
*********************************************************************************************************
*                                        WRITE DATA TO ISO7816 BUFFER
*
* 函数说明: 往ISO7816 buffer中写数据
*
* 入口参数: ISO7816x   只能是HT_ISO7816_0/HT_ISO7816_1中一个
*
*           Data       往ISO7816 Buffer中要写的数据
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_ISO7816_SendData(HT_ISO7816_TypeDef* ISO7816x, uint8_t Data)
{
    /*  assert_param  */
    
    
    ISO7816x->SBUF = Data;
}

/*
*********************************************************************************************************
*                                         READ ISO7816 BUFFER
*
* 函数说明: 读ISO7816中数据
*
* 入口参数: ISO7816x   只能是HT_ISO7816_0/HT_ISO7816_1中一个
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
uint8_t HT_ISO7816_ReceiveData(HT_ISO7816_TypeDef* ISO7816x)
{
    /*  assert_param  */
    
    
    return ISO7816x->SBUF;
}

/*
*********************************************************************************************************
*                                   ENABLE OR DISABLE RX DIGITAL FILTER
*
* 函数说明: 使能或关闭外部中断的数字滤波功能
*
* 入口参数: RXFLTEn     中断引脚数字滤波设置位，可以为以下参数或其组合
*                        @arg INT_PINFLT_RXFLT_RX0
*                        @arg INT_PINFLT_RXFLT_RX1
*                        @arg INT_PINFLT_RXFLT_RX2
*                        @arg INT_PINFLT_RXFLT_RX3
*                        @arg INT_PINFLT_RXFLT_RX4
*                        @arg INT_PINFLT_RXFLT_RX5
*                        @arg INT_PINFLT_RXFLT_RX6
*
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_UART_RXFilterConfig(uint32_t RXFLTEn,FunctionalState NewState)
{
    /*  assert_param  */
  
    RXFLTEn &= INT_PINFLT_RXFLT;  
    if (NewState != DISABLE)
    {       
        HT_INT->PINFLT |= RXFLTEn;            /*!< 使能数字滤波         */
    }
    else
    {
        HT_INT->PINFLT &= ~RXFLTEn;           /*!< 关闭数字滤波         */
    }    
}
