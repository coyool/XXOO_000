/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_lcd.h 
* 文件标识：见配置管理计划书 
* 摘要：LCD移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131025
*******************************************************************/
#ifndef _DRV_LCD_H_
#define _DRV_LCD_H_

/*--------------------宏定义---------------------------*/
#define PIN_OFF             0
#define PIN_ON              1 

#define USE_MB9AFA32N_EVB
#if defined (USE_MB9AFA32N_EVB)
/*-   Hardware pins configuration    -*/
/* SEG&COM number */
#define LCD_SEG_NUM           20
#define LCD_COM_NUM           8
/* Segment */
#define LCD_SEG0              PIN_OFF
#define LCD_SEG1              PIN_OFF
#define LCD_SEG2              PIN_OFF
#define LCD_SEG3              PIN_OFF
#define LCD_SEG4              PIN_OFF
#define LCD_SEG5              PIN_OFF
#define LCD_SEG6              PIN_ON
#define LCD_SEG7              PIN_ON
#define LCD_SEG8              PIN_ON
#define LCD_SEG9              PIN_ON
#define LCD_SEG10             PIN_ON
#define LCD_SEG11             PIN_ON
#define LCD_SEG12             PIN_ON
#define LCD_SEG13             PIN_ON
#define LCD_SEG14             PIN_ON
#define LCD_SEG15             PIN_ON
#define LCD_SEG16             PIN_ON
#define LCD_SEG17             PIN_ON
#define LCD_SEG18             PIN_ON
#define LCD_SEG19             PIN_ON
#define LCD_SEG20             PIN_ON
#define LCD_SEG21             PIN_ON
#define LCD_SEG22             PIN_ON
#define LCD_SEG23             PIN_ON
#define LCD_SEG24             PIN_OFF
#define LCD_SEG25             PIN_OFF
#define LCD_SEG26             PIN_OFF
#define LCD_SEG27             PIN_OFF
#define LCD_SEG28             PIN_OFF
#define LCD_SEG29             PIN_OFF
#define LCD_SEG30             PIN_OFF
#define LCD_SEG31             PIN_OFF
#define LCD_SEG32             PIN_OFF
#define LCD_SEG33             PIN_OFF
#define LCD_SEG34             PIN_OFF
#define LCD_SEG35             PIN_OFF
#define LCD_SEG36             PIN_OFF
#define LCD_SEG37             PIN_OFF
#define LCD_SEG38             PIN_OFF
#define LCD_SEG39             PIN_OFF

/* COM */
#define LCD_COM0              PIN_ON
#define LCD_COM1              PIN_ON
#define LCD_COM2              PIN_ON
#define LCD_COM3              PIN_ON
#define LCD_COM4              PIN_ON
#define LCD_COM5              PIN_ON
#define LCD_COM6              PIN_ON
#define LCD_COM7              PIN_ON 

/* VV */
#define LCD_VV0               PIN_OFF
#define LCD_VV1               PIN_OFF
#define LCD_VV2               PIN_OFF
#define LCD_VV3               PIN_OFF
#define LCD_VV4               PIN_ON

/* LCD back light */
//#define LCD_BACK_LIGHT_IO_DDR    bFM3_GPIO_DDR3_P5
//#define LCD_BACK_LIGHT_IO_PDOR   bFM3_GPIO_PDOR3_P5

/*-      Software     -*/
/* LCD control */
#define LCD_MODE                 LCD_8_COM_1DIV8_DUTY
#define LCD_8COM_BIAS            LCD_1DIV4_BIAS
#define LCD_8COM_BIAS_LP         LCD_1DIV3_BIAS		//电池模式下用1/3bias更清晰
#define LCD_DRIVE_MODE           LCDC_INT_RES
#define LCD_INTERNAL_RES_VAL     LCD_DIV_RES_10k
#define LCD_INTERNAL_RES_VAL_LP  LCD_DIV_RES_100k
#define LCD_IO_INPUT_MODE        LCD_INPUT_IO_CONNECT
/* LCD clock */
#define LCD_CLOCK_MODE           LCD_CLOCK_SEL_SUBCLK
#define LCD_CLOCK_PRES           68				//32768/68/8=60hz

#else
    #error Select a certain evaluation board!	
#endif	


/*--------------------外部常量申明---------------------*/

/*--------------------外部变量申明---------------------*/


/*--------------------外部函数申明-------------------------*/
void DRV_LCD_Init(uint8 lcdVV);
void DRV_LCD_Uninit(void);
void DRV_LCD_ClrAll(void);
void DRV_LCD_FillAll(void);

void DRV_LCD_Test(void);



#endif /* _RESOURCE_H_ */


