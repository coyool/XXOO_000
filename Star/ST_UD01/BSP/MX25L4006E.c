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
功能:FLASH芯片数据读写
接口:Init_Flash          初始化函数。            输入参数:无
                                                 输出参数:无
                                                
     MX25L3206_Read        Flash数据读取函数     输入参数:INT32U Dst：目标地址,INT32U NByte:	要读取的数据字节数,INT8U* RcvBufPt:接收缓存的指针
                                                 输出参数:成功返回OK，错误返回FAIL。
                                                
     MX25L3206_Write        Flash数据写入函数    输入参数:INT32U Dst：目标地址,INT8U* SndbufPt:发送缓存区指针,INT32U NByte:要写的数据字节数
                                                 输出参数:成功返回OK，错误返回FAIL。
                                                 
     MX25L3206_SecErase  Flash数据扇区擦除函数   输入参数:sec1 擦除扇区号。
                                                 输出参数:成功返回OK，错误返回FAIL。
                                                  
     MX25L3206_Erase     Flash数据擦除函数       输入参数:sec1 擦除起始扇区号，sec2 擦除终止扇区号
                                                 输出参数:成功返回OK，错误返回FAIL。
日期:2012.10.19

更改 2012.10.25
1、增加读写时器件繁忙判断，超时错误退出功能
2、增加数据写入时，将要写入页是否为新扇区判断。
3、取消写数据状态寄存器备份恢复，改为只对 64块设置写保护，其他区块没有写保护。
******************************************************************************/

/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include    "all_header_file.h"


U08 FLASH_ERROR;

/*******************************************************************************
 * @function_name: MX25L3206_IOInit
 * @function_file: Dr_MX25L3206E.c
 * @描述：MX25L3206所用IO口初始化
 * @参数：         无
 * @param:ser      
 *---------------------------------------------------------
 * @修改人： ffq    (2013-10-09)
 ******************************************************************************/
void MX25L3206_IOInt(void)
{
/* 5V --- 3.3V */    
//    IO_ConfigFuncMClkPin(IO_MCLK_X0X1_GPIO);
//    IO_DisableFunc(IO_PORT4,IO_PINx2);              //SCK
//    IO_DisableFunc(IO_PORT4,IO_PINx4);              //SI
//    
//    IO_ConfigGPIOPin(IO_PORT4,IO_PINx5,IO_DIR_INPUT, IO_PULLUP_ON);  //cs
//    IO_ConfigGPIOPin(IO_PORT4,IO_PINx2,IO_DIR_INPUT, IO_PULLUP_ON);  //sck
//    IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_INPUT, IO_PULLUP_ON);   //si
//    
//    IO_WriteGPIOPin(IO_PORT4,IO_PINx5,IO_BIT_CLR);  //cs
//    IO_WriteGPIOPin(IO_PORT4,IO_PINx2,IO_BIT_CLR);  //sck
//    IO_WriteGPIOPin(IO_PORT4,IO_PINx4,IO_BIT_CLR);   //si
//    IO_ConfigGPIOPin(IO_PORT4,IO_PINx3,IO_DIR_INPUT, IO_PULLUP_ON); //so
    
/* 3.3V --- 3.3V */    
    IO_DisableFunc(IO_PORT4,IO_PINx2);              //SCK
    IO_DisableFunc(IO_PORT4,IO_PINx4);              //SI
    
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx5,IO_DIR_OUTPUT, IO_PULLUP_OFF);  //cs
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx2,IO_DIR_OUTPUT, IO_PULLUP_OFF);  //sck
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_OUTPUT, IO_PULLUP_OFF);   //si
    
    IO_WriteGPIOPin(IO_PORT4,IO_PINx5,IO_BIT_SET);  //cs
    IO_WriteGPIOPin(IO_PORT4,IO_PINx2,IO_BIT_SET);  //sck
    IO_WriteGPIOPin(IO_PORT4,IO_PINx4,IO_BIT_SET);   //si
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx3,IO_DIR_INPUT, IO_PULLUP_ON); //so
}

void MX25L3206_IOPowerDownInt(void)
{
    //IO_ConfigFuncMClkPin(IO_MCLK_X0X1_GPIO);
    IO_DisableFunc(IO_PORT4,IO_PINx2);              //SCK
    IO_DisableFunc(IO_PORT4,IO_PINx4);              //SI
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx5,IO_DIR_INPUT, IO_PULLUP_ON); //cs
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx2,IO_DIR_INPUT, IO_PULLUP_ON); //sclk 
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx4,IO_DIR_INPUT, IO_PULLUP_ON); //si
    IO_ConfigGPIOPin(IO_PORT4,IO_PINx3,IO_DIR_INPUT, IO_PULLUP_ON); //so
}

/*******************************************************************************
** 函数名称: Statsend													
** 函数功能：MX25L双倍接收SPI时序	
** 入口参数: 无															
** 出口参数:SPI接收数据																
********************************************************************************/		
unsigned char  Statsend(void)
{
    union Byte_Bit     Trans;
    u8  i;
    unsigned char   Ch;			
    for(i=0;i<4;i++)                                                  //双路输出，只需要4个SPI周期
    {
        Trans.Byte=(Trans.Byte<<2);	                                  //每次左移2位				
	Atsck_Low;                                                     //数据在下降沿准备，在下个下降沿来临前读取
	Atsck_High;	
	if(Atsout)                                                     //SO端口的数据是8、6、4、2位
	{
            Trans.Bit.B1=1;
	}
	else
	{
            Trans.Bit.B1=0;
	}	
	if(Atsin_data)                                                //SI端口的数据是7、5、3、1位
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
** 函数名称: Send_Byte													
** 函数功能：通过硬件SPI发送一个字节			
** 入口参数:data															
** 出口参数:无																
************************************************************************/
static void Send_Byte(u8 data)
{
    union Byte_Bit     Trans;
    u8 i;
    Trans.Byte=data;		//入口参数赋值给共用体Trans.Byte		                 
    for (i=0;i<8;i++)
    {
        Atsck_Low;                   /*低电平改变数据*/
        Atsin(Trans.Bit.B7);       /*传送最高位*/
        Atsck_High;                    /*上升沿输出数据*/
        Trans.Byte=(Trans.Byte<<1);    /*左移一位，移位7次*/
    }
    Atsck_Low;     								
}

/************************************************************************
** 函数名称:Get_Byte														
** 函数功能:通过硬件SPI接口接收一个字节到处理器						
** 入口参数:无																
** 出口参数:返回值																
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
    return(Ch);                    // 返回接收到的数据    				
}

/************************************************************************
** 函数名称:AT45DBXXX_Read																										
** 函数功能:MX25L3206的快速读函数,可选择读ID和读数据操作				
** 入口参数:
**			INT32U Dst：目标地址,				
**      	       INT32U NByte:	要读取的数据字节数
**			INT8U* RcvBufPt:接收缓存的指针

** 出口参数:操作成功则返回OK,失败则返回FAIL		

************************************************************************/
U08 MX25L3206_Read(u8* RcvBufPt, u32 Dst, U16 NByte)    
{
    u32 i = 0;
    u8 StatRgVal = 0 ;
    u16 k = 0;
    //************************IO端口初始化	
    MX25L3206_IOInt(); 
    if ((Dst+NByte > MAX_ADDR)||(NByte == 0))	
        return (FAIL);	                                 //检查入口参数，0字节，或是超出最大存储范围，出错。 
    do
    {
        CE_Low();			 
        Send_Byte(0x05);						    // 发送读状态寄存器命令
        StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
        CE_High();	
        k++;
        if(k > 15000)                              //如果芯片繁忙时间超过15000*40uS = 600MS，则出错退出。扇区擦除时间。
        {  
            return (FAIL);
        }
    }	
    while ((StatRgVal & 0x01) );					//检查芯片是否空闲  
    
     CE_Low();		
     Send_Byte(0x3B); 						                          //发送双倍速度读取命令3B:Dread command	
     Send_Byte(((Dst & 0xFFFFFF) >> 16));	                           // 发送地址信息:该地址由3个字节组成
     Send_Byte(((Dst & 0xFFFF) >> 8));
     Send_Byte(Dst & 0xFF);
     Send_Byte(0xFF);						                          // 发送一个空闲字节，等待芯片将数据送出。
     Atsin_in;                                                    //在DOBEL模式下，将数据输出口改为输入，以便和SO实现双路接收。
     for (i = 0; i < NByte; i++)		
     {
	*RcvBufPt = Statsend ();
	RcvBufPt ++ ;
     }
     Atsin_Out;                                                       //接收完毕后，数据输出口改为输出。
     CE_High();			

     return (OK);
}

/************************************************************************
** 函数名称:MX25L3206_SecErase												
** 函数功能:根据指定的扇区号擦除								
** 入口参数:
**			INT32U sec1：擦除扇区号,			
** 出口参数:操作成功则返回OK,失败则返回FAIL		
************************************************************************/
u8 MX25L3206_SecErase(u32 sec1)
{
    u8  temp1 = 0,StatRgVal = 0;
    u32 SecnHdAddr = 0;	 
    u32 k = 0 ;
	 
    MX25L3206_IOInt ();
	
    /* ***** 检查入口参?*****/
    if (sec1 > SEC_MAX)                                 //超出最大扇区1023
    {
        return (FAIL);	
    }
    if (sec1 > 1007)                                    //待擦除扇区是最后一个扇区煎：1023扇区
    {
        k = 0;
	do
	{
            CE_Low();			 
            Send_Byte(0x05);				// 发送读状态寄存器命令
            StatRgVal = Get_Byte();			// 保存读得的状态寄存器值
            CE_High();	
            k++;
            if (k > 15000 )                             //如果芯片繁忙时间超过15000*40uS = 600MS，则出错退出。扇区擦除时间。
            {  
                return (FAIL);
            }
        }		   
	while ((StatRgVal & 0x01) );			//检查芯片是否空闲
	   
	CE_Low();			 
	Send_Byte(0x05);				// 发送读状态寄存器命令
	temp1 = Get_Byte();				// 保存读得的状态寄存器值
	CE_High();
	   
	CE_Low();			
	Send_Byte(0x06);				// 使状态寄存器可写
	CE_High();	

	CE_Low();								  	
	Send_Byte(0x01);				// 发送写状态寄存器指令
	Send_Byte(0);					// 清0BPx位，使Flash芯片全区可写 
	CE_High();
    }
   
    k = 0;
    do
    {
        CE_Low();			 
	Send_Byte(0x05);				// 发送读状态寄存器命令
	StatRgVal = Get_Byte();				// 保存读得的状态寄存器值
	CE_High();	
	k++;
	if (k > 1000 )                                 //如果芯片繁忙时间超过1000*40us = 40MS，则出错退出。寄存器设置等待时间。
	{  
            return (FAIL);
	}
    }
    while ((StatRgVal & 0x01) );			//检查状态寄存器是否设置完毕
	
    CE_Low();			
    Send_Byte(0x06);					// 发送写使能命令
    CE_High();	
	
    SecnHdAddr = SEC_SIZE * sec1;			// 计算扇区的起始地址,一个扇区4KBit大小
    CE_Low();		
    Send_Byte(0x20);				        // 发送扇区擦除指令
    Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16));         // 发送3个字节的地址信息
    Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
    Send_Byte(SecnHdAddr & 0xFF);
    CE_High();	
        
	
    k = 0;
    do
    {
        CE_Low();			 
	Send_Byte(0x05);				// 发送读状态寄存器命令
	StatRgVal = Get_Byte();			        // 保存读得的状态寄存器值
	CE_High();	
        k++;
	if (k > 15000 )                                //如果芯片繁忙时间超过15000*40uS = 600MS，则出错退出。扇区擦除时间。
	{  
            return (FAIL);
        }
    }
    while ((StatRgVal & 0x01) );		       //检查芯片是否空闲
	
       
    if (sec1 > 1007)
    { 
        CE_Low();			
        Send_Byte(0x06);			      // 发送写使能命令
        CE_High();			
		
        CE_Low();			
        Send_Byte(0x01);			     // 发送写状态寄存器指令
        Send_Byte(temp1);			     // 恢复状态寄存器设置信息 
        CE_High(); 
    }
    
    return (OK);	
}

/************************************************************************
** 函数名称:AT45DBXXX_Write											
** 函数功能:MX25L3206的写函数，可写1个和多个数据到指定地址									
** 入口参数:
**			INT32U Dst：目标地址,范围 0x0 - MAX_ADDR（MAX_ADDR = 0x3FFFFF）				
**			INT8U* SndbufPt:发送缓存区指针
**          INT32U NByte:要写的数据字节数

** 出口参数:操作成功则返回OK,失败则返回FAIL		

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
        return (FAIL);	                              //检查入口参数
    }
	
    do                                               //检查芯片是否空闲
    {
        CE_Low();
        Send_Byte(0x05);			     // 发送读状态寄存器命令
        StatRgVal = Get_Byte();		             // 保存读得的状态寄存器值
        CE_High();	
        k++;
        if (k > 15000 )                              //如果芯片繁忙时间超过15000*40uS = 600MS，则出错退出。扇区擦除时间。
        {  
            return (FAIL);
        }
    }
    while ((StatRgVal & 0x01) );					
	 
    if ((StatRgVal & 0xBC) != 0x84 )                 //判断状态寄存器是否为正常保护，0X84值，
    {
        FLASH_ERROR = 0X05;
        CE_Low();			
        Send_Byte(0x06);			     // 写使能，使状态寄存器可写
        CE_High();			

        CE_Low();		
        Send_Byte(0x01);			     // 发送写状态寄存器指令
        Send_Byte(0x84);                             // 恢复状态寄存器保护位
        CE_High();	  
    }
    else 
    {
	FLASH_ERROR = 0;
    }
	
    if (Dst > 0x3EFFFF )                               //如果写入区域为第64块
    {          
        k = 0;
        do
        {
            CE_Low();			 
            Send_Byte(0x05);						    // 发送读状态寄存器命令
            StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
            CE_High();	            
            k++;
            if (k > 1000 )                           //如果芯片繁忙时间超过1000*40us = 40MS，则出错退出。寄存器设置等待时间。
            {  
                return (FAIL);
            }
        }
        while ((StatRgVal & 0x01) );					//检查寄存器是否设置完毕
                   
        CE_Low();			
        Send_Byte(0x06);							     // 写使能，使状态寄存器可写
        CE_High();			

        CE_Low();		
        Send_Byte(0x01);							    // 发送写状态寄存器指令
        Send_Byte(0);							        // 清0BPx位，使Flash芯片全区可写 
        CE_High();		
    }
		
    page = Dst/256;
    page_addr = Dst % 256;
   
    for(; i < NByte; )
    {
        k = 0;
        do                                                  //检查芯片是否空闲
        {
            CE_Low();			 
            Send_Byte(0x05);						           // 发送读状态寄存器命令
            StatRgVal = Get_Byte();					       // 保存读得的状态寄存器值
            CE_High();	            
            k++;
            if (k > 1000 )                                  //如果芯片繁忙时间超过1000*40us = 40MS，则出错退出。寄存器设置等待时间。
            {  
                return (FAIL);
            }
        }
        while ((StatRgVal & 0x01) );					  //检查页写是否完毕。完毕后写下一页
	
         addr3 = (u8)(page >> 8) ;
       	 addr2 = (u8)(page & 0xFF);
       	 addr1 = (page_addr & 0xFF);

         if ((page % 16) == 0)                       //判断待写页是否为新扇区起始页。
        {
             if (page_addr == 0)                       //地址为页起始地址
             {
                 MX25L3206_SecErase(page/16);            //待写扇区删除 
             }
	    
        }
           	 
         CE_Low();			
         Send_Byte(0x06);						       // 发送写使能命令
         CE_High();			

        CE_Low();			
        Send_Byte(0x02); 						       // 发送字节数据烧写命令
        Send_Byte(addr3);                             // 发送3个字节的地址信息 
        Send_Byte(addr2);
        Send_Byte(addr1);

         for ( ;i < NByte;i ++)
         {
            data = *SndbufPt;
            Send_Byte(data);	                          // 发送被烧写的数据
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
	  	
    if (Dst > 0x3EFFFF )                               //如果写入区域为第64块
    {         
	 k = 0;
	 do
	 {
             CE_Low();			 
             Send_Byte(0x05);						    // 发送读状态寄存器命令
             StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
             CE_High();	
	         k++;
             if (k > 1000 )                                  //如果芯片繁忙时间超过1000*40us = 40MS，则出错退出。寄存器设置等待时间。
             {  
                 return (FAIL);
             }
	  }
	  while ((StatRgVal & 0x01) );					//检查芯片是否空闲

	  CE_Low();			
	  Send_Byte(0x06);							// 发送写使能命令
	  CE_High();			
			
	  CE_Low();			
	  Send_Byte(0x01);							// 发送写状态寄存器指令
	  Send_Byte(0X84);							// 设置写保护位，保护64块
	  CE_High();
    }	 
    return (OK);		
}

/************************************************************************
** 函数名称:MX25L3206_Erase												
** 函数功能:根据指定的扇区号选取最高效的算法擦除								
** 入口参数:
**			INT32U sec1：起始扇区号,
**			INT32U sec2：终止扇区号
** 出口参数:操作成功则返回OK,失败则返回FAIL		
************************************************************************/
u8 MX25L3206_Erase(u16 sec1, u16 sec2)
{
    u8  temp1 = 0,temp2 = 0,StatRgVal = 0;
    u32 k;
    u32 SecnHdAddr = 0;	  			
    u32 no_SecsToEr = 0;				   			// 要擦除的扇区数目
    u32 CurSecToEr = 0;	  						    // 当前要擦除的扇区号
	 
    MX25L3206_IOInt ();
	 
    /*  检查入口参数 */
    if ((sec1 > SEC_MAX)||(sec2 > SEC_MAX))	
    {
        return (FAIL);	
    }
    k = 0 ;
    do
    {
        CE_Low();			 
        Send_Byte(0x05);						    // 发送读状态寄存器命令
        StatRgVal = Get_Byte();					    // 保存读得的状态寄存器值
        CE_High();	
        k++;
        if (k > 15000 )                          //如果芯片繁忙时间超过15000*40uS = 600MS，则出错退出。扇区擦除时间。
        {  
            return (FAIL);
	}
    }
    while ((StatRgVal & 0x01) );					//检查芯片是否空闲

    if (((sec1 > sec2)?sec1 : sec2) > 1007)        //如果擦除扇区位于64块。则解除写保护
    {
        CE_Low();			 
	Send_Byte(0x05);								// 发送读状态寄存器命令
	temp1 = Get_Byte();							// 保存读得的状态寄存器值
	CE_High();
		   
	CE_Low();			
	Send_Byte(0x06);								// 使状态寄存器可写
	CE_High();	

	CE_Low();								  	
	Send_Byte(0x01);								// 发送写状态寄存器指令
	Send_Byte(0);								// 清0BPx位，使Flash芯片全区可写 
	CE_High(); 

        do
        {
            CE_Low();			 
            Send_Byte(0x05);						    // 发送读状态寄存器命令
            StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
            CE_High();	
            k++;
            if (k > 1000 )                           //如果芯片繁忙时间超过1000*40us = 40MS，则出错退出。寄存器设置等待时间。
            {  
                return (FAIL);
            }
        }
        while ((StatRgVal & 0x01) );					//检查状态寄存器设置是否完成
    }

    CE_Low();			
    Send_Byte(0x06);								// 发送写使能命令
    CE_High();	

	                                                /* 如果用户输入的起始扇区号大于终止扇区号，则在内部作出调整 */
    if (sec1 > sec2)
    {
        temp2 = sec1;
        sec1  = sec2;
        sec2  = temp2;
    } 
	                                                /* 若起止扇区号相等则擦除单个扇区 */
    if (sec1 == sec2)	
    {
        SecnHdAddr = SEC_SIZE * sec1;				// 计算扇区的起始地址
        CE_Low();	
        Send_Byte(0x20);							// 发送扇区擦除指令
        Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // 发送3个字节的地址信息
        Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
        Send_Byte(SecnHdAddr & 0xFF);
        CE_High();			
        do
        {
            CE_Low();			 
            Send_Byte(0x05);						    // 发送读状态寄存器命令
            StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
            CE_High();	
            k++;
            if (k > 15000 )                          //如果芯片繁忙时间超过15000*40uS = 600MS，则出错退出。
            {  
                return (FAIL);
            }
        }
	while ((StatRgVal & 0x01) );					//检查芯片是否空闲
    }	
   /* 若两个扇区之间的间隔够大，则采取16扇区擦除算法 */
    else 
    {
        no_SecsToEr = sec2 - sec1 +1;					// 获取要擦除的扇区数目
        CurSecToEr  = sec1;								// 从起始扇区开始擦除	
        while (no_SecsToEr >= 16)
        { 
	     SecnHdAddr = SEC_SIZE * CurSecToEr;			// 计算扇区的起始地址	     
	     CE_Low();			
	     Send_Byte(0x06);								// 发送写使能命令
	     CE_High();
		 
             CE_Low();	
	     Send_Byte(0xD8);							// 发送64KB块擦除指令
	     Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // 发送3个字节的地址信息
             Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
             Send_Byte(SecnHdAddr & 0xFF);
             CE_High();
             k = 0; 
             do
	     {  
                  CE_Low();			 
		  Send_Byte(0x05);						    // 发送读状态寄存器命令
		  StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
		  CE_High();	
	          k++;
		  if (k > 60000 )                          //如果芯片繁忙时间超过60000*40uS = 2.4S，则出错退出。
		  {  
                      return (FAIL);
                  } 
             }
	     while ((StatRgVal & 0x01) );					//检查芯片是否空闲
             DRV_WD_FeedDog();
             CurSecToEr  += 16;							// 计算擦除了16个扇区后,和擦除区域相邻的待擦除扇区号
             no_SecsToEr -=  16;							// 对需擦除的扇区总数作出调整
        }
	/* 采用扇区擦除算法擦除剩余的扇区 */
	while (no_SecsToEr >= 1)
        {
	     SecnHdAddr = SEC_SIZE * CurSecToEr;			// 计算扇区的起始地址
	     
	     CE_Low();			
	     Send_Byte(0x06);								// 发送写使能命令
	     CE_High();
		 
	     CE_Low();	
             Send_Byte(0x20);							// 发送扇区擦除指令
	     Send_Byte(((SecnHdAddr & 0xFFFFFF) >> 16)); // 发送3个字节的地址信息
             Send_Byte(((SecnHdAddr & 0xFFFF) >> 8));
             Send_Byte(SecnHdAddr & 0xFF);
             CE_High();			
             do
	     {
                  CE_Low();			 
		  Send_Byte(0x05);						    // 发送读状态寄存器命令
		  StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
		  CE_High();	
	          k++;
		  if (k > 15000 )                          //如果芯片繁忙时间超过15000*40uS = 600MS，则出错退出。
		  {  
                      return (FAIL);
                  }
             }
	     while ((StatRgVal & 0x01) );					//检查芯片是否空闲
             CurSecToEr  += 1;
             no_SecsToEr -=  1;
        }
    }
   /* 擦除结束,恢复状态寄存器信息							*/

    if (((sec1 > sec2)?sec1 : sec2) > 1007)
    {
        do
	{
            CE_Low();			 
            Send_Byte(0x05);						    // 发送读状态寄存器命令
            StatRgVal = Get_Byte();					// 保存读得的状态寄存器值
            CE_High();	
	    DRV_WD_FeedDog();
	}
	while ((StatRgVal & 0x01) );					//检查芯片是否空闲

	CE_Low();			
	Send_Byte(0x06);								// 发送写使能命令
	CE_High();			
		
	CE_Low();			
	Send_Byte(0x01);								// 发送写状态寄存器指令
	Send_Byte(temp1);								// 恢复状态寄存器设置信息 
	CE_High();
    }
    return (OK);
}



