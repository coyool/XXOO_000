/*!
 ******************************************************************************
 **
 ** \file wd_fm3.h
 **
 ** \brief head file of watchdog timer driver
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
 
#ifndef _WD_FM3_H_
#define _WD_FM3_H_
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_WD == OFF)
    #if (PERIPHERAL_ENABLE_WD == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_WD == ON) && (PERIPHERAL_AVAILABLE_WD == ON)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/* HWD unlock mode */
/*! \brief unlock all registers except control register */   
#define HWD_UNLOCK_ALL_EXCEPT_CTRL           (0)
/*! \brief unlock all registers */  
#define HWD_UNLOCK_ALL                       (1)

/*!
 ******************************************************************************
 ** \brief HWD interrupr callback function type definition
 ******************************************************************************
 */
typedef void HWD_IntCallbackT (void);
/*!
 ******************************************************************************
 ** \brief SWD interrupr callback function type definition
 ******************************************************************************
 */
typedef void SWD_IntCallbackT (void);

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/* Hardtware watchdog timer */
void HWD_EnableReset(void);
void HWD_DisableReset(void);
void HWD_EnableInt(HWD_IntCallbackT* IntCallback);
void HWD_DisableInt(void);
void HWD_SetCount(uint32_t Cnt);
uint32_t HWD_GetCurCount(void);
void HWD_UnlockReg(uint8_t Mode);
FlagStatusT HWD_GetIntFlag(void);
void HWD_ClrIntFlag(void);
void HWD_IntHandler(void);


/* Software watchdog timer */
void SWD_EnableReset(void);
void SWD_DisableReset(void);
void SWD_EnableInt(SWD_IntCallbackT* IntCallback);
void SWD_DisableInt(void);
void SWD_SetCount(uint32_t Cnt);
uint32_t SWD_GetCurCount(void);
void SWD_UnlockAllReg(void);
void SWD_LockAllReg(void);
FlagStatusT SWD_GetIntFlag(void);
void SWD_ClrIntFlag(void);
void SWD_IntHandler(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _CR_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */



