/*!
 ******************************************************************************
 **
 ** \file mft_wfg_fm3.h
 **
 ** \brief Waveform Generator Unit drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.21
 **
 ** \date 2012-08-20
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

#ifndef _MFT_WFG_H_
#define _MFT_WFG_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_MFT == OFF)
    #if (PERIPHERAL_ENABLE_MFT_WFG == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFT_WFG == ON) && (PERIPHERAL_AVAILABLE_MFT == ON)
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
    #define IS_MFT_UNIT_VALID(MFTUnit)   ((MFTUnit == MFT_UNIT0) || \
                                          (MFTUnit == MFT_UNIT1) || \
                                          (MFTUnit == MFT_UNIT2) ) 
#endif

/* WFG channel amount */
/*! \brief WFG channel amount */
#define WFG_CH_GROUP_AMOUNT 3

/* WFG channel pairs */
/*! \brief WFG CH10 */
#define WFG_CH10 0
/*! \brief WFG CH32 */
#define WFG_CH32 2
/*! \brief WFG CH54 */
#define WFG_CH54 4
/*! \brief WFG Channel none */
#define WFG_NONE -1

/* FRT channel */
/*! \brief FRT CH0 */
#define FRT_CH0 0
/*! \brief FRT CH1 */
#define FRT_CH1 1
/*! \brief FRT CH2 */
#define FRT_CH2 2
/*! \brief FRT Channel none */
#define FRT_NONE -1

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief WFG clock division
 ******************************************************************************
 */
typedef enum
{
    WFG_DIV1 = 0,     //!< WFG Division 1 
    WFG_DIV2,         //!< WFG Division 2 
    WFG_DIV4,         //!< WFG Division 4 
    WFG_DIV8,         //!< WFG Division 8 
    WFG_DIV16,        //!< WFG Division 16 
    WFG_DIV32,        //!< WFG Division 32 
    WFG_DIV64,        //!< WFG Division 64 
    
}WFG_ClockDivT;

/*!
 ******************************************************************************
 ** \brief WFG operation
 ******************************************************************************
 */
typedef enum
{
    WFG_MODE_THROUGH = 0,           //!< WFG through mode
    WFG_MODE_RT_PPG = 1,            //!< WFG RT-PPG mode
    WFG_MODE_TIMER_PPG = 2,         //!< WFG Timer-PPG mode
    WFG_MODE_RT_DEAD_TIMER = 4,     //!< WFG RT Dead Timer mode
    WFG_MODE_PPG_DEAD_TIMER = 7,    //!< WFG PPG Dead Timer mode
    
}WFG_OptModeT;

/*!
 ******************************************************************************
 ** \brief WFG CTEN bits setting
 ******************************************************************************
 */
typedef enum
{
    WFG_GTEN_BITS_00B = 0,         //!<  Set GTEN = 00b
    WFG_GTEN_BITS_01B,             //!<  Set GTEN = 01b
    WFG_GTEN_BITS_10B,             //!<  Set GTEN = 10b
    WFG_GTEN_BITS_11B,             //!<  Set GTEN = 11b
  
}WFG_GTENBitsT;

/*!
 ******************************************************************************
 ** \brief WFG PPG selection
 ******************************************************************************
 */
typedef enum
{
    WFG_PPG_SEL_PPG0 = 0,           //!<  Select PPG0
    WFG_PPG_SEL_PPG2,               //!<  Select PPG2
    WFG_PPG_SEL_PPG4,               //!<  Select PPG4
}WFG_PPGSelT;

/*!
 ******************************************************************************
 ** \brief WFG PGEN bits setting
 ******************************************************************************
 */
typedef enum
{
    WFG_PGEN_BITS_00B = 0,          //!<  Set PGEN = 00b
    WFG_PGEN_BITS_01B,              //!<  Set PGEN = 01b
    WFG_PGEN_BITS_10B,              //!<  Set PGEN = 10b
    WFG_PGEN_BITS_11B,              //!<  Set PGEN = 01b
  
}WFG_PGENBitsT;

