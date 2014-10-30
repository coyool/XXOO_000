/*!
 ******************************************************************************
 **
 ** \file can_fm3.c
 **
 ** \brief can drivers
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
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "can_fm3.h"

/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

#if (PERIPHERAL_AVAILABLE_CAN == ON) && (PERIPHERAL_ENABLE_CAN == ON)
/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief CAN channel0 and channel 1 register
 ******************************************************************************
 */ 
static FM3_CAN_TypeDef* pFM3_CAN_REG[2] = 
{
    (FM3_CAN_TypeDef*)(FM3_CAN0),
    (FM3_CAN_TypeDef*)(FM3_CAN1),
};

/*!
 ******************************************************************************
 ** \brief CAN callback function array
 ******************************************************************************
*/ 
CAN_IntHandlerArray CANIntArray[CAN_CH_MAX] = 
{
    NULL,
    NULL,
};

/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Config the prescaler divided
 **
 ** \param PrescalerValue
 ** \arg   CAN_PrescalerNoDiv~CAN_Prescaler12Div
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_SetClkDiv(CAN_ClkDivT Div)
{
    FM3_CANPRES->CANPRE = Div;
}

/*!
 ******************************************************************************
 ** \brief enable test mode
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_EnableTestMode(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.TEST = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable test mode, mean normal operation
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_DisableTestMode(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.TEST = 0;
}

/*!
 ******************************************************************************
 ** \brief enable write access to the CAN bit timming register(BTR) and CAN  
 **  prescaler extension register (BRPER)
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_EnableBitTimingSetting(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.CCE = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable write access to the CAN bit timming register(BTR) and CAN  
 **  prescaler extension register (BRPER)
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_DisableBitTimingSetting(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.CCE = 0;
}

/*!
 ******************************************************************************
 ** \brief enable automatic retransmission when arbitration is lost or an  error
 **  is detected
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_EnableAutoTrsf(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.DAR = 0;
}

/*!
 ******************************************************************************
 ** \brief Disable automatic retransmission when arbitration is lost or an  error
 **  is detected
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_DisableAutoTrsf(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.DAR = 1;
}

/*!
 ******************************************************************************
 ** \brief Enable the setting of status interrupt code in the CAN interrupt
 **  register when a change of the BOFF or EWarn bit in the CAN status register
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_EnableErrIntCode(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.EIE = 1;
}

/*!
 ******************************************************************************
 ** \brief Disable the setting of status interrupt code in the CAN interrupt
 **  register when a change of the BOFF or EWarn bit in the CAN status register
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_DisableErrIntCode(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.EIE = 0;
}

/*!
 ******************************************************************************
 ** \brief enbale the setting of status interrupt code in the CAN interrupt register
 **  when a change of the TxOK,RxOK, or LEC bit in the CAN status resgister
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_EnableStatIntCode(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.SIE = 1;
}

/*!
 ******************************************************************************
 ** \brief disable the setting of status interrupt code in the CAN interrupt register
 **  when a change of the TxOK,RxOK, or LEC bit in the CAN status resgister
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_DisableStatIntCode(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.SIE = 0;
}

/*!
 ******************************************************************************
 ** \brief enbale interupt generation
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_EnableInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.IE = 1;
}

/*!
 ******************************************************************************
 ** \brief disable interupt generation
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_DisableInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.IE = 0;
}

/*!
 ******************************************************************************
 ** \brief CAN Controller initialization
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \param CAN_OptModeT Mode
 ** \arg   CAN_INIT       CAN initialization
 **        CAN_OPERATION  CAN controller can operate
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_SetOptMode(uint8_t Ch, CAN_OptModeT Mode)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->CTRLR_f.INIT = Mode;
}

/*!
 ******************************************************************************
 ** \brief get can state
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **
 ** \param Type
 ** \arg   enum of CAN_StatusT, 
 **        contain CAN_BUS_OFF,CAN_WARNING,CAN_ERROR_PASSIVE,CAN_RX_OK,CAN_TX_OK
 **
 ** \retval 1: SET
 **         0: RESET
 **
 ******************************************************************************
 */
