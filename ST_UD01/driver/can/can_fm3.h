/*!
 ******************************************************************************
 **
 ** \file can_fm3.h
 **
 ** \brief can drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2012-11-09
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

#ifndef _can_FM3_H_
#define _can_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_CAN == OFF)
    #if (PERIPHERAL_ENABLE_CAN == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_AVAILABLE_CAN == ON) && (PERIPHERAL_ENABLE_CAN == ON)
/*Can max channel*/
#define CAN_CH_MAX  2
#define CAN_CH0     0x00
#define CAN_CH1     0x01

/*transfer state*/
#define CAN_TRSF_BUSY               0
#define CAN_TRSF_FINISH             1

/*receiving state*/
#define CAN_RCV_BUSY                0
#define CAN_RCV_FINISH              1
/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*prescaler value enum*/
typedef enum
{
    CAN_CLK_1 = 0,    
    CAN_CLK_1Div2 = 0x01,  
    CAN_CLK_1Div4 = 0x02,    
    CAN_CLK_1Div8 = 0x04,  
    CAN_CLK_2Div3 = 0x08,    
    CAN_CLK_1Div3 = 0x09,      
    CAN_CLK_1Div6 = 0x0A,    
    CAN_CLK_1Div12 = 0x0B,  
    CAN_CLK_1Div5 = 0x0C,  
    CAN_CLK_1Div10 = 0x0E,  

}CAN_ClkDivT;

/*!
 ******************************************************************************
 ** \brief CAN operation state definition
 ******************************************************************************
 */
typedef enum
{
    CAN_OPERATION = 0,    //!< Set CAN at the initialization state
    CAN_INIT,   //!< Set CAN at the operation state
  
}CAN_OptModeT;

/*!
 ******************************************************************************
 ** \brief CAN status definition
 ******************************************************************************
 */
typedef enum
{
    CAN_BUS_OFF = 0,     //!< CAN Bus off
    CAN_WARNING,         //!< Send or receive error counter has reached or exceeded 96.
    CAN_ERROR_PASSIVE,   //!< Error passive
    CAN_RX_OK,           //!< Message received successfully
    CAN_TX_OK,           //!< Message sent successfully
  
}CAN_StatusT;

/*!
 ******************************************************************************
 ** \brief CAN Last error code definition
 ******************************************************************************
 */
typedef enum
{
    LEC_NORMAL = 0,     //!< Normal
    LEC_STUFF_ERR,      //!< Stuff error
    LEC_FORM_ERR,       //!< Form error
    LEC_ACK_ERR,        //!< ACK error
    LEC_BIT1_ERR,       //!< Bit 1 error
    LEC_BIT0_ERR,       //!< Bit 0 error
    LEC_CRC_ERR,        //!< CRC 0 error
    LEC_UNDETECT,       //!< Undetect error
  
}CAN_LastErrCodeT;

/*!
 ******************************************************************************
 ** \brief CAN Bus Off  state
 ******************************************************************************
 */
typedef enum
{
	CAN_BUSOFF_ERR = 0,
	CAN_BUSOFF_EWARNING,
	
}CAN_ErrNotifyT;

/*!
 ******************************************************************************
 ** \brief CAN Bit timing information definition
 ******************************************************************************
 */
typedef struct
{
    uint8_t TSeg2;      //!< Tseg2 area
    uint8_t TSeg1;      //!< Tseg1 area
    uint8_t SWJ;        //!< SWJ area
    uint16_t BRP;       //!< BRP area
   
}CAN_BitTimingInfoT;


/*!
 ******************************************************************************
 ** \brief CAN TX pin state definition
 ******************************************************************************
 */
typedef enum
{
    PIN_NORMAL = 0,             //!< TX normal
    PIN_OUTPUT_SAMPLE_POINT,    //!< TX output a sample point
    PIN_OUTPUT_DOMINANT,        //!< TX output dominant
    PIN_OUTPUT_RECESSIVE,       //!< TX output recessive
  
}CAN_TXPinStateT;

/*!
 ******************************************************************************
 ** \brief CAN test mode definition
 ******************************************************************************
 */
