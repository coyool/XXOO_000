#ifndef __ARDUINO_H_
#define __ARDUINO_H_



/*** define and type ***/
typedef enum
{
    LOW = 0,
    HIGH
}digitalValue_TYPE;

typedef enum {
    INPUT = 0x0UL,        /*!< Input Mode */
    OUTPUT = 0x1UL,       /*!< Output Mode */
//    INPUT_PULLUP,   
    OPEN_DRAIN = 0x2UL,   /*!< Open-Drain Mode */
    QUASI_IO = 0x3UL      /*!< Quasi-bidirectional Mode */
//    PWM
}pinMode_TYPE;

//#define GPIO_PMD_INPUT          0x0UL /*!< Input Mode */
//#define GPIO_PMD_OUTPUT         0x1UL /*!< Output Mode */
//#define GPIO_PMD_OPEN_DRAIN     0x2UL /*!< Open-Drain Mode */
//#define GPIO_PMD_QUASI          0x3UL /*!< Quasi-bidirectional Mode */

typedef enum{
    FALLING,
    RISING,
    CHANGE,
    //LOW    = xGPIO_LOW_LEVEL,
    //HIGH   = xGPIO_HIGH_LEVEL,
}pinIntMode_TYPE;

typedef enum{
    EXTERNAL,
    DEFAULT,
    INTERNAL,
//    INTERNAL1V1,
//    INTERNAL2V56,
}AnalogRefer_TYPE;

typedef enum
{
    OFF = 0,
    ON
}SWITCH_TYPE;








/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
//extern 
//extern 
//extern 
//extern 
//extern 


/*** include ***/
#include "Digital_IO.h"
#include "Serial.h"
#include "Time.h"

#endif   /* end __ARDUINO_H_ */


