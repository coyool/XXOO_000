#include    "all_header_file.h"




//uint8_t flash_read = 0;

//uint16_t crc = 0u;
//uint8_t tab[128] = {0};

uint32_t flash_addr_temp = 0; 

/**
 * @brief 把app_data_buffer的数据写入flash中
 * @param 要写入flash的地址
 * @param 要写入数据的地址
 * @param 要写入的数据长度
 */
uint32_t Write(uint32_t flash_addr,uint32_t *data_addr,uint8_t length)
{
	uint16_t i,j;
    uint8_t tab[128] = {0};
    uint16_t crc = 0u;
    uint8_t flash_read;
    
    j = 0u;
    flash_read = 0u;
    
	for (i=0; i<length; i++)
	{
        flash_read = MFlash_Write((uint16_t*)(flash_addr), *(data_addr+i));
        
        tab[j] = MFlash_Read16bit((uint16_t*)(flash_addr)) & 0xFF;
        tab[j+1] = (MFlash_Read16bit((uint16_t*)(flash_addr)) >>8) & 0xFF;  
        tab[j+2] = MFlash_Read16bit ((uint16_t*)(flash_addr + 2)) & 0xFF;
        tab[j+3] = (MFlash_Read16bit ((uint16_t*)(flash_addr + 2)) >> 8) & 0xFF;
        j = j + 4;
        
        flash_addr += 4;
        flash_addr_temp = flash_addr;
        
        if (0 != flash_read)
        {
            flash_read = 2u;
            return flash_read;    /* flash error 1 or 2 */
        } 
	}
   
    crc = Cal_CRC16(tab,128);      /* memcmp */
    if (crcvalue != crc)
    {
        flash_read = 4u;           /* CRC error */
    }
    else
    {
        flash_read = 0u;           /* normal */
    }    
    
    return flash_read;
}


/**
 * @brief 把app_data_buffer的数据写入flash中
 * @param 要写入flash的地址
 * @param 要写入数据的地址
 * @param 要写入的数据长度
 */
//uint32_t Write(uint32_t flash_addr,uint32_t *data_addr,uint8_t length)
//{
//	uint8_t i;
//	for(i=0;i<length;i++)
//	{
//        MFlash_Write((uint16_t*)(flash_addr), *(data_addr+i));
//        flash_addr += 4;
//	}
//    return (0);
//}
