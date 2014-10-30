/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：updata module
*
* 文件标识：main.c
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-8-15
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/





#include    "all_header_file.h"

/*** static function prototype declarations ***/
static void UART_setup(void);
static void BSP(void);

/*** static variable declarations ***/
// UART config 
//static MFS_UARTModeConfigT tUARTModeConfigT =
//{
//    57600,                   /* 4800 or 19200  57600 */
//    UART_DATABITS_8,        
//    UART_STOPBITS_1,        
//    UART_PARITY_NONE,       
//    UART_BITORDER_LSB,
//    UART_NRZ,               /* level inversion */  
//};


/*** extern variable declarations ***/



/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void ST_UD_status(void)
{
    uint8_t i;
    uint8_t tab_VectTab_address[8] = {0};
    char file_name_temp[FILE_NAME_LENGTH] = {0};
    char file_size_temp[FILE_SIZE_LENGTH] = {0};

    /* power on LED201 LED202 Blink */
    for (i=0u; i<10u; i++)
    {
        bFM3_GPIO_PDOR0_PC = 0u;     /* LED202 on */   
        bFM3_GPIO_PDOR0_PD = 0u;     /* LED201 on */      
        delay_ms(100u);
        bFM3_GPIO_PDOR0_PC = 1u;     /* LED202 off */
        bFM3_GPIO_PDOR0_PD = 1u;     /* LED201 off */        
        delay_ms(100u);
    } 
    
    /* power on BUZZER buzz */
    oneSound(20,0); 
    
    /* enable peripheral fuction */
    MFS_UARTEnableTX(UartUSBCh);
    //MFS_UARTEnableRX(UartUSBCh);
    
/*------ printf versions informations ----------------------------------------*/     
    // ...
    MX25L3206_Read((uint8_t *)tab_VectTab_address, 
                   (VectTab_address - USER_FLASH_START_ADDRESS),
                   8u);
    MX25L3206_Read((uint8_t*)file_name_temp,
                   (uint32_t)VERSION_ADDRESS,
                   FILE_NAME_LENGTH);
    MX25L3206_Read((uint8_t*)file_size_temp,
                   (uint32_t)FLASH_IMAGE_SIZE_ADDRESS,
                   FILE_SIZE_LENGTH);
    
    printf("\r\n");   /* space */
    printf("ST_UD01(IAP) V1.0.0 20140902 \r\n");
    printf("click button: download program to meter \r\n");
    printf("push  button: refresh flash of ST_UD01 \r\n");
    printf("\r\n");
    printf("confirm the following information: \r\n");
    printf("(1) Version: ");
    printf(file_name_temp);
    printf("\r\n");
    printf("(2) File size: ");
    printf(file_size_temp);
    printf("  byte");
    printf("\r\n");
    printf("(3) VectTab_Address: (SP) (PC) ");
    printHex(tab_VectTab_address,8u);
    printf("\r\n");
    //Flash no have image ！！！
    //printf();
}

/*******************************************************************************
* 函数名称: BSP
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
static void BSP(void)
{   
    BUTTON_KEY_setup();
    LED_setup();
    BUZZ_setup();
    //UART_setup();
    UART_Port_init();                                /* UART IO init */             
    //UARTConfigMode(UART52_Ch, &tUARTModeConfigT);    /* UART setup */  
    UARTConfigMode(UartUSBCh, &tUARTModeConfigT);
    
    uint8_t i;
    uint8_t tab_VectTab_address[8] = {0};
    char file_name_temp[FILE_NAME_LENGTH] = {0};
    char file_size_temp[FILE_SIZE_LENGTH] = {0};

    /* power on LED201 LED202 Blink */
    for (i=0u; i<10u; i++)
    {
        bFM3_GPIO_PDOR0_PC = 0u;     /* LED202 on */   
        bFM3_GPIO_PDOR0_PD = 0u;     /* LED201 on */      
        delay_ms(100u);
        bFM3_GPIO_PDOR0_PC = 1u;     /* LED202 off */
        bFM3_GPIO_PDOR0_PD = 1u;     /* LED201 off */        
        delay_ms(100u);
    } 
    
    /* power on BUZZER buzz */
    oneSound(20,0); 
    
    /* enable peripheral fuction */
    MFS_UARTEnableTX(UartUSBCh);
    MFS_UARTEnableRX(UartUSBCh);
    
/*------ printf versions informations ----------------------------------------*/     
    // ...
    MX25L3206_Read((uint8_t *)tab_VectTab_address, 
                   (VectTab_address - USER_FLASH_START_ADDRESS),
                   8u);
    MX25L3206_Read((uint8_t*)file_name_temp,
                   (uint32_t)VERSION_ADDRESS,
                   FILE_NAME_LENGTH);
    MX25L3206_Read((uint8_t*)file_size_temp,
                   (uint32_t)FLASH_IMAGE_SIZE_ADDRESS,
                   FILE_SIZE_LENGTH);
    
    printf("\r\n");   /* space */
    printf("ST_UD01(IAP) V1.0.0 20140902 \r\n");
    printf("click button: download program to meter \r\n");
    printf("push  button: refresh flash of ST_UD01 \r\n");
    printf("\r\n");
    printf("confirm the following information: \r\n");
    printf("(1) Version: ");
    printf(file_name_temp);
    printf("\r\n");
    printf("(2) File size: ");
    printf(file_size_temp);
    printf("  byte");
    printf("\r\n");
    printf("(3) VectTab_Address: (SP) (PC) ");
    printHex(tab_VectTab_address, 8u);
    printf("\r\n");
    //Flash no have image 
    //printf();
    
/*----------------------------------------------------------------------------*/
    
//#define  TEST_1    
#ifdef   TEST_1     
    printf("helloworld!\r\n");
    
    uint8_t tab_01[5] = {0xFF,0x01,0x02,0x03,0x04};
    printHex(tab_01,5);
    
    uint8_t temp;
    printf("start\r\n");
    Receive_Byte(UART52_Ch , &temp, Rev_timeout); 
    printf("end\r\n");
     
    //test_flash();
#endif        

}

/*******************************************************************************
* 函数名称: main
* 输入参数: 
* 输出参数: 
* --返回值: 0
* 函数功能: --
*******************************************************************************/
int main(void)
{
    BSP();
    IAP();    

   return 0u;
}


