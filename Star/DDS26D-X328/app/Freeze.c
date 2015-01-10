
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Freeze.c 
* 文件标识：
* 摘要：冻结数据处理单元

* 当前版本：1.0.0 
* 作者：冯秋雄
* 完成日期：20130927
*******************************************************************/
//#include "Freeze.h"
#include "Include.h"

/*Private define---------------------------------------------*/
#define FREEZE_DAILY_REPAIR_NUM			7

#define EE_DAILY_FREEZE_HEAD_ADDR			EE_BAK_STRUCT_ADDR(EE_FreezeBak.DailyFreezeHead)

#define EE_DAILY_FREEZE_DATA_ADDR			EE_STRUCT_ADDR(EE_FreezeDataInfo.DailyFreezeData)


/*Private typedef---------------------------------------------*/
typedef struct
{
	uint16 Year;
	uint8 Month;
	uint8 Day;
	uint8 Week;
	uint8 Hour;
	uint8 Minute;
	uint8 Second;
}tFreezeTime;

/*Private variable--------------------------------------------*/
static tFreezeTime sDailyFreezeTime;		//日冻结时间
//static tFreezeTime	sTimeFreezeTime;			//整点冻结时间

/*static const uint32 cTimeFreezeCaptureObjects[] =
{
	0x08000002,0x03000002,0x03020002
};*/

/*Private functions-------------------------------------------*/
static tDateTime FreezeTimeToDateTime(const tDateTime *pDateTime, const tFreezeTime *pTime)
{
	tDateTime DateTime = *pDateTime;

	DateTime.HighByteYear = (uint8)(pTime->Year >> 8);
	DateTime.LowByteYear = (uint8)(pTime->Year);
	DateTime.Month = pTime->Month;
	DateTime.Day = pTime->Day;
	DateTime.Week = pTime->Week;
	DateTime.Hour = pTime->Hour;
	DateTime.Minute = pTime->Minute;
	DateTime.Second = pTime->Second;
	return DateTime;
}

static tFreezeTime FreezeDateTimeToTime(const tDateTime *pDateTime)
{
	tFreezeTime Time;

	Time.Year = ((uint16)(pDateTime->HighByteYear)<<8) + pDateTime->LowByteYear;
	Time.Month = pDateTime->Month;
	Time.Day = pDateTime->Day;
	Time.Week = pDateTime->Week;
	Time.Hour = pDateTime->Hour;
	Time.Minute = pDateTime->Minute;
	Time.Second = pDateTime->Second;
	return Time;
}

/*****************************************************************************************
*   SaveMonthBillingData
入口参数: pTime--时间指针,不允许有通配,Type--0表示当前结算日期，1表示上一个结算日期，2表示下
        一个结算日期
出口参数: 无
返回参数：tFreezeDailyTime
函数功能：根据传入的时间计算当月或是上月的月结算时间

*****************************************************************************************/
static tFreezeTime FreezeCalcDailyPeriod(const tDateTime *pTime, ePeriodType Type)
{
	tDateTime SocTime = *pTime;
	uint16 Year = ((uint16)(SocTime.HighByteYear)<<8) + SocTime.LowByteYear;;
	uint8 Day;

	if(Type == PERIOD_LAST)
	{
		if(SocTime.Day > 1)
		{
			SocTime.Day--;
		}
		else
		{
			SocTime.Month--;
            if(SocTime.Month == 0)
            {
                SocTime.Month = 12;
                if(Year > BASIC_YEAR)
                {
                    Year--;
					SocTime.HighByteYear = (uint8)(Year >> 8);
					SocTime.LowByteYear = (uint8)(Year);
                }
                else
                {
                    SocTime.Month = 0;
                }
            }
			SocTime.Day = GetMonthDays(&SocTime); //上一个月的最后一天
		}
	}
	else if(Type == PERIOD_NEXT)
	{
		Day = GetMonthDays(&SocTime);
        SocTime.Day++;
        if(SocTime.Day > Day)
        {
            SocTime.Day = 1;
            SocTime.Month++;
            if(SocTime.Month > 12)
            {
                SocTime.Month = 1;
                if(Year < BASIC_YEAR+99)
                {
                    Year++;
                }
                else
                {
                    Year = BASIC_YEAR;
                }
				SocTime.HighByteYear = (uint8)(Year >> 8);
				SocTime.LowByteYear = (uint8)(Year);
            }
        }
	}
	SocTime.Hour = 0;
	SocTime.Minute = 0;
	SocTime.Second = 0;
	SocTime.Week = GetWeekDay(&SocTime);
	return FreezeDateTimeToTime(&SocTime);
}

