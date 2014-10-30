/**
* mix.c -- 综合参数
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-8
* 最后修改时间: 2009-5-8
*/

//#include <stdio.h>
#include <string.h>

#define DEFINE_PARAMIX

#include "plat_include/debug.h"
#include "app_include/param/mix.h"
#include "app_include/param/meter.h"
#include "app_include/param/commport.h"
#include "plat_include/bin.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
#include "app_include/lib/bcd.h"
#include "app_include/lib/align.h"

//para_commport_t ParaCommPort[MAX_COMMPORT];
//para_mix_t ParaMix;

const para_commport_t *GetParaCommPort(unsigned int port)
{
	return &ParaCommPort[port];
}

void SetParaCommPort(unsigned char port, para_commport_t para_port)
{
	if(port >= MAX_COMMPORT) return ;
	ParaCommPort[port] = para_port;
}
/**
* @brief 载入缺省综合参数
*/
void LoadDefParaMix(char flg)
{
	int i;
	if(flg==1)
	{
		memset(ParaCommPort,0,sizeof(ParaCommPort));

		for(i=0; i<MAX_COMMPORT; i++) {
                   ParaCommPort[i].flag |= 0x0020;
			ParaCommPort[i].baudrate = 1200;
			ParaCommPort[i].frame = COMMFRAME_BAUD_1200|COMMFRAME_STOPBIT_1|COMMFRAME_HAVECHECK|COMMFRAME_EVENCHECK|COMMFRAME_DATA_8;
			ParaCommPort[i].cycle = 60;
			ParaCommPort[i].periodnum = 1;
			ParaCommPort[i].period[0].hour_start = 0;
			ParaCommPort[i].period[0].min_start = 0;
			ParaCommPort[i].period[0].hour_end = 24;
			ParaCommPort[i].period[0].min_end = 0;
		}
	}
	else if(flg == 2)
	{
		memset(&ParaMix,0,sizeof(ParaMix));

		ParaMix.metabnor.diff = 00;
		ParaMix.metabnor.fly = 00;
		ParaMix.metabnor.stop = 00;
		ParaMix.metabnor.time = 4;
		
		ParaMix.bactsend = 0;

		ParaMix.isig.flagin = 0x01;
		ParaMix.isig.flagattr = 0x40;
	}
}

/**
* @brief 从文件中载入综合参数
* @return 0成功, 否则失败
*/

int LoadParaMix(void)
{
	int rt;

	
	rt = para_readflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	AssertLog(rt==-1,"readflash para_mix failed!\n");
	if(-1 == rt) LoadDefParaMix(1);
	
	rt = para_readflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	AssertLog(rt==-1,"readflash para_commport failed!\n");
	if(-1 == rt) LoadDefParaMix(2);

	return rt;
}

/**
* @brief 保存综合参数
* @return 0成功, 否则失败
*/
int SaveParaMix(char flg)
{
	int rt;
	
	if(flg == 1)
	{
		rt = para_writeflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
		AssertLog(rt==-1,"readflash para_mix failed!\n");

	}
	else if(flg == 2)
	{
		rt = para_writeflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
		AssertLog(rt==-1,"readflash para_commport failed!\n");
	}
	
	return rt;
}

extern void AutoReadCenMetInit(void);
void CenMetRmdInit(void);


/**
* @brief 终端参数F12操作(终端状态量输入参数)
*/
int ParaOperationF12(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		buf[0] = ParaMix.isig.flagin;
		buf[1] = ParaMix.isig.flagattr;
		//buf[2] = ParaMix.isig.reserv[0];
	}
	else {
		ParaMix.isig.flagin = buf[0];
		ParaMix.isig.flagattr = buf[1];
		//ParaMix.isig.reserv[0] = buf[2];

		SaveParaMix(1);
	}

	return 0;
}

///later extern void AutoReadCenMetInit(void);
///later void CenMetRmdInit(void);

