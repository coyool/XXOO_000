#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "plat_include/debug.h"
#include "plat_include/plat_defines.h"
#include "plat_include/plat_gprs.h"
#include "plat_include/m35gprs.h"
#include "plat_include/mc55gprs.h"
/*
˵��:GPRS/CDMAģ���ϵ��ʼ��
�������:
    Mode Gprs���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsInit(GPRS_MODE Mode)
{
	return _GprsInit(Mode);
}
/*
˵��:GPRS/CDMAģ��ػ�����
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsPowerOff(void)
{
	_GprsPowerOff();
	return SUCCEED;
}
/**
* @brief ��GPRS/CDMAģ���ϵ�
* @param ��
*/
void GprsPowerOn(void)
{
	_GprsPowerOn();
}

/*
˵��:GPRS/CDMA���粦��
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsDialOn(void)
{
	return _GprsDialOn();
}

/*
˵��:�Ͽ�GPRS/CDMA��������
�������:
    ��
�������:
    ��
����ֵ:
    ��
*/
void GprsDialOff(void)
{
	_GprsDialOff();
}

/*
˵��:��ȡ��ǰģ���ź�ǿ��
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ������ź�ǿ��ֵ��ʧ�� -ERRFAILED
*/
int GprsGetSig(void)
{
	return _GprsGetSig();
}

/*
˵��:����GPRSģ�� APN
�������:
    apn APN������ָ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsSetApn(const char *apn)
{
	return _GprsSetApn(apn);
}

/*
˵��:ͨ��ģ�鷢��һ������Ϣ
�������:
    ������Ϣ������ָ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsSendMessage(const unsigned char *msg,const char *no,int msglen)
{
	return _GprsSendMessage(msg,no,msglen);
}

/*
˵��:����һ���µĶ���Ϣ
�������:
    ���ջ�����ָ��
�������:
    ��
����ֵ:
    �ɹ� SUCCEED��ʧ�� -ERRFAILED
	*/
unsigned char debug_sms[220];
unsigned char debug_sms_len = 0;
unsigned char debug_sms_rv = 0;
int GprsRecvMessage(unsigned char *msg,char *no,int msglen,int nolen)
{
	if(debug_sms_rv == 1)
	{
		debug_sms_rv = 0;
		if(msglen<debug_sms_len)
		{
			PrintLog(LOGTYPE_DEBUG,"����̫С��\r\n");
			return -ERRFAILED;
		}
		memcpy(msg,debug_sms,debug_sms_len);
		strcpy(no,"13714325180");
		return SUCCEED;
	}
	return _GprsRecvMessage(msg,no,msglen,nolen);
}

/*
˵��:��ȡGPRS/CDMAģ�鵱ǰ״̬
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ�����ģ�鵱ǰ״̬���룬ʧ�� -ERRFAILED
*/
int GprsGetModuleState(void)
{
	return -ERRFAILED;
}

/*
˵��:��ȡģ��RING�ŵ�ǰ״̬
�������:
    ��
�������:
    ��
����ֵ:
    �ɹ�����ģ��״̬��0��1����ʧ�� -ERRFAILED
*/
int GprsGetRingState(void)
{
	return -ERRFAILED;	
}
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
void GprsSetUserPwd(const char *usr,const char *pwd)
{
}

/*
˵��:���ö������ĺ���
�������:
    no �������ĺ��뻺��ָ��,len ���ĺ��볤��
�������:
    ��
����ֵ:
    ��
*/
int  GprsSetMsgCenterNo(const char *no,int len)
{
	return -ERRFAILED;
}
/*
˵��:���ö�·���ù���
�������:
    flag =GCMUX_OFF�رգ�=CMUX_ON����
*/
void GprsSetCmux(int flag)
{
	(flag=flag);
}

/*
˵��:�õ�APN
����ֵ:
	apn
*/
const char* GprsGetApn(void)
{
	return NULL;
}
/*
˵��:�õ�Gprs���ӷ�ʽ
����ֵ:
	���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
*/
int GprsGetMode(void)
{
	return GSMODE_ATCMD;
}
/*
˵��:�õ�Gprs��Ip��ַ
����ֵ:
���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
*/
unsigned int GprsGetIP(void)
{
	return 0;
}
/*
    ���ص�ǰ�ź�״̬ 1 �ã�0 ����
*/
int GprsSigqualityF(void)
{
	return 0;
}
/**
* @brief �������״̬
* @return ���߷���1, ���߷���0
*/

int GprsLineState(void)
{
	return _GprsLineState();
}
/**
* @brief ���TCP״̬
* @return ���߷���1, ���߷���0
*/

int GprsTcpState(void)
{
	return _GprsTCPState();
}
/*
˵��:ͨ��DNS����õ�IP��ַ
�������:
	web_addr WWW��ַ
���
	ip ip��ַ
����
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int GprsDns(const char* web_addr,varip_t* ip)
{
	return _GprsDns(web_addr,ip);
}
