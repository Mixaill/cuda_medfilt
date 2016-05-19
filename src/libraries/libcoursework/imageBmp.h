//imageBmp.h
//
//Written by: Mark Bernard      <mark.bernard@rogers.com>
//            Mikhail Paulyshka <me@mixaill.tk>
//            NetVipeC
#pragma once 

#include <cstdint>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

#include "imageData.h"

class ImageBmp {
public:
    //methods
    static ImageData* Load(const std::string& filePath,bool isColor=true);
    static bool Save(ImageData* imageData, const std::string& filePath);

private:
    static const short BMP_MAGIC_NUMBER_LE = 0x4D42;
    static const short BMP_BITS_PER_PIXEL = 24;
    static const short BMP_BITS_PER_PIXEL_PADDED = 32; //-V112
    static const short BMP_BYTES_PER_PIXEL = 3;
    static const short BMP_BYTES_PER_PIXEL_PADDED = 4; //-V112
    
    #pragma pack(push,1)
    typedef struct tagBitmapFileHeader {
        uint16_t bfType;
        uint32_t bfSize;
        uint16_t bfReserved1;
        uint16_t bfReserved2;
        uint32_t bfOffBits;
    } BitmapFileHeader;

    typedef struct tagBitmapInfoHeader {
        uint32_t biSize;
        int32_t  biWidth;
        int32_t  biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        int32_t  biXPelsPerMeter;
        int32_t  biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    } BitmapInfoHeader;
    #pragma pack(pop)

    static BitmapFileHeader getHeaderBmfh(ImageData* imageData);
    static BitmapInfoHeader getHeaderBmih(ImageData* imageData);
    static unsigned int getPadding(ImageData* imageData);
    static unsigned int getPitch(ImageData* imageData);
};
