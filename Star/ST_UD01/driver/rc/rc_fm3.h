/*!
 *******************************************************************************
 **
 ** \file rc_fm3.h
 **
 ** \brief Remote control module driver head file
 **
 ** \author FSDC AE Team
 **
 ** \version V0.10
 **
 ** \date 2012-08-15
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor Design(Chengdu) Co.,Ltd.
 **
 *******************************************************************************
 **
 ** \note Other information.
 **
 *******************************************************************************
 */ 

#ifndef _RC_FM3_H_
#define _RC_FM3_H_
/*----------------------------------------------------------------------------*/
/* include files                                                              */
/*----------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_RC == OFF)
    #if (PERIPHERAL_ENABLE_RC == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_RC == ON) && (PERIPHERAL_AVAILABLE_RC == ON)
/*----------------------------------------------------------------------------*/
/* constants and macros                                                       */
/*----------------------------------------------------------------------------*/

/**********Remote control module receive configuration macros******************/
#define RC_MODE_CONFIG_MARK                       0x06
#define CEC_REMOTE_CLK_DIV_CONFIG_MARK            0x0FFF

#define RC_ADDRESS1_CONFIG_MARK                   0x1F
#define RC_ADDRESS2_CONFIG_MARK                   0x1F

#define CEC_LOGIC_ADDRESS1_CONFIG_MARK            0x0E
#define CEC_LOGIC_ADDRESS2_CONFIG_MARK            0x0E

#define NEC_RECEIVER_DATA_MARK                    0xFFFFFFFF
#define SIRCS_RECEIVER_DATA_MARK                  0x0000FFFF
#define CEC_RECEIVER_DATA_MARK                    0x000000FF

/********Remote control module transmission configuration macros***************/
#define TX_SIGNAL_FREE_TIME_VALUE_MARK            0x0F

/*! \brief Check RC channel parameter */ 
#define IS_RC_CH_VALID(CH) ((CH == RC_CH0)||(CH == RC_CH1))
                                                        
/*! \brief Check switch value parameter */ 
#define IS_RC_SWITCH_VAL_VALID(SWITCH_VAL)  ((SWITCH_VAL == 0)||(SWITCH_VAL == 1))

/*! \brief Check RC mode parameter */
#define IS_RC_MODE_VALID(MODE)    ((MODE == SIRCS_MODE)||(MODE == NEC_MODE)||\
                                  (MODE == CEC_MODE))
                           
/*! \brief Check channel function parameter */
#define IS_RC_CHANNEL_FUNCTION_VALID(FUNC)  ((FUNC == SIRCS_RECEIVER)||(FUNC == NEC_RECEIVER)||\
                                              (FUNC == CEC_RECEIVER)||(FUNC == CEC_TRANSFER)) 

/*! \brief Check clock div value parameter */
#define IS_RC_CLOCK_DIV_VALUE_VALID(DIV_VALUE)  ((DIV_VALUE<=4095)||(DIV_VALUE>0))

/*! \brief Check clock div value parameter */
#define IS_RC_TX_SIGNAL_FREE_TIME_VALUE_VALID(TIME_VALUE)  ((TIME_VALUE<=15)||(TIME_VALUE>0))
                                
/*----------------------------------------------------------------------------*/
/* \brief CEC Remote Channel Value enumeration                                */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    RC_CH0 = 0,
    RC_CH1 = 1
}ChannelValueT;

/*!
*******************************************************************************
**
** \brief Remote control module ISR number.
**
********************************************************************************
*/
typedef enum 
{
    RC_RX_INT = 0,  
    RC_TX_INT 
}RCIntNumT;

/*----------------------------------------------------------------------------*/
/* \brief  HDMI-CEC/remote module Channel function value enumeration          */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    SIRCS_RECEIVER           = 0,
    NEC_RECEIVER,
    CEC_RECEIVER,
    CEC_TRANSFER
}ChannelFunctionT;

/*----------------------------------------------------------------------------*/
/* \brief FM3 core Remote control module interrupt enable/disable select 
    value enumeration      */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    RC_INT_DISABLE = 0,
    RC_INT_ENABLE  = 1
}RCIntEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief Remote control module start bit interrupt enable/disable select 
   value enumeration      */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    START_BIT_INT_DISABLE  = 0,
    START_BIT_INT_ENABLE   = 1
}StartBitIntEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief Remote control module repeat bit interrupt enable/disable select 
   value enumeration     */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    REPEAT_BIT_INT_DISABLE  = 0,
    REPEAT_BIT_INT_ENABLE   = 1
}RepeatBitIntEnableConfT;


