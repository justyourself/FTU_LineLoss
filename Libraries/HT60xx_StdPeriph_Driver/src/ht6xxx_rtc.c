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
* File         : ht6xxx_rtc.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : 
*********************************************************************************************************
*/

#define  __HT6XXX_RTC_C

#include "ht6xxx_rtc.h"

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

static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};

/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         INITIAL RTC MODULE
*
* 函数说明: 初始化RTC模块
*
* 入口参数: RTC_InitStruct     RTC初始化结构体指针，主要包含4个参数: 
*                              1) RTC_TMR1    : RTC定时器1初始化结构，主要包含使能与定时周期设置两方面
*                              2) RTC_TMR2    : RTC定时器2初始化结构，主要包含使能与定时周期设置两方面
*                              3) RTC_Alarm   : RTC闹钟设置 
*                              4) RTC_TOUT    : RTCTout输出配置                         
*
* 返回参数: 无                                      
* 
* 特殊说明: 中断配置是直接赋值，未被配置的位会被直接写为0
*********************************************************************************************************
*/
void HT_RTC_Init(RTC_InitTypeDef* RTC_InitStruct)
{
    /*  assert_param  */
    

    if(RTC_InitStruct->RTC_TMR1.NewState != DISABLE)                 /*!< 配置RTC定时器1         */
    {
        HT_RTC->RTCTMR1 = (uint32_t)RTC_InitStruct->RTC_TMR1.Count;  /*!< 设置RTC定时器1定时周期 */    
        HT_RTC->RTCCON |= RTC_RTCCON_RTC1EN;                         /*!< 使能RTC定时器1         */     
    }
    else
    {
        HT_RTC->RTCCON &= ~RTC_RTCCON_RTC1EN;                        /*!< 关闭RTC定时器1         */
    }   

    if(RTC_InitStruct->RTC_TMR2.NewState != DISABLE)                 /*!< 配置RTC定时器2         */
    {
        HT_RTC->RTCTMR2 = (uint32_t)RTC_InitStruct->RTC_TMR2.Count;  /*!< 设置RTC定时器2定时周期 */    
        HT_RTC->RTCCON |= RTC_RTCCON_RTC2EN;                         /*!< 使能RTC定时器2         */     
    }
    else
    {
        HT_RTC->RTCCON &= ~RTC_RTCCON_RTC2EN;                        /*!< 关闭RTC定时器2         */
    }   
                                                                     /*!< 配置闹铃寄存器         */
    HT_RTC->ALMR = (uint32_t)(RTC_InitStruct->RTC_Alarm.HourAndMinute & RTC_ALMR);
    
    HT_RTC->RTCCON &= ~(uint32_t)RTC_RTCCON_TOUT;
    HT_RTC->RTCCON |= (uint32_t)RTC_InitStruct->RTC_TOUT;            /*!< 配置TOUT输出           */   
} 

/*
*********************************************************************************************************
*                                         READ RTC TIME
*
* 函数说明: 读RTC时间寄存器，包括周/年/月/日/时/分/秒共7个寄存器
*
* 入口参数: pRTCTime    void*型变量，其指向对象用于存储RTC时间，在程序中实际被当成uint8_t*来处理
*                       存储循序如下：
*                       pRTCTime[0]: WEEKR
*                       pRTCTime[1]: YEARR
*                       pRTCTime[2]: MONTHR
*                       pRTCTime[3]: DAYR
*                       pRTCTime[4]: HOURR
*                       pRTCTime[5]: MINR
*                       pRTCTime[6]: SECR
*
* 返回参数: 无                                      
* 
* 特殊说明: 用户应尽量避免在读的过程中去写这些寄存器
*********************************************************************************************************
*/
void HT_RTC_Read(void* pRTCTime)
{
    /*  assert_param  */
    
    uint8_t *pRTCRead = pRTCTime;
    
    HT_RTC->RTCRD = RTC_RTCRD_READFLAG;                /*!< 设置开始读标志        */
    while(HT_RTC->RTCRD & RTC_RTCRD_READFLAG);         /*!< 等待可以读            */
                                                       /*!< 开始读                */
    pRTCRead[0] = HT_RTC->WEEKR & RTC_WEEKR;      
    pRTCRead[1] = HT_RTC->YEARR & RTC_YEARR;
    pRTCRead[2] = HT_RTC->MONTHR& RTC_MONTHR;
    pRTCRead[3] = HT_RTC->DAYR  & RTC_DAYR;
    pRTCRead[4] = HT_RTC->HOURR & RTC_HOURR;
    pRTCRead[5] = HT_RTC->MINR  & RTC_MINR;
    pRTCRead[6] = HT_RTC->SECR  & RTC_SECR;

} 

/*
*********************************************************************************************************
*                                         WRITE RTC TIME     
*
* 函数说明: 写RTC时间寄存器，包括周/年/月/日/时/分/秒共7个寄存器
*
* 入口参数: pRTCTime    void*型变量，其指向对象用于存储RTC时间，在程序中实际被当成uint8_t*来处理
*                       存储循序如下：
*                       pRTCTime[0]: WEEKR
*                       pRTCTime[1]: YEARR
*                       pRTCTime[2]: MONTHR
*                       pRTCTime[3]: DAYR
*                       pRTCTime[4]: HOURR
*                       pRTCTime[5]: MINR
*                       pRTCTime[6]: SECR
*
* 返回参数: 无                                      
* 
* 特殊说明: 1) 由于一次更新所有计时寄存器，如果用户想更新部分寄存器，用户调用此函数前需先读出所有计时寄存器，
*              然后更新需要更新的寄存器，最后一起写入。
*           2) 用户应避免在写的过程中读这些寄存器
*           3) 用户应保证写入数据的有效性
*********************************************************************************************************
*/
void HT_RTC_Write(void* pRTCTime)
{
    /*  assert_param  */
    
    uint8_t *pRTCWrite = pRTCTime;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    
                                                       /*!< 保存当前写保护状态    */ 
    HT_CMU->WPREG = CMU_WPREG_UnProtected;             /*!< 关闭写保护功能        */
    
    HT_RTC->RTCWR = RTC_RTCWR_CLEAR;                   /*!< 清除写标志            */
                                                       /*!< 开始写                */
    HT_RTC->WEEKR  = pRTCWrite[0] & RTC_WEEKR;      
    HT_RTC->YEARR  = pRTCWrite[1] & RTC_YEARR;
    HT_RTC->MONTHR = pRTCWrite[2] & RTC_MONTHR;
    HT_RTC->DAYR   = pRTCWrite[3] & RTC_DAYR;
    HT_RTC->HOURR  = pRTCWrite[4] & RTC_HOURR;
    HT_RTC->MINR   = pRTCWrite[5] & RTC_MINR;
    HT_RTC->SECR   = pRTCWrite[6] & RTC_SECR;
    
    HT_RTC->RTCWR = RTC_RTCWR_UPDATE;                  /*!< 写完成                */
    
    HT_CMU->WPREG = writeProtect;                      /*!< 恢复之前写保护设置    */

} 

/*
*********************************************************************************************************
*                                    RTC TIMER CONFIGURE     
*
* 函数说明: 设置RTC定时器1或者定时器2的使能状态以及定时周期
*
* 入口参数: RTCTMRx    选择RTC定时器1或者RTC定时器2
*
*           RTCTMR_InitStruct      RTC定时器初始化结构体指针，主要包含以下两个参数：
*                                  a) SetState  : 设置定时器使能或者关闭
*                                  b) Count     : 设置定时器定时周期                                   
*                                                 对RTC定时器1，每一个Count表示1s
*                                                 对RTC定时器2，每一个Count表示0.0625s
*
* 返回参数: 无                                      
* 
* 特殊说明: 实际定时周期为(1+COUNT)，即在1-65536之间
*********************************************************************************************************
*/
void HT_RTC_TMRConfig(RTCTMR_SelectTypeDef RTCTMRx, RTCTMR_InitTypeDef* RTCTMR_InitStruct)
{
    /*  assert_param  */
                           
    
    if(RTCTMR_InitStruct->NewState != DISABLE)
    {
        if (RTCTMRx == RTCTMR1)
            HT_RTC->RTCTMR1 = (uint32_t)RTCTMR_InitStruct->Count;  /*!< 设置RTC定时器1定时周期  */
        else
            HT_RTC->RTCTMR2 = (uint32_t)RTCTMR_InitStruct->Count;  /*!< 设置RTC定时器2定时周期  */
        
        HT_RTC->RTCCON |= (uint32_t)RTCTMRx;                       /*!< 使能RTC定时器           */
            
    }
    else
    {
            HT_RTC->RTCCON &= ~(uint32_t)RTCTMRx;                  /*!< 关闭RTC定时器           */
    }

}

/*
*********************************************************************************************************
*                                       SET TOUT    
*
* 函数说明: 设置Tout输出特定信号
*
* 入口参数: RTCToutSet   Tout输出设置参数
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_RTC_ToutSet(RTCTout_TypeDef RTCToutSet)
{
    /*  assert_param  */
    
    HT_RTC->RTCCON &= ~(uint32_t)RTC_RTCCON_TOUT;
    HT_RTC->RTCCON |= (uint32_t)RTCToutSet; 
}

/*
*********************************************************************************************************
*                                        SET ALARM    
*
* 函数说明: 设置闹钟
*
* 入口参数: RTCAlarmSet  存储设置闹铃的时与分
*
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_RTC_AlarmSet(RTCAlarm_TypeDef RTCAlarmSet)
{
    /*  assert_param  */
    
    HT_RTC->ALMR = (uint32_t)(RTCAlarmSet.HourAndMinute & RTC_ALMR);
}

/*
*********************************************************************************************************
*                                 ENABLE OR DISABLE RTC INTERRUPT    
*
* 函数说明: 使能或者关闭RTC中断
*
* 入口参数: ITEn       RTC中断设置位，可以为以下参数或其组合
*                        @arg RTC_RTCIE_SECIE
*                        @arg RTC_RTCIE_MINIE
*                        @arg RTC_RTCIE_HRIE
*                        @arg RTC_RTCIE_DAYIE
*                        @arg RTC_RTCIE_MTHIE
*                        @arg RTC_RTCIE_RTC1IE
*                        @arg RTC_RTCIE_RTC2IE
*                        @arg RTC_RTCIE_ALMIE
*
*           NewState   = ENABLE： 使能中断
*                      = DISABLE：关闭中断
* 返回参数: 无                                      
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_RTC_ITConfig(uint8_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    
    if (NewState != DISABLE)
    {       
        HT_RTC->RTCIE |= (uint32_t)ITEn;            /*!< 使能RTC中断           */
    }
    else
    {
        HT_RTC->RTCIE &= ~(uint32_t)ITEn;           /*!< 关闭RTC中断           */
    } 
}

/*
*********************************************************************************************************
*                            GET SPECIFIED RTC INTERRUPT FLAG STATUS
*
* 函数说明: 获取相应RTC中断标志状态
*
* 入口参数: ITFlag     想要检查的某个RTC中断，可以为以下参数:
*                        @arg RTC_RTCIF_SECIF
*                        @arg RTC_RTCIF_MINIF
*                        @arg RTC_RTCIF_HRIF
*                        @arg RTC_RTCIF_DAYIF
*                        @arg RTC_RTCIF_MTHIF
*                        @arg RTC_RTCIF_RTC1IF
*                        @arg RTC_RTCIF_RTC2IF
*                        @arg RTC_RTCIF_ALMIF
*
* 返回参数: ITStatus    = SET：  相应中断标志产生
*                       = RESET：相应中断标志未产生
* 
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_RTC_ITFlagStatusGet(uint8_t ITFlag)
{
    /*  assert_param  */
    
    if (HT_RTC->RTCIF & ITFlag)
    {       
        return SET;                        /*!< RTC Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< RTC Interrupt Flag is reset */
    } 
}

