#pragma once 

#include <string>

#include "imageData.h"
#include "libcoursework_api.h"

LIBCOURSEWORK_API ImageData* image_extend(ImageData* image,const int windowSize);
LIBCOURSEWORK_API void image_placeExtendedBack(ImageData* image, ImageData* image_extended, const int windowSize);

LIBCOURSEWORK_API ImageData* image_difference(ImageData* a, ImageData* b);

LIBCOURSEWORK_API bool string_ends_with(std::string const & value, std::string const & ending);
LIBCOURSEWORK_API std::string string_get_extension(std::string const & str);
