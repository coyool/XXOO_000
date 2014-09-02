#include <string.h>
#include <stdio.h>

#include "plat_include/sys_config.h"
#include "plat_include/Plat_defines.h"
#include "plat_include/Plat_uart.h"
#include "plat_include/M35gprs.h"
#include "plat_include/GprsCtrl.h"
#include "plat_include/plat_network.h"
#include "plat_include/sleep.h"
#include "plat_include/shellcmd.h"
#include "plat_include/debug.h"
#include "app_include/param/unique.h"
#include "app_include/gprs/gprs_dev.h"

#ifdef GPRS_MODULE_M35
static int InitTeTaBps(void);//�̶�BPS
static unsigned char _atsend_cache[50];//AT����ͻ���
static unsigned char _atcmd_cache[220];//AT������ܻ���
static char _apn[32] = {0}; //apn
static char cdma_passwd[32]={0};//�û���
static char cdma_user[32] = {0};//����



enum ip_status_t
{
	IPSTATE_IP_INITIAL,
	IPSTATE_IP_START,
	IPSTATE_IP_CONFIG,
	IPSTATE_IP_IND,
	IPSTATE_IP_GPRSACT,
	IPSTATE_IP_STATUS,
	IPSTATE_TCP_CONNECTING,
	IPSTATE_UDP_CONNECTING,
	IPSTATE_IP_CLOSE,
	IPSTATE_CONNECT_OK,
	IPSTATE_PDP_DEACT,
	IPSTATE_ERROR = -1,
};
enum ip_status_t GetGprsStat(void);

#define INTERVAL	5
#define INTERVAL_LONG	200
#define TIMEOUT 200
#define TIMEOUT_LONG 2000
#define CH_REMOTE	0
#define CH_CLIENT	1

enum cmdindex{
	/*0*/	INDEX_TEST_RATE		,
	/*1*/	INDEX_TEST_ATE0		,
	/*2*/	INDEX_ATE0			,
	/*3*/	INDEX_TEST_CIPMUX1	,
	/*4*/	INDEX_CIPMUX1		,
	/*5*/	INDEX_TEST_CIPMODE0 ,
	/*6*/	INDEX_CIPMODE0 		,
	/*7*/	INDEX_CIPSHUT		,
	/*8*/	INDEX_TEST_CPIN		,
	/*9*/	INDEX_TEST_CSQ		,
	/*10*/	INDEX_TEST_CREG		,
	/*11*/	INDEX_TEST_CGATT	,
	/*12*/	INDEX_CGATT1		,
	/*13*/	INDEX_TEST_QISTAT,
	/*14*/	INDEX_CIICR			,
	/*15*/	INDEX_TEST_CIFSR	,
	/*16*/	INDEX_CSTT			,
	/*17*/	INDEX_TEST_CIPSPRT2	,
	/*18*/	INDEX_CIPSPRT2		,
	/*19*/	INDEX_TEST_CSTT		,
	/*20*/	INDEX_TEST_CIPQSEND1,
	/*21*/	INDEX_CIPQSEND1,
	/*22*/	INDEX_CIPSERVER0,
	/*23*/	INDEX_QINDI,
	/*24*/	INDEX_QIHEAD,
	/*25*/  INDEX_QICLOSE,
	/*26*/  INDEX_QIDEACT,
	/*27*/  INDEX_TEST_CSCA,
	/*28*/  INDEX_CSCA,
	/*29*/  INDEX_CMGF,
	/*30*/  INDEX_CNMI,
	/*31*/  INDEX_QMGDA,
	/*32*/  INDEX_CMGR,
	/*33*/  INDEX_QIRD,

};

static const unsigned char resp_ok[] = "\r\nOK\r\n";


