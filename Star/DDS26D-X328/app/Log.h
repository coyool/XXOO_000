#ifndef __LOG_H
#define __LOG_H

//#include "Include.h"

// Export define------------------------------------
#define FLASH_PAGE_SIZE			256
#define FLASH_SECTOR_SIZE		4096
#define FLASH_NO_ADDR			0xFFFFFFFF
#define EE_NO_ADDR				0xFFFFFFFF

/* Export typedef-----------------------------------------------------------*/
#pragma pack(push, 1)
typedef struct
{
	uint32 NvmDataAddr;		
//	uint32 EE_Addr;		//EE地址
//	uint32 FLASH_Addr;	//FLASH地址
    uint32 AllNum;      //总共计录数
    uint16 Index;       //滚动计录计数器
    uint16 NumVal;      //有效计录数
    uint16 MaxNum;		//最大条数
	uint16 DataLen;		//单条数据长度
    uint16 CheckSum;    //信息头校验和
}tLogHead;
#pragma pack(pop)

/*External functions-----------------------------------------*/
BOOL_B LOG_Init(uint32 NvmHeadAddr, uint32 NvmDataAddr, uint16 Len, uint16 MaxNum);
BOOL_B LOG_RollAdd(uint32 NvmHeadAddr, void * event);
uint16 LOG_FiFo(uint32 NvmHeadAddr, void * event, uint16 point);
uint16 LOG_FiFo_AbsOffset(uint32 NvmHeadAddr, void * event, uint16 AbsOffset);
uint16 LOG_FiFo_Time(uint32 NvmHeadAddr, void * event, uint16 point, uint32 TimeOffset, uint16 TimeLen);
BOOL_B LOG_ModifyPointer(uint32 NvmHeadAddr, uint16 Num, uint8 Type);
BOOL_B LOG_Empty(uint32 NvmHeadAddr);
BOOL_B LOG_EntryInUseGet(uint32 NvmHeadAddr, tLogHead *pHead);
#endif

