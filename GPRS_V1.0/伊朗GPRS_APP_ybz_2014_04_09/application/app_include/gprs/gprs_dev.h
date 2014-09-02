/**
* gprs_dev.h -- GPRS模块管理
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-19
* 最后修改时间: 2009-5-19
*/

#ifndef _GPRS_DEV_H
#define _GPRS_DEV_H

#include "gprs.h"
#include "plat_include/plat_network.h"

#define GPRSMODULE_MC35    0x00

#define GSTAT_SIG_UNKNOWN    99

#define GSTAT_DIAL_OFF    0
#define GSTAT_DIAL_ON    1

#define GSTAT_LINE_OFF    0
#define GSTAT_LINE_ON    1

#define GSTAT_DEV_OK    0
#define GSTAT_DEV_RESOK    1
#define GSTAT_DEV_DEVERR    2
#define GSTAT_DEV_SIMERR    3 
#define GSTAT_DEV_NOSIG    4

//extern unsigned char GprsDevSigState;
extern unsigned char GprsDevDailState;
extern unsigned char GprsDevLineState;
extern unsigned char GprsDevModuleState;
extern unsigned char GprsDevRingFlag;
extern unsigned char GprsLoginState;
extern unsigned char GprsActConnetSvr;
extern unsigned char GprsConnetMode;
extern short GprsDevSigdBm;

void GprsDevInit(void);
int GprsRestart(void);
int GprsCheck(void);
int GprsDail(void);
int GprsDailOff(void);
int GprsConnect(int sel);
void GprsDisConnect(void);

int GprsLineState(void);
void GprsDectectLine(unsigned char uc);
int GprsGetChar(unsigned char *buf);
int GprsRawSend(const unsigned char *buf, int len);
void GprsCheckTime(void);
void GprsUpdSigAmp(void);

#define svr_lineled(arg) {if(arg) led_online_on();\
						 else led_online_off();}

void StrToIp(const char* StrIp,varip_t * Ip);

enum SMS_TYPE{SMS_UNKNOWN,SMS_ACTIVE,SMS_PARAM,SMS_QUERY,SMS_CHARGE,SMS_GETPARAM,SMS_GETERROR,SMS_ACTIVE_PARAM};
/*
* 解析思达自定义格式的短信
*+JACT:集中器密码，通信模式，集中器编号，域名，IP地址，端口号,APN,ID,服务器密码；
*
* 6B      9B          变长      变长    变长   变长    变长  变长 变长 变长（缺省6B）
* 例： +JACT:000000000,GPRS,szwzw.gicp.net,121.35.163.27,2227,CMNET,,222222;
* 
*/
struct SmSParam
{
	char password[20];
	char Mode[20];
	char SerialNo[20];
	char WebAddr[32];
	char IpAddr[20];
	char Port[20];
	char Apn[20];
	char UserId[20];
	char UserPassword[20];
};
struct SmSCharge
{
	unsigned char token[20];
};
struct SMS_DATA
{
	enum SMS_TYPE type;
	unsigned char data[200];
};
int UnPackSmsCmd(const char* sms,struct SMS_DATA* sdata);
void StrToIp(const char* StrIp,varip_t * Ip);
#endif /*_GPRS_DEV_H*/