static void FreezeCalcDailyFreezeTime(const tDateTime *pDateTime)
{
	uint8 Result;
	tDateTime FreezeDateTime;
	
	sDailyFreezeTime = FreezeCalcDailyPeriod(pDateTime, PERIOD_CURRENT);
	FreezeDateTime = FreezeTimeToDateTime(pDateTime, &sDailyFreezeTime);
	Result = ClockCmp(pDateTime, &FreezeDateTime);
	if(Result == TIME1_LARGE_TIME2)//当前时间夸过结算点取下一次结算时间
	{
		sDailyFreezeTime = FreezeCalcDailyPeriod(pDateTime, PERIOD_NEXT);
	}
}

static void FreezeDailyDataFreeze(tDateTime *pDateTime)
{
	tDailyFreezeDataInfo Data = {0};
	uint8 i;
	uint16 Len, TempLen;

	Data.Time = *pDateTime;
	Data.Time.Second = 0;
	Len = sizeof(tDateTime);
	for(i=0; i<sizeof(cDailyFreezeCaptureObjects)/4; i++)
	{
		if(cDailyFreezeCaptureObjects[i] == 0x08000002)		//except time
		{
			continue;
		}
		TempLen = API_GetDataValue(cDailyFreezeCaptureObjects[i], (uint8*)&Data+Len);
		if(TempLen > 4)
		{
			TempLen = 4;
		}
		Len += TempLen;
	}
	LOG_RollAdd(EE_DAILY_FREEZE_HEAD_ADDR, &Data);
}

static uint16 FreezeDailyRepair(const tDateTime *pDateTime)
{
	tDateTime DateTime, DateTime1, DateTime2;
	tFreezeTime DailyTime1, DailyTime2;
	uint8 Result, i, j;
	uint16 Temp;

	DailyTime1 = FreezeCalcDailyPeriod(pDateTime, PERIOD_CURRENT);
	Temp = LOG_FiFo_Time(EE_DAILY_FREEZE_HEAD_ADDR, &DateTime, 
			1, OffsetOfStruct(tDailyFreezeDataInfo, Time), sizeof(DateTime));
	if(Temp == 0xffff)	//没有结算点则取掉电时间
	{
		Clock_GetPowerDownTime(&DateTime);//get power down time
	}
	DailyTime2 = FreezeCalcDailyPeriod(&DateTime, PERIOD_CURRENT);
	DateTime2 = FreezeTimeToDateTime(&DateTime, &DailyTime2);
	for(i=0; i<FREEZE_DAILY_REPAIR_NUM; i++)
	{
		DateTime1 = FreezeTimeToDateTime(&DateTime, &DailyTime1);
		Result = ClockCmp(&DateTime1, &DateTime2);
		if(Result == TIME1_LARGE_TIME2)
		{
			DailyTime1 = FreezeCalcDailyPeriod(&DateTime1, PERIOD_LAST);
		}
		else
		{
			break;
		}
	}
	j = i;
	for(;j>0; j--)
	{
		DateTime1 = FreezeTimeToDateTime(&DateTime, &DailyTime1);
		DailyTime1 = FreezeCalcDailyPeriod(&DateTime1, PERIOD_NEXT);
		DateTime1 = FreezeTimeToDateTime(&DateTime, &DailyTime1);
		FreezeDailyDataFreeze(&DateTime1);			//freeze the data
		if(PowerDownDetect() != 0)
		{
			return POWER_UP_INIT_FAIL;
		}
	}
	return POWER_UP_INIT_SUCESS;
}

