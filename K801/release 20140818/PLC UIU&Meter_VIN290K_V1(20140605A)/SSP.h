/*********************************************************************
 *
 *               SSP Access Routines Definitions
 *
 *********************************************************************
 * FileName:        SSP.h
 * Dependencies:   
 * Processor:       SH99F01
 * Complier:        Keil C51 v9.00
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
 * Ellie Fan            2010/3/23	   Original                                
 ********************************************************************/

#ifndef _SSP_H_
#define _SSP_H_

#define PAGE_SIZE	256    // size of a sector(256 Byte)

/****************************************************************************
 * Function:            uchar SSP_Read(uint adr)
 * Input Parameter:     adr      - physical address of flash memeory
 * OutPut Parameter:    None
 * Return Value:        1 byte data
 * Description:         This function read a byte of data from Flash(CODE block or EEPROM block)
 ***************************************************************************/
uchar SSP_Read(uint adr);


/****************************************************************************
 * Function:            void SSP_Write(uint adr, uchar dat)
 * Input Parameter:     adr      - physical address of flash memeory (EEPROM block)
 *                      dat      - data to be wirte
 * OutPut Parameter:    None
 * Return Value:        None
 * Description:         This function writes a byte of data into Flash
 ***************************************************************************/
void SSP_Write(uint adr, uchar dat);


/**************************************************************************
 * Function:          SSP_Erase(uchar page)
 * Input parameter:   page      - sector index of flash memory (EEPROM block)
 * Output parameter:  None
 * Return Value:      None
 * Description:       This funcition erases a sector in Flash
 **************************************************************************/
void SSP_Erase(uchar page);



/**************************************************************************
 * Function:          SSP_CopyPage(uchar from, uchar to, uchar start, uchar end)
 * Input parameter:   from       -  source sector index 	   (EEPROM block)
 					  to         -  destination sector index	(EEPROM block)
     				  start      -  the start offset address in a sector   
					  end        -  the end offset address in a sector
 * Output parameter:  None       
 * Return Value:      None       
 * Description:       This funcition copy the data(between start offset and end offset) from the souce sector to the destination section 
 **************************************************************************/
void SSP_CopyPage(uchar from, uchar to, uchar start, uchar end);




/**************************************************************************
 * Function:          void Write_FLASH(uint adr, uchar dat)
 * Input parameter:   addr		- data address (CODE block)
 					  dat		- data 
 * Output parameter:  None
 * Return Value:      None
 * Description:       This funcition is to write one byte data to Flash
 **************************************************************************/
void Write_FLASH(uint adr, uchar dat);


/**************************************************************************
 * Function:          void Erase_FLASH(uchar page)
 * Input parameter:   page      - sector index of flash (CODE block)
 * Output parameter:  None
 * Return Value:      None
 * Description:       This funcition erases a sector of Flash
 **************************************************************************/
void Erase_FLASH(uchar page);


#endif

