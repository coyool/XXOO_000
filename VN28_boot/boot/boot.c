/*******************************************************************************
* Copyright 2014      ����˼���Ǳ�  �̼���  
* All right reserved
*
* �ļ����ƣ�bootloader
*
* �ļ���ʶ��boot.c
* ժ    Ҫ��boot loop
*
* ��ǰ�汾��V1.0
* ��    �ߣ�F06553
* ������ڣ�2014-7-1
* ���뻷����IAR_for_ARM 6.50
* 
* Դ����˵���� 
*   warning : 1. Ymodem only support 128 byte frame    
*             2. ban use interrupt in boot
*           
*                 
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
static uint8_t tab_128[PACKET_SIZE] = {0};

static int32_t   Size = 0;
static uint8_t   Number[10] = "          ";
static uint16_t  updata = 0;
static uint16_t  updata_OK = 0;

/* UART config */
static MFS_UARTModeConfigT tUARTModeConfigT =
{
    57600,                   /* 4800 or 19200  57600 */
    UART_DATABITS_8,        
    UART_STOPBITS_1,        
    UART_PARITY_NONE,       
    UART_BITORDER_LSB,
    UART_NRZ,               /* level inversion */  
};

/*** extern variable declarations ***/
typedef void (*pFunction)(void);        /* ���庯��ָ�� */
pFunction Jump_To_Application;
uint32_t  JumpAddress;                    /* ��ת��ַ */



/*** boot Port ***/
/*******************************************************************************
* ��������: delay 
* �������: 
* �������: 
* --����ֵ: 
* ��������: --
*******************************************************************************/
void Delay_ms(uint32_t Cnt)
{
    uint32_t i;
    
    for (; Cnt ; Cnt--)
    {    
        for (i=SystemCoreClock/4000; i; i--);
    }    
}

/*******************************************************************************
* ��������: 
* �������: NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
*          This parameter can be one of the following values:
*       1: NVIC_VectTab_RAM
*       2: NVIC_VectTab_FLASH
*  Offset: Vector Table base offset field. This value must be a multiple of 0x100.
* �������: 
* --����ֵ: None
* ��������: Sets the vector table location and Offset.
* warning:  SP�����λ��Զ��0������ζ�Ŷ�ջ����4�ֽڶ���  
*******************************************************************************/
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{
    //
    //
    SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

/*******************************************************************************
* ��������: PC jump function
* �������: 
* �������: 
* --����ֵ: 
* ��������: --
*******************************************************************************/
void Jump_To_app(void)
{
    __disable_irq();//�ر����ж�
    JumpAddress = *(volatile uint32_t*) (VectTab_address + 4);
    Jump_To_Application = (pFunction) JumpAddress;
    /* Initialize user application's Stack Pointer */
    __set_MSP(*(__IO uint32_t*) VectTab_address);                 /* set SP_main */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, VectTab_address);     /* �����ж�������ƫ�����Ĵ��� */
    Jump_To_Application();
}

/*******************************************************************************
* ��������: 
* �������:  read Flash address
* �������:  
* --����ֵ:  read appoint location of data
* ��������:  read IAP flag
*******************************************************************************/
uint16_t MFlash_Read16bit(uint16_t* pReadAddr)
{    
    uint8_t  *Addr;
    uint16_t  ReadData;  
    Addr = (uint8_t *)pReadAddr;
    
    if ((Addr>=(uint8_t *)FLASH_STR_ADDR)&&(Addr<=(uint8_t *)(FLASH_END_ADDR-2)))
    {
        if (((uint32_t)Addr%2) == 1)
        {
            Addr--;
            ReadData = (uint16_t)(Flash_Read(Addr));
        }
        else
        {
            ReadData = (uint16_t)(Flash_Read(Addr));
        } 
        return ReadData;
    }

    return 0u;

}


#define  TEST

