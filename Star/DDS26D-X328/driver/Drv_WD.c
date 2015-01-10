

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
#include "Include.h"

/*--------------------全局常量定义---------------------*/

/*--------------------全局变量定义---------------------*/

/*--------------------内部函数申明-------------------------*/
void HWDIntCallbackFunc(void);

/*********************************************************** 
函数功能：看门狗初始化，开始看门狗监控
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_WD_Init(void)
{
	/* Set hardware watchdog count */
	HWD_UnlockReg(HWD_UNLOCK_ALL_EXCEPT_CTRL);
	HWD_SetCount(400000); /* 200,000* (1/100kHz) = 2s */
	/* Disable hardware watchdog reset */
	HWD_UnlockReg(HWD_UNLOCK_ALL);
	HWD_EnableReset();
	/* Enable hardware watchdog interrupt */
	HWD_UnlockReg(HWD_UNLOCK_ALL);
	HWD_EnableInt(HWDIntCallbackFunc);
	DRV_WD_FeedDog();
}

/*********************************************************** 
函数功能：看门狗初喂狗
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_WD_FeedDog(void)
{
	HWD_UnlockReg(HWD_UNLOCK_ALL);
	HWD_ClrIntFlag();
}

/*********************************************************** 
函数功能：看门狗终端回调函数
入口参数：
出口参数：
备注说明:
***********************************************************/
void HWDIntCallbackFunc(void)
{
	return;
}



