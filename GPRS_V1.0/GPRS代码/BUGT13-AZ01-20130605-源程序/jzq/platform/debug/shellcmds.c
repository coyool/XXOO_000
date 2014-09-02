 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <stdint.h>

 #include "plat_include/timeal.h"
 #include "plat_include/shellcmd.h"
 #include "hal_include/rtc.h"
 #include "plat_include/timer.h"
#include "app_include/gprs/gprs_dev.h"
#include "app_include/param/term.h"
#include "app_include/param/operation_inner.h"
#include "plat_include/func.h"
#include "plat_include/plat_gprs.h"
#include "app_include/uplink/svrcomm.h"
#include "plat_include/reset.h"
#include "plat_include/debug.h"
#include "plat_include/ymodem.h"
#include "app_include/uplink/svrmsg.h"

static int shell_showtime(int argc, char *argv[])
{
	sysclock_t clock;
	const char *pstr;

	SysClockRead(&clock);
	pstr = SysClockFormat(&clock);
	PrintLog(LOGTYPE_DEBUG,"当前时间: %s\n", pstr);
	return 0;
}
DECLARE_SHELL_CMD("time", shell_showtime, "显示当前时间");

static int shell_showclock(int argc, char *argv[])
{
	sysclock_t clock;
	const char *pstr;
	struct sysclock extclock;

	if(ExtClockRead(&extclock)) {
		PrintLog(LOGTYPE_DEBUG,"read ext clock fail\n");
		return 1;
	}

	clock.year = extclock.year;
	clock.month = extclock.month;
	clock.day = extclock.day;
	clock.hour = extclock.hour;
	clock.minute = extclock.minute;
	clock.second = extclock.second;
	clock.week = extclock.week;

	SysClockRead(&clock);
	pstr = SysClockFormat(&clock);
	PrintLog(LOGTYPE_DEBUG,"当前时间: %s\n", pstr);
	return 0;
}
DECLARE_SHELL_CMD("clock", shell_showclock, "显示当前实时时钟");
void ds3231_read_ymdwhms(unsigned char *str);
static int shell_settime(int argc, char *argv[])
{
	int i;
	sysclock_t clock;
	utime_t utime, utimecur;
	struct sysclock extclock;

	if(argc != 7) return 1;

	i = atoi(argv[1]);
	if(i < 0 || i > 99) {
		PrintLog(LOGTYPE_DEBUG,"invalid year\n");
		return 1;
	}
	clock.year = i;

	i = atoi(argv[2]);
	if(i < 1 || i > 12) {
		PrintLog(LOGTYPE_DEBUG,"invalid month\n");
		return 1;
	}
	clock.month = i;

	i = atoi(argv[3]);
	if(i < 1 || i > 31) {
		PrintLog(LOGTYPE_DEBUG,"invalid day\n");
		return 1;
	}
	clock.day = i;

	i = atoi(argv[4]);
	if(i < 0 || i > 23) {
		PrintLog(LOGTYPE_DEBUG,"invalid hour\n");
		return 1;
	}
	clock.hour = i;

	i = atoi(argv[5]);
	if(i < 0 || i > 59) {
		PrintLog(LOGTYPE_DEBUG,"invalid minute\n");
		return 1;
	}
	clock.minute = i;

	i = atoi(argv[6]);
	if(i < 0 || i > 59) {
		PrintLog(LOGTYPE_DEBUG,"invalid second\n");
		return 1;
	}
	clock.second = i;

	utimecur = UTimeReadCurrent();
	utime = SysClockToUTime(&clock);

	PrintLog(LOGTYPE_DEBUG,"set sys clock to %s\n", SysClockFormat(&clock));
	SysClockSet(&clock);
	SysClockRead(&clock);
	PrintLog(LOGTYPE_DEBUG,"set ext clock to %s\n", SysClockFormat(&clock));

	extclock.year = clock.year;
	extclock.month = clock.month;
	extclock.day = clock.day;
	extclock.hour = clock.hour;
	extclock.minute = clock.minute;
	extclock.second = clock.second;
	extclock.week = clock.week;

	sysclock_set(&extclock);

	if(utimecur > utime) i = utimecur - utime;
	else i = utime - utimecur;

	if(i > 300){
		SysRecalAllRTimer();
//		UpdateAllNodeSndTime();
	}

	PrintLog(LOGTYPE_DEBUG,"设置终端时间成功\n");
	return 0;
}
DECLARE_SHELL_CMD("settime", shell_settime, "设置终端时间");

