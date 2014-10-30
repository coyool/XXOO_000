/*!
 ******************************************************************************
 **
 ** \file qprc_fm3.h
 **
 ** \brief QPRC drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.21
 **
 ** \date 2012-08-25
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

#ifndef _QPRC_FM3_H_
#define _QPRC_FM3_H_

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_QPRC == OFF)
    #if (PERIPHERAL_ENABLE_QPRC == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_AVAILABLE_QPRC == ON) && (PERIPHERAL_ENABLE_QPRC == ON)

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#if (MCU_TYPE == DEVICE_TYPE5)
    /*! \brief QPRC channel number */
    #define QPRC_CH_NUM              1
    /*! \brief QPRC channel 0 */
    #define QPRC_CH0              0
    #define IS_QPRC_CH_VALID(Ch)     ((Ch) ==  QPRC_CH0)
#elif (MCU_TYPE == DEVICE_TYPE0) || (MCU_TYPE == DEVICE_TYPE1) || \
      (MCU_TYPE == DEVICE_TYPE8) || (MCU_TYPE == DEVICE_TYPE9)
    /*! \brief QPRC channel number */
    #define QPRC_CH_NUM              2
    /*! \brief QPRC channel 0 */
    #define QPRC_CH0              0
    /*! \brief QPRC channel 1 */
    #define QPRC_CH1              1
    #define IS_QPRC_CH_VALID(Ch)     (((Ch) ==  QPRC_CH0) || ((Ch) ==  QPRC_CH1))
#else 
    /*! \brief QPRC channel number */
    #define QPRC_CH_NUM              3
    /*! \brief QPRC channel 0 */
    #define QPRC_CH0              0
    /*! \brief QPRC channel 1 */
    #define QPRC_CH1              1
    /*! \brief QPRC channel 2 */
    #define QPRC_CH2              2
    #define IS_QPRC_CH_VALID(Ch)     (((Ch) ==  QPRC_CH0) || \
                                      ((Ch) ==  QPRC_CH1) || \
                                      ((Ch) ==  QPRC_CH2))
#endif


/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
/*!
 ******************************************************************************
 ** \brief Swap function selection
 ******************************************************************************
 */
typedef enum
{
    AIN_BIN_SWAP_OFF = 0,  //!< No swap
    AIN_BIN_SWAP_ON,       //!< Swap AIN with BIN
    
}QPRC_SwapModeT;

/*!
 ******************************************************************************
 ** \brief QPRCR compare object selection
 ******************************************************************************
 */
typedef enum
{
    QPRCR_CMP_PC_CNT = 0,  //!< Compare QPRCR with PC count
    QPRCR_CMP_RC_CNT,      //!< Compare QPRCR with RC count
    
}QPRC_QPRCRCmpObjT;

/*!
 ******************************************************************************
 ** \brief ZIN function selection
 ******************************************************************************
 */
typedef enum
{
    ZIN_COUNT_CLR_FUNC = 0,  //!< Count clear function
    ZIN_GATE_FUNC,           //!< Gate function
    
}QPRC_ZINFuncT;

/*!
 ******************************************************************************
 ** \brief CGE bits setting
 ******************************************************************************
 */
typedef enum
{
    ZIN_CGE_BITS_00B = 0x00, //!< CGE=00B, disable
    ZIN_CGE_BITS_01B,        //!< CGE=01B, falling or low level valid
    ZIN_CGE_BITS_10B,        //!< CGE=10B, rising or high level valid
    ZIN_CGE_BITS_11B,        //!< CGE=11B, both edge or disable
    
}QPRC_ZINCGEBitsT;

/*!
 ******************************************************************************
 ** \brief AIN or BIN valid edge
 ******************************************************************************
 */
typedef enum
{
    EDGE_DETECT_DISABLE = 0x00,  //!< Edge detect disable
    EDGE_DETECT_FALLING,         //!< Falling edge detect
    EDGE_DETECT_RISING,          //!< Rising edge detect
    EDGE_DETECT_BOTH,            //!< Both edge detect
    
}QPRC_BINDetectEdgeT, QPRC_AINDetectEdgeT;

/*!
 ******************************************************************************
 ** \brief PC reset mask setting
 ******************************************************************************
 */
typedef enum
{
    PC_RESET_MASK_DISABLE = 0,  //!< PC reset mask disable
    PC_RESET_MASK_TWICE,        //!< PC reset mask twice
    PC_RESET_MASK_FOUR_TIMES,   //!< PC reset mask four times
    PC_RESET_MASK_EIGHT_TIMES,  //!< PC reset mask eight times
    
}QPRC_PCResetMaskSelT;

/*!
 ******************************************************************************
 ** \brief PC mode setting
 ******************************************************************************
 */
typedef enum
{
    PC_MODE_STOP = 0,       //!< PC disable mode
    PC_MODE_UPDOWN,         //!< PC up/down mode
    PC_MODE_PHASE_DIFF,     //!< PC phase different mode
    PC_MODE_DIRECTIONAL,    //!< PC directional mode
}QPRC_PCModeT;

/*!
 ******************************************************************************
 ** \brief RC mode setting
 ******************************************************************************
 */
typedef enum
{
    RC_MODE_STOP = 0,           //!< RC disable mode
    RC_MODE_ZIN_TRIG,           //!< ZIN trigger mode
    RC_MODE_PCOUF_TRIG,         //!< PC underflow and overflow mode
    RC_MODE_ZIN_PCOUF_TRIG,     //!< ZIN trigger or PC underflow and overflow mode
    
}QPRC_RCModeT;

/*!
 ******************************************************************************
 ** \brief PC range setting
 ******************************************************************************
 */
typedef enum
{
    PC_RANGE_16K = 0,  //!< PC range with 16K area
    PC_RANGE_8K,       //!< PC range with 8K area
    
}QPRC_PCRangeT;

/*!
 ******************************************************************************
 ** \brief PC direction
 ******************************************************************************
 */
typedef enum
{
    PC_DIR_INCREASE = 0, //!< PC counts increasely
    PC_DIR_DECREASE,     //!< PC counts descreasely
    
}QPRC_PCDirT;

/*!
 ******************************************************************************
 ** \brief PC status interrupt
 ******************************************************************************
 */
typedef enum
{
    PC_INT_STATUS_ZERO_INDEX = 0,  //!< PC zero index interrupt status
    PC_INT_STATUS_OVERFLOW,        //!< PC overflow interrupt status
    PC_INT_STATUS_UNDERFLOW,       //!< PC underflow interrupt status
    
}QPRC_PCIntStatusT;

/*!
 ******************************************************************************
 ** \brief RC outrange interrupt callback
 ******************************************************************************
 */
typedef void QPRC_RCOutRangeIntT (void);
/*!
 ******************************************************************************
 ** \brief PC status interrupt callback
 ******************************************************************************
 */
typedef void QPRC_PCStatusIntT(QPRC_PCIntStatusT Type);
/*!
 ******************************************************************************
 ** \brief QPCCR match interrupt callback
 ******************************************************************************
 */
typedef void QPRC_QPCCRMatchIntT(void);
/*!
 ******************************************************************************
 ** \brief QPRCR match interrupt callback
 ******************************************************************************
 */
typedef void QPRC_QPRCRMatchIntT(void);
/*!
 ******************************************************************************
 ** \brief QPCCR and QPRCR match interrupt callback
 ******************************************************************************
 */
typedef void QPRC_QPCCRAndQPRCRMatchIntT(void);
/*!
 ******************************************************************************
 ** \brief PC inversion interrupt callback
 ******************************************************************************
 */
typedef void QPRC_PCInvIntT(void);
 
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/* 1. Count/compare value setting */
/* PC */
void QPRC_SetPCCntVal(uint8_t Ch, uint16_t Val);
uint16_t QPRC_GetPCCntVal(uint8_t Ch);
void QPRC_SetQPCCRVal(uint8_t Ch, uint16_t Val);
/* RC */
void QPRC_SetRCCntVal(uint8_t Ch, uint16_t Val);
uint16_t QPRC_GetRCCntVal(uint8_t Ch);
void QPRC_SetQPRCRVal(uint8_t Ch, uint16_t Val);
/* PC maximum value */
void QPRC_SetPCMaxCntVal(uint8_t Ch, uint16_t Val);
/* 2. Control setting */
void QPRC_SelAINBINSwap(uint8_t Ch, QPRC_SwapModeT SwapMode);
void QPRC_SelQPRCRCmpObj(uint8_t Ch, QPRC_QPRCRCmpObjT Obj);
void QPRC_SelZINFunc(uint8_t Ch, QPRC_ZINFuncT Func);
void QPRC_SetZINDetectEdge(uint8_t Ch, QPRC_ZINCGEBitsT Bits);
void QPRC_SetBINDetectEdge(uint8_t Ch, QPRC_BINDetectEdgeT Edge);
void QPRC_SetAINDetectEdge(uint8_t Ch, QPRC_AINDetectEdgeT Edge);
void QPRC_SetPCResetMask(uint8_t Ch, QPRC_PCResetMaskSelT Mask);
void QPRC_EnablePCCnt(uint8_t Ch);
void QPRC_DisablePCCnt(uint8_t Ch);
void QPRC_SetPCMode(uint8_t Ch, QPRC_PCModeT Mode);
void QPRC_SetRCMode(uint8_t Ch, QPRC_RCModeT Mode);
/* 3. Interrupt setting */
/* PC Outrange int */
void QPRC_SelRCOutRange(uint8_t Ch, QPRC_PCRangeT RangeType);
void QPRC_EnableRCOutRangeInt(uint8_t Ch, QPRC_RCOutRangeIntT* IntCallback);
void QPRC_DisableRCOutRangeInt(uint8_t Ch);
IntStatusT QPRC_GetRCOutrangeIntFlag(uint8_t Ch);
void QPRC_ClrRCOutrangeIntFlag(uint8_t Ch);
/* PC overflow/underflow/zero index int */
void QPRC_EnablePCStatusInt(uint8_t Ch, QPRC_PCStatusIntT* IntCallback);
void QPRC_DisablePCStatusInt(uint8_t Ch);
IntStatusT QPRC_GetPCZeroIndexIntFlag(uint8_t Ch);
void QPRC_ClrPCZeroIndexIntFlag(uint8_t Ch);
IntStatusT QPRC_GetPCUnderflowIntFlag(uint8_t Ch);
void QPRC_ClrPCUnderflowIntFlag(uint8_t Ch);
IntStatusT QPRC_GetPCOverflowIntFlag(uint8_t Ch);
void QPRC_ClrPCOverflowIntFlag(uint8_t Ch);
/* QPCCR match int */
void QPRC_EnableQPCCRMatchInt(uint8_t Ch, QPRC_QPCCRMatchIntT* IntCallback);
void QPRC_DisableQPCCRMatchInt(uint8_t Ch);
IntStatusT QPRC_GetQPCCRMatchIntFlag(uint8_t Ch);
void QPRC_ClrQPCCRMatchIntFlag(uint8_t Ch);
/* QPRCR match int */
void QPRC_EnableQPRCRMatchInt(uint8_t Ch, QPRC_QPRCRMatchIntT* IntCallback);
void QPRC_DisableQPRCRMatchInt(uint8_t Ch);
IntStatusT QPRC_GetQPRCRMatchIntFlag(uint8_t Ch);
void QPRC_ClrQPRCRMatchIntFlag(uint8_t Ch);
/* QPCCR(with PC cnt) and QPRCR(with RC cnt) match */
void QPRC_EnableQPCCRAndQPRCRMatchInt(uint8_t Ch, 
                                      QPRC_QPCCRAndQPRCRMatchIntT* IntCallback);
void QPRC_DisableQPCCRAndQPRCRMatchInt(uint8_t Ch);
IntStatusT QPRC_GetQPCCRAndQPRCRMatchIntFlag(uint8_t Ch);
void QPRC_ClrQPCCRAndQPRCRMatchIntFlag(uint8_t Ch);
/* PC last direction status */
QPRC_PCDirT QPRC_GetPCDirWhenFlow(uint8_t Ch);
QPRC_PCDirT QPRC_GetPCLastDir(uint8_t Ch);
/* PC Inversion interrupt */
void QPRC_EnablePCInvInt(uint8_t Ch, QPRC_PCInvIntT* IntCallback);
void QPRC_DisablePCInvInt(uint8_t Ch);
IntStatusT QPRC_GetPCInvInt(uint8_t Ch);
void QPRC_ClrPCInvInt(uint8_t Ch);
/* 4. IRQ */
void QPRC_IntHandler(uint8_t Ch, uint16_t IrqValue);

#ifdef __cplusplus
}
#endif

#endif

#endif   /* _QPRC_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */

