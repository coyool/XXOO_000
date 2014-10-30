#ifndef _UARTOPEN_H_
#define _UARTOPEN_H_

#define MAX_PORT 5



/*
˵��:��ĳ��ָ���Ĵ���
�������:
port �˿ںţ�0~9��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int UartOpen(unsigned int port);

/*
˵��:�ر�һ���Ѵ򿪵Ĵ���
�������:
port �˿ںţ�0~9��
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
void UartClose(unsigned int port);
/*
˵��:���ô���ͨ�Ų���
�������:
port �˿ںţ�0~9����baud �����ʣ�databits ����λ��5~8����
stopbits ֹͣλ��1~2����parity У��λ��N ��У�飬E żУ�飬O��У�飩
�������:
��
����ֵ:
��
*/
void UartSetPara(unsigned int port, int baud, int databits, int stopbits, char parity);

/*
˵��:�Ӵ��ڽ�������
�������:
port �˿ںţ�0~9����len ����������
�������:
buf ���ջ�����
����ֵ:
�ɹ� ���ݳ��ȣ�ʧ�� -ERRFAILED
*/
int UartRecvData(unsigned int port, unsigned char *buf, int len);

/*
˵��:ͨ�����ڷ�������
�������:
port �˿ںţ�0~9����buf ���ͻ�������len ����������
�������:
��
����ֵ:
�ɹ� SUCCEED��ʧ�� -ERRFAILED
*/
int UartSendData(unsigned int port,const unsigned char *buf, int len);
/*
˵��:����ܻ���
*/
void UartClearRecive(unsigned int port);
/*
˵��:���ܻ��������ݵĳ���
*/
int UartRecvDataLength(unsigned int port);
/*
	���õ��Դ��ں�
*/
void SetDebugPort(unsigned char Port);
#endif
