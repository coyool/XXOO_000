
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_bt.h 
* 文件标识：见配置管理计划书 
* 摘要：基本定时器移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131024
*******************************************************************/
#include "Include.h"

/*******************************************************************************
 * @function_name: Config_SystemClock_Priority
 * @function_file: BT.c
 * @描述：配置各中断优先级
 * @入口参数：
 * @出口参数:  无
 * @Attention:   
 *---------------------------------------------------------
 * @修改人：
 ******************************************************************************/
void Config_SystemClock_Priority(void)
{
	//开启系统定时器,重装载值(时间)为1ms
	//SysTick_Config(SystemCoreClock/1000);   
	//设置优先级组：抢占式优级（0～3），子优先级（0～3）
	NVIC_SetPriorityGrouping(5);
	//设置FRTIM_IRQn抢占优先级为1 
	NVIC_SetPriority(FRTIM_IRQn,NVIC_EncodePriority(5,0,0));
	//设置系统定时器中断抢占优先级为2
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(5,1,0));
	//设置基本定时器7中断抢占优先级为3
	NVIC_SetPriority(BTIM0_7_IRQn,NVIC_EncodePriority(5,0,0));
}

/*--------------------全局常量定义---------------------*/

/*--------------------全局变量定义---------------------*/


/*--------------------内部函数申明-------------------------*/

/*********************************************************** 
函数功能：RF 1ms定时中断初始化
入口参数：
出口参数：
备注说明：BT_CH_4 1ms
***********************************************************/
/*void RF1MSInit(void)
{
	BT_RTInitRegT reg;

	//初始化1ms定时器
	reg.Cycle=1000-1;							//执行多少个定时器周期数=1000*(16000000/16)=0.001s
	reg.Clock=BT_CLK_DIV_16;					//定时器时钟分频
	reg.InputEdge=BT_TRG_DISABLE;				//定时器触发脉冲（开启、关闭）
	reg.TimerMode=BT_16BIT_TIMER;				//选择16位定时器
	reg.Polarity=BT_POLARITY_NORMAL;			//定时器极性
	reg.Mode=BT_MODE_CONTINUOUS; 				//定时器工作模式

    BT_SetIOMode(BT_CH_4,BT_IO_MODE_0);                         //IO口0模式
    BT_RTInit(BT_CH_4,&reg);		                            //初始化定时器
    BT_RTEnableUnderflowInt(BT_CH_4,RF_1msServerFucCall);     //使能RT溢出中断
    NVIC_EnableIRQ(BTIM0_7_IRQn);                               //开启定时器中断
    BT_RTDisableCount(BT_CH_4);                                  //使能RT计数功能
}
*/
/*********************************************************** 
函数功能：RF 1ms定时开始启动
入口参数：
出口参数：
备注说明：BT_CH_4 1ms
***********************************************************/
void RF1MSStart(void)
{
	BT_RTEnableCount(BT_CH_4);									//使能RT计数功能
	BT_RTStartSoftTrig(BT_CH_4);								//启动RT的软件触发
}

/*********************************************************** 
函数功能：RF 1ms定时停电初始
入口参数：
出口参数：
备注说明：关闭BT_CH_4 1ms
***********************************************************/
void RF1MSPowerDownInit(void)
{
    BT_RTDisableCount(BT_CH_4);                                  //使能RT计数功能
    BT_RTDisableUnderflowInt(BT_CH_4);     						 //使能RT溢出中断
}



