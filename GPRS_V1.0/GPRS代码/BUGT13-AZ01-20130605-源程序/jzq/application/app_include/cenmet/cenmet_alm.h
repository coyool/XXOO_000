/**
* cenmet_alm.h -- ��Ƹ澯���
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-6-14
* ����޸�ʱ��: 2009-6-14
*/

#ifndef _CENMET_ALARM_H
#define _CENMET_ALARM_H

void CMetAlm1MinProc(void);
void CMetAlmRdmetProc(void);
void CMetAlm15MinProc(void);
void ClearCMetAlarm(void);
void ReadStatus(unsigned char *status, unsigned short mid);
void GetIMetMdbCurrent(unsigned char *pbuf);

#endif /*_CENMET_ALARM_H*/

