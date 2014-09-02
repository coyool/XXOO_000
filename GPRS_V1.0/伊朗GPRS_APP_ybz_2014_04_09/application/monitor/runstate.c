//#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plat_include/debug.h"
#include "plat_include/syslock.h"
#include "app_include/monitor/runstate.h"
#include "plat_include/flashsave.h"

runstate_t RunState;

static int SelfRebootFlag = 0;
 
#if 0
/**
* @brief ��ȡ����״̬�޸�ָ��
*/
runstate_t *RunStateModify(void)
{
	return &RunState;
}
#endif

/**
* @brief �������״̬
*/
void ClearRunState(void)
{
	memset(&RunState, 0, sizeof(RunState));
}

/**
* @brief ����Ƿ���ͨ���Ը�λ������
* @return 0-��, 1-��
*/
int IsSelfReboot(void)
{
	return SelfRebootFlag;
}

int RunStateInit(void)
{
	int rt;

	memset(&RunState, 0, sizeof(RunState));
	DebugPrint(0, "  load runstate(size=%d)...", sizeof(RunState));
	
	rt = para_readflash_bin(TERM_FILEINDEX_RUNSTATE,(unsigned char *)&RunState,sizeof(RunState));
	AssertLog(rt==-1,"readflash runstate failed!\n");
	PrintLog(LOGTYPE_DEBUG,"readflash runstate rt(%d)\n",rt);
	return 0;
}

/**
* @brief ��������״̬
*/
void SaveRunState(void)
{
	int rt;
	
	rt = para_writeflash_bin(TERM_FILEINDEX_RUNSTATE,(unsigned char *)&RunState,sizeof(RunState));
	AssertLog(rt==-1,"writeflash runstate failed!\n");
	PrintLog(LOGTYPE_DEBUG,"writeflash runstate rt(%d)\n",rt);
}

#if 0
DECLARE_CYCLE_SAVE(SaveRunState, 0);

int shell_runstate(int argc, char *argv[])
{
	PrintLog(LOGTYPE_DEBUG, "ʱ�ӵ��: %s\n", RunState.battery?"��":"����");
	PrintLog(LOGTYPE_DEBUG, "�е�״̬: %s\n", RunState.pwroff?"��":"��");
	PrintLog(LOGTYPE_DEBUG, "ң��״̬: %02X\n", RunState.isig_stat);
	PrintLog(LOGTYPE_DEBUG, "ң�ű�λ: %02X\n", RunState.isig_chg);

	PrintLog(LOGTYPE_DEBUG, "��س��: %s\n", RunState.batcharge?"�����":"ֹͣ");
	PrintLog(LOGTYPE_DEBUG, "�������: %s\n", RunState.batbad?"δ����":"����");
	PrintLog(LOGTYPE_DEBUG, "�ն��޳�: %s\n\n", RunState.outgrp?"Ͷ��":"���");

	PrintLog(LOGTYPE_DEBUG, "��Ҫ�¼�: head=%d, cur=%d, snd=%d\n", 
		RunState.alarm.head[0], RunState.alarm.cur[0], RunState.alarm.snd[0]);
	PrintLog(LOGTYPE_DEBUG, "һ���¼�: head=%d, cur=%d, snd=%d\n", 
		RunState.alarm.head[1], RunState.alarm.cur[1], RunState.alarm.snd[1]);
	PrintLog(LOGTYPE_DEBUG, "�¼���־: ");
	PrintHexLog(LOGTYPE_DEBUG, RunState.alarm.stat, 8);
	PrintLog(LOGTYPE_DEBUG, "\n");

	PrintLog(LOGTYPE_DEBUG, "softchg=");
	PrintHexLog(LOGTYPE_DEBUG, RunState.softchg.ver, 8);

	PrintLog(LOGTYPE_DEBUG, "cnt_snderr= %d, %d\n", RunState.cnt_snderr[0], RunState.cnt_snderr[1]);
	PrintLog(LOGTYPE_DEBUG, "flag_acd= %d\n", RunState.flag_acd);

	PrintLog(LOGTYPE_DEBUG, "timepoweroff= ");
	PrintHexLog(LOGTYPE_DEBUG, RunState.timepoweroff, 8);

	PrintLog(LOGTYPE_DEBUG, "malmflag= ");
	PrintHexLog(LOGTYPE_DEBUG, RunState.malmflag, LEN_MALM_STAT);

	PrintLog(LOGTYPE_DEBUG, "\n");
	return 0;
}
DECLARE_SHELL_CMD("runstate", shell_runstate, "��ʾ����״̬����");
#endif

