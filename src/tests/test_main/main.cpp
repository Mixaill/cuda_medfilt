#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

#include "benchmark.h"
#include "config.h"
#include "image.h"
#include "utils.h"
#include "noise.h"

#ifdef  ENABLE_FILTER_CPU
#include "filter_cpu.h"
#endif

#ifdef  ENABLE_FILTER_CUDA
#include "filter_cuda.h"
#include "cudahelper.h"
#endif

#ifdef ENABLE_FILTER_ARRAYFIRE
#include "filter_arrayfire.h"
#endif

void test_cpu_ctmf(const std::string& filename, int window, int repeats, std::ofstream& str)
{
    std::string filter_name = "cpu_ctmf";

    ImageData* imageData = Image::Load(filename,false);

    Benchmark::Start();

    for (int i = 0; i < repeats; i++)
    {
        filter_cpu_ctmf(imageData, window);
    }

    //output
    double r = Benchmark::Stop() / repeats;
    double rs = imageData->width*imageData->height / (r / 1000)/1000000;
    std::string image_size = std::to_string(imageData->width) + "x" + std::to_string(imageData->height);

    std::cout << "| "<<std::setw(11) << filter_name << " | " << std::setw(10) << image_size << " | " <<std::setw(11)<< window << " | " << std::setw(10) << r << " | " << std::setw(13) << rs << " |" << std::endl;
    str << filter_name <<";" << image_size << ";" << window << ";" << r << ";" << rs << std::endl;

    //save
    Image::Save(imageData, "out/"
        + std::to_string(imageData->width) + "_filtered_"
        + std::to_string(window) + "_"
        + filter_name+".bmp");
}

void test_cpu_hist(const std::string& filename, int window, int repeats, std::ofstream& str)
{
    std::string filter_name = "cpu_hist";

    ImageData* imageData = Image::Load(filename,false);

    Benchmark::Start();

    for (int i = 0; i < repeats; i++)
    {
        filter_cpu_hist(imageData, window);
    }

    //output
    double r = Benchmark::Stop() / repeats;
    double rs = imageData->width*imageData->height / (r / 1000)/1000000;
    std::string image_size = std::to_string(imageData->width) + "x" + std::to_string(imageData->height);

    std::cout << "| "<<std::setw(11) << filter_name << " | " << std::setw(10) << image_size << " | " <<std::setw(11)<< window << " | " << std::setw(10) << r << " | " << std::setw(13) << rs << " |" << std::endl;
    str << filter_name <<";" << image_size << ";" << window << ";" << r << ";" << rs << std::endl;

    //save
    Image::Save(imageData, "out/"
                           + std::to_string(imageData->width) + "_filtered_"
                           + std::to_string(window) + "_"
                           + filter_name+".bmp");
}


void test_cpu_simple(const std::string& filename, int window, int repeats, std::ofstream& str)
{
    std::string filter_name = "cpu_simple";

    ImageData* imageData = Image::Load(filename,false);

    Benchmark::Start();

    for (int i = 0; i < repeats; i++)
    {
        filter_cpu_simple(imageData, window);
    }

    //output
    double r = Benchmark::Stop() / repeats;
    double rs = imageData->width*imageData->height / (r / 1000) / 1000000;
    std::string image_size = std::to_string(imageData->width) + "x" + std::to_string(imageData->height);

    std::cout << "| " << std::setw(11) << filter_name << " | " << std::setw(10) << image_size << " | " << std::setw(11) << window << " | " << std::setw(10) << r << " | " << std::setw(13) << rs << " |" << std::endl;
    str << filter_name << ";" << image_size << ";" << window << ";" << r << ";" << rs << std::endl;

    //save
    Image::Save(imageData, "out/"
        + std::to_string(imageData->width) + "_filtered_"
        + std::to_string(window) + "_"
        + filter_name + ".bmp");
}

void test_cuda_hist(const std::string& filename, int window, int repeats, std::ofstream& str)
{
    std::string filter_name = "cuda_hist";

    ImageData* imageData = Image::Load(filename,false);

    Benchmark::Start();

    for (int i = 0; i < repeats; i++)
    {
        filter_cuda_hist(imageData, window);
    }

    //output
    double r = Benchmark::Stop() / repeats;
    double rs = imageData->width*imageData->height / (r / 1000) / 1000000;
    std::string image_size = std::to_string(imageData->width) + "x" + std::to_string(imageData->height);

    std::cout << "| " << std::setw(11) << filter_name << " | " << std::setw(10) << image_size << " | " << std::setw(11) << window << " | " << std::setw(10) << r << " | " << std::setw(13) << rs << " |" << std::endl;
    str << filter_name << ";" << image_size << ";" << window << ";" << r << ";" << rs << std::endl;

    //save
    Image::Save(imageData, "out/"
        + std::to_string(imageData->width) + "_filtered_"
        + std::to_string(window) + "_"
        + filter_name + ".bmp");
}

