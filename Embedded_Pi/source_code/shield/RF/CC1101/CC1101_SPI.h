#ifndef __RF_SPIXX_H_
#define __RF_SPIXX_H_

/*** define and type ***/



/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern u8 TestSOMI(void);
extern void SpiWriteReg(u8 addr, u8 value);
//extern u8 SpiReadReg(u8 addr);
extern void SpiWriteBurstReg(u8 addr, u8 *buffer, u8 count);
extern void SpiReadBurstReg (u8 addr, u8 *buffer, u8 count);
extern void SpiWriteStrobe(u8 strobe);
extern u8 SpiReadStatus(u8 addr);
extern u8 SPI_byte(u8 dat);

#endif   /* end __RF_SPI_H_ */


