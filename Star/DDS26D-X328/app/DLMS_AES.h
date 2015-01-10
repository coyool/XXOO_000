#ifndef __DLMS_AES_H
#define __DLMS_AES_H

/* Includes ------------------------------------------------------------------*/


/* Public  defines -----------------------------------------------------------*/
/* Public  functions ---------------------------------------------------------*/
extern void AES_Expandkey( uint8 *key, uint8 *expKey);
extern void AES_Encrypt( unsigned char *in, unsigned char *expKey, unsigned char *out);
extern void AES_Decrypt( unsigned char *in, unsigned char *expKey, unsigned char *out);

#endif
