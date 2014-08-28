/*!
 ******************************************************************************
 **
 ** \file mfs_fm3.h
 **
 ** \brief MFS drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-26
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
 
#ifndef _MFS_FM3_H_
#define _MFS_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_MFS == OFF)
    #if (PERIPHERAL_ENABLE_MFS_UART == ON) || \
        (PERIPHERAL_ENABLE_MFS_CSIO == ON) || \
        (PERIPHERAL_ENABLE_MFS_I2C == ON)  || \
        (PERIPHERAL_ENABLE_MFS_LIN == ON)  
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_MFS_UART == ON) || \
    (PERIPHERAL_ENABLE_MFS_CSIO == ON) || \
    (PERIPHERAL_ENABLE_MFS_I2C == ON)  || \
    (PERIPHERAL_ENABLE_MFS_LIN == ON)  

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#if (MCU_TYPE == DEVICE_TYPE0) || \
    (MCU_TYPE == DEVICE_TYPE1) || \
    (MCU_TYPE == DEVICE_TYPE2) || \
    (MCU_TYPE == DEVICE_TYPE4) || \
    (MCU_TYPE == DEVICE_TYPE6) || \
    (MCU_TYPE == DEVICE_TYPE7)
    #define MFS_MAX_CH                  (8)
#elif (MCU_TYPE == DEVICE_TYPE3)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_K)
        #define MFS_MAX_CH              (6)  /* Actually only 4 Ch, Ch.0,1,3,5 */
    #else
        #define MFS_MAX_CH              (8)
    #endif
#elif (MCU_TYPE == DEVICE_TYPE5)
    #define MFS_MAX_CH                  (6)  /* Actually only 4 Ch, Ch.0,1,3,5 */
#elif (MCU_TYPE == DEVICE_TYPE8)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_R)
        #define MFS_MAX_CH                  (16)
    #elif (MCU_PACKAGE == DEVICE_PACKAGE_N)
        #define MFS_MAX_CH                  (14)    
    #else
        #define MFS_MAX_CH                  (12)  /* Actually only 10 Ch, Ch.0~7,10,11 */   
    #endif
#elif (MCU_TYPE == DEVICE_TYPE9)
    #if (MCU_PACKAGE == DEVICE_PACKAGE_L) || (MCU_PACKAGE == DEVICE_PACKAGE_M)
        #define MFS_MAX_CH                  (8)
    #else
        #define MFS_MAX_CH                  (6) /* Actually only 4 Ch, Ch.0,1,3,5 */
    #endif
#endif

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief FIFO selection 
 ******************************************************************************
 */
typedef enum
{
    MFS_FIFO1TX_FIFO2RX = 0,    //!< Transmit FIFO:FIFO1; Receive FIFO:FIFO2
    MFS_FIFO2TX_FIFO1RX,        //!< Transmit FIFO:FIFO2; Receive FIFO:FIFO1
}MFS_FIFOSelectionT;

/*!
******************************************************************************
** \brief MFS callback.
******************************************************************************
*/
typedef void (*MFS_CallbackT) (void);


#if (MFS_MAX_CH == 4)
    /* \brief ISR number. */
    typedef enum 
    {
        MFS_IntCh0RX = 0, 
        MFS_IntCh0TX, 
        MFS_IntCh0Status,
        MFS_IntCh1RX, 
        MFS_IntCh1TX, 
        MFS_IntCh1Status,
        MFS_IntCh2RX, 
        MFS_IntCh2TX, 
        MFS_IntCh2Status,
        MFS_IntCh3RX, 
        MFS_IntCh3TX, 
        MFS_IntCh3Status,
    } MFS_IntNumT;
    /* \brief MFS Channel number */
    typedef enum 
    {
        MFS_Ch0 = 0, 
        MFS_Ch1,
        MFS_Ch2,
        MFS_Ch3,
    } MFS_ChannelNumT;
