/*******************************************************************************
* Copyright 2014      深圳思达仪表  固件部  
* All right reserved
*
* 文件名称：shell
*
* 文件标识：
* 摘    要：shell code
*
* 当前版本：
* 作    者：F06553
* 完成日期：2014-9-5
* 编译环境：D:\software\IAR_for_ARM\arm
* 
* 源代码说明：
*******************************************************************************/
#include    "all_header_file.h"

/*** static function prototype declarations ***/



/*** static variable declarations ***/
const uint32_t SHELL_TIMEOUT = 0xFFFFFFFFu;
const char Shell_command[SHELL_NUM][SHELL_LEN] =  
{
    {"ST_UD"},
    {" flash --map\r"},
    {" status\r"},
    {" reset\r"},
    {" quit\r"},
    {" NULL"}
};

void (*task_index[SHELL_NUM])(void) = 
{
    NVIC_SystemReset,    /* "ST_UD" invalid  */
    Flash_map,           /* printf Flash image */  
    ST_UD_status,        /* printf status information */ 
    NVIC_SystemReset,    /* system reset, also soft reset */
    quit
};

/*** extern variable declarations ***/





/*******************************************************************************
* 函数名称: quit
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void quit(void)
{
    __NOP();
}



/*******************************************************************************
* 函数名称: 
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void UART_Shell_RX(uint8_t *dat, uint32_t size_max, uint32_t timeout)
{
    uint32_t timeout_cnt = 0;
    uint32_t size_cnt =0;
    
    /* Check and receive */
    if(dat != NULL)
    {
        /* Enable the  RX*/
        //MFS_UARTEnableRX(InUseCh);
        while ((timeout_cnt < timeout) 
               && ('\r' != *(dat - 1)) 
               && (size_cnt < size_max))
        {
            if (MFS_UARTGetOEStatus(UartUSBCh) 
                || MFS_UARTGetPEStatus(UartUSBCh) 
                || MFS_UARTGetFEStatus(UartUSBCh))
            {
                MFS_UARTErrorClr(UartUSBCh);  //CLR error flag
                break;//Error happen
            }
            if(MFS_UARTGetRXRegFullStatus(UartUSBCh))
            {
                *dat = MFS_UARTRXOneData(UartUSBCh);
                dat++;
                size_cnt++;
                bFM3_GPIO_PDOR0_PC = ~bFM3_GPIO_PDIR0_PC;  /* LED 202 */
            }
            timeout_cnt++;
        }
        /* Disable the  RX*/
        //MFS_UARTDisableRX(InUseCh);
    }
    return;
}

/*******************************************************************************
* 函数名称: shell
* 输入参数: 
* 输出参数: 
* --返回值: 
* 函数功能: --
*******************************************************************************/
void shell(void)
{
    uint8_t Shell_buff_tab[SHELL_LEN] = {0u};
    uint8_t temp = 0u;
    uint8_t Char = 0u;
    uint8_t i = 0u;
    int cmp_val = 0u;
    
    temp = Get_One_char(MFS_Ch0, &Char);
    if (1u == temp)
    {
        if (('\r' == Char))
        {
            Char = 0u;
            bFM3_GPIO_PDOR0_PC = 0;  /* LED 202 */
            
            UART_Shell_RX(Shell_buff_tab, SHELL_LEN, SHELL_TIMEOUT);
            
            cmp_val = memcmp((&Shell_command[0][0]), Shell_buff_tab, 5);
            if (0 == cmp_val)
            {
                for (i=0u; i<SHELL_NUM; i++)
                {
                    cmp_val = memcmp((&Shell_command[i][0]), &Shell_buff_tab[5], SHELL_LEN-5);
                    if (0u == cmp_val)
                    {
                        //(*task_index[i])(void);
                        (*task_index[i])();
                    }//end if judge shell
                }//end for 
            }//end if cmp "ST_UD"
        }//end if cmp 'S' 's'    
    }    
  
    bFM3_GPIO_PDOR0_PC = 1u; /* LED 202 */
}