/*
*********************************************************************************************************
*                                   CLEAR RTC INTERRUPT FLAG
*
* 函数说明: 清除RTC中断标志
*
* 入口参数: ITFlag     想要清除的某个RTC中断标志，可以为以下参数或其组合:
*                        @arg RTC_RTCIF_SECIF
*                        @arg RTC_RTCIF_MINIF
*                        @arg RTC_RTCIF_HRIF
*                        @arg RTC_RTCIF_DAYIF
*                        @arg RTC_RTCIF_MTHIF
*                        @arg RTC_RTCIF_RTC1IF
*                        @arg RTC_RTCIF_RTC2IF
*                        @arg RTC_RTCIF_ALMIF
*
* 返回参数: 无
* 
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_RTC_ClearITPendingBit(uint8_t ITFlag)
{
    /*  assert_param  */
    
      
    HT_RTC->RTCIF  &= ~((uint32_t)ITFlag);                  /*!< Clear RTC Interrupt Flag       */
    
}

/*
*********************************************************************************************************
*                                LOAD RTC COFFIENT FROM INFO FLASH
*
* 函数说明: 从InfoFlash中装载RTC补偿系数
*
* 入口参数: 无
*
* 返回参数: Bool    = FALSE：Info中存储的系数异常，装载失败
*                   = TRUE ：Info中存储的系数正常，装载成功
* 
* 特殊说明: 用户应保证在调用此函数时Flash处于读状态
*********************************************************************************************************
*/
Bool HT_RTC_LoadInfoData(void)
{
    uint32_t i, chksum;
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    
                                                           /*!< 保存当前写保护状态    */    
	
    for(i=0,chksum=0; i<14; i++)
    {
        chksum += HT_INFO->DataArry[i];
    }
    if((chksum != HT_INFO->DataArry[14])||(HT_INFO->DataArry[14]==0))                     
    {                                                      /*!< Info中系数异常        */
        /*!< 用户可在此进一步增加处理代码   */
        
        return FALSE;
    }
    else                                                   /*!< Info中系数正常        */
    {
        HT_CMU->WPREG = CMU_WPREG_UnProtected;             /*!< 关闭写保护功能        */
        
        if (HT_RTC->DFAH    != HT_INFO->Muster.iDFAH)   HT_RTC->DFAH    = HT_INFO->Muster.iDFAH;
        if (HT_RTC->DFAL    != HT_INFO->Muster.iDFAL)   HT_RTC->DFAL    = HT_INFO->Muster.iDFAL;
        if (HT_RTC->DFBH    != HT_INFO->Muster.iDFBH)   HT_RTC->DFBH    = HT_INFO->Muster.iDFBH;
        if (HT_RTC->DFBL    != HT_INFO->Muster.iDFBL)   HT_RTC->DFBL    = HT_INFO->Muster.iDFBL;
        if (HT_RTC->DFCH    != HT_INFO->Muster.iDFCH)   HT_RTC->DFCH    = HT_INFO->Muster.iDFCH;
        if (HT_RTC->DFCL    != HT_INFO->Muster.iDFCL)   HT_RTC->DFCL    = HT_INFO->Muster.iDFCL;
        if (HT_RTC->DFDH    != HT_INFO->Muster.iDFDH)   HT_RTC->DFDH    = HT_INFO->Muster.iDFDH;
        if (HT_RTC->DFDL    != HT_INFO->Muster.iDFDL)   HT_RTC->DFDL    = HT_INFO->Muster.iDFDL;
        if (HT_RTC->DFEH    != HT_INFO->Muster.iDFEH)   HT_RTC->DFEH    = HT_INFO->Muster.iDFEH;
        if (HT_RTC->DFEL    != HT_INFO->Muster.iDFEL)   HT_RTC->DFEL    = HT_INFO->Muster.iDFEL;
        if (HT_RTC->Toff    != HT_INFO->Muster.iToff)   HT_RTC->Toff    = HT_INFO->Muster.iToff;
        if (HT_RTC->MCON01  != HT_INFO->Muster.iMCON01) HT_RTC->MCON01  = HT_INFO->Muster.iMCON01;
        if (HT_RTC->MCON23  != HT_INFO->Muster.iMCON23) HT_RTC->MCON23  = HT_INFO->Muster.iMCON23;
        if (HT_RTC->MCON45  != HT_INFO->Muster.iMCON45) HT_RTC->MCON45  = HT_INFO->Muster.iMCON45;
                                                           /*!< 装载系数              */
        
        HT_CMU->WPREG = writeProtect;                      /*!< 恢复之前写保护设置    */

        return TRUE;
    }
}

