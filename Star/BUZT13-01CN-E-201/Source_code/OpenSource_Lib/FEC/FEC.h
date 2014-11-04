#ifndef __FEC_H_
#define __FEC_H_


/*** define and type ***/
#define FEC_LIMIT_LEN   1024u
#define  A7139_onTheAir_LIMIT_SIZE 64u
#define min(a,b)    (a <= b ? a : b)

/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern u32 FEC_enCode(u8 *output, u8 *input, u16 size);
extern u32 FEC_deCode(u8 *output, const u8 *input, u32 size);
extern void FEC_test(void);

#endif   /* end __FEC_H_ */



