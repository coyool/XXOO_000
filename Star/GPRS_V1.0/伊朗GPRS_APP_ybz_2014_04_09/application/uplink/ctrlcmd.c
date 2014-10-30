/**
* ctrlcmd.c -- 控制命令
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-29
* 最后修改时间: 2009-5-30
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plat_include/debug.h"
#include "plat_include/timeal.h"
#include "plat_include/timer.h"
#include "app_include/lib/bcd.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
#include "app_include/monitor/runstate.h"
#include "app_include/param/mix.h"
#include "app_include/param/meter.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/svrcomm.h"
#include "app_include/gprs/gprs_dev.h"

/**
* @brief 与主站断开
*/
static int CtrlCmdDisconnectSvr(unsigned short metpid, const unsigned char *buf, int len, int *actlen)
{
	UpdateLineStateInit(0);
	SvrCommLineState = LINESTAT_OFF;
	GprsDisConnect();
	return 0;
}

extern void db_flash_time(utime_t mintime,utime_t bigtime);

/**
* @brief 较时命令处理
* @param metpid 测量点号
* @param buf 缓存区指针
* @param len 缓存区长度
* @param actlen 有效数据长度(由函数返回)
* @return 0成功, 返回-1表示缓存区溢出,返回-2表示失败, 
* @note 以下同类参数和返回值相同, 不做重复注释
*/
static int CtrlCmdTime(unsigned short metpid, const unsigned char *buf, int len, int *actlen)
{
	sysclock_t clock;
	//extclock_t extclock;
	utime_t utimecur, utime;
	int i;

	clock.week = buf[4]>>5;
	if(clock.week == 7 || clock.week == 0) clock.week = 0;
	else clock.week -= 1;

	clock.second = buf[0];
	clock.minute = buf[1];
	clock.hour = buf[2];
	clock.day = buf[3];
	clock.month = buf[4] & 0x1f;
	clock.year = buf[5];
	BcdToHex(&clock.year, 6);

	if(clock.year > 37) return -2;
	
	utimecur = UTimeReadCurrent();
	utime = SysClockToUTime(&clock);

	SysClockSet(&clock);
	SysClockRead(&clock);
#if 0
	extclock.century = 0;
	extclock.year = clock.year;
	extclock.month = clock.month;
	extclock.day = clock.day;
	extclock.hour = clock.hour;
	extclock.minute = clock.minute;
	extclock.second = clock.second;
	extclock.week = clock.week;

	ExtClockWrite(&extclock);
#endif
	if(utimecur > utime) 
	{
		i = utimecur - utime;
		
		//db_flash_time(utime,utimecur);
	}
	else i = utime - utimecur;

	if(i > 300) SysRecalAllRTimer();
	SysRecalAllCTimer();
	return 0;
}

extern int SaveParaMix(char flg);
#ifdef ALARM_MODULE
/**
* @brief 允许主动发送
*/
static int CtrlCmdEnActSnd(unsigned short metpid, const unsigned char *buf, int len, int *actlen)
{
	unsigned char old = ParaMix.bactsend;

	ParaMix.bactsend = 0;

	if(old != ParaMix.bactsend) SaveParaMix(1);//SetSaveParamFlag(SAVEFLAG_MIX);

	return 0;
}

/**
* @brief 禁止主动发送
*/
static int CtrlCmdDisActSnd(unsigned short metpid, const unsigned char *buf, int len, int *actlen)
{
	unsigned char old = ParaMix.bactsend;

	ParaMix.bactsend = 1;

	if(old != ParaMix.bactsend) SaveParaMix(1);//SetSaveParamFlag(SAVEFLAG_MIX);

	return 0;
}

/**
* @brief 激活连接主站
*/
static int CtrlCmdConnectSvr(unsigned short metpid, const unsigned char *buf, int len, int *actlen)
{
	GprsActConnetSvr = 1;
	return 0;
}

/**
* @brief 指定端口重启抄表
*/
extern unsigned char AutoReading;
static int CtrlCmdResStartReadMet(unsigned short metpid, const unsigned char *buf, int len, int *actlen)
{
	unsigned char port;

	port = buf[0];
	if(0 == port || port > 3) return -2;
	/*
	port -= 1;
	pcfg = &ParaCommPort[port];
	old = pcfg->flag;

	pcfg->flag &= ~RDMETFLAG_ENABLE;
	if(old != pcfg->flag) SetSaveParamFlag(SAVEFLAG_MIX);
	*/
        AutoReading =1;
	return 0;
}
#endif

