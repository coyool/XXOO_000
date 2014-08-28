/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：main
*
* 文件标识：main.c
* 摘    要：MB9AF005   IAP flash updata 
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-6-30
* 编译环境：IAR_for_ARM
* 
* 源代码说明： 直接刷新 片内128KB Flash 
*
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/





/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: 
*******************************************************************************/
int main()
{
    //BSP();       /* Board Support Package */
    
    boot();      /* boot loop */
    
    return 0;
}
