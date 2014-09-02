/**
* meter.c -- ��Ʋ���
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-7
* ����޸�ʱ��: 2009-5-7
*/

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "plat_include/debug.h"
#include "app_include/param/meter.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
//#include "app_include/param/metp.h"
#include "app_include/cenmet/cenmet_comm.h"
#include "plat_include/flashsave.h"
#include "app_include/cenmet/forward.h"
#include "plat_include/dl645_2007.h"
#include "plat_include/sys_config.h"
#include "app_include/lib/bcd.h"
 #include "plat_include/shellcmd.h"


simple_para_meter_t ParaMeter[MAX_METER];
#define METER_PARAM_MAGIC	0x872563aa
extern void DUseClsUpdata(void);
/**
* @brief ӳ��������Ӧ��ź�����
*/
#if 0
void MappingMetp(void)
{
	unsigned short id, metpid;
	
	memset(MetpMap, 0xff, sizeof(MetpMap));

	for(id=0; id<MAX_METER; id++) {
		if(EMPTY_METER(id)) continue;

		metpid = ParaMeter[id].metp_id;
		metpid -= 1;
		//MetpMap[metpid].type = METP_METER;
		MetpMap[metpid].metid = id;
	}
}
#endif
static int shell_PrintMeter(int argc,char* argv[])
{
	int id;
	if(argc < 2)
		return 1;
	id = atoi(argv[1]);
	PrintLog(LOGTYPE_DEBUG,"������  : %d\r\n",id);
	PrintLog(LOGTYPE_DEBUG,"�������  : %d\r\n",ParaMeter[id].metp_id);
	PrintLog(LOGTYPE_DEBUG,"�������ַ: %02x%02x%02x-%02x%02x%02x\r\n",
	ParaMeter[id].addr[0],ParaMeter[id].addr[1],ParaMeter[id].addr[2],
	ParaMeter[id].addr[3],ParaMeter[id].addr[4],ParaMeter[id].addr[5]);

	return 0;	
}
DECLARE_SHELL_CMD("pmetet", shell_PrintMeter, "��ӡ���");

/**
* @brief ���ļ��������Ʋ���
* @return 0�ɹ�, ����ʧ��
*/
int LoadParaMeter(void)
{
	int rt;
	
	rt = para_readflash_bin(PARA_FILEINDEX_METER,(unsigned char *)ParaMeter,sizeof(ParaMeter));
	AssertLog(rt==-1,"readflash para_meter failed!\n");
	INIT_SHELL_CMD(shell_PrintMeter);
	return rt;
}

/**
* @brief �����Ʋ���
* @return 0�ɹ�, ����ʧ��
*/
int SaveParaMeter(void)
{
	int rt;
	
	rt = para_writeflash_bin(PARA_FILEINDEX_METER,(unsigned char *)ParaMeter,sizeof(ParaMeter));
	AssertLog(rt==-1,"writeflash para_meter failed!\n");

	return rt;
}