/*********************************************************************
***************flash测试函数******************************************
**********************************************************************/
void test_flash(void)
{   
    uint16  t1=0,t2=0,t3=0,t4=0;
    uint8 FlashBuffer1[256];
    uint8 FlashBuffer2[256];
    uint8 FlashBuffer3[256];
    
 //   IO_DisableAnalogInput(IO_AN15);
 //   IO_DisableFunc(IO_PORT1,IO_PINxF);
 //   IO_ConfigGPIOPin(IO_PORT1,IO_PINxF,IO_DIR_OUTPUT,IO_PULLUP_OFF);
    MX25L3206_Erase(0, 3);
    for(t1=0;t1<256;t1++)
    {
        //FlashBuffer1[t1] = t1;
        FlashBuffer1[t1] = 0x77;
    }
    MX25L3206_Write((uint32)0x1100,FlashBuffer1,sizeof(FlashBuffer1));
    MX25L3206_Read(FlashBuffer2,(uint32)0x1100,sizeof(FlashBuffer2));
    MX25L3206_Read(FlashBuffer3,(uint32)0x2000,sizeof(FlashBuffer2));
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
   //     IO_WriteGPIOPin(IO_PORT1,IO_PINxF,IO_BIT_SET);
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
   //     IO_WriteGPIOPin(IO_PORT1,IO_PINxF,IO_BIT_CLR);
    }
}
//==========================================================================
