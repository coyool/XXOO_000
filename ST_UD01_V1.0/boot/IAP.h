#ifndef __IAP_H_
#define __IAP_H_


/*** define and type ***/
#define    ApplicationAddress            0x00010000
#define    USER_FLASH_END_ADDRESS        0x0001FF80      /* 0x0001FF84 updata flag */ 
#define    FLASH_SIZE                    0x0001FFFF 

#define NVIC_VectTab_RAM        ((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH      ((uint32_t)0x00000000)
#define VectTab_address          0x00010000    /* intvec_start, This value must be a multiple of 0x100 */

/* Compute the FLASH upload image size */  
#define FLASH_IMAGE_SIZE         (uint32_t) (USER_FLASH_END_ADDRESS - (ApplicationAddress - 0x0))

typedef  void (*pFunction)(void);


/*** extern variable declarations ***/
extern pFunction Jump_To_Application;
extern __IO uint32_t JumpAddress;

extern uint8_t file_name[FILE_NAME_LENGTH];


/*** extern function prototype declarations ***/
extern void IAP(void);


#endif


