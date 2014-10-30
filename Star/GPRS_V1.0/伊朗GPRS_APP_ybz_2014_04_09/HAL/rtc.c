#include <string.h>
#include <stdio.h>
#include "plat_include/func.h"
#include "hal_include/rtc.h"
#include "hal_include/hal_config.h"
#include "plat_include/debug.h"


static utime_t utime;

//在秒中断里调用 
inline void Tick(void)
{
	utime ++;
}
/**
*@brief 更新当前时间,每秒调用一次
*@type: 0 立刻更新 非0：1分钟更新一次
*/
#ifdef USE_STM32_RTC
void rtc_update(unsigned int type)
{
#if 0     //表达式为0 不编译以下
	if(type!=0)
	{
		//utime ++;
		if((utime%60)!=43)	//每当43秒的时候更新当前时间
			return;
	}

	utime = stm32rtc_read();
#endif
}
/**
* @brief 读取外部时钟
* @param 时钟变量指针
* @return 0成功, 否则失败
*/
int ExtClockRead(struct sysclock *clock)
{
	//utime_to_sysclock(stm32rtc_read(),clock);
	utime_to_sysclock(utime,clock);
	return 0;
}
#else	 
static int hms_check(const unsigned char *str);
static int ymd_check(const unsigned char *str);
static int sysclock_check(const struct sysclock *pclk);
static int hms_check(const unsigned char *str)
{
	if(isbcd(str,3)==1)
	{	
		if((*str<0x60)&&(*(str+1)<0x60)&&(*(str+2)<0x60))
			return 1;
	}
	return 0;

}

static int ymd_check(const unsigned char *str)
{
	if(isbcd(str,3)==1)
	{
		if((*str<0x32)&&(*(str+1)<0x13))
			return 1;
	}
	return 0;
}

static int sysclock_check(const struct sysclock *pclk)
{
  	if(pclk->month > 12)	return 0;
	if(pclk->day > 31)		return 0;
	if(pclk->hour > 23)		return 0;
	if(pclk->minute > 59)	return 0;
	if(pclk->second > 59)	return 0;
	return 1;
}

/**
* @brief 读取外部时钟
* @param 时钟变量指针
* @return 0成功, 否则失败
*/
int ExtClockRead(struct sysclock *clock)
{
	unsigned char tm1[7];
	unsigned char tm2[7];
	unsigned int i;
	for(i=0;i<3;i++)
	{
		read_ymdwhms(tm1);
		read_ymdwhms(tm2);
		if((0==memcmp(tm1,tm2,sizeof tm1))&&
			(hms_check(tm1)==1)&&(ymd_check(tm1+4)==1))
		{
			break;
		}
	}
	if(i==3)
	{
		PrintLog(LOGTYPE_DEBUG,"RX8025 read error!!!!\r\n");
		return -1;
	}		
	clock->second=bcdtohex(tm2[0]);	//second
	clock->minute=bcdtohex(tm2[1]);	//minus
	clock->hour=bcdtohex(tm2[2]);	//hour
	clock->week=bcdtohex(tm2[3]);	//week
	clock->day=bcdtohex(tm2[4]);	//day
	clock->month=bcdtohex(tm2[5]);	//month
	clock->year=bcdtohex(tm2[6]);	//year	
	return 0;
}
void rtc_update(unsigned int type)
{
	unsigned char tm1[7];
	unsigned char tm2[7];
	struct sysclock ck;
	unsigned int i;

	if(type!=0){
		//utime ++;
		if((utime%60)!=43)	//每当43秒的时候更新当前时间
			return;
	}
    //  PrintLog(LOGTYPE_DEBUG,"update time...\r\n");
	for(i=0;i<3;i++)
	{
		read_ymdwhms(tm1);
		read_ymdwhms(tm2);
		if((0==memcmp(tm1,tm2,sizeof(tm1)-1))&&
			(hms_check(tm1+4)==1)&&(ymd_check(tm1)==1))
		{
			break;
		}
		else
		{
   		if(0!=memcmp(tm1,tm2,sizeof(tm1)-1))
			PrintLog(LOGTYPE_DEBUG,"read two time error\r\n");
		if(hms_check(tm1+4)==0)
			PrintLog(LOGTYPE_DEBUG,"hms_check error\r\n");

		if(ymd_check(tm1)==0)
			PrintLog(LOGTYPE_DEBUG,"ymd_check error\r\n");
		}
	}
	if(i==3){
		PrintLog(LOGTYPE_DEBUG,"RX8025 read error!!!!\r\n");
		return;
	}
	
	ck.year = bcdtohex(tm2[0]);	//year
	ck.month =bcdtohex(tm2[1]);	//month
	ck.day =bcdtohex(tm2[2]);	//day
	ck.week =bcdtohex(tm2[3]);	//week
	ck.hour =bcdtohex(tm2[4]);	//hour
	ck.minute =bcdtohex(tm2[5]);	// minus
	ck.second =bcdtohex(tm2[6]);	//second
	
	utime = sysclock_to_utime(&ck);

//	PrintLog(LOGTYPE_DEBUG,"rtc is time_t %d\n%2x年%2x月%2x日 星期%2x %2x:%2x:%2x\r\n",
//		utime,tm2[0],tm2[1],tm2[2],tm2[3],tm2[4],tm2[5],tm2[6]);

}
#endif


