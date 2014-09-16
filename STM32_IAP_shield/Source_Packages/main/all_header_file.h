#ifndef __all_H_
#define __all_H_


/*** C standard lib ***/
#include <stdio.h>      /* import printf scanf   */ 
#include <stdlib.h>     /* import string convert */
#include <string.h>     /* import memcpy memset memcmp ¡£¡£¡£*/

/*** Cortex-M3 lib ***/
#include <intrinsics.h>	/* _nop_() enalbe ICCARM (IAR) */




/*** STM32 lib ***/
#include  "stm32f10x.h"
//#include  "stm32f10x_conf.h"





/***  user lib ***/
#include  "def_and_type.h"
#include  "BSP.h"
#include  "UART_Drv.h"
#include  "Pin.h"








#endif





