#include "cudahelper.h"

#include <cuda_runtime.h>

#include "nvidia_helpers/helper_cuda.h"
#include "nvidia_helpers/helper_string.h"

#include <iostream>

//Initialize CUDA device with highest Gflops value
int cw_cuda_DeviceInit()
{
    int deviceCount;
    checkCudaErrors(cudaGetDeviceCount(&deviceCount));

    if (deviceCount == 0)
    {
        std::cerr << "CUDA error: no devices supporting CUDA." << std::endl;
        exit(EXIT_FAILURE);
    }

    int dev = gpuGetMaxGflopsDeviceId();
    cudaDeviceProp deviceProp;

    checkCudaErrors(cudaSetDevice(dev));
    checkCudaErrors(cudaGetDeviceProperties(&deviceProp, dev));
    printf("GPU Device %d: \"%s\" with compute capability %d.%d\n\n", dev, deviceProp.name, deviceProp.major, deviceProp.minor);

    return dev;
}

bool cw_cuda_printVersionInfo()
{
    int driverVersion, runtimeVersion;
    cudaDriverGetVersion(&driverVersion);
    cudaRuntimeGetVersion(&runtimeVersion);

    printf("  CUDA Driver  Version: %d.%d\n", driverVersion / 1000, (driverVersion % 100) / 10);
    printf("  CUDA Runtime Version: %d.%d\n", runtimeVersion / 1000, (runtimeVersion % 100) / 10);

    // Min spec is SM 1.0 devices
    bool bVal = checkCudaCapabilities(1, 0);
    return bVal;
}