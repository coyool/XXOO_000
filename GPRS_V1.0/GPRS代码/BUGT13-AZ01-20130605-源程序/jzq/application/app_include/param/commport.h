/**
* commport.h -- ͨ�Ŷ˿ڲ���ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-8
* ����޸�ʱ��: 2009-5-8
*/
#include "app_include/param/unique.h"

#ifndef _PARAM_COMMPORT_H
#define _PARAM_COMMPORT_H

#define DEFAULT_RDMET_CYCLE		15   //ȱʡ����ʱ����(��)

#define COMMFRAME_BAUD_300		0
#define COMMFRAME_BAUD_600		0x20
#define COMMFRAME_BAUD_1200		0x40
#define COMMFRAME_BAUD_2400		0x60
#define COMMFRAME_BAUD_4800		0x80
#define COMMFRAME_BAUD_7200		0xa0
#define COMMFRAME_BAUD_9600		0xc0
#define COMMFRAME_BAUD_19200	0xe0

#define COMMFRAME_STOPBIT_1		0
#define COMMFRAME_STOPBIT_2		0x10

#define COMMFRAME_NOCHECK		0
#define COMMFRAME_HAVECHECK		0x08

#define COMMFRAME_EVENCHECK		0
#define COMMFRAME_ODDCHECK	    0x04

#define COMMFRAME_DATA_5		0
#define COMMFRAME_DATA_6		1
#define COMMFRAME_DATA_7		2
#define COMMFRAME_DATA_8		3

#define COMMPORT_CEN_485_1		1 // 1  //����˿�(�ܱ�)
#define COMMPORT_CEN_485_2		2 // 2  //����˿�(�ܱ�)
#define COMMPORT_CEN_485_NUM	1 // 2  //����˿�(��)

#define MAX_COMMPORT		COMMPORT_CEN_485_NUM //���˿���

//#define COMMPORT_CENMETP	COMMPORT_CEN_485  //ȱʡ����ӿںͲ�����ȡ�˿�(��Ҫ�ǳ���ʱ����)
//#define COMMPORT_USERMETER	COMMPORT_USR_485  //485�û�����ӿ�
//#define COMMPORT_USRMETEXP1	COMMPORT_USR_EXP1
//#define COMMPORT_USRMETEXP2	COMMPORT_USR_EXP2
#define   COMMPORT_DLMS	33

#define RDMETFLAG_ENABLE		0x0001  //��"1"�������Զ�������"0" Ҫ���ն˸��ݳ���ʱ���Զ�����
#define RDMETFLAG_ALL			0x0002  //��"1"Ҫ���ն�ֻ���ص����"0"Ҫ���ն˳����б�
#define RDMETFLAG_FREZ			0x0004  //Ҫ���ն˲��ù㲥���᳭����"0"��Ҫ��
#define RDMETFLAG_CHECKTIME		0x0008  //��"1"Ҫ���ն˶�ʱ�Ե��㲥Уʱ����"0"��Ҫ��
#define RDMETFLAG_FINDMET		0x0010  //��"1"Ҫ���ն���Ѱ����������ĵ����"0"��Ҫ��
#define RDMETFLAG_RDSTATUS		0x0020  //��"1"Ҫ���ն˳���"���״̬��"����"0"��Ҫ��
//F33 �ն˳������в�������
//F34 ���ն˽ӿڵ�ͨ��ģ��Ĳ�������
#define MAXNUM_PERIOD	24
typedef struct {
	unsigned char hour_start;
	unsigned char min_start;
	unsigned char hour_end;
	unsigned char min_end;
}cfg_period_t;

typedef struct {
	unsigned short flag;  //̨�����г������п�����
	unsigned char time_hour;	//������-ʱ��:ʱ
	unsigned char time_minute;  //������-ʱ��:��
	unsigned int dateflag;  //������-����   �ܱ�
	unsigned char cycle;  //������ʱ��, ��, 1~60  �ܱ�
	unsigned char chktime_day;     //�Ե��㲥Уʱ��ʱʱ��:��
	unsigned char chktime_hour;    //�Ե��㲥Уʱ��ʱʱ��:ʱ
	unsigned char chktime_minute;  //�Ե��㲥Уʱ��ʱʱ��:��
	unsigned char periodnum;	//������ʱ����
	unsigned char reserv;
	cfg_period_t period[MAXNUM_PERIOD];  //������ʱ��? �ܱ�
	
	unsigned int baudrate;  //���ն˽ӿڶ�Ӧ�˵�ͨ�����ʣ�bps��
	unsigned char frame;  //���ն˽ӿڶ˵�ͨ�ſ�����
} para_commport_t;

const para_commport_t *GetParaCommPort(unsigned int port);

#endif /*_PARAM_COMMPORT_H*/

