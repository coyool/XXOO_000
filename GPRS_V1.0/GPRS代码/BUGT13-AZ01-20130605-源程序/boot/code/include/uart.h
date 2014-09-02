#ifndef _UART_H_
#define _UART_H_

void InitSerial(void);
/**
  * @brief  Print a string on the HyperTerminal
  * @param  s: The string to be printed
  * @retval None
  */
void SerialPutString(uint8_t *s);

uint32_t SerialKeyPressed(uint8_t *key);

void SerialPutChar(uint8_t c);
#define Send_Byte SerialPutChar

uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum);
void Int2Str(uint8_t* str, int32_t intnum);
#endif
