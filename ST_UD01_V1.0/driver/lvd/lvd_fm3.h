/*!
 ******************************************************************************
 **
 ** \file lvd_fm3.h
 **
 ** \brief head file of low voltage detection driver
 **
 ** 
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2011-12-8
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
 
#ifndef _LVD_FM3_H_
#define _LVD_FM3_H_
 
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"

#if (PERIPHERAL_AVAILABLE_LVD == OFF)
    #if (PERIPHERAL_ENABLE_LVD == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_LVD == ON) && (PERIPHERAL_AVAILABLE_LVD == ON)

/* LVD configuration definition */
/*! \brief LVD type A */
#define   LVD_TYPE_A     0
/*! \brief LVD type B */
#define   LVD_TYPE_B     1
/*! \brief LVD type C */
#define   LVD_TYPE_C     2

/* LVD type extraction */ 
#if (MCU_TYPE == DEVICE_TYPE0) || (MCU_TYPE == DEVICE_TYPE1) || (MCU_TYPE == DEVICE_TYPE2) || \
    (MCU_TYPE == DEVICE_TYPE4) || (MCU_TYPE == DEVICE_TYPE5)
    #define LVD_CONFIG_TYPE     LVD_TYPE_A
#elif (MCU_TYPE == DEVICE_TYPE3) || (MCU_TYPE == DEVICE_TYPE7)
    #define LVD_CONFIG_TYPE     LVD_TYPE_B
#else
    #define LVD_CONFIG_TYPE     LVD_TYPE_C
#endif 
 
/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
#if (LVD_CONFIG_TYPE == LVD_TYPE_A)
    /*! \brief LVD interrupt voltage setting 2.8V */
    #define LVD_VOL_VAL_2_8                      0 // 2.8V
    /*! \brief LVD interrupt voltage setting 3.0V */
    #define LVD_VOL_VAL_3_0                      1 // 3.0V
    /*! \brief LVD interrupt voltage setting 3.2V */
    #define LVD_VOL_VAL_3_2                      2 // 3.2V
    /*! \brief LVD interrupt voltage setting 3.6V */
    #define LVD_VOL_VAL_3_6                      3 // 3.6V
    /*! \brief LVD interrupt voltage setting 3.7V */
    #define LVD_VOL_VAL_3_7                      4 // 3.7V
    /*! \brief LVD interrupt voltage setting 4.0V */
    #define LVD_VOL_VAL_4_0                      7 // 4.0V
    /*! \brief LVD interrupt voltage setting 4.1V */
    #define LVD_VOL_VAL_4_1                      8 // 4.1V
    /*! \brief LVD interrupt voltage setting 4.2V */
    #define LVD_VOL_VAL_4_2                      9 // 4.2V  
    /*! \brief Check parameter */
    #define IS_LVD_VAL(LVDVal)                      ((LVDVal == LVD_VOL_VAL_2_8) \
                                                    ||(LVDVal == LVD_VOL_VAL_3_0)\
                                                    ||(LVDVal == LVD_VOL_VAL_3_2)\
                                                    ||(LVDVal == LVD_VOL_VAL_3_6)\
                                                    ||(LVDVal == LVD_VOL_VAL_3_7)\
                                                    ||(LVDVal == LVD_VOL_VAL_4_0)\
                                                    ||(LVDVal == LVD_VOL_VAL_4_1)\
                                                    ||(LVDVal == LVD_VOL_VAL_4_2))
