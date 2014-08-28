/*!
 ******************************************************************************
 **
 ** \file csio_fm3.c	
 **
 ** \brief CSIO drivers
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
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "csio_fm3.h"

#if (PERIPHERAL_AVAILABLE_MFS == OFF)
    #if (PERIPHERAL_ENABLE_MFS_CSIO == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFS_CSIO == ON) &&  (PERIPHERAL_AVAILABLE_MFS == ON)
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
    static FM3_MFS_CSIO_TypeDef* pMFS_CSIO_REG[4] = 
    {
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS0_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS1_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS2_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS3_CSIO),
    };
#elif (MFS_MAX_CH == 6)
    static FM3_MFS_CSIO_TypeDef* pMFS_CSIO_REG[6] = 
    {
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS0_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS1_CSIO),
        NULL,
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS3_CSIO),
        NULL,
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS5_CSIO),
    };
#elif (MFS_MAX_CH == 8)
    static FM3_MFS_CSIO_TypeDef* pMFS_CSIO_REG[8] = 
    {
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS0_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS1_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS2_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS3_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS4_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS5_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS6_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS7_CSIO),
    };   
#elif (MFS_MAX_CH == 12)
    static FM3_MFS_CSIO_TypeDef* pMFS_CSIO_REG[12] = 
    {
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS0_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS1_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS2_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS3_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS4_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS5_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS6_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS7_CSIO),
        NULL,
        NULL,
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS10_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS11_CSIO),
    };   
#elif (MFS_MAX_CH == 14)
    static FM3_MFS_CSIO_TypeDef* pMFS_CSIO_REG[14] = 
    {
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS0_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS1_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS2_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS3_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS4_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS5_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS6_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS7_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS8_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS9_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS10_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS11_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS12_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS13_CSIO),
    };   
#elif (MFS_MAX_CH == 16)
    static FM3_MFS_CSIO_TypeDef* pMFS_CSIO_REG[16] = 
    {
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS0_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS1_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS2_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS3_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS4_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS5_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS6_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS7_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS8_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS9_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS10_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS11_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS12_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS13_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS14_CSIO),
        (FM3_MFS_CSIO_TypeDef*)(FM3_MFS15_CSIO),
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
 ** \brief Config the CSIO baud rate
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **     
 ** \param baudrate
 ** \arg   Baud rate value
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigBaudrate(uint8_t Ch, MFS_CSIOModeConfigT *pModeConfig)
{
    uint16_t tBGR;
    uint32_t tSysFrePCLK2;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    tSysFrePCLK2 = SystemCoreClock/(1<<((FM3_CRG->APBC2_PSR&0x03)));
    tBGR = (uint16_t)(((tSysFrePCLK2 + (pModeConfig->Baudrate / 2)) / (uint32_t)pModeConfig->Baudrate) - 1);

    pMFS_CSIO_REG[Ch]->BGR = tBGR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO Mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOSetCSIOMode(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSMR = pMFS_CSIO_REG[Ch]->SMR;
    tSMR = tSMR & 0x1F;//Clear MD0-2
    tSMR = tSMR | MFS_MODE_CSIO;//Set MD0-2 as 010
    pMFS_CSIO_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO SIO mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigSIO(uint8_t Ch)
{
    uint8_t tSCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSCR = pMFS_CSIO_REG[Ch]->SCR;
    tSCR = tSCR & (~(CSIO_SCR_SPI));
    pMFS_CSIO_REG[Ch]->SCR = tSCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO SPI mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigSPI(uint8_t Ch)
{
    uint8_t tSCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSCR = pMFS_CSIO_REG[Ch]->SCR;
    tSCR = tSCR | (CSIO_SCR_SPI);
    pMFS_CSIO_REG[Ch]->SCR = tSCR;
    return;
}

/*!
 ******************************************************************************
 ** \brief Config the CSIO Master mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigMaster(uint8_t Ch)
{
    uint8_t tSCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSCR = pMFS_CSIO_REG[Ch]->SCR;
    tSCR = tSCR & (~(CSIO_SCR_MS));
    pMFS_CSIO_REG[Ch]->SCR = tSCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO Slave mode
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigSlave(uint8_t Ch)
{
    uint8_t tSCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSCR = pMFS_CSIO_REG[Ch]->SCR;
    tSCR = tSCR | (CSIO_SCR_MS);
    pMFS_CSIO_REG[Ch]->SCR = tSCR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO BDS as LSB
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigLSB(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSMR = pMFS_CSIO_REG[Ch]->SMR;
    tSMR = tSMR & (~(CSIO_SMR_BDS));
    pMFS_CSIO_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO BDS as MSB
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigMSB(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSMR = pMFS_CSIO_REG[Ch]->SMR;
    tSMR = tSMR | (CSIO_SMR_BDS);
    pMFS_CSIO_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Enable the CSIO Clock
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableClock(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSMR = pMFS_CSIO_REG[Ch]->SMR;
    tSMR = tSMR | (CSIO_SMR_SCKE);
    pMFS_CSIO_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Disable the CSIO Clock
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableClock(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSMR = pMFS_CSIO_REG[Ch]->SMR;
    tSMR = tSMR & (~(CSIO_SMR_SCKE));
    pMFS_CSIO_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO Not Invert
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigNoInv(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSMR = pMFS_CSIO_REG[Ch]->SMR;
    tSMR = tSMR & (~(CSIO_SMR_SCINV));
    pMFS_CSIO_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO Invert
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigInv(uint8_t Ch)
{
    uint8_t tSMR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    tSMR = pMFS_CSIO_REG[Ch]->SMR;
    tSMR = tSMR | (CSIO_SMR_SCINV);
    pMFS_CSIO_REG[Ch]->SMR = tSMR;
    return;
}
/*!
 ******************************************************************************
 ** \brief Config the CSIO wait bit length
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Mode initialization data
 ** \arg   Mode info
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigWT(uint8_t Ch, MFS_CSIOModeConfigT *pModeConfig)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    ASSERT(pModeConfig->Waitbits <= CSIO_WAITBITS_3);

    /* Wait bit */
    switch(pModeConfig->Waitbits)
    {
        case CSIO_WAITBITS_0:
            break;
        case CSIO_WAITBITS_1:
            tESCR = tESCR | CSIO_ESCR_WT0;
            break;
        case CSIO_WAITBITS_2:
            tESCR = tESCR | CSIO_ESCR_WT1;
            break;
        case CSIO_WAITBITS_3:
            tESCR = tESCR | CSIO_ESCR_WT0;
            tESCR = tESCR | CSIO_ESCR_WT1;
            break;
        default:
            break;
    }

    pMFS_CSIO_REG[Ch]->ESCR = (pMFS_CSIO_REG[Ch]->ESCR & ~(CSIO_ESCR_WT0 | CSIO_ESCR_WT1)) | tESCR;
    return;
}

