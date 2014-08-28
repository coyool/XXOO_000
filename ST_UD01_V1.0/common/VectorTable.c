/*��005��֧���ж�����ƫ�ƣ����������������У���app����ʹ�����жϣ���boot�����б�����Ӹ��ļ���
    ע�� ApplicationAddress ��Ҫ����Ϊapp������ת��ַ��
  ��boot���̲��Ƽ�ʹ���жϣ��߻���app�жϳ�ͻ����ɳ����ܷɣ�
    ��boot����ʹ���жϣ������޸ĸ��ļ������boot�жϺ�app�ж��жϣ�ʹapp�жϴ���ȷ���жϵ�ַ���룬�Ӷ�˳������*/

/***************************************************
** @brief �ж�������
** @auther Thp
** @Date 2013-07-17
** @Logs
***************************************************/
//#include "mcu.h"
//#include "pdl_header.h"

#include    "all_header_file.h"

//#define ApplicationAddress 0x00000000
//#define NVIC_VectTab_RAM   	((uint32_t)0x20000000)
//#define NVIC_VectTab_FLASH 	((uint32_t)0x00000000)

//typedef void (*pFunction)(void);///<���庯��ָ��
//pFunction Jump_To_Application;
//uint32_t  JumpAddress;///<��ת��ַ

/****************************************************
** @brief boot ��ת��app����
*****************************************************/
//void Jump_To_app(void)
//{
//  __disable_irq();///<�ر����ж�
//  JumpAddress = *(volatile uint32_t*) (ApplicationAddress + 4);
//  Jump_To_Application = (pFunction) JumpAddress;
//  /* Initialize user application's Stack Pointer */
//  __set_MSP(*(__IO uint32_t*) ApplicationAddress);
//  Jump_To_Application();
//}


//==========================================================================================================
  /**
  * @brief  Sets the vector table location and Offset.
  * @param  NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
  *   This parameter can be one of the following values:
  *     @arg NVIC_VectTab_RAM
  *     @arg NVIC_VectTab_FLASH
  * @param  Offset: Vector Table base offset field. This value must be a multiple of 0x100.
  * @retval None
  */
//static void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
//{
//
//  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
//}

/**
 * @brief boot ��ת��app����
 */
//void Jump_To_app(void)
//{
//	 __disable_irq();///<�ر����ж�
//	 JumpAddress = *(volatile uint32_t*) (ApplicationAddress + 4); 
//	 Jump_To_Application = (pFunction) JumpAddress;	
//	 /* Initialize user application's Stack Pointer */
//	 __set_MSP(*(__IO uint32_t*) ApplicationAddress);
//	 NVIC_SetVectorTable(NVIC_VectTab_FLASH, ApplicationAddress);
//}
//=======================================================================================================================


