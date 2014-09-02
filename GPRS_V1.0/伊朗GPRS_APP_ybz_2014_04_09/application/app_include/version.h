/**
* version.h -- 版本号头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-3-31
* 最后修改时间: 2009-5-6
*/

#ifndef _VERSION_H
#define _VERSION_H

//工程版本号
#define VERPROJ_MAIN		0

#define VERSION_MAJOR		0   //主版本号

#define VERSION_MINOR		03   //次版本号

#define VERSION_PROJECT		2   //工程版本号


//版本发布日期
#if 1
#define RELEASE_DATE_YEAR	12
#define RELEASE_DATE_MONTH 2
#define RELEASE_DATE_DAY	21
#else
extern const unsigned char _mk_year;
extern const unsigned char _mk_month;
extern const unsigned char _mk_day;
extern const unsigned char _mk_hour;
extern const unsigned char _mk_minute;
#endif

#endif /*_VERSION_H*/

