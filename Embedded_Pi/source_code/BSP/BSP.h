#ifndef __BSP_H_
#define __BSP_H_


/*** define and type ***/
#define SysTick_ENABLLE(a)   if(a)\
                                  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;\
                                  else\
                                  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk

typedef struct
{
    u32 systick_cnt;
    
}TIMER_TYPE;



/*** extern variable declarations ***/
extern TIMER_TYPE timer;


/*** extern function prototype declarations ***/
extern void BSP(void);


#endif   /* end __BSP_H_ */


