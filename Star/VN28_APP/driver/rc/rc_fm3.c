/*!
 *******************************************************************************
 **
 ** \file  rc_fm3.c
 **
 ** \brief Remote control module driver firmware
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


/*----------------------------------------------------------------------------*/
/* include files                                                              */
/*----------------------------------------------------------------------------*/
#include "rc_fm3.h"
/*----------------------------------------------------------------------------*/
/* constants and macros                                                       */
/*----------------------------------------------------------------------------*/

#if (PERIPHERAL_ENABLE_RC == ON) && (PERIPHERAL_AVAILABLE_RC == ON)
 
/*----------------------------------------------------------------------------*/
/* local data                                                                 */
/*----------------------------------------------------------------------------*/
/* Remote control module channel register group point  */
static FM3_HDMICEC_TypeDef* pRC_REGGRP[2] = 
{
    (FM3_HDMICEC_TypeDef*)(FM3_HDMICEC0_BASE),
    (FM3_HDMICEC_TypeDef*)(FM3_HDMICEC1_BASE)
}; 

static RCTRS_HDMICECTxStatusCB *pRC_HDMICECTxStatusCallback[2] = {NULL, NULL};
static RCTRS_HDMICECBusErrorCB* pRC_HDMICECBusErrorCallback[2] = {NULL, NULL };
static RCRCV_ACKCB* pRC_ACKCallback[2] = {NULL, NULL};
static RCRCV_StartDetectCB* pRC_StartDetectCallback[2] = {NULL, NULL};
static RCRCV_CntOverflowCB* pRC_CntOverflowCallback[2] = {NULL, NULL};
static RCRCV_MaxWidthViolationCB* pRC_MaxWidthCallback[2] = {NULL, NULL};
static RCRCV_MinWidthViolationCB* pRC_MinWidthCallback[2] = {NULL, NULL};

/*------------------Remote control module register set------------------------*/

/*!
 *******************************************************************************
 ** \brief Remote control module channel module selection set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \param ModeConfT RCRCVMode 
 **        Remote control module operation mode setting bits configuration 
 **        This parameter can be one of the following values:
 ** \arg   SIRCS_MODE,
 ** \arg   NONE_USE_MODE,
 ** \arg   NEC_MODE,
 ** \arg   RC_MODE       
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetMode(ChannelValueT Channel,ModeConfT RCRCVMode)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    ASSERT(IS_RC_MODE_VALID(RCRCVMode));
    pRC_REGGRP[Channel]->RCCR = (pRC_REGGRP[Channel]->RCCR&(~RC_MODE_CONFIG_MARK))|
              (( RCRCVMode << 1) & RC_MODE_CONFIG_MARK);
}
                         
/*!
 *******************************************************************************
 ** \brief Remote control module get channel mode configuration  
 **
 ** \param ChannelValueT Channel 
 **        The Remote control module get channel 
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 ** 
 ** \return ModeConfT RCRCVMode
 **         Remote control module mode configuration
 ** \arg   SIRCS_MODE 	      
 ** \arg   NONE_USE_MODE
 ** \arg   NEC_MODE
 ** \arg   RC_MODE  
 *******************************************************************************
 */
ModeConfT RC_GetModeConfig(ChannelValueT Channel)
{
    uint8_t RCMode;
 
    ASSERT(IS_RC_CH_VALID(Channel));
    RCMode = pRC_REGGRP[Channel]->RCCR & RC_MODE_CONFIG_MARK;
    RCMode = RCMode>> 1;
    return (ModeConfT)RCMode; 
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable receive data or start receive 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 ** 
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableReceive(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCCR_f.EN  = RC_RX_START;
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable data receive or stop receive
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 ** 
 *******************************************************************************
 */
void RCRCV_DisableReceive(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCCR_f.EN  = RC_RX_STOP;
}

/*!
 *******************************************************************************
 ** \brief Remote control module threshold selection parameter set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1    
 **
 ** \param ThresholdSelectConfT ThresholdSelect 
 **        Remote control module threshold type configuration 
 **        (Use "H" Width Setting Register A/B to set a reference for determining
 **        "0" or "1")
 **        This parameter can be one of the following values:
 ** \arg   RC_THRESHOLD_TYPE0  (TYPE0 1:W>A&&W<=B  0:A<W<B)
 ** \arg   RC_THRESHOLD_TYPE1  (TYPE1 1:A<W<B      0:W>A&&W<=B) 
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetThreshold(ChannelValueT Channel,
                        ThresholdSelectConfT ThresholdSelect)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCCR_f.THSEL  = ThresholdSelect;
}

/*!
 *******************************************************************************
 ** \brief Remote control module start bit width parameters set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1    
 **
 ** \param uint8_t StartBitWidthValue 
 **        Remote control module start bit width clock count value configuration 
 **        (If "H" with a width over the set value is received, 
 **         it is identified as a start bit)
 **        This parameter can be one of the following values:
 ** \arg   0~255(When OVFSEL=0, the set value must be RCSHW <= 127 
 **             (equal to or less than a value not to be detected as overflow))  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetStartBitWidth(ChannelValueT Channel,uint8_t StartBitWidthValue)
{   
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCSHW      =  StartBitWidthValue;
}

/*----------------------------------------------------------------------------*/
/*!
 *******************************************************************************
 ** \brief Remote control module  A width parameters set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1      
 **
 ** \param uint8_t DataWidthValueA 
 **        Remote control module  H width clock count value A configuration
 **        ("H" Width Setting Register A)
 **        This parameter can be one of the following values:
 ** \arg   2~255(Values to be set in this register must be:2 <= RCDAHW < RCDBHW)
 **             (In CEC mode, it must be RCDAHW < 46 
 **             (less than the ACK response pulse width))
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetDataWidthA(ChannelValueT Channel,uint8_t DataWidthValueA)
{   
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCDAHW     =  DataWidthValueA ;
}

/*----------------------------------------------------------------------------*/
/*!
 *******************************************************************************
 ** \brief Remote control module data width B parameters set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1    
 **
 ** (In CEC mode, it must be RCDAHW < 46 (less than the ACK response pulse width))
 **
 ** \param uint8_t DataWidthValueB 
 **        Remote control module  H width clock count value B configuration
 **        ("H" Width Setting Register B)
 **        This parameter can be one of the following values:
 ** \arg   3~255(Be sure to set a value: RCDAHW < RCDBHW < RCSHW)
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetDataWidthB(ChannelValueT Channel,uint8_t DataWidthValueB)
{   
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCDBHW     =  DataWidthValueB ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module repeat width parameter set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1    
 **
 ** \param uint8_t RepeatBitWidthValue 
 **        Remote control module repeat bit width clock count value configuration
 **        (Used to set a ¡°H¡± width used for determining a repeat code)
 **        This parameter can be one of the following values:
 ** \arg   0~255(A value to be set to this register must be RCRHW < RCSHW) 
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_NECSetRepeatWidth(ChannelValueT Channel,uint8_t RepeatBitWidthValue)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCRHW  = RepeatBitWidthValue; 
}

/*!
 *******************************************************************************
 ** \brief Remote control module address compare receive address value1 set 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \param uint8_t RxAddressCompareValue1 
 **        Remote control module device address1 configuration
 **        This parameter can be one of the following values:
 ** \arg   0~31(When CEC mode do not set "0x0F" (broadcast address) to this register)
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetRXAddrCMPValue1(ChannelValueT Channel,uint8_t RxAddressCompareValue1)              
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCADR1 = ((pRC_REGGRP[Channel]->RCADR1&(~RC_ADDRESS1_CONFIG_MARK))| 
                                  (RxAddressCompareValue1 & RC_ADDRESS1_CONFIG_MARK));
}

/*!
 *******************************************************************************
 ** \brief Remote control module address compare receive address value2 set 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \param uint8_t RxAddressCompareValue2
 **        Remote control module device address2 configuration
 **        This parameter can be one of the following values:
 ** \arg   0~31(When CEC mode do not set "0x0F" (broadcast address) to this register)
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetRXAddrCMPValue2(ChannelValueT Channel,uint8_t RxAddressCompareValue2)              
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCADR2 = ((pRC_REGGRP[Channel]->RCADR2&(~RC_ADDRESS2_CONFIG_MARK)) | 
                                  (RxAddressCompareValue2 & RC_ADDRESS2_CONFIG_MARK));
}
/*!
 *******************************************************************************
 ** \brief Remote control module HDMI CEC address compare enable set 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_HDMICECEnableAddrCMP(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCCR_f.ADRCE  = ADDRESS_COMPARE_ENABLE;
}

/*!
 *******************************************************************************
 ** \brief Remote control module SIRCS address compare enable set 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SIRCSEnableAddrCMP(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCCR_f.ADRCE  = ADDRESS_COMPARE_ENABLE;
}

/*!
 *******************************************************************************
 ** \brief Remote control module HDMI CEC address compare disable set 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_HDMICECDisableAddrCMP(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCCR_f.ADRCE  = ADDRESS_COMPARE_DISABLE;
}

/*!
 *******************************************************************************
 ** \brief Remote control module SIRCS address compare disable set 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SIRCSDisableAddrCMP(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCCR_f.ADRCE  = ADDRESS_COMPARE_DISABLE;
}

/*!
 *******************************************************************************
 ** \brief Remote control module source clock selection 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \param ClkSouceSelectConfT ClkSouceSelect 
 **        Remote control module operating source clock selection configuration,
 **        which use for Rx and Tx clock and count width(after division)
 **        The "PERIPHERAL_CLOCK" is APB2 bus clock (PCLK2),
 **        if select "SELECT_PERIPHERAL_CLOCK" PCLK2 should be initial first 
 **        This parameter can be one of the following values:
 ** \arg   SELECT_PERIPHERAL_CLOCK    
 ** \arg   SELECT_SUB_CLOCK  
 **
 ** \return ErrorStatusT Ret  
 **         CEC clock source configuration result
 ** \arg    RET_ERROR  
 ** \arg    RET_SUCCESS 
 **
 *******************************************************************************
 */
ErrorStatusT RC_SetClockSource(ChannelValueT Channel,ClkSouceSelectConfT ClkSouceSelect)                                 
{
    ASSERT(IS_RC_CH_VALID(Channel));
    ASSERT(IS_RC_SWITCH_VAL_VALID(ClkSouceSelect));
    
    if(ClkSouceSelect == SELECT_SUB_CLOCK)
    {
        if((!(FM3_CRG->SCM_STR & (1UL << 3))))
        {
            return RET_ERROR;
            /* #error "Sub clock is not ready."  */
        }
        FM3_DS->RCK_CTL_f.CECCKE = SET; 
    }
    pRC_REGGRP[Channel]->RCCKD_f.CKSEL = ClkSouceSelect; 
    
    return RET_SUCCESS;
}
/*!
 *******************************************************************************
 ** \brief Remote control module source clock division set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \param uint16_t ClkDivValue 
 **        Remote control module operating clock division ratio value configuration
 **        (Division ratio becomes CKDIV + 1,1 division means no division)
 **        Propose division peripheral clock ratio to near 32KHz
 **        This parameter can be one of the following values:
 ** \arg   0~4095(no division if CKSEL=1(SELECT_SUB_CLOCK))         
 **
 ** \return ErrorStatusT Ret  
 **         RC clock division configuration result
 ** \arg    RET_ERROR  
 ** \arg    RET_SUCCESS 
 **
 *******************************************************************************
 */
ErrorStatusT RC_SetClockDiv(ChannelValueT Channel,uint16_t ClkDivValue)                                 
{
    ASSERT(IS_RC_CH_VALID(Channel));
    ASSERT(IS_RC_CLOCK_DIV_VALUE_VALID(ClkDivValue));
    if(ClkDivValue > 4095)
    {
        return RET_ERROR;
        /* #error "Be sure to set a value: 0~4095(division ratio becomes CKDIV + 1),*/
        /* 1 division (no division) through 4096 division can be set." */
    }
    pRC_REGGRP[Channel]->RCCKD  = ((pRC_REGGRP[Channel]->RCCKD&(~CEC_REMOTE_CLK_DIV_CONFIG_MARK)) | 
                         (ClkDivValue & CEC_REMOTE_CLK_DIV_CONFIG_MARK)); 
                         /* no division if CKSEL=1(SELECT_SUB_CLOCK) */
    return RET_SUCCESS;
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable start bit interrupt 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableStartInt(ChannelValueT Channel, RCRCV_StartDetectCB* pIntIRQCallback)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_StartDetectCallback[Channel] = pIntIRQCallback;
    pRC_REGGRP[Channel]->RCST_f.ST       =  RESET;
    pRC_REGGRP[Channel]->RCST_f.STIE     =  START_BIT_INT_ENABLE;  
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable start bit interrupt 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_DisableStartInt(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCST_f.STIE     =  START_BIT_INT_DISABLE;  
}

/*!
 *******************************************************************************
 ** \brief Remote control module get start bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return FlagStatusT Flag value
 **
 *******************************************************************************
 */
FlagStatusT RCRCV_GetStartIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    return (FlagStatusT)pRC_REGGRP[Channel]->RCST_f.ST;
}

/*!
 *******************************************************************************
 ** \brief Remote control module clear start bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_ClrStartIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCST_f.ST = RESET;
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable ACK bit interrupt 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableACKInt(ChannelValueT Channel, RCRCV_ACKCB* pIntIRQCallback)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_ACKCallback[Channel] = pIntIRQCallback;
    pRC_REGGRP[Channel]->RCST_f.ACK    =  RESET;   
    pRC_REGGRP[Channel]->RCST_f.ACKIE  =  ACK_INT_ENABLE;
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable ACK bit interrupt 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_DisableACKInt(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCST_f.ACKIE      =  ACK_INT_DISABLE;
}

/*!
 *******************************************************************************
 ** \brief Remote control module get HDMI CEC ACK bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return FlagStatusT  Flag value
 **
 *******************************************************************************
 */
FlagStatusT  RCRCV_GetACKIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    return (FlagStatusT)pRC_REGGRP[Channel]->RCST_f.ACK;
}

/*!
 *******************************************************************************
 ** \brief Remote control module  clear ACK bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_ClrACKIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCST_f.ACK = RESET;
}

/*!
 *******************************************************************************
 ** \brief Remote control module get receive EOM bit value
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return uint8_t EOM value
 **
 *******************************************************************************
 */
uint8_t RCRCV_GetReceiveEOM(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    return (FlagStatusT)pRC_REGGRP[Channel]->RCST_f.EOM;
}

/*!
 *******************************************************************************
 ** \brief Remote control module clear receive EOM bit value
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_ClrReceiveEOM(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCST_f.EOM = RESET;
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable overflow interrupt 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableOverflowInt(ChannelValueT Channel, RCRCV_CntOverflowCB* pIntIRQCallback)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_CntOverflowCallback[Channel] = pIntIRQCallback;
    pRC_REGGRP[Channel]->RCST_f.OVFIE  =  OVERFLOW_INT_ENABLE  ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable overflow interrupt 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_DisableOverflowInt(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCST_f.OVFIE  =  OVERFLOW_INT_DISABLE  ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module overflow interrupt time set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \param OverFlowTimeSelectConfT OverFlowTimeSelect 
 **        Remote control module counter overflow detection condition configuration)
 **        This parameter can be one of the following values:
 ** \arg   COUNTE_128_CLOCKS_OVERFLOW   
 ** \arg   COUNTE_256_CLOCKS_OVERFLOW  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetOverflowIntTime(ChannelValueT Channel,
                              OverFlowTimeSelectConfT OverFlowTimeSelect)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    ASSERT(IS_RC_SWITCH_VAL_VALID(OverFlowTimeSelect));
    pRC_REGGRP[Channel]->RCST_f.OVFSEL =  OverFlowTimeSelect  ; 
}

/*!
 *******************************************************************************
 ** \brief Remote control module get overflow bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return FlagStatusT  Flag value
 **
 *******************************************************************************
 */
FlagStatusT RCRCV_GetOverflowIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    return (FlagStatusT)pRC_REGGRP[Channel]->RCST_f.OVF;
}

/*!
 *******************************************************************************
 ** \brief Remote control module  clear overflow bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_ClrOverflowIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCST_f.OVF = RESET;
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable repeat bit interrupt
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_NECEnableRepeatInt(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCRC_f.RC     =  RESET;
    pRC_REGGRP[Channel]->RCRC_f.RCIE   =  REPEAT_BIT_INT_ENABLE; 
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable repeat bit interrupt
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_NECDisableRepeatInt(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCRC_f.RCIE     =  REPEAT_BIT_INT_DISABLE; 
}

/*!
 *******************************************************************************
 ** \brief Remote control module get repeat bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return FlagStatusT Flag value
 **
 *******************************************************************************
 */
FlagStatusT RCRCV_NECGetRepeatIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    return (FlagStatusT)pRC_REGGRP[Channel]->RCRC_f.RC;
}

/*!
 *******************************************************************************
 ** \brief Remote control module clear repeat bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_NECClrRepeatIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCRC_f.RC = RESET;
}

/*!
 *******************************************************************************
 ** \brief  Remote control module enable maximum bit interrupt detection
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableMaxDetection(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLE_f.LELE    =  MAX_DATA_WIDTH_VIOLATION_DETECT_ENABLE; 
}

/*!
 *******************************************************************************
 ** \brief  Remote control module disable maximum bit interrupt detection
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_DisableMaxDetection(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLE_f.LELE    =  MAX_DATA_WIDTH_VIOLATION_DETECT_DISABLE; 
}

/*!
 *******************************************************************************
 ** \brief  Remote control module enable maximum bit detection interrupt 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableMaxDetectionInt(ChannelValueT Channel, RCRCV_MaxWidthViolationCB* pIntIRQCallback)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_MaxWidthCallback[Channel] = pIntIRQCallback;
    pRC_REGGRP[Channel]->RCLE_f.LEL     =  RESET;
    pRC_REGGRP[Channel]->RCLE_f.LELIE   =  MAX_BIT_INT_ENABLE; 
}

/*!
 *******************************************************************************
 ** \brief  Remote control module disable maximum bit detection interrupt 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_DisableMaxDetectionInt(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLE_f.LELIE   =  MAX_BIT_INT_DISABLE  ; 
}

/*!
 *******************************************************************************
 ** \brief  Remote control module maximum bit width set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \param uint8_t MaxBitWidthValue 
 **        Remote control module maximum data bit width clock count value 
 **        configuration(If a data bit with a width more than RCLESW is received,
 **        it will be detected as a maximum data bit width)
 **        This parameter can be one of the following values:
 ** \arg   0~255
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetMaxBitWidth(ChannelValueT Channel,uint8_t MaxBitWidthValue)
{
    ASSERT(IS_RC_CH_VALID(Channel)); 
    pRC_REGGRP[Channel]->RCLELW         =  MaxBitWidthValue;
}

/*!
 *******************************************************************************
 ** \brief Remote control module get maximum bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return FlagStatusT  Flag value
 **
 *******************************************************************************
 */
FlagStatusT RCRCV_GetMaxBitIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    return (FlagStatusT)pRC_REGGRP[Channel]->RCLE_f.LEL;
}

/*!
 *******************************************************************************
 ** \brief Remote control module  clear maximum bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_ClrMaxBitIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLE_f.LEL = RESET;
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable minimum bit interrupt detection
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableMinDetection(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLE_f.LESE    =  MIN_DATA_WIDTH_VIOLATION_DETECT_ENABLE; 
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable minimum bit interrupt detection
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_DisableMinDetection(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLE_f.LESE    =  MIN_DATA_WIDTH_VIOLATION_DETECT_DISABLE; 
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable minimum data bit detection interrupt  
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableMinDetectionInt(ChannelValueT Channel, RCRCV_MinWidthViolationCB* pIntIRQCallback)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_MinWidthCallback[Channel] = pIntIRQCallback;
    pRC_REGGRP[Channel]->RCLE_f.LESIE   =  MIN_BIT_INT_ENABLE  ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable minimum bit detection interrupt  
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_DisableMinDetectionInt(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLE_f.LESIE   =  MIN_BIT_INT_DISABLE  ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable error pulse output 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_EnableErrorPulseOutput(ChannelValueT Channel)
{
     ASSERT(IS_RC_CH_VALID(Channel));
     pRC_REGGRP[Channel]->RCLE_f.EPE  =  ERROR_PULSE_OUTPUT_ENABLE  ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable error pulse output 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_DisableErrorPulseOutput(ChannelValueT Channel)
{
     ASSERT(IS_RC_CH_VALID(Channel));
     pRC_REGGRP[Channel]->RCLE_f.EPE  =  ERROR_PULSE_OUTPUT_DISABLE  ;
}

/*!
 *******************************************************************************
 ** \brief  Remote control module minimum bit width set 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \param uint8_t MinBitWidthValue 
 **        Remote control module minimum data bit width clock count value configuration
 **        (If a data bit with a width more than RCLELW is received, 
 **        it will be detected as a minimum data bit width)
 **        This parameter can be one of the following values:
 ** \arg   0~255
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_SetMinBitWidth(ChannelValueT Channel,uint8_t MinBitWidthValue)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLESW         =  MinBitWidthValue; 
}

/*!
 *******************************************************************************
 ** \brief Remote control module get minimum bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return FlagStatusT  Flag value
 **
 *******************************************************************************
 */
