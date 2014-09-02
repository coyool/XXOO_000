/**
* event.h -- �¼��ӿ�ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-3-30
* ����޸�ʱ��: 2009-3-30
*/

#ifndef _SYS_EVENT_H
#define _SYS_EVENT_H

//#include <unistd.h>
//#include <pthread.h>

/**
* ÿ���¼��������԰���32���¼�,
* ��һ��32bit������ʾ, ÿbit����һ���¼�,
* ��bit��1��ʾ���¼�����,��0��ʾδ����
* ÿ���¼��ĺ����Զ���
*/

///�¼����Ʊ����ṹ
typedef struct {
	//pthread_mutex_t mutex;
	//pthread_cond_t cond;
	unsigned long event;
} sys_event_t;

/**
* @brief ��ʼ���¼����Ʊ���
* @param pctrl �¼����Ʊ���ָ��
*/
void SysInitEvent(sys_event_t *pctrl);

/**
* @brief �ȴ��¼�
* @param pctrl �¼����Ʊ���ָ��
* @param bwait �ȴ�ʱ�Ƿ����, 
*          ���Ϊ0, �򲻹���û���յ�����յ��¼�, ��������ֱ�ӷ���
*          ���Ϊ1, �����û���յ�����յ��¼�, ���񽫹���, ֱ���յ�
* @param waitmask ��Ҫ�ȴ����¼�
* @param pevent ���յ����¼�
*/
void SysWaitEvent(sys_event_t *pctrl, int bwait, unsigned long waitmask, unsigned long *pevent);

/**
* @brief �����¼�
* @param pctrl �¼����Ʊ���ָ��
* @param event ��Ҫ���͵��¼�
*/
void SysSendEvent(sys_event_t *pctrl, unsigned long event);

#endif /*_SYS_EVENT_H*/
