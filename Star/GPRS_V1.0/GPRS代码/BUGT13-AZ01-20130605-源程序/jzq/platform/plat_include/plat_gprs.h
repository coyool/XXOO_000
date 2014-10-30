#ifndef __PLAT_GPRS_H__
#define __PLAT_GPRS_H__
#include "plat_include/plat_network.h"

//��·���ÿ���
typedef enum{
	GCMUX_OFF = 0,
	GCMUX_ON
}CMUX_TYPE;

//GPRSģ��״̬
typedef enum{
	GSTATE_BEGIN = 0,		// ��ʼ״̬
	GSTATE_DEVINITING,		//��ʼ��״̬
	GSTATE_DEVINITFAILED,	// +��ʼ��ʧ��
	GSTATE_DEVINITED,		//��ʼ�����
	GSTATE_CHECKSIG,		//��������ź�
	GSTATE_SIGFAILED,		//û�м�⵽�����ź�
	GSTATE_APNSETFAILED,	// +����APNʧ��
	GSTATE_APNSETED,			// +����APN�ɹ�
	GSTATE_NETREGFAILED,	// +����ע��ʧ��
	GSTATE_NETREGED,			// +����ע��ɹ�
	GSTATE_DIALFAILED,		//����ʧ��
	GSTATE_DIALING,			//������
	GSTATE_DIALED,			//���ųɹ�
}GPRS_STATE;

typedef enum{
	GSMODE_PPPD,//PPP��ʽ����
	GSMODE_ATCMD//AT���ʽ����
}GPRS_MODE;

/*
˵��:GPRS/CDMAģ���ϵ��ʼ��
�������:
    Mode Gprs���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsInit(GPRS_MODE Mode);


/*
˵��:GPRS/CDMAģ��ػ�����
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsPowerOff(void);



/**
* @brief ��GPRS/CDMAģ���ϵ�
* @param ��
*/
void GprsPowerOn(void);

/*
˵��:GPRS/CDMA���粦��
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsDialOn(void);

/*
˵��:�Ͽ�GPRS/CDMA��������
�������:
    ��
�������:
    ��
����ֵ:
    ��
*/
void GprsDialOff(void);

/*
˵��:��ȡ��ǰģ���ź�ǿ��
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ������ź�ǿ��ֵ��ʧ�� -ERRFAILED
*/
int GprsGetSig(void);

/*
˵��:����GPRSģ�� APN
�������:
    apn APN������ָ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsSetApn(const char *apn);

/*
˵��:ͨ��ģ�鷢��һ������Ϣ
�������:
    ������Ϣ������ָ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsSendMessage(const unsigned char *msg,const char *no,int msglen);

/*
˵��:����һ���µĶ���Ϣ
�������:
    ���ջ�����ָ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
	*/
int GprsRecvMessage(unsigned char *msg,char *no,int msglen,int nolen);

#if 0
/*
˵��:��ȡGSMģ���ͺ�
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ�����ģ�����ͱ��룬ʧ�� -ERRFAILED
*/
int GprsGetModuleType(void);
#endif
/*
˵��:��ȡGPRS/CDMAģ�鵱ǰ״̬
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ�����ģ�鵱ǰ״̬���룬ʧ�� -ERRFAILED
*/
int GprsGetModuleState(void);

/*
˵��:��ȡģ��RING�ŵ�ǰ״̬
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ�����ģ��״̬��0��1����ʧ�� -ERRFAILED
*/
int GprsGetRingState(void);

/*
˵��:����GPRS/CDMA���ŵ��û�������
�������:
    usr �û�������ָ��
    pwd ���뻺��ָ��
    usrlen �û�������
    pwdlen ���볤��
�������:
    ��
����ֵ:
    ��
*/
void GprsSetUserPwd(const char *usr,const char *pwd);

/*
˵��:���ö������ĺ���
�������:
    no �������ĺ��뻺��ָ��,len ���ĺ��볤��
�������:
    ��
����ֵ:
    ��
*/
int  GprsSetMsgCenterNo(const char *no,int len);

/*
˵��:���ö�·���ù���
�������:
    flag =GCMUX_OFF�رգ�=CMUX_ON����
*/
void GprsSetCmux(int flag);

/*
˵��:�õ�APN
����ֵ:
	apn
*/
const char* GprsGetApn(void);
/*
˵��:�õ�Gprs���ӷ�ʽ
����ֵ:
	���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
*/
int GprsGetMode(void);
/*
˵��:�õ�Gprs��Ip��ַ
����ֵ:
���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
*/
unsigned int GprsGetIP(void);
/**
* @brief �������״̬
* @return ���߷���1, ���߷���0
*/

int GprsLineState(void);
/*
    ���ص�ǰ�ź�״̬ 1 �ã�0 ����
*/
int GprsSigqualityF(void);
/**
* @brief ���TCP״̬
* @return ���߷���1, ���߷���0
*/

int GprsTcpState(void);
/*
˵��:ͨ��DNS����õ�IP��ַ
�������:
	web_addr WWW��ַ
���
	ip ip��ַ
����
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsDns(const char* web_addr,varip_t* ip);
#endif