FlagStatusT  RCRCV_GetMinBitIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    return (FlagStatusT )pRC_REGGRP[Channel]->RCLE_f.LES;
}

/*!
 *******************************************************************************
 ** \brief Remote control module clear minimum bit interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_ClrMinBitIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCLE_f.LES = RESET;
}

/*!
 *******************************************************************************
 ** \brief Remote control module get receive data ,just read the receive register
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \return uint32_t receive data
 **
 *******************************************************************************
 */
uint32_t RCRCV_GetReceiveData(ChannelValueT Channel)
{
    uint32_t ReceiveDataTemp = 0;
    
    ASSERT(IS_RC_CH_VALID(Channel));
    ReceiveDataTemp = (pRC_REGGRP[Channel]->RCDTHH)<<24;     
    ReceiveDataTemp = ReceiveDataTemp | (pRC_REGGRP[Channel]->RCDTHL)<<16;         
    ReceiveDataTemp = ReceiveDataTemp | (pRC_REGGRP[Channel]->RCDTLH)<<8;     
    ReceiveDataTemp = ReceiveDataTemp | pRC_REGGRP[Channel]->RCDTLL;    
    return ReceiveDataTemp;
}

/*!
 *******************************************************************************
 ** \brief Remote control module clear receive data
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCRCV_ClrReceiveData(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->RCDTHH = 0;     
    pRC_REGGRP[Channel]->RCDTHL = 0;          
    pRC_REGGRP[Channel]->RCDTLH = 0;  
    pRC_REGGRP[Channel]->RCDTLL = 0;  
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable transmission 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **    
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECEnableTransmission(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->TXCTRL_f.TXEN  = RC_TX_START;
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable transmission 
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **    
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECDisableTransmission(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->TXCTRL_f.TXEN  = RC_TX_STOP;
}

/*!
 *******************************************************************************
 ** \brief Remote control module transmission start bit valid select set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECEnableTxStartBit(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->TXCTRL_f.START  = CEC_START_BIT_VALID;
}

/*!
 *******************************************************************************
 ** \brief Remote control module transmission start bit invalid select set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECDisableTxStartBit(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->TXCTRL_f.START  = CEC_START_BIT_INVALID;
}

/*!
 *******************************************************************************
 ** \brief Remote control module set transmission EOM bit output value
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \param EOMBitOutputValueT CECEOMBitOutputValue
 **        This controls EOM transmission bit output 0 or 1
 **        This parameter can be one of the following values:
 ** \arg   EOM_OUTPUT_0
 ** \arg   EOM_OUTPUT_1   
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECSetTxEOMOutputValue(ChannelValueT Channel, 
                                      CEC_EOMBitOutputValueT CECEOMBitOutputValue)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    ASSERT(IS_RC_SWITCH_VAL_VALID(CECEOMBitOutputValue));
    pRC_REGGRP[Channel]->TXCTRL_f.EOM = CECEOMBitOutputValue;
}

/*!
 *******************************************************************************
 ** \brief Remote control module get transmission EOM bit output value
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return CECEOMBitOutputValueT GetCECEOMBitOutputValue
 **        The controls EOM transmission bit output value
 **        This parameter can be one of the following values:
 ** \arg   EOM_OUTPUT_0
 ** \arg   EOM_OUTPUT_1   
 **
 *******************************************************************************
 */
