/**
* gprs_task.c GPRS����ͨ������
* 
* ����: zhuzhiqiang
* ����ʱ��: 2008-5-20
* ����޸�ʱ��: 2009-5-20
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "plat_include/event.h"
#include "plat_include/debug.h"
#include "plat_include/plat_defines.h"
#include "plat_include/reset.h"
#include "app_include/param/term.h"
#include "app_include/param/unique.h"
#include "app_include/uplink/svrnote.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
#include "app_include/uplink/svrcomm.h"
#include "app_include/uplink/keepalive.h"
//#include "uplink/tranfile_active.h"
#include "app_include/lib/align.h"
#include "app_include/param/unique.h"
#include "app_include/gprs/gprs_dev.h"
#include "plat_include/timer.h"
#include "hal_include/switch.h"
#include "plat_include/plat_gprs.h"
#include "app_include/param/operation_inner.h"
#include "app_include/uplink/svrmsg.h"
#include "app_include/uplink/keepalive.h"
#include "app_include/cenmet/forward.h"			
#include "plat_include/timeal.h"
#include "plat_include/plat_thread.h"


//����֪ͨ������վ:1��֪ͨ 

static char PhoneNo[30]={0};
/*
	˵��:��ǰ��״̬
	0	û�в��ųɹ�
	1	���ųɹ�,�ȴ�����
	2	û�����ӳɹ�
	3	���ӳɹ�
*/
static char ErrorCode = 0;
#include "mkdate.c"
int DoSms()
{
	static char ShortMsg[220];
	char pno[30]={0};
	memset(ShortMsg,0,sizeof ShortMsg);
	if(GprsRecvMessage((unsigned char*)ShortMsg,pno,sizeof(ShortMsg),sizeof(pno)) == SUCCEED)
	{
		struct SMS_DATA data;
		int rtn;
		memset(&data,0,sizeof data);
		rtn = UnPackSmsCmd(ShortMsg,&data);
		if(rtn == SUCCEED)
		{
			strcpy(PhoneNo,pno);
			if(data.type == SMS_PARAM)
			{
				varip_t ip;
				const char* ack = "+JACT: PARAM OK";
				unsigned port;
				struct SmSParam *param = (struct SmSParam *)data.data;
				StrToIp(param->IpAddr,&ip);
				port =  atoi(param->Port);
				memcpy(ParaTerm.svrip.ipmain,ip.uc,sizeof ip);
				ParaTerm.svrip.portmain = port;
				PrintLog(LOGTYPE_DEBUG,"Ip change by sms,new ip is:%d.%d.%d.%d:%d",
					ParaTerm.svrip.ipmain[0],
					ParaTerm.svrip.ipmain[1],
					ParaTerm.svrip.ipmain[2],
					ParaTerm.svrip.ipmain[3],
					ParaTerm.svrip.portmain);
				strcpy((char *)ParaTerm.svrip.apn,param->Apn);
				memset(ParaUni.web_addr,0,sizeof(ParaUni.web_addr));
				memcpy(ParaUni.web_addr,param->WebAddr,sizeof(param->WebAddr));
				GprsSendMessage((const unsigned char*)ack,PhoneNo,strlen(ack));
				SaveParaTerm();
				return 1;
			}else if(data.type == SMS_CHARGE)
			{
				//��ֵ
				#ifdef METER_MODULE
				DoRecharge(data.data);
				#endif
				return 1;
			}else if(data.type == SMS_ACTIVE)
			{
				//���Ż���
				sprintf(ShortMsg,"+JACT:ACTIVE OK,GPRS,%02x%02x%02x%02x,%d.%d.%d.%d,%s,%d,%s,%s,%d",
				ParaUni.addr_area[1],ParaUni.addr_area[0],
				ParaUni.addr_sn[1],ParaUni.addr_sn[0],
				ParaTerm.svrip.ipmain[0],
				ParaTerm.svrip.ipmain[1],
				ParaTerm.svrip.ipmain[2],
				ParaTerm.svrip.ipmain[3],
				ParaUni.web_addr,ParaTerm.svrip.portmain,
				ParaTerm.svrip.apn,MKTIME,GprsDevSigdBm);
				GprsSendMessage((const unsigned char*)ShortMsg,PhoneNo,strlen(ShortMsg));

				SvrConnectNote();
				return 1;
			}else if(data.type == SMS_ACTIVE_PARAM)
			{
				//���Ż���
				varip_t ip;
				unsigned port;
				struct SmSParam *param = (struct SmSParam *)data.data;
				StrToIp(param->IpAddr,&ip);
				port =  atoi(param->Port);
				memcpy(ParaTerm.svrip.ipmain,ip.uc,sizeof ip);
				ParaTerm.svrip.portmain = port;
				PrintLog(LOGTYPE_DEBUG,"Ip change by sms,new ip is:%d.%d.%d.%d:%d",
					ParaTerm.svrip.ipmain[0],
					ParaTerm.svrip.ipmain[1],
					ParaTerm.svrip.ipmain[2],
					ParaTerm.svrip.ipmain[3],
					ParaTerm.svrip.portmain);
				strcpy((char *)ParaTerm.svrip.apn,param->Apn);
				memset(ParaUni.web_addr,0,sizeof(ParaUni.web_addr));
				memcpy(ParaUni.web_addr,param->WebAddr,sizeof(param->WebAddr));

				sprintf(ShortMsg,"+JACT:ACTIVE OK,GPRS,%02x%02x%02x%02x,%d.%d.%d.%d,%s,%d,%s,%s",
				ParaUni.addr_area[1],ParaUni.addr_area[0],
				ParaUni.addr_sn[1],ParaUni.addr_sn[0],
				ParaTerm.svrip.ipmain[0],
				ParaTerm.svrip.ipmain[1],
				ParaTerm.svrip.ipmain[2],
				ParaTerm.svrip.ipmain[3],
				ParaUni.web_addr,ParaTerm.svrip.portmain,
				ParaTerm.svrip.apn,MKTIME);
				GprsSendMessage((const unsigned char*)ShortMsg,PhoneNo,strlen(ShortMsg));
				SvrConnectNote();
				SaveParaTerm();
				return 1;
			}else if(data.type == SMS_QUERY)
			{
				//��ѯʣ�����
				#ifdef METER_MODULE
				ReadMoney();
				#endif
				return 1;
			}else if(data.type == SMS_GETPARAM)
			{
				sprintf(ShortMsg,"+JACT:GETPARAM,GPRS,%02x%02x%02x%02x,%d.%d.%d.%d,%s,%d,%s,%s",
				ParaUni.addr_area[1],ParaUni.addr_area[0],
				ParaUni.addr_sn[1],ParaUni.addr_sn[0],
				ParaTerm.svrip.ipmain[0],
				ParaTerm.svrip.ipmain[1],
				ParaTerm.svrip.ipmain[2],
				ParaTerm.svrip.ipmain[3],
				ParaUni.web_addr,ParaTerm.svrip.portmain,
				ParaTerm.svrip.apn,MKTIME);
				SendSmsMessage(ShortMsg);
				return 1;
			}else if(data.type == SMS_GETERROR)
			{
				const char* ErrorMgs[]={"DIAL ON ERROR","DIAL ON OK","CONNECT FAILED ERROR","CONNECTED"};
				if(ErrorCode < sizeof(ErrorMgs)/sizeof(ErrorMgs[0]))
				{
					PrintLog(LOGTYPE_DEBUG,"Send Error Request\r\n");
					SendSmsMessage(ErrorMgs[ErrorCode]);	
				}
				return 1;
			}
		}
		else
		{
			if(rtn != -ERRSMSERROR)
			{
				SendSmsMessage("+JACT:SMS FAILED");
				PrintLog(LOGTYPE_DEBUG,"Short Message Error!\r\n");
				return 1;
			}
		}

	} 
	return 0;
}

