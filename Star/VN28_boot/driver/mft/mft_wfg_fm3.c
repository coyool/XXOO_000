/*!
 ******************************************************************************
 **
 ** \file mft_wfg_fm3.c
 **
 ** \brief Waveform Generator Unit all cell drivers file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.21
 **
 ** \date 2012-8-20
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
#include "mft_wfg_fm3.h"

#if (PERIPHERAL_ENABLE_MFT_WFG == ON) && (PERIPHERAL_AVAILABLE_MFT == ON)
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*! \brief WFG register structure array */
static FM3_MFT_WFG_TypeDef* pFM3_MFT_WFG_BASE[MFT_AMOUNT] =
{
    (FM3_MFT_WFG_TypeDef*)(FM3_MFT0_WFG_BASE),
#if (MFT_AMOUNT > 1)    
    (FM3_MFT_WFG_TypeDef*)(FM3_MFT1_WFG_BASE),
#endif
#if (MFT_AMOUNT > 2)    
    (FM3_MFT_WFG_TypeDef*)(FM3_MFT2_WFG_BASE),
#endif
};
/*! \brief DTTI interrupt callback function */
static MFT_DTTIIntCallbackT *DTTIIntCallback[MFT_AMOUNT]= {(void*)0};
/*! \brief WFG interrupt callback function */
static MFT_WFGIntCallbackT *WFGIntCallback[MFT_AMOUNT][WFG_CH_GROUP_AMOUNT] = {(void*)0};

