/**
* alarm.c -- 事件告警接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-15
* 最后修改时间: 2009-5-15
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plat_include/debug.h"
#include "plat_include/syslock.h"
#include "plat_include/timeal.h"
#include "plat_include/bin.h"
#include "app_include/lib/bcd.h"
#include "app_include/monitor/runstate.h"
#include "app_include/monitor/alarm.h"
#include "app_include/cenmet/qrycurve.h"
#include "app_include/param/term.h"
#include "app_include/uplink/svrnote.h"
#include "plat_include/flashsave.h"
#include "app_include/uplink/svrcomm.h"

#define ALMNUM_PERFILE		32
#define ALARM_BASEMASK		((unsigned char)~(ALMNUM_PERFILE-1))

//static int LockIdAlarm = -1;

/**
* @brief 获得事件属性
* @param flag 事件标志, 参照ALMFLAG_ABNOR...
* @param erc 事件ID
* @param prank 事件等级变量指针, 1-重要事件, 0-一般事件
* @return 返回1表示事件有效(需保存), 返回0表示无效
*/
/*static int GetAlarmAttribute(unsigned char flag, unsigned char erc, unsigned char *prank)
{
	unsigned char idx, mask;

	*prank = 0;

	if((0 == erc) || (erc > 64))  return 0;
	erc -= 1;

	idx = erc>>3;	//第n字节
	mask = 1<<(erc&0x07);	//第n位
	if(flag&0x0f) RunStateModify.alarm.stat[idx] &= ~mask;
	else RunStateModify.alarm.stat[idx] |= mask;

	if(mask & (ParaTerm.almflag.valid[idx])) {
		if(mask & (ParaTerm.almflag.rank[idx])) *prank = 1;
		return 1;
	}

	return 0;
} */

/**
* @brief 事件查询起始指针合法性判断
* @param idx 事件分类索引(重要事件,一般事件)
* @param pm 查询起始指针
* @return 返回2表示到末尾, 返回1表示非法指针, 返回0表示合法
*/
static int InvalidPm(unsigned char idx, unsigned char pm)
{
	if(RunState.alarm.cur[idx] == pm) return 2;
	else if(RunState.alarm.cur[idx] == RunState.alarm.head[idx]) {
		return 2;//队列为空
	}
	else if(RunState.alarm.cur[idx] > RunState.alarm.head[idx]) {
		if(pm >= RunState.alarm.cur[idx] || pm < RunState.alarm.head[idx]) return 1;
	}
	else {
		if(pm >= RunState.alarm.cur[idx] && pm < RunState.alarm.head[idx]) return 1;
	}

	return 0;
}

/**
* @brief 保存事件
* @param idx 事件分类索引(重要事件,一般事件)
* @param pbuf 储存事件缓存区指针
*/
static void SaveAlarm(unsigned char idx, const alarm_t *pbuf)
{
	unsigned char cur;
	int rt;

	AssertLogReturnVoid(idx>=2, "invalid idx(%d)\n", idx);

	//LockSysLock(LockIdAlarm);

	if(RunState.alarm.cur[idx] == RunState.alarm.head[idx])  //empty
		RunStateModify.alarm.cur[idx] = RunStateModify.alarm.head[idx] = 0;

	cur = RunState.alarm.cur[idx];

	rt = alarm_writeflash_bin(idx,(const void *)pbuf,sizeof(alarm_t));
	AssertLogReturnVoid(rt==-1,"writeflash alarm cur(%d) failed!\n",cur);
	PrintLog(LOGTYPE_DEBUG,"writeflash alarm cur(%d) rt(%d)\n",cur,rt);
	RunStateModify.alarm.cur[idx]++;
	if(RunState.alarm.cur[idx] == RunState.alarm.head[idx]) RunStateModify.alarm.head[idx]++;

	//UnlockSysLock(LockIdAlarm);
}

