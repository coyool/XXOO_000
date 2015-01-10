/*!
 ******************************************************************************
 **
 ** \file mainflash_fm3.h
 **
 ** \author FSLA AE Team
 **
 ** \version V0.21
 **
 ** \date 2012-08-07
 **
 ** \brief main flash driver header file
 **       
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 */

#ifndef _MAINFLASH_FM3_H_
#define _MAINFLASH_FM3_H_
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_MAINFLASH == OFF)
    #if (PERIPHERAL_ENABLE_MAINFLASH == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_AVAILABLE_MAINFLASH == ON) && (PERIPHERAL_ENABLE_MAINFLASH == ON) 

#if (MCU_SERIES != DEVICE_SERIES_MB9AF13X) && (MCU_SERIES != DEVICE_SERIES_MB9AFA3X)
    #define     MFLASH_CODE1                 0x1550
    #define     MFLASH_CODE2                 0x0AA8
#else
    #define     MFLASH_CODE1                 0x0AA8
    #define     MFLASH_CODE2                 0x0554
#endif

#define     MFLASH_RET_OK                0
#define     MFLASH_RET_INVALID_PARA      1
#define     MFLASH_RET_ABNORMAL          2

#define     MFLASH_CHK_TOGG_NORMAL       0
#define     MFLASH_CHK_TOGG_ABNORMAL     1

#define     MFLASH_CHK_DPOL_NORMAL       0
#define     MFLASH_CHK_DPOL_ABNORMAL     1

#define     MFLASH_CHK_DPOL_MASK         (uint16_t)0x0080
#define     MFLASH_CHK_TOGG_MASK         (uint16_t)0x0040
#define     MFLASH_CHK_TLOV_MASK         (uint16_t)0x0020

#define     MFLASH_END_ADDR              (0x000FFFFF)

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
uint8_t MFlash_SectorErase(uint16_t* p_sec_addr);
uint8_t MFlash_Write( uint16_t*  pWriteAddr, uint32_t WriteData ) ;
uint8_t MFlash_HWrite( uint16_t*  pWriteAddr, uint16_t WriteData ) ;

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_AVAILABLE_MAINFLASH */

#endif /* _MAINFLASH_FM3_H_ */
