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
* @brief 获取运行状态修改指针
*/
runstate_t *RunStateModify(void)
{
	return &RunState;
}
#endif

/**
* @brief 清除运行状态
*/
void ClearRunState(void)
{
	memset(&RunState, 0, sizeof(RunState));
}

/**
* @brief 软件是否是通过自复位重启的
* @return 0-否, 1-是
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
* @brief 保存运行状态
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
	PrintLog(LOGTYPE_DEBUG, "时钟电池: %s\n", RunState.battery?"低":"正常");
	PrintLog(LOGTYPE_DEBUG, "市电状态: %s\n", RunState.pwroff?"无":"有");
	PrintLog(LOGTYPE_DEBUG, "遥信状态: %02X\n", RunState.isig_stat);
	PrintLog(LOGTYPE_DEBUG, "遥信变位: %02X\n", RunState.isig_chg);

	PrintLog(LOGTYPE_DEBUG, "电池充电: %s\n", RunState.batcharge?"充电中":"停止");
	PrintLog(LOGTYPE_DEBUG, "电池连接: %s\n", RunState.batbad?"未连接":"正常");
	PrintLog(LOGTYPE_DEBUG, "终端剔除: %s\n\n", RunState.outgrp?"投入":"解除");

	PrintLog(LOGTYPE_DEBUG, "重要事件: head=%d, cur=%d, snd=%d\n", 
		RunState.alarm.head[0], RunState.alarm.cur[0], RunState.alarm.snd[0]);
	PrintLog(LOGTYPE_DEBUG, "一般事件: head=%d, cur=%d, snd=%d\n", 
		RunState.alarm.head[1], RunState.alarm.cur[1], RunState.alarm.snd[1]);
	PrintLog(LOGTYPE_DEBUG, "事件标志: ");
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
DECLARE_SHELL_CMD("runstate", shell_runstate, "显示运行状态变量");
#endif

