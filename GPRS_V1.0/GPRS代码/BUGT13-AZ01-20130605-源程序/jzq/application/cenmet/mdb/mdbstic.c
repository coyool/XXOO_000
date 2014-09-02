/**
* mdbstic.c -- 统计数据函数
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-14
* 最后修改时间: 2010-1-22
*/

#define DEFINE_MDBSTIC

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFINE_MDBSTIC

#include "plat_include/debug.h"
#include "plat_include/timeal.h"
#include "app_include/param/capconf.h"
#include "app_include/lib/bcd.h"
#include "app_include/lib/align.h"
#include "app_include/param/metp.h"
#include "app_include/param/meter.h"
#include "app_include/param/mix.h"
#include "app_include/monitor/alarm.h"
#include "app_include/cenmet/mdbstic.h"

mdbstic_t MdbStic, MdbSticBakup;

#define STICPWRMIN_DEFAULT    0x7fffffff
extern unsigned short IMetMetpid;

/**
* @brief 将日统计清空(换日)
*/
void MdbSticEmptyDay(void)
{
	sysclock_t clock;

	memset(&MdbStic.term_day, 0, sizeof(termstic_t));

	MdbStic.term_day.sig_max = 0;
	MdbStic.term_day.sig_min = 32;

	SysClockReadCurrent(&clock);
	MdbStic.day = clock.day;
}
/**
* @brief 将月统计清空(换月)
*/
void MdbSticEmptyMonth(void)
{
	sysclock_t clock;

	memset(&MdbStic.term_mon, 0, sizeof(termstic_t));

	SysClockReadCurrent(&clock);
	MdbStic.year = clock.year;
	MdbStic.mon = clock.month;
	MdbStic.upflow_flag = 0;
}

/**
* @brief 更新终端统计数据
*/
#ifdef ALARM_MODULE
static void UpdateMdbSticTerm(void)
{
	MdbStic.term_day.pwr_time++;
	MdbStic.term_mon.pwr_time++;

	//通信流量超限监测
	if(0 == ParaMix.upflow_max || MdbStic.upflow_flag) return;

	if(MdbStic.term_mon.comm_bytes > ParaMix.upflow_max) {
		alarm_t almbuf;

		almbuf.erc = 32;
		almbuf.len = 8;
		DEPART_LONG(MdbStic.term_mon.comm_bytes, almbuf.data);
		DEPART_LONG(ParaMix.upflow_max, almbuf.data+4);

		MakeAlarm(ALMFLAG_ABNOR, &almbuf);
		MdbStic.upflow_flag = 1;
	}
}
#endif
/**
* @brief 更新上行通信流量
* @param bytes 通信流量
*/
void UpdateSticComm(unsigned int bytes)
{
	MdbStic.term_day.comm_bytes += bytes;
	MdbStic.term_mon.comm_bytes += bytes;
}

void UpdateSticSig(unsigned char sig)
{
	sysclock_t clock;

	if(sig > 31 || sig == 0) return;

	if(sig > MdbStic.term_day.sig_max) {
		SysClockReadCurrent(&clock);
		MdbStic.term_day.sig_max = sig;
		MdbStic.term_day.sigmax_time[0] = clock.minute;
		MdbStic.term_day.sigmax_time[1] = clock.hour;
	}

	else if(sig < MdbStic.term_day.sig_min) {
		SysClockReadCurrent(&clock);
		MdbStic.term_day.sig_min = sig;
		MdbStic.term_day.sigmin_time[0] = clock.minute;
		MdbStic.term_day.sigmin_time[1] = clock.hour;
	}
}

/**
* @brief 更新复位次数
*/
void UpdateSticResetCount(void)
{
	MdbStic.term_day.rst_cnt += 1;
	MdbStic.term_mon.rst_cnt += 1;
}

/**
* @brief 更新统计数据库
*/
void UpdateMdbStic(void)
{
#ifdef ALARM_MODULE
	UpdateMdbSticTerm();
#endif
}

/**
* @brief 统计数据库初始化
*/
void MdbSticInit(void)
{
	MdbSticEmptyDay();
	MdbSticEmptyMonth();
}

/**
* @brief 备份终端统计数据(存入冻结数据库中)
* @param flag 备份标志, 0-日数据, 1-月数据
* @param buf 缓存区指针
*/
void BakupSticTerm(unsigned char flag, unsigned char *buf)
{
	termstic_t *psrc;

	if(flag) psrc = &MdbSticBakup.term_mon;
	else psrc = &MdbSticBakup.term_day;

	DEPART_SHORT(psrc->pwr_time, buf); buf += 2;
	DEPART_SHORT(psrc->rst_cnt, buf); buf += 2;
	smallcpy(buf, psrc->sw_cnt, 4); buf += 4;
	smallcpy(buf, &psrc->comm_bytes, 4); buf += 4;
	if(!flag) {
		if(psrc->sig_max < 32) {
			buf[0] = 143 - psrc->sig_max*2;
			buf[1] = psrc->sigmax_time[0];
			buf[2] = psrc->sigmax_time[1];
			HexToBcd(buf+1, 2);
		}
		else {
			buf[0] = 0;
			buf[1] = 0xee;
			buf[2] = 0xee;
		}
		buf += 3;

		if(psrc->sig_min > 0) {
			buf[0] = 143 - psrc->sig_min*2;
			buf[1] = psrc->sigmin_time[0];
			buf[2] = psrc->sigmin_time[1];
			HexToBcd(buf+1, 2);
		}
		else {
			buf[0] = 0;
			buf[1] = 0xee;
			buf[2] = 0xee;
		}
		buf += 3;
	}
}

/**
* @brief 备份统计数据(防止抄表时间太长导致统计时间出错)
*/
void BakupMdbStic(void)
{
	memcpy(&MdbSticBakup, &MdbStic, sizeof(MdbSticBakup));
}

