/**
* datause.h -- 支持数据配置参数头文件
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-8
* 最后修改时间: 2009-5-8
*/

#ifndef _PARAM_DATAUSE_H
#define _PARAM_DATAUSE_H

#define METCLS_1XMF		1   //单相复费率电表
#define METCLS_3XMF		2   //三相复费率电表
#define METCLS_1XSF		10	//单相单费率电表

#define USRCLS_A		1  //大型专变(A类)
#define USRCLS_B		2  //中小型专变(B类)
#define USRCLS_C		3  //三相一般工商业用户(C类)
#define USRCLS_D		4  //单相一般工商业用户(D类)
#define USRCLS_E		5  //居民用户(E类)
#define USRCLS_F		6  //公用配变考核计量点(F类)

//F38, F39 1, 2类数据配置设置 (在终端支持的数据配置内)
#define MAX_USERCLASS	7
#define MAX_METCLASS	16
typedef struct {
	unsigned char num;
	unsigned char fnmask[31];  //F1~F248
} cfg_datafns_t;

typedef struct {
	cfg_datafns_t met[MAX_METCLASS];
} cfg_usercls_data_all_t;

typedef union {
	cfg_usercls_data_all_t datacls1[MAX_USERCLASS];  //F38
	cfg_usercls_data_all_t datacls2[MAX_USERCLASS];  //F39
} para_datause_all_t;

#define CLS1_MAX_ITERMS      8
#define CLS2_MAX_ITERMS      8

typedef struct {
	unsigned char metcls;
	unsigned char usrcls;
	cfg_datafns_t met;
} cfg_usercls_data_t;

typedef struct {
	cfg_usercls_data_t datacls1[CLS1_MAX_ITERMS];  //F38
	cfg_usercls_data_t datacls2[CLS2_MAX_ITERMS];  //F39
} para_datause_t;

#ifndef DEFINE_PARADATAUSE
extern /*const*/ para_datause_t ParaDataUse;
#endif

extern const cfg_datafns_t ValidDataCls1_1;
extern const cfg_datafns_t ValidDataCls2_1;
extern const cfg_datafns_t ValidDataCls1_2;
extern const cfg_datafns_t ValidDataCls2_2;

extern int DUseCls1FnVaid(unsigned char usrcls, unsigned char metcls, unsigned char fn);
extern int DUseCls2FnVaid(unsigned char usrcls, unsigned char metcls, unsigned char fn);

#endif /*_PARAM_DATAUSE_H*/