static uint16 FreezePeriodDataGet(uint32 DataId, uint8 *pBuf)
{
	uint8 Point;
	uint8 *data = pBuf;
	uint16 Index, Result = 0x8000+DA_READ_WRITE_DENIED;
	tDailyFreezeDataInfo DailyFreezeDataInfo;
	
	Point = *data;
	if(Point == 0)
	{
		return Result;
	}
	Index = LOG_FiFo(EE_DAILY_FREEZE_HEAD_ADDR, &DailyFreezeDataInfo, Point);
	if(Index == 0xffff)
	{
		memset(&DailyFreezeDataInfo, 0 ,sizeof(DailyFreezeDataInfo));
	}       
    *(tDailyFreezeDataInfo*)pBuf = *(tDailyFreezeDataInfo*)((uint8*)&DailyFreezeDataInfo);
	
	return Result;
}



/*Export functions-------------------------------------------*/
uint16 FreezeInit(void)
{
	tDateTime DateTime;
	
	Clock_GetTime(&DateTime);
	LOG_Init(EE_DAILY_FREEZE_HEAD_ADDR, EE_DAILY_FREEZE_DATA_ADDR, sizeof(tDailyFreezeDataInfo), FREEZE_DAILY_MAX_NUM);
	if(PowerDownDetect() != 0)
	{
		return POWER_UP_INIT_FAIL;
	}
/*	LOG_Init(EE_TIME_FREEZE_HEAD_ADDR, FLASH_TIME_FREEZE_DATA_ADDR, sizeof(tTimeFreezeDataInfo), FLASH_TIME_FREEZE_MAX_SPACE/sizeof(tTimeFreezeDataInfo));
	if(PowerDownDetect() != 0)
	{
		return POWER_UP_INIT_FAIL;
	}*/
	//-------daiyl freeze---
	FreezeCalcDailyFreezeTime(&DateTime);
	//--------整点冻结
//	FreezeCalcTimeBillingTime(&DateTime);
	TaskAdd(FreezeProcess, 600, 10000, 1);
	return FreezeDailyRepair(&DateTime);
}

/*****************************************************************************************
*   Action:    判断是否需要冻结,并更新下一次冻结时间,需要每秒调用一次
*   Input:     无
*   Output:    无
*****************************************************************************************/
void FreezeProcess(void)
{
	tDateTime DateTime, FreezeDateTime;
	uint8 Result;
	
	Clock_GetTime(&DateTime);
	//------日冻结
	FreezeDateTime = FreezeTimeToDateTime(&DateTime, &sDailyFreezeTime);
	Result = ClockCmp(&DateTime, &FreezeDateTime);
	if((Result == TIME1_EQUAL_TIME2) || (Result == TIME1_LARGE_TIME2))
	{
		FreezeDailyDataFreeze(&FreezeDateTime);			//freeze the data
		sDailyFreezeTime = FreezeCalcDailyPeriod(&DateTime, PERIOD_NEXT);
	}
	else if(Result == TIME_INVALID)
	{
		FreezeCalcDailyFreezeTime(&DateTime);
	}
	//------整点冻结
/*	FreezeDateTime = FreezeTimeToDateTime(&DateTime, &sTimeFreezeTime);
	Result = Clock_CmpDateTime12(&DateTime, &FreezeDateTime);
	if((Result == TIME1_EQUAL_TIME2) || (Result == TIME1_LARGE_TIME2))
	{
		FreezeTimeDataFreeze(&FreezeDateTime);			//freeze the data
		sTimeFreezeTime = FreezeCalcTimePeriod(&DateTime, PERIOD_NEXT);
	}
	else if(Result == TIME_INVALID)
	{
		FreezeCalcTimeBillingTime(&DateTime);
	}
	*/
}

/*****************************************************************************************
*   Action:    更新下一次冻结时间,在发生校时调用
*   Input:     无
*   Output:    无
*****************************************************************************************/
void FreezeResetFreezeTime(void)
{
	tDateTime DateTime;
	
	Clock_GetTime(&DateTime);
	FreezeCalcDailyFreezeTime(&DateTime);
//	FreezeCalcTimeBillingTime(&DateTime);
}

/*****************************************************************************************
*   Action:    清除冻结相关数据
*   Input:     无
*   Output:    无
*****************************************************************************************/
void FreezeResetData(void)
{
	LOG_Empty(EE_DAILY_FREEZE_HEAD_ADDR);
	//LOG_Empty(EE_TIME_FREEZE_HEAD_ADDR);
}

