#ifndef __DIGITAL_IO_H_
#define __DIGITAL_IO_H_


/*** define and type ***/
//#define  digitalWrite(pin,value)  pin = value
//#define  digitalRead(pin)         pin

#define P00_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x00)
#define P01_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x04)
#define P02_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x08)
#define P03_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x0C)
#define P04_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x10)
#define P05_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x14)
#define P06_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x18)
#define P07_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x0C)
#define P10_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x20)
#define P11_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x24)
#define P12_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x28)
#define P13_DOUT           (volatile uint32_t *)(GPIO_PIN_DATA_BASE+0x2C)



/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void pinMode_ALL(GPIO_T *port, u32 Pin, u32 Mode);
extern void digitalWrite(u32 pin, digitalValue_TYPE value);
extern u8 digitalRead(u32 pin);
//extern 
//extern 


#endif   /* end __DIGITAL_IO_H_ */


