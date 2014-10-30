/*
 *预定义的宏
 */
#ifndef __PLAT_DEFINE_H__
#define __PLAT_DEFINE_H__

/*
库的名称
*/
#define  _LIB_NAME_ "plat"




/*
    函数有返回值时，当只返回函数执行的成功或失败时，0表示成功，
失败的代码可以参考错误代码值，当函数返回值是表示数据长度时均为
正值或0；
*/

#define SUCCEED     0   /* Operation suceed */

/*
    所有返回的错误代码都是负值，以下定义的错误代码都是正值，在
返回时前面加负号变成负值后返回。
*/

#define ERRFAILED      1 /* 执行失败 */
#define	ERRPERM        2 /* 禁止操作 */
#define	ERRNOENT       3 /* 没有该文件或路径 */
#define	ERR2BIG        4 /* 参数列表太长 */
#define	ERRNOEXEC      5 /* 执行错误 */
#define	ERRBADF        6 /* 文件编号错误 */
#define	ERRACCES       7 /* 无权限 */
#define	ERRFAULT       8 /* 地址错误 */
#define	ERRBUSY	       9 /* 设备或资源正在在使用 */
#define	ERREXIST       10 /* 文件已存在 */
#define	ERRNODEV       11 /* 没有改设备 */
#define	ERRNOTDIR      12 /* 不是正确路径 */
#define	ERRINVAL       13 /* 参数错误 */
#define	ERRNFILE       14 /* 文件列表溢出 */
#define	ERRMFILE       15 /* 文件重复打开 */
#define	ERRFBIG        16 /* 文件太大 */
#define	ERRRANGE       17 /* 参数越界 */
#define ERRCONFIG      18 /* 配置错误 */
#define ERRGPRS        19 /* GPRS错误 */
#define ERRSMSERROR	   20 /* 短信错误 */
/*
    按键值定义
*/

#define VK_LEFT             0x25    //37
#define VK_UP               0x26	    
#define VK_RIGHT            0x27
#define VK_DOWN             0x28
#define VK_ENTER            0x0D    //13
#define VK_ESC              0x1B    //27
#define VK_PROGRAM          0x0E
#define VK_LONG_LEFT        0x0F
#define VK_LONG_UP          0x10
#define VK_LONG_RIGHT       0x11
#define VK_LONG_DOWN        0x12
#define VK_LONG_ENTER       0x13
#define VK_LONG_ESC         0x14
#define VK_LONG_PROGRAM     0x15
#define VK_GRP_ENTER_LEFT   0x16
#define VK_GRP_ENTER_UP     0x17
#define VK_GRP_ENTER_RIGHT  0x18
#define VK_GRP_ENTER_DOWN   0x19

#endif