FlagStatusT CAN_GetStatus(uint8_t Ch, CAN_StatusT Type)
{
    uint8_t retval;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    switch (Type)
    {
    case CAN_BUS_OFF:
      retval = pFM3_CAN_REG[Ch]->STATR_f.BOFF;
      break;
      
    case CAN_WARNING:
      retval = pFM3_CAN_REG[Ch]->STATR_f.EWARM;
      break;
      
    case CAN_ERROR_PASSIVE:
      retval = pFM3_CAN_REG[Ch]->STATR_f.EPASS;
      break;
      
    case CAN_RX_OK:
      retval = pFM3_CAN_REG[Ch]->STATR_f.RXOK;
      break;
      
    case CAN_TX_OK:
      retval = pFM3_CAN_REG[Ch]->STATR_f.TXOK;
      break;
      
    default : break;
    }
    
    return (FlagStatusT)retval;
}

/*!
 ******************************************************************************
 ** \brief Get last error code bits
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval 0: normal, successful transmission or reception
 **         1: stuff  error, six or mor dominant or recessive bits have been detected
 **            consecutively in a message
 **         2: form error, a wrong fixed format part of a received frame has been detected
 **         3: Ack error, a sent message was not acknowledged by anoter node
 **         4: bit 1 error, in the sent message data excluding the arbitration field, 
 **            bits that have been sent as recessive data is detected as dominant data
 **         5: Bit 0 error, in the sent message data excluding the arbitration field, 
 **            bits that have been sent as dominant data is detected as recessive d ata
 **         6: CRC error, the CRC data in a received message did not match with the calculated CRC value
 **         7: Undetected, the bus is in idle state
 ******************************************************************************
*/
CAN_LastErrCodeT CAN_GetLastErrCode(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    return (CAN_LastErrCodeT)(pFM3_CAN_REG[Ch]->STATR & 0x0007);
}

/*!
 ******************************************************************************
 ** \brief Clear RxOK  flag
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval none
 ** 
 ******************************************************************************
 */
void CAN_ClrRxOKFlag(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->STATR_f.RXOK = 0;
}

/*!
 ******************************************************************************
 ** \brief Clear TxOK  flag
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval none
 ** 
 ******************************************************************************
 */
void CAN_ClrTxOKFlag(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->STATR_f.TXOK = 0;
}

/*!
 ******************************************************************************
 ** \brief Clear last error code  flag
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval none
 ** 
 ******************************************************************************
 */
void CAN_ClrLastErrCodeFlag(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->STATR &= 0xF8; 
}
     
/*!
 ******************************************************************************
 ** \brief Get receive error passive indication
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval 1: the receive error counter has reaached the error passive level 
 **            defined in the CAN specification
 **         0: the receive error counter is below the error  passive level
 ** 
 ******************************************************************************
 */
FlagStatusT CAN_GetRcvPasFlag(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    return (FlagStatusT)pFM3_CAN_REG[Ch]->ERRCNT_f.RP;
}

/*!
 ******************************************************************************
 ** \brief Get receive error counter
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval 0~127 : receive error counter value.
 ** 
 ******************************************************************************
 */
uint8_t CAN_GetRcvErrCnt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    return (pFM3_CAN_REG[Ch]->ERRCNT & 0x7F00) >> 8;
}


/*!
 ******************************************************************************
 ** \brief Get send error counter
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval 0~255 : send error counter value.
 ** 
 ******************************************************************************
 */
uint8_t CAN_GetTrsfErrCnt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    return (pFM3_CAN_REG[Ch]->ERRCNT & 0x00FF);
}

/*!
 ******************************************************************************
 ** \brief Configure bit timing register(BTP and BRPE) include  TSeg2,TSeg1,SJW
 **        ,BRP,BRPE
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \param CAN_BitTimingInfoT
 ** \arg   the value of TSeg2,TSeg1,SJW,BRP,BRPE
 **         
 ** \retval none
 ** 
 ******************************************************************************
 */
void CAN_ConfigBitTiming(uint8_t Ch, CAN_BitTimingInfoT *pInfo)
{
    uint16_t tBTR;
    
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tBTR = (pInfo->TSeg2<<12)+(pInfo->TSeg1<<8)+(pInfo->SWJ<<6);
    tBTR |= (pInfo->BRP -  1) & 0x003F;
    
    pFM3_CAN_REG[Ch]->BTR = tBTR;
    pFM3_CAN_REG[Ch]->BRPER = ((pInfo->BRP - 1) &  0x03C0) >> 6;
}

