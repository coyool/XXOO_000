/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：
*
* 文件标识：Ymodem.c
* 摘    要：
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-8-13
* 编译环境：IAR_ARM
* 
* 源代码说明：This file provides all the software functions related to the ymodem 
*             protocol
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
//static  int32_t Receive_Byte (uint8_t *c, uint32_t timeout);
uint32_t UartSend_Byte (uint8_t Ch, uint8_t c);
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout);
static void Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length);
static void Ymodem_PreparePacket(uint8_t *SourceBuf, uint8_t *data, uint8_t pktNo, uint32_t sizeBlk);
static uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte);
static uint16_t Cal_CRC16(const uint8_t* data, uint32_t size);
static void Ymodem_SendPacket(uint8_t *data, uint16_t length);


/*** static variable declarations ***/
__IO uint32_t FlashDestination = ApplicationAddress; /* Flash user program offset */
//uint16_t PageSize = PAGE_SIZE;
//uint32_t EraseCounter = 0x0;
//uint32_t NbrOfPage = 0u;
//FLASH_Status FLASHStatus = FLASH_COMPLETE;
__IO uint32_t RamSource;



/*** extern variable declarations ***/
//uint8_t tab_1024[1024u];
uint8_t file_name[FILE_NAME_LENGTH] = {0};
int32_t flash_image_size = 0u;
volatile const uint32_t  Rev_timeout = 12000u; /* about 10S */ 






/*******************************************************************************
* 函数名称: SerialPutString
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void SerialPutString(uint8_t *dat)
{
    ASSERT(NULL != dat);
    
    if (NULL != dat)
    {
        UARTPollTX_string(dat);
    }    
}

///*******************************************************************************
//* 函数名称:  Receive byte from sender
//* 输入参数:  c: Character 
//*            timeout: Timeout 
//* 输出参数: 
//* --返回值:   0: Byte received
//*            -1: Timeout
//* 函数功能: --
//*******************************************************************************/
//static  int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
//{
//    while (timeout-- > 0u)
//    {
//        if (1u == SerialKeyPressed(c))
//        {
//            return 0u;
//        }
//    }
//    
//    return -1;
//}
//
/*******************************************************************************
* 函数名称: Send a byte
* 输入参数: c: Character
* 输出参数: 
* --返回值: 0: Byte sent
* 函数功能: --
*******************************************************************************/
uint32_t UartSend_Byte (uint8_t Ch, uint8_t c)
{
    //SerialPutChar(c);
    Put_char(Ch, c);
    if (UartUSBCh == Ch)
    {
        bFM3_GPIO_PDOR0_PD = ~bFM3_GPIO_PDIR0_PD; /* LED201 */
    }
        
    return 0u;
}

/*******************************************************************************
* 函数名称: Receive a packet from sender
* 输入参数:  data
*            length
*            timeout
* 输出参数   0: end of transmission
*           -1: abort by sender
*           >0: packet length
* --返回值   0: normally return
*           -1: timeout or packet error
*            1: abort by user
* 函数功能: --
*******************************************************************************/
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
    uint8_t c = 0u;
    uint16_t i = 0u;
    uint16_t packet_size = 0u;
    uint16_t CRC16_fixed = 0u;
    
    *length = 0u;
    
    if (Receive_Byte(UartUSBCh, &c, timeout) != 0u)
    {
        return -1;
    }
    
    switch (c)
    {
        case SOH:    /* 128 byte*/
            packet_size = PACKET_SIZE;
            break;
            
//        case STX:    /* 1024 byte*/
//            packet_size = PACKET_1K_SIZE;
//            break;
            
        case EOT:    /* session apply for end */
            return 0u;
            
        case CAN:    /* force abort with two of CAN */  
            if ((0 == Receive_Byte(UartUSBCh, &c, timeout)) && (CAN == c))
            {
                *length = -1;
                return 0u;
            }
            else
            {
                return -1;
            }
            
        case ABORT1:
        case ABORT2:
            return 1u;
            
        default:
            return -1;
    }
  
    *data = c;
    for (i=1u; i<(packet_size + PACKET_OVERHEAD); i++)
    {
        if (Receive_Byte(UartUSBCh, data + i, timeout) != 0u)
        {
            return -1;
        }
    }
    
    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
    {
        return -1;
    }
    
    CRC16_fixed = data[131];
    CRC16_fixed = CRC16_fixed << 8;
    CRC16_fixed = CRC16_fixed | data [132];
    if (Cal_CRC16(&data[PACKET_HEADER], 128) != CRC16_fixed)   
    {
        return -1;                          
    }

    *length = packet_size;

    return 0u;
}

/*******************************************************************************
* 函数名称: Receive a file using the ymodem protocol
* 输入参数: buf: Address of the first byte
* 输出参数: 
* --返回值: The size of the file
* 函数功能: only support 128 byte with Ymodem protocol
*******************************************************************************/
int32_t Ymodem_Receive (uint8_t *buf)
{
    uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr;
    int32_t i, j, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;
    uint8_t write_flash_check_flag = 0u;
    uint8_t  read_flash_check_flag = 0u; 
    uint8_t       flash_check_flag = 0u;
    uint8_t write_flash_check_flag_01 = 0u; 
    
    /* Initialize FlashDestination variable */
    FlashDestination = ApplicationAddress;

    for (session_done = 0u, errors = 0u, session_begin = 0u; ; )
    {
        for (packets_received = 0u, file_done = 0u, buf_ptr = buf; ; )
        {
            switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
            {
                case 0:
                    errors = 0u;
                    switch (packet_length)
                    {
                        /* Abort by sender */
                        case - 1:
                            UartSend_Byte(UartUSBCh, ACK);
                            return 0;
                        /* End of transmission */
                        case 0:
                            UartSend_Byte(UartUSBCh, ACK);
                            file_done = 1;
                            /* session_done = 1;   Note (1) !!! */   
                            break;
                        /* Normal packet */
                        default:
                            if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                            {
                                UartSend_Byte(UartUSBCh, NAK);
                            }
                            else
                            {
                                if (0u == packets_received)
                                {
                                    /* Filename packet */
                                    /* if (packet_data[PACKET_HEADER] != 0) */
                                    if ((packet_data[PACKET_HEADER] != 0) && (packet_data[4] != 0) && (packet_data[4] != 0x30))  
                                    {
                                        /* Filename packet has valid data */
                                        for (i = 0u, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH); )
                                        {
                                            file_name[i++] = *file_ptr++;
                                        }
                                        file_name[i++] = '\0';
                                        
                                        for (i = 0u, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
                                        {
                                            file_size[i++] = *file_ptr++;
                                        }
                                        file_size[i++] = '\0';
                                        Str2Int(file_size, &size);

                                        /* Test the size of the image to be sent */
                                        /* Image size is greater than Flash size */
                                        if (size > FLASH_IMAGE_MAX_SIZE)  //!!!
                                        {
                                            /* End session */
                                            UartSend_Byte(UartUSBCh, CAN);
                                            UartSend_Byte(UartUSBCh, CAN);
                                            return -1;
                                        }

                                        /* Erase the FLASH block */
                                        //__disable_irq();
                                        //MFlash_SectorErase ((uint16_t*)0x00001000);//擦除第2扇区
                                        //MFlash_SectorErase ((uint16_t*)0x00010000); //擦除第3扇区
                                        
                                        /* Erase extern Flash(MX25L4006) sector 0-32(132K byte) */
                                        flash_check_flag = 0u;
                                        flash_check_flag = MX25L3206_Erase(0u, 32u);                        
                                        /* write meter versions */
                                        write_flash_check_flag = 0u;
                                        write_flash_check_flag = MX25L3206_Write((uint32_t)VERSION_ADDRESS,
                                                                                 file_name,
                                                                                 FILE_NAME_LENGTH);
                                        /* write .bin size */
                                        write_flash_check_flag_01 = 0u;
                                        write_flash_check_flag_01 = MX25L3206_Write((uint32_t)FLASH_IMAGE_SIZE_ADDRESS,
                                                                                    file_size,
                                                                                    FILE_SIZE_LENGTH);
                                        
                                        if ((OK != flash_check_flag) 
                                            && (OK != write_flash_check_flag) 
                                            && (OK != write_flash_check_flag_01))
                                        {
                                            /* End session */
                                            UartSend_Byte(UartUSBCh, CAN);
                                            UartSend_Byte(UartUSBCh, CAN);
                                            return -2;
                                        } 
                                                                   
                                        UartSend_Byte(UartUSBCh, ACK);
                                        UartSend_Byte(UartUSBCh, CRC16);
                                    }
                                    /* Filename packet is empty, end session */
                                    else
                                    {
                                        UartSend_Byte(UartUSBCh, ACK);
                                        file_done = 1;
                                        session_done = 1;
                                        break;
                                    }/* if (packet_data[PACKET_HEADER] != 0) */
                                }
                                /* Data packet */
                                else
                                {
                                    memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
                                    //RamSource = (uint32_t)buf;  /* converter point 32 bit data */
                                    
//                                    for (j = 0;(j < packet_length) && (FlashDestination <  ApplicationAddress + size); j += 4)
//                                    {
//                                        /* write Program into Flash Fujitsu */
//                                        MFlash_Write((uint16_t*)(FlashDestination), *(uint32_t *)RamSource);  //return 0 normal
//
//                                        if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)
//                                        {
//                                            /* End session */
//                                            UartSend_Byte(UartUSBCh, CAN);
//                                            UartSend_Byte(UartUSBCh, CAN);
//                                            return -2;
//                                        }
//                                        FlashDestination += 4;
//                                        RamSource += 4;
//                                    }
                                    
                                    /* write extern Flash(MX25L4006) 128K byte */                                
                                    write_flash_check_flag = 0u;
                                    write_flash_check_flag = MX25L3206_Write((uint32_t)FlashDestination, buf, PACKET_SIZE);
                                    read_flash_check_flag = 0u;
                                    memset(packet_data, 0, PACKET_SIZE + PACKET_HEADER);
                                    read_flash_check_flag = MX25L3206_Read((uint8_t*)(packet_data), (uint32_t)FlashDestination, PACKET_SIZE);
                                    flash_check_flag = 0u;
                                    flash_check_flag = memcmp((uint8_t *)buf, (uint8_t *)packet_data, PACKET_SIZE);
                                    
                                    if ((OK == write_flash_check_flag)
                                        && (OK == read_flash_check_flag) 
                                        && (0 == flash_check_flag))
                                    {
                                        _NOP();
                                    }
                                    else
                                    {
                                        /* End session */
                                        UartSend_Byte(UartUSBCh, CAN);
                                        UartSend_Byte(UartUSBCh, CAN);
                                        return -2;
                                    }    
                                    FlashDestination += PACKET_SIZE;
                                        
                                    UartSend_Byte(UartUSBCh, ACK);
                                }/* if (packets_received == 0) */
                            
                            packets_received ++;
                            session_begin = 1;
                            }/* if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff)) */
                            
                        }/* switch (packet_length) */
                        break;
                               
                case 1:     /* abort by user */
                    UartSend_Byte(UartUSBCh, CAN);
                    UartSend_Byte(UartUSBCh, CAN);
                    return -3;
                    
                default:
                    if (session_begin > 0)
                    {
                        errors ++;
                    }
                    if (errors > MAX_ERRORS)
                    {
                        UartSend_Byte(UartUSBCh, CAN);
                        UartSend_Byte(UartUSBCh, CAN);
                        return 0;
                    }
          
                    /* Initialize FlashDestination variable */
                    FlashDestination = ApplicationAddress;      /* Note 2 !!! */

                    //delay_ms(2000);   /* Rev_byte for for, so don't use it */
                    UartSend_Byte(UartUSBCh, CRC16);   /* send "C" until secureCRT download  */
                    break;
            }/* switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT)) */ 
            
            if (file_done != 0)
            {
                break;
            }
        }/* for (packets_received = 0, file_done = 0, buf_ptr = buf; ;) */
        if (session_done != 0)
        {
            break;
        }
    }/* for (session_done = 0, errors = 0, session_begin = 0; ;) */
    
    return (int32_t)size;
}



