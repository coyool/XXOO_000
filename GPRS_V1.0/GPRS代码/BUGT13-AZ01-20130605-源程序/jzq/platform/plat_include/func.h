
#ifndef __FUNC_H
#define __FUNC_H

#define OFSIZE(type,field)	sizeof(((type *)0)->field)
#define ARRAY_SIZE(a)	(sizeof(a)/sizeof(a[0]))

/**
 * u8类型转换成字符
 * @i: u8类型
 * @return 字符类型
*/
unsigned char hex_to_char(unsigned char i);

/**
 * 字符类型转换成u8
 * @i: u8类型
 * @return 字符类型
*/
unsigned char char_to_hex(unsigned char i);

unsigned char bcdtohex(unsigned char bcd);

unsigned char hextobcd(unsigned char hex);

unsigned int isbcd(const unsigned char *bcd,unsigned int n);


unsigned int string_to_hex(unsigned int *a,const unsigned char *str);

unsigned int string_to_bcd(unsigned int *a,const unsigned char *str);

#define swap(x, y) ({typeof(x) t=x; x=y; y=t;}) 
#endif
