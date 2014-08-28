/*!
 ******************************************************************************
 **
 ** \file dac_fm3.h
 **
 ** \brief DAC drivers head file
 **
 ** \author FSDC AE Team
 **
 ** \version V0.1
 **
 ** \date 2012-10-12
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(ChengDu) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */

#ifndef _DAC_FM3_H_
#define _DAC_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/

#if (PERIPHERAL_AVAILABLE_DAC == ON) && (PERIPHERAL_ENABLE_DAC == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#define     DAC10_CH0        (0)     //!< DAC channel 0
#define     DAC10_CH1        (1)     //!< DAC channel 1

/*! \brief Check select parameter */ 
#define IS_DAC_CH_VALID(Ch)      ((Ch == DAC10_CH0) \
                                 ||(Ch == DAC10_CH1))

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void DAC_Enable(uint8_t Ch);                   //enable DAC function
void DAC_Disable(uint8_t Ch);                  //disable DAC function
void DAC_SetDigitalVal(uint8_t Ch, uint16_t ConvertValue);  //set DAC digital input

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_AVAILABLE_DAC */


#endif /* _DAC_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */

