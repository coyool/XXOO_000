/**
* ttyshell.c -- 串口命令行调试
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-6-5
* 最后修改时间: 2009-6-5
*/

#include <stdio.h>
#include <string.h>
#include "ucos_ii.h"


#include "plat_include/debug.h"
#include "plat_include/shellcmd.h"
#include "plat_include/plat_thread.h"


static void ReadLine(char *buf, int maxlen)
{
	while(1) {
		__GlobalUnlock();
		if(fgets(buf, maxlen-1, stdin) != NULL) {
		//if(gets(buf) != NULL) {
			buf[maxlen-1] = 0;
			__GlobalLock();
			return;
		}

		__GlobalLock();
		Sleep(10);
	}
}

#define SHELLARG_NUM	12
static char CmdLineArgBuf[SHELLARG_NUM][64];
static char *CmdLineArgV[SHELLARG_NUM];

/**
* @brief 串口命令行任务
*/
static void *TtyShellTask(void *arg)
{
	static char command[256];

	shell_func pfunc;
	int argc;

	PrintLog(LOGTYPE_DEBUG,"tty shell start\n");

	while(1) 
	{
		PrintLog(LOGTYPE_DEBUG,"$:");
		ReadLine(command, 256);

		argc = ShellParseArg(command, CmdLineArgV, SHELLARG_NUM);
		if(argc > 0) 
		{
			pfunc = FindShellFunc(CmdLineArgV[0]);
			if(NULL != pfunc) 
			{
				(*pfunc)(argc, CmdLineArgV);
				continue;
			}
		}
		PrintLog(LOGTYPE_DEBUG,command);
	}

//	return 0;
}

int TtyShellStart(void)
{
	int argc;

	for(argc=0; argc<SHELLARG_NUM; argc++) 
	{
		CmdLineArgV[argc] = CmdLineArgBuf[argc];
	}
	{
		TtyShellTask(NULL);	
	}
	return 0;
}

