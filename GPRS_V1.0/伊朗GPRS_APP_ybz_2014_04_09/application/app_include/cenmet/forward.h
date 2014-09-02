/**
* foward.h -- 表计透明转发
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-17
* 最后修改时间: 2009-5-17
*/

#ifndef _CENMET_FORWARD_H
#define _CENMET_FORWARD_H

struct data_t
{
	unsigned int len;
	unsigned char* data;
};

void CenMetForward(const struct data_t *pcmd, struct data_t *pecho);
void PlForwardF1(const struct data_t *pcmd, struct data_t *pecho,unsigned char poto);
//初始化抄表线程
void InitCenmet(void);
//抄读剩余金额
void ReadMoney(void);
//充值
void DoRecharge(unsigned char*);
#endif /*_CENMET_FORWARD_H*/

