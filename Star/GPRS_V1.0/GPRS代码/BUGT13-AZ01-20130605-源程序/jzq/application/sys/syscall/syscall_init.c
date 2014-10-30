/**
* syscall_init.c -- ϵͳ������ģ���ʼ��
* 
* ����: zhuzhiqiang
* ����ʱ��: 2008-3-31
* ����޸�ʱ��: 2009-3-31
*/
//#include <stdio.h>

#include "plat_include/debug.h"

extern int SysTimerInit(void);
extern int SysTimeInit(void);

/**
* @brief ϵͳ������ģ���ʼ������
* @return ����0��ʾ�ɹ�, ����ʧ��
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

