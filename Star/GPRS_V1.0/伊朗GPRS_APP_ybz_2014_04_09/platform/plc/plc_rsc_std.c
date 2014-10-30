/**
* plc_rsc_std.c -- 瑞斯康载波通信模块
* 
* 作者: hulijun
* 创建时间: 2012-3-16
* 最后修改时间: 2012-3-16
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "app_include/lib/align.h"
#include "app_include/lib/bcd.h"

#include "plat_include/plc_3762.h"
#include "plat_include/debug.h"
#include "hal_include/switch.h"
#include "plat_include/event.h"
#include "plat_include/app_proto.h"
#include "plat_include/sys_config.h"
#include "plat_include/plat_thread.h"

#ifdef DEFINE_PLCOMM

#define MAX_PLCOMM_RETRY		3
#define PLCOMM_TIMEOUT			30
//#define PLRSC_READMET_TIMEOUT		300
#define PLRSC_READMET_TIMEOUT		60


/**
* @brief 接收特定数据帧
* @param pmodule 载波控制模块指针
* @param waitafn 指定回应的AFN
* @param waitdt 指定回应的DT
* @param timeout 超时时间(100ms)
* @return 成功返回0, 失败返回负数(参见错误码PLCERR_XXX)
*/
int PlRsc3762RecvPktTimeout(struct amr_module_t *pmodule, unsigned char waitafn, unsigned short waitdt, int timeout)
{
	int times;
	unsigned short dt;
	unsigned char afn;
	plc3762_pkt_t *prcv = (plc3762_pkt_t *)pmodule->link_recv_buf;

	for(times=0; times<timeout; times++) {
		Sleep(5);//100ms

		if(!Pl3762RecvPkt(pmodule)) {
			afn = prcv->afn;
			dt = (((unsigned short)(prcv->dt[1]))<<8) + (unsigned short)(prcv->dt[0]);
			if(afn == PL3762_AFN_ACTIVESND){
				Pl3762SendEchoPkt(pmodule, 0);
				continue;
			}
			if(afn == PL3762_AFN_ECHO) {
				if(dt == 0x0001 && waitafn == 0) return 0;
				else return PLCERR_INVALID;
			}
			else if(waitafn == afn && waitdt == dt) return 0;
			#if 0
			else if(prcv->ctrl & 0x40) {
				//PlMessageProc(*pdest, *pprcv);
				//return PLCERR_INVALID;
			}
			#endif
		}
		
	}

	return PLCERR_TIMEOUT;
}

