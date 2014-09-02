
#ifndef _SVRMSG_H
#define _SVRMSG_H

#define QUERY_CACHE_NUM		6
#define QUERY_CACHE_LEN		1152

#ifndef define_QueryCache
extern unsigned int QueryCache[QUERY_CACHE_NUM][QUERY_CACHE_LEN/4];
#define para_rwbuflen 5500
#define para_rwbuf  &QueryCache[1][0] 
#define updata_rwbuflen 1152
#define updata_rwbuf &QueryCache[1][0]
#define data_rwbuf &QueryCache[3][0]

#endif
struct data_t;
//DLMS Õ∏¥´
void DlmsForward(const struct data_t* pcmd,struct data_t* pecho);
//¥¶¿ÌDLMS√¸¡Ó
int SvrMessageDlms(unsigned char itf);
#endif

