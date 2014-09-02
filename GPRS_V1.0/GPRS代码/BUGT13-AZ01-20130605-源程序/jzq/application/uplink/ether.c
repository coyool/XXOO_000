/**
* ether.c -- 以太网通信(客户端模式)
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-19
* 最后修改时间: 2009-5-19
*/

//#include <stdio.h>
#include <string.h>

#include "plat_include/debug.h"
#include "lib/align.h"
#include "param/term.h"
#include "uplink/svrnote.h"
#include "uplink_pkt.h"
#include "uplink_dl.h"
#include "svrcomm.h"
#include "keepalive.h"
#include "gprs/gprs_dev.h"
#include "plat_include/switch.h"

#define ch_server   0
#define ch_client   1
/**
* @brief 连接到服务器
* @return 成功0, 否则失败
*/
static int EtherConnect(void)
{
	static int ServerSel = 0;					   
	static int CountFail = 0;
	char count = 0;
	struct ethernet_state ether_state;
//@later 
	if(ParaTerm.uplink.proto);  //UDP
	else ; //TCP

	if(0 == ServerSel) {
		PrintLog(LOGTYPE_SHORT, "connect to %d.%d.%d.%d, %d...\n",
					ParaTerm.svrip.ipmain[0],ParaTerm.svrip.ipmain[1],ParaTerm.svrip.ipmain[2],ParaTerm.svrip.ipmain[3],ParaTerm.svrip.portmain);
		//__GlobalUnlock();
		ethernet_setup_tcpclient(ParaTerm.svrip.ipmain,ParaTerm.svrip.portmain);
		//__GlobalLock();
	}
	else {
		PrintLog(LOGTYPE_SHORT, "connect to %d.%d.%d.%d, %d...\n",
					ParaTerm.svrip.ipbakup[0],ParaTerm.svrip.ipbakup[1],ParaTerm.svrip.ipbakup[2],ParaTerm.svrip.ipbakup[3],ParaTerm.svrip.portbakup);
		//__GlobalUnlock();
		ethernet_setup_tcpclient(ParaTerm.svrip.ipbakup,ParaTerm.svrip.portbakup);
		//__GlobalLock();
	}
	
	count = 0;
	while(count<5)
	{
		count++;
		Sleep(100);
		ethernet_conncet_status(&ether_state);
		
		PrintLog(LOGTYPE_SHORT,"ether_state(%d)\n",ether_state.lclient);
		
		if(ether_state.lclient == CCONNECTED)
		{
			PrintLog(LOGTYPE_SHORT, "connect succeed.\r\n");
			CountFail = 0;
			
			return 0;
		}
	}

	PrintLog(LOGTYPE_SHORT, "connect failed.\n");
	
	CountFail++;
	if(CountFail > 10) {
		CountFail = 0;

		if(0 == ServerSel) {
			if(ParaTerm.svrip.ipbakup[0]) ServerSel = 1;
		}
		else {
			ServerSel = 0;
		}
	}

	return 1;
}

/**
* @brief 与服务器断开连接
* @return 成功0, 否则失败
*/
static int EtherDisconnect(void)
{
	int count = 0;
	struct ethernet_state ether_state;

	PrintLog(LOGTYPE_DEBUG,"****EtherDisconnect \n");
	ethernet_close_connect(ch_client);
	while(count<5)
	{
		count++;
		Sleep(100);
		ethernet_conncet_status(&ether_state);
		if(ether_state.lclient == CCLOSED)
		{
			svr_lineled(0);
			return 0;
		}
	}
	
	return 1;
}

/**
* @brief 登陆服务器
* @return 成功0, 否则失败
*/
static int EtherLogon(void)
{
	SetKeepAlive(KEEPALIVE_FLAG_LOGONFAIL);

	SvrCommLineState = LINESTAT_OFF;

	if(UplinkLogon(UPLINKITF_ETHER)) {
		EtherDisconnect();
		return 1;
	}
	
	svr_lineled(1);
	SvrCommLineState = LINESTAT_ON;
	SetKeepAlive(KEEPALIVE_FLAG_LOGONOK);
	return 0;
}

/**
* @brief 保持与服务器的连接
* @return 成功0, 否则失败
*/
static int EtherKeepAlive(void)
{
	int rtn;

	PrintLog(LOGTYPE_SHORT, "ether keep alive...\n");

	if(LINESTAT_ON == SvrCommLineState) {
		rtn = UplinkLinkTest(UPLINKITF_ETHER);
		if((UPRTN_FAIL == rtn) || (UPRTN_TIMEOUT == rtn)) {
			SvrCommLineState = LINESTAT_OFF;
			EtherDisconnect();
			svr_lineled(0);
		}
		else if(UPRTN_OK == rtn) {
			return 0;
		}
		else {
			SvrMessageProc(UPLINKITF_ETHER);
			return 0;
		}
	}

	if(!EtherConnect()) {
		Sleep(50);
		return(EtherLogon());
	}
	
 	EtherDisconnect();
	SvrCommLineState = LINESTAT_OFF;
	return 1;
}

