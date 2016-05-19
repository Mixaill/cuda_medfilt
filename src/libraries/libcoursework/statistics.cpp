#include "statistics.h"

#include <cmath>

cw_double4 Statistics::MSE(ImageData* a, ImageData* b){
    cw_double4 mse;
    mse.r = -1;
    mse.g = -1;
    mse.b = -1;
    mse.a = -1;

    if ((a->height != b->height)||(a->width != b->width))
    {
        return mse;
    }

    unsigned int w = a->width;
    unsigned int h = a->height;

    for (unsigned int y = 0; y < h; y++)
    {
        for (unsigned int x = 0; x < w; x++)
        {
            mse.r = mse.r + pow(std::abs(a->data_uint8[0][y*w + x] - b->data_uint8[0][y*w + x]), 2);
            mse.g = mse.g + pow(std::abs(a->data_uint8[1][y*w + x] - b->data_uint8[1][y*w + x]), 2);
            mse.b = mse.b + pow(std::abs(a->data_uint8[2][y*w + x] - b->data_uint8[2][y*w + x]), 2);
            mse.a = mse.a + pow(std::abs(a->data_uint8[3][y*w + x] - b->data_uint8[3][y*w + x]), 2);
        }
    }
    
    mse.r = mse.r  / (double)(w*h);
    mse.g = mse.g  / (double)(w*h);
    mse.b = mse.b  / (double)(w*h);
    mse.a = mse.a  / (double)(w*h);

    return mse;
}

cw_double4 Statistics::PSNR(ImageData* a,ImageData* b){
    cw_double4 psnr;

    cw_double4 mse = MSE(a, b);

    psnr.r = 10 * log10(255 * 255 / mse.r);
    psnr.g = 10 * log10(255 * 255 / mse.g);
    psnr.b = 10 * log10(255 * 255 / mse.b);
    psnr.a = 10 * log10(255 * 255 / mse.a);

    return psnr;
}
