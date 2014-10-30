/*!
 ******************************************************************************
 **
 ** \file extbus_fm3.h
 **
 ** \brief external bus drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
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
 
#ifndef _EXTBUS_FM3_H_
#define _EXTBUS_FM3_H_
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_EXTBUS == OFF)
    #if (PERIPHERAL_ENABLE_EXTBUS == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_EXTBUS == ON) && (PERIPHERAL_AVAILABLE_EXTBUS == ON)

#define EXTB_CS0                0x00
#define EXTB_CS1                0x01
#define EXTB_CS2                0x02
#define EXTB_CS3                0x03
#define EXTB_CS4                0x04
#define EXTB_CS5                0x05
#define EXTB_CS6                0x06
#define EXTB_CS7                0x07

#define EXTB_CS_MAX             0x07

#define EXTB_TIM_WIDLC_MAX      0x0F
#define EXTB_TIM_WWEC_MAX       0x0E
#define EXTB_TIM_WADC_MAX       0x0E

#define EXTB_TIM_WACC_MIN       0x02
#define EXTB_TIM_WACC_MAX       0x0F

#define EXTB_TIM_RIDLC_MAX      0x0F
#define EXTB_TIM_FRADC_MAX      0x0F
#define EXTB_TIM_RADC_MAX       0x0F
#define EXTB_TIM_RACC_MAX       0x0F

#define EXTB_AREA_MASK_MAX      0x3F
#define EXTB_AREA_ADDR_MAX      0xFF

#define EXTB_ATIM_ALEW_MAX      0x0F
#define EXTB_ATIM_ALES_MAX      0x0F
#define EXTB_ATIM_ALC_MAX       0x0F

#define EXTB_DCLKR_MDIV_MAX     0x0F

/*! \brief This bit is used to select how to set MOEX width */
#define EXTB_MODEX_MOEXEUP      13
/*! \brief This bit is used to select a CS assertion from the start of accessing to the end of address output in multiplex mode */
#define EXTB_MODEX_MPXCSOF      12
/*! \brief This bit is used to select whether or not the address is output to the data lines in multiplex mode */
#define EXTB_MODEX_MPXDOFF      11
/*! \brief This bit is used to set up the polarity of the ALE signal */
#define EXTB_MODEX_ALEINV       9
/*! \brief This bit is used to select operation bus mode */
#define EXTB_MODEX_MPXMODE      8
/*! \brief This bit is used to select to which idle cycle the write data output is extended */
#define EXTB_MODEX_SHRTDOUT     7
/*! \brief This bit is used to control the external RDY function */
#define EXTB_MODEX_RDY          6
/*! \brief This bit controls the mode of NOR Flash memory page access */
#define EXTB_MODEX_PAGE         5
/*! \brief This bit controls the mode used to connect with a NAND Flash memory */
#define EXTB_MODEX_NAND         4
/*! \brief This bit can disable the write enable signal(MWEX) operation */
#define EXTB_MODEX_WEOFF        3
/*! \brief This bit can enable the byte mask signal(MDQM) for read access */
#define EXTB_MODEX_RBMON        2
/*! \brief This bit specifies the data bit width of a device to be connected */
#define EXTB_MODEX_WDTH1        1
/*! \brief This bit specifies the data bit width of a device to be connected */
#define EXTB_MODEX_WDTH0        0
/*! \brief These bits set the number of idle cycles after write access */
#define EXTB_TIMX_WIDLC         28
/*! \brief These bits set the number of assert cycles of write enable */
#define EXTB_TIMX_WWEC          24
/*! \brief These bits set the number of setup cycles of write address */
#define EXTB_TIMX_WADC          20
/*! \brief These bits set the number of cycles required for write access */
#define EXTB_TIMX_WACC          16
/*! \brief These bits set the number of idle cycles after read access */
#define EXTB_TIMX_RIDLC         12
/*! \brief These bits exclusively set a NOR Flash memory that supports page read access when MODE.PAGE=1 */
#define EXTB_TIMX_FRADC         8
/*! \brief These bits set the number of setup cycles of read address */
#define EXTB_TIMX_RADC          4
/*! \brief These bits set the number of cycles required for read access */
#define EXTB_TIMX_RACC          0
/*! \brief These bits set the value to mask [26:20] of the internal address (27:20) setin ADDR */
#define EXTB_AREAX_MASK         16
/*! \brief These bits specify the address to set the corresponding MCSX area */
#define EXTB_AREAX_ADDR         0
/*! \brief These bits are used to set the assertion period for MALE */
#define EXTB_ATIMX_ALEW         8
/*! \brief These bits are used to set the setup cycle for ALE assertion */
#define EXTB_ATIMX_ALES         4
/*! \brief These bits are used to set the address latch cycle */
#define EXTB_ATIMX_ALC          0
/*! \brief This bit is used to set MCLK division enable */
#define EXTB_DCLKR_MCLKON       4
/*! \brief These bits are used to set the division ratio of the division clock */
#define EXTB_DCLKR_MDIV         0


/*! \brief MOEX width is set with FRADC */
#define EXTB_MOEX_WIDTH_FRADC       1
/*! \brief MOEX width is set with RACC-RADC */
#define EXTB_MOEX_WIDTH_RACC_RADC   0
/*! \brief ALE signal becomes positive polarity */
#define EXTB_ALE_P              0
/*! \brief ALE signal becomes negative polarity */
#define EXTB_ALE_N              1
/*! \brief Indicate Ext BUS as seperate mode */
#define EXTB_MODE_SEPERATE      0
/*! \brief Indicate Ext BUS as multiplex mode */
#define EXTB_MODE_MULTIPLEX     1
/*! \brief Indicate Ext BUS data width to 8 bit */
#define EXTB_DATAWIDTH_8B       0
/*! \brief Indicate Ext BUS data width to 16 bit */
#define EXTB_DATAWIDTH_16B      1


/*!
******************************************************************************
 ** \brief external bus write idle cycle definition
******************************************************************************
*/
#define EXTB_WIDLC_01C          0x00
#define EXTB_WIDLC_02C          0x01
#define EXTB_WIDLC_03C          0x02
#define EXTB_WIDLC_04C          0x03
#define EXTB_WIDLC_05C          0x04
#define EXTB_WIDLC_06C          0x05
#define EXTB_WIDLC_07C          0x06
#define EXTB_WIDLC_08C          0x07
#define EXTB_WIDLC_09C          0x08
#define EXTB_WIDLC_10C          0x09
#define EXTB_WIDLC_11C          0x0A
#define EXTB_WIDLC_12C          0x0B
#define EXTB_WIDLC_13C          0x0C
#define EXTB_WIDLC_14C          0x0D
#define EXTB_WIDLC_15C          0x0E
#define EXTB_WIDLC_16C          0x0F

/*!
******************************************************************************
 ** \brief external bus write enable cycle definition
******************************************************************************
*/
#define EXTB_WWEC_01C           0x00
#define EXTB_WWEC_02C           0x01
#define EXTB_WWEC_03C           0x02
#define EXTB_WWEC_04C           0x03
#define EXTB_WWEC_05C           0x04
#define EXTB_WWEC_06C           0x05
#define EXTB_WWEC_07C           0x06
#define EXTB_WWEC_08C           0x07
#define EXTB_WWEC_09C           0x08
#define EXTB_WWEC_10C           0x09
#define EXTB_WWEC_11C           0x0A
#define EXTB_WWEC_12C           0x0B
#define EXTB_WWEC_13C           0x0C
#define EXTB_WWEC_14C           0x0D
#define EXTB_WWEC_15C           0x0E

/*!
******************************************************************************
 ** \brief external bus write access setup cycle definition
******************************************************************************
*/
#define EXTB_WADC_01C           0x00
#define EXTB_WADC_02C           0x01
#define EXTB_WADC_03C           0x02
#define EXTB_WADC_04C           0x03
#define EXTB_WADC_05C           0x04
#define EXTB_WADC_06C           0x05
#define EXTB_WADC_07C           0x06
#define EXTB_WADC_08C           0x07
#define EXTB_WADC_09C           0x08
#define EXTB_WADC_10C           0x09
#define EXTB_WADC_11C           0x0A
#define EXTB_WADC_12C           0x0B
#define EXTB_WADC_13C           0x0C
#define EXTB_WADC_14C           0x0D
#define EXTB_WADC_15C           0x0E

