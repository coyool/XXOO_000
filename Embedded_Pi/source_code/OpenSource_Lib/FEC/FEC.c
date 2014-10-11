/*******************************************************************************
* Copyright 2014      Jr 
* All right reserved
*
* 文件名称：FEC
*
* 文件标识：
* 摘    要：
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-10-10
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
u8  fecEncodeTable[] = 
{ 
    0, 3, 1, 2, 
    3, 0, 2, 1,
    3, 0, 2, 1,
    0, 3, 1, 2 
}; 


/*** extern variable declarations ***/






/*******************************************************************************
* Description : payload data pass (3,1,4)convolution encode and Interleave encode 
*               transmitted over the air 
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void FEC_enCode(void)
{
    
}


/*******************************************************************************
* Description : Recv data pass Interleave decode and Viterbi decode restore 
*               payload data
* Syntax      : 
* Parameters I: 
* Parameters O: 
* return      : 
*******************************************************************************/
void FEC_deCode(void)
{
    
}

