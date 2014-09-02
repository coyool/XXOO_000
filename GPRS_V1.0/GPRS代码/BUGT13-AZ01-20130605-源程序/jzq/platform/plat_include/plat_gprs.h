#ifndef __PLAT_GPRS_H__
#define __PLAT_GPRS_H__
#include "plat_include/plat_network.h"

//多路复用开关
typedef enum{
	GCMUX_OFF = 0,
	GCMUX_ON
}CMUX_TYPE;

//GPRS模块状态
typedef enum{
	GSTATE_BEGIN = 0,		// 初始状态
	GSTATE_DEVINITING,		//初始化状态
	GSTATE_DEVINITFAILED,	// +初始化失败
	GSTATE_DEVINITED,		//初始化完成
	GSTATE_CHECKSIG,		//检测网络信号
	GSTATE_SIGFAILED,		//没有检测到网络信号
	GSTATE_APNSETFAILED,	// +设置APN失败
	GSTATE_APNSETED,			// +设置APN成功
	GSTATE_NETREGFAILED,	// +网络注册失败
	GSTATE_NETREGED,			// +网络注册成功
	GSTATE_DIALFAILED,		//拨号失败
	GSTATE_DIALING,			//拨号中
	GSTATE_DIALED,			//拨号成功
}GPRS_STATE;

typedef enum{
	GSMODE_PPPD,//PPP方式连接
	GSMODE_ATCMD//AT命令方式连接
}GPRS_MODE;

/*
说明:GPRS/CDMA模块上电初始化
输入参数:
    Mode Gprs连接方式:GSMODE_PPPD PPP方式连接,GSMODE_ATCMD AT命令方式连接
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsInit(GPRS_MODE Mode);


/*
说明:GPRS/CDMA模块关机掉电
输入参数:
    无
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsPowerOff(void);



/**
* @brief 给GPRS/CDMA模块上电
* @param 无
*/
void GprsPowerOn(void);

/*
说明:GPRS/CDMA网络拨号
输入参数:
    无
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsDialOn(void);

/*
说明:断开GPRS/CDMA拨号连接
输入参数:
    无
输出参数:
    无
返回值:
    无
*/
void GprsDialOff(void);

/*
说明:读取当前模块信号强度
输入参数:
    无
输出参数:
    无
返回值:
    成功返回信号强度值，失败 -ERRFAILED
*/
int GprsGetSig(void);

/*
说明:设置GPRS模块 APN
输入参数:
    apn APN缓冲区指针
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsSetApn(const char *apn);

/*
说明:通过模块发送一条短消息
输入参数:
    发送消息缓冲区指针
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
int GprsSendMessage(const unsigned char *msg,const char *no,int msglen);

/*
说明:接收一条新的短消息
输入参数:
    接收缓存区指针
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
	*/
int GprsRecvMessage(unsigned char *msg,char *no,int msglen,int nolen);

#if 0
/*
说明:读取GSM模块型号
输入参数:
    无
输出参数:
    无
返回值:
    成功返回模块类型编码，失败 -ERRFAILED
*/
int GprsGetModuleType(void);
#endif
/*
说明:读取GPRS/CDMA模块当前状态
输入参数:
    无
输出参数:
    无
返回值:
    成功返回模块当前状态编码，失败 -ERRFAILED
*/
int GprsGetModuleState(void);

/*
说明:获取模块RING脚当前状态
输入参数:
    无
输出参数:
    无
返回值:
    成功返回模块状态（0、1），失败 -ERRFAILED
*/
int GprsGetRingState(void);

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
void GprsSetUserPwd(const char *usr,const char *pwd);

/*
说明:设置短信中心号码
输入参数:
    no 短信中心号码缓存指针,len 中心号码长度
输出参数:
    无
返回值:
    无
*/
int  GprsSetMsgCenterNo(const char *no,int len);

/*
说明:设置多路复用功能
输入参数:
    flag =GCMUX_OFF关闭，=CMUX_ON开启
*/
void GprsSetCmux(int flag);

/*
说明:得到APN
返回值:
	apn
*/
const char* GprsGetApn(void);
/*
说明:得到Gprs连接方式
返回值:
	连接方式:GSMODE_PPPD PPP方式连接,GSMODE_ATCMD AT命令方式连接
*/
int GprsGetMode(void);
/*
说明:得到Gprs的Ip地址
返回值:
连接方式:GSMODE_PPPD PPP方式连接,GSMODE_ATCMD AT命令方式连接
*/
unsigned int GprsGetIP(void);
/**
* @brief 检测在线状态
* @return 在线返回1, 掉线返回0
*/

int GprsLineState(void);
/*
    返回当前信号状态 1 好，0 不好
*/
int GprsSigqualityF(void);
/**
* @brief 检测TCP状态
* @return 在线返回1, 掉线返回0
*/

int GprsTcpState(void);
/*
说明:通过DNS服务得到IP地址
输入参数:
	web_addr WWW网址
输出
	ip ip地址
返回
成功 SUCCEED，失败 -ERRFAILED
*/
int GprsDns(const char* web_addr,varip_t* ip);
#endif
