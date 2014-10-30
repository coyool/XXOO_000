/**
* metp.h -- ���������ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-7
* ����޸�ʱ��: 2009-5-7
*/

#ifndef _PARAM_METP_H
#define _PARAM_METP_H

#include "app_include/param/capconf.h"

#define PWRTYPE_3X3W	1
#define PWRTYPE_3X4W	2
#define PWRTYPE_1X		3

//F25, �������������
typedef struct {
	unsigned short pt;    //��ѹ����������
	unsigned short ct;    //��������������
	unsigned short vol_rating;    //���ѹ, 0.1V
	unsigned short amp_rating;    //�����, 0.01A
	unsigned int pwr_rating;   //�����, 0.0001kVA
	unsigned char pwrtype;    //��Դ���߷�ʽ, 
	                             // 1=��������,2=��������,3=����
	unsigned char pwrphase;   //����������, 0-unknown, 1-A, 2-B, 3-C
	unsigned char unused[2];
} cfg_mpbase_t;

//F26, ��������ֵ����
typedef struct {
	unsigned short volok_up;    //��ѹ�ϸ�����, 0.1V
	unsigned short volok_low;    //��ѹ�ϸ�����, 0.1V
	unsigned short vol_lack;    //��ѹ��������, 0.1V
	unsigned short vol_over;    //��ѹ����, 0.1V
	unsigned short vol_less;    //Ƿѹ����, 0.1V

	unsigned int amp_over;    //��������, 0.001A
	unsigned int amp_limit;    //���������, 0.001A
	unsigned int zamp_limit;    //�����������, 0.001A

	unsigned int pwr_over;    //���ڹ���������, 0.0001kVA
	unsigned int pwr_limit;    //���ڹ�������, 0.0001kVA

	unsigned short vol_unb;    //�����ѹ��ƽ����ֵ, 0.1%
	unsigned short amp_unb;    //���������ƽ����ֵ, 0.1%

	unsigned char time_volover;   //Խ�޳���ʱ��, ��
	unsigned char time_volless;
	unsigned char time_ampover;
	unsigned char time_amplimit;
	unsigned char time_zamp;
	unsigned char time_pwrover;
	unsigned char time_pwrlimit;
	unsigned char time_volunb;
	unsigned char time_ampunb;
	unsigned char time_volless_2;    //����ʧѹʱ����ֵ, min

	unsigned char resr_pwrover[2];  //Խ�޻ָ�ϵ�� 0.1% BCD��ʽ
	unsigned char resr_pwrlimit[2];
	unsigned char resr_volover[2];
	unsigned char resr_volless[2];
	unsigned char resr_ampover[2];
	unsigned char resr_amplimit[2];
	unsigned char resr_zamp[2];
	unsigned char resr_volunb[2];
	unsigned char resr_ampunb[2];
} cfg_mplimit_t;

//�๦�ܲ��������
typedef struct {
	unsigned short mid;   //use for save
	cfg_mpbase_t base;
	cfg_mplimit_t limit;
} para_cenmetp_t;

//�����㵽��������ӳ��
#define METP_NONE		0
#define METP_METER		1
#define METP_PULSE		2
#define METP_USER		3

typedef struct {
	//unsigned char type;   //����, 0-��Ч, 1-���, 2-����
	//unsigned char reserv;
	unsigned char  metid;   //���-��Ӧ����,����ŵȵ�
} metp_map_t;

#ifndef DEFINE_PARAMETP
extern /*const*/ metp_map_t MetpMap[MAX_METP];
#endif

#define EMPTY_CENMETP(mid)		EMPTY_METER(mid)

#endif /*_PARAM_METP_H*/

