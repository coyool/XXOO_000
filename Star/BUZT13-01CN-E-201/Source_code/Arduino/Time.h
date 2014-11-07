#ifndef __TIME_H_
#define __TIME_H_


/*** define and type ***/
//#define delayUs(us)    CLK_SysTickDelay(us)   //can't use it


/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern u32 millis(void); 
extern u32 micros(void);
extern void delayMs(u32 ms);
extern void delayUs(u32 us);
//extern 


#endif   /* end __TIME_H_ */


