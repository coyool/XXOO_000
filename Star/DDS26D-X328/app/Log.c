/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Log.c
* 文件标识：
* 摘要：FIFO数据存储管理

* 当前版本：1.0.0 
* 作者：冯秋雄
* 完成日期：20130927
*******************************************************************/
//#include "Log.h"
#include "Include.h"

/*Private define---------------------------------------------*/
#define LOG_NVM_READ(x,y,z)				NvmBytesRead(x,y,z)	
#define LOG_NVM_WRITE(x,y,z)			NvmBytesWrite(x,y,z)	
#define LOG_FLASH_ERASE(x)				//FLASH_ERASE_SECTOR(((x)&0x3fffffff)/FLASH_SECTOR_SIZE)
/*Private variable--------------------------------------------*/

/*Private functions-------------------------------------------*/
/*****************************************************************************************
*   Action:    查找事件记录指定点(指针有效模式)的地址
*   Input:    pLogHead   - 头信息
*              point - 提取最近第N点数据, 0000表示读取最早一条, 0001表示读取最近一条.
*   Output:    0xffff-暂无记录 ,其它值为数据存储的绝对地址偏移
*****************************************************************************************/
static uint16 LOG_AbsAddr(const tLogHead *pLogHead, uint16 point)
{
    uint16 AbsOffset = 0xffff;

	if(pLogHead->NumVal == 0) 
	{
		return AbsOffset;  // 记录为空
    }
    if(point > 0)
    {
        if(point > pLogHead->NumVal) 
		{
			return AbsOffset;
        }
        AbsOffset = (pLogHead->Index == pLogHead->NumVal) ? (pLogHead->Index - point): (pLogHead->Index + pLogHead->NumVal - point) % pLogHead->NumVal;
    }
    else
    {
        AbsOffset = (pLogHead->Index == pLogHead->NumVal) ? 0 : pLogHead->Index;
    }
    return AbsOffset;
}

/*Export functions-------------------------------------------*/
/*****************************************************************************************
*   Action:    滚动添加记录函数, 累加记录计数器, 当前记录指针指向最近一条
*   Input:     EE_Addr  - EEPROM记录地址, 0xFFFFFFFF表示不使用EE
*			EE_BackupAddr--备份数据地址
*			FLASH_Addr - FLASH记录地址0xFFFFFFFF表示不使用FLASH ；在EE与FLASH都有效的情
*				况下，头信息存在EE_Addr的EE中，而数据信息存FLASH_Addr的FLASH中；其
*				它情况则是头信息存在开始紧接着存数据
*              Len   - 单条事件记录内容长度
*              MaxNum   - 事件总记录条数，包括了为了方便数据处理而多存的条数，比实际
			需要的条数可能要大
*   Output:    0--FALSE 失败; 1--TRUE 成功
*****************************************************************************************/
BOOL_B LOG_Init(uint32 NvmHeadAddr, uint32 NvmDataAddr, uint16 Len, uint16 MaxNum)
{
	tLogHead LogHead;
	tNvmType Type;

	if(!LOG_NVM_READ(NvmHeadAddr, &LogHead, sizeof(LogHead)))
	{
		my_memset(&LogHead, 0, sizeof(LogHead));
	}

	if((LogHead.DataLen != Len) || (LogHead.NvmDataAddr != NvmDataAddr) || (LogHead.MaxNum != MaxNum))
	{
		my_memset(&LogHead, 0, sizeof(LogHead));
		LogHead.DataLen = Len;
		LogHead.NvmDataAddr = NvmDataAddr;
		LogHead.MaxNum = MaxNum;
	}
	Type = NvmGetType(NvmDataAddr);
	if((Type == FLASH_EXTERNAL) && (LogHead.Index == 0))
	{
		LOG_FLASH_ERASE(NvmDataAddr);		//erase first page
	}
	return LOG_NVM_WRITE(NvmHeadAddr, &LogHead, sizeof(LogHead));
}