/*!
 ******************************************************************************
 ** \brief Get interrupt code
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval 0x0000: No interrupt
 **         0x0001~0x0020: an interrupt cause indicates a message object number
 **                        (massage interrupt code)
 **         0x0021~0x7FFF: Unused
 **         0x8000: indicates an interrupt by a change in the CAN status register
 **                 (status interrupt code)
 **         0x8001~0xFFFF: Unused
 ** 
 ******************************************************************************
 */
uint16_t CAN_GetIntCode(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    return (pFM3_CAN_REG[Ch]->INTR);
}

/*!
 ******************************************************************************
 ** \brief Get Rx pin monitor bit
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \retval 0: indicate the CAN bus is in the dominant state
 **         1: indicate the CAN bus is in the recessive state
 ** 
 ******************************************************************************
 */
uint8_t CAN_GetBusState(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    return pFM3_CAN_REG[Ch]->TESTR_f.RX;
}

/*!
 ******************************************************************************
 ** \brief set Tx pin control
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **     
 ** \param CAN_TXPinStateT State
 ** \arg   enum of PIN_NORMAL~PIN_OUTPUT_RECESSIVE
 **  
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_SetTXPinState(uint8_t Ch, CAN_TXPinStateT State)
{
    uint16_t tTESTR;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tTESTR = pFM3_CAN_REG[Ch]->TESTR;
    tTESTR = (tTESTR & 0x9F) | (State << 5);
    pFM3_CAN_REG[Ch]->TESTR = tTESTR;
}

/*!
 ******************************************************************************
 ** \brief config test mode
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **     
 ** \param CAN_TestModeT Mode
 ** \arg   enum of SilentMode~SeftTestMode_With_BasicMode
 **  
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ConfigTestMode(uint8_t Ch, CAN_TestModeT Mode)
{
    uint16_t tTESTR;
    
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    switch (Mode)
    {
    case SilentMode:
      tTESTR = pFM3_CAN_REG[Ch]->TESTR & 0xE0;
      tTESTR |= (1UL << 3); //Silent = 1
      break;
    case LoopBackMode:
      tTESTR = pFM3_CAN_REG[Ch]->TESTR & 0xE0;
      tTESTR |= (1UL << 4); //LBack =  1
      break;
      
    case SeftTestMode:
      tTESTR = pFM3_CAN_REG[Ch]->TESTR & 0xE0;
      tTESTR |= 0x18; //Silent = 1; LBack = 1;
      break;
      
    case SilentMode_With_BasicMode:
      tTESTR = pFM3_CAN_REG[Ch]->TESTR & 0xE0;
      tTESTR |= 0x0C; //Silent = 1; Basic = 1;
      break;
      
    case LoopBackMode_With_BasicMode:
      tTESTR = pFM3_CAN_REG[Ch]->TESTR & 0xE0;
      tTESTR |= 0x14; //LBack = 1; Basic = 1;
      break;
        
    case SeftTestMode_With_BasicMode:
      tTESTR = pFM3_CAN_REG[Ch]->TESTR & 0xE0;
      tTESTR |= 0x1C; //LBack = 1; Silent = 1; Basic = 1;
      break;
    default: break;
    }
    
    pFM3_CAN_REG[Ch]->TESTR = tTESTR;
}

/*!
 ******************************************************************************
 ** \brief Get message transmitting busy flag
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \retval 0: Indicates that data transfer is not performed between the IF1 and 
 **            message RAM
 **         1: indicates thate data transfer is being performed between the IF1
 **            and message RAM
 **
 ******************************************************************************
 */
FlagStatusT CAN_GetMsgTrsfBusyFlag(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    return (FlagStatusT)pFM3_CAN_REG[Ch]->IF1CREQ_f.BUSY;
}

/*!
 ******************************************************************************
 ** \brief enable test mode transfer
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \retval none
 **
 ******************************************************************************
 */
void CAN_EnableTestModeTrsf(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->IF1CREQ_f.BUSY = 1;
}

