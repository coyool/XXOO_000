/*!
 ******************************************************************************
 **
 ** \file Dr_MX25L3206E.c
 **
 ** \brief UART drivers
 **
 ** \author 
 **
 ** \version V0.10
 **
 ** \date 2013-10-10
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */


 /*****************************************************************************
����:FLASHоƬ���ݶ�д
�ӿ�:Init_Flash          ��ʼ��������            �������:��
                                                 �������:��
                                                
     MX25L3206_Read        Flash���ݶ�ȡ����     �������:INT32U Dst��Ŀ���ַ,INT32U NByte:	Ҫ��ȡ�������ֽ���,INT8U* RcvBufPt:���ջ����ָ��
                                                 �������:�ɹ�����OK�����󷵻�FAIL��
                                                
     MX25L3206_Write        Flash����д�뺯��    �������:INT32U Dst��Ŀ���ַ,INT8U* SndbufPt:���ͻ�����ָ��,INT32U NByte:Ҫд�������ֽ���
                                                 �������:�ɹ�����OK�����󷵻�FAIL��
                                                 
     MX25L3206_SecErase  Flash����������������   �������:sec1 ���������š�
                                                 �������:�ɹ�����OK�����󷵻�FAIL��
                                                  
     MX25L3206_Erase     Flash���ݲ�������       �������:sec1 ������ʼ�����ţ�sec2 ������ֹ������
                                                 �������:�ɹ�����OK�����󷵻�FAIL��
����:2012.10.19

���� 2012.10.25
1�����Ӷ�дʱ������æ�жϣ���ʱ�����˳�����
2����������д��ʱ����Ҫд��ҳ�Ƿ�Ϊ�������жϡ�
3��ȡ��д����״̬�Ĵ������ݻָ�����Ϊֻ�� 64������д��������������û��д������
******************************************************************************/

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/

#include "Include.h"


U08 FLASH_ERROR;

/*******************************************************************************
 * @function_name: MX25L3206_IOInit
 * @function_file: Dr_MX25L3206E.c
 * @������MX25L3206����IO�ڳ�ʼ��
 * @������         ��
 * @param:ser      
 *---------------------------------------------------------
 * @�޸��ˣ� ffq    (2013-10-09)
 ******************************************************************************/
void MX25L3206_IOInt(void)
{
    IO_ConfigFuncMClkPin(IO_MCLK_X0X1_GPIO);
    IO_DisableFunc(IO_PORT4,IO_PINxB);              //SCK
    IO_DisableFunc(IO_PORT4,IO_PINxC);              //SI
    
    IO_ConfigGPIOPin(IO_PORTE,IO_PINx2,IO_DIR_INPUT, IO_PULLUP_OFF); //CS
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxB,IO_DIR_INPUT, IO_PULLUP_OFF); //SCK 
    IO_ConfigGPIOPin(IO_PORTE,IO_PINx3,IO_DIR_INPUT, IO_PULLUP_OFF); //SI
    
    IO_WriteGPIOPin(IO_PORTE,IO_PINx2,IO_BIT_CLR);   //CS
    IO_WriteGPIOPin(IO_PORT4,IO_PINxB,IO_BIT_CLR);   //SCK
    IO_WriteGPIOPin(IO_PORTE,IO_PINx3,IO_BIT_CLR);   //SI
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxC,IO_DIR_INPUT, IO_PULLUP_OFF); //SO
}

void MX25L3206_IOPowerDownInt(void)
{
    IO_ConfigFuncMClkPin(IO_MCLK_X0X1_GPIO);
    IO_DisableFunc(IO_PORT4,IO_PINxB);              //SCK
    IO_DisableFunc(IO_PORT4,IO_PINxC);              //SI
    IO_ConfigGPIOPin(IO_PORTE,IO_PINx2,IO_DIR_INPUT, IO_PULLUP_OFF);  //CS
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxB,IO_DIR_INPUT, IO_PULLUP_OFF);  //SCK
    IO_ConfigGPIOPin(IO_PORTE,IO_PINx3,IO_DIR_INPUT, IO_PULLUP_OFF);  //SI
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxC,IO_DIR_INPUT, IO_PULLUP_OFF);  //SO
}

/*******************************************************************************
** ��������: Statsend													
** �������ܣ�MX25L˫������SPIʱ��	
** ��ڲ���: ��															
** ���ڲ���:SPI��������																
********************************************************************************/		
unsigned char  Statsend(void)
{
    union Byte_Bit     Trans;
    u8  i;
    unsigned char   Ch;			
    for(i=0;i<4;i++)                                                  //˫·�����ֻ��Ҫ4��SPI����
    {
        Trans.Byte=(Trans.Byte<<2);	                                  //ÿ������2λ				
	Atsck_Low;                                                     //�������½���׼�������¸��½�������ǰ��ȡ
	Atsck_High;	
	if(Atsout)                                                     //SO�˿ڵ�������8��6��4��2λ
	{
            Trans.Bit.B1=1;
	}
	else
	{
            Trans.Bit.B1=0;
	}	
	if(Atsin_data)                                                //SI�˿ڵ�������7��5��3��1λ
	{
            Trans.Bit.B0=1;
	}
	else
	{
            Trans.Bit.B0=0;
        }			
    }
    Ch=Trans.Byte;
    return(Ch);
}


/************************************************************************
** ��������: Send_Byte													
** �������ܣ�ͨ��Ӳ��SPI����һ���ֽ�			
** ��ڲ���:data															
** ���ڲ���:��																
************************************************************************/
void Send_Byte(u8 data)
{
    union Byte_Bit     Trans;
    u8 i;
    Trans.Byte=data;		//��ڲ�����ֵ��������Trans.Byte		                 
    for (i=0;i<8;i++)
    {
        Atsck_Low;                   /*�͵�ƽ�ı�����*/
	Atsin(Trans.Bit.B7);       /*�������λ*/
	Atsck_High;                    /*�������������*/
	Trans.Byte=(Trans.Byte<<1);    /*����һλ����λ7��*/
    }
    Atsck_Low;     								
}

/************************************************************************
** ��������:Get_Byte														
** ��������:ͨ��Ӳ��SPI�ӿڽ���һ���ֽڵ�������						
** ��ڲ���:��																
** ���ڲ���:����ֵ																
************************************************************************/
u8 Get_Byte(void)
{
    union Byte_Bit     Trans;
    u8   i;
    unsigned char   Ch;		
    for(i=0;i<8;i++)
    {
        Trans.Byte=(Trans.Byte<<1);	        			
	Atsck_Low;
	Atsck_High;
	if(Atsout)
	{
            Trans.Bit.B0=1;
	}
	else
	{
            Trans.Bit.B0=0;
	}			
    }		
    Ch=Trans.Byte;
    return(Ch);                    // ���ؽ��յ�������    				
}

