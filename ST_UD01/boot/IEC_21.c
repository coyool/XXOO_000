/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：21 协议
*
* 文件标识： 
* 摘    要：升级模块, 进入升级使用了21协议
*
* 当前版本：
* 作    者：wzg
* 完成日期：
* 编译环境：
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

const uint8_t CommadSendB0[]=             
{
    soh, 'B', '0', etx, 0x71,  
};

const uint8_t Commadrequest[]=             
{
    '/', '?', '!' ,er,cr,
};

const uint8_t CommadChangeBraud[]=             
{
    ack,'0','4','1',er,cr,
};

const uint8_t CommadConfirmPass[]=             
{
    soh,'P','4',stx,'(',0x69,0x41,0x6A,0x76,0x30,0x71,0x39,0x37,0x32,
    0x66,0x39,0x31,0x61,0x67,0x4B,0x64,0x39,0x30,0x32,0x30,0x38,0x33,0x41,
    0x37,0x35,0x37,0x33,0x38,0x45,0x34,0x44,0x33,0x33,0x36,0x46,0x46,0x35,
    0x33,0x43,0x33,0x30,0x37,0x32,0x42,0x42,0x46,0x35,0x30,')',etx,0x16,
};

const uint8_t CommadEnterUpgrade[]=             
{
    soh,'E','2',stx,'4','4','.','0','.','0','(',')',etx,0x77,
};

const uint8_t CommadReadProgamVersion[]=             
{
    soh,'R','1',stx,'0','.','2','.','0','(',')',etx,0x51, 
};

const uint8_t sSoftWare_Versions_ASCLL[23]=
{
	'D','D','S','2','6','D','-','X','3','2',
	'8','-','2','0','1','4','0','8','2','3',
	'-','V','1'
};			//  DTS27	

MFS_UARTModeConfigT tUART4800ModeConfigT =
{
    4800,                   /* 4800 or 19200  57600 */
    UART_DATABITS_7,        
    UART_STOPBITS_1,        
    UART_PARITY_EVEN,       
    UART_BITORDER_LSB,
    UART_NRZ,               /* level inversion */  
};

MFS_UARTModeConfigT tUART300ModeConfigT =
{
    300,                   /* 4800 or 19200  57600 */
    UART_DATABITS_7,        
    UART_STOPBITS_1,        
    UART_PARITY_EVEN,       
    UART_BITORDER_LSB,
    UART_NRZ,               /* level inversion */  
};

/*******************************************************************************
* 函数名称: Transmit a data packet using the ymodem protocol
* 输入参数: data
*           length
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void SendPacket(uint8_t *data, uint16_t length)
{
    uint16_t i;
    i = 0;
    
    while (i < length)
    {
        UartSend_Byte(UART52_Ch, data[i]);
        i++;
        bFM3_GPIO_PDOR0_PD = ~bFM3_GPIO_PDIR0_PD; /* LED201 blink */
    }
    //bFM3_GPIO_PDOR0_PD = ~bFM3_GPIO_PDIR0_PD; /* LED201 */
}