/*!
 ******************************************************************************
 ** \brief Set message number Ram for writting
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** 
 ** \param MsgBufNum
 ** \arg   0x01~0x20: specifies a message number to perform processing
 **
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_WriteMsgRAM(uint8_t Ch, uint8_t MsgBufNum)
{
    uint16_t tIFxCREQ;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tIFxCREQ =pFM3_CAN_REG[Ch]->IF1CREQ;
    tIFxCREQ = (tIFxCREQ & 0x8000) | MsgBufNum;
    pFM3_CAN_REG[Ch]->IF1CREQ = tIFxCREQ;
}

/*!
 ******************************************************************************
 ** \brief Get message receiving busy flag
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \retval 0: Indicates that data transfer is not performed between the IF2 and 
 **            message RAM
 **         1: indicates thate data transfer is being performed between the IF2
 **            and message RAM
 **
 ******************************************************************************
 */
FlagStatusT CAN_GetMsgRcvBusyFlag(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    return (FlagStatusT)pFM3_CAN_REG[Ch]->IF2CREQ_f.BUSY;
}

/*!
 ******************************************************************************
 ** \brief enable test mode receiving
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \retval none
 **
 ******************************************************************************
 */
void CAN_EnableTestModeRcv(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    pFM3_CAN_REG[Ch]->IF2CREQ_f.BUSY = 1;
}



