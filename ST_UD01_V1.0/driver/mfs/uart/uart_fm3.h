/*!
 ******************************************************************************
 **
 ** \file uart_fm3.h
 **
 ** \brief UART drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-09
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

#ifndef _UART_FM3_H_
#define _UART_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "..\mfs_fm3.h"

#if (PERIPHERAL_AVAILABLE_MFS == OFF)
    #if (PERIPHERAL_ENABLE_MFS_UART == ON)
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFS_UART == ON) && (PERIPHERAL_AVAILABLE_MFS == ON)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief MFS: UART mode */
#define MFS_MODE_UART                   0

/*! \brief Databit Length: 5 */
#define UART_DATABITS_5                 5
/*! \brief Databit Length: 6 */
#define UART_DATABITS_6                 6
/*! \brief Databit Length: 7 */
#define UART_DATABITS_7                 7
/*! \brief Databit Length: 8 */
#define UART_DATABITS_8                 8
/*! \brief Databit Length: 9 */
#define UART_DATABITS_9                 9

/*! \brief BIT Length switch case: 8 bit */
#define UART_ESCR_BITLEN_8   0
/*! \brief BIT Length switch case: 5 bit */
#define UART_ESCR_BITLEN_5   1
/*! \brief BIT Length switch case: 6 bit */
#define UART_ESCR_BITLEN_6   2
/*! \brief BIT Length switch case: 7 bit */
#define UART_ESCR_BITLEN_7   3
/*! \brief BIT Length switch case: 9 bit */
#define UART_ESCR_BITLEN_9   4

/*! \brief Stopbit Length: 1 */
#define UART_STOPBITS_1                 1
/*! \brief Stopbit Length: 2 */
#define UART_STOPBITS_2                 2
/*! \brief Stopbit Length: 3 */
#define UART_STOPBITS_3                 3
/*! \brief Stopbit Length: 4 */
#define UART_STOPBITS_4                 4

/*! \brief Parity mode: None */
#define UART_PARITY_NONE                0
/*! \brief Parity mode: Odd */
#define UART_PARITY_ODD                 1
/*! \brief Parity mode: Even */
#define UART_PARITY_EVEN                2

/*! \brief Bit order: MSB */
#define UART_BITORDER_MSB               1
/*! \brief Bit order: LSB */
#define UART_BITORDER_LSB               0

/*! \brief Inverted serial data format: NRZ */
#define UART_NRZ                   0
/*! \brief Inverted serial data format: Inverted NRZ */
#define UART_INV_NRZ               1

/*! \brief MFS UART Mininum FIFO Channel: 4 */
#define UART_Ch_FIFOMIN            0
#define UART_Ch_FIFOMAX            MFS_MAX_CH
/*! \brief MFS UART Max Channel Number */
#define UART_Ch_MAX                MFS_MAX_CH
/*! \brief FIFO max depth*/
#define UART_FIFO_MAX_DEPTH      16

/*! \brief SCR: Programmable Clear bit */
#define UART_SCR_UPCL      0x80
/*! \brief SCR: Receive interrupt enable bit */
#define UART_SCR_RIE       0x10
/*! \brief SCR: Transmit interrupt enable bit */
#define UART_SCR_TIE       0x08
/*! \brief SCR: Transmit bus idle interrupt enable bit */
#define UART_SCR_TBIE      0x04
/*! \brief SCR: Receive operation enable bit */
#define UART_SCR_RXE       0x02
/*! \brief SCR: Transmit operation enable bit */
#define UART_SCR_TXE       0x01

/*! \brief SMR: Wake-up control bit */
#define UART_SMR_WUCR  0x10
/*! \brief SMR: Stop bit length select bit */
#define UART_SMR_SBL   0x08
/*! \brief SMR: Transfer direction select bit */
#define UART_SMR_BDS   0x04
/*! \brief SMR: Serial data output enable bit */
#define UART_SMR_SOE   0x01

/*! \brief SSR: Receive error flag clear bit */
#define UART_SSR_REC       0x80
/*! \brief SSR: Parity error flag bit */
#define UART_SSR_PE        0x20
/*! \brief SSR: Framing error flag bit */
#define UART_SSR_FRE       0x10
/*! \brief SSR: Overrun error flag bit */
#define UART_SSR_ORE       0x08
/*! \brief SSR: Receive data full flag */
#define UART_SSR_RDRF      0x04    //0100
/*! \brief SSR: Transmit data empty flag bi */
#define UART_SSR_TDRE      0x02
/*! \brief SSR: Transmit bus idle flag */
#define UART_SSR_TBI       0x01

/*! \brief ESCR: Flow control enable bit */
#define UART_ESCR_FLWEN    0x80
/*! \brief ESCR: Extension stop bit length select bit */
#define UART_ESCR_ESBL     0x40
/*! \brief ESCR: Inverted serial data format bit */
#define UART_ESCR_INV      0x020
/*! \brief ESCR: Parity enable bit */
#define UART_ESCR_PEN      0x010
/*! \brief ESCR: Parity select bit */
#define UART_ESCR_P        0x08

/*! \brief BGR1: External clock select bit */
#define UART_BGR1_EXT      0x80

/*! \brief FCR1: Re-transmit data lost detect enable bit */
#define UART_FCR1_FLSTE   0x10
/*! \brief FCR1: Receive FIFO idle detection enable bit */
#define UART_FCR1_FRIIE   0x08
/*! \brief FCR1: Transmit FIFO data request bit */
#define UART_FCR1_FDRQ   0x04
/*! \brief FCR1: Transmit FIFO interrupt enable bit */
#define UART_FCR1_FTIE   0x02
/*! \brief FCR1: FIFO select bit */
#define UART_FCR1_FSEL   0x01

/*! \brief FCR0: FIFO re-transmit data lost flag bit */
#define UART_FCR0_FLST     0x40
/*! \brief FCR0: FIFO pointer reload bit */
#define UART_FCR0_FLD      0x20
/*! \brief FCR0: FIFO pointer save bit */
#define UART_FCR0_FSET     0x10
/*! \brief FCR0: FIFO2 reset bit */
#define UART_FCR0_FCL2     0x08
/*! \brief FCR0: FIFO1 reset bit */
#define UART_FCR0_FCL1     0x04
/*! \brief FCR0: FIFO2 operation enable bit */
#define UART_FCR0_FE2      0x02
/*! \brief FCR0: FIFO1 operation enable bit */
#define UART_FCR0_FE1      0x01

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
******************************************************************************
** \brief UART Mode initialization data structure
******************************************************************************
*/
typedef struct
{
    uint32_t Baudrate;  //!< Baudrate
    uint8_t Databits;   //!< Data bit length
    uint8_t Stopbits;   //!< Stop bit length
    uint8_t Parity;     //!< Parity
    uint8_t Bitorder;   //!< Bit order
    uint8_t NRZ;        //!< Signal NRZ
}MFS_UARTModeConfigT;
/*!
******************************************************************************
** \brief UART FIFO mode initialization data structure
******************************************************************************
*/
typedef struct
{
    MFS_FIFOSelectionT FIFOSel;    //!< FIFO Selection
    uint8_t Bytecount1; //!< Byte count of FIFO1
    uint8_t Bytecount2; //!< Byte count of FIFO2
}MFS_UARTFIFOConfigT;
/*!
******************************************************************************
** \brief UART ISR Call back
******************************************************************************
*/
typedef struct
{
    MFS_CallbackT pISRRXCallback;//!< ISR RX call back function
    MFS_CallbackT pISRTXCallback;//!< ISR TX call back function
}MFS_UARTISRCallbackT;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* Config */
void MFS_UARTSetUARTMode(uint8_t Ch);
void MFS_UARTConfigST(uint8_t Ch, MFS_UARTModeConfigT *pModeConfig);
void MFS_UARTConfigLSB(uint8_t Ch);
void MFS_UARTConfigMSB(uint8_t Ch);
void MFS_UARTConfigEvenParity(uint8_t Ch);
void MFS_UARTConfigOddParity(uint8_t Ch);
void MFS_UARTConfigDT(uint8_t Ch, MFS_UARTModeConfigT *pModeConfig);
void MFS_UARTConfigNoInv(uint8_t Ch);
void MFS_UARTConfigInv(uint8_t Ch);
void MFS_UARTConfigBaudrate(uint8_t Ch, uint32_t baudrate);
void MFS_UARTConfigFSEL(uint8_t Ch, MFS_FIFOSelectionT FIFOConfig);
void MFS_UARTConfigFByte1(uint8_t Ch, uint8_t FByte1);
void MFS_UARTConfigFByte2(uint8_t Ch, uint8_t FByte2);
void MFS_UARTEnableSOE(uint8_t Ch);
void MFS_UARTDisableSOE(uint8_t Ch);
void MFS_UARTEnableParity(uint8_t Ch);
void MFS_UARTDisableParity(uint8_t Ch);
void MFS_UARTEnableFIFO2(uint8_t Ch);
void MFS_UARTDisableFIFO2(uint8_t Ch);
void MFS_UARTEnableFIFO1(uint8_t Ch);
void MFS_UARTDisableFIFO1(uint8_t Ch);
void MFS_UARTClearFDRQ(uint8_t Ch);
void MFS_UARTEnableTX(uint8_t Ch);
void MFS_UARTDisableTX(uint8_t Ch);
void MFS_UARTEnableRX(uint8_t Ch);
void MFS_UARTDisableRX(uint8_t Ch);
void MFS_UARTSWRst(uint8_t Ch);
void MFS_UARTResetFIFO2(uint8_t Ch);
void MFS_UARTResetFIFO1(uint8_t Ch);
/* RX/TX */
void MFS_UARTTXOneData(uint8_t Ch, uint16_t Data);
uint16_t MFS_UARTRXOneData(uint8_t Ch);
/* Int */
void MFS_UARTEnableTXBusIdleInt(uint8_t Ch);
void MFS_UARTEnableTXInt(uint8_t Ch, void(*TXFunc)());
void MFS_UARTEnableTXFIFOEmptyInt(uint8_t Ch, void(*TXFunc)());
void MFS_UARTEnableRXInt(uint8_t Ch, void(*RXFunc)());
void MFS_UARTDisableTXBusIdleInt(uint8_t Ch);
void MFS_UARTDisableTXInt(uint8_t Ch);
void MFS_UARTDisableTXFIFOEmptyInt(uint8_t Ch);
void MFS_UARTDisableRXInt(uint8_t Ch);
/* Status */
FlagStatusT MFS_UARTGetOEStatus(uint8_t Ch);
FlagStatusT MFS_UARTGetPEStatus(uint8_t Ch);
FlagStatusT MFS_UARTGetFEStatus(uint8_t Ch);
FlagStatusT MFS_UARTGetRXRegFullStatus(uint8_t Ch);
FlagStatusT MFS_UARTGetTXRegEmptyStatus(uint8_t Ch);
FlagStatusT MFS_UARTGetTXBusIdleStatus(uint8_t Ch);
uint16_t MFS_UARTGetFBYTE1(uint8_t Ch);
uint16_t MFS_UARTGetFBYTE2(uint8_t Ch);
/* Error clear */
void MFS_UARTErrorClr(uint8_t Ch);
#ifdef __cplusplus
}
#endif

#endif

#endif /* _UART_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
