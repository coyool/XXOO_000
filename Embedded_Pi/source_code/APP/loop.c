/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：loop
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-9-19
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
u8 A7139_TxBuffer_onTheAir[A7139_onTheAir_len] = {0};
u32 temp = 0u;

u8 A7139_TxBuffer[200] = {03,01,02,03};
u8 A7139_RxBuffer[200] = {0};

/*** extern variable declarations ***/






/*******************************************************************************
* 函数名称: loop
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void loop(void)
{
    //u32 temp = 0u;
    u8 i;
    
    temp = FEC_enCode(A7139_TxBuffer_onTheAir, 
                      A7139_TxBuffer, 
                      4);
//    temp = FEC_enCode(A7139_TxBuffer_onTheAir, 
//                      Tab_64, 
//                      A7139_payload_len);
    printf("# bytes of on the Air : %d \r\n", temp);
    printf("cla time : %dus \r\n", timer.systick_cnt);
    
    
    FEC_Decode(A7139_TxBuffer_onTheAir, temp);
    printf("\r\n");
    printf("FEC dec \r\n");
    for (i=0; i<temp/2; i++) 
    {
        printf("0x%02X%s", A7139_TxBuffer_onTheAir[i], 
                       (i % 16 == 15) ? "\r\n" : (i % 2 == 1) ? " " : " ");
    }
    printf("cla time : %dus \r\n", timer.systick_cnt);
    
    
    for (;;)
    {
        LED_Blink(LED1, 10, 1000);
        
    }//end for(;;)    
}

