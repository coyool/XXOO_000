/**
* dbase.c -- 历史数据库操作
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-11
* 最后修改时间: 2009-5-11
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define def_ReadCache

#include "plat_include/debug.h"
#include "plat_include/timeal.h"
#include "plat_include/bin.h"
#include "plat_include/mutex.h"
#include "plat_include/syslock.h"
#include "app_include/lib/dbtime.h"
#include "app_include/param/capconf.h"
#include "app_include/cenmet/dbconfig.h"
#include "app_include/cenmet/dbase.h"
#include "plat_include/flashsave.h"
#include "app_include/cenmet/mdbcur.h"

unsigned char ReadCache[READCACHE_SIZE];//@ "AHB_RAM_MEMORY";//调用开始到调用结束之间不应释放全局锁

//static int LockIdDbase = -1;
//static sys_mutex_t DbReadMutex;

#define DB_WRLOCK		LockSysLock(LockIdDbase)
#define DB_WRUNLOCK		UnlockSysLock(LockIdDbase)

/**
* @brief 锁住数据库读取(读取数据库数据之前必须锁住)
*/
void DbaseReadLock(void)
{
	//DB_WRLOCK;
	//SysLockMutex(&DbReadMutex);
}

/**
* @brief 解锁数据库读取(读取数据库数据之后必须解锁)
*/
void DbaseReadUnlock(void)
{
	//DB_WRUNLOCK;
	//SysUnlockMutex(&DbReadMutex);
}

/**
* @brief 获得读取数据库的结果数据缓存
* @return 结果数据缓存区指针
*/
unsigned char *DbaseReadCache(void)
{
	//return(ReadCache+sizeof(dbhead_t));
	return(ReadCache);
}
int DbGetItemNum(unsigned char frez)
{
	switch(frez) {
	case DBFREZ_15MIN: return 96; 
	case DBFREZ_30MIN: return 48; 
	case DBFREZ_1HOUR: return 24; 
	case DBFREZ_DAY:
	case DBFREZ_CPDAY:
	case DBFREZ_MONTH: return 1; 
	default: return 0;
	}
}
int DbGetItemOffset(int itemnum, dbtime_t dbtime)
{
	int idx;

	if(1 == itemnum) return 0;

	idx = (int)dbtime.s.tick & 0xff;
	idx = idx/(96/itemnum);
	/*if(0 == idx) idx = itemnum - 1;
	else idx -= 1;*/

	return idx;
}
/**
* @brief 保存历史数据库
* @param dbid 数据ID
* @param buf 缓存区指针
* @param frez 冻结密度
* @param clock 当前时钟
* @return 成功返回0, 否则失败
*/
int DbaseSave(unsigned short dbid, const unsigned char *buf, unsigned char frez, dbtime_t dbtime)
{
	int itemlen;
	unsigned short metid;
	unsigned char frztype;
	int rt;

	itemlen = DbaseItemSize(dbid);	/*每项大小*/
	AssertLogReturn(itemlen<=0, 1, "invalid dbid(%d)\n", dbid);
	rt = DbaseItemType(dbid,&frztype);
	AssertLogReturn(rt, 1, "invalid dbid(%d)\n", dbid);
	rt = DbaseGetMetid(dbid,&metid);
	AssertLogReturn(rt, 1, "invalid dbid(%d)\n", dbid);
	
	rt = db_writeflash_bin(frztype,metid,buf,itemlen,dbtime);
	
	//AssertLogReturn(rt,-1,"dbase save fail frez(%d)metid(%d)",frztype,metid);
	AssertLog(rt==-1,"dbase save fail frez(%d)metid(%d)\n",frztype,metid);
	
	PrintLog(LOGTYPE_DEBUG,"dbase save frez(%d) metid(%d) len(%d) rt(%d)\n",frztype,metid,itemlen,rt);
	PrintHexLog(LOGTYPE_DEBUG,buf,itemlen);

	return 0;
}