static int GprsLogon(void)
{
	SetKeepAlive(KEEPALIVE_FLAG_LOGONFAIL);

	SvrCommLineState = LINESTAT_OFF;

	PrintLog(LOGTYPE_DEBUG,"��¼��վ...");
	if(UplinkLogon(UPLINKITF_GPRS)) {
		//ScreenDisplayAlarm("��¼��վʧ��");
		PrintLog(LOGTYPE_DEBUG,"��¼��վʧ��");
		GprsDisConnect();
		GprsDevLineState = GSTAT_LINE_OFF;
		return 1;
	}
	//ScreenDisplayAlarm("��¼��վ�ɹ�");
	PrintLog(LOGTYPE_DEBUG,"��¼��վ�ɹ�");

	GprsLoginState = 1;
	SetKeepAlive(KEEPALIVE_FLAG_LOGONOK);

	return 0;
}

static void GprsLogOff(void)
{
	GprsDisConnect();
	GprsDevLineState = GSTAT_LINE_OFF;//�Ͽ�TCP����
	SvrCommLineState = LINESTAT_OFF;//�Ͽ���վ����
}

static int GprsKeepAlive(void)
{
	static int cnt_connecterr = 0;
	static int server_sel = 0;
	static int cnt_logonerr = 0;
	static int wait_time = 50;
	//static int wait_connect = 1500;

	int rtn, i;

	PrintLog(LOGTYPE_SHORT, "gprs keep alive...\n");

	if(GprsDevLineState == GSTAT_DIAL_OFF||GprsDevLineState == GSTAT_LINE_OFF) {
		SvrCommLineState = LINESTAT_OFF;
	}
LINK_TEST:
	if(LINESTAT_ON == SvrCommLineState) //TCP�ɹ�
	{
		rtn = UplinkLinkTest(UPLINKITF_GPRS);
		if((UPRTN_FAIL == rtn) || (UPRTN_TIMEOUT == rtn)) {
			PrintLog(LOGTYPE_SHORT, "������...\n");
			SvrCommLineState = LINESTAT_OFF;//������
			GprsDisConnect();
		}
		else if(UPRTN_OK == rtn) {
			return 0;
		}
		else if(UPRTN_OKRCV == rtn){
			SvrMessageProc(UPLINKITF_GPRS);
			return 0;
		}else if(UPRTN_OKDLMS == rtn)
		{
			SvrMessageDlms(UPLINKITF_GPRS);
			return 0;			
		}else if(-ERRGPRS == rtn)
		{
			return -ERRGPRS;
		}
	}
 /*
	if(GSTAT_LINE_ON == GprsDevLineState) {
		if(!GprsLogon()) {
			cnt_logonerr = 0;
//			alarmled &= ~1;
			return 0;
		}
	}
  */
mark_continue:

	if(GSTAT_DIAL_ON != GprsDevDailState) 
	{
		if(GprsDail() == -ERRFAILED) 
			goto mark_fail;
#ifdef METER_MODULE	
		Signel_led(Signel_R,1);
#endif
		ErrorCode = 1;//���ųɹ�
	}
  
	//ScreenDisplayAlarm("������վ...");
	ErrorCode = 2;
	PrintLog(LOGTYPE_SHORT,"������վ...");

   	
	for(i=0; i<9; i++) 
	{
		SysClearCurrentTaskState();
		DoSms();
		{
			int w  = rand()%1500;
			Sleep(w);
		}
		if(!GprsConnect(server_sel)) 
		{
			
			if(!GprsLogon()) {
#ifdef METER_MODULE	
				Signel_led(Signel_G,1);
#endif
				cnt_logonerr = 0;
				SvrCommLineState = LINESTAT_ON;//��¼�ɹ���
				UpdateLineStateInit(1);
				ErrorCode = 3;
				wait_time = 20;
				goto LINK_TEST;
			//	return 0;
			}
		}
		server_sel++;
		if(server_sel == 3)
		{
			server_sel = 0;
		}
		{
			int w  = rand()%wait_time;
			Sleep(w);
			wait_time *= 10; 
		}
	}
	

	PrintLog(LOGTYPE_SHORT,"������վʧ��");
	GprsLoginState = 2;	//����ʧ��

//       DebugPrint(0,"cnt_logonerr=%d,alarmled=%x\n",cnt_logonerr,alarmled);
       cnt_logonerr++;
//	if(cnt_logonerr>5) alarmled |= 1;
	//if((cnt_logonerr%3) != 0) return 1;
	
	GprsDisConnect();
	GprsDailOff();
	GprsDevDailState = GSTAT_DIAL_OFF;

	//if(cnt_logonerr > 100) 
	{
		PrintLog(LOGTYPE_ALARM, "logon fail so much times..,reboot\n");
		SysReset();
		//SysRestart();
	}

	cnt_connecterr++;
	if(cnt_connecterr > 1) {
		cnt_connecterr = 0;
		GprsRestart();
	}

mark_fail:
	GprsDevDailState = GSTAT_DIAL_OFF;
	GprsDevLineState = GSTAT_LINE_OFF;
	SvrCommLineState = LINESTAT_OFF;
	GprsRestart();//add by lzq
	goto mark_continue;
}
#if 0
static unsigned char heartbeatflg = 0;

/**
* @brief �����ļ���ʱ��ʱ��
*/

static int CTimerHeartbeat(unsigned long arg)
{	
	static unsigned char bactive = 0;
		
	if(KeepAliveInPeriod()) 
	{//��������
		if(!bactive) 
		{
			bactive = 1;
			heartbeatflg = 1;
		}
		else if(!KeepAliveProc()) 
			heartbeatflg = 1;
	}
	else {
		bactive = 0;
	}
	
	return 0;
}


static void GprsActiveTask(void)
{
	unsigned long ev;
	int bactive;
	int reconnect = 0;
	int feedcnt = 0;

	//int qryampcnt = 0;
		
	UplinkClearState(UPLINKITF_GPRS);
	SvrCommLineState = LINESTAT_OFF;

	GprsDevInit();

	bactive = KeepAliveInPeriod();
	if(!GprsRestart() && bactive)
	{
		GprsCheckTime();
		GprsKeepAlive();
	}
	
	SysAddCTimer(1,CTimerHeartbeat,0);
	
	while(1) 
	{
		SvrCommPeekEvent(SVREV_NOTE|SVREV_SMS|SVREV_CSQ, &ev);
		if(ev&SVREV_NOTE)
		{
			if(!RefreshKeepAlive()) GprsKeepAlive();
			if(LINESTAT_ON == SvrCommLineState)
				if(SvrNoteProc(UPLINKITF_GPRS))
				{
					
				}
		}
		if(ev&SVREV_SMS)
		{
			 if(GprsDevDailState == GSTAT_DIAL_ON)
			 {
			 	  GprsSendMessage((unsigned char*)GetSmsMessage(),PhoneNo,strlen(GetSmsMessage()));
			 }
		} 
		if(ev&SVREV_CSQ)
		{
			GprsGetSig();
		}
		if(GprsDevLineState == GSTAT_LINE_OFF) 
		{
			if(0 == reconnect) 
			{
				SvrCommLineState = LINESTAT_OFF;
				GprsKeepAlive();
				reconnect = 1;
			}
		}
		if(LINESTAT_ON == SvrCommLineState) 
		{
			int rt;
			reconnect = 0;
			rt = UplinkRecvPkt(UPLINKITF_GPRS); 
			if(rt == PROTO_376)
			{
				SvrMessageProc(UPLINKITF_GPRS);
			}
			if(rt == PROTO_DLMS)
			{
				SvrMessageDlms(UPLINKITF_GPRS);
			}
			//SvrDwnActProc(UPLINKITF_GPRS);
		}
		if(heartbeatflg)
		{
			GprsKeepAlive();
			heartbeatflg = 0;
		}
		if(GprsDevDailState == GSTAT_DIAL_ON)
		{//
			DoSms();
		}

		feedcnt++;
		if(feedcnt > 300) 
		{
			feedcnt = 0;
			//SysClearCurrentTaskState();
			PrintLog(LOGTYPE_DEBUG,"gprs line state chk...\n");
			GprsLineState(); 
			GprsUpdSigAmp();
			PrintLog(LOGTYPE_DEBUG,"SvrCommLineState(%d) GprsDevLineState(%d)\n",SvrCommLineState,GprsDevLineState);
		}

		Sleep(5);
	}
}
#endif
#if 1
void ReTryConnect()
{
	while(1)
	{
		if(GprsDail() != -ERRFAILED)
		{
		#ifdef METER_MODULE	
			Signel_led(Signel_R,1);
		#endif
			ErrorCode = 1;//���ųɹ�
			if(LINESTAT_ON == SvrCommLineState)
			{
				SvrCommLineState = LINESTAT_OFF;
				GprsDevLineState = GSTAT_LINE_OFF; 
				if(GprsKeepAlive()==-ERRGPRS)
				{
					Sleep(10);
					continue;					
				}
				break;
			}
			else
				break;
		}
		Sleep(10);

	}
}
static sys_event_t Second = {0};
static sys_event_t Minute = {0};
static sys_event_t Beate = {0};
#define LINK_TEST		1<<0
#define CHECK_SIG		1<<1
#define CHECK_SMS		1<<2
#define UPLINK_DATA		1<<3
#define ONLINE_CHK		1<<4

