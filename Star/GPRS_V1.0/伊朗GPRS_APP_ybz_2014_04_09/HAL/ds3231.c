#include "stm32f10x_gpio.h"
#include <stdio.h>

#ifdef SMALL_CONCENT
	#define PORT_DS3231_SDA          GPIOC
	#define PORT_DS3231_SCL          GPIOC
	#define PIN_NBR_DS3231_SDA    GPIO_Pin_2
	#define PIN_NBR_DS3231_SCL     GPIO_Pin_3
#else
	#define PORT_DS3231_SDA          GPIOB
	#define PORT_DS3231_SCL          GPIOB
	#define PIN_NBR_DS3231_SDA    GPIO_Pin_5
	#define PIN_NBR_DS3231_SCL     GPIO_Pin_6
#endif

#define SCL_H         (GPIO_SetBits(PORT_DS3231_SCL, PIN_NBR_DS3231_SCL)) 
#define SCL_L         (GPIO_ResetBits(PORT_DS3231_SCL, PIN_NBR_DS3231_SCL)) 
   
#define SDA_H         (GPIO_SetBits(PORT_DS3231_SDA, PIN_NBR_DS3231_SDA)) 
#define SDA_L         (GPIO_ResetBits(PORT_DS3231_SDA, PIN_NBR_DS3231_SDA))  

#define I2C_SDA_STATE ((PIN_NBR_DS3231_SDA->IDR&PIN_NBR_DS3231_SDA) != 0)

#define delay_5   200
#define delay_10   400
#define delay_100   4000

struct I2C_T
{ 
	GPIO_TypeDef* SdaPort;
	u16	SdaPin; 
	GPIO_TypeDef* SclPort;
	u16 SclPin;
};
void ds3231_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_NBR_DS3231_SDA;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_DS3231_SDA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = PIN_NBR_DS3231_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_DS3231_SCL, &GPIO_InitStructure);	
}

void delay1(int us)  
{  
//   unsigned int i=200; 
   unsigned int i=2000; 

   while(us--) 
   { 
           while(i) 
        { 
         i--; 
        } 
    
   } 
} 
void Start(void)  
{  
  SCL_L;  
  delay1(delay_5);  
  SDA_H;  
//=====================
  SCL_L;   
  delay1(delay_10);  
  SDA_H;  
  delay1(delay_10); 
  SCL_H;  
  delay1(delay_10); 
  SDA_L;  
  delay1(delay_10); 
//=====================
  SCL_L;   
  delay1(delay_5);  
  SDA_H;  
}
void Stop(void)  
{  
//  SCL_L;   
//  SDA_L;  
//===================
  SCL_L;   
  delay1(delay_10); 
  SDA_L;  
  delay1(delay_10); 
  SCL_H;  
  delay1(delay_10); 
  delay1(delay_10); 
  SDA_H;  
  delay1(delay_10); 
//===================
  SCL_L;   
} 
void SendByte(unsigned char Dat)  
{  
  unsigned char i=0;  
  unsigned char T_Data=0;  
  SCL_L;   
  delay1(delay_10);  
  T_Data=Dat;  

  for(i=0;i<8;i++)  
    {          
         if(T_Data&0x80)  
            SDA_H;  
          else   
            SDA_L;  
          delay1(delay_10);  
          SCL_L;  
          delay1(delay_10);  
          SCL_H;  
          delay1(delay_10);  
          T_Data=T_Data<<1;  
          SCL_L;  
          delay1(delay_10);  
   }  
  SDA_H;//�ͷ�����   
  delay1(delay_10);   
  SCL_L;     /*********************/  
  delay1(delay_10);  
  SCL_H;     /*********************/  
  delay1(delay_10); 
  SCL_L;  
}  
unsigned char ReceiveByte(unsigned char b)  
{  
  unsigned char i;  
  unsigned char Dat=0;  
  u8 temp;
  SCL_L;   
  delay1(delay_10);  
  for(i=0;i<8;i++)  
    {  
          SCL_H;  
          delay1(delay_5);  
          Dat=Dat<<1;  
          delay1(delay_5); 
          GPIO_SetBits(PORT_DS3231_SDA, PIN_NBR_DS3231_SDA);
		  temp=GPIO_ReadInputDataBit(PORT_DS3231_SDA, PIN_NBR_DS3231_SDA);
//          if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)  //������
          if(temp==1)    //GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==1)  //������
            Dat|=0x01;  
          else  
            Dat|=0x00;  
         delay1(delay_5);  
         SCL_L;   
         delay1(delay_5);   
    }  
    
//    ds3231_mode_out();
//  DDRD|=0x04;//����Ϊ��� 
  if(b)//ÿ��һ��������ACK��nACK  
    SDA_H;  
  else  
    SDA_L;  
          
  delay1(delay_5);  
  SCL_H;  
  delay1(delay_5);  
  SCL_L;  
  delay1(delay_5);  
  SDA_H;        //�ͷ�����  
  delay1(delay_100);
    
  return Dat;  
} 
 
