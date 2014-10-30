/**
* plc_3762.c -- GDW 376.2协议处理
* 
* 作者: zhuzhiqiang
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "plat_include/plc_3762.h"
#include "app_include/lib/align.h"
#include "plat_include/plat_uart.h"
#include "plat_include/debug.h"
#include "plat_include/sys_config.h"
#ifdef DEFINE_PLCOMM

#define MIN_PKT_LEN				15
#define PKT_DL_LEN				12
#define PL3762_RCVTIMEOUT		6

#define MAX_PLCOMM_RETRY		3
#define PLCOMM_TIMEOUT			30

#define PLMET_OP_NUM			16

static int FillDest(const plc_dest_t *dest, unsigned char *buf)
{
	int len;

	len = 0;
	if(dest->route.level <= PLC_ROUTENUM) {
		len = dest->route.level * 6;
	}

	memcpy(buf, dest->src, 6); buf += 6;
	if(len) memcpy(buf, dest->route.addr, len); buf += len;
	memcpy(buf, dest->dest, 6);

	return (len+12);
}

/**
* @brief 发送报文
* @param pmodule 载波控制模块指针
* @param dest 目标地址, 为NULL时表示不包含地址域
* @param special 1-info字段由上层填写, 0-info字段自动填充
* @return 成功返回0, 失败返回负数(PLCERR_XXX)
*/
int Pl3762SendPkt(struct amr_module_t *pmodule, const plc_dest_t *dest, int special)
{
	unsigned char *pbuf = pmodule->link_dl_buf;
	int addrlen, wlen, i;
	unsigned char chk;
	plc3762_pkt_t *pkt = (plc3762_pkt_t *)pmodule->link_send_buf;

	//AssertLogReturn(NULL == Plc3762CommFunc[pmodule->port-1].psend, PLCERR_INVALID, "null func(%d)", pmodule->port);
	AssertLogReturn(pkt->len > MAX_3762PKT_LEN, PLCERR_INVALID, "len too long(%d)", pkt->len);

	if(NULL != dest) 
		addrlen = FillDest(dest, pbuf+10);
	else 
		addrlen = 0;
	wlen = addrlen + pkt->len + 15;

	*pbuf++ = 0x68;
	DEPART_SHORT(wlen, pbuf); 
	pbuf += 2;
	*pbuf++ = pkt->ctrl;
	if(special) {
		smallcpy(pbuf, pkt->info, 6);
		pbuf += 6;
	}
	else {
		if(addrlen) 
			*pbuf++ = (dest->route.level << 4) + 0x05;
		else 
			*pbuf++ = 0;
		*pbuf++ = 0;
		*pbuf++ = 0;
		*pbuf++ = 0;
		*pbuf++ = 0;
		*pbuf++ = 0;
	}

	pbuf += addrlen;

	*pbuf++ = pkt->afn;
	*pbuf++ = pkt->dt[0];
	*pbuf++ = pkt->dt[1];

	memcpy(pbuf, pkt->data, pkt->len);

	pbuf = pmodule->link_dl_buf + 3;
	for(chk=0,i=0; i<(wlen-5); i++) 
		chk += *pbuf++;
	*pbuf++ = chk;
	*pbuf = 0x16;
	
	PrintLog(LOGTYPE_DOWNLINK, "PL3762(%d) send(%d): AFN=%02X, DT=%02X%02X\n", pmodule->port, wlen, pkt->afn, pkt->dt[0], pkt->dt[1]);
	PrintHexLog(LOGTYPE_DOWNLINK, pmodule->link_dl_buf, wlen);
	//(*Plc3762CommFunc[pmodule->port-1].psend)(Plc3762CommFunc[pmodule->port-1].port, pmodule->link_dl_buf, wlen);
	//(*pmodule->commfunc.psend)(pmodule->commfunc.uartport, pmodule->link_dl_buf, wlen);
	
	UartSendData(PLC_PORT, pmodule->link_dl_buf, wlen);
		
	return 0;
}

void Pl3762StartRecv(struct amr_module_t *pmodule)
{
	pmodule->link_dl_recvlen = 0;
	pmodule->link_dl_recvstat = 0;
	pmodule->link_dl_recvmax = 0;
	pmodule->link_dl_recvtimes = 0;
}

