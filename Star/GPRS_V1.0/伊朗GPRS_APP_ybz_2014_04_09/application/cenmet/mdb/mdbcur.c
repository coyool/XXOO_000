/**
* mdbcur.c -- 当前表计数据
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-10
* 最后修改时间: 2009-5-10
*/

//#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define DEFINE_MDBCURRENT

#include "plat_include/debug.h"
#include "plat_include/mutex.h"
#include "app_include/param/capconf.h"
#include "app_include/cenmet/mdbcur.h"
#include "plat_include/timeal.h"
#include "app_include/lib/bcd.h"
#include "app_include/monitor/alarm.h"
#include "app_include/cenmet/cenmet_comm.h"
#include "app_include/cenmet/cenmet_proto.h"
#include "app_include/monitor/runstate.h"
#include "app_include/param/metp.h"
#include "app_include/param/meter.h"

mdbcur_t MdbCurrent[MdbCurNum];

//static sys_mutex_t MdbCurMutex;

void MdbCurrentReInit(unsigned char id)
{
	LockMdbCurrent();
	//PrintLog(LOGTYPE_DEBUG,"**MdbCurrentReInit user(%d)\n",id);
	memset(&MdbCurrent[id], FLAG_MDBEMPTY, sizeof(mdbcur_t));
	MdbCurrent[id].syntony_value[0]  = 0;
	MdbCurrent[id].syntony_rating[0] = 0;

	UnlockMdbCurrent();
}

void MdbCurrentReInit2(unsigned char id)
{
	LockMdbCurrent();
	//PrintLog(LOGTYPE_DEBUG,"**MdbCurrentReInit2 user(%d)\n",id);
	memset(MdbCurrent[id].pwra, FLAG_MDBFAIL, sizeof(mdbcur_t)-offsetof(mdbcur_t,pwra));
	MdbCurrent[id].syntony_value[0]  = 0;
	MdbCurrent[id].syntony_rating[0] = 0;

	UnlockMdbCurrent();
}

/**
* @brief 锁住MdbCurrent(防止异步读写)
*/
void LockMdbCurrent(void)
{
	//SysLockMutex(&MdbCurMutex);
	//@unuse
}

/**
* @brief 解锁MdbCurrent
*/
void UnlockMdbCurrent(void)
{
	//SysUnlockMutex(&MdbCurMutex);
	//@unuse
}

/**
* @brief 当前表计数据初始化
* @return 成功0, 否则失败
*/
int MdbCurrentInit(void)
{
//	SysInitMutex(&MdbCurMutex);

	MdbCurrentReInit(MdbCudIdSvr);
	MdbCurrentReInit(MdbCurIdSave);
	return 0;
}

#define MCPYFLAG_GROUP		0x80
#define MCPYFLAG_LSHIFT		0x40  //multi
#define MCPYFLAG_RSHIFT		0x20  //div
#define MCPYFLAG_DL2007		0x10  //DL/T645-2007特殊格式数据
typedef struct {
	unsigned short itemid;
	unsigned char flag;  //低4位为偏移值
	unsigned char len;  //低4位为GBlen, 高4位为DL645len
	unsigned char *pbuf;
} mdbcur_cpy_t;

static const mdbcur_cpy_t MdbCopyB6XX[] = {
	{0xb63f, MCPYFLAG_GROUP, 4,  NULL}, 
	{0xb630, 0, 0x33, &mdbcur(0).pwra[0]}, 
	{0xb631, 0, 0x33, &mdbcur(0).pwra[3]}, 
	{0xb632, 0, 0x33, &mdbcur(0).pwra[6]}, 
	{0xb633, 0, 0x33, &mdbcur(0).pwra[9]}, 

	{0xb64f, MCPYFLAG_GROUP, 4, NULL}, 
	{0xb640, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x02, 0x23, &mdbcur(0).pwri[0]}, 
	{0xb641, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x02, 0x23, &mdbcur(0).pwri[3]}, 
	{0xb642, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x02, 0x23, &mdbcur(0).pwri[6]}, 
	{0xb643, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x02, 0x23, &mdbcur(0).pwri[9]}, 

	{0xb65f, MCPYFLAG_GROUP, 4, NULL}, 
	{0xb650, 0, 0x22, &mdbcur(0).pwrf[0]}, 
	{0xb651, 0, 0x22, &mdbcur(0).pwrf[2]}, 
	{0xb652, 0, 0x22, &mdbcur(0).pwrf[4]}, 
	{0xb653, 0, 0x22, &mdbcur(0).pwrf[6]}, 

	{0xb61f, MCPYFLAG_GROUP, 3, NULL},
	{0xb611, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x01, 0x22, &mdbcur(0).vol[0]}, 
	{0xb612, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x01, 0x22, &mdbcur(0).vol[2]}, 
	{0xb613, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x01, 0x22, &mdbcur(0).vol[4]}, 

	{0xb62f, MCPYFLAG_GROUP, 3, NULL},
	{0xb621, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x01, 0x23, &mdbcur(0).amp[0]}, 
	{0xb622, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x01, 0x23, &mdbcur(0).amp[3]}, 
	{0xb623, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x01, 0x23, &mdbcur(0).amp[6]}, 

	{0xb624, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x01, 0x23, &mdbcur(0).amp[9]}, 

	{0xb66f, MCPYFLAG_GROUP, 6, NULL}, 
	{0xb660, 0, 0x22, &mdbcur(0).phase_arc[0]}, 
	{0xb661, 0, 0x22, &mdbcur(0).phase_arc[2]}, 
	{0xb662, 0, 0x22, &mdbcur(0).phase_arc[4]}, 
	{0xb663, 0, 0x22, &mdbcur(0).phase_arc[6]}, 
	{0xb664, 0, 0x22, &mdbcur(0).phase_arc[8]}, 
	{0xb665, 0, 0x22, &mdbcur(0).phase_arc[10]},

	{0xb6ef, MCPYFLAG_GROUP, 4,  NULL}, 
	{0xb6e0, 0, 0x33, &mdbcur(0).pwrv[0]}, 
	{0xb6e1, 0, 0x33, &mdbcur(0).pwrv[3]}, 
	{0xb6e2, 0, 0x33, &mdbcur(0).pwrv[6]}, 
	{0xb6e3, 0, 0x33, &mdbcur(0).pwrv[9]}, 
};

static const mdbcur_cpy_t MdbCopyB3XX[] = {
	{0xb31f, MCPYFLAG_GROUP, 4, NULL},
	{0xb310, 0, 0x22, &mdbcur(0).volbr_cnt[0]}, 
	{0xb311, 0, 0x22, &mdbcur(0).volbr_cnt[2]}, 
	{0xb312, 0, 0x22, &mdbcur(0).volbr_cnt[4]}, 
	{0xb313, 0, 0x22, &mdbcur(0).volbr_cnt[6]}, 

	{0xb32f, MCPYFLAG_GROUP, 4, NULL},
	{0xb320, 0, 0x33, &mdbcur(0).volbr_times[0]}, 
	{0xb321, 0, 0x33, &mdbcur(0).volbr_times[3]}, 
	{0xb322, 0, 0x33, &mdbcur(0).volbr_times[6]}, 
	{0xb323, 0, 0x33, &mdbcur(0).volbr_times[9]}, 

	{0xb33f, MCPYFLAG_GROUP, 4, NULL},
	{0xb330, 0, 0x44, &mdbcur(0).volbr_timestart[0]}, 
	{0xb331, 0, 0x44, &mdbcur(0).volbr_timestart[4]}, 
	{0xb332, 0, 0x44, &mdbcur(0).volbr_timestart[8]}, 
	{0xb333, 0, 0x44, &mdbcur(0).volbr_timestart[12]}, 

	{0xb34f, MCPYFLAG_GROUP, 4, NULL},
	{0xb340, 0, 0x44, &mdbcur(0).volbr_timeend[0]}, 
	{0xb341, 0, 0x44, &mdbcur(0).volbr_timeend[4]}, 
	{0xb342, 0, 0x44, &mdbcur(0).volbr_timeend[8]}, 
	{0xb343, 0, 0x44, &mdbcur(0).volbr_timeend[12]},
};

static const mdbcur_cpy_t MdbCopyC0XX[] = {
	{0xc01f, MCPYFLAG_GROUP, 2, NULL},
	{0xc010, MCPYFLAG_RSHIFT|0x02, 0x43, &mdbcur(0).met_clock[3]}, 
	//{0xc011, MCPYFLAG_RSHIFT|0x02, 0x32, &mdbcur(0).met_clock[0]}, 
	{0xc011, 0, 0x33, &mdbcur(0).met_clock[0]}, 

	{0xc020, 0, 0x11, &mdbcur(0).met_runstate[0]},
	{0xc021, 0, 0x11, &mdbcur(0).met_runstate[1]},
	{0xc02e, 0, 0xee, &mdbcur(0).flagchg_state[0]},
	{0xc02f, 0, 0xee, &mdbcur(0).met_runstate[0]},
};

static const mdbcur_cpy_t MdbCopyB2XX[] = {
	{0xb21f, MCPYFLAG_GROUP, 5, NULL},
	{0xb210, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x02, 0x46, &mdbcur(0).prog_time[0]}, 
	{0xb211, MCPYFLAG_DL2007|MCPYFLAG_LSHIFT|0x02, 0x46, &mdbcur(0).dmnclr_time[0]}, 
	{0xb212, MCPYFLAG_DL2007, 0x23, &mdbcur(0).prog_cnt[0]}, 
	{0xb213, MCPYFLAG_DL2007, 0x23, &mdbcur(0).dmnclr_cnt[0]}, 
	{0xb214, MCPYFLAG_DL2007, 0x34, &mdbcur(0).bat_runtime[0]},
	{0xb215, 0, 0x33, &mdbcur(0).metclr_cnt[0]},
	{0xb216, 0, 0x66, &mdbcur(0).metclr_time[0]},
	{0xb217, 0, 0x33, &mdbcur(0).evclr_cnt[0]},
	{0xb218, 0, 0x66, &mdbcur(0).evclr_time[0]},
	{0xb219, 0, 0x33, &mdbcur(0).chktime_cnt[0]},
	{0xb21a, 0, 0x66, &mdbcur(0).chktime_time[0]},
};

static const mdbcur_cpy_t MdbCopy90XX[] = {
	{0x901f, MCPYFLAG_GROUP, 5, NULL},
	{0x9010, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[0]}, 
	{0x9011, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[5]}, 
	{0x9012, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[10]}, 
	{0x9013, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[15]}, 
	{0x9014, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[20]}, 

	{0x902f, MCPYFLAG_GROUP, 5, NULL},
	{0x9020, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[0]}, 
	{0x9021, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[5]}, 
	{0x9022, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[10]}, 
	{0x9023, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[15]}, 
	{0x9024, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[20]}, 

	{0x903f, MCPYFLAG_GROUP, 3, NULL},
	{0x9031, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_a[0]}, //正有功分相
	{0x9032, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_b[0]}, 
	{0x9033, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_c[0]}, 

	{0x904f, MCPYFLAG_GROUP, 3, NULL},
	{0x9041, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_a[0]}, //反有功分相
	{0x9042, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_b[0]}, 
	{0x9043, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_c[0]}, 

	{0x905f, MCPYFLAG_GROUP, 3, NULL},
	{0x9051, 0, 0x44, &mdbcur(0).enepi_a[0]},//正无功 1,2象限分相之和
	{0x9052, 0, 0x44, &mdbcur(0).enepi_b[0]},
	{0x9053, 0, 0x44, &mdbcur(0).enepi_c[0]},

	{0x906f, MCPYFLAG_GROUP, 3, NULL},
	{0x9061, 0, 0x44, &mdbcur(0).eneni_a[0]},//反无功，3，4象限之和
	{0x9062, 0, 0x44, &mdbcur(0).eneni_b[0]},
	{0x9063, 0, 0x44, &mdbcur(0).eneni_c[0]},
};

static const mdbcur_cpy_t MdbCopy91XX[] = {
	{0x911f, MCPYFLAG_GROUP, 5, NULL},
	{0x9110, 0, 0x44, &mdbcur(0).enepi[0]}, 
	{0x9111, 0, 0x44, &mdbcur(0).enepi[4]}, 
	{0x9112, 0, 0x44, &mdbcur(0).enepi[8]}, 
	{0x9113, 0, 0x44, &mdbcur(0).enepi[12]}, 
	{0x9114, 0, 0x44, &mdbcur(0).enepi[16]}, 

	{0x913f, MCPYFLAG_GROUP, 5, NULL},
	{0x9130, 0, 0x44, &mdbcur(0).enepi1[0]}, 
	{0x9131, 0, 0x44, &mdbcur(0).enepi1[4]}, 
	{0x9132, 0, 0x44, &mdbcur(0).enepi1[8]}, 
	{0x9133, 0, 0x44, &mdbcur(0).enepi1[12]}, 
	{0x9134, 0, 0x44, &mdbcur(0).enepi1[16]}, 

	{0x914f, MCPYFLAG_GROUP, 5, NULL},
	{0x9140, 0, 0x44, &mdbcur(0).enepi4[0]}, 
	{0x9141, 0, 0x44, &mdbcur(0).enepi4[4]}, 
	{0x9142, 0, 0x44, &mdbcur(0).enepi4[8]}, 
	{0x9143, 0, 0x44, &mdbcur(0).enepi4[12]}, 
	{0x9144, 0, 0x44, &mdbcur(0).enepi4[16]}, 

	{0x912f, MCPYFLAG_GROUP, 5, NULL},
	{0x9120, 0, 0x44, &mdbcur(0).eneni[0]}, 
	{0x9121, 0, 0x44, &mdbcur(0).eneni[4]}, 
	{0x9122, 0, 0x44, &mdbcur(0).eneni[8]}, 
	{0x9123, 0, 0x44, &mdbcur(0).eneni[12]}, 
	{0x9124, 0, 0x44, &mdbcur(0).eneni[16]}, 

	{0x915f, MCPYFLAG_GROUP, 5, NULL},
	{0x9150, 0, 0x44, &mdbcur(0).eneni2[0]}, 
	{0x9151, 0, 0x44, &mdbcur(0).eneni2[4]}, 
	{0x9152, 0, 0x44, &mdbcur(0).eneni2[8]}, 
	{0x9153, 0, 0x44, &mdbcur(0).eneni2[12]}, 
	{0x9154, 0, 0x44, &mdbcur(0).eneni2[16]}, 

	{0x916f, MCPYFLAG_GROUP, 5, NULL},
	{0x9160, 0, 0x44, &mdbcur(0).eneni3[0]}, 
	{0x9161, 0, 0x44, &mdbcur(0).eneni3[4]}, 
	{0x9162, 0, 0x44, &mdbcur(0).eneni3[8]}, 
	{0x9163, 0, 0x44, &mdbcur(0).eneni3[12]}, 
	{0x9164, 0, 0x44, &mdbcur(0).eneni3[16]},
};
//扩展标识 07规约用 上一次日冻结数据
static const mdbcur_cpy_t MdbCopy92XX[] = {
	{0x921f, MCPYFLAG_GROUP, 5, NULL},
	{0x9210, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[0]}, 
	{0x9211, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[5]}, 
	{0x9212, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[10]}, 
	{0x9213, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[15]}, 
	{0x9214, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[20]}, 

	{0x922f, MCPYFLAG_GROUP, 5, NULL},
	{0x9220, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[0]}, 
	{0x9221, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[5]}, 
	{0x9222, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[10]}, 
	{0x9223, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[15]}, 
	{0x9224, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[20]}, 	

	{0x923f, MCPYFLAG_GROUP, 5, NULL},
	{0x9230, 0, 0x44, &mdbcur(0).enepi[0]}, 
	{0x9231, 0, 0x44, &mdbcur(0).enepi[4]}, 
	{0x9232, 0, 0x44, &mdbcur(0).enepi[8]}, 
	{0x9233, 0, 0x44, &mdbcur(0).enepi[12]}, 
	{0x9234, 0, 0x44, &mdbcur(0).enepi[16]}, 

	{0x924f, MCPYFLAG_GROUP, 5, NULL},
	{0x9240, 0, 0x44, &mdbcur(0).eneni[0]}, 
	{0x9241, 0, 0x44, &mdbcur(0).eneni[4]}, 
	{0x9242, 0, 0x44, &mdbcur(0).eneni[8]}, 
	{0x9243, 0, 0x44, &mdbcur(0).eneni[12]}, 
	{0x9244, 0, 0x44, &mdbcur(0).eneni[16]}, 

	{0x925f, MCPYFLAG_GROUP, 5, NULL},
	{0x9250, 0, 0x44, &mdbcur(0).enepi1[0]}, 
	{0x9251, 0, 0x44, &mdbcur(0).enepi1[4]}, 
	{0x9252, 0, 0x44, &mdbcur(0).enepi1[8]}, 
	{0x9253, 0, 0x44, &mdbcur(0).enepi1[12]}, 
	{0x9254, 0, 0x44, &mdbcur(0).enepi1[16]}, 

	{0x928f, MCPYFLAG_GROUP, 5, NULL},
	{0x9280, 0, 0x44, &mdbcur(0).enepi4[0]}, 
	{0x9281, 0, 0x44, &mdbcur(0).enepi4[4]}, 
	{0x9282, 0, 0x44, &mdbcur(0).enepi4[8]}, 
	{0x9283, 0, 0x44, &mdbcur(0).enepi4[12]}, 
	{0x9284, 0, 0x44, &mdbcur(0).enepi4[16]}, 

	{0x926f, MCPYFLAG_GROUP, 5, NULL},
	{0x9260, 0, 0x44, &mdbcur(0).eneni2[0]}, 
	{0x9261, 0, 0x44, &mdbcur(0).eneni2[4]}, 
	{0x9262, 0, 0x44, &mdbcur(0).eneni2[8]}, 
	{0x9263, 0, 0x44, &mdbcur(0).eneni2[12]}, 
	{0x9264, 0, 0x44, &mdbcur(0).eneni2[16]}, 

	{0x927f, MCPYFLAG_GROUP, 5, NULL},
	{0x9270, 0, 0x44, &mdbcur(0).eneni3[0]}, 
	{0x9271, 0, 0x44, &mdbcur(0).eneni3[4]}, 
	{0x9272, 0, 0x44, &mdbcur(0).eneni3[8]}, 
	{0x9273, 0, 0x44, &mdbcur(0).eneni3[12]}, 
	{0x9274, 0, 0x44, &mdbcur(0).eneni3[16]},
};
/*
//扩展标识 07规约用 上一次结算日冻结数据
static const mdbcur_cpy_t MdbCopy94XX[] = {
	{0x941f, MCPYFLAG_GROUP, 5, NULL},
	{0x9410, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[0]}, 
	{0x9411, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[5]}, 
	{0x9412, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[10]}, 
	{0x9413, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[15]}, 
	{0x9414, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa[20]}, 

	{0x942f, MCPYFLAG_GROUP, 5, NULL},
	{0x9420, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[0]}, 
	{0x9421, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[5]}, 
	{0x9422, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[10]}, 
	{0x9423, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[15]}, 
	{0x9424, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena[20]}, 	

	{0x951f, MCPYFLAG_GROUP, 5, NULL},
	{0x9510, 0, 0x44, &mdbcur(0).enepi[0]}, 
	{0x9511, 0, 0x44, &mdbcur(0).enepi[4]}, 
	{0x9512, 0, 0x44, &mdbcur(0).enepi[8]}, 
	{0x9513, 0, 0x44, &mdbcur(0).enepi[12]}, 
	{0x9514, 0, 0x44, &mdbcur(0).enepi[16]}, 

	{0x952f, MCPYFLAG_GROUP, 5, NULL},
	{0x9520, 0, 0x44, &mdbcur(0).eneni[0]}, 
	{0x9521, 0, 0x44, &mdbcur(0).eneni[4]}, 
	{0x9522, 0, 0x44, &mdbcur(0).eneni[8]}, 
	{0x9523, 0, 0x44, &mdbcur(0).eneni[12]}, 
	{0x9524, 0, 0x44, &mdbcur(0).eneni[16]}, 

	{0x953f, MCPYFLAG_GROUP, 5, NULL},
	{0x9530, 0, 0x44, &mdbcur(0).enepi1[0]}, 
	{0x9531, 0, 0x44, &mdbcur(0).enepi1[4]}, 
	{0x9532, 0, 0x44, &mdbcur(0).enepi1[8]}, 
	{0x9533, 0, 0x44, &mdbcur(0).enepi1[12]}, 
	{0x9534, 0, 0x44, &mdbcur(0).enepi1[16]}, 

	{0x954f, MCPYFLAG_GROUP, 5, NULL},
	{0x9540, 0, 0x44, &mdbcur(0).enepi4[0]}, 
	{0x9541, 0, 0x44, &mdbcur(0).enepi4[4]}, 
	{0x9542, 0, 0x44, &mdbcur(0).enepi4[8]}, 
	{0x9543, 0, 0x44, &mdbcur(0).enepi4[12]}, 
	{0x9544, 0, 0x44, &mdbcur(0).enepi4[16]}, 

	{0x955f, MCPYFLAG_GROUP, 5, NULL},
	{0x9550, 0, 0x44, &mdbcur(0).eneni2[0]}, 
	{0x9551, 0, 0x44, &mdbcur(0).eneni2[4]}, 
	{0x9552, 0, 0x44, &mdbcur(0).eneni2[8]}, 
	{0x9553, 0, 0x44, &mdbcur(0).eneni2[12]}, 
	{0x9554, 0, 0x44, &mdbcur(0).eneni2[16]}, 

	{0x956f, MCPYFLAG_GROUP, 5, NULL},
	{0x9560, 0, 0x44, &mdbcur(0).eneni3[0]}, 
	{0x9561, 0, 0x44, &mdbcur(0).eneni3[4]}, 
	{0x9562, 0, 0x44, &mdbcur(0).eneni3[8]}, 
	{0x9563, 0, 0x44, &mdbcur(0).eneni3[12]}, 
	{0x9564, 0, 0x44, &mdbcur(0).eneni3[16]},
};
*/
static const mdbcur_cpy_t MdbCopyA0XX[] = {
	{0xa01f, MCPYFLAG_GROUP, 5, NULL},
	{0xa010, 0, 0x33, &mdbcur(0).dmnpa[0]}, 
	{0xa011, 0, 0x33, &mdbcur(0).dmnpa[3]}, 
	{0xa012, 0, 0x33, &mdbcur(0).dmnpa[6]}, 
	{0xa013, 0, 0x33, &mdbcur(0).dmnpa[9]}, 
	{0xa014, 0, 0x33, &mdbcur(0).dmnpa[12]}, 

	{0xa02f, MCPYFLAG_GROUP, 5, NULL},
	{0xa020, 0, 0x33, &mdbcur(0).dmnna[0]}, 
	{0xa021, 0, 0x33, &mdbcur(0).dmnna[3]}, 
	{0xa022, 0, 0x33, &mdbcur(0).dmnna[6]}, 
	{0xa023, 0, 0x33, &mdbcur(0).dmnna[9]}, 
	{0xa024, 0, 0x33, &mdbcur(0).dmnna[12]}, 
};

static const mdbcur_cpy_t MdbCopyB0XX[] = {
	{0xb01f, MCPYFLAG_GROUP, 5, NULL},
	{0xb010, 0, 0x44, &mdbcur(0).dmntpa[0]}, 
	{0xb011, 0, 0x44, &mdbcur(0).dmntpa[4]}, 
	{0xb012, 0, 0x44, &mdbcur(0).dmntpa[8]}, 
	{0xb013, 0, 0x44, &mdbcur(0).dmntpa[12]}, 
	{0xb014, 0, 0x44, &mdbcur(0).dmntpa[16]}, 

	{0xb02f, MCPYFLAG_GROUP, 5, NULL},
	{0xb020, 0, 0x44, &mdbcur(0).dmntna[0]}, 
	{0xb021, 0, 0x44, &mdbcur(0).dmntna[4]}, 
	{0xb022, 0, 0x44, &mdbcur(0).dmntna[8]}, 
	{0xb023, 0, 0x44, &mdbcur(0).dmntna[12]}, 
	{0xb024, 0, 0x44, &mdbcur(0).dmntna[16]}, 
};

static const mdbcur_cpy_t MdbCopyA1XX[] = {
	{0xa11f, MCPYFLAG_GROUP, 5, NULL},
	{0xa110, 0, 0x33, &mdbcur(0).dmnpi[0]}, 
	{0xa111, 0, 0x33, &mdbcur(0).dmnpi[3]}, 
	{0xa112, 0, 0x33, &mdbcur(0).dmnpi[6]}, 
	{0xa113, 0, 0x33, &mdbcur(0).dmnpi[9]}, 
	{0xa114, 0, 0x33, &mdbcur(0).dmnpi[12]},

	{0xa12f, MCPYFLAG_GROUP, 5, NULL},
	{0xa120, 0, 0x33, &mdbcur(0).dmnni[0]}, 
	{0xa121, 0, 0x33, &mdbcur(0).dmnni[3]}, 
	{0xa122, 0, 0x33, &mdbcur(0).dmnni[6]}, 
	{0xa123, 0, 0x33, &mdbcur(0).dmnni[9]}, 
	{0xa124, 0, 0x33, &mdbcur(0).dmnni[12]}, 
};

//扩展标识 07规约用 上一次日冻结数据
static const mdbcur_cpy_t MdbCopyA2XX[] = {
	{0xa21f, MCPYFLAG_GROUP, 5, NULL},
	{0xa210, 0, 0x33, &mdbcur(0).dmnpa[0]}, 
	{0xa211, 0, 0x33, &mdbcur(0).dmnpa[3]}, 
	{0xa212, 0, 0x33, &mdbcur(0).dmnpa[6]}, 
	{0xa213, 0, 0x33, &mdbcur(0).dmnpa[9]}, 
	{0xa214, 0, 0x33, &mdbcur(0).dmnpa[12]}, 

	{0xa22f, MCPYFLAG_GROUP, 5, NULL},
	{0xa220, 0, 0x33, &mdbcur(0).dmnna[0]}, 
	{0xa221, 0, 0x33, &mdbcur(0).dmnna[3]}, 
	{0xa222, 0, 0x33, &mdbcur(0).dmnna[6]}, 
	{0xa223, 0, 0x33, &mdbcur(0).dmnna[9]}, 
	{0xa224, 0, 0x33, &mdbcur(0).dmnna[12]}, 
};

static const mdbcur_cpy_t MdbCopyB1XX[] = {
	{0xb11f, MCPYFLAG_GROUP, 5, NULL},
	{0xb110, 0, 0x44, &mdbcur(0).dmntpi[0]}, 
	{0xb111, 0, 0x44, &mdbcur(0).dmntpi[4]}, 
	{0xb112, 0, 0x44, &mdbcur(0).dmntpi[8]}, 
	{0xb113, 0, 0x44, &mdbcur(0).dmntpi[12]}, 
	{0xb114, 0, 0x44, &mdbcur(0).dmntpi[16]}, 

	{0xb12f, MCPYFLAG_GROUP, 5, NULL},
	{0xb120, 0, 0x44, &mdbcur(0).dmntni[0]}, 
	{0xb121, 0, 0x44, &mdbcur(0).dmntni[4]}, 
	{0xb122, 0, 0x44, &mdbcur(0).dmntni[8]}, 
	{0xb123, 0, 0x44, &mdbcur(0).dmntni[12]}, 
	{0xb124, 0, 0x44, &mdbcur(0).dmntni[16]}, 
};

static const mdbcur_cpy_t MdbCopy94XX[] = {
	{0x941f, MCPYFLAG_GROUP, 5, NULL},
	{0x9410, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_lm[0]}, 
	{0x9411, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_lm[5]}, 
	{0x9412, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_lm[10]}, 
	{0x9413, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_lm[15]}, 
	{0x9414, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_lm[20]},

	{0x942f, MCPYFLAG_GROUP, 5, NULL},
	{0x9420, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_lm[0]}, 
	{0x9421, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_lm[5]}, 
	{0x9422, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_lm[10]}, 
	{0x9423, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_lm[15]}, 
	{0x9424, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_lm[20]}, 

	{0x943f, MCPYFLAG_GROUP, 3, NULL},
	{0x9431, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_a_lm[0]}, 
	{0x9432, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_b_lm[0]}, 
	{0x9433, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_c_lm[0]}, 

	{0x944f, MCPYFLAG_GROUP, 3, NULL},
	{0x9441, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_a_lm[0]}, 
	{0x9442, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_b_lm[0]}, 
	{0x9443, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_c_lm[0]}, 

	{0x945f, MCPYFLAG_GROUP, 3, NULL},
	{0x9451, 0, 0x44, &mdbcur(0).enepi_a_lm[0]},
	{0x9452, 0, 0x44, &mdbcur(0).enepi_b_lm[0]},
	{0x9453, 0, 0x44, &mdbcur(0).enepi_c_lm[0]},

	{0x946f, MCPYFLAG_GROUP, 3, NULL},
	{0x9461, 0, 0x44, &mdbcur(0).eneni_a_lm[0]},
	{0x9462, 0, 0x44, &mdbcur(0).eneni_b_lm[0]},
	{0x9463, 0, 0x44, &mdbcur(0).eneni_c_lm[0]},
};

static const mdbcur_cpy_t MdbCopy95XX[] = {
	{0x951f, MCPYFLAG_GROUP, 5, NULL},
	{0x9510, 0, 0x44, &mdbcur(0).enepi_lm[0]}, 
	{0x9511, 0, 0x44, &mdbcur(0).enepi_lm[4]}, 
	{0x9512, 0, 0x44, &mdbcur(0).enepi_lm[8]}, 
	{0x9513, 0, 0x44, &mdbcur(0).enepi_lm[12]}, 
	{0x9514, 0, 0x44, &mdbcur(0).enepi_lm[16]},

	{0x953f, MCPYFLAG_GROUP, 5, NULL},
	{0x9530, 0, 0x44, &mdbcur(0).enepi1_lm[0]}, 
	{0x9531, 0, 0x44, &mdbcur(0).enepi1_lm[4]}, 
	{0x9532, 0, 0x44, &mdbcur(0).enepi1_lm[8]}, 
	{0x9533, 0, 0x44, &mdbcur(0).enepi1_lm[12]}, 
	{0x9534, 0, 0x44, &mdbcur(0).enepi1_lm[16]}, 

	{0x954f, MCPYFLAG_GROUP, 5, NULL},
	{0x9540, 0, 0x44, &mdbcur(0).enepi4_lm[0]}, 
	{0x9541, 0, 0x44, &mdbcur(0).enepi4_lm[4]}, 
	{0x9542, 0, 0x44, &mdbcur(0).enepi4_lm[8]}, 
	{0x9543, 0, 0x44, &mdbcur(0).enepi4_lm[12]}, 
	{0x9544, 0, 0x44, &mdbcur(0).enepi4_lm[16]},

	{0x952f, MCPYFLAG_GROUP, 5, NULL},
	{0x9520, 0, 0x44, &mdbcur(0).eneni_lm[0]}, 
	{0x9521, 0, 0x44, &mdbcur(0).eneni_lm[4]}, 
	{0x9522, 0, 0x44, &mdbcur(0).eneni_lm[8]}, 
	{0x9523, 0, 0x44, &mdbcur(0).eneni_lm[12]}, 
	{0x9524, 0, 0x44, &mdbcur(0).eneni_lm[16]},

	{0x955f, MCPYFLAG_GROUP, 5, NULL},
	{0x9550, 0, 0x44, &mdbcur(0).eneni2_lm[0]}, 
	{0x9551, 0, 0x44, &mdbcur(0).eneni2_lm[4]}, 
	{0x9552, 0, 0x44, &mdbcur(0).eneni2_lm[8]}, 
	{0x9553, 0, 0x44, &mdbcur(0).eneni2_lm[12]}, 
	{0x9554, 0, 0x44, &mdbcur(0).eneni2_lm[16]},

	{0x956f, MCPYFLAG_GROUP, 5, NULL},
	{0x9560, 0, 0x44, &mdbcur(0).eneni3_lm[0]}, 
	{0x9561, 0, 0x44, &mdbcur(0).eneni3_lm[4]}, 
	{0x9562, 0, 0x44, &mdbcur(0).eneni3_lm[8]}, 
	{0x9563, 0, 0x44, &mdbcur(0).eneni3_lm[12]}, 
	{0x9564, 0, 0x44, &mdbcur(0).eneni3_lm[16]},
};

