/*!
 ******************************************************************************
 **
 ** \file rtc.h
 **
 ** \brief Real Time Clock drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-21
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */ 
#ifndef _RTC_FM3_H_
#define _RTC_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_RTC == OFF)
    #if (PERIPHERAL_ENABLE_RTC == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_AVAILABLE_RTC == ON) && (PERIPHERAL_ENABLE_RTC == ON)

/*---------------------------------------------------------------------------*/
/* RTC configuration                                                         */
/*---------------------------------------------------------------------------*/
#define  RTC_TYPE_A         0
#define  RTC_TYPE_B         1

#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE4) || (MCU_TYPE == DEVICE_TYPE5) 
    #define  RTC_CONFIG_TYPE      RTC_TYPE_A 
#else 
    #define  RTC_CONFIG_TYPE      RTC_TYPE_B
#endif

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*  RTC source clock  */
/*! \brief Select sub clock */
#define RTC_CLOCK_IN_SUB_CLOCK     0
/*! \brief Select main clock */
#define RTC_CLOCK_IN_MAIN_CLOCK    1

#if (RTC_CONFIG_TYPE == RTC_TYPE_A)
    /*! \brief Maximum caibration value difinition */
    #define RTC_CALIBRATION_MAX_VALUE     (0x7F)
#else
    /*! \brief Maximum caibration value difinition */
    #define RTC_CALIBRATION_MAX_VALUE     (0x3FF)
#endif
/*! \brief Check parameter */
#define IS_CALI_VAL_VALID(Val)            (Val <= RTC_CALIBRATION_MAX_VALUE)

/*  RTC divider operation  */
/*! \brief Enable RTC divider */
#define RTC_DIV_DISABLE             0
/*! \brief Disable RTC divider */
#define RTC_DIV_ENABLE              1    

#if (RTC_CONFIG_TYPE == RTC_TYPE_A)
    /*  RTC divider raido value  */
    /*! \brief Subout freq = source clock */
    #define RTC_DIV_RATIO_SETTING_1             0
    /*! \brief Subout freq = source clock/2 */
    #define RTC_DIV_RATIO_SETTING_2             1
    /*! \brief Subout freq = source clock/4 */
    #define RTC_DIV_RATIO_SETTING_4             2
    /*! \brief Subout freq = source clock/8 */
    #define RTC_DIV_RATIO_SETTING_8             3
    /*! \brief Subout freq = source clock/16 */
    #define RTC_DIV_RATIO_SETTING_16            4
    /*! \brief Subout freq = source clock/32 */
    #define RTC_DIV_RATIO_SETTING_32            5
    /*! \brief Subout freq = source clock/64 */
    #define RTC_DIV_RATIO_SETTING_64            6
    /*! \brief Subout freq = source clock/128 */
    #define RTC_DIV_RATIO_SETTING_128           7
    /*! \brief Subout freq = source clock/256 */
    #define RTC_DIV_RATIO_SETTING_256           8
    /*! \brief Subout freq = source clock/512 */
    #define RTC_DIV_RATIO_SETTING_512           9

    /*! \brief RTC division value check */
    #define IS_RTC_DIV_VALID(Val)            (Val <= RTC_DIV_RATIO_SETTING_512)

#else
    /*  RTC divider raido value  */
    /*! \brief Subout freq = source clock */
    #define RTC_DIV_RATIO_SETTING_1             0
    /*! \brief Subout freq = source clock/2 */
    #define RTC_DIV_RATIO_SETTING_2             1
    /*! \brief Subout freq = source clock/4 */
    #define RTC_DIV_RATIO_SETTING_4             2
    /*! \brief Subout freq = source clock/8 */
    #define RTC_DIV_RATIO_SETTING_8             3
    /*! \brief Subout freq = source clock/16 */
    #define RTC_DIV_RATIO_SETTING_16            4
    /*! \brief Subout freq = source clock/32 */
    #define RTC_DIV_RATIO_SETTING_32            5
    /*! \brief Subout freq = source clock/64 */
    #define RTC_DIV_RATIO_SETTING_64            6
    /*! \brief Subout freq = source clock/128 */
    #define RTC_DIV_RATIO_SETTING_128           7
    /*! \brief Subout freq = source clock/256 */
    #define RTC_DIV_RATIO_SETTING_256           8
    /*! \brief Subout freq = source clock/512 */
    #define RTC_DIV_RATIO_SETTING_512           9
    /*! \brief Subout freq = source clock/1024 */
    #define RTC_DIV_RATIO_SETTING_1024          10
    /*! \brief Subout freq = source clock/2048 */
    #define RTC_DIV_RATIO_SETTING_2048          11 
    /*! \brief Subout freq = source clock/4096 */
    #define RTC_DIV_RATIO_SETTING_4096          12
    /*! \brief Subout freq = source clock/8192 */
    #define RTC_DIV_RATIO_SETTING_8192          13
    /*! \brief Subout freq = source clock/16384 */
    #define RTC_DIV_RATIO_SETTING_16384         14
    /*! \brief Subout freq = source clock/32768 */
    #define RTC_DIV_RATIO_SETTING_32768         15

    /*! \brief RTC division value check */
    #define IS_RTC_DIV_VALID(Val)            (Val <= RTC_DIV_RATIO_SETTING_32768)

