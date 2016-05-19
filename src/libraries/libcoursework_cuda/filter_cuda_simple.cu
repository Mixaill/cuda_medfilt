// JackOLantern, PD
// Joseph Loser & Duncan Hill, MIT

#include "filter_cuda.h"

#include "config.h"

#include <cstdint>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "nvidia_helpers/helper_cuda.h"

//Kernels
__global__
void filter_cuda_simple_kernel(const uint8_t * data_dev_input, uint8_t * data_dev_output, const unsigned int width, const unsigned int height, const unsigned int windowSize) {
    //window radius & length
    const int windowRadius = windowSize / 2;
    const int windowLength = windowSize * windowSize;
        
    //current coordinates
    int x = blockIdx.x*blockDim.x + threadIdx.x;
    int y = blockIdx.y*blockDim.y + threadIdx.y;
    if (x > (width - 1) || y > (height - 1))
    {
        return;
    }

    uint8_t* device_window = new uint8_t[windowLength];

    // Populate the filter_array
    int filter_array_index = 0;
    for (int iy = y - windowRadius; iy <= y + windowRadius; iy++)
    {
        if (iy<0 || iy>(height - 1))
        {
            continue;
        }

        for (int ix = x - windowRadius; ix <= x + windowRadius; ix++)
        {
            if (ix<0 || ix>(width - 1))
            {
                continue;
            }

            device_window[filter_array_index++] = data_dev_input[iy*width + ix];
        }
    }

    //sort filter array
    uint8_t tmp;
    for (int i = 0; i <= windowLength / 2; i++)
    {
        for (int j = i + 1; j < windowLength; j++)
        {
            if (device_window[i] > device_window[j])
            { 
                tmp = device_window[i];
                device_window[i] = device_window[j];
                device_window[j] = tmp;
            }
        }
    }

    data_dev_output[y*width + x] = device_window[windowLength / 2];

    delete[] device_window;
}

void filter_cuda_simple_channel(uint8_t* data, const int w, const int h, const int windowSize)
{
    const int size = w * h * sizeof(uint8_t);

    //memory allocation
    uint8_t* device_input_data = nullptr;
    uint8_t* device_output_data = nullptr;

    checkCudaErrors(cudaMalloc((void **)& device_input_data, size));
    checkCudaErrors(cudaMalloc((void **)& device_output_data, size));
    checkCudaErrors(cudaMemcpy(device_input_data, data, size, cudaMemcpyHostToDevice));

    dim3 dimBlock(CUDA_TILE_SIZE_X, CUDA_TILE_SIZE_Y, 1);
    dim3 dimGrid(w / CUDA_TILE_SIZE_X+1, h / CUDA_TILE_SIZE_Y+1, 1);

    filter_cuda_simple_kernel << <dimGrid, dimBlock >> >(device_input_data, device_output_data, w, h, windowSize);

    checkCudaErrors(cudaPeekAtLastError());

    checkCudaErrors(cudaMemcpy(data, device_output_data, size, cudaMemcpyDeviceToHost));

    checkCudaErrors(cudaFree(device_input_data));
    checkCudaErrors(cudaFree(device_output_data));

}

void filter_cuda_simple(ImageData* img, unsigned int windowSize) {

    if (img->isColor)
    {
        for (unsigned int i = 0; i < ImageData::CHANNEL_COUNT_RGB; i++)
        {
            filter_cuda_simple_channel(img->data_uint8[i], img->width, img->height, windowSize);
        }
    }
    else
    {
        filter_cuda_simple_channel(img->data_uint8[3], img->width, img->height, windowSize);
    }
}