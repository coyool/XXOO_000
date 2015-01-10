#ifndef __OBIS_H
#define __OBIS_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef eDataAccessResult(*IC_FUNC)(const tObjRequest * req, tCosemMem * mem);
#pragma pack(push, 1)
typedef struct
{
	uint32 Value;
}tDLMS_AttrOperation;

/* OBIS code 定义 */
typedef struct
{
	uint8 a;
	uint8 b;
	uint8 c;
	uint8 d;
	uint8 e;
	uint8 f;
}tObisCode;

/* 基本对象数据结构定义 */
typedef struct
{
	tObisCode logic_name;       // 对象逻辑名, LN参考
	tDLMS_AttrOperation access_right[3];      // 对象属性访问权限 
	uint16 item;
}tBaseObj;

/* Data类对象数据结构定义 */
typedef struct
{
	tObisCode LogicName;        // 对象逻辑名, LN参考
	tDLMS_AttrOperation access_right[3];      // 对象属性访问权限 bit0-get  bit1-set  bit2-action
    uint16 item;
	eCosemDataType DataType;    // 数据类型
}tDataObj;
#pragma pack(pop)
#pragma pack(push, 1)
/* 接口类对象数据结构定义 */
typedef struct
{
	uint16 class_id;      // 类ID
	uint8 ver;            // 类版本号
	uint8 attribute_num;  // 属性数量
	uint8 method_num;     // 方法数量
	IC_FUNC GetFunc;      // get_service 接口函数指针
	IC_FUNC SetFunc;      // set_service 接口函数指针
	IC_FUNC ActionFunc;   // action_service 接口函数指针
	const void * obj;     // 对象基址
	uint8 obj_size;       // 对象大小
	uint16 obj_num;        // 对象数
}tObjTable;

/* Register, Extended Register, Demand Register类对象数据结构定义 */
typedef struct
{
	tObisCode LogicName;        // 对象逻辑名
	tDLMS_AttrOperation access_right[3];      // 对象属性访问权限 bit0-get  bit1-set  bit2-action
    uint16 item;
	eCosemDataType DataType;    // 数据类型
//	tScalerUnitInfo ScalerUnit; // 单位量纲
}tRegisterObj, tExtRegisterObj;

/* Profile 捕获对象数据结构定义 */
typedef struct
{
	uint16 ClassId;			    // 类名
	tObisCode LogicName;		// 对象逻辑名
	int8 AttributeIndex;	    // 属性索引
//	uint16 DataIndex;		    // 数据索引
    eCosemDataType DataType;    // 数据类型
    tScalerUnitInfo ScalerUnit; // 单位量纲
}tCaptureObj;

/* Generic Profile类对象数据结构定义 */
typedef struct
{
	tObisCode logic_name;		// 对象逻辑名
	tDLMS_AttrOperation access_right[3];      // 对象属性访问权限 bit0-get  bit1-set  bit2-action
    uint16 item;
    const tCaptureObj * capture_obj;
    uint8 capture_obj_num;
}tProfileObj;
#pragma pack(pop)
/* Exported constants --------------------------------------------------------*/
//extern const tObjTable cObjectTable[];
//extern const uint8 cValidObjTabNum;

/* Exported defines ----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
BOOL_B ObjFindThroughObis(tObjFind *req);
BOOL_B ObjFindThroughDataId(tObjFind *req);
const tObjTable* ObjGetObjctTAbleInfo(uint8 *pIndex, uint8 Type);


#endif 

