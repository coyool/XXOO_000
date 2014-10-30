/**
* uplink_dl.c -- 上行通信数据链路层
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-18
* 最后修改时间: 2009-5-18
*/

#include <stdio.h>

#include "plat_include/debug.h"
#include "plat_include/sleep.h"
#include "plat_include/plat_defines.h"
//#include "include/sys/schedule.h"
#include "app_include/lib/align.h"
#include "app_include/param/term.h"
#include "app_include/param/unique.h"
#include "app_include/param/mix.h"
#include "app_include/monitor/runstate.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
#include "app_include/uplink/svrcomm.h"
#include "app_include/uplink/hdlc.h"
#include "app_include/lib/crc.h"


static struct uplink_timer_t {
	int cnt;
	int max;
	int flag;
} UplinkTimer[UPLINKITF_NUM];

static struct uplink_fsm_t {
	unsigned char *pbuf;
	unsigned short cnt;
	unsigned short maxlen;
	unsigned char stat;
	unsigned char proto;//通讯协议类型:1=376.1,2=DLMS(HDLC)
} UplinkFsm[UPLINKITF_NUM];

extern int SvrCommFlag;
int IRrcvFlag = 0;

/**
* @brief 检查数据包的合法性
* @param itf 接口编号
* @param pkt 数据包指针
* @return 0合法, 否则非法
*/
static int CheckPacket(unsigned char itf, const uplink_pkt_t *pkt)
{
	unsigned short i, len;
	const unsigned char *puc;
	unsigned char chk;

	len = UPLINKAPP_LEN(pkt);
	if(len > UPLINK_RCVMAX(itf)) return -1;
	len += LEN_UPLINKHEAD;

	puc = &pkt->ctrl;
	chk = 0;
	for(i=0; i<len; i++) chk += *puc++;

	if(chk != *puc++) return -2;
	if(UPLINK_TAIL != *puc) return -3;


	if(pkt->area[0] != ParaUni.addr_area[0] || pkt->area[1] != ParaUni.addr_area[1]) {
		return -4;
	}
  
	if((0xff == pkt->sn[0]) && (0xff == pkt->sn[1])) {
		//pkt->grp &= 0xfe;
		return 0;
	}
        
    if(pkt->sn[0] != ParaUni.addr_sn[0] || pkt->sn[1] != ParaUni.addr_sn[1])
			return -7;

	if(pkt->ctrl&UPCTRL_DIR) return -8;

	return 0;

}

/**
* @brief 将数据包格式化为上行通信格式
* @param itf 接口编号
* @param pkt 数据包指针
* @return 成功返回数据包长度, 失败返回-1
*/
static int MakePacket(unsigned char itf, uplink_pkt_t *pkt)
{
	int i, len;
	unsigned char *puc;
	unsigned char chk;
	unsigned short us;

	len = UPLINKAPP_LEN(pkt) & 0xffff;
	AssertLogReturn(len > UPLINK_SNDMAX(itf), -1, "invalid len(%d)\n", len);
	len += LEN_UPLINKHEAD;

	us = (unsigned short)len;
	us <<= 2;
    us |= 0x0002;
	pkt->head = pkt->dep = UPLINK_HEAD;
	pkt->len1[0] = (unsigned char)us;
	pkt->len1[1] = (unsigned char)(us>>8);
	pkt->len2[0] = pkt->len1[0];
	pkt->len2[1] = pkt->len1[1];
	//pkt->grp = 0;

	puc = (unsigned char *)&pkt->ctrl;
	chk = 0;
	for(i=0; i<len; i++) chk += *puc++;

	*puc++ = chk;
	*puc = UPLINK_TAIL;
	len += LEN_UPLINKDL;

	return(len);
}

/**
* @brief 设置接收定时器
* @param itf 接口编号
* @param max 超时时间(100ms)
*/
static void SetUpTimer(unsigned char itf, int max)
{
	UplinkTimer[itf].cnt = 0;
	UplinkTimer[itf].max = max;
	UplinkTimer[itf].flag = 1;
}

