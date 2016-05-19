#include "imagePng.h"
#include "lodepng.h"

#include <fstream>

ImageData* ImagePng::Load(const std::string& filePath) {
	
	std::vector<uint8_t> pngdata;
	unsigned error = lodepng::load_file(pngdata, filePath); //load the image file with given filename

	if (error != 0)
		return nullptr;// Не смогли загрузить файл
		//std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

    std::vector<uint8_t> image; //изображение в виде r-g-b-a
    unsigned width, height;     //ширина и высота
	lodepng::State state; //optionally customize this one

    //декодирование
    error = lodepng::decode(image, width, height, state, pngdata);

    //создание объекта ImageData
	ImageData* imageData = new ImageData(width, height);

    //заполнение ImageData
    for (unsigned int i = 0; i < imageData->height; i++)
    {
        for (unsigned int j = 0; j < imageData->width; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                imageData->data_uint8[k][i*imageData->width + j] = image[i * width * 4 + j * 4 + k];
            }
        }
    }
	if (state.info_png.color.colortype == LCT_GREY)
		imageData->toGrayscale();
    return imageData;
}

bool ImagePng::Save(ImageData* imageData, const std::string& filePath) {
    //создание вектора для lodepng
    std::vector<uint8_t> image;
    image.reserve(imageData->width * imageData->height * ImageData::CHANNEL_COUNT_RGBA);

    //заполнение вектора данными
    for (unsigned int i = 0; i < imageData->height; i++)
    {
        for (unsigned int j = 0; j < imageData->width; j++)
        {
            if (imageData->isColor)
            {
                for (int k = 0; k < ImageData::CHANNEL_COUNT_RGBA; k++)
                {
                    image.push_back(imageData->data_uint8[k][i*imageData->width + j]);
                }
            }
            else
            {
                for (int k = 0; k < ImageData::CHANNEL_COUNT_RGB; k++)
                {
                    image.push_back(imageData->data_uint8[3][i*imageData->width + j]);
                }
                image.push_back(255);
            }
        }
    }
    
    //запись в файл

    unsigned error = lodepng::encode(filePath, image, imageData->width, imageData->height);

    return true;
}