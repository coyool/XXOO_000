#include    "all_header_file.h"
#include "string.h"

#define biao 1
#define jizhongqi 2

#define Device biao

#if Device == biao
#define DATALEN_MAX 20
#else
#define DATALEN_MAX 100
#endif

/**************************************************************************************************************
* FUNCTION PROTOTYPES
*/ 
unsigned short fecDecode(unsigned char *pDecData, unsigned char* pInData, unsigned short RemBytes);
static unsigned char hammWeight(unsigned char a);
static unsigned char min(unsigned char a, unsigned char b);
#ifndef NULL
#define NULL 0
#endif
void mem_set(unsigned char *data, unsigned char len, unsigned char n);

/**************************************************************************************************************
* GLOBAL VARIABLES
*/

// The payload + CRC are 31 bytes. This way the complete packet to be received will fit in the RXFIFO 
unsigned char rxBuffer[4];    // Buffer used to hold data read from the RXFIFO (4 bytes are read at a time)
unsigned char rxPacket[31];   // Data + CRC after being interleaved and decoded 

// Look-up source state index when:
//                    Destination state --\   /-- Each of two possible source states 
const unsigned char aTrellisSourceStateLut[8][2] = 
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

// Look-up expected output when:
//                      Destination state --\   /-- Each of two possible source states 
const unsigned char aTrellisTransitionOutput[8][2] = 
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

// Look-up input bit at encoder when:
//                     Destination state                                                     
const unsigned char aTrellisTransitionInput[8] = 
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

/**************************************************************************************************************
* DEFINES
*/

// NUMBER_OF_BYTES_AFTER_DECODING should be given the length of the payload + CRC (CRC is optional)
#define NUMBER_OF_BYTES_AFTER_DECODING  31
#define NUMBER_OF_BYTES_BEFORE_DECODING (4 * ((NUMBER_OF_BYTES_AFTER_DECODING / 2) + 1))

/**************************************************************************************************************
* @fn          hammWeight
*
* @brief       Calculates Hamming weight of byte (# bits set)
*
* @param       a - Byte to find the Hamming weight for
*
* @return      Hamming weight (# of bits set in a)
*/ 
static unsigned char hammWeight(unsigned char a) 
{ 
  a = ((a & 0xAA) >> 1) + (a & 0x55);
  a = ((a & 0xCC) >> 2) + (a & 0x33);
  a = ((a & 0xF0) >> 4) + (a & 0x0F); 
  return a;
} 

/**************************************************************************************************************
* @fn          min
*
* @brief       Returns the minimum of two values
*
* @param       a - Value 1
*              b - Value 2
*
* @return      Minimum of two values
*              Value 1 (Value 1 < Value 2)
*              Value 2 (Value 2 < Value 1)
*/ 
static unsigned char min(unsigned char a, unsigned char b)
{
  return (a <= b ? a : b);  
} 



  // Two sets of buffers (last, current) for each destination state for holding: 
  static unsigned char nCost[2][8];           // Accumulated path cost 
  static unsigned long aPath[2][8];           // Encoder input data (32b window)
   
  // Indices of (last, current) buffer for each iteration
  static unsigned char iLastBuf; 
  static unsigned char iCurrBuf;
   
  // Number of bits in each path buffer
  static unsigned char nPathBits;
   

	  
