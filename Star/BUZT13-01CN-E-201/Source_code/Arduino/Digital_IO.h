#ifndef __DIGITAL_IO_H_
#define __DIGITAL_IO_H_


/*** define and type ***/



/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void pinMode_ALL(GPIO_T *port, u32 Pin, u32 Mode);
extern void digitalWrite(__IO u32* pin, digitalValue_TYPE value);
extern u8 digitalRead(__IO u32* pin);
//extern 
//extern 


#endif   /* end __DIGITAL_IO_H_ */


