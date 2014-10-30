/**
 * up485.c: ÉÏÐÐ485
 * Ð»ÎÄ¸Õ 
 * 2011.9.8
 */

#include <stdio.h>
#include <string.h>

#include "plat_include/sleep.h"
#include "ucos_ii.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
#include "app_include/uplink/svrcomm.h"
#include "app_include/version.h"
#include "plat_include/debug.h"
#include "plat_include/plat_thread.h"
#include "app_include/uplink/svrmsg.h"
#include "plat_include/Plat_uart.h"


extern int up485_init(void);
extern int IrInit(void);
extern void EthSvrTcpMonitor(void);
extern int EthSvrTcpInit(void);
#ifdef BIG_CONCENT
void task_up485(void *p_arg)
{
	(void)p_arg;

	__GlobalLock();
	
	PrintLog(LOGTYPE_DEBUG,"task--up485 start...\r\n");

	up485_init();
	//IrInit();
	//EthSvrTcpInit();
	
	UplinkClearState(UPLINKITF_SERIAL);
	//UplinkClearState(UPLINKITF_IR); 
//	UplinkClearState(UPLINKITF_ETHERSVR); 
	
	while(1){
		
		int rt = UplinkRecvPkt(UPLINKITF_SERIAL);

		if(rt == PROTO_376) 
		{
			SvrMessageProc(UPLINKITF_SERIAL);
		}else if(rt == PROTO_DLMS)
		{
			SvrMessageDlms(UPLINKITF_SERIAL);
		}
/*		if(!UplinkRecvPkt(UPLINKITF_IR)) {
			SvrMessageProc(UPLINKITF_IR);
		}
*/		
	//	EthSvrTcpMonitor();
        
        Sleep(20); 
	}
}
void InitUp485(void)
{
#define TASK_UP485_STK_SIZE 400
#define TASK_UP485_PRIO 6
	SysCreateTask(task_up485,0,"Task Up485",0,TASK_UP485_STK_SIZE,TASK_UP485_PRIO);//APP 400 6
}
#endif

