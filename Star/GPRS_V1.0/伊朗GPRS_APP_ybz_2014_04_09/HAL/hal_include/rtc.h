#ifndef __RTC_H
#define __RTC_H

///系统时间结构
struct sysclock{
	unsigned char year;    // year - 2000
	unsigned char month;	// 1~12
	unsigned char day;	   // 1~31
	unsigned char hour;  // 0~23
	unsigned char minute;  // 0~59
	unsigned char second;  // 0~59
	unsigned char week;  //from sunday, 0~6
};

typedef unsigned int utime_t;
void rtc_update(unsigned int type);
void sysclock_read(struct sysclock *pclock);
utime_t utime_read(void);
int sysclock_set(const struct sysclock *pclock);
utime_t sysclock_to_utime(const struct sysclock *ptime);
void utime_to_sysclock(utime_t utime, struct sysclock *ptime);
int ExtClockRead(struct sysclock *clock);

#endif