/**
* @brief 读取历史数据库数据
* @param dbid 数据ID
* @param frez 冻结密度
* @param clock 查询时钟
* @return 成功返回实际读取长度, 失败返回-1
*/
int DbaseRead(unsigned short dbid, unsigned char frez, dbtime_t dbtime)
{
	int itemlen,itemnum;
	unsigned short metid;
	unsigned char frztype;
	int rt,rtn=-1;

	PrintLog(LOGTYPE_DEBUG,"dbase read dbid(%d) frez(%d) dbtime:%d-%d-%d",dbid,frez,dbtime.s.year,dbtime.s.month,dbtime.s.day);

	rt = DbaseItemType(dbid,&frztype);
	AssertLogReturn(rt, -1, "invalid dbid(%d)\n", dbid);
	rt = DbaseGetMetid(dbid,&metid);
	AssertLogReturn(rt, -1, "invalid dbid(%d)\n", dbid);
	
	itemlen = DbaseItemSize(dbid);
	if(itemlen <= 0) return -1;

	itemnum = DbGetItemNum(frez);	/*根据冻结密度获取项数*/
	AssertLogReturn(0==itemnum, -1, "invalid frez(%d)\n", frez);
	
	PrintLog(LOGTYPE_DEBUG,"metid(%d) frztype(%d) itemnum(%d) itemlen(%d)\n",metid,frztype,itemnum,itemlen);

	if(itemnum>1)
	{
		int i;
		dbtime_t dbtime1;
		unsigned char * pcahe = ReadCache;

		dbtime1.s.year = dbtime.s.year;
		dbtime1.s.month = dbtime.s.month;
		dbtime1.s.day = dbtime.s.day;
		dbtime1.s.tick = 0;
		if(itemnum*itemlen>sizeof(ReadCache))   return -1;
		for(i=0;i<itemnum;i++)
		{
			rt = db_readflash_bin(frztype,metid,pcahe,itemlen,dbtime1);
			if(rt == itemlen)  rtn = 0;
			else memset(pcahe,FLAG_MDBFAIL,itemlen);
			
			if(frez==DBFREZ_1HOUR) dbtime1.s.tick+=4;
			else dbtime1.s.tick+=frez;
			pcahe += itemlen;
		}
		
		if(rtn == 0)  return itemlen*itemnum;
		else rtn = -1;
	}
	else
	{
		rtn = db_readflash_bin(frztype,metid,ReadCache,itemlen,dbtime);

		if(rtn == itemlen) return itemlen;
		else return -1;
	}

	//PrintHexLog(LOGTYPE_DEBUG,ReadCache,itemlen*itemnum);
        return -1;
}

void DbaseDelete(unsigned short dbid)
{

}

void DbaseDeleteAll(void)
{
	//SysRemoveFiles(DBASE_SAVEPATH "*.db");
	
	//SystemCmd("rm -f " DBASE_SAVEPATH "*.db");
	//db_rmfalsh_bin();
	//alarm_rmfalsh_bin();
}

void DbaseFormat(void)
{
	//int metersn;

	//for(metersn=1;metersn<MAX_METER;metersn++){
		//DebugPrint(0,"DbaseFormat...type(DB) metersn(%d)\n",metersn);
		//db_rmflash_bin(DB_SAVE_DAY,metersn-1);
		//db_rmflash_bin(DB_SAVE_CPYDAY,metersn-1);
		//db_rmflash_bin(DB_SAVE_MONTH,metersn-1);
		//db_rmflash_bin(DB_SAVE_CURVE,metersn-1);
	//}
	db_rmflash_all_bin();

	//DebugPrint(0,"rm DB_SAVE_TERMDAY\n");
	//db_rmflash_bin(DB_SAVE_TERMDAY,0);
	//DebugPrint(0,"rm DB_SAVE_TERMMON\n");
	//db_rmflash_bin(DB_SAVE_TERMMON,0);
	
	//DebugPrint(0,"DbaseFormat...TERM_FILEINDEX_RUNSTATE\n");
	para_rmflash_bin(TERM_FILEINDEX_RUNSTATE);
	//DebugPrint(0,"DbaseFormat...TERM_FILEINDEX_MDBSTIC\n");	
	para_rmflash_bin(TERM_FILEINDEX_MDBSTIC);
	
	//DebugPrint(0,"DbaseFormat...alarm\n");	
	alarm_rmflash_bin();
	DebugPrint(0,"DbaseFormat finish...\n");	
}

//extern int DbClearInit(void);//@later

//DECLARE_INIT_FUNC(DbaseInit);
int DbaseInit(void)
{
	//DbClearInit();//@later

//	LockIdDbase = RegisterSysLock();
	//SysInitMutex(&DbReadMutex);

	//for(mid=0; mid<DBID_END; mid++) WriteCacheCuv[mid] = NULL;

	DebugPrint(0, "db met day(%d) item size = %d\n", DBID_MET_DAY(0), DbaseItemSize(DBID_MET_DAY(0)));
	DebugPrint(0, "db met rmd(%d) item size = %d\n", DBID_MET_CPYDAY(0), DbaseItemSize(DBID_MET_CPYDAY(0)));
	DebugPrint(0, "db met mon(%d) item size = %d\n", DBID_MET_MONTH(0), DbaseItemSize(DBID_MET_MONTH(0)));
	//DebugPrint(0, "db met sticday(%d) item size = %d\n", DBID_METSTIC_DAY(0), DbaseItemSize(DBID_METSTIC_DAY(0)));
	//DebugPrint(0, "db met sticmon(%d) item size = %d\n", DBID_METSTIC_MONTH(0), DbaseItemSize(DBID_METSTIC_MONTH(0)));
	DebugPrint(0, "db term sticday(%d) item size = %d\n", DBID_TERMSTIC_DAY, DbaseItemSize(DBID_TERMSTIC_DAY));
	DebugPrint(0, "db term sticmon(%d) item size = %d\n", DBID_TERMSTIC_MONTH, DbaseItemSize(DBID_TERMSTIC_MONTH));
	DebugPrint(0, "db met curve(%d) item size = %d\n", DBID_MET_CURVE(0), DbaseItemSize(DBID_MET_CURVE(0)));

	//SET_INIT_FLAG(DbaseInit);
	return 0;
}

