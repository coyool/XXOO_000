/******************** *****(C) COPYRIGHT 2010 LOKEE ************************
* File Name :  metproto.c
* Author : 
* Version : 
* Date : 
* Description :表规约库接口
*******************************************************************************
* 
*******************************************************************************/
#include <string.h>
#include "app_include/cenmet/metproto.h"

const metfunc_t metfunc_list[] = {
	{1, 0, 0, 0, 1200, 8, 1, 'E', dl645_read, NULL, dl645_get_itemid}, //DL/T645-1997部颁规约(已测试)
	{30, 0, 0, 0, 2400, 8, 1, 'E', dl645_2007_read, NULL, dl645_2007_get_itemid},//DL/T645-2007(已测试)
	{0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL},
};

int get_metfunc(UCHAR proto_id, UCHAR functype, metfunc_t **ppfunc)
{
	int i;
	
	for(i=0; metfunc_list[i].id != 0; i++) {
		if(metfunc_list[i].id == proto_id) {
			switch(functype) {
			case FUNCTYPE_READ:
				if(NULL == metfunc_list[i].read) return 1;
				break;

			case FUNCTYPE_SET:
				if(NULL == metfunc_list[i].set) return 1;
				break;

			default: return 1;
			}

			*ppfunc = (metfunc_t *)&metfunc_list[i];
			return 0;
		}
	}

	return 1;
}


