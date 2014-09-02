/**
* keepalive.h -- �����������������
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-19
* ����޸�ʱ��: 2009-5-19
*/

#ifndef _SVRCOMM_KEEPALIVE_H
#define _SVRCOMM_KEEPALIVE_H

#define KEEPALIVE_FLAG_LOGONFAIL    0
#define KEEPALIVE_FLAG_LOGONOK    1

int KeepAliveInPeriod(void);
void ClearKeepAlive(void);
int KeepAliveProc(void);
int RefreshKeepAlive(void);
void SetKeepAlive(unsigned char flag);
int CheckinActivePeriod(void);

#endif /*_SVRCOMM_KEEPALIVE_H*/

