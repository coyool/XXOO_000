/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：EXIT shield lib
*
* 文件标识：
* 摘    要：
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-9-24
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/




/*******************************************************************************
* Description : EXTI interrupt enable or disable
*               MRx: 线x上的中断屏蔽 (Interrupt Mask on line x)
* Syntax      : EXTI_IE(ENABLE);  
*               EXTI_IE(DISABLE); 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void EXTI_IE(u32 EXTI_Line, FunctionalState status)
{
    /* Clear EXTI line configuration */
    EXTI->IMR &= ~EXTI_Line;
    EXTI->EMR &= ~EXTI_Line;
    
    if (status == ENABLE)
    {
        EXTI->IMR |= EXTI_Line;   
    }
    else
    {
        EXTI->IMR &= ~EXTI_Line;    
    }
}



