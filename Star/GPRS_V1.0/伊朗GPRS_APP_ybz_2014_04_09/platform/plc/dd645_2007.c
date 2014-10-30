/**
* dl645_2007.c -- DL/T645-2007协议分析
* 
* 作者: zhuzhiqiang
* 创建时间: 2009-5-21
* 最后修改时间: 2009-5-21
*/

#include <stdio.h>
#include <string.h>
#include "app_include/lib/align.h"
#include "plat_include/plcomm.h"
#include "plat_include/dl645_2007.h"
#include "plat_include/debug.h"

#if 0
struct id_convert {
	unsigned short idshort;
	unsigned long idlong;
};
static const struct id_convert IdConvertion[] = {
	{0x9010, 0x00010000},	//
	{0x9011, 0x00010100},
	{0x9012, 0x00010200},
	{0x9013, 0x00010300},
	{0x9014, 0x00010400},
	{0x901f, 0x0001ff00},

	{0x9410, 0x00010001},
	{0x9411, 0x00010101},
	{0x9412, 0x00010201},
	{0x9413, 0x00010301},
	{0x9414, 0x00010401}, 
	{0x941f, 0x0001ff01},

	{0xb630, 0x02030000},
	{0xb631, 0x02030100},
	{0xb640, 0x02040000},	//xx.xx xx.xxxx
	{0xb641, 0x02040100}, 
	{0xb611, 0x02010100}, 
	{0xb612, 0x02010200},
	{0xb613, 0x02010300},
	{0xb61f, 0x0201ff00},
	//电流数据块	
	{0xb621, 0x02020100},
	{0xb622, 0x02020200},
	{0xb623, 0x02020300}, 
	{0xb62f, 0x0202ff00},	//xx.xx xxx.xxx
	{0xc011, 0x04000102ul},	//终端时间

	{0x921f, 0x05060101},  //日冻结电能量
	{0x922f, 0x05060201},  //日冻结反向电能量

	{0x911f, 0x0003ff00},
	{0x902f, 0x0002ff00},
	{0x912f, 0x0004ff00},
	{0x913f, 0x0005ff00},
	{0x914f, 0x0008ff00},
	{0x915f, 0x0006ff00},
	{0x916f, 0x0007ff00},
	{0x942f, 0x0002ff01},
	{0x951f, 0x0003ff01},
	{0x952f, 0x0004ff01},
	{0x953f, 0x0005ff01},
	{0x954f, 0x0008ff01},
	{0x955f, 0x0006ff01},
	{0x956f, 0x0007ff01},

	{0xc02f, 0x040005ff},

	{0xd113, 0x04000503},  //电表运行状态字3
	{0xd120, 0x1e000101},  //上一次合闸时刻, yymmddhhmnss
	{0xd130, 0x1d000101},  //上一次跳闸时刻, yymmddhhmnss

	{0xb212, 0x03300000},  //编程总次数
	{0xb210, 0x03300001},  //最近一次编程时刻

	{0xb219, 0x03300400}, //校时总次数
	{0xb21a, 0x03300401}, //最近一次事校时时刻

	{0xb21b, 0x03300500}, //时段表编程记录
	{0xb21c, 0x03300501}, //发生时刻

	{0xc010, 0x04000101}, //日期及星期4
	{0xc011, 0x04000102}, //时间3
	{0xb213, 0x03300200}, //需量清零总次数
	{0xb211, 0x03300201}, //最近一次需量清零时刻
	{0xb212, 0x03300000}, //编程总次数
	{0xb210, 0x03300001}, //最近一次编程时刻
	{0xb214, 0x0280000a}, //电池工作时间
	{0xb215, 0x03300100}, //电表清零次数
	{0xb216, 0x03300101}, //最近一次电表清零时刻
	{0xb217, 0x03300300}, //事件清零总次数
	{0xb218, 0x03300301}, //最近一次事件清零时刻
	{0xb219, 0x03300400}, //校时总次数
	{0xb21a, 0x03300401}, //最近一次事校时时刻

	//断相次数和累计时间
	{0xb311, 0x13010001},
	{0xb312, 0x13020001},
	{0xb313, 0x13030001},
	{0xb321, 0x13010002},
	{0xb322, 0x13020002},
	{0xb323, 0x13030002},
	{0xb331, 0x13010101},
	{0xb332, 0x13020101},
	{0xb333, 0x13030101},
	{0xb341, 0x13012501},
	{0xb342, 0x13022501},
	{0xb342, 0x13022501},

	{0xd140, 0x03320201},  //上1次购电后总购电次数
	{0xd141, 0x00900200},  //（当前）剩余金额
	{0xd142, 0x03330601},  //上1次购电后累计购电金额
	{0xd143, 0x00900100},  //（当前）剩余电量
	{0xd144, 0x00900101},  //（当前）透支电量 
	{0xd145, 0x03320601},  //上1次购电后累计购电量
	{0xd146, 0x04000f04},  //透支电量限值
	{0xd147, 0x04000f01},  //报警电量1限值
	{0xd148, 0x04000f02},  //报警电量2限值
};
#define SIZE_IDCONVERTION		(sizeof(IdConvertion)/sizeof(IdConvertion[0]))
#endif

#define CPYFLAG_MASK		0xf000
#define CPYFLAG_OFFSET		0x0000
#define CPYFLAG_GROUP		0x1000

#define CPYDATA_MASK		0x0fff
typedef struct {
	unsigned short rid;  // 1997ID
	unsigned char rlen;  //目标拷贝长度
	unsigned long id;  // 2007ID
	unsigned char len;  // 2007标准数据项长度
	unsigned short cpycfg;  //数据拷贝配置
} dl645_2007_t;
static const dl645_2007_t dl645_2007_tab[] = {//added by ZY
	
	//组合有总功电能数据块  当前
	{0x9000, 4, 0x00000000, 4, 0},
	{0x9001, 4, 0x00000100, 4, 0},
	{0x9002, 4, 0x00000200, 4, 0},
	{0x9003, 4, 0x00000300, 4, 0},
	{0x9004, 4, 0x00000400, 4, 0}, 
	{0x900f, 20, 0x0000ff00, 20, 0}, 
	//正向有功电能数据块
	{0x9010, 4, 0x00010000, 4, 0},
	{0x9011, 4, 0x00010100, 4, 0},
	{0x9012, 4, 0x00010200, 4, 0},
	{0x9013, 4, 0x00010300, 4, 0},
	{0x9014, 4, 0x00010400, 4, 0}, 
	{0x901f, 20, 0x0001ff00, 20, 0}, 
	//反向有功电能数据块
	{0x9020, 4, 0x00020000, 4, 0}, 
	{0x9021, 4, 0x00020100, 4, 0},
	{0x9022, 4, 0x00020200, 4, 0},
	{0x9023, 4, 0x00020300, 4, 0},
	{0x9024, 4, 0x00020400, 4, 0}, 
	{0x902f, 20, 0x0002ff00, 20, 0},
	//当前正向无功总电能(组合无功1电能数据块)
	{0x9110, 4, 0x00030000, 4, 0}, 
	{0x9111, 4, 0x00030100, 4, 0},
	{0x9112, 4, 0x00030200, 4, 0},
	{0x9113, 4, 0x00030300, 4, 0},
	{0x9114, 4, 0x00030400, 4, 0}, 
	{0x911f, 20, 0x0003ff00, 20, 0},
	//当前反向无功总电能(组合无功2电能数据块)
	{0x9120, 4, 0x00040000, 4, 0}, 
	{0x9121, 4, 0x00040100, 4, 0},
	{0x9122, 4, 0x00040200, 4, 0},
	{0x9123, 4, 0x00040300, 4, 0},
	{0x9124, 4, 0x00040400, 4, 0}, 
	{0x912f, 20, 0x0004ff00, 20, 0},
	//一象限无功电能数据块
	{0x9130, 4, 0x00050000, 4, 0}, 
	{0x9131, 4, 0x00050100, 4, 0}, 
	{0x9132, 4, 0x00050200, 4, 0},
	{0x9133, 4, 0x00050300, 4, 0},
	{0x9134, 4, 0x00050400, 4, 0},
	{0x913f, 20, 0x0005ff00, 20, 0},
	//四象限无功电能数据块
	{0x9140, 4, 0x00080000, 4, 0},
	{0x9141, 4, 0x00080100, 4, 0},
	{0x9142, 4, 0x00080200, 4, 0},
	{0x9143, 4, 0x00080300, 4, 0}, 
	{0x9144, 4, 0x00080400, 4, 0}, 
	{0x914f, 20, 0x0008ff00, 20, 0},
	//二象限无功电能数据块
	{0x9150, 4, 0x00060000, 4, 0}, 
	{0x9151, 4, 0x00060100, 4, 0},
	{0x9152, 4, 0x00060200, 4, 0},
	{0x9153, 4, 0x00060300, 4, 0},
	{0x9154, 4, 0x00060400, 4, 0},
	{0x915f, 20, 0x0006ff00, 20, 0},
	//三象限无功电能数据块
	{0x9160, 4, 0x00070000, 4, 0},
	{0x9161, 4, 0x00070100, 4, 0},
	{0x9162, 4, 0x00070200, 4, 0}, 
	{0x9163, 4, 0x00070300, 4, 0},
	{0x9164, 4, 0x00070400, 4, 0},
	{0x916f, 20, 0x0007ff00, 20, 0},

	{0x9201, 5, 0x05060001, 5, 0},  //日冻结时间
    //日冻结(07为上一次日冻结)
	{0x921f, 20, 0x05060101, 20, 0},  //正向有功电能示值（总、费率1～M）
	{0x9210,  4, 0x05060101, 20, 0},
	{0x922f, 20, 0x05060201, 20, 0},  //反向有功电能示值（总、费率1～M）
	{0x9220,  4, 0x05060201, 20, 0},
	{0x923f, 20, 0x05060301, 20, 0},  //正向无功（组合无功1）电能示值（总、费率1～M）
	{0x9230,  4, 0x05060301, 20, 0},
	{0x924f, 20, 0x05060401, 20, 0},  //反向无功（组合无功2）电能示值（总、费率1～M）
	{0x9240,  4, 0x05060401, 20, 0},

	{0x925f, 20, 0x05060501, 20, 0},  //一象限无功电能示值（总、费率1～M）
	{0x9250,  4, 0x05060501, 20, 0},
	{0x926f, 20, 0x05060601, 20, 0},  //二象限无功电能示值（总、费率1～M）
	{0x9260,  4, 0x05060601, 20, 0},
	{0x927f, 20, 0x05060701, 20, 0},  //三象限无功电能示值（总、费率1～M）
	{0x9270,  4, 0x05060701, 20, 0},
	{0x928f, 20, 0x05060801, 20, 0},  //四象限无功电能示值（总、费率1～M）
	{0x9280,  4, 0x05060801, 20, 0},

	{0xa21f, 40, 0x05060901, 40,0},	//正向有功最大需量及发生时间（总、费率1～M）
	{0xb21f, 40, 0x05060901, 40,0},
	{0xa22f, 40, 0x05060a01, 40,0},	//反向有功最大需量及发生时间（总、费率1～M）
	{0xb22f, 40, 0x05060a01, 40,0},
	
	//抄表日(07为上一次结算日数据)
	{0x931f, 20, 0x0001ff01, 20, 0},  //正向有功电能示值（总、费率1～M）
	{0x932f, 20, 0x0002ff01, 20, 0},  //反向无功电能示值（总、费率1～M）
	{0x933f, 20, 0x0003ff01, 20, 0},  //正向无功（组合无功1）电能示值（总、费率1～M）
	{0x934f, 20, 0x0004ff01, 20, 0},  //反向有功（组合无功2）电能示值（总、费率1～M）

	{0x935f, 20, 0x0005ff01, 20, 0},  //一象限无功电能示值（总、费率1～M）
	{0x936f, 20, 0x0006ff01, 20, 0},  //二象限无功电能示值（总、费率1～M）
	{0x937f, 20, 0x0007ff01, 20, 0},  //三象限无功电能示值（总、费率1～M）
	{0x938f, 20, 0x0008ff01, 20, 0},  //四象限无功电能示值（总、费率1～M）
	
	{0xa31f, 40, 0x0101ff01, 40, 0},	//正向有功最大需量及发生时间（总、费率1～M）
	{0xb31f, 40, 0x0101ff01, 40, 0},
	{0xa32f, 40, 0x0102ff01, 40, 0},	//反向有功最大需量及发生时间（总、费率1～M）
	{0xb32f, 40, 0x0102ff01, 40, 0},

	//上月
	///组合有总功电能数据块
	{0x9400, 4, 0x00000001, 4, 0},
	{0x9401, 4, 0x00000101, 4, 0},
	{0x9402, 4, 0x00000201, 4, 0},
	{0x9403, 4, 0x00000301, 4, 0},
	{0x9404, 4, 0x00000401, 4, 0}, 
	{0x940f, 20, 0x0000ff01, 20, 0}, 
#if VERSION_AREA == VERSION_JIANGXI
	//上月数据 (07为月末那一天日冻结的数据)
	//正向有功电能数据块
	{0x941f, 20, 0x05060101, 20, 0}, 
	{0x9410,  4, 0x05060101, 20, 0}, 
	//反向有功电能数据块
	{0x942f, 20, 0x05060201, 20, 0},
	{0x9420,  4, 0x05060201, 20, 0},
	//当前正向无功总电能(组合无功1电能数据块)
	{0x951f, 20, 0x05060301, 20, 0},
	{0x9510,  4, 0x05060301, 20, 0},
	//当前反向无功总电能(组合无功2电能数据块)
	{0x952f, 20, 0x05060401, 20, 0},
	{0x9520,  4, 0x05060401, 20, 0},
	//一象限无功电能数据块
	{0x953f, 20, 0x05060501, 20, 0},
	{0x9530,  4, 0x05060501, 20, 0},
	//四象限无功电能数据块
	{0x954f, 20, 0x05060801, 20, 0},
	{0x9540,  4, 0x05060801, 20, 0},
	//二象限无功电能数据块
	{0x955f, 20, 0x05060601, 20, 0},
	{0x9550,  4, 0x05060601, 20, 0},
	//三象限无功电能数据块
	{0x956f, 20, 0x05060701, 20, 0},
	{0x9560,  4, 0x05060701, 20, 0},
#else
	//正向有功电能数据块
	{0x9410, 4, 0x00010001, 4, 0},
	{0x9411, 4, 0x00010101, 4, 0},
	{0x9412, 4, 0x00010201, 4, 0},
	{0x9413, 4, 0x00010301, 4, 0},
	{0x9414, 4, 0x00010401, 4, 0}, 
	{0x941f, 20, 0x0001ff01, 20, 0}, 
	//反向有功电能数据块
	{0x9420, 4, 0x00020001, 4, 0}, 
	{0x9421, 4, 0x00020101, 4, 0},
	{0x9422, 4, 0x00020201, 4, 0},
	{0x9423, 4, 0x00020301, 4, 0},
	{0x9424, 4, 0x00020401, 4, 0}, 
	{0x942f, 20, 0x0002ff01, 20, 0},
	//当前正向无功总电能(组合无功1电能数据块)
	{0x9510, 4, 0x00030001, 4, 0}, 
	{0x9511, 4, 0x00030101, 4, 0},
	{0x9512, 4, 0x00030201, 4, 0},
	{0x9513, 4, 0x00030301, 4, 0},
	{0x9514, 4, 0x00030401, 4, 0}, 
	{0x951f, 20, 0x0003ff01, 20, 0},
	//当前反向无功总电能(组合无功2电能数据块)
	{0x9520, 4, 0x00040001, 4, 0}, 
	{0x9521, 4, 0x00040101, 4, 0},
	{0x9522, 4, 0x00040201, 4, 0},
	{0x9523, 4, 0x00040301, 4, 0},
	{0x9524, 4, 0x00040401, 4, 0}, 
	{0x952f, 20, 0x0004ff01, 20, 0},
	//一象限无功电能数据块
	{0x9530, 4, 0x00050001, 4, 0}, 
	{0x9531, 4, 0x00050101, 4, 0}, 
	{0x9532, 4, 0x00050201, 4, 0},
	{0x9533, 4, 0x00050301, 4, 0},
	{0x9534, 4, 0x00050401, 4, 0},
	{0x953f, 20, 0x0005ff01, 20, 0},
	//四象限无功电能数据块
	{0x9540, 4, 0x00080001, 4, 0},
	{0x9541, 4, 0x00080101, 4, 0},
	{0x9542, 4, 0x00080201, 4, 0},
	{0x9543, 4, 0x00080301, 4, 0}, 
	{0x9544, 4, 0x00080401, 4, 0}, 
	{0x954f, 20, 0x0008ff01, 20, 0},
	//二象限无功电能数据块
	{0x9550, 4, 0x00060001, 4, 0}, 
	{0x9551, 4, 0x00060101, 4, 0},
	{0x9552, 4, 0x00060201, 4, 0},
	{0x9553, 4, 0x00060301, 4, 0},
	{0x9554, 4, 0x00060401, 4, 0},
	{0x955f, 20, 0x0006ff01, 20, 0},
	//三象限无功电能数据块
	{0x9560, 4, 0x00070001, 4, 0},
	{0x9561, 4, 0x00070101, 4, 0},
	{0x9562, 4, 0x00070201, 4, 0}, 
	{0x9563, 4, 0x00070301, 4, 0},
	{0x9564, 4, 0x00070401, 4, 0},
	{0x956f, 20, 0x0007ff01, 20, 0},
#endif
	//3.最大需量数据块
	//正向有功总最大需量
	{0xa01f, 40, 0x0101ff00, 40, 0}, 
	{0xa010, 8, 0x01010000, 8, 0},
	{0xa011, 8, 0x01010100, 8, 0}, 
	{0xa012, 8, 0x01010200, 8, 0},
	{0xa013, 8, 0x01010300, 8, 0}, 
	{0xa014, 8, 0x01010400, 8, 0},
	//反向有功总最大需量
	{0xa02f, 40,0x0102ff00, 40, 0},
	{0xa020, 8, 0x01020000, 8, 0},
	{0xa021, 8,0x01020100, 8, 0}, 
	{0xa022, 8,0x01020200, 8, 0},
	{0xa023, 8,0x01020300, 8, 0}, 
	{0xa024, 8,0x01020400, 8, 0},
	//正向无功总最大需量
	{0xa11f, 40,0x0103ff00, 40, 0},
	{0xa110, 8, 0x01030000, 8, 0},
	{0xa111, 8,0x01030100, 8, 0}, 
	{0xa112, 8,0x01030200, 8, 0},
	{0xa113, 8,0x01030300, 8, 0}, 
	{0xa114, 8,0x01030400, 8, 0},
	//反向无功总最大需量
	{0xa12f, 40,0x0104ff00, 40, 0},
	{0xa120, 8, 0x01040000, 8, 0},
	{0xa121, 8,0x01040100, 8, 0}, 
	{0xa122, 8,0x01040200, 8, 0},
	{0xa123, 8,0x01040300, 8, 0}, 
	{0xa124, 8,0x01040400, 8, 0},

#if 0
	//正向有功总最大需量发生时间
	{0xb01f, 20, 0x0101ff00, 40, CPYFLAG_GROUP|5}, 
	{0xb010, 4, 0x01010000, 8, 3},
	{0xb011, 4, 0x01010100, 8, 3}, 
	{0xb012, 4, 0x01010200, 8, 3}, 
	{0xb013, 4, 0x01010300, 8, 3}, 
	{0xb014, 4, 0x01010400, 8, 3},
	//反向有功总最大需量发生时间
	{0xb02f, 20,0x0102ff00, 40, CPYFLAG_GROUP|5},
	{0xb020, 4, 0x01020000, 8, 3},
	{0xb021, 4,0x01020100, 8, 3}, 
	{0xb022, 4,0x01020200, 8, 3},
	{0xb023, 4,0x01020300, 8, 3}, 
	{0xb024, 4,0x01020400, 8, 3},
	//正向无功总最大需量发生时间
	{0xb11f, 20,0x0103ff00, 40, CPYFLAG_GROUP|5},
	{0xb110, 4, 0x01030000, 8, 3},
	{0xb111, 4,0x01030100, 8, 3}, 
	{0xb112, 4,0x01030200, 8, 3},
	{0xb113, 4,0x01030300, 8, 3}, 
	{0xb114, 4,0x01030400, 8, 3},
	//反向无功总最大需量发生时间
	{0xb12f, 20,0x0104ff00, 40, CPYFLAG_GROUP|5},
	{0xb120, 4, 0x01040000, 8, 3},
	{0xb121, 4,0x01040100, 8, 3}, 
	{0xb122, 4,0x01040200, 8, 3},
	{0xb123, 4,0x01040300, 8, 3}, 
	{0xb124, 4,0x01040400, 8, 3},
#endif
	//上月
#if VERSION_AREA == VERSION_JIANGXI
	//正向有功总最大需量
	{0xa41f, 40, 0x05060901, 40, 0},
	//反向有功总最大需量
	{0xa42f, 40,0x05060a01, 40,0},
#else
	//正向有功总最大需量
	{0xa41f, 40, 0x0101ff01, 40, 0}, 
	{0xa410, 8, 0x01010001, 8, 0},
	{0xa411, 8, 0x01010101, 8, 0}, 
	{0xa412, 8, 0x01010201, 8, 0},
	{0xa413, 8, 0x01010301, 8, 0}, 
	{0xa414, 8, 0x01010401, 8, 0},
	//反向有功总最大需量
	{0xa42f, 40,0x0102ff01, 40, 0},
	{0xa420, 8, 0x01020001, 8, 0},
	{0xa421, 8,0x01020101, 8, 0}, 
	{0xa422, 8,0x01020201, 8, 0},
	{0xa423, 8,0x01020301, 8, 0}, 
	{0xa424, 8,0x01020401, 8, 0},
#endif
	//正向无功总最大需量
	{0xa51f, 40,0x0103ff01, 40, 0},
	{0xa510, 8, 0x01030001, 8, 0},
	{0xa511, 8,0x01030101, 8, 0}, 
	{0xa512, 8,0x01030201, 8, 0},
	{0xa513, 8,0x01030301, 8, 0}, 
	{0xa514, 8,0x01030401, 8, 0},
	//反向无功总最大需量
	{0xa52f, 40,0x0104ff01, 40, 0},
	{0xa520, 8, 0x01040001, 8, 0},
	{0xa521, 8,0x01040101, 8, 0}, 
	{0xa522, 8,0x01040201, 8, 0},
	{0xa523, 8,0x01040301, 8, 0}, 
	{0xa524, 8,0x01040401, 8, 0},
#if 0
#if VERSION_AREA == VERSION_JIANGXI
	//正向有功总最大需量发生时间
	{0xb41f, 40, 0x05060901, 40, 0},
	//反向有功总最大需量发生时间
	{0xb42f, 40,0x05060a01, 40,0},
#else
	//正向有功总最大需量发生时间
	{0xb41f, 20, 0x0101ff01, 40, CPYFLAG_GROUP|5}, 
	{0xb410, 4, 0x01010001, 8, 3},
	{0xb411, 4, 0x01010101, 8, 3}, 
	{0xb412, 4, 0x01010201, 8, 3}, 
	{0xb413, 4, 0x01010301, 8, 3}, 
	{0xb414, 4, 0x01010401, 8, 3},
	//反向有功总最大需量发生时间
	{0xb42f, 20,0x0102ff01, 40, CPYFLAG_GROUP|5},
	{0xb420, 4, 0x01020001, 8, 3},
	{0xb421, 4,0x01020101, 8, 3}, 
	{0xb422, 4,0x01020201, 8, 3},
	{0xb423, 4,0x01020301, 8, 3}, 
	{0xb424, 4,0x01020401, 8, 3},
#endif
#endif
#if 0
	//正向无功总最大需量发生时间
	{0xb51f, 20,0x0103ff01, 40, CPYFLAG_GROUP|5},
	{0xb510, 4, 0x01030001, 8, 3},
	{0xb511, 4,0x01030101, 8, 3}, 
	{0xb512, 4,0x01030201, 8, 3},
	{0xb513, 4,0x01030301, 8, 3}, 
	{0xb514, 4,0x01030401, 8, 3},
	//反向无功总最大需量发生时间
	{0xb52f, 20,0x0104ff01, 40, CPYFLAG_GROUP|5},
	{0xb520, 4, 0x01040001, 8, 3},
	{0xb521, 4,0x01040101, 8, 3}, 
	{0xb522, 4,0x01040201, 8, 3},
	{0xb523, 4,0x01040301, 8, 3}, 
	{0xb524, 4,0x01040401, 8, 3},
#endif

	//4//电压数据块
	{0xb611, 2, 0x02010100, 2, 0}, 
	{0xb612, 2, 0x02010200, 2, 0},
	{0xb613, 2, 0x02010300, 2, 0},
	{0xb61f, 6, 0x0201ff00, 6, 0},
	//电流数据块	
	{0xb621, 3, 0x02020100, 3, 0},
	{0xb622, 3, 0x02020200, 3, 0},
	{0xb623, 3, 0x02020300, 3, 0}, 
	{0xb62f, 9, 0x0202ff00, 9, 0},
	//零线电流
	{0xb624, 3, 0x02800001, 3, 0}, 
	//瞬时有功功率数据块
	{0xb630, 3, 0x02030000, 3, 0},
	{0xb631, 3, 0x02030100, 3, 0},
	{0xb632, 3, 0x02030200, 3, 0},
	{0xb633, 3, 0x02030300, 3, 0},
	{0xb63f, 12, 0x0203ff00, 12, 0},
	//瞬时无功功率数据块
	{0xb640, 3, 0x02040000, 3, 0}, 
	{0xb641, 3, 0x02040100, 3, 0}, 
	{0xb642, 3, 0x02040200, 3, 0}, 
	{0xb643, 3, 0x02040300, 3, 0}, 
	{0xb64f, 12, 0x0204ff00, 12, 0}, 
	//总功率因数数据块
	{0xb650, 2, 0x02060000, 2, 0},
	{0xb651, 2, 0x02060100, 2, 0}, 
	{0xb652, 2, 0x02060200, 2, 0}, 
	{0xb653, 2, 0x02060300, 2, 0},
	{0xb65f, 8, 0x0206ff00, 8, 0}, 

	//视在功率
	{0xb6e0, 3, 0x02050000, 3, 0},
	{0xb6e1, 3, 0x02050100, 3, 0},
	{0xb6e2, 3, 0x02050200, 3, 0},
	{0xb6e3, 3, 0x02050300, 3, 0},
	{0xb6ef, 12, 0x0205ff00, 12, 0},

	//ABC相位角
	{0xb660, 2, 0x02070100, 2, 0}, 
	{0xb661, 2, 0x02070200, 2, 0}, 
	{0xb662, 2, 0x02070300, 2, 0},

	//电表运行状态字数据块
	{0xc020, 2, 0x04000501, 2, 0},
	{0xc021, 2, 0x04000502, 2, 0},
	{0xc022, 2, 0x04000503, 2, 0},
	{0xc023, 2, 0x04000504, 2, 0},
	{0xc024, 2, 0x04000505, 2, 0},
	{0xc025, 2, 0x04000506, 2, 0},
	{0xc026, 2, 0x04000507, 2, 0},
	{0xc02f, 14, 0x040005ff, 14, 0},

	{0xc010, 4, 0x04000101, 4, 0}, //日期及星期4
	{0xc011, 3, 0x04000102, 3, 0}, //时间3
	{0xb213, 3, 0x03300200, 3, 0}, //需量清零总次数
	{0xb211, 6, 0x03300201, 194, 0}, //最近一次需量清零时刻
	{0xb212, 3, 0x03300000, 3, 0}, //编程总次数
	{0xb210, 6, 0x03300001, 50, 0}, //最近一次编程时刻
	{0xb214, 4, 0x0280000a, 4, 0}, //电池工作时间
	{0xb215, 3, 0x03300100, 3, 0}, //电表清零次数
	{0xb216, 6, 0x03300101, 106, 0}, //最近一次电表清零时刻
	{0xb217, 3, 0x03300300, 3, 0}, //事件清零总次数
	{0xb218, 6, 0x03300301, 14, 0}, //最近一次事件清零时刻
	{0xb219, 3, 0x03300400, 3, 0}, //校时总次数
	{0xb21a, 6, 0x03300401, 16, 4}, //最近一次事校时时刻
	{0xc33f, 24, 0x04010001, 42, 0},  //第一套第一日时段表数据
	{0xc030, 3, 0x04000409, 3, 0},  //有功脉冲常数
	{0xc031, 3, 0x0400040a, 3, 0},  //无功脉冲常数
	{0xc040, 3, 0x04000306, 3, 0},  //CT变比
	{0xc041, 3, 0x04000307, 3, 0},  //PT变比
	{0xc117, 2, 0x04000b01, 2, 0},  //第一结算日

	//断相次数和累计时间
	{0xb311, 2, 0x13010001, 3, 0},
	{0xb312, 2, 0x13020001, 3, 0},
	{0xb313, 2, 0x13030001, 3, 0},
	{0xb321, 3, 0x13010002, 3, 0},
	{0xb322, 3, 0x13020002, 3, 0},
	{0xb323, 3, 0x13030002, 3, 0},
	{0xb331, 4, 0x13010101, 6, 1},
	{0xb332, 4, 0x13020101, 6, 1},
	{0xb333, 4, 0x13030101, 6, 1},
	{0xb341, 4, 0x13012501, 6, 1},
	{0xb342, 4, 0x13022501, 6, 1},
	{0xb343, 4, 0x13032501, 6, 1},

	//失压次数和累计时间
	{0xb350, 3, 0x10000001, 3, 0}, //失压总次数
	{0xb351, 3, 0x10000002, 3, 0}, //失压总累计时间 
	{0xb352, 3, 0x03110000, 3, 0}, //停电总次数

	//铜损铁损电能量
	{0xe300, 4, 0x00850000, 4, 0},
	{0xe301, 4, 0x00860000, 4, 0},
	{0xe302, 4, 0x00850001, 4, 0},
	{0xe303, 4, 0x00860001, 4, 0},

	//ABC分相电能量
	{0xe310, 4, 0x00150000, 4, 0},
	{0xe311, 4, 0x00160000, 4, 0},
	{0xe312, 4, 0x00170000, 4, 0},
	{0xe313, 4, 0x00180000, 4, 0},
	{0xe320, 4, 0x00290000, 4, 0},
	{0xe321, 4, 0x002a0000, 4, 0},
	{0xe322, 4, 0x002b0000, 4, 0},
	{0xe323, 4, 0x002c0000, 4, 0},
	{0xe330, 4, 0x003d0000, 4, 0},
	{0xe331, 4, 0x003e0000, 4, 0},
	{0xe332, 4, 0x003f0000, 4, 0},
	{0xe333, 4, 0x00400000, 4, 0},
	//上月ABC分相电能量
	{0xe340, 4, 0x00150001, 4, 0},
	{0xe341, 4, 0x00160001, 4, 0},
	{0xe342, 4, 0x00170001, 4, 0},
	{0xe343, 4, 0x00180001, 4, 0},
	{0xe350, 4, 0x00290001, 4, 0},
	{0xe351, 4, 0x002a0001, 4, 0},
	{0xe352, 4, 0x002b0001, 4, 0},
	{0xe353, 4, 0x002c0001, 4, 0},
	{0xe360, 4, 0x003d0001, 4, 0},
	{0xe361, 4, 0x003e0001, 4, 0},
	{0xe362, 4, 0x003f0001, 4, 0},
	{0xe363, 4, 0x00400001, 4, 0},

	//谐波含有率
	{0xe01f, 38, 0x020a01ff, 42, 0},
	{0xe03f, 38, 0x020a02ff, 42, 0},
	{0xe05f, 38, 0x020a03ff, 42, 0},
	{0xe11f, 36, 0x020b01ff, 42, 2},
	{0xe13f, 36, 0x020b02ff, 42, 2},
	{0xe15f, 36, 0x020b03ff, 42, 2},

	{0xd110, 1, 0x04000503, 2, 0},  //电表运行状态字3(用于F161)
	{0xd111, 5, 0x1e000101, 6, 1},  //上一次合闸时刻, yymmddhhmnss
	{0xd112, 5, 0x1d000101, 6, 1},  //上一次跳闸时刻, yymmddhhmnss

	{0xd120, 2, 0x03300000, 3, 0}, //编程总次数(用于F165)
	{0xd121, 5, 0x03300001, 50, 1}, //最近一次编程时刻(用于F165)
	{0xd122, 2, 0x03300e00, 3, 0}, //开表盖总次数
	{0xd123, 5, 0x03300e01, 60, 1},  //最近一次开表盖时间

	{0xd130, 2, 0x03300400, 3, 0}, //校时总次数(用于F166)
	{0xd131, 5, 0x03300401, 16, 5}, //最近一次事校时时刻(用于F166)
	{0xd132, 2, 0x03300500, 3, 0}, //时段表编程记录
	{0xd133, 5, 0x03300501, 120, 1}, //发生时刻

	{0xe481, 2, 0x03330201, 2, 0},  //上1次购电后总购电次数
	{0x9b1a, 5, 0x00900201, 4, 0},  //（当前）透支金额
	{0x9b1b, 5, 0x00900202, 2, 0},  //（当前）购电次数
	{0x9b1d, 5, 0x00900200, 4, 0},  //（当前）剩余金额
	{0x9b1e, 5, 0x03330601, 4, 0},  //上1次购电后累计购电金额
	{0x9b10, 4, 0x00900100, 4, 0},  //（当前）剩余电量
	{0x9b11, 4, 0x00900101, 4, 0},  //（当前）透支电量 
	{0xe450, 4, 0x03320601, 4, 0},  //上1次购电后累计购电量
	{0xe454, 4, 0x04000f04, 4, 0},  //透支电量限值
	{0xe451, 4, 0x04000f01, 4, 0},  //报警电量1限值
	{0xe452, 4, 0x04000f02, 4, 0},  //报警电量2限值

	{0xd201, 27, 0x03100101, 27, 0},  //上日电压合格率统计数据
	{0xd301, 2, 0x03301e01, 2, 0},  //上日停电时间统计(分钟)

	{0xd2f0, 27, 0x0310ff00, 27, 0},  //(当日)电压合格率统计数据
	{0xd2f1, 27, 0x0310ff01, 27, 0},  //(当日)A相电压合格率统计数据
	{0xd2f2, 27, 0x0310ff02, 27, 0},  //(当日)B相电压合格率统计数据
	{0xd2f3, 27, 0x0310ff03, 27, 0},  //(当日)C相电压合格率统计数据

	{0xd200, 27, 0x03100000, 27, 0},  //(当月)电压合格率统计数据
	{0xd210, 27, 0x03100100, 27, 0},  //(当月)A相电压合格率统计数据
	{0xd220, 27, 0x03100200, 27, 0},  //(当月)B相电压合格率统计数据
	{0xd230, 27, 0x03100300, 27, 0},  //(当月)C相电压合格率统计数据

	{0xd300, 3, 0x03110000, 3, 0},  //掉电总次数
	{0xd310, 4, 0x03110100, 4, 0},  //掉电总时间
	
	{0xd410, 1, 0x07100100, 1, 0},  //继电器状态字
	{0xd420, 4, 0x07010101, 4, 0},  //剩余电量（云南个旧）
	{0xd430, 4, 0x07020101, 4, 0},  //剩余金额（云南个旧）

 	{0xff98, 40, 0x04f11007, 40, 0},  //采集器下所挂电表的地址 	
	{0xffa1, 18, 0x04f10a01, 18, 0},  //采集器门节点告警事件记录
	{0xffa2,  3, 0x03300D00,  3, 0},  //采集器门节点  开表箱门总次数 安徽版本
 	{0xffa3, 12, 0x03300D01, 12, 0},  //采集器门节点  (上一次开表箱门记录) 安徽版本
 	//end
	{0, 0, 0, 0, 0},
};
const dl645_2007_t *proto_dl2007_itemlist = dl645_2007_tab;

