/*******************************************************************************
** 硬件平台: 005电表
** 固件库  ：V1.0
** 文件名  : UserSecPulse.h
** 实现功能: 秒脉冲发生
** 作者    : 微控事业部
** 创建日期: 2013/10/28
** 版权    : 利尔达科技有限公司
** ----------------------------------------------------------------------------
** 修改者:   
** 修改日期: 
** 修改内容:  
*******************************************************************************/
#ifndef __USERSECPULSE_H__
#define __USERSECPULSE_H__

//#define RTC_COEF_INIT               (3372)
#define RTC_COEF_INIT               (2937)		//DTS27-VN31调试值

#define USE_OSC_ADJUST        //使用HCR校准

extern uint8_t g_SecFlag;

typedef struct
{
    uint16_t    Coff;               //偏移量校正值
    uint16_t    Old_Coff;           //偏移对齐计数值
    
}RTC_CalibT;

extern RTC_CalibT RTC_Calib;     //RTC矫正相关参数

typedef struct
{
    uint8_t     Index;              //频率计数器索引
    uint8_t     Reserved;
    uint16_t    HcfCnt;             //高频频率计数器
    uint16_t    HcfCntOld;          //
    uint32_t    HcfAdd;             //高频频率计数器累加器
    uint32_t    SysFreq;            //系统时钟频率
    uint32_t    SysFreq1;           //
    uint32_t    FreqCoef;           //频率系数
    uint16_t    HcfCnt_EMU;             //高频频率计数器,for emu
}Hcr_CalibT;
extern Hcr_CalibT Hcr_Calib;   //Hcr校准相关参数


void DRV_RTCSecPulseInit(void);
void DRV_RTCSecPulsePowerDownInit(void);
void UserBt0_Init(void);
void UserBt1_Init(void);
void UserBT1_MatchCallBack(void);
void UserBT1_UnderflowCallBack(void);
void UserBt7_Init(void);
void RTC_Init(void);
void InitSubout_32768(void);
void RtcWriteCoef(int64_t ppm10);
#endif
