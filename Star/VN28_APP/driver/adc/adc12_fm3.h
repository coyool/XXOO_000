/*!
 ******************************************************************************
 **
 ** \file adc12_fm3.h
 **
 ** \brief adc drivers head file
 **
 ** \author FSLA AE Team
 **
 ** \version V0.21
 **
 ** \date 2012-08-16   
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

#ifndef _adc12_fm3_H_
#define _adc12_fm3_H_
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "fm3_common.h"
#include "pdl_user.h"
#include "debug.h"


#if (PERIPHERAL_AVAILABLE_ADC == OFF)
    #if (PERIPHERAL_ENABLE_ADC == ON) 
        #error Peripheral not available, do not enable it!
    #endif
#endif

#if (PERIPHERAL_ENABLE_ADC == ON) && (PERIPHERAL_AVAILABLE_ADC == ON)

/*---------------------------------------------------------------------------*/
/* Extract ADC type                                                          */
/*---------------------------------------------------------------------------*/
#if (MCU_TYPE == DEVICE_TYPE0) || (MCU_TYPE == DEVICE_TYPE1) || (MCU_TYPE == DEVICE_TYPE2) \
    || (MCU_TYPE == DEVICE_TYPE4) || (MCU_TYPE == DEVICE_TYPE5)
    #define ADC_CONFIG_TYPE      ADC_TYPE_A
#else
    #define ADC_CONFIG_TYPE      ADC_TYPE_B
#endif


/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/

/* ADC unit definition */
/*! \brief ADC unit 0 */ 
#define     ADC12_UNIT0     (0)
/*! \brief ADC unit 1 */ 
#define     ADC12_UNIT1     (1)
/*! \brief ADC unit 2 */ 
#define     ADC12_UNIT2     (2)

/*! \brief Check ADC unit parameter */ 
#define IS_ADC_UNIT_VALID(Unit) ((Unit == ADC12_UNIT0)\
                                ||(Unit == ADC12_UNIT1)\
                                ||(Unit == ADC12_UNIT2))

/* ADC channel definition */
#define     ADC12_CH0        (0)     //!< ADC channel 0
#define     ADC12_CH1        (1)     //!< ADC channel 1
#define     ADC12_CH2        (2)     //!< ADC channel 2
#define     ADC12_CH3        (3)     //!< ADC channel 3
#define     ADC12_CH4        (4)     //!< ADC channel 4
#define     ADC12_CH5        (5)     //!< ADC channel 5
#define     ADC12_CH6        (6)     //!< ADC channel 6
#define     ADC12_CH7        (7)     //!< ADC channel 7
#define     ADC12_CH8        (8)     //!< ADC channel 8
#define     ADC12_CH9        (9)     //!< ADC channel 9
#define     ADC12_CH10       (10)    //!< ADC channel 10
#define     ADC12_CH11       (11)    //!< ADC channel 11
#define     ADC12_CH12       (12)    //!< ADC channel 12
#define     ADC12_CH13       (13)    //!< ADC channel 13
#define     ADC12_CH14       (14)    //!< ADC channel 14
#define     ADC12_CH15       (15)    //!< ADC channel 15
#define     ADC12_CH16       (16)    //!< ADC channel 16
#define     ADC12_CH17       (17)    //!< ADC channel 17
#define     ADC12_CH18       (18)    //!< ADC channel 18
#define     ADC12_CH19       (19)    //!< ADC channel 19
#define     ADC12_CH20       (20)    //!< ADC channel 20
#define     ADC12_CH21       (21)    //!< ADC channel 21
#define     ADC12_CH22       (22)    //!< ADC channel 22
#define     ADC12_CH23       (23)    //!< ADC channel 23
#define     ADC12_CH24       (24)    //!< ADC channel 24
#define     ADC12_CH25       (25)    //!< ADC channel 25
#define     ADC12_CH26       (26)    //!< ADC channel 26
#define     ADC12_CH27       (27)    //!< ADC channel 27
#define     ADC12_CH28       (28)    //!< ADC channel 28
#define     ADC12_CH29       (29)    //!< ADC channel 29
#define     ADC12_CH30       (30)    //!< ADC channel 30
#define     ADC12_CH31       (31)    //!< ADC channel 31
                                

