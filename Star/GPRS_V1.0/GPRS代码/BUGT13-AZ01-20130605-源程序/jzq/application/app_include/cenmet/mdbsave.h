/**
* mdbsave.h -- 状态数据保存
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-14
* 最后修改时间: 2009-5-14
*/

#ifndef _MDB_SAVE_H
#define _MDB_SAVE_H

//void SaveMdbUene(void);
void SaveMdbStic(void);
//void SaveMdbTGrp(void);
void SaveMdb(void);

void ResetMdbDay(void);
void ResetMdbMonth(void);

void BakupMdb(void);

#endif /*_MDB_SAVE_H*/

