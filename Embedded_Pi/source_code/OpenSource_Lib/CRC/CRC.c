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
* 源代码说明：calc -- calculate
*           
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/



/*** extern variable declarations ***/






/*******************************************************************************
* Description : CC1101 CRC software implementation, detail:DN502.
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
u16 calc_CRC_CC1101(const u8 *FIFO_buff, const u8 size)
{
     u8 i;
     u8 j;
     u16 CRC_SUM;
     u8 dat;
     
     CRC_SUM = 0xFFFF;        /* CRC init value = 0xFFFF */
     
     for (j=0; j<size; j++)
     {   
         dat = *(FIFO_buff + j);  
         
         for (i=0; i<8; i++)
         {
             if (((CRC_SUM & 0x8000) >> 8) ^ (dat & 0x80))
             {
                CRC_SUM = (CRC_SUM << 1) ^ 0x8005; //CRC_16算法 0x8005 查看DN095
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

/*******************************************************************************
* Description : 查表实现
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
