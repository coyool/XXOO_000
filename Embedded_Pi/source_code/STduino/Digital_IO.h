#ifndef __Digital_IO_H_
#define __Digital_IO_H_


/*** define and type ***/
#define     LOW     Bit_RESET 
#define     HIGH    Bit_SET

typedef enum
{
    PA0 = 0,
    PA1,
    PA2,
    PA3,
    PA4,
    PA5,
    PA6,
    PA7,
    PA8,
    PA9,
    PA10,
//    PA11, // USB_DM
//    PA12, // USB_DP
//    PA13, // SWDIO__TMS
//    PA14, // SWCLK__TCLK
//    PA15, // TDI__D8
    PB0,
    PB1,
//    PB2,  // BOOT1
//    PB3,  // SWO TDO
//    PB4,  // USB_DISC__TRST
    PB5,
    PB6,
    PB7,
    PB8,
    PB9,
    PB10,
    PB11,
    PB12,
    PB13,
    PB14,
    PB15,
    PC0,
    PC1,
    PC2,
    PC3,
    PC4,
    PC5,
    PC6,
    PC7,
    PC8,
    PC9,
    PC10,
    PC11,
    PC12,
    PC13,
//    PC14, // OSC32_IN
//    PC15, // OSC32_OUT
//    PD0,  // OSC_IN
//    PD1,  // OSC_OUT
    PD2
}STM32F103x_PIN_TYPE;


typedef struct
{
     GPIO_TypeDef*  PORTx;
     u16            pinx;
}PORT_PIN_TYPE;

/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void pinMode_ALL(GPIO_TypeDef* PORT, u16 pin, 
                        GPIOMode_TypeDef mode, GPIOSpeed_TypeDef speed);
extern void digitalWrite_ALL(GPIO_TypeDef* PORT, u16 pin, BitAction BitVal);
extern u8 digitalRead_ALL(GPIO_TypeDef* PORT, u16 pin);

extern void pinMode_Px(STM32F103x_PIN_TYPE pin, GPIOMode_TypeDef mode);
extern void digitalWrite_Px(STM32F103x_PIN_TYPE pin, BitAction BitVal);
extern u8 digitalRead_Px(STM32F103x_PIN_TYPE pin);


#endif   /* end __Digital_IO_H_ */


