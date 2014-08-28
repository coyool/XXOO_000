/*!
 ******************************************************************************
 **
 ** \file lin_fm3.c
 **
 ** \brief LIN drivers
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
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "lin_fm3.h"

#if (PERIPHERAL_ENABLE_MFS_LIN == ON) && (PERIPHERAL_AVAILABLE_MFS == ON)
/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
#if (MFS_MAX_CH == 4)
    static FM3_MFS_LIN_TypeDef* pMFS_LIN_REG[4] = 
    {
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS0_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS1_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS2_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS3_LIN),
    };
#elif (MFS_MAX_CH == 6)
    static FM3_MFS_LIN_TypeDef* pMFS_LIN_REG[6] = 
    {
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS0_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS1_LIN),
        NULL,
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS3_LIN),
        NULL,
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS5_LIN),
    };
#elif (MFS_MAX_CH == 8)
    static FM3_MFS_LIN_TypeDef* pMFS_LIN_REG[8] = 
    {
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS0_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS1_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS2_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS3_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS4_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS5_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS6_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS7_LIN),
    };
#elif (MFS_MAX_CH == 12)
    static FM3_MFS_LIN_TypeDef* pMFS_LIN_REG[12] = 
    {
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS0_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS1_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS2_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS3_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS4_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS5_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS6_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS7_LIN),
        NULL,
        NULL,
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS10_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS11_LIN),        
    };
#elif (MFS_MAX_CH == 14)
    static FM3_MFS_LIN_TypeDef* pMFS_LIN_REG[14] = 
    {
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS0_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS1_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS2_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS3_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS4_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS5_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS6_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS7_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS8_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS9_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS10_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS11_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS12_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS13_LIN),
    };
#elif (MFS_MAX_CH == 16)
    static FM3_MFS_LIN_TypeDef* pMFS_LIN_REG[16] = 
    {
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS0_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS1_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS2_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS3_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS4_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS5_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS6_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS7_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS8_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS9_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS10_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS11_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS12_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS13_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS14_LIN),
        (FM3_MFS_LIN_TypeDef*)(FM3_MFS15_LIN),
    };
#endif
/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/
extern MFS_CallbackT g_pMFSIRQCallback[MFS_MAX_CH*3];
/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Config the LIN baud rate
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param baudrate
 ** \param unsigned int
 ** \arg   Baud rate value
 **         
 ** \retval None
 **
 ******************************************************************************
 */
