/*!
 ******************************************************************************
 **
 ** \file qprc_fm3.c
 **
 ** \brief QPRC module driver.
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.1.0
 **
 ** \date 2011-12-30
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
#include "qprc_fm3.h"

#if (PERIPHERAL_AVAILABLE_QPRC == ON) && (PERIPHERAL_ENABLE_QPRC == ON)

/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*! \brief QPRC register array */
static FM3_QPRC_TypeDef* pFM3_QPRC_BASE[QPRC_CH_NUM] = 
{
    (FM3_QPRC_TypeDef*)(FM3_QPRC0_BASE),
#if (QPRC_CH_NUM > 1)    
    (FM3_QPRC_TypeDef*)(FM3_QPRC1_BASE),
#endif
#if (QPRC_CH_NUM > 2) 
    (FM3_QPRC_TypeDef*)(FM3_QPRC2_BASE),
#endif    
};
/*! \brief QPRC outrange interrupt callback function */
static QPRC_RCOutRangeIntT* RCOutRangeIntCallback[QPRC_CH_NUM] = {(void*)0};
/*! \brief QPRC status interrupt callback function */
static QPRC_PCStatusIntT* PCStatusIntCallback[QPRC_CH_NUM] = {(void*)0};
/*! \brief QPCCR match interrupt callback function */
static QPRC_QPCCRMatchIntT* QPCCRMatchIntCallback[QPRC_CH_NUM] = {(void*)0};
/*! \brief QPRCR match interrupt callback function */
static QPRC_QPRCRMatchIntT* QPRCRMatchIntCallback[QPRC_CH_NUM] = {(void*)0};
/*! \brief QPCCR and QPRCR match interrupt callback function */
static QPRC_QPCCRAndQPRCRMatchIntT* QPCCRAndQPRCRMatchIntCallback[QPRC_CH_NUM] = {(void*)0};
/*! \brief PC inversion interrupt callback function */
static QPRC_PCInvIntT* PCInvCallbackInt[QPRC_CH_NUM] = {(void*)0};


/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief  Set count value of Position counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Val Count value
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetPCCntVal(uint8_t Ch, uint16_t Val)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QPCR = Val;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get count value of Position counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return Count value
 **
 *****************************************************************************
 */
uint16_t QPRC_GetPCCntVal(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return (uint16_t)(pFM3_QPRC_BASE[Ch]->QPCR);
}
    
/*!
 ******************************************************************************
 ** \brief  Set compare value of Position counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Val Compare Value
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetQPCCRVal(uint8_t Ch, uint16_t Val)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QPCCR = Val;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set count value of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Val Compare Value
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetRCCntVal(uint8_t Ch, uint16_t Val)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QRCR = Val;
    return;
}

     
/*!
 ******************************************************************************
 ** \brief  Get count value of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return Count value
 **
 *****************************************************************************
 */
uint16_t QPRC_GetRCCntVal(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return((uint16_t)pFM3_QPRC_BASE[Ch]->QRCR);
}

/*!
 ******************************************************************************
 ** \brief  Set compare value of Position and Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Val Compare Value
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetQPRCRVal(uint8_t Ch, uint16_t Val)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QPRCR = Val;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set maximum count value of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Val Compare Value
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetPCMaxCntVal(uint8_t Ch, uint16_t Val)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QMPR = Val;
    return;
}


/*!
 ******************************************************************************
 ** \brief  Set swap mode for AIN and BIN
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  SwapMode Swap mode
 ** \arg    AIN_BIN_SWAP_OFF
 ** \arg    AIN_BIN_SWAP_ON
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SelAINBINSwap(uint8_t Ch, QPRC_SwapModeT SwapMode)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT((AIN_BIN_SWAP_OFF == SwapMode) || (AIN_BIN_SWAP_ON == SwapMode));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(1ul<<7);
    pFM3_QPRC_BASE[Ch]->QCR |= (SwapMode<<7);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set compare mode of Position and Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Obj Compare object
 ** \arg    QPRCR_CMP_PC_CNT
 ** \arg    QPRCR_CMP_RC_CNT
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SelQPRCRCmpObj(uint8_t Ch, QPRC_QPRCRCmpObjT Obj)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT((QPRCR_CMP_PC_CNT == Obj) || (QPRCR_CMP_RC_CNT == Obj));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(1ul<<6);
    pFM3_QPRC_BASE[Ch]->QCR |= (Obj<<6);
    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set ZIN function of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Func ZIN function
 ** \arg    ZIN_COUNT_CLR_FUNC
 ** \arg    ZIN_GATE_FUNC
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SelZINFunc(uint8_t Ch, QPRC_ZINFuncT Func)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT((ZIN_COUNT_CLR_FUNC == Func) || (ZIN_GATE_FUNC == Func));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(1ul<<5); /* Select  */
    pFM3_QPRC_BASE[Ch]->QCR |= (Func<<5);
    return;
}
   