/*******************************************************************************
* 函数名称: Prepare the first block
* 输入参数: timeout
*           0: end of transmission
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length)
{
    uint16_t i, j;
    uint8_t file_ptr[10] = {0};

    /* Make first three packet */
    data[0] = SOH;
    data[1] = 0x00;
    data[2] = 0xff;
  
    /* Filename packet has valid data */
    for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH);i++)
    {
        data[i + PACKET_HEADER] = fileName[i];
    }

    data[i + PACKET_HEADER] = 0x00;
  
    Int2Str (file_ptr, *length);
    for (j =0, i = i + PACKET_HEADER + 1; file_ptr[j] != '\0' ; )
    {
        data[i++] = file_ptr[j++];
    }
  
    for (j = i; j < PACKET_SIZE + PACKET_HEADER; j++)
    {
        data[j] = 0;
    }
}

/*******************************************************************************
* 函数名称: Prepare the data packet
* 输入参数: timeout
*           0: end of transmission
* 输出参数: 
* --返回值: 
* 函数功能: 可以改成返回最大错误，errors >= 0x0A  
*           *SourceBuf == 空指针(NULL), need to take care 
*******************************************************************************/
void Ymodem_PreparePacket(uint8_t *SourceBuf, uint8_t *data, uint8_t pktNo, uint32_t sizeBlk)
{
    uint16_t i, size, packetSize;
    uint8_t* file_ptr = NULL;
    uint8_t  read_flash_check_flag = 0u; 
      
    /* Make first three packet */
    packetSize = PACKET_SIZE;
    size = sizeBlk < packetSize ? sizeBlk :packetSize;

    data[0] = SOH;
    data[1] = pktNo;
    data[2] = (~pktNo);             
//    file_ptr = SourceBuf;
//    
//    /* Filename packet has valid data */
//    for (i = PACKET_HEADER; i < size + PACKET_HEADER; i++)
//    {
//        data[i] = *file_ptr++;
//    }
//    if (size <= packetSize)
//    {
//        for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
//        {
//            data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
//        }
//    }
    
    file_ptr = SourceBuf;
    read_flash_check_flag = MX25L3206_Read((uint8_t*)(&data[PACKET_HEADER]),
                                           (uint32_t)file_ptr,
                                           PACKET_SIZE);
    if (OK == read_flash_check_flag)
    {
        _NOP();
    }   
    else
    {
        //errors = 0x0A;
        printf(" read flash check fail ");
    }    
    
    if (size <= packetSize)
    {
        for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
        {
            data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
        }
    }
    
}

