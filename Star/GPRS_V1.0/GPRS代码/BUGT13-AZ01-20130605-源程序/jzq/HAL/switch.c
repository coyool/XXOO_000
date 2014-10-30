#include "stm32f10x.h"
#include "misc.h"
#include "hal_include/switch.h"
#include "plat_include/debug.h"
#include "hal_include/hal_config.h"

//信号强度灯
#define	PORT_SIGNEL_LED   	GPIOB

#define	PIN_SIGNEL_LED1		GPIO_Pin_0
#define	PIN_SIGNEL_LED2		GPIO_Pin_1

//485
#define PORT_485	      GPIOA
#define PIN_NBR_485	GPIO_Pin_4
//PLC 复位
#define PORT_PLC	      GPIOA
#define PIN_NBR_PLC		GPIO_Pin_1

//485通信灯
#define PORT_CB_LED             GPIOC                    
#define PIN_NBR_CB_LED	      GPIO_Pin_8

//在线灯
#define PORT_LED_ONLINE      GPIOE
#define PIN_NBR_LED_ONLINE GPIO_Pin_9

//系统状态灯
#define PORT_LED_SYSTEM      GPIOB
#define PIN_NBR_LED_SYSTEM GPIO_Pin_13


#define PORT_KEY1		      GPIOE
#define PIN_NBR_KEY1 	      GPIO_Pin_7

//Gprs 电源
#ifdef M35_GPRS 
	#ifdef SMALL_CONCENT
	#define PORT_GPRS_VCC         GPIOB
	#define PIN_NBR_GPRS_VCC    GPIO_Pin_1
	#endif
	#ifdef METER_MODULE
	#define PORT_GPRS_VCC         GPIOB
	#define PIN_NBR_GPRS_VCC    GPIO_Pin_12
	#endif
	#ifdef BIG_CONCENT
	#define PORT_GPRS_VCC         GPIOB
	#define PIN_NBR_GPRS_VCC    GPIO_Pin_12
	#endif

#endif
#ifdef MC55I_GPRS
#define PORT_GPRS_VCC         GPIOB
#define PIN_NBR_GPRS_VCC    GPIO_Pin_12
#endif

//GPRS PWRKEY
#define PORT_GPRS_PWRKEY	      GPIOE
#define PIN_NBR_GPRS_PWRKEY	GPIO_Pin_8
//
//#define PORT_KEY0		GPIO_PORT0
//#define PIN_NBR_KEY0 	6

//LED控制-----------------------------------------------------------------------
/**
 * led初始化
*/
#ifdef METER_MODULE
void led_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin = PIN_NBR_CB_LED;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(PORT_CB_LED, &GPIO_InitStructure);

	GPIO_SetBits(PORT_CB_LED, PIN_NBR_CB_LED);	 // turn off all led
}
#endif
#ifdef SMALL_CONCENT
void PowerCheckInit(void)
{
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
#endif
#ifdef BIG_CONCENT
void Enable485_init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
	
  	GPIO_InitStructure.GPIO_Pin = PIN_NBR_485;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(PORT_485, &GPIO_InitStructure);
	GPIO_SetBits(PORT_485,PIN_NBR_485);
}
#endif
#ifdef METER_MODULE
void Signel_led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = PIN_SIGNEL_LED1|PIN_SIGNEL_LED2;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_SIGNEL_LED, &GPIO_InitStructure);
	GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1|PIN_SIGNEL_LED2);
}
extern short GprsDevSigdBm;
unsigned char shining_led_state = 0;
unsigned char LongShiningState = 0;
void shining_led(void)
{
	static char sig = 0;
	static unsigned short t1 = 0;
	static unsigned short t2 = 0;
	if(shining_led_state == 0)
		return;
	if(t2 == 0)
	{
		t2 = 6;
		sig = (GprsDevSigdBm / 4) + 1;
		if(sig>5)sig=5;
	}
	
	if(((t1>15)&&(sig>0))||(LongShiningState > 0))
	{//亮
		Signel_led(Signel_DEFAULT,1);
	}
	else
	{//灭
	    Signel_led(Signel_DEFAULT,0);
	}
	if(t1 == 0)
	{
		t2 --;	
		t1 = 24;	
		if(sig>0)
			sig --;
	}
	else t1--;
	if(LongShiningState>0)
		LongShiningState--;
}
void Signel_red_led(unsigned char on)
{
	if(on == 1)
		GPIO_ResetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1);
	else
		GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1);	
}
void Signel_green_led(unsigned char on)
{
	if(on == 1)
		GPIO_ResetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED2);
	else
		GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED2);	
}
//控制告警灯颜色
//on :1亮灯
void Signel_led(unsigned char state,unsigned char on)
{
	static unsigned char old_state = 0;
	if(state == Signel_DEFAULT)
		state = old_state;
	else
		old_state = state;
	shining_led_state = 1;
	switch(state)
	{
	case 0:
		GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1|PIN_SIGNEL_LED2);
		shining_led_state = 0;
	break;
	case 1:
		if(on == 1)
			GPIO_ResetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1);
		else
			GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1);
		GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED2);
	break;
	case 2://需要闪烁
		GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1);
		if(on == 1)
			GPIO_ResetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED2);
		else
			GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED2);
	break;
	case 3:
 		if(on == 1)
			GPIO_ResetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1|PIN_SIGNEL_LED2);
		else
			GPIO_SetBits(PORT_SIGNEL_LED,PIN_SIGNEL_LED1|PIN_SIGNEL_LED2);
	break;
	}
}