/**
* @brief 停止定时器
*/
static void StopUpTimer(unsigned char itf)
{
	UplinkTimer[itf].flag = 0;
}

/**
* @brief 清除接收状态
* @param itf 接口编号
*/
void UplinkClearState(unsigned char itf)
{
	UplinkFsm[itf].pbuf = UPLINK_RCVBUF(itf);
	UplinkFsm[itf].stat = 0;
	UplinkFsm[itf].cnt = 0;
	UplinkFsm[itf].maxlen = 0;

	StopUpTimer(itf);
}

/**
* @brief 接收定时器处理
*/
static void UpTimerProc(unsigned char itf)
{
	if(!UplinkTimer[itf].flag) {
		UplinkFsm[itf].pbuf = UPLINK_RCVBUF(itf);
		UplinkFsm[itf].stat = 0;
		UplinkFsm[itf].cnt = 0;
		UplinkFsm[itf].maxlen = 0;
		return;
	}

	UplinkTimer[itf].cnt++;
	if(UplinkTimer[itf].cnt >= UplinkTimer[itf].max) {
		UplinkClearState(itf);
	}

	return;
}

//#define UPLINK_NEEDPRINT(itf)    (LOGITF_SVRCOMM != GetLogInterface())
//#define UPLINK_NEEDPRINT(itf)    ((itf) > UPLINKITF_ETHMTN)

/**
* @brief 打印通信帧内容
* @param pkt 通信帧指针
* @param prompt 提示字符串指针
* @param applen 通信帧应用层长度
*/
static void UplinkPrintPkt(const uplink_pkt_t *pkt, const char *prompt, unsigned short applen)
{
	unsigned short len;
	//int logolvl = GetLogType();

	//if(logolvl < LOGTYPE_SHORT) return;

	len = applen;
	PrintLog(LOGTYPE_UPLINK, "%s: C=%02XH, AFN=%02X, len=%d\r\n", prompt, pkt->ctrl, pkt->afn, len);

	//if(logolvl != LOGTYPE_UPLINK) return;

	len += MINLEN_UPLINKPKT;

	PrintHexLog(LOGTYPE_UPLINK, (unsigned char *)pkt, len);
}

