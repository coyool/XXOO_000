/**
* dbconfig.h -- 历史数据库配置
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-11
* 最后修改时间: 2009-5-11
*/

#ifndef _DB_CONFIG_H
#define _DB_CONFIG_H

#include "app_include/lib/dbtime.h"

#define DBID_MET_DAY(mid)			((unsigned short)0+(mid)) 
#define DBID_MET_CPYDAY(mid)		((unsigned short)80+(mid))
#define DBID_MET_MONTH(mid)			((unsigned short)160+(mid))
#define DBID_MET_CURVE(mid)			((unsigned short)240+(mid))
#define DBID_TERMSTIC_DAY			((unsigned short)321)
#define DBID_TERMSTIC_MONTH			((unsigned short)322)

#define DBID_END					323

#define DBSAVE_DAY		0    //日冻结
#define DBSAVE_CPYDAY	1    //抄表日冻结
#define DBSAVE_MONTH	2    //月冻结
#define DBSAVE_CURVE	3    //曲线

#define DBATTR_METP		0    //测量点数据
#define DBATTR_TGRP		1    //总加组数据
#define DBATTR_TERM		2    //终端数据
#define DBATTR_UMETP	3    //用户表数据
#define DBATTR_IMPUSR	4    //重点用户表数据
#define DBATTR_ANASI	5   //直流模拟量数据

#define DBFLAG_RDTIME	0x01	//带抄表时间
#define DBFLAG_FENUM	0x02   //带费率数
#define DBFLAG_SPECIAL	0x04  //特殊处理(如数据不是按顺序排列)

typedef struct {
	unsigned char flag;    //数据标志
	unsigned char len;    //子项数据长度
	unsigned int offset;    //子项数据地址基于数据库基地址的偏移
} dbsonconfig_t;

typedef struct {
	unsigned char grpid;    //组标识, F1~F8=0, F9~F16=1,...
	unsigned char cids;   //数据单元掩码
	unsigned short dbid;   //数据库ID
	unsigned char type;   //数据库类型
	unsigned char attr;    //数据库属性
	unsigned short colen;   //数据库列长度, 所有测量点数据存于一个文件时用
	const dbsonconfig_t *psons;
} dbaseconfig_t;
extern const dbaseconfig_t DbaseConfig[];

//#define DB_OFFSET(type, var)	((unsigned int)(&(((type *)0)->var)))
#define DB_OFFSET(type, var)	((unsigned int)((char*)&(((type *)0)->var)-(char*)0))

int DbaseItemSize(unsigned short dbid);
int DbaseItemType(unsigned short dbid,unsigned char *frztype);

//int DbaseFileName(char *filename, unsigned short dbid, dbtime_t dbtime);
//int DbaseFileGroupName(char *filename, unsigned short dbid);
int DbaseGetMetid(unsigned short dbid,unsigned short *metid);

#endif /*_DB_CONFIG_H*/

