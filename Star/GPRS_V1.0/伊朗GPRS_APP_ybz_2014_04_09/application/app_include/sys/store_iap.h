#ifndef __STORE_IAP_H
#define __STORE_IAP_H



//保存代码
int store_iap(const void *pbuf,unsigned short len);
//在线升级开始
void store_iap_start(void);
//结束
void store_iap_end(void);
/*
在线升级时，最先调用iap_start,然后将收到的代码帧按顺序调用save_code,最后调用iap_end

*/



#endif

