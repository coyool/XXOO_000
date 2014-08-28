
/*!
 ******************************************************************************
 **
 ** \file workflash.h
 **
 ** \brief head file of flash API
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-21
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 */

#ifndef __WORKFLASH_FM3_H_
#define __WORKFLASH_FM3_H_

#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_WORKFLASH == OFF)
    #if (PERIPHERAL_ENABLE_WORKFLASH == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif


#if (PERIPHERAL_AVAILABLE_WORKFLASH == ON) && (PERIPHERAL_ENABLE_WORKFLASH == ON)

/*---------------------------------------------------------------------------*/
/* \brief Flash erase commands                                           */
/*---------------------------------------------------------------------------*/
#define FLASH_CHIP_ERASE_1   0x00AA   // Flash erase commands
#define FLASH_CHIP_ERASE_2   0x0055
#define FLASH_CHIP_ERASE_3   0x0080
#define FLASH_CHIP_ERASE_4   0x00AA
#define FLASH_CHIP_ERASE_5   0x0055
#define FLASH_CHIP_ERASE_6   0x0010

#define FLASH_SECTOR_ERASE_1 0x00AA   // sector erase commands
#define FLASH_SECTOR_ERASE_2 0x0055
#define FLASH_SECTOR_ERASE_3 0x0080
#define FLASH_SECTOR_ERASE_4 0x00AA
#define FLASH_SECTOR_ERASE_5 0x0055
#define FLASH_SECTOR_ERASE_6 0x0030
/*---------------------------------------------------------------------------*/
/* \brief Flash write commands                                               */
/*---------------------------------------------------------------------------*/
#define FLASH_WRITE_1        0x00AA   
#define FLASH_WRITE_2        0x0055 
#define FLASH_WRITE_3        0x00A0
/*---------------------------------------------------------------------------*/
/* \brief Flash read/reset commands                                          */
/*---------------------------------------------------------------------------*/
#define FLASH_READ_RESET     0x00F0   
/*---------------------------------------------------------------------------*/
/* \brief Flash sector erase suspend/restart commands                        */
/*---------------------------------------------------------------------------*/
#define FLASH_SECTOR_ERASE_SUSPEND 0x00B0 // Sector erase suspend command
#define FLASH_SECTOR_ERASE_RESTART 0x0030 // Sector erase restart (resume) command
/*---------------------------------------------------------------------------*/
/* \brief Flash sector address                                               */
/*---------------------------------------------------------------------------*/
#define FLASH_ADDRESS_SA0    0x200C0000     // address of sector SA0
#define FLASH_ADDRESS_SA1    0x200C2000     // address of sector SA1
#define FLASH_ADDRESS_SA2    0x200C4000     // address of sector SA2
#define FLASH_ADDRESS_SA3    0x200C6000     // address of sector SA3
  
#define FLASH_STR_ADDR      (0x200c0000)  
#define FLASH_END_ADDR      (0x200c8000)  
/*---------------------------------------------------------------------------*/
/* \brief Flash hardware sequence flags                                      */
/*---------------------------------------------------------------------------*/
#define FLASH_DQ7 0x0080    // data polling flag bit (DPOL) position
#define FLASH_DQ6 0x0040    // data toggle flag bit (TOGG) position
#define FLASH_DQ5 0x0020    // time limit exceeding flag bit (TLOV) position
#define FLASH_DQ3 0x0008    // sector erase timer flag bit (SETI) position
/*---------------------------------------------------------------------------*/
/* \brief Flash registers' bit                                               */
/*---------------------------------------------------------------------------*/
#define WFLASH_WFASZR_ASZ	0x01  //WFASZR
#define WFLASH_WFRWTR_RWT       0x01   //WFRWTR
#define WFLASH_WFSTR_RDY        0x01   //WFSTR
#define WFLASH_WFSTR_HNG        0x02  
/*---------------------------------------------------------------------------*/
/* \brief Flash operation mode                                       */
/*---------------------------------------------------------------------------*/
#define WFLASH_PGM_MODE         0x00
#define WFLASH_ROM_MODE         0x01
/*---------------------------------------------------------------------------*/
/* \brief Flash register check bit mask                                      */
/*---------------------------------------------------------------------------*/
#define FLASH_CHK_DPOL_MASK         (uint8_t)0x0080
#define FLASH_CHK_TOGG_MASK         (uint8_t)0x0040
#define FLASH_CHK_TLOV_MASK         (uint8_t)0x0020
/*---------------------------------------------------------------------------*/
/* \brief Flash command address                                      */
/*---------------------------------------------------------------------------*/
#define ADDR_AA8           0x0AA8
#define ADDR_554           0x0554 
   
#define CHIPERASE_ADDR_AA8 ((FLASH_STR_ADDR&0xFFFFF000)|ADDR_AA8)
#define CHIPERASE_ADDR_554 ((FLASH_STR_ADDR&0xFFFFF000)|ADDR_554)  


/*---------------------------------------------------------------------------*/
/* \brief Flash return parameters                                            */
/*---------------------------------------------------------------------------*/
#define  FLASH_RET_INVALID_PARA      1
#define  FLASH_RET_OK                0
#define  FLASH_RET_ABNORMAL          2

#define  FLASH_CHK_TOGG_NORMAL       0
#define  FLASH_CHK_TOGG_ABNORMAL     1
#define  FLASH_CHK_DPOL_NORMAL       0
#define  FLASH_CHK_DPOL_ABNORMAL     1
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*Operate*/
void     WFlash_ChipErase    (void);
void     WFlash_ReadResetCmd(uint16_t *pResetSecAddr);
void     WFlash_Suspend  (uint32_t  sec_addr);
void     WFlash_Restart  (uint32_t  sec_addr);
uint8_t  WFlash_Read8bit    (uint8_t  *pReadAddr);
uint16_t WFlash_Read16bit   (uint16_t *pReadAddr);
uint8_t  WFlash_SectorErase (uint32_t *pSecAddr );
uint8_t  WFlash_Write8bit   (uint8_t  *pWriteAddr, uint8_t  WriteData );
uint8_t  WFlash_Write16bit  (uint16_t *pWriteAddr, uint16_t WriteData ); 
uint8_t  WFlash_Write32bit  (uint32_t *pWriteAddr, uint32_t WriteData );

#ifdef __cplusplus
}
#endif

#endif /* PERIPHERAL_AVAILABLE_WORKFLASH */


#endif /* __WORKFLASH_FM3_H_ */