/*----------------------------------------------------------------------------*/
/* \brief  Remote control module ack bit interrupt enable/disable select 
   value enumeration       */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    ACK_INT_DISABLE       = 0,
    ACK_INT_ENABLE        = 1
}ACKIntEnableSelectConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module overflow interrupt enable/disable select 
   value enumeration      */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    OVERFLOW_INT_DISABLE       = 0,
    OVERFLOW_INT_ENABLE        = 1
}OverFlowIntEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module max bit interrupt enable/disable select 
   value enumeration       */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    MAX_BIT_INT_DISABLE       = 0,
    MAX_BIT_INT_ENABLE        = 1
}MaxBitIntEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module min bit interrupt enable/disable select 
   value enumeration       */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    MIN_BIT_INT_DISABLE       = 0,
    MIN_BIT_INT_ENABLE        = 1
}MinBitIntEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module bus error detect interrupt enable/disable 
   select value enumeration */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    BUS_ERROR_INT_DISABLE       = 0,
    BUS_ERROR_INT_ENABLE        = 1
}BusErrorDetectIntEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module Tx status interrupt enable/disable select 
   value enumeration */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    TX_STATUS_INT_DISABLE       = 0,
    TX_STATUS_INT_ENABLE        = 1
}TxStatusIntEnableConfT;


/********Remote control module Receive Configuration Value Enumerations********/
/*----------------------------------------------------------------------------*/
/* \brief  Remote control module operation mode value enumeration             */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    SIRCS_MODE         = 0,
    NONE_USE_MODE,
    NEC_MODE,
    CEC_MODE
}ModeConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module start reception value enumeration            */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    RC_RX_STOP  = 0,   
    RC_RX_START         
}RxStartConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module comparison between reception address and 
           device address enable/disabe select value enumeration   */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    ADDRESS_COMPARE_DISABLE  = 0,   
    ADDRESS_COMPARE_ENABLE         
}AddressCompareEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module threshold selection value enumeration        */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    RC_THRESHOLD_TYPE0  = 0,   /* TYPE0 1:W>A&&W>=B  0:A<W<B      */
    RC_THRESHOLD_TYPE1         /* TYPE1 1:A<W<B      0: W>A&&W>=B */
}ThresholdSelectConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module operating clock value enumeration            */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    SELECT_PERIPHERAL_CLOCK    = 0,   
    SELECT_SUB_CLOCK         
}ClkSouceSelectConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module overflow time select value enumeration       */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    COUNTE_128_CLOCKS_OVERFLOW  = 0,   
    COUNTE_256_CLOCKS_OVERFLOW         
}OverFlowTimeSelectConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module maximum data bit width violation detection 
   enable/disable select value enumeration */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    MAX_DATA_WIDTH_VIOLATION_DETECT_DISABLE = 0,   
    MAX_DATA_WIDTH_VIOLATION_DETECT_ENABLE         
}MaxBitDetectEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module minimum data bit width violation detection 
   enable/disable select value enumeration */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    MIN_DATA_WIDTH_VIOLATION_DETECT_DISABLE = 0,   
    MIN_DATA_WIDTH_VIOLATION_DETECT_ENABLE         
}MinBitDetectEnableConfT;

/*----------------------------------------------------------------------------*/
/* \brief  Remote control module error pulse output enable/disable select 
   value enumeration */
/*----------------------------------------------------------------------------*/
typedef enum 
{
    ERROR_PULSE_OUTPUT_DISABLE = 0,   
    ERROR_PULSE_OUTPUT_ENABLE         
}ErrorPulseOutputEnableConfT;

/*****Remote control module transmission configuration value enumerations******/
typedef enum 
{
    RC_TX_STOP = 0,   
    RC_TX_START         
}TxStartConfT;

typedef enum 
{
    CEC_START_BIT_INVALID = 0,   
    CEC_START_BIT_VALID         
}CECStartBitValidSelectConfT;

typedef enum 
{
    EOM_OUTPUT_0 = 0,   
    EOM_OUTPUT_1         
}CEC_EOMBitOutputValueT;

/*!
********************************************************************************
** \brief HDMI-CEC/remote callback 
********************************************************************************
*/
/* RC transfer */
typedef void RCTRS_HDMICECTxStatusCB(void);
typedef void RCTRS_HDMICECBusErrorCB(void);
/* RC receive */
typedef void RCRCV_ACKCB(void);
typedef void RCRCV_StartDetectCB(void);
typedef void RCRCV_CntOverflowCB(void);
typedef void RCRCV_MaxWidthViolationCB(void);
typedef void RCRCV_MinWidthViolationCB(void);
/*!
********************************************************************************
** \brief HDMI-CEC/remote library functions
********************************************************************************
*/
/* HDMI-CEC receive */                     
void RCRCV_SetMode (ChannelValueT Channel, ModeConfT RCRCVMode)                 ;
ModeConfT RC_GetModeConfig (ChannelValueT Channel)                                ;
void RCRCV_EnableReceive (ChannelValueT Channel)                                  ;
void RCRCV_DisableReceive (ChannelValueT Channel)                                 ;
void RCRCV_SetThreshold (ChannelValueT Channel, 
                         ThresholdSelectConfT ThresholdSelect)                    ;
void RCRCV_SetStartBitWidth (ChannelValueT Channel, 
                             uint8_t StartBitWidthValue)                          ;
void RCRCV_SetDataWidthA (ChannelValueT Channel, 
                          uint8_t DataWidthValueA)                                ;
void RCRCV_SetDataWidthB (ChannelValueT Channel, 
                          uint8_t DataWidthValueB)                                ;
void RCRCV_NECSetRepeatWidth (ChannelValueT Channel, 
                              uint8_t RepeatBitWidthValue)                        ;
void RCRCV_SetRXAddrCMPValue1 (ChannelValueT Channel,                             
                               uint8_t RxAddressCompareValue1)                    ;
void RCRCV_SetRXAddrCMPValue2 (ChannelValueT Channel, 
                               uint8_t RxAddressCompareValue2)                    ;
void RCRCV_HDMICECEnableAddrCMP (ChannelValueT Channel)                           ;
void RCRCV_SIRCSEnableAddrCMP (ChannelValueT Channel)                             ;
void RCRCV_HDMICECDisableAddrCMP (ChannelValueT Channel)                          ;
void RCRCV_SIRCSDisableAddrCMP (ChannelValueT Channel)                            ;
ErrorStatusT RC_SetClockSource (ChannelValueT Channel, 
                                       ClkSouceSelectConfT ClkSouceSelect)               ;
ErrorStatusT RC_SetClockDiv (ChannelValueT Channel, 
                                    uint16_t ClkDivValue)                                ;
void RCRCV_EnableStartInt(ChannelValueT Channel, RCRCV_StartDetectCB* pIntIRQCallback);
void RCRCV_DisableStartInt (ChannelValueT Channel)                                ;
FlagStatusT RCRCV_GetStartIntFlag (ChannelValueT Channel)                         ;
void RCRCV_ClrStartIntFlag (ChannelValueT Channel)                                ;
void RCRCV_EnableACKInt(ChannelValueT Channel, RCRCV_ACKCB* pIntIRQCallback);
void RCRCV_DisableACKInt (ChannelValueT Channel)                                  ;
FlagStatusT RCRCV_GetACKIntFlag (ChannelValueT Channel)                           ;
void RCRCV_ClrACKIntFlag (ChannelValueT Channel)                                  ;
uint8_t RCRCV_GetReceiveEOM (ChannelValueT Channel)                               ;
void RCRCV_ClrReceiveEOM (ChannelValueT Channel)                                  ;
void RCRCV_EnableOverflowInt(ChannelValueT Channel, RCRCV_CntOverflowCB* pIntIRQCallback);
void RCRCV_DisableOverflowInt (ChannelValueT Channel)                             ;
void RCRCV_SetOverflowIntTime (ChannelValueT Channel, 
                               OverFlowTimeSelectConfT OverFlowTimeSelect)       ;
FlagStatusT RCRCV_GetOverflowIntFlag (ChannelValueT Channel)                      ;
void RCRCV_ClrOverflowIntFlag (ChannelValueT Channel)                             ;
void RCRCV_NECEnableRepeatInt (ChannelValueT Channel)                             ;
void RCRCV_NECDisableRepeatInt (ChannelValueT Channel)                            ;
FlagStatusT RCRCV_NECGetRepeatIntFlag (ChannelValueT Channel)                     ;
void RCRCV_NECClrRepeatIntFlag (ChannelValueT Channel)                            ;
void RCRCV_EnableMaxDetection (ChannelValueT Channel)                             ;
void RCRCV_DisableMaxDetection (ChannelValueT Channel)                            ;
void RCRCV_EnableMaxDetectionInt(ChannelValueT Channel, RCRCV_MaxWidthViolationCB* pIntIRQCallback);
void RCRCV_DisableMaxDetectionInt (ChannelValueT Channel)                         ;
void RCRCV_SetMaxBitWidth (ChannelValueT Channel, 
                           uint8_t MaxBitWidthValue)                              ;
FlagStatusT RCRCV_GetMaxBitIntFlag (ChannelValueT Channel)                        ;
void RCRCV_ClrMaxBitIntFlag (ChannelValueT Channel)                               ;
void RCRCV_EnableMinDetection (ChannelValueT Channel)                             ;
void RCRCV_DisableMinDetection (ChannelValueT Channel)                            ;
void RCRCV_EnableMinDetectionInt(ChannelValueT Channel, RCRCV_MinWidthViolationCB* pIntIRQCallback);
void RCRCV_DisableMinDetectionInt (ChannelValueT Channel)                         ;
void RCRCV_EnableErrorPulseOutput (ChannelValueT Channel)                         ;
void RCRCV_DisableErrorPulseOutput (ChannelValueT Channel)                        ;
void RCRCV_SetMinBitWidth (ChannelValueT Channel, 
                           uint8_t MinBitWidthValue)                              ;
FlagStatusT RCRCV_GetMinBitIntFlag (ChannelValueT Channel)                        ;
void RCRCV_ClrMinBitIntFlag (ChannelValueT Channel)                               ;
uint32_t RCRCV_GetReceiveData (ChannelValueT Channel)                             ;
void RCRCV_ClrReceiveData (ChannelValueT Channel)                                 ;
/* HDMI-CEC transfer */
void RCTRS_HDMICECEnableTransmission (ChannelValueT Channel)                             ;
void RCTRS_HDMICECDisableTransmission (ChannelValueT Channel)                            ;
void RCTRS_HDMICECEnableTxStartBit (ChannelValueT Channel)                        ;
void RCTRS_HDMICECDisableTxStartBit (ChannelValueT Channel)                       ;
void RCTRS_HDMICECSetTxEOMOutputValue (ChannelValueT Channel, 
                                       CEC_EOMBitOutputValueT CECEOMBitOutputValue);
uint8_t RCTRS_HDMICECGetTxEOMOutputValue (ChannelValueT Channel)                  ;
void RCTRS_HDMICECSetTxValue (ChannelValueT Channel, uint8_t TxValue)             ;
void RCTRS_HDMICECSetTxSignalFreeTimeValue (ChannelValueT Channel, 
                                                   uint8_t TxSignalFreeTimeValue)        ;
uint8_t HDMICECGetTxACKValue (ChannelValueT Channel)                              ;
void RCTRS_HDMICECEnableTxStatusInt(ChannelValueT Channel, 
                                           RCTRS_HDMICECTxStatusCB* pIRQCallback);
void RCTRS_HDMICECDisableTxStatusInt (ChannelValueT Channel)                      ;
FlagStatusT RCTRS_HDMICECGetTxStatusIntFlag (ChannelValueT Channel)               ;
void RCTRS_HDMICECClrTxStatusIntFlag (ChannelValueT Channel)                      ;
void RCTRS_HDMICECEnableBusErrorDetectInt(ChannelValueT Channel, 
                                                 RCTRS_HDMICECBusErrorCB* pIRQCallback);
void RCTRS_HDMICECDisableBusErrorDetectInt (ChannelValueT Channel)                ;
void RCTRS_HDMICECClrBusErrorDetectIntFlag (ChannelValueT Channel)                ;
FlagStatusT RCTRS_HDMICECGetBusErrorDetectIntFlag (ChannelValueT Channel)         ;
void RCTRS_HDMICECSendData(ChannelValueT Channel, uint8_t Data);
/* Interrupt handler */
void HDMICEC_IntHandler(uint8_t Ch);

#endif

#endif
