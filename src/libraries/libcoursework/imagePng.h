#pragma once

#include <string>
#include "imageData.h"

//����� ��� ������ � PNG �������
class ImagePng
{
public:
    //�������� ����������� �� �����
    static ImageData* Load(const std::string& filePath);

    //������ ����������� � ����
    static bool Save(ImageData* imageData, const std::string& filePath);
};
