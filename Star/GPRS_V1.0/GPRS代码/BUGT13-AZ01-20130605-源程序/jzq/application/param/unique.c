/**
* unique.c -- 自定义参数
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-18
* 最后修改时间: 2009-5-18
*/

//#include <stdio.h>
#include <string.h>

#define DEFINE_PARAUNI
#include "includes.h"
#include "plat_include/debug.h"
#include "app_include/param/unique.h"
#include "app_include/version.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
#include "plat_include/timer.h"
#include "plat_include/platpara.h"

//para_uni_t ParaUni;
void LoadDefParaUni(void)
{
	memset(&ParaUni,0,sizeof(ParaUni));
	ParaUni.project_no = 0;
	//ProjectNumber = ParaUni.project_no;

	ParaUni.uplink = 2;//GPRS 2/Ether 3;	//debug purpose,should by GPRS in release version
	ParaUni.num_factory[0] = 0x66;
	ParaUni.num_factory[1] = 0x66;
	ParaUni.addr_area[0] = 0x55;
	ParaUni.addr_area[1] = 0x07;
	ParaUni.addr_sn[0] = 1;
	ParaUni.addr_sn[1] = 0;

	ParaUni.addr_mac[0] = 0x56;
	ParaUni.addr_mac[1] = 0x34;
	ParaUni.addr_mac[2] = 0x12;
	ParaUni.addr_mac[3] = 0x48;
	ParaUni.addr_mac[4] = 0x80;
	ParaUni.addr_mac[5] = 0x00;

	ParaUni.serialfunc = 0;
	ParaUni.dooreventflag = 0;

	ParaUni.gprsdialtype = 1;
}

void LoadDefParaUni2(void)
{
	memset(&ParaUni,0,sizeof(ParaUni));
	ParaUni.project_no = 0;
	//ProjectNumber = ParaUni.project_no;

	ParaUni.uplink = 2;//GPRS 2/Ether 3;	//debug purpose,should by GPRS in release version
	ParaUni.num_factory[0] = 0x66;
	ParaUni.num_factory[1] = 0x66;
	//ParaUni.addr_area[0] = 0x55;
	//ParaUni.addr_area[1] = 0x07;
	//ParaUni.addr_sn[0] = 1;
	//ParaUni.addr_sn[1] = 0;

	ParaUni.addr_mac[0] = 0x56;
	ParaUni.addr_mac[1] = 0x34;
	ParaUni.addr_mac[2] = 0x12;
	ParaUni.addr_mac[3] = 0x48;
	ParaUni.addr_mac[4] = 0x80;
	ParaUni.addr_mac[5] = 0x00;

	ParaUni.serialfunc = 0;
	ParaUni.dooreventflag = 0;

	ParaUni.gprsdialtype = 1;
}
 

int LoadParaUni(void)
{
	int rt;
	
	rt = para_readflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	AssertLog(rt==-1,"readflash para_uni failed!\n");
	if(rt==-1)
	{
		LoadDefParaUni();
		return 1;
	}
	
	return 0;
}

/**
* @brief 保存终端参数
* @return 0成功, 否则失败
*/
int SaveParaUni(void)
{
	int rt;
	
	rt = para_writeflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	AssertLog(rt==-1,"writeflash para_uni failed!\n");
	
	return 0;
}
/**
* @brief 终端参数F85操作
* flag : 0 读终端地址 1 写终端地址
*/
int ParaOperationF85(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		memcpy(buf, ParaUni.addr_area, 4);
		
	}
	else {
		memcpy(ParaUni.addr_area, buf, 4);
		
		SaveParaUni();
	}

	return 0;
}
/**
* @brief 设置主站网址
* flag : 0 读取 1 设置
*/
int ParaOperationF161(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	int i;
	if(0 == flag) {
		PrintLog(LOGTYPE_DEBUG,"F161:%s\r\n",ParaUni.web_addr);
		memset(buf,0,sizeof(ParaUni.web_addr));
		for(i=0;i<sizeof(ParaUni.web_addr);i++)
			if(ParaUni.web_addr[i]!=0)
				buf[i] = ParaUni.web_addr[i];
			else
				break;
		
	}
	else {
		memset(ParaUni.web_addr,0,sizeof(ParaUni.web_addr));
		for(i=0;i<sizeof(ParaUni.web_addr);i++)
			if(buf[i] != 0)
				ParaUni.web_addr[i]=buf[i];
			else
				break;
		SaveParaUni();
	}

	return 0;
}


/**
* @brief 设置上行通信方式
* flag : 0 读取 1 设置
*/
int ParaOperationF169(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		buf[0] = ParaUni.uplink;
		
	}
	else {
		ParaUni.uplink = buf[0];
		SaveParaUni();
	}

	return 0;
}

/**
* @brief 设置上行通信方式
* flag : 0 读取 1 设置
*/
int ParaOperationF171(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		buf[0] = ParaUni.repair_portstate;
		
	}
	else {
		   ParaUni.repair_portstate = buf[0];
                PrintLog(LOGTYPE_DEBUG,"ParaUni.repair_portstate=%d\n",buf[0]);
//                platpara_write(0,buf,1);
		   SaveParaUni();
	}

	return 0;
}

extern unsigned char logType;
static int timerid; 
/**
* @brief 下载文件超时定时器
*/
static int CTimerLogType(unsigned long arg)
{	
	PrintLog(LOGTYPE_ALARM,"debug close...\n");
	
	timerid  = -1;
	logType = 0;
	
	return 1;
}

/**
* @brief 设置调试级别
* flag : 0 读取 1 设置
*/
int ParaOperationF170(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{

	if(0 == flag) {
		buf[0] = logType;
	}
	else {
		logType = buf[0];
		
		if((logType != 0)&&(logType <=LOGTYPE_DEBUG))
		{
			if(timerid==-1) timerid = SysAddCTimer(3600,CTimerLogType,0);
			else SysClearCTimer(timerid);
		}
		else {
			if(timerid != -1)
			{
				SysStopCTimer(timerid);
				timerid = -1;
			}
		}
	}

	return 0;
}

/**
* @brief 终端参数F240操作 浙江扩增
* flag : 0 读终端地址 1 写终端地址
*/
int ParaOperationF240(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		memcpy(buf, ParaUni.addr_area, 4);
		
	}
	else {
		memcpy(ParaUni.addr_area, buf, 4);
		
		SaveParaUni();
	}

	return 0;
}

char* GetUserPhoneNumber(void)
{
	return (char*)ParaUni.manuno;
}
