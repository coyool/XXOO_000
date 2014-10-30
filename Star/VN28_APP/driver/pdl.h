/*!
 ******************************************************************************
 **
 ** \file pdl.h
 **
 ** \brief  Common header file for FM3 Peripheral Driver Library 
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

/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

#ifndef _PDL_H_
#define _PDL_H_

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "fm3_common.h"

/**
 ******************************************************************************
 ** Global Device type defintions
 ******************************************************************************/ 
#define DEVICE_TYPE0 0  ///< FM3 device type0
#define DEVICE_TYPE1 1  ///< FM3 device type1
#define DEVICE_TYPE2 2  ///< FM3 device type2
#define DEVICE_TYPE3 3  ///< FM3 device type3
#define DEVICE_TYPE4 4  ///< FM3 device type4
#define DEVICE_TYPE5 5  ///< FM3 device type5
#define DEVICE_TYPE6 6  ///< FM3 device type6
#define DEVICE_TYPE7 7  ///< FM3 device type7
#define DEVICE_TYPE8 8  ///< FM3 device type8
#define DEVICE_TYPE9 9  ///< FM3 device type9


/**
 ******************************************************************************
 ** Global User Package Choice List
 ******************************************************************************/ 
#define DEVICE_PACKAGE_K  10
#define DEVICE_PACKAGE_L  20
#define DEVICE_PACKAGE_M  30
#define DEVICE_PACKAGE_N  40
#define DEVICE_PACKAGE_R  50
#define DEVICE_PACKAGE_S  60
#define DEVICE_PACKAGE_T  70

/**
 ******************************************************************************
 ** Device series defintions
 ******************************************************************************/ 
#define DEVICE_SERIES_MB9AF10X   0
#define DEVICE_SERIES_MB9AF11X  10
#define DEVICE_SERIES_MB9AF13X  20
#define DEVICE_SERIES_MB9AF14X  25
#define DEVICE_SERIES_MB9AF15X  26
#define DEVICE_SERIES_MB9AF31X  30
#define DEVICE_SERIES_MB9AF34X  33
#define DEVICE_SERIES_MB9AFA3X  34
#define DEVICE_SERIES_MB9AFA4X  36
#define DEVICE_SERIES_MB9AFB4X  37
#define DEVICE_SERIES_MB9BF10X  40
#define DEVICE_SERIES_MB9BF11X  45
#define DEVICE_SERIES_MB9BF12X  46
#define DEVICE_SERIES_MB9BF21X  55
#define DEVICE_SERIES_MB9BF30X  60
#define DEVICE_SERIES_MB9BF31X  65
#define DEVICE_SERIES_MB9BF32X  66
#define DEVICE_SERIES_MB9BF40X  70
#define DEVICE_SERIES_MB9BF41X  75
#define DEVICE_SERIES_MB9BF50X  80
#define DEVICE_SERIES_MB9BF51X  85
#define DEVICE_SERIES_MB9BF52X  86
#define DEVICE_SERIES_MB9BF61X 100
#define DEVICE_SERIES_MB9BFD1X 110

/**
 ******************************************************************************
 ** User Setting Include file  
 ******************************************************************************/  
#include "pdl_device.h"   /* must be included here */

/**
 ******************************************************************************
 ** PDL ON/OFF definition
 ******************************************************************************/  
#define   OFF       0
#define   ON        1

/*!
 ******************************************************************************
 ** \brief Device type extraction
 ******************************************************************************
 */ 
#if (MCU_SERIES == DEVICE_SERIES_MB9AF10X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE0
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AF11X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE1
  #elif (MCU_PACKAGE == DEVICE_PACKAGE_K)
    #define MCU_TYPE DEVICE_TYPE5
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AF13X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_K) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_L)
    #define MCU_TYPE DEVICE_TYPE3
  #elif (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE7
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AF14X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE6
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AF15X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_R) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE8
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AF31X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE1
  #elif (L3_PACKAGE == L3_DEVICE_PACKAGE_K)
    #define MCU_TYPE DEVICE_TYPE5
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AF34X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE6
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AFA3X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE7
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AFA4X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE6
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9AFB4X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_N)
    #define MCU_TYPE DEVICE_TYPE6
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF10X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE0
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF11X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE4
  #elif (MCU_PACKAGE == DEVICE_PACKAGE_S) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_T)
    #define MCU_TYPE DEVICE_TYPE2
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF12X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_K) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M)  
    #define MCU_TYPE DEVICE_TYPE9
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF21X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_S) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_T)
    #define MCU_TYPE DEVICE_TYPE2
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF30X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE0
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF31X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE4
  #elif (MCU_PACKAGE == DEVICE_PACKAGE_S) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_T)
    #define MCU_TYPE DEVICE_TYPE2
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF32X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_K) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M)  
    #define MCU_TYPE DEVICE_TYPE9
  #else
   #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF40X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE0
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF41X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE4
  #elif (MCU_PACKAGE == DEVICE_PACKAGE_S) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_T)
    #define MCU_TYPE DEVICE_TYPE2
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF50X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE0
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF51X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_N) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_R)
    #define MCU_TYPE DEVICE_TYPE4
  #elif (MCU_PACKAGE == DEVICE_PACKAGE_S) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_T)
    #define MCU_TYPE DEVICE_TYPE2
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF52X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_K) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_L) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_M)  
    #define MCU_TYPE DEVICE_TYPE9
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BF61X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_S) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_T)
    #define MCU_TYPE DEVICE_TYPE2
  #else
    #error Device Package not found!
  #endif

#elif (MCU_SERIES == DEVICE_SERIES_MB9BFD1X)
  #if   (MCU_PACKAGE == DEVICE_PACKAGE_S) || \
        (MCU_PACKAGE == DEVICE_PACKAGE_T)
    #define MCU_TYPE DEVICE_TYPE2
  #else
    #error Device Package not found!
  #endif

#else
  #error Device not found!
#endif

/*!
 ******************************************************************************
 ** \brief Device peripheral available/unavailable difinition
 ******************************************************************************
 */ 
/*! \brief ADC */
#define PERIPHERAL_AVAILABLE_ADC                 ON   

/*! \brief Base Timer */
#define PERIPHERAL_AVAILABLE_BT                  ON  

/*! \brief CAN */
#if (MCU_SERIES == DEVICE_SERIES_MB9BFD1X) ||  \
    (MCU_SERIES == DEVICE_SERIES_MB9BF51X) ||  \
    (MCU_SERIES == DEVICE_SERIES_MB9BF41X) ||  \
    (MCU_SERIES == DEVICE_SERIES_MB9BF50X) ||  \
    (MCU_SERIES == DEVICE_SERIES_MB9BF40X) 
    #define PERIPHERAL_AVAILABLE_CAN             ON   
#else
    #define PERIPHERAL_AVAILABLE_CAN             OFF
#endif

#if (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7)
    /*! \brief CRC */
    #define PERIPHERAL_AVAILABLE_CRC                 OFF  
#else
    /*! \brief CRC */
    #define PERIPHERAL_AVAILABLE_CRC                 ON
#endif

/*! \brief Clock */
#define PERIPHERAL_AVAILABLE_CLK                 ON   

/*! \brief DAC */
#if (MCU_SERIES == DEVICE_SERIES_MB9AFA3X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF12X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF32X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF52X)  
    #define PERIPHERAL_AVAILABLE_DAC             ON   
#elif (MCU_SERIES == DEVICE_SERIES_MB9AF13X)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_N) || (MCU_PACKAGE == DEVICE_PACKAGE_M) 
        #define PERIPHERAL_AVAILABLE_DAC         ON
    #else
        #define PERIPHERAL_AVAILABLE_DAC         OFF
    #endif 
#else
    #define PERIPHERAL_AVAILABLE_DAC         OFF
#endif   

/*! \brief Clock */
#define PERIPHERAL_AVAILABLE_CLOCK               ON  

/*! \brief Daul Timer */       
#if ((MCU_SERIES != DEVICE_SERIES_MB9AF13X) && (MCU_SERIES != DEVICE_SERIES_MB9AFA3X))        
    #define PERIPHERAL_AVAILABLE_DT              ON
#else
    #define PERIPHERAL_AVAILABLE_DT              OFF
#endif 

/*! \brief DMA */       
#if ((MCU_SERIES != DEVICE_SERIES_MB9AF13X) && (MCU_SERIES != DEVICE_SERIES_MB9AFA3X))        
    #define PERIPHERAL_AVAILABLE_DMAC              ON
