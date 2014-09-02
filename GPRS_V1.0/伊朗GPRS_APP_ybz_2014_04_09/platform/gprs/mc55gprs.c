#include <string.h>
#include <stdio.h>

#include "plat_include/sys_config.h"
#include "plat_include/Plat_defines.h"
#include "plat_include/Plat_uart.h"
#include "plat_include/Mc55gprs.h"
#include "plat_include/GprsCtrl.h"
#include "plat_include/plat_network.h"
#include "plat_include/sleep.h"
#include "plat_include/func.h"
#ifdef GPRS_MODULE_MC55I
#define INTERVAL	40
#define INTERVAL_LONG	200
#define TIMEOUT 200
#define TIMEOUT_LONG 2000

static unsigned char _atsend_cache[50];//AT����ͻ���
static unsigned char _atcmd_cache[220];//AT������ܻ���
static char _apn[32] = {0}; //apn

static int InitTeTaBps(void);

enum cmdindex{
	/*0*/	TEST_RATE		,
	/*1*/	SET_ATE		,
	/*2*/	SET_URC		,
	/*3*/	SICI0,
	/*4*/	SICS0_CONTYPE,
	/*5*/	SICS0_INACTTO,
	/*6*/	SISI1,
	/*7*/	SISS1_SRVTYPE,
	/*8*/	SISS1_CONID,
	/*9*/	SISO1,
	/*10*/	CSQ,
	/*11*/	SSYNC,
	/*12*/	IPR,
	/*13*/	SISC1,
	/*14*/	SISO,
	/*15*/	SISS2_SRVTYPE,
	/*16*/	SISS2_CONID,
	/*17*/	SISI2,
	/*18*/	SISO2,
	/*19*/	SISC2,
	/*20*/	SISS2_SRVTYPE_NONE,
	/*21*/	SISO0,
	/*22*/	SISC0,
	/*23*/	SISS0_SRVTYPE_NONE,
	/*24*/	SISS1_SRVTYPE_N0NE,
	/*25*/	CIMI,
	/*26*/	COPS,
	/*27*/	MONI,
	/*28*/	COPS2,
	/*29*/	COPS12,
	/*30*/	CREG,
	/*31*/	CGATT1,
	/*32*/	CGATT2,
	/*33*/	CMGF,
	/*34*/	CMGR,
	/*34*/  CMGD,
	/*35*/	TEST_AT,
};
static const unsigned char resp_ok[] = "\r\nOK\r\n";
static const unsigned char resp_ok2[] = "\r\nOK\r\n";
static const struct atlist_struct atlist[] = {
	/*0*/	{"AT+IPR?\r", 	2, {"\r\n+IPR:","AT+IPR?\r\r\n+IPR: ", NULL, NULL}},
	/*1*/	{"ATE0\r",		2,{resp_ok,"ATE0\r\r\nOK\r\n",NULL,NULL}},
	/*2*/	{"AT^SCFG=Tcp/WithURCs,off\r",2,{"\r\n^SCFG: \"Tcp/WithURCs\",\"off\"\r\n\r\nOK\r\n",NULL,NULL}},
 	/*3*/	{"AT^SICI=0\r",2,"\r\n^SICI: ","\r\nERROR",NULL,NULL},
	/*4*/	{"AT^SICS=0,conType,GPRS0\r",1,resp_ok2,NULL,NULL,NULL},
	/*5*/	{"AT^SICS=0,\"inactTO\",\"3600\"\r",1,resp_ok2,NULL,NULL,NULL},	
	/*6*/	{"AT^SISI=1\r",2,"\r\n^SISI: ","\r\nERROR",NULL,NULL},
	/*7*/	{"AT^SISS=1,srvType,socket\r",1,resp_ok2,NULL,NULL,NULL},
	/*8*/	{"AT^SISS=1,conId,0\r",1,resp_ok2,NULL,NULL,NULL},
	/*9*/	{"AT^SISO=1\r",1,resp_ok2,NULL,NULL,NULL},	
	/*10*/	{"AT+CSQ\r",1,"\r\n+CSQ: ",NULL,NULL,NULL},
	/*11*/	{"AT^SSYNC=1\r",1,{resp_ok,NULL,NULL,NULL}},
	/*12*/	{"AT+IPR=115200\r",2,{resp_ok,"AT+IPR=115200\r\r\nOK\r\n",NULL,NULL}},
	/*13*/	{"AT^SISC=1\r",1,resp_ok2,NULL,NULL,NULL},
	/*14*/	{"AT^SISO?\r",1,"\r\n^SISO: ",NULL,NULL,NULL},
	/*15*/	{"AT^SISS=2,srvType,socket\r",1,resp_ok2,NULL,NULL,NULL},
	/*16*/	{"AT^SISS=2,conId,0\r",1,resp_ok2,NULL,NULL,NULL},
	/*17*/	{"AT^SISI=2\r",2,"\r\n^SISI: ","\r\nERROR",NULL,NULL},
	/*18*/	{"AT^SISO=2\r",1,resp_ok2,NULL,NULL,NULL},
	/*19*/	{"AT^SISC=2\r",1,resp_ok2,NULL,NULL,NULL},
	/*20*/	{"AT^SISS=2,srvType,none\r",1,resp_ok2,NULL,NULL,NULL},
	/*21*/	{"AT^SISO=0\r",1,resp_ok2,NULL,NULL,NULL},
	/*22*/	{"AT^SISC=0\r",1,resp_ok2,NULL,NULL,NULL},
	/*23*/	{"AT^SISS=0,srvType,none\r",1,resp_ok2,NULL,NULL,NULL},
	/*24*/	{"AT^SISS=1,srvType,none\r",1,resp_ok2,NULL,NULL,NULL},
	/*25*/	{"AT+CIMI\r",1,"\r\n4600",NULL,NULL,NULL},
	/*26*/	{"AT+COPS=?\r",1,"\r\n+COPS:",NULL,NULL,NULL},
	/*27*/	{"AT^MONI\r",1,"\r\n",NULL,NULL,NULL},
	/*28*/	{"AT+COPS=2\r",1,resp_ok,NULL,NULL,NULL},
	/*29*/	{"AT+COPS=1,2,\"46000\"\r",1,resp_ok,NULL,NULL,NULL},
	/*30*/	{"AT+CREG?\r",3,"\r\n+CREG: 0,1","\r\n+CREG: 0,5","\r\n+CREG: 0,2",NULL},
	/*31*/	{"AT+CGATT?\r",1,"\r\n+CGATT: 1",NULL,NULL,NULL},
	/*32*/	{"AT+CGATT=1\r",1,resp_ok,NULL,NULL,NULL},
	/*33*/	{"AT+CMGF=1\r",1,resp_ok,NULL,NULL,NULL},
	/*34*/	{"AT+CMGR=1\r\n",1,{"\r\n+CMGR:",NULL,NULL,NULL}},
	/*35*/	{"AT+CMGD=1\r\n",1,{resp_ok,NULL,NULL,NULL}},
	/*36*/	{"\r\nAT\r\n", 1, {"\r\nOK\r\n", NULL, NULL, NULL}},

};

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
int Mc55Mode(int proto)
{
	_proto = proto;
	return SUCCEED;
}

