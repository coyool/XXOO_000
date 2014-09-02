/**
* plccomm.c -- 载波通信接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-21
* 最后修改时间: 2009-5-21
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#include "app_include/version.h"
#include "app_include/lib/align.h"
#include "app_include/param/unique.h"
#include "app_include/param/meter.h"
#include "app_include/cenmet/forward.h"
#include "plat_include/plcomm.h"
#include "app_include/lib/bcd.h"
#include "app_include/monitor/runstate.h"
#include "plat_include/dl645_1997.h"	
#include "plat_include/dl645_2007.h"
#include "plat_include/debug.h"	
#include "plat_include/plat_uart.h"
#include "plat_include/app_proto.h"
#include "plat_include/mutex.h"
#include "plat_include/amrplat.h"
#include "plat_include/timer.h"
#include "app_include/param/commport.h"
#include "plat_include/sys_config.h"
#include "app_include/uplink/hdlc.h"
#include "plat_include/shellcmd.h"

#ifdef DEFINE_PLCOMM

#define MAX_READITEM_NUM		510

typedef struct{
	unsigned short PlCurMetid;		//当前抄读序号
	unsigned short PlCurItemIdx; 	//当前抄读数据项序号
	unsigned short PlItemNum;		//当前测量点总的数据项数量
	unsigned short PlItemId[MAX_READITEM_NUM];	// 抄读的数据标识数组
	unsigned short PlReadItem;  	//读取的数据项标志
	unsigned long PlReadRealId;  	//实际读取的数据项标识
} PortCurrReadStat;

//static PortCurrReadStat CurrReadStat[MAX_COMMPORT];
#define MAX_PL_REQCOUNT			2
#define MAX_GDWPKT_LEN		1000


#if 0

//return 1 stop , 0 - not stop
static int CheckMeterRead(int port)
{
	unsigned short metid, mid;
	int stop = 1;
	int paused = 0;
	int metadded = 0;
	int chgflag = GetPlMeterChgFlag();

	for(metid=0; metid<MAX_METER; metid++) {
		if(EMPTY_METER(metid)) continue;
		if(!PLC_METER(metid)) continue;

		mid = ParaMeter[metid].metp_id - 1;
		if(mid >= MAX_METP) continue;
		if(ParaMeter[metid].proto != METTYPE_DL645 && ParaMeter[metid].proto != METTYPE_DL645_2007) continue;

		if(0 == PlcRamState[metid].requested && chgflag) {
			if(0 == paused) {
				//PlEsStdAMRCtrl(PLESCMD_PAUSE);
				paused = 1;
			}
			PrintLog(LOGTYPE_SHORT, "Add Meter %d..\n", metid+1);
			//PlEsStdAddMeter(metid);
			stop = 0;
			metadded = 1;
		}
		else if(stop && PlcAllReadCheck(mid, CurrReadStat[port].PlItemId)) {
			stop = 0;
		}
	}

	//if(paused) PlEsStdAMRCtrl(PLESCMD_RESUME);
	if(chgflag) SetPlMeterChgFlag(0);

	for(metid=0; metid<MAX_METER; metid++) {
		if(PlcRamState[metid].requested >= MAX_PL_REQCOUNT)
			PlcRamState[metid].requested = 1;
	}

	return stop;
}
#endif

extern int GetPlModuleNumber(void);
/**
* @brief 获取用户表抄读内容
* @param port 端口号
* @param metid 请求序号
* @param reqnodeaddr 请求的从节点地址
* @param *appbuf 传递buf
* @return 返回0表示成功，非0表示获取失败
*/
#if 0
int GetUserMeterReadContent(int port, unsigned short metid, unsigned char *reqnodeaddr, unsigned char *frame, unsigned char *appbuf, int *alen)
{

	int curport = 0;
	unsigned short mid = 0, itemid;
	usrmet_prot_t* pfunc=NULL;
	unsigned char proto = 0,readmask = 1;
	int i, flagchk = 0;
	unsigned char tmpframe;
	//sysclock_t curtime;  //当前时间
	//int haveowner = 0; //东软模式需判断是否有采集器地址
	//unsigned short tmp_mpid;

	if(port<=0 || port>MAX_PORT) return ERR_FAILPORT;
	curport = port-1;

 	if(metid == 0 || metid > MAX_METER) {
		return REQREAD_NOTEXIST;
	}
	metid -= 1;

	for(i=0; i<6; i++) {
		if(ParaMeter[metid].addr[i] != reqnodeaddr[i]) {
			return REQREAD_NOTEXIST;
		}
	}

	if(ParaMeter[metid].port != port) return REQREAD_NOTEXIST;
	if(EMPTY_METER(metid) || !USER_METER(metid)) return REQREAD_NOTEXIST;

	mid = ParaMeter[metid].metp_id;
	if(0 == mid || mid > MAX_METP) return REQREAD_NOTEXIST;
	
	mid -= 1;

mark_continue:
	proto = (ParaMeter[metid].proto == METTYPE_DL645_2007)? 2 : 1;
	pfunc = (usrmet_prot_t*)GetUserMetProto(proto);
	if(NULL == pfunc) 
		return REQREAD_ERROR;
	else 
	{

		if(CurrReadStat[curport].PlCurMetid != metid + 1)
		{ // 新的电表序号
			CurrReadStat[curport].PlCurMetid = metid + 1;
			CurrReadStat[curport].PlItemNum = PlcAllReadCheck(mid, CurrReadStat[curport].PlItemId);
			CurrReadStat[curport].PlCurItemIdx = 0;

			if(CurrReadStat[curport].PlItemNum > 0)
			{
				readmask = 2;
				if(!PlcRamState[metid].startflag)
				{
					PlcRamState[metid].requested = 1;
					PlcRamState[metid].startflag = 1;
					PlcRamState[metid].frezokflag = 0;
				}
				if(0 == PlcAllState.start_time) 
					PlcAllState.start_time = UTimeReadCurrent();
			}
			else
			{
				CurrReadStat[curport].PlCurMetid = 0;
				return REQREAD_DONE;
			}
		}
		else
		{
			CurrReadStat[curport].PlCurItemIdx++;
		}
	
		if(CurrReadStat[curport].PlCurItemIdx >= CurrReadStat[curport].PlItemNum){ // 数据id读完成
			if(PlcRamState[metid].requested > 1) SavePlMdb(1);
			
			if(PlcMetIsNeedReadCheck(metid)>0) return REQREAD_SKIP;
			/*if(PLMODULE_TOPSCOMM == GetPlModuleNo() || PLMODULE_LME == GetPlModuleNo()){ // 检查该测量点是否抄读完成
				if(PlcState[metid].failcount > 100){ //防止一直抄读同一块载波表
					PlcState[metid].failcount = 0;
					readmask = 1;
				}
				else{
					pltcreaditems.num = PlcAllReadCheck(mid, pltcreaditems.itemid);
					pltcreaditems.metid = metid + 1;
					
					if(pltcreaditems.num > 0){
						readmask = 2;
						itemid = pltcreaditems.itemid[0];
						PrintLog(LOGTYPE_DOWNLINK, "read met%d %04X..\n", pltcreaditems.metid, itemid);
					}else readmask = 1;
				}
			}else readmask = 1;*/
			readmask = 1;
			
			CurrReadStat[curport].PlCurMetid = 0;
			if(0 == PlcRamState[metid].requested) PlcRamState[metid].requested = MAX_PL_REQCOUNT;
			else PlcRamState[metid].requested++;
			if(PlcRamState[metid].requested > MAX_PL_REQCOUNT) flagchk = 1;
		}
		else 
		{
			readmask = 2;
			
			itemid = CurrReadStat[curport].PlItemId[CurrReadStat[curport].PlCurItemIdx];
			PrintLog(LOGTYPE_DOWNLINK, "Get[%d] read met%d %04X..\n", port, CurrReadStat[curport].PlCurMetid, itemid);
			if(GetProjectNumber() == VERPROJ_JIANGSU) FillPlMdbFailData(mid, itemid);
		}
	
	}

	if(0== readmask || 1 == readmask) {//抄读成功或无此表规约处理模块
		return REQREAD_DONE;
	}
	else{
		i = pfunc->makeread(ParaMeter[metid].addr, itemid, appbuf, MAX_GDWPKT_LEN);

		if(i<0){
			PlcState[metid].failcount++;
			goto mark_continue;
		}
		*alen = i;
		tmpframe = (ParaMeter[metid].speed<<5)&0xe0;
		tmpframe |= (ParaCommPort[port-1].frame & 0x1f);
		*frame = tmpframe;
		CurrReadStat[curport].PlReadItem = itemid;
		if(1 == proto) CurrReadStat[curport].PlReadRealId = (unsigned long)itemid;
		else {
			varul_t varl;
			varl.uc[0] = appbuf[10] - 0x33;
			varl.uc[1] = appbuf[11] - 0x33;
			varl.uc[2] = appbuf[12] - 0x33;
			varl.uc[3] = appbuf[13] - 0x33;
			CurrReadStat[curport].PlReadRealId = varl.ul;
		}
		return REQREAD_SUCCESS;
	}

	if(flagchk) {
		if(CheckMeterRead(curport)) {
			/*PlEsStdAMRCtrl(PLESCMD_PAUSE);	//stop  无需暂停，让其自动学习*/
			if(0 == PlcAllState.end_time) PlcAllState.end_time = UTimeReadCurrent();
			SavePlMdb(1);
			return REQREAD_SUCCESS;
		}
		else PlcRamState[metid].requested -= 2;
	}

	return REQREAD_SUCCESS;
}
#endif
/**
* @brief 载波表主动上报表地址
* @param *pdata 标准的DLT645-2007帧
* @return 返回0表示成功，非0表示获取失败
*/
#if 0
static int PlMetReportAddrProc(unsigned char *pdata)
{

	unsigned char datalen = pdata[9];
	unsigned char *pmet = pdata+10;
	unsigned char reportnum;
	para_updmeter_t updmeter;
	int j;

	for(j=0; j<datalen; j++) pmet[j] = pmet[j] - 0x33;
	reportnum = pmet[4];

	PrintLog(LOGTYPE_DOWNLINK, "PlMetReportAddr Nums = %d...\n", reportnum);
	if(0 == reportnum || reportnum>16) return ERR_COMPARE;

	if(1 == reportnum){
		memset(&updmeter, 0, sizeof(updmeter));
		updmeter.valid = 1;	
		if(HexIsEmpty(&pdata[1], 6, 0x00)){	//为全载电表地址
			memset(updmeter.owneraddr, 0x00, 6);
			smallcpy(updmeter.addr, &pmet[5], 6);
			updmeter.proto = pmet[11];
		}
		else if(!HexComp(&pdata[1],  &pmet[5], 6)){	//相等 电表地址为全EE
			memset(updmeter.addr, 0xee, 6);
			updmeter.proto = 0;
			smallcpy(updmeter.owneraddr, &pdata[1], 6);
		}
		else{
			smallcpy(updmeter.addr, &pmet[5], 6);
			updmeter.proto = pmet[11];
			smallcpy(updmeter.owneraddr, &pdata[1], 6);
		}
		PrintLog(LOGTYPE_DOWNLINK, "PLC Protol=%d, addr:%02X%02X%02X%02X%02X%02X, ", updmeter.proto, 
			updmeter.addr[5],updmeter.addr[4],updmeter.addr[3],updmeter.addr[2],updmeter.addr[1],updmeter.addr[0]);
		PrintLog(LOGTYPE_DOWNLINK, "owneraddr:%02X%02X%02X%02X%02X%02X\n", updmeter.owneraddr[5],
			updmeter.owneraddr[4],updmeter.owneraddr[3],updmeter.owneraddr[2],updmeter.owneraddr[1],updmeter.owneraddr[0]);
		AddToParaUpdMeter(PlUpdMeter, COMMPORT_PLC, updmeter);
	}
	else{
		for(j=0; j<reportnum; j++){
			memset(&updmeter, 0, sizeof(updmeter));
			updmeter.valid = 1;	
			smallcpy(updmeter.addr, &pmet[5+7*j], 6);
			updmeter.proto = pmet[11+7*j];
			smallcpy(updmeter.owneraddr, &pdata[1], 6);
			PrintLog(LOGTYPE_DOWNLINK, "PLC Protol=%d, addr:%02X%02X%02X%02X%02X%02X, ", updmeter.proto, 
				updmeter.addr[5],updmeter.addr[4],updmeter.addr[3],updmeter.addr[2],updmeter.addr[1],updmeter.addr[0]);
			PrintLog(LOGTYPE_DOWNLINK, "owneraddr:%02X%02X%02X%02X%02X%02X\n", updmeter.owneraddr[5],
				updmeter.owneraddr[4],updmeter.owneraddr[3],updmeter.owneraddr[2],updmeter.owneraddr[1],updmeter.owneraddr[0]);
			AddToParaUpdMeter(PlUpdMeter, COMMPORT_PLC, updmeter);
		}
	}

	return ECHO_OK;




}
#endif
/**
* @brief 处理上报数据
* @param port 端口号
* @param metid 上报的电表序号
* @param reqnodeaddr 上报的表协议
* @param *appbuf 上报数据buf
* @param alen 上报数据的长度
* @param readinginfo 抄读信息
* @return 返回0表示成功，非0表示处理失败
*/
#if 0
int UserMeterReportDataProc(int port, unsigned short metid, unsigned char proto, unsigned char *appbuf, int alen, rep_reading_info readinginfo)
{

	unsigned char tmpbuf[AMRMOD_APP_BUFFER_SIZE];
	unsigned char *pdata;
	unsigned short mid;
	int applen, i;
	unsigned short itemid;
	unsigned char itemuc[4];
	unsigned long idlong;
	int curport=0;

	if(port<=0 || port>MAX_PORT) return ERR_FAILPORT;
	curport = port - 1;
	
	if(alen <12 || alen >AMRMOD_APP_BUFFER_SIZE) return ERR_OUTRANGE;
	if(metid == 0 || metid > MAX_METER) return ERR_OUTRANGE;

	smallcpy(tmpbuf, appbuf, alen);
	pdata = tmpbuf; 

	PrintLog(LOGTYPE_DOWNLINK, "Report[%d] met%d proto=%d, ", port, metid, proto);
	if(proto == APPPROTO_DL1997) {
		itemuc[0] = pdata[10] - 0x33;
		itemuc[1] = pdata[11] - 0x33;
		itemid = MAKE_SHORT(itemuc);
		idlong = (unsigned long)itemid;
		PrintLog(LOGTYPE_DOWNLINK, "id=%04X, read=%04X,%08X\n", itemid, 
			CurrReadStat[curport].PlReadItem, CurrReadStat[curport].PlReadRealId);
	}
	else if(proto == APPPROTO_DL2007) {
		itemuc[0] = pdata[10] - 0x33;
		itemuc[1] = pdata[11] - 0x33;
		itemuc[2] = pdata[12] - 0x33;
		itemuc[3] = pdata[13] - 0x33;
		idlong = MAKE_LONG(itemuc);
		if(0xacacacac == idlong){	//主动上报表号
			return PlMetReportAddrProc(tmpbuf);
		}

		if(CurrReadStat[curport].PlReadRealId == idlong) itemid = CurrReadStat[curport].PlReadItem;  //避免请求与更新的ID不一致
		else itemid = Dl2007ConverItemid(idlong);
		PrintLog(LOGTYPE_DOWNLINK, "id=%08X, id97=%04X, read=%04X,%08X\n", idlong, itemid, 
			CurrReadStat[curport].PlReadItem, CurrReadStat[curport].PlReadRealId);
	}
	else {
		PrintLog(LOGTYPE_DOWNLINK, "error proto\n");
		return ERR_OUTRANGE;
	}

	metid -= 1;

	for(i=0; i<6; i++) {
		if(ParaMeter[metid].addr[i] != pdata[i+1]) {
			return ERR_COMPARE;
		}
	}

	if(proto == APPPROTO_DL1997) {
		applen = Dl1997CheckRead(ParaMeter[metid].addr, itemid, pdata, alen);
	}
	else if(proto == APPPROTO_DL2007) {
//		applen = Dl2007CheckRead(ParaMeter[metid].addr, itemid, pdata, alen);
	}
	else {
		return ERR_COMPARE;
	}

//	PlcState[metid].reported = 1;
	
	if(applen <= 0) PrintLog(LOGTYPE_DOWNLINK, "check error(%d)\n", applen);
	else PrintLog(LOGTYPE_DOWNLINK, "check ok(%d)\n", applen);

	mid = ParaMeter[metid].metp_id;
	if(mid == 0 || mid > MAX_METP) return ERR_OUTRANGE;
	mid -= 1;

//	TimeReadSysClock(&PlcState[metid].oktime);
/*	
	if(applen > 0) {
		if(CurrReadStat[curport].PlReadRealId == idlong) itemid = CurrReadStat[curport].PlReadItem;  //避免请求与更新的ID不一致
		UpdatePlMdb(mid, itemid, pdata, applen);
	}
	else {
		UpdatePlMdb(mid, CurrReadStat[curport].PlReadItem, pdata, applen);
		return ERR_EXECFAIL;
	}

	PlcState[metid].routes = readinginfo.routes;
	PlcState[metid].quality = readinginfo.quality;
	PlcState[metid].phase = readinginfo.phase;
	PlcState[metid].failcount = 0;
	PlcState[metid].okflag = 1;	 
*/
	return ECHO_OK;
}
#endif

/**
* @brief 获取电表参数
* @param port 端口号
* @param metid 电表序号 1~MAX_METER
* @param *proto 表协议 1 - DL/T645-1997; 2 - DL/T645-2007
* @param *addr 表地址
* @param *owneraddr 对应采集器地址
* @return 返回0表示成功，非0表示处理失败
*/
int GetMeterParamProc(int port, unsigned short metid, unsigned char *proto, unsigned char *addr, unsigned char *owneraddr)
{	
	if(port <= 0 || port > MAX_COMMPORT) return ERR_FAILPORT;
	if(metid == 0 || metid > MAX_METER) return ERR_OUTRANGE;

	metid -= 1;
	
	if(EMPTY_METER(metid)) return ERR_INVALID;

//	if(ParaMeter[metid].port != port) return ERR_COMPARE;

//	*proto = (ParaMeter[metid].proto==METTYPE_DL645_2007)?2:1;;
	smallcpy(addr, ParaMeter[metid].addr, 6);
//	smallcpy(owneraddr, ParaMeter[metid].owneraddr, 6);

	return ECHO_OK;
}


static const int UartBaudList[8] = {300, 600, 1200, 2400, 4800, 7200, 9600, 19200};
void UartSetParaFrame(unsigned int port, unsigned char frame)
{
	int baud, databits, stopbits;
	unsigned char uc;
	char parity;

	uc = (frame>>5)&0x07;
	baud = UartBaudList[uc];

	databits = (int)(frame&0x03) + 5;
	if(frame & 0x08) {
		if(frame & 0x04) parity = 'O';
		else parity = 'E';
	}
	else parity = 'N';
	if(frame & 0x10) stopbits = 2;
	else stopbits = 1;

	UartSetPara(port, baud, databits, stopbits, parity);
}

int SepUartSendData(unsigned int port,const unsigned char *buf, int len)
{
	return UartSendData(port, buf, len);
}




/**
* @brief 根据表号读数据
* @param metid 表号
* @param itemid 数据项标识
* @param buf 数据帧缓存区指针
* @param len 缓存区长度
* @return 成功返回实际数据长度, 失败返回负数
*/
int PlReadMeter(unsigned short metid, unsigned short itemid, unsigned char *buf, int len)
{
	int applen, port,cmdlen=0, i;
	unsigned char cmdbuf[AMRMOD_EVENT_BUFFER_SIZE];
	unsigned char eventbuf[AMRMOD_EVENT_BUFFER_SIZE];
	unsigned char proto = 0;
	const usrmet_prot_t *pfunc;
	struct amr_module_t * pmodule;
	
	if(metid >= MAX_METER || EMPTY_METER(metid)) return -1;
	Sleep(5);

	port = -1;//ParaMeter[metid].port;
	if(port <= 0 || port > MAX_COMMPORT) return -1;

//	proto = (ParaMeter[metid].proto == METTYPE_DL645_2007)? 2 : 1;

	pfunc = GetUserMetProto(proto);
	if(NULL == pfunc) return -1;

	cmdlen = (*pfunc->makeread)(ParaMeter[metid].addr, itemid, cmdbuf, MAX_GDWPKT_LEN);
	if(cmdlen < 14) return -1;
	pmodule = FindAmrModule(port);

	if(pmodule == NULL) return -1;

	applen = AMRMakePlEvent(pmodule, AMREVENT_READMET, cmdbuf, cmdlen, eventbuf);
	
	
	if(applen <=0) return -1;

	applen = (*pfunc->checkread)(ParaMeter[metid].addr, itemid, eventbuf, applen);
	
	//PrintLog(0, "check applen =%d\n", applen);
	if(applen <= 0) return -1;

	for(i=0; i<len; i++) {
		if(i < applen) *buf++ = eventbuf[i];
		else *buf++ = 0;
	}

	return applen;
}


/**
* @brief 简单检查Dl645数据包的合法性
* @param pkt 数据包指针
* @param len 数据包长度
* @return 0合法, 1非法
*/
int Chk645Packet(const unsigned char *pdata, int len,dl645_module_t *pkt)
{
	int i;
	const unsigned char *puc;
	unsigned char check;
	dl645_module_t *tmp_pkt;//(pdata+i), applen-i)

	for(i=0; i<len; i++) {
		if(0x68 == pdata[i]) break;
	}
	if((i+8) >= len) return 1;
	tmp_pkt = (dl645_module_t*)pdata+i;
	len=len-i;
	


	if(len < DL645PKTLEN_MIN) return 1;
	if(DL645PKT_HEAD != tmp_pkt->head || DL645PKT_HEAD != tmp_pkt->dep) return 1;

	if(tmp_pkt->len+12 != len) return 1;
	
	check = 0;
	puc = (const unsigned char *)tmp_pkt;
	for(i=0; i<tmp_pkt->len+10; i++) check += *puc++;
	if(check != *puc++) return 1;
	if(DL645PKT_TAIL != *puc) return 1;
	memcpy(pkt,tmp_pkt,sizeof(dl645_module_t));
 	return 0;
}

/**
* @brief 透明转发
* @param pcmd 转发命令帧
* @param pecho 回应数据帧
*/
void PlForwardF1(const struct data_t *pcmd,struct data_t *pecho,unsigned char poto)
{	

	int applen;
	int addr;
	HDLC_STRUCT hdlc;

	mutex_lock(MUTEX_3);
	
	pecho->len=0;
	if(poto == 0)
	{
		if(UnparseDLMSPacket(pcmd->len,pcmd->data,&hdlc) > 0)
		{
			//得到物理地址
			if(hdlc.Addr1.type == 1)
			{
				PrintLog(LOGTYPE_DEBUG,"\r\n地址域只有一个字节,缺乏HDLC物理地址\r\n");
				mutex_unlock(MUTEX_3);
				return;
			}
			if(hdlc.Addr1.LoAddr <0x10)
			{
				PrintLog(LOGTYPE_DEBUG,"\r\nHDLC地址非法\r\n");
				mutex_unlock(MUTEX_3);
				return;	
			}
			addr = hdlc.Addr1.LoAddr;
			PrintLog(LOGTYPE_DEBUG,"\r\nHDLC地址:%d\r\n",addr);
		}
		else
		{
			mutex_unlock(MUTEX_3);
			return;
		}
		if(ParaMeter[addr-1].metp_id>0)
			SetupPlMeterAddr(FindAmrModule(-1),ParaMeter[addr-1].addr);
		else
		{
			mutex_unlock(MUTEX_3);
			return;	
		}
	}
	else
	{
		dl645_module_t pkt;
		if(Chk645Packet(pcmd->data,pcmd->len,&pkt)==0)
			SetupPlMeterAddr(FindAmrModule(-1),pkt.addr);
		else
		{
			mutex_unlock(MUTEX_3);
			return;
		}
	}
	FindAmrModule(-1)->app_proto = poto;
 //透传非645帧数据内容
	applen = AMRMakePlEvent(FindAmrModule(-1), AMREVENT_READMET, (unsigned char *)pcmd->data, pcmd->len, pecho->data);

	if(applen>0) 
	{
		pecho->len = applen;
	}

	mutex_unlock(MUTEX_3);
}
#ifdef TEST_PLC

