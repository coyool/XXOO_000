/*!
 ******************************************************************************
 **
 ** \file lin_fm3.h
 **
 ** \brief LIN drivers head file
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
 
#ifndef _LIN_FM3_H_
#define _LIN_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "..\mfs_fm3.h"

#if (PERIPHERAL_AVAILABLE_MFS == OFF)
    #if (PERIPHERAL_ENABLE_MFS_LIN == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFS_LIN == ON) && (PERIPHERAL_AVAILABLE_MFS == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief MFS: LIN mode */
#define MFS_MODE_LIN                      0x60

/*! \brief BreakFieldbit Length: 13 */
#define LIN_BREAKFIELDBITS_13                 13
/*! \brief BreakFieldbit Length: 14 */
#define LIN_BREAKFIELDBITS_14                 14
/*! \brief BreakFieldbit Length: 15 */
#define LIN_BREAKFIELDBITS_15                 15
/*! \brief BreakFieldbit Length: 16 */
#define LIN_BREAKFIELDBITS_16                 16

/*! \brief BreakField BIT Length switch case: 13 bit */
#define LIN_ESCR_BREAKFIELDBITLEN_13   0x00
/*! \brief BreakField BIT Length switch case: 14 bit */
#define LIN_ESCR_BREAKFIELDBITLEN_14   0x04
/*! \brief BreakField BIT Length switch case: 15 bit */
#define LIN_ESCR_BREAKFIELDBITLEN_15   0x08
/*! \brief BreakField BIT Length switch case: 16 bit */
#define LIN_ESCR_BREAKFIELDBITLEN_16   0x0C


/*! \brief BreakDelimiter Length: 1 */
#define LIN_BREAKDELIMITER_1                 1
/*! \brief BreakDelimiter Length: 2 */
#define LIN_BREAKDELIMITER_2                 2
/*! \brief BreakDelimiter Length: 3 */
#define LIN_BREAKDELIMITER_3                 3
/*! \brief BreakDelimiter Length: 4 */
#define LIN_BREAKDELIMITER_4                 4

/*! \brief Break Delimiter Length switch case: 1 bit */
#define LIN_ESCR_BREAKDELIMITERLENGTH_1   0x00
/*! \brief Break Delimiter Length switch case: 2 bit */
#define LIN_ESCR_BREAKDELIMITERLENGTH_2   0x01
/*! \brief Break Delimiter Length switch case: 3 bit */
#define LIN_ESCR_BREAKDELIMITERLENGTH_3   0x02
/*! \brief Break Delimiter Length switch case: 4 bit */
#define LIN_ESCR_BREAKDELIMITERLENGTH_4   0x03



/*! \brief Stopbit Length: 1 */
#define LIN_STOPBITS_1                 1
/*! \brief Stopbit Length: 2 */
#define LIN_STOPBITS_2                 2
/*! \brief Stopbit Length: 3 */
#define LIN_STOPBITS_3                 3
/*! \brief Stopbit Length: 4 */
#define LIN_STOPBITS_4                 4

/*! \brief Transmit FIFO:FIFO1; Receive FIFO:FIFO2 */
#define LIN_FIFO1_TX  0x00
/*! \brief Transmit FIFO:FIFO2; Receive FIFO:FIFO1 */
#define LIN_FIFO2_TX  0x01

/*! \brief MFS LIN Mininum FIFO Channel */
#define LIN_Ch_FIFOMIN            0
/*! \brief MFS LIN Max Channel Number */
#define LIN_Ch_MAX                MFS_MAX_CH
/*! \brief MFS LIN Master mode */
#define LIN_MASTER    0
/*! \brief MFS LIN Slave mode */
#define LIN_SLAVE     1

/*! \brief SCR: Programmable Clear bit */
#define LIN_SCR_UPCL   0x80
/*! \brief SCR: Master/Slave function select bit */
#define LIN_SCR_MS    0x40
/*! \brief SCR: LIN Break field setting bit */
#define LIN_SCR_LBR    0x20
/*! \brief SCR: Receive interrupt enable bit */
#define LIN_SCR_RIE   0x10
/*! \brief SCR: Transmit interrupt enable bit */
#define LIN_SCR_TIE   0x08
/*! \brief SCR: Transmit bus idle interrupt enable bit */
#define LIN_SCR_TBIE   0x04
/*! \brief SCR: Receive operation enable bit */
#define LIN_SCR_RXE   0x02
/*! \brief SCR: Transmit operation enable bit */
#define LIN_SCR_TXE   0x01

