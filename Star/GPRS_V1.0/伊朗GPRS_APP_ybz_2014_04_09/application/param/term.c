/**
* term.h -- 终端参数
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-6
* 最后修改时间: 2009-5-6
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEFINE_PARATERM

#include "plat_include/debug.h"
#include "app_include/param/term.h"
#include "app_include/param/unique.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
#include "app_include/lib/bcd.h"
/*
static const unsigned char DefaultAlarmFlag[16] = {
	0x8d, 0x38, 0x10, 0xc0, 0x01, 0x00, 0x00, 0x00,  //valid
	0x00, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,  //rank
};
*/
//para_term_t ParaTerm;

/**
* @brief 载入缺省的终端参数
*/
void LoadDefParaTerm(void)
{
	memset(&ParaTerm,0,sizeof(ParaTerm));
	ParaTerm.tcom.rsnd = 0x205A;
	//ParamTermBuffer.tcom.rsnd = 0x200a;
	ParaTerm.tcom.cycka = 5;//15;
	ParaTerm.tcom.flagcon = 0x01;
	//服务器ip
	ParaTerm.svrip.ipmain[0] = 183;
	ParaTerm.svrip.ipmain[1] = 17 ;
	ParaTerm.svrip.ipmain[2] = 212;
	ParaTerm.svrip.ipmain[3] = 158;
	ParaTerm.svrip.portmain = 3004;

	strcpy((char *)ParaTerm.svrip.apn, "CMNET");
	
	ParaTerm.termip.portlisten = 9000;
	ParaTerm.uplink.countdail = 4;
	ParaTerm.uplink.timedail = 60;

//	memcpy(ParaTerm.almflag.valid, DefaultAlarmFlag, 16);
	
	ParaTerm.pwd.pwd[0] = ParaTerm.pwd.pwd[1] = 0x00;

	//终端IP
	ParaTerm.termip.ipterm[0] = 192;
	ParaTerm.termip.ipterm[1] = 168;
	ParaTerm.termip.ipterm[2] = 8;
	ParaTerm.termip.ipterm[3] = 203;
	ParaTerm.termip.maskterm[0] = 255;
	ParaTerm.termip.maskterm[1] = 255;
	ParaTerm.termip.maskterm[2] = 255;
	ParaTerm.termip.maskterm[3] = 0;
	ParaTerm.termip.ipgatew[0] = 192;
	ParaTerm.termip.ipgatew[1] = 168;
	ParaTerm.termip.ipgatew[2] = 0;
	ParaTerm.termip.ipgatew[3] = 1;

	ParaTerm.uplink.proto = 0;	
	ParaTerm.uplink.mode = 1;
	ParaTerm.uplink.clientmode = 0;
	
	ParaTerm.uplink.timedown = 10;
	ParaTerm.uplink.onlineflag = 0x00FFFFFF;
}

/**
* @brief 载入缺省的终端参数
* @brief 主台通讯参数除外
*/
void LoadDefParaTerm2(void)
{

	ParaTerm.tcom.rsnd = 0x205A;
	//ParamTermBuffer.tcom.rsnd = 0x200a;
	ParaTerm.tcom.cycka = 5;//15;
	ParaTerm.tcom.flagcon = 0x01;
	//服务器ip
/*	ParaTerm.svrip.ipmain[0] = 192;
	ParaTerm.svrip.ipmain[1] = 168;
	ParaTerm.svrip.ipmain[2] = 8;
	ParaTerm.svrip.ipmain[3] = 88;
	ParaTerm.svrip.portmain = 8601;

	strcpy((char *)ParaTerm.svrip.apn, "CMNET");
*/	
	ParaTerm.termip.portlisten = 9000;
	ParaTerm.uplink.countdail = 4;
	ParaTerm.uplink.timedail = 60;

//	memcpy(ParaTerm.almflag.valid, DefaultAlarmFlag, 16);
	
	ParaTerm.pwd.pwd[0] = ParaTerm.pwd.pwd[1] = 0x00;

	//终端IP
	ParaTerm.termip.ipterm[0] = 192;
	ParaTerm.termip.ipterm[1] = 168;
	ParaTerm.termip.ipterm[2] = 8;
	ParaTerm.termip.ipterm[3] = 203;
	ParaTerm.termip.maskterm[0] = 255;
	ParaTerm.termip.maskterm[1] = 255;
	ParaTerm.termip.maskterm[2] = 255;
	ParaTerm.termip.maskterm[3] = 0;
	ParaTerm.termip.ipgatew[0] = 192;
	ParaTerm.termip.ipgatew[1] = 168;
	ParaTerm.termip.ipgatew[2] = 0;
	ParaTerm.termip.ipgatew[3] = 1;

	ParaTerm.uplink.proto = 0;	
	ParaTerm.uplink.mode = 1;
	ParaTerm.uplink.clientmode = 0;
	
	ParaTerm.uplink.timedown = 10;
	ParaTerm.uplink.onlineflag = 0x00FFFFFF;
}


/**
* @brief 从文件中载入终端参数
* @return 0成功, 否则失败
*/
int LoadParaTerm(void)
{
	int rt=-1;

	DebugPrint(0, "  load param term...\n");

	rt = para_readflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	AssertLog(rt==-1,"readflash para_term failed!\n");
	if(rt==-1)
	{
		LoadDefParaTerm();
		return 1;
	}
	
	return 0;
}

/**
* @brief 根据参数设置网络地址
*/
void SetParaNetAddr(void)
{
	//set_local_ipaddr(ParaTerm.termip.ipterm);
	PrintLog(LOGTYPE_SHORT, "set local ipaddr: %d.%d.%d.%d...\n",
					ParaTerm.termip.ipterm[0],ParaTerm.termip.ipterm[1],ParaTerm.termip.ipterm[2],ParaTerm.termip.ipterm[3]);
}
/**
* @brief 保存终端参数
* @return 0成功, 否则失败
*/
int SaveParaTerm(void)
{
	int rt;

	rt = para_writeflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	AssertLogReturn(rt==-1,1,"writeflash para_term failed!\n");

	return 0;
}

/**
* @brief 终端参数F1操作(终端上行通信口通信参数设置)
* @param flag 操作方式, 0-读, 1-写
* @param metpid 测量点号
* @param buf 缓存区指针
* @param len 缓存区长度
* @param actlen 有效数据长度(由函数返回)
* @return 0成功, 否则失败(参看POERR_XXX宏定义)
* @note 以下同类参数和返回值相同, 不做重复注释
*/
int ParaOperationF1(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) smallcpy(buf, &ParaTerm.tcom.rts, 6);
	else{
		smallcpy(&ParaTerm.tcom.rts, buf, 6);
		SaveParaTerm();
	}
	return 0;
}

/**
* @brief 终端参数F2操作(空操作)(终端上行通信口无线中继转发设置)
*/
int ParaOperationF2(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		buf[0] = 0;
		*actlen = 1;
	}
	else {
		int i;

		i = (int)buf[0] & 0xff;
		*actlen = i*2;
		if(*actlen > len) return POERR_FATAL;
		else SaveParaTerm();
	}

	return 0;
}

/**
* @brief 终端参数F4操作(主站电话号码和短信中心号码)
*/
int ParaOperationF4(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) smallcpy(buf, ParaTerm.smsc.phone, 16);
	else {
		smallcpy(ParaTerm.smsc.phone, buf, 16);
		SaveParaTerm();
	}
	
	return 0;
}

/**
* @brief 终端参数F5操作(终端上行通信消息认证参数设置)
*/
int ParaOperationF5(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) smallcpy(buf, &ParaTerm.pwd.art, 3);
	else {
		smallcpy(&ParaTerm.pwd.art, buf, 3);
		SaveParaTerm();
	}

	return 0;
}
#ifdef afn04f6
/**
* @brief 终端参数F6操作(终端组地址设置)
*/
int ParaOperationF6(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) smallcpy(buf, ParaTerm.grpaddr.addr, 16);
	else {
		smallcpy(ParaTerm.grpaddr.addr, buf, 16);
		SaveParaTerm();
	}

	return 0;
}
#endif

/**
* @brief 终端参数F3操作(主站IP地址和端口)
*/
int ParaOperationF3(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	
	if(0 == flag) smallcpy(buf, ParaTerm.svrip.ipmain, 28);
	else {
		smallcpy(ParaTerm.svrip.ipmain, buf, 28);
		ParaTerm.svrip.apn[16] = 0;
		SaveParaTerm();
	}

	return 0;
}
/**
* @brief 终端参数F7操作(终端IP地址和端口)
*/
int ParaOperationF7(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	int alen;
	int lenusr, lenpwd;
	unsigned char *pbuf;

	if(0 == flag) {
		lenusr = strlen(ParaTerm.termip.username);
		lenpwd = strlen(ParaTerm.termip.pwd);
		alen = 24 + lenusr + lenpwd;
		if(alen > len) return POERR_FATAL;
		*actlen = alen;

		pbuf = buf;
		smallcpy(pbuf, ParaTerm.termip.ipterm, 12); pbuf += 12;
		*pbuf++ = ParaTerm.termip.proxy_type;
		smallcpy(pbuf, ParaTerm.termip.ipproxy, 4); pbuf += 4;
		*pbuf++ = (unsigned char)ParaTerm.termip.portproxy;
		*pbuf++ = (unsigned char)(ParaTerm.termip.portproxy >> 8);
		*pbuf++ = ParaTerm.termip.proxy_connect;
		*pbuf++ = lenusr;
		if(lenusr) strcpy((char *)pbuf, ParaTerm.termip.username);
		pbuf += lenusr;
		*pbuf++ = lenpwd;
		if(lenpwd) strcpy((char *)pbuf, ParaTerm.termip.pwd);
		pbuf += lenpwd;
		pbuf[0] = ParaTerm.termip.portlisten;
		pbuf[1] = ParaTerm.termip.portlisten >> 8;
	}
	else {
		lenusr = (int)buf[20]&0xff;
		pbuf = buf;
		pbuf += 21 + lenusr;
		lenpwd = (int)(*pbuf)&0xff;
		alen = 24 + lenusr + lenpwd;
		*actlen = alen;
		if(alen > len) return POERR_FATAL;

		if(lenusr > 32 || lenpwd > 32) return POERR_INVALID;

		pbuf = buf;
		smallcpy(ParaTerm.termip.ipterm, pbuf, 12); pbuf += 12;
		ParaTerm.termip.proxy_type = *pbuf++;
		smallcpy(ParaTerm.termip.ipproxy, pbuf, 4); pbuf += 4;
		ParaTerm.termip.portproxy = ((unsigned short)pbuf[1]<<8) + (unsigned short)pbuf[0];
		pbuf += 2;
		ParaTerm.termip.proxy_connect = *pbuf++;
		pbuf += 1;
		if(lenusr) smallcpy(ParaTerm.termip.username, pbuf, lenusr);
		if(lenusr < 32) ParaTerm.termip.username[lenusr] = 0;
		else ParaTerm.termip.username[31] = 0;
		pbuf += lenusr+1;
		if(lenpwd) smallcpy(ParaTerm.termip.pwd, pbuf, lenpwd);
		if(lenpwd < 32) ParaTerm.termip.pwd[lenpwd] = 0;
		else ParaTerm.termip.pwd[31] = 0;
		pbuf += lenpwd;
		ParaTerm.termip.portlisten = ((unsigned short)pbuf[1]<<8) + (unsigned short)pbuf[0];
		SaveParaTerm();
	}

	return 0;
}

/**
* @brief 终端参数F8操作(终端上行通信工作方式(以太专网或虚拟专网))
*/
int ParaOperationF8(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	unsigned char *pbuf = buf;

	if(0 == flag) {
		pbuf[0] = (ParaTerm.uplink.proto) ? 0x80 : 0;
		pbuf[0] |= (ParaTerm.uplink.mode&0x03) << 4;
		pbuf[0] |= (ParaTerm.uplink.clientmode+1) & 0x03;
		pbuf[1] = ParaTerm.uplink.timedail;
		pbuf[2] = ParaTerm.uplink.timedail>>8;
		pbuf[3] = ParaTerm.uplink.countdail;
		pbuf[4] = ParaTerm.uplink.timedown;
		pbuf[5] = ParaTerm.uplink.onlineflag & 0xff;
		pbuf[6] = (ParaTerm.uplink.onlineflag >> 8) & 0xff;
		pbuf[7] = (ParaTerm.uplink.onlineflag >> 16) & 0xff;
	}
	else {
		ParaTerm.uplink.proto = (pbuf[0]&0x80) ? 1 : 0;
		ParaTerm.uplink.mode = (pbuf[0]>>4)&0x03;
		ParaTerm.uplink.clientmode = pbuf[0] & 0x03;
		if(ParaTerm.uplink.clientmode) ParaTerm.uplink.clientmode -= 1;		  //
		ParaTerm.uplink.timedail = ((unsigned short)pbuf[2]<<8) +(unsigned short)pbuf[1];
		ParaTerm.uplink.countdail = pbuf[3];
		ParaTerm.uplink.timedown = pbuf[4];
		ParaTerm.uplink.onlineflag = ((unsigned long)pbuf[7]<<16) + ((unsigned long)pbuf[6]<<8) + pbuf[5];
		SaveParaTerm();
	}

	return 0;
}
#if 0
/**
* @brief 终端参数F9操作(终端事件记录配置设置)
*/
int ParaOperationF9(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) memcpy(buf, ParaTerm.almflag.valid, 16);
	else {
		memcpy(ParaTerm.almflag.valid, buf, 16);
		SaveParaTerm();
	}
	
	return 0;
}
#endif
/**
* @brief 终端参数F16操作(虚拟专网用户名、密码)
*/
int ParaOperationF16(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) memcpy(buf, ParaTerm.vpn.user, 64);
	else {
		memcpy(ParaTerm.vpn.user, buf, 64);
		SaveParaTerm();
	}
	
	return 0;
}



