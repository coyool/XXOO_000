/**
 * uplink_gprs.h: GPRS
 * Ð»ÎÄ¸Õ 
 * 2011.8.29
 */
#ifndef __GPRS_H
#define __GPRS_H

#include "hal_include/switch.h"
#include "plat_include/Plat_uart.h"



int dev_sim900_get_local_ip(unsigned char *ipaddr);

int dev_sim900_getchar(unsigned char *buf);
int dev_sim900_send(const unsigned char *buf,int len);

void dev_sim900_com_init(void);
void dev_sim900_pwr_onoff(void);
void dev_gprssvr_init(void);
int  dev_sim900_connect(unsigned char* ip, char* apn,unsigned short port, unsigned char proto);

#endif
