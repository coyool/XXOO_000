/*********************************************************************
 *
 *             RouteTable access routines 
 *
 *********************************************************************
 * FileName:        RouteTable.c
 * Dependencies:    Compiler.h
 * Processor:       SH99F01
 * Complier:        Keil C51 v9.00
 * Company:         Sinowealth Electronic, Inc.
 *
 * Software License Agreement
 *
 * This software is owned by Sinowealth Electronic Inc. ("Sinowealth") 
 * and is supplied to you for use exclusively as described in the 
 * associated software agreement.  This software is protected by 
 * software and other intellectual property laws.  Any use in 
 * violation of the software license may subject the user to criminal 
 * sanctions as well as civil liability.  Copyright 2009 Sinowealth
 * Electronic Inc.  All rights reserved.
 *
 * This software is provided "AS IS."  SINOWEALTH DISCLAIMS ALL 
 * WARRANTIES, EXPRESS, IMPLIED, STATUTORY OR OTHERWISE, NOT LIMITED 
 * TO MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND 
 * INFRINGEMENT.  Sinowealth shall in no event be liable for special, 
 * incidental, or consequential damages.
 *
 *
 * Author               Date           Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Ellie Fan            2010/7/9	   Original   
 * Joseph Zhou          2011/3/21	   Modify                                 
 ********************************************************************/


#include  <SH99F01.h>
#include  "MAIN.h"
#include  "SSP.h"
#include  "RouteTable.h"
#include  "string.h"


//MASTER路由表,  size = 11
typedef struct
{
	uchar client_SN[6];          //CLIENT序列號
	uchar net_ID;                //CLIENT網絡地址
	uchar relay;                 //中繼級數
	uchar RA;                    //最后一段中繼
	uchar BACKUP_RA;             //最后一段備份中繼  
    uchar link;                  //入網成功標志
}ROUTE_ITEM;

uchar 	client_size;


/***************************************************************************************************************************
 * Function:           void InitRouteTable()
 * Input Parameter:    None
 * OutPut Parameter:   None
 * Return Value:       None
 * Description:        This function  Initializes the Concetrator RouteTable(clear all the Routetable information and route table size) 
 *****************************************************************************************************************************/   
void InitRouteTable()                 //將EEROM區域全部擦除
{	
	uchar  index = 0;    
	while(index<PAGES)
	{		     
		SSP_Erase(index);	        
		index++; 
	}
}

void WriteRouteTable(uint dest, uchar *src, uchar len)
{
	uchar  dat;
	uchar  index;
	index = 0;
	FLASHCON = 0x01;
	while(index<len)
	{
		dat = src[index];
		SSP_Write(dest, dat);
		dest++;
		index++;
	}
	FLASHCON = 0x00;
}


/*****************************************************************************************************************************
 * Function:           Address_Mapping(uint route_item_index)
 * Input Parameter:    route_item_index       -路由表條目索引值 					 
 * OutPut Parameter:   None
 * Return Value:       返回route_item_index條目對應項的物理存儲地址
 * Description:        該函數用來獲取route_item_index條目對應項的物理存儲地址
 *****************************************************************************************************************************/
uint Address_Mapping(uchar route_item_index)
{
	return (route_item_index%ROUTE_ITEM_PER_PAGE)*sizeof(ROUTE_ITEM)+(route_item_index/ROUTE_ITEM_PER_PAGE)*PAGE_SIZE;					//physical index
}
   

uchar RouteTable_GetItem(uchar addr)        //得到特定netID對應的索引號
{
	uchar index = 0;
	uint physical_addr;
	uchar ID;
 	FLASHCON = 0x01;
	while(index<client_size)
	{
		physical_addr = Address_Mapping(index);
		ID = SSP_Read(physical_addr+6);	//CBYTE[physical_addr+6];
		if(ID==addr)
		{
			return index;
		}
		++index;
	}
	FLASHCON = 0x00;
	return 0xFF;
}


uchar RouteTable_GetItem_SN(uchar *client_SN)        //得到特定SN對應的索引號
{
	uchar index = 0;
	uint physical_addr;
	uchar client_SN_temp[6];
	FLASHCON = 0x01;
	while(index<client_size)
	{
		physical_addr = Address_Mapping(index);
		RouteTable_GetSN(index, client_SN_temp);
		if(!memcmp(client_SN_temp,client_SN,6))
		{
			return index;
		}
		++index;
	}
	FLASHCON = 0x00;
	return 0xFF;
}