static int CTimerSecond(unsigned long arg)
{	
	SysSendEvent(&Second,(unsigned long)-1);
	return 0;
}
static int CTimerMinute(unsigned long arg)
{	
	SysSendEvent(&Minute,(unsigned long)-1);
	return 0;
}
static int CTimerBeat(unsigned long arg)
{	
	SysSendEvent(&Beate,(unsigned long)-1);
	return 0;
}
extern int CheckThreadState;
static void GprsInactiveTask(void)
{
#define TIME_RESET		1800  // 3minute
	//@change later ����
	unsigned long ev,sv,mv,bv;
	int onlinetime, onlinemax;
	int BeatId;
//	static int timeout = -1; //����ʱ�����

	SysInitEvent(&Minute);
	SysInitEvent(&Second);

	UplinkClearState(UPLINKITF_GPRS);
	SvrCommLineState = LINESTAT_OFF;

	GprsDevInit();
	GprsRestart();
	onlinemax = (int)ParaTerm.uplink.timedown&0xff;

	onlinetime = 0;

	SysAddCTimer(1,CTimerSecond,0);
	SysAddCTimer(60,CTimerMinute,0);
	BeatId = SysAddCTimer(((int)(ParaTerm.tcom.cycka)&0xff)*60,CTimerBeat,0);
	if(LineStateInit() == 1)
	{
		SvrCommLineState = LINESTAT_ON;
		PrintLog(LOGTYPE_DEBUG,"��λ����Ҫ����\r\n");
	}

	if(ParaTerm.uplink.clientmode == 0)
	{
		PrintLog(LOGTYPE_DEBUG,"��������ģʽ\r\n");
		GprsKeepAlive();
	}
	else
	{
		PrintLog(LOGTYPE_DEBUG,"��������ģʽ\r\n");
		PrintLog(LOGTYPE_DEBUG,"clientmode:%d\r\n",ParaTerm.uplink.clientmode);
		PrintLog(LOGTYPE_DEBUG,"timedown:%d\r\n",ParaTerm.uplink.timedown);
	}

	while(1) 
	{
		if(GSTAT_DIAL_ON != GprsDevDailState) 
		{
			if(GprsDail() == -ERRFAILED)
			{
				Sleep(100); 
				continue;
			}
			ErrorCode = 1;//���ųɹ�
#ifdef METER_MODULE	
			Signel_led(Signel_R,1);
#endif
		}
		SvrCommPeekEvent(SVREV_NOTE|SVREV_SMS|SVREV_CSQ|SVREV_USER_SMS|SVREV_ACTIVE|SVREV_ALERM, &ev);
		//if(CheckinActivePeriod())
		{//�Ǹ߷���,��������
			if((ev&SVREV_NOTE)||(SvrCommPeekNoteid() != -1))
			{//��ʱ������
				PrintLog(LOGTYPE_DEBUG,"��ʱ������\r\n");
				if(GSTAT_DIAL_ON == GprsDevDailState) 
				{
					GprsKeepAlive();
					onlinetime = 0;
					//timeout = -1;
					SysClearCTimer(BeatId);
				}
			}
			if(ev&SVREV_ALERM)
			{
				if(LINESTAT_ON == SvrCommLineState)
				{
					if(SvrNoteProc(UPLINKITF_GPRS))
					{
						onlinetime = 0;
						//timeout = -1;
						SysClearCTimer(BeatId);						
					}
				}
			}
			if(ev&SVREV_ACTIVE)
			{
				PrintLog(LOGTYPE_DEBUG,"������...\r\n");
#ifdef METER_MODULE	
					Signel_led(Signel_R,1);
#endif
				 GprsLogOff();
				 GprsKeepAlive();
				 onlinetime = 0;
			}
		}
		if(ev&SVREV_SMS)//��Ҫ���Ͷ�Ϣ
		{
			PrintLog(LOGTYPE_DEBUG,"���Ͷ�Ϣ:%s,%s\r\n",PhoneNo,GetSmsMessage());
			GprsSendMessage((unsigned char*)GetSmsMessage(),PhoneNo,strlen(GetSmsMessage()));
		} 
		if(ev&SVREV_USER_SMS)
		{
			GprsSendMessage((unsigned char*)GetUserSmsMessage(),GetUserPhoneNumber(),strlen(GetUserSmsMessage()));
		}
   		if(ev&SVREV_NOTE) 
		{//����֪ͨ��
			if(LINESTAT_OFF == SvrCommLineState) 
			{
				GprsKeepAlive();
				onlinetime = 0;
				//timeout = -1;
				SysClearCTimer(BeatId);
			}
		}
		
		if(ev&SVREV_CSQ)
		{//�����ѯ�ź�����
			GprsGetSig();
		}
		SysWaitEvent(&Minute,0,CHECK_SIG|ONLINE_CHK,&mv);
		SysWaitEvent(&Second,0,CHECK_SMS|LINK_TEST|UPLINK_DATA,&sv);

		if(GprsDevDailState == GSTAT_DIAL_ON)
		{//���Ŵ���
			if(CHECK_SMS&sv) 
			{
				static int sms_timeout = 0;
				if(sms_timeout == 0)
				{
					DoSms();
					sms_timeout = 5;
				}
				else sms_timeout--;
			}
		}

		//GprsLineState();
		if(LINESTAT_ON == SvrCommLineState) 
		{
			int rt;
			if(UPLINK_DATA&sv) 
			{
				rt = UplinkRecvPkt(UPLINKITF_GPRS);
				if(rt == PROTO_376) 
				{
					onlinetime = 0;
					//timeout = -1;
					SysClearCTimer(BeatId);
					SvrMessageProc(UPLINKITF_GPRS);
	#ifdef METER_MODULE
					LedLongShining();
	#endif
				}
				if(rt == PROTO_DLMS) 
				{
					onlinetime = 0;
					//timeout = -1;
					SysClearCTimer(BeatId);
					SvrMessageDlms(UPLINKITF_GPRS);
	#ifdef METER_MODULE
					LedLongShining();
	#endif
				}
				if(rt == -ERRGPRS)
				{
				   ReTryConnect();
				   onlinetime = 0;
				   //timeout = -1;
				   SysClearCTimer(BeatId);
				}
			}
			SysWaitEvent(&Beate,0,LINK_TEST,&bv);
			if(LINK_TEST&bv) 
			{
				GprsKeepAlive();
			}

			if(ParaTerm.uplink.clientmode != 0)
			{
				if(ONLINE_CHK&mv)
				{
					onlinetime++;	
					
				}
				if(onlinetime >= onlinemax) //��ʱ����
				{
					//��ʱ����
					GprsLogOff();
					UpdateLineStateInit(0);
#ifdef METER_MODULE	
					Signel_led(Signel_R,1);
#endif
				}
			}
		}
//		else  GprsLogOff();

		
		Sleep(5);//0.1s
		if(CHECK_SIG&mv)
		{ 
			if(CheckThreadState)
			{
				PrintLog(LOGTYPE_DEBUG,"Stop Thread\r\n");
				while(1)Sleep(100);
			}
			SysClearCurrentTaskState();
			PrintLog(LOGTYPE_DEBUG,"gprs line state chk...\n");
			if( -ERRFAILED == GprsLineState()) //������
			{
#ifdef METER_MODULE	
			Signel_led(Signel_OFF,1);
#endif
				ErrorCode = 0;
				ReTryConnect();
				onlinetime = 0;	
			} 
			GprsUpdSigAmp();
			if(LINESTAT_ON == SvrCommLineState) 
			{
				if(GprsTcpState() == 0)
				{
#ifdef METER_MODULE	
					Signel_led(Signel_R,1);
#endif
					SvrCommLineState = LINESTAT_OFF; 
					GprsKeepAlive();
					onlinetime = 0;
				}
			}
			onlinemax = (int)ParaTerm.uplink.timedown&0xff;
			if(onlinemax == 0)
				onlinemax = 10;
			PrintLog(LOGTYPE_DEBUG,"TIME:%s\r\n",UTimeFormat(UTimeReadCurrent()));
			PrintLog(LOGTYPE_DEBUG,"SvrCommLineState(%d) GprsDevLineState(%d)\r\n",SvrCommLineState,GprsDevLineState);
		}
	}
}
#endif

void GprsTask(void)
{
	//unsigned char dev;

//	dev = ReadRightModuleNo();
	//PrintLog(LOGTYPE_DEBUG,"Module No = %d\n", dev);
		
 GprsInactiveTask();
	
//	GprsActiveTask();
}

void SendSmsTest(char* msg,char* ph)
{
	strcpy(PhoneNo,ph);
	SendSmsMessage(msg);
}

