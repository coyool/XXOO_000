#ifndef _YMODEM_H_
#define _YMODEM_H_
#include <stdint.h>
/**
  * @brief  Receive a file using the ymodem protocol.
  * @param  buf: Address of the first byte.
  * @retval The size of the file.
  */
int32_t Ymodem_Receive (uint8_t *buf);
#endif