/*******************************************************************************
* 函数名称: Update CRC16 for input byte
* 输入参数: CRC input value 
*           input byte
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
    uint32_t crc = crcIn;
    uint32_t in = byte|0x100;
    
    do
    {
        crc <<= 1;
        in <<= 1;
        if(in&0x100)
        {
            ++crc;
        }  
        
        if(crc&0x10000)
        {
            crc ^= 0x1021;
        }
        
    }
    while(!(in&0x10000));
    
    return crc&0xffffu;
}

/*******************************************************************************
* 函数名称: Cal CRC16 for YModem Packet
* 输入参数: data
*           length
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
    uint32_t crc = 0u;
    const uint8_t* dataEnd = data+size;
    
    while(data<dataEnd)
    {  
        crc = UpdateCRC16(crc,*data++);
    }
    
    crc = UpdateCRC16(crc,0u);
    crc = UpdateCRC16(crc,0u);
    
    return crc&0xffffu;
}

///**
//  * @brief  Cal Check sum for YModem Packet
//  * @param  data
//  * @param  length
//   * @retval None
//  */
//uint8_t CalChecksum(const uint8_t* data, uint32_t size)
//{
// uint32_t sum = 0;
// const uint8_t* dataEnd = data+size;
// while(data < dataEnd )
//   sum += *data++;
// return sum&0xffu;
//}