void test_cuda_simple(const std::string& filename, int window, int repeats, std::ofstream& str)
{
    std::string filter_name = "cuda_simple";

    ImageData* imageData = Image::Load(filename,false);

    Benchmark::Start();

    for (int i = 0; i < repeats; i++)
    {
        filter_cuda_simple(imageData, window);
    }

    //output
    double r = Benchmark::Stop() / repeats;
    double rs = imageData->width*imageData->height / (r / 1000) / 1000000;
    std::string image_size = std::to_string(imageData->width) + "x" + std::to_string(imageData->height);

    std::cout << "| " << std::setw(11) << filter_name << " | " << std::setw(10) << image_size << " | " << std::setw(11) << window << " | " << std::setw(10) << r << " | " << std::setw(13) << rs << " |" << std::endl;
    str << filter_name << ";" << image_size << ";" << window << ";" << r << ";" << rs << std::endl;

    //save
    Image::Save(imageData, "out/"
        + std::to_string(imageData->width) + "_filtered_"
        + std::to_string(window) + "_"
        + filter_name + ".bmp");
}

void test_cuda_arrayfire(const std::string& filename, int window, int repeats, std::ofstream& str)
{
    std::string filter_name = "arrayfire";

    ImageData* imageData = Image::Load(filename,false);

    Benchmark::Start();

    for (int i = 0; i < repeats; i++)
    {
        filter_arrayfire_medfilt(imageData, window);
    }

    //output
    double r = Benchmark::Stop() / repeats;
    double rs = imageData->width*imageData->height / (r / 1000) / 1000000;
    std::string image_size = std::to_string(imageData->width) + "x" + std::to_string(imageData->height);

    std::cout << "| " << std::setw(11) << filter_name << " | " << std::setw(10) << image_size << " | " << std::setw(11) << window << " | " << std::setw(10) << r << " | " << std::setw(13) << rs << " |" << std::endl;
    str << filter_name << ";" << image_size << ";" << window << ";" << r << ";" << rs << std::endl;

    //save
    Image::Save(imageData, "out/"
        + std::to_string(imageData->width) + "_filtered_"
        + std::to_string(window) + "_"
        + filter_name + ".bmp");
}


void test_lena(int size_start, int size_end,int window_start,int window_end, int repeats)
{

    std::cout << "TEST--->LENA IMAGE" << std::endl;
    std::cout << "/=====================================================================\\" << std::endl;
    std::cout << "| Filter name | Image size | Window size | Time, msec | Speed, MPix/s |" << std::endl;
    std::ofstream logfile = std::ofstream("out/time_lena.txt");
    logfile << "filter name;image size; window size; time, msec; speed, mpix/s"<<std::endl;
    for (unsigned int size = size_start; size <= size_end; size *= 2)
    {
        std::string filename = "assets/lena_" + std::to_string(size) + ".bmp";

        ImageData* imageData = Image::Load(filename);
        if (imageData == nullptr)
        {
            std::cout << "Error: File " << filename << " is not found" << std::endl;
            continue;
        }
        imageData->toGrayscale();

        Image::Save(imageData, "out/lena_" + std::to_string(size) + "_grayscale.bmp");

        Noise::AddSaltAndPepper(imageData);
        std::string file_noised = "out/lena_" + std::to_string(size) + "_noise.bmp";
        Image::Save(imageData, file_noised);

        for (unsigned int w = window_start; w <= window_end; w += 2)
        {
            std::cout << "|---------------------------------------------------------------------|" << std::endl;

            #ifdef  ENABLE_FILTER_CPU
            test_cpu_ctmf(file_noised, w, repeats, logfile);
            test_cpu_hist(file_noised, w, repeats, logfile);
            test_cpu_simple(file_noised, w, repeats, logfile);
            #endif

            #ifdef  ENABLE_FILTER_CUDA
            test_cuda_hist(file_noised, w, repeats, logfile);
            test_cuda_simple(file_noised, w, repeats, logfile);
            #endif

            #ifdef  ENABLE_FILTER_ARRAYFIRE
            test_cuda_arrayfire(file_noised, w, repeats, logfile);
            #endif
        }
    }

    logfile.flush();
    logfile.close();
}

int main()
{
    //info about CUDA device 
    #ifdef ENABLE_FILTER_CUDA
    cw_cuda_DeviceInit();
    #endif

    #ifdef ENABLE_FILTER_ARRAYFIRE
    filter_arrayfire_init();
    #endif

    test_lena(128,1024,3,9,5);
    std::cout << std::endl<<std::endl<< "Press any key to exit..." << std::endl;
    std::cin.get();
    return 0;
}
