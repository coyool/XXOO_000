#ifndef _MC55GPRS_H_
#define _MC55GPRS_H_
#include "plat_include/sys_config.h"
#ifdef GPRS_MODULE_MC55I
#include "plat_include/plat_gprs.h"

/*
所有返回的错误代码都是负值，以下定义的错误代码都是正值，在
返回时前面加负号变成负值后返回。
*/

#define  GPRS_BAUDRATE 115200

/*
说明:GPRS/CDMA模块上电初始化
输入参数:
Mode Gprs连接方式:GSMODE_PPPD PPP方式连接,GSMODE_ATCMD AT命令方式连接
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int Mc55Init(GPRS_MODE Mode);
#define _GprsInit Mc55Init

/*
说明:GPRS/CDMA模块关机掉电
输入参数:
    无
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int Mc55PowerOff(void);
#define _GprsPowerOff Mc55PowerOff

/**
* @brief 给GPRS/CDMA模块上电
* @param 无
*/
void Mc55PowerOn(void);
#define _GprsPowerOn Mc55PowerOn
/*
说明:GPRS/CDMA网络拨号
输入参数:
无
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int Mc55DialOn(void);
#define _GprsDialOn Mc55DialOn
/*
说明:断开GPRS/CDMA拨号连接
输入参数:
无
输出参数:
无
返回值:
无
*/
void Mc55DialOff(void);
#define _GprsDialOff Mc55DialOff
/*
说明:读取当前模块信号强度
输入参数:
无
输出参数:
无
返回值:
成功返回信号强度值，失败 -ERRFAILED
*/
int Mc55GetSig(void);
#define _GprsGetSig Mc55GetSig
/*
说明:设置GPRS模块 APN
输入参数:
apn APN缓冲区指针
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int Mc55SetApn(const char *apn);
#define _GprsSetApn Mc55SetApn 
/*
说明:设置短信中心号码
输入参数:
no 短信中心号码缓存指针,len 中心号码长度
输出参数:
无
返回值:
无
*/
int  Mc55SetMsgCenterNo(const char *no,int len);
/*
说明:通过模块发送一条短消息
输入参数:
发送消息缓冲区指针
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int Mc55SendMessage(const unsigned char *msg,const char *no,int msglen);
#define _GprsSendMessage Mc55SendMessage
/*
说明:接收一条新的短消息
输入参数:
接收缓存区指针
输出参数:
无
返回值:
成功 SUCCEED，失败 -ERRFAILED
*/
int Mc55RecvMessage(unsigned char *msg,char *no,int msglen,int nolen);
#define _GprsRecvMessage	Mc55RecvMessage
/*
说明:设置(TCP/UDP)模式
输入参数:
proto ,=SOCKET_TCP TCP, =SOCKET_UDP UDP
输出参数:
无
返回值:
返回值：成功返回通信描述符，失败返回 -ERRFAILED
*/
int Mc55Mode(int proto);
#define _GprsMode Mc55Mode
/**
* @brief 连接到服务器
* @param ip 服务器IP
* @param port 服务器端口号
* @return 成功0, 否则失败
*/
int Mc55Connect(unsigned long ip, unsigned short port);
#define _GprsConnect Mc55Connect

/**
* @brief 与服务器断开连接
*/
void Mc55Disconnect(void);
#define _GprsDisconnect Mc55Disconnect



/*
说明:向网络通信接口发送数据(TCP)
输入参数:
buf 发送数据缓冲区指针，len 发送数据长度
输出参数:
无
返回值:
SUCCEED，失败 -ERRFAILED
*/
int Mc55RawSend(const unsigned char *buf, int len);
#define _GprsRawSend Mc55RawSend
/*
说明:从网络通信接口读取数据
输入参数:
	len 接收数据缓存长度
输出参数:
	buf 接收数据缓冲区指针
返回值:
成功返回接收数据长度，失败0
*/
int Mc55GetChar(unsigned char *buf,int len);
#define _GprsGetChar Mc55GetChar
#endif
#endif
