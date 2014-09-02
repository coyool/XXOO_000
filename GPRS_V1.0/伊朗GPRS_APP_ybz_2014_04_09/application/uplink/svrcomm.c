/**
* svrcomm.c -- ������ͨ��
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-19
* ����޸�ʱ��: 2009-5-19
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "plat_include/sleep.h"
#include "plat_include/debug.h"
#include "plat_include/event.h"
#include "plat_include/timeal.h"
#include "plat_include/timer.h"
#include "app_include/param/unique.h"
#include "app_include/param/mix.h"
#include "app_include/monitor/runstate.h"
#include "app_include/uplink/svrnote.h"
#include "app_include/cenmet/qrycurve.h"
#include "app_include/cenmet/qrydata.h"
#include "app_include/monitor/alarm.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
#include "app_include/uplink/keepalive.h"
#include "app_include/uplink/svrcomm.h"
#include "app_include/param/term.h"
#include "app_include/param/meter.h"
#include "plat_include/ring.h"
#include "plat_include/plat_thread.h"

//#define  USE_TASK_LIST

//static unsigned int SvrCommNoteId = 0;
static sys_event_t SvrCommEvent = {0};
unsigned char SvrCommInterface = UPLINKITF_GPRS;
int SvrCommLineState = 0;
int SvrCommFlag = 0;  //����վͨ�ű�־, 0-��ͨ��, 1-��ͨ��
static int TimerIdTaskCls1[MAX_DTASK_CLS1];
static signed char SvrCommNoteId[MAX_DTASK_CLS1];

void InitSvrCommNoteIdList(void)
{
	//SvrCommNoteId = -1;
}
/**
* @brief ��������ͨ���¼�
* @param waitmask ��������
* @param pevent �����¼�����ָ��
*/
void SvrCommPeekEvent(unsigned long waitmask, unsigned long *pevent)
{
	SysWaitEvent(&SvrCommEvent, 0, waitmask, pevent);
}
/**
* @brief ��������ͨ���¼�����
* @rtn	-1,û�������,���������������
* @param pevent �����¼�����ָ��
*/
int SvrCommPeekNoteid(void)
{
	int i = 0;
	for(;i<MAX_DTASK_CLS1;i++)
	{
		if(SvrCommNoteId[i] == 0)
		{
//			SvrCommNoteId[i] = -1;
			return i; 
		}
	}
	return -1;
}
void SvrCommRemoveNoteid(int id)
{
	AssertLogReturnVoid(id > 64, "invalid note id(%d)\n", id);
	SvrCommNoteId[id] = -1;
}

/**
* @brief ��������ͨ���¼�
* @param id �¼����
*/
void SvrCommNote(unsigned char id)
{
	AssertLogReturnVoid(id > 64, "invalid note id(%d)\n", id);	// todo ��ǰ0�����񲻿���,Ӧ��ȥ��id<=0
	if(id != SVRNOTEID_ALARM)
		SvrCommNoteId[id] = 0;
	SysSendEvent(&SvrCommEvent,SVREV_NOTE);
}
/**
* @brief ������������
*/
void SvrConnectNote(void)
{
	SysSendEvent(&SvrCommEvent,SVREV_ACTIVE);
}
void SvrAlermNote(void)
{
	SysSendEvent(&SvrCommEvent,SVREV_ALERM);
}

/**
* @brief 1�����������ϴ���ʱ��
* @param arg �����
* @param utime ��ǰʱ��
*/
static void RTimerDataCls1(unsigned long arg, utime_t utime)
{
	PrintLog(LOGTYPE_SHORT, "start task class one %d ...\n", arg);
	//�������վ������!
	SvrCommNote(arg);
}
/**
* @brief 1�����������ϴ���ʼ��
* @param tid �����
*/
void DataTaskCls1ReInit(int tid)
{
	rtimer_conf_t conf;

	AssertLogReturnVoid(tid < 0 || tid >= MAX_DTASK_CLS1, "invalid taskid(%d)\n", tid);

	SysInitRTimerConf(&conf);

	if(0 == para_comb.uniq.ParaTaskCls1[tid].valid) {
		//PrintLog(LOGTYPE_UPLINK, "DataTaskCls1ReInit:%d Disable\r\n",tid);
		if(TimerIdTaskCls1[tid] >= 0) SysStopRTimer(TimerIdTaskCls1[tid]);
		TimerIdTaskCls1[tid] = -1;
		return;
	}
	PrintLog(LOGTYPE_UPLINK, "DataTaskCls1ReInit:%d Enable\r\n",tid);
	conf.basetime.year = para_comb.uniq.ParaTaskCls1[tid].base_year;
	conf.basetime.month = para_comb.uniq.ParaTaskCls1[tid].base_month;
	conf.basetime.day = para_comb.uniq.ParaTaskCls1[tid].base_day;
	conf.basetime.hour = para_comb.uniq.ParaTaskCls1[tid].base_hour;
	conf.basetime.minute = para_comb.uniq.ParaTaskCls1[tid].base_minute;
	conf.basetime.second = para_comb.uniq.ParaTaskCls1[tid].base_second;
	conf.bonce = 0;
	conf.tdev = para_comb.uniq.ParaTaskCls1[tid].dev_snd;
	conf.tmod = para_comb.uniq.ParaTaskCls1[tid].mod_snd;
	conf.curtime = UTimeReadCurrent();

	if(TimerIdTaskCls1[tid] >= 0) SysStopRTimer(TimerIdTaskCls1[tid]);
	TimerIdTaskCls1[tid] = SysAddRTimer(&conf, RTimerDataCls1, tid);
}

