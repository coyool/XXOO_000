/*!
 ******************************************************************************
 **
 ** \file dmac_fm3.h
 **
 ** \brief DMA head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-21
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 */ 

#ifndef _DMAC_FM3_H_
#define _DMAC_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_ENABLE_DMAC == ON) && (PERIPHERAL_AVAILABLE_DMAC == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/* DMAC priority mode definition */
/*! \brief DMAC channel priority fixed */
#define   DMAC_PRIO_FIX             (0)
/*! \brief DMAC channel priority rolated */
#define   DMAC_PRIO_ROLATE          (1)

/* DMAC channel definition */
/*! \brief DMAC channel 0 */
#define   DMAC_CH0                  (0)
/*! \brief DMAC channel 1 */
#define   DMAC_CH1                  (1)
/*! \brief DMAC channel 2 */
#define   DMAC_CH2                  (2)
/*! \brief DMAC channel 3 */
#define   DMAC_CH3                  (3)
/*! \brief DMAC channel 4 */
#define   DMAC_CH4                  (4)
/*! \brief DMAC channel 5 */
#define   DMAC_CH5                  (5)
/*! \brief DMAC channel 6 */
#define   DMAC_CH6                  (6)
/*! \brief DMAC channel 7 */
#define   DMAC_CH7                  (7)

/* DMAC interrupt callback function type definition */
/*! \brief DMAC transfer completion call back function type definition */
typedef void DMAC_TrsfCmpCallbackT(void); 
/*! \brief DMAC transfer error call back function type definition */
typedef void DMAC_TrsfErrCallbackT(void); 

/*!
 ******************************************************************************
 ** \brief DMAC input enumeration
 ******************************************************************************
 */
typedef enum
{
    DMAC_SW_TRIG = 0,          //!< Software trigger
    DAMC_USB0_EP1_INT = 32,    //!< IDREQ0
    DAMC_USB0_EP2_INT,         //!< IDREQ1
    DAMC_USB0_EP3_INT,         //!< IDREQ2
    DAMC_USB0_EP4_INT,         //!< IDREQ3
    DAMC_USB0_EP5_INT,         //!< IDREQ4
    DMAC_ADC0_SCAN_INT,        //!< IDREQ5
    DMAC_ADC1_SCAN_INT,        //!< IDREQ6
    DMAC_ADC2_SCAN_INT,        //!< IDREQ7
    DMAC_BT0_IRQ0,             //!< IDREQ8
    DMAC_BT2_IRQ0,             //!< IDREQ9
    DMAC_BT4_IRQ0,             //!< IDREQ10
    DMAC_BT6_IRQ0,             //!< IDREQ11
    DMAC_MFS0_RX_INT,          //!< IDREQ12
    DMAC_MFS0_TX_INT,          //!< IDREQ13
    DMAC_MFS1_RX_INT,          //!< IDREQ14
    DMAC_MFS1_TX_INT,          //!< IDREQ15
    DMAC_MFS2_RX_INT,          //!< IDREQ16
    DMAC_MFS2_TX_INT,          //!< IDREQ17
    DMAC_MFS3_RX_INT,          //!< IDREQ18
    DMAC_MFS3_TX_INT,          //!< IDREQ19
    DMAC_MFS4_RX_INT,          //!< IDREQ20
    DMAC_MFS4_TX_INT,          //!< IDREQ21
    DMAC_MFS5_RX_INT,          //!< IDREQ22
    DMAC_MFS5_TX_INT,          //!< IDREQ23
    DMAC_MFS6_RX_INT,          //!< IDREQ24
    DMAC_MFS6_TX_INT,          //!< IDREQ25
    DMAC_MFS7_RX_INT,          //!< IDREQ26
    DMAC_MFS7_TX_INT,          //!< IDREQ27
    DMAC_EXT_INT_0,            //!< IDREQ28
    DMAC_EXT_INT_1,            //!< IDREQ29
    DMAC_EXT_INT_2,            //!< IDREQ30
    DMAC_EXT_INT_3,            //!< IDREQ31
}DMAC_InputSourceT;

/*!
 ******************************************************************************
 ** \brief Interrupt type
 ******************************************************************************
 */
typedef enum
{
    DRQ_USB0_EP1_INT =0,      //!< IDREQ0
    DRQ_USB0_EP2_INT,         //!< IDREQ1
    DRQ_USB0_EP3_INT,         //!< IDREQ2
    DRQ_USB0_EP4_INT,         //!< IDREQ3
    DRQ_USB0_EP5_INT,         //!< IDREQ4
    DRQ_ADC0_SCAN_INT,        //!< IDREQ5
    DRQ_ADC1_SCAN_INT,        //!< IDREQ6
    DRQ_ADC2_SCAN_INT,        //!< IDREQ7
    DRQ_BT0_IRQ0,             //!< IDREQ8
    DRQ_BT2_IRQ0,             //!< IDREQ9
    DRQ_BT4_IRQ0,             //!< IDREQ10
    DRQ_BT6_IRQ0,             //!< IDREQ11
    DRQ_MFS0_RX_INT,          //!< IDREQ12
    DRQ_MFS0_TX_INT,          //!< IDREQ13
    DRQ_MFS1_RX_INT,          //!< IDREQ14
    DRQ_MFS1_TX_INT,          //!< IDREQ15
    DRQ_MFS2_RX_INT,          //!< IDREQ16
    DRQ_MFS2_TX_INT,          //!< IDREQ17
    DRQ_MFS3_RX_INT,          //!< IDREQ18
    DRQ_MFS3_TX_INT,          //!< IDREQ19
    DRQ_MFS4_RX_INT,          //!< IDREQ20
    DRQ_MFS4_TX_INT,          //!< IDREQ21
    DRQ_MFS5_RX_INT,          //!< IDREQ22
    DRQ_MFS5_TX_INT,          //!< IDREQ23
    DRQ_MFS6_RX_INT,          //!< IDREQ24
    DRQ_MFS6_TX_INT,          //!< IDREQ25
    DRQ_MFS7_RX_INT,          //!< IDREQ26
    DRQ_MFS7_TX_INT,          //!< IDREQ27
    DRQ_EXT_INT_0,            //!< IDREQ28
    DRQ_EXT_INT_1,            //!< IDREQ29
    DRQ_EXT_INT_2,            //!< IDREQ30
    DRQ_EXT_INT_3,            //!< IDREQ31
    
}DMAC_IntSrcT;

typedef enum
{
    DMAC_INT_OUTPUT_CPU = 0,
    DMAC_INT_OUTPUT_DMAC,
    
}DMAC_IntOutputT;

/*!
 ******************************************************************************
 ** \brief DMAC mode enumeration
 ******************************************************************************
 */
typedef enum
{
    DMAC_BLOCK_TRSF_MODE = 0,  //!< DMAC block transfer mode
    DMAC_BURST_TRSF_MODE,      //!< DMAC burst transfer mode
    DMAC_DEMAND_TRSF_MODE,     //!< DMAC demand transfer mode
  
}DMAC_ModeT;

/*!
 ******************************************************************************
 ** \brief DMAC transfer width enumeration
 ******************************************************************************
 */
typedef enum
{
    DMAC_TRST_WIDTH_8BIT = 0,  //!< 8 bit
    DMAC_TRST_WIDTH_16BIT,     //!< 16 bit
    DMAC_TRST_WIDTH_32BIT,     //!< 32 bit
    
}DMAC_TrsfWidthT;

/*!
 ******************************************************************************
 ** \brief DMAC stop status enumeration
 ******************************************************************************
 */
typedef enum
{
    DMAC_STOP_STAT_INIT = 0,       //!< initialization
    DMAC_STOP_STAT_TRSF_ERR,       //!< transfer error
    DMAC_STOP_STAT_FORCE_STOP,     //!< stop by peripheral or EB/DE bit
    DMAC_STOP_STAT_SRC_ERR,        //!< source address access error
    DMAC_STOP_STAT_DST_ERR,        //!< source address access error
    DMAC_STOP_STAT_TRSF_SUCCESS,   //!< transfer successfully
    DMAC_STOP_STAT_TRSF_PAUSE,     //!< transfer pause
    
}DMAC_StopStatT;

/* DMAC reload definition */
/*! \brief DMAC reload count disable */
#define  DMAC_RELOAD_DISABLE         (0)
/*! \brief DMAC reload count enable */
#define  DMAC_RELOAD_ENABLE          (1)

/* DMAC address variation mode */
/*! \brief DMAC address increased */
#define  DMAC_ADDR_INC               (0)
/*! \brief DMAC address fixed */
#define  DMAC_ADDR_FIX               (1)

/* DMAC TC BC reload definition */
/*! \brief DMAC TC BC reload disable  */
#define  DMAC_TC_BC_RL_DISABLE       (0)
/*! \brief DMAC TC BC reload enbale  */
#define  DMAC_TC_BC_RL_ENABLE        (1)

/*--------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* DMA Global Setting */
void DMAC_EntireEnable(void);
void DMAC_EntireDisable(void);
void DMAC_EntireSetPause(void);
void DMAC_EntireCancelPause(void);
FlagStatusT DMAC_EntireGetStopStat(void);
void DMAC_EntireSetPrioMode(uint8_t Mode);
/* DMA Local(channel) Setting */
void DMAC_EnableChOpt(uint8_t Ch);
void DMAC_DisableChOpt(uint8_t Ch);
void DMAC_SetChPause(uint8_t Ch);
void DMAC_CancelChPause(uint8_t Ch);
void DMAC_EnableChSWTrig(uint8_t Ch);
void DMAC_DisableChSWTrig(uint8_t Ch);
void DMAC_SelectChInputTrig(uint8_t Ch, DMAC_InputSourceT ScrTrig);
void DMAC_SetChBlockCnt(uint8_t Ch, uint8_t Cnt);
void DMAC_SetChTrstCnt(uint8_t Ch, uint16_t Cnt);
void DMAC_SetChMode(uint8_t Ch, DMAC_ModeT Mode);
void DMAC_SetChTrsfWidth(uint8_t Ch, DMAC_TrsfWidthT Width);
void DMAC_ConfigChSrcInfo(uint8_t Ch, uint8_t FS, uint8_t RS, uint32_t SA);
void DMAC_ConfigChDstInfo(uint8_t Ch, uint8_t FD, uint8_t RD, uint32_t DA);
void DMAC_EnableChReloadCnt(uint8_t Ch);
void DMAC_DisableChReloadCnt(uint8_t Ch);
void DMAC_EnableChErrInt(uint8_t Ch, DMAC_TrsfErrCallbackT* TrsfErrCallback);
void DMAC_DisableChErrInt(uint8_t Ch);
void DMAC_EnableChCmpInt(uint8_t Ch, DMAC_TrsfCmpCallbackT* TrsfCmpCallback);
void DMAC_DisableChCmpInt(uint8_t Ch);
DMAC_StopStatT DMAC_GetChStopStat(uint8_t Ch);
void DMAC_ClrChStopStat(uint8_t Ch);
void DMAC_EnableChEnBitClr(uint8_t Ch);
void DMAC_DisableChEnBitClr(uint8_t Ch);
/* Interrupt output selection */
void DMAC_SelIntOutput(DMAC_IntSrcT Src, DMAC_IntOutputT OutputType);
/* Interrupt handler */
void DMAC_IntHandler(uint8_t Ch);
#ifdef __cplusplus
}
#endif

#endif

#endif
