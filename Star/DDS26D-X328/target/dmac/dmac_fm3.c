/*!
 ******************************************************************************
 **
 ** \file dmac_fm3.c
 **
 ** \brief this file provides all DMA drivers
 **
 ** \author FSLA AE Team
 **
 ** \version V0.21
 **
 ** \date 2012-08-09
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/

#include "dmac_fm3.h"

#if (PERIPHERAL_AVAILABLE_DMAC == OFF)
    #if (PERIPHERAL_ENABLE_DMAC == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_DMAC == ON) && (PERIPHERAL_AVAILABLE_DMAC == ON)
/*---------------------------------------------------------------------------*/
/* local definitions                                                         */
/*---------------------------------------------------------------------------*/
#define  DMACA_BASE_ADDR         (volatile uint32_t*)(&FM3_DMAC->DMACA0)
#define  DMACB_BASE_ADDR         (volatile uint32_t*)(&FM3_DMAC->DMACB0)
#define  DMACSA_BASE_ADDR        (volatile uint32_t*)(&FM3_DMAC->DMACSA0)
#define  DMACDA_BASE_ADDR        (volatile uint32_t*)(&FM3_DMAC->DMACDA0)

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*! \brief DMAC transfer completion callback function */  
static DMAC_TrsfCmpCallbackT* DMACTrstCmpCallback[8] = {(void*)0};
/*! \brief DMAC transfer error callback function */ 
static DMAC_TrsfErrCallbackT* DMACTrstErrCallback[8] = {(void*)0};

/*!
 ******************************************************************************
 ** \brief Enable the DMAC operation entirely
 **
 ** \param None
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_EntireEnable(void)
{
    bFM3_DMAC_DMACR_DE = 1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable the DMAC operation entirely
 **
 ** \param None
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_EntireDisable(void)
{
    bFM3_DMAC_DMACR_DE = 0;
    return;
}

/*!
 ******************************************************************************
 ** \brief Pause the DMAC operation entirely
 **
 ** \param None
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_EntireSetPause(void)
{
    FM3_DMAC->DMACR &= ~(15ul<<24);
    FM3_DMAC->DMACR |= (1ul<<24);
    return;
}

/*!
 ******************************************************************************
 ** \brief Cancel the pause of DMAC operation entirely
 **
 ** \param None
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_EntireCancelPause(void)
{
    FM3_DMAC->DMACR &= ~(15ul<<24);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get the stop status of DMAC operation entirely
 **
 ** \param None
 ** 
 ** \return None
 ******************************************************************************
 */
FlagStatusT DMAC_EntireGetStopStat(void)
{
    return (FlagStatusT)bFM3_DMAC_DMACR_DS;
}

/*!
 ******************************************************************************
 ** \brief Set the priority mode of DMAC
 **
 ** \param Mode
 ** \arg DMAC_PRIO_FIX
 ** \arg DMAC_PRIO_ROLATE
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_EntireSetPrioMode(uint8_t Mode)
{
    if(Mode == DMAC_PRIO_FIX)
    {
        bFM3_DMAC_DMACR_PR = 0;
    }
    else
    {
        bFM3_DMAC_DMACR_PR = 1;
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable the operation of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_EnableChOpt(uint8_t Ch)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr |= (1ul<<31);
    return;
}
 
/*!
 ******************************************************************************
 ** \brief Disable the operation of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_DisableChOpt(uint8_t Ch)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr &= ~(1ul<<31);
    return;
}

/*!
 ******************************************************************************
 ** \brief Pause the operation of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_SetChPause(uint8_t Ch)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr |= (1ul<<30);
    return;
}


/*!
 ******************************************************************************
 ** \brief Cancel the pause of the operation of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_CancelChPause(uint8_t Ch)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr &= ~(1ul<<30);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable software trigger of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_EnableChSWTrig(uint8_t Ch)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr |= (1ul<<29);
    return;
}


/*!
 ******************************************************************************
 ** \brief Disable software trigger of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_DisableChSWTrig(uint8_t Ch)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr &= ~(1ul<<29);
    return;
}

/*!
 ******************************************************************************
 ** \brief Select the input trigger of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param SrcTrig Input source trigger
 ** \arg DMAC_SW_TRIG
 ** \arg [DAMC_USB0_EP1_INT, DAMC_USB0_EP5_INT]
 ** \arg [DMAC_ADC0_SCAN_INT, DMAC_ADC2_SCAN_INT]
 ** \arg DMAC_BT0_IRQ0, DMAC_BT0_IRQ2, DMAC_BT0_IRQ4, DMAC_BT0_IRQ6
 ** \arg DMAC_MFS0_RX_INT, DMAC_MFS0_TX_INT
 ** \arg ...
 ** \arg DMAC_MFS7_RX_INT, DMAC_MFS7_TX_INT
 ** \arg [DMAC_EXT_INT_0, DMAC_EXT_INT_3]
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_SelectChInputTrig(uint8_t Ch, DMAC_InputSourceT ScrTrig)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr &= ~(63ul<<23);
    *pdmacaaddr |= (ScrTrig<<23);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the block count of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param Cnt Block count
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_SetChBlockCnt(uint8_t Ch, uint8_t Cnt)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr &= ~(15ul<<16);
    *pdmacaaddr |= ((Cnt-1)<<16);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the transfer count of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param Cnt Transfer count
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_SetChTrstCnt(uint8_t Ch, uint16_t Cnt)
{
    volatile uint32_t* pdmacaaddr;
    pdmacaaddr = DMACA_BASE_ADDR + 4*Ch;
    *pdmacaaddr &= 0xFFFF0000;
    *pdmacaaddr |= (Cnt-1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the transfer mode of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param Mode Transfer mode
 ** \arg DMAC_BLOCK_TRSF_MODE
 ** \arg DMAC_BURST_TRSF_MODE
 ** \arg DMAC_DEMAND_TRSF_MODE
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_SetChMode(uint8_t Ch, DMAC_ModeT Mode)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr &= ~(3ul<<28);
    *pdmacbaddr |= (Mode<<28);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the transfer width of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param Width Transfer width
 ** \arg DMAC_TRST_WIDTH_8BIT
 ** \arg DMAC_TRST_WIDTH_16BIT
 ** \arg DMAC_TRST_WIDTH_32BIT
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_SetChTrsfWidth(uint8_t Ch, DMAC_TrsfWidthT Width)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr &= ~(3ul<<26);
    *pdmacbaddr |= (Width<<26);
    return;
}

/*!
 ******************************************************************************
 ** \brief Configure the source information of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param FS Fixed source
 ** \arg DMAC_ADDR_INC
 ** \arg DMAC_ADDR_FIX
 ** \param RS Reload count for source
 ** \arg DMAC_RELOAD_DISABLE
 ** \arg DMAC_RELOAD_ENABLE
 ** \param SA Source address
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_ConfigChSrcInfo(uint8_t Ch, uint8_t FS, uint8_t RS, uint32_t SA)
{
    volatile uint32_t* pdmacbaddr;
    volatile uint32_t* pdmacsaaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    pdmacsaaddr = DMACSA_BASE_ADDR + 4*Ch;
    /* FS setting */
    *pdmacbaddr &= ~(1ul<<25);
    *pdmacbaddr |= (FS<<25);
    /* RS setting */
    *pdmacbaddr &= ~(1ul<<22);
    *pdmacbaddr |= (RS<<22);
    /* SA setting */
    *pdmacsaaddr = SA;
    return;
}

/*!
 ******************************************************************************
 ** \brief Configure the destination information of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param FD Fixed destination
 ** \arg DMAC_ADDR_INC
 ** \arg DMAC_ADDR_FIX
 ** \param RD Reload count for destination
 ** \arg DMAC_RELOAD_DISABLE
 ** \arg DMAC_RELOAD_ENABLE
 ** \param DA Destination address
 ** 
 ** \return None
 ******************************************************************************
 */
void DMAC_ConfigChDstInfo(uint8_t Ch, uint8_t FD, uint8_t RD, uint32_t DA)
{
    volatile uint32_t* pdmacbaddr;
    volatile uint32_t* pdmacdaaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    pdmacdaaddr = DMACDA_BASE_ADDR + 4*Ch;
    /* FD setting */
    *pdmacbaddr &= ~(1ul<<24);
    *pdmacbaddr |= (FD<<24);
    /* RD setting */
    *pdmacbaddr &= ~(1ul<<21);
    *pdmacbaddr |= (RD<<21);
    /* DA setting */
    *pdmacdaaddr = DA;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable reload count of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_EnableChReloadCnt(uint8_t Ch)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr |= (1ul<<23);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable reload count of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_DisableChReloadCnt(uint8_t Ch)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr &= ~(1ul<<23);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable error interrupt of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param TrsfErrCallback Transfer error callback function
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_EnableChErrInt(uint8_t Ch, DMAC_TrsfErrCallbackT* TrsfErrCallback)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr |= (1ul<<20);
    DMACTrstErrCallback[Ch] = TrsfErrCallback;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable error interrupt of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_DisableChErrInt(uint8_t Ch)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr &= ~(1ul<<20);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable completion interrupt of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 ** \param TrsfCmpCallback transfer completion callback function
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_EnableChCmpInt(uint8_t Ch, DMAC_TrsfCmpCallbackT* TrsfCmpCallback)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr |= (1ul<<19);
    DMACTrstCmpCallback[Ch] = TrsfCmpCallback;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable completion interrupt of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_DisableChCmpInt(uint8_t Ch)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr &= ~(1ul<<19);
    return;
}


