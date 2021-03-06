#pragma once

#define DEFAULT_GAUSSIAN_STDDEV 30

#define DEFAULT_SALTPEPPER_PROBABILITY_LO 0.05
#define DEFAULT_SALTPEPPER_PROBABILITY_HI 0.95

#define DEFAULT_FILTER "cpu_simple"
#define DEFAULT_WINDOWSIZE 3

#define ENABLE_FILTER_CPU
#define ENABLE_FILTER_CUDA

#define ENABLE_FEATURE_OPENMP

#define CUDA_TILE_SIZE_X 16
#define CUDA_TILE_SIZE_Y 16