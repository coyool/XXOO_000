#ifndef __IIC_H
#define __IIC_H


/*Export define------------------------------------------------*/
#define LOOP_DLY_1US            (0) 

#define SET_IIC_SCL()            (FM3_GPIO->DDR4&= ~ IO_PINxB)


#define CLR_IIC_SCL()            (FM3_GPIO->DDR4|= IO_PINxB)                                       


#define SET_IIC_SDA()             (FM3_GPIO->DDR4&= ~ IO_PINxA)                                   


#define CLR_IIC_SDA()             (FM3_GPIO->DDR4|= IO_PINxA)                                   

#define SET_IIC_SDA_INPUT()       (FM3_GPIO->DDR4&= ~ IO_PINxA)                    


#define SET_IIC_SDA_OUTPUT()      (FM3_GPIO->DDR4|= IO_PINxA)                  
	
#define IIC_SDA_GET()             (FM3_GPIO->PDIR4&= IO_PINxA)                                             


/*Export functions-------------------------------------------------*/
void IIcInit(void);                                      
void IIcPowerDownInit(void);
volatile void Dly1us(void);
void IIC_start(void);
void IIC_stop(void);
uint8 detect_sda(void);
uint8 IIC_receive400k(void);
uint8 IIC_send400k(uint8 data);
void IIC_Test_Task(void);
#endif

