/**
* curve.c -- 历史数据查询接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-13
* 最后修改时间: 2010-2-8
*/

//#include <stdio.h>
#include <string.h>

#include "app_include/version.h"
#include "plat_include/debug.h"
#include "plat_include/timeal.h"
#include "app_include/param/capconf.h"
#include "app_include/param/metp.h"
#include "app_include/param/mix.h"
#include "app_include/cenmet/dbconfig.h"
#include "app_include/cenmet/dbase.h"
#include "app_include/cenmet/mdbconf.h"
#include "app_include/cenmet/qrycurve.h"
#include "app_include/lib/bcd.h"
#include "app_include/lib/dbtime.h"
#include "app_include/lib/align.h"
#include "app_include/param/meter.h"
#include "app_include/cenmet/mdbcur.h"

typedef int (*query_fn)(unsigned short dbid, qrycurve_buffer_t **psendbuffer, const dbaseconfig_t *pconfig, int offset);

struct special_copy {
	unsigned char gprid;
	int (*pfunc)(const unsigned char *psrc, unsigned char *pdst, const dbsonconfig_t *pson, int offset);
};

static dbtime_t QueryDbTime;
static unsigned char QueryFrez;
static int QueryDataNum;
//static int QueryTask;  // 1-主动任务发送, 0-查询
static unsigned char QueryDuid[4];
static int QueryStep;
static int QueryMetpid;
static const struct special_copy *SpConfig;
static int SpConfigNum;

/**
* @brief 处理需量(F185~F196)数据拷贝
* @param psrc 源数据缓存区指针
* @param pdst 目的数据缓存区指针
* @param pson 数据库子项配置信息
* @param offset 数据库内数据项信息索引偏移值
* @return 成功返回数据长度, 失败返回-1
*/
static int SpecialCopyDmn(const unsigned char *psrc, unsigned char *pdst, const dbsonconfig_t *pson, int offset)
{
	int i;

	for(i=0; i<MAXNUM_METPRD; i++) {
		smallcpy(pdst, &psrc[3*i], 3);
		smallcpy(pdst+3, &psrc[3*MAXNUM_METPRD+4*i], 4);
		pdst += 7;
	}

	return 35;
}

static int SpecialCopyPlStic(const unsigned char *psrc, unsigned char *pdst, const dbsonconfig_t *pson, int offset)
{
	int i;
	unsigned short failnum;

	failnum = MAKE_SHORT(QueryDuid+2);
	if(failnum != 0x0640)  return 0;  //F55

	for(i=0; i<3; i++) *pdst++ = *psrc++;
	psrc++;
	failnum = MAKE_SHORT(psrc); psrc += 2;
	if(failnum > 100) failnum = 100;
	DEPART_SHORT(failnum, pdst); pdst += 2;

	if(failnum) memcpy(pdst, psrc, failnum*2);

	return(failnum*2+5);
}

static const struct special_copy SpecialCopyConfig[] = {
	{6,  SpecialCopyPlStic},
	{23, SpecialCopyDmn},
	{24, SpecialCopyDmn},
};

#define SIZE_SPECIALCOPYCONFIG		(sizeof(SpecialCopyConfig)/sizeof(SpecialCopyConfig[0]))

