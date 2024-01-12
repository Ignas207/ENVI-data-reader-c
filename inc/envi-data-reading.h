#ifndef ENVI_C_DATA_READING_H
#define ENVI_C_DATA_READING_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "envi-c-handling.h"

bool ENVI_ReadBIL(ENVI_C_Handler_t *const handler, uint8_t *buff, size_t captureNo);

#ifdef __cplusplus
}
#endif


#endif
