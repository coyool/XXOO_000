#ifndef __STORE_IAP_H
#define __STORE_IAP_H



//�������
int store_iap(const void *pbuf,unsigned short len);
//����������ʼ
void store_iap_start(void);
//����
void store_iap_end(void);
/*
��������ʱ�����ȵ���iap_start,Ȼ���յ��Ĵ���֡��˳�����save_code,������iap_end

*/



#endif

