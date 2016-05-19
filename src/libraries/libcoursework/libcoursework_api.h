#pragma once

#ifdef _MSC_VER
  #ifdef LIBCOURSEWORK_EXPORT
   #define LIBCOURSEWORK_API __declspec(dllexport) 
  #else
   #define LIBCOURSEWORK_API __declspec(dllimport) 
  #endif
#else
  #define LIBCOURSEWORK_API
#endif
