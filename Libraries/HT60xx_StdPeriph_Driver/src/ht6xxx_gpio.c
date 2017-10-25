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
* File         : ht6xxx_gpio.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_GPIO_C

#include "ht6xxx_gpio.h"

/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/

#define  GPIO_Mode_AFx            0x10                      /*!< GPIO复用功能被使用             */

/*
*********************************************************************************************************
*                                             本地变量
*********************************************************************************************************
*/

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};
/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         INITIAL THE GPIO
*
* 函数说明: 配置GPIO，可一次初始化同一组GPIO中多个Pin，但其GPIO_Mode必须一致
*
* 入口参数: GPIOx              只能是HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE中的一个
*
*           GPIO_InitStruct    GPIO管脚初始化结构体指针，主要包含4个参数: 
*                              1) GPIO_Pin         : 被初始化管脚:  GPIO_Pin_0 ~ GPIO_Pin_15
*                              2) GPIO_Mode        : GPIO模式，为以下四种模式之一：
*                                                    GPIO_Mode_IOIN   IO输入
*                                                    GPIO_Mode_IOOUT  IO输出
*                                                    GPIO_Mode_AF1    复用功能1
*                                                    GPIO_Mode_AF2    复用功能2
*                              3) GPIO_InputStruct : 数字输入时上拉功能配置 
*                              4) GPIO_OutputStruct: 数字输出时开漏功能配置
*
* 返回参数: 无                                      
* 
* 特殊说明: 1)部分模拟功能比如ADCIN/Segment/Com，当管脚配置为这些功能时，配置输入上拉以及输出开漏功能无
*             意义
*           2)用户应保证在函数执行过程中寄存器写保护状态不要被改变          
*********************************************************************************************************
*/
void HT_GPIO_Init(HT_GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
    /*  assert_param  */
    
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01]; /*!< 保存当前写保护状态        */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                              /*!< 关闭写保护功能            */
    
    if(GPIO_InitStruct->GPIO_Mode & GPIO_Mode_AFx)                      /*!< 判断GPIO是否复用为功能Pin */  
    {
        GPIOx->IOCFG |= GPIO_InitStruct->GPIO_Pin;                      /*!< 配置GPIO为复用功能Pin     */ 
        
        if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_AF1)                 /*!< 判断是否为第一复用功能    */ 
        {
            GPIOx->AFCFG &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);     /*!< 配置为第一复用功能        */ 
        }
        else
        {
            GPIOx->AFCFG |= (uint32_t)GPIO_InitStruct->GPIO_Pin;        /*!< 配置为第二复用功能        */
        }
    }
    else                                                                 
    {
        GPIOx->IOCFG &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< 配置GPIO为IO              */ 
        
        if(GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IOIN)                /*!< 判断是否为GPIO输入        */
        {
            GPIOx->PTDIR &= ~((uint32_t)GPIO_InitStruct->GPIO_Pin);     /*!< 配置为IO输入              */ 
        }
        else
        {
            GPIOx->PTDIR |= (uint32_t)GPIO_InitStruct->GPIO_Pin;        /*!< 配置为IO输出              */
        }
    }
    
    
    if(GPIO_InitStruct->GPIO_InputStruct == GPIO_Input_Up)              /*!< 判断是否使能数字输入上拉  */
    {
        GPIOx->PTUP &=  ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< 配置输入上拉功能          */
    }
    else
    {
        GPIOx->PTUP |=  (uint32_t)GPIO_InitStruct->GPIO_Pin;            /*!< 关闭输入上拉功能          */
    }
    
    if(GPIO_InitStruct->GPIO_OutputStruct == GPIO_Output_OD)            /*!< 判断是否使能开漏输出功能  */
    {
        GPIOx->PTOD &=  ~((uint32_t)GPIO_InitStruct->GPIO_Pin);         /*!< 配置开漏输出              */
    }
    else
    {
        GPIOx->PTOD |=  (uint32_t)GPIO_InitStruct->GPIO_Pin;            /*!< 配置推挽输出              */
    }  

    HT_CMU->WPREG = writeProtect;                                       /*!< 恢复之前写保护设置        */    
} 


