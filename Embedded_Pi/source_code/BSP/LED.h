#ifndef __LED_H_
#define __LED_H_


/*** define and type ***/
/* Embedded_Pi hardware config */
#define LEDn        1




#define LED1(a) if (a)  \
                  GPIO_SetBits(GPIOB,GPIO_Pin_13);\
                  else        \
                  GPIO_ResetBits(GPIOB,GPIO_Pin_13)   

typedef enum 
{
    LED1 = 1,
    LED2 = 2,
    LED3 = 3,
    LED4 = 4,
    LED5 = 5     
}LED_TYPEDEF;







/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void LED_setup(LED_TYPEDEF LED);
extern void LED_Blink(LED_TYPEDEF LED, __IO u32 cnt, const u32 interval);
extern void LED_on(LED_TYPEDEF LED);
extern void LED_off(LED_TYPEDEF LED);


#endif   /* end __LED_H_ */


