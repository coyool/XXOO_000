/**
* syscall_init.c -- 系统调用子模块初始化
* 
* 作者: zhuzhiqiang
* 创建时间: 2008-3-31
* 最后修改时间: 2009-3-31
*/
//#include <stdio.h>

#include "plat_include/debug.h"

extern int SysTimerInit(void);
extern int SysTimeInit(void);

/**
* @brief 系统调用子模块初始化函数
* @return 返回0表示成功, 否则失败
*/
//DECLARE_INIT_FUNC(SysCallInit);
int SysCallInit(void)
{
	PrintLog(LOGTYPE_DEBUG,"  SysCall init...\n");


	if(SysTimerInit()) return 1;
	if(mutex_create(MUTEX_3))
		 PrintLog(LOGTYPE_DEBUG," mutex_create(MUTEX_3) Error...\n");
	//SET_INIT_FLAG(SysCallInit);

	return 0;
}

