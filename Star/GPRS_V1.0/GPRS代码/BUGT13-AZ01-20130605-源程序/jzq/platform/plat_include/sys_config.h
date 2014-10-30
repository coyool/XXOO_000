/**
* sys_config.h -- ϵͳ����ģ�������ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-3-30
* ����޸�ʱ��: 2009-3-30
*/

#ifndef _SYS_CONFIG_H
#define _SYS_CONFIG_H

/*
	С����������
	���еĴ�С������ԭ��ͼ�Ĳ�ͬ���ɴ˺����
*/
/**********************
	С����������
***********************/
//#define SMALL_CONCENT


/***********************
 *	����������	   *
 ***********************/
//#define BIG_CONCENT

/***********************
*	���Gprsģ�鶨��   *
************************/
//#define METER_MODULE


#define MAX_PTHREAD		32  //�������߳�(����)��

//���ʱ�Ӷ�ʱ����
#define MAX_RTIMER		8
//�����Զ�ʱ����
#define MAX_CTIMER   	8

#define MAX_SYSLOCK		2

/*
	���Կ�ʹ�õĴ��ں���
*/
#ifdef SMALL_CONCENT
#define DEBUG_PORT 	1
#endif

#ifdef BIG_CONCENT
#define DEBUG_PORT 	2
#endif

#ifdef METER_MODULE
#define DEBUG_PORT 	1
#endif

/*
	ϵͳ����
*/
#ifdef METER_MODULE
#define SYSTEM_NAME 	"���ģ��"
#endif

#ifdef SMALL_CONCENT
#define SYSTEM_NAME 	"���׼�����"
#endif

#ifdef BIG_CONCENT
#define SYSTEM_NAME 	"������"
#endif




#ifdef METER_MODULE
#define USE_SECOND
#endif
/*
	�ز�����
*/
#ifdef BIG_CONCENT
//	#define TEST_PLC
#endif

#define DEBUG_BPS	115200

//#define GPRS_MODULE_MC55I
#define GPRS_MODULE_M35


/*
	Gprsʹ�õĴ��ں���
*/
#ifdef METER_MODULE
#define  GPRS_PORT 2
#else
#define  GPRS_PORT 0
#endif

#define UART_IR	      4
#define UART_DOWN485_1 	0
#define GPRS_FRAG		1460

#ifdef BIG_CONCENT
#define DEFINE_PLCOMM
#endif

#ifdef 	METER_MODULE 
#define DEFINE_SERIAL
#endif


#ifdef 	DEFINE_PLCOMM
#define PLC_PORT        3
#endif

#define FLASH_PFPARA_SECTOR			1
#define FLASH_PFPARA_BACK_SECTOR 	2

#endif /*_SYS_CONFIG_H*/
