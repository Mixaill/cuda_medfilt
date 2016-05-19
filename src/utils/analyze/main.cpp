#include <fstream>
#include <iostream>
#include <string>

#include "config.h"
#include "image.h"
#include "utils.h"
#include "statistics.h"

void help()
{
    std::cout 
        << "coursework_analyze: make image analysis" << std::endl << std::endl
        << "usage: coursework_filter original.bmp noise.bmp filtered.bmp" << std::endl
        << "    *.bmp - input filenames" << std::endl;
}

void analyze(int argc, char* argv[])
{
    std::string filename= std::string(argv[1]);
    filename=filename.substr(0,filename.size()-4);  //-V112
    //load
  
    ImageData* img_in=Image::Load(argv[1]);
    ImageData* img_noise = Image::Load(argv[2]);
    ImageData* img_out = Image::Load(argv[3]);

    ImageData* img_diff;

    //image difference
    img_diff = image_difference(img_in, img_noise);
    Image::Save(img_diff, filename + "_diff_in-noise.bmp");
    delete img_diff;

    img_diff = image_difference(img_in, img_out);
    Image::Save(img_diff, filename + "_diff_in-out.bmp");
    delete img_diff;

    img_diff = image_difference(img_noise, img_out);
    Image::Save(img_diff, filename + "_diff_noise-out.bmp");
    delete img_diff;

    std::ofstream str = std::ofstream(filename +  "_analyze.txt");
    cw_double4 mse = Statistics::MSE(img_in, img_out);
    cw_double4 psnr = Statistics::PSNR(img_in, img_out);
    str << "MSE:" << std::endl
        << "      R:" << mse.r << std::endl
        << "      G:" << mse.g << std::endl
        << "      B:" << mse.b << std::endl
        << "      A:" << mse.a << std::endl
        << "PSNR:" << std::endl
        << "      R:" << psnr.r << std::endl
        << "      G:" << psnr.g << std::endl
        << "      B:" << psnr.b << std::endl
        << "      A:" << psnr.a << std::endl;

    str.flush();
    str.close();
    
    delete img_in;
    delete img_noise;
    delete img_out;
}

int main(int argc, char* argv[])
{
    if(argc<4) //-V112
    {
        help();
    }
    else
    {
        analyze(argc,argv);
    }
    return 0;
}