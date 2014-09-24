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
* 源代码说明：
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
void delay_us(__IO u32 us)
{
    for (; us>0; us--);
}


/*******************************************************************************
* Description : daley_ms   delay_ms(10) = 13ms
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void delay_ms(__IO u32 ms)
{
    __IO u32 cnt;

    //cnt = us * (SystemCoreClock/72000000UL);	
    for (; ms>0; ms--)
    {
        for (cnt=1000u; cnt>0; cnt--);
    }
}

/*******************************************************************************
* Description : delay_s
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void delay_s(__IO u32 cnt)
{
    for ( ; cnt>0; cnt--)
    {
        delay_ms(1000u);
    }    
}