#elif (MFS_MAX_CH == 6)
    /* \brief ISR number. */
    typedef enum 
    {
        MFS_IntCh0RX = 0, 
        MFS_IntCh0TX, 
        MFS_IntCh0Status,
        MFS_IntCh1RX, 
        MFS_IntCh1TX, 
        MFS_IntCh1Status,
        MFS_IntCh2RX, 
        MFS_IntCh2TX, 
        MFS_IntCh2Status,
        MFS_IntCh3RX, 
        MFS_IntCh3TX, 
        MFS_IntCh3Status,
        MFS_IntCh4RX, 
        MFS_IntCh4TX,  
        MFS_IntCh4Status,
        MFS_IntCh5RX, 
        MFS_IntCh5TX, 
        MFS_IntCh5Status,
    } MFS_IntNumT;
    /* \brief MFS Channel number */
    typedef enum 
    {
        MFS_Ch0 = 0, 
        MFS_Ch1,
        MFS_Ch2,
        MFS_Ch3,
        MFS_Ch4,
        MFS_Ch5,
    } MFS_ChannelNumT;
#elif (MFS_MAX_CH == 8)
    /* \brief ISR number. */
    typedef enum 
    {
        MFS_IntCh0RX = 0, 
        MFS_IntCh0TX, 
        MFS_IntCh0Status,
        MFS_IntCh1RX, 
        MFS_IntCh1TX, 
        MFS_IntCh1Status,
        MFS_IntCh2RX, 
        MFS_IntCh2TX, 
        MFS_IntCh2Status,
        MFS_IntCh3RX, 
        MFS_IntCh3TX, 
        MFS_IntCh3Status,
        MFS_IntCh4RX, 
        MFS_IntCh4TX,  
        MFS_IntCh4Status,
        MFS_IntCh5RX, 
        MFS_IntCh5TX, 
        MFS_IntCh5Status,
        MFS_IntCh6RX, 
        MFS_IntCh6TX,
        MFS_IntCh6Status,
        MFS_IntCh7RX, 
        MFS_IntCh7TX, 
        MFS_IntCh7Status,
    } MFS_IntNumT;
    /* \brief MFS Channel number */
    typedef enum 
    {
        MFS_Ch0 = 0, 
        MFS_Ch1,
        MFS_Ch2,
        MFS_Ch3,
        MFS_Ch4,
        MFS_Ch5,
        MFS_Ch6,
        MFS_Ch7,
    } MFS_ChannelNumT;
#elif (MFS_MAX_CH == 10)
    /* \brief ISR number. */
    typedef enum 
    {
        MFS_IntCh0RX = 0, 
        MFS_IntCh0TX, 
        MFS_IntCh0Status,
        MFS_IntCh1RX, 
        MFS_IntCh1TX,
        MFS_IntCh1Status,
        MFS_IntCh2RX, 
        MFS_IntCh2TX,
        MFS_IntCh2Status,
        MFS_IntCh3RX, 
        MFS_IntCh3TX,
        MFS_IntCh3Status,
        MFS_IntCh4RX, 
        MFS_IntCh4TX,
        MFS_IntCh4Status,
        MFS_IntCh5RX, 
        MFS_IntCh5TX,
        MFS_IntCh5Status,
        MFS_IntCh6RX, 
        MFS_IntCh6TX,
        MFS_IntCh6Status,
        MFS_IntCh7RX, 
        MFS_IntCh7TX,
        MFS_IntCh7Status,
        MFS_IntCh8RX, 
        MFS_IntCh8TX,
        MFS_IntCh8Status,
        MFS_IntCh9RX, 
        MFS_IntCh9TX,
        MFS_IntCh9Status,
    } MFS_IntNumT;
    /* \brief MFS Channel number */
    typedef enum 
    {
        MFS_Ch0 = 0, 
        MFS_Ch1,
        MFS_Ch2,
        MFS_Ch3,
        MFS_Ch4,
        MFS_Ch5,
        MFS_Ch6,
        MFS_Ch7,
        MFS_Ch8,
        MFS_Ch9,
    } MFS_ChannelNumT;    
#elif (MFS_MAX_CH == 14)
    /* \brief ISR number. */
    typedef enum 
    {
        MFS_IntCh0RX = 0, 
        MFS_IntCh0TX, 
        MFS_IntCh0Status,
        MFS_IntCh1RX, 
        MFS_IntCh1TX,
        MFS_IntCh1Status,
        MFS_IntCh2RX, 
        MFS_IntCh2TX,
        MFS_IntCh2Status,
        MFS_IntCh3RX, 
        MFS_IntCh3TX,
        MFS_IntCh3Status,
        MFS_IntCh4RX, 
        MFS_IntCh4TX,
        MFS_IntCh4Status,
        MFS_IntCh5RX, 
        MFS_IntCh5TX,
        MFS_IntCh5Status,
        MFS_IntCh6RX, 
        MFS_IntCh6TX,
        MFS_IntCh6Status,
        MFS_IntCh7RX, 
        MFS_IntCh7TX,
        MFS_IntCh7Status,
        MFS_IntCh8RX, 
        MFS_IntCh8TX,
        MFS_IntCh8Status,
        MFS_IntCh9RX, 
        MFS_IntCh9TX,
        MFS_IntCh9Status,
        MFS_IntCh10RX, 
        MFS_IntCh10TX,
        MFS_IntCh10Status,
        MFS_IntCh11RX, 
        MFS_IntCh11TX,
        MFS_IntCh11Status,
        MFS_IntCh12RX, 
        MFS_IntCh12TX,
        MFS_IntCh12Status,
        MFS_IntCh13RX, 
        MFS_IntCh13TX,
        MFS_IntCh13Status,
    } MFS_IntNumT;
    /* \brief MFS Channel number */
    typedef enum 
    {
        MFS_Ch0 = 0, 
        MFS_Ch1,
        MFS_Ch2,
        MFS_Ch3,
        MFS_Ch4,
        MFS_Ch5,
        MFS_Ch6,
        MFS_Ch7,
        MFS_Ch8,
        MFS_Ch9,
        MFS_Ch10,
        MFS_Ch11,
        MFS_Ch12,
        MFS_Ch13,
    } MFS_ChannelNumT; 
#elif (MFS_MAX_CH == 16)
    /* \brief ISR number. */
    typedef enum 
    {
        MFS_IntCh0RX = 0, 
        MFS_IntCh0TX, 
        MFS_IntCh0Status,
        MFS_IntCh1RX, 
        MFS_IntCh1TX,
        MFS_IntCh1Status,
        MFS_IntCh2RX, 
        MFS_IntCh2TX,
        MFS_IntCh2Status,
        MFS_IntCh3RX, 
        MFS_IntCh3TX,
        MFS_IntCh3Status,
        MFS_IntCh4RX, 
        MFS_IntCh4TX,
        MFS_IntCh4Status,
        MFS_IntCh5RX, 
        MFS_IntCh5TX,
        MFS_IntCh5Status,
        MFS_IntCh6RX, 
        MFS_IntCh6TX,
        MFS_IntCh6Status,
        MFS_IntCh7RX, 
        MFS_IntCh7TX,
        MFS_IntCh7Status,
        MFS_IntCh8RX, 
        MFS_IntCh8TX,
        MFS_IntCh8Status,
        MFS_IntCh9RX, 
        MFS_IntCh9TX,
        MFS_IntCh9Status,
        MFS_IntCh10RX, 
        MFS_IntCh10TX,
        MFS_IntCh10Status,
        MFS_IntCh11RX, 
        MFS_IntCh11TX,
        MFS_IntCh11Status,
        MFS_IntCh12RX, 
        MFS_IntCh12TX,
        MFS_IntCh12Status,
        MFS_IntCh13RX, 
        MFS_IntCh13TX,
        MFS_IntCh13Status,
        MFS_IntCh14RX, 
        MFS_IntCh14TX,
        MFS_IntCh14Status,
        MFS_IntCh15RX, 
        MFS_IntCh15TX,
        MFS_IntCh15Status,
    } MFS_IntNumT;
    /* \brief MFS Channel number */
    typedef enum 
    {
        MFS_Ch0 = 0, 
        MFS_Ch1,
        MFS_Ch2,
        MFS_Ch3,
        MFS_Ch4,
        MFS_Ch5,
        MFS_Ch6,
        MFS_Ch7,
        MFS_Ch8,
        MFS_Ch9,
        MFS_Ch10,
        MFS_Ch11,
        MFS_Ch12,
        MFS_Ch13,
        MFS_Ch14,
        MFS_Ch15,
    } MFS_ChannelNumT;     
#endif    
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif /* _mfs_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
