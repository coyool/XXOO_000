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
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
u8 hanMing(Uchar8 RX_sequence, Uchar8 likely);


/*** static variable declarations ***/
u8  fecEncodeTable[] = 
{ 
    0, 3, 1, 2, 
    3, 0, 2, 1,
    3, 0, 2, 1,
    0, 3, 1, 2 
}; 


/*** extern variable declarations ***/
Uchar8 input[10]={3,1,2,3}; 
Uchar8 i;
Uchar8 j;
Uint16 val;
Uint16 fecReg;
Uint16 fecOutput; 
Uint32 intOutput; 
Uchar8 fec[20]; 
Uchar8 interleaved[20]; 
Uint16 inputNum = 0, fecNum;  // 16 bit 防止不够 
Uint16 checksum; 
Uchar8 length = 3;


u8 length = 





/*******************************************************************************
* Description : payload data pass (3,1,4)convolution encode and Interleave encode 
*               transmitted over the air 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void FEC_enCode(u8 *input, u8 length)
{
    inputNum = length + 1;  
     
    printf("Input: [%5d bytes]\n", inputNum); 
    for (i=0; i<inputNum; i++) 
    {
        printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");
    }     
    printf("\r\n");
     
    // Generate CRC 
    checksum = Calc_CRCSUM(input, 4);      
    input[inputNum++] = (checksum >> 8);    
    input[inputNum++] = checksum & 0x00FF; 
    
    printf("Appended CRC: [%5d bytes]\n", inputNum); 
    for (i = 0; i < inputNum; i++) 
    {
        printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " "); 
    }        
    printf("\n\n"); 
    
    // Append Trellis Terminator 
    input[inputNum] = 0x0B; 
    input[inputNum + 1] = 0x0B; 
    fecNum = 2*((inputNum / 2) + 1); // inputNum为奇数  fecNum = inputNum + 1；偶数 + 2   不在循环不用优化 
    
    printf("Appended Trellis terminator: [%5d bytes]\n", fecNum); 
    for (i = 0; i < fecNum; i++) 
    {
        printf("%02X%s", input[i], (i % 8 == 7) ? "\n" : (i % 2 == 1) ? " " : " ");  
    }        
    printf("\n\n"); 
  
    // FEC encode 
    fecReg = 0;  
    for (i = 0; i < fecNum; i++) 
    { 
         fecReg = (fecReg & 0x700) | (input[i] & 0xFF); // S2 S1 S0  data(8bit)   
         fecOutput = 0; 
         for (j = 0; j < 8; j++) 
         { 
             fecOutput = (fecOutput << 2) | fecEncodeTable[fecReg >> 7]; //查表状态机 
             fecReg = (fecReg << 1) & 0x7FF; 
         } 
         fec[i * 2] = fecOutput >> 8;  //fec_H
         fec[i * 2 + 1] = fecOutput & 0xFF; //fec_L 
    }
    //printf("%02X%s", fecReg & 0x700,"\n");
     
    printf("FEC encoder output: [%5d bytes]\n", fecNum * 2); 
    for (i = 0; i < fecNum * 2; i++) 
    { 
         printf("%02X%s", fec[i], (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
    }      
    printf("\n\n"); 
    
    // TX Perform interleaving 
    for (i = 0; i < fecNum * 2; i += 4) 
    { 
        intOutput = 0; 
        for (j = 0; j < 4*4; j++)
        { 
           intOutput = (intOutput << 2) | ((fec[i +(~j & 0x03)] >> (2 *  ((j & 0x0C) >> 2) )) & 0x03);
           // (~j) & 0x03 == (~j) % 4;  ((j & 0x0C) >> 2) == j/4;   
           //printf("%0d%s", i +(~j & 0x03), (i % 16 == 15) ? "\n" : " "); //
           //printf("%02X%s",~j);  
        } 
        //printf("\n\n"); //
        
        interleaved[i] = (intOutput >> 24) & 0xFF; 
        interleaved[i + 1] = (intOutput >> 16) & 0xFF; 
        interleaved[i + 2] = (intOutput >> 8) & 0xFF; 
        interleaved[i + 3] = (intOutput >> 0) & 0xFF; 
    } 
    
    printf("TX Interleaver output: [%5d bytes]\n", fecNum * 2); 
    for (i = 0; i < fecNum * 2; i++) 
    { 
         printf("%02X%s", interleaved[i], (i % 16 == 15) ? "\n" : (i % 4 == 3) ? " " : " "); 
    } 
    printf("\n\n"); 
    
    //printf("%0d%s",sizeof( unsigned short int));
}


/*******************************************************************************
* Description : Recv data pass Interleave decode and Viterbi decode restore 
*               payload data
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void FEC_deCode(void)
{
    
}

