#ifndef __MAIN_H_
#define __MAIN_H_


/*** define and type ***/
#define CPU_F ((double)8388608)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))



/*** extern variable declarations ***/
//extern __IO u32 systick_cnt;
//extern __IO u32 systick_ms;
extern const u32 systick_fixedTime;

/*** extern function prototype declarations ***/



#endif   /* end __MAIN_H_ */