void RouteTable_GetSN(uchar route_item_index, uchar* client_SN)   //從eeprom中讀取某一索引號對應的SN
{
	 uint data physical_addr;
	 physical_addr = Address_Mapping(route_item_index);

 	 FLASHCON = 0x01;
	 *client_SN     = SSP_Read(physical_addr);		//CBYTE[physical_addr];
	 *(client_SN+1) = SSP_Read(physical_addr+1);	//CBYTE[physical_addr+1];
	 *(client_SN+2) = SSP_Read(physical_addr+2);	//CBYTE[physical_addr+2];
	 *(client_SN+3) = SSP_Read(physical_addr+3);	//CBYTE[physical_addr+3];
	 *(client_SN+4) = SSP_Read(physical_addr+4);	//CBYTE[physical_addr+4];
	 *(client_SN+5) = SSP_Read(physical_addr+5);		//CBYTE[physical_addr+5];
	 FLASHCON = 0x00;
     return;
}


uchar RouteTable_GetID(uchar route_item_index)					//從eeprom中讀取某一索引號對應的netID
{
	 uint  physical_addr;
	 uchar ID;
	 physical_addr =  Address_Mapping(route_item_index) + 6;

 	 FLASHCON = 0x01;
	 ID  = SSP_Read(physical_addr);		//CBYTE[physical_addr];
	 FLASHCON = 0x00;   
	 return ID;
}



uchar RouteTable_GetRelay(uchar route_item_index)				//從eeprom中讀取某一索引號對應的中繼級數
{
	uint  physical_addr;
	uchar relay;

	physical_addr = Address_Mapping(route_item_index) + 7;
	FLASHCON = 0x01;
	relay = SSP_Read(physical_addr);	//CBYTE[physical_addr];
	FLASHCON = 0x00;
	return 	relay;
}



uchar RouteTable_GetRA(uchar route_item_index)					//從eeprom中讀取某一索引號對應的父節點
{
	uint   physical_addr;
	uchar  route;
	
	physical_addr = Address_Mapping(route_item_index) + 8;
	FLASHCON = 0x01;
	route = SSP_Read(physical_addr);	//CBYTE[physical_addr];
    FLASHCON = 0x00;
	return route;
}



uchar RouteTable_GetBACKUPRA(uchar route_item_index)			//從eeprom中讀取某一索引號對應的備用父節點
{
	uint  physical_addr;
	uchar  route;
	
	physical_addr = Address_Mapping(route_item_index) + 9;
	FLASHCON = 0x01;
	route = SSP_Read(physical_addr);	//CBYTE[physical_addr];
    FLASHCON = 0x00;
	return route;
}



uchar RouteTable_GetLink(uchar route_item_index)				//從eeprom中讀取某一索引號對應的Link信息
{
	uint  physical_addr;
	uchar link;
	
	physical_addr = Address_Mapping(route_item_index) + 10;
	FLASHCON = 0x01;
	link = SSP_Read(physical_addr);		//CBYTE[physical_addr];
    FLASHCON = 0x00;
	return link;
}




void RouteTable_LoadItem(uchar route_item_index, uchar* client_SN, uchar netID)      //第一次把表格加載進去
{	
     uint  physical_addr;
	 uchar src[7];
	 physical_addr = Address_Mapping(route_item_index);
     src[0] = *client_SN;
	 src[1] = *(client_SN+1);
	 src[2] = *(client_SN+2);
	 src[3] = *(client_SN+3);
	 src[4] = *(client_SN+4);
	 src[5] = *(client_SN+5);
	 src[6] = netID;
     WriteRouteTable(physical_addr, src, 7);
}



