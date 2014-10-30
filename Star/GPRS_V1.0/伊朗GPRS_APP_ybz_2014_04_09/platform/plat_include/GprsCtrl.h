#ifndef _GPRSCTRL_H_
#define _GPRSCTRL_H_
#ifdef _WIN32
#include <Windows.h>
#else
#include <ucos_ii.h>
#endif
#include "hal_include/switch.h"
//AT命令构成
struct atlist_struct
{
	const unsigned char *cmd;
	unsigned int keynum;
	const unsigned char *key[4];
	unsigned char times;//接受次数0=一次,1=两次
};


#define gprsline_pwroff 	gprs_vcc_on
	
#define gprsline_pwron 	    gprs_vcc_off



#define TickSleep(X) sleep(X)

/**
* 发送AT指令和GPRS模块通信
* @param Port 端口号
* @list:　发送和响应表
* @time: 延时时间
* @return: 0 回复正确
*          <0 回复不正确
*/
typedef int (* PreReciveFun)(int port,char* buf);//接受预处理,并且清空接受
int SendAtCmd(const struct atlist_struct *list,unsigned int interval,unsigned int time,unsigned char* buf,int buf_len);
/**
* GPRS接收数据包,如果在指定的timeout内没有收到数据，就认为接收失败，如果收到一个字节后2个20ms内没有收到下一个字节，认为接收完成
* @param Port 端口号
* @param interval:字节间的间隔,设IP地址的那个命令，就是先收到OK，过很久才收CONNECT OK
* @param timeout: 延时时间，一般时间和长时间两种
* @param return: 发送命令之后收到第一个回复字节之间的最长时间
*/

//接收时的睡眠周期
unsigned int AtCmdRecv(unsigned int interval,unsigned int timeout,unsigned char* atcmd_cache,unsigned int cache_len);

/**
* 字符类型转换成u8
* @i: u8类型
* @return 字符类型
*/
unsigned char char_to_hex(unsigned char i);

void SetSms(unsigned char Need);
#endif
