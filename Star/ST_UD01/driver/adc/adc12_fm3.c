/*!
 ******************************************************************************
 **
 ** \file adc12_fm3.c
 **
 ** \brief this file provides adc 12-bit drivers.
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.10   
 **
 ** \date 2011-12-14
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
#include "adc12_fm3.h"

#if (PERIPHERAL_ENABLE_ADC == ON) && (PERIPHERAL_AVAILABLE_ADC == ON)
/*---------------------------------------------------------------------------*/
/* Bit definition                                                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
/*! \brief ADC register array  */ 
static FM3_ADC_TypeDef* ADC12Reg[] = 
{
    (FM3_ADC_TypeDef*)(FM3_ADC0),
    (FM3_ADC_TypeDef*)(FM3_ADC1),
    (FM3_ADC_TypeDef*)(FM3_ADC2),
};
/*! \brief FIFO OVE callback function array  */ 
static ADC_FIFOOVECallbackT* FIFOOVECallback[3] = {(void*)0};
/*! \brief Scan interrupt callback function array  */ 
static ADC_ScanCallbackT* ScanCallback[3] = {(void*)0};
/*! \brief Priority interrupt callback function array  */ 
static ADC_PrioCallbackT* PrioCallback[3] = {(void*)0};
/*! \brief Comparison interrupt callback function array  */ 
static ADC_CmpCallbackT* CmpCallback[3] = {(void*)0};

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*------------------- ADC global setting ------------------------------------*/

/*!
 ******************************************************************************
 ** \brief Force stop ADC
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 **
 ** \Return None
 **
 ******************************************************************************
 */
void ADC12_ForceStop(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*forbid conversion*/
    ADC12Reg[Unit] ->ADSR |= (1ul<<7);
    return;
}

/*!
 ******************************************************************************
 ** \brief Select FIFO data location
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 ** \param Loc Data location
 ** \arg ADC12_FIFO_DATA_MSB_ALIGN
 ** \arg ADC12_FIFO_DATA_LSB_ALIGN
 **
 ** \Return None
 **
 ******************************************************************************
 */
void ADC12_SelFIFODataLoc(uint8_t Unit, ADC12_FIFODataAlignT Loc)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_FIFO_DATA_VALID(Loc));
    
    /* data location setting */
    ADC12Reg[Unit] ->ADSR &= ~ (1ul<<6);
    ADC12Reg[Unit] ->ADSR |= (Loc<<6);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get the priority pending flag of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 **
 ** \return Priority pending flag
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetPrioPendingFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    return (FlagStatusT) ((ADC12Reg[Unit] ->ADSR & (1ul<<2))>>2);
}

