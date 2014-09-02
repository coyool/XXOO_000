/*
应用程序原来的存储接口在一起，为了移植的方便，也把所有的存储接口统一在一个函数
*/
#include "app_include/sys/store.h"
#include "app_include/sys/store_fre.h"
#include "app_include/sys/store_alarm.h"
#include "app_include/sys/store_para.h"
#include "plat_include/flashsave.h"
/*
 *读数据库中的电表数据
 *frztype：0:日冻结 1:抄表日冻结 2:月冻结 3:曲线 4:终端日冻结 5: 终端月冻结
 *mid:表编号0~63
 *buf:终据缓冲区指针
 *len:终据缓冲区长度
 *dbtime:时标
 */
/*
int db_readflash_bin(unsigned char frztype,unsigned int mid,void *buf,unsigned short len,dbtime_t dbtime)
{
	return fre_read(frztype,dbtime,mid,buf,len);
}

int db_writeflash_bin(unsigned char frztype,unsigned int mid,const void *buf,unsigned short len,dbtime_t dbtime)
{
	return fre_save(frztype,dbtime,mid,buf,len);
}
 

int db_rmflash_bin(unsigned char frztype,unsigned int mid)
{
	return fre_rm(frztype,mid);
}
 */
/*int db_rmflash_time_bin(unsigned char type,dbtime_t dbtime)
{
	return fre_rm_time(type,dbtime);
}

int db_rmflash_interval_bin(dbtime_t time1,dbtime_t time2)
{
	return fre_rm_interval(time1,time2);
}

int db_rmflash_all_bin(void)
{
	return fre_rm_all();
} */




int para_writeflash_bin(unsigned char type,const void *buf,unsigned short len)
{
	return para_save(type,buf,len);
}
int para_readflash_bin(unsigned char type,void *buf,unsigned short len)
{
	return para_read(type,buf,len);
}


int para_rmflash_bin(unsigned char idx)
{
	return para_rm(idx);
}


int alarm_writeflash_bin(unsigned char type,const void *pbuf,unsigned short len)
{
	return alarm_save(type,pbuf,len);
}

int alarm_readflash_bin(unsigned char type,unsigned int cur,void *pbuf,unsigned short len)
{	
	return alarm_read(type,cur,pbuf,len);
}
int alarm_rmflash_bin(void)
{
	return alarm_rm();
}

#if 0
#include "string.h"
#include "flash.h"
#include "crc.h"
#include "print.h"
#include "delay.h"
#if 0

void test_para(void)
{
	int i,j;
	for(j=1;j<8;j++){
		for(i=0;i<sizeof cctest;i++)
			cctest[i]=(i/3+j);
		para_writeflash_bin(j,cctest,sizeof cctest);
	}
	for(j=1;j<8;j++){
		memset(cctest,0,sizeof cctest);
		para_readflash_bin(j,cctest,sizeof cctest);
		for(i=0;i<sizeof cctest;i++)
			if(cctest[i]!=(unsigned char)(i/3+j))
				break;
		if(i!=sizeof cctest)
			printdbg(PRINT_NORMAL,"err\n");
	}
	for(j=1;j<8;j++)
		para_rm(j);

	for(j=1;j<8;j++){
		memset(cctest,0,sizeof cctest);
		para_readflash_bin(j,cctest,sizeof cctest);
		for(i=0;i<sizeof cctest;i++)
			if(cctest[i]!=(unsigned char)(i/3+j))
				break;
		if(i!=sizeof cctest)
			printdbg(PRINT_NORMAL,"err\n");
	}
}

#define ALARM_N	80
void test_alarm(void)
{
	int i,j,it,cnt,icnt;
	unsigned char testbuf[100];
	cnt = 0;
	icnt=1000;
	it=0;
#if 0
			//cnt =alarm_count_get();
			for(i=0;i<sizeof testbuf;i++)
				testbuf[i]=1;
			alarm_save(0,testbuf,100);
			for(i=0;i<sizeof testbuf;i++)
				testbuf[i]=2;
			alarm_save(0,testbuf,100);
			
			memset(testbuf,0,sizeof testbuf);
			alarm_read(0,1,testbuf,100);
	
			memset(testbuf,0,sizeof testbuf);
			alarm_read(0,0,testbuf,100);

#endif
	while(1){
		for(j=1;j<2;j++){
			while(cnt<300){
				for(i=0;i<sizeof testbuf;i++)
					testbuf[i]=i/3+cnt;
				alarm_save(j,testbuf,100);
				cnt ++;
				icnt = cnt;
			}
			cnt=299;
			icnt=255;
			while(cnt){
				
				delay_us(200000);
				memset(testbuf,0,sizeof testbuf);
				alarm_read(j,icnt,testbuf,100);
				for(i=0;i<sizeof testbuf;i++)
					if(testbuf[i]!=(unsigned char)(i/3+cnt))
						break;
				if(i==sizeof testbuf)
					print(PRINT_NORMAL,"check cnt icnt: %d it:%d ok\n",icnt,it);
				else
					print(PRINT_NORMAL,"check cnt icnt :%d it %d err\n",icnt,it);
				cnt --;
				icnt--;
				if(icnt<0)
					icnt=255;
			}
		}
	}

}

#endif
#include "rtc.h"
unsigned char testbuf[36];
const unsigned int tpoint[]={35,5,15,24*35,35,15};

void time_to_dbtime(dbtime_t *t,unsigned int time)
{
	struct sysclock clk;
	utime_to_sysclock(time,&clk);
	t->s.year = clk.year;
	t->s.month = clk.month;
	t->s.day = clk.day;
	t->s.tick = clk.hour*4;	
}
#define TN	3
void test_fre(void)
{

	unsigned int i,j,k,h;
	dbtime_t dbtime;
	utime_t time1 = 3600*24*365*10;
	utime_t time2=time1;
	unsigned char cn1,cn2,testbuf[36];
	
	cn1=cn2=0;
	while(1)
	{
		for(j=0;j<tpoint[TN];j++){
			
			time1+=3600;
			time_to_dbtime(&dbtime,time1);
			cn1 = 0;
			for(k=0;k<64;k++){
				cn1++;
				memset(testbuf,cn1,sizeof testbuf);
				db_writefalsh_bin(TN,k,testbuf,sizeof testbuf,dbtime);
				delay_us(10000);
			}
			
		}
		utime_t time1 = 3600*24*365*10;
		for(j=0;j<tpoint[TN];j++){
			time1+=3600;
			time_to_dbtime(&dbtime,time1);
			
			cn1=0;
			for(k=0;k<64;k++){
				cn1++;
				memset(testbuf,0,sizeof testbuf);
				db_readfalsh_bin(TN,k,testbuf,sizeof testbuf,dbtime);
				for(h=0;h<sizeof testbuf;h++)
					if(testbuf[h]!=cn1)
						break;
				if(h==sizeof testbuf)
					printdbg(PRINT_NORMAL,"day freeze test day:%d,meter:%d ok\n",time2/24/3600,i);
				else
					printdbg(PRINT_NORMAL,"day freeze test day:%d,meter:%d err\n",time2/24/3600,i);
				delay_us(20000);
			}
		}
	}
}


#endif

