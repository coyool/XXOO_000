/**
* cuvupdate.h -- 历史数据库冻结(日,月, 抄表日)
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-17
* 最后修改时间: 2009-5-17
*/

#ifndef _MDB_CUVUPDATE_H
#define _MDB_CUVUPDATE_H

void UpdateCurveDay(void);
void UpdateCurveMonth(void);
void UpdateCurveRmd(void);
unsigned short MetReadItemsChk(unsigned short mid, unsigned short *itemid,unsigned char type);

#endif /*_MDB_CUVUPDATE_H*/

