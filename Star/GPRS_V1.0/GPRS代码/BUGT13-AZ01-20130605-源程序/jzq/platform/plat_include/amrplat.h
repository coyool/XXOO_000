/**
* amrplat.h -- 定义集中抄表控制接口
* 
* 作者: hulijun
*/

#ifndef _AMRPLAT_H
#define _AMRPLAT_H
#include "plat_include/event.h"
#include "app_include/param/capconf.h"

//模块类型No定义
#define AMRMODULE_EASTSOFT 		1	//东软
#define AMRMODULE_ES_III  		2	//东软III代兼容模式
#define AMRMODULE_ES_IV			3	//东软IV代兼容模式
#define AMRMODULE_TOPSCOMM		4	//鼎信
#define AMRMODULE_FXXC 			5	//晓程
#define AMRMODULE_RISECOMM		6	//瑞斯康
#define AMRMODULE_MIRACOMM		7  	//弥亚微
#define AMRMODULE_LME  			8	//力合微
#define AMRMODULE_SUNRAY  		9	//桑锐
#define AMRMODULE_RZTE			10  //新鸿基
#define AMRMODULE_CFDA			11	//友迅达
#define AMRMODULE_USER_485		12  //用户485表
#define AMRMODULE_SOFTROUTE		13   //晓程软路由(国网)
#define AMRMODULE_XC_NW			14  //南网晓程
#define AMRMODULE_ES_7E			15  //东软7E

#define APPPROTO_DL1997			1
#define APPPROTO_DL2007			2


#define AMRMETMAP_HASH_NUM			64
#define AMRMETMAP_HASH_MASK			(AMRMETMAP_HASH_NUM-1)

#define AMRMETMAP_ATTR_INVALID		0  //无效节点
#define AMRMETMAP_ATTR_OWNER  		0x01  //节点属于采集器
#define AMRMETMAP_ATTR_DIRECT		0x02  //节点为直接载波节点(载波表、采集器)

#define AMRMETMAP_TYPE_DIRECTMET 	AMRMETMAP_ATTR_DIRECT  //载波表
#define AMRMETMAP_TYPE_485MET		AMRMETMAP_ATTR_OWNER   //采集器下485表
#define AMRMETMAP_TYPE_OWNER  		(AMRMETMAP_ATTR_OWNER|AMRMETMAP_ATTR_DIRECT)  //采集器

#define AMRMETMAP_FLAG_DEL			0

//载波路由表（最多5级中继）
#define PLC_ROUTENUM    5
typedef struct {
	unsigned char level;  //路由级数
	unsigned char phase;   //0-未指定, 1-a, 2-b,3-c
	unsigned char addr[PLC_ROUTENUM*6];
} plc_route_t;

//目的配置
typedef struct {
	unsigned short metid;  // 从1开始, 0无效
	unsigned short itemid;
	unsigned char portcfg;
	unsigned char proto;
	unsigned char src[6];
	unsigned char dest[6];
	unsigned char owner[6];
	plc_route_t route;
} plc_dest_t;

typedef struct st_usrmet_map {
	//属性, 0-无效
	//BIT0-节点是否属于采集器
	//BIT1-是否直接载波节点(载波表、采集器)
	//01-采集器下485表, 02-载波表, 03-采集器(包含其下第一个电表)
	unsigned char attr;
	unsigned char addr[6];  //表地址
	unsigned char owner_addr[6];  //采集器地址
	unsigned char proto;  //协议, 1-97, 2-07
	unsigned char flag_chg;  //修改标志
	unsigned char reserv;
	struct st_usrmet_map *pbrother; //采集器下485电表链表指针, 载波表无效
	struct st_usrmet_map *methash_next;  //表地址索引散列表指针
	struct st_usrmet_map *ownerhash_next;  //采集器地址索引散列表指针
} usrmet_map_t;

//事件定义
#define AMREVENT_READMET  		0x0001		//实时抄读
#define AMREVENT_CHANGEMET		0x0002		//表档案变更
#define AMREVENT_CTRLCMD  		0x0004 		//控制命令 (重启、暂停、恢复)
#define AMREVENT_CHECKTIME		0x0008		//广播校时
#define AMREVENT_DIRECTFWD		0x0010		//直接透传非645帧
#define AMREVENT_METERREPORT  	0x0020		//表号主动上报(搜表)

#define AMREVENT_ALL				0xffff

#define AMRMOD_EVENT_BUFFER_SIZE 	(512+32)
#define AMRMOD_APP_BUFFER_SIZE		(512+16)
#define AMRMOD_LINK_BUFFER_SIZE		1024

