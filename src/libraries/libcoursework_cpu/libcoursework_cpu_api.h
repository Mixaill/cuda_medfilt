#pragma once

#ifdef _MSC_VER
  #ifdef LIBCOURSEWORK_CPU_EXPORT
   #define LIBCOURSEWORK_CPU_API __declspec(dllexport) 
  #else
   #define LIBCOURSEWORK_CPU_API __declspec(dllimport) 
  #endif
#else
  #define LIBCOURSEWORK_CPU_API
#endif