/*!
 ******************************************************************************
 ** \brief Config the CSIO data bit length
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Mode initialization data
 ** \arg   Mode info
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigDT(uint8_t Ch, MFS_CSIOModeConfigT *pModeConfig)
{
    uint8_t tESCR = 0;

    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    ASSERT((pModeConfig->Databits >= CSIO_DATABITS_5) && (pModeConfig->Databits <= CSIO_DATABITS_9));

    /* Data bits */
    switch(pModeConfig->Databits)
    {
        case CSIO_DATABITS_8:
          tESCR = tESCR | CSIO_ESCR_BITLEN_8;
          break;
        case CSIO_DATABITS_5:
          tESCR = tESCR | CSIO_ESCR_BITLEN_5;
          break;
        case CSIO_DATABITS_6:
          tESCR = tESCR | CSIO_ESCR_BITLEN_6;
          break;
        case CSIO_DATABITS_7:
          tESCR = tESCR | CSIO_ESCR_BITLEN_7;
          break;
        case CSIO_DATABITS_9:
          tESCR = tESCR | CSIO_ESCR_BITLEN_9;
          break;
        default:
          break;
    }

    pMFS_CSIO_REG[Ch]->ESCR = (pMFS_CSIO_REG[Ch]->ESCR & ~(CSIO_ESCR_BITLEN_5 | CSIO_ESCR_BITLEN_6 | CSIO_ESCR_BITLEN_7 | CSIO_ESCR_BITLEN_8 | CSIO_ESCR_BITLEN_9)) | tESCR;
    return;
}

