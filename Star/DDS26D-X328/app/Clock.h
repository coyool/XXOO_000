#ifndef __CLOCK_H
#define __CLOCK_H

/*Export define----------------------------------------------*/
#define BASIC_YEAR				2000
#define YEAR16_NO_SPECIFY      0XFFFF    //年通配符，两字节
#define YEAR_NO_SPECIFY         0XFF    //年通配符,一字节
#define MONTH_DST_END           0XFD
#define MONTH_DST_START         0XFE
#define MONTH_NO_SPECIFY        0XFF
#define DAY_LAST_BUT_ONE        0XFD    //2nd last day of month
#define DAY_LAST                0XFE    //last day of month
#define DAY_NO_SPECIFY          0XFF
#define DAY_WEEK_NO_SPECIFY     0XFF
#define HOUR_NO_SPECIFY         0XFF
#define MINUTE_NO_SPECIFY       0XFF
#define SECOND_NO_SPECIFY       0XFF
#define DEVIATION_NO_SPECIFY    0X8000


#define MONDAY                  1
#define TUESDAY                 2
#define WEDNESDAY               3
#define THURSDAY                4
#define FRIDAY                  5
#define SATURDAY                6
#define SUNDAY                  7

enum
{
	CLOCK_SET_TYPE = 0,
	DST_TYPE,
	TARIFF_ACTIVE_TYPE,
	TARIFF_TYPE,
	SPECIAL_DAY_TYPE,
	PROFILE_TIME_TYPE,
	
	NORMAL_TYPE = 0xff
};

typedef enum
{
	TIME1_EQUAL_TIME2           =0,
	TIME1_LARGE_TIME2           =1,
	TIME1_LESS_TIME2            =2,
	TIME_INVALID                =0xFF
}eTimeCmpResult;

/*Export typedef---------------------------------------------*/

typedef union
{
    struct
    {
        uint8 InvalidValue          : 1;    //  bit 0 (LSB): invalid a value,
        uint8 DoubtfulValue         : 1;    //  bit 1: doubtful b value,
        uint8 DifBaseC              : 1;    //  bit 2: different clock base c,
        uint8 InvalidStatusD        : 1;    //  bit 3: invalid clock status d,
        uint8 Reserved              : 3;    //  bit 4-6: reserved,
        uint8 DST_Active            : 1;    //  bit 7 (MSB): daylight saving active
    }Bits;
    uint8 Value;
}tClockStatus;

typedef struct
{
	int16 TimeZone;
    tClockStatus ClockStatus;       //此时钟状态只有在数据存储到NVM中去时才用到
    tDateTime DST_Begin;
    tDateTime DST_End;
    int8 DST_Deviation;
    uint8 DST_Enable;
    uint8 ClockBase;                    //1-Interal crystal
    uint8 BroadcastFlag;			//广播校时标志，每天只允许一次
}tClockPara;

typedef struct
{
	tClockPara ClockPara;
    uint16 CheckSum;
}tEE_ClockPara;

/*Export functions----------------------------------------------*/
void ClockPowerUpInit(void);
void ClockBatteryUpInit(void);
void Clock_Process(void);
uint8 Clock_GetTime(tDateTime *pDateTime);
void Clock_GetPowerDownTime(tDateTime *pDateTime);
uint8 GetWeekDay(const tDateTime *pDate);
uint32 DateTimeToSeconds(const tDateTime *pTime);
BOOL_B IsClockValid(const tDateTime *pTime);
void SecondsToDateTime(tDateTime *pTime, const uint32 second);
uint8 GetMonthDays(const tDateTime *pTime);
uint16 Clock_ParaGet(uint32 DataId, uint8 *pBuf);
uint8 Clock_ParaSet(uint32 DataId, uint8 *pBuf, uint16 Len);
int32 GetDuration(const tDateTime *pClock1, const tDateTime *pClock2);
uint16 CalcCalendarToDays(const tDateTime *pTime);
eTimeCmpResult ClockCmp(const tDateTime *pTime1, const tDateTime *pTime2);
uint8 ClockStateGet(void);
#endif

