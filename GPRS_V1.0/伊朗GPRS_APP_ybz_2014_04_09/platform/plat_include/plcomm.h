/**
* plccomm.h -- �ز�ͨ�Žӿ�ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-4-24
* ����޸�ʱ��: 2009-4-24
*/

#ifndef _PLCOMM_H
#define _PLCOMM_H
#include "app_include/param/meter.h"

#define COMMPORT_ACMET		0  //���������ӿ�
#define COMMPORT_CEN_485		ParaUni.cenmeter_rs485 // 1  //����˿�(�ܱ�)
#define COMMPORT_USR_485		ParaUni.usrmeter_rs485 // 2  //����˿�(485�ֱ�)
#define COMMPORT_CASCADE		ParaUni.cascade_rs485 // 29  //�����˿�
#define COMMPORT_USR_EXP1 27	//485_1 ��չΪ�ز��˿�
#define COMMPORT_USR_EXP2 28	//485_2 ��չΪ�ز��˿�
#define COMMPORT_PLC		30  //�ز��˿�


#define PLC_UART_PORT		UART_PLC
#define BAUD_PLC	9600
#define PARITY_PLC 'E'

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

#define ECHO_OK     0   /* Operation suceed */
#define ERR_FAILPORT      -1 /* �˿ڲ����� */
#define	ERR_PNULL      	-2 /* ����ָ��Ϊ�� */
#define	ERR_EXECFAIL      -3 /* ִ�д��� */
#define ERR_OUTRANGE		-4 /* Խ�� */
#define ERR_COMPARE		-5 /* �Ƚϴ��� */
#define ERR_INVALID		-6 /* ��Ч*/
#define ERR_TIMEOUT		-7 /* ��ʱ����*/

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
* @brief ��ȡ��Ʋ����޸ı�־
* return 0-δ�޸ģ� 1-�޸�
*/
int GetPlMeterChgFlag(void);
/**
* @brief amr��ʼ������
* @return ����0��ʾ�ɹ�, ����ʧ��
*/

int PlcCommInit(void);

#endif /*_PLCOMM_H*/