static void PlRscStdCtrlCmdProc(struct amr_module_t *pmodule)
{
	unsigned char ctrlcmd;
	static const char *flagname[] = {"restart", "pause", "resume", "reset"};

	ctrlcmd = pmodule->event_cmd_buf[0];

	if(ctrlcmd > PLC_AMRCTRL_RESETMODULE){
		PrintLog(LOGTYPE_DOWNLINK, "PlRscStd AmrEvent_CtrlCmd Error(%d)!...\n", ctrlcmd);
		goto mark_fail;
	}
	PrintLog(LOGTYPE_DOWNLINK, "PlRscStd AmrEvent_CtrlCmd : %s...\n", flagname[ctrlcmd-1]);
	
	switch(ctrlcmd){
	case PLC_AMRCTRL_RESTART:
		pmodule->flag_stoped = 0;
		pmodule->flag_reseting = 1;
		break;
	case PLC_AMRCTRL_PAUSE:
		pmodule->flag_stoped = 1;
		break;
	case PLC_AMRCTRL_RESUME:
  		pmodule->flag_stoped = 0;
		break;
	case PLC_AMRCTRL_RESETMODULE:
		pmodule->flag_stoped = 0;
		pmodule->flag_sync = 2;
		break;
	default:
		break;
	}
	
mark_fail:
	pmodule->event_echortn = 0;
	pmodule->event_echoed = 1;
	return;
}
//修改成DLMS透传格式的数据帧
#ifdef TEST_PLC
extern int GetRskTimeout(void);
#endif
static void PlRscStdForwardProc(struct amr_module_t *pmodule)
{
	plc3762_pkt_t *pkt = (plc3762_pkt_t *)pmodule->link_send_buf;
	plc3762_pkt_t *prcv = (plc3762_pkt_t *)pmodule->link_recv_buf;
	plc_dest_t dest;
	#ifdef TEST_PLC
	int timeout = GetRskTimeout();
	#else
	int timeout = PLRSC_READMET_TIMEOUT;
	#endif
	unsigned char *pdata;
	int i;
	
	pdata = pmodule->event_cmd_buf;

	memset((unsigned char*)&dest, 0, sizeof(plc_dest_t));


	smallcpy(dest.dest, pmodule->MeterAddr, 6);
	
	dest.metid = 1;	//没用
	dest.proto = pmodule->app_proto;//透传DLMS:0,07:2
	dest.portcfg = pmodule->port;
	dest.route.level = 0;
	dest.route.phase = 0;
	dest.src[0] = pmodule->module_id[0];
	dest.src[1] = pmodule->module_id[1];
	dest.src[2] = pmodule->module_id[2];
	dest.src[3] = pmodule->module_id[3];
	dest.src[4] = pmodule->module_id[4];
	dest.src[5] = pmodule->module_id[5];

	pkt->afn = PL3762_AFN_FORWARD;
	FN_DT(1, pkt->dt);
	pkt->data[0] = dest.proto;
	pkt->data[1] = (unsigned char)(pmodule->event_sendlen);
	memcpy(pkt->data+2, pdata, pmodule->event_sendlen);
	pkt->len = pmodule->event_sendlen + 2;

	pkt->ctrl = 0x41;
	if(Pl3762SendPkt(pmodule, &dest, 0)) goto mark_fail;
	//if(PlRsc3762RecvPktTimeout(pmodule, PL3762_AFN_FORWARD, 1, PLRSC_READMET_TIMEOUT)) goto mark_fail;
	if(PlRsc3762RecvPktTimeout(pmodule, PL3762_AFN_FORWARD, 1, timeout)) goto mark_fail;
	
	for(i=0; i<6; i++) 
	{
		if(pmodule->link_recv_dest.src[i] != dest.dest[i]) goto mark_fail;
	}
	if(prcv->data[0] != dest.proto)  goto mark_fail;
	if(prcv->len != (prcv->data[1]+2))  goto mark_fail;
	i = prcv->data[1] & 0xff;
	if(i <= 0) goto mark_fail;

	smallcpy(pmodule->event_echo_buf, prcv->data + 2, i);

	pmodule->event_echortn = i;
	pmodule->event_echoed = 1;
	return;
	
mark_fail:
	pmodule->event_echortn = 0;
	pmodule->event_echoed = 1;
	return;
}

static void PlRscStdEventProc(struct amr_module_t *pmodule, unsigned long event)
{
	if(event & AMREVENT_READMET) {
		PlRscStdForwardProc(pmodule);
	}
	else if(event & AMREVENT_CHANGEMET) {
		PrintLog(LOGTYPE_DOWNLINK, "PlRscStd AmrEvent Changemet...\n");
		pmodule->flag_sync = 1;
	}
	else if(event & AMREVENT_CTRLCMD){
		PlRscStdCtrlCmdProc(pmodule);
	}
	else if(event & AMREVENT_DIRECTFWD) {
		Pl3762DirectForwardProc(pmodule);
	}
	else{	//防止不支持的事件循环等待
		pmodule->event_echortn = 0;
		pmodule->event_echoed = 1;
	}
}

/**
* @brief 模块初始化
*/
static unsigned char main_addr[6]={0x30,0x30,0x30,0x30,0x30,0x31};
static void MakeMainAddr(void)
{
	unsigned short iaddr = 0;
	
	iaddr = MAKE_SHORT(ParaUni.addr_sn);
	main_addr[0] = ((iaddr%100)/10 << 4) + iaddr%10;
	iaddr /= 100;
	main_addr[1] = ((iaddr%100)/10 << 4) + iaddr%10;
	iaddr /= 100;
	main_addr[2] = ((iaddr%100)/10 << 4) + iaddr%10;
}
static void PlRscStdModuleInit(struct amr_module_t *pmodule)
{
	PrintLog(LOGTYPE_DOWNLINK, "Reset PLC_Rsc module...\n");
	//PlatResetPlcModule();
	MakeMainAddr();
	PlcRest();
	Sleep(40);
	Pl3762ResetModule(pmodule,1);
	Sleep(10);
	Pl3762ResetModule(pmodule,2);
	Sleep(10);
	Pl3762ResetModule(pmodule,3);
	Sleep(10);
	memcpy(pmodule->module_id,main_addr,6);
	Pl3762SetModuleAddr(pmodule,main_addr);
	Sleep(40);
	Pl3762ReadModuleManufactor(pmodule);
	Sleep(10);
	Pl3762ReadModuleAddr(pmodule);
	Sleep(10);
}

/**
* @brief 清除抄表状态
* @param arg 端口号
*/ 
#if 0
static void PlRscStdClearReadState(struct amr_module_t *pmodule)
{
//	int i;

	pmodule->cur_metid = 0;
	
	for(i=0; i<MAX_METER; i++) {
		if(0 == pmodule->met_map[i].attr) continue;
		pmodule->met_map[i].reserv = 0;
	}

}
#endif
/**
* @brief 生成目标地址(DAU)
* @param *pmodule 控制块
* @param dest 返回目标地址变量指针
*/ 
#if 0
static void PlRscStdMakeDest(struct amr_module_t *pmodule, plc_dest_t *dest)
{

	unsigned char proto;
	unsigned char *pdata;
	
	memset((unsigned char *)dest, 0, sizeof(plc_dest_t));
	
	pdata = pmodule->app_send_buf;

	proto = pdata[8];
	proto &= 0x1f;
	if(proto & 0x10) {
		if(proto == 0x10) proto = 1;
		else proto = 2;
	}
	else if(proto == 0x08) proto = 2;
	else proto = 1;
	smallcpy(dest->dest, pdata+1, 6);

	dest->src[0] = pmodule->module_id[0];
	dest->src[1] = pmodule->module_id[1];
	dest->src[2] = pmodule->module_id[2];
	dest->src[3] = pmodule->module_id[3];
	dest->src[4] = pmodule->module_id[4];
	dest->src[5] = pmodule->module_id[5];
	dest->metid = pmodule->cur_metid+1;
	dest->portcfg = pmodule->port;
	dest->proto = proto;
	dest->route.level = 0;
	dest->route.phase = 0;

}
#endif
#if 0
static int PlRscStdReadData(struct amr_module_t *pmodule, rep_reading_info *preadinginfo)
{
	plc3762_pkt_t *pkt = (plc3762_pkt_t *)pmodule->link_send_buf;
	plc3762_pkt_t *prcv = (plc3762_pkt_t *)pmodule->link_recv_buf;
	plc_dest_t dest;
	unsigned char *pdata;
	int i, applen;
	
	PlRscStdMakeDest(pmodule, &dest);
	
	pdata = pmodule->app_send_buf;
	
	pkt->afn = PL3762_AFN_FORWARD;
	FN_DT(1, pkt->dt);
	pkt->data[0] = dest.proto;
	pkt->data[1] = (unsigned char)(pmodule->app_sendlen);
	memcpy(pkt->data+2, pdata, pmodule->app_sendlen);
	pkt->len = pmodule->app_sendlen + 2;

	pkt->ctrl = 0x41;
	if(Pl3762SendPkt(pmodule, &dest, 0)) goto mark_fail;
	if(PlRsc3762RecvPktTimeout(pmodule, PL3762_AFN_FORWARD, 1, PLRSC_READMET_TIMEOUT)) goto mark_fail;
	
	for(i=0; i<6; i++) {
		if(pmodule->link_recv_dest.src[i] != dest.dest[i]) goto mark_fail;
	}
	if(prcv->data[0] != dest.proto)  goto mark_fail;
	if(prcv->len != (prcv->data[1]+2))  goto mark_fail;
	applen = prcv->data[1] & 0xff;
	if(applen <= 0) goto mark_fail;

	//去掉FE
	pdata = prcv->data + 2;
	for(i=0; i<applen; i++) {
		if(0x68 == pdata[i]) break;
	}
	if((i+8) >= applen) goto mark_fail;

	Pl3762GetReadingInfo(pmodule, preadinginfo);
	smallcpy(pmodule->app_recv_buf, pdata+i, applen-i);
	pmodule->app_proto = dest.proto;
	pmodule->app_recvlen = applen-i;
	
	return pmodule->app_recvlen;
	
mark_fail:
	return PLCERR_INVALID;
}
#endif
#if 0
/**
* @brief 数据抄读处理
* @param *pmodule 控制块
*/
static void PlRscStdReadingProc(struct amr_module_t *pmodule)
{

	int rtn= -1;
	usrmet_map_t *pmap;
	int port = pmodule->port;
	rep_reading_info readinginfo;
	
	for(; pmodule->cur_metid<MAX_METER; pmodule->cur_metid++) {
		pmap = &pmodule->met_map[pmodule->cur_metid];
		
		//无效或者连续失败15次 reserv 用作失败次数纪录
		if(0 == pmap->attr || pmap->reserv >= 15) continue;
		if(pmodule->dataexecfunc.pget_readcontent)
		rtn = (*pmodule->dataexecfunc.pget_readcontent)(port, pmodule->cur_metid+1, pmap->addr, 
			&pmodule->commfunc.frame, pmodule->app_send_buf, &pmodule->app_sendlen);
		
		if(rtn == REQREAD_DONE || rtn == REQREAD_SKIP) continue;
		else if(rtn == REQREAD_NOTEXIST){
			pmap->attr = 0;
			pmodule->flag_sync = 1;
			break;
		}else break;
	}

	if(rtn == REQREAD_SUCCESS && pmodule->app_sendlen >12){
		rtn = PlRscStdReadData(pmodule, &readinginfo);
		if(rtn > 0){
			rtn = (*pmodule->dataexecfunc.pproc_reportdata)(port, pmodule->cur_metid+1, 
			pmodule->app_proto, pmodule->app_recv_buf, pmodule->app_recvlen, readinginfo);
			if(rtn == ECHO_OK){	//抄到数据成功
				pmap->reserv = 0; //失败次数清零
				return;
			}
		}
		pmap->reserv++;
	}

}
#endif
static void PlRscStdMsgProc(struct amr_module_t *pmodule)
{
	plc3762_pkt_t *prcv = (plc3762_pkt_t *)pmodule->link_recv_buf;

	//主动上报数据处理
	if(PL3762_AFN_ACTIVESND == prcv->afn) Pl3762SendEchoPkt(pmodule, 0);
}

