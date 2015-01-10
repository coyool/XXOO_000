/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：Billing.c
* 文件标识：
* 摘要：结算数据管理

* 当前版本：1.0.0 
* 作者：冯秋雄
* 完成日期：20130927
*******************************************************************/
//#include "Billing.h"
#include "Include.h"

/*Private typedef--------------------------------------------*/

/*Private define---------------------------------------------*/
#define EE_MONTH_BILLING_HEAD_ADDR					EE_BAK_STRUCT_ADDR(EE_MonthBillingBak.MonthBillingFreezeHead)

#define EE_BILLING_DATA_ADDR						EE_STRUCT_ADDR(EE_MonthBillingData.MonthBilling)


/*Public variable---------------------------------------------*/

/*Private variable--------------------------------------------*/
static tDateTime sMonthBillingTime;

//static tMaxDemandData sMaxData[MONTH_BILLING_MAX_DEMAND_NUM] = {0};

/*Private functions-------------------------------------------*/
/*****************************************************************************************
*   Action:    月结初始化，在上电时调用,
*   Input:     无
*   Output:    无
*****************************************************************************************/
static tDateTime MonthBillingPeriod(const tDateTime *pTime, ePeriodType Type)
{
	tDateTime DateTime2 = *pTime;
	uint16 Year = ((uint16)(DateTime2.HighByteYear)<<8) + DateTime2.LowByteYear;
	
	if(Type == PERIOD_LAST)	//上一个
	{
		if(DateTime2.Month > 1)
		{
			DateTime2.Month--;
		}
		else
		{
			DateTime2.Month = 12;
			Year--;
			DateTime2.HighByteYear = (uint8)(Year>>8);
			DateTime2.LowByteYear = (uint8)(Year);
		}
	}
	else if(Type == PERIOD_NEXT)	//下一个
	{
		if(DateTime2.Month < 12)
		{
			DateTime2.Month++;
		}
		else
		{
			DateTime2.Month = 1;
			Year++;
			DateTime2.HighByteYear = (uint8)(Year>>8);
			DateTime2.LowByteYear = (uint8)(Year);
		}
	}
	DateTime2.Day = 1;
	DateTime2.Hour = 0;
	DateTime2.Minute = 0;
	DateTime2.Second = 0;
	DateTime2.Week = GetWeekDay(&DateTime2);
	return DateTime2;
}

static void BillingCalcCurBillingTime(const tDateTime *pDateTime)
{
	uint8 Result;
	
	sMonthBillingTime = MonthBillingPeriod(pDateTime, PERIOD_CURRENT);
	Result = ClockCmp(pDateTime, &sMonthBillingTime);
//	if((Result == TIME1_EQUAL_TIME2) || (Result == TIME1_LARGE_TIME2))//当前时间夸过结算点取下一次结算时间
	if(Result == TIME1_LARGE_TIME2)//当前时间夸过结算点取下一次结算时间
	{
		sMonthBillingTime = MonthBillingPeriod(pDateTime, PERIOD_NEXT);
	}
}

/*****************************************************************************************
*   Action:    月结相关的电能数据获取
*   Input:     无
*   Output:    无
*****************************************************************************************/
static uint16 BillingDataGet(uint8 *pBuf)
{
	uint16 i,Len =0,Temp;
	
	for(i=0; i<sizeof(cMonthBillingObjects)/4; i++)
	{
		if(cMonthBillingObjects[i] == 0x08000002)		//except time
		{
			continue;
		}
		Temp = API_GetDataValue(cMonthBillingObjects[i], pBuf+Len);
		if(Temp > sizeof(tDateTime))
		{
			Temp = 4;
		}
		Len += Temp;
	}
	return Len;
}

/*****************************************************************************************
*   Action:    月结保存函数，保存一次月结数据
*   Input:     无
*   Output:    无
*****************************************************************************************/
#pragma optimize=none
static void BillingDataSave(tDateTime *pTime)
{
	uint16 Len, Temp;
	tMonthBillingDataInfo MonthBillingDataInfo = {0};
	uint8 *pBuf = (uint8*)&MonthBillingDataInfo;

	MonthBillingDataInfo.Time = *pTime;
	MonthBillingDataInfo.Time.Second = 0;		//keep second equal to zero
	Len = sizeof(tDateTime);
	Temp = BillingDataGet(pBuf+Len);
	Len += Temp;
	*(uint16*)(pBuf+Len) = CALC_CHECKSUM(pBuf, Len);
	LOG_RollAdd(EE_MONTH_BILLING_HEAD_ADDR, pBuf);
}