static int Pl3762CheckRcvPacket(struct amr_module_t *pmodule)
{
	unsigned char chk;
	int i, len;
	unsigned char *pbuf = pmodule->link_recv_buf;
	int addrlen;
	plc3762_pkt_t *prcv;

	len = MAKE_SHORT(pbuf+1);
	if(len < MIN_PKT_LEN || len > (MAX_3762PKT_LEN+15)) return 1;

	if((pbuf[3]&0x80) == 0) return 4;

	if(pbuf[len-1] != 0x16) return 2;

	for(i=3,chk=0; i<(len-2); i++) chk += pbuf[i];
	if(chk != pbuf[len-2]) return 3;
	prcv = (plc3762_pkt_t *)pbuf;

	if(prcv->info[0]&0x04) {
		unsigned char level;

		pmodule->link_recv_dest_valid = 1;
		if(pmodule->module_flag & AMRMOD_FLAG_ROUTEADDR) {
			if(pbuf[4]&0x01) level = 0;
			else level = pbuf[4] >> 4;
		}
		else level = 0;
		
		addrlen = (int)level * 6;
		if((addrlen+12) > (len-MIN_PKT_LEN)) return 6;
		pbuf += 10;
		smallcpy(pmodule->link_recv_dest.src, pbuf, 6); pbuf += 6;
		if(level > PLC_ROUTENUM) {
			pmodule->link_recv_dest.route.level = PLC_ROUTENUM;
			smallcpy(pmodule->link_recv_dest.route.addr, pbuf, PLC_ROUTENUM*6);
		}
		else {
			pmodule->link_recv_dest.route.level = level;
			if(level) smallcpy(pmodule->link_recv_dest.route.addr, pbuf, addrlen);
		}

		pbuf += addrlen;
		smallcpy(pmodule->link_recv_dest.dest, pbuf, 6); pbuf += 6;
		len -= (addrlen+12);
		if(len < MIN_PKT_LEN) return 7;
		memmove(&prcv->afn, pbuf, len-MIN_PKT_LEN+3);
	}
	else {
		pmodule->link_recv_dest_valid = 0;
	}

	prcv->len = len - MIN_PKT_LEN;

	return 0;
}


/**
* @brief 接收报文(循环调用)
* @param pmodule 载波控制模块指针
* @return 返回0表示收到报文, 返回1表示未收到
*/
int Pl3762RecvPkt(struct amr_module_t *pmodule)
{
	unsigned char uc;

	pmodule->link_dl_recvtimes++;
	if(pmodule->link_dl_recvtimes > PL3762_RCVTIMEOUT) Pl3762StartRecv(pmodule);

	//while((*Plc3762CommFunc[pmodule->port-1].precv)(Plc3762CommFunc[pmodule->port-1].port, &uc, 1) > 0) {
	while(UartRecvData(PLC_PORT, &uc, 1) > 0) {
		pmodule->link_dl_recvtimes = 0;

		switch(pmodule->link_dl_recvstat) {
		case 0:
			if(0x68 == uc) {
				pmodule->link_dl_recvlen = 1;
				pmodule->link_recv_buf[0] = uc;
				pmodule->link_dl_recvstat = 1;
			}
			break;
		case 1:
			pmodule->link_recv_buf[pmodule->link_dl_recvlen++] = uc;
			if(pmodule->link_dl_recvlen >= 3) {
				unsigned short us;

				us = MAKE_SHORT(pmodule->link_recv_buf+1);
				if(us < MIN_PKT_LEN || us > (MAX_3762PKT_LEN+15)) {
					pmodule->link_dl_recvstat = 0;
					break;
				}
				pmodule->link_dl_recvmax = (int)us;
				pmodule->link_dl_recvstat = 2;
			}
			break;
		case 2:
			pmodule->link_recv_buf[pmodule->link_dl_recvlen++] = uc;
			if(pmodule->link_dl_recvlen >= pmodule->link_dl_recvmax) {
				int len = MAKE_SHORT(pmodule->link_recv_buf+1);
				int rtn;
				plc3762_pkt_t *pkt;

				PrintLog(LOGTYPE_DOWNLINK, "PLC(%d) recv %d:\n", pmodule->port, len);
				PrintHexLog(LOGTYPE_DOWNLINK, pmodule->link_recv_buf, len);
				pmodule->link_dl_recvstat = 0;
				rtn = Pl3762CheckRcvPacket(pmodule);
				if(rtn) {
					PrintLog(LOGTYPE_DOWNLINK, "PLC(%d) Check error(%d)\n", pmodule->port, rtn);
				}
				else {
					pkt = (plc3762_pkt_t *)pmodule->link_recv_buf;
					PrintLog(LOGTYPE_DOWNLINK, "PLC(%d) Check OK: AFN=%02X, dt=%02X%02X, len=%d\n",
								pmodule->port, pkt->afn, pkt->dt[0], pkt->dt[1], pkt->len);
					return 0;
				}
			}
			break;
		default:
			pmodule->link_dl_recvstat = 0;
			break;
		}
	}

	return 1;
}

