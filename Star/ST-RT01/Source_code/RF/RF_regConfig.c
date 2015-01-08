/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-12-2
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include  "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/
//const PN[60] = 
//{
//
//};


// PA table
const uint8_t PA_Tab[8] = {0x00,0x12,0x0e,0x34,0x60,0xc5,0xc1,0xc0};  /* 使用PA ramping 功率爬坡 */
//const uint8_t PA_Tab[8] = {0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00};    /* 不使用PA ramping */



#ifdef  Base_Freq_433_925MHz
// Channel number = 0 
// Channel spacing = 199.951172 
// PA ramping = false 
// Address config = No address check 
// Base frequency = 433.924988 
// Packet length = 60 
// Data rate = 19.9852 
// RX filter BW = 101.562500 
// Device address = 0 
// Whitening = true 
// CRC enable = true 
// FEC enable = true
// Packet length mode = Fixed packet length mode. Length configured in PKTLEN register 
// Deviation = 20.629883 
// TX power = 10 
// Manchester enable = false 
// Modulation format = 2-FSK 
// CRC autoflush = false 
// Data format = Normal mode 
// Preamble count = 4 
// Modulated = true 
// Sync word qualifier mode = 30/32 sync word bits detected 
// Carrier frequency = 433.924988 
const uint8_t CC1101_regConfigTab[47][2] = 
{
    {IOCFG2,      0x06},     // 0 IOCFG2         GDO2 OUTPUT PIN CONFIGURATION 
    {IOCFG1,      0x2E},     // 1
    {IOCFG0,      0x2E},     // 2 IOCFG0         GDO0 OUTPUT PIN CONFIGURATION 
    {FIFOTHR,     0x47},     // 3 FIFOTHR        RX FIFO AND TX FIFO THRESHOLDS 
    {SYNC1,       0xAD},     // 4 SYNC1          SYNC WORD, HIGH BYTE 
    {SYNC0,       0x9B},     // 5 SYNC0          SYNC WORD, LOW BYTE 
    {PKTLEN,      0x3C},     // 6 PKTLEN         PACKET LENGTH  60 byte
    {PKTCTRL1,    0x04},     // 7
    {PKTCTRL0,    0x44},     // 8 PKTCTRL0       PACKET AUTOMATION CONTROL
    {ADDR,        0x00},     // 9
    {CHANNR,      0x00},     // 10
    {FSCTRL1,     0x08},     // 11 FSCTRL1       FREQUENCY SYNTHESIZER CONTROL
    {FSCTRL0,     0x00},     // 12 
    {FREQ2,       0x10},     // 13 FREQ2         FREQUENCY CONTROL WORD, HIGH BYTE 
    {FREQ1,       0xB0},     // 14 FREQ1         FREQUENCY CONTROL WORD, MIDDLE BYTE 
    {FREQ0,       0x7E},     // 15 FREQ0         FREQUENCY CONTROL WORD, LOW BYTE 
    {MDMCFG4,     0xC9},     // 16 MDMCFG4       MODEM CONFIGURATION 
    {MDMCFG3,     0x93},     // 17 MDMCFG3       MODEM CONFIGURATION 
    {MDMCFG2,     0x03},     // 18 MDMCFG2       MODEM CONFIGURATION 
    {MDMCFG1,     0xA2},     // 19 MDMCFG1       MODEM CONFIGURATION  0x22+0x80  FEC enable
    {MDMCFG0,     0xF8},     // 20
    {DEVIATN,     0x35},     // 21 DEVIATN       MODEM DEVIATION SETTING 
    {MCSM2,       0x07},     // 22
    {MCSM1,       0x30},     // 23  
    {MCSM0,       0x18},     // 24 MCSM0         MAIN RADIO CONTROL STATE MACHINE CONFIGURATION 
    {FOCCFG,      0x1D},     // 25 FOCCFG        FREQUENCY OFFSET COMPENSATION CONFIGURATION 
    {BSCFG,       0x1C},     // 26 BSCFG         BIT SYNCHRONIZATION CONFIGURATION 
    {AGCCTRL2,    0xC7},     // 27 AGCCTRL2      AGC CONTROL 
    {AGCCTRL1,    0x00},     // 28 AGCCTRL1      AGC CONTROL 
    {AGCCTRL0,    0xB2},     // 29 AGCCTRL0      AGC CONTROL 
    {WOREVT1,     0x87},     // 30
    {WOREVT0,     0x6B},     // 31
    {WORCTRL,     0xFB},     // 32 WORCTRL       WAKE ON RADIO CONTROL
    {FREND1,      0xB6},     // 33 FREND1        FRONT END RX CONFIGURATION
    {FREND0,      0x17},     // 34 Tx power configuration PA raming
    {FSCAL3,      0xE9},     // 35 FSCAL3        FREQUENCY SYNTHESIZER CALIBRATION 
    {FSCAL2,      0x2A},     // 36 FSCAL2        FREQUENCY SYNTHESIZER CALIBRATION 
    {FSCAL1,      0x00},     // 37 FSCAL1        FREQUENCY SYNTHESIZER CALIBRATION 
    {FSCAL0,      0x1F},     // 38 FSCAL0        FREQUENCY SYNTHESIZER CALIBRATION
    {RCCTRL1,     0x41},     // 39
    {RCCTRL0,     0x00},     // 40
    {FSTEST,      0x59},     // 41
    {PTEST,       0x7F},     // 42
    {AGCTEST,     0x3F},     // 43
    {RF_TEST2,    0x81},     // 44 TEST2         VARIOUS TEST SETTINGS 
    {RF_TEST1,    0x35},     // 45 TEST1         VARIOUS TEST SETTINGS 
    {RF_TEST0,    0x09},     // 46 TEST0         VARIOUS TEST SETTINGS 
};
#endif



