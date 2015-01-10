/*****************************************************************************************
*  	Function:  
*   
*    Author:	冯秋雄
*    Created on:	2013-09-12
*****************************************************************************************/
#include "Include.h"


void IIC_stop(void);
/*****************************************************************************************
*   Dly1us
*   Function : delay time 3us
*   Action   : 
*	Input    : None
*	Output   : None
*	Return   : None
*****************************************************************************************/
volatile void Dly1us(void)
{
    uint32 i =0;

    for(i = LOOP_DLY_1US;  i > 0; i--)
    {
        ;
    }
}

/*****************************************************************************************
*   IIC_send400k
*   Function : IIC send data at speed of 400k
*   Action   : 
*	Input    : data - the data which needed to send
*	Output   : None
*	Return   : fail--FALSE success--TRUE 
*****************************************************************************************/
uint8 IIC_send400k(uint8 data)
{
    uint16 i;
    uint8 tmp;

    for(i = 8; i > 0; i--)
    {
        CLR_IIC_SCL();

        if(data & 0x80)
        {
            SET_IIC_SDA();
        }
        else
        {
            CLR_IIC_SDA();
        }
        data <<= 1;
        //Dly1us();        
        SET_IIC_SCL();
        //Dly1us();
    }

    CLR_IIC_SCL();
    //Dly1us();
    SET_IIC_SDA();
    SET_IIC_SDA_INPUT();
    SET_IIC_SCL();
    //Dly1us();

    tmp = IIC_SDA_GET() ?  FALSE:TRUE ; //(GPIOB->IDR & IICSDA_PIN)

    //Dly1us();
    CLR_IIC_SCL();
    SET_IIC_SDA_OUTPUT();

    return tmp;
}

/*****************************************************************************************
*   IIC_receive400k
*   Function : IIC receive data at speed of 400k
*   Action   : 
*	Input    : None
*	Output   : None
*	Return   : the data received
*****************************************************************************************/
uint8 IIC_receive400k(void)
{
    uint8 i, tmp = 0;

    SET_IIC_SDA();
    SET_IIC_SDA_INPUT();
    for(i = 8; i > 0; i--)
    {
        SET_IIC_SCL();
        tmp <<= 1;
        //Dly1us();

        if(IIC_SDA_GET())//(GPIOB->IDR & IICSDA_PIN)
        {
            tmp++;
        }
        CLR_IIC_SCL();
        //Dly1us();
    }
    SET_IIC_SDA_OUTPUT();
    //Dly1us();

    return tmp;
}

void IIcInit(void)
{
   IO_DisableFunc(IO_PORT4,IO_PINxA);
   IO_DisableFunc(IO_PORT4,IO_PINxB);
   IO_ConfigGPIOPin(IO_PORT4,IO_PINxA,IO_DIR_INPUT,IO_PULLUP_OFF);
   IO_ConfigGPIOPin(IO_PORT4,IO_PINxB,IO_DIR_INPUT,IO_PULLUP_OFF);
   IO_WriteGPIOPin(IO_PORT4,IO_PINxA,IO_BIT_CLR);
   IO_WriteGPIOPin(IO_PORT4,IO_PINxB,IO_BIT_CLR);
 
   IIC_stop();
}

void IIcPowerDownInit(void)
{
    IO_DisableFunc(IO_PORT4,IO_PINxA);
    IO_DisableFunc(IO_PORT4,IO_PINxB);
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxA,IO_DIR_INPUT,IO_PULLUP_OFF);
    IO_ConfigGPIOPin(IO_PORT4,IO_PINxB,IO_DIR_INPUT,IO_PULLUP_OFF);
}
/*****************************************************************************************
*   IIC_start
*   Function : Init the start configuration of IIC
*   Action   : 
*	Input    : None
*	Output   : None
*	Return   : None
*****************************************************************************************/
void IIC_start(void)
{
    CLR_IIC_SCL();
    //Dly1us();

    SET_IIC_SDA_OUTPUT();
    SET_IIC_SDA();
    //Dly1us();

    /* start condition */
    SET_IIC_SCL();
    //Dly1us();

    CLR_IIC_SDA();
    //Dly1us();

    CLR_IIC_SCL();
}

/*****************************************************************************************
*   IIC_start
*   Function : configure the scl and sda Of the IIC
*   Action   : 
*	Input    : None
*	Output   : None
*	Return   : None
*****************************************************************************************/
void IIC_stop(void)
{
    CLR_IIC_SCL();
    //Dly1us();

    /* stop condition */
    SET_IIC_SDA_OUTPUT();
    CLR_IIC_SDA();
    //Dly1us();

    SET_IIC_SCL();
    //Dly1us();

    SET_IIC_SDA();
    //Dly1us();
    //CLR_IIC_SCL();
  
}

/*****************************************************************************************
*   detect_sda
*   Function : wait the needed state of SDA
*   Action   : 
*	Input    : None
*	Output   : None
*	Return   : TRUE--success  FALSE--fail 
*****************************************************************************************/
uint8 detect_sda(void)
{
    uint16 temp0;

    IIC_stop();
    SET_IIC_SDA();
    SET_IIC_SDA_INPUT();
    for(temp0 = 16; temp0 > 0; temp0--)
    {
        if(!IIC_SDA_GET())
        {
            break;
        }        
        //Dly1us();
    }
    SET_IIC_SDA_OUTPUT();
    if (temp0 != 0)           // If sda = low, disturb
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void IIC_Test_Task(void)
{
   IIC_start();
   IIC_send400k(0xaa);
   IIC_receive400k();
   IIC_stop();
}


