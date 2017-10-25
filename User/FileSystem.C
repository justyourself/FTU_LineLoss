#include "TypeMeter.h"
#include "TypeE2p.h"
#include "TypeRAM.h"
//#include "msp430x54x.h"
#include "ht6xxx_lib.h"
#include "DLT698.h"					
#include "Measure.h"
//#include "In430.h"
#include "Port.h"
#include "TypeFlash.h"
#include "Data.h"
#include "General.h"
#include "Mem.h"

//#include "RsComm.h"
#include "interrupt.h"
#include "Mem.h"
#include "Time.h"
#include "Power.h"
#if ( MEASCHIP == ADE7878 )	
#include "AD7878.h"
#endif
#include "Initial.h"
//#include "Produce.h"
#if ( MEASCHIP == IDT90E32 )							//12.08.29	
#include "IDT90E32.h"
#endif

//擦除指定扇区
short Erase_Sector(unsigned short nDest)
{
	unsigned long Value;
	
	Value = nDest;
	Value *= FLASH_SECTOR_SIZE;

	return BlockErase( Value );
}

//格式化FAT区
void File_Format( void )
{
	unsigned short i;
	
	for( i=FAT_SECTOR;i<DATA_START_SECTOR;i++)
	{
		Erase_Sector(i);		
	}	
}	

//FBuff.Buff	缓存用Buff，静态变量，通用4096字节

//读剩余扇区数并返回
//RemainStartSector剩余空闲的第一个扇区地址
unsigned short GetRemainSectors( unsigned short* RemainStartSector )
{
	unsigned short i,j;
	unsigned long Fls_Src;
	unsigned short CurSector;
	unsigned short IdleSector;
	unsigned char* Ptr;
	
	*RemainStartSector = 0xFFFF;
	
	CurSector = 0;
	IdleSector = 0;
	for( i=FAT_INDEX_START_SECTOR;i<FAT_INDEX_END_SECTOR;i++ )
	{
		Fls_Src = i;
		Fls_Src *= FLASH_SECTOR_SIZE;
		Read_Flash( FBuff.Buff, Fls_Src, 4096 );

		Ptr = FBuff.Buff;
		for( j=0;j<FAT_SECTOR_INDEXS;j++ )
		{
			if(( *Ptr == 0xFF )&&( *(Ptr+1) == 0xFF )&&( *(Ptr+2) == 0xFF ))
			{
				if( *RemainStartSector == 0xFFFF ) *RemainStartSector = CurSector;
				IdleSector++;				
			}
			Ptr += 3;		
			CurSector++;
			if( CurSector >= ( FLASH_SECTOR_COUNT - DATA_START_SECTOR ) ) return IdleSector; 		
		}	
	}
    
    return IdleSector;
}	

/*
//从当前扇区开始查找下一个空闲的扇区地址
unsigned short GetNextRemainSector( unsigned short StartSector )
{
	unsigned char Buff[6];
	unsigned short i,j;
	unsigned long Fls_Src;
	unsigned short IdleSector;
	unsigned char* Ptr;
	unsigned short StartIndexSector;

	Ptr = Buff;
	
	IdleSector = StartSector;
	StartIndexSector = StartSector / FAT_SECTOR_INDEXS; 			//可能不在索引的第一页
	StartSector = StartSector % FAT_SECTOR_INDEXS;
	
	Fls_Src = (unsigned long)(StartIndexSector+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;
	Fls_Src += StartSector*3;
	for( i=StartIndexSector;i<(FAT_INDEX_END_SECTOR-FAT_INDEX_START_SECTOR);i++ )
	{
//		Fls_Src = (unsigned long)(i+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;	
		for( j=StartSector;j<FAT_SECTOR_INDEXS;j++ )
		{		
			Read_Flash( Ptr, Fls_Src, 3 );
			if(( *Ptr == 0xFF )&&( *(Ptr+1) == 0xFF )&&( *(Ptr+2) == 0xFF ))
			{
				return IdleSector;
			}		
			IdleSector++;
			if( IdleSector >= ( FLASH_SECTOR_COUNT - DATA_START_SECTOR ) ) return 0xFFFF; 		//超出有效范围
			Fls_Src += 3;			
		}	
		StartSector = 0;
		Fls_Src = (unsigned long)(i+1+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;
	}	
	return 0xFFFF; 		//超出有效范围
}	
*/

