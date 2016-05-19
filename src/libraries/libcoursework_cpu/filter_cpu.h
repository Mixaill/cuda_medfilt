#pragma once 

#include "imageData.h"
#include "libcoursework_cpu_api.h"

LIBCOURSEWORK_CPU_API void filter_cpu_simple(ImageData* imageData, const unsigned int windowSize);
LIBCOURSEWORK_CPU_API void filter_cpu_ctmf  (ImageData* imageData, const unsigned int windowSize);
LIBCOURSEWORK_CPU_API void filter_cpu_hist  (ImageData* imageData, const unsigned int windowSize);
