/**
* file.c -- 文件操作接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2008-5-16
* 最后修改时间: 2009-3-30
*/


//#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <string.h>
#include "include/sys/file.h"
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <linux/reboot.h>
//#include <sys/reboot.h>
//#include <dirent.h>

/**
* @brief 获得文件大小(以字节为单位)
* @param pf 文件指针
*/
int SysGetFileSize(FILE *pf)
{
   /*interface*/ 
	return 0;
}

///缓存区长度
#define MAXLEN_PATH		64
#define MAXLEN_HEAD		16
#define MAXLEN_TAIL		16

/**
* @brief 解析文件名过滤器
* @param src 过滤器
* @param path 输出过滤器路径名
* @param filterhead 输出过滤器通配符前文件名
* @param filtertail 输出过滤器通配符后文件名
* @return 0表示成功, 否则失败
*/
static int parseFilter(const char *src, char *path, char *filterhead, char *filtertail)
{
	int i = strlen(src);
	int pos;

	if(i >= MAXLEN_PATH) return 1;

	for(pos=i-1; pos >=0; pos--) {
		if('/' == src[pos]) break;
	}

	if(pos <= 0) {
		strcpy(path, "./");
	}
	else {
		for(i=0; i<=pos; i++) *path++ = *src++;
		*path = 0;
	}

	i = 0;
	pos = 0;
	for(; 0!=*src; src++) {
		if('*' == *src) {
			pos = 0;
			i = 1;
		}
		else if(0 == i) {
			*filterhead++ = *src;
			pos++;
			if(pos >= MAXLEN_HEAD) return 1;
		}
		else {
			*filtertail++ = *src;
			pos++;
			if(pos >= MAXLEN_TAIL) return 1;
		}
	}
	*filterhead = 0;
	*filtertail = 0;

	return 0;
}

/**
* @brief 比较文件名与过滤器
* @param file 文件名
* @param filterhead 输出过滤器通配符前文件名
* @param filtertail 输出过滤器通配符后文件名
* @return 1表示匹配,0表示不匹配
*/
static int compareFilter(const char *file, const char *filterhead, const char *filtertail)
{
	int i, j;

	if(0 != *filterhead) {
		for(i=0; 0!=*filterhead; filterhead++,i++) {
			if(file[i] != *filterhead) break;
			else if(0 == file[i]) break;
		}
		if(0 != *filterhead) return 0;
	}

	if(0 != *filtertail) {
		i = strlen(filtertail);
		filtertail += (i-1);
		j = strlen(file);
		if(i > j) return 0;
		file += (j-1);

		for(j=0; j<i; i--,filtertail--,file--) {
			if(*filtertail != *file) break;
		}
		if(j >= i) return 1;
		else return 0;
	}

	return 1;
}

/**
* @brief 删除一个文件
* @param file 文件名
*/
void SysRemoveOneFile(const char *file)
{
   /*interface*/ 
	return;
}

/**
* @brief 删除多个文件
* @param files 文件名过滤器, 如"*.dat", "k*", "y*.c"等等
* @return 0表示成功,否则表示失败
* @note 最好不要用来操作过长文件名或者过深路径名(总和不能超过64字节),
      否则操作效率不大大降低
*/
int SysRemoveFiles(const char *files)
{
   /*interface*/ 
	return 0;
}

int SysListFiles(const char *files, const char *outfile)
{
   /*interface*/ 
	return 0;
}

