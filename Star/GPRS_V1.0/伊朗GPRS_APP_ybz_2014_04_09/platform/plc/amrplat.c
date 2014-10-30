/**
* amrplat.c -- 集中抄表应用平台接口
* 
* 作者: hulijun
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "includes.h"

#include "plat_include/plat_thread.h"
#include "app_include/lib/align.h"
#include "plat_include/amrplat.h"
#include "plat_include/plc_3762.h"
#include "plat_include/plc_rsc_std.h"
#include "plat_include/mutex.h"
#include "plat_include/debug.h"
#include "plat_include/plat_uart.h"
#include "plat_include/sys_config.h"

#ifdef DEFINE_PLCOMM

#define MAX_AMR_MODULE	1
static struct amr_module_t AmrModuleList[MAX_AMR_MODULE];
static int amr_module_index = 0;

struct amr_module_config {
	int module_no;  //模块类型编号
	const char *check_name;  //自适应检查名称
	const char *module_name;	//模块名称
	unsigned int module_flag;  //模块处理标志位, PLMOD_FLAG_XXX
	void (*start_func)(void* arg);  //启动函数
};
#if 0
static const struct amr_module_config AmrModuleConfig[] = {
	{AMRMODULE_RISECOMM, "10", "RISECOMM", 0, PlRscStdTask},
	/*{PLMODULE_SOFTROUTECOMM, "YL", 0, PlYlStartup, 0},*/
};
#endif  
GET_RUNSTATE_PWROFF_CBFUN __GetPwrOffState = NULL;

/**
* @brief 新建一个集中抄表控制模块
* @param port 端口号 1~MAX_PORT
* @return 成功返回0, 失败返回-1
*/
struct amr_module_t *NewAmrModule()
{
	struct amr_module_t *pmodule;
	if(amr_module_index >= MAX_AMR_MODULE)
		return NULL;
	//init
	pmodule = AmrModuleList+amr_module_index++;
	memset(pmodule, 0, sizeof(struct amr_module_t));
	pmodule->port = COMMPORT_DLMS;
	pmodule->priv = NULL;
	pmodule->comm_lock = PLC_MUTEX;//固定使用这个锁
	SysInitEvent(&pmodule->comm_event);
	mutex_create(pmodule->comm_lock);

	return pmodule;
}

/**
* @brief 通过端口号查找载波控制模块
* @param 端口号
* @return 成功返回载波模块指针, 失败返回NULL
*/
struct amr_module_t *FindAmrModule(int port)
{
	int i = 0;
	if(port == -1)
		return 	AmrModuleList;
	for(;i<amr_module_index;i++)
		if(AmrModuleList[i].port == port )
			return &AmrModuleList[i];
	return NULL;	
}
/**
* @brief 探测载波模块
* @param port 端口号 1~MAX_PORT
* @return 成功返回0, 否则失败
*/
/*
int AutoProbePlcModule(struct amr_module_t *pmodule)
{
	int i, retry, pos;
	int okmax, oki, sizec;
	const struct amr_module_config *pconfig;

	
	sizec = sizeof(AmrModuleConfig)/sizeof(AmrModuleConfig[0]);
	for(retry=0; retry<3; retry++) {
		if(!Pl3762ReadModuleManufactor(pmodule)) {
			okmax = -1;
			oki = -1;
			pconfig = AmrModuleConfig;
			//寻找符合度最高的标识
			for(i=0; i<sizec; i++,pconfig++) {
				for(pos=0; 0 != pconfig->check_name[pos]; pos++) {
					if(pconfig->check_name[pos] != pmodule->module_ver[pos]) break;
				}
				if(0 == pconfig->check_name[pos] && pos > okmax) {
					okmax = pos;
					oki = i;
				}
			}
			PrintLog(LOGTYPE_DOWNLINK, "PLC(%d) probed: %c%c%c%c\n", pmodule->port, 
				pmodule->module_ver[1], pmodule->module_ver[0], pmodule->module_ver[3], pmodule->module_ver[2]);

			if(oki >= 0) {
				pmodule->module_no = AmrModuleConfig[oki].module_no;
				pmodule->start_func = AmrModuleConfig[oki].start_func;
				pmodule->module_flag = AmrModuleConfig[oki].module_flag;
				strcpy(pmodule->module_name, AmrModuleConfig[oki].module_name);
				return ECHO_OK;
			}
			else return ERR_INVALID;
		}

		Sleep(100);
	}

	return ERR_TIMEOUT;
}

int ProbeModule(struct amr_module_t *pmodule)
{
	int i, sizec;
	
	sizec = sizeof(AmrModuleConfig)/sizeof(AmrModuleConfig[0]);

	for(i=0; i<sizec; i++) {
		if(AmrModuleConfig[i].module_no == pmodule->module_no){
			pmodule->start_func = AmrModuleConfig[i].start_func;
			pmodule->module_flag = AmrModuleConfig[i].module_flag;
			strcpy(pmodule->module_name, AmrModuleConfig[i].module_name);
			return 0;
		}
	}

	return ERR_EXECFAIL;
}

*/
static void *AmrStartTask(void *arg)
{
	__GlobalLock();
	PlRscStdTask((struct amr_module_t *)arg);
	
//	while(1) Sleep(1000);
	return NULL;
}


