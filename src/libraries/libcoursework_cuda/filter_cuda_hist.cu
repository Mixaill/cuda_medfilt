// 

#include "filter_cuda.h"

#include "config.h"

#include <cstdint>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "nvidia_helpers/helper_cuda.h"

#include "utils.h"
#include "benchmark.h"
#include <iostream>

__global__ void filter_cuda_hist_kernel(const uint8_t* dev_input,
                                              uint8_t* dev_output,
                                        const int width,
                                        const int height,
                                        const int edge_x,
                                        const int edge_y,
                                        const int windowRadius,
                                        const int windowLength)
{
    int x = blockIdx.x*blockDim.x + threadIdx.x;
    int y = blockIdx.y*blockDim.y + threadIdx.y;

    if (x<edge_x || x >= width-edge_x || y<edge_y || y >= height-edge_y)
    {
        return;
    }

    short hist[256];

    //clear hist
    for (int i = 0; i < 256; i++)
    {
        hist[i] = 0;
    }

    //fill hist
    for (int iy = y - windowRadius; iy <= y + windowRadius; iy++)
    {
        for (int ix = x - windowRadius; ix <= x + windowRadius; ix++)
        {
            hist[dev_input[iy*width + ix]]++;
        }
    }

    int cpt = 0;
    uint8_t ic = 0;

    //parse hist
    for (ic = 0; ic < 256; ic++)
    {
        cpt += hist[ic];

        if (cpt > windowLength/2)
        {
            break;
        }
    }

    dev_output[y*width + x] = ic;
}

void filter_cuda_hist_channel(uint8_t* data, const int w, const int h, const int windowSize)
{
    const int size = w * h * sizeof(uint8_t);
    int windowRadius = windowSize / 2;

    //memory allocation
    uint8_t* device_input_data = nullptr;
    uint8_t* device_output_data = nullptr;

    checkCudaErrors(cudaMalloc((void **)& device_input_data, size));
    checkCudaErrors(cudaMalloc((void **)& device_output_data, size));
    checkCudaErrors(cudaMemcpy(device_input_data, data, size, cudaMemcpyHostToDevice));

    dim3 dimBlock(CUDA_TILE_SIZE_X, CUDA_TILE_SIZE_Y,1);
    dim3 dimGrid(w / CUDA_TILE_SIZE_X +1 , h / CUDA_TILE_SIZE_Y +1,1);

    filter_cuda_hist_kernel << <dimGrid, dimBlock >> >(device_input_data, device_output_data, w, h, windowSize/2,windowSize/2, windowRadius,windowSize*windowSize);
   
    checkCudaErrors(cudaPeekAtLastError());

    checkCudaErrors(cudaMemcpy(data, device_output_data, size, cudaMemcpyDeviceToHost));

    checkCudaErrors(cudaFree(device_input_data));
    checkCudaErrors(cudaFree(device_output_data));
  
}

void filter_cuda_hist(ImageData* img, unsigned int windowSize)
{


    ImageData* img_extended = image_extend(img,windowSize);

    if (img->isColor)
    {
        for (unsigned int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {          
            filter_cuda_hist_channel(img_extended->data_uint8[i],img_extended->width, img_extended->height, windowSize);
        }
    }
    else
    {
        filter_cuda_hist_channel(img_extended->data_uint8[3], img_extended->width, img_extended->height, windowSize);
    }

    image_placeExtendedBack(img,img_extended,windowSize);

    delete img_extended;
}