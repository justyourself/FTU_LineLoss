/*
*********************************************************************************************************
*                                              HT6XXXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*
*
* Project      : HT6xxx
* File         : ht6x3x_emu.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : Only support HT6x3x
*********************************************************************************************************
*/

#define  __HT6X3X_EMU_C

#include "ht6x3x_emu.h"

#if defined  HT6x3x                                              /* This File Only support HT6x3x */

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
*                                       INITIAL EMU MODULE
*
* 函数说明: 初始化EMU常数计量模块
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 必须保证PLL时钟或HRC时钟至少有一个使能，此模块才可以正常工作
*********************************************************************************************************
*/
void HT_EMU_Init()
{
    /*  assert_param  */

}

/*
*********************************************************************************************************
*                                      EMU Register Read
*
* 函数说明: 获取EMU模块可读寄存器的值
*
* 入口参数: EMU_Register  预读取寄存器名：
*                         1) EMU_PowerInQ : 无功功率输入寄存器
*                         2) EMU_PowerInP : 有功功率输入寄存器
*                         3) EMU_HFConst  : 输出脉冲频率设置
*                         4) EMU_EnergyP  : 有功能量计数寄存器（读后清零）
*                         5) EMU_EnergyQ  : 无功能量计数寄存器（读后清零）
*
* 返回参数: 寄存器值
*
* 特殊说明: 1)EMU_EnergyP、EMU_EnergyQ为读后清零寄存器
            2)若入口参数不合法，返回值为0
*********************************************************************************************************
*/
uint32_t HT_EMU_RegisterRead(EMUReg_TypeDef EMU_Register)
{
    /*  assert_param  */
    switch (EMU_Register)
    {
      case EMU_PowerInQ:                    /*!< 无功功率输入寄存器          */
        return HT_EMU->PowerInQ;

      case EMU_PowerInP:                    /*!< 有功功率输入寄存器          */
        return HT_EMU->PowerInP;

      case EMU_HFConst:                     /*!< 输出脉冲频率设置            */
        return HT_EMU->HFConst;

      case EMU_EnergyP:                     /*!< 有功能量计数寄存器          */
        HT_EMU->ReadCtrl |= EMU_EnergyP;
          while(HT_EMU->ReadCtrl & EMU_READCRTL_ENERGYP_RDY);
                return HT_EMU->EnergyP;

      case EMU_EnergyQ:                     /*!< 无功功率输入寄存器          */
        HT_EMU->ReadCtrl |= EMU_EnergyQ;
          while(HT_EMU->ReadCtrl & EMU_READCRTL_ENERGYQ_RDY);
                return HT_EMU->EnergyQ;

      default:
        return 0x0;                         /*!< 入口参数不合法              */
    }
}

/*
*********************************************************************************************************
*                                      EMU Register Write
*
* 函数说明: 向EMU模块可写寄存器赋值
*
* 入口参数: EMU_Register  预写入寄存器名：
*                         1) EMU_PowerInQ : 无功功率输入寄存器
*                         2) EMU_PowerInP : 有功功率输入寄存器
*                         3) EMU_HFConst  : 输出脉冲频率设置
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_EMU_RegisterWrite(EMUReg_TypeDef EMU_Register,uint32_t RegisterValue)
{
    /*  assert_param  */
    switch (EMU_Register)
    {
      case EMU_PowerInQ:                    /*!< 无功功率输入寄存器          */
        HT_EMU->PowerInQ = RegisterValue;
        break;

      case EMU_PowerInP:                    /*!< 有功功率输入寄存器          */
        HT_EMU->PowerInP = RegisterValue;
        break;

      case EMU_HFConst:                     /*!< 输出脉冲频率设置            */
        HT_EMU->HFConst  = RegisterValue;
        break;

      default:
        break;                            /*!< 入口参数不合法              */
    }
}


#endif                                               /* This File Only support HT6x3x */

