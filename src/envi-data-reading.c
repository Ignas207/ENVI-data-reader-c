#include "envi-data-reading.h"
#include "envi-c-handling.h"

bool ENVI_ReadBIL(ENVI_C_Handler_t *const handler, uint8_t *buff, size_t captureNo)
{
    size_t buffSize = handler->linesCount * handler->bandsCount * handler->dataType;
    uint32_t tempVal = 0;
    size_t currentChannel = 0;
    ENVI_C_Image_t *tempImg = ENVI_ACCES_IMG_BUFF(handler, currentChannel);
    size_t pixelCount = tempImg->writtenPixels;

    for (size_t i = 0; i < buffSize; ++i)
    {

        // if(tempImg->allocSize == 0)
        // continue;

        // Casting to the appropriate type
        switch (handler->dataType)
        {
        case ENVI_C_DATA_TYPE_F32:

            if(handler->dataEndianes == ENVI_C_BIG_ENDIAN)
            {
                // Constructing the F32 value
                tempVal = buff[i] + (buff[i + 1] << 8) + (buff[i + 2] << 16) + (buff[i + 3] << 24);
            }
            else
                tempVal = (buff[i] << 24) + (buff[i + 1] << 16) + (buff[i + 2] << 8) + buff[i + 3];
            
            i += 3;
            tempImg->dataBuff[pixelCount] = *(float *)&tempVal;
            break;
        default:
            break;
        }

        // Incrementing the pixel location.
        pixelCount++;

        // Checking if we need to switch to the next band.
        if ((pixelCount % handler->samplesCount) == 0)
        {
            // Save the amount of pixels written
            tempImg->writtenPixels = pixelCount;
            if(currentChannel > handler->bandsCount)
                break;

            currentChannel++;
            tempImg = ENVI_ACCES_IMG_BUFF(handler, currentChannel);
            pixelCount = tempImg->writtenPixels;
        }
    }

    return true;
}
