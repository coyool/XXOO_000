/**
* foward.h -- ���͸��ת��
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-17
* ����޸�ʱ��: 2009-5-17
*/

#ifndef _CENMET_FORWARD_H
#define _CENMET_FORWARD_H

struct data_t
{
	unsigned int len;
	unsigned char* data;
};

void CenMetForward(const struct data_t *pcmd, struct data_t *pecho);
void PlForwardF1(const struct data_t *pcmd, struct data_t *pecho,unsigned char poto);
//��ʼ�������߳�
void InitCenmet(void);
//����ʣ����
void ReadMoney(void);
//��ֵ
void DoRecharge(unsigned char*);
#endif /*_CENMET_FORWARD_H*/

