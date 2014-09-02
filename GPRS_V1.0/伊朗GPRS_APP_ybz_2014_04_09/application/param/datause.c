/**
* datause.c -- 支持数据配置参数
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-8
* 最后修改时间: 2009-5-8
*/

//#include <stdio.h>
#include <string.h>

#define DEFINE_PARADATAUSE

#include "plat_include/debug.h"
#include "app_include/param/datause.h"
#include "plat_include/bin.h"
#include "app_include/param/operation.h"
#include "app_include/param/operation_inner.h"
#include "app_include/param/meter.h"

para_datause_t ParaDataUse;

#define Para_datause_buf para_rwbuf   

const cfg_datafns_t ValidDataCls1_1 = {
	19,
	{
	0xfe, 0x03, 0x00, 0xff,
	0xff, 0xff, 0x01, 0x03,
	0x00, 0x00, 0x00, 0xff,
	0x7f, 0xff, 0x0f, 0x00,
	0xff, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	}
};

const cfg_datafns_t ValidDataCls2_1 = {
	14,
	{
	0xff, 0x0f, 0xff, 0xff,
	0xff, 0x0c, 0x3f, 0x00,
	0x00, 0x00, 0xff, 0xff,
	0xff, 0x3f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	}
};

const cfg_datafns_t ValidDataCls1_2 = {
	19,
	{
	0xfe, 0x03, 0x00, 0xff,
	0xff, 0xff, 0x01, 0x03,
	0x00, 0x00, 0x00, 0xff,
	0x00, 0x00, 0x00, 0x00,
	0xff, 0xff, 0xff, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 
	}
};

const cfg_datafns_t ValidDataCls2_2 = {
	23,
	{
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x10, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x01, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	}
};

