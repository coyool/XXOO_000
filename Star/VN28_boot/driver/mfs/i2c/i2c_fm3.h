/*!
 ******************************************************************************
 **
 ** \file I2C_fm3.h
 **
 ** \brief I2C drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2012-02-09
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

#ifndef _I2C_FM3_H_
#define _I2C_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "..\mfs_fm3.h"

#if (PERIPHERAL_AVAILABLE_MFS == OFF)
    #if (PERIPHERAL_ENABLE_MFS_I2C == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFS_I2C == ON) && (PERIPHERAL_AVAILABLE_MFS == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief MFS I2C Mininum FIFO Channel: MFS_MAX_CH */
#define I2C_Ch_FIFOMIN          0
#define I2C_Ch_FIFOMAX          MFS_MAX_CH
/*! \brief MFS I2C Max Channel Number */
#define I2C_Ch_MAX              MFS_MAX_CH
/*! \brief FIFO max depth*/
#define I2C_FIFO_MAX_DEPTH      16

/*! \brief MFS: I2C mode */
#define MFS_MODE_I2C                   0x80

/*! \brief Enable I2C Bus */
#define I2C_BUS_EN         1
/*! \brief Disable I2C Bus */
#define I2C_BUS_DIS        0

/*! \brief Master mode */
#define I2C_MASTER         1
/*! \brief Slave mode */
#define I2C_SLAVE          0

#define I2cDev_IRQSTATUS_SLAVE_TX          1
#define I2cDev_IRQSTATUS_SLAVE_RX          0

/*! \ brief DMA mode */
#define I2C_DMA_EN    1
#define I2C_DMA_DIS    0

/*! \brief Master or Slave mode bit*/
#define I2C_IBCR_MSS  0x80
/*! \brief Generate iteration start condition*/
#define I2C_IBCR_SCC  0x40
/*! \brief active operation mode*/
#define I2C_IBCR_ACT  0x40
/*! \brief ACK enable bit*/
#define I2C_IBCR_ACKE 0x20
/*! \brief condition detect interrupt enable*/
#define I2C_IBCR_CNDE 0x08
/*! \brief wait bit*/
#define I2C_IBCR_WSEL 0x10
/*! \brief interrupt enable*/
#define I2C_IBCR_INTE 0x04
/*! \brief bus error flag*/
#define I2C_IBCR_BER  0x02
/*! \brief interrupt flag*/
#define I2C_IBCR_INT  0x01

/*! \brief Receive interrupt enable bit*/
#define I2C_SMR_RIE          0x08
/*! \brief Transmit interrupt enable bit*/
#define I2C_SMR_TIE          0x04

/*! \brief first byte bit*/
#define I2C_IBSR_FBT  0x80
/*! \brief acknowledge flag bit*/
#define I2C_IBSR_RACK  0x40
/*! \brief data direction flag bit*/
#define I2C_IBSR_TRX  0x10
/*! \brief arbitration lost flag bit*/
#define I2C_IBSR_AL  0x08
/*! \brief Iteration start condition check bit*/
#define I2C_IBSR_RSC  0x04
/*! \brief Stop condition check bit*/
#define I2C_IBSR_SPC  0x02
/*! \brief bus state bit*/
#define I2C_IBSR_BB  0x01

/*! \brief DMA enable bit*/
#define I2C_SSR_DMA   0x20
/*! \brief over run error flag bit*/
#define I2C_SSR_ORE   0x08
/*! \brief Received data full flag bit*/
#define I2C_SSR_RDRF   0x04
/*! \brief Transmit data empty flag bit*/
#define I2C_SSR_TDRE  0x02
/*! \brief Transmit bus idle flag bit*/
#define I2C_SSR_TBI  0x01

/*! \brief I2C interface operation enable bit*/
#define I2C_ISMK_EN  0x80

/*! \brief slave address enable bit*/
#define I2C_ISBA_SAEN  0x80

/*! \brief I2C enable bit*/
#define I2C_BUS_EN_BIT    0x80

/*! \brief FCR0: FIFO re-transmit data lost flag bit */
#define I2C_FCR0_FLST   0x40
/*! \brief FCR0: FIFO pointer reload bit */
#define I2C_FCR0_FLD   0x20
/*! \brief FCR0: FIFO pointer save bit */
#define I2C_FCR0_FSET   0x10
/*! \brief FCR0: FIFO2 reset bit */
#define I2C_FCR0_FCL2   0x08
/*! \brief FCR0: FIFO1 reset bit */
#define I2C_FCR0_FCL1   0x04
/*! \brief FCR0: FIFO2 operation enable bit */
#define I2C_FCR0_FE2   0x02
/*! \brief FCR0: FIFO1 operation enable bit */
#define I2C_FCR0_FE1   0x01

/*! \brief FCR1: Re-transmit data lost detect enable bit */
#define I2C_FCR1_FLSTE   0x10
/*! \brief FCR1: Receive FIFO idle detection enable bit */
#define I2C_FCR1_FRIIE   0x08
/*! \brief FCR1: Transmit FIFO data request bit */
#define I2C_FCR1_FDRQ   0x04
/*! \brief FCR1: Transmit FIFO interrupt enable bit */
#define I2C_FCR1_FTIE   0x02
/*! \brief FCR1: FIFO select bit */
#define I2C_FCR1_FSEL   0x01

/*! \brief ACK level*/
#define ACK_HIGH_LEVEL  1
#define ACK_LOW_LEVEL   0

