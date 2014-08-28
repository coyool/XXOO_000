/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：LED
*
* 文件标识：LED.c
* 摘    要：LED lib
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-7-4
* 编译环境：IAR_for_ARM
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/



/*******************************************************************************
* 函数名称: LED IO init
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: -- 需要发哥帮改
*******************************************************************************/
void LED_IO_init(void)
{
    bFM3_LCDC_LCDCC3_PICTL = 1; //P56 P1D 管脚为SEG管脚，当需要配置为GPIO输入时，使用该语句！

    //bFM3_GPIO_ADE_AN13 = 0;   // 0 GPIO  1特殊功能
    bFM3_GPIO_PFR5_P5 = 0;      // 0 GPIO  1外设功能
    bFM3_GPIO_DDR5_P5 = 1;      // 0 输入  1输出
    //bFM3_GPIO_PCR1_PD = 1;    // 上拉
}