static const struct atlist_struct atlist[] = {
	/*0*/	{"AT+IPR?\r\n", 2, {"\r\n+IPR: ","AT+IPR?", NULL, NULL}},
	/*1*/	{"\r\nAT\r\n", 1, {"\r\nOK\r\n", NULL, NULL, NULL}},
	/*2*/	{"ATE0&W\r\n",2,{"ATE0&W\r\r\nOK\r\n",resp_ok,NULL,NULL}},
	/*3*/	{"AT+CIPMUX?\r\n",1,{"\r\n+CIPMUX: 1",NULL,NULL,NULL}},
	/*4*/	{"AT+CIPMUX=1\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*5*/	{"AT+CIPMODE?\r\n",1,{"\r\n+CIPMODE: 0",NULL,NULL,NULL}},
	/*6*/	{"AT+CIPMODE=0\r\n", 1, {resp_ok, NULL, NULL, NULL}},
	/*7*/	{"AT+CIPSHUT\r\n", 1, {"\r\nSHUT OK\r\n", NULL, NULL, NULL}},
	/*8*/	{"AT+CPIN?\r\n", 1, {"\r\n+CPIN: READY",NULL, NULL, NULL}},
	/*9*/	{"AT+CSQ\r\n", 1, {"\r\n+CSQ: ",NULL,NULL,NULL}},
	/*10*/	{"AT+CREG?\r\n", 2, {"\r\n+CREG: 0,1","\r\n+CREG: 0,5",NULL,NULL}},
	/*11*/	{"AT+CGATT?\r\n",1,{"\r\n+CGATT: 1",NULL,NULL,NULL}},
	/*12*/	{"AT+CGATT=1\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*13*/	{"AT+QISTAT\r\n",1,{"\r\nOK\r\n\r\nSTATE: ",NULL,NULL,NULL}},
	/*14*/	{"AT+CIICR\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*15*/	{"AT+CIFSR\r\n",1,{"\r\n",NULL,NULL,NULL}},
	/*16*/	{"AT+CSTT\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*17*/	{"AT+CIPSPRT?\r\n",1,{"\r\n+CIPSPRT: 1\r\n",NULL,NULL,NULL}},
	/*18*/	{"AT+CIPSPRT=1\r\n",1,{"\r\nOK\r\n",NULL,NULL,NULL}},
	/*19*/  {"AT+CSTT?\r\n",1,{"\r\n+CSTT: \"",NULL,NULL,NULL}},
	/*20*/  {"AT+CIPQSEND?\r\n",1,{"\r\n+CIPQSEND: 1",NULL,NULL,NULL}},
	/*21*/  {"AT+CIPQSEND=1\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*22*/	{"AT+CIPSERVER=0\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*23*/	{"AT+QINDI=1\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*24*/	{"AT+QIHEAD=1\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*25*/	{"AT+QICLOSE\r\n",1,{"\r\nCLOSE OK\r\n",NULL,NULL,NULL}},
	/*26*/	{"AT+QIDEACT\r\n",1,{"\r\nDEACT OK\r\n",NULL,NULL,NULL}},
	/*27*/	{"AT+CSCA?\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*28*/	{"AT+CSCA=\"%s\"\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*29*/	{"AT+CMGF=1\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*30*/	{"AT+CNMI=2,1,0,0,0\r\n",1,{resp_ok,NULL,NULL,NULL}},
 	/*31*/	{"AT+QMGDA=\"DEL ALL\"\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*32*/	{"AT+CMGR=1\r\n",1,{"\r\n+CMGR:",NULL,NULL,NULL}},
	/*33*/	{"AT+QIRD=0,1,0,1024\r\n",1,{"\r\n+CMGR:",NULL,NULL,NULL}},
	//{"AT+CMGF=0\r\n", 1, {mc37_atecho_OK, NULL, NULL, NULL}},
};

/*
˵��:GPRS/CDMAģ���ϵ��ʼ��
�������:
Mode Gprs���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35Init(GPRS_MODE Mode)
{
	gprs_switch_init();
	UartOpen(GPRS_PORT);
	UartSetPara(GPRS_PORT,GPRS_BAUDRATE,8, 1,'N');
	M35PowerOff();
	TickSleep(240);
    M35PowerOn();
	TickSleep(120);

	return SUCCEED;
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
int M35PowerOff(void)
{
	gprsline_pwroff();
	return SUCCEED;
}
/**
* @brief ��GPRS/CDMAģ���ϵ�
* @param ��
*/
void M35PowerOn(void)
{
	gprsline_pwron();
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
int M35DialOn(void)
{
	static int delete_sms = 0;
	int timeout = 50;
	
	if(-ERRFAILED == InitTeTaBps())
	{
		PrintLog(LOGTYPE_DEBUG,"��ʼ��TE������ʧ��\r\n");
		return -1;
	}
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)));//����յ�һ����֡�����л���
	
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
			return -ERRFAILED;
	
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_CPIN],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//����sim��
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QINDI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���û���������
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QIHEAD],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//����Gprs����ͷ
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_CMGF],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���ö�Ϣ�ı�ģʽ
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_CNMI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���ö�Ϣ����ģʽ(��֪ͨ)
		return -ERRFAILED;
   	timeout = 50;
	while(1)
	{
		if(M35GetSig() > 1)	//�����ź�
			break;
	 	if (timeout-- == 0)
		{
			return -ERRFAILED;
		}
		TickSleep(50);
	 }
#ifdef METER_MODULE	
	Signel_led(Signel_Y,1);	
#endif
	timeout = 50;
	while(1)
	{
		if(-ERRFAILED != SendAtCmd(&atlist[INDEX_TEST_CREG],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//������
			break;
		if (timeout-- == 0)
		{
			return -ERRFAILED;
		}
		TickSleep(50);
	}
	{
		struct atlist_struct plist;
		const char* cont = "AT+CGDCONT=1,\"IP\",\"%s\"\r\n";
		
		sprintf((char*)_atsend_cache,cont,_apn);
		PrintLog(LOGTYPE_DEBUG,"Apn:%s,Cmd:%s\r\n",_apn,(char*)_atsend_cache);
		plist.cmd = _atsend_cache;
		plist.keynum = 1;
		plist.key[0] = resp_ok;
		if(-ERRFAILED == SendAtCmd(&plist,INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))	 //����PDP,APN
		{
			return -ERRFAILED;
		}
	}
	timeout = 10;
	while(1)
	{
   		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_CGATT1],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���ø���GPRS��
			break;
	 	TickSleep(50);
		if(timeout-- == 0)
			break;
	}
	if((delete_sms%3) == 0)
	{
		delete_sms++;		
		if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QMGDA],INTERVAL,TIMEOUT*4,_atcmd_cache,sizeof(_atcmd_cache)))//ɾ��ȫ���Ķ���
			return -ERRFAILED;
	}
	timeout = 50;
	while(1)
	{
		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_TEST_CGATT],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//��ѯ�����Ƿ�ɹ�
			return SUCCEED;
		TickSleep(50);
	   	M35GetSig();
		TickSleep(50);
		if (timeout-- == 0)
		{
			return -ERRFAILED;
		}
	}
}
/*
	-1	AT ����ͨѶʧ��
	-2	AT ����ͨѶʧ��
	-3	Sim��ʧ��
	-4	AT ����ͨѶʧ��
	-5	AT ����ͨѶʧ��
	-6	AT ����ͨѶʧ��
	-7	AT ����ͨѶʧ��
	-8	�ź�ǿ����
	-9	Ѱ������ʧ��
	-10	AT ����ͨѶʧ��
	-11 ɾ������ʧ��
	-12	���總��ʧ��	
*/
int M35DialOnTest(void)
{
	static int delete_sms = 0;
	int timeout = 50;
	M35Init(GSMODE_ATCMD);

	if(-ERRFAILED == InitTeTaBps())
	{
		PrintLog(LOGTYPE_DEBUG,"��ʼ��TE������ʧ��\r\n");
		return -1;
	}
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)));//����յ�һ����֡�����л���
	
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
			return -2;
	
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_CPIN],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//����sim��
		return -3;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QINDI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���û���������
		return -4;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QIHEAD],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//����Gprs����ͷ
		return -5;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_CMGF],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���ö�Ϣ�ı�ģʽ
		return -6;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_CNMI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���ö�Ϣ����ģʽ(��֪ͨ)
		return -7;
   	timeout = 50;
	while(1)
	{
		if(M35GetSig() > 5)	//�����ź�
			break;
	 	if (timeout-- == 0)
		{
			return -8;
		}
		TickSleep(50);
	 }
#ifdef METER_MODULE	
	Signel_led(Signel_Y,1);	
#endif
	timeout = 50;
	while(1)
	{
		if(-ERRFAILED != SendAtCmd(&atlist[INDEX_TEST_CREG],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//������
			break;
		if (timeout-- == 0)
		{
			return -9;
		}
		TickSleep(50);
	}
	{
		struct atlist_struct plist;
		const char* cont = "AT+CGDCONT=1,\"IP\",\"%s\"\r\n";
		PrintLog(LOGTYPE_DEBUG,"APN:%s\r\n",_apn);
		sprintf((char*)_atsend_cache,cont,_apn);
		plist.cmd = _atsend_cache;
		plist.keynum = 1;
		plist.key[0] = resp_ok;
		if(-ERRFAILED == SendAtCmd(&plist,INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))	 //����PDP,APN
		{
			return -10;
		}

	}
	timeout = 10;
	while(1)
	{
   		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_CGATT1],INTERVAL*4,TIMEOUT_LONG*4,_atcmd_cache,sizeof(_atcmd_cache)))//���ø���GPRS��
			break;
	 	TickSleep(50);
		if(timeout-- == 0)
			break;
	}
	if((delete_sms%3) == 0)
	{
		delete_sms++;		
		if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QMGDA],INTERVAL*4,TIMEOUT_LONG*10,_atcmd_cache,sizeof(_atcmd_cache)))//ɾ��ȫ���Ķ���
			return -11;
	}
	timeout = 50;
	while(1)
	{
		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_TEST_CGATT],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//��ѯ�����Ƿ�ɹ�
			return SUCCEED;
		TickSleep(50);
	   	M35GetSig();
		TickSleep(50);
		if (timeout-- == 0)
		{
			return -12;
		}
	}
}
/*
˵��:GPRS/CDMA����״̬
�������:
��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35LineState(void)
{
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_CPIN],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//����sim��
		return -ERRFAILED;
	if(-ERRFAILED == SendAtCmd(&atlist[INDEX_TEST_CREG],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//������
		return -ERRFAILED;
	return SUCCEED;	
}
/**
* @brief ���TCP����״̬
* @return ���߷���1, ���߷���0
*/

