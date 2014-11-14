#ifndef __ADVANCED_IO_H_
#define __ADVANCED_IO_H_


/*** define and type ***/
#define PWM_dutyRatio_50  50


typedef struct
{
    PWM_T *PWMx;    /* PWM Group A or B */
    u32 ChannelNum; /* PWM channel number. Valid values are between 0~3 */ 
    IRQn_Type PWMx_IRQn; /* PWM Group A or B IRQ */
}PWM_INFO_TYPE;


/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void tone(PWM_T *PWMx, u32 PWM_CHx, IRQn_Type PWMx_IRQn, u32 frequency);
extern void noTone(PWM_T *PWMx, u32 PWM_CHx, IRQn_Type PWMx_IRQn);
//extern 
//extern 


#endif   /* end __ADVANCED_IO_H_ */


