#ifndef __IEC_21_H_
#define __IEC_21_H_

#define soh                      0x01
#define stx                      0x02
#define etx                      0x03
#define eot                      0x04
#define enq                      0x05
#define ack                      0x06
#define cr                       0x0a
#define er                       0x0d
#define nak                      0x15

typedef enum
{
	VERSION_EQU = 0,
    VERSION_LOW,
    VERSION_HIGH, 
    VERSION_TYPE_ERR,
}VERSION_TYPEDEF;

void IEC62056_21_Process(void);

#endif