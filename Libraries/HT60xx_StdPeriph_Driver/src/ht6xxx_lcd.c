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
* File         : ht6xxx_lcd.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_LCD_C

#include "ht6xxx_lcd.h"

/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/

#if  defined  HT6x1x                                                        /*!< HT6x1x                */
#define    MaxSegNum                 36
#elif  defined  HT502x                                                      /*!< HT502x                */
#define    MaxSegNum                 43
#else                                                                       /*!< HT6x2x  HT501x        */
#define    MaxSegNum                 41
#endif

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
*                                         INITIAL LCD MODULE
*
* 函数说明: 初始化LCD模块
*
* 入口参数: LCD_InitStruct     LCD初始化结构体指针，主要包含5个参数: 
*                              1) LCD_Bias       : LCD偏压驱动
*                              2) LCD_Com        : LCDCom Number
*                              3) LCD_CLK        : LCD扫描频率 
*                              4) LCD_ChargeMode : LCD充电模式        
*                              5) LCD_Vrsel[3..0]: LCD对比度，数值越小，对比度越高                      
*
* 返回参数: 无                                      
* 
* 特殊说明: 用户在配置LCD寄存器前应先使能LCD模块，具体参见HT_CMU_ClkCtrl0Config()函数
*********************************************************************************************************
*/
void HT_LCD_Init(LCD_InitTypeDef* LCD_InitStruct)
{
    /*  assert_param  */
    
    uint32_t tempreg;

    tempreg  = LCD_InitStruct->LCD_Bias;                   /*!< LCD偏压驱动              */
    tempreg |= LCD_InitStruct->LCD_Com;                    /*!< LCDCom Number            */
    tempreg |= LCD_InitStruct->LCD_CLK;                    /*!< LCD扫描频率              */

    HT_LCD->LCDCLK = tempreg;                              /*!< 配置LCDCLK寄存器         */

    tempreg  = (uint32_t)LCD_InitStruct->LCD_ChargeMode;   /*!< LCD充电模式设置          */
    tempreg |= (LCD_InitStruct->LCD_Vrsel & LCD_LCDCON_VRSEL);
                                                           /*!< LCD对比度设置            */
    HT_LCD->LCDCON = tempreg;                              /*!< 配置LCDCON寄存器         */ 
} 

/*
*********************************************************************************************************
*                                         WRITE LCD SEGMENT
*
* 函数说明: 写
*
* 入口参数: SegNum     被写的一个Segment,只能在0-36之间
* 
*           SegValue   往Segment写的值                   
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_LCD_Write(uint8_t SegNum, uint8_t SegValue)
{
    /*  assert_param  */
    
    
    if(SegNum>MaxSegNum)
        return;
    HT_LCD->LCDBUF[SegNum] = SegValue;
} 

/*
*********************************************************************************************************
*                                         WRITE LCD SEGMENT
*
* 函数说明: 写
*
* 入口参数: SegNum     被写的一个Segment,只能在0-36之间
* 
*           SegValue   往Segment写的值                   
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
uint8_t HT_LCD_Read(uint8_t SegNum)
{
    /*  assert_param  */
        
    return (uint8_t)HT_LCD->LCDBUF[SegNum];
} 

