#ifndef _STM32_RTC_H_
#define _STM32_RTC_H_
#include "stm32f10x.h"
//��ʼ��ʱ��
void Stm32RtcInit(u32 default_time);
//��ʱ��
u32 stm32rtc_read(void);
//дʱ��
void stm32rtc_set(u32 time);

#endif
