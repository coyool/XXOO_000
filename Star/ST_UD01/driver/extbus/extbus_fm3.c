/*!
 ******************************************************************************
 **
 ** \file extbus_fm3.c
 **
 ** \brief this file provides external interrupt drivers.
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
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "extbus_fm3.h"

#if (PERIPHERAL_ENABLE_EXTBUS == ON) && (PERIPHERAL_AVAILABLE_EXTBUS == ON)

/*---------------------------------------------------------------------------*/
/* local definitions                                                         */
/*---------------------------------------------------------------------------*/
#define EXTBMODE_BASE_ADDR     (volatile uint32_t*)(&FM3_EXBUS->MODE0)
#define EXTBTIM_BASE_ADDR      (volatile uint32_t*)(&FM3_EXBUS->TIM0)
#define EXTBAREA_BASE_ADDR     (volatile uint32_t*)(&FM3_EXBUS->AREA0)
#if (MCU_TYPE != DEVICE_TYPE0)
#define EXTBATIM_BASE_ADDR     (volatile uint32_t*)(&FM3_EXBUS->ATIM0)
#define EXTBDCLKR_BASE_ADDR    (volatile uint32_t*)(&FM3_EXBUS->DCLKR)
#endif
/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local functions prototypes                                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

#if (MCU_TYPE != DEVICE_TYPE0)
/*!
 ******************************************************************************
 ** \brief Set MOEX width
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 ** 
 ** \param Width
 ** \arg EXTB_MOEX_WIDTH_FRADC
 ** \arg EXTB_MOEX_WIDTH_RACC_RADC
 **
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetMOEXWidth(uint8_t CS, uint8_t Width)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x1ul<<EXTB_MODEX_MOEXEUP);
    *pModex |= Width<<EXTB_MODEX_MOEXEUP;
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear MPXCSOF bit. Asserts MCSX in ALC cycle period
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableMCSXInALC(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x1ul<<EXTB_MODEX_MPXCSOF);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set MPXCSOF bit. Does not assert MCSX in ALC cycle period
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **  
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableMCSXInALC(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex |= 0x1ul<<EXTB_MODEX_MPXCSOF;
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear MPXDOFF bit. Outputs the address to the data lines
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableAddrOutput(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x1ul<<EXTB_MODEX_MPXDOFF);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set MPXDOFF bit. Does not output the address to the data lines.
 **  
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableAddrOutput(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex |= 0x1ul<<EXTB_MODEX_MPXDOFF;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set ALE signal polarity
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **
 ** \param Polarity
 ** \arg EXTB_ALE_P
 ** \arg EXTB_ALE_N
 **       
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetALEPolarity(uint8_t CS, uint8_t Polarity)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(1ul<<EXTB_MODEX_ALEINV);
    *pModex |= Polarity<<EXTB_MODEX_ALEINV;
    return;
}


/*!
 ******************************************************************************
 ** \brief Set External bus operation mode
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **
 ** \param Polarity
 ** \arg EXTB_MODE_SEPERATE
 ** \arg EXTB_MODE_MULTIPLEX
 **       
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetOptMode(uint8_t CS, uint8_t Mode)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(1ul<<EXTB_MODEX_MPXMODE);
    *pModex |= Mode<<EXTB_MODEX_MPXMODE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear SHRTDOUT bit. Extends the write data output to the last idle cycle
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableWriteIdleCycleExtend(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(1ul<<EXTB_MODEX_SHRTDOUT);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set SHRTDOUT bit. Stops the write data output at the first idle cycle
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableWriteIdleCycleExtend(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex |= 0x1ul<<EXTB_MODEX_SHRTDOUT;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set RDY bit. External RDY mode ON
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableExtRDY(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex |= 0x1ul<<EXTB_MODEX_RDY;
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear RDY bit. External RDY mode OFF
 **
 ** \param Ch
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableExtRDY(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x1ul<<EXTB_MODEX_RDY);
    return;
}
#endif

/*!
 ******************************************************************************
 ** \brief Set PAGE bit. NOR Flash memory page access mode is turned ON
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableNorPageMode(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex |= 0x1ul<<EXTB_MODEX_PAGE;
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear PAGE bit. NOR Flash memory page access mode is turned OFF
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableNorPageMode(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x1ul<<EXTB_MODEX_PAGE);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set NAND bit. NAND Flash memory mode is turned ON
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableNandMode(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex |= 0x1ul<<EXTB_MODEX_NAND;
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear NAND bit. NAND Flash memory mode is turned OFF
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableNandMode(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x1ul<<EXTB_MODEX_NAND);
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear WEOFF bit. WEX enable
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableWEX(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x1ul<<EXTB_MODEX_WEOFF);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set WEOFF bit. WEX disable
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableWEX(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex |= 0x1ul<<EXTB_MODEX_WEOFF;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set RBMON bit.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableReadByteMask(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex |= 0x1ul<<EXTB_MODEX_RBMON;
    return;
}

/*!
 ******************************************************************************
 ** \brief Clear RBMON bit.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableReadByteMask(uint8_t CS)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x1ul<<EXTB_MODEX_RBMON);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set EX Bus Data Width, 8bit or 16bit.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Width
 ** \arg EXTB_DATAWIDTH_8B     -> 0
 ** \arg EXTB_DATAWIDTH_16B   -> 1
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetDataWidth(uint8_t CS, uint8_t Width)
{
    volatile uint32_t *pModex;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Width <= EXTB_DATAWIDTH_16B);
    pModex = EXTBMODE_BASE_ADDR + CS;
    *pModex &= ~(0x03ul<<EXTB_MODEX_WDTH0);
    *pModex |= (uint32_t)Width<<EXTB_MODEX_WDTH0;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the number of idle cycles after write access, 1 ~ 16 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_WIDLC_01C
 ** \arg EXTB_WIDLC_02C
 ** \arg ...
 ** \arg EXTB_WIDLC_15C
 ** \arg EXTB_WIDLC_16C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetWriteIdleCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pTIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_TIM_WIDLC_MAX);
    pTIMx = EXTBTIM_BASE_ADDR + CS;
    *pTIMx &= ~(0x0Ful<<EXTB_TIMX_WIDLC);
    *pTIMx |= (uint32_t)Cycle<<EXTB_TIMX_WIDLC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the number of assert cycles of write enable, 1 ~ 15 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_WWEC_01C
 ** \arg EXTB_WWEC_02C
 ** \arg ...
 ** \arg EXTB_WWEC_14C
 ** \arg EXTB_WWEC_15C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetWriteEnableCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pTIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_TIM_WWEC_MAX);
    pTIMx = EXTBTIM_BASE_ADDR + CS;
    *pTIMx &= ~(0x0Ful<<EXTB_TIMX_WWEC);
    *pTIMx |= (uint32_t)Cycle<<EXTB_TIMX_WWEC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the number of setup cycles after write address, 1 ~ 15 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_WADC_01C
 ** \arg EXTB_WADC_02C
 ** \arg ...
 ** \arg EXTB_WADC_14C
 ** \arg EXTB_WADC_15C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetWriteAddrSetupCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pTIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_TIM_WADC_MAX);
    pTIMx = EXTBTIM_BASE_ADDR + CS;
    *pTIMx &= ~(0x0Ful<<EXTB_TIMX_WADC);
    *pTIMx |= (uint32_t)Cycle<<EXTB_TIMX_WADC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the number of cycles required of write access, 3 ~ 16 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_WACC_03C
 ** \arg EXTB_WACC_04C
 ** \arg ...
 ** \arg EXTB_WACC_15C
 ** \arg EXTB_WACC_16C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetWriteAccessCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pTIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT((Cycle >= EXTB_TIM_WACC_MIN) && (Cycle <= EXTB_TIM_WACC_MAX));
    pTIMx = EXTBTIM_BASE_ADDR + CS;
    *pTIMx &= ~(0x0Ful<<EXTB_TIMX_WACC);
    *pTIMx |= (uint32_t)Cycle<<EXTB_TIMX_WACC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the number of idle cycles after read access, 1 ~ 16 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_RIDLC_01C
 ** \arg EXTB_RIDLC_02C
 ** \arg ...
 ** \arg EXTB_RIDLC_15C
 ** \arg EXTB_RIDLC_16C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetReadIdleCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pTIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_TIM_RIDLC_MAX);
    pTIMx = EXTBTIM_BASE_ADDR + CS;
    *pTIMx &= ~(0x0Ful<<EXTB_TIMX_RIDLC);
    *pTIMx |= (uint32_t)Cycle<<EXTB_TIMX_RIDLC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the initial wait time of the addr during read access to Flash memory, 0 ~ 15 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 ** 
 ** \param Cycle
 ** \arg EXTB_FRADC_00C
 ** \arg EXTB_FRADC_01C
 ** \arg ...
 ** \arg EXTB_FRADC_14C
 ** \arg EXTB_FRADC_15C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetFristReadCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pTIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_TIM_FRADC_MAX);
    pTIMx = EXTBTIM_BASE_ADDR + CS;
    *pTIMx &= ~(0x0Ful<<EXTB_TIMX_FRADC);
    *pTIMx |= (uint32_t)Cycle<<EXTB_TIMX_FRADC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the number of setup cycles of read access, 0 ~ 15 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_RADC_00C
 ** \arg EXTB_RADC_01C
 ** \arg ...
 ** \arg EXTB_RADC_14C
 ** \arg EXTB_RADC_15C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetReadAddrSetupCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pTIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_TIM_RADC_MAX);
    pTIMx = EXTBTIM_BASE_ADDR + CS;
    *pTIMx &= ~(0x0Ful<<EXTB_TIMX_RADC);
    *pTIMx |= (uint32_t)Cycle<<EXTB_TIMX_RADC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the number of cycles required for read access, 1 ~ 16 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_RACC_01C
 ** \arg EXTB_RACC_02C
 ** \arg ...
 ** \arg EXTB_RACC_15C
 ** \arg EXTB_RACC_16C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetReadAccessCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pTIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_TIM_RACC_MAX);
    pTIMx = EXTBTIM_BASE_ADDR + CS;
    *pTIMx &= ~(0x0Ful<<EXTB_TIMX_RACC);
    *pTIMx |= (uint32_t)Cycle<<EXTB_TIMX_RACC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the address mask.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param MaskAddr
 ** \arg EXTB_MASKADDR_128MB
 ** \arg EXTB_MASKADDR_64MB
 ** \arg EXTB_MASKADDR_32MB
 ** \arg EXTB_MASKADDR_16MB
 ** \arg EXTB_MASKADDR_8MB
 ** \arg EXTB_MASKADDR_4MB
 ** \arg EXTB_MASKADDR_2MB
 ** \arg EXTB_MASKADDR_1MB
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetMaskAddress(uint8_t CS, uint8_t MaskAddr)
{
    volatile uint32_t *pAREAx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(MaskAddr <= EXTB_AREA_MASK_MAX);
    pAREAx = EXTBAREA_BASE_ADDR + CS;
    *pAREAx &= ~(0x7Ful<<EXTB_AREAX_MASK);
    *pAREAx |= (uint32_t)MaskAddr<<EXTB_AREAX_MASK;
    return;
}

/*!
 ******************************************************************************
 ** \brief These bits specify the address to set the corresponding MCSX area.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Addr
 ** \arg 00~FFH
 **
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetAccessAddress(uint8_t CS, uint8_t Addr)
{
    volatile uint32_t *pAREAx;
    ASSERT(CS <= EXTB_CS_MAX);
//    ASSERT(Addr <= EXTB_AREA_ADDR_MAX);
    pAREAx = EXTBAREA_BASE_ADDR + CS;
    *pAREAx &= ~(0xFFul<<EXTB_AREAX_ADDR);
    *pAREAx |= (uint32_t)Addr<<EXTB_AREAX_ADDR;
    return;
}

#if (MCU_TYPE != DEVICE_TYPE0)
/*!
 ******************************************************************************
 ** \brief Set the assertion period for MALE, 1 ~ 16 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_ALEW_01C
 ** \arg EXTB_ALEW_02C
 ** \arg ...
 ** \arg EXTB_ALEW_15C
 ** \arg EXTB_ALEW_16C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetAddrLatchEnWidth(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pATIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_ATIM_ALEW_MAX);
    pATIMx = EXTBATIM_BASE_ADDR + CS;
    *pATIMx &= ~(16ul<<EXTB_ATIMX_ALEW);
    *pATIMx |= (uint32_t)Cycle<<EXTB_ATIMX_ALEW;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the setup cycle for ALE assertion, 0 ~ 15 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_ALES_00C
 ** \arg EXTB_ALES_01C
 ** \arg ...
 ** \arg EXTB_ALES_14C
 ** \arg EXTB_ALES_15C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetAddrLatchEnSetupCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pATIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_ATIM_ALES_MAX);
    pATIMx = EXTBATIM_BASE_ADDR + CS;
    *pATIMx &= ~(0x0Ful<<EXTB_ATIMX_ALES);
    *pATIMx |= (uint32_t)Cycle<<EXTB_ATIMX_ALES;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the address latch cycle, 1 ~ 16 cycles.
 **
 ** \param CS
 ** \arg EXTB_CS0
 ** \arg EXTB_CS1
 ** \arg ...
 ** \arg EXTB_CS6
 ** \arg EXTB_CS7
 **         
 ** \param Cycle
 ** \arg EXTB_ALC_01C
 ** \arg EXTB_ALC_02C
 ** \arg ...
 ** \arg EXTB_ALC_15C
 ** \arg EXTB_ALC_16C
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetAddrLatchCycle(uint8_t CS, uint8_t Cycle)
{
    volatile uint32_t *pATIMx;
    ASSERT(CS <= EXTB_CS_MAX);
    ASSERT(Cycle <= EXTB_ATIM_ALC_MAX);
    pATIMx = EXTBATIM_BASE_ADDR + CS;
    *pATIMx &= ~(0x0Ful<<EXTB_ATIMX_ALC);
    *pATIMx |= (uint32_t)Cycle<<EXTB_ATIMX_ALC;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable MCLK division.
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_EnableMCLKDivRatio(void)
{
    volatile uint32_t *pDCLKR;
    pDCLKR = EXTBDCLKR_BASE_ADDR;
    *pDCLKR |= 0x1ul<<EXTB_DCLKR_MCLKON;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable MCLK division, fix to Div 1
 **
 ** \param None
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_DisableMCLKDivRatio(void)
{
    volatile uint32_t *pDCLKR;
    pDCLKR = EXTBDCLKR_BASE_ADDR;
    *pDCLKR &= ~(0x1ul<<EXTB_DCLKR_MCLKON);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the division ratio of the division clock, 1 ~ 16 division.
 **
 ** \param Div
 ** \arg EXTB_MCLK_DIV01
 ** \arg EXTB_MCLK_DIV02
 ** \arg ...
 ** \arg EXTB_MCLK_DIV15
 ** \arg EXTB_MCLK_DIV16
 **         
 ** \return None
 **
 ******************************************************************************
 */
void EXTB_SetMCLKDivRatio(uint8_t Div)
{
    volatile uint32_t *pDCLKR;
    ASSERT(Div <= EXTB_DCLKR_MDIV_MAX);
    pDCLKR = EXTBDCLKR_BASE_ADDR;
    *pDCLKR &= ~(0x0Ful<<EXTB_DCLKR_MDIV);
    *pDCLKR |= Div<<EXTB_DCLKR_MDIV;
    return;
}

#endif

#endif

/* END OF FILE */