/************************************************************************
** ��������:AT45DBXXX_Read																										
** ��������:MX25L3206�Ŀ��ٶ�����,��ѡ���ID�Ͷ����ݲ���				
** ��ڲ���:
**			INT32U Dst��Ŀ���ַ,				
**      	       INT32U NByte:	Ҫ��ȡ�������ֽ���
**			INT8U* RcvBufPt:���ջ����ָ��

** ���ڲ���:�����ɹ��򷵻�OK,ʧ���򷵻�FAIL		

************************************************************************/
U08 MX25L3206_Read(u8* RcvBufPt,u32 Dst, U16 NByte)    
{
    u32 i = 0;
    u8 StatRgVal = 0 ;
    u16 k = 0;
    //************************IO�˿ڳ�ʼ��	
    MX25L3206_IOInt(); 
    if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	
        return (FAIL);	                                 //�����ڲ�����0�ֽڣ����ǳ������洢��Χ������ 
    do
    {
        CE_Low();			 
	Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
	StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
	CE_High();	
	k++;
	if(k > 1772)                              //���оƬ��æʱ�䳬��1772*254uS = 450MS��������˳�����������ʱ�䡣
        {  
            return (FAIL);
	}
    }	
    while ((StatRgVal & 0x01) );					//���оƬ�Ƿ����  
    
     CE_Low();		
     Send_Byte(0x3B); 						                          //����˫���ٶȶ�ȡ����3B:Dread command	
     Send_Byte(((Dst & 0xFFFFFF) >> 16));	                           // ���͵�ַ��Ϣ:�õ�ַ��3���ֽ����
     Send_Byte(((Dst & 0xFFFF) >> 8));
     Send_Byte(Dst & 0xFF);
     Send_Byte(0xFF);						                          // ����һ�������ֽڣ��ȴ�оƬ�������ͳ���
     Atsin_in;                                                    //��DOBELģʽ�£�����������ڸ�Ϊ���룬�Ա��SOʵ��˫·���ա�
     for (i = 0; i < NByte; i++)		
     {
	*RcvBufPt = Statsend ();
	RcvBufPt ++ ;
     }
     Atsin_Out;                                                       //������Ϻ���������ڸ�Ϊ�����
     CE_High();			

     return (OK);
}

/************************************************************************
** ��������:MX25L3206_SecErase												
** ��������:����ָ���������Ų���								
** ��ڲ���:
**			INT32U sec1������������,			
** ���ڲ���:�����ɹ��򷵻�OK,ʧ���򷵻�FAIL		
************************************************************************/
u8 MX25L3206_SecErase(u32 sec1)
{
    u8  temp1 = 0,StatRgVal = 0;
    u32 SecnHdAddr = 0;	 
    u32 k = 0 ;
	 
    MX25L3206_IOInt ();
	
    /* ***** �����ڲ�?*****/
    if (sec1 > SEC_MAX)                                 //�����������1023
    {
        return (FAIL);	
    }
    if (sec1 > 1007)                                    //���������������һ�������壺1023����
    {
        k = 0;
	do
	{
            CE_Low();			 
            Send_Byte(0x05);				// ���Ͷ�״̬�Ĵ�������
            StatRgVal = Get_Byte();			// ������õ�״̬�Ĵ���ֵ
            CE_High();	
	    k++;
            if (k > 1772 )                             //���оƬ��æʱ�䳬��1772*254uS = 450MS��������˳�����������ʱ�䡣
            {  
                return (FAIL);
            }
        }		   
	while ((StatRgVal & 0x01) );			//���оƬ�Ƿ����
	   
	CE_Low();			 
	Send_Byte(0x05);				// ���Ͷ�״̬�Ĵ�������
	temp1 = Get_Byte();				// ������õ�״̬�Ĵ���ֵ
	CE_High();
	   
	CE_Low();			
	Send_Byte(0x06);				// ʹ״̬�Ĵ�����д
	CE_High();	

	CE_Low();								  	
	Send_Byte(0x01);				// ����д״̬�Ĵ���ָ��
	Send_Byte(0);					// ��0BPxλ��ʹFlashоƬȫ����д 
	CE_High();
    }
   
    k = 0;
    do
    {
        CE_Low();			 
	Send_Byte(0x05);				// ���Ͷ�״̬�Ĵ�������
	StatRgVal = Get_Byte();				// ������õ�״̬�Ĵ���ֵ
	CE_High();	
	k++;
	if (k > 157 )                                 //���оƬ��æʱ�䳬��157*254us = 40MS��������˳����Ĵ������õȴ�ʱ�䡣
	{  
            return (FAIL);
	}
    }
    while ((StatRgVal & 0x01) );			//���״̬�Ĵ����Ƿ��������
	
    CE_Low();			
    Send_Byte(0x06);					// ����дʹ������
    CE_High();	
	
    SecnHdAddr = SEC_SIZE * sec1;			// ������������ʼ��ַ,һ������4KBit��С
    CE_Low();		
    Send_Byte(0x20);				        // ������������ָ��
    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16));         // ����3���ֽڵĵ�ַ��Ϣ
    Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
    Send_Byte(SecnHdAddr & 0xFF);
    CE_High();	
        
	
    k = 0;
    do
    {
        CE_Low();			 
	Send_Byte(0x05);				// ���Ͷ�״̬�Ĵ�������
	StatRgVal = Get_Byte();			        // ������õ�״̬�Ĵ���ֵ
	CE_High();	
        k++;
	if (k > 1772 )                                //���оƬ��æʱ�䳬��1772*254uS = 450MS��������˳�����������ʱ�䡣
	{  
            return (FAIL);
        }
    }
    while ((StatRgVal & 0x01) );		       //���оƬ�Ƿ����
	
       
    if (sec1 > 1007)
    { 
        CE_Low();			
        Send_Byte(0x06);			      // ����дʹ������
        CE_High();			
		
        CE_Low();			
        Send_Byte(0x01);			     // ����д״̬�Ĵ���ָ��
	Send_Byte(temp1);			     // �ָ�״̬�Ĵ���������Ϣ 
        CE_High(); 
    }
    
    return (OK);	
}

