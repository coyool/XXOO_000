

/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_lcd.h 
* 文件标识：见配置管理计划书 
* 摘要：LCD移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131024
*******************************************************************/
#include "Include.h"

/*--------------------全局常量定义---------------------*/

/*--------------------全局变量定义---------------------*/

/*--------------------内部函数申明-------------------------*/


/*********************************************************** 
函数功能：LCD初始化函数
入口参数：lcdVV-LCD供电电压:0-高压，1-低压
出口参数：
备注说明:
***********************************************************/
void DRV_LCD_Init(uint8 lcdVV)
{
    uint32_t vvmap, commap, lowsegmap, highsegmap;
    /* Initialize LCD back light */
    //LCD_BACK_LIGHT_IO_PDOR = 0;
    //LCD_BACK_LIGHT_IO_DDR = 1;
    /* LCD clock initialization */
    LCD_SelSrcClk(LCD_CLOCK_MODE);
    LCD_SetClkDiv(LCD_CLOCK_PRES);
    /* Power driver setting */
    LCD_ConfigDivResMode(LCD_DRIVE_MODE);
    if(LCD_DRIVE_MODE == LCDC_INT_RES)
    {
		if(lcdVV)
		{
	       	LCD_SelDivResVal(LCD_INTERNAL_RES_VAL_LP);
		}
		else
		{
	      	LCD_SelDivResVal(LCD_INTERNAL_RES_VAL);
		}
    }
    /* Configure VVx, COMx, SEGx Pin */
    vvmap = (LCD_VV0 << 0) | (LCD_VV1 << 1) | (LCD_VV2 << 2) | (LCD_VV3 << 3) | 
            (LCD_VV4 << 4);
    commap = (LCD_COM0 << 0) | (LCD_COM1 << 1) | (LCD_COM2 << 2) | (LCD_COM3 << 3) | 
             (LCD_COM4 << 4) | (LCD_COM5 << 5) | (LCD_COM6 << 6) | (LCD_COM7 << 7);
    lowsegmap = (LCD_SEG0 << 0) | (LCD_SEG1 << 1) | (LCD_SEG2 << 2) | (LCD_SEG3 << 3) | 
                (LCD_SEG4 << 4) | (LCD_SEG5 << 5) | (LCD_SEG6 << 6) | (LCD_SEG7 << 7) |
                (LCD_SEG8 << 8) | (LCD_SEG9 << 9) | (LCD_SEG10 << 10) | (LCD_SEG11 << 11) |
                (LCD_SEG12 << 12) | (LCD_SEG13 << 13) | (LCD_SEG14 << 14) | (LCD_SEG15 << 15) |
                (LCD_SEG16 << 16) | (LCD_SEG17 << 17) | (LCD_SEG18 << 18) | (LCD_SEG19 << 19) |
                (LCD_SEG20 << 20) | (LCD_SEG21 << 21) | (LCD_SEG22 << 22) | (LCD_SEG23 << 23) |
                (LCD_SEG24 << 24) | (LCD_SEG25 << 25) | (LCD_SEG26 << 26) | (LCD_SEG27 << 27) |
                (LCD_SEG28 << 28) | (LCD_SEG29 << 29) | (LCD_SEG30 << 30) | (LCD_SEG31 << 31);
    highsegmap = (LCD_SEG32 << 0) | (LCD_SEG33 << 1) | (LCD_SEG34 << 2) | (LCD_SEG35 << 3) |
                 (LCD_SEG36 << 4) | (LCD_SEG37 << 5) | (LCD_SEG38 << 6) | (LCD_SEG39 << 7);               
    LCD_ConfigVVPin(vvmap); 
    LCD_ConfigCOMPin(commap);    
    LCD_ConfigSEGPin(lowsegmap, highsegmap);  
    /* LCD input IO cut-off setting */
	//if(lcdVV)
	//{
	//	LCD_ConfigInputIOMode(LCD_INPUT_IO_CUTOFF);
	//}
	//else
	//{
		LCD_ConfigInputIOMode(LCD_INPUT_IO_CONNECT);
	//}
    /* Disable blank display */
    LCD_DisableBlankDisp();
    /* Clear RAM */
    LCD_ClrAllRAMArea();
    /* LCD mode setting */
	if(lcdVV)
	{
		LCD_Sel8COMBias(LCD_8COM_BIAS_LP);	//电池模式下用1/3bias更清晰
	}
	else
	{
		LCD_Sel8COMBias(LCD_8COM_BIAS);
	}
    LCD_SetDispMode(LCD_MODE);
    LCD_EnableTimerModeOpt();		//低功耗下显示配置!!
}

/*********************************************************** 
函数功能：LCD关闭初始化函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_LCD_Uninit(void)
{
    /* Clear RAM */
    LCD_ClrAllRAMArea();
    /* LCD mode setting */
    LCD_SetDispMode(LCD_STOP);
}
    
/*********************************************************** 
函数功能：LCD全部清除
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_LCD_ClrAll(void)
{
	/* Clear RAM */
	LCD_ClrAllRAMArea();
}

/*********************************************************** 
函数功能：LCD全部显示
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_LCD_FillAll(void)
{
	/* Fill RAM */
	LCD_FillAllRAMArea();
}

uint8 testLCD;
/*********************************************************** 
函数功能：LCD测试函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_LCD_Test(void)
{  
	//uint8 i,j;

	if(testLCD=!testLCD)
	{
		DRV_LCD_ClrAll();		
    /*    for(i=0;i<40;i++)
		{
			for(j=0;j<8;j++)
			{
				*((volatile uint8*)(&FM3_LCDC->LCDRAM00 + i)) = 0x01<<j;
			}
		}*/

	}
	else
	{
		DRV_LCD_FillAll();
		/*for(i=0;i<40;i++)
		{
			for(j=0;j<4;j++)
			{
				*((volatile uint8*)(&FM3_LCDC->LCDRAM00 + i)) = 0x11<<j;
			}
		}*/
	}
}


