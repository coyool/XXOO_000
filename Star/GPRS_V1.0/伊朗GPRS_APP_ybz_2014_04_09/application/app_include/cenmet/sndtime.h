/**
* sndtime.h -- ����������ʱ�䴦��
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-27
* ����޸�ʱ��: 2009-5-27
*/

#ifndef _MDB_SENDTIME_H
#define _MDB_SENDTIME_H

unsigned int GetSndTime(const unsigned char *pnfn);
void ClearSndTime(void);
void SetSndTime(const unsigned char *pnfn, unsigned int sndtime, unsigned char step);
void SaveSndTime(void);
void SaveSndTimeFile(void);

void RemoveDataTaskTime(int taskid);
void InitDataTaskTime(int taskid);

#endif /*_MDB_SENDTIME_H*/

