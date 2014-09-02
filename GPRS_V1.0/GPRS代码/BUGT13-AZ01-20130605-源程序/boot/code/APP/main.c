#include <stdint.h>
#include "stm32f10x_conf.h"
#include "system_stm32f10x.h"
#include "misc.h"
#include "W25X64.h"
#include "switch.h"
#include "iap.h"
#include "uart.h"
#include "ymodem.h"
#include "sys_config.h"

void NVIC_Configuration(void);
typedef  void (*pFunction)(void);
void delay_init(u8 SYSCLK);

void Signel_led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
}
void Led_OnOff(unsigned char on)
{
  	if(on)
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);	
}
void TIM_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_DeInit(TIM2);
	TIM_InternalClockConfig(TIM2);
	//预分频系数为36000-1，这样计数器时钟为72MHz/36000 = 2kHz
	TIM_TimeBaseStructure.TIM_Prescaler = 10000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//设置计数溢出大小，每计100个数就产生一个更新事件
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	//清除溢出中断标志
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	//禁止ARR预装载缓冲器
	TIM_ARRPreloadConfig(TIM2, DISABLE);
	//开启TIM2的中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
int main()
{
	pFunction Jump_To_Application;
	uint32_t JumpAddress;

	SystemInit();		/* 配置系统时钟为72M */	
	NVIC_Configuration();

	watchdog_init();
	delay_init(72);	//72M
	w25x64_init();

	InitSerial();
	Signel_led_init();
	TIM_init();
	SerialPutString("BootLoader Runing...\n");
	SerialPutString("BootLoader for ");
	SerialPutString(SYSTEM_NAME);
	SerialPutString("\n");
	ymodem();

	iap();
	SerialPutString("Load App!\n");

	TIM_Cmd(TIM2,DISABLE);
	if (((*(__IO uint32_t*)APP_LOCATION) & 0x2FFE0000 ) == 0x20000000)
	{
		JumpAddress = *(__IO uint32_t*) (APP_LOCATION + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		__set_MSP(*(__IO uint32_t*) APP_LOCATION);
		Jump_To_Application();
	}
	return 0;
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif
        //选择中断分组1

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
        //选择TIM2的中断通道

        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      

        //抢占式中断优先级设置为0

        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;

       //响应式中断优先级设置为0

        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;

        //使能中断

        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

        NVIC_Init(&NVIC_InitStructure);
}
void TIM2_IRQHandler(void)
{

	static unsigned char on = 1;
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
		on = !on;
		Led_OnOff(on);	
	}
}
