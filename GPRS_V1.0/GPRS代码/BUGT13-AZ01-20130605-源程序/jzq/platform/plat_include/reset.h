/**
* syslock.c -- ϵͳ��λ����ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-5
* ����޸�ʱ��: 2009-5-5
*/

#ifndef _RESET_H
#define _RESET_H

/**
* @brief ��λϵͳ
*/
void SysRestart(void);
/**
* @brief ϵͳ����
*/
void SysPowerDown(void);
/**
* @brief Ӧ�ó����˳�
*/
void SysAppExit(void);

#if BOARD_VERSION == 1
/**
* @brief ���̵���(����������)
*/
void SysPowerDownImm(void);
#endif

#endif /*_RESET_H*/
