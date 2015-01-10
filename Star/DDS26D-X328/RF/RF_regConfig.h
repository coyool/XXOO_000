#ifndef __RF_REG_CONFIG_H_
#define __RF_REG_CONFIG_H_

/*** define and type ***/
// TX power 
#define PA_LEVEL_0   0x10
#define PA_LEVEL_1   0x11
#define PA_LEVEL_2   0x12
#define PA_LEVEL_3   0x13
#define PA_LEVEL_4   0x14
#define PA_LEVEL_5   0x15
#define PA_LEVEL_6   0x16
#define PA_LEVEL_7   0x17

#define TI_CCxxx0_NUM_RXBYTES  0x7F
#define CRC_POSITION           14
#define RF_CRC_OK              0x80

#define IOCFG2       0x00        // GDO2 output pin configuration
#define IOCFG1       0x01        // GDO1 output pin configuration
#define IOCFG0       0x02        // GDO0 output pin configuration
#define FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define SYNC1        0x04        // Sync word, high byte
#define SYNC0        0x05        // Sync word, low byte
#define PKTLEN       0x06        // Packet length
#define PKTCTRL1     0x07        // Packet automation control
#define PKTCTRL0     0x08        // Packet automation control
#define ADDR         0x09        // Device address
#define CHANNR       0x0A        // Channel number
#define FSCTRL1      0x0B        // Frequency synthesizer control
#define FSCTRL0      0x0C        // Frequency synthesizer control
#define FREQ2        0x0D        // Frequency control word, high byte
#define FREQ1        0x0E        // Frequency control word, middle byte
#define FREQ0        0x0F        // Frequency control word, low byte
#define MDMCFG4      0x10        // Modem configuration
#define MDMCFG3      0x11        // Modem configuration
#define MDMCFG2      0x12        // Modem configuration
#define MDMCFG1      0x13        // Modem configuration
#define MDMCFG0      0x14        // Modem configuration
#define DEVIATN      0x15        // Modem deviation setting
#define MCSM2        0x16        // Main Radio Cntrl State Machine config
#define MCSM1        0x17        // Main Radio Cntrl State Machine config
#define MCSM0        0x18        // Main Radio Cntrl State Machine config
#define FOCCFG       0x19        // Frequency Offset Compensation config
#define BSCFG        0x1A        // Bit Synchronization configuration
#define AGCCTRL2     0x1B        // AGC control
#define AGCCTRL1     0x1C        // AGC control
#define AGCCTRL0     0x1D        // AGC control
#define WOREVT1      0x1E        // High byte Event 0 timeout
#define WOREVT0      0x1F        // Low byte Event 0 timeout
#define WORCTRL      0x20        // Wake On Radio control
#define FREND1       0x21        // Front end RX configuration
#define FREND0       0x22        // Front end TX configuration
#define FSCAL3       0x23        // Frequency synthesizer calibration
#define FSCAL2       0x24        // Frequency synthesizer calibration
#define FSCAL1       0x25        // Frequency synthesizer calibration
#define FSCAL0       0x26        // Frequency synthesizer calibration
#define RCCTRL1      0x27        // RC oscillator configuration
#define RCCTRL0      0x28        // RC oscillator configuration
#define FSTEST       0x29        // Frequency synthesizer cal control
#define PTEST        0x2A        // Production test
#define AGCTEST      0x2B        // AGC test
#define RF_TEST2     0x2C        // Various test settings
#define RF_TEST1     0x2D        // Various test settings
#define RF_TEST0     0x2E        // Various test settings 

// Strobe commands
#define SRES         0x30        // Reset chip.
#define SFSTXON      0x31        // Enable/calibrate freq synthesizer
#define SXOFF        0x32        // Turn off crystal oscillator.
#define SCAL         0x33        // Calibrate freq synthesizer & disable
#define SRX          0x34        // Enable RX.
#define STX          0x35        // Enable TX.
#define SIDLE        0x36        // Exit RX / TX
#define SAFC         0x37        // AFC adjustment of freq synthesizer
#define SWOR         0x38        // Start automatic RX polling sequence
#define SPWD         0x39        // Enter pwr down mode when CSn goes hi
#define SFRX         0x3A        // Flush the RX FIFO buffer.
#define SFTX         0x3B        // Flush the TX FIFO buffer.
#define SWORRST      0x3C        // Reset real time clock.
#define SNOP         0x3D        // No operation.

// Status registers
#define PARTNUM      0x30        // Part number
#define VERSION      0x31        // Current version number
#define FREQEST      0x32        // Frequency offset estimate
#define LQI          0x33        // Demodulator estimate for link quality
#define RSSI         0x34        // Received signal strength indication
#define MARCSTATE    0x35        // Control state machine state
#define WORTIME1     0x36        // High byte of WOR timer
#define WORTIME0     0x37        // Low byte of WOR timer
#define PKTSTATUS    0x38        // Current GDOx status and packet status
#define VCO_VC_DAC   0x39        // Current setting from PLL cal module
#define TXBYTES      0x3A        // Underflow and # of bytes in TXFIFO
#define RXBYTES      0x3B        // Overflow and # of bytes in RXFIFO
#define NUM_RXBYTES  0x7F        // Mask "# of bytes" field in _RXBYTES

// Other memory locations
#define PATABLE      0x3E
#define TXFIFO       0x3F
#define RXFIFO       0x3F

#define RXFIFOSINGLE 0xBF

// Masks for appended status bytes
#define LQI_RX       0x01        // Position of LQI byte
#define CRC_OK       0x80        // Mask "CRC_OK" bit within LQI byte

// Definitions to support burst/single access:
#define WRITE_BURST  0x40
#define READ_SINGLE  0x80
#define READ_BURST   0xC0


/*** extern variable declarations ***/
extern const uint8_t PA_Tab[8];
extern const uint8_t CC1101_regConfigTab[47][2];



/*** extern function prototype declarations ***/



#endif   /* end __RF_REG_CONFIG_H_ */