/************************************************************************
** ��������:AT45DBXXX_Write											
** ��������:MX25L3206��д��������д1���Ͷ�����ݵ�ָ����ַ									
** ��ڲ���:
**			INT32U Dst��Ŀ���ַ,��Χ 0x0 - MAX_ADDR��MAX_ADDR = 0x3FFFFF��				
**			INT8U* SndbufPt:���ͻ�����ָ��
**           	INT32U NByte:Ҫд�������ֽ���

** ���ڲ���:�����ɹ��򷵻�OK,ʧ���򷵻�FAIL		

************************************************************************/
U08 MX25L3206_Write(u32 Dst,  u8* SndbufPt,U16 NByte)
{
    u8 addr3 = 0, addr2 = 0, StatRgVal = 0,addr1 = 0;
    u16 page_addr =0,i = 0,k = 0;
    u8 data;
    u32 page = 0;
	 
    MX25L3206_IOInt ();
	
    if (( (Dst + NByte-1 > MAX_ADDR)||(NByte == 0) ))
    {
        return (FAIL);	                              //�����ڲ���
    }
	
    do                                               //���оƬ�Ƿ����
    {
        CE_Low();
	Send_Byte(0x05);			     // ���Ͷ�״̬�Ĵ�������
	StatRgVal = Get_Byte();		             // ������õ�״̬�Ĵ���ֵ
	CE_High();	
	k++;
	if (k > 1772 )                              //���оƬ��æʱ�䳬��1772*254uS = 450MS��������˳�����������ʱ�䡣
        {  
            return (FAIL);
	}
    }
    while ((StatRgVal & 0x01) );					
	 
    if ((StatRgVal & 0xBC) != 0x84 )                 //�ж�״̬�Ĵ����Ƿ�Ϊ����������0X84ֵ��
    {
        FLASH_ERROR = 0X05;
	CE_Low();			
	Send_Byte(0x06);			     // дʹ�ܣ�ʹ״̬�Ĵ�����д
	CE_High();			

	CE_Low();		
	Send_Byte(0x01);			     // ����д״̬�Ĵ���ָ��
	Send_Byte(0x84);                             // �ָ�״̬�Ĵ�������λ��
        CE_High();
        return (FAIL);
    }
    else 
    {
	FLASH_ERROR = 0;
    }
	
    if (Dst > 0x3EFFFF )                               //���д������Ϊ��64��
    {          
        k = 0;
	do
        {
            CE_Low();			 
	    Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
	    StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
	    CE_High();	            
            k++;
	    if (k > 157 )                           //���оƬ��æʱ�䳬��157*254us = 40MS��������˳����Ĵ������õȴ�ʱ�䡣
            {  
                return (FAIL);
	    }
  	 }
	 while ((StatRgVal & 0x01) );					//���Ĵ����Ƿ��������
            	   
	 CE_Low();			
	 Send_Byte(0x06);							     // дʹ�ܣ�ʹ״̬�Ĵ�����д
	 CE_High();			

	 CE_Low();		
	 Send_Byte(0x01);							    // ����д״̬�Ĵ���ָ��
	 Send_Byte(0);							        // ��0BPxλ��ʹFlashоƬȫ����д 
         CE_High();		
    }
		
    page = Dst/256;
    page_addr = Dst % 256;
   
    for(; i < NByte; )
    {
        k = 0;
        do                                                  //���оƬ�Ƿ����
        {
            CE_Low();			 
	    Send_Byte(0x05);						           // ���Ͷ�״̬�Ĵ�������
	    StatRgVal = Get_Byte();					       // ������õ�״̬�Ĵ���ֵ
	    CE_High();	            
            k++;
	    if (k > 157 )                                  //���оƬ��æʱ�䳬��157*254us = 40MS��������˳����Ĵ������õȴ�ʱ�䡣
            {  
                return (FAIL);
	    }
         }
	 while ((StatRgVal & 0x01) );					  //���ҳд�Ƿ���ϡ���Ϻ�д��һҳ
	
         addr3 = (u8)(page >> 8) ;
       	 addr2 = (u8)(page & 0xFF);
       	 addr1 = (page_addr & 0xFF);

         if ((page % 16) == 0)                       //�жϴ�дҳ�Ƿ�Ϊ��������ʼҳ��
	 {
             if (page_addr == 0)                       //��ַΪҳ��ʼ��ַ
             {
                 MX25L3206_SecErase(page/16);            //��д����ɾ�� 
             }
	    
	 }
           	 
         CE_Low();			
         Send_Byte(0x06);						       // ����дʹ������
         CE_High();			

         CE_Low();			
         Send_Byte(0x02); 						       // �����ֽ�������д����
         Send_Byte(addr3);                             // ����3���ֽڵĵ�ַ��Ϣ 
	 Send_Byte(addr2);
         Send_Byte(addr1);

         for ( ;i < NByte;i ++)
         {
             data = *SndbufPt;
	     Send_Byte(data);	                          // ���ͱ���д������
	     SndbufPt ++;
	     page_addr ++;
             if (page_addr >= 256)
             {
                 page_addr = 0;
                 page ++ ;
                 break;
             }
			   
         }		 
         CE_High();	
         i ++;
    }   
	  	
    if (Dst > 0x3EFFFF )                               //���д������Ϊ��64��
    {         
	 k = 0;
	 do
	 {
             CE_Low();			 
             Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
             StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
             CE_High();	
	     k++;
             if (k > 157 )                                  //���оƬ��æʱ�䳬��157*254us = 40MS��������˳����Ĵ������õȴ�ʱ�䡣
             {  
                 return (FAIL);
             }
	  }
	  while ((StatRgVal & 0x01) );					//���оƬ�Ƿ����

	  CE_Low();			
	  Send_Byte(0x06);							// ����дʹ������
	  CE_High();			
			
	  CE_Low();			
	  Send_Byte(0x01);							// ����д״̬�Ĵ���ָ��
	  Send_Byte(0X84);							// ����д����λ������64��
	  CE_High();
    }	 
    return (OK);		
}

/************************************************************************
** ��������:MX25L3206_Erase												
** ��������:����ָ����������ѡȡ���Ч���㷨����								
** ��ڲ���:
**			INT32U sec1����ʼ������,
**			INT32U sec2����ֹ������
** ���ڲ���:�����ɹ��򷵻�OK,ʧ���򷵻�FAIL		
************************************************************************/
u8 MX25L3206_Erase(u16 sec1, u16 sec2)
{
    u8  temp1 = 0,temp2 = 0,StatRgVal = 0;
    u32 k;
    u32 SecnHdAddr = 0;	  			
    u32 no_SecsToEr = 0;				   			// Ҫ������������Ŀ
    u32 CurSecToEr = 0;	  						    // ��ǰҪ������������
	 
    MX25L3206_IOInt ();
	 
    /*  �����ڲ��� */
    if ((sec1 > SEC_MAX)||(sec2 > SEC_MAX))	
    {
        return (FAIL);	
    }
    k = 0 ;
    do
    {
        CE_Low();			 
        Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
        StatRgVal = Get_Byte();					    // ������õ�״̬�Ĵ���ֵ
        CE_High();	
	k++;
        if (k > 1772 )                          //���оƬ��æʱ�䳬��1772*254uS = 450MS��������˳�����������ʱ�䡣
        {  
            return (FAIL);
	}
    }
    while ((StatRgVal & 0x01) );					//���оƬ�Ƿ����

    if (((sec1 > sec2)?sec1 : sec2) > 1007)        //�����������λ��64�顣����д����
    {
        CE_Low();			 
	Send_Byte(0x05);								// ���Ͷ�״̬�Ĵ�������
	temp1 = Get_Byte();							// ������õ�״̬�Ĵ���ֵ
	CE_High();
		   
	CE_Low();			
	Send_Byte(0x06);								// ʹ״̬�Ĵ�����д
	CE_High();	

	CE_Low();								  	
	Send_Byte(0x01);								// ����д״̬�Ĵ���ָ��
	Send_Byte(0);								// ��0BPxλ��ʹFlashоƬȫ����д 
	CE_High(); 

	do
        {
            CE_Low();			 
            Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
            StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
            CE_High();	
            k++;
            if (k > 157 )                           //���оƬ��æʱ�䳬��157*254us = 40MS��������˳����Ĵ������õȴ�ʱ�䡣
            {  
                return (FAIL);
            }
	}
        while ((StatRgVal & 0x01) );					//���״̬�Ĵ��������Ƿ����
    }

    CE_Low();			
    Send_Byte(0x06);								// ����дʹ������
    CE_High();	

	                                                /* ����û��������ʼ�����Ŵ�����ֹ�����ţ������ڲ��������� */
    if (sec1 > sec2)
    {
        temp2 = sec1;
        sec1  = sec2;
	sec2  = temp2;
    } 
	                                                /* ����ֹ���������������������� */
    if (sec1 == sec2)	
    {
        SecnHdAddr = SEC_SIZE * sec1;				// ������������ʼ��ַ
	CE_Low();	
	Send_Byte(0x20);							// ������������ָ��
	Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ����3���ֽڵĵ�ַ��Ϣ
	Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
	Send_Byte(SecnHdAddr & 0xFF);
	CE_High();			
	do
	{
            CE_Low();			 
            Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
            StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
            CE_High();	
            k++;
            if (k > 1772 )                          //���оƬ��æʱ�䳬��1772*254uS = 450MS��������˳���
            {  
                return (FAIL);
            }
        }
	while ((StatRgVal & 0x01) );					//���оƬ�Ƿ����
    }	
   /* ����������֮��ļ���������ȡ16���������㷨 */
    else 
    {
        no_SecsToEr = sec2 - sec1 +1;					// ��ȡҪ������������Ŀ
	CurSecToEr  = sec1;								// ����ʼ������ʼ����	
        while (no_SecsToEr >= 16)
        { 
	     SecnHdAddr = SEC_SIZE * CurSecToEr;			// ������������ʼ��ַ	     
	     CE_Low();			
	     Send_Byte(0x06);								// ����дʹ������
	     CE_High();
		 
             CE_Low();	
	     Send_Byte(0xD8);							// ����64KB�����ָ��
	     Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ����3���ֽڵĵ�ַ��Ϣ
             Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
             Send_Byte(SecnHdAddr & 0xFF);
             CE_High();
             k = 0; 
             do
	     {  
                  CE_Low();			 
		  Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
		  StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
		  CE_High();	
	          k++;
		  if (k > 9450 )                          //���оƬ��æʱ�䳬��9450*254uS = 2.4S��������˳���
		  {  
                      return (FAIL);
                  } 
             }
	     while ((StatRgVal & 0x01) );					//���оƬ�Ƿ����
             DRV_WD_FeedDog();
             CurSecToEr  += 16;							// ���������16��������,�Ͳ����������ڵĴ�����������
             no_SecsToEr -=  16;							// �������������������������
        }
	/* �������������㷨����ʣ������� */
	while (no_SecsToEr >= 1)
        {
	     SecnHdAddr = SEC_SIZE * CurSecToEr;			// ������������ʼ��ַ
	     
	     CE_Low();			
	     Send_Byte(0x06);								// ����дʹ������
	     CE_High();
		 
	     CE_Low();	
             Send_Byte(0x20);							// ������������ָ��
	     Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // ����3���ֽڵĵ�ַ��Ϣ
             Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
             Send_Byte(SecnHdAddr & 0xFF);
             CE_High();			
             do
	     {
                  CE_Low();			 
		  Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
		  StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
		  CE_High();	
	          k++;
		  if (k > 1772 )                          //���оƬ��æʱ�䳬��1772*254uS = 450MS��������˳���
		  {  
                      return (FAIL);
                  }
             }
	     while ((StatRgVal & 0x01) );					//���оƬ�Ƿ����
             CurSecToEr  += 1;
             no_SecsToEr -=  1;
        }
    }
   /* ��������,�ָ�״̬�Ĵ�����Ϣ							*/

    if (((sec1 > sec2)?sec1 : sec2) > 1007)
    {
        do
	{
            CE_Low();			 
            Send_Byte(0x05);						    // ���Ͷ�״̬�Ĵ�������
            StatRgVal = Get_Byte();					// ������õ�״̬�Ĵ���ֵ
            CE_High();	
	    DRV_WD_FeedDog();
	}
	while ((StatRgVal & 0x01) );					//���оƬ�Ƿ����

	CE_Low();			
	Send_Byte(0x06);								// ����дʹ������
	CE_High();			
		
	CE_Low();			
	Send_Byte(0x01);								// ����д״̬�Ĵ���ָ��
	Send_Byte(temp1);								// �ָ�״̬�Ĵ���������Ϣ 
	CE_High();
    }
    return (OK);
}