/*!
 ******************************************************************************
 ** \brief Get the priority status flag of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 **
 ** \return Priority status flag
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetPrioStatusFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));;
    return (FlagStatusT) ((ADC12Reg[Unit] ->ADSR & (1ul<<1))>>1);
}

/*!
 ******************************************************************************
 ** \brief Get the scan status flag of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 **
 ** \return Scan status flag
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetScanStatusFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    return (FlagStatusT) (ADC12Reg[Unit] ->ADSR & (1ul));
}

/*!
 ******************************************************************************
 ** \brief Select the register of sample time setting of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 ** \param RegSel Register selection
 ** \arg   ADC12_SAMPLE_TIME_SEL_REG0
 ** \arg   ADC12_SAMPLE_TIME_SEL_REG1
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SelSampleTimeReg(uint8_t Unit, uint8_t Ch, ADC12_STSelRegT RegSel)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_CH_VALID(Ch));
    ASSERT(IS_ADC_ST_SEL_VALID(RegSel));

    ADC12Reg[Unit] ->ADSS01 &= ~(1ul<<Ch);
    ADC12Reg[Unit] ->ADSS01 |= (RegSel<<Ch);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the sample time 0 of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 ** \param BitsST Bits of sample time
 ** \param BitsSTX multiply of sample time
 ** \arg   ADC12_SAMPLE_TIME_N1
 ** \arg   ADC12_SAMPLE_TIME_N4
 ** \arg   ADC12_SAMPLE_TIME_N8
 ** \arg   ADC12_SAMPLE_TIME_N16
 ** \arg   ADC12_SAMPLE_TIME_N32
 ** \arg   ADC12_SAMPLE_TIME_N64
 ** \arg   ADC12_SAMPLE_TIME_N128
 ** \arg   ADC12_SAMPLE_TIME_N256
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetSampleTime0(uint8_t Unit, uint8_t BitsST, ADC12_SamplingTimeNT BitsSTX)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_SAMPEL_TIMEN_VALID(BitsSTX));
    
    ADC12Reg[Unit] ->ADST0 = (BitsST | (BitsSTX<<5));
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the sample time 1 of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 ** \param BitsST Bits of sample time
 ** \param BitsSTX multiply of sample time
 ** \arg   ADC12_SAMPLE_TIME_N1
 ** \arg   ADC12_SAMPLE_TIME_N4
 ** \arg   ADC12_SAMPLE_TIME_N8
 ** \arg   ADC12_SAMPLE_TIME_N16
 ** \arg   ADC12_SAMPLE_TIME_N32
 ** \arg   ADC12_SAMPLE_TIME_N64
 ** \arg   ADC12_SAMPLE_TIME_N128
 ** \arg   ADC12_SAMPLE_TIME_N256
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetSampleTime1(uint8_t Unit, uint8_t BitsST, ADC12_SamplingTimeNT BitsSTX)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_SAMPEL_TIMEN_VALID(BitsSTX));
    
    ADC12Reg[Unit] ->ADST1 = (BitsST | (BitsSTX<<5));
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the compare time of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 ** \param Div Division rate
 ** \arg 2~9 for Type0, 1~65 for other Type
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetCompareTime(uint8_t Unit,  uint8_t Div)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_DIV_VALID(Div));
    if((Div == 1) || (Div >= 10))
    {
    #if(ADC_CONFIG_TYPE == ADC_TYPE_B)
        if(Div == 1)
        {
            ADC12Reg[Unit] ->ADCT = 0x80;
        }
        else
        {
            ADC12Reg[Unit] ->ADCT = (Div-2);
        }
    #endif
    }
    else if((Div >= 2) && (Div <= 9))
    {
         ADC12Reg[Unit] ->ADCT = (Div-2);
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable FIFO OVE interrupt of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 ** \param IntCallback Interrupt callback function
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_EnableFIFOOVEInt(uint8_t Unit, ADC_FIFOOVECallbackT* IntCallback)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IntCallback != NULL);
    
    FIFOOVECallback[Unit] = IntCallback;
    ADC12Reg[Unit] ->ADCR |= 1ul;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable FIFO OVE interrupt of ADC12 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 ** \param IntCallback Interrupt callback function
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_DisableFIFOOVEInt(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ADC12Reg[Unit] ->ADCR &= ~1ul;
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable ADC operation
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 ** \param IntCallback Interrupt callback function
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_EnableOpt(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit] ->ADCEN |= 0x01u;
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable ADC operation
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_DisableOpt(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit] ->ADCEN &= ~0x01u;
    return;
}

/*!
 ******************************************************************************
 ** \brief Get the ADC operation status
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 **
 ** \return Operation status
 ** \relval RESET
 ** \relval SET
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetOptStat(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    return (FlagStatusT)((ADC12Reg[Unit] ->ADCEN & 0x02u)>>1);
}

#if (ADC_CONFIG_TYPE == ADC_TYPE_B)
/*!
 ******************************************************************************
 ** \brief Set ADC enable stable time
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2 
 **
 ** \param EnableTime Enable time
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetOptStableTrsfTime(uint8_t Unit, uint8_t EnableTime)
{  
    ADC12Reg[Unit] ->ADCEN &= (0x00FF);
    ADC12Reg[Unit] ->ADCEN |= (EnableTime<<8);
    return;
}
#endif
/*------------------- ADC scan conversion setting ---------------------------*/

/*!
 ******************************************************************************
 ** \brief  Read Scan conversion FIFO empty bit
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2   
 **
 ** \return Scan conversion FIFO empty bit
 ** \retval RESET Data remains in FIFO
 ** \retval SET FIFO is empty.
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetScanFIFOEmptyFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Read Scan conversion FIFO empty bit*/
    if((ADC12Reg[Unit] ->SCCR & (1ul<<7)) == (1ul<<7))
    {
        return SET;
    }
    else
    {
        return RESET;      
    }
}

