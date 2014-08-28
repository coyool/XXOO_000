/*!
 ******************************************************************************
 **
** \file lpcm_fm3.c
 **
 ** \brief this file provides all low power consumption drivers
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-18
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

#include "lpcm_fm3.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_ENABLE_LPCM == ON) && (PERIPHERAL_AVAILABLE_LPCM == ON)
/*!
 ******************************************************************************
 ** \brief define SCR address
 ******************************************************************************
 */
#define CM3_Core_CSR  *((volatile unsigned int*)(0xE000ED10UL))

/*---------------------------------------------------------------------------*/
/* local datatypes                                                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* local data                                                                */
/*---------------------------------------------------------------------------*/
#define LPCM_BACKUP_REG_BASE   ((volatile uint8_t*)0x40035900)

/*---------------------------------------------------------------------------*/
/* global data                                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global functions                                                          */
/*---------------------------------------------------------------------------*/

/*!
 ******************************************************************************
 ** \brief WRFSR dummy read
 ******************************************************************************
 */
static uint16_t WRFSRDummyRead(void)
{
    uint16_t Dummy;
    Dummy = FM3_DS->WRFSR;
    return Dummy;
}

/*!
 ******************************************************************************
 ** \brief go to standby mode
 **
 ** \param tp the one type of standby mode.
 **        This parameter can be one of the following values:
 ** \arg   STB_SlpMode sleep mode
 ** \arg   STB_TimerMode timer mode
 ** \arg   STB_RTCMode RTC mode
 ** \arg   DPSTB_RTCMode deep standby RTC mode
 ** \arg   DPSTB_StopMode deep standby stop mode

 ** \param io_stus set io status when enter standby mode.
 **        This parameter can be one of the following values:
 ** \arg   STB_IO_KEEP keep io status
 **        STB_IO_HIZ  keep hi-z
 **
 ** \retval None
 **
 ******************************************************************************
 */
void LowPwrCon_GoToStandByMode(LowPwrModeT Type, uint8_t IOStat)
{
    WRFSRDummyRead();
    switch (Type)
    {
        case STB_SlpMode:
            CM3_Core_CSR &= 0xFFFFFFFFB;
            __WFI();
            break;
        case STB_TimerMode:
            if( IOStat == 0 )
            {
                FM3_CRG->STB_CTL = 0x1ACC0000;
            }
            else
            {
                FM3_CRG->STB_CTL = 0x1ACC0010;
            }
            CM3_Core_CSR |= 0x00000004;
            __WFI();
            break;
        case STB_StopMode:
            FM3_DS->PMD_CTL &= ~0x01; /* RTCE=0 */
            if( IOStat == 0 )
            {
                FM3_CRG->STB_CTL = 0x1ACC0002;
            }
            else
            {
                FM3_CRG->STB_CTL = 0x1ACC0012;
            }
            CM3_Core_CSR |= 0x00000004;
            __WFI();
            break;    
#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE5) || (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)  
        case STB_RTCMode:
            FM3_DS->PMD_CTL |= 0x01; /* RTCE=1 */
            if( IOStat == 0 )
            {
                FM3_CRG->STB_CTL = 0x1ACC0002;
            }
            else
            {
                FM3_CRG->STB_CTL = 0x1ACC0012;
            }
            CM3_Core_CSR |= 0x00000004;
            __WFI();
            break;
        case DPSTB_RTCMode: 
            FM3_DS->PMD_CTL |= 0x01; /* RTCE=1 */
            if( IOStat == 0 )
            {
                FM3_CRG->STB_CTL = 0x1ACC0006;
            }
            else
            {
                FM3_CRG->STB_CTL = 0x1ACC0016;
            }
            CM3_Core_CSR |= 0x00000004;
            __WFI();
            break;
        case DPSTB_StopMode:
            FM3_DS->PMD_CTL &= ~0x01; /* RTCE=0 */
            if( IOStat == 0 )
            {
                FM3_CRG->STB_CTL = 0x1ACC0006;
            }
            else
            {
                FM3_CRG->STB_CTL = 0x1ACC0016;
            }
            CM3_Core_CSR |= 0x00000004;
            __WFI();
            break;    
#endif                     
        default:
            break;
    }
}

