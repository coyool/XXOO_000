/*!
 ******************************************************************************
 **
 ** \file bt_fm3.c
 **
 ** \brief Base timer module driver.
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.21
 **
 ** \date 2012-08-21
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ******************************************************************************
 */

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "bt_fm3.h"

#if (PERIPHERAL_ENABLE_BT == ON) && (PERIPHERAL_AVAILABLE_BT == ON)
/*---------------------------------------------------------------------------*/
/* local defines                                                             */
/*---------------------------------------------------------------------------*/
/*! \brief RT register base address */
#define  GetRTRegBaseAddr(Ch)      (FM3_BT0_RT_BASE + ((Ch)/4)*0x100 + (Ch)*0x40)
/*! \brief PWM register base address */
#define  GetPWMRegBaseAddr(Ch)     (FM3_BT0_PWM_BASE + ((Ch)/4)*0x100 + (Ch)*0x40)
/*! \brief PPG register base address */
#define  GetPPGRegBaseAddr(Ch)     (FM3_BT0_PPG_BASE + ((Ch)/4)*0x100 + (Ch)*0x40)
/*! \brief PWC register base address */
#define  GetPWCRegBaseAddr(Ch)     (FM3_BT0_PWC_BASE + ((Ch)/4)*0x100 + (Ch)*0x40)
/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*! \brief Pointer array of basetimer STC bit0 interrupt callback function */
static BT_RTTrigIntCallback *STCBit0IntCallback[BT_CH_NUM] = {(void*)0};
/*! \brief Pointer array of basetimer STC bit1 interrupt callback function */
static BT_RTTrigIntCallback *STCBit1IntCallback[BT_CH_NUM] = {(void*)0};
/*! \brief Pointer array of basetimer STC bit2 interrupt callback function */
static BT_RTTrigIntCallback *STCBit2IntCallback[BT_CH_NUM] = {(void*)0};

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief Set Base timer I/O Mode
 **
 ** \param Ch Requested channel
 ** \param IOMode requested I/O mode
 ******************************************************************************
 */
void BT_SetIOMode(uint8_t Ch, uint8_t IOMode)
{
    
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    ASSERT(IS_BT_IOMODE(IOMode));
    
    switch (Ch)
    {
        case BT_CH_0:
        case BT_CH_1:
            FM3_BTIOSEL03->BTSEL0123 = (FM3_BTIOSEL03->BTSEL0123&0xf0)|IOMode;
            break;
        case BT_CH_2:
        case BT_CH_3:
            FM3_BTIOSEL03->BTSEL0123 = (FM3_BTIOSEL03->BTSEL0123&0x0f)|(IOMode<<4);
            break;
        case BT_CH_4:
        case BT_CH_5:
            FM3_BTIOSEL47->BTSEL4567 = (FM3_BTIOSEL47->BTSEL4567&0xf0)|IOMode;
            break;
        case BT_CH_6:
        case BT_CH_7:
            FM3_BTIOSEL47->BTSEL4567 = (FM3_BTIOSEL47->BTSEL4567&0x0f)|(IOMode<<4);
            break;
        default:
            break;	            
	}
    return;
}

/*--------------------------- Reload Timer ----------------------------------*/
/*!
 ******************************************************************************
 ** \brief Reload timer initialization
 **
 ** \param Ch Requested channel
 ** \param pInit Initialization data pointor
 ******************************************************************************
 */
