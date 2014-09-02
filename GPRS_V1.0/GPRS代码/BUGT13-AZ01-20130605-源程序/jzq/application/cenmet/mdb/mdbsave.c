/**
* mdbsave.c -- ״̬���ݱ���
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-14
* ����޸�ʱ��: 2009-5-14
*/

//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define DEFINE_MDBTGRP

#include "plat_include/debug.h"
#include "plat_include/bin.h"
#include "plat_include/syslock.h"
#include "plat_include/timeal.h"
#include "app_include/param/capconf.h"
#include "app_include/cenmet/mdbconf.h"
#include "app_include/cenmet/dbconfig.h"
#include "app_include/cenmet/mdbstic.h"
#include "plat_include/flashsave.h"

//static int LockIdMdbSave = -1;

/**
* @brief ���MDB����
*/
void ClearMdbSave(void)
{
	MdbSticInit();
}

/**
* @brief MDB�����ʼ��
* @return �ɹ�0, ����ʧ��
*/
int MdbSaveInit(void)
{
	int rt;
	
	MdbSticInit();

	DebugPrint(0, "  load stic.dat...\n");
	rt = para_readflash_bin(TERM_FILEINDEX_MDBSTIC,(unsigned char *)&MdbStic, sizeof(MdbStic));
	AssertLog(rt==-1,"read Mdbstic failed!\n");
	PrintLog(LOGTYPE_DEBUG,"readflash mdbstic rt(%d)\n",rt);
	UpdateSticResetCount();
	
	return rt;
}

/**
* @brief ����ͳ�����ݿ�
*/
void SaveMdbStic(void)
{
	int rt;
	
	//LockSysLock(LockIdMdbSave);
	rt = para_writeflash_bin(TERM_FILEINDEX_MDBSTIC,(unsigned char *)&MdbStic, sizeof(MdbStic));
	AssertLog(rt==-1,"write Mdbstic failed!\n");
	PrintLog(LOGTYPE_DEBUG,"writeflash mdbstic rt(%d)\n",rt);
	//UnlockSysLock(LockIdMdbSave);
}

/**
* @brief ��������
*/
void SaveMdb(void)
{
	//SaveMdbUene();
	SaveMdbStic();
	//SaveMdbTGrp();
}
//DECLARE_CYCLE_SAVE(SaveMdb, 0);

void ResetMdbDay(void)
{

	MdbSticEmptyDay();
}

void ResetMdbMonth(void)
{

	MdbSticEmptyMonth();
}

void BakupMdb(void)
{
	BakupMdbStic();
}

