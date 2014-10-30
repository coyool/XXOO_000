/**
* amrplat.c -- ���г���Ӧ��ƽ̨�ӿ�
* 
* ����: hulijun
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
	int module_no;  //ģ�����ͱ��
	const char *check_name;  //����Ӧ�������
	const char *module_name;	//ģ������
	unsigned int module_flag;  //ģ�鴦���־λ, PLMOD_FLAG_XXX
	void (*start_func)(void* arg);  //��������
};
#if 0
static const struct amr_module_config AmrModuleConfig[] = {
	{AMRMODULE_RISECOMM, "10", "RISECOMM", 0, PlRscStdTask},
	/*{PLMODULE_SOFTROUTECOMM, "YL", 0, PlYlStartup, 0},*/
};
#endif  
GET_RUNSTATE_PWROFF_CBFUN __GetPwrOffState = NULL;

/**
* @brief �½�һ�����г������ģ��
* @param port �˿ں� 1~MAX_PORT
* @return �ɹ�����0, ʧ�ܷ���-1
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
	pmodule->comm_lock = PLC_MUTEX;//�̶�ʹ�������
	SysInitEvent(&pmodule->comm_event);
	mutex_create(pmodule->comm_lock);

	return pmodule;
}

/**
* @brief ͨ���˿ںŲ����ز�����ģ��
* @param �˿ں�
* @return �ɹ������ز�ģ��ָ��, ʧ�ܷ���NULL
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
* @brief ̽���ز�ģ��
* @param port �˿ں� 1~MAX_PORT
* @return �ɹ�����0, ����ʧ��
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
			//Ѱ�ҷ��϶���ߵı�ʶ
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
* @brief ����ģ������
* @param port �˿ں� 1~MAX_PORT
* @return �ɹ�����0, ����ʧ��
*/

int AMRModuleStartup(struct amr_module_t * arg)
{	
	#define TASK_PLC_STK_SIZE 200
	#define TASK_PLC_PRIO 		10
	SysCreateTask(AmrStartTask,arg,"amrtask",-1,TASK_PLC_STK_SIZE,TASK_PLC_PRIO);//PLC 200 10
	return ECHO_OK;
}

/**
* @brief ��ȡģ��İ汾��Ϣ
* @param port �˿ں� 1~MAX_PORT
* @param modulever ģ��汾����
* @return �ɹ�����0, ����ʧ��
*/
int AMRGetModuleVersion(struct amr_module_t * port, char *modulever)
{

	strcpy(modulever, port->module_ver);
	return ECHO_OK;
}

/**
* @brief ��ȡģ�������
* @param port �˿ں� 1~MAX_PORT
* @param modulename ģ�����ƻ���
* @return �ɹ�����0, ����ʧ��
*/
int AMRGetModuleName(struct amr_module_t * port, char *modulename)
{
	strcpy(modulename, port->module_name);
	return ECHO_OK;
}


/**
* @brief ��ȡģ������ͱ��
* @param port �˿ں� 1~MAX_PORT
* @param modulename ģ�����ƻ���
* @return �ɹ��������ͱ��, ����ʧ��
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
* @brief ֪ͨ�ز��̲߳��ȴ����
* @param port �˿ں� 1~MAX_PORT
* @param ev �¼���־
* @param cmddata �¼����ݵ�����
* @param cmdlen �¼����ݵ����ݳ���
* @param echodata �¼����ص�����
* @return ���ؽ�� �������ݳ���
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
* @brief ֪ͨ�ز��߳�(���ȴ�)
* @param port �˿ں� 1~MAX_PORT
* @param ev �¼���־
* @return ����ִ�н��
*/
int AMRMakePlEventNoWait(struct amr_module_t * port, unsigned long ev)
{
	SysSendEvent(&port->comm_event, ev);
 	return ECHO_OK;
}

/**
* @brief ��Ӧ�ز��¼�
* @param pmodule �ز�ģ��ָ��
* @param rtn ��Ӧ���
*/
int AMREchoPlEvent(struct amr_module_t * port, int rtn)
{
	port->event_echortn = rtn;
	port->event_echoed = 1;
	return ECHO_OK;
}

/**
* @brief �㲥�ز��¼�
* @param ev �¼���־
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
* @brief ��ʼ������˿ڵ����ݴ�����غ���
* @param port �˿ں� 1~MAX_PORT
* @param fun ����ͨ����صĲ���������
* @return �ɹ�����0, ����ʧ��
*/
int AMRInitDataExecFun(struct amr_module_t * port, AMR_DATAEXEC_FUNC fun)
{
	port->dataexecfunc = fun;
	return ECHO_OK;
}

/**
* @brief ���õ�ǰͣ��״̬�ص�����
* @param port �˿ں� 1~MAX_PORT
* @param fun  ����
*/
void AMRSetGetPwroffStateFun(GET_RUNSTATE_PWROFF_CBFUN fun)
{
	__GetPwrOffState =  fun;
}

/**
* @brief ��ȡģ����ѱ�״̬
* @param port �˿ں� 1~MAX_PORT
* @param modulename ģ�����ƻ���
* @return �ɹ������ѱ�״̬ 0 - δ�ѱ� 1 - �㲥�ѱ��� 2 - �����ϱ���
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
* @brief ���������ϱ����645֡
* @param addr Ŀ�ĵ�ַ(�ɼ�����ַ)
* @param num 485�����
* @param repaddr ����ַ����֡������ָ��
* @param buf 	���ص����ݻ�����ָ��
* @param len ����������
* @return �ɹ���������֡����, ʧ�ܷ���-1
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

DECLARE_SHELL_CMD("amrpara", shell_amrpara, "��ѯ������");
#endif
#endif


