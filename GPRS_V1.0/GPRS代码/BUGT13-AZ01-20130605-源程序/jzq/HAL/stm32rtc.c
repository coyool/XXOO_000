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
	//Ԥ��Ƶϵ��Ϊ36000-1������������ʱ��Ϊ72MHz/36000 = 2kHz
	//TIM_TimeBaseStructure.TIM_Prescaler = 36000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 10000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//���ü��������С��ÿ��2000�����Ͳ���һ�������¼�
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	//�������жϱ�־
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//��ֹARRԤװ�ػ�����
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	//����TIM2���ж�
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
