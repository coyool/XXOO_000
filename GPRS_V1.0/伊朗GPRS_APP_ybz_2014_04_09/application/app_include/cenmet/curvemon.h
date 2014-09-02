/**
* curvemon.h -- 月冻结类历史数据结构
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-12
* 最后修改时间: 2009-5-12
*/

#ifndef _CURVE_MON_H
#define _CURVE_MON_H

#include "app_include/cenmet/mdbconf.h"

typedef struct {
	unsigned char rdtime[5];    //抄表时间, 分时日月年
	unsigned char fenum;  //费率数

	//F17
	unsigned char enepa[5*MAXNUM_METPRD];  //正向有功电能, 0~4, 0.0001kWh
	unsigned char enepi[4*MAXNUM_METPRD];  //正向无功电能, 0~4, 0.01kvarh
	unsigned char enepi1[4*MAXNUM_METPRD];  //正向无功一象限电能, 0~4, 0.01kvarh
	unsigned char enepi4[4*MAXNUM_METPRD];  //正向无功四象限电能, 0~4, 0.01kvarh
	//F18
	unsigned char enena[5*MAXNUM_METPRD];  //反向有功电能, 0~4, 0.0001kWh
	unsigned char eneni[4*MAXNUM_METPRD];  //反向无功电能, 0~4, 0.01kvarh
	unsigned char eneni2[4*MAXNUM_METPRD];  //反向无功二象限电能, 0~4, 0.01kvarh
	unsigned char eneni3[4*MAXNUM_METPRD];  //反向无功三象限电能, 0~4, 0.01kvarh
	//F19
	unsigned char dmnpa[3*MAXNUM_METPRD];  //当月正向有功最大需量,0~4,0.0001kW
	unsigned char dmntpa[4*MAXNUM_METPRD];  //当月正向有功最大需量发生时间,0~4,分时日月
	unsigned char dmnpi[3*MAXNUM_METPRD];  //当月正向无功最大需量,0~4,0.0001kvar
	unsigned char dmntpi[4*MAXNUM_METPRD];  //当月正向无功最大需量发生时间,0~4,分时日月
	//F20
	unsigned char dmnna[3*MAXNUM_METPRD];  //当月反向有功最大需量,0~4,0.0001kW
	unsigned char dmntna[4*MAXNUM_METPRD];  //当月反向有功最大需量发生时间,0~4,分时日月
	unsigned char dmnni[3*MAXNUM_METPRD];  //当月反向无功最大需量,0~4,0.0001kvar
	unsigned char dmntni[4*MAXNUM_METPRD];  //当月反向无功最大需量发生时间,0~4,分时日月
	//F21
	//unsigned char enepa_day[4*MAXNUM_METPRD];     //当月正向有功电能量, 0.0001kWh
	//F22
	//unsigned char enepi_day[4*MAXNUM_METPRD];     //当月正向无功电能量, 0.0001kvarh
	//F23
	//unsigned char enena_day[4*MAXNUM_METPRD];     //当月反向有功电能量, 0.0001kWh
	//F24
	//unsigned char eneni_day[4*MAXNUM_METPRD];     //当月反向无功电能量, 0.0001kvarh
	//F157
	unsigned char enepa_abc[5*3];	//ABC相正向有功电能示值, 0.0001kWh
	//F158
	unsigned char enepi_abc[4*3];	//ABC相正向无功电能示值, 0.01kvarh
	//F159
	unsigned char enena_abc[5*3];	//ABC相反向有功电能示值, 0.0001kWh
	//F160
	unsigned char eneni_abc[4*3];	//ABC相正向无功电能示值, 0.01kvarh
} db_metmon_t;

//预付费信息月冻结
typedef struct{
	//F215  购用电信息
	unsigned char readtimebuy[5];  //分时日月年
	unsigned char buyene_cnt[2];  //购电次数 
	unsigned char money_res[5];  //剩余金额 
	unsigned char money_buyed[5];  //累计购电金额
	unsigned char ene_res[4];  //剩余电量
	unsigned char ene_overdraft[4];  //透支电量
	unsigned char ene_buyed[4];  //累计购电量 
	unsigned char ene_ticklmt[4];  //赊欠门限电量
	unsigned char ene_alarm[4];  //报警电量
	unsigned char ene_err_buy[4];  //故障电量

    //F216 结算信息       
	unsigned char readtimepre[5];
	unsigned char fenumbanla;
	unsigned char ene_febuyed[MAXNUM_METPRD*5];  //已结有功电能
	unsigned char ene_feunbuy[MAXNUM_METPRD*5];  //未结有功电能
	unsigned char ene_err_banla[5];    //故障电量
	
}db_metmonpre_t;

typedef struct {
	//F51
	unsigned char pwr_time[2];    //终端月供电时间, min
	unsigned char rest_cnt[2];    //终端月复位次数
	//F52
	unsigned char sw_time[4];	 //跳闸累计次数
	//F54
	unsigned char comm_bytes[4];  //终端与主站月通信流量, 字节
} db_termmon_t;

#endif /*_CURVE_MON_H*/

