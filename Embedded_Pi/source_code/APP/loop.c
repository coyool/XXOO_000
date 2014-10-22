/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：loop
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-9-19
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
u32 temp = 0u;


/*** extern variable declarations ***/






/*******************************************************************************
* 函数名称: loop
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void loop(void)
{
    __NOP();
    for (;;)
    {
        //LED_Blink(LED1, 10, 1000);
        //RF_test();
        LED_on(LED1);
        //delay_ms(1000);
        //delay_us(100);
        delay_s(5);
        LED_off(LED1);
        //delay_ms(1000);
        //delay_us(10);
        delay_s(10);
        
    }//end for(;;)    
}

