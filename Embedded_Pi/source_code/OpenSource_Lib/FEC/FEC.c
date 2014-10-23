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
*             lut -- look up table
*             destn -- destination
*             进一步优化的方式，可以通过Trellis输入输出关系
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
static const u8  fecEncodeTable[] = 
{ 
    0, 3, 1, 2, 
    3, 0, 2, 1,
    3, 0, 2, 1,
    0, 3, 1, 2 
}; 

/*
* Look-up source state index when:
* Each of two possible source states to Destination state
*/
static const u8 TrellisSrcState_Lut[8][2] = 
{ 
  {0, 4},     // State {0,4} -> State 0
  {0, 4},     // State {0,4} -> State 1
  {1, 5},     // State {1,5} -> State 2
  {1, 5},     // State {1,5} -> State 3
  {2, 6},     // State {2,6} -> State 4
  {2, 6},     // State {2,6} -> State 5
  {3, 7},     // State {3,7} -> State 6
  {3, 7},     // State {3,7} -> State 7 
}; 

/*
* Look-up expected output when:
* Each of two possible source states to Destination state
*/
static const u8 Trellis_g1g0[8][2] = 
{ 
  {0, 3},     // State {0,4} -> State 0 produces {"00", "11"}
  {3, 0},     // State {0,4} -> State 1 produces {"11", "00"}
  {1, 2},     // State {1,5} -> State 2 produces {"01", "10"}
  {2, 1},     // State {1,5} -> State 3 produces {"10", "01"}
  {3, 0},     // State {2,6} -> State 4 produces {"11", "00"}
  {0, 3},     // State {2,6} -> State 5 produces {"00", "11"}
  {2, 1},     // State {3,7} -> State 6 produces {"10", "01"}
  {1, 2},     // State {3,7} -> State 7 produces {"01", "10"}
};

/*
Look-up input bit at encoder when Destination state
*/
static const u8 Trellis_i[8] =   //input data i 
{ 
  0,
  1,
  0,
  1,
  0,
  1,
  0,
  1,
};

/* Two sets of buffers (last, current) for each destination state for holding */ 
static u8  nCost[2][8];    // Accumulated path cost 
static u32 Path[2][8];     // Encoder input data (32b window)

/* Indices of (last, current) buffer for each iteration */
static u8 iLastBuf; 
static u8 iCurrBuf;

/* Number of bits in each path buffer */
static u8 nPathBits;

/*** extern variable declarations ***/
u8 A7139_TxBuffer_onTheAir[A7139_onTheAir_len] = {0};
u8 A7139_TxBuffer[200] = {03,01,02,03};
u8 A7139_RxBuffer[200] = {0};





