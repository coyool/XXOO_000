#include <stdio.h>
#include "includes.h"
#include "plat_include/plat_uart.h"

static OS_STK debug_task_stk[STARTUP_TASK_STK_SIZE];		  //定义栈
void Task_Debug(void *p_arg);
int flag = 0;
void Task_LED(void *p_arg)
{
	unsigned char buf[20];
	int len;
    (void)p_arg;                		// 'p_arg' 并没有用到，防止编译器提示警告
 	SysTick_init();
    OSTaskCreate(Task_Debug,(void *)0,
	   &debug_task_stk[STARTUP_TASK_STK_SIZE-1], 5);
	while (1)
    {
	   OSTimeDly(10);
	   PrintLog(LOGTYPE_DEBUG,"1\n");
	   if(flag == 1)
	   		while(flag)OSTimeDly(10);
		flag = 1;
		len = UartRecvData(1,buf,20);
        if(len > 0)
		{
			UartSendData(1,buf,len);
			UartSendData(1,"OK\n",3);
			
		} 						   
		flag = 0;

    }
}
void Task_Debug(void *p_arg)
{
	unsigned char buf[21];
	int len;
    (void)p_arg;                		// 'p_arg' 并没有用到，防止编译器提示警告

    while (1)
    {
        OSTimeDly(11);
	   PrintLog(LOGTYPE_DEBUG,"2\n");
	   if(flag == 1)
	   		while(flag)OSTimeDly(10);
		flag = 1;
		len = UartRecvData(1,buf,20);
		buf[len] = 0;
        if(len > 0)
		{
			PrintLog(LOGTYPE_DEBUG,"debug: %s\n",buf);
		
		} 
		flag = 0;
    }
}
#ifdef OS_APP_HOOKS_EN
void          App_TCBInitHook         (OS_TCB          *ptcb)
{
	  (void)ptcb;
}
void          App_TaskCreateHook      (OS_TCB          *ptcb)
{
	  (void)ptcb;
}
void          App_TaskDelHook         (OS_TCB          *ptcb)
{
	  (void)ptcb;	
}
void          App_TaskIdleHook        (void)
{
}
void          App_TaskStatHook        (void)
{
}
void          App_TaskSwHook          (void)
{
}
void          App_TimeTickHook        (void)
{
}
#endif
