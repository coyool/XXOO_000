/**
* mdb.c -- 监测模块初始化
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-15
* 最后修改时间: 2009-5-15
*/

//#include <stdio.h>
#include <string.h>

#include "ucos_ii.h"

#include "hal_include/switch.h"
#include "plat_include/debug.h"

#include "plat_include/event.h"
#include "plat_include/timeal.h"
#include "plat_include/timer.h"
#include "app_include/cenmet/mdbstic.h"
#include "plat_include/reset.h"
#include "app_include/monitor/alarm.h"
#include "app_include/lib/bcd.h"
#include "app_include/monitor/runstate.h"

#include "app_include/param/mix.h"

 
#define CMSEV_1MIN		    0x00000020   // 1min static

#define METEV_WAIT    CMSEV_1MIN

static sys_event_t MonitorEvent;

/**
* @brief 生成表计任务事件
* @param event 需生成的事件
*/
static void MakeCenMetEvent(unsigned long event)
{
	SysSendEvent(&MonitorEvent, event);
}


static void RTimer1Minute(unsigned long arg, utime_t utime)
{
	MakeCenMetEvent(CMSEV_1MIN);
}


extern int RunStateInit(void);

static void PowerMonitor(alarm_t *pbuf)
{

	int stat = 0;

	//__GlobalUnlock();
//	stat = pwr_state();
	//DebugPrint(0,"stat=%d\n",stat);
	//__GlobalLock();
	
	if(stat) {  //power on
	      //DebugPrint(0,"RunState.pwroff=%d\n",RunState.pwroff);
			if(RunState.pwroff) 
			{
			#ifdef ALARM_MODULE
			sysclock_t clock;

			RunStateModify.pwroff = 0;
			memset((unsigned char *)pbuf, 0, sizeof(alarm_t));
			pbuf->erc = 14;
			pbuf->len = 5;
			//记录上次停电时间			
			pbuf->min = RunState.timepoweroff[0];
			pbuf->hour = RunState.timepoweroff[1];
			pbuf->day = RunState.timepoweroff[2];
			pbuf->mon = RunState.timepoweroff[3];
			pbuf->year = RunState.timepoweroff[4];
			HexToBcd(&pbuf->min, 5);

			SysClockReadCurrent(&clock);
			pbuf->data[0] = clock.minute;
			pbuf->data[1] = clock.hour;
			pbuf->data[2] = clock.day;
			pbuf->data[3] = clock.month;
			pbuf->data[4] = clock.year;
			HexToBcd(pbuf->data, 5);
			MakeAlarm(ALMFLAG_NORMAL, pbuf);
			#endif
			}

	}
	else {  //power off
	      if(0 == RunState.pwroff) {
		  #ifdef ALARM_MODULE
			sysclock_t clock;

			RunStateModify.pwroff = 1;

			memset((unsigned char *)pbuf, 0, sizeof(alarm_t));
			//记录停电时间	
			SysClockReadCurrent(&clock);
			RunStateModify.timepoweroff[0] = pbuf->min = clock.minute;
			RunStateModify.timepoweroff[1] = pbuf->hour = clock.hour;
			RunStateModify.timepoweroff[2] = pbuf->day = clock.day;
			RunStateModify.timepoweroff[3] = pbuf->mon = clock.month;
			RunStateModify.timepoweroff[4] = pbuf->year = clock.year;
			HexToBcd(&pbuf->min, 5);
			
			pbuf->erc = 14;
			pbuf->len = 5;
			memset(pbuf->data, 0xee, 5);
			MakeAlarm(ALMFLAG_ABNOR, pbuf);
			#endif
		}

		SysPowerDown();
	}
}



/**
* @brief 监测模块初始化
* @return 成功0, 否则失败
*/
int MonitorInit(void)
{
	RunStateInit();
	return 0;
}
extern unsigned char logType;

void MonitorTask(void)
{
	unsigned char count = 3;

	unsigned long ev;
	rtimer_conf_t conf;
	alarm_t alarmbuf;

	static unsigned char count_restart = 9;

	SysInitRTimerConf(&conf);
	conf.curtime = UTimeReadCurrent();
	conf.bonce = 0;
	memset(&conf.basetime, 0, sizeof(sysclock_t));

	conf.tdev = 1;
	conf.tmod = UTIMEDEV_MINUTE;  // 1 minute
	SysAddRTimer(&conf, RTimer1Minute, 0);
	
	Sleep(50);
	logType = 0;
	count = 6;
	while(1)
	{
		SysWaitEvent(&MonitorEvent, 1, METEV_WAIT, &ev);

		if(ev&CMSEV_1MIN)
		{
#ifdef ALARM_MODULE
			UpdateMdbStic();
#endif
			//SysClearCurrentTaskState();
		}

		if(count>=3)
		{
			watchdog_feed();
			//led_run_flash();
			count = 0;
		}
		count++;
		
		
		if(count_restart==0) PowerMonitor(&alarmbuf);
		
		Sleep(20);
	}
	
}



