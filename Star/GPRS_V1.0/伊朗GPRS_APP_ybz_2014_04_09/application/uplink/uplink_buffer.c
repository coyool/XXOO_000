/**
* uplink_buffer.c -- 上行通信缓存区定义
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-18
* 最后修改时间: 2009-5-18
*/

//#include <stdio.h>

//#include "include/debug.h"
#include <string.h>
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
#include "app_include/sys/rs485.h"

/*static int empty_rawsend(const unsigned char *buf, int len)
{
	return 0;
}

static int empty_getchar(unsigned char *buf)
{
	return 0;
}*/

static int linestat_empty(void)
{
	return 1;
}
/*interface*/
///later 
/*
extern int SerialGetChar(unsigned char *buf);
extern int SerialRawSend(const unsigned char *buf, int len);

extern int IrGetChar(unsigned char *buf);
extern int IrRawSend(const unsigned char *buf, int len);

extern int GprsGetChar(unsigned char * buf);
extern int GprsRawSend(const unsigned char * buf,int len);

extern int GprsLineState(void);
*/

//extern int up485_getchar(unsigned char *buf);
//extern int up485_send(const unsigned char *buf,int len);
extern int GprsGetChar(unsigned char *buf);
extern int GprsRawSend(const unsigned char *buf,int len);
//extern int EtherGetChar(unsigned char *buf);
//extern int EtherRawSend(const unsigned char *buf,int len);
//extern int IrGetChar(unsigned char *buf);
//extern int IrRawSend(const unsigned char *buf,int len);
//extern int EtherSvrGetChar(unsigned char *buf);
//extern int EtherSvrRawSend(const unsigned char *buf, int len);


#define SerialRawSend up485_send
#define SerialGetChar up485_getchar 

#define MAXLEN_SERIALITF_BUF	(1152/4)
//#define MAXLEN_ETHSVRITF_BUF	1152
#define MAXLEN_GPRSITF_BUF		1152
//#define MAXLEN_IRITF_BUF		(1152/4)

static unsigned int g_gprsitf_rcvbuf[MAXLEN_GPRSITF_BUF/4];
static unsigned int g_gprsitf_sndbuf[MAXLEN_GPRSITF_BUF/4];
#ifdef BIG_CONCENT
static unsigned int g_serialitf_rcvbuf[MAXLEN_SERIALITF_BUF/4];
static unsigned int g_serialitf_sndbuf[MAXLEN_SERIALITF_BUF/4];
#endif
//static unsigned int g_iritf_rcvbuf[MAXLEN_IRITF_BUF/4];
//static unsigned int g_iritf_sndbuf[MAXLEN_IRITF_BUF/4];
//static unsigned int g_ethitf_rcvbuf[MAXLEN_ETHSVRITF_BUF/4];
//static unsigned int g_ethitf_sndbuf[MAXLEN_ETHSVRITF_BUF/4];

const uplinkitf_t UplinkInterface[UPLINKITF_NUM] = {
#ifdef BIG_CONCENT
	{(unsigned char *)g_serialitf_rcvbuf, (unsigned char *)g_serialitf_sndbuf, 
		SerialRawSend, SerialGetChar, linestat_empty, 5, 
		1024, 1024, 512, 0},
#endif
	/*{(unsigned char *)g_iritf_rcvbuf, (unsigned char *)g_iritf_sndbuf, 
		IrRawSend, IrGetChar, linestat_empty, 20, 
		256, 256, 128, 0}, */
	 {(unsigned char *)g_gprsitf_rcvbuf, (unsigned char *)g_gprsitf_sndbuf, 
		GprsRawSend, GprsGetChar, linestat_empty, 60, 
		1024, 1024, 512, 0},
/*	 {(unsigned char *)g_gprsitf_rcvbuf, (unsigned char *)g_gprsitf_sndbuf, 
		EtherRawSend, EtherGetChar, linestat_empty, 6, 
		1024, 1024, 512, 0},
	 {(unsigned char *)g_ethitf_rcvbuf, (unsigned char *)g_ethitf_sndbuf, 
		EtherSvrRawSend, EtherSvrGetChar, linestat_empty, 6, 
		1024, 1024, 512, 0},
	 {(unsigned char *)g_gprsitf_rcvbuf, (unsigned char *)g_gprsitf_sndbuf, 
		GprsRawSend, GprsGetChar, linestat_empty, 60, 
		1024, 1024, 512, 0},*/
};



