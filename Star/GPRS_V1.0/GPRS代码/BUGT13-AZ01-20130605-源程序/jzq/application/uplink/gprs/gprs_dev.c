/**
* gprs_dev.c -- GPRS模块管理
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-19
* 最后修改时间: 2009-5-19
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "plat_include/debug.h"
#include "plat_include/plat_defines.h"
#include "plat_include/plat_network.h"
#include "app_include/gprs/gprs_dev.h"
#include "app_include/lib/align.h"
#include "app_include/param/term.h"
#include "plat_include/plat_gprs.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/svrcomm.h"
#include "plat_include/reset.h"
#include "app_include/param/unique.h"
#include "plat_include/ring.h"
#include "plat_include/timeal.h"


//unsigned int GprsCmdBuffer[MAXSIZE_CMDBUF/4];
//unsigned char GprsDevModuleState;
unsigned char GprsDevDailState;
unsigned char GprsDevLineState;
unsigned char GprsDevRingFlag;
unsigned char GprsLoginState = 2;	// 0-正在登录1-登录成功 2-登录失败
unsigned char GprsActConnetSvr = 0;	// 1 - 激活终端连接主站 
unsigned char GprsConnetMode = 0;	//  0x01 - 客户端连接 0x02 - 服务端连接

short GprsDevSigdBm = 0;

static int hsocket  = 0;
struct Grpsbuf_t{
	unsigned char buf[1024];
	int len;
	unsigned char* cur;
}Grpsbuf;
void GprsDevInit(void)
{
	GprsInit(GSMODE_ATCMD);
	Grpsbuf.len = 0;
	Grpsbuf.cur = Grpsbuf.buf;
}

void GprsGetName(char *name)
{
	//strcpy(name, gprs_devp->name);
}

void GprsUpdSigAmp(void)
{	
	GprsDevSigdBm = GprsGetSig();
	PrintLog(LOGTYPE_SHORT, "signal quality = %d\n", GprsDevSigdBm);
}
static void GprsStateClear(void)
{
	GprsDevDailState = GSTAT_DIAL_OFF;
	GprsDevLineState = GSTAT_LINE_OFF;
//	GprsDevState = GSTAT_DEV_DEVERR;
//	GprsDevSigState = GSTAT_SIG_UNKNOWN;
	SvrCommLineState = LINESTAT_OFF;
	GprsLoginState = 0;
}
int GprsRestart(void)
{

	GprsStateClear();

	NetCloseSocket(hsocket);
	
	GprsSetApn(ParaTerm.svrip.apn);

	GprsSetUserPwd((const char *)ParaTerm.vpn.user, (const char *)ParaTerm.vpn.pwd);
	return 0;
}

int GprsCheck(void)
{
	if(GprsGetSig()>8)
		return 0;
	else
		return -1;
}

int GprsDail(void)
{
	static int cnt_dailerr = 0;//拨号失败次数
	int rtn;

	GprsDevDailState = GSTAT_DIAL_OFF;
	
	rtn = GprsDialOn();

	if(rtn == SUCCEED) {
		srand(UTimeReadCurrent());
		cnt_dailerr = 0;
		PrintLog(LOGTYPE_DEBUG,"\033[1;32;40mGprs dail successed.\033[0m\n");
		GprsDevDailState = GSTAT_DIAL_ON;
	}
	else{
		cnt_dailerr++;
		if(cnt_dailerr > 100){
			PrintLog(LOGTYPE_DEBUG,"GPRS dail fail times(%d)..,reboot\n", cnt_dailerr);
			SysReset();
		}
		PrintLog(LOGTYPE_DEBUG,"Gprs dail failed.\n");
	}

	return rtn;
}

int GprsDailOff(void)
{
	GprsDialOff();
	return 0;
}

int GprsConnect(int sel)
{
	unsigned long ip;
	unsigned short port;
	int rt;
	varip_t vip;
	GprsSetApn(ParaTerm.svrip.apn);


	hsocket = NetCreateSocket(PF_AT,ParaTerm.uplink.proto);
	if(hsocket == -ERRFAILED)
		return -1;

	if(0 == sel) 
	{
		if(*(unsigned int*)ParaTerm.svrip.ipmain == 0)
			return -ERRFAILED;
		memcpy(vip.uc,ParaTerm.svrip.ipmain,4);
		port = ParaTerm.svrip.portmain;
	}
	else if(1 == sel)
	{
		if(*(unsigned int*)ParaTerm.svrip.ipbakup == 0)
			return -ERRFAILED;
		ip = MAKE_LONG(ParaTerm.svrip.ipbakup);
		port = ParaTerm.svrip.portbakup;
		if(0 == ip || 0 == port) 
		{
			memcpy(vip.uc,ParaTerm.svrip.ipmain,4);
			port = ParaTerm.svrip.portmain;
		}
		else 
		{
			memcpy(vip.uc,ParaTerm.svrip.ipbakup,4);
			port = ParaTerm.svrip.portbakup;
		}

	}else if(2 == sel)
	{
		varip_t ip;
		PrintLog(LOGTYPE_DEBUG,"IP:DNS\r\n");
		if(strlen((const char*)ParaUni.web_addr)>0)
		{
				GprsDns((const char*)ParaUni.web_addr,&ip);
				if(ip.ul != 0)
				{
					vip = ip;
					if(ParaTerm.svrip.portmain != 0)
						port = ParaTerm.svrip.portmain;
					else
						port = ParaTerm.svrip.portbakup; 
				}
				else
				return -ERRFAILED;
		}
		else
		return -ERRFAILED;
	}
	else
		return -ERRFAILED;	
	rt = NetConnectHost(hsocket,vip,port);
	if(rt == SUCCEED)
	{
		GprsDevLineState = GSTAT_LINE_ON;//TCP连接成功
	}
	return rt;
}

void GprsDisConnect(void)
{

	PrintLog(LOGTYPE_DEBUG,"gprs dis connect...\n");
	GprsDevLineState = GSTAT_LINE_OFF;
	SvrCommLineState = LINESTAT_OFF;
	NetCloseSocket(hsocket);
#ifdef METER_MODULE	
		Signel_led(Signel_R,1);
#endif
	return ;
}

int GprsRing(int flag)
{
	return 0;
}


void GprsDectectLine(unsigned char uc)
{
	return ;
}
extern void UpdateSticComm(unsigned int bytes);
int GprsGetChar(unsigned char *buf)
{
	int rtn;
TRY:
	if(Grpsbuf.len>0)
	{
		*buf = *Grpsbuf.cur;
		Grpsbuf.cur++;
		Grpsbuf.len--;
		return 1;
	}
	else
	{
		Grpsbuf.cur = Grpsbuf.buf;
		Grpsbuf.len = 0;	
	}
	rtn = NetRecvData(hsocket,(char*)Grpsbuf.buf,sizeof(Grpsbuf.buf));
	if(rtn > 0)
	{
		Grpsbuf.len	= rtn;
		goto TRY;
	}
	else return rtn;
}

int GprsRawSend(const unsigned char *buf, int len)
{
	return(NetSendData(hsocket,(char*)buf, len));
}

//unsigned char GprsSigQuality(void)
//{ 
//	return GprsDevSigState;
//}

void GprsCheckTime(void)
{
	//(*gprs_devp->dev_checktime)();
}


/*
* 转换字符串ip地址到数字型ip地址
*/
void StrToIp(const char* StrIp,varip_t * Ip)
{
	char ip_str[5]={0};
	char *p_ip_str = ip_str;
	const char* pStrip = StrIp;
	int ip_index =0;
	while(*pStrip)
	{
		if(*pStrip != '.')
			*p_ip_str++ = *pStrip;
		else
		{
			Ip->uc[ip_index++] = atoi(ip_str);
			memset(ip_str,0,sizeof(ip_str));
			p_ip_str = ip_str;
		}
		pStrip++;
	}
	Ip->uc[ip_index++] = atoi(ip_str);
}
static int UnPackSmsParamCmd(const char* sms,struct SmSParam* sdata);
/*
* 解析思达自定义格式的短信
*+JACT:ACTIVE	激活主动连接主站
*+JACT:QUEREY	查询剩余电量
*+JACT:CHARGE,以#号结束taken码(20个ASCII码)	充值
*+JACT:PARAM,以#结束的密码串	解密后的串包括(通信模式，集中器编号，域名，IP地址，端口号,APN,ID,服务器密码)；
*
* 6B      9B          变长      变长    变长   变长    变长  变长 变长 变长（缺省6B）
* 例： +JACT:TCP,0001,szwzw.gicp.net,121.35.163.27,2227,CMNET,userid,222222;
* 
*/
#define ACTIVE_CONNECT 		"ACTIVE"
#define QUEREY_REMAINDER 	"QUERY"
#define CHARGE_MONEY		"CHARGE"
#define	SETUP_PARAM			"PARAM"
#define GET_PARAM			"GETPARAM"
#define GET_ERROR			"ERROR"
int UnPackSmsCmd(const char* sms,struct SMS_DATA* sdata)
{
	const char* SmsHead = "+JACT:";
	const char* pbuf = sms;
	unsigned char* pdata;
	const char * cmds[] = {ACTIVE_CONNECT,QUEREY_REMAINDER,CHARGE_MONEY,SETUP_PARAM,GET_PARAM,GET_ERROR};
	enum SMS_TYPE cmd_id[] = {SMS_ACTIVE,SMS_QUERY,SMS_CHARGE,SMS_PARAM,SMS_GETPARAM,SMS_GETERROR};


	int i;

	if(memcmp(sms,SmsHead,strlen(SmsHead)) != 0)
	{
		PrintLog(LOGTYPE_DEBUG,"Sms Head Error!\n");
		return -ERRSMSERROR;
	}	
	pbuf += strlen(SmsHead);
	//	PrintLog(LOGTYPE_DEBUG,field_name[step]);
	sdata->type = SMS_UNKNOWN;
	for(i=0;i<sizeof(cmds)/sizeof(cmds[0]);i++)
	{
		if(memcmp(pbuf,cmds[i],strlen(cmds[i])) == 0)
		{
			sdata->type = cmd_id[i];
			pbuf += strlen(cmds[i]);
			break;
		}
	}
	if(sdata->type == SMS_UNKNOWN)
		return -ERRFAILED;

	puts(":");
	if(sdata->type == SMS_PARAM)
	{
		 if(UnPackSmsParamCmd(pbuf,(struct SmSParam*)sdata->data)==-1)
		 {
		 	return -ERRFAILED;
		 }
	}
	else if(sdata->type == SMS_ACTIVE)
	{
		if(*pbuf == ',')
		{
			if(UnPackSmsParamCmd(pbuf,(struct SmSParam*)sdata->data)==-1)
		 	{
		 		return -ERRFAILED;
		 	}
			else 
				sdata->type = SMS_ACTIVE_PARAM;
					
		}	
	}
	else if(sdata->type == SMS_CHARGE)
	{
		pdata = sdata->data;
		if(*pbuf == ',')
			*pbuf++;
		for(i = 0;i < 20 ;i++)
		{
			if(isdigit(*pbuf))
				*pdata++ = *pbuf++;
			else
				return -1;
		}
		if(*pbuf != '#')
			return -1;
	}
	return SUCCEED;
}
int TestChar(char b)
{
	if(((b>='0')&&(b<='9'))||((b>='A')&&(b<='Z'))||((b>='a')&&(b<='z'))||(b == ',')||(b == '.'))
	{
		return 1;
	}
	else
		return 0;
}
int UnPackSmsParamCmd(const char* sms,struct SmSParam* sdata)
{
	char * data_list[9] = {sdata->password,sdata->Mode,sdata->SerialNo,sdata->WebAddr,
	sdata->IpAddr,sdata->Port,sdata->Apn,sdata->UserId,sdata->UserPassword};
	int step = 0;
	const char* pbuf = sms;
	char* pdata = NULL;	
	while(*pbuf != 0)
	{
		if(*pbuf != ',')
		{
			putchar(*pbuf);
			if(pdata)
				*pdata++ = *pbuf;
		}
		else
		{
			putchar('\n');
			pdata = data_list[step];
			step++;
			puts(":");			
		}
		pbuf++;
	}	

	return 0;
}
