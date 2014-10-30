#ifndef _MC55GPRS_H_
#define _MC55GPRS_H_
#include "plat_include/sys_config.h"
#ifdef GPRS_MODULE_MC55I
#include "plat_include/plat_gprs.h"

/*
���з��صĴ�����붼�Ǹ�ֵ�����¶���Ĵ�����붼����ֵ����
����ʱǰ��Ӹ��ű�ɸ�ֵ�󷵻ء�
*/

#define  GPRS_BAUDRATE 115200

/*
˵��:GPRS/CDMAģ���ϵ��ʼ��
�������:
Mode Gprs���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int Mc55Init(GPRS_MODE Mode);
#define _GprsInit Mc55Init

/*
˵��:GPRS/CDMAģ��ػ�����
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int Mc55PowerOff(void);
#define _GprsPowerOff Mc55PowerOff

/**
* @brief ��GPRS/CDMAģ���ϵ�
* @param ��
*/
void Mc55PowerOn(void);
#define _GprsPowerOn Mc55PowerOn
/*
˵��:GPRS/CDMA���粦��
�������:
��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int Mc55DialOn(void);
#define _GprsDialOn Mc55DialOn
/*
˵��:�Ͽ�GPRS/CDMA��������
�������:
��
�������:
��
����ֵ:
��
*/
void Mc55DialOff(void);
#define _GprsDialOff Mc55DialOff
/*
˵��:��ȡ��ǰģ���ź�ǿ��
�������:
��
�������:
��
����ֵ:
�ɹ������ź�ǿ��ֵ��ʧ�� -ERRFAILED
*/
int Mc55GetSig(void);
#define _GprsGetSig Mc55GetSig
/*
˵��:����GPRSģ�� APN
�������:
apn APN������ָ��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int Mc55SetApn(const char *apn);
#define _GprsSetApn Mc55SetApn 
/*
˵��:���ö������ĺ���
�������:
no �������ĺ��뻺��ָ��,len ���ĺ��볤��
�������:
��
����ֵ:
��
*/
int  Mc55SetMsgCenterNo(const char *no,int len);
/*
˵��:ͨ��ģ�鷢��һ������Ϣ
�������:
������Ϣ������ָ��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int Mc55SendMessage(const unsigned char *msg,const char *no,int msglen);
#define _GprsSendMessage Mc55SendMessage
/*
˵��:����һ���µĶ���Ϣ
�������:
���ջ�����ָ��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int Mc55RecvMessage(unsigned char *msg,char *no,int msglen,int nolen);
#define _GprsRecvMessage	Mc55RecvMessage
/*
˵��:����(TCP/UDP)ģʽ
�������:
proto ,=SOCKET_TCP TCP, =SOCKET_UDP UDP
�������:
��
����ֵ:
����ֵ���ɹ�����ͨ����������ʧ�ܷ��� -ERRFAILED
*/
int Mc55Mode(int proto);
#define _GprsMode Mc55Mode
/**
* @brief ���ӵ�������
* @param ip ������IP
* @param port �������˿ں�
* @return �ɹ�0, ����ʧ��
*/
int Mc55Connect(unsigned long ip, unsigned short port);
#define _GprsConnect Mc55Connect

/**
* @brief ��������Ͽ�����
*/
void Mc55Disconnect(void);
#define _GprsDisconnect Mc55Disconnect



/*
˵��:������ͨ�Žӿڷ�������(TCP)
�������:
buf �������ݻ�����ָ�룬len �������ݳ���
�������:
��
����ֵ:
SUCCEED��ʧ�� -ERRFAILED
*/
int Mc55RawSend(const unsigned char *buf, int len);
#define _GprsRawSend Mc55RawSend
/*
˵��:������ͨ�Žӿڶ�ȡ����
�������:
	len �������ݻ��泤��
�������:
	buf �������ݻ�����ָ��
����ֵ:
�ɹ����ؽ������ݳ��ȣ�ʧ��0
*/
int Mc55GetChar(unsigned char *buf,int len);
#define _GprsGetChar Mc55GetChar
#endif
#endif
