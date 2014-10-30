/*!
 ******************************************************************************
 **
 ** \file wc_fm3.c
 **
 ** \brief this file provides all watch counter drivers
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-18
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
#include "wc_fm3.h"

#if (PERIPHERAL_ENABLE_WC == ON) && (PERIPHERAL_AVAILABLE_WC == ON)

/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/
/* CLK_SEL bit definition */
/*! \brief SEL_IN */  
#define  CLK_SEL_SEL_IN       ((uint16_t)0x0001)
/*! \brief SEL_OUT */  
#define  CLK_SEL_SEL_OUT      ((uint16_t)0x0100)

/* CLK_EN bit definition */
/*! \brief CLK_EN */
#define  CLK_EN_CLK_EN          ((uint8_t)0x01)
/*! \brief CLK_EN_R */
#define  CLK_EN_CLK_EN_R        ((uint8_t)0x02)

/* WCCR bit definition */
/*! \brief WCIF */
#define  WCCR_WCIF              ((uint8_t)0x01)
/*! \brief WCIE */
#define  WCCR_WCIE              ((uint8_t)0x02)
/*! \brief CS */
#define  WCCR_CS                ((uint8_t)0x0C)
/*! \brief WCOP */
#define  WCCR_WCOP              ((uint8_t)0x40)
/*! \brief WCEN */
#define  WCCR_WCEN              ((uint8_t)0x80)

/*! \brief Watch counter Callback function */
static WC_IntHandlerCallbackT *pWC_IntCallback = NULL;

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief Select input clock for watch counter prescaler
 **
 ** \param InClock set the input clock
 **        This parameter can be one of the following values:
 ** \arg   WCPRES_INPUT_CLOCK_SUB_CLOCK
 ** \arg   WCPRES_INPUT_CLOCK_MAIN_CLOCK
 **         
 **
 ** \return None
 **
 ******************************************************************************
 */
void WCPRES_ClkInSel(uint8_t InClock) 
{
    /* Check the parameter */
    ASSERT (IS_WCPRES_INPUT_CLOCK_VALID(InClock));
    
    /* Set the input clock for watch counter */
    if(InClock == WCPRES_INPUT_CLOCK_SUB_CLOCK)
    {
        bFM3_WC_CLK_SEL_SEL_IN = 0;
    }
    else
    {
        bFM3_WC_CLK_SEL_SEL_IN = 1;
    }
      
    return;
}

/*!
 ******************************************************************************
 ** \brief Select ouput clock array for watch counter prescaler
 **
 ** \param OutClock set the output clock array
 **        This parameter can be one of the following values:
 ** \arg   WCPRES_OUTPUT_CLOCK_ARRAY0
 ** \arg   WCPRES_OUTPUT_CLOCK_ARRAY1
 **         
 ** \return None
 **
 ******************************************************************************
 */
