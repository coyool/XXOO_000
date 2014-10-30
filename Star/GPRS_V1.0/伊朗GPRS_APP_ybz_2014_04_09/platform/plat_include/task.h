/**
* event.h -- 任务接口头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-3-30
* 最后修改时间: 2009-3-31
*/

#ifndef _SYS_TASK_H
#define _SYS_TASK_H

//#include <unistd.h>

/**
* @brief 创建一个任务(线程)
* @param routine 任务起始执行函数
* @param arg 执行函数参数
* @param name 任务名称
* @param timeout 任务超时时间(秒), 为0表示永不超时, <0采用缺省的3600秒,
*                任务超过timeout不调用ClearCurrentTaskState, 系统会复位
* @return 成功返回0, 失败返回非零值
*/
int SysCreateTask(void *(*routine)(void *), void *arg, const char *name, int timeout);

/**
* @brief 清除当前任务超时状态
*/
void SysClearCurrentTaskState(void);

/**
* @brief 检查当前任务状态,如果有任务超时会复位
*/
void SysCheckTaskState(void);

/**
* @brief 获取当前任务名称
*/
const char *SysCurrentTaskName(void);

/**
* @brief 等待所有任务(线程)结束,主进程调用
*/
void SysWaitTaskEnd(void);

#endif /*_SYS_TASK_H*/
