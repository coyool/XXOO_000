/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_rtc.h 
* 文件标识：见配置管理计划书 
* 摘要：基本定时器移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131104
*******************************************************************/
#ifndef _DRV_RTC_H_
#define _DRV_RTC_H_

/*--------------------宏定义---------------------------*/
typedef struct 			//年 月 日 周 时 分 秒 百分之一秒 偏差 状态
{
	uint8 Status;
	int16 Deviation;
	uint8 HSec;
	uint8 Sec;
	uint8 Min;
	uint8 Hour;
	uint8 Week;			//1-7对应:周一到周日
	uint8 Date;
	uint8 Mon;
	uint16 Year;
}DATE_TIME;

//时钟状态定义，同DLMS
#define RTC_STATUS_INVALID_VALUE	0x01		//发生事件后时间不能恢复，详情由制造商规定（例如：在时钟的电源被中断后）。
#define RTC_STATUS_DOUBTFUL_VALUE		0x02	//发生事件后时间能恢复，但是数值不能被保证，详情况由制造商具体规定。
#define RTC_STATUS_DIFF_CLOCK_BASE		0x04	//如果实际的时钟基准与规定源的时钟基准不同，该比特将会被置。
#define RTC_STATUS_INVALID_STATUS		0x08	//此位表示至少有一个时钟状态位是无效的，某些位可能是正确的
//#define RTC_STATUS_		0x10
//#define RTC_STATUS_		0x20
//#define RTC_STATUS_		0x40
//#define RTC_STATUS_DSA		0x80			//夏令时由应用提供//标志位置为真：发出时间包含夏令时偏差（夏季）；标志位置为假：发出时间不包含夏令时偏差（正常时间）

//RST_STR: ReSeT STatus Register)
#define RST_SRST            0x0100    
#define RST_FCSR        	0x0080      
#define RST_CSVR			0x0040
#define RST_HWDT			0x0020	
#define RST_SWDT			0x0010
#define RST_LVDH			0x0008
#define RST_INITX			0x0002
#define RST_PONR			0x0001

typedef struct
{
//  DATE_TIME dateTime;
  tDateTime dateTime;
  uint16 CheckSum;    //校验和
}tEE_DATE_TIME;


#define  ONESECODERTCPPM   10000000  //扩大10倍  

#define  MAXSECODECALI     2    //100S*365*3 =109500  每天错100S  3年停电时间
typedef struct 
{
  int32  RTC_Error_ACC;   //累计的PPM
  int32  ErrorSeconds;    //修正多少秒
  int32  ResultCaliPPM;   // 晶振原始频偏 + 温度的频偏 
  int16  CaliDirect;     // 秒的增加或者减少
  uint16  checkSum;
}SOFTCALIPPM_TYPE;
/*--------------------外部常量申明---------------------*/

/*--------------------外部变量申明---------------------*/
extern uint8 SecFlag;
extern uint8 MinFlag;
extern tEE_DATE_TIME G_CurDateTime;
extern __no_init tEE_DATE_TIME CalcPpmDateTime;
extern __no_init  SOFTCALIPPM_TYPE  SoftCaliPPM;

/*--------------------外部函数申明-------------------------*/
void DRV_RTC_Rest(uint8 flag);
void DRV_RTC_Init(void);
void DRV_RTC_InitUpBatt(void);
void DRV_RTC_InitPowerDown(void);
void DRV_RTC_FlashTask(void);
void DRV_RTC_Get(tDateTime *dateTime);
BOOL_B DRV_RTC_Set(tDateTime *dateTime);
uint8 DRV_RTC_Chk(tDateTime *c_rtc);
uint8 DRV_RTC_GetSecState(void);
uint8 DRV_RTC_GetMinState(void);
uint8 DRV_RTC_GetHourState(void);
void DRV_RTC_Test(void);
void UserRTCSecIntCallback(void);
void DRV_RTCSave(void);
void DRV_RTCLoad(void);
void DRV_RTCPowerDownTimeGet(tDateTime *dateTime);
void DRV_RTC_ResetState(void);
void DRV_RTC_ValueRest(void);
uint8 CheckRTCSfr(void);
void DRV_RTC_CalcSecDeviation(void);
uint8  DRV_RTC_CheckSoftCailValue(void);

#endif
