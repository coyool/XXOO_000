/*!
 ******************************************************************************
 **
 ** \file dt_fm3.c
 **
 ** \brief Dual timer driver
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-08
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
#include "dt_fm3.h"

#if (PERIPHERAL_AVAILABLE_DT ==ON) && (PERIPHERAL_ENABLE_DT == ON)

/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
static DT_RegInfoT *ptDT_RegInfo[2] = 
{
    (DT_RegInfoT *)(FM3_DTIM_BASE),                         //Ch 1 address
    (DT_RegInfoT *)(FM3_DTIM_BASE + sizeof(DT_RegInfoT))    //Ch 2 address
};

static DT_CallbackT g_pDTIRQCallback[2] = 
{ 
    NULL,
    NULL,
};
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
 ** \brief Clear the interrupt source
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **         
 ** \return None
 **
 ******************************************************************************
 */
void DT_IntClrInt (uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    ptDT_RegInfo[Ch-1]->Timerintclr = 0xFFFFFFFF;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable dt function
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_ConfigEnableTimer (uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    ptDT_RegInfo[Ch-1]->Timercontrol = ptDT_RegInfo[Ch-1]->Timercontrol 
                                        | (DT_CTL_TIMEREN);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable dt function
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_ConfigDisableTimer (uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    ptDT_RegInfo[Ch-1]->Timercontrol = ptDT_RegInfo[Ch-1]->Timercontrol 
                                        & (~DT_CTL_TIMEREN);
    return;
}
/*!
 ******************************************************************************
 ** \brief Enable dt int
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_IntEnableInt (uint8_t Ch, void(*TXFunc)())
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    ASSERT(TXFunc != NULL);

    g_pDTIRQCallback[Ch-1] = TXFunc;
    ptDT_RegInfo[Ch-1]->Timercontrol = ptDT_RegInfo[Ch-1]->Timercontrol 
                                        | (DT_CTL_TIMERINT);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable dt int
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_IntDisableInt (uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    g_pDTIRQCallback[Ch-1] = NULL;
    ptDT_RegInfo[Ch-1]->Timercontrol = ptDT_RegInfo[Ch-1]->Timercontrol 
                                        & (~DT_CTL_TIMERINT);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set load register
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **         
 ** \param value
 ** \arg   16 bit/32 bit data
 **         
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_ConfigSetLoadValue (uint8_t Ch, uint32_t value)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    /*Set the load value*/    
    ptDT_RegInfo[Ch-1]->Timerload = value;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set back load register
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **         
 ** \param value
 ** \arg   16 bit/32 bit data
 **         
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_ConfigSetBackLoadValue (uint8_t Ch, uint32_t value)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    /*Set the BG load value*/    
    ptDT_RegInfo[Ch-1]->TimerBGload = value;
    return;
}
/*!
 ******************************************************************************
 ** \brief Set count size of dt
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **         
 ** \param value count size: 16 bit/32 bit
 ** \arg   DT_CNT_SIZE_16
 ** \arg   DT_CNT_SIZE_32
 **         
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_ConfigSetTimerSize (uint8_t Ch, uint8_t size)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    ASSERT((DT_CNT_SIZE_16 == size) || (DT_CNT_SIZE_32 == size));
    /*Set the load value*/
    if(DT_CNT_SIZE_16 == size)
    {
        ptDT_RegInfo[Ch-1]->Timercontrol = ptDT_RegInfo[Ch-1]->Timercontrol 
                                            & (~DT_CTL_TIMERSIZE);
    }
    else
    {
        ptDT_RegInfo[Ch-1]->Timercontrol = ptDT_RegInfo[Ch-1]->Timercontrol 
                                            | (DT_CTL_TIMERSIZE);
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief DT mode config
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **         
 ** \param mode mode type
 ** \arg   DT_MODE_FREERUN
 ** \arg   DT_MODE_PERIOD
 ** \arg   DT_MODE_ONESHOT
 **         
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_ConfigSetTimerMode (uint8_t Ch, uint8_t mode)
{
    uint32_t tControl;
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    ASSERT((DT_MODE_FREERUN == mode) 
           || (DT_MODE_PERIOD == mode) 
           || (DT_MODE_ONESHOT == mode));
    /*Config the mode*/
    tControl = ptDT_RegInfo[Ch-1]->Timercontrol;
    switch(mode)
    {
        case DT_MODE_FREERUN:
            tControl = tControl & (~DT_CTL_TIMERMODE);
            tControl = tControl & (~DT_CTL_ONESHOT);
            break;
        case DT_MODE_PERIOD:
            tControl = tControl | (DT_CTL_TIMERMODE);
            tControl = tControl & (~DT_CTL_ONESHOT);
            break;
        case DT_MODE_ONESHOT:
            tControl = tControl | (DT_CTL_ONESHOT);
            break;
        default:
            break;
    }
    ptDT_RegInfo[Ch-1]->Timercontrol = tControl;
    return;
}

/*!
 ******************************************************************************
 ** \brief Set prescaler of dt
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **         
 ** \param prescaler scaler
 ** \arg   DT_PRE_BY_1
 ** \arg   DT_PRE_BY_16
 ** \arg   DT_PRE_BY_256
 **         
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_ConfigSetTimerPre (uint8_t Ch, uint8_t prescaler)
{
    uint32_t tControl;
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    ASSERT((DT_PRE_BY_1 == prescaler) 
           || (DT_PRE_BY_16 == prescaler) 
           || (DT_PRE_BY_256 == prescaler));
    /*Set the prescaler*/
    tControl = ptDT_RegInfo[Ch-1]->Timercontrol;
    tControl = tControl & (~(DT_CTL_TIMERPRE16|DT_CTL_TIMERPRE256));
    switch(prescaler)
    {
        case DT_PRE_BY_1:
            break;
        case DT_PRE_BY_16:
            tControl = tControl | (DT_CTL_TIMERPRE16);
            break;
        case DT_PRE_BY_256:
            tControl = tControl | (DT_CTL_TIMERPRE256);
            break;
        default:
            break;
    }
    ptDT_RegInfo[Ch-1]->Timercontrol = tControl;
    return;
}

/*!
 ******************************************************************************
 ** \brief Dual timer control configuration
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **         
 ** \param dt_configT mode information
 ** \arg   DT_ConfigT
 **         
 **
 ** \return None
 **
 ******************************************************************************
 */
void DT_ConfigMode (uint8_t Ch, DT_ConfigT *ptdt_config)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));
    /* Stop and Clear Interrupt */
    DT_IntDisableInt(Ch);
    DT_IntClrInt(Ch);
    /* Set TimerXControl register */
    /* TimerEn -> Disable */
    DT_ConfigDisableTimer(Ch);
    /* TimerMode */
    DT_ConfigSetTimerMode(Ch, ptdt_config->TimerMode);
    /* Timer count size */
    DT_ConfigSetTimerSize(Ch, ptdt_config->TimerSize);
    /* Timer prescaler */
    DT_ConfigSetTimerPre(Ch, ptdt_config->TimerPre);
    return;
}

