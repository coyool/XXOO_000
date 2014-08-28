/*!
 ******************************************************************************
 **
 ** \file mft_frt_fm3.c
 **
 ** \brief Free-run Timer Unit all cell drivers file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.21
 **
 ** \date 2012-08-16
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
#include "mft_frt_fm3.h"

#if (PERIPHERAL_ENABLE_MFT_FRT == ON) && (PERIPHERAL_AVAILABLE_MFT == ON) 
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*! \brief FRT register structure array */
static FM3_MFT_FRT_TypeDef* pFM3_MFT_FRT_BASE[MFT_AMOUNT] =
{
    (FM3_MFT_FRT_TypeDef*)(FM3_MFT0_FRT_BASE),
#if (MFT_AMOUNT > 1)    
    (FM3_MFT_FRT_TypeDef*)(FM3_MFT1_FRT_BASE),
#endif
#if (MFT_AMOUNT > 2)    
    (FM3_MFT_FRT_TypeDef*)(FM3_MFT2_FRT_BASE),
#endif
};
/*! \brief Peak match interrupt callback function */
static FRT_PeakMatchIntCallbackT* PeakMatchIntCallback[MFT_AMOUNT][MFT_FRT_CH_AMOUNT] = {(void*)0};
/*! \brief Zore match interrupt callback function */
static FRT_ZeroMatchIntCallbackT* ZeroMatchIntCallback[MFT_AMOUNT][MFT_FRT_CH_AMOUNT] = {(void*)0};

/*---------------------------------------------------------------------------*/
/* local definition                                                          */
/*---------------------------------------------------------------------------*/
#define GET_BASE_TCSA_ADDR(MFTUnit)            (volatile uint16_t*)&pFM3_MFT_FRT_BASE[MFTUnit]->TCSA0
#define GET_BASE_TCSB_ADDR(MFTUnit)            (volatile uint16_t*)&pFM3_MFT_FRT_BASE[MFTUnit]->TCSB0
#define GET_BASE_TCCP_ADDR(MFTUnit)            (volatile uint16_t*)&pFM3_MFT_FRT_BASE[MFTUnit]->TCCP0
#define GET_BASE_TCDT_ADDR(MFTUnit)            (volatile uint16_t*)&pFM3_MFT_FRT_BASE[MFTUnit]->TCDT0

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief  Configure clock division value for FRT channel
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 ** \param  Div Division value
 ** \arg    FRT_DIV1
 ** \arg    FRT_DIV2
 ** \arg    FRT_DIV4
 ** \arg    FRT_DIV8
 ** \arg    FRT_DIV16
 ** \arg    FRT_DIV32
 ** \arg    FRT_DIV64
 ** \arg    FRT_DIV128
 ** \arg    FRT_DIV256
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTSetClkDiv(uint8_t MFTUnit, uint8_t FRTCh, MFT_FRTClkDivT Div)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    ASSERT(Div <= FRT_DIV256);

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa &= ~(15ul);
    *tcsa |= Div;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Initialize FRT operation state
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTSetStateInit(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa |= (1ul<<4);    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Cancel FRT operation state initialization
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTCancelStateInit(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa &= ~(1ul<<4);    
    return;
}


/*!
 ******************************************************************************
 ** \brief  Set count mode for FRT channel
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 ** \param  Mode FRT mode
 ** \arg    CNT_MODE_UP
 ** \arg    CNT_MODE_UPDOWN
 **
 ** \return TRUE,FALSE
 **
 *****************************************************************************
*/
void MFT_FrtSetCntMode(uint8_t MFTUnit, uint8_t FRTCh, MFT_FRTCntModeT Mode)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    ASSERT((CNT_MODE_UP == Mode) || (CNT_MODE_UPDOWN == Mode));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa &= ~(1ul<<5);    
    *tcsa |= (Mode<<5);    
    
    return ;
}

/*!
 ******************************************************************************
 ** \brief  Start free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTStart(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
 
    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa &= ~(1ul<<6);    
    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Stop free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTStop(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa |= (1ul<<6);    
    
    return;
}


/*!
 ******************************************************************************
 ** \brief  Enable buffer function of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTEnableBufFunc(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa |= (1ul<<7);       
    return;   
}

/*!
 ******************************************************************************
 ** \brief  Disable buffer function of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTDisableBufFunc(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa &= ~(1ul<<7);       
    return;   
}

/*!
 ******************************************************************************
 ** \brief  Enable peak match interrupt of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 ** \param  IntCallback  Interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTEnablePeakMatchInt(uint8_t MFTUnit, uint8_t FRTCh, 
                               FRT_PeakMatchIntCallbackT* IntCallback)
{
     volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    PeakMatchIntCallback[MFTUnit][FRTCh] = IntCallback;
    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa |= (1ul<<8);    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable peak match interrupt of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTDisablePeakMatchInt(uint8_t MFTUnit, uint8_t FRTCh)
{
     volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa &= ~(1ul<<8);    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get peak match interrupt flag of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return Interrupt flag
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
*/
IntStatusT MFT_FRTGetPeakMatchIntFlag(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    
    return (IntStatusT)((*tcsa & (1ul<<9))>>9); 
}

