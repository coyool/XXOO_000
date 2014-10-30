/*!
 ******************************************************************************
 **
** \file lpcm_fm3.h
 **
 ** \brief head file of low power consumption drivers
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
 
/*!
 ******************************************************************************
 ** \brief Low Power Mode type structure
 ******************************************************************************
 */

#ifndef _LPCM_FM3_H_
#define _LPCM_FM3_H_

#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_LPCM == OFF)
    #if (PERIPHERAL_ENABLE_LPCM == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_LPCM == ON) && (PERIPHERAL_AVAILABLE_LPCM == ON)

/*! \brief set low as valid level */  
#define VALID_LEVEL_LOW       0
/*! \brief set high as valid level */  
#define VALID_LEVEL_HIGH      1


/*!
******************************************************************************
 ** \brief WIFSR bit define
******************************************************************************
*/
/*! \brief WRTCI bit of WIFSR */
#define WIFSR_WRTCI                 (1ul<<0)
/*! \brief WLVDI bit of WIFSR */
#define WIFSR_WLVDI                 (1ul<<1)
/*! \brief WUI0 bit of WIFSR */
#define WIFSR_WUI0                  (1ul<<2)
/*! \brief WUI1 bit of WIFSR */
#define WIFSR_WUI1                  (1ul<<3)
/*! \brief WUI2 bit of WIFSR */
#define WIFSR_WUI2                  (1ul<<4)
/*! \brief WUI3 bit of WIFSR */
#define WIFSR_WUI3                  (1ul<<5)
/*! \brief WUI4 bit of WIFSR */
#define WIFSR_WUI4                  (1ul<<6)
/*! \brief WUI5 bit of WIFSR */
#define WIFSR_WUI5                  (1ul<<7)
/*! \brief WCEC0I bit of WIFSR */
#define WIFSR_WCEC0I                (1ul<<8)
/*! \brief WCEC1I bit of WIFSR */
#define WIFSR_WCEC1I                (1ul<<9)

/*!
******************************************************************************
 ** \brief WRFSR bit define
******************************************************************************
*/
/*! \brief WINITX bit of WRFSR */
#define WRFSR_WINITX                 (1ul<<0)
/*! \brief WLVDH bit of WRFSR */
#define WRFSR_WLVDH                  (1ul<<1)

/*!
******************************************************************************
 ** \brief define standby mode type
******************************************************************************
*/
typedef enum LowPwrMode
{
    STB_SlpMode=1,    //!< standby sleep mode
    STB_TimerMode,    //!< standby timer mode    
    STB_StopMode,     //!< standby stop mode  
#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE5) || (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)       
    STB_RTCMode,      //!< standby RTC mode 
    DPSTB_RTCMode,    //!< deep standby RTC mode
    DPSTB_StopMode,    //!< deep standby stop mode
#endif         
} LowPwrModeT;


/*!
******************************************************************************
 ** \brief IO status define
******************************************************************************
*/
#define STB_IO_KEEP      0
#define STB_IO_HIZ       1

#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE5) || (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)  
/*!
******************************************************************************
 ** \brief define return cuase from deep sleep
******************************************************************************
*/
typedef enum 
{
    DSTB_RetCause_RTCInt=1,    //!< return from RTC interrupt
    DSTB_RetCause_LVD,         //!< return from LVD interrupt
    DSTB_RetCause_WkupPin1,    //!< return from Wkup pin1 detection
    DSTB_RetCause_WkupPin2,    //!< return from Wkup pin2 detection
    DSTB_RetCause_WkupPin3,    //!< return from Wkup pin3 detection
    DSTB_RetCause_WkupPin4,    //!< return from Wkup pin4 detection
    DSTB_RetCause_WkupPin5,    //!< return from Wkup pin5 detection
    DSTB_RetCause_CEC0,        //!< return from Wkup CEC1 reception interrupt
    DSTB_RetCause_CEC1,        //!< return from Wkup CEC2 reception interrupt
} RetCauseEnT;