#define AMRMOD_FLAG_ROUTEADDR 		0x0001  //模块376.2路由包含地址
#define AMRMOD_FLAG_USEOWNER  		0x0002  //将采集器地址作为从节点地址(采集器下电表不做从节点)
#define AMRMOD_FLAG_USER485   		0x0004  //纯粹的485端口
#define AMRMOD_FLAG_WLCTRL			0x0008  //微功率无线通信(控制字不同)

//模块同步缓存
typedef struct {
	unsigned short del_num;  //删除数目
	unsigned short add_num;  //添加数目
	unsigned char del_list[6*MAX_METER];  //删除列表
	unsigned short add_list[MAX_METER];  //添加列表
} plc_module_sync_t;


typedef int (*GET_RUNSTATE_PWROFF_CBFUN) (void);//终端停电状态回调函数




//上报抄表相关信息
typedef struct {
	unsigned char routes;		//中继路由级数
	unsigned char channel;		//信道标识
	unsigned char phase;		//载波抄读相位
	unsigned char quality;		//载波信号品质
	unsigned char reserve[4];	//备用
}rep_reading_info;

//串口数据交互接口
typedef struct {
	//获取电表参数 
	//带入:port-端口号， metid-电表序号 
	//返回: *proto-表协议 *addr-表地址 *owneraddr-采集器地址
	int (*pget_metparam)(int port, unsigned short metid, unsigned char *proto, unsigned char *addr, unsigned char *owneraddr);
	//获取抄读内容
	//带入:port-端口号， metid-电表序号 *reqaddr-请求的电表地址
	//返回: *frame-端口通信控制字(1200-8-1-'E') *appbuf-标准的97/07应用报文 *alen-报文长度
	int (*pget_readcontent)(int port, unsigned short metid, unsigned char *reqaddr, unsigned char *frame, unsigned char *appbuf, int *alen);
	//上报数据处理
	//带入:port-端口号， metid-电表序号 proto-表协议 *appbuf-标准的97/07应用报文(电表返回的数据) alen-报文长度 readinginfo-抄读信息
	//返回: int 上报处理结果
	int (*pproc_reportdata)(int port, unsigned short metid, unsigned char proto, unsigned char *appbuf, int alen, rep_reading_info readinginfo);
} AMR_DATAEXEC_FUNC;

typedef struct{
	int channel;		//当前信道
	int netbuilded;	//组网成功标识
	int started;		//已开始组网flag
	int sumnodes;		//总的需组网节点数
	int onlinenodes;	//已组网成功数量
	int count;	 		//时间计数
} AMR_BUILDNET_INFO;

//搜表相关信息
struct search_ctrlparam{
	int ctrlcmd;				//搜表控制  1- 启动搜表 0 - 停止搜表  
	int wait_search_time;		//等待广播搜表结束时间 单位 分钟
	int wait_report_time;		//等待激活上报时间 单位 分钟
	int report_timer;			//激活上报次数
};

typedef struct{
	struct search_ctrlparam ctrlparam;	//控制信息
	int plctrl;			//搜表控制 1 - 需要启动 2 - 需要停止 0 - 无需处理
	int plsearching;	//当前搜表状态 1 - 表示正在搜表中 0 - 未搜表
	int plstat;			//搜表状态 1- 已广播搜表 2 - 已激活上报 
	int plcount; 		//计数
	int plactivetimer;	//已激活上报次数
}AMR_SEARCHING_INFO;

struct amr_module_t {
	int port;  //对应端口号, 1~32, 31为载波
	int module_no;  //模块类型编号
	void (*start_func)(void* arg);  //启动函数

	unsigned int module_flag; //模块处理标志位, PLMOD_FLAG_XXX
	char module_name[16];	 //模块名称
	char module_ver[32];  //模块版本(按序2个字节厂商代码, 2芯片代码-年月日-版本)
	unsigned char module_id[6];  //模块编号

	unsigned short cur_metid;

	char flag_reseting;  //重启抄表
	char flag_stoped;  //停止抄表
	char flag_oldstop;	// 之前停止状态
	int flag_count;	//无通信计数
	char flag_sync;  //需档案同步标志 1 - 只是同步档案  2 - 需复位模块再同步档案
	char flag_busy;  //信道忙标志

	AMR_BUILDNET_INFO buildnetinfo;	//组网相关参数
	AMR_SEARCHING_INFO searchinfo;	//搜表相关信息
		
	unsigned int comm_lock;  //通信锁

	sys_event_t comm_event;  //异步通信事件变量
	unsigned char MeterAddr[6];//表地址
	int event_sendlen;
	int event_echortn;  //异步事件响应结果返回
	int event_echoed;  //异步事件返回标志, 0-未返回, 1-返回
	unsigned char* event_cmd_buf;  //异步通信命令缓存
	unsigned char* event_echo_buf;  //异步通信响应缓存