uint8_t RCTRS_HDMICECGetTxEOMOutputValue(ChannelValueT Channel)
{
    CEC_EOMBitOutputValueT GetCECEOMBitOutputValue;
    
    ASSERT(IS_RC_CH_VALID(Channel));
    GetCECEOMBitOutputValue = (CEC_EOMBitOutputValueT)pRC_REGGRP[Channel]->TXCTRL_f.EOM ;
    return GetCECEOMBitOutputValue;
}

/*!
 *******************************************************************************
 ** \brief Remote control module set transmission output value
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \param uint8 TxValue
 **        The output transmission value
 **        This parameter can be one of the following values:
 ** \arg   0~255 
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECSetTxValue(ChannelValueT Channel,uint8_t TxValue)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->TXDATA = TxValue;
}

/*!
 *******************************************************************************
 ** \brief Remote control module transmission signal free time set
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 ** 
 ** \param uint8_t TxSignalFreeTimeValue
 **        These bits are used to set a time to check free state on the Remote 
 **        control module bus before starting transmission
 **        This parameter can be one of the following values:
 ** \arg   0~15(time is (Set value + 1) cycle)
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECSetTxSignalFreeTimeValue(ChannelValueT Channel,
                                           uint8_t TxSignalFreeTimeValue)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    ASSERT(IS_RC_TX_SIGNAL_FREE_TIME_VALUE_VALID(TxSignalFreeTimeValue));
    pRC_REGGRP[Channel]->SFREE =  (pRC_REGGRP[Channel]->SFREE&(~TX_SIGNAL_FREE_TIME_VALUE_MARK))| 
                                  (TxSignalFreeTimeValue & TX_SIGNAL_FREE_TIME_VALUE_MARK);
}

/*!
 *******************************************************************************
 ** \brief Remote control module transmission get transmission ACK value  
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 ** 
 ** \return uint8_t TxACKValue
 **
 *******************************************************************************
 */