/*****************************************************
* @brief ����������ض���app
* @param app�жϵ�������ַ
**********************************************************/
void Jump_To_Handler(uint32_t * HandlerAddress)
{
  JumpAddress = *HandlerAddress;
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

/**************************************************
NMI                                             2
**************************************************/
void NMI_Handler(void)
{
  while(1);
}
/**************************************************
Hard Fault                                      3
**************************************************/
void HardFault_Handler(void)
{
  while(1);
}
/**************************************************
MPU Fault                                       4
**************************************************/
void MemManage_Handler(void)
{
  while(1);
}
/**************************************************
Bus Fault                                       5
**************************************************/
void BusFault_Handler(void)
{
  while(1);
}
/**************************************************
Usage Fault                                     6
**************************************************/
void UsageFault_Handler(void)
{
  while(1);
}
/**************************************************
SVCall                                          11
**************************************************/
void SVC_Handler(void)
{

}
/**************************************************
Debug Monitor                                   12
**************************************************/
void DebugMon_Handler(void)
{

}
/**************************************************
PendSV                                          14
**************************************************/
void PendSV_Handler(void)
{
	Jump_To_Handler((uint32_t *)(ApplicationAddress+(14*4)));///<ƫ����ת��app���жϽ��д���
}
/**************************************************
Systick                                         15
**************************************************/
void SysTick_Handler(void)
{
	Jump_To_Handler((uint32_t *)(ApplicationAddress+(15*4)));///<ƫ����ת��app���жϽ��д���
//  bFM3_GPIO_PDORE_P0 ^=1;
}
/**************************************************
Clock Super Visor                              16
**************************************************/
void CSV_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(16*4)));
}
/**************************************************
Software Watchdog Timer                         17
**************************************************/
void SWDT_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(17*4)));
}
/**************************************************
Low Voltage Detector                            18
**************************************************/
void LVD_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(18*4)));
}
/**************************************************
Wave Form Generator / DTIF                      19
**************************************************/
void MFT_WG_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(19*4)));
}
/**************************************************
External Interrupt Request ch.0 to ch.7         20
**************************************************/
void INT0_7_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(20*4)));
}
/**************************************************
External Interrupt Request ch.8 to ch.15        21
**************************************************/
void INT8_31_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(21*4)));
}
/**************************************************
MultiFunction Serial ch.0                       22
**************************************************/
void MFS0_8_RX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(22*4)));
}
/**************************************************
MultiFunction Serial ch.0                       23
**************************************************/
void MFS0_8_TX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(23*4)));
}
/**************************************************
MultiFunction Serial ch.1                       24
**************************************************/
void MFS1_9_RX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(24*4)));
}
/**************************************************
MultiFunction Serial ch.1                       25
**************************************************/
void MFS1_9_TX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(25*4)));
}
/**************************************************
MultiFunction Serial ch.2                       26
**************************************************/
void MFS2_10_RX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(26*4)));
}
/**************************************************
MultiFunction Serial ch.2                       27
**************************************************/
void MFS2_10_TX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(27*4)));
}
/**************************************************
MultiFunction Serial ch.3                       28
**************************************************/
void MFS3_11_RX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(28*4)));
}
/**************************************************
MultiFunction Serial ch.3                       29
**************************************************/
void MFS3_11_TX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(29*4)));
}
/**************************************************
MultiFunction Serial ch.4                       30
**************************************************/
void MFS4_12_RX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(30*4)));
}
/**************************************************
MultiFunction Serial ch.4                       31
**************************************************/
void MFS4_12_TX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(31*4)));
}
/**************************************************
MultiFunction Serial ch.5                       32
**************************************************/
void MFS5_13_RX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(32*4)));
}
/**************************************************
MultiFunction Serial ch.5                       33
**************************************************/
void MFS5_13_TX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(33*4)));
}
/**************************************************
MultiFunction Serial ch.6                       34
**************************************************/
void MFS6_14_RX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(34*4)));
}
/**************************************************
MultiFunction Serial ch.6                       35
**************************************************/
void MFS6_14_TX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(35*4)));
}
/**************************************************
MultiFunction Serial ch.7                       36
**************************************************/
void MFS7_15_RX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(36*4)));
}
/**************************************************
MultiFunction Serial ch.7                       37
**************************************************/
void MFS7_15_TX_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(37*4)));
}
/**************************************************
PPG                                             38
**************************************************/
void PPG_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(38*4)));
}
/**************************************************
OSC / PLL / Realtime Clock                      39
**************************************************/
void TIM_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(39*4)));
}
/**************************************************
ADC0                                            40
**************************************************/
void ADC0_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(40*4)));
}
/**************************************************
Free-run Timer                                  41
**************************************************/
void MFT_FRT_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(41*4)));
}
/**************************************************
Input Capture                                   42
**************************************************/
void MFT_IPC_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(42*4)));
}
/**************************************************
Output Compare                                  43
**************************************************/
void MFT_OPC_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(43*4)));
}
/**************************************************
Base Timer ch.0 to ch.7                         44
**************************************************/
void BT0_7_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(44*4)));
}
/**************************************************
LCDC                                            45
**************************************************/
void ADC2_LCD_IRQHandler(void)
{
  Jump_To_Handler((uint32_t *)(ApplicationAddress+(45*4)));
}
