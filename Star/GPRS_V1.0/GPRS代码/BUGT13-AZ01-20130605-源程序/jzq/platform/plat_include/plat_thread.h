#ifndef __PLAT_THREAD_H__
#define __PLAT_THREAD_H__


void SysInitTaskState(int pro,int timeout);
void SysSetTaskName(int pid,const char* name);
/*
˵��:����һ������(�߳�)
�������:
    routine ������ʼִ�к���
    arg ִ�к�������
    name ��������
    timeout ����ʱʱ��(��),Ϊ0��ʾ������ʱ,-1 ȱʡ��ʱʱ�� 1 Сʱ
	stk_size ��ջ��С
	prio �ȼ�
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
#define SysCreateTask(routine,arg,name,timeout,stk_size,prio)\
{															  \
	static  OS_STK  stk[stk_size];								\
	OSTaskCreateExt((void (*)(void *))routine,					\
							(void			*) arg,				 \
							(OS_STK 		*)&stk[stk_size - 1], \
							(INT8U			 ) prio,			   \
							(INT16U 		 ) prio,				\
							(OS_STK 		*)&stk[0],				 \
							(INT32U 		 ) stk_size,			  \
							(void			*) 0,					   \
							(INT16U 		 )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));	\
							SysSetTaskName(prio,name);	 \
							SysInitTaskState(prio,timeout<0?3600:timeout);			\
}

/*
˵��:�����ǰ����ʱ״̬
�������:
    ��
�������:
    ��
����ֵ:
    ��
*/
inline void SysClearCurrentTaskState(void);

/*
˵��:��鵱ǰ����״̬,���������ʱ�Ḵλ
�������:
    ��
�������:
    ��
����ֵ:
    ��
*/
void SysCheckTaskState(void);

/*
˵��:��ȡ��ǰ��������
�������:
    ��
�������:
    ��
����ֵ:
    ���ƻ���ָ��
*/
const char *SysCurrentTaskName(void);

/*
˵��:�ȴ���������(�߳�)����,�����̵���
�������:
    ��
�������:
    ��
����ֵ:
    ��
*/
void SysWaitTaskEnd(void);

/*
˵��:��ȡָ�����������
�������:
    �����
�������:
    ��
����ֵ:
    ���ƻ���ָ��
*/
const char *SysGetTaskName(int pid);
#endif