void BT_RTInit (uint8_t Ch, BT_RTInitRegT* pInit)
{
    FM3_BT_RT_TypeDef *rtreg0, *rtreg1;
    uint16_t tmcr;
    
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));    
    ASSERT(IS_BT_CLOCK(pInit->Clock)); 
    ASSERT(IS_BT_TRG_EDGE(pInit->InputEdge));
    ASSERT(IS_BT_BIT_TIMER(pInit->TimerMode));
    ASSERT(IS_BT_POLARITY(pInit->Polarity));
    ASSERT(IS_BT_MODE(pInit->Mode));  
      
    if (pInit->TimerMode == BT_32BIT_TIMER)
    {
        ASSERT((Ch % 2) == 0); /* Even channel */
        rtreg1 = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr((Ch+1));
    }
    
    /* Get RT register base address  */
    rtreg0 = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    
    
    /* Reset Timer Function */
    rtreg0->TMCR = 0;
    
    tmcr = ((pInit->Clock&0x07)<<12)|
            (pInit->InputEdge<<8)|
            (pInit->TimerMode<<7)|
            (pInit->Polarity<<3)|
            (pInit->Mode<<2)|
             BT_RT_MODE<<4;
    
    rtreg0->TMCR = tmcr;                            
    rtreg0->TMCR2 = (pInit->Clock&0x08)>>3;
    
    
    if (pInit->TimerMode == BT_16BIT_TIMER)
    {
        ASSERT((pInit->Cycle)<=0x0000ffff); //Only one register used for 16-bit timer
        rtreg0->PCSR = (uint16_t)pInit->Cycle;
    }
    else if (pInit->TimerMode == BT_32BIT_TIMER)
    {
        /* 
         * In 32-bit mode, for the reload timer, set the reload value of the 
         * upper 16 of 32 bits in the PWM Cycle Set Register of the odd channel. 
         * Then, set the reload value of the lower 16 bits in the PWM Cycle Set 
         * Register of the even channel.
         */
        rtreg1->PCSR = (uint16_t)(pInit->Cycle>>16);
        rtreg0->PCSR = (uint16_t)pInit->Cycle;
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable trigger interrupt of Reload Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback Interrupt callback function
 ******************************************************************************
 */
void BT_RTEnableTrigInt(uint8_t Ch, BT_RTTrigIntCallback* IntCallback)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    
    STCBit2IntCallback[Ch] = IntCallback;
    rtreg->STC |= (1ul<<6); 
    return; 
}

/*!
 ******************************************************************************
 ** \brief Disable trigger interrupt of Reload Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback Interrupt callback function
 ******************************************************************************
 */
void BT_RTDisableTrigInt(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    
    rtreg->STC &= ~(1ul<<6); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get trigger interrupt flag of Reload Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_RTGetTrigIntFlag(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    
    return((IntStatusT)((rtreg->STC & (1ul<<2))>>2));
}

/*!
 ******************************************************************************
 ** \brief Clear trigger interrupt flag of Reload Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_RTClrTrigIntFlag(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    rtreg->STC &= ~(1ul<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable underflow interrupt flag of Reload Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback interrupt callback function
 ******************************************************************************
 */
void BT_RTEnableUnderflowInt(uint8_t Ch, BT_RTUFIntCallback* IntCallback)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    
    STCBit0IntCallback[Ch] = IntCallback;
    rtreg->STC |= (1ul<<4); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable underflow interrupt flag of Reload Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_RTDisableUnderflowInt(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    rtreg->STC &= ~(1ul<<4); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get underflow interrupt flag of Reload Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_RTGetUnderflowIntFlag(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    return ((IntStatusT)(rtreg->STC & 1ul));
}

/*!
 ******************************************************************************
 ** \brief Clr underflow interrupt flag of Reload Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_RTClrUnderflowIntFlag(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    rtreg->STC &= ~1ul;
    return;
}

/*!
 ******************************************************************************
 ** \brief Start triggered by software
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_RTStartSoftTrig(uint8_t Ch)
{    
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch); 
    rtreg->TMCR |= 1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable Reload Timer count
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_RTEnableCount(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch); 
    rtreg->TMCR |= (1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable Reload Timer count
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_RTDisableCount(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch); 
    rtreg->TMCR &= ~(1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get count value of Reload timer in 16-bit timer mode.
 **
 ** \param Ch Requested channel
 ** \return Count value
 ******************************************************************************
 */
uint16_t BT_RTGet16bitCount(uint8_t Ch)
{
    FM3_BT_RT_TypeDef *rtreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch); 
    
    return (rtreg->TMR);
}

/*!
 ******************************************************************************
 ** \brief Get count value of Reload timer in 32-bit timer mode.
 **
 ** \param Ch Requested channel
 ** \return Count value
 ******************************************************************************
 */
uint32_t BT_RTGet32bitCount(uint8_t Ch)
{
    uint32_t data; 
    FM3_BT_RT_TypeDef *rtreg0, *rtreg1;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));    
    ASSERT((Ch % 2) == 0); /* Even channel */
    /* Get RT register base address  */
    rtreg0 = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch); 
    rtreg1 = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr((Ch+1)); 
    /* Get lower 16bit data */
    data = (uint32_t)rtreg0->TMR;     
    /* Get upper 16bit data */
    data |= (uint32_t)(rtreg1->TMR)<<16;
    return data;
}