static int oneHexCharToHex(char h)  
{  
    int x = 0;  
    if (isdigit(h)) {  
        x = h - '0';  
    } else if (isupper(h)) {  
        x = h - 'A' + 10;  
    } else {  
        x = h - 'a' + 10;  
    }  
    return x;  
} 
/*Chaneg TEXT HEX such as E5BC35 to  
* E5BC35 -->0xe5,0xbc,0x35 
* inStr must >0 otherwise return -1*/  
int ChangeTHEX2CPTR(const char * inHexString, char * outHex)  
{  
    int success = -1;  
    int len = 0;  
    int i;  
    char ch1, ch2;  
    do   
    {  
        if (NULL == inHexString || NULL == outHex) {  
            PrintLog(LOGTYPE_DEBUG,"error: inHexString or outHex is null!\n");  
            break;  
        }  
        len = strlen(inHexString);  
        if (len <= 1) {  
            PrintLog(LOGTYPE_DEBUG,"error: strlen(inHexString) <= 1!\n");  
            break;  
        }  
        len &= ~1;  
        for (i=0; i<len; i+=2) {  
            ch1 = inHexString[i];  
            ch2 = inHexString[i+1];  
            outHex[i/2 + 1] = 0;  
            if (isxdigit(ch1) && isxdigit(ch2)) {  
                ch1 = oneHexCharToHex(ch1);  
                ch2 = oneHexCharToHex(ch2);  
                outHex[i/2] = (ch1 << 4) | ch2;  
            } else {  
                PrintLog(LOGTYPE_DEBUG,"error: %c or %c is not hex digit!\n", ch1, ch2);  
                goto exit;  
            }  
        }  
        success = len/2;//OK   
    } while (0);  
exit:  
    return success;  
} 
static int shell_TestPlc(int argc, char *argv[]);
DECLARE_SHELL_CMD("plc", shell_TestPlc, "载波测试命令");
const char* cmds[]={
	"7EA0080221215309177E",
	"7EA0080221219305D17E",
	"7EA02C02212110172AE6E600601DA109060760857405080101BE10040E01000000065F1F040000001CFFFFFAD37E",
	"7EA01A022121324EC7E6E600C001C100030100010800FF020032687E",
	"7EA01A02212132064BE6E600C001C100030100010801FF0200E7F77E",
	"7EA01A02212132064BE6E600C001C100030100010802FF0200E7F77E",
	"7EA01A02212132064BE6E600C001C100030100010803FF0200E7F77E"
	};
unsigned char cmd_buf[600];
//static const unsigned char ddd[]={0x7E,0xA0,0xB2,0x48,0x68,0xFE,0xFF,0x75,0x32,0x0E,0x3B,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x00,0x02,0xDE,0x82,0x00,0x00,0x00,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x00,0x02,0xDE,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x00,0x02,0xDE,0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x7E};
//static const unsigned char ddd[]={0x7E,0xA0,0x08,0x02,0x21,0x21,0x93,0x05,0xD1,0x7E};
//static const unsigned char ddd[]={68,0xD1,0x00,0x41,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x66,0x55,0x00,0x02,0x01,0x00,0x00,0xB4,0x7E,0xA0,0xB2,0x48,0x68,0xFE,0xFF,0x75,0x32,0x0E,0x3B,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x00,0x02,0xDE,0x82,0x00,0x00,0x00,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x0E,0x3B,0xE6,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x00,0x02,0xDE,0xE6,0x00,0xC0,0x01,0x81,0x00,0x07,0x00,0x00,0x63,0x01,0x00,0xFF,0x00,0x02,0xDE,0x82,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0xBE,0x16};
//static const unsigned char ddd[] = {0x7E,1,2,3,4,5,6,7,8,9,0x7E};
/*static const unsigned char ddd[] = {0x7E,0xA0,0x2C,0x02,0x21,0x21,0x10,0x17,0x2A,0xE6,0xE6,0x00,0x60,
0x1D,0xA1,0x09,0x06,0x07,0x60,0x85,0x74,0x05,0x08,0x01,0x01,0xBE,0x10,0x04,0x0E,0x01,0x00,0x00,
0x00,0x06,0x5F,0x1F,0x04,0x00,0x00,0x00,0x1C,0xFF,0xFF,0xFA,0xD3,0x7E};
*/
static const unsigned char ad[]={0x00,0x00,0x01,0x00,0x00,0x00};
static unsigned char rb[600];
struct PlcTestParam_t
{
	unsigned char run;
	unsigned char once;
	int Interval;
	int Timeout;
	unsigned char index;
};
struct PlcTestParam_t PlcTestParam={0,0,10,100,0};
int GetRskTimeout(void)
{
	return PlcTestParam.Timeout;
}
int SendCount=1,ReciveCount=1;
extern int MkGoodDLMSPacket(int rxlen,unsigned char * rxbuf);
void SendPlcCmd(int index)
{
	int applen = 0;
	int cmd_len;

	if((cmd_len=ChangeTHEX2CPTR(cmds[index],(char*)cmd_buf))>0)
	{
		MkGoodDLMSPacket(cmd_len,cmd_buf);
		PrintLog(LOGTYPE_DEBUG,"\r\n(++TX %04d Times++)#############Running Plc Test!#############\r\n",SendCount++);
		SetupPlMeterAddr(FindAmrModule(-1),ad);
		applen = AMRMakePlEvent(FindAmrModule(-1), AMREVENT_READMET, (unsigned char *)cmd_buf, cmd_len, rb);
	
		if(applen>0) 
		{
	//		int i;
			PrintLog(LOGTYPE_DEBUG,"\r\n(++RX %04d Times++)#############Plc recived#############\r\n",ReciveCount++);
		/*	PrintLog(LOGTYPE_DEBUG,"Plc recv:\r\n");
			for(i=0;i<applen;i++)
			{
				PrintLog(LOGTYPE_DEBUG,"%02X",rb[i]);
				if((i%8) == 0)
					PrintLog(LOGTYPE_DEBUG,":");
			}
			PrintLog(LOGTYPE_DEBUG,"\r\n");*/
		}		
	}
	
}
void TestPlc(void)
{
	int i;

	

	if((PlcTestParam.run == 0)&&(PlcTestParam.once == 0))
		return;
	PlcTestParam.once = 0;	
	
	for(i=0;i<3;i++)
	{
		SendPlcCmd(i);
		Sleep(50);
	}

	if(PlcTestParam.index)
	{
		SendPlcCmd(PlcTestParam.index+2);	
	}
		
	Sleep(50);
}

static int shell_TestPlc(int argc, char *argv[])
{
	if(argc>=2)
	{
		if(strcmp(argv[1],"start") == 0)
		{
			 PlcTestParam.run = 1;
			 return 0;
		}
		else if(strcmp(argv[1],"stop") == 0)
		{
			  PlcTestParam.run = 0;
			  return 0;
		}
		else if(strcmp(argv[1],"index") == 0)
		{
			  if(argc >= 3)
			  {
			  	  PlcTestParam.index = atoi(argv[2]);
			  }
			  return 0;
		}
		else if(strcmp(argv[1],"set") == 0)
		{
			if(argc >= 4)
			{
				if(strcmp(argv[2],"Interval")==0)
				{
					PlcTestParam.Interval = atoi(argv[3]);
					return 0;
				}else if(strcmp(argv[2],"Timeout")==0)
				{
					PlcTestParam.Timeout = atoi(argv[3]);
					return 0;
				}
					
			}
		}else if(strcmp(argv[1],"once")==0)
		{
			PlcTestParam.once = 1;
			return 0;
		}
	}
	PrintLog(LOGTYPE_DEBUG,"plc 命令: \r\n");
	PrintLog(LOGTYPE_DEBUG,"plc start \r\n");
	PrintLog(LOGTYPE_DEBUG,"plc stop\r\n");
	PrintLog(LOGTYPE_DEBUG,"plc set Interval x\r\n");
	PrintLog(LOGTYPE_DEBUG,"plc set Timeout  x\r\n");
	PrintLog(LOGTYPE_DEBUG,"plc once\r\n");
	PrintLog(LOGTYPE_DEBUG,"plc index\r\n");
	
	return 0;
}
#endif
/**
* @brief 透明转发读数据 AFN=10,F9
* @param dest 目标地址
* @param itemid 数据项标识
* @param buf 数据帧缓存区指针
* @param len 缓存区长度
* @return 成功返回实际数据长度, 失败返回负数
*/
/*int PlForwardRead(const plc_dest_t *dest, unsigned long itemid, unsigned char *buf, int len)
{
	plc_module_t *pmodule;
	plc_dest_t *pdest;
	int applen;

	pmodule = FindPlcModule(dest->portcfg);
	if(NULL == pmodule) return -1;

	if(pmodule->flag_busy) return -1;

	SysLockMutex(&pmodule->comm_lock);

	pdest = (plc_dest_t *)pmodule->event_cmd_buf;
	memcpy(pdest, dest, sizeof(plc_dest_t));

	if(dest->proto == METTYPE_DL645_2007) {
		DEPART_LONG(itemid, (unsigned char *)(pdest+1));
	}
	else {
		DEPART_SHORT(itemid, (unsigned char *)(pdest+1));
	}

	applen = MakePlEvent(PLEVENT_FORWARDF9);

	if(applen <= 0 || applen > len) {
		SysUnlockMutex(&pmodule->comm_lock);
		return -1;
	}

	memcpy(buf, pmodule->event_echo_buf, applen);

	SysUnlockMutex(&pmodule->comm_lock);
	return applen;
}*/

/**
* @brief 载波控制命令
* @param port 端口号
* @param amrcmd 控制抄表命令
* @return 成功返回0, 失败返回1
*/
int PlCtrlCmd(enum amrctrl_cmd amrcmd, int port)
{
	int applen, cmdlen;
	

	unsigned char cmdbuf[AMRMOD_EVENT_BUFFER_SIZE];
	unsigned char eventbuf[AMRMOD_EVENT_BUFFER_SIZE];

	cmdbuf[0] = amrcmd;
	cmdlen = 1;

	applen = AMRMakePlEvent(FindAmrModule(port), AMREVENT_CTRLCMD, cmdbuf, cmdlen, eventbuf);

	if(applen <0) return -1;

	return 0;
}

/**
* @brief 载波控制命令
* @param port 端口号
* @param amrcmd 控制抄表命令
* @return 成功返回0, 失败返回1
*/
int PlUserMetCtrlCmd(enum amrctrl_cmd amrcmd)
{
	int port, applen, cmdlen;
	unsigned char cmdbuf[AMRMOD_EVENT_BUFFER_SIZE];
	unsigned char eventbuf[AMRMOD_EVENT_BUFFER_SIZE];

	cmdbuf[0] = amrcmd;
	cmdlen = 1;
	port = COMMPORT_PLC+1;
	applen = AMRMakePlEvent(FindAmrModule(port), AMREVENT_CTRLCMD, cmdbuf, cmdlen, eventbuf);

//	port = COMMPORT_USR_485+1;
//	applen = AMRMakePlEvent(FindAmrModule(port), AMREVENT_CTRLCMD, cmdbuf, cmdlen, eventbuf);
	
	if(applen <0) return -1;

	return 0;
}

static int TimerIdPlChnageMeterParam = -1;
static int CTimerPlChnageMeterParam(unsigned long arg)
{
	TimerIdPlChnageMeterParam = -1;
	AMRBroadcastPlEvent(AMREVENT_CHANGEMET);
	return 1;
}

/**
* @brief 修改载波表参数
*/
void PlChangeMeterParam(void)
{
	if(TimerIdPlChnageMeterParam < 0) TimerIdPlChnageMeterParam = SysAddCTimer(90, CTimerPlChnageMeterParam, 0);
	else SysClearCTimer(TimerIdPlChnageMeterParam);
}

/**
* @brief 载波忙定时器
*/
/*static int CTimerPlCommBusy(unsigned long arg)
{
	plc_module_t *pmodule = (plc_module_t *)arg;

	pmodule->flag_busy = 0;

	return 1;
}*/

/**
* @brief 启动表号自动上报
* @param flag  1- 启动搜表， 0 - 停止抄表
* @return 成功返回0, 失败返回1
*/
int PlMeterReportCtrl(int flag)
{
#if 0
	int port, applen, cmdlen;
	unsigned char cmdbuf[AMRMOD_EVENT_BUFFER_SIZE];
	unsigned char eventbuf[AMRMOD_EVENT_BUFFER_SIZE];
	struct search_ctrlparam *ctrlparam;

	if((0 != flag) &&(1 != flag)) return 1;

	ctrlparam = (struct search_ctrlparam *)cmdbuf;

	ctrlparam->ctrlcmd = flag;
	ctrlparam->wait_search_time = ParaUni.waitsearchtime;
	ctrlparam->report_timer = ParaUni.waitreptimer;
	if(GetProjectNumber() == VERPROJ_ZHEJIANG){
		ctrlparam->wait_report_time = ParaUni.waitreporttime;
	}
	
	cmdlen = sizeof(struct search_ctrlparam);
	port = COMMPORT_PLC+1;
	applen = AMRMakePlEvent(FindAmrModule(port), AMREVENT_METERREPORT, cmdbuf, cmdlen, eventbuf);

	if(applen <0) return -1;
#endif
	return 0;

}

/**
* @brief 添加载波表到模块中
* @param metid 表序号
* @return 成功返回0, 失败返回1
*/
int PlAddrMeter(unsigned short metid)
{
	PlChangeMeterParam();
	//AMRMakePlEventNoWait();
	
	//AMRMakePlEvent(port, PLEVENT_READMET, cmdbuf, cmdlen, eventbuf);
	return 0;
}

/**
* @brief 删除载波模块中的表地址
* @param addr 表地址
* @return 成功返回0, 失败返回1
*/
int PlDelMeter(unsigned short metid)
{
	PlChangeMeterParam();
	return 0;
}


static int ProtoCheckRead(const unsigned char *addr, unsigned long itemid, unsigned char *buf, int len, int proto)
{
	unsigned char check, *puc;
	int i, datalen;
	unsigned short us;

	if(len < 14) return -1;
	if(0x68 != buf[0] || 0x68 != buf[7]) return -2;
	for(i=0; i<6; i++) {
		if(addr[i] != buf[i+1]) return -3;
	}

	if(proto == 1) {
		if((buf[8]&0xdf) != 0x81) return -4;
		us = ((unsigned short)(buf[11]-0x33)<<8) + (unsigned short)(buf[10]-0x33);
		if(us != (unsigned short)itemid) return -5;
	}
	else {
		unsigned long ul;

		if((buf[8]&0xdf) != 0x91) return -4;
		ul = (((unsigned char)(buf[13]-0x33))<<24) + (((unsigned char)(buf[12]-0x33))<<16)
				+(((unsigned char)(buf[11]-0x33))<<8) + ((unsigned char)(buf[10]-0x33));
		if(ul != itemid) return -5;
	}
	datalen = (int)buf[9]&0xff;
	if(datalen < 2 || (datalen+12) > len) return -6;

	puc = buf;
	check = 0;
	for(i=0; i<(datalen+10); i++) check += *puc++;
	if(check != *puc++) return -7;
	if(0x16 != *puc) return -8;

	if(proto == 2) {
		if(datalen < 4) return -9;
		datalen -= 4;
		puc = buf + 14;
	}
	else {
		datalen -= 2;
		puc = buf + 12;
	}

	for(i=0; i<datalen; i++) buf[i] = (*puc++) - 0x33;

	return datalen;
}

