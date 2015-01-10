
/*******************************************************************
* Copyright (c) 2013,深圳市思达仪表有限公司 
* All rights reserved. 
* 
* 文件名称：drv_bt.h 
* 文件标识：见配置管理计划书 
* 摘要：基本定时器移植层

* 当前版本：1.0.0 
* 作者：张玉清
* 完成日期：20131024
*******************************************************************/
#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

/*---------------------------------------------------------------------------*/
/* constants and macros                                                      */
/*---------------------------------------------------------------------------*/
/*! \brief Twrice coefficient of 32768Hz oscillator */
//#define RTC_OSC_TWICE_PARA                (0.034029)//(0.034029) //表3
#define RTC_OSC_TWICE_PARA                (0.0339)//(0.034029) //表3

//#define RTC_OSC_TWICE_PARA                (0.034029)//(0.034029) //表2
//#define RTC_OSC_TWICE_PARA                (0.035)//(0.034029) //表2

//#define RTC_OSC_TWICE_PARA                (0.034029)//(0.034029) //表1
//#define RTC_OSC_TWICE_PARA                (0.035)//(0.034029) //表1

/*! \brief Turn temperature of 32768Hz oscillator */
//#define RTC_OSC_TURN_TEMP                 (25)//(25.2)//表3
#define RTC_OSC_TURN_TEMP                 (23.5)//(25.2)//表3

//#define RTC_OSC_TURN_TEMP                 (25.2)//(25.2)//表2
//#define RTC_OSC_TURN_TEMP                 (23.0)//(25.2)//表2

//#define RTC_OSC_TURN_TEMP                 (25.2)//(25.2)//表1
//#define RTC_OSC_TURN_TEMP                 (23.0)//(25.2)//表1

/*! \brief 1 pulse in 20*32768 pluses */
#define RTC_CALI_PPM_PER_STEP         (1.526)
/*! \brief Center frequency deviation, ((32768-32760)/32760)*10^6 */
#define RTC_32768HZ_PPM_OFFSET        (244.2)
/*! \brief Default frequency tolerance at 25 degree */
#define RTC_NORMAL_DEC_DEFAULT_VAL    0//(12.2)
//#define RTC_NORMAL_DEC_DEFAULT_VAL    1.2//(12.2)//1#
//#define RTC_NORMAL_DEC_DEFAULT_VAL    6.3//(12.2)//2#
//#define RTC_NORMAL_DEC_DEFAULT_VAL    7.3//(12.2)//3#
//#define RTC_NORMAL_DEC_DEFAULT_VAL    1.6//(12.2)//4#
//#define RTC_NORMAL_DEC_DEFAULT_VAL    2.4//(12.2)//5#
//#define RTC_NORMAL_DEC_DEFAULT_VAL    2.4//(12.2)//6#

/*! \brief Address of normal demarcation value */
#define RTC_NORMAL_DEC_VAL_ADDR       (0x0001FFFC)
/*! \brief Count cycle = (32768/RTC_CLOCK_PRECCALER_VALUE)s */
#define RTC_CLOCK_PRECCALER_VALUE     (32760)
/*! \brief Calibraiton period setting [s] */
#define RTC_CLOCK_CALIBRATION_CYCLE     (20)
/*! \brief Rewrite the calibration value after this period elapses [s] */
#define RTC_CLOCK_CALIBRATION_PERIOD     (2)
/*! \brief ADC Channel Number */
#define ADC_CH_NUM                       3//(4)
/*! \brief Select ADC channel */
#define ADC_SMPL_CH                 (AN00|AN01|AN15)//(AN00|AN01|AN02|AN03)
/*! \brief Temperature sample times by ADC per time, set to 1~16 */
#define ADC_TEMP_SAMPLE_TIMES_PER_TIME   (8)


/* Funciton return difinition */
/*! \brief Return normal */
#define FUNC_RET_OK         0
/*! \brief Return NG */
#define FUNC_RET_NG         1

/* Variable flag */
/*! \brief Positive flag */
#define POS_FLAG            0
/*! \brief Negative flag */
#define NEG_FLAG            1

/* Power status */
/*! \brief DC power supply */
#define POWER_DC            0
/*! \brief BAT power supply */
#define POWER_BAT           1
/*! \brief DC power threshold value */
#define POW_VOLT_THRES      (1809) /*  thres value=(3.6*4096*(6.8/(10+6.8)))/3.3 */

/*! \brief Multiply twrice coefficient by 10^6 to make int */
#define  RTC_OSC_TWICE_PARA_A           ((uint32_t)(RTC_OSC_TWICE_PARA*1000000))
/*! \brief Multiply turn temperature by 10 to make int*/
#define  RTC_OSC_TURN_TEMP_A            ((uint32_t)(RTC_OSC_TURN_TEMP*10))
/*! \brief Multiply (1/(20*32768))*10^6 to make int */
#define  RTC_CALI_PPM_PER_STEP_A     ((uint32_t)(RTC_CALI_PPM_PER_STEP*1000))
/*! \brief Multiply center frequency deviation by 10 to make int */
#define  RTC_32768HZ_PPM_OFFSET_A       ((uint32_t)(RTC_32768HZ_PPM_OFFSET*10))
/*! \brief Multiply default frequency tolerance by 10 to make int*/
#define  RTC_NORMAL_DEC_DEFAULT_VAL_A   ((uint32_t)(RTC_NORMAL_DEC_DEFAULT_VAL*10))
//#define  DEFAULT_CRYSTA_PPM             ((int)(RTC_NORMAL_DEC_DEFAULT_VAL*10))
//#define  DEFAULT_CRYSTA_OFFSET          ((int)(RTC_NORMAL_DEC_DEFAULT_VAL*1000))

#define RTC_CAL_CENTER_VAL				160 /* (32768-32760)/32760 = 244.2 */
											/* 1/32768*20 = 1.526 */
											/* 244.2/1.526 = 160 */

/*---------------------------------------------------------------------------*/
/* global valiable                                                           */
/*---------------------------------------------------------------------------*/
typedef struct
{
    short int Slope;               //存放片内温度传感器温度斜率       
    short int T0;                  //存放片内温度传感器0摄氏度时的通道电压
    short int Vol_adc;             //校准温度(25度)下电压ADC值
    short int Ntc_adc;             //校准温度(25度)下NTC热敏电阻的ADC值
    short int R_value;             //分压电阻值
    short int Ntc_R_25;            //校准温度(25度)下的NTC热敏电阻的电阻值
    short int CryFactor;           //存放晶振曲线二次项系数       
    short int Crytemp;             //存放晶振误差的顶点温度
    int32 CryOffset;           		//存放晶振顶点温度下的误差值x1000
    int32 CryOffsetPPM;        		//存放晶振顶点温度偏差ppm数
    short int RTC_state;           //温度补偿使能/禁止
    unsigned short int CRC;
    
}tEE_RTC_Calibrate;
extern tEE_RTC_Calibrate  CalParameter;

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

uint16 CaliProcess(int16 Temp);  
void InitCaliVal(void);
void CaliProcessTask(void);
void DRV_RTCCryOffsetSet(int16 CryOffset);
void AdjustCryOffset(int16 PPMTest);
void PowerDownInitCail(void);
void CalcPPM(int16 Temp);
void CaliProcessTaskPowerDown(void);

#ifdef __cplusplus
}
#endif

#endif /* _RTC_FM3_H_ */
/*****************************************************************************/
/* END OF FILE */
