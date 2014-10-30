/**
* operation.c -- 参数操作
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-7
* 最后修改时间: 2009-5-8
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plat_include/debug.h"
#include "plat_include/syslock.h"
#include "app_include/param/capconf.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
#include "app_include/monitor/runstate.h"
#include "app_include/param/param_init.h"

typedef int (*operation_fn)(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen);
typedef int (*specialop_fn)(int flag, unsigned short metpid, param_specialop_t *option);

#define PRATTR_TERM		0   //终端参数
#define PRATTR_METP		1    //测量点参数
#define PRATTR_TGRP		2    //总加组参数
#define PRATTR_TASK1	3    //1类任务参数
#define PRATTR_TASK2	4    //2类任务参数
#define PRATTR_SWP		5   //轮次参数
#define PRATTR_ANASIG	6  //直流模拟量参数

typedef struct {
	unsigned char special;
	unsigned char attr;
	unsigned char datalen;
	unsigned int saveflag;
	operation_fn pfunc;
} paraitem_t;

typedef struct {
	unsigned bend;
	unsigned char grpid;
	unsigned char num;
	const paraitem_t *pitemlist;
} paragrp_t;
/*
static int ParaOperationInvalid(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	return POERR_INVALID;
}

static int ParaOperationFatal(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	return POERR_FATAL;
}*/

int ParaOperationZero(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		memset(buf, 0, *actlen);
	}

	return 0;
}

int ParaOperationZero2(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		buf[0] = 0;
		*actlen = 1;
	}
	else {
		int alen;

		alen = (int)buf[0]&0xff;
		alen *= *actlen;
		*actlen = alen + 1;
		if(*actlen > len) return POERR_FATAL;
	}

	return 0;
}

static const paraitem_t ItemListF1[] = {
	/*F1 */ {0, PRATTR_TERM, 6, SAVEFLAG_TERM, ParaOperationF1},
	/*F2 */ {0, PRATTR_TERM, 1, SAVEFLAG_TERM, ParaOperationF2},
	/*F3 */ {0, PRATTR_TERM, 28, SAVEFLAG_TERM, ParaOperationF3},
	/*F4 */ {0, PRATTR_TERM, 16, SAVEFLAG_TERM, ParaOperationF4},
	/*F5 */ {0, PRATTR_TERM, 3, SAVEFLAG_TERM, ParaOperationF5},
	/*F6 */ {0, PRATTR_TERM, 16, SAVEFLAG_TERM, ParaOperationZero},
	/*F7 */ {0, PRATTR_TERM, 24, SAVEFLAG_TERM, ParaOperationF7},
	/*F8 */ {0, PRATTR_TERM, 8, SAVEFLAG_TERM, ParaOperationF8},
};

static const paraitem_t ItemListF9[] = {
	/*F9  */ {0, PRATTR_TERM, 16, SAVEFLAG_TERM, ParaOperationZero},
	/*F10 */ {1, PRATTR_TERM, 2, SAVEFLAG_METER, (operation_fn)ParaOperationF10},
	/*F11 */ {0, PRATTR_TERM, 1, SAVEFLAG_TERM, ParaOperationZero},
	/*F12 */ {0, PRATTR_TERM, 2, SAVEFLAG_TERM, ParaOperationZero},//ParaOperationF12},
	/*F13 */ {0, PRATTR_TERM, 1, SAVEFLAG_TERM, ParaOperationZero},
	/*F14 */ {0, PRATTR_TERM, 1, SAVEFLAG_TERM, ParaOperationZero},
	/*F15 */ {0, PRATTR_TERM, 1, SAVEFLAG_TERM, ParaOperationZero},
	/*F16 */ {0, PRATTR_TERM, 64, SAVEFLAG_TERM, ParaOperationF16},
};

static const paraitem_t ItemListF25[] = {
	/*F25 */ {0, PRATTR_METP, 11, SAVEFLAG_CMETP, ParaOperationZero},//ParaOperationF25},
	/*F26 */ {0, PRATTR_METP, 57, SAVEFLAG_CMETP,ParaOperationZero},// ParaOperationF26},
	/*F27 */ {0, PRATTR_METP, 24, SAVEFLAG_CMETP, ParaOperationZero},
	/*F28 */ {0, PRATTR_METP, 4, SAVEFLAG_CMETP, ParaOperationZero},
	/*F29 */ {0, PRATTR_METP, 12, SAVEFLAG_METP, ParaOperationZero},
	/*F30 */ {0, PRATTR_METP, 1, SAVEFLAG_METP, ParaOperationZero},
	/*F31 */ {0, PRATTR_METP, 1, SAVEFLAG_TERM, ParaOperationZero},
};

static const paraitem_t ItemListF33[] = {
	/*F33 */ {1, PRATTR_TERM, 1, SAVEFLAG_MIX, ParaOperationZero},//(operation_fn)ParaOperationF33},
	/*F34 */ {0, PRATTR_TERM, 1, SAVEFLAG_MIX,ParaOperationZero},
	/*F35 */ {0, PRATTR_TERM, 1, SAVEFLAG_MIX, ParaOperationZero},
	/*F36 */ {0, PRATTR_TERM, 4, SAVEFLAG_MIX, ParaOperationZero},//ParaOperationF36},
	/*F37 */ {0, PRATTR_TERM, 7, SAVEFLAG_MIX, ParaOperationZero},
	/*F38 */ {1, PRATTR_TERM, 2, SAVEFLAG_DATAUSE, ParaOperationZero},//(operation_fn)ParaOperationF38},
	/*F39 */ {1, PRATTR_TERM, 2, SAVEFLAG_DATAUSE, ParaOperationZero},//(operation_fn)ParaOperationF39},
};
static const paraitem_t ItemListF57[] = {
	/*F57 */ {0, PRATTR_TERM, 3, SAVEFLAG_TERM, ParaOperationZero},
	/*F58 */ {0, PRATTR_TERM, 1, SAVEFLAG_TERM, ParaOperationZero},
	/*F59 */ {0, PRATTR_TERM, 4, SAVEFLAG_MIX, ParaOperationZero},//ParaOperationF59},
	/*F60 */ {0, PRATTR_TERM, 80, SAVEFLAG_MIX, ParaOperationZero},
	/*F61 */ {0, PRATTR_TERM, 1, SAVEFLAG_TERM, ParaOperationZero},
	/*F62 */ {0, PRATTR_METP, 5, SAVEFLAG_METP, ParaOperationZero},	
};

static const paraitem_t ItemListF81[] = {
	/*F81*/ {0, PRATTR_TERM, 1, 0, ParaOperationZero},
	/*F82 */ {0, PRATTR_TERM, 1, 0, ParaOperationZero},
	/*F83 */ {0, PRATTR_TERM, 1, 0, ParaOperationZero},
	/*F84 */ {0, PRATTR_TERM, 1, 0, ParaOperationZero},
	/*F85 */ {0, PRATTR_TERM, 4, 0, ParaOperationF85},
	/*F86 */ {0, PRATTR_TERM, 3, 0, ParaOperationZero},
	/*F87 */ {0, PRATTR_TERM, 1, 0, ParaOperationZero},
};

static const paraitem_t ItemListF65[] = {
	/*F65 */ {0, PRATTR_TASK1, 9, SAVEFLAG_TASK, ParaOperationF65},
	/*F66 */ {0, PRATTR_TASK2, 9, SAVEFLAG_TASK, ParaOperationZero},//ParaOperationF66},
	/*F67 */ {0, PRATTR_TASK1, 1, SAVEFLAG_TASK, ParaOperationF67},
	/*F68 */ {0, PRATTR_TASK2, 1, SAVEFLAG_TASK, ParaOperationZero},//ParaOperationF68},
};
static const paraitem_t ItemListF161[]={
	/*F161*/ {0, PRATTR_TERM, 32, 0, ParaOperationF161}
};

static const paraitem_t ItemListF169[] = {
	/*F169*/ {0, PRATTR_TERM, 1, 0, ParaOperationF169},
	/*F170*/ {0, PRATTR_TERM, 1, 0, ParaOperationF170},
///	/*F171*/ {0, PRATTR_TERM, 1, 0, ParaOperationF171},
///	/*F172*/ {0, PRATTR_TERM, 5, 0, ParaOperationF172},
///	/*F173*/ {0, PRATTR_TERM, 1, 0, ParaOperationF173},
///	/*F174*/ {0, PRATTR_TERM, 1, 0, ParaOperationF174},
///	/*F175*/ {0, PRATTR_TERM, 1, 0, ParaOperationF175},
      /*F176*/  {0, PRATTR_TERM, 1, 0, ParaOperationF171},
};

static const paragrp_t GroupList[] = {
	{0, 0, sizeof(ItemListF1)/sizeof(paraitem_t), ItemListF1},
	{0, 1, sizeof(ItemListF9)/sizeof(paraitem_t), ItemListF9},
	{0, 3, sizeof(ItemListF25)/sizeof(paraitem_t), ItemListF25},
	{0, 4, sizeof(ItemListF33)/sizeof(paraitem_t), ItemListF33},
	{0, 7, sizeof(ItemListF57)/sizeof(paraitem_t), ItemListF57},
	{0, 8, sizeof(ItemListF65)/sizeof(paraitem_t), ItemListF65},
	{0, 10, sizeof(ItemListF81)/sizeof(paraitem_t), ItemListF81},
	{0, 20,sizeof(ItemListF161)/sizeof(paraitem_t), ItemListF161},
	{0, 21, sizeof(ItemListF169)/sizeof(paraitem_t), ItemListF169},
	{1, 0xff, 0, NULL},
};

