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
而不需要管理具体的地址分配以及校验合的计算与存放,具体的使用可
以参考最下方两个测试函数的使用
*******************************************************************/
//#include "NvmOperation.h"
#include "Include.h"


/*Private define----------------------------------------------*/

/*Private functions-------------------------------------------*/
/*****************************************************************************************
*   Action:   带备分数据的读EEPROM
*   Input:     EE_Addr--EEPROM正常数地址; EE_BackupAddr--备份的地址,
			pBuf--数据存放的位置，包含最后两个字节的校验和,
*   Output:    0--失败，1--成功
*****************************************************************************************/
static BOOL_B PublicReadDulBackupData(uint32 EE_Addr, uint32 EE_BackupAddr, void *pBuf, uint16 Len)
{
	uint8 *pData = (uint8*)pBuf;
	
	if(Len < 2)
	{
		return FALSE;
	}		
	EE_READ_DATA((uint16)EE_Addr, pData, Len);
	if(CALC_CHECKSUM(pData, Len-2) != *(uint16*)&pData[Len-2])
	{
		EE_READ_DATA((uint16)EE_BackupAddr, pData, Len);
		if(CALC_CHECKSUM(pData, Len-2) != *(uint16*)&pData[Len-2])
		{
			return FALSE;
		}
		return (BOOL_B)EE_WRITE_DATA((uint16)EE_Addr, pData, Len);
	}
	else
	{
		if(!EE_CHECK_DATA((uint16)EE_BackupAddr, pData, Len))
		{
			return (BOOL_B)EE_WRITE_DATA((uint16)EE_BackupAddr, pData, Len);
		}
	}
	return TRUE;
}

/*****************************************************************************************
*   Action:   带备分数据的写EEPROM
*   Input:     EE_Addr--EEPROM正常数地址; EE_BackupAddr--备份的地址,
			pBuf--数据存放的位置，包含最后两个字节的校验和,
*   Output:    0--失败，1--成功
*****************************************************************************************/
static BOOL_B PublicWriteDulBackupData(uint32 EE_Addr, uint32 EE_BackupAddr, void *pBuf, uint16 Len)
{
	uint8 *pData = (uint8*)pBuf;
	
	if(Len < 2)
	{
		return FALSE;
	}
	*(uint16*)&pData[Len-2] = CALC_CHECKSUM(pData, Len-2);
	if(EE_WRITE_DATA((uint16)EE_Addr, pData, Len))
	{
		return (BOOL_B)EE_WRITE_DATA((uint16)EE_BackupAddr, pData, Len);
	}
	return FALSE;
}


/*Public functions-----------------------------------------------------------------------------------------*/
/******************************************************************************************
 *   读取存储空间里的数据
 *   addr表示存储类型及存储空间
 *   取最高两位表示: 00-片外EEPROM 01-片外FLASH 10-片内FLASH 11-保留
 *   地址范围:       0 ~ 3FFFFFFF(1024M bytes)
******************************************************************************************/
tNvmType NvmGetType(uint32 Addr)
{
	return (tNvmType)(Addr >> 30);
}

/******************************************************************************************
 * action:  读取存储空间里的数据，带备份的数据则自动对校验和进行运算.其它的则不
 		包含校验和的运算，需调用者自行运算
 *   addr表示存储类型及存储空间
 *   取最高两位表示: 00-片外EEPROM 01-片外FLASH 10-片内FLASH 11-保留
 *   地址范围:       0 ~ 3FFFFFFF(1024M bytes)
 * pBuf:    读取内容缓冲地址, 如果是带备份的数据则包含最后两个字节的校验和,
 * Len:    读取长度, 带校验和(或是备份)的数据需采用类似
	typedef struct
	{
		uint32 NvmDataAddr;		
		uint32 AllNum;      //总共计录数
		uint16 Index;       //滚动计录计数器
		uint16 NumVal;      //有效计录数
		uint16 MaxNum;		//最大条数
		uint16 DataLen;		//单条数据长度
		uint16 CheckSum;    //信息头校验和
	}tLogHead;
	的结构来定义。其它不带备份的数据则没有要求，
 		例: 
 		tLogHead   LogHead;
 		NvmBytesRead(NvmHeadAddr, &LogHead, sizeof(LogHead)
******************************************************************************************/
BOOL_B NvmBytesRead(uint32 Addr, void *pBuf, uint16 Len)
{
	tNvmType NvmType;
	
	NvmType = NvmGetType(Addr);
	Addr &= 0x3fffffff;
	switch(NvmType)
	{
		case EEPROM_TYPE:
			if(Addr < EE_BAK_SPACE_SIZE)
			{
				if((Addr+Len) >= EE_BAK_SPACE_SIZE/2)
				{
					return FALSE;
				}
				return PublicReadDulBackupData(Addr, Addr+EE_BAK_SPACE_SIZE/2, pBuf, Len);
			}
			else
			{
				if(Addr >= EE_SPACE_MAX_SIZE)
				{
					return FALSE;
				}
				return (BOOL_B)EE_READ_DATA(Addr, pBuf, Len);
			}
			break;
		case FLASH_EXTERNAL:
			if((Addr + Len)> FLASH_SPACE_MAX_SIZE)
			{
				return FALSE;
			}
			return (BOOL_B)FLASH_READ_DATA(Addr, pBuf, Len);
		default:
			break;
	}
	return FALSE;
}

