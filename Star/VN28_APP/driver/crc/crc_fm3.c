/*!
 ******************************************************************************
 **
 ** \file crc_fm3.c
 **
 ** \brief this file provides both hardware CRC16 and CRC32 arithmetic drivers
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-24
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

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "crc_fm3.h"

#if (PERIPHERAL_ENABLE_CRC == ON) && (PERIPHERAL_AVAILABLE_CRC == ON)
/*---------------------------------------------------------------------------*/
/* CRCCR bit definition                                                      */
/*---------------------------------------------------------------------------*/
#define CR_INT           0x01U
#define CR_CRC32         0x02U
#define CR_LTLEND        0x04U
#define CR_LSBFST        0x08U
#define CR_CRCLTE        0x10U
#define CR_CRCLSF        0x20U
#define CR_FXOR          0x40U

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/
static uint32_t  CRC_WriteCRCData (CRC_DataInfoT *pCRCDataInfo, uint32_t InitVal,
                                  uint8_t CR);

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Calculate CRC32 code
 **
 ** \param pCRCConfigInfo point to CRC configuration information structure
 ** \param pCRCDataInfo   point to CRC data information structure
 ** \param CRCInitVal32   CRC initialization value
 **
 ** \return CRC32 code
 **
 ******************************************************************************
 */
uint32_t  CRC_CalcCRC32 (CRC_ConfigInfoT *pCRCConfigInfo, CRC_DataInfoT *pCRCDataInfo, 
                        uint32_t CRCInitVal32)
{
    uint8_t regcr = 0;
    uint32_t crccode = 0;
    
    /* Check the parameter */
    ASSERT(IS_CRC_FINALXOR(pCRCConfigInfo->CrcInfoBit.FinalXor));
    ASSERT(IS_CRC_RESULT_BITORDER(pCRCConfigInfo->CrcInfoBit.ResultBitOrder));
    ASSERT(IS_CRC_RESULT_BYTEORDER(pCRCConfigInfo->CrcInfoBit.ResultByteOrder));
    ASSERT(IS_CRC_BITORDER(pCRCConfigInfo->CrcInfoBit.BitOrder));
    ASSERT(IS_CRC_BITORDER(pCRCConfigInfo->CrcInfoBit.ByteOrder));
    
    /*
     *  Set value of CRCCR
     */
    /* CRC32, INIT */
    regcr = (CR_CRC32 | CR_INT);
    
    regcr |=  (uint8_t)((pCRCConfigInfo->CrcInfoBit.FinalXor        << 6) |
                        (pCRCConfigInfo->CrcInfoBit.ResultBitOrder  << 5) |
                        (pCRCConfigInfo->CrcInfoBit.ResultByteOrder << 4) |
                        (pCRCConfigInfo->CrcInfoBit.BitOrder        << 3) |
                        (pCRCConfigInfo->CrcInfoBit.ByteOrder       << 2));
    /* Write CRC data */
    crccode = CRC_WriteCRCData(pCRCDataInfo, CRCInitVal32, regcr);
    
    return crccode;
} 

/*!
 ******************************************************************************
 ** \brief Calculate CRC16 code
 **
 ** \param pCRCConfigInfo point to CRC configuration information structure
 ** \param pCRCDataInfo   point to CRC data information structure
 ** \param CRCInitVal16   CRC initialization value
 **
 ** \return CRC16 code
 **
 ******************************************************************************
 */
uint16_t  CRC_CalcCRC16 (CRC_ConfigInfoT *pCRCConfigInfo, CRC_DataInfoT *pCRCDataInfo, 
                        uint16_t CRCInitVal16)
{
    uint8_t regcr = 0;
    uint32_t crccode = 0;
    
    /* Check the parameter */
    ASSERT(IS_CRC_FINALXOR(pCRCConfigInfo->CrcInfoBit.FinalXor));
    ASSERT(IS_CRC_RESULT_BITORDER(pCRCConfigInfo->CrcInfoBit.ResultBitOrder));
    ASSERT(IS_CRC_RESULT_BYTEORDER(pCRCConfigInfo->CrcInfoBit.ResultByteOrder));
    ASSERT(IS_CRC_BITORDER(pCRCConfigInfo->CrcInfoBit.BitOrder));
    ASSERT(IS_CRC_BITORDER(pCRCConfigInfo->CrcInfoBit.ByteOrder));
    
    /*
     *  Set value of CRCCR
     */
    /* CRC16, INIT */
    regcr &= ~CR_CRC32;
    regcr |= CR_INT;
    regcr |=  (uint8_t)((pCRCConfigInfo->CrcInfoBit.FinalXor        << 6) |
                        (pCRCConfigInfo->CrcInfoBit.ResultBitOrder  << 5) |
                        (pCRCConfigInfo->CrcInfoBit.ResultByteOrder << 4) |
                        (pCRCConfigInfo->CrcInfoBit.BitOrder        << 3) |
                        (pCRCConfigInfo->CrcInfoBit.ByteOrder       << 2));
    /* Write CRC data */
    crccode = CRC_WriteCRCData(pCRCDataInfo, (uint32_t)CRCInitVal16, regcr);
    
    /* Check CRC result output format */
    if (pCRCConfigInfo->CrcInfoBit.ResultByteOrder == CRC_RESULT_BYTEORDER_BIG)
    {
        /* read bit15-bit0 */
        crccode = (uint16_t)crccode;
        
    }
    else
    {
        /* read bit31-bit16 */
        crccode = (uint16_t)(crccode>>16);
    }
    
    return (uint16_t)crccode;
} 

/*!
 ******************************************************************************
 ** \brief Write CRC data
 **
 ** \param pCRCDataInfo point to CRC data information structure
 ** \param InitVal CRC initialization value 
 ** \param CR CRC register value
 **
 ** \return CRC data register value (CRCR)
 **
 ******************************************************************************
 */
static uint32_t  CRC_WriteCRCData (CRC_DataInfoT *pCRCDataInfo, uint32_t InitVal, 
                                  uint8_t CR)
{
    uint32_t i;
    
    /* Check the parameter */
    ASSERT(IS_CRC_DATA_WIDTH (pCRCDataInfo->Width));
    ASSERT(IS_CRC_DATA_POINTER (pCRCDataInfo->pData));
    ASSERT(IS_CRC_DATA_NUM (pCRCDataInfo->Num));
              
    /* write initial value */
    FM3_CRC->CRCINIT = InitVal;
    
    /* init (write CRCCR) */
    FM3_CRC->CRCCR = CR;
    
    /* write data */
    if (pCRCDataInfo->Width == CRC_INDATA_W_8) 
    {
        for (i = 0; i < pCRCDataInfo->Num; i++) 
        {
            FM3_CRC->CRCINLL = ((uint8_t*)pCRCDataInfo->pData)[i]; /* write CRC data with byte align */
        }
        
    } 
    else if (pCRCDataInfo->Width == CRC_INDATA_W_16) 
    {
        for (i = 0; i < pCRCDataInfo->Num; i++) 
        {
            FM3_CRC->CRCINL = ((uint16_t*)pCRCDataInfo->pData)[i]; /* write CRC data with half-word align */
        }
        
    } 
    else 
    {
        for (i = 0; i < pCRCDataInfo->Num; i++) 
        {
            FM3_CRC->CRCIN  = ((uint32_t*)pCRCDataInfo->pData)[i]; /* write CRC data with word align */
        }
        
    }  
    return FM3_CRC->CRCR;
}

/*****************************************************************************/

#endif

/* END OF FILE */
                
