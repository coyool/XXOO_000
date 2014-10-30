/*******************************************************************************
** 硬件平台: 富士通MB9AF005学习板
** 固件库  ：V1.0
** 文件名  : main.c
** 实现功能:
** 作者    : 微控事业部
** 创建日期: 2013/8/11
** 版权    : 利尔达科技集团
** -----------------------------------------------------------------------------
** 修改者:
** 修改日期:
** 修改内容:
*******************************************************************************/
#include "pdl_header.h"
#include "UserLed.h"
#include "delay.h"
#include "flash.h"


/******************************************************************************/
/**                          以下是内部宏定义                              	 **/
/******************************************************************************/

/******************************************************************************/
/**                          以下是内部函数申明                            	 **/
/******************************************************************************/

/******************************************************************************/
/**                          以下是全局变量定义                              **/
/******************************************************************************/

/******************************************************************************/
/**                          以下是内部函数定义                              **/
/******************************************************************************/


#define NVIC_VectTab_RAM        ((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH      ((uint32_t)0x00000000)
#define boot_address      BOOT_ADDRESS             //0x00000000

typedef void (*pFunction)(void);///<定义函数指针
static pFunction Jump_To_Application;
static uint32_t  JumpAddress;///<跳转地址


/**
* @brief  Sets the vector table location and Offset.
* @param  NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
*   This parameter can be one of the following values:
*     @arg NVIC_VectTab_RAM
*     @arg NVIC_VectTab_FLASH
* @param  Offset: Vector Table base offset field. This value must be a multiple of 0x100.
* @retval None
*/
static void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{

  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

/**
 * @brief boot 跳转到app函数
 */
static void Jump_To_app(void)
{
	 __disable_irq();//关闭总中断
	 JumpAddress = *(volatile uint32_t*) (boot_address + 4);
	 Jump_To_Application = (pFunction) JumpAddress;
	 /* Initialize user application's Stack Pointer */
	 __set_MSP(*(__IO uint32_t*) boot_address);
	 NVIC_SetVectorTable(NVIC_VectTab_FLASH, boot_address);
	 Jump_To_Application();
}


/*******************************************************************************
** 函数名  : main
** 功能    : 主函数
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/11
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
int main(void)
{
    SwitchToHighCR(CLOCK_MAINPLL_OFF,CLOCK_MAIN_OSC_OFF,CLOCK_SUB_OSC_ON,BaseSysetmClockDIV1);
	__enable_irq();               //
	SysTick_Config(SystemCoreClock/20);//开启系统定时器,重装载值(时间)为0.1s，SystemCoreClock=20MHz	
	//UserLed_Init();//LED的IO口初始化
    
    bFM3_LCDC_LCDCC3_PICTL = 1;//P56 P1D 管脚为SEG管脚，当需要配置为GPIO输入时，使用该语句！
    //bFM3_GPIO_ADE_AN13 = 0;   // 0 GPIO  1特殊功能
    bFM3_GPIO_PFR5_P5 = 0;    // 0 GPIO  1外设功能
    bFM3_GPIO_DDR5_P5 = 1;    // 0 输入  1输出
    //bFM3_GPIO_PCR1_PD = 1;    // 上拉
    
    //DelayMs(10000);
    
	while(1)
    {
         DelayMs(10000);
         
         __disable_irq();  /* disable EA */
         //write appointed address value
         MFlash_Write((uint16_t*) 0x0001FF84,(uint32_t) 0x00000000); //必须强制转换 写 32bit
         
         Jump_To_app();       //此处也可以使用复位
         //__NOP();
         
         //bFM3_GPIO_PDOR1_PD ^= 1; 
    }
}

/*******************************************************************************
** 函数名  : SysTick_Handler
** 功能    : 系统定时中断处理函数
** 输入参数: 无
** 输出参数: 无
** 作者    : 微控事业部
** 创建日期: 2013/8/11
--------------------------------------------------------------------------------
** 修改者  :
** 修改日期:
** 修改内容:
*******************************************************************************/
void SysTick_Handler()
{
	//UserLed_Blink();//LED翻转闪烁
    bFM3_GPIO_PDOR5_P5 ^= 1;    //LED4亮起 报警灯
}
