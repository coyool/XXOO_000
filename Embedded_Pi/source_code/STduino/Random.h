#ifndef __RANDOM_H_
#define __RANDOM_H_


/*** define and type ***/



/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void randomSeed(unsigned int seed);
extern int random(u32 howbig);
extern int randomRange(u32 howsmall, u32 howbig);



#endif   /* end __RANDOM_H_ */


