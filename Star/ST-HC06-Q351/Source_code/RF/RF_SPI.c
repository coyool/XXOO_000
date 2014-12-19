
/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* �ļ����ƣ�RF software sPI
*
* �ļ���ʶ��
* ժ    Ҫ��
*
* ��ǰ�汾��
* ��    �ߣ�F06553
* ������ڣ�2014-12-2
* ���뻷����D:\software\IAR_for_ARM\arm
* 
* Դ����˵����
*******************************************************************************/

#include "include.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/







/*******************************************************************************
* ��������:  SPI  SO_IN ���Ͳ���
* �������: 
* �������:  i        
* ����˵��: tsd �����ڵ�ַ�������ֽ�֮���Լ������ֽ�֮��
*           ��ʱ > 80 ns, 
*           CSN ����ʱ MCUҪ��
*           SOMI ���� ���ܽ��� data ����  delay 250 us
* ʹ����Դ: 
*******************************************************************************/
uint8_t TestSOMI(void)
{
    uint8_t i;
    //Delay100Us();
    NOP();
    NOP();
    for (i=10; i>0; i--)      //250 us
    {
        delayUs(100);
        if (SO_IN_L) 
        {
            break;
        }    
    } 
	
    return i;
}

/*******************************************************************************
* ��������: SPI д�Ĵ���   
* �������: addr �Ĵ�����ַ  value д���ֵ
* �������:        
* ����˵��: tsd �����ڵ�ַ�������ֽ�֮���Լ������ֽ�֮��
*           ��ʱ > 80 ns, 
*           CSN ����ʱ MCUҪ��
*           SOMI ���� ���ܽ��� data ����  delay 250 us
* ʹ����Դ: 
*******************************************************************************/
void SpiWriteReg(uint8_t addr, uint8_t value)
{
     CSN_OUT_L;              //CSN=0;
     if (TestSOMI())
     {    
         SPI_byte(addr);	     //д��ַ
         SPI_byte(value);        //д��data
     }
     else
     {
          RF.SOMI_false = 1;    // TestSOMI ����
     }    
     CSN_OUT_H;              //CSN = 1;         
}

/*******************************************************************************
* ��������: SPI ���Ĵ���   
* �������: addr ��ȡ�Ĵ�����ַ
* �������:        
* ����˵��: 
* ʹ����Դ: 
*******************************************************************************/
/*
uint8_t SpiReadReg(uint8_t addr)
{
    uint8_t temp = 0;
    uint8_t value = 0;

    temp = addr | READ_SINGLE;    //���Ĵ�������
    CSN_OUT_L;                    //CSN=0;
     if (TestSOMI())
    {    
        SPI_byte(temp);
        value = SPI_byte(0);
    } 
    else
    {
          RF.SOMI_false = 1;    // TestSOMI ����
          value = 0;
    }    
    CSN_OUT_H;                    //CSN=1
    
    return value;
}
*/
/**************************************************
* ��������: SPI  д����ֽ�  
* �������: addr �Ĵ�����ַ  *buff д��dataָ��   count д�볤��
* �������:        
* ����˵��: 
* ʹ����Դ: 
**************************************************/
void SpiWriteBurstReg(uint8_t addr, uint8_t *buffer, uint8_t count)
{
     uint8_t i = 0;
     uint8_t temp = 0;
  
     temp = addr | WRITE_BURST;
     CSN_OUT_L;                     //CSN=0;
     if (TestSOMI())
     {    
         SPI_byte(temp);                //addr 
         for (i=0; i<count; i++)
         {
             SPI_byte(buffer[i]);        
         }
     }
     else
     {
          RF.SOMI_false = 1;    // TestSOMI ����
     }    
     CSN_OUT_H;                     //CSN=1;
}

/**********************************************************
* ��������: SPI  ������ֽ�  
* �������: addr �Ĵ�����ַ   *buff ��ſռ�ָ��  count ��������
* �������:        
* ����˵��: 
* ʹ����Դ: 
**********************************************************/
void SpiReadBurstReg (uint8_t addr, uint8_t *buffer, uint8_t count)
{
    uint8_t i = 0;
    uint8_t temp = 0;
    
    temp = addr | READ_BURST;	        //д��Ҫ�������üĴ�����ַ�Ͷ�����
    CSN_OUT_L;                          //CSN=0;
    if (TestSOMI())
    {    
        SPI_byte(temp);   
        for (i=0; i<count; i++) 
        {
            buffer[i] = SPI_byte(0);
        }
    }
    else
    {
          RF.SOMI_false = 1;    // TestSOMI ����
    }    
    CSN_OUT_H;                          //CSN=1;
}

/**************************************************
* ��������: SPI д��������   
* �������: addr д����������ַ
* �������:        
* ����˵��: 
* ʹ����Դ: 
**************************************************/
void SpiWriteStrobe(uint8_t strobe)
{
    CSN_OUT_L;               //CSN=0;
    if (TestSOMI())
    {    
        SPI_byte(strobe);        //д������
    } 
    else
    {
          RF.SOMI_false = 1;    // TestSOMI ����
    }    
    CSN_OUT_H;               //CSN=1;
}

/**************************************************
* ��������: SPI ����������   
* �������: addr �������������ַ
* �������: ��� ״̬�Ĵ��� data        
* ����˵��: 
* ʹ����Դ: ���Ľ�
**************************************************/
uint8_t SpiReadStatus(uint8_t addr)
{
    uint8_t value = 0;
    uint8_t temp = 0;
  
    temp = addr | READ_BURST;	//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    CSN_OUT_L;                  //CSN=0;
    if (TestSOMI())
    {    
        SPI_byte(temp);
        value = SPI_byte(0);
    }
    else
    {
          RF.SOMI_false = 1;    // TestSOMI ����
          value = 0;
    }    
    CSN_OUT_H;                 //CSN=1;
  
    return value;
}

/**************************************************
* ��������: SPI ����ʱ�� 
* �������: SPI_byte(addr)  SPI_byte(data) ������   
* �������: SPI_byte(0)  ����data         
* ����˵��:  �����ض�дdata 
* ʹ����Դ: 
**************************************************/
uint8_t SPI_byte(uint8_t dat)
{
    uint8_t i;
    
    for (i=0; i<8; i++)
    {
        if (dat & 0x80)
        {    
            SI_OUT_H;      // SIMO = 1;  high level write data 
        }
		else
		{
		    SI_OUT_L;      // SIMO = 0;
		} 
        SCLK_OUT_L;         // SCK = 0;
        
        delayUs(5);        // ARM   
        
        dat = dat << 1;
        SCLK_OUT_H;         // SCK = 1; 
        if (SO_IN_H)       //1 
        {    
            dat++;         // high level read data 
        }    
        
    }//end for SPI SYN serial
    
    return dat;
}