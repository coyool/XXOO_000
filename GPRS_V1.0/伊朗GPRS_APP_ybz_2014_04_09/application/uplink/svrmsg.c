/**
* msgproc.c -- 上行通信命令处理
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-18
* 最后修改时间: 2009-5-18
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define define_QueryCache

#include "app_include/uplink/svrmsg.h"
#include "plat_include/sleep.h"
#include "app_include/version.h"
#include "plat_include/debug.h"
#include "plat_include/reset.h"
#include "plat_include/mutex.h"
#include "plat_include/timeal.h"
#include "app_include/lib/bcd.h"
#include "app_include/param/term.h"
#include "app_include/param/unique.h"
#include "app_include/param/operation.h"
#include "app_include/param/metp.h"
#include "app_include/param/mix.h"
#include "app_include/monitor/runstate.h"
#include "app_include/monitor/alarm.h"
#include "app_include/cenmet/qrycurve.h"
#include "app_include/cenmet/qrydata.h"
#include "app_include/cenmet/forward.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
#include "app_include/uplink/svrcomm.h"
#include "app_include/uplink/hdlc.h"

#include "app_include/param/meter.h"
#include "app_include/lib/align.h"
#include "plat_include/plcomm.h"

#include "plat_include/sys_config.h"


//static unsigned char SvrAddrRecord = 0;   //记录主站参数变更事件用
static unsigned char CountPwdError[UPLINKITF_NUM] = {0};

#define MAXCNT_PWDERR    3

typedef struct st_qrycurve_buffer_unlock {
	//in
	struct st_qrycurve_buffer_unlock *next;
	int maxlen;
	unsigned char * buffer;

	//out
	int datalen;
} qrycurve_buffer_unlock_t;

typedef union {
	qrycurve_buffer_unlock_t unblock;
	qrycurve_buffer_t block;
} qrycurve_buffer_union_t;

unsigned int QueryCache[QUERY_CACHE_NUM][QUERY_CACHE_LEN/4];
//static sys_mutex_t QueryCacheMutex;
static qrycurve_buffer_union_t QueryCacheList[QUERY_CACHE_NUM];

/**
* @brief 获得查询缓存区组
* @param 每个缓存区的最大长度
* @return 缓存区组指针
*/

qrycurve_buffer_t *GetQueryCache(int maxlen)
{
	int i;

	if(maxlen <= 0 || maxlen > (QUERY_CACHE_LEN-MINLEN_UPLINKPKT)) {
		ErrorLog("invalid maxlen(%d)\n", maxlen);
		maxlen = QUERY_CACHE_LEN-MINLEN_UPLINKPKT;
	}

	for(i=0; i<(QUERY_CACHE_NUM-1); i++) {
		QueryCacheList[i].unblock.next = &QueryCacheList[i+1].unblock;
		QueryCacheList[i].unblock.maxlen = maxlen-OFFSET_UPDATA;
		QueryCacheList[i].unblock.buffer = (unsigned char *)(QueryCache[i]) + OFFSET_UPDATA;
		QueryCacheList[i].unblock.datalen = 0;
	}
	QueryCacheList[i].unblock.next = NULL;
	QueryCacheList[i].unblock.maxlen = maxlen-OFFSET_UPDATA;
	QueryCacheList[i].unblock.buffer = (unsigned char *)(QueryCache[i]) + OFFSET_UPDATA;
	QueryCacheList[i].unblock.datalen = 0;

	return &(QueryCacheList[0].block);
}

/**
* @brief 缓存区清零
* @param buffer 缓存区指针
*/

void ClearQueryCache(qrycurve_buffer_t *buffer)
{
	for(; NULL != buffer; buffer = buffer->next) buffer->datalen = 0;
}

/**
* @brief 锁定缓存区组操作
*/
void QueryCacheLock(void)
{
//	SysLockMutex(&QueryCacheMutex);
}

/**
* @brief 解锁缓存区组操作
*/
void QueryCacheUnlock(void)
{
//	SysUnlockMutex(&QueryCacheMutex);
}

static uplink_timetag_t TimeTag[UPLINKITF_NUM];
static unsigned char TpvFlag[UPLINKITF_NUM] = {0};
static unsigned char SelfCmdFlag[UPLINKITF_NUM] = {0};

/**
* @brief 发送命令回应帧
* @param itf 接口编号
* @param pkt 发送帧指针
* @return 成功0, 否则失败
*/
int SvrEchoSend(unsigned char itf, uplink_pkt_t *pkt)
{
	unsigned char *paux;
	int rtn;
	unsigned int delay = 0;

//	SysClearCurrentTaskState();

	if(SelfCmdFlag[itf]) {
		unsigned char *psrc, *pdst;
		unsigned short cpylen = UPLINKAPP_LEN(pkt);
		unsigned short cpyi;

		psrc = pkt->data + cpylen - 1;
		pdst = psrc + 5;

		for(cpyi=0; cpyi<cpylen; cpyi++) *pdst-- = *psrc--;

		pdst = pkt->data;
		pdst[0] = pdst[1] = 0;  //p0
		pdst[2] = 0x01;  //F9
		pdst[3] = 0x01;
		pdst[4] = pkt->afn;

		pkt->afn = UPAFN_TRANFILE;
		cpylen += 5;
		UPLINKAPP_LEN(pkt) = cpylen;
	}

	paux = pkt->data;
	paux += UPLINKAPP_LEN(pkt);

	if(RunState.flag_acd) {
		pkt->ctrl &= 0xef;
		pkt->ctrl |= 0x20;
	#ifdef ALARM_MODULE
		GeAlarmEc(paux);
	#else
		*paux = 0;
		*(paux+1) = 0;
	#endif
		paux += 2;
		UPLINKAPP_LEN(pkt) += 2;
	}

	if(TpvFlag[itf]) {
		pkt->seq |= UPSEQ_TPV;
		paux[0] = TimeTag[itf].pfc;
		paux[1] = TimeTag[itf].time[0];
		paux[2] = TimeTag[itf].time[1];
		paux[3] = TimeTag[itf].time[2];
		paux[4] = TimeTag[itf].time[3];
		paux[5] = TimeTag[itf].dly;
		UPLINKAPP_LEN(pkt) += LEN_UPLINK_TIMETAG;
	}

	if(0 == (pkt->seq & UPSEQ_FIN)) {
		delay = (int)UPLINKAPP_LEN(pkt)&0xffff;
		delay += 16;
		delay >>= 7;
		delay *= UPLINK_TIMEOUT(itf);
		delay += UPLINK_TIMEOUT(itf)*3;
		/*delay = UPLINKAPP_LEN(pkt);
		delay &= 0xffff;
		delay >>= 2;
		delay += 250;*/
	}

	rtn = UplinkSendPkt(itf, pkt);
	if(!rtn) {
		if(delay) Sleep(delay);
	}

	return(rtn);
}

