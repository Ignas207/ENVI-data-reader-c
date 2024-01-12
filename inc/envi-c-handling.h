/**
 * @file envi-c-handling.h
 * @author Ignas207 (https://github.com/Ignas207/ENVI-data-reader-c)
 * @brief ENVI file reader user functions
 * @version 1.0
 * @date 2024-01-12
 * 
 * @copyright Copyright (c) 2024 MIT License
 * 
 */
#ifndef ENVI_C_DATA_HANDLING_H
#define ENVI_C_DATA_HANDLING_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ENVI_DEFAULT_BAND_COUNT 3U

/**
 * @brief More info here: https://envi.geoscene.cn/help/Subsystems/envi/Content/ExploreImagery/ENVIHeaderFiles.htm
 * 
 */
enum ENVI_C_DATA_TYPE
{
    ENVI_C_DATA_UNKNOWN,
    ENVI_C_DATA_TYPE_F32,
};

/**
    * @brief Info can be found here:
    * https://envi.geoscene.cn/help/Subsystems/envi/Content/ExploreImagery/ENVIImageFiles.htm
    *
    */
enum ENVI_C_DATA_INTERLIEVING_TYPE
{
    ENVI_C_DATA_BIL,
    ENVI_C_DATA_BSQ,
    ENVI_C_DATA_BIP,
};

enum ENVI_C_DATA_ENDIANES
{
    ENVI_C_BIG_ENDIAN,
    ENVI_C_LITTLE_ENDIAN,
};

typedef struct
{
    float wavelenght;
    uint32_t allocSize;
    size_t writtenPixels;
    // TODO: in the future make this an opaque type!
    float *dataBuff;
} ENVI_C_Image_t;

typedef struct
{
    size_t samplesCount;
    size_t linesCount;
    size_t bandsCount;
    enum ENVI_C_DATA_TYPE dataType;
    enum ENVI_C_DATA_INTERLIEVING_TYPE dataInterlieving;
    enum ENVI_C_DATA_ENDIANES dataEndianes;
    uint32_t defaultBandCount;
    size_t defaultBands[ENVI_DEFAULT_BAND_COUNT];
    FILE *dataStream;
    ENVI_C_Image_t *images;
} ENVI_C_Handler_t;

#define ENVI_ACCES_IMG_BUFF(handlerPtr, location) ((handlerPtr->images + (location)))

/**
 * @brief Opens the ENVI data file and configures the handler with the HDR file.
 * 
 * @param hdrPath HDR file path
 * @param dataPath Binary data file
 * @return ENVI_C_Handler_t* Handler for the parser if succesfull, else NULL.
 */
ENVI_C_Handler_t *ENVI_OpenDataFile(const char *hdrPath, const char *dataPath);

/**
 * @brief Get a specified HSI image.
 * 
 * @param handler 
 * @param captureNumber Image number we want to get.
 * @return ENVI_C_Image_t* 
 */
ENVI_C_Image_t *ENVI_GetCapture(ENVI_C_Handler_t *const handler, size_t captureNumber);

/**
 * @brief Destroys the handler and frees the memory.
 * 
 * @param handler 
 * @return true 
 * @return false 
 */
bool ENVI_CleanupHandler(ENVI_C_Handler_t **const handler);

/**
 * @brief Reads and parses the data from the binary file.
 * 
 * @param handler 
 * @return true Parsing succesfull
 * @return false Parsing failled
 */
bool ENVI_ReadDataFile(ENVI_C_Handler_t *const handler);

#ifdef __cplusplus
}
#endif

#endif
