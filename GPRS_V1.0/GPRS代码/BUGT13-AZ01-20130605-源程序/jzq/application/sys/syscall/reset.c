/**
* syslock.c -- ϵͳ��λ����
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-5
* ����޸�ʱ��: 2009-5-15
*/

//#include <stdio.h>
#include <stdlib.h>

#include "plat_include/debug.h"
#include "plat_include/reset.h"
#include "plat_include/syslock.h"
#include "app_include/monitor/runstate.h"
#include "app_include/cenmet/mdbsave.h"
#include "hal_include/switch.h"
void SysRestart(void)
{

	PrintLog(LOGTYPE_DEBUG,"sys reset!\n\r");
//	SaveMdb();
	SaveRunState();

	Sleep(100);

	SysReset();
	//_exit(0);
	//return;
}

void SysPowerDown(void)
{
	PrintLog(LOGTYPE_DEBUG,"power down!\n\r");
//	SaveMdb();
	SaveRunState();

	Sleep(100);
	SysHalt();
	//return;
}

void SysAppExit(void)
{
	/*interface*/
	return;
}

