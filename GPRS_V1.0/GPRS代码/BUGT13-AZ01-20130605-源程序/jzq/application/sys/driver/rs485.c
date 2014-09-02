/**
* rs485.c -- RS485驱动接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-4-23
* 最后修改时间: 2009-5-9
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
* @brief 锁住Rs485端口
* @param port 端口号
*/
void Rs485Lock(void)
{
	__GlobalUnlock()
	mutex_lock(MUTEX_1);
	__GlobalLock()
}

/**
* @brief 解锁Rs485端口
* @param port 端口号
*/
void Rs485Unlock(void)
{
	mutex_unlock(MUTEX_1);
}

/**
* @brief 复位RS485端口
* @param port RS485端口号
* @return 0成功, 否则失败
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
* @brief 设置RS485端口波特率和属性
* @param port 端口号
* @param baud 波特率
* @param databits 数据位, 5~8
* @param stopbits 停止位, 1~2
* @param parity 校验位
*/
int  Rs485Set(unsigned int port, int baud, int databits, int stopbits, char parity)
{
	  UartSetPara(port,baud,databits,stopbits,parity);
	  return 0;
}

static const int RUMBaudList[8] = {300, 600, 1200, 2400, 4800, 7200, 9600, 19200};

/**
* @brief 设置用户485表端口
* @param port 端口
* @param frame 端口通信控制字
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
* @brief 向RS485端口发送数据
* @param port 端口号
* @param buf 发送缓存区
* @param len 缓存区长度
* @param 0成功, 否则失败
*/
int Rs485Send(unsigned int port, const unsigned char *buf, int len)
{
	AssertLogReturn(port>=RS485_PORTNUM, 1, "invalid port(%d)\n", port);
	AssertLog(len<=0, "invalid len(%d)\n", len);

	return UartSendData(Rs485UartPort[port],buf,len);
}

/**
* @brief 从RS485端口接收数据
* @param port 端口号
* @param buf 接收缓存区
* @param len 缓存区长度
* @return 失败返回-1, 成功返回接收到的字节数, 返回0表示未接收到数据
*/
int Rs485Recv(unsigned int port, unsigned char *buf, int len)
{
	AssertLogReturn(port>=RS485_PORTNUM, -1, "invalid port(%d)\n", port);

	return UartRecvData(Rs485UartPort[port],buf,len);
}
/**
* @brief 清除RS485端口接收缓存
* @param port 端口号
* @return 失败返回-1, 成功返回接收到的字节数, 返回0表示未接收到数据
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
* @brief 初始化RS485端口
* @return 0成功, 否则失败
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


