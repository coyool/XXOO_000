/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* �ļ����ƣ�ISR
*
* �ļ���ʶ��
* ժ    Ҫ��
*
* ��ǰ�汾��
* ��    �ߣ�F06553
* ������ڣ�2014-11-7
* ���뻷����D:\software\IAR_for_ARM\arm
* 
* Դ����˵����
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/




/*******************************************************************************
* Description : Basic Timer Interrupt Service Routine  fixed time = 15.625ms
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
#pragma vector=BASICTIMER_VECTOR 
__interrupt void BasicTimer_ISR(void)
{ 
//    systick_cnt++;
//    RF_timeOverCnt++;
//    if (50 == systick_cnt)
//    {
//        systick_cnt = 0u;   
//    }  
    
    if (2u == Serial.RxFlag)
    {
        Serial_timeoutCnt++;
    }
    else
    {
    }    
    
    if (Serial_timeoutCnt >= Serial_timeout_62ms)
    {
        Serial_timeoutCnt = 0u;
        Serial.RxCnt = 0u;
        Serial.RxFlag = 1u;
    }
    else
    {
    }    
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
#pragma vector=UART0TX_VECTOR
__interrupt void Serial_TX_ISR(void)
{	
	Serial_send();
	U0IFG &=~UTXIFG0;	
}

/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
#pragma vector=UART0RX_VECTOR
__interrupt void Serial_RX_ISR (void)
{
	Serial_Recv(U0RXBUF);	
	U0IFG &=~URXIFG0;	
}

/*******************************************************************************
* Description : rising edge(sync)  falling edge (RX_finish) 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
#pragma vector=PORT1_VECTOR
__interrupt void RF_IRQ(void)
{
	RF.RX_FLG = 1u; 
	GDO2_HARDWARE_FlAG_CLEAR;//���Ӳ����־
    
}   