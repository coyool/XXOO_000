/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_wd.h 
* 文件标识：见配置管理计划书 
* 摘要：看门狗移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131024
*******************************************************************/
#ifndef _DRV_WD_H_
#define _DRV_WD_H_

/*--------------------宏定义---------------------------*/
#define Stop_watchdog()     {FM3_HWWDT->WDG_LCK = 0x1ACCE551;FM3_HWWDT->WDG_LCK = 0xE5331AAE;FM3_HWWDT->WDG_CTL = 0;}
// 200,000* (1/100kHz) = 2s 
#define Set_watchdog()      {FM3_HWWDT->WDG_LCK = 0x1ACCE551;FM3_HWWDT->WDG_LDR = 200000;\
                             FM3_HWWDT->WDG_LCK = 0x1ACCE551;FM3_HWWDT->WDG_LCK = 0xE5331AAE;\
                             FM3_HWWDT->WDG_CTL = 0x03;}
#define Feed_watchdog()     {FM3_HWWDT->WDG_LCK = 0x1ACCE551;FM3_HWWDT->WDG_ICL = 0x00;FM3_HWWDT->WDG_ICL = 0xFF;}

/*--------------------外部常量申明---------------------*/

/*--------------------外部变量申明---------------------*/


/*--------------------外部函数申明-------------------------*/
void DRV_WD_Init(void);
void DRV_WD_FeedDog(void);







#endif

