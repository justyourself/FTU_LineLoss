
#define VCARD			GPIOB_VCARD_CTRL
#define VCARD_SET		HT_GPIOB->PTSET |= GPIOB_VCARD_CTRL
#define VCARD_CLR		HT_GPIOB->PTCLR |= GPIOB_VCARD_CTRL
#define VCARD_OUTPUT	HT_GPIOB->PTDIR |= GPIOB_VCARD_CTRL

#define CARDRST			GPIOE_CARDRST
#define CARDRST_SET		HT_GPIOE->PTSET |= GPIOE_CARDRST
#define CARDRST_CLR		HT_GPIOE->PTCLR |= GPIOE_CARDRST
#define CARDRST_OUTPUT	HT_GPIOE->PTDIR |= GPIOE_CARDRST

#define VESAM			GPIOD_VESAM_CTL
#define VESAM_SET		HT_GPIOD->PTSET |= GPIOD_VESAM_CTL
#define VESAM_CLR		HT_GPIOD->PTCLR |= GPIOD_VESAM_CTL
#define VESAM_OUTPUT	HT_GPIOD->PTDIR |= GPIOD_VESAM_CTL

#define ESAMCS			GPIOG_ESAM_CS
#define ESAMCS_SET		HT_GPIOG->PTSET |= GPIOG_ESAM_CS
#define ESAMCS_CLR		HT_GPIOG->PTCLR |= GPIOG_ESAM_CS

#define ESAMCLK			GPIOG_ESAM_CLK
#define ESAMCLK_SET		HT_GPIOG->PTSET |= GPIOG_ESAM_CLK
#define ESAMCLK_CLR		HT_GPIOG->PTCLR |= GPIOG_ESAM_CLK

#define ESAMMOSI		GPIOG_ESAM_MOSI
#define ESAMMOSI_SET	HT_GPIOG->PTSET |= GPIOG_ESAM_MOSI
#define ESAMMOSI_CLR	HT_GPIOG->PTCLR |= GPIOG_ESAM_MOSI

#define MS006			50		//50*0.25ms = 12.5ms

#define ICRS_MinSize  	5
#define ICRTx_MaxSize  ICRS_MinSize+285	//200
#define ICRRx_MaxSize  ICRS_MinSize+285

#define PSAM		0
#define ESAM		1

#define ENCRYPT		0		//加密
#define DECRYPT		1		//解密
#define MACOPRT		2		//MAC运算

#define FILE_No1	0x01	//文件1
#define FILE_No2	0x02	//文件2
#define FILE_No3	0x03	//文件3
#define FILE_No4	0x04	//文件4
#define FILE_No5	0x05	//文件5
#define FILE_No6	0x06	//文件6
#define FILE_No7	0x07	//文件7
#define FILE_No8	0x08	//文件8
#define FILE_No9	0x09	//文件9
#define FILE_No10	0x0A	//文件10
#define FILE_No15	0x0F	//文件15

#define CARD_BUYEC		0x01	//用户购电卡
#define CARD_PREPARE	0x06	//预置卡
/*
#define ICALARM_10	0x10	//认证错误
#define ICALARM_11	0x11	//ESAM验证失败
#define ICALARM_12	0x12	//客户编号不匹配
#define ICALARM_13	0x13	//充值次数错误
#define ICALARM_14	0x14	//购电超囤积
#define ICALARM_15	0x15	//现场参数设置卡对本表已经失效
#define ICALARM_16	0x16	//修改密钥错误
#define ICALARM_17	0x17	//未按铅封键
#define ICALARM_18	0x18	//提前拔卡
#define ICALARM_19	0x19	//修改表号卡满（该卡无空余表号分配）
#define ICALARM_20	0x20	//修改密钥卡次数为零
#define ICALARM_21	0x21	//表计已开户（开户卡插入已经开过户的表计）
#define ICALARM_22	0x22	//表计未开户（用户卡插入还未开过户的表计）
#define ICALARM_23	0x23	//卡损坏或不明类型卡（如翻插卡、插铁片等）
#define ICALARM_24	0x24	//表计电压过低（此时操作IC卡可能会导致表计复位或损坏IC卡）
#define ICALARM_25	0x25	//卡文件格式不合法（包括帧头错，帧尾错，校验错）
#define ICALARM_26	0x26	//卡类型错
#define ICALARM_27	0x27	//已经开过户的新开户卡（新开户卡回写区有数据）
#define ICALARM_28	0x28	//其他错误（卡片选择文件错、读文件错、写文件错等）
*/

#define ICALARM_31	0x31	//电表故障			//新国网	//13.09.24
#define ICALARM_32	0x32	//无效卡片			//新国网	//13.09.24
#define ICALARM_33	0x33	//卡与表不匹配		//新国网	//13.09.24
#define ICALARM_34	0x34	//售电操作错误		//新国网	//13.09.24
#define ICALARM_35	0x35	//接触不良			//新国网	//13.09.24
#define ICALARM_36	0x36	//超囤积			//新国网	//13.09.24

#define ICALARM_10	ICALARM_32	//认证错误
#define ICALARM_11	ICALARM_31	//ESAM验证失败
#define ICALARM_12	ICALARM_33	//客户编号不匹配
#define ICALARM_13	ICALARM_34	//充值次数错误
#define ICALARM_14	ICALARM_36	//购电超囤积
//#define ICALARM_15	0x15	//现场参数设置卡对本表已经失效
#define ICALARM_16	0x00	//修改密钥错误
//#define ICALARM_17	0x17	//未按铅封键
#define ICALARM_18	ICALARM_35	//提前拔卡
//#define ICALARM_19	0x19	//修改表号卡满（该卡无空余表号分配）
//#define ICALARM_20	0x20	//修改密钥卡次数为零
#define ICALARM_21	ICALARM_34	//表计已开户（开户卡插入已经开过户的表计）
#define ICALARM_22	ICALARM_34	//表计未开户（用户卡插入还未开过户的表计）
#define ICALARM_23	ICALARM_32	//卡损坏或不明类型卡（如翻插卡、插铁片等）
#define ICALARM_24	ICALARM_31	//表计电压过低（此时操作IC卡可能会导致表计复位或损坏IC卡）
#define ICALARM_25	ICALARM_34	//卡文件格式不合法（包括帧头错，帧尾错，校验错）
#define ICALARM_26	ICALARM_32	//卡类型错
#define ICALARM_27	ICALARM_34	//已经开过户的新开户卡（新开户卡回写区有数据）
#define ICALARM_28	ICALARM_35	//其他错误（卡片选择文件错、读文件错、写文件错等）

//错误信息字：
//ICALARM_31	//电表故障															//新国网	//13.12.03
#define ICERROR_01	0x01	//电表电压过低											//新国网	//13.12.03
#define ICERROR_02	0x02	//操作ESAM错误											//新国网	//13.12.03
#define ICERROR_03	0x03	//ESAM复位错（ESAM损坏或未安装）						//新国网	//13.12.03

//ICALARM_32	//无效卡片															//新国网	//13.12.03
#define ICERROR_04	0x04	//卡片复位错（卡损坏或不明类型卡，如反插卡、插铁片等）	//新国网	//13.12.03
#define ICERROR_05	0x05	//身份认证错（通信成功但是密文不匹配）					//新国网	//13.12.03
#define ICERROR_06	0x06	//外部认证错（通信成功但是认证不通过）					//新国网	//13.12.03
#define ICERROR_07	0x07	//未发行的卡片（读卡片时返回6B00）						//新国网	//13.12.03
#define ICERROR_08	0x08	//卡类型错												//新国网	//13.12.03
#define ICERROR_09	0x09	//卡片操作为授权（密钥状态不为公钥时插参数预置卡）		//新国网	//13.12.03
#define ICERROR_10	0x0A	//MAC校验错误											//新国网	//13.12.03

//ICALARM_33	//卡与表不匹配															//新国网	//13.12.03
#define ICERROR_11	0x0B	//表号不一致											//新国网	//13.12.03
#define ICERROR_12	0x0C	//客户编号不一致										//新国网	//13.12.03
#define ICERROR_13	0x0D	//卡序列号不一致										//新国网	//13.12.03

//ICALARM_34	//售电操作错误															//新国网	//13.12.03
#define ICERROR_14	0x0E	//卡片文件格式不合法									//新国网	//13.12.03
#define ICERROR_15	0x0F	//购电卡插入未开户表									//新国网	//13.12.03
#define ICERROR_16	0x10	//补卡插入未开户表										//新国网	//13.12.03
#define ICERROR_17	0x11	//购电次数错误											//新国网	//13.12.03
#define ICERROR_18	0x12	//用户卡返写信息文件不为空								//新国网	//13.12.03

//ICALARM_35	//接触不良															//新国网	//13.12.03
#define ICERROR_19	0x13	//操作卡片通讯错误										//新国网	//13.12.03
#define ICERROR_20	0x14	//提前拔卡												//新国网	//13.12.03

//ICALARM_36	//超囤积															//新国网	//13.12.03
#define ICERROR_21	0x15	//剩余金额超囤积										//新国网	//13.12.03

#define RELAY_ON	0x00	//磁保持继电器闭合
#define RELAY_OFF	0x01	//磁保持继电器断开

//#define CYC5000		5000	//等待单字节		//V6
//#define CYC5000		60000	//等待单字节		//V6
//#define CYC60000	60000	//等待多字节		//V6
#define CYC5000		65000*4	//等待单字节		//V6
//#define CYC60000	65000*4	//等待多字节		//V6
#define CYC60000	65000*8	//等待多字节		//V6	//15.09.26
#define CYC120000	65000*8	//等待多字节		//V6
//#define CYC120000	65000*5	//等待多字节		//V6
#define CYC240000	65000*16	//等待多字节		//V6
//#define CYC240000	65000*12	//等待多字节		//V6

short Card_CoolRST( short CARD_TYPE );
void Card_Release( short CARD_TYPE );
void GetAndCompareESAMPara( void );
void RelayDrive( unsigned char DriveType );
void JudgeSTA_State( void );								
void OperateRelay( void );

short SelectDF( unsigned short DirFile );
short RecDataDelay( short CARD_TYPE, unsigned long Cycle, unsigned short RecLength );
//short Card_CoolRST( short CARD_TYPE );
short Card_HotRST( short CARD_TYPE );
//void Card_Release( short CARD_TYPE );
void ICDataSend( short CARD_TYPE, short Length, unsigned long Cycle, unsigned short RecLength );
short DisperseAuthentication( short FileNo, unsigned char* Dest, unsigned char* Source );
short GetChallenge( short CARD_TYPE, unsigned char* Dest, unsigned short Length );
short GetResponse( short CARD_TYPE, unsigned char* Dest, short Length );
short ExternalAuthentication( short CARD_TYPE, unsigned char* Source, short FileNo, short Length );
short EnDecrypt( short CARD_TYPE, unsigned char* Dest, unsigned char* Source, short OperateType ,short FileNo, short Length );
short InternalAuthentication( short FileNo, unsigned char* DisperseGene );

short ESAMExternalAuthentication( short FileNo, short FrameType );
short PSAMExternalAuthentication( short FileNo, unsigned char* DisperseGene );
short ReadBinary( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length );
short UpdateBinary( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length );
void ICProce( void );
short GetFullFrame( short FrameType );
short Card_Purview( short FrameType );
void ICComm_Ctrl( short FrameType );

void IC_BuyECNumber( void );//用户卡中的表号设置卡
void IC_BuyEC( void );		//购电卡
void IC_Check( void );		//检查卡
void IC_Relay( void );		//继电器检测卡
void IC_IncEC( void );		//增加电量卡
void IC_Prepare( void );	//预置卡
void IC_ModifyNmber( void );//修改表号卡
void IC_Emergency( void );	//应急卡	
void IC_ModifyKey( void );	//应急卡	
void IC_FastTest( void );	//快速测试卡	
void IC_SegSet( void );		//费率时段设置卡
//short ReadRecord( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length );
short ReadRecord( short CARD_TYPE, unsigned char* Dest );
short UpdateRecord( short CARD_TYPE, unsigned char* Dest, short FileNo, short StartAddr, short Length );
short Write_ESAMBurse( void );
void SwapFrameData( unsigned char* Dest, unsigned char* Source, short ItemCount );
short UpdateKey( unsigned char* Source, short FileNo );

void Clr_RemainEC( void );
void Clr_EC( void );
void Clr_PrePara( void );
void Clr_Event( void );

void Clr_AccountFlag( void );
void RS_Decrypt( unsigned char* ComBuf );

