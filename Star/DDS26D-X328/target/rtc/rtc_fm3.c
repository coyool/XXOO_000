/*!
 ******************************************************************************
 **
 ** \file rtc.c
 **
 ** \brief this file provides all Real Time Clock drivers
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2012-05-22
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

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "rtc_fm3.h"

#if (PERIPHERAL_AVAILABLE_RTC == ON) && (PERIPHERAL_ENABLE_RTC == ON)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/* convert DEC to BCD */
#define DECtoBCD(x)           ((((x)/10)<<4) + ((x)%10))
/* convert BCD to DEC */
#define BCDtoDEC(x)           ((((x)>>4)*10) + ((x)&0x0F))

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*! \brief store alarm mode */  
static uint8_t s_AlarmMode;
/*! \brief store RTC alarm interrupt callback function handler */ 
static RTC_AlarmIntCallback* s_RTCAlarmIntCallback;
/*! \brief store RTC timer interrupt callback function handler */ 
static RTC_TimerIntCallback* s_RTCTimerIntCallback;
/*! \brief store RTC sub second interrupt callback function handler */
static RTC_SubSecIntCallback* s_RTCSubSecIntCallback;
/*! \brief store RTC second interrupt callback function handler */
static RTC_SecIntCallback* s_RTCSecIntCallback;
/*! \brief store RTC minute interrupt callback function handler */
static RTC_MinIntCallback* s_RTCMinIntCallback;
/*! \brief store RTC hour interrupt callback function handler */
static RTC_HourIntCallback* s_RTCHourIntCallback;


/*---------------------------------------------------------------------------*/
/*                    RTC Clock                                              */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief RTC source clock selection
 **
 ** \param InClock set the input clock
 **        This parameter can be one of the following values:
 ** \arg   RTC_CLOCK_IN_SUB_CLOCK
 ** \arg   RTC_CLOCK_IN_MAIN_CLOCK        
 **
 ** \return None
 **
 ******************************************************************************
 */
void RTC_ClkInSel(uint8_t InClk)
{
    /* Parameter check */
    ASSERT((InClk == RTC_CLOCK_IN_SUB_CLOCK) || (InClk == RTC_CLOCK_IN_MAIN_CLOCK));
    
    if(RTC_CLOCK_IN_SUB_CLOCK == InClk)
    {
        FM3_RTC->WTCLKS = 0x0;  /* select sub clock */
        while(FM3_RTC->WTCLKM & 0x03 != 0x02); 
    }
    else if(RTC_CLOCK_IN_MAIN_CLOCK == InClk)
    {
        FM3_RTC->WTCLKS = 0x1;  /* select sub clock */
        while(FM3_RTC->WTCLKM & 0x03 != 0x03); 
    }
}

/*!
 ******************************************************************************
 ** \brief Set the calibration value of RTC
 **
 ** \param CaliValue calibration value
 **
 ** \arg   0~1023
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_SetCaliVal(uint16_t CaliValue)
{
    ASSERT(IS_CALI_VAL_VALID(CaliValue));
#if (RTC_CONFIG_TYPE == RTC_TYPE_A)  
    FM3_RTC->WTCAL = (CaliValue & 0x7F);
#else
    FM3_RTC->WTCAL = (CaliValue & 0x3FF);
#endif    
}

/*!
 ******************************************************************************
 ** \brief Enable RTC calibration function
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableCali(void)
{
    FM3_RTC->WTCALEN = 0x01;
}

/*!
 ******************************************************************************
 ** \brief Disable RTC calibration function
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableCali(void)
{
    FM3_RTC->WTCALEN = 0x00;
}

/*!
 ******************************************************************************
 ** \brief Enable the divider of subout pin
 **
 ** \param None
 **  
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableSuboutDiv(void)
{
    bFM3_RTC_WTDIVEN_WTDIVEN = 1;
    while(1 != bFM3_RTC_WTDIVEN_WTDIVRDY);
}

/*!
 ******************************************************************************
 ** \brief Disable the divider of subout pin
 **
 ** \param None
 **  
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableSuboutDiv(void)
{
    bFM3_RTC_WTDIVEN_WTDIVEN = 0;
    while(0 != bFM3_RTC_WTDIVEN_WTDIVRDY);
}

/*!
 ******************************************************************************
 ** \brief Set RTC Subout Divider ratio
 **
 ** \param Div
 ** \arg   RTC_DIV_RATIO_SETTING_1
 ** \arg   RTC_DIV_RATIO_SETTING_2
 ** \arg   RTC_DIV_RATIO_SETTING_4
 ** \arg   RTC_DIV_RATIO_SETTING_8
 ** \arg   RTC_DIV_RATIO_SETTING_16
 ** \arg   RTC_DIV_RATIO_SETTING_32
 ** \arg   RTC_DIV_RATIO_SETTING_64
 ** \arg   RTC_DIV_RATIO_SETTING_128
 ** \arg   RTC_DIV_RATIO_SETTING_256
 ** \arg   RTC_DIV_RATIO_SETTING_512
 ** \arg   RTC_DIV_RATIO_SETTING_1024
 ** \arg   RTC_DIV_RATIO_SETTING_2048
 ** \arg   RTC_DIV_RATIO_SETTING_4096
 ** \arg   RTC_DIV_RATIO_SETTING_8192
 ** \arg   RTC_DIV_RATIO_SETTING_16384
 ** \arg   RTC_DIV_RATIO_SETTING_32768
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_SetSuboutDivVal(uint8_t Div)
{
    /* Parameter check */
    ASSERT(IS_RTC_DIV_VALID(Div));
    
    FM3_RTC->WTDIV &= 0xF0;
    FM3_RTC->WTDIV |= Div;   
}