#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE5) || (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)  

/*!
 ******************************************************************************
 ** \brief WRFSR dummy read
 ******************************************************************************
 */
static uint16_t WIFSRDummyRead(void)
{
    uint16_t Dummy;
    Dummy = FM3_DS->WIFSR;
    return Dummy;
}

/*!
 ******************************************************************************
 ** \brief Enable return cause from deep standby mode
 **
 ** \param Type return cause type
 ** \arg   DSTB_RetCause_RTCInt
 ** \arg   DSTB_RetCause_LVD
 ** \arg   DSTB_RetCause_WkupPin1
 ** \arg   DSTB_RetCause_WkupPin2
 ** \arg   DSTB_RetCause_WkupPin3
 ** \arg   DSTB_RetCause_WkupPin4
 ** \arg   DSTB_RetCause_WkupPin5
 ** \arg   DSTB_RetCause_CEC1
 ** \arg   DSTB_RetCause_CEC2
 **         
 ** \return None
 **
 ******************************************************************************
 */
void LowPwrCon_EnableRetCause(RetCauseEnT Type)
{
    WIFSRDummyRead();
    switch (Type)
    {
        case DSTB_RetCause_RTCInt:
            bFM3_DS_WIER_WRTCE = 1;
            break;
        case DSTB_RetCause_LVD:
            bFM3_DS_WIER_WLVDE = 1;
            break;
        case DSTB_RetCause_WkupPin1:
            FM3_GPIO->ADE &= ~0x0002ul;
            FM3_GPIO->PFR1 |= 0x0002ul;
            bFM3_DS_WIER_WUI1E = 1;
            break;
        case DSTB_RetCause_WkupPin2:
            FM3_GPIO->PFR2 |= 0x0002ul;
            bFM3_DS_WIER_WUI2E = 1;
            break;
        case DSTB_RetCause_WkupPin3:
            FM3_GPIO->PFR6 |= 0x0001ul;
            bFM3_DS_WIER_WUI3E = 1;
            break;
        case DSTB_RetCause_WkupPin4:
            break;
        case DSTB_RetCause_WkupPin5:
            break;
        case DSTB_RetCause_CEC0:
            bFM3_DS_WIER_WCEC0E = 1;
            break;
        case DSTB_RetCause_CEC1:
            bFM3_DS_WIER_WCEC1E = 1;
            break;
    }
}

/*!
 ******************************************************************************
 ** \brief Disable return cause from deep standby mode
 **
 ** \param Type return cause type
 ** \arg   DSTB_RetCause_RTCInt
 ** \arg   DSTB_RetCause_LVD
 ** \arg   DSTB_RetCause_WkupPin1
 ** \arg   DSTB_RetCause_WkupPin2
 ** \arg   DSTB_RetCause_WkupPin3
 ** \arg   DSTB_RetCause_WkupPin4
 ** \arg   DSTB_RetCause_WkupPin5
 ** \arg   DSTB_RetCause_CEC1
 ** \arg   DSTB_RetCause_CEC2
 **         
 ** \return None
 **
 ******************************************************************************
 */
void LowPwrCon_DisableRetCause(RetCauseEnT Type)
{
    switch (Type)
    {
        case DSTB_RetCause_RTCInt:
            bFM3_DS_WIER_WRTCE = 0;
            break;
        case DSTB_RetCause_LVD:
            bFM3_DS_WIER_WLVDE = 0;
            break;
        case DSTB_RetCause_WkupPin1:
            bFM3_DS_WIER_WUI1E = 0;
            break;
        case DSTB_RetCause_WkupPin2:
            bFM3_DS_WIER_WUI2E = 0;
            break;
        case DSTB_RetCause_WkupPin3:
            bFM3_DS_WIER_WUI3E = 0;
            break;
        case DSTB_RetCause_WkupPin4:
            break;
        case DSTB_RetCause_WkupPin5:
            break;
        case DSTB_RetCause_CEC0:
            bFM3_DS_WIER_WCEC0E = 0;
            break;
        case DSTB_RetCause_CEC1:
            bFM3_DS_WIER_WCEC1E = 0;
            break;
    }
}

