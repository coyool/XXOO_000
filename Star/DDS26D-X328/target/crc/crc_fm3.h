/*!
 ******************************************************************************
 **
 ** \file crc_fm3.h
 **
 ** \brief CRC drivers head file
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
 **
 ** \note Other information.
 **
 ******************************************************************************
 */ 
 
#ifndef _CRC_FM3_H_
#define _CRC_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_CRC == OFF)
    #if (PERIPHERAL_ENABLE_CRC == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_CRC == ON) && (PERIPHERAL_AVAILABLE_CRC == ON)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/* CRC result bit xor type */
/*! \brief Result bit xor on */
#define CRC_FINALXOR_OFF                     0
/*! \brief Result bit xor off */
#define CRC_FINALXOR_ON                      1  
/*! \brief Check CRC result bit */
#define IS_CRC_FINALXOR(xor)                   ((xor == CRC_FINALXOR_ON) || \
                                                (xor == CRC_FINALXOR_OFF))

/* CRC result bit order type  */
/*! \brief MSB bit order  */
#define CRC_RESULT_BITORDER_MSB              0
/*! \brief LSB bit order  */
#define CRC_RESULT_BITORDER_LSB              1
/*! \brief Check CRC result bit order  */
#define IS_CRC_RESULT_BITORDER(order)          ((order == CRC_RESULT_BITORDER_MSB) || \
                                                  (order == CRC_RESULT_BITORDER_LSB))
/* CRC result byte order type */
/*! \brief Big endian byte order */
#define CRC_RESULT_BYTEORDER_BIG             0
/*! \brief Little endian byte order */
#define CRC_RESULT_BYTEORDER_LITTLE          1
/*! \brief Check CRC result byte order */
#define IS_CRC_RESULT_BYTEORDER(order)            ((order == CRC_RESULT_BYTEORDER_BIG) || \
                                                   (order == CRC_RESULT_BYTEORDER_LITTLE))       
                                                                                                  
/* CRC bit order type */
/*! \brief MSB bit order  */
#define CRC_BITORDER_MSB                     0
/*! \brief LSB bit order  */
#define CRC_BITORDER_LSB                     1
/*! \brief Check CRC bit order */
#define IS_CRC_BITORDER(order)                   ((order == CRC_BITORDER_MSB) || \
                                                  (order == CRC_BITORDER_LSB))                                                                                            
                                                 
/* CRC byte order type */
/*! \brief Big endian byte order */
#define CRC_BYTEORDER_BIG                    0
/*! \brief Little endian byte order */
#define CRC_BYTEORDER_LITTLE                 1
/*! \brief Check parameter */
#define IS_CRC_BYTEORDER(order)                  ((order == CRC_BYTEORDER_BIG) || \
                                                  (order == CRC_BYTEORDER_LITTLE))  

/* CRC mode type */
/*! \brief CRC16 mode */
#define CRC_MODE_CRC16                       0
/*! \brief CRC32 mode */
#define CRC_MODE_CRC32                       1
/*! \brief Check CRC mode */
#define IS_CRC_MODE(mode)                      ((mode == CRC_MODE_CRC16) || \
                                                (mode == CRC_MODE_CRC32))  

/* Data width type */
/*! \brief Byte */
#define CRC_INDATA_W_8                       0
/*! \brief Half word */
#define CRC_INDATA_W_16                      1
/*! \brief Word */
#define CRC_INDATA_W_32                      2
/*! \brief Check data width */
#define IS_CRC_DATA_WIDTH(width)                ((width == CRC_INDATA_W_8) || \
                                                 (width == CRC_INDATA_W_16)|| \
                                                 (width == CRC_INDATA_W_32))  
/*! \brief Check data pointer */
#define IS_CRC_DATA_POINTER(pData)             (pData != NULL)

/*! \brief Check data size */
#define IS_CRC_DATA_NUM(num)                   ( num != 0)                                                     
                                                        
/*!
 ******************************************************************************
 ** \brief CRC configuration infomation structure
 ******************************************************************************
 */    
typedef union
{
    uint8_t  CrcInfoByte;                //!< CRC information byte    
    struct {
        uint8_t FinalXor : 1;            //!< CRC final XOR    
        uint8_t ResultBitOrder :1;       //!< CRC result bit order 
        uint8_t ResultByteOrder : 1;     //!< CRC result byte order 
        uint8_t BitOrder : 1;            //!< CRC input bit order 
        uint8_t ByteOrder : 1;           //!< CRC input byte order 
        uint8_t : 3;                     //!< Reserved 
    } CrcInfoBit;
} CRC_ConfigInfoT;


/*!
 ******************************************************************************
 ** \brief CRC data infomation structure
 ******************************************************************************
 */
typedef struct
{
    uint8_t* pData;                      //!< CRC data address
    uint8_t  Width;                      //!< CRC data width
    uint8_t  Num;                        //!< CRC data size
} CRC_DataInfoT;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
uint32_t  CRC_CalcCRC32 (CRC_ConfigInfoT *pCRCConfigInfo, CRC_DataInfoT *pCRCDataInfo, 
                        uint32_t CRCInitVal32);
uint16_t  CRC_CalcCRC16 (CRC_ConfigInfoT *pCRCConfigInfo, CRC_DataInfoT *pCRCDataInfo, 
                        uint16_t CRCInitVal16);
#ifdef __cplusplus
}
#endif

#endif

#endif /* _CRC_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */                        