void WCPRES_ClkOutSel (uint32_t OutClock)
{
    /* Check the parameter */
    ASSERT (IS_WCPRES_OUTPUT_CLOCK_ARRAY_VALID(OutClock));
    
    /* Select output clock array */
    if(OutClock == WCPRES_OUTPUT_CLOCK_ARRAY0)
    {
        bFM3_WC_CLK_SEL_SEL_OUT = 0;
    }
    else
    {
        bFM3_WC_CLK_SEL_SEL_OUT = 1;
    }
    
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable/Disable ouput clock of watch counter prescaler
 **
 ** \param ClkEn Enable/Disable clock output
 **        This parameter can be one of the following values:
 ** \arg   ENABLE
 ** \arg   DISABLE
 **         
 ** \return None
 **
 ******************************************************************************
 */
void WCPRES_ClkEn (FuncStateT ClkEn)
{
    /* Check the parameter */
    ASSERT (IS_FUNCTIONAL_STATE(ClkEn));
    
    /* enable/disable clock output */   
    if(ClkEn == ENABLE)
    {
        bFM3_WC_CLK_EN_CLK_EN = 1;
    }
    else
    {
        bFM3_WC_CLK_EN_CLK_EN = 0;
    }

    return;
}

/*!
 ******************************************************************************
 ** \brief Get the clock status of watch counter prescaler
 **
 ** \param None.
 **         
 ** \return Clock status
 ** \retval RESET
 ** \retval SET
 ** 
 **
 ******************************************************************************
 */
FlagStatusT WCPRES_GetClkStat (void)
{
    return (FlagStatusT)bFM3_WC_CLK_EN_CLK_EN_R;
}

/*!
 ******************************************************************************
 ** \brief Get the clock status of watch counter prescaler
 **
 ** \param CntClk Count clock
 ** \arg WC_OUTPUT_CLOCK_WCCK0
 ** \arg WC_OUTPUT_CLOCK_WCCK1
 ** \arg WC_OUTPUT_CLOCK_WCCK2
 ** \arg WC_OUTPUT_CLOCK_WCCK3
 **         
 ** \return None
 ** 
 ******************************************************************************
 */
void WC_CntClkSel(uint8_t CntClk)
{
    /* Check the parameter */
    ASSERT (IS_WC_OUTPUT_CLOCK_VALID(CntClk));
    FM3_WC->WCCR &= ~(3ul << 2);
    FM3_WC->WCCR |= (CntClk << 2);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable interrupt of watch counter
 **
 ** \param IntCallback Interrupt callback function
 **         
 ** \return None
 ** 
 ******************************************************************************
 */
void WC_EnableInt(WC_IntHandlerCallbackT* IntCallback)
{
    ASSERT(IntCallback != NULL);
    pWC_IntCallback = IntCallback;
    bFM3_WC_WCCR_WCIE = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable interrupt of watch counter
 **
 ** \param None
 **         
 ** \return None
 ** 
 ******************************************************************************
 */
void WC_DisableInt(void)
{
    bFM3_WC_WCCR_WCIE = 0;
}


/*!
 ******************************************************************************
 ** \brief Set the counter value of watch counter
 **
 ** \param Cnt Count value
 **         
 ** \return None
 ** 
 ******************************************************************************
 */
void WC_SetCntVal(uint8_t Cnt)
{
    ASSERT(Cnt <= 63);
    FM3_WC->WCRL = Cnt;
}

/*!
 ******************************************************************************
 ** \brief Start watch counter 
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */
void WC_Start(void)
{
    bFM3_WC_WCCR_WCEN = 1;
}
 
/*!
 ******************************************************************************
 ** \brief Stop watch counter 
 **
 ** \param None
 **
 ** \return None
 **
 ******************************************************************************
 */ 
 void WC_Stop(void)
{
    bFM3_WC_WCCR_WCEN = 0;
}

/*!
 ******************************************************************************
 ** \brief Get count value
 **
 ** \param None
 **
 ** \return Count value
 **
 ******************************************************************************
 */
uint8_t WC_GetCntVal(void)
{
    return (FM3_WC->WCRD);
}

/*!
 ******************************************************************************
 ** \brief Get watch counter operation status
 **
 ** \param None
 **         
 ** \return Operation status
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */ 
FlagStatusT WC_GetOpStat(void)
{
    return (FlagStatusT)bFM3_WC_WCCR_WCOP;
} 

/*!
 ******************************************************************************
 ** \brief Get watch counter interrupt status
 **
 ** \param None
 **         
 ** \return Interrupt status
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */ 
IntStatusT WC_GetIntFlag(void)
{
    return (IntStatusT)bFM3_WC_WCCR_WCIF;
}

/*!
 ******************************************************************************
 ** \brief Clear watch counter interrupt flag
 **
 ** \param None
 **         
 ** \return None
 ** 
 ******************************************************************************
 */ 
void WC_ClrIntFlag(void)
{
    bFM3_WC_WCCR_WCIF = 0;
}

/*!
 ******************************************************************************
 ** \brief Watch counter interrupt handler
 **
 ** \param None
 **         
 ** \return None
 ** 
 ******************************************************************************
 */ 
void WC_IntHandler(void)
{
    if(SET == WC_GetIntFlag())
    {
        WC_ClrIntFlag();
        if(pWC_IntCallback != NULL) 
        {
            pWC_IntCallback();
        }
    }
}

#endif

/*****************************************************************************/

/* END OF FILE */