/*********************************************************************
***************flash���Ժ���******************************************
**********************************************************************/
void test_flash(void)
{   
    uint16  t1=0,t2=0,t3=0,t4=0;
    uint8 FlashBuffer1[256];
    uint8 FlashBuffer2[256];
    IO_DisableAnalogInput(IO_AN15);
    IO_DisableFunc(IO_PORT1,IO_PINxF);
    IO_ConfigGPIOPin(IO_PORT1,IO_PINxF,IO_DIR_OUTPUT,IO_PULLUP_OFF);
    for(t1=0;t1<256;t1++)
    {
        FlashBuffer1[t1] = t1;
    }
    MX25L3206_Write((uint32)0x1E0000,FlashBuffer1,sizeof(FlashBuffer1));
    MX25L3206_Read(FlashBuffer2,(uint32)0x1E0000,sizeof(FlashBuffer2));
    //MX25L3206_Erase(0,1023);
    //MX25L3206_SecErase(480);
    for(t2=0;t2<256;t2++)
    {
        if(FlashBuffer2[t2] == FlashBuffer1[t2])
        {
            t3=t3+1;
        }
    }
    if(t3 == 256)
    {
        IO_WriteGPIOPin(IO_PORT1,IO_PINxF,IO_BIT_SET);
    }
    MX25L3206_SecErase(480);
    MX25L3206_Read(FlashBuffer2,(uint32)0x1E0000,sizeof(FlashBuffer2));
    for(t2=0;t2<256;t2++)
    {
        if(FlashBuffer2[t2] == 0xFF)
        {
            t4=t4+1;
        }
    }
    if(t4 == 256)
    {
        IO_WriteGPIOPin(IO_PORT1,IO_PINxF,IO_BIT_CLR);
    }
}
//==========================================================================