void LedLongShining(void)
{
	LongShiningState = 20;
}
#endif
//GPRS电源控制------------------------------------------------------------------
/**
 * gprs电源控制脚初始化
*/
void gprs_switch_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = PIN_NBR_GPRS_VCC;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_GPRS_VCC, &GPIO_InitStructure);
    gprs_vcc_off();
}


 void gprs_vcc_on(void)
{
 	GPIO_SetBits(PORT_GPRS_VCC,PIN_NBR_GPRS_VCC);
}

void gprs_vcc_off(void)
{
	GPIO_ResetBits(PORT_GPRS_VCC,PIN_NBR_GPRS_VCC);
}



void watchdog_feed(void)
{
	IWDG_ReloadCounter();
/*	
	if(GPIO_ReadOutputDataBit(PORT_WATCHDOG,PIN_NBR_WATCHDOG))
		GPIO_ResetBits(PORT_WATCHDOG,PIN_NBR_WATCHDOG);
	else
		GPIO_SetBits(PORT_WATCHDOG,PIN_NBR_WATCHDOG);
*/
}
void SysReset(void)
{
   __set_FAULTMASK(1);      // 关闭所有中端
	NVIC_SystemReset();// 复位
	while(1);
}
void SysHalt(void)
{
   __set_FAULTMASK(1);      // 关闭所有中端
	while(1);
}
//初始化看门狗
void watchdog_init( void )
{
	/* WWDG configuration */
	/* Enable WWDG clock */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* 时钟分频,40K/256=156HZ(6.4ms)*/
 	IWDG_SetPrescaler(IWDG_Prescaler_256);

   	/* 喂狗时间 5s/6.4MS=781 .注意不能大于0xfff*/
	IWDG_SetReload(781);

	IWDG_ReloadCounter();

	/* 使能狗狗*/
	IWDG_Enable();


}
void PlcRest(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
  	GPIO_InitStructure.GPIO_Pin = PIN_NBR_PLC;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(PORT_PLC, &GPIO_InitStructure);

	GPIO_SetBits(PORT_PLC,PIN_NBR_PLC);
	Sleep(2);
	GPIO_ResetBits(PORT_PLC,PIN_NBR_PLC);
	Sleep(4);
	GPIO_SetBits(PORT_PLC,PIN_NBR_PLC);
}
#ifdef METER_MODULE
unsigned char  shining_485_state = 0;
void Shining485Led(void)
{
	GPIO_ResetBits(PORT_CB_LED, PIN_NBR_CB_LED);
	shining_485_state = 10;	
}
void Close485Led(void)
{
	if(shining_485_state == 1)
	{
		 GPIO_SetBits(PORT_CB_LED, PIN_NBR_CB_LED);
		 shining_485_state = 0;
	}
	else if(shining_485_state>1)
			shining_485_state--;
}
void Set485Led(unsigned char on)
{
	if(on)
		GPIO_ResetBits(PORT_CB_LED, PIN_NBR_CB_LED);		
	else
		GPIO_SetBits(PORT_CB_LED, PIN_NBR_CB_LED);	
}
#endif
#ifdef BIG_CONCENT
/*
	系统运行灯初始化
*/
void Signel_led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
      
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = PIN_NBR_LED_SYSTEM;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PORT_SIGNEL_LED, &GPIO_InitStructure);
	GPIO_SetBits(PORT_LED_SYSTEM,PIN_NBR_LED_SYSTEM);
}
void shining_led(void)
{
#define FLASH_CYCLE 25
	static	 unsigned char on = 0;
//	static	 unsigned char count = 0; 

	if(on)
		GPIO_SetBits(PORT_LED_SYSTEM,PIN_NBR_LED_SYSTEM);
	else
		GPIO_ResetBits(PORT_LED_SYSTEM,PIN_NBR_LED_SYSTEM);
//	if(count++ > FLASH_CYCLE)
	{
//		count = 0;
		on = !on;
	}	
}
#endif

#define  ON_LINE	0xA5A5 
#define  OFF_LINE	0x5A5A 
/*
	在线标志,初始化
*/
unsigned char LineStateInit()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	if ((BKP_ReadBackupRegister(BKP_DR1) != ON_LINE)&&
	(BKP_ReadBackupRegister(BKP_DR1) != OFF_LINE))
	{
	
		PrintLog(LOGTYPE_DEBUG,"\r\n ON LINE STATE not yet configured....");

		BKP_WriteBackupRegister(BKP_DR1, OFF_LINE);
		return 0;//没有在线
	}
	else
	{
		if(BKP_ReadBackupRegister(BKP_DR1) == ON_LINE)
	   		return 1;
		else 	 
		   	return 0;
	}
}
/*
	更新在线标志
	State  	1 已经上线	
			0 未上线	
*/
void UpdateLineStateInit(unsigned char State)
{
	if(1 == State)
	{
		BKP_WriteBackupRegister(BKP_DR1, ON_LINE);
	}
	else
	{
		BKP_WriteBackupRegister(BKP_DR1, OFF_LINE);
	}
}