static const mdbcur_cpy_t MdbCopyA4XX[] = {
	{0xa41f, MCPYFLAG_GROUP, 5, NULL},
	{0xa410, 0, 0x33, &mdbcur(0).dmnpa_lm[0]}, 
	{0xa411, 0, 0x33, &mdbcur(0).dmnpa_lm[3]}, 
	{0xa412, 0, 0x33, &mdbcur(0).dmnpa_lm[6]}, 
	{0xa413, 0, 0x33, &mdbcur(0).dmnpa_lm[9]}, 
	{0xa414, 0, 0x33, &mdbcur(0).dmnpa_lm[12]}, 

	{0xa42f, MCPYFLAG_GROUP, 5, NULL},
	{0xa420, 0, 0x33, &mdbcur(0).dmnna_lm[0]}, 
	{0xa421, 0, 0x33, &mdbcur(0).dmnna_lm[3]}, 
	{0xa422, 0, 0x33, &mdbcur(0).dmnna_lm[6]}, 
	{0xa423, 0, 0x33, &mdbcur(0).dmnna_lm[9]}, 
	{0xa424, 0, 0x33, &mdbcur(0).dmnna_lm[12]},
};

static const mdbcur_cpy_t MdbCopyB4XX[] = {
	{0xb41f, MCPYFLAG_GROUP, 5, NULL},
	{0xb410, 0, 0x44, &mdbcur(0).dmntpa_lm[0]}, 
	{0xb411, 0, 0x44, &mdbcur(0).dmntpa_lm[4]}, 
	{0xb412, 0, 0x44, &mdbcur(0).dmntpa_lm[8]}, 
	{0xb413, 0, 0x44, &mdbcur(0).dmntpa_lm[12]}, 
	{0xb414, 0, 0x44, &mdbcur(0).dmntpa_lm[16]}, 

	{0xb42f, MCPYFLAG_GROUP, 5, NULL},
	{0xb420, 0, 0x44, &mdbcur(0).dmntna_lm[0]}, 
	{0xb421, 0, 0x44, &mdbcur(0).dmntna_lm[4]}, 
	{0xb422, 0, 0x44, &mdbcur(0).dmntna_lm[8]}, 
	{0xb423, 0, 0x44, &mdbcur(0).dmntna_lm[12]}, 
	{0xb424, 0, 0x44, &mdbcur(0).dmntna_lm[16]}, 
};

static const mdbcur_cpy_t MdbCopyA5XX[] = {
	{0xa51f, MCPYFLAG_GROUP, 5, NULL},
	{0xa510, 0, 0x33, &mdbcur(0).dmnpi_lm[0]}, 
	{0xa511, 0, 0x33, &mdbcur(0).dmnpi_lm[3]}, 
	{0xa512, 0, 0x33, &mdbcur(0).dmnpi_lm[6]}, 
	{0xa513, 0, 0x33, &mdbcur(0).dmnpi_lm[9]}, 
	{0xa514, 0, 0x33, &mdbcur(0).dmnpi_lm[12]},

	{0xa52f, MCPYFLAG_GROUP, 5, NULL},
	{0xa520, 0, 0x33, &mdbcur(0).dmnni_lm[0]}, 
	{0xa521, 0, 0x33, &mdbcur(0).dmnni_lm[3]}, 
	{0xa522, 0, 0x33, &mdbcur(0).dmnni_lm[6]}, 
	{0xa523, 0, 0x33, &mdbcur(0).dmnni_lm[9]}, 
	{0xa524, 0, 0x33, &mdbcur(0).dmnni_lm[12]}, 
};

static const mdbcur_cpy_t MdbCopyB5XX[] = {
	{0xb51f, MCPYFLAG_GROUP, 5, NULL},
	{0xb510, 0, 0x44, &mdbcur(0).dmntpi_lm[0]}, 
	{0xb511, 0, 0x44, &mdbcur(0).dmntpi_lm[4]}, 
	{0xb512, 0, 0x44, &mdbcur(0).dmntpi_lm[8]}, 
	{0xb513, 0, 0x44, &mdbcur(0).dmntpi_lm[12]}, 
	{0xb514, 0, 0x44, &mdbcur(0).dmntpi_lm[16]}, 

	{0xb52f, MCPYFLAG_GROUP, 5, NULL},
	{0xb520, 0, 0x44, &mdbcur(0).dmntni_lm[0]}, 
	{0xb521, 0, 0x44, &mdbcur(0).dmntni_lm[4]}, 
	{0xb522, 0, 0x44, &mdbcur(0).dmntni_lm[8]}, 
	{0xb523, 0, 0x44, &mdbcur(0).dmntni_lm[12]}, 
	{0xb524, 0, 0x44, &mdbcur(0).dmntni_lm[16]}, 
};

static const mdbcur_cpy_t MdbCopyE0XX[] = {
	/*A相谐波电压含有率*/
	{0xe01f, MCPYFLAG_GROUP, 19, NULL},
	{0xe010, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[1]},
	{0xe011, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[3]},
	{0xe012, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[5]},
	{0xe013, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[7]},
	{0xe014, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[9]},
	{0xe015, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[11]},
	{0xe016, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[13]},
	{0xe017, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[15]},
	{0xe018, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[17]},
	{0xe019, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[19]},
	{0xe01a, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[21]},
	{0xe01b, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[23]},
	{0xe01c, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[25]},
	{0xe01d, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[27]},
	{0xe02e, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[29]},
	{0xe020, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[31]},
	{0xe021, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[33]},
	{0xe022, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[35]},
	{0xe023, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[37]},
	/*B相谐波电压含有率*/
	{0xe03f, MCPYFLAG_GROUP, 19, NULL},
	{0xe030, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[39]},
	{0xe031, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[41]},
	{0xe032, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[43]},
	{0xe033, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[45]},
	{0xe034, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[47]},
	{0xe035, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[49]},
	{0xe036, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[51]},
	{0xe037, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[53]},
	{0xe038, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[55]},
	{0xe039, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[57]},
	{0xe03a, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[59]},
	{0xe03b, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[61]},
	{0xe03c, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[63]},
	{0xe03d, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[65]},
	{0xe03e, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[67]},
	{0xe040, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[69]},
	{0xe041, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[71]},
	{0xe042, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[73]},
	{0xe043, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[75]},
	/*C相谐波电压含有率*/
	{0xe05f, MCPYFLAG_GROUP, 19, NULL},
	{0xe050, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[77]},
	{0xe051, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[79]},
	{0xe052, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[81]},
	{0xe053, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[83]},
	{0xe054, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[85]},
	{0xe055, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[87]},
	{0xe056, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[89]},
	{0xe057, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[91]},
	{0xe058, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[93]},
	{0xe059, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[95]},
	{0xe05a, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[97]},
	{0xe05b, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[99]},
	{0xe05c, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[101]},
	{0xe05d, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[103]},
	{0xe05e, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[105]},
	{0xe060, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[107]},
	{0xe061, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[109]},
	{0xe062, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[111]},
	{0xe063, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[113]},
};

static const mdbcur_cpy_t MdbCopyE1XX[] = {
	/*A相谐波电流含有率*/
	{0xe11f, MCPYFLAG_GROUP, 18, NULL},
	{0xe110, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[115]},
	{0xe111, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[117]},
	{0xe112, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[119]},
	{0xe113, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[121]},
	{0xe114, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[123]},
	{0xe115, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[125]},
	{0xe116, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[127]},
	{0xe117, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[129]},
	{0xe118, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[131]},
	{0xe119, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[133]},
	{0xe11a, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[135]},
	{0xe11b, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[137]},
	{0xe11c, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[139]},
	{0xe11d, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[141]},
	{0xe12e, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[143]},
	{0xe120, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[145]},
	{0xe121, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[147]},
	{0xe122, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[149]},
//	{0xe123, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[151]},
	/*B相谐波电流含有率*/
	{0xe13f, MCPYFLAG_GROUP, 18, NULL},
	{0xe130, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[151]},
	{0xe131, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[153]},
	{0xe132, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[155]},
	{0xe133, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[157]},
	{0xe134, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[159]},
	{0xe135, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[161]},
	{0xe136, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[163]},
	{0xe137, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[165]},
	{0xe138, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[167]},
	{0xe139, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[169]},
	{0xe13a, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[171]},
	{0xe13b, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[173]},
	{0xe13c, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[175]},
	{0xe13d, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[177]},
	{0xe13e, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[179]},
	{0xe140, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[181]},
	{0xe141, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[183]},
	{0xe142, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[185]},
//	{0xe143, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[185]},
	/*C相谐波电流含有率*/
	{0xe15f, MCPYFLAG_GROUP, 18, NULL},
	{0xe150, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[187]},
	{0xe151, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[189]},
	{0xe152, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[191]},
	{0xe153, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[193]},
	{0xe154, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[195]},
	{0xe155, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[197]},
	{0xe156, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[199]},
	{0xe157, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[201]},
	{0xe158, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[203]},
	{0xe159, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[205]},
	{0xe15a, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[207]},
	{0xe15b, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[209]},
	{0xe15c, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[211]},
	{0xe15d, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[213]},
	{0xe15e, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[215]},
	{0xe160, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[217]},
	{0xe161, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[219]},
	{0xe162, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[221]},
//	{0xe163, MCPYFLAG_RSHIFT|0x01, 0x22, &mdbcur(0).syntony_rating[221]},
};

