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
static int InitTeTaBps(void);//固定BPS
static unsigned char _atsend_cache[50];//AT命令发送缓存
static unsigned char _atcmd_cache[220];//AT命令接受缓存
static char _apn[32] = {0}; //apn
static char cdma_passwd[32]={0};//用户名
static char cdma_user[32] = {0};//密码



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
说明:GPRS/CDMA模块上电初始化
输入参数:
Mode Gprs连接方式:GSMODE_PPPD PPP方式连接,GSMODE_ATCMD AT命令方式连接
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
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
说明:GPRS/CDMA模块关机掉电
输入参数:
    无
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int M35PowerOff(void)
{
	gprsline_pwroff();
	return SUCCEED;
}
/**
* @brief 给GPRS/CDMA模块上电
* @param 无
*/
void M35PowerOn(void)
{
	gprsline_pwron();
}

/*
说明:GPRS/CDMA网络拨号
输入参数:
无
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int M35DialOn(void)
{
	static int delete_sms = 0;
	int timeout = 50;
	
	if(-ERRFAILED == InitTeTaBps())
	{
		PrintLog(LOGTYPE_DEBUG,"初始化TE波特率失败\r\n");
		return -1;
	}
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)));//如果收到一样的帧表名有回显
	
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
			return -ERRFAILED;
	
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_CPIN],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//测试sim卡
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QINDI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置缓冲收数据
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QIHEAD],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置Gprs数据头
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_CMGF],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置短息文本模式
		return -ERRFAILED;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_CNMI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置短息就收模式(不通知)
		return -ERRFAILED;
   	timeout = 50;
	while(1)
	{
		if(M35GetSig() > 1)	//看看信号
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
		if(-ERRFAILED != SendAtCmd(&atlist[INDEX_TEST_CREG],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//找网络
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
		if(-ERRFAILED == SendAtCmd(&plist,INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))	 //定义PDP,APN
		{
			return -ERRFAILED;
		}
	}
	timeout = 10;
	while(1)
	{
   		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_CGATT1],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置附着GPRS网
			break;
	 	TickSleep(50);
		if(timeout-- == 0)
			break;
	}
	if((delete_sms%3) == 0)
	{
		delete_sms++;		
		if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QMGDA],INTERVAL,TIMEOUT*4,_atcmd_cache,sizeof(_atcmd_cache)))//删除全部的短信
			return -ERRFAILED;
	}
	timeout = 50;
	while(1)
	{
		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_TEST_CGATT],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//查询附着是否成功
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
	-1	AT 命令通讯失败
	-2	AT 命令通讯失败
	-3	Sim卡失败
	-4	AT 命令通讯失败
	-5	AT 命令通讯失败
	-6	AT 命令通讯失败
	-7	AT 命令通讯失败
	-8	信号强度弱
	-9	寻找网络失败
	-10	AT 命令通讯失败
	-11 删除短信失败
	-12	网络附着失败	
*/
int M35DialOnTest(void)
{
	static int delete_sms = 0;
	int timeout = 50;
	M35Init(GSMODE_ATCMD);

	if(-ERRFAILED == InitTeTaBps())
	{
		PrintLog(LOGTYPE_DEBUG,"初始化TE波特率失败\r\n");
		return -1;
	}
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)));//如果收到一样的帧表名有回显
	
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
			return -2;
	
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_CPIN],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//测试sim卡
		return -3;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QINDI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置缓冲收数据
		return -4;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QIHEAD],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置Gprs数据头
		return -5;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_CMGF],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置短息文本模式
		return -6;

	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_CNMI],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//设置短息就收模式(不通知)
		return -7;
   	timeout = 50;
	while(1)
	{
		if(M35GetSig() > 5)	//看看信号
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
		if(-ERRFAILED != SendAtCmd(&atlist[INDEX_TEST_CREG],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//找网络
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
		if(-ERRFAILED == SendAtCmd(&plist,INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))	 //定义PDP,APN
		{
			return -10;
		}

	}
	timeout = 10;
	while(1)
	{
   		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_CGATT1],INTERVAL*4,TIMEOUT_LONG*4,_atcmd_cache,sizeof(_atcmd_cache)))//设置附着GPRS网
			break;
	 	TickSleep(50);
		if(timeout-- == 0)
			break;
	}
	if((delete_sms%3) == 0)
	{
		delete_sms++;		
		if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QMGDA],INTERVAL*4,TIMEOUT_LONG*10,_atcmd_cache,sizeof(_atcmd_cache)))//删除全部的短信
			return -11;
	}
	timeout = 50;
	while(1)
	{
		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_TEST_CGATT],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//查询附着是否成功
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
说明:GPRS/CDMA网络状态
输入参数:
无
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int M35LineState(void)
{
	if(-ERRFAILED==SendAtCmd(&atlist[INDEX_TEST_CPIN],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//测试sim卡
		return -ERRFAILED;
	if(-ERRFAILED == SendAtCmd(&atlist[INDEX_TEST_CREG],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//找网络
		return -ERRFAILED;
	return SUCCEED;	
}
/**
* @brief 检测TCP连接状态
* @return 在线返回1, 掉线返回0
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
说明:断开GPRS/CDMA拨号连接,恢复模块到初始状态
输入参数:
无
输出参数:
无
返回值:
无
*/
void M35DialOff(void)
{
	if(-ERRFAILED == SendAtCmd(&atlist[INDEX_QIDEACT],INTERVAL,TIMEOUT*32,_atcmd_cache,sizeof(_atcmd_cache)))
		PrintLog(LOGTYPE_DEBUG,"QIDEACT Timeout!\r\n");
}
/*
说明:读取当前模块信号强度
输入参数:
无
输出参数:
无
返回值:
成功返回信号强度值，失败 -ERRFAILED
*/
int M35GetSig(void)
{
	unsigned int a;
	if(-ERRFAILED == SendAtCmd(&atlist[INDEX_TEST_CSQ],INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
	{
		PrintLog(LOGTYPE_DEBUG,"TEST_CSQ Timeout!\r\n");
		return 0;//最差的信号强度
	}
	a = char_to_hex(_atcmd_cache[8]);//第8字节开始是rssi
	if(_atcmd_cache[9]!=','){	//还有一位
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
说明:设置GPRS模块 APN
输入参数:
apn APN缓冲区指针
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int M35SetApn(const char *apn)
{
	memset(_apn,0,sizeof(_apn));
	strncpy(_apn,apn,sizeof(_apn));
	return SUCCEED;
}

/*
说明:设置GPRS/CDMA拨号的用户和密码
输入参数:
usr 用户名缓存指针
pwd 密码缓存指针
usrlen 用户名长度
pwdlen 密码长度
输出参数:
无
返回值:
无
*/
void M35SetUserPwd(const char *usr,const char *pwd)
{
	memset(cdma_user,0,sizeof(cdma_user));
	memset(cdma_passwd,0,sizeof(cdma_passwd));

	strncpy(cdma_user,usr,sizeof(cdma_user));
	strncpy(cdma_passwd,pwd,sizeof(cdma_passwd));
}
/*
说明:设置短信中心号码
输入参数:
no 短信中心号码缓存指针,len 中心号码长度
输出参数:
无
返回值:
无
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
说明:通过模块发送一条短消息
输入参数:
发送消息缓冲区指针
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
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
说明:接收一条新的短消息
输入参数:
接收缓存区指针
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int M35RecvMessage(unsigned char *msg,char *no,int msglen,int nolen)
{
	int rt = SendAtCmd(&atlist[INDEX_CMGR],INTERVAL*4,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache));
	if(SUCCEED == rt)
	{
		PrintLog(LOGTYPE_DEBUG,(char*)_atcmd_cache);
		unPirseSms(_atcmd_cache,msg,no,msglen,nolen);
		PrintLog(LOGTYPE_DEBUG,"msg:%s,no:%s\n",msg,no);
		if(-ERRFAILED==SendAtCmd(&atlist[INDEX_QMGDA],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//删除全部的短信
			return -ERRFAILED;
		return SUCCEED;
	}
	return -ERRFAILED;
}

/*
说明:Gprs连接模式
值: =SOCKET_TCP TCP, =SOCKET_UDP UDP
*/
static int _proto;

/*
说明:设置(TCP/UDP)模式
输入参数:
proto ,=SOCKET_TCP TCP, =SOCKET_UDP UDP
输出参数:
无
返回值:
返回值：成功返回通信描述符，失败返回 -ERRFAILED
*/
int M35Mode(int proto)
{
	_proto = proto;

	return SUCCEED;
}

/**
* @brief 连接到服务器
* @param ip 服务器IP
* @param port 服务器端口号
* @return 成功0, 否则失败
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
			M35Disconnect();//断开连接
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
				M35Disconnect();//断开连接
				TickSleep(20*15);				
			}
			break;
		case IPSTATE_IP_INITIAL:
		case IPSTATE_IP_STATUS:
		case IPSTATE_IP_CLOSE:
			try = -1;
			//状态正常
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
		/*这条合令如果格式正确先收到OK，过一会再收到CONNECT OK
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
			M35Disconnect();//断开连接
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
* @brief 与服务器断开连接
*/
void M35Disconnect()
{
	if(-ERRFAILED == SendAtCmd(&atlist[INDEX_QICLOSE],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))
		PrintLog(LOGTYPE_DEBUG,"QICLOSE Timeout!\r\n");
}

/*
说明:向网络通信接口发送数据(TCP)
输入参数:
buf 发送数据缓冲区指针，len 发送数据长度
输出参数:
无
返回值:
SUCCEED，失败 -ERRFAILED
*/
const char *data_accept = "\r\nSEND OK";
int M35RawSend(const unsigned char *buf, int len)
{
	struct atlist_struct plist;

	UartClearRecive(GPRS_PORT);	//将串口驱动读缓冲区读空

	sprintf((char *)_atsend_cache,"AT+QISEND=%d\r",len);
	plist.cmd = _atsend_cache;
	plist.keynum = 1;
	plist.key[0] = "\r\n> ";
	if(-ERRFAILED==SendAtCmd(&plist,INTERVAL,TIMEOUT_LONG,_atcmd_cache,sizeof(_atcmd_cache)))
		return -ERRFAILED;

	UartClearRecive(GPRS_PORT);		//将串口驱动读缓冲区读空
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
说明:从网络通信接口读取数据
输入参数:
len 接收数据缓存长度
输出参数:
buf 接收数据缓冲区指针
返回值:
成功返回接收数据长度，失败 0
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

		UartClearRecive(GPRS_PORT);	//将串口驱动读缓冲区读空
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
				case 1://地址
				case 2://协议类型
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
说明:固定TE-TA波特率
@param Port端口号
*/
static int InitTeTaBps()
{
	int i;
	struct atlist_struct plist;
	PrintLog(LOGTYPE_DEBUG,"\r\n\033[1;31;40mGprs BPS : %d \033[0m\r\n",GPRS_BAUDRATE);
	UartSetPara(GPRS_PORT,GPRS_BAUDRATE,8,1,'N');
	for (i=0;i<60;i++)
	{
		if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_TEST_ATE0],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//波特率正常
			break;
		TickSleep(20);
	}
	for (i=0;i<10;i++)
	if(-ERRFAILED!=SendAtCmd(&atlist[INDEX_TEST_RATE],INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache)))//波特率正常
		return SUCCEED;


	for(i=0;i<sizeof(local_rate)/sizeof(local_rate[0]);i++)
	{

		UartSetPara(GPRS_PORT,local_rate[i],8,1,'N');
		PrintLog(LOGTYPE_DEBUG,"Baudrate test : %d\r\n",local_rate[i]);

		sprintf((char *)_atsend_cache,"AT+IPR=%d\r",GPRS_BAUDRATE);
		plist.cmd = (unsigned char*)_atsend_cache;
		plist.keynum = 2;
		plist.key[0] = resp_ok; 	//没有回显回复OK
		plist.key[1] = "AT+IPR=";	//可能有回显
		if(-ERRFAILED!=SendAtCmd(&plist,INTERVAL,TIMEOUT,_atcmd_cache,sizeof(_atcmd_cache))){
			UartSetPara(GPRS_PORT,GPRS_BAUDRATE,8,1,'N');//如果设置成功，将芯片波特率改过来
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
测试Gprs环境
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
	return (enum ip_status_t)j;	//得到IP状态
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
说明:通过DNS服务得到IP地址
输入参数:
	web_addr WWW网址
输出
	ip ip地址
返回
成功 SUCCEED，失败 -ERRFAILED
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
			M35Disconnect();//断开连接
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
			//状态正常
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