/*******************************************************************************
* 函数名称: Transmit a data packet using the ymodem protocol
* 输入参数: data
*           length
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void Ymodem_SendPacket(uint8_t *data, uint16_t length)
{
    uint16_t i;
    i = 0;
    
    while (i < length)
    {
        UartSend_Byte(UART52_Ch, data[i]);
        i++;
    }
    bFM3_GPIO_PDOR0_PD = ~bFM3_GPIO_PDIR0_PD; /* LED201 */
}

/*******************************************************************************
* 函数名称: Transmit a file using the ymodem protocol
* 输入参数: buf: Address of the first byte
* 输出参数: 
* --返回值: The size of the file
* 函数功能: only support CRC, only support 128 byte a frame 
*           *buf == 空指针(NULL), need to take care 
*******************************************************************************/
uint8_t Ymodem_Transmit (uint8_t *buf, const uint8_t* sendFileName, uint32_t sizeFile)
{
  
    uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD] = {0};
    uint8_t FileName[FILE_NAME_LENGTH] = {0};
    uint8_t *buf_ptr = NULL;      
    //uint8_t tempCheckSum ;
    uint16_t tempCRC, blkNumber;
    uint8_t receivedC[2] = {0};
    uint8_t i;  //CRC16_F = 0
    uint32_t errors, ackReceived, size = 0, pktSize = 0; 
    
    errors = 0;
    ackReceived = 0;
    
/*--------- send file name and file length packet -----------------------------*/
    for (i=0; i<(FILE_NAME_LENGTH - 1); i++)
    {
        FileName[i] = sendFileName[i];
    }
    //CRC16_F = 1;   /* judge CRC16 */      
    
    /* Prepare first block */
    Ymodem_PrepareIntialPacket(&packet_data[0], FileName, &sizeFile);
  
    do 
    {
        delay_ms(5);
        /* Send Packet */
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
        /* Send CRC */
        tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
        UartSend_Byte(UART52_Ch, tempCRC >> 8);
        UartSend_Byte(UART52_Ch, tempCRC & 0xFF);
        
        //delay_ms(1500); /* erase Flash time 2S */
        /* Wait for Ack and 'C' */
        if (0 == Receive_Byte(UART52_Ch, &receivedC[0], Rev_timeout))  
        {
            if (receivedC[0] == ACK)  /* ACK */
            { 
                if (0 == Receive_Byte(UART52_Ch, &receivedC[1], Rev_timeout)
                     && (CRC16 == receivedC[1]))
                {
                    /* Packet transfered correctly */
                    ackReceived = 1;
                    receivedC[1]= 0; //clear "C"
                }
                else
                {
                    /* NAK or ...  send again */
                    errors++; 
                }//end  if Rev "C"    
            }
            else 
            {
                /* NAK or ...  send again */
                errors++; 
            }//end  if Rev ACK    
        }
        else
        {
            errors++;
        }
    }while (!ackReceived && (errors < 0x0A));
  
    if (errors >=  0x0A)
    {
        printf(" send file name and file length packet error ");
        return errors;
    }
    