/*--------------------------- PWM -------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief PWM timer initialization
 **
 ** \param Ch Requested channel
 ** \param pInit Initialization data pointor
 ******************************************************************************
 */
void BT_PWMInit (uint8_t Ch, BT_PWMInitRegT* pInit)
{
    uint16_t tmcr;   
    FM3_BT_PWM_TypeDef *pwmreg;
    
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));    
    ASSERT(IS_BT_CLOCK(pInit->Clock));    
    ASSERT(IS_BT_RESTART(pInit->Restart));
    ASSERT(IS_BT_OUTPUT_MASK(pInit->OutputMask));
    ASSERT(IS_BT_TRG_EDGE(pInit->InputEdge));
    ASSERT(IS_BT_POLARITY(pInit->Polarity));
    ASSERT(IS_BT_MODE(pInit->Mode));             
    
    /* Get RT register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch); 
    
    /* Reset Timer Function */
    pwmreg->TMCR = 0;
    
    tmcr = ((pInit->Clock&0x07)<<12)|
            (pInit->Restart<<11)|
            (pInit->OutputMask<<10)|
            (pInit->InputEdge<<8)|
            (pInit->Polarity<<3)|
            (pInit->Mode<<2)|
             BT_PWM_MODE<<4;
    
    pwmreg->TMCR = tmcr;                            
    pwmreg->TMCR2 = (pInit->Clock&0x08)>>3;
    pwmreg->PCSR = pInit->Cycle;
    pwmreg->PDUT = pInit->Duty;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable trigger interrupt of PWM timer
 **
 ** \param Ch BT channel
 ** \param IntCallback Interrupt callback function
 ******************************************************************************
 */
void BT_PWMEnableTrigInt(uint8_t Ch, BT_PWMTrigIntCallback* IntCallback)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    
    STCBit2IntCallback[Ch] = IntCallback;
    pwmreg->STC |= (1ul<<6); 
    return; 
}

/*!
 ******************************************************************************
 ** \brief Disable trigger interrupt of PWM Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback Interrupt callback function
 ******************************************************************************
 */
