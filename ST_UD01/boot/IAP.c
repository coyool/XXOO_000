/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：IAP
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014/8/18
* 编译环境：E:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/
static void refresh_flash(void);
static void download_program_to_meter(void);
//static void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);
//static void Jump_To_app(void);

/*** static variable declarations ***/
uint8_t tab_1024[1024] = {0};
//uint8_t file_name[FILE_NAME_LENGTH];
MFS_UARTModeConfigT tUARTModeConfigT =
{
    57600,                   /* 4800 or 19200  57600 */
    UART_DATABITS_8,        
    UART_STOPBITS_1,        
    UART_PARITY_NONE,       
    UART_BITORDER_LSB,
    UART_NRZ,               /* level inversion */  
};



/*** extern variable declarations ***/
//pFunction Jump_To_Application;
//__IO uint32_t JumpAddress;







/*******************************************************************************
* 函数名称: 
* 输入参数: NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
*          This parameter can be one of the following values:
*       1: NVIC_VectTab_RAM
*       2: NVIC_VectTab_FLASH
*  Offset: Vector Table base offset field. This value must be a multiple of 0x100.
* 输出参数: 
* --返回值: None
* 函数功能: Sets the vector table location and Offset.
* warning:  SP最低两位永远是0，这意味着堆栈总是4字节对齐  
*******************************************************************************/
//void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
//{
//    //
//    //
//    SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
//}

/*******************************************************************************
* 函数名称: PC jump function
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
//void Jump_To_app(void)
//{
//    __disable_irq();//关闭总中断
//    JumpAddress = *(volatile uint32_t*) (VectTab_address + 4);
//    Jump_To_Application = (pFunction) JumpAddress;
//    /* Initialize user application's Stack Pointer */
//    __set_MSP(*(__IO uint32_t*) VectTab_address);                 /* set SP_main */
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTab_address);     /* 配置中断向量表偏移量寄存器 */
//    Jump_To_Application();
//}

/*******************************************************************************
* 函数名称: refresh_flash
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void refresh_flash(void)
{
    uint8_t Number[10] = "          ";
    int32_t Size = 0;

    MFS_UARTEnableRX(UartUSBCh);
    
    SerialPutString("Waiting for the file to be sent ...\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);
    
    MFS_UARTDisableRX(UartUSBCh);
    
    if (Size > 0)
    {
        SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
        SerialPutString(file_name);
        Int2Str(Number, Size);
        SerialPutString("\n\r Size: ");
        SerialPutString(Number);
        SerialPutString(" Bytes\r\n");
        SerialPutString("--------------------------------\r\n");
        
        oneSound(10,0);
        
        /* read extern Flash(MX25L4006) verify image */
#define  TEST_IMAGE
#ifdef   TEST_IMAGE     
        uint32_t i = 0;
        uint32_t j = ApplicationAddress; 
        memset(&tab_1024[0], 0, PACKET_SIZE);
        for (i=0u; i<1056; i++) /* (132*1024)/128 0-32 sector */
        {
            MX25L3206_Read((uint8_t *)tab_1024, j, PACKET_SIZE);
            j += PACKET_SIZE;
            printHex(tab_1024,PACKET_SIZE);
        }          
#endif        
        /* check CRC32 */
        //...
        /* updata image complete, jump application routine */
        //Jump_To_app();  
        //while (1);  
    }
    else if (Size == -1)
    {
        SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
    }
    else if (Size == -2)
    {
        SerialPutString("\n\n\rVerification failed!\n\r");
    }
    else if (Size == -3)
    {
        SerialPutString("\r\n\nAborted by user.\n\r");
    }
    else
    {
        SerialPutString("\n\rFailed to receive the file!\n\r");
    }
    
    bFM3_GPIO_PDOR0_PD = 1; /* LED 201 light off */
    bFM3_GPIO_PDOR0_PC = 1; /* LED 202 light off */
}

