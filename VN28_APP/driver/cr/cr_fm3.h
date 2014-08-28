/*!
 ******************************************************************************
 **
 ** \file cr_fm3.h
 **
 ** \brief head file of CR trimming driver
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
 
#ifndef _CR_FM3_H_
#define _CR_FM3_H_
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_CR == OFF)
    #if (PERIPHERAL_ENABLE_CR == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_CR == ON) && (PERIPHERAL_AVAILABLE_CR == ON)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

typedef enum 
{
    CR_FRQ_DIV_4 = 0,     //!< \brief Frequency divsion ratio 1/4 
    CRC_FRQ_DIV_8,        //!< \brief Frequency divsion ratio 1/8
    CRC_FRQ_DIV_16,       //!< \brief Frequency divsion ratio 1/16
    CRC_FRQ_DIV_32,       //!< \brief Frequency divsion ratio 1/32
#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7)   
    CRC_FRQ_DIV_64,
    CRC_FRQ_DIV_128,
    CRC_FRQ_DIV_256,
    CRC_FRQ_DIV_512,
#endif
    
}CR_FreqDivT;  

#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7)   
/*! \brief Check parameter */
#define IS_CRC_FQR_DIV(val)                    ((val == CR_FRQ_DIV_4)     \
                                                ||(val == CRC_FRQ_DIV_8)  \
                                                ||(val == CRC_FRQ_DIV_16) \
                                                ||(val == CRC_FRQ_DIV_32) \
                                                ||(val == CRC_FRQ_DIV_64) \
                                                ||(val == CRC_FRQ_DIV_128)\
                                                ||(val == CRC_FRQ_DIV_256)\
                                                ||(val == CRC_FRQ_DIV_512))
#else
/*! \brief Check parameter */
#define IS_CRC_FQR_DIV(val)                    ((val == CR_FRQ_DIV_4) \
                                                ||(val == CRC_FRQ_DIV_8) \
                                                ||(val == CRC_FRQ_DIV_16) \
                                                ||(val == CRC_FRQ_DIV_32))
#endif
                                           


/*! \brief Disable write protection mode of MCR_FTRM  */
#define CR_UNLOCK							((uint32_t)0x1acce554)


/*! \brief flash address of CR trimming data  */
#define CR_TRIMMING_DATA             		*((volatile unsigned int*)(0x00101004UL))

                             
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void CR_RegUnLock(void);
void CR_RegLock(void);
void CR_SetTrimData(int16_t TrimData);
void CR_SetFreqDiv(CR_FreqDivT Div);
int16_t CR_GetTrimData(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _CR_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */







