#include <stdint.h>
#include "stm32f10x_flash.h"
#include "iapcmd.h"

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{ 
  /* Unlock the Program memory */
  FLASH_Unlock();

  /* Clear all FLASH flags */  
  FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP | FLASH_FLAG_PGERR
                  | FLASH_FLAG_WRPRTERR | FLASH_FLAG_OPTERR);   
}
void FLASH_If_finish(void)
{
	FLASH_Lock();
}
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  FlashAddress: start address for writing data buffer
  * @param  Data: pointer on data buffer
  * @param  DataLength: length of data buffer (unit is 32-bit word)   
  * @retval 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
int FLASH_If_Write(uint32_t FlashAddress, uint32_t* Data ,uint16_t DataLength)
{
  FLASH_Status status = FLASH_BUSY;
  uint32_t* malPointer = (uint32_t *)Data;

  while (malPointer < (Data + DataLength))
  {
	/* Write the buffer to the memory */
	status = FLASH_ProgramWord(FlashAddress,*malPointer);
  
    if (status != FLASH_COMPLETE)
    {
      /* Error occurred while writing data in Flash memory */
      return (-1);
    }

    /* Check if flash content matches memBuffer */
    if ((*(uint32_t*)FlashAddress) != *malPointer)
    {
        /* flash content doesn't match memBuffer */
        return(-1);
    }
    /* Increment the memory pointer */
    FlashAddress += 4;
	malPointer++;
  }

  return (0);
}

//dst 程序空间地址
//src 缓冲地址 
int copy_ram_to_flash(unsigned int dst,unsigned char*  src,int length)
{
	int len =  length / 4;
	int rt;
	if((length%4) >0)
 		return -1;
	rt = FLASH_If_Write(dst,(uint32_t*)src,len);
	return rt;
}

int erase_sector(unsigned int start,unsigned int end)
{
	unsigned int addr = start;
	while(addr <= end)
	{
		FLASH_Status rt = FLASH_ErasePage(addr);
		if(rt != FLASH_COMPLETE)
			return 1;
		addr += FLASH_PAGE_SIZE;
	}
	return 0;
}