uint8_t HDMICECGetTxACKValue(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    return (FlagStatusT )pRC_REGGRP[Channel]->TXSTS_f.ACKSV;
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable Tx status interrupt set
 **        When communication of a status bit at 10 bit in each block transfer is 
 **        completed,the ITST bit will be set to "1".
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECEnableTxStatusInt(ChannelValueT Channel, RCTRS_HDMICECTxStatusCB* pIRQCallback)
{
   ASSERT(IS_RC_CH_VALID(Channel));
   pRC_HDMICECTxStatusCallback[Channel] = pIRQCallback;
   pRC_REGGRP[Channel]->TXSTS_f.ITST     =  RESET;
   pRC_REGGRP[Channel]->TXCTRL_f.ITSTEN  =  TX_STATUS_INT_ENABLE  ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable Tx status interrupt 
 **        When communication of a status bit at 10 bit in each block transfer is 
 **        completed, the ITST bit will be set to "1".
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECDisableTxStatusInt(ChannelValueT Channel)
{
   ASSERT(IS_RC_CH_VALID(Channel));
   pRC_REGGRP[Channel]->TXCTRL_f.ITSTEN  =  TX_STATUS_INT_DISABLE  ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module get Tx Status interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return FlagStatusT  Flag value
 **
 *******************************************************************************
 */
FlagStatusT RCTRS_HDMICECGetTxStatusIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));    
    return (FlagStatusT )pRC_REGGRP[Channel]->TXSTS_f.ITST;
}

/*!
 *******************************************************************************
 ** \brief Remote control module clear Tx status interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECClrTxStatusIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->TXSTS_f.ITST = RESET;
}

/*!
 *******************************************************************************
 ** \brief Remote control module enable Tx bus error detect interrupt
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 ** 
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECEnableBusErrorDetectInt(ChannelValueT Channel, RCTRS_HDMICECBusErrorCB* pIRQCallback)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_HDMICECBusErrorCallback[Channel] = pIRQCallback;
    pRC_REGGRP[Channel]->TXSTS_f.IBR     =  RESET;  
    pRC_REGGRP[Channel]->TXCTRL_f.IBREN  =  BUS_ERROR_INT_ENABLE  ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module disable Tx bus error detect interrupt
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 ** 
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECDisableBusErrorDetectInt(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->TXCTRL_f.IBREN  =  BUS_ERROR_INT_DISABLE  ;
}
/*!
 *******************************************************************************
 ** \brief Remote control module clear Tx Bus error detect interrupt
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECClrBusErrorDetectIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));
    pRC_REGGRP[Channel]->TXSTS_f.IBR = RESET;
}

/*!
 *******************************************************************************
 ** \brief Remote control module get Tx bus error detect interrupt flag
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return FlagStatusT  Flag value
 **
 *******************************************************************************
 */