/**
* @brief 接收数据帧(100ms运行一次)
* @param itf 接口编号
* @return 返回0表示接收到一个正确的帧, 返回-1表示没有
*/
/*int UplinkRecvPkt(unsigned char itf)
{
	struct uplink_fsm_t *fsm = &UplinkFsm[itf];
	const uplinkitf_t *pitf = &UplinkInterface[itf];
	uplink_pkt_t *pkt;
	HDLC_STRUCT hdlc;
	int rev = 0;
	unsigned short len1, len2;
	int rt;

	UpTimerProc(itf);
	UplinkClearState(itf);
	rev = (*pitf->getchar)(pitf->rcvbuf);
	if(rev == -ERRGPRS)
	{
		return -ERRGPRS;	
	}
	if(rev>0)
	{

		if(DLMS_HEAD == *(fsm->pbuf))	
		{
			int rt;
			rt = UnparseDLMSPacket(rev,fsm->pbuf,&hdlc); 
			 if(rt > 0)
			 {
			 	return PROTO_DLMS;
			 }
		}
		while(rev--) 
		{

			switch(fsm->stat) 
			{
			case 0:
			 	if(UPLINK_HEAD == *(fsm->pbuf)) 
				{
					fsm->stat = 1;
					fsm->maxlen = 4;
					fsm->pbuf++;
					fsm->proto = 1;
					SetUpTimer(itf, pitf->timeout);
				}
				break;
			case 1:
				fsm->cnt++;
				fsm->pbuf++;
				if(fsm->cnt >= fsm->maxlen) fsm->stat = 2;
				break;

			case 2:
				if(UPLINK_HEAD != *(fsm->pbuf)) 
				{
					break;
				}
				fsm->pbuf++;

				//@change later: 加上规约判断
				pkt = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
				len1 = ((unsigned short)pkt->len1[1]<<8) + (unsigned short)pkt->len1[0];
				len1 >>= 2;
				len2 = ((unsigned short)pkt->len2[1]<<8) + (unsigned short)pkt->len2[0];
				len2 >>= 2;
				if((len1 != len2) || (len1 > pitf->rcvmax) || (len1 < LEN_UPLINKHEAD)) 
				{
					UplinkClearState(itf);
					break;
				}

				len2 -= LEN_UPLINKHEAD;
				UPLINKAPP_LEN(pkt) = len2;

				len1 += 1;
				fsm->cnt = 0;
				fsm->maxlen = len1;

				len1 /= 200;
				len1 *= 10;
				len1 += pitf->timeout;
				SetUpTimer(itf, len1);
				fsm->stat = 3;
				break;
			case 3:
				fsm->pbuf++;
				fsm->cnt++;
				if(fsm->cnt >= fsm->maxlen) fsm->stat = 4;
				break;
			case 4:
				if(UPLINK_TAIL != *(fsm->pbuf)) 
				{
					break;
				}

				fsm->pbuf++;

				StopUpTimer(itf);
				pkt = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
				UplinkPrintPkt(pkt, "RECV", UPLINKAPP_LEN(pkt));

				rt = CheckPacket(itf, pkt);
				if(rt!=0)
				{
					PrintLog(LOGTYPE_DEBUG,"recv chk err(%d)\n",rt);
					break;
				}
				else 
				{
					UplinkClearState(itf);
					SvrCommFlag = 1;
					return PROTO_376;
				}
			default:
				break;
			}
		}

	 }
		return -1;
} */
int UplinkRecv376Pkt(unsigned char itf,unsigned char uc)
{
	struct uplink_fsm_t *fsm = &UplinkFsm[itf];
	const uplinkitf_t *pitf = &UplinkInterface[itf];
	uplink_pkt_t *pkt;
	switch(fsm->stat) 
	{
	case 1:
		fsm->cnt++;
		*fsm->pbuf = uc;
		fsm->pbuf++;

		if(fsm->cnt >= fsm->maxlen) fsm->stat = 2;
		break;

	case 2:
		{
			unsigned short len1, len2;
			if(UPLINK_HEAD != uc) 
			{
				UplinkClearState(itf);
			}
			*fsm->pbuf = uc;
			fsm->pbuf++;
	
			//@change later: 加上规约判断
			pkt = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
			len1 = ((unsigned short)pkt->len1[1]<<8) + (unsigned short)pkt->len1[0];
			len1 >>= 2;
			len2 = ((unsigned short)pkt->len2[1]<<8) + (unsigned short)pkt->len2[0];
			len2 >>= 2;
			if((len1 != len2) || (len1 > pitf->rcvmax) || (len1 < LEN_UPLINKHEAD)) 
			{
				UplinkClearState(itf);
				break;
			}
	
			len2 -= LEN_UPLINKHEAD;
			UPLINKAPP_LEN(pkt) = len2;
	
			len1 += 1;
			fsm->cnt = 0;
			fsm->maxlen = len1;
	
			len1 /= 200;
			len1 *= 10;
			len1 += pitf->timeout;
			SetUpTimer(itf, len1);
			fsm->stat = 3;
		}
		break;
	case 3:
		*fsm->pbuf = uc;
		fsm->pbuf++;
		fsm->cnt++;
		if(fsm->cnt >= fsm->maxlen) fsm->stat = 4;
		break;
	case 4:	 
		{
			int rt;
			if(UPLINK_TAIL != uc) 
			{
				UplinkClearState(itf);
				break;
			}
			*fsm->pbuf = uc;
			fsm->pbuf++;
	
			StopUpTimer(itf);
			pkt = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
			UplinkPrintPkt(pkt, "RECV", UPLINKAPP_LEN(pkt));
	
			rt = CheckPacket(itf, pkt);
			if(rt!=0)
			{
				PrintLog(LOGTYPE_DEBUG,"recv chk err(%d)\n",rt);
				UplinkClearState(itf);
				break;
			}
			else 
			{
				UplinkClearState(itf);

				return PROTO_376;
			}
		}
	default:
		UplinkClearState(itf);
		break;
	}	
	return 0;
}
int UplinkRecvDlmsPkt(unsigned char itf,unsigned char uc)
{
	struct uplink_fsm_t *fsm = &UplinkFsm[itf];
	const uplinkitf_t *pitf = &UplinkInterface[itf];
	const unsigned char* data = UPLINK_RCVBUF(itf);
//	PrintLog(LOGTYPE_DEBUG,"DLMS itf(%d) r=%02X,s=%d\r\n",itf, uc, fsm->stat);
	switch(fsm->stat)
	{
	case 1:
		fsm->cnt++;
		*fsm->pbuf = uc;
		fsm->pbuf++;

		if(fsm->cnt >= fsm->maxlen) fsm->stat = 2;
		break;
	case 2:
		{
			unsigned short P_Len,F_Type;
			F_Type = data[1];
			F_Type <<= 8;
			F_Type += data[2];
			P_Len = F_Type&0x07ff;
			if(P_Len > pitf->rcvmax) 
			{
				UplinkClearState(itf);
				break;
			}
			fsm->cnt = 0;
			*fsm->pbuf = uc;
			fsm->pbuf++;
			fsm->maxlen = P_Len-3;
			//PrintLog(LOGTYPE_DEBUG,"DLMS itf(%d) fsm->maxlen=%d\r\n",itf, fsm->maxlen);
			P_Len /= 200;
			P_Len *= 10;
			P_Len += pitf->timeout;
			SetUpTimer(itf, P_Len);
			fsm->stat = 3;
		}
		break;
	case 3:
		*fsm->pbuf = uc;
		fsm->pbuf++;
		fsm->cnt++;
		if(fsm->cnt >= fsm->maxlen) fsm->stat = 4;	
		break;
	case 4:
		{
			if(DLMS_HEAD != uc) 
			{
				UplinkClearState(itf);
				break;
			}
			*fsm->pbuf = uc;
			if(pppfcs16(PPPINITFCS16,UPLINK_RCVBUF(itf)+1,fsm->pbuf-UPLINK_RCVBUF(itf)-1)!=PPPGOODFCS16)
			{
				UplinkClearState(itf);
				PrintLog(LOGTYPE_DEBUG,"DLMS CRC ERROR\r\n");
				break;//CRC Error!
			}
			else
			{
				PrintLog(LOGTYPE_DEBUG,"DLMS CRC OK\r\n");
				PrintHexLog(LOGTYPE_DEBUG,UPLINK_RCVBUF(itf),fsm->pbuf-UPLINK_RCVBUF(itf)+1);
				UplinkClearState(itf);
				return PROTO_DLMS;				
			}	
		}
 	}
	return 0;
}
/**
* @brief 接收数据帧(100ms运行一次)
* @param itf 接口编号
* @return 返回0表示接收到一个正确的帧, 返回-1表示没有
*/
int UplinkRecvPkt(unsigned char itf)
{
	struct uplink_fsm_t *fsm = &UplinkFsm[itf];
	const uplinkitf_t *pitf = &UplinkInterface[itf];
	unsigned char uc;

	int rev = 0;
	UpTimerProc(itf);
	while(1) 
	{
		rev = (*pitf->getchar)(&uc);
	   	if(rev == 1)
		{
			//PrintLog(LOGTYPE_DEBUG,"GPRS:%d\r\n",uc);
			//if(UPLINKITF_SERIAL == itf);
			//PrintLog(LOGTYPE_DEBUG,"itf(%d) r=%02X,s=%d\r\n",itf, uc, fsm->stat);
			if(fsm->stat == 0)
			{
				if(UPLINK_HEAD == uc) 
				{
					fsm->stat = 1;
					fsm->maxlen = 4;
					*fsm->pbuf = uc;
					fsm->pbuf++;
					fsm->proto = 1;
					SetUpTimer(itf, pitf->timeout);
				}
				else if(DLMS_HEAD == uc)
				{
					//PrintLog(LOGTYPE_DEBUG,"GPRS DLMS:%02X\r\n",uc);
					fsm->stat = 1;
					fsm->maxlen = 2;
					*fsm->pbuf = uc;
					fsm->pbuf++;
					fsm->proto = 2;
					SetUpTimer(itf, pitf->timeout);
				}	
			}
			else
			{
				if(fsm->proto == 1)
				{
					int rt;
					rt = UplinkRecv376Pkt(itf,uc);
					if(rt > 0)
						return rt;
				}
				else if(fsm->proto == 2)
				{
					//DLMS数据帧
					int rt;
					//PrintLog(LOGTYPE_DEBUG,"GPRS DLMS:%02X\r\n",uc);
					rt = UplinkRecvDlmsPkt(itf,uc);
					if(rt > 0)
						return rt;
				}
			}
		}
		else
			return rev;
	}

}
/**
* @brief 发送一个通信帧
* @param itf 接口编号
* @param pkt 通信帧指针
* @return 成功返回0, 否则失败
*/
int UplinkSendPkt(unsigned char itf, uplink_pkt_t *pkt)
{
	int len;
	unsigned short lenapp;
	unsigned char seq;

	pkt->ctrl |= UPCTRL_DIR;
	pkt->area[0] = ParaUni.addr_area[0];
	pkt->area[1] = ParaUni.addr_area[1];
	pkt->sn[0] = ParaUni.addr_sn[0];
	pkt->sn[1] = ParaUni.addr_sn[1];

	lenapp = UPLINKAPP_LEN(pkt);
	seq = pkt->seq;
	len = MakePacket(itf, pkt);

	if(-1 == len) {
		UPLINKAPP_LEN(pkt) = lenapp;
		pkt->seq = seq;
		return 1;
	}
	
	UplinkPrintPkt(pkt, "SEND", lenapp);

	if((*UplinkInterface[itf].rawsend)((unsigned char *)pkt, len)) {
		UPLINKAPP_LEN(pkt) = lenapp;
		pkt->seq = seq;
		return 1;
	}

//	if(UPLINKITF_IR == itf) IRrcvFlag =1;
	UPLINKAPP_LEN(pkt) = lenapp;
	pkt->seq = seq;
	return 0;
}

