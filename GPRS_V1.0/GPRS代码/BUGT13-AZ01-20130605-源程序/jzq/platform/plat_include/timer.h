


/**
* timer.h -- ��ʱ�������ӿ�ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2008-5-16
* ����޸�ʱ��: 2009-3-31
*/

#ifndef _SYS_TIMER_H
#define _SYS_TIMER_H

#include "hal_include/rtc.h"

/*
* ���Զ�ʱ����������ʽ
* argΪ���Ӷ�ʱ��ʱָ���Ĳ���
* timeΪ����ʱ�ĵ�ǰʱ��
*/
typedef void (*rtimerproc_t)(unsigned long arg, utime_t time);

/*
* ��Զ�ʱ����������ʽ
* argΪ���Ӷ�ʱ��ʱָ���Ĳ���
* ��������0ʱ��ʾ����ִ�ж�ʱ��
* ���ط���ֵ��ʾֹͣ�ö�ʱ����ִ��
*/
typedef int (*ctimerproc_t)(unsigned long arg);

/**
* @brief ���ÿ�������
* @param routine ����������ں���
*/
void SysSetFastRoutine(void (*routine)(void));

/**
* @brief ���һ����Զ�ʱ��
* @param dev ��ʱ��ִ�м��(����Ϊ��λ)
* @param proc ��ʱ��������
* @param arg ��ʱ��������
*/
int SysAddCTimer(int dev, ctimerproc_t proc, unsigned long arg);
/**
* @brief ֹͣ��Զ�ʱ��
* @param id ��ʱ��id
*/
void SysStopCTimer(int id);
/**
* @brief �����Զ�ʱ��������,���¿�ʼ����
* @param id ��ʱ��id
*/
void SysClearCTimer(int id);

/**
* @brief ���¼���������Զ�ʱ��
*/
void SysRecalAllCTimer(void);

//ʱ�Ӷ�ʱ���������ݽṹ
typedef struct {
	utime_t curtime;  //��ǰʱ��
	unsigned char bonce;  //�Ƿ�ִֻ��һ��
	unsigned char tdev;  //ִ��ʱ������ֵ
	unsigned char tmod;  //ִ���¼����λ(�μ�utime_add)
	struct sysclock basetime;  //ִ�л�׼ʱ��
	                                 //��ʱ����Ϊ1Сʱ,��׼ʱ��ķ�Ϊ5,��ÿ���0:05, 1:05, 2:05, ...ִ��
} rtimer_conf_t;

/**
* @brief ��ʼ��ʱ�Ӷ�ʱ�����ñ���
*/
void SysInitRTimerConf(rtimer_conf_t *pconf);
/**
* @brief ���¼���ʱ�Ӷ�ʱ��
* @param id ��ʱ��id
*/
void SysRecalRTimer(int id);
/**
* @brief ���¼�������ʱ�Ӷ�ʱ��
*/
void SysRecalAllRTimer(void);
/**
* @brief ���һ��ʱ�Ӷ�ʱ��
* @param pconfig ��ʱ�������ñ���ָ��
* @param proc ��ʱ��������
* @param arg ����������
* @return �ɹ�ʱ���ض�ʱ��id, ���򷵻�-1
*/
int SysAddRTimer(const rtimer_conf_t *pconf, rtimerproc_t proc, unsigned long arg);
/**
* @brief ֹͣһ��ʱ�Ӷ�ʱ��
* @param id ��ʱ��id
*/
void SysStopRTimer(int id);

/**
* @brief ��ȡ������������������
*/
unsigned int GetAppStartSeconds(void);
/*
���ж�ʱ���߳�
*/
void RunTimeThread(void);
/*
��ʱ����Ϣ
*/
int shell_timerinfo(int argc, char *argv[]);
#endif /*_SYS_TIMER_H*/