/******************************************************************************************
 * action:  读取存储空间里的数据，带备份的数据则自动对校验和(CRC16)进行运算.其它的则不
 		包含校验和的运算，需调用者自行运算
 *   addr表示存储类型及存储空间
 *   取最高两位表示: 00-片外EEPROM 01-片外FLASH 10-片内FLASH 11-保留
 *   地址范围:       0 ~ 3FFFFFFF(1024M bytes)
 * pBuf:    写内容缓冲地址,如果是带备份的数据则包含最后两个字节的校验和,此
 			校验和由此函数内部计算，只需多开辟两个字节的空间即可
 * Len:    写数据的长度,  带校验和(或是备份)的数据需采用类似
	typedef struct
	{
		uint32 NvmDataAddr;		
		uint32 AllNum;      //总共计录数
		uint16 Index;       //滚动计录计数器
		uint16 NumVal;      //有效计录数
		uint16 MaxNum;		//最大条数
		uint16 DataLen;		//单条数据长度
		uint16 CheckSum;    //信息头校验和
	}tLogHead;
	的结构来定义,即是把校验和放在最后。其它不带备份的数据则没有要求，
 			例: 
 			tLogHead   LogHead;
 			NvmBytesWrite(NvmHeadAddr, &LogHead, sizeof(LogHead)
******************************************************************************************/
BOOL_B NvmBytesWrite(uint32 Addr, void *pBuf, uint16 Len)
{
	tNvmType NvmType;
	
	NvmType = NvmGetType(Addr);
	Addr &= 0x3fffffff;
	switch(NvmType)
	{
		case EEPROM_TYPE:
			if(Addr < EE_BAK_SPACE_SIZE)
			{
				if((Addr+Len) >= EE_BAK_SPACE_SIZE/2)		//超出了备份数据区的地址
				{
					return FALSE;
				}
				return PublicWriteDulBackupData(Addr, Addr+EE_BAK_SPACE_SIZE/2, pBuf, Len);
			}
			else
			{
				if(Addr >= EE_SPACE_MAX_SIZE)
				{
					return FALSE;
				}
				return (BOOL_B)EE_WRITE_DATA(Addr, pBuf, Len);
			}
			break;
		case FLASH_EXTERNAL:
			if((Addr + Len)> FLASH_SPACE_MAX_SIZE)
			{
				return FALSE;
			}
			return (BOOL_B)FLASH_WRITE_DATA(Addr, pBuf, Len);
		default:
			break;
	}
	return FALSE;
}

BOOL_B NvmBytesDirectWrite(uint32 Addr, void *pBuf, uint16 Len)
{
	tNvmType NvmType;
	
	NvmType = NvmGetType(Addr);
	Addr &= 0x3fffffff;
	switch(NvmType)
	{
		case EEPROM_TYPE:
			return (BOOL_B)EE_WRITE_DATA(Addr, pBuf, Len);
		case FLASH_EXTERNAL:
			return (BOOL_B)FLASH_WRITE_DATA(Addr, pBuf, Len);
		default:
			break;
	}
	return FALSE;
}

BOOL_B NvmBytesDirectRead(uint32 Addr, void *pBuf, uint16 Len)
{
	tNvmType NvmType;
	
	NvmType = NvmGetType(Addr);
	Addr &= 0x3fffffff;
	switch(NvmType)
	{
		case EEPROM_TYPE:
			return (BOOL_B)EE_READ_DATA(Addr, pBuf, Len);
		case FLASH_EXTERNAL:
			return (BOOL_B)FLASH_READ_DATA(Addr, pBuf, Len);
		default:
			break;
	}
	return FALSE;
}

