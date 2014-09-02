/**
* dbase.h -- ��ʷ���ݿ����ͷ�ļ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-11
* ����޸�ʱ��: 2009-5-11
*/

#ifndef _DBASE_H
#define _DBASE_H

#include "plat_include/timeal.h"
#include "app_include/lib/dbtime.h"

#define DBFREZ_15MIN	1
#define DBFREZ_30MIN	2
#define DBFREZ_1HOUR	3
#define DBFREZ_DAY		4
#define DBFREZ_MONTH	5
#define DBFREZ_CPDAY    6

int DbGetItemNum(unsigned char frez);

//�����ļ���0:15��ʼ,��24:00����
int DbGetItemOffset(int itemnum, dbtime_t dbtime);

int DbaseSave(unsigned short dbid, const unsigned char *buf, unsigned char frez, dbtime_t dbtime);
void DbaseFlush(void);

void DbaseReadLock(void);
void DbaseReadUnlock(void);
unsigned char *DbaseReadCache(void);
int DbaseRead(unsigned short dbid, unsigned char frez, dbtime_t dbtime);

void DbaseDelete(unsigned short dbid);
void DbaseDeleteAll(void);
void DbaseFormat(void);

#define READCACHE_SIZE		3000	

#ifndef def_ReadCache
extern unsigned char ReadCache[READCACHE_SIZE];
#define dbfrez_len     512
#define dbfrez_buffer (ReadCache+(READCACHE_SIZE-dbfrez_len))//1���ÿ�ʼ�����ý���֮�䲻Ӧ�ͷ�ȫ����
#endif

#endif /*_DBASE_H*/