#endif

#if (RTC_CONFIG_TYPE == RTC_TYPE_B)
/*  RTC CO 1/2 division selection */
/*! \brief RTCCO = CO */
#define RTC_CO_SEL_2Hz                       0
/*! \brief RTCCO = CO/2 */
#define RTC_CO_SEL_1Hz                       1
#endif

/*  RTC Timer Mode  */
/*! \brief Once mode */
#define RTC_TIMER_ONCE              0 
/*! \brief Cycle mode */
#define RTC_TIMER_CYCLE             1

/* Count mode when reset time */
/*! \brief Continue after time set */
#define RTC_CNT_CONTINUE            0
/*! \brief Reset after time set */
#define RTC_CNT_RESET               1

#define RTC_CO_OUTPUT                 0
#define RTC_SUBOUT_OUTPUT             1

/* RTCCO pin output definition */
/*! \brief Output from RTCCO_0 */
#define RTC_PIN_RTCCO_0             1
/*! \brief Output from RTCCO_1 */
#define RTC_PIN_RTCCO_1             2
/*! \brief Output from RTCCO_2 */
#define RTC_PIN_RTCCO_2             3

/* SUBOUT pin output definition */
/*! \brief Output from SUBOUT_0 */
#define RTC_PIN_SUBOUT_0            1
/*! \brief Output from SUBOUT_1 */
#define RTC_PIN_SUBOUT_1            2 
/*! \brief Output from SUBOUT_2 */
#define RTC_PIN_SUBOUT_2            3

/* Alarm mode */
/*! \brief Once mode */
#define RTC_ALARM_ONCE   0
/*! \brief Cycle mode */
#define RTC_ALARM_CYCLE  1

/* RTC timer interrupt status */
/*! \brief RTC timer interrupt diable */
#define RTC_TIMER_INT_DISABLE   0
/*! \brief RTC timer interrupt enable */
#define RTC_TIMER_INT_ENABLE    1

/*---------------------------------------------------------------------------*/
/* RTC Interrupt Callback Function                                           */
/*---------------------------------------------------------------------------*/
/*! \brief RTC Alarm interrupt callback function */  
typedef void RTC_AlarmIntCallback(void); 
/*! \brief RTC Timer interrupt callback function */  
typedef void RTC_TimerIntCallback(void); 
/*! \brief RTC sub second interrupt callback function */  
typedef void RTC_SubSecIntCallback(void); 
/*! \brief RTC second interrupt callback function */  
typedef void RTC_SecIntCallback(void); 
/*! \brief RTC minute interrupt callback function */ 
typedef void RTC_MinIntCallback(void); 
/*! \brief RTC hour interrupt callback function */ 
typedef void RTC_HourIntCallback(void); 
/*!
 ******************************************************************************
 ** \brief RTC time infomation structure
 ******************************************************************************
 */
typedef struct 
{
    uint8_t Sec;    //!< second value 
    uint8_t Min;    //!< minute value 
    uint8_t Hour;   //!< hour value 
}RTC_TimeInfoT;

/*!
 ******************************************************************************
 ** \brief RTC date infomation structure
 ******************************************************************************
 */
typedef struct
{
    uint8_t Date;     //!< day value 
    uint8_t Mon;     //!< month value 
    uint8_t Year;    //!< year value 
}RTC_DateInfoT;

/*!
 ******************************************************************************
 ** \brief RTC alarm comparison flag structure
 ******************************************************************************
 */
typedef struct
{
    uint8_t Rsv1:1;    //!< Reserved bit 
    uint8_t Rsv2:1;    //!< Reserved bit 
    uint8_t Rsv3:1;    //!< Reserved bit
    uint8_t YearEn:1;  //!< Year compare enable flag 
    uint8_t MonEn:1;   //!< Month compare enable flag 
    uint8_t DateEn:1;   //!< Date compare enable flag 
    uint8_t HourEn:1;  //!< Hour compare enable flag 
    uint8_t MinEn:1;   //!< Minute compare enable flag 
}RTC_AlarmCmpFlagT;

/*!
 ******************************************************************************
 ** \brief RTC alarm time information structure
 ******************************************************************************
 */
