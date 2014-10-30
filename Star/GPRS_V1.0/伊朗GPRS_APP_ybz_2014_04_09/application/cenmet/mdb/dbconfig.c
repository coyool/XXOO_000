/**
* dbconfig.c -- 历史数据库配置
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-12
* 最后修改时间: 2009-5-12
*/
//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "app_include/param/capconf.h"
#include "plat_include/timeal.h"
#include "app_include/lib/dbtime.h"
#include "app_include/cenmet/dbconfig.h"
#include "app_include/cenmet/mdbfrez.h"
#include "app_include/cenmet/curveday.h"
#include "app_include/cenmet/curvemon.h"
#include "app_include/cenmet/curvermd.h"
#include "plat_include/flashsave.h"

typedef struct {
	unsigned short dbid_base;
	unsigned short dbid_end;
	int itemsize;
	unsigned char frztype;
} dbinfo_t;
static const dbinfo_t DbaseInfo[] = {
	{DBID_MET_DAY(0), DBID_MET_DAY(MAX_CENMETP), sizeof(db_metday_t), DB_SAVE_DAY},//日数据
	{DBID_MET_CPYDAY(0), DBID_MET_CPYDAY(MAX_CENMETP), sizeof(db_metrmd_t),DB_SAVE_CPYDAY},//抄表日数据
	{DBID_MET_MONTH(0), DBID_MET_MONTH(MAX_CENMETP), sizeof(db_metmon_t),DB_SAVE_MONTH},//月数据
	{DBID_TERMSTIC_DAY, DBID_TERMSTIC_DAY+1, sizeof(db_termday_t), DB_SAVE_TERMDAY},//终端日数据
	{DBID_TERMSTIC_MONTH, DBID_TERMSTIC_MONTH+1, sizeof(db_termmon_t),DB_SAVE_TERMMON},//终端月数据
	{DBID_MET_CURVE(0), DBID_MET_CURVE(MAX_CENMETP),sizeof(db_metcurve_t),DB_SAVE_CURVE},//测量点曲线
};
#define SIZE_DBASEINFO		(sizeof(DbaseInfo)/sizeof(DbaseInfo[0]))

/**
* @brief 获取数据库的数据项大小
* @param dbid 数据库ID
* @return 成功返回数据项大小, 否则返回0
*/
int DbaseItemSize(unsigned short dbid)
{
	int i;

	for(i=0; i<SIZE_DBASEINFO; i++) {
		if(dbid >= DbaseInfo[i].dbid_base && dbid < DbaseInfo[i].dbid_end)
			return DbaseInfo[i].itemsize;
	}

	return 0;
}

int DbaseItemType(unsigned short dbid,unsigned char *frztype)
{
	int i;

	for(i=0; i<SIZE_DBASEINFO; i++) {
		if(dbid >= DbaseInfo[i].dbid_base && dbid < DbaseInfo[i].dbid_end)
		{
			*frztype = DbaseInfo[i].frztype;
			return 0;
		}
	}

	return 1;
}

/*
rt 0 ok 1 failed
*/
int DbaseGetMetid(unsigned short dbid,unsigned short *metid)
{
    unsigned short i;
    int rt;

    *metid = MAX_CENMETP+1;
	for(i=0; i<SIZE_DBASEINFO; i++) {
		if(dbid >= DbaseInfo[i].dbid_base && dbid < DbaseInfo[i].dbid_end) {
			*metid = dbid - DbaseInfo[i].dbid_base;
			rt = 0;
		}
	}

    if(*metid>MAX_CENMETP) rt = 1;

    return rt;
}


typedef struct {
	unsigned int paramid;
	int itemsize;
	const char *filename;
} infoparam_t;

//static const infoparam_t DbParaInfo[] = {
//{FILEINDEX_TERM,"daymet"},
//
//}

