/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：Hamming
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-10-22
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：calc -- calculate
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/







/*******************************************************************************
* Description : calculate Hamming Weight  分治法
* Syntax      : calcHammingWeight(a ^ b); 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
u8 calcHammingWeight(u8 tmp)
{
    tmp = ((tmp & 0xAA) >> 1) + (tmp & 0x55);
    tmp = ((tmp & 0xCC) >> 2) + (tmp & 0x33);
    tmp = ((tmp & 0xF0) >> 4) + (tmp & 0x0F); 
    return tmp;
}
