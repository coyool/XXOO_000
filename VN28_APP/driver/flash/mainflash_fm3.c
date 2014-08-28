/*!
 ******************************************************************************
 **
 ** \file mainflash_fm3.c
 **
 ** \author FSLA AE Team
 **
 ** \version V0.1
 **
 ** \date 2013-02-18
 **
 ** \brief This file provides an example to configure IO for GPIO, ADC and 
 **        peripheral function.
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

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "mainflash_fm3.h"

#if (PERIPHERAL_AVAILABLE_MAINFLASH == ON) && (PERIPHERAL_ENABLE_MAINFLASH == ON) 

/*!
 ******************************************************************************
 ** \brief Read a half word data from Flash
 ** \param addr Pointer to read data address
 ******************************************************************************
 */
#define Flash_Read(addr)        *(volatile uint16_t*)((uint32_t)(addr))

/*!
 ******************************************************************************
 ** \brief Wirte a half word data into Flash
 ** \param addr Pointer to read data address
 ** \param data Write data
 ******************************************************************************
 */
#define Flash_Write(addr, data) *(volatile uint16_t*)((uint32_t)(addr)) = ( uint16_t)(data)

#if defined ( __ICCARM__ )
__ramfunc 
#endif
static uint8_t MFlash_CheckToggle( uint16_t* pAddr );

#if defined ( __ICCARM__ )
__ramfunc 
#endif 
static void MFlash_ReadResetCmd(uint16_t* pResetSecAddr);

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
#if defined ( __ICCARM__ )
__ramfunc 
#endif 
static void MFlash_ReadResetCmd(uint16_t* pResetSecAddr)
{
    uint8_t  dummy;

    /*  issue read/reset command    */
    Flash_Write(0x0000, 0xF0U) ;
    dummy = Flash_Read(pResetSecAddr) ;
    if(dummy)  /* avoid warning */  
        ;
    return ;
}

/*!
 ******************************************************************************
 ** \brief Flash sector erase       
 **
 ** \param pEraseSecAddr address of flash sector  
 **
 ** \return Operation status
 ** \retval MFLASH_RET_OK
 ** \retval MFLASH_RET_ABNORMAL
 ** \retval MFLASH_RET_INVALID_PARA
 **
 ******************************************************************************
 */
#if defined ( __ICCARM__ )
__ramfunc 
#endif 
uint8_t MFlash_SectorErase(uint16_t* pSecAddr)
{
    uint8_t  dummy;
    uint8_t ret = MFLASH_RET_OK;
    if(pSecAddr == (uint16_t*)0 || pSecAddr > (uint16_t*)MFLASH_END_ADDR)
        ret = MFLASH_RET_INVALID_PARA;
    else
    {
        /* CPU programming mode setting */
        FM3_FLASH_IF->FASZR = 0x01;
        dummy = FM3_FLASH_IF->FASZR;
        if(dummy) ;
        Flash_Write(MFLASH_CODE1, 0x00AA);
        Flash_Write(MFLASH_CODE2, 0x0055);
        Flash_Write(MFLASH_CODE1, 0x0080);
        Flash_Write(MFLASH_CODE1, 0x00AA);
        Flash_Write(MFLASH_CODE2, 0x0055);
        Flash_Write(pSecAddr, 0x0030);

         /*  if execution result of the automatic algorithm of flash memory is abnormally completed  */
        if( MFlash_CheckToggle(pSecAddr) == MFLASH_CHK_TOGG_ABNORMAL)
        {
            /*  sending the read/reset command to the reset sector  */
            MFlash_ReadResetCmd(pSecAddr) ;

            /*  return flash operation abnormally   */
            ret  = MFLASH_RET_ABNORMAL ;
        }

        /*  CPU ROM mode setting    */
        FM3_FLASH_IF->FASZR = 2; 
        dummy = FM3_FLASH_IF->FASZR;
        
    }
    return ret;
}

/*!
 ******************************************************************************
 ** \brief Flash word write       
 **
 ** \param pWriteAddr address of flash data
 ** \param WriteData write data
 **
 ** \return Operation status
 ** \retval MFLASH_RET_OK
 ** \retval MFLASH_RET_ABNORMAL
 ** \retval MFLASH_RET_INVALID_PARA
 **
 ******************************************************************************
 */
#if defined ( __ICCARM__ )
__ramfunc 
#endif 
uint8_t MFlash_Write( uint16_t*  pWriteAddr, uint32_t WriteData ) 
{
    uint8_t   ucRetValue  = MFLASH_RET_OK ;
    uint8_t   cnt=2, dummy;
    if(pWriteAddr == (uint16_t*)0 || pWriteAddr > (uint16_t*)MFLASH_END_ADDR){
        /*  return parameter invalid    */
        ucRetValue  = MFLASH_RET_INVALID_PARA ;
    }else{
        /*  CPU programming mode setting    */
        FM3_FLASH_IF->FASZR = 1; 
        dummy = FM3_FLASH_IF->FASZR;
        if(dummy)   /* avoid warning */  
            ;  
        for(;cnt;cnt--)
        {
            /*  issue write command   */
            Flash_Write(MFLASH_CODE1, 0x00AA) ;
            Flash_Write(MFLASH_CODE2, 0x0055) ;
            Flash_Write(MFLASH_CODE1, 0x00A0) ;
            Flash_Write(pWriteAddr, (uint16_t)WriteData);
            
            /*  execution result of the automatic algorithm of flash memory is abnormally complete or verify error  */
            if(( MFlash_CheckToggle(pWriteAddr) == MFLASH_CHK_TOGG_ABNORMAL) ||
              ( Flash_Read(pWriteAddr) != WriteData))
            {
                /*  issue read/reset command to reset sector    */
                MFlash_ReadResetCmd(pWriteAddr) ;
    
                /*  return flash operation abnormally   */
                ucRetValue  = MFLASH_RET_ABNORMAL ;
            }      
            /* Prepare next h-word write */
            pWriteAddr++;
            WriteData=WriteData>>16; 
        }
        /*  CPU ROM mode setting    */
        FM3_FLASH_IF->FASZR = 2; 
        dummy = FM3_FLASH_IF->FASZR;
    }

    return ucRetValue ;
}

/*!
 ******************************************************************************
 ** \brief Flash half-word write  
 **
 ** \param pWriteAddr address of flash data
 ** \param WriteData write data
 **
 ** \return Operation status
 ** \retval MFLASH_RET_OK
 ** \retval MFLASH_RET_ABNORMAL
 ** \retval MFLASH_RET_INVALID_PARA
 **
 ******************************************************************************
 */
#if defined ( __ICCARM__ )
__ramfunc 
#endif 
uint8_t MFlash_HWrite( uint16_t*  pWriteAddr, uint16_t WriteData ) 
{
    uint8_t   ucRetValue  = MFLASH_RET_OK ;
    uint8_t   cnt=1, dummy;
    if(pWriteAddr == (uint16_t*)0 || pWriteAddr > (uint16_t*)MFLASH_END_ADDR){
        /*  return parameter invalid    */
        ucRetValue  = MFLASH_RET_INVALID_PARA ;
    }else{
        /*  CPU programming mode setting    */
        FM3_FLASH_IF->FASZR = 1; 
        dummy = FM3_FLASH_IF->FASZR;
        if(dummy)
            ;   /* avoid warning */  
        for(;cnt;cnt--)
        {
            /*  issue write command   */
            Flash_Write(MFLASH_CODE1, 0x00AA) ;
            Flash_Write(MFLASH_CODE2, 0x0055) ;
            Flash_Write(MFLASH_CODE1, 0x00A0) ;
            Flash_Write(pWriteAddr, (uint16_t)WriteData);
            
            /*  execution result of the automatic algorithm of flash memory is abnormally complete or verify error  */
            if(( MFlash_CheckToggle(pWriteAddr) == MFLASH_CHK_TOGG_ABNORMAL) ||
              ( Flash_Read(pWriteAddr) != WriteData))
            {
                /*  issue read/reset command to reset sector    */
                MFlash_ReadResetCmd(pWriteAddr) ;
    
                /*  return flash operation abnormally   */
                ucRetValue  = MFLASH_RET_ABNORMAL ;
            }      
            /* Prepare next h-word write */
            pWriteAddr++;
            WriteData=WriteData>>16; 
        }
        /*  CPU ROM mode setting    */
        FM3_FLASH_IF->FASZR = 2; 
        dummy = FM3_FLASH_IF->FASZR;
    }

    return ucRetValue ;
}

/*!
 ******************************************************************************
 ** \brief automatic algorithm of flash memory execution 
 **
 ** \param pAddr address of flash data
 **
 ** \return Operation status
 ** \retval MFLASH_RET_OK
 ** \retval MFLASH_RET_ABNORMAL
 ** \retval MFLASH_RET_INVALID_PARA
 **
 ******************************************************************************
 */
#if defined ( __ICCARM__ )
__ramfunc 
#endif 
static uint8_t MFlash_CheckToggle( uint16_t* pAddr )
{
    uint16_t   usSequenceFlag1, usSequenceFlag2 ;  /*  hardware sequence flag */
    uint8_t   ucRetValue  = MFLASH_CHK_TOGG_NORMAL ;

    /* set hardware sequence flag */
    usSequenceFlag1 = Flash_Read(pAddr) ;
    usSequenceFlag2 = Flash_Read(pAddr) ;
    /*  if automatic algorithm is executing */
    while((( usSequenceFlag1 ^ usSequenceFlag2) & MFLASH_CHK_TOGG_MASK) == MFLASH_CHK_TOGG_MASK)
    {
        /*  if exceeds the timing limit */
        if(( usSequenceFlag1 & MFLASH_CHK_TLOV_MASK) == MFLASH_CHK_TLOV_MASK)
        {
            /* set hardware sequence flag */
            usSequenceFlag1 = Flash_Read(pAddr) ;
            usSequenceFlag2 = Flash_Read(pAddr) ;

            /*  if automatic algorithm is executing */
            if((( usSequenceFlag1 ^ usSequenceFlag2) & MFLASH_CHK_TOGG_MASK) == MFLASH_CHK_TOGG_MASK)
            {
                /*  abnormally complete */
                ucRetValue  = MFLASH_CHK_TOGG_ABNORMAL ;

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
