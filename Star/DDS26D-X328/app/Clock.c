/*****************************************************************************************
*  	Function:  ʱ�ӵ�Ԫ��
*   
*    Author:	������
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
��ڲ���: ��ǰ����
���ڲ���: ��
���ز�����true-����; false-������
�������ܣ������ж�

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
��ڲ���: pTime ��ǰ���ʱ��ָ��; Days  ���������
���ڲ���: ��
���ز�������
�������ܣ�����������date, ��׼ʱ��Ϊ:BASE_YEAR.BASE_MONTH.BASE_DAY(2000.01.01)

*****************************************************************************************/
static void DaysToDate(tDateTime *pTime, const uint16 Days)
{
    uint16 Num;
    uint16 Counter, Year = BASIC_YEAR;
    uint8 Buz;
    tDateTime Time = *pTime;       //�ݴ浱ǰʱ��

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
        if(Counter >= Num)    //ȷ������
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
			    Counter = 0;  //������Ϊ0������whileѭ��
            }
		}
    }
	Time.Week = GetWeekDay(&Time);
    *pTime = Time;
}

/*****************************************************************************************
*   SeconsToTime
��ڲ���: pTime ��ǰ���ʱ��ָ��; Seconds ���������
���ڲ���: ��
���ز�������
�������ܣ�����������date, ��׼ʱ��Ϊ:BASE_YEAR.BASE_MONTH.BASE_DAY

*****************************************************************************************/
static void SeconsToTime(tDateTime *pTime, const uint32 Seconds)
{
    uint32 Counter = Seconds;
    tDateTime Time = *pTime;       //�ݴ浱ǰʱ��

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
		if(Flag == 0)		//�㲥Уʱ
		{	
			Duration = GetDuration(&DateTime, &DateTime1);
			if(Duration > 300 || Duration < -300)		//5�����ڿ��Թ㲥Уʱ
			{
				return DA_OTHER_REASON;
			}
			DateTime2 = DateTime1;
			if(DateTime2.Hour > 22)
			{
				uint32 Seconds;
				
				Seconds = DateTimeToSeconds(&DateTime2) + 86400;	//��һ��
				SecondsToDateTime(&DateTime2, Seconds);
			}
			DateTime2.Hour = 0;
			DateTime2.Minute = 0;
			DateTime2.Second = 0;
			Duration = GetDuration(&DateTime1, &DateTime2);
			if((Duration <= 300) && (Duration >= -300))		//��������5�����ڲ��ɹ㲥Уʱ
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
        memset(&RTCResetRecord, 0, sizeof(RTCResetRecord));	//���RTC ��λ���Լ�¼	
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
	Clock_Process();	//����ʱ����״̬��
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
	if(DRV_RTC_GetSecState() != 0)		//���¼�
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
��ڲ���: pTime--ָ��ʱ���жϵ�ʱ�䣬������ͨ���.
���ڲ���: ��
���ز�����1--��Ч, 0--��Ч
�������ܣ��ж�ʱ���Ƿ���Ч

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
��ڲ���: pClock1,pClock2---������Ҫ��������ʱ��ָ��
���ڲ���: ��
���ز�������������ʱ�䣬pClock1 > pClock2 ����������pClock1 < pClock2���ظ���
�������ܣ���������ʱ��ļ�����˺���û��ͨ�䣬���жϺϷ���

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
��ڲ���: pDate��Ҫ���ҵ�����
���ڲ���: ��
���ز���������day of week
�������ܣ�������ת��Ϊ��Ӧ��day of week

*****************************************************************************************/
uint8 GetWeekDay(const tDateTime *pDate)
{
    uint16 week = CalcCalendarToDays(pDate);
	
    return ((week + 5) % 7 + 1);
}


/*****************************************************************************************
*   GetMonthDays
��ڲ���: ����
���ڲ���: ��
���ز������������ڵ��µ�����,0Ϊ�·ݳ���
�������ܣ������µ�����

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
��ڲ���: ��
���ڲ���: ��
���ز�������
�������ܣ�����Ӧ������ת������ʱ��(��׼Ϊ2000.01.01)

*****************************************************************************************/
void SecondsToDateTime(tDateTime *pTime, const uint32 second)
{
    uint16 DayCounter;
	uint32 SecCounter;

    DayCounter = second/((uint32)60*60*24);        //�ܹ�����
	SecCounter = second%((uint32)60*60*24);        //����(����һ���ʱ������)

    DaysToDate(pTime, DayCounter);      //����ת��������
    SeconsToTime(pTime, SecCounter);    //����ת��ʱ��
}

/*****************************************************************************************
*   DateTimeToSeconds
��ڲ���: pTime--��Ҫת���������ʱ��,��Ϊת���Ļ�׼��2000.01.01;00:00:00,�������ʱ�����
    �������ʱ��
���ڲ���: ��
���ز�������
�������ܣ�������ʱ���ȥ��׼ʱ���ת��������

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
��ڲ���: ��Ҫת��������������ʱ�䣬������ڻ�׼ʱ��
���ڲ���: ��
���ز���������days, �����򷵻�0xffff
�������ܣ����ݹ���������ת��Ϊ��Ӧ������(��׼Ϊ2000.01.01)

*****************************************************************************************/
uint16 CalcCalendarToDays(const tDateTime *pTime)
{
    uint8 i;
    uint16 days = 0;
    uint16 year = ((uint16)(pTime->HighByteYear)<<8) + pTime->LowByteYear;

    if(year <= BASIC_YEAR)  //Ϊ2000��
    {
        days = 0;
    }
	year -= BASIC_YEAR;
    if((year) % 4)  //��ݲ�����
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
            days += 1;  //���곬��2�·ݣ����1��
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
*   Action:    ʱ��������ݶ�ȡ,��ҪΪ��8����
*   Input:     DataId--����ID��pBuf--���ݴ�ŵ�ָ��
*   Output:    ��ȡ���ݵĳ���
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
*   Action:    ���������������
*   Input:     DataId--����ID��pBuf--���ݴ�ŵ�ָ��,Len--�������ݵĳ���
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
	if((uint16)(DataId>>8) == 0x0001)	//�㲥Уʱ
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
			case 2:	//����ʱ��
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


