/*!
 ******************************************************************************
 **
 ** \file dualflash_fm3.c
 **
 ** \brief dualflash module driver.
 **
 ** \author FSLA AE Team
 **
 ** \version V0.1
 **
 ** \date 2012-12-30
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
#ifndef _DUALFLASH_FM3_H_
#define _DUALFLASH_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_DUALFLASH == ON) && (PERIPHERAL_ENABLE_DUALFLASH == ON)

#define     FLASH_LOW_BANK1_START_ADDR              (uint8_t*)(0x00000000)
#define     FLASH_LOW_BANK1_END_ADDR                (uint8_t*)(0x00003FFF)
#define     FLASH_LOW_BANK2_START_ADDR              (uint8_t*)(0x00200000)
#define     FLASH_LOW_BANK2_END_ADDR                (uint8_t*)(0x00208000)
#define     FLASH_HIGH_BANK_START_ADDR              (uint8_t*)(0x00004000)
#define     FLASH_HIGH_BANK_END_ADDR                (uint8_t*)(0x00080000)

#define     IS_FLASH_ADDR_VALID(Addr)               ((Addr >= FLASH_LOW_BANK1_START_ADDR && Addr<=FLASH_LOW_BANK1_END_ADDR) \
                                                   ||(Addr >= FLASH_LOW_BANK2_START_ADDR && Addr<=FLASH_LOW_BANK2_END_ADDR) \
                                                   ||(Addr >= FLASH_HIGH_BANK_START_ADDR && Addr<=FLASH_HIGH_BANK_END_ADDR))

#define     DFLASH_RET_OK                0
#define     DFLASH_RET_INVALID_PARA      1
#define     DFLASH_RET_ABNORMAL          2

#define     DFLASH_CHK_TOGG_NORMAL       0
#define     DFLASH_CHK_TOGG_ABNORMAL     1

#define     DFLASH_CHK_DPOL_NORMAL       0
#define     DFLASH_CHK_DPOL_ABNORMAL     1

#define     DFLASH_CHK_DPOL_MASK         (uint8_t)0x80
#define     DFLASH_CHK_TOGG_MASK         (uint8_t)0x40
#define     DFLASH_CHK_TLOV_MASK         (uint8_t)0x20

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
uint8_t DFlash_SectorErase(uint8_t* p_sec_addr);
uint8_t DFlash_ByteWrite( uint8_t*  pWriteAddr, uint8_t WriteData ) ;
#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_AVAILABLE_DUALFLASH  */

#endif /* _DUALFLASH_FM3_H_  */
