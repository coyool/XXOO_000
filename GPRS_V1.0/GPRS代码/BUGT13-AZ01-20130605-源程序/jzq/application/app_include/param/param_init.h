#ifndef _PARA_INIT_H
#define _PARA_INIT_H

#include "app_include/param/commport.h"
#include "app_include/param/capconf.h"

//F12, 状态量输入参数
typedef struct {
	unsigned char flagin;    //接入标志位
	unsigned char flagattr;    //属性标志位
	//unsigned char reserv[2];
} para_isig_t;

#if 0
//F59 电能表异常判别阈值设定
typedef struct {
	unsigned char diff;  //电能量超差阈值
	unsigned char fly;  //电能表飞走阈值
	unsigned char stop;  //电能表停走阈值
	unsigned char time;  //电能表校时阈值
} cfg_metabnor_t;
#endif

typedef struct {
	para_isig_t isig;//F12终端状态量输入参数
	unsigned char bactsend;  //禁止主动上报, 1-禁止,0-不禁止
	unsigned int upflow_max;  //F36 终端上行通信流量门限设置 byte
	//para_commport_t ParaCommPort[MAX_COMMPORT];
//	cfg_metabnor_t metabnor;
} para_mix_t;


//F1, 终端通信参数设置
typedef struct {
	unsigned char rts;		//终端数传机延时时间
	unsigned char delay;	//终端作为启动站允许发送延时时间
	unsigned short rsnd;	//终端等待从动站相应的超时时间和重发次数
	unsigned char flagcon;	//需要主站确认的通信服务(CON=1)的标志
	unsigned char cycka;	//心跳周期, 分
} cfg_tcom_t;

//F2, 终端中继转发设置, 保留

//F4, 主站电话号码和短信中心号码
typedef struct {
	unsigned char phone[8];
	unsigned char sms[8];
} cfg_smsc_t;

//F5, 终端密码设置
typedef struct {
	unsigned char art;    //密码算法编号
	unsigned char pwd[2];    //密钥
} cfg_pass_t;

#ifdef afn04f6
//F6, 终端组地址设置
typedef struct {
	unsigned char addr[16];
} cfg_grpaddr_t;
#endif

//F16, 虚拟专网用户名, 密码
typedef struct {
	char user[32];
	char pwd[32];
} cfg_vpn_t;

//F3, 主站IP地址和端口号
typedef struct {
	unsigned char ipmain[4];
	unsigned short portmain;
	unsigned char ipbakup[4];
	unsigned short portbakup;
	char apn[20];
} cfg_svrip_t;
//F7, 终端IP地址和端口
typedef struct {
	unsigned char ipterm[4];
	unsigned char maskterm[4];
	unsigned char ipgatew[4];
	unsigned char ipproxy[4];
	unsigned short portproxy;
	unsigned char proxy_type;
	unsigned char proxy_connect;
	char username[32];
	char pwd[32];
	unsigned short portlisten;
} cfg_termip_t;

//F8, 终端上行通信工作方式
typedef struct {
	unsigned char proto;  //0-TCP, 1-UDP
	unsigned char mode;   //0-mix, 1-client, 2-server
	unsigned char clientmode;   //0-永久在线, 1-被动激活, 2-时段在线
	unsigned char countdail;  //重拨次数
	unsigned short timedail;  //重拨间隔 second
	unsigned char timedown;  //无通信自动断线时间 minute
	unsigned char unsued;
	unsigned int onlineflag;  //在线时段标志
} cfg_uplink_t;


//F9, 终端事件记录配置
typedef struct {
	unsigned char valid[8];   //事件有效
	unsigned char rank[8];    //事件等级, 1-重要事件, 0-一般事件
} cfg_almflag_t;


typedef struct {
	cfg_tcom_t tcom;    //F1
	//F2
	cfg_svrip_t svrip;    //F3
	cfg_smsc_t smsc;    //F4
	cfg_pass_t pwd;    //F5
	//cfg_grpaddr_t grpaddr;    //F6
	cfg_termip_t termip;    //F7
	cfg_uplink_t uplink;    //F8
	cfg_almflag_t almflag;    //F9
	cfg_vpn_t vpn;   //F16

	//char term_name[20];  //F97, 终端名称
} para_term_t;

typedef struct {
	unsigned char valid;
	unsigned char dev_snd;   //定时上报周期
	unsigned char mod_snd;  //0~3依次表示分、时、日、月
	unsigned char base_year;  //上报基准时间: 年
	unsigned char base_month;
	unsigned char base_day;
	unsigned char base_hour;
	unsigned char base_minute;
	unsigned char base_second;
	unsigned char base_week;
} para_task_t;

typedef struct {
	unsigned char project_no;  //地方工程版本标识
	unsigned char uplink;   //上行通信接口
	//unsigned char unuse[3];
	unsigned char num_factory[2]; //厂商编号gq
	unsigned char addr_area[2];  //终端地址-区域码
	unsigned char addr_sn[2];   //终端地址-序号

	unsigned char addr_mac[6];  //终端MAC地址

	char manuno[14];  //终端外部生产编号
	char manuno_inner[12];  //终端内部生产编号
	
	unsigned char serialfunc; //本地串口功能, 0 - tty_shell，1 - 串口上行通信
	unsigned char dooreventflag;  // 查询采集器门节点告警事件，0 - 不需要， 1 - 需要
	unsigned char gprs_chktime;	//利用GPRS拨1008611校时 0 - 不需要， 1- 需要
	unsigned char gprsdialtype; //Mg2636或SIM900模块连接主站方式选择,0-AT命令, 1-PPPD
	// yhf
	unsigned char repair_portstate;
	para_task_t ParaTaskCls1[MAX_DTASK_CLS1];
	unsigned char web_addr[32];	 //F160,主站网址
	unsigned char unuse[1];
} para_uni_t;

typedef struct {
    para_term_t  term;
    para_mix_t   mix;  
    para_commport_t ParaCommPort[MAX_COMMPORT];
    para_uni_t   uniq;
}para_Comb_t;

#ifndef define_para_comb
extern para_Comb_t para_comb;
#endif

void LoadParaComb2(void);

#endif








