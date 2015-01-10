#ifndef __LOADPROFILE_H
#define __LOADPROFILE_H

/*Export define------------------------------------------------*/
#define LOADPFOFILE_MAX_NUM						57600	
#define LOADPROFILE_CATPURE_PERIOD				900		//seconds
#define LOADPROFILE_MAX_CAPTURE_OBJECTS			14

/*Export typedef----------------------------------------------*/
#pragma pack(push, 1)
typedef enum
{
	PERIOD_CURRENT,
	PERIOD_LAST,	
	PERIOD_NEXT,
}ePeriodType;

typedef struct
{
	uint32 DataId;
	uint8 DataLen;
}tProfileCaptureDataInfo;

typedef struct
{
    tDateTime Time;
	uint32 Data[LOADPROFILE_MAX_CAPTURE_OBJECTS];
/*
	uint32 Energy_DEMAND_A;
	uint32 Energy_DEMAND_A_;
	uint32 Energy_DEMAND_S;
	uint32 Energy_DEMAND_S_;
	*/
}tLoadProfileDataInfo;

typedef struct
{
	uint8 Num;
	uint32 Objects[LOADPROFILE_MAX_CAPTURE_OBJECTS];
}tLoadProfileCaptureObjects;

typedef struct
{
	uint32 Period;
	tLoadProfileCaptureObjects CaptureObjects;
	uint16 CheckSum;
}tLoadProfilePara;

typedef struct
{
	tLoadProfilePara LoadProfilePara;
}tEE_LoadProfileInfo;

typedef struct
{
	tLogHead LoadProfileHead;
}tEE_LoadProfileBak;

typedef struct
{
    tLoadProfileDataInfo LoadProfileDataInfo[LOADPFOFILE_MAX_NUM];
	uint8 Reserve0[FLASH_SECTOR_SIZE - (sizeof(tLoadProfileDataInfo)*LOADPFOFILE_MAX_NUM%FLASH_SECTOR_SIZE)];
	uint8 Reserve1[FLASH_SECTOR_SIZE];
}tFLASH_LoadProfileDataInfo;

#pragma pack(pop)


/*Export functions----------------------------------------------*/
void LoadProfile_Init(void);
void LoadProfileProcess(void);
uint16 LoadProfileBufGet(uint32 NvmHeadAddr, uint8 *pValue, uint16 LogNumMax);
void LoadProfileResetCaptureTime(void);
void LoadProfileResetData(void);
uint16 LoadProfileParaGet(uint32 DataId, uint8 *pBuf);
uint8 LoadProfileParaSet(uint32 DataId, uint8 *pBuf, uint16 Len);
#endif

