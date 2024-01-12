#include "envi-c-handling.h"
#include "logs.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using cv::Mat;
using std::cout;
using std::vector;

int main(void)
{
    cout << "Hello world!\n";
    vector<Mat> hsiPictures;

    auto enviHandler = ENVI_OpenDataFile("path/to/hdr/file", "path/to/data/file");
    if (enviHandler == nullptr)
    {
        LOG_ERROR("Failed to open HSI data!");
        std::exit(1);
    }

    ENVI_ReadDataFile(enviHandler);
    hsiPictures.reserve(enviHandler->bandsCount);
    for (size_t i = 0; i < enviHandler->bandsCount; ++i)
    {
        auto hsiImg = ENVI_GetCapture(enviHandler, i);
        if (hsiImg == nullptr)
            continue;

        Mat tempImg(cv::Size(enviHandler->samplesCount, enviHandler->linesCount), CV_32FC1);
        std::memcpy(tempImg.data, hsiImg->dataBuff, hsiImg->allocSize);
        hsiPictures.push_back(tempImg);
        tempImg.release();
    }

    ENVI_CleanupHandler(&enviHandler);

    for (auto &img : hsiPictures)
    {
        cv::imshow("HSI picture", img);
        cv::waitKey(0);
    }

    for (auto &img : hsiPictures)
    {
        img.release();
    }

    cv::destroyAllWindows();
    return 0;
}