static int SetIP0(int argc, char *argv[])
{
	varip_t ip;
	if(argc != 2) return 1;
	StrToIp(argv[1],&ip);
	memcpy(ParaTerm.svrip.ipmain,ip.uc,sizeof ip);
	PrintLog(LOGTYPE_DEBUG,"new main ip is:%d.%d.%d.%d\r\n",
		ParaTerm.svrip.ipmain[0],
		ParaTerm.svrip.ipmain[1],
		ParaTerm.svrip.ipmain[2],
		ParaTerm.svrip.ipmain[3]);
	SaveParaTerm();
	return 0;
}
DECLARE_SHELL_CMD("setip0", SetIP0, "Setup main IP Address");

static int SetIP1(int argc, char *argv[])
{
	varip_t ip;
	if(argc != 2) return 1;
	StrToIp(argv[1],&ip);
	memcpy(ParaTerm.svrip.ipbakup,ip.uc,sizeof ip);
	PrintLog(LOGTYPE_DEBUG,"new backup ip is:%d.%d.%d.%d\r\n",
		ParaTerm.svrip.ipbakup[0],
		ParaTerm.svrip.ipbakup[1],
		ParaTerm.svrip.ipbakup[2],
		ParaTerm.svrip.ipbakup[3]);
	SaveParaTerm();
	return 0;
}
DECLARE_SHELL_CMD("setip1", SetIP1, "Setup backup IP Address");

static int GetIP_Port(int argc, char *argv[])
{
	PrintLog(LOGTYPE_DEBUG,"main ip is:%d.%d.%d.%d\r\n",
		ParaTerm.svrip.ipmain[0],
		ParaTerm.svrip.ipmain[1],
		ParaTerm.svrip.ipmain[2],
		ParaTerm.svrip.ipmain[3]);
	PrintLog(LOGTYPE_DEBUG,"main port is:%d\r\n",
		ParaTerm.svrip.portmain);

	PrintLog(LOGTYPE_DEBUG,"backup ip is:%d.%d.%d.%d\r\n",
		ParaTerm.svrip.ipbakup[0],
		ParaTerm.svrip.ipbakup[1],
		ParaTerm.svrip.ipbakup[2],
		ParaTerm.svrip.ipbakup[3]);
	PrintLog(LOGTYPE_DEBUG,"backup port is:%d\r\n",
		ParaTerm.svrip.portbakup);
		PrintLog(LOGTYPE_DEBUG,"Apn is:%s\r\n",
		ParaTerm.svrip.apn);
	return 0;
}
DECLARE_SHELL_CMD("getip", GetIP_Port, "Print IP Address And Port");
static int SetWebAddress(int argc, char *argv[])
{
	if(argc != 2) return 1;
	strncpy((char*)ParaUni.web_addr,argv[1],sizeof(ParaUni.web_addr));
	PrintLog(LOGTYPE_DEBUG,"new web address:%s\r\n",ParaUni.web_addr);
	SaveParaTerm();
	return 0;
}
DECLARE_SHELL_CMD("setweb", SetWebAddress, "Set Web Address");

static int SetPort0(int argc, char *argv[])
{
	if(argc != 2) return 1;
	ParaTerm.svrip.portmain = atoi(argv[1]);
	PrintLog(LOGTYPE_DEBUG,"new main port is:%d\r\n",
		ParaTerm.svrip.portmain);
	SaveParaTerm();
	return 0;
}
DECLARE_SHELL_CMD("setport0", SetPort0, "Setup main IP port");

static int SetPort1(int argc, char *argv[])
{
	if(argc != 2) return 1;
	ParaTerm.svrip.portbakup = atoi(argv[1]);
	PrintLog(LOGTYPE_DEBUG,"new backup port is:%d\r\n",
		ParaTerm.svrip.portbakup);
	SaveParaTerm();
	return 0;
}
DECLARE_SHELL_CMD("setport1", SetPort1, "Setup backup IP port");

static int SetApn(int argc, char *argv[])
{
	if(argc != 2) return 1;
	strncpy(ParaTerm.svrip.apn,argv[1],sizeof(ParaTerm.svrip.apn));
	PrintLog(LOGTYPE_DEBUG,"new Apn is:%s\r\n",
		ParaTerm.svrip.apn);
	SaveParaTerm();
	return 0;
}
DECLARE_SHELL_CMD("setapn", SetApn, "Setup Apn");