int M35TCPState(void)
{
	enum ip_status_t stat=GetGprsStat();
	if(stat == IPSTATE_CONNECT_OK)
		return 1;
	else
		return 0;
}
/*
˵��:�Ͽ�GPRS/CDMA��������,�ָ�ģ�鵽��ʼ״̬
�������:
��
�������:
��
����ֵ:
��
*/
void M35DialOff(void)
{
	if(-ERRFAILED == SendAtCmd(&atlist[INDEX_QIDEACT],INTERVAL,TIMEOUT*32,_atcmd_cache,sizeof(_atcmd_cache)))
		PrintLog(LOGTYPE_DEBUG,"QIDEACT Timeout!\r\n");
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
int M35GetSig(void)
{
	unsigned int a;
	if(-ERRFAILED == SendAtCmd(&atlist[INDEX_TEST_CSQ],INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		PrintLog(LOGTYPE_DEBUG,"TEST_CSQ Timeout!\r\n");
		return 0;//�����ź�ǿ��
	}
	a = char_to_hex(_atcmd_cache[8]);//��8�ֽڿ�ʼ��rssi
	if(_atcmd_cache[9]!=','){	//����һλ
		a *= 10;
		a += char_to_hex(_atcmd_cache[9]);
	}
	PrintLog(LOGTYPE_DEBUG,"CSQ:%d\r\n",a);
	if(a!=99)
		return a;
	else
		return 0;
}
/*
static int _SetApn(void)
{
	struct atlist_struct plist;
	unsigned char cmd[40];
	if(strlen((const char *)_apn)>16)
		return -1;
	strcpy((char *)cmd,"AT+QICSGP=1,\"");
	strcat((char *)cmd,(char *)_apn);
	strcat((char *)cmd,"\"\r\n");	
	plist.cmd = cmd;
	plist.keynum = 1;
	plist.key[0] = resp_ok;
	if(-ERRFAILED == SendAtCmd(&plist,INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		PrintLog(LOGTYPE_DEBUG,"SetApn Timeout!\r\n");
		return -ERRFAILED;
	}
	else
		return SUCCEED;
} */
/*
˵��:����GPRSģ�� APN
�������:
apn APN������ָ��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int M35SetApn(const char *apn)
{
	memset(_apn,0,sizeof(_apn));
	strncpy(_apn,apn,sizeof(_apn));
	return SUCCEED;
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
void M35SetUserPwd(const char *usr,const char *pwd)
{
	memset(cdma_user,0,sizeof(cdma_user));
	memset(cdma_passwd,0,sizeof(cdma_passwd));

	strncpy(cdma_user,usr,sizeof(cdma_user));
	strncpy(cdma_passwd,pwd,sizeof(cdma_passwd));
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
int  M35SetMsgCenterNo(const char *no,int len)
{
	const char* pno = NULL;
	struct atlist_struct plist;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_CSCA],INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		return -ERRFAILED;
	}
	pno = strchr((char*)_atcmd_cache,'"');
	if (pno == NULL)
	{
		return -ERRFAILED;
	}
	if(memcmp(pno+1,no,len)==0)
		return SUCCEED;

	sprintf((char*)_atcmd_cache,(char*)atlist[INDEX_CSCA].cmd,no);

	plist.cmd = _atcmd_cache;
	plist.keynum = atlist[INDEX_CSCA].keynum;

	plist.key[0] = atlist[INDEX_CSCA].key[0];
	if(-ERRFAILED==SendAtCmd(&plist,INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		return -ERRFAILED;
	}
	return SUCCEED;
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
static const char* StopSms="\x1A\x1B\r\n";
int M35SendMessage(const unsigned char *msg,const char *no,int msglen)
{
	int timeout = 10;
	struct atlist_struct plist;
	unsigned char* pCmd = _atcmd_cache;
	if(strlen(no)==0)
		return 	-ERRFAILED;
	sprintf((char*)_atsend_cache,"AT+CMGS=\"%s\"\r\n",no);
	plist.cmd = _atsend_cache;
	plist.keynum = 1;
	plist.key[0] = "\r\n> ";
	if(-ERRFAILED==SendAtCmd(&plist,INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		return -ERRFAILED;
	}
	UartSendData(GPRS_PORT,msg,msglen);
	UartSendData(GPRS_PORT,(unsigned char*)StopSms,strlen(StopSms));
	memset(_atcmd_cache,0,sizeof(_atcmd_cache));
	while(timeout-->0)
	{
		int rtn = AtCmdRecv(INTERVAL,TIMEOUT_LONG,pCmd,sizeof(_atcmd_cache));
		if(rtn)
		{
			pCmd += rtn;
		}
		if((pCmd-_atcmd_cache)>0)
		{
			PrintLog(LOGTYPE_DEBUG,(char*)_atcmd_cache);
			if((strstr((char*)_atcmd_cache,"+CMGS:")!=NULL) && (strstr((char*)_atcmd_cache,"OK")!=NULL))
			{
				PrintLog(LOGTYPE_DEBUG,"\r\nSMS SEND OK!\r\n");
				return SUCCEED;
			}
		}
	}
	return -ERRFAILED;
}
void unPirseSms(unsigned char* SmsBuff,unsigned char *msg,char *no,int msglen,int nolen)
{
	unsigned char* Scan = SmsBuff;
	int step=0;
	while(*Scan)
	{
		switch(step)
		{
		case 0://+CMGR:
			if(*Scan == ':')
				step++;
			Scan++;
			break;
		case 1://"REC UNREAD"
			if (*Scan == ',')
				step++;
			Scan++;
			break;
		case 2://tele no
			if (*Scan == ',')
			{
				step++;
				*no = 0;
			}
			else if(*Scan!='"')
			{
				if(nolen > 0)
				{
					*no++ = *Scan;
					nolen --;
				}
				else
				{
					no --;
					*no = 0;
				}
			}
			Scan++;
			break;
		case 3:
			if (*Scan == ',')
				step++;
			Scan++;
			break;
		case 4:
			if ((*(Scan-1) == '\r')&&(*Scan == '\n'))
				step++;
			Scan++;
			break;
		case 5://msg
			if (*Scan == ';')
			{
				*msg = 0;
				return;
			}
			else
			{
				if(msglen>0)
				{
					*msg++ = *Scan;
					msglen--;
				}
				else
				{
					msg--;
					*msg = 0;
				}
			}

			Scan++;
			break;
		}
	}
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
int M35RecvMessage(unsigned char *msg,char *no,int msglen,int nolen)
{
	int rt = SendAtCmd(&atlist[INDEX_CMGR],INTERVAL*4,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache));
	if(SUCCEED == rt)
	{
		PrintLog(LOGTYPE_DEBUG,(char*)_atcmd_cache);
		unPirseSms(_atcmd_cache,msg,no,msglen,nolen);
		PrintLog(LOGTYPE_DEBUG,"msg:%s,no:%s\n",msg,no);
		if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QMGDA],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//ɾ��ȫ���Ķ���
			return -ERRFAILED;
		return SUCCEED;
	}
	return -ERRFAILED;
}

/*
˵��:Gprs����ģʽ
ֵ: =SOCKET_TCP TCP, =SOCKET_UDP UDP
*/
static int _proto;

/*
˵��:����(TCP/UDP)ģʽ
�������:
proto ,=SOCKET_TCP TCP, =SOCKET_UDP UDP
�������:
��
����ֵ:
����ֵ���ɹ�����ͨ����������ʧ�ܷ��� -ERRFAILED
*/
int M35Mode(int proto)
{
	_proto = proto;

	return SUCCEED;
}

/**
* @brief ���ӵ�������
* @param ip ������IP
* @param port �������˿ں�
* @return �ɹ�0, ����ʧ��
*/

int M35Connect(unsigned long ip, unsigned short port)
{
	static unsigned long old_ip = 0;
	static unsigned short old_port = 0;
	int State,try = 5;

	while(try>0)
	{
		State=GetGprsStat();
		if(State < 0)
		{
			PrintLog(LOGTYPE_DEBUG,"Gprs Stat Timeout");
			return -ERRFAILED;
		}
		switch(State)
		{
		case IPSTATE_TCP_CONNECTING:
		case IPSTATE_UDP_CONNECTING:
			M35Disconnect();//�Ͽ�����
			TickSleep(20*15);
			if(try<3)
			{
				if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QIDEACT],INTERVAL,TIMEOUT*32,_atcmd_cache,sizeof(_atcmd_cache)))
				{
					PrintLog(LOGTYPE_DEBUG,"QIDEACT Timeout!\r\n");
					break;
				}
			}
			break;
		case IPSTATE_CONNECT_OK:
			if((old_ip == ip)&&(old_port == port))
			{
				return SUCCEED;
			}
			else
			{
				M35Disconnect();//�Ͽ�����
				TickSleep(20*15);				
			}
			break;
		case IPSTATE_IP_INITIAL:
		case IPSTATE_IP_STATUS:
		case IPSTATE_IP_CLOSE:
			try = -1;
			//״̬����
			break;
		default:
			if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QIDEACT],INTERVAL,TIMEOUT*32,_atcmd_cache,sizeof(_atcmd_cache)))
			{
				PrintLog(LOGTYPE_DEBUG,"QIDEACT Timeout!\r\n");
				break;
			}
			break;
		}
		try--;
	}
	if (try == 0)
	{
		PrintLog(LOGTYPE_DEBUG,"Gprs Stat Error!\r\n");
		return -ERRFAILED;
	}


	{
		const unsigned char* connect_ok="\r\nOK\r\n\r\nCONNECT OK\r\n";
		const unsigned char* already_connect="\r\nOK\r\n\r\nALREADY CONNECT";
		//const unsigned char* connect_fail="\r\nOK\r\n\r\nCONNECT FAIL";


		struct atlist_struct plist;
		unsigned char ip_addr[4];
		int rtn;
		static int Rate = 1;
		old_ip = ip;
		old_port = port;
		memcpy(ip_addr,&ip,4);
		/*�������������ʽ��ȷ���յ�OK����һ�����յ�CONNECT OK
		*/
		sprintf((char *)_atsend_cache,"AT+QIOPEN=\"%s\",\"%d.%d.%d.%d\",%d\r\n",
			(_proto==SOCKET_UDP?"UDP":"TCP"),ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3],port);
		plist.cmd = _atsend_cache;
		plist.keynum = 2;
		plist.key[0]=connect_ok;
		plist.key[1]=already_connect;
		plist.key[2]=NULL;//connect_fail;
		plist.times = 2;
		TickSleep(20);
		SetSms(1);
		rtn = SendAtCmd(&plist,INTERVAL_LONG,TIMEOUT_LONG*Rate,_atcmd_cache,sizeof(_atcmd_cache));
		SetSms(0);
		TickSleep(20);
		if(-ERRFAILED == rtn)
		{
			Rate += 1;
			PrintLog(LOGTYPE_DEBUG,"QIOPEN Timeout!\r\n");
			return -ERRFAILED;
		}
		else if(rtn == 0)
		{
			Rate = 1;;
			PrintLog(LOGTYPE_DEBUG,"QIOPEN OK!\r\n");
			return SUCCEED;
		} 
		else if(rtn == 1)
		{
			PrintLog(LOGTYPE_DEBUG,"Already connect!try again.\r\n");
			M35Disconnect();//�Ͽ�����
			return -ERRFAILED;
		}
		else if(rtn == 2)
		{
			PrintLog(LOGTYPE_DEBUG,"connect failed!try again.\r\n");
			return -ERRFAILED;
		}
	}
	return -ERRFAILED;
}

