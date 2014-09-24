#ifndef __Digital_IO_H_
#define __Digital_IO_H_


/*** define and type ***/
#define     LOW     Bit_RESET 
#define     HIGH    Bit_SET


/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void pinMode(GPIO_TypeDef* PORT, u16 pin, 
                    GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);
extern void digitalWrite(GPIO_TypeDef* PORT, u16 pin, BitAction BitVal);
extern u8 digitalRead(GPIO_TypeDef* PORT, u16 pin);


#endif   /* end __Digital_IO_H_ */


