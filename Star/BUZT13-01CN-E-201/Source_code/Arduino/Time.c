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
    u32 ms = 0;
    
    ms = systick_ms;
    
    return ms;
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
u32 micros(void)
{
    u32 ms;
    u32 cycle_cnt;
    u32 res;

    do 
    {
        cycle_cnt = SysTick->VAL;
        ms = systick_ms;
    }while (ms != systick_ms);

    res = (ms * 1000u) +
          (((CyclesPerUs * systick_fixedTime) - cycle_cnt) / CyclesPerUs);

    return res;
}


/*******************************************************************************
* Description : Pauses the program for the amount of time (in miliseconds) 
*               specified as parameter.
* warnings    : #define delayUs(us)    CLK_SysTickDelay(us) 
*               occupy SysTick(core clock)!!!
* Syntax      : delay(ms)
* Parameters I: ms: the number of milliseconds to pause (u32)
* Parameters O: 
* return      : nothing
*******************************************************************************/
void delayMs(u32 ms)
{
    for ( ; ms>0; ms--)
    {
        delayUs(1000u);
    }
}

/*******************************************************************************
* Description : Pauses the program for the amount of time (in microseconds) 
*               specified as parameter. 
* Caveats     : This function works very accurately in the range 10 microseconds 
*               and up. It cannot assure that delayMicroseconds will perform 
*               precisely for smaller delay-times.  
*               10u(11.6us) 100us(108.7us) 500(540.9us) 1000us(1.08ms)
* Syntax      : delayUs(us)
* Parameters I: us -- the number of microseconds to pause (u32)
* Parameters O: 
* return      : None
*******************************************************************************/
void delayUs(u32 us)
{
    u32 cnt = 0u;
    
    cnt = 9u * us;
    for ( ; cnt>0; cnt--);
}