/*****************************************************************************************
*   Action:    滚动添加记录函数, 累加记录计数器, 当前记录指针指向最近一条
*   Input:     EE_Addr  - EEPROM记录地址, 0xFFFFFFFF表示不使用EE
*			FLASH_Addr - FLASH记录地址0xFFFFFFFF表示不使用FLASH ；在EE与FLASH都有效的情
				况下，头信息存在EE_Addr的EE中，而数据信息存FLASH_Addr的FLASH中；其
				它情况则是头信息存在开始紧接着存数据
*              evnet - 待写入事件记录内容缓冲
*              len   - 单条事件记录内容长度
*              num   - 事件总记录条数，包括了为了方便数据处理而多存的条数，比实际
			需要的条数可能要大
*   Output:    0--FALSE 失败; 1--TRUE 成功
*****************************************************************************************/
BOOL_B LOG_RollAdd(uint32 NvmHeadAddr, void * event)
{
	tLogHead LogHead = {0};
	tNvmType Type;

	if(!LOG_NVM_READ(NvmHeadAddr, &LogHead, sizeof(LogHead)))
	{
		return FALSE;
	}
	Type = NvmGetType(LogHead.NvmDataAddr);
	LogHead.AllNum++;
	if(LogHead.Index >= LogHead.MaxNum)
	{
		LogHead.Index = 1;
		if(Type == FLASH_EXTERNAL)
		{
			LOG_FLASH_ERASE(LogHead.NvmDataAddr);
		}
	}
	else
	{
		if(Type == FLASH_EXTERNAL)
		{
			if((uint32)(LogHead.Index)*LogHead.DataLen%FLASH_SECTOR_SIZE+LogHead.DataLen > FLASH_SECTOR_SIZE)//need erase next sector
			{
				LOG_FLASH_ERASE((LogHead.NvmDataAddr+(uint32)(LogHead.Index)*LogHead.DataLen+LogHead.DataLen));
			}
		}
		LogHead.Index++;
	}
	if(LogHead.NumVal < LogHead.MaxNum)
	{
		LogHead.NumVal++;
	}
	else
	{
		LogHead.NumVal = LogHead.MaxNum;
	}
	if(LOG_NVM_WRITE(LogHead.NvmDataAddr + (LogHead.Index -1) * LogHead.DataLen, event, LogHead.DataLen))
	{		
		return LOG_NVM_WRITE(NvmHeadAddr, &LogHead, sizeof(LogHead));
	}
	return FALSE;
}

/*****************************************************************************************
*   Action:    事件记录指定点(指针有效模式)输出
*   Input:      EE_Addr  - EEPROM记录地址, 0xFFFFFFFF表示不使用EE
*			FLASH_Addr - FLASH记录地址0xFFFFFFFF表示不使用FLASH ；在EE与FLASH都有效的情
				况下，头信息存在EE_Addr的EE中，而数据信息存FLASH_Addr的FLASH中；其
				它情况则是头信息存在开始紧接着存数据
*              evnet - 待存放事件记录内容缓冲
*              len   - 单条事件记录内容长度
*              point - 提取最近第N点数据, 0000表示读取最早一条, 0001表示读取最近一条.
*   Output:    0xffff-暂无记录 ,其它值为数据存储的绝对地址偏移
*****************************************************************************************/
uint16 LOG_FiFo(uint32 NvmHeadAddr, void * event, uint16 point)
{
    uint16 AbsOffset;
	tLogHead LogHead;

	if(!LOG_NVM_READ(NvmHeadAddr, &LogHead, sizeof(LogHead)))
	{
		return 0xffff;
	}
	AbsOffset = LOG_AbsAddr(&LogHead, point);
	if(!LOG_NVM_READ(LogHead.NvmDataAddr + (uint32)AbsOffset * LogHead.DataLen, event, LogHead.DataLen))
	{
		return 0xffff;
	}
    return AbsOffset;
}

/*****************************************************************************************
*   Action:    绝对地址数据读取
*   Input:      EE_Addr  - EEPROM记录地址, 0xFFFFFFFF表示不使用EE
*			FLASH_Addr - FLASH记录地址0xFFFFFFFF表示不使用FLASH ；在EE与FLASH都有效的情
				况下，头信息存在EE_Addr的EE中，而数据信息存FLASH_Addr的FLASH中；其
				它情况则是头信息存在开始紧接着存数据
*              evnet - 待存放事件记录内容缓冲
*              len   - 单条事件记录内容长度
*              AbsOffset -要读取数据的绝对地址
*   Output:    0--出错，其它为所读数据的长度
*****************************************************************************************/
uint16 LOG_FiFo_AbsOffset(uint32 NvmHeadAddr, void * event, uint16 AbsOffset)
{
	tLogHead LogHead;

	if(!LOG_NVM_READ(NvmHeadAddr, &LogHead, sizeof(LogHead)))
	{
		return 0;
	}
	if(!LOG_NVM_READ(LogHead.NvmDataAddr + (uint32)AbsOffset * LogHead.DataLen, event, LogHead.DataLen))
	{
		return 0;
	}
	return LogHead.DataLen;
}