/**
* @brief 抄表控制检查
* @param *pmodule 控制块
* @return 返回0表示可以抄表，非0表示暂停抄表
*/
#if 0
static int PlRscStdAmrStartCheck(struct amr_module_t *pmodule)
{	
	if(!pmodule->flag_stoped){	//未停止抄表
		if(pmodule->flag_sync){	//需要档案同步
			if(2 == pmodule->flag_sync) PlRscStdModuleInit(pmodule);
//			InitAmrMetMap(pmodule, 1);
			pmodule->flag_sync = 0;
		}
		
		if(pmodule->flag_reseting){	//需重启抄表
			pmodule->flag_reseting = 0;
			PlRscStdClearReadState(pmodule);
		}
	}

	return pmodule->flag_stoped;
}
#endif

void PlRscStdStartRecv(struct amr_module_t *pmodule)
{
	pmodule->link_dl_recvlen = 0;
	pmodule->link_dl_recvstat = 0;
	pmodule->link_dl_recvmax = 0;
	pmodule->link_dl_recvtimes = 0;
}

/**
* @brief 瑞斯康任务处理
* @param arg 端口号
*/
void PlRscStdTask(void * arg)
{
	unsigned long ev;
	struct amr_module_t* pmodule = (struct amr_module_t*)arg;
	
	PrintLog(LOGTYPE_DOWNLINK, "PlRscStdTask running... \n");
		
	Sleep(30);
	
	
	PlRscStdStartRecv(pmodule);
	PlRscStdModuleInit(pmodule);
//	InitAmrMetMap(pmodule, 1);
	while(1){
		SysClearCurrentTaskState();

		/*检查是否需要启动自动抄表*/
//		plstopflag = PlRscStdAmrStartCheck(pmodule);

		//接收数据帧并处理
		if(!Pl3762RecvPkt(pmodule)) PlRscStdMsgProc(pmodule);
		/*事件处理*/
		SysWaitEvent(&pmodule->comm_event, 0, AMREVENT_ALL, &ev);
		if(ev) PlRscStdEventProc(pmodule, ev);
		
		Sleep(2);
	}
}
#endif
