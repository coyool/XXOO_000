/**
* svrcomm.h -- ������ͨ��
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-19
* ����޸�ʱ��: 2009-5-19
*/

#ifndef _SVRCOMM_H
#define _SVRCOMM_H
#include "app_include/uplink/uplink_pkt.h"

void SvrCommPeekEvent(unsigned long waitmask, unsigned long *pevent);
int SvrNoteProc(unsigned char itf);

int SvrMessageProc(unsigned char itf);
int SvrMessageDlms(unsigned char itf);
int SvrEchoSend(unsigned char itf, uplink_pkt_t *pkt);
void SvrEchoNoData(unsigned char itf, uplink_pkt_t *pkt);
int IsSvrMsgAfn(unsigned char afn);

#define LINESTAT_OFF    0
#define LINESTAT_ON    1
extern int SvrCommLineState;

extern unsigned char SvrCommInterface;

int SvrCommHaveTask(void);

void SvrSendFileInfo(unsigned char itf);

/**
* @brief ��������ͨ���¼�����
* @rtn	-1,û�������,���������������
* @param pevent �����¼�����ָ��
*/
int SvrCommPeekNoteid(void);

__inline void SendSmsMessage(const char* msg);
__inline void SendUserSmsMessage(const char* msg);
__inline char* GetSmsMessage(void);
__inline char* GetUserSmsMessage(void);
__inline void CallCsq(void);
void SvrConnectNote(void);
void SvrCommRemoveNoteid(int id);
void SvrAlermNote(void);
#endif /*_SVRCOMM_H*/

