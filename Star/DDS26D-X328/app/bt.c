

#include "pdl_header.h"


static BT_RTInitRegT tRTInitRegT = 
{
    4999,                               //执行多少个定时器周期数
    BT_CLK_DIV_1,                       //定时器时钟分频
    BT_TRG_DISABLE,                     //定时器触发脉冲（开启、关闭）
    BT_32BIT_TIMER,                     //选择32位定时器
    BT_POLARITY_NORMAL,                 //定时器极性
    BT_MODE_CONTINUOUS,                 //定时器工作模式
};


/********************************************************/
//基本定时器溢出中断函数
/********************************************************/
static void RTUnderflowIntHandler(void)
{
    TaskRunFlag = 1;
}


/*******************************************************/
//功能:基本时钟定时器函数
//输入参数:
//输出参数:	
//函数返回值说明:
//使用的资源：
//作者：
//日期：   		
//备注:
/*******************************************************/
void BTInit(void)
{
    NVIC_EnableIRQ(BTIM0_7_IRQn);                               //开启定时器中断
    BT_SetIOMode(BT_CH_0,BT_IO_MODE_0);                         //IO口0模式
    BT_RTInit(BT_CH_0,&tRTInitRegT);                            //初始化定时器
    BT_RTEnableUnderflowInt(BT_CH_0,RTUnderflowIntHandler);     //使能RT溢出中断
    BT_RTEnableCount(BT_CH_0);                                  //使能RT计数功能
    BT_RTStartSoftTrig(BT_CH_0);                                //启动RT的软件触发
}


