#include "hal_include/switch.h"
#include "plat_include/Plat_uart.h"
#include "app_include/sys/rs485.h"
#ifdef BIG_CONCENT
int up485_init(void)
{
	Enable485_init();
	UartOpen(UART_UP485);
	UartSetPara(UART_UP485,9600,8,1,'E');
	return 0;
}


int up485_getchar(unsigned char *buf)
{
	return UartRecvData(UART_UP485,buf,1);
}

int up485_send(const unsigned char *buf,int len)
{
	return UartSendData(UART_UP485,buf,len);
}
#endif

