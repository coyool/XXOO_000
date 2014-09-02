/**
* cenmet_alm.h -- 表计告警监测
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-6-14
* 最后修改时间: 2009-6-14
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

