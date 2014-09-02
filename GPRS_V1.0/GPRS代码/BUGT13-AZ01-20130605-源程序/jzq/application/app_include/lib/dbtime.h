/**
* dbtime.h -- ���ݿ�ʱ���ʽ
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-24
* ����޸�ʱ��: 2009-5-24
*/

#ifndef _DB_TIME_H
#define _DB_TIME_H

#include "plat_include/timeal.h"
#include "app_include/sys/store.h"

#define DBTIME_ISONEDAY(t1,t2)	((t1).us[1] == (t2).us[1] && (t1).s.day == (t2).s.day)

#define DBTIME_SYSCLOCK(dbtime, pclock) { \
	(pclock)->year = (dbtime).s.year; \
	(pclock)->month = (dbtime).s.month; \
	(pclock)->day = (dbtime).s.day; \
	(pclock)->hour = (dbtime).s.tick >> 2; \
	(pclock)->minute = ((dbtime).s.tick & 0x03)*15; \
	(pclock)->second = 0; }

#define SYSCLOCK_DBTIME(pclock, dbtime) { \
	(dbtime).s.year = (pclock)->year; \
	(dbtime).s.month = (pclock)->month; \
	(dbtime).s.day = (pclock)->day; \
	(dbtime).s.tick = ((pclock)->hour<<2)+((pclock)->minute/15); }

#define DBTIME_UTIME(dbtime, utime) { \
	sysclock_t __tmp_clock; \
	DBTIME_SYSCLOCK(dbtime, &__tmp_clock); \
	utime = SysClockToUTime(&__tmp_clock); }

#define UTIME_DBTIME(utime, dbtime) { \
	sysclock_t __tmp_clock; \
	UTimeToSysClock(utime, &__tmp_clock); \
	SYSCLOCK_DBTIME(&__tmp_clock, dbtime); }

void DbTimeAddOneDay(dbtime_t *pdbtime);
void DbTimeAddOneMonth(dbtime_t *pdbtime);
void DbTimeAddOneTick(dbtime_t *pdbtime);
void DbTimeDelOneDay(dbtime_t *pdbtime);
void DbTimeDelOneMonth(dbtime_t *pdbtime);

#endif /*_DB_TIME_H*/