#define CTRLATTR_TERM		0
#define CTRLATTR_SWP		1
#define CTRLATTR_TGRP		2
#define CTRLATTR_METP		3
struct ctrl_cmd {
	unsigned char mask;
	unsigned char attr;
	unsigned short datalen;
	int (*pfunc)(unsigned short metpid, const unsigned char *buf, int len, int *actlen);
};

static const struct ctrl_cmd ctrlcmd_grp4[] = {
#ifdef ALARM_MODULE
	/*F29*/ {0x10, CTRLATTR_TERM, 0, CtrlCmdEnActSnd},
#endif
	/*F31*/ {0x40, CTRLATTR_TERM, 6, CtrlCmdTime},
};

static const struct ctrl_cmd ctrlcmd_grp5[] = {
//	/*F37*/ {0x10, CTRLATTR_TERM, 0, CtrlCmdDisActSnd},
//	/*F38*/ {0x20, CTRLATTR_TERM, 0, CtrlCmdConnectSvr},
	/*F39*/ {0x40, CTRLATTR_TERM, 0, CtrlCmdDisconnectSvr},

};
#ifdef ALARM_MODULE
static const struct ctrl_cmd ctrlcmd_grp7[] = {
	/*F51*/ {0x04, CTRLATTR_TERM, 1, CtrlCmdResStartReadMet},
};
#endif

struct ctrlcmd_list {
	unsigned char grpid;
	const struct ctrl_cmd *fnlist;
	int sizelist;
};
static const struct ctrlcmd_list CtrlCmdList[] = {
	{3, ctrlcmd_grp4, sizeof(ctrlcmd_grp4)/sizeof(ctrlcmd_grp4[0])},
	{4, ctrlcmd_grp5, sizeof(ctrlcmd_grp5)/sizeof(ctrlcmd_grp5[0])},
#ifdef ALARM_MODULE
	{6, ctrlcmd_grp7, sizeof(ctrlcmd_grp7)/sizeof(ctrlcmd_grp7[0])},
#endif
};
#define NUM_LIST	(sizeof(CtrlCmdList)/sizeof(CtrlCmdList[0]))

/**
* @brief 控制命令
* @param buf 输入缓存区指针
* @param len 缓存区长度
* @param pactlen 有效数据长度(由函数返回)
* @return 0成功, 返回-1表示缓存区溢出,返回-2表示失败
*/
int CtrlCommand(unsigned char *buf, int len, int *pactlen)
{
	unsigned short metpid;
	unsigned char pns, pnmask, fns, fnmask, grpid;
	int actlen, berror, idx, rtn, itemlen;
	const struct ctrlcmd_list *plist;
	const struct ctrl_cmd *pfunc;

	AssertLogReturn(len<4, -1, "invalid len(%d)\n", len);

	pns = buf[0];
	if(0 == buf[1]) metpid = 0;
	else metpid = ((unsigned short)(buf[1]-1)<<3) + 1;

	grpid = buf[3];
	fns = buf[2];

	buf += 4;
	actlen = 4;
	len -= 4;
	berror = 0;

	plist = CtrlCmdList;
	for(idx=0; idx<NUM_LIST; idx++,plist++) {
		if(grpid == plist->grpid) break;
	}
	if(idx >= NUM_LIST) return -1;

	if(0 == metpid) pnmask = 0x80;
	else pnmask = 1;
	for(; pnmask!=0; pnmask<<=1,metpid++) {
		if(metpid && (pns&pnmask) == 0) continue;

		for(fnmask=1; 0!=fnmask; fnmask<<=1) {
			if(0 == (fnmask&fns)) continue;

			pfunc = plist->fnlist;
			for(idx=0; idx<plist->sizelist; idx++,pfunc++) {
				if(fnmask & pfunc->mask) break;
			}
			if(idx >= plist->sizelist) return -1;

			switch(pfunc->attr) {
			case CTRLATTR_TERM:
				if(0 != metpid) return -1;
				break;
			case CTRLATTR_METP:
				if(0 == metpid || metpid > MAX_METP) return -1;
				break;
			default: return -1;
			}

			itemlen = pfunc->datalen;
			if(len < itemlen) return -1;

			rtn = (*pfunc->pfunc)(metpid, buf, len, &itemlen);
			if(rtn == -1) return -1;
			if(rtn < 0) berror = 1;

			buf += itemlen;
			actlen += itemlen;
			len -= itemlen;
			if(len < 0) return -1;
		}
	}

	*pactlen = actlen;
	if(berror) return -2;
	else return 0;
}


