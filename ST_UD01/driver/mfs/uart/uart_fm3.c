/*!
 ******************************************************************************
 **
 ** \file uart_fm3.c
 **
 ** \brief UART drivers
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
#include "uart_fm3.h"
/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/
#if (PERIPHERAL_ENABLE_MFS_UART == ON) 
/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
#if (MFS_MAX_CH == 4)
    static FM3_MFS_UART_TypeDef* pMFS_UART_REG[4] = 
    {
        (FM3_MFS_UART_TypeDef*)(FM3_MFS0_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS1_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS2_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS3_UART),
    };
#elif (MFS_MAX_CH == 6)
    static FM3_MFS_UART_TypeDef* pMFS_UART_REG[6] = 
    {
        (FM3_MFS_UART_TypeDef*)(FM3_MFS0_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS1_UART),
        NULL,
        (FM3_MFS_UART_TypeDef*)(FM3_MFS3_UART),
        NULL,
        (FM3_MFS_UART_TypeDef*)(FM3_MFS5_UART),
    };
#elif (MFS_MAX_CH == 8)
    static FM3_MFS_UART_TypeDef* pMFS_UART_REG[8] = 
    {
        (FM3_MFS_UART_TypeDef*)(FM3_MFS0_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS1_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS2_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS3_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS4_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS5_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS6_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS7_UART),
    };   
#elif (MFS_MAX_CH == 12)
    static FM3_MFS_UART_TypeDef* pMFS_UART_REG[12] = 
    {
        (FM3_MFS_UART_TypeDef*)(FM3_MFS0_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS1_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS2_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS3_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS4_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS5_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS6_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS7_UART),
        NULL,
        NULL,
        (FM3_MFS_UART_TypeDef*)(FM3_MFS10_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS11_UART),
    };   
#elif (MFS_MAX_CH == 14)
    static FM3_MFS_UART_TypeDef* pMFS_UART_REG[14] = 
    {
        (FM3_MFS_UART_TypeDef*)(FM3_MFS0_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS1_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS2_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS3_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS4_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS5_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS6_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS7_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS8_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS9_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS10_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS11_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS12_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS13_UART),
    };   
#elif (MFS_MAX_CH == 16)
    static FM3_MFS_UART_TypeDef* pMFS_UART_REG[16] = 
    {
        (FM3_MFS_UART_TypeDef*)(FM3_MFS0_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS1_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS2_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS3_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS4_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS5_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS6_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS7_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS8_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS9_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS10_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS11_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS12_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS13_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS14_UART),
        (FM3_MFS_UART_TypeDef*)(FM3_MFS15_UART),
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
 ** \brief Config the UART baud rate
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param baudrate Baud rate value
 ** \arg   int
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UARTConfigBaudrate(uint8_t Ch, uint32_t baudrate)
{
    uint16_t tBGR;
    uint32_t tSysFrePCLK2;
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tSysFrePCLK2 = SystemCoreClock/(1<<((FM3_CRG->APBC2_PSR&0x03)));
    tBGR = (uint16_t)(((tSysFrePCLK2 + (baudrate / 2)) / (uint32_t)baudrate) - 1);
    pMFS_UART_REG[Ch]->BGR = tBGR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART Mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTSetUARTMode(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tSMR = pMFS_UART_REG[Ch]->SMR;
    tSMR = tSMR & 0x1F;//Clear MD0-2
    tSMR = tSMR | MFS_MODE_UART;//Set MD0-2 as 000
    pMFS_UART_REG[Ch]->SMR = tSMR;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable the SOE
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTEnableSOE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SMR = pMFS_UART_REG[Ch]->SMR | UART_SMR_SOE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable the SOE
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTDisableSOE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SMR = pMFS_UART_REG[Ch]->SMR & (~(UART_SMR_SOE));
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART stop bit length
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
void MFS_UARTConfigST(uint8_t Ch, MFS_UARTModeConfigT *pModeConfig)
{
    uint8_t tSMR = 0;
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    ASSERT((pModeConfig->Stopbits >= UART_STOPBITS_1) && (pModeConfig->Stopbits <= UART_STOPBITS_4));

    tSMR = pMFS_UART_REG[Ch]->SMR;
    tESCR = pMFS_UART_REG[Ch]->ESCR;

    tSMR = tSMR & (~(UART_SMR_SBL));
    tESCR = tESCR & (~(UART_ESCR_ESBL));
    /* Stop bit */
    switch(pModeConfig->Stopbits)
    {
        case UART_STOPBITS_1:
            break;
        case UART_STOPBITS_2:
            tSMR = tSMR | UART_SMR_SBL;
            break;
        case UART_STOPBITS_3:
            tESCR = tESCR | UART_ESCR_ESBL;
            break;
        case UART_STOPBITS_4:
            tSMR = tSMR | UART_SMR_SBL;
            tESCR = tESCR | UART_ESCR_ESBL;
            break;
        default:
            break;
    }
    pMFS_UART_REG[Ch]->SMR = tSMR;
    pMFS_UART_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART BDS as LSB
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTConfigLSB(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tSMR = pMFS_UART_REG[Ch]->SMR;
    tSMR = tSMR & (~(UART_SMR_BDS));
    pMFS_UART_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART BDS as MSB
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTConfigMSB(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tSMR = pMFS_UART_REG[Ch]->SMR;
    tSMR = tSMR | (UART_SMR_BDS);
    pMFS_UART_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART Parity as Even
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTConfigEvenParity(uint8_t Ch)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tESCR = pMFS_UART_REG[Ch]->ESCR;
    tESCR = tESCR & (~(UART_ESCR_P));
    pMFS_UART_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART Parity as Odd
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTConfigOddParity(uint8_t Ch)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tESCR = pMFS_UART_REG[Ch]->ESCR;
    tESCR = tESCR | (UART_ESCR_P);
    pMFS_UART_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Disable parity
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTDisableParity(uint8_t Ch)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tESCR = pMFS_UART_REG[Ch]->ESCR;
    tESCR = tESCR & (~(UART_ESCR_PEN));
    pMFS_UART_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Enable parity
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTEnableParity(uint8_t Ch)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tESCR = pMFS_UART_REG[Ch]->ESCR;
    tESCR = tESCR | (UART_ESCR_PEN);
    pMFS_UART_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART data bit length
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
void MFS_UARTConfigDT(uint8_t Ch, MFS_UARTModeConfigT *pModeConfig)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    ASSERT((pModeConfig->Databits >= UART_DATABITS_5) && (pModeConfig->Databits <= UART_DATABITS_9));

    tESCR = (pMFS_UART_REG[Ch]->ESCR & ~(UART_ESCR_BITLEN_5 | UART_ESCR_BITLEN_6 | UART_ESCR_BITLEN_7 | UART_ESCR_BITLEN_8 | UART_ESCR_BITLEN_9));
    /* Data bits */
    switch(pModeConfig->Databits)
    {
        case UART_DATABITS_8:
          tESCR = tESCR | UART_ESCR_BITLEN_8;
          break;
        case UART_DATABITS_5:
          tESCR = tESCR | UART_ESCR_BITLEN_5;
          break;
        case UART_DATABITS_6:
          tESCR = tESCR | UART_ESCR_BITLEN_6;
          break;
        case UART_DATABITS_7:
          tESCR = tESCR | UART_ESCR_BITLEN_7;
          break;
        case UART_DATABITS_9:
          tESCR = tESCR | UART_ESCR_BITLEN_9;
          break;
        default:
          break;
    }

    pMFS_UART_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART Not Invert
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTConfigNoInv(uint8_t Ch)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tESCR = pMFS_UART_REG[Ch]->ESCR;
    tESCR = tESCR & (~(UART_ESCR_INV));
    pMFS_UART_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART Invert
 **
 ** \param Ch Requested channel
  ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTConfigInv(uint8_t Ch)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    tESCR = pMFS_UART_REG[Ch]->ESCR;
    tESCR = tESCR | (UART_ESCR_INV);
    pMFS_UART_REG[Ch]->ESCR = tESCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART FIFO
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param pFIFOConfig FIFO config information
 ** \arg   pointer of FIFO config info
 **         
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UARTConfigFSEL(uint8_t Ch, MFS_FIFOSelectionT FIFOConfig)
{
    uint8_t tFCR1 = 0;
    
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    ASSERT((FIFOConfig == MFS_FIFO1TX_FIFO2RX) || (FIFOConfig == MFS_FIFO2TX_FIFO1RX));

    tFCR1 = pMFS_UART_REG[Ch]->FCR1;
    if(FIFOConfig == MFS_FIFO1TX_FIFO2RX)
    {
        tFCR1 = tFCR1 & (~UART_FCR1_FSEL);
    }
    else
    {
        tFCR1 = tFCR1 | UART_FCR1_FSEL;
    }

    pMFS_UART_REG[Ch]->FCR1 = tFCR1;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the UART FIFO FByte1
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param FByte1 FIFO1 Byte Register
 ** \arg   0 - 127
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTConfigFByte1(uint8_t Ch, uint8_t FByte1)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    
    pMFS_UART_REG[Ch]->FBYTE1 = FByte1;
}

/*!
 ******************************************************************************
 ** \brief Config the UART FIFO FByte2
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param FByte2 FIFO2 Byte Register
 ** \arg   0 - 127
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_UARTConfigFByte2(uint8_t Ch, uint8_t FByte2)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    
    pMFS_UART_REG[Ch]->FBYTE2 = FByte2;
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
void MFS_UARTResetFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    
    pMFS_UART_REG[Ch]->FCR0 = pMFS_UART_REG[Ch]->FCR0 | UART_FCR0_FCL2;
    
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
void MFS_UARTResetFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    
    pMFS_UART_REG[Ch]->FCR0 = pMFS_UART_REG[Ch]->FCR0 | UART_FCR0_FCL1;
    
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
void MFS_UARTEnableFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    pMFS_UART_REG[Ch]->FCR0 = pMFS_UART_REG[Ch]->FCR0 | UART_FCR0_FE2;
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
void MFS_UARTEnableFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    pMFS_UART_REG[Ch]->FCR0 = pMFS_UART_REG[Ch]->FCR0 | UART_FCR0_FE1;
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
void MFS_UARTDisableFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    pMFS_UART_REG[Ch]->FCR0 = pMFS_UART_REG[Ch]->FCR0 & (~(UART_FCR0_FE2));
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
void MFS_UARTDisableFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    pMFS_UART_REG[Ch]->FCR0 = pMFS_UART_REG[Ch]->FCR0 & (~(UART_FCR0_FE1));
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear the FDRQ
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UARTClearFDRQ(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->FCR1 = pMFS_UART_REG[Ch]->FCR1 &
        (~UART_FCR1_FDRQ);
    return;
}

/*!
 ******************************************************************************
 ** \brief Initializes the UART internal state
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UARTSWRst(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR | UART_SCR_UPCL;
    return;
}
/*!
 ******************************************************************************
 ** \brief Transmit one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Data TX data
 ** \arg   5 - 9 bits data
 **
 ** \retval None
 **
 ******************************************************************************
 */
void MFS_UARTTXOneData(uint8_t Ch, uint16_t Data)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->TDR = Data;
    return;
}

/*!
 ******************************************************************************
 ** \brief Receive one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return Data
 ** \retval RX data 
 **
 ******************************************************************************
 */
uint16_t MFS_UARTRXOneData(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    return (pMFS_UART_REG[Ch]->RDR & 0x01FF);
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
void MFS_UARTEnableTX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR | UART_SCR_TXE;
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
void MFS_UARTEnableRX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR | UART_SCR_RXE;
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
void MFS_UARTDisableTX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR & (~UART_SCR_TXE);
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
void MFS_UARTDisableRX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR & (~UART_SCR_RXE);
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
void MFS_UARTEnableTXBusIdleInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR | UART_SCR_TBIE;
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
void MFS_UARTEnableTXInt(uint8_t Ch, void(*TXFunc)())
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    ASSERT(TXFunc != NULL);
    g_pMFSIRQCallback[Ch*3+1] = TXFunc;
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR | UART_SCR_TIE;
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
void MFS_UARTEnableTXFIFOEmptyInt(uint8_t Ch, void(*TXFunc)())
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    ASSERT(TXFunc != NULL);
    g_pMFSIRQCallback[Ch*3+1] = TXFunc;
    pMFS_UART_REG[Ch]->FCR1 = pMFS_UART_REG[Ch]->FCR1 | UART_FCR1_FTIE;
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
void MFS_UARTEnableRXInt(uint8_t Ch, void(*RXFunc)())
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    ASSERT(RXFunc != NULL);
    g_pMFSIRQCallback[Ch*3] = RXFunc;
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR | UART_SCR_RIE;
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
void MFS_UARTDisableTXBusIdleInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR & (~UART_SCR_TBIE);
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
void MFS_UARTDisableTXInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR & (~UART_SCR_TIE);
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
void MFS_UARTDisableTXFIFOEmptyInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= UART_Ch_MAX));
    pMFS_UART_REG[Ch]->FCR1 = pMFS_UART_REG[Ch]->FCR1 &
        (~UART_FCR1_FTIE);
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
void MFS_UARTDisableRXInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SCR = pMFS_UART_REG[Ch]->SCR & (~UART_SCR_RIE);
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
FlagStatusT MFS_UARTGetOEStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG[Ch]->SSR &
        UART_SSR_ORE) == UART_SSR_ORE));
}

