/**
* metp.h -- 测量点参数头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-7
* 最后修改时间: 2009-5-7
*/

#ifndef _PARAM_METP_H
#define _PARAM_METP_H

#include "app_include/param/capconf.h"

#define PWRTYPE_3X3W	1
#define PWRTYPE_3X4W	2
#define PWRTYPE_1X		3

//F25, 测量点基本参数
typedef struct {
	unsigned short pt;    //电压互感器倍率
	unsigned short ct;    //电流互感器倍率
	unsigned short vol_rating;    //额定电压, 0.1V
	unsigned short amp_rating;    //额定电流, 0.01A
	unsigned int pwr_rating;   //额定负荷, 0.0001kVA
	unsigned char pwrtype;    //电源接线方式, 
	                             // 1=三相三线,2=三相四线,3=单相
	unsigned char pwrphase;   //单相表接线相, 0-unknown, 1-A, 2-B, 3-C
	unsigned char unused[2];
} cfg_mpbase_t;

//F26, 测量点限值参数
typedef struct {
	unsigned short volok_up;    //电压合格上限, 0.1V
	unsigned short volok_low;    //电压合格下限, 0.1V
	unsigned short vol_lack;    //电压断相门限, 0.1V
	unsigned short vol_over;    //过压门限, 0.1V
	unsigned short vol_less;    //欠压门限, 0.1V

	unsigned int amp_over;    //过流门限, 0.001A
	unsigned int amp_limit;    //额定电流门限, 0.001A
	unsigned int zamp_limit;    //零序电流上限, 0.001A

	unsigned int pwr_over;    //视在功率上上限, 0.0001kVA
	unsigned int pwr_limit;    //视在功率上限, 0.0001kVA

	unsigned short vol_unb;    //三相电压不平衡限值, 0.1%
	unsigned short amp_unb;    //三相电流不平衡限值, 0.1%

	unsigned char time_volover;   //越限持续时间, 分
	unsigned char time_volless;
	unsigned char time_ampover;
	unsigned char time_amplimit;
	unsigned char time_zamp;
	unsigned char time_pwrover;
	unsigned char time_pwrlimit;
	unsigned char time_volunb;
	unsigned char time_ampunb;
	unsigned char time_volless_2;    //连续失压时间限值, min

	unsigned char resr_pwrover[2];  //越限恢复系数 0.1% BCD格式
	unsigned char resr_pwrlimit[2];
	unsigned char resr_volover[2];
	unsigned char resr_volless[2];
	unsigned char resr_ampover[2];
	unsigned char resr_amplimit[2];
	unsigned char resr_zamp[2];
	unsigned char resr_volunb[2];
	unsigned char resr_ampunb[2];
} cfg_mplimit_t;

//多功能测量点参数
typedef struct {
	unsigned short mid;   //use for save
	cfg_mpbase_t base;
	cfg_mplimit_t limit;
} para_cenmetp_t;

//测量点到各个电表的映射
#define METP_NONE		0
#define METP_METER		1
#define METP_PULSE		2
#define METP_USER		3

typedef struct {
	//unsigned char type;   //类型, 0-无效, 1-电表, 2-脉冲
	//unsigned char reserv;
	unsigned char  metid;   //表号-对应电表号,脉冲号等等
} metp_map_t;

#ifndef DEFINE_PARAMETP
extern /*const*/ metp_map_t MetpMap[MAX_METP];
#endif

#define EMPTY_CENMETP(mid)		EMPTY_METER(mid)

#endif /*_PARAM_METP_H*/