void GetReOverEC( unsigned char* ReDest, unsigned char* OverDest );
void GetICECState( unsigned char* ReDest, unsigned char* OverDest );
void GetECState( void );
void Clr_ReOverEC( void );
//short InDecrease( short CARD_TYPE, unsigned char* Source, short StartAddr, unsigned char Ins );
short InDecrease( short CARD_TYPE, unsigned char* Source );
unsigned char GetInDeData( unsigned char* Dest, unsigned char* Source );
short Write_ESAMPeriod( void );
//void RelayDrive( unsigned char DriveType );

short DisperseAuthenticationCardNo( short FileNo, unsigned char* DisperseGene );
short DisperseAuthenticationRand( unsigned char* Dest, unsigned char* Source );

short ReadBinaryIncMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short S_StartAddr );
short UpdateBinaryIncMAC( short CARD_TYPE, unsigned char* Source, unsigned char* DisperseGene );
short ReadBinaryRecMAC( short CARD_TYPE, unsigned char* Dest, short S_FileNo, short D_FileNo, short S_StartAddr );		//新国网	//13.11.23
short UpdateBinaryRecMAC( short CARD_TYPE, unsigned char* Source, short D_FileNo );

short ReadBinaryMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short S_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene );
short ReadRecMAC( short CARD_TYPE, unsigned char* Dest, short D_FileNo, short S_FileNo, short D_StartAddr, short RLength, short WLength, short R_Type, unsigned char* DisperseGene );
short UpdateBinaryMAC( short CARD_TYPE, unsigned char* Source, short FileNo, short StartAddr, short Length, short W_Type, unsigned char* DisperseGene );

void CommInitIC( short CARD_TYPE );
void CYCDelay(unsigned short cycle); /*输入cycle为希望的周期数的1/6。*/ 

void BCD3_HEX1( unsigned char* Dest, unsigned short Addr );
void GetBackBuyECFile1( unsigned char* Dest );
void GetLSEC( unsigned char* Dest, unsigned short Mon_No );
void GetBackBuyECFile2( unsigned char* Dest );
void SwapSegFeeFile( unsigned char* Source );
void SwapSegFeeFile( unsigned char* Source );
void FillSegFee( unsigned char* Dest, unsigned short DNumber, unsigned short SNumber );
short SegFeeFileProsess( unsigned char* Source, unsigned short SFileNo );

void CreateTripStartRec( unsigned char TripType );
void CreateTripEndRec( void );
void CreateILInsertRec( void );
void CreatICPrgRec( unsigned char CardType );

void IC_IOState( void );
void AddBuyECSumBCD( unsigned char* Source ); 		//累加累计购电金额

void CreatBuyECRec( void );						//产生购电记录

void IncConsumECSum( unsigned short Num, short PriceType );		//15.07.31
void GetFRAMConsumECSum( unsigned char* Dest );
unsigned long GetCurrentECPrice( void );
unsigned short GetCurrentStepNo( short Type  );
unsigned long GetCurrentStepPrice( void );
void GetCycleInsideECPp0( unsigned char* Dest  );
//void GetAndCompareESAMPara( void );
unsigned long GetDecData( unsigned char* Source );
//void OperateRelay( void );
short WriteESAMRunInfoFile( void );
void SaveBuyECState( void );

short WriteESAMRunInfoFilePassWordState( unsigned char* Source );

void KeyRelayOn( void );									//11.06.27

short CompareEncrypt( unsigned char* Dest, unsigned char* Source, unsigned short Length );		//新国网

short SelectDF_ESAM( unsigned short DirFile );			//新国网
//void JudgeSTA_State( void );						//新国网			

short UpdateBinaryDecMAC( short CARD_TYPE, unsigned char* Source );				//新国网	//13.11.27
void DecBuyECSumBCD( unsigned char* Source ); 		//退费累计购电金额		//新国网	//13.11.27

void GetYearCycleInsideECPp0( unsigned char* Dest );						//新国网	//13.11.28
void ProcessICAlarmFlg( void );												//新国网	//13.12.03
void InitialAbnormalInsertCardMeassage( void );								//新国网	//13.12.02
short JudgeMeterVolt80Un( void );												//新国网	//13.12.03
void SetJudgePermitFlag( void );											//新国网	//13.12.10			
void ClearJudgePermitFlag( void );											//新国网	//13.12.10			

unsigned long GetCurrentFeePrice( void );		                             //新国网    //13.12.12
void ExitPreTripState( void );													//11.08.19
unsigned long GetBuyECSum( void ); 							//取累计购电金额