/*!
 ******************************************************************************
 ** \brief Read deep standby mode return cause flag 
 **
 ** \param None
 **         
 ** \return None
 ** \relval DSTB_RetCauseFlag_Null
 ** \relval DSTB_RetCauseFlag_RTCInt
 ** \relval DSTB_RetCauseFlag_LVD
 ** \relval DSTB_RetCauseFlag_WkupPin0
 ** \relval DSTB_RetCauseFlag_WkupPin1
 ** \relval DSTB_RetCauseFlag_WkupPin2
 ** \relval DSTB_RetCauseFlag_WkupPin3
 ** \relval DSTB_RetCauseFlag_WkupPin4
 ** \relval DSTB_RetCauseFlag_WkupPin5
 ** \relval DSTB_RetCauseFlag_CEC0
 ** \relval DSTB_RetCauseFlag_CEC1
 ******************************************************************************
 */
RetCauseFlagT LowPwrCon_ReadRetCause(void)
{
    uint16_t rWIFSR,rWRFSR;
    rWRFSR = FM3_DS->WRFSR;
    rWIFSR = FM3_DS->WIFSR;
    
    if(rWRFSR & WRFSR_WINITX)
    {
        return DSTB_RetCauseFlag_INITX;
    }
    
    if(rWRFSR & DSTB_RetCauseFlag_LVDReset)
    {
        return DSTB_RetCauseFlag_LVDReset;
    }
    
    if(rWIFSR & WIFSR_WRTCI)
    {
        return DSTB_RetCauseFlag_RTCInt;
    }
    else if(rWIFSR & WIFSR_WLVDI)
    {
        return DSTB_RetCauseFlag_LVDInt;
    }
    else if(rWIFSR & WIFSR_WUI0)
    {
        return DSTB_RetCauseFlag_WkupPin0;
    }
    else if(rWIFSR & WIFSR_WUI1)
    {
        return DSTB_RetCauseFlag_WkupPin1;
    }
    else if(rWIFSR & WIFSR_WUI2)
    {
        return DSTB_RetCauseFlag_WkupPin2;
    }
    else if(rWIFSR & WIFSR_WUI3)
    {
        return DSTB_RetCauseFlag_WkupPin3;
    }
    else if(rWIFSR & WIFSR_WUI4)
    {
        return DSTB_RetCauseFlag_WkupPin4;
    }
    else if(rWIFSR & WIFSR_WUI5)
    {
        return DSTB_RetCauseFlag_WkupPin5;
    }
    else if(rWIFSR & WIFSR_WCEC0I)
    {
        return DSTB_RetCauseFlag_CEC0;
    }
    else if(rWIFSR & WIFSR_WCEC1I)
    {
        return DSTB_RetCauseFlag_CEC1;
    }
    return DSTB_RetCauseFlag_Null;
}

/*!
 ******************************************************************************
 ** \brief Set the valid level of wkup pin
 ** 
 ** \param PinIndex Pin index
 ** \arg WKUP_PIN1
 ** \arg WKUP_PIN2
 ** \arg WKUP_PIN3
 ** \arg WKUP_PIN4
 ** \arg WKUP_PIN5
 ** \param Level
 ** \arg VALID_LEVEL_LOW
 ** \arg VALID_LEVEL_HIGH
 **         
 ** \return None
 **
 ******************************************************************************
 */
void LowPwrCon_SetWkupPinLevel(WkupPinIndexT PinIndex, uint8_t Level)
{
    FM3_DS->WILVR &= ~(1ul << PinIndex);
    FM3_DS->WILVR |= (Level << PinIndex);
}

/*!
 ******************************************************************************
 ** \brief Write the backup register
 ** 
 ** \param Backup register index
 ** \arg BACKUP_REG1
 ** \arg BACKUP_REG2
 ** \arg BACKUP_REG3
 ** \arg BACKUP_REG4
 ** \arg BACKUP_REG5
 ** \arg BACKUP_REG6
 ** \arg BACKUP_REG7
 ** \arg BACKUP_REG8
 ** \arg BACKUP_REG9
 ** \arg BACKUP_REG10
 ** \arg BACKUP_REG11
 ** \arg BACKUP_REG12
 ** \arg BACKUP_REG13
 ** \arg BACKUP_REG14
 ** \arg BACKUP_REG15
 ** \arg BACKUP_REG16
 ** \param Data the data written into backup register
 **         
 ** \return None
 **
 ******************************************************************************
 */
void LowPwrCon_WriteBackupReg(BackupRegT RegIndex, uint8_t Data)
{
    *(uint8_t*)(LPCM_BACKUP_REG_BASE + RegIndex) = Data;
}

/*!
 ******************************************************************************
 ** \brief Read the backup register
 ** 
 ** \param Backup register index
 ** \arg BACKUP_REG1
 ** \arg BACKUP_REG2
 ** \arg BACKUP_REG3
 ** \arg BACKUP_REG4
 ** \arg BACKUP_REG5
 ** \arg BACKUP_REG6
 ** \arg BACKUP_REG7
 ** \arg BACKUP_REG8
 ** \arg BACKUP_REG9
 ** \arg BACKUP_REG10
 ** \arg BACKUP_REG11
 ** \arg BACKUP_REG12
 ** \arg BACKUP_REG13
 ** \arg BACKUP_REG14
 ** \arg BACKUP_REG15
 ** \arg BACKUP_REG16
 ** \param Data Store the read data
 **         
 ** \return None
 **
 ******************************************************************************
 */
void LowPwrCon_ReadBackupReg(BackupRegT RegIndex, uint8_t *Data)
{
    *Data = *(uint8_t*)(LPCM_BACKUP_REG_BASE + RegIndex);
}
#endif

#if (MCU_TYPE == DEVICE_TYPE6) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9) 
/*!
 ******************************************************************************
 ** \brief Config sub clock connection for CEC
 ** 
 ** \param SubClkMode Sub clock mode
 ** \arg CEC_SUB_CLOCK_DISABLE
 ** \arg CEC_SUB_CLOCK_ENABLE
 **         
 ** \return None
 **
 ******************************************************************************
 */
void LowPwrCon_ConfigCECSubClk(CECSubClkT SubClkMode)
{
    if(SubClkMode == CEC_SUB_CLOCK_DISABLE)
    {
        bFM3_DS_RCK_CTL_CECCKE = 0;
    }
    else if(SubClkMode == CEC_SUB_CLOCK_ENABLE)
    {
        bFM3_DS_RCK_CTL_CECCKE = 1;
    }
    return;
}

/*!
 ******************************************************************************
 ** \brief Config sub clock connection for RTC
 ** 
 ** \param SubClkMode Sub clock mode
 ** \arg RTC_SUB_CLOCK_DISABLE
 ** \arg RTC_SUB_CLOCK_ENABLE
 **         
 ** \return None
 **
 ******************************************************************************
 */
void LowPwrCon_ConfigRTCSubClk(RTCSubClkT SubClkMode)
{
    if(SubClkMode == RTC_SUB_CLOCK_DISABLE)
    {
        bFM3_DS_RCK_CTL_RTCCKE = 0;
    }
    else if(SubClkMode == RTC_SUB_CLOCK_ENABLE)
    {
        bFM3_DS_RCK_CTL_RTCCKE = 1;
    }
    return;
}
#endif

#if (MCU_TYPE == DEVICE_TYPE5) || (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)  
/*!
 ******************************************************************************
 ** \brief Config deep standby mode RAM retention
 ** 
 ** \param Mode RAM retention mode
 ** \arg DSTB_RAM_RETENTION_DISABLE
 ** \arg DSTB_RAM_RETENTION_ENABLE
 **         
 ** \return None
 **
 ******************************************************************************
 */
void LowPwrCon_ConfigDStbRAMRetention(DStbRAMModeT Mode)
{
    if(Mode == DSTB_RAM_RETENTION_DISABLE)
    {
        FM3_DS->DSRAMR &= ~0x03u; 
    }
    else if(Mode == DSTB_RAM_RETENTION_ENABLE)
    {
        FM3_DS->DSRAMR |= 0x03u; 
    }
    return;
}
#endif

#endif