static int SetSn(int argc, char *argv[])
{
//	unsigned int sn = 0;
	if(argc != 2) return 1;
	string_to_bcd((unsigned int*)ParaUni.addr_area,(unsigned char*)argv[1]);
	PrintLog(LOGTYPE_DEBUG,"A1:%02X%02X  A2:%02X%02X\r\n",
		ParaUni.addr_area[0],
		ParaUni.addr_area[1],
		ParaUni.addr_sn[0],
		ParaUni.addr_sn[1]);
	SaveParaTerm();
	return 0;
}
DECLARE_SHELL_CMD("setsn", SetSn, "设置表号(A2A1) 例如:00010755");
static int GetSn(int argc, char *argv[])
{
	PrintLog(LOGTYPE_DEBUG,"A1:%02X%02X  A2:%02X%02X\r\n",
		ParaUni.addr_area[1],
		ParaUni.addr_area[0],
		ParaUni.addr_sn[1],
		ParaUni.addr_sn[0]);
	return 0;
}
DECLARE_SHELL_CMD("getsn", GetSn, "打印表号");
static int GetCsq(int argc, char *argv[])
{
    CallCsq();
	return 0;
}
DECLARE_SHELL_CMD("getcsq", GetCsq, "打印信号强度");
static int ReBoot(int argc, char *argv[])
{
    SysRestart();
	return 0;
}
DECLARE_SHELL_CMD("reboot", ReBoot, "复位系统");
extern unsigned char debug_sms[220];
extern unsigned char debug_sms_len;
extern unsigned char debug_sms_rv;
static int shell_sms_rv(int argc, char *argv[])
{
	if(argc < 2)
		return 1;
	debug_sms_rv = 1;
	strcpy((char*)debug_sms,argv[1]);
	debug_sms_len = strlen(argv[1]);	
	PrintLog(LOGTYPE_DEBUG,"Send Sms:%s\r\n",debug_sms);
	return 1;
}
DECLARE_SHELL_CMD("sms_rv", shell_sms_rv, "短信接受测试");
void SendSmsTest(char* msg,char* ph);
static int shell_sms_sd(int argc, char *argv[])
{
	if(argc < 2)
		return 1;
	PrintLog(LOGTYPE_DEBUG,"Send Sms Test.\r\n");
	SendSmsTest(argv[1],"+8613714325180");
	return 1;
}
DECLARE_SHELL_CMD("sms_sd", shell_sms_sd, "短信发送测试");
#ifdef METER_MODULE
extern short GprsDevSigdBm;
extern unsigned char shining_led_state;
static int shell_shining(int argc, char *argv[])
{
	if(argc < 2)
		return 1;
	shining_led_state = 1;
	GprsDevSigdBm = atoi(argv[1]);
	return 1;
}
DECLARE_SHELL_CMD("shine", shell_shining, "闪烁测试");
#endif
extern unsigned char logType;
static int shell_ymodem(int argc, char *argv[])
{
	unsigned char old = logType;
	int32_t rt;
	logType = 0;
	Sleep(50);
	rt = Ymodem_Receive((unsigned char*)data_rwbuf);
	logType	= old;
	PrintLog(LOGTYPE_DEBUG,"Ymodem_Receive return %d\r\n",rt);
	SysRestart();
	return 1;
}
DECLARE_SHELL_CMD("ymodem", shell_ymodem, "升级程序");
static int shell_log(int argc,char* argv[])
{
	if(argc < 2)
		return 1;
	if(strcmp(argv[1],"on")==0)
		logType = LOGTYPE_DEBUG;
	if(strcmp(argv[1],"off")==0)		
		logType = LOGTYPE_CLOSE;
	return 1;
}
DECLARE_SHELL_CMD("log", shell_log, "打开关闭调试输出");
int CheckThreadState = 0;
static int shell_TestCheckThreadState(int argc,char* argv[])
{
	CheckThreadState = 1;
	return 1;	
}
DECLARE_SHELL_CMD("stopthread", shell_TestCheckThreadState, "暂停线程");
void shell_cms_init(void)
{
	INIT_SHELL_CMD(shell_showtime);
	INIT_SHELL_CMD(shell_showclock);
	INIT_SHELL_CMD(shell_settime);
	INIT_SHELL_CMD(SetIP0);
	INIT_SHELL_CMD(SetIP1);
	INIT_SHELL_CMD(GetIP_Port);
	INIT_SHELL_CMD(SetPort0);
	INIT_SHELL_CMD(SetPort1);
	INIT_SHELL_CMD(SetApn);
	INIT_SHELL_CMD(SetSn);
	INIT_SHELL_CMD(GetSn);
	INIT_SHELL_CMD(GetCsq);
	INIT_SHELL_CMD(ReBoot);
	INIT_SHELL_CMD(SetWebAddress);
	INIT_SHELL_CMD(shell_sms_rv);
	INIT_SHELL_CMD(shell_sms_sd);
	INIT_SHELL_CMD(shell_ymodem);
	INIT_SHELL_CMD(shell_log);
#ifdef METER_MODULE
	INIT_SHELL_CMD(shell_shining);
#endif
	INIT_SHELL_CMD(shell_TestCheckThreadState);

}