/*!
 ******************************************************************************
 ** \brief  Read Scan conversion FIFO full bit
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2   
 **
 ** \return Scan conversion FIFO full bit
 ** \retval RESET Data can be input to FIFO.
 ** \retval SET FIFO is full.
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetScanFIFOFullFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Read Scan conversion FIFO empty bit*/
    if((ADC12Reg[Unit] ->SCCR & (1ul<<6)) == (1ul<<6))
    {
        return SET;
    }
    else
    {
        return RESET;      
    }
}

/*!
 ******************************************************************************
 ** \brief  Read Scan conversion FIFO overrun flag
 **
 ** \param ADC12_Unit set ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2   
 **
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */
IntStatusT ADC12_GetScanFIFOOVRFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Read Scan conversion FIFO empty bit*/
    if((ADC12Reg[Unit] ->SCCR & (1ul<<5)) == (1ul<<5))
    {
        return SET;
    }
    else
    {
        return RESET;      
    }
    
}

/*!
 ******************************************************************************
 ** \brief Clear fifo for scan mode
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_ClrScanFIFO(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Clear Data in FIFO*/
    ADC12Reg[Unit]->SCCR |= (1ul<<4);		
    return;
}

/*!
 ******************************************************************************
 ** \brief Set scan mode
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Mode
 ** \arg ADC12_SCAN_MODE_SINGLE
 ** \arg ADC12_SCAN_MODE_REPEAT
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetScanMode(uint8_t Unit, ADC12_ScanModeT Mode)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_SCAN_MODE_VALID(Mode));
    
    ADC12Reg[Unit]->SCCR &= ~(1ul<<2);
    ADC12Reg[Unit]->SCCR |= (Mode<<2);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable timer trigger of  scan mode
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_EnableScanTimerTrig(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->SCCR |= (1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable timer trigger of  scan mode
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_DisableScanTimerTrig(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->SCCR &= ~(1ul<<1);
    return;
}

/*!
 ******************************************************************************
 ** \brief Select trigger timer of  scan mode
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param TimerCh Timer channel
 ** \arg   ADC12_TIMER_TRIGGER_NONE
 ** \arg   ADC12_TIMER_TRIGGER_MFT
 ** \arg   ADC12_TIMER_TRIGGER_BT0
 ** \arg   ...
 ** \arg   ADC12_TIMER_TRIGGER_BT7
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SelScanTrigTimerCh(uint8_t Unit, ADC12_TrigTimerChT TimerCh)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_TRIGGER_MODE_CH(TimerCh));
    
    ADC12Reg[Unit]->SCTSL &= ~15ul;
    ADC12Reg[Unit]->SCTSL |= TimerCh;
    return;
}

/*!
 ******************************************************************************
 ** \brief Start scan conversion
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_StartScan(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->SCCR |= (1ul);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set scan FIFO stage
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Stage Scan FIFO stage
 ** \arg   ADC12_SCAN_FIFO_STAGE1
 ** \arg   ADC12_SCAN_FIFO_STAGE2
 ** \arg   ...
 ** \arg   ADC12_SCAN_FIFO_STAGE16
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetScanFIFOStage(uint8_t Unit, ADC12_ScanFIFOStageT Stage)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_SCAN_FIFO_STAGE_VALID(Stage));
    
    ADC12Reg[Unit]->SFNS &= ~15ul;
    ADC12Reg[Unit]->SFNS |= Stage;
    return;
}

/*!
 ******************************************************************************
 ** \brief Get the FIFO information of scan conversion
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param ScanFIFOInfo Scan FIFO information
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_GetScanFIFOInfo(uint8_t Unit, ADC_ScanFIFOInfoT* ScanFIFOInfo)
{
    volatile uint16_t reg;
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    reg = ADC12Reg[Unit]->SCFDL;
    if((reg & (1ul<<12)) == (1ul<<12))
    {
        ScanFIFOInfo->DataValid = ADC12_SCAN_DATA_INVALID;
    }
    else
    {
        ScanFIFOInfo->DataValid = ADC12_SCAN_DATA_VALID;
    }
    
    if((reg & (3ul<<8)) == (1ul<<8))
    {
        ScanFIFOInfo->TrigMode = ADC12_SCAN_TRIG_BY_SW;
    }
    else if((reg & (3ul<<8)) == (2ul<<8))
    {
        ScanFIFOInfo->TrigMode = ADC12_SCAN_TRIG_BY_TIMER;
    }
    
    ScanFIFOInfo->Ch = reg & 0x001F;
    
    return; 
}

/*!
 ******************************************************************************
 ** \brief Get the FIFO data of scan conversion
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
uint16_t ADC12_GetScanFIFOData(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    return (uint16_t)ADC12Reg[Unit]->SCFDH;
}

/*!
 ******************************************************************************
 ** \brief Select scan conversion channels
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param ChIndex Scan channels index
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SelScanCh(uint8_t Unit, uint32_t ChIndex)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->SCIS01 = (uint16_t)ChIndex;
    ADC12Reg[Unit]->SCIS23 = (uint16_t)(ChIndex>>16);
    
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable scan conversion interrupt
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param IntCallBack Interrupt callback function
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_EnableScanInt(uint8_t Unit, ADC_ScanCallbackT* IntCallBack)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IntCallBack != (NULL));
    
    ScanCallback[Unit] = IntCallBack;
    ADC12Reg[Unit]->ADCR |= (1ul<<3);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable scan conversion interrupt
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_DisableScanInt(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
     
    ADC12Reg[Unit]->ADCR &= ~(1ul<<3);
    return;
}

/*!
 ******************************************************************************
 ** \brief Get the interrupt flag of scan conversion
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return Interrupt flag of scan conversion
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */
IntStatusT ADC12_GetScanIntFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    if((ADC12Reg[Unit]->ADCR & (1ul<<7)) == (1ul<<7))
    {
        return SET;
    }
    
    return RESET;
}
/*!
 ******************************************************************************
 ** \brief Clear the interrupt flag of scan conversion
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_ClrScanIntFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->ADCR &= ~(1ul<<7);
}


/*------------------- ADC priority conversion setting -----------------------*/
/*!
 ******************************************************************************
 ** \brief  Get priority conversion FIFO empty bit
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2   
 **
 ** \return Priority conversion FIFO empty bit
 ** \retval RESET Data remains in FIFO
 ** \retval SET FIFO is empty
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetPrioFIFOEmptyFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Read priority conversion FIFO empty bit*/
    if((ADC12Reg[Unit] ->PCCR & (1ul<<7))== (1ul<<7))
    {
        return SET;
    }
    else
    {
        return RESET;      
    }
}

/*!
 ******************************************************************************
 ** \brief  Get Priority conversion FIFO full bit
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2   
 **
 ** \return Priority conversion FIFO full bit
 ** \retval RESET Data can be input to FIFO.
 ** \retval SET FIFO is full
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetPrioFIFOFullFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Read Scan conversion FIFO empty bit*/
    if((ADC12Reg[Unit] ->PCCR & (1ul<<6))== (1ul<<6))
    {
        return SET;
    }
    else
    {
        return RESET;      
    }
}

/*!
 ******************************************************************************
 ** \brief  Get Priority conversion overrun flag
 **
 ** \param Unit set ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2   
 **
 ** \return Priority conversion overrun flag
 ** \retval RESET
 ** \retval SET
 **
 ******************************************************************************
 */
FlagStatusT ADC12_GetPrioFIFOOVRFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Read Scan conversion FIFO empty bit*/
    if((ADC12Reg[Unit] ->PCCR & (1ul<<5))== (1ul<<5))
    {
        return SET;
    }
    else
    {
        return RESET;      
    }
}

/*!
 ******************************************************************************
 ** \brief Clear fifo for priority mode
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_ClrPrioFIFO(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Clear Data in FIFO*/
    ADC12Reg[Unit]->PCCR |= (1ul<<4);		
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the external trigger mode of priority conversion 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Mode External trigger mode 
 ** \arg   ADC12_PRIO_EXT_TRIG_P1A
 ** \arg   ADC12_PRIO_EXT_TRIG_EXT_INPUT
 ** 
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SelPrioExtTrigMode(uint8_t Unit, ADC12_PrioExtTrigSelT Mode)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    /*Clear Data in FIFO*/
    ADC12Reg[Unit]->PCCR &= ~(1ul<<3);
    ADC12Reg[Unit]->PCCR |= (Mode<<3);
    
    return;
}


/*!
 ******************************************************************************
 ** \brief Enable Priority conversion external trigger 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void ADC12_EnablePrioExtTrig(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->PCCR |= (1ul<<2); 
    return;   
  
}

/*!
 ******************************************************************************
 ** \brief Disable Priority conversion external trigger 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void ADC12_DisablePrioExtTrig(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
   
    ADC12Reg[Unit]->PCCR &= ~(1ul<<2); 
    return;   
}

/*!
 ******************************************************************************
 ** \brief Enable Priority conversion timer trigger 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void ADC12_EnablePrioTimerTrig(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->PCCR |= (1ul<<1); 
    return;   
}

/*!
 ******************************************************************************
 ** \brief Disable Priority conversion timer trigger 
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void ADC12_DisablePrioTimerTrig(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->PCCR &= ~(1ul<<1); 
    return;   
}

/*!
 ******************************************************************************
 ** \brief Select trigger timer of  priority mode
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param TimerCh Timer channel
 ** \arg   ADC12_TIMER_TRIGGER_NONE
 ** \arg   ADC12_TIMER_TRIGGER_MFT
 ** \arg   ADC12_TIMER_TRIGGER_BT0
 ** \arg   ...
 ** \arg   ADC12_TIMER_TRIGGER_BT7
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SelPrioTrigTimerCh(uint8_t Unit, ADC12_TrigTimerChT TimerCh)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_TRIGGER_MODE_CH(TimerCh));
    
    ADC12Reg[Unit]->PRTSL &= ~15ul;
    ADC12Reg[Unit]->PRTSL |= TimerCh;
}

/*!
 ******************************************************************************
 ** \brief Start priority conversion by software trigger
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \retval None
 **
 ******************************************************************************
 */
void ADC12_StartPrio(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
   
    ADC12Reg[Unit]->PCCR |= (1ul<<0); 
    return;   
}

/*!
 ******************************************************************************
 ** \brief Set priority FIFO stage
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Stage FIFO stage
 ** \arg ADC12_PRIORITY_FIFO_STAGE1
 ** \arg ADC12_PRIORITY_FIFO_STAGE2
 ** \arg ADC12_PRIORITY_FIFO_STAGE3
 ** \arg ADC12_PRIORITY_FIFO_STAGE4
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetPrioFIFOStage(uint8_t Unit, ADC12_PriorityFIFOStageT Stage)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_P_FIFO_STAGE_VALID(Stage));
    /*select FIFO Stage*/
    ADC12Reg[Unit] ->PFNS &= ~3ul;
    ADC12Reg[Unit] ->PFNS |= Stage;
}