/*!
 ******************************************************************************
 ** \brief Get the stop status of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return Stop status
 ** \arg DMAC_STOP_STAT_INIT
 ** \arg DMAC_STOP_STAT_TRSF_ERR
 ** \arg DMAC_STOP_STAT_FORCE_STOP
 ** \arg DMAC_STOP_STAT_SRC_ERR
 ** \arg DMAC_STOP_STAT_DST_ERR
 ** \arg DMAC_STOP_STAT_TRSF_SUCCESS
 ** \arg DMAC_STOP_STAT_TRSF_PAUSE
 ******************************************************************************
 */
DMAC_StopStatT DMAC_GetChStopStat(uint8_t Ch)
{
    volatile uint32_t* pdmacbaddr;
    uint8_t stat;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    stat = (*pdmacbaddr & (7ul<<16)) >> 16;
    switch (stat)
    {
        case DMAC_STOP_STAT_INIT:
        case DMAC_STOP_STAT_TRSF_ERR:
        case DMAC_STOP_STAT_FORCE_STOP:
        case DMAC_STOP_STAT_SRC_ERR:
        case DMAC_STOP_STAT_DST_ERR:
        case DMAC_STOP_STAT_TRSF_SUCCESS:
            return (DMAC_StopStatT)stat;
        case DMAC_STOP_STAT_TRSF_PAUSE:
            return DMAC_STOP_STAT_TRSF_PAUSE;
    }
   return DMAC_STOP_STAT_INIT;
}

/*!
 ******************************************************************************
 ** \brief Clear the stop status flag of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_ClrChStopStat(uint8_t Ch)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr &= ~(7ul<<16);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable enable bit clear of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_EnableChEnBitClr(uint8_t Ch)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr &= ~(1ul);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable enable bit clear of a specific DMAC channel 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_DisableChEnBitClr(uint8_t Ch)
{
    volatile uint32_t* pdmacbaddr;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    *pdmacbaddr |= (1ul);
    return;
}


/*!
 ******************************************************************************
 ** \brief Disable enable bit clear of a specific DMAC channel 
 **
 ** \param Src Interrupt source type
 ** \param OutputType Interrupt output type
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_SelIntOutput(DMAC_IntSrcT Src, DMAC_IntOutputT OutputType)
{
    FM3_INTREQ->DRQSEL &= ~(1ul<<Src);
    FM3_INTREQ->DRQSEL |= (OutputType<<Src);
    return;
}

/*!
 ******************************************************************************
 ** \brief DMA interrupt handler 
 **
 ** \param Ch DMAC channel
 ** \arg DMAC_CH0
 ** \arg ...
 ** \arg DMAC_CH7
 **
 ** \return None
 ******************************************************************************
 */
void DMAC_IntHandler(uint8_t Ch)
{
    volatile uint32_t *pdmacbaddr; 
    uint8_t stat;
    pdmacbaddr = DMACB_BASE_ADDR + 4*Ch;
    stat = (*pdmacbaddr & (7ul<<16)) >> 16;
    switch (stat)
    {
        case DMAC_STOP_STAT_TRSF_ERR:    
        case DMAC_STOP_STAT_FORCE_STOP:
        case DMAC_STOP_STAT_SRC_ERR:
        case DMAC_STOP_STAT_DST_ERR:
            if(DMACTrstErrCallback != (void*)0)
            {
                DMACTrstErrCallback[Ch]();
            }
            break;
        case DMAC_STOP_STAT_TRSF_SUCCESS:
            if(DMACTrstCmpCallback != (void*)0)
            {
                DMACTrstCmpCallback[Ch]();
            }
            break;
        default:
            break;
        
    }
    *pdmacbaddr &= ~(7ul<<16); /* Clear interrupt flag */
}
#endif
