#ifndef   _ALL_H_
#define   _ALL_H_

/*** C standard lib ***/
#include <stdio.h>      /* import printf scanf   */ 
//#include <stdlib.h>     /* import string convert */
//#include <string.h>     /* import memcpy memset memcmp ... */
//#include <stdarg.h>


/*** cortex M0 lib ***/
//#include <intrinsics.h>	/* _nop_() enalbe */

#include "M051Series.h"
#include "system_M051Series.h"


/***  user lib ***/
#include  "def_and_type.h"
#include "main.h"

/* APP lib */
//#include "RF.h"
#include "PLC_PWM.h"


/* BSP lib */




/* OpenSource lib */
#include "CRC.h"
//#include "FEC.h"
//#include "Hamming.h"


/* shield lib */
#include "LED.h"
#include "PLC.h"


/* arduino lib */
#include "Arduino.h"






#endif