/*!
 ******************************************************************************
 **
 ** \file mft_ocu_fm3.c
 **
 ** \brief Output Compare Unit all cell drivers file
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
#include "mft_ocu_fm3.h"

#if (PERIPHERAL_ENABLE_MFT_OCU == ON) && (PERIPHERAL_AVAILABLE_MFT == ON)
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
static FM3_MFT_OCU_TypeDef* pFM3_MFT_OCU_BASE[MFT_AMOUNT] =
{
    (FM3_MFT_OCU_TypeDef*)(FM3_MFT0_OCU_BASE),
#if (MFT_AMOUNT > 1)    
    (FM3_MFT_OCU_TypeDef*)(FM3_MFT1_OCU_BASE),
#endif    
#if (MFT_AMOUNT > 2)    
    (FM3_MFT_OCU_TypeDef*)(FM3_MFT2_OCU_BASE),
#endif 
};

/*---------------------------------------------------------------------------*/
/* local definition                                                          */
/*---------------------------------------------------------------------------*/
#define GET_BASE_OCFS_ADDR(MFTUnit)              (volatile uint8_t*)&pFM3_MFT_OCU_BASE[MFTUnit]->OCFS10
#define GET_BASE_OCSA_ADDR(MFTUnit)              (volatile uint8_t*)&pFM3_MFT_OCU_BASE[MFTUnit]->OCSA10
#define GET_BASE_OCSB_ADDR(MFTUnit)              (volatile uint8_t*)&pFM3_MFT_OCU_BASE[MFTUnit]->OCSB10
#define GET_BASE_OCSC_ADDR(MFTUnit)              (volatile uint8_t*)&pFM3_MFT_OCU_BASE[MFTUnit]->OCSC
#define GET_BASE_OCCP_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_OCU_BASE[MFTUnit]->OCCP0

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
static OCU_IntCallbackT *OCUIntCallback[MFT_AMOUNT][OCU_AMOUNT] = {(void*)0};


/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief  Select the free run timer to connect with output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 ** \param  FRTCh Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUSelFRTCh(uint8_t MFTUnit, uint8_t OCUCh, uint8_t FRTCh)
{
    volatile uint8_t* ocfs;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    if((OCUCh == OCU_CH0) || (OCUCh == OCU_CH1))
    {
        ocfs = GET_BASE_OCFS_ADDR(MFTUnit);
    }
    else if((OCUCh == OCU_CH2) || (OCUCh == OCU_CH3))
    {
        ocfs = GET_BASE_OCFS_ADDR(MFTUnit) + 1;
    }
    else if((OCUCh == OCU_CH4) || (OCUCh == OCU_CH5))
    {
        ocfs = GET_BASE_OCFS_ADDR(MFTUnit) + 4;
    }
    else
    {
        return;
    }
    
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocfs &= ~(15ul);
        *ocfs |= FRTCh;
    }
    else
    {
        *ocfs &= ~(15ul<<4);
        *ocfs |= (FRTCh<<4);
    }
    return;
    
}

/*!
 ******************************************************************************
 ** \brief  Start output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUStart(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint8_t* ocsa;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsa |= (1ul);
    }
    else
    {
        *ocsa |= (1ul<<1);
    }
    return;
    
}

/*!
 ******************************************************************************
 ** \brief  Stop output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUStop(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint8_t* ocsa;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsa &= ~(1ul);
    }
    else
    {
        *ocsa &= ~(1ul<<1);
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable buffer function of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUEnableBufFunc(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint8_t* ocsa;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsa &= ~(1ul<<2);
    }
    else
    {
        *ocsa &= ~(1ul<<3);
    }
    return;
    
}

/*!
 ******************************************************************************
 ** \brief  Disable buffer function of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUDisableBufFunc(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint8_t* ocsa;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsa |= (1ul<<2);
    }
    else
    {
        *ocsa |= (1ul<<3);
    }
    return;
    
}

/*!
 ******************************************************************************
 ** \brief  Enable buffer function of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 ** 
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUEnableInt(uint8_t MFTUnit, uint8_t OCUCh, OCU_IntCallbackT *IntCallback)
{
    volatile uint8_t* ocsa;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    ASSERT((IntCallback != NULL));
    
    OCUIntCallback[MFTUnit][OCUCh] = IntCallback;
    
    ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsa |= (1ul<<4);
    }
    else
    {
        *ocsa |= (1ul<<5);
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable interrupt of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUDisableInt(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint8_t* ocsa;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsa &= ~(1ul<<4);
    }
    else
    {
        *ocsa &= ~(1ul<<5);
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get interrupt flag of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
IntStatusT MFT_OCUGetIntFlag(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint8_t* ocsa;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        return (IntStatusT)((*ocsa & (1ul<<6))>>6);
    }

    return (IntStatusT)((*ocsa & (1ul<<7))>>7);
}

/*!
 ******************************************************************************
 ** \brief  Clear interrupt flag of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUClrIntFlag(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint8_t* ocsa;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsa &= ~(1ul<<6);
    }
    else
    {
        *ocsa &= ~(1ul<<7);
    }

    return ;
}

/*!
 ******************************************************************************
 ** \brief  Set RTx pin level of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUSetRTxPinLevel(uint8_t MFTUnit, uint8_t OCUCh, OCU_RTLevelT Level)
{
    volatile uint8_t* ocsb;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsb = GET_BASE_OCSB_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsb &= ~(1ul);
        *ocsb |= Level;
    }
    else
    {
        *ocsb &= ~(1ul<<1);
        *ocsb |=  (Level<<1);
    }
    return;
}


/*!
 ******************************************************************************
 ** \brief  Get RTx pin level of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
OCU_RTLevelT MFT_OCUGetRTxPinLevel(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint8_t* ocsb;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsb = GET_BASE_OCSB_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        return (OCU_RTLevelT)(*ocsb & 1ul);
    }
    
    return (OCU_RTLevelT)((*ocsb & (1ul<<1))>>1);
}

/*!
 ******************************************************************************
 ** \brief  Set buffer transfer mode of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUSetBufTrsfMode(uint8_t MFTUnit, uint8_t OCUCh, OCU_BufTrsfModeT Mode)
{
    volatile uint8_t* ocsb;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    ocsb = GET_BASE_OCSB_ADDR(MFTUnit) + (OCUCh/2)*4;
    temp = OCUCh%2;
    if(temp == 0)
    {
        *ocsb &= ~(1ul<<5);
        *ocsb |= (Mode<<5);
    }
    else
    {
        *ocsb &= ~(1ul<<6);
        *ocsb |=  (Mode<<6);
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set operation mode of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 ** \param  Mode Operation mode
 ** \arg    OddCh_1Change_EvenCh_1Change
 ** \arg    OddCh_2Change_EvenCh_1Change
 ** \arg    OddCh_1Change_EvenCh_ActHigh
 ** \arg    OddCh_ActHigh_EvenCh_1Change
 ** \arg    OddCh_ActLow_EvenCh_1Change
 ** \arg    OddCh_ActHigh_EvenCh_ActHigh
 ** \arg    OddCh_ActLow_EvenCh_ActLow
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUSetOptMode(uint8_t MFTUnit, uint8_t OCUCoupleCh, OCU_OptModeT Mode)
{
    volatile uint8_t *ocsc, *ocsb;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_COUPLE_CH10 == OCUCoupleCh) || (OCU_COUPLE_CH32 == OCUCoupleCh)
           || (OCU_COUPLE_CH54 == OCUCoupleCh));
    ASSERT((OddCh_1Change_EvenCh_1Change == Mode) || (OddCh_2Change_EvenCh_1Change == Mode) ||
           (OddCh_1Change_EvenCh_ActHigh == Mode) || (OddCh_ActHigh_EvenCh_1Change == Mode) ||
           (OddCh_ActLow_EvenCh_1Change == Mode)  || (OddCh_ActHigh_EvenCh_ActHigh == Mode) ||
           (OddCh_ActLow_EvenCh_ActLow == Mode));
    
    ocsb = GET_BASE_OCSB_ADDR(MFTUnit) + (OCUCoupleCh/2)*4;
    ocsc = GET_BASE_OCSC_ADDR(MFTUnit);
    switch (Mode)
    {
        case OddCh_1Change_EvenCh_1Change:
            *ocsb &= ~(1ul<<4);
            *ocsc &= ~(3ul<<OCUCoupleCh);
            break;
        case OddCh_2Change_EvenCh_1Change:
            *ocsb |= (1ul<<4);
            *ocsc &= ~(3ul<<OCUCoupleCh);
            break;
        case OddCh_1Change_EvenCh_ActHigh:
            *ocsb &= ~(1ul<<4);
            *ocsc &= ~(3ul<<OCUCoupleCh);
            *ocsc |= (1ul<<OCUCoupleCh);
            break;
        case OddCh_ActHigh_EvenCh_1Change:
            *ocsb &= ~(1ul<<4);
            *ocsc &= ~(3ul<<OCUCoupleCh);
            *ocsc |= (2ul<<OCUCoupleCh);
            break;
        case OddCh_ActLow_EvenCh_1Change:
            *ocsb |= (1ul<<4);
            *ocsc &= ~(3ul<<OCUCoupleCh);
            *ocsc |= (2ul<<OCUCoupleCh);
            break;
        case OddCh_ActHigh_EvenCh_ActHigh:
            *ocsb &= ~(1ul<<4);
            *ocsc |= (3ul<<OCUCoupleCh);
            break;
        case OddCh_ActLow_EvenCh_ActLow:
            *ocsb |= (1ul<<4);
            *ocsc |= (3ul<<OCUCoupleCh);
            break;
        default:
            break;
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set compare value of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 ** \param  CmpVal Compare value
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_OCUSetCmpValBuf(uint8_t MFTUnit, uint8_t OCUCh, uint16_t CmpVal)
{
    volatile uint16_t* occp;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    occp = GET_BASE_OCCP_ADDR(MFTUnit) + OCUCh*2;
    *occp = CmpVal;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get compare value of output compare unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  OCUCh Output compare unit channel
 ** \arg    OCU_CH0
 ** \arg    ...
 ** \arg    OCU_CH5
 **
 ** \return Compare value
 **
 *****************************************************************************
 */