/**
* @brief 处理特殊拷贝
* @param psrc 源数据缓存区指针
* @param pdst 目的数据缓存区指针
* @param pconfig 数据库配置信息
* @param offset 数据库内数据项信息索引偏移值
* @return 成功返回数据长度, 失败返回-1, 不做处理返回0
*/
static int SpecialCopy(const unsigned char *psrc, unsigned char *pdst, const dbaseconfig_t *pconfig, int offset)
{
	int i;

	for(i=0; i<SpConfigNum; i++) {
		if(pconfig->grpid == SpConfig[i].gprid) {
			return (*(SpConfig[i].pfunc))(psrc, pdst, pconfig->psons+offset, offset);
		}
	}

	return 0;
}
extern void SetDataFlgValid(void);
/**
* @brief 查询日冻结历史数据
* @param dbid 数据库ID
* @param sendbuffer 返回数据缓存链表
* @param pconfig 数据库配置信息
* @param offset 数据库内数据项信息索引偏移值
* @return 成功0, 否则失败
*/
static int QueryDbaseDay(unsigned short dbid, qrycurve_buffer_t **psendbuffer, const dbaseconfig_t *pconfig, int offset)
{
	const dbsonconfig_t *pson = pconfig->psons + offset;
	int cpylen, addlen, metpoffset;
	qrycurve_buffer_t *pbuffer = *psendbuffer;
	unsigned char *pcache, *puc;
	dbtime_t dbtime;//, dbtimecur;
	int emptyflag = 0;
	metpoffset = QueryMetpid * pconfig->colen;

	DebugPrint(0, "call query dbaseday \n");


	QueryDbTime.s.tick = 0;
	dbtime.u = QueryDbTime.u;
	
	{
		sysclock_t clock;
		SysClockReadCurrent(&clock);
//		SYSCLOCK_DBTIME(&clock, dbtimecur);
		//if(QueryTask && pconfig->attr == DBATTR_UMETP) DbTimeDelOneDay(&dbtimecur); //载波表数据只能发前1天的数据
	}
#if 0
/*
 * 由于日冻结数据都是在0:00分冻结的，因此冻结的是前一天
 * 的数据，所以查询日期推后一天
 */
	if(0 == QueryTask) {
		DbTimeAddOneDay(&dbtime);
	}
#endif


	while(DbaseRead(dbid, DBFREZ_DAY, dbtime) < 0) {
		emptyflag = 1;
		break;
	/*	DbTimeAddOneDay(&dbtime);
		DbTimeAddOneDay(&QueryDbTime);
		if(QueryDbTime.u >= dbtimecur.u) return 1;*/
	}
	
	DebugPrint(0, "run query dbaseday stp 1\n");

	addlen = 0;
	if(pson->flag & DBFLAG_RDTIME) addlen += 5;
	if(pson->flag & DBFLAG_FENUM) addlen += 1;

	cpylen = (int)pson->len&0xff;
	QRYCBUF_SCAN(pbuffer, cpylen+addlen+7, *psendbuffer, QueryStep);
	if(NULL == pbuffer) return 1;

	pcache = DbaseReadCache();
	pcache += metpoffset;
	puc = QRYCBUF_DATA(pbuffer);

	puc[0] = QueryDuid[0];
	puc[1] = QueryDuid[1];
	puc[2] = QueryDuid[2];
	puc[3] = QueryDuid[3];
	puc += 4;

	puc[0] = QueryDbTime.s.day;
	puc[1] = QueryDbTime.s.month;
	puc[2] = QueryDbTime.s.year;
	
	HexToBcd(puc, 3);
	puc += 3;

	if(pson->flag & DBFLAG_RDTIME) {
		smallcpy(puc, pcache, 5);
		puc += 5;
	}
	if(pson->flag & DBFLAG_FENUM) {
		*puc++ = MAXNUM_FEEPRD;
	}

	if(emptyflag) memset(pcache+pson->offset, FLAG_MDBFAIL, cpylen);
	else SetDataFlgValid();

	DebugPrint(0, "run query dbaseday stp 2\n");

	if(pson->flag & DBFLAG_SPECIAL) {
		int rtn;

		rtn = SpecialCopy(pcache+pson->offset, puc, pconfig, offset);
		if(rtn < 0) return 1;
		else if(0 == rtn) smallcpy(puc, pcache + pson->offset, cpylen);
		else cpylen = rtn;
	}
	else smallcpy(puc, pcache + pson->offset, cpylen);

	QRYCBUF_ADD(pbuffer, cpylen + addlen + 7);
	
	DebugPrint(0, "run query dbaseday stp 3\n");

	return 0;
}

