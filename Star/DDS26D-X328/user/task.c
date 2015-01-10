/******************************************************************************
Copyright (c) 2013-2043,深圳市思达仪表有限公司 
All rights reserved. 

文件名称：Task.c 
摘    要：任务调度
作    者：张玉清
当前版本：V1.2.0-20140608

历史记录：
V1.2.0-20140608-张玉清
1 添加两级任务，替代定时器任务
2 按照编程规范整理重构代码
3 TaskAdd中的参数直接使用ms

V1.1.0-20130927-张玉清
1 解决长周期任务被超时任务打断后定时不准问题
2 添加定时器任务

V1.0.0-20090101-张玉清
1 原版
******************************************************************************/
#include "Include.h"

/*---------------------------------------------------------------------------*/
/*内部常数和宏定义                                                           */
/*---------------------------------------------------------------------------*/
#define DEBUG_TASK

/*---------------------------------------------------------------------------*/
/*内部结构体、枚举和类声明                                                   */
/*---------------------------------------------------------------------------*/
typedef struct				//任务队列
{
	uint32 delay;			//添加任务后延时执行的时间
	uint32 period;			//添加任务后周期执行的时间
	void (*Task)(void);		//任务函数指针
	uint8 runMe;			//当前时间片需要执行的标志
}TASK_BUFF;

/*---------------------------------------------------------------------------*/
/*变量定义                                                                   */
/*---------------------------------------------------------------------------*/
static TASK_BUFF taskBuffLow[MAX_TASKS_LOW];	//低优先级任务队列
static TASK_BUFF taskBuffHigh[MAX_TASKS_HIGH];	//高优先级任务队列
static uint32 tickCounterInt;		//系统节拍//中断中修改
static uint32 tickCounter;			//系统节拍
uint8 runMe;

/*---------------------------------------------------------------------------*/
/*内部函数声明                                                               */
/*---------------------------------------------------------------------------*/
static void TaskHighFlashDispatch(void);
static void TaskDelete(uint8 index, uint8 priority);
//void TaskTick(void);
void SysTick_Handler(void);

/*---------------------------------------------------------------------------*/
/*函数定义                                                                   */
/*---------------------------------------------------------------------------*/
/*********************************************************** 
函数功能：	系统节拍定时器中断回调函数
入口参数：
出口参数：
备注说明：
***********************************************************/
//void TaskTick(void)
void SysTick_Handler(void)
{
	tickCounterInt++;
	runMe=1;
}

/*********************************************************** 
函数功能：	任务初始化
入口参数：	uint32 tickLow	//低优先级任务节拍周期ms	//0-代表关闭任务节拍
			uint32 tickHigh	//高优先级任务节拍周期ms	//0-代表关闭任务节拍
出口参数：
备注说明：
***********************************************************/
void TaskInit(uint32 tickLow, uint32 tickHigh)
{
	BT_RTInitRegT reg;

	if(tickLow == 0)
	{
		NVIC_DisableIRQ(SysTick_IRQn);								//在进入主循环前在起动任务TaskStart
	}
	else
	{
		//配置低优先级任务定时器
		SysTick_Config((SystemCoreClock/(uint32)1000) * tickLow);	//配置任务定时器，重装载值(时间)为SYS_TICK_LOW ms
		NVIC_DisableIRQ(SysTick_IRQn);								//在进入主循环前在起动任务TaskStart
	}

	if(tickHigh == 0)
	{
		BT_RTDisableCount(BT_CH_6);                                 //禁止RT计数功能
		BT_RTDisableUnderflowInt(BT_CH_6);     						//禁止RT溢出中断
	}
	else
	{
		//配置高优先级任务定时器
		reg.Cycle=((((SystemCoreClock/16)/(uint32)1000) * tickHigh) -1);//配置任务定时器，重装载值(时间)为SYS_TICK_HIGH ms
		reg.Clock=BT_CLK_DIV_16;									//定时器时钟分频
		reg.InputEdge=BT_TRG_DISABLE;								//定时器触发脉冲（开启、关闭）
		reg.TimerMode=BT_16BIT_TIMER;								//选择16位定时器
		reg.Polarity=BT_POLARITY_NORMAL;							//定时器极性
		reg.Mode=BT_MODE_CONTINUOUS; 								//定时器工作模式

	    BT_SetIOMode(BT_CH_6,BT_IO_MODE_0);                         //IO口0模式
	    BT_RTInit(BT_CH_6,&reg);		                            //初始化定时器
	    BT_RTEnableUnderflowInt(BT_CH_6,TaskHighFlashDispatch);     //使能RT溢出中断
	    NVIC_EnableIRQ(BTIM0_7_IRQn);                               //开启定时器中断
	    BT_RTDisableCount(BT_CH_6);                                 //禁止RT计数功能
	}
	
	//初始化任务队列
	memset((uint8 *)taskBuffLow, 0, sizeof(taskBuffLow));
	memset((uint8 *)taskBuffHigh, 0, sizeof(taskBuffHigh));
	tickCounterInt=0;
	tickCounter=0;
}

