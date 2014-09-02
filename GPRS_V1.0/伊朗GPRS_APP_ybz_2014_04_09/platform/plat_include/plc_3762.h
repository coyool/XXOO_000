/**
* plc_3762.h -- GDW 376.2协议处理接口
* 
* 作者: zhuzhiqiang
*/


#include "plat_include/plcomm.h"
#include "plat_include/amrplat.h"

#ifndef _PLC_3762_H
#define _PLC_3762_H

#define MAX_3762PKT_LEN		1000

typedef struct {
	unsigned short len;  //data len
	unsigned char unuse;
	unsigned char ctrl;
	unsigned char info[6];
	unsigned char afn;
	unsigned char dt[2];
	unsigned char data[MAX_3762PKT_LEN];
} plc3762_pkt_t;

#define FN_DT(fn, dt) { \
	(dt)[0] = (unsigned char)(1 << (((fn)-1)&0x07)); \
	(dt)[1] = (unsigned char)(((fn)-1)>>3); \
}

#define PL3762_AFN_ECHO			0   //确认/否认 
#define PL3762_AFN_INIT			1	//初始化
#define PL3762_AFN_FORWARD		2   //数据转发
#define PL3762_AFN_QRYDATA		3   //查询数据 
#define PL3762_AFN_LINKTEST		4   //链路接口检测
#define PL3762_AFN_CTRLCMD		5   //控制命令 
#define PL3762_AFN_ACTIVESND	6   //主动上报
#define PL3762_AFN_QRYROUTE		0x10  //路由查询
#define PL3762_AFN_SETROUTE		0x11  //路由设置
#define PL3762_AFN_CTRLROUTE	0x12  //路由控制 
#define PL3762_AFN_ROUTEFWD		0x13  //路由数据转发
#define PL3762_AFN_ROUTEREAD	0x14  //路由数据抄读
#define PL3762_AFN_DEBUG		0xf0  //内部调试 

#define PLECHO_ERR_TIMEOUT		0   //0为通信超时
#define PLECHO_ERR_DATA			1   //1为无效数据单元
#define PLECHO_ERR_LENGTH		2   //2为长度错
#define PLECHO_ERR_CHKSUM		3   //3为校验错误
#define PLECHO_ERR_FN			4   //4为信息类不存在
#define PLECHO_ERR_FORMAT		5   //5为格式错误
#define PLECHO_ERR_DUPMET		6   //6为表号重复
#define PLECHO_ERR_METER		7   //7为表号不存在
#define PLECHO_ERR_NOANS		8   //8为电表应用层无应答
#define PLECHO_ERR_TASKFAIL		9   //9为任务启动失败

enum plc_amrctrl_cmd {
	PLC_AMRCTRL_RESTART = 1,	//重启抄表
	PLC_AMRCTRL_PAUSE = 2, //停止抄表
	PLC_AMRCTRL_RESUME = 3,	//恢复抄表
	PLC_AMRCTRL_RESETMODULE = 4, //复位模块(恢复后自动重启模块)
};

struct pl3762_slavenode_info{
	unsigned char addr[6];
	unsigned char info[2];
};

int Pl3762SendPkt(struct amr_module_t *pmodule, const plc_dest_t *dest, int special);
void Pl3762StartRecv(struct amr_module_t *pmodule);
int Pl3762RecvPkt(struct amr_module_t *pmodule);
int Pl3762RecvPktTimeout(struct amr_module_t *pmodule, unsigned char waitafn, unsigned short waitdt, int timeout);

int Pl3762SetRouterMode(struct amr_module_t *pmodule);
int Pl3762ReadModuleAddr(struct amr_module_t *pmodule);
int Pl3762ReadModuleManufactor(struct amr_module_t *pmodule);
void Pl3762SendEchoPkt(struct amr_module_t *pmodule, unsigned char errcode);
void Pl3762DelMeterPause(struct amr_module_t *pmodule, int num, const unsigned char *addr);
int Pl3762AMRCtrl(struct amr_module_t *pmodule, enum plc_amrctrl_cmd cmd);
int Pl3762SyncMeters(struct amr_module_t *pmodule);
int Pl3762ResetModule(struct amr_module_t *pmodule, unsigned char fn);
int Pl3762SetCommand(struct amr_module_t *pmodule, int special);
int Pl3762SetModuleAddr(struct amr_module_t *pmodule, unsigned char *module_addr);
int Pl3762DelMeter(struct amr_module_t *pmodule, int num, const unsigned char *addr);
int Pl3762QueryRouter(struct amr_module_t *pmodule, unsigned char *routerinfo);
int SpecPl3762SyncMeters(struct amr_module_t *pmodule);
void Pl3762DirectForwardProc(struct amr_module_t *pmodule);
void Pl3762GetReadingInfo(struct amr_module_t *pmodule, rep_reading_info *preadinginfo);

//返回错误码
#define PLCERR_INVALID		-1
#define PLCERR_TIMEOUT		-2

#endif /*_PLC_3762_H*/

