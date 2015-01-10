#ifndef __APP_API_H
#define __APP_API_H

/*Export functions---------------------------------------------*/
uint16 API_GetDataValue(uint32 DataId, void *pDataBuf);
uint8 API_SetDataValue(uint32 DataId, void *pDataBuf, uint16 len);

#endif

