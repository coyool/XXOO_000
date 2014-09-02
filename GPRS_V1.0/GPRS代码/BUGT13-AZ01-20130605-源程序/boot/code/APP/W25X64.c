#include "stm32f10x_spi.h"
#include <string.h>
#include "sys_config.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

u8 SPI_FLASH_SendByte(u8 byte);	  
void SPI_FLASH_WaitForWriteEnd(void);
#define WriteEnable               0x06       //???,???????
#define WriteDisable              0x04       //???
#define ReadStatusRegister        0x05       //??????
#define WriteStatusRegister       0x01       //??????
#define Read_Data                 0x03       //???????
#define FastReadData              0x0B       //?????????
#define FastReadDualOutput        0x3B       //????????????????
#define Page_Program              0x02      

#define BlockErace                0xD8       //???
#define SectorErace               0x20       //????
#define ChipErace                 0xC7       //???
#define Power_Down                0xB9       //????

#define ReleacePowerDown          0xAB       //??????
#define ReadDeviceID              0xAB       //????ID??


#define ReadDeviceID              0xAB       //?????????ID??
#define ReadManuIDDeviceID        0x90       //??????ID?????ID??
#define ReadJedec_ID              0x9F       //JEDEC?ID??

#define WIP_Flag   0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                0xA5

#ifdef SMALL_CONCENT 
#define PC_CS_FLASH	GPIO_Pin_4	   //SPI1-CSn
#define GPIO_CS_FLASH	GPIOC
#endif
#ifdef BIG_CONCENT 
#define PC_CS_FLASH	GPIO_Pin_7	   //SPI1-CSn
#define GPIO_CS_FLASH	GPIOB
#endif
#ifdef METER_MODULE
#define PC_CS_FLASH	GPIO_Pin_4	   //SPI1-CSn
#define GPIO_CS_FLASH	GPIOC
#endif

#define W25X64_SECTOR_SIZE	4096
#define W25X64_SECTORS		2048
#define W25X64_SIZE			(W25X64_SECTOR_SIZE*W25X64_SECTORS)

#define SPI_FLASH_CS_LOW()        GPIO_ResetBits(GPIO_CS_FLASH, PC_CS_FLASH)
#define SPI_FLASH_CS_HIGH()        GPIO_SetBits(GPIO_CS_FLASH, PC_CS_FLASH)
/*
spi flash 设备初始化
*/
void w25x64_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
						 RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  PC_CS_FLASH;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_CS_FLASH, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  

	SPI_FLASH_CS_HIGH();

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //双工模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; //CLK上升沿采样，因为上升沿是第二个边沿动作，所以也可以理解为第二个边沿采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  ////SPI频率
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;	  //crc7，stm32spi带硬件ecc
	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE);

	SPI_FLASH_CS_HIGH();
}
static void w25x64_write_enble(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(WriteEnable);
  SPI_FLASH_CS_HIGH();
}
/*static void w25x64_write_disable(void)
{
  SPI_FLASH_CS_LOW();
  SPI_FLASH_SendByte(WriteDisable);
  SPI_FLASH_CS_HIGH();
}*/
void w25x64_read(unsigned int addr,void *pbuf,unsigned int bytes)
{
	u8* data_buf = pbuf;
	if(addr+bytes>W25X64_SIZE)
		return;
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(Read_Data);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((addr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((addr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(addr & 0xFF);

  while (bytes--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *data_buf = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    data_buf++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}
void w25x64_sector_erase(unsigned int addr)
{
	if(addr>=W25X64_SECTORS)
		return;
	addr *= W25X64_SECTOR_SIZE;
  /* Send write enable instruction */
  w25x64_write_enble();

  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Sector Erase instruction */
  SPI_FLASH_SendByte(SectorErace);
  /* Send SectorAddr high nibble address byte */
  SPI_FLASH_SendByte((addr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SPI_FLASH_SendByte((addr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SPI_FLASH_SendByte(addr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(void)
{
  /* Send write enable instruction */
  w25x64_write_enble();

  /* Bulk Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Bulk Erase instruction  */
  SPI_FLASH_SendByte(ChipErace);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}
void w25x64_program(unsigned int addr,const void *pbuf,unsigned int bytes)
{
	const u8* data_buf;
	if(addr+bytes>W25X64_SIZE)
		return;
	data_buf = pbuf;
  while(1)
  {
	  /* Enable the write access to the FLASH */
	  w25x64_write_enble();

	  /* Select the FLASH: Chip Select low */
	  SPI_FLASH_CS_LOW();
	  /* Send "Write to Memory " instruction */
	  SPI_FLASH_SendByte(Page_Program);
	  /* Send WriteAddr high nibble address byte to write to */
	  SPI_FLASH_SendByte((addr & 0xFF0000) >> 16);
	  /* Send WriteAddr medium nibble address byte to write to */
	  SPI_FLASH_SendByte((addr & 0xFF00) >> 8);
	  /* Send WriteAddr low nibble address byte to write to */
	  SPI_FLASH_SendByte(addr & 0xFF);

	  /* while there is data to be written on the FLASH */
	  while (bytes--)
	  {
		/* Send the current byte */
		SPI_FLASH_SendByte(*data_buf);
		/* Point on the next byte to be written */
		data_buf++;
		addr++;
		//256 Page
		if(0 == ((unsigned int)addr&0x000000ff))
		{
			break;
		}
	  }

	  /* Deselect the FLASH: Chip Select high */
	  SPI_FLASH_CS_HIGH();

	  /* Wait the end of Flash writing */
	  SPI_FLASH_WaitForWriteEnd();

	  //finished 
	  if(bytes == (unsigned int)-1)
		break;
  }
}
/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
unsigned char SPI_FLASH_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read Status Register" instruction */
  SPI_FLASH_SendByte(ReadStatusRegister);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