/*! \brief Check ADC channel parameter */ 
#define IS_ADC_CH_VALID(CH) ((CH == ADC12_CH0)||(CH == ADC12_CH1)||(CH == ADC12_CH2)||(CH == ADC12_CH3)\
                            ||(CH == ADC12_CH4)||(CH == ADC12_CH5)||(CH == ADC12_CH6)||(CH == ADC12_CH7)\
                            ||(CH == ADC12_CH8)||(CH == ADC12_CH9)||(CH == ADC12_CH10)||(CH == ADC12_CH11)\
                            ||(CH == ADC12_CH12)||(CH == ADC12_CH13)||(CH == ADC12_CH14)||(CH == ADC12_CH15)\
                            ||(CH == ADC12_CH16)||(CH == ADC12_CH17)||(CH == ADC12_CH18)||(CH == ADC12_CH19)\
                            ||(CH == ADC12_CH20)||(CH == ADC12_CH21)||(CH == ADC12_CH22)||(CH == ADC12_CH23)\
                            ||(CH == ADC12_CH24)||(CH == ADC12_CH25)||(CH == ADC12_CH26)||(CH == ADC12_CH27)\
                            ||(CH == ADC12_CH28)||(CH == ADC12_CH29)||(CH == ADC12_CH30)||(CH == ADC12_CH31))
                                                        
/*! \brief Check parameter Channel for priority 1 */ 
#define IS_ADC_P1_CH_VALID(CH) ((CH == ADC12_CH0)||(CH == ADC12_CH1)||(CH == ADC12_CH2)\
                                ||(CH == ADC12_CH3)||(CH == ADC12_CH4)||(CH == ADC12_CH5)\
                                ||(CH == ADC12_CH6)||(CH == ADC12_CH7))
                           
/*! \brief Check parameter Channel for priority 2 */ 
#define IS_ADC_P2_CH_VALID(CH) ((CH == ADC12_CH0)||(CH == ADC12_CH1)||(CH == ADC12_CH2)||(CH == ADC12_CH3)\
                                ||(CH == ADC12_CH4)||(CH == ADC12_CH5)||(CH == ADC12_CH6)||(CH == ADC12_CH7)\
                                ||(CH == ADC12_CH8)||(CH == ADC12_CH9)||(CH == ADC12_CH10)||(CH == ADC12_CH11)\
                                ||(CH == ADC12_CH12)||(CH == ADC12_CH13)||(CH == ADC12_CH14)||(CH == ADC12_CH15)\
                                ||(CH == ADC12_CH16)||(CH == ADC12_CH17)||(CH == ADC12_CH18)||(CH == ADC12_CH19)\
                                ||(CH == ADC12_CH20)||(CH == ADC12_CH21)||(CH == ADC12_CH22)||(CH == ADC12_CH23)\
                                ||(CH == ADC12_CH24)||(CH == ADC12_CH25)||(CH == ADC12_CH26)||(CH == ADC12_CH27)\
                                ||(CH == ADC12_CH28)||(CH == ADC12_CH29)||(CH == ADC12_CH30)||(CH == ADC12_CH31))

/*! \brief Check parameter Channel for compare */ 
#define IS_ADC_CMP_CH_VALID(CH) ((CH == ADC12_CH0)||(CH == ADC12_CH1)||(CH == ADC12_CH2)||(CH == ADC12_CH3)\
                                ||(CH == ADC12_CH4)||(CH == ADC12_CH5)||(CH == ADC12_CH6)||(CH == ADC12_CH7)\
                                ||(CH == ADC12_CH8)||(CH == ADC12_CH9)||(CH == ADC12_CH10)||(CH == ADC12_CH11)\
                                ||(CH == ADC12_CH12)||(CH == ADC12_CH13)||(CH == ADC12_CH14)||(CH == ADC12_CH15)\
                                ||(CH == ADC12_CH16)||(CH == ADC12_CH17)||(CH == ADC12_CH18)||(CH == ADC12_CH19)\
                                ||(CH == ADC12_CH20)||(CH == ADC12_CH21)||(CH == ADC12_CH22)||(CH == ADC12_CH23)\
                                ||(CH == ADC12_CH24)||(CH == ADC12_CH25)||(CH == ADC12_CH26)||(CH == ADC12_CH27)\
                                ||(CH == ADC12_CH28)||(CH == ADC12_CH29)||(CH == ADC12_CH30)||(CH == ADC12_CH31))



/*!
 ******************************************************************************
 ** \brief define FIFO data alignment 
 ******************************************************************************
 */
typedef enum
{
    ADC12_FIFO_DATA_MSB_ALIGN,     //!< Places conversion data on the MSB side
    ADC12_FIFO_DATA_LSB_ALIGN = 1, //!< Places conversion data on the LSB side
}ADC12_FIFODataAlignT;


/*!
 ******************************************************************************
 ** \brief  Check parameter FIFO data align mode
 ******************************************************************************
 */

#define IS_ADC_FIFO_DATA_VALID(Mode)    ((Mode == ADC12_FIFO_DATA_MSB_ALIGN)\
                                        ||(Mode == ADC12_FIFO_DATA_LSB_ALIGN))
/*!
 ******************************************************************************
 ** \brief define adc sampling time n 
 ******************************************************************************
 */
typedef enum
{
    ADC12_SAMPLE_TIME_N1 =   0, //!< ADC sampling time 1 N
    ADC12_SAMPLE_TIME_N4 =   1, //!< ADC sampling time 4 N
    ADC12_SAMPLE_TIME_N8 =   2, //!< ADC sampling time 8 N
    ADC12_SAMPLE_TIME_N16 =  3, //!< ADC sampling time 16 N
    ADC12_SAMPLE_TIME_N32 =  4, //!< ADC sampling time 32 N
    ADC12_SAMPLE_TIME_N64 =  5, //!< ADC sampling time 64 N
    ADC12_SAMPLE_TIME_N128 = 6, //!< ADC sampling time 128 N
    ADC12_SAMPLE_TIME_N256 = 7, //!< ADC sampling time 256 N
}ADC12_SamplingTimeNT;

/*! \brief Check ADC sample time parameter */ 
#define IS_ADC_SAMPEL_TIMEN_VALID(SampleTime) ((SampleTime == ADC12_SAMPLE_TIME_N1)\
                                            ||(SampleTime == ADC12_SAMPLE_TIME_N1)\
                                            ||(SampleTime == ADC12_SAMPLE_TIME_N4)\
                                            ||(SampleTime == ADC12_SAMPLE_TIME_N8)\
                                            ||(SampleTime == ADC12_SAMPLE_TIME_N16)\
                                            ||(SampleTime == ADC12_SAMPLE_TIME_N32)\
                                            ||(SampleTime == ADC12_SAMPLE_TIME_N64)\
                                            ||(SampleTime == ADC12_SAMPLE_TIME_N128)\
                                            ||(SampleTime == ADC12_SAMPLE_TIME_N256))

/*!
 ******************************************************************************
 ** \brief define adc sampling time select region
 ******************************************************************************
 */
typedef enum
{
    ADC12_SAMPLE_TIME_SEL_REG0 = 0,  //!< ADC sampling time select region 0
    ADC12_SAMPLE_TIME_SEL_REG1 = 1,  //!< ADC sampling time select region 1
}ADC12_STSelRegT;

/*! \brief Check select parameter */ 
#define IS_ADC_ST_SEL_VALID(Select)      ((Select == ADC12_SAMPLE_TIME_SEL_REG0)\
                                            ||(Select == ADC12_SAMPLE_TIME_SEL_REG1))
/*! \brief Check compare time parameter */ 
#define IS_ADC_DIV_VALID(Div) ((Div) >= 1)

/*!
 ******************************************************************************
 ** \brief define adc scan mode choose:single or repeat
 ******************************************************************************
 */
typedef enum
{
    ADC12_SCAN_MODE_ONE_SHOT = 0,  //!< ADC conversion one-shot mode
    ADC12_SCAN_MODE_REPEAT = 1,    //!< ADC conversion repeat mode
}ADC12_ScanModeT;

/*! \brief Check scan mode parameter */ 
#define IS_ADC_SCAN_MODE_VALID(ScanMode) ((ScanMode == ADC12_SCAN_MODE_ONE_SHOT)\
                                        ||(ScanMode == ADC12_SCAN_MODE_REPEAT))

/*!
 ******************************************************************************
 ** \brief Scan FIFO information structure
 ******************************************************************************
 */
typedef struct 
{
    uint8_t DataValid:1;  //!< Data valid flag
    uint8_t TrigMode:2;   //!< trigger mode 
    uint8_t Ch:5;         //!< currect channel
}ADC_ScanFIFOInfoT;

/* Data validity definition */
/*! \brief data valid */ 
#define ADC12_SCAN_DATA_VALID            (0)
/*! \brief data invalid */ 
#define ADC12_SCAN_DATA_INVALID          (1)

/* trigger mode definition */
/*! \brief trigger by software */ 
#define ADC12_SCAN_TRIG_BY_SW               (0)
/*! \brief trigger by timer */ 
#define ADC12_SCAN_TRIG_BY_TIMER            (1)

/*!
 ******************************************************************************
 ** \brief define adc  conversion timer trigger selection
 ******************************************************************************
 */
typedef enum
{ 
    ADC12_TIMER_TRIGGER_NONE = 0,  	//!<No selected trigger (Input is fixed to "0".)
    ADC12_TIMER_TRIGGER_MFT = 1, 	//!<Starts scan conversion with the multifunction timer.
    ADC12_TIMER_TRIGGER_BT0 = 2, 	//!<Base timer ch.0
    ADC12_TIMER_TRIGGER_BT1 = 3, 	//!<Base timer ch.1
    ADC12_TIMER_TRIGGER_BT2 = 4, 	//!<Base timer ch.2
    ADC12_TIMER_TRIGGER_BT3 = 5, 	//!<Base timer ch.3
    ADC12_TIMER_TRIGGER_BT4 = 6, 	//!<Base timer ch.4
    ADC12_TIMER_TRIGGER_BT5 = 7, 	//!<Base timer ch.5
    ADC12_TIMER_TRIGGER_BT6 = 8, 	//!<Base timer ch.6
    ADC12_TIMER_TRIGGER_BT7 = 9, 	//!<Base timer ch.7
}ADC12_TrigTimerChT;

/*! \brief Check trigger mode parameter */ 
#define IS_ADC_TRIGGER_MODE_CH(Trigger)     ((Trigger == ADC12_TIMER_TRIGGER_NONE)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_MFT)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_BT0)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_BT1)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_BT2)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_BT3)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_BT4)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_BT5)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_BT6)\
                                            ||(Trigger == ADC12_TIMER_TRIGGER_BT7))
/*!
 ******************************************************************************
 ** \brief define adc FIFO stage counter(scan)
 ******************************************************************************
 */
typedef enum
{
    ADC12_SCAN_FIFO_STAGE1=0,//!< ADC FIFO stage 1
    ADC12_SCAN_FIFO_STAGE2=1,//!< ADC FIFO stage 2
    ADC12_SCAN_FIFO_STAGE3=2,//!< ADC FIFO stage 3
    ADC12_SCAN_FIFO_STAGE4=3,//!< ADC FIFO stage 4
    ADC12_SCAN_FIFO_STAGE5=4,//!< ADC FIFO stage 5
    ADC12_SCAN_FIFO_STAGE6=5,//!< ADC FIFO stage 6
    ADC12_SCAN_FIFO_STAGE7=6,//!< ADC FIFO stage 7
    ADC12_SCAN_FIFO_STAGE8=7,//!< ADC FIFO stage 8
    ADC12_SCAN_FIFO_STAGE9=8,//!< ADC FIFO stage 9
    ADC12_SCAN_FIFO_STAGE10=9,//!< ADC FIFO stage 10
    ADC12_SCAN_FIFO_STAGE11=10,//!< ADC FIFO stage 11
    ADC12_SCAN_FIFO_STAGE12=11,//!< ADC FIFO stage 12
    ADC12_SCAN_FIFO_STAGE13=12,//!< ADC FIFO stage 13
    ADC12_SCAN_FIFO_STAGE14=13,//!< ADC FIFO stage 14
    ADC12_SCAN_FIFO_STAGE15=14,//!< ADC FIFO stage 15
    ADC12_SCAN_FIFO_STAGE16=15,//!< ADC FIFO stage 16
}ADC12_ScanFIFOStageT;

/*! \brief Check scan mode FIFO stage parameter */ 
#define IS_ADC_SCAN_FIFO_STAGE_VALID(FIFOStage) ((FIFOStage == ADC12_SCAN_FIFO_STAGE1)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE2)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE3)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE4)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE5)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE6)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE7)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE8)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE9)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE10)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE11)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE12)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE13)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE14)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE15)\
                                                ||(FIFOStage == ADC12_SCAN_FIFO_STAGE16))
/*!
 ******************************************************************************
 ** \brief define adc FIFO stage counter(priority)
 ******************************************************************************
 */
typedef enum
{
    ADC12_PRIORITY_FIFO_STAGE1=0,//!< ADC FIFO stage 1
    ADC12_PRIORITY_FIFO_STAGE2=1,//!< ADC FIFO stage 2
    ADC12_PRIORITY_FIFO_STAGE3=2,//!< ADC FIFO stage 3
    ADC12_PRIORITY_FIFO_STAGE4=3,//!< ADC FIFO stage 4
}ADC12_PriorityFIFOStageT;

/*! \brief Check Priority mode FIFO stage parameter */ 
#define IS_ADC_P_FIFO_STAGE_VALID(FIFOStage) ((FIFOStage == ADC12_PRIORITY_FIFO_STAGE1)\
                                            ||(FIFOStage == ADC12_PRIORITY_FIFO_STAGE2)\
                                            ||(FIFOStage == ADC12_PRIORITY_FIFO_STAGE3)\
                                            ||(FIFOStage == ADC12_PRIORITY_FIFO_STAGE4))

/*!
 ******************************************************************************
 ** \brief define priority FIFO information structure
 ******************************************************************************
 */
typedef struct 
{
    uint8_t DataValid:1;  //!< Data valid flag
    uint8_t TrigMode:2;   //!< Trigger mode
    uint8_t Ch:5;         //!< Current channel
}ADC_PrioFIFOInfoT;

/* Data validity definition */
/*! \brief data valid */ 
#define ADC12_PRIO_DATA_VALID            (0)
/*! \brief data invalid */ 
#define ADC12_PRIO_DATA_INVALID          (1)

/* trigger mode definition */
/*! \brief trigger by software */ 
#define ADC12_PRIO_TRIG_BY_SW               (0)
/*! \brief trigger by timer */ 
#define ADC12_PRIO_TRIG_BY_TIMER            (1)
/*! \brief trigger by external input */ 
#define ADC12_PRIO_TRIG_BY_EXT_INPUT        (2)

/*!
 ******************************************************************************
 ** \brief define priority external trigger setting selection
 ******************************************************************************
 */
typedef enum
{
    ADC12_PRIO_EXT_TRIG_P1A = 0,     //!< ADC priority external trigger setting by P1A
    ADC12_PRIO_EXT_TRIG_EXT_INPUT,   //!< ADC priority external trigger setting by ext input  
}ADC12_PrioExtTrigSelT;

