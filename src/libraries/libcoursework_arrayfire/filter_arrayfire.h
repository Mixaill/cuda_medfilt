#pragma once 

#include "imageData.h"
#include "libcoursework_arrayfire_api.h"

LIBCOURSEWORK_ARRAYFIRE_API void filter_arrayfire_init();
LIBCOURSEWORK_ARRAYFIRE_API void filter_arrayfire_medfilt(ImageData* imageData, unsigned int windowSize);