/*!
******************************************************************************
 ** \brief external bus write access cycle definition
******************************************************************************
*/
#define EXTB_WACC_03C           0x02
#define EXTB_WACC_04C           0x03
#define EXTB_WACC_05C           0x04
#define EXTB_WACC_06C           0x05
#define EXTB_WACC_07C           0x06
#define EXTB_WACC_08C           0x07
#define EXTB_WACC_09C           0x08
#define EXTB_WACC_10C           0x09
#define EXTB_WACC_11C           0x0A
#define EXTB_WACC_12C           0x0B
#define EXTB_WACC_13C           0x0C
#define EXTB_WACC_14C           0x0D
#define EXTB_WACC_15C           0x0E
#define EXTB_WACC_16C           0x0F

/*!
******************************************************************************
 ** \brief external bus read idle cycle definition
******************************************************************************
*/
#define EXTB_RIDLC_01C          0x00
#define EXTB_RIDLC_02C          0x01
#define EXTB_RIDLC_03C          0x02
#define EXTB_RIDLC_04C          0x03
#define EXTB_RIDLC_05C          0x04
#define EXTB_RIDLC_06C          0x05
#define EXTB_RIDLC_07C          0x06
#define EXTB_RIDLC_08C          0x07
#define EXTB_RIDLC_09C          0x08
#define EXTB_RIDLC_10C          0x09
#define EXTB_RIDLC_11C          0x0A
#define EXTB_RIDLC_12C          0x0B
#define EXTB_RIDLC_13C          0x0C
#define EXTB_RIDLC_14C          0x0D
#define EXTB_RIDLC_15C          0x0E
#define EXTB_RIDLC_16C          0x0F

/*!
******************************************************************************
 ** \brief external bus first read address cycle definition
******************************************************************************
*/
#define EXTB_FRADC_00C          0x00
#define EXTB_FRADC_01C          0x01
#define EXTB_FRADC_02C          0x02
#define EXTB_FRADC_03C          0x03
#define EXTB_FRADC_04C          0x04
#define EXTB_FRADC_05C          0x05
#define EXTB_FRADC_06C          0x06
#define EXTB_FRADC_07C          0x07
#define EXTB_FRADC_08C          0x08
#define EXTB_FRADC_09C          0x09
#define EXTB_FRADC_10C          0x0A
#define EXTB_FRADC_11C          0x0B
#define EXTB_FRADC_12C          0x0C
#define EXTB_FRADC_13C          0x0D
#define EXTB_FRADC_14C          0x0E
#define EXTB_FRADC_15C          0x0F

/*!
******************************************************************************
 ** \brief external bus read access setup cycle definition
******************************************************************************
*/
#define EXTB_RADC_00C           0x00
#define EXTB_RADC_01C           0x01
#define EXTB_RADC_02C           0x02
#define EXTB_RADC_03C           0x03
#define EXTB_RADC_04C           0x04
#define EXTB_RADC_05C           0x05
#define EXTB_RADC_06C           0x06
#define EXTB_RADC_07C           0x07
#define EXTB_RADC_08C           0x08
#define EXTB_RADC_09C           0x09
#define EXTB_RADC_10C           0x0A
#define EXTB_RADC_11C           0x0B
#define EXTB_RADC_12C           0x0C
#define EXTB_RADC_13C           0x0D
#define EXTB_RADC_14C           0x0E
#define EXTB_RADC_15C           0x0F

/*!
******************************************************************************
 ** \brief external bus read access cycle definition
******************************************************************************
*/
#define EXTB_RACC_01C           0x00
#define EXTB_RACC_02C           0x01
#define EXTB_RACC_03C           0x02
#define EXTB_RACC_04C           0x03
#define EXTB_RACC_05C           0x04
#define EXTB_RACC_06C           0x05
#define EXTB_RACC_07C           0x06
#define EXTB_RACC_08C           0x07
#define EXTB_RACC_09C           0x08
#define EXTB_RACC_10C           0x09
#define EXTB_RACC_11C           0x0A
#define EXTB_RACC_12C           0x0B
#define EXTB_RACC_13C           0x0C
#define EXTB_RACC_14C           0x0D
#define EXTB_RACC_15C           0x0E
#define EXTB_RACC_16C           0x0F