/*****************************************************************************************
*   Action:    事件记录指定点(指针有效模式)输出
*   Input:     EE_Addr  - EEPROM记录地址, 0xFFFFFFFF表示不使用EE
*			FLASH_Addr - FLASH记录地址0xFFFFFFFF表示不使用FLASH ；在EE与FLASH都有效的情
				况下，头信息存在EE_Addr的EE中，而数据信息存FLASH_Addr的FLASH中；其
				它情况则是头信息存在开始紧接着存数据
*              evnet - 待存放事件记录内容缓冲
*              len   - 单条事件记录内容长度
*              point - 提取最近第N点数据, 0000表示读取最早一条, 0001表示读取最近一条.
*              TimeOffset - 事件中记录时间的位置(在事件中的地址偏移)
*              TimeLen - 时间的实际长度
*   Output:    0xffff-暂无记录 ,其它值为数据存储的绝对地址偏移
*****************************************************************************************/
uint16 LOG_FiFo_Time(uint32 NvmHeadAddr, void * event, uint16 point, uint32 TimeOffset, uint16 TimeLen)
{
    uint16 AbsOffset;
	tLogHead LogHead;

	if(!LOG_NVM_READ(NvmHeadAddr, &LogHead, sizeof(LogHead)))
	{
		return 0xffff;
	}
	AbsOffset = LOG_AbsAddr(&LogHead, point);
	if(AbsOffset == 0xffff)
	{
		return 0xffff;
	}
	if(!LOG_NVM_READ(LogHead.NvmDataAddr+ (uint32)AbsOffset * LogHead.DataLen, event, LogHead.DataLen))
	{
		return 0xffff;
	}
	return AbsOffset;
}