unsigned short Dl2007ConverItemid(unsigned long lid)
{
	//unsigned short itemid;
#if 0
	int i;

	for(i=0; i<SIZE_IDCONVERTION; i++) {
		if(lid == IdConvertion[i].idlong)
			return IdConvertion[i].idshort;
	}
#else
	const dl645_2007_t *pitem = proto_dl2007_itemlist;

	while(0 != pitem->rid) {
		if(pitem->id == lid) return pitem->rid;
		pitem++;
	}
#endif

	return 0xffff;
}

unsigned long Dl2007LongId(unsigned short sid)
{
#if 0
	int i;

	for(i=0; i<SIZE_IDCONVERTION; i++) {
		if(sid == IdConvertion[i].idshort) return IdConvertion[i].idlong;
	}
#else
	const dl645_2007_t *pitem = proto_dl2007_itemlist;

	while(0 != pitem->rid) {
		if(pitem->rid == sid) return pitem->id;
		pitem++;
	}
#endif

	return 0xffffffff;
}

/**
* @brief 生成读数据命令帧
* @param addr 目的地址
* @param itemid 数据项标识
* @param buf 数据帧缓存区指针
* @param len 缓存区长度
* @return 成功返回数据帧长度, 失败返回-1
*/
int Dl2007MakeReadPktRaw(const unsigned char *addr, unsigned long itemid, unsigned char *buf, int len)
{
	unsigned char check, *puc;
	int i;

	AssertLogReturn(len < 16, -1, "too short buffer len(%d)\n", len);

	puc = buf;
	*puc++ = 0x68;
	for(i=0; i<6; i++) *puc++ = addr[i];
	*puc++ = 0x68;
	*puc++ = 0x11;
	*puc++ = 0x04;
	DEPART_LONG(itemid, puc);
	for(i=0; i<4; i++) puc[i] += 0x33;
	puc = buf;
	check = 0;
	for(i=0; i<14; i++) check += *puc++;
	*puc++ = check;
	*puc = 0x16;

	return 16;
}

