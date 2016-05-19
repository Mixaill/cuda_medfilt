#pragma once

#include "libcoursework_cuda_api.h"

LIBCOURSEWORK_CUDA_API int cw_cuda_DeviceInit();

//Print version of CUDA Driver, CUDA Runtime and NPP library
LIBCOURSEWORK_CUDA_API bool cw_cuda_printVersionInfo();