/**
* plc_proto.c -- 载波应用协议接口
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-21
* 最后修改时间: 2009-5-21
*/

#include <stdio.h>
#include <string.h>

#include "plat_include/plcomm.h"
#include "plat_include/app_proto.h"
#include "plat_include/dl645_1997.h"
#include "plat_include/dl645_2007.h"

static const usrmet_prot_t PlcProtoList[] = {
	{1, Dl1997MakeReadPkt, Dl1997MakeWritePkt, Dl1997MakeChkTimePkt, Dl1997CheckRead, Dl1997CheckWrite},
	{2, Dl2007MakeReadPkt, Dl2007MakeWritePkt, Dl2007MakeChkTimePkt, Dl2007CheckRead, Dl2007CheckWrite},
};
#define NUM_LIST	(sizeof(PlcProtoList)/sizeof(PlcProtoList[0]))

const usrmet_prot_t *GetUserMetProto(unsigned char proto)
{
	int i;

	for(i=0; i<NUM_LIST; i++) {
		if(PlcProtoList[i].protoid == proto) return &PlcProtoList[i];
	}

	return NULL;
}
