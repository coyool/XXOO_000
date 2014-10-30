/**
* qrycurve.h -- ��ʷ���ݲ�ѯ�ӿ�
* 
* ����: zhuzhiqiang
* ����ʱ��: 2009-5-14
* ����޸�ʱ��: 2009-5-14
*/

#ifndef _QRY_CURVE_H
#define _QRY_CURVE_H

//��ѯ��ʷ���ݻ���������
typedef struct st_qrycurve_buffer {
	//in
	struct st_qrycurve_buffer * const next;
	const int maxlen;
	unsigned char * const buffer;

	//out
	int datalen;
} qrycurve_buffer_t;

#define QRYCBUF_MAXLEN(pbuffer)		((pbuffer)->maxlen - (pbuffer)->datalen)
#define QRYCBUF_DATA(pbuffer)		((pbuffer)->buffer + (pbuffer)->datalen)
#define QRYCBUF_NEXT(pbuffer)		(pbuffer) = (pbuffer)->next
#define QRYCBUF_ADD(pbuffer, len)	(pbuffer)->datalen += (len)

#define QRYCBUF_SCAN(pbuffer, dlen, pbakbuf, step) { \
	while(QRYCBUF_MAXLEN(pbuffer) < (dlen)) { \
		QRYCBUF_NEXT(pbuffer); \
		(step) += 1; \
		(pbakbuf) = (pbuffer); \
		if(NULL == (pbuffer)) break; \
	} \
}

int QueryCurve(const unsigned char *itemid, const unsigned char *timemark, int timemarklen, qrycurve_buffer_t *sendbuffer, int bactive);

int ActiveSendAlarm(qrycurve_buffer_t *sendbuffer);
int QueryAlarm(const unsigned char *recvbuf, qrycurve_buffer_t *sendbuffer);

qrycurve_buffer_t *GetQueryCache(int maxlen);
void ClearQueryCache(qrycurve_buffer_t *buffer);
void QueryCacheLock(void);
void QueryCacheUnlock(void);

#endif /*_QRY_CURVE_H*/

