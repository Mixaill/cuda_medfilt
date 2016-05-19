#include "imageData.h"
#include "filter_arrayfire.h"
#include <arrayfire.h>

void filter_arrayfire_init(){
	af::info();
}

void filter_arrayfire_medfilt(ImageData* imageData, unsigned int windowSize){

	//запуск обработки канала
    if (imageData->isColor)
    {
        #ifdef ENABLE_FEATURE_OPENMP

        #pragma omp parallel for num_threads(ImageData::CHANNEL_COUNT_RGB)
        for (int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            af::array arr_in(imageData->width, imageData->height, imageData->data_uint8[i]);
            af::array arr_out=af::medfilt(arr_in, windowSize, windowSize, AF_PAD_SYM);
            arr_out.host(imageData->data_uint8[i]);
        }

        #else
        for (int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            af::array arr_in(imageData->width, imageData->height, imageData->data_uint8[i]);
            af::array arr_out=af::medfilt(arr_in, windowSize, windowSize, AF_PAD_SYM);
            arr_out.host(imageData->data_uint8[i]);
        }
        #endif
    }
    else
    {
        af::array arr_in(imageData->width, imageData->height, imageData->data_uint8[3]);
        af::array arr_out = af::medfilt(arr_in, windowSize, windowSize, AF_PAD_SYM);
        arr_out.host(imageData->data_uint8[3]);
    }
}