#else
    #define PERIPHERAL_AVAILABLE_DMAC              OFF 
#endif 

/*! \brief CR trimming */  
#define PERIPHERAL_AVAILABLE_CR                   ON   

/*! \brief CSV */       
#define PERIPHERAL_AVAILABLE_CSV                  ON

/*! \brief External interrupt */       
#define PERIPHERAL_AVAILABLE_EXTINT               ON  

/*! \brief External interrupt */   
#define PERIPHERAL_AVAILABLE_EXTBUS               ON

/*! \brief Work Flash */  
#if (MCU_SERIES == DEVICE_SERIES_MB9BF51X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF41X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF31X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF11X)   
    #if (MCU_PACKAGE == DEVICE_PACKAGE_R) || (MCU_PACKAGE == DEVICE_PACKAGE_N) 
        #define PERIPHERAL_AVAILABLE_WORKFLASH            ON 
    #elif
        #define PERIPHERAL_AVAILABLE_WORKFLASH            OFF 
    #endif
#elif (MCU_SERIES == DEVICE_SERIES_MB9AFB4X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9AFA4X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9AF34X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9AF14X)
    #define PERIPHERAL_AVAILABLE_WORKFLASH            OFF 
#elif (MCU_SERIES == DEVICE_SERIES_MB9AF31X) ||  (MCU_SERIES == DEVICE_SERIES_MB9AF11X)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_K)
        #define PERIPHERAL_AVAILABLE_WORKFLASH            ON
    #else
        #define PERIPHERAL_AVAILABLE_WORKFLASH            OFF
    #endif  
#else
    #define PERIPHERAL_AVAILABLE_WORKFLASH            OFF      
#endif 

/*! \brief Main Flash & Dual Flash */
#if (MCU_SERIES == DEVICE_SERIES_MB9AFB4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF34X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF14X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF15X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF52X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF32X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF12X)
    #define  PERIPHERAL_AVAILABLE_DUALFLASH     ON
    #define  PERIPHERAL_AVAILABLE_MAINFLASH     OFF
#else
    #define  PERIPHERAL_AVAILABLE_DUALFLASH     OFF
    #define  PERIPHERAL_AVAILABLE_MAINFLASH     ON
#endif      

/*! \brief IO */  
#define  PERIPHERAL_AVAILABLE_IO         ON

/*! \brief LVD */      
#define PERIPHERAL_AVAILABLE_LVD         ON

/*! \brief LPCM */ 
#define PERIPHERAL_AVAILABLE_LPCM        ON

/*! \brief MFS */        
#define PERIPHERAL_AVAILABLE_MFS         ON

/*! \brief MFT */   
#if (MCU_TYPE != DEVICE_TYPE6)
    #define PERIPHERAL_AVAILABLE_MFT         ON
#endif

/*! \brief QPRC */  
#if (MCU_SERIES == DEVICE_SERIES_MB9AFB4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF34X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF14X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA3X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF13X)          
    #define PERIPHERAL_AVAILABLE_QPRC        OFF  
#else
    #define PERIPHERAL_AVAILABLE_QPRC        ON
#endif

/*! \brief Watch counter */   
#if (MCU_SERIES == DEVICE_SERIES_MB9AFA3X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF13X)       
    #define PERIPHERAL_AVAILABLE_WC              OFF   
#else
    #define PERIPHERAL_AVAILABLE_WC              ON
#endif

/*! \brief Hardware watchdog */         
#define PERIPHERAL_AVAILABLE_WD                 ON 

/*! \brief RTC */          
#if (MCU_SERIES == DEVICE_SERIES_MB9AFB4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF34X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF14X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA3X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF13X)
    #define PERIPHERAL_AVAILABLE_RTC         ON 
#elif (MCU_SERIES == DEVICE_SERIES_MB9BF51X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9BF41X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9BF31X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9BF11X)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_R) || (MCU_PACKAGE == DEVICE_PACKAGE_N)
        #define PERIPHERAL_AVAILABLE_RTC         ON     
    #else
        #define PERIPHERAL_AVAILABLE_RTC         OFF
    #endif