/**
* @brief 生成读数据命令帧
* @param addr 目的地址
* @param itemid 数据项标识
* @param buf 数据帧缓存区指针
* @param len 缓存区长度
* @return 成功返回数据帧长度, 失败返回-1
*/
int Dl2007MakeReadPkt(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len)
{
	unsigned char check, *puc;
	int i;
	unsigned long idlong;
	const dl645_2007_t *pitem = proto_dl2007_itemlist;

	AssertLogReturn(len < 16, -1, "too short buffer len(%d)\n", len);

#if 0
	for(i=0; i<SIZE_IDCONVERTION; i++) {
		if(itemid == IdConvertion[i].idshort) break;
	}
	if(i >= SIZE_IDCONVERTION) {
		ErrorLog("unknown id(%04XH)\n", itemid);
		return -1;
	}
	idlong = IdConvertion[i].idlong;
#else
	//DebugPrintLog(0, "start search %04X...\n", itemid);
	while(0 != pitem->rid) {
		if(pitem->rid == itemid) break;
		pitem++;
	}
	if(0 == pitem->rid) return -1;
	idlong = pitem->id;	
#endif

	puc = buf;
	*puc++ = 0x68;
	for(i=0; i<6; i++) *puc++ = addr[i];
	*puc++ = 0x68;
	*puc++ = 0x11;
	*puc++ = 0x04;
	DEPART_LONG(idlong, puc);
	for(i=0; i<4; i++) puc[i] += 0x33;
	puc = buf;
	check = 0;
	for(i=0; i<14; i++) check += *puc++;
	*puc++ = check;
	*puc = 0x16;

	return 16;
}