/**************************************************************************************************************
* @fn          fecDecode
*
* @brief       De-interleaves and decodes a given input buffer
*
* @param       pDecData  - Pointer to where to put decoded data (NULL when initializing at start of packet)
*              pInData   - Pointer to received data
*              nRemBytes - of remaining (decoded) bytes to decode 
*          
*
* @return      Number of bytes of decoded data stored at pDecData
*/ 
unsigned short fecDecode(unsigned char *pDecData, unsigned char* pInData, unsigned short nRemBytes)
{ 
  // Variables used to hold # Viterbi iterations to run, # bytes output,
  // minimum cost for any destination state, bit index of input symbol 
  unsigned char nIterations;
  unsigned short nOutputBytes = 0;
  unsigned char nMinCost;
  signed char iBit = 8 - 2; 
  unsigned char nCost0; 
  unsigned char nCost1;
 
  unsigned char iSrcState0;
  unsigned char iSrcState1;
  unsigned char nInputBit; 
	  
    unsigned char aDeintData[4];
    signed char iOut;
    signed char iIn; 
  
  // Initialize variables at start of packet (and return without doing any more) 
  if (pDecData == NULL) {
    unsigned char n ;
    memset((unsigned char *)nCost, 0, sizeof(nCost));
    for (n = 1; n < 8; n++) 
      nCost[0][n] = 100; 
    iLastBuf = 0; 
    iCurrBuf = 1;
    nPathBits = 0; 
    return 0;
  } 

  { 
    // De-interleave received data (and change pInData to point to de-interleaved data) 
    for (iOut = 0; iOut < 4; iOut++) {
      unsigned char dataByte = 0;
      for (iIn = 3; iIn >= 0; iIn--) 
        dataByte = (dataByte << 2) | ((pInData[iIn] >>( 2 * iOut)) & 0x03);
      aDeintData[iOut] = dataByte;
    }
    pInData = aDeintData;
  }  

  //处理交织数据，一次性最多处理4个字节，一个编码符号(2B)
  // Process up to 4 bytes of de-interleaved input data, processing one encoder symbol (2b) at a time 
  for (nIterations = 16; nIterations > 0; nIterations--) {
    
    unsigned char iDestState; 
    unsigned char symbol  = ((*pInData) >> iBit) & 0x03;
  
    // Find minimum cost so that we can normalize costs (only last iteration used)
    nMinCost = 0xFF;
    
    // Get 2b input symbol (MSB first) and do one iteration of Viterbi decoding    
    if ((iBit -= 2) < 0) {
      iBit = 6; 
      pInData++;            // Update pointer to the next byte of received data
    }
    
    // For each destination state in the trellis, calculate hamming costs for both possible paths into state and
    // select the one with lowest cost. 
    for (iDestState = 0; iDestState < 8; iDestState++) 
    { 
      nInputBit = aTrellisTransitionInput[iDestState];  /* 查看八态图 */
      
      // Calculate cost of transition from each of the two source states (cost is Hamming difference between
      // received 2b symbol and expected symbol for transition)
      iSrcState0 = aTrellisSourceStateLut[iDestState][0];
      nCost0  = nCost[iLastBuf][iSrcState0];
      nCost0 += hammWeight(symbol ^ aTrellisTransitionOutput[iDestState][0]);
      
      iSrcState1 = aTrellisSourceStateLut[iDestState][1];
      nCost1  = nCost[iLastBuf][iSrcState1];
      nCost1 += hammWeight(symbol ^ aTrellisTransitionOutput[iDestState][1]);
      // Select transition that gives lowest cost in destination state, copy that source state's path and add
      // new decoded bit 
      if (nCost0 <= nCost1) {
        nCost[iCurrBuf][iDestState] = nCost0; 
        nMinCost = min(nMinCost, nCost0);
        aPath[iCurrBuf][iDestState] = (aPath[iLastBuf][iSrcState0] << 1) | nInputBit; 
      } else {
        nCost[iCurrBuf][iDestState] = nCost1; 
        nMinCost = min(nMinCost, nCost1);
        aPath[iCurrBuf][iDestState] = (aPath[iLastBuf][iSrcState1] << 1) | nInputBit;
      }
    }   
    nPathBits++;
	//如果编码历史数据足够长，则输出一个解码数据
    // If trellis history is sufficiently long, output a byte of decoded data 
    if (nPathBits == 32) {
      *pDecData++ = (aPath[iCurrBuf][0] >> 24) & 0xFF; 
      nOutputBytes++;
      nPathBits -= 8;
      nRemBytes--;
    }
	//处理过3个符号网络结束后，清除剩余数据
    // After having processed 3-symbol trellis terminator, flush out remaining data 
    if ((nRemBytes <= 3) && (nPathBits == ((8 * nRemBytes) + 3))) {
      while (nPathBits >= 8) { 
        *pDecData++ = (aPath[iCurrBuf][0] >> (nPathBits - 8)) & 0xFF;
        nOutputBytes++;
        nPathBits -= 8;
      } 
      return nOutputBytes;
    } 

    // Swap current and last buffers for next iteration
    iLastBuf = (iLastBuf + 1) % 2;
    iCurrBuf = (iCurrBuf + 1) % 2;    
  }
  
  // Normalize costs so that minimum cost becomes 0
  { 
    unsigned char iState; 
    for (iState = 0; iState < 8; iState++)
      nCost[iLastBuf][iState] -= nMinCost; 
  }
  return nOutputBytes;
}



unsigned char OutData[30];

u8 FEC_Decode(unsigned char *data, unsigned char datalen)
{  
  unsigned short nBytes;
  unsigned char *pDecData;
  unsigned char *pEncData;
  unsigned char DecLen = 0;
  unsigned short nBytesOut;
  
  timer.systick_cnt = 0;
  SysTick_ENABLLE(ENABLE);   
  
  pEncData = data;
  pDecData = data;
  fecDecode(NULL, NULL, 0);

  nBytes = datalen;
  while (nBytes > 0)
  {
    nBytesOut = fecDecode(pDecData, pEncData, nBytes);
    pDecData += nBytesOut;
    DecLen += nBytesOut;
    pEncData += 4;
//    if(DecLen != 0)
//    {
//      if(data[0] > DATALEN_MAX)//命令中，长度最长20
//        return 0;
//      if(DecLen >= data[0])
//      {
//        DecLen = data[0];
//        crc = data[DecLen - 2];
//        crc = (crc << 8) | data[DecLen - 1];
//        crc2 = CRC16Default(data, DecLen - 2);
//        if(crc == crc2)
//        {
//          return DecLen;
//        }else{
//          return 0;
//        }
//      }
//    }
    if(nBytes > nBytesOut)
      nBytes -= nBytesOut;
    else
      nBytes = 0;
  }
  
    SysTick_ENABLLE(DISABLE);
  
  return DecLen;
}