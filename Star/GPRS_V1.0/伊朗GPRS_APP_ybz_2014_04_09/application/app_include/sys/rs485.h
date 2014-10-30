/**
* rs485.c -- RS485�����ӿ�ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-4-24
* ����޸�ʱ��: 2009-4-27
*/

#ifndef _RS485_H
#define _RS485_H
#include "plat_include/debug.h"
//RS485 �˿���
#define RS485_PORTNUM	2
#define UART_UP485 4
/**
* @brief ��λRS485�˿�
* @param port RS485�˿ں�
* @return 0�ɹ�, ����ʧ��
*/
int Rs485Reset(unsigned int port);
/**
* @brief ����RS485�˿ڲ����ʺ�����
* @param port �˿ں�
* @param baud ������
* @param databits ����λ, 5~8
* @param stopbits ֹͣλ, 1~2
* @param parity У��λ
*/
//int  Rs485Set(unsigned int port, int baud, int databits, int stopbits, char parity);
#define  Rs485Set  UartSetPara
/**
* @brief ��RS485�˿ڷ�������
* @param port �˿ں�
* @param buf ���ͻ�����
* @param len ����������
* @param 0�ɹ�, ����ʧ��
*/
//int Rs485Send(unsigned int port, const unsigned char *buf, int len);
#define Rs485Send	UartSendData
/**
* @brief ��RS485�˿ڽ�������
* @param port �˿ں�
* @param buf ���ջ�����
* @param len ����������
* @return ʧ�ܷ���-1, �ɹ����ؽ��յ����ֽ���, ����0��ʾδ���յ�����
*/
//int Rs485Recv(unsigned int port, unsigned char *buf, int len);
#define  Rs485Recv UartRecvData
/**
* @brief ���RS485�˿ڽ��ջ���
* @param port �˿ں�
*/
//void Rs485ClearRecBuf(unsigned int port);
#define  Rs485ClearRecBuf  UartClearRecive
/**
* @brief ��סRs485�˿�
* @param port �˿ں�
*/
void Rs485Lock(void);

/**
* @brief ����Rs485�˿�
* @param port �˿ں�
*/
void Rs485Unlock(void);

#ifdef BIG_CONCENT
//void SetFrame485Port(unsigned int port, unsigned char frame);
/**
* @brief ��ʼ������485
*/
int up485_init(void);

/**
* @brief ��������485����
*/
int up485_getchar(unsigned char *buf);
/**
* @brief ��������485����
*/
int up485_send(const unsigned char *buf,int len);
#endif
#endif /*_RS485_H*/