/*!
 ******************************************************************************
 ** \brief Get priority FIFO information
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param PrioFIFOInfo pointer to FIFO information structure
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_GetPrioFIFOInfo(uint8_t Unit, ADC_PrioFIFOInfoT* PrioFIFOInfo)
{
    volatile uint16_t reg;
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    reg = ADC12Reg[Unit]->PCFDL;
    if((reg & (1ul<<12)) == (1ul<<12))
    {
        PrioFIFOInfo->DataValid = ADC12_PRIO_DATA_INVALID;
    }
    else
    {
        PrioFIFOInfo->DataValid = ADC12_PRIO_DATA_VALID;
    }
    
    if((reg & (3ul<<8)) == (1ul<<8))
    {
        PrioFIFOInfo->TrigMode = ADC12_PRIO_TRIG_BY_SW;
    }
    else if((reg & (3ul<<8)) == (2ul<<8))
    {
        PrioFIFOInfo->TrigMode = ADC12_PRIO_TRIG_BY_TIMER;
    }
    else if((reg & (3ul<<8)) == (4ul<<8))
    {
        PrioFIFOInfo->TrigMode = ADC12_PRIO_TRIG_BY_EXT_INPUT;
    }
    
    PrioFIFOInfo->Ch = reg & 0x001F;
    
    return; 
}

/*!
 ******************************************************************************
 ** \brief Get priority FIFO data
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
uint16_t ADC12_GetPrioFIFOData(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    return ADC12Reg[Unit]->PCFDH;
}

/*!
 ******************************************************************************
 ** \brief Enable priority conversion interrupt
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param IntCallBack Interrupt callback function
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_EnablePrioInt(uint8_t Unit, ADC_PrioCallbackT* IntCallBack)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_UNIT_VALID(IntCallBack != NULL));
    
    PrioCallback[Unit] = IntCallBack;
    ADC12Reg[Unit]->ADCR |= (1ul<<2); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable priority conversion interrupt
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param IntCallBack Interrupt callback function
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_DisablePrioInt(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->ADCR &= ~(1ul<<2); 
    return;
}

/*!
 ******************************************************************************
 ** \brief Get interrupt flag of priority conversion
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
IntStatusT ADC12_GetPrioIntFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    if((ADC12Reg[Unit]->ADCR & (1ul<<6)) == (1ul<<6))
    {
        return SET;
    }
    return RESET;
}

/*!
 ******************************************************************************
 ** \brief Clear interrupt flag of priority conversion
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_ClrPrioIntFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->ADCR &= ~(1ul<<6);
}

/*!
 ******************************************************************************
 ** \brief Set the channel of priority level 1
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Ch
 ** \arg ADC12_CH0
 ** \arg ADC12_CH1
 ** \arg ...
 ** \arg ADC12_CH7
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SelPrioLevel1Ch(uint8_t Unit, uint8_t Ch)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_P1_CH_VALID(Ch));
    
    ADC12Reg[Unit]->PCIS &= ~(7ul);
    ADC12Reg[Unit]->PCIS |= (Ch);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set the channel of priority level 2
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Ch
 ** \arg ADC12_CH0
 ** \arg ADC12_CH1
 ** \arg ...
 ** \arg ADC12_CH15
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SelPrioLevel2Ch(uint8_t Unit, uint8_t Ch)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_P2_CH_VALID(Ch));
    
    ADC12Reg[Unit]->PCIS &= ~(31ul<<3);
    ADC12Reg[Unit]->PCIS |= (Ch<<3);
    return;
}

/*------------------- ADC comparison function setting -----------------------*/
/*!
 ******************************************************************************
 ** \brief Set compare value
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param CmpVal Compare value
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetCmpVal(uint8_t Unit, uint16_t CmpVal)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(CmpVal <= 4095);
    
    ADC12Reg[Unit]->CMPD = ((CmpVal & ~3ul) << 4);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set compare channel
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Ch
 ** \arg ADC12_CH0
 ** \arg ADC12_CH1
 ** \arg ...
 ** \arg ADC12_CH15
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_SetCmpCh(uint8_t Unit, uint8_t Ch)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC_CMP_CH_VALID(Ch));
    
    ADC12Reg[Unit]->CMPCR &= ~(31ul);
    ADC12Reg[Unit]->CMPCR |= Ch;
    return;
}


/*!
 ******************************************************************************
 ** \brief Set compare mode0
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Mode0
 ** \arg ADC12_CMP_SINGLE_CH
 ** \arg ADC12_CMP_ALL_CH
 **
 ** \return None
 **
 ******************************************************************************
 */           
void ADC12_SetCmpMode0(uint8_t Unit, ADC12_CmpMode0T Mode0)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC12_CMP_MODE0_VALID(Mode0));
    
    ADC12Reg[Unit]->CMPCR &= ~(1ul<<5);
    ADC12Reg[Unit]->CMPCR |= (Mode0<<5);
    return;
}

/*!
 ******************************************************************************
 ** \brief Set compare mode1
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param Mode1
 ** \arg ADC12_CMP_SMALL
 ** \arg ADC12_CMP_EQU_MORE
 **
 ** \return None
 **
 ******************************************************************************
 */            
void ADC12_SetCmpMode1(uint8_t Unit, ADC12_CmpMode1T Mode1)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IS_ADC12_CMP_MODE1_VALID(Mode1));
    
    ADC12Reg[Unit]->CMPCR &= ~(1ul<<6);
    ADC12Reg[Unit]->CMPCR |= (Mode1<<6);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable compare function
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */  
void ADC12_EnableCmp(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->CMPCR |= (1ul<<7);
    return;
}

/*!
 ******************************************************************************
 ** \brief Disable compare function
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */ 
void ADC12_DisableCmp(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    ADC12Reg[Unit]->CMPCR &= ~(1ul<<7);
    return;
}

/*!
 ******************************************************************************
 ** \brief Enable interrupt of compare function
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 ** \param IntCallBack Interrupt callback function
 **
 ** \return None
 **
 ******************************************************************************
 */ 
void ADC12_EnableCmpInt(uint8_t Unit, ADC_CmpCallbackT* IntCallBack)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    ASSERT(IntCallBack != NULL);
    
    CmpCallback[Unit] = IntCallBack;
    ADC12Reg[Unit]->ADCR |= (1ul<<1);
    
}

/*!
 ******************************************************************************
 ** \brief Disable interrupt of compare function
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
void ADC12_DisableSCmpInt(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));

    ADC12Reg[Unit]->ADCR &= ~(1ul<<1);
    
}

/*!
 ******************************************************************************
 ** \brief Get interrupt flag of compare function
 **
 ** \param Unit ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2
 **
 ** \return None
 **
 ******************************************************************************
 */
IntStatusT ADC12_GetCmpIntFlag(uint8_t Unit)
{
    /* Check the parameter */
    ASSERT(IS_ADC_UNIT_VALID(Unit));
    
    if((ADC12Reg[Unit]->ADCR & (1ul<<5)) == (1ul<<5))
    {
        return SET;
    }
    return RESET;
}

/*---------------------------- ADC IRQ --------------------------------------*/
/*!
 ******************************************************************************
 ** \brief ADC interrupt handler 
 ** 
 ** \param Unit set ADC unit
 ** \arg   ADC_UNIT0
 ** \arg   ADC_UNIT1
 ** \arg   ADC_UNIT2  
 ** \param Irq interrupt request
 **        This parameter can be one of the following values:
 ** \arg   
 ** \retval None
 **
 ******************************************************************************
 */
void ADC_IntHandler(uint8_t Unit, uint8_t Irq)
{
    /* SCAN Converter Interrupt */
    if((Irq & 0x02U) != 0)
    {
        /* Clear Interrupt */
        ADC12Reg[Unit]->ADCR &= ~(1ul<<7);
        if(ScanCallback[Unit]  != NULL)
        {
            /* Call CallBackIrq */
            ScanCallback[Unit] ();

        }

    }
	
    /* PRI Converter Interrupt */
    if((Irq & 0x01U) != 0) 
    {  
        /* Clear Interrupt */
        ADC12Reg[Unit]->ADCR  &= ~(1ul<<6);
        if(PrioCallback[Unit] != NULL) 
        {
            /* Call CallBackIrq */
            PrioCallback[Unit]();
        }
    }

    /* Over run Interrupt */
    if((Irq & 0x04U) != 0) 
    {  
        /* Check interrupt */
        if((ADC12Reg[Unit]->SCCR & (1ul<<5)) != 0)
        {
            /* Clear Interrupt */
            ADC12Reg[Unit]->SCCR &= ~(1ul<<5);
            
            if(FIFOOVECallback[Unit]  != NULL) 
	    {
                /* Call CallBackIrq */
                FIFOOVECallback[Unit] (ADC12_SCAN_OVE_MODE);
            }
        }
        
        if((ADC12Reg[Unit]->PCCR & (1ul<<5)) != 0)
        {
            /* Clear Interrupt */
            ADC12Reg[Unit]->PCCR &= ~(1ul<<5);
            
            if (FIFOOVECallback[Unit]  != NULL) 
            {
                /* Call CallBackIrq */
                FIFOOVECallback[Unit] (ADC12_PRIO_OVE_MODE);
            }
        }
    }  
	
    /* Compare Interrupt */
    if ((Irq & 0x08U) != 0) 
    { 
        /* Clear Interrupt */
        ADC12Reg[Unit]->ADCR &= ~(1ul<<5);
        
        if (CmpCallback[Unit]  != NULL) 
	{
            /* Call CallBackIrq */
            CmpCallback[Unit] ();
        }
    }
}

#endif
/*****************************************************************************/

/* END OF FILE */