/*!
 ******************************************************************************
 ** \brief Read Dual timer current count value
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return Current value
 ** \retval 32 bit Value data
 **
 ******************************************************************************
 */
uint32_t DT_StatusReadValue (uint8_t Ch)
{
    uint32_t ttimervalue;
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));

    ttimervalue = ptDT_RegInfo[Ch-1]->Timervalue;
    /* 32 bit or 16 bit*/
    if((ptDT_RegInfo[Ch-1]->Timercontrol & DT_CTL_TIMERSIZE) != 0)
    {
        /* 32 bit */
    }
    else
    {
      	/* 16 bit */
      	ttimervalue = ttimervalue & 0x0000FFFF;
    }
    return(ttimervalue);
}

/*!
 ******************************************************************************
 ** \brief Read Dual timer current RIS
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return RIS
 ** \retval 0/1
 **
 ******************************************************************************
 */
uint32_t DT_StatusReadRIS (uint8_t Ch)
{
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));

    return(ptDT_RegInfo[Ch-1]->TimerRIS);
}

/*!
 ******************************************************************************
 ** \brief Read Dual timer current timer mode
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return Timer mode
 ** \retval DT_MODE_FREERUN
 ** \retval DT_MODE_PERIOD
 ** \retval DT_MODE_ONESHOT
 **
 ******************************************************************************
 */
uint8_t DT_StatusReadTimerMode (uint8_t Ch)
{
    uint32_t ttimercontrol;
    uint8_t ttimermode;
    
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));

    ttimercontrol = ptDT_RegInfo[Ch-1]->Timercontrol;
    if((ttimercontrol & DT_CTL_ONESHOT) != 0)/* Is it one shot mode? */
    {
      	ttimermode = DT_MODE_ONESHOT;
    }
    else if((ttimercontrol & DT_CTL_TIMERMODE) != 0)/* Is it periodic mode? */
        {
          	ttimermode = DT_MODE_PERIOD;
        }
        else
        {
          	ttimermode = DT_MODE_FREERUN;
        }
    return(ttimermode);
}

/*!
 ******************************************************************************
 ** \brief Read Dual timer current prescaler
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return Prescaler
 ** \retval DT_PRE_BY_1
 ** \retval DT_PRE_BY_16
 ** \retval DT_PRE_BY_256
 **
 ******************************************************************************
 */
uint8_t DT_StatusReadTimerPrescaler (uint8_t Ch)
{
    uint32_t ttimercontrol;
    uint8_t ttimerpre;
    
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));

    ttimercontrol = ptDT_RegInfo[Ch-1]->Timercontrol;
    ttimercontrol = ttimercontrol & (DT_CTL_TIMERPRE16 | DT_CTL_TIMERPRE256);
    ttimerpre = (uint8_t)(ttimercontrol >> 2);
    return(ttimerpre);
}

/*!
 ******************************************************************************
 ** \brief Read Dual timer current timer size
 **
 ** \param Ch Requested channel
 ** \arg   DT_CH_1
 ** \arg   DT_CH_2
 **
 ** \return Timer size
 ** \retval DT_CNT_SIZE_16
 ** \retval DT_CNT_SIZE_32
 **
 ******************************************************************************
 */
uint8_t DT_StatusReadTimerSize (uint8_t Ch)
{
    uint32_t ttimercontrol;
    uint8_t ttimersize;
    
    /* Check Cfg parameter */
    ASSERT((DT_CH_1 == Ch) || (DT_CH_2 == Ch));

    ttimercontrol = ptDT_RegInfo[Ch-1]->Timercontrol;
    ttimercontrol = ttimercontrol & DT_CTL_TIMERSIZE;
    ttimersize = (uint8_t)(ttimercontrol >> 1);
    return(ttimersize);
}
/*!
 ******************************************************************************
 ** \brief This function handles SystemTick Handler.
 **
 ** \param None        
 **
 ** \retval None
 **
 ******************************************************************************
 */
void DT_IntHandler(uint8_t Ch)
{   
    if(DT_StatusReadRIS(Ch) == SET)
    {
        DT_IntClrInt(Ch);
        if(g_pDTIRQCallback[Ch-1] != NULL)
        {
            g_pDTIRQCallback[Ch-1]();
        }
    }
}

#endif

/*****************************************************************************/

/* END OF FILE */
