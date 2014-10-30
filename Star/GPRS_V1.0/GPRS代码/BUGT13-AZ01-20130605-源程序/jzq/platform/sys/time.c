/**
* time.c -- ʱ�Ӳ����ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2008-5-16
* ����޸�ʱ��: 2009-4-24
*/


#include <stdio.h>

#include "hal_include/hal_config.h"
#include "plat_include/debug.h"
#include "hal_include/rtc.h"
#include "plat_include/timeal.h"

static struct sysclock ClockSysStart;
/**
* @brief ��ȡϵͳʱ��
* @param pclock ��ȡ��ʱ��ֵ����ָ��
* @return �ɹ�ʱ����0, ���򷵻ط���ֵ
*/
int SysClockRead(struct sysclock *pclock)
{

	sysclock_read(pclock);
	return 0;
}

/**
* @brief ����ϵͳʱ��
* @param pclock ���õ�ʱ��ֵ����ָ��
* @return �ɹ�ʱ����0, ���򷵻ط���ֵ
*/
int SysClockSet(const struct sysclock *pclock)
{

	sysclock_set(pclock);
	return 0;
}

/*
* @brief ��ϵͳʱ��ת��Ϊutime_t��ʽ
* @param ptime ϵͳʱ��ֵ����ָ��
* @return �ɹ�ʱ���ض�Ӧ��utime_tʱ��, ���򷵻�-1
*/
utime_t SysClockToUTime(const struct sysclock *ptime)
{
	utime_t cal;
	
	cal = sysclock_to_utime(ptime);
	return cal;
}

/**
* @brief ��utime_t��ʽת��Ϊϵͳʱ��
* @param utime ��ת��utime_tʱ�����
* @param ptime ת�����ϵͳʱ��ֵ����ָ��
*/
void UTimeToSysClock(utime_t utime, struct sysclock *ptime)
{

	utime_to_sysclock(utime,ptime);
}

/**
* @brief �Ƚ�2��ϵͳʱ��Ĳ���
* @param ptime1 ��һ��ϵͳʱ�����ָ��
* @param ptime2 �ڶ���ϵͳʱ�����ָ��
* @return 
*   2��ʱ��֮��Ĳ���, ����Ϊ��λ
*   <0 ��ʾptime1����ptime2
*   =0 ��ʾ2��ʱ����ͬ
*   >0 ��ʾptime1����ptime2
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
* @brief ��ԭ��ʱ��������һ��ʱ��
* @param time ԭʼʱ�����
* @param mod ���ӵ�ʱ����ֵ
* @param dev ���ӵ�ʱ�䵥λ
*     UTIMEDEV_MINUTE -- ��
*     UTIMEDEV_HOUR -- Сʱ
*     UTIMEDEV_DAY -- ��
*     UTIMEDEV_MONTH -- ��
* @return ���Ӻ��ʱ��ֵ
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
* @brief �õ���ǰϵͳʱ��
*   ����������ض�ʱ������ʱ���µĵ�ǰʱ�仺��ֵ, ���ҪԶ����SysClockRead()
* @param pclock ����ʱ��ı���ָ��
*/
void SysClockReadCurrent(struct sysclock *pclock)
{

	SysClockRead(pclock);
}

/**
* @brief �õ���ǰϵͳʱ��(utime_t��ʽ)
*   ����������ض�ʱ������ʱ���µĵ�ǰʱ�仺��ֵ, ���ҪԶ����SysClockRead()
* @return ��ǰϵͳʱ��(utime_t��ʽ)
*/
utime_t UTimeReadCurrent(void)
{

	return utime_read();
}

static char TimeFormatBuffer[64];
///static struct timeval TimeValStart;

/**
* @brief ��utime_tʱ��ת��Ϊascii�ַ���
* @param time �����ʱ��
* @return ʱ���ַ���ָ��
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
* @brief ��sysclock_tʱ��ת��Ϊascii�ַ���
* @param clock �����ʱ��
* @return ʱ���ַ���ָ��
*/
const char *SysClockFormat(const struct sysclock *pclock)
{
	sprintf(TimeFormatBuffer, "20%02d-%d-%d %d:%d:%d", pclock->year, pclock->month, pclock->day,
			pclock->hour, pclock->minute, pclock->second);

	return TimeFormatBuffer;
}

/**
* @brief ��ʼ����ʱ��
*/
void StartTimeMeasure(void)
{
	/*interface*/
	return ;
}

/**
* @brief ֹͣ����ʱ��
* @return �ӿ�ʼ������ֹͣ�����ĺ�����
*/
int StopTimeMeasure(void)
{
	/*interface*/
	return 0;
}

/**
* @brief ֹͣ����ʱ��(��ʽ������)
* @return �ӿ�ʼ������ֹͣ�����ĺ�����(�ַ�����ʽ)
*/
const char *StopTimeMeasureFormat(void)
{
	/*interface*/
	return 0;
}

/**
* @brief ���ϵͳ����ʱ��
* @param clock ʱ�ӱ���ָ��
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
* @brief ��ȡ�ⲿʱ��
* @param clock ����ʱ�ӱ���ָ��
* @return ����0��ʾ�ɹ�, ����ʧ��
*/
int ReadExternSysClock(sysclock_t *clock)
{

	SysClockRead(clock);
	return 0;
}

/**
* @brief ʱ��ģ���ʼ������
* @return ����0��ʾ�ɹ�, ����ʧ��
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

