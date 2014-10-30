/**
* uplink_pkt.h -- ����ͨ��֡�ṹ
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-18
* ����޸�ʱ��: 2009-5-18
*/

#ifndef _UPLINK_PKT_H
#define _UPLINK_PKT_H

#define LEN_UPLINKDL    8
#define LEN_UPLINKHEAD   8
#define MINLEN_UPLINKPKT    (LEN_UPLINKDL+LEN_UPLINKHEAD)

#define UPLINK_HEAD    0x68
#define UPLINK_TAIL    0x16

#define DLMS_HEAD	0x7E
/*
�����ڷ��������������Ϊ��ȥ������,
��ַ��, AFN, SEQ�ĳ���, ����ȥLEN_UPLINKHEAD
Ϊhead+len1[0]�Ķ�����
*/
#define UPLINKAPP_LEN(pkt)    (*((unsigned short *)(pkt)))
typedef struct {
	unsigned char head;    //=68H
	unsigned char len1[2];
	unsigned char len2[2];
	unsigned char dep;    //=68H
	unsigned char ctrl;    //������C
	unsigned char area[2];
	unsigned char sn[2];
	unsigned char grp;
	unsigned char afn;    //Ӧ�ò㹦����
	unsigned char seq;
	unsigned char data[1022];
} uplink_pkt_t;
#define OFFSET_UPDATA		((int)(((uplink_pkt_t *)0)->data))

#define UPCTRL_DIR		0x80
#define UPCTRL_PRM		0x40
#define UPCTRL_CODE		0x0f

#define UPSEQ_TPV		0x80
#define UPSEQ_FIR		0x40
#define UPSEQ_FIN		0x20
#define UPSEQ_CON		0x10
#define UPSEQ_SEQ		0x0f
#define UPSEQ_SPKT		(UPSEQ_FIR|UPSEQ_FIN)

#define UPPWD_LENGTH	16   //У�����볤��(�ֽ�)

//PRM=1
#define UPCMD_RESET			1    //��λ����
#define UPCMD_USRDATA		4    //�û�����
#define UPCMD_LINKTEST		9    //��·����
#define UPCMD_REQFIRD		10    //����1������
#define UPCMD_REQSECD		11    //����2������
#define UPCMD_SELFDEF		13    //�Զ�������

//PRM=0
#define UPECHO_CONF			0    //�Ͽ�
#define UPECHO_USRDATA		8    //�û�����
#define UPECHO_DENY			9    //����: �����ٻ�������
#define UPECHO_LINKSTAT		11    //��·״̬

#define UPAFN_ECHO			0x00     //ȷ��/����
#define UPAFN_RESET			0x01    //��λ
#define UPAFN_LINKTEST		0x02    //��·�ӿڼ��
#define UPAFN_RELAY			0x03    //�м�վ����
#define UPAFN_SETPARA		0x04    //���ò���
#define UPAFN_CTRL			0x05    //��������
#define UPAFN_KEY			0x06    //��ԿЭ��
#define UPAFN_CASCADE		0x08    //�����������ϱ�
#define UPAFN_REQCFG		0x09    //�����ն�����
#define UPAFN_QRYPARA		0x0a    //��ѯ����
#define UPAFN_QRYTASK		0x0b    //�������ݲ�ѯ
#define UPAFN_QRYCLS1		0x0c    //����1 ������
#define UPAFN_QRYCLS2		0x0d    //����2 ������
#define UPAFN_QRYCLS3		0x0e    //����3 ������
#define UPAFN_TRANFILE		0x0f    //�ļ�����
#define UPAFN_FORWARD		0x10    //����ת��
#define UPAFN_LOGO			0x20    //������Ϣ

#define UPLINK_MAXLEN_FILEDATA    512
#define UPLINK_WINNUM_FILEDATA    32

//ʱ���ǩ
#define LEN_UPLINK_TIMETAG	6
typedef struct {
	unsigned char pfc;
	unsigned char time[4];
	unsigned char dly;
} uplink_timetag_t;

//���ݵ�Ԫ��ʶ
typedef struct {
	unsigned char da[2];    //��Ϣ��
	unsigned char dt[2];    //��Ϣ��
} uplink_duid_t;

//ȷ��/���ϱ���
typedef struct {
	unsigned char da[2];    //��Ϣ��
	unsigned char dt[2];    //��Ϣ��
	unsigned char afn;
	unsigned char data[1];
} uplink_ack_t;

#define UPLINK_PID0(puid)    { \
	((uplink_duid_t *)(puid))->da[0] = ((uplink_duid_t *)(puid))->da[1] = 0; }

#define UPLINK_PID1(id, puid) { \
	((uplink_duid_t *)(puid))->da[0] = (unsigned char)(1<<(((id)-1)&0x07)); \
		((uplink_duid_t *)(puid))->da[1] = (unsigned char)(1<<(((id)-1)>>3)); }

#define UPLINK_FID(id, puid) { \
	((uplink_duid_t *)(puid))->dt[1] = (unsigned char)(((id)-1)>>3); \
	((uplink_duid_t *)(puid))->dt[0] = (unsigned char)(1<<(((id)-1)&0x07)); }

#endif /*_UPLINK_PKT_H*/

