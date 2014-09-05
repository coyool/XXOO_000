
/*********************************************************************
 *			Uart Task  Module 
 *********************************************************************
 * FileName:        UATR.c
 * Dependencies:    SH99F01.h
 *					MAIN.h
 *                  UART.h
 *                  PLT.h
 * Company:         Sinowealth Micro Electronics ltd
 *
 * All rights reserved.
 *
 * Author           Date         Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * robbie.zhang    2010.4.13       Version 1.0
 ********************************************************************/
#include  <SH99F01.h>
#include  "MAIN.h"
#include  "UART.h"
#include  "PLT.h"
#include "string.h"


//UART Parameter

bit	  uart_TI_ready=0;
bit	  uart_RI_start=0;
bit	  uart_TI_ok=0;	   	// complete
bit	  uart_RI_ok=0;

UART_TRANS	xdata uart_trans;

void UartReturn(uchar count);


/****************************************************************************
 * Function:          void InitUART()
 * Input Parameter:   None
 * OutPut Parameter:  None
 * Return Value:      None
 * Description:       Uart Parameter Setting
 *                                              
 ***************************************************************************/
void InitUART()
{
    RCLK = 1;   	    // set timer2 as baud rate clock
	TCLK = 1;

	PCON &= 0x0F;	    //SMOD = 0 SSTAT = 0;	

	if (uBaudOption==0)		//选定2.4K
	{
	 	TH2 = 0xFF;		// 16000/32/(0x10000-0xFF30) = 2.4k
	 	TL2 = 0x30;
	 	RCAP2H = 0xFF;	// reload value
	 	RCAP2L = 0x30;
	}
	else if (uBaudOption==1)		    
	{
	 	TH2 = 0xFF;		// 16000/32/(0x10000-0xFF98) = 4.8k
	 	TL2 = 0x98;
	 	RCAP2H = 0xFF;	// reload value
	 	RCAP2L = 0x98;
	}
	else if (uBaudOption==2)
	{
	 	TH2 = 0xFF;		// 16000/32/(0x10000-0xFFCC) = 9.6k
	 	TL2 = 0xCC;
	 	RCAP2H = 0xFF;	// reload value
	 	RCAP2L = 0xCC;
	}
	else	  // default
	{
	 	TH2 = 0xFF;		// 16000/32/(0x10000-0xFF30) = 2.4k
	 	TL2 = 0x30;
	 	RCAP2H = 0xFF;	// reload value
	 	RCAP2L = 0x30;
	}

  	TR2 = 1;		// start timer
	
	SADEN = 0;		//address don't care
	SADDR = 0;
	SM0_FE = 1;		// 11b~mode3 , 9 bit (8bit+1bit)
	SM1_RXOV = 1;

	SM2_TXCOL = 0;	// mode 1
	REN = 1;		// enable receive
	ES = 1;         // enable uart interrupt
  
    RI = 0;		// initial clear
    TI = 0;	
	memset(&uart_trans,0,sizeof(UART_TRANS));	
	uart_trans.uart_timeout = 0xFFFF;		  
	return;
}

/****************************************************************************
 * Function:          void Uart_Task()
 * Input Parameter:   uart_RI_flag, uart_TI_flag
 * OutPut Parameter:  transmit_enable
 * Return Value:      None
 * Description:       Receive data from Serial Port or transmit data to seiral port
 *                                              
 ***************************************************************************/
