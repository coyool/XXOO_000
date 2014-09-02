/**
* task.c -- 任务系统接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2008-5-16
* 最后修改时间: 2009-3-30
*/
#include<string.h>
#include "ucos_ii.h"
#include "plat_include/debug.h"
#include "plat_include/reset.h"
#define MAXLEN_TASKNAME		32


struct task_info {
	int timecnt;  //计数
	int timeout;  //超时
	const char* TaskName;
};
static struct task_info TaskInfos[32]={{0}};

inline void SysClearCurrentTaskState(void)
{
	TaskInfos[OSTCBCur->OSTCBPrio].timecnt = 0;
}
void SysCheckTaskState(void)
{
	int i;

	for(i = 0; i<MAXLEN_TASKNAME; i++) {
		if(TaskInfos[i].timeout > 0) {
			TaskInfos[i].timecnt++;
			if(TaskInfos[i].timecnt > TaskInfos[i].timeout) {
				ErrorLog("\n!!task %d(%s) timeout, system restart...\n", i, TaskInfos[i].TaskName);
				SysRestart();
			}
		}
	}
}
void SysInitTaskState(int pro,int timeout)
{
   TaskInfos[pro].timeout = timeout;
}
const char *SysCurrentTaskName(void)
{
	return TaskInfos[OSTCBCur->OSTCBPrio].TaskName;
}

const char *SysGetTaskName(int pid)
{
	return TaskInfos[pid].TaskName;;
}
void SysSetTaskName(int pid,const char* name)
{
	TaskInfos[pid].TaskName = name;
}
