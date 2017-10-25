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
* File         : ht6xxx_spi.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_SPI_C

#include "ht6xxx_spi.h"

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
*                                 INITIAL SPI MODULE
*
* 函数说明: 初始化SPI模块
*
* 入口参数:  SPIx               只能是HT_SPI0/HT_SPI1中一个
*            SPI_InitStruct     SPI模块初始化结构体指针，主要包含5个参数: 
*                               1) SPI_CSInCtrol : 是否打开SPI_CS输入 
*                               2) SPI_Baudrate  : SPI时钟设置
*                               3) SPI_CPHA      : 时钟相位设置
*                               4) SPI_CPOL      : 时钟极性设置        
*                               5) SPI_Mode      : SPI主从模式设置    
*
* 返回参数: 无                                      
* 
* 特殊说明: 用户在配置SPI寄存器前应先使能SPI模块，具体参见HT_CMU_ClkCtrl0Config()函数
*********************************************************************************************************
*/
void HT_SPI_Init(HT_SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg = 0x81;
    
    if(SPI_InitStruct->SPI_CSInCtrol != DISABLE)
    {
        tempreg &= ~SPI_SPICON_SSDIS;                               /*!< 打开SPI_CS输入          */
    }
    
    tempreg |= SPI_InitStruct->SPI_Baudrate;                        /*!< 配置时钟频率            */
    tempreg |= SPI_InitStruct->SPI_CPHA;                            /*!< 配置相位                */
    tempreg |= SPI_InitStruct->SPI_CPOL;                            /*!< 配置时钟极性            */
    tempreg |= SPI_InitStruct->SPI_Mode;                            /*!< 配置主从模式            */
    
    SPIx->SPICON = tempreg;
  
} 

/*
*********************************************************************************************************
*                            GET SPECIFIED SPI INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应SPI中断标志状态
*
* 入口参数: SPIx       只能是HT_SPI0/HT_SPI1中一个
*           ITFlag     想要检查的某个SPI中断，可以为以下参数:
*                        @arg SPI_SPISTA_MODF
*                        @arg SPI_SPISTA_SSERR   注：此标志位不是中断标志位
*                        @arg SPI_SPISTA_WCOL    注：此标志位不是中断标志位
*                        @arg SPI_SPISTA_SPIF
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
* 
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_SPI_ITFlagStatusGet(HT_SPI_TypeDef* SPIx, uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (SPIx->SPISTA & ITFlag)
    {       
        return SET;                              /*!< Interrupt Flag is set   */
    }
    else
    {
        return RESET;                            /*!< Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR SPI INTERRUPT FLAG
*
* 函数说明: 清除SPI中断标志
*
* 入口参数:     SPIx       只能是HT_SPI0/HT_SPI1中一个
*               ITFlag     想要清除的某个SPI中断标志，可以为以下参数或其组合:
*                        @arg SPI_SPISTA_MODF
*                        @arg SPI_SPISTA_SSERR   注：此标志位不是中断标志位
*                        @arg SPI_SPISTA_WCOL    注：此标志位不是中断标志位
*                        @arg SPI_SPISTA_SPIF
*
* 返回参数: 无
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_SPI_ClearITPendingBit(HT_SPI_TypeDef* SPIx, uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    SPIx->SPISTA  &= ~((uint32_t)ITFlag);               /*!< Clear SPI Interrupt Flag      */
    
}

/*
*********************************************************************************************************
*                              WRITE AND READ SPIDAT REGISTER
*
* 函数说明: 往SPI数据寄存器写数据，同时读数据
*
* 入口参数: SPIx       只能是HT_SPI0/HT_SPI1中一个
*           halfword   需要写的数据  
*
* 返回参数: SPIDAT  : SPIDAT寄存器中读到的数据
* 
* 特殊说明: 无
*********************************************************************************************************
*/
uint8_t HT_SPI_SendByte(HT_SPI_TypeDef* SPIx, uint16_t halfword)
{
    while(SET==HT_SPI_ITFlagStatusGet(SPIx, SPI_SPISTA_SPIF));
    SPIx->SPIDAT = halfword;
    
    while(RESET==HT_SPI_ITFlagStatusGet(SPIx, SPI_SPISTA_SPIF));
    return (uint8_t)SPIx->SPIDAT;
}

/*
*********************************************************************************************************
*                                 SET SPI_CS HIGH
*
* 函数说明: 设定SPICS为高
*
* 入口参数: SPIx       只能是HT_SPI0/HT_SPI1中一个    
*
* 返回参数: 无
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_SPI_CSHigh(HT_SPI_TypeDef* SPIx)
{
    /*  assert_param  */
    
    SPIx->SPISSN = 0x03;                       /*!< SPI CS拉高           */
}

/*
*********************************************************************************************************
*                                 SET SPI_CS LOW
*
* 函数说明: 设定SPICS为低
*
* 入口参数: SPIx       只能是HT_SPI0/HT_SPI1中一个   
*
* 返回参数: 无
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_SPI_CSLow(HT_SPI_TypeDef* SPIx)
{
    /*  assert_param  */
    
    SPIx->SPISSN = 0x02;                       /*!< SPI CS拉低           */
}

