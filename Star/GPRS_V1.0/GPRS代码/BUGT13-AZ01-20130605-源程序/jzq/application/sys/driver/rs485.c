/**
* rs485.c -- RS485�����ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-4-23
* ����޸�ʱ��: 2009-5-9
*/
#include "plat_include/plat_uart.h"
#include "app_include/sys/rs485.h"
#include "plat_include/debug.h"
#include "plat_include/mutex.h"

static  int Rs485UartPort[RS485_PORTNUM] = {

	UART_DOWN485_1, 
	UART_DOWN485_2, 
};

/**
* @brief ��סRs485�˿�
* @param port �˿ں�
*/
void Rs485Lock(void)
{
	__GlobalUnlock()
	mutex_lock(MUTEX_1);
	__GlobalLock()
}

/**
* @brief ����Rs485�˿�
* @param port �˿ں�
*/
void Rs485Unlock(void)
{
	mutex_unlock(MUTEX_1);
}

/**
* @brief ��λRS485�˿�
* @param port RS485�˿ں�
* @return 0�ɹ�, ����ʧ��
*/
int Rs485Reset(unsigned int port)
{
	return 0;
}
 /*
static struct {
	int baud;
	int databits;
	int stopbits;
	char parity;
} old_uart[RS485_PORTNUM] = {
	{1200, 8, 1, 'E'},
	{1200, 8, 1, 'E'},
};
   */
/**
* @brief ����RS485�˿ڲ����ʺ�����
* @param port �˿ں�
* @param baud ������
* @param databits ����λ, 5~8
* @param stopbits ֹͣλ, 1~2
* @param parity У��λ
*/
int  Rs485Set(unsigned int port, int baud, int databits, int stopbits, char parity)
{
	  UartSetPara(port,baud,databits,stopbits,parity);
	  return 0;
}

static const int RUMBaudList[8] = {300, 600, 1200, 2400, 4800, 7200, 9600, 19200};

/**
* @brief �����û�485��˿�
* @param port �˿�
* @param frame �˿�ͨ�ſ�����
*/
void SetFrame485Port(unsigned int port, unsigned char frame)
{
	int baud, databits, stopbits;
	unsigned char uc;
	char parity;

	uc = (frame>>5)&0x07;
	baud = RUMBaudList[uc];

	databits = (int)(frame&0x03) + 5;
	if(frame & 0x08) {
		if(frame & 0x04) parity = 'O';
		else parity = 'E';
	}
	else parity = 'N';
	if(frame & 0x10) stopbits = 2;
	else stopbits = 1;

	Rs485Set(port, baud, databits, stopbits, parity);
}

/**
* @brief ��RS485�˿ڷ�������
* @param port �˿ں�
* @param buf ���ͻ�����
* @param len ����������
* @param 0�ɹ�, ����ʧ��
*/
int Rs485Send(unsigned int port, const unsigned char *buf, int len)
{
	AssertLogReturn(port>=RS485_PORTNUM, 1, "invalid port(%d)\n", port);
	AssertLog(len<=0, "invalid len(%d)\n", len);

	return UartSendData(Rs485UartPort[port],buf,len);
}

/**
* @brief ��RS485�˿ڽ�������
* @param port �˿ں�
* @param buf ���ջ�����
* @param len ����������
* @return ʧ�ܷ���-1, �ɹ����ؽ��յ����ֽ���, ����0��ʾδ���յ�����
*/
int Rs485Recv(unsigned int port, unsigned char *buf, int len)
{
	AssertLogReturn(port>=RS485_PORTNUM, -1, "invalid port(%d)\n", port);

	return UartRecvData(Rs485UartPort[port],buf,len);
}
/**
* @brief ���RS485�˿ڽ��ջ���
* @param port �˿ں�
* @return ʧ�ܷ���-1, �ɹ����ؽ��յ����ֽ���, ����0��ʾδ���յ�����
*/
void Rs485ClearRecBuf(unsigned int port)
{
	AssertLogReturnVoid(port>=RS485_PORTNUM, "invalid port(%d)\n", port);
	UartClearRecive(Rs485UartPort[port]);	
}
int up485_init(void)
{
	char port;

	for(port=0; port<RS485_PORTNUM; port++) 
	{
		UartClose(Rs485UartPort[port]);
		UartOpen(Rs485UartPort[port]);
	}
	
	return 0;
}


/**
* @brief ��ʼ��RS485�˿�
* @return 0�ɹ�, ����ʧ��
*/
int Rs485Init(void)
{
	char port;
	int rt;
	
	for(port=0; port<RS485_PORTNUM; port++) 
	{
		UartClose(Rs485UartPort[port]);
		UartOpen(Rs485UartPort[port]);
	}
	
    rt = mutex_create(MUTEX_1);
    AssertLog(rt!=0,"mutex creat mutex1 failed!\n");
    return 0;
}

int up485_getchar(unsigned char *buf,int len)
{
	return UartRecvData(UART_UP485,buf,len);
}

int up485_send(const unsigned char *buf,int len)
{
	return UartSendData(UART_UP485,buf,len);
}


