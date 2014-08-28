/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：button
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-8-18
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/





/*******************************************************************************
* 函数名称: button
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void BUTTON_KEY_setup(void)
{
    bFM3_LCDC_LCDCC3_PICTL = 1; //P56 P1D 管脚为SEG管脚，当需要配置为GPIO输入时，使用该语句！

    //bFM3_GPIO_ADE_AN13 = 0;   // 0 GPIO  1特殊功能
    bFM3_GPIO_PFR3_PE = 0;      // 0 GPIO  1外设功能
    bFM3_GPIO_DDR3_PE = 0;      // 0 输入  1输出
    bFM3_GPIO_PCR1_PD = 1;    // 上拉
}


