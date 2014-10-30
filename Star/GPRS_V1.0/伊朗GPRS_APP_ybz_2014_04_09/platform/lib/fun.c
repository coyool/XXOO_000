#include "plat_include/func.h"
						 								  /**
 * u8类型转换成字符
 * @i: u8类型
 * @return 字符类型
*/
unsigned char hex_to_char(unsigned char i)
{
	if(i<10)
		i = '0'+i;
	else if((i>=0x0a)&&(i<=0x0f))
		i = 'A' + (i-0x0a);
	return i;
}

/**
 * 字符类型转换成u8
 * @i: u8类型
 * @return 字符类型
*/
unsigned char char_to_hex(unsigned char i)
{
  	if((i>='0')&&(i<='9'))
		i -= '0';
	else if((i>='a')&&(i<='f'))
		i -= 'a';
	else if((i>='A')&&(i<='F'))
		i -= 'A';
	return i;	 
}

unsigned char bcdtohex(unsigned char bcd)
{
	return (bcd>>4)*10+(bcd&0x0f);
}

unsigned char hextobcd(unsigned char hex)
{
	return (((hex/10)<<4)|(hex%10));
}
/*
inline void nbcdtohex(unsigned char *hex,const unsigned char *bcd,unsigned int n)
{
	while(n--){
		*hex++ = bcdtohex(*bcd++);
	}
}*/

unsigned int isbcd(const unsigned char *bcd,unsigned int n)
{
	while(n--){
		if(((*bcd&0x0f)>0x09)||((*bcd&0xf0)>0x90))
			return 0;
		bcd++;
	}
	return 1;
}


unsigned int string_to_hex(unsigned int *a,const unsigned char *str)
{
	unsigned int i,j;
	*a=0;
	j=0;
	for(i=0;i<8;i++){
		if(!((*str>='0')&&(*str<='9')))
			break;
		*a *= 10;
		*a += char_to_hex(*str);
		str++;
		j++;
	}
	return j;
}
unsigned int string_to_bcd(unsigned int *a,const unsigned char *str)
{
	unsigned int i,j;
	*a=0;
	j=0;
	for(i=0;i<8;i++){
		if(!((*str>='0')&&(*str<='9')))
			break;
		*a <<= 4;
		*a += char_to_hex(*str);
		str++;
		j++;
	}
	return j;	
}
