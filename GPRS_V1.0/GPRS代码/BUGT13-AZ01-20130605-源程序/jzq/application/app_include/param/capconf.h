/**
* capconf.h -- ��������
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-10
* ����޸�ʱ��: 2009-5-10
*/

#ifndef _PARAM_CAPCONF_H
#define _PARAM_CAPCONF_H

#include "plat_include/sys_config.h"

#ifdef METER_MODULE
#define MAX_METER    16   //�����/����������
#endif

#ifdef SMALL_CONCENT
#define MAX_METER    500   //�����/����������
#endif

#ifdef BIG_CONCENT
#define MAX_METER    500   //�����/����������
#endif

#define MAX_METP		128   //����������

#define MAX_CENMETP		MAX_METER     //���๦�ܲ�������, �ز���ƵĲ������ֻ�ܴ������

#define MAX_IMPORTANT_USER		0   //����ص��û�����


#define MAX_PULSE	0  //���������
#define MAX_ISIG	1  //״̬�����������
#define MAX_TGRP	0  //�ܼ��������
#define MAX_DIFFA	0  //�й��ܵ�����������

#define MAX_DTASK_CLS1		64   // 1�����������
#define MAX_DTASK_CLS2		0  // 2�����������

//#define MAX_SWP		0   //�������ִ�
//#define MASK_SWP	0x00  //�ִ�����

//#define MAX_ANASIG		1	//���ֱ��ģ������
//#define MASK_ANASIG		0x00

//#define MAX_VOLANA		0  //����ѹ/����ģ����(δ��)

#endif /*_PARAM_CAPCONF_H*/

