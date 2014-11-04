/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：Digital I/O
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-11-3
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/






/*******************************************************************************
* Description : Configures the specified pin to behave either as an input or an 
*               output,other.
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void pinMode_ALL(GPIO_T *port, u32 Pin, u32 Mode)
{    
    port->PMD = (port->PMD & ~(0x3 << (Pin << 1))) | (Mode << (Pin << 1));   
}

/*******************************************************************************
* Description : 效率低 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void digitalWrite(u32 pin, digitalValue_TYPE value)
{
    /* assert */
    ASSERT_PARAM(((pin%10) < 8)&& (pin < 48));
    ASSERT_PARAM((value==HIGH)||(value==LOW));
    
    GPIO_PIN_ADDR(pin/10, pin%10) = HIGH;
}

/*******************************************************************************
* Description : 效率低
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
u8 digitalRead(u32 pin)
{
    u8 return_val = 0u;
    
    /* assert */
    ASSERT_PARAM(((pin%10) < 8)&& (pin < 48));
    
    return_val = GPIO_PIN_ADDR(pin/10, pin%10); 
    
    return return_val;
}


