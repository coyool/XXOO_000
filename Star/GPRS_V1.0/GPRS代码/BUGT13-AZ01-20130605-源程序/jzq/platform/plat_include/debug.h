/**
* debug.h -- 调试信息管理头文件
* 实现中...
* 作者: zhuzhiqiang
* 创建时间: 2009-4-3
* 最后修改时间: 2009-4-27
*/

#ifndef _DEBUG_H
#define _DEBUG_H


//#include "print.h"
#include "app_include/version.h"
#include "sleep.h"
#include "mutex.h"

#define Sleep(ticks) sleep(ticks)



#define OPEN_ASSERTLOG
#define OPEN_DEBUGPRINT

//配置(主版本为0才能打开调试)
#if VERSION_MAJOR == 0
#define OPEN_DEBUGPRINT
#else
#undef OPEN_DEBUGPRINT
#endif

/************/
#if 0
#define LOGTYPE_CLOSE			0  //关闭
#define LOGTYPE_ALARM			1  //告警
#define LOGTYPE_SHORT			2  //简短信息
#define LOGTYPE_CENMET			3  //下行通信信息
#define LOGTYPE_UPLINK			4  //上行通信信息
#define LOGTYPE_DOWNLINK		5  //载波数据打印
#define LOGTYPE_DEBUG			6  //调试打印信息
#define LOGTYPE_TEST			10 //测试打印
#define LOGTYPE_ONLYSAVE		60
#else
#define LOG_OPEN   PRINT_NORMAL
#define LOG_CLOSE  PRINT_NORMAL+3

#define LOGTYPE_CLOSE			0  //关闭
#define LOGTYPE_ALARM			1  //告警
#define LOGTYPE_SHORT			2  //简短信息
#define LOGTYPE_CENMET			3  //下行通信信息
#define LOGTYPE_UPLINK			4  //上行通信信息
#define LOGTYPE_DOWNLINK		5  //载波数据打印
#define LOGTYPE_DEBUG			6  //调试打印信息
#endif

/*
*@brief 设置打印输出状态
*@param State输出状态 1输出到调试串口,0不输出
*/
void DebugLog(unsigned char State);

//打印运行调试信息
//#define PrintLog  print

void PrintLog(int type, const char *format, ...);

void PrintHexLog(int type, const unsigned char *buf, int len);


//调试打印,正式运行版不出现
#ifdef OPEN_DEBUGPRINT
#define DebugPrint		PrintLog
#else
#define DebugPrint(a, ...)
#endif

/****************/
//正式运行版本不出现
#ifdef OPEN_ASSERTLOG

//#define PATH_ASSERTLOG		"/tmp/"

//发生非正常错误时使用,将信息写入日志中
void PrintErrorLog(int bexit, const char *file, int line, const char *format, ...);

#define ErrorExit(format...) { \
	PrintErrorLog(1, __FILE__, __LINE__, format); \
}

#define ErrorLog(format...) { \
	PrintErrorLog(0, __FILE__, __LINE__, format); \
}

#define AssertLog(cond, format...) { \
	if(cond) { \
		PrintErrorLog(0, __FILE__, __LINE__, format); \
	} \
}

#define AssertLogReturn(cond, rtn, format...) { \
	if(cond) { \
		PrintErrorLog(0, __FILE__, __LINE__, format); \
		return rtn; } \
}

#define AssertLogReturnVoid(cond, format...) { \
	if(cond) { \
		PrintErrorLog(0, __FILE__, __LINE__, format); \
		return; } \
}

#else /*OPEN_ASSERTLOG*/

#define ErrorLog(format...)
#define AssertLog(cond, format...)
#define AssertLogReturn(cond, rtn, format...) { if(cond) return rtn; }
#define AssertLogReturnVoid(cond, format...) { if(cond) return; }

#endif

void smallcpy(void *dest, const void *src, unsigned int len);

extern const char *FindStr(const char *str, const char *key);
int SystemCmd(const char *str);

void DebugInit(void);

#endif /*_DEBUG_H*/

