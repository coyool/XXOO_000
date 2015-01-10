/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：include.h 
* 文件标识：见配置管理计划书 
* 摘要：全局头文件包含，所有模块应用该头文件

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131022
*******************************************************************/
#ifndef  _INLCUDES_C_
#define  _INLCUDES_C_

#include <string.h>
#include <stdint.h> 

/*--------------------宏定义---------------------------*/
#define DLMS_LIB_USED						1

/*user*/
#include <math.h>
#include "Project.h"
#include "Main.h"
#include "Task.h"
#include "Public.h"

/*target*/
#include"pdl_header.h"

/*protocol*/
#if DLMS_LIB_USED
#include "DLMS\DLMS_ProtocolLib.h"
#include "DLMS\DLMS_Obis.h"
#else 
#include "DLMS\DLMS_Include.h"
#endif




/*driver*/
#include "Drv_IO.h"
#include "Drv_WD.h"
#include "Drv_BT.h"
#include "Drv_LCD.h"
#include "Drv_Uart.h"
//#include "Drv_SPI.h"
//#include "Drv_RF.h"
#include "Drv_RTC.h"
#include "Drv_ADC.h"
#include "PowerManage.h"
#include "IIC.h"
#include "Eeprom.h"
//#include "MX25L3206E.h"
#include "Screen.h"
#include "TempSample.h"
#include "Drv_RTCcal.h"
#include "Drv_RTCSecPulse.h"
#include "Drv_ATT7059.h"
#include "Drv_Calibrate.h"
#include "key.h"
#include "drv_Led.h"
#include "DL645_07.h"

/* RF */
#include "RF_regConfig.h"
#include "RF_SPI.h"
#include "RF.h"
#include "RF_link.h"

/*app*/
#include "Clock.h"
#include "Log.h"
#include "LoadProfile.h"
#include "Billing.h"
#include "Event.h"
#include "Freeze.h"
#include "MeterInfo.h"
//#include "Tariff.h"
#include "App_Api.h"
#include "display.h"
#include "ProtocolManage.h"
#include "Energy.h"
//#include "Demand.h"
#include "RfModel.h"
#include "IEC62056-21.h"        //chy
#include "DataStructTab.h"
#include "NvmOperation.h"
#include "DLMS_AES.h"           //chy











/*--------------------外部常量申明---------------------*/

/*--------------------外部变量申明---------------------*/

/*--------------------外部函数申明-------------------------*/







#endif
