/*****************************************************************************************
*  	Function:  时钟单元，
*   
*    Author:	冯秋雄
*    Created on:	2013-09-12
*****************************************************************************************/
//#include "Clock.h"
#include "Include.h"

/*Private typedef--------------------------------------------*/
typedef enum
{
	OUT_OF_DST				=0,
	IN_DST_BUT_NOT_SPEC,
	IN_DST_AND_SPEC,
}eTimeTypeOfDst;

typedef struct
{
	uint8 HighYear;
	uint8 LowYear;
	uint8 Month;
	uint8 Day;
}tClock_YMD;

typedef union
{
    struct
    {
        uint16 YearNoSpecify        :1;
        uint16 Month_DST_End        :1;
        uint16 Month_DST_Start      :1;
        uint16 MonthNoSpecify       :1;
        uint16 DayLastButOne        :1;
        uint16 DayLast              :1;
        uint16 DayNoSpecify         :1;
        uint16 DayWeekNospecify     :1;
        uint16 HourNoSpecify        :1;
        uint16 MinuteNoSpecify      :1;
        uint16 SecondNoSpecify      :1;
        uint16 Reserve2             :5;

    }Bits;
    uint16 Value;
}tWildcardCode;

static const uint8 cMonthDays[] = {29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const tClockPara cClockPara =
{
	420,
    {0},
    {0xff,0xff,3,0xfe,7,2,0,0,0,0,0,0},
    {0xff,0xff,10,0xfe,7,3,0,0,0,0,0,0},
    60,
    1,
	1,
	1
};


//static tClockPara sRTC_Para = {0};
//static tDateTime sDaylightSavingStr, sDaylightSavingEnd;
//static uint8 sDaylightSavingEnable;
static tClockStatus sClockStatus = {0};
__no_init uint16 sClockBroadcastDays;

#define EE_CLOCK_PARA_ADDR				EE_STRUCT_ADDR(EE_ClockPara)

/*Private functions-------------------------------------------------*/

/*****************************************************************************************
*   CalcCalendarToDays
入口参数: 当前日期
出口参数: 无
返回参数：true-闰年; false-非闰年
函数功能：闰年判断

*****************************************************************************************/
static BOOL_B IsLeapYear(const tDateTime *pTime)
{
    uint16 Year;
	
    Year = ((uint16)(pTime->HighByteYear)<<8) + pTime->LowByteYear;
    if((Year % 4) == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************************
*   DaysToDate
入口参数: pTime 当前电表时钟指针; Days  输入的天数
出口参数: 无
返回参数：无
函数功能：由天数计算date, 基准时间为:BASE_YEAR.BASE_MONTH.BASE_DAY(2000.01.01)

*****************************************************************************************/
static void DaysToDate(tDateTime *pTime, const uint16 Days)
{
    uint16 Num;
    uint16 Counter, Year = BASIC_YEAR;
    uint8 Buz;
    tDateTime Time = *pTime;       //暂存当前时间

    Time.HighByteYear = BASIC_YEAR/256; //2000.01.01
	Time.LowByteYear = BASIC_YEAR%256;
    Time.Month = 1;
    Time.Day = 1;

    Counter = Days;

    while(Counter > 0)
    {
        if(IsLeapYear(&Time))
        {
            Num = 366;
        }
        else
        {
            Num = 365;
        }
        if(Counter >= Num)    //确定年限
        {
            Year += 1;
		 	Time.HighByteYear = (uint8)(Year>>8);
			Time.LowByteYear = (uint8)Year;
            Counter -= Num;
        }
		else
		{
            Buz = GetMonthDays(&Time);
            if(Counter >= Buz)
			{
				Time.Month += 1;
				Counter -= Buz;
                continue;
			}
            else
            {
                Time.Day += Counter;
			    Counter = 0;  //天数减为0，结束while循环
            }
		}
    }
	Time.Week = GetWeekDay(&Time);
    *pTime = Time;
}

/*****************************************************************************************
*   SeconsToTime
入口参数: pTime 当前电表时钟指针; Seconds 输入的秒数
出口参数: 无
返回参数：无
函数功能：由天数计算date, 基准时间为:BASE_YEAR.BASE_MONTH.BASE_DAY

*****************************************************************************************/
static void SeconsToTime(tDateTime *pTime, const uint32 Seconds)
{
    uint32 Counter = Seconds;
    tDateTime Time = *pTime;       //暂存当前时间

	Time.Second = (uint8)(Counter%60);
    Counter /= 60;

    Time.Minute = (uint8)(Counter%60);
    Counter /= 60;

    Time.Hour = (uint8)(Counter%24);

    *pTime = Time;
}

static void Clock_SaveClockStatus(tClockStatus Status)
{
	tEE_ClockPara EE_ClockPara;
	
	NvmBytesRead(EE_CLOCK_PARA_ADDR, &EE_ClockPara, sizeof(EE_ClockPara));
	if(!IsCheckSumRight(EE_ClockPara))
	{
		EE_ClockPara.ClockPara = cClockPara;
	}
	EE_ClockPara.ClockPara.ClockStatus = Status;
	CalcCheckSum(EE_ClockPara);
	NvmBytesWrite(EE_CLOCK_PARA_ADDR, &EE_ClockPara, sizeof(EE_ClockPara));
}


static uint8 ClockSetTime(uint8 Flag, uint8* pBuf, uint16 Len)
{
	tDateTime DateTime;
	
	if(Len != sizeof(tDateTime))
	{
		return DA_OTHER_REASON;
	}
	DateTime = *(tDateTime*)pBuf;
	if(IsClockValid(&DateTime))
	{
		tDateTime DateTime1,DateTime2;
		int32 Duration;
		tEE_ClockPara EE_ClockPara;

		Clock_GetTime(&DateTime1);
		if(Flag == 0)		//广播校时
		{	
			Duration = GetDuration(&DateTime, &DateTime1);
			if(Duration > 300 || Duration < -300)		//5分钟内可以广播校时
			{
				return DA_OTHER_REASON;
			}
			DateTime2 = DateTime1;
			if(DateTime2.Hour > 22)
			{
				uint32 Seconds;
				
				Seconds = DateTimeToSeconds(&DateTime2) + 86400;	//下一天
				SecondsToDateTime(&DateTime2, Seconds);
			}
			DateTime2.Hour = 0;
			DateTime2.Minute = 0;
			DateTime2.Second = 0;
			Duration = GetDuration(&DateTime1, &DateTime2);
			if((Duration <= 300) && (Duration >= -300))		//零点的正负5分钟内不可广播校时
			{
				return DA_OTHER_REASON;
			}
			NvmBytesRead(EE_CLOCK_PARA_ADDR, &EE_ClockPara, sizeof(EE_ClockPara));
			if(!IsCheckSumRight(EE_ClockPara))
			{
				EE_ClockPara.ClockPara = cClockPara;
			}
			if(EE_ClockPara.ClockPara.BroadcastFlag == 0)
			{
				return DA_OTHER_REASON;
			}
			EE_ClockPara.ClockPara.BroadcastFlag = 0;
			CalcCheckSum(EE_ClockPara);
			NvmBytesWrite(EE_CLOCK_PARA_ADDR, &EE_ClockPara, sizeof(EE_ClockPara));
		}
		else
		{
//			EventInsertLog(EVENT_SYNCHRONIZATION_STR, NULL);
		}
        memset(&RTCResetRecord, 0, sizeof(RTCResetRecord));	//清除RTC 复位测试记录	
		DRV_RTC_Set(&DateTime);
		if(Flag != 0)		
		{
			DRV_RTC_ResetState();
			sClockStatus.Value = 0;
		}
		Clock_SaveClockStatus(sClockStatus);		//save the clock status
		if(Flag != 0)
		{
			BillingResetBillingTime();
			FreezeResetFreezeTime();
		}
	}
	else
	{
		return DA_OTHER_REASON;
	}
	return DA_SUCCESS;
}

static void ClockResetBroadcastFlag(void)
{
	tEE_ClockPara EE_ClockPara;
	
	NvmBytesRead(EE_CLOCK_PARA_ADDR, &EE_ClockPara, sizeof(EE_ClockPara));
	if(!IsCheckSumRight(EE_ClockPara))
	{
		return;
	}
	if(EE_ClockPara.ClockPara.BroadcastFlag == 1)
	{
		return;
	}
	EE_ClockPara.ClockPara.BroadcastFlag = 1;
	CalcCheckSum(EE_ClockPara);
	NvmBytesWrite(EE_CLOCK_PARA_ADDR, &EE_ClockPara, sizeof(EE_ClockPara));
}

/*Export functions-------------------------------------------------*/
void ClockPowerUpInit(void)
{
	ClockBatteryUpInit();
	TaskAdd(Clock_Process, 0, 300, 1);		
}

void ClockBatteryUpInit(void)
{
	tEE_ClockPara EE_ClockPara;

	NvmBytesRead(EE_CLOCK_PARA_ADDR, &EE_ClockPara, sizeof(EE_ClockPara));
	if(!IsCheckSumRight(EE_ClockPara))
	{
		EE_ClockPara.ClockPara = cClockPara;
	}
//	sDaylightSavingStr = EE_ClockPara.ClockPara.DST_Begin;
//	sDaylightSavingEnd = EE_ClockPara.ClockPara.DST_End;
//	sDaylightSavingEnable = EE_ClockPara.ClockPara.DST_Enable;
	sClockStatus = EE_ClockPara.ClockPara.ClockStatus;
	Clock_Process();	//更新时钟与状态字
	Clock_GetTime(&EE_ClockPara.ClockPara.DST_Begin);
	Clock_GetPowerDownTime(&EE_ClockPara.ClockPara.DST_End);
	if(CalcCalendarToDays(&EE_ClockPara.ClockPara.DST_End) != CalcCalendarToDays(&EE_ClockPara.ClockPara.DST_Begin))
	{
		ClockResetBroadcastFlag();
	}
	sClockBroadcastDays = CalcCalendarToDays(&EE_ClockPara.ClockPara.DST_Begin);
}

void Clock_Process(void)
{
	tDateTime DateTime;
	tDateTime TempDateTime;
	uint16 Days;
	
	DRV_RTC_Get(&DateTime);
	if((DateTime.ClockStatus & 0x03) != (sClockStatus.Value & 0x03))
	{
		sClockStatus.Value |= (DateTime.ClockStatus & 0x03);
		Clock_SaveClockStatus(sClockStatus);		//save the clock status
	}
	if(DRV_RTC_GetSecState() != 0)		//秒事件
	{
//		EventSecondProcess();
	}
	Clock_GetTime(&TempDateTime);
	Days = CalcCalendarToDays(&TempDateTime);
	if(sClockBroadcastDays != Days)
	{
		sClockBroadcastDays = Days;
		ClockResetBroadcastFlag();
	}
}

uint8 Clock_GetTime(tDateTime *pDateTime)
{
	DRV_RTC_Get(pDateTime);
	pDateTime->ClockStatus = sClockStatus.Value;
	return true;
}

/*****************************************************************************************
*   IsClockValid
入口参数: pTime--指向时需判断的时间，不能有通配符.
出口参数: 无
返回参数：1--有效, 0--无效
函数功能：判断时间是否有效

*****************************************************************************************/
BOOL_B IsClockValid(const tDateTime *pTime)
{
    tDateTime DateTime = *pTime;
	uint16 Year = ((uint16)(pTime->HighByteYear)<<8) + pTime->LowByteYear;

    if(DateTime.Hour > 23)
        return FALSE;
    if(DateTime.Minute > 59)
        return FALSE;
    if(DateTime.Second > 59)
        return FALSE;

    if((Year < 2012) || (Year > 2099))
        return FALSE;
    if(DateTime.Month > 12 || DateTime.Month == 0)
        return FALSE;

    if((DateTime.Day > GetMonthDays(&DateTime)) || (DateTime.Day == 0))
    {
        return FALSE;
    }
    return TRUE;
}

void Clock_GetPowerDownTime(tDateTime *pDateTime)
{
	DRV_RTCPowerDownTimeGet(pDateTime);
}

/*****************************************************************************************
*   Duration
入口参数: pClock1,pClock2---两个需要计算间隔的时间指针
出口参数: 无
返回参数：返回相差的时间，pClock1 > pClock2 返回正数，pClock1 < pClock2返回负数
函数功能：计算两个时间的间隔，此函数没有通配，不判断合法性

*****************************************************************************************/
int32 GetDuration(const tDateTime *pClock1, const tDateTime *pClock2)
{
    int32 Temp;

    Temp = DateTimeToSeconds(pClock1);
    Temp -= DateTimeToSeconds(pClock2);
    return Temp;
}


/*****************************************************************************************
*   GetWeekDay
入口参数: pDate需要查找的日期
出口参数: 无
返回参数：返回day of week
函数功能：由日期转化为相应的day of week

*****************************************************************************************/
uint8 GetWeekDay(const tDateTime *pDate)
{
    uint16 week = CalcCalendarToDays(pDate);
	
    return ((week + 5) % 7 + 1);
}


/*****************************************************************************************
*   GetMonthDays
入口参数: 日期
出口参数: 无
返回参数：传入日期的月的天数,0为月份出错
函数功能：计算月的天数

*****************************************************************************************/
uint8 GetMonthDays(const tDateTime *pTime)
{
    uint8 Month = pTime->Month;

    if((Month == 0) || (Month >12))
    {
        return 0;
    }
    if(IsLeapYear(pTime))
    {
        if(Month == 2)
        {
            Month = 0;
        }
    }

    return cMonthDays[Month];
}

/*****************************************************************************************
*   SecondsToDateTime
入口参数: 无
出口参数: 无
返回参数：无
函数功能：把相应的秒数转成日期时间(基准为2000.01.01)

*****************************************************************************************/
void SecondsToDateTime(tDateTime *pTime, const uint32 second)
{
    uint16 DayCounter;
	uint32 SecCounter;

    DayCounter = second/((uint32)60*60*24);        //总共天数
	SecCounter = second%((uint32)60*60*24);        //余数(不足一天的时间秒数)

    DaysToDate(pTime, DayCounter);      //天数转换成日期
    SeconsToTime(pTime, SecCounter);    //秒数转成时间
}

/*****************************************************************************************
*   DateTimeToSeconds
入口参数: pTime--需要转成秒的日期时间,因为转换的基准是2000.01.01;00:00:00,所以入口时间必须
    大于这个时间
出口参数: 无
返回参数：无
函数功能：把日期时间减去基准时间后转换成秒数

*****************************************************************************************/
uint32 DateTimeToSeconds(const tDateTime *pTime)
{
    uint16 Days;
    uint32 Second;

    Days = CalcCalendarToDays(pTime);
    if(Days == 0xffff)
    {
        return 0;
    }
    Second = (uint32)Days*86400 + (uint32)(pTime->Hour)*3600
        + (uint32)(pTime->Minute)*60 +(pTime->Second);

    return Second;
}

/*****************************************************************************************
*   CalcCalendarToDays
入口参数: 需要转换成天数据日期时间，必须大于基准时间
出口参数: 无
返回参数：返回days, 出错则返回0xffff
函数功能：根据公历日历，转换为相应的天数(基准为2000.01.01)

*****************************************************************************************/
uint16 CalcCalendarToDays(const tDateTime *pTime)
{
    uint8 i;
    uint16 days = 0;
    uint16 year = ((uint16)(pTime->HighByteYear)<<8) + pTime->LowByteYear;

    if(year <= BASIC_YEAR)  //为2000年
    {
        days = 0;
    }
	year -= BASIC_YEAR;
    if((year) % 4)  //年份不整除
    {
        days = (year)/4+1+365*(year);
    }
    else
    {
        days = (year)/4+365*(year);
    }

    if(IsLeapYear(pTime))
    {
        if(pTime->Month> 2)
        {
            days += 1;  //闰年超过2月份，多加1天
        }
    }

    for(i = 1; i < (pTime->Month); i++)
    {
        days += cMonthDays[i];
    }

    days += (pTime->Day) - 1;

    return days;
}

/*****************************************************************************************
*   Action:    时钟相关数据读取,主要为类8数据
*   Input:     DataId--数据ID；pBuf--数据存放的指针
*   Output:    读取数据的长度
*****************************************************************************************/
uint16 Clock_ParaGet(uint32 DataId, uint8 *pBuf)
{
	uint16 Result = 0x8000+DA_READ_WRITE_DENIED, Item = (uint16)(DataId>>8);
	uint8 Attr = (uint8)DataId, Class = (uint8)(DataId>>24);
	tDateTime DateTime;

	
	if((Class != 8) && (Class != 1))
	{
		return Result;
	}
	Clock_GetTime(&DateTime);
	switch(Item)
	{
		case 0x0000:
			switch(Attr)
			{
				case 2:		//time
				{
					Result = sizeof(tDateTime);
					*(tDateTime*)pBuf = DateTime;
					break;
				}
				default:
					break;
			}
			break;
		case 0x7200:	//time
			if(Attr == 2)
			{
				((tDlmsTime*)pBuf)->Hour = DateTime.Hour;
				((tDlmsTime*)pBuf)->Minute = DateTime.Minute;
				((tDlmsTime*)pBuf)->Second = DateTime.Second;
				((tDlmsTime*)pBuf)->Hundredths = DateTime.HundredthSecond;
				Result = sizeof(tDlmsTime);
			}
			break;
		case 0x7201:	//date
			if(Attr == 2)
			{
				((tDlmsDate*)pBuf)->HighByteYear = DateTime.HighByteYear;
				((tDlmsDate*)pBuf)->LowByteYear = DateTime.LowByteYear;
				((tDlmsDate*)pBuf)->Month = DateTime.Month;
				((tDlmsDate*)pBuf)->DayOfMonth = DateTime.Day;
				((tDlmsDate*)pBuf)->DayOfWeek = DateTime.Week;
				Result = sizeof(tDlmsDate);
			}
			break;
		default:
			break;
	}
	return Result;
}

/*****************************************************************************************
*   Action:    冻结相关数据设置
*   Input:     DataId--数据ID；pBuf--数据存放的指针,Len--所设数据的长度
*   Output:    
*****************************************************************************************/
uint8 Clock_ParaSet(uint32 DataId, uint8 *pBuf, uint16 Len)
{
	uint8 Result = DA_READ_WRITE_DENIED;
	uint8 Attr = (uint8)DataId;
	
	if((uint8)(DataId>>24) != 8)
	{
		return Result;
	}
	if((uint16)(DataId>>8) == 0x0001)	//广播校时
	{
		if(Attr == 0x0002)
		{
			return ClockSetTime(0, pBuf, Len);	
		}
	}
	else
	{
		switch(Attr)
		{
			case 2:	//设置时间
				Result = ClockSetTime(1, pBuf, Len);	
				break;
			default:
				break;
		}
	}
	return Result;
}


//---------------------------

eTimeCmpResult ClockCmp(const tDateTime *pTime1, const tDateTime *pTime2)
{
	int32 Temp;
	
	Temp = GetDuration(pTime1, pTime2);
	if(Temp > 0)
	{
		return TIME1_LARGE_TIME2;
	}
	else if(Temp < 0)
	{
		return TIME1_LESS_TIME2;
	}
	return TIME1_EQUAL_TIME2;
}

uint8 ClockStateGet(void)
{
   tDateTime  date_time;
    uint8 ClockState;
    Clock_GetTime(&date_time);
    if(((date_time.ClockStatus)&0x03)!=0x00)
    {
        ClockState=1;
    }
    else 
    {
        ClockState=0;
    }
    return ClockState;
}