/**
* @brief 查询月冻结历史数据
* @param dbid 数据库ID
* @param sendbuffer 返回数据缓存链表
* @param pconfig 数据库配置信息
* @param offset 数据库内数据项信息索引偏移值
* @return 成功0, 否则失败
*/
static int QueryDbaseMonth(unsigned short dbid, qrycurve_buffer_t **psendbuffer, const dbaseconfig_t *pconfig, int offset)
{
	const dbsonconfig_t *pson = pconfig->psons + offset;
	int cpylen, addlen, metpoffset;
	qrycurve_buffer_t *pbuffer = *psendbuffer;
	unsigned char *pcache, *puc;
	dbtime_t dbtime;//, dbtimecur;
       int emptyflag = 0;
	metpoffset = QueryMetpid * pconfig->colen;

	QueryDbTime.s.day = 1;
	QueryDbTime.s.tick = 0;
	dbtime.u = QueryDbTime.u;

	{
		sysclock_t clock;
		SysClockReadCurrent(&clock);
//		SYSCLOCK_DBTIME(&clock, dbtimecur);
		//if(QueryTask && pconfig->attr == DBATTR_UMETP) DbTimeDelOneDay(&dbtimecur); //载波表数据只能发前1天的数据
	}

#if 0
/*
 * 由于月冻结数据都是在0:00分冻结的，因此冻结的是前一月
 * 的数据，所以查询日期推后一月
 */
	if(0 == QueryTask) {
		DbTimeAddOneMonth(&dbtime);
	}
#endif

	while(DbaseRead(dbid, DBFREZ_MONTH, dbtime) < 0) {
		emptyflag = 1;
		break;
/*		DbTimeAddOneMonth(&dbtime);
		DbTimeAddOneMonth(&QueryDbTime);
		if(QueryDbTime.u >= dbtimecur.u) return 1;*/
	}

	addlen = 0;
	if(pson->flag & DBFLAG_RDTIME) addlen += 5;
	if(pson->flag & DBFLAG_FENUM) addlen += 1;

	cpylen = (int)pson->len&0xff;
	QRYCBUF_SCAN(pbuffer, cpylen+addlen+6, *psendbuffer, QueryStep);
	if(NULL == pbuffer) return 1;

	pcache = DbaseReadCache();
	pcache += metpoffset;
	puc = QRYCBUF_DATA(pbuffer);

	puc[0] = QueryDuid[0];
	puc[1] = QueryDuid[1];
	puc[2] = QueryDuid[2];
	puc[3] = QueryDuid[3];
	puc += 4;
	
	puc[0] = QueryDbTime.s.month;
	puc[1] = QueryDbTime.s.year;

	HexToBcd(puc, 2);
	puc += 2;

	if(pson->flag & DBFLAG_RDTIME) {
		smallcpy(puc, pcache, 5);
		puc += 5;
	}
	if(pson->flag & DBFLAG_FENUM) {
		*puc++ = MAXNUM_FEEPRD;
	}

	if(emptyflag) memset(pcache+pson->offset, FLAG_MDBFAIL, cpylen);
	else SetDataFlgValid();

	if(pson->flag & DBFLAG_SPECIAL) {
		int rtn;

		rtn = SpecialCopy(pcache+pson->offset, puc, pconfig, offset);
		if(rtn < 0) return 1;
		else if(0 == rtn) smallcpy(puc, pcache + pson->offset, cpylen);
		else cpylen = rtn;
	}
	else smallcpy(puc, pcache + pson->offset, cpylen);

	QRYCBUF_ADD(pbuffer, cpylen+addlen+6);

	return 0;
}

/*static inline int DataIsEmpty(const unsigned char *buf, int len)
{
	int i;

	for(i=0; i<len; i++) {
		if(*buf++ != 0xee) return 0;
	}
	return 1;
}*/

#if 0
#define ADD_ONEDAY(dbtime, badded) { \
	if(!badded) { \
		badded = 1; \
		switch(QueryFrez) { \
		case DBFREZ_15MIN: dbtime.s.tick = 1; break; \
		case DBFREZ_30MIN: dbtime.s.tick = 2; break; \
		default: dbtime.s.tick = 4; break; \
		} \
	} \
	DbTimeAddOneDay(&(dbtime)); \
}
#else
#define ADD_ONEDAY(dbtime) { \
	dbtime.s.tick = 0; \
	DbTimeAddOneDay(&(dbtime)); \
}
#endif

