/**
* event.c -- 事件接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2008-5-16
* 最后修改时间: 2010-12-27
*/



#include "plat_include/event.h"
#include "plat_include/mutex.h"


/**
* @brief 初始化事件控制变量
* @param pctrl 事件控制变量指针
*/
void SysInitEvent(sys_event_t *pctrl)
{
	pctrl->event = 0;
}

/**
* @brief 等待事件
* @param pctrl 事件控制变量指针
* @param bwait 等待时是否挂起, 
*          如果为0, 则不管有没有收到想接收的事件, 函数都将直接返回
*          如果为1, 则如果没有收到想接收的事件, 任务将挂起, 直到收到
* @param waitmask 需要等待的事件
* @param pevent 接收到的事件
*/
void SysWaitEvent(sys_event_t *pctrl, int bwait, unsigned long waitmask, unsigned long *pevent)
{
	unsigned long ul;

	ul = pctrl->event;
	if(ul&waitmask) {
		ul &= waitmask;
		pctrl->event &= ~waitmask;
	}
	else ul = 0;

	*pevent = ul;
}

/**
* @brief 发送事件
* @param pctrl 事件控制变量指针
* @param event 需要发送的事件
*/
void SysSendEvent(sys_event_t *pctrl, unsigned long event)
{
	pctrl->event |= event;
}

