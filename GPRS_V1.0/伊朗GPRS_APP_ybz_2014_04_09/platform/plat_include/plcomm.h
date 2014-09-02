/**
* plccomm.h -- 载波通信接口头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-4-24
* 最后修改时间: 2009-4-24
*/

#ifndef _PLCOMM_H
#define _PLCOMM_H
#include "app_include/param/meter.h"

#define COMMPORT_ACMET		0  //交流采样接口
#define COMMPORT_CEN_485		ParaUni.cenmeter_rs485 // 1  //抄表端口(总表)
#define COMMPORT_USR_485		ParaUni.usrmeter_rs485 // 2  //抄表端口(485分表)
#define COMMPORT_CASCADE		ParaUni.cascade_rs485 // 29  //级联端口
#define COMMPORT_USR_EXP1 27	//485_1 扩展为载波端口
#define COMMPORT_USR_EXP2 28	//485_2 扩展为载波端口
#define COMMPORT_PLC		30  //载波端口


#define PLC_UART_PORT		UART_PLC
#define BAUD_PLC	9600
#define PARITY_PLC 'E'

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

#define ECHO_OK     0   /* Operation suceed */
#define ERR_FAILPORT      -1 /* 端口不存在 */
#define	ERR_PNULL      	-2 /* 操作指针为空 */
#define	ERR_EXECFAIL      -3 /* 执行错误 */
#define ERR_OUTRANGE		-4 /* 越界 */
#define ERR_COMPARE		-5 /* 比较错误 */
#define ERR_INVALID		-6 /* 无效*/
#define ERR_TIMEOUT		-7 /* 超时错误*/

#define APPPROTO_DL1997			1
#define APPPROTO_DL2007			2

struct plwrite_config_t{
	unsigned short itemid;
	unsigned char *pwd;
	int pwdlen;
	const unsigned char *command;
	int cmdlen;
}; 





typedef struct {
	unsigned char head;
	unsigned char addr[6];
	unsigned char dep;
	unsigned char cmd;
	unsigned char len;
	unsigned char data[2];
} dl645_module_t;

#define DL645PKTLEN_MIN 		12
#define DL645PKT_HEAD   		0x68
#define DL645PKT_TAIL   		0x16

//int PlCtrlCmd(enum amrctrl_cmd amrcmd, int port);
//int PlUserMetCtrlCmd(enum amrctrl_cmd amrcmd);
int PlReadMeter(unsigned short metid, unsigned short itemid, unsigned char *buf, int len);
 
void PlCheckTimeInit(void);
int PlCheckTime(int port);
int PlStartLearnRoute(void);

extern int PlStdResetModule(unsigned char fn);
extern int GetPlModuleVersion(char *module_ver);

/**
* @brief 获取表计参数修改标志
* return 0-未修改， 1-修改
*/
int GetPlMeterChgFlag(void);
/**
* @brief amr初始化函数
* @return 返回0表示成功, 否则失败
*/

int PlcCommInit(void);

#endif /*_PLCOMM_H*/

