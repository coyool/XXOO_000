/**
* tranfile.c -- 文件传输处理
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-6-9
* 最后修改时间: 2009-6-9
*/

#include <string.h>
#include <stdlib.h>

#include "plat_include/debug.h"
#include "plat_include/timeal.h"
#include "plat_include/timer.h"
#include "app_include/lib/align.h"
#include "app_include/lib/crc.h"
#include "plat_include//reset.h"
#include "app_include/uplink/uplink_pkt.h"
#include "app_include/uplink/uplink_dl.h"
//#include "terminfo.h"
#include "app_include/uplink/svrcomm.h"
//#include "tranfile_active.h"
#include "app_include/uplink/svrmsg.h"
#include "app_include/sys/store_iap.h"
#include "app_include/sys/iap_save.h"



#define DWNFLAG_WORKFILE	1   //下载标准程序文件
#define DWNFLAG_RAWFILE		2   //下载普通文件
#define DWNFLAG_RAWFILE_ZIP		3  //下载压缩普通文件

//extern void __GlobalLock(void);
//extern void __GlobalUnlock(void);


struct dwnfile_ctrl {
	unsigned char flag;

	int timerid;
	unsigned char *pmem;
	//FILE *pfcache;
	unsigned int lastlen;

	unsigned short serial;
	unsigned short snmax;
	unsigned int mask;
	unsigned int full_mask;
	unsigned int filesize;
	unsigned short filecrc;

	unsigned short blksize;
	unsigned char winsize;
	unsigned char fileid;
	unsigned char blast;

	unsigned char svr_addr;

	//char filename[64];
};
static struct dwnfile_ctrl DownFileCtrl;

#if 0
struct upfile_ctrl {
	int timerid;
	unsigned char *pmem;
	FILE *pf;

	unsigned int memsize;

	unsigned short blksize;
	unsigned short serial;
};
static struct upfile_ctrl UpFileCtrl;
#endif

#define FILECODE_REQ_DWNWORK	0   //请求下载标准文件
#define FILECODE_REQ_DWNRAW		1   //请求下载普通文件
#define FILECODE_REQ_UPRAW		2   //请求上传普通文件
#define FILECODE_DWNDATA		3  //下载文件数据及回应
#define FILECODE_UPDATA			4  //上传文件数据及回应
#define FILECODE_REJECT			5  //否认响应
#define FILECODE_INFO			6  //发送文件信息(断点续传用)
#define FILECODE_REQ_DWNRAW_ZIP		7   //请求下载普通文件(压缩格式)
#define FILECODE_REQ_UPRAW_ZIP		8   //请求上传普通文件(压缩格式)
#define FILECODE_REQ_DWN_ACTIVE		9
#define FILECODE_DWNACT_DATA		10
#define FILECODE_MAX			11

#define TRANFILE_REQ_SN    0xb6589031
#define FILEID_PROTO    0
#define FILEID_PROGRAM    1
#define FILEID_DRIVER    2
#define FILEID_PROTO_XZIP    3
#define FILEID_PROGRAM_XZIP    4
#define FILEID_DRIVER_XZIP    5
#define FILE_XZIP_MAGIC	3699

#define LEN_REQ_DWNWORK		sizeof(struct req_dwnwork)
//23
struct req_dwnwork {
	unsigned char duid[4];
	unsigned char code;
	unsigned char req[4];
	unsigned char size[4];
	unsigned char fileid;
	unsigned char filecrc[2];
	unsigned char blksize[2];
	unsigned char timeout[4];
	unsigned char winsize; //<=32
};

#define LEN_REQ_DWNRAW		sizeof(struct req_dwnraw)
//22
struct req_dwnraw {
	unsigned char duid[4];
	unsigned char code;
	unsigned char req[4];
	unsigned char size[4];
	unsigned char filecrc[2];
	unsigned char blksize[2];
	unsigned char timeout[4];
	unsigned char winsize; //<=32
	char filename[1];
};

#define LEN_REQ_UPRAW		sizeof(struct req_upraw)
//21
struct req_upraw {
	unsigned char duid[4];
	unsigned char code;
	unsigned char req[4];
	unsigned char size[4];
	unsigned char filecrc[2];
	unsigned char blksize[2];
	unsigned char timeout[4];
	char filename[1];
};

#define LEN_DWNDATA_HEAD	sizeof(struct down_data)
//9
struct down_data {
	unsigned char duid[4];
	unsigned char code;
	unsigned char serial[2];
	unsigned char crc[2];
	unsigned char data[0];
};

struct dfile_info {
	unsigned char duid[4];
	unsigned char code;
	unsigned char filesize[4];
	unsigned char filecrc[2];
	unsigned char serial[2];
	unsigned char mask[4];
	unsigned char blksize[2];
	unsigned char fileid;
	unsigned char winsize;
	unsigned char unuse;
};

/**
* @brief 清除下载文件控制
*/
static void ClearDownFileCtrl(void)
{
	DownFileCtrl.flag = 0;

	if(DownFileCtrl.timerid >= 0) {
		SysStopCTimer(DownFileCtrl.timerid);
		DownFileCtrl.timerid = -1;
	}

	if(NULL != DownFileCtrl.pmem) {
	//	free(DownFileCtrl.pmem);
		DownFileCtrl.pmem = NULL;
	}

	//if(NULL != DownFileCtrl.pfcache) {
	//	fclose(DownFileCtrl.pfcache);
	//	DownFileCtrl.pfcache = NULL;
//	}

//	remove(DWNFILE_CACHE);
}

#if 0

/**
* @brief 清除上传文件控制
*/
static inline void ClearUpFileCtrl(void)
{
	if(UpFileCtrl.timerid >= 0) {
		SysStopCTimer(UpFileCtrl.timerid);
		UpFileCtrl.timerid = -1;
	}

	if(NULL != UpFileCtrl.pmem) {
		free(UpFileCtrl.pmem);
		UpFileCtrl.pmem = NULL;
	}

	if(NULL != UpFileCtrl.pf) {
		fclose(UpFileCtrl.pf);
		UpFileCtrl.pf = NULL;
	}

	remove(ZIP_TEMPFILE_UP);
}

#endif

/**
* @brief 下载文件超时定时器
*/
static int CTimerDownFile(unsigned long arg)
{
	PrintLog(LOGTYPE_ALARM, "down file timeout\n");

	DownFileCtrl.timerid = -1;
	ClearDownFileCtrl();

	return 1;
}

/**
* @brief 请求下载标准文件
* @param itf 接口编号
*/
static void ReqDownWork(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct req_dwnwork *preq = (struct req_dwnwork *)(prcv->data);
	struct req_dwnwork *pecho = (struct req_dwnwork *)(psnd->data);
	unsigned long timeout, memsize;

	if(UPLINKAPP_LEN(prcv) < LEN_REQ_DWNWORK) return;

	smallcpy(psnd->data, prcv->data, LEN_REQ_DWNWORK);
	UPLINKAPP_LEN(psnd) = LEN_REQ_DWNWORK;

	if(TRANFILE_REQ_SN != MAKE_LONG(preq->req)) goto mark_fail;

	ClearDownFileCtrl();

	DownFileCtrl.flag = DWNFLAG_WORKFILE;
	DownFileCtrl.fileid = preq->fileid;
	DownFileCtrl.filesize = MAKE_LONG(preq->size);
	DownFileCtrl.serial = 0;
	DownFileCtrl.mask = 0;
	DownFileCtrl.winsize = preq->winsize;
	DownFileCtrl.blksize = MAKE_SHORT(preq->blksize);
	DownFileCtrl.filecrc = MAKE_SHORT(preq->filecrc);
	DownFileCtrl.blast = 0;
	DownFileCtrl.lastlen = 0;

	if(DownFileCtrl.blksize < 50 || DownFileCtrl.blksize > (UPLINK_RCVMAX(itf) - LEN_DWNDATA_HEAD)) 
		goto mark_fail;
	if(0 == DownFileCtrl.winsize || DownFileCtrl.winsize > 32) 
		goto mark_fail;
	if(32 == DownFileCtrl.winsize) 
		DownFileCtrl.full_mask = 0xffffffff;
	else 
		DownFileCtrl.full_mask = ((unsigned int)1<<(DownFileCtrl.winsize)) -1;
	if(DownFileCtrl.fileid > FILEID_DRIVER_XZIP) 
		goto mark_fail;

	memsize = (unsigned long)DownFileCtrl.blksize * (unsigned long)DownFileCtrl.winsize;
	if(memsize>updata_rwbuflen) 
		DownFileCtrl.pmem = NULL;
	else 
		DownFileCtrl.pmem = (unsigned char *)updata_rwbuf;
	if(NULL == DownFileCtrl.pmem) 
	{
		ErrorLog("malloc %d bytes fail\n", memsize);
		goto mark_fail;
	}

	//remove(DWNFILE_CACHE);
	//DownFileCtrl.pfcache = fopen(DWNFILE_CACHE, "wb");
	//if(NULL == DownFileCtrl.pfcache) {
	//	ErrorLog("open %s fail\n", DWNFILE_CACHE);
	//	free(DownFileCtrl.pmem);
	//	DownFileCtrl.pmem = NULL;
	//	goto mark_fail;
	//}
	
	store_iap_start();

	timeout = MAKE_LONG(preq->timeout);
	DownFileCtrl.timerid = SysAddCTimer(timeout, CTimerDownFile, 0);
	if(DownFileCtrl.timerid < 0) 
	{
		ErrorLog("add ctimer %d sec fail\n", timeout);
		ClearDownFileCtrl();
		goto mark_fail;
	}

	DownFileCtrl.snmax = DownFileCtrl.filesize / DownFileCtrl.blksize;
	if(DownFileCtrl.filesize % DownFileCtrl.blksize) 
		DownFileCtrl.snmax++;
	DownFileCtrl.snmax--;

	DownFileCtrl.svr_addr = prcv->grp & 0xfe;

	PrintLog(LOGTYPE_SHORT, "start download work file(%d): len=%d, timeout=%d, snmax=%d\n", 
			DownFileCtrl.fileid, DownFileCtrl.filesize, timeout, DownFileCtrl.snmax);

	pecho->code = FILECODE_REQ_DWNWORK;
	SvrEchoSend(itf, psnd);
	return;

mark_fail:
	pecho->code = FILECODE_REJECT;
	UPLINKAPP_LEN(psnd) = 5;
	SvrEchoSend(itf, psnd);
}
#if 0
/**
* @brief 请求下载普通文件
* @param zipped 1-压缩文件, 0-非压缩文件
* @param itf 接口编号
*/
static void ReqDownRawCommon(int zipped, unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct req_dwnraw *preq = (struct req_dwnraw *)(prcv->data);
	struct req_dwnraw *pecho = (struct req_dwnraw *)(psnd->data);
	unsigned long timeout, memsize;
	int namelen;

	if(UPLINKAPP_LEN(prcv) <= LEN_REQ_DWNRAW) return;

	smallcpy(psnd->data, prcv->data, LEN_REQ_DWNRAW);
	UPLINKAPP_LEN(psnd) = LEN_REQ_DWNRAW;

	if(TRANFILE_REQ_SN != MAKE_LONG(preq->req)) goto mark_fail;

	ClearDownFileCtrl();

	if(zipped) DownFileCtrl.flag = DWNFLAG_RAWFILE_ZIP;
	else DownFileCtrl.flag = DWNFLAG_RAWFILE;
	DownFileCtrl.filesize = MAKE_LONG(preq->size);
	DownFileCtrl.serial = 0;
	DownFileCtrl.mask = 0;
	DownFileCtrl.winsize = preq->winsize;
	DownFileCtrl.blksize = MAKE_SHORT(preq->blksize);
	DownFileCtrl.filecrc = MAKE_SHORT(preq->filecrc);
	DownFileCtrl.blast = 0;
	DownFileCtrl.lastlen = 0;

	if(DownFileCtrl.blksize < 50 || DownFileCtrl.blksize > (UPLINK_RCVMAX(itf) - LEN_DWNDATA_HEAD)) goto mark_fail;
	if(0 == DownFileCtrl.winsize || DownFileCtrl.winsize > 32) goto mark_fail;
	if(32 == DownFileCtrl.winsize) DownFileCtrl.full_mask = 0xffffffff;
	else DownFileCtrl.full_mask = ((unsigned int)1<<(DownFileCtrl.winsize)) - 1;

	*(prcv->data + UPLINKAPP_LEN(prcv)) = 0;
	namelen = strlen(preq->filename) + 1;
	if((namelen+LEN_REQ_DWNRAW) > UPLINKAPP_LEN(prcv) || namelen > 64) goto mark_fail;
	strcpy(DownFileCtrl.filename, (char *)preq->filename);

	memsize = (unsigned long)DownFileCtrl.blksize * (unsigned long)DownFileCtrl.winsize;
	DownFileCtrl.pmem = malloc(memsize);
	if(NULL == DownFileCtrl.pmem) {
		ErrorLog("malloc %d bytes fail\n", memsize);
		goto mark_fail;
	}

	remove(DWNFILE_CACHE);
	DownFileCtrl.pfcache = fopen(DWNFILE_CACHE, "wb");
	if(NULL == DownFileCtrl.pfcache) {
		ErrorLog("open %s fail\n", DWNFILE_CACHE);
		free(DownFileCtrl.pmem);
		DownFileCtrl.pmem = NULL;
		goto mark_fail;
	}

	timeout = MAKE_LONG(preq->timeout);
	DownFileCtrl.timerid = SysAddCTimer(timeout, CTimerDownFile, 0);
	if(DownFileCtrl.timerid < 0) {
		ErrorLog("add ctimer %d sec fail\n", timeout);
		ClearDownFileCtrl();
		goto mark_fail;
	}

	DownFileCtrl.snmax = DownFileCtrl.filesize / DownFileCtrl.blksize;
	if(DownFileCtrl.filesize % DownFileCtrl.blksize) DownFileCtrl.snmax++;
	DownFileCtrl.snmax--;

	DownFileCtrl.svr_addr = prcv->grp & 0xfe;

	PrintLog(LOGTYPE_SHORT, "start download raw file(%d:%s): len=%d, timeout=%d, snmax=%d\n", 
			zipped, DownFileCtrl.filename, DownFileCtrl.filesize, timeout, DownFileCtrl.snmax);

	if(zipped) pecho->code = FILECODE_REQ_DWNRAW_ZIP;
	else pecho->code = FILECODE_REQ_DWNRAW;

	SvrEchoSend(itf, psnd);
	return;

mark_fail:
	pecho->code = FILECODE_REJECT;
	UPLINKAPP_LEN(psnd) = 5;
	SvrEchoSend(itf, psnd);
}

static void ReqDownRaw(unsigned char itf)
{
	ReqDownRawCommon(0, itf);
}

static void ReqDownRawZip(unsigned char itf)
{
	ReqDownRawCommon(1, itf);
}
#endif

/**
* @brief 下载文件数据
* @param itf 接口编号
*/
static void DownloadData(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct down_data *pdwn = (struct down_data *)(prcv->data);
	struct down_data *pecho = (struct down_data *)(psnd->data);
	unsigned short serial;
	unsigned int len, offset, mask;

	len = (unsigned int)UPLINKAPP_LEN(prcv)&0xffff;
	if(len < LEN_DWNDATA_HEAD) return;

	smallcpy(psnd->data, prcv->data, LEN_DWNDATA_HEAD);
	UPLINKAPP_LEN(psnd) = LEN_DWNDATA_HEAD;
	DebugPrint(LOGTYPE_SHORT, "dwn serial=%02X%02X\n", pdwn->serial[1], pdwn->serial[0]);
	DebugPrint(LOGTYPE_SHORT, "echo serial=%02X%02X\n", pecho->serial[1], pecho->serial[0]);
	DebugPrint(LOGTYPE_SHORT, "rdata=%08X, dwn=%08X; sdata=%08X, echo=%08X\n", prcv->data, pdwn, psnd->data, pecho);

	if(NULL == DownFileCtrl.pmem ) {
		pecho->code = FILECODE_REJECT;
		UPLINKAPP_LEN(psnd) = 5;
		SvrEchoSend(itf, psnd);
		return;
	}

	len -= LEN_DWNDATA_HEAD;

	serial = MAKE_SHORT(pdwn->serial);
	DebugPrint(LOGTYPE_SHORT, "serial=%d:%d, len=%d\n", serial, DownFileCtrl.serial, len);
	if(serial > DownFileCtrl.snmax) return;
	if(len > DownFileCtrl.blksize) return;
	if(len < DownFileCtrl.blksize && serial != DownFileCtrl.snmax) return;
	if(serial >= (DownFileCtrl.serial + DownFileCtrl.winsize)) return;

	{
		unsigned short crc1, crc2;

		crc1 = CalculateCRC(pdwn->data, len);
		crc2 = MAKE_SHORT(pdwn->crc);
		if(crc1 != crc2) return;
	}
	DebugPrint(LOGTYPE_SHORT, "crc ok\n");

	if(serial < DownFileCtrl.serial) {
		SvrEchoSend(itf, psnd);
		return;
	}

	SysClearCTimer(DownFileCtrl.timerid);

	if(serial == DownFileCtrl.snmax) {
		DownFileCtrl.lastlen = len;
		DownFileCtrl.blast = 1;
		offset = serial - DownFileCtrl.serial;
		if(offset < 31) {
			mask = 1<<(offset+1);
			mask -= 1;
			DownFileCtrl.full_mask = mask;
		}
	}

	serial -= DownFileCtrl.serial;
	mask = 1<<serial;
	offset = (unsigned int)DownFileCtrl.blksize * (unsigned int)serial;
	memcpy(DownFileCtrl.pmem + offset, pdwn->data, len);
	DownFileCtrl.mask |= mask;

	if(DownFileCtrl.mask != DownFileCtrl.full_mask) {
		SvrEchoSend(itf, psnd);
		return;
	}

	DebugPrint(LOGTYPE_SHORT, "full\n");

	if(!DownFileCtrl.blast) len =  (unsigned int)DownFileCtrl.blksize * DownFileCtrl.winsize;
	else len = (unsigned int)DownFileCtrl.blksize*(DownFileCtrl.snmax-DownFileCtrl.serial)+DownFileCtrl.lastlen;
	//else len = (unsigned int)DownFileCtrl.blksize * (DownFileCtrl.winsize-1) + DownFileCtrl.lastlen;
	//fwrite(DownFileCtrl.pmem, len, 1, DownFileCtrl.pfcache);
	
	store_iap(DownFileCtrl.pmem, len);
	
	if(!DownFileCtrl.blast) {
		DownFileCtrl.serial += DownFileCtrl.winsize;
		DownFileCtrl.mask = 0;
		SvrEchoSend(itf, psnd);
		return;
	}

	/**end**/
	DebugPrint(LOGTYPE_SHORT, "end\n");

	SysStopCTimer(DownFileCtrl.timerid);
	DownFileCtrl.timerid = -1;
	//fclose(DownFileCtrl.pfcache);
	//DownFileCtrl.pfcache = NULL;
	
	

	PrintLog(LOGTYPE_SHORT, "down file end ok\n");

	ClearDownFileCtrl();
	SvrEchoSend(itf, psnd);
	
	Sleep(300);
	
	store_iap_end();

	SysRestart();
	return;

}

#if 0
/**
* @brief 上传文件超时定时器
*/
static int CTimerUpFile(unsigned long arg)
{
	PrintLog(LOGTYPE_ALARM, "up file timeout\n");

	UpFileCtrl.timerid = -1;
	ClearUpFileCtrl();

	return 1;
}

/**
* @brief 请求上传普通文件
* @param zipped 1-压缩文件, 0-非压缩文件
* @param itf 接口编号
*/
static void ReqReadRawCommon(int zipped, unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct req_upraw *preq = (struct req_upraw *)(prcv->data);
	struct req_upraw *pecho = (struct req_upraw *)(psnd->data);
	unsigned long timeout;
	int len, i;
	unsigned short crc;

	if(UPLINKAPP_LEN(prcv) <= LEN_REQ_UPRAW) return;

	smallcpy(psnd->data, prcv->data, LEN_REQ_UPRAW);
	UPLINKAPP_LEN(psnd) = LEN_REQ_UPRAW;

	if(TRANFILE_REQ_SN != MAKE_LONG(preq->req)) goto mark_fail;

	ClearUpFileCtrl();

	UpFileCtrl.serial = 0;
	UpFileCtrl.blksize = MAKE_SHORT(preq->blksize);
	UpFileCtrl.memsize = UpFileCtrl.blksize;

	if(UpFileCtrl.blksize < 50 || UpFileCtrl.blksize > (UPLINK_RCVMAX(itf) - LEN_DWNDATA_HEAD)) goto mark_fail;

	*(prcv->data + UPLINKAPP_LEN(prcv)) = 0;
	len = strlen(preq->filename) + 1;
	if((len+LEN_REQ_UPRAW) > UPLINKAPP_LEN(prcv)) goto mark_fail;

	UpFileCtrl.pmem = malloc((unsigned long)UpFileCtrl.blksize);
	if(NULL == UpFileCtrl.pmem) {
		ErrorLog("malloc %d bytes fail\n", UpFileCtrl.blksize);
		goto mark_fail;
	}

	if(zipped) {
		__GlobalUnlock();
		i = CompressFile(FILE_XZIP_MAGIC, preq->filename, ZIP_TEMPFILE_UP);
		__GlobalLock();
		if(i) {
			PrintLog(LOGTYPE_SHORT, "zip file %s fail\n", preq->filename);
			goto mark_fail;
		}

		UpFileCtrl.pf = fopen(ZIP_TEMPFILE_UP, "rb");
	}
	else {
		UpFileCtrl.pf = fopen(preq->filename, "rb");
	}

	if(NULL == UpFileCtrl.pf) {
		ErrorLog("open %s fail\n", preq->filename);
		free(UpFileCtrl.pmem);
		UpFileCtrl.pmem = NULL;
		goto mark_fail;
	}

	crc = 0;
	len = 0;
	while(1) {
		i = fread(UpFileCtrl.pmem, 1, UpFileCtrl.blksize, UpFileCtrl.pf);
		if(i > 0) {
			CalculateCRCStep(UpFileCtrl.pmem, i, &crc);
			len += i;
		}
		if(i < UpFileCtrl.blksize) break;
	}
	DEPART_LONG(len, pecho->size);
	DEPART_SHORT(crc, pecho->filecrc);
	fseek(UpFileCtrl.pf, 0, SEEK_SET);

	timeout = MAKE_LONG(preq->timeout);
	UpFileCtrl.timerid = SysAddCTimer(timeout, CTimerUpFile, 0);
	if(UpFileCtrl.timerid < 0) {
		ErrorLog("add ctimer %d sec fail\n", timeout);
		ClearUpFileCtrl();
		goto mark_fail;
	}

	PrintLog(LOGTYPE_SHORT, "start upload file: len=%d, timeout=%d\n", len, timeout);

	if(zipped) pecho->code = FILECODE_REQ_UPRAW_ZIP;
	else pecho->code = FILECODE_REQ_UPRAW;
	SvrEchoSend(itf, psnd);
	return;

mark_fail:
	pecho->code = FILECODE_REJECT;
	UPLINKAPP_LEN(psnd) = 5;
	SvrEchoSend(itf, psnd);
}

static void ReqReadRaw(unsigned char itf)
{
	ReqReadRawCommon(0, itf);
}

static void ReqReadRawZip(unsigned char itf)
{
	ReqReadRawCommon(1, itf);
}


/**
* @brief 上传文件传输数据
* @param itf 接口编号
*/
static void UploadData(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct down_data *pdwn = (struct down_data *)(prcv->data);
	struct down_data *pecho = (struct down_data *)(psnd->data);
	unsigned short serial, crc;
	int len;

	len = (unsigned int)UPLINKAPP_LEN(prcv)&0xffff;
	if(len < LEN_DWNDATA_HEAD) return;

	smallcpy(psnd->data, prcv->data, LEN_DWNDATA_HEAD);
	UPLINKAPP_LEN(psnd) = LEN_DWNDATA_HEAD;

	if(NULL == UpFileCtrl.pmem || NULL == UpFileCtrl.pf) goto mark_fail;

	serial = MAKE_SHORT(pdwn->serial);
	if(serial == (UpFileCtrl.serial+1)) {
		if(UpFileCtrl.memsize < UpFileCtrl.blksize) { //end
			PrintLog(LOGTYPE_SHORT, "upload file end\n");
			ClearUpFileCtrl();
			return;
		}

		len = fread(UpFileCtrl.pmem, 1, UpFileCtrl.blksize, UpFileCtrl.pf);
		if(len <= 0) goto mark_end_fail;

		UpFileCtrl.memsize = len;
		UpFileCtrl.serial++;
	}

	if(serial != UpFileCtrl.serial) goto mark_end_fail;

	SysClearCTimer(UpFileCtrl.timerid);

	memcpy(pecho->data, UpFileCtrl.pmem, UpFileCtrl.memsize);
	UPLINKAPP_LEN(psnd) = UpFileCtrl.memsize + LEN_DWNDATA_HEAD;
	crc = CalculateCRC(UpFileCtrl.pmem, UpFileCtrl.memsize);
	DEPART_SHORT(crc, pecho->crc);

	SvrEchoSend(itf, psnd);
	return;

mark_end_fail:
	ClearUpFileCtrl();
mark_fail:
	pecho->code = FILECODE_REJECT;
	UPLINKAPP_LEN(psnd) = 5;
	SvrEchoSend(itf, psnd);
	return;
}

/**
* @brief 上传文件传输中止
* @param itf 接口编号
*/
static void UpLoadReject(unsigned char itf)
{
	ClearUpFileCtrl();
}

/**
* @brief 复制文件下载进程信息
* @param pifno 复制缓存区指针
*/
static inline void CopyFileInfo(struct dfile_info *pinfo)
{
	pinfo->duid[0] = pinfo->duid[1] = 0;
	pinfo->duid[2] = 0x02;  //F10
	pinfo->duid[3] = 0x01;
	pinfo->code = FILECODE_INFO;

	DEPART_LONG(DownFileCtrl.filesize, pinfo->filesize);
	DEPART_SHORT(DownFileCtrl.serial, pinfo->serial);
	DEPART_LONG(DownFileCtrl.mask, pinfo->mask);
	DEPART_SHORT(DownFileCtrl.blksize, pinfo->blksize);
	DEPART_SHORT(DownFileCtrl.filecrc, pinfo->filecrc);
	pinfo->fileid = DownFileCtrl.fileid;
	pinfo->winsize = DownFileCtrl.winsize;
}

/**
* @brief 主动发送文件下载进程信息(登录后发送)
* @param itf 接口编号
*/
void SvrSendFileInfo(unsigned char itf)
{
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct dfile_info *pinfo = (struct dfile_info *)(psnd->data);

	if(NULL == DownFileCtrl.pmem || NULL == DownFileCtrl.pfcache) return;

	psnd->ctrl = UPCMD_USRDATA|UPCTRL_PRM;
	psnd->afn = UPAFN_TRANFILE;
	UPLINKAPP_LEN(psnd) = sizeof(struct dfile_info);

	CopyFileInfo(pinfo);
	psnd->seq = UPSEQ_SPKT;
	psnd->grp = DownFileCtrl.svr_addr;

	UplinkSendPkt(itf, psnd);
}
#endif
/**
* @brief 复制文件下载进程信息
* @param pifno 复制缓存区指针
*/
static void CopyFileInfo(struct dfile_info *pinfo)
{
/*	pinfo->duid[0] = pinfo->duid[1] = 0;
	pinfo->duid[2] = 0x02;  //F10
	pinfo->duid[3] = 0x01;*/
	pinfo->code = FILECODE_INFO;

	DEPART_LONG(DownFileCtrl.filesize, pinfo->filesize);
	DEPART_SHORT(DownFileCtrl.serial, pinfo->serial);
	DEPART_LONG(DownFileCtrl.mask, pinfo->mask);
	DEPART_SHORT(DownFileCtrl.blksize, pinfo->blksize);
	DEPART_SHORT(DownFileCtrl.filecrc, pinfo->filecrc);
	pinfo->fileid = DownFileCtrl.fileid;
	pinfo->winsize = DownFileCtrl.winsize;
}
/**
* @brief 处理读取文件下载进程信息请求
* @param itf 接口编号
*/
static void SvrDownFileInfo(unsigned char itf)
{
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct dfile_info *pinfo = (struct dfile_info *)(psnd->data);

	if(NULL == DownFileCtrl.pmem ) return;

	psnd->ctrl = UPCMD_USRDATA;
	psnd->afn = UPAFN_TRANFILE;
	UPLINKAPP_LEN(psnd) = sizeof(struct dfile_info);

	CopyFileInfo(pinfo);

	SvrEchoSend(itf, psnd);
}


typedef void (*dealfunc_t)(unsigned char itf);
static const dealfunc_t DealFunc[FILECODE_MAX] = {
	ReqDownWork,	/*FILECODE_REQ_DWNWORK*/
	NULL,		/*FILECODE_REQ_DWNRAW*/
	NULL,		/*FILECODE_REQ_UPRAW*/
	DownloadData,	/*FILECODE_DWNDATA*/
	NULL,		/*FILECODE_UPDATA*/
	NULL,	/*FILECODE_REJECT*/
	SvrDownFileInfo, /*FILECODE_INFO*/
	NULL, /*FILECODE_REQ_DWNRAW_ZIP*/
	NULL, /*FILECODE_REQ_UPRAW_ZIP*/
	NULL, /*FILECODE_REQ_DWN_ACTIVE*/
	NULL, /*FILECODE_DWNACT_DATA*/
};

/**
* @brief 处理自定义文件传输
* @param itf 接口编号
*/
void SvrSelfTranFile(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	unsigned char code;

	if(UPLINKAPP_LEN(prcv) < 5) return;

	code = *(prcv->data + 4);
	if(code >= FILECODE_MAX) return;

	psnd->ctrl = UPECHO_USRDATA;
	psnd->afn = UPAFN_TRANFILE;

	if(NULL != DealFunc[code]) (DealFunc[code])(itf);
}

/**
* @brief 自定义文件传输初始化
*/
//DECLARE_INIT_FUNC(SvrSelfTranFileInit);
int SvrSelfTranFileInit(void)
{
	DownFileCtrl.timerid = -1;
	DownFileCtrl.pmem = NULL;
	//DownFileCtrl.pfcache = NULL;

	//UpFileCtrl.timerid = -1;
	//UpFileCtrl.pmem = NULL;
	//UpFileCtrl.pf = NULL;

	//DownFileCtrlF1.pmem = NULL;
	//DownFileCtrlF1.pfcache = NULL;

	//SvrDwnActInit();

	//SET_INIT_FLAG(SvrSelfTranFileInit);

	return 0;
}

#if 0		//浙江
/**
* @brief 清除下载文件控制
*/
static inline void ClearDownFileCtrlF1(void)
{
	if(NULL != DownFileCtrlF1.pmem) {
		free(DownFileCtrlF1.pmem);
		DownFileCtrlF1.pmem = NULL;
	}

	if(NULL != DownFileCtrlF1.pfcache) {
		fclose(DownFileCtrlF1.pfcache);
		DownFileCtrlF1.pfcache = NULL;
	}

	remove(DWNFILE_CACHE);
}

static void DownloadAttrStart(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct req_dwnworkf1 *preq = (struct req_dwnworkf1 *)(prcv->data);
	struct req_filehead *phead = (struct req_filehead *)(preq->data);
	unsigned long memsize;
	unsigned int len;
	int namelen;

	if(preq->attr != 0) return;
	if(MAKE_LONG(preq->serial) != 0) return;
	
	len = (unsigned int)UPLINKAPP_LEN(prcv)&0xffff;
	if(len < LEN_REQ_DWNWORKF1 + LEN_REQ_FILEHEAD) return;
	len -= LEN_REQ_DWNWORKF1;
	
	smallcpy(psnd->data, prcv->data, 4);
	
	if(TRANFILE_REQ_SN != MAKE_LONG(phead->sn)) goto mark_fail;

	ClearDownFileCtrlF1();

	DownFileCtrlF1.serial = MAKE_LONG(preq->serial);
	DownFileCtrlF1.sumblk = MAKE_SHORT(preq->sumblk);
	DownFileCtrlF1.blksize = MAKE_SHORT(preq->blksize);
	DownFileCtrlF1.svr_addr = prcv->grp & 0xfe;

	if(DownFileCtrlF1.blksize < 20 || DownFileCtrlF1.blksize > len) goto mark_fail;

	namelen = strlen(phead->filename) + 1;
	if(namelen > len || namelen > 64) goto mark_fail;
	strcpy(DownFileCtrlF1.filename, (char *)phead->filename);

	memsize = (unsigned long)DownFileCtrlF1.blksize;
	DownFileCtrlF1.pmem = malloc(memsize);
	if(NULL == DownFileCtrlF1.pmem) {
		ErrorLog("malloc %d bytes fail\n", memsize);
		goto mark_fail;
	}

	remove(DWNFILE_CACHE);
	DownFileCtrlF1.pfcache = fopen(DWNFILE_CACHE, "wb");
	if(NULL == DownFileCtrlF1.pfcache) {
		ErrorLog("open %s fail\n", DWNFILE_CACHE);
		free(DownFileCtrlF1.pmem);
		DownFileCtrlF1.pmem = NULL;
		goto mark_fail;
	}

	if(len == DownFileCtrlF1.blksize){
		memcpy(DownFileCtrlF1.pmem, phead->filename + namelen, len - namelen - 4);
		fwrite(DownFileCtrlF1.pmem, len - namelen - 4, 1, DownFileCtrlF1.pfcache);
	}

	PrintLog(LOGTYPE_SHORT, "start download work file: %s snmax=%d\n", 
		DownFileCtrlF1.filename, DownFileCtrlF1.sumblk);

	smallcpy(psnd->data + 4, preq->serial, 4);
	UPLINKAPP_LEN(psnd) = LEN_REQ_ECHO;
	SvrEchoSend(itf, psnd);
	return;
	
mark_fail:
	memset(psnd->data + 4, 0xff, 4);
	UPLINKAPP_LEN(psnd) = LEN_REQ_ECHO;
	SvrEchoSend(itf, psnd);	
}

static void DownloadAttrMiddle(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct req_dwnworkf1 *preq = (struct req_dwnworkf1 *)(prcv->data);
	unsigned int serial, len;

	len = (unsigned int)UPLINKAPP_LEN(prcv)&0xffff;
	if(len < LEN_REQ_DWNWORKF1) return;

	smallcpy(psnd->data, prcv->data, 4);

	if(NULL == DownFileCtrlF1.pmem || NULL == DownFileCtrlF1.pfcache) {
		goto mark_end_fail;
	}

	len -= LEN_REQ_DWNWORKF1;

	serial = MAKE_LONG(preq->serial);
	//DebugPrint(LOGTYPE_SHORT, "serial=%d:%d, len=%d\n", serial, DownFileCtrl.serial, len);
	if(serial >= DownFileCtrlF1.sumblk) return;
	if(len > DownFileCtrlF1.blksize) return;
	if(len < DownFileCtrlF1.blksize && serial != DownFileCtrlF1.sumblk - 1) return;

	if(serial == DownFileCtrlF1.serial + 1){
		DownFileCtrlF1.serial++;
		
		if(len != DownFileCtrlF1.blksize) goto mark_end_fail;

		memcpy(DownFileCtrlF1.pmem, preq->data, len);
		fwrite(DownFileCtrlF1.pmem, len, 1, DownFileCtrlF1.pfcache);
	}
	
	DEPART_LONG(DownFileCtrlF1.serial, psnd->data + 4);
	UPLINKAPP_LEN(psnd) = LEN_REQ_ECHO;
	SvrEchoSend(itf, psnd);
	return;

mark_end_fail:
	ClearDownFileCtrlF1();
	memset(psnd->data + 4, 0xff, 4);
	UPLINKAPP_LEN(psnd) = LEN_REQ_ECHO;
	SvrEchoSend(itf, psnd);
}

static void DownloadAttrEnd(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct req_dwnworkf1 *preq = (struct req_dwnworkf1 *)(prcv->data);
	unsigned int serial, len;
	char cmd[128];

	len = (unsigned int)UPLINKAPP_LEN(prcv)&0xffff;
	if(len < LEN_REQ_DWNWORKF1) return;

	smallcpy(psnd->data, prcv->data, 4);
	UPLINKAPP_LEN(psnd) = 8;

	if(NULL == DownFileCtrlF1.pmem || NULL == DownFileCtrlF1.pfcache) {
		goto mark_end_fail;
	}

	len -= LEN_REQ_DWNWORKF1;

	serial = MAKE_LONG(preq->serial);
	
	if(serial != DownFileCtrlF1.sumblk - 1) return;

	if(len > DownFileCtrlF1.blksize) return;
	
	memcpy(DownFileCtrlF1.pmem, preq->data, len);
	fwrite(DownFileCtrlF1.pmem, len, 1, DownFileCtrlF1.pfcache);

	fclose(DownFileCtrlF1.pfcache);
	DownFileCtrlF1.pfcache = NULL;

	//remove
	remove(DownFileCtrlF1.filename);
	//copy
	sprintf(cmd, "cp %s %s",  DWNFILE_CACHE, DownFileCtrlF1.filename);
	SystemCmd(cmd);

	PrintLog(LOGTYPE_SHORT, "down file end ok\n");

	ClearDownFileCtrlF1();
	SvrEchoSend(itf, psnd);

	Sleep(300);
	SysRestart();
	return;

mark_end_fail:
	ClearDownFileCtrlF1();
	memset(psnd->data + 4, 0xff, 4);
	UPLINKAPP_LEN(psnd) = LEN_REQ_ECHO;
	SvrEchoSend(itf, psnd);
}

/**
* @brief 处理376.1规约文件传输
* @param itf 接口编号
*/
void SvrTranFileF1(unsigned char itf)
{
	uplink_pkt_t *prcv = (uplink_pkt_t *)UPLINK_RCVBUF(itf);
	uplink_pkt_t *psnd = (uplink_pkt_t *)UPLINK_SNDBUF(itf);
	struct req_dwnworkf1 *preq = (struct req_dwnworkf1 *)(prcv->data);
	unsigned int serial;
	
	if(UPLINKAPP_LEN(prcv) < LEN_REQ_DWNWORKF1) return;

	if(preq->flag != 1) return;

	if(preq->code != 0) return;

	psnd->ctrl = UPECHO_USRDATA;
	psnd->afn = UPAFN_TRANFILE;

	serial = MAKE_LONG(preq->serial);

	if(0 == preq->attr){
		if(serial == 0) DownloadAttrStart(itf);
		else DownloadAttrMiddle(itf);
	}
	else if(1 == preq->attr) DownloadAttrEnd(itf);
}
#endif


