/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：delay
*
* 文件标识：
* 摘    要：
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-9-19
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：待确定
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/








/*******************************************************************************
* Description : delay_us   delay_us(10) = 13us
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : cnt = us * (SystemCoreClock/72000000UL);	
*******************************************************************************/
void delayUs(__IO u32 us)
{
    __IO u32 cnt;
    
    for (; us>0; us--)
    {
        for (cnt=5u; cnt>0; cnt--);
    }
}


/*******************************************************************************
* Description : daley_ms   delay_ms(10) = 13ms
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void delayMs(__IO u32 ms)
{
    __IO u32 cnt;

    //cnt = us * (SystemCoreClock/72000000UL);	
    for (; ms>0; ms--)
    {
        for (cnt=5550u; cnt>0; cnt--)
        {
            __NOP();
        }
    }
}

/*******************************************************************************
* Description : delay_s
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void delay_S(__IO u32 cnt)
{
    for ( ; cnt>0; cnt--)
    {
        delayMs(1000u);
    }    
}