void BT_PWMDisableTrigInt(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    
    pwmreg->STC &= ~(1ul<<6); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get trigger interrupt flag of PWM Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_PWMGetTrigIntFlag(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    
    return((IntStatusT)((pwmreg->STC & (1ul<<2))>>2));
}

/*!
 ******************************************************************************
 ** \brief Clear trigger interrupt flag of PWM Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PWMClrTrigIntFlag(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    pwmreg->STC &= ~(1ul<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable underflow interrupt flag of PWM Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback interrupt callback function
 ******************************************************************************
 */
void BT_PWMEnableUnderflowInt(uint8_t Ch, BT_PWMUFIntCallback* IntCallback)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    
    STCBit0IntCallback[Ch] = IntCallback;
    pwmreg->STC |= (1ul<<4); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable underflow interrupt flag of PWM Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PWMDisableUnderflowInt(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    pwmreg->STC &= ~(1ul<<4); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get underflow interrupt flag of PWM Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_PWMGetUnderflowIntFlag(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    return ((IntStatusT)(pwmreg->STC & 1ul));
}

/*!
 ******************************************************************************
 ** \brief Clr underflow interrupt flag of PWM Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PWMClrUnderflowIntFlag(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    pwmreg->STC &= ~1ul;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable duty match flag of PWM Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback interrupt callback function
 ******************************************************************************
 */
void BT_PWMEnableDutyMatchInt(uint8_t Ch, BT_PWMUFIntCallback* IntCallback)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    
    STCBit1IntCallback[Ch] = IntCallback;
    pwmreg->STC |= (1ul<<5); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable duty match flag of PWM Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PWMDisableDutyMatchInt(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    pwmreg->STC &= ~(1ul<<5); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get duty match flag of PWM Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_PWMGetDutyMatchIntFlag(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    return ((IntStatusT)((pwmreg->STC & (1ul<<1))>>1));
}

/*!
 ******************************************************************************
 ** \brief Clr duty match flag of PWM Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PWMClrDutyMatchIntFlag(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch);
    pwmreg->STC &= ~(1ul<<1);
    return;
}
/*!
 ******************************************************************************
 ** \brief Start triggered by software
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_PWMStartSoftTrig(uint8_t Ch)
{    
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch); 
    pwmreg->TMCR |= 1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable PWM Timer count
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_PWMEnableCount(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch); 
    pwmreg->TMCR |= (1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable PWM Timer count
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_PWMDisableCount(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch); 
    pwmreg->TMCR &= ~(1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get count value of PWM timer in 16-bit timer mode.
 **
 ** \param Ch Requested channel
 ** \return Count value
 ******************************************************************************
 */
uint16_t BT_PWMGet16bitCount(uint8_t Ch)
{
    FM3_BT_PWM_TypeDef *pwmreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWM register base address  */
    pwmreg = (FM3_BT_PWM_TypeDef*)GetPWMRegBaseAddr(Ch); 
    
    return (pwmreg->TMR);
}

/*--------------------------- PPG -------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief PPG timer initialization
 **
 ** \param Ch Requested channel  
 ** \param pInit Initialization data pointor
 ******************************************************************************
 */
void BT_PPGInit (uint8_t Ch, BT_PPGInitRegT* pInit)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    uint16_t tmcr;    
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));    
    ASSERT(IS_BT_CLOCK(pInit->Clock));    
    ASSERT(IS_BT_RESTART(pInit->Restart));
    ASSERT(IS_BT_OUTPUT_MASK(pInit->OutputMask));
    ASSERT(IS_BT_TRG_EDGE(pInit->InputEdge));
    ASSERT(IS_BT_POLARITY(pInit->Polarity));
    ASSERT(IS_BT_MODE(pInit->Mode)); 
    
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch); 
    
    /* Reset Timer Function */
    ppgreg->TMCR = 0;
    
    tmcr = ((pInit->Clock&0x07)<<12)|
            (pInit->Restart<<11)|
            (pInit->OutputMask<<10)|
            (pInit->InputEdge<<8)|
            (pInit->Polarity<<3)|
            (pInit->Mode<<2)|
             BT_PPG_MODE<<4;
    
    ppgreg->TMCR = tmcr;                            
    ppgreg->TMCR2 = (pInit->Clock&0x08)>>3;
    ppgreg->PRLL = pInit->LowWidth;
    ppgreg->PRLH = pInit->HighWidth;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable trigger interrupt of PPG timer
 **
 ** \param Ch BT channel
 ** \param IntCallback Interrupt callback function
 ******************************************************************************
 */
void BT_PPGEnableTrigInt(uint8_t Ch, BT_PPGTrigIntCallback* IntCallback)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch);
    
    STCBit2IntCallback[Ch] = IntCallback;
    ppgreg->STC |= (1ul<<6); 
    return; 
}

/*!
 ******************************************************************************
 ** \brief Disable trigger interrupt of PPG Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback Interrupt callback function
 ******************************************************************************
 */
void BT_PPGDisableTrigInt(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch);
    
    ppgreg->STC &= ~(1ul<<6); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get trigger interrupt flag of PPG Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_PPGGetTrigIntFlag(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch);
    
    return((IntStatusT)((ppgreg->STC & (1ul<<2))>>2));
}

/*!
 ******************************************************************************
 ** \brief Clear trigger interrupt flag of PPG Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PPGClrTrigIntFlag(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch);
    ppgreg->STC &= ~(1ul<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable underflow interrupt flag of PPG Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback interrupt callback function
 ******************************************************************************
 */