/*
˵��:GPRS/CDMAģ���ϵ��ʼ��
�������:
Mode Gprs���ӷ�ʽ:GSMODE_PPPD PPP��ʽ����,GSMODE_ATCMD AT���ʽ����
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int Mc55Init(GPRS_MODE Mode)
{
	gprs_switch_init();
	UartOpen(GPRS_PORT);
	UartSetPara(GPRS_PORT,GPRS_BAUDRATE,8, 1,'N');
	Mc55PowerOn();
	TickSleep(60);
	Mc55PowerOff();
	TickSleep(400);
	Mc55PowerOn();
	TickSleep(20);
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
int Mc55PowerOff(void)
{
	gprsline_pwroff();
	return SUCCEED;
}
/**
* @brief ��GPRS/CDMAģ���ϵ�
* @param ��
*/
void Mc55PowerOn(void)
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
int Mc55DialOn(void)
{
	int timeout;

	if(-ERRFAILED == InitTeTaBps())
	{
		PrintLog(LOGTYPE_DEBUG,"��ʼ��TE������ʧ��\r\n");
		return -1;
	}
	if(-ERRFAILED==SendAtCmd(&atlist[SET_ATE],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���û���
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[SSYNC],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//SSYNC PIN
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[SET_URC],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//TCP ֪ͨ
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[CIMI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//TCP ֪ͨ
		return -ERRFAILED;

	if(_atcmd_cache[6]!='1')
	{		//ֻ����ͨ�Ŀ��Ų�������
		if(-ERRFAILED==SendAtCmd(&atlist[COPS2],INTERVAL,TIMEOUT*4,_atcmd_cache,sizeof(_atcmd_cache)))
			return -1;
		if(-ERRFAILED==SendAtCmd(&atlist[COPS12],INTERVAL,TIMEOUT*4,_atcmd_cache,sizeof(_atcmd_cache)))
			return -1;
	}

	if(-ERRFAILED==SendAtCmd(&atlist[CMGF],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//���Ÿ�ʽ:�ı�
		return -ERRFAILED;

	if(Mc55GetSig()<=1)	//�����ź�
		return -ERRFAILED;


	timeout = 100;
	while(1)
	{
		int rt = SendAtCmd(&atlist[CREG],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache));
		if((rt == 0) || (rt == 1))
			break;
		TickSleep(4);
		Mc55GetSig();
		if (timeout-- == 0)
		{
			return -ERRFAILED;
		}
	}

	if(-ERRFAILED==SendAtCmd(&atlist[CGATT1],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//��ѯ�����Ƿ�ɹ�
		if(-ERRFAILED==SendAtCmd(&atlist[CGATT2],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//��ѯ�����Ƿ�ɹ�
			return -ERRFAILED;

	return SUCCEED;

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
void Mc55DialOff(void)
{
	
}
static const int local_rate[]={115200,57600,38400,19200,9600,4800,2400,1200};
/*
˵��:�̶�TE-TA������
@param Port�˿ں�
*/
static int InitTeTaBps(void)
{
	int i;
	struct atlist_struct plist;
	char cmd[50];
/*	for (i=0;i<60;i++)
	{
		if(-ERRFAILED!=SendAtCmd(&atlist[TEST_AT],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//����������
			break;
		TickSleep(20);
	}*/
	for (i=0;i<10;i++)
		if(SendAtCmd(&atlist[TEST_RATE],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))>=0)
			return SUCCEED;
	for(i=0;i<sizeof(local_rate)/sizeof(local_rate[0]);i++)
	{

		UartSetPara(GPRS_PORT,local_rate[i],8,1,'N');
		PrintLog(LOGTYPE_DEBUG,"Baudrate test : %d\r\n",local_rate[i]);

		sprintf((char *)cmd,"AT+IPR=%d\r",GPRS_BAUDRATE);
		plist.cmd = (unsigned char*)cmd;
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
/*
˵��:��ȡ��ǰģ���ź�ǿ��
�������:
��
�������:
��
����ֵ:
�ɹ������ź�ǿ��ֵ��ʧ�� -ERRFAILED
*/
int Mc55GetSig(void)
{
	unsigned int a;
	if(-ERRFAILED == SendAtCmd(&atlist[CSQ],INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		PrintLog(LOGTYPE_DEBUG,"TEST_CSQ Timeout!\r\n");
		return 0;//�����ź�ǿ��
	}
	a = char_to_hex(_atcmd_cache[8]);//��8�ֽڿ�ʼ��rssi
	if((_atcmd_cache[9])>='0' && (_atcmd_cache[9]<='9'))//����һλ
	{	
		a *= 10;
		a += char_to_hex(_atcmd_cache[9]);
	}
	PrintLog(LOGTYPE_DEBUG,"CSQ:%d\r\n",a);
	if(a!=99)
		return a;
	else
		return 0;
}
static int cal_addr(int loc,const unsigned char* buf)
{
	int j;
	unsigned int i,a;
	for(i=0;i<4;i++){
		loc += string_to_hex(&a,buf+loc);
		loc ++;
		j<<=8;
		j|=a;
	}
	return j;
}
static int find_index(int loc,int n,char ch,unsigned char* buf,int buf_len)
{
	int i;
	for(i=0;loc<buf_len;loc++){
		if(buf[loc]==ch)
			i++;
		if(i==n)	//�ͻ��˷��ڵڶ�������ͨ��
			break;
	}
	if(loc>=buf_len)
		return -1;
	return loc;
}
#define REMOTE_STATE	0
#define REMOTE_IP		1
#define CLIENT_STATE	2
#define SERVER_STATE	3
#define	LOCAL_IP		4
static int connect_info(int type)
{
	int i;
	if(-ERRFAILED == SendAtCmd(&atlist[SISO],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))<0)
		return -1;
	switch(type){
	case REMOTE_STATE:
		i = find_index(0,1,'^',_atcmd_cache,sizeof(_atcmd_cache));	//��0��ʼ�ҵ���һ��^
		if(i<0)	return -1;
		if(_atcmd_cache[i+11]!='S')	//S��ʾSocket
			return -1;
		if((_atcmd_cache[i+20]=='2')&&(_atcmd_cache[i+24]=='4'))
			return 0;			//��Զ����������
		if((_atcmd_cache[i+20]=='4')&&(_atcmd_cache[i+24]=='4'))
			return 1;			//Զ��������
		return 2;
	case REMOTE_IP:
		i = find_index(0,1,'^',_atcmd_cache,sizeof(_atcmd_cache));
		if(i<0)		return -1;
		if((_atcmd_cache[i+11])!='S')
			return -1;			
		i = find_index(i,13,'"',_atcmd_cache,sizeof(_atcmd_cache));
		if(i<0)	return -1;
		i++;
		return cal_addr(i,_atcmd_cache);
	case CLIENT_STATE:
		i = find_index(0,2,'^',_atcmd_cache,sizeof(_atcmd_cache));
		if(i<0)	return -1;
		if((_atcmd_cache[i+11])!='S')
			return -1;			//û�д�
		if((_atcmd_cache[i+20]=='4')&&(_atcmd_cache[i+24]=='2'))
			return 1;			//�����ӵ�Զ�̷�����
		return 0;
	case SERVER_STATE:
		i = find_index(0,3,'^',_atcmd_cache,sizeof(_atcmd_cache));
		if(i<0)	return -1;
		if((_atcmd_cache[i+11])!='S')
			return -1;			//û�д�
		if((_atcmd_cache[i+20]=='3')&&(_atcmd_cache[i+24]=='3'))
			return 1;			//�����ӵ�Զ�̷�����
		return 0;
	case LOCAL_IP:
		i = find_index(0,2,'^',_atcmd_cache,sizeof(_atcmd_cache));
		if(i<0)	return -1;
		if((_atcmd_cache[i+11])=='S')	//�пͻ��˴��ѵõ�
			goto calip;
		i = find_index(0,3,'^',_atcmd_cache,sizeof(_atcmd_cache));
		if(i<0)	return -1;
		if((_atcmd_cache[i+11])!='S')	//û������
			return -1;
		calip:
		i = find_index(i,11,'"',_atcmd_cache,sizeof(_atcmd_cache));
		if(i<0)	return -1;
		i++;
		return cal_addr(i,_atcmd_cache);
	default:	
		return -1;
	}
}

/**
* @brief ���ӵ�������
* @param ip ������IP
* @param port �������˿ں�
* @return �ɹ�0, ����ʧ��
*/

int Mc55Connect(unsigned long ip, unsigned short port)
{
//��ѯ״̬
	unsigned char * ip_addr = (unsigned char *)&ip;
	if(-ERRFAILED == SendAtCmd(&atlist[SICI0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
		return -1;
	if(connect_info(CLIENT_STATE)>=0)//��û�н������ſ��Խ���
		return -1;
	
	//����service profile-------------------------------------------------------
	if(SendAtCmd(&atlist[SISS1_SRVTYPE],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))<0)
		return -1;
	if(SendAtCmd(&atlist[SISS1_CONID],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))<0)
		return -1;
	
	struct atlist_struct plist;
	if(_proto == SOCKET_TCP)
	{
		sprintf((char *)_atsend_cache,"AT^SISS=1,address,\"socktcp://%d.%d.%d.%d:%d\"\r",
			ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3],port);
	}
	else
	{
		sprintf((char *)_atsend_cache,"AT^SISS=1,address,\"sockudp://%d.%d.%d.%d:%d\"\r",
			ip_addr[0],ip_addr[1],ip_addr[2],ip_addr[3],port);		
	}
	plist.cmd = _atsend_cache;
	plist.keynum=1;
	plist.key[0]=resp_ok2;
	if(SendAtCmd(&plist,INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))<0)
		return -1;

	
	SendAtCmd(&atlist[SISI1],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache));//��ѯconnect״̬

	if(SendAtCmd(&atlist[SISO1],INTERVAL_LONG,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache))<0)
		return -1;	
	connect_info(CLIENT_STATE);

	return 0;
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
int Mc55SetApn(const char *apn)
{
	memset(_apn,0,sizeof(_apn));
	strncpy(_apn,apn,sizeof(_apn));
	return SUCCEED;
}

/**
* @brief ��������Ͽ�����
*/
void Mc55Disconnect()
{
	if(SendAtCmd(&atlist[SISC1],INTERVAL_LONG,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache))<0)
		PrintLog(LOGTYPE_DEBUG,"Mc55Disconnect Time out!\r\n");
	
	if(SendAtCmd(&atlist[SISS1_SRVTYPE_N0NE],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))<0)
		PrintLog(LOGTYPE_DEBUG,"Mc55Disconnect Time out!\r\n");
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

int Mc55RawSend(const unsigned char *buf, int len)
{
	struct atlist_struct plist;
	unsigned int a;
	
	if(len>GPRS_FRAG)
		return -1;
	UartClearRecive(GPRS_PORT);	//����������������������
	sprintf((char *)_atsend_cache,"AT^SISW=%d,%d\r",0,len);
	plist.cmd =_atsend_cache;
	plist.keynum = 1;
	plist.key[0] = "\r\n^SISW: ";
	if(SendAtCmd(&plist,100,500,_atcmd_cache,sizeof(_atcmd_cache))<0)
		return -1;
	string_to_hex(&a,&_atcmd_cache[12]);//ǰ���Ђ���
	if(a==len){		//ģ����Խ�����ô��������
		UartClearRecive(GPRS_PORT);		//����������������������
		PrintLog(LOGTYPE_DEBUG,"GPRS send data: \r\n");
		UartSendData(GPRS_PORT,buf,len);
		PrintLog(LOGTYPE_DEBUG,"GPRS send result: \r\n");
		AtCmdRecv(INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache));
		if(memcmp(_atcmd_cache,resp_ok2,a)!=0)
			return -1;
	}
	return len;
}
/*
˵��:������ͨ�Žӿڶ�ȡ����
�������:
len �������ݻ��泤��
�������:
buf �������ݻ�����ָ��
����ֵ:
�ɹ����ؽ������ݳ��ȣ�ʧ�� 0
*/
static const char *resp_sisr="\r\n^SISR: ";
int Mc55GetChar(unsigned char *buf,int len)
{
	unsigned int a,i;
	
	UartClearRecive(GPRS_PORT);	//����������������������
	sprintf((char *)_atsend_cache,"AT^SISR=%d,%d\r",0,len);
	UartSendData(UART_GPRS,_atsend_cache,strlen((char *)_atsend_cache));
	
	a = AtCmdRecv(100,500,_atcmd_cache,sizeof(_atcmd_cache));
	if(a<21)		//�ظ������ٳ�����21
		return -1;
	if(0!=memcmp(_atcmd_cache,resp_sisr,strlen(resp_sisr)))
		return -1;
	i=string_to_hex(&a,&_atcmd_cache[12]);
	if(a==0)
		return 0;
	len = a<=len?a:len;	//��BGS2�ֲᣬa�ض�<=len
	i = 12+i+2;			//������ʼ��ַ
	a = sizeof(_atcmd_cache)-i;	//ʣ��ռ�
	if(len<=a)
	{
		memcpy(buf,_atcmd_cache+i,len);
	}else
	{		//��������������Ļ�����
		memcpy(buf,_atcmd_cache+i,a);
		UartRecvData(UART_GPRS,buf+a,len-a);
	}
	return len;	
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
int Mc55SendMessage(const unsigned char *msg,const char *no,int msglen)
{
	struct atlist_struct plist;
	sprintf((char *)_atsend_cache,"AT^CMGS=\"%s\"\r\n",no);
	plist.cmd = _atcmd_cache;
	plist.keynum = 1;
	plist.key[0] = "\r\n> ";	
	if(-ERRFAILED==SendAtCmd(&plist,INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		return -ERRFAILED;
	}
   	UartSendData(GPRS_PORT,msg,msglen);
	UartSendData(GPRS_PORT,(unsigned char*)StopSms,strlen(StopSms));
	if(AtCmdRecv(INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		PrintLog(LOGTYPE_DEBUG,(char*)_atcmd_cache);
		if((strstr("+CMGS:",(char*)_atcmd_cache)!=NULL) && (strstr("OK",(char*)_atcmd_cache)!=NULL))
		{
			return SUCCEED;
		}
	}
	return -ERRFAILED;
}
static void unPirseSms(unsigned char* SmsBuff,unsigned char *msg,char *no,int msglen,int nolen)
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
int Mc55RecvMessage(unsigned char *msg,char *no,int msglen,int nolen)
{
 	if(SUCCEED == SendAtCmd(&atlist[CMGR],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		PrintLog(LOGTYPE_DEBUG,(char*)_atcmd_cache);
		unPirseSms(_atcmd_cache,msg,no,msglen,nolen);
		PrintLog(LOGTYPE_DEBUG,"msg:%s,no:%s\n",msg,no);
		if(-ERRFAILED==SendAtCmd(&atlist[CMGD],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//ɾ��ȫ���Ķ���
			return -ERRFAILED;
		return SUCCEED;
	}


	return -ERRFAILED;
}
#endif
