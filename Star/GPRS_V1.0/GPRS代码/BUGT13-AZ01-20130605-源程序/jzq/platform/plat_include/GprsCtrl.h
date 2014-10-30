#ifndef _GPRSCTRL_H_
#define _GPRSCTRL_H_
#ifdef _WIN32
#include <Windows.h>
#else
#include <ucos_ii.h>
#endif
#include "hal_include/switch.h"
//AT�����
struct atlist_struct
{
	const unsigned char *cmd;
	unsigned int keynum;
	const unsigned char *key[4];
	unsigned char times;//���ܴ���0=һ��,1=����
};


#define gprsline_pwroff 	gprs_vcc_on
	
#define gprsline_pwron 	    gprs_vcc_off



#define TickSleep(X) sleep(X)

/**
* ����ATָ���GPRSģ��ͨ��
* @param Port �˿ں�
* @list:�����ͺ���Ӧ��
* @time: ��ʱʱ��
* @return: 0 �ظ���ȷ
*          <0 �ظ�����ȷ
*/
typedef int (* PreReciveFun)(int port,char* buf);//����Ԥ����,������ս���
int SendAtCmd(const struct atlist_struct *list,unsigned int interval,unsigned int time,unsigned char* buf,int buf_len);
/**
* GPRS�������ݰ�,�����ָ����timeout��û���յ����ݣ�����Ϊ����ʧ�ܣ�����յ�һ���ֽں�2��20ms��û���յ���һ���ֽڣ���Ϊ�������
* @param Port �˿ں�
* @param interval:�ֽڼ�ļ��,��IP��ַ���Ǹ�����������յ�OK�����ܾò���CONNECT OK
* @param timeout: ��ʱʱ�䣬һ��ʱ��ͳ�ʱ������
* @param return: ��������֮���յ���һ���ظ��ֽ�֮����ʱ��
*/

//����ʱ��˯������
unsigned int AtCmdRecv(unsigned int interval,unsigned int timeout,unsigned char* atcmd_cache,unsigned int cache_len);

/**
* �ַ�����ת����u8
* @i: u8����
* @return �ַ�����
*/
unsigned char char_to_hex(unsigned char i);

void SetSms(unsigned char Need);
#endif