static const mdbcur_cpy_t MdbCopyE2XX[] = {
	/*A相谐波电压有效值*/
	{0xe21f, MCPYFLAG_GROUP, 3, NULL},
	{0xe210, 0, 0xcc, &mdbcur(0).syntony_value[1]},
	{0xe211, 0, 0xcc, &mdbcur(0).syntony_value[13]},
	{0xe212, 0, 0xcc, &mdbcur(0).syntony_value[25]},
	/*B相谐波电压有效值*/
	{0xe22f, MCPYFLAG_GROUP, 3, NULL},
	{0xe220, 0, 0xcc, &mdbcur(0).syntony_value[37]},
	{0xe221, 0, 0xcc, &mdbcur(0).syntony_value[49]},
	{0xe222, 0, 0xcc, &mdbcur(0).syntony_value[61]},
	/*C相谐波电压有效值*/
	{0xe23f, MCPYFLAG_GROUP, 3, NULL},
	{0xe230, 0, 0xcc, &mdbcur(0).syntony_value[73]},
	{0xe231, 0, 0xcc, &mdbcur(0).syntony_value[85]},
	{0xe232, 0, 0xcc, &mdbcur(0).syntony_value[97]},
	/*A相谐波电流有效值*/
	{0xe24f, MCPYFLAG_GROUP, 3, NULL},
	{0xe240, 0, 0xcc, &mdbcur(0).syntony_value[109]},
	{0xe241, 0, 0xcc, &mdbcur(0).syntony_value[121]},
	{0xe242, 0, 0xcc, &mdbcur(0).syntony_value[133]},
	/*B相谐波电流有效值*/
	{0xe25f, MCPYFLAG_GROUP, 3, NULL},
	{0xe250, 0, 0xcc, &mdbcur(0).syntony_value[145]},
	{0xe251, 0, 0xcc, &mdbcur(0).syntony_value[157]},
	{0xe252, 0, 0xcc, &mdbcur(0).syntony_value[169]},
	/*C相谐波电流有效值*/
	{0xe26f, MCPYFLAG_GROUP, 3, NULL},
	{0xe260, 0, 0xcc, &mdbcur(0).syntony_value[181]},
	{0xe261, 0, 0xcc, &mdbcur(0).syntony_value[193]},
	{0xe262, 0, 0xcc, &mdbcur(0).syntony_value[205]},
};

static const mdbcur_cpy_t MdbCopyE3XX[] = {
	{0xe300, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enecopper[0]}, 
	{0xe301, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).eneiron[0]}, 
	{0xe302, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enecopper_lm[0]}, 
	{0xe303, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).eneiron_lm[0]},

	{0xe31f, MCPYFLAG_GROUP, 4, NULL},
	{0xe310, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_a[0]}, 
	{0xe311, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_a[0]},
	{0xe312, 0, 0x44, &mdbcur(0).enepi_a[0]},
	{0xe313, 0, 0x44, &mdbcur(0).eneni_a[0]},

	{0xe32f, MCPYFLAG_GROUP, 4, NULL},
	{0xe320, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_b[0]}, 
	{0xe321, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_b[0]},
	{0xe322, 0, 0x44, &mdbcur(0).enepi_b[0]},
	{0xe323, 0, 0x44, &mdbcur(0).eneni_b[0]},

	{0xe33f, MCPYFLAG_GROUP, 4, NULL},
	{0xe330, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_c[0]}, 
	{0xe331, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_c[0]},
	{0xe332, 0, 0x44, &mdbcur(0).enepi_c[0]},
	{0xe333, 0, 0x44, &mdbcur(0).eneni_c[0]},

	{0xe34f, MCPYFLAG_GROUP, 4, NULL},
	{0xe340, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_a_lm[0]}, 
	{0xe341, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_a_lm[0]},
	{0xe342, 0, 0x44, &mdbcur(0).enepi_a_lm[0]},
	{0xe343, 0, 0x44, &mdbcur(0).eneni_a_lm[0]},

	{0xe35f, MCPYFLAG_GROUP, 4, NULL},
	{0xe350, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_b_lm[0]}, 
	{0xe351, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_b_lm[0]},
	{0xe352, 0, 0x44, &mdbcur(0).enepi_b_lm[0]},
	{0xe353, 0, 0x44, &mdbcur(0).eneni_b_lm[0]},

	{0xe36f, MCPYFLAG_GROUP, 4, NULL},
	{0xe360, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enepa_c_lm[0]}, 
	{0xe361, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).enena_c_lm[0]},
	{0xe362, 0, 0x44, &mdbcur(0).enepi_c_lm[0]},
	{0xe363, 0, 0x44, &mdbcur(0).eneni_c_lm[0]},
};

static const mdbcur_cpy_t MdbCopyD1XX[] = {
	{0xd110, 0, 0x11, &mdbcur(0).sw_status[0]},
	{0xd111, 0, 0x55, &mdbcur(0).pwron_time[0]},
	{0xd112, 0, 0x55, &mdbcur(0).pwroff_time[0]},

	{0xd120, 0, 0x22, &mdbcur(0).progsw_cnt[0]},
	{0xd121, 0, 0x55, &mdbcur(0).progsw_time[0]},
	{0xd122, 0, 0x22, &mdbcur(0).boxsw_cnt[0]},
	{0xd123, 0, 0x55, &mdbcur(0).boxsw_time[0]},

	{0xd130, 0, 0x22, &mdbcur(0).clkchg_cnt[0]},
	{0xd131, 0, 0x55, &mdbcur(0).clkchg_time[0]},
	{0xd132, 0, 0x22, &mdbcur(0).prdchg_cnt[0]},
	{0xd133, 0, 0x55, &mdbcur(0).prdchg_time[0]},
};

static const mdbcur_cpy_t MdbCopyE4XX[] = {
	{0xe481, 0, 0x22, &mdbcur(0).buyene_cnt[0]},
	{0xe450, 0, 0x44, &mdbcur(0).ene_buyed[0]},
	{0xe454, 0, 0x44, &mdbcur(0).ene_ticklmt[0]},
	{0xe451, 0, 0x44, &mdbcur(0).ene_alarm[0]},
	{0xe452, 0, 0x44, &mdbcur(0).ene_error[0]},
};

static const mdbcur_cpy_t MdbCopy9BXX[] = {
	{0x9b1d, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).money_res[0]},
	{0x9b1e, MCPYFLAG_LSHIFT|0x02, 0x45, &mdbcur(0).money_buyed[0]},
	{0x9b10, 0, 0x44, &mdbcur(0).ene_res[0]},
	{0x9b11, 0, 0x44, &mdbcur(0).ene_overdraft[0]},
};

#define GROUP_MASK		0xff00
typedef struct {
	unsigned short itemid;
	unsigned short num;
	const mdbcur_cpy_t *list;
} mdbcur_grpcpy_t;
static const mdbcur_grpcpy_t MdbCopyGroup[] = {
	{0xb600, sizeof(MdbCopyB6XX)/sizeof(mdbcur_cpy_t), MdbCopyB6XX},
	{0xb300, sizeof(MdbCopyB3XX)/sizeof(mdbcur_cpy_t), MdbCopyB3XX},
	{0xc000, sizeof(MdbCopyC0XX)/sizeof(mdbcur_cpy_t), MdbCopyC0XX},
	{0xb200, sizeof(MdbCopyB2XX)/sizeof(mdbcur_cpy_t), MdbCopyB2XX},
	{0x9000, sizeof(MdbCopy90XX)/sizeof(mdbcur_cpy_t), MdbCopy90XX},
	{0x9100, sizeof(MdbCopy91XX)/sizeof(mdbcur_cpy_t), MdbCopy91XX},
	{0x9200, sizeof(MdbCopy92XX)/sizeof(mdbcur_cpy_t), MdbCopy92XX},
	{0x9b00, sizeof(MdbCopy9BXX)/sizeof(mdbcur_cpy_t), MdbCopy9BXX}, //购用电
	{0xa000, sizeof(MdbCopyA0XX)/sizeof(mdbcur_cpy_t), MdbCopyA0XX},
	{0xb000, sizeof(MdbCopyB0XX)/sizeof(mdbcur_cpy_t), MdbCopyB0XX},
	{0xa100, sizeof(MdbCopyA1XX)/sizeof(mdbcur_cpy_t), MdbCopyA1XX},
	{0xa200, sizeof(MdbCopyA2XX)/sizeof(mdbcur_cpy_t), MdbCopyA2XX},
	{0xb100, sizeof(MdbCopyB1XX)/sizeof(mdbcur_cpy_t), MdbCopyB1XX},
	{0x9400, sizeof(MdbCopy94XX)/sizeof(mdbcur_cpy_t), MdbCopy94XX},
	{0x9500, sizeof(MdbCopy95XX)/sizeof(mdbcur_cpy_t), MdbCopy95XX},
	{0xa400, sizeof(MdbCopyA4XX)/sizeof(mdbcur_cpy_t), MdbCopyA4XX},
	{0xb400, sizeof(MdbCopyB4XX)/sizeof(mdbcur_cpy_t), MdbCopyB4XX},
	{0xa500, sizeof(MdbCopyA5XX)/sizeof(mdbcur_cpy_t), MdbCopyA5XX},
	{0xb500, sizeof(MdbCopyB5XX)/sizeof(mdbcur_cpy_t), MdbCopyB5XX},
	{0xe000, sizeof(MdbCopyE0XX)/sizeof(mdbcur_cpy_t), MdbCopyE0XX},
	{0xe100, sizeof(MdbCopyE1XX)/sizeof(mdbcur_cpy_t), MdbCopyE1XX},
	{0xe200, sizeof(MdbCopyE2XX)/sizeof(mdbcur_cpy_t), MdbCopyE2XX},
	{0xe300, sizeof(MdbCopyE3XX)/sizeof(mdbcur_cpy_t), MdbCopyE3XX},
	{0xe400, sizeof(MdbCopyE4XX)/sizeof(mdbcur_cpy_t), MdbCopyE4XX}, //购用电
	{0xd100, sizeof(MdbCopyD1XX)/sizeof(mdbcur_cpy_t), MdbCopyD1XX},

	{0, 0, NULL},
};

/**
* @brief BCD移位操作
*/
static void BcdShiftRight(unsigned char *bcd, int len)
{
	int i;
	unsigned char uc;

	if(len <= 0) return;

	len -= 1;

	for(i=0; i<len; i++) {
		uc = *bcd;
		uc = (uc>>4)&0x0f;
		uc |= (bcd[1]<<4)&0xf0;
		*bcd++ = uc;
	}

	uc = *bcd;
	*bcd = (uc>>4)&0x0f;
}

