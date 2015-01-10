/*!
 ******************************************************************************
 **
 ** \file dac_fm3.c	
 **
 ** \brief dac drivers
 **
 ** \author FSDC AE Team
 **
 ** \version V0.1
 **
 ** \date 2012-10-12
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(ChengDu) Co.,Ltd.
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
#include"dac10_fm3.h"

#if (PERIPHERAL_AVAILABLE_DAC == OFF)
    #if (PERIPHERAL_ENABLE_DAC == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_AVAILABLE_DAC == ON)  && (PERIPHERAL_ENABLE_DAC == ON)

/*!
 ******************************************************************************
 ** \brief DAC Enable
 **
 ** \param Ch
 ** \arg DAC10_CH0
 ** \arg DAC10_CH1
 **
 ** \return None
 **
 ******************************************************************************
 */
void DAC_Enable(uint8_t Ch)
{
    ASSERT(IS_DAC_CH_VALID(Ch));
    /* enable DAC function by DAE bit */
    if(Ch == DAC10_CH0)    //channel 0
        FM3_DAC->DACR0 |= 1ul;     
    else                   //channel 1
        FM3_DAC->DACR1 |= 1ul;     
    
    return;
}

/*!
 ******************************************************************************
 ** \brief DAC Disable
 **
 ** \param Ch
 ** \arg DAC10_CH0
 ** \arg DAC10_CH1
 **
 ** \return None
 **
 ******************************************************************************
 */
void DAC_Disable(uint8_t Ch)
{
    ASSERT(IS_DAC_CH_VALID(Ch));
    /* disable DAC function by clear DAE bit */
    if(Ch == DAC10_CH0)       //channel 0
        FM3_DAC->DACR0 &= ~(1ul);     
    else                      //channel 1
        FM3_DAC->DACR1 &= ~(1ul);   
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the digital value of DAC
 **
 ** \param Ch
 ** \arg DAC10_CH0
 ** \arg DAC10_CH1
 ** \param ConvertValue
 ** \arg 0~1023
 **
 ** \return None
 **
 ******************************************************************************
 */
void DAC_SetDigitalVal(uint8_t Ch, uint16_t ConvertValue)
{
    ASSERT(IS_DAC_CH_VALID(Ch));
    /* set DAC input digital value */
    if(Ch == DAC10_CH0)         //channel0
        FM3_DAC->DADR0 = ConvertValue;   
    else                        //channel1
        FM3_DAC->DADR1 = ConvertValue;  
    
    return;
}

#endif

/*****************************************************************************/

/* END OF FILE */