/**
* @brief 发送回应无数据帧
* @param itf 接口编号
* @param pkt 发送帧缓存区指针
*/
void SvrEchoNoData(unsigned char itf, uplink_pkt_t *pkt)
{
	uplink_ack_t *pack = (uplink_ack_t *)pkt->data;

	pkt->ctrl = UPECHO_DENY;
	pkt->afn = UPAFN_ECHO;
	pack->da[0] = pack->da[1] = 0;
	pack->dt[0] = 0x02;  //F2
	pack->dt[1] = 0;
	UPLINKAPP_LEN(pkt) = 4;

	SvrEchoSend(itf, pkt);
}

/**
* @brief 计算密码校验
* @param pwd 密钥
* @param pdata 数据缓存区指针
* @param len 数据缓存区长度
* @return 密码校验数
*/
static unsigned short CalculatePwd(unsigned short pwd, const unsigned char *pdata, int len)
{
	unsigned short crc;
	int i;

	crc = 0;
	if(len <= 0) return 0;

	while(len-- != 0) {
		crc ^= (unsigned short)(*pdata++)&0x00ff;

		for(i=0; i<8; i++) {
			if(crc&0x0001) crc = (crc>>1)^pwd;
			else crc >>= 1;
		}
	}

	return(crc);
}

/**
* @brief 校验密码的正确性
* @param 密码
* @pdata 数据缓存区指针
* @return 正确返回0, 错误返回1
*/
static int CheckPassword(const unsigned char *pass, const unsigned char *pdata,unsigned char itf, unsigned char gpnum)
{
	unsigned short crc, pwd;
	unsigned char afn;
        
        
	pwd = ((unsigned short)ParaTerm.pwd.pwd[1]<<8) + (unsigned short)ParaTerm.pwd.pwd[0];
	if(ParaTerm.pwd.art == 0) {
		crc = ((unsigned short)pass[1]<<8) + (unsigned short)pass[0];
		return 0;
	}
	else if(ParaTerm.pwd.art == 255) {
		afn = pdata[6];
		if((afn == UPAFN_RESET) || (afn == UPAFN_SETPARA ) || 
			(afn == UPAFN_CTRL) || (afn == UPAFN_TRANFILE) || (afn == UPAFN_FORWARD)) {
			//if(itf == SvrCommInterface) {
				//memcpy(mak1,pass,4);
				//frameLen =   pass -pdata -6;
				//esamFrame = pdata+6;
				///Rs485EsamMode(1);
				///later
				/*
				if(checkEsamMAC(esamFrame, frameLen, mak2, gpnum) == 0) {
					for(i=0; i<4; i++){
						if(mak1[i] != mak2[3-i]) break;
					}
					if(i >= 4){
						///Rs485EsamMode(0);
						return 0;
					}
				}
				ESAMEcho(itf,3);
				*/
				///Rs485EsamMode(0);
				//return 1;
			//} 
		}
		crc = ((unsigned short)pass[1]<<8) + (unsigned short)pass[0];
	}
	else {
		crc = CalculatePwd(pwd, pdata, 12);
		pwd = ((unsigned short)pass[1]<<8) + (unsigned short)pass[0];
	}
	if(crc == pwd) return 0;
	else return 1;
}

/**
* @brief 复位命令处理
* @param itf 接口编号
*/
static void SvrReset(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	unsigned char *pecho = psnd->data;
	unsigned char *pcmd = prcv->data;
	unsigned char resetcmd;

	if(UPLINKAPP_LEN(prcv) != 4) return;

	psnd->ctrl = UPECHO_CONF;
	psnd->afn = UPAFN_ECHO;

	UPLINK_PID0(pecho);

	if(0 != pcmd[3]) return;
	resetcmd = pcmd[2];

	if(1 == resetcmd) { //硬件初始化
		UPLINK_FID(1, pecho);
		UPLINKAPP_LEN(psnd) = 4;
		SvrEchoSend(itf, psnd);
	}
	else if(2 == resetcmd) { //数据区初始化
		UPLINK_FID(1, pecho);
		UPLINKAPP_LEN(psnd) = 4;
		SvrEchoSend(itf, psnd);
		Sleep(50);

		ClearAllData();
		SaveCleanData();

#ifdef ALARM_MODULE
		CheckSoftChange();
#endif
		SysRestart();
		return;
	}
	else if((4 == resetcmd) || (8 == resetcmd)) { //参数及数据区初始化(恢复出厂配置)

		UPLINK_FID(1, pecho);
		UPLINKAPP_LEN(psnd) = 4;
		SvrEchoSend(itf, psnd);
		Sleep(50);

		ClearAllParam();
		ClearAllData();

	}
#if 0
	else if(8 == resetcmd)  //参数及数据区初始化
	{
		ClearFactoryParam();
		ClearAllData();

	}
#endif

	UPLINK_FID(1, pecho);
	UPLINKAPP_LEN(psnd) = 4;
	SvrEchoSend(itf, psnd);
	Sleep(50);
	if(UPLINKITF_GPRS == itf) Sleep(300);

	SysRestart();

	return;
}

