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
    u32 i;
    
    port->PMD = (port->PMD & ~(0x3 << (Pin << 1))) | (u32Mode << (i << 1));   
}

/*******************************************************************************
* Description : digitalWrite
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void digitalWrite(__IO u32* pin, digitalValue_TYPE value)
{
    pin = value;
}

/*******************************************************************************
* Description : digitalRead
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
u8 digitalRead(__IO u32* pin)
{
    u8 return_val;
    
    if (1 == pin)
    {
        return_val = HIGH;
    }
    else
    {
        return_val = LOW;
    }
    
    return return_val;
}


