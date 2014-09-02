/**
* keepalive.c -- 保持与服务器的连接
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-19
* 最后修改时间: 2009-5-19
*/

//#include <stdio.h>
#include <string.h>

#include "plat_include/debug.h"
#include "plat_include/timeal.h"
#include "app_include/param/term.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/svrcomm.h"
#include "app_include/uplink/keepalive.h"

/**
* @brief 获取心跳周期
* @return 心跳周期(100ms)
*/
static int GetCycKeepAlive(void)
{
	int i;

	i = (int)(ParaTerm.tcom.cycka)&0xff;
	i *= 600;

	return(i);
}

#define TIMEOUT_KEEPALIVE    600    // 1 minute
#define TIMEOUT_GNOWAIT    600    // 1 minute
typedef struct {
	int cnt;
	int cnt_max;
	int cnt_getcyc;
	int cnt_retry;
} keepalive_stat_t;
static keepalive_stat_t keepalive_stat;

/**
* @brief 清除状态
*/
void ClearKeepAlive(void)
{
	int cyc = GetCycKeepAlive();

	keepalive_stat.cnt = 0;
	keepalive_stat.cnt_retry = 0;
	keepalive_stat.cnt_getcyc = 0;
	keepalive_stat.cnt_max = cyc;
}

/**
* @brief 检测是否在可上线时段
* @return 可上线返回1, 不可上线返回0
*/
int KeepAliveInPeriod(void)
{
	if(ParaTerm.uplink.clientmode != 2) 
		return 1;
	else
		return 0;
}

/**
* @brief 检测是否在可上线时段
* @return 可上线返回1, 不可上线返回0
*/
int CheckinActivePeriod()
{
		sysclock_t clock;
		unsigned int mask;


		SysClockReadCurrent(&clock);
		mask = (unsigned int)1<<(clock.hour);
		if(mask&ParaTerm.uplink.onlineflag) return 1;
		else return 0;	
}
/**
* @brief 保持连接处理
* @return 返回0-需要发送链路测试或者登陆, 1-不要测试链路
*/
int KeepAliveProc(void)
{
	int cnt_maxretry = (int)ParaTerm.uplink.countdail & 0xff;

	if(cnt_maxretry == 0) cnt_maxretry = 3;

	if((LINESTAT_OFF == SvrCommLineState) && (keepalive_stat.cnt_max > 1800)
		&& (keepalive_stat.cnt_retry < cnt_maxretry))
	{
		int cnt_timedail = (int)ParaTerm.uplink.timedail & 0xffff;

		cnt_timedail *= 10;
		if(cnt_timedail == 0) cnt_timedail = 600;

		keepalive_stat.cnt++;
		if((keepalive_stat.cnt*10) > cnt_timedail) {
			//DebugPrint(LOGTYPE_DEBUG, "****cnt_timedail=%d, retry=%d, maxtry=%d, cnt_max=%d\n", 
				//cnt_timedail, keepalive_stat.cnt_retry, cnt_maxretry, keepalive_stat.cnt_max);
			keepalive_stat.cnt_retry++;
			keepalive_stat.cnt = 0;
			keepalive_stat.cnt_getcyc = 0;
			keepalive_stat.cnt_max = GetCycKeepAlive();
			return 0;
		}
	}
	else if(keepalive_stat.cnt_max >= 100) {
		keepalive_stat.cnt++;
		DebugPrint(LOGTYPE_DEBUG, "####cnt_max=%d cnt(%d)\r\n", keepalive_stat.cnt_max,keepalive_stat.cnt);
		if((keepalive_stat.cnt*10) > keepalive_stat.cnt_max) {
			keepalive_stat.cnt = 0;
			keepalive_stat.cnt_getcyc = 0;
			keepalive_stat.cnt_max = GetCycKeepAlive();
			return 0;
		}
	}
	else {
		//DebugPrint(LOGTYPE_DEBUG,"*****cnt_max=%d\n", keepalive_stat.cnt_max);
		keepalive_stat.cnt_getcyc++;
		if((keepalive_stat.cnt_getcyc*10) > TIMEOUT_KEEPALIVE) {
			keepalive_stat.cnt_getcyc = 0;
			keepalive_stat.cnt_max = GetCycKeepAlive();
			return 0;
		}
	}

	return 1;
}

/**
* @brief 刷新链路处理
* @return 成功0, 否则失败
*/
int RefreshKeepAlive(void)
{
	if((LINESTAT_ON == SvrCommLineState) && 
		(keepalive_stat.cnt >= TIMEOUT_GNOWAIT) &&
		SvrCommHaveTask())
	{
		keepalive_stat.cnt = 0;
		keepalive_stat.cnt_getcyc = 0;
		keepalive_stat.cnt_max = GetCycKeepAlive();
		return 0;
	}

	return 1;
}

/**
* @brief 设置标志
* @param flag 标志
*/
void SetKeepAlive(unsigned char flag)
{
	switch(flag) {
	case KEEPALIVE_FLAG_LOGONFAIL:
		//keepalive_stat.cnt_retry++;
		break;

	case KEEPALIVE_FLAG_LOGONOK:
		keepalive_stat.cnt_retry = 0;
		break;

	default: break;
	}
}