/**
* @brief 登陆/心跳报文处理
* @param itf 接口编号
*/
static void SvrLinkTest(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	unsigned char *pecho = psnd->data;
	unsigned char *pcmd = prcv->data;
	unsigned char linktest;

	if(UPLINKAPP_LEN(prcv) != 4) return;

	psnd->ctrl = UPECHO_CONF;
	psnd->afn = UPAFN_ECHO;

	UPLINK_PID0(pecho);

	if(0 != pcmd[3]) return;
	linktest = pcmd[2];

	switch(linktest){
		case 1:
		case 2:
		case 4:
			UPLINK_FID(1, pecho);
			UPLINKAPP_LEN(psnd) = 4;
			SvrEchoSend(itf, psnd);
			break;
		default:
			break;
	}
	return;
}

//static alarm_t ParaChgRecord;
/*
static void StartRecordParamChg(void)
{
	memset(&ParaChgRecord, 0, sizeof(ParaChgRecord));

	ParaChgRecord.erc = 3;
	ParaChgRecord.len = 1;
	ParaChgRecord.data[1] = SvrAddrRecord;
}
*/
static void AddRecordParamChg(const unsigned char *pnfn)
{
#ifdef ALARM_MODULE
	unsigned char *pdata;

	if((ParaChgRecord.len + 4) > MAXLEN_ALMDATA) {
		MakeAlarm(ALMFLAG_ABNOR, &ParaChgRecord);
		StartRecordParamChg();
	}

	pdata = ParaChgRecord.data + ParaChgRecord.len;
	pdata[0] = pnfn[0];
	pdata[1] = pnfn[1];
	pdata[2] = pnfn[2];
	pdata[3] = pnfn[3];
	ParaChgRecord.len += 4;
#endif
}

static void EndRecordParamChg(void)
{
#ifdef ALARM_MODULE
	if(ParaChgRecord.len > 1) MakeAlarm(ALMFLAG_ABNOR, &ParaChgRecord);
#endif
}

/**
* @brief 设置参数命令处理
* @param itf 接口编号
*/
static void SvrSetParam(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	uplink_ack_t *pack = (uplink_ack_t *)psnd->data;
	int ballok = 1;
	int rcvlen, sendlen, actlen, rtn;
	unsigned char *pdatarcv, *pdatasnd;

	psnd->ctrl = UPECHO_USRDATA;
	psnd->afn = UPAFN_ECHO;
	pack->da[0] = pack->da[1] = 0;
	pack->dt[1] = 0;
	pack->afn = prcv->afn;

	pdatarcv = prcv->data;
	pdatasnd = pack->data;
	rcvlen = UPLINKAPP_LEN(prcv);
	sendlen = 5;

	if(rcvlen <= 4) goto mark_fail;

	ClearSaveParamFlag();
//	StartRecordParamChg();

	while(rcvlen > 4) {
		actlen = 4;
		rtn = WriteParam(pdatarcv, rcvlen, &actlen);
		if(rtn == POERR_FATAL) goto mark_fail;

		*pdatasnd++ = pdatarcv[0];
		*pdatasnd++ = pdatarcv[1];
		*pdatasnd++ = pdatarcv[2];
		*pdatasnd++ = pdatarcv[3];
		if(rtn == POERR_OK) {
			*pdatasnd++ = 0;

			AddRecordParamChg(pdatarcv);
		}
		else {
			*pdatasnd++ = 1;
			ballok = 0;
		}

		rcvlen -= actlen;
		pdatarcv += actlen;
		sendlen += 5;
	}

	//SaveParam();
	EndRecordParamChg();

	if(ballok) {
		pack->dt[0] = 0x01;  //F1
		UPLINKAPP_LEN(psnd) = 4;
	}
	else {
		pack->dt[0] = 0x04; //F3
		UPLINKAPP_LEN(psnd) = sendlen;
	}

	SvrEchoSend(itf, psnd);
	return;

mark_fail:
	pack->dt[0] = 0x02;  //F2
	UPLINKAPP_LEN(psnd) = 4;
	SvrEchoSend(itf, psnd);

	return;
}

