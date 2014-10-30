/*!
 ******************************************************************************
 **
 ** \file clock_fm3.c
 **
 ** \brief Clock module driver.
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
 ** \note History: \n
 ** V0.1 2011-12-30 original version \n
 **
 ******************************************************************************
 */
 

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "clock_fm3.h"

#if (PERIPHERAL_ENABLE_CLOCK == ON) && (PERIPHERAL_AVAILABLE_CLOCK == ON)
/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Switch to main clock
 **
 ** \param SubOscMode Sub oscillator mode
 ** \arg CLOCK_SUB_OSC_OFF
 ** \arg CLOCK_SUB_OSC_ON
 ** \param MainPLLMode Main PLL mode
 ** \arg CLOCK_MAINPLL_OFF
 ** \arg CLOCK_MAINPLL_ON
 ** \param Core clock divider factor
 ** \arg BaseSysetmClockDIV1
 ** \arg BaseSysetmClockDIV2
 ** \arg BaseSysetmClockDIV3
 ** \arg BaseSysetmClockDIV4
 ** \arg BaseSysetmClockDIV6
 ** \arg BaseSysetmClockDIV8
 ** \arg BaseSysetmClockDIV16
 ** 
 ** \return None
 ******************************************************************************
 */
void SwitchToMainClk(uint8_t SubOscMode, uint8_t MainPLLMode, uint8_t BaseSysetmClockDIV)
{
    uint32_t u32IoRegisterRead;     
    /* Switch master clock to High-CR firstly */
    FM3_CRG->SCM_CTL &= ~0xE0;      
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    FM3_CRG->SCM_CTL |= (1UL<<1);               /* enable main oscillator */ 
    while (!(FM3_CRG->SCM_STR&(1UL<<1)));       /* wait for Main clock oscillation stable */
    
    /* Switch master clock to Main Osc */
    FM3_CRG->SCM_CTL  |= (CLOCK_MODE_MAIN_OSC<<5);
    
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    if(SubOscMode == CLOCK_SUB_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_SOSCE = 1;
        while(!bFM3_CRG_SCM_STR_SORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_SOSCE = 0;
    }
    
    if(MainPLLMode == CLOCK_MAINPLL_ON)
    {
        bFM3_CRG_SCM_CTL_PLLE = 1;
        while(!bFM3_CRG_SCM_STR_PLRDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_PLLE = 0;
    }
    
    FM3_CRG->BSC_PSR = BaseSysetmClockDIV;
    
    SystemCoreClockUpdate();
    
    return;
}

/*!
 ******************************************************************************
 ** \brief Switch to main PLL clock
 **
 ** \param SubOscMode Sub oscillator mode
 ** \arg CLOCK_SUB_OSC_OFF
 ** \arg CLOCK_SUB_OSC_ON
 ** \param Core clock divider factor
 ** \arg BaseSysetmClockDIV1
 ** \arg BaseSysetmClockDIV2
 ** \arg BaseSysetmClockDIV3
 ** \arg BaseSysetmClockDIV4
 ** \arg BaseSysetmClockDIV6
 ** \arg BaseSysetmClockDIV8
 ** \arg BaseSysetmClockDIV16
 ** \param PLL Frequency multiplier
 ** \arg PLLMultiplier1
 ** \arg PLLMultiplier2
 ** \arg ... 
 ** \arg PLLMultiplier49
 ** \arg PLLMultiplier50
 **
 ** \return None
 ******************************************************************************
 */
void SwitchToMainPLL(uint8_t SubOscMode, uint8_t BaseSysetmClockDIV ,uint8_t PLLMultiplier)
{
    uint32_t u32IoRegisterRead;    
    /* Switch master clock to High-CR firstly */
    FM3_CRG->SCM_CTL &= ~0xE0;      
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    FM3_CRG->SCM_CTL |= (1UL<<1);               /* enable main oscillator */ 
    while (!(FM3_CRG->SCM_STR&(1UL<<1)));       /* wait for Main clock oscillation stable */
    
    FM3_CRG->PLL_CTL2  = PLLMultiplier;         /* set PLLN */
    bFM3_CRG_SCM_CTL_PLLE = 1;                 /* PLL on */
    while(!bFM3_CRG_SCM_STR_PLRDY);            /* wait for PLL clock oscillation stable */
    
    /* Switch master clock to Main PLL */
    FM3_CRG->SCM_CTL  |= (CLOCK_MODE_MAIN_PLL<<5);
    
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    if(SubOscMode == CLOCK_SUB_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_SOSCE = 1;
        while(!bFM3_CRG_SCM_STR_SORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_SOSCE = 0;
    }
    
    FM3_CRG->BSC_PSR = BaseSysetmClockDIV;
    
    SystemCoreClockUpdate();
    
    return;
}

/*!
 ******************************************************************************
 ** \brief Switch to High-Speed CR PLL clock
 **
 ** \param MainOscMode Main oscillator mode
 ** \arg CLOCK_MAIN_OSC_OFF
 ** \arg CLOCK_MAIN_OSC_ON
 ** \param SubOscMode Sub oscillator mode
 ** \arg CLOCK_SUB_OSC_OFF
 ** \arg CLOCK_SUB_OSC_ON
 ** \param Core clock divider factor
 ** \arg BaseSysetmClockDIV1
 ** \arg BaseSysetmClockDIV2
 ** \arg BaseSysetmClockDIV3
 ** \arg BaseSysetmClockDIV4
 ** \arg BaseSysetmClockDIV6
 ** \arg BaseSysetmClockDIV8
 ** \arg BaseSysetmClockDIV16
 ** \param PLL Frequency multiplier
 ** \arg PLLMultiplier1
 ** \arg PLLMultiplier2
 ** \arg ... 
 ** \arg PLLMultiplier49
 ** \arg PLLMultiplier50
 **
 ** \return None
 ******************************************************************************
 */
void SwitchToHighCRPLL(uint8_t MainOscMode, uint8_t SubOscMode, uint8_t BaseSysetmClockDIV ,uint8_t PLLMultiplier)
{
    uint32_t u32IoRegisterRead;    
    /* Switch master clock to High-CR firstly */
    FM3_CRG->SCM_CTL &= ~0xE0;      
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    bFM3_CRG_PSW_TMR_PINC = 1;                 /* Select high CR as PLL input  */
    FM3_CRG->PLL_CTL2  = PLLMultiplier;        /* set PLLN */
    bFM3_CRG_SCM_CTL_PLLE = 1;                 /* PLL on */
    while(!bFM3_CRG_SCM_STR_PLRDY);            /* wait for PLL clock oscillation stable */
    
    /* Switch master clock to Main PLL */
    FM3_CRG->SCM_CTL  |= (CLOCK_MODE_MAIN_PLL<<5);
    
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    if(MainOscMode == CLOCK_MAIN_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_MOSCE = 1;
        while(!bFM3_CRG_SCM_STR_MORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_MOSCE = 0;
    }
    
    if(SubOscMode == CLOCK_SUB_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_SOSCE = 1;
        while(!bFM3_CRG_SCM_STR_SORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_SOSCE = 0;
    }
    
    FM3_CRG->BSC_PSR = BaseSysetmClockDIV;
    
    SystemCoreClockUpdate();
    
    return;
}

/*!
 ******************************************************************************
 ** \brief Switch to sub clock
 **
 ** \param MainPLLMode Main PLL mode
 ** \arg CLOCK_MAINPLL_OFF
 ** \arg CLOCK_MAINPLL_ON
 ** \param MainOscMode Main clock mode
 ** \arg CLOCK_MAIN_OSC_OFF
 ** \arg CLOCK_MAIN_OSC_ON
 ** \param Core clock divider factor
 ** \arg BaseSysetmClockDIV1
 ** \arg BaseSysetmClockDIV2
 ** \arg BaseSysetmClockDIV3
 ** \arg BaseSysetmClockDIV4
 ** \arg BaseSysetmClockDIV6
 ** \arg BaseSysetmClockDIV8
 ** \arg BaseSysetmClockDIV16
 ** 
 ** \return None
 ******************************************************************************
 */
void SwitchToSubClk(uint8_t MainPLLMode, uint8_t MainOscMode, uint8_t BaseSysetmClockDIV)
{
    uint32_t u32IoRegisterRead;    
    /* Switch master clock to High-CR firstly */
    FM3_CRG->SCM_CTL &= ~0xE0;      
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    FM3_CRG->SCM_CTL |= (1UL<<1);               /* enable main oscillator */ 
    while (!(FM3_CRG->SCM_STR&(1UL<<1)));       /* wait for Main clock oscillation stable */
    
    /* Switch master clock to sub clock */
    FM3_CRG->SCM_CTL  |= (CLOCK_MODE_SUB_OSC<<5);
    
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    if(MainOscMode == CLOCK_MAIN_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_MOSCE = 1;
        while(!bFM3_CRG_SCM_STR_MORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_MOSCE = 0;
    }
    
    if(MainPLLMode == CLOCK_MAINPLL_ON)
    {
        bFM3_CRG_SCM_CTL_PLLE = 1;
        while(!bFM3_CRG_SCM_STR_PLRDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_PLLE = 0;
    }
    
    FM3_CRG->BSC_PSR = BaseSysetmClockDIV;
    
    SystemCoreClockUpdate();
    
    return;
}


/*!
 ******************************************************************************
 ** \brief Switch to high speed CR
 **
 ** \param MainPLLMode Main PLL mode
 ** \arg CLOCK_MAINPLL_OFF
 ** \arg CLOCK_MAINPLL_ON
 ** \param MainOscMode Main clock mode
 ** \arg CLOCK_MAIN_OSC_OFF
 ** \arg CLOCK_MAIN_OSC_ON
 ** \param SubOscMode Sub clock mode
 ** \arg CLOCK_SUB_OSC_OFF
 ** \arg CLOCK_SUB_OSC_ON
 ** \param Core clock divider factor
 ** \arg BaseSysetmClockDIV1
 ** \arg BaseSysetmClockDIV2
 ** \arg BaseSysetmClockDIV3
 ** \arg BaseSysetmClockDIV4
 ** \arg BaseSysetmClockDIV6
 ** \arg BaseSysetmClockDIV8
 ** \arg BaseSysetmClockDIV16
 ** 
 ** \return None
 ******************************************************************************
 */
void SwitchToHighCR(uint8_t MainPLLMode, uint8_t MainOscMode, 
                    uint8_t SubOscMode, uint8_t BaseSysetmClockDIV)
{
    uint32_t u32IoRegisterRead;   
    /* Switch master clock to High-CR firstly */
    FM3_CRG->SCM_CTL &= ~0xE0;      
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    if(MainOscMode == CLOCK_MAIN_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_MOSCE = 1;
        while(!bFM3_CRG_SCM_STR_MORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_MOSCE = 0;
    }
    
    if(MainPLLMode == CLOCK_MAINPLL_ON)
    {
        bFM3_CRG_SCM_CTL_PLLE = 1;
        while(!bFM3_CRG_SCM_STR_PLRDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_PLLE = 0;
    }
    
    if(SubOscMode == CLOCK_SUB_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_SOSCE = 1;
        while(!bFM3_CRG_SCM_STR_SORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_SOSCE = 0;
    }
    
    FM3_CRG->BSC_PSR = BaseSysetmClockDIV;
    
    SystemCoreClockUpdate();
    
    return;
}

/*!
 ******************************************************************************
 ** \brief Switch to low speed CR
 **
 ** \param MainPLLMode Main PLL mode
 ** \arg CLOCK_MAINPLL_OFF
 ** \arg CLOCK_MAINPLL_ON
 ** \param MainOscMode Main clock mode
 ** \arg CLOCK_MAIN_OSC_OFF
 ** \arg CLOCK_MAIN_OSC_ON
 ** \param SubOscMode Sub clock mode
 ** \arg CLOCK_SUB_OSC_OFF
 ** \arg CLOCK_SUB_OSC_ON
 ** \param Core clock divider factor
 ** \arg BaseSysetmClockDIV1
 ** \arg BaseSysetmClockDIV2
 ** \arg BaseSysetmClockDIV3
 ** \arg BaseSysetmClockDIV4
 ** \arg BaseSysetmClockDIV6
 ** \arg BaseSysetmClockDIV8
 ** \arg BaseSysetmClockDIV16
 ** 
 ** \return None
 ******************************************************************************
 */
void SwitchToLowCR(uint8_t MainPLLMode, uint8_t MainOscMode, 
                   uint8_t SubOscMode, uint8_t BaseSysetmClockDIV)
{
    uint32_t u32IoRegisterRead;     
    /* Switch master clock to High-CR firstly */
    FM3_CRG->SCM_CTL &= ~0xE0;      /* Switch master clock to High-CR firstly */
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    /* Switch master clock to Low-CR mode */
    FM3_CRG->SCM_CTL |= (CLOCK_MODE_LOW_CR<<5);      /* Switch master clock to Low-CR*/
    
    do                                              
    {                                               
        u32IoRegisterRead = (FM3_CRG->SCM_CTL&0xE0); 
    }while ((FM3_CRG->SCM_STR&0xE0) != u32IoRegisterRead); /* wait for clock stability */
    
    if(MainOscMode == CLOCK_MAIN_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_MOSCE = 1;
        while(!bFM3_CRG_SCM_STR_MORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_MOSCE = 0;
    }
    
    if(MainPLLMode == CLOCK_MAINPLL_ON)
    {
        bFM3_CRG_SCM_CTL_PLLE = 1;
        while(!bFM3_CRG_SCM_STR_PLRDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_PLLE = 0;
    }
    
    if(SubOscMode == CLOCK_SUB_OSC_ON)
    {
        bFM3_CRG_SCM_CTL_SOSCE = 1;
        while(!bFM3_CRG_SCM_STR_SORDY);
    }
    else
    {
        bFM3_CRG_SCM_CTL_SOSCE = 0;
    }
    
    FM3_CRG->BSC_PSR = BaseSysetmClockDIV;
    
    SystemCoreClockUpdate();
    
    return;
}

#endif

/*****************************************************************************/
/* END OF FILE */
