#include "envi-hdr-parsing.h"
#include "envi-c-handling.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logs.h"

static bool InterpretData(ENVI_C_Handler_t *const handler, char *buff);
static bool ExtractValueFromKey(char *str, char *value);
static bool ExtractWavelenghtVal(ENVI_C_Handler_t *const handler, char *str);
static enum OBJ_EXTRACTION selectedObj = OBJ_NONE;

bool ENVI_ParseHDR(ENVI_C_Handler_t *const handler, FILE *const hdrStream)
{
    char buff[BUFF_LEN] = {0};
    while (fgets(buff, BUFF_LEN, hdrStream) != NULL)
    {
        InterpretData(handler, buff);
    }

    return true;
}

static bool InterpretData(ENVI_C_Handler_t *const handler, char *buff)
{
    char tempBuff[TEMP_BUFF_LEN] = {0};
    static size_t wavelenghtCount = 0;
    ENVI_C_Image_t *tempImg;

    switch (selectedObj)
    {
    case OBJ_NONE:
        break;

    // TODO: this can cause issues if this section for some reason comes BEFORE
    // the actual values. Or the handlers values are not yet allocated.
    // Not sure if that is possilbe, but yeah.
    case OBJ_WAVELENGHT:
        
        ExtractValueFromKey(buff, tempBuff);
        float temp;
        sscanf(tempBuff, "%f.2", &temp);
        if(wavelenghtCount < handler->bandsCount)
        {
            // handler->images[wavelenghtCount]->wavelenght = temp;
            tempImg = ENVI_ACCES_IMG_BUFF(handler, wavelenghtCount);
            tempImg->wavelenght = temp;
            tempImg->dataBuff = NULL;
            ++wavelenghtCount;
        }
        else
            selectedObj = OBJ_NONE;
        return true;

    case OBJ_DEFAULT_BANDS:
        ExtractValueFromKey(buff, tempBuff);
        size_t tempI;
        sscanf(tempBuff, "%zu", &tempI);
        if(handler->defaultBandCount < ENVI_DEFAULT_BAND_COUNT)
        {
            handler->defaultBands[handler->defaultBandCount] = tempI;
            handler->defaultBandCount++;
        }
        return true;
    }

    if (strncmp(buff, ENVI_FILE_START, sizeof(ENVI_FILE_START) - 1) == 0)
    {
        LOG_INFO("ENVI header found!");
        return true;
    }

    if (strncmp(buff, SAMLPLES_COUNT_KEY, sizeof(SAMLPLES_COUNT_KEY) - 1) == 0)
    {
        ExtractValueFromKey(buff + sizeof(SAMLPLES_COUNT_KEY), tempBuff);
        sscanf(tempBuff, "%zu", &handler->samplesCount);
        LOG_INFO("Found %zu samples!", handler->samplesCount);
        return true;
    }

    if (strncmp(buff, LINES_COUNT_KEY, sizeof(LINES_COUNT_KEY) - 1) == 0)
    {
        ExtractValueFromKey(buff + sizeof(LINES_COUNT_KEY), tempBuff);
        sscanf(tempBuff, "%zu", &handler->linesCount);
        LOG_INFO("Found %zu lines!", handler->linesCount);
        return true;
    }

    if (strncmp(buff, BANDS_KEY, sizeof(BANDS_KEY) - 1) == 0)
    {
        ExtractValueFromKey(buff + sizeof(BANDS_KEY), tempBuff);
        sscanf(tempBuff, "%zu", &handler->bandsCount);
        // TODO: error checking here!
        handler->images = calloc(handler->bandsCount, sizeof(ENVI_C_Image_t));

        // for (size_t i = 0; i < handler->bandsCount; ++i)
        // {
        //     handler->images[i] = (ENVI_C_Image_t*)calloc(1, sizeof(ENVI_C_Image_t));
        // }
        return true;
    }

    if (strncmp(buff, DATA_TYPE_KEY, sizeof(DATA_TYPE_KEY) - 1) == 0)
    {
        ExtractValueFromKey(buff + sizeof(DATA_TYPE_KEY), tempBuff);
        switch (tempBuff[0])
        {
        case '4':
            handler->dataType = ENVI_C_DATA_TYPE_F32;
            break;

        default:
            handler->dataType = ENVI_C_DATA_UNKNOWN;
            return false;
        }

        return true;
    }

    if (strncmp(buff, DATA_INTERLIEVE_KEY, sizeof(DATA_INTERLIEVE_KEY) - 1) == 0)
    {
        ExtractValueFromKey(buff + sizeof(DATA_INTERLIEVE_KEY), tempBuff);
        if (strncmp(tempBuff, DATA_INTERLIEVE_BIL, sizeof(DATA_INTERLIEVE_BIL)) == 0)
        {
            handler->dataInterlieving = ENVI_C_DATA_BIL;
            return true;
        }

        if (strncmp(tempBuff, DATA_INTERLIEVE_BIP, sizeof(DATA_INTERLIEVE_BIP)) == 0)
        {
            handler->dataInterlieving = ENVI_C_DATA_BIP;
            return true;
        }

        if (strncmp(tempBuff, DATA_INTERLIEVE_BSQ, sizeof(DATA_INTERLIEVE_BSQ)) == 0)
        {
            handler->dataInterlieving = ENVI_C_DATA_BSQ;
            return true;
        }
    }

    if (strncmp(buff, WAVELENGHT_KEY, sizeof(WAVELENGHT_KEY) - 1) == 0)
    {
        selectedObj = OBJ_WAVELENGHT;
        return true;
    }

    if (strncmp(buff, DATA_ENDIANES_KEY, sizeof(DATA_ENDIANES_KEY) - 1) == 0)
    {
        ExtractValueFromKey(buff + sizeof(DATA_ENDIANES_KEY), tempBuff);
        sscanf(tempBuff, "%zu", &handler->dataEndianes);
        return true;
    }

    if (strncmp(buff, DEFAULT_BANDS_KEY, sizeof(DEFAULT_BANDS_KEY) - 1) == 0)
    {
        selectedObj = OBJ_DEFAULT_BANDS;
        return true;
    }

    return false;
}

static bool ExtractValueFromKey(char *str, char *value)
{
    size_t count = 0;
    for (char ch = *str; ch != '\0'; ch = *++str)
    {
        switch (ch)
        {
        case ' ':
        case ',':
        case '=':
        case '{':
            break;
        case '}':
            selectedObj = OBJ_NONE;
            break;

        case '\n':
            return true;

        default:
            value[count] = ch;
            if (count < TEMP_BUFF_LEN)
                ++count;
            else
                return true;
        }
    }

    return false;
}
