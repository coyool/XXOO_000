/**
* operation.c -- 参数模块初始化
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-8
* 最后修改时间: 2009-5-8
*/

//#include <stdio.h>

#define define_para_comb

#include "plat_include/debug.h"
#include "app_include/param/operation_inner.h"
#include "app_include/param/param_init.h"
#include "plat_include/flashsave.h"

para_Comb_t para_comb;

extern void MappingMetp(void);
extern void LoadDefParaTerm(void);
extern void LoadDefParaUni(void);
extern void LoadDefParaMix(char flg);
extern void LoadDefParaTerm2(void);
extern void LoadDefParaUni2(void);
//恢复出厂默认
void LoadParaComb2(void)
{
	int rt;

	LoadDefParaTerm2();
	LoadDefParaUni2();
	//LoadDefParaMix(1);
//	LoadDefParaMix(2);
	rt = para_writeflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	AssertLog(rt==-1,"writeflash para_comb failed!\n");
}
//系统上电默认参数
void w25x64_read(unsigned int addr,void *pbuf,unsigned int bytes);
static void LoadParaComb(void)
{
	int rt;
	
	LoadDefParaTerm();
	LoadDefParaUni();
//	LoadDefParaMix(1);
	//LoadDefParaMix(2);			 
	rt = para_readflash_bin(PARA_FILEINDEX_COMB,(unsigned char *)&para_comb,sizeof(para_comb));
	AssertLog(rt==-1,"readflash para_comb failed!\n");
}
/**
* @brief 参数模块初始化
* @return 0成功, 否则失败
*/
int ParamInit(void)
{
	PrintLog(LOGTYPE_DEBUG,"load param ...\n\r");

	//SetParaNetAddr();
	LoadParaComb();
	LoadParaMeter();

	//LoadParaCenMetp();
	//LoadParaMix();
//	LoadParaDataUse();
	
//	MappingMetp();

	return 0;
}

