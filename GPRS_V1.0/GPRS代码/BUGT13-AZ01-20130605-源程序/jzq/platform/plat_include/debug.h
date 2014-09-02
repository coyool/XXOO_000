/**
* debug.h -- ������Ϣ����ͷ�ļ�
* ʵ����...
* ����: zhuzhiqiang
* ����ʱ��: 2009-4-3
* ����޸�ʱ��: 2009-4-27
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

//����(���汾Ϊ0���ܴ򿪵���)
#if VERSION_MAJOR == 0
#define OPEN_DEBUGPRINT
#else
#undef OPEN_DEBUGPRINT
#endif

/************/
#if 0
#define LOGTYPE_CLOSE			0  //�ر�
#define LOGTYPE_ALARM			1  //�澯
#define LOGTYPE_SHORT			2  //�����Ϣ
#define LOGTYPE_CENMET			3  //����ͨ����Ϣ
#define LOGTYPE_UPLINK			4  //����ͨ����Ϣ
#define LOGTYPE_DOWNLINK		5  //�ز����ݴ�ӡ
#define LOGTYPE_DEBUG			6  //���Դ�ӡ��Ϣ
#define LOGTYPE_TEST			10 //���Դ�ӡ
#define LOGTYPE_ONLYSAVE		60
#else
#define LOG_OPEN   PRINT_NORMAL
#define LOG_CLOSE  PRINT_NORMAL+3

#define LOGTYPE_CLOSE			0  //�ر�
#define LOGTYPE_ALARM			1  //�澯
#define LOGTYPE_SHORT			2  //�����Ϣ
#define LOGTYPE_CENMET			3  //����ͨ����Ϣ
#define LOGTYPE_UPLINK			4  //����ͨ����Ϣ
#define LOGTYPE_DOWNLINK		5  //�ز����ݴ�ӡ
#define LOGTYPE_DEBUG			6  //���Դ�ӡ��Ϣ
#endif

/*
*@brief ���ô�ӡ���״̬
*@param State���״̬ 1��������Դ���,0�����
*/
void DebugLog(unsigned char State);

//��ӡ���е�����Ϣ
//#define PrintLog  print

void PrintLog(int type, const char *format, ...);

void PrintHexLog(int type, const unsigned char *buf, int len);


//���Դ�ӡ,��ʽ���а治����
#ifdef OPEN_DEBUGPRINT
#define DebugPrint		PrintLog
#else
#define DebugPrint(a, ...)
#endif

/****************/
//��ʽ���а汾������
#ifdef OPEN_ASSERTLOG

//#define PATH_ASSERTLOG		"/tmp/"

//��������������ʱʹ��,����Ϣд����־��
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

