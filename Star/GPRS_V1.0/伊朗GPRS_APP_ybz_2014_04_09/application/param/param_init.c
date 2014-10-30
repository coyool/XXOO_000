/**
* operation.c -- ����ģ���ʼ��
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-8
* ����޸�ʱ��: 2009-5-8
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
//�ָ�����Ĭ��
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
//ϵͳ�ϵ�Ĭ�ϲ���
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
* @brief ����ģ���ʼ��
* @return 0�ɹ�, ����ʧ��
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