/*******************************************************************************
* 函数名称: 
* 输入参数: 
	function bi jiao t1,t2 de  da xiao 
	canshu   t1,L-->H;
			t2,L-->H
			len ,can shu de chang du 
	return     if t1<t2,return 0;
			t2<=t1 return 1;

* 输出参数: 
* --返回值: 
* 函数功能: -- 
*******************************************************************************/
//uint8_t Max3(uint8_t *t1,uint8_t *t2,uint8_t len)
//{
//	uint8_t i;
//	for(i=0;i<len;i++)
//	{
//        if(*(t1+i)<*(t2+i))
//        {     
//            return 0;
//        }
//        else if(*(t1+i)>*(t2+i))
//        { 
//            return 1;
//        }        
//	}		 	
//	return 1;
//}
/*******************************************************************************
* 函数名称:  比对版本号,返回提示
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: -- 
*******************************************************************************/
uint8_t  ReadVersion(void)
{
    uint16_t i;
    uint8_t packet_data[128] = {0};
    volatile uint8_t ackReceived = 0;
    uint32_t errors = 0u;
    volatile uint8_t return_val = 0;
        
    UARTConfigMode(UART52_Ch,&tUART4800ModeConfigT);      // 第一步, 初始化波特率,让表回到 300波特率
    delay_ms(5);    
    SendPacket((uint8_t *)&CommadSendB0,sizeof(CommadSendB0));
    delay_ms(1000);  
    UARTConfigMode(UART52_Ch,&tUART300ModeConfigT);        //更改一下 波特率 300 
    delay_ms(5);     
    SendPacket((uint8_t *)&Commadrequest,sizeof(Commadrequest)); //第二步, 发送请求命令   
    delay_ms(1000);   
    SendPacket((uint8_t *)&CommadChangeBraud,sizeof(CommadChangeBraud)); //第三步, 发送表的更改波特率命令
    delay_ms(500);     
    Get_One_char(UART52_Ch, packet_data);    
    UARTConfigMode(UART52_Ch,&tUART4800ModeConfigT);      //更改一下 波特率 4800
    delay_ms(5);   
    
    do                                                  //读版本号      
    {   
        //MFS_UARTEnableRX(UART52_Ch);
        ackReceived = 0;
        SendPacket((uint8_t *)&CommadReadProgamVersion,sizeof(CommadReadProgamVersion));
        memset(packet_data,0,sizeof(packet_data));
        
        Get_One_char(UART52_Ch, packet_data);   
        MFS_UARTEnableRX(UART52_Ch);
        
        for(i=0;i<50;i++)
        {  
            if (Receive_Byte(UART52_Ch, packet_data + i, 1000) != 0)
            {
               errors++;   // ACC error
               break;    
            }
            else
            {
                _NOP();
            }    
        }
        ackReceived = 1;
        MFS_UARTDisableRX(UART52_Ch);
//        if(memcmp(packet_data+7, &sSoftWare_Versions_ASCLL, sizeof(sSoftWare_Versions_ASCLL)) != 0) // 比对版本号
//        {  
//             if(memcmp(packet_data+7, &sSoftWare_Versions_ASCLL, 12) != 0) // 型号
//             {
//                  return_val = VERSION_TYPE_ERR;  
//             }
//             else
//             {
//                 if(Max3(packet_data+19,(uint8_t *)&sSoftWare_Versions_ASCLL+12,11)) //下载的版本小于读出的版本 ==1
//                 {
//                    return_val = VERSION_HIGH;  
//                 }
//                 else
//                 {
//                    return_val = VERSION_LOW;   
//                 }                           
//             }                        
//        } 
//        else
//        {          
//            return_val = VERSION_EQU;           //名称版本 完成相同
//        } 
        
//        int32_t temp = 0;
//        temp = memcmp(packet_data+8, &sSoftWare_Versions_ASCLL, 11);
        if (memcmp(packet_data+7, &sSoftWare_Versions_ASCLL, 11) != 0) // 比对版本号
        {
            return_val = METER_TYPE_ERR;  
        } 
        else
        {
            if (memcmp(packet_data+19, (uint8_t *)&sSoftWare_Versions_ASCLL+12, 11) <= 0)
            {
                return_val = VERSION_OK; 
            }
            else
            {
                return_val = VERSION_ERR;
            }    
        }    
        
        delay_ms(500);  //loop interval
        
    }while (!ackReceived && (errors < 3u));  
    
    return return_val;
}
/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: -- 
*******************************************************************************/
void Enter21Upgrade(void)
{     
     UARTConfigMode(UART52_Ch,&tUART4800ModeConfigT);      // 第一步, 初始化波特率,让表回到 300波特率
	 delay_ms(5);    
     SendPacket((uint8_t *)&CommadSendB0,sizeof(CommadSendB0));
     delay_ms(1000);  
     UARTConfigMode(UART52_Ch,&tUART300ModeConfigT);        //更改一下 波特率 300 
     delay_ms(5);     
     SendPacket((uint8_t *)&Commadrequest,sizeof(Commadrequest)); //第二步, 发送请求命令   
     delay_ms(1000);   
     SendPacket((uint8_t *)&CommadChangeBraud,sizeof(CommadChangeBraud)); //第三步, 发送表的更改波特率命令
     delay_ms(500);       
     UARTConfigMode(UART52_Ch,&tUART4800ModeConfigT);      //更改一下 波特率 4800
	 delay_ms(5);    
     SendPacket((uint8_t *)&CommadConfirmPass,sizeof(CommadConfirmPass));//第四步, 验证密码
     delay_ms(200);
     SendPacket((uint8_t *)&CommadEnterUpgrade,sizeof(CommadEnterUpgrade));//第五步, 发送进入升级命令
     delay_ms(200);     
}

/*******************************************************************************
* 函数名称:   升级第一步, 进入BOOT 区域, 比对版本号
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: -- 
*******************************************************************************/
void IEC62056_21_Process(void)
{   
    uint8_t verstate;
	
    MFS_UARTEnableTX(UART52_Ch);
    //MFS_UARTEnableRX(UART52_Ch);
    
    verstate =ReadVersion();
    
    switch(verstate)
    {
        case METER_TYPE_ERR:
            BuzzSound(VERSION_ERR);
            printf("METER_TYPE_ERR");
            break;
        case VERSION_ERR:      
            BuzzSound(VERSION_ERR);
            printf("VERSION_ERR");
          break;     
          
        case VERSION_OK:
            BuzzSound(VERSION_OK);
            printf("VERSION_OK");
            Enter21Upgrade();
        break;       
    }

    MFS_UARTDisableTX(UART52_Ch);
    //MFS_UARTDisableRX(UART52_Ch);
}




