/*!
 ******************************************************************************
 ** \brief  Set ZIN detect edge of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Bits CGE bits
 ** \arg    ZIN_CGE_BITS_00B
 ** \arg    ZIN_CGE_BITS_01B
 ** \arg    ZIN_CGE_BITS_10B
 ** \arg    ZIN_CGE_BITS_11B
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetZINDetectEdge(uint8_t Ch, QPRC_ZINCGEBitsT Bits)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT((ZIN_CGE_BITS_00B == Bits) || (ZIN_CGE_BITS_01B == Bits) 
           || (ZIN_CGE_BITS_10B == Bits) || (ZIN_CGE_BITS_11B == Bits))

    pFM3_QPRC_BASE[Ch]->QCR &= ~(3ul<<14);
    pFM3_QPRC_BASE[Ch]->QCR |= (Bits<<14);
    return;
}
         
/*!
 ******************************************************************************
 ** \brief  Set BIN detect edge of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Edge BIN edge
 ** \arg    EDGE_DETECT_DISABLE
 ** \arg    EDGE_DETECT_FALLING
 ** \arg    EDGE_DETECT_RISING
 ** \arg    EDGE_DETECT_BOTH
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetBINDetectEdge(uint8_t Ch, QPRC_BINDetectEdgeT Edge)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT((EDGE_DETECT_DISABLE == Edge) || (EDGE_DETECT_FALLING == Edge)
           || (EDGE_DETECT_RISING == Edge) ||  (EDGE_DETECT_BOTH == Edge));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(3ul<<12);
    pFM3_QPRC_BASE[Ch]->QCR |= (Edge<<12);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set AIN detect edge of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Edge AIN edge
 ** \arg    EDGE_DETECT_DISABLE
 ** \arg    EDGE_DETECT_FALLING
 ** \arg    EDGE_DETECT_RISING
 ** \arg    EDGE_DETECT_BOTH
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetAINDetectEdge(uint8_t Ch, QPRC_AINDetectEdgeT Edge)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT((EDGE_DETECT_DISABLE == Edge) || (EDGE_DETECT_FALLING == Edge)
           || (EDGE_DETECT_RISING == Edge) ||  (EDGE_DETECT_BOTH == Edge));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(3ul<<10);
    pFM3_QPRC_BASE[Ch]->QCR |= (Edge<<10);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set reset mask of Position
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Mask Reset mask
 ** \arg    PC_RESET_MASK_DISABLE
 ** \arg    PC_RESET_MASK_TWICE
 ** \arg    PC_RESET_MASK_FOUR_TIMES
 ** \arg    PC_RESET_MASK_EIGHT_TIMES
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetPCResetMask(uint8_t Ch, QPRC_PCResetMaskSelT Mask)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT((PC_RESET_MASK_DISABLE == Mask) || (PC_RESET_MASK_TWICE == Mask)
           || (PC_RESET_MASK_FOUR_TIMES == Mask) || (PC_RESET_MASK_EIGHT_TIMES == Mask));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(3ul<<8);
    pFM3_QPRC_BASE[Ch]->QCR |= (Mask<<8);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Start Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_EnablePCCnt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(1ul<<4);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Stop Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_DisablePCCnt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QCR |= (1ul<<4);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set Position Counter mode
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Mode Position Counter mode
 ** \arg    PC_MODE_STOP
 ** \arg    PC_MODE_UPDOWN
 ** \arg    PC_MODE_PHASE_DIFF
 ** \arg    PC_MODE_DIRECTIONAL
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetPCMode(uint8_t Ch, QPRC_PCModeT Mode)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(3ul);
    pFM3_QPRC_BASE[Ch]->QCR |= (Mode);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set Revolution Counter mode
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  Mode Position Counter mode
 ** \arg    RC_MODE_STOP
 ** \arg    RC_MODE_ZIN_TRIG
 ** \arg    RC_MODE_PCOUF_TRIG
 ** \arg    RC_MODE_ZIN_PCOUF_TRIG
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SetRCMode(uint8_t Ch, QPRC_RCModeT Mode)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QCR &= ~(3ul<<2);
    pFM3_QPRC_BASE[Ch]->QCR |= (Mode<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Set Revolution Counter outrange
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  RangeType Range type
 ** \arg    PC_RANGE_16K
 ** \arg    PC_RANGE_8K
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_SelRCOutRange(uint8_t Ch, QPRC_PCRangeT RangeType)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QECR &= ~(1ul);
    pFM3_QPRC_BASE[Ch]->QECR |= RangeType;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable outrange interrupt of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  IntCallback Interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_EnableRCOutRangeInt(uint8_t Ch, QPRC_RCOutRangeIntT* IntCallback)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT(IntCallback != NULL);
    
    RCOutRangeIntCallback[Ch] = IntCallback;
    pFM3_QPRC_BASE[Ch]->QECR |= (1ul<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable outrange interrupt of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_DisableRCOutRangeInt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QECR &= ~(1ul<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get outrange interrupt flag of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
IntStatusT QPRC_GetRCOutrangeIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
  
    return (IntStatusT)((pFM3_QPRC_BASE[Ch]->QECR & (1ul<<1))>>1);
}

/*!
 ******************************************************************************
 ** \brief  Clear outrange interrupt flag of Revolution Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_ClrRCOutrangeIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QECR &= ~(1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable status interrupt of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  IntCallback Interrupt callback
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_EnablePCStatusInt(uint8_t Ch, QPRC_PCStatusIntT* IntCallback)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT(IntCallback != NULL);
    
    PCStatusIntCallback[Ch] = IntCallback;
    pFM3_QPRC_BASE[Ch]->QICRL |= (1ul<<4);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable status interrupt of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_DisablePCStatusInt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<4);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get Zero index interrupt flag of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
IntStatusT QPRC_GetPCZeroIndexIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return(IntStatusT)((pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<7))>>7);
}

/*!
 ******************************************************************************
 ** \brief  Clear zero index interrupt flag of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_ClrPCZeroIndexIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<7);
    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get underflow interrupt flag of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
IntStatusT QPRC_GetPCUnderflowIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return(IntStatusT)((pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<5))>>5);
}

/*!
 ******************************************************************************
 ** \brief  Clear underflow interrupt flag of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_ClrPCUnderflowIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<5);
    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get overflow interrupt flag of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
IntStatusT QPRC_GetPCOverflowIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return(IntStatusT)((pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<6))>>6);
}

/*!
 ******************************************************************************
 ** \brief  Clear overflow interrupt flag of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_ClrPCOverflowIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<6);
    
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable QPCCR match interrupt
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  IntCallback Interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_EnableQPCCRMatchInt(uint8_t Ch, QPRC_QPCCRMatchIntT* IntCallback)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT(IntCallback != NULL);
    
    QPCCRMatchIntCallback[Ch] = IntCallback;
    pFM3_QPRC_BASE[Ch]->QICRL |= 1ul;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable QPCCR match interrupt
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_DisableQPCCRMatchInt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRL &= ~1ul;
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get QPCCR match interrupt flag
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return Interrupt flag
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
 */
