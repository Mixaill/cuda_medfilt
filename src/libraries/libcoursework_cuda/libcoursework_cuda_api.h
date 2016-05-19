#pragma once

#ifdef _MSC_VER
  #ifdef LIBCOURSEWORK_CUDA_EXPORT
   #define LIBCOURSEWORK_CUDA_API __declspec(dllexport) 
  #else
   #define LIBCOURSEWORK_CUDA_API __declspec(dllimport) 
  #endif
#else
  #define LIBCOURSEWORK_CUDA_API
#endif
