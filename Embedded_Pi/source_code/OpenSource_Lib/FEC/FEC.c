/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：FEC
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-10-10
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：STM32 具备单周期 硬件乘除法
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
u8 hanMing(u8 RX_sequence, u8 likely);


/*** static variable declarations ***/
u8  fecEncodeTable[] = 
{ 
    0, 3, 1, 2, 
    3, 0, 2, 1,
    3, 0, 2, 1,
    0, 3, 1, 2 
}; 


/*** extern variable declarations ***/






/*******************************************************************************
* Description : payload data pass(3,1,4)convolution encode and Interleave encode 
*               transmitted over the air 
*               IntOutput -- Interleave Output
*               note: A7139 FIFO buffer size 64 byte.
* Syntax      : 
* Parameters I: input -- payload data  ( MAX 508 byte)
*               size -- payload length ( MAX 508 byte)
* Parameters O: output -- interleave data on the Air (MAX 1024)
* return      : Bytes on the Air
*******************************************************************************/
u32 FEC_enCode(u8 *output, u8 *input, const u16 size)
{ 
    u8 i;
    u8 j; 
    u16 CRC_checksum= 0u; 
    u16 fecReg = 0u;    /* S2 S1 S0 d7 ~ d0 */
    u16 fecNum = 0u;    /* before fec encode size */
    u32 inputNum = 0u;  /* Payload size = data + CRC + Terminator */
    u16 fecOutput = 0u; 
    u32 intOutput = 0u;
    u8 fec[FEC_LIMIT_LEN] = {0}; 
    u32 return_val = 0u;
    
    /* Check the parameters */
    ASSERT (input != NULL);
    ASSERT (output != NULL); 
    ASSERT (size < 508u);
    
    timer.systick_cnt = 0u;
    SysTick_ENABLLE(ENABLE);   
    
    inputNum = size;
#define FEC_Debug 
#ifdef FEC_Debug         
    printf("Input: [%d bytes] \r\n", inputNum); 
    for (i=0; i<inputNum; i++) 
    {
        printf("0x%02X%s", input[i], (i % 16 == 15) ? "\r\n" : (i % 2 == 1) ? " " : " ");
    }     
    printf("\r\n");
    printf("\r\n");
#endif 
    
    // Generate CRC 
    CRC_checksum = calc_CRC_CC1101(input, size);      
    input[inputNum] = CRC_checksum >> 8;    
    inputNum++;
    input[inputNum] = CRC_checksum & 0x00FF; 
    inputNum++;

#ifdef FEC_Debug      
    printf("Appended CRC: [%d bytes] \r\n", inputNum); 
    for (i=0; i<inputNum; i++) 
    {
        printf("0x%02X%s", input[i], (i % 16 == 15) ? "\r\n" : (i % 2 == 1) ? " " : " "); 
    }        
    printf("\r\n"); 
    printf("\r\n");
#endif 
    
    // Append Trellis Terminator 
    input[inputNum] = 0x0B;        //前000 用于结束符
    input[inputNum + 1] = 0x0B; 
    fecNum = 2*((inputNum / 2) + 1);  /* 奇数0x0B, 偶数0x0B 0x0B */

#ifdef FEC_Debug 
    printf("Appended Trellis terminator: [%d bytes] \r\n", fecNum); 
    for (i=0; i<fecNum; i++) 
    {
        printf("0x%02X%s", input[i], (i % 16 == 15) ? "\r\n" : (i % 2 == 1) ? " " : " ");  
    }        
    printf("\r\n"); 
    printf("\r\n");
#endif 
    
    // FEC encode 
    for (i=0; i<fecNum; i++) 
    { 
         fecReg = (fecReg & 0x700) | (input[i] & 0xFF); // S2 S1 S0  data(8bit)   
         fecOutput = 0; 
         for (j=0; j<8; j++) 
         { 
             fecOutput = (fecOutput << 2) | fecEncodeTable[fecReg >> 7]; //查表状态机 
             fecReg = (fecReg << 1) & 0x7FF; 
         } 
         fec[i*2] = fecOutput >> 8;  //fec_H
         fec[(i*2)+1] = fecOutput & 0xFF; //fec_L 
    }
    //printf("%X%s", fecReg & 0x700,"\n");
    
#ifdef FEC_Debug 
    printf("FEC encoder output: [%d bytes] \r\n", fecNum * 2); 
    for (i=0; i<(fecNum*2); i++) 
    { 
        printf("0x%02X%s", fec[i], (i % 16 == 15) ? "\r\n" : (i % 4 == 3) ? " " : " "); 
        //printf("%#02X%s", fec[i], (i % 16 == 15) ? "\r\n" : (i % 4 == 3) ? " " : " "); 
    }      
    printf("\r\n"); 
    printf("\r\n");
#endif 
    
    // TX Perform interleaving 
    for (i=0; i<(fecNum*2); i=i+4) 
    { 
        intOutput = 0; 
        for (j=0; j <(4*4); j++)
        { 
            // (~j) & 0x03 == (~j) % 4;  ((j & 0x0C) >> 2) == j/4;   
            //printf("%0d%s", i +(~j & 0x03), (i % 16 == 15) ? "\n" : " "); //
            //printf("%02X%s",~j);  
           intOutput = (intOutput << 2) 
                        | ((fec[i+(~j & 0x03)]>>(2*((j&0x0C)>>2)))&0x03);
        } 
       
        output[i] = (intOutput >> 24) & 0xFF; 
        output[i+1] = (intOutput >> 16) & 0xFF; 
        output[i+2] = (intOutput >> 8) & 0xFF; 
        output[i+3] = (intOutput >> 0) & 0xFF; 
    } 
#ifdef FEC_Debug 
    printf("TX Interleaver output: [%d bytes] \r\n", fecNum * 2); 
    for (i=0; i <(fecNum*2); i++) 
    { 
        printf("0x%02X%s", output[i], (i % 16 == 15) ? "\r\n" : (i % 4 == 3) ? " " : " "); 
        //printf("%#02X%s", output[i], (i % 16 == 15) ? "\r\n" : (i % 4 == 3) ? " " : " "); 
    } 
    printf("\r\n");
    printf("\r\n");
#endif     
    return_val = fecNum * 2;
        
    SysTick_ENABLLE(DISABLE);
    
    return return_val;
}


/*******************************************************************************
* Description : Recv data pass Interleave decode and Viterbi decode restore 
*               payload data
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void FEC_deCode(u8 *output, u8 *input, const u32 size)
{
    ASSERT (input != NULL);
    ASSERT (output != NULL); 
    ASSERT (size < 1024u);
    
    
    
}


//Hamming distance





