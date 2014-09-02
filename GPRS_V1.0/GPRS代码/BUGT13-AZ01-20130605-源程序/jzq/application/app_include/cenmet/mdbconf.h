/**
* mdbconf.h -- 表计数据配置
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-11
* 最后修改时间: 2009-5-11
*/

#ifndef _MDB_CONF_H
#define _MDB_CONF_H

#include "app_include/cenmet/dbase.h"

//表计费率数
#define MAXNUM_FEEPRD   4
#define MAXNUM_METPRD	(MAXNUM_FEEPRD+1)

//曲线数据冻结密度
//#define CURVE_FREZ		1  // 15 minute
#define CURVE_FREZ		DBFREZ_1HOUR      // 60 minute

#endif /*_MDB_CONF_H*/
