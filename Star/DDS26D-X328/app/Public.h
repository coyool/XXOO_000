#ifndef __PUBLIC_H
#define __PUBLIC_H

#include "Include.h"

//typedef enum {FALSE = 0, TRUE = !FALSE}BOOL_B;
typedef enum {false = 0, true = !false}bool;

#ifndef FALSE
#define FALSE 	false
#endif
#ifndef TRUE
#define TRUE	true
#endif

#define BOOL_B 				bool
/* Export define-----------------------------------------------------------*/
#define CALC_CHECKSUM(x,y)			PublicCrc16(x,y)

#define CHECKSUM_VAL(x)				(*(uint16*)((uint8*)&(x)+sizeof(x)-2))
#define DATALEN(x) 					(sizeof(x)-2)
/*-----------------------------------------------------------------------------
CalcCheckSum使用说明:此宏是专门用来计算结构体checksum的，不用在一般的数组计算上
参数x为要计算的结构体变量，y为结构体里的成员checksum.注意checksum成员要放在结体的最后
同样的IsCheckSumRight是专门为判断用CalcCheckSum宏计算的checksum是否正确，例如:
typedef struct
	{
		uint32 NvmDataAddr;		
		uint32 AllNum;      //总共计录数
		uint16 Index;       //滚动计录计数器
		uint16 NumVal;      //有效计录数
		uint16 MaxNum;		//最大条数
		uint16 DataLen;		//单条数据长度
		uint16 CheckSum;    //信息头校验和
	}tLogHead;
	tLogHead LogHead;
	则应这样调用:
	CalcCheckSum(LogHead)
	IsCheckSumRight(LogHead)
------------------------------------------------------------------------------*/
#define CalcCheckSum(x) 			(CHECKSUM_VAL(x) = CALC_CHECKSUM((uint8*)&(x), DATALEN(x)))
#define IsCheckSumRight(x) 			(((CHECKSUM_VAL(x) == CALC_CHECKSUM((uint8*)&(x), DATALEN(x))))? TRUE:FALSE)

/* External functions-----------------------------------------------------------*/

uint16 PublicCrc16(const uint8 *pBuf, uint16 Len);
void PublicDelayMs(uint32 ms);
void PublicDelayUs(uint16 us);
void PublicSwap(uint8 *pBuf, uint16 Len);
void DelayTest(void);


#endif

