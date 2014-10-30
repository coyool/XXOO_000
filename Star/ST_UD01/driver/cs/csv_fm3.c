/*!
 ******************************************************************************
 **
 ** \file csv_fm3.c
 **
 ** \brief Clock supervisor module driver.
 **
 ** \author FSLA AE Team
 **
 ** \version V0.1
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
 ** \note History: \n
 ** V0.1 2011-12-30 original version \n
 **
 ******************************************************************************
 */
 

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "csv_fm3.h"

#if (PERIPHERAL_AVAILABLE_CSV == OFF)
    #if (PERIPHERAL_ENABLE_CSV == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_CSV == ON) && (PERIPHERAL_AVAILABLE_CSV == ON)
/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
static CSV_IntCallbackT *CSVIntCallback = (void*)0;

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
 ** \brief The main CSV function is enabled
 ******************************************************************************
 */
void CSV_EnableMainCSV(void) 
{
    bFM3_CRG_CSV_CTL_MCSVE = 1;
}

/*!
 ******************************************************************************
 ** \brief The main CSV function is disabled
 ******************************************************************************
 */
void CSV_DisableMainCSV(void) 
{
    bFM3_CRG_CSV_CTL_MCSVE = 0;  
}

/*!
 ******************************************************************************
 ** \brief The sub CSV function is enabled.
 ******************************************************************************
 */
void CSV_EnableSubCSV(void) 
{
    bFM3_CRG_CSV_CTL_SCSVE = 1;  
}

/*!
 ******************************************************************************
 ** \brief The sub CSV function is disabled¡£
 ******************************************************************************
 */
void CSV_DisableSubCSV(void) 
{
    bFM3_CRG_CSV_CTL_SCSVE = 0; 
}



/*!
 ******************************************************************************
 ** \brief The FCS function is enabled.
 ******************************************************************************
 */
void CSV_EnableFCS(void) 
{
    bFM3_CRG_CSV_CTL_FCSDE = 1;
}

/*!
 ******************************************************************************
 ** \brief The FCS function is disabled
 ******************************************************************************
 */
void CSV_DisableFCS(void) 
{
    bFM3_CRG_CSV_CTL_FCSDE = 0;
}

/*!
 ******************************************************************************
 ** \brief The FCS reset is enabled.
 ******************************************************************************
 */
void CSV_EnableFCSReset(void) 
{
    bFM3_CRG_CSV_CTL_FCSRE = 1;
}


/*!
 ******************************************************************************
 ** \brief The FCS reset is disabled.
 ******************************************************************************
 */
void CSV_DisableFCSReset(void) 
{
    bFM3_CRG_CSV_CTL_FCSRE = 0;
}
/*!
 ******************************************************************************
 ** \brief Enables FCS interrupts
 ******************************************************************************
 */
void CSV_EnableFCSInt(CSV_IntCallbackT* IntCallback)
{
    CSVIntCallback = IntCallback;
    bFM3_CRG_INT_ENR_FCSE = 1;
}

/*!
 ******************************************************************************
 ** \brief Disables FCS interrupts
 ******************************************************************************
 */
void CSV_DisableFCSInt(void) 
{
    bFM3_CRG_INT_ENR_FCSE = 0;  
}

/*!
 ******************************************************************************
 ** \brief Clears the FCS interrupt cause.
 ******************************************************************************
 */
void CSV_ClrFCSIntRequest(void) 
{
    bFM3_CRG_INT_CLR_FCSC = 1;  
}

/*!
 ******************************************************************************
 ** \brief Get Anomalous frequency detection interrupt status
 **
 ** \return interrupt status
 ** \retval 0 No FCS interrupt has been asserted.
 ** \retval 1 An FCS interrupt has been asserted.
 ******************************************************************************
 */
uint8_t CSV_GetFCSIntRequest(void) 
{
    return bFM3_CRG_INT_STR_FCSI;
}

/*!
 ******************************************************************************
 ** \brief FCS count cycle setting
 **
 ** \param Cycle 
 ** \arg FCS_CYCLE_256      1/256 frequency of high-speed CR oscillation   
 ** \arg FCS_CYCLE_512      1/512 frequency of high-speed CR oscillation
 ** \arg FCS_CYCLE_1024     1/1024 frequency of high-speed CR oscillation
 ******************************************************************************
 */
void CSV_SetFCSCycle(uint16_t Cycle) 
{ 
    ASSERT(IS_FCS_CYCLE(Cycle));
    FM3_CRG->CSV_CTL &= 0x0fff;
    FM3_CRG->CSV_CTL |= (Cycle<<12);
}

/*!
 ******************************************************************************
 ** \brief Get CSV status
 **
 ** \return Status of Sub CSV function
 ** \retval CSV_SUB_FAIL A sub clock failure has been detected.
 ** \retval CSV_MAIN_FAIL A main clock failure has been detected.
 ******************************************************************************
 */
uint8_t CSV_GetCSVFailCause(void)
{
    return FM3_CRG->CSV_STR;
}


/*!
 ******************************************************************************
 ** \brief Frequency lower detection window setting
 **
 ** \param Limit limit value
 ******************************************************************************
 */
void CSV_SetFCSDetectLower(uint16_t Limit) 
{
    FM3_CRG->FCSWL_CTL = Limit;  
}

/*!
 ******************************************************************************
 ** \brief Frequency upper detection window setting
 **
 ** \param Limit Limit value
 ******************************************************************************
 */
void CSV_SetFCSDetectUpper(uint16_t Limit) 
{
    FM3_CRG->FCSWH_CTL = Limit;        
}

/*!
 ******************************************************************************
 ** \brief Get the counter value of frequency detection using the main clock.
 **
 ** \return Frequency detection counter value
 ******************************************************************************
 */
uint16_t CSV_GetFCSDetectCount(void) 
{
    return FM3_CRG->FCSWD_CTL;        
}

/*!
 ******************************************************************************
 ** \brief CSV interrupt handler
 ******************************************************************************
 */
void CSV_IntHandler(void)
{
    bFM3_CRG_INT_CLR_FCSC = 1;
    CSVIntCallback();
}

#endif

/*****************************************************************************/
/* END OF FILE */
