/*******************************************************************************
** 硬件平台: 005电表
** 固件库  ：V1.0
** 文件名  : UserSecPulse.c
** 实现功能: 秒脉冲产生
** 作者    : 微控事业部
** 创建日期: 2013/10/28
** 版权    : 利尔达科技有限公司
** ----------------------------------------------------------------------------
** 修改者:   
** 修改日期: 
** 修改内容:  
*******************************************************************************/
#include "Include.h"

RTC_CalibT RTC_Calib;
Hcr_CalibT Hcr_Calib;
uint8_t g_SecFlag;

void RTC_HcrCalib(void);

//void SysFrqTest(void)
//{
//    LDRV_UsartSend(UART_485, (uint8 *)&Hcr_Calib.FreqCoef, 4);
//}

void RTC_Test_Subout(void)
{
    IO_EnableFunc(IO_PORT0, IO_PINxF); 
	IO_ConfigFuncSUBOUTxPin(IO_SUBOUT_SUBOUT_0);
}
/*********************************************************** 
函数功能：RTC秒脉冲初始化
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_RTCSecPulseInit(void)
{
  //秒脉冲
  UserBt0_Init();
  UserBt1_Init();
  //系统时钟校准
  //Hcr校准参数初始化
  Hcr_Calib.Index     = 0;            
  Hcr_Calib.HcfCnt    = 0;            
  Hcr_Calib.HcfCntOld = 0;            
  Hcr_Calib.HcfAdd    = 0;            
  Hcr_Calib.SysFreq   = 16000000;     
  Hcr_Calib.FreqCoef  = 10000;        
  UserBt7_Init();
  TaskAdd(RTC_HcrCalib, 0, 1000, 1);
  //TaskAdd(SysFrqTest, 10, 4000);

//  RTC_Test_Subout();	//test
}

/*********************************************************** 
函数功能：RTC秒脉冲初始化
入口参数：
出口参数：
备注说明:
***********************************************************/
void DRV_RTCSecPulsePowerDownInit(void)
{
  //秒脉冲
  BT_PWMDisableCount(BT_CH_0); 
  //使能匹配中断
  BT_PWMDisableDutyMatchInt(BT_CH_1);
  //使能下溢出中断
  BT_PWMDisableUnderflowInt(BT_CH_1);
  /* Enable count operatoin */
  BT_PWMDisableCount(BT_CH_1);  
 
  BT_RTDisableCount(BT_CH_7);
}

/*******************************************************************************
** 函数名  : UserBt0_Init
** 功能    : BT0初始化
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/10/28
-------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void UserBt0_Init(void)
{
  BT_PWMInitRegT UserBT_PWMInitReg;
  
  //初始化Subout 32768Hz
  InitSubout_32768();
  
  //IO配置: BT0时钟源选择SUBOUT
  IO_ConfigFuncBTTIOBxInputPin(IO_BT_CH0,IO_BT_TIOBx_INPUT_SUBOUT);
  
  /* Set initialization data */
  UserBT_PWMInitReg.Cycle = 0xFFFF;    //Cycle = T*(m+1) = us @ PCLK1 = 16MHz
  UserBT_PWMInitReg.Duty = 0xFFFF - 32763;     //Duty = T*(n+1) = us  @ PCLK1 = 16MHz
  UserBT_PWMInitReg.Clock = BT_CLK_FALL_EDGE;//外部时钟下降沿计数
  UserBT_PWMInitReg.Restart = BT_RESTART_ENABLE;
  UserBT_PWMInitReg.OutputMask = BT_MASK_DISABLE;
  UserBT_PWMInitReg.InputEdge = BT_TRG_DISABLE;
  UserBT_PWMInitReg.Polarity = BT_POLARITY_INVERTED;//计数期间输出H,匹配产生下降沿
  UserBT_PWMInitReg.Mode = BT_MODE_CONTINUOUS;//持续模式
  
  /* IO模式选择 */
  BT_SetIOMode(BT_CH_0,BT_IO_MODE_7);//IO模式7 BT0内部触发BT1
  
  /* PWM register initialization */
  BT_PWMInit(BT_CH_0,&UserBT_PWMInitReg); 
  
  /* Enable count operatoin */
  BT_PWMEnableCount(BT_CH_0); 
  
  BT_PWMStartSoftTrig(BT_CH_0);//软件启动
}