uint8 NvmTest(void)
{
	uint8 temp[255];
	uint8 temp1[255];
	uint8 i;
	uint8 succ=0;

	for(i=0;i<255;i++)
	{
		temp[i]=i;
		temp1[i]=0;
	}
	if(NvmBytesWrite(EE_ADDR_BASIC, temp, sizeof(temp)))
	{
		if(NvmBytesRead(EE_ADDR_BASIC, temp1, sizeof(temp1)))
		{
			for(i=0;i<255;i++)
			{
				if(temp[i]!=temp1[i])
				{
					break;
				}
				else if(i==254)
				{
					succ=1;
				}
			}
		}
	}

	
	for(i=0;i<255;i++)
	{
		temp[i]=i;
		temp1[i]=0;
	}
	if(succ)
	{
		succ=0;
		if(NvmBytesWrite(EE_BAK_BASIC_ADDR, temp, sizeof(temp)))
		{
			if(NvmBytesRead(EE_BAK_BASIC_ADDR, temp1, sizeof(temp1)))
			{
				for(i=0;i<255;i++)
				{
					if(temp[i]!=temp1[i])
					{
						break;
					}
					else if(i==254)
					{
						succ=1;
					}
				}
			}
		}
	}

	for(i=0;i<255;i++)
	{
		temp[i]=i;
		temp1[i]=0;
	}
	if(succ)
	{
		succ=0;
		if(FLASH_ERASE_SECTOR(0))
		{
			if(NvmBytesWrite(FLASH_ADDR_DISTINGUISH_ADDR, temp, sizeof(temp)))
			{
				if(NvmBytesRead(FLASH_ADDR_DISTINGUISH_ADDR, temp1, sizeof(temp1)))
				{
					for(i=0;i<255;i++)
					{
						if(temp[i]!=temp1[i])
						{
							break;
						}
						else if(i==254)
						{
							succ=1;
						}
					}
				}
			}
		}
	}

	return succ;
}

uint8 DSTTest(void)
{
	DST_Test DST_TestTemp;
	DST_Test DST_TestTemp1;
	uint16 i;
	uint8 succ=0;

	//测试EE_STRUCT_ADDR
	for(i=0;i<10;i++)
	{
		DST_TestTemp.member1[i]=i+1;
		DST_TestTemp.member2[i]=i+1;

		DST_TestTemp1.member1[i]=0;
		DST_TestTemp1.member2[i]=0;
	}
	if(NvmBytesWrite(EE_STRUCT_ADDR(DST_TestEEP.member1[0]), DST_TestTemp.member1, sizeof(DST_TestTemp.member1)))
	{
		if(NvmBytesRead(EE_STRUCT_ADDR(DST_TestEEP.member1[0]), DST_TestTemp1.member1, sizeof(DST_TestTemp1.member1)))
		{
			for(i=0;i<sizeof(DST_TestTemp1.member1);i++)
			{
				if(DST_TestTemp.member1[i]!=DST_TestTemp1.member1[i])
				{
					succ=0;
					break;
				}
				else if(i==sizeof(DST_TestTemp1.member1)-1)
				{
					succ=1;
				}
			}
		}
	}

	//测试EE_BAK_STRUCT_ADDR
	if(succ)
	{
		succ=0;
		for(i=0;i<10;i++)
		{
			DST_TestTemp.member1[i]=i+1;
			DST_TestTemp.member2[i]=i+1;

			DST_TestTemp1.member1[i]=0;
			DST_TestTemp1.member2[i]=0;
		}
		if(NvmBytesWrite(EE_BAK_STRUCT_ADDR(DST_TestEEPBak), &DST_TestTemp, sizeof(DST_TestTemp)))
		{
			if(NvmBytesRead(EE_BAK_STRUCT_ADDR(DST_TestEEPBak), &DST_TestTemp1, sizeof(DST_TestTemp1)))
			{
				for(i=0;i<sizeof(DST_TestTemp1);i++)
				{
					if(((uint8 *)&DST_TestTemp)[i] != ((uint8 *)&DST_TestTemp1)[i])
					{
						succ=0;
						break;
					}
					else if(i==sizeof(DST_TestTemp1)-1)
					{
						succ=1;
					}
				}
			}
		}
	}
	//测试FLASH_STRUCT_ADDR
	if(succ)
	{
		succ=0;
		for(i=0;i<10;i++)
		{
			DST_TestTemp.member1[i]=i+1;
			DST_TestTemp.member2[i]=i+1;

			DST_TestTemp1.member1[i]=0;
			DST_TestTemp1.member2[i]=0;
		}
		if(NVM_FLASH_ERASE(FLASH_STRUCT_ADDR(DST_TestFlash)))
		{
			if(NvmBytesWrite(FLASH_STRUCT_ADDR(DST_TestFlash), &DST_TestTemp, sizeof(DST_TestTemp)))
			{
				if(NvmBytesRead(FLASH_STRUCT_ADDR(DST_TestFlash), &DST_TestTemp1, sizeof(DST_TestTemp1)))
				{
					for(i=0;i<sizeof(DST_TestTemp1);i++)
					{
						if(((uint8 *)&DST_TestTemp)[i] != ((uint8 *)&DST_TestTemp1)[i])
						{
							succ=0;
							break;
						}
						else if(i==sizeof(DST_TestTemp1)-1)
						{
							succ=1;
						}
					}
				}
			}
		}
	}
	return succ;
}

