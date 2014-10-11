/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：Interrput
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-10-11
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/






/*******************************************************************************
* Description : Re-enables interrupts (after they've been disabled by 
*               noInterrupts()). Interrupts allow certain important tasks to 
*               happen in the background and are enabled by default. Some 
*               functions will not work while interrupts are disabled, and 
*               incoming communication may be ignored. Interrupts can slightly 
*               disrupt the timing of code, however, and may be disabled for 
*               particularly critical sections of code.           
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void Interrupts(void)
{
    __set_PRIMASK(0);  
#ifdef  CPU_Pro  0
    __set_FAULTMASK(0);
#endif
}

/*******************************************************************************
* Description : Disables interrupts (you can re-enable them with interrupts()). 
*               Interrupts allow certain important tasks to happen in the 
*               background and are enabled by default. Some functions will not 
*               work while interrupts are disabled, and incoming communication 
*               may be ignored. Interrupts can slightly disrupt the timing of 
*               code, however, and may be disabled for particularly critical 
*               sections of code.
*
*               iar private : 
*               __enable_irq()
*                   
*               core fuction: 
*               __set_PRIMASK(1) -- only allow NMI and hard fault, All other 
*               interrupt/exception are blocked. (The current CPU priority = 0).
*
*               __set_FAULTMASK(1) -- only allow NMI, All other 
*               interrupt/exception are blocked. (The current CPU priority = -1).    
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void noInterrupt(void)
{
    __set_PRIMASK(1);
#ifdef  CPU_Pro  0
    __set_FAULTMASK(1);
#endif
}