/*---------------------------------------------------------------------------*/
/* local definition                                                          */
/*---------------------------------------------------------------------------*/
#define GET_BASE_WFSA_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_WFG_BASE[MFTUnit]->WFSA10
#define GET_BASE_WFTM_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_WFG_BASE[MFTUnit]->WFTM10
#define GET_BASE_NZCL_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_WFG_BASE[MFTUnit]->NZCL
#define GET_BASE_WFIR_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_WFG_BASE[MFTUnit]->WFIR

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief  Set clcok division of Waveform Generator
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  Div Clcok division
 ** \arg    WFG_DIV1
 ** \arg    WFG_DIV2
 ** \arg    WFG_DIV4
 ** \arg    WFG_DIV8
 ** \arg    WFG_DIV16
 ** \arg    WFG_DIV32
 ** \arg    WFG_DIV64
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGSetClockDiv(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_ClockDivT Div)
{
    volatile uint16_t* wfsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    ASSERT((WFG_DIV1 == Div) || (WFG_DIV2 == Div) || (WFG_DIV4 == Div)
           || (WFG_DIV8 == Div) || (WFG_DIV16 == Div) || (WFG_DIV32 == Div)
           || (WFG_DIV64 == Div));
    
    wfsa = GET_BASE_WFSA_ADDR(MFTUnit) + WFGCoupleCh;
    
    *wfsa &= ~7ul;
    *wfsa |= Div;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set clcok division of Waveform Generator
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  Mode WFG operation mode
 ** \arg    WFG_MODE_THROUGH
 ** \arg    WFG_MODE_RT_PPG
 ** \arg    WFG_MODE_TIMER_PPG
 ** \arg    WFG_MODE_RT_DEAD_TIMER
 ** \arg    WFG_MODE_PPG_DEAD_TIMER
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGSetOptMode(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_OptModeT Mode)
{
    volatile uint16_t* wfsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    ASSERT((WFG_MODE_THROUGH == Mode) || (WFG_MODE_RT_PPG == Mode) || (WFG_MODE_TIMER_PPG == Mode)
           || (WFG_MODE_RT_DEAD_TIMER == Mode) || (WFG_MODE_PPG_DEAD_TIMER == Mode));
    
    wfsa = GET_BASE_WFSA_ADDR(MFTUnit) + WFGCoupleCh;
    
    *wfsa &= ~(7ul<<3);
    *wfsa |= (Mode<<3);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set GTEN bits of Waveform Generator
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  Bits Bits value
 ** \arg    WFG_GTEN_BITS_00B
 ** \arg    WFG_GTEN_BITS_01B
 ** \arg    WFG_GTEN_BITS_10B
 ** \arg    WFG_GTEN_BITS_11B
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGSetGTENBits(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_GTENBitsT Bits)
{
    volatile uint16_t* wfsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    ASSERT((WFG_GTEN_BITS_00B == Bits) || (WFG_GTEN_BITS_01B == Bits) 
           || (WFG_GTEN_BITS_10B == Bits) || (WFG_GTEN_BITS_11B == Bits));
    
    wfsa = GET_BASE_WFSA_ADDR(MFTUnit) + WFGCoupleCh;
    *wfsa &= ~(3ul<<6);
    *wfsa |= (Bits<<6);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Select PPG timer of Waveform Generator
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  PPGSel PPG selection
 ** \arg    WFG_PPG_SEL_PPG0
 ** \arg    WFG_PPG_SEL_PPG2
 ** \arg    WFG_PPG_SEL_PPG4
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGSelPPG(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_PPGSelT PPGSel)
{
    volatile uint16_t* wfsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    ASSERT((WFG_PPG_SEL_PPG0 == PPGSel) || (WFG_PPG_SEL_PPG2 == PPGSel) || (WFG_PPG_SEL_PPG4 == PPGSel));
    
    wfsa = GET_BASE_WFSA_ADDR(MFTUnit) + WFGCoupleCh;
    *wfsa &= ~(3ul<<8);
    *wfsa |= (PPGSel<<8);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set PGEN bits of Waveform Generator
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  Bits Bits value
 ** \arg    WFG_PGEN_BITS_00B
 ** \arg    WFG_PGEN_BITS_01B
 ** \arg    WFG_PGEN_BITS_10B
 ** \arg    WFG_PGEN_BITS_11B
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGSetPGENBits(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_PGENBitsT Bits)
{
    volatile uint16_t* wfsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    ASSERT((WFG_PGEN_BITS_00B == Bits) || (WFG_PGEN_BITS_01B == Bits) 
            || (WFG_PGEN_BITS_10B == Bits) || (WFG_PGEN_BITS_11B == Bits));
    
    wfsa = GET_BASE_WFSA_ADDR(MFTUnit) + WFGCoupleCh;
    *wfsa &= ~(3ul<<10);
    *wfsa |= (Bits<<10);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set DMOD bit of Waveform Generator
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  Bit Bit value
 ** \arg    WFG_DMOD_BIT_0B
 ** \arg    WFG_DMOD_BIT_1B
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGSetDMODBit(uint8_t MFTUnit, uint8_t WFGCoupleCh, WFG_DMODBitT Bit)
{
    volatile uint16_t* wfsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    ASSERT((WFG_DMOD_BIT_0B == Bit) || (WFG_DMOD_BIT_1B == Bit));
    
    wfsa = GET_BASE_WFSA_ADDR(MFTUnit) + WFGCoupleCh;
    *wfsa &= ~(1ul<<12);
    *wfsa |= (Bit<<10);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set DMOD bit of Waveform Generator
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  CntVal Count value
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGSetCntVal(uint8_t MFTUnit, uint8_t WFGCoupleCh, uint16_t CntVal)
{
    volatile uint16_t* wftm;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    
    wftm = GET_BASE_WFTM_ADDR(MFTUnit) + WFGCoupleCh;
    *wftm = CntVal;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Start WFG and enable WFG interrupt
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  IntCallback Interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGEnableInt(uint8_t MFTUnit, uint8_t WFGCoupleCh, MFT_WFGIntCallbackT* IntCallback)
{
     volatile uint16_t* wfir;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    
    WFGIntCallback[MFTUnit][WFGCoupleCh/2] = IntCallback;
    wfir = GET_BASE_WFIR_ADDR(MFTUnit);
    
    *wfir |= (1ul<<(6+4*(WFGCoupleCh/2)));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Stop WFG and disable WFG interrupt
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGDisableInt(uint8_t MFTUnit, uint8_t WFGCoupleCh)
{
     volatile uint16_t* wfir;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    
    wfir = GET_BASE_WFIR_ADDR(MFTUnit);
    
    *wfir |= (1ul<<(7+4*(WFGCoupleCh/2)));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get the operation status of WFG
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 **
 ** \return Operation status
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
 */
FlagStatusT MFT_WFGGetOptStatus(uint8_t MFTUnit, uint8_t WFGCoupleCh)
{
    volatile uint16_t* wfir; 
    uint8_t tempbit = 0;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    
    wfir = GET_BASE_WFIR_ADDR(MFTUnit);
    tempbit = *wfir &(1ul<<(6+4*(WFGCoupleCh/2)));
    tempbit = tempbit >> (6+4*(WFGCoupleCh/2));
    return (FlagStatusT)tempbit;
}

/*!
 ******************************************************************************
 ** \brief  Get the interrupt flag of WFG
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 **
 ** \return Interrupt flag
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
 */
IntStatusT MFT_WFGGetIntFlag(uint8_t MFTUnit, uint8_t WFGCoupleCh)
{
    volatile uint16_t* wfir; 
    uint8_t tempbit = 0;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    
    wfir = GET_BASE_WFIR_ADDR(MFTUnit);
    tempbit = *wfir &(1ul<<(4+4*(WFGCoupleCh/2)));
    tempbit = tempbit >> (4+4*(WFGCoupleCh/2));
    return (IntStatusT)tempbit;
}

/*!
 ******************************************************************************
 ** \brief  Clear the interrupt flag of WFG
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_WFGClrIntFlag(uint8_t MFTUnit, uint8_t WFGCoupleCh)
{
     volatile uint16_t* wfir;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((WFG_CH10 == WFGCoupleCh) || (WFG_CH32 == WFGCoupleCh) || (WFG_CH54 == WFGCoupleCh));
    
    wfir = GET_BASE_WFIR_ADDR(MFTUnit);
    
    *wfir &= ~(1ul<<(5+4*(WFGCoupleCh/2)));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable DTIF interrupt of NZCL
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 ** \param  IntCallback Interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_NZCLEnableDTIFInt(uint8_t MFTUnit, NZCL_DTIFIntTrigT TrigMode, 
                           MFT_DTTIIntCallbackT* IntCallback)
{
    volatile uint16_t* nzcl;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT(IntCallback != NULL);
    ASSERT((TrigMode == DTIF_INT_DTTIxX_PIN_TRIG)
           || (TrigMode == DTIF_INT_SW_TRIG)
           || (TrigMode == DTIF_INT_BOTH_TRIG) );
    
    nzcl = GET_BASE_NZCL_ADDR(MFTUnit);
    
    DTTIIntCallback[MFTUnit] = IntCallback;
    if(TrigMode == DTIF_INT_DTTIxX_PIN_TRIG)
    {
        *nzcl |= 1ul;
    }
    else if(TrigMode == DTIF_INT_SW_TRIG)
    {
        ;
    }
    else if(TrigMode == DTIF_INT_BOTH_TRIG)
    {
        *nzcl |= 1ul;
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable DTIF interrupt of NZCL
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  WFGCoupleCh WFG couple channel
 ** \arg    WFG_CH10
 ** \arg    WFG_CH32
 ** \arg    WFG_CH54
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_NZCLDisableDTIFInt(uint8_t MFTUnit, NZCL_DTIFIntTrigT TrigMode)
{
    volatile uint16_t* nzcl;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((TrigMode == DTIF_INT_DTTIxX_PIN_TRIG)
           || (TrigMode == DTIF_INT_SW_TRIG)
           || (TrigMode == DTIF_INT_BOTH_TRIG) );
    
    nzcl = GET_BASE_NZCL_ADDR(MFTUnit) ;
    if(TrigMode == DTIF_INT_DTTIxX_PIN_TRIG)
    {
        *nzcl &= ~1ul;
    }
    else if(TrigMode == DTIF_INT_SW_TRIG)
    {
        ;
    }
    else if(TrigMode == DTIF_INT_BOTH_TRIG)
    {
        *nzcl &= ~1ul ;
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set noise canceller width of NZCL
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  Width noise canceller width
 ** \arg    NZCL_NOISE_CANCEL_NULL
 ** \arg    NZCL_NOISE_CANCEL_4PCLK
 ** \arg    NZCL_NOISE_CANCEL_8PCLK
 ** \arg    NZCL_NOISE_CANCEL_16PCLK
 ** \arg    NZCL_NOISE_CANCEL_32PCLK
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_NZCLSetNoiseCancelWidth(uint8_t MFTUnit, NZCL_NoiseCanceT Width)
{
    volatile uint16_t* nzcl;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    
    nzcl = GET_BASE_NZCL_ADDR(MFTUnit) ;
    *nzcl &= ~(7ul<<1);
    *nzcl |= (Width<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Forcely generate DTIF interrupt of NZCL
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  Width noise canceller width
 ** \arg    NZCL_NOISE_CANCEL_NULL
 ** \arg    NZCL_NOISE_CANCEL_4PCLK
 ** \arg    NZCL_NOISE_CANCEL_8PCLK
 ** \arg    NZCL_NOISE_CANCEL_16PCLK
 ** \arg    NZCL_NOISE_CANCEL_32PCLK
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_NZCLForceGen(uint8_t MFTUnit)
{
    volatile uint16_t* nzcl;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    
    nzcl = GET_BASE_NZCL_ADDR(MFTUnit) ;
    *nzcl |= (1ul<<4);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get the interrupt flag of NZCL
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 **
 ** \return NZCL interrupt flag
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
 */
IntStatusT MFT_NZCLGetIntFlag(uint8_t MFTUnit)
{
    volatile uint16_t* nzcl;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    
    nzcl = GET_BASE_NZCL_ADDR(MFTUnit);
    return (IntStatusT)(*nzcl & 1ul);
}

/*!
 ******************************************************************************
 ** \brief  Clear the interrupt flag of NZCL
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_NZCLClrIntFlag(uint8_t MFTUnit)
{
    volatile uint16_t* wfir;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    
    wfir = GET_BASE_WFIR_ADDR(MFTUnit) ;
    *wfir |= (1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief   WFG interrupt handler sub function
 **
 ** \param   MFTUnit MFT unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param   IrqValue irq number value
 **
 ** \retval  None
 **
 ******************************************************************************
 */
void MFT_WFG_IntHandler(uint8_t MFTUnit, uint16_t IrqValue)
{
    volatile uint16_t* wfir;  
    uint8_t i;
    
    for(i=0;i<4;i++)
    {
        if((1ul<<(i + MFTUnit*4)) == (IrqValue & (1ul<<(i + MFTUnit*4))))
        {
            if(i != 0)    /* WFG Interrupt */
            {
                /* Clear Interrupt */
                wfir = GET_BASE_WFIR_ADDR(MFTUnit);
                *wfir &= ~(1ul<<(5+4*(i-1)));
                
                if(WFGIntCallback[MFTUnit][i-1] != NULL)
                {
                    /* Call CallBackIrq */
                    WFGIntCallback[MFTUnit][i-1]();
                }
            }
            else         /* DTIF Interrupt */
            {
                /* Clear Interrupt */
                //wfir = GET_BASE_WFIR_ADDR(MFTUnit);
                //*wfir |= (1ul << 1);
                
                if(DTTIIntCallback[MFTUnit] != NULL)
                {
                    /* Call CallBackIrq */
                    DTTIIntCallback[MFTUnit]();
                }
                          
            }
        }
    }
    
    return;
}

#endif

/*****************************************************************************/
/* END OF FILE */
