/**
* terminfo.c -- �ն�������Ϣ
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-9
* ����޸�ʱ��: 2009-5-9
*/

#include <string.h>

#include "plat_include/debug.h"
#include "app_include/version.h"
#include "app_include/lib/bcd.h"
#include "app_include/param/datause.h"
#include "app_include/param/unique.h"
#include "app_include/param/capconf.h"
#include "app_include/cenmet/info.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
#if 0
#ifdef OPEN_DEBUGPRINT
#define CHAR_DEBUGINFO		'D'
#else
#ifdef OPEN_ASSERTLOG
#define CHAR_DEBUGINFO		'T'
#else
#define CHAR_DEBUGINFO		'R'
#endif
#endif


#define VERSIONINFO_LEN		41


static const unsigned char VersionInfo[VERSIONINFO_LEN] = {
	'G', 'W', 'D', 'J',	//���̴���(4)
	'D', 'J', 'G', 'Z', '2', '2', 0, 0,	//�豸���(8)
	'0'+VERSION_MAJOR, '0'+VERSION_MINOR/10, '0'+VERSION_MINOR%10, '0'+VERSION_PROJECT,	//����汾(4)
	0, 0, 0,	//����������ڣ�������(3)
	'6', '4', 'K', '/', '0', '8', 'M', '/', CHAR_DEBUGINFO, 0, 0,	//����������Ϣ��(11)
	'3', '7', '6', '1',	//ͨ��Э��.�汾��(4)
	'0', '0', '0', '1',	//Ӳ���汾��(4)
	0x20, 0x11,0x11//Ӳ����������(3)
};

#define PORTINFO_LEN		41
static const unsigned char PortInfo[PORTINFO_LEN] = {
	0, MAX_ISIG, 0, 0,
	MAX_METER%256, MAX_METER/256, 0x00, 0x04, 0x00, 0x04,
	0, 0, 0, 0, 0, 0, 
	2, 
	0x02, 0x20, 0x00, 0x4b, 0x00, 0x00, MAX_CENMETP%256, MAX_CENMETP/256, 0x00, 0x06, 0x00, 0x06, //RS485_1
	0x03, 0x60, 0x00, 0x4b, 0x00, 0x00, MAX_METER%256, MAX_METER/256, 0x00, 0x06, 0x00, 0x06, //RS485_2
};

#define ELSEINFO_LEN		35
static const unsigned char ElseInfo[ELSEINFO_LEN] = {
	MAX_METP%256, MAX_METP/256, MAX_TGRP, MAX_DTASK_CLS2, MAX_DIFFA, 4, 3, 3,
	3, 3, 3, 31, 12, 0, 0, 0,
	0, 0x7f, 0x00, 16, 16, 16, 16, 16, 
	16, 16, 0, 0, 0, 0, 0, 0, 
	0, 0, 0,
};

static const cfg_datafns_t ValidParamFns = {
	11,
	{
	0xdd, 0x8b, 0x00, 0x03,
	0x69, 0x00, 0x00, 0x04,
	0x00, 0x00, 0x00
	}
};

static const cfg_datafns_t ValidCtrlFns = {
	7,
	{
	0x00, 0x00, 0x00, 0x50,
	0x70, 0x00, 0x04, 
	}
};

static const unsigned char ValidAlarmFlag[8] = {
	0x8D, 0x38, 0x10, 0xc0,
	0x01, 0x00, 0x00, 0x00,
};

