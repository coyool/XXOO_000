/*********************************************************************
 *
 *               RouteTable Access Routines Definitions
 *
 *********************************************************************
 * FileName:        SSP.h
 * Dependencies:   
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
 * Ellie Fan            2010/3/23	   Original                                
 ********************************************************************/

 
//路由表存儲在類EEROM塊區中(大小：8*256, 8個扇區，每個扇區256Byte）

#ifndef _ROUTETABLE_H_
#define _ROUTETABLE_H_
	
#define PAGES      8        
#define PAGE_BKUP  7   

#define ROUTE_ITEM_PER_PAGE (PAGE_SIZE/sizeof(ROUTE_ITEM))         //每個扇區的路由表數目

void InitRouteTable();
uchar RouteTable_GetItem(uchar addr);  
uchar RouteTable_GetItem_SN(uchar *client_SN);
void  RouteTable_GetSN(uchar route_item_index, uchar* client_SN);
uchar RouteTable_GetID(uchar route_item_index);
uchar RouteTable_GetRelay(uchar route_item_index);
uchar RouteTable_GetRA(uchar route_item_index);
uchar RouteTable_GetBACKUPRA(uchar route_item_index);  
uchar RouteTable_GetLink(uchar route_item_index);
uchar RouteTable_CMPSNNETID(uchar* client_SN, uchar netID);
uchar RouteTable_ROUTECHECK(uchar status,uchar netID, uchar route);

void RouteTable_LoadItem(uchar route_item_index, uchar* client_SN, uchar netID);
void RouteTable_UpDateItem(uchar route_item_index, uchar netID, uchar relay, uchar route, uchar backup_route, uchar link);
void RouteTable_ChangeLink(uchar route_item_index,uchar link);
void RouteTable_ReadItem(uchar route_item_index, uchar *frame);     //第一次把表格加載進去
 
#endif
