#include "stm32f10x_conf.h"
#include "stm32f10x.h"
#include "stm32f10x_iwdg.h"

void SysReset(void)
{
   __set_FAULTMASK(1);      // �ر������ж�
	NVIC_SystemReset();// ��λ
	while(1);
}

void watchdog_init( void )
{
	/* WWDG configuration */
	/* Enable WWDG clock */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
 	IWDG_SetPrescaler(IWDG_Prescaler_256);

   	/* ι��ʱ�� 5s/6.4MS=781 .ע�ⲻ�ܴ���0xfff*/
	IWDG_SetReload(3124);
	//IWDG_SetReload(781);
	IWDG_ReloadCounter();

	/* ʹ�ܹ���*/
	IWDG_Enable();
}
void watchdog_feed(void)
{
	IWDG_ReloadCounter();
}
