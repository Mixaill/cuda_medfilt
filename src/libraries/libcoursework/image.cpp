#include "image.h"
#include "utils.h"
#include "imageBmp.h"
#include "imagePng.h"

ImageData* Image::Load(const std::string& filepath, bool isColor)
{
    std::string ext = string_get_extension(filepath);
    
    if (ext == "bmp")
    {
        return ImageBmp::Load(filepath,isColor);
    }
    else if (ext == "png")
    {
        return ImagePng::Load(filepath);
    }
    else
    {
        return nullptr;
    }
}

bool Image::Save(ImageData* imageData, const std::string& filepath)
{
    if (imageData == nullptr)
    {
        std::cout << "Error: troubles with image saving" << std::endl;
        exit(1);
    }

    std::string ext = string_get_extension(filepath);
    if (ext == "bmp")
    {
        ImageBmp::Save(imageData,filepath);
    }
    else if (ext == "png")
    {
        ImagePng::Save(imageData, filepath);
    }
    else
    {
        return false;
    }
    return true;
}