/**
* @brief 读取系统时间
* @param pclock 读取的时间值变量指针
*/


void sysclock_read(struct sysclock *pclock)
{
	utime_to_sysclock(utime,pclock);
}

/**
* @brief 读取utime
*
*/
utime_t utime_read(void)
{
	utime_t tmp = utime;
	return tmp;
}

/**
* @brief 设置系统时间
* @param pclock 设置的时间值变量指针
* @return 成功时返回0, 否则返回非零值
*/
#ifdef USE_STM32_RTC
int sysclock_set(const struct sysclock *pclock)
{
	utime=sysclock_to_utime(pclock);
//	stm32rtc_set(utime);
	return 	0;
}
#else
/**
* @brief 设置外部时钟
* @param 时钟变量指针
* @return 0成功, 否则失败
*/

int sysclock_set(const struct sysclock *pclock)
{	
	unsigned char tm[7];
	struct sysclock clk;
	if(sysclock_check(pclock)==1)
	{	
	  	utime =  sysclock_to_utime(pclock);
		utime_to_sysclock(utime,&clk);
		tm[6] = hextobcd(clk.second);
		tm[5] = hextobcd(clk.minute);
		tm[4] = hextobcd(clk.hour);
		tm[3] = hextobcd(clk.week);
		tm[2] = hextobcd(clk.day);
		tm[1] = hextobcd(clk.month);
		tm[0] = hextobcd(clk.year);
		write_ymdwhms(tm);
		return 0;
	}
	return -1;
}
#endif



static const int DaysBeforeMonth[12] = {
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};

/*
* @brief 将系统时间转换为utime_t格式
* @param ptime 系统时间值变量指针
* @return 成功时返回对应的utime_t时间, 否则返回-1
*/
utime_t sysclock_to_utime(const struct sysclock *ptime)
{
	int year, day;
	int cal;
	
	year = (int)(ptime->year)&0xff;
	if(year != 0) day = ((year-1)>>2) + 1;
	else day = 0;
	cal = year *365 + day; //days
	day = cal;

	if(ptime->month > 2 && 0 == (ptime->year&0x03)) day += 1;


	if(ptime->month != 0 && ptime->month < 13) {
		day += DaysBeforeMonth[ptime->month - 1];
	}

	if(0 != ptime->day) day += ptime->day - 1;

	cal = day*1440;

	day = ptime->hour;
	day *= 60;
	cal += day;

	cal += ptime->minute;
	cal *= 60;

	cal += ptime->second;

	return (utime_t)cal;
}

/**
* @brief 将utime_t格式转换为系统时间
* @param utime 需转换utime_t时间变量
* @param ptime 转换后的系统时间值变量指针
*/
void utime_to_sysclock(utime_t utime, struct sysclock *ptime)
{
	unsigned char uc;
	int max;

	ptime->second = utime%60;
	utime /= 60;

	ptime->minute = utime%60;
	utime /= 60;

	ptime->hour = utime%24;
	utime /= 24;

	ptime->week = (unsigned char)((utime+6)%7);  // 2000.1.1 is saturday

	max = utime/1461;  //四年1461天
	ptime->year = (unsigned char)(max<<2);//闰年周期数
	//utime %= 1461;
	utime -= max*1461;
	uc = 0;
	max = 366;
	while(utime >= max) {//最多循环３次，闰年内
		utime -= max;
		max = 365;
		uc += 1;
	}
	ptime->year += uc;

	uc=1;
	max = 31;
	while(utime >= max) {
		uc++;
		utime -= max;

		if(2 == uc) {
			if(0 == (ptime->year&0x03)) max = 29;
			else max = 28;
		}
		else if(uc < 8) {
			if(uc&0x01) max = 31;
			else max = 30;
		}
		else {
			if(uc&0x01) max = 30;
			else max = 31;
		}
	}
	ptime->month = uc;

	ptime->day = utime + 1;
}




