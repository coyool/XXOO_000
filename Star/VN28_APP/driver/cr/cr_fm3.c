/*!
 ******************************************************************************
 **
 ** \file cr_fm3.c
 **
 ** \brief cr trimming driver
 **
 ** 
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-8
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
#include "cr_fm3.h"

#if (PERIPHERAL_ENABLE_CR == ON) && (PERIPHERAL_AVAILABLE_CR == ON)
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
 ** \brief Enable CRC trimming setting
 **
 ** \param None
 ** 
 ** \retval None
 **
 ******************************************************************************
 */ 
void CR_RegUnLock(void)
{
    FM3_CRTRIM->MCR_RLR = CR_UNLOCK;
    return;
}


/*!
 ******************************************************************************
 ** \brief Disable CRC trimming setting
 **
 ** \param None
 ** 
 ** \retval None
 **
 ******************************************************************************
 */ 
void CR_RegLock(void)
{
    FM3_CRTRIM->MCR_RLR = 0;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the frequency trimmin value
 **
 ** \param TrimData CR Trimming data
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CR_SetTrimData(int16_t TrimData)
{
#if (MCU_TYPE == DEVICE_TYPE1) || (MCU_TYPE == DEVICE_TYPE2) || \
    (MCU_TYPE == DEVICE_TYPE4) || (MCU_TYPE == DEVICE_TYPE5)  
    FM3_CRTRIM->MCR_FTRM = (TrimData & 0x00FF);
#else
    FM3_CRTRIM->MCR_FTRM = (TrimData & 0x03FF);    
#endif    
    return;
}


/*!
 ******************************************************************************
 ** \brief Set CR oscillation Frequency division 
 **
 ** \param FrqDiv Frequency divsion ratio
 ** \arg CR_FRQ_DIV_4
 ** \arg CR_FRQ_DIV_8
 ** \arg CR_FRQ_DIV_16
 ** \arg CR_FRQ_DIV_32
 ** \arg CR_FRQ_DIV_64 (only Type 3, 7)
 ** \arg CR_FRQ_DIV_128 (only Type 3, 7)
 ** \arg CR_FRQ_DIV_256 (only Type 3, 7)
 ** \arg CR_FRQ_DIV_512 (only Type 3, 7)
 ** 
 ** \retval None
 **
 ** \note The signal after division is internal signal, which connects with a
 **       timer input.
 **
 ******************************************************************************
 */
void CR_SetFreqDiv(CR_FreqDivT Div)
{
    /* Check the parameter*/
    ASSERT (IS_CRC_FQR_DIV(Div));

    FM3_CRTRIM->MCR_PSR = Div;	 
    return;
}


/*!
 ******************************************************************************
 ** \brief Get CR trimming data in flash
 **
 ** \param None
 ** 
 ** \return CRC trimming data
 **
 ******************************************************************************
 */
int16_t CR_GetTrimData(void)
{
    return FM3_FLASH_IF->CRTRMM;
}


#endif

 /*****************************************************************************/
/* END OF FILE */