/**
* @brief 以太网通信任务
*/
void EtherTask(void)
{
	unsigned long ev;
	int feedcnt = 0;
	
	UplinkClearState(UPLINKITF_ETHER);
	SvrCommLineState = LINESTAT_OFF;
	
	while(2 == ParaTerm.uplink.mode) Sleep(200);  //server mode

	Sleep(1000); //延时10s，以等待网络初始化(SetParaNetAddr)完成。
	EtherKeepAlive();

	while(1) {
		SvrCommPeekEvent(SVREV_NOTE, &ev);

		if(ev&SVREV_NOTE) {
			SysClearCurrentTaskState();
			if(!RefreshKeepAlive()) EtherKeepAlive();
		}

		if(LINESTAT_ON == SvrCommLineState) {
			if(!UplinkRecvPkt(UPLINKITF_ETHER)) {
				Sleep(20);
				SvrMessageProc(UPLINKITF_ETHER);
			}

			//SvrDwnActProc(UPLINKITF_ETHER);
		}

		if(!KeepAliveProc()) EtherKeepAlive();

		Sleep(10);
		feedcnt++;
		if(feedcnt > 50) {
			feedcnt = 0;
			SysClearCurrentTaskState();
		}
	}
}

/**
* @brief 从以太网通信接口读取一个字节
* @param buf 返回字符指针
* @return 成功0, 否则失败
*/
int EtherGetChar(unsigned char *buf)
{
	return ethernet_read(ch_client,buf,1);
}

/**
* @brief 向以太网通信接口发送数据
* @param buf 发送缓存区指针
* @param len 缓存区长度
* @return 成功0, 否则失败
*/
int EtherRawSend(const unsigned char *buf, int len)
{
	if(ethernet_write(ch_client,buf,len)!=len) {
		DebugPrint(1, "send fail\n");
		goto mark_failend;
	}

	return 0;

mark_failend:
	PrintLog(LOGTYPE_SHORT, "connection corrupted.\n");
	EtherDisconnect();
	AssertLog(1," ");

	SvrCommLineState = LINESTAT_OFF;
	return 1;
}


/**
* @brief 以太网通信任务(TCP)
*/

#define time_10s 100

int EthSvrTcpInit(void)
{

	int count = 0;
	struct ethernet_state ether_state;
	
	SetParaNetAddr();

	while(count<5)
	{
		ethernet_setup_tcpserver(ParaTerm.termip.portlisten);
		count++;
		Sleep(100);
		ethernet_conncet_status(&ether_state);
		if(ether_state.lserver == SLISTENING)
		{
			PrintLog(LOGTYPE_SHORT,"ether server listen at tcp port %d...\n", ParaTerm.termip.portlisten);
			return 0;
		}
	}

	PrintLog(LOGTYPE_SHORT,"ether server listen at tcp port %d failed...\n", ParaTerm.termip.portlisten);
	
	return 1;
}

static unsigned short count_nodata = 0;

void EthSvrTcpMonitor(void)
{
	static unsigned char count_time = time_10s;
	static unsigned char count_svrchk = 0;
	static struct ethernet_state ether_state;
	
	count_time++;
	if(count_time>time_10s) 
	{
		ethernet_conncet_status(&ether_state);
		count_time = 0;
	}
	
	if(ether_state.lserver == SLISTENING)
	{
		count_svrchk = 0;
		if(ether_state.rclient == CCONNECTED)
		{
			if(!UplinkRecvPkt(UPLINKITF_ETHERSVR)) {
				SvrMessageProc(UPLINKITF_ETHERSVR);
			}

			count_nodata++;
			if(count_nodata >= 1200) {
				count_nodata = 0;
				PrintLog(LOGTYPE_SHORT, "tcp svr timeout, disconnect...\n");
				ethernet_close_connect(ch_server);
			}
		}
	}
	else
	{
		count_svrchk ++;
		if(count_svrchk>180)
		{
			ethernet_close_tcpserver();
			ethernet_setup_tcpserver(ParaTerm.termip.portlisten);
			EthSvrTcpInit();
			count_svrchk = 0;
		}
	}

}



/**
* @brief 从以太网通信接口读取一个字节
* @param buf 返回字符指针
* @return 成功0, 否则失败
*/
int EtherSvrGetChar(unsigned char *buf)
{
	int rt;

	rt = ethernet_read(ch_server,buf,1);
	if(rt==1) count_nodata=0;

	return rt;
}

/**
* @brief 向以太网通信接口发送数据
* @param buf 发送缓存区指针
* @param len 缓存区长度
* @return 成功0, 否则失败
*/
int EtherSvrRawSend(const unsigned char *buf, int len)
{
	if(ethernet_write(ch_server,buf,len)!=len) {
		DebugPrint(1, "send fail\r\n");
		goto mark_failend;
	}

	return 0;

mark_failend:
	ethernet_close_connect(ch_server);
	PrintLog(LOGTYPE_SHORT, "tcp svr send err, disconnect...\n");
	return 1;
}










