/*!
 ******************************************************************************
 **
 ** \file reset_fm3.h
 **
 ** \brief Reset driver header file
 **
 ** 
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2012-12-28 15:54
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
 
#ifndef _CR_FM3_H_
#define _CR_FM3_H_
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_RESET == OFF)
    #if (PERIPHERAL_ENABLE_RESET == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_RESET == ON) && (PERIPHERAL_AVAILABLE_RESET == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief Reset cause definition
 ******************************************************************************
 */
typedef enum
{
    RESET_BY_PWR_ON = 0,    //!< Power on reset 
    RESET_BY_INITX,         //!< INITX pin reset 
#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7)    
    RESET_BY_LVD,           //!< LVD reset
#endif    
    RESET_BY_SWD,           //!< Software watchdog reset
    RESET_BY_HWD,           //!< Hardware watchdog reset
    RESET_BY_CSV,           //!< CSV reset
    RESET_BY_FCS,           //!< FCS reset
    RESET_BY_SW ,           //!< Software reset
    RESET_BY_NULL,
}RST_ResetCauseT;
                             
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

RST_ResetCauseT RST_GetResetCause(void);
  
#ifdef __cplusplus
}
#endif

#endif

#endif /* _CR_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */







