#ifndef __FLASH_H__
#define __FLASH_H__

#include "pdl_header.h"


/* End of the Flash address */
#define    USER_FLASH_END_ADDRESS        0x0001FFFF
#define 	BOOT_ADDRESS           0x00000000
/* Define the user application size */
#define    USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)


uint32_t Write(uint32_t flash_addr, uint32_t *data_addr, uint8_t length);

#endif
