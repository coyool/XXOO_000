/**
* event.c -- �¼��ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2008-5-16
* ����޸�ʱ��: 2010-12-27
*/



#include "plat_include/event.h"
#include "plat_include/mutex.h"


/**
* @brief ��ʼ���¼����Ʊ���
* @param pctrl �¼����Ʊ���ָ��
*/
void SysInitEvent(sys_event_t *pctrl)
{
	pctrl->event = 0;
}

/**
* @brief �ȴ��¼�
* @param pctrl �¼����Ʊ���ָ��
* @param bwait �ȴ�ʱ�Ƿ����, 
*          ���Ϊ0, �򲻹���û���յ�����յ��¼�, ��������ֱ�ӷ���
*          ���Ϊ1, �����û���յ�����յ��¼�, ���񽫹���, ֱ���յ�
* @param waitmask ��Ҫ�ȴ����¼�
* @param pevent ���յ����¼�
*/
void SysWaitEvent(sys_event_t *pctrl, int bwait, unsigned long waitmask, unsigned long *pevent)
{
	unsigned long ul;

	ul = pctrl->event;
	if(ul&waitmask) {
		ul &= waitmask;
		pctrl->event &= ~waitmask;
	}
	else ul = 0;

	*pevent = ul;
}

/**
* @brief �����¼�
* @param pctrl �¼����Ʊ���ָ��
* @param event ��Ҫ���͵��¼�
*/
void SysSendEvent(sys_event_t *pctrl, unsigned long event)
{
	pctrl->event |= event;
}