/**
* @brief 获取超时时间和重发次数
* @param ptime 超时时间指针(1s)
* @param retry 重发次数指针
*/
static void GetTimeout(int *ptime, int *retry)
{
	int i;

	i = (int)(ParaTerm.tcom.rsnd)&0x0fff;
	if(0 == i) i = 300;
	*ptime = i;

	i = (int)(ParaTerm.tcom.rsnd)&0x3000;
	i >>= 12;
	*retry = i+1;
}

/**
* @brief 判断是否回应报文
* @pkt 通信帧指针
* @return 是回应报文返回1, 否则返回0
*/
static int IsEchoPkt(uplink_pkt_t *pkt)
{
	unsigned char ctrl;

	ctrl = pkt->ctrl;
	if(0 == (ctrl&UPCTRL_PRM)) return 1;
	else if(0 == pkt->afn) return 1;

	return 0;
}
extern int DoSms(void);
/**
* @brief 发送报文并等待回应
* @param itf 接口编号
* @param pkt 通信帧指针
* @return 成功返回0,否则返回错误编码
*/
static int UplinkSendWait(unsigned char itf, uplink_pkt_t *psnd)
{
	uplinkitf_t *pitf = (uplinkitf_t *)&UplinkInterface[itf];
	int i, j,rt;
	int times=300, retry=3;
	utime_t WaitTime;
	uplink_pkt_t *prcv;

	psnd->seq |= UPSEQ_CON;

	prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	
	rt = UplinkRecvPkt(itf);
	if(rt == PROTO_376) 
	{
		if(!IsEchoPkt(prcv)) 
			return UPRTN_FAILRCV;
	}
	else if(rt == PROTO_DLMS)
	{
		 return UPRTN_OKDLMS;
	}


	GetTimeout(&times, &retry);

	PrintLog(LOGTYPE_SHORT, "logon Time:%d Retry%d.\r\n",times,retry);

	for(i=0; i<retry; i++) {
		//PrintLog(LOGTYPE_SHORT, "logon Retry%d.\r\n",i);
		if(DoSms() == 1)
			 return UPRTN_TIMEOUT;
		if(!(*pitf->linestat)()) return UPRTN_FAIL;
		PrintLog(LOGTYPE_SHORT, "Send logon %s.\r\n",UTimeFormat(UTimeReadCurrent()));
		if(UplinkSendPkt(itf, psnd)) return UPRTN_FAIL;
		WaitTime = UTimeReadCurrent() + times;
		for(j=0; j<times; j++) 
		{
			if(UTimeReadCurrent()>WaitTime)
				break;
			if(UTimeReadCurrent()%30  == 0)
			{
				if(DoSms()==1)
					return UPRTN_TIMEOUT;	
			}
			//PrintLog(LOGTYPE_SHORT, "logon waited %d.\r\n",UTimeReadCurrent());
			rt = UplinkRecvPkt(itf); 
			if(rt == PROTO_376) 
			{
				PrintLog(LOGTYPE_SHORT, "Recive logon %s.\r\n",UTimeFormat(UTimeReadCurrent()));
				if(!IsEchoPkt(prcv)) 
					return UPRTN_OKRCV;
				else 
					return UPRTN_OK;
			}
			if(rt == PROTO_DLMS)
			{
				 return UPRTN_OKDLMS;
			}
			if(rt == -ERRGPRS)
			{
				return -ERRGPRS;
			}
			Sleep(50);
		}
	}

	return UPRTN_TIMEOUT;
}

