/**
* file.c -- �ļ������ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2008-5-16
* ����޸�ʱ��: 2009-3-30
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
* @brief ����ļ���С(���ֽ�Ϊ��λ)
* @param pf �ļ�ָ��
*/
int SysGetFileSize(FILE *pf)
{
   /*interface*/ 
	return 0;
}

///����������
#define MAXLEN_PATH		64
#define MAXLEN_HEAD		16
#define MAXLEN_TAIL		16

/**
* @brief �����ļ���������
* @param src ������
* @param path ���������·����
* @param filterhead ���������ͨ���ǰ�ļ���
* @param filtertail ���������ͨ������ļ���
* @return 0��ʾ�ɹ�, ����ʧ��
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
* @brief �Ƚ��ļ����������
* @param file �ļ���
* @param filterhead ���������ͨ���ǰ�ļ���
* @param filtertail ���������ͨ������ļ���
* @return 1��ʾƥ��,0��ʾ��ƥ��
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
* @brief ɾ��һ���ļ�
* @param file �ļ���
*/
void SysRemoveOneFile(const char *file)
{
   /*interface*/ 
	return;
}

/**
* @brief ɾ������ļ�
* @param files �ļ���������, ��"*.dat", "k*", "y*.c"�ȵ�
* @return 0��ʾ�ɹ�,�����ʾʧ��
* @note ��ò�Ҫ�������������ļ������߹���·����(�ܺͲ��ܳ���64�ֽ�),
      �������Ч�ʲ���󽵵�
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

