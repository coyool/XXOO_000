/*!
 ******************************************************************************
 **
 ** \file dt_fm3.h
 **
 ** \brief Dual Timer driver head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-08
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
 
#ifndef _DT_FM3_H_
#define _DT_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_DT == OFF)
    #if (PERIPHERAL_ENABLE_DT == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_AVAILABLE_DT ==ON) && (PERIPHERAL_ENABLE_DT == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief Dual timer Ch 1*/
#define DT_CH_1 1
/*! \brief Dual timer Ch 2*/
#define DT_CH_2 2

/*! \brief Dual timer running mode: free run */
#define DT_MODE_FREERUN 0
/*! \brief Dual timer running mode: periodic */
#define DT_MODE_PERIOD 1
/*! \brief Dual timer running mode: one shot */
#define DT_MODE_ONESHOT 2

/*! \brief Dual timer factor of frequency division for dual timer: 1 */
#define DT_PRE_BY_1 0
/*! \brief Dual timer factor of frequency division for dual timer: 16 */
#define DT_PRE_BY_16 1
/*! \brief Dual timer factor of frequency division for dual timer: 256 */
#define DT_PRE_BY_256 2

/*! \brief Dual timer count size: 16 bit */
#define DT_CNT_SIZE_16 0
/*! \brief Dual timer count size: 32 bit */
#define DT_CNT_SIZE_32 1

/*! \brief Control Register: Enable bit */
#define DT_CTL_TIMEREN  0x80
/*! \brief Control Register: Mode bit */
#define DT_CTL_TIMERMODE  0x40
/*! \brief Control Register: Interrupt enable bit */
#define DT_CTL_TIMERINT  0x20
/*! \brief Control Register: Prescale bit */
#define DT_CTL_TIMERPRE16 0x04
/*! \brief Control Register: Prescale bit */
#define DT_CTL_TIMERPRE256 0x08
/*! \brief Control Register: Counter size bit */
#define DT_CTL_TIMERSIZE 0x02
/*! \brief Control Register: One-shot mode bit */
#define DT_CTL_ONESHOT 0x01
/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
 /*!
 ******************************************************************************
 ** \brief Dual timer base register structure.
 **
 ** Inculdes: registers info
 **
 ******************************************************************************
 */
typedef struct
{
    uint32_t Timerload;             //!< Load register
    uint32_t Timervalue;            //!< Value register
    uint32_t Timercontrol;          //!< Control register
    uint32_t Timerintclr;           //!< Int control register
    uint32_t TimerRIS;              //!< RIS register
    uint32_t TimerMIS;              //!< MIS register
    uint32_t TimerBGload;           //!< BG load register
    uint8_t RESERVED0[4];           //!< Reserved
}DT_RegInfoT;

/*!
 ******************************************************************************
 ** \brief DT callback.
 ******************************************************************************
 */
typedef void (*DT_CallbackT) (void);
 /*!
 ******************************************************************************
 ** \brief Dual timer configuration structure.
 **
 ** Inculdes: config info
 **
 ******************************************************************************
 */
typedef struct
{
    uint8_t TimerMode;          //!< Timer mode
    uint8_t TimerPre;           //!< Timer prescalter
    uint8_t TimerSize;          //!< Timer counter size
}DT_ConfigT;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* Config */
void DT_ConfigEnableTimer (uint8_t Ch);
void DT_ConfigDisableTimer (uint8_t Ch);
void DT_ConfigSetLoadValue (uint8_t Ch, uint32_t value);
void DT_ConfigSetBackLoadValue (uint8_t Ch, uint32_t value);
void DT_ConfigSetTimerSize (uint8_t Ch, uint8_t size);
void DT_ConfigSetTimerMode (uint8_t Ch, uint8_t mode);
void DT_ConfigSetTimerPre (uint8_t Ch, uint8_t prescaler);
void DT_ConfigMode (uint8_t Ch, DT_ConfigT *pdt_configT);
void DT_ConfigmsMode (uint8_t Ch, DT_ConfigT *ptdt_config);
/* Int */
void DT_IntClrInt (uint8_t Ch);
void DT_IntEnableInt (uint8_t Ch, void(*TXFunc)());
void DT_IntDisableInt (uint8_t Ch);
void DT_IntHandler(uint8_t Ch);
/* Status */
uint32_t DT_StatusReadValue (uint8_t Ch);
uint32_t DT_StatusReadRIS (uint8_t Ch);
uint8_t DT_StatusReadTimerMode (uint8_t Ch);
uint8_t DT_StatusReadTimerPrescaler (uint8_t Ch);
uint8_t DT_StatusReadTimerSize (uint8_t Ch);
#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_AVAILABLE_DT */

#endif /* _DT_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
