
#ifndef __Ymodem_CRC16_H_
#define __Ymodem_CRC16_H_


/*** define and type ***/



/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern unsigned short CalculateCRC(const unsigned char *buffer, int count);
extern void CalculateCRCStep(const unsigned char *buffer, int count, unsigned short *pcrc);

extern uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte);
extern uint16_t Cal_CRC16(const uint8_t* data, uint32_t size);


#endif


