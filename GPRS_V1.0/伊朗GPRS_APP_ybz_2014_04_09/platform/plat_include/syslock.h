/**
* syslock.c -- ϵͳ��Դ���ӿں���ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-5
* ����޸�ʱ��: 2009-5-5
*/

#ifndef _SYSLOCK_H
#define _SYSLOCK_H

/**
* @brief ע��һ��ϵͳ��Դ��
* @return �ɹ�������Դ��id, ���򷵻�-1
*/
int RegisterSysLock(void);
/**
* @brief ��סһ��ϵͳ��Դ��
* @param id ϵͳ��Դ��id
*/
void LockSysLock(int id);
/**
* @brief ����һ��ϵͳ��Դ��
* @param id ϵͳ��Դ��id
*/
void UnlockSysLock(int id);

/**
* @brief ��ֹ����ע����ϵͳ��Դ���Ĳ���
*/
void SysLockHalt(void);

#endif /*_SYSLOCK_H*/

