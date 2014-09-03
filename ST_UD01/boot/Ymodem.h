#ifndef __Ymodem_H_
#define __Ymodem_H_


/*** define and type ***/
#define PACKET_SEQNO_INDEX      (1u)
#define PACKET_SEQNO_COMP_INDEX (2u)

#define PACKET_HEADER           (3u)
#define PACKET_TRAILER          (2u)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128u)
#define PACKET_1K_SIZE          (1024u)

#define FILE_NAME_LENGTH        (128u)  /* file name 127 + '\0', (256u) */
#define FILE_SIZE_LENGTH        (16u)

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CAN                     (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

#define NAK_TIMEOUT             (3000u) /* about 3S */ 
#define MAX_ERRORS              (5u)


/*** extern variable declarations ***/
extern uint16_t crcvalue;
//extern uint8_t tab_1024[1024u];
extern volatile const uint32_t  Rev_timeout;
extern uint8_t file_name[FILE_NAME_LENGTH];
extern int32_t flash_image_size;


/*** extern function prototype declarations ***/
extern int32_t Ymodem_Receive (uint8_t *buf);
extern uint8_t Ymodem_Transmit (uint8_t *buf, const uint8_t* sendFileName, uint32_t sizeFile);

extern uint32_t UartSend_Byte (uint8_t Ch, uint8_t c); 
extern void SerialPutString(uint8_t *dat);

#endif   /* end __Ymodem_H_ */








