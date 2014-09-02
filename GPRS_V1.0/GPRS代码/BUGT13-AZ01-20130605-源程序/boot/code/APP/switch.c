#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"

void SysReset(void)
{
   __set_FAULTMASK(1);      // 关闭所有中端
	NVIC_SystemReset();// 复位
	while(1);
}

void watchdog_init( void )
{
	/* WWDG configuration */
	/* Enable WWDG clock */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* 时钟分频,40K/256=156HZ(6.4ms)*/
 	IWDG_SetPrescaler(IWDG_Prescaler_256);

   	/* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
	IWDG_SetReload(3124);
	//IWDG_SetReload(781);
	IWDG_ReloadCounter();

	/* 使能狗狗*/
	IWDG_Enable();
}
void watchdog_feed(void)
{
	IWDG_ReloadCounter();
}
