/**
* syslock.c -- ϵͳ��Դ���ӿں���
* ��ĳЩ����ǰ��Ҫ��ֹ�ļ�����, �����
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-5
* ����޸�ʱ��: 2009-5-5
*/

#include "include/debug.h"
//#include "include/sys/mutex.h"
#include "sys/sys_config.h"

///static int SysLockNum = 0;
///static sys_mutex_t SysLock[MAX_SYSLOCK];

/**
* @brief ע��һ��ϵͳ��Դ��
* @return �ɹ�������Դ��id, ���򷵻�-1
*/
int RegisterSysLock(void)
{
	/*interface*/
	return 0;
}

/**
* @brief ��סһ��ϵͳ��Դ��
* @param id ϵͳ��Դ��id
*/
void LockSysLock(int id)
{
	/*interface*/
	return;
}

/**
* @brief ����һ��ϵͳ��Դ��
* @param id ϵͳ��Դ��id
*/
void UnlockSysLock(int id)
{
	/*interface*/
	return;
}

/**
* @brief ��ֹ����ע����ϵͳ��Դ���Ĳ���
*/
void SysLockHalt(void)
{
	/*interface*/
	return;
}

