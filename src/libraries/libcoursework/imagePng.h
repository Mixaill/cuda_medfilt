#pragma once

#include <string>
#include "imageData.h"

//Класс для работы с PNG файлами
class ImagePng
{
public:
    //Загрузка изображения из файла
    static ImageData* Load(const std::string& filePath);

    //Запись изображения в файл
    static bool Save(ImageData* imageData, const std::string& filePath);
};