/*******************************************************************************
** 函数名  : UserBt1_Init
** 功能    : BT1初始化
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/10/28
-------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void UserBt1_Init(void)
{
  BT_PWMInitRegT UserBT_PWMInitReg;
  //IO配置 使能TIOAn+1输出波形
  IO_EnableFunc(IO_PORT4, IO_PINx1);
  IO_ConfigFuncBTTIOAxOuputPin(BT_CH_1, IO_BT_TIOAx_OUTPUT_TIOAx_0);
  
  /* Set initialization data */
  UserBT_PWMInitReg.Cycle = 50000;   //Cycle = T*(m+1) =3125us @ PCLK1 = 16MHz
  UserBT_PWMInitReg.Duty = 50000-100;     //Duty = T*(n+1) = us  @ PCLK1 = 16MHz
  UserBT_PWMInitReg.Clock = BT_CLK_DIV_1;//16M PCLK1时钟
  UserBT_PWMInitReg.Restart = BT_RESTART_DISABLE;
  UserBT_PWMInitReg.OutputMask = BT_MASK_DISABLE;
  UserBT_PWMInitReg.InputEdge = BT_TRG_EDGE_FALL;//外部下降沿触发
  UserBT_PWMInitReg.Polarity = BT_POLARITY_INVERTED;//反向
  UserBT_PWMInitReg.Mode = BT_MODE_ONESHOT;//单次模式
  
  /* PWM register initialization */
  BT_PWMInit(BT_CH_1,&UserBT_PWMInitReg); 
  
  //使能匹配中断
  BT_PWMEnableDutyMatchInt(BT_CH_1,UserBT1_MatchCallBack);
	//使能下溢出中断
	BT_PWMEnableUnderflowInt(BT_CH_1,UserBT1_UnderflowCallBack);
  NVIC_EnableIRQ(BTIM0_7_IRQn);//NVIC使能BT0~7中断
  /* Enable count operatoin */
  BT_PWMEnableCount(BT_CH_1);  
}

