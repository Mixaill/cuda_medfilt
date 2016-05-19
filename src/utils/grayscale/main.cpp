#include <fstream>
#include <iostream>

#include "image.h"

void toGrayscale(int argc, char* argv[])
{
    std::string filename=std::string(argv[1]);

    ImageData* imageData = Image::Load(filename);
    imageData->toGrayscale();
    Image::Save(imageData, filename.substr(0, filename.size() - 4) + "_noise.bmp"); //-V112

    delete imageData;
}

void help()
{
    std::cout<<"coursework_grayscale: convert image from color to grayscale" <<std::endl <<std::endl
             <<"usage: coursework_grayscale input.bmp"                       <<std::endl
             <<"   input.bmp - input filename"                               <<std::endl;
}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        help();
    }
    else
    {
        toGrayscale(argc,argv);
    }
    return 0;
}