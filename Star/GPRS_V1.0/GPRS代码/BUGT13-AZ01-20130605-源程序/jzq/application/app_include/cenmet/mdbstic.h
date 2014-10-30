/**
* mdbstic.h -- 统计数据
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-14
* 最后修改时间: 2009-5-14
*/

#ifndef _MDB_STIC_H
#define _MDB_STIC_H
#if 0
typedef struct {
	unsigned char min;
	unsigned char hour;
	unsigned char day;
	unsigned char mon;
} stictime_t;

typedef struct {
	unsigned int pwramax[4];    //最大有功功率 0.1W
	stictime_t pwramax_time[4];    //最大有功功率发生时间
	unsigned short pwrzero_time[4];    //有功功率为零时间, 分

	unsigned int dmmax[4];   //分相最大需量 0.1W
	stictime_t dmtime[4];   //分相最大需量发生时间

	unsigned short vol_overtime[3];    //过压累计时间
	unsigned short vol_lesstime[3];    //欠压累计时间
	unsigned short vol_uptime[3];    //越上限累计时间
	unsigned short vol_lowtime[3];    //越下限累计时间
	unsigned short vol_oktime[3];    //合格累计时间

	unsigned short volmax[3];
	unsigned short volmin[3];
	stictime_t volmax_time[3];
	stictime_t volmin_time[3];
	unsigned int volsum[3];    //平均电压
	unsigned short volsnum[3];

	unsigned short ampunb_time;   //电流不平衡累计时间
	unsigned short volunb_time;   //电压不平衡累计时间
	unsigned short ampunb_max;    //电流不平衡最大值,%
	unsigned short volunb_max;    //电压不平衡最大值,%
	stictime_t ampunbmax_time;    //电流不平衡最大值发生时间
	stictime_t volunbmax_time;    //电压不平衡最大值发生时间

	unsigned short ampover_time[3];   //过流累计时间
	unsigned short ampup_time[3];   //电流越限累计时间
	unsigned short zampup_time;   //零序电流越限累计时间
	unsigned int ampmax[4];    //电流最大值
	stictime_t ampmax_time[4];   //电流最大值发生时间

	unsigned short pwrvov_time;    //视在功率越上上限累计时间
	unsigned short pwrvup_time;    //视在功率越上限累计时间

	unsigned short pwrf_time[3];   //功率因数区段累计时间

	unsigned short load_max;  //负载率最大值,%
	unsigned short load_min;  //负载率最小值,%
	stictime_t loadmax_time;  //负载率最大值发生时间
	stictime_t loadmin_time;  //负载率最小值发生时间

	unsigned short amplost_time[3];   //失流累计时间
	unsigned short ampbreak_time[3];  //断流累计时间 (电流开路)
} metpstic_t;

typedef struct {
	unsigned short amp_max[19];  //单相2-19次谐波电流日最大值
	unsigned short vol_max[19];  //单相2-19次谐波电压含有率及总畸变率日最大值
	stictime_t ampmax_time[19];  //单相2-19次谐波电流日最大值发生时间
	stictime_t volmax_time[19];  //单相2-19次谐波电压含有率及总畸变率日最大值发生时间
	unsigned short volup_times[19];  //单相谐波电压含有率越限日累计时间
	unsigned short ampup_times[19];  //单相谐波电流越限日累计时间
} syntony_stic_t;

typedef struct {
	int pwrmax;
	stictime_t pwrmax_time;
	int pwrmin;
	stictime_t pwrmin_time;

	unsigned short pwrzero_time;
} tgrpstic_day_t;

typedef struct {
	int pwrmax;
	stictime_t pwrmax_time;
	int pwrmin;
	stictime_t pwrmin_time;

	unsigned short pwrzero_time;

	//@add later: 未处理超限累计
	unsigned short pcov_time;   //超功率定值月累计时间及累计电量
	unsigned short ecov_time;  //超月电量定值月累计时间及累计电量
	int pcov_ene;
	int ecov_ene;
} tgrpstic_mon_t;

typedef struct {
	unsigned short up_time;  //越上限累计时间, min
	unsigned short dwn_time;  //越下限累计时间
	int max_v;  //最大值 , 0.01
	int min_v;  //最小值 , 0.01
	stictime_t max_time;
	stictime_t min_time;
} anasig_stic_t;
#endif

typedef struct {
	unsigned short pwr_time;   //供电时间
	unsigned short rst_cnt;   //复位次数
	unsigned char sw_cnt[4];   //跳闸累计次数
	unsigned int comm_bytes;  //终端与主站通信流量
	unsigned char sig_max;
	unsigned char sigmax_time[2];  //min, hour
	unsigned char sig_min;
	unsigned char sigmin_time[2];  //min, hour
} termstic_t;

typedef struct {
	//metpstic_t metp_day[MAX_CENMETP];
	//metpstic_t metp_mon[MAX_CENMETP];

	//tgrpstic_day_t tgrp_day[MAX_TGRP];
	//tgrpstic_mon_t tgrp_mon[MAX_TGRP];

	//anasig_stic_t anasig_day[MAX_ANASIG];
	//anasig_stic_t anasig_mon[MAX_ANASIG];

	termstic_t term_day;
	termstic_t term_mon;

	//syntony_stic_t syntony_day[3]; //abc

	unsigned char year;
	unsigned char mon;
	unsigned char day;
	unsigned char upflow_flag;  //与主站通信流量超限标志,0-未超限, 1-超限
} mdbstic_t;

#ifndef DEFINE_MDBSTIC
extern /*const*/ mdbstic_t MdbStic;
#endif

void UpdateMdbStic(void);
void MdbSticEmptyDay(void);
void MdbSticEmptyMonth(void);
void MdbSticInit(void);

void UpdateSticResetCount(void);
//void UpdateSticComm(unsigned int bytes);
//void UpdateSticSwCount(int offset);
//void UpdateSticSig(unsigned char sig);

//void BakupSticMetp(unsigned char flag, unsigned short mid, unsigned char *buf);
//void BakupSticSyntony(unsigned char flag, unsigned short mid, unsigned char *buf);
void BakupSticTerm(unsigned char flag, unsigned char *buf);

//void BakupSticTGrpDay(unsigned short tid, unsigned char *buf);
//void BakupSticTGrpMon(unsigned short tid, unsigned char *buf);

//void BakupSticAnaSig(unsigned char flag, unsigned short sigid, unsigned char *buf);

void BakupMdbStic(void);

#endif /*_MDB_STIC_H*/

