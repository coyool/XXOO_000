/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：NvmOperation.c
* 文件标识：
* 摘要：非易失性存储器操作单元

* 当前版本：1.0.0 
* 作者：冯秋雄
* 完成日期：20131015
* 说明:
	配合DatastructTab.h单元使用，集数据存储地址管理与数据存储操作
为一体的非易失性存储器数据管理单元
	使用者只需要配置好所需数据是放在FLASH/带备份的EE/不带备份的EE,
而不需要管理具体的地址分配以及校验合的计算与存放,对于带备份的
数据需要传进来的BUF指针里预留最后两个字节来存放校验和，具体的使用可
以参考.c文件最下方两个测试函数的使用
*******************************************************************/

#ifndef __NVM_OPERATION_H
#define __NVM_OPERATION_H


/*Public typedef----------------------------------------------*/
typedef enum
{
	EEPROM_TYPE				= 0x00,
	FLASH_EXTERNAL			= 0x01,
	FLASH_MCU				= 0x10,			
}tNvmType;

/*Public define----------------------------------------------*/

#define EE_READ_DATA(x,y,z)			ReadEepromBytes(y,x,z)		//EEPROM读函数y为操作地址，x为所操作的Buf, z为操作的长度
#define EE_WRITE_DATA(x,y,z)		WriteEepromBytes(y,x,z)		//EEPROM写函数y为操作地址，x为所操作的Buf, z为操作的长度
#define EE_CHECK_DATA(x,y,z)		CheckEepromBytes(y,x,z)		//EEPROM数据检查函数y为操作地址，x为所操作的Buf, z为操作的长度

#define FLASH_ERASE_PAGE(x)			
#define FLASH_ERASE_SECTOR(x)		1//MX25L3206_SecErase(x)		//x为所擦除的扇区号
#define FLASH_READ_DATA(x,y,z)		1//MX25L3206_Read(y,x,z)		//y为操作地址，x为所操作的Buf, z为操作的长度
#define FLASH_WRITE_DATA(x,y,z)		1//MX25L3206_Write(x,y,z)		//y为操作地址，x为所操作的Buf, z为操作的长度
#define NVM_FLASH_ERASE(x)			1//FLASH_ERASE_SECTOR(((x)&0x3fffffff)/FLASH_SECTOR_SIZE)		//NVM单元扇区擦除宏

#ifndef CALC_CHECKSUM
#define CALC_CHECKSUM(x,y)			PublicCrc16(x,y)
#endif
/*Public functions----------------------------------------------*/
tNvmType NvmGetType(uint32 Addr);
BOOL_B NvmBytesRead(uint32 Addr, void *pBuf, uint16 Len);
BOOL_B NvmBytesWrite(uint32 Addr, void *pBuf, uint16 Len);
BOOL_B NvmBytesDirectWrite(uint32 Addr, void *pBuf, uint16 Len);	//直接对存储器操作的函数，只根据地址进行存储器类型区分，
BOOL_B NvmBytesDirectRead(uint32 Addr, void *pBuf, uint16 Len); 	//直接对存储器操作的函数，只根据地址进行存储器类型区分，

uint8 NvmTest(void);
uint8 DSTTest(void);

#endif

