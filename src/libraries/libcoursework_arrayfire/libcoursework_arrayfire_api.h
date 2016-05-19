#pragma once

#ifdef _MSC_VER
  #ifdef LIBCOURSEWORK_ARRAYFIRE_EXPORT
   #define LIBCOURSEWORK_ARRAYFIRE_API __declspec(dllexport) 
  #else
   #define LIBCOURSEWORK_ARRAYFIRE_API __declspec(dllimport) 
  #endif
#else
  #define LIBCOURSEWORK_ARRAYFIRE_API
#endif