/*
*********************************************************************************************************
*                         INITIAL GPIO TO SPECIFIC ALTERNATE FUNCTION
*
* 函数说明: 配置GPIO，可一次初始化同一组GPIO中多个Pin，每个Pin GPIO_Mode可以不一致
*
* 入口参数: GPIOx              只能是HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE中的一个
*
*           GPIO_InitAFStruct  GPIO管脚初始化结构体指针，主要包含4个参数: 
*                              1) GPIO_Pin         : 被初始化管脚:  GPIO_Pin_0 ~ GPIO_Pin_15
*                              2) GPIO_AFMode      : GPIO模式（复用功能）：可为以下某一组不同Pin的复用功能组合
*                                                    注：这里只列出了GPIOA和GPIOE
*                                                    GPIOB/GPIOC/GPIOD 复用功能定义参见ht60xx_gpio.h
*                                                    PA0_SEG28           PE0_TMR0  
*                                                    PA1_SEG29           PE1_TX4   
*                                                    PA2_SEG30           PE1_TOUT2 
*                                                    PA3_SEG31           PE2_RX4   
*                                                    PA4_SEG32           PE2_ESAMIO
*                                                    PA5_INT0            PE3_CLKOUT
*                                                    PA6_INT1            PE4_RX3   
*                                                    PA7_INT2            PE4_CARDIO
*                                                    PA7_SEG33           PE5_TX3   
*                                                    PA8_INT3            PE6_TMR1  
*                                                    PA8_SEG34           PE7_LVDIN 
*                                                    PA9_INT4            PE8_POWIN 
*                                                    PA10_INT5           PE9_TMR2  
*                                                    PA11_INT6           PE10_TMR3 
*                                                    PA12_SEG35          PE11_TX2  
*                                                    PA12_ADCIN0         PE12_RX2  
*                                                    PA13_SEG36          PE13_SCL  
*                                                    PA13_ADCIN1         PE14_SDA    
*
*                              3) GPIO_InputStruct : 数字输入时上拉功能配置 
*                              4) GPIO_OutputStruct: 数字输出时开漏功能配置
*
* 返回参数: 无                                      
* 
* 特殊说明: 1)所有被初始化的管脚都将被初始化为复用功能Pin
*           2)用户应保证GPIO_Pin与GPIO_AFMode相对应，以GPIOx=HT_GPIOA为例，如果GPIO_Pin含有GPIO_Pin_0，则
              GPIO_AFMode中也应相应含有PA0_SEG28才对
*           3)部分模拟功能比如ADCIN/Segment/Com，当管脚配置为这些功能时，配置输入上拉以及输出开漏功能无
*             意义
*           4)用户应保证在函数执行过程中寄存器写保护状态不要被改变
*********************************************************************************************************
*/
void HT_GPIO_AFInit(HT_GPIO_TypeDef* GPIOx, GPIO_InitAFTypeDef* GPIO_InitAFStruct)
{
    /*  assert_param  */
    
    uint32_t AFState;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01]; /*!< 保存当前写保护状态        */
    
    HT_CMU->WPREG = CMU_WPREG_UnProtected;                              /*!< 关闭写保护功能            */
    
    GPIOx->IOCFG |= GPIO_InitAFStruct->GPIO_Pin;                        /*!< 配置为复用管脚            */
    
    AFState = GPIOx->AFCFG & (~(uint32_t)GPIO_InitAFStruct->GPIO_Pin);  /*!< 保存未配置管脚复用状态    */
    
    GPIOx->AFCFG = (uint32_t)GPIO_InitAFStruct->GPIO_AFMode | AFState;  /*!< 配置相应管脚复用功能      */
    
    if(GPIO_InitAFStruct->GPIO_InputStruct == GPIO_Input_Up)            /*!< 判断是否使能数字输入上拉  */
    {
        GPIOx->PTUP &=  ~((uint32_t)GPIO_InitAFStruct->GPIO_Pin);       /*!< 配置输入上拉功能          */
    }
    else
    {
        GPIOx->PTUP |=  (uint32_t)GPIO_InitAFStruct->GPIO_Pin;          /*!< 关闭输入上拉功能          */
    }
    
    if(GPIO_InitAFStruct->GPIO_OutputStruct == GPIO_Output_OD)          /*!< 判断是否使能开漏输出功能  */
    {
        GPIOx->PTOD &=  ~((uint32_t)GPIO_InitAFStruct->GPIO_Pin);       /*!< 配置开漏输出              */
    }
    else
    {
        GPIOx->PTOD |=  (uint32_t)GPIO_InitAFStruct->GPIO_Pin;          /*!< 配置推挽输出              */
    } 

    HT_CMU->WPREG = writeProtect;                                       /*!< 恢复之前写保护设置        */    
} 

/*
*********************************************************************************************************
*                                          SET GPIO
*
* 函数说明: 使IO输出管脚输出高电平，对复用功能Pin以及IO输入无效
*
* 入口参数: GPIOx       只能是HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE中的一个
*
*           GPIO_Pin    GPIO管脚：GPIO_Pin_0 ~ GPIO_Pin_15
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_GPIO_BitsSet(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*  assert_param  */
        
    GPIOx->PTSET = (uint32_t)GPIO_Pin;                      /*!< 使对应IO输出高         */  
} 

/*
*********************************************************************************************************
*                                          RESET GPIO
*
* 函数说明: 使IO输出管脚输出低电平，对复用功能Pin以及IO输入无效
*
* 入口参数: GPIOx       只能是HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE中的一个
*
*           GPIO_Pin    GPIO管脚：GPIO_Pin_0 ~ GPIO_Pin_15
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_GPIO_BitsReset(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*  assert_param  */
        
    GPIOx->PTCLR = (uint32_t)GPIO_Pin;                      /*!< 使对应IO输出低         */  
} 

/*
*********************************************************************************************************
*                                         TOGGLE GPIO
*
* 函数说明: 使IO输出管脚输出电平翻转，对复用功能Pin以及IO输入无效
*
* 入口参数: GPIOx       只能是HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE中的一个
*
*           GPIO_Pin    GPIO管脚：GPIO_Pin_0 ~ GPIO_Pin_15
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_GPIO_BitsToggle(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*  assert_param  */
        
    GPIOx->PTTOG = (uint32_t)GPIO_Pin;                      /*!< 翻转对应IO口电平       */  
} 

/*
*********************************************************************************************************
*                                      READ GPIO BITS
*
* 函数说明: 读特定端口的Pin的状态
*
* 入口参数: GPIOx       只能是HT_GPIOA/HT_GPIOB/HT_GPIOC/HT_GPIOD/HT_GPIOE中的一个
*
*           GPIO_Pin    GPIO管脚：GPIO_Pin_0 ~ GPIO_Pin_15
*                       当GPIO_Pin = GPIO_Pin_All 时，为读整个端口IO输入Pin值
*
* 返回参数: 端口所读Pin的状态                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
uint16_t HT_GPIO_BitsRead(HT_GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    /*  assert_param  */
        
    return (uint16_t)(GPIOx->PTDAT & GPIO_Pin);             /*!< 读对应IO口电平         */  
} 

/*
*********************************************************************************************************
*                                 SET GPIO ON HIGH CURRENT MODE 
*
* 函数说明: 设置部分GPIO超大电流模式
*
* 入口参数: GPIO_Pin    大电流模式设置Bit，可以为以下参数或其组合:
*                         @arg  GPIO_HDPORT_PC0
*                         @arg  GPIO_HDPORT_PA6
*                         @arg  GPIO_HDPORT_PA7
*                         @arg  GPIO_HDPORT_PA8
*
*           NewState    = ENABLE： 使能大电流模式
*                       = DISABLE：普通电流模式
*
* 返回参数: 无                                     
* 
* 特殊说明: 只有PC0/PA6/PA7/PA8有效
*********************************************************************************************************
*/
void HT_GPIO_HighCurrentSet(uint16_t GPIO_Pin, FunctionalState NewState)
{
    /*  assert_param  */
        
    if (NewState != DISABLE)
    {       
        HT_GPIOHDPORT |= (uint32_t)(GPIO_Pin & GPIO_HDPORT);    /*!< 使能GPIO大电流模式     */ 
    }
    else
    {
        HT_GPIOHDPORT &= ~(uint32_t)(GPIO_Pin & GPIO_HDPORT);  /*!< 关闭GPIO大电流模式     */ 
    }   
    
} 