/*****************************************************************************************
*   Action:    修改当前记录Index与有效值
*   Input:     addr  - NVM记录地址 , Num -- 要修改的偏移条数, Len--每条记录的长度,
*			Type-- 0:往前偏移, 1:住后偏移
*   Output:    no
*****************************************************************************************/
BOOL_B LOG_ModifyPointer(uint32 NvmHeadAddr, uint16 Num, uint8 Type)
{
    uint8 TempBuf[FLASH_PAGE_SIZE];
    tLogHead LogHead = {0};
	tNvmType NvmType;

	if(!LOG_NVM_READ(NvmHeadAddr, &LogHead, sizeof(LogHead)))
	{
		return FALSE;
	}

    if(Type == 0)   //向前移
    {
		NvmType = NvmGetType(LogHead.NvmDataAddr);
		if(LogHead.NumVal <= Num)
		{
			LOG_Empty(NvmHeadAddr);
			return TRUE;
		}
		else
		{
			if(LogHead.Index == LogHead.NumVal)
			{
				LogHead.Index = LogHead.Index-Num;
			}
			else
			{
				LogHead.Index = (LogHead.Index+LogHead.NumVal-Num)%LogHead.NumVal;
			}
			LogHead.NumVal -= Num;
		}
		if(NvmType == FLASH_EXTERNAL)
		{
			uint32 BasiceAddr;
			uint16 TempLen = FLASH_PAGE_SIZE;
			
			LOG_FLASH_ERASE(LogHead.NvmDataAddr+(uint32)(LogHead.Index)*LogHead.DataLen+FLASH_SECTOR_SIZE);	//Erase next sector
			if(((uint32)(LogHead.Index)*LogHead.DataLen)%FLASH_SECTOR_SIZE != 0)
			{
				BasiceAddr = ((uint32)(LogHead.Index)*LogHead.DataLen)/FLASH_SECTOR_SIZE*FLASH_SECTOR_SIZE;
				while(BasiceAddr < (uint32)(LogHead.Index)*LogHead.DataLen)
				{
					if(BasiceAddr+TempLen > (uint32)(LogHead.Index)*LogHead.DataLen)
					{
						TempLen = (uint32)(LogHead.Index)*LogHead.DataLen - BasiceAddr;
					}
					LOG_NVM_READ(LogHead.NvmDataAddr+BasiceAddr, TempBuf, TempLen); // 读取记录
					LOG_NVM_WRITE(LogHead.NvmDataAddr+BasiceAddr+FLASH_SECTOR_SIZE, TempBuf, TempLen); // 保存记录到下一个sector
					BasiceAddr += TempLen;
				}
				TempLen = FLASH_PAGE_SIZE;
				LOG_FLASH_ERASE(LogHead.NvmDataAddr+(uint32)(LogHead.Index)*LogHead.DataLen);	//Erase current sector
				BasiceAddr = ((uint32)(LogHead.Index)*LogHead.DataLen)/FLASH_SECTOR_SIZE*FLASH_SECTOR_SIZE;
				while(BasiceAddr < (uint32)(LogHead.Index)*LogHead.DataLen)		//还原数据
				{
					if(BasiceAddr+TempLen > (uint32)(LogHead.Index)*LogHead.DataLen)
					{
						TempLen = (uint32)(LogHead.Index)*LogHead.DataLen - BasiceAddr;
					}
					LOG_NVM_READ(LogHead.NvmDataAddr+BasiceAddr+FLASH_SECTOR_SIZE, TempBuf, TempLen); // 读取记录
					LOG_NVM_WRITE(LogHead.NvmDataAddr+BasiceAddr, TempBuf, TempLen); // 保存记录到当前sector
					BasiceAddr += TempLen;
				}
				LOG_FLASH_ERASE(LogHead.NvmDataAddr+(uint32)(LogHead.Index)*LogHead.DataLen+FLASH_SECTOR_SIZE);	//Erase next sector
			}
		}
    }
    else if(Type == 1)  //往后移
    {
		return FALSE;
    }
	return LOG_NVM_WRITE(NvmHeadAddr, &LogHead, sizeof(LogHead));
}


/*****************************************************************************************
*   Action:    快速清空事件记录内容
*   Input:     EE_Addr  - EEPROM记录地址, 0xFFFFFFFF表示不使用EE
*			FLASH_Addr - FLASH记录地址0xFFFFFFFF表示不使用FLASH ；在EE与FLASH都有效的情
				况下，头信息存在EE_Addr的EE中，而数据信息存FLASH_Addr的FLASH中；其
				它情况则是头信息存在开始紧接着存数据
*   Output:    no
*****************************************************************************************/
BOOL_B LOG_Empty(uint32 NvmHeadAddr)
{
    tLogHead LogHead = {0};
	tNvmType NvmType;

	if(!LOG_NVM_READ(NvmHeadAddr, &LogHead, sizeof(LogHead)))
	{
		return FALSE;
	}
	NvmType = NvmGetType(LogHead.NvmDataAddr);
	if(NvmType == FLASH_EXTERNAL)
	{
		LOG_FLASH_ERASE(LogHead.NvmDataAddr);		//erase first page
	}
	LogHead.AllNum = 0;
	LogHead.Index = 0;
	LogHead.NumVal = 0;
	return LOG_NVM_WRITE(NvmHeadAddr, &LogHead, sizeof(LogHead));
}

/*****************************************************************************************
*   Action:    获取事件记录栈内的数据, 可以用于判定事件记录栈已满或末满
*   Input:     EE_Addr  - EEPROM记录地址, 0xFFFFFFFF表示不使用EE
*			FLASH_Addr - FLASH记录地址0xFFFFFFFF表示不使用FLASH ；在EE与FLASH都有效的情
				况下，头信息存在EE_Addr的EE中，而数据信息存FLASH_Addr的FLASH中；其
				它情况则是头信息存在开始紧接着存数据
			pHead--存放返回的信息	
*   Output:    
******************************************************************************************/
BOOL_B LOG_EntryInUseGet(uint32 NvmHeadAddr, tLogHead *pHead)
{
	if(!LOG_NVM_READ(NvmHeadAddr, pHead, sizeof(*pHead)))
	{
		return FALSE;
	}
    return TRUE;
}

