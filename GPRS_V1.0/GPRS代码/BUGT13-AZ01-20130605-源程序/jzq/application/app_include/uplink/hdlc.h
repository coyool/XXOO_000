#ifndef _HDLC_H_
#define _HDLC_H_

struct ADDR_4BYTE
{
	unsigned char type;	//1=1字节地址,2=2字节地址,4=4字节地址,其他的都是错误的
	unsigned short HiAddr;
	unsigned short LoAddr;
};
typedef struct{
	struct ADDR_4BYTE Addr1;
	unsigned char Addr2;
	unsigned char S_Flag;
	unsigned char Control_Code;
	unsigned short Info_length;
	unsigned char *Info_packer;
}HDLC_STRUCT;	  
#define PROTO_376	1
#define PROTO_DLMS	2

int UnparseDLMSPacket(int rxlen,unsigned char * rxbuf,HDLC_STRUCT *hdlc);
/*
查找DLMS数据帧
@ret
0 查找失败
>0 查找成功
*/
int FindHdlcPcket(unsigned char* buf,int len);
#endif
