/*!
 ******************************************************************************
 **
 ** \file wd_fm3.c
 **
 ** \brief watchdog timer driver
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
 */

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "wd_fm3.h"
 
#if (PERIPHERAL_ENABLE_WD == ON) && (PERIPHERAL_AVAILABLE_WD == ON)

/*---------------------------------------------------------------------------*/
/* WD bit definition                                                      */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/
/*! \brief store hardware interrupt callback function handler */ 
static HWD_IntCallbackT *HWDIntCallback;
/*! \brief store software interrupt callback function handler */ 
static HWD_IntCallbackT *SWDIntCallback;

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Enable reset function of hardware watchdog
 **
 ** \param None.
 **         
 ** \return None
 ******************************************************************************
 */
void HWD_EnableReset(void)
{
    bFM3_HWWDT_WDG_CTL_RESEN = 1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable reset function of hardware watchdog
 **
 ** \param None.
 **         
 ** \return None
 ******************************************************************************
 */
void HWD_DisableReset(void)
{
    bFM3_HWWDT_WDG_CTL_RESEN = 0 ;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable interrupt function of hardware watchdog
 **
 ** \param None.
 **         
 ** \return None
 ******************************************************************************
 */
void HWD_EnableInt(HWD_IntCallbackT* IntCallback)
{
    bFM3_HWWDT_WDG_CTL_INTEN = 1;
    HWDIntCallback = IntCallback;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable interrupt function of hardware watchdog
 **
 ** \param None.
 **         
 ** \return None
 ******************************************************************************
 */
void HWD_DisableInt(void)
{
    bFM3_HWWDT_WDG_CTL_INTEN = 0;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the timer count of hardware watchdog
 **
 ** \param Cnt Count value.
 **         
 ** \return None
 ******************************************************************************
 */
void HWD_SetCount(uint32_t Cnt)
{
    FM3_HWWDT->WDG_LDR = Cnt;
    return;
}

/*!
 ******************************************************************************
 ** \brief Get current timer count of hardware watchdog
 **
 ** \param None
 **         
 ** \return Current timer count value
 ******************************************************************************
 */
uint32_t HWD_GetCurCount(void)
{
    return (FM3_HWWDT->WDG_VLR);
}

/*!
 ******************************************************************************
 ** \brief Unlock the hardware watchdog register 
 **
 ** \param Mode
 ** \arg HWD_UNLOCK_ALL_EXCEPT_CTRL
 ** \arg HWD_UNLOCK_ALL
 **         
 ** \return None
 ******************************************************************************
 */
void HWD_UnlockReg(uint8_t Mode)
{
    if(HWD_UNLOCK_ALL_EXCEPT_CTRL == Mode)
    {
        FM3_HWWDT->WDG_LCK = 0x1ACCE551;
    }
    else if(HWD_UNLOCK_ALL == Mode)
    {
        FM3_HWWDT->WDG_LCK = 0x1ACCE551;
        FM3_HWWDT->WDG_LCK = 0xE5331AAE;
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief Get the interrupt flag of hardware watchdog
 **
 ** \param None
 **         
 ** \return Interrupt flag
 ** \arg RESET
 ** \arg SET
 ******************************************************************************
 */
FlagStatusT HWD_GetIntFlag(void)
{
    return (FlagStatusT)bFM3_HWWDT_WDG_RIS_RIS;
}

/*!
 ******************************************************************************
 ** \brief Clear the interrupt flag of hardware watchdog
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void HWD_ClrIntFlag(void)
{
    FM3_HWWDT->WDG_ICL = 0x55u;
    FM3_HWWDT->WDG_ICL = 0xAAu;
}

/*!
 ******************************************************************************
 ** \brief Hardware watchdog interrupt handler API
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void HWD_IntHandler(void)
{
    HWDIntCallback();
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable software watchdog reset 
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_EnableReset(void)
{
    bFM3_SWWDT_WDOGCONTROL_RESEN = 1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable software watchdog reset 
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_DisableReset(void)
{
    bFM3_SWWDT_WDOGCONTROL_RESEN = 0;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable software watchdog interrupt 
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_EnableInt(SWD_IntCallbackT* IntCallback)
{
    SWDIntCallback = IntCallback;
    bFM3_SWWDT_WDOGCONTROL_INTEN = 1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable software watchdog interrupt 
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_DisableInt(void)
{
    bFM3_SWWDT_WDOGCONTROL_INTEN = 0;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the count of  software watchdog 
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_SetCount(uint32_t Cnt)
{
    FM3_SWWDT->WDOGLOAD = Cnt;
    return;
}


/*!
 ******************************************************************************
 ** \brief Get the current count of  software watchdog 
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
uint32_t SWD_GetCurCount(void)
{
    return FM3_SWWDT->WDOGVALUE;
}

/*!
 ******************************************************************************
 ** \brief Unlock all registers of software watchdog
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_UnlockAllReg(void)
{
    FM3_SWWDT->WDOGLOCK = 0x1ACCE551;
    return;
}

/*!
 ******************************************************************************
 ** \brief Lock all registers of software watchdog
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_LockReg(void)
{
    FM3_SWWDT->WDOGLOCK = 0;
    return;
}

/*!
 ******************************************************************************
 ** \brief Get the interrupt flag of software watchdog
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
FlagStatusT SWD_GetIntFlag(void)
{
    return(FlagStatusT)bFM3_SWWDT_WDOGRIS_RIS;
}

/*!
 ******************************************************************************
 ** \brief Clear the interrupt flag of software watchdog
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_ClrIntFlag(void)
{
    FM3_SWWDT->WDOGINTCLR = 0;
    return;
}

/*!
 ******************************************************************************
 ** \brief SWD IRQ handler
 **
 ** \param None
 **         
 ** \return None
 ******************************************************************************
 */
void SWD_IntHandler(void)
{
    SWDIntCallback();
}

/*****************************************************************************/

#endif

/* END OF FILE */