extern int CtrlCommand(unsigned char *buf, int len, int *pactlen);
/**
* @brief 控制命令处理
* @param itf 接口编号
*/
static void SvrCtrlCmd(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	uplink_ack_t *pack = (uplink_ack_t *)psnd->data;
	int ballok = 1;
	int rcvlen, sendlen, actlen, rtn;
	unsigned char *pdatarcv, *pdatasnd;

	psnd->ctrl = UPECHO_USRDATA;
	psnd->afn = UPAFN_ECHO;
	pack->da[0] = pack->da[1] = 0;
	pack->dt[1] = 0;
	pack->afn = prcv->afn;

	pdatarcv = prcv->data;
	pdatasnd = pack->data;
	rcvlen = UPLINKAPP_LEN(prcv);
	sendlen = 5;

	if(rcvlen < 4) goto mark_fail;

	//@change later: 加入参数改变告警
	ClearSaveParamFlag();

	while(rcvlen >= 4) {
		actlen = 4;
		rtn = CtrlCommand(pdatarcv, rcvlen, &actlen);
		if(rtn == -1) goto mark_fail;

		*pdatasnd++ = pdatarcv[0];
		*pdatasnd++ = pdatarcv[1];
		*pdatasnd++ = pdatarcv[2];
		*pdatasnd++ = pdatarcv[3];
		if(rtn == 0) *pdatasnd++ = 0;
		else {
			*pdatasnd++ = 1;
			ballok = 0;
		}

		rcvlen -= actlen;
		pdatarcv += actlen;
		sendlen += 5;
	}

	//SaveParam();

	if(ballok) {
		pack->dt[0] = 0x01;  //F1
		UPLINKAPP_LEN(psnd) = 4;
	}
	else {
		pack->dt[0] = 0x04; //F3
		UPLINKAPP_LEN(psnd) = sendlen;
	}

	SvrEchoSend(itf, psnd);
	return;

mark_fail:
	pack->dt[0] = 0x02;  //F2
	UPLINKAPP_LEN(psnd) = 4;
	SvrEchoSend(itf, psnd);

	return;
}

/**
* @brief 身份认证及密钥协商处理
* @param itf 接口编号
*/
static void SvrKeyConsult(unsigned char itf)
{
	/*interface*/
	return;
}

extern int ReadTermInfo(const unsigned char *pnfn, unsigned char *buf, int len);
/**
* @brief 查询终端配置命令处理
* @param itf 接口编号
*/

static void SvrReqConfig(unsigned char itf)
{

	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	int rcvlen, sendlen, rtn, sendmax;
	unsigned char *pdatarcv, *pdatasnd;

	psnd->ctrl = UPECHO_USRDATA;
	psnd->afn = UPAFN_REQCFG;

	pdatarcv = prcv->data;
	pdatasnd = psnd->data;
	rcvlen = UPLINKAPP_LEN(prcv);
	sendlen = 0;
	sendmax = UPLINK_SNDMAX(itf);

	while(rcvlen >= 4) {
		rtn = ReadTermInfo(pdatarcv, pdatasnd, sendmax);
		if(rtn > 0) {
			sendmax -= rtn;
			if(sendmax < 0) {
				ErrorLog("send max error(%d,%d)\n", sendmax, rtn);
				goto mark_end;
			}

			sendlen += rtn;
			pdatasnd += rtn;
		}

		rcvlen -= 4;
		pdatarcv += 4;
	}

	if(0 == sendlen) goto mark_end;

	UPLINKAPP_LEN(psnd) = sendlen;
	SvrEchoSend(itf, psnd);
	return;

mark_end:
	if(0 == sendlen) {
		SvrEchoNoData(itf, psnd);
		return;
	}

	UPLINKAPP_LEN(psnd) = sendlen;
	SvrEchoSend(itf, psnd);
}


/**
* @brief 查询参数命令处理
* @param itf 接口编号
*/
static void SvrQueryParam(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	int rcvlen, sendlen, actlen, rtn, sendmax;
	unsigned char *pdatarcv, *pdatasnd;
	para_readinfo_t readinfo;

	psnd->ctrl = UPECHO_USRDATA;
	psnd->afn = UPAFN_QRYPARA;

	pdatarcv = prcv->data;
	pdatasnd = psnd->data;
	rcvlen = UPLINKAPP_LEN(prcv);
	sendlen = 0;
	sendmax = UPLINK_SNDMAX(itf);

	while(rcvlen >= 4) {
		readinfo.actlen = 4;
		readinfo.buf = pdatarcv;
		readinfo.len = rcvlen;

		rtn = ReadParam(pdatasnd, sendmax, &actlen, &readinfo);
		if(rtn == POERR_FATAL) goto mark_end;
		else if(rtn == POERR_OK) {
			sendmax -= actlen;
			if(sendmax < 0) {
				ErrorLog("send max error(%d,%d)\n", sendmax,actlen);
				goto mark_end;
			}
			sendlen += actlen;
			pdatasnd += actlen;

			rcvlen -= readinfo.actlen;
			pdatarcv += readinfo.actlen;
		}
		else {
			rcvlen -= readinfo.actlen;
			pdatarcv += readinfo.actlen;
		}
	}

	if(0 == sendlen) goto mark_end;

	UPLINKAPP_LEN(psnd) = sendlen;
	SvrEchoSend(itf, psnd);
	return;

mark_end:
	if(0 == sendlen) {
		SvrEchoNoData(itf, psnd);
		return;
	}

	UPLINKAPP_LEN(psnd) = sendlen;
	SvrEchoSend(itf, psnd);
}


#define MAX_REPROT_METNUM	15	
/**
* @brief 读取当前时间
* @param buf 缓存区指针
* @param plen 返回的缓存区长度
* @return 成功0, 否则失败(参数和返回值以下同类函数相同)
*/
static int MdbStatTime(unsigned char *buf, int *plen)
{
	sysclock_t clk;

	*plen = 6;

	SysClockRead(&clk);

	buf[0] = clk.second;
	buf[1] = clk.minute;
	buf[2] = clk.hour;
	buf[3] = clk.day;
	buf[4] = clk.month;
	buf[5] = clk.year;
	HexToBcd(buf, 6);

	buf[4] &= 0x1f;
	if(clk.week == 0) clk.week = 7;
	buf[4] |= (clk.week<<5)&0xe0;
	return 0;
}

/**
* @brief 查询一类数据命令处理
* @param itf 接口编号
*/
extern short GprsDevSigdBm;
static void SvrQueryClassOne(unsigned char itf)
{

	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
//	int rtn;
	unsigned char itemid[4];

	psnd->ctrl = UPECHO_USRDATA;
	psnd->afn = UPAFN_QRYCLS1;
	
	smallcpy(itemid, prcv->data,4);

	{
		int sendlen, actlen;
		unsigned short metpid, itemid;
		unsigned char pns, pnmask, fns, grpid, fnmask, basepn;
		const unsigned char *rcvbuf;
		int rcvlen, bsended;
		unsigned char *sendbuf;
		int sendmax;
		//int first = 1;
		//unsigned char seq = 0;
		//unsigned char pnflg,fnflg;
	
		rcvbuf = prcv->data;
		rcvlen = UPLINKAPP_LEN(prcv);
		sendbuf = psnd->data;
		sendmax = UPLINK_SNDMAX(itf);
		sendlen = 0;
		bsended = 0;

		while(rcvlen >= 4) {
			pns = rcvbuf[0];
			basepn = rcvbuf[1];
			if(0 == basepn) metpid = 0;
			else metpid = ((unsigned short)(basepn-1)<<3) + 1;

			fns = rcvbuf[2];
			grpid = rcvbuf[3];

			rcvbuf += 4;
			rcvlen -= 4;
			if(metpid > MAX_METP) continue;
			if(0 == metpid) pnmask = 0x80;
			else pnmask = 1;
			for(; pnmask!=0; metpid++,pnmask<<=1) {

				if(metpid && (pns&pnmask) == 0) continue;
				
			    //if(0x80==pnmask)  pnflg =0;
			   // else  pnflg = pns&(~(pnmask*2-1));
			    
				for(fnmask=1; fnmask!=0;fnmask<<=1) {

					if((fns&fnmask) == 0) continue;
					
				   // if(0x80==fnmask)    fnflg = 0;
				  //  else    fnflg = fns&(~(fnmask*2-1));

					if(0 == metpid) {
						sendbuf[0] = sendbuf[1] = 0;
					}
					else {
						sendbuf[0] = pnmask;
						sendbuf[1] = basepn;
					}
					sendbuf[2] = fnmask;
					sendbuf[3] = grpid;

					itemid = ((unsigned short)grpid<<8) + (unsigned short)fnmask;
					actlen = 0;
			
				//	rtn = ReadMdb(metpid, itemid, sendbuf+4, sendmax,&actlen);
					if(itemid == 0x0002)
					{
						MdbStatTime(sendbuf+4,&actlen);
					}
					else
					if((metpid == 0)&&(itemid == 0x1701))
					{
						memcpy(sendbuf+4,&GprsDevSigdBm,1);
						actlen=1;
					}
					else
						goto mark_end;
					//if(!rtn) 
					{
						actlen += 4;
						sendmax -= actlen;
						if(sendmax < 0) {
							ErrorLog("send max error(%d,%d)\n", sendmax, actlen);
							sendlen = -1;
							goto mark_end;
						}
						sendbuf += actlen;
						sendlen += actlen;
					}
				}
			}
		}

mark_end:
		if(sendlen > 0) {
			UPLINKAPP_LEN(psnd) = sendlen;
			SvrEchoSend(itf, psnd);
		}
		else if(0 == bsended)
			SvrEchoNoData(itf, psnd);
	}
	   
}

//static unsigned char DataValidFlg;
/*
static void SetDataFlgInvalid(void)
{
	DataValidFlg = 0;
}
*/
//rtn 0 invalid 1 valid
/*
static unsigned char  ChkDataFlgValid(void)
{
	
	return DataValidFlg;
}

void SetDataFlgValid(void)
{
	DataValidFlg = 1;
}
*/
/**
* @brief 读取2类数据
* @param rcvbuf 接收缓存区指针
* @param rcvlen 接收缓存区长度
* @param sendbuf 发送缓存区指针
* @param sendmax 发送缓存区最大长度
* @param bactive 是否主动发送, 0-查询, 1主动发送, D1~D7曲线抽取密度
* @return 成功返回0, 失败返回1, 缓存区溢出返回-1
*/
int QueryDataClassTwo(const unsigned char *rcvbuf, int rcvlen, qrycurve_buffer_t *pcache, int bactive)
{
#ifdef ALARM_MODULE
	int rtn;
	unsigned short metpid;
	unsigned char pns, pnmask, fns, grpid, fnmask, pnid;

	unsigned char itemid[4];

	//if((rcvbuf[0] == 0xff) &&( rcvbuf[1] == 0xff) && (rcvbuf[2] == 0x80)&&(rcvbuf[3] == 0x1e))    goto mark_alldata;   
	//if((rcvbuf[0] == 0xff) &&( rcvbuf[1] == 0xff) && (rcvbuf[2] == 0x01)&&(rcvbuf[3] == 0x64))    goto mark_801alldata;  
	
	while(rcvlen >= 4) {
		pns = rcvbuf[0];
		if(0 == rcvbuf[1]) metpid = 0;
		else metpid = ((unsigned short)(rcvbuf[1]-1)<<3) + 1;
		fns = rcvbuf[2];
		grpid = rcvbuf[3];
		pnid = rcvbuf[1];
		
		rcvlen -= 4;
		rcvbuf += 4;

		if(metpid > MAX_METP) continue;

		if(0 == metpid) pnmask = 0x80;
		else pnmask = 1;
		for(; pnmask!=0; pnmask<<=1,metpid++) {
			if(metpid && (pns&pnmask) == 0) continue;

			for(fnmask=1; fnmask!=0; fnmask<<=1) {
				if((fns&fnmask) == 0) continue;
#ifdef ALARM_MODULE
				if(0 == metpid) {
					itemid[0] = itemid[1] = 0;
				}
				else {
					itemid[0] = pnmask;
					itemid[1] = pnid;
				}
				itemid[2] = fnmask;
				itemid[3] = grpid;

				rtn = QueryCurve(itemid, rcvbuf, rcvlen, pcache, bactive);
#endif
				if(rtn < 0) return 1;
				else if(rtn == 0) return -1;
				if(!bactive) {
					rcvlen -= rtn;
					if(rcvlen < 0) return 1;
					rcvbuf += rtn;
				}
			}
		}

	}
#endif	
	return 0;
	
//mark_alldata:
	//return QueryF248(rcvbuf,&pcache);
//mark_801alldata:
	//return  QueryF801(rcvbuf,&pcache);
}