/**
* @brief 延时一段时间(根据接口和帧长度)
* @param itf 接口编号
* @param pkt 发送帧指针
* @return 成功返回0,否则返回错误编码
*/
/*
int UplinkDelay(unsigned char itf, const uplink_pkt_t *pkt)
{
	uplinkitf_t *pitf = (uplinkitf_t *)&UplinkInterface[itf];
	int i, timeout;

	timeout = (int)UPLINKAPP_LEN(pkt)&0xffff;
	if(timeout > UPLINK_SNDMAX(itf)) return UPRTN_FAIL;
	timeout += LEN_UPLINKHEAD;

	timeout >>= 7;
	timeout *= UPLINK_TIMEOUT(itf);
	timeout += UPLINK_TIMEOUT(itf)*3;
	timeout /= 10;

	for(i=0; i<timeout; i++) {
		if(!(*pitf->linestat)()) return UPRTN_FAIL;

		if(UplinkRecvPkt(itf)==PROTO_376) {
			pkt = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
			if(pkt->ctrl & UPCTRL_PRM) {
				if(IsSvrMsgAfn(pkt->afn)) return UPRTN_OKRCV;
			}
		}

		Sleep(10);
	}

	return UPRTN_OK;
}
*/
/**
* @brief 主动发送数据
* @param itf 接口编号
* @param flag 发送标志, 0-不等待响应, 1-等待响应
* @return 成功返回0,否则返回错误编码
*/
int UplinkActiveSend(unsigned char itf, unsigned char flag, uplink_pkt_t *psnd)
{
	static unsigned char fseq = 0;

	psnd->seq = fseq&0x0f;
	fseq++;
	psnd->seq |= UPSEQ_SPKT;
	psnd->ctrl |= UPCTRL_PRM;
	psnd->grp = 0;


	if((!flag) || (UPLINK_ATTR(itf)&UPLINKATTR_NOECHO))
		return(UplinkSendPkt(itf, psnd));
	else {
		psnd->seq |= UPSEQ_CON;
		return(UplinkSendWait(itf, psnd));
	}
}