#if (RTC_CONFIG_TYPE == RTC_TYPE_B)
/*!
 ******************************************************************************
 ** \brief Set RTC calibration cycle
 **
 ** \param CaliCycle Calibartion cycle
 ** \arg   1~64
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_SetCaliCycle(uint8_t CaliCycle)
{
    if((CaliCycle > 64) || (CaliCycle == 0)) return;
    FM3_RTC->WTCALPRD = CaliCycle - 1;
}

/*!
 ******************************************************************************
 ** \brief Set RTC CO output format
 **
 ** \param COSel CO output format
 ** \arg   RTC_CO_SEL_2Hz
 ** \arg   RTC_CO_SEL_1Hz
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_COSel(uint8_t COSel)
{
    /* Parameter check */
    ASSERT((COSel == RTC_CO_SEL_2Hz) || (COSel == RTC_CO_SEL_1Hz));
    
    if(RTC_CO_SEL_2Hz == COSel)
    {
        FM3_RTC->WTCOSEL = 0x00;
    }
    else
    {
        FM3_RTC->WTCOSEL = 0x01;
    }
}
#endif

/*---------------------------------------------------------------------------*/
/*                    RTC Calendar                                           */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Initial Control register of Calendar
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_RegInit(void)
{
    FM3_RTC->WTCR1 = 0;
    FM3_RTC->WTCR2 = 0;
}

/*!
 ******************************************************************************
 ** \brief Set counter cycle of RTC
 **
 ** \param CntCycle counter cycle value
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_SetCntCycle(uint32_t CntCycle)
{
    FM3_RTC->WTBR = CntCycle;
}

/*!
 ******************************************************************************
 ** \brief Stop RTC 
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_Stop(void)
{
    FM3_RTC->WTCR1_f.ST = 0;
    while(FM3_RTC->WTCR1_f.RUN); /* wait until RTC stop */
}

/*!
 ******************************************************************************
 ** \brief Start RTC 
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_Start(void)
{
     FM3_RTC->WTCR1_f.ST = 1;  /* start RTC */
     while(!FM3_RTC->WTCR1_f.RUN); /* wait until RTC start */
}

/*!
 ******************************************************************************
 ** \brief Reset RTC 
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_Reset(void)
{
    FM3_RTC->WTCR1_f.SRST = 1;
}

/*!
 ******************************************************************************
 ** \brief Write RTC time 
 **
 ** \param pTime pointer to time information structure
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_InitTime(RTC_TimeInfoT* pTime)
{
    uint8_t DecSec, DecMin, DecHour;
    /* Parameter check */
    ASSERT((pTime->Sec < 60) || (pTime->Min < 60) || (pTime->Hour < 24));
    
    
    /* write time */
    DecSec = pTime->Sec;
    DecMin = pTime->Min;
    DecHour = pTime->Hour;
    FM3_RTC->WTSR = DECtoBCD(DecSec);
    FM3_RTC->WTMIR = DECtoBCD(DecMin);
    FM3_RTC->WTHR = DECtoBCD(DecHour);
}