/*!
 ******************************************************************************
 ** \brief Set message number Ram for reading
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** 
 ** \param MsgBufNum
 ** \arg   0x01~0x20: specifies a message number to perform processing
 **
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ReadMsgRAM(uint8_t Ch, uint8_t MsgBufNum)
{
    uint16_t tIFxCREQ;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tIFxCREQ =pFM3_CAN_REG[Ch]->IF2CREQ;
    tIFxCREQ = (tIFxCREQ & 0x8000) | MsgBufNum;
    pFM3_CAN_REG[Ch]->IF2CREQ = tIFxCREQ;
}

/*!
 ******************************************************************************
 ** \brief configure Command Mask Regsiter of write message
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param CAN_WriteMsgMaskT *pMask
 ** \arg  struct of write message command
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ConfigWriteMsgMask(uint8_t Ch,  CAN_WriteMsgMaskT *pMask)
{
    uint16_t tIFxCMSK = 0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tIFxCMSK |= (1UL << 7);//(WD/RD = 1) data is writting to RAM 
    tIFxCMSK |= (pMask->Mask<<6) + (pMask->Arb<<5) + (pMask->Control<<4);
    tIFxCMSK |= (pMask->CIP<<3) + (pMask->TxRqst<<2);
    tIFxCMSK |= (pMask->DataA<<1) + (pMask->DataB<<0);
      
    pFM3_CAN_REG[Ch]->IF1CMSK = tIFxCMSK;
}

/*!
 ******************************************************************************
 ** \brief configure Command Mask Regsiter of read message
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param CAN_ReadMsgMaskT *pMask
 ** \arg  struct of reading message command
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ConfigReadMsgMask(uint8_t Ch,  CAN_ReadMsgMaskT *pMask)
{
    uint16_t tIFxCMSK = 0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tIFxCMSK &= ~(1UL << 7);//(WD/RD = 0) data is writting to RAM 
    tIFxCMSK |= (pMask->Mask<<6) + (pMask->Arb<<5) + (pMask->Control<<4);
    tIFxCMSK |= (pMask->CIP<<3) + (pMask->NewDat<<2);
    tIFxCMSK |= (pMask->DataA<<1) + (pMask->DataB<<0);
      
    pFM3_CAN_REG[Ch]->IF2CMSK = tIFxCMSK;
}

/*!
 ******************************************************************************
 ** \brief write mask information
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  CAN_MaskInfoT *pInfo
 ** \arg  struct of mask information
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_WriteMaskInfo(uint8_t Ch, CAN_MaskInfoT *pInfo)
{
    uint32_t tIFxMSK=0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
   
    tIFxMSK |= (pInfo->MXtd << 31) + (pInfo->MDIR << 30) ;
    if(pInfo->MXtd == 1)  //ID 29bit
    {
        tIFxMSK |= pInfo->MskID;
    }
    else
    {
        tIFxMSK |= pInfo->MskID << 18; //ID 11bit
    }
    
    pFM3_CAN_REG[Ch]->IF1MSK = tIFxMSK;
}

/*!
 ******************************************************************************
 ** \brief read  Mask information
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  CAN_MaskInfoT *pInfo
 ** \arg  struct of mask information
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ReadMaskInfo(uint8_t Ch, CAN_MaskInfoT *pInfo)
{
    uint32_t tIFxMSK=0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
   
    tIFxMSK = pFM3_CAN_REG[Ch]->IF2MSK;
    pInfo->MXtd = (tIFxMSK >> 31) & 0x01;
    pInfo->MDIR = (tIFxMSK >> 30) & 0x01;
    if(pInfo->MXtd == 1)  //ID 29bit
    {
        pInfo->MskID =  tIFxMSK & 0x1FFFFFFF; //ID 29bit
    }
    else
    {
        pInfo->MskID = (tIFxMSK >> 18) & 0x7FF;//ID 11bit
    }
}

/*!
 ******************************************************************************
 ** \brief write arbitration information
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  CAN_AbrInfoT *pInfo
 ** \arg  struct of arbitration information
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_WriteAbrInfo(uint8_t Ch, CAN_AbrInfoT *pInfo)
{
    uint32_t tIFxARB=0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tIFxARB |= (pInfo->MsgEn << 31) + (pInfo->Xtd << 30);
    tIFxARB |= (pInfo->DIR << 29);
    if(pInfo->Xtd == 1)
    {
        tIFxARB |=  pInfo->ID; //ID 29bit
    }
    else
    {
        tIFxARB |=  pInfo->ID << 18;//ID 11bit
    }
    pFM3_CAN_REG[Ch]->IF1ARB = tIFxARB;
}

/*!
 ******************************************************************************
 ** \brief read arbitration information
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  CAN_AbrInfoT *pInfo
 ** \arg  struct of arbitration information
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ReadAbrInfo(uint8_t Ch, CAN_AbrInfoT *pInfo)
{
    uint32_t tIFxARB=0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tIFxARB = pFM3_CAN_REG[Ch]->IF2ARB;
    pInfo->MsgEn = (tIFxARB >>  31) & 0x01;
    pInfo->Xtd = (tIFxARB >>  30) & 0x01;
    pInfo->DIR = (tIFxARB >>  29) & 0x01;

    if(pInfo->Xtd == 1)
    {
        pInfo->ID =  tIFxARB & 0x1FFFFFFF; //ID 29bit
    }
    else
    {
        pInfo->ID = (tIFxARB >> 18) & 0x7FF;//ID 11bit
    }
}

/*!
 ******************************************************************************
 ** \brief write configure Message Control Register  
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  CAN_CtrlInfoT *pInfo
 ** \arg  struct of Message Control register
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_WriteCtrlInfo(uint8_t Ch, CAN_CtrlInfoT *pInfo)
{
    uint16_t tIFxMCTR=0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    tIFxMCTR |= (pInfo->NewDat << 15) + (pInfo->MsgLst << 14);
    tIFxMCTR |= (pInfo->IntPnd << 13) + (pInfo->UMask << 12);
    tIFxMCTR |= (pInfo->TxIE << 11) + (pInfo->RxIE << 10);
    tIFxMCTR |= (pInfo->RmtEn << 9) + (pInfo->TxRqst << 8);
    tIFxMCTR |= (pInfo->EoB << 7) + (pInfo->DLC << 0);

    pFM3_CAN_REG[Ch]->IF1MCTR = tIFxMCTR;
}

/*!
 ******************************************************************************
 ** \brief read configure Message Control Register
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  CAN_CtrlInfoT *pInfo
 ** \arg  struct of Message Control register
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ReadCtrlInfo(uint8_t Ch, CAN_CtrlInfoT *pInfo)
{
    uint16_t tIFxMCTR=0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    tIFxMCTR = pFM3_CAN_REG[Ch]->IF2MCTR;
    
    pInfo->NewDat = (tIFxMCTR >> 15) & 0x01;
    pInfo->MsgLst = (tIFxMCTR >> 14) & 0x01;
    pInfo->IntPnd = (tIFxMCTR >> 13) & 0x01;
    pInfo->UMask = (tIFxMCTR >> 12) & 0x01;
    pInfo->TxIE = (tIFxMCTR >> 11) & 0x01;
    pInfo->RxIE = (tIFxMCTR >> 10) & 0x01;
    pInfo->RmtEn = (tIFxMCTR >> 9) & 0x01;
    pInfo->TxRqst = (tIFxMCTR >> 8) & 0x01;
    pInfo->EoB = (tIFxMCTR >> 7) & 0x01;
    pInfo->DLC = tIFxMCTR  & 0x0F;
}

/*!
 ******************************************************************************
 ** \brief write data 
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  * pData
 ** \arg    point of data buffer
 ** 
 ** \param  Size
 ** \arg    length of writting
 **         Value 1~8
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_WriteData(uint8_t Ch, uint8_t* pData, uint8_t Size)
{
    uint32_t DatBuf=0;
    uint32_t DatBuf1=0;
    uint32_t DatBuf2=0;
    uint8_t count=0;
    uint8_t Array_Index = 0;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    ASSERT(Size <= 8);
    
    while(Size --)
    {
        DatBuf = DatBuf + (pData[count]<<(8*Array_Index));
        if(count <= 3)
        {
            DatBuf1 = DatBuf;  //
        }
        else
        {
            DatBuf2 = DatBuf;  //
        }
        
        count ++;
        Array_Index++;
        if(count == 4)
        {
            DatBuf = 0;   
            Array_Index = 0;
        }
    }
    
    pFM3_CAN_REG[Ch]->IF1DTA_L = DatBuf1;
    pFM3_CAN_REG[Ch]->IF1DTB_L = DatBuf2;
}

/*!
 ******************************************************************************
 ** \brief read data 
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  * pData
 ** \arg    point of data buffer
 ** 
 ** \param  Size
 ** \arg    length of reading 
 **         Value 1~8
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ReadData(uint8_t Ch, uint8_t* pData, uint8_t Size)
{
    uint32_t DatBuf=0;
    uint32_t DatBuf1=0;
    uint8_t count=0;
    uint8_t count1=0;
    
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    ASSERT(Size <= 8);
    
    DatBuf = pFM3_CAN_REG[Ch]->IF2DTA_L;
    DatBuf1 = pFM3_CAN_REG[Ch]->IF2DTB_L;
    while(Size --)
    {
        if(count == 4)
        {
            DatBuf = DatBuf1;     
            count1 = 0;
        }
        
        pData[count]= (DatBuf >> (8*count1)) & 0x000000FF;
        
        count ++;
        count1 ++;
    }
}

/*!
 ******************************************************************************
 ** \brief Get tansmit request flag 
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  MsgBufNum
 ** \arg    TxRqst1~TxRqst32
 ** 
 ** \retval 0: no transfer request
 **         1: detect transfer  request
 **
 ******************************************************************************
 */