/**
* @brief 清除事件计数
* @param idx 事件分类索引(重要事件,一般事件)
*/
void ClearAlarm(unsigned char idx)
{
	AssertLogReturnVoid(idx>=2, "invalid idx(%d)\n", idx);

	//LockSysLock(LockIdAlarm);
	RunStateModify.alarm.head[idx] = 0;
	RunStateModify.alarm.cur[idx] = 0;
	RunStateModify.alarm.snd[idx] = 0;
	//UnlockSysLock(LockIdAlarm);
}
/*
static const char *AlarmName[] = {
	"未知!", // 0
	"数据初始化!",
	"参数丢失!",
	"参数变更!",
	"状态量变位!",  // 4
	"遥控跳闸!",
	"功控跳闸!",
	"电控跳闸!", 
	"电表参数变更!", // 8
	"电流回路异常!",
	"电压回路异常!",
	"相序异常!",
	"电表时间超差!", //12
	"电表故障!", 
	"终端停/上电!", 
	"谐波越限!",
	"直流模拟量越限!", // 16
	"电压/电流不平衡!", 
	"电容器投切自锁!", 
	"购电参数变更!",
	"消息认证错误!", // 20
	"终端故障!", 
	"电量差动越限!", 
	"电控告警!",
	"电压越限!", // 24
	"电流越限!", 
	"视在功率越限!",
	"电表示度下降!",
	"电量超差!", // 28
	"电表飞走!", 
	"电表停走!", 
	"485抄表失败!", 
	"通信流量超限!", //32
	"电表状态字变位!", 
	"CT异常!",
	"发现未知电表!",
	"终端停电!",
};
#define ALARMNAME_NUM		(sizeof(AlarmName)/sizeof(AlarmName[0]))
*/
/**
* @brief 生成事件告警
* @param flag 事件标志(参照ALMFLAG_IMPORTANT...)
* @param pbuf 储存事件缓存区指针
*/
void MakeAlarm(unsigned char flag, alarm_t *pbuf)
{
	unsigned char dbid = 0;

	DebugPrint(LOGTYPE_ALARM, "make alarm %d\n", pbuf->erc);

	AssertLogReturnVoid(pbuf->len>MAXLEN_ALMDATA, "invalid len(%d)\n", pbuf->len);

	//bvalid = GetAlarmAttribute(flag, pbuf->erc, &rank);
	//if(!bvalid) return;

	/*if(pbuf->erc < ALARMNAME_NUM) 
		PrintLog(LOGTYPE_ALARM, "!alarm %d, r=%d(%s)\r\n", pbuf->erc, rank, AlarmName[pbuf->erc]);
	else PrintLog(LOGTYPE_ALARM, "!alarm %d, r=%d\r\n", pbuf->erc, rank);*/
	if(14 != pbuf->erc) {  //停电/上电
		sysclock_t clock;

		SysClockReadCurrent(&clock);
		pbuf->min = clock.minute;
		pbuf->hour = clock.hour;
		pbuf->day = clock.day;
		pbuf->mon = clock.month;
		pbuf->year = clock.year;
		HexToBcd(&pbuf->min, 5);
	}

/*	if(flag&ALMFLAG_IMPORTANT) dbid = 0;
	else if(flag&ALMFLAG_NOIMPORTANT) dbid = 1;
	else if(rank) dbid = 0;
	else dbid = 1;*/
	
	SaveAlarm(dbid, pbuf);
	if(0 == dbid) 
	{
		RunStateModify.flag_acd = 1;
		//SvrCommNote(SVRNOTEID_ALARM);
		SvrAlermNote();
	}

	SaveRunState();	
}

/**
* @brief 获得事件计数
* @param ec 返回事件计数指针(Event Count)
*/
void GetAlarmEc(unsigned char *ec)
{
	//unsigned short us;
	int i;

	for(i=0; i<2; i++) {	//i=0 重要事件,i=1 一般事件
		ec[i] = RunState.alarm.cur[i];
	}

}

/**
* @brief 事件接口初始化
* @return 成功0, 否则失败
*/
//DECLARE_INIT_FUNC(AlarmInit);
int AlarmInit(void)
{

	return 0;
}

