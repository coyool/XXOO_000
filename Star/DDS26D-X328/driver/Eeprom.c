/*****************************************************************************************
*  	Function:  
*   
*    Author:	冯秋雄
*    Created on:	2013-09-12
					2013-10-31 添加读EEPROM检测功能
*****************************************************************************************/
#include "Include.h"

#ifndef my_memcmp
#define my_memcmp 	memcmp
#endif

/*****************************************************************************************
*   
*   Function :EEPROM_AddrSend,
*   Action   : 发送EE需要操作的地址
*	Input    : addr EE地址
*	Output   : None
*	Return   : TRUE--success  FALSE--fail 
*****************************************************************************************/
uint8 EEPROM_AddrSend(uint8 dev, uint32 addr)
{
	#if EEPROMTYPE <= 16
	if(!IIC_send400k(dev | ((unsigned char)(addr / 0x100) << 1))) return FALSE;    
    if(!IIC_send400k((unsigned char)addr)) return FALSE;
	#elif EEPROMTYPE <= 512	
	if(!IIC_send400k(dev)) return FALSE;
    if(!IIC_send400k((unsigned char)(addr >> 8))) return FALSE;
    if(!IIC_send400k((unsigned char)addr)) return FALSE;
	#else
   	if(!IIC_send400k(dev | ((unsigned char)(addr >> 16) << 1))) return FALSE;
    if(!IIC_send400k((unsigned char)(addr >> 8))) return FALSE;
    if(!IIC_send400k((unsigned char)addr)) return FALSE;
	#endif

	return TRUE;
}


/*****************************************************************************************
*   read_IIC_bytes
*   Function : Read data from dev at addr
*   Action   : 
*	Input    : addr--the address which start to read, buffer--the pointer
to buffer used to store data
               len-- the length of data needed to read, dev-- read from which 
               device
*	Output   : None
*	Return   : 1--success  0--fail 
*****************************************************************************************/
uint8 read_IIC_bytes(void *buffer, uint8 dev, uint16 addr, uint16 lenth)//(uint16 addr, void *buffer, uint16 lenth, uint8 dev)
{
	uint8 *p ;
	uint16 cTemp1,cTemp0;
	
	for(cTemp1 = EE_TryTimes; cTemp1 != 0; cTemp1--)
	{
		if(cTemp1)
		{
			if (detect_sda() == FALSE)
			{
				continue;
			}
		}

		IIC_start();
		if(!EEPROM_AddrSend(dev,addr)) 
		{
			continue;
		}

		p = (uint8 *)buffer;
		IIC_start();
#if EEPROMTYPE <= 16
		if(!IIC_send400k(dev | 0x01 | ((unsigned char)(addr / 0x100) << 1))) 
		{
			continue;
		}
#elif EEPROMTYPE <= 512
		if(!IIC_send400k(dev | 0x01)) 
		{
			continue;
		}
#else
		if(!IIC_send400k(dev | 0x01 | ((unsigned char)(addr >> 16) << 1))) 
		{
			continue;
		}
#endif
		for(cTemp0 = lenth-1; cTemp0 != 0; cTemp0--)
		{
			*p = IIC_receive400k();
			p++;
			SET_IIC_SDA_OUTPUT(); // ack
			CLR_IIC_SDA();
			SET_IIC_SCL();
			Dly1us();

			CLR_IIC_SCL();
			SET_IIC_SDA();
		}
		*p = IIC_receive400k();

		SET_IIC_SDA_OUTPUT();
		SET_IIC_SDA();
		SET_IIC_SCL();//收完最后一个字节发送NO ACK信号
		Dly1us();

		CLR_IIC_SCL();
		IIC_stop();

		return TRUE;
	}
	IIC_stop();

	return FALSE;
}

/*****************************************************************************************
*   ReadEepromBytes
*   Function : Read data from dev at addr
*   Action   : 
*	Input    : addr--the address which start to read, buffer--the pointer 
to buffer used to store data
               len-- the length of data needed to read
*	Output   : None
*	Return   : 1--success  0--fail 
*****************************************************************************************/
uint8 ReadEepromBytes(void *buffer, uint16 addr, uint16 lenth)//(uint16 addr, void *buffer, uint16 lenth)
{
    uint8 repeat;

    if (lenth == 0)
    {
        return FALSE;
    }

	for(repeat = 3; repeat!= 0; repeat--)
	{
		if(read_IIC_bytes(buffer, EEPROM_DEV, addr, lenth))//(addr, buffer, lenth, EEPROM_DEV)
		{
			return TRUE;
		}
	}
    return FALSE;	
}


/*****************************************************************************************
*   Write_IIC_Bytes
*   Function : Write data from dev at addr
*   Action   : 
*	Input    : addr--the address which start to Write, buffer--the pointer to buffer used to store data
               len-- the length of data needed to read
*	Output   : None
*	Return   : 1--success  0--fail 
*****************************************************************************************/
uint8 Write_IIC_Bytes(const void *buffer, uint8 dev, uint16 addr, uint16 lenth)//(uint16 addr, const void *buffer, uint16 lenth, uint8 dev)
{
	uint8 * p;
	uint16 cTemp0,cTemp1;
	uint8 flag;
	
	for(cTemp1 = EE_TryTimes; cTemp1 != 0; cTemp1--)
	{
		if(cTemp1)
		{
			if (detect_sda() == FALSE)
			{
				continue;
			}
		}

		IIC_start();
		if(!EEPROM_AddrSend(dev, addr)) 
		{
			continue;
		} 

		p = (uint8 *)buffer;
		for(cTemp0 = lenth; cTemp0 > 0; cTemp0--)
		{
			flag = IIC_send400k(*p);
			if(!flag)
			{
				break;
			}
			p++;
		}
		if(cTemp0 != 0)
		{
			continue;
		}
		IIC_stop();

		return TRUE;
	}
	IIC_stop();

	return FALSE;
}