/**
* @brief �ն˲���F10����(�ն˵��ܱ�/��������װ�����ò���)
* @param flag ������ʽ, 0-��, 1-д
* @param option ����ѡ��
* @return 0�ɹ�, ����ʧ��(�ο�POERR_XXX�궨��)
*/
typedef struct {
	unsigned short index;  //���������,������, �����ط�����
	unsigned short metp_id;    //�����������, 1~2040, 0��ʾ��Ч
	unsigned char port;    //ͨ�Ŷ˿ں�
	unsigned char proto;    //ͨ�Ź�Լ����, 1-DL645(1997), 2-��������, 30-DL645(2007)
	unsigned char addr[6];    //ͨ�ŵ�ַ
	unsigned char pwd[6];    //ͨ������
	unsigned char prdnum;    //���ܷ��ʸ���
	unsigned char intdotnum;    //�й�����ʾֵ����λ��С��λ����
	unsigned char owneraddr[6];    //�����ɼ�����ַ
	unsigned char usrclass;    //�û�����ź͵������
	unsigned char speed;  //ͨ������
} tm_para_meter_t;
static tm_para_meter_t tmp_meter;
int ParaOperationF10(int flag, unsigned short metpid, param_specialop_t *option)
{
	(void)metpid;
	if(0 == flag) {
		unsigned short num, i, metid, actnum;
		const unsigned char *prbuf;
		unsigned char *pwbuf;
		unsigned short wlen;

		num = ((unsigned short)option->rbuf[1]<<8) + ((unsigned short)option->rbuf[0]&0xff);
		option->ractlen = ((int)num<<1) + 2;
		if(option->rlen < option->ractlen) return POERR_FATAL;

		wlen = 2;
		prbuf = option->rbuf + 2;
		pwbuf = option->wbuf + 2;
		actnum = 0;
		for(i=0; i<num; i++,prbuf+=2) {
			metid = ((unsigned short)prbuf[1]<<8) + ((unsigned short)prbuf[0]&0xff);
			if(metid == 0 || metid > MAX_METER) continue;
			
			if((wlen+27) > option->wlen) break;
			metid -= 1;

			*pwbuf++ = prbuf[0];
			*pwbuf++ = prbuf[1];
			memset(&tmp_meter,0,sizeof(tmp_meter));
			tmp_meter.metp_id = ParaMeter[metid].metp_id;
			memcpy(tmp_meter.addr, ParaMeter[metid].addr,6);
			memcpy(pwbuf, &tmp_meter.metp_id, 25);
			pwbuf[2] &= 0x1f;
			pwbuf[2] |= tmp_meter.speed << 5;
			pwbuf += 25;
			wlen += 27;
			actnum++;
		}

		option->wactlen = wlen;
		option->wbuf[0] = actnum;
		option->wbuf[1] = actnum>>8;
	}
	else
	{
		unsigned short num, i, metid, metpid;
		const unsigned char *prbuf;
		int allfail = 1;
//		unsigned char portid, proto;
		int mettype = 0; // 1 - �ܱ� 3 - �ñ�ɾ
		
		num = ((unsigned short)option->rbuf[1]<<8) + ((unsigned short)option->rbuf[0]&0xff);
		option->ractlen = (int)num*27 + 2;		
		if(option->rlen < option->ractlen) return POERR_FATAL;

		prbuf = option->rbuf + 2;
		for(i=0; i<num; i++) {
			metid = ((unsigned short)prbuf[1]<<8) + ((unsigned short)prbuf[0]&0xff);
			metpid = ((unsigned short)prbuf[3]<<8) + ((unsigned short)prbuf[2]&0xff);
			if(metid == 0 || metid > MAX_METER) goto mark_skip;
		
//			portid = prbuf[4] & 0x1f;
//			proto = prbuf[5];

			if(metpid != 0) {

				if(metid > MAX_CENMETP) {
					goto mark_skip;
				}
				else {//metid <= MAX_CENMETP
					/*if(portid == (COMMPORT_CEN_485_1 + 1) || portid == (COMMPORT_CEN_485_2 + 1)) {  // 485�ܱ�
						if(proto == METTYPE_ACSAMP || proto == METTYPE_PLC) goto mark_skip;	 */
						allfail = 0;
						mettype = 0;
				/*	}
					else goto mark_skip;*/
				}
			}
			else {
				allfail = 0;
				mettype = 3;
			}
			
			metid -= 1;
			prbuf += 2;

			if(1 == mettype){ // �ܱ�
				if((metpid != ParaMeter[metid].metp_id) || /*(proto != ParaMeter[metid].proto) ||*/
					/*(portid != ParaMeter[metid].port) ||*/ (memcmp(&ParaMeter[metid].addr, &prbuf[4], 6) != 0)){

				}
			}
			else if(3 == mettype){ // ��ɾ���ı�, ԭ��Ϊ�ܱ�

			}
			mettype = 0;
			
			memcpy(&tmp_meter.metp_id, prbuf, 25); prbuf += 25;
			ParaMeter[metid].metp_id = tmp_meter.metp_id;
			memcpy(ParaMeter[metid].addr, tmp_meter.addr,6);
#ifdef METER_MODULE
			memcpy(&ParaMeter[metid].prdnum,&tmp_meter.prdnum,8);	
			BcdToString(&ParaMeter[metid].prdnum,8,ParaUni.manuno);
#endif
			//ParaMeter[metid].speed = ParaMeter[metid].port >> 5;
			//ParaMeter[metid].port &= 0x1f;
			{//�޸ĵ����
				unsigned int idAddress = 0x040004E0;
				unsigned char data_buf[12]={0x01,0x11,0x11,0x11,0x00,0x00,0x00,0x00};
				unsigned char* AddrBuf = data_buf+8;
				struct data_t cmd;
			 	struct data_t echo;
				unsigned char sbuf[30]; 
				unsigned char rbuf[30];
				unsigned int hdlcid = tmp_meter.metp_id | (0x0001<<16);

				memcpy(AddrBuf,&hdlcid,4);
				
				cmd.data = sbuf;
				echo.data = rbuf; 
				echo.len = sizeof(rbuf);
				cmd.len = Dl2007MakeWritePktRaw(tmp_meter.addr,idAddress,data_buf,sizeof(data_buf),sbuf);
				#ifdef DEFINE_PLCOMM
				PrintLog(LOGTYPE_DEBUG,"METER Update\r\n");
				PrintHexLog(LOGTYPE_DEBUG,cmd.data,cmd.len);
				PrintLog(LOGTYPE_DEBUG,"\r\n");
				PlForwardF1(&cmd, &echo,2);//͸��
				PrintLog(LOGTYPE_DEBUG,"METER Reciive\r\n");
				PrintHexLog(LOGTYPE_DEBUG,echo.data,echo.len);
				PrintLog(LOGTYPE_DEBUG,"\r\n");
				#elif defined(DEFINE_SERIAL)
					CenMetForward(&cmd, &echo);
				#else
					#error "error!"
				#endif
			}	

			continue;
		mark_skip:
			prbuf += 27;
		}

		if(allfail) return POERR_INVALID;
#ifdef ALARM_MODULE
		MappingMetp();

		DUseClsUpdata();
#endif
		SaveParaMeter();
	}

	return 0;
}