/**
* @brief 将事件变量转换成上行通信格式
* @param palm 事件变量指针
* @param buf 输出缓存区指针
* @return 填入缓存区的字节数
*/
static int FormatAlarm(alarm_t *palm, unsigned char *buf)
{
	int len;

	if(0 == palm->erc || palm->len > MAXLEN_ALMDATA) return 0;

	len = palm->len;
	len += 7;

	memcpy(buf, &palm->erc, len);
	buf[1] += 5;

	return(len);
}

/**
* @brief 获得上行通信偏移值
* @param dbid 事件分类索引(重要事件,一般事件)
* @param pm 缓存区偏移值
* @return 上行通信偏移值(事件个数?)
*/
static unsigned char GetAlarmOffset(unsigned char dbid, unsigned char pm)
{
	/*if(RunState.alarm.head[dbid] <= pm)
		pm -= RunState.alarm.head[dbid];
	else 
		pm += ((unsigned short)256 - (unsigned short)RunState.alarm.head[dbid]);*/

	return(pm);
}
//请求事件记录(下行报文头部)
typedef struct {
	unsigned char p0[2];
	unsigned char fn[2];
	unsigned char pm;
	unsigned char pn;
} almqry_t;
//请求事件记录(上行报文头部)
typedef struct {
	unsigned char p0[2];
	unsigned char fn[2];
	unsigned char ec[2];
	unsigned char pm;	//起始指针
	unsigned char pn;	//结束指针
} almsnd_t;

/**
* @brief 主动发送告警
* @param sendbuffer 发送缓存区指针
* @return 成功0, 否则失败
*/
int ActiveSendAlarm(qrycurve_buffer_t *sendbuffer)
{ 
	alarm_t almbuf;
	almsnd_t *psend = (almsnd_t *)QRYCBUF_DATA(sendbuffer);
	unsigned char dbid = 0;
	unsigned char cur;
	int rtn;

	RunStateModify.flag_acd = 0;

	rtn = InvalidPm(dbid, RunState.alarm.snd[dbid]);
	if(rtn == 1) {//invalid Pm, adjust to alarm.head
		RunStateModify.alarm.snd[dbid] = RunState.alarm.head[dbid];
	}
	else if(rtn == 2) return 1;  //empty

	psend->p0[0] = psend->p0[1] = 0;
	psend->fn[1] = 0;
	if(0 == dbid) psend->fn[0] = 0x01;	//重要事件
	else psend->fn[0] = 0x02;	//一般事件
	GetAlarmEc(psend->ec);
	psend->pm = GetAlarmOffset(dbid, RunState.alarm.snd[dbid]);
	psend->pn = psend->pm + 1;

	while(1) {
		cur = RunStateModify.alarm.snd[dbid];
		rtn = alarm_readflash_bin(dbid,cur,(unsigned char*)&almbuf,sizeof(alarm_t));
		AssertLog(rtn==-1,"readflash alarm cur(%d) failed!\n",cur);
		if(rtn==-1) break;
		rtn = FormatAlarm(&almbuf, (unsigned char *)(psend+1));
		if(rtn <= 0) {
			RunStateModify.alarm.snd[dbid] += 1;
			if(RunState.alarm.snd[dbid] == RunState.alarm.cur[dbid]) return 1;
		}
		else break;
	}

	if(QRYCBUF_MAXLEN(sendbuffer) < (rtn+sizeof(almsnd_t))) {
		ErrorLog("too short len(%d)\n", QRYCBUF_MAXLEN(sendbuffer));
		return 1;
	}

	QRYCBUF_ADD(sendbuffer, rtn+sizeof(almsnd_t));
	return 0;
}

