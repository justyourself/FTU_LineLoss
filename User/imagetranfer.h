//升级程序重写20130525: image_transfer..

//image_transfer status ::= enum  ---detailed status of image transfer process
//{		
#define  TRANSFER_NOT_INITIATED  		0
#define  TRANSFER_INITIATED      		1
#define  VERIFICATION_INITIATED  		2
#define  VERIFICATION_SUCCESSFUL 		3
#define  VERIFICATION_FAILED        4
#define  ACTIVATION_INITIATED       5
#define  ACTIVATION_SUCCESSFUL      6
#define  ACTIVATION_FAILED       		7

//Image_Tranfer_Flag 标志位
#define IMAGE_FLS4K_NEED    		 (0x01)		//FLASH(4K)需要进行先读出再写入操作,为零则ACTION2直接写入..
#define ACTION1_INTI_FLAG        (0x02)		//ACTION1执行标志..		其余操作清零..
#define ACTION2_TRAN_FLAG        (0x04)		//ACTION2执行标志..
#define ACTION3_VERIFY_FLAG      (0x08)		//ACTION3执行标志..
#define ACTION4_ACTIVATE_FLAG    (0x10)		//ACTION1执行操作通过标志..
#define ACTIVATE_TIME_FLAG       (0x20)		//时间到激活标志..
#define FLAG_BIT6                (0x40)
#define FLAG_BIT7                (0x80)
//	== 0; //标志清零..//初始化..


#define	CPU_FLS_PAGE		0x0200							//cpu 檫除程序，每扇区为0x200 = 512个字节..
//INFO=1800-19FF,只有写INFOA需要打开LOCKA开关,INFO每段是128个字节..
#define	CPU_INFOA  			0x1980							//0x1980-19FF..
#define	CPU_INFOB  			0x1900							//0x1900-197F..
#define	CPU_INFOC  			0x1880							//0x1880-18FF..
#define	CPU_INFOD  			0x1800							//0x1800-187F..


//}

#define IMAGE_BLOCK_SIZE   256			// 如果要改需要选择大于等于256..
short GetImage_transferClassData( unsigned char* Dest, unsigned char* Source ); 
unsigned char SetImage_transferClassData( unsigned char *Source , unsigned short DataLen );
unsigned char ActionImage_transferClassData( unsigned char *Source, unsigned short DataLen );

unsigned long GenerateCRC32(unsigned long oldcrc32, unsigned char * dataptr,unsigned short len);
unsigned long Flash_GenerateCRC32(unsigned long Addr,unsigned long len );

//激活函数(命令即或和时间激活)..
unsigned char Activate_Image_Transfer(unsigned char Enter_Flag );
unsigned char Verify_Image_Transfer(unsigned char Enter_Flag );			//校验..
///void Image_tranfer_init(void);
///void Image_tranfer_NewOrOld(void);
