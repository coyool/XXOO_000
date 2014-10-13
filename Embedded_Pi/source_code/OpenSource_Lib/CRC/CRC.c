/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-10-13
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/






/*******************************************************************************
* Description : CRC_CC1101
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
u16 CRC_CC1101(u8 *FIFO_buff, u8 n)
{
    // CRC 软件解码  输入CRC_reg = CRC种子  输出 CRC_reg = CRC 校验和 
Uint16 Calc_CRCSUM(Uchar8 *FIFO_buff, Uchar8 n)
{
     Uchar8 i;
     Uchar8 x;
     Uint16 CRC_SUM;
     Uchar8 dat;
     
     CRC_SUM = 0xFFFF; //CRC_SUM的初始值 = 0xFFFF，相关内容查看DN502 
     
     for (x=0; x<n; x++)
     {   
         dat = *(FIFO_buff + x);  //导入FIFO_buff 数组数据
         
         for (i=0; i<8; i++)
         {
             if (((CRC_SUM & 0x8000) >> 8) ^ (dat & 0x80))
             {
                CRC_SUM = (CRC_SUM << 1) ^ 0x8005;  //CRC_16算法   0x8005 查看DN095
             }
             else
             {
                CRC_SUM = (CRC_SUM << 1);
             }//end if    
             dat = dat << 1;
         }// 1 byte Clac 
     }//end for  CRC_SUM 
     
     return CRC_SUM;
}
