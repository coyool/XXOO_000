/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：RF 
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-10-8
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/
u8 RF_RecvBuff[RF_buffSize] = {0};







/*******************************************************************************
* Description : RF_test
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_test(void)
{
    RF_Send();
    delay_s(12);
    RF_Recv();
}


/*******************************************************************************
* Description : RF_Send
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_Send(void)
{
#define  RF_module_A7139
#ifdef   RF_module_A7139
    LSD_RF_FreqSet(0);    /* set RF channel 0 */   
    LSD_RF_SendPacket(PN9_Tab, RF_buffSize);    /* send PN9 data */
#endif 
    
    
    
}


/*******************************************************************************
* Description : RF_Recv
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void RF_Recv(void)
{
#define  RF_module_A7139
#ifdef   RF_module_A7139
    if (TRUE == A7139_Recv_flag)
    {
        A7139_Recv_flag = 0u;
        memset(RF_RecvBuff, 0u, RF_buffSize);
        A7139_Recv(RF_RecvBuff, RF_buffSize);
    }
    else
    {
    }//end if 
#endif 
    
    
    
}


