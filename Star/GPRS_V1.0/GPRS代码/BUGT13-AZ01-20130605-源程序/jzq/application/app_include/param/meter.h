/**
* meter.h -- 表计参数头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-7
* 最后修改时间: 2009-5-7
*/

#ifndef _PARAM_METER_H
#define _PARAM_METER_H

#include "app_include/param/capconf.h"
#include "app_include/param/commport.h"
#include "app_include/param/datause.h"
#include "app_include/version.h"
#include "plat_include/timeal.h"
#include "plat_include/sys_config.h"


//F10, 终端电能表/交流采样配置参数
#define METTYPE_DL645		1
#define METTYPE_ACSAMP		2
#define METTYPE_DL645_XF	11	//江苏扩增
#define METTYPE_DL645_TF	12	//江苏扩增
#define METTYPE_DL645_2007	30

#define METTYPE_PLC			31

/*
typedef struct {
	unsigned short index;  //表计索引号,储存用, 其他地方不用
	unsigned short metp_id;    //所属测量点号, 1~2040, 0表示无效
	unsigned char port;    //通信端口号
	unsigned char proto;    //通信规约类型, 1-DL645(1997), 2-交流采样, 30-DL645(2007)
	unsigned char addr[6];    //通信地址
	unsigned char pwd[6];    //通信密码
	unsigned char prdnum;    //电能费率个数
	unsigned char intdotnum;    //有功电能示值整数位和小数位个数
	unsigned char owneraddr[6];    //所属采集器地址
	unsigned char usrclass;    //用户分类号和电表分类号
	unsigned char speed;  //通信速率
} para_meter_t;
*/
typedef struct {
	unsigned char metp_id;    //所属测量点号, 1~2040, 0表示无效
//	unsigned char port;    //通信端口号
	//unsigned char proto;    //通信规约类型, 1-DL645(1997), 2-交流采样, 30-DL645(2007)
	unsigned char addr[6];    //通信地址
#ifdef METER_MODULE
	unsigned char prdnum;    //电能费率个数
	unsigned char usrclass;    //用户分类号和电表分类号
	unsigned char owneraddr[6];    //所属采集器地址
#endif
//	unsigned char speed;  //通信速率
} simple_para_meter_t;

#ifndef DEFINE_PARAMETER
extern /*const*/simple_para_meter_t ParaMeter[MAX_METER];
#endif

///无效表配置
#define EMPTY_METER(metid)		(ParaMeter[metid].metp_id == 0)
// || ParaMeter[metid].proto == 0)

#define METERSN_TO_METPID(metersn)  (ParaMeter[metersn-1].metp_id)

//总表
//#define CEN_METER(metid)		(ParaMeter[metid].port == (COMMPORT_CEN_485_1+1) || \
//								 ParaMeter[metid].port == (COMMPORT_CEN_485_2+1)) 
//RS485总表
#define RS485_CENMETER(metid)	(ParaMeter[metid].port == (COMMPORT_CEN_485_1+1) || \
								 ParaMeter[metid].port == (COMMPORT_CEN_485_2+1)) 
//载波用户表
#define PLC_METER(metid)		0
//RS485用户表
#define RS485_USRMETER(metid)	0
//用户表
#define USER_METER(metid)		0
//单费率表
#define SF_METER(metid)			(ParaMeter[metid].prdnum == 1 || \
								(ParaMeter[metid].proto == METTYPE_DL645_TF))
//多费率表 顺序为总、峰、平、谷、尖
#define XF_METER(metid)			(ParaMeter[metid].proto == METTYPE_DL645_XF)								

//两费率表 顺序为总、峰、谷
#define TF_METER(metid)			(ParaMeter[metid].proto == METTYPE_DL645_TF)	


#endif /*_PARAM_METER_H*/

