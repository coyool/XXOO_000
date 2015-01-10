#ifndef __FREEZE_H
#define __FREEZE_H


/*Export define-----------------------------------------------------------*/
#define FREEZE_DAILY_MAX_NUM						62
#define FREEZE_TIME_MAX_NUM							254

#define FREEZE_TIME_FREEZE_PERIOD					3600	//seconds

/*Export variable-----------------------------------------------------------*/
static const uint32 cDailyFreezeCaptureObjects[] =
{
	0x08000002,
	0x03200002,	//L1相正向有功总电量
	0x03220002,	//L1相反向有功总电量
	0x03340002,	//L1相组合有功电量(正向反)
/*	
	0x03000002,		//正向有功
	0x03020002		//反向有功
	0x03000002,0x03000102,0x03000202,0x03000302,0x03000402,		//正向有功
	0x03020002,0x03020102,0x03020202,0x03020302,0x03020402,		//反向有功
	0x03040002,0x03040102,0x03040202,0x03040302,0x03040402,		//正向无功
	0x03060002,0x03060102,0x03060202,0x03060302,0x03060402,		//反向无功
	0x03080002,0x03080102,0x03080202,0x03080302,0x03080402,		//I象限无功
	0x030A0002,0x030A0102,0x030A0202,0x030A0302,0x030A0402,		//II象限无功
	0x030C0002,0x030C0102,0x030C0202,0x030C0302,0x030C0402,		//III象限无功
	0x030E0002,0x030E0102,0x030E0202,0x030E0302,0x030E0402,		//IV象限无功
	*/
};

#define FREEZE_DAILY_CAPTURE_DATA_NUM		(sizeof(cDailyFreezeCaptureObjects)/4-1)

/*Export typedef-----------------------------------------------------------*/
#pragma pack(push, 1)
typedef struct
{
    tDateTime Time;
	uint32 Data[FREEZE_DAILY_CAPTURE_DATA_NUM];
}tDailyFreezeDataInfo;
/*
typedef struct
{
    tDateTime Time;
	uint32 Energy_A;
	uint32 Energy_A_;
}tTimeFreezeDataInfo;
*/
typedef struct
{
	tLogHead DailyFreezeHead;
//	tLogHead TimeFreezeHead;		//整点冻结
}tEE_FreezeBak;

typedef struct
{
	tDailyFreezeDataInfo DailyFreezeData[FREEZE_DAILY_MAX_NUM];
}tEE_FreezeDataInfo;

typedef struct
{
    tDailyFreezeDataInfo DailyFreezeData[FREEZE_DAILY_MAX_NUM];
	uint8 Reserve0[FLASH_SECTOR_SIZE - (sizeof(tDailyFreezeDataInfo)*FREEZE_DAILY_MAX_NUM%FLASH_SECTOR_SIZE)];
	uint8 Reserve1[FLASH_SECTOR_SIZE];
/*	tTimeFreezeDataInfo TimeFreezeDataInfo[FREEZE_TIME_MAX_NUM];
	uint8 Reserve2[FLASH_SECTOR_SIZE - (sizeof(tTimeFreezeDataInfo)*FREEZE_TIME_MAX_NUM%FLASH_SECTOR_SIZE)];
	uint8 Reserve3[FLASH_SECTOR_SIZE];
	*/
}tFLASH_FreezeDataInfo;
#pragma pack(pop)

/*-Export functions------------------------------------*/
uint16 FreezeInit(void);
void FreezeProcess(void);
void FreezeResetFreezeTime(void);
void FreezeResetData(void);
uint16 FreezeParaGet(uint32 DataId, uint8 *pBuf);
uint8 FreezeParaSet(uint32 DataId, uint8 *pBuf, uint16 Len);

#endif