#elif(LVD_CONFIG_TYPE == LVD_TYPE_B)
    /*! \brief LVD interrupt voltage setting 2.0V */
    #define LVD_VOL_VAL_2_0                      0 // 2.0V
    /*! \brief LVD interrupt voltage setting 2.1V */
    #define LVD_VOL_VAL_2_1                      1 // 2.1V
    /*! \brief LVD interrupt voltage setting 2.2V */
    #define LVD_VOL_VAL_2_2                      2 // 2.2V
    /*! \brief LVD interrupt voltage setting 2.3V */
    #define LVD_VOL_VAL_2_3                      3 // 2.3V
    /*! \brief LVD interrupt voltage setting 2.4V */
    #define LVD_VOL_VAL_2_4                      4 // 2.4V
    /*! \brief LVD interrupt voltage setting 2.5V */
    #define LVD_VOL_VAL_2_5                      5 // 2.5V
    /*! \brief LVD interrupt voltage setting 2.6V */
    #define LVD_VOL_VAL_2_6                      6 // 2.6V
    /*! \brief LVD interrupt voltage setting 2.8V */
    #define LVD_VOL_VAL_2_8                      7 // 2.8V  
    /*! \brief LVD interrupt voltage setting 3.0V */
    #define LVD_VOL_VAL_3_0                      8 // 3.0V
    /*! \brief LVD interrupt voltage setting 3.2V */
    #define LVD_VOL_VAL_3_2                      9 // 3.2V
    /*! \brief LVD interrupt voltage setting 3.6V */
    #define LVD_VOL_VAL_3_6                      10 // 3.6V  
    /*! \brief LVD interrupt voltage setting 3.7V */
    #define LVD_VOL_VAL_3_7                      11 // 3.7V
    /*! \brief LVD interrupt voltage setting 4.0V */
    #define LVD_VOL_VAL_4_0                      12 // 4.0V
    /*! \brief LVD interrupt voltage setting 4.1V */
    #define LVD_VOL_VAL_4_1                      13 // 4.1V  
    /*! \brief LVD interrupt voltage setting 4.2V */
    #define LVD_VOL_VAL_4_2                      14 // 4.2V  
    /*! \brief Check parameter */
    #define IS_LVD_VAL(LVDVal)                      ((LVDVal == LVD_VOL_VAL_2_0) \
                                                    ||(LVDVal == LVD_VOL_VAL_2_1)\
                                                    ||(LVDVal == LVD_VOL_VAL_2_2)\
                                                    ||(LVDVal == LVD_VOL_VAL_2_3)\
                                                    ||(LVDVal == LVD_VOL_VAL_2_4)\
                                                    ||(LVDVal == LVD_VOL_VAL_2_5)\
                                                    ||(LVDVal == LVD_VOL_VAL_2_6)\
                                                    ||(LVDVal == LVD_VOL_VAL_2_8)\
                                                    ||(LVDVal == LVD_VOL_VAL_3_0)\
                                                    ||(LVDVal == LVD_VOL_VAL_3_2)\
                                                    ||(LVDVal == LVD_VOL_VAL_3_6)\
                                                    ||(LVDVal == LVD_VOL_VAL_3_7)\
                                                    ||(LVDVal == LVD_VOL_VAL_4_0)\
                                                    ||(LVDVal == LVD_VOL_VAL_4_1)\
                                                    ||(LVDVal == LVD_VOL_VAL_4_2))
    /*! \brief LVD interrupt voltage setting 1.53V */
    #define LVD_RESET_VOL_1_53                     1 // 1.53V
    /*! \brief LVD interrupt voltage setting 1.93V */
    #define LVD_RESET_VOL_1_93                     4 // 1.93V
    /*! \brief Check parameter */
    #define IS_LVD_RESET_VAL(Val)                  ((Val == LVD_RESET_VOL_1_53) \
                                                    ||(Val == LVD_RESET_VOL_1_93))
    

#elif(LVD_CONFIG_TYPE == LVD_TYPE_C)
    #if (MCU_TYPE == DEVICE_TYPE6) || (MCU_TYPE == DEVICE_TYPE8) 
        /*! \brief LVD interrupt voltage setting 1.70V */
        #define LVD_VOL_VAL_1_70                      4 // 1.70V
        /*! \brief LVD interrupt voltage setting 1.75V */
        #define LVD_VOL_VAL_1_75                      5 // 1.75V
        /*! \brief LVD interrupt voltage setting 1.80V */
        #define LVD_VOL_VAL_1_80                      6 // 1.80V
        /*! \brief LVD interrupt voltage setting 1.85V */
        #define LVD_VOL_VAL_1_85                      7 // 1.85V  
        /*! \brief LVD interrupt voltage setting 1.90V */
        #define LVD_VOL_VAL_1_90                      8 // 1.90V
        /*! \brief LVD interrupt voltage setting 1.95V */
        #define LVD_VOL_VAL_1_95                      9 // 1.95V
        /*! \brief LVD interrupt voltage setting 2.00V */
        #define LVD_VOL_VAL_2_00                      10 // 2.00V  
        /*! \brief LVD interrupt voltage setting 2.05V */
        #define LVD_VOL_VAL_2_05                      11 // 2.05V
        /*! \brief LVD interrupt voltage setting 2.50V */
        #define LVD_VOL_VAL_2_50                      12 // 2.50V
        /*! \brief LVD interrupt voltage setting 2.60V */
        #define LVD_VOL_VAL_2_60                      13 // 2.60V
        /*! \brief LVD interrupt voltage setting 2.70V */
        #define LVD_VOL_VAL_2_70                      14 // 2.70V  
        /*! \brief LVD interrupt voltage setting 2.80V */
        #define LVD_VOL_VAL_2_80                      15 // 2.80V  
        /*! \brief LVD interrupt voltage setting 2.90V */
        #define LVD_VOL_VAL_2_90                      16 // 2.90V
        /*! \brief LVD interrupt voltage setting 3.00V */
        #define LVD_VOL_VAL_3_00                      17 // 3.00V  
        /*! \brief LVD interrupt voltage setting 3.10V */
        #define LVD_VOL_VAL_3_10                      18 // 3.10V  
        /*! \brief LVD interrupt voltage setting 3.20V */
        #define LVD_VOL_VAL_3_20                      19 // 3.20V  
        /*! \brief Check parameter */
        #define IS_LVD_VAL(LVDVal)                      ((LVDVal == LVD_VOL_VAL_1_70)\
                                                        ||(LVDVal == LVD_VOL_VAL_1_75)\
                                                        ||(LVDVal == LVD_VOL_VAL_1_80)\
                                                        ||(LVDVal == LVD_VOL_VAL_1_85)\
                                                        ||(LVDVal == LVD_VOL_VAL_1_90)\
                                                        ||(LVDVal == LVD_VOL_VAL_1_95)\
                                                        ||(LVDVal == LVD_VOL_VAL_2_00)\
                                                        ||(LVDVal == LVD_VOL_VAL_2_05)\
                                                        ||(LVDVal == LVD_VOL_VAL_2_50)\
                                                        ||(LVDVal == LVD_VOL_VAL_2_60)\
                                                        ||(LVDVal == LVD_VOL_VAL_2_70)\
                                                        ||(LVDVal == LVD_VOL_VAL_2_80)\
                                                        ||(LVDVal == LVD_VOL_VAL_2_90)\
                                                        ||(LVDVal == LVD_VOL_VAL_3_00)\
                                                        ||(LVDVal == LVD_VOL_VAL_3_10)\
                                                        ||(LVDVal == LVD_VOL_VAL_3_20))
        /*! \brief LVD reset voltage setting 1.50V */
        #define LVD_RESET_VOL_1_50                      0 // 1.50V
        /*! \brief LVD reset voltage setting 1.55V */
        #define LVD_RESET_VOL_1_55                      1 // 1.55V
        /*! \brief LVD reset voltage setting 1.60V */
        #define LVD_RESET_VOL_1_60                      2 // 1.60V
        /*! \brief LVD reset voltage setting 1.65V */
        #define LVD_RESET_VOL_1_65                      3 // 1.65V
        /*! \brief LVD reset voltage setting 1.70V */
        #define LVD_RESET_VOL_1_70                      4 // 1.70V
        /*! \brief LVD reset voltage setting 1.75V */
        #define LVD_RESET_VOL_1_75                      5 // 1.75V
        /*! \brief LVD reset voltage setting 1.80V */
        #define LVD_RESET_VOL_1_80                      6 // 1.80V
        /*! \brief LVD reset voltage setting 1.85V */
        #define LVD_RESET_VOL_1_85                      7 // 1.85V  
        /*! \brief LVD reset voltage setting 1.90V */
        #define LVD_RESET_VOL_1_90                      8 // 1.90V
        /*! \brief LVD reset voltage setting 1.95V */
        #define LVD_RESET_VOL_1_95                     9 // 1.95V
        /*! \brief LVD reset voltage setting 2.00V */
        #define LVD_RESET_VOL_2_00                      10 // 2.00V  
        /*! \brief LVD reset voltage setting 2.05V */
        #define LVD_RESET_VOL_2_05                      11 // 2.05V
        /*! \brief LVD reset voltage setting 2.50V */
        #define LVD_RESET_VOL_2_50                      12 // 2.50V
        /*! \brief LVD reset voltage setting 2.60V */
        #define LVD_RESET_VOL_2_60                      13 // 2.60V
        /*! \brief LVD reset voltage setting 2.70V */
        #define LVD_RESET_VOL_2_70                      14 // 2.70V  
        /*! \brief LVD reset voltage setting 2.80V */
        #define LVD_RESET_VOL_2_80                      15 // 2.80V  
        /*! \brief LVD reset voltage setting 2.90V */
        #define LVD_RESET_VOL_2_90                      16 // 2.90V
        /*! \brief LVD reset voltage setting 3.00V */
        #define LVD_RESET_VOL_3_00                      17 // 3.00V  
        /*! \brief LVD reset voltage setting 3.10V */
        #define LVD_RESET_VOL_3_10                      18 // 3.10V  
        /*! \brief LVD reset voltage setting 3.20V */
        #define LVD_RESET_VOL_3_20                      19 // 3.20V  
        /*! \brief Check parameter */
        #define IS_LVD_RESET_VAL(LVDVal)                ((LVDVal == LVD_RESET_VOL_1_50)\
                                                        ||(LVDVal == LVD_RESET_VOL_1_55)\
                                                        ||(LVDVal == LVD_RESET_VOL_1_60)\
                                                        ||(LVDVal == LVD_RESET_VOL_1_65)\
                                                        ||(LVDVal == LVD_RESET_VOL_1_70)\
                                                        ||(LVDVal == LVD_RESET_VOL_1_75)\
                                                        ||(LVDVal == LVD_RESET_VOL_1_80)\
                                                        ||(LVDVal == LVD_RESET_VOL_1_85)\
                                                        ||(LVDVal == LVD_RESET_VOL_1_90)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_00)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_05)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_50)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_60)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_70)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_80)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_90)\
                                                        ||(LVDVal == LVD_RESET_VOL_3_00)\
                                                        ||(LVDVal == LVD_RESET_VOL_3_10)\
                                                        ||(LVDVal == LVD_RESET_VOL_3_20))    

    #elif (MCU_TYPE == DEVICE_TYPE9)
        /*! \brief LVD interrupt voltage setting 2.80V */
        #define LVD_VOL_VAL_2_80                      3 // 2.80V  
        /*! \brief LVD interrupt voltage setting 2.90V */
        #define LVD_VOL_VAL_3_00                      4 // 2.90V
        /*! \brief LVD interrupt voltage setting 3.00V */
        #define LVD_VOL_VAL_3_20                      5 // 3.00V  
        /*! \brief LVD interrupt voltage setting 3.10V */
        #define LVD_VOL_VAL_3_60                      6 // 3.10V  
        /*! \brief LVD interrupt voltage setting 3.20V */
        #define LVD_VOL_VAL_3_70                      7 // 3.20V  
        /*! \brief LVD interrupt voltage setting 3.00V */
        #define LVD_VOL_VAL_4_00                      8 // 3.00V  
        /*! \brief LVD interrupt voltage setting 3.10V */
        #define LVD_VOL_VAL_4_10                      9 // 3.10V  
        /*! \brief LVD interrupt voltage setting 3.20V */
        #define LVD_VOL_VAL_4_20                      10 // 3.20V  
        /*! \brief Check parameter */
        #define IS_LVD_VAL(LVDVal)                      ((LVDVal == LVD_VOL_VAL_2_80)\
                                                        ||(LVDVal == LVD_VOL_VAL_3_00)\
                                                        ||(LVDVal == LVD_VOL_VAL_3_20)\
                                                        ||(LVDVal == LVD_VOL_VAL_3_60)\
                                                        ||(LVDVal == LVD_VOL_VAL_3_70)\
                                                        ||(LVDVal == LVD_VOL_VAL_4_00)\
                                                        ||(LVDVal == LVD_VOL_VAL_4_10)\
                                                        ||(LVDVal == LVD_VOL_VAL_4_20))
        /*! \brief LVD reset voltage setting 2.45V */
        #define LVD_RESET_VOL_2_45                      0 // 2.45V
        /*! \brief LVD reset voltage setting 2.60V */
        #define LVD_RESET_VOL_2_60                      1 // 2.60V
        /*! \brief LVD reset voltage setting 2.70V */
        #define LVD_RESET_VOL_2_70                      2 // 2.70V
        /*! \brief LVD reset voltage setting 2.80V */
        #define LVD_RESET_VOL_2_80                      3 // 2.80V
        /*! \brief LVD reset voltage setting 3.00V */
        #define LVD_RESET_VOL_3_00                      4 // 3.00V
        /*! \brief LVD reset voltage setting 3.20V */
        #define LVD_RESET_VOL_3_20                      5 // 3.20V
        /*! \brief LVD reset voltage setting 3.60V */
        #define LVD_RESET_VOL_3_60                      6 // 3.60V
        /*! \brief LVD reset voltage setting 3.70V */
        #define LVD_RESET_VOL_3_70                      7 // 3.70V
        /*! \brief LVD reset voltage setting 4.00V */
        #define LVD_RESET_VOL_4_00                      8 // 4.00V
        /*! \brief LVD reset voltage setting 4.10V */
        #define LVD_RESET_VOL_4_10                      9 // 4.10V
        /*! \brief LVD reset voltage setting 4.20V */
        #define LVD_RESET_VOL_4_20                      10 // 4.20V

        /*! \brief Check parameter */
        #define IS_LVD_RESET_VAL(LVDVal)                ((LVDVal == LVD_RESET_VOL_2_45)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_60)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_70)\
                                                        ||(LVDVal == LVD_RESET_VOL_2_80)\
                                                        ||(LVDVal == LVD_RESET_VOL_3_00)\
                                                        ||(LVDVal == LVD_RESET_VOL_3_60)\
                                                        ||(LVDVal == LVD_RESET_VOL_3_70)\
                                                        ||(LVDVal == LVD_RESET_VOL_4_00)\
                                                        ||(LVDVal == LVD_RESET_VOL_4_10)\
                                                        ||(LVDVal == LVD_RESET_VOL_4_20))
    #endif
