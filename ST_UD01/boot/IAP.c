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
void refresh_flash(void);
static void download_flash_to_meter(void);
static void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);
static void Jump_To_app(void);

/*** static variable declarations ***/
uint8_t tab_1024[1024] = {0};
//uint8_t file_name[FILE_NAME_LENGTH];

/*** extern variable declarations ***/
pFunction Jump_To_Application;
__IO uint32_t JumpAddress;







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
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{
    //
    //
    SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

/*******************************************************************************
* 函数名称: PC jump function
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void Jump_To_app(void)
{
    __disable_irq();//关闭总中断
    JumpAddress = *(volatile uint32_t*) (VectTab_address + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) VectTab_address);                 /* set SP_main */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTab_address);     /* 配置中断向量表偏移量寄存器 */
    Jump_To_Application();
}

/*******************************************************************************
* 函数名称: refresh_flash
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void refresh_flash(void)
{
    uint8_t Number[10] = "          ";
    int32_t Size = 0;

    //SerialPutString("Waiting for the file to be sent ...\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);
    if (Size > 0)
    {
        //SerialPutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
        //SerialPutString(file_name);
    //    Int2Str(Number, Size);
        //SerialPutString("\n\r Size: ");
        //SerialPutString(Number);
        //SerialPutString(" Bytes\r\n");
        //SerialPutString("-------------------\n");
        
        /* updata image complete, jump application routine */
     //   Jump_To_app();  
        while (1);  
    }
    else if (Size == -1)
    {
        //SerialPutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
    }
    else if (Size == -2)
    {
        //SerialPutString("\n\n\rVerification failed!\n\r");
    }
    else if (Size == -3)
    {
        //SerialPutString("\r\n\nAborted by user.\n\r");
    }
    else
    {
        //SerialPutString("\n\rFailed to receive the file!\n\r");
    }
}

/*******************************************************************************
* 函数名称: download_flash_to_meter
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void download_flash_to_meter(void)
{
    uint32_t status = 0u; 
    uint8_t tick = 0u;
    uint8_t Rev_flag = 0u;

    MFS_UARTEnableRX(InUseCh);
    MFS_UARTEnableTX(InUseCh);
    
    //SerialPutString("\n\n\rSelect Receive File ... \n\r");
    Rev_flag = Receive_Byte(&tick, Rev_timeout); 
    if (0u == Rev_flag)
    {
        _NOP();
    }    
    else
    {
        return;
    }    
    
    if (tick == CRC16)
    {
        /* Transmit the flash image through ymodem protocol */
        status = Ymodem_Transmit((uint8_t*)ApplicationAddress, (const uint8_t*)"DownloadFlashImage.bin", FLASH_IMAGE_SIZE);
        MFS_UARTDisableRX(InUseCh);
        
        if (status != 0) 
        {
            //SerialPutString("\n\rError Occured while Transmitting File\n\r");
            //buzzer(); /* BUZZER 201 */
        }
        else
        {
            //SerialPutString("\n\rFile Trasmitted Successfully \n\r");
        }
    }
    else
    {
        //SerialPutString("\r\n\nAborted by user.\n\r");  
    }
    
    //MFS_UARTDisableRX(InUseCh);
    MFS_UARTDisableTX(InUseCh);
}

/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void IAP(void)
{
 
    while (1u)
    {
        if (0u == button_key)
        {
            delay_ms(50);
            if (0u == button_key)
            {
                delay_ms(500u);
                if (1u == button_key)
                {
                    button_key = CLICK;
                    bFM3_GPIO_PDOR0_PD = ~bFM3_GPIO_PDIR0_PD; /* LED201 */
                    download_flash_to_meter();
                }
                else
                {
                    delay_ms(10000u);
                    if (0u == button_key)
                    {
                        button_key = LONG;
                        bFM3_GPIO_PDOR0_PC = ~bFM3_GPIO_PDIR0_PC;  /* LED 202 */
                        refresh_flash(); 
                    }       
                }    
            }/* end if (0u == button_key) */ 
        }    
        
        delay_ms(10);
    }    
}


