/**
* timer.c -- ��ʱ�������ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2008-5-16
* ����޸�ʱ��: 2009-6-6
*/

//#include <stdio.h>
#include <string.h>
#include "ucos_ii.h"
#include "plat_include/sys_config.h"
#include "plat_include/mutex.h"
#include "plat_include/timeal.h"
#include "plat_include/timer.h"
#include "plat_include/debug.h"
#include "plat_include/sleep.h"
#include "plat_include/shellcmd.h"
#include "plat_include/plat_thread.h"


//ʱ�Ӷ�ʱ�����Ʊ����ṹ
typedef struct st_rtman {
	struct st_rtman *next;
	struct st_rtman *prev;
	int id;
	rtimerproc_t proc;//�ص�����
	unsigned long arg;//����
	utime_t tstart;
	utime_t tbase;
	unsigned char dev;//��ʱ����
	unsigned char mod;//��λ,0min/1hour/2day/3month
	unsigned char bonce;	//����
	unsigned char flag;
	utime_t basetime;
} rtman_t;

//��Զ�ʱ�����Ʊ����ṹ
typedef struct st_ctman {
	struct st_ctman *next;
	struct st_ctman *prev;
	int id;
	ctimerproc_t proc;
	unsigned long arg;
	unsigned long dev;//��ʱ����
	unsigned long end;//��ʱʱ��//��ǰ����ֵ
	unsigned char flag;
} ctman_t;

static void CTimerProc(void);
static void RTimerProc(utime_t timec);

//����ִ������ָ��(200msִ��һ��)
static void (*FastRountine)(void) = NULL;

static unsigned int AppStartSecs = 0;

/**
* @brief ��ȡ������������������
*/
unsigned int GetAppStartSeconds(void)
{
	return AppStartSecs;
}

/**
* @brief ϵͳ��ʱ������
* @param arg ��������
*/
#ifdef USE_SECOND
extern unsigned char SecondFlag;
#endif
void task_sysTimerTask(void *arg)
{
	sysclock_t clock;
	int countRtimer;
	int countCtimer;

	int countPrint = 0;

	__GlobalLock();
	
	PrintLog(LOGTYPE_DEBUG,"call task systime task\n");
	SysClockRead(&clock);

	countRtimer = countCtimer = 0;
	//SysTimerInit();
	
	while(1) {
		if(NULL != FastRountine) (*FastRountine)();
		countCtimer++;
		if(countCtimer >= 5) 
		{
			SysClearCurrentTaskState();
			countCtimer = 0;
			AppStartSecs++;
			CTimerProc();
			countPrint++;
			if(countPrint >= 60) 
			{ // 1 minute
				countPrint = 0;
				//PrintLog(LOGTYPE_ONLYSAVE, "\n***current time = %s***\n", SysClockFormat(&clock));
			}
		}


		countRtimer++;
		if(countRtimer >= 4)  { // 0.8s
			countRtimer = 0;
			SysClockRead(&clock);
			//PrintLog(LOGTYPE_DEBUG,"clock %02d-%02d-%02d %02d:%02d:%02d %d\n",clock.year,clock.month,
			//clock.day,clock.hour,clock.minute,clock.second,clock.week);
			RTimerProc(UTimeReadCurrent());
		}

		Sleep(10);//ÿ����λ20����
	}
}

/**
* @brief ���ÿ�������
* @param routine ����������ں���
*/
void SysSetFastRoutine(void (*routine)(void))
{
	FastRountine = routine;
}

#define TMAN_EMPTY   0
#define TMAN_VALID   1

//ʱ�Ӷ�ʱ�����Ʊ�����
static rtman_t RTimerList;
static rtman_t RTimerBuffer[MAX_RTIMER];
//��Զ�ʱ�����Ʊ�����
static ctman_t CTimerList;
static ctman_t CTimerBuffer[MAX_CTIMER];
//ͬ����ʱ�����ʵĻ������
//atic sys_mutex_t CTimerMutex, RTimerMutex;
//����ID,������ֹID�ظ�
static int CTimerIdAddup = 0;
static int RTimerIdAddup = 0;

