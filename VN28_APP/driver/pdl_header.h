/*!
 ******************************************************************************
 **
 ** \file pdl_header.h
 **
 ** \brief Driver head define
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

#ifndef _PDL_HEADER_H_
#define _PDL_HEADER_H_

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

/* peripheral driver head files */
#if (PERIPHERAL_ENABLE_ADC == ON) && (PERIPHERAL_AVAILABLE_ADC == ON)
#include "adc\adc12_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_BT == ON) && (PERIPHERAL_AVAILABLE_BT == ON)
#include "bt\bt_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_CLOCK == ON) && (PERIPHERAL_AVAILABLE_CLOCK == ON)
#include "clock\clock_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_CR == ON) && (PERIPHERAL_AVAILABLE_CR == ON)
#include "cr\cr_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_CRC == ON) && (PERIPHERAL_AVAILABLE_CRC == ON)
#include "crc\crc_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_CSV == ON) && (PERIPHERAL_AVAILABLE_CSV == ON)
#include "cs\csv_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_CAN == ON) && (PERIPHERAL_AVAILABLE_CAN == ON)
#include "can\can_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_DMAC == ON) && (PERIPHERAL_AVAILABLE_DMAC == ON)
#include "dmac\dmac_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_DT == ON) && (PERIPHERAL_AVAILABLE_DT == ON)
#include "dt\dt_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_DAC == ON) && (PERIPHERAL_AVAILABLE_DAC == ON)
#include "dac\dac10_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_DUALFLASH == ON) && (PERIPHERAL_AVAILABLE_DUALFLASH == ON)
#include "flash\dualflash_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_EXTINT == ON) && (PERIPHERAL_AVAILABLE_EXTINT == ON)
#include "extint\extint_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_EXTBUS == ON) && (PERIPHERAL_AVAILABLE_EXTBUS == ON)
#include "extbus\extbus_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_IO == ON) && (PERIPHERAL_AVAILABLE_IO == ON)
#include "io\io_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_LPCM == ON) && (PERIPHERAL_AVAILABLE_LPCM == ON)
#include "lpcm\lpcm_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_LVD == ON) && (PERIPHERAL_AVAILABLE_LVD == ON)
#include "lvd\lvd_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_LCDC == ON) && (PERIPHERAL_AVAILABLE_LCDC == ON)
#include "lcdc\lcdc_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFS_UART == ON) || \
    (PERIPHERAL_ENABLE_MFS_CSIO == ON) || \
    (PERIPHERAL_ENABLE_MFS_I2C == ON) || \
    (PERIPHERAL_ENABLE_MFS_LIN == ON)  
#include "mfs\mfs_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFS_UART == ON)
#include "mfs\uart\uart_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFS_CSIO == ON)
#include "mfs\csio\csio_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFS_I2C == ON)
#include "mfs\i2c\i2c_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFS_LIN == ON)
#include "mfs\lin\lin_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFT_FRT == ON)
#include "mft\mft_frt_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFT_OCU == ON)
#include "mft\mft_ocu_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFT_WFG == ON)
#include "mft\mft_wfg_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFT_ICU == ON)
#include "mft\mft_icu_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MFT_ADCMP == ON)
#include "mft\mft_adcmp_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_MAINFLASH == ON) && (PERIPHERAL_AVAILABLE_MAINFLASH == ON)
#include "flash\mainflash_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_QPRC == ON) && (PERIPHERAL_AVAILABLE_QPRC == ON)
#include "qprc\qprc_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_RC == ON) && (PERIPHERAL_AVAILABLE_RC == ON)
#include "rc\rc_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_RTC == ON) && (PERIPHERAL_AVAILABLE_RTC == ON)
#include "rtc\rtc_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_RESET == ON) && (PERIPHERAL_AVAILABLE_RESET == ON)
#include "reset\reset_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_UID == ON) && (PERIPHERAL_AVAILABLE_UID == ON)
#include "uid\uid_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_WC == ON) && (PERIPHERAL_AVAILABLE_WC == ON)
#include "wc\wc_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_WD == ON) && (PERIPHERAL_AVAILABLE_WD == ON)
#include "wd\wd_fm3.h"
#endif

#if (PERIPHERAL_ENABLE_WORKFLASH == ON) && (PERIPHERAL_AVAILABLE_WORKFLASH == ON)
#include "flash\workflash_fm3.h"
#endif

#include "resource.h"

#endif /* _PDL_HEADER_H_ */

/*****************************************************************************/
/* END OF FILE */
