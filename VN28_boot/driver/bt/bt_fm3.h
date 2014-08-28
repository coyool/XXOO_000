/*!
 ******************************************************************************
 **
 ** \file basetimer_fm3.h
 **
 ** \brief Base timer module driver
 ** 
 **
 ** \author 
 **
 ** \version V0.01
 **
 ** \date 2011-11-29
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
 
#ifndef _BT_FM3_H_
#define _BT_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_BT == OFF)
    #if (PERIPHERAL_ENABLE_BT == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif


#if (PERIPHERAL_ENABLE_BT == ON) && (PERIPHERAL_AVAILABLE_BT == ON)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/*! \brief Base timer total channel number */
#if (MCU_TYPE == DEVICE_TYPE2) || (MCU_TYPE == DEVICE_TYPE8) 
    #define BT_CH_NUM                       16
#else
    #define BT_CH_NUM                       8
#endif

#if (MCU_TYPE == DEVICE_TYPE2) || (MCU_TYPE == DEVICE_TYPE8) 
    /*! \brief Basetimer channel 0 */
    #define BT_CH_0                         0
    /*! \brief Basetimer channel 1 */
    #define BT_CH_1                         1
    /*! \brief Basetimer channel 2 */
    #define BT_CH_2                         2
    /*! \brief Basetimer channel 3 */
    #define BT_CH_3                         3
    /*! \brief Basetimer channel 4 */
    #define BT_CH_4                         4
    /*! \brief Basetimer channel 5 */
    #define BT_CH_5                         5
    /*! \brief Basetimer channel 6 */
    #define BT_CH_6                         6
    /*! \brief Basetimer channel 7 */
    #define BT_CH_7                         7
    /*! \brief Basetimer channel 8 */
    #define BT_CH_8                         8
    /*! \brief Basetimer channel 9 */
    #define BT_CH_9                         9
    /*! \brief Basetimer channel 10 */
    #define BT_CH_10                        10
    /*! \brief Basetimer channel 11 */
    #define BT_CH_11                        11
    /*! \brief Basetimer channel 12 */
    #define BT_CH_12                        12
    /*! \brief Basetimer channel 13 */
    #define BT_CH_13                        13
    /*! \brief Basetimer channel 14 */
    #define BT_CH_14                        14
    /*! \brief Basetimer channel 15 */
    #define BT_CH_15                        15
    
    /*! \brief Basetimer channel check list */
    #define IS_BT_CH(CH) (((CH) == BT_CH_0) || \
                          ((CH) == BT_CH_1) || \
                          ((CH) == BT_CH_2) || \
                          ((CH) == BT_CH_3) || \
                          ((CH) == BT_CH_4) || \
                          ((CH) == BT_CH_5) || \
                          ((CH) == BT_CH_6) || \
                          ((CH) == BT_CH_7) || \
                          ((CH) == BT_CH_8) || \
                          ((CH) == BT_CH_9) || \
                          ((CH) == BT_CH_10) || \
                          ((CH) == BT_CH_11) || \
                          ((CH) == BT_CH_12) || \
                          ((CH) == BT_CH_13) || \
                          ((CH) == BT_CH_14) || \
                          ((CH) == BT_CH_15))
#else
    /*! \brief Basetimer channel 0 */
    #define BT_CH_0                         0
    /*! \brief Basetimer channel 1 */
    #define BT_CH_1                         1
    /*! \brief Basetimer channel 2 */
    #define BT_CH_2                         2
    /*! \brief Basetimer channel 3 */
    #define BT_CH_3                         3
    /*! \brief Basetimer channel 4 */
    #define BT_CH_4                         4
    /*! \brief Basetimer channel 5 */
    #define BT_CH_5                         5
    /*! \brief Basetimer channel 6 */
    #define BT_CH_6                         6
    /*! \brief Basetimer channel 7 */
    #define BT_CH_7                         7
    
    /*! \brief Basetimer channel check list */
    #define IS_BT_CH(CH) (((CH) == BT_CH_0) || \
                          ((CH) == BT_CH_1) || \
                          ((CH) == BT_CH_2) || \
                          ((CH) == BT_CH_3) || \
                          ((CH) == BT_CH_4) || \
                          ((CH) == BT_CH_5) || \
                          ((CH) == BT_CH_6) || \
                          ((CH) == BT_CH_7))
#endif



