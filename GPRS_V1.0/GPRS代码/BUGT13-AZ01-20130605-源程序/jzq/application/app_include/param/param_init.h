#ifndef _PARA_INIT_H
#define _PARA_INIT_H

#include "app_include/param/commport.h"
#include "app_include/param/capconf.h"

//F12, ״̬���������
typedef struct {
	unsigned char flagin;    //�����־λ
	unsigned char flagattr;    //���Ա�־λ
	//unsigned char reserv[2];
} para_isig_t;

#if 0
//F59 ���ܱ��쳣�б���ֵ�趨
typedef struct {
	unsigned char diff;  //������������ֵ
	unsigned char fly;  //���ܱ������ֵ
	unsigned char stop;  //���ܱ�ͣ����ֵ
	unsigned char time;  //���ܱ�Уʱ��ֵ
} cfg_metabnor_t;
#endif

typedef struct {
	para_isig_t isig;//F12�ն�״̬���������
	unsigned char bactsend;  //��ֹ�����ϱ�, 1-��ֹ,0-����ֹ
	unsigned int upflow_max;  //F36 �ն�����ͨ�������������� byte
	//para_commport_t ParaCommPort[MAX_COMMPORT];
//	cfg_metabnor_t metabnor;
} para_mix_t;


//F1, �ն�ͨ�Ų�������
typedef struct {
	unsigned char rts;		//�ն���������ʱʱ��
	unsigned char delay;	//�ն���Ϊ����վ��������ʱʱ��
	unsigned short rsnd;	//�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ����ط�����
	unsigned char flagcon;	//��Ҫ��վȷ�ϵ�ͨ�ŷ���(CON=1)�ı�־
	unsigned char cycka;	//��������, ��
} cfg_tcom_t;

//F2, �ն��м�ת������, ����

//F4, ��վ�绰����Ͷ������ĺ���
typedef struct {
	unsigned char phone[8];
	unsigned char sms[8];
} cfg_smsc_t;

//F5, �ն���������
typedef struct {
	unsigned char art;    //�����㷨���
	unsigned char pwd[2];    //��Կ
} cfg_pass_t;

#ifdef afn04f6
//F6, �ն����ַ����
typedef struct {
	unsigned char addr[16];
} cfg_grpaddr_t;
#endif

//F16, ����ר���û���, ����
typedef struct {
	char user[32];
	char pwd[32];
} cfg_vpn_t;

//F3, ��վIP��ַ�Ͷ˿ں�
typedef struct {
	unsigned char ipmain[4];
	unsigned short portmain;
	unsigned char ipbakup[4];
	unsigned short portbakup;
	char apn[20];
} cfg_svrip_t;
//F7, �ն�IP��ַ�Ͷ˿�
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

//F8, �ն�����ͨ�Ź�����ʽ
typedef struct {
	unsigned char proto;  //0-TCP, 1-UDP
	unsigned char mode;   //0-mix, 1-client, 2-server
	unsigned char clientmode;   //0-��������, 1-��������, 2-ʱ������
	unsigned char countdail;  //�ز�����
	unsigned short timedail;  //�ز���� second
	unsigned char timedown;  //��ͨ���Զ�����ʱ�� minute
	unsigned char unsued;
	unsigned int onlineflag;  //����ʱ�α�־
} cfg_uplink_t;


//F9, �ն��¼���¼����
typedef struct {
	unsigned char valid[8];   //�¼���Ч
	unsigned char rank[8];    //�¼��ȼ�, 1-��Ҫ�¼�, 0-һ���¼�
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

	//char term_name[20];  //F97, �ն�����
} para_term_t;

typedef struct {
	unsigned char valid;
	unsigned char dev_snd;   //��ʱ�ϱ�����
	unsigned char mod_snd;  //0~3���α�ʾ�֡�ʱ���ա���
	unsigned char base_year;  //�ϱ���׼ʱ��: ��
	unsigned char base_month;
	unsigned char base_day;
	unsigned char base_hour;
	unsigned char base_minute;
	unsigned char base_second;
	unsigned char base_week;
} para_task_t;

typedef struct {
	unsigned char project_no;  //�ط����̰汾��ʶ
	unsigned char uplink;   //����ͨ�Žӿ�
	//unsigned char unuse[3];
	unsigned char num_factory[2]; //���̱��gq
	unsigned char addr_area[2];  //�ն˵�ַ-������
	unsigned char addr_sn[2];   //�ն˵�ַ-���

	unsigned char addr_mac[6];  //�ն�MAC��ַ

	char manuno[14];  //�ն��ⲿ�������
	char manuno_inner[12];  //�ն��ڲ��������
	
	unsigned char serialfunc; //���ش��ڹ���, 0 - tty_shell��1 - ��������ͨ��
	unsigned char dooreventflag;  // ��ѯ�ɼ����Žڵ�澯�¼���0 - ����Ҫ�� 1 - ��Ҫ
	unsigned char gprs_chktime;	//����GPRS��1008611Уʱ 0 - ����Ҫ�� 1- ��Ҫ
	unsigned char gprsdialtype; //Mg2636��SIM900ģ��������վ��ʽѡ��,0-AT����, 1-PPPD
	// yhf
	unsigned char repair_portstate;
	para_task_t ParaTaskCls1[MAX_DTASK_CLS1];
	unsigned char web_addr[32];	 //F160,��վ��ַ
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