/*!
 ******************************************************************************
 ** \brief Write RTC date 
 **
 ** \param pDate pointer to date information structure
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_InitDate(RTC_DateInfoT* pDate)
{
    uint8_t DecDate, DecMon, DecYear;
    /* write date */
    DecDate = pDate->Date;
    DecMon = pDate->Mon;
    DecYear = pDate->Year;
    FM3_RTC->WTDR = DECtoBCD(DecDate);
    FM3_RTC->WTMOR = DECtoBCD(DecMon);
    FM3_RTC->WTYR = DECtoBCD(DecYear);
}

/*!
 ******************************************************************************
 ** \brief Initialize RTC day 
 **
 ** \param Day Day enumeration data
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_InitDay(RTC_DayInfoT Day)
{
    /* Parameter check */
    ASSERT(Day <= RTC_DAY_SAT);
    /* write Day */
    FM3_RTC->WTDW = Day;
}

/*!
 ******************************************************************************
 ** \brief Rewrite RTC time 
 **
 ** \param pTime pointer to time information structure
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_WriteTime(RTC_TimeInfoT* pTime, uint8_t CntRstFlag)
{
    uint8_t DecSec, DecMin, DecHour;
    /* Parameter check */
    ASSERT((pTime->Sec < 60) || (pTime->Min < 60) || (pTime->Hour < 24));
 
    while(FM3_RTC->WTCR1_f.BUSY); /* wait when RTC is busy */
    
    if(RTC_CNT_CONTINUE == CntRstFlag)
        FM3_RTC->WTCR1_f.SCST = 1;
    else
        FM3_RTC->WTCR1_f.SCRST = 1;
    /* write time */
    DecSec = pTime->Sec;
    DecMin = pTime->Min;
    DecHour = pTime->Hour;
    FM3_RTC->WTSR = DECtoBCD(DecSec);
    FM3_RTC->WTMIR = DECtoBCD(DecMin);
    FM3_RTC->WTHR = DECtoBCD(DecHour);
    
    if(RTC_CNT_CONTINUE == CntRstFlag)
        FM3_RTC->WTCR1_f.SCST = 0;
    else
        FM3_RTC->WTCR1_f.SCRST = 0;

}

/*!
 ******************************************************************************
 ** \brief Write RTC date 
 **
 ** \param pDate pointer to date information structure
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_WriteDate(RTC_DateInfoT* pDate, uint8_t CntRstFlag)
{
    uint8_t DecDate, DecMon, DecYear;
    while(FM3_RTC->WTCR1_f.BUSY); /* wait when RTC is busy */
    
    if(RTC_CNT_CONTINUE == CntRstFlag)
        FM3_RTC->WTCR1_f.SCST = 1;
    else
        FM3_RTC->WTCR1_f.SCRST = 1;
    /* write Date */
    DecDate = pDate->Date;
    DecMon = pDate->Mon;
    DecYear = pDate->Year;
    FM3_RTC->WTDR = DECtoBCD(DecDate);
    FM3_RTC->WTMOR = DECtoBCD(DecMon);
    FM3_RTC->WTYR = DECtoBCD(DecYear);
    
    if(RTC_CNT_CONTINUE == CntRstFlag)
        FM3_RTC->WTCR1_f.SCST = 0;
    else
        FM3_RTC->WTCR1_f.SCRST = 0;
}

/*!
 ******************************************************************************
 ** \brief Write RTC date 
 **
 ** \param pDate pointer to day information enumeration
 ** \param CntRstFlag Countinue writing flag
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_WriteDay(RTC_DayInfoT Day, uint8_t CntRstFlag)
{
    /* Parameter check */
    ASSERT(Day <= RTC_DAY_SAT);
    
    while(FM3_RTC->WTCR1_f.BUSY); /* wait when RTC is busy */
    
    if(RTC_CNT_CONTINUE == CntRstFlag)
        FM3_RTC->WTCR1_f.SCST = 1;
    else
        FM3_RTC->WTCR1_f.SCRST = 1;
    /* write Day */
    FM3_RTC->WTDW = Day;
    
    if(RTC_CNT_CONTINUE == CntRstFlag)
        FM3_RTC->WTCR1_f.SCST = 0;
    else
        FM3_RTC->WTCR1_f.SCRST = 0;
}