/**
* @brief 接收特定数据帧
* @param pmodule 载波控制模块指针
* @param waitafn 指定回应的AFN
* @param waitdt 指定回应的DT
* @param timeout 超时时间(100ms)
* @return 成功返回0, 失败返回负数(参见错误码PLCERR_XXX)
*/
int Pl3762RecvPktTimeout(struct amr_module_t *pmodule, unsigned char waitafn, unsigned short waitdt, int timeout)
{
	int times;
	unsigned short dt;
	unsigned char afn;
	plc3762_pkt_t *prcv = (plc3762_pkt_t *)pmodule->link_recv_buf;

	for(times=0; times<timeout; times++) {
		Sleep(10);//100ms

		if(!Pl3762RecvPkt(pmodule)) {
			afn = prcv->afn;
			dt = (((unsigned short)(prcv->dt[1]))<<8) + (unsigned short)(prcv->dt[0]);
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

/**
* @brief 发送命令并等待确认
* @param pmodule 载波控制模块指针
* @return 成功返回0, 失败返回负数(参见错误码PLCERR_XXX)
*/
int Pl3762SetCommand(struct amr_module_t *pmodule, int special)
{
	plc3762_pkt_t *pkt = (plc3762_pkt_t *)pmodule->link_send_buf;
	plc3762_pkt_t *prcv = (plc3762_pkt_t *)pmodule->link_recv_buf;
	int i;
	int rtn=0;

	if(pmodule->module_flag&AMRMOD_FLAG_WLCTRL) pkt->ctrl = 0x4a;
	else pkt->ctrl = 0x41;
	for(i=0; i<MAX_PLCOMM_RETRY; i++) {
		if(Pl3762SendPkt(pmodule, NULL, special)) return PLCERR_INVALID;
		rtn = Pl3762RecvPktTimeout(pmodule, 0, 1, PLCOMM_TIMEOUT);
		if(!rtn) {
			if(pmodule->link_recv_dest_valid) return PLCERR_INVALID;
			else if(prcv->afn == 0) {
				if((prcv->dt[0] == 1) && (prcv->dt[1] == 0)) return 0;
				else return PLCERR_INVALID;
			}
			else return PLCERR_INVALID;
		}
		else if(rtn == PLCERR_INVALID)  return PLCERR_INVALID;
	}

	return PLCERR_TIMEOUT;
}

/**
* @brief 发送回应帧
* @param pmodule 载波控制模块指针
* @param errcode 回应错误码
*/
void Pl3762SendEchoPkt(struct amr_module_t *pmodule, unsigned char errcode)
{
	plc3762_pkt_t *psnd = (plc3762_pkt_t *)pmodule->link_send_buf;

	psnd->afn = PL3762_AFN_ECHO;
	if(errcode) {
		psnd->len = 1;
		FN_DT(2, psnd->dt);
		psnd->data[0] = errcode;
	}
	else {
		psnd->len = 4;
		FN_DT(1, psnd->dt);
		psnd->data[0] = 0xff;
		psnd->data[1] = 0xff;
		psnd->data[2] = psnd->data[3] = 0;
	}

	psnd->ctrl = 1;
	Pl3762SendPkt(pmodule, NULL, 0);
}

/**
* @brief 启动/停止/继续自动抄表命令
* @param pmodule 载波控制模块指针
* @param cmd 1-restart AMR,2-pause,3-resume
* @return 返回0表示成功, 否则失败
*/
int Pl3762AMRCtrl(struct amr_module_t *pmodule, enum plc_amrctrl_cmd cmd)
{
	static const char *flagname[] = {"restart", "pause", "resume"};
	plc3762_pkt_t *pkt = (plc3762_pkt_t *)pmodule->link_send_buf;

	PrintLog(LOGTYPE_SHORT, "AMR(%d) %s...\n", pmodule->port, flagname[(int)cmd-1]);

	pkt->afn = PL3762_AFN_CTRLROUTE;
	FN_DT((int)cmd, pkt->dt);
	pkt->len = 0;

	return Pl3762SetCommand(pmodule, 0);
}

int Pl3762SetRouterMode(struct amr_module_t *pmodule)
{
	plc3762_pkt_t *pkt = (plc3762_pkt_t *)pmodule->link_send_buf;

	pkt->afn = PL3762_AFN_SETROUTE;
	FN_DT(4, pkt->dt);
	pkt->data[0] = 0;
	pkt->data[1] = 0;
	pkt->data[2] = 0;
	pkt->len = 3;

	return Pl3762SetCommand(pmodule, 0);
}

/**
* @brief 读取模块地址
* @param pmodule 载波控制模块指针
* @return 返回0表示成功, 否则失败
*/
int Pl3762ReadModuleAddr(struct amr_module_t *pmodule)
{
	//static int balarmed = 0;

	plc3762_pkt_t *rpkt=(plc3762_pkt_t *)pmodule->link_recv_buf;
	plc3762_pkt_t *spkt=(plc3762_pkt_t *)pmodule->link_send_buf;
	int i;

	spkt->afn = PL3762_AFN_QRYDATA;
	FN_DT(4, spkt->dt);
	spkt->len = 0;
	if(pmodule->module_flag&AMRMOD_FLAG_WLCTRL) spkt->ctrl = 0x4a;
	else spkt->ctrl = 0x41;

	for(i=0; i<MAX_PLCOMM_RETRY; i++) {
		if(Pl3762SendPkt(pmodule, NULL, 0)) return PLCERR_INVALID;
		if(!Pl3762RecvPktTimeout(pmodule, PL3762_AFN_QRYDATA, 8, PLCOMM_TIMEOUT)) {
			if(pmodule->link_recv_dest_valid) return PLCERR_INVALID;
			else if(rpkt->afn == PL3762_AFN_QRYDATA) {
				if(rpkt->dt[0] == 0x08 && rpkt->dt[1] == 0) {
					smallcpy(pmodule->module_id, &rpkt->data[0], 6);
					PrintLog(LOGTYPE_ALARM, "PLC module id=%02X%02X-%02X%02X-%02X%02X\n",
						pmodule->module_id[5],pmodule->module_id[4],pmodule->module_id[3],
						pmodule->module_id[2],pmodule->module_id[1],pmodule->module_id[0]);
					return 0;
				}
			}
		}
	}

	/*if(balarmed || pmodule->port != (COMMPORT_PLC+1)) return PLCERR_TIMEOUT;
	balarmed = 1;

	{
		alarm_t alarm;

		memset(&alarm, 0, sizeof(alarm));
		alarm.erc = 21;
		alarm.len = 1;
		alarm.data[0] = 6;
		MakeAlarm(ALMFLAG_ABNOR, &alarm);
	}*/
	return PLCERR_TIMEOUT;
}

/**
* @brief 读取模块地址
* @param pmodule 载波控制模块指针
* @return 返回0表示成功, 否则失败
*/
int Pl3762SetModuleAddr(struct amr_module_t *pmodule, unsigned char *module_addr)
{
	plc3762_pkt_t *spkt=(plc3762_pkt_t *)pmodule->link_send_buf;

	spkt->afn = PL3762_AFN_CTRLCMD;
	FN_DT(1, spkt->dt);
	smallcpy(spkt->data, module_addr, 6);
	spkt->len = 6;

	return Pl3762SetCommand(pmodule, 0);
}

/**
* @brief 读取模块厂商代码和版本号
* @param pmodule 载波控制模块指针
* @return 成功返回0, 否则失败
*/
int Pl3762ReadModuleManufactor(struct amr_module_t *pmodule)
{
	plc3762_pkt_t *rpkt=(plc3762_pkt_t *)pmodule->link_recv_buf;
	plc3762_pkt_t *spkt=(plc3762_pkt_t *)pmodule->link_send_buf;
	int i, times;
	
	spkt->afn = PL3762_AFN_QRYDATA;
	FN_DT(1, spkt->dt);
	spkt->len = 0;
	if(pmodule->module_flag&AMRMOD_FLAG_WLCTRL) spkt->ctrl = 0x4a;
	else spkt->ctrl = 0x41;

	for(i=0; i<3; i++) {
		if(Pl3762SendPkt(pmodule, NULL, 0)) 
			return PLCERR_INVALID;

		for(times=0; times<PLCOMM_TIMEOUT; times++) 
		{
			Sleep(2);//100ms
			if(!Pl3762RecvPkt(pmodule)) 
			{
				if(pmodule->link_recv_dest_valid) 
					return PLCERR_INVALID;
				else if(rpkt->afn == PL3762_AFN_QRYDATA) 
				{
					if(rpkt->dt[0] == 0x01 && rpkt->dt[1] == 0) 
					{
						smallcpy(pmodule->module_ver, rpkt->data, 9);
						/*pmodule->module_ver[0] = (char)rpkt->data[0];
						pmodule->module_ver[1] = (char)rpkt->data[1];
						pmodule->module_ver[2] = (char)rpkt->data[2];
						pmodule->module_ver[3] = (char)rpkt->data[3];
						pmodule->module_ver[4] = '-';
						sprintf(pmodule->module_ver+5, "%02X%02X%02X-", rpkt->data[6], rpkt->data[5], rpkt->data[4]);
						sprintf(pmodule->module_ver+12, "%02X%02X", rpkt->data[7], rpkt->data[8]);*/
						return 0;
					}
				}
			}
		}
	}

	return PLCERR_TIMEOUT;
}

/**
* @brief 将表地址添加到模块
* @param pmodule 载波控制模块指针
* @param num 要添加的表数
* @param addr-表地址+表序号+协议类型,num*9字节bcd码
*/
int Pl3762AddMeter(struct amr_module_t *pmodule, int num, const unsigned char *addr)
{
	plc3762_pkt_t *pkt=(plc3762_pkt_t *)pmodule->link_send_buf;

	if(num <= 0 || num > PLMET_OP_NUM) return PLCERR_INVALID;

	pkt->afn = PL3762_AFN_SETROUTE;
	FN_DT(1, pkt->dt);
	pkt->data[0] = (unsigned char)num;
	smallcpy(&pkt->data[1], addr, num*9);
	pkt->len = num*9 + 1;
	return Pl3762SetCommand(pmodule, 0);
}

/**
* @brief 将表地址从模块删除
* @param pmodule 载波控制模块指针
* @param num 要删除的表数
* @param addr-表地址,num*6字节bcd码
*/
int Pl3762DelMeter(struct amr_module_t *pmodule, int num, const unsigned char *addr)
{
	plc3762_pkt_t *pkt=(plc3762_pkt_t *)pmodule->link_send_buf;

	if(num <= 0 || num > PLMET_OP_NUM) return PLCERR_INVALID;

	pkt->afn = PL3762_AFN_SETROUTE;
	FN_DT(2, pkt->dt);
	pkt->data[0] = (unsigned char)num;
	smallcpy(&pkt->data[1], addr, num*6);
	pkt->len = num*6 + 1;
	return Pl3762SetCommand(pmodule, 0);
}

/**
* @brief 将表地址从模块删除(需先暂停 后恢复)
* @param pmodule 载波控制模块指针
* @param num 要删除的表数
* @param addr-表地址,num*6字节bcd码
*/
void Pl3762DelMeterPause(struct amr_module_t *pmodule, int num, const unsigned char *addr)
{
	Pl3762AMRCtrl(pmodule, PLC_AMRCTRL_PAUSE);
	Pl3762DelMeter(pmodule, num, addr);
	Pl3762AMRCtrl(pmodule, PLC_AMRCTRL_RESUME);
}

/**
* @brief 删除模块所有表
* @param pmodule 载波控制模块指针
*/
#if 0
void Pl3762DelAllMeter(struct amr_module_t *pmodule)
{
	unsigned char list[6*PLMET_OP_NUM];
	unsigned char *puc;
	int metid, delmets;
	unsigned flagmask;

	if(pmodule->module_flag & AMRMOD_FLAG_USEOWNER) flagmask = AMRMETMAP_ATTR_DIRECT; //不包含采集器下485电表
	else flagmask = AMRMETMAP_ATTR_DIRECT|AMRMETMAP_ATTR_OWNER;

	delmets = 0;
	puc = list;
	for(metid=0; metid<MAX_METER; metid++) {
		if(0 == (pmodule->module_flag & flagmask)) continue;

		if((pmodule->module_flag & AMRMOD_FLAG_USEOWNER) && (pmodule->met_map[metid].attr & AMRMETMAP_ATTR_OWNER)) { //采集器
			puc[0] = pmodule->met_map[metid].owner_addr[0];
			puc[1] = pmodule->met_map[metid].owner_addr[1];
			puc[2] = pmodule->met_map[metid].owner_addr[2];
			puc[3] = pmodule->met_map[metid].owner_addr[3];
			puc[4] = pmodule->met_map[metid].owner_addr[4];
			puc[5] = pmodule->met_map[metid].owner_addr[5];
		}
		else {
			puc[0] = pmodule->met_map[metid].addr[0];
			puc[1] = pmodule->met_map[metid].addr[1];
			puc[2] = pmodule->met_map[metid].addr[2];
			puc[3] = pmodule->met_map[metid].addr[3];
			puc[4] = pmodule->met_map[metid].addr[4];
			puc[5] = pmodule->met_map[metid].addr[5];
		}
		delmets++;
		if(delmets >= PLMET_OP_NUM) {
			Pl3762DelMeter(pmodule, delmets, list);
			delmets = 0;
			puc = list;
			
		}		
	}

	if(delmets) {
		Pl3762DelMeter(pmodule, delmets, list);
	}

	SysClearCurrentTaskState();
}
#endif


/**
* @brief 读取模块配置表数目
* @param pmodule 载波控制模块指针
* @return 成功返回表计数目, 失败返回-1
*/
int Pl3762ReadMeterNum(struct amr_module_t *pmodule)
{
	plc3762_pkt_t *rpkt=(plc3762_pkt_t *)pmodule->link_recv_buf;
	plc3762_pkt_t *spkt=(plc3762_pkt_t *)pmodule->link_send_buf;
	int i;
	
	spkt->afn = PL3762_AFN_QRYROUTE;
	FN_DT(1, spkt->dt);
	spkt->len = 0;
	if(pmodule->module_flag&AMRMOD_FLAG_WLCTRL) spkt->ctrl = 0x4a;
	else spkt->ctrl = 0x41;

	for(i=0; i<3; i++) {
		if(Pl3762SendPkt(pmodule, NULL, 0)) return PLCERR_INVALID;
		if(!Pl3762RecvPktTimeout(pmodule, PL3762_AFN_QRYROUTE, 1, PLCOMM_TIMEOUT)) {
			if(pmodule->link_recv_dest_valid) return PLCERR_INVALID;
			else if(rpkt->afn == PL3762_AFN_QRYROUTE) {
				if(rpkt->dt[0] == 0x01 && rpkt->dt[1] == 0) {
					return (int)MAKE_SHORT(rpkt->data);
				}
			}
		}
	}

	return PLCERR_INVALID;
}

/**
* @brief 查询模块表计信息
* @param pmodule 载波控制模块指针
* @param metid 查询开始表序号
* @param pinfo 返回的载波从节点信息
* @param maxnum pinfo最大数目
* @return 成功返回查询到的表计数目
*/
int Pl3762QueryMeter(struct amr_module_t *pmodule, unsigned short metid, struct pl3762_slavenode_info *pinfo, int maxnum)
{
	plc3762_pkt_t *rpkt=(plc3762_pkt_t *)pmodule->link_recv_buf;
	plc3762_pkt_t *spkt=(plc3762_pkt_t *)pmodule->link_send_buf;
	unsigned char readnum = 0, *paddr;
	int i, retry;

	if(maxnum <= 0) return PLCERR_INVALID;
	else if(maxnum > PLMET_OP_NUM) maxnum = PLMET_OP_NUM;

	spkt->afn = PL3762_AFN_QRYROUTE;
	FN_DT(2, spkt->dt);
	DEPART_SHORT(metid + 1, &spkt->data[0]);
	spkt->data[2] = maxnum;
	spkt->len = 3;
	if(pmodule->module_flag&AMRMOD_FLAG_WLCTRL) spkt->ctrl = 0x4a;
	else spkt->ctrl = 0x41;

	for(retry=0; retry<3; retry++) {
		if(Pl3762SendPkt(pmodule, NULL, 0)) return PLCERR_INVALID;
		if(!Pl3762RecvPktTimeout(pmodule, PL3762_AFN_QRYROUTE, 2, PLCOMM_TIMEOUT)) {
			if(pmodule->link_recv_dest_valid) return PLCERR_INVALID;
			else if(rpkt->afn == PL3762_AFN_QRYROUTE) {
				if(rpkt->dt[0] == 0x02 && rpkt->dt[1] == 0) {
					readnum  = rpkt->data[2];
					if(readnum > maxnum) return PLCERR_INVALID;

					paddr = rpkt->data + 3;
					for(i=0; i<readnum; i++,paddr+=8,pinfo++) {
						memcpy(pinfo, paddr, 8);
					}

					return readnum;
				}
			}
		}
	}

	return PLCERR_TIMEOUT;
}
#if 0
static int AddToDelList(struct amr_module_t *pmodule, const unsigned char *addr)
{

	unsigned short i;
	unsigned char *paddr;

	for(i=0,paddr=pmodule->module_sync.del_list; i<pmodule->module_sync.del_num; i++,paddr+=6) {
		if(paddr[0] == addr[0] && paddr[1] == addr[1] && paddr[2] == addr[2] &&
			paddr[3] == addr[3] && paddr[4] == addr[4] && paddr[5] == addr[5]) {
			break;
		}
	}
	if(i < pmodule->module_sync.del_num) return 1;

	for(i=0; i<6; i++) paddr[i] = addr[i];
	pmodule->module_sync.del_num++;

	return 0;
}
#endif
/**
* @brief 改变模块的表计配置
* @param pmodule 载波控制模块指针
* @return 0-成功, 否则失败
*/
int Pl3762ChangeMeters(struct amr_module_t *pmodule)
{
#if 0
	int num;
	unsigned short metid, i;
	unsigned char opbuf[10*PLMET_OP_NUM];
	unsigned char *paddr, *paddr2;

	//删除需要删除的电表
	paddr = opbuf;
	paddr2 = pmodule->module_sync.del_list;
	for(i=0,num=0; i<pmodule->module_sync.del_num; i++,paddr2+=6) {
		smallcpy(paddr, paddr2, 6);
		paddr += 6;
		num++;
		if(num >= PLMET_OP_NUM) {
			Pl3762DelMeter(pmodule, num, opbuf);
			num = 0;
			paddr = opbuf;
		}
	}
	if(num) Pl3762DelMeter(pmodule, num, opbuf);

	//添加电表
	for(i=0,paddr=opbuf,num=0; i<pmodule->module_sync.add_num; i++) {
		metid = pmodule->module_sync.add_list[i];
		if((pmodule->module_flag & AMRMOD_FLAG_USEOWNER) && 
			(pmodule->met_map[metid].attr == AMRMETMAP_TYPE_OWNER)) {
			smallcpy(paddr, pmodule->met_map[metid].owner_addr, 6);
		}
		else {
			smallcpy(paddr, pmodule->met_map[metid].addr, 6);
		}
		paddr += 6;
		DEPART_SHORT(metid + 1, paddr);
		paddr += 2;
		*paddr++ = pmodule->met_map[metid].proto;
		num++;
		if(num >= PLMET_OP_NUM) {
			Pl3762AddMeter(pmodule, num, opbuf);
			num = 0;
			paddr = opbuf;
		}
	}
	if(num) Pl3762AddMeter(pmodule, num, opbuf);

	pmodule->module_sync.add_num = 0;
	pmodule->module_sync.del_num = 0;
#endif
	return 0;
}

/**
* @brief 同步模块与集中器的表计配置
* @param pmodule 载波控制模块指针
* @return 0-成功, 否则失败
*/
int Pl3762SyncMeters(struct amr_module_t *pmodule)
{
#if 0

#endif
	return 0;
}

/**
* @brief 模块初始化
* @param pmodule 载波控制模块指针
* @param fn  数据单元标识(F1硬件初始化 F2参数区初始化 F3数据区初始化)
* @return 返回0表示成功，非0表示设置失败
*/
int Pl3762ResetModule(struct amr_module_t *pmodule, unsigned char fn)
{
	plc3762_pkt_t *pkt = (plc3762_pkt_t *)pmodule->link_send_buf;

	if(fn < 1 || fn > 3)	return PLCERR_INVALID;
	pkt->afn = PL3762_AFN_INIT;
	FN_DT(fn, pkt->dt);
	pkt->len = 0;

	return Pl3762SetCommand(pmodule, 0);
}

/**
* @brief 查询路由状态信息 
*/
int Pl3762QueryRouter(struct amr_module_t *pmodule, unsigned char *routerinfo)
{
	plc3762_pkt_t *rpkt=(plc3762_pkt_t *)pmodule->link_recv_buf;
	plc3762_pkt_t *spkt=(plc3762_pkt_t *)pmodule->link_send_buf;
	int i, times;
	
	spkt->afn = PL3762_AFN_QRYROUTE;
	FN_DT(4, spkt->dt);
	spkt->len = 0;
	if(pmodule->module_flag&AMRMOD_FLAG_WLCTRL) spkt->ctrl = 0x4a;
	else spkt->ctrl = 0x41;

	for(i=0; i<3; i++) {
		if(Pl3762SendPkt(pmodule, NULL, 0)) return PLCERR_INVALID;

		for(times=0; times<PLCOMM_TIMEOUT; times++) {
			Sleep(10);//100ms
			if(!Pl3762RecvPkt(pmodule)) {
				if(pmodule->link_recv_dest_valid) return PLCERR_INVALID;
				else if(rpkt->afn == PL3762_AFN_QRYROUTE) {
					if(rpkt->dt[0] == 0x08 && rpkt->dt[1] == 0) {
						PrintLog(LOGTYPE_DOWNLINK, "router info:");
						PrintHexLog(LOGTYPE_DOWNLINK, rpkt->data, rpkt->len);
						smallcpy(routerinfo, rpkt->data, rpkt->len);
						return 0;
					}
				}
			}
		}
	}

	return PLCERR_TIMEOUT;
}

#define SPEC_PLMET_OP_NUM			1

/**
* @brief 读取模块配置表数目
* @param pmodule 载波控制模块指针
* @return 成功返回表计数目, 失败返回-1
*/
int SpecPl3762ReadMeterNum(struct amr_module_t *pmodule)
{
	plc3762_pkt_t *rpkt=(plc3762_pkt_t *)pmodule->link_recv_buf;
	plc3762_pkt_t *spkt=(plc3762_pkt_t *)pmodule->link_send_buf;
	int retry;

	spkt->afn = PL3762_AFN_QRYROUTE;
	FN_DT(2, spkt->dt);
	spkt->data[2] = SPEC_PLMET_OP_NUM;
	spkt->len = 3;
	if(pmodule->module_flag&AMRMOD_FLAG_WLCTRL) spkt->ctrl = 0x4a;
	else spkt->ctrl = 0x41;
	
	for(retry=0; retry<10; retry++) {
		DEPART_SHORT(retry+1, &spkt->data[0]);
		if(Pl3762SendPkt(pmodule, NULL, 0)) return PLCERR_INVALID;
		if(!Pl3762RecvPktTimeout(pmodule, PL3762_AFN_QRYROUTE, 2, PLCOMM_TIMEOUT)) {
			if(pmodule->link_recv_dest_valid) return PLCERR_INVALID;
			else if(rpkt->afn == PL3762_AFN_QRYROUTE) {
				if(rpkt->dt[0] == 0x02 && rpkt->dt[1] == 0) {
					return MAKE_SHORT(rpkt->data);
				}
			}
		}
	}

	return PLCERR_TIMEOUT;
}

/**
* @brief 改变模块的表计配置(不支持AFN=10 F1查询从节点数量)
* @param pmodule 载波控制模块指针
* @return 0-成功, 否则失败
*/
int SpecPl3762ChangeMeters(struct amr_module_t *pmodule)
{
#if 0
	int num;
	unsigned short metid, i;
	unsigned char opbuf[10*SPEC_PLMET_OP_NUM];
	unsigned char *paddr, *paddr2;

	//删除需要删除的电表
	paddr = opbuf;
	paddr2 = pmodule->module_sync.del_list;
	for(i=0,num=0; i<pmodule->module_sync.del_num; i++,paddr2+=6) {
		smallcpy(paddr, paddr2, 6);
		paddr += 6;
		num++;
		if(num >= SPEC_PLMET_OP_NUM) {
			Pl3762DelMeter(pmodule, num, opbuf);
			num = 0;
			paddr = opbuf;
		}
	}
	if(num) Pl3762DelMeter(pmodule, num, opbuf);

	//添加电表
	for(i=0,paddr=opbuf,num=0; i<pmodule->module_sync.add_num; i++) {
		metid = pmodule->module_sync.add_list[i];
		if((pmodule->module_flag & AMRMOD_FLAG_USEOWNER) && 
			(pmodule->met_map[metid].attr == AMRMETMAP_TYPE_OWNER)) {
			smallcpy(paddr, pmodule->met_map[metid].owner_addr, 6);
		}
		else {
			smallcpy(paddr, pmodule->met_map[metid].addr, 6);
		}
		paddr += 6;
		DEPART_SHORT(metid + 1, paddr);
		paddr += 2;
		*paddr++ = pmodule->met_map[metid].proto;
		num++;
		if(num >= SPEC_PLMET_OP_NUM) {
			Pl3762AddMeter(pmodule, num, opbuf);
			num = 0;
			paddr = opbuf;
		}
	}
	if(num) Pl3762AddMeter(pmodule, num, opbuf);

	pmodule->module_sync.add_num = 0;
	pmodule->module_sync.del_num = 0;
#endif
	return 0;
}

/**
* @brief 同步模块与集中器的表计配置(不支持AFN=10 F1查询从节点数量)
* @param pmodule 载波控制模块指针
* @return 0-成功, 否则失败
*/
int SpecPl3762SyncMeters(struct amr_module_t *pmodule)
{

	return 0;
}

/**
* @brief 直接接收报文(不解析报文)
* @param pmodule 载波控制模块指针
* @return 返回0表示收到报文, 返回1表示未收到
*/
static int Pl3762DirectRecvPkt(struct amr_module_t *pmodule)
{
	unsigned char uc;

	AssertLogReturn(pmodule->port == 0 || pmodule->port > MAX_PORT, 1, "invalid port(%d)", pmodule->port);

	pmodule->link_dl_recvtimes++;
	if(pmodule->link_dl_recvtimes > PL3762_RCVTIMEOUT) Pl3762StartRecv(pmodule);

	//while((*Plc3762CommFunc[pmodule->port-1].precv)(Plc3762CommFunc[pmodule->port-1].port, &uc, 1) > 0) {
	while(UartRecvData(PLC_PORT, &uc, 1) > 0) {
		pmodule->link_dl_recvtimes = 0;

		switch(pmodule->link_dl_recvstat) {
		case 0:
			if(0x68 == uc) {
				pmodule->link_dl_recvlen = 1;
				pmodule->link_recv_buf[0] = uc;
				pmodule->link_dl_recvstat = 1;
			}
			break;
		case 1:
			pmodule->link_recv_buf[pmodule->link_dl_recvlen++] = uc;
			if(pmodule->link_dl_recvlen >= 3) {
				unsigned short us;

				us = MAKE_SHORT(pmodule->link_recv_buf+1);
				if(us < MIN_PKT_LEN || us > (MAX_3762PKT_LEN+15)) {
					pmodule->link_dl_recvstat = 0;
					break;
				}
				pmodule->link_dl_recvmax = (int)us;
				pmodule->link_dl_recvstat = 2;
			}
			break;
		case 2:
			pmodule->link_recv_buf[pmodule->link_dl_recvlen++] = uc;
			if(pmodule->link_dl_recvlen >= pmodule->link_dl_recvmax) {
				pmodule->link_dl_recvlen = MAKE_SHORT(pmodule->link_recv_buf+1);
				PrintLog(LOGTYPE_DOWNLINK, "PLC(%d) recv %d:\n", pmodule->port, pmodule->link_dl_recvlen);
				PrintHexLog(LOGTYPE_DOWNLINK, pmodule->link_recv_buf, pmodule->link_dl_recvlen);
				pmodule->link_dl_recvstat = 0;
				return 0;
			}
			break;
		default:
			pmodule->link_dl_recvstat = 0;
			break;
		}
	}

	return 1;
}

void Pl3762DirectForwardProc(struct amr_module_t *pmodule)
{
	unsigned char *pdata;
	int i;
	int applen,times;
	
	pdata = pmodule->event_cmd_buf;	
	applen=pmodule->event_sendlen;
	
	for(i=0; i<applen; i++) {
		if(0x68 == pdata[i]) break;
	}
	if((i+8) >= applen) goto mark_fail;
	
	pdata+=i;
	applen-=i;
	if((i+8) >= applen) goto mark_fail;
	
	memcpy(pmodule->link_dl_buf, pdata, applen);
	PrintLog(LOGTYPE_DOWNLINK, "PLGDW Direct send(%d):\n", applen);
	PrintHexLog(LOGTYPE_DOWNLINK, (unsigned char *)pdata, applen);
	
	UartSendData(PLC_PORT, pmodule->link_dl_buf, applen);
	for(times=0; times<300; times++){
		Sleep(10);//100ms
     	if(!Pl3762DirectRecvPkt(pmodule)) break;
	}
	if(times >= 300) goto mark_fail;	//超时
	
	smallcpy(pmodule->event_echo_buf, pmodule->link_recv_buf, pmodule->link_dl_recvlen);
	pmodule->event_echortn = pmodule->link_dl_recvlen;
	pmodule->event_echoed = 1;
	return;
	
mark_fail:
	pmodule->event_echortn = 0;
	pmodule->event_echoed = 1;
	return;
}

/**
* @brief 获取抄读相关信息
* @param *pmodule 载波控制模块指针
* @param *readinginfo 抄读相关信息
* @return void
*/
void Pl3762GetReadingInfo(struct amr_module_t *pmodule, rep_reading_info *preadinginfo)
{
	plc3762_pkt_t *prcv = (plc3762_pkt_t *)pmodule->link_recv_buf;
	
	memset(preadinginfo, 0x00, sizeof(rep_reading_info));
	//抄表信息域
	preadinginfo->routes = prcv->info[0]>>4;
	preadinginfo->channel = prcv->info[1]&0x0f;
	if(prcv->info[2]&0x03) preadinginfo->phase = 0x01<<((prcv->info[2]&0x03)-1);
	else preadinginfo->phase = 0;
	preadinginfo->quality = prcv->info[3]&0x0f;	
}
#endif
