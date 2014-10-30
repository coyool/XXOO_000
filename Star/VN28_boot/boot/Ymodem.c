/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：Ymodem protocol
*
* 文件标识：Ymodem.c
* 摘    要：file  transmit protocol
*
* 当前版本：V1.0
* 作    者：F06553
* 完成日期：2014-7-4
* 编译环境：IAR_for_ARM
* 
* 源代码说明： upper PC tool -- secureCRT 
*   warning： 1. Ymodem only support 128 byte of frame
*             2. FileName
*
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
uint32_t temp = 0;


/*** extern variable declarations ***/
uint8_t FileName[FILE_NAME_LENGTH] = {0};
//uint32_t flashdestination, ramsource;   
uint16_t crcvalue = 0; 

/*******************************************************************************
* 函数命名: Receive_Packet
* 输入参数: - data  -length  - timeout
* 输出参数: - *length: 
*                   0: end of transmission
*                  -1: abort by sender
*                  >0: packet length
* —返回值:         0: normally return
*                 -1: timeout or packet error
*                  1: abort by user
* 函数功能:  Receive Ymodem a block
*******************************************************************************/
/* 接收一包数据  参数分别为     数据缓存        一包长度           超时时间 */
int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
    uint16_t i, packet_size;  //一包数据长度
    uint8_t c;               //接收一个字符的临时保存变量
    
    crcvalue = 0; 
    *length = 0;
    if (Receive_Byte(&c, timeout) != 0)
    {
        return -1;  //如果在规定时间  没有字符传过来  就直接退出该函数
    }
    //如果接收到一个字符了  说明收到一个串口命令     判断是什么命令
    switch (c)
    {
        case 0x01://设置包长度为128字节命令   /* start of 128-byte data packet */
                    packet_size=128;
                    break;
//        case 0x02://设置包长度为1024字节命令  /* start of 1024-byte data packet */
//                    packet_size=1024;
//                    break;
        case 0x04:// 传输结束命令             /* end of transmission */
                    __NOP();
                    return 0;
//                    break;
        case 0x18://传输成功后的结束命令      /* two of these in succession aborts transfer */
                    if ((Receive_Byte(&c, timeout) == 0) && (c == 0x18))  //接收到一个字符  并且收到结束命令
                    {
                        *length = -1;//长度
                        return 0;   //正常退出
                    }
                    else
                    {
                        return -1;  // 异常退出
                    }
        case 0x41:                  /* 'A' == 0x41, abort by user */
        case 0x61:                  /* 'a' == 0x61, abort by user */
                    return 1;
        default:
                    return -1;
    }
    *data = c;      //保存收到的字符c

    for (i=1; i<(packet_size + 5); i ++)   /* 接收一包的数据  (包括数据包头的3 + 数据包尾2) */
    {
        if (Receive_Byte(data + i, timeout) != 0)   /* 数据保存在data数组中 */
        {
            return -1;
        }
    }
    //包头校验
    if (data[1] != ((data[2] ^ 0xff) & 0xff))  //data[1] 中的数据必须等于data[2] 中的数据取反
    {
        return -1;
    }
    //CRC校验
    crcvalue = (data[132] | (data[131]<<8));
    if (Cal_CRC16(&data[3], 128) != crcvalue)   /* 将data数组中，其中128个纯数据取出做CRC校验 */
    {
        return -1;                             //如果校验失败，则终止接收
    }

    *length = packet_size;
    return 0;
}

/*******************************************************************************
* 函数命名: Ymodem
* 输入参数: - address of the first byte
* 输出参数: - None 
* —返回值: - The size of the file 
* 函数功能: Receive a file using the ymodem protocol
*******************************************************************************/
int32_t Ymodem_Receive (uint8_t *buf)
{
    uint8_t packet_data[128 + 5], file_size[50], *file_ptr, *buf_ptr;
    int32_t i, packet_length, session_done, file_done, errors, session_begin, size = 0, packets_received = 0;
    uint32_t flashdestination, ramsource;

    /* Initialize flashdestination variable */
    flashdestination = 0;
    flashdestination = APPLICATION_ADDRESS;
    //flashdestination = 0x1100;

    for (session_done = 0, errors = 0, session_begin = 0; ;)
    {
        for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
        {
            switch (Receive_Packet(packet_data, &packet_length, 0x10000))//纯数据缓存在 packet_data 中 0x1000
            {
                case 0://正常传输 or 传输结束
                        errors = 0;
                        switch (packet_length)//数据包长度
                        {

                            case -1:/* Abort by sender */
                                    Put_char(ACK);
                                    return 0;

                            case 0:/* End of transmission */
                                    Put_char(ACK);
                                    file_done = 1;
                                   // session_done = 1; //ybz 改变Ymodem协议 多个bin文件不可以连续传输。因为升级中掉电后，bin文件还存在在上位机的进程中，上电后再给一个bin文件升级时，其实是download了两个bin文件. 加上此句会造成上位机卡死。 
                                    break;

                            default:/* Normal packet */
                                    if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                                    {
                                        Put_char(NAK);
                                    }
                                    else
                                    {
                                        if (packets_received == 0)
                                        {
                                            /* Filename packet */
                                            if ((packet_data[PACKET_HEADER] != 0) && (packet_data[4] != 0) && (packet_data[4] != 0x30))
                                            {
                                                /* Filename packet has valid data */
                                                for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                                                {
                                                    FileName[i++] = *file_ptr++;
                                                }
                                                FileName[i++] = '\0';
                                                for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH); )
                                                {
                                                    file_size[i++] = *file_ptr++;
                                                }
                                                file_size[i++] = '\0';
                                                Str2Int(file_size, &size);
                                                temp = size; 
                                                /* Test the size of the image to be sent */
                                                /* Image size is greater than Flash size */

                                                if (size > USER_FLASH_SIZE)   //USER_FLASH_SIZE + 1
                                                {
                                                    /* End session */
                                                    Put_char(CA);
                                                    Put_char(CA);
                                                    return -1;       //写入bin文件的size 大于 Flash可以写入的size
                                                }
                                                /* erase user application area */
                                                __disable_irq();
                                               // MFlash_SectorErase ((uint16_t*)0x00001000);//擦除第2扇区
                                                MFlash_SectorErase ((uint16_t*)0x00010000); //擦除第3扇区
                                                Put_char(ACK);
                                                Put_char(CRC16);
                                                __NOP();
                                            }
                                            /* Filename packet is empty, end session */
                                            else
                                            {
                                                Put_char(ACK);
                                                file_done = 1;
                                                session_done = 1;
                                                break;
                                            }
                                        }  /* Filename packet */
                                        else /* Data packet */
                                        {
                                            memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
                                            ramsource = (uint32_t)buf;
                                            
                                            /* Write received data in Flash */
                                            //                  要写的地址           数据首地址              数据长度
                                            if (0 == Write(flashdestination, (uint32_t*)ramsource, (uint16_t)packet_length/4))   
                                            {
                                                Put_char(ACK);
                                                flashdestination = flashdestination + packet_length;
                                            }
                                            else /* An error occurred while writing to Flash memory */
                                            {
                                                /* End session */
                                                Put_char(CA);
                                                Put_char(CA);
                                                return -2;
                                            }
                                        } //end  if(packets_received == 0)
                                        packets_received++;
                                        session_begin = 1;
                                    }//end if()   /* Normal packet */
                        }
                        break;  //break  case 0://正常传输 or 传输结束

                case 1://abort by user
                        Put_char(CA);
                        Put_char(CA);
                        return -3;

                default:    //如果会话开始后发生错误
                        if (session_begin > 0)
                        {
                            errors++;
                        }
                        if (errors > MAX_ERRORS)  //错误累积到一定程度之后，终止会话。  （5次) 
                        {
                            Put_char(CA);
                            Put_char(CA);
                            return 0;
                        }
                        
                        flashdestination = 0; //这两句修复了，升级过程中掉电。再上电后重新发送APP_bin第一次发生错误后，再次发送APP_bin写入flash地址错误 (正确的地址应为0x0000_1100)
                        flashdestination = APPLICATION_ADDRESS; //重新发送C字符后， 重新初始化APP_address 
                        //flashdestination = 0x1100;
                        Delay_ms(1000);  //ybz
                        
                        Put_char(CRC16);  //如果没有接收到字符  就一直向终端发送字符C
                        break; //break  switch
            }
            if (file_done != 0) //发送文件过程有错，退出文件传输
            {
                break;    //break  for (file_done = 0)
            }
        }
        if (session_done != 0)  //会话发生错误， 退出会话
        {
            break;       //break  for (session_done = 0)
        }
    }
    return (int32_t)size;
}
