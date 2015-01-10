
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_io.h 
* 文件标识：见配置管理计划书 
* 摘要：IO移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131024
*******************************************************************/
#ifndef _DRV_IO_H_
#define _DRV_IO_H_

/*--------------------宏定义---------------------------*/
//#define   OFF       0
//#define   ON        1

#define IO_PORT_LED_ALARM		IO_PORT8
#define IO_PIN_LED_ALARM		IO_PINx0

#define IO_PORT_PRO_SWI		IO_PORT6
#define IO_PIN_PRO_SWI		IO_PINx3

#define IO_PORT_KEY_SCR		IO_PORT3
#define IO_PIN_KEY_SCR		IO_PINx8

#define IO_PORT_MEN_POW		IO_PORT3
#define IO_PIN_MEN_POW		IO_PINx5


//======================================================白工RF用
#define M_TXD_LED_OUTPUT    {FM3_GPIO->PFR3&= ~IO_PINxF;FM3_GPIO->PCR3&= ~IO_PINxF;FM3_GPIO->DDR3|= IO_PINxF;}
#define M_TXD_LED_HIGH      {FM3_GPIO->PDOR3|= IO_PINxF;}

#define M_RXD_LED_OUTPUT    {FM3_GPIO->PFR4&= ~IO_PINxC;FM3_GPIO->PCR4&= ~IO_PINxC;FM3_GPIO->DDR4|= IO_PINxC;}
#define M_RXD_LED_HIGH      {FM3_GPIO->PDOR4|= IO_PINxC;}
//======================================================
/*--------------------外部常量申明---------------------*/

/*--------------------外部变量申明---------------------*/


/*--------------------外部函数申明-------------------------*/
void DRV_IO_Init(void);
void DRV_IO_PowerDownInit(void);

void TestIO(void);







#endif