static void BcdShiftLeft(unsigned char *bcd, int len)
{
	int i;
	unsigned char uc;
	unsigned char *p;

	if(len <= 0) return;

	len -= 1;
	bcd += len;

	for(i=len; i>0; i--) {
		p = bcd;
		uc = *bcd--;

		uc = (uc<<4)&0xf0;
		uc |= ((*bcd)>>4)&0x0f;
		*p = uc;
	}

	uc = *bcd;
	*bcd = (uc<<4)&0xf0;
}

/**
* @brief 将读取到的数据拷贝到mdbcurrent
* @param pcfg 数据项拷贝配置
* @param offset 偏移值
* @param flag 拷贝选项
*/
static void MdbCurCopyData(const mdbcur_cpy_t *pcfg, const unsigned char *psrc, unsigned int offset, unsigned char flag)
{
	unsigned char *pdst = pcfg->pbuf;
	unsigned char gblen, dl645len, i, shift, uc;

	pdst += offset;

	//if(offset) DebugPrint(0, "update %04X to %08X...\n", pcfg->itemid, pdst);
	
	if((UPCURFLAG_GB == flag) || (0 == pcfg->flag)) {
		smallcpy(pdst, psrc, pcfg->len&0x0f);
		return;
	}

	if((UPCURFLAG_2007 == flag) && (pcfg->flag&MCPYFLAG_DL2007)) {
		smallcpy(pdst, psrc, pcfg->len&0x0f);
		return;
	}

	shift = 0;
	gblen = pcfg->len&0x0f;
	dl645len = (pcfg->len>>4)&0x0f;
	if(pcfg->flag&MCPYFLAG_RSHIFT) { //div
		shift = pcfg->flag&0x0f;
		uc = shift&0x01;
		shift >>= 1;
		psrc += shift;
		dl645len -= shift;
		shift = uc;
	}
	else if(pcfg->flag&MCPYFLAG_LSHIFT) { //multi
		shift = pcfg->flag&0x0f;
		uc = shift&0x01;
		shift >>= 1;
		for(i=0; i<shift; i++) *pdst++ = 0;
		gblen -= shift;
		shift = uc;
	}

	for(i=0; i<gblen; i++) {
		if(i>= dl645len) pdst[i] = 0;
		else pdst[i] = psrc[i];
	}

	if(!shift) return;

	if(pcfg->flag&MCPYFLAG_RSHIFT) { //div
		if(2 == gblen) {
			unsigned short us = ((unsigned short)pdst[1]<<8)+(unsigned short)pdst[0];

			us >>= 4;
			pdst[1] = (us>>8) & 0xff;
			pdst[0] = us & 0xff;
		}
		else if(4 == gblen) {
			unsigned long ul = ((unsigned long)pdst[3]<<24) + ((unsigned long)pdst[2]<<16) \
							+ ((unsigned long)pdst[1]<<8) + (unsigned long)pdst[0];

			ul >>= 4;
			pdst[0] = ul & 0xff;
			pdst[1] = (ul>>8) & 0xff;
			pdst[2] = (ul>>16) & 0xff;
			pdst[3] = (ul>>24) & 0xff;
		}
		else
			BcdShiftRight(pdst, gblen);
	}
	else {
		if(2 == gblen) {
			unsigned short us = ((unsigned short)pdst[1]<<8)+(unsigned short)pdst[0];

			us <<= 4;
			pdst[1] = (us>>8) & 0xff;
			pdst[0] = us & 0xff;
		}
		else if(4 == gblen) {
			unsigned long ul = ((unsigned long)pdst[3]<<24) + ((unsigned long)pdst[2]<<16) \
							+ ((unsigned long)pdst[1]<<8) + (unsigned long)pdst[0];

			ul <<= 4;
			pdst[0] = ul & 0xff;
			pdst[1] = (ul>>8) & 0xff;
			pdst[2] = (ul>>16) & 0xff;
			pdst[3] = (ul>>24) & 0xff;
		}
		else
			BcdShiftLeft(pdst, gblen);
	}
}

/**
@breif 07规约需量特殊处理
@return 0-成功处理, 1-不是需量数据,继续处理, -1-不需要处理的数据
*/
static int SpecialCpyDmn2007(unsigned char mid, unsigned short itemid, const unsigned char *buf, int len,unsigned char id)
{
	//@07规约应该处理块数据就足够了, 不会不支持块读取
	unsigned char *pdst, *ptime;
	int i;
	
	AssertLogReturn(id>=MdbCurNum,-1,"invalid mdbcurrent id(%d)\n",id);

	//if(GetMProtoVersion() < 0x020a) return 1;  //表协议版本小于2.10不做特殊处理

	switch(itemid) {
	case 0xa01f: pdst = MdbCurrent[id].dmnpa; break;
	case 0xa02f: pdst = MdbCurrent[id].dmnna; break;
	case 0xa11f: pdst = MdbCurrent[id].dmnpi; break;
	case 0xa12f: pdst = MdbCurrent[id].dmnni; break;
	case 0xa41f: pdst = MdbCurrent[id].dmnpa_lm; break;
	case 0xa42f: pdst = MdbCurrent[id].dmnna_lm; break;
	case 0xa51f: pdst = MdbCurrent[id].dmnpi_lm; break;
	case 0xa52f: pdst = MdbCurrent[id].dmnni_lm; break;

      //
       case 0xa21f: pdst = MdbCurrent[id].dmnpa; break;
       case 0xa22f: pdst = MdbCurrent[id].dmnna; break;

	//需量时间不需要处理
	case 0xb01f:	
	case 0xb02f:
	case 0xb11f:
	case 0xb12f:
	case 0xb41f:
	case 0xb42f:
	case 0xb51f:
	case 0xb52f:
		return -1;
	default:
		return 1;
	}

	ptime = pdst + (3*MAXNUM_METPRD);
	for(i=0; i<MAXNUM_METPRD; i++) {
		pdst[0] = buf[0];
		pdst[1] = buf[1];
		pdst[2] = buf[2];
		ptime[0] = buf[3];
		ptime[1] = buf[4];
		ptime[2] = buf[5];
		ptime[3] = buf[6];

		pdst += 3;
		ptime += 4;
		buf += 8;
	}

	return 0;
}

unsigned char met_runstate_bak[MAX_CENMETP][14];

void UpdateMdbCurrent(unsigned short metpid, unsigned short itemid, const unsigned char *buf, int len, unsigned char flag,unsigned char user)
{
	unsigned char  dealflag = flag&UPCURFLAG_MASK;
	const mdbcur_grpcpy_t *pgrp = MdbCopyGroup;
	const mdbcur_cpy_t *pitem;
	int i, num, index;
	unsigned int offset;
	//unsigned short metid;
	unsigned char id;
	unsigned char bakrunstate[16] = {FLAG_MDBEMPTY};

	if((metpid==0) ||(metpid > MAX_METP)) return;
	metpid -= 1;

	if(user==MdbCudIdSvr) {
		offset = 0; 
		id = 0;
	}
	else if(user==MdbCurIdSave){
		id = 1;
		offset = 1;
		offset *= sizeof(mdbcur_t);
	}
	else{
		AssertLogReturnVoid(1,"invalid user(%d)\n",user);
	}
	
	if(flag&UPCURFLAG_ERROR) return;
	else metcommclr(metpid);

	if(UPCURFLAG_2007 == flag && SpecialCpyDmn2007(metpid, itemid, buf, len,id) <= 0) return;

	for(;0 != pgrp->itemid; pgrp++) {
		if((itemid&0xff00) != pgrp->itemid) continue;

		pitem = pgrp->list;
		for(index=0; index<pgrp->num; index++,pitem++) {
			if(itemid != pitem->itemid) continue;

			LockMdbCurrent();
			
			if(0xc020 == itemid) bakrunstate[0] = met_runstate_bak[metpid][0];
			else if(0xc02f == itemid) smallcpy(bakrunstate, met_runstate_bak[metpid], 14);
	
			if(MCPYFLAG_GROUP == pitem->flag) {
				num = pitem->len & 0xff;
				pitem++;

				for(i=0; i<num; i++,pitem++) {
					MdbCurCopyData(pitem, buf, offset, dealflag);
					if(UPCURFLAG_GB == dealflag) buf += pitem->len&0x0f;
					else if(UPCURFLAG_2007 == dealflag) {
						if(pitem->flag & MCPYFLAG_DL2007) buf += pitem->len&0x0f;
						else buf += (pitem->len >> 4)&0x0f;
					}
					else buf += (pitem->len >> 4)&0x0f;
				}
			}
			else {
				MdbCurCopyData(pitem, buf, offset, dealflag);
			}

			if(0xc020 == itemid) {
				smallcpy(met_runstate_bak[metpid],MdbCurrent[user].met_runstate,1);

				if(FLAG_MDBEMPTY == bakrunstate[0] || FLAG_MDBFAIL == bakrunstate[0]){
					MdbCurrent[user].flagchg_state[0] = bakrunstate[0] = 0;
				}
				else{
					MdbCurrent[user].flagchg_state[0] = bakrunstate[0] ^ MdbCurrent[user].met_runstate[0];
				}
			}else if(0xc02f == itemid){
				smallcpy(met_runstate_bak[metpid],MdbCurrent[user].met_runstate,14);

				if(HexIsEmpty(bakrunstate, 14, FLAG_MDBEMPTY) || HexIsEmpty(bakrunstate, 14, FLAG_MDBFAIL)) {
					memset(bakrunstate, 0, 14);
				}
				else {
					for(i=0; i<14; i++) bakrunstate[i] = bakrunstate[i] ^ MdbCurrent[user].met_runstate[i];
				}
				smallcpy(MdbCurrent[user].flagchg_state, bakrunstate, 14);
			}
			
			UnlockMdbCurrent();

			return;
		}
	}
}

void UpdateMdbCurrFailData(unsigned short metpid, unsigned short itemid, unsigned char flag)
{
	const mdbcur_grpcpy_t *pgrp = MdbCopyGroup;
	const mdbcur_cpy_t *pitem;
	int i, num, index;
	unsigned int offset;

	if((metpid==0) ||(metpid > MAX_METP)) return;
	metpid -= 1;

	offset = 0;

	for(;0 != pgrp->itemid; pgrp++) {
		if((itemid&0xff00) != pgrp->itemid) continue;

		pitem = pgrp->list;
		for(index=0; index<pgrp->num; index++,pitem++) {
			if(itemid != pitem->itemid) continue;

			LockMdbCurrent();

			if(MCPYFLAG_GROUP == pitem->flag) {
				num = pitem->len & 0xff;
				pitem++;

				for(i=0; i<num; i++,pitem++) {
					memset(pitem->pbuf + offset, FLAG_MDBFAIL,  pitem->len&0x0f);
				}
			}
			else {
				memset(pitem->pbuf + offset, FLAG_MDBFAIL,  pitem->len&0x0f);
			}

			UnlockMdbCurrent();

			return;
		}
	}
}

static const unsigned short mdbids_F25[] = {
	0xb63f, 0xb64f, 0xb65f, 0xb61f,
	0xb62f, 0xb6ef, 0xb624,
};

#if 1
static const unsigned short mdbids_F26[] = {
	0xb310, 0xb311, 0xb312, 0xb313, 
	0xb320, 0xb321, 0xb322, 0xb323, 
	0xb330, 0xb331, 0xb332, 0xb333, 
	0xb340, 0xb341, 0xb342, 0xb343, 
};
#else
static const unsigned short mdbids_F26[] = {
	0xb31f, 0xb32f, 0xb33f, 0xb34f, 
};
#endif

static const unsigned short mdbids_F27[] = {
	0xc010, 0xc011,  
	0xb210, 0xb211, 0xb212, 0xb213, 
	0xb214, 
	0xb215, 0xb216, 0xb217, 0xb218,
	0xb219, 0xb21a,
};

static const unsigned short mdbids_F28[] = {
	0xc02f, 
};

static const unsigned short mdbids_F29[] = {
	0xe300, 0xe301,
};

static const unsigned short mdbids_F30[] = {
	0xe302, 0xe303,
};

static const unsigned short mdbids_F31[] = {
	0xe310, 0xe311, 0xe312, 0xe313,
	0xe320, 0xe321, 0xe322, 0xe323,
	0xe330, 0xe331, 0xe332, 0xe333,
};

static const unsigned short mdbids_F32[] = {
	0xe340, 0xe341, 0xe342, 0xe343,
	0xe350, 0xe351, 0xe352, 0xe353,
	0xe360, 0xe361, 0xe362, 0xe363,
};

static const unsigned short mdbids_F33[] = {
	0x901f, 0x911f, 0x913f, 0x914f,
};

static const unsigned short mdbids_F34[] = {
	0x902f, 0x912f, 0x915f, 0x916f,
};

static const unsigned short mdbids_F35[] = {
	0xa01f, 0xb01f, 0xa11f, 0xb11f,
};

static const unsigned short mdbids_F36[] = {
	0xa02f, 0xb02f, 0xa12f, 0xb12f,
};

static const unsigned short mdbids_F37[] = {
	0x941f, 0x951f, 0x953f, 0x954f,
};

static const unsigned short mdbids_F38[] = {
	0x942f, 0x952f, 0x955f, 0x956f,
};

static const unsigned short mdbids_F39[] = {
	0xa41f, 0xb41f, 0xa51f, 0xb51f,
};

static const unsigned short mdbids_F40[] = {
	0xa42f, 0xb42f, 0xa52f, 0xb52f,
};

static const unsigned short mdbids_F49[] = {
	0xb660, 0xb661, 0xb662, 
	0xb663, 0xb664, 0xb665,
};

static const unsigned short mdbids_F129[] = {
	0x901f,
};

static const unsigned short mdbids_F130[] = {
	0x911f,
};

static const unsigned short mdbids_F131[] = {
	0x902f,
};

static const unsigned short mdbids_F132[] = {
	0x912f,
};

static const unsigned short mdbids_F133[] = {
	0x913f,
};

static const unsigned short mdbids_F134[] = {
	0x915f,
};

static const unsigned short mdbids_F135[] = {
	0x916f,
};

static const unsigned short mdbids_F136[] = {
	0x914f,
};

static const unsigned short mdbids_F137[] = {
	0x941f,
};

static const unsigned short mdbids_F138[] = {
	0x951f,
};

static const unsigned short mdbids_F139[] = {
	0x942f,
};

static const unsigned short mdbids_F140[] = {
	0x952f,
};

static const unsigned short mdbids_F141[] = {
	0x953f,
};

static const unsigned short mdbids_F142[] = {
	0x955f,
};

static const unsigned short mdbids_F143[] = {
	0x956f,
};

static const unsigned short mdbids_F144[] = {
	0x954f,
};

/*static const unsigned short mdbids_F145[] = {
	0xa01f, 0xb01f,
};

static const unsigned short mdbids_F146[] = {
	0xa11f, 0xb11f,
};

static const unsigned short mdbids_F147[] = {
	0xa02f, 0xb02f,
};

static const unsigned short mdbids_F148[] = {
	0xa12f, 0xb12f,
};

static const unsigned short mdbids_F149[] = {
	0xa41f, 0xb41f,
};

static const unsigned short mdbids_F150[] = {
	0xa51f, 0xb51f,
};

static const unsigned short mdbids_F151[] = {
	0xa42f, 0xb42f,
};

static const unsigned short mdbids_F152[] = {
	0xa52f, 0xb52f,
};*/

static const unsigned short mdbids_F161[] = {
	0xd110, 0xd111, 0xd112,
};

static const unsigned short mdbids_F165[] = {
	0xd120, 0xd121, 0xd122, 0xd123,
};

static const unsigned short mdbids_F166[] = {
	0xd130, 0xd131, 0xd132, 0xd133, 
};

static const unsigned short mdbids_F167[] = {
	0xe481, 0x9b1d, 0x9b1e,0x9b10, 
	0x9b11, 0xe450, 0xe454,0xe451,
	0xe452
};
typedef struct {
	unsigned short itemid;   //数据单元标识
	unsigned short len;    //数据长度
	unsigned char *pbase;  //数据所在指针
	const unsigned short *rditems;    //抄表数据标识
	unsigned short rdnum;   //抄表数据项数目
	unsigned char  bfenum;   //是否带费率数
	unsigned char  brdtime;   //是否带终端抄表时间
} mdbcur_offset_t;


static const mdbcur_offset_t MdbOffsetF25[] = {
	{0x0301, 62, mdbcur(0).pwra, mdbids_F25, sizeof(mdbids_F25)/2, 0, 1}, //F25
	{0x0302, 52, mdbcur(0).volbr_cnt, mdbids_F26, sizeof(mdbids_F26)/2, 0, 1}, //F26
	{0x0304, 55, mdbcur(0).met_clock, mdbids_F27, sizeof(mdbids_F27)/2, 0, 1}, //F27
	{0x0308, 28, mdbcur(0).flagchg_state, mdbids_F28, sizeof(mdbids_F28)/2, 0, 1}, //F28
	{0x0310, 10, mdbcur(0).enecopper, mdbids_F29, sizeof(mdbids_F29)/2, 0, 1}, //F29
	{0x0320, 10, mdbcur(0).enecopper_lm, mdbids_F30, sizeof(mdbids_F30)/2, 0, 1}, //F30
	{0x0340, 54, mdbcur(0).enepa_a, mdbids_F31, sizeof(mdbids_F31)/2, 0, 1}, //F31
	{0x0380, 54, mdbcur(0).enepa_a_lm, mdbids_F32, sizeof(mdbids_F32)/2, 0, 1}, //F32
};

static const mdbcur_offset_t MdbOffsetF33[] = {
	{0x0401, 85, mdbcur(0).enepa, mdbids_F33, sizeof(mdbids_F33)/2, 1, 1}, //F33
	{0x0402, 85, mdbcur(0).enena, mdbids_F34, sizeof(mdbids_F34)/2, 1, 1}, //F34
	{0x0404, 70, mdbcur(0).dmnpa, mdbids_F35, sizeof(mdbids_F35)/2, 1, 1}, //F35
	{0x0408, 70, mdbcur(0).dmnna, mdbids_F36, sizeof(mdbids_F36)/2, 1, 1}, //F36
	{0x0410, 85, mdbcur(0).enepa_lm, mdbids_F37, sizeof(mdbids_F37)/2, 1, 1}, //F37
	{0x0420, 85, mdbcur(0).enena_lm, mdbids_F38, sizeof(mdbids_F38)/2, 1, 1}, //F38
	{0x0440, 70, mdbcur(0).dmnpa_lm, mdbids_F39, sizeof(mdbids_F39)/2, 1, 1}, //F39
	{0x0480, 70, mdbcur(0).dmnna_lm, mdbids_F40, sizeof(mdbids_F40)/2, 1, 1}, //F40
};

static const mdbcur_offset_t MdbOffsetF49[] = {
	{0x0601, 12, mdbcur(0).phase_arc, mdbids_F49, sizeof(mdbids_F49)/2, 0, 0}, //F49
};

static const mdbcur_offset_t MdbOffsetF129[] = {
	{0x1001, 25, mdbcur(0).enepa, mdbids_F129, sizeof(mdbids_F129)/2, 1, 1}, //F129
	{0x1002, 20, mdbcur(0).enepi, mdbids_F130, sizeof(mdbids_F130)/2, 1, 1}, //F130
	{0x1004, 25, mdbcur(0).enena, mdbids_F131, sizeof(mdbids_F131)/2, 1, 1}, //F131
	{0x1008, 20, mdbcur(0).eneni, mdbids_F132, sizeof(mdbids_F132)/2, 1, 1}, //F132
	{0x1010, 20, mdbcur(0).enepi1, mdbids_F133, sizeof(mdbids_F133)/2, 1, 1}, //F133
	{0x1020, 20, mdbcur(0).eneni2, mdbids_F134, sizeof(mdbids_F134)/2, 1, 1}, //F134
	{0x1040, 20, mdbcur(0).eneni3, mdbids_F135, sizeof(mdbids_F135)/2, 1, 1}, //F135
	{0x1080, 20, mdbcur(0).enepi4, mdbids_F136, sizeof(mdbids_F136)/2, 1, 1}, //F136
};

static const mdbcur_offset_t MdbOffsetF137[] = {
	{0x1101, 25, mdbcur(0).enepa_lm, mdbids_F137, sizeof(mdbids_F137)/2, 1, 1}, //F137
	{0x1102, 20, mdbcur(0).enepi_lm, mdbids_F138, sizeof(mdbids_F138)/2, 1, 1}, //F138
	{0x1104, 25, mdbcur(0).enena_lm, mdbids_F139, sizeof(mdbids_F139)/2, 1, 1}, //F139
	{0x1108, 20, mdbcur(0).eneni_lm, mdbids_F140, sizeof(mdbids_F140)/2, 1, 1}, //F140
	{0x1110, 20, mdbcur(0).enepi1_lm, mdbids_F141, sizeof(mdbids_F141)/2, 1, 1}, //F141
	{0x1120, 20, mdbcur(0).eneni2_lm, mdbids_F142, sizeof(mdbids_F142)/2, 1, 1}, //F142
	{0x1140, 20, mdbcur(0).eneni3_lm, mdbids_F143, sizeof(mdbids_F143)/2, 1, 1}, //F143
	{0x1180, 20, mdbcur(0).enepi4_lm, mdbids_F144, sizeof(mdbids_F144)/2, 1, 1}, //F144
};

static const mdbcur_offset_t MdbOffsetF161[] = {
	{0x1401, 11, mdbcur(0).sw_status, mdbids_F161, sizeof(mdbids_F161)/2, 0, 1}, //F161
	{0x1410, 14, mdbcur(0).progsw_cnt, mdbids_F165, sizeof(mdbids_F165)/2, 0, 1}, //F165
	{0x1420, 14, mdbcur(0).clkchg_cnt, mdbids_F166, sizeof(mdbids_F166)/2, 0, 1}, //F166
	{0x1440, 36, mdbcur(0).buyene_cnt, mdbids_F167, sizeof(mdbids_F167)/2, 0, 1}, //F167
	{0x1480, 50, mdbcur(0).ene_febuyed, NULL, 0, 1, 1}, //F168
};

/**
* @brief 更新抄表时间
* @param mid 测量点号, 0~
*/
void UpdateMdbCurRdTime(unsigned short mid)
{
	sysclock_t clock;

	AssertLogReturnVoid(mid>=MdbCurNum,"invalid mdbcurrent mid(%d)\n",mid);
	SysClockReadCurrent(&clock);

	LockMdbCurrent();
	mdbcur(mid).rdtime[0] = clock.minute;
	mdbcur(mid).rdtime[1] = clock.hour;
	mdbcur(mid).rdtime[2] = clock.day;
	mdbcur(mid).rdtime[3] = clock.month;
	mdbcur(mid).rdtime[4] = clock.year;
	HexToBcd(mdbcur(mid).rdtime, 5);
	UnlockMdbCurrent();
}

static int ReadMdbF57(unsigned short mid, unsigned short itemid, unsigned char *buf, int len)
{
	int actlen;
	unsigned char *pdata;
	unsigned short items[6];

	switch(itemid) {
	case 0x0701://F57
		if(len < 217) return -1;

		MdbCurrent[MdbCudIdSvr].syntony_value[0] = 19;
		actlen = (int)MdbCurrent[MdbCudIdSvr].syntony_value[0]&0xff;
		if(actlen > 19) actlen = 19;

		if(0 == actlen) actlen = 1;
		else actlen = (actlen-1)*6*2 + 1;

		pdata = MdbCurrent[MdbCudIdSvr].syntony_value;
		memcpy(buf, pdata, actlen);
		break;

	case 0x0702://F58
		if(len < 223) return -1;

		items[0] = 0xe01f;
		items[1] = 0xe03f;
		items[2] = 0xe05f;
		items[3] = 0xe11f;
		items[4] = 0xe13f;
		items[5] = 0xe15f;
		ReadImmMdbCur(mid, items, 6,MdbCudIdSvr);

		MdbCurrent[MdbCudIdSvr].syntony_rating[0] = 19;
		actlen = (int)MdbCurrent[MdbCudIdSvr].syntony_rating[0]&0xff;
		if(actlen > 19) actlen = 19;

		if(0 == actlen) actlen = 1;
		else actlen = actlen*3*2 + (actlen-1)*3*2 + 1 ;
		
		pdata = MdbCurrent[MdbCudIdSvr].syntony_rating;
		memcpy(buf, pdata, actlen);
	/*	buf += 115;
		pdata += 115;
		for(i=0; i<3; i++) {
			memcpy(buf, pdata, 36);
			buf += 36;
			pdata += 38;
		}*/
		break;

	default:
		return -2;
	}

	return actlen;
}

static int ReadMdbF145(unsigned short mid, unsigned short itemid, unsigned char *buf, int len)
{
	#define ITEM_LEN	(7*MAXNUM_METPRD+6)

	unsigned char *pdmn, *ptime;
	unsigned short items[2];
	int i;

	if(len < ITEM_LEN) return -1;

	switch(itemid) {
	case 0x1201://F145
		pdmn = MdbCurrent[MdbCudIdSvr].dmnpa;
		ptime = MdbCurrent[MdbCudIdSvr].dmntpa;
		items[0] = 0xa01f;
		items[1] = 0xb01f;
		break;
	case 0x1202://F146
		pdmn = MdbCurrent[MdbCudIdSvr].dmnpi;
		ptime = MdbCurrent[MdbCudIdSvr].dmntpi;
		items[0] = 0xa11f;
		items[1] = 0xb11f;
		break;
	case 0x1204://F147
		pdmn = MdbCurrent[MdbCudIdSvr].dmnna;
		ptime = MdbCurrent[MdbCudIdSvr].dmntna;
		items[0] = 0xa02f;
		items[1] = 0xb02f;
		break;
	case 0x1208://F148
		pdmn = MdbCurrent[MdbCudIdSvr].dmnni;
		ptime = MdbCurrent[MdbCudIdSvr].dmntni;
		items[0] = 0xa12f;
		items[1] = 0xb12f;
		break;
	case 0x1210://F149
		pdmn = MdbCurrent[MdbCudIdSvr].dmnpa_lm;
		ptime = MdbCurrent[MdbCudIdSvr].dmntpa_lm;
		items[0] = 0xa41f;
		items[1] = 0xb41f;
		break;
	case 0x1220://F150
		pdmn = MdbCurrent[MdbCudIdSvr].dmnpi_lm;
		ptime = MdbCurrent[MdbCudIdSvr].dmntpi_lm;
		items[0] = 0xa51f;
		items[1] = 0xb51f;
		break;
	case 0x1240://F151
		pdmn = MdbCurrent[MdbCudIdSvr].dmnna_lm;
		ptime = MdbCurrent[MdbCudIdSvr].dmntna_lm;
		items[0] = 0xa42f;
		items[1] = 0xb42f;
		break;
	case 0x1280://F152
		pdmn = MdbCurrent[MdbCudIdSvr].dmnni_lm;
		ptime = MdbCurrent[MdbCudIdSvr].dmntni_lm;
		items[0] = 0xa52f;
		items[1] = 0xb52f;
		break;
	default: return -2;
	}

	ReadImmMdbCur(mid, items, 2,MdbCudIdSvr);
	UpdateMdbCurRdTime(MdbCudIdSvr);

	smallcpy(buf, MdbCurrent[MdbCudIdSvr].rdtime, 5);
	buf += 5;
	*buf++ = MAXNUM_FEEPRD;

	//DebugPrint(0, "copy dmn from %08X,%08X...\n", pdmn, ptime);
	
	for(i=0; i<MAXNUM_METPRD; i++) {
		smallcpy(buf, pdmn, 3); buf += 3; pdmn += 3;
		smallcpy(buf, ptime, 4); buf += 4; ptime += 4;
	}

	return ITEM_LEN;
}

typedef int (*readmdbcur_fn)(unsigned short, unsigned short, unsigned char *, int);
typedef struct {
	unsigned short itemid;
	unsigned short bfunc;
	const mdbcur_offset_t *list;
	unsigned short num;
} mdbcur_grpread_t;
static const mdbcur_grpread_t MdbGroupRead[] = {
	{0x0301, 0, MdbOffsetF25,  sizeof(MdbOffsetF25)/sizeof(mdbcur_offset_t)},
	{0x0401, 0, MdbOffsetF33,  sizeof(MdbOffsetF33)/sizeof(mdbcur_offset_t)},
	{0x0601, 0, MdbOffsetF49,  sizeof(MdbOffsetF49)/sizeof(mdbcur_offset_t)},
	{0x0701, 1, (const mdbcur_offset_t *)ReadMdbF57, 0},
	{0x1001, 0, MdbOffsetF129, sizeof(MdbOffsetF129)/sizeof(mdbcur_offset_t)}, 
	{0x1101, 0, MdbOffsetF137, sizeof(MdbOffsetF137)/sizeof(mdbcur_offset_t)}, 
	{0x1201, 1, (const mdbcur_offset_t *)ReadMdbF145, 0},
	{0x1401, 0, MdbOffsetF161, sizeof(MdbOffsetF161)/sizeof(mdbcur_offset_t)}, 
	{0, 0, NULL, 0},
};


/**
* @brief 读取当前表计数据
* @param metpid 测量点号, 1~MAX_CENMETP
* @param itemid 数据项编号
* @param buf 缓存区指针
* @param len 缓存区长度
* @return 成功返回实际读取长度, 失败返回负数, 无此数据项返回-2, 缓存区溢出返回-1
*/
int ReadMdbCurrent(unsigned short metpid, unsigned short itemid, unsigned char *buf, int len)
{
	mdbcur_t *pdb;
	unsigned char *pdata;
	unsigned short metersn;
	int i, actlen;
	const mdbcur_grpread_t *pgrp = MdbGroupRead;
	const mdbcur_offset_t *poffset;

	actlen = 0;

	if((metpid==0) ||(metpid > MAX_METP)) return -2;
	metpid -= 1;
	metersn = MetpMap[metpid].metid+1;
		
	if((metersn==0) ||(metersn > MAX_METER)) return -3;
	if(EMPTY_METER(metersn-1)) return -4;
	
	itemid &= 0x7fff;

	for(;0 != pgrp->itemid; pgrp++) {
		if((pgrp->itemid&0xff00) == (itemid&0xff00)) break;
	}
	if(0 == pgrp->itemid) return -5;

	if(pgrp->bfunc) return ((*((readmdbcur_fn)pgrp->list))(metersn, itemid, buf, len));

	poffset = pgrp->list;
	pdb = &MdbCurrent[MdbCudIdSvr];

	for(i=0; i<pgrp->num; i++,poffset++) {
		if(itemid == poffset->itemid) break;
	}
	if(i >= pgrp->num) return -6;

	actlen = poffset->len;
	if(poffset->brdtime) actlen += 5;
	if(poffset->bfenum) actlen += 1;
	if(actlen > len) return -1;

	if(NULL != poffset->rditems)
		ReadImmMdbCur(metersn, poffset->rditems, poffset->rdnum,MdbCudIdSvr);

	LockMdbCurrent();

	if(poffset->brdtime) {
		smallcpy(buf, pdb->rdtime, 5);
		buf += 5;
	}

	if(poffset->bfenum) *buf++ = MAXNUM_FEEPRD;

	pdata = poffset->pbase;
	//pdata += (unsigned int)metpid*sizeof(mdbcur_t);
	smallcpy(buf, pdata, poffset->len);
	
	UnlockMdbCurrent();

	return actlen;
}


