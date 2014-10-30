/**
* sys_config.h -- 系统抽象模块配置文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-3-30
* 最后修改时间: 2009-3-30
*/

#ifndef _SYS_CONFIG_H
#define _SYS_CONFIG_H

/*
	小集中器定义
	所有的大小集中器原理图的不同都由此宏控制
*/
/**********************
	小集中器定义
***********************/
//#define SMALL_CONCENT


/***********************
 *	大集中器定义	   *
 ***********************/
//#define BIG_CONCENT

/***********************
*	表端Gprs模块定义   *
************************/
//#define METER_MODULE


#define MAX_PTHREAD		32  //软件最大线程(任务)数

//最大时钟定时器数
#define MAX_RTIMER		8
//最大相对定时器数
#define MAX_CTIMER   	8

#define MAX_SYSLOCK		2

/*
	调试口使用的串口号码
*/
#ifdef SMALL_CONCENT
#define DEBUG_PORT 	1
#endif

#ifdef BIG_CONCENT
#define DEBUG_PORT 	2
#endif

#ifdef METER_MODULE
#define DEBUG_PORT 	1
#endif

/*
	系统名称
*/
#ifdef METER_MODULE
#define SYSTEM_NAME 	"表端模块"
#endif

#ifdef SMALL_CONCENT
#define SYSTEM_NAME 	"简易集中器"
#endif

#ifdef BIG_CONCENT
#define SYSTEM_NAME 	"大集中器"
#endif




#ifdef METER_MODULE
#define USE_SECOND
#endif
/*
	载波测试
*/
#ifdef BIG_CONCENT
//	#define TEST_PLC
#endif

#define DEBUG_BPS	115200

//#define GPRS_MODULE_MC55I
#define GPRS_MODULE_M35


/*
	Gprs使用的串口号码
*/
#ifdef METER_MODULE
#define  GPRS_PORT 2
#else
#define  GPRS_PORT 0
#endif

#define UART_IR	      4
#define UART_DOWN485_1 	0
#define GPRS_FRAG		1460

#ifdef BIG_CONCENT
#define DEFINE_PLCOMM
#endif

#ifdef 	METER_MODULE 
#define DEFINE_SERIAL
#endif


#ifdef 	DEFINE_PLCOMM
#define PLC_PORT        3
#endif

#define FLASH_PFPARA_SECTOR			1
#define FLASH_PFPARA_BACK_SECTOR 	2

#endif /*_SYS_CONFIG_H*/