/**
* @brief 写入参数
* @param buf 输入缓存区指针
* @param len 缓存区长度
* @param pactlen 有效数据长度(由函数返回)
* @return 0成功, 否则失败(参看POERR_XXX宏定义)
*/
int WriteParam(unsigned char *buf, int len, int *pactlen)
{
	const paragrp_t *pgrp = GroupList;
	const paraitem_t *pitem;
	unsigned short metpid;
	unsigned char pns, pnmask, fns, fnmask;
	int actlen, berror, idx, rtn, itemlen;

	AssertLogReturn(len<=4, POERR_INVALID, "invalid len(%d)\n", len);

	pns = buf[0];
	if(0 == buf[1]) metpid = 0;
	else metpid = ((unsigned short)(buf[1]-1)<<3) + 1;

	fns = buf[3];
	while(0 == pgrp->bend) {
		if(fns == pgrp->grpid) break;
		pgrp++;
	}
	if(pgrp->bend) return POERR_FATAL;
	fns = buf[2];

	buf += 4;
	actlen = 4;
	len -= 4;
	berror = 0;

	if(0 == metpid) pnmask = 0x80;
	else pnmask = 1;
	for(; pnmask!=0; pnmask<<=1,metpid++) {
		if(metpid && (pns&pnmask) == 0) continue;

		pitem = pgrp->pitemlist;
		for(idx=0,fnmask=1; idx<pgrp->num; idx++,fnmask<<=1,pitem++) {
			if(0 == (fnmask&fns)) continue;

			switch(pitem->attr) {
			case PRATTR_TERM:
				if(0 != metpid) return POERR_FATAL;
				break;
			case PRATTR_METP:
				if(0 == metpid || metpid > MAX_METP) return POERR_FATAL;
				break;
			case PRATTR_TASK1:
				if(0 == metpid || metpid > MAX_DTASK_CLS1) return POERR_FATAL;
				break;
			default:
				ErrorLog("invalid attr(%d)\n", pitem->attr);
				return POERR_FATAL;
			}

			if(((int)pitem->datalen&0xff) > len) return POERR_FATAL;

			if(pitem->special) {
				specialop_fn pfunc = (specialop_fn)pitem->pfunc;
				param_specialop_t specialop;

				specialop.pn = metpid;
				specialop.rbuf = buf;
				specialop.rlen = len;
				specialop.ractlen = (int)pitem->datalen&0xff;
				specialop.wbuf = NULL;
				specialop.wlen = 0;
				specialop.wactlen = 0;

				rtn = (*pfunc)(1, metpid, &specialop);
				itemlen = specialop.ractlen;
				if(POERR_FATAL == rtn) return POERR_FATAL;
				else if(POERR_OK != rtn) berror = 1;
				else SetSaveParamFlag(pitem->saveflag);
			}
			else {
				itemlen = (int)pitem->datalen&0xff;
				rtn = (*(pitem->pfunc))(1, metpid, buf, len, &itemlen);
				if(POERR_FATAL == rtn) return POERR_FATAL;
				else if(POERR_OK != rtn) berror = 1;
				else SetSaveParamFlag(pitem->saveflag);
			}

			buf += itemlen;
			actlen += itemlen;
			len -= itemlen;
		}
	}

	*pactlen = actlen;
	if(berror) return POERR_INVALID;
	else return POERR_OK;
}

