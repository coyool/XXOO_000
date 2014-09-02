#ifndef __PLATPARA_H
#define __PLATPARA_H


struct platform_para
{
	unsigned char printport;	//打印口配置
	unsigned char ipaddr[4];
	unsigned char ethaddr[6];
//	unsigned char rev;	//调试进用来使参数校验错误重新设置
};

#define PLATPARA_PRINTPORT	0
#define PLATPARA_IPADDR		1
#define PLATPARA_ETHADDR 	2

extern struct platform_para platpara;

void platpara_init(void);
int platpara_read(unsigned char type,void *pbuf,unsigned int len);
int platpara_write(unsigned char type,const void *pbuf,unsigned int len);
#endif
