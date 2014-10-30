/*!
 ******************************************************************************
 **
 ** \file mft_icu_fm3.c
 **
 ** \brief  Input Capture Unit all cell drivers file
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
#include "mft_icu_fm3.h"
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
#if (PERIPHERAL_ENABLE_MFT_ICU == ON) && (PERIPHERAL_AVAILABLE_MFT == ON)
/*! \brief FRT register structure array */
static FM3_MFT_ICU_TypeDef* pFM3_MFT_ICU_BASE[MFT_AMOUNT] =
{
    (FM3_MFT_ICU_TypeDef*)(FM3_MFT0_ICU_BASE),
#if (MFT_AMOUNT > 1)    
    (FM3_MFT_ICU_TypeDef*)(FM3_MFT1_ICU_BASE),
#endif
#if (MFT_AMOUNT > 2)    
    (FM3_MFT_ICU_TypeDef*)(FM3_MFT2_ICU_BASE),
#endif
};

/*! \brief ICU interrupt callback function */
static ICU_IntCallbackT* ICUIntCallback[MFT_AMOUNT][ICU_AMOUNT] = {(void*)0};

/*---------------------------------------------------------------------------*/
/* local definition                                                          */
/*---------------------------------------------------------------------------*/
#define GET_BASE_ICFS_ADDR(MFTUnit)              (volatile uint8_t*)&pFM3_MFT_ICU_BASE[MFTUnit]->ICFS10
#define GET_BASE_ICSA_ADDR(MFTUnit)              (volatile uint8_t*)&pFM3_MFT_ICU_BASE[MFTUnit]->ICSA10
#define GET_BASE_ICSB_ADDR(MFTUnit)              (volatile uint8_t*)&pFM3_MFT_ICU_BASE[MFTUnit]->ICSB10
#define GET_BASE_ICCP_ADDR(MFTUnit)              (volatile uint16_t*)&pFM3_MFT_ICU_BASE[MFTUnit]->ICCP0

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief  Select the free run timer to connect with input capture unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ICUCh Input capture unit channel
 ** \arg    ICU_CH0
 ** \arg    ICU_CH1
 ** \arg    ICU_CH2
 ** \arg    ICU_CH3
 ** \param  FRTCh Free run timer channel
 ** \arg    FRT_CH0
 ** \arg    FRT_CH1
 ** \arg    FRT_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_ICUSelFRTCh(uint8_t MFTUnit, uint8_t ICUCh, uint8_t FRTCh)
{
    volatile uint8_t* icfs;  
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ICU_CH0 == ICUCh) || (ICU_CH1 == ICUCh) || (ICU_CH2 == ICUCh) || (ICU_CH3 == ICUCh));
    ASSERT((FRT_CH0 == FRTCh) || (FRT_CH1 == FRTCh) || (FRT_CH2 == FRTCh));
    
    icfs = GET_BASE_ICFS_ADDR(MFTUnit) + (ICUCh/2);
    temp = ICUCh%2;
    if(temp == 0)
    {
        *icfs &= ~(15ul);
        *icfs |= FRTCh;
    }
    else
    {
        *icfs &= ~(15ul<<4);
        *icfs |= (FRTCh<<4);
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set the valid edge of input capture unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ICUCh Input capture unit channel
 ** \arg    ICU_CH0
 ** \arg    ICU_CH1
 ** \arg    ICU_CH2
 ** \arg    ICU_CH3
 ** \param  ValidEdge Valid edge
 ** \arg    ICU_VALID_EDGE_NULL
 ** \arg    ICU_VALID_EDGE_RISING
 ** \arg    ICU_VALID_EDGE_FALLING
 ** \arg    ICU_VALID_EDGE_BOTH
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_ICUSetTrigMode(uint8_t MFTUnit, uint8_t ICUCh, MFT_ICUValidEdgeT ValidEdge)
{
    volatile uint8_t* icsa; 
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ICU_CH0 == ICUCh) || (ICU_CH1 == ICUCh) || (ICU_CH2 == ICUCh) || (ICU_CH3 == ICUCh));
    ASSERT((ICU_VALID_EDGE_NULL == ValidEdge) || (ICU_VALID_EDGE_RISING == ValidEdge)
           || (ICU_VALID_EDGE_FALLING == ValidEdge) || (ICU_VALID_EDGE_BOTH == ValidEdge));
    
    icsa = GET_BASE_ICSA_ADDR(MFTUnit) + (ICUCh/2)*4;
    temp = ICUCh%2;
    if(temp == 0)
    {
        *icsa &= ~(3ul);
        *icsa |= ValidEdge;
    }
    else
    {
        *icsa &= ~(3ul<<2);
        *icsa |= (ValidEdge<<2);
    }
    return;
    
}

/*!
 ******************************************************************************
 ** \brief  Enable the interrupt of input capture unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ICUCh Input capture unit channel
 ** \arg    ICU_CH0
 ** \arg    ICU_CH1
 ** \arg    ICU_CH2
 ** \arg    ICU_CH3
 ** \param  IntCallback Pointer to interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_ICUEnableInt(uint8_t MFTUnit, uint8_t ICUCh, ICU_IntCallbackT* IntCallback)
{
    volatile uint8_t* icsa; 
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ICU_CH0 == ICUCh) || (ICU_CH1 == ICUCh) || (ICU_CH2 == ICUCh) || (ICU_CH3 == ICUCh));
    ASSERT((IntCallback != NULL));
    
    ICUIntCallback[MFTUnit][ICUCh] = IntCallback;
    
    icsa = GET_BASE_ICSA_ADDR(MFTUnit) + (ICUCh/2)*4;
    temp = ICUCh%2;
    if(temp == 0)
    {
        *icsa |= (1<<4);
    }
    else
    {
        *icsa |= (1<<5);
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable the interrupt of input capture unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ICUCh Input capture unit channel
 ** \arg    ICU_CH0
 ** \arg    ICU_CH1
 ** \arg    ICU_CH2
 ** \arg    ICU_CH3
 ** \param  IntCallback Pointer to interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_ICUDisableInt(uint8_t MFTUnit, uint8_t ICUCh)
{
    volatile uint8_t* icsa; 
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ICU_CH0 == ICUCh) || (ICU_CH1 == ICUCh) || (ICU_CH2 == ICUCh) || (ICU_CH3 == ICUCh));
    
    icsa = GET_BASE_ICSA_ADDR(MFTUnit) + (ICUCh/2)*4;
    temp = ICUCh%2;
    if(temp == 0)
    {
        *icsa &= ~(1ul<<4);
    }
    else
    {
        *icsa &= ~(1ul<<5);
    }
    return;
}


/*!
 ******************************************************************************
 ** \brief  Get the interrupt flag of input capture unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ICUCh Input capture unit channel
 ** \arg    ICU_CH0
 ** \arg    ICU_CH1
 ** \arg    ICU_CH2
 ** \arg    ICU_CH3
 **
 ** \return Interrupt flag
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
 */
IntStatusT MFT_ICUGetIntFlag(uint8_t MFTUnit, uint8_t ICUCh)
{
    volatile uint8_t* icsa; 
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ICU_CH0 == ICUCh) || (ICU_CH1 == ICUCh) || (ICU_CH2 == ICUCh) || (ICU_CH3 == ICUCh));
    
    icsa = GET_BASE_ICSA_ADDR(MFTUnit) + (ICUCh/2)*4;
    temp = ICUCh%2;
    if(temp == 0)
    {
        return (IntStatusT)((*icsa & (1ul<<6))>>6);
    }
    
    return (IntStatusT)((*icsa & (1ul<<7))>>7);
    
}

/*!
 ******************************************************************************
 ** \brief  Clear the interrupt flag of input capture unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ICUCh Input capture unit channel
 ** \arg    ICU_CH0
 ** \arg    ICU_CH1
 ** \arg    ICU_CH2
 ** \arg    ICU_CH3
 **
 ** \return None
 **
 *****************************************************************************
 */