IntStatusT QPRC_GetQPCCRMatchIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return (IntStatusT)((pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<1))>>1);
}

/*!
 ******************************************************************************
 ** \brief  Clear QPCCR match interrupt flag
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_ClrQPCCRMatchIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable QPRCR match interrupt
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  IntCallback Interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_EnableQPRCRMatchInt(uint8_t Ch, QPRC_QPRCRMatchIntT* IntCallback)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT(IntCallback != NULL);
    
    QPRCRMatchIntCallback[Ch] = IntCallback;
    pFM3_QPRC_BASE[Ch]->QICRL |= (1ul<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable QPRCR match interrupt
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_DisableQPRCRMatchInt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get QPRCR match interrupt flag
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return Interrupt status
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
 */
IntStatusT QPRC_GetQPRCRMatchIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return (IntStatusT)((pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<3))>>3);
}

/*!
 ******************************************************************************
 ** \brief  Clear QPRCR match interrupt flag
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_ClrQPRCRMatchIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<3);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Enable QPCCR and QPRCR match interrupt
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  IntCallback Interrupt callback
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_EnableQPCCRAndQPRCRMatchInt(uint8_t Ch, 
                                      QPRC_QPCCRAndQPRCRMatchIntT* IntCallback)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    ASSERT(IntCallback != NULL);
    
    QPCCRAndQPRCRMatchIntCallback[Ch] = IntCallback;
    pFM3_QPRC_BASE[Ch]->QICRH |= (1ul<<4);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable QPCCR and QPRCR match interrupt
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_DisableQPCCRAndQPRCRMatchInt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRH &= ~(1ul<<4);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get QPCCR and QPRCR match interrupt flag
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return Interrupt status
 ** \arg    RESET
 ** \arg    SET 
 **
 *****************************************************************************
 */
