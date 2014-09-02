/**
* curvemon.h -- �¶�������ʷ���ݽṹ
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-12
* ����޸�ʱ��: 2009-5-12
*/

#ifndef _CURVE_MON_H
#define _CURVE_MON_H

#include "app_include/cenmet/mdbconf.h"

typedef struct {
	unsigned char rdtime[5];    //����ʱ��, ��ʱ������
	unsigned char fenum;  //������

	//F17
	unsigned char enepa[5*MAXNUM_METPRD];  //�����й�����, 0~4, 0.0001kWh
	unsigned char enepi[4*MAXNUM_METPRD];  //�����޹�����, 0~4, 0.01kvarh
	unsigned char enepi1[4*MAXNUM_METPRD];  //�����޹�һ���޵���, 0~4, 0.01kvarh
	unsigned char enepi4[4*MAXNUM_METPRD];  //�����޹������޵���, 0~4, 0.01kvarh
	//F18
	unsigned char enena[5*MAXNUM_METPRD];  //�����й�����, 0~4, 0.0001kWh
	unsigned char eneni[4*MAXNUM_METPRD];  //�����޹�����, 0~4, 0.01kvarh
	unsigned char eneni2[4*MAXNUM_METPRD];  //�����޹������޵���, 0~4, 0.01kvarh
	unsigned char eneni3[4*MAXNUM_METPRD];  //�����޹������޵���, 0~4, 0.01kvarh
	//F19
	unsigned char dmnpa[3*MAXNUM_METPRD];  //���������й��������,0~4,0.0001kW
	unsigned char dmntpa[4*MAXNUM_METPRD];  //���������й������������ʱ��,0~4,��ʱ����
	unsigned char dmnpi[3*MAXNUM_METPRD];  //���������޹��������,0~4,0.0001kvar
	unsigned char dmntpi[4*MAXNUM_METPRD];  //���������޹������������ʱ��,0~4,��ʱ����
	//F20
	unsigned char dmnna[3*MAXNUM_METPRD];  //���·����й��������,0~4,0.0001kW
	unsigned char dmntna[4*MAXNUM_METPRD];  //���·����й������������ʱ��,0~4,��ʱ����
	unsigned char dmnni[3*MAXNUM_METPRD];  //���·����޹��������,0~4,0.0001kvar
	unsigned char dmntni[4*MAXNUM_METPRD];  //���·����޹������������ʱ��,0~4,��ʱ����
	//F21
	//unsigned char enepa_day[4*MAXNUM_METPRD];     //���������й�������, 0.0001kWh
	//F22
	//unsigned char enepi_day[4*MAXNUM_METPRD];     //���������޹�������, 0.0001kvarh
	//F23
	//unsigned char enena_day[4*MAXNUM_METPRD];     //���·����й�������, 0.0001kWh
	//F24
	//unsigned char eneni_day[4*MAXNUM_METPRD];     //���·����޹�������, 0.0001kvarh
	//F157
	unsigned char enepa_abc[5*3];	//ABC�������й�����ʾֵ, 0.0001kWh
	//F158
	unsigned char enepi_abc[4*3];	//ABC�������޹�����ʾֵ, 0.01kvarh
	//F159
	unsigned char enena_abc[5*3];	//ABC�෴���й�����ʾֵ, 0.0001kWh
	//F160
	unsigned char eneni_abc[4*3];	//ABC�������޹�����ʾֵ, 0.01kvarh
} db_metmon_t;

//Ԥ������Ϣ�¶���
typedef struct{
	//F215  ���õ���Ϣ
	unsigned char readtimebuy[5];  //��ʱ������
	unsigned char buyene_cnt[2];  //������� 
	unsigned char money_res[5];  //ʣ���� 
	unsigned char money_buyed[5];  //�ۼƹ�����
	unsigned char ene_res[4];  //ʣ�����
	unsigned char ene_overdraft[4];  //͸֧����
	unsigned char ene_buyed[4];  //�ۼƹ����� 
	unsigned char ene_ticklmt[4];  //��Ƿ���޵���
	unsigned char ene_alarm[4];  //��������
	unsigned char ene_err_buy[4];  //���ϵ���

    //F216 ������Ϣ       
	unsigned char readtimepre[5];
	unsigned char fenumbanla;
	unsigned char ene_febuyed[MAXNUM_METPRD*5];  //�ѽ��й�����
	unsigned char ene_feunbuy[MAXNUM_METPRD*5];  //δ���й�����
	unsigned char ene_err_banla[5];    //���ϵ���
	
}db_metmonpre_t;

typedef struct {
	//F51
	unsigned char pwr_time[2];    //�ն��¹���ʱ��, min
	unsigned char rest_cnt[2];    //�ն��¸�λ����
	//F52
	unsigned char sw_time[4];	 //��բ�ۼƴ���
	//F54
	unsigned char comm_bytes[4];  //�ն�����վ��ͨ������, �ֽ�
} db_termmon_t;

#endif /*_CURVE_MON_H*/