/**
* @brief ��������Ͽ�����
*/
void M35Disconnect()
{
	if(-ERRFAILED == SendAtCmd(&atlist[INDEX_QICLOSE],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
		PrintLog(LOGTYPE_DEBUG,"QICLOSE Timeout!\r\n");
}

/*
˵��:������ͨ�Žӿڷ�������(TCP)
�������:
buf �������ݻ�����ָ�룬len �������ݳ���
�������:
��
����ֵ:
SUCCEED��ʧ�� -ERRFAILED
*/
const char *data_accept = "\r\nSEND OK";
int M35RawSend(const unsigned char *buf, int len)
{
	struct atlist_struct plist;

	UartClearRecive(GPRS_PORT);	//����������������������

	sprintf((char *)_atsend_cache,"AT+QISEND=%d\r",len);
	plist.cmd = _atsend_cache;
	plist.keynum = 1;
	plist.key[0] = "\r\n> ";
	if(-ERRFAILED==SendAtCmd(&plist,INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
		return -ERRFAILED;

	UartClearRecive(GPRS_PORT);		//����������������������
	PrintLog(LOGTYPE_DEBUG,"GPRS send data: \r\n");
	UartSendData(GPRS_PORT,buf,len);
	PrintLog(LOGTYPE_DEBUG,"GPRS send result: \r\n");
	memset(_atcmd_cache,0,sizeof(_atcmd_cache));

	if(AtCmdRecv(INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		int i;
		PrintLog(LOGTYPE_DEBUG,(char*)_atcmd_cache);
		for(i=0;i<50;i++)
			if(0==memcmp(_atcmd_cache+i,data_accept,strlen(data_accept)))
				return 0;
	}
	return -1;
}
const char* Error = "\r\nERROR";

/*
˵��:������ͨ�Žӿڶ�ȡ����
�������:
len �������ݻ��泤��
�������:
buf �������ݻ�����ָ��
����ֵ:
�ɹ����ؽ������ݳ��ȣ�ʧ�� 0
*/
int M35GetChar(unsigned char *buf,int len)
{

		const char* cmd = "AT+QIRD=0,1,0,1024\r\n";
		unsigned char* Cur = buf;
		unsigned char* Scan = NULL;
		unsigned char* DataBuf = buf;
		unsigned char* Frame; 
		int data_len = 0;
		int rev_n;
		int step = 0;
		int wait = 10;
		int try = 5;
TRY_SEND:
		Cur = buf;
		Scan = NULL;
		DataBuf = buf;
		Frame = buf;
		wait = 10;
		step = 0;
		data_len = 0;

		UartClearRecive(GPRS_PORT);	//����������������������
		memset(buf,0,len);
		TickSleep(5);
		UartSendData(GPRS_PORT,(unsigned char*)cmd,strlen(cmd));
		PrintLog(LOGTYPE_DEBUG,"GPRS GetChar receive: ");
		while(1)
		{
			TickSleep(1);
			rev_n = UartRecvData(GPRS_PORT,Cur,buf + len - Cur);
			if(rev_n > 0)
			{
				PrintLog(LOGTYPE_DEBUG,(char*)Cur);
				Cur += rev_n;
				wait = 100;
			}
			else
			{
				if(wait==0)
				{
					PrintLog(LOGTYPE_DEBUG,"\r\nGetChar Timeout!\r\n");
					return -ERRFAILED;
				}
				wait--;
				TickSleep(1);
				continue;
			}
			if(step == 0)
			{
				if((Cur - Frame)>8)
				{
					const char* head="\r\n+QIRD:";
					const char* info ="\r\n+QIRDI: 0,1,0\r\n";
					char * dst = NULL;
					
					if(memcmp(Error,buf,strlen(Error)) == 0)
					{
						try --;
						if(try == 0)
						{
							PrintLog(LOGTYPE_DEBUG,"\r\nGetChar try!\r\n");
							return -ERRGPRS;
						}
						else
						{
							TickSleep(50);
							goto TRY_SEND;
						}
					}
					dst = strstr((char*)buf,info);
					if(dst)
					{
						Frame = Frame + strlen(info);
					}
					dst = strstr((char*)Frame,head);
					if(dst)
					{
						Scan = Frame + 8;
						step = 1;
					}
					else  if(memcmp(head,Frame,strlen(head)) == 0)
					{
						Scan = Frame+8;
						step = 1;//head finded.
					}
					else
					{
						PrintLog(LOGTYPE_DEBUG,"\r\nGetChar not found +QIRD!\r\n");
						return -ERRFAILED;
					}
				}
				else
					continue;
			}
			while(Scan!=Cur)
			{
				switch(step)
				{
				case 1://��ַ
				case 2://Э������
					if(*Scan == ',')
					{
						step ++;
					}
					Scan++;
					break;				
				case 3:
					if(*Scan != '\r')
					{
						data_len *= 10;
						data_len += char_to_hex(*Scan);
					}
					else
					{
						step++;
					}
					Scan++;
					break;
				case 4:
					if((*Scan != '\r')&&(*Scan != '\n'))
					{
						step++;
						break;
					}
					Scan++;					
					break;
				case 5:
					if(data_len == 0)
					{
						PrintLog(LOGTYPE_DEBUG,"\r\nGetChar all\r\n");
						return DataBuf-buf;
					}
					data_len--;
					*DataBuf++ = *Scan++;
					if(DataBuf == (buf+len))
					{
						PrintLog(LOGTYPE_DEBUG,"\r\nGetChar full\r\n");
						return DataBuf-buf; 
					}
					break;
				default:
					PrintLog(LOGTYPE_DEBUG,"\r\nGetChar step default\r\n");
					return -ERRFAILED;
				}
			}
		}
//		PrintLog(LOGTYPE_DEBUG,"\r\nGetChar End\r\n");
//	return 0;
}
static const int local_rate[]={57600,38400,19200,115200,9600,4800,2400,1200};
/*
˵��:�̶�TE-TA������
@param Port�˿ں�
*/
static int InitTeTaBps()
{
	int i;
	struct atlist_struct plist;
	PrintLog(LOGTYPE_DEBUG,"\r\n\033[1;31;40mGprs BPS : %d \033[0m\r\n",GPRS_BAUDRATE);
	UartSetPara(GPRS_PORT,GPRS_BAUDRATE,8,1,'N');
	for (i=0;i<60;i++)
	{
		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_TEST_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//����������
			break;
		TickSleep(20);
	}
	for (i=0;i<10;i++)
	if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_TEST_RATE],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//����������
		return SUCCEED;


	for(i=0;i<sizeof(local_rate)/sizeof(local_rate[0]);i++)
	{

		UartSetPara(GPRS_PORT,local_rate[i],8,1,'N');
		PrintLog(LOGTYPE_DEBUG,"Baudrate test : %d\r\n",local_rate[i]);

		sprintf((char *)_atsend_cache,"AT+IPR=%d\r",GPRS_BAUDRATE);
		plist.cmd = (unsigned char*)_atsend_cache;
		plist.keynum = 2;
		plist.key[0] = resp_ok; 	//û�л��Իظ�OK
		plist.key[1] = "AT+IPR=";	//�����л���
		if(-ERRFAILED!=SendAtCmd(&plist,INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))){
			UartSetPara(GPRS_PORT,GPRS_BAUDRATE,8,1,'N');//������óɹ�����оƬ�����ʸĹ���
			return SUCCEED;
		}
	}
	return -ERRFAILED;

}
static const char *ipstt[]={
	"IP INITIAL",
	"IP START",
	"IP CONFIG",
	"IP IND",
	"IP GPRSACT",
	"IP STATUS",
	"TCP CONNECTING",
	"UDP CONNECTING",
	"IP CLOSE",
	"CONNECT OK",
	"PDP DEACT"
};

/*
����Gprs����
*/
enum ip_status_t GetGprsStat(void)
{
	int j;
	const char **cptr;
	const char *ptr;
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_QISTAT],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))<0)
		return IPSTATE_ERROR;

	cptr = &ipstt[0];
	ptr = (char *)&_atcmd_cache[15];
	for(j=0;j<sizeof(ipstt)/sizeof(ipstt[0]);j++)
	{
		if(strstr(ptr,*cptr) != NULL)
			break;
		/*if(memcmp(ptr,*cptr,strlen(*cptr))==0)
			break;*/
		cptr++;
	}
	if(j>=sizeof(ipstt)/sizeof(ipstt[0]))
		return IPSTATE_ERROR;
	return (enum ip_status_t)j;	//�õ�IP״̬
}
static void GetIp(const unsigned char * str,varip_t* ip)
{
	const unsigned char* cur;
	const char* ok="\r\nOK\r\n";

	if(memcmp(str,ok,strlen(ok))==0)
	{
		cur = str + strlen(ok);
		StrToIp((const char*)cur,ip);
	}
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
/*int M35Dns(const char* web_addr,varip_t* ip)
{
	struct atlist_struct plist;
	const char* cmd = "AT+QIDNSGIP=\"%s\"\r\n";
	int rtn;
	sprintf((char *)_atsend_cache,cmd,web_addr);
	plist.cmd = _atsend_cache;
	plist.keynum = 2;
	plist.key[0]=resp_ok;
	plist.key[1]=(unsigned char*)"+CMS ERROR";
	rtn = SendAtCmd(&plist,INTERVAL_LONG*8,TIMEOUT_LONG*32,_atcmd_cache,sizeof(_atcmd_cache));
	if(rtn == 0)
	{
		 PrintLog(LOGTYPE_DEBUG,"%s\r\n",_atcmd_cache);
		 GetIp(_atcmd_cache,ip);
		 return SUCCEED;
	}
	return 	-ERRFAILED;
}*/
int M35Dns(const char* web_addr,varip_t* ip)
{
	int State,try = 5;

	while(try>0)
	{
		State=GetGprsStat();
		if(State < 0)
		{
			PrintLog(LOGTYPE_DEBUG,"Gprs Stat Timeout");
			return -ERRFAILED;
		}
		switch(State)
		{
		case IPSTATE_TCP_CONNECTING:
		case IPSTATE_UDP_CONNECTING:
		case IPSTATE_CONNECT_OK:
			M35Disconnect();//�Ͽ�����
			if(try<3)
			{
				if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QIDEACT],INTERVAL,TIMEOUT*32,_atcmd_cache,sizeof(_atcmd_cache)))
				{
					PrintLog(LOGTYPE_DEBUG,"QIDEACT Timeout!\r\n");
					break;
				}
			}
			break;
		case IPSTATE_IP_INITIAL:
		case IPSTATE_IP_STATUS:
		case IPSTATE_IP_CLOSE:
			try = -1;
			//״̬����
			break;
		default:
			if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QIDEACT],INTERVAL,TIMEOUT*32,_atcmd_cache,sizeof(_atcmd_cache)))
			{
				PrintLog(LOGTYPE_DEBUG,"QIDEACT Timeout!\r\n");
				break;
			}
			break;
		}
		try--;
	}
	if (try == 0)
	{
		PrintLog(LOGTYPE_DEBUG,"Gprs Stat Error!\r\n");
		return -ERRFAILED;
	}
	{
		unsigned int rt = 0,buf_len = sizeof(_atcmd_cache);
		unsigned int timeout = 500;
		unsigned char* pbuf = _atcmd_cache;
		unsigned char* end = _atcmd_cache+sizeof(_atcmd_cache);
		const char* cmd = "AT+QIDNSGIP=\"%s\"\r\n";
		sprintf((char *)_atsend_cache,cmd,web_addr);
		UartClearRecive(GPRS_PORT);
		UartSendData(GPRS_PORT,_atsend_cache,strlen((const char*)_atsend_cache));
		PrintLog(LOGTYPE_DEBUG,"GPRS send: ");
		PrintLog(LOGTYPE_DEBUG,(char*)_atsend_cache);
	
		while ((pbuf < end)&&(timeout>0))
		{
			SetSms(1);
			rt = AtCmdRecv(INTERVAL_LONG,TIMEOUT_LONG*8,pbuf,buf_len);
			SetSms(0);
			PrintLog(LOGTYPE_DEBUG,"GPRS DNS receive: ");
			PrintLog(LOGTYPE_DEBUG,(char*)pbuf);
			if(rt==0)
			{
				PrintLog(LOGTYPE_DEBUG,"\r\n");
				return -1;
			}
			if(NULL != strstr((char*)_atcmd_cache,"\r\nOK\r\n"))
			{
				if(NULL != strstr((char*)_atcmd_cache,"."))
				{
					GetIp(_atcmd_cache,ip);
					return SUCCEED;
				}
				timeout = 60;
			}
			if(NULL != strstr((char*)_atcmd_cache,"ERROR"))
			{
				return 	-ERRFAILED;	
			}
			pbuf += rt;
			buf_len -= rt;
		    timeout--;
			Sleep(1);
		}
		return 	-ERRFAILED;
	}
}
#endif