/* Base timer IO mode selection*/
/*! \brief I/O mode 0 (Standard 16-bit timer mode) */
#define BT_IO_MODE_0                     0
/*! \brief I/O mode 1 (timer full mode) */
#define BT_IO_MODE_1                     1
/*! \brief I/O mode 2 (Shared external trigger mode) */
#define BT_IO_MODE_2                     2
/*! \brief I/O mode 3 (Shared channel signal trigger mode) */
#define BT_IO_MODE_3                     3
/*! \brief I/O mode 4 (Timer start/stop mode) */
#define BT_IO_MODE_4                     4
/*! \brief I/O mode 5 (Software-based simultaneous startup mode) */
#define BT_IO_MODE_5                     5
/*! \brief I/O mode 6 (Software-based startup and timer start/stop mode) */
#define BT_IO_MODE_6                     6
/*! \brief I/O mode 7 (Timer start mode) */
#define BT_IO_MODE_7                     7
/*! \brief I/O mode 8 (Shared channel signal trigger and timer start/stop mode) */
#define BT_IO_MODE_8                     8

/*! \brief Basetimer I/O mode check list */
#define IS_BT_IOMODE(IOMODE) (((IOMODE) == BT_IO_MODE_0) || \
                              ((IOMODE) == BT_IO_MODE_1) || \
                              ((IOMODE) == BT_IO_MODE_2) || \
                              ((IOMODE) == BT_IO_MODE_3) || \
                              ((IOMODE) == BT_IO_MODE_4) || \
                              ((IOMODE) == BT_IO_MODE_5) || \
                              ((IOMODE) == BT_IO_MODE_6) || \
                              ((IOMODE) == BT_IO_MODE_7) || \
                              ((IOMODE) == BT_IO_MODE_8))


/* Base timer function selection */
/*! \brief Base timer reset mode */
#define BT_RST_MODE                     0
/*! \brief Base timer PWM function */
#define BT_PWM_MODE                     1
/*! \brief Base timer PPG function */
#define BT_PPG_MODE                     2
/*! \brief Base timer reload timer function */
#define BT_RT_MODE                      3
/*! \brief Base timer PWC function */
#define BT_PWC_MODE                     4

/* Base timer count clock selection */
/*! \brief PCLK */
#define BT_CLK_DIV_1                    0
/*! \brief PCLK/4 */
#define BT_CLK_DIV_4                    1
/*! \brief PCLK/16 */
#define BT_CLK_DIV_16                   2
/*! \brief PCLK/128 */
#define BT_CLK_DIV_128                  3
/*! \brief PCLK/256 */
#define BT_CLK_DIV_256                  4
/*! \brief External clock (rising edge event) */
#define BT_CLK_RIS_EDGE                 5
/*! \brief External clock (falling edge event) */
#define BT_CLK_FALL_EDGE                6
/*! \brief External clock (both edge event) */
#define BT_CLK_BOTH_EDGE                7
/*! \brief PCLK/512 */
#define BT_CLK_DIV_512                  8
/*! \brief PCLK/1024 */
#define BT_CLK_DIV_1024                 9
/*! \brief PCLK/2048 */
#define BT_CLK_DIV_2048                 10

/*! \brief Basetimer Clock divider check list */
#define IS_BT_CLOCK(CLOCK) (((CLOCK) == BT_CLK_DIV_1) || \
                            ((CLOCK) == BT_CLK_DIV_4) || \
                            ((CLOCK) == BT_CLK_DIV_16) || \
                            ((CLOCK) == BT_CLK_DIV_128) || \
                            ((CLOCK) == BT_CLK_DIV_256) || \
                            ((CLOCK) == BT_CLK_RIS_EDGE) || \
                            ((CLOCK) == BT_CLK_FALL_EDGE) || \
                            ((CLOCK) == BT_CLK_BOTH_EDGE) || \
                            ((CLOCK) == BT_CLK_DIV_512) || \
                            ((CLOCK) == BT_CLK_DIV_1024) || \
                            ((CLOCK) == BT_CLK_DIV_2048))


/* Base timer restart enable */
/*! \brief Restart disabled */
#define BT_RESTART_DISABLE              0
/*! \brief Restart enabled */
#define BT_RESTART_ENABLE               1

/*! \brief Basetimer restart bit check list */
#define IS_BT_RESTART(RESTART)  (((RESTART) == BT_RESTART_DISABLE) || \
                                ((RESTART) == BT_RESTART_ENABLE))

/* Base timer output mask */
/*! \brief Normal output */
#define BT_MASK_DISABLE                 0
/*! \brief Fixed to LOW output */
#define BT_MASK_ENABLE                  1

/*! \brief Basetimer output maske bit check list */
#define IS_BT_OUTPUT_MASK(MASK) (((MASK) == BT_MASK_DISABLE) || \
                                 ((MASK) == BT_MASK_ENABLE))

/* Base timer Trigger input edge selection (PWM,PPG,Reload timer) */
/*! \brief Trigger input disabled */
#define BT_TRG_DISABLE                  0
/*! \brief Rising edge */
#define BT_TRG_EDGE_RIS                 1
/*! \brief Falling edge */
#define BT_TRG_EDGE_FALL                2
/*! \brief Both edges */
#define BT_TRG_EDGE_BOTH                3

/*! \brief Basetimer Trigger input edge check list */
#define IS_BT_TRG_EDGE(EDGE) (((EDGE) == BT_TRG_DISABLE) || \
                              ((EDGE) == BT_TRG_EDGE_RIS) || \
                              ((EDGE) == BT_TRG_EDGE_FALL) || \
                              ((EDGE) == BT_TRG_EDGE_BOTH))

/* Base timer PWC measurement edge selection (PWC)*/
/*! \brief HIGH pulse width measurement (¡ü to ¡ý) */
#define BT_PWC_EDGE_HIGH                0
/*! \brief Cycle measurement between rising edges (¡ü to ¡ü) */
#define BT_PWC_EDGE_RIS                 1
/*! \brief Cycle measurement between falling edges (¡ý to ¡ý) */
#define BT_PWC_EDGE_FALL                2
/*! \brief Pulse width measurement between all edges (¡ü or ¡ý to ¡ý or ¡ü) */
#define BT_PWC_EDGE_BOTH                3
/*! \brief LOW pulse width measurement (¡ý to ¡ü) */
#define BT_PWC_EDGE_LOW                 4

/*! \brief Basetimer PWC measurement edge check list */
#define IS_BT_PWC_EDGE(EDGE) (((EDGE) == BT_PWC_EDGE_HIGH) || \
                              ((EDGE) == BT_PWC_EDGE_RIS) || \
                              ((EDGE) == BT_PWC_EDGE_FALL) || \
                              ((EDGE) == BT_PWC_EDGE_BOTH) || \
                              ((EDGE) == BT_PWC_EDGE_LOW))

/* Base timer Output polarity specification */
/*! \brief Normal polarity */
#define BT_POLARITY_NORMAL              0
/*! \brief Inverted polarity */
#define BT_POLARITY_INVERTED            1

/*! \brief Basetimer Output polarity bit check list */
#define IS_BT_POLARITY(POLARITY) (((POLARITY) == BT_POLARITY_NORMAL) || \
                                  ((POLARITY) == BT_POLARITY_INVERTED))

/* Base timer mode selection */
/*! \brief Continuous operation */
#define BT_MODE_CONTINUOUS              0
/*! \brief One-shot operation */
#define BT_MODE_ONESHOT                 1

/*! \brief Basetimer mode check list */
#define IS_BT_MODE(MODE) (((MODE) == BT_MODE_CONTINUOUS) || \
                          ((MODE) == BT_MODE_ONESHOT))

/* 32-bit timer selection for RT/PWC */
/*! \brief 16-bit timer mode */
#define BT_16BIT_TIMER                  0
/*! \brief 32-bit timer mode */
#define BT_32BIT_TIMER                  1

/*! \brief Basetimer 32-bit timer selection bit check list */
#define IS_BT_BIT_TIMER(TIMER) (((TIMER) == BT_16BIT_TIMER) || \
                                ((TIMER) == BT_32BIT_TIMER))

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*! \brief RT trigger interrupt callback function */
typedef void BT_RTTrigIntCallback(void);
/*! \brief RT underflow interrupt callback function */
typedef void BT_RTUFIntCallback(void);

/*! \brief PWM trigger interrupt callback function */
typedef void BT_PWMTrigIntCallback(void);
/*! \brief PWM underflow interrupt callback function */
typedef void BT_PWMUFIntCallback(void);
/*! \brief PWM duty match interrupt callback function */
typedef void BT_PWMDMIntCallback(void);

/*! \brief PPG trigger interrupt callback function */
typedef void BT_PPGTrigIntCallback(void);
/*! \brief PPG underflow interrupt callback function */
typedef void BT_PPGUFIntCallback(void);

