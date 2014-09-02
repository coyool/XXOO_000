#include "stdio.h"
#include "plat_include/mutex.h"
#include "ucos_ii.h"
#include "stddef.h"
#include "plat_include/debug.h"
#include "plat_include/plat_thread.h"

/*
  ucos�У��������������ȼ���ת�����⣬��Ҫָ��һ�����ȼ�����ͬ��ռ��һ������TCB��
  ����ÿһ����������Ҫ����ռ��һ��������Ȼ�����Ƕ��������ȼ��зǳ��ϸ��Ҫ��
  Ӧ�����ƻ�������ʹ�á�
  ���ź���ȴû�����������⣬���ڼ����������������ȼ������Ǻ��ϸ�Ҫ���Ӧ��������
  �ö�ֵ�ź����������ã���Ȼ�ǱȽϺ���ġ�
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
	PrintLog(LOGTYPE_DEBUG,"������%d����Ч,\n",n);
	return -1;
lock_err_1:
	PrintLog(LOGTYPE_DEBUG,"������%d��ֵ��%d,������Χ,\n",n,sem_data.OSCnt);
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
	PrintLog(LOGTYPE_DEBUG,"������%d������Ч,\n",n);
	return -1;
unlock_err_1:
//	PrintLog(LOGTYPE_DEBUG,"C%d %d\r\n",OSTCBCur->OSTCBPrio,sem_data.OSCnt);
	PrintLog(LOGTYPE_DEBUG,"(%d)������%d����ֵ��%d,������Χ,\n",OSTCBCur->OSTCBPrio,n,sem_data.OSCnt);
	return -1;
}