/**
* @brief 查询告警
* @param recvbuf 接收查询选项缓存区指针
* @param sendbuffer 发送缓存区指针
* @return 成功0, 否则失败
*/
int QueryAlarm(const unsigned char *recvbuf, qrycurve_buffer_t *sendbuffer)
{
	const almqry_t *pqry = (const almqry_t *)recvbuf;
	almsnd_t *psend;
	alarm_t almbuf;
	unsigned char *pbuf;
	unsigned char dbid, base;
	int datalen, num, i, rtn;

	/*DebugPrint(0, "query alarm %02X%02X:%02X%02X %d-%d\n",
		pqry->p0[0], pqry->p0[1], pqry->fn[0], pqry->fn[1], pqry->pm, pqry->pn);
	DebugPrint(0, "head=%d, cur=%d, snd=%d\n", 
		RunState.alarm.head[0], RunState.alarm.cur[0], RunState.alarm.snd[0]);*/

	//if(pqry->p0[0] || pqry->p0[1] || pqry->fn[1]) return 1;  // ? this is true, complier bug?...
	if(pqry->p0[0] | pqry->p0[1] | pqry->fn[1]) return 1;  // change to this...

	if(1 == pqry->fn[0]) {
		dbid = 0;
	}
	else if(2 == pqry->fn[0]) {
		dbid = 1;
	}
	else return 1;

	if(pqry->pm == pqry->pn) return 1;
	if(RunState.alarm.cur[dbid] == RunState.alarm.head[dbid]) return 1;

	RunStateModify.flag_acd = 0;

	if(pqry->pn > pqry->pm) num = (int)(pqry->pn - pqry->pm)&0xff;
	else num = (int)((unsigned short)256 + (unsigned short)pqry->pn - (unsigned short)pqry->pm)&0xff;
	base = pqry->pm;
	//base = pqry->pm + RunState.alarm.head[dbid];
	if(InvalidPm(dbid , base)) {
		base = RunState.alarm.head[dbid];
	}

	psend = (almsnd_t *)QRYCBUF_DATA(sendbuffer);
	psend->p0[0] = psend->p0[1] = 0;
	psend->fn[1] = 0;
	psend->fn[0] = pqry->fn[0];
	GetAlarmEc(psend->ec);
	psend->pm = psend->pn = base;
	datalen = sizeof(almsnd_t);
	pbuf = (unsigned char *)(psend+1);

	DebugPrint(0, "start query %d alarm, dbid=%d, base=%d\n", num, dbid, base);

	for(i=0; i<num; i++) {
		if((datalen + MAXLEN_ALMDATA) > QRYCBUF_MAXLEN(sendbuffer)) {//分帧处理
			unsigned char bakpn = psend->pn;

			if(datalen <= (int)sizeof(almsnd_t)) {
				ErrorLog("too short len(%d)\n", datalen);
				return 1;
			}
			QRYCBUF_ADD(sendbuffer, datalen);
			QRYCBUF_NEXT(sendbuffer);
			if(NULL == sendbuffer) return 1;

			psend = (almsnd_t *)QRYCBUF_DATA(sendbuffer);
			psend->p0[0] = psend->p0[1] = 0;
			psend->fn[1] = 0;
			psend->fn[0] = pqry->fn[0];
			GetAlarmEc(psend->ec);
			psend->pm = psend->pn = bakpn;
			datalen = sizeof(almsnd_t);
			pbuf = (unsigned char *)(psend+1);
		}

		rtn = alarm_readflash_bin(dbid,base,(unsigned char*)&almbuf,sizeof(alarm_t));
		AssertLog(rtn==-1,"readflash alarm cur(%d) failed!\n",base);
		if(rtn==-1) break;
		rtn = FormatAlarm(&almbuf, pbuf);
		DebugPrint(0, "fomat alarm %d, len=%d\n", base, rtn);
		if(rtn <= 0) {
			base++;
			if(base == RunState.alarm.cur[dbid]) break;
			continue;
		}

		base++;
		psend->pn++;
		datalen += rtn;
		pbuf += rtn;
		if(base == RunState.alarm.cur[dbid]) break;
	}

	DebugPrint(0, "datalen=%d, min=%d\n", sendbuffer->datalen, sizeof(almsnd_t));
	if(datalen <= (int)sizeof(almsnd_t)) return 1;
	QRYCBUF_ADD(sendbuffer, datalen);
	return 0;
}


