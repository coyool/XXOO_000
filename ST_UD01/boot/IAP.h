#ifndef __IAP_H_
#define __IAP_H_

typedef signed long long int64;
typedef signed long  int32;
typedef signed short int16;
typedef signed char  int8;

typedef volatile signed long  vint32;
typedef volatile signed short vint16;
typedef volatile signed char  vint8;

typedef unsigned long long uint64;
typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

typedef volatile unsigned long  vuint32;
typedef volatile unsigned short vuint16;
typedef volatile unsigned char  vuint8;
/*** define and type ***/
#define    ApplicationAddress          0x0          /* MX25L(0x0)  MB9AF005 Flash(0x1100) */ 
#define    USER_FLASH_START_ADDRESS    0x0001100
#define    USER_FLASH_END_ADDRESS      0x0001FF80   /* 0x0001FF84 updata flag, the last 0x80 is used for flag, don't touch */ 
#define    FLASH_SIZE                  0x0001FFFF 

#define    NVIC_VectTab_RAM        ((uint32_t)0x20000000)
#define    NVIC_VectTab_FLASH      ((uint32_t)0x00000000)
#define    VectTab_address         (0x00010000)    /* intvec_start, This value must be a multiple of 0x100 */

/* Compute the FLASH upload image size, This value must be a multiple of 0x80(128)  Note[3] */  
#define    FLASH_IMAGE_MAX_SIZE    (uint32_t) (USER_FLASH_END_ADDRESS - (USER_FLASH_START_ADDRESS - 0x0))

/* information */
#define    VERSION_ADDRESS                  (0x00020000)
#define    FLASH_IMAGE_SIZE_ADDRESS         (0x00020100)


typedef  void (*pFunction)(void);

typedef  struct
{
    uint32_t     cnt_now;
    uint32_t     cnt_last;
    uint32_t     cnt_interval;
}CNT_TYPE;

/*** extern variable declarations ***/
//extern pFunction Jump_To_Application;
//extern __IO uint32_t JumpAddress;
//extern uint8_t file_name[FILE_NAME_LENGTH];
extern MFS_UARTModeConfigT tUARTModeConfigT;
extern uint8_t tab_1024[1024];

/*** extern function prototype declarations ***/
extern void IAP(void);


#endif