/*****************************************************************************************
*   Action:    冻结相关数据读取
*   Input:     DataId--数据ID；pBuf--数据存放的指针
*   Output:    读取数据的长度
*****************************************************************************************/
uint16 FreezeParaGet(uint32 DataId, uint8 *pBuf)
{
	uint16 Result = 0x8000+DA_OTHER_REASON;
	//uint8 i;
	//tLogHead LogHead = {0};

	Result = FreezePeriodDataGet(DataId, pBuf);
	//switch(DataId)
	//{
		//-----daily freeze---------
		/*case 0x07010002:	//Buf
			Result = LoadProfileBufGet(EE_DAILY_FREEZE_HEAD_ADDR, pBuf, FREEZE_DAILY_MAX_NUM);
			break;
		case 0x07010003:	//capture objects
			pBuf[0] = sizeof(cDailyFreezeCaptureObjects)/4;
			Result = 1;
			for(i=0; i<pBuf[0]; i++)
			{
				*(uint32*)(pBuf+Result) = cDailyFreezeCaptureObjects[i];
				Result += 4;
			}
			break;
		case 0x07010004:	//catpure period
			*(uint32*)pBuf = 86400;
			Result = 4;
			break;
		case 0x07010005:	//sort method
			*pBuf = 1;
            Result = 1;
			break;
		case 0x07010006:	//sort object
			*pBuf = 0;
            Result = 1;
			break;
		case 0x07010007:	//entries in use
			if(LOG_EntryInUseGet(EE_DAILY_FREEZE_HEAD_ADDR, &LogHead))
			{
				if(LogHead.NumVal > FREEZE_DAILY_MAX_NUM)
				{
					*(uint32*)pBuf = FREEZE_DAILY_MAX_NUM;
				}
				else
				{
					*(uint32*)pBuf = LogHead.NumVal;
				}
			}
			Result = 4;
			break;	
		case 0x07010008:	//profile entries
			*(uint32*)pBuf = FREEZE_DAILY_MAX_NUM;
			Result = 4;
			break;*/
		//---整点冻结
/*		case 0x07030002:	//Buf
			Result = LoadProfileBufGet(EE_TIME_FREEZE_HEAD_ADDR, pBuf, FREEZE_TIME_MAX_NUM);
			break;
		case 0x07030003:	//capture objects
			pBuf[0] = sizeof(cTimeFreezeCaptureObjects)/4;
			Result = 1;
			for(i=0; i<pBuf[0]; i++)
			{
				*(uint32*)(pBuf+Result) = cTimeFreezeCaptureObjects[i];
				Result += 4;
			}
			break;
		case 0x07030004:	//catpure period
			*(uint32*)pBuf = 3600;
			Result = 4;
			break;
		case 0x07030005:	//sort method
			*pBuf = 1;
            Result = 1;
			break;
		case 0x07030006:	//sort object
			*pBuf = 0;
            Result = 1;
			break;
		case 0x07030007:	//entries in use
			if(LOG_EntryInUseGet(EE_TIME_FREEZE_HEAD_ADDR, &LogHead))
			{
				if(LogHead.NumVal > FREEZE_TIME_MAX_NUM)
				{
					*(uint32*)pBuf = FREEZE_TIME_MAX_NUM;
				}
				else
				{
					*(uint32*)pBuf = LogHead.NumVal;
				}
			}
			else
			{
				*(uint32*)pBuf = 0;
			}
			Result = 4;
			break;	
		case 0x07030008:	//profile entries
			*(uint32*)pBuf = FREEZE_TIME_MAX_NUM;
			Result = 4;
			break;	
			*/
		//default:
		//	break;
	//}
	return Result;
}

/*****************************************************************************************
*   Action:    冻结相关数据设置
*   Input:     DataId--数据ID；pBuf--数据存放的指针,Len--所设数据的长度
*   Output:    
*****************************************************************************************/
uint8 FreezeParaSet(uint32 DataId, uint8 *pBuf, uint16 Len)
{
	uint8 Result = DA_READ_WRITE_DENIED;
	
	switch(DataId)
	{
		default:
			break;
	}
	return Result;
}