//从当前扇区开始查找下一个空闲的扇区地址
unsigned short GetNextRemainSector( unsigned short StartSector )
{
	unsigned char Buff[6];
	unsigned short i,j;
	unsigned long Fls_Src;
	unsigned short IdleSector;
	unsigned char* Ptr;
	unsigned short StartIndexSector;

	Ptr = Buff;
	
	IdleSector = StartSector;
	StartIndexSector = StartSector / FAT_SECTOR_INDEXS; 			//可能不在索引的第一页
	StartSector = StartSector % FAT_SECTOR_INDEXS;
	
	if( StartSector == 0 )											//从头查找时先都整页查找，以提高查找速度
	{
		Fls_Src = (unsigned long)(StartIndexSector+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;
		Read_Flash( FBuff.Buff, Fls_Src, 4096 );
		Ptr = FBuff.Buff;
		for( i=StartIndexSector;i<(FAT_INDEX_END_SECTOR-FAT_INDEX_START_SECTOR);i++ )
		{
			for( j=StartSector;j<FAT_SECTOR_INDEXS;j++ )
			{		
				if(( *Ptr == 0xFF )&&( *(Ptr+1) == 0xFF )&&( *(Ptr+2) == 0xFF ))
				{
					return IdleSector;
				}		
				IdleSector++;
				if( IdleSector >= ( FLASH_SECTOR_COUNT - DATA_START_SECTOR ) ) return 0xFFFF; 		//超出有效范围
				Ptr += 3;			
			}	
			StartSector = 0;
			Fls_Src += 4096;
			Read_Flash( FBuff.Buff, Fls_Src, 4096 );
			Ptr = FBuff.Buff;
		}		
	}	
	else
	{
		Fls_Src = (unsigned long)(StartIndexSector+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;
		Fls_Src += StartSector*3;
		for( i=StartIndexSector;i<(FAT_INDEX_END_SECTOR-FAT_INDEX_START_SECTOR);i++ )
		{
//			Fls_Src = (unsigned long)(i+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;	
			for( j=StartSector;j<FAT_SECTOR_INDEXS;j++ )
			{		
				Read_Flash( Ptr, Fls_Src, 3 );
				if(( *Ptr == 0xFF )&&( *(Ptr+1) == 0xFF )&&( *(Ptr+2) == 0xFF ))
				{
					return IdleSector;
				}		
				IdleSector++;
				if( IdleSector >= ( FLASH_SECTOR_COUNT - DATA_START_SECTOR ) ) return 0xFFFF; 		//超出有效范围
				Fls_Src += 3;			
			}	
			StartSector = 0;
			Fls_Src = (unsigned long)(i+1+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;
		}
	}		
	return 0xFFFF; 		//超出有效范围
}	



//将下一个空闲的扇区索引写入当前扇区索引
void WriteNextSectorNo( unsigned short CurSector, unsigned short NextSector )
{
	unsigned char Buff[6];
//	unsigned short i,j;
	unsigned long Fls_Src;
	unsigned short IdleSector;
	unsigned char* Ptr;
	unsigned short IndexPages;

	Ptr = Buff;
	
	IndexPages = CurSector / FAT_SECTOR_INDEXS; 			//可能不在索引的第一页
	IdleSector = CurSector % FAT_SECTOR_INDEXS;
	
	Fls_Src = (unsigned long)((IndexPages+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE);			//索引从第二页开始
	Fls_Src += IdleSector*3;
	RAM_Write( Ptr, (unsigned char*)&NextSector, 2 );
	*(Ptr+2) = ChkNum( Ptr, 2 );
	Write_Flash_Direct( Fls_Src, Ptr, 3 );
	Write_Flash_Direct( Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, Ptr, 3 );//备份	
	
}	


//从头开始查找下一个同文件名的目录地址，并返回第一个空目录地址
unsigned short GetDirNo( unsigned char* FileName, unsigned short* RemainStartDir )
{
	unsigned char Buff[22];
	unsigned short i;
	unsigned long Fls_Src;
	unsigned short CurDir;
	unsigned char* Ptr;

	Ptr = Buff;
	
	*RemainStartDir = 0xFFFF;
	
	CurDir = 0;
	Fls_Src = FLASH_SECTOR_SIZE;				//目录从第一页而不是第0页开始
	for( i=0;i<FAT_DIRS;i++ )
	{
		Read_Flash( Ptr, Fls_Src, 10 );
		if( IsAllData( Ptr, FAT_DIR_LEN, 0xff ) == 0 )	//有空余目录位置
		{
			if( *RemainStartDir == 0xFFFF ) *RemainStartDir = CurDir;
		}
		else
		{
			if( *(Ptr+9) != ChkNum( Ptr, 9 ) )
			{
				Read_Flash( Ptr+10, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, 10 );
				if( *(Ptr+19) != ChkNum( Ptr+10, 9 ) ) return 0xFFFF;
				else
				{
					RAM_Write( Ptr, Ptr+10, 10 );				
				}	
			}							
		}			
		if( Data_Comp( Ptr, FileName, 5 ) == 0 )
		{
			return CurDir;			
		}	
		CurDir++;
		Fls_Src += FAT_DIR_LEN;			
	}	
	return 0xFFFF; 		//无指定文件名
}	

//从头开始查找下一个同文件名的目录地址，并返回第一个空目录地址
unsigned short GetNewDirNo( unsigned char* FileName, unsigned short* RemainStartDir )
{
	unsigned char Buff[22];
	unsigned short i;
	unsigned long Fls_Src;
	unsigned short CurDir;
	unsigned char* Ptr;
	unsigned char* Point;

	Point = Buff;
	
	*RemainStartDir = 0xFFFF;
	
	CurDir = 0;
	Fls_Src = FLASH_SECTOR_SIZE;				//目录从第一页而不是第0页开始
	Read_Flash( FBuff.Buff, Fls_Src, 4096 );
	Ptr = FBuff.Buff;
	for( i=0;i<FAT_DIRS;i++ )
	{
		if( IsAllData( Ptr, FAT_DIR_LEN, 0xff ) == 0 )	//有空余目录位置
		{
			if( *RemainStartDir == 0xFFFF ) *RemainStartDir = CurDir;
		}
		else
		{
			if( *(Ptr+9) != ChkNum( Ptr, 9 ) )
			{
				Read_Flash( Point, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, 10 );
				if( *(Point+9) != ChkNum( Point, 9 ) ) return 0xFFFF;
				else
				{
					RAM_Write( Ptr, Point, 10 );				
				}	
			}							
		}			
		if( Data_Comp( Ptr, FileName, 5 ) == 0 )
		{
			return CurDir;			
		}	
		CurDir++;
		Fls_Src += FAT_DIR_LEN;			
		Ptr += FAT_DIR_LEN;
	}	
	return 0xFFFF; 		//无指定文件名
}	


//读剩余目录数并返回
//RemainStartDir剩余空闲的第一个目录地址
unsigned short GetRemainDirs( unsigned short* RemainStartDir )
{
	unsigned short i;
	unsigned long Fls_Src;
	unsigned short IdleDir;
	unsigned char* Ptr;
	
	*RemainStartDir = 0xFFFF;
	
	Fls_Src = FLASH_SECTOR_SIZE;
	Read_Flash( FBuff.Buff, Fls_Src, 4096 );

	IdleDir = 0;
	Ptr = FBuff.Buff;
	for( i=0;i<FAT_DIRS;i++ )
	{
		if( IsAllData( Ptr, FAT_DIR_LEN, 0xff ) == 0 )
		{
			if( *RemainStartDir == 0xFFFF ) *RemainStartDir = i;
			IdleDir++;				
		}		
		Ptr += FAT_DIR_LEN;
	}	
	return IdleDir;
}	

//typedef struct
//{
//	unsigned short DirAddr;
//	unsigned short StartSector;
//	unsigned short Sectors;
//}FILEPARA

//在生成文件前，已知需分配的总块数和FLASH能容纳这么多数据块
//返回文件指向目录位置、第一个索引位置
unsigned short CreatFile( FILEPARA* FilePara, unsigned char* FileName )
{
	unsigned char Buff[12];
//	unsigned char Buff2[4];
	unsigned char* Ptr;	
//	unsigned char* Point;	
	unsigned short DirAddr;
	unsigned short RemainStartDir;
	unsigned long Fls_Src;
	unsigned short i;
	unsigned short StartSector;
	unsigned short CurSector;
	unsigned short NextSector;
	
	Ptr = Buff;
//	Point = Buff2;
//	DirAddr = GetDirNo( FileName, &RemainStartDir );						//取空文件目录地址
	DirAddr = GetNewDirNo( FileName, &RemainStartDir );						//取空文件目录地址
	if( DirAddr != 0xFFFF ) return 0xFFFF;									//该文件名已存在，创建失败
	if( RemainStartDir == 0xFFFF ) return 0xFFFF;							//无空文件目录地址
	FilePara->DirAddr = RemainStartDir;										//取空文件目录地址
	
	StartSector = 0;														//从第一单元索引起始查找
	for( i=0;i<FilePara->Sectors;i++ )
	{
		NextSector = GetNextRemainSector( StartSector );					//搜索空索引位置
		if( NextSector == 0xFFFF ) return 0xFFFF;							//已无空索引位置
		if( i == 0 )
		{
			FilePara->StartSector = NextSector;								//文件起始索引位置
			RAM_Write( Ptr, FileName, 5 );
			RAM_Write( Ptr+5, (unsigned char*)&FilePara->StartSector, 2 );
			RAM_Write( Ptr+7, (unsigned char*)&FilePara->Sectors, 2 );
			*(Ptr+9) = ChkNum( Ptr, 9 );
			Fls_Src = FLASH_SECTOR_SIZE;									//FAT文件目录区
			Fls_Src += RemainStartDir * 10;
			Write_Flash_Direct( Fls_Src, Ptr, 10 );								//保存文件目录，FLASH中FF可以直接写入新值	
			Write_Flash_Direct( Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, Ptr, 10 );//备份文件目录，FLASH中FF可以直接写入新值	
		}
		else
		{
			WriteNextSectorNo( CurSector, NextSector );						//在当前索引位置，写入下一索引地址
		}		
		CurSector = NextSector;												//将下一索引地址作为当前索引地址
		StartSector = NextSector + 1;
	}	
	if( i == FilePara->Sectors )												//索引分配完毕
	{
		WriteNextSectorNo( CurSector, 0x0000 );								//最后一个索引填入0x0000,表示文件结束块
	}	
	return 0;
}	

//从文件的起始端开始删除指定数量的索引
//StartSector:起始索引位置
//SectorSum:索引数量
//返回0：正常删除；非零：删除异常
unsigned short DeleteFileSector( unsigned short StartSector, unsigned short SectorSum )					
{
	unsigned char Buff[6];
	unsigned short i;
	unsigned short Addr;
	unsigned long Fls_Src;
	unsigned short CurSector;
	unsigned char* Ptr;
	unsigned char* Point;
	unsigned short Sector;
	unsigned long PageNo;
	unsigned long NewPageNo;

	Point = Buff;
	
	CurSector = StartSector;
	for( i=0;i<SectorSum; )
	{
		PageNo = CurSector / FAT_SECTOR_INDEXS; 					//可能不在索引的第一页
		if( PageNo > FAT_INDEX_SECTOR_LEN ) return 0xFFFF;			//索引数错误,超出有效范围	
		Sector = CurSector % FAT_SECTOR_INDEXS;
    	
		Fls_Src = (PageNo+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;
		Read_Flash( FBuff.Buff, Fls_Src, 4096 );
		
		Ptr = FBuff.Buff;
		do
		{
			Addr = Sector * 3;
			RAM_Write( Point, Ptr+Addr, 3 );						//读取下一索引地址
			if( *(Point+2) != ChkNum( Point, 2 ) )
			{
				Read_Flash( Point+3, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, 3 );
				if( *(Point+5) != ChkNum( Point+3, 2 ) ) return 0xFFFF;
				else
				{
					RAM_Write( Point, Point+3, 3 );				
				}	
			}	
			RAM_Write( (unsigned char*)&CurSector, Point, 2 );				
			RAM_DataFill( Ptr+Addr, 3, 0xFF);					//回收扇区时把索引全部填0xFF,方便创建文件时直接写入，不需要再擦除
			NewPageNo = CurSector / FAT_SECTOR_INDEXS;
			Sector = CurSector % FAT_SECTOR_INDEXS;	
			i++;
			if(( i>= SectorSum )||( CurSector == 0 )) break;	//文件结束
		}while( PageNo == NewPageNo );							//同一索引页内在RAM内操作
		
		BlockErase(Fls_Src);
		Write_Flash_Direct( Fls_Src, FBuff.Buff, 4096 );
		Fls_Src += FAT_SECTOR_BAK*FLASH_SECTOR_SIZE;			//更新当前索引
		BlockErase(Fls_Src);
		Write_Flash_Direct( Fls_Src, FBuff.Buff, 4096 );				//更新备份索引

		if(( i>= SectorSum )||( CurSector == 0 )) break;		//文件结束
	}	
	
	return 0;
	
}

//在删除文件前，已知文件名
//返回0，删除成功，非0：删除失败
unsigned short DeletFile( FILECURVEPARA* FileCurvePara, unsigned char* FileName )
{
	unsigned char Buff[24];
	unsigned char* Ptr;	
	unsigned short DirAddr;
//	unsigned short RemainStartDir;
	unsigned long Fls_Src;
	unsigned short StartSector;
	unsigned short SectorSum;
	
	Ptr = Buff;
//	DirAddr = GetDirNo( FileName, &RemainStartDir );						//取空文件目录地址
//	if( DirAddr == 0xFFFF ) return 0xFFFF;									//该文件名不存在，删除失败
	
	DirAddr = FileCurvePara->DirNo;											//由删除文件前先打开文件获得		
	Fls_Src = FAT_SECTOR * FLASH_SECTOR_SIZE;
	Fls_Src += DirAddr * 10;
	Read_Flash( Ptr, Fls_Src, 10 );
	if( *(Ptr+9) != ChkNum( Ptr, 9 ) )
	{
		Read_Flash( Ptr+10, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, 10 );
		if( *(Ptr+19) != ChkNum( Ptr+10, 9 ) ) return 0xFFFF;
		else
		{
			RAM_Write( Ptr, Ptr+10, 10 );				
		}	
	}	
	
	RAM_Write( (unsigned char*)&StartSector, Ptr+5, 2 );
	RAM_Write( (unsigned char*)&SectorSum, Ptr+7, 2 );

	if( DeleteFileSector( StartSector, SectorSum ) != 0 ) return 0xFFFF;

	RAM_DataFill( Ptr, 10, 0xFF);
	Fls_Src = FAT_SECTOR*FLASH_SECTOR_SIZE;
	Read_Flash( FBuff.Buff, Fls_Src, 4096 );
	RAM_Write( FBuff.Buff+DirAddr * 10, Ptr, 10 );
	
	BlockErase(Fls_Src);
	Write_Flash_Direct( Fls_Src, FBuff.Buff, 4096 );
	Fls_Src += FAT_SECTOR_BAK*FLASH_SECTOR_SIZE;			//更新当前目录
	BlockErase(Fls_Src);
	Write_Flash_Direct( Fls_Src, FBuff.Buff, 4096 );				//更新备份目录

	return 0;
}	

//打开文件，查找有否对应的文件名，占用空间是否一致，返回指向第一个扇区号
unsigned short Open_File( FILECURVEPARA* FileCurvePara, unsigned char* FileName )
{
	unsigned char Buff[24];
	unsigned char* Ptr;	
	unsigned short DirAddr;
	unsigned short RemainStartDir;
	unsigned long Fls_Src;
	unsigned short StartSector;
	unsigned short SectorSum;
	
	Ptr = Buff;
	DirAddr = GetDirNo( FileName, &RemainStartDir );						//取当前文件目录地址
	if( DirAddr == 0xFFFF ) return 0xFFFF;									//该文件名不存在，删除失败
	
	Fls_Src = FAT_SECTOR * FLASH_SECTOR_SIZE;
	Fls_Src += DirAddr * 10;
	Read_Flash( Ptr, Fls_Src, 10 );
	if( *(Ptr+9) != ChkNum( Ptr, 9 ) )
	{
		Read_Flash( Ptr+10, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, 10 );
		if( *(Ptr+19) != ChkNum( Ptr+10, 9 ) ) return 0xFFFF;
		else
		{
			RAM_Write( Ptr, Ptr+10, 10 );				
		}	
	}	
	
	RAM_Write( (unsigned char*)&StartSector, Ptr+5, 2 );
	RAM_Write( (unsigned char*)&SectorSum, Ptr+7, 2 );
	if( SectorSum != FileCurvePara->Pages ) return 0xFFFF;					//存储空间是否一致
	else FileCurvePara->StartSectorNo = StartSector;

	FileCurvePara->DirNo = DirAddr;
	return 0;
}	

//根据起始索引号，读取指定索引对应扇区的物理首地址,SectorSum=0时指向第一条
long GetAppointSector( unsigned short StartSector, unsigned short SectorSum )					
{
	unsigned char Buff[6];
	unsigned short i;
//	unsigned short Addr;
	long Fls_Src;
	unsigned short CurSector;
	unsigned char* Point;
	unsigned short Sector;
	unsigned long PageNo;

	Point = Buff;
	
	CurSector = StartSector;
	for( i=0;i<SectorSum;i++ )
	{
		PageNo = CurSector / FAT_SECTOR_INDEXS; 					//可能不在索引的第一页
		if( PageNo > FAT_INDEX_SECTOR_LEN ) return -1;				//索引数错误,超出有效范围	
		Sector = CurSector % FAT_SECTOR_INDEXS;
    	
		Fls_Src = (PageNo+FAT_INDEX_START_SECTOR) * FLASH_SECTOR_SIZE;
		
		Fls_Src += Sector * 3;
		Read_Flash( Point, Fls_Src, 3 );
		if( *(Point+2) != ChkNum( Point, 2 ) )
		{
			Read_Flash( Point+3, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, 3 );		//取备份数据
			if( *(Point+5) != ChkNum( Point+3, 2 ) ) return -1;
			else
			{
				RAM_Write( Point, Point+3, 3 );				
			}	
		}	
		RAM_Write( (unsigned char*)&CurSector, Point, 2 );			//下一索引地址	
	}
	
	CurSector += DATA_START_SECTOR;									//前一索引的值，即为对应的扇区号
	Fls_Src = CurSector;
	Fls_Src *= FLASH_SECTOR_SIZE;									//真实物理地址
	
	return Fls_Src;
	
}

//////////////////////////////////////////////////////////////////
//从头开始查找下一个同文件名的目录地址，并返回第一个空目录地址
void GetDirTo_FBuff( void )								//17.03.08
{
	unsigned long Fls_Src;

	Fls_Src = FLASH_SECTOR_SIZE;				//目录从第一页而不是第0页开始
	Read_Flash( FBuff.Buff, Fls_Src, 4096 );
}

//在RAM中从头开始查找下一个同文件名的目录地址，并返回第一个空目录地址
unsigned short GetDirNo_Form_FBuff( unsigned char* FileName, unsigned short* RemainStartDir )		//17.03.08
{
	unsigned char Buff[22];
	unsigned short i;
	unsigned long Fls_Src;
	unsigned short CurDir;
	unsigned char* Ptr;
	unsigned char* Point;

	Point = Buff;
	Ptr = FBuff.Buff;
	
	*RemainStartDir = 0xFFFF;
	
	CurDir = 0;
	Fls_Src = FLASH_SECTOR_SIZE;				//目录从第一页而不是第0页开始
	for( i=0;i<FAT_DIRS;i++ )
	{
//		Read_Flash( Ptr, Fls_Src, 10 );
		if( IsAllData( Ptr, FAT_DIR_LEN, 0xff ) == 0 )	//有空余目录位置
		{
			if( *RemainStartDir == 0xFFFF ) *RemainStartDir = CurDir;
		}
		else
		{
			if( *(Ptr+FAT_DIR_LEN-1) != ChkNum( Ptr, FAT_DIR_LEN-1 ) )
			{
				Read_Flash( Point, Fls_Src+FAT_SECTOR_BAK*FLASH_SECTOR_SIZE, 10 );
				if( *(Point+FAT_DIR_LEN-1) != ChkNum( Point, FAT_DIR_LEN-1 ) ) return 0xFFFF;
				else
				{
					RAM_Write( Ptr, Point, FAT_DIR_LEN );				
				}	
			}							
		}			
		if( Data_Comp( Ptr, FileName, 5 ) == 0 )
		{
			return CurDir;			
		}	
		CurDir++;
		Fls_Src += FAT_DIR_LEN;		
		Ptr += FAT_DIR_LEN;		
	}	
	return 0xFFFF; 		//无指定文件名
}	

//当当前目录已在FBuff中时，打开文件，查找有否对应的文件名，占用空间是否一致，返回指向第一个扇区号
unsigned short Open_File_In_FBuff( FILECURVEPARA* FileCurvePara, unsigned char* FileName )				//17.03.08
{
	unsigned char* Ptr;	
	unsigned short DirAddr;
	unsigned short RemainStartDir;
	unsigned short StartSector;
	unsigned short SectorSum;

	Ptr = FBuff.Buff;

	DirAddr = GetDirNo_Form_FBuff( FileName, &RemainStartDir );						//取当前文件目录地址
	if( DirAddr == 0xFFFF ) return 0xFFFF;									//该文件名不存在，删除失败
	Ptr += DirAddr * FAT_DIR_LEN;
	
	RAM_Write( (unsigned char*)&StartSector, Ptr+5, 2 );
	RAM_Write( (unsigned char*)&SectorSum, Ptr+7, 2 );
	if( SectorSum != FileCurvePara->Pages ) return 0xFFFF;					//存储空间是否一致
	else FileCurvePara->StartSectorNo = StartSector;

	FileCurvePara->DirNo = DirAddr;
	return 0;
}	




