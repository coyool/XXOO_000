/**
* app_proto.h -- 载波应用协议接口
* 
* 作者: hulijun
* 创建时间: 2012-2-27
* 最后修改时间: 2012-2-27
*/

#ifndef _APP_PROTO_H
#define _APP_PROTO_H

typedef struct {
	unsigned char protoid;
	int (*makeread)(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
	int (*makewrite)(const unsigned char *addr, const struct plwrite_config_t *pconfig, unsigned char *buf, int len);
	int (*makechktime)(const sysclock_t *pclock, unsigned char *buf, int len);
	int (*checkread)(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len);
	int (*checkwrite)(const unsigned char *addr, unsigned short itemid, const unsigned char *buf, int len);
} usrmet_prot_t;

const usrmet_prot_t *GetUserMetProto(unsigned char proto);

#endif /*_APP_PROTO_H*/