/**
* @brief ��ȡF1 �ն˰汾��Ϣ
* @param buf ������ָ��
* @param len ����������, �ɹ�����ʵ�ʶ�ȡ����, ���򷵻�-1
*/
static int ReadVersionInfo(unsigned char *buf, int len)
{
	//unsigned char tmpbuf[32];
	//int i, projnum = 0;
        int i;

	if(len < VERSIONINFO_LEN) return -1;

	memcpy(buf, VersionInfo, VERSIONINFO_LEN);
	
	for(i=0; i<8; i++) {
		if(0 == ParaUni.manuno[i]) break;
		buf[i+4] = ParaUni.manuno[i];
	}
	buf[16] = RELEASE_DATE_DAY;
	buf[17] = RELEASE_DATE_MONTH;
	buf[18] = RELEASE_DATE_YEAR;
	HexToBcd(buf+16, 3);

	i = 0;
	if(i/10 != 0) {
		buf[28] = '0' + i/10;
		buf[29] = '0' + i%10;
	}
	else {
		buf[28] = '0'+i;
	}

#if 0
	if(PrintCMetProtoInfo(tmpbuf) > 0) {
		buf[30] = '0'+tmpbuf[5]/10;
		buf[31] = '0'+tmpbuf[5]%10;
		buf[32] = '0'+tmpbuf[6]/10;
		buf[33] = '0'+tmpbuf[6]%10;
	}
	if(ReadDriverVersion(tmpbuf, 4) > 0) {
		buf[34] = '0' + tmpbuf[0];
		buf[35] = '0' + tmpbuf[1];
		buf[36] = '0' + tmpbuf[2];
		buf[37] = '0' + tmpbuf[3];
	}

#endif
	/*
	if(ReadDriverVersion(tmpbuf, 8) > 0) {
		buf[34] = '0'+tmpbuf[0]/10;
		buf[35] = '0'+tmpbuf[0]%10;
		buf[36] = '0'+tmpbuf[1]/10;
		buf[37] = '0'+tmpbuf[1]%10;
		buf[38] = tmpbuf[4];
		buf[39] = tmpbuf[3];
		buf[40] = tmpbuf[2];
	}*/

	return VERSIONINFO_LEN;
}

/**
* @brief ��ȡF2 �ն�֧�ֵ����롢�����ͨ�Ŷ˿�����
*/
static int ReadPortInfo(unsigned char *buf, int len)
{
       if(len < PORTINFO_LEN) return -1;

       memcpy(buf, PortInfo, PORTINFO_LEN);
	
/*       if((ParaUni.uplink&0x0f) == UPLINKITF_ETHER){       
               memset(buf+10, 0, 1);
               memcpy(buf+10+1, ParaUni.addr_area, 2);
               memcpy(buf+10+3, ParaUni.addr_sn, 2);	
               memset(buf+10+5, 0, 1);
       }
       else{*/
               memcpy(buf+10, ParaUni.addr_mac, 6);
       //}

       return PORTINFO_LEN;
}

/**
* @brief ��ȡF3 �ն�֧�ֵ���������
*/
static int ReadElseInfo(unsigned char *buf, int len)
{
	if(len < ELSEINFO_LEN) return -1;

	memcpy(buf, ElseInfo, ELSEINFO_LEN);

	return ELSEINFO_LEN;
}

/**
* @brief ��ȡF4 �ն�֧�ֵĲ�������
*/
static int ReadValidParamFns(unsigned char *buf, int len)
{
	int alen;

	alen = ((int)ValidParamFns.num&0xff) + 1;
	if(len < alen) return -1;

	memcpy(buf, &ValidParamFns.num, alen);

	return alen;
}

/**
* @brief ��ȡF5 �ն�֧�ֵĿ�������
*/
static int ReadValidCtrlFns(unsigned char *buf, int len)
{
	int alen;

	alen = ((int)ValidCtrlFns.num&0xff) + 1;
	if(len < alen) return -1;

	memcpy(buf, &ValidCtrlFns.num, alen);

	return alen;
}

/**
* @brief ��ȡF6 �ն�֧�ֵ�1����������
*/
static int ReadValidDataCls1Fns(unsigned char *buf, int len)
{
	int alen, tmplen, i;

	alen = 2;
	if(len < alen) return -1;
	*buf++ = 0x3e;
	*buf++= 0;

	for(i=0; i<2; i++) {
		tmplen = ((int)ValidDataCls1_1.num&0xff)+1;
		alen += tmplen;
		if(len < alen) return -1;
		memcpy(buf, &ValidDataCls1_1.num, tmplen);
		buf += tmplen;
	}

	for(i=0; i<2; i++) {
		tmplen = ((int)ValidDataCls1_2.num&0xff)+1;
		alen += tmplen;
		if(len < alen) return -1;
		memcpy(buf, &ValidDataCls1_2.num, tmplen);
		buf += tmplen;
	}

	tmplen = ((int)ValidDataCls1_1.num&0xff)+1;
	alen += tmplen;
	if(len < alen) return -1;
	memcpy(buf, &ValidDataCls1_1.num, tmplen);
	buf += tmplen;

	return alen;
}

