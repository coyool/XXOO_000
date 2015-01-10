/*!
 ******************************************************************************
 **
 ** \file pdl_user.h
 **
 ** \brief User configuration file for each example project
 **
 ** \author FSLA AE Team 
 **
 ** \version V0.10
 **
 ** \date 2012-01-19
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
#ifndef _PDL_USER_H_
#define _PDL_USER_H_

#include "pdl.h"
/**
 ******************************************************************************
 ** PDL resource enable configuration
 ******************************************************************************/
/*! \brief ADC activity configuration */
#define PERIPHERAL_ENABLE_ADC                           ON

/*! \brief Base timer activity configuration */
#define PERIPHERAL_ENABLE_BT                            ON

/*! \brief Clock activity configuration */
#define PERIPHERAL_ENABLE_CLOCK                         ON

/*! \brief CR activity configuration */
#define PERIPHERAL_ENABLE_CR                            OFF 

/*! \brief CRC activity configuration */
#define PERIPHERAL_ENABLE_CRC                           OFF 

/*! \brief CSV activity configuration */
#define PERIPHERAL_ENABLE_CSV                           OFF 

/*! \brief CAN activity configuration */
#define PERIPHERAL_ENABLE_CAN                           OFF 

/*! \brief DMAC activity configuration */
#define PERIPHERAL_ENABLE_DMAC                          OFF  

/*! \brief DT activity configuration */
#define PERIPHERAL_ENABLE_DT                            OFF  

/*! \brief DAC activity configuration */
#define PERIPHERAL_ENABLE_DAC                           OFF

/*! \brief Dual Flash activity configuration */
#define PERIPHERAL_ENABLE_DUALFLASH                     OFF

/*! \brief External interrupt activity configuration */
#define PERIPHERAL_ENABLE_EXTINT                        ON

/*! \brief External bus activity configuration */
#define PERIPHERAL_ENABLE_EXTBUS                        OFF

/*! \brief Ethernet activity configuration */
#define PERIPHERAL_ENABLE_ETHER                         OFF

/*! \brief IO activity configuration */
#define PERIPHERAL_ENABLE_IO                            ON

/*! \brief Low power consumption mode activity configuration */
#define PERIPHERAL_ENABLE_LPCM                          ON

/*! \brief Low power detection activity configuration */
#define PERIPHERAL_ENABLE_LVD                           ON

/*! \brief LCD controller activity configuration */
#define PERIPHERAL_ENABLE_LCDC                          ON

/*! \brief UART activity configuration */
#define PERIPHERAL_ENABLE_MFS_UART                      ON

/*! \brief CSIO activity configuration */
#define PERIPHERAL_ENABLE_MFS_CSIO                      OFF

/*! \brief I2C activity configuration */
#define PERIPHERAL_ENABLE_MFS_I2C                       OFF

/*! \brief LIN activity configuration */
#define PERIPHERAL_ENABLE_MFS_LIN                       OFF

/*! \brief MFT's free run timer activity configuration */
#define PERIPHERAL_ENABLE_MFT_FRT                       ON

/*! \brief MFT's output compare unit activity configuration */
#define PERIPHERAL_ENABLE_MFT_OCU                       OFF

/*! \brief MFT's output compare unit activity configuration */
#define PERIPHERAL_ENABLE_MFT_WFG                       OFF

/*! \brief MFT's input capture unit activity configuration */
#define PERIPHERAL_ENABLE_MFT_ICU                       OFF

/*! \brief MFT's ADC compare activity configuration */
#define PERIPHERAL_ENABLE_MFT_ADCMP                     OFF

/*! \brief Main Flash activity configuration */
#define PERIPHERAL_ENABLE_MAINFLASH                     ON

/*! \brief NMI activity configuration */
#define PERIPHERAL_ENABLE_NMI                           OFF

/*! \brief PPG activity configuration */
#define PERIPHERAL_ENABLE_PPG                           OFF

/*! \brief QPRC activity configuration */
#define PERIPHERAL_ENABLE_QPRC                          OFF

/*! \brief Remote control activity configuration */
#define PERIPHERAL_ENABLE_RC                            OFF

/*! \brief RTC activity configuration */
#define PERIPHERAL_ENABLE_RTC                           ON

/*! \brief RTC activity configuration */
#define PERIPHERAL_ENABLE_RESET                         ON

/*! \brief Unique ID activity configuration */
#define PERIPHERAL_ENABLE_UID                           OFF

/*! \brief USB activity configuration */
#define PERIPHERAL_ENABLE_USB                           OFF

/*! \brief Watch counter activity configuration */
#define PERIPHERAL_ENABLE_WC                            OFF

/*! \brief Watchdog activity configuration */
#define PERIPHERAL_ENABLE_WD                            ON

/*! \brief Work Flash activity configuration */
#define PERIPHERAL_ENABLE_WORKFLASH                     OFF

/**
 ******************************************************************************
 ** Debug mode switch
 ******************************************************************************/
//#define DEBUG_MODE

/**
 ******************************************************************************
 ** Hardware board selection
 ******************************************************************************/
#if !defined (USE_MB9BF506R_EVB) && !defined (USE_MB9AF314L_EVB) &&  !defined (USE_MB9BF618S_EVB) \
   &&  !defined (USE_MB9AFA32N_EVB) &&  !defined (USE_LQFP32_ADPT) && !defined (USE_LQFP48_ADPT) \
   && !defined (USE_LQFP64_ADPT) &&  !defined (USE_LQFP80_ADPT) &&  !defined (USE_LQFP100_ADPT) \
   && !defined (USE_LQFP120_ADPT) &&  !defined (USE_LQFP144_ADPT) && !defined (USE_LQFP176_ADPT)
//#define USE_MB9BF506R_EVB
//#define USE_MB9AF314L_EVB
//#define USE_MB9AF618S_EVB
//#define USE_MB9AFA32N_EVB
//#define USE_LQFP32_ADPT
//#define USE_LQFP48_ADPT
//#define USE_LQFP64_ADPT
//#define USE_LQFP80_ADPT
//#define USE_LQFP100_ADPT
//#define USE_LQFP120_ADPT
//#define USE_LQFP144_ADPT
//#define USE_LQFP176_ADPT
#endif 

#endif