typedef struct
{
    uint8_t Min;    //!< minute of alarm time 
    uint8_t Hour;   //!< hour of alarm time 
}RTC_AlarmTimeInfo;

/*!
 ******************************************************************************
 ** \brief RTC alarm date information structure
 ******************************************************************************
 */
typedef struct
{
    uint8_t Date;    //!< day of alarm date 
    uint8_t Mon;    //!< month of alarm date 
    uint8_t Year;   //!< year of alarm date 
}RTC_AlarmDateInfo;

/*!
 ******************************************************************************
 ** \brief RTC alarm information structure
 ******************************************************************************
 */
typedef struct
{
    RTC_AlarmTimeInfo Time;                 //!< alarm time
    RTC_AlarmDateInfo Date;                 //!< alarm date
    uint8_t AlarmMode;                      //!< alarm mode
}RTC_AlarmInfoT; 

/*!
 ******************************************************************************
 ** \brief RTC timer information structure
 ******************************************************************************
 */
typedef struct
{
    uint32_t Count;                             //!< count value of timer
    uint32_t  TimerMode;                         //!< count timer mode
}RTC_TimerInfoT;

/*!
 ******************************************************************************
 ** \brief RTC Day information structure
 ******************************************************************************
 */
typedef enum
{
    RTC_DAY_SUN = 0,
    RTC_DAY_MON,
    RTC_DAY_TUE,
    RTC_DAY_WED,
    RTC_DAY_THU,
    RTC_DAY_FRI,
    RTC_DAY_SAT,
}RTC_DayInfoT;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*   RTC Clock   */
void RTC_ClkInSel(uint8_t InClk);
void RTC_SetCaliVal(uint16_t CaliValue);
void RTC_EnableCali(void);
void RTC_DisableCali(void);
void RTC_EnableSuboutDiv(void);
void RTC_DisableSuboutDiv(void);
void RTC_SetSuboutDivVal(uint8_t Div);
#if (RTC_CONFIG_TYPE == RTC_TYPE_B)
void RTC_SetCaliCycle(uint8_t CaliCycle);
void RTC_COSel(uint8_t COSel);
#endif
/*   Calendar Module    */
void RTC_RegInit(void);
void RTC_SetCntCycle(uint32_t CntCycle);
void RTC_Stop(void);
void RTC_Start(void);
void RTC_Reset(void);
void RTC_InitTime(RTC_TimeInfoT* pTime);
void RTC_InitDate(RTC_DateInfoT* pDate);
void RTC_InitDay(RTC_DayInfoT Day);
void RTC_WriteTime(RTC_TimeInfoT* pTime,uint8_t CntRstFlag);
void RTC_WriteDate(RTC_DateInfoT* pDate,uint8_t CntRstFlag);
void RTC_WriteDay(RTC_DayInfoT Day, uint8_t CntRstFlag);
void RTC_ReadTime(RTC_TimeInfoT* pTime);
void RTC_ReadDate(RTC_DateInfoT* pDate);
void RTC_ReadDay(RTC_DayInfoT *pDay);
void RTC_EnableAlarmMinCmp(void);
void RTC_DisableAlarmMinCmp(void);
void RTC_EnableAlarmHourCmp(void);
void RTC_DisableAlarmHourCmp(void);
void RTC_EnableAlarmDateCmp(void);
void RTC_DisableAlarmDateCmp(void);
void RTC_EnableAlarmMonCmp(void);
void RTC_DisableAlarmMonCmp(void);
void RTC_EnableAlarmYearCmp(void);
void RTC_DisableAlarmYearCmp(void);
void RTC_ConfigAlarm(RTC_AlarmInfoT *pAlarmInfo);
void RTC_EnableAlarmInt(RTC_AlarmIntCallback* RTCAlarmIntCallback);
void RTC_DisableAlarmInt(void);
void RTC_EnableSubSecInt(RTC_SubSecIntCallback* RTCSubSecIntCallBack);
void RTC_DisableSubSecInt(void);
void RTC_EnableSecInt(RTC_SecIntCallback* RTCSecIntCallBack);
void RTC_DisableSecInt(void);
void RTC_EnableMinInt(RTC_MinIntCallback* RTCMinIntCallBack);
void RTC_DisableMinInt(void);
void RTC_EnableHourInt(RTC_HourIntCallback* RTCHourIntCallback);
void RTC_DisableHourInt(void);
void RTC_EnableTimer(void);
void RTC_DisableTimer(void);
void RTC_EnableTimerInt(RTC_TimerIntCallback* RTCTimerIntCallback);
void RTC_DisableTimerInt(void);
void RTC_ConfigTimer(RTC_TimerInfoT* pTimerInfo);
void RTC_ClrAllIntFlag(void);
/* Interrupt handler */
void RTC_IntHandler(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _RTC_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */

