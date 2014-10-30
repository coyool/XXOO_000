/*!
 ******************************************************************************
 **
 ** \file csv_fm3.h
 **
 ** \brief Clock supervisor module driver
 ** 
 **
 ** \author 
 **
 ** \version V0.01
 **
 ** \date 2011-12-30
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
 
#ifndef _CSV_FM3_H_
#define _CSV_FM3_H_


/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_ENABLE_CSV == ON) && (PERIPHERAL_AVAILABLE_CSV == ON)
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief 1/256 frequency of high-speed CR oscillation */
#define FCS_CYCLE_256        5
/*! \brief 1/512 frequency of high-speed CR oscillation */
#define FCS_CYCLE_512        6
/*! \brief 1/1024 frequency of high-speed CR oscillation [Initial value] */
#define FCS_CYCLE_1024       7

/*! \brief FCS count cycle check list */
#define IS_FCS_CYCLE(CYCLE) (((CYCLE) == FCS_CYCLE_256) || \
                             ((CYCLE) == FCS_CYCLE_512) || \
                             ((CYCLE) == FCS_CYCLE_1024))


/*! \brief A sub clock failure has been detected. */
#define CSV_SUB_FAIL        2
/*! \brief A main clock failure has been detected. */
#define CSV_MAIN_FAIL       1

/*---------------------------------------------------------------------------*/
/* types, enums and structures                                               */
/*---------------------------------------------------------------------------*/
typedef void CSV_IntCallbackT(void);

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void CSV_EnableMainCSV(void);
void CSV_DisableMainCSV(void);
void CSV_EnableSubCSV(void);
void CSV_DisableSubCSV(void);
void CSV_EnableFCS(void);
void CSV_FCSDisable(void);
void CSV_EnableFCSReset(void);
void CSV_DisableFCSReset(void);
void CSV_EnableFCSInt(CSV_IntCallbackT* IntCallback);
void CSV_DisableFCSInt(void);
void CSV_ClrFCSIntRequest(void);
uint8_t CSV_GetFCSIntRequest(void);
void CSV_SetFCSCycle(uint16_t Cycle);
uint8_t CSV_GetCSVFailCause(void);
void CSV_SetFCSDetectLower(uint16_t Limit);
void CSV_SetFCSDetectUpper(uint16_t Limit);
uint16_t CSV_GetFCSDetectCount(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _CLOCK_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
