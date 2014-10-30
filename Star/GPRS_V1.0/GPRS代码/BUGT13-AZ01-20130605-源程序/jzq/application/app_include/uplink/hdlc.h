#ifndef _HDLC_H_
#define _HDLC_H_

struct ADDR_4BYTE
{
	unsigned char type;	//1=1�ֽڵ�ַ,2=2�ֽڵ�ַ,4=4�ֽڵ�ַ,�����Ķ��Ǵ����
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
����DLMS����֡
@ret
0 ����ʧ��
>0 ���ҳɹ�
*/
int FindHdlcPcket(unsigned char* buf,int len);
#endif