/*********************************************************** 
函数功能：	任务启动
入口参数：
出口参数：
备注说明：	上电初始时化完成后调用，避免没有初始化没完成就开始运行任务
***********************************************************/
void TaskStart(void)
{
	//起动低优先级任务
	NVIC_EnableIRQ(SysTick_IRQn);

	//起动高优先级任务
	BT_RTEnableCount(BT_CH_6);					//使能RT计数功能
	BT_RTStartSoftTrig(BT_CH_6);				//启动RT的软件触发
}

/*********************************************************** 
函数功能：	任务添加函数
入口参数：
			void (*Task)(void);		//任务函数指针
			uint32 delay;			//添加任务后延时执行的时间 ms//必须为对应的SYS_TICK的整数倍
			uint32 period;			//添加任务后周期执行的时间 ms//必须为对应的SYS_TICK的整数倍
			uint8 priority; 		//优先级，0级在中断中运行，1级在主程序中运行
出口参数：
备注说明：
			1 固定周期任务，只能在系统初始化时调用一次，且调用前要确保已经TaskInit
			2 单次任务，添加时务必保证该任务不会在执行前反复添加
			3 0级任务一定要每个检查，所有0级任务执行时间之和不能超过时间片的1/100
***********************************************************/
void TaskAdd(void (*Task)(void), const uint32 delay, const uint32 period, const uint8 priority)
{
	uint8 i;

	//合法性检查
	ASSERT(priority <= 1);
	ASSERT(Task!=0);
	ASSERT(!((priority==0) && (((delay%SYS_TICK_HIGH)!=0)||((period%SYS_TICK_HIGH)!=0))));
	ASSERT(!((priority==1) && (((delay%SYS_TICK_LOW)!=0)||((period%SYS_TICK_LOW)!=0))));

	//插入任务队列
	if(priority==0)
	{
		for(i=0; i<MAX_TASKS_HIGH; i++)
		{
			if(taskBuffHigh[i].Task==0)
			{
				taskBuffHigh[i].Task=Task;
				taskBuffHigh[i].delay=delay/SYS_TICK_HIGH;
				taskBuffHigh[i].period=period/SYS_TICK_HIGH;
				break;
			}
			ASSERT(i < MAX_TASKS_HIGH-1);		//队列满
		}
	}
	else
	{
		for(i=0; i<MAX_TASKS_LOW; i++)
		{
			if(taskBuffLow[i].Task==0)
			{
				taskBuffLow[i].Task=Task;
                if(period>delay)
                {
					taskBuffLow[i].delay=tickCounter - (period-delay)/SYS_TICK_LOW;
                }
				else
				{
					taskBuffLow[i].delay=tickCounter + (delay-period)/SYS_TICK_LOW;
				}
				taskBuffLow[i].period=period/SYS_TICK_LOW;
				break;
			}
			ASSERT(i < MAX_TASKS_LOW-1);		//队列满
		}
	}
}

/*********************************************************** 
函数功能：	低优先级任务刷新函数
入口参数：
出口参数：
备注说明：	1 
***********************************************************/
void TaskFlash(void)
{
	uint8 i;

	if(tickCounter!=tickCounterInt)
	{
		//中断数据保护
		NVIC_DisableIRQ(SysTick_IRQn);
		tickCounter=tickCounterInt;
		NVIC_EnableIRQ(SysTick_IRQn);
		
		for(i=0; i<MAX_TASKS_LOW; i++)
		{
			if(taskBuffLow[i].Task)							//有任务
			{
				if((int32)(tickCounter-taskBuffLow[i].delay) >= (int32)taskBuffLow[i].period)		//延时结束
				{
					taskBuffLow[i].runMe=1;					//"runme"标志加1
					if(taskBuffLow[i].period)				//周期任务则再次定时运行
					{
						taskBuffLow[i].delay=tickCounter;
					}
				}
			}
		}
	}
}

