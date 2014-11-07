/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：PLC
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-11-7
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/





/*******************************************************************************
* Description : PLC_setup
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void PLC_setup(void)
{
    /* PLC Tx Pin set */
    pinMode_ALL(P0, 0, OUTPUT);
    pinMode_ALL(P0, 1, OUTPUT);
    
    /* PLC Tx Timer */
    
}
