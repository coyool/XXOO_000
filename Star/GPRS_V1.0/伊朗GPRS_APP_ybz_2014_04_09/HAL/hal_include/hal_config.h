#ifndef _HEL_CONFIG_H_
#define _HEL_CONFIG_H_
#include "plat_include/sys_config.h"
//#define MC55I_GPRS
#define M35_GPRS 
//#define USE_STM32_RTC

#ifdef METER_MODULE
#define USE_STM32_RTC
#endif
#ifdef BIG_CONCENT
#define USE_DS3231_RTC
#endif
#ifdef SMALL_CONCENT
#define USE_DS3231_RTC
#endif

//#if defined(USE_STM32_RTC)
#include "hal_include/stm32rtc.h"
//#elif 
#if defined(USE_8025_RTC)
#include "hal_include/rx8025.h"
#elif defined(USE_DS3231_RTC)
#include "hal_include/ds3231.h"
#endif

#endif