/**
* @brief 读取参数
* @param buf 输出缓存区指针
* @param len 缓存区长度
* @param pactlen 有效数据长度(由函数返回)
* @param readinfo 输入缓存区信息
* @return 0成功, 否则失败(参看POERR_XXX宏定义)
*/
int ReadParam(unsigned char *buf, int len, int *pactlen, para_readinfo_t *readinfo)
{
	const paragrp_t *pgrp = GroupList;
	unsigned short metpid;
	unsigned char pns, pnmask, fns, *preadfns;
	int rbuflen, actlen;
	unsigned char *prbuf;

	*pactlen = 0;

	AssertLogReturn(len<4, POERR_INVALID, "invalid len(%d)\n", len);

	for(rbuflen=0; rbuflen<4; rbuflen++) buf[rbuflen] = readinfo->buf[rbuflen];
	pns = buf[0];
	if(0 == buf[1]) metpid = 0;
	else metpid = ((unsigned short)(buf[1]-1)<<3) + 1;

	fns = buf[3];
	while(0 == pgrp->bend) {
		if(fns == pgrp->grpid) break;
		pgrp++;
	}
	if(pgrp->bend) return POERR_INVALID;
	fns = buf[2];

	prbuf = readinfo->buf + 4;
	rbuflen = readinfo->len - 4;
	readinfo->actlen = 4;
	buf[2] = 0;
	preadfns = &buf[2];
	buf += 4;
	len -= 4;
	actlen = 4;

	if(0 == metpid) pnmask = 0x80;
	else pnmask = 1;
	for(; pnmask!=0; pnmask<<=1,metpid++) {
		unsigned char fnmask;
		int idx, rtn, itemlen;
		const paraitem_t *pitem;

		if(metpid && (pns&pnmask) == 0) continue;

		pitem = pgrp->pitemlist;
		for(idx=0,fnmask=1; idx<pgrp->num; idx++,fnmask<<=1,pitem++) {
			if(0 == (fnmask&fns)) continue;

			switch(pitem->attr) {
			case PRATTR_TERM:
				if(0 != metpid) return POERR_FATAL;
				break;
			case PRATTR_METP:
				if(0 == metpid || metpid > MAX_METP) return POERR_FATAL;
				break;
			case PRATTR_TASK1:
				if(0 == metpid || metpid > MAX_DTASK_CLS1) return POERR_FATAL;
				break;
			default:
				ErrorLog("invalid attr(%d)\n", pitem->attr);
				return POERR_FATAL;
			}

			if(((int)pitem->datalen&0xff) > len) return POERR_FATAL;

			if(pitem->special) {
				specialop_fn pfunc = (specialop_fn)pitem->pfunc;
				param_specialop_t specialop;

				specialop.pn = metpid;
				specialop.wbuf = buf;
				specialop.wlen = len;
				specialop.wactlen = (int)pitem->datalen&0xff;

				specialop.rbuf = prbuf;
				specialop.rlen = rbuflen;
				specialop.ractlen = 0;

				rtn = (*pfunc)(0, metpid, &specialop);
				itemlen = specialop.wactlen;
				if(POERR_FATAL == rtn) return POERR_FATAL;
				else if(POERR_OK == rtn) *preadfns |= fnmask;
				else itemlen = 0;

				prbuf += specialop.ractlen;
				rbuflen -= specialop.ractlen;
				readinfo->actlen += specialop.ractlen;
				
			}
			else {
				itemlen = (int)pitem->datalen&0xff;
				rtn = (*(pitem->pfunc))(0, metpid, buf, len, &itemlen);
				if(POERR_FATAL == rtn) return POERR_FATAL;
				else if(POERR_OK == rtn) *preadfns |= fnmask;
				else itemlen = 0;
			}

			if(itemlen > 0) {
				buf += itemlen;
				len -= itemlen;
				actlen += itemlen;
			}
		}
	}

	if(0 == *preadfns) return POERR_INVALID;  //no data
	*pactlen = actlen;

	return POERR_OK;
}

static unsigned int SaveFlag = 0;

/**
* @brief 清除参数储存标志
*/
void ClearSaveParamFlag(void)
{
	SaveFlag = 0;
}

/**
* @brief 设置参数储存标志
* @param flag 标志位
*/
void SetSaveParamFlag(unsigned int flag)
{
	SaveFlag |= flag;
}

extern void ClearRunState(void);
extern void ClearMdbSave(void);
extern void SaveMdb(void);
extern void DbaseFormat(void);
///extern void ClearPulse();

/**
* @brief 清除所有数据
*/
void ClearAllData(void)
{
#ifdef ALARM_MODULE
	ClearRunState();
	ClearMdbSave();
	DbaseFormat();
#endif
///	ClearPulse();
}

/**
* @brief 保存清除之后的数据
*/
void SaveCleanData(void)
{
#ifdef ALARM_MODULE
	SaveRunState();
	SaveMdb();
#endif
}


/**
* @brief 清除所有参数(恢复出厂)
*/
void ClearAllParam(void)
{
	//para_rmflash_bin(PARA_FILEINDEX_COMB);
	LoadParaComb2();
	para_rmflash_bin(PARA_FILEINDEX_METER);
	para_rmflash_bin(PARA_FILEINDEX_METP);
	para_rmflash_bin(PARA_FILEINDEX_DATAUSE1);
	para_rmflash_bin(PARA_FILEINDEX_DATAUSE2);
	
	return;
}

/**
* @brief 清除所有参数(除通信参外)
*/
void ClearFactoryParam(void)
{

	ClearAllParam();
	
	return;
}
