#ifndef _STM32_RTC_H_
#define _STM32_RTC_H_
#include "stm32f10x.h"
//初始化时钟
void Stm32RtcInit(u32 default_time);
//读时钟
u32 stm32rtc_read(void);
//写时钟
void stm32rtc_set(u32 time);

#endif