/*--------- send data packets -----------------------------*/    
    buf_ptr = buf;
    size = sizeFile;
    blkNumber = 0x01;
    /* Here 128 bytes package is used to send the packets */
      
  
    /* Resend packet if NAK  for a count of 10 else end of commuincation */
    while (size)
    {
        /* Prepare next packet */
        Ymodem_PreparePacket(buf_ptr, &packet_data[0], blkNumber, size);
        ackReceived = 0;
        receivedC[0]= 0x00;
        errors = 0;
        do
        {
            /* Send next packet */
            pktSize = PACKET_SIZE;        
            Ymodem_SendPacket(packet_data, pktSize + PACKET_HEADER);
            
            /* Send CRC or Check Sum based on CRC16_F */
            /* Send CRC or Check Sum based on CRC16_F */
            tempCRC = Cal_CRC16(&packet_data[3], pktSize);
            UartSend_Byte(UART52_Ch, tempCRC >> 8);
            UartSend_Byte(UART52_Ch, tempCRC & 0xFF);
               
           // delay_ms(15);
            /* Wait for Ack */
            if ((Receive_Byte(UART52_Ch, &receivedC[0], Rev_timeout) == 0)  
                && (receivedC[0] == ACK))
            {
                ackReceived = 1;  
                if (size > pktSize)
                {
                    buf_ptr += pktSize;  
                    size -= pktSize;                   
//                    if (blkNumber == (FLASH_IMAGE_MAX_SIZE/PACKET_1K_SIZE))   
//                    {
//                        return 0xFF; /*  error */
//                    }
//                    else
//                    {
//                        blkNumber++;
//                    }
                    blkNumber++;  /* only 128 byte */
                }
                else     /* the last frame */                
                {
                    buf_ptr += pktSize;     
                    size = 0;                // !!! 
                }/* if (size > pktSize) */
            }
            else
            {
                errors++;
            }
        }while(!ackReceived && (errors < 0x0A));
        /* Resend packet if NAK  for a count of 10 else end of commuincation */

        if (errors >=  0x0A)
        {
            printf(" send data packets error "); 
            return errors;
        }
    }/* while (size) */
    
    ackReceived = 0;
    receivedC[0] = 0x00;
    errors = 0;
    do 
    {
        UartSend_Byte(UART52_Ch, EOT);
        /* Send (EOT); */
        /* Wait for Ack */
        if ((0u == Receive_Byte(UART52_Ch, &receivedC[0], Rev_timeout)) 
            && (ACK == receivedC[0]))
        {
            ackReceived = 1;  
        }
        else
        {
            errors++;
        }
    }while (!ackReceived && (errors < 0x0A));
    
    if (errors >=  0x0A)
    {
        printf(" send EOT error "); 
        return errors;
    }
 
/*--------- Last packet preparation -----------------------------*/     
    ackReceived = 0;
    receivedC[0] = 0x00;
    errors = 0;

    packet_data[0] = SOH;
    packet_data[1] = 0;
    packet_data [2] = 0xFF;

    for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
    {
        packet_data [i] = 0x00;
    }
  
    do 
    {
        /* Send Packet */
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
        /* Send CRC or Check Sum based on CRC16_F */
        tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
        UartSend_Byte(UART52_Ch, tempCRC >> 8);
        UartSend_Byte(UART52_Ch, tempCRC & 0xFF);

        /* Wait for Ack and 'C' */
        if (Receive_Byte(UART52_Ch, &receivedC[0], Rev_timeout) == 0)  
        {
            if (receivedC[0] == ACK)
            { 
                /* Packet transfered correctly */
                ackReceived = 1;
            }
            else
            {
                 errors++;
            }    
        }
        else
        {
            errors++;
        }

    }while (!ackReceived && (errors < 0x0A));
    /* Resend packet if NAK  for a count of 10  else end of commuincation */
    if (errors >=  0x0A)
    {
        printf(" Last packet error ");
        return errors; 
    }  
    
    return 0; /* file trasmitted successfully */
}

