/**
* syslock.c -- 系统资源锁接口函数
* 在某些操作前需要禁止文件操作, 如掉电
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-5
* 最后修改时间: 2009-5-5
*/

#include "include/debug.h"
//#include "include/sys/mutex.h"
#include "sys/sys_config.h"

///static int SysLockNum = 0;
///static sys_mutex_t SysLock[MAX_SYSLOCK];

/**
* @brief 注册一个系统资源锁
* @return 成功返回资源锁id, 否则返回-1
*/
int RegisterSysLock(void)
{
	/*interface*/
	return 0;
}

/**
* @brief 锁住一个系统资源锁
* @param id 系统资源锁id
*/
void LockSysLock(int id)
{
	/*interface*/
	return;
}

/**
* @brief 解锁一个系统资源锁
* @param id 系统资源锁id
*/
void UnlockSysLock(int id)
{
	/*interface*/
	return;
}

/**
* @brief 禁止所有注册了系统资源锁的操作
*/
void SysLockHalt(void)
{
	/*interface*/
	return;
}