/*******************************************************************************
* ��������: boot
* �������: null
* �������: printf information
* --����ֵ: null
* ��������: -- IAP  Ymodem 
* warning:  updata -- updata ing must in Flash high address (0x0001FF04)
*           updata_OK -- updata compelet must in Flash low address (0x00001004)
*******************************************************************************/
void boot(void)
{
    uint8_t j; 
    
    //SysTick_Config(SystemCoreClock/20); 
    __disable_irq();//�ر����ж�
    
#ifdef  TEST    
    LED_IO_init(); 
    bFM3_GPIO_PDOR5_P5 = 1;     /* LED off */     
#endif
    
    /* read appointed address value 16 bit  */ 
    /* 0x80(128 byte) if frame isn't 128 byte compensation 0x1A*/
    updata = MFlash_Read16bit ((uint16_t*)0x0001FF84);    
    updata_OK = MFlash_Read16bit ((uint16_t*)0x0001FFA4);  

#ifdef  TEST    
    bFM3_GPIO_PDOR5_P5 = 0;   /* LED light */
#endif
    
//    Jump_To_app(); 
//    while (1);
    
    /* updata_OK == 0  updata == 0xFFFF stand for updata OK */
	if ( ((0xFFFF==updata) && (0u==updata_OK)) )  
	{
#ifdef  TEST
        bFM3_GPIO_PDOR5_P5 = 0;    /* LED4 light */
        Delay_ms(1000u); //test
        bFM3_GPIO_PDOR5_P5 = 1; 
        Delay_ms(1000u); //test
#endif          
        
        Jump_To_app();  /* updata image complete, jump application routine */
        
	}
	else  /* updata is not complete, need to updta image */
    {
        UART_Port_init();                        /* UART IO init */             
        UARTConfigMode(&tUARTModeConfigT);       /* UART setup */   
        UARTPollTX_string("\n\n\rbootloader begin������\r\n");
        
        while (1)
		{
			//UARTPollTX_string("Waiting for the file to be sent ... (press 'C' to abort)\r\n");
            UARTPollTX_string("1...\r\n");
            
            MFS_UARTEnableRX(InUseCh);
            MFS_UARTEnableTX(InUseCh);
            
            Size = Ymodem_Receive(&tab_128[0u]);
			if (Size > 0)
			{
				UARTPollTX_string("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
				UARTPollTX_string(FileName);
				Int2Str(Number, Size);
				UARTPollTX_string("\n\r Size: ");
				UARTPollTX_string(Number);
				UARTPollTX_string(" Bytes\r\n");
				UARTPollTX_string("--------------------------------\n");
                 
                //write appointed address value  updata OK flag
                MFlash_Write((uint16_t*) 0x0001FFA4,(uint32_t) 0x00000000);  /* ����ǿ��ת�� д 32bit */

                
				NVIC_SystemReset();  /* system reset, also soft reset */
				while(1);            
			}
			else if (Size == -1)
			{
				UARTPollTX_string("\n\n\rThe image size is higher than the allowed space memory!\n\r");
                break;
			}
			else if (Size == -2)
			{
				UARTPollTX_string("\n\n\rVerification failed!\n\r");
                break;
			}
			else if (Size == -3)
			{
				UARTPollTX_string("\r\n\nAborted by user.\n\r");
                break;
			}
			else    /* SecureCRT  Ctrl + C  CAN , Size = 0 */
			{
				UARTPollTX_string("\n\rFailed to receive the file!\n\r");
                break;
			}
		}//end while
        
    }//end if 
    
#ifdef  TEST  
    for (j=0; j<5; j++)
    {
        bFM3_GPIO_PDOR5_P5 = 0;    /* LED4 light */
        Delay_ms(1000u); //test
        bFM3_GPIO_PDOR5_P5 = 1; 
        Delay_ms(1000u); //test
    }    
    
#endif    
    
    NVIC_SystemReset();          /* system reset, also soft reset */  /*  */
    
    while(1);                   /* routine fatal error */
}