IntStatusT QPRC_GetQPCCRAndQPRCRMatchIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return(IntStatusT)((pFM3_QPRC_BASE[Ch]->QICRH & (1ul<<5))>>5);
}

/*!
 ******************************************************************************
 ** \brief  Clear QPCCR and QPRCR match interrupt flag
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_ClrQPCCRAndQPRCRMatchIntFlag(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRH &= ~(1ul<<5);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get direction of Position Counter when PC overflows and underflows
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return QPRC direction
 ** \arg    PC_DIR_INCREASE
 ** \arg    PC_DIR_DECREASE 
 **
 *****************************************************************************
 */
QPRC_PCDirT QPRC_GetPCDirWhenFlow(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return(QPRC_PCDirT)((pFM3_QPRC_BASE[Ch]->QICRH & (1ul<<3))>>3);
}

/*!
 ******************************************************************************
 ** \brief  Get last direction of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return QPRC direction
 ** \arg    PC_DIR_INCREASE
 ** \arg    PC_DIR_DECREASE 
 **
 *****************************************************************************
 */
QPRC_PCDirT QPRC_GetPCLastDir(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return(QPRC_PCDirT)((pFM3_QPRC_BASE[Ch]->QICRH & (1ul<<2))>>2);
}

/*!
 ******************************************************************************
 ** \brief  Enable direction inversion interrupt of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 ** \param  IntCallback interrupt callback function
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_EnablePCInvInt(uint8_t Ch, QPRC_PCInvIntT* IntCallback)             
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    PCInvCallbackInt[Ch] = IntCallback;
    pFM3_QPRC_BASE[Ch]->QICRH |= (1ul);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Disable direction inversion interrupt of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_DisablePCInvInt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRH &= ~(1ul);
    return;
}

/*!
 ******************************************************************************
 ** \brief  Get direction inversion interrupt flag of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return Interrupt flag
 ** \arg    RESET
 ** \arg    SET
 **
 *****************************************************************************
 */
IntStatusT QPRC_GetPCInvInt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    return(IntStatusT)((pFM3_QPRC_BASE[Ch]->QICRH & (1ul<<1))>>1);
}

/*!
 ******************************************************************************
 ** \brief  Clear direction inversion interrupt flag of Position Counter
 **
 ** \param  Ch QPRC channel
 ** \arg    QPRC_CH0
 ** \arg    QPRC_CH1
 ** \arg    QPRC_CH2
 **
 ** \return None
 **
 *****************************************************************************
 */