/*****************************************************************************************
*   Action:    月结修补函数，在掉电上电或者校时时调用
*   Input:     无
*   Output:    无
*****************************************************************************************/
static uint16 BillingMonthRepair(const tDateTime *pDateTime)
{
	tDateTime DateTime1, DateTime2, TempDateTime;
	uint8 Result;
	vuint8 i, j;
	uint8 k;
	uint16 Temp;
	
	DateTime1 = MonthBillingPeriod(pDateTime, PERIOD_CURRENT);
	Temp = LOG_FiFo_Time(EE_MONTH_BILLING_HEAD_ADDR, &TempDateTime, 
		1, OffsetOfStruct(tMonthBillingDataInfo, Time), sizeof(TempDateTime));
	if(Temp == 0xffff)	//没有结算点则取掉电时间
	{
		Clock_GetPowerDownTime(&TempDateTime);//get power down time
	}
	DateTime2 = MonthBillingPeriod(&TempDateTime, PERIOD_CURRENT);
	for(i=0; i<MONTH_BILLING_MAX_NUM; i++)		//计算补点数
	{
		Result = ClockCmp(&DateTime1, &DateTime2);
		if(Result == TIME1_LARGE_TIME2)
		{
			DateTime1 = MonthBillingPeriod(&DateTime1, PERIOD_LAST);
		}
		else
		{
			break;
		}
	}
	if(PowerDownDetect() != 0)
	{
		return POWER_UP_INIT_FAIL;
	}
	j = i;
	k = i;
	if(j == MONTH_BILLING_MAX_NUM)		//第一个补的数据夸过了月需要把需量清零(即超出了需量周期)
	{
//		my_memset(sMaxData, 0, sizeof(sMaxData));
	}
	for(; j>0; j--)
	{
		DateTime1 = MonthBillingPeriod(&DateTime1, PERIOD_NEXT);
		BillingDataSave(&DateTime1);
		if(j == k)	//reset maximum demand
		{
//			my_memset(sMaxData, 0, sizeof(sMaxData));
		}
		DRV_WD_FeedDog();
		if(PowerDownDetect() != 0)
		{
			return POWER_UP_INIT_FAIL;
		}
	}
	return POWER_UP_INIT_SUCESS;
}

static uint16 BillingGetDataLen(void)
{
	tMonthBillingDataInfo MonthBillingDataInfo;
	uint16 Len;
	
	Len = BillingDataGet((uint8*)&MonthBillingDataInfo);
	Len += sizeof(tDateTime);
	return Len;
}

#pragma optimize=none
static uint16 BillingLogInit(uint32 HeadAddr, uint32 DataAddr)
{
	uint16 Len;

	Len = BillingGetDataLen();
	//Len += 2;	//加上2个字节的校验和   20140722发现问题，会覆盖后面的数据。
	if(PowerDownDetect() != 0)
	{
		return POWER_UP_INIT_FAIL;
	}
	LOG_Init(HeadAddr, DataAddr, Len, MONTH_BILLING_MAX_NUM);
    return POWER_UP_INIT_SUCESS;
}

#pragma optimize=none
static uint16 BillingDataInit(void)
{
	tDateTime DateTime;

	Clock_GetTime(&DateTime);
	BillingCalcCurBillingTime(&DateTime);
	if(POWER_UP_INIT_SUCESS != BillingLogInit(EE_MONTH_BILLING_HEAD_ADDR, EE_BILLING_DATA_ADDR))
	{
		return POWER_UP_INIT_FAIL;
	}
	if(PowerDownDetect() != 0)
	{
		return POWER_UP_INIT_FAIL;
	}
	if(PowerDownDetect() != 0)
	{
		return POWER_UP_INIT_FAIL;
	}
	return BillingMonthRepair(&DateTime);
}

/*static uint8 BillingPeriodDataPointGet(uint32 DataId)
{
	uint8 Point;
	uint8 Item = (uint8)(DataId>>8);
	
	Point = (Item>>4);
	return Point;
}*/

/*static uint16 BillingGetDataPosition(uint32 DataId, uint8 *pBuf)
{
	uint16 i,Len =0,Temp;
	uint8 TempBuf[20];
	
	for(i=0; i<sizeof(cMonthBillingObjects)/4; i++)
	{
		if(cMonthBillingObjects[i] == 0x08000002)		//except time
		{
			Temp = 12;
		}
		else
		{
			Temp = API_GetDataValue(cMonthBillingObjects[i], TempBuf);
			if(Temp > sizeof(tDateTime))
			{
				Temp = 4;
			}
		}
		if(DataId == cMonthBillingObjects[i])
		{
			break;
		}
		Len += Temp;
	}
	*pBuf = Temp;
	return Len;
}*/

