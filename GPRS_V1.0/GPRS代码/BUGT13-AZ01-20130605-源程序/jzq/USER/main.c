#include <stdio.h>
#include "includes.h"
#include "plat_include/mutex.h"
#include "hal_include/switch.h"
#include "plat_include/platpara.h"
#include "hal_include/rtc.h"
#include "plat_include/sleep.h"
#include "app_include/appmain.h"
#include "plat_include/plat_driver.h"
#include "plat_include/plat_thread.h"
#include "plat_include/timeal.h"
#include "plat_include/sys_config.h"
#include "plat_include/debug.h"

void Stm32RtcInit(void);
void RunSystemTask(void);


int main(void)
{
  	BSP_Init(); //板级驱动	


	OSInit();  //uCOS init
	PlatInit();	 // 模块init

	rtc_update(0);

	RunSystemTask();  //task add

	OSStart();
    return 0;
 }
  void task_run(void *arg);
 void RunSystemTask(void)
 {
 	#define TASK_RUN_STK_SIZE 150
	#define TASK_RUN_PRIO 3
	
  	SysCreateTask(task_run,0,"Main Task",0,TASK_RUN_STK_SIZE,TASK_RUN_PRIO); //主线程,定时用RTC校时	150	3
 }
extern void shining_led(void);
#include "mkdate.c"
void task_run(void *arg)
{
 	SysTick_init();
	mutex_init();
 	if(__init_GlobalLock()!=0)
		SysReset();
	__GlobalLock();
	platpara_init();
	printf("\r\nstart runing...\r\n");
	printf("platform make at:\033[1;31;40m%s \033[0m\r\n",MKTIME);
	printf("Sysetm \033[1;32;40m%s\033[0m start at:%s\r\n",SYSTEM_NAME,UTimeFormat(UTimeReadCurrent()));

	RunAppTask();
	while(1)
	{
		rtc_update(1);

		if(UTimeReadCurrent()%300 == 0)
			printf("\r\nTime:%s\r\n",UTimeFormat(UTimeReadCurrent()));

		watchdog_feed();
#ifdef 	BIG_CONCENT
		shining_led();
#endif
		SysCheckTaskState();
		sleep(50);

	}
}