/*!
 ******************************************************************************
 ** \brief Get Parity Error flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return Parity error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_UARTGetPEStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG[Ch]->SSR  &
        UART_SSR_PE) == UART_SSR_PE));
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
FlagStatusT MFS_UARTGetFEStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG[Ch]->SSR &
        UART_SSR_FRE) == UART_SSR_FRE));
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
FlagStatusT MFS_UARTGetRXRegFullStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG[Ch]->SSR &
        UART_SSR_RDRF) == UART_SSR_RDRF));
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
FlagStatusT MFS_UARTGetTXRegEmptyStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG[Ch]->SSR &
        UART_SSR_TDRE) == UART_SSR_TDRE));
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
FlagStatusT MFS_UARTGetTXBusIdleStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);

    return ((FlagStatusT)((pMFS_UART_REG[Ch]->SSR &
        UART_SSR_TBI) == UART_SSR_TBI));
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
uint16_t MFS_UARTGetFBYTE1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);

    return (pMFS_UART_REG[Ch]->FBYTE1);
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
uint16_t MFS_UARTGetFBYTE2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);

    return (pMFS_UART_REG[Ch]->FBYTE2);
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
void MFS_UARTErrorClr(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= UART_Ch_MAX);
    pMFS_UART_REG[Ch]->SSR = pMFS_UART_REG[Ch]->SSR | UART_SSR_REC;
    return;
}

/*****************************************************************************/

#endif

/* END OF FILE */
