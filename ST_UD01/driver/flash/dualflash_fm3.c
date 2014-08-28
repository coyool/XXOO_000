/*!
 ******************************************************************************
 **
 ** \file dualflash_fm3.c
 **
 ** \brief dual flash driver
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

#include "dualflash_fm3.h"

#if (PERIPHERAL_AVAILABLE_DUALFLASH == OFF)
    #if (PERIPHERAL_ENABLE_DUALFLASH == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_AVAILABLE_DUALFLASH == ON) && (PERIPHERAL_ENABLE_DUALFLASH == ON)

/*!
 ******************************************************************************
 ** \brief Read a half word data from Flash
 ** \param addr Pointer to read data address
 ******************************************************************************
 */
#define Flash_Read(addr)        *(volatile uint8_t*)((uint32_t)(addr))

/*!
 ******************************************************************************
 ** \brief Wirte a half word data into Flash
 ** \param addr Pointer to read data address
 ** \param data Write data
 ******************************************************************************
 */
#define Flash_Write(addr, data) *(volatile  uint8_t*)((uint32_t)(addr)) = ( uint8_t)(data)

static uint8_t DFlash_CheckToggle(uint8_t* p_sec_addr );
static uint8_t DFlash_ReadResetCmd(uint8_t* pResetSecAddr);

/******************************************************************************/
/* Notes:                                                                     */
/* The feature of Dual Flash is that High bank can erase/write low bank,      */
/* low bank can erase/write high bank.                                        */
/******************************************************************************/

#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Issue read/reset command
 **
 ** \param pResetSecAddr address of reset sector  
 **
 ** \return None
 **
 ******************************************************************************
 */
static uint8_t DFlash_ReadResetCmd(uint8_t* pResetSecAddr)
{
    uint8_t  dummy;

    /*  issue read/reset command    */
    Flash_Write(0x0000, 0xF0U) ;
    dummy = Flash_Read(pResetSecAddr) ;
    return dummy ;
}

#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Flash sector erase       
 **
 ** \param pEraseSecAddr address of flash sector  
 **
 ** \return Operation status
 ** \retval DFLASH_RET_OK
 ** \retval DFLASH_RET_ABNORMAL
 ** \retval DFLASH_RET_INVALID_PARA
 **
 ******************************************************************************
 */
uint8_t DFlash_SectorErase(uint8_t* p_sec_addr) 
{
    uint8_t ret = DFLASH_RET_OK;
    if(!IS_FLASH_ADDR_VALID(p_sec_addr))
        ret = DFLASH_RET_INVALID_PARA;
    else
    {
        Flash_Write(0xAA8, 0xAA);
        Flash_Write(0x554, 0x55);
        Flash_Write(0xAA8, 0x80);
        Flash_Write(0xAA8, 0xAA);
        Flash_Write(0x554, 0x55);
        Flash_Write(p_sec_addr, 0x30);

         /*  if execution result of the automatic algorithm of flash memory is abnormally completed  */
        if( DFlash_CheckToggle(p_sec_addr) == DFLASH_CHK_TOGG_ABNORMAL)
        {
            /*  sending the read/reset command to the reset sector  */
            DFlash_ReadResetCmd(p_sec_addr) ;

            /*  return flash operation abnormally   */
            ret  = DFLASH_RET_ABNORMAL ;
        }
        
    }
    return ret;
}

#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief Flash byte write       
 **
 ** \param pWriteAddr address of flash data
 ** \param WriteData write data
 **
 ** \return Operation status
 ** \retval DFLASH_RET_OK
 ** \retval DFLASH_RET_ABNORMAL
 ** \retval DFLASH_RET_INVALID_PARA
 **
 ******************************************************************************
 */
uint8_t DFlash_ByteWrite( uint8_t*  pWriteAddr, uint8_t WriteData ) 
{
    uint8_t   ucRetValue  = DFLASH_RET_OK ;
    uint8_t  cnt=1;
    if(!IS_FLASH_ADDR_VALID(pWriteAddr)){
        ucRetValue = DFLASH_RET_INVALID_PARA;
    }else{
        for(;cnt;cnt--)
        {
            /*  issue write command   */
            Flash_Write(0xAA8, 0xAA) ;
            Flash_Write(0x554, 0x55) ;
            Flash_Write(0xAA8, 0xA0) ;
            Flash_Write(pWriteAddr, (uint8_t)WriteData);
            
            /*  execution result of the automatic algorithm of flash memory is abnormally complete or verify error  */
            if(( DFlash_CheckToggle(pWriteAddr) == DFLASH_CHK_TOGG_ABNORMAL) ||
              ( Flash_Read(pWriteAddr) != WriteData))
            {
                /*  issue read/reset command to reset sector    */
                DFlash_ReadResetCmd(pWriteAddr) ;
    
                /*  return flash operation abnormally   */
                ucRetValue  = DFLASH_RET_ABNORMAL ;
            }      
            /* Prepare next h-word write */
            pWriteAddr++;
            WriteData=WriteData>>16; 
        }
    }

    return ucRetValue ;
}

#if defined ( __ICCARM__ )
#pragma location=".flashcode"
#endif
/*!
 ******************************************************************************
 ** \brief automatic algorithm of flash memory execution 
 **
 ** \param pAddr address of flash data
 **
 ** \return Operation status
 ** \retval DFLASH_RET_OK
 ** \retval DFLASH_RET_ABNORMAL
 ** \retval DFLASH_RET_INVALID_PARA
 **
 ******************************************************************************
 */
static uint8_t DFlash_CheckToggle( uint8_t* pAddr )
{
    uint8_t   usSequenceFlag1, usSequenceFlag2 ;  /*  hardware sequence flag */
    uint8_t   ucRetValue  = DFLASH_CHK_TOGG_NORMAL ;

    /* set hardware sequence flag */
    usSequenceFlag1 = Flash_Read(pAddr) ;
    usSequenceFlag2 = Flash_Read(pAddr) ;
    /*  if automatic algorithm is executing */
    while((( usSequenceFlag1 ^ usSequenceFlag2) & DFLASH_CHK_TOGG_MASK) == DFLASH_CHK_TOGG_MASK)
    {
        /*  if exceeds the timing limit */
        if(( usSequenceFlag1 & DFLASH_CHK_TLOV_MASK) == DFLASH_CHK_TLOV_MASK)
        {
            /* set hardware sequence flag */
            usSequenceFlag1 = Flash_Read(pAddr) ;
            usSequenceFlag2 = Flash_Read(pAddr) ;

            /*  if automatic algorithm is executing */
            if((( usSequenceFlag1 ^ usSequenceFlag2) & DFLASH_CHK_TOGG_MASK) == DFLASH_CHK_TOGG_MASK)
            {
                /*  abnormally complete */
                ucRetValue  = DFLASH_CHK_TOGG_ABNORMAL ;

                break;
            }
        }

        /* set hardware sequence flag */
        usSequenceFlag1 = Flash_Read(pAddr) ;
        usSequenceFlag2 = Flash_Read(pAddr) ;
    }

    return ucRetValue ;
}

#endif