/*! \brief PWC overflow interrupt callback function */
typedef void BT_PWCOFIntCallback(void);
/*! \brief PWC measure complete interrupt callback function */
typedef void BT_PWCMCIntCallback(void);


/*!
 ******************************************************************************
 ** \brief PWM timer initialization data structure
 ******************************************************************************
 */
typedef struct 
{    
    uint16_t        Cycle;       //!< PWM Cycle Settng    
    uint16_t        Duty;        //!< PWM Duty Setting    
    uint16_t        Clock;       //!< Clock selection    
    uint16_t        Restart;     //!< Restart enable   
    uint16_t        OutputMask;  //!< Output level of PWM output waveforms    	
    uint16_t        InputEdge;   //!< Trigger input edge selection    
    uint16_t        Polarity;    //!< Output polarity specification    
    uint16_t        Mode;        //!< Selects continuous pulse output or one-shot pulse output  
} BT_PWMInitRegT;  


/*!
 ******************************************************************************
 ** \brief PPG timer initialization data structure
 ******************************************************************************
 */
typedef struct 
{  
    uint16_t        LowWidth;       //!< LOW width of PPG output waveforms */    
    uint16_t        HighWidth;      //!< HIGH width of PPG output waveforms */     
    uint16_t        Clock;          //!< Count clock selection */
    uint16_t        Restart;        //!< Restart enable */
    uint16_t        OutputMask;     //!< Output level of PWM output waveforms */ 
    uint16_t        InputEdge;      //!< Trigger input edge selection */ 
    uint16_t        Polarity;       //!< Output polarity specification */
    uint16_t        Mode;           //!< Selects continuous pulse output or one-shot pulse output */ 
} BT_PPGInitRegT;  

/*!
 ******************************************************************************
 ** \brief PWC timer initialization data structure
 ******************************************************************************
 */
typedef struct
{
        
    uint16_t        Clock;          //!< Count clock selection */
    uint16_t        MeasureEdge;    //!< Measurement edge selection */
    uint16_t        TimerMode;      //!< 32-bit timer selection */ 
    uint16_t        Mode;           //!< Selects Continuous measurement mode or One-shot measurement mode */ 
} BT_PWCInitRegT; 

/*!
 ******************************************************************************
 ** \brief Reload timer initialization data structure
 ******************************************************************************
 */
typedef struct
{    
    uint32_t        Cycle;          //!< PWM Cycle Settng    
    uint16_t        Clock;          //!< Clock selection    
    uint16_t        InputEdge;      //!< Trigger input edge selection  
    uint16_t        TimerMode;      //!< 32-bit timer selection */ 
    uint16_t        Polarity;       //!< Output polarity specification    
    uint16_t        Mode;           //!< Selects continuous pulse output or one-shot pulse output  
} BT_RTInitRegT;  

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*----------------- 0. IO mode ----------------*/
void BT_SetIOMode(uint8_t Ch, uint8_t IOMode);  
/*----------------- 1. RT ---------------------*/  
/* 1.1 Init  */
void BT_RTInit (uint8_t Ch, BT_RTInitRegT* pInit);
/* 1.2 Int  */
/* 1.2.1 Trigger Int  */
void BT_RTEnableTrigInt(uint8_t Ch, BT_RTTrigIntCallback* IntCallback);
void BT_RTDisableTrigInt(uint8_t Ch);
IntStatusT BT_RTGetTrigIntFlag(uint8_t Ch);
void BT_RTClrTrigIntFlag(uint8_t Ch);
/* 1.2.2 Underflow Int  */
void BT_RTEnableUnderflowInt(uint8_t Ch, BT_RTUFIntCallback* IntCallback);
void BT_RTDisableUnderflowInt(uint8_t Ch);
IntStatusT BT_RTGetUnderflowIntFlag(uint8_t Ch);
void BT_RTClrUnderflowIntFlag(uint8_t Ch);
/* 1.3 Control  */
void BT_RTDisableCount(uint8_t Ch);
void BT_RTEnableCount(uint8_t Ch);
void BT_RTStartSoftTrig(uint8_t Ch);
/* 1.4 Get Count value  */
uint16_t BT_RTGet16bitCount(uint8_t Ch);
uint32_t BT_RTGet32bitCount(uint8_t Ch);
/*----------------- 2. PWM ---------------------*/  
/* 2.1 Init  */
void BT_PWMInit (uint8_t Ch, BT_PWMInitRegT* pInit);
/* 2.2 Int */
/* 2.2.1 Trigger Int  */
void BT_PWMEnableTrigInt(uint8_t Ch, BT_PWMTrigIntCallback* IntCallback);
void BT_PWMDisableTrigInt(uint8_t Ch);
IntStatusT BT_PWMGetTrigIntFlag(uint8_t Ch);
void BT_PWMClrTrigIntFlag(uint8_t Ch);
/* 2.2.2  Underflow Int  */
void BT_PWMEnableUnderflowInt(uint8_t Ch, BT_PWMUFIntCallback* IntCallback);
void BT_PWMDisableUnderflowInt(uint8_t Ch);
IntStatusT BT_PWMGetUnderflowIntFlag(uint8_t Ch);
void BT_PWMClrUnderflowIntFlag(uint8_t Ch);
/* 2.2.3  Duty match Int  */
void BT_PWMEnableDutyMatchInt(uint8_t Ch, BT_PWMDMIntCallback* IntCallback);
void BT_PWMDisableDutyMatchInt(uint8_t Ch);
IntStatusT BT_PWMGetDutyMatchIntFlag(uint8_t Ch);
void BT_PWMClrDutyMatchIntFlag(uint8_t Ch);
/* 2.3 Control  */
void BT_PWMDisableCount(uint8_t Ch);
void BT_PWMEnableCount(uint8_t Ch);
void BT_PWMStartSoftTrig(uint8_t Ch);
/* 2.4 Get Count value  */
uint16_t BT_PWMGet16bitCount(uint8_t Ch);
/*----------------- 3. PPG ---------------------*/  
/* 3.1 Init  */
void BT_PPGInit (uint8_t Ch, BT_PPGInitRegT* pInit);
/* 3.2 Int */
/* 3.2.1 Trigger Int  */
void BT_PPGEnableTrigInt(uint8_t Ch, BT_PPGTrigIntCallback* IntCallback);
void BT_PPGDisableTrigInt(uint8_t Ch);
IntStatusT BT_PPGGetTrigIntFlag(uint8_t Ch);
void BT_PPGClrTrigIntFlag(uint8_t Ch);
/* 3.2.2  Underflow Int  */
void BT_PPGEnableUnderflowInt(uint8_t Ch, BT_PPGUFIntCallback* IntCallback);
void BT_PPGDisableUnderflowInt(uint8_t Ch);
IntStatusT BT_PPGGetUnderflowIntFlag(uint8_t Ch);
void BT_PPGClrUnderflowIntFlag(uint8_t Ch);
/* 3.3 Control  */
void BT_PPGDisableCount(uint8_t Ch);
void BT_PPGEnableCount(uint8_t Ch);
void BT_PPGStartSoftTrig(uint8_t Ch);
/* 3.4 Get Count value  */
uint16_t BT_PPGGet16bitCount(uint8_t Ch);
/*----------------- 4. PWC ---------------------*/  
/* 4.1 Init  */
void BT_PWCInit (uint8_t Ch, BT_PWCInitRegT* pInit);
/* 4.2 Int */
/* 4.2.1 Overflow Int  */
void BT_PWCEnableOverflowInt(uint8_t Ch, BT_PWCOFIntCallback* IntCallback);
void BT_PWCDisableOverflowInt(uint8_t Ch);
IntStatusT BT_PWCGetOverflowIntFlag(uint8_t Ch);
void BT_PWCClrOverflowIntFlag(uint8_t Ch);
/* 4.2.2  Measure complete Int  */
void BT_PWCEnableMeasCmpInt(uint8_t Ch, BT_PWCMCIntCallback* IntCallback);
void BT_PWCDisableMeasCmpInt(uint8_t Ch);
IntStatusT BT_PWCGetMeasCmpIntFlag(uint8_t Ch);
/* 4.2.3 Measure error status */
FlagStatusT BT_PWCGetMeasErrStat(uint8_t Ch);
/* 4.3 Control  */
void BT_PWCDisableCount(uint8_t Ch);
void BT_PWCEnableCount(uint8_t Ch);
/* 4.4 Get measure data */
uint16_t BT_PWCGet16bitMeasData(uint8_t Ch);
uint16_t BT_PWCGet32bitMeasData(uint8_t Ch);
/*----------------- 5. Interrupt handler ---------------------*/  
void BT_IntHandler(uint8_t Ch, uint8_t IntType);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _BT_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