	int app_proto;  //当前应用层协议, 1-97, 2-07
	int app_sendlen;
	int app_recvlen;
	unsigned char app_send_buf[AMRMOD_APP_BUFFER_SIZE];  //应用数据发送缓存(DL645)
	unsigned char app_recv_buf[AMRMOD_APP_BUFFER_SIZE];  //应用数据接收缓存

	int link_recv_dest_valid;  //链路接收目的地址有效, 0-无效, 1-有效
	plc_dest_t link_recv_dest;  //链路接收目的地址
	int align_pad1;  //对齐
	unsigned char link_send_buf[AMRMOD_LINK_BUFFER_SIZE];  //链路数据发送缓存(376.2)
	unsigned char link_recv_buf[AMRMOD_LINK_BUFFER_SIZE];  //链路数据接收缓存
	unsigned char link_dl_buf[AMRMOD_LINK_BUFFER_SIZE];  //底层使用缓存
	int link_dl_recvlen;  //底层使用
	int link_dl_recvstat;
	int link_dl_recvmax;
	int link_dl_recvtimes;

	AMR_DATAEXEC_FUNC dataexecfunc;

	
	void *priv;  //模块自定义缓存指针
};


#define MAX485METERNUM				50		//采集器下最大电表数

#define METER485READ				0
#define PLMETERREAD					-1				
#define OWNERCOMPLETE				-2

/**
* @brief 新建一个集中抄表控制模块
* @param port 端口号 1~MAX_PORT
* @return 成功返回0, 失败返回-1
*/
struct amr_module_t *NewAmrModule(void);

int AMRModuleStartup(struct amr_module_t *);
int AMRGetModuleName(struct amr_module_t * port, char *modulename);
int AMRGetModuleNo(struct amr_module_t * port);
int AMRGetModuleVersion(struct amr_module_t * port, char *modulever);

int AMRMakePlEvent(struct amr_module_t * port, unsigned long ev, unsigned char *cmddata, int cmdlen, unsigned char *echodata);
int AMRMakePlEventNoWait(struct amr_module_t * port, unsigned long ev);
int AMREchoPlEvent(struct amr_module_t * port, int rtn);
void AMRBroadcastPlEvent(unsigned long ev);


int AMRInitDataExecFun(struct amr_module_t * port, AMR_DATAEXEC_FUNC fun);
void AMRSetGetPwroffStateFun(GET_RUNSTATE_PWROFF_CBFUN fun);
int AMRGetModuleSearchStat(struct amr_module_t * port);

struct amr_module_t *FindAmrModule(int port);
extern GET_RUNSTATE_PWROFF_CBFUN __GetPwrOffState;

void SetupPlMeterAddr(struct amr_module_t *pmodule, const unsigned char *addr);
int ProbeModule(struct amr_module_t *pmodule);
int AutoProbePlcModule(struct amr_module_t *pmodule);
/**
* @brief 通过端口号查找载波控制模块
* @param 端口号
* @return 成功返回载波模块指针, 失败返回NULL
*/
struct amr_module_t *FindAmrModule(int port);
typedef enum {
	REQREAD_SUCCESS = 0,	//正常请求抄读
	REQREAD_DONE,			//请求抄读完成，无需请求
	REQREAD_SKIP,           //跳至下一节点请求
	REQREAD_ERROR,    		//请求测量点通信协议无效
	REQREAD_NOTEXIST,		//不存在该节点
}REQREAD_STATE;

enum amrctrl_cmd {
	AMRCTRL_RESTART = 1,		//重启抄表
	AMRCTRL_PAUSE = 2,		//停止抄表
	AMRCTRL_RESUME = 3,		//恢复抄表
	AMRCTRL_RESETMODULE = 4, //复位模块(恢复后自动重启模块)
};

#define ECHO_OK     0   /* Operation suceed */
#define ERR_FAILPORT      -1 /* 端口不存在 */
#define	ERR_PNULL      	-2 /* 操作指针为空 */
#define	ERR_EXECFAIL      -3 /* 执行错误 */
#define ERR_OUTRANGE		-4 /* 越界 */
#define ERR_COMPARE		-5 /* 比较错误 */
#define ERR_INVALID		-6 /* 无效*/
#define ERR_TIMEOUT		-7 /* 超时错误*/

typedef struct{
	unsigned char addr[6]; //上报的表地址
	unsigned char proto; 	//表协议01 - 97协议 30 - 07协议
	unsigned char reserv;
}report_addr_t;
extern int Dl2007MakeReportAddrPkt(const unsigned char *addr, unsigned char num, report_addr_t *repaddr, unsigned char *buf, int len);

#endif /*_AMRPLAT_H*/