/*!
******************************************************************************
 ** \brief define deep standby mode return cause flag
******************************************************************************
*/
typedef enum 
{
    DSTB_RetCauseFlag_Null = 0,
    DSTB_RetCauseFlag_INITX,       //!< return from INITX input reset
    DSTB_RetCauseFlag_LVDReset,    //!< return from LVD reset
    DSTB_RetCauseFlag_RTCInt,      //!< return from RTC intrrupt
    DSTB_RetCauseFlag_LVDInt,      //!< return from LVD interrupt
    DSTB_RetCauseFlag_WkupPin0,    //!< return from Wkup pin1 detection
    DSTB_RetCauseFlag_WkupPin1,    //!< return from Wkup pin1 detection
    DSTB_RetCauseFlag_WkupPin2,    //!< return from Wkup pin2 detection
    DSTB_RetCauseFlag_WkupPin3,    //!< return from Wkup pin3 detection
    DSTB_RetCauseFlag_WkupPin4,    //!< return from Wkup pin4 detection
    DSTB_RetCauseFlag_WkupPin5,    //!< return from Wkup pin5 detection
    DSTB_RetCauseFlag_CEC0,        //!< return from Wkup CEC1 reception interrupt
    DSTB_RetCauseFlag_CEC1,        //!< return from Wkup CEC2 reception interrupt
} RetCauseFlagT;


/*!
******************************************************************************
 ** \brief define wkup pin index
******************************************************************************
*/
typedef enum 
{
    WKUP_PIN1=0,    //!< index of wkup pin 1
    WKUP_PIN2,      //!< index of wkup pin 2
    WKUP_PIN3,      //!< index of wkup pin 3
    WKUP_PIN4,      //!< index of wkup pin 4
    WKUP_PIN5,      //!< index of wkup pin 5
} WkupPinIndexT;

/*!
******************************************************************************
 ** \brief backup register index
******************************************************************************
*/
typedef enum 
{
    BACKUP_REG1=0,    //!< index of backup register 1
    BACKUP_REG2,      //!< index of backup register 2
    BACKUP_REG3,      //!< index of backup register 3
    BACKUP_REG4,      //!< index of backup register 4
    BACKUP_REG5,      //!< index of backup register 5
    BACKUP_REG6,      //!< index of backup register 6
    BACKUP_REG7,      //!< index of backup register 7
    BACKUP_REG8,      //!< index of backup register 8
    BACKUP_REG9,      //!< index of backup register 9
    BACKUP_REG10,      //!< index of backup register 10
    BACKUP_REG11,      //!< index of backup register 11
    BACKUP_REG12,      //!< index of backup register 12
    BACKUP_REG13,      //!< index of backup register 13
    BACKUP_REG14,      //!< index of backup register 14
    BACKUP_REG15,      //!< index of backup register 15
    BACKUP_REG16,      //!< index of backup register 16
} BackupRegT;
#endif

/*!
******************************************************************************
 ** \brief sub clock connection for CEC
******************************************************************************
*/
typedef enum
{
    CEC_SUB_CLOCK_DISABLE = 0,
    CEC_SUB_CLOCK_ENABLE,
}CECSubClkT;

/*!
******************************************************************************
 ** \brief sub clock connection for RTC
******************************************************************************
*/
typedef enum
{
    RTC_SUB_CLOCK_DISABLE = 0,
    RTC_SUB_CLOCK_ENABLE,
}RTCSubClkT;

/*!
******************************************************************************
 ** \brief RAM retention for deep standby mode
******************************************************************************
*/
typedef enum
{
    DSTB_RAM_RETENTION_DISABLE = 0,
    DSTB_RAM_RETENTION_ENABLE,
}DStbRAMModeT;

#ifdef __cplusplus
extern "C" {
#endif

void LowPwrCon_GoToStandByMode(LowPwrModeT Type, uint8_t IOStat);
#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE5) || (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)  
void LowPwrCon_EnableRetCause(RetCauseEnT Type);
void LowPwrCon_DisableRetCause(RetCauseEnT Type);
RetCauseFlagT LowPwrCon_ReadRetCause(void);
void LowPwrCon_SetWkupPinLevel(WkupPinIndexT PinIndex, uint8_t Level);
void LowPwrCon_WriteBackupReg(BackupRegT RegIndex, uint8_t Data);
void LowPwrCon_ReadBackupReg(BackupRegT RegIndex, uint8_t *Data);
#endif
#if (MCU_TYPE == DEVICE_TYPE6) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)  
void LowPwrCon_ConfigCECSubClk(CECSubClkT SubClkMode);
void LowPwrCon_ConfigRTCSubClk(RTCSubClkT SubClkMode);
#endif
#if (MCU_TYPE == DEVICE_TYPE5) || (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)  
void LowPwrCon_ConfigDStbRAMRetention(DStbRAMModeT Mode);
#endif
#ifdef __cplusplus
}
#endif


#endif

#endif
