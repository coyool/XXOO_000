/*******************************************************************************
* Copyright 2014    ����˼���Ǳ�  �̼���  
* All right reserved
*
* �ļ����ƣ�main.c
*
* �ļ���ʶ��
* ժ    Ҫ��HOST( ) build an application to be loaded into Flash memory (X328) using
*           In-Application Programming (IAP, through USART).
*
*
* ��ǰ�汾��
* ��    �ߣ�F06553  
* ������ڣ�2014-7-24
* ���뻷����IAR_for_ARM 6.50
* 
* Դ����˵����
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
* ��������: main
* �������: null
* �������: null
* --����ֵ: null
* ��������: HOST (UART) build an app
*******************************************************************************/
int main(void)
{
    //BSP();   
   // HOST();
    __NOP();
    return 0u;
}

