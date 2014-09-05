/*********************************************************************
 *
 *         PLT Module Defs for Protocol
 *
 *********************************************************************
 * FileName:        PLT.h
 * Dependencies:    
 * Processor:       SH99F01
 * Complier:        Keil C51 v7.06
 * Company:         Sinowealth Electronic, Inc.
 *
 * Software License Agreement
 *
 * This software is owned by Sinowealth Electronic Inc. ("Sinowealth") 
 * and is supplied to you for use exclusively as described in the 
 * associated software agreement.  This software is protected by 
 * software and other intellectual property laws.  Any use in 
 * violation of the software license may subject the user to criminal 
 * sanctions as well as civil liability.  Copyright 2009 Sinowealth
 * Electronic Inc.  All rights reserved.
 *
 * This software is provided "AS IS."  SINOWEALTH DISCLAIMS ALL 
 * WARRANTIES, EXPRESS, IMPLIED, STATUTORY OR OTHERWISE, NOT LIMITED 
 * TO MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
 * INFRINGEMENT.  Sinowealth shall in no event be liable for special, 
 * incidental, or consequential damages.
 *
 *
 * Author               Date           Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * robbie.zhang       2010.4.13       Version 1.0                               
 ********************************************************************/


#ifndef _PLT_H_
#define _PLT_H_

#define UMR1			0x00
#define UMR2			0x01
#define UMR3			0x02
#define TXFC			0x03
#define RXFC			0x04
#define SSCI			0x05
#define SSCQ			0x06
#define TACQ			0x07
#define TFA				0x08
#define CRCH			0x09
#define CRCL			0x0A
#define SNR				0x0B
#define NACQDT			0x0C
#define NACQUT			0x0D
#define NACQCT			0x0E
#define NTRKST			0x0F
#define PLRSSIL			0x10
#define PLRSSIH			0x11
#define FRAM_OFFSET	 	0x12
#define UMR4	     	0x13
#define CFD_CNT	     	0x14
#define CFD_UP	     	0x15
#define CFD_DOWN	 	0x16

#define  FRAME_LENGTH   31
#define  SSLP_LENGTH	4

#define  FRAME_INTERVAL 10

#define  MS_01 			1600     //0.1ms
#define  MS_02			3200	 //0.2ms
#define  MS_03 			4800     //0.3ms
#define  MS_05 			8000     //0.5ms
#define  MS_06 			9600     //0.6ms
#define  MS_09 			14400    //0.9ms


//#define NB_CNT					9		//(((FRAME_LENGTH+2)/SSLP_LENGTH)+1), 2011-1
//#define ZERO_TOTAL_LEN  		36		//(NB_CNT*SSLP_LENGTH)
//#define TM_RXZEROCROSSTMOUT     125		//(NB_CNT*50)/(AC_FREQ/10)+35   

#define NB_CNT					8		//(((FRAME_LENGTH+2)/SSLP_LENGTH)+1), 2011-1
#define ZERO_TOTAL_LEN  		32		//(NB_CNT*SSLP_LENGTH)
#define TM_RXZEROCROSSTMOUT     115		//(NB_CNT*50)/(AC_FREQ/10)+35  

#define FRAME_DURATION_DMZC_TXTAIL	 80 //100	// 40	// 410+35	 , 2013-09-29
#define FRAME_DURATION_DMZC_TX	  450 //450	  // 425+25
#define FRAME_DURATION_DMZC_RX	  430 //425	  // 290+125=415ms act
#define FRAME_DURATION_NBSF_TX	  90	  // 80+10
#define FRAME_DURATION_NBSF_RX	  80	  // 76ms act

#define TRANS_MODE_NBSF             0	
#define TRANS_MODE_DMZC             1	

////Input Parameter
extern bit   transmit_enable;  
////Output Parameter



extern bit   rx_ok_flag;            // 物理層全部成功接收標志
extern bit   tx_ok_flag;            // 物理層全部成功發送標志
extern uchar rssi;         			// 物理層接收幀信號強度


extern bit   zc_flag;			 	//過零檢測標誌

extern bit   rx_flag;            	// 物理層成功接收一幀標志
extern bit   tx_flag;            	// 物理層成功發送一幀標志  
      
extern  uchar xdata plt_buf_trans[FRAME_LENGTH+2];

extern PLT_TRANS	xdata plt_trans;
extern uint xdata Timer_count;
extern bit   fa_flag;


extern bit transmit_continue;
extern uint xdata   plt_trans_timeout;	
extern uint xdata   plt_recev_timeout;
extern bit en_delay_half;
extern bit en_zc_delay;
extern bit en_delay_flag;
extern bit frame_transmit_ready;
extern uchar zc_delay_cnt;

/*********************************************************************************************
 * Function:           void InitPLT(void)
 * Input Parameter:    None
 * OutPut Parameter:   None
 * Return Value:       None
 * Description:        This function  Initializes Registers and variables of PLT Module 
**********************************************************************************************/
void   InitPLT();


/***********************************************************************************************
 * Function:          void PLTTask(void)
 * Input Parameter:   None
 * OutPut Parameter:  None
 * Return Value:      None
 * Description:       This function acts as a task of PLT Module. Main application must call
 *                    this function repeatdly to ensure the PLT transmit and receive data on time.                          
************************************************************************************************/
void   PLTTask();



/***********************************************************************************************
 * Function:          void PLT_TimerTask(void)
 * Input Parameter:   None
 * OutPut Parameter:  None
 * Return Value:      None
 * Description:       This function deal with the timer Task of PLT Module. it is called in Timer 
     				  interrupt routine to make sure the timer task of PLT is run on time.
************************************************************************************************/
void   PLT_TimerTask();


/********************************************************************************************************
 * Function:          bit Transmit(void)
 * Input parameter:   None
 * OutPut parameter:  None
 * Return Value:      char info whether transmission is started. Return 1 if started, else return 0.
 * Description:       This funcition should be called when user want Transmit data.
 *                    Caller must make sure that the PutFrame function is called 
 *                    to fill in the PLT tx_buf before call this function.
 **********************************************************************************************************/
bit  Transmit();



/****************************************************************************
 * Function:          void GetFrame(uchar *frame)  
 * Input parameter:   None
 * OutPut parameter:  frame    -point to the begining of received data packet.
 * Return Value:      None
 * Description:       This funcition get reveive data from PLT rx_buf
 ***************************************************************************/
void   GetFrame(uchar *frame);


/*******************************************************************************
 * Function:          void  PutFrame(uchar *frame)
 * Input parameter:   frame    -pointer to to the  begining of tramsmit data packet
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition fill PLT tx_buf with data to be transmitted.              
 *******************************************************************************/
void   PutFrame(uchar *frame);


/*******************************************************************************
 * Function:          bit ACFreqDetect()
 * Input parameter:   None
 * OutPut parameter:  None
 * Return Value:      None
 * Description:       This funcition is auto-detect AC frequency (50Hz or 60Hz)            
 *******************************************************************************/
bit ACFreqDetect();


/**********************************************************************************************
 * Function:          void  PLTSetting()
 * Input parameter:   None
 * OutPut parameter:  None
 * Return Value:      None 
 * Description:       This funcition configure the register of PLT Module accord to the transmode              
 ***********************************************************************************************/
void   PLTSetting();


#endif