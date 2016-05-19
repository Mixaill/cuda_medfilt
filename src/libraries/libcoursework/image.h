#pragma once

#include "libcoursework_api.h"

#include "imageData.h"
#include <string>

//Класс для загрузки и сохранения графических файлов
class LIBCOURSEWORK_API Image{
public:
    static ImageData* Load(const std::string& filepath,bool isColor=true);
    static bool Save(ImageData* imageData, const std::string& filepatg);
};
