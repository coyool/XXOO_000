/**
* commport.h -- 通信端口参数头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-8
* 最后修改时间: 2009-5-8
*/
#include "app_include/param/unique.h"

#ifndef _PARAM_COMMPORT_H
#define _PARAM_COMMPORT_H

#define DEFAULT_RDMET_CYCLE		15   //缺省抄表时间间隔(分)

#define COMMFRAME_BAUD_300		0
#define COMMFRAME_BAUD_600		0x20
#define COMMFRAME_BAUD_1200		0x40
#define COMMFRAME_BAUD_2400		0x60
#define COMMFRAME_BAUD_4800		0x80
#define COMMFRAME_BAUD_7200		0xa0
#define COMMFRAME_BAUD_9600		0xc0
#define COMMFRAME_BAUD_19200	0xe0

#define COMMFRAME_STOPBIT_1		0
#define COMMFRAME_STOPBIT_2		0x10

#define COMMFRAME_NOCHECK		0
#define COMMFRAME_HAVECHECK		0x08

#define COMMFRAME_EVENCHECK		0
#define COMMFRAME_ODDCHECK	    0x04

#define COMMFRAME_DATA_5		0
#define COMMFRAME_DATA_6		1
#define COMMFRAME_DATA_7		2
#define COMMFRAME_DATA_8		3

#define COMMPORT_CEN_485_1		1 // 1  //抄表端口(总表)
#define COMMPORT_CEN_485_2		2 // 2  //抄表端口(总表)
#define COMMPORT_CEN_485_NUM	1 // 2  //抄表端口(总)

#define MAX_COMMPORT		COMMPORT_CEN_485_NUM //最大端口数

//#define COMMPORT_CENMETP	COMMPORT_CEN_485  //缺省抄表接口和参数读取端口(主要是抄表时间间隔)
//#define COMMPORT_USERMETER	COMMPORT_USR_485  //485用户表抄表接口
//#define COMMPORT_USRMETEXP1	COMMPORT_USR_EXP1
//#define COMMPORT_USRMETEXP2	COMMPORT_USR_EXP2
#define   COMMPORT_DLMS	33

#define RDMETFLAG_ENABLE		0x0001  //置"1"不允许自动抄表，置"0" 要求终端根据抄表时段自动抄表
#define RDMETFLAG_ALL			0x0002  //置"1"要求终端只抄重点表，置"0"要求终端抄所有表
#define RDMETFLAG_FREZ			0x0004  //要求终端采用广播冻结抄表，置"0"不要求
#define RDMETFLAG_CHECKTIME		0x0008  //置"1"要求终端定时对电表广播校时，置"0"不要求
#define RDMETFLAG_FINDMET		0x0010  //置"1"要求终端搜寻新增或更换的电表，置"0"不要求
#define RDMETFLAG_RDSTATUS		0x0020  //置"1"要求终端抄读"电表状态字"，置"0"不要求
//F33 终端抄表运行参数设置
//F34 与终端接口的通信模块的参数设置
#define MAXNUM_PERIOD	24
typedef struct {
	unsigned char hour_start;
	unsigned char min_start;
	unsigned char hour_end;
	unsigned char min_end;
}cfg_period_t;

typedef struct {
	unsigned short flag;  //台区集中抄表运行控制字
	unsigned char time_hour;	//抄表日-时间:时
	unsigned char time_minute;  //抄表日-时间:分
	unsigned int dateflag;  //抄表日-日期   总表
	unsigned char cycle;  //抄表间隔时间, 分, 1~60  总表
	unsigned char chktime_day;     //对电表广播校时定时时间:日
	unsigned char chktime_hour;    //对电表广播校时定时时间:时
	unsigned char chktime_minute;  //对电表广播校时定时时间:分
	unsigned char periodnum;	//允许抄表时段数
	unsigned char reserv;
	cfg_period_t period[MAXNUM_PERIOD];  //允许抄表时段? 总表
	
	unsigned int baudrate;  //与终端接口对应端的通信速率（bps）
	unsigned char frame;  //与终端接口端的通信控制字
} para_commport_t;

const para_commport_t *GetParaCommPort(unsigned int port);

#endif /*_PARAM_COMMPORT_H*/