void MFT_ICUClrIntFlag(uint8_t MFTUnit, uint8_t ICUCh)
{
    volatile uint8_t* icsa; 
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ICU_CH0 == ICUCh) || (ICU_CH1 == ICUCh) || (ICU_CH2 == ICUCh) || (ICU_CH3 == ICUCh));
    
    icsa = GET_BASE_ICSA_ADDR(MFTUnit) + (ICUCh/2)*4;
    temp = ICUCh%2;
    if(temp == 0)
    {
        *icsa &= ~(1ul<<6);
    }
    else
    {
        *icsa &= ~(1ul<<7);
    }
    return ;
}

/*!
 ******************************************************************************
 ** \brief  Get the last captured edge of input capture unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ICUCh Input capture unit channel
 ** \arg    ICU_CH0
 ** \arg    ICU_CH1
 ** \arg    ICU_CH2
 ** \arg    ICU_CH3
 **
 ** \return None
 **
 *****************************************************************************
 */
MFT_ICULastEdgeT MFT_ICUGetLastEdge(uint8_t MFTUnit, uint8_t ICUCh)
{
    volatile uint8_t* icsb; 
    uint8_t temp;
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ICU_CH0 == ICUCh) || (ICU_CH1 == ICUCh) || (ICU_CH2 == ICUCh) || (ICU_CH3 == ICUCh));
    
    icsb = GET_BASE_ICSB_ADDR(MFTUnit) + (ICUCh/2)*4;
    temp = ICUCh%2;
    if(temp == 0)
    {
        return (MFT_ICULastEdgeT)((*icsb & (1ul<<0))>>0);
    }
    return (MFT_ICULastEdgeT)((*icsb & (1ul<<1))>>1);
    
}

/*!
 ******************************************************************************
 ** \brief  Read capture data of input capture unit
 **
 ** \param  MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param  ICUCh Input capture unit channel
 ** \arg    ICU_CH0
 ** \arg    ICU_CH1
 ** \arg    ICU_CH2
 ** \arg    ICU_CH3
 **
 ** \return None
 **
 *****************************************************************************
 */
uint16_t MFT_ICUReadCaptureData(uint8_t MFTUnit, uint8_t ICUCh)
{
    volatile uint16_t* icsb; 
    /* Check parameter */
    ASSERT(IS_MFT_UNIT_VALID(MFTUnit));
    ASSERT((ICU_CH0 == ICUCh) || (ICU_CH1 == ICUCh) || (ICU_CH2 == ICUCh) || (ICU_CH3 == ICUCh));
    
    icsb = GET_BASE_ICCP_ADDR(MFTUnit) + ICUCh*2;

    return *icsb;
    
}

/*!
 ******************************************************************************
 ** \brief   ICU interrupt handler sub function
 **
 ** \param   MFTUnit MFT Unit
 ** \arg    MFT_UNIT0
 ** \arg    MFT_UNIT1
 ** \arg    MFT_UNIT2
 ** \param   IrqValue irq number value
 **
 ** \retval  None
 **
 ******************************************************************************
 */
void MFT_ICU_IntHandler(uint8_t MFTUnit, uint16_t IrqValue)
{
    volatile uint8_t* icsa;  
    uint8_t i,temp;
    
    /* Peak match Interrupt */
    for(i=0;i<4;i++)
    {
        if((1ul<<(i + MFTUnit*4)) == (IrqValue & (1ul<<(i + MFTUnit*4))))
        {
            /* Clear Interrupt */
            icsa = GET_BASE_ICSA_ADDR(MFTUnit) + (i/2)*4;
            temp = i%2;
            if(temp == 0)
            {
                *icsa &= ~(1ul<<6);
            }
            else
            {
                *icsa &= ~(1ul<<7);
            }
            if(ICUIntCallback[MFTUnit][i] != NULL)
            {
                /* Call CallBackIrq */
                ICUIntCallback[MFTUnit][i]();
            }
        }
    }
    
   
    return;
}

#endif

/*****************************************************************************/
/* END OF FILE */