/*!
******************************************************************************
 ** \brief external bus mask address definition
******************************************************************************
*/
#define EXTB_MASKADDR_128MB     0x7F
#define EXTB_MASKADDR_64MB      0x3F
#define EXTB_MASKADDR_32MB      0x1F
#define EXTB_MASKADDR_16MB      0x0F
#define EXTB_MASKADDR_8MB       0x07
#define EXTB_MASKADDR_4MB       0x03
#define EXTB_MASKADDR_2MB       0x01
#define EXTB_MASKADDR_1MB       0x00

/*!
******************************************************************************
 ** \brief external bus mask address definition
******************************************************************************
*/
#define EXTB_AREA0_ADDR_DEFAULT 0x00
#define EXTB_AREA1_ADDR_DEFAULT 0x10
#define EXTB_AREA2_ADDR_DEFAULT 0x20
#define EXTB_AREA3_ADDR_DEFAULT 0x30
#define EXTB_AREA4_ADDR_DEFAULT 0x40
#define EXTB_AREA5_ADDR_DEFAULT 0x50
#define EXTB_AREA6_ADDR_DEFAULT 0x60
#define EXTB_AREA7_ADDR_DEFAULT 0x70

/*!
******************************************************************************
 ** \brief external bus Address latch enable width definition
******************************************************************************
*/
#define EXTB_ALEW_01C           0x00
#define EXTB_ALEW_02C           0x01
#define EXTB_ALEW_03C           0x02
#define EXTB_ALEW_04C           0x03
#define EXTB_ALEW_05C           0x04
#define EXTB_ALEW_06C           0x05
#define EXTB_ALEW_07C           0x06
#define EXTB_ALEW_08C           0x07
#define EXTB_ALEW_09C           0x08
#define EXTB_ALEW_10C           0x09
#define EXTB_ALEW_11C           0x0A
#define EXTB_ALEW_12C           0x0B
#define EXTB_ALEW_13C           0x0C
#define EXTB_ALEW_14C           0x0D
#define EXTB_ALEW_15C           0x0E
#define EXTB_ALEW_16C           0x0F

/*!
******************************************************************************
 ** \brief external bus Address latch enable setup cycle definition
******************************************************************************
*/
#define EXTB_ALES_00C           0x00
#define EXTB_ALES_01C           0x01
#define EXTB_ALES_02C           0x02
#define EXTB_ALES_03C           0x03
#define EXTB_ALES_04C           0x04
#define EXTB_ALES_05C           0x05
#define EXTB_ALES_06C           0x06
#define EXTB_ALES_07C           0x07
#define EXTB_ALES_08C           0x08
#define EXTB_ALES_09C           0x09
#define EXTB_ALES_10C           0x0A
#define EXTB_ALES_11C           0x0B
#define EXTB_ALES_12C           0x0C
#define EXTB_ALES_13C           0x0D
#define EXTB_ALES_14C           0x0E
#define EXTB_ALES_15C           0x0F

/*!
******************************************************************************
 ** \brief external bus Address latch cycle definition
******************************************************************************
*/
#define EXTB_ALC_01C            0x00
#define EXTB_ALC_02C            0x01
#define EXTB_ALC_03C            0x02
#define EXTB_ALC_04C            0x03
#define EXTB_ALC_05C            0x04
#define EXTB_ALC_06C            0x05
#define EXTB_ALC_07C            0x06
#define EXTB_ALC_08C            0x07
#define EXTB_ALC_09C            0x08
#define EXTB_ALC_10C            0x09
#define EXTB_ALC_11C            0x0A
#define EXTB_ALC_12C            0x0B
#define EXTB_ALC_13C            0x0C
#define EXTB_ALC_14C            0x0D
#define EXTB_ALC_15C            0x0E
#define EXTB_ALC_16C            0x0F