uint16_t MFS_LINConfigBaudrate(uint8_t Ch, uint32_t baudrate)
{
    uint16_t tBGR;
    uint32_t tSysFrePCLK2;
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    tSysFrePCLK2 = SystemCoreClock/(1<<((FM3_CRG->APBC2_PSR&0x03)));
    tBGR = (uint16_t)(((tSysFrePCLK2 + (baudrate / 2)) / (uint32_t)baudrate) - 1);
    pMFS_LIN_REG[Ch]->BGR = tBGR;
    return tBGR;
}
/*!
 ******************************************************************************
 ** \brief Config the LIN Mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINSetLINMode(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    tSMR = pMFS_LIN_REG[Ch]->SMR;
    tSMR = tSMR & 0x1F;//Clear MD0-2
    tSMR = tSMR | MFS_MODE_LIN;//Set MD0-2 as 010
    pMFS_LIN_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the LIN stop bit length
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Mode initialization data
 ** \arg   pointer of Mode info
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINConfigST(uint8_t Ch, MFS_LINModeConfigT *pModeConfig)
{
    uint8_t tSMR = 0;
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    ASSERT((pModeConfig->Stopbits >= LIN_STOPBITS_1) && (pModeConfig->Stopbits <= LIN_STOPBITS_4));

    tSMR = pMFS_LIN_REG[Ch]->SMR;
    tESCR = pMFS_LIN_REG[Ch]->ESCR;

    tSMR = tSMR & (~(LIN_SMR_SBL));
    tESCR = tESCR & (~(LIN_ESCR_ESBL));
    /* Stop bit */
    switch(pModeConfig->Stopbits)
    {
        case LIN_STOPBITS_1:
            break;
        case LIN_STOPBITS_2:
            tSMR = tSMR | LIN_SMR_SBL;
            break;
        case LIN_STOPBITS_3:
            tESCR = tESCR | LIN_ESCR_ESBL;
            break;
        case LIN_STOPBITS_4:
            tSMR = tSMR | LIN_SMR_SBL;
            tESCR = tESCR | LIN_ESCR_ESBL;
            break;
        default:
            break;
    }
    pMFS_LIN_REG[Ch]->SMR = tSMR;
    pMFS_LIN_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the LIN Break Field Length
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Mode initialization data
 ** \arg   pointer of Mode info
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINConfigBFL(uint8_t Ch, MFS_LINModeConfigT *pModeConfig)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    ASSERT((pModeConfig->BreakFieldbits >= LIN_BREAKFIELDBITS_13) && (pModeConfig->BreakFieldbits <= LIN_BREAKFIELDBITS_16));

    tESCR = pMFS_LIN_REG[Ch]->ESCR;
    
    tESCR = tESCR & (~(LIN_ESCR_BREAKFIELDBITLEN_16));
    /* Break Field Length bits */
    switch(pModeConfig->BreakFieldbits)
    {
        case LIN_BREAKFIELDBITS_13:
            tESCR = tESCR | LIN_ESCR_BREAKFIELDBITLEN_13;
            break;
        case LIN_BREAKFIELDBITS_14:
            tESCR = tESCR | LIN_ESCR_BREAKFIELDBITLEN_14;
            break;
        case LIN_BREAKFIELDBITS_15:
            tESCR = tESCR | LIN_ESCR_BREAKFIELDBITLEN_15;
            break;
        case LIN_BREAKFIELDBITS_16:
            tESCR = tESCR | LIN_ESCR_BREAKFIELDBITLEN_16;
            break;
        default:
            break;
    }
    pMFS_LIN_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the LIN Break Delimiter Length
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Mode initialization data
 ** \arg   pointer of Mode info
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINConfigBDL(uint8_t Ch, MFS_LINModeConfigT *pModeConfig)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    ASSERT((pModeConfig->BreakDelimiter >= LIN_BREAKDELIMITER_1) && (pModeConfig->BreakDelimiter <= LIN_BREAKDELIMITER_4));

    tESCR = pMFS_LIN_REG[Ch]->ESCR;
    
    tESCR = tESCR & (~(LIN_ESCR_BREAKDELIMITERLENGTH_4));
    /* Break Delimiter Length bits */
    switch(pModeConfig->BreakDelimiter)
    {
        case LIN_BREAKDELIMITER_1:
            tESCR = tESCR | LIN_ESCR_BREAKDELIMITERLENGTH_1;
            break;
        case LIN_BREAKDELIMITER_2:
            tESCR = tESCR | LIN_ESCR_BREAKDELIMITERLENGTH_2;
            break;
        case LIN_BREAKDELIMITER_3:
            tESCR = tESCR | LIN_ESCR_BREAKDELIMITERLENGTH_3;
            break;
        case LIN_BREAKDELIMITER_4:
            tESCR = tESCR | LIN_ESCR_BREAKDELIMITERLENGTH_4;
            break;
        default:
            break;
    }
    pMFS_LIN_REG[Ch]->ESCR = tESCR;
    return;
}

/*!
 ******************************************************************************
 ** \brief Config the LIN baud rate
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Mode selection
 ** \arg   MASTER, SLAVE         
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINSetMode(uint8_t Ch, uint8_t MS_Mode)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    if(MS_Mode == LIN_MASTER)
    {
        pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR & (~LIN_SCR_MS);
    }
    else
    {
        pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR | LIN_SCR_MS;
    }
}

/*!
 ******************************************************************************
 ** \brief Send LIN break field
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINSetBreakField(uint8_t Ch)
{
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR | LIN_SCR_LBR;
}

/*!
 ******************************************************************************
 ** \brief Config the LIN ISR call back
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param  Mode initialization data
 ** \param MFS_LINISRCallbackT
 ** \arg   Mode info
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINConfigISRCallback(uint8_t Ch, MFS_LINISRCallbackT *pCallback)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    ASSERT((pCallback->pISRRXCallback != NULL) &&
        (pCallback->pISRTXCallback != NULL));
    /* Init the callback */
    g_pMFSIRQCallback[Ch*3] = pCallback->pISRRXCallback;
    g_pMFSIRQCallback[Ch*3+1] = pCallback->pISRTXCallback;
    g_pMFSIRQCallback[Ch*3+2] = pCallback->pISRStatusCallback;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the LIN FIFO
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param FIFO FIFOselection information
 ** \arg   MFS_FIFO1TX_FIFO2RX/MFS_FIFO2TX_FIFO1RX
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINConfigFSEL(uint8_t Ch, MFS_FIFOSelectionT FIFOSel)
{
    uint8_t tFCR1 = 0;
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    ASSERT((FIFOSel == MFS_FIFO1TX_FIFO2RX) || (FIFOSel == MFS_FIFO2TX_FIFO1RX));
    
    if(FIFOSel == MFS_FIFO1TX_FIFO2RX)
    {
    }
    else
    {
        tFCR1 = tFCR1 | LIN_FCR1_FSEL;
    }
    pMFS_LIN_REG[Ch]->FCR1 = tFCR1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Config the LIN FIFO FByte1
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param FByte1
 ** \arg   1 - 128
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINConfigFByte1(uint8_t Ch, uint8_t FByte1)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    
    pMFS_LIN_REG[Ch]->FBYTE1 = FByte1;
}

/*!
 ******************************************************************************
 ** \brief Config the LIN FIFO FByte2
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param FByte2
 ** \arg   1 - 128
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINConfigFByte2(uint8_t Ch, uint8_t FByte2)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    
    pMFS_LIN_REG[Ch]->FBYTE2 = FByte2;
}
/*!
 ******************************************************************************
 ** \brief FIFO2 reset
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval none
 **
 ******************************************************************************
 */
void MFS_LINResetFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    
    pMFS_LIN_REG[Ch]->FCR0 = pMFS_LIN_REG[Ch]->FCR0 | LIN_FCR0_FCL2;
    
    return;
}
/*!
 ******************************************************************************
 ** \brief FIFO1 reset
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval none
 **
 ******************************************************************************
 */
void MFS_LINResetFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    
    pMFS_LIN_REG[Ch]->FCR0 = pMFS_LIN_REG[Ch]->FCR0 | LIN_FCR0_FCL1;
    
    return;
}
/*!
 ******************************************************************************
 ** \brief Enable FIFO2 operation
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINEnableFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    pMFS_LIN_REG[Ch]->FCR0 = pMFS_LIN_REG[Ch]->FCR0 | LIN_FCR0_FE2;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable FIFO2 operation
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINDisableFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    pMFS_LIN_REG[Ch]->FCR0 = pMFS_LIN_REG[Ch]->FCR0 & ~(LIN_FCR0_FE2);
    return;
}


/*!
 ******************************************************************************
 ** \brief Disable FIFO1 operation
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINDisableFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    pMFS_LIN_REG[Ch]->FCR0 = pMFS_LIN_REG[Ch]->FCR0 & ~(LIN_FCR0_FE1);
    return;
}
/*!
 ******************************************************************************
 ** \brief Enable FIFO1 operation
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINEnableFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    pMFS_LIN_REG[Ch]->FCR0 = pMFS_LIN_REG[Ch]->FCR0 | LIN_FCR0_FE1;
    return;
}
/*!
 ******************************************************************************
 ** \brief Reset the FDRQ
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINResetFDRQ(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->FCR1 = pMFS_LIN_REG[Ch]->FCR1 &
        (~LIN_FCR1_FDRQ);
    return;
}
/*!
 ******************************************************************************
 ** \brief Transmit one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Data
 ** \arg   5 - 9 bits data
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINTXOneData(uint8_t Ch, uint8_t Data)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->TDR = Data;
    return;
}

/*!
 ******************************************************************************
 ** \brief Receive one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval Data
 **
 ******************************************************************************
 */
uint8_t MFS_LINRXOneData(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    return (pMFS_LIN_REG[Ch]->RDR & 0x00FF);
}