/**
* @brief ���һ����Զ�ʱ��
* @param dev ��ʱ��ִ�м��(����Ϊ��λ)
* @param proc ��ʱ��������
* @param arg ��ʱ��������
*/
int SysAddCTimer(int dev, ctimerproc_t proc, unsigned long arg)
{
	int i;
	ctman_t *p;

	AssertLogReturn(dev<=0, -1, "invalid dev(%d)\n", dev);

	//sysLockMutex(&CTimerMutex);

	for(i=0; i<MAX_CTIMER; i++) {
		if(TMAN_EMPTY == CTimerBuffer[i].flag) {
			p = &(CTimerBuffer[i]);
			p->flag = TMAN_VALID;
			p->proc = proc;
			p->arg = arg;
			p->end = UTimeReadCurrent() + dev;
			p->dev = dev;
			p->id = (CTimerIdAddup & 0xffff) | (i << 16);
			CTimerIdAddup++;

			p->next = &CTimerList;
			p->prev = CTimerList.prev;
			CTimerList.prev->next = &CTimerBuffer[i];
			CTimerList.prev = &CTimerBuffer[i];

			//sysUnlockMutex(&CTimerMutex);
			return p->id;
		}
	}

	//sysUnlockMutex(&CTimerMutex);

	ErrorLog("CTimer full\n");
	return -1;
}