/*!
 ******************************************************************************
 ** \brief Config the CSIO FIFO
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
void MFS_CSIOConfigFSEL(uint8_t Ch, MFS_FIFOSelectionT FIFOSel)
{
    uint8_t tFCR1 = 0;
    
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    ASSERT((FIFOSel == MFS_FIFO1TX_FIFO2RX) || (FIFOSel == MFS_FIFO2TX_FIFO1RX));

    tFCR1 = pMFS_CSIO_REG[Ch]->FCR1;
    if(FIFOSel == MFS_FIFO1TX_FIFO2RX)
    {
        tFCR1 = tFCR1 & (~CSIO_FCR1_FSEL);
    }
    else
    {
        tFCR1 = tFCR1 | CSIO_FCR1_FSEL;
    }

    pMFS_CSIO_REG[Ch]->FCR1 = tFCR1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Config the CSIO FIFO FByte1
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param FByte1
 ** \arg   0 - 127
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigFByte1(uint8_t Ch, uint8_t FByte1)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    
    pMFS_CSIO_REG[Ch]->FBYTE1 = FByte1;
}

/*!
 ******************************************************************************
 ** \brief Config the CSIO FIFO FByte2
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param FByte2
 ** \arg   0 - 127
 **         
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOConfigFByte2(uint8_t Ch, uint8_t FByte2)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    
    pMFS_CSIO_REG[Ch]->FBYTE2 = FByte2;
}
/*!
 ******************************************************************************
 ** \brief Clear the FDRQ
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOClearFDRQ(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->FCR1 = pMFS_CSIO_REG[Ch]->FCR1 &
        (~CSIO_FCR1_FDRQ);
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
void MFS_CSIOEnableSOE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SMR = pMFS_CSIO_REG[Ch]->SMR  | CSIO_SMR_SOE;
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
void MFS_CSIODisableSOE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SMR = pMFS_CSIO_REG[Ch]->SMR & (~CSIO_SMR_SOE);
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
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOTXOneData(uint8_t Ch, uint16_t Data)
{   
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->TDR = Data;
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
 **
 ******************************************************************************
 */
uint16_t MFS_CSIORXOneData(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    return (pMFS_CSIO_REG[Ch]->RDR & 0x01FF);
}

/*!
 ******************************************************************************
 ** \brief Transfer one data
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **         
 ** \param Data
 ** \arg   5 - 9 bits data
 **
 ** \return None
 **
 ******************************************************************************
 */
uint16_t MFS_CSIOTransferOneData(uint8_t Ch, uint16_t Data)
{
    uint16_t read;
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    while(!MFS_CSIOGetTXBusIdleStatus(Ch));
    
    MFS_CSIOEnableRX(Ch);
    MFS_CSIOEnableTX(Ch);
    
    MFS_CSIOTXOneData(Ch,Data);
    
    while(!MFS_CSIOGetRXRegFullStatus(Ch));
    
    read = MFS_CSIORXOneData(Ch);
    
    MFS_CSIODisableRX(Ch);
    
    return read;
}

/*!
 ******************************************************************************
 ** \brief Enable tranmit function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableTX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR | CSIO_SCR_TXE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable receive function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableRX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);    
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR | CSIO_SCR_RXE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable transmit
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableTX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR & (~CSIO_SCR_TXE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable receive function
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableRX(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR & (~CSIO_SCR_RXE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX Bus idle interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableTXBusIdleInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR | CSIO_SCR_TBIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable transmit interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableTXInt(uint8_t Ch, void(*TXFunc)())
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    ASSERT(TXFunc != NULL);
    g_pMFSIRQCallback[Ch*3+1] = TXFunc;
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR | CSIO_SCR_TIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable TX FIFO Empty interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableTXFIFOEmptyInt(uint8_t Ch, void(*TXFunc)())
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    ASSERT(TXFunc != NULL);
    g_pMFSIRQCallback[Ch*3+1] = TXFunc;
    pMFS_CSIO_REG[Ch]->FCR1 = pMFS_CSIO_REG[Ch]->FCR1 | CSIO_FCR1_FTIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable receive interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableRXInt(uint8_t Ch, void(*RXFunc)())
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    ASSERT(RXFunc != NULL);
    g_pMFSIRQCallback[Ch*3] = RXFunc;
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR | CSIO_SCR_RIE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable the Data Lost detection
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableFLSTE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR1 = pMFS_CSIO_REG[Ch]->FCR1 | CSIO_FCR1_FLSTE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable the received FIFO idle detection.
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIOEnableFRIIE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR1 = pMFS_CSIO_REG[Ch]->FCR1 | CSIO_FCR1_FRIIE;
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
void MFS_CSIOEnableFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR0 = pMFS_CSIO_REG[Ch]->FCR0 | CSIO_FCR0_FE2;
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
void MFS_CSIOEnableFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR0 = pMFS_CSIO_REG[Ch]->FCR0 | CSIO_FCR0_FE1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX Bus idle interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableTXBusIdleInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR & (~CSIO_SCR_TBIE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable transmit interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableTXInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR & (~CSIO_SCR_TIE);
    g_pMFSIRQCallback[Ch*3+1] = NULL;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable TX FIFO Empty interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableTXFIFOEmptyInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR1 = pMFS_CSIO_REG[Ch]->FCR1 & (~CSIO_FCR1_FTIE);
    g_pMFSIRQCallback[Ch*3+1] = NULL;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable receive interrupt
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableRXInt(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR & (~CSIO_SCR_RIE);
    g_pMFSIRQCallback[Ch*3] = NULL;
    return;
}


/*!
 ******************************************************************************
 ** \brief Disable the Data Lost detection
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableFLSTE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR1 = pMFS_CSIO_REG[Ch]->FCR1 & ~(CSIO_FCR1_FLSTE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable the received FIFO idle detection.
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return None
 **
 ******************************************************************************
 */