FlagStatusT CAN_GetTxRqstFlag(uint8_t Ch, uint8_t MsgBufNum)
{
    uint8_t RetVal;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    if((MsgBufNum - 1) >= 0)
    {
        RetVal = (pFM3_CAN_REG[Ch]->TREQR >> (MsgBufNum-1)) & 0x01;
    }
    
    return (FlagStatusT)RetVal;
}

/*!
 ******************************************************************************
 ** \brief Get data update flag 
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  MsgBufNum
 ** \arg    NewDat1~NewDat32
 ** 
** \retval 0: 
 **
 ******************************************************************************
 */
FlagStatusT CAN_GetNewDataFlag(uint8_t Ch, uint8_t MsgBufNum)
{
    uint8_t RetVal;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    if((MsgBufNum - 1) >= 0)
    {
        RetVal = (pFM3_CAN_REG[Ch]->NEWDT >> (MsgBufNum-1)) & 0x01;
    }
    
    return (FlagStatusT)RetVal;
}

/*!
 ******************************************************************************
 ** \brief Get interrupt pending flag
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  MsgBufNum
 ** \arg    IntPnd1~IntPnd32
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
FlagStatusT CAN_GetIntPendingFlag(uint8_t Ch, uint8_t MsgBufNum)
{
    uint8_t RetVal;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    if((MsgBufNum - 1) >= 0)
    {
        RetVal = (pFM3_CAN_REG[Ch]->INTPND >> (MsgBufNum-1)) & 0x01;
    }
    return (FlagStatusT)RetVal;
}

/*!
 ******************************************************************************
 ** \brief Get Message valid flag
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 ** 
 ** \param  MsgBufNum
 ** \arg    MsgVal1~MsgVal32
 ** 
 ** \retval None
 **
 ******************************************************************************
 */
