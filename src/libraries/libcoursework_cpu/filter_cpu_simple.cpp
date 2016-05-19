#include "filter_cpu.h"
#include "imageData.h"

#include <algorithm> //std::sort
#include <cassert>   //assert
#include <cstring>   //memcpy

#include "config.h"
#include "utils.h"

void filter_cpu_simple_channel(uint8_t* data, unsigned int w, unsigned int h, unsigned int windowSize)
{
    unsigned const int w_edge = windowSize / 2;
    unsigned const int h_edge = windowSize / 2;

    uint8_t* window = new uint8_t[windowSize*windowSize];
    uint8_t* data_out = new uint8_t[w*h];

    //Обработка
    for (int y = h_edge; y < (h - h_edge); y++)
    {      
        for (unsigned int x = w_edge; x < (w - w_edge); x++)
        {
            //Заполнение окна
            for (unsigned int window_y = 0; window_y < windowSize; window_y++)
            {
                for (unsigned int window_x = 0; window_x < windowSize; window_x++)
                {
                    window[window_y*windowSize + window_x] = data[(y - h_edge + window_y)*w + (x - w_edge + window_x)];
                }
            }

            //сортировка и запись
            std::nth_element(window, window + windowSize*windowSize / 2, window + windowSize*windowSize);
            data_out[y*w + x] = window[(windowSize * windowSize - 1) / 2];
        }
    }

    //возврат назад
    memcpy(data,data_out,w*h*sizeof(uint8_t));

    //чистка
    delete[] data_out;
    delete[] window;
}

void filter_cpu_simple(ImageData* imageData, const unsigned int windowSize){
    assert(windowSize % 2 != 0); //windowSize must be odd number

    ImageData* imageDataExtended = image_extend(imageData,windowSize);
    //запуск обработки канала
    if (imageData->isColor)
    {
        #ifdef ENABLE_FEATURE_OPENMP

        #pragma omp parallel for num_threads(ImageData::CHANNEL_COUNT_RGB)
        for (int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            filter_cpu_simple_channel(imageDataExtended->data_uint8[i], imageDataExtended->width, imageDataExtended->height, windowSize);
        }

        #else
        for (int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            filter_cpu_simple_channel(imageDataExtended->data_uint8[i], imageDataExtended->width, imageDataExtended->height, windowSize);
        }
        #endif
    }
    else
    {
        filter_cpu_simple_channel(imageData->data_uint8[3], imageData->width, imageData->height, windowSize);
    }

    image_placeExtendedBack(imageData,imageDataExtended,windowSize);
    delete imageDataExtended;
}
