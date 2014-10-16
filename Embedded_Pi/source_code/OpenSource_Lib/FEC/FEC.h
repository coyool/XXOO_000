#ifndef __FEC_H_
#define __FEC_H_


/*** define and type ***/
#define FEC_LIMIT_LEN   1024u


/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern u32 FEC_enCode(u8 *output, u8 *input, const u16 size);



#endif   /* end __FEC_H_ */



