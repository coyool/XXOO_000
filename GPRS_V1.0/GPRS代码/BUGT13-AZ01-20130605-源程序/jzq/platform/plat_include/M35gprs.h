#ifndef _M35GPRS_H_
#define _M35GPRS_H_

#include "plat_include/sys_config.h"
#ifdef GPRS_MODULE_M35
#include "plat_include/plat_gprs.h"
#include "plat_include/plat_network.h"
/*
���з��صĴ�����붼�Ǹ�ֵ�����¶���Ĵ�����붼����ֵ����
����ʱǰ��Ӹ��ű�ɸ�ֵ�󷵻ء�
*/
#define  GPRS_BAUDRATE 19200


/*
˵��:GPRS/CDMAģ���ϵ��ʼ��
�������:
Mode Gprs���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35Init(GPRS_MODE Mode);
#define _GprsInit M35Init

/*
˵��:GPRS/CDMAģ��ػ�����
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35PowerOff(void);
#define _GprsPowerOff M35PowerOff

/**
* @brief ��GPRS/CDMAģ���ϵ�
* @param ��
*/
void M35PowerOn(void);
#define _GprsPowerOn M35PowerOn
/*
˵��:GPRS/CDMA���粦��
�������:
��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35DialOn(void);
#define _GprsDialOn M35DialOn
/*
˵��:�Ͽ�GPRS/CDMA��������
�������:
��
�������:
��
����ֵ:
��
*/
void M35DialOff(void);
#define _GprsDialOff M35DialOff
/*
˵��:��ȡ��ǰģ���ź�ǿ��
�������:
��
�������:
��
����ֵ:
�ɹ������ź�ǿ��ֵ��ʧ�� -ERRFAILED
*/
int M35GetSig(void);
#define _GprsGetSig M35GetSig
/*
˵��:����GPRSģ�� APN
�������:
apn APN������ָ��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35SetApn(const char *apn);
#define _GprsSetApn M35SetApn 

/*
˵��:˵��:GPRS/CDMA����״̬
�������:
��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35LineState(void);
#define _GprsLineState M35LineState 

/*
˵��:˵��:TCP����״̬
�������:
��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35TCPState(void);
#define _GprsTCPState M35TCPState 
/*
˵��:���ö������ĺ���
�������:
no �������ĺ��뻺��ָ��,len ���ĺ��볤��
�������:
��
����ֵ:
��
*/
int  M35SetMsgCenterNo(const char *no,int len);
/*
˵��:ͨ��ģ�鷢��һ������Ϣ
�������:
������Ϣ������ָ��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35SendMessage(const unsigned char *msg,const char *no,int msglen);
#define _GprsSendMessage M35SendMessage
/*
˵��:����һ���µĶ���Ϣ
�������:
���ջ�����ָ��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35RecvMessage(unsigned char *msg,char *no,int msglen,int nolen);
#define _GprsRecvMessage	M35RecvMessage
/*
˵��:����(TCP/UDP)ģʽ
�������:
proto ,=SOCKET_TCP TCP, =SOCKET_UDP UDP
�������:
��
����ֵ:
����ֵ���ɹ�����ͨ����������ʧ�ܷ��� -ERRFAILED
*/
int M35Mode(int proto);
#define _GprsMode M35Mode
/**
* @brief ���ӵ�������
* @param ip ������IP
* @param port �������˿ں�
* @return �ɹ�0, ����ʧ��
*/
int M35Connect(unsigned long ip, unsigned short port);
#define _GprsConnect M35Connect

/**
* @brief ��������Ͽ�����
*/
void M35Disconnect(void);
#define _GprsDisconnect M35Disconnect



/*
˵��:������ͨ�Žӿڷ�������(TCP)
�������:
buf �������ݻ�����ָ�룬len �������ݳ���
�������:
��
����ֵ:
SUCCEED��ʧ�� -ERRFAILED
*/
int M35RawSend(const unsigned char *buf, int len);
#define _GprsRawSend M35RawSend
/*
˵��:������ͨ�Žӿڶ�ȡ����
�������:
	len �������ݻ��泤��
�������:
	buf �������ݻ�����ָ��
����ֵ:
�ɹ����ؽ������ݳ��ȣ�ʧ��0
*/
int M35GetChar(unsigned char *buf,int len);
#define _GprsGetChar M35GetChar

/*
˵��:ͨ��DNS����õ�IP��ַ
�������:
	web_addr WWW��ַ
���
	ip ip��ַ
����
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35Dns(const char* web_addr,varip_t* ip);
#define _GprsDns M35Dns

#endif

#endif