/**
* @brief ��ȡF7 �ն�֧�ֵ�2����������
*/
static int ReadValidDataCls2Fns(unsigned char *buf, int len)
{
	int alen, tmplen, i;

	alen = 2;
	if(len < alen) return -1;
	*buf++ = 0x3e;
	*buf++ = 0;
	
	for(i=0; i<2; i++) {
		tmplen = ((int)ValidDataCls2_1.num&0xff)+1;
		alen += tmplen;
		if(len < alen) return -1;
		memcpy(buf, &ValidDataCls2_1.num, tmplen);
		buf += tmplen;
	}

	for(i=0; i<2; i++) {
		tmplen = ((int)ValidDataCls2_2.num&0xff)+1;
		alen += tmplen;
		if(len < alen) return -1;
		memcpy(buf, &ValidDataCls2_2.num, tmplen);
		buf += tmplen;
	}

	tmplen = ((int)ValidDataCls2_1.num&0xff)+1;
	alen += tmplen;
	if(len < alen) return -1;
	memcpy(buf, &ValidDataCls2_1.num, tmplen);
	buf += tmplen;

	return alen;
}

/**
* @brief ��ȡF7 �ն�֧�ֵ�2����������
*/
static int ReadValidAlarmFlag(unsigned char *buf, int len)
{
	if(len < 8) return -1;

	memcpy(buf, ValidAlarmFlag, 8);
	return 8;
}
#endif
/**
* @brief ��ȡF6 �ն˰汾��Ϣ
* @param buf ������ָ��
* @param len ����������, �ɹ�����ʵ�ʶ�ȡ����, ���򷵻�-1
*/
#include "mkdate.c"
static int ReadVersionInfox(unsigned char *buf, int len)
{
	memset(buf,0,16);
	strcpy((char*)buf,MKTIME);
	return 16;
}
typedef struct {
	unsigned char bend;
	unsigned char grpid;
	unsigned char fnmask;
	int (*pfunc)(unsigned char *, int);
} readinfo_list_t;
static const readinfo_list_t ReadInfoList[] = {
	/*F1  *///{0, 0, 0x01, ReadVersionInfo},
	/*F2  *///{0, 0, 0x02, ReadPortInfo},
	/*F3  *///{0, 0, 0x04, ReadElseInfo},
	/*F4  *///{0, 0, 0x08, ReadValidParamFns},
	/*F5  *///{0, 0, 0x10, ReadValidCtrlFns},
	/*F6  *///{0, 0, 0x20, ReadValidDataCls1Fns},
	/*F7  *///{0, 0, 0x40, ReadValidDataCls2Fns},
	/*F8  *///{0, 0, 0x80, ReadValidAlarmFlag},
	/*F16  */ {0, 1, 0x80, ReadVersionInfox},
	{1, 0, 0, NULL},
};

/**
* @brief ��ȡ�ն�������Ϣ
* @param pnfn pn,fn��Ϣָ��
* @param buf ������ָ��
* @param len ����������, �ɹ�����ʵ�ʶ�ȡ����, ���򷵻�-1
*/
int ReadTermInfo(const unsigned char *pnfn, unsigned char *buf, int len)
{
	const readinfo_list_t *plist = ReadInfoList;
	unsigned char grpid, fns, echofn;
	unsigned char *pbak = buf;
	int actlen, rtn;

	if(0 != pnfn[0] || 0 != pnfn[1] || len <= 4) return -1;

	grpid = pnfn[3];
	fns = pnfn[2];

	buf[0] = buf[1] = buf[2] = 0;
	buf[3] = pnfn[3];
	buf += 4;
	actlen = 4;
	len -= 4;
	echofn = 0;

	for(; 0==plist->bend; plist++) {
		if(grpid != plist->grpid || 0 == (fns&plist->fnmask)) continue;

		rtn = (*plist->pfunc)(buf, len);
		if(rtn < 0) break;

		echofn |= plist->fnmask;
		buf += rtn;
		actlen += rtn;
		len -= rtn;
		if(len < 0) return -1;
	}

	if(0 == echofn) return -1;
	pbak[2] = echofn;
	return actlen;
}