#endif

#if (LVD_CONFIG_TYPE == LVD_TYPE_B)
/* LVD Low power mode definition */
typedef enum
{
    LPC_NORMAL_MODE = 0,  //!<  normal mode 
    LPC_LOW_POWER_MODE,   //!<  low power mode 
  
}LVD_LowPowerModeT;
/* Check parameter */
#define IS_LPC_LPM(Mode)        ((Mode == LPC_NORMAL_MODE) \
                                 ||(Mode == LPC_LOW_POWER_MODE))    
#endif


/*! \brief Disable write protection mode of LVD_CTL */
#define LVD_UNLOCK		                 ((uint32_t)0x1acce553)


/*!
 ******************************************************************************
 ** \brief LVD Interrupt callback function definition
 ******************************************************************************
 */
typedef void (LVD_IntHandlerCallbackT) (void);
                                                 
/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void LVD_EnableInt(LVD_IntHandlerCallbackT* IntCallback);
void LVD_DisableInt(void);
void LVD_SetDetectVolt(uint8_t VoltLevel);
IntStatusT LVD_GetIntFlag(void);
void  LVD_ClrIntFlag (void);
FlagStatusT LVD_GetIntRdyStat(void);
void LVD_UnlockCtrlReg(void);
void LVD_LockCtrlReg(void);
#if (LVD_CONFIG_TYPE == LVD_TYPE_B) || (LVD_CONFIG_TYPE == LVD_TYPE_C)
void LVD_EnableReset(void);
void LVD_DisableReset(void);
void LVD_SetResetVolt(uint8_t VoltLevel);
FlagStatusT LVD_GetResetRdyStat(void);
#endif
#if (LVD_CONFIG_TYPE == LVD_TYPE_B)
void LVD_ConfigLowPowerMode(LVD_LowPowerModeT Mode);
#endif
void LVD_IntHandler(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* _LVD_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
 
