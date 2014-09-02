/**
* metp.c -- ���������
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-7
* ����޸�ʱ��: 2009-5-7
*/

//#include <stdio.h>
#include <string.h>

#define DEFINE_PARAMETP

#include "plat_include/debug.h"
#include "app_include/param/metp.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
#include "app_include/param/meter.h"
#include "app_include/lib/bcd.h"


metp_map_t MetpMap[MAX_METP];

#define para_metp_buf para_rwbuf 

#if 0
/**
* @brief ���ļ�������๦�ܲ��������
* @return 0�ɹ�, ����ʧ��
*/
static int LoadParaCenMetp(para_cenmetp_t *p)
{
	int rt;
	
	rt = para_readflash_bin(PARA_FILEINDEX_METP,(unsigned char *)p,sizeof(para_cenmetp_t));
	AssertLog(rt,"readflash para_metp failed!\n");

	return rt;
}

/**
* @brief ����๦�ܲ��������
* @return 0�ɹ�, ����ʧ��
*/
static int SaveParaCenMetp(para_cenmetp_t *p)
{
	int rt;
	
	rt = para_writeflash_bin(PARA_FILEINDEX_METP,(unsigned char *)p,sizeof(para_cenmetp_t));
	AssertLog(rt,"writeflash para_metp failed!\n");
 
	return rt;
}
#endif

/**
* @brief �ն˲���F25����
* @param flag ������ʽ, 0-��, 1-д
* @param metpid �������
* @param buf ������ָ��
* @param len ����������
* @param actlen ��Ч���ݳ���(�ɺ�������)
* @return 0�ɹ�, ����ʧ��(�ο�POERR_XXX�궨��)
* @note ����ͬ������ͷ���ֵ��ͬ, �����ظ�ע��
*/
int ParaOperationF25(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	unsigned short mid = metpid - 1;
	para_cenmetp_t *ParaCenMetp = (para_cenmetp_t *)para_metp_buf;

	para_readflash_bin(PARA_FILEINDEX_METP,(unsigned char *)ParaCenMetp,sizeof(para_cenmetp_t));
	
	if(0 == flag) {
		if(mid >= MAX_CENMETP) {
			return 1;
		}
		else {
			buf[0] = ParaCenMetp[mid].base.pt & 0xff;
			buf[1] = ParaCenMetp[mid].base.pt >> 8;
			buf[2] = ParaCenMetp[mid].base.ct & 0xff;
			buf[3] = ParaCenMetp[mid].base.ct >> 8;
			UnsignedToBcd(ParaCenMetp[mid].base.vol_rating, &buf[4], 2);
			UnsignedToBcd(ParaCenMetp[mid].base.amp_rating/10, &buf[6], 1);
			UnsignedToBcd(ParaCenMetp[mid].base.pwr_rating, &buf[7], 3);
			buf[10] = ((ParaCenMetp[mid].base.pwrphase&0x03)<<2) + (ParaCenMetp[mid].base.pwrtype&0x03);
		}
	}
	else {
		if(mid >= MAX_CENMETP) {
			return 1;
		}
		else {
			SetSaveParamFlag(SAVEFLAG_METP);

			ParaCenMetp[mid].base.pt = ((unsigned short)buf[1]<<8) + ((unsigned short)buf[0]);
			ParaCenMetp[mid].base.ct = ((unsigned short)buf[3]<<8) + ((unsigned short)buf[2]);
			ParaCenMetp[mid].base.vol_rating = BcdToUnsigned(&buf[4], 2);
			ParaCenMetp[mid].base.amp_rating = BcdToUnsigned(&buf[6], 1) * 10;
			ParaCenMetp[mid].base.pwr_rating = BcdToUnsigned(&buf[7], 3);
			ParaCenMetp[mid].base.pwrtype = buf[10] & 0x03;
			ParaCenMetp[mid].base.pwrphase = (buf[10]>>2)&0x03;
			
			para_writeflash_bin(PARA_FILEINDEX_METP,(unsigned char *)ParaCenMetp,sizeof(para_cenmetp_t));
		}
	}

	return 0;
}


/**
* @brief �ն˲���F26����(��������ֵ����)
*/
int ParaOperationF26(int flag, unsigned short metpid, unsigned char *buf, int len, int *actlen)
{
	unsigned short mid = metpid - 1;
	unsigned char *puc = buf;
	para_cenmetp_t *ParaCenMetp = (para_cenmetp_t *)para_metp_buf;

	para_readflash_bin(PARA_FILEINDEX_METP,(unsigned char *)ParaCenMetp,sizeof(para_cenmetp_t));

	if(0 == flag) {
		if(mid >= MAX_CENMETP) {
			return 1;
		}
		else {
			UnsignedToBcd(ParaCenMetp[mid].limit.volok_up, puc, 2); puc += 2;
			UnsignedToBcd(ParaCenMetp[mid].limit.volok_low, puc, 2); puc += 2;
			UnsignedToBcd(ParaCenMetp[mid].limit.vol_lack, puc, 2); puc += 2;

			UnsignedToBcd(ParaCenMetp[mid].limit.vol_over, puc, 2); puc += 2;
			*puc++ = ParaCenMetp[mid].limit.time_volover;
			*puc++ = ParaCenMetp[mid].limit.resr_volover[0];
			*puc++ = ParaCenMetp[mid].limit.resr_volover[1];

			UnsignedToBcd(ParaCenMetp[mid].limit.vol_less, puc, 2); puc += 2;
			*puc++ = ParaCenMetp[mid].limit.time_volless;
			*puc++ = ParaCenMetp[mid].limit.resr_volless[0];
			*puc++ = ParaCenMetp[mid].limit.resr_volless[1];

			UnsignedToBcd(ParaCenMetp[mid].limit.amp_over, puc, 3); puc += 3;
			*puc++ = ParaCenMetp[mid].limit.time_ampover;
			*puc++ = ParaCenMetp[mid].limit.resr_ampover[0];
			*puc++ = ParaCenMetp[mid].limit.resr_ampover[1];

			UnsignedToBcd(ParaCenMetp[mid].limit.amp_limit, puc, 3); puc += 3;
			*puc++ = ParaCenMetp[mid].limit.time_amplimit;
			*puc++ = ParaCenMetp[mid].limit.resr_amplimit[0];
			*puc++ = ParaCenMetp[mid].limit.resr_amplimit[1];

			UnsignedToBcd(ParaCenMetp[mid].limit.zamp_limit, puc, 3); puc += 3;
			*puc++ = ParaCenMetp[mid].limit.time_zamp;
			*puc++ = ParaCenMetp[mid].limit.resr_zamp[0];
			*puc++ = ParaCenMetp[mid].limit.resr_zamp[1];

			UnsignedToBcd(ParaCenMetp[mid].limit.pwr_over, puc, 3); puc += 3;
			*puc++ = ParaCenMetp[mid].limit.time_pwrover;
			*puc++ = ParaCenMetp[mid].limit.resr_pwrover[0];
			*puc++ = ParaCenMetp[mid].limit.resr_pwrover[1];

			UnsignedToBcd(ParaCenMetp[mid].limit.pwr_limit, puc, 3); puc += 3;
			*puc++ = ParaCenMetp[mid].limit.time_pwrlimit;
			*puc++ = ParaCenMetp[mid].limit.resr_pwrlimit[0];
			*puc++ = ParaCenMetp[mid].limit.resr_pwrlimit[1];

			UnsignedToBcd(ParaCenMetp[mid].limit.vol_unb, puc, 2); puc += 2;
			*puc++ = ParaCenMetp[mid].limit.time_volunb;
			*puc++ = ParaCenMetp[mid].limit.resr_volunb[0];
			*puc++ = ParaCenMetp[mid].limit.resr_volunb[1];

			UnsignedToBcd(ParaCenMetp[mid].limit.amp_unb, puc, 2); puc += 2;
			*puc++ = ParaCenMetp[mid].limit.time_ampunb;
			*puc++ = ParaCenMetp[mid].limit.resr_ampunb[0];
			*puc++ = ParaCenMetp[mid].limit.resr_ampunb[1];

			*puc = ParaCenMetp[mid].limit.time_volless_2;			
		}
	}
	else {
		if(mid < MAX_CENMETP) {
			ParaCenMetp[mid].limit.volok_up = BcdToUnsigned(puc, 2); puc += 2;
			ParaCenMetp[mid].limit.volok_low = BcdToUnsigned(puc, 2); puc += 2;
			ParaCenMetp[mid].limit.vol_lack = BcdToUnsigned(puc, 2); puc += 2;

			ParaCenMetp[mid].limit.vol_over = BcdToUnsigned(puc, 2); puc += 2;
			ParaCenMetp[mid].limit.time_volover = *puc++;
			ParaCenMetp[mid].limit.resr_volover[0] = puc[0];
			ParaCenMetp[mid].limit.resr_volover[1] = puc[1];
			puc += 2;

			ParaCenMetp[mid].limit.vol_less = BcdToUnsigned(puc, 2); puc += 2;
			ParaCenMetp[mid].limit.time_volless = *puc++;
			ParaCenMetp[mid].limit.resr_volless[0] = puc[0];
			ParaCenMetp[mid].limit.resr_volless[1] = puc[1];
			puc += 2;

			puc[2] &= 0x7f;
			ParaCenMetp[mid].limit.amp_over = BcdToUnsigned(puc, 3); puc += 3;
			ParaCenMetp[mid].limit.time_ampover = *puc++;
			ParaCenMetp[mid].limit.resr_ampover[0] = puc[0];
			ParaCenMetp[mid].limit.resr_ampover[1] = puc[1];
			puc += 2;

			puc[2] &= 0x7f;
			ParaCenMetp[mid].limit.amp_limit = BcdToUnsigned(puc, 3); puc += 3;
			ParaCenMetp[mid].limit.time_amplimit = *puc++;
			ParaCenMetp[mid].limit.resr_amplimit[0] = puc[0];
			ParaCenMetp[mid].limit.resr_amplimit[1] = puc[1];
			puc += 2;

			puc[2] &= 0x7f;
			ParaCenMetp[mid].limit.zamp_limit = BcdToUnsigned(puc, 3); puc += 3;
			ParaCenMetp[mid].limit.time_zamp = *puc++;
			ParaCenMetp[mid].limit.resr_zamp[0] = puc[0];
			ParaCenMetp[mid].limit.resr_zamp[1] = puc[1];
			puc += 2;

			ParaCenMetp[mid].limit.pwr_over = BcdToUnsigned(puc, 3); puc += 3;
			ParaCenMetp[mid].limit.time_pwrover = *puc++;
			ParaCenMetp[mid].limit.resr_pwrover[0] = puc[0];
			ParaCenMetp[mid].limit.resr_pwrover[1] = puc[1];
			puc += 2;

			ParaCenMetp[mid].limit.pwr_limit = BcdToUnsigned(puc, 3); puc += 3;
			ParaCenMetp[mid].limit.time_pwrlimit = *puc++;
			ParaCenMetp[mid].limit.resr_pwrlimit[0] = puc[0];
			ParaCenMetp[mid].limit.resr_pwrlimit[1] = puc[1];
			puc += 2;

			ParaCenMetp[mid].limit.vol_unb = BcdToUnsigned(puc, 2); puc += 2;
			ParaCenMetp[mid].limit.time_volunb = *puc++;
			ParaCenMetp[mid].limit.resr_volunb[0] = puc[0];
			ParaCenMetp[mid].limit.resr_volunb[1] = puc[1];
			puc += 2;

			ParaCenMetp[mid].limit.amp_unb = BcdToUnsigned(puc, 2); puc += 2;
			ParaCenMetp[mid].limit.time_ampunb = *puc++;
			ParaCenMetp[mid].limit.resr_ampunb[0] = puc[0];
			ParaCenMetp[mid].limit.resr_ampunb[1] = puc[1];
			puc += 2;

			ParaCenMetp[mid].limit.time_volless_2 = *puc;
			
			para_writeflash_bin(PARA_FILEINDEX_METP,(unsigned char *)ParaCenMetp,sizeof(para_cenmetp_t));
			
		}
	}

	return 0;
}