/*!
 ******************************************************************************
 ** \brief  Clear peak match interrupt flag of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTClrPeakMatchIntFlag(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa &= ~(1ul<<9);
    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable zero match interrupt of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 ** \param  IntCallback Interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTEnableZeroMatchInt(uint8_t MFTUnit, uint8_t FRTCh, 
                               FRT_ZeroMatchIntCallbackT* IntCallback)
{
     volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    ZeroMatchIntCallback[MFTUnit][FRTCh] = IntCallback;
    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa |= (1ul<<13);    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable zero match interrupt of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTDisableZeroMatchInt(uint8_t MFTUnit, uint8_t FRTCh)
{
     volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsa &= ~(1ul<<13);    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get zero match interrupt flag of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return Interrupt flag
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
*/
IntStatusT MFT_FRTGetZeroMatchIntFlag(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    
    return (IntStatusT)((*tcsa & (1ul<<14))>>14); 
}

/*!
 ******************************************************************************
 ** \brief  Clear zero match interrupt flag of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTClrZeroMatchIntFlag(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    
    *tcsa &= ~(1ul<<14);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set the source clock of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 ** \param  SrcClk  Source clock
 ** \arg    FRT_SRC_CLK_PCLK
 ** \arg    FRT_SRC_CLK_EXT_CLK
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTSetSrcClk(uint8_t MFTUnit, uint8_t FRTCh, MFT_FRTSrcClkT SrcClk)
{
    volatile uint16_t* tcsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));

    tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*FRTCh;
    
    *tcsa &= ~(1ul<<15);
    *tcsa |= (SrcClk<<15);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Output the start trigger to ADC when zero match occurs
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 ** \param  ADCUnit  ADC unit
 ** \arg    MFT_ADC_UNIT0
 ** \arg    MFT_ADC_UNIT1
 ** \arg    MFT_ADC_UNIT2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTEnableADCTrig(uint8_t MFTUnit, uint8_t FRTCh, uint8_t ADCUnit)
{
    volatile uint16_t* tcsb;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    ASSERT((MFT_ADC_UNIT0 == ADCUnit) || (MFT_ADC_UNIT1 == ADCUnit) || (MFT_ADC_UNIT2 == ADCUnit));
    
    tcsb = GET_BASE_TCSB_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsb |= (1ul<<ADCUnit);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Don't output the start trigger to ADC when zero match occurs
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 ** \param  ADCUnit  ADC unit
 ** \arg    MFT_ADC_UNIT0
 ** \arg    MFT_ADC_UNIT1
 ** \arg    MFT_ADC_UNIT2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTDisableADCTrig(uint8_t MFTUnit, uint8_t FRTCh, uint8_t ADCUnit)
{
    volatile uint16_t* tcsb;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    ASSERT((MFT_ADC_UNIT0 == ADCUnit) || (MFT_ADC_UNIT1 == ADCUnit) || (MFT_ADC_UNIT2 == ADCUnit));
    
    tcsb = GET_BASE_TCSB_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcsb &= ~(1ul<<ADCUnit);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set the count cycle buffer of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 ** \param  CntCycle Count cycle
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTSetCycleValBuf(uint8_t MFTUnit, uint8_t FRTCh, uint16_t CntCycle)
{
    volatile uint16_t* tccp;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    tccp = GET_BASE_TCCP_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tccp = CntCycle;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get the count cycle of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return Count cycle
 **
 *****************************************************************************
*/
uint16_t MFT_FRTGetCycleVal(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tccp;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    tccp = GET_BASE_TCCP_ADDR(MFTUnit) + 0x0008*FRTCh;
    return (uint16_t)*tccp;
}

/*!
 ******************************************************************************
 ** \brief  Clear current count value of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_FRTClrCurCntVal(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcdt;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    tcdt = GET_BASE_TCDT_ADDR(MFTUnit) + 0x0008*FRTCh;
    *tcdt = 0;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get current count value of free run timer
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  FRTCh  Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return Current count value
 **
 *****************************************************************************
*/
uint16_t MFT_FRTGetCurCntVal(uint8_t MFTUnit, uint8_t FRTCh)
{
    volatile uint16_t* tcdt;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    tcdt = GET_BASE_TCDT_ADDR(MFTUnit) + 0x0008*FRTCh;
    return (uint16_t)*tcdt;
}


/*!
 ******************************************************************************
 ** \brief   FRT interrupt handler sub function
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
void MFT_FRT_IntHandler(uint8_t MFTUnit, uint16_t IrqValue)
{
    volatile uint16_t* tcsa;  
    uint8_t i;
    
    /* Peak match Interrupt */
    for(i=0;i<3;i++)
    {
        if((1ul<<(i + MFTUnit*6)) == (IrqValue & (1ul<<(i + MFTUnit*6))))
        {
            /* Clear Interrupt */
            tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*i;
            *tcsa &= ~(1ul<<9);
            if(PeakMatchIntCallback[MFTUnit][i] != NULL)
            {
                /* Call CallBackIrq */
                PeakMatchIntCallback[MFTUnit][i]();
            }
        }
    }
    
    /* Zore match Interrupt */
    for(i=0;i<3;i++)
    {
        /* Trigger FRT channel 0 zero Interrupt */
        if((1ul<<(3 + i + MFTUnit*6)) == (IrqValue & (1ul<<(3 + i + MFTUnit*6))))
        {
            /* Clear Interrupt */
            tcsa = GET_BASE_TCSA_ADDR(MFTUnit) + 0x0008*i;
            *tcsa &= ~(1ul<<14);
            if(ZeroMatchIntCallback[MFTUnit][i] != NULL)
            {
                /* Call CallBackIrq */
                ZeroMatchIntCallback[MFTUnit][i]();
            }
        }
    }
   
    return;
}

#endif

/*****************************************************************************/
/* END OF FILE */