/**
* @brief 生成读数据命令帧
* @param addr 目的地址
* @param pconfig 写配置指针
* @param buf 数据帧缓存区指针
* @param len 缓存区长度
* @return 成功返回数据帧长度, 失败返回-1
*/
int Dl2007MakeWritePkt(const unsigned char *addr, const struct plwrite_config_t *pconfig, unsigned char *buf, int len)
{
	unsigned char check, *puc;
	int i;
	unsigned long idlong;
	const dl645_2007_t *pitem = proto_dl2007_itemlist;

	AssertLog(pconfig->cmdlen < 0 || pconfig->cmdlen > 241, "invalid cmdlen(%d)\n", pconfig->cmdlen);
	AssertLogReturn((pconfig->cmdlen+16) > len, -1, "too short buffer len(%d)\n", len);

#if 0
	for(i=0; i<SIZE_IDCONVERTION; i++) {
		if(pconfig->itemid == IdConvertion[i].idshort) break;
	}
	if(i >= SIZE_IDCONVERTION) {
		ErrorLog("unknown id(%04XH)\n", pconfig->itemid);
		return -1;
	}
	idlong = IdConvertion[i].idlong;
#else
	while(0 != pitem->rid) {
		if(pitem->rid == pconfig->itemid) break;
		pitem++;
	}
	if(0 == pitem->rid) return -1;
	idlong = pitem->id;
#endif

	puc = buf;
	*puc++ = 0x68;
	for(i=0; i<6; i++) *puc++ = addr[i];
	*puc++ = 0x68;
	*puc++ = 0x14;
	*puc++ = (unsigned char)pconfig->cmdlen + 4;
	DEPART_LONG(idlong, puc);
	for(i=0; i<4; i++) puc[i] += 0x33;
	puc = buf;
	check = 0;
	for(i=0; i<14; i++) check += *puc++;
	for(i=0; i<pconfig->cmdlen; i++) {
		*puc = pconfig->command[i] + 0x33;
		check += *puc++;
	}
	*puc++ = check;
	*puc = 0x16;

	return (pconfig->cmdlen+16);
}
//生成写数据帧
int Dl2007MakeWritePktRaw(const unsigned char *addr, unsigned int Itemid, unsigned char *data,unsigned char data_len,unsigned char *buf)
{
	unsigned char check, *puc;
	int i;

	puc = buf;
	*puc++ = 0x68;
	for(i=0; i<6; i++) *puc++ = addr[i];
	*puc++ = 0x68;
	*puc++ = 0x14;
	*puc++ = data_len + 4;
	DEPART_LONG(Itemid, puc);
	for(i=0; i<4; i++) puc[i] += 0x33;
	puc = buf;
	check = 0;
	for(i=0; i<14; i++) check += *puc++;
	for(i=0; i<data_len; i++) {
		*puc = data[i] + 0x33;
		check += *puc++;
	}
	*puc++ = check;
	*puc = 0x16;

	return data_len+16;
}
/**
* @brief 生成较时数据帧
* @param clock 当前时间
* @param buf 数据帧缓存区指针
* @param len 缓存区长度
* @return 成功返回数据帧长度, 失败返回-1
*/
int Dl2007MakeChkTimePkt(const sysclock_t *pclock, unsigned char *buf, int len)
{
	unsigned char check, *puc;
	int i;

	AssertLogReturn(len < 18, -1, "too short buffer len(%d)\n", len);

	puc = buf;
	*puc++ = 0x68;
	for(i=0; i<6; i++) *puc++ = 0x99;
	*puc++ = 0x68;
	*puc++ = 0x08;
	*puc++ = 0x06;
	*puc++ = (pclock->second/10 << 4) + (pclock->second%10) + 0x33;
	*puc++ = (pclock->minute/10 << 4) + (pclock->minute%10) + 0x33;
	*puc++ = (pclock->hour/10 << 4) + (pclock->hour%10) + 0x33;
	*puc++ = (pclock->day/10 << 4) + (pclock->day%10) + 0x33;
	*puc++ = (pclock->month/10 << 4) + (pclock->month%10) + 0x33;
	*puc++ = (pclock->year/10 << 4) + (pclock->year%10) + 0x33;
	puc = buf;
	check = 0;
	for(i=0; i<16; i++) check += *puc++;
	*puc++ = check;
	*puc = 0x16;

	return 18;
}