#elif (MCU_SERIES == DEVICE_SERIES_MB9AF15X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9BF12X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9BF32X) || \
      (MCU_SERIES == DEVICE_SERIES_MB9BF52X)                
    #define PERIPHERAL_AVAILABLE_RTC         ON 
#elif (MCU_SERIES == DEVICE_SERIES_MB9AF31X) || (MCU_SERIES == DEVICE_SERIES_MB9AF11X)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_K)
        #define PERIPHERAL_AVAILABLE_RTC         ON 
    #else
        #define PERIPHERAL_AVAILABLE_RTC         OFF
    #endif
#else
    #define PERIPHERAL_AVAILABLE_RTC         OFF    
#endif
 
/*! \brief CEC */
#if (MCU_SERIES == DEVICE_SERIES_MB9AFB4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF34X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF14X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA3X)
    #define PERIPHERAL_AVAILABLE_RC         ON
#elif (MCU_SERIES == DEVICE_SERIES_MB9AF13X)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_N) || (MCU_PACKAGE == DEVICE_PACKAGE_M)
        #define PERIPHERAL_AVAILABLE_RC         ON
    #elif
        #define PERIPHERAL_AVAILABLE_RC         OFF
    #endif
#else
    #define PERIPHERAL_AVAILABLE_RC         OFF
#endif

/*! \brief RESET */
#define PERIPHERAL_AVAILABLE_RESET          ON

/*! \brief LCD */
#if (MCU_SERIES == DEVICE_SERIES_MB9AFB4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA3X)       
    #define PERIPHERAL_AVAILABLE_LCDC         ON
#else
    #define PERIPHERAL_AVAILABLE_LCDC         OFF      
#endif

/*! \brief Unique ID */  
#if (MCU_SERIES == DEVICE_SERIES_MB9AF14X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF34X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFA4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AFB4X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9AF15X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF12X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF32X) || \
    (MCU_SERIES == DEVICE_SERIES_MB9BF52X)   
    #define PERIPHERAL_AVAILABLE_UID          ON 
#else
    #define PERIPHERAL_AVAILABLE_UID          OFF
#endif 

/*! \brief USB, Ethernet not support  */  
#define PERIPHERAL_AVAILABLE_USB            OFF
#define PERIPHERAL_AVAILABLE_ETHER          OFF

/*!
 ******************************************************************************
 ** \brief Interrupt Type extraction 
 ** \attention Interrupt relocation function is not used for Type 4,5,6,8,9
 ******************************************************************************
 */
/*! \brief FM3 MCU interrupt Type A */
#define INT_TYPE_A                      0
/*! \brief FM3 MCU interrupt Type B */
#define INT_TYPE_B                      1
/*! \brief FM3 MCU interrupt Type C */
#define INT_TYPE_C                      2

#if (MCU_TYPE == DEVICE_TYPE0) || \
    (MCU_TYPE == DEVICE_TYPE1) || \
    (MCU_TYPE == DEVICE_TYPE2) || \
    (MCU_TYPE == DEVICE_TYPE4) || \
    (MCU_TYPE == DEVICE_TYPE5) || \
    (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE8) || \
    (MCU_TYPE == DEVICE_TYPE9)
    #define MCU_INT_TYPE       INT_TYPE_A
#elif (MCU_TYPE == DEVICE_TYPE3) || \
      (MCU_TYPE == DEVICE_TYPE7)
    #define MCU_INT_TYPE       INT_TYPE_C
#else
    #error MCU Type not found!
#endif

/*!
 ******************************************************************************
 ** \brief IRQ name definition for all type MCUs
 ******************************************************************************
 */
