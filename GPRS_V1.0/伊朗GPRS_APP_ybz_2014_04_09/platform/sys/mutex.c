#include "stdio.h"
#include "plat_include/mutex.h"
#include "ucos_ii.h"
#include "stddef.h"
#include "plat_include/debug.h"
#include "plat_include/plat_thread.h"

/*
  ucos中，互斥量考虑优先级反转的问题，需要指定一个优先级，等同于占用一个任务TCB。
  这样每一个互斥量就要额外占用一个任务，显然除非是对任务优先级有非常严格的要求，
  应该限制互斥量的使用。
  而信号量却没有这样的问题，对于集中器这样任务优先级并不是很严格要求的应用来讲，
  用二值信号量作互斥用，显然是比较合算的。
 */

#define MUTEX_MAX	10

static OS_EVENT *mutex[MUTEX_MAX];

void mutex_init(void)
{
	int i;
	for(i=0;i<sizeof(mutex)/sizeof(mutex[0]);i++)
		mutex[i]=NULL;
}




int mutex_create(unsigned int n)
{
	if(n>=sizeof(mutex)/sizeof(mutex[0]))
		return -1;
	if(mutex[n]!=NULL)
		return -1;
	mutex[n] = OSSemCreate(1);
	if(mutex[n]==NULL)
		return -1;
	return 0;
}

int mutex_lock(unsigned int n)
{	
	unsigned char err;
	OS_SEM_DATA sem_data;
	
	if(n>=sizeof(mutex)/sizeof(mutex[0]))
		goto lock_err_0;
	if(mutex[n]==NULL)
		goto lock_err_0;
	if(OSSemQuery(mutex[n],&sem_data)!=OS_NO_ERR)
		goto lock_err_0;

	if(sem_data.OSCnt>1)
		goto lock_err_1;
	if(n>0)
		__GlobalUnlock();

	//PrintLog(LOGTYPE_DEBUG,"A%d %d %d\r\n",OSTCBCur->OSTCBPrio,sem_data.OSCnt,n);
	OSSemPend(mutex[n],0,&err);	
	//PrintLog(LOGTYPE_DEBUG,"B%d %d %d\r\n",OSTCBCur->OSTCBPrio,sem_data.OSCnt,n);
	if(n>0)
		__GlobalLock();

	if(err!=OS_NO_ERR)
		goto lock_err_1;
	return 0;
lock_err_0:
	PrintLog(LOGTYPE_DEBUG,"互斥量%d锁无效,\n",n);
	return -1;
lock_err_1:
	PrintLog(LOGTYPE_DEBUG,"互斥量%d锁值是%d,超出范围,\n",n,sem_data.OSCnt);
	return -1;
}

int mutex_unlock(unsigned int n)
{	
	OS_SEM_DATA sem_data;
	if(n>=sizeof(mutex)/sizeof(mutex[0]))
		goto unlock_err_0;
	if(mutex[n]==NULL)
		goto unlock_err_0;
	if(OSSemQuery(mutex[n],&sem_data)!=OS_NO_ERR)
		goto unlock_err_0;
	if(sem_data.OSCnt!=0)
		goto unlock_err_1;
	
	if(OSSemPost(mutex[n])!=OS_NO_ERR)
		goto unlock_err_1;
	//PrintLog(LOGTYPE_DEBUG,"C%d %d %d\r\n",OSTCBCur->OSTCBPrio,sem_data.OSCnt,n);
	return 0;
unlock_err_0:
	PrintLog(LOGTYPE_DEBUG,"互斥量%d解锁无效,\n",n);
	return -1;
unlock_err_1:
//	PrintLog(LOGTYPE_DEBUG,"C%d %d\r\n",OSTCBCur->OSTCBPrio,sem_data.OSCnt);
	PrintLog(LOGTYPE_DEBUG,"(%d)互斥量%d解锁值是%d,超出范围,\n",OSTCBCur->OSTCBPrio,n,sem_data.OSCnt);
	return -1;
}