/**
* @brief 检查读取数据回应帧并解析数据
* @param addr 目的地址
* @param itemid 数据项标识
* @param buf 帧缓存区指针(解析后的数据也放入这个缓存)
* @param len 缓存区长度
* @return 成功返回解析数据长度, 失败返回负数
*/
int Dl2007CheckRead(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len)
{
	unsigned char check, *puc;
	int i, shift = 0, datalen = 0;
	unsigned long ul;
	const dl645_2007_t *pitem = proto_dl2007_itemlist;
	unsigned short offset, cpyflag;

	while((*buf!=0x68) && (len>0)) {
		buf++;
		shift++;	//前导符字节数
		len--;
	}

	if(len < 13) return -1;
	if(0x68 != buf[0] || 0x68 != buf[7]) return -2;
	for(i=0; i<6; i++) {
		if(addr[i] != buf[i+1]) return -3;
	}
	puc = buf;
	datalen = (int)buf[9]&0xff;
	if((datalen+12) > len) return -6;
	puc = buf;
	check = 0;
	for(i=0; i<(datalen+10); i++) check += *puc++;
	if(check != *puc++) return -7;
	if(0x16 != *puc) return -8;
	if(0xc0 == buf[8]) return -4; // 东软载波芯片通讯超时自定义控制字
	if(buf[8]&0x40) return DL645_UNSUPPORT_ITEM;
	if((buf[8]&0xdf) != 0x91) return -4;

	ul = (((unsigned char)(buf[13]-0x33))<<24) + (((unsigned char)(buf[12]-0x33))<<16)
		+(((unsigned char)(buf[11]-0x33))<<8) + ((unsigned char)(buf[10]-0x33));

#if 0
	for(i=0; i<SIZE_IDCONVERTION; i++) {
		if(ul == IdConvertion[i].idlong) break;
	}
	if( (i>=SIZE_IDCONVERTION) || (IdConvertion[i].idshort!=itemid) )
		return -5;
#else
	while(0 != pitem->rid) {
		if(pitem->id == ul && pitem->rid == itemid) break;
		pitem++;
	}
	if(0 == pitem->rid || pitem->rid != itemid) return -5;
	if(len <= 0) return -1;
#endif
	if(datalen < 4 ) return -6;
	datalen -= 4;

	puc = buf;
	check = 0;
	for(i=0; i<(datalen+14); i++) check += *puc++;
	if(check != *puc++) return -7;
	if(0x16 != *puc) return -8;

	puc = buf + 14;
	cpyflag = pitem->cpycfg&CPYFLAG_MASK;
	offset = pitem->cpycfg&CPYDATA_MASK;
	if(cpyflag == CPYFLAG_GROUP) {
		unsigned short grpnum = offset;
		unsigned char *pdst = buf - shift;
		unsigned char *pend = pdst + len;
		pitem++;
		datalen = 0;
		for(;grpnum!=0;grpnum--,pitem++) {
			cpyflag = pitem->cpycfg&CPYFLAG_MASK;
			offset = pitem->cpycfg&CPYDATA_MASK;
			if(cpyflag == CPYFLAG_GROUP) continue;
			for(i=0; i<pitem->rlen; i++) pdst[i] = puc[i+offset] - 0x33;
			datalen += pitem->rlen;
			pdst += pitem->rlen;
			puc += pitem->len;
		}
		while(pdst<pend) *pdst++ = 0;
	}
	else {
		buf -= shift;
		puc += offset;
		datalen = pitem->rlen;
		for(i=0; i<pitem->rlen; i++) buf[i] = puc[i] - 0x33;
		for(; i<len; i++) buf[i] = 0;
	}

	/*puc = buf + 14;
	for(i=0; i<datalen; i++) buf[i] = (*puc++) - 0x33;*/

	return datalen;
}

