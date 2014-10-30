/**
* dbconfig.h -- ��ʷ���ݿ�����
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-11
* ����޸�ʱ��: 2009-5-11
*/

#ifndef _DB_CONFIG_H
#define _DB_CONFIG_H

#include "app_include/lib/dbtime.h"

#define DBID_MET_DAY(mid)			((unsigned short)0+(mid)) 
#define DBID_MET_CPYDAY(mid)		((unsigned short)80+(mid))
#define DBID_MET_MONTH(mid)			((unsigned short)160+(mid))
#define DBID_MET_CURVE(mid)			((unsigned short)240+(mid))
#define DBID_TERMSTIC_DAY			((unsigned short)321)
#define DBID_TERMSTIC_MONTH			((unsigned short)322)

#define DBID_END					323

#define DBSAVE_DAY		0    //�ն���
#define DBSAVE_CPYDAY	1    //�����ն���
#define DBSAVE_MONTH	2    //�¶���
#define DBSAVE_CURVE	3    //����

#define DBATTR_METP		0    //����������
#define DBATTR_TGRP		1    //�ܼ�������
#define DBATTR_TERM		2    //�ն�����
#define DBATTR_UMETP	3    //�û�������
#define DBATTR_IMPUSR	4    //�ص��û�������
#define DBATTR_ANASI	5   //ֱ��ģ��������

#define DBFLAG_RDTIME	0x01	//������ʱ��
#define DBFLAG_FENUM	0x02   //��������
#define DBFLAG_SPECIAL	0x04  //���⴦��(�����ݲ��ǰ�˳������)

typedef struct {
	unsigned char flag;    //���ݱ�־
	unsigned char len;    //�������ݳ���
	unsigned int offset;    //�������ݵ�ַ�������ݿ����ַ��ƫ��
} dbsonconfig_t;

typedef struct {
	unsigned char grpid;    //���ʶ, F1~F8=0, F9~F16=1,...
	unsigned char cids;   //���ݵ�Ԫ����
	unsigned short dbid;   //���ݿ�ID
	unsigned char type;   //���ݿ�����
	unsigned char attr;    //���ݿ�����
	unsigned short colen;   //���ݿ��г���, ���в��������ݴ���һ���ļ�ʱ��
	const dbsonconfig_t *psons;
} dbaseconfig_t;
extern const dbaseconfig_t DbaseConfig[];

//#define DB_OFFSET(type, var)	((unsigned int)(&(((type *)0)->var)))
#define DB_OFFSET(type, var)	((unsigned int)((char*)&(((type *)0)->var)-(char*)0))

int DbaseItemSize(unsigned short dbid);
int DbaseItemType(unsigned short dbid,unsigned char *frztype);

//int DbaseFileName(char *filename, unsigned short dbid, dbtime_t dbtime);
//int DbaseFileGroupName(char *filename, unsigned short dbid);
int DbaseGetMetid(unsigned short dbid,unsigned short *metid);

#endif /*_DB_CONFIG_H*/

