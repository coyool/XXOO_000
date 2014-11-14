#ifndef __ARDUINO_H_
#define __ARDUINO_H_



/*** define and type ***/
typedef enum
{
    LOW = 0,
    HIGH
}digitalValue_TYPE;

typedef enum 
{
    INPUT = 0x0UL,        /*!< Input Mode */
    OUTPUT = 0x1UL,       /*!< Output Mode */
//    INPUT_PULLUP,  
//    推免
    OPEN_DRAIN = 0x2UL,   /*!< Open-Drain Mode */
    QUASI_IO = 0x3UL      /*!< Quasi-bidirectional Mode */
//    PWM
}pinMode_TYPE;


typedef enum
{
    FALLING,
    RISING,
    CHANGE,
    //LOW    = xGPIO_LOW_LEVEL,
    //HIGH   = xGPIO_HIGH_LEVEL,
}pinIntMode_TYPE;

typedef enum
{
    EXTERNAL,
    DEFAULT,
    INTERNAL,
}AnalogRefer_TYPE;

typedef enum
{
    OFF = 0,
    ON
}SWITCH_TYPE;

/*
*   Stores M0516 specific information related to a given Maple pin.
*/
typedef struct  
{ 
    GPIO_T *port;
    u32 pin;
    
    
}M0516_PIN_TYPE;





/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
//extern 
//extern 
//extern 
//extern 
//extern 


/*** include ***/
#include "Advanced_IO.h"
#include "Digital_IO.h"
#include "Serial.h"
#include "Time.h"

#endif   /* end __ARDUINO_H_ */