/*!\brief direction of I2C BUS data*/
#define TRX_RECEIVE     0
#define TRX_SEND        1

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
******************************************************************************
** \brief I2C Mode initialization data structure
******************************************************************************
*/
typedef struct
{
    uint8_t MasterSalve;        //!< Master/Slave
    uint8_t DMAMode;            //!< enable/disable DMA mode
    uint8_t SlaveAddr;          //!< Slave Address  A6~A0 + R/W
    uint8_t Bytecount1;         //!< Byte count of FIFO1
    uint8_t Bytecount2;         //!< Byte count of FIFO2
    uint32_t Baudrate;          //!< Baudrate
}MFS_I2CModeConfigT;
/*!
******************************************************************************
** \brief I2C FIFO mode initialization data structure
******************************************************************************
*/
typedef enum
{
    I2C_FIFO1_TX_FIFO2_RX = 0,
    I2C_FIFO1_RX_FIFO2_TX,
    
}MFS_I2CFIFOSelT;
/*!
******************************************************************************
** \brief I2C ISR Call back
******************************************************************************
*/
typedef struct
{
    MFS_CallbackT pISRRXCallback;//!< ISR RX call back function
    MFS_CallbackT pISRTXCallback;//!< ISR TX call back function
    MFS_CallbackT pISRStatusCallback;//!< ISR TX call back function
}MFS_I2CISRCallbackT;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/* Config */
void MFS_I2CConfigBaudrate(uint8_t Ch, uint32_t baudrate);
void MFS_I2CConfigI2CMode(uint8_t Ch);
void MFS_I2CConfigMasterMode(uint8_t Ch);
void MFS_I2CConfigSlaveMode(uint8_t Ch);
void MFS_I2CClrSCC(uint8_t Ch);
void MFS_I2CSetSCC(uint8_t Ch);
void MFS_I2CClrMSS(uint8_t Ch);
void MFS_I2CEnableI2C(uint8_t Ch);
void MFS_I2CDisableI2C(uint8_t Ch);
void MFS_I2CClrSPC(uint8_t Ch);
void MFS_I2CClrRSC(uint8_t Ch);
void MFS_I2CWSEL8(uint8_t Ch);
void MFS_I2CWSEL9(uint8_t Ch);
void MFS_I2CEnableACKE(uint8_t Ch);
void MFS_I2CDisableACKE(uint8_t Ch);
void MFS_I2CClrRcvErrFlag(uint8_t Ch);
void MFS_I2CEnableSlaveAddr(uint8_t Ch);
void MFS_I2CDisableSlaveAddr(uint8_t Ch);
void MFS_I2CConfigSlaveAddr(uint8_t Ch, uint8_t SlaveAddr);
void MFS_I2CEnableFIFO1(uint8_t Ch);
void MFS_I2CEnableFIFO2(uint8_t Ch);
void MFS_I2CEnableRcvFIFOIdleDetect(uint8_t Ch);
void MFS_I2CDisableFIFO1(uint8_t Ch);
void MFS_I2CDisableFIFO2(uint8_t Ch);
void MFS_I2CDisableRcvFIFOIdleDetect(uint8_t Ch);
void MFS_I2CResetFIFO1(uint8_t Ch);
void MFS_I2CResetFIFO2(uint8_t Ch);
void MFS_I2CSetFIFO1Length(uint8_t Ch, uint32_t Length);
void MFS_I2CSetFIFO2Length(uint8_t Ch, uint32_t Length);
void MFS_I2CClrFIFOSendIntReq(uint8_t Ch);
void MFS_I2CClrFIFOSendReq(uint8_t Ch);
void MFS_I2CSetFIFOSendReq(uint8_t Ch);
void MFS_I2CSelFIFO(uint8_t Ch, MFS_I2CFIFOSelT FIFOSel);
void MFS_I2CSetMaskSlaveAddrBits(uint8_t Ch, uint8_t MaskBits);

/*Interrupt*/
void MFS_I2CClrINT(uint8_t Ch);
void MFS_I2CEnableCND(uint8_t Ch);
void MFS_I2CDisableCND(uint8_t Ch);
void MFS_I2CEnableINT(uint8_t Ch);
void MFS_I2CDisableINT(uint8_t Ch);
void MFS_I2CEnableRIE(uint8_t Ch);
void MFS_I2CDisableRIE(uint8_t Ch);
void MFS_I2CEnableTIE(uint8_t Ch);
void MFS_I2CDisableTIE(uint8_t Ch);
void MFS_I2CEnableFIFOTrsINT(uint8_t Ch);
void MFS_I2CDisableFIFOTrsINT(uint8_t Ch);
void MFS_I2CConfigIntCallback(uint8_t Ch, MFS_I2CISRCallbackT* pISRCallback);

/*State*/
FlagStatusT MFS_I2CGetINTStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetTDREStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetRDRFStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetSPCStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetMSSStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetACTStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetBusErrStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetFstByteStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetAckStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetDataDirStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetArbitLostStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetIterStartStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetBusStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetOverRunErrStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetTBIStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetI2CENStatus(uint8_t Ch);
FlagStatusT MFS_I2CGetFDRQStatus(uint8_t Ch);
/*Rx/Tx*/
uint8_t MFS_I2CReadAData(uint8_t Ch);
void MFS_I2CWriteAData(uint8_t Ch, uint16_t Data);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _I2C_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