/**
* @brief 查询二类数据命令处理
* @param itf 接口编号
*/
#if 0
static void SvrQueryClassTwo(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	qrycurve_buffer_t *cache;
	int first;
	unsigned char seq = 0;

	if(UPLINKAPP_LEN(prcv) < 6) return;
	SetDataFlgInvalid();
	QueryCacheLock();

	cache = GetQueryCache(UPLINK_RCVMAX(itf));

	QueryDataClassTwo(prcv->data, UPLINKAPP_LEN(prcv), cache, 0);

	if((0 == cache->datalen)||!ChkDataFlgValid()) {
		SvrEchoNoData(itf, psnd);
		QueryCacheUnlock();
		return;
	}

	first = 1;
	for(; NULL!=cache; cache=cache->next) {
		/*DebugPrint(0, "cache=%08XH, buff=%08XH, next=%08XH, max=%d, datalen=%d\n",
			cache, cache->buffer, cache->next, cache->maxlen, cache->datalen);*/
		if(cache->datalen <= 0) break;

		psnd = (uplink_pkt_t *)(cache->buffer - OFFSET_UPDATA);
		psnd->ctrl = UPECHO_USRDATA;
		psnd->afn = UPAFN_QRYCLS2;
		psnd->grp = prcv->grp & 0xfe;
		if(first) {
			first = 0;
			if(cache->next->datalen > 0) {  //muti packet
				//DebugPrint(0, "multi packet\n");
				seq = prcv->seq & 0x0f;
				psnd->seq = seq++;
				psnd->seq |= UPSEQ_FIR;
			}
			else { //single packet
				psnd->seq = prcv->seq & 0x0f;
				psnd->seq |= UPSEQ_SPKT;
			}
		}
		else {
			psnd->seq = seq++ & 0x0f;
			if(NULL == cache->next || cache->next->datalen <= 0)
				psnd->seq |= UPSEQ_FIN;  //last
		}

		UPLINKAPP_LEN(psnd) = cache->datalen;

		SvrEchoSend(itf, psnd);
	}

	QueryCacheUnlock();
}
#endif
/**
* @brief 查询三类数据命令处理
* @param itf 接口编号
*/
#ifdef ALARM_MODULE
static void SvrQueryClassThree(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	qrycurve_buffer_t *cache;
	int first;
	unsigned char seq = 0;

	if(UPLINKAPP_LEN(prcv) < 6) return;

	QueryCacheLock();

	cache = GetQueryCache(UPLINK_RCVMAX(itf));

	QueryAlarm(prcv->data, cache);

	if(0 == cache->datalen) {
		QueryCacheUnlock();
		SvrEchoNoData(itf, psnd);
		return;
	}

	first = 1;
	for(; NULL!=cache; cache=cache->next) {
		if(cache->datalen <= 0) break;

		psnd = (uplink_pkt_t *)(cache->buffer - OFFSET_UPDATA);
		psnd->ctrl = UPECHO_USRDATA;
		psnd->afn = UPAFN_QRYCLS3;
		psnd->grp = prcv->grp & 0xfe;
		if(first) {
			first = 0;
			if(cache->next->datalen > 0) {  //muti packet
				seq = prcv->seq & 0x0f;
				psnd->seq = seq++;
				psnd->seq |= UPSEQ_FIR;
			}
			else { //single packet
				psnd->seq = prcv->seq & 0x0f;
				psnd->seq |= UPSEQ_SPKT;
			}
		}
		else {
			psnd->seq = seq++ & 0x0f;
			if(NULL == cache->next || cache->next->datalen <= 0)
				psnd->seq |= UPSEQ_FIN;  //last
		}

		UPLINKAPP_LEN(psnd) = cache->datalen;

		SvrEchoSend(itf, psnd);
	}

	QueryCacheUnlock();
}
#endif
#if 0


/**
* @brief 自定义命令处理
* @param itf 接口编号
*/
static void SvrSelfCmd(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	unsigned char *puc;
	unsigned short len;

	len = UPLINKAPP_LEN(prcv);

	if(len < 9) return;

	puc = prcv->data + 4;
	prcv->afn = *puc++;
	len -= 5;

	//memcpy(prcv->data, puc, len);
	smallcpy(prcv->data, puc, len);
	UPLINKAPP_LEN(prcv) = len;

	SelfCmdFlag[itf] = 1;
	SvrMessageProc(itf);
	SelfCmdFlag[itf] = 0;
}

extern void SvrSelfTranFile(unsigned char itf);
extern void SvrTranFileF1(unsigned char itf);

