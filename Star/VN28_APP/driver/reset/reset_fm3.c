/*!
 ******************************************************************************
 **
 ** \file reset_fm3.c
 **
 ** \brief Reset driver
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
#include "reset_fm3.h"

#if (PERIPHERAL_ENABLE_RESET == ON) && (PERIPHERAL_AVAILABLE_RESET == ON)
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
 ** \brief Get reset status
 **
 ** \param None
 ** 
 ** \return Reset cause
 ** \retval RESET_BY_PWR_ON
 ** \retval RESET_BY_INITX
 ** \retval RESET_BY_LVD
 ** \retval RESET_BY_SWD
 ** \retval RESET_BY_HWD
 ** \retval RESET_BY_CSV
 ** \retval RESET_BY_FCS
 ** \retval RESET_BY_SW
 **
 ******************************************************************************
 */ 
RST_ResetCauseT RST_GetResetCause(void)
{
    uint16_t cause;
    cause = FM3_CRG->RST_STR;
    
    if(cause == 0)
    {
        return RESET_BY_NULL;
    }
    
    if((cause & 0x0001) == 0x0001)
    {
        return RESET_BY_PWR_ON;
    }
    
    if((cause & 0x0002) == 0x0002)
    {
        return RESET_BY_INITX;
    }
#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7)    
    if((cause & 0x0008) == 0x0008)
    {
        return RESET_BY_LVD;
    }
#endif
    if((cause & 0x0010) == 0x0010)
    {
        return RESET_BY_SWD;
    }  
    
    if((cause & 0x0020) == 0x0020)
    {
        return RESET_BY_HWD;
    } 
    
    if((cause & 0x0080) == 0x0080)
    {
        return RESET_BY_FCS;
    } 
    
    if((cause & 0x0040) == 0x0040)
    {
        return RESET_BY_CSV;
    } 
    
    if((cause & 0x0080) == 0x0080)
    {
        return RESET_BY_SW;
    } 
    
    return RESET_BY_NULL;
}

#endif

 /*****************************************************************************/
/* END OF FILE */
