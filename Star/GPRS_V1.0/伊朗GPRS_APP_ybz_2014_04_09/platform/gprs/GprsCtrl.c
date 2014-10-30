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
* GPRS接收数据包,如果在指定的timeout内没有收到数据，就认为接收失败，如果收到一个字节后2个20ms内没有收到下一个字节，认为接收完成
* @param Port 端口号
* @param interval:字节间的间隔,设IP地址的那个命令，就是先收到OK，过很久才收CONNECT OK
* @param timeout: 延时时间，一般时间和长时间两种
* @param return: 发送命令之后收到第一个回复字节之间的最长时间
*/
//接收时的睡眠周期
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

	while(1){//一定间隔内没有收到数据了，即认为这次数据完成
		//如果长度超出，要将底层的缓冲读完，否则由于CPU速度远大于波特率，后面的命令会收到残留的数据
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
				step = 1;		//收到一个数据，开始下一个步骤
				time_c=0;		//清计时器，作为下一步骤的接收计时
				bytes=n;		//接收到第一个字节
			}
			break;
		case 1:
			if(0==n){
				time_c++;
				if(time_c>=interval)	//接连40ms没有收到数据，认为接收完毕
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
	//只有接收长度超过缓冲区长度才会到这时，本循环就是要收完剩余的数据丢弃
	UartClearRecive(GPRS_PORT);	//将串口驱动读缓冲区读空
	return bytes;
}
/**
* 发送AT指令和GPRS模块通信
* @param Port 端口号
* @list:　发送和响应表
* @time: 延时时间
* @return: 0 回复正确
*          <0 回复不正确
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

