/**
* capconf.h -- 容量配置
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-10
* 最后修改时间: 2009-5-10
*/

#ifndef _PARAM_CAPCONF_H
#define _PARAM_CAPCONF_H

#include "plat_include/sys_config.h"

#ifdef METER_MODULE
#define MAX_METER    16   //最大电表/交流采样数
#endif

#ifdef SMALL_CONCENT
#define MAX_METER    500   //最大电表/交流采样数
#endif

#ifdef BIG_CONCENT
#define MAX_METER    500   //最大电表/交流采样数
#endif

#define MAX_METP		128   //最大测量点数

#define MAX_CENMETP		MAX_METER     //最大多功能测量点数, 载波表计的测量点号只能大于这个

#define MAX_IMPORTANT_USER		0   //最大重点用户表数


#define MAX_PULSE	0  //脉冲最大数
#define MAX_ISIG	1  //状态量输入最大数
#define MAX_TGRP	0  //总加组最大数
#define MAX_DIFFA	0  //有功总电能量差动最大数

#define MAX_DTASK_CLS1		64   // 1类任务最大数
#define MAX_DTASK_CLS2		0  // 2类任务最大数

//#define MAX_SWP		0   //最大控制轮次
//#define MASK_SWP	0x00  //轮次掩码

//#define MAX_ANASIG		1	//最大直流模拟量数
//#define MASK_ANASIG		0x00

//#define MAX_VOLANA		0  //最大电压/电流模拟量(未用)

#endif /*_PARAM_CAPCONF_H*/