/**
* @brief 查询曲线类历史数据
* @param dbid 数据库ID
* @param sendbuffer 返回数据缓存链表
* @param pconfig 数据库配置信息
* @param offset 数据库内数据项信息索引偏移值
* @return 成功0, 否则失败
*/
static int QueryDbaseCurve(unsigned short dbid, qrycurve_buffer_t **psendbuffer, const dbaseconfig_t *pconfig, int offset)
{
	const dbsonconfig_t *pson = pconfig->psons + offset;
	int cpylen, itemsize, itemnum, frezdev;
	qrycurve_buffer_t *pbuffer = *psendbuffer;
	unsigned char *pcache, *puc;
	int starti, endi, i, maxi, metpoffset;
	dbtime_t dbtime, dbtimecur;
	int emptyflag = 0;

	/*DebugPrint(0, "query curve dbid=%d, offset=%d, %02d-%d-%d %d, frez=%d, active=%d\n",
				dbid, offset, QueryDbTime.s.year, QueryDbTime.s.month, QueryDbTime.s.day,
				QueryDbTime.s.tick, QueryFrez, QueryTask);*/

	metpoffset = QueryMetpid * pconfig->colen;

	switch(QueryFrez) {
	case DBFREZ_15MIN: frezdev = 15; break;
	case DBFREZ_30MIN: frezdev = 30; break;
	case DBFREZ_1HOUR: frezdev = 60; break;
	default: return 1;
	}

	dbtime.u = QueryDbTime.u;
	{
		sysclock_t clock;
		SysClockReadCurrent(&clock);
		SYSCLOCK_DBTIME(&clock, dbtimecur);
	}
	if(dbtime.u > dbtimecur.u) return 1;

	itemsize = DbaseItemSize(dbid);
	if(itemsize <= 0) return 1;
	itemnum = DbGetItemNum(QueryFrez);
	if(itemnum <= 0) return 1;

	maxi = itemnum;
	if(DBTIME_ISONEDAY(dbtime, dbtimecur)) {
		maxi = DbGetItemOffset(itemnum, dbtimecur);
		maxi += 1;
	}

	while(DbaseRead(dbid, QueryFrez, dbtime) < 0) {
		emptyflag = 1;
		break;
//		ADD_ONEDAY(dbtime);
//		if(dbtime.u >= dbtimecur.u) return 1;
	}

	cpylen = (int)pson->len&0xff;
	starti = DbGetItemOffset(itemnum, dbtime);

mark_recopy:
	//skip the head empty buffer
	pcache = DbaseReadCache();
	pcache += metpoffset;

#if 1
	endi = maxi;
#else
	puc = pcache + starti*itemsize;

	for(;starti<maxi; starti++,puc+=itemsize) {
		if(!DataIsEmpty(puc+pson->offset, cpylen)) break;
	}
	if(starti >= maxi) {
		//DebugPrint(0, "all empty\n");
		if(0 == QueryTask) return 1;

		ADD_ONEDAY(dbtime);
		if(dbtime.u >= dbtimecur.u) return 1;
		goto mark_retry;
	}

	endi = starti+1;
	puc = pcache + endi*itemsize;
	i = 0;
	for(; endi<maxi; endi++,puc+=itemsize) {
		if(DataIsEmpty(puc+pson->offset, cpylen)) {
			i++;
			if(i >= 4) break;  //太多空数据
		}
		else i = 0;
	}
	if(i) {
		if(endi < maxi) endi -= (i-1);
		else endi -= i;
	}
#endif

	i = endi - starti;
	if(i > QueryDataNum) {
		endi = starti + QueryDataNum;
		i = endi -starti;
	}

	//防止过长(一个最长帧填不下)
	{
		int tmplen = cpylen*i+11;
		while(pbuffer->maxlen < tmplen) {
			endi--;
			i--;
			tmplen -= cpylen;
			if(i <= 0) return 1;
		}
	}
	QRYCBUF_SCAN(pbuffer, cpylen*i+11, *psendbuffer, QueryStep);
	if(NULL == pbuffer) return 1;

	puc = QRYCBUF_DATA(pbuffer);
	QRYCBUF_ADD(pbuffer, cpylen*i+11);

	puc[0] = QueryDuid[0];
	puc[1] = QueryDuid[1];
	puc[2] = QueryDuid[2];
	puc[3] = QueryDuid[3];
	puc += 4;
	puc[4] = dbtime.s.year;
	puc[3] = dbtime.s.month;
	puc[2] = dbtime.s.day;
	puc[1] = (frezdev*starti)/60;
	puc[0] = (frezdev*starti)%60;
	HexToBcd(puc, 5);
	puc[5] = QueryFrez;
	puc[6] = i;
	puc += 7;

	pcache += itemsize*starti;
	for(; starti<endi; starti++) {
		if(emptyflag == 1){
			memset(puc, FLAG_MDBFAIL, cpylen);
		}
		else{
			SetDataFlgValid();
			if(pson->flag & DBFLAG_SPECIAL) {
				int rtn;

				rtn = SpecialCopy(pcache+pson->offset, puc, pconfig, offset);
				if(rtn < 0) return 1;
				else if(0 == rtn) smallcpy(puc, pcache + pson->offset, cpylen);
				else cpylen = rtn;
			}
			else smallcpy(puc, pcache + pson->offset, cpylen);
		}
		puc += cpylen;
		pcache += itemsize;
		QueryDataNum -= 1;
	}

	if( QueryDataNum > 0) {
		if(endi >= maxi) {
			return 0;
		}
		else {
			starti = endi;
			goto mark_recopy;
		}
	}

	return 0;
}
/**
* @brief 查询历史数据
* @param itemid 数据标识指针
* @param timemark 查询时间标识指针
* @param timemarklen 查询时间标识最大长度
* @param sendbuf 返回数据缓存链表
* @param bactive 是否主动发送, 0-查询, 1主动发送, D1~D7曲线抽取密度
* @return 成功返回查询时间标识长度, 无法识别返回-1, 缓存区溢出返回0
*/
int QueryCurve(const unsigned char *itemid, const unsigned char *timemark, int timemarklen, qrycurve_buffer_t *sendbuffer, int bactive)
{
	const dbaseconfig_t *pconfig;
	int offset, rtn;
	query_fn pfunc;
	unsigned short dbid, metpid,metersn;
	unsigned char fnmask;
	
	DebugPrint(0, "call query curve \n");
	
	if(QRYCBUF_MAXLEN(sendbuffer) < 4) {
		QRYCBUF_NEXT(sendbuffer);
		if(NULL == sendbuffer) return 0;
	}
	
	DebugPrint(0, "run query curve stp 1\n");

	//PrintLog(0, "qry %02X%02X%02X%02X, timelen=%d, bactive=%d...\n", itemid[0], itemid[1], itemid[2], itemid[3], timemarklen, bactive);

	if(0 == itemid[1] || 0 == itemid[0]) {
		metpid = 0;
	}
	else {
		metpid = ((unsigned short)(itemid[1]-1)<<3) + 1;
		for(fnmask=1; fnmask != 0; fnmask<<=1,metpid++) {
			if(fnmask & itemid[0]) break;
		}
		if(metpid > MAX_METP) return 0;
	}
	
	DebugPrint(0, "run query curve stp 2\n");

	QueryDuid[0] = itemid[0];
	QueryDuid[1] = itemid[1];
	QueryDuid[2] = itemid[2];
	QueryDuid[3] = itemid[3];

	//DebugPrint(0, "umet=%d, metpid=%d\n", umet, metpid);
	pconfig = DbaseConfig;
	
	for(; pconfig->cids!=0; pconfig++) {
		if(itemid[3] == pconfig->grpid && (itemid[2]&pconfig->cids) != 0) break;
	}
	
	DebugPrint(0, "run query curve stp 3\n");

	if(0 == pconfig->cids) return -1;
	for(fnmask=1; fnmask!=0; fnmask<<=1) {
		if(fnmask&pconfig->cids) break;
	}

	DebugPrint(0, "run query curve stp 4\n");
	if(0 == fnmask) return -1;
	
	DebugPrint(0, "run query curve stp 5\n");

	for(offset=0; fnmask!=0; fnmask<<=1,offset++) {
		if(fnmask&itemid[2]) break;
	}
	if(0 == fnmask) return -1;

	//DebugPrint(0, "start qry(dbid=%d, offset=%d, col=%d)...\n", pconfig->dbid, offset, pconfig->colen);

	DbaseReadLock();

	SpConfig = SpecialCopyConfig;
	SpConfigNum = SIZE_SPECIALCOPYCONFIG;

	if(bactive) {
		timemarklen = 1;
		//multir = (unsigned char)(bactive >> 1);
	}
	else {
		;//multir = 0;
	}
	QueryStep = 0;

	switch(pconfig->type) {
	case DBSAVE_DAY:
	case DBSAVE_CPYDAY:
		if(bactive) {
			//QueryDbTime.u = GetSndTime(QueryDuid);
			//QueryDbTime.s.tick = 0;
		}
		else {
			if(timemarklen < 3) {
				timemarklen = -1;
				goto mark_end;
			}
			timemarklen = 3;
			QueryDbTime.s.day = timemark[0];
			QueryDbTime.s.month = timemark[1];
			QueryDbTime.s.year = timemark[2];
			BcdToHex(&QueryDbTime.s.day, 3);
			QueryDbTime.s.tick = 0;
		}
		DebugPrint(0,"get dbase day fun\n");
		pfunc = QueryDbaseDay;
		break;

	case DBSAVE_MONTH:
		if(bactive) {
			//QueryDbTime.u = GetSndTime(QueryDuid);
			//QueryDbTime.s.day = 1;
			//QueryDbTime.s.tick = 0;
		}
		else {
			if(timemarklen < 2) {
				timemarklen = -1;
				goto mark_end;
			}
			timemarklen = 2;
			QueryDbTime.s.day = 1;
			QueryDbTime.s.month = timemark[0];
			QueryDbTime.s.year = timemark[1];
			BcdToHex(&QueryDbTime.s.month, 2);
			QueryDbTime.s.tick = 0;
		}
		DebugPrint(0,"get dbase mon fun\n");
		pfunc = QueryDbaseMonth;
		break;

	case DBSAVE_CURVE:
		if(bactive) {
			//QueryDbTime.u = GetSndTime(QueryDuid);
			//QueryDataNum = 255;
			//switch(multir) {
			//case 1: QueryFrez = CURVE_FREZ; break;
			//case 2: QueryFrez = CURVE_FREZ+1; break;
			//case 3: QueryFrez = CURVE_FREZ+1; break;
			//case 4: QueryFrez = CURVE_FREZ+2; break;
			//default: QueryFrez = CURVE_FREZ; break;
			//}
			//if(QueryFrez > DBFREZ_1HOUR) QueryFrez = DBFREZ_1HOUR;
		}
		else {
			if(timemarklen < 7) {
				timemarklen = -1;
				goto mark_end;
			}
			timemarklen = 7;
			{
				sysclock_t clock;

				clock.minute = timemark[0];
				clock.hour = timemark[1];
				clock.day = timemark[2];
				clock.month = timemark[3];
				clock.year = timemark[4];
				BcdToHex(&clock.year, 5);
				SYSCLOCK_DBTIME(&clock, QueryDbTime);
			}
			QueryFrez = timemark[5];
			QueryDataNum = timemark[6];
		}
		DebugPrint(0,"get dbase curve fun\n");
		pfunc = QueryDbaseCurve;
		break;

	default:
		ErrorLog("invalid db type(%d)\n", pconfig->type);
		timemarklen = -1;
		goto mark_end;
	}

	QueryMetpid = 0;

	switch(pconfig->attr) {
	case DBATTR_METP:
		DebugPrint(0,"metpid (%d)\n",metpid);

		if(metpid == 0 || metpid > MAX_METP) goto mark_end;
		
		metersn = MetpMap[metpid-1].metid+1;
		DebugPrint(0,"metersn (%d)\n",metersn);
		
		if(metersn == 0 || metersn > MAX_METER) goto mark_end;
		dbid = pconfig->dbid + metersn - 1;
		break;

	case DBATTR_TERM:
		if(metpid != 0) goto mark_end;
		dbid = pconfig->dbid;
		break;
		
	default:
		ErrorLog("invalid db attr(%d)\n", pconfig->attr);
		goto mark_end;
	}
	DebugPrint(0, "run query curve stp 6\n");

	rtn = (*pfunc)(dbid, &sendbuffer, pconfig, offset);
	if((rtn) && NULL == sendbuffer) {
		DbaseReadUnlock();
		DebugPrint(0, "run query curve stp 7\n");

		return 0;
	}

mark_end:
	DbaseReadUnlock();
	DebugPrint(0, "run query curve stp 8\n");

	return timemarklen;
}