#define Base_Freq_408_925MHz
#ifdef  Base_Freq_408_925MHz
// Base frequency = 408.924744 
// Device address = 0 
// CRC autoflush = false 
// PA ramping = false 
// Modulation format = 2-FSK 
// Deviation = 47.607422 
// Carrier frequency = 408.924744 
// Modulated = true 
// CRC enable = true 
// Whitening = true
// Preamble count = 4 
// Packet length mode = Fixed packet length mode. Length configured in PKTLEN register 
// Sync word qualifier mode = 30/32 sync word bits detected 
// Packet length = 60 
// TX power = 10 
// Channel spacing = 199.951172 
// Channel number = 0 
// Data format = Normal mode 
// Address config = No address check 
// RX filter BW = 325.000000  
// Manchester enable = false 
// Data rate = 99.9756 

const uint8_t CC1101_regConfigTab[47][2] = 
{
	{IOCFG2,      0x06},     // 0       GDO2 OUTPUT PIN CONFIGURATION
	{IOCFG1,      0x2E},     // 1
	{IOCFG0,      0x2E},     // 2       GDO0 OUTPUT PIN CONFIGURATION 
	{FIFOTHR,     0x47},     // 3       RX FIFO AND TX FIFO THRESHOLDS 
	{SYNC1,       0x9B},     // 4       SYNC WORD, HIGH BYTE 
	{SYNC0,       0xAD},     // 5       SYNC WORD, LOW BYTE 
	{PKTLEN,      0x3C},     // 6       PACKET LENGTH      60 byte
	{PKTCTRL1,    0x04},     // 7
	{PKTCTRL0,    0x44},     // 8       PACKET AUTOMATION CONTROL
	{ADDR,        0x00},     // 9
	{CHANNR,      0x00},     // 10
	{FSCTRL1,     0x08},     // 11      FREQUENCY SYNTHESIZER CONTROL 
	{FSCTRL0,     0x00},     // 12
	{FREQ2,       0x0F},     // 13      FREQUENCY CONTROL WORD, HIGH BYTE 
	{FREQ1,       0xBA},     // 14      FREQUENCY CONTROL WORD, MIDDLE BYTE 
	{FREQ0,       0x56},     // 15      FREQUENCY CONTROL WORD, LOW BYTE 
	{MDMCFG4,     0x5B},     // 16      MODEM CONFIGURATION 
	{MDMCFG3,     0xF8},     // 17      MODEM CONFIGURATION 
	{MDMCFG2,     0x03},     // 18      MODEM CONFIGURATION
	{MDMCFG1,     0x22},     // 19
	{MDMCFG0,     0xF8},     // 20
	{DEVIATN,     0x47},     // 21
	{MCSM2,       0x07},     // 22
	{MCSM1,       0x30},     // 23  
	{MCSM0,       0x18},     // 24      MAIN RADIO CONTROL STATE MACHINE CONFIGURATION 
	{FOCCFG,      0x1D},     // 25      FREQUENCY OFFSET COMPENSATION CONFIGURATION 
	{BSCFG,       0x1C},     // 26      BIT SYNCHRONIZATION CONFIGURATION 
	{AGCCTRL2,    0xC7},     // 27      AGC CONTROL 
	{AGCCTRL1,    0x00},     // 28      AGC CONTROL 
	{AGCCTRL0,    0xB2},     // 29      AGC CONTROL 
    {WOREVT1,     0x87},     // 30 
    {WOREVT0,     0x6B},     // 31	
	{WORCTRL,     0xFB},     // 32      WAKE ON RADIO CONTROL 
	{FREND1,      0xB6},     // 33      FRONT END RX CONFIGURATION 
    {FREND0,      0x10},     // 34      发射功率	
	{FSCAL3,      0xEA},     // 35      FREQUENCY SYNTHESIZER CALIBRATION 
	{FSCAL2,      0x2A},     // 36      FREQUENCY SYNTHESIZER CALIBRATION 
	{FSCAL1,      0x00},     // 37      FREQUENCY SYNTHESIZER CALIBRATION 
	{FSCAL0,      0x1F},     // 38      FREQUENCY SYNTHESIZER CALIBRATION
    {RCCTRL1,     0x41},     // 39
    {RCCTRL0,     0x00},     // 40
    {FSTEST,      0x59},     // 41	
    {PTEST,       0x7F},     // 42
    {AGCTEST,     0x3F},     // 43
    {RF_TEST2,    0x88},     // 44  
    {RF_TEST1,    0x31},     // 45   
    {RF_TEST0,    0x0B},     // 46
};
#endif 





