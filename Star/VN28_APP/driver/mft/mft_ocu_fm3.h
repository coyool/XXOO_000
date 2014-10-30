/*!
 ******************************************************************************
 **
 ** \file mft_ocu_fm3.h
 **
 ** \brief Output Compare Unit drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.1.0
 **
 ** \date 2012-1-11 10:15
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
#ifndef _MFT_OCU_H_
#define _MFT_OCU_H_
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_MFT == OFF)
    #if (PERIPHERAL_ENABLE_MFT_ICU == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFT_OCU == ON) && (PERIPHERAL_AVAILABLE_MFT == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief Chip has how many MFT units */
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
    #define IS_MFT_UNIT_VALID(MFTUnit)  ((MFTUnit == MFT_UNIT0) || \
                                         (MFTUnit == MFT_UNIT1) || \
                                         (MFTUnit == MFT_UNIT2) ) 
#endif

/*! \brief 1 unit MFT has how many OCU */
#define OCU_AMOUNT    6
/* OCCP channel */
/*! \brief OCCP CH0 */
#define OCU_CH0 0
/*! \brief OCCP CH1 */
#define OCU_CH1 1
/*! \brief OCCP CH2 */
#define OCU_CH2 2
/*! \brief OCCP CH3 */
#define OCU_CH3 3
/*! \brief OCCP CH4 */
#define OCU_CH4 4
/*! \brief OCCP CH5 */
#define OCU_CH5 5
/*! \brief OCCP Channel none */
#define OCU_NONE -1

/* FRT channel */
/*! \brief FRT CH0 */
#define FRT_CH0 0
/*! \brief FRT CH1 */
#define FRT_CH1 1
/*! \brief FRT CH2 */
#define FRT_CH2 2
/*! \brief FRT Channel none */
#define FRT_NONE -1

/*! \brief OCU couple ch01 */
#define OCU_COUPLE_CH10       0
/*! \brief OCU couple ch23 */
#define OCU_COUPLE_CH32       2
/*! \brief OCU couple ch45 */
#define OCU_COUPLE_CH54       4

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief RT pin level type definition
 ******************************************************************************
 */
typedef enum 
{
    OCU_RT_LEVEL_LOW = 0,
    OCU_RT_LEVEL_HIGH,
}OCU_RTLevelT;

/*!
 ******************************************************************************
 ** \brief OCU buffer transfer mode type definition
 ******************************************************************************
 */
typedef enum
{
    OCU_BUF_TRSF_ZERO_MATCH = 0,
    OCU_BUF_TRSF_PEAK_MATCH,
    
}OCU_BufTrsfModeT;

/*!
 ******************************************************************************
 ** \brief OCU operation mode type definition
 ******************************************************************************
 */
typedef enum
{
    OddCh_1Change_EvenCh_1Change = 0, 
    OddCh_2Change_EvenCh_1Change, 
    OddCh_1Change_EvenCh_ActHigh, 
    OddCh_ActHigh_EvenCh_1Change,
    OddCh_ActLow_EvenCh_1Change,
    OddCh_ActHigh_EvenCh_ActHigh,
    OddCh_ActLow_EvenCh_ActLow,
    
}OCU_OptModeT;

/*!
 ******************************************************************************
 ** \brief OCU interrupt callback function
 ******************************************************************************
 */
typedef void OCU_IntCallbackT (void);

/*---------------------------------------------------------------------------*/
/* declare functions                                                         */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
void MFT_OCUSelFRTCh(uint8_t MFTUnit, uint8_t OCUCh, uint8_t FRTCh);
void MFT_OCUStart(uint8_t MFTUnit, uint8_t OCUCh);  
void MFT_OCUStop(uint8_t MFTUnit, uint8_t OCUCh);  
void MFT_OCUEnableBufFunc(uint8_t MFTUnit, uint8_t OCUCh); 
void MFT_OCUDisableBufFunc(uint8_t MFTUnit, uint8_t OCUCh); 
void MFT_OCUEnableInt(uint8_t MFTUnit, uint8_t OCUCh, OCU_IntCallbackT *IntCallback); 
void MFT_OCUDisableInt(uint8_t MFTUnit, uint8_t OCUCh); 
IntStatusT MFT_OCUGetIntFlag(uint8_t MFTUnit, uint8_t OCUCh);
void MFT_OCUSetRTxPinLevel(uint8_t MFTUnit, uint8_t OCUCh, OCU_RTLevelT Level); 
OCU_RTLevelT MFT_OCUGetRTxPinLevel(uint8_t MFTUnit, uint8_t OCUCh);
void MFT_OCUSetBufTrsfMode(uint8_t MFTUnit, uint8_t OCUCh, OCU_BufTrsfModeT Mode);
void MFT_OCUSetOptMode(uint8_t MFTUnit, uint8_t OCUCh, OCU_OptModeT Mode);    
void MFT_OCUSetCmpValBuf(uint8_t MFTUnit, uint8_t OCUCh, uint16_t CmpVal);
uint16_t MFT_OCUGetCmpVal(uint8_t MFTUnit, uint8_t OCUCh);
void MFT_OCU_IntHandler(uint8_t MFTUnit, uint16_t IrqValue);
#ifdef __cplusplus
}
#endif

#endif

#endif /* DRV_MFT_OCU_H */

/*****************************************************************************/
/* END OF FILE */
