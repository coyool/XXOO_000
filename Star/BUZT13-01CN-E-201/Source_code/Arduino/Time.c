/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：Time
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-10-31
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/



/*******************************************************************************
* Description : Returns the number of milliseconds since the board began
*               running the current program. This number will overflow (go back 
*               to zero), after approximately XX days
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : Number of milliseconds since the program started (u32)
*******************************************************************************/
u32 millis(void)
{
    u32 cnt = 0;
    return cnt;
}

/*******************************************************************************
* Description : Returns the number of microseconds since the Arduino board began
*               running the current program. This number will overflow (go back 
*               to zero), after approximately XX minutes
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : Number of microseconds since the program started (u32)
*******************************************************************************/
void micros(void)
{
    
}


/*******************************************************************************
* Description : Pauses the program for the amount of time (in miliseconds) 
*               specified as parameter.
* Syntax      : delay(ms)
* Parameters I: ms: the number of milliseconds to pause (u32)
* Parameters O: 
* return      : nothing
*******************************************************************************/
void delayMs(u32 ms)
{
    for (; ms>0; ms--);
    {
        delayUs(1000);
    }
}

/*******************************************************************************
* Description : Pauses the program for the amount of time (in microseconds) 
*               specified as parameter. 
* Caveats     : This function works very accurately in the range 10 microseconds 
*               and up. It cannot assure that delayMicroseconds will perform 
*               precisely for smaller delay-times.            
* Syntax      : delayUs(us)
* Parameters I: us -- the number of microseconds to pause (u32)
* Parameters O: 
* return      : None
*******************************************************************************/
void delayUs(u32 us)
{
    for (; us>0; us--);
}