void BT_PPGEnableUnderflowInt(uint8_t Ch, BT_PPGUFIntCallback* IntCallback)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch);
    
    STCBit0IntCallback[Ch] = IntCallback;
    ppgreg->STC |= (1ul<<4); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable underflow interrupt flag of PPG Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PPGDisableUnderflowInt(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch);
    ppgreg->STC &= ~(1ul<<4); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get underflow interrupt flag of PPG Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_PPGGetUnderflowIntFlag(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch);
    return ((IntStatusT)(ppgreg->STC & 1ul));
}

/*!
 ******************************************************************************
 ** \brief Clr underflow interrupt flag of PPG Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PPGClrUnderflowIntFlag(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch);
    ppgreg->STC &= ~1ul;
    return;
}

/*!
 ******************************************************************************
 ** \brief Start triggered by software
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_PPGStartSoftTrig(uint8_t Ch)
{    
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch); 
    ppgreg->TMCR |= 1;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable PPG Timer count
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_PPGEnableCount(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch); 
    ppgreg->TMCR |= (1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable PPG Timer count
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_PPGDisableCount(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch); 
    ppgreg->TMCR &= ~(1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get count value of PPG timer in 16-bit timer mode.
 **
 ** \param Ch Requested channel
 ** \return Count value
 ******************************************************************************
 */
uint16_t BT_PPGGet16bitCount(uint8_t Ch)
{
    FM3_BT_PPG_TypeDef *ppgreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PPG register base address  */
    ppgreg = (FM3_BT_PPG_TypeDef*)GetPPGRegBaseAddr(Ch); 
    
    return (ppgreg->TMR);
}

/*--------------------------- PWC -------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief PWC timer initialization
 **
 ** \param Ch Requested channel
 ** \param pInit Initialization data pointor
 ******************************************************************************
 */
void BT_PWCInit (uint8_t Ch, BT_PWCInitRegT* pInit)
{
    uint16_t tmcr;
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));    
    ASSERT(IS_BT_CLOCK(pInit->Clock));
    ASSERT(IS_BT_PWC_EDGE(pInit->MeasureEdge));
    ASSERT(IS_BT_BIT_TIMER(pInit->TimerMode));
    ASSERT(IS_BT_MODE(pInit->Mode)); 
    
    if (pInit->TimerMode == BT_32BIT_TIMER)
    {
        ASSERT((Ch % 2) == 0); /* Even channel */
    }
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch); 
    
    /* Reset Timer Function */
    pwcreg->TMCR = 0; 
      
    tmcr = ((pInit->Clock&0x07)<<12)|
            (pInit->MeasureEdge<<8)|
            (pInit->TimerMode<<7)|
            (pInit->Mode<<2)|
             BT_PWC_MODE<<4;
    
    pwcreg->TMCR = tmcr;                            
    pwcreg->TMCR2 = (pInit->Clock&0x08)>>3;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable overflow interrupt of PWC Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback Interrupt callback function
 ******************************************************************************
 */
void BT_PWCEnableOverflowInt(uint8_t Ch, BT_PWCOFIntCallback* IntCallback)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch);
    
    STCBit0IntCallback[Ch] = IntCallback;
    pwcreg->STC |= (1ul<<4); 
    return; 
}

/*!
 ******************************************************************************
 ** \brief Disable overflow interrupt of PWC Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback Interrupt callback function
 ******************************************************************************
 */
void BT_PWCDisableOverflowInt(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get RT register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch);
    
    pwcreg->STC &= ~(1ul<4); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get trigger interrupt flag of PWC Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_PWCGetOverflowIntFlag(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch);
    
    return((IntStatusT)(pwcreg->STC & 1ul));
}

/*!
 ******************************************************************************
 ** \brief Clear overflow interrupt flag of PWC Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PWCClrOverflowIntFlag(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch);
    pwcreg->STC &= ~(1ul);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable measure complete interrupt flag of PWC Timer
 **
 ** \param Ch BT channel
 ** \param IntCallback interrupt callback function
 ******************************************************************************
 */
void BT_PWCEnableMeasCmpInt(uint8_t Ch, BT_PWCMCIntCallback* IntCallback)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch);
    
    STCBit2IntCallback[Ch] = IntCallback;
    pwcreg->STC |= (1ul<<6); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable measure complete interrupt flag of PWC Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
void BT_PWCDisableMeasCmpInt(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch);
    pwcreg->STC &= ~(1ul<<6); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get  measure complete interrupt flag of PWC Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