void I2cByteWrite(unsigned char device,unsigned int addr,unsigned char bytedata)  
{  
  Start();  
  delay1(delay_5);  
  SendByte(device);  
  delay1(delay_5);  
  SendByte(addr);  
  delay1(delay_5);  
  SendByte(bytedata);  
  delay1(delay_5);  
  Stop();  
}    
unsigned char I2cByteRead(unsigned char device,unsigned int addr)  
{  
  unsigned char Dat=0;  
    
  Start();  
  SendByte(device);  
  delay1(delay_5);  
  SendByte(addr);  
  delay1(delay_5);  
  Start();//Restart  
  SendByte(0xd1);//����  
  delay1(delay_5);  
  Dat=ReceiveByte(1);//ֻ����һ���ֽ�,���Է�nACK  
  Stop();  
  return Dat;  
}



#define DS3231_WriteAddress 0xD0    //����д��ַ 
#define DS3231_ReadAddress  0xD1    //��������ַ

#define DS3231_SECOND       0x00    //��
#define DS3231_MINUTE       0x01    //��
#define DS3231_HOUR         0x02    //ʱ
#define DS3231_WEEK         0x03    //����
#define DS3231_DAY          0x04    //��
#define DS3231_MONTH        0x05    //��
#define DS3231_YEAR         0x06    //��

unsigned char B_BCD(unsigned char val)//B��ת��ΪBCD��  
{  
  unsigned char i,j,k;  
  i=val/10;  
  j=val%10;  
  k=j+(i<<4);  
  return k;  
} 

void ds3231_read_ymdwhms(unsigned char *str)
{
    *str++ = I2cByteRead(DS3231_WriteAddress,DS3231_YEAR);    
    *str++ = I2cByteRead(DS3231_WriteAddress,DS3231_MONTH);    
    *str++ = I2cByteRead(DS3231_WriteAddress,DS3231_DAY);    
	*str++ = I2cByteRead(DS3231_WriteAddress,DS3231_WEEK);    

	*str++ = I2cByteRead(DS3231_WriteAddress,DS3231_HOUR)&0x3f;    //ʱ 24Сʱ��                
	*str++ = I2cByteRead(DS3231_WriteAddress,DS3231_MINUTE);  //��
	*str++ = I2cByteRead(DS3231_WriteAddress,DS3231_SECOND);  //��
//	PrintLog(LOGTYPE_DEBUG,"\r\ntime:%02x-%02x-%02x %02x:%02x:%02x\r\n",Year,Month,Day,Hour,Min,Sec);
}
void ds3231_write_ymdwhms(const unsigned char *str)
{  
  
  I2cByteWrite(DS3231_WriteAddress,DS3231_YEAR,*str++);//�޸���  

  I2cByteWrite(DS3231_WriteAddress,DS3231_MONTH,*str++);//�޸���  

  I2cByteWrite(DS3231_WriteAddress,DS3231_DAY,*str++);//�޸���  
    
  I2cByteWrite(DS3231_WriteAddress,DS3231_WEEK,*str++);//�޸���

  I2cByteWrite(DS3231_WriteAddress,DS3231_HOUR,*str++);//�޸�ʱ  

  I2cByteWrite(DS3231_WriteAddress,DS3231_MINUTE,*str++);//�޸ķ�  
 
  I2cByteWrite(DS3231_WriteAddress,DS3231_SECOND,*str++);//�޸���  
}  

