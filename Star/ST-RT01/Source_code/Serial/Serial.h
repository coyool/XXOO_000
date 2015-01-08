#ifndef __SERIAL_H_
#define __SERIAL_H_


/*** define and type ***/
typedef enum
{
    bps_300 = 0,
    bps_600,
    bps_1200,
    bps_2400,
    bps_4800,
    bps_9600,
    bps_19200,
    bps_38400,
    bps_57600,
    bps_115200
}SERIAL_BPS_TYPE;

typedef struct 
{
    u8 TxFlag;      // 1 send finish,  0 sending or standby   
    u8 RxFlag;		// 1 receive finish, 0 receiving or standy
    u8 TxCnt;        
    u8 RxCnt; 
    u8 TxBuff[64];
    u8 RxBuff[64];
    // .. 
}SERIAL_TYPE;

/*** extern variable declarations ***/
extern const u8 Serial_fixed_TxRx_Len;
extern SERIAL_TYPE Serial;
extern u32 Serial_timeoutCnt;
extern const u32 Serial_timeout_62ms;


/*** extern function prototype declarations ***/
extern void Serial_begin(SERIAL_BPS_TYPE bps);
extern void Serial_TxInt_enable(void);
extern void Serial_TxInt_disable(void);
extern void Serial_RxInt_enable(void);
extern void Serial_RxInt_disable(void);
extern void Serial_TxMode(void);
extern void Serial_RxMode(void);
extern void Serial_send(void);
extern void Serial_Recv(u8 RevByte);
extern u8 Serial_available(u8 *rxBuff, const u8 len);

#endif   /* end __SERIAL_H_ */


