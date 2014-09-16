/*******************************************************************************
* Copyright 2014    深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：main.c
*
* 文件标识：
* 摘    要：HOST( ) build an application to be loaded into Flash memory (X328) using
*           In-Application Programming (IAP, through USART).
*
*
* 当前版本：
* 作    者：F06553  
* 完成日期：2014-7-24
* 编译环境：IAR_for_ARM 6.50
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
void BSP(void);


/*** static variable declarations ***/



/*** extern variable declarations ***/





#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/*******************************************************************************
* 函数名称: main
* 输入参数: null
* 输出参数: null
* --返回值: null
* 函数功能: HOST (UART) build an app
*******************************************************************************/
int main(void)
{
    //BSP();   
   // HOST();
    __NOP();
    return 0u;
}

