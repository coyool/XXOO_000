/*!
 ******************************************************************************
 **
 ** \file interrupt.h
 **
 ** \brief interrupt handler header file
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.10
 **
 ** \date 2012-11-20
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
 ******************************************************************************/
#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "fm3_common.h"
#include "pdl_user.h"

/*! \brief Chip has how many MFT units */
#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7) || \
    (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9) || \
    (MCU_TYPE == DEVICE_TYPE5)
    #define INT_MFT_AMOUNT 1
    
#elif (MCU_TYPE == DEVICE_TYPE1)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_L)
        #define INT_MFT_AMOUNT 1
    #else
        #define INT_MFT_AMOUNT 2                   
    #endif
#elif (MCU_TYPE == DEVICE_TYPE0)
    #define INT_MFT_AMOUNT 2
    
#else 
    #define INT_MFT_AMOUNT 3
#endif

/* External peripheral IRQ interface function  */
#if (PERIPHERAL_ENABLE_EXTINT == ON)
extern void NMI_IntHandler(void);
#endif

#if (PERIPHERAL_ENABLE_WD == ON)
extern void HWD_IntHandler(void);
#endif

#if (PERIPHERAL_ENABLE_CSV == ON)
extern void CSV_IntHandler(void);
#endif

#if (PERIPHERAL_ENABLE_WD == ON)
extern void SWD_IntHandler(void);
#endif

#if (PERIPHERAL_ENABLE_LVD == ON)
extern void LVD_IntHandler(void);
#endif

#if (PERIPHERAL_ENABLE_MFT_WFG == ON)
extern void MFT_WFG_IntHandler(uint8_t MFTUnit, uint16_t IrqValue);
#endif

#if (PERIPHERAL_ENABLE_EXTINT == ON)
extern void ExtInt_IntHandler(uint8_t Ch);
#endif

#if (PERIPHERAL_ENABLE_DT == ON)
extern void DT_IntHandler(uint8_t Ch);
#endif

#if (PERIPHERAL_ENABLE_QPRC == ON)
extern void QPRC_IntHandler(uint8_t Ch, uint16_t IrqValue);
#endif

#if (PERIPHERAL_ENABLE_MFS_UART == ON) || \
    (PERIPHERAL_ENABLE_MFS_CSIO == ON) || \
    (PERIPHERAL_ENABLE_MFS_I2C == ON) || \
    (PERIPHERAL_ENABLE_MFS_LIN == ON)  
extern void MFS_RX_IntHandler(uint8_t Ch);
extern void MFS_TX_IntHandler(uint8_t Ch, uint8_t IntType);
#endif

#if (PERIPHERAL_ENABLE_PPG == ON)
extern void PPG_IntHandler(uint8_t Ch);
#endif

#if (PERIPHERAL_ENABLE_WC == ON)
extern void WC_IntHandler(void);
#endif

#if (PERIPHERAL_ENABLE_RTC == ON)
extern void RTC_IntHandler(void);
#endif

#if (PERIPHERAL_ENABLE_ADC == ON)
extern void ADC_IntHandler(uint8_t Unit, uint8_t IntType);
#endif

#if (PERIPHERAL_ENABLE_LCDC == ON)
extern void LCD_IntHandler(void);
#endif

#if (PERIPHERAL_ENABLE_MFT_FRT == ON)
extern void MFT_FRT_IntHandler(uint8_t MFTUnit, uint16_t IrqValue);
#endif 

#if (PERIPHERAL_ENABLE_MFT_ICU == ON)
extern void MFT_ICU_IntHandler(uint8_t MFTUnit, uint16_t IrqValue);
#endif

#if (PERIPHERAL_ENABLE_MFT_OCU == ON)
extern void MFT_OCU_IntHandler(uint8_t MFTUnit, uint8_t OCUCh);
#endif

#if (PERIPHERAL_ENABLE_BT == ON)
extern void BT_IntHandler(uint8_t Ch, uint8_t IntType);
#endif

#if (PERIPHERAL_ENABLE_CAN == ON)
extern void CAN_IntHandler(uint8_t Ch);
#endif

#if (PERIPHERAL_ENABLE_ETHER == ON)
extern void ETHER_IntHandler(uint8_t Ch, uint8_t IntType);
#endif

#if (PERIPHERAL_ENABLE_USB == ON)
extern void USBF_IntHandler(uint8_t Ch, uint8_t IntType);
extern void USB_IntHandler(uint8_t Ch, uint8_t IntType);
#endif 

#if (PERIPHERAL_ENABLE_RC == ON) 
extern void HDMICEC_IntHandler(uint8_t Ch);
#endif

#if (PERIPHERAL_ENABLE_DMAC == ON)
extern void DMAC_IntHandler(uint8_t Ch);
#endif

#if (PERIPHERAL_ENABLE_WORKFLASH == ON)
extern void FLASH_IntHandler(void);
#endif



#endif
