#include "envi-data-reading.h"
#include "envi-hdr-parsing.h"
#include <float.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "logs.h"

static inline bool OpenFile(char *dir, FILE **stream);

ENVI_C_Handler_t *ENVI_OpenDataFile(const char *hdrPath, const char *dataPath)
{
    if (hdrPath == NULL)
    {
        LOG_ERROR("HDR file path is NULL!");
        return NULL;
    }
    if (dataPath == NULL)
    {
        LOG_ERROR("Data file path is NULL!");
        return NULL;
    }

    ENVI_C_Handler_t *handler = (ENVI_C_Handler_t *)calloc(1, sizeof(ENVI_C_Handler_t));
    if (handler == NULL)
    {
        LOG_ERROR("Failed to allocate memory for handler!");
        return NULL;
    }

    FILE *hdrFile = NULL;
    if (!OpenFile(hdrPath, &hdrFile))
    {
        LOG_ERROR("Failed to open HDR info: %s", hdrPath);
        ENVI_CleanupHandler(&handler);
        return false;
    }

    if (!OpenFile(dataPath, &handler->dataStream))
    {
        LOG_ERROR("Failed to open data file: %s", dataPath);
        ENVI_CleanupHandler(&handler);
        fclose(hdrFile);
        return false;
    }

    ENVI_ParseHDR(handler, hdrFile);
    fclose(hdrFile);
    return handler;
}

bool ENVI_ReadDataFile(ENVI_C_Handler_t *const handler)
{
    if (handler->dataStream == NULL)
        return false;

    size_t imgSize = handler->linesCount * handler->samplesCount;
    switch (handler->dataType)
    {
        case ENVI_C_DATA_TYPE_F32:
            imgSize *= sizeof(float);
            break;

        default:
            break;
    }

    // Allocating the image buffers
    for (size_t i = 0; i < handler->bandsCount; ++i)
    {
        if (ENVI_ACCES_IMG_BUFF(handler, i)->allocSize != 0)
            continue;

        void *temp = calloc(1, imgSize);
        if (temp != NULL)
        {
            ENVI_ACCES_IMG_BUFF(handler, i)->dataBuff = temp;
            ENVI_ACCES_IMG_BUFF(handler, i)->allocSize = imgSize;
        }
        else
        {
            ENVI_ACCES_IMG_BUFF(handler, i)->allocSize = 0;
        }
    }

    imgSize = handler->linesCount * handler->bandsCount;
    switch (handler->dataType)
    {
        case ENVI_C_DATA_TYPE_F32:
            imgSize *= sizeof(float);
            break;
            
        default:
            break;
    }

    // Writing to the image buffers according to the ENVI file type.
    size_t rowAmount = handler->samplesCount;
    uint8_t *captureBuff = calloc(1, imgSize);

    for (size_t i = 0; i < rowAmount; ++i)
    {
        memset(captureBuff, 0, imgSize);
        size_t ret = fread(captureBuff, sizeof(uint8_t), imgSize, handler->dataStream);
        if (ret != imgSize)
        {
            LOG_ERROR("Ret is not imgSize: %u", ret);
        }

        switch (handler->dataInterlieving)
        {
        case ENVI_C_DATA_BIL:
            ENVI_ReadBIL(handler, captureBuff, i);
            break;
        case ENVI_C_DATA_BSQ:
            LOG_ERROR("BSQ not implemented!");
            break;
        case ENVI_C_DATA_BIP:
            LOG_ERROR("BIP not implemented!");
            break;
        }
    }

    free(captureBuff);
    return true;
}

ENVI_C_Image_t *ENVI_GetCapture(ENVI_C_Handler_t *const handler, size_t captureNumber)
{
    ENVI_C_Image_t *temp = NULL;

    if (captureNumber < handler->bandsCount)
        temp = ENVI_ACCES_IMG_BUFF(handler, captureNumber);

    return temp;
}

bool ENVI_CleanupHandler(ENVI_C_Handler_t **const handler)
{
    if ((handler == NULL) || (*handler == NULL))
        return false;

    ENVI_C_Handler_t *tempHandler = *handler;

    if (tempHandler->dataStream != NULL)
    {
        fclose(tempHandler->dataStream);
        tempHandler->dataStream = NULL;
    }

    // Freeing the image buffers
    ENVI_C_Image_t *tempImg;
    for (size_t i = 0; i < tempHandler->bandsCount; ++i)
    {
        tempImg = ENVI_ACCES_IMG_BUFF(tempHandler, i);
        // tempImg = tempHandler->images[i];
        if (tempImg == NULL)
            continue;

        // Freeing image buffer
        if (tempImg->dataBuff != NULL)
            free(tempImg->dataBuff);
    }

    // Freeing the image handler
    free(tempHandler->images);

    // Freeing the main handler
    free(*handler);
    *handler = NULL;
    return true;
}

static inline bool OpenFile(char *dir, FILE **stream)
{
    *stream = fopen(dir, "rb");
    if (*stream != NULL)
        return true;

    LOG_ERROR("Failed to open file %s", dir);
    return false;
}
