/*!
 ******************************************************************************
 **
 ** \file workflash_fm3.c
 **
 ** \brief work flash drivers for MB9AF310K series MCU
 **
 ** \author FSLA AE Team
 **
 ** \version V0.10
 **
 ** \date 2013-02-01
 **
 ** \attention THIS SAMPLE CODE IS PROVIDED AS IS. FUJITSU SEMICONDUCTOR
 **            ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR
 **            OMMISSIONS.
 **
 ** (C) Copyright 200x-201x by Fujitsu Semiconductor(Shanghai) Co.,Ltd.
 **
 ******************************************************************************
 **
 ** \note Other information.
 **
 ******************************************************************************
 */
/*---------------------------------------------------------------------------*/
/* include files                                                             */
/*---------------------------------------------------------------------------*/
#include "workflash_fm3.h"

#if (PERIPHERAL_AVAILABLE_WORKFLASH == ON) && (PERIPHERAL_ENABLE_WORKFLASH == ON)

/*---------------------------------------------------------------------------*/
/* \brief Flash function Macro                                               */
/*---------------------------------------------------------------------------*/
#define  Flash_Read(addr)            *(volatile uint16_t*)((uint32_t)(addr))
#define  Flash_Write(addr, data)     *(volatile uint16_t*)((uint32_t)(addr)) = (uint16_t)(data)
#define  FlashWKF_Write(addr, data)  *(volatile uint16_t*)((uint32_t)(addr)) = (uint16_t)(data)

static uint8_t WFlash_WriteOrigin( uint16_t* pWriteAddr, uint16_t WriteData ) ;
static uint8_t WFlashDummy(uint8_t Status);
static void     WFlash_WaitRdyState (void);
static FlagStatusT WFlash_GetRdyStatus(void);
static FlagStatusT WFlash_GetRunMode(void);
static void WFlash_SetRunMode(uint8_t programmode);
/*!
 ******************************************************************************
 ** \brief operate flash dummy 
 **
 ** \param Dummy status
 ** \arg   
 **     
 ** \return None
 **
 ******************************************************************************
 */
uint8_t WFlashDummy(uint8_t Status)
{
    uint8_t  dummy;
    dummy = Status;
    return dummy;
}
/*!
 ******************************************************************************
 ** \brief Get work flash ready status
 **
 ** \param None
 ** \arg   None
 **         
 ** \return FlagStatus
 **
 ******************************************************************************
 */
FlagStatusT WFlash_GetRdyStatus(void)
{
   return ((FlagStatusT)((FM3_WORKFLASH_IF->WFSTR & WFLASH_WFSTR_RDY) == WFLASH_WFSTR_RDY));
}
/*!
 ******************************************************************************
 ** \brief Get work flash program status
 **
 ** \param None
 ** \arg   None
 **         
 ** \return FlagStatus
 **
 ******************************************************************************
 */
FlagStatusT WFlash_GetRunMode(void)
{
  return ((FlagStatusT)((FM3_WORKFLASH_IF->WFASZR & WFLASH_WFASZR_ASZ) == WFLASH_WFASZR_ASZ));
}
/*!
 ******************************************************************************
 ** \brief Set work flash programming mode
 **
 ** \param program mode:
 ** \arg   WFLASH_PGM_MODE
           WFLASH_ROM_MODE
 **         
 ** \return None
 **
 ******************************************************************************
 */
void WFlash_SetRunMode(uint8_t programmode)
{
    uint8_t PGMMode;
    PGMMode = programmode;
    FM3_WORKFLASH_IF->WFASZR = PGMMode;
}

/*!
 ******************************************************************************
 ** \brief wait work flash erase/write operation end 
 **
 ** \param None
 ** \arg   None
 **     
 ** \return None
 **
 ******************************************************************************
 */
void WFlash_WaitRdyState(void)
{
    while(!WFlash_GetRdyStatus());
}
/*!
 ******************************************************************************
 ** \brief set flash back to read or reset mode 
 **
 ** \param pResetSecAddr
 ** \arg   operate address
 **     
 ** \return None
 **
 ******************************************************************************
 */
void WFlash_ReadResetCmd(uint16_t* pResetSecAddr)
{
    WFlash_SetRunMode(WFLASH_PGM_MODE); 
    WFlashDummy(WFlash_GetRunMode());  
    
    /*  issue read/reset command    */
    Flash_Write(pResetSecAddr, FLASH_READ_RESET) ;
    
    WFlash_SetRunMode(WFLASH_ROM_MODE); 
    WFlashDummy(WFlash_GetRunMode()); 
    WFlashDummy(Flash_Read(pResetSecAddr));
}
/*!
 ******************************************************************************
 ** \brief check flash operating status 
 **
 ** \param p_sec_addr
 ** \arg   operate address
 **     
 ** \return FLASH_CHK_TOGG_NORMAL   : successfully complete 
 **         FLASH_CHK_TOGG_ABNORMAL : flash operation abnormally 
 **
 ******************************************************************************
 */
static uint8_t WFlash_CheckToggle( uint16_t* p_sec_addr )
{
    uint8_t    ucRetValue  = FLASH_CHK_TOGG_NORMAL ;  
    uint32_t   usSequenceFlag1, usSequenceFlag2 ;  /*  hardware sequence flag */
    
    if((p_sec_addr<=(uint16_t *)(FLASH_END_ADDR-2))&&((p_sec_addr>=(uint16_t *)FLASH_STR_ADDR)))
    {
      /* set hardware sequence flag */
      usSequenceFlag1 = Flash_Read(p_sec_addr) ;
      usSequenceFlag2 = Flash_Read(p_sec_addr) ;
      /*  if automatic algorithm is executing */
      while((( usSequenceFlag1 ^ usSequenceFlag2) & FLASH_CHK_TOGG_MASK) == FLASH_CHK_TOGG_MASK)
      {
        /*  if exceeds the timing limit */
        if(( usSequenceFlag1 & FLASH_CHK_TLOV_MASK) == FLASH_CHK_TLOV_MASK)
        {
            /* set hardware sequence flag */
            usSequenceFlag1 = Flash_Read(p_sec_addr) ;
            usSequenceFlag2 = Flash_Read(p_sec_addr) ;

            /*  if automatic algorithm is executing */
            if((( usSequenceFlag1 ^ usSequenceFlag2) & FLASH_CHK_TOGG_MASK) == FLASH_CHK_TOGG_MASK)
            {
                /*  abnormally complete */
                ucRetValue  = FLASH_CHK_TOGG_ABNORMAL ;
                break;
            }
        }
        /* set hardware sequence flag */
        usSequenceFlag1 = Flash_Read(p_sec_addr) ;
        usSequenceFlag2 = Flash_Read(p_sec_addr) ;
      }
    }
    else
        ucRetValue = FLASH_CHK_TOGG_ABNORMAL;
    return ucRetValue ;
}
/*!
 ******************************************************************************
 ** \brief read flash in 8 bit mode 
 **
 ** \param pSecAddr
 ** \arg   operate address
 **     
 ** \return readed 8 bit data from flash
 **
 ******************************************************************************
 */
uint8_t WFlash_Read8bit(uint8_t* pReadAddr)
{
    uint8_t  ReadData;    
    uint8_t  *Addr;
    Addr = (uint8_t *)pReadAddr;
    
     if((Addr>=(uint8_t *)FLASH_STR_ADDR)&&(Addr<=(uint8_t *)(FLASH_END_ADDR-1)))
     {        
        if(((uint32_t)Addr%2)== 1)
        {
            Addr--;
            ReadData = (uint8_t)(((Flash_Read(Addr))&0xFF00)>>8);
        }
        else
        {
            ReadData = (uint8_t)((Flash_Read(Addr))&0x00FF);
        } 
        return ReadData;
     }
     return 0;
}
/*!
 ******************************************************************************
 ** \brief read flash in 16 bit mode 
 **
 ** \param pSecAddr
 ** \arg   operate address
 **     
 ** \return readed 16 bit data from flash
 **
 ******************************************************************************
 */