void MFS_CSIODisableFRIIE(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR1 = pMFS_CSIO_REG[Ch]->FCR1 & ~(CSIO_FCR1_FRIIE);
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
void MFS_CSIODisableFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR0 = pMFS_CSIO_REG[Ch]->FCR0 & ~(CSIO_FCR0_FE2);
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
void MFS_CSIODisableFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    pMFS_CSIO_REG[Ch]->FCR0 = pMFS_CSIO_REG[Ch]->FCR0 & ~(CSIO_FCR0_FE1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get Overrun Error flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return Overrun error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_CSIOGetOEStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return ((FlagStatusT)((pMFS_CSIO_REG[Ch]->SSR &
        CSIO_SSR_ORE) == CSIO_SSR_ORE));
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
FlagStatusT MFS_CSIOGetRXRegFullStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return ((FlagStatusT)((pMFS_CSIO_REG[Ch]->SSR &
        CSIO_SSR_RDRF) == CSIO_SSR_RDRF));
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
FlagStatusT MFS_CSIOGetTXRegEmptyStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return ((FlagStatusT)((pMFS_CSIO_REG[Ch]->SSR &
        CSIO_SSR_TDRE) == CSIO_SSR_TDRE));
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
FlagStatusT MFS_CSIOGetTXBusIdleStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return ((FlagStatusT)((pMFS_CSIO_REG[Ch]->SSR &
        CSIO_SSR_TBI) == CSIO_SSR_TBI));
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
uint8_t MFS_CSIOFBYTE1Status(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return (pMFS_CSIO_REG[Ch]->FBYTE1);
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
uint8_t MFS_CSIOFBYTE2Status(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);

    return (pMFS_CSIO_REG[Ch]->FBYTE2);
}
/*!
 ******************************************************************************
 ** \brief Get data lost flag flag
 **
 ** \param Ch Requested channel
 ** \arg   MFS_Ch0-MFS_Ch15
 **
 ** \return Overrun error flag
 ** \retval SET
 ** \retval RESET
 **
 ******************************************************************************
 */
FlagStatusT MFS_CSIOGetFLSTStatus(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));

    return ((FlagStatusT)((pMFS_CSIO_REG[Ch]->FCR0 &
        CSIO_FCR0_FLST) == CSIO_FCR0_FLST));
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
void MFS_CSIOErrorClr(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SSR = pMFS_CSIO_REG[Ch]->SSR | CSIO_SSR_REC;
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
void MFS_CSIOProgramClr(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT(Ch <= CSIO_Ch_MAX);
    pMFS_CSIO_REG[Ch]->SCR = pMFS_CSIO_REG[Ch]->SCR | CSIO_SCR_UPCL;
    return;
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
void MFS_CSIOResetFIFO2(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    
    pMFS_CSIO_REG[Ch]->FCR0 = pMFS_CSIO_REG[Ch]->FCR0 | CSIO_FCR0_FCL2;
    
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
void MFS_CSIOResetFIFO1(uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((Ch <= CSIO_Ch_MAX));
    
    pMFS_CSIO_REG[Ch]->FCR0 = pMFS_CSIO_REG[Ch]->FCR0 | CSIO_FCR0_FCL1;
    
    return;
}
/*****************************************************************************/

#endif

/* END OF FILE */