/*!
******************************************************************************
 ** \brief external bus MCLK division ratio definition
******************************************************************************
*/
#define EXTB_MCLK_DIV01         0x00
#define EXTB_MCLK_DIV02         0x01        
#define EXTB_MCLK_DIV03         0x02
#define EXTB_MCLK_DIV04         0x03
#define EXTB_MCLK_DIV05         0x04
#define EXTB_MCLK_DIV06         0x05
#define EXTB_MCLK_DIV07         0x06
#define EXTB_MCLK_DIV08         0x07
#define EXTB_MCLK_DIV09         0x08
#define EXTB_MCLK_DIV10         0x09
#define EXTB_MCLK_DIV11         0x0A
#define EXTB_MCLK_DIV12         0x0B
#define EXTB_MCLK_DIV13         0x0C
#define EXTB_MCLK_DIV14         0x0D
#define EXTB_MCLK_DIV15         0x0E
#define EXTB_MCLK_DIV16         0x0F

#ifdef __cplusplus
extern "C" {
#endif
/* Set MODE0 ~ MODE7 */
#if (MCU_TYPE != DEVICE_TYPE0)  
void EXTB_SetMOEXWidth(uint8_t CS, uint8_t Width);
void EXTB_EnableMCSXInALC(uint8_t CS);
void EXTB_DisableMCSXInALC(uint8_t CS);
void EXTB_EnableAddrOutput(uint8_t CS);
void EXTB_DisableAddrOutput(uint8_t CS);
void EXTB_SetALEPolarity(uint8_t CS, uint8_t Polarity);
void EXTB_SetOperMode(uint8_t CS, uint8_t Mode);
void EXTB_EnableWriteIdleCycleExtend(uint8_t CS);
void EXTB_DisableWriteIdleCycleExtend(uint8_t CS);
void EXTB_EnableExtRDY(uint8_t CS);
void EXTB_DisableExtRDY(uint8_t CS);
#endif
void EXTB_EnableNorPageMode(uint8_t CS);
void EXTB_DisableNorPageMode(uint8_t CS);
void EXTB_EnableNandMode(uint8_t CS);
void EXTB_DisableNandMode(uint8_t CS);
void EXTB_EnableWEX(uint8_t CS);
void EXTB_EnableWEX(uint8_t CS);
void EXTB_EnableReadByteMask(uint8_t CS);
void EXTB_DisableReadByteMask(uint8_t CS);
void EXTB_SetDataWidth(uint8_t CS, uint8_t Width);

/*Set TIM0 ~ TIM7 */
void EXTB_SetWriteIdleCycle(uint8_t CS, uint8_t Cycle);
void EXTB_SetWriteEnableCycle(uint8_t CS, uint8_t Cycle);
void EXTB_SetWAddressSetupCycle(uint8_t CS, uint8_t Cycle);
void EXTB_SetWriteAccessCycle(uint8_t CS, uint8_t Cycle);
void EXTB_SetReadIdleCycle(uint8_t CS, uint8_t Cycle);
void EXTB_SetFristReadCycle(uint8_t CS, uint8_t Cycle);
void EXTB_SetRAddressSetupCycle(uint8_t CS, uint8_t Cycle);
void EXTB_SetReadAccessCycle(uint8_t CS, uint8_t Cycle);

/* Set AREA0 ~ AREA7 */
void EXTB_SetMaskAddress(uint8_t CS, uint8_t MaskAddr);
void EXTB_SetAccessAddress(uint8_t CS, uint8_t Addr);

#if (MCU_TYPE != DEVICE_TYPE0)
/* Set ATIM0 ~ ATIM7 */
void EXTB_SetAddrLatchEnWidth(uint8_t CS, uint8_t Cycle);
void EXTB_SetAddrLatchEnSetupCycle(uint8_t CS, uint8_t Cycle);
void EXTB_SetAddrLatchCycle(uint8_t CS, uint8_t Cycle);

/* Set DCLKR */
void EXTB_EnableMCLKDivRatio(void);
void EXTB_DisableMCLKDivRatio(void);
void EXTB_SetMCLKDivRatio(uint8_t Div);
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif /* _EXTBUS_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */

