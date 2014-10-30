/*!
 ******************************************************************************
 **
 ** \file fm3_common.h
 **
 ** \brief This file provides all global definitions for peripheral library.
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-11-28
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

#ifndef _FM3_COMMON_H_
#define _FM3_COMMON_H_

#include "mb9xfxxx.h"

/*---------------------------------------------------------------------------*/
/* \brief Flag state definition                                              */
/*---------------------------------------------------------------------------*/
#ifndef FALSE
#define FALSE          0
#endif 
/*---------------------------------------------------------------------------*/
/* \brief Flag state definition                                              */
/*---------------------------------------------------------------------------*/
#ifndef TRUE
#define TRUE           1
#endif
/*---------------------------------------------------------------------------*/
/* \brief Flag type enumeration                                              */
/*---------------------------------------------------------------------------*/
typedef enum 
{
    RESET = 0, 
    SET = 1,
} FlagStatusT, IntStatusT;

/*---------------------------------------------------------------------------*/
/* \brief Funciton state enumeration                                         */
/*---------------------------------------------------------------------------*/
typedef enum 
{
    DISABLE = 0, 
    ENABLE = 1,
} FuncStateT;

/*---------------------------------------------------------------------------*/
/* \brief Check function status                                              */
/*---------------------------------------------------------------------------*/
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

/*---------------------------------------------------------------------------*/
/* \brief Error status enumeration                                           */
/*---------------------------------------------------------------------------*/
typedef enum 
{
    RET_ERROR = 0, 
    RET_SUCCESS = 1
} ErrorStatusT;

#endif /* _FM3_COMMON_H_ */
/*****************************************************************************/
/* END OF FILE */ 

