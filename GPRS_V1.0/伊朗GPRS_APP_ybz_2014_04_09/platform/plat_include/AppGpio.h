/**
* gpio -- GPIO�����ӿ�ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-4-23
* ����޸�ʱ��: 2009-4-23
*/

#ifndef _GPIO_H
#define _GPIO_H

#if 0
//GPIO�ܽź�
//x = 0~31
#define GPIO_PA(x)		((unsigned char)0x00|((x)&0x1f))
#define GPIO_PB(x)		((unsigned char)0x20|((x)&0x1f))
#define GPIO_PC(x)		((unsigned char)0x40|((x)&0x1f))
#define GPIO_PD(x)		((unsigned char)0x60|((x)&0x1f))
#endif

/**
* @brief ����GPIO�ܽŷ���
* @param io �ܽź�
* @param direct ����, 0����, 1���
*/
void GpioSetDirect(unsigned char io, int direct);
/**
* @brief ����GPIO�ܽ������˲�
* @param io �ܽź�
* @param ison 0�ر�, 1��
*/
void GpioSetDeglitch(unsigned char io, int ison);
/**
* @brief ����GPIO�ܽ������ƽ
* @param io �ܽź�
* @param value 0�͵�ƽ, 1�ߵ�ƽ
*/
void GpioSetValue(unsigned char io, int value);
/**
* @brief ��ȡGPIO�ܽ������ƽ
* @param io �ܽź�
* @return 0�͵�ƽ, 1�ߵ�ƽ
*/
int GpioGetValue(unsigned char io);
/**
* @brief ����CPU�ڲ�UARTģʽ(����)
* @param port UART�˿ں�
* @param mode ģʽ,1ΪRS485ģʽ
*/
void GpioSetUartMode(int port, int mode);

//�ⲿʱ�Ӹ�ʽ
typedef struct {
	unsigned char century;
	unsigned char year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char week;
} extclock_t;

/**
* @brief ��ȡ�ⲿʱ��
* @param ʱ�ӱ���ָ��
* @return 0�ɹ�, ����ʧ��
*/
int ExtClockRead(extclock_t *clock);

/**
* @brief �����ⲿʱ��
* @param ʱ�ӱ���ָ��
* @return 0�ɹ�, ����ʧ��
*/
int ExtClockWrite(const extclock_t *clock);

/**
* @brief ��鲢У���ⲿʱ������
* @return 0-�ɹ�, -1ʧ��
*/
int ExtClockCheck(void);

/**
* @brief ��ȡ��������汾��Ϣ
*        �汾��Ϣ����:���汾��(1), �ΰ汾��(1), ����������(3)(BCD��ʽ), ��(1)
* @param buf ������ָ��
* @param len ����������
* @return �ɹ����ض�ȡ����(6�ֽ�), ���򷵻�-1
*/
int ReadDriverVersion(unsigned char *buf, int len);

/**
???
*/
int ReadDriverName(unsigned char *buf, int len);

/**
* @brief ��ȡ��ģ����
* @return ��ģ����
*/
unsigned char ReadLeftModuleNo(void);

/**
* @brief ��ȡ��ģ����
* @return ��ģ����
*/
unsigned char ReadRightModuleNo(void);

/**
* @brief ��ȡ�ն��¶�
* @return �ն��¶�, 1=1���϶�
*/
int GpioReadTemperature(void);

#define GPIO_POWER_RMODULE	6
#define GPIO_IGT			4
#define GPIO_GPRS_RESET		5
#define GPIO_GPRS_DTR		26
#define GPIO_GPRS_RTS		28
#define GPIO_GPRS_CTS		29
#define GPIO_GPRS_DCD		25
#define GPIO_GPRS_DSR		24
#define GPIO_GPRS_RI		27
//#define PLCM_NRST			4
//#define GPIO_RS485_CTR	7
#define GPIO_ALARMBEEP	20  //PC11

#define TE_7022SIG_PIN		12
#define TE_POCON_PIN		13

//��չio
#define PLCM_SET			101
#define PLCM_NRST			102
#define AD_SW				103
#define GPIO_CHARGE		109
#define GPIO_SAMPLING		110
#define ESAM_DISABLE		117

typedef struct {
	unsigned short len;
	unsigned char buf[512];
}esam_t;

/**
* @brief ��λESAM
* @return 0�ɹ�, ����ʧ��
*/
int ResetESAM(void);
/**
* @brief ��ESAM���к�
* @param    revBuf  �������ݽӿ�ָ��
* @return 0�ɹ�, ����ʧ��
*/
int ReadESAMSerialNum(unsigned char *revBuf);
/**
* @brief ��ESAM
* @param    etRead  �������ݽӿ�ָ��
* @return 0�ɹ�, ����ʧ��
*/
int ReadESAM(esam_t *rcv );
/**
* @brief ��ESAM
* @param    etRead  д�����ݽӿ�ָ��
* @return 0�ɹ�, ����ʧ��
*/
int WriteESAM(esam_t *cmd);

//#define GPIO_ALARM				GPIO_PC(14)
#if 1

void InitEsamAndSerNum(void);
int SetEsamUartMode(int uartmode);
int SetSysPowerDown(void);
int ReadPowerOffStat(void);
int ReadBatterVolt(int bimm);
int ReadClockBattVolt(unsigned char *buf);
int ReadState(unsigned char *buf);
int SetPowerDown(void);
int SetWatchdog(int flag);
int FeedWatchdog(void);
int ReadRunningSec(void);
#endif

#endif /*_GPIO_H*/
