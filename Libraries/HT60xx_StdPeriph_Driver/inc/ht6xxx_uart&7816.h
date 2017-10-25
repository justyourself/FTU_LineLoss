/*
**********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht6xxx_uart&7816.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
**********************************************************************************************************
*/

#ifndef __HT6XXX_UART7816_H__
#define __HT6XXX_UART7816_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
    
/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/

/* 
* @brief  UART通讯逻辑定义
*/
typedef enum
{ 
    UartLogicPositive = UART_UARTCON_UNEG_POSITIVE,     /*!< UART通讯选择正逻辑             */
    UartLogicNegetive = UART_UARTCON_UNEG_NEGETIVE,     /*!< UART通讯选择负逻辑             */
#if defined  HT6x2x
    UartLogicAuto = UART_UARTCON_UNEG_AUTO,             /*!< UART通讯选择接收逻辑自动判断   */
#endif   
}UARTLogic_TypeDef;                                     /*!< end of group UARTLogic_TypeDef */   

/* 
* @brief  UART停止位定义
*/
typedef enum
{ 
    OneStopBits = UART_UARTCON_STOPSEL_1Bit,            /*!< 1位停止位                         */
    TwoStopBits = UART_UARTCON_STOPSEL_2Bit,            /*!< 2位停止位                         */
}UARTStopBits_TypeDef;                                  /*!< end of group UARTStopBits_TypeDef */

/* 
* @brief  UART数据长度位定义
*/
typedef enum
{ 
    WordLength_7Bits = UART_UARTCON_LENSEL_7Bit,        /*!< 7位数据位                          */
    WordLength_8Bits = UART_UARTCON_LENSEL_8Bit,        /*!< 8位数据位                          */
}UARTWordLength_TypeDef;                                /*!< end of group UARTWordLength_TypeDef*/

/* 
* @brief  UART奇偶校验位定义
*/
typedef enum
{ 
    UartParity_Disable = 0xff,                          /*!< 没有奇偶校验位                     */
    UartParity_Always1 = UART_UARTCON_PARITYSEL_1,      /*!< 奇偶校验位恒定为1                  */
    UartParity_Always0 = UART_UARTCON_PARITYSEL_0,      /*!< 奇偶校验位恒定为0                  */
    UartParity_ODD = UART_UARTCON_PARITYSEL_ODD,        /*!< 奇校验                             */
    UartParity_EVEN = UART_UARTCON_PARITYSEL_EVEN,      /*!< 偶校验                             */
}UARTParity_TypeDef;                                    /*!< end of group UARTParity_TypeDef    */


/* 
* @brief  UART模块初始化结构定义
*/
typedef struct
{
    UARTLogic_TypeDef  UART_Logic;                      /*!< 串口逻辑设置                       */                                                                          
    UARTStopBits_TypeDef  UART_StopBits;                /*!< 串口停止位设置                     */                                                                         
    UARTWordLength_TypeDef  UART_WordLength;            /*!< 串口通讯数据长度设置               */ 
    UARTParity_TypeDef  UART_Parity;                    /*!< 串口奇偶校验位设置                 */
    uint32_t  UART_BaudRate;                            /*!< 串口波特率, 比如1200, 2400         */
    FunctionalState  ReceiveEN;                         /*!< 串口接收使能控制                   */ 
    FunctionalState  SendEN;                            /*!< 串口发送使能控制                   */ 
}UART_InitTypeDef;                                      /*!< end of group UART_InitTypeDef      */

/* 
* @brief  红外调制输出极性定义
*/
typedef enum
{ 
    IRPolarityPositive = UART_IRCON_IRLVL_POSITIVE,     /*!< 红外调制输出极性为正               */
    IRPolarityNegetive = UART_IRCON_IRLVL_NEGETIVE,     /*!< 红外调制输出极性为负               */
}IRPolarity_TypeDef;                                    /*!< end of group IRPolarity_TypeDef    */

/* 
* @brief  红外调制输出极性定义
*/
typedef enum
{ 
    IRDuty50 = UART_IRDUTY_IRDUTY_50,                   /*!< 红外调制波形占空比50%              */
    IRDuty25 = UART_IRDUTY_IRDUTY_25,                   /*!< 红外调制波形占空比25%              */
    IRDuty12P5 = UART_IRDUTY_IRDUTY_12P5,               /*!< 红外调制波形占空比12.5%            */
    IRDuty6P25 = UART_IRDUTY_IRDUTY_6P25,               /*!< 红外调制波形占空比6.25%            */
}IRDuty_TypeDef;                                        /*!< end of group IRDuty_TypeDef        */

/* 
* @brief  红外模块初始化结构定义
*/
typedef struct
{
    UART_InitTypeDef  UARTModule;                        /*!< UART通讯初始化                     */                                                                          
    IRPolarity_TypeDef  IRPolarity;                      /*!< 红外调制输出极性配置               */                                                                         
    IRDuty_TypeDef  IRDuty;                              /*!< 红外调制波形占空比设置             */ 
}IR_InitTypeDef;                                         /*!< end of group IR_InitTypeDef        */

/* 
* @brief  ISO7816奇偶校验位定义
*/
typedef enum
{ 
    ISO7816Parity_ODD = UART_ISO7816CON_7816PARITY_ODD,  /*!< 奇校验                             */
    ISO7816Parity_EVEN = UART_ISO7816CON_7816PARITY_EVEN,/*!< 偶校验                             */
}ISO7816Parity_TypeDef;                                  /*!< end of group ISO7816Parity_TypeDef */

/* 
* @brief  ISO7816自动重发重收次数定义
*/
typedef enum
{ 
    Repeat_Zero = UART_ISO7816CON_REPTR_0,               /*!< 自动重发重收次数设为0              */
    Repeat_One = UART_ISO7816CON_REPTR_1,                /*!< 自动重发重收次数设为1              */
    Repeat_Two = UART_ISO7816CON_REPTR_2,                /*!< 自动重发重收次数设为2              */
    Repeat_Three = UART_ISO7816CON_REPTR_3,              /*!< 自动重发重收次数设为3              */
}ISO7816Repeat_TypeDef;                                  /*!< end of group ISO7816Repeat_TypeDef */

/* 
* @brief  ISO7816响应位长度定义
*/
typedef enum
{ 
    ACKLen_1Bit = UART_ISO7816CON_ACKLEN_1Bit,            /*!< ACK响应长度1Bit                   */
    ACKLen_1P5Bit = UART_ISO7816CON_ACKLEN_1P5Bit,        /*!< ACK响应长度1.5Bit                 */
    ACKLen_2Bit = UART_ISO7816CON_ACKLEN_2Bit,            /*!< ACK响应长度2Bit                   */
}ISO7816ACKLen_TypeDef;                                   /*!< end of group ISO7816ACKLen_TypeDef*/    

/* 
* @brief  ISO7816模块初始化结构定义
*/
typedef struct
{
    ISO7816Parity_TypeDef  ISO7816_Parity;                /*!< ISO7816奇偶设定                   */                                                                          
    ISO7816Repeat_TypeDef  ISO7816_RepeatTime;            /*!< ISO7816重发重收次数设定           */
    FunctionalState  AutoRxEN;                            /*!< 自动重收功能设定                  */ 
    FunctionalState  AutoTxEN;                            /*!< 自动重发功能设定                  */ 
    ISO7816ACKLen_TypeDef  ISO7816_AckLen;                /*!< ISO7816响应长度设定               */ 
    uint32_t  ISO7816_BaudRate;                           /*!< ISO7816波特率设定，比如9600       */ 
}ISO7816_InitTypeDef;                                     /*!< end of group ISO7816_InitTypeDef  */


/*
*********************************************************************************************************
*                                             全局变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           全局函数申明
*********************************************************************************************************
*/
void HT_UART_Init(HT_UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);
void HT_IR_Init(HT_UART_TypeDef* UARTx, IR_InitTypeDef* IR_InitStruct);
void HT_ISO7816_Init(HT_ISO7816_TypeDef* ISO7816x, ISO7816_InitTypeDef* ISO7816_InitStruct);
void HT_UART_ITConfig(HT_UART_TypeDef* UARTx, uint8_t ITEn, FunctionalState NewState);
ITStatus HT_UART_ITFlagStatusGet(HT_UART_TypeDef* UARTx, uint8_t ITFlag);
void HT_UART_ClearITPendingBit(HT_UART_TypeDef* UARTx, uint8_t ITFlag);
void HT_ISO7816_ITConfig(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITEn, FunctionalState NewState);
ITStatus HT_ISO7816_ITFlagStatusGet(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITFlag);
void HT_ISO7816_ClearITPendingBit(HT_ISO7816_TypeDef* ISO7816x, uint8_t ITFlag);
void HT_UART_SendData(HT_UART_TypeDef* UARTx, uint16_t Data);
uint8_t HT_UART_ReceiveData(HT_UART_TypeDef* UARTx);
void HT_ISO7816_SendData(HT_ISO7816_TypeDef* ISO7816x, uint8_t Data);
uint8_t HT_ISO7816_ReceiveData(HT_ISO7816_TypeDef* ISO7816x);
void HT_UART_RXFilterConfig(uint32_t RXFLTEn,FunctionalState NewState);






#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_UART7816_H__ */ 
