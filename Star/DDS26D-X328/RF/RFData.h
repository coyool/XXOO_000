

#ifndef _RFDATA_H
#define _RFDATA_H

 
#define 	ReadFreezeRFID	 			0x50
#define     ReadBillingRFID  			0x20
#define     ReadTimeRFID	 			0xF1
#define     ReadGeographyRFID			0xF3
#define     ReadCurrentEnergyRFID		0xA3
#define		ReadBroadcastEnergyRFID 	0xA4
#define     ReadCurrentEnergy1RFID  	0xA5
#define     ReadVoltageCurrentRFID  	0xA9
#define     ReadPowerRFID           	0x83
#define 	ReadPowerFactorRFID			0x8B
#define     ReadStealCountRFID      	0x8A
#define     ReadDisplayMeterIDRFID  	0xAF

#define     WriteMeterIDRFID			0xA6
#define     WriteBroadcastAdjustTime	0xF0
#define     WriteGeographyRFID          0xF2

#define 	LowToHigh                0
#define 	HighToLow                1



extern uint8 RFGetData(uint8 RFDI,uint8 *pBuf);
extern uint8 RFSetData(uint8 RFDI,uint8 *pBuf);

#endif

