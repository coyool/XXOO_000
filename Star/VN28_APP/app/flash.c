#include "pdl_header.h"
#include "flash.h"

/**
 * @brief 把app_data_buffer的数据写入flash中
 * @param 要写入flash的地址
 * @param 要写入数据的地址
 * @param 要写入的数据长度
 */
uint32_t Write(uint32_t flash_addr,uint32_t *data_addr,uint8_t length)
{
	uint8_t i;
	for(i=0;i<length;i++)
	{
        MFlash_Write((uint16_t*)(flash_addr), *(data_addr+i));
        flash_addr += 4;
	}
    return (0);
}