void QPRC_ClrPCInvInt(uint8_t Ch)
{
    /* Check parameter */
    ASSERT(IS_QPRC_CH_VALID(Ch));
    
    pFM3_QPRC_BASE[Ch]->QICRH &= ~(1ul<<1);
    return;
}


/*!
 ******************************************************************************
 ** \brief   QPRC interrupt handler sub function
 **
 ** \param   MFTUnit MFT unit
 ** \arg     MFT_UNIT0
 ** \arg     MFT_UNIT1
 ** \param   IrqValue irq number value
 **
 ** \retval  None
 **
 ******************************************************************************
 */
void QPRC_IntHandler(uint8_t Ch, uint16_t IrqValue)
{
    /* QPCCR match */
    if((1ul<<(2 + Ch*6)) == (IrqValue & (1ul<<(2 + Ch*6))))
    {
        /* Clear Interrupt */
        if(pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<1))
        {
            if(QPCCRMatchIntCallback[Ch] != NULL)
            {
                /* Call CallBackIrq */
                QPCCRMatchIntCallback[Ch]();
            }
        }
    }

    /* QPRCR match */
    if((1ul<<(3 + Ch*6)) == (IrqValue & (1ul<<(3 + Ch*6))))
    {
        /* Clear Interrupt */
        pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<3);
        if(QPRCRMatchIntCallback[Ch] != NULL)
        {
            /* Call CallBackIrq */
            QPRCRMatchIntCallback[Ch]();
        }
    }
    
    /* PC status match */
    if((1ul<<(4 + Ch*6)) == (IrqValue & (1ul<<(4 + Ch*6))))
    {
        if((pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<7)) == (1ul<<7)) /* Zero index */
        {
            pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<7);
            PCStatusIntCallback[Ch](PC_INT_STATUS_ZERO_INDEX);
        }
      
        if((pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<6)) == (1ul<<6)) /* OVF index */
        {
            pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<6);
            PCStatusIntCallback[Ch](PC_INT_STATUS_OVERFLOW);
        }
        
        if((pFM3_QPRC_BASE[Ch]->QICRL & (1ul<<5)) == (1ul<<5)) /* UDF index */
        {
            pFM3_QPRC_BASE[Ch]->QICRL &= ~(1ul<<5);
            PCStatusIntCallback[Ch](PC_INT_STATUS_UNDERFLOW);
        }
    }
   
    /* PC invert match */
    if((1ul<<(5 + Ch*6)) == (IrqValue & (1ul<<(5 + Ch*6))))
    {
        /* Clear Interrupt */
        pFM3_QPRC_BASE[Ch]->QICRH &= ~(1ul<<1);
        if(PCInvCallbackInt[Ch] != NULL)
        {
            /* Call CallBackIrq */
            PCInvCallbackInt[Ch]();
        }
    }
    
    /* RC outrange */
    if((1ul<<(6 + Ch*6)) == (IrqValue & (1ul<<(6 + Ch*6))))
    {
        /* Clear Interrupt */
        pFM3_QPRC_BASE[Ch]->QECR &= ~(1ul<<1);
        if(RCOutRangeIntCallback[Ch] != NULL)
        {
            /* Call CallBackIrq */
            RCOutRangeIntCallback[Ch]();
        }
    }
    
    /* QPCCR and QPRCR match */
    if((1ul<<(7 + Ch*6)) == (IrqValue & (1ul<<(7 + Ch*6))))
    {
        /* Clear Interrupt */
        pFM3_QPRC_BASE[Ch]->QICRH &= ~(1ul<<5);
        if(QPCCRAndQPRCRMatchIntCallback[Ch] != NULL)
        {
            /* Call CallBackIrq */
            QPCCRAndQPRCRMatchIntCallback[Ch]();
        }
    }
    
    pFM3_QPRC_BASE[Ch]->QICR &= ~0x22EAul;
    
    return;
}

#endif

/*****************************************************************************/
/* END OF FILE */


