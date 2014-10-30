
//#include "delay.h"
#include "hal_include/mx25l64.h"

#define PORT_MX25L64_CS		GPIOB
#define PIN_NBR_MX25L64_CS	      GPIO_Pin_6
#define FLASH_SPI_SCK_PIN          GPIO_Pin_3
#define FLASH_SPI_MOSI_PIN        GPIO_Pin_5
#define FLASH_SPI_MISO_PIN        GPIO_Pin_4

#define FLASH_SPI_WP_PIN           GPIOD    
#define PIN_NBR_SPI_WP               GPIO_Pin_7
//static inline void mx25l64_cs_set(void);
//static inline void mx25l64_cs_clr(void);

void sFLASH_LowLevel_Init(void)
{
#if 0
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  
  /*!< Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /*!< SPI pins configuration *************************************************/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    /*!< SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< SPI MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  FLASH_SPI_MOSI_PIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  FLASH_SPI_MISO_PIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /*!< Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);

  /*!< Configure sFLASH Card CS pin in output pushpull mode ********************/
  GPIO_InitStructure.GPIO_Pin = PIN_NBR_MX25L64_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(PORT_MX25L64_CS, &GPIO_InitStructure);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = PIN_NBR_SPI_WP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(FLASH_SPI_WP_PIN, &GPIO_InitStructure);
    GPIO_ResetBits(FLASH_SPI_WP_PIN,PIN_NBR_SPI_WP);
#endif
}

void ssp3_init(void)
{	
#if 0
     SPI_InitTypeDef  SPI_InitStructure;
     int_dis(INT_SRC_NBR_SPI3);
 
     sFLASH_LowLevel_Init();
     /*!< Deselect the FLASH: Chip Select high */
     mx25l64_cs_set();
     /*!< SPI configuration */
     SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
     SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
     SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
     SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
     SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
     SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
     SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
     SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
     SPI_InitStructure.SPI_CRCPolynomial = 7;
     SPI_Init(SPI3, &SPI_InitStructure);
     /*!< Enable the sFLASH_SPI  */
     SPI_Cmd(SPI3, ENABLE);
#endif
}

#define MX25L64_CMD_RDSR	0x05
#define MX25L64_CMD_WREN	0x06
#define MX25L64_CMD_READ	0x03
#define MX25L64_CMD_SE		0x20
#define MX25L64_CMD_PP		0x02
/*

void mx25l64_spi_delay(void)
{
     //while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
//     delay_us(2);
}

static inline void mx25l64_cs_clr(void)
{
    // GPIO_ResetBits(PORT_MX25L64_CS,PIN_NBR_MX25L64_CS);
}

static inline void mx25l64_cs_set(void)
{
    // GPIO_SetBits(PORT_MX25L64_CS,PIN_NBR_MX25L64_CS);
}
 */

/**
 * �Ƿ�MX25L6445���ڿ���״̬����д��
 * @return 1 ���� 0 �ǿ���
 */
 /*
static unsigned int mx25l64_idle(void)
{	
#if 0
	unsigned int data;
	unsigned int i=0;

	while(i<400){	//200���룮��������������60ms
		mx25l64_cs_clr();
		mx25l64_spi_delay();
		SPI3->DR = MX25L64_CMD_RDSR;
		mx25l64_spi_delay();
		SPI3->DR;	//����RX FIFO
		SPI3->DR = 0xff;	//����ʱ��
		mx25l64_spi_delay();
		data = SPI3->DR;
		mx25l64_cs_set();
		if(0==(data&0x01))	//���ڽ���д����(��д)
			return 1;
		i++;
		delay_us(500);
	}
	ssp3_init();
	delay_us(1000);
#endif
	return 0;
}
*/
/**
 * MX25L6445дʹ��
 * MX25L6445��������������³���д
 * �ϵ�/WRDIָ�����/WRSRָ�����/ҳд���/���������/���/оƬ��/����д
 * /�������ͽ���/Ƭ�����ͽ���
 */
unsigned int mx25l64_write_enble(void)
{
#if 0
	if(0==mx25l64_idle())
		return 0;
	mx25l64_cs_clr();
	SPI3->DR = MX25L64_CMD_WREN;
	mx25l64_spi_delay();
	SPI3->DR;
	mx25l64_cs_set();
	mx25l64_spi_delay();
#endif
	return 1;
}


/**
 * ������
 * @addr:��ַ
 * @buf:����
 * @bytes:�ֽ���
 * @return ʵ�ʶ�ȡ�ֽ���
 */
unsigned int mx25l64_read(unsigned int addr,void *pbuf,unsigned int bytes)
{
#if 0
	unsigned int i;
	unsigned char *buf = pbuf;
	if(addr+bytes>MX25L64_SIZE)
		return 0;
	if(0==mx25l64_idle())
		return 0;

	mx25l64_cs_clr();
	mx25l64_spi_delay();
	
	SPI3->DR = MX25L64_CMD_READ;
	mx25l64_spi_delay();
	SPI3->DR;
	
	SPI3->DR = (unsigned char)(addr>>16);
	mx25l64_spi_delay();
	SPI3->DR;	
	
	SPI3->DR = (unsigned char)(addr>>8);
	mx25l64_spi_delay();
	SPI3->DR;
	
	SPI3->DR = (unsigned char)addr;
	mx25l64_spi_delay();
	SPI3->DR;
	
	for(i=0;i<bytes;i++){
		SPI3->DR = 0xff;
		mx25l64_spi_delay();
		*buf = SPI3->DR;
		buf++;
	}
	
	mx25l64_cs_set();
	mx25l64_spi_delay();
	return i;
#endif
	return 0;
}

/**
 * ��������
 *@addr ��������������
 *@return 1 �ɹ� 0 �ɹ�
 */
unsigned int mx25l64_sector_erase(unsigned int addr)
{
#if 0
	if(addr>=MX25L64_SECTORS)
		return 0;
	if(0==mx25l64_write_enble())
		return 0;

	addr *= MX25L64_SECTOR_SIZE;
	mx25l64_cs_clr();
	mx25l64_spi_delay();
	
	SPI3->DR = MX25L64_CMD_SE;
	mx25l64_spi_delay();
	SPI3->DR;
	
	SPI3->DR= (unsigned char)(addr>>16);
	mx25l64_spi_delay();
	SPI3->DR;
	
	SPI3->DR = (unsigned char)(addr>>8);
	mx25l64_spi_delay();
	SPI3->DR;
	
	SPI3->DR = (unsigned char)(addr);
	mx25l64_spi_delay();
	SPI3->DR;
	
	mx25l64_cs_set();
	mx25l64_spi_delay();
#endif
	return 1;
}

/**
 * ����д��ÿ������K�ֽ�,ÿ��ֻ��������Ϊ��λдһ��������������������ַд��������Ҫ4096�ֽڿռ��RAM��
 * ����һ��д����ʱ����Ϊ�����뵽�ļ�β�����Ի�����Ҫ�����ݶ���������Ҫ��һ�����塣ÿ��д�ļ�����Ҫ����
 * ����sector,���ϲ�������ƿس�
 * @sector :��д������ 0~2047
 * @buf:	������
 * @bytes:  ����
 * @return  д�ɹ��������ֽ���
 */
unsigned int mx25l64_program(unsigned int addr,const void *pbuf,unsigned int bytes)
{
#if 0
	if((addr+bytes)>MX25L64_SIZE)
		return 0;
/*�ĳɲ�����
	unsigned int addr;
	addr = sector*MX25L64_SECTOR_SIZE;
	if(mx25l64_sector_erase(addr)==0)
		return 0;
*/
	unsigned int i;
	const unsigned char *buf=pbuf;
	i=0;
	while(i<bytes){
		if(mx25l64_write_enble()==0)
			return i;
		mx25l64_cs_clr();
		mx25l64_spi_delay();
		
		SPI3->DR = MX25L64_CMD_PP;
		mx25l64_spi_delay();
		SPI3->DR;
		
		SPI3->DR = (unsigned char)(addr>>16);
		mx25l64_spi_delay();
		SPI3->DR;
		
		SPI3->DR = (unsigned char)(addr>>8);
		mx25l64_spi_delay();
		SPI3->DR;
		
		SPI3->DR = (unsigned char)(addr);
		mx25l64_spi_delay();
		SPI3->DR;
		
		//д����
		while(i<bytes){
			SPI3->DR = *buf++;
			mx25l64_spi_delay();
			SPI3->DR;
			i++;
			addr++;
			if(0==(addr&0x000000ff)){	//ÿҳ256�ֽ�,�����ҳд�꣬���¶����ַ
				mx25l64_cs_set();	//���ߣ���ʼ�ڲ�д
				mx25l64_spi_delay();
				break;
			}
		}
	}
	mx25l64_cs_set();
	mx25l64_spi_delay();
	return i;
#endif
	return 0;
}

 //char Tx_Buffer[] = "STM32F20x SPI Firmware Library Example: communication with an M25L SPI FLASH";
 //char Rx_Buffer[512];
void test()
{
 
  //mx25l64_program(0x700000, Tx_Buffer, 40);

  /* Read data from SPI FLASH memory */
 // mx25l64_read(0x700000, Rx_Buffer, 40);
  
}