/**
* @brief ֹͣ��Զ�ʱ��
* @param id ��ʱ��id
*/
void SysStopCTimer(int id)
{
	ctman_t *p;
	int offset;

	offset = id>>16;
	if((offset < 0) || (offset >= MAX_CTIMER)) {
		ErrorLog("invalid id(%d)\n", id);
		return;
	}

	p = &CTimerBuffer[offset];

	//sysLockMutex(&CTimerMutex);

	if((p->id == id) && (TMAN_EMPTY != p->flag)) {
		p->flag = TMAN_EMPTY;
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	//sysUnlockMutex(&CTimerMutex);
}

/**
* @brief ֹͣ��Զ�ʱ��(ָ����ʽ)
* @param p ��ʱ�����Ʊ���ָ��
*/
static void StopCTimerP(ctman_t *p)
{
	if(TMAN_EMPTY != p->flag) {
		p->flag = TMAN_EMPTY;
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}
}

/**
* @brief �����Զ�ʱ��������,���¿�ʼ����
* @param id ��ʱ��id
*/
void SysClearCTimer(int id)
{
	ctman_t *p;
	int offset;

	offset = id>>16;
	if((offset < 0) || (offset >= MAX_CTIMER)) {
		ErrorLog("invalid id(%d)\n", id);
		return;
	}

	p = &CTimerBuffer[offset];

	//sysLockMutex(&CTimerMutex);

	if((p->id == id) && (TMAN_EMPTY != p->flag)) {
		p->end = UTimeReadCurrent() + p->dev;
	}

	//sysUnlockMutex(&CTimerMutex);
}
/**
* @brief ���¼���������Զ�ʱ��
*/
void SysRecalAllCTimer(void)
{
	ctman_t *p;

	DebugPrint(LOGTYPE_ALARM, "recall all ctimer...\n");

	//sysLockMutex(&RTimerMutex);

	p = CTimerList.next;
	while(p != &CTimerList) 
	{
		if(TMAN_EMPTY != p->flag)
		{
			p->end = UTimeReadCurrent() + p->dev;
		}
		p = p->next;
	}

	//sysUnlockMutex(&RTimerMutex);
}
/**
* @brief ��Զ�ʱ��������
*/
static void CTimerProc(void)
{
	ctimerproc_t func;
	ctman_t *p, *p1;
	int rc;

	//sysLockMutex(&CTimerMutex);

	p = CTimerList.next;
	while(p != &CTimerList) {
		p1 = p->next;

		//p->cnt++;

		if(UTimeReadCurrent() > p->end) {
			p->end = p->dev + UTimeReadCurrent();
			func = p->proc;
			rc = (*func)(p->arg);
			if(rc) StopCTimerP(p);
		}

		p = p1;
	}

	//sysUnlockMutex(&CTimerMutex);
}

/**
* @brief �������ü����׼ʱ��
* @param p ʱ�Ӷ�ʱ�����Ʊ���ָ��
* @return ��׼ʱ��
*/
static utime_t RTimerGetBase(unsigned char mod, const sysclock_t *baseclock)
{
	utime_t rtn;
	sysclock_t clock;

	memcpy(&clock, baseclock, sizeof(clock));

	clock.year = clock.month = 0;
	switch(mod) {
	case UTIMEDEV_MINUTE:
		clock.minute = 0;
	case UTIMEDEV_HOUR:
		clock.hour = 0;
	case UTIMEDEV_DAY:
		clock.day = 0;
		break;
	default: //month
		if(clock.day) clock.day -= 1;
		break;
	}

	rtn = 0;
	if(0 != clock.day) rtn += ((utime_t)(clock.day)&0xff)*1440*60;
	if(0 != clock.hour) rtn += ((utime_t)(clock.hour)&0xff)*60*60;
	if(0 != clock.minute) rtn += ((utime_t)(clock.minute)&0xff)*60;

	return rtn;
}

/**
* @brief ���¼��㶨ʱ������ʱ��
* @param p ʱ�Ӷ�ʱ�����Ʊ���ָ��
* @param curtime ��ǰʱ��
*/
static void RecalRTimerP(rtman_t *p, utime_t curtime)
{
	sysclock_t clock;

	if(UTIMEDEV_MINUTE == p->dev) {
		if(0 == p->mod) p->dev = 15;
		else p->dev = 1;
	}
	if(p->mod > UTIMEDEV_MONTH) p->mod = UTIMEDEV_HOUR;

	if(p->bonce) return;

	UTimeToSysClock(curtime, &clock);

	switch(p->mod) {
	case UTIMEDEV_MONTH:
		clock.month = 1;
	case UTIMEDEV_DAY:
		clock.day = 1;
	case UTIMEDEV_HOUR:
		clock.hour = 0;
	case UTIMEDEV_MINUTE:
		clock.minute = 0;
		clock.second = 0;
		break;
	default:
		return;
	}

	p->tstart = SysClockToUTime(&clock);
	p->tbase = p->tstart;
	//p->tstart += RTimerGetBase(p);
	p->tstart += p->basetime;
	while(curtime >= p->tstart) {
		p->tbase = UTimeAdd(p->tbase, p->mod, p->dev);
		p->tstart = p->tbase + p->basetime;
		//p->tstart += RTimerGetBase(p);
		//p->tstart += p->basetime;
	}

	//DebugPrint(LOGTYPE_SHORT, "rtimer start: %s\n", UTimeFormat(p->tstart));

	//PrintLog(LOGTYPE_DEBUG,"rcal timer: %s\n", ascii_utime(p->tstart));
	//print_utime(p->tstart, "recal rtimer");
}

/**
* @brief ���¼���ʱ�Ӷ�ʱ��
* @param id ��ʱ��id
*/
void SysRecalRTimer(int id)
{
	rtman_t *p;
	int offset;

	offset = id>>16;
	if((offset < 0) || (offset >= MAX_RTIMER)) return;

	p = &RTimerBuffer[offset];

	//sysLockMutex(&RTimerMutex);

	if((p->id == id) && (TMAN_EMPTY != p->flag)) {
		RecalRTimerP(p, UTimeReadCurrent());
	}

	//sysUnlockMutex(&RTimerMutex);
}

/**
* @brief ���¼�������ʱ�Ӷ�ʱ��
*/
void SysRecalAllRTimer(void)
{
	rtman_t *p;

	DebugPrint(LOGTYPE_ALARM, "recall all rtimer...\n");

	//sysLockMutex(&RTimerMutex);

	p = RTimerList.next;
	while(p != &RTimerList) {
		if(TMAN_EMPTY != p->flag) {
			RecalRTimerP(p, UTimeReadCurrent());
		}

		p = p->next;
	}

	//sysUnlockMutex(&RTimerMutex);
}

/**
* @brief ��ʼ��ʱ�Ӷ�ʱ�����ñ���
*/
void SysInitRTimerConf(rtimer_conf_t *pconf)
{
	memset(pconf, 0, sizeof(rtimer_conf_t));
}

/**
* @brief ���һ��ʱ�Ӷ�ʱ��
* @param pconfig ��ʱ�������ñ���ָ��
* @param proc ��ʱ��������
* @param arg ����������
* @return �ɹ�ʱ���ض�ʱ��id, ���򷵻�-1
*/
int SysAddRTimer(const rtimer_conf_t *pconf, rtimerproc_t proc, unsigned long arg)
{
	int i;
	rtman_t *p;
	utime_t curtime = UTimeReadCurrent();

	if((0 == pconf->tdev) || (pconf->tmod > UTIMEDEV_MONTH)) {
		ErrorLog("invalid conf(%d, %d)\n", pconf->tdev, pconf->tmod);
		return -1;
	}

	PrintLog(LOGTYPE_UPLINK,"add rtimer: %d, %d\r\n", pconf->tdev, pconf->tmod);

	//SysLockMutex(&RTimerMutex);

	for(i=0; i<MAX_RTIMER; i++) {
		if(TMAN_EMPTY == RTimerBuffer[i].flag) {
			p = &(RTimerBuffer[i]);
			p->flag = TMAN_VALID;
			p->proc = proc;
			p->arg = arg;
			p->bonce = pconf->bonce;
			if(pconf->bonce) {
				p->dev = 1;
				p->mod = 1;
				p->tstart = pconf->curtime;
			}
			else {
				p->dev = pconf->tdev;
				p->mod = pconf->tmod;
				//p->basetime = pconf->basetime;
				p->basetime = RTimerGetBase(pconf->tmod, &pconf->basetime);
				RecalRTimerP(p, curtime);
			}
			p->id = (RTimerIdAddup & 0xffff) | (i << 16);
			RTimerIdAddup++;

			p->next = &RTimerList;
			p->prev = RTimerList.prev;
			RTimerList.prev->next = &RTimerBuffer[i];
			RTimerList.prev = &RTimerBuffer[i];

			//SysUnlockMutex(&RTimerMutex);
			return p->id;
		}
	}

	//SysUnlockMutex(&RTimerMutex);

	ErrorLog("RTimer full\n");
	return -1;
}

/**
* @brief ֹͣһ��ʱ�Ӷ�ʱ��
* @param id ��ʱ��id
*/
void SysStopRTimer(int id)
{
	rtman_t *p;
	int offset;

	offset = id>>16;
	if((offset < 0) || (offset >= MAX_RTIMER)) return;

	p = &RTimerBuffer[offset];

	//SysLockMutex(&RTimerMutex);

	if((p->id == id) && (TMAN_EMPTY != p->flag)) {
		p->flag = TMAN_EMPTY;
		p->prev->next = p->next;
		p->next->prev = p->prev;
	}

	//SysUnlockMutex(&RTimerMutex);
}

/**
* @brief ֹͣһ��ʱ�Ӷ�ʱ��(ָ����ʽ)
* @param p ʱ�Ӷ�ʱ�����Ʊ���ָ��
*/
static void StopRTimerP(rtman_t *p)
{
	if(TMAN_EMPTY == p->flag) return;

	p->flag = TMAN_EMPTY;
	p->prev->next = p->next;
	p->next->prev = p->prev;
}

/**
* @brief ʱ�Ӷ�ʱ��������
* @param timec ��ǰʱ��
*/
static void RTimerProc(utime_t timec)
{
	rtimerproc_t func;
	rtman_t *p, *p1;
	int bstart;
	int comp;

	//SysLockMutex(&RTimerMutex);

	p = RTimerList.next;
	while(p != &RTimerList) {
		p1 = p->next;
		bstart = 0;

		if(timec >= p->tstart) {
			bstart = 1;
			if(!(p->bonce)) {
				comp = timec - p->tstart;
				if(comp > 3600) {   //�Ӻ�ִ�д���һСʱ���¼���
					RecalRTimerP(p, timec);
				}
				else {
					do {
						p->tbase = UTimeAdd(p->tbase, p->mod, p->dev);
						p->tstart = p->tbase + p->basetime;
						//p->tstart += RTimerGetBase(p);
					} while(timec >= p->tstart);
				}
			}
		}

		if(bstart) {
			func = p->proc;

			(*func)(p->arg, timec);

			if(p->bonce) StopRTimerP(p);
		}

		p = p1;
	}

	//SysUnlockMutex(&RTimerMutex);
}
#define TASK_TIMER_STK_SIZE 100
#define TASK_TIMER_PRIO 9
void RunTimeThread(void)
{
		SysCreateTask(task_sysTimerTask,0,"Timer Task",-1,TASK_TIMER_STK_SIZE,TASK_TIMER_PRIO);//��ʱ���߳�	100	9
}
int shell_timerinfo(int argc, char *argv[]);
DECLARE_SHELL_CMD("timerinfo",shell_timerinfo, "��ʾϵͳ��ʱ����Ϣ");
/**
* @brief ��ʱ��ģ���ʼ������
* @return ����0��ʾ�ɹ�, ����ʧ��
*/
int SysTimerInit(void)
{
	int i;

	for(i=0; i<MAX_RTIMER; i++) RTimerBuffer[i].flag = TMAN_EMPTY;
	RTimerList.next = RTimerList.prev = &RTimerList;

	for(i=0; i<MAX_CTIMER; i++) CTimerBuffer[i].flag = TMAN_EMPTY;
	CTimerList.next = CTimerList.prev = &CTimerList;

 	INIT_SHELL_CMD(shell_timerinfo);
	return 0;
}

#if 1
int shell_timerinfo(int argc, char *argv[])
{
	char flag;

	if(2 != argc) {
		PrintLog(LOGTYPE_DEBUG, "timerinfo r/c\n");
		return 1;
	}

	flag = argv[1][0];

	if('c' == flag) {
		ctman_t *p;

		p = CTimerList.next;
		while(p != &CTimerList) {

			PrintLog(LOGTYPE_DEBUG, "ctimer %2d: ", ((unsigned int)p-(unsigned int)CTimerBuffer)/sizeof(ctman_t));
			PrintLog(LOGTYPE_DEBUG, "dev=%ds, end=%d\n", p->dev, p->end);

			p = p->next;
		}
	}
	else if('r' == flag) {
		rtman_t *p;

		p = RTimerList.next;
		while(p != &RTimerList) {

			PrintLog(LOGTYPE_DEBUG, "rtimer %2d: ", ((unsigned int)p-(unsigned int)RTimerBuffer)/sizeof(rtman_t));
			PrintLog(LOGTYPE_DEBUG, "dev=%d, mod=%d, once=%d\n", p->dev, p->mod, p->bonce);
			PrintLog(LOGTYPE_DEBUG, "  base=%s, ", UTimeFormat(p->basetime));
			PrintLog(LOGTYPE_DEBUG, "start=%s\n", UTimeFormat(p->tstart));

			p = p->next;
		}
	}
	else {
		PrintLog(LOGTYPE_DEBUG, "invalid arg\n");
		return 1;
	}

	PrintLog(LOGTYPE_DEBUG, "end\n");
	return 0;
}

#endif