/*******************************************************************************
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void FEC_test(void)
{
    u32 tmp = 0u; 
    u8 i;
    
    tmp = FEC_enCode(A7139_TxBuffer_onTheAir, 
                  A7139_TxBuffer, 
                  4);
//    tmp = FEC_enCode(A7139_TxBuffer_onTheAir, 
//                      Tab_64, 
//                      A7139_payload_len);
    printf("# bytes of on the Air : %d \r\n", tmp);
    printf("cla time : %dus \r\n", timer.systick_cnt);
    
    //FEC_Decode(A7139_TxBuffer_onTheAir, tmp);
    FEC_deCode(A7139_RxBuffer, A7139_TxBuffer_onTheAir, tmp);
    printf("\r\n");
    printf("FEC dec \r\n");
    for (i=0; i<tmp/2; i++) 
    {
//        printf("0x%02X%s", A7139_TxBuffer_onTheAir[i], 
//                       (i % 16 == 15) ? "\r\n" : (i % 2 == 1) ? " " : " ");
        printf("0x%02X%s", A7139_RxBuffer[i], 
                       (i % 16 == 15) ? "\r\n" : (i % 2 == 1) ? " " : " ");
    }
    printf("cla time : %dus \r\n", timer.systick_cnt);
}


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
    
    /* calc program execute time */
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
* Description : 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
static u32 Viterbi_deCode(u8 *pDecData, const u8 *pInData, u32 nRemBytes)
{
    u8 i;
    s8 j;
    s8 iBit = 6;  /* 8 - 2 */
    u32 nOutputBytes = 0u;
    u8 nMinCost = 0xFF;
    u8 nCost0 = 0u; 
    u8 nCost1 = 0u;

    u8 iSrcState0 = 0u;
    u8 iSrcState1 = 0u;
    u8 nInputBit = 0u; 
      
    u8 DeintData[4] = {0};
    u8 *pDeintData = NULL;
    
    /*
    * De-interleave received data (and change pInData to point to de-interleaved
    * data) 
    */
    for (i=0; i<4; i++) 
    {
        u8 dat = 0;
        for (j=3; j>=0; j--) 
        {
            dat = (dat << 2) | ((pInData[j] >> (2 * i)) & 0x03);
        }    
        
        DeintData[i] = dat;
    }
    pDeintData = DeintData;
    /*
    * Process up to 4 bytes of de-interleaved input data, processing one encoder
    * symbol (2b) at a time 
    */ 
    for (i=0; i<16; i++) 
    {
        u8 iDestState; 
        u8 symbol = ((*pDeintData) >> iBit) & 0x03;
  
        /* 
        * Find minimum cost so that we can normalize costs 
        * (only last iteration used)
        */
        nMinCost = 0xFF;
    
    /* 
    * Get 2b input symbol (MSB first) and do one iteration of Viterbi decoding    
    */
        if ((iBit -= 2) < 0) 
        {
            iBit = 6; 
            pDeintData++; /* Update pointer to the next byte of received data */
        }
    
        /*
        * For each destination state in the trellis, calculate hamming costs for
        * both possible paths into state and select the one with lowest cost. 
        */
        for (iDestState=0; iDestState<8; iDestState++) 
        { 
            nInputBit = Trellis_i[iDestState];  //查看八态图
            /* 
            * Calculate cost of transition from each of the two source states 
            * (cost is Hamming difference between received 2b symbol
            * and expected symbol for transition)
            */
            iSrcState0 = TrellisSrcState_Lut[iDestState][0];
            nCost0  = nCost[iLastBuf][iSrcState0];
            nCost0 += calcHammingWeight(symbol ^ Trellis_g1g0[iDestState][0]);

            iSrcState1 = TrellisSrcState_Lut[iDestState][1];
            nCost1  = nCost[iLastBuf][iSrcState1];
            nCost1 += calcHammingWeight(symbol ^ Trellis_g1g0[iDestState][1]);
            /*          
            * Select transition that gives lowest cost in destination state, 
            * copy that source state's path and add new decoded bit.
            */
            if (nCost0 <= nCost1) 
            {
                nCost[iCurrBuf][iDestState] = nCost0; 
                nMinCost = min(nMinCost, nCost0);
                Path[iCurrBuf][iDestState] = (Path[iLastBuf][iSrcState0] << 1) 
                                              | nInputBit; 
            } 
            else 
            {
                nCost[iCurrBuf][iDestState] = nCost1; 
                nMinCost = min(nMinCost, nCost1);
                Path[iCurrBuf][iDestState] = (Path[iLastBuf][iSrcState1] << 1)
                                              | nInputBit;
            }
        }//end for (iDestState = 0; iDestState < 8; iDestState++)  
        
        nPathBits++;
        
    /* If trellis history is sufficiently long, output a byte of decoded data */
        if (32u == nPathBits) 
        {
            *pDecData++ = (Path[iCurrBuf][0] >> 24) & 0xFF; //??
            nOutputBytes++;
            nPathBits = nPathBits - 8;
            nRemBytes--;
        }
        
        /* After having processed 3-symbol trellis terminator,flush out remaining 
           data*/ 
        if ((nRemBytes <= 3) && (nPathBits == ((8 * nRemBytes) + 3))) 
        {
            while (nPathBits >= 8) 
            { 
                *pDecData++ = (Path[iCurrBuf][0] >> (nPathBits - 8)) & 0xFF;
                nOutputBytes++;
                nPathBits = nPathBits - 8;
            } 
            return nOutputBytes;
        } 

        /* Swap current and last buffers for next iteration */
        iLastBuf = (iLastBuf + 1) % 2;
        iCurrBuf = (iCurrBuf + 1) % 2;    //??
    }//end for(i=0; i<16; i++) 
  
    /* Normalize costs so that minimum cost becomes 0 */
    u8 iState; 
    for (iState = 0; iState < 8; iState++)
    {
        nCost[iLastBuf][iState] = nCost[iLastBuf][iState] - nMinCost;
    }
     
    return nOutputBytes;
}


/*******************************************************************************
* Description : Recv data pass Interleave decode(3,1,4) and Viterbi decode  
*               restore payload data
* Syntax      : 
* Parameters I: input  
*               size
* Parameters O: output 
* return      : The decoded data length
*******************************************************************************/
u32 FEC_deCode(u8 *output, const u8 *input, u32 size)
{
    u32 nBytes = 0u;
    u32 DecLen = 0u;
    u32 nBytesOut = 0u;
    u8 i;
    
    ASSERT (input != NULL);
    ASSERT (output != NULL); 
    ASSERT (size < 1024u);
    
    /* calc program execute time */
    timer.systick_cnt = 0u;
    SysTick_ENABLLE(ENABLE);   

    /* init global variable */
    memset((u8 *)nCost, 0u, sizeof(nCost));
    /* Trellis start from 000(S2,S1,S0),other State is Impossible */
    for (i=1; i<8; i++)
    {
        nCost[0][i] = 100u; 
    }    
    iLastBuf = 0u; 
    iCurrBuf = 1u;
    nPathBits = 0u; 
 
    nBytes = size;
    while (nBytes > 0)
    {
        nBytesOut = Viterbi_deCode(output, input, nBytes);
        output += nBytesOut;
        DecLen += nBytesOut;
        input += 4;

        if(nBytes > nBytesOut)
        nBytes = nBytes - nBytesOut;
        else
        nBytes = 0;
    }//end while

    SysTick_ENABLLE(DISABLE);

    return DecLen; 
}


//Hamming distance





