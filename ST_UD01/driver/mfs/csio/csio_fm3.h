/*!
 ******************************************************************************
 **
 ** \file csio_fm3.h
 **
 ** \brief CSIO drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.20
 **
 ** \date 2012-06-07
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
 
#ifndef _CSIO_FM3_H_
#define _CSIO_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "..\mfs_fm3.h"

#if (PERIPHERAL_ENABLE_MFS_CSIO == ON) &&  (PERIPHERAL_AVAILABLE_MFS == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief MFS: CSIO mode */
#define MFS_MODE_CSIO                   0x40
/*! \brief CSIO: SIO mode */
#define MFS_CSIO_SIO                    0
/*! \brief CSIO: SPI mode */
#define MFS_CSIO_SPI                    1

/*! \brief Databit Length: 5 */
#define CSIO_DATABITS_5                 5
/*! \brief Databit Length: 6 */
#define CSIO_DATABITS_6                 6
/*! \brief Databit Length: 7 */
#define CSIO_DATABITS_7                 7
/*! \brief Databit Length: 8 */
#define CSIO_DATABITS_8                 8
/*! \brief Databit Length: 9 */
#define CSIO_DATABITS_9                 9 

/*! \brief BIT Length switch case: 8 bit */
#define CSIO_ESCR_BITLEN_8   0
/*! \brief BIT Length switch case: 5 bit */
#define CSIO_ESCR_BITLEN_5   1
/*! \brief BIT Length switch case: 6 bit */
#define CSIO_ESCR_BITLEN_6   2
/*! \brief BIT Length switch case: 7 bit */
#define CSIO_ESCR_BITLEN_7   3
/*! \brief BIT Length switch case: 9 bit */
#define CSIO_ESCR_BITLEN_9   4

/*! \brief Waitbit Length: 0 */
#define CSIO_WAITBITS_0                 0
/*! \brief Waitbit Length: 1 */
#define CSIO_WAITBITS_1                 1
/*! \brief Waitbit Length: 2 */
#define CSIO_WAITBITS_2                 2
/*! \brief Waitbit Length: 3 */
#define CSIO_WAITBITS_3                 3

/*! \brief Bit order: MSB */
#define CSIO_BITORDER_MSB               1
/*! \brief Bit order: LSB */
#define CSIO_BITORDER_LSB               0

/*! \brief Serial clock invert bit: "HIGH" detection format */
#define CSIO_CLK               0
/*! \brief Serial clock invert bit: "LOW" detection format */
#define CSIO_CLK_INV               1

/*! \brief Master mode */
#define CSIO_MASTER         0
/*! \brief Slave mode */
#define CSIO_SLAVE          1

/*! \brief Transmit FIFO:FIFO1; Receive FIFO:FIFO2 */
#define CSIO_FIFO1_TX  0x00
/*! \brief Transmit FIFO:FIFO2; Receive FIFO:FIFO1 */
#define CSIO_FIFO2_TX  0x01

/*! \brief MFS CSIO Mininum FIFO Channel: 4 */
#define CSIO_Ch_FIFOMIN                0
#define CSIO_Ch_FIFOMAX                MFS_MAX_CH
/*! \brief MFS CSIO Max Channel Number */
#define CSIO_Ch_MAX                MFS_MAX_CH
/*! \brief FIFO max depth*/
#define CSIO_FIFO_MAX_DEPTH      16

/*! \brief SCR: Programmable clear bit */
#define CSIO_SCR_UPCL   0x80
/*! \brief SCR: Master/Slave function select bit */
#define CSIO_SCR_MS   0x40
/*! \brief SCR: SPI corresponding bit */
#define CSIO_SCR_SPI   0x20
/*! \brief SCR: Receive interrupt enable bi  */
#define CSIO_SCR_RIE   0x10
/*! \brief SCR: Transmit interrupt enable bit */
#define CSIO_SCR_TIE   0x08
/*! \brief SCR: Transmit bus idle interrupt enable bit */
#define CSIO_SCR_TBIE   0x04
/*! \brief SCR: Data receive enable bit */
#define CSIO_SCR_RXE   0x02
/*! \brief SCR: Data transmission enable bit */
#define CSIO_SCR_TXE   0x01

/*! \brief SMR: Wake-up control bit */
#define CSIO_SMR_WUCR 0x10
/*! \brief SMR: Serial clock invert bit */
#define CSIO_SMR_SCINV 0x08
/*! \brief SMR: Transfer direction select bit */
#define CSIO_SMR_BDS 0x04
/*! \brief SMR: Master mode serial clock output enable bit */
#define CSIO_SMR_SCKE 0x02
/*! \brief SMR: Serial data output enable bit */
#define CSIO_SMR_SOE 0x01

/*! \brief SSR: Receive error flag clear bit */
#define CSIO_SSR_REC   0x80
/*! \brief SSR: Overrun error flag bit */
#define CSIO_SSR_ORE   0x08
/*! \brief SSR: Receive data full flag bit */
#define CSIO_SSR_RDRF   0x04
/*! \brief SSR: Transmit data empty flag bit */
#define CSIO_SSR_TDRE   0x02
/*! \brief SSR: Transmit bus idle flag bit */
#define CSIO_SSR_TBI   0x01

/*! \brief SSR: Serial output pin set bit */
#define CSIO_ESCR_SOP 0x80
/*! \brief SSR: Data transmit/receive wait select bits */
#define CSIO_ESCR_WT1 0x10
/*! \brief SSR: Data transmit/receive wait select bits */
#define CSIO_ESCR_WT0 0x08
/*! \brief SSR: Data length select bits */
#define CSIO_ESCR_L2 0x04
/*! \brief SSR: Data length select bits */
#define CSIO_ESCR_L1 0x02
/*! \brief SSR: Data length select bits */
#define CSIO_ESCR_L0 0x01

/*! \brief FCR1: Re-transmit data lost detect enable bit */
#define CSIO_FCR1_FLSTE   0x10
/*! \brief FCR1: Receive FIFO idle detection enable bit */
#define CSIO_FCR1_FRIIE   0x08
/*! \brief FCR1: Transmit FIFO data request bit */
#define CSIO_FCR1_FDRQ   0x04
/*! \brief FCR1: Transmit FIFO interrupt enable bit */
#define CSIO_FCR1_FTIE   0x02
/*! \brief FCR1: FIFO select bit */
#define CSIO_FCR1_FSEL   0x01

/*! \brief FCR0: FIFO re-transmit data lost flag bit */
#define CSIO_FCR0_FLST   0x40
/*! \brief FCR0: FIFO pointer reload bit */
#define CSIO_FCR0_FLD   0x20
/*! \brief FCR0: FIFO pointer save bit */
#define CSIO_FCR0_FSET   0x10
/*! \brief FCR0: FIFO2 reset bit */
#define CSIO_FCR0_FCL2   0x08
/*! \brief FCR0: FIFO1 reset bit */
#define CSIO_FCR0_FCL1   0x04
/*! \brief FCR0: FIFO2 operation enable bit */
#define CSIO_FCR0_FE2   0x02
/*! \brief FCR0: FIFO1 operation enable bit */
#define CSIO_FCR0_FE1   0x01

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
******************************************************************************
** \brief CSIO Mode initialization data structure
******************************************************************************
*/
typedef struct
{
    uint8_t Mode;               //!< CSIO Mode
    uint8_t MasterSalve;        //!< Master/Slave
    uint32_t Baudrate;          //!< Baudrate
    uint8_t Databits;           //!< Data bit length
    uint8_t Waitbits;           //!< Wait bit length
    uint8_t Bitorder;           //!< Bit order
    uint8_t Sckinv;             //!< Serial clock Inv
}MFS_CSIOModeConfigT;
/*!
******************************************************************************
** \brief CSIO FIFO mode initialization data structure
******************************************************************************
*/
typedef struct
{
    MFS_FIFOSelectionT FIFOSel;    //!< FIFO Selection
    uint8_t Bytecount1; //!< Byte count of FIFO1
    uint8_t Bytecount2; //!< Byte count of FIFO2
}MFS_CSIOFIFOConfigT;
/*!
******************************************************************************
** \brief CSIO ISR Call back
******************************************************************************
*/
typedef struct
{
    MFS_CallbackT pISRRXCallback;//!< ISR call back function
    MFS_CallbackT pISRTXCallback;//!< ISR call back function
}MFS_CSIOISRCallbackT;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* Config */
void MFS_CSIOSetCSIOMode(uint8_t Ch);
void MFS_CSIOConfigNoInv(uint8_t Ch);
void MFS_CSIOConfigInv(uint8_t Ch);
void MFS_CSIOConfigSIO(uint8_t Ch);
void MFS_CSIOConfigSPI(uint8_t Ch);
void MFS_CSIOConfigMaster(uint8_t Ch);
void MFS_CSIOConfigSlave(uint8_t Ch);
void MFS_CSIOConfigLSB(uint8_t Ch);
void MFS_CSIOConfigMSB(uint8_t Ch);
void MFS_CSIOConfigBaudrate(uint8_t Ch, MFS_CSIOModeConfigT *pModeConfig);
void MFS_CSIOConfigWT(uint8_t Ch, MFS_CSIOModeConfigT *pModeConfig);
void MFS_CSIOConfigDT(uint8_t Ch, MFS_CSIOModeConfigT *pModeConfig);
void MFS_CSIOConfigFSEL(uint8_t Ch, MFS_FIFOSelectionT FIFOSel);
void MFS_CSIOConfigFByte1(uint8_t Ch, uint8_t FByte1);
void MFS_CSIOConfigFByte2(uint8_t Ch, uint8_t FByte2);
void MFS_CSIOClearFDRQ(uint8_t Ch);
void MFS_CSIOEnableSOE(uint8_t Ch);
void MFS_CSIODisableSOE(uint8_t Ch);
void MFS_CSIOEnableFLSTE(uint8_t Ch);
void MFS_CSIODisableFLSTE(uint8_t Ch);
void MFS_CSIOEnableFRIIE(uint8_t Ch);
void MFS_CSIODisableFRIIE(uint8_t Ch);
void MFS_CSIOEnableFIFO2(uint8_t Ch);
void MFS_CSIOEnableFIFO1(uint8_t Ch);
void MFS_CSIODisableFIFO2(uint8_t Ch);
void MFS_CSIODisableFIFO1(uint8_t Ch);
void MFS_CSIOEnableClock(uint8_t Ch);
void MFS_CSIODisableClock(uint8_t Ch);
void MFS_CSIOEnableTX(uint8_t Ch);
void MFS_CSIOEnableRX(uint8_t Ch);
void MFS_CSIODisableTX(uint8_t Ch);
void MFS_CSIODisableRX(uint8_t Ch);
void MFS_CSIOResetFIFO2(uint8_t Ch);
void MFS_CSIOResetFIFO1(uint8_t Ch);
/* RX/TX */
void MFS_CSIOTXOneData(uint8_t Ch, uint16_t Data);
uint16_t MFS_CSIORXOneData(uint8_t Ch);
uint16_t MFS_CSIOTransferOneData(uint8_t Ch, uint16_t Data);
/* Int */
void MFS_CSIOEnableTXBusIdleInt(uint8_t Ch);
void MFS_CSIOEnableTXInt(uint8_t Ch, void(*TXFunc)());
void MFS_CSIOEnableTXFIFOEmptyInt(uint8_t Ch, void(*TXFunc)());
void MFS_CSIODisableTXBusIdleInt(uint8_t Ch);
void MFS_CSIODisableTXInt(uint8_t Ch);
void MFS_CSIODisableTXFIFOEmptyInt(uint8_t Ch);
void MFS_CSIOEnableRXInt(uint8_t Ch, void(*RXFunc)());
void MFS_CSIODisableRXInt(uint8_t Ch);
/* Status */
FlagStatusT MFS_CSIOGetOEStatus(uint8_t Ch);
FlagStatusT MFS_CSIOGetRXRegFullStatus(uint8_t Ch);
FlagStatusT MFS_CSIOGetTXRegEmptyStatus(uint8_t Ch);
FlagStatusT MFS_CSIOGetTXBusIdleStatus(uint8_t Ch);
uint8_t MFS_CSIOFBYTE1Status(uint8_t Ch);
uint8_t MFS_CSIOFBYTE2Status(uint8_t Ch);
FlagStatusT MFS_CSIOGetFLSTStatus(uint8_t Ch);
/* Programable/Error clear */
void MFS_CSIOProgramClr(uint8_t Ch);
void MFS_CSIOErrorClr(uint8_t Ch);
#ifdef __cplusplus
}
#endif

#endif

#endif /* _CSIO_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
