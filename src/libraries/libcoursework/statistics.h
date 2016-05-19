#pragma once

#include "imageData.h"
#include "libcoursework_api.h"

class LIBCOURSEWORK_API Statistics
{
public:
    static cw_double4 MSE(ImageData* a, ImageData* b);
    static cw_double4 PSNR(ImageData* a, ImageData* b);
};