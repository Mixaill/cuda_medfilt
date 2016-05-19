#pragma once

#include "imageData.h"
#include "libcoursework_cuda_api.h"

LIBCOURSEWORK_CUDA_API void filter_cuda_simple(ImageData* img, unsigned int windowSize);
LIBCOURSEWORK_CUDA_API void filter_cuda_hist(ImageData* img, unsigned int windowSize);