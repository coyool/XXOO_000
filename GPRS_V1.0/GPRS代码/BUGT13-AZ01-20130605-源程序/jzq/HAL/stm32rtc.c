#include <stdio.h>

#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_bkp.h"
#include "hal_include/stm32rtc.h"
#include "plat_include/debug.h"
#include "stm32f10x_iwdg.h"

void Stm32RtcInit(u32 default_time)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	TIM_InternalClockConfig(TIM2);
	//预分频系数为36000-1，这样计数器时钟为72MHz/36000 = 2kHz
	//TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 10000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//设置计数溢出大小，每计2000个数就产生一个更新事件
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	//清除溢出中断标志
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//禁止ARR预装载缓冲器
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	//开启TIM2的中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
	//now = default_time;
}
extern inline void Tick(void);
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		//now++;
		Tick();	
	}
}
