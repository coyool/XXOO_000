#ifndef __PROJECT_H
#define __PROJECT_H

typedef signed long long int64;
typedef signed long  int32;
typedef signed short int16;
typedef signed char  int8;

typedef volatile signed long  vint32;
typedef volatile signed short vint16;
typedef volatile signed char  vint8;

typedef unsigned long long uint64;
typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

typedef volatile unsigned long  vuint32;
typedef volatile unsigned short vuint16;
typedef volatile unsigned char  vuint8;



//typedef enum {FALSE = 0, TRUE = !FALSE}BOOL_B;
typedef enum {false = 0, true = !false}bool;

#ifndef FALSE
#define FALSE 	false
#endif
#ifndef TRUE
#define TRUE	true
#endif

#define BOOL_B 				bool

#endif

