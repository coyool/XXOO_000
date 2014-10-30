/*!
 ******************************************************************************
 **
 ** \file extint_fm3.c
 **
 ** \brief this file provides external interrupt drivers.
 **
 ** \author FSLA AE Team
 **
 ** \version V0.21
 **
 ** \date 2012-08-09
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
#include "extint_fm3.h"

#if (PERIPHERAL_ENABLE_EXTINT == ON) && (PERIPHERAL_AVAILABLE_EXTINT == ON)

/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
static EXTI_IntCallBackT* ExtIntCallback[32] = {(void*)0};
static EXTI_IntCallBackT* NMIIntCallback = {(void*)0};


/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Enable external interrupt 
 **
 ** \param Ch
 ** \arg EXTI_CH0
 ** \arg EXTI_CH1
 ** \arg ...
 ** \arg EXTI_CH30
 ** \arg EXTI_CH31
 ** \param IntCallBack Interrupt callback function
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTI_EnableInt(uint8_t Ch, EXTI_IntCallBackT* IntCallBack)
{
    FM3_EXTI->ENIR |= (1ul<<Ch);
    if(Ch <= EXTI_CH31)
    {
        ExtIntCallback[Ch] = IntCallBack;
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief Dsiable external interrupt 
 **
 ** \param Ch
 ** \arg EXTI_CH0
 ** \arg EXTI_CH1
 ** \arg ...
 ** \arg EXTI_CH30
 ** \arg EXTI_CH31
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTI_DisableInt(uint8_t Ch)
{
    FM3_EXTI->ENIR &= ~(1ul<<Ch);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get the external interrupt flag
 **
 ** \param Ch
 ** \arg EXTI_CH0
 ** \arg EXTI_CH1
 ** \arg ...
 ** \arg EXTI_CH30
 ** \arg EXTI_CH31
 **         
 ** \return None
 ** \relval RESET
 ** \relval SET
 **
 ******************************************************************************
 */
FlagStatusT EXTI_GetIntFlag(uint8_t Ch)
{
    uint8_t eirrbit;
    eirrbit = (FM3_EXTI->EIRR & (1ul<<Ch)) >> Ch;
    return (FlagStatusT)eirrbit;
}

/*!
 ******************************************************************************
 ** \brief Clear the external interrupt flag
 **
 ** \param Ch
 ** \arg EXTI_CH0
 ** \arg EXTI_CH1
 ** \arg ...
 ** \arg EXTI_CH30
 ** \arg EXTI_CH31
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTI_ClrIntFlag(uint8_t Ch)
{
    FM3_EXTI->EICL &= ~(1ul<<Ch);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set external interrupt detect mode
 **
 ** \param Ch
 ** \arg EXTI_CH0
 ** \arg EXTI_CH1
 ** \arg ...
 ** \arg EXTI_CH30
 ** \arg EXTI_CH31
 ** \param Mode
 ** \arg EXTI_LEVEL_LOW_DETECT
 ** \arg EXTI_LEVEL_HIGH_DETECT
 ** \arg EXTI_EDGE_RISING
 ** \arg EXTI_EDGE_FALLING
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTI_SetIntDetectMode(uint8_t Ch, EXTI_DetectModeT Mode)
{
    FM3_EXTI->ELVR &= ~(3ul<<(2*Ch));
    FM3_EXTI->ELVR |= (Mode<<(2*Ch));
    return;
}

/*!
 ******************************************************************************
 ** \brief Set external interrupt detect mode
 **
 ** \param Ch
 ** \arg EXTI_CH0
 ** \arg EXTI_CH1
 ** \arg ...
 ** \arg EXTI_CH30
 ** \arg EXTI_CH31
 ** \param Mode
 **         
 ** \return Mode External interrupt detect flag
 ** \retval EXTI_LEVEL_LOW_DETECT
 ** \retval EXTI_LEVEL_HIGH_DETECT
 ** \retval EXTI_EDGE_RISING
 ** \retval EXTI_EDGE_FALLING
 **
 ******************************************************************************
 */
EXTI_DetectModeT EXTI_GetIntDetectSignal(uint8_t Ch)
{
    uint8_t mode;
    mode = (FM3_EXTI->ELVR & (3ul<<(2*Ch))) >> (2*Ch);
    return (EXTI_DetectModeT)mode;
}

/*!
 ******************************************************************************
 ** \brief Get the NMI interrupt flag
 **
 ** \param None
 **         
 ** \return Interrupt flag
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */
FlagStatusT EXTI_NMIGetIntFlag(void)
{
    return (FlagStatusT)bFM3_EXTI_NMIRR_NR0;
}


/*!
 ******************************************************************************
 ** \brief Clear the NMI interrupt flag
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTI_NMIClrIntFlag(void)
{
    bFM3_EXTI_NMICL_NCL0 = 0;
}

/*!
 ******************************************************************************
 ** \brief Set the interrupt callback function of NMI
 **
 ** \param IntCallback pointer to interrupt callback function
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTI_NMISetIntCallback(NMI_IntCallBackT* IntCallback)
{
    NMIIntCallback = IntCallback;
}

/*!
 ******************************************************************************
 ** \brief External interrupt handler
 **
 ** \param Ch External interrupt channel
 ** \arg EXTI_CH0
 ** \arg EXTI_CH1
 ** \arg ...
 ** \arg EXTI_CH30
 ** \arg EXTI_CH31
 **         
 ** \return None
 **
 ******************************************************************************
 */
void ExtInt_IntHandler(uint8_t Ch)
{
    uint32_t eirrbit, enirbit;
    eirrbit = FM3_EXTI->EIRR;
    enirbit = FM3_EXTI->ENIR;
    if((eirrbit & (1ul<<Ch)) && (enirbit & (1ul<<Ch)))
    {
        FM3_EXTI->EICL &= ~(1ul<<Ch);
        ExtIntCallback[Ch]();
    }
    
}

/*!
 ******************************************************************************
 ** \brief NMI interrupt handler
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void NMI_IntHandler(void)
{
    if(bFM3_EXTI_NMIRR_NR0)
    {
        bFM3_EXTI_NMICL_NCL0 = 0;
        NMIIntCallback();
    }
}

#endif
    

/*****************************************************************************/

/* END OF FILE */