/*****************************************************************************************
*   WriteEepromBytes
*   Function : Write data into EEPROM at addr
*   Action   : 
*	Input    : addr--the address which start to Write, buffer--the pointer 
to buffer used to store data
               len-- the length of data needed to read
*	Output   : None
*	Return   : 1--success  0--fail 
*****************************************************************************************/
uint8 WriteEepromBytes(const void *buffer, uint16 addr, uint16 lenth)//(uint16 addr, const void *buffer, uint16 lenth)
{
    uint16 repeat, bleng;
    uint8  cbak[EE_PAGELEN];

    if (lenth == 0)
    {
        return FALSE;
    }

    do
    {    //(addr % EE_PAGELEN) 已经写完的长度
        if(((addr % EE_PAGELEN) + lenth) > EE_PAGELEN) //已经写的长度加上要写数据的长度大于一页的长度
        {
            bleng = EE_PAGELEN - (addr % EE_PAGELEN);
            lenth -= bleng;//下一页需要多少长度
        }
        else //比一页的长度小 则直接赋值
        {
            bleng = lenth;
            lenth  = 0;
        }

        for(repeat = 3; repeat!= 0; repeat--)
        {
            read_IIC_bytes(cbak, EEPROM_DEV, addr, bleng);//(addr, cbak, bleng, EEPROM_DEV)
            if(my_memcmp((uint8*)buffer, cbak, bleng)==0)//比较buffer和cbak在bleng个字节的值
            {
                repeat = 3;     // wirte ok!
                break;          // if the data is same,break
            }
            if(!Write_IIC_Bytes(buffer, EEPROM_DEV, addr, bleng))//(addr, buffer, bleng, EEPROM_DEV))
            {
                return FALSE;
            }
        }

        if(0 == repeat)
        {
            return FALSE;
        }
        buffer  = (uint8 *)buffer + bleng;
        addr += bleng;
    }while(lenth != 0);

    return TRUE;
}

/*****************************************************************************************
*   CheckEepromBytes
*   Function : check data into EEPROM at addr
*   Action   : 
*	Input    : addr--the address which start to Write, buffer--the pointer 
to buffer used to store data
               len-- the length of data needed to check data
*	Output   : None
*	Return   : 1--success  0--fail 
*****************************************************************************************/
BOOL_B CheckEepromBytes(const void *buffer, uint16 addr, uint16 lenth)
{
	uint8 dev = EEPROM_DEV, Temp;
	uint16 cTemp1,cTemp0;
	const uint8 *p;
	
	for(cTemp1 = EE_TryTimes; cTemp1 != 0; cTemp1--)
	{
		if(cTemp1)
		{
			if (detect_sda() == FALSE)
			{
				continue;
			}
		}

		IIC_start();
		if(!EEPROM_AddrSend(dev,addr)) 
		{
			continue;
		}

		p = (uint8 *)buffer;
		IIC_start();
#if EEPROMTYPE <= 16
		if(!IIC_send400k(dev | 0x01 | ((unsigned char)(addr / 0x100) << 1))) 
		{
			continue;
		}
#elif EEPROMTYPE <= 512
		if(!IIC_send400k(dev | 0x01)) 
		{
			continue;
		}
#else
		if(!IIC_send400k(dev | 0x01 | ((unsigned char)(addr >> 16) << 1))) 
		{
			continue;
		}
#endif
		for(cTemp0 = lenth-1; cTemp0 != 0; cTemp0--)
		{
			Temp = IIC_receive400k();
			if(Temp != *p++)
			{
				IIC_stop();
				return FALSE;
			}
			SET_IIC_SDA_OUTPUT(); // ack
			CLR_IIC_SDA();
			SET_IIC_SCL();
			Dly1us();

			CLR_IIC_SCL();
			SET_IIC_SDA();
		}
		Temp = IIC_receive400k();
		if(Temp != *p)
		{
			IIC_stop();
			return FALSE;
		}

		SET_IIC_SDA_OUTPUT();
		SET_IIC_SDA();
		SET_IIC_SCL();//收完最后一个字节发送NO ACK信号
		Dly1us();

		CLR_IIC_SCL();
		IIC_stop();

		return TRUE;
	}
	IIC_stop();

	return FALSE;
}

void EEp_Test_Task(void)
{
 uint16 i; 
 uint16 temp[128],temp0[128];
 for(i=0;i<128;i++)
  {
      temp[i]=i;
  }    
  WriteEepromBytes(temp,0x00,256);
  ReadEepromBytes(temp0,0x00,256);
  if(memcmp(temp, temp0, 256)==0)
  {
    //SetSegment(SEG_hand,0);
    //RefreshLCD();
  }  
  else 
  { 
    //SetSegment(SEG_battery,0);
    //RefreshLCD();
  }    
}