/**
* @brief 转发抄读数据命令
* @param rbuf 命令缓存区指针
* @param rlen 命令缓存区长度
* @param sbuf 返回数据缓存区指针
* @param smaxlen 返回数据缓存区最大长度
* @return 成功返回返回数据长度, 失败返回-1
*/
static int ForwardReadMetData(const unsigned char *rbuf, int rlen, unsigned char *sbuf, int smaxlen)
{
	unsigned long itemid=0;
	int  applen, cmdlen,port;
	unsigned char proto, errcode,routenum,i,*puc,idlen=4;
	unsigned char tmpbuf[64],addr[6],cmdbuf[256],check;
	unsigned short idshort=0;
	int protol=1;
	
	port=rbuf[0];
	puc=cmdbuf;
	
	routenum = rbuf[1];
	if(routenum > PLC_ROUTENUM) return -1;
	i = (unsigned int)routenum * 6;
	if((i+8) > rlen) return -1;

	memcpy(addr,rbuf+2+i,6);
	rbuf+=i+8;
	rlen-=i+8;
	proto = rbuf[0] & 0x03;
	if(proto==1) {
		itemid = MAKE_LONG(rbuf+1);
		idlen=4;
	}
	else if(proto == 0) {
		itemid = MAKE_SHORT(rbuf+1);
		idshort=(unsigned short)itemid;
		idlen=2;
	}
	
	*puc++ = 0x68;
	for(i=0; i<6; i++) *puc++ = addr[i];
	*puc++ = 0x68;
	if(proto == 1) {
		*puc++ = 0x11;
		*puc++ = 0x04;
		DEPART_LONG(itemid, puc);
		for(i=0; i<4; i++) puc[i] += 0x33;
		puc=cmdbuf;
		check = 0;
		for(i=0; i<14; i++) check += *puc++;
		cmdlen = 16;
	}
	else {
		*puc++ = 0x01;
		*puc++ = 0x02;
		*puc++ = (unsigned char)(idshort) + 0x33;
		*puc++ = (unsigned char)(idshort>>8) + 0x33;
 		puc=cmdbuf;
		check = 0;
		for(i=0; i<12; i++) check += *puc++;
		cmdlen = 14;
	}
	
	*puc++ = check;
	*puc = 0x16;

	applen = AMRMakePlEvent(FindAmrModule(port), AMREVENT_READMET, cmdbuf, cmdlen, tmpbuf);

	
	if(applen <=0) return -1;
	
	if(proto==1) protol=2;
	else protol=1;
	applen = ProtoCheckRead(addr, itemid, tmpbuf, applen, protol);
	
	
	if(applen <= 0) errcode = 1;
	else errcode = 5;
	
	sbuf[0] = sbuf[1] = 0;
	sbuf[2] = 1;  //F9
	sbuf[3] = 1;
	sbuf += 4;

	*sbuf++ = port;
	smallcpy(sbuf, addr, 6); sbuf += 6;
	*sbuf++ = errcode;
	if(errcode != 5) {
		*sbuf++ = idlen;
		DEPART_LONG(itemid, sbuf);
		return(idlen+13);
	}

	if((applen + idlen+13) > smaxlen) return -1;

	*sbuf++ = idlen+applen;
	DEPART_LONG(itemid, sbuf);
	sbuf += idlen;

	smallcpy(sbuf, tmpbuf, applen);
	smallcpy(sbuf, tmpbuf, applen);
	

	return (applen+idlen+13);

}


/**
* @brief 载波透明转发命令
* @param rbuf 命令缓存区指针
* @param rlen 命令缓存区长度
* @param sbuf 返回数据缓存区指针
* @param smaxlen 返回数据缓存区最大长度
* @return 成功返回返回数据长度, 失败返回-1
*/
int PlcForward(const unsigned char *rbuf, int rlen, unsigned char *sbuf, int smaxlen)
{
	int rtn=0;
	unsigned char cmd=0;
	
	if(rlen < 4) return -1;

	if(0 != rbuf[0] || 0 != rbuf[1]) return -1;	//p0
	if(1 != rbuf[3]) return -1;	//f9-f16
	cmd = rbuf[2];

	rbuf += 4;
	rlen -= 4;
		
	switch(cmd) {
	case 0x01://F9转发主站直接对电表的抄读
		rtn = ForwardReadMetData(rbuf, rlen, sbuf, smaxlen);
		break;
	default:
		rtn = -1;
		break;
	}

	return rtn;

}

int GetPlModuleName(char *module_name)
{
	AMRGetModuleName(FindAmrModule(31), module_name);

	return 0;
}

int GetPlModuleNumber(void)
{
	int projectNo;
    projectNo=AMRGetModuleNo(FindAmrModule(31));
	return projectNo;

}
int GetPlModuleVersion(char *module_ver)
{
	AMRGetModuleVersion(FindAmrModule(31), module_ver);

	return 0;
}
/**
* @brief amr初始化函数
* @return 返回0表示成功, 否则失败
*/

int PlcCommInit(void)
{
	struct amr_module_t *pmodule;
	#ifdef TEST_PLC
	INIT_SHELL_CMD(shell_TestPlc);
	#endif
	pmodule = NewAmrModule();
	if(pmodule == NULL) 
	{
		PrintLog(LOGTYPE_DEBUG,"PlcCommInit Init:NewAmrModule return NULL\r\n");
		return 1;
	}

	AMR_DATAEXEC_FUNC amrdataexec;
	
	PrintLog(LOGTYPE_DEBUG,"  amr init..\n");

//	memset(CurrReadStat, 0x00, sizeof(CurrReadStat));

	UartOpen(PLC_PORT);
	UartSetPara(PLC_PORT,9600,8,1,'E');

	amrdataexec.pget_metparam = GetMeterParamProc;
	amrdataexec.pget_readcontent = NULL;//GetUserMeterReadContent;
	amrdataexec.pproc_reportdata = NULL;//UserMeterReportDataProc;
	AMRInitDataExecFun(pmodule, amrdataexec);
	
	AMRModuleStartup(pmodule);

//	SET_INIT_FLAG(PlcCommInit);
	return 0;
}

#endif