/*********************************************************** 
函数功能：	低优先级任务调度函数
入口参数：
出口参数：
备注说明：
***********************************************************/
void TaskDispatch(void)
{
	uint8 i;

	for(i=0; i<MAX_TASKS_LOW; i++)
	{
		if(taskBuffLow[i].runMe)					//有执行请求
		{
			ASSERT(taskBuffLow[i].Task);			//是否为空任务
			if(taskBuffLow[i].Task)
			{
				(*taskBuffLow[i].Task)();			//执行任务
				taskBuffLow[i].runMe=0;				//复位/减小runme标志
				if(taskBuffLow[i].period == 0)		//单次任务则删除该任务
				{
					TaskDelete(i, 1);  				//周期为0 则运行完删除
				}
				//PowerManage();					//及时检测掉电
			}
		}
	}
}

/*********************************************************** 
函数功能：	高级任务任务刷新调度
入口参数：
出口参数：
备注说明：	高级任务任务刷新、调度同时在定时器中完成
***********************************************************/
static void TaskHighFlashDispatch(void)
{
	uint8 i;

	for(i=0; i<MAX_TASKS_HIGH; i++)
	{
		if(taskBuffHigh[i].Task)							//有任务
		{
			if(taskBuffHigh[i].delay == 0)					//延时结束
			{
				(*taskBuffHigh[i].Task)();					//执行任务
				if(taskBuffHigh[i].period)					//周期任务则再次定时运行
				{
					taskBuffHigh[i].delay=taskBuffHigh[i].period-1;
				}
				else
				{
					TaskDelete(i, 0);			  			//周期为0 则运行完删除
				}
			}
			else
			{
				taskBuffHigh[i].delay--;
			}
		}
	}
}

/*********************************************************** 
函数功能：	任务删除函数
入口参数：
			uint8 index			//任务索引号
			uint8 priority; 	//优先级，0级在中断中运行，1级在主程序中运行
出口参数：
备注说明：
***********************************************************/
static void TaskDelete(uint8 index, uint8 priority)
{
	if(priority==0)
	{
		ASSERT(index<MAX_TASKS_HIGH);	//判断范围
		memset((uint8 *)&taskBuffHigh[index], 0, sizeof(taskBuffHigh[index]));
	}
	else
	{
		ASSERT(index<MAX_TASKS_LOW);	//判断范围
		memset((uint8 *)&taskBuffLow[index], 0, sizeof(taskBuffLow[index]));
	}
}

/*********************************************************** 
函数功能：	任务复位函数
入口参数：
			void (*Task)(void);		//任务函数指针
			uint32 delay;			//添加任务后延时执行的时间 ms//必须为对应的SYS_TICK的整数倍
			uint32 period;			//添加任务后周期执行的时间 ms//必须为对应的SYS_TICK的整数倍
			uint8 priority; 		//优先级，0级在中断中运行，1级在主程序中运行
出口参数：
备注说明：
			1 和任务添加类似，如果改任务已经添加则将延时复位，如果没有则添加
***********************************************************/
void TaskReset(void (*Task)(void), const uint32 delay, const uint32 period, const uint8 priority)
{
	uint8 i;

	//合法性检查
	ASSERT(priority <= 1);
	ASSERT(Task!=0);
	ASSERT(!((priority==0) && (((delay%SYS_TICK_HIGH)!=0)||((period%SYS_TICK_HIGH)!=0))));
	ASSERT(!((priority==1) && (((delay%SYS_TICK_LOW)!=0)||((period%SYS_TICK_LOW)!=0))));

	//如果改任务已经添加则将延时复位
	if(priority==0)
	{
		for(i=0; i<MAX_TASKS_HIGH; i++)
		{
			if(taskBuffHigh[i].Task==Task)
			{
				taskBuffHigh[i].delay=delay/SYS_TICK_HIGH;
				taskBuffHigh[i].period=period/SYS_TICK_HIGH;
				return;
			}
		}
	}
	else
	{
		for(i=0; i<MAX_TASKS_LOW; i++)
		{
			if(taskBuffLow[i].Task==Task)
			{
                if(period>delay)
                {
					taskBuffLow[i].delay=tickCounter - (period-delay)/SYS_TICK_LOW;
                }
				else
				{
					taskBuffLow[i].delay=tickCounter + (delay-period)/SYS_TICK_LOW;
				}
				taskBuffLow[i].period=period/SYS_TICK_LOW;
				return;
			}
		}
	}
	
	//如果没有添加则添加
	TaskAdd(Task, delay, period, priority);
}