/**
* @brief ���������ϴ���ʼ��
*/
static void DataTaskInit(void)
{
	int tid;

	for(tid=0; tid<MAX_DTASK_CLS1; tid++) 
	{
		TimerIdTaskCls1[tid] = -1;
		SvrCommNoteId[tid] = -1;
		DataTaskCls1ReInit(tid);
	}
}
static char SendSmsBuf[150];
__inline void SendSmsMessage(const char* msg)
{
	strcpy(SendSmsBuf,msg);
	SysSendEvent(&SvrCommEvent,SVREV_SMS);
} 
static char SendUserSmsBuf[150];
__inline void SendUserSmsMessage(const char* msg)
{
	strcpy(SendUserSmsBuf,msg);
	SysSendEvent(&SvrCommEvent,SVREV_USER_SMS);
} 

__inline char* GetSmsMessage(void)
{
	return SendSmsBuf;
}
__inline char* GetUserSmsMessage(void)
{
	return SendSmsBuf;
}
__inline void CallCsq(void)
{
	SysSendEvent(&SvrCommEvent,SVREV_CSQ);
} 
extern void GprsTask(void);
extern void *GprsChkRingTask(void *arg);
extern void SerialActiveTask(void);
//extern void IrActiveTask(void);
//extern unsigned char CheckParamOkFlag(void);




extern void GprsTask(void);
extern void EtherTask(void);
extern int SvrSelfTranFileInit(void);
/**
* @brief �澯����
* @param itf �ӿڱ��
* @return ��������0, ������ֹ
*/
static int AlarmProc(unsigned char itf)
{
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	qrycurve_buffer_t *cache;
	int rtn;

	cache = GetQueryCache(UPLINK_SNDNOR(itf));

	while(1) {
		ClearQueryCache(cache);
		ActiveSendAlarm(cache);
		
		smallcpy(psnd->data,cache->buffer,cache->datalen);
		if(cache->datalen <= 4) break;

		//psnd = (uplink_pkt_t *)(cache->buffer - OFFSET_UPDATA);
		psnd->ctrl = UPECHO_USRDATA;
		psnd->afn = UPAFN_QRYCLS3;
		psnd->seq = UPSEQ_SPKT;
		UPLINKAPP_LEN(psnd) = cache->datalen;
		rtn = UplinkActiveSend(itf, 1, psnd);
		if((UPRTN_OKRCV == rtn) || (UPRTN_FAILRCV == rtn)) {
			if(UPRTN_OKRCV == rtn) {
				RunStateModify.cnt_snderr[0] = 0;
				RunStateModify.alarm.snd[0]++;
			}
			QueryCacheUnlock();
			SvrMessageProc(itf);
			QueryCacheLock();
			return 1;
		}
		else if(UPRTN_OKDLMS == rtn)
		{
			SvrMessageDlms(itf);
			return 1;
		}
		else if(UPRTN_TIMEOUT == rtn) {
			RunStateModify.cnt_snderr[0]++;
			if(RunStateModify.cnt_snderr[0] > 5) {  //skip this alarm
				RunStateModify.cnt_snderr[0] = 0;
				RunStateModify.alarm.snd[0]++;
			}

			return 1;
		}
		else if(rtn) return rtn;

		RunStateModify.alarm.snd[0]++;
		SysClearCurrentTaskState();
	}

	SaveRunState();

	return 0;
}

/**
* @brief ����ͨ���¼�����
* @param itf �ӿڱ��
* @return ��ɷ���0,δ��ɷ���1
*/
int SvrNoteProc(unsigned char itf)
{
	//int tid, offset;
	//unsigned int mask;
//	DebugPrint(LOGTYPE_SHORT, "actsend %08XH, %d\n", SvrCommNoteId,ParaMix.bactsend);

	if(ParaMix.bactsend) return 0;

	if(AlarmProc(itf)) goto mark_end;
	
	return 0;

mark_end:
	return 1;
}

/**
* @brief ����ͨ�Ŵ�������
*/
void SvrCommTask(void *arg)
{
	/*osh_event_init(&svrcomm_event);
	keepalive_clear();
	load_svrcomm_itf();*/
	__GlobalLock();

	DataTaskInit();
	

	
	SvrSelfTranFileInit();

	SysInitEvent(&SvrCommEvent);
	ClearKeepAlive();
	InitSvrCommNoteIdList();

	Sleep(100);

	DebugPrint(0,"uplink channel is %s ...\n", "GSM/GPRS");

#ifdef ALARM_MODULE
	CheckSoftChange();
	CheckParamLoadOk();
#endif
	switch(SvrCommInterface) {
	case UPLINKITF_GPRS:
		GprsTask();
		break;
//	case UPLINKITF_ETHER:
//		EtherTask();
//		break;
		
	default: break;
	}

	while(1) Sleep(1000);
}

/**
* @brief �Ƿ������������¼�
* @return �з���1, �޷���0
*/
int SvrCommHaveTask(void)
{
	if(ParaMix.bactsend) return 0;

	else return 0;
}