FlagStatusT RCTRS_HDMICECGetBusErrorDetectIntFlag(ChannelValueT Channel)
{
    ASSERT(IS_RC_CH_VALID(Channel));    
    return (FlagStatusT)pRC_REGGRP[Channel]->TXSTS_f.IBR ;
}

/*!
 *******************************************************************************
 ** \brief Remote control module send one byte data
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 ** \param Data Send data
 **  
 ** \return None
 **
 *******************************************************************************
 */
void RCTRS_HDMICECSendData(ChannelValueT Channel, uint8_t Data)
{
    ASSERT(IS_RC_CH_VALID(Channel));  
    pRC_REGGRP[Channel]->TXDATA = Data;
    return;
}

/*!
 *******************************************************************************
 ** \brief Remote control module interrupt handles
 **
 ** \param ChannelValueT Channel
 **        Remote control module channel which set to
 **        This parameter can be one of the following values:
 ** \arg   RC_CH0
 ** \arg   RC_CH1  
 **  
 ** \return None
 **
 *******************************************************************************
 */
void HDMICEC_IntHandler(uint8_t Ch)
{
    /* Remote control transfer */
    if((pRC_REGGRP[Ch]->RCST_f.ST == SET) && 
       (pRC_REGGRP[Ch]->RCST_f.STIE == SET))
    {
        pRC_REGGRP[Ch]->RCST_f.ST = RESET;
        if(pRC_StartDetectCallback[Ch] != NULL)
        {
            pRC_StartDetectCallback[Ch]();
        }
    }
  
  
    if((pRC_REGGRP[Ch]->RCST_f.ACK == SET) && 
       (pRC_REGGRP[Ch]->RCST_f.ACKIE == SET))
    {
        pRC_REGGRP[Ch]->RCST_f.ACK = RESET;
        if(pRC_ACKCallback[Ch] != NULL)
        {
            pRC_ACKCallback[Ch]();
        }
        
    }
    
    if((pRC_REGGRP[Ch]->RCST_f.OVF == SET) && 
       (pRC_REGGRP[Ch]->RCST_f.OVFIE == SET))
    {
        pRC_REGGRP[Ch]->RCST_f.OVF = RESET;
        if(pRC_CntOverflowCallback[Ch] != NULL)
        {
            pRC_CntOverflowCallback[Ch]();
        }
    }
    
    if((pRC_REGGRP[Ch]->RCLE_f.LEL == SET) && 
       (pRC_REGGRP[Ch]->RCLE_f.LELIE == SET))
    {
        pRC_REGGRP[Ch]->RCLE_f.LEL = RESET;
        if(pRC_MaxWidthCallback[Ch] != NULL)
        {   
            pRC_MaxWidthCallback[Ch]();
        }
    }
    
    if((pRC_REGGRP[Ch]->RCLE_f.LES == SET) && 
       (pRC_REGGRP[Ch]->RCLE_f.LESIE == SET))
    {
        pRC_REGGRP[Ch]->RCLE_f.LES = RESET;
        if(pRC_MinWidthCallback[Ch] != NULL)
        {   
            pRC_MinWidthCallback[Ch]();
        }
    }
    
    /* HDMI-CEC transfer */
    if((pRC_REGGRP[Ch]->TXSTS_f.IBR == SET) &&
       (pRC_REGGRP[Ch]->TXCTRL_f.IBREN == SET))
    {
        pRC_REGGRP[Ch]->TXSTS_f.IBR = RESET;
        if(pRC_HDMICECBusErrorCallback[Ch] != NULL)
        {
            pRC_HDMICECBusErrorCallback[Ch]();
        }
    }

    if((pRC_REGGRP[Ch]->TXSTS_f.ITST == SET) && 
       (pRC_REGGRP[Ch]->TXCTRL_f.ITSTEN == SET))
    {
        pRC_REGGRP[Ch]->TXSTS_f.ITST = RESET;
        if(pRC_HDMICECTxStatusCallback[Ch] != NULL)
        {
            pRC_HDMICECTxStatusCallback[Ch]();
        }
    }
}

#endif
