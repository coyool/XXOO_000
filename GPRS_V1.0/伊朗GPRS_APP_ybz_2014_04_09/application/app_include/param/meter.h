/**
* meter.h -- ��Ʋ���ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-7
* ����޸�ʱ��: 2009-5-7
*/

#ifndef _PARAM_METER_H
#define _PARAM_METER_H

#include "app_include/param/capconf.h"
#include "app_include/param/commport.h"
#include "app_include/param/datause.h"
#include "app_include/version.h"
#include "plat_include/timeal.h"
#include "plat_include/sys_config.h"


//F10, �ն˵��ܱ�/�����������ò���
#define METTYPE_DL645		1
#define METTYPE_ACSAMP		2
#define METTYPE_DL645_XF	11	//��������
#define METTYPE_DL645_TF	12	//��������
#define METTYPE_DL645_2007	30

#define METTYPE_PLC			31

/*
typedef struct {
	unsigned short index;  //���������,������, �����ط�����
	unsigned short metp_id;    //�����������, 1~2040, 0��ʾ��Ч
	unsigned char port;    //ͨ�Ŷ˿ں�
	unsigned char proto;    //ͨ�Ź�Լ����, 1-DL645(1997), 2-��������, 30-DL645(2007)
	unsigned char addr[6];    //ͨ�ŵ�ַ
	unsigned char pwd[6];    //ͨ������
	unsigned char prdnum;    //���ܷ��ʸ���
	unsigned char intdotnum;    //�й�����ʾֵ����λ��С��λ����
	unsigned char owneraddr[6];    //�����ɼ�����ַ
	unsigned char usrclass;    //�û�����ź͵������
	unsigned char speed;  //ͨ������
} para_meter_t;
*/
typedef struct {
	unsigned char metp_id;    //�����������, 1~2040, 0��ʾ��Ч
//	unsigned char port;    //ͨ�Ŷ˿ں�
	//unsigned char proto;    //ͨ�Ź�Լ����, 1-DL645(1997), 2-��������, 30-DL645(2007)
	unsigned char addr[6];    //ͨ�ŵ�ַ
#ifdef METER_MODULE
	unsigned char prdnum;    //���ܷ��ʸ���
	unsigned char usrclass;    //�û�����ź͵������
	unsigned char owneraddr[6];    //�����ɼ�����ַ
#endif
//	unsigned char speed;  //ͨ������
} simple_para_meter_t;

#ifndef DEFINE_PARAMETER
extern /*const*/simple_para_meter_t ParaMeter[MAX_METER];
#endif

///��Ч������
#define EMPTY_METER(metid)		(ParaMeter[metid].metp_id == 0)
// || ParaMeter[metid].proto == 0)

#define METERSN_TO_METPID(metersn)  (ParaMeter[metersn-1].metp_id)

//�ܱ�
//#define CEN_METER(metid)		(ParaMeter[metid].port == (COMMPORT_CEN_485_1+1) || \
//								 ParaMeter[metid].port == (COMMPORT_CEN_485_2+1)) 
//RS485�ܱ�
#define RS485_CENMETER(metid)	(ParaMeter[metid].port == (COMMPORT_CEN_485_1+1) || \
								 ParaMeter[metid].port == (COMMPORT_CEN_485_2+1)) 
//�ز��û���
#define PLC_METER(metid)		0
//RS485�û���
#define RS485_USRMETER(metid)	0
//�û���
#define USER_METER(metid)		0
//�����ʱ�
#define SF_METER(metid)			(ParaMeter[metid].prdnum == 1 || \
								(ParaMeter[metid].proto == METTYPE_DL645_TF))
//����ʱ� ˳��Ϊ�ܡ��塢ƽ���ȡ���
#define XF_METER(metid)			(ParaMeter[metid].proto == METTYPE_DL645_XF)								

//�����ʱ� ˳��Ϊ�ܡ��塢��
#define TF_METER(metid)			(ParaMeter[metid].proto == METTYPE_DL645_TF)	


#endif /*_PARAM_METER_H*/

