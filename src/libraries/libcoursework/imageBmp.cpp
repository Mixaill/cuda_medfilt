// Written by:   Mikhail Paulyshka <me@mixaill.tk>
// With help of:
//               Mark Bernard     
//               NetVipeC
//               Krzysztof Kachniarz 
//               Sufian Latif 

#include "imageBmp.h"
#include <cmath>

ImageData* ImageBmp::Load(const std::string& filePath,bool isColor) {

    // Открытие файла на чтение
    std::ifstream pFile(filePath, std::ios_base::binary);
    if (!pFile.is_open()) {
        return nullptr;
    }

    //Чтение BitmapFileHeader
    BitmapFileHeader bmfh;
    pFile.read((char*)&bmfh, sizeof(BitmapFileHeader));

    //Проверка магического числа
    if (bmfh.bfType != BMP_MAGIC_NUMBER_LE) {
        pFile.close();
        return nullptr;
    }

    //Чтение  BitmapInfoHeader
    BitmapInfoHeader bmih;
    pFile.read((char*)&bmih, sizeof(BitmapInfoHeader));

    //save the width, height and bits per pixel for external use
    ImageData* imageData = new ImageData(bmih.biWidth, bmih.biHeight);
    imageData->isColor = isColor;

    //FIXME: сейчас поддерживается только 24bpp, добавить 8 и 32
    if (bmih.biBitCount != 24)
    {
        pFile.close();
        return nullptr;
    }

    unsigned int pitch = getPitch(imageData);
    uint8_t* data = new uint8_t[pitch];

    //Чтение изображения, N.B: строки в обратном порядке
	for (int y = imageData->height - 1; y >= 0; y--)
    {
        pFile.read((char*)data, pitch);
        for (int x = 0; x < imageData->width; x ++)
        {
            // Чтение каналов. N.B: каналы в обратном порядке
            if (!imageData->isColor)
            {
                imageData->data_uint8[3][y*imageData->width + x] = data[x * 3];
            }

            imageData->data_uint8[2][y*imageData->width + x] = data[x * 3];
            imageData->data_uint8[1][y*imageData->width + x] = data[x * 3 + 1];
            imageData->data_uint8[0][y*imageData->width + x] = data[x * 3 + 2];

        }
    }

    //Закрытие файла
    pFile.close();

    delete[] data;
    
    return imageData;
}

bool ImageBmp::Save(ImageData* imageData, const std::string& filePath) 
{    
    //Открытие файла на запись
    std::ofstream pFile(filePath, std::ios_base::binary);
    if (!pFile.is_open()) {
        return false;
    }

    //Генерация BMP заголовков
    BitmapFileHeader bmfh = getHeaderBmfh(imageData);
    BitmapInfoHeader bmih = getHeaderBmih(imageData);

    //Запись BMP заголовков
    pFile.write((const char*)&bmfh, sizeof(BitmapFileHeader));
    pFile.write((const char*)&bmih, sizeof(BitmapInfoHeader));
  
    //Готовим изображение
    unsigned int pitch = getPitch(imageData);
    uint8_t* tofile = new uint8_t[imageData->height*pitch]{0};
 
    for (unsigned int y = 0 ; y < imageData->height; y++)
    {
        for (unsigned int x = 0; x < imageData->width; x++ )
        {
            if (imageData->isColor)
            {
                tofile[y*pitch + x*BMP_BYTES_PER_PIXEL + 0] = imageData->data_uint8[2][(imageData->height - 1 - y)*imageData->width + x];
                tofile[y*pitch + x*BMP_BYTES_PER_PIXEL + 1] = imageData->data_uint8[1][(imageData->height - 1 - y)*imageData->width + x];
                tofile[y*pitch + x*BMP_BYTES_PER_PIXEL + 2] = imageData->data_uint8[0][(imageData->height - 1 - y)*imageData->width + x];
            }
            else
            {
                tofile[y*pitch + x*BMP_BYTES_PER_PIXEL + 0] = imageData->data_uint8[3][(imageData->height - 1 - y)*imageData->width + x];
                tofile[y*pitch + x*BMP_BYTES_PER_PIXEL + 1] = imageData->data_uint8[3][(imageData->height - 1 - y)*imageData->width + x];
                tofile[y*pitch + x*BMP_BYTES_PER_PIXEL + 2] = imageData->data_uint8[3][(imageData->height - 1 - y)*imageData->width + x];
            }
        }
    }

    pFile.write((char*)tofile, imageData->height*pitch*sizeof(uint8_t));

    //Закрытие файла
    pFile.close();

    delete[] tofile;

    return true;
}

ImageBmp::BitmapFileHeader ImageBmp::getHeaderBmfh(ImageData* imageData){
    
    BitmapFileHeader bmfh;

    bmfh.bfOffBits = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfType = BMP_MAGIC_NUMBER_LE;
    bmfh.bfSize = ((imageData->width * imageData->height) * BMP_BYTES_PER_PIXEL + bmfh.bfOffBits);

    return bmfh;
}

ImageBmp::BitmapInfoHeader ImageBmp::getHeaderBmih(ImageData* imageData)
{
    BitmapInfoHeader bmih;

    bmih.biSize = sizeof(BitmapInfoHeader);
    bmih.biWidth = imageData->width;
    bmih.biHeight = imageData->height;
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;
    bmih.biCompression = 0;
    bmih.biSizeImage = getPitch(imageData)*imageData->height;
    bmih.biXPelsPerMeter = 0;
    bmih.biYPelsPerMeter = 0;
    bmih.biClrUsed = 0;
    bmih.biClrImportant = 0;

    return bmih;
}

unsigned int ImageBmp::getPitch(ImageData* imageData){
    return ((imageData->width * BMP_BITS_PER_PIXEL ) + (BMP_BITS_PER_PIXEL_PADDED-1))
		    / BMP_BITS_PER_PIXEL_PADDED * BMP_BYTES_PER_PIXEL_PADDED;
}

unsigned int ImageBmp::getPadding(ImageData* imageData){
    return getPitch(imageData) - (imageData->width * BMP_BYTES_PER_PIXEL);
}