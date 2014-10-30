/**
* amrplat.h -- ���弯�г�����ƽӿ�
* 
* ����: hulijun
*/

#ifndef _AMRPLAT_H
#define _AMRPLAT_H
#include "plat_include/event.h"
#include "app_include/param/capconf.h"

//ģ������No����
#define AMRMODULE_EASTSOFT 		1	//����
#define AMRMODULE_ES_III  		2	//����III������ģʽ
#define AMRMODULE_ES_IV			3	//����IV������ģʽ
#define AMRMODULE_TOPSCOMM		4	//����
#define AMRMODULE_FXXC 			5	//����
#define AMRMODULE_RISECOMM		6	//��˹��
#define AMRMODULE_MIRACOMM		7  	//����΢
#define AMRMODULE_LME  			8	//����΢
#define AMRMODULE_SUNRAY  		9	//ɣ��
#define AMRMODULE_RZTE			10  //�º��
#define AMRMODULE_CFDA			11	//��Ѹ��
#define AMRMODULE_USER_485		12  //�û�485��
#define AMRMODULE_SOFTROUTE		13   //������·��(����)
#define AMRMODULE_XC_NW			14  //��������
#define AMRMODULE_ES_7E			15  //����7E

#define APPPROTO_DL1997			1
#define APPPROTO_DL2007			2


#define AMRMETMAP_HASH_NUM			64
#define AMRMETMAP_HASH_MASK			(AMRMETMAP_HASH_NUM-1)

#define AMRMETMAP_ATTR_INVALID		0  //��Ч�ڵ�
#define AMRMETMAP_ATTR_OWNER  		0x01  //�ڵ����ڲɼ���
#define AMRMETMAP_ATTR_DIRECT		0x02  //�ڵ�Ϊֱ���ز��ڵ�(�ز����ɼ���)

#define AMRMETMAP_TYPE_DIRECTMET 	AMRMETMAP_ATTR_DIRECT  //�ز���
#define AMRMETMAP_TYPE_485MET		AMRMETMAP_ATTR_OWNER   //�ɼ�����485��
#define AMRMETMAP_TYPE_OWNER  		(AMRMETMAP_ATTR_OWNER|AMRMETMAP_ATTR_DIRECT)  //�ɼ���

#define AMRMETMAP_FLAG_DEL			0

//�ز�·�ɱ����5���м̣�
#define PLC_ROUTENUM    5
typedef struct {
	unsigned char level;  //·�ɼ���
	unsigned char phase;   //0-δָ��, 1-a, 2-b,3-c
	unsigned char addr[PLC_ROUTENUM*6];
} plc_route_t;

//Ŀ������
typedef struct {
	unsigned short metid;  // ��1��ʼ, 0��Ч
	unsigned short itemid;
	unsigned char portcfg;
	unsigned char proto;
	unsigned char src[6];
	unsigned char dest[6];
	unsigned char owner[6];
	plc_route_t route;
} plc_dest_t;

typedef struct st_usrmet_map {
	//����, 0-��Ч
	//BIT0-�ڵ��Ƿ����ڲɼ���
	//BIT1-�Ƿ�ֱ���ز��ڵ�(�ز����ɼ���)
	//01-�ɼ�����485��, 02-�ز���, 03-�ɼ���(�������µ�һ�����)
	unsigned char attr;
	unsigned char addr[6];  //���ַ
	unsigned char owner_addr[6];  //�ɼ�����ַ
	unsigned char proto;  //Э��, 1-97, 2-07
	unsigned char flag_chg;  //�޸ı�־
	unsigned char reserv;
	struct st_usrmet_map *pbrother; //�ɼ�����485�������ָ��, �ز�����Ч
	struct st_usrmet_map *methash_next;  //���ַ����ɢ�б�ָ��
	struct st_usrmet_map *ownerhash_next;  //�ɼ�����ַ����ɢ�б�ָ��
} usrmet_map_t;

//�¼�����
#define AMREVENT_READMET  		0x0001		//ʵʱ����
#define AMREVENT_CHANGEMET		0x0002		//�������
#define AMREVENT_CTRLCMD  		0x0004 		//�������� (��������ͣ���ָ�)
#define AMREVENT_CHECKTIME		0x0008		//�㲥Уʱ
#define AMREVENT_DIRECTFWD		0x0010		//ֱ��͸����645֡
#define AMREVENT_METERREPORT  	0x0020		//��������ϱ�(�ѱ�)

#define AMREVENT_ALL				0xffff

#define AMRMOD_EVENT_BUFFER_SIZE 	(512+32)
#define AMRMOD_APP_BUFFER_SIZE		(512+16)
#define AMRMOD_LINK_BUFFER_SIZE		1024

