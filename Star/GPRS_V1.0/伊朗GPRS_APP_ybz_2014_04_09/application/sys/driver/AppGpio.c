/**
* gpio.c -- GPIO�����ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-4-23
* ����޸�ʱ��: 2009-4-23
*/

//#include <stdio.h>
#include <string.h>

#include "include/debug.h"
#include "include/sys/AppGpio.h"
#include "include/sys/timeal.h"
#include "include/sys/mutex.h"
//#include "include/sys/schedule.h"

typedef union {
	unsigned long ul;
	struct {
		unsigned char pin;
		unsigned char value;
	} ch;
} gpio_arg_t;

typedef struct {
	unsigned char in_or_out[3];
	unsigned char plority[3];
}port_config;

static int FidGpio = -1;
static int FidTmp = -1;
static int FidRtc = -1;
static int FidEsam = -1;
static int FidKey = -1;
static int FidAdc[2] = {-1, -1};
static int FidWdg = -1;
static int FidAds = -1;
static int FidTcaio = -1;	//����io��

/**
* @brief GPIO�����ӿڳ�ʼ��
* @return 0�ɹ�, ����ʧ��
*/
int GpioInit(void)
{
	/*interface*/
	return 0;
}

/**
* @brief ����GPIO�ܽŷ���
* @param io �ܽź�
* @param direct ����, 0����, 1���
*/
void GpioSetDirect(unsigned char io, int direct)
{
	/*interface*/
	return;
}

/**
* @brief ����GPIO�ܽ������˲�
* @param io �ܽź�
* @param ison 0�ر�, 1��
*/
void GpioSetDeglitch(unsigned char io, int ison)
{
	return;
}

/**
* @brief ����GPIO�ܽ������ƽ
* @param io �ܽź�
* @param value 0�͵�ƽ, 1�ߵ�ƽ
*/
void GpioSetValue(unsigned char io, int value)
{
	/*interface*/
	return;
}

/**
* @brief ��ȡGPIO�ܽ������ƽ
* @param io �ܽź�
* @return 0�͵�ƽ, 1�ߵ�ƽ
*/
int GpioGetValue(unsigned char io)
{
	/*interface*/
	return 0;
}

/**
* @brief ����CPU�ڲ�UARTģʽ(����)
* @param port UART�˿ں�
* @param mode ģʽ,1ΪRS485ģʽ
*/
void GpioSetUartMode(int port, int mode)
{
	return;
}

/**
* @brief ��ȡ�ն��¶�
* @return �ն��¶�, 1=1���϶�
*/
int GpioReadTemperature(void)
{
	/*interface*/
	return 0;
}

/**
* @brief ��ȡ�ⲿʱ��
* @param ʱ�ӱ���ָ��
* @return 0�ɹ�, ����ʧ��
*/
int ExtClockRead(struct sysclock *clock)
{
	/*interface*/
	return 0;
}

/**
* @brief �����ⲿʱ��
* @param ʱ�ӱ���ָ��
* @return 0�ɹ�, ����ʧ��
*/
int ExtClockWrite(const extclock_t *clock)
{
	/*interface*/
	return 0;
}

/**
* @brief ��鲢У���ⲿʱ������
* @return 0-�ɹ�, -1ʧ��
*/
/*int ExtClockCheck(void)
{
	AssertLog(-1==FidGpio, "invalid fid(%d)\n", FidGpio);

	if(0 != ioctl(FidGpio, 19, 0)) {
		DebugPrint(LOGTYPE_ALARM, "check ext clock error!!\n");
		return ioctl(FidGpio, 18, 0);
	}

	return 0;
}*/

/**
* @brief ��ȡ��������汾��Ϣ
*        �汾��Ϣ����:���汾��(1), �ΰ汾��(1), ����������(3)(BCD��ʽ), ��(1)
* @param buf ������ָ��
* @param len ����������
* @return �ɹ����ض�ȡ����(6�ֽ�), ���򷵻�-1
*/
int ReadDriverVersion(unsigned char *buf, int len)
{
	/*interface*/
	return 0;
}

/**
* @brief ��ȡ��������汾����
* @param buf ������ָ��
* @param len ����������
* @return �ɹ����ض�ȡ����(6�ֽ�), ���򷵻�-1
*/
int ReadDriverName(unsigned char *buf, int len)
{
	/*interface*/
	return 0;
}

/**
* @brief ��ȡ��ģ����
* @return ��ģ����
*/
unsigned char ReadLeftModuleNo(void)
{
	return 0;
}

/**
* @brief ��ȡ��ģ����
* @return ��ģ����
*/
unsigned char ReadRightModuleNo(void)
{
	return 0;
}
/**
* @brief ����ϵͳͣ��
* @return 0-�ɹ�, 1-ʧ��
*/
int SetSysPowerDown(void)
{
	/*interface*/
	return 0;
}