uint16_t WFlash_Read16bit(uint16_t* pReadAddr)
{    
    uint8_t  *Addr;
    uint16_t  ReadData;  
    Addr = (uint8_t *)pReadAddr;
    if((Addr>=(uint8_t *)FLASH_STR_ADDR)&&(Addr<=(uint8_t *)(FLASH_END_ADDR-2)))
    {
        if(((uint32_t)Addr%2) == 1)
        {
            Addr--;
            ReadData = (uint16_t)(Flash_Read(Addr));
        }
        else
        {
            ReadData = (uint16_t)(Flash_Read(Addr));
        } 
        return ReadData;
    }
    
    return 0;

}
/*!
 ******************************************************************************
 ** \brief read flash in 16 bit mode 
 **
 ** \param p_sec_addr
 ** \arg   operate address
 **     
 ** \param WriteData
 ** \arg   Data need to be written
 **     
 ** \return FLASH_CHK_TOGG_NORMAL   : successfully complete
 **         FLASH_CHK_TOGG_ABNORMAL : flash operation abnormally 
 **
 ******************************************************************************
 */
static uint8_t WFlash_WriteOrigin( uint16_t* pWriteAddr, uint16_t WriteData ) 
{
    uint8_t  ucRetValue  = FLASH_RET_OK;
    uint32_t  AddrAA8,Addr554;

    if((pWriteAddr<=(uint16_t *)(FLASH_END_ADDR-2))&&((pWriteAddr>=(uint16_t *)FLASH_STR_ADDR)))
    {
        AddrAA8 = (((uint32_t)pWriteAddr&0xFFFFF000)|ADDR_AA8); 
        Addr554 = (((uint32_t)pWriteAddr&0xFFFFF000)|ADDR_554);
        /*  CPU programming mode setting    */
        WFlash_SetRunMode(WFLASH_PGM_MODE);
        WFlashDummy(WFlash_GetRunMode()); 
        
        /*  issue write command   */
        Flash_Write(AddrAA8, FLASH_WRITE_1) ;
        Flash_Write(Addr554, FLASH_WRITE_2) ;
        Flash_Write(AddrAA8, FLASH_WRITE_3) ;
        Flash_Write(pWriteAddr,(uint16_t)WriteData);
        
        /*  if execution result of the automatic algorithm of flash memory is abnormally completed  */
        if( WFlash_CheckToggle((uint16_t *)pWriteAddr) == FLASH_CHK_TOGG_ABNORMAL)
        {
            /*  sending the read/reset command to the reset sector  */
            WFlash_ReadResetCmd((uint16_t *)pWriteAddr) ;
        
        /*  return flash operation abnormally   */
        ucRetValue = FLASH_RET_ABNORMAL ;
        }
        /*  CPU ROM mode setting    */
        WFlash_SetRunMode(WFLASH_ROM_MODE);
        WFlashDummy(WFlash_GetRunMode()); 
    }
    else
    {
        ucRetValue  = FLASH_RET_INVALID_PARA ;
    }
    return ucRetValue ;
}
/*!
 ******************************************************************************
 ** \brief write 8 bit data to target address  
 **
 ** \param pWriteAddr
 ** \arg   operate address
 **     
 ** \param WriteData
 ** \arg   Data need to be written
 **     
 ** \return FLASH_RET_OK            : successfully complete
 **         FLASH_RET_ABNORMAL      : flash operation abnormally 
 **         FLASH_RET_INVALID_PARAM : parameter invalid
 **
 ******************************************************************************
 */
uint8_t WFlash_Write8bit( uint8_t *pWriteAddr, uint8_t  WriteData )
{
    uint8_t   ucRetValue = FLASH_RET_OK ;
    uint16_t  WriteDataH, WriteDataL, WriteDataCon;
    uint8_t   *Addr;
    Addr = (uint8_t *)pWriteAddr;  
    if((pWriteAddr<=(uint8_t *)(FLASH_END_ADDR-1))&&((pWriteAddr>=(uint8_t *)FLASH_STR_ADDR)))
    {
        if(((uint32_t)pWriteAddr%2)== 1)
        {
            Addr--;
            WriteDataL   = (Flash_Read(Addr))&0x00FF;
            WriteDataH   = ((uint16_t)WriteData)<<8;
            WriteDataCon = (WriteDataH|WriteDataL);
        }
        else
        {
            WriteDataH   = (Flash_Read(Addr))&0xFF00;
            WriteDataCon = (WriteDataH|(uint16_t)WriteData);
        }
        ucRetValue     = WFlash_WriteOrigin((uint16_t*)Addr,WriteDataCon);
    }
    else
    {
        ucRetValue  = FLASH_RET_INVALID_PARA;
    } 
    return ucRetValue ;
}
/*!
 ******************************************************************************
 ** \brief write 16 bit data to target address  
 **
 ** \param pWriteAddr
 ** \arg   operate address
 **     
 ** \param WriteData
 ** \arg   Data need to be written
 **     
 ** \return FLASH_RET_OK            : successfully complete
 **         FLASH_RET_ABNORMAL      : flash operation abnormally 
 **         FLASH_RET_INVALID_PARAM : parameter invalid
 **
 ******************************************************************************
 */
