/*!
 ******************************************************************************
 **
 ** \file mft_frt_fm3.h
 **
 ** \brief Free-run Timer Unit drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.1.0
 **
 ** \date 2012-1-9 10:50
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

#ifndef _MFT_FRT_H_
#define _MFT_FRT_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_MFT == OFF)
    #if (PERIPHERAL_ENABLE_MFT_FRT == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFT_FRT == ON) && (PERIPHERAL_AVAILABLE_MFT == ON) 
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
    #define IS_MFT_UNIT_VALID(MFTUnit)     ((MFTUnit == MFT_UNIT0) || \
                                         (MFTUnit == MFT_UNIT1) || \
                                         (MFTUnit == MFT_UNIT2) ) 
#endif

/* FRT channel amount */
/*! \brief FRT channel amount */
#define MFT_FRT_CH_AMOUNT 3
  
/* FRT channel */
/*! \brief FRT CH0 */
#define FRT_CH0 0
/*! \brief FRT CH1 */
#define FRT_CH1 1
/*! \brief FRT CH2 */
#define FRT_CH2 2
/*! \brief FRT Channel none */
#define FRT_NONE -1

/* ADC unit */
/*! \brief ADC unit0 */
#define MFT_ADC_UNIT0 0
/*! \brief ADC unit1 */
#define MFT_ADC_UNIT1 1
/*! \brief ADC unit2 */
#define MFT_ADC_UNIT2 2
/*! \brief ADC unit none */
#define MFT_ADC_NONE -1


/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief Free run timer clock division
 ******************************************************************************
 */
typedef enum 
{
    FRT_DIV1 = 0,       //!<  Peripheral clock divided by 1
    FRT_DIV2,           //!<  Peripheral clock divided by 2
    FRT_DIV4,           //!<  Peripheral clock divided by 4
    FRT_DIV8,           //!<  Peripheral clock divided by 8
    FRT_DIV16,          //!<  Peripheral clock divided by 16
    FRT_DIV32,          //!<  Peripheral clock divided by 32
    FRT_DIV64,          //!<  Peripheral clock divided by 64
    FRT_DIV128,         //!<  Peripheral clock divided by 128
    FRT_DIV256,         //!<  Peripheral clock divided by 256

} MFT_FRTClkDivT;

/*!
 ******************************************************************************
 ** \brief Free run timer source clock
 ******************************************************************************
 */
typedef enum
{
    FRT_SRC_CLK_PCLK = 0,  //!< Use peripheral clock for FRT source clock
    FRT_SRC_CLK_EXT_CLK,   //!< Use external clock for FRT source clock
}MFT_FRTSrcClkT;

/*!
 ******************************************************************************
 ** \brief Free run timer mode type definition
 ******************************************************************************
 */
typedef enum
{
    CNT_MODE_UP = 0,    //!< Free run timer Up-count mode
    CNT_MODE_UPDOWN,    //!< Free run timer Up/Down-count mode
}MFT_FRTCntModeT;

/*!
 ******************************************************************************
 ** \brief Peak match interrupt callback function type definition
 ******************************************************************************
 */
typedef void FRT_PeakMatchIntCallbackT(void);
/*!
 ******************************************************************************
 ** \brief Zero match interrupt callback function type definition
 ******************************************************************************
 */
typedef void FRT_ZeroMatchIntCallbackT(void);

/*--------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
void MFT_FRTSetClkDiv(uint8_t MFTUnit, uint8_t FRTCh, MFT_FRTClkDivT Div);
void MFT_FRTSetStateInit(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTCancelStateInit(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FrtSetCntMode(uint8_t MFTUnit, uint8_t FRTCh, MFT_FRTCntModeT Mode);
void MFT_FRTStart(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTStop(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTEnableBufFunc(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTDisableBufFunc(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTEnablePeakMatchInt(uint8_t MFTUnit, uint8_t FRTCh, 
                               FRT_PeakMatchIntCallbackT* IntCallback);
void MFT_FRTDisablePeakMatchInt(uint8_t MFTUnit, uint8_t FRTCh);
IntStatusT MFT_FRTGetPeakMatchIntFlag(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTClrPeakMatchIntFlag(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTEnableZeroMatchInt(uint8_t MFTUnit, uint8_t FRTCh, 
                               FRT_ZeroMatchIntCallbackT* IntCallback);
void MFT_FRTDisableZeroMatchInt(uint8_t MFTUnit, uint8_t FRTCh);
IntStatusT MFT_FRTGetZeroMatchIntFlag(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTClrZeroMatchIntFlag(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTSetSrcClk(uint8_t MFTUnit, uint8_t FRTCh, MFT_FRTSrcClkT SrcClk);
void MFT_FRTEnableADCTrig(uint8_t MFTUnit, uint8_t FRTCh, uint8_t ADCUnit);
void MFT_FRTDisableADCTrig(uint8_t MFTUnit, uint8_t FRTCh, uint8_t ADCUnit);
void MFT_FRTSetCycleValBuf(uint8_t MFTUnit, uint8_t FRTCh, uint16_t CntCycle);
uint16_t MFT_FRTGetCycleVal(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRTClrCurCntVal(uint8_t MFTUnit, uint8_t FRTCh);
uint16_t MFT_FRTGetCurCntVal(uint8_t MFTUnit, uint8_t FRTCh);
void MFT_FRT_IntHandler(uint8_t MFTUnit, uint16_t IrqValue);
#ifdef __cplusplus
}
#endif

#endif

#endif /* _MFT_FRT_H_ */

/*****************************************************************************/
/* END OF FILE */
