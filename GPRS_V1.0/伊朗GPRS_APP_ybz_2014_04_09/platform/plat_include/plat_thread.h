#ifndef __PLAT_THREAD_H__
#define __PLAT_THREAD_H__


void SysInitTaskState(int pro,int timeout);
void SysSetTaskName(int pid,const char* name);
/*
说明:创建一个任务(线程)
输入参数:
    routine 任务起始执行函数
    arg 执行函数参数
    name 任务名称
    timeout 任务超时时间(秒),为0表示永不超时,-1 缺省超时时间 1 小时
	stk_size 堆栈大小
	prio 先级
输出参数:
    无
返回值:
    成功 SUCCEED，失败 -ERRFAILED
*/
#define SysCreateTask(routine,arg,name,timeout,stk_size,prio)\
{															  \
	static  OS_STK  stk[stk_size];								\
	OSTaskCreateExt((void (*)(void *))routine,					\
							(void			*) arg,				 \
							(OS_STK 		*)&stk[stk_size - 1], \
							(INT8U			 ) prio,			   \
							(INT16U 		 ) prio,				\
							(OS_STK 		*)&stk[0],				 \
							(INT32U 		 ) stk_size,			  \
							(void			*) 0,					   \
							(INT16U 		 )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));	\
							SysSetTaskName(prio,name);	 \
							SysInitTaskState(prio,timeout<0?3600:timeout);			\
}

/*
说明:清除当前任务超时状态
输入参数:
    无
输出参数:
    无
返回值:
    无
*/
inline void SysClearCurrentTaskState(void);

/*
说明:检查当前任务状态,如果有任务超时会复位
输入参数:
    无
输出参数:
    无
返回值:
    无
*/
void SysCheckTaskState(void);

/*
说明:获取当前任务名称
输入参数:
    无
输出参数:
    无
返回值:
    名称缓存指针
*/
const char *SysCurrentTaskName(void);

/*
说明:等待所有任务(线程)结束,主进程调用
输入参数:
    无
输出参数:
    无
返回值:
    无
*/
void SysWaitTaskEnd(void);

/*
说明:获取指定任务的名称
输入参数:
    任务号
输出参数:
    无
返回值:
    名称缓存指针
*/
const char *SysGetTaskName(int pid);
#endif

