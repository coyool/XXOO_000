/**
* ir.c -- ���������ͨ��
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-6-12
* ����޸�ʱ��: 2009-6-12
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
* @brief �Ӻ����ͨ�Žӿڶ�ȡһ���ֽ�
* @param buf �����ַ�ָ��
* @return �ɹ�����, ����ʧ��
*/
int IrGetChar(unsigned char *buf,int len)
{
	return UartRecvData(UART_IR,buf,len);
}

/**
* @brief ������ͨ�Žӿڷ�������
* @param buf ���ͻ�����ָ��
* @param len ����������
* @return �ɹ�0, ����ʧ��
*/
int IrRawSend(const unsigned char *buf, int len)
{
	return UartSendData(UART_IR,buf,len);
}