/**
* @brief 获取一个文件的储存时间,表号，文件类型
* @param filename 数据库文件名
* @param 返回的数据库储存时间
* @return 月日期返回2, 天日期返回1, 非法日期返回0
*/
int AnalysisFileName(char *filename,unsigned char *metid,dbtime_t *dbtime)
{
#if 0
	const char *ps, *pe;
	int i, skip,type;
	unsigned char *puc = &(dbtime->year);
	unsigned char date;

	pe = filename;
	for(i=0; i<SIZE_DBASEINFO; i++)
	{
		if(NULL != FindStr(filename,DbaseInfo[i].filename)  break;
	}

	if(i==SIZE_DBASEINFO) goto markparam;
	else type = (i+20);
	
	if('m' == *pe) {
		skip = 2; //month
		dbtime->day = 1;
	}
	else {
		skip = 3;  //day
	}

	ps = pe;
	while(0 != *ps) {
		if('@' == *ps) break;
		ps++;
	}
	if(0 == *ps) return 0;
//get metid
	ps -= 2;
	*metid = 0;
	if((*ps>='0')&&(*ps<='9')) *metid = (*ps-'0')*10;
	ps++;
	if((*ps>='0')&&(*ps<='9')) *metid += (*ps-'0');
	ps++;
	
	ps++;
	pe = ps;
	while(0 != *pe) {
		if('.' == *pe) break;
		pe++;
	}
	
	if(0 == *pe) return 0;
//get dbtime	
	if((int)(pe-ps) != (skip*2)) return 0;

	for(i=0; i<skip; i++,ps+=2) {
		date = (ps[0]-'0')*10;
		date += ps[1] - '0';
		*puc++ = date;
	}

	return type;
	
markparam:
#endif
	return 0;
}

//F1~F8
static const dbsonconfig_t cons_grp1[] = {
	/*F1  */{0x03, 85, DB_OFFSET(db_metday_t, enepa[0])},
	/*F2  */{0x03, 85, DB_OFFSET(db_metday_t, enena[0])},
	/*F3  */{0x03, 70, DB_OFFSET(db_metday_t, dmnpa[0])},
	/*F4  */{0x03, 70, DB_OFFSET(db_metday_t, dmnna[0])},
	///*F5  */{0x02, 20, DB_OFFSET(db_metday_t, enepa_day[0])},
	///*F6  */{0x02, 20, DB_OFFSET(db_metday_t, enepi_day[0])},
	///*F7  */{0x02, 20, DB_OFFSET(db_metday_t, enena_day[0])},
	///*F8  */{0x02, 20, DB_OFFSET(db_metday_t, eneni_day[0])},
};

//F9~F12
static const dbsonconfig_t cons_grp2[] = {
	/*F9  */{0x03, 85, DB_OFFSET(db_metrmd_t, enepa[0])},
	/*F10 */{0x03, 85, DB_OFFSET(db_metrmd_t, enena[0])},
	/*F11 */{0x03, 70, DB_OFFSET(db_metrmd_t, dmnpa[0])},
	/*F12 */{0x03, 70, DB_OFFSET(db_metrmd_t, dmnna[0])},
};

//F17~F24
static const dbsonconfig_t cons_grp3[] = {
	/*F17 */{0x03, 85, DB_OFFSET(db_metmon_t, enepa[0])},
	/*F18 */{0x03, 85, DB_OFFSET(db_metmon_t, enena[0])},
	/*F19 */{0x03, 70, DB_OFFSET(db_metmon_t, dmnpa[0])},
	/*F20 */{0x03, 70, DB_OFFSET(db_metmon_t, dmnna[0])},
	///*F21 */{0x02, 20, DB_OFFSET(db_metmon_t, enepa_day[0])},
	///*F22 */{0x02, 20, DB_OFFSET(db_metmon_t, enepi_day[0])},
	///*F23 */{0x02, 20, DB_OFFSET(db_metmon_t, enena_day[0])},
	///*F24 */{0x02, 20, DB_OFFSET(db_metmon_t, eneni_day[0])},
};

//F49~F50
static const dbsonconfig_t cons_grp7a[] = {
	/*F49 */{0x00, 4, DB_OFFSET(db_termday_t, pwr_time[0])},
	/*F50 */{0x00, 4, DB_OFFSET(db_termday_t, sw_time[0])},
};

//F51~F52
static const dbsonconfig_t cons_grp7b[] = {
	/*F51 */{0x00, 4, DB_OFFSET(db_termmon_t, pwr_time[0])},
	/*F52 */{0x00, 4, DB_OFFSET(db_termmon_t, sw_time[0])},
};

//F53
static const dbsonconfig_t cons_grp7c[] = {
	/*F53 */{0x00, 4, DB_OFFSET(db_termday_t, comm_bytes[0])},
};

//F54
static const dbsonconfig_t cons_grp7d[] = {
	/*F54 */{0x00, 4, DB_OFFSET(db_termmon_t, comm_bytes[0])},
};

//F81~F88
static const dbsonconfig_t cons_grp11[] = {
	/*F81 */{0x00, 3, DB_OFFSET(db_metcurve_t, pwra[0])},
	/*F82 */{0x00, 3, DB_OFFSET(db_metcurve_t, pwra[3])},
	/*F83 */{0x00, 3, DB_OFFSET(db_metcurve_t, pwra[6])},
	/*F84 */{0x00, 3, DB_OFFSET(db_metcurve_t, pwra[9])},
	/*F85 */{0x00, 3, DB_OFFSET(db_metcurve_t, pwri[0])},
	/*F86 */{0x00, 3, DB_OFFSET(db_metcurve_t, pwri[3])},
	/*F87 */{0x00, 3, DB_OFFSET(db_metcurve_t, pwri[6])},
	/*F88 */{0x00, 3, DB_OFFSET(db_metcurve_t, pwri[9])},
};

//F89~F95
static const dbsonconfig_t cons_grp12[] = {
	/*F89 */{0x00, 2, DB_OFFSET(db_metcurve_t, vol[0])},
	/*F90 */{0x00, 2, DB_OFFSET(db_metcurve_t, vol[2])},
	/*F91 */{0x00, 2, DB_OFFSET(db_metcurve_t, vol[4])},
	/*F92 */{0x00, 3, DB_OFFSET(db_metcurve_t, amp[0])},
	/*F93 */{0x00, 3, DB_OFFSET(db_metcurve_t, amp[3])},
	/*F94 */{0x00, 3, DB_OFFSET(db_metcurve_t, amp[6])},
	/*F95 */{0x00, 3, DB_OFFSET(db_metcurve_t, amp[9])},
};

//F97~F104
static const dbsonconfig_t cons_grp13[] = {
	/*F97 */{0x00, 4, DB_OFFSET(db_metcurve_t, uenepa[0])},
	/*F98 */{0x00, 4, DB_OFFSET(db_metcurve_t, uenepi[0])},
	/*F99 */{0x00, 4, DB_OFFSET(db_metcurve_t, uenena[0])},
	/*F100*/{0x00, 4, DB_OFFSET(db_metcurve_t, ueneni[0])},
	/*F101*/{0x00, 4, DB_OFFSET(db_metcurve_t, enepa[0])},
	/*F102*/{0x00, 4, DB_OFFSET(db_metcurve_t, enepi[0])},
	/*F103*/{0x00, 4, DB_OFFSET(db_metcurve_t, enena[0])},
	/*F104*/{0x00, 4, DB_OFFSET(db_metcurve_t, eneni[0])},
};

//F105~F110
static const dbsonconfig_t cons_grp14[] = {
	/*F105*/{0x00, 2, DB_OFFSET(db_metcurve_t, pwrf[0])},
	/*F106*/{0x00, 2, DB_OFFSET(db_metcurve_t, pwrf[2])},
	/*F107*/{0x00, 2, DB_OFFSET(db_metcurve_t, pwrf[4])},
	/*F108*/{0x00, 2, DB_OFFSET(db_metcurve_t, pwrf[6])},
	///*F109*/{0x00, 6, DB_OFFSET(db_metcurve_t, vol_arc[0])},
	///*F110*/{0x00, 6, DB_OFFSET(db_metcurve_t, amp_arc[0])},
};

//F145~F148
static const dbsonconfig_t cons_grp19[] = {
	/*F145*/{0x00, 4, DB_OFFSET(db_metcurve_t, enepi1[0])},
	/*F146*/{0x00, 4, DB_OFFSET(db_metcurve_t, enepi4[0])},
	/*F147*/{0x00, 4, DB_OFFSET(db_metcurve_t, eneni2[0])},
	/*F148*/{0x00, 4, DB_OFFSET(db_metcurve_t, eneni3[0])},
};

//F153~F156
static const dbsonconfig_t cons_grp20a[] = {
	/*F153*/{0x01,15, DB_OFFSET(db_metday_t, enepa_abc[0])},
	/*F154*/{0x01,12, DB_OFFSET(db_metday_t, enepi_abc[0])},
	/*F155*/{0x01,15, DB_OFFSET(db_metday_t, enena_abc[0])},
	/*F156*/{0x01,12, DB_OFFSET(db_metday_t, eneni_abc[0])},
};

//F157~F160
static const dbsonconfig_t cons_grp20b[] = {
	/*F157*/{0x01,15, DB_OFFSET(db_metmon_t, enepa_abc[0])},
	/*F158*/{0x01,12, DB_OFFSET(db_metmon_t, enepi_abc[0])},
	/*F159*/{0x01,15, DB_OFFSET(db_metmon_t, enena_abc[0])},
	/*F160*/{0x01,12, DB_OFFSET(db_metmon_t, eneni_abc[0])},
};

//F161~F168
static const dbsonconfig_t cons_grp21[] = {
	/*F161*/{0x03,25, DB_OFFSET(db_metday_t, enepa[0])},
	/*F162*/{0x03,20, DB_OFFSET(db_metday_t, enepi[0])},
	/*F163*/{0x03,25, DB_OFFSET(db_metday_t, enena[0])},
	/*F164*/{0x03,20, DB_OFFSET(db_metday_t, eneni[0])},
	/*F165*/{0x03,20, DB_OFFSET(db_metday_t, enepi1[0])},
	/*F166*/{0x03,20, DB_OFFSET(db_metday_t, eneni2[0])},
	/*F167*/{0x03,20, DB_OFFSET(db_metday_t, eneni3[0])},
	/*F168*/{0x03,20, DB_OFFSET(db_metday_t, enepi4[0])},
};

//F169~F176
static const dbsonconfig_t cons_grp22[] = {
	/*F169*/{0x03,25, DB_OFFSET(db_metrmd_t, enepa[0])},
	/*F170*/{0x03,20, DB_OFFSET(db_metrmd_t, enepi[0])},
	/*F171*/{0x03,25, DB_OFFSET(db_metrmd_t, enena[0])},
	/*F172*/{0x03,20, DB_OFFSET(db_metrmd_t, eneni[0])},
	/*F173*/{0x03,20, DB_OFFSET(db_metrmd_t, enepi1[0])},
	/*F174*/{0x03,20, DB_OFFSET(db_metrmd_t, eneni2[0])},
	/*F175*/{0x03,20, DB_OFFSET(db_metrmd_t, eneni3[0])},
	/*F176*/{0x03,20, DB_OFFSET(db_metrmd_t, enepi4[0])},
};

//F177~F184
static const dbsonconfig_t cons_grp23[] = {
	/*F177*/{0x03,25, DB_OFFSET(db_metmon_t, enepa[0])},
	/*F178*/{0x03,20, DB_OFFSET(db_metmon_t, enepi[0])},
	/*F179*/{0x03,25, DB_OFFSET(db_metmon_t, enena[0])},
	/*F180*/{0x03,20, DB_OFFSET(db_metmon_t, eneni[0])},
	/*F181*/{0x03,20, DB_OFFSET(db_metmon_t, enepi1[0])},
	/*F182*/{0x03,20, DB_OFFSET(db_metmon_t, eneni2[0])},
	/*F183*/{0x03,20, DB_OFFSET(db_metmon_t, eneni3[0])},
	/*F184*/{0x03,20, DB_OFFSET(db_metmon_t, enepi4[0])},
};

//F185~F188
static const dbsonconfig_t cons_grp24a[] = {
	/*F185*/{0x07,35, DB_OFFSET(db_metday_t, dmnpa[0])},
	/*F186*/{0x07,35, DB_OFFSET(db_metday_t, dmnpi[0])},
	/*F187*/{0x07,35, DB_OFFSET(db_metday_t, dmnna[0])},
	/*F188*/{0x07,35, DB_OFFSET(db_metday_t, dmnni[0])},
};

//F189~F192
static const dbsonconfig_t cons_grp24b[] = {
	/*F189*/{0x07,35, DB_OFFSET(db_metrmd_t, dmnpa[0])},
	/*F190*/{0x07,35, DB_OFFSET(db_metrmd_t, dmnpi[0])},
	/*F191*/{0x07,35, DB_OFFSET(db_metrmd_t, dmnna[0])},
	/*F192*/{0x07,35, DB_OFFSET(db_metrmd_t, dmnni[0])},
};

//F193~F196
static const dbsonconfig_t cons_grp25[] = {
	/*F193*/{0x07,35, DB_OFFSET(db_metmon_t, dmnpa[0])},
	/*F194*/{0x07,35, DB_OFFSET(db_metmon_t, dmnpi[0])},
	/*F195*/{0x07,35, DB_OFFSET(db_metmon_t, dmnna[0])},
	/*F196*/{0x07,35, DB_OFFSET(db_metmon_t, dmnni[0])},
};

/*static const dbsonconfig_t cons_grp27a[] = {
	{0x00, 41, DB_OFFSET(db_metmonpre_t, readtimebuy)},
	{0x00, 61, DB_OFFSET(db_metmonpre_t, readtimepre)},	
};
 
static const dbsonconfig_t cons_grp27c[] = {
	{0x00, 41, DB_OFFSET(db_cpday_prepaidinfo_t, readtimebuy)},	
};*/

//F221
static const dbsonconfig_t cons_grp28[] = {
	{0x00,6,  DB_OFFSET(db_termday_t, sig_max)},
};


const dbaseconfig_t DbaseConfig[] = {
	{0,   0xff, DBID_MET_DAY(0),      DBSAVE_DAY,   DBATTR_METP, 0, cons_grp1},
	{1,   0x0f, DBID_MET_CPYDAY(0),   DBSAVE_CPYDAY,DBATTR_METP, 0, cons_grp2},
	{2,   0xff, DBID_MET_MONTH(0),    DBSAVE_MONTH, DBATTR_METP, 0, cons_grp3},
	//{3,   0xff, DBID_METSTIC_DAY(0),  DBSAVE_DAY,   DBATTR_METP, 0, cons_grp4},
	//{4,   0x7f, DBID_METSTIC_MONTH(0),DBSAVE_MONTH, DBATTR_METP, 0, cons_grp5},
	//{5,   0x04, DBID_METSTIC_DAY(0),  DBSAVE_DAY,   DBATTR_METP, 0, cons_grp6a},
	//{5,   0x08, DBID_METSTIC_MONTH(0),DBSAVE_MONTH, DBATTR_METP, 0, cons_grp6b},
	{6,   0x03, DBID_TERMSTIC_DAY,    DBSAVE_DAY,   DBATTR_TERM, 0, cons_grp7a},
	{6,   0x0c, DBID_TERMSTIC_MONTH,  DBSAVE_MONTH, DBATTR_TERM, 0, cons_grp7b},
	{6,   0x10, DBID_TERMSTIC_DAY,    DBSAVE_DAY,   DBATTR_TERM, 0, cons_grp7c},
	{6,   0x20, DBID_TERMSTIC_MONTH,  DBSAVE_MONTH, DBATTR_TERM, 0, cons_grp7d},
	//{6,   0xc0, DBID_PLSTICDAY,       DBSAVE_DAY,   DBATTR_TERM, 0, cons_grp7e},
	//{7,   0x07, DBID_TGRP_DAY(0),     DBSAVE_DAY,   DBATTR_TGRP, 0, cons_grp8a},
	//{7,   0x38, DBID_TGRP_MONTH(0),   DBSAVE_MONTH, DBATTR_TGRP, 0, cons_grp8b},
	//{8,   0x03, DBID_TGRP_MONTH(0),   DBSAVE_MONTH, DBATTR_TGRP, 0, cons_grp9},
	//{9,   0x0f, DBID_TGRP_CURVE(0),   DBSAVE_CURVE, DBATTR_TGRP, 0, cons_grp10},
	{10,  0xff, DBID_MET_CURVE(0),    DBSAVE_CURVE, DBATTR_METP, 0, cons_grp11},
	{11,  0x7f, DBID_MET_CURVE(0),    DBSAVE_CURVE, DBATTR_METP, 0, cons_grp12},
	{12,  0xff, DBID_MET_CURVE(0),    DBSAVE_CURVE, DBATTR_METP, 0, cons_grp13},
	{13,  0x3f, DBID_MET_CURVE(0),    DBSAVE_CURVE, DBATTR_METP, 0, cons_grp14},
	//{14,  0x3f, DBID_METSTIC_DAY(0),  DBSAVE_DAY,   DBATTR_METP, 0, cons_grp15},
	//{15,  0x07, DBID_METSTIC_DAY(0),  DBSAVE_DAY,   DBATTR_METP, 0, cons_grp16},
	//{16,  0x01, DBID_ANASIG_DAY(0),   DBSAVE_DAY,   DBATTR_ANASI,0, cons_grp17a},
	//{16,  0x02, DBID_ANASIG_MONTH(0), DBSAVE_MONTH, DBATTR_ANASI,0, cons_grp17b},
	//{17,  0x02, DBID_ANASIG_CURVE(0), DBSAVE_CURVE, DBATTR_ANASI,0, cons_grp18a},
	//{17,  0x10, DBID_METSTIC_DAY(0), DBSAVE_DAY, DBATTR_METP,0, cons_grp18b},
	//{17,  0x20, DBID_METSTIC_MONTH(0), DBSAVE_MONTH, DBATTR_METP,0, cons_grp18c},
	{18,  0x0f, DBID_MET_CURVE(0),    DBSAVE_CURVE, DBATTR_METP, 0, cons_grp19},
	{19,  0x0f, DBID_MET_DAY(0),      DBSAVE_DAY,   DBATTR_METP, 0, cons_grp20a},
	{19,  0xf0, DBID_MET_MONTH(0),    DBSAVE_MONTH, DBATTR_METP, 0, cons_grp20b},
	{20,  0xff, DBID_MET_DAY(0),      DBSAVE_DAY,   DBATTR_METP, 0, cons_grp21},
	{21,  0xff, DBID_MET_CPYDAY(0),   DBSAVE_CPYDAY,DBATTR_METP, 0, cons_grp22},
	{22,  0xff, DBID_MET_MONTH(0),    DBSAVE_MONTH, DBATTR_METP, 0, cons_grp23},
	{23,  0x0f, DBID_MET_DAY(0),      DBSAVE_DAY,   DBATTR_METP, 0, cons_grp24a},
	{23,  0xf0, DBID_MET_CPYDAY(0),   DBSAVE_CPYDAY,DBATTR_METP, 0, cons_grp24b},
	{24,  0x0f, DBID_MET_MONTH(0),    DBSAVE_MONTH, DBATTR_METP, 0, cons_grp25},
	
	{27,  0x10, DBID_TERMSTIC_DAY,    DBSAVE_DAY,   DBATTR_TERM, 0, cons_grp28},

	{0, 0, 0, 0, 0, 0, NULL},
};