/*!
 ******************************************************************************
 ** \brief Enable tranmit function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINEnableTX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR | LIN_SCR_TXE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable receive function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINEnableRX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR | LIN_SCR_RXE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable FIFO
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \param nfifo (FE1 or FE2)
 ** \arg   1 - 2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINFIFOEnable(uint8_t Ch, uint8_t nfifo)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    ASSERT((nfifo == LIN_FCR0_FE1) || (nfifo == LIN_FCR0_FE2));
    pMFS_LIN_REG[Ch]->FCR0 = pMFS_LIN_REG[Ch]->FCR0 | nfifo;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable transmit
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINDisableTX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR & (~LIN_SCR_TXE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable receive function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINDisableRX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR & (~LIN_SCR_RXE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable FIFO
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \param nfifo (FE1 or FE2)
 ** \arg   1 - 2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINFIFODisable(uint8_t Ch, uint8_t nfifo)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    ASSERT((nfifo == LIN_FCR0_FE1) || (nfifo == LIN_FCR0_FE2));
    pMFS_LIN_REG[Ch]->FCR0 = pMFS_LIN_REG[Ch]->FCR0 & (~nfifo);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX Bus idle interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINEnableTXBusIdleInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR | LIN_SCR_TBIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINEnableTXInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR | LIN_SCR_TIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable LIN break field detect interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINEnableLBIEInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->ESCR = pMFS_LIN_REG[Ch]->ESCR | LIN_ESCR_LBIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX FIFO Empty interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINEnableTXFIFOEmptyInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    pMFS_LIN_REG[Ch]->FCR1 = pMFS_LIN_REG[Ch]->FCR1 | LIN_FCR1_FTIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable RX interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINEnableRXInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR | LIN_SCR_RIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX Bus idle interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINDisableTXBusIdleInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR & (~LIN_SCR_TBIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINDisableTXInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR & (~LIN_SCR_TIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable the Wake-up function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINDisableWakeUp(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SMR = pMFS_LIN_REG[Ch]->SMR & (~LIN_SMR_WUCR);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable the Wake-up function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINEnableWakeUp(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SMR = pMFS_LIN_REG[Ch]->SMR | LIN_SMR_WUCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Enable Serial Ouput
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINEnableSOE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SMR = pMFS_LIN_REG[Ch]->SMR  | LIN_SMR_SOE;
    return;
}
/*!
 ******************************************************************************
 ** \brief Disable Serial Output
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_LINDisableSOE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SMR = pMFS_LIN_REG[Ch]->SMR & (~LIN_SMR_SOE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable LIN break field detect interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINDisableLBIEInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->ESCR = pMFS_LIN_REG[Ch]->ESCR & (~LIN_ESCR_LBIE);
    return;
}


/*!
 ******************************************************************************
 ** \brief Disable TX FIFO Empty interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINDisableTXFIFOEmptyInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= LIN_Ch_MAX));
    pMFS_LIN_REG[Ch]->FCR1 = pMFS_LIN_REG[Ch]->FCR1 &
        (~LIN_FCR1_FTIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable RX interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_LINDisableRXInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR & (~LIN_SCR_RIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get Overrun Error flag
 **
 ** \param Ch Requested channel
 ** \param unsigned char
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return Overrun error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_LINGetOEStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);

    return ((FlagStatusT)((pMFS_LIN_REG[Ch]->SSR &
        LIN_SSR_ORE) == LIN_SSR_ORE));
}

/*!
 ******************************************************************************
 ** \brief Get LIN Break field detection flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return LIN Break field detection flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_LINGetLBDStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);

    return ((FlagStatusT)((pMFS_LIN_REG[Ch]->SSR  &
        LIN_SSR_LBD) == LIN_SSR_LBD));
}

/*!
 ******************************************************************************
 ** \brief Get Frame Error flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return Frame error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_LINGetFEStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);

    return ((FlagStatusT)((pMFS_LIN_REG[Ch]->SSR &
        LIN_SSR_FRE) == LIN_SSR_FRE));
}

/*!
 ******************************************************************************
 ** \brief Get receive register full flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return RDR Full flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_LINGetRXRegFullStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);

    return ((FlagStatusT)((pMFS_LIN_REG[Ch]->SSR &
        LIN_SSR_RDRF) == LIN_SSR_RDRF));
}

/*!
 ******************************************************************************
 ** \brief Get transmit register empty flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return TDR Empty flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_LINGetTXRegEmptyStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);

    return ((FlagStatusT)((pMFS_LIN_REG[Ch]->SSR &
        LIN_SSR_TDRE) == LIN_SSR_TDRE));
}

/*!
 ******************************************************************************
 ** \brief Get transmit Bus idle flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return TX Bus idle flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_LINGetTXBusIdleStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);

    return ((FlagStatusT)((pMFS_LIN_REG[Ch]->SSR &
        LIN_SSR_TBI) == LIN_SSR_TBI));
}
/*!
 ******************************************************************************
 ** \brief Get the FDRQ to request for the transmit FIFO data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval TX FIFO data request flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_LINGetFDRQStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->FCR1 = pMFS_LIN_REG[Ch]->FCR1 | LIN_FCR1_FDRQ;

    return ((FlagStatusT)((pMFS_LIN_REG[Ch]->FCR &
        LIN_FCR1_FDRQ) == LIN_FCR1_FDRQ));
}

/*!
 ******************************************************************************
 ** \brief Get FBYTE1
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return FBYTE1
 ** \retval 0 - 127
 **
 ******************************************************************************
 */
uint8_t MFS_LINGetFBYTE1Status(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);

    return (pMFS_LIN_REG[Ch]->FBYTE1);
}
/*!
 ******************************************************************************
 ** \brief Get FBYTE2
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return FBYTE2
 ** \retval 0 - 127
 **
 ******************************************************************************
 */
uint8_t MFS_LINGetFBYTE2Status(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);

    return (pMFS_LIN_REG[Ch]->FBYTE2);
}

/*!
 ******************************************************************************
 ** \brief Clear the error flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval none
 **
 ******************************************************************************
 */
void MFS_LINClrError(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SSR = pMFS_LIN_REG[Ch]->SSR | LIN_SSR_REC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Programmable clear
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval none
 **
 ******************************************************************************
 */
void MFS_LINClrProgram(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SCR = pMFS_LIN_REG[Ch]->SCR | LIN_SCR_UPCL;
    return;
}


/*!
 ******************************************************************************
 ** \brief Clear LIN Break field detection flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return none
 **
 ******************************************************************************
 */
void MFS_LINClrLBD(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= LIN_Ch_MAX);
    pMFS_LIN_REG[Ch]->SSR &= (~LIN_SSR_LBD);
}

#endif
/*****************************************************************************/

/* END OF FILE */