static uint16 BillingPeriodDataGet(uint32 DataId, uint8 *pBuf)
{
	uint8 Point;
	uint8 *data = pBuf;
	uint16 Index, Result = 0x8000+DA_READ_WRITE_DENIED;
	tMonthBillingDataInfo MonthBillingDataInfo;
	
	//Point = BillingPeriodDataPointGet(DataId);
	Point = *data;
	if(Point == 0)
	{
		return Result;
	}
	Index = LOG_FiFo(EE_MONTH_BILLING_HEAD_ADDR, &MonthBillingDataInfo, Point);
	if(Index == 0xffff)
	{
		memset(&MonthBillingDataInfo, 0 ,sizeof(MonthBillingDataInfo));
	}       
    *(tMonthBillingDataInfo*)pBuf = *(tMonthBillingDataInfo*)((uint8*)&MonthBillingDataInfo);
	
	/*DataId &= 0xffff0fff;
	Result = BillingGetDataPosition(DataId, pBuf);
	Point = *pBuf;
	if(Point == 4)
	{
		*(uint32*)pBuf = *(uint32*)((uint8*)&MonthBillingDataInfo+Result);
	}
	else 
	{
		*(tDateTime*)pBuf = *(tDateTime*)((uint8*)&MonthBillingDataInfo+Result);
	}
	Result = Point; */
	return Result;
}

/*Export functions-------------------------------------------*/
/*****************************************************************************************
*   Action:    月结初始化，在上电时调用,
*   Input:     无
*   Output:    无
*****************************************************************************************/
uint16 BillingPowerUpInit(void)
{
	BillingBatteryUpInit();
	TaskAdd(BillingProcess, 0, 1000, 1);
	return BillingDataInit();
}

void BillingBatteryUpInit(void)
{
	//BillingMaxDemandInit();
}

uint8 BillingIsDataIdBillingPeriod(uint32 DataId)
{
	uint8 Item = (uint8)(DataId>>8); 
	uint16 i;
	uint8 Result = 0;

	if(((Item & 0xf0) >> 4) > MONTH_BILLING_MAX_NUM)
	{
		return 0;
	}
	DataId &= 0xFFFF0FFF;
	for(i=0; i<sizeof(cMonthBillingObjects)/4; i++)
	{
		if(DataId == cMonthBillingObjects[i])
		{
			break;
		}
		if((DataId&0xffffff00) == (cMonthBillingObjects[i]&0xffffff00))
		{
			Result = 0x01;
		}
	}
	if(i >= sizeof(cMonthBillingObjects)/4)
	{
		if(Result == 0x01)
		{
			if((Item&0xf0) != 0)
			{
				return 0x02;		//属于结算数据但是属性不同
			}
		}
		return FALSE;
	}
	else
	{
		if((Item&0xf0) != 0)
		{
			return 0x01;		//属于结算数据且属性也一致
		}
	}
	return FALSE;
}

/*****************************************************************************************
*   Action:    最大需量掉电保存
*   Input:     无
*   Output:    无
*****************************************************************************************/
void BillingPowerDownProcess(void)
{
	/*
	tMaxDemandInfo MaxDemandInfo;
	uint8 i;

	NvmBytesRead(EE_BILLING_MAX_DEMAND_DATA_ADDR, &MaxDemandInfo, sizeof(MaxDemandInfo));
	if(!IsCheckSumRight(MaxDemandInfo))
	{
		my_memset(&MaxDemandInfo, 0, sizeof(MaxDemandInfo));
	}
	for(i=0; i<MONTH_BILLING_MAX_DEMAND_NUM; i++)
	{
		MaxDemandInfo.MaxDemand[i] = sMaxData[i];
	}
	CalcCheckSum(MaxDemandInfo);
	NvmBytesWrite(EE_BILLING_MAX_DEMAND_DATA_ADDR, &MaxDemandInfo, sizeof(MaxDemandInfo));
	*/
}

/*****************************************************************************************
*   Action:   清最大需量
*   Input:     无
*   Output:    无
*****************************************************************************************/
void BillingDataReset(void)
{
	tDateTime DateTime;
//	uint8 i;
	
	Clock_GetTime(&DateTime);
	BillingCalcCurBillingTime(&DateTime);
/*	for(i = 0; i<sizeof(sMaxData)/sizeof(tMaxDemandData); i++)		//reset maximum demand
	{
		my_memset(&sMaxData[i], 0, sizeof(tMaxDemandData));
	}
	*/
}

void BillingResetBillingTime(void)
{
	tDateTime DateTime;
	
	Clock_GetTime(&DateTime);
	BillingCalcCurBillingTime(&DateTime);
}

void BillingGetBillingTime(tDateTime *pDateTime, uint8 Flag)
{
	tDateTime DateTime;

	Clock_GetTime(&DateTime);
	if(Flag == 0)	//即将来到的结算日
	{
		*pDateTime = sMonthBillingTime;
	}
	else	//当前月的结算日
	{
		*pDateTime = MonthBillingPeriod(&DateTime, PERIOD_CURRENT);
	}
}