void Uart_Task()
{

	uchar i,length,temp1;
	bit	uart_check_fail_flag = 0;

	if(uart_RI_start)	        
	{											 
		while(uart_trans.uart_timeout)    //receive other data
		{
			if(RI)
			{				
				ACC = SBUF;
				if(P==RB8)	// even check
				{
				  uart_trans.uart_buf[uart_trans.uart_ptr] = SBUF;
				  uart_trans.uart_ptr++;
				  uart_trans.uart_timeout = UART_DELAY;			  				  				  
				}
				else
				{
				  //uart_trans.uart_timeout = 0;		// quit faster
				  uart_check_fail_flag = 1;				  
				}

				RI = 0;
			}			
		}
		
		if(uart_check_fail_flag)
		{
			uart_RI_ok = 0;		// receive fail
			memset(&uart_trans,0,sizeof(UART_TRANS));
		}
		else
		{
			uart_RI_ok = 1;		// receive ok
			uart_trans.uart_framelen = uart_trans.uart_ptr;		// dummy
			uart_trans.uart_ptr = 0;	

			// 2014/6/4, add CRC check////////////
			for(i=0;i<(uart_trans.uart_framelen+4);i++)
			{
				if (uart_trans.uart_buf[i]==0x68)
					  break;
			}
			length = uart_trans.uart_buf[i+9];
			if ((length+12)<=uart_trans.uart_framelen)
			{
				temp1 = UartCheckSum(&uart_trans.uart_buf[i],length+10);		// L + 10 byte	(include 0x68)
				//if (temp != uart_trans.uart_buf[i+length+11])			// CRC
				if (temp1 != uart_trans.uart_buf[i+length+10])			// bug, 2014/6/4	(start from index 0)
				{
					uart_RI_ok = 0;		// receive fail
					memset(&uart_trans,0,sizeof(UART_TRANS));
					//uart_trans.uart_timeout = 0xFFFF;		// modify,2014/6/4	
				}
			}
			else
			{
				uart_RI_ok = 0;		// receive fail
				memset(&uart_trans,0,sizeof(UART_TRANS));
				//uart_trans.uart_timeout = 0xFFFF;		// modify, 2014/6/4				
			}
			///////////////////////////////////////	

			
						
		}
		uart_check_fail_flag = 0;
		uart_RI_start=0;				
		uart_trans.uart_timeout = 0xFFFF;
		ES = 1;		// care, uart timeout, enable ES
		// not enable ES
	}

	else if(uart_TI_ready)				//return uart data to other device
	{
		ES=0;		// dummy
		
		TI = 0;		// dummy
		RI = 0;


//	   // 2014/3/21, UIU
////		for(i=0;i<(uart_trans.uart_framelen+4);i++)
////		{
////			if (plt_trans.plt_buf[i]==0x68)
////			  break;
////		}
////		if (memcmp(&plt_trans.plt_buf[i+1],meter_local_addr,6)==0)		// address match
////		{
//			for(i=0;i<uart_trans.uart_framelen;i++)		
//				UartReturn(plt_trans.plt_buf[i]);
//
//			uart_idle_timeout = 0;	   // 2014/3/21, must update after trans ok
////		}

		// 2014/5/27, enable again
		for(i=0;i<(uart_trans.uart_framelen+4);i++)
		{
			if (plt_trans.plt_buf[i]==0x68)
			  break;
		}
		if (memcmp(&plt_trans.plt_buf[i+1],meter_local_addr,6)==0)		// address match
		{
			for(i=0;i<uart_trans.uart_framelen;i++)		
				UartReturn(plt_trans.plt_buf[i]);

		// 2014/6/5
		     uart_tx_idle_timeout = 0;	   // 2014/3/21, must update after trans ok
		}

												
		uart_TI_ok = 1;
		uart_TI_ready = 0;				
				
		ES=1;
	}
	return;
}

/****************************************************************************
 * Function:          void UartReturn(uchar dat)
 * Input Parameter:   uart data
 * OutPut Parameter:  None
 * Return Value:      None
 * Description:       transmit data by serial port
 *                                              
 ***************************************************************************/
void UartReturn(uchar dat)		 
{		
		uint Uart_Delay = 0XFFFF;	

		ACC = dat;
		TB8 = P;	// even check	 第9数据位 写入奇偶校验位
		SBUF = dat;
		while(!TI)
		{
			Uart_Delay--;
			if(Uart_Delay==0)
			break;
		}
		TI=0;
		return;
}

uchar UartCheckSum(uchar *buf, uchar len)
{
	uchar k,sum=0;

	for (k=0;k<len;k++)
	   sum+=buf[k];
	return sum;
}

