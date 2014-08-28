/*!
 ******************************************************************************
 **
 ** \file mft_adcmp_fm3.h
 **
 ** \brief  ADC Start Compare Unit drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.1.0
 **
 ** \date 2012-08-21
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

#ifndef DRV_MFT_ADCMP_H
#define DRV_MFT_ADCMP_H

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_MFT == OFF)
    #if (PERIPHERAL_ENABLE_MFT_ADCMP == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFT_ADCMP == ON) && (PERIPHERAL_AVAILABLE_MFT == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9) || \
    (MCU_TYPE == DEVICE_TYPE5)
    #define MFT_AMOUNT 1
    
#elif (MCU_TYPE == DEVICE_TYPE1)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_L)
        #define MFT_AMOUNT 1
    #else
        #define MFT_AMOUNT 2                   
    #endif
#elif (MCU_TYPE == DEVICE_TYPE0)
    #define MFT_AMOUNT 2
    
#else 
    #define MFT_AMOUNT 3
#endif

#if (MFT_AMOUNT == 1)
    /*! \brief MFT unit0 */
    #define MFT_UNIT0       0
    #define IS_MFT_UNIT_VALID(MFTUnit)     (MFTUnit == MFT_UNIT0) 
#elif (MFT_AMOUNT == 2)
    /*! \brief MFT unit0 */
    #define MFT_UNIT0       0
    /*! \brief MFT unit1 */
    #define MFT_UNIT1       1
    #define IS_MFT_UNIT_VALID(MFTUnit)     ((MFTUnit == MFT_UNIT0) || (MFTUnit == MFT_UNIT1))
#else
    /*! \brief MFT unit0 */
    #define MFT_UNIT0       0
    /*! \brief MFT unit1 */
    #define MFT_UNIT1       1
    /*! \brief MFT unit0 */
    #define MFT_UNIT2       2
    #define IS_MFT_UNIT_VALID(Unit)     ((MFTUnit == MFT_UNIT0) || \
                                         (MFTUnit == MFT_UNIT1) || \
                                         (MFTUnit == MFT_UNIT2) ) 
#endif

/* ADC compare channel */
/*! \brief ADCMP CH0 */
#define ADCMP_CH0 0
/*! \brief ADCMP CH1 */
#define ADCMP_CH1 1
/*! \brief ADCMP CH2 */
#define ADCMP_CH2 2
/*! \brief ADCMP Channel none */
#define ADCMP_NONE -1

/* FRT channel */
/*! \brief FRT Channel none */
#define ADCMP_FRT_NONE   0
/*! \brief FRT CH0 */
#define ADCMP_FRT_CH0    1
/*! \brief FRT CH1 */
#define ADCMP_FRT_CH1    2
/*! \brief FRT CH2 */
#define ADCMP_FRT_CH2    3

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief ADC compare unit compare mode
 ******************************************************************************
 */
typedef enum
{
    ADCMP_UPPEAKDOWN_ACCP = 0,           //!<  Compared with ACCP in up/peak/down mode
    ADCMP_UP_ACCP,                       //!<  Compared with ACCP in up mode
    ADCMP_DOWN_ACCP,                     //!<  Compared with ACCP in down mode
    ADCMP_UP_ACCP_PEAKDOWN_ACCPDN,       //!<  Compared with ACCP in up/peak mode, ACCPDN in down mode
  
}ADCMP_CmpModeT;

/*!
 ******************************************************************************
 ** \brief ADC compare unit buffer transfer mode
 ******************************************************************************
 */
typedef enum
{
    ADCMP_BUF_TRSF_ZERO_MATCH = 0, //!<  buffer transfer when zero matchs
    ADCMP_BUF_TRSF_PEAK_MATCH,     //!<  buffer transfer when peak matchs
  
}ADCMP_BufTrsfModeT;

/*!
 ******************************************************************************
 ** \brief ADC compare unit scan start trigger mode
 ******************************************************************************
 */
typedef enum
{
    ADCMP_SCAN_TRIG_ADCMP = 0,    //!<  scan triggered by ADCMP
    ADCMP_SCAN_TRIG_OR_FTR012,    //!<  scan triggered by FRT
    
}ADCMP_ScanTrigModeT;

/*!
 ******************************************************************************
 ** \brief ADC compare unit trigger source selecetion
 ******************************************************************************
 */
typedef enum
{
    ADCMP_PRIO_TRIG_ADCMP = 0,  //!<  priority triggered by ADCMP
    ADCMP_PRIO_TRIG_OR_FTR012,   //!<  priority triggered by FRT
    
}ADCMP_PrioTrigModeT;

/*---------------------------------------------------------------------------*/
/* declare functions                                                         */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
void MFT_ADCMPSelFRTCh(uint8_t MFTUnit, uint8_t ADCMPCh, uint8_t FRTCh);
void MFT_ADCMPSetCmpMode(uint8_t MFTUnit, uint8_t ADCMPCh, ADCMP_CmpModeT Mode);
void MFT_ADCMPEnableBufFunc(uint8_t MFTUnit, uint8_t ADCMPCh);
void MFT_ADCMPDisableBufFunc(uint8_t MFTUnit, uint8_t ADCMPCh);
void MFT_ADCMPSetBufTrsfMode(uint8_t MFTUnit, uint8_t ADCMPCh, ADCMP_BufTrsfModeT Mode);
void MFT_ADCMPSetACCPBuf(uint8_t MFTUnit, uint8_t ADCMPCh, uint16_t Val);
uint16_t  MFT_ADCMPGetACCPVal(uint8_t MFTUnit, uint8_t ADCMPCh);
void MFT_ADCMPSetACCPDNBuf(uint8_t MFTUnit, uint8_t ADCMPCh, uint16_t Val);
uint16_t  MFT_ADCMPGetACCPDNVal(uint8_t MFTUnit, uint8_t ADCMPCh);
void MFT_ADCMPSelADCScanTrig(uint8_t MFTUnit, uint8_t ADCMPCh, ADCMP_ScanTrigModeT Mode);
void MFT_ADCMPSelADCPrioTrig(uint8_t MFTUnit, uint8_t ADCMPCh, ADCMP_PrioTrigModeT Mode);

#ifdef __cplusplus
}
#endif

#endif

#endif /* DRV_MFT_FRT_H */

/*****************************************************************************/
/* END OF FILE */
