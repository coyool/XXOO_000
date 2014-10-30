/**
* ir.c -- 红外口上行通信
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-6-12
* 最后修改时间: 2009-6-12
*/

#include "plat_include/plat_uart.h"
#include "plat_include/sys_config.h"

int IrInit(void)
{
	UartOpen(UART_IR);
	UartSetPara(UART_IR,1200,8,1, 'E');        
    return 0;
}
/**
* @brief 从红外口通信接口读取一个字节
* @param buf 返回字符指针
* @return 成功长度, 否则失败
*/
int IrGetChar(unsigned char *buf,int len)
{
	return UartRecvData(UART_IR,buf,len);
}

/**
* @brief 向红外口通信接口发送数据
* @param buf 发送缓存区指针
* @param len 缓存区长度
* @return 成功0, 否则失败
*/
int IrRawSend(const unsigned char *buf, int len)
{
	return UartSendData(UART_IR,buf,len);
}