#if (MCU_INT_TYPE == INT_TYPE_A)
    #define CSV_IRQHandler(void)             IRQ0_Handler(void)  /* CSV */
    #define SWDT_IRQHandler(void)            IRQ1_Handler(void)  /* SW watchdog */
    #define LVD_IRQHandler(void)             IRQ2_Handler(void)  /* LVD */       
    #define MFT_WG_IRQHandler(void)          IRQ3_Handler(void)  /* MFT's WFG */   
    #define INT0_7_IRQHandler(void)          IRQ4_Handler(void)  /* Ext Int 0~7 */ 
    #define INT8_31_IRQHandler(void)         IRQ5_Handler(void)  /* Ext Int 8~31 */ 
    #define DT_QPRC_IRQHandler(void)         IRQ6_Handler(void)  /* DT & QRRC */  
    #define MFS0_8_RX_IRQHandler(void)       IRQ7_Handler(void)  /* MFS 0&8 RX */  
    #define MFS0_8_TX_IRQHandler(void)       IRQ8_Handler(void)  /* MFS 0&8 RX */ 
    #define MFS1_9_RX_IRQHandler(void)       IRQ9_Handler(void)  /* MFS 1&9 RX */ 
    #define MFS1_9_TX_IRQHandler(void)       IRQ10_Handler(void) /* MFS 1&9 RX */ 
    #define MFS2_10_RX_IRQHandler(void)      IRQ11_Handler(void) /* MFS 2&10 RX */ 
    #define MFS2_10_TX_IRQHandler(void)      IRQ12_Handler(void) /* MFS 2&10 RX */ 
    #define MFS3_11_RX_IRQHandler(void)      IRQ13_Handler(void) /* MFS 3&11 RX */ 
    #define MFS3_11_TX_IRQHandler(void)      IRQ14_Handler(void) /* MFS 3&11 RX */ 
    #define MFS4_12_RX_IRQHandler(void)      IRQ15_Handler(void) /* MFS 4&12 RX */ 
    #define MFS4_12_TX_IRQHandler(void)      IRQ16_Handler(void) /* MFS 4&12 RX */ 
    #define MFS5_13_RX_IRQHandler(void)      IRQ17_Handler(void) /* MFS 5&13 RX */ 
    #define MFS5_13_TX_IRQHandler(void)      IRQ18_Handler(void) /* MFS 5&13 RX */ 
    #define MFS6_14_RX_IRQHandler(void)      IRQ19_Handler(void) /* MFS 6&14 RX */ 
    #define MFS6_14_TX_IRQHandler(void)      IRQ20_Handler(void) /* MFS 6&14 RX */ 
    #define MFS7_15_RX_IRQHandler(void)      IRQ21_Handler(void) /* MFS 7&15 RX */ 
    #define MFS7_15_TX_IRQHandler(void)      IRQ22_Handler(void) /* MFS 7&15 RX */ 
    #define PPG_IRQHandler(void)             IRQ23_Handler(void) /* PPG */
    #define TIM_IRQHandler(void)             IRQ24_Handler(void) /* MOSC,SOSC,PLL,WC,RTC */
    #define ADC0_IRQHandler(void)            IRQ25_Handler(void) /* ADC0 */
    #define ADC1_IRQHandler(void)            IRQ26_Handler(void) /* ADC1 */
    #define ADC2_LCD_IRQHandler(void)        IRQ27_Handler(void) /* ADC2, LCD */
    #define MFT_FRT_IRQHandler(void)         IRQ28_Handler(void) /* MFT's FRT */
    #define MFT_IPC_IRQHandler(void)         IRQ29_Handler(void) /* MFT's IPC */
    #define MFT_OPC_IRQHandler(void)         IRQ30_Handler(void) /* MFT's OPC */
    #define BT0_7_IRQHandler(void)           IRQ31_Handler(void) /* BT0~7 */
    #define ETHER0_CAN0_IRQHandler(void)     IRQ32_Handler(void) /* CAN0, Ether0 */
    #define ETHER1_CAN1_IRQHandler(void)     IRQ33_Handler(void) /* CAN1, Ether1 */
    #define USB0F_IRQHandler(void)           IRQ34_Handler(void) /* USB0 Function */
    #define USB0_IRQHandler(void)            IRQ35_Handler(void) /* USB0 Function&Host */
    #define USB1F_HDMICEC0_IRQHandler(void)  IRQ36_Handler(void) /* USB1 Function, CEC0 */
    #define USB1_HDMICEC1_IRQHandler(void)   IRQ37_Handler(void) /* USB1 Function&Host, CEC1 */
    #define DMAC0_IRQHandler(void)           IRQ38_Handler(void) /* DMAC0 */
    #define DMAC1_IRQHandler(void)           IRQ39_Handler(void) /* DMAC1 */ 
    #define DMAC2_IRQHandler(void)           IRQ40_Handler(void) /* DMAC2 */ 
    #define DMAC3_IRQHandler(void)           IRQ41_Handler(void) /* DMAC3 */ 
    #define DMAC4_IRQHandler(void)           IRQ42_Handler(void) /* DMAC4 */ 
    #define DMAC5_IRQHandler(void)           IRQ43_Handler(void) /* DMAC5 */ 
    #define DMAC6_IRQHandler(void)           IRQ44_Handler(void) /* DMAC6 */
    #define DMAC7_IRQHandler(void)           IRQ45_Handler(void) /* DMAC7 */
    #define BT8_15_IRQHandler(void)          IRQ46_Handler(void) /* BT8~15 */
    #define FLASH_IRQHandler(void)           IRQ47_Handler(void) /* Work Flash */
#elif (MCU_INT_TYPE == INT_TYPE_C)
    #define CSV_IRQHandler(void)              IRQ0_Handler(void)  /* A <- */
    #define SWDT_IRQHandler(void)             IRQ1_Handler(void)  /* A <- */
    #define LVD_IRQHandler(void)              IRQ2_Handler(void)  /* A <- */       
    #define MFT_WG_IRQHandler(void)           IRQ3_Handler(void)  /* A <- */
    #define INT0_7_IRQHandler(void)           IRQ4_Handler(void)  /* A <- */
    #define INT8_31_IRQHandler(void)          IRQ5_Handler(void)  /* A <- */
    #define MFS0_8_RX_IRQHandler(void)        IRQ6_Handler(void)    /* A <- */
    #define MFS0_8_TX_IRQHandler(void)        IRQ7_Handler(void)    /* A <- */
    #define MFS1_9_RX_IRQHandler(void)        IRQ8_Handler(void)    /* A <- */
    #define MFS1_9_TX_IRQHandler(void)        IRQ9_Handler(void)    /* A <- */
    #define MFS2_10_RX_IRQHandler(void)       IRQ10_Handler(void)  /* A <- */
    #define MFS2_10_TX_IRQHandler(void)       IRQ11_Handler(void)  /* A <- */
    #define MFS3_11_RX_IRQHandler(void)       IRQ12_Handler(void)  /* A <- */
    #define MFS3_11_TX_IRQHandler(void)       IRQ13_Handler(void)  /* A <- */
    #define MFS4_12_RX_IRQHandler(void)       IRQ14_Handler(void)  /* A <- */
    #define MFS4_12_TX_IRQHandler(void)       IRQ15_Handler(void)  /* A <- */
    #define MFS5_13_RX_IRQHandler(void)       IRQ16_Handler(void)  /* A <- */
    #define MFS5_13_TX_IRQHandler(void)       IRQ17_Handler(void)  /* A <- */
    #define MFS6_14_RX_IRQHandler(void)       IRQ18_Handler(void)  /* A <- */
    #define MFS6_14_TX_IRQHandler(void)       IRQ19_Handler(void)  /* A <- */
    #define MFS7_15_RX_IRQHandler(void)       IRQ20_Handler(void)  /* A <- */
    #define MFS7_15_TX_IRQHandler(void)       IRQ21_Handler(void)  /* A <- */
    #define PPG_IRQHandler(void)              IRQ22_Handler(void)  /* A <- */
    #define TIM_IRQHandler(void)              IRQ23_Handler(void)  /* A <- */
    #define ADC0_IRQHandler(void)             IRQ24_Handler(void)  /* A <- */
    #define MFT_FRT_IRQHandler(void)          IRQ25_Handler(void)  /* A <- */
    #define MFT_IPC_IRQHandler(void)          IRQ26_Handler(void)  /* A <- */
    #define MFT_OPC_IRQHandler(void)          IRQ27_Handler(void)  /* A <- */
    #define BT0_7_IRQHandler(void)            IRQ28_Handler(void)  /* A <- */      
    #define ADC2_LCD_IRQHandler(void)         IRQ29_Handler(void)  /* A <- */
    #define USB1F_HDMICEC0_IRQHandler(void)   IRQ30_Handler(void)  /* A <- */
    #define USB1_HDMICEC1_IRQHandler(void)    IRQ31_Handler(void)  /* A <- */
#else
    #error Interrupt Type not found!
#endif

#endif /* _DEV_CONFIG_H_ */
