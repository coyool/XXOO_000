#ifndef __BOOT_H_
#define __BOOT_H_


/*** define and type ***/
#define NVIC_VectTab_RAM        ((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH      ((uint32_t)0x00000000)
#define ApplicationAddress       APPLICATION_ADDRESS
#define VectTab_address          0x00010000    /* intvec_start, This value must be a multiple of 0x100 */


#define FLASH_STR_ADDR           (0x1000)  
#define FLASH_END_ADDR           (0x1FFFF) 
#define Flash_Read(addr)         *(volatile uint16_t*)((uint32_t)(addr))


/*** extern variable declarations ***/
//extern pFunction Jump_To_Application;
extern uint32_t  JumpAddress;  

/*** extern function prototype declarations ***/
extern void Delay_ms(uint32_t Cnt);
extern void boot(void);
extern uint16_t MFlash_Read16bit(uint16_t* pReadAddr);

extern void Jump_To_app(void);
extern void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);

#endif


