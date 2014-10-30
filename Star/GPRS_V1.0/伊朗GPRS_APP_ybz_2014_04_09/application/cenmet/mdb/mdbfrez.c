/**
* dbase.c -- 小时冻结数据
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-13
* 最后修改时间: 2009-5-13
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plat_include/debug.h"
#include "plat_include/timeal.h"
#include "app_include/param/capconf.h"
#include "app_include/param/meter.h"
#include "app_include/param/metp.h"
#include "app_include/lib/bcd.h"
#include "app_include/cenmet/mdbfrez.h"
#include "app_include/cenmet/mdbcur.h"
#include "app_include/cenmet/mdbconf.h"
#include "app_include/cenmet/dbconfig.h"
#include "app_include/cenmet/dbase.h"
#include "app_include/cenmet/cenmet_comm.h"


//由定时器锁定的时间, 避免由于抄表等长时操作使冻结延误
static utime_t UTimeFrez;
void SetUTimeFrez(utime_t utime)
{
	UTimeFrez = utime;
}

utime_t GetUTimeFrez(void)
{
	return UTimeFrez;
}


/**
* @brief 保存测量点曲线冻结数据
* @param clock 当前时间
*/
static void UpdateMdbFrezMetp(const sysclock_t *clock)
{
	unsigned short metersn;
	db_metcurve_t *pbuffer = (db_metcurve_t *)dbfrez_buffer;
	//unsigned char minu;
	dbtime_t dbtime;
	unsigned short MetCuvItems[MAX_READITEM_NUM];
	unsigned short MetCuvItemsNum;

#if 0
	minu = clock->minute / 15;

#if CURVE_FREZ == 1
#elif CURVE_FREZ == 2	//冻结密度30分钟,在0分和30分冻结
	if(0 != minu && 2 != minu) return;
#elif CURVE_FREZ == 3	//冻结密度60分钟,在0分冻结
	if(0 != minu) return;
#else
	return;
#endif
#endif

	SYSCLOCK_DBTIME(clock, dbtime);

	for(metersn=1; metersn<=MAX_CENMETP; metersn++) {  
		if(EMPTY_CENMETP(metersn-1)) continue;
		if(metcommchk(metersn-1)) continue;
		
		PrintLog(LOGTYPE_DEBUG,"cuv hourfrez metersn(%d)\n",metersn);
		
		MetCuvItemsNum = MetReadItemsChk(metersn,MetCuvItems,CURVE_FREZ);
		
		ReadImmMdbCur(metersn,MetCuvItems,MetCuvItemsNum,MdbCurIdSave);

		LockMdbCurrent();
		smallcpy(pbuffer->pwra, MdbCurrent[MdbCurIdSave].pwra, 50);

		smallcpy(pbuffer->enepa, &MdbCurrent[MdbCurIdSave].enepa[1], 4);
		smallcpy(pbuffer->enepi, MdbCurrent[MdbCurIdSave].enepi, 4);
		smallcpy(pbuffer->enena, &MdbCurrent[MdbCurIdSave].enena[1], 4);
		smallcpy(pbuffer->eneni, MdbCurrent[MdbCurIdSave].eneni, 4);

		//smallcpy(pbuffer->vol_arc, MdbCurrent.phase_arc, 12);
		//4象限无功电能量
		smallcpy(pbuffer->enepi1, MdbCurrent[MdbCurIdSave].enepi1, 4);
		smallcpy(pbuffer->enepi4, MdbCurrent[MdbCurIdSave].enepi4, 4);
		smallcpy(pbuffer->eneni2, MdbCurrent[MdbCurIdSave].eneni2, 4);
		smallcpy(pbuffer->eneni3, MdbCurrent[MdbCurIdSave].eneni3, 4);
		UnlockMdbCurrent();

		DbaseSave(DBID_MET_CURVE(metersn-1), (unsigned char *)pbuffer, CURVE_FREZ, dbtime);//@later
	}
}

/**
* @brief 保存曲线冻结数据
*/
void UpdateMdbFrez(void)
{
	sysclock_t clock;

	UTimeToSysClock(UTimeFrez, &clock);
	UpdateMdbFrezMetp(&clock);
}

typedef struct {
	unsigned short itemid;
	unsigned short dbid;
	unsigned char len;
	unsigned char offset;
} mdbfrez_list_t;
static const mdbfrez_list_t cons_mdbfrez_list[] = {
	{0x0b01, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, pwra[0])},
	{0x0b02, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, pwra[3])},
	{0x0b04, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, pwra[6])},
	{0x0b08, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, pwra[9])},
	{0x0b10, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, pwri[0])},
	{0x0b20, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, pwri[3])},
	{0x0b40, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, pwri[6])},
	{0x0b80, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, pwri[9])},

	{0x0e01, DBID_MET_CURVE(0), 2, DB_OFFSET(db_metcurve_t, pwrf[0])},
	{0x0e02, DBID_MET_CURVE(0), 2, DB_OFFSET(db_metcurve_t, pwrf[2])},
	{0x0e04, DBID_MET_CURVE(0), 2, DB_OFFSET(db_metcurve_t, pwrf[4])},
	{0x0e08, DBID_MET_CURVE(0), 2, DB_OFFSET(db_metcurve_t, pwrf[6])},

	{0x0c01, DBID_MET_CURVE(0), 2, DB_OFFSET(db_metcurve_t, vol[0])},
	{0x0c02, DBID_MET_CURVE(0), 2, DB_OFFSET(db_metcurve_t, vol[2])},
	{0x0c04, DBID_MET_CURVE(0), 2, DB_OFFSET(db_metcurve_t, vol[4])},
	{0x0c08, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, amp[0])},
	{0x0c10, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, amp[3])},
	{0x0c20, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, amp[6])},
	{0x0c40, DBID_MET_CURVE(0), 3, DB_OFFSET(db_metcurve_t, amp[9])},

	{0x0d01, DBID_MET_CURVE(0), 4, DB_OFFSET(db_metcurve_t, uenepa[0])},
	{0x0d02, DBID_MET_CURVE(0), 4, DB_OFFSET(db_metcurve_t, uenepi[0])},
	{0x0d04, DBID_MET_CURVE(0), 4, DB_OFFSET(db_metcurve_t, uenena[0])},
	{0x0d08, DBID_MET_CURVE(0), 4, DB_OFFSET(db_metcurve_t, ueneni[0])},
	{0x0d10, DBID_MET_CURVE(0), 4, DB_OFFSET(db_metcurve_t, enepa[0])},
	{0x0d20, DBID_MET_CURVE(0), 4, DB_OFFSET(db_metcurve_t, enepi[0])},
	{0x0d40, DBID_MET_CURVE(0), 4, DB_OFFSET(db_metcurve_t, enena[0])},
	{0x0d80, DBID_MET_CURVE(0), 4, DB_OFFSET(db_metcurve_t, eneni[0])},

	//{0x0f01, DBID_ANASIG_CURVE(0), 2, DB_OFFSET(db_anasig_curve_t, value[0])},

	{0, 0, 0},
};

/**
* @brief 读取当前表计小时冻结数据
* @param metpid 测量点号, 1~MAX_CENMETP
* @param itemid 数据项编号
* @param buf 缓存区指针
* @param len 缓存区长度
* @return 成功返回实际读取长度, 失败返回负数, 无此数据项返回-2, 缓存区溢出返回-1
*/
int ReadMdbFrez(unsigned short metpid, unsigned short itemid, unsigned char *buf, int len)
{
	unsigned short dbid;
	const mdbfrez_list_t *plist = (const mdbfrez_list_t *)cons_mdbfrez_list;
	utime_t utime;
	int offset, itemlen;
	int cpynum, i;
	unsigned char *pbuffer;
	dbtime_t dbtime;

	if((metpid==0) ||(metpid > MAX_CENMETP)) return -2;
	metpid -= 1;
	
	while(0 != plist->itemid) {
		if(itemid == plist->itemid) break;
		plist++;
	}
	if(0 == plist->itemid) return -2;

	dbid = plist->dbid;
	dbid += metpid;

#if CURVE_FREZ == 1  // 15min
	cpynum = 4; i=900;
#elif CURVE_FREZ == 2  // 30min
	cpynum = 2; i=1800;
#elif CURVE_FREZ == 3  // 60min
	cpynum = 1; i=3600;
#else
	return -2;
#endif

	if((cpynum*plist->len+2) > len) return -1;
	len = cpynum*plist->len+2;

	utime = UTimeReadCurrent();
#if CURVE_FREZ != 3
	utime -= 3600;  // one hour before
	utime = utime - (utime%3600);
#endif

	*buf = (unsigned char)((utime/3600)%24);
	HexToBcd(buf, 1);
	buf++;
	*buf = CURVE_FREZ;
	buf++;

#if CURVE_FREZ != 3
	utime += i;
#endif

	UTIME_DBTIME(utime, dbtime);
	itemlen = DbaseItemSize(dbid);
	i = DbGetItemNum(CURVE_FREZ);
	offset = DbGetItemOffset(i, dbtime);

	//@change later: 23:15~24:00中的24:00看不到

#if CURVE_FREZ != 3
	if((offset + cpynum) >= i) {
		cpynum -= 1;
	}
#endif

	if(DbaseRead(dbid, CURVE_FREZ, dbtime) < 0) {
		return -2;
	}
	pbuffer = DbaseReadCache();
	pbuffer += offset*itemlen;

	for(i=0; i<cpynum; i++,buf+=plist->len,pbuffer+=itemlen) {
		smallcpy(buf, pbuffer+plist->offset, plist->len);
	}

	return len;
}