/**
* @brief 登陆到服务器
* @param itf 接口编号
* @return 成功返回0,否则失败
*/
int UplinkLogon(unsigned char itf)
{
	uplink_pkt_t *pkt = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	uplink_duid_t *pduid;
	int pid = -1;

	pduid = (uplink_duid_t *)pkt->data;
	pkt->ctrl = UPCMD_LINKTEST|UPCTRL_PRM;
	pkt->afn = UPAFN_LINKTEST;
//	pid = SvrCommPeekNoteid();
	if(pid == -1)
		pduid->da[0] = pduid->da[1] = 0;   //p0
	else
	{
		UPLINK_FID(pid,pduid);
	}
	pduid->dt[0] = 1;    //F1
	pduid->dt[1] = 0;
	UPLINKAPP_LEN(pkt) = 4;

	if(UplinkActiveSend(itf, 1, pkt)) goto MARK_FLOGFAIL;

	pkt = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	pduid = (uplink_duid_t *)pkt->data;
	if((0 != pduid->dt[1]) || (2 == pduid->dt[0])) goto MARK_FLOGFAIL;

	PrintLog(LOGTYPE_SHORT, "logon ok.\r\n");
//	if(pid != -1)
//		SvrCommRemoveNoteid(pid);
	//SvrSendFileInfo(itf);
	return 0;

MARK_FLOGFAIL:
	PrintLog(LOGTYPE_SHORT, "logon fail.\r\n");
	return 1;
}

/**
* @brief 链路检测
* @param itf 接口编号
* @return 成功返回0,否则返回错误编码
*/
int UplinkLinkTest(unsigned char itf)
{
	uplink_pkt_t *pkt = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	uplink_duid_t *pduid;
	int rtn;
	int pid = 0;

	pduid = (uplink_duid_t *)pkt->data;
		pid = SvrCommPeekNoteid();

		pkt->ctrl = UPCMD_LINKTEST|UPCTRL_PRM;
		pkt->grp = 0;
		pkt->afn = UPAFN_LINKTEST;
		if(pid == -1)
		{
			pduid->da[0] = pduid->da[1] = 0;   //p0
		}
		else
		{
			PrintLog(LOGTYPE_DEBUG,"SvrCommPeekNoteid:%d\r\n",pid);
			UPLINK_PID1(pid+1,pduid);
		}
		pduid->dt[0] = 0x04;    //F3
		pduid->dt[1] = 0;
		UPLINKAPP_LEN(pkt) = 4;
	
		//@change later;
		/*if(stat_acd)
		{
			unsigned char *paux;
	
			paux = pkt->data;
			paux += UPLINKAPP_LEN(pkt);
	
			pkt->ctrl &= 0xef;
			pkt->ctrl |= 0x20;
			get_alarmec(paux);
			UPLINKAPP_LEN(pkt) += 2;
		}*/
	
		rtn = UplinkActiveSend(itf, 1, pkt);
		if((UPRTN_FAIL == rtn)&&(UPRTN_TIMEOUT == rtn)) 
		{
			PrintLog(LOGTYPE_SHORT, "link test fail\r\n");

		}
		else if(UPRTN_OK == rtn) 
		{
			PrintLog(LOGTYPE_SHORT, "link test ok.\r\n");
			if(pid != -1)
				SvrCommRemoveNoteid(pid);
		}else
		{
			return rtn;
		}
	
	return(rtn);
}

