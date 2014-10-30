#ifndef __BUTTON_H_
#define __BUTTON_H_


/*** define and type ***/
#define   button_key     bFM3_GPIO_PDIR3_PE

typedef enum 
{  
    NO_KEY = 0,
    CLICK  = 1,
    LONG   = 2
}Button_KEY_TypeDef; 

/*** extern variable declarations ***/



/*** extern function prototype declarations ***/
extern void BUTTON_KEY_setup(void);


#endif   /* end __BUTTON_H_ */