/**
* @brief 启动模块任务
* @param port 端口号 1~MAX_PORT
* @return 成功返回0, 否则失败
*/

int AMRModuleStartup(struct amr_module_t * arg)
{	
	#define TASK_PLC_STK_SIZE 200
	#define TASK_PLC_PRIO 		10
	SysCreateTask(AmrStartTask,arg,"amrtask",-1,TASK_PLC_STK_SIZE,TASK_PLC_PRIO);//PLC 200 10
	return ECHO_OK;
}

/**
* @brief 获取模块的版本信息
* @param port 端口号 1~MAX_PORT
* @param modulever 模块版本缓存
* @return 成功返回0, 否则失败
*/
int AMRGetModuleVersion(struct amr_module_t * port, char *modulever)
{

	strcpy(modulever, port->module_ver);
	return ECHO_OK;
}

/**
* @brief 获取模块的名称
* @param port 端口号 1~MAX_PORT
* @param modulename 模块名称缓存
* @return 成功返回0, 否则失败
*/
int AMRGetModuleName(struct amr_module_t * port, char *modulename)
{
	strcpy(modulename, port->module_name);
	return ECHO_OK;
}


/**
* @brief 获取模块的类型编号
* @param port 端口号 1~MAX_PORT
* @param modulename 模块名称缓存
* @return 成功返回类型编号, 否则失败
*/
int AMRGetModuleNo(struct amr_module_t * port)
{

	return port->module_no;

}
void SetupPlMeterAddr(struct amr_module_t *pmodule, const unsigned char *addr)
{
	memcpy(pmodule->MeterAddr,addr,sizeof(pmodule->MeterAddr));
}

/**
* @brief 通知载波线程并等待结果
* @param port 端口号 1~MAX_PORT
* @param ev 事件标志
* @param cmddata 事件传递的数据
* @param cmdlen 事件传递的数据长度
* @param echodata 事件返回的数据
* @return 返回结果 返回数据长度
*/
int AMRMakePlEvent(struct amr_module_t * port, unsigned long ev, unsigned char *cmddata, int cmdlen, unsigned char *echodata)
{
	int rtn = 0,timeout = 30;
	if(port == NULL)
		return 0; 
	mutex_lock(port->comm_lock);

	port->event_echoed = 0;

	port->event_sendlen = cmdlen;
	port->event_cmd_buf = cmddata;
	port->event_echo_buf = echodata;
	
	SysSendEvent(&port->comm_event, ev);
	SysClearCurrentTaskState();

	while(port->event_echoed == 0) 
	{
		SysClearCurrentTaskState();
		timeout--;
		Sleep(20);
		if(timeout == 0)
		{
			goto EXIT;
		}
	}

	port->event_echoed = 0;
	rtn = port->event_echortn;

EXIT:
//	SysClearCurrentTaskState();
	mutex_unlock(port->comm_lock);
	
	return rtn;
}

/**
* @brief 通知载波线程(不等待)
* @param port 端口号 1~MAX_PORT
* @param ev 事件标志
* @return 返回执行结果
*/
int AMRMakePlEventNoWait(struct amr_module_t * port, unsigned long ev)
{
	SysSendEvent(&port->comm_event, ev);
 	return ECHO_OK;
}

/**
* @brief 响应载波事件
* @param pmodule 载波模块指针
* @param rtn 响应结果
*/
int AMREchoPlEvent(struct amr_module_t * port, int rtn)
{
	port->event_echortn = rtn;
	port->event_echoed = 1;
	return ECHO_OK;
}

/**
* @brief 广播载波事件
* @param ev 事件标志
*/
void AMRBroadcastPlEvent(unsigned long ev)
{
#if 0
	amr_module_t *pmodule;
	int port;
	
	for(port=0; port<MAX_PORT; port++){
		pmodule = AmrModuleList[port];
		if(pmodule != NULL) SysSendEvent(&pmodule->comm_event, ev);
	}
#endif
}

/**
* @brief 初始化抄表端口的数据处理相关函数
* @param port 端口号 1~MAX_PORT
* @param fun 串口通信相关的参数及函数
* @return 成功返回0, 否则失败
*/
int AMRInitDataExecFun(struct amr_module_t * port, AMR_DATAEXEC_FUNC fun)
{
	port->dataexecfunc = fun;
	return ECHO_OK;
}

/**
* @brief 设置当前停电状态回调函数
* @param port 端口号 1~MAX_PORT
* @param fun  函数
*/
void AMRSetGetPwroffStateFun(GET_RUNSTATE_PWROFF_CBFUN fun)
{
	__GetPwrOffState =  fun;
}

/**
* @brief 获取模块的搜表状态
* @param port 端口号 1~MAX_PORT
* @param modulename 模块名称缓存
* @return 成功返回搜表状态 0 - 未搜表 1 - 广播搜表中 2 - 激活上报中
*/
int AMRGetModuleSearchStat(struct amr_module_t * port)
{
	int rtn = 0;
	if(port->searchinfo.plsearching)
	{
		rtn = port->searchinfo.plstat;
	}
	else 
		rtn = 0;
	return rtn;
}




/**
* @brief 生成主动上报表号645帧
* @param addr 目的地址(采集器地址)
* @param num 485表个数
* @param repaddr 电表地址数据帧缓存区指针
* @param buf 	返回的数据缓存区指针
* @param len 缓存区长度
* @return 成功返回数据帧长度, 失败返回-1
*/
int Dl2007MakeReportAddrPkt(const unsigned char *addr, unsigned char num, report_addr_t *repaddr, unsigned char *buf, int len)
{
	unsigned char check, *puc;
	int i, j;

	AssertLogReturn(len < 16, -1, "too short buffer len(%d)\n", len);
	if(num<=0 || num > 16){
		PrintLog(LOGTYPE_DOWNLINK, "Make report addr num(%d) is too long!\n", num);
		return -1;
	}
	
	puc = buf;
	*puc++ = 0x68;
	for(i=0; i<6; i++) *puc++ = addr[i];
	*puc++ = 0x68;
	*puc++ = 0x91;
	*puc++ = 4 + 1 + 7*num;
	DEPART_LONG(0xacacacac, puc);
	for(i=0; i<4; i++) puc[i] += 0x33;
	puc += 4;
	*puc++ = num + 0x33;
	
	for(i=0; i<num; i++){
		smallcpy(puc, repaddr[i].addr, 6);
		for(j=0; j<6; j++) puc[j] += 0x33;
		puc += 6;
		*puc++ = repaddr[i].proto + 0x33;
	}

	puc = buf;
	check = 0;
	for(i=0; i<(14+1+7*num); i++) check += *puc++;
	*puc++ = check;
	*puc = 0x16;

	return (14+1+7*num +2);
}
#if 0
static int shell_amrpara(int argc, char *argv[])
{
	int metid;
	int port;
	amr_module_t *pmodule;
	usrmet_map_t *pmap;
	
	if(argc < 2) goto mark_invalid;
	port = atoi(argv[1]);

	if(port<=0 || port>MAX_PORT) goto mark_invalid;
	if(NULL == AmrModuleList[port-1]) goto mark_invalid;

	pmodule = AmrModuleList[port-1];

	pmap = AmrModuleList[port-1]->met_map;

	for(metid=0; metid <MAX_METER; metid++, pmap++){
		if(!pmap->attr) continue;

		DebugPrintLog(0, "met_map attr=%d proto=%d ", pmap->attr, pmap->proto);
		DebugPrintLog(0, "addr:%02X%02X%02X%02X%02X%02X", pmap->addr[5], pmap->addr[4],
			pmap->addr[3], pmap->addr[2],pmap->addr[1], pmap->addr[0]);
		DebugPrintLog(0, "owner:%02X%02X%02X%02X%02X%02X\n", pmap->owner_addr[5], pmap->owner_addr[4],
			pmap->owner_addr[3], pmap->owner_addr[2],pmap->owner_addr[1], pmap->owner_addr[0]);
	}
	
	return 0;

mark_invalid:
	DebugPrintLog(0, "usage: amrpara [port] <1~31>\n");
	return 1;
}

DECLARE_SHELL_CMD("amrpara", shell_amrpara, "查询抄表档案");
#endif
#endif