typedef enum
{
    SilentMode,                     //!< Silent mode
    LoopBackMode,                   //!< Loopback mode
    SeftTestMode,                   //!< Silent+Loopback mode
    SilentMode_With_BasicMode,      //!< Silent mode with basic mode
    LoopBackMode_With_BasicMode,    //!< Loopback mode with basic mode
    SeftTestMode_With_BasicMode,   //!< Silent+Loopback mode with basic mode
  
}CAN_TestModeT;


/*!
 ******************************************************************************
 ** \brief CAN Message Mask register1 definition
 ******************************************************************************
 */
typedef struct
{
    uint8_t Rsv : 1;
    uint8_t Mask : 1;
    uint8_t Arb : 1;
    uint8_t Control : 1;
    uint8_t CIP : 1;
    uint8_t TxRqst : 1;
    uint8_t DataA : 1;
    uint8_t DataB : 1;
}CAN_WriteMsgMaskT;

/*!
 ******************************************************************************
 ** \brief CAN Message Mask register2 definition
 ******************************************************************************
 */
typedef struct
{
    uint8_t Rsv : 1;
    uint8_t Mask : 1;
    uint8_t Arb : 1;
    uint8_t Control : 1;
    uint8_t CIP : 1;
    uint8_t NewDat : 1;
    uint8_t DataA : 1;
    uint8_t DataB : 1;
}CAN_ReadMsgMaskT;

/*!
 ******************************************************************************
 ** \brief CAN Message Mask register definition
 ******************************************************************************
 */
typedef struct
{
    uint32_t MskID:29;
    uint32_t Rsv:1;
    uint32_t MDIR:1;
    uint32_t MXtd:1;
    
}CAN_MaskInfoT;

/*!
 ******************************************************************************
 ** \brief CAN Message Abr register definition
 ******************************************************************************
 */
typedef struct
{
    uint32_t ID:29;
    uint32_t DIR:1;
    uint32_t Xtd:1;
    uint32_t MsgEn:1;
    
}CAN_AbrInfoT;

/*!
 ******************************************************************************
 ** \brief CAN Message Control register definition
 ******************************************************************************
 */
typedef struct
{
    uint16_t DLC:4;
    uint16_t Rsv:3;
    uint16_t EoB:1;
    uint16_t TxRqst:1;
    uint16_t RmtEn:1;
    uint16_t RxIE:1;
    uint16_t TxIE:1;
    uint16_t UMask:1;
    uint16_t IntPnd:1;
    uint16_t MsgLst:1;
    uint16_t NewDat:1;
    
}CAN_CtrlInfoT;

/*!
 ******************************************************************************
 ** \brief CAN Interrupt callback function definition
 ******************************************************************************
 */
typedef void CAN_TxIntHandlerCallbackT(int32_t Status, uint32_t ObjId);
typedef void CAN_RxIntHandlerCallbackT(int32_t Status, uint32_t ObjId);
typedef void CAN_ErrIntHandlerCallbackT(CAN_ErrNotifyT Status);
typedef void CAN_StatusIntHandlerCallbackT(CAN_LastErrCodeT Status);

/*!
 ******************************************************************************
 ** \brief structure of CAN Interrupt callback function definition
 ******************************************************************************
 */
typedef struct
{
    CAN_TxIntHandlerCallbackT *TxIntHandlerCallback;
    CAN_RxIntHandlerCallbackT *RxIntHandlerCallback;
    CAN_ErrIntHandlerCallbackT *ErrIntHandlerCallback;
    CAN_StatusIntHandlerCallbackT *StatusIntHandlerCallback;
}CAN_IntHandlerArray;

extern CAN_IntHandlerArray CANIntArray[CAN_CH_MAX];

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
  
/* 0. Can Prescaler */
void CAN_SetClkDiv(CAN_ClkDivT Div);

/* 1. Global control */
/* 1.1 Control register */
void CAN_EnableTestMode(uint8_t Ch);
void CAN_DisableTestMode(uint8_t Ch);
void CAN_EnableBitTimingSetting(uint8_t Ch);
void CAN_DisableBitTimingSetting(uint8_t Ch);
void CAN_EnableAutoTrsf(uint8_t Ch);
void CAN_DisableAutoTrsf(uint8_t Ch);
void CAN_EnableErrIntCode(uint8_t Ch);
void CAN_DisableErrIntCode(uint8_t Ch);
void CAN_EnableStatIntCode(uint8_t Ch);
void CAN_DisableStatIntCode(uint8_t Ch);
void CAN_EnableInt(uint8_t Ch);
void CAN_DisableInt(uint8_t Ch);
void CAN_SetOptMode(uint8_t Ch, CAN_OptModeT Mode);

/* 1.2 Status register */
FlagStatusT CAN_GetStatus(uint8_t Ch, CAN_StatusT Type);
CAN_LastErrCodeT CAN_GetLastErrCode(uint8_t Ch);
void CAN_ClrRxOKFlag(uint8_t Ch);
void CAN_ClrTxOKFlag(uint8_t Ch);
void CAN_ClrLastErrCodeFlag(uint8_t Ch);

/* 1.3 Error register */
FlagStatusT CAN_GetRcvPasFlag(uint8_t Ch);
uint8_t CAN_GetRcvErrCnt(uint8_t Ch);
uint8_t CAN_GetTrsfErrCnt(uint8_t Ch);

/* 1.4 Bit timing register */
void CAN_ConfigBitTiming(uint8_t Ch, CAN_BitTimingInfoT *pInfo);

/* 1.5 Interrupt code register */
uint16_t CAN_GetIntCode(uint8_t Ch);

/* 1.7 Test mode register */
uint8_t CAN_GetBusState(uint8_t Ch);
void CAN_SetTXPinState(uint8_t Ch, CAN_TXPinStateT State);
void CAN_ConfigTestMode(uint8_t Ch, CAN_TestModeT Mode);

/* 2. Message interface */

/* 2.1 IFx Command Request */
/* 2.1.1 IF1 Command Request (Send) */
FlagStatusT CAN_GetMsgTrsfBusyFlag(uint8_t Ch);
void CAN_EnableTestModeTrsf(uint8_t Ch);
void CAN_WriteMsgRAM(uint8_t Ch, uint8_t MsgBufNum);

/* 2.1.2 IF2 Command Request (Receive) */
FlagStatusT CAN_GetMsgRcvBusyFlag(uint8_t Ch);
void CAN_EnableTestModeRcv(uint8_t Ch);
void CAN_ReadMsgRAM(uint8_t Ch, uint8_t MsgBufNum);

/* 2.2 IFx Command Mask */
/* 2.1.1 IF1 Command Mask (Send) */
void CAN_ConfigWriteMsgMask(uint8_t Ch,  CAN_WriteMsgMaskT *pMask);
/* 2.1.2 IF2 Command Mask (Receive) */
void CAN_ConfigReadMsgMask(uint8_t Ch,  CAN_ReadMsgMaskT *pMask);

/* 2.2 IFx Mask */
void CAN_WriteMaskInfo(uint8_t Ch, CAN_MaskInfoT *pInfo);
void CAN_ReadMaskInfo(uint8_t Ch, CAN_MaskInfoT *pInfo);

/* 2.3 IFx Abr */
void CAN_WriteAbrInfo(uint8_t Ch, CAN_AbrInfoT *pInfo);
void CAN_ReadAbrInfo(uint8_t Ch, CAN_AbrInfoT *pInfo);

/* 2.4 IFx Control */
void CAN_WriteCtrlInfo(uint8_t Ch, CAN_CtrlInfoT *pInfo);
void CAN_ReadCtrlInfo(uint8_t Ch, CAN_CtrlInfoT *pInfo);

/* 2.5 IFx Data */
void CAN_WriteData(uint8_t Ch, uint8_t* pData, uint8_t Size);
void CAN_ReadData(uint8_t Ch, uint8_t* pData, uint8_t Size);

/* 3. Message handler */
FlagStatusT CAN_GetTxRqstFlag(uint8_t Ch, uint8_t MsgBufNum);
FlagStatusT CAN_GetNewDataFlag(uint8_t Ch, uint8_t MsgBufNum);
FlagStatusT CAN_GetIntPendingFlag(uint8_t Ch, uint8_t MsgBufNum);
FlagStatusT CAN_GetMsgValidFlag(uint8_t Ch, uint8_t MsgBufNum);

/* 4. Interrupt callback function */
void CAN_ConfigIntCallback(uint8_t Ch, CAN_IntHandlerArray* IntHandlerArray);

#ifdef __cplusplus
}
#endif

#endif

#endif 
/* _can_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