/**
* @brief 文件传输命令处理
* @param itf 接口编号
*/
static void SvrTranFile(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	unsigned char *puc;

	if(UPLINKAPP_LEN(prcv) < 4) return;

	puc = prcv->data;
	if(0 != puc[0] || 0 != puc[1] ) return;

#if (PROTOCOL_VERSION==PROTOCOL_GDW2005)
	//below is for 内蒙下载文件
	if((1 == puc[2]) && (0 == puc[3])) //F1
	{
		srv_dwnfile_F1(itf);
		
	}
	if((2 == puc[2]) && (0 == puc[3])) //F2
	{
		srv_dwnfile_info_F2(itf);
	}

	if((4 == puc[2]) && (0 == puc[3])) //F3
	{
		srv_dwnfile_reset_F3(itf);
	}

	if((8 == puc[2]) && (0 == puc[3])) //F4
	{
		srv_dwnfile_info_F4(itf);
	}

#endif

	if((1 == puc[2]) && (0 == puc[3])){
		SvrTranFileF1(itf); //F1
		return;
	}

	if(1 == puc[3]) {	
		if(1 == puc[2]) SvrSelfCmd(itf);  //F9
		else if(2 == puc[2]) SvrSelfTranFile(itf);//f10
		else if(4 == puc[2]) SvrShellProc(itf);//f11
		else if(8 == puc[2]) SvrtftpProc(itf);//f12
		else if(0x10 == puc[2]) SvrSysCmdProc(itf);//F13
	}

	return;
}
#endif
/**
* @brief 透明转发命令处理
* @param itf 接口编号
*/

void DlmsForward(const struct data_t* pcmd,struct data_t* pecho)
{
	//DebugPrint(LOGTYPE_DEBUG, "\r\nDlmsForward(const struct data_t* pcmd,struct data_t* pecho)\r\n");
#ifdef DEFINE_PLCOMM
	PlForwardF1(pcmd, pecho,0);//DLMS透传
#elif defined(DEFINE_SERIAL)
	CenMetForward(pcmd, pecho);
#else
	#error "error!"
#endif
}

extern void SvrSelfTranFile(unsigned char itf);

/**
* @brief 文件传输命令处理
* @param itf 接口编号
*/
static void SvrTranFile(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	unsigned char *puc;

	if(UPLINKAPP_LEN(prcv) < 4) return;

	puc = prcv->data;
	if(0 != puc[0] || 0 != puc[1] ) return;


	if((1 == puc[3])&&(2 == puc[2])) {	
		SvrSelfTranFile(itf);//f10
	}

	return;
}


struct srvmsg_func_t {
	unsigned char needpass;
	void (*pf)(unsigned char itf);
};
static const struct srvmsg_func_t srvmsg_func[] = {
	{0, NULL}, /*0x00, UPAFN_ECHO*/    
	{1, SvrReset}, /*0x01, UPAFN_RESET*/   
	{0, SvrLinkTest}, /*0x02, UPAFN_LINKTEST*/
	{0, NULL}, /*0x03, UPAFN_RELAY*/
	{1, SvrSetParam}, /*0x04, UPAFN_SETPARA*/
	{1, SvrCtrlCmd}, /*0x05, UPAFN_CTRL*/
	{0, SvrKeyConsult}, /*0x06*/
	{0, NULL}, /*0x07*/
	{0, NULL}, /*0x08, UPAFN_CASCADE*/
	{0, SvrReqConfig}, /*0x09, UPAFN_REQCFG*/
	{0, SvrQueryParam}, /*0x0a, UPAFN_QRYPARA*/
	{0, NULL}, /*0x0b, UPAFN_QRYTASK*/
	{0, SvrQueryClassOne}, /*0x0c, UPAFN_QRYCLS1*/
	{0, NULL},//SvrQueryClassTwo}, /*0x0d, UPAFN_QRYCLS2*/
#ifdef ALARM_MODULE
	{0, SvrQueryClassThree}, /*0x0e, UPAFN_QRYCLS3*/
#else
	{0, NULL}, /*0x0e*/
#endif
	{1, SvrTranFile}, /*0x0f, UPAFN_TRANFILE*/
	{1, NULL}, /*0x10, UPAFN_FORWARD*/
	{0, NULL}, /*0x11*/
	{0, NULL}, /*0x12*/
	{0, NULL}, /*0x13*/
};
#define SVRAFN_MAX		((int)(sizeof(srvmsg_func)/sizeof(srvmsg_func[0])))

/**
* @brief 生成密码错误告警
* @param pwd 错误的密码
*/ 
static void MakePwdErrAlarm(unsigned char *pwd)
{
#ifdef ALARM_MODULE
	alarm_t buf;

	memset((unsigned char *)&buf, 0, sizeof(alarm_t));
	buf.erc = 20;
	buf.len = 17;
	buf.data[0] = pwd[0];
	buf.data[1] = pwd[1];
	buf.data[16] = SvrAddrRecord;

	MakeAlarm(ALMFLAG_ABNOR, &buf);
#endif
}

/**
* @brief DLMS服务器命令处理
* @param itf 接口编号
*/

int SvrMessageDlms(unsigned char itf)
{
	int rt;
	unsigned char *prcv = UPLINK_RCVBUF(itf);
	unsigned char *psnd = UPLINK_SNDBUF(itf);
	DebugPrint(LOGTYPE_DEBUG, "\r\nSvrMessageDlms\r\n");
	rt = FindHdlcPcket(prcv,UPLINK_RCVMAX(itf));
	if(rt > 0)
	{
		struct data_t cmd,echo;
	    cmd.len = rt;
		cmd.data = prcv;
		echo.len = UPLINK_RCVMAX(itf);
		echo.data = psnd;
//		DebugPrint(LOGTYPE_DEBUG, "\r\nDlmsForward, cmd.len=%d\r\n", cmd.len);
//		PrintHexLog(LOGTYPE_DEBUG,cmd.data,cmd.len);
		DlmsForward(&cmd,&echo);
//		DebugPrint(LOGTYPE_DEBUG, "\r\nDlmsForward, echo.len=%d\r\n", echo.len);

		if(echo.len>0)
			UplinkInterface[itf].rawsend(echo.data, echo.len);
	}
	return 1;
}

