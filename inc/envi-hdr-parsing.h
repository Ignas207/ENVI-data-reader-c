/**
 * @file envi-c-handling.h
 * @author Ignas207 (https://github.com/Ignas207/ENVI-data-reader-c)
 * @brief ENVI file reader HDR parsing
 * @version 1.0
 * @date 2024-01-12
 * 
 * @copyright Copyright (c) 2024 MIT License
 * 
 */
#ifndef ENVI_C_HDR_PARSING_H
#define ENVI_C_HDR_PARSING_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "envi-c-handling.h"

#define BUFF_LEN 512U
#define TEMP_BUFF_LEN 64U

#define ENVI_FILE_START "ENVI"
#define SAMLPLES_COUNT_KEY "samples"
#define LINES_COUNT_KEY "lines"
#define BANDS_KEY "bands"
#define DATA_TYPE_KEY "data type"
#define DATA_INTERLIEVE_KEY "interleave"
#define DATA_INTERLIEVE_BIL "BIL"
#define DATA_INTERLIEVE_BSQ "BSQ"
#define DATA_INTERLIEVE_BIP "BIP"
#define DATA_ENDIANES_KEY "byte order"
#define DEFAULT_BANDS_KEY "default bands"
#define WAVELENGHT_KEY "wavelength"

enum OBJ_EXTRACTION
{
    OBJ_NONE,
    OBJ_WAVELENGHT = 1,
    OBJ_DEFAULT_BANDS,
};

bool ENVI_ParseHDR(ENVI_C_Handler_t *const handler, FILE *const hdrStream);

#ifdef __cplusplus
}
#endif

#endif