//更新路由表每一項路由信息
void RouteTable_UpDateItem(uchar route_item_index, uchar netID, uchar relay, uchar route, uchar backup_route, uchar link)  
{	
	uchar src[5];
	src[0] = netID;
	src[1] = relay;
	src[2] = route;
	src[3] = backup_route;
	src[4] = link;
	SSP_Erase(PAGE_BKUP);
	SSP_CopyPage(route_item_index/ROUTE_ITEM_PER_PAGE,PAGE_BKUP,0,(route_item_index%ROUTE_ITEM_PER_PAGE)*sizeof(ROUTE_ITEM)+5);
	WriteRouteTable((route_item_index%ROUTE_ITEM_PER_PAGE)*sizeof(ROUTE_ITEM)+PAGE_BKUP*PAGE_SIZE+6,src,5);
	SSP_CopyPage(route_item_index/ROUTE_ITEM_PER_PAGE,PAGE_BKUP,((route_item_index%ROUTE_ITEM_PER_PAGE)+1)*sizeof(ROUTE_ITEM),ROUTE_ITEM_PER_PAGE*sizeof(ROUTE_ITEM)-1);	
	SSP_Erase(route_item_index/ROUTE_ITEM_PER_PAGE);
	SSP_CopyPage(PAGE_BKUP,route_item_index/ROUTE_ITEM_PER_PAGE,0,ROUTE_ITEM_PER_PAGE*sizeof(ROUTE_ITEM)-1);
}


//更新路由表Link信息
void RouteTable_ChangeLink(uchar route_item_index,uchar link)  
{	
	uchar src[1];
	src[0] = link;
	SSP_Erase(PAGE_BKUP);
	SSP_CopyPage(route_item_index/ROUTE_ITEM_PER_PAGE,PAGE_BKUP,0,(route_item_index%ROUTE_ITEM_PER_PAGE)*sizeof(ROUTE_ITEM)+9);
	WriteRouteTable((route_item_index%ROUTE_ITEM_PER_PAGE)*sizeof(ROUTE_ITEM)+PAGE_BKUP*PAGE_SIZE+10,src,1);
	SSP_CopyPage(route_item_index/ROUTE_ITEM_PER_PAGE,PAGE_BKUP,((route_item_index%ROUTE_ITEM_PER_PAGE)+1)*sizeof(ROUTE_ITEM),ROUTE_ITEM_PER_PAGE*sizeof(ROUTE_ITEM)-1);	
	SSP_Erase(route_item_index/ROUTE_ITEM_PER_PAGE);
	SSP_CopyPage(PAGE_BKUP,route_item_index/ROUTE_ITEM_PER_PAGE,0,ROUTE_ITEM_PER_PAGE*sizeof(ROUTE_ITEM)-1);
}





uchar RouteTable_CMPSNNETID(uchar* client_SN, uchar netID)
{
	uchar index = 0;
	uchar netID_temp;
	uchar client_SN_temp[6];
	uchar status = 0;
	uchar route_temp;
	while(index<client_size)
	{
		RouteTable_GetSN(index, client_SN_temp); 
		netID_temp = RouteTable_GetID(index);
		route_temp = RouteTable_GetRA(index);
		if(!memcmp(client_SN_temp,client_SN,6))
			status |= 0X01;
		if(netID_temp==netID)
			status |= 0X02;
		++index;
	}
	return status;
}



uchar RouteTable_ROUTECHECK(uchar status,uchar netID,uchar route)
{
	uchar index = 0;
	uchar route_status = 3;
	uchar route_temp;
	uchar netID_temp;
	uchar link;
	if((status==3)||(status==1))
	{
		while(index<client_size)
		{
			netID_temp = RouteTable_GetID(index);
			route_temp = RouteTable_GetRA(index);
			link = RouteTable_GetLink(index);	
			if(netID_temp==netID)
			{
				if((route_temp==route)&&(link==1))
				{
					route_status = 2;
					break;
				}
			}
			index++;
		}
	}
	if(route_status!=2)
	{	
		if(route==0XFF)
			route_status = 1;
		else
		{
			index = 0;
			while(index<client_size)
			{
				netID_temp = RouteTable_GetID(index);	
				link = RouteTable_GetLink(index);
				if((netID_temp==route)&&(link==1))
				{
					route_status = 1;
					break;
				}	
				index++;
			}
		}
	}
	return route_status;
}




void RouteTable_ReadItem(uchar route_item_index, uchar *frame)      //第一次把表格加載進去
{	
	uchar index = 0; 
	uint  physical_addr;
	physical_addr = Address_Mapping(route_item_index);
	FLASHCON = 0x01;
	while(index<9)
	{
		frame[index] = SSP_Read(physical_addr+index);	//CBYTE[physical_addr+index];
		index++;
	}
	FLASHCON = 0x00;
	return;
}