/*! \brief Check trigger mode parameter */ 
#define IS_ADC_PRIO_EXT_TRIG_SEL_VALID(Mode) ((Mode == ADC12_PRIO_EXT_TRIG_P1A)\
                                             ||(Mode == ADC12_PRIO_EXT_TRIG_EXT_INPUT))

/*!
 ******************************************************************************
 ** \brief define compare mode 0
 ******************************************************************************
 */
typedef enum 
{
    ADC12_CMP_SINGLE_CH = 0, //!< ADC comparison all channel
    ADC12_CMP_ALL_CH = 1,    //!< ADC comparison enable
} ADC12_CmpMode0T;

/*! \brief Check mode0 parameter */ 
#define IS_ADC12_CMP_MODE0_VALID(Mode) ((Mode == ADC12_CMP_SINGLE_CH) \
                                        || (Mode == ADC12_CMP_ALL_CH))
/*!
 ******************************************************************************
 ** \brief define compare mode 1
 ******************************************************************************
 */
typedef enum 
{
    ADC12_CMP_SMALL = 0,       //!< if smaller than set value, int occurs
    ADC12_CMP_EQU_MORE = 1,    //!< if equ or more than set value, int occurs
} ADC12_CmpMode1T;

/*! \brief Check mode1 parameter */ 
#define IS_ADC12_CMP_MODE1_VALID(Mode) ((Mode == ADC12_CMP_EQU_MORE) \
                                        || (Mode == ADC12_CMP_SMALL))


/*------------ Interrupt callback function type definition ------------------*/ 
/*! \brief FIFO OVE callback function type definition  */ 
typedef void ADC_FIFOOVECallbackT(uint8_t OVEIntType);
/*! \brief ADC scan callback function type definition  */ 
typedef void ADC_ScanCallbackT(void);
/*! \brief ADC priority callback function type definition  */ 
typedef void ADC_PrioCallbackT(void);
/*! \brief ADC compare callback function type definition  */ 
typedef void ADC_CmpCallbackT(void);

/* FIFO OVE interrupt type */
/*! \brief scan FIFO OVE interrupt mode  */ 
#define ADC12_SCAN_OVE_MODE      0
/*! \brief Priority FIFO OVE interrupt mode  */
#define ADC12_PRIO_OVE_MODE      1

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/* Global */
void ADC12_ForceStop(uint8_t Unit);
void ADC12_SelFIFODataLoc(uint8_t Unit, ADC12_FIFODataAlignT Loc);
FlagStatusT ADC12_GetPrioPendingFlag(uint8_t Unit);
FlagStatusT ADC12_GetPrioStatusFlag(uint8_t Unit);
FlagStatusT ADC12_GetScanStatusFlag(uint8_t Unit);
void ADC12_SelSampleTimeReg(uint8_t Unit, uint8_t Ch, ADC12_STSelRegT RegSel);
void ADC12_SetSampleTime0(uint8_t Unit, uint8_t BitsST, ADC12_SamplingTimeNT BitsSTX);
void ADC12_SetSampleTime1(uint8_t Unit, uint8_t BitsST, ADC12_SamplingTimeNT BitsSTX);
void ADC12_SetCompareTime(uint8_t Unit,  uint8_t Div);
void ADC12_EnableFIFOOVEInt(uint8_t Unit, ADC_FIFOOVECallbackT* IntCallback);
void ADC12_DisableFIFOOVEInt(uint8_t Unit);
void ADC12_EnableOpt(uint8_t Unit);
void ADC12_DisableOpt(uint8_t Unit);
FlagStatusT ADC12_GetOptStat(uint8_t Unit);
#if(ADC_CONFIG_TYPE == ADC_TYPE_B)  
void ADC12_SetOptStableTrsfTime(uint8_t Unit, uint8_t EnableTime);
#endif
/* Scan conversion */
FlagStatusT ADC12_GetScanFIFOEmptyFlag(uint8_t Unit);
FlagStatusT ADC12_GetScanFIFOFullFlag(uint8_t Unit);
FlagStatusT ADC12_GetScanFIFOOVRFlag(uint8_t Unit);
void ADC12_ClrScanFIFO(uint8_t Unit);
void ADC12_SetScanMode(uint8_t Unit, ADC12_ScanModeT Mode);
void ADC12_EnableScanTimerTrig(uint8_t Unit);
void ADC12_DisableScanTimerTrig(uint8_t Unit);
void ADC12_SelScanTrigTimerCh(uint8_t Unit, ADC12_TrigTimerChT TimerCh);
void ADC12_StartScan(uint8_t Unit);
void ADC12_SetScanFIFOStage(uint8_t Unit, ADC12_ScanFIFOStageT Stage);
void ADC12_GetScanFIFOInfo(uint8_t Unit, ADC_ScanFIFOInfoT* ScanFIFOInfo);
uint16_t ADC12_GetScanFIFOData(uint8_t Unit);
void ADC12_SelScanCh(uint8_t Unit, uint32_t ChIndex);
void ADC12_EnableScanInt(uint8_t Unit, ADC_ScanCallbackT* IntCallBack);
void ADC12_DisableScanInt(uint8_t Unit);
IntStatusT ADC12_GetScanIntFlag(uint8_t Unit);
void ADC12_ClrScanIntFlag(uint8_t Unit);
/* Priority conversion */
FlagStatusT ADC12_GetPrioFIFOEmptyFlag(uint8_t Unit);
FlagStatusT ADC12_GetPrioFIFOFullFlag(uint8_t Unit);
FlagStatusT ADC12_GetPrioFIFOOVRFlag(uint8_t Unit);
void ADC12_ClrPrioFIFO(uint8_t Unit);
void ADC12_SelPrioExtTrigMode(uint8_t Unit, ADC12_PrioExtTrigSelT Mode);
void ADC12_EnablePrioExtTrig(uint8_t Unit);
void ADC12_DisablePrioExtTrig(uint8_t Unit);
void ADC12_EnablePrioTimerTrig(uint8_t Unit);
void ADC12_DisablePrioTimerTrig(uint8_t Unit);
void ADC12_SelPrioTrigTimerCh(uint8_t Unit, ADC12_TrigTimerChT TimerCh);
void ADC12_SelPrioLevel1Ch(uint8_t Unit, uint8_t Ch);
void ADC12_SelPrioLevel2Ch(uint8_t Unit, uint8_t Ch);
void ADC12_StartPrio(uint8_t Unit);
void ADC12_SetPrioFIFOStage(uint8_t Unit, ADC12_PriorityFIFOStageT Stage);
void ADC12_GetPrioFIFOInfo(uint8_t Unit, ADC_PrioFIFOInfoT* PrioFIFOInfo);
uint16_t ADC12_GetPrioFIFOData(uint8_t Unit);
void ADC12_EnablePrioInt(uint8_t Unit, ADC_PrioCallbackT* IntCallBack);
void ADC12_DisablePrioInt(uint8_t Unit);
IntStatusT ADC12_GetPrioIntFlag(uint8_t Unit);
void ADC12_ClrPrioIntFlag(uint8_t Unit);
/* Comparison function */
void ADC12_SetCmpVal(uint8_t Unit, uint16_t CmpVal);
void ADC12_SetCmpCh(uint8_t Unit, uint8_t Ch);
void ADC12_SetCmpMode0(uint8_t Unit, ADC12_CmpMode0T Mode0);
void ADC12_SetCmpMode1(uint8_t Unit, ADC12_CmpMode1T Mode1);
void ADC12_EnableCmp(uint8_t Unit);
void ADC12_DisableCmp(uint8_t Unit);
void ADC12_EnableCmpInt(uint8_t Unit, ADC_CmpCallbackT* IntCallBack);
void ADC12_DisableCmpInt(uint8_t Unit);
IntStatusT ADC12_GetCmpIntFlag(uint8_t Unit);
/* Interrupt handler */ 
void ADC_IntHandler(uint8_t Unit, uint8_t Irq);
#ifdef __cplusplus
}
#endif

#endif

#endif /* _Adc12_fm3_H_ */
