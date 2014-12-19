#ifndef __RF_SPIXX_H_
#define __RF_SPIXX_H_

/*** define and type ***/



/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern uint8 TestSOMI(void);
extern void SpiWriteReg(uint8 addr, uint8 value);
//extern uint8 SpiReadReg(uint8 addr);
extern void SpiWriteBurstReg(uint8 addr, uint8 *buffer, uint8 count);
extern void SpiReadBurstReg (uint8 addr, uint8 *buffer, uint8 count);
extern void SpiWriteStrobe(uint8 strobe);
extern uint8 SpiReadStatus(uint8 addr);
extern uint8 SPI_byte(uint8 dat);

#endif   /* end __RF_SPI_H_ */


