#ifndef __RANDOM_H_
#define __RANDOM_H_


/*** define and type ***/



/*** extern variable declarations ***/
extern __IO u32 randomSeedValue; 
extern __IO u32 randomValue;

extern u32 randomSeedValue4bit;

/*** extern function prototype declarations ***/
extern void randomSeed(unsigned int seed);
extern u32 random(u32 howbig);
extern u32 randomRange(u32 howsmall, u32 howbig);
extern void randomTest(void);


#endif   /* end __RANDOM_H_ */