/*******************************************************************************
** 函数名  : UserBT1_MatchCallBack
** 功能    : BT1 PWM匹配回调函数
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/10/28
-------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void UserBT1_MatchCallBack(void)
{
  uint16_t tmr;
  NVIC_DisableIRQ(BTIM0_7_IRQn);
	/*********寄存器操作**********/
  bFM3_BT0_PWM_TMCR_STRG  = 1;                    //触发BT0
	//--------------------------------------------------------------------------
	while(FM3_BT0_PWM->TMR> 0xFFFE)      //对齐边沿等待
	{;}
	tmr = FM3_BT1_PWM->TMR;                 //获取当前计数值
	RTC_Calib.Old_Coff = FM3_BT1_PWM->PDUT - tmr;   //提取当前OFFSET值
	FM3_BT1_PWM->PCSR = 50000;
	if(RTC_Calib.Old_Coff >= RTC_Calib.Coff)
	{
			RTC_Calib.Old_Coff = RTC_Calib.Coff-1;
	}
	FM3_BT1_PWM->PDUT = 50000+RTC_Calib.Old_Coff-RTC_Calib.Coff;
	bFM3_BT1_PWM_TMCR_CKS0 = 1;
	bFM3_BT1_PWM_TMCR_CKS1 = 1;
  NVIC_EnableIRQ(BTIM0_7_IRQn);//NVIC使能BT0~7中断
}
/*******************************************************************************
** 函数名  : UserBT1_UnderflowCallBack
** 功能    : BT1 下溢出中断回调函数,恢复BT1输入时钟为PCLK1
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/10/30
-------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void UserBT1_UnderflowCallBack(void)
{
    NVIC_DisableIRQ(BTIM0_7_IRQn);
    bFM3_BT1_PWM_TMCR_CKS0 = 0;
    bFM3_BT1_PWM_TMCR_CKS1 = 0;
    NVIC_EnableIRQ(BTIM0_7_IRQn);//NVIC使能BT0~7中断
}

/*******************************************************************************
** 函数名  : InitSubout_32768
** 功能    : 初始化Subout输出,用于提供BT0计数时钟
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/10/28
-------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void InitSubout_32768(void)
{
  RTC_DisableSuboutDiv();
  RTC_SetSuboutDivVal(RTC_DIV_RATIO_SETTING_1);//32768Hz/1
  RTC_EnableSuboutDiv();
}
/*******************************************************************************
** 函数名  : UserBt7_Init
** 功能    : BT1初始化RT模式,BT7用来对HCR计数,以校准HCR
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/10/29
-------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void UserBt7_Init(void)
{
  BT_RTInitRegT UserBT_RTInitRegT;
  UserBT_RTInitRegT.Clock=BT_CLK_DIV_1024;//HCR 1024分频
  UserBT_RTInitRegT.TimerMode=BT_16BIT_TIMER;//16 bit模式
  UserBT_RTInitRegT.Cycle=0xFFFF;
  UserBT_RTInitRegT.InputEdge=BT_TRG_DISABLE;//禁止外部触发
  UserBT_RTInitRegT.Mode=BT_MODE_CONTINUOUS;//连续模式
  UserBT_RTInitRegT.Polarity=BT_POLARITY_NORMAL;
  
  //设置IO模式0
  BT_SetIOMode(BT_CH_7,BT_IO_MODE_0);
  
  BT_RTInit(BT_CH_7,&UserBT_RTInitRegT);//初始化
  
  BT_RTEnableCount(BT_CH_7);
  
  BT_RTStartSoftTrig(BT_CH_7);//软件触发
}

/*******************************************************************************
** 函数名  : RTCSecIntCallback
** 功能    : RTC秒中断回调函数
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/10/29
-------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void RTC_HcrCalib(void)
{
  int16_t Cftmp = 0;
  int16_t temp;
#ifdef USE_OSC_ADJUST   
  
  NVIC_DisableIRQ(BTIM0_7_IRQn);
  temp=Hcr_Calib.HcfCnt;
  //Hcr_Calib.HcfCnt = 0;
  NVIC_EnableIRQ(BTIM0_7_IRQn);//NVIC使能BT0~7中断

  if((temp > 14843)&&(temp < 16406))  //在15625Hz
  {
    Hcr_Calib.HcfAdd = Hcr_Calib.HcfAdd + temp;
    if(++Hcr_Calib.Index>=30)                   //70-10 = 60次数据滤波
    {
      Hcr_Calib.Index = 10;
      Hcr_Calib.SysFreq  = (Hcr_Calib.HcfAdd*1024)/20;
      Hcr_Calib.FreqCoef = Hcr_Calib.SysFreq/1600;        //计算频率系数
      Hcr_Calib.HcfAdd = 0;
    }
    if(Hcr_Calib.Index < 10)                  //上电6秒钟内快速计算
    {
      Cftmp = Hcr_Calib.HcfCntOld - temp;
      if(Cftmp < 0)
      {
        Cftmp = -Cftmp;
      }
      if(Cftmp<15)
      {
        //Hcr_Calib.SysFreq1 = (Hcr_Calib.HcfCntOld+temp)<< 9;
        //Hcr_Calib.FreqCoef = Hcr_Calib.SysFreq1/1600;   //计算频率系数
        Hcr_Calib.SysFreq = (Hcr_Calib.HcfCntOld+temp)<< 9;
        Hcr_Calib.FreqCoef = Hcr_Calib.SysFreq/1600;   //计算频率系数
        Hcr_Calib.Index  = 10;
        Hcr_Calib.HcfAdd = 0;
        //                    RTCParameter.NtcFlag = 1;
      }
      Hcr_Calib.HcfCntOld = temp;
    }
  }
  Hcr_Calib.HcfCnt_EMU=Hcr_Calib.SysFreq>>10;

  //----------------------------------------------------------------------
  
#endif
  
}

/*******************************************************************************
** 函数名  : RtcWriteCoef
** 功能    : 根据ppm更新系数
** 输入参数: 最大范围+-200ppm; coeff:+-2000,单位0.1ppm
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/10/28
-------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void RtcWriteCoef(int64_t ppm10)
{  
  int32_t coff32;
  
//   if((ppm10 < 2000)&&(ppm10 > -2000))
//   {
    //换算0.125ppm
//    ppm10 = (ppm10*1000)/625;    //换算成.0625ppm个数
    ppm10 = (ppm10*Hcr_Calib.FreqCoef)/6250;    //换算成.0625ppm个数
    
    coff32 = (RTC_COEF_INIT*Hcr_Calib.FreqCoef)/10000;
    
    coff32 = coff32 + ppm10;
    
    RTC_Calib.Coff = (uint16_t)coff32;
//   }
}