/*!
 ******************************************************************************
 ** \brief WFG DMOD bit setting
 ******************************************************************************
 */
typedef enum
{
    WFG_DMOD_BIT_0B = 0,        //!<  Set DMOD = 0b
    WFG_DMOD_BIT_1B,            //!<  Set DMOD = 1b
  
}WFG_DMODBitT;

/*!
 ******************************************************************************
 ** \brief WFG noise canceller width setting
 ******************************************************************************
 */
typedef enum
{
    NZCL_NOISE_CANCEL_NULL = 0,     //!< No noise canceller
    NZCL_NOISE_CANCEL_4PCLK,        //!< Noise canceller with 4 PCLK
    NZCL_NOISE_CANCEL_8PCLK,        //!< Noise canceller with 8 PCLK
    NZCL_NOISE_CANCEL_16PCLK,       //!< Noise canceller with 16 PCLK
    NZCL_NOISE_CANCEL_32PCLK,       //!< Noise canceller with 32 PCLK
}NZCL_NoiseCanceT;


/*!
 ******************************************************************************
 ** \brief DTIF trigger mode
 ******************************************************************************
 */
typedef enum
{
    DTIF_INT_DTTIxX_PIN_TRIG = 0,  //!< DTIF interrupt trigger by DTTIxX pin
    DTIF_INT_SW_TRIG,              //!< DTIF interrupt trigger by software 
    DTIF_INT_BOTH_TRIG,            //!< DTIF interrupt trigger by both DTTIxX pin and software
} NZCL_DTIFIntTrigT;

/*!
 ******************************************************************************
 ** \brief WFG DTTI interrupt callback function
 ******************************************************************************
 */
typedef void MFT_DTTIIntCallbackT(void);

/*!
 ******************************************************************************
 ** \brief WFG interrupt callback function
 ******************************************************************************
 */
typedef void MFT_WFGIntCallbackT(void);

/*---------------------------------------------------------------------------*/
/* declare functions                                                         */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* WFG */
void MFT_WFGSetClockDiv(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_ClockDivT Div);
void MFT_WFGSetOptMode(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_OptModeT Mode);
void MFT_WFGSetGTENBits(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_GTENBitsT Bits);
void MFT_WFGSelPPG(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_PPGSelT PPGSel);
void MFT_WFGSetPGENBits(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_PGENBitsT Bits);
void MFT_WFGSetDMODBit(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_DMODBitT Bit);
void MFT_WFGSetCntVal(uint8_t MFTUnit, uint8_t WFGCoupleCh, uint16_t CntVal);
void MFT_WFGEnableInt(uint8_t MFTUnit, uint8_t WFGCoupleCh, MFT_WFGIntCallbackT* IntCallback);
void MFT_WFGDisableInt(uint8_t MFTUnit, uint8_t WFGCoupleCh);
FlagStatusT MFT_WFGGetOptStatus(uint8_t MFTUnit, uint8_t WFGCoupleCh);
IntStatusT MFT_WFGGetIntFlag(uint8_t MFTUnit, uint8_t WFGCoupleCh);
void MFT_WFGClrIntFlag(uint8_t MFTUnit, uint8_t WFGCoupleCh);
void MFT_WFG_IntHandler(uint8_t MFTUnit, uint16_t IrqValue);
/* NZCL */
void MFT_NZCLEnableDTIFInt(uint8_t MFTUnit, NZCL_DTIFIntTrigT TrigMode, 
                           MFT_DTTIIntCallbackT* IntCallback);
void MFT_NZCLDisableDTIFInt(uint8_t MFTUnit, NZCL_DTIFIntTrigT TrigMode);
void MFT_NZCLSetNoiseCancelWidth(uint8_t MFTUnit,NZCL_NoiseCanceT Width);
void MFT_NZCLForceGen(uint8_t MFTUni);
IntStatusT MFT_NZCLGetIntFlag(uint8_t MFTUnit);
void MFT_NZCLClrIntFlag(uint8_t MFTUnit);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _MFT_WFG_H_ */

/*****************************************************************************/
/* END OF FILE */
