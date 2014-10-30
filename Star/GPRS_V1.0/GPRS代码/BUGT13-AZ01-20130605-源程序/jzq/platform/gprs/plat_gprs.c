#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "plat_include/debug.h"
#include "plat_include/plat_defines.h"
#include "plat_include/plat_gprs.h"
#include "plat_include/m35gprs.h"
#include "plat_include/mc55gprs.h"
/*
说明:GPRS/CDMA模块上电初始化
输入参数:
    Mode Gprs连接方式:GSMODE_PPPD PPP方式连接,GSMODE_ATCMD AT命令方式连接
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsInit(GPRS_MODE Mode)
{
	return _GprsInit(Mode);
}
/*
说明:GPRS/CDMA模块关机掉电
输入参数:
    无
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsPowerOff(void)
{
	_GprsPowerOff();
	return SUCCEED;
}
/**
* @brief 给GPRS/CDMA模块上电
* @param 无
*/
void GprsPowerOn(void)
{
	_GprsPowerOn();
}

/*
说明:GPRS/CDMA网络拨号
输入参数:
    无
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsDialOn(void)
{
	return _GprsDialOn();
}

/*
说明:断开GPRS/CDMA拨号连接
输入参数:
    无
输出参数:
    无
返回值:
    无
*/
void GprsDialOff(void)
{
	_GprsDialOff();
}

/*
说明:读取当前模块信号强度
输入参数:
    无
输出参数:
    无
返回值:
    成功返回信号强度值，失败 -ERRFAILED
*/
int GprsGetSig(void)
{
	return _GprsGetSig();
}

/*
说明:设置GPRS模块 APN
输入参数:
    apn APN缓冲区指针
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsSetApn(const char *apn)
{
	return _GprsSetApn(apn);
}

/*
说明:通过模块发送一条短消息
输入参数:
    发送消息缓冲区指针
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsSendMessage(const unsigned char *msg,const char *no,int msglen)
{
	return _GprsSendMessage(msg,no,msglen);
}

/*
说明:接收一条新的短消息
输入参数:
    接收缓存区指针
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
	*/
unsigned char debug_sms[220];
unsigned char debug_sms_len = 0;
unsigned char debug_sms_rv = 0;
int GprsRecvMessage(unsigned char *msg,char *no,int msglen,int nolen)
{
	if(debug_sms_rv == 1)
	{
		debug_sms_rv = 0;
		if(msglen<debug_sms_len)
		{
			PrintLog(LOGTYPE_DEBUG,"缓冲太小了\r\n");
			return -ERRFAILED;
		}
		memcpy(msg,debug_sms,debug_sms_len);
		strcpy(no,"13714325180");
		return SUCCEED;
	}
	return _GprsRecvMessage(msg,no,msglen,nolen);
}

/*
说明:读取GPRS/CDMA模块当前状态
输入参数:
    无
输出参数:
    无
返回值:
    成功返回模块当前状态编码，失败 -ERRFAILED
*/
int GprsGetModuleState(void)
{
	return -ERRFAILED;
}

/*
说明:获取模块RING脚当前状态
输入参数:
    无
输出参数:
    无
返回值:
    成功返回模块状态（0、1），失败 -ERRFAILED
*/
int GprsGetRingState(void)
{
	return -ERRFAILED;	
}
/*
说明:设置GPRS/CDMA拨号的用户和密码
输入参数:
    usr 用户名缓存指针
    pwd 密码缓存指针
    usrlen 用户名长度
    pwdlen 密码长度
输出参数:
    无
返回值:
    无
*/
void GprsSetUserPwd(const char *usr,const char *pwd)
{
}

/*
说明:设置短信中心号码
输入参数:
    no 短信中心号码缓存指针,len 中心号码长度
输出参数:
    无
返回值:
    无
*/
int  GprsSetMsgCenterNo(const char *no,int len)
{
	return -ERRFAILED;
}
/*
说明:设置多路复用功能
输入参数:
    flag =GCMUX_OFF关闭，=CMUX_ON开启
*/
void GprsSetCmux(int flag)
{
	(flag=flag);
}

/*
说明:得到APN
返回值:
	apn
*/
const char* GprsGetApn(void)
{
	return NULL;
}
/*
说明:得到Gprs连接方式
返回值:
	连接方式:GSMODE_PPPD PPP方式连接,GSMODE_ATCMD AT命令方式连接
*/
int GprsGetMode(void)
{
	return GSMODE_ATCMD;
}
/*
说明:得到Gprs的Ip地址
返回值:
连接方式:GSMODE_PPPD PPP方式连接,GSMODE_ATCMD AT命令方式连接
*/
unsigned int GprsGetIP(void)
{
	return 0;
}
/*
    返回当前信号状态 1 好，0 不好
*/
int GprsSigqualityF(void)
{
	return 0;
}
/**
* @brief 检测在线状态
* @return 在线返回1, 掉线返回0
*/

int GprsLineState(void)
{
	return _GprsLineState();
}
/**
* @brief 检测TCP状态
* @return 在线返回1, 掉线返回0
*/

int GprsTcpState(void)
{
	return _GprsTCPState();
}
/*
说明:通过DNS服务得到IP地址
输入参数:
	web_addr WWW网址
输出
	ip ip地址
返回
成功 SUCCEED，失败 -ERRFAILED
*/
int GprsDns(const char* web_addr,varip_t* ip)
{
	return _GprsDns(web_addr,ip);
}