/*! \brief SMR: Wake-up control bit */
#define LIN_SMR_WUCR 0x10
/*! \brief SMR: Stop bit length select bit */
#define LIN_SMR_SBL 0x08
/*! \brief SMR: Serial data output enable bit */
#define LIN_SMR_SOE 0x01

/*! \brief SSR: Receive error flag clear bit */
#define LIN_SSR_REC   0x80
/*! \brief SSR: LIN Break field detection flag bit */
#define LIN_SSR_LBD   0x20
/*! \brief SSR: Framing error flag bit */
#define LIN_SSR_FRE   0x10
/*! \brief SSR: Overrun error flag bit */
#define LIN_SSR_ORE   0x08
/*! \brief SSR: Receive data full flag */
#define LIN_SSR_RDRF   0x04
/*! \brief SSR: Transmit data empty flag bi */
#define LIN_SSR_TDRE   0x02
/*! \brief SSR: Transmit bus idle flag */
#define LIN_SSR_TBI   0x01

/*! \brief ESCR: Extension stop bit length select bit */
#define LIN_ESCR_ESBL 0x40
/*! \brief ESCR: LIN Break field detect interrupt enable bit */
#define LIN_ESCR_LBIE 0x10


/*! \brief BGR1: External clock select bit */
#define LIN_BGR1_EXT 0x80

/*! \brief FCR1: Re-transmit data lost detect enable bit */
#define LIN_FCR1_FLSTE   0x10
/*! \brief FCR1: Receive FIFO idle detection enable bit */
#define LIN_FCR1_FRIIE   0x08
/*! \brief FCR1: Transmit FIFO data request bit */
#define LIN_FCR1_FDRQ   0x04
/*! \brief FCR1: Transmit FIFO interrupt enable bit */
#define LIN_FCR1_FTIE   0x02
/*! \brief FCR1: FIFO select bit */
#define LIN_FCR1_FSEL   0x01

/*! \brief FCR0: FIFO re-transmit data lost flag bit */
#define LIN_FCR0_FLST   0x40
/*! \brief FCR0: FIFO pointer reload bit */
#define LIN_FCR0_FLD   0x20
/*! \brief FCR0: FIFO pointer save bit */
#define LIN_FCR0_FSET   0x10
/*! \brief FCR0: FIFO2 reset bit */
#define LIN_FCR0_FCL2   0x08
/*! \brief FCR0: FIFO1 reset bit */
#define LIN_FCR0_FCL1   0x04
/*! \brief FCR0: FIFO2 operation enable bit */
#define LIN_FCR0_FE2   0x02
/*! \brief FCR0: FIFO1 operation enable bit */
#define LIN_FCR0_FE1   0x01

/*! \brief LIN_DATALENGTH_MAX: 8 bytes data field  */
#define LIN_DATALENGTH_MAX   0x08


/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
******************************************************************************
** \brief LIN Mode initialization data structure
******************************************************************************
*/
typedef struct
{
    uint32_t Baudrate;  //!< Baudrate
    uint8_t BreakFieldbits;   //!< Break field bit length
    uint8_t BreakDelimiter;    //!< LIN Break delimiter length
    uint8_t Stopbits;   //!< Stop bit length
}MFS_LINModeConfigT;
/*!
******************************************************************************
** \brief LIN FIFO mode initialization data structure
******************************************************************************
*/
typedef struct
{
    MFS_FIFOSelectionT FIFOSel;    //!< FIFO Selection
    uint8_t Bytecount1; //!< Byte count of FIFO1
    uint8_t Bytecount2; //!< Byte count of FIFO2
}MFS_LINFIFOConfigT;
/*!
******************************************************************************
** \brief LIN ISR Call back
******************************************************************************
*/
typedef struct
{
    MFS_CallbackT pISRRXCallback;//!< ISR RX call back function
    MFS_CallbackT pISRTXCallback;//!< ISR TX call back function
    MFS_CallbackT pISRStatusCallback;
}MFS_LINISRCallbackT;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* Config */
void MFS_LINSetLINMode(uint8_t Ch);
void MFS_LINConfigST(uint8_t Ch, MFS_LINModeConfigT *pModeConfig);
void MFS_LINConfigBFL(uint8_t Ch, MFS_LINModeConfigT *pModeConfig);
void MFS_LINConfigBDL(uint8_t Ch, MFS_LINModeConfigT *pModeConfig);
uint16_t MFS_LINConfigBaudrate(uint8_t Ch, uint32_t baudrate);
void MFS_LINSetMode(uint8_t Ch, uint8_t MS_Mode);
void MFS_LINConfigISRCallback(uint8_t Ch, MFS_LINISRCallbackT *pCallback);
void MFS_LINConfigFSEL(uint8_t Ch, MFS_FIFOSelectionT FIFOSel);
void MFS_LINConfigFByte1(uint8_t Ch, uint8_t FByte1);
void MFS_LINConfigFByte2(uint8_t Ch, uint8_t FByte2);
void MFS_LINResetFIFO2(uint8_t Ch);
void MFS_LINResetFIFO1(uint8_t Ch);
void MFS_LINEnableFIFO2(uint8_t Ch);
void MFS_LINDisableFIFO2(uint8_t Ch);
void MFS_LINDisableFIFO1(uint8_t Ch);
void MFS_LINEnableFIFO1(uint8_t Ch);
void MFS_LINResetFDRQ(uint8_t Ch);
void MFS_LINEEnableSO(uint8_t Ch);
void MFS_LINDisableSOE(uint8_t Ch);
void MFS_LINSetBreakField(uint8_t Ch);
void MFS_LINEnableTX(uint8_t Ch);
void MFS_LINEnableRX(uint8_t Ch);
void MFS_LINFIFOEnable(uint8_t Ch, uint8_t nfifo);
void MFS_LINDisableTX(uint8_t Ch);
void MFS_LINDisableRX(uint8_t Ch);
void MFS_LINFIFODisable(uint8_t Ch, uint8_t nfifo);
void MFS_LINEnableSOE(uint8_t Ch);
void MFS_LINDisableSOE(uint8_t Ch);
/* RX/TX */
void MFS_LINTXOneData(uint8_t Ch, uint8_t Data);
uint8_t MFS_LINRXOneData(uint8_t Ch);
/* Int */
void MFS_LINEnableTXBusIdleInt(uint8_t Ch);
void MFS_LINDisableTXBusIdleInt(uint8_t Ch);
void MFS_LINEnableTXInt(uint8_t Ch);
void MFS_LINDisableTXInt(uint8_t Ch);
void MFS_LINEnableTXFIFOEmptyInt(uint8_t Ch);
void MFS_LINDisableTXFIFOEmptyInt(uint8_t Ch);
void MFS_LINEnableRXInt(uint8_t Ch);
void MFS_LINDisableRXInt(uint8_t Ch);
void MFS_LINEnableWakeUp(uint8_t Ch);
void MFS_LINDisableWakeUp(uint8_t Ch);
void MFS_LINEnableLBIEInt(uint8_t Ch);
void MFS_LINDisableLBIEInt(uint8_t Ch);
/* Status */
FlagStatusT MFS_LINGetOEStatus(uint8_t Ch);
FlagStatusT MFS_LINGetLBDStatus(uint8_t Ch);
FlagStatusT MFS_LINGetFEStatus(uint8_t Ch);
FlagStatusT MFS_LINGetRXRegFullStatus(uint8_t Ch);
FlagStatusT MFS_LINGetTXRegEmptyStatus(uint8_t Ch);
FlagStatusT MFS_LINGetTXBusIdleStatus(uint8_t Ch);
FlagStatusT MFS_LINGetFDRQStatus(uint8_t Ch);
uint8_t MFS_LINGetFBYTE1Status(uint8_t Ch);
uint8_t MFS_LINGetFBYTE2Status(uint8_t Ch);
/* Error/Programable clear */
void MFS_LINClrError(uint8_t Ch);
void MFS_LINClrProgram(uint8_t Ch);
void MFS_LINClrLBD(uint8_t Ch);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _LIN_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