uint16_t MFT_OCUGetCmpVal(uint8_t MFTUnit, uint8_t OCUCh)
{
    volatile uint16_t* occp;  
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((OCU_CH0 == OCUCh) || (OCU_CH1 == OCUCh) || (OCU_CH2 == OCUCh) || 
           (OCU_CH3 == OCUCh) || (OCU_CH4 == OCUCh) || (OCU_CH5 == OCUCh));
    
    occp = GET_BASE_OCCP_ADDR(MFTUnit) + (OCUCh/2)*2;
    
    return (uint16_t)(*occp);
}

/*!
 ******************************************************************************
 ** \brief   OCU interrupt handler sub function
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
void MFT_OCU_IntHandler(uint8_t MFTUnit, uint16_t IrqValue)
{
    volatile uint8_t* ocsa;  
    uint8_t i,temp;
    
    /* Peak match Interrupt */
    for(i=0;i<6;i++)
    {
        if((1ul<<(i + MFTUnit*6)) == (IrqValue & (1ul<<(i + MFTUnit*6))))
        {
            /* Clear Interrupt */
            ocsa = GET_BASE_OCSA_ADDR(MFTUnit) + (i/2)*4;
            temp = i%2;
            if(temp == 0)
            {
                *ocsa &= ~(1ul<<6);
            }
            else
            {
                *ocsa &= ~(1ul<<7);
            }
            if(OCUIntCallback[MFTUnit][i] != NULL)
            {
                /* Call CallBackIrq */
                OCUIntCallback[MFTUnit][i]();
            }
        }
    }
    
    return;
}

#endif

/*****************************************************************************/
/* END OF FILE */
