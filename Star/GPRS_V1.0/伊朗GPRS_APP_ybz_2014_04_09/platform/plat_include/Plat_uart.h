#ifndef _UARTOPEN_H_
#define _UARTOPEN_H_

#define MAX_PORT 5



/*
说明:打开某个指定的串口
输入参数:
port 端口号（0~9）
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int UartOpen(unsigned int port);

/*
说明:关闭一个已打开的串口
输入参数:
port 端口号（0~9）
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
void UartClose(unsigned int port);
/*
说明:设置串口通信参数
输入参数:
port 端口号（0~9），baud 波特率，databits 数据位（5~8），
stopbits 停止位（1~2），parity 校验位（N 无校验，E 偶校验，O奇校验）
输出参数:
无
返回值:
无
*/
void UartSetPara(unsigned int port, int baud, int databits, int stopbits, char parity);

/*
说明:从串口接收数据
输入参数:
port 端口号（0~9），len 缓存区长度
输出参数:
buf 接收缓存区
返回值:
成功 数据长度，失败 -ERRFAILED
*/
int UartRecvData(unsigned int port, unsigned char *buf, int len);

/*
说明:通过串口发送数据
输入参数:
port 端口号（0~9），buf 发送缓存区，len 缓存区长度
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int UartSendData(unsigned int port,const unsigned char *buf, int len);
/*
说明:清接受缓存
*/
void UartClearRecive(unsigned int port);
/*
说明:接受缓存中数据的长度
*/
int UartRecvDataLength(unsigned int port);
/*
	设置调试串口号
*/
void SetDebugPort(unsigned char Port);
#endif
