//MCU: SH99F01
#include  <SH99F01.h>
#include  "MAIN.h"
#include  "APP.h"
#include  "UART.h"
#include  "PLT.h"
#include  "string.h"

bit	  plt_rx_start = 0;		// debug
bit	  plt_tx_ready = 0;

bit	  plt_tx_continue = 0;

bit   plt_rx_ok = 0;
bit   plt_tx_ok = 0;

void App_Task()
{
	uchar i;
	PLC_TX = (PLCON&0X01);		 // LED PLC_TX
	PLC_RX = RXFA|RXNFA;		 // LED PLC_RX

	if(rx_ok_flag)
	{
		rx_ok_flag = 0;	

		if ((plt_buf_trans[30]&0x80)==0x80)		//with continue
		{
			for(i=0;i<29;i++)
			{
			  	plt_trans.plt_buf[plt_trans.plt_ptr] = plt_buf_trans[i];			
				plt_trans.plt_ptr++;
			}
		}
		else		  		// 	without continue
		{
			for(i=0;i<29;i++)
			{
			  	plt_trans.plt_buf[plt_trans.plt_ptr] = plt_buf_trans[i];			
				plt_trans.plt_ptr++;				
			}	

			//plt_trans.plt_framelen = plt_trans.plt_ptr;			
			 //plt_trans.plt_framelen = plt_trans.plt_buf[29];
			 //uart_trans.uart_framelen = plt_trans.plt_buf[29];
			 uart_trans.uart_framelen = plt_buf_trans[29];	

			plt_trans.plt_ptr = 0;			
			plt_rx_ok = 1;		// user clear			

			uart_TI_ready = 1; 	// debug

		}

		//PLC_RX = !PLC_RX;	// debug
			
	}
	if(tx_ok_flag)
	{
		tx_ok_flag  = 0;

		if(plt_tx_continue)
		{
			if (uart_trans.uart_framelen<=58)
			{
				memset(plt_buf_trans,0,FRAME_LENGTH);
				for(i=0;i<29;i++)
				{
					plt_buf_trans[i] = uart_trans.uart_buf[uart_trans.uart_ptr];
					uart_trans.uart_ptr++;
				}

			 	//plt_trans.plt_buf[29] = plt_trans.plt_framelen;
				plt_buf_trans[29] = uart_trans.uart_framelen;				  

				plt_buf_trans[30] &= 0x7F;		// without continue
			
			//	while(!(Transmit()));				
			Transmit();	   	
				plt_tx_continue = 0;	
			}

			else if (uart_trans.uart_framelen<=87)
			{
				if (uart_trans.uart_ptr<58)			 // care!  can't  <=58    (uart_trans.uart_ptr decision first, increment second)
				{
					memset(plt_buf_trans,0,FRAME_LENGTH);
					for(i=0;i<29;i++)
					{
						plt_buf_trans[i] = uart_trans.uart_buf[uart_trans.uart_ptr];
						uart_trans.uart_ptr++;
					}

			 	//plt_trans.plt_buf[29] = plt_trans.plt_framelen;
				plt_buf_trans[29] = uart_trans.uart_framelen;
					plt_buf_trans[30] |= 0x80;		// with continue
				
					//while(!(Transmit()));
					Transmit();	   	
					plt_tx_continue = 1;	
				}
				else
				{
					memset(plt_buf_trans,0,FRAME_LENGTH);
					for(i=0;i<29;i++)
					{
						plt_buf_trans[i] = uart_trans.uart_buf[uart_trans.uart_ptr];
						uart_trans.uart_ptr++;
					}

			 	//plt_trans.plt_buf[29] = plt_trans.plt_framelen;
				plt_buf_trans[29] = uart_trans.uart_framelen;
					plt_buf_trans[30] &= 0x7F;		// without continue
				
				//	while(!(Transmit()));					
				Transmit();	   	
					plt_tx_continue = 0;	
				}
	
			}	
			else		  // >87 bytes
			{
				if (uart_trans.uart_ptr<58)	 	// care! can't <=58
				{
					memset(plt_buf_trans,0,FRAME_LENGTH);
					for(i=0;i<29;i++)
					{
						plt_buf_trans[i] = uart_trans.uart_buf[uart_trans.uart_ptr];
						uart_trans.uart_ptr++;
					}

			 	//plt_trans.plt_buf[29] = plt_trans.plt_framelen;
				plt_buf_trans[29] = uart_trans.uart_framelen;
					plt_buf_trans[30] |= 0x80;		// with continue				
				
			//		while(!(Transmit()));
			Transmit();	   	
					plt_tx_continue = 1;
				}
				else
				{
					memset(plt_buf_trans,0,FRAME_LENGTH);
					for(i=0;i<29;i++)
					{
						plt_buf_trans[i] = uart_trans.uart_buf[uart_trans.uart_ptr];
						uart_trans.uart_ptr++;
					}

			 	//plt_trans.plt_buf[29] = plt_trans.plt_framelen;
				plt_buf_trans[29] = uart_trans.uart_framelen;

					plt_buf_trans[30] &= 0x7F;		// without continue
					plt_buf_trans[30] |= 0x40;		// length error flag
				
		//			while(!(Transmit()));					
		Transmit();	   	
					plt_tx_continue = 0;
				}
			}
		}//end if(plt_tx_continue)

		else	   // tx complete
		{
			//memset(&uart_trans,0,sizeof(UART_TRANS));	// dummy	
			//memset(&plt_trans,0,sizeof(PLT_TRANS));	// dummy	
//			memset(plt_buf_trans,0,FRAME_LENGTH+2);		// dummy
//			uart_trans.uart_timeout = 0xFFFF;	
//			plt_trans.plt_timeout = 0xFFFF;	
  
			plt_tx_ok=1;		 // user clear
		}							
	}
	if(uart_TI_ok)
	{
		uart_TI_ok = 0;

		memset(&plt_trans,0,sizeof(PLT_TRANS));		// dummy
				
	}


	if(uart_RI_ok)
	{
		
//		uart_RI_ok = 0;
//			for(i=0;i<uart_trans.uart_framelen;i++)
//			  plt_trans.plt_buf[i] = uart_trans.uart_buf[i];
//		uart_TI_ready = 1;


		// 2014/6/5
	 	uart_rx_idle_timeout = 0;		

	   // 2014/3/21, UIU
//		for(i=0;i<(uart_trans.uart_framelen+4);i++)
//		{
//			if (uart_trans.uart_buf[i]==0x68)
//			  break;
//		}
//		if ((memcmp(&uart_trans.uart_buf[i+1],meter_local_addr,6)!=0)&&(uart_trans.uart_framelen>10))		// address mismatch
//		{
//			memcpy(meter_local_addr,&uart_trans.uart_buf[i+1],6);
//			memcpy(meter_local_addr_bak,meter_local_addr,6);			
//		}

		// 2014/5/27, enable again
		for(i=0;i<(uart_trans.uart_framelen+4);i++)
		{
			if (uart_trans.uart_buf[i]==0x68)
			  break;
		}
		if ((memcmp(&uart_trans.uart_buf[i+1],meter_local_addr,6)!=0)&&(uart_trans.uart_framelen>10))		// address mismatch
		{
			memcpy(meter_local_addr,&uart_trans.uart_buf[i+1],6);
			memcpy(meter_local_addr_bak,meter_local_addr,6);			
		}


	    if (uart_trans.uart_framelen<=29)
		{
			memset(plt_buf_trans,0,FRAME_LENGTH);
			for(i=0;i<uart_trans.uart_framelen;i++)
			  plt_buf_trans[i] = uart_trans.uart_buf[i];


			 //	plt_trans.plt_buf[29] = plt_trans.plt_framelen;
			 plt_buf_trans[29] = uart_trans.uart_framelen;	 // bug
			plt_buf_trans[30] &= 0x7F;	

			//		while(!(Transmit()));
			Transmit();	   	
			uart_RI_ok = 0;		
			plt_tx_continue = 0;	// dummy	 			 			
		}
		else
		{
			memset(plt_buf_trans,0,FRAME_LENGTH);
			for(i=0;i<29;i++)
			{
				plt_buf_trans[i] = uart_trans.uart_buf[i];
				uart_trans.uart_ptr++;
			}

			// 	plt_trans.plt_buf[29] = plt_trans.plt_framelen;
			plt_buf_trans[29] = uart_trans.uart_framelen;
			plt_buf_trans[30] |= 0x80;		// with continue
		
	//		while(!(Transmit()));
	        Transmit();	   	
			uart_RI_ok = 0;	
			plt_tx_continue = 1;			
		}
	}
}