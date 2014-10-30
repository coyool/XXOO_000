#ifndef _DS3231_H_
#define _DS3231_H_
/** 
  * @brief  Ds3231 时钟芯片I2C总线初始化  
  */
void ds3231_Init(void);
/** 
  * @brief 读取Ds3231 时钟芯片的年月日周时分秒
  * @param 传出7个字节的数据:年月日周时分秒,每个字节都是BCD码
  */
void ds3231_read_ymdwhms(unsigned char *str);
/** 
  * @brief 更改Ds3231 时钟芯片的年月日周时分秒
  * @param 传入7个字节的数据:年月日周时分秒,每个字节都是BCD码
  */
void ds3231_write_ymdwhms(const unsigned char *str);

#define  read_ymdwhms ds3231_read_ymdwhms
#define  write_ymdwhms ds3231_write_ymdwhms

#endif
