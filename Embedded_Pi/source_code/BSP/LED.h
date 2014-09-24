#ifndef __LED_H_
#define __LED_H_


/*** define and type ***/
/* Embedded_Pi hardware config */
#define LEDn                             1
#define LED1_PIN                         GPIO_Pin_13
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOB


#define LED1(a) if (a)  \
                  GPIO_SetBits(GPIOB,GPIO_Pin_13);\
                  else        \
                  GPIO_ResetBits(GPIOB,GPIO_Pin_13)   

typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3,
  LED5 = 4     
}Led_TypeDef;







/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void LED_setup(Led_TypeDef LED);
extern void LED_Blink(Led_TypeDef LED, __IO u32 cnt, const u32 interval);
extern void LED_on(Led_TypeDef LED);
extern void LED_off(Led_TypeDef LED);


#endif   /* end __LED_H_ */


