/*******************************************************************************
* Copyright 2015      Jr 
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：Jr
* 完成日期：2015-1-23
* 编译环境：D:\software\IAR_for_ARM\arm   IAR for ARM 6.50 
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/





/*******************************************************************************
* Description : Configures the reference voltage used for analog input (the value used as the top of the input range). 
*               The options are:
*               DEFAULT: the default analog reference of 5 volts  or 3.3 volts (JP1)
*               INTERNAL: an built-in reference, equal to __volts on the Embedded Pi
*               EXTERNAL: the voltage applied to the AREF pin (0 to 5V only) is used as the reference.
* Syntax      : analogReference(type)
* Parameters I: type: which type of reference to use (DEFAULT, INTERNAL, or EXTERNAL).
* Parameters O: 
* return      : 
* Note        : After changing the analog reference, the first few readings from analogRead() may not be accurate.
* Warning     : Don't use anything less than 0V or more than 5V for external reference voltage on the AREF pin! 
*               If you're using an external reference on the AREF pin, you must set the analog reference to EXTERNAL before calling analogRead(). 
*               Otherwise, you will short together the active reference voltage (internally generated) and the AREF pin, 
*               possibly damaging the microcontroller on your Arduino board
*******************************************************************************/
void analogReference(void)
{
	/* Check the parameters */
	ASSERT ();
	
	
	
}
