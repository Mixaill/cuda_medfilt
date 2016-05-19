#include <cstdint>
#include <cstring>

#include "imageData.h"

ImageData::ImageData(unsigned int w, unsigned int h)
{
    width = w;
    height = h;


    data_uint8 = new uint8_t*[CHANNEL_COUNT_RGBA];
    for (unsigned int i = 0; i < CHANNEL_COUNT_RGBA; i++)
    {
        data_uint8[i] = new uint8_t[width*height];
    }

    data_cwuchar4 = nullptr;
}

ImageData::ImageData(unsigned int w, unsigned int h, bool color)
{
    width = w;
    height = h;
    isColor = color;

    data_uint8 = new uint8_t*[CHANNEL_COUNT_RGBA];
    for (unsigned int i = 0; i < CHANNEL_COUNT_RGBA; i++)
    {
        data_uint8[i] = new uint8_t[width*height];
    }

    data_cwuchar4 = nullptr;
}

ImageData::~ImageData(){
    for (unsigned int i = 0; i < CHANNEL_COUNT_RGBA; i++)
    {
        delete[] data_uint8[i];
    }
    delete[] data_uint8;

    if (data_cwuchar4 != nullptr)
    {
        delete[] data_cwuchar4;
    }
}

void ImageData::GenerateCWUchar4()
{
    if (data_uint8==nullptr)
    {
        return;
    }

    if (data_cwuchar4 == nullptr)
    {
        data_cwuchar4 = new cw_uchar4[width*height];
    }

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            for (unsigned int k = 0; k < CHANNEL_COUNT_RGBA; k++)
            {
                data_cwuchar4[y*width + x].arr[k] = data_uint8[k][y*width + x];
            }
        }
    }
}

void ImageData::toGrayscale()
{
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            data_uint8[3][y*width + x] = (unsigned char)(  0.11*data_uint8[0][y*width + x] 
                                                         + 0.59*data_uint8[1][y*width + x] 
                                                         + 0.30*data_uint8[2][y*width + x]);
        }
    }
    isColor = false;
}

void ImageData::toOneChannel(unsigned int remaining_channel)
{
    if (remaining_channel > 3)
    {
        return;
    }

    if (remaining_channel == 3)
    {
        isColor = false;
        return;
    }

    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            data_uint8[3][y*width + x] = data_uint8[remaining_channel][y*width + x];
        }
    }

    isColor = false;
}