/**
* @brief ������
* @return 0-����, 1-�е�״̬
*/
int ReadPowerOffStat(void)
{
	/*interface*/
	return 0;
}

/**
* @brief ����ESAM����ģʽ
* @param uartmode 1-7816_T0 2 - 7816_T1  3 - USART 
* @return �ɹ����ض�ȡ����(6�ֽ�), ���򷵻�-1
*/

int SetEsamUartMode(int uartmode)
{
	/*interface*/
	return 0;
}

/**
* @brief ��λESAM
* @return 0�ɹ�, ����ʧ��
*/
int ResetESAM(void)
{
	/*interface*/
	return 0;
}

static int EsamUartRecv(unsigned char *buf, int timeout)
{
	unsigned char *puc = buf;
	int i, recvlen = 0;

	recvlen = 0;
	while(timeout) {
		Sleep(10);
		while(1){
			///i = EsamRecv(puc, 1);
			if(i<=0) break;

			timeout = 15;
			
			puc++;
			recvlen++;
			if(recvlen >= 255) break;
		}
		
		if(recvlen >= 255) break;
		timeout--;
	}
	
	return recvlen;
}

static unsigned char SerialNum[8] = {0};

/**
* @brief ��ESAM���к�
* @param    revBuf  �������ݽӿ�ָ��
* @return 0�ɹ�, ����ʧ��
*/
int ReadESAMSerialNum(unsigned char *revBuf)
{
	smallcpy(revBuf, SerialNum, 8);

	return 0;
}

int Esam_PPSInit(void)
{
	int len;
	unsigned char ruc[32];
	unsigned char uc[] = {0xff,0x10,0x94,0x7b};

	///EsamSend(uc, 4);

	len = EsamUartRecv(ruc, 30);

	if(len<4) return 1;

	if((0xff==ruc[len-4]) && (0x10==ruc[len-3]) && (0x94==ruc[len-2]) && (0x7b== 
	ruc[len-1])) return 0;

	return 1;	
}

/**
* @brief ��λ�Ҷ�ESAM���к�
* @param    revBuf  �������ݽӿ�ָ��
* @return 0�ɹ�, ����ʧ��
*/
void InitEsamAndSerNum(void)
{
	/*interface*/
	return ;
}

/**
* @brief ��ESAM
* @param    etRead  �������ݽӿ�ָ��
* @return 0�ɹ�, ����ʧ��
*/
int ReadESAM(esam_t *etRead )
{
	int rtn;

	AssertLog(-1==FidEsam, "invalid fid(%d)\n", FidEsam);

	rtn = EsamUartRecv(etRead->buf, 100);

	etRead->len = (unsigned short)rtn;

	return 0;
}
/**
* @brief дESAM
* @param    etRead  д�����ݽӿ�ָ��
* @return 0�ɹ�, ����ʧ��
*/
int WriteESAM(esam_t *cmd){

	esam_t et,etRev ;
	int rtn;
	AssertLog(-1==FidEsam, "invalid fid(%d)\n", FidEsam);

	et.len = cmd->len;
	memcpy(et.buf,cmd->buf,cmd->len);

	if(et.len>5) {
		et.len = 5;

		///rtn = EsamSend(et.buf, et.len);
		if(rtn == -1) {
			ErrorLog("set esam data fail\n");
			return rtn;
		}
		
		rtn = EsamUartRecv(etRev.buf, 30);
		if(0 == rtn){
			ErrorLog("read esam data fail\n");
			return rtn;
		}

		memcpy(et.buf,cmd->buf+5,cmd->len-5);
		et.len =cmd->len - 5;

		///rtn = EsamSend(et.buf, et.len);
		if(-1 == rtn){
			ErrorLog("set esam data fail\n");
			return rtn;
		}
	}
	else{
		///rtn = EsamSend(et.buf, et.len);
		if(-1 == rtn){
			ErrorLog("set esam data fail\n");
			return rtn;
		}
	}

	return 0;
}

/**
* @brief ��ȡ���õ�ص�ѹ
* @brief bimm 1-������ȡ, 0-������ȡ
* @return ��ѹֵ, -1-ʧ��
*/
int ReadBatterVolt(int bimm)
{
	/*interface*/
	return 0;
}

/**
* @brief ��ȡʱ�ӵ�ص�ѹ
* @param buf ���ݻ�����ָ��
* @return 0-�ɹ�, 1-ʧ��
*/
int ReadClockBattVolt(unsigned char *buf)
{
	/*interface*/
	return 0;
}

/**
* @brief ι���Ź�
* @return 0-�ɹ�, 1-ʧ��
*/
int FeedWatchdog(void)
{
	/*interface*/
	return 0;
}

/**
* @brief ��ȡ��������
* @return �������������� -1 - ��ʾ��ȡʧ��
*/
int ReadRunningSec(void)
{
	/*interface*/
	return 0;
}

