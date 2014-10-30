#include "app_include/lib/crc.h"
#include "app_include/uplink/hdlc.h"
#include "plat_include/sys_config.h"

int UnparseDLMSPacket(int rxlen,unsigned char * rxbuf,HDLC_STRUCT *hdlc)
{
	if (rxlen>=10)
	{
		unsigned short F_type;
		unsigned char S_Flag;
		unsigned short P_Len;
		struct ADDR_4BYTE DestAddr;
		unsigned char SouceAddr;
		unsigned char Control_Code;
		unsigned short CS;
		unsigned char * pbuf = rxbuf;
		while((*pbuf != 0x7E)&&(rxlen > 0))
		{
			pbuf++;
			rxlen--;
			if (rxlen == 0)
			{
				return -1;
			}
		}	
		rxbuf = pbuf;
		pbuf++;
		F_type=*pbuf++;//frame type and length
		F_type <<= 8;
		F_type += *pbuf++;
		S_Flag=(F_type&0x0800)>>11;//得到S标志
		P_Len=F_type&0x07ff;//得到数据保文的长度

		if((*pbuf & 0x01) == 0x01)
		{
			DestAddr.HiAddr = (*pbuf)>>1;
			DestAddr.LoAddr = 0;
			DestAddr.type = 1; 
		}
		else if(((*pbuf & 0x01) == 0x00)&&((*(pbuf+1) & 0x01) == 0x01))
		{
			DestAddr.HiAddr = (*pbuf++) >> 1;
			DestAddr.LoAddr = (*pbuf++) >> 1;;
			DestAddr.type = 2;
		}else if(((*pbuf & 0x01) == 0x00)&&((*(pbuf+1) & 0x01) == 0x00)&&((*(pbuf+2) & 0x01) == 0x00)&&((*(pbuf+3) & 0x01) == 0x01))
		{
			DestAddr.HiAddr = (*pbuf++) >> 1;
			DestAddr.HiAddr <<= 7;
			DestAddr.HiAddr |= (*pbuf++) >> 1; 

			DestAddr.LoAddr = (*pbuf++) >> 1;
			DestAddr.LoAddr <<= 7;
			DestAddr.LoAddr |= (*pbuf++) >> 1;
			DestAddr.type = 4;
		}else
		{
			return -1;
		}
		SouceAddr = (*pbuf++) >> 1;
		Control_Code = *pbuf++;
		CS = *pbuf++;
		CS <<= 8;
		CS |= *pbuf++;
		if(pppfcs16(PPPINITFCS16,rxbuf+1,pbuf-rxbuf-1)!=PPPGOODFCS16)
		{
			return -1;//CRC Error!
		}
		if(*pbuf == 0x7E)
		{
			hdlc->Addr1=DestAddr;
			hdlc->Addr2=SouceAddr;
			hdlc->Control_Code=Control_Code;
			hdlc->S_Flag=S_Flag;
			hdlc->Info_length = 0;			
			return pbuf + 1 - rxbuf;
		}
		if(P_Len>rxlen)
			return -1;
		if ((P_Len-10)>0)
		{
			CS=rxbuf[P_Len-1];
			CS<<=8;
			CS=rxbuf[P_Len];
			if(pppfcs16(PPPINITFCS16,rxbuf+1,P_Len)!=PPPGOODFCS16)
			{
				return -1;//CRC Error!   
			}
			hdlc->Addr1=DestAddr;
			hdlc->Addr2=SouceAddr;
			hdlc->Control_Code=Control_Code;
			hdlc->S_Flag=S_Flag;
			hdlc->Info_length=P_Len-12;
			hdlc->Info_packer = &rxbuf[11];
			return P_Len+2;
		}
	}
	return -1;
}
int FindHdlcPcket(unsigned char* buf,int len)
{
	HDLC_STRUCT hdlc;
	return UnparseDLMSPacket(len,buf,&hdlc);	
}
#ifdef TEST_PLC
int MkGoodDLMSPacket(int rxlen,unsigned char * rxbuf)
{
	if (rxlen>=10)
	{
		unsigned short F_type;
		unsigned short P_Len;
		struct ADDR_4BYTE DestAddr;
		unsigned short CS;
		unsigned char * pbuf = rxbuf;
		while((*pbuf != 0x7E)&&(rxlen > 0))
		{
			pbuf++;
			rxlen--;
			if (rxlen == 0)
			{
				return -1;
			}
		}	
		rxbuf = pbuf;
		pbuf++;
		F_type=*pbuf++;//frame type and length
		F_type <<= 8;
		F_type += *pbuf++;

		if((*pbuf & 0x01) == 0x01)
		{
			DestAddr.HiAddr = (*pbuf)>>1;
			DestAddr.LoAddr = 0;
			DestAddr.type = 1; 
		}
		else if(((*pbuf & 0x01) == 0x00)&&((*(pbuf+1) & 0x01) == 0x01))
		{
			DestAddr.HiAddr = (*pbuf++) >> 1;
			DestAddr.LoAddr = (*pbuf++) >> 1;;
			DestAddr.type = 2;
		}else if(((*pbuf & 0x01) == 0x00)&&((*(pbuf+1) & 0x01) == 0x00)&&((*(pbuf+2) & 0x01) == 0x00)&&((*(pbuf+3) & 0x01) == 0x01))
		{
			DestAddr.HiAddr = (*pbuf++) >> 1;
			DestAddr.HiAddr <<= 7;
			DestAddr.HiAddr |= (*pbuf++) >> 1; 

			DestAddr.LoAddr = (*pbuf++) >> 1;
			DestAddr.LoAddr <<= 7;
			DestAddr.LoAddr |= (*pbuf++) >> 1;
			DestAddr.type = 4;
		}else
		{
			return -1;
		}
		*pbuf++;
		*pbuf++;
		CS = pppfcs16(PPPINITFCS16,rxbuf+1,pbuf-rxbuf-1);
		CS = ~CS;
		*pbuf++ = CS;
		CS >>= 8;
		*pbuf++ = CS;
		
		if(*pbuf == 0x7E)
		{
			return 0;
		}
		if(P_Len>rxlen)
			return -1;
		if ((P_Len-10)>0)
		{
			if(pppfcs16(PPPINITFCS16,rxbuf+1,P_Len-2)!=PPPGOODFCS16)
			{
				return -1;//CRC Error!   
			}
			rxbuf[P_Len-1] = CS;
			CS >>= 8;
			rxbuf[P_Len] = CS;

			return P_Len+2;
		}
	}
	return 0;
}
#endif