/*****************************************************************************************
*   Action:    月结进程函数，每秒调用
*   Input:     无
*   Output:    无
*****************************************************************************************/
#pragma optimize=none
void BillingProcess(void)
{
	tDateTime DateTime;
	uint8 Result;

	Clock_GetTime(&DateTime);
	Result = ClockCmp(&DateTime, &sMonthBillingTime);
	if((Result == TIME1_EQUAL_TIME2) || (Result == TIME1_LARGE_TIME2))
	{
		BillingDataSave(&sMonthBillingTime);//billing
		sMonthBillingTime = MonthBillingPeriod(&DateTime, PERIOD_NEXT);
//		my_memset(sMaxData, 0, sizeof(sMaxData));		//clear max demand
		//DemandDataClear();			//清需量
	}
	else if(Result == TIME_INVALID)
	{
		BillingCalcCurBillingTime(&DateTime);
	}
}

/*****************************************************************************************
*   Action:    月结相关数据读取
*   Input:     DataId--数据ID；pBuf--数据存放的指针
*   Output:    读取数据的长度
*****************************************************************************************/
uint16 BillingParaGet(uint32 DataId, uint8 *pBuf)
{
	uint16 Result = 0x8000+DA_READ_WRITE_DENIED;//, i;
	//tLogHead LogHead;
	//uint8 Temp;

    Result = BillingPeriodDataGet(DataId, pBuf);
	/*Temp = BillingIsDataIdBillingPeriod(DataId);
	if(Temp == 1)
	{
		return BillingPeriodDataGet(DataId, pBuf);
	}
	else if(Temp == 2)
	{
		DataId &= 0xffff0fff;		//读属性的则直接去电能单元读取
	}
	switch((uint8)(DataId>>24))	//月结数据
	{
		case 0x07:
			switch((uint8)DataId)
			{
				case 2:
					Result = LoadProfileBufGet(EE_MONTH_BILLING_HEAD_ADDR, pBuf, MONTH_BILLING_MAX_NUM);
					break;
				case 3:
					pBuf[0] = sizeof(cMonthBillingObjects)/4;
					Result = 1;
					for(i=0; i<pBuf[0]; i++)
					{
						*(uint32*)(pBuf+Result) = cMonthBillingObjects[i];
						Result += 4;
					}
					break;
				case 4:	// capture_period
					*(uint32*)pBuf = 0;
					Result = 4;
					break;
				case 7:	// entries_in_use
					if(LOG_EntryInUseGet(EE_MONTH_BILLING_HEAD_ADDR, &LogHead))
					{
						if(LogHead.NumVal > MONTH_BILLING_MAX_NUM)
						{
							*(uint32*)pBuf = MONTH_BILLING_MAX_NUM;
						}
						else
						{
							*(uint32*)pBuf = LogHead.NumVal;
						}
					}
					Result = 4;
					break;
				case 8:	// profile_entries
					*(uint32*)pBuf = MONTH_BILLING_MAX_NUM;
					Result = 4;
					break;		
				default:
					break;
			}
			break;
		case 0x04:		//最大需量
			Result = BillingMaxDemandGet(DataId, pBuf);
			break;
		case 0x09:		//清最大需量脚本
			if((uint8)DataId == 2)
			{
				*pBuf = sizeof(cClearMaxDemandScript)/4;
				Result = 1;
				for(i=0; i<*pBuf; i++)
				{
					*(uint32*)(pBuf+Result) = cClearMaxDemandScript[i];
					Result += 4;
				}
			}
			break;
			
		default:
			break;
	}*/
	return Result;
}

/*****************************************************************************************
*   Action:    冻结相关数据设置
*   Input:     DataId--数据ID；pBuf--数据存放的指针,Len--所设数据的长度
*   Output:    
*****************************************************************************************/
uint8 BillingParaSet(uint32 DataId, uint8 *pBuf, uint16 Len)
{	
	switch(DataId)
	{
		case 0x09050081:	//电表清零脚本
			energyEepError = 0;         //清电能出错标志
//			EventInsertLog(EVENT_METER_CLEAR, NULL);
			EventClear();//清事件
		    EnergyClear();//清电量
		    DRV_WD_FeedDog();		//喂狗
		    FreezeResetData();//清日冻结
//			my_memset(sMaxData, 0, sizeof(sMaxData));	//最大需量清零
			DRV_WD_FeedDog();		//喂狗
			EnergyClear();//清电能量
			LOG_Empty(EE_MONTH_BILLING_HEAD_ADDR);		//月结清零
			DRV_WD_FeedDog();		//喂狗
			//DemandDataClear();		//清需量
			return DA_SUCCESS;
		default:
			break;
	}
	return DA_READ_WRITE_DENIED;
}

