#include "stdio.h"
#include "stdlib.h"

#include "plat_include/sleep.h"
#include "ucos_ii.h"
#include "hal_include/switch.h"
#include "plat_include/mutex.h"
#include "plat_include/debug.h"
#include "plat_include/timer.h"
#include "plat_include/shellcmd.h"
#include "plat_include/plcomm.h"
#include "plat_include/sys_config.h"
#include "plat_include/plat_thread.h"
#include "app_include/cenmet/forward.h"



#define TASK_BOARD_RUN_STK_SIZE 200
#define TASK_GPRS_STK_SIZE  300

//#define TASK_IR_STK_SIZE 600
//#define TASK_INSIDE_STK_SIZE 600
//#define TASK_TIMER_STK_SIZE 100
#define TASK_MET_STK_SIZE 400
//#define TASK_METSTIC_STK_SIZE 100


#define TASK_UP485_PRIO 6
#define TASK_IR_PRIO 7
#define TASK_GPRS_PRIO 8
#define TASK_INSIDE_PRIO 9
//#define TASK_TIMER_PRIO 10
#define TASK_MET_PRIO 11
#define TASK_METSTIC_PRIO 12

#define TASK_BOARD_RUN_PRIO 60


//__no_init static  OS_STK  task_gprs_stk[TASK_GPRS_STK_SIZE] @ 0x20080000;
//static  OS_STK  task_gprs_stk[TASK_GPRS_STK_SIZE];

//static  OS_STK  task_inside_stk[TASK_INSIDE_STK_SIZE]@ "AHB_RAM_MEMORY";
//static  OS_STK  task_ir_stk[TASK_IR_STK_SIZE]@ "AHB_RAM_MEMORY";
//static  OS_STK  task_timer_stk[TASK_TIMER_STK_SIZE];
//static  OS_STK  task_met_stk[TASK_MET_STK_SIZE];
//static  OS_STK  task_metstic_stk[TASK_METSTIC_STK_SIZE]@ "AHB_RAM_MEMORY";

//static  OS_STK  task_board_run_stk[TASK_BOARD_RUN_STK_SIZE];


void task_gprs(void *);
void task_up485(void *);
void task_ir(void *);
void task_inside(void *);
void task_sysTimerTask(void *);
void SvrCommTask(void *arg);
int	MonitorInit(void);
void MonitorTask(void);

extern void *CenMetTask(void * arg);
extern void InitUp485(void);
extern int SysInit(void);
extern int ParamInit(void);
void RunTestTask(void);
void RunStackCheck(void);

void task_app(void *arg)
{
	(void)arg;
	
	__GlobalLock();	
	
	watchdog_feed();
 
	if(SysInit()) {
		DebugPrint(0,"system init failed!! quit\n");
		return;
	}

	watchdog_feed();

	if(ParamInit()) {
		DebugPrint(0,"param init failed!! quit\n");
		return;
	}

	watchdog_feed();

//	MonitorInit();
	RunTimeThread();

	InitCenmet();	//´®¿Ú³­±í

#ifndef TEST_PLC
	SysCreateTask(SvrCommTask,0,"Svr Comm Task",-1,TASK_GPRS_STK_SIZE,TASK_GPRS_PRIO);//uplink 300 8
#endif
#ifdef BIG_CONCENT
	InitUp485();
#endif
//	MonitorTask();
#ifdef DEFINE_PLCOMM
	PlcCommInit();
#endif
	
#ifdef TEST_PLC
	SysCreateTask(RunTestTask,0,"Test Task",0,200,25);//test 200 25
#endif
//	SysCreateTask(RunStackCheck,0,"Stack Check Task",0,200,26);//check 200 26
		TtyShellStart();
//while(1)Sleep(2000);;
}
void RunAppTask()
{
#define TASK_APP_STK_SIZE 150
#define TASK_APP_PRIO 		21
   SysCreateTask(task_app,0,"Task App",0,TASK_APP_STK_SIZE,TASK_APP_PRIO);//APP 150 21
}

#ifdef TEST_PLC
void TestPlc(void);
void RunTestTask(void)
{
	__GlobalLock();
	PrintLog(LOGTYPE_DEBUG,"Running Test Task!\r\n");
	while(1)
	{
		TestPlc();
		Sleep(50);
	}
}
#endif

void RunStackCheck(void)
{
	OS_STK_DATA stk;
	PrintLog(LOGTYPE_DEBUG,"Running Stack Check!\r\n");
	while(1)
	{
		int i;
		for(i=0;i<32;i++)
		{
			if(i == 26)
				continue;
			if(OSTaskStkChk(i,&stk) == OS_ERR_NONE)
			{
				const char *name = SysGetTaskName(i);
				if(name)
				PrintLog(LOGTYPE_DEBUG,"Stack pro:%d,	free:%d,	used:%d		name:%s\r\n",i,stk.OSFree,stk.OSUsed,name);		
			}
		}
		OSTimeDly(10);
		
	}
}


