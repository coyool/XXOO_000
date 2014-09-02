#include <stdio.h>
#include <string.h>
#include "plat_include/plat_defines.h"
#include "plat_include/Plat_uart.h"
#include "plat_include/GprsCtrl.h"
#include "plat_include/M35gprs.h"
#include "plat_include/sys_config.h"
#include "plat_include/sleep.h"
#include "plat_include/debug.h"
static unsigned char sms_f = 0;

void SetSms(unsigned char Need)
{  
	 sms_f = Need;
}

/**
* GPRS�������ݰ�,�����ָ����timeout��û���յ����ݣ�����Ϊ����ʧ�ܣ�����յ�һ���ֽں�2��20ms��û���յ���һ���ֽڣ���Ϊ�������
* @param Port �˿ں�
* @param interval:�ֽڼ�ļ��,��IP��ַ���Ǹ�����������յ�OK�����ܾò���CONNECT OK
* @param timeout: ��ʱʱ�䣬һ��ʱ��ͳ�ʱ������
* @param return: ��������֮���յ���һ���ظ��ֽ�֮����ʱ��
*/
//����ʱ��˯������
#define RECV_DELAY	 1
static const char* er = "\r\nERROR"; 
static const char* cm = "\r\n+CMTI"; 

unsigned int AtCmdRecv(unsigned int interval,unsigned int timeout,unsigned char* atcmd_cache,unsigned int cache_len)
{
	unsigned int bytes = 0;
	unsigned int n;
	unsigned int time_c = 0;
	unsigned int step=0;
	interval/=RECV_DELAY;
	timeout/=RECV_DELAY;

	while(1){//һ�������û���յ������ˣ�����Ϊ����������
		//������ȳ�����Ҫ���ײ�Ļ�����꣬��������CPU�ٶ�Զ���ڲ����ʣ������������յ�����������
		if(bytes>=cache_len){
			break;
		}
		n=UartRecvData(GPRS_PORT,&atcmd_cache[bytes],cache_len-bytes);
		switch(step){
		case 0:
			if(n==0){
				time_c++;
				if(time_c>=timeout)
					return 0;
			}else{
				step = 1;		//�յ�һ�����ݣ���ʼ��һ������
				time_c=0;		//���ʱ������Ϊ��һ����Ľ��ռ�ʱ
				bytes=n;		//���յ���һ���ֽ�
			}
			break;
		case 1:
			if(0==n){
				time_c++;
				if(time_c>=interval)	//����40msû���յ����ݣ���Ϊ�������
					return bytes;
			}else{
				time_c = 0;
				bytes += n;
				if(bytes >= 7)
				{
					if(memcmp(atcmd_cache,er,7) == 0)
					{
						return 0;
					}
					if(sms_f)
					{
						if(memcmp(atcmd_cache,cm,6) == 0)
						{
							return 0;
						}
					}	
				}
			}
			break;
		default:
			return 0;
		}

		TickSleep(RECV_DELAY);

	}
	//ֻ�н��ճ��ȳ������������ȲŻᵽ��ʱ����ѭ������Ҫ����ʣ������ݶ���
	UartClearRecive(GPRS_PORT);	//����������������������
	return bytes;
}
/**
* ����ATָ���GPRSģ��ͨ��
* @param Port �˿ں�
* @list:�����ͺ���Ӧ��
* @time: ��ʱʱ��
* @return: 0 �ظ���ȷ
*          <0 �ظ�����ȷ
*/
int SendAtCmd(const struct atlist_struct *list,unsigned int interval,unsigned int time,unsigned char* buf,int buf_len)
{
	unsigned int i,k=0;
	unsigned char* pbuf = buf;
	UartClearRecive(GPRS_PORT);
	memset(buf,0,buf_len);
	UartSendData(GPRS_PORT,list->cmd,strlen((const char *)list->cmd));
	PrintLog(LOGTYPE_DEBUG,"GPRS send: ");
	PrintLog(LOGTYPE_DEBUG,(char*)list->cmd);

	while (k<(unsigned int)(list->times+1))
	{
		i = AtCmdRecv(interval,time,pbuf,buf_len);
		pbuf +=i;
		buf_len -= i;

		PrintLog(LOGTYPE_DEBUG,"GPRS receive: ");
		PrintLog(LOGTYPE_DEBUG,(char*)buf);
		if(i==0)
		{
			PrintLog(LOGTYPE_DEBUG,"\r\n");
			return -1;
		}
		for(i=0;i<list->keynum;i++)
		{
			if(NULL != strstr((char*)buf,(char*)list->key[i]))
			{
				return i;
			}
		}
		k++;
	}


	return -1;
}

