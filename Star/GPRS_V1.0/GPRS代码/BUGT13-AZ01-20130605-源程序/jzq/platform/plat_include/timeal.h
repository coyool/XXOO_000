/**
* timeal.h -- ʱ�Ӳ����ӿ�ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2008-5-16
* ����޸�ʱ��: 2009-4-23
*/


#ifndef _SYS_TIMEAL_H
#define _SYS_TIMEAL_H

#include "hal_include/rtc.h"

typedef struct sysclock sysclock_t;
 
/**
* @brief ��ȡϵͳʱ��
* @param pclock ��ȡ��ʱ��ֵ����ָ��
* @return �ɹ�ʱ����0, ���򷵻ط���ֵ
*/
int SysClockRead(sysclock_t *pclock);
/**
* @brief ����ϵͳʱ��
* @param pclock ���õ�ʱ��ֵ����ָ��
* @return �ɹ�ʱ����0, ���򷵻ط���ֵ
*/
int SysClockSet(const sysclock_t *pclock);

/*
* @brief ��ϵͳʱ��ת��Ϊutime_t��ʽ
* @param ptime ϵͳʱ��ֵ����ָ��
* @return �ɹ�ʱ���ض�Ӧ��utime_tʱ��, ���򷵻�-1
*/
utime_t SysClockToUTime(const sysclock_t *ptime);
/**
* @brief ��utime_t��ʽת��Ϊϵͳʱ��
* @param utime ��ת��utime_tʱ�����
* @param ptime ת�����ϵͳʱ��ֵ����ָ��
*/
void UTimeToSysClock(utime_t utime, sysclock_t *ptime);

/**
* @brief �Ƚ�2��ϵͳʱ��Ĳ���
* @param ptime1 ��һ��ϵͳʱ�����ָ��
* @param ptime2 �ڶ���ϵͳʱ�����ָ��
* @return 
*   2��ʱ��֮��Ĳ���, ����Ϊ��λ
*   <0 ��ʾptime1����ptime2
*   =0 ��ʾ2��ʱ����ͬ
*   >0 ��ʾptime1����ptime2
*/
int SysClockDifference(const sysclock_t *ptime1, const sysclock_t *ptime2);

///ʱ������ʽ
#define UTIMEDEV_MINUTE		0  //��
#define UTIMEDEV_HOUR		1  //Сʱ
#define UTIMEDEV_DAY		2  //��
#define UTIMEDEV_MONTH		3  //��
/**
* @brief ��ԭ��ʱ��������һ��ʱ��
* @param time ԭʼʱ�����
* @param mod ���ӵ�ʱ����ֵ
* @param dev ���ӵ�ʱ�䵥λ
*     UTIMEDEV_MINUTE -- ��
*     UTIMEDEV_HOUR -- Сʱ
*     UTIMEDEV_DAY -- ��
*     UTIMEDEV_MONTH -- ��
* @return ���Ӻ��ʱ��ֵ
*/
utime_t UTimeAdd(utime_t time, int mod, int dev);

/**
* @brief �õ���ǰϵͳʱ��
*   ����������ض�ʱ������ʱ���µĵ�ǰʱ�仺��ֵ, ���ҪԶ����SysClockRead()
* @param pclock ����ʱ��ı���ָ��
*/
void SysClockReadCurrent(sysclock_t *pclock);

/**
* @brief �õ���ǰϵͳʱ��(utime_t��ʽ)
*   ����������ض�ʱ������ʱ���µĵ�ǰʱ�仺��ֵ, ���ҪԶ����SysClockRead()
* @return ��ǰϵͳʱ��(utime_t��ʽ)
*/
utime_t UTimeReadCurrent(void);

/**
* ע��: XXFormat����ʹ��ͬһ��������,��˲�Ҫͬʱ����
*/

/**
* @brief ��utime_tʱ��ת��Ϊascii�ַ���
* @param time �����ʱ��
* @return ʱ���ַ���ָ��
*/
const char *UTimeFormat(utime_t time);
/**
* @brief ��sysclock_tʱ��ת��Ϊascii�ַ���
* @param clock �����ʱ��
* @return ʱ���ַ���ָ��
*/
const char *SysClockFormat(const sysclock_t *pclock);

/**
* @brief ��ʼ����ʱ��
*/
void StartTimeMeasure(void);
/**
* @brief ֹͣ����ʱ��
* @return �ӿ�ʼ������ֹͣ�����ĺ�����
*/
int StopTimeMeasure(void);
/**
* @brief ֹͣ����ʱ��(��ʽ������)
* @return �ӿ�ʼ������ֹͣ�����ĺ�����(�ַ�����ʽ)
*/
const char *StopTimeMeasureFormat(void);

/**
* @brief ���ϵͳ����ʱ��
* @param clock ʱ�ӱ���ָ��
*/
void GetClockSysStart(sysclock_t *clock);

/**
* @brief ��ȡ�ⲿʱ��
* @param clock ����ʱ�ӱ���ָ��
* @return ����0��ʾ�ɹ�, ����ʧ��
*/
int ReadExternSysClock(sysclock_t *clock);
 
#endif /*_SYS_TIMEAL_H*/