/**
* @brief 服务器命令处理
* @param itf 接口编号
*/
int SvrMessageProc(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	const struct srvmsg_func_t *plist;
	unsigned char *paux;

	DebugPrint(LOGTYPE_UPLINK, "srv msg proc, len=%d, f=%02X\r\n", UPLINKAPP_LEN(prcv), prcv->data[0]);

	//if(UplinkCheckEchoPkt(prcv)) return;
	if(0 == (prcv->ctrl&UPCTRL_PRM) && prcv->afn != UPAFN_TRANFILE) return -1;
	if(prcv->afn >= SVRAFN_MAX) return -2;
	plist = &srvmsg_func[prcv->afn];
	if(NULL == plist->pf) return -3;

	TpvFlag[itf] = 0;
	if(prcv->seq&UPSEQ_TPV) {
		TpvFlag[itf] = 1;
		if(UPLINKAPP_LEN(prcv) <= LEN_UPLINK_TIMETAG) return -4;

		paux = prcv->data;
		paux += UPLINKAPP_LEN(prcv);
		paux -= LEN_UPLINK_TIMETAG;
		TimeTag[itf].pfc = paux[0];
		TimeTag[itf].time[0] = paux[1];
		TimeTag[itf].time[1] = paux[2];
		TimeTag[itf].time[2] = paux[3];
		TimeTag[itf].time[3] = paux[4];
		TimeTag[itf].dly = paux[5];

		if(TimeTag[itf].dly) {  //计算时间有效
			sysclock_t clk;
			utime_t time1, time2;
			int diff;

			SysClockReadCurrent(&clk);
			clk.day = TimeTag[itf].time[3];
			clk.hour = TimeTag[itf].time[2];
			clk.minute = TimeTag[itf].time[1];
			BcdToHex(&(clk.day), 3);
			time1 = SysClockToUTime(&clk);
			time2 = UTimeReadCurrent();
			if(time1 > time2) diff = time1 - time2;
			else diff = time2 - time1;
			diff /= 60;
			if(diff > ((int)TimeTag[itf].dly&0xff)) return -5;
		}

		UPLINKAPP_LEN(prcv) -= LEN_UPLINK_TIMETAG;
	}

	psnd->seq = prcv->seq & 0x0f;
	psnd->seq |= UPSEQ_SPKT;
	psnd->grp = prcv->grp;
	psnd->grp &= 0xfe;
//	SvrAddrRecord = (prcv->grp)>>1;

	if(plist->needpass && (prcv->ctrl&UPCTRL_PRM)) {
		unsigned char grpnum = 0;//单地址
		unsigned short termaddr = MAKE_SHORT(prcv->sn);
		
		if(UPLINKAPP_LEN(prcv) <= UPPWD_LENGTH) return -6;

		paux = prcv->data;
		paux += UPLINKAPP_LEN(prcv);
		paux -= UPPWD_LENGTH;

		if(termaddr == 65535) grpnum = 9;//广播地址
		
		if(CheckPassword(paux, &prcv->ctrl, itf, grpnum)) {
			if(CountPwdError[itf] < MAXCNT_PWDERR) {
				CountPwdError[itf]++;
				if(CountPwdError[itf] >= MAXCNT_PWDERR) {
					MakePwdErrAlarm(paux);
				}
			}
			return -7;
		}
		
		CountPwdError[itf] = 0;
		UPLINKAPP_LEN(prcv) -= UPPWD_LENGTH;
	}
	
///mark_exec:
	(*plist->pf)(itf);
	
	return 0;
}

/**
* @brief 检查AFN是否为终端会响应的命令
* @param afn AFN
* @return 会响应返回1, 否则返回0
*/
int IsSvrMsgAfn(unsigned char afn)
{
	if(afn >= SVRAFN_MAX) return 0;

	if(srvmsg_func[afn].pf != NULL) return 1;
	else return 0;
}
/*
int is_svrmsg_cmd(unsigned char afn)
{
	srvmsg_func_t *plist = (srvmsg_func_t *)srvmsg_func;

	while(0xff != plist->afn) {
		if(afn == plist->afn) return 1;

		plist++;
	}

	return 0;
}*/
#if 0	
/**
* @brief      ESAMEcho
* @param   itf   channel   
* @param     ERRFlag  
*                   0  正确
*                   1  签名校验错误
*                   2  密文校验错误
*                   3  mac校验错误
*                   4  其他错误
*/
static void ESAMEcho(unsigned char itf,int ERRFlag){
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	uplink_ack_t *pack = (uplink_ack_t *)psnd->data;

	psnd->ctrl = UPECHO_USRDATA;
	psnd->afn = UPAFN_ECHO;
	pack->da[0] = pack->da[1] = 0;
	pack->dt[1] = 0;

	if( (ERRFlag == 1)||(ERRFlag == 2) ){
		pack->dt[0] = 0x08;//f4
	 	psnd->data[4] = ERRFlag;
		UPLINKAPP_LEN(psnd) = 21;
		memset(psnd->data+5,0xff,16);
	 }else if(ERRFlag == 3){
	 	pack->dt[0] = 0x08;//f4
	 	psnd->data[4] = ERRFlag;
		UPLINKAPP_LEN(psnd) = 21;
		//KeyConsultF5(psnd->data+5);
	}else if(ERRFlag == 4){
	 	pack->dt[0] = 0x02;//f2
	       UPLINKAPP_LEN(psnd) = 4;
	}else if(ERRFlag == 0){
		 pack->dt[0] = 0x01;//f1
	        UPLINKAPP_LEN(psnd) = 4;
	}else{
		 return;
	}
	SvrEchoSend(itf, psnd);
}
#endif
