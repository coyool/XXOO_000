#ifndef ___H_
#define ___H_


/*** define and type ***/
#define RF_DI                  RF_macRecvBuff[5]      
#define RF_MASK                RF_macRecvBuff[8]
#define RF_DATA_COLLECT        RF_macRecvBuff[10]
#define RF_R_SEND_NUMBER       RF_macRecvBuff[11]  
#define SET_R_ADDR             0x04


#define AddFlag                  0
#define SubFlag                  1  

#define BackOrder                0
#define Order                    1

#define NORMAL      0
#define ABNORMAL    1

/*** extern variable declarations ***/
extern u8 RF_availableSendFlag;
extern u8 ReadMeterSerialNumber[8];
extern u8 RF_macRecvBuff[15];

/*** extern function prototype declarations ***/
extern void RF_linkLayerProtocolStack(void);



#endif   /* end ___H_ */

