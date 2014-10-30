/**
* log.c 运行信息打印
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-4-3
* 最后修改时间: 2009-4-27
*/

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "plat_include/debug.h"
#include "plat_include/plat_uart.h"
#include "plat_include/sys_config.h"

//#include"print.h"



unsigned char logType = LOGTYPE_DEBUG;
const char* LOG_STR[]={"","ALARM:","SHORT:","CENMET:","UPLINK:","DOWNLINK:","DEBUG:"};
unsigned char LogSwitch;

	
/**
* @brief 打印运行调试信息
* @param format 打印格式
*/
static char buffer[256];
void PrintLog(int type, const char *format, ...)
{

	va_list va;

	if((logType < type)||(logType >LOGTYPE_DEBUG)) return;
	
	va_start(va, format);
	vsprintf(buffer, format, va);
	va_end(va);

	//len = strlen(buffer);
	printf(buffer);

}


static const char FormatHexChar[16] = {
	'0', '1', '2', '3', '4', '5', '6', '7', 
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};
/**
* @brief 打印一串十六进制数组
* @param buf 缓存区指针
* @param len 缓存区长度
*/
void PrintHexLog(int type, const unsigned char *buf, int len)
{
	char str;
	int i;
	unsigned char uc;

	if((logType < type)||(logType > LOGTYPE_DEBUG)) 
		return;

	for(i=1; i<=len; i++) 
	{
		uc = *buf++;
		str = FormatHexChar[uc>>4];
		PrintLog(type, "%c", str);
		str = FormatHexChar[uc&0x0f];
		PrintLog(type, "%c", str);
		if(0 == (i&0x1f)) 
		{
			PrintLog(type, "\r\n");
		}
		else if(0 == (i&0x07)) 
		{
			PrintLog(type, "%c", ':');
		}
		
		if(LOGTYPE_DEBUG == logType)
		{ 
			if((i%192)==0) Sleep(1);
		}

	}

 	PrintLog(type, "\r\n");
}
static char Erbuffer[256];
void PrintErrorLog(int bexit, const char *file, int line, const char *format, ...)
{
	static int pastline = 0, pastsum;
	const char *p;
	int sum;
	va_list va;

	
	va_start(va, format);
	vsprintf(Erbuffer, format, va);
	va_end(va);

	if(strlen(file) > 300) return;
	p = file;
	sum = 0;
	while(0 != *p) sum += *p++;
	if(sum == pastsum && pastline == line) return;
	pastsum = sum;
	pastline = line;

	PrintLog(LOGTYPE_ALARM, "\n!!error!! %s line %d:%s", file, line,Erbuffer);

	//if(bexit) SysAppExit();
}


void smallcpy(void *dest, const void *src, unsigned int len)
{
	unsigned int i;
	unsigned char *pdest = (unsigned char *)dest;
	const unsigned char *psrc = (const unsigned char *)src;

	for(i=0; i<len; i++) *pdest++ = *psrc++;
}


const char *FindStr(const char *str, const char *key)
{
	const char *pstr = str;
	int i;

	while(*pstr != 0) {
		i = 0;
		while(key[i] != 0) {
			if(pstr[i] != key[i]) break;
			i++;
		}
		if(key[i] == 0) return pstr;

		pstr++;
	}

	return NULL;
}