/*******************************************************************************
* 函数名称: download_program_to_meter
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void download_program_to_meter(void)
{
    uint32_t status = 0u; 
    uint8_t tick_C = 0u;
    uint8_t Rev_flag = 0u;
    uint8_t read_flash_check_flag = 0u;
    uint8_t file_size[FILE_SIZE_LENGTH] = {0};
    
    /* wait for meter send 'C', meter printf something before send 'C' */
    delay_ms(1000);  
    
    MFS_UARTSWRst(UART52_Ch);  //reset UART
    UARTConfigMode(UART52_Ch, &tUARTModeConfigT);
    MFS_UARTEnableRX(UART52_Ch);
    MFS_UARTEnableTX(UART52_Ch);
    
    //SerialPutString("\n\n\rSelect Receive File ... \n\r");
    SerialPutString("\n\n\rdownlod image .bin ... \n\r");
    Rev_flag = Receive_Byte(UART52_Ch , &tick_C, Rev_timeout); 
    if (0u == Rev_flag)
    {
        _NOP();
    }    
    else
    {
        bFM3_GPIO_PDOR0_PD = 1; /* LED 201 light off */
        return;
    }    
    
    if (tick_C == CRC16)
    {
        read_flash_check_flag = 0u;
        read_flash_check_flag = MX25L3206_Read((uint8_t*)(file_size),
                                               (uint32_t)FLASH_IMAGE_SIZE_ADDRESS,
                                               FILE_SIZE_LENGTH);
        if (OK != read_flash_check_flag)
        {
            return;
        }    
        Str2Int(file_size, &flash_image_size);  /* str to hex */
        
        /* Transmit the flash image through ymodem protocol */
        status = Ymodem_Transmit((uint8_t*)ApplicationAddress,   //!!!!Note
                                 (const uint8_t*)"DownloadFlashImage.bin",
                                 flash_image_size);  /* , ,FLASH_IMAGE_MAX_SIZE */
        
        MFS_UARTDisableRX(UART52_Ch);
        
        if (status != 0) 
        {
            SerialPutString("\n\rError Occured while Transmitting File\n\r");
            oneSound(10, 300);  /* BUZZER 201 buzz */
        }
        else
        {
            SerialPutString("\n\rFile Trasmitted Successfully \n\r");
            oneSound(10, 0);  /* BUZZER 201 buzz */
        }
    }
    else
    {
        SerialPutString("\r\n\nAborted by user or no Rev 'C' .\n\r");  
    }
    
    bFM3_GPIO_PDOR0_PD = 1; /* LED 201 light off */
    bFM3_GPIO_PDOR0_PC = 1; /* LED 202 light off */
    //MFS_UARTDisableRX(UART52_Ch);
    MFS_UARTDisableTX(UART52_Ch);
}

/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: click button -- download program
*           push  button -- refresh  flash
*******************************************************************************/
void IAP(void)
{
    uint8_t i = 0;
    
    while (1u)
    {
        if (0u == button_key)
        {
            delay_ms(50u);
            if (0u == button_key)
            {
                delay_ms(500u);    // C discharge time
                i++;
                if ((1u == button_key) && (i < 10u))
                {   
                    i = 0u;
                    button_key = CLICK;
                    //bFM3_GPIO_PDOR0_PD = ~bFM3_GPIO_PDIR0_PD; /* LED201 */
                    bFM3_GPIO_PDOR0_PD = 0;  /* LED 201 light */
                    delay_ms(500u);
                    IEC62056_21_Process();
                    download_program_to_meter();
                }
                else
                {
                    if ((0u == button_key) && (i >= 10))
                    {
                        i = 0u;
                        button_key = LONG;
                        //bFM3_GPIO_PDOR0_PC = ~bFM3_GPIO_PDIR0_PC;  /* LED 202 */
                        bFM3_GPIO_PDOR0_PC = 0; /* LED 202 light */
                        delay_ms(500u);
                        refresh_flash(); 
                    }       
                }    
            }/* end if (0u == button_key) */ 
        }    
        
        delay_ms(10);
    }    
}


