/**
* event.h -- ����ӿ�ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-3-30
* ����޸�ʱ��: 2009-3-31
*/

#ifndef _SYS_TASK_H
#define _SYS_TASK_H

//#include <unistd.h>

/**
* @brief ����һ������(�߳�)
* @param routine ������ʼִ�к���
* @param arg ִ�к�������
* @param name ��������
* @param timeout ����ʱʱ��(��), Ϊ0��ʾ������ʱ, <0����ȱʡ��3600��,
*                ���񳬹�timeout������ClearCurrentTaskState, ϵͳ�Ḵλ
* @return �ɹ�����0, ʧ�ܷ��ط���ֵ
*/
int SysCreateTask(void *(*routine)(void *), void *arg, const char *name, int timeout);

/**
* @brief �����ǰ����ʱ״̬
*/
void SysClearCurrentTaskState(void);

/**
* @brief ��鵱ǰ����״̬,���������ʱ�Ḵλ
*/
void SysCheckTaskState(void);

/**
* @brief ��ȡ��ǰ��������
*/
const char *SysCurrentTaskName(void);

/**
* @brief �ȴ���������(�߳�)����,�����̵���
*/
void SysWaitTaskEnd(void);

#endif /*_SYS_TASK_H*/
