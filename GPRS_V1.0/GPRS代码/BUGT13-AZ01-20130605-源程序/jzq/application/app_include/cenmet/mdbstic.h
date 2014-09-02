/**
* mdbstic.h -- ͳ������
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-14
* ����޸�ʱ��: 2009-5-14
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
	unsigned int pwramax[4];    //����й����� 0.1W
	stictime_t pwramax_time[4];    //����й����ʷ���ʱ��
	unsigned short pwrzero_time[4];    //�й�����Ϊ��ʱ��, ��

	unsigned int dmmax[4];   //����������� 0.1W
	stictime_t dmtime[4];   //���������������ʱ��

	unsigned short vol_overtime[3];    //��ѹ�ۼ�ʱ��
	unsigned short vol_lesstime[3];    //Ƿѹ�ۼ�ʱ��
	unsigned short vol_uptime[3];    //Խ�����ۼ�ʱ��
	unsigned short vol_lowtime[3];    //Խ�����ۼ�ʱ��
	unsigned short vol_oktime[3];    //�ϸ��ۼ�ʱ��

	unsigned short volmax[3];
	unsigned short volmin[3];
	stictime_t volmax_time[3];
	stictime_t volmin_time[3];
	unsigned int volsum[3];    //ƽ����ѹ
	unsigned short volsnum[3];

	unsigned short ampunb_time;   //������ƽ���ۼ�ʱ��
	unsigned short volunb_time;   //��ѹ��ƽ���ۼ�ʱ��
	unsigned short ampunb_max;    //������ƽ�����ֵ,%
	unsigned short volunb_max;    //��ѹ��ƽ�����ֵ,%
	stictime_t ampunbmax_time;    //������ƽ�����ֵ����ʱ��
	stictime_t volunbmax_time;    //��ѹ��ƽ�����ֵ����ʱ��

	unsigned short ampover_time[3];   //�����ۼ�ʱ��
	unsigned short ampup_time[3];   //����Խ���ۼ�ʱ��
	unsigned short zampup_time;   //�������Խ���ۼ�ʱ��
	unsigned int ampmax[4];    //�������ֵ
	stictime_t ampmax_time[4];   //�������ֵ����ʱ��

	unsigned short pwrvov_time;    //���ڹ���Խ�������ۼ�ʱ��
	unsigned short pwrvup_time;    //���ڹ���Խ�����ۼ�ʱ��

	unsigned short pwrf_time[3];   //�������������ۼ�ʱ��

	unsigned short load_max;  //���������ֵ,%
	unsigned short load_min;  //��������Сֵ,%
	stictime_t loadmax_time;  //���������ֵ����ʱ��
	stictime_t loadmin_time;  //��������Сֵ����ʱ��

	unsigned short amplost_time[3];   //ʧ���ۼ�ʱ��
	unsigned short ampbreak_time[3];  //�����ۼ�ʱ�� (������·)
} metpstic_t;

typedef struct {
	unsigned short amp_max[19];  //����2-19��г�����������ֵ
	unsigned short vol_max[19];  //����2-19��г����ѹ�����ʼ��ܻ����������ֵ
	stictime_t ampmax_time[19];  //����2-19��г�����������ֵ����ʱ��
	stictime_t volmax_time[19];  //����2-19��г����ѹ�����ʼ��ܻ����������ֵ����ʱ��
	unsigned short volup_times[19];  //����г����ѹ������Խ�����ۼ�ʱ��
	unsigned short ampup_times[19];  //����г������Խ�����ۼ�ʱ��
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

	//@add later: δ�������ۼ�
	unsigned short pcov_time;   //�����ʶ�ֵ���ۼ�ʱ�估�ۼƵ���
	unsigned short ecov_time;  //���µ�����ֵ���ۼ�ʱ�估�ۼƵ���
	int pcov_ene;
	int ecov_ene;
} tgrpstic_mon_t;

typedef struct {
	unsigned short up_time;  //Խ�����ۼ�ʱ��, min
	unsigned short dwn_time;  //Խ�����ۼ�ʱ��
	int max_v;  //���ֵ , 0.01
	int min_v;  //��Сֵ , 0.01
	stictime_t max_time;
	stictime_t min_time;
} anasig_stic_t;
#endif

typedef struct {
	unsigned short pwr_time;   //����ʱ��
	unsigned short rst_cnt;   //��λ����
	unsigned char sw_cnt[4];   //��բ�ۼƴ���
	unsigned int comm_bytes;  //�ն�����վͨ������
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
	unsigned char upflow_flag;  //����վͨ���������ޱ�־,0-δ����, 1-����
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

