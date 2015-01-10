/*!
 ******************************************************************************
 **
 ** \file mft_adcmp_fm3.c
 **
 ** \brief  ADC Start Compare Unit all cell drivers file
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


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "mft_adcmp_fm3.h"

#if (PERIPHERAL_ENABLE_MFT_ADCMP == ON) && (PERIPHERAL_AVAILABLE_MFT == ON)
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*! \brief FRT register structure array */
static FM3_MFT_ADCMP_TypeDef* pFM3_MFT_ADCMP_BASE[MFT_AMOUNT] =
{
    (FM3_MFT_ADCMP_TypeDef*)(FM3_MFT0_ADCMP_BASE),
#if (MFT_AMOUNT > 1)    
    (FM3_MFT_ADCMP_TypeDef*)(FM3_MFT1_ADCMP_BASE),
#endif
#if (MFT_AMOUNT > 2)    
    (FM3_MFT_ADCMP_TypeDef*)(FM3_MFT2_ADCMP_BASE),
#endif
};

/*---------------------------------------------------------------------------*/
/* local definition                                                          */
/*---------------------------------------------------------------------------*/
#define GET_BASE_ACSA_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_ADCMP_BASE[MFTUnit]->ACSA
#define GET_BASE_ACSB_ADDR(MFTUnit)              (volatile uint8_t*)&pFM3_MFT_ADCMP_BASE[MFTUnit]->ACSB
#define GET_BASE_ATSA_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_ADCMP_BASE[MFTUnit]->ATSA
#define GET_BASE_ACCP_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_ADCMP_BASE[MFTUnit]->ACCP0
#define GET_BASE_ACCPDN_ADDR(MFTUnit)            (volatile uint16_t*)&pFM3_MFT_ADCMP_BASE[MFTUnit]->ACCPDN0

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief  Select free run timer for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPSelFRTCh(uint8_t MFTUnit, uint8_t ADCMPCh, uint8_t FRTCh)
{
    volatile uint16_t* acsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    ASSERT((ADCMP_FRT_NONE == FRTCh) || ((ADCMP_FRT_CH0) == FRTCh) 
           || (ADCMP_FRT_CH1 == FRTCh) || (ADCMP_FRT_CH2 == FRTCh));
    
    acsa = GET_BASE_ACSA_ADDR(MFTUnit);
    *acsa &= ~(3ul<<(2*ADCMPCh));
    *acsa |= (FRTCh<<(2*ADCMPCh));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set compare mode for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPSetCmpMode(uint8_t MFTUnit, uint8_t ADCMPCh, ADCMP_CmpModeT Mode)
{
    volatile uint16_t* acsa;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    ASSERT((ADCMP_UPPEAKDOWN_ACCP == Mode) || (ADCMP_UP_ACCP == Mode)
           || (ADCMP_DOWN_ACCP == Mode) || (ADCMP_UP_ACCP_PEAKDOWN_ACCPDN == Mode));
    
    acsa = GET_BASE_ACSA_ADDR(MFTUnit);
    *acsa &= ~(3ul<<(8+2*ADCMPCh));
    *acsa |= (Mode<<(8+2*ADCMPCh));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable buffer function for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPEnableBufFunc(uint8_t MFTUnit, uint8_t ADCMPCh)
{
    volatile uint8_t* acsb;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    
    acsb = GET_BASE_ACSB_ADDR(MFTUnit);
    *acsb &= ~(1ul<<(ADCMPCh));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable buffer function for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPDisableBufFunc(uint8_t MFTUnit, uint8_t ADCMPCh)
{
    volatile uint8_t* acsb;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    
    acsb = GET_BASE_ACSB_ADDR(MFTUnit);
    *acsb |= (1ul<<(ADCMPCh));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set buffer transfer mode for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPSetBufTrsfMode(uint8_t MFTUnit, uint8_t ADCMPCh, ADCMP_BufTrsfModeT Mode)
{
    volatile uint8_t* acsb;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    ASSERT((ADCMP_BUF_TRSF_ZERO_MATCH == Mode) || (ADCMP_BUF_TRSF_PEAK_MATCH == Mode));
    
    acsb = GET_BASE_ACSB_ADDR(MFTUnit);
    *acsb &= ~(1ul<<(4+ADCMPCh));
    *acsb |= (Mode<<(4+ADCMPCh));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set ACCP buffer value for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPSetACCPBuf(uint8_t MFTUnit, uint8_t ADCMPCh, uint16_t Val)
{
    volatile uint16_t* accp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
   
    accp = GET_BASE_ACCP_ADDR(MFTUnit) + ADCMPCh*4;
    *accp = Val;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get ACCP value for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
uint16_t  MFT_ADCMPGetACCPVal(uint8_t MFTUnit, uint8_t ADCMPCh)
{
    volatile uint16_t* accp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    
    accp = GET_BASE_ACCP_ADDR(MFTUnit) + ADCMPCh*4;
    return (uint16_t) *accp;
}

/*!
 ******************************************************************************
 ** \brief  Set ACCPDN buffer value for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPSetACCPDNBuf(uint8_t MFTUnit, uint8_t ADCMPCh, uint16_t Val)
{
    volatile uint16_t* accpdn;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
   
    accpdn = GET_BASE_ACCPDN_ADDR(MFTUnit) + ADCMPCh*4;
    *accpdn = Val;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get ACCPDN value for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
uint16_t  MFT_ADCMPGetACCPDNVal(uint8_t MFTUnit, uint8_t ADCMPCh)
{
    volatile uint16_t* accpdn;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    
    accpdn = GET_BASE_ACCPDN_ADDR(MFTUnit) + ADCMPCh*4;
    return (uint16_t) *accpdn;
}

/*!
 ******************************************************************************
 ** \brief  Select ADC scan start trigger source for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPSelADCScanTrig(uint8_t MFTUnit, uint8_t ADCMPCh, ADCMP_ScanTrigModeT Mode)
{
    volatile uint16_t* atsa;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    ASSERT((ADCMP_SCAN_TRIG_ADCMP == Mode) || (ADCMP_SCAN_TRIG_OR_FTR012 == Mode));
    
    atsa = GET_BASE_ATSA_ADDR(MFTUnit);
    *atsa &= ~(3ul<<(2*ADCMPCh));  
    *atsa |= (Mode<<(2*ADCMPCh));
    return;
}

/*!
 ******************************************************************************
 ** \brief  Select ADC priority start trigger source for a certain ADC compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ADCMPCh ADC campare channel
 ** \arg    ADCMP_CH0
 ** \arg    ADCMP_CH1
 ** \arg    ADCMP_CH2
 ** \param  FRTCh  Free run timer channel
 ** \arg    ADCMP_FRT_NONE
 ** \arg    ADCMP_FRT_CH0
 ** \arg    ADCMP_FRT_CH1
 ** \arg    ADCMP_FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
*/
void MFT_ADCMPSelADCPrioTrig(uint8_t MFTUnit, uint8_t ADCMPCh, ADCMP_PrioTrigModeT Mode)
{
    volatile uint16_t* atsa;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ADCMP_CH0 == ADCMPCh) || (ADCMP_CH1 == ADCMPCh) || (ADCMP_CH2 == ADCMPCh));
    ASSERT((ADCMP_PRIO_TRIG_ADCMP == Mode) || (ADCMP_PRIO_TRIG_OR_FTR012 == Mode));
    
    atsa = GET_BASE_ATSA_ADDR(MFTUnit);
    *atsa &= ~(3ul<<(8+2*ADCMPCh));  
    *atsa |= (Mode<<(8+2*ADCMPCh));
    return;
}

#endif
/*****************************************************************************/
/* END OF FILE */