FlagStatusT CAN_GetMsgValidFlag(uint8_t Ch, uint8_t MsgBufNum)
{
    uint8_t RetVal;
    /* Check Cfg parameter */
    ASSERT(Ch < CAN_CH_MAX);
    
    RetVal = (pFM3_CAN_REG[Ch]->MSGVAL >> (MsgBufNum-1)) & 0x01;
    
    return (FlagStatusT)RetVal;
}

/*!
 ******************************************************************************
 ** \brief Config the CAN Transmit ISR call back
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **         
 ** \param   CAN_IntHandlerArray* IntHandlerArray
 ** \arg   structure of CAN IRQ  callback function
 **
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_ConfigIntCallback(uint8_t Ch, CAN_IntHandlerArray* IntHandlerArray)
{
    CANIntArray[Ch].TxIntHandlerCallback = IntHandlerArray->TxIntHandlerCallback;
    CANIntArray[Ch].RxIntHandlerCallback = IntHandlerArray->RxIntHandlerCallback;
    CANIntArray[Ch].ErrIntHandlerCallback = IntHandlerArray->ErrIntHandlerCallback;
    CANIntArray[Ch].StatusIntHandlerCallback = IntHandlerArray->StatusIntHandlerCallback;
}

/*!
 ******************************************************************************
 ** \brief CAN interrupt handler
 **
 ** \param Ch Requested channel
 ** \arg   CAN_Ch0,CAN_Ch1
 **
 ** \retval None
 **
 ******************************************************************************
 */
void CAN_IntHandler(uint8_t Ch)
{
    uint16_t IntStatus;
    uint8_t LastErrCode;
    
    IntStatus = CAN_GetIntCode(Ch);
    if(IntStatus == 0x8000)/*status interrupt*/
    {
    	if(CAN_GetStatus(Ch,CAN_ERROR_PASSIVE)==1) //error warning
    	{
            if(CANIntArray[Ch].ErrIntHandlerCallback != NULL)
            {
    	        CANIntArray[Ch].ErrIntHandlerCallback(CAN_BUSOFF_EWARNING);
            }
    	}
        
    	if(CAN_GetStatus(Ch,CAN_BUS_OFF)==1) //bus off
    	{
            if(CANIntArray[Ch].ErrIntHandlerCallback != NULL)
            {
    	        CANIntArray[Ch].ErrIntHandlerCallback(CAN_BUSOFF_ERR);
            }
    	}
        
        LastErrCode = CAN_GetLastErrCode(Ch);
    	if(LastErrCode != 0x00) //last error  code
    	{
            if(CANIntArray[Ch].StatusIntHandlerCallback != NULL)
            {
    		CANIntArray[Ch].StatusIntHandlerCallback((CAN_LastErrCodeT)LastErrCode);
            }
    	}
    }
    else if( (IntStatus >= 0x0001)&&(IntStatus <= 0x0020)) /*message interrupt*/
    {
        if(CAN_GetStatus(Ch,CAN_TX_OK) == 1) //Tx OK
        {
            if(CANIntArray[Ch].TxIntHandlerCallback != NULL)
            {
                CANIntArray[Ch].TxIntHandlerCallback(CAN_TRSF_FINISH,IntStatus);
            }
        }
        if(CAN_GetStatus(Ch,CAN_RX_OK) == 1) //Rx OK
        {
            if(CANIntArray[Ch].RxIntHandlerCallback != NULL)
            {
                CANIntArray[Ch].RxIntHandlerCallback(CAN_RCV_FINISH,IntStatus);
            }
        }
    }
    else
    {
    	;
    }
}

#endif

/*****************************************************************************/
/* END OF FILE */
