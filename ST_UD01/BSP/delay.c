/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：
*
* 文件标识：delay.c
* 摘    要：software delay
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-8-28
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/




/*******************************************************************************
* 函数名称: delay 
* 输入参数: cnt  
* 输出参数: 
* --返回值: 
* 函数功能: -- 
*******************************************************************************/
void delay_ms(uint32_t Cnt)
{
    uint32_t i;
    
    for (; Cnt ; Cnt--)
    {    
        for (i=SystemCoreClock/3200; i; i--);
    }    
}


static void PublicCtlDelay(unsigned long ulCount)
{

  
    __asm("    subs    r0, #1\n"
          "    bne.n   PublicCtlDelay\n"
          "    bx      lr");
}

/*********************************************************** 
函数功能：延时函数 Xms
入口参数：ms
出口参数：
备注说明:
***********************************************************/
void PublicDelayMs(uint32_t ms)
{
  PublicCtlDelay((ms * (SystemCoreClock/3000)));
}

/*********************************************************** 
函数功能：延时函数 Xus
入口参数：us
出口参数：
备注说明:实际delay=(2.6+x)us
***********************************************************/
void PublicDelayUs(uint16_t us)
{
	uint32_t n;

	//n=T/t=(xus/1000000)/((1/SystemCoreClock)*XX)	//XX为循环周期指令数
	n=(uint32_t)us*((SystemCoreClock/5000000UL)+1);	//5000000UL针对每个单片机需要调试
	for(;n>0;n--)
	{
		//DRV_WD_FeedDog();
	}
}