/*!
 ******************************************************************************
 ** \brief Write RTC date,time,day
 **
 ** \param pDate pointer to day information enumeration
 ** \param CntRstFlag Countinue writing flag
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_WriteDateTimeDay(RTC_DateInfoT* pDate,RTC_TimeInfoT* pTime,RTC_DayInfoT Day,uint8_t CntRstFlag)
{
    uint8_t DecDate, DecMon, DecYear;
    uint8_t DecSec, DecMin, DecHour;
    /* Parameter check */
    ASSERT((pTime->Sec < 60) || (pTime->Min < 60) || (pTime->Hour < 24));
     /* Parameter check */
    ASSERT(Day <= RTC_DAY_SAT);
    
    while(FM3_RTC->WTCR1_f.BUSY); /* wait when RTC is busy */
    
    if(RTC_CNT_CONTINUE == CntRstFlag)
        FM3_RTC->WTCR1_f.SCST = 1;
    else
        FM3_RTC->WTCR1_f.SCRST = 1;
    /* write Date */
    DecDate = pDate->Date;
    DecMon = pDate->Mon;
    DecYear = pDate->Year;
    FM3_RTC->WTDR = DECtoBCD(DecDate);
    FM3_RTC->WTMOR = DECtoBCD(DecMon);
    FM3_RTC->WTYR = DECtoBCD(DecYear);
    
    /* write time */
    DecSec = pTime->Sec;
    DecMin = pTime->Min;
    DecHour = pTime->Hour;
    FM3_RTC->WTSR = DECtoBCD(DecSec);
    FM3_RTC->WTMIR = DECtoBCD(DecMin);
    FM3_RTC->WTHR = DECtoBCD(DecHour);
    
    /* write Day */
    FM3_RTC->WTDW = Day;
    
    if(RTC_CNT_CONTINUE == CntRstFlag)
        FM3_RTC->WTCR1_f.SCST = 0;
    else
        FM3_RTC->WTCR1_f.SCRST = 0;
}

/*!
 ******************************************************************************
 ** \brief Read RTC time 
 **
 ** \param pTime pointer to time information structure
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_ReadTime(RTC_TimeInfoT* pTime)
{
    uint8_t BcdSec, BcdMin, BcdHour;
    while(FM3_RTC->WTCR2_f.CREAD); /* wait when RTC is read status */
    FM3_RTC->WTCR2_f.CREAD  = 1;
    while(FM3_RTC->WTCR2_f.CREAD ); /* wait when RTC is read status  */
    FM3_RTC->WTCR1_f.INTCRI = 0;   /* clear read complete flag */
    /* read time */
    BcdSec = FM3_RTC->WTSR;
    BcdMin = FM3_RTC->WTMIR;
    BcdHour = FM3_RTC->WTHR;
    pTime->Sec = BCDtoDEC(BcdSec);
    pTime->Min = BCDtoDEC(BcdMin);
    pTime->Hour = BCDtoDEC(BcdHour);
}

/*!
 ******************************************************************************
 ** \brief Read RTC date 
 **
 ** \param pTime pointer to date information structure
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_ReadDate(RTC_DateInfoT* pDate)
{
    uint8_t BcdDate, BcdMon, BcdYear;
    while(FM3_RTC->WTCR2_f.CREAD); /* wait when RTC is read status */
    FM3_RTC->WTCR2_f.CREAD  = 1;
    while(FM3_RTC->WTCR2_f.CREAD ); /* wait when RTC is read status  */
    FM3_RTC->WTCR1_f.INTCRI = 0;   /* clear read complete flag */

    /* read date */
    BcdDate = FM3_RTC->WTDR;
    BcdMon = FM3_RTC->WTMOR;
    BcdYear = FM3_RTC->WTYR;
    pDate->Date = BCDtoDEC(BcdDate);
    pDate->Mon = BCDtoDEC(BcdMon);
    pDate->Year = BCDtoDEC(BcdYear);
}



/*!
 ******************************************************************************
 ** \brief Read RTC day 
 **
 ** \param pTime pointer to day information enumeration
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_ReadDay(RTC_DayInfoT *pDay)
{
    while(FM3_RTC->WTCR2_f.CREAD); /* wait when RTC is read status */
    FM3_RTC->WTCR2_f.CREAD  = 1;
    while(FM3_RTC->WTCR2_f.CREAD ); /* wait when RTC is read status  */
    FM3_RTC->WTCR1_f.INTCRI = 0;   /* clear read complete flag */

    /* read date */
    *pDay = (RTC_DayInfoT)FM3_RTC->WTDW;
}

/*!
 ******************************************************************************
 ** \brief Read RTC date,time,day 
 **
 ** \param pTime pointer to day information enumeration
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_ReadDateTimeDay(RTC_DateInfoT* pDate,RTC_TimeInfoT* pTime,RTC_DayInfoT *pDay)
{
    uint8_t BcdSec, BcdMin, BcdHour;
    uint8_t BcdDate, BcdMon, BcdYear;
    while(FM3_RTC->WTCR2_f.CREAD); /* wait when RTC is read status */
    FM3_RTC->WTCR2_f.CREAD  = 1;
    while(FM3_RTC->WTCR2_f.CREAD ); /* wait when RTC is read status  */
    FM3_RTC->WTCR1_f.INTCRI = 0;   /* clear read complete flag */
    
    /* read date */
    BcdDate = FM3_RTC->WTDR;
    BcdMon = FM3_RTC->WTMOR;
    BcdYear = FM3_RTC->WTYR;
    pDate->Date = BCDtoDEC(BcdDate);
    pDate->Mon = BCDtoDEC(BcdMon);
    pDate->Year = BCDtoDEC(BcdYear);
    
    /* read time */
    BcdSec = FM3_RTC->WTSR;
    BcdMin = FM3_RTC->WTMIR;
    BcdHour = FM3_RTC->WTHR;
    pTime->Sec = BCDtoDEC(BcdSec);
    pTime->Min = BCDtoDEC(BcdMin);
    pTime->Hour = BCDtoDEC(BcdHour);
    
    /* read date */
    *pDay = (RTC_DayInfoT)FM3_RTC->WTDW;
}

/*!
 ******************************************************************************
 ** \brief Enable minute comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableAlarmMinCmp(void)
{
    FM3_RTC->WTCR1_f.MIEN = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable minute comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableAlarmMinCmp(void)
{
    FM3_RTC->WTCR1_f.MIEN = 0;
}

/*!
 ******************************************************************************
 ** \brief Enable hour comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableAlarmHourCmp(void)
{
    FM3_RTC->WTCR1_f.HEN = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable hour comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableAlarmHourCmp(void)
{
    FM3_RTC->WTCR1_f.HEN = 0;
}

/*!
 ******************************************************************************
 ** \brief Enable day comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableAlarmDateCmp(void)
{
    FM3_RTC->WTCR1_f.DEN = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable day comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableAlarmDateCmp(void)
{
    FM3_RTC->WTCR1_f.DEN = 0;
}

/*!
 ******************************************************************************
 ** \brief Enable month comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableAlarmMonCmp(void)
{
    FM3_RTC->WTCR1_f.MOEN = 1;
}


/*!
 ******************************************************************************
 ** \brief Disable month comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableAlarmMonCmp(void)
{
    FM3_RTC->WTCR1_f.MOEN = 0;
}

/*!
 ******************************************************************************
 ** \brief Enable year comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableAlarmYearCmp(void)
{
    FM3_RTC->WTCR1_f.YEN = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable year comparison of RTC Alarm
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableAlarmYearCmp(void)
{
    FM3_RTC->WTCR1_f.YEN = 0;
}

/*!
 ******************************************************************************
 ** \brief Configure alarm information
 **
 ** \param pAlarmInfo pointer to alarm information structure
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_ConfigAlarm(RTC_AlarmInfoT *pAlarmInfo)
{
    FM3_RTC->WTCR1_f.INTALI = 0; /* clear alarm interrupt flag */
    /* set alarm time and date */
    FM3_RTC->ALMIR = DECtoBCD(pAlarmInfo->Time.Min);
    FM3_RTC->ALHR = DECtoBCD(pAlarmInfo->Time.Hour);
    FM3_RTC->ALDR = DECtoBCD(pAlarmInfo->Date.Date);
    FM3_RTC->ALMOR = DECtoBCD(pAlarmInfo->Date.Mon);
    FM3_RTC->ALYR = DECtoBCD(pAlarmInfo->Date.Year);
    s_AlarmMode = pAlarmInfo->AlarmMode;
}

/*!
 ******************************************************************************
 ** \brief Enable alarm interrupt
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableAlarmInt(RTC_AlarmIntCallback* RTCAlarmIntCallback)
{
    s_RTCAlarmIntCallback = RTCAlarmIntCallback;
    FM3_RTC->WTCR1_f.INTALIE = 1; /* enable alarm interrupt */
}

/*!
 ******************************************************************************
 ** \brief Disable alarm interrupt
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableAlarmInt(void)
{
    FM3_RTC->WTCR1_f.INTALIE = 0; /* enable alarm interrupt */
}

/*!
 ******************************************************************************
 ** \brief Enable sub second interrupt of RTC
 **
 ** \param RTCSubSecIntCallBack pointer to sub second interrupt callback 
 **        function
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableSubSecInt(RTC_SubSecIntCallback* RTCSubSecIntCallBack)
{
    s_RTCSubSecIntCallback = RTCSubSecIntCallBack;
    FM3_RTC->WTCR1_f.INTSSIE = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable sub second interrupt of RTC
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableSubSecInt(void)
{
    FM3_RTC->WTCR1_f.INTSSIE = 0;
}

/*!
 ******************************************************************************
 ** \brief Enable second interrupt of RTC
 **
 ** \param RTCSubSecIntCallBack pointer to second interrupt callback 
 **        function
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableSecInt(RTC_SecIntCallback* RTCSecIntCallBack)
{
    s_RTCSecIntCallback = RTCSecIntCallBack;
    FM3_RTC->WTCR1_f.INTSIE = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable second interrupt of RTC
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableSecInt(void)
{
    FM3_RTC->WTCR1_f.INTSIE = 0;
}

/*!
 ******************************************************************************
 ** \brief Enable minute interrupt of RTC
 **
 ** \param RTCSubSecIntCallBack pointer to minute interrupt callback 
 **        function
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableMinInt(RTC_MinIntCallback* RTCMinIntCallBack)
{
    s_RTCMinIntCallback = RTCMinIntCallBack;
    FM3_RTC->WTCR1_f.INTMIE = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable minute interrupt of RTC
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableMinInt(void)
{
    FM3_RTC->WTCR1_f.INTMIE = 0;
}

/*!
 ******************************************************************************
 ** \brief Enable hour interrupt of RTC
 **
 ** \param RTCSubSecIntCallBack pointer to hour interrupt callback 
 **        function
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableHourInt(RTC_HourIntCallback* RTCHourIntCallback)
{
    s_RTCHourIntCallback = RTCHourIntCallback;
    FM3_RTC->WTCR1_f.INTHIE = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable hour interrupt of RTC
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableHourInt(void)
{
    FM3_RTC->WTCR1_f.INTHIE = 0;
}

/*!
 ******************************************************************************
 ** \brief Enable timer
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableTimer(void)
{
    FM3_RTC->WTCR2_f.TMST = 1;  /* start timer */
    
}

/*!
 ******************************************************************************
 ** \brief Disable timer
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableTimer(void)
{
    FM3_RTC->WTCR2_f.TMST = 0;  /* Disable timer */
    while(FM3_RTC->WTCR2_f.TMRUN); /* wait until timer is not in process */
}

/*!
 ******************************************************************************
 ** \brief Enable timer interrupt
 **
 ** \param RTCTimerIntCallback Pointer to timer interrupt callback function
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_EnableTimerInt(RTC_TimerIntCallback* RTCTimerIntCallback)
{
    s_RTCTimerIntCallback = RTCTimerIntCallback;
    FM3_RTC->WTCR1_f.INTTMI = 0;
    FM3_RTC->WTCR1_f.INTTMIE = 1;
}

/*!
 ******************************************************************************
 ** \brief Diable timer interrupt
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_DisableTimerInt(void)
{
    FM3_RTC->WTCR1_f.INTTMIE = 0;
}

/*!
 ******************************************************************************
 ** \brief Config timer
 **
 ** \param pTimerInfo pointer to timer information structure
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_ConfigTimer(RTC_TimerInfoT* pTimerInfo)
{
    /* Parameter check */
    ASSERT((pTimerInfo->TimerMode == RTC_TIMER_ONCE) || 
           (pTimerInfo->TimerMode == RTC_TIMER_CYCLE));
  
    if(pTimerInfo->TimerMode == RTC_TIMER_ONCE)
        FM3_RTC->WTCR2_f.TMEN = 0;
    else
        FM3_RTC->WTCR2_f.TMEN = 1;
    FM3_RTC->WTTR = pTimerInfo->Count; /* set the timer count */
}

/*!
 ******************************************************************************
 ** \brief Clear all RTC interrupt Flag
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_ClrAllIntFlag(void)
{
    FM3_RTC->WTCR1 &= 0xFF00FFFF;
}


/*!
 ******************************************************************************
 ** \brief RTC interrupt handler
 **
 ** \param void
 **         
 ** \return None
 **
 ******************************************************************************
 */
void RTC_IntHandler(void)
{
    RTC_TimeInfoT CurTime;
    RTC_DateInfoT CurDate;
    unsigned char alarm_invalid=0;
    if(FM3_RTC->WTCR1_f.INTALI) /* Alarm occured? */
    {
        FM3_RTC->WTCR1_f.INTALI = 0;
        RTC_ReadDate(&CurDate);
        RTC_ReadTime(&CurTime);
        if(FM3_RTC->WTCR1_f.MIEN)
        {
            if(FM3_RTC->ALMIR != DECtoBCD(CurTime.Min)) alarm_invalid = 1;
        }
        if(FM3_RTC->WTCR1_f.HEN)
        {
            if(FM3_RTC->ALHR != DECtoBCD(CurTime.Hour)) alarm_invalid = 1;
        }
        if(FM3_RTC->WTCR1_f.DEN)
        {
            if(FM3_RTC->ALDR != DECtoBCD(CurDate.Date)) alarm_invalid = 1;
        }
        if(FM3_RTC->WTCR1_f.MOEN)
        {
            if(FM3_RTC->ALMOR != DECtoBCD(CurDate.Mon)) alarm_invalid = 1;
        }
        if(FM3_RTC->WTCR1_f.YEN)
        {
            if(FM3_RTC->ALYR != DECtoBCD(CurDate.Year)) alarm_invalid = 1;
        }
        
        if(alarm_invalid == 0)
        {
            s_RTCAlarmIntCallback();
            
            if(s_AlarmMode == RTC_ALARM_ONCE)
            {
                FM3_RTC->WTCR1_f.INTALIE = 0; /* disable alarm interrupt */
                /* clear alarm register */
                FM3_RTC->ALMIR = 0;
                FM3_RTC->ALHR = 0;
                FM3_RTC->ALDR = 0;
                FM3_RTC->ALMOR = 0;
                FM3_RTC->ALYR = 0;
                FM3_RTC->WTCR1_f.MIEN = 0;
                FM3_RTC->WTCR1_f.HEN =  0;
                FM3_RTC->WTCR1_f.DEN = 0;
                FM3_RTC->WTCR1_f.MOEN = 0;
                FM3_RTC->WTCR1_f.YEN = 0;
            }
        }
    }
    if(FM3_RTC->WTCR1_f.INTTMI && FM3_RTC->WTCR1_f.INTTMIE)
    {
        FM3_RTC->WTCR1_f.INTTMI = 0;
        s_RTCTimerIntCallback();
        
    }
    if(FM3_RTC->WTCR1_f.INTSSI && FM3_RTC->WTCR1_f.INTSSIE) /* 0.5s int occur? */
    {
        FM3_RTC->WTCR1_f.INTSSI = 0;
        s_RTCSubSecIntCallback();
    }
    if(FM3_RTC->WTCR1_f.INTSI && FM3_RTC->WTCR1_f.INTSIE) /* 1s int occur? */
    {
        FM3_RTC->WTCR1_f.INTSI = 0;
        s_RTCSecIntCallback();
    }
    if(FM3_RTC->WTCR1_f.INTMI && FM3_RTC->WTCR1_f.INTMIE) /* 1min int occur? */
    {
        FM3_RTC->WTCR1_f.INTMI = 0;
        s_RTCMinIntCallback();
    }
    if(FM3_RTC->WTCR1_f.INTHI && FM3_RTC->WTCR1_f.INTHIE) /* 1hour int occur? */
    {
        FM3_RTC->WTCR1_f.INTHI = 0;
        s_RTCHourIntCallback();
    }
    /* clear all interrupt flag */
    FM3_RTC->WTCR1 &= ~(0x00FF0000);
}

#endif