uint8_t WFlash_Write16bit( uint16_t* pWriteAddr, uint16_t WriteData ) 
{
    uint8_t  ucRetValue  = FLASH_RET_OK;
    uint8_t  count,DataBuf;
    uint8_t  *Addr;
    
    if((pWriteAddr<=(uint16_t *)(FLASH_END_ADDR-2))&&((pWriteAddr>=(uint16_t *)FLASH_STR_ADDR)))
    {
        if(((uint32_t)pWriteAddr%2) == 0) 
        {
            ucRetValue = WFlash_WriteOrigin(pWriteAddr,WriteData);
        }
        else
        {
            Addr = (uint8_t *)pWriteAddr;
            for(count=2;count>0;count--)
            {
                switch(count)
                {
                    case 2:DataBuf = (uint8_t)( WriteData&0x00FF)     ; break; 
                    case 1:DataBuf = (uint8_t)((WriteData&0xFF00)>>8) ; break;  
                }
                ucRetValue |= WFlash_Write8bit(Addr,DataBuf);
                Addr++;
            }    
        }
    }
    else
    {
        ucRetValue  = FLASH_RET_INVALID_PARA ;
    }  
     return ucRetValue ;
}
/*!
 ******************************************************************************
 ** \brief write 32 bit data to target address  
 **
 ** \param pWriteAddr
 ** \arg   operate address
 **     
 ** \param WriteData
 ** \arg   Data need to be written
 **     
 ** \return FLASH_RET_OK            : successfully complete
 **         FLASH_RET_ABNORMAL      : flash operation abnormally 
 **         FLASH_RET_INVALID_PARAM : parameter invalid
 **
 ******************************************************************************
 */
uint8_t WFlash_Write32bit(uint32_t *pWriteAddr, uint32_t WriteData ) 
{
    uint8_t   ucRetValue  = FLASH_RET_OK ;
    uint8_t   count,DataBuf;
    uint8_t   *Addr;
    Addr = (uint8_t *)pWriteAddr; 
    
    if((pWriteAddr<=(uint32_t *)(FLASH_END_ADDR-4))&&((pWriteAddr>=(uint32_t *)FLASH_STR_ADDR)))
    {
        for(count=4;count>0;count--)
        {
            switch(count)
            {
                case 4: DataBuf = (uint8_t)( WriteData&0x000000FF)     ; break;
                case 3: DataBuf = (uint8_t)((WriteData&0x0000FF00)>>8) ; break;
                case 2: DataBuf = (uint8_t)((WriteData&0x00FF0000)>>16); break;
                case 1: DataBuf = (uint8_t)((WriteData&0xFF000000)>>24); break;   
            }
            ucRetValue |= WFlash_Write8bit(Addr,DataBuf);
            Addr++;
        }
    }
    else
    {
        ucRetValue  = FLASH_RET_INVALID_PARA ;
    } 
    return ucRetValue;
}
/*!
 ******************************************************************************
 ** \brief erase flash in sector erase mode  
 **
 ** \param pSecAddr
 ** \arg   operate address
 **     
 ** \return FLASH_RET_OK            : successfully complete
 **         FLASH_RET_ABNORMAL      : flash operation abnormally 
 **         FLASH_RET_INVALID_PARAM : parameter invalid
 **
 ******************************************************************************
 */
uint8_t WFlash_SectorErase(uint32_t* pSecAddr)
{
    uint8_t  ucRetValue = FLASH_RET_OK;
    uint32_t AddrAA8,Addr554;
    
    if((pSecAddr==(uint32_t *)FLASH_ADDRESS_SA0)
       ||(pSecAddr==(uint32_t *)FLASH_ADDRESS_SA1)
       ||(pSecAddr==(uint32_t *)FLASH_ADDRESS_SA2)
       ||(pSecAddr==(uint32_t *)FLASH_ADDRESS_SA3)) 
    {
        AddrAA8 = (((uint32_t)pSecAddr&0xFFFFF000)|ADDR_AA8); 
        Addr554 = (((uint32_t)pSecAddr&0xFFFFF000)|ADDR_554);  
        /* CPU programming mode setting */
        WFlash_SetRunMode(WFLASH_PGM_MODE);
        WFlashDummy(WFlash_GetRunMode()); 

        Flash_Write(AddrAA8, FLASH_SECTOR_ERASE_1);
        Flash_Write(Addr554, FLASH_SECTOR_ERASE_2);
        Flash_Write(AddrAA8, FLASH_SECTOR_ERASE_3);
        Flash_Write(AddrAA8, FLASH_SECTOR_ERASE_4);
        Flash_Write(Addr554, FLASH_SECTOR_ERASE_5);
        Flash_Write(pSecAddr, FLASH_SECTOR_ERASE_6);
        
         /*  if execution result of the automatic algorithm of flash memory is abnormally completed  */
        if( WFlash_CheckToggle((uint16_t*)pSecAddr) == FLASH_CHK_TOGG_ABNORMAL)
        {
            /*  sending the read/reset command to the reset sector  */
            WFlash_ReadResetCmd((uint16_t*)pSecAddr) ;

            /*  return flash operation abnormally   */
            ucRetValue  = FLASH_RET_ABNORMAL ;
        }
        /*  CPU ROM mode setting    */
        WFlash_SetRunMode(WFLASH_ROM_MODE);
        WFlashDummy(WFlash_GetRunMode()); 
    }
    else
    {
        ucRetValue = FLASH_RET_INVALID_PARA;
    }
    return ucRetValue;
}
/*!
 ******************************************************************************
 ** \brief erase flash in chip erase mode  
 **
 ** \param None
 ** \arg   None
 **     
 ** \return None     
 **
 ******************************************************************************
 */
void WFlash_ChipErase(void)
{
    uint32_t  AddrAA8,Addr554;
    AddrAA8 = CHIPERASE_ADDR_AA8; 
    Addr554 = CHIPERASE_ADDR_554; 
    
    /* CPU programming mode setting */

    WFlash_SetRunMode(WFLASH_PGM_MODE);
    WFlashDummy(WFlash_GetRunMode());    
    
    Flash_Write(AddrAA8, FLASH_CHIP_ERASE_1);
    Flash_Write(Addr554, FLASH_CHIP_ERASE_2);
    Flash_Write(AddrAA8, FLASH_CHIP_ERASE_3);
    Flash_Write(AddrAA8, FLASH_CHIP_ERASE_4);
    Flash_Write(Addr554, FLASH_CHIP_ERASE_5);
    Flash_Write(AddrAA8, FLASH_CHIP_ERASE_6);
    
    WFlash_WaitRdyState();

    /*  CPU ROM mode setting    */
    WFlash_SetRunMode(WFLASH_ROM_MODE);
    WFlashDummy(WFlash_GetRunMode()); 
}
/*!
 ******************************************************************************
 ** \brief suspend work flash erase  
 **
 ** \param pSecAddr
 ** \arg   operate address
 **     
 ** \return None
 **
 ******************************************************************************
 */
void WFlash_Suspend(uint32_t sec_addr)
{
    WFlash_SetRunMode(WFLASH_PGM_MODE);
    WFlashDummy(WFlash_GetRunMode()); 
    Flash_Write(sec_addr, FLASH_SECTOR_ERASE_SUSPEND);  
    WFlash_SetRunMode(WFLASH_ROM_MODE);
    WFlashDummy(WFlash_GetRunMode()); 
}
/*!
 ******************************************************************************
 ** \brief resume work flash erasing  
 **
 ** \param pSecAddr
 ** \arg   operate address
 **     
 ** \return None
 **
 ******************************************************************************
 */
void WFlash_Restart(uint32_t sec_addr)
{
    WFlash_SetRunMode(WFLASH_PGM_MODE);
    WFlashDummy(WFlash_GetRunMode()); 
    Flash_Write(sec_addr, FLASH_SECTOR_ERASE_RESTART);
    WFlash_SetRunMode(WFLASH_ROM_MODE);
    WFlashDummy(WFlash_GetRunMode()); 
}

#endif