/**
* @brief 检查写数据回应帧
* @param addr 目的地址
* @param itemid 数据项标识
* @param buf 帧缓存区指针
* @param len 缓存区长度
* @return 成功返回0, 失败返回负数
*/
int Dl2007CheckWrite(const unsigned char *addr, unsigned short itemid, const unsigned char *buf, int len)
{
	unsigned char check;
	int i, datalen;
	const unsigned char *puc;

	if(len < 11) return -1;
	if(0x68 != buf[0] || 0x68 != buf[7]) return -2;
	for(i=0; i<6; i++) {
		if(addr[i] != buf[i+1])
			return -3;

	}
	if((buf[8]&0xdf) != 0x94) return -4;
	datalen = (int)buf[9]&0xff;
	if((datalen+12) > len) return -6;

	puc = buf;
	check = 0;
	for(i=0; i<(datalen+10); i++) check += *puc++;
	if(check != *puc++) return -7;
	if(0x16 != *puc) return -8;

	return 0;
}

int SepDl2007MakeReadPkt(const unsigned char *owneraddr, unsigned char *addr, unsigned short itemid, unsigned char *buf, int len)
{
	unsigned char check, *puc;
	int i;
	unsigned long idlong;
	const dl645_2007_t *pitem = proto_dl2007_itemlist;

	AssertLogReturn(len < 16, -1, "too short buffer len(%d)\n", len);

	while(0 != pitem->rid) {
		if(pitem->rid == itemid) break;
		pitem++;
	}
	if(0 == pitem->rid) return -1;
	idlong = pitem->id;	
	
	puc = buf;
	*puc++ = 0x68;
	for(i=0; i<6; i++) *puc++ = owneraddr[i];
	*puc++ = 0x68;
	*puc++ = 0x11;
	*puc++ = 0x0a;
	DEPART_LONG(idlong, puc);
	puc+=4;
	for(i=0; i<6; i++) *puc++ = addr[i];
	puc = buf + 10;
	for(i=0; i<10; i++) puc[i] += 0x33;
	puc = buf;
	check = 0;
	for(i=0; i<20; i++) check += *puc++;
	*puc++ = check;
	*puc = 0x16;

	return 22;
}


/**
* @brief 检查读取数据回应帧并解析数据
* @param addr 目的地址
* @param itemid 数据项标识
* @param buf 帧缓存区指针(解析后的数据也放入这个缓存)
* @param len 缓存区长度
* @return 成功返回解析数据长度, 失败返回负数
*/
int SepDl2007CheckRead(const unsigned char *addr, unsigned short itemid, unsigned char *buf, int len)
{
	unsigned char check, *puc,*recbuf = buf;
	int i, shift = 0, datalen = 0;
//	unsigned int ul; 


	while((*buf!=0x68) && (len>0)) {
		buf++;
		shift++;	//前导符字节数
		len--;
	}

	if(len < 13) return -1;
	if(0x68 != buf[0] || 0x68 != buf[7]) return -2;
	for(i=0; i<6; i++) 
	{
		if(addr[i] != 0xAA)
		{
				if(addr[i] != buf[i+1])
					return -3;
		}
	}
	puc = buf;
	datalen = (int)buf[9]&0xff;
	if((datalen+12) > len) return -6;
	puc = buf;
	check = 0;
	for(i=0; i<(datalen+10); i++) check += *puc++;
	if(check != *puc++) return -7;
	if(0x16 != *puc) return -8;
	if(0xc0 == buf[8]) return -4; // 东软载波芯片通讯超时自定义控制字
	if(buf[8]&0x40) return DL645_UNSUPPORT_ITEM;
	if((buf[8]&0xdf) != 0x91) return -4;

//	ul = (((unsigned char)(buf[13]-0x33))<<24) + (((unsigned char)(buf[12]-0x33))<<16)
//		+(((unsigned char)(buf[11]-0x33))<<8) + ((unsigned char)(buf[10]-0x33));

	if(datalen < 4 ) return -6;
	datalen -= 4;

	puc = buf;
	check = 0;
	for(i=0; i<(datalen+14); i++) check += *puc++;
	if(check != *puc++) return -7;
	if(0x16 != *puc) return -8;

	puc = buf + 14;

	for(i=0; i<datalen; i++) recbuf[i] = (*puc++) - 0x33;

	return datalen;
}
/**
* @brief 检查返回的数据回应帧并解析数据域得到地址,命令码
* @param addr 得到目的地址
* @param buf 帧缓存区指针(解析后的数据也放入这个缓存)
* @param len 缓存区长度
* @return 成功返回解析数据长度, 失败返回负数
*/
int SepDl2007CheckFrame(unsigned char *addr, unsigned char *code, unsigned char *buf, int len)
{
	unsigned char check, *puc,*recbuf = buf;
	int i, shift = 0, datalen = 0;
//	unsigned int ul; 


	while((*buf!=0x68) && (len>0)) {
		buf++;
		shift++;	//前导符字节数
		len--;
	}

	if(len < 12) return -1;
	if(0x68 != buf[0] || 0x68 != buf[7]) return -2;
	for(i=0; i<6; i++) 
	{
		addr[i] = buf[i+1];
	}
	puc = buf;
	datalen = (int)buf[9]&0xff;
	if((datalen+12) > len) return -6;
	puc = buf;
	check = 0;
	for(i=0; i<(datalen+10); i++) check += *puc++;
	if(check != *puc++) return -7;
	if(0x16 != *puc) return -8;
	*code = buf[8];
	puc = buf + 14;
	for(i=0; i<datalen; i++) recbuf[i] = (*puc++) - 0x33;

	return datalen;
}

