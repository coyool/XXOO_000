/**
* time.c -- 时钟操作接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2008-5-16
* 最后修改时间: 2009-4-24
*/


#include <stdio.h>

#include "hal_include/hal_config.h"
#include "plat_include/debug.h"
#include "hal_include/rtc.h"
#include "plat_include/timeal.h"

static struct sysclock ClockSysStart;
/**
* @brief 读取系统时间
* @param pclock 读取的时间值变量指针
* @return 成功时返回0, 否则返回非零值
*/
int SysClockRead(struct sysclock *pclock)
{

	sysclock_read(pclock);
	return 0;
}

/**
* @brief 设置系统时间
* @param pclock 设置的时间值变量指针
* @return 成功时返回0, 否则返回非零值
*/
int SysClockSet(const struct sysclock *pclock)
{

	sysclock_set(pclock);
	return 0;
}

/*
* @brief 将系统时间转换为utime_t格式
* @param ptime 系统时间值变量指针
* @return 成功时返回对应的utime_t时间, 否则返回-1
*/
utime_t SysClockToUTime(const struct sysclock *ptime)
{
	utime_t cal;
	
	cal = sysclock_to_utime(ptime);
	return cal;
}

/**
* @brief 将utime_t格式转换为系统时间
* @param utime 需转换utime_t时间变量
* @param ptime 转换后的系统时间值变量指针
*/
void UTimeToSysClock(utime_t utime, struct sysclock *ptime)
{

	utime_to_sysclock(utime,ptime);
}

/**
* @brief 比较2个系统时间的差异
* @param ptime1 第一个系统时间变量指针
* @param ptime2 第二个系统时间变量指针
* @return 
*   2个时间之间的差异, 以秒为单位
*   <0 表示ptime1早于ptime2
*   =0 表示2个时间相同
*   >0 表示ptime1晚于ptime2
*/
int SysClockDifference(const struct sysclock *ptime1, const struct sysclock *ptime2)
{
	utime_t cal1, cal2;
	int diff;

	cal1 = SysClockToUTime(ptime1);
	cal2 = SysClockToUTime(ptime2);

	diff = (int)cal1 - (int)cal2;

	return diff;
}

/**
* @brief 在原有时间上增加一段时间
* @param time 原始时间变量
* @param mod 增加的时间数值
* @param dev 增加的时间单位
*     UTIMEDEV_MINUTE -- 分
*     UTIMEDEV_HOUR -- 小时
*     UTIMEDEV_DAY -- 天
*     UTIMEDEV_MONTH -- 月
* @return 增加后的时间值
*/
utime_t UTimeAdd(utime_t time, int mod, int dev)
{
	utime_t rtn = time;
	int addup;

	if(mod < UTIMEDEV_MONTH) {
		if(UTIMEDEV_MINUTE== mod) addup = 60;  //minute
		else if(UTIMEDEV_HOUR == mod) addup = 3600;   //hour
		else addup = 86400;   //day

		addup *= dev;
	}
	else { //month
		sysclock_t clk;
		int i;

		addup = 0;
		UTimeToSysClock(time, &clk);

		for(i=0; i<dev;i++) {
			if(2 == clk.month) {
				if(0 == (clk.year&0x03)) addup += 29*1440*60;
				else addup += 28*1440*60;
			}
			else if(clk.month < 8) {
				if(clk.month&0x01) addup += 31*1440*60;
				else addup += 30*1440*60;
			}
			else {
				if(clk.month&0x01) addup += 30*1440*60;
				else addup += 31*1440*60;
			}

			clk.month++;
			if(clk.month > 12) {
				clk.month = 1;
				clk.year++;
			}
		}
	}

	rtn += addup;
	return(rtn);
}

/**
* @brief 得到当前系统时间
*   这个函数返回定时器任务定时更新的当前时间缓存值, 因此要远快于SysClockRead()
* @param pclock 储存时间的变量指针
*/
void SysClockReadCurrent(struct sysclock *pclock)
{

	SysClockRead(pclock);
}

/**
* @brief 得到当前系统时间(utime_t格式)
*   这个函数返回定时器任务定时更新的当前时间缓存值, 因此要远快于SysClockRead()
* @return 当前系统时间(utime_t格式)
*/
utime_t UTimeReadCurrent(void)
{

	return utime_read();
}

static char TimeFormatBuffer[64];
///static struct timeval TimeValStart;

/**
* @brief 将utime_t时间转换为ascii字符串
* @param time 输入的时间
* @return 时间字符串指针
*/
const char *UTimeFormat(utime_t time)
{
	sysclock_t clock;

	UTimeToSysClock(time, &clock);

	sprintf(TimeFormatBuffer, "20%02d-%d-%d %d:%d:%d", clock.year, clock.month, clock.day,
			clock.hour, clock.minute, clock.second);

	return TimeFormatBuffer;
}

/**
* @brief 将sysclock_t时间转换为ascii字符串
* @param clock 输入的时间
* @return 时间字符串指针
*/
const char *SysClockFormat(const struct sysclock *pclock)
{
	sprintf(TimeFormatBuffer, "20%02d-%d-%d %d:%d:%d", pclock->year, pclock->month, pclock->day,
			pclock->hour, pclock->minute, pclock->second);

	return TimeFormatBuffer;
}

/**
* @brief 开始测量时间
*/
void StartTimeMeasure(void)
{
	/*interface*/
	return ;
}

/**
* @brief 停止测量时间
* @return 从开始测量到停止经过的毫秒数
*/
int StopTimeMeasure(void)
{
	/*interface*/
	return 0;
}

/**
* @brief 停止测量时间(格式化返回)
* @return 从开始测量到停止经过的毫秒数(字符串形式)
*/
const char *StopTimeMeasureFormat(void)
{
	/*interface*/
	return 0;
}

/**
* @brief 获得系统启动时间
* @param clock 时钟变量指针
*/
void GetClockSysStart(sysclock_t *clock)
{
	clock->year = ClockSysStart.year;
	clock->month = ClockSysStart.month;
	clock->day = ClockSysStart.day;
	clock->hour = ClockSysStart.hour;
	clock->minute = ClockSysStart.minute;
	clock->second = ClockSysStart.second;
	clock->week = ClockSysStart.week;
}

/**
* @brief 读取外部时钟
* @param clock 返回时钟变量指针
* @return 返回0表示成功, 否则失败
*/
int ReadExternSysClock(sysclock_t *clock)
{

	SysClockRead(clock);
	return 0;
}

/**
* @brief 时钟模块初始化函数
* @return 返回0表示成功, 否则失败
*/
//DECLARE_INIT_FUNC(SysTimeInit);
int SysTimeInit(void)
{
	ClockSysStart.year = 0;
	ClockSysStart.month = 1;
	ClockSysStart.day = 1;
	ClockSysStart.hour = ClockSysStart.minute = ClockSysStart.second = 0;
	//#ifdef USE_STM32_RTC
		Stm32RtcInit(SysClockToUTime(&ClockSysStart));
	//#endif

	#ifdef USE_DS3231_RTC
		ds3231_Init();
	#endif

	ReadExternSysClock(&ClockSysStart);

 
	return 0;
}

