/**
 * uplink_gprs.c: GPRS
 * 谢文刚 
 * 2011.8.26
 */
#include <string.h>

#include "hal_include/switch.h"

#include "sim900.h"
#include "include/debug.h"
#include "bgs2.h"
#include "include/param/term.h"
//实际验证得波特率在1200时不适

/*
const static unsigned char xt[]={0x68,0x32,0x00,0x32,0x00,0x68,0xc9,0x00,0x00,
0x00,0x00,0x00,0x02,0x60,0x00,0x00,0x04,0x00,0x2f,0x16};
*/


struct connect_state
{
	unsigned char commok:1;//com 
	unsigned char serverok:1;//lison
	unsigned char remoteok:1;//sever ok
	unsigned char clientok:1;//
};
struct connect_state cstate;


void connected_check(void);
void dev_sim900_com_init()
{
	struct uart_mode mode;
	
	mode.baudrate = GPRS_BAUDRATE;
	mode.parity = UART_NO_PARITY;
	mode.stop = 1;
	mode.len = 8;
	s_open(UART_GPRS,&mode);	
	gprs_switch_init();	//端口初始化
}
void dev_sim900_pwr_onoff(void)
{
    
#if 0
	sim900_repwr();	//掉电10秒重新上电
	sim900_onoff();	//开关动作
	sim900_init();
#else
      module_pwroff();
	module_pwron();
	module_init();
#endif
}

void dev_gprssvr_init(void)
{
      const char* apn = "szbank3g.gd";
     
      module_pwroff();
	module_pwron();
	module_init();
      set_apn(apn);
}
int dev_sim900_connect(unsigned char* ip, char *apn,unsigned short port, unsigned char proto)
{
	int rt;
#if 0
	//__GlobalUnlock()
	rt = sim900_setup_client(proto,(unsigned char const*)apn,ip,port);
	//__GlobalLock()
#else
	rt = set_apn(apn);
	if(rt == -1) return -1;
	//rt = config_client(ip,port);
	//if(rt == -1) return -1;
	rt = open_client(ip,port);
#endif
	return rt;
}

int dev_sim900_get_local_ip(unsigned char *ipaddr)
{
	int rt;
#if 0
	if(cstate.commok==0)
		return -1;
  	sim900_get_local_ip(ipaddr);
	return 0;
#else
	//rt = get_connection_info(ipaddr);

	return rt;
#endif
}


int dev_sim900_getchar(unsigned char *buf)
{
	static  unsigned char rcvbuf[1024];
	static  int rcvlen = 0;
	static  int head = 0;
	
	if(rcvlen==0){
		head = 0;
            if(ParaTerm.uplink.mode == 1)
		    rcvlen = module_recv(1,rcvbuf,sizeof(rcvbuf));
            else  if(ParaTerm.uplink.mode == 2)
                 rcvlen = module_recv(0,rcvbuf,sizeof(rcvbuf));
		if(rcvlen==-1)rcvlen = 0;
		
		return 0;
	}
	else{
		*buf = rcvbuf[head];
		head++;
		rcvlen--;
		return 1; 
	}

}

int dev_sim900_send(const unsigned char *buf,int len)
{
      if(ParaTerm.uplink.mode == 1)
	{ 
	  if(len==module_send(1,buf,len))  return 0;
      }
      else if(ParaTerm.uplink.mode == 2)
      {
        if(len==module_send(0,buf,len))  return 0;
      }
      return -1;
//	return s_write(UART_GPRS,buf,len);
}