#define AMRMOD_FLAG_ROUTEADDR 		0x0001  //ģ��376.2·�ɰ�����ַ
#define AMRMOD_FLAG_USEOWNER  		0x0002  //���ɼ�����ַ��Ϊ�ӽڵ��ַ(�ɼ����µ�����ӽڵ�)
#define AMRMOD_FLAG_USER485   		0x0004  //�����485�˿�
#define AMRMOD_FLAG_WLCTRL			0x0008  //΢��������ͨ��(�����ֲ�ͬ)

//ģ��ͬ������
typedef struct {
	unsigned short del_num;  //ɾ����Ŀ
	unsigned short add_num;  //�����Ŀ
	unsigned char del_list[6*MAX_METER];  //ɾ���б�
	unsigned short add_list[MAX_METER];  //����б�
} plc_module_sync_t;


typedef int (*GET_RUNSTATE_PWROFF_CBFUN) (void);//�ն�ͣ��״̬�ص�����




//�ϱ����������Ϣ
typedef struct {
	unsigned char routes;		//�м�·�ɼ���
	unsigned char channel;		//�ŵ���ʶ
	unsigned char phase;		//�ز�������λ
	unsigned char quality;		//�ز��ź�Ʒ��
	unsigned char reserve[4];	//����
}rep_reading_info;

//�������ݽ����ӿ�
typedef struct {
	//��ȡ������ 
	//����:port-�˿ںţ� metid-������ 
	//����: *proto-��Э�� *addr-���ַ *owneraddr-�ɼ�����ַ
	int (*pget_metparam)(int port, unsigned short metid, unsigned char *proto, unsigned char *addr, unsigned char *owneraddr);
	//��ȡ��������
	//����:port-�˿ںţ� metid-������ *reqaddr-����ĵ���ַ
	//����: *frame-�˿�ͨ�ſ�����(1200-8-1-'E') *appbuf-��׼��97/07Ӧ�ñ��� *alen-���ĳ���
	int (*pget_readcontent)(int port, unsigned short metid, unsigned char *reqaddr, unsigned char *frame, unsigned char *appbuf, int *alen);
	//�ϱ����ݴ���
	//����:port-�˿ںţ� metid-������ proto-��Э�� *appbuf-��׼��97/07Ӧ�ñ���(����ص�����) alen-���ĳ��� readinginfo-������Ϣ
	//����: int �ϱ�������
	int (*pproc_reportdata)(int port, unsigned short metid, unsigned char proto, unsigned char *appbuf, int alen, rep_reading_info readinginfo);
} AMR_DATAEXEC_FUNC;

typedef struct{
	int channel;		//��ǰ�ŵ�
	int netbuilded;	//�����ɹ���ʶ
	int started;		//�ѿ�ʼ����flag
	int sumnodes;		//�ܵ��������ڵ���
	int onlinenodes;	//�������ɹ�����
	int count;	 		//ʱ�����
} AMR_BUILDNET_INFO;

//�ѱ������Ϣ
struct search_ctrlparam{
	int ctrlcmd;				//�ѱ����  1- �����ѱ� 0 - ֹͣ�ѱ�  
	int wait_search_time;		//�ȴ��㲥�ѱ����ʱ�� ��λ ����
	int wait_report_time;		//�ȴ������ϱ�ʱ�� ��λ ����
	int report_timer;			//�����ϱ�����
};

typedef struct{
	struct search_ctrlparam ctrlparam;	//������Ϣ
	int plctrl;			//�ѱ���� 1 - ��Ҫ���� 2 - ��Ҫֹͣ 0 - ���账��
	int plsearching;	//��ǰ�ѱ�״̬ 1 - ��ʾ�����ѱ��� 0 - δ�ѱ�
	int plstat;			//�ѱ�״̬ 1- �ѹ㲥�ѱ� 2 - �Ѽ����ϱ� 
	int plcount; 		//����
	int plactivetimer;	//�Ѽ����ϱ�����
}AMR_SEARCHING_INFO;

struct amr_module_t {
	int port;  //��Ӧ�˿ں�, 1~32, 31Ϊ�ز�
	int module_no;  //ģ�����ͱ��
	void (*start_func)(void* arg);  //��������

	unsigned int module_flag; //ģ�鴦���־λ, PLMOD_FLAG_XXX
	char module_name[16];	 //ģ������
	char module_ver[32];  //ģ��汾(����2���ֽڳ��̴���, 2оƬ����-������-�汾)
	unsigned char module_id[6];  //ģ����

	unsigned short cur_metid;

	char flag_reseting;  //��������
	char flag_stoped;  //ֹͣ����
	char flag_oldstop;	// ֮ǰֹͣ״̬
	int flag_count;	//��ͨ�ż���
	char flag_sync;  //�赵��ͬ����־ 1 - ֻ��ͬ������  2 - �踴λģ����ͬ������
	char flag_busy;  //�ŵ�æ��־

	AMR_BUILDNET_INFO buildnetinfo;	//������ز���
	AMR_SEARCHING_INFO searchinfo;	//�ѱ������Ϣ
		
	unsigned int comm_lock;  //ͨ����

	sys_event_t comm_event;  //�첽ͨ���¼�����
	unsigned char MeterAddr[6];//���ַ
	int event_sendlen;
	int event_echortn;  //�첽�¼���Ӧ�������
	int event_echoed;  //�첽�¼����ر�־, 0-δ����, 1-����
	unsigned char* event_cmd_buf;  //�첽ͨ�������
	unsigned char* event_echo_buf;  //�첽ͨ����Ӧ����

	int app_proto;  //��ǰӦ�ò�Э��, 1-97, 2-07
	int app_sendlen;
	int app_recvlen;
	unsigned char app_send_buf[AMRMOD_APP_BUFFER_SIZE];  //Ӧ�����ݷ��ͻ���(DL645)
	unsigned char app_recv_buf[AMRMOD_APP_BUFFER_SIZE];  //Ӧ�����ݽ��ջ���

	int link_recv_dest_valid;  //��·����Ŀ�ĵ�ַ��Ч, 0-��Ч, 1-��Ч
	plc_dest_t link_recv_dest;  //��·����Ŀ�ĵ�ַ
	int align_pad1;  //����
	unsigned char link_send_buf[AMRMOD_LINK_BUFFER_SIZE];  //��·���ݷ��ͻ���(376.2)
	unsigned char link_recv_buf[AMRMOD_LINK_BUFFER_SIZE];  //��·���ݽ��ջ���
	unsigned char link_dl_buf[AMRMOD_LINK_BUFFER_SIZE];  //�ײ�ʹ�û���
	int link_dl_recvlen;  //�ײ�ʹ��
	int link_dl_recvstat;
	int link_dl_recvmax;
	int link_dl_recvtimes;

	AMR_DATAEXEC_FUNC dataexecfunc;

	
	void *priv;  //ģ���Զ��建��ָ��
};


#define MAX485METERNUM				50		//�ɼ������������

#define METER485READ				0
#define PLMETERREAD					-1				
#define OWNERCOMPLETE				-2

/**
* @brief �½�һ�����г������ģ��
* @param port �˿ں� 1~MAX_PORT
* @return �ɹ�����0, ʧ�ܷ���-1
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
* @brief ͨ���˿ںŲ����ز�����ģ��
* @param �˿ں�
* @return �ɹ������ز�ģ��ָ��, ʧ�ܷ���NULL
*/
struct amr_module_t *FindAmrModule(int port);
typedef enum {
	REQREAD_SUCCESS = 0,	//�������󳭶�
	REQREAD_DONE,			//���󳭶���ɣ���������
	REQREAD_SKIP,           //������һ�ڵ�����
	REQREAD_ERROR,    		//���������ͨ��Э����Ч
	REQREAD_NOTEXIST,		//�����ڸýڵ�
}REQREAD_STATE;

enum amrctrl_cmd {
	AMRCTRL_RESTART = 1,		//��������
	AMRCTRL_PAUSE = 2,		//ֹͣ����
	AMRCTRL_RESUME = 3,		//�ָ�����
	AMRCTRL_RESETMODULE = 4, //��λģ��(�ָ����Զ�����ģ��)
};

#define ECHO_OK     0   /* Operation suceed */
#define ERR_FAILPORT      -1 /* �˿ڲ����� */
#define	ERR_PNULL      	-2 /* ����ָ��Ϊ�� */
#define	ERR_EXECFAIL      -3 /* ִ�д��� */
#define ERR_OUTRANGE		-4 /* Խ�� */
#define ERR_COMPARE		-5 /* �Ƚϴ��� */
#define ERR_INVALID		-6 /* ��Ч*/
#define ERR_TIMEOUT		-7 /* ��ʱ����*/

typedef struct{
	unsigned char addr[6]; //�ϱ��ı��ַ
	unsigned char proto; 	//��Э��01 - 97Э�� 30 - 07Э��
	unsigned char reserv;
}report_addr_t;
extern int Dl2007MakeReportAddrPkt(const unsigned char *addr, unsigned char num, report_addr_t *repaddr, unsigned char *buf, int len);

#endif /*_AMRPLAT_H*/