IntStatusT BT_PWCGetMeasCmpIntFlag(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch);
    return (IntStatusT)((pwcreg->STC & (1ul<<2))>>2);
}

/*!
 ******************************************************************************
 ** \brief Get measure error status of PWC Timer
 **
 ** \param Ch BT channel
 ******************************************************************************
 */
FlagStatusT BT_PWCGetMeasErrStat(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch);
    return (IntStatusT)((pwcreg->STC & (1ul<<7))>>7);
}

/*!
 ******************************************************************************
 ** \brief Enable PWC Timer count
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_PWCEnableCount(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch); 
    pwcreg->TMCR |= (1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable PWC Timer count
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_PWCDisableCount(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch); 
    pwcreg->TMCR &= ~(1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get measure value of PWC timer in 16-bit timer mode.
 **
 ** \param Ch Requested channel
 ** \return Count value
 ******************************************************************************
 */
uint16_t BT_PWCGet16bitMeasData(uint8_t Ch)
{
    FM3_BT_PWC_TypeDef *pwcreg;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    /* Get PWC register base address  */
    pwcreg = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch); 
    
    return (pwcreg->DTBF);
}

/*!
 ******************************************************************************
 ** \brief Get measure value of PWC timer in 32-bit timer mode.
 **
 ** \param Ch Requested channel
 ** \return Count value
 ******************************************************************************
 */
uint16_t BT_PWCGet32bitMeasData(uint8_t Ch)
{
    uint32_t data; 
    FM3_BT_PWC_TypeDef *pwcreg0, *pwcreg1;
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));    
    ASSERT((Ch % 2) == 0); /* Even channel */
    /* Get RT register base address  */
    pwcreg0 = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr(Ch); 
    pwcreg1 = (FM3_BT_PWC_TypeDef*)GetPWCRegBaseAddr((Ch+1)); 
    /* Get lower 16bit data */
    data = (uint32_t)pwcreg0->DTBF;     
    /* Get upper 16bit data */
    data |= (uint32_t)(pwcreg1->DTBF)<<16;
    return data;
}

/*!
 ******************************************************************************
 ** \brief Base timer Interrupt Handler routine
 **
 ** \param Ch Requested channel
 ******************************************************************************
 */
void BT_IntHandler(uint8_t Ch, uint8_t IntType)
{
    FM3_BT_RT_TypeDef *rtreg;
    uint8_t irq;
    
    /* Check parameters */
    ASSERT(IS_BT_CH(Ch));
    
    rtreg = (FM3_BT_RT_TypeDef*)GetRTRegBaseAddr(Ch);
    
    /* Check Interrupt Request */
    irq = rtreg->STC; 
    
    /* 
     * Trigger Interrupt (PWM,PPG,Reload timer)
     * Measurement completion Interrupt (PWC)
     */
    if ((irq & (1ul<<2)) == (1ul<<2)) 
    {
        /* 
         * Clear Interrupt.   
         */ 
        rtreg->STC &= ~(1ul<<2); 

        if (STCBit2IntCallback[Ch] != NULL) 
        {
            /* Call CallBackIrq */
            STCBit2IntCallback[Ch]();
        }
    }
    
    /* Duty Match Interrupt (PWM)*/   
    if ((irq & (1ul<<1)) == (1ul<<1)) 
    {
        /* 
         * Clear Interrupt.   
         */ 
        rtreg->STC &= ~(1ul<<1); 

        if (STCBit1IntCallback[Ch] != NULL) 
        {
            /* Call CallBackIrq */
            STCBit1IntCallback[Ch]();
        }
    }
  
    /* 
     * Underflow Interrupt (PWM,PPG,Reload timer)
     * Overflow Interrupt (PWC) 
     */
    if ((irq & 1ul) == 1ul) 
    {
        /* Clear Interrupt */
        rtreg->STC &= ~1ul;

        if (STCBit0IntCallback[Ch] != NULL) 
        {
            /* Call CallBackIrq */
            STCBit0IntCallback[Ch]();
        }
    }   

}

#endif

/*****************************************************************************/
/* END OF FILE */
