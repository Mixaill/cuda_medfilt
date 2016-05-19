#include <fstream>
#include <iostream>

#include "config.h"
#include "image.h"
#include "utils.h"

#ifdef  ENABLE_FILTER_CPU
#include "filter_cpu.h"
#endif

#ifdef  ENABLE_FILTER_CUDA
#include "filter_cuda.h"
#endif

void help()
{
    std::cout<<"coursework_filter: filters image"                         << std::endl << std::endl
    <<"usage: coursework_filter input.bmp [filter] [windowSize]" << std::endl
    <<"    input.bmp - input filename"                           << std::endl
    <<"    filter: median filter implementation to use"          << std::endl
    #ifdef ENABLE_FILTER_CPU
    <<"        cpu_simple  : CPU simple median filter"           <<std::endl
    <<"        cpu_ctmf    : CPU constant time median filter"    << std::endl
    #endif
    #ifdef  ENABLE_FILTER_CUDA
    <<"        cuda_simple : CUDA simple median filter"    <<std::endl
    <<"        cuda_hist   : CUDA filter based on histogram" << std::endl
    #endif
    <<"    windowSize: size of median filter window"             <<std::endl;
}

void filter_noise(int argc, char* argv[])
{
    //parse command line
    std::string filename=std::string(argv[1]);

    std::string filter = DEFAULT_FILTER;
    if(argc>2)
    {
        filter=std::string(argv[2]);
    }

    unsigned int windowSize = DEFAULT_WINDOWSIZE;
    if(argc>3)
    {
        windowSize=atoi(argv[3]);
    }

    //load
    ImageData* imageData = Image::Load(filename);
    
    //filter
    if (filter == "cuda_simple")
    {
        #ifdef  ENABLE_FILTER_CUDA
        filter_cuda_simple(imageData, windowSize);
        #endif
    } else if (filter == "cuda_hist") {
        #ifdef  ENABLE_FILTER_CUDA
        filter_cuda_hist(imageData, windowSize);
        #endif
    } else if(filter=="cpu_simple") {
        #ifdef  ENABLE_FILTER_CPU
        filter_cpu_simple(imageData,windowSize);
        #endif
    } else if (filter == "cpu_ctmf"){
        #ifdef  ENABLE_FILTER_CPU
        filter_cpu_ctmf(imageData, windowSize);
        #endif
    }
    else {
        std::cout<<"Incorrect filter name: "<<argv[2]<<std::endl<<std::endl;
        help();
        exit(2);
    }

    //save
    if(string_ends_with(filename,"_noise.bmp"))
    {
        filename=filename.substr(0,filename.size()-10);
    }   
    Image::Save(imageData, filename.substr(0, filename.size() - 4) + "_noise.bmp"); //-V112

    delete imageData;
}

int main(int argc, char* argv[])
{
    if(argc<2)
    {
        help();
    }
    else
    {
        filter_noise(argc,argv);
    }
    return 0;
}