/**
* @brief 终端参数F33操作(终端抄表运行参数设置)
* @param flag 操作方式, 0-读, 1-写
* @param option 操作选项
* @return 0成功, 否则失败(参看POERR_XXX宏定义)
*/
int ParaOperationF33(int flag, unsigned short metpid, param_specialop_t *option)
{
	if(0 == flag) {
		int num, i, portid, actnum;
		const unsigned char *prbuf;
		unsigned char *pwbuf, prd;
		unsigned short wlen, tmplen;

		num = (int)option->rbuf[0]&0xff;
		option->ractlen = num + 1;
		if(option->rlen < option->ractlen) return POERR_FATAL;

		wlen = 1;
		prbuf = option->rbuf + 1;
		pwbuf = option->wbuf + 1;
		actnum = 0;
		for(i=0; i<num; i++,prbuf++) {
			portid = (int)prbuf[0]&0xff;
			//if(portid == 0 || portid >MAX_COMMPORT) continue;
			//portid -= 1;
			if( portid > 3 || portid < 2) continue;
                   portid -= 2;
                   
			tmplen = ParaCommPort[portid].periodnum * 4 + 14;
			if((wlen+tmplen) > option->wlen) break;

			*pwbuf++ = prbuf[0];
			*pwbuf++ = ParaCommPort[portid].flag & 0xff;
			*pwbuf++ = ParaCommPort[portid].flag >> 8;
			*pwbuf++ = ParaCommPort[portid].dateflag & 0xff;
			*pwbuf++ = (ParaCommPort[portid].dateflag>>8) & 0xff;
			*pwbuf++ = (ParaCommPort[portid].dateflag>>16) & 0xff;
			*pwbuf++ = (ParaCommPort[portid].dateflag>>24) & 0xff;
			pwbuf[0] = ParaCommPort[portid].time_minute;
			pwbuf[1] = ParaCommPort[portid].time_hour;
			HexToBcd(pwbuf, 2); pwbuf += 2;
			*pwbuf++ = ParaCommPort[portid].cycle;
			pwbuf[0] = ParaCommPort[portid].chktime_minute;
			pwbuf[1] = ParaCommPort[portid].chktime_hour;
			pwbuf[2] = ParaCommPort[portid].chktime_day;
			HexToBcd(pwbuf, 3); pwbuf += 3;
			*pwbuf++ = ParaCommPort[portid].periodnum;
			for(prd=0; prd<ParaCommPort[portid].periodnum; prd++) {
				pwbuf[0] = ParaCommPort[portid].period[prd].min_start;
				pwbuf[1] = ParaCommPort[portid].period[prd].hour_start;
				pwbuf[2] = ParaCommPort[portid].period[prd].min_end;
				pwbuf[3] = ParaCommPort[portid].period[prd].hour_end;
				HexToBcd(pwbuf, 4);
				pwbuf += 4;
			}
			wlen += tmplen;
			actnum++;
		}

		option->wactlen = wlen;
		option->wbuf[0] = actnum;
	}
	else {
		int num, i, portid;
		const unsigned char *prbuf;
		unsigned short rlen, tmplen;
		unsigned char prd;
		unsigned char rmdhour, rmdminute, cycle;

		num = (int)option->rbuf[0]&0xff;
		if((num*14+1) > option->rlen) return POERR_FATAL;
		//option->ractlen = num*14 + 1;	

		prbuf = option->rbuf + 1;
		rlen = 1;
		for(i=0; i<num; i++) {
			portid = (int)prbuf[0]&0xff;
			tmplen = prbuf[13];
			tmplen = tmplen * 4 + 14;
			rlen += tmplen;
			if(rlen > option->rlen) return POERR_FATAL;

			/*if(portid == 0 || portid > MAX_COMMPORT || prbuf[13] > 24) {
				prbuf += tmplen;
				continue;
			}
			portid -= 1;*/
			if(portid < 2 || portid >3 || prbuf[13] > 24) {
				prbuf += tmplen;
				continue;
			}
			portid -= 2;
			prbuf += 1;

			//备份初始设置
			cycle = ParaCommPort[portid].cycle;
			rmdhour = ParaCommPort[portid].time_hour;
			rmdminute = ParaCommPort[portid].time_minute;

			ParaCommPort[portid].flag = ((unsigned short)prbuf[1]<<8) + ((unsigned short)prbuf[0]&0xff); prbuf += 2;
			ParaCommPort[portid].dateflag = ((unsigned int)prbuf[3]<<24) + ((unsigned int)prbuf[2]<<16)
							+ ((unsigned int)prbuf[1]<<8) + ((unsigned int)prbuf[0]&0xff);
			prbuf += 4;
			ParaCommPort[portid].time_minute = *prbuf++;
			ParaCommPort[portid].time_hour = *prbuf++;
			BcdToHex(&ParaCommPort[portid].time_hour, 2);
			ParaCommPort[portid].cycle = *prbuf++;
			ParaCommPort[portid].chktime_minute = *prbuf++;
			ParaCommPort[portid].chktime_hour = *prbuf++;
			ParaCommPort[portid].chktime_day = *prbuf++;
			BcdToHex(&ParaCommPort[portid].chktime_day, 3);

			ParaCommPort[portid].periodnum = *prbuf++;
			DebugPrint(LOGTYPE_SHORT, "prd num = %d\n", ParaCommPort[portid].periodnum);
			for(prd=0; prd<ParaCommPort[portid].periodnum; prd++) {
				ParaCommPort[portid].period[prd].min_start = prbuf[0];
				ParaCommPort[portid].period[prd].hour_start = prbuf[1];
				ParaCommPort[portid].period[prd].min_end = prbuf[2];
				ParaCommPort[portid].period[prd].hour_end = prbuf[3];
				BcdToHex(&ParaCommPort[portid].period[prd].hour_start, 4);
				prbuf += 4;
			}

			//重新初始化抄表时间间隔和抄表日定时器
			if((portid == COMMPORT_CEN_485_1)||(portid == COMMPORT_CEN_485_2)) {
				if(cycle != ParaCommPort[portid].cycle) AutoReadCenMetInit();
				if(rmdhour != ParaCommPort[portid].time_hour || rmdminute != ParaCommPort[portid].time_minute)
					CenMetRmdInit();
			}
		}

		option->ractlen = rlen;
		SaveParaMix(2);
	}

	return 0;
}

/**
* @brief 终端参数F36操作(终端上行通信流量门限设置)
*/
int ParaOperationF36(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		buf[0] = ParaMix.upflow_max & 0xff;
		buf[1] = (ParaMix.upflow_max>>8) & 0xff;
		buf[2] = (ParaMix.upflow_max>>16) & 0xff;
		buf[3] = (ParaMix.upflow_max>>24) & 0xff;
	}
	else {
		ParaMix.upflow_max = ((unsigned int)buf[3]<<24) + ((unsigned int)buf[2]<<16) 
						+ ((unsigned int)buf[1]<<8) + ((unsigned int)buf[0]&0xff);
		SaveParaMix(1);
	}

	return 0;
}

/**
* @brief 终端参数F59操作
*/
int ParaOperationF59(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	if(0 == flag) {
		buf[0] = ParaMix.metabnor.diff;
		buf[1] = ParaMix.metabnor.fly;
		HexToBcd(buf, 2);
		buf[2] = ParaMix.metabnor.stop;
		buf[3] = ParaMix.metabnor.time;
	}
	else {
		ParaMix.metabnor.diff = buf[0];
		ParaMix.metabnor.fly = buf[1];
		BcdToHex(&ParaMix.metabnor.diff, 2);
		ParaMix.metabnor.stop = buf[2];
		ParaMix.metabnor.time = buf[3];
		
		SaveParaMix(1);
	}

	return 0;
}