#define enepa    0x00000001
#define enena    0x00000002
#define enepa1   0x00000004
#define enena1   0x00000008
#define eneabc   0x00000010
#define enei4    0x00000020
#define enei     0x00000040
#define dmnpa    0x00000080
#define dmnpa1   0x00000100
#define dmnna    0x00000200
#define dmnna1   0x00000400
#define pwrt     0x00000800
#define pwrabc   0x00001000
#define volabc   0x00002000
#define curabc   0x00004000
#define amp      0x00008000
#define enebuy   0x00010000
#define enefebuy 0x00020000
/**
* @brief 载入缺省支持数据参数
*/
static void GetDefParaDataUse(unsigned char type,unsigned char usrcls, unsigned char metcls, cfg_datafns_t *p)
{
	unsigned int mask = 0;
	
	memset(p,0,sizeof(cfg_datafns_t));
	
	/******F38******/ 
	if(38 == type){
		// 大类号0 默认
		if(0 == usrcls){
			p->num = 17;
			p->fnmask[16] = 0x01;//F129
			
			return; 
		}
		//C类
		if(USRCLS_C==usrcls)
		{
		    if(metcls<4)
		    {
				p->num = 19;
				p->fnmask[16] = 0x0f;  //F129~F132
				p->fnmask[18] = 0x01;  //F145
			}
			else if((metcls>=4)&&(metcls<10))
			{
				p->num = 17;
				p->fnmask[16] = 0x01;  //F129
			}
			else if(metcls==10)
			{
				p->num = 19;
				p->fnmask[16] = 0x01;  //F129
				p->fnmask[18] = 0x01;  //F145
			}
			else if((metcls>=11)&&(metcls<13))
			{
				p->num = 19;
				p->fnmask[16] = 0x0f;  //F129~F132
				p->fnmask[18] = 0x05;  //F145/F147
				
			}
			else{
				goto mark38;
			}
			
			return; 
		}

	
		//D类
		if(USRCLS_D==usrcls){
		    if(metcls<4)
		    {
				p->num = 17;
				p->fnmask[16] = 0x05;  //F129/F131
			}
			else if((metcls>=4)&&(metcls<6)){
				p->num = 17;
				p->fnmask[16] = 0x01;  //F129
			}
			else{
				goto mark38;
			}
			
			return;
		}

		//E类
		if(USRCLS_E==usrcls){
		    if(metcls<4)
		    {
				p->num = 19;
				p->fnmask[16] = 0x0f;  //F129~F132
				p->fnmask[18] = 0x01;  //F145
			}
			else if((metcls>=4)&&(metcls<10)){
				p->num = 17;
				p->fnmask[16] = 0x01;  //F129
			}
			else if((metcls>=10)&&(metcls<14)){
				p->num = 17;
				p->fnmask[16] = 0x05;  //F129/F131
			}
			else if((metcls>=14)&&(metcls<16)){
				p->num = 17;
				p->fnmask[16] = 0x01;  //F129
			}
			else{
				goto mark38;
			}
			
			return ;
		}

		//F类
		if(USRCLS_F==usrcls){
		    if(metcls==0){
				p->num = 17;
				p->fnmask[4] = 0x01;  //F33
				p->fnmask[16] = 0x01;  //F129
			}
			else if(metcls==5){
				p->num = 19;
				p->fnmask[3] = 0x41;  //F25/F31
				p->fnmask[4] = 0x07;  //F33/F34/F35
				p->fnmask[16] = 0xff;  //F129~F136
				p->fnmask[18] = 0x01;  //F145
			}
			else{
				goto mark38;
			}
			
			return ;
	    }

mark38:
		p->num = 17;
		p->fnmask[16] = 0x01;//F129
		
		return ;

	}
	else if(39==type)
	{
		//C类
		if(USRCLS_C==usrcls)
		{
		    if((metcls==0)||(metcls==2)) mask |=enena+enepa1+enena1+eneabc+enei4+enei+dmnpa+pwrt+volabc+curabc+enebuy;
		    else if((metcls==1)||(metcls==3)) mask |=enepa+enena+enepa1+enena1+eneabc+enei4+enei+dmnpa+pwrt+volabc+curabc;
		    else if((metcls==4)||(metcls==5))mask |= enepa;
		    else if(metcls==10) mask |= enepa+dmnpa;
		    else if(metcls==11) mask |= enepa+enena+dmnpa+dmnna+pwrt+pwrabc+volabc+curabc+amp+enepa1+enena1+eneabc+enei4+enei+dmnpa1+dmnna1;
		    else if(metcls==12) mask |= enepa+enena+dmnpa+dmnna+pwrt+pwrabc+volabc+curabc+amp+enepa1+enena1+eneabc+enei4+enei+dmnpa1+dmnna1;
		    else if((metcls==6)||(metcls==8)) mask |= enepa+enebuy;
		    else if((metcls==7)||(metcls==9)) mask |=enepa;
	    }
	    else if(USRCLS_D==usrcls)
	    {
			if((metcls==0)||(metcls==2)) mask |=enepa+enena+pwrt+volabc+curabc+amp+enebuy;
		    else if((metcls==1)||(metcls==3)) mask |=enepa+enena+pwrt+volabc+curabc+amp;
		    else if((metcls==4)||(metcls==5))mask |= enepa+enena;
	    }
	    else if(USRCLS_E==usrcls)
	    {
		  if((metcls==0)||(metcls==2)) mask |=enena+enepa1+enena1+eneabc+enei4+enei+dmnpa+pwrt+volabc+curabc+enebuy;
		    else if((metcls==1)||(metcls==3)) mask |=enepa+enena+enepa1+enena1+eneabc+enei4+enei+dmnpa+pwrt+volabc+curabc;
		    else if((metcls==4)||(metcls==5))mask |= enepa;
		    else if((metcls==6)||(metcls==8)) mask |= enepa+enebuy;
		    else if((metcls==7)||(metcls==9)) mask |=enepa;
		    else if((metcls==10)||(metcls==12)) mask |=enepa+enena+pwrt+volabc+curabc+amp+enebuy;
		    else if((metcls==11)||(metcls==13)) mask |=enepa+enena+pwrt+volabc+curabc+amp;
		    else if((metcls==14)||(metcls==15))mask |= enepa+enena;
	    }
	    else if((USRCLS_A==usrcls)||(USRCLS_B==usrcls)||(USRCLS_F==usrcls))
	    {
			mask |=enepa+enena+enepa1+enena1+eneabc+enei4+enei+dmnpa+dmnna+pwrt+pwrabc+volabc+curabc;
	    }
	
		p->num = 27;

		/*正向有功总电能示值*/
		p->fnmask[0] = 0x01;	//F001
		p->fnmask[1] = 0x01;	//F009
		p->fnmask[2] = 0x01;	//F017
		p->fnmask[12] = 0x10;	//F101
		p->fnmask[20] = 0x01;	//F161
		p->fnmask[21] = 0x01;	//F169
		p->fnmask[22] = 0x01;	//F177
		
		// 大类号0 默认
		if(0 == usrcls)	return; 

		if(mask&enena){
			/*反向有功总电能示值*/
			p->fnmask[0] |= 0x02;	//F002
			p->fnmask[1] |= 0x02;	//F010
			p->fnmask[2] |= 0x02;	//F018
			p->fnmask[12] |= 0x40;	//F103
			p->fnmask[20] |= 0x04;	//F163
			p->fnmask[21] |= 0x04;	//F171
			p->fnmask[22] |= 0x04;	//F179
		}
		if(mask&enepa1){
			/*正向分时有功电能示值*/
			p->fnmask[0] |= 0x01;	//F001
			p->fnmask[1] |= 0x01;	//F009
			p->fnmask[2] |= 0x01;	//F017
			p->fnmask[20] |= 0x01;	//F161
			p->fnmask[21] |= 0x01;	//F169
			p->fnmask[22] |= 0x01;	//F177
		}
		/*反向分时有功电能示值*/
		if(mask&enena1){
			p->fnmask[0] |= 0x02;	//F002
			p->fnmask[1] |= 0x02;	//F010
			p->fnmask[2] |= 0x02;	//F018
			p->fnmask[20] |= 0x04;	//F163
			p->fnmask[21] |= 0x04;	//F171
			p->fnmask[22] |= 0x04;	//F179
		}
		
		/*分相有功电能示值*/
		if(mask&eneabc){
			p->fnmask[19] |= 0x01;	//F153
			p->fnmask[19] |= 0x10;	//F157
		}			
		
		/*四象限无功电能示值*/
		if(mask&enei4){
			p->fnmask[0] |= 0x03;	//F001 F002
			p->fnmask[1] |= 0x03;	//F009 F010
			p->fnmask[2] |= 0x03;	//F017 F018
			p->fnmask[18] |= 0x0f;	//F145-148
			p->fnmask[20] |= 0xf0;	//F165-F168
			p->fnmask[21] |= 0xf0;	//F173-F176
			p->fnmask[22] |= 0xf0;	//F181-F184
		}
		/*正反向无功电能示值*/
		if(mask&enei){
			p->fnmask[0] |= 0x03;	//F001 F002
			p->fnmask[1] |= 0x03;	//F009 F010
			p->fnmask[2] |= 0x03;	//F017 F018
			p->fnmask[12] |= 0xa0;	//F102/104
			p->fnmask[20] |= 0x0a;	//F162 F164
			p->fnmask[21] |= 0x0a;	//F170 F172
			p->fnmask[22] |= 0x0a;	//F178 F180
		}
		/*正向有功最大需量
		  分时正向有功需量
		*/
		if((mask&dmnpa)||(mask&dmnpa1)){
			p->fnmask[0] |= 0x04;	//F003
			p->fnmask[1] |= 0x04;	//F011
			p->fnmask[2] |= 0x04;	//F019
			p->fnmask[23] |= 0x01;	//F185
			p->fnmask[23] |= 0x04;
			p->fnmask[23] |= 0x10;	//F189
			p->fnmask[24] |= 0x01;	//F193
		}
		/*反向有功最大需量
		分时反向有功需量*/
		if((mask&dmnna)||(mask&dmnna1))
		{
			p->fnmask[0] |= 0x08;	//F004
			p->fnmask[1] |= 0x08;	//F012
			p->fnmask[2] |= 0x08;	//F020
			p->fnmask[23] |= 0x01;	//F187
			p->fnmask[23] |= 0x40;	//F191
			p->fnmask[24] |= 0x04;	//F195
		}
		/*总有功功率*/
		if(mask&pwrt) p->fnmask[10] |= 0x01;	//F081
		
		/*分相有功功率*/
		if(mask&pwrabc) p->fnmask[10] |= 0x0e;	//F082-F084
		/*各相电压*/
		if(mask&volabc) p->fnmask[11] |= 0x07;	//F089-F091
		/*各相电流*/
		if(mask&curabc) p->fnmask[11] |= 0x38;	//F092-F094
		/*零序电流*/

		if(mask&amp)  p->fnmask[11] |= 0x40;	//F095
		/*购电信息 结算信息*/
		if((mask&enebuy)||(mask&enefebuy))p->fnmask[26] |= 0xc0;	//F215 F216
	}
	#if 0	
		//C类
		if(USRCLS_C==usrcls)
		{
		    if(metcls<4)
		    {
				p->num = 27;
				p->fnmask[0] = 0x0f;	//F001-F004
				p->fnmask[1] = 0x0f;	//F009-F012
				p->fnmask[2] = 0x0f;	//F017-F020

				p->fnmask[10] = 0x0f;	//F081-F084
				p->fnmask[11] = 0x3f;	//F089-F094
				p->fnmask[12] = 0xf0;  //F101~F104
				p->fnmask[18] = 0x0f;  //F145~F148
				p->fnmask[19] = 0x11;  //F153 F157
				p->fnmask[20] = 0xff;  //F161~F168
				p->fnmask[21] = 0xff;  //F169~F176
				p->fnmask[22] = 0xff;  //F177~F184
				p->fnmask[23] = 0x55;  //F185/F187/F189/F191
				p->fnmask[24] = 0x05;  //F193 F195
				p->fnmask[26] = 0xc1;  //F209 F215 F216

			}
			else if((metcls>=4)&&(metcls<10))
			{
				p->num = 23;
				p->fnmask[12] = 0x10;  //F101
				p->fnmask[20] = 0x01;  //F161
				p->fnmask[21] = 0x01;  //F169
				p->fnmask[22] = 0x01;  //F177
			}
			else if(metcls==10)
			{
		        p->num = 25;
				p->fnmask[12] = 0x10;  //F101
				p->fnmask[20] = 0x01;  //F161
				p->fnmask[21] = 0x01;  //F169
				p->fnmask[22] = 0x01;  //F177
				p->fnmask[23] = 0x11;  //F185/F189
				p->fnmask[24] = 0x01;  //F193
			}
			else if((metcls>=11)&&(metcls<13))
			{
				p->num = 25;
				p->fnmask[12] = 0xf0;  //F101~F104
				p->fnmask[20] = 0x0f;  //F161~F164
				p->fnmask[21] = 0x0f;  //F169~F172
				p->fnmask[22] = 0x0f;  //F177~F180
				p->fnmask[23] = 0x55;  //F185/F187/F189/F191
				p->fnmask[24] = 0x05;  //F193/F195
			}
			else{
				goto mark39;
			}
			
			return; 
		}

	
		//D类
		if(USRCLS_D==usrcls){
		    if(metcls<4)
		    {
				p->num = 23;
				p->fnmask[12] = 0x50;  //F101/F103
				p->fnmask[20] = 0x05;  //F161/F163
				p->fnmask[21] = 0x05;  //F169/F171
				p->fnmask[22] = 0x05;  //F177~F179
			}
			else if((metcls>=4)&&(metcls<6)){
				p->num = 23;
				p->fnmask[12] = 0x10;  //F101
				p->fnmask[20] = 0x01;  //F161
				p->fnmask[21] = 0x01;  //F169
				p->fnmask[22] = 0x01;  //F177
			}
			else{
				goto mark39;
			}
			
			return;
		}

		//E类
		if(USRCLS_E==usrcls){
		    if(metcls<4)
		    {
				p->num = 25;
				p->fnmask[12] = 0xf0;  //F101~F104
				p->fnmask[20] = 0x0f;  //F161~F164
				p->fnmask[21] = 0x0f;  //F169~F172
				p->fnmask[22] = 0x0f;  //F177~F180
				p->fnmask[23] = 0x11;  //F185/F189
				p->fnmask[24] = 0x01;  //F193
			}
			else if((metcls>=4)&&(metcls<10)){
				p->num = 23;
				p->fnmask[12] = 0x10;  //F101
				p->fnmask[20] = 0x01;  //F161
				p->fnmask[21] = 0x01;  //F169
				p->fnmask[22] = 0x01;  //F177
			}
			else if((metcls>=10)&&(metcls<14)){
				p->num = 23;
				p->fnmask[12] = 0x50;  //F101/F103
				p->fnmask[20] = 0x05;  //F161/F163
				p->fnmask[21] = 0x05;  //F169/F171
				p->fnmask[22] = 0x05;  //F177~F179
			}
			else if((metcls>=14)&&(metcls<16)){
				p->num = 23;
				p->fnmask[12] = 0x10;  //F101
				p->fnmask[20] = 0x01;  //F161
				p->fnmask[21] = 0x01;  //F169
				p->fnmask[22] = 0x01;  //F177
			}
			else{
				goto mark39;
			}
			
			return ;
		}

		//F类
		if(USRCLS_F==usrcls){
		    if(metcls==0){
				p->num = 23;
		        p->fnmask[0] = 0x01;	//F1
		        p->fnmask[2] = 0x01;	//F17
				p->fnmask[12] = 0x10;//F101
				p->fnmask[20] = 0x01;//F161
				p->fnmask[22] = 0x01;//F177
			}
			else if(metcls==5){
				p->num = 25;
		        p->fnmask[0] = 0x07;	//F1~F3
		        p->fnmask[1] = 0x07;	//F9~F11
		        p->fnmask[2] = 0x07;	//F17~F19
		        p->fnmask[3] = 0xFF;	//F25~32
		        p->fnmask[4] = 0x7F;	//F33~F39
		        p->fnmask[10] = 0xFF;//F81~F88
				p->fnmask[12] = 0xF0;//F101~104
		        p->fnmask[13] = 0x0F;//F105~F108
		        p->fnmask[18] = 0x0F;//F145~F148
				p->fnmask[19] = 0xFF;//F153~F160
				p->fnmask[20] = 0xFF;//F161~168
				p->fnmask[21] = 0xFF;//F169~F176
				p->fnmask[22] = 0xFF;//F177~F184
				p->fnmask[23] = 0x11;//F185 F189
				p->fnmask[24] = 0x01;//F193
			}
			else{
				goto mark39;
			}
			
			return ;
	    } 
mark39:
		p->num = 23;
		p->fnmask[12] = 0x10;	//F101
		p->fnmask[20] = 0x01;	//F161
		p->fnmask[22] = 0x01;	//F177
		
		return; 
	
#endif
     
}

/**
* @brief 载入缺省支持数据参数
*/
static void LoadDefParaDataUse(para_datause_all_t * p,char flg)
{
	int usrcls,metcls;

	if(p==NULL) return;
	
	memset(p, 0, sizeof(para_datause_all_t));

	if(flg == 1)
	{
		for(usrcls=0;usrcls<=USRCLS_F;usrcls++){
			for(metcls=0; metcls<MAX_METCLASS; metcls++){
				GetDefParaDataUse(38,usrcls,metcls,&p->datacls1[usrcls].met[metcls]);
			}

		}
	}
	else if(flg == 2)
	{
		for(usrcls=0;usrcls<=USRCLS_F;usrcls++){
			for(metcls=0; metcls<MAX_METCLASS; metcls++){
				GetDefParaDataUse(39,usrcls,metcls,&p->datacls2[usrcls].met[metcls]);
			}

		}
	}

}

static void DUseCls1Updata(para_datause_all_t * p)
{
	unsigned int i,j,usr,met,mid;
	unsigned int usrcls[CLS1_MAX_ITERMS];

	memset(usrcls,0,sizeof(usrcls));
	for(mid=0;mid<MAX_CENMETP;mid++){
		if(EMPTY_METER(mid)) continue;
		if(0==ParaMeter[mid].usrclass)continue;
		if((ParaMeter[mid].usrclass>>4)>USRCLS_F)continue;

		for(i=0;i<CLS1_MAX_ITERMS;i++)
		{
			if(usrcls[i]==ParaMeter[mid].usrclass) break;
		}

		if(i==CLS1_MAX_ITERMS){

			for(j=0;j<CLS1_MAX_ITERMS;j++){
				if(usrcls[j]==0) break;
			}
			
			if(j<CLS1_MAX_ITERMS){
				usrcls[j] = ParaMeter[mid].usrclass;
			}
			else{
				AssertLog(1,">MAX_ITERMS(%d)!\n",CLS1_MAX_ITERMS);
				break;
			}

		}
		else continue;
	}

    for(i=0;i<CLS1_MAX_ITERMS;i++){
		if(0==usrcls[i]) continue;
		
		usr = (usrcls[i]>>4);
		met = (usrcls[i]&0x0f);
		ParaDataUse.datacls1[i].usrcls = usr;				
		ParaDataUse.datacls1[i].metcls = met;
		if(p==NULL)  continue;
		ParaDataUse.datacls1[i].met.num = p->datacls1[usr].met[met].num;					
		smallcpy(ParaDataUse.datacls1[i].met.fnmask,p->datacls1[usr].met[met].fnmask,p->datacls1[usr].met[met].num);
    }

}

static void DUseCls2Updata(para_datause_all_t * p)
{
	unsigned int i,j,usr,met,mid;
	unsigned int usrcls[CLS2_MAX_ITERMS];

	memset(usrcls,0,sizeof(usrcls));
	for(mid=0;mid<MAX_CENMETP;mid++){
		if(EMPTY_METER(mid)) continue;
		if(0==ParaMeter[mid].usrclass)continue;
		if((ParaMeter[mid].usrclass>>4)>USRCLS_F)continue;

		for(i=0;i<CLS2_MAX_ITERMS;i++)
		{
			if(usrcls[i]==ParaMeter[mid].usrclass) break;
		}

		if(i==CLS2_MAX_ITERMS){

			for(j=0;j<CLS2_MAX_ITERMS;j++){
				if(usrcls[j]==0) break;
			}
			
			if(j<CLS2_MAX_ITERMS){
				usrcls[j] = ParaMeter[mid].usrclass;
			}
			else{
				AssertLog(1,">MAX_ITERMS(%d)!\n",CLS2_MAX_ITERMS);
				break;
			}
		}
		else continue;
	}

    for(i=0;i<CLS2_MAX_ITERMS;i++){
		if(0==usrcls[i]) continue;
		
		usr = (usrcls[i]>>4);
		met = (usrcls[i]&0x0f);
		ParaDataUse.datacls2[i].usrcls = usr;				
		ParaDataUse.datacls2[i].metcls = met;
		if(p == NULL) continue;
		ParaDataUse.datacls2[i].met.num = p->datacls2[usr].met[met].num;					
		smallcpy(ParaDataUse.datacls2[i].met.fnmask,p->datacls2[usr].met[met].fnmask,p->datacls2[usr].met[met].num);
    }
    
}

void DUseClsUpdata(void){
	int rt;
	para_datause_all_t * ParaDataUseAll = (para_datause_all_t*)Para_datause_buf;

	memset(ParaDataUseAll,0,sizeof(ParaDataUseAll));
	
	rt = para_readflash_bin(PARA_FILEINDEX_DATAUSE1,(unsigned char*)ParaDataUseAll,sizeof(para_datause_all_t));
	if(rt==-1)LoadDefParaDataUse(ParaDataUseAll,1);
	DUseCls1Updata(ParaDataUseAll);
	
	rt = para_readflash_bin(PARA_FILEINDEX_DATAUSE2,(unsigned char *)ParaDataUseAll,sizeof(para_datause_all_t));
	if(rt==-1)LoadDefParaDataUse(ParaDataUseAll,2);
	DUseCls2Updata(ParaDataUseAll);

	return;
}

/**
* @brief 从文件中载入支持数据参数
* @return 0成功, 否则失败
*/
int LoadParaDataUse(void)
{
	DUseClsUpdata();

	return 0;
}

/**
* @brief 终端参数F38操作(1类数据配置设置 (在终端支持的1类数据配置内))
* @param flag 操作方式, 0-读, 1-写
* @param option 操作选项
* @return 0成功, 否则失败(参看POERR_XXX宏定义)
*/
int ParaOperationF38(int flag, unsigned short metpid, param_specialop_t *option)
{
	int rt;
	para_datause_all_t * ParaDataUseAll = (para_datause_all_t*)Para_datause_buf;
	
	rt = para_readflash_bin(PARA_FILEINDEX_DATAUSE1,(unsigned char *)ParaDataUseAll,sizeof(para_datause_all_t));
	if(rt==-1)LoadDefParaDataUse(ParaDataUseAll,1);
	
	if(0 == flag) {
		int usercls, num, i, metcls, actnum;
		const unsigned char *prbuf;
		unsigned char *pwbuf;
		unsigned short wlen, tmplen;

		usercls = (int)option->rbuf[0]&0xff;
		num = (int)option->rbuf[1]&0xff;
		option->ractlen = num + 2;
		if(option->rlen < option->ractlen) return POERR_FATAL;
		if(usercls >= MAX_USERCLASS) return POERR_INVALID;

		wlen = 2;
		prbuf = option->rbuf + 2;
		pwbuf = option->wbuf + 2;
		actnum = 0;
		for(i=0; i<num; i++,prbuf++) {
			metcls = (int)prbuf[0]&0xff;
			if(metcls > MAX_METCLASS) continue;

			tmplen = (int)ParaDataUseAll->datacls1[usercls].met[metcls].num & 0xff;
			if((wlen+tmplen+2) > option->wlen) break;

			*pwbuf++ = prbuf[0];
			*pwbuf++ = ParaDataUseAll->datacls1[usercls].met[metcls].num;
			if(tmplen) memcpy(pwbuf, ParaDataUseAll->datacls1[usercls].met[metcls].fnmask, tmplen);
			pwbuf += tmplen;

			wlen += tmplen+2;
			actnum++;
		}

		option->wactlen = wlen;
		option->wbuf[0] = usercls;
		option->wbuf[1] = actnum;
	}
	else {
		int usercls, num, i, metcls;
		const unsigned char *prbuf;
		int tmplen, fidnum;

		usercls = (int)option->rbuf[0]&0xff;
		num = (int)option->rbuf[1]&0xff;
		if(usercls >= MAX_USERCLASS) return POERR_FATAL;

		option->ractlen = 2;
		prbuf = option->rbuf + 2;
		for(i=0; i<num; i++) {
			metcls = (int)(*prbuf++)&0xff;
			if(metcls >= MAX_METCLASS) return POERR_FATAL;
			
			tmplen = (int)(*prbuf++)&0xff;
			option->ractlen += tmplen+2;
			if(option->ractlen > option->rlen) return POERR_FATAL;
			if(tmplen > 31) fidnum = 31;
			else fidnum = tmplen;

			ParaDataUseAll->datacls1[usercls].met[metcls].num = fidnum;
			if(tmplen) memcpy(ParaDataUseAll->datacls1[usercls].met[metcls].fnmask, prbuf, fidnum);
			prbuf += tmplen;
		}
		rt = para_writeflash_bin(PARA_FILEINDEX_DATAUSE1,(unsigned char *)ParaDataUseAll,sizeof(para_datause_all_t));
		AssertLog(rt==-1,"write para_datause cls1 failed!");
		DUseCls1Updata(ParaDataUseAll);
	}

	return 0;
}

/**
* @brief 终端参数F39操作
* @param flag 操作方式, 0-读, 1-写
* @param option 操作选项
* @return 0成功, 否则失败(参看POERR_XXX宏定义)
*/
int ParaOperationF39(int flag, unsigned short metpid, param_specialop_t *option)
{
	int rt;
	para_datause_all_t * ParaDataUseAll = (para_datause_all_t*)Para_datause_buf;

	rt = para_readflash_bin(PARA_FILEINDEX_DATAUSE2,(unsigned char *)ParaDataUseAll,sizeof(para_datause_all_t));
	if(rt==-1)LoadDefParaDataUse(ParaDataUseAll,2);
	
	if(0 == flag) {
		int usercls, num, i, metcls, actnum;
		const unsigned char *prbuf;
		unsigned char *pwbuf;
		unsigned short wlen, tmplen;

		///readflash(1,mid,ParaDataUseAll,sizeof(para_datause_all_t));

		usercls = (int)option->rbuf[0]&0xff;
		num = (int)option->rbuf[1]&0xff;
		option->ractlen = num + 2;
		if(option->rlen < option->ractlen) return POERR_FATAL;
		if(usercls >= MAX_USERCLASS) return POERR_INVALID;

		wlen = 2;
		prbuf = option->rbuf + 2;
		pwbuf = option->wbuf + 2;
		actnum = 0;
		for(i=0; i<num; i++,prbuf++) {
			metcls = (int)prbuf[0]&0xff;
			if(metcls >= MAX_METCLASS) continue;

			tmplen = (int)ParaDataUseAll->datacls2[usercls].met[metcls].num & 0xff;
			if((wlen+tmplen+2) > option->wlen) break;

			*pwbuf++ = prbuf[0];
			*pwbuf++ = ParaDataUseAll->datacls2[usercls].met[metcls].num;
			if(tmplen) memcpy(pwbuf, ParaDataUseAll->datacls2[usercls].met[metcls].fnmask, tmplen);
			pwbuf += tmplen;

			wlen += tmplen+2;
			actnum++;
		}

		option->wactlen = wlen;
		option->wbuf[0] = usercls;
		option->wbuf[1] = actnum;
	}
	else {
		int usercls, num, i, metcls;
		const unsigned char *prbuf;
		int tmplen, fidnum;

		usercls = (int)option->rbuf[0]&0xff;
		num = (int)option->rbuf[1]&0xff;
		if(usercls >= MAX_USERCLASS) return POERR_FATAL;

		option->ractlen = 2;
		prbuf = option->rbuf + 2;
		for(i=0; i<num; i++) {
			metcls = (int)(*prbuf++)&0xff;
			if(metcls >= MAX_METCLASS) return POERR_FATAL;

			tmplen = (int)(*prbuf++)&0xff;
			option->ractlen += tmplen+2;
			if(option->ractlen > option->rlen) return POERR_FATAL;
			if(tmplen > 31) fidnum = 31;
			else fidnum = tmplen;

			ParaDataUseAll->datacls2[usercls].met[metcls].num = fidnum;
			if(tmplen) memcpy(ParaDataUseAll->datacls2[usercls].met[metcls].fnmask, prbuf, fidnum);
			prbuf += tmplen;
		}

		rt = para_writeflash_bin(PARA_FILEINDEX_DATAUSE2,(unsigned char *)ParaDataUseAll,sizeof(para_datause_all_t));
		AssertLog(rt==-1,"write para_datause cls2 failed!");
		DUseCls2Updata(ParaDataUseAll);
	}

	return 0;
}

int DUseCls1FnVaid(unsigned char usrcls, unsigned char metcls, unsigned char fn)
{
	unsigned char fnmask;
	unsigned char mask;
	unsigned char i;
	cfg_datafns_t deffns;
	int rt;

	fn -= 1;
	
	if((0==metcls)&&(0==usrcls))
	{
		GetDefParaDataUse(39,usrcls,metcls,&deffns);
		fnmask = deffns.fnmask[fn>>3];
		goto mark;
	}
	else if(usrcls>USRCLS_F)
	{
		return 0;
	}
	
	for(i=0;i<CLS1_MAX_ITERMS;i++){
		if((usrcls == ParaDataUse.datacls1[i].usrcls)&&(metcls == ParaDataUse.datacls1[i].metcls)) break;

	}
	if(i<CLS1_MAX_ITERMS) fnmask = ParaDataUse.datacls1[i].met.fnmask[fn>>3];
	else 
	{
		GetDefParaDataUse(38,usrcls,metcls,&deffns);
		fnmask = deffns.fnmask[fn>>3];
	}
	
mark:	
	mask = 1<<(fn&0x07);
	rt = ((fnmask&mask)?1:0);

	return rt;
}

int DUseCls2FnVaid(unsigned char usrcls, unsigned char metcls, unsigned char fn)
{
	unsigned char fnmask;
	unsigned char mask;
	unsigned char i;
	cfg_datafns_t deffns;
	int rt;
	
	fn -= 1;
	if((0==metcls)&&(0==usrcls))
	{
		GetDefParaDataUse(39,usrcls,metcls,&deffns);
		fnmask = deffns.fnmask[fn>>3];
		goto mark;
	}
	else if(usrcls>USRCLS_F)
	{
		return 0;
	}
	
	for(i=0;i<CLS2_MAX_ITERMS;i++){
		if((usrcls == ParaDataUse.datacls2[i].usrcls)&&(metcls == ParaDataUse.datacls2[i].metcls)) break;

	}
	
	//PrintLog(LOGTYPE_DEBUG,"usrcls(%d) metcls(%d) i(%d)\n",usrcls,metcls,i);

	if(i<CLS2_MAX_ITERMS) 
	{
		fnmask = ParaDataUse.datacls2[i].met.fnmask[fn>>3];
	}
	else 
	{
		GetDefParaDataUse(39,usrcls,metcls,&deffns);
		fnmask = deffns.fnmask[fn>>3];
	}

mark:	
	mask = 1<<(fn&0x07);
	rt = ((fnmask&mask)?1:0);
	return rt;
}


