/**
 * @file envi-c-handling.h
 * @author Ignas207 (https://github.com/Ignas207/ENVI-data-reader-c)
 * @brief ENVI file reader binary data parsing
 * @version 1.0
 * @date 2024-01-12
 * 
 * @copyright Copyright (c) 2024 MIT License
 * 
 */
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
