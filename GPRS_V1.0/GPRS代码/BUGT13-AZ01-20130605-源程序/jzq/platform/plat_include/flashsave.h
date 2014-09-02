/*                                                                **
* flash save 头文件
*
*/

#ifndef _SYS_FLASHSVAE_H
#define _SYS_FLASHSVAE_H

#include "app_include/lib/dbtime.h"
#include "hal_include/rtc.h"


#define DB_SAVE_DAY		0    //日冻结        (370 400)
#define DB_SAVE_CPYDAY	1    //抄表日冻结    (316 400)
#define DB_SAVE_MONTH	2    //月冻结        (370 400)
#define DB_SAVE_CURVE	3    //曲线          (98  100)
#define DB_SAVE_TERMDAY	4    //终端日冻结    (18  36)
#define DB_SAVE_TERMMON	5    //终端月冻结    (18  36)

#define PARA_FILEINDEX_COMB		1          //(-,1024)
#define PARA_FILEINDEX_METER	2          //(1792 4000)
#define PARA_FILEINDEX_METP		3          //(5376 5500)
#define PARA_FILEINDEX_DATAUSE1      4     //(3584 4000)
#define PARA_FILEINDEX_DATAUSE2      5     //(3584 4000)
#define TERM_FILEINDEX_RUNSTATE      6     //(52 128)
#define TERM_FILEINDEX_MDBSTIC       7     //(20 128)
//rn 0 ok 
//rt -1 failed 
int db_readflash_bin(unsigned char frztype,unsigned int metid,void *buf,unsigned short len,dbtime_t dbtime);
int db_writeflash_bin(unsigned char frztype,unsigned int metid,const void *buf,unsigned short len,dbtime_t dbtime);
int db_rmflash_bin(unsigned char frztype,unsigned int metid);
int db_rmflash_interval_bin(dbtime_t time1,dbtime_t time2);
int db_rmflash_all_bin(void);
void db_flash_time(utime_t mintime,utime_t bigtime);

int para_readflash_bin(unsigned char idx,void *buf,unsigned short len);
int para_writeflash_bin(unsigned char idx,const void *buf,unsigned short len);
int para_rmflash_bin(unsigned char idx);

 
//dbid 0 imp 1 nor 
//cur 0-255
//maxlen 100
int alarm_writeflash_bin(unsigned char dbid,const void* buf,unsigned short len);
int alarm_readflash_bin(unsigned char dbid,unsigned int cur,void* buf,unsigned short len);
int alarm_rmflash_bin(void);



#endif
