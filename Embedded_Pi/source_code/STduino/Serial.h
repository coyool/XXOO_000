#ifndef __SERIAL_H_
#define __SERIAL_H_


/*** define and type ***/
typedef enum
{
    BIN = 2u,
    OCT = 8u,
    DEC = 10u,
    HEX = 16u
    //...
}NUMBER_TYPE;


/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void Serial_begin(void);
extern void Serial_end(void);
//extern int Serial_print(const char* format,...);
extern int Serial_print(va_list args, NUMBER_TYPE number);
#endif   /* end __SERIAL_H_